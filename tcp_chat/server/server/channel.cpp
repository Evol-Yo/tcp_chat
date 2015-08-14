#include "server.h"


channel::channel(user_manage *user_manage)
	:_id(-1), _user_manage(user_manage)
{
	_online_users = new client_weak_ptr[100];
}

channel::~channel(){
	delete []_online_users;
}

int channel::put_data(const data_info &data)
{
	return (_input.push(data));
}

int channel::get_data(data_info &data)
{
	return _input.pop(data);
}

int channel::add_online_user(int conn, const client_info &cli)
{
	client_shared_ptr cli_sp = find_user(cli._name);
	if(!cli_sp){
		return -1;
	}

	//以conn作为该在线用户的索引
	client_shared_ptr cli_tmp = _online_users[conn].lock();
	if(cli_tmp){
		return -1;
	}
	_online_users[conn] = cli_sp;

	//向其他用户广播该上线用户(只需要广播其id号即可)
	msg_info msg;
	data_info data;
	data._id = cli_tmp->_id;
	data._cmd = CMD_ONLINE;
	data._datalen = 0;
	data_info::val_to_str(data, msg._msg);
	msg._head._msglen = msg._msg.length();
	msg._head._id = _id;
	broadcast(msg);

	return 0;
}

typename channel::client_shared_ptr channel::find_user(const string &name)
{
	client_array_t::iterator iter = _clients.begin();
	while(iter != _clients.end()){
 		client_shared_ptr cli_sp = iter->lock();
		if(cli_sp->_name == name){
			return cli_sp;
		}
		iter++;
	}
	return client_shared_ptr();
}

int channel::register_user(const client_info &cli)
{
	if(_clients.size() > MAX_USERS){
		return -1;
	}

	client_shared_ptr cli_sp = _user_manage->register_user(cli);
	if(!cli_sp){
		return -1;
	}

	cli_sp->_channel_id = _clients.size()-1;
	cli_sp->_name = cli._name;
	_clients.push_back(cli_sp);

	return 0;	
}

void channel::get_user_list(msg_info &msg)
{
	client_array_t::iterator iter = _clients.begin();
	data_info data;
	while(iter != _clients.end()){
		string str;
		char buf[5];
		bzero(buf, sizeof(buf));
		client_shared_ptr cli_sp = iter->lock();
		data._id = cli_sp->_id;
		data._cmd = CMD_USER_LIST;
		data._data = cli_sp->_name;
		data._datalen = data._data.length();
		data_info::val_to_str(data, str);
		//需要加入长度信息（4个字符）
		sprintf(buf, "%4lu", str.length());
		msg._msg += buf;
		msg._msg += str;

		iter++;
	}
	msg._head._id = _id;
	msg._head._msglen = msg._msg.length();
}

int channel::run()
{

	int nfds = 0;
	struct epoll_event events[MAX_EVENTS];
	while(1){
		//设置epoll_wait的定时器是因为主线程发送的信号可能丢失
		nfds = epoll_wait(_epollfd, events, MAX_EVENTS, 100);
		if(nfds == -1){
			if(errno != EINTR){
				log_error("epoll_wait error");
				return -1;
			}
		}
		int i = 0;
		//我们先将接收到的数据放入消息队列中
		for(; i < nfds; i++){
			int conn = events[i].data.fd;	
			if(events[i].events & EPOLLIN){
				msg_info msg;
				data_info data;
				server::recv_msg(conn, msg);
				data_info::str_to_val(msg._msg, data);
				put_data(data);
			}else{
				log_error("events wrong");
			}
		}
		//循环处理消息队列中的内容
		while(1){
			data_info data;
			if(get_data(data) < 0){		//取数据
				break;
			}
			proc_data(data);	//处理数据
		}
	}

	return 0;
}
		
int channel::init(int id, const string &name)
{
	_id = id;
	_name = name;
	_epollfd = epoll_create(MAX_EVENTS);
	if(_epollfd == -1){
		log_error("epoll create error");
		return -1;
	}
	return 0;
}

int channel::proc_data(data_info &data)
{
	int conn = 0;
	int cmd = data._cmd;
	msg_info msg;
	switch(cmd){
		case CMD_MSG:
			data_info::val_to_str(data, msg._msg);
			msg._head._id = _id;
			msg._head._msglen = msg._msg.length();
			broadcast(msg);
			break;
		case CMD_REGISTER:
			{
				int i = 4;
				char buf[5];
				client_info cli;
				//获取连接的套结字,server模块会将套接字描述符添加到data._data的尾部(占4个字符)
				bzero(buf, sizeof(buf));
				for(; i < 4; i++){
					buf[i] = data._data[data._data.size() - 4 + i];
					data._data[data._data.size() - 4 + i] = 0;
				}
				conn = atoi(buf);

				client_info::str_to_val(data._data, cli);

				//将该用户注册到好友列表中
				if(register_user(cli) < 0){
					log_error("register_user error");
					return -1;
				}
				log_message("register_user success, user:%s", data._data.c_str());
				//在这里我们应该关闭该套接字，以使客户端重新登陆，为了方便，我们直接
				//将其添加到在线列表中
				//close(conn)
				if(add_online_user(conn, cli) < 0){
					close(conn);
					return -1;
				}
				

				//将其加入到epoll事件中
				if(add_event(conn) < 0){
					close(conn);
					return -1;
				}
				get_user_list(msg);
				server::send_msg(conn, msg);
			}
			break;
		case CMD_ONLINE:
			{
				client_info cli;
				int i = 4;
				char buf[5];
				bzero(buf, sizeof(buf));
				for(; i < 4; i++){
					buf[i] = data._data[data._data.size() - 4 + i];
					data._data[data._data.size() - 4 + i] = 0;
				}
				conn = atoi(buf);

				client_info::str_to_val(data._data, cli);
				if(add_online_user(conn, cli) < 0){
					close(conn);
					return -1;
				}
				//将其加入到epoll事件中
				if(add_event(conn) < 0){
					close(conn);
					return -1;
				}
				get_user_list(msg);
				server::send_msg(conn, msg);
			}
			break;
		default:
			break;
	}

	return 0;
}

int channel::broadcast(const msg_info &msg)
{
	client_weak_ptr cli_wp = _online_users[0];
	int i = 0;
	for( ; i < MAX_ONLINE; ){
		if(!cli_wp.expired()){
			client_shared_ptr cli_sp = cli_wp.lock();
			if(cli_sp){
				if(server::send_msg(i, msg) < 0){
					log_error("send msg error, sock: %d, msg: %s", i, msg._msg.c_str());
				}
			}
		}
		cli_wp = _online_users[++i];
	}

	return 0;
}

int channel::add_event(int conn)
{
	struct epoll_event ev;
	ev.events = EPOLLIN;
    ev.data.fd = conn;
    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, conn, &ev) == -1) {
		log_error("add epoll_event error");
		return -1;
    }
	return 0;
}
