#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <nids.h>

#include "plugin.h"
#include "header.h"
#include "network.h"

plugin_t plugin_udp();
void init_udp();
int udp_watch();

plugin_t plugin_udp(){
    plugin_t udp={
	.init=init_udp,
	.disect=udp_watch,
	.description="udp_detector",
    };
    return udp;
}

void init_udp(){
}

int udp_watch(struct tuple4 *addr, char *buf, int len, struct ip_header *iph){
    struct udp_header *udph;
    udph = (struct udp_header *)((char *)iph + iph->ihl*4);
    int dest=ntohs(udph->dest);
    if(dest == 37)
	fprintf(stdout,"###TIME\t");
    else if(dest == 53)
	fprintf(stdout,"###DNS\t");
    else if(dest == 69)
	fprintf(stdout,"###DNS\t");
    else if(dest == 111)
	fprintf(stdout,"###RPC\t");
    else if(dest == 161)
	fprintf(stdout,"###SNMP\t");
    else
	fprintf(stdout,"###UDP\t");
    fprintf(stdout,"%s:%d  \t",(char *)int_ntoa(iph->saddr), ntohs(udph->source));
    fprintf(stdout,"%s:%d  \t",(char *)int_ntoa(iph->daddr), ntohs(udph->dest));
    fprintf(stdout,"len(%d)\n",ntohs(udph->len));
    return 0;
}
