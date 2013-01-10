/*
 * ip_print.h
 *
 *  Created on: 2012-3-2
 *      Author: ocean
 */

#ifndef IP_PRINT_H_
#define IP_PRINT_H_


#include <cstring>
#include "../base/plugin.h"
#include "../lp.h"


struct ip_header{
	uchar version[1]; //0
	uchar header_length[1]; //0
	uchar tos[1]; //服务质量 1
	uchar packet_length[2]; //总长度 2,3
	uchar identify[2]; //标识 4,5
	uchar off[2]; //偏移 6,7
	uchar ttl[1]; //生存时间 8
	uchar protocol[1]; //协议类型 9
	uchar checksum[2]; //校验和 10,11
	uchar source_ip[4]; //源ip 12,13,14,15
	uchar destination_ip[4]; //目的ip 16,17,18,19
};


extern "C"{
	plugin register_proto_ip();
}


#endif /* IP_PRINT_H_ */
