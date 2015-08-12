#ifndef _USER_MANAGE_H
#define _USER_MANAGE_H

#include <string>
#include <memory>
#include <vector>
#include "util.h"
 
class user_manage {

	public:
		typedef shared_ptr<client_info>		client_shared_ptr;
		typedef weak_ptr<client_info>		client_weak_ptr;

		client_shared_ptr register_user(const client_info &cli);

	private:
		enum{MAX_USERS = 100};
		typedef vector<client_shared_ptr> 		client_array_t;

		client_array_t		_clients;
};

 
#endif //USER_MANAGE_H

