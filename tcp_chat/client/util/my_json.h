#ifndef _MY_JSON_H
#define _MY_JSON_H
 
#include <json/json.h>
#include <string>

using namespace std;

class my_json {
	public:
		static void val_to_str(Json::Value &val, string &str){
#ifdef _FAST_JSON
			Json::FastWriter writer;
			str = writer.write(val);
#else
			Json::StyledWriter writer;
			str = writer.write(val);
#endif
		}

		static int str_to_val(const string &str, Json::Value &val){
			Json::Reader reader;
			return reader.parse(str.c_str(), val, false);
		}
};

 
#endif //MY_JSON_H

