#include "channel.h"
#include "util.h"

server::server(user_manage *user_mg)
	:_sock(0), _user_manage(user_mg)
{
	//初始化channel
	channel chan(user_mg);
	chan._id = 0;
	chan._name = "bit";
	_channels.push_back(chan);
}

int server::init(const string &ip, unsigned short port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		log_error("create socket error");
		return -1;
	}
	_sock = sock;
	int reuse = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	struct sockaddr_in seraddr;	
	bzero(&seraddr, sizeof(seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(port);
	seraddr.sin_addr.s_addr = inet_addr(ip.c_str());

	if(bind(sock, (struct sockaddr*)&seraddr, sizeof(seraddr)) < 0){
		log_error("bind socket error");
		close(sock);
		_sock = 0;
		return -2;
	}

	if(listen(sock, 5) < 0){
		log_error("listen socket error");
		close(sock);
		_sock = 0;
		return -3;
	}

	return 0;
}

int server::run()
{	
	while(1){

		msg_info msg;
		data_info data;

		int conn = accept(_sock, NULL, NULL);
		log_message("accept success");
		if(recv_msg(conn, msg) < 0){
			return -1;
		}

		if(proc_msg(conn, msg) < 0){
			return -1;
		}

		_channels[msg._head._id].run();
	}

	return 0;
}

int server::recv_msg(int sock, msg_info &msg)
{
	char buf[BUF_SIZE];
	bzero(buf, sizeof(buf));

	//先读取长度字段
	ssize_t size = 0;
	do{
		int len = recv(sock, buf + size, 4-size, 0);
		if(len < 0){
			log_error("recv error");
			return len;
		}
		size += len;
	} while(size == 4);

	int msglen = atoi(buf);

	//再读取消息字段
	size = 0;
	bzero(buf, sizeof(buf));
	do{
		int len = recv(sock, buf + size, msglen - size + 4, 0);
		if(len < 0){
			log_error("recv error");
			return len;
		}
		size += len;
	} while(size == msglen);

	if(buf[0] != '%' || buf[1] != '#'){
		log_error("message head error!");
		return -1;
	}
	if(buf[msglen + 1] != '#' || buf[msglen + 2] != '%'){
		log_error("message tail error!");
		return -1;
	}
	buf[msglen + 1] = '\0';
	buf[msglen + 2] = '\0';

	//反序列化
	msg_info::str_to_val(buf+2, msg);

	return msglen;
}


int server::send_msg(int sock, const msg_info &msg)
{
	string str;
	msg_info::val_to_str(msg, str);
	int msglen = str.length();

	char buf[BUF_SIZE + 5];
	bzero(buf , sizeof(buf));
	//加上长度字段,和特殊标志，用以检查错误
	sprintf(buf, "%4d", msglen);
	sprintf(buf+4, "%%#");
	sprintf(buf+6, "%s", str.c_str());
	sprintf(buf + msglen + 1, "#%%");

	return send(sock, buf, msglen+8, 0);
}

int server::proc_msg(int conn, const msg_info &msg)
{
	int cmd = msg._head._cmd;
	if(cmd < 0){
		return -1;
	}
	
	if(cmd == CMD_REGISTER){
		client_info cli;
		client_info::str_to_val(msg._msg, cli);
		cli._valid = true;
		if(_channels[msg._head._id].register_user(cli) < 0){
			log_error("register_user error");
			return -1;
		}
		log_message("register_user success, user:%s", msg._msg.c_str());
		//在这里我们应该关闭连接，等待用户重新登陆
		//close(conn)
	}

	//cmd == CMD_ONLINE
	client_info cli;
	client_info::str_to_val(msg._msg, cli);
	if(_channels[msg._head._id].add_online_user(conn, cli) < 0){
		log_error("add_online_user error");
		return -1;
	}
	log_message("add_online_user success, user: %s", msg._msg.c_str());
	msg_info user_list;
	_channels[msg._head._id].send_user_list(user_list);
	
	//向该用户发送好友列表
	send_msg(conn, user_list);

	return 0;
}
