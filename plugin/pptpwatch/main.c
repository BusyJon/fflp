
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "plugin.h"
#include "header.h"
#include "network.h"
#include "str.h"
#include "debug.h"
#include "util.h"
#include "print.h"

plugin_t plugin_pptp();
void init_pptp();
int pptp_watch();

plugin_t plugin_pptp(){
    plugin_t pptp={
	.init=init_pptp,
	.disect=pptp_watch,
	.description="disect common pptp packet(! mppc && ! mppe )",
    };
    return pptp;
}

void init_pptp(){
}

#define GRE_HEADER_LEN 12
#define PPP_HEADER_LEN 1
#define GRE 47
#define TCP 6
#define UDP 17

#if 0
int pptp_watch(struct ip *iph, int pktlen)
{
    char * grehdr,*gredata,* pppdata;
    int ip_data_len,gre_data_len,ppp_data_len,tcp_data_len;
    IP_INFO_TYPE data_body;	
    ip_data_len = ntohs(iph->ip_len) - iph->ip_hl*4;
    grehdr = (char *)iph + iph->ip_hl*4; // gre stream
    gredata = grehdr + 12; //len bug
    if(iph->ip_p == GRE)
    {
	gre_data_len = gre_header_check(grehdr,ip_data_len);
	if(!gre_data_len)	
	    return 0;
	if((u_char)(gredata[0]) == 0x80 && (u_char)(gredata[1]) == 0xfd)
	    printf("find ccp packet\n");
	ppp_data_len = ppp_header_check(gredata,ip_data_len-GRE_HEADER_LEN);
	if(!ppp_data_len)	
	    return 0;
	pppdata = gredata + 1;
	tcp_data_len = tcpip_decode(pppdata,ip_data_len-GRE_HEADER_LEN-PPP_HEADER_LEN,&data_body);
	if(tcp_data_len)	
	    write_gre_log(thread_id, iph,&data_body);
	return 0;
    }
    else 
	return 0;
}
#endif


inline int http_decode(char * data, int datalen, http_info_t *hi);

int pptp_watch(struct ip_header *iph, int len){
    char *grehdr, *gredata, *pppdata;
    int ip_data_len, gre_data_len, ppp_data_len;
    http_info_t hi;
    hi.http_type=-1;
    //print_data((u_char*)iph, len, 16);    

    ip_data_len = ntohs(iph->tot_len) - iph->ihl*4;
    grehdr = (char *)iph + iph->ihl*4;

    gredata = grehdr + GRE_HEADER_LEN;
    gre_data_len=ip_data_len-GRE_HEADER_LEN;
    if(iph->protocol == GRE)
    {
	int ret=check_gre_ppp_header(grehdr,ip_data_len);
	eq_exit(ret,0);
	ret=check_ppp_ip_header(gredata,gre_data_len);
	eq_exit(ret,0);
	pppdata = gredata + PPP_HEADER_LEN;
	ppp_data_len=gre_data_len - PPP_HEADER_LEN;
	http_decode(pppdata, ppp_data_len, &hi);
	if(hi.http_type != -1){
	    if(hi.http_type == HTTP_TYPE_GET){
		FPRINTF("###PPTP HTTP GET  %s url(%s%s)\n",adres(&hi.addr),hi.host,hi.url);
	    }
	    if(hi.http_type == HTTP_TYPE_POST){
		FPRINTF("###PPTP HTTP POST %s url(%s%s)\n",adres(&hi.addr),hi.host,hi.url);
	    }
	    if(hi.http_type == HTTP_TYPE_HEAD){
		FPRINTF("###PPTP HTTP HEAD %s url(%s%s)\n",adres(&hi.addr),hi.host,hi.url);
	    }
	    if(hi.http_type == HTTP_TYPE_RESP){
		FPRINTF("###PPTP HTTP PESP %s resp(%s)\n",adres(&hi.addr),hi.resp_code);
	    }
	}
    }
    return 0;
}

inline int http_decode(char * data, int datalen, http_info_t *hi)
{
    struct ip_header *iph;
    u_short ip_pkg_len;
    struct tcp_header *tcph;
    u_short ip_data_len;
    //skill, the last struct should be char* type
    char *tcpdata;
    int tcp_data_len;

    hi->http_type=-1;
    iph = (struct ip_header*)data;
    ip_pkg_len=datalen;
    check_ip_header(iph,ip_pkg_len);
    hi->addr.saddr = iph->saddr;
    hi->addr.daddr = iph->daddr;

    if(iph->protocol == TCP)
    {
	tcph = (struct tcp_header *)((char *)iph + iph->ihl*4);
	ip_data_len = ip_pkg_len - iph->ihl*4;
	tcpdata = (char *)tcph + tcph->doff * 4;
	tcp_data_len = ip_data_len - tcph->doff*4;
	hi->addr.source = ntohs(tcph->source);
	hi->addr.dest = ntohs(tcph->dest);
	memset(hi->host, 0, sizeof(hi->host));
	memset(hi->url, 0, sizeof(hi->url));
	//pick up request method and resp code
	if(tcp_data_len > 0)
	{
	    if(strncasecmp((char*)tcpdata,"GET",3)==0)
		hi->http_type = HTTP_TYPE_GET;
	    else if(strncasecmp((char*)tcpdata,"HEAD ",5)==0)
		hi->http_type = HTTP_TYPE_HEAD;
	    else if(strncasecmp((char*)tcpdata,"POST ",5)==0)
		hi->http_type = HTTP_TYPE_POST;
	    else if(strncasecmp((char*)tcpdata,"HTTP/1.",6)==0)
		hi->http_type = HTTP_TYPE_RESP;
	    else
		hi->http_type = HTTP_TYPE_OTHERS;
	}
	else{
	    return 0;
	}
	//Pick up host and url
	if(hi->http_type < HTTP_TYPE_RESP && 
	    hi->http_type > HTTP_TYPE_OTHERS)
	{
	    char *url_start=NULL, *url_end=NULL, *host_start=NULL, *host_end=NULL;
	    url_start = strstr((const char*)tcpdata, "/");
	    if(url_start && (url_start - tcpdata < tcp_data_len))
	    {
		url_end = strcasestr((const char*)url_start, "HTTP/1.");
	    }
	    if(url_start && url_end && (url_end - tcpdata < tcp_data_len))
	    {
		int len = MIN((url_end - url_start), MAX_BUFF_LEN);
		STRNCPY(hi->url, url_start, len);
	    }
	    host_start = strcasestr((const char*)tcpdata, "HOST: ");
	    if(host_start && (host_start - tcpdata < tcp_data_len))
	    {
		host_end = strcasestr((const char*)host_start, "\r\n");
	    }
	    if(host_start && host_end && (host_end - tcpdata < tcp_data_len))
	    {
		int len = MIN(host_end - host_start - 6, MAX_BUFF_LEN);
		strncpy(hi->host, host_start + 6, len);
	    }
	}
	if(hi->http_type == HTTP_TYPE_RESP)
	{
	    char *resp_code=strstr(tcpdata," ");
	    STRNCPY(hi->resp_code+1,resp_code,3);
	}
    }
    return 0;
}
