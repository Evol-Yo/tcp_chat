#ifndef _CHANNEL_H
#define _CHANNEL_H

#include <string>
#include <vector>
#include <memory>
#include "data_pool.h"
#include "my_json.h"
#include "server.h"
#include "user_manage.h"
#include "util.h"
#include "sys/epoll.h"

class server;

class channel {

	friend class server;
	public:
		typedef data_pool<data_info> 	data_pool_t;	
		typedef shared_ptr<client_info> client_shared_ptr;
		typedef weak_ptr<client_info>	client_weak_ptr;

		channel(user_manage *user_manage = NULL);
		~channel();

		int put_data(const data_info &data);
		int get_data(data_info &data);
		
		int add_online_user(int conn, const client_info &cli);

		int register_user(const client_info &cli);
		void get_user_list(msg_info &msg);
		int run();
		int init(int id, const string &name);

	private:
		//将消息队列中的消息经过处理，打包成msg，调用broadcast广播数据
		int proc_data(data_info &data);
		int broadcast(const msg_info &msg);
		int add_event(int conn);
		client_shared_ptr find_user(const string &name);

	private:
		typedef int						id_type;
		typedef vector<client_weak_ptr>	client_array_t;
		typedef client_weak_ptr*		online_user_array_t;
		
		enum{MAX_USERS = 10, 
			MAX_ONLINE = 100, 
			MAX_EVENTS = 100};

		//用于epoll
		int					_epollfd;

		id_type			_id; 		//channel id
		string			_name;		//channel name
		client_array_t	_clients;
		data_pool_t		_input;
		online_user_array_t _online_users;
		user_manage		*_user_manage;
};


#endif //CHANNEL_H

