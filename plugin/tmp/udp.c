/*
 * udp.cpp
 *
 *  Created on: 2012-3-8
 *      Author: ocean
 */

/*
 * udp.cpp
 *
 *  Created on: 2012-3-8
 *      Author: ocean
 */

#include "udp.h"

static bool identify_proto_udp(const uchar *pkt, int length);
static void* init_udp_header(const uchar *pkt, int length);
static void destroy_udp_header(void *_header);
static void print_udp_header(void* _header);
static int get_udp_header_length();


plugin register_proto_udp(){
	plugin udp;
	udp.proto_name="udp";
	udp.lower_proto_name="ip";
	udp.identify=identify_proto_udp;
	udp.init_header=init_udp_header;
	udp.print_header=print_udp_header;
	udp.destroy_header=destroy_udp_header;
	udp.get_header_length=get_udp_header_length;
	return udp;
}


bool identify_proto_udp(const uchar *pkt, int length){
	uchar protocol=17;
	if(memcmp(pkt+9,&protocol,1)==0){
		printf(">> identifed by udp >> ip header 9th protocol :");
		print_hex(pkt+9);
		printf("\n");
		return true;
	}
	return false;
}



void* init_udp_header(const uchar *pkt, int length){
	udp_header *header=new udp_header;
	memcpy(header->source_port,pkt,2);
	memcpy(header->destination_port,pkt+2,2);
	memcpy(header->length,pkt+4,4);
	memcpy(header->checksum,pkt+6,2);
	return header;
}


void destroy_udp_header(void *_header){
	delete _header;
}
void print_udp_header(void* _header){
}
int get_udp_header_length(){
	return 8;
}






