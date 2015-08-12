#include "server.h"

int channel::put_msg(const msg_info &msg)
{
	return _input.push(msg);
}

void channel::get_msg(msg_info &msg)
{
	msg = _input.pop();
}

int channel::add_online_user(int conn, const client_info &cli)
{
	client_shared_ptr cli_sp = find_user(cli._name);
	if(!cli_sp){
		return -1;
	}

	client_shared_ptr cli_tmp = _online_users[conn].lock();
	if(cli_tmp || !cli_tmp->_valid || cli_tmp->_online){
		return -1;
	}
	_online_users[conn] = cli_sp;
	cli_sp->_online = true;

	//向其他用户广播该上线用户(只需要广播其id号即可)
	msg_info msg;
	char buf[4];
	bzero(buf, sizeof(buf));
	sprintf(buf, "%4d", cli_sp->_id);
	msg._msg = buf;
	msg._head._msglen = msg._msg.length();
	msg._head._id = _id;
	msg._head._cmd = CMD_ONLINE;
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
		return -2;
	}

	cli_sp->_channel_id = _clients.size()-1;
	cli_sp->_name = cli._name;
	_clients.push_back(cli_sp);

	return 0;	
}

void channel::send_user_list(msg_info &msg)
{
	client_array_t::iterator iter = _clients.begin();
	data_info data;
	while(iter != _clients.end()){

		string str;
		char buf[4];
		bzero(buf, sizeof(buf));
		client_shared_ptr cli_sp = iter->lock();
		data._id = cli_sp->_id;
		data._data = cli_sp->_name;
		data._datalen = data._data.length();
		data_info::val_to_str(data, str);
		sprintf(buf, "%4lu", str.length());
		//添加是否在线信息
		if(cli_sp->_online){
			sprintf(buf+4, "%2d", 1);
		}else{
			sprintf(buf+4, "%2d", 2);
		}
		msg._msg += buf;
		msg._msg += str;

		iter++;
	}
	msg._head._id = _id;
	msg._head._cmd = CMD_USER_LIST;
	msg._head._msglen = msg._msg.length();
}

int channel::run()
{

	while(1){
		msg_info msg;
		get_msg(msg);
		proc_msg(msg);
	}

	return 0;
}

int channel::proc_msg(msg_info &msg)
{
	data_info data;
	int cmd = -1;
	if(data_info::str_to_val(msg._msg, data) < 0){
		return -1;
	}
	cmd = data._cmd;
	switch(cmd){
		case CMD_MSG:
			broadcast(msg);
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
			if(cli_sp->_online){
				if(server::send_msg(i, msg) < 0){
					log_error("send msg error, sock: %d, msg: %s", i, msg._msg.c_str());
				}
			}
		}
		cli_wp = _online_users[++i];
	}

	return 0;
}
