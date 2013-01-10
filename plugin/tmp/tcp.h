/*
 * tcp.h
 *
 *  Created on: 2012-3-8
 *      Author: ocean
 */

#ifndef TCP_H_
#define TCP_H_

#include <cstring>
#include "../base/plugin.h"
#include "../lp.h"

struct tcp_header{
	uchar source_port[2];//0,1
	uchar destination_port[2];//2,3
	uchar sequence_number[4];//4,5,6,7
	uchar ack_number[4];//8,9,10,11
	uchar header_length[1];//4个bit 12
	uchar flag[2];//1个字节+4个bit 12,13
	uchar window_size[2];//14,15
	uchar checksum[2]; //16/17
	uchar urgent_point[2];//19,20
	uchar option[12];
};
extern "C"{
	plugin register_proto_tcp();
}
#endif /* TCP_H_ */
