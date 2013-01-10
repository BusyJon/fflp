/*
 * http.h
 *
 *  Created on: 2012-3-11
 *      Author: ocean
 */


#ifndef HTTP_H_
#define HTTP_H_

#include <cstring>
#include "../base/plugin.h"
#include "../lp.h"
#include <pcre++.h>
using namespace pcrepp;

struct http_header{

};

extern "C"{
	plugin register_proto_http();
}


#endif /* HTTP_H_ */
