#include "user_manage.h"

typename user_manage::client_shared_ptr user_manage::register_user(const client_info &cli)
{
	if(_clients.size() > MAX_USERS){
		return client_shared_ptr();
	}
	client_shared_ptr cli_sp(new client_info);
	*cli_sp = cli;
	cli_sp->_id = _clients.size();
	cli_sp->_valid = true;
	_clients.push_back(cli_sp);
	return cli_sp;
}
