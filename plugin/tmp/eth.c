/*
 * lp_print.c
 *
 *  Created on: 2012-3-2
 *      Author: ocean
 */

#include "libnet.h"
#include "eth.h"

static void print_eth_header(void *_header);
static void* init_eth_header(const uchar* pkt, int length);
static void destroy_eth_header(void *_header);
static int get_eth_header_length();

plugin register_proto_eth() {
    plugin eth;
    eth.proto_name = "eth";
    eth.lower_proto_name = "";
    eth.identify = NULL;
    eth.init_header = init_eth_header;
    eth.destroy_header = destroy_eth_header;
    eth.print_header = print_eth_header;
    eth.get_header_length=get_eth_header_length;
    return eth;
}

void* init_eth_header(const uchar* pkt, int length) {
    eth_header *header = new eth_header;
    memcpy(header->destination, pkt, 6);
    memcpy(header->source, pkt + 6, 6);
    memcpy(header->type, pkt + 12, 2);
    return (void*) header;
}

void destroy_eth_header(void *_header) {
    delete (eth*)_header;
}

void print_eth_header(void *_header) {
    eth_header *header = (eth_header*) _header;
    printf("destination:");
    for (int i = 0; i < 5; i++) {
	print_hex((header->destination) + i);
	printf(":");
    }
    print_hex((header->destination) + 5);

    printf("\nsource:");
    for (int i = 0; i < 5; i++) {
	print_hex((header->source) + i);
	printf(":");
    }
    print_hex((header->source) + 5);

    printf("\ntype:");
    print_hex(header->type, 2);
    printf("\n");
}

int  get_eth_header_length(){
    return 14;
}
