/*
 * http.cpp
 *
 *  Created on: 2012-3-11
 *      Author: ocean
 */


#include "http.h"


static bool identify_proto_http(const uchar *pkt, int length);
static void* init_http_header(const uchar *pkt, int length);
static void destroy_http_header(void *_header);
static void print_http_header(void* _header);


plugin register_proto_http(){
	plugin http;
	http.proto_name="http";
	http.lower_proto_name="tcp";
	http.identify=identify_proto_http;
	http.init_header=init_http_header;
	http.print_header=print_http_header;
	http.destroy_header=destroy_http_header;
	return http;
}


bool identify_proto_http(const uchar *pkt, int length){
	const uchar *pkt_t=pkt+32;
	int len=length-32;
	const uchar *id_str[3]={(uchar*)"GET",(uchar*)"POST",(uchar*)"HTTP"};
	if(memcmp(pkt_t,id_str[0],3)==0
		|| memcmp(pkt_t,id_str[1],4)==0
		|| memcmp(pkt_t,id_str[2],4)==0){
		printf(">> identifed by http");
		return true;
	}
	return false;
}


void* init_http_header(const uchar *pkt, int length){
	http_header *header=new http_header;
	return header;
}


void destroy_http_header(void *_header){
	delete _header;
}
void print_http_header(void* _header){
}



