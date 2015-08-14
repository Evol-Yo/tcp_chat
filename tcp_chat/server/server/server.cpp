#include "channel.h"
#include "util.h"

server::server(user_manage *user_mg)
	:_sock(0)
{
	//初始化channel
	_channels = new channel[MAX_CHANNELS];
	_thread = new pthread_t[MAX_CHANNELS];
	int i = 0;
	for(; i < MAX_CHANNELS; i++){
		_channels[i]._user_manage = user_mg;
		_channels[i].init(i, "");
		_thread[i] = -1;
	}
}

server::~server()
{
	delete []_channels;	
	delete []_thread;
}

void *server::channel_run(void *chan)
{
	int ret = ((channel*)chan)->run();
	return (void*)ret;
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

void sig_handler(int signo)
{
	//我们什么都不用做，仅仅用来唤醒channle线程
}

int server::run()
{	
	//当有新用户上线时，我们需要唤醒channel线程
	signal(SIGUSR1, sig_handler);
	pthread_create(&_thread[0], NULL, channel_run, &_channels[0]);

	while(1){

		msg_info msg;
		data_info data;

		int conn = accept(_sock, NULL, NULL);
		log_message("accept success");
		//接收用户信息
		if(recv_msg(conn, msg) < 0){
			return -1;
		}
		int id = msg._head._id;
		data_info::str_to_val(msg._msg, data);
		_channels[id].put_data(data);
		pthread_kill(_thread[id], SIGUSR1);
	}

	return 0;
}

int server::recv_msg(int sock, msg_info &msg)
{
	char buf[BUF_SIZE + 13];
	bzero(buf, sizeof(buf));

	//先读取id字段和长度字段
	ssize_t size = 0;
	do{
		int len = recv(sock, buf + size, 10-size, 0);
		if(len < 0){
			log_error("recv error");
			return len;
		}
		size += len;
	} while(size < 10);

	if(buf[0] != '%' || buf[1] != '#'){
		log_error("recv error");
		//我们应该尝试恢复数据(找到下一个以%#开头的字符序列)， 为了测试，我们直接退出
		return -1;
	}
	int msglen = atoi(buf + 6);
	bzero(buf + 6, 4);
	int id = atoi(buf+2);
	bzero(buf, 6);

	//再读取消息字段
	size = 0;
	do{
		int len = recv(sock, buf + size, msglen - size + 2, 0);
		if(len < 0){
			log_error("recv error");
			return len;
		}
		size += len;
	} while(size < msglen + 2);

	if(buf[msglen] != '%' || buf[msglen+1] != '#'){
		log_error("recv error!");
		return -1;
	}
	buf[msglen] = '\0';
	buf[msglen + 1] = '\0';

	msg._head._id = id;
	msg._head._msglen = msglen;
	msg._msg = buf;

	return msglen;
}


int server::send_msg(int sock, const msg_info &msg)
{
	string str;
	char buf[BUF_SIZE + 13];
	bzero(buf , sizeof(buf));
	//加上长度字段,和特殊标志，用以检查错误
	sprintf(buf, "%%#");
	sprintf(buf + 2, "%4d", msg._head._id);
	sprintf(buf+6, "%4d", msg._head._msglen);
	sprintf(buf+10, "%s", msg._msg.c_str());
	sprintf(buf+10+msg._msg.length(), "%%#");

	return send(sock, buf, msg._head._msglen+12, 0);
}
