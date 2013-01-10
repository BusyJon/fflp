#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

#include "plugin.h"
#include "header.h"
#include "network.h"
#include "str.h"
#include "print.h"
#include "mppcdecode.h"

#define GRE 47	//GRE Protocol

plugin_t plugin_pptp_mppc();
void init_pptp_mppc();
int disect_pptp_mppc(struct ip_header *ip,int len);

plugin_t plugin_pptp_mppc(){
    plugin_t pptp_mppc={
	.init=init_pptp_mppc,
	.disect=disect_pptp_mppc,
	.description="pptp mppc watch",
    };
    return pptp_mppc;
}

void init_pptp_mppc(){}

int disect_pptp_mppc(struct ip_header *iph, int len){
    struct gre_header *grehdr;
    u_char *gredata, *pppdata;
    /* IP */
    int ip_data_len = ntohs(iph->tot_len) - iph->ihl*4;
    if(ip_data_len == 0)
	return 0;
    if(iph->protocol == GRE)
    {
	/* GRE */
	grehdr = (struct gre_header*)((u_char *)iph + iph->ihl*4); 
	int gre_hdr_len=get_gre_hdr_len((struct gre_header*)grehdr,ip_data_len);
	gredata = (u_char*)grehdr + gre_hdr_len; 
	int gre_data_len = ip_data_len - gre_hdr_len; 
	if(gre_data_len == 0)
	    return 0;
	/* PPP MPPC */
	if(gredata[0] != 0xfd)
	    return 0;
	int ppp_hdr_len=1; // ppp_mppe
	pppdata = gredata + ppp_hdr_len;//not right
	int ppp_data_len=gre_data_len - ppp_hdr_len;
	if(ppp_data_len <= 0)
	    return 0;
	//print_data(pppdata, ppp_data_len, 16);
	/* MPPCD */
	u_long buflen=1400;
	u_char *buf=(u_char*)malloc(buflen);
	u_char *buf_bak=buf;
	memset(buf,0,buflen);
	mppcd_ppp_data(pppdata, ppp_data_len, buf, &buflen);
	/* IP in PPP */
	/*
	int ret=find_ip_in_mppcd(&buf, &buflen);
	if(ret == 0){
	    DEBUG("PPP MPPCD: it's not ip pkg in ppp mppcd\n");
	    goto freebuf;
	}
	*/
	buf+=2;
	buflen-=2;
	if(buflen >= 20)
	    print_ip_all((struct ip_header*)buf);

freebuf:
	free(buf_bak);
	return 1;
    }
    return 1;
}


