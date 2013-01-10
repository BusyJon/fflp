#include "ip.h"

static bool identify_proto_ip(const uchar *pkt, int length);
static void* init_ip_header(const uchar *pkt, int length);
static void destroy_ip_header(void *_header);
static void print_ip_header(void* _header);
static int get_ip_header_length();

plugin register_proto_ip() {
	plugin ip;
	ip.proto_name = "ip";
	ip.lower_proto_name = "eth";
	ip.identify = identify_proto_ip;
	ip.init_header = init_ip_header;
	ip.destroy_header = destroy_ip_header;
	ip.print_header = print_ip_header;
	ip.get_header_length = get_ip_header_length;
	return ip;
}

bool identify_proto_ip(const uchar *pkt, int length) {
	uchar type[2] = { 8, 0 };
	if (memcmp(pkt + 12, type, 2) == 0){
		printf(">> identifed by ip >> eth header 12th protocol :");
		print_hex(pkt + 12, 2);
		printf("\n");
		return true;
	}
	return false;
}

void* init_ip_header(const uchar *pkt, int length) {
	ip_header *header = new ip_header;
	uchar version = pkt[0] >> 4;
	uchar header_length = pkt[0] & 0x0f;
	memcpy(header->version, &version, 1);
	memcpy(header->header_length, &header_length, 1);
	memcpy(header->tos, pkt + 1, 1);
	memcpy(header->packet_length, pkt + 2, 2);
	memcpy(header->identify, pkt + 4, 2);
	memcpy(header->off, pkt + 6, 2);
	memcpy(header->ttl, pkt + 8, 1);
	memcpy(header->protocol, pkt + 9, 1);
	memcpy(header->checksum, pkt + 10, 2);
	memcpy(header->source_ip, pkt + 12, 4);
	memcpy(header->destination_ip, pkt + 16, 4);
	return (void*) header;
}

void destroy_ip_header(void *_header) {
	delete _header;
}

void print_ip_header(void *_header) {
	ip_header *header = (ip_header*) _header;
//	printf("version:");
//	print_hex(header->version);
//	printf("\nheader_length:");
//	print_hex(header->header_length);
//	printf("\ntos:");
//	print_hex(header->tos);
//	printf("\npacket_length:");
//	print_hex(header->packet_length,2);
//	printf("\nidentify:");
//	print_hex(header->identify,2);
//	printf("\noff:");
//	print_hex(header->off,2);
//	printf("\nttl:");
//	print_hex(header->ttl);
//	printf("\nprotocol:");
//	print_hex(header->protocol);
//	printf("\nchecksum:");
//	print_hex(header->checksum,2);
//	printf("\nsource_ip:");
//	print_hex(header->source_ip,4);
//	printf("\ndestination_ip:");
//	print_hex(header->destination_ip,4);
//	printf("-------------------------------->>\n");
}

int get_ip_header_length() {
	return 20;
}
