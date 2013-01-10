/*
 * eth.h
 *
 *  Created on: 2012-3-8
 *      Author: ocean
 */

#ifndef ETH_H_
#define ETH_H_

#include "../base/plugin.h"
#include "../lp.h"

struct eth_header{
	uchar destination[6];
	uchar source[6];
	uchar type[2];
};

extern "C"{
	plugin register_proto_eth();
}


#endif /* ETH_H_ */
