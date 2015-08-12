#include "server.h"
#include "util.h"


int main()
{
	user_manage user_manage;
	server server(&user_manage);

	if(server.init("127.0.0.1", 49999) < 0){
		log_error("server init error");
		return 1;
	}
	if(server.run() < 0){
		log_error("server run error");
		return 2;
	}

	return 0;
}
