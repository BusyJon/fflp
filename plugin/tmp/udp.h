/*
 * udp.h
 *
 *  Created on: 2012-3-8
 *      Author: ocean
 */

#ifndef UDP_H_
#define UDP_H_

#include <cstring>
#include "../base/plugin.h"
#include "../lp.h"

struct udp_header{
	uchar source_port[2];//0,1
	uchar destination_port[2];//2,3
	uchar length[2];//4,5
	uchar checksum[2];//6,7
};

extern "C"{
	plugin register_proto_udp();
}

#endif /* UDP_H_ */
