
#ifndef CB_ENTRY_H_
#define CB_ENTRY_H_

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <nids.h>
#include <netinet/ip.h>

#include "plugin.h"

#include "proto_header.h"
#include "klist.h"
#include "print.h"
#include "debug.h"

void ip_packet_entry(struct iphdr *data, int len);
void udp_packet_entry(struct tuple4 *addr, char *buf, int len, struct iphdr *iph);
void tcp_packet_entry(struct tcp_stream *ns,void **param);

#endif
