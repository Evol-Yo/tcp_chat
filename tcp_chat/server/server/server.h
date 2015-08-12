#ifndef _SERVER_H_
#define _SERVER_H_

#include <iostream>
#include <strings.h>
#include <map>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <array>
#include <memory>

#include "data_pool.h"
#include "my_json.h"
#include "util.h"
#include "channel.h"
#include "user_manage.h"

class channel;

class server : public my_json 
{
	friend class channel;
	public:
		server(user_manage *user_mg);

		~server(){}

		int init(const string &ip, unsigned short port);
		int run();
		static int recv_msg(int sock, msg_info &msg);
		static int send_msg(int sock, const msg_info &msg);

	private:
		int proc_msg(int conn, const msg_info &msg);
		static int str_to_msginfo(const string& str, msg_info &msg);
		static int msginfo_to_str(msg_info &msg, const string& str);

	private:
		typedef vector<channel>		channel_array_t;
		enum{MAX_CHANNELS = 4};
		
		int					_sock;
		user_manage			*_user_manage;
		channel_array_t		_channels;
};


#endif
