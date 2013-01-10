/*
 * tcp.cpp
 *
 *  Created on: 2012-3-8
 *      Author: ocean
 */

#include "tcp.h"

static bool identify_proto_tcp(const uchar *pkt, int length);
static void* init_tcp_header(const uchar *pkt, int length);
static void destroy_tcp_header(void *_header);
static void print_tcp_header(void* _header);
static int get_tcp_header_length();

plugin register_proto_tcp() {
	plugin tcp;
	tcp.proto_name = "tcp";
	tcp.lower_proto_name = "ip";
	tcp.identify = identify_proto_tcp;
	tcp.init_header = init_tcp_header;
	tcp.print_header = print_tcp_header;
	tcp.destroy_header = destroy_tcp_header;
	tcp.get_header_length = get_tcp_header_length;
	return tcp;
}

bool identify_proto_tcp(const uchar *pkt, int length) {
	uchar protocol = 6;
	if (memcmp(pkt + 9, &protocol, 1) == 0) {
		printf(">> identifed by tcp >> ip header 9th protocol:");
		print_hex(pkt + 9);
		printf("\n");
		return true;
	}
	return false;
}

void* init_tcp_header(const uchar *pkt, int length) {
	tcp_header *header = new tcp_header;
	memcpy(header->source_port, pkt, 2);
	memcpy(header->destination_port, pkt + 2, 2);
	memcpy(header->sequence_number, pkt + 4, 4);
	memcpy(header->ack_number, pkt + 8, 4);
	uchar header_length = pkt[12] >> 4;
	memcpy(header->header_length, &header_length, 1);
	*header->flag = 0; //第一个字节为0
	memcpy(header->flag + 1, pkt + 13, 1);
	memcpy(header->window_size, pkt + 14, 2);
	memcpy(header->checksum, pkt + 16, 2);
	memcpy(header->urgent_point, pkt + 18, 2);
	memcpy(header->option, pkt + 20, 12);
	return header;
}

void destroy_tcp_header(void *_header) {
	delete _header;
}
void print_tcp_header(void* _header) {

}
int get_tcp_header_length() {
	return 32;
}
