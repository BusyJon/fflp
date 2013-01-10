
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <nids.h>

#include "plugin.h"
#include "header.h"
#include "network.h"
#include "str.h"

plugin_t plugin_http();
void init_http();
int http_watch();

plugin_t plugin_http(){
    plugin_t http={
	.init=init_http,
	.disect=http_watch,
	.description="http detector",
    };
    return http;
}

//插件私有变量
int head,get,post;
int resp;
#define MAX_URL_LEN 512
char url[MAX_URL_LEN];

//插件初始化时被执行
void init_http(){
    head=get=post=0;
    resp=0;
    memset(url, 0, MAX_URL_LEN);
}

int http_watch(struct tcp_stream *a_tcp, void **ai){
    if(a_tcp->nids_state == NIDS_JUST_EST){
	if(a_tcp->addr.dest == 80){
#if 0
	    addr_info_t *ai=(addr_info_t*)malloc(sizeof(addr_info_t));
	    init_addr_info(&a_tcp->addr,ai);
#endif
	    a_tcp->client.collect++;
	    a_tcp->server.collect++;
	    //fprintf(stdout, "###ESTABLISH: %s:%d <-> %s:%d\n", );
	    return 0;
	}
    }

    if(a_tcp->nids_state == NIDS_DATA){
	if(a_tcp->client.count_new <= 0 && a_tcp->server.count_new <= 0) 
	    return 0;
	struct half_stream *client=&a_tcp->client;
	struct half_stream *server=&a_tcp->server;
	if(server->count_new){
	    char *url_begin,*url_end,*domain_begin,*domain_end;
	    //get url
	    domain_begin=strstr(server->data, "Host:");
	    if(domain_begin == NULL){ 
		//nids_discard(a_tcp, 0);
		return 0;
	    }
	    domain_end=strstr(domain_begin,"\r\n");
	    if(domain_end == NULL){	
		//nids_discard(a_tcp, 0);
		return 0;
	    }
	    domain_begin+=6;
	    //get domain
	    url_begin=strstr(server->data,"/");
	    url_end=strstr(url_begin," ");
	    STRNCPY(url, domain_begin, domain_end-domain_begin);
	    STRNCATM(url, url_begin, url_end-url_begin, MAX_URL_LEN-strlen(url));
	    //get action type
	    if(strncmp(server->data,"GET ",4) == 0){
		get++;
		fprintf(stdout, "###HTTP GET: %s url(%s)\n",adres2(&a_tcp->addr," -> "), url);
	    }
	    if(strncmp(server->data,"HEAD ",5) == 0){
		head++;
		fprintf(stdout, "###HTTP HEAD: %s url(%s)\n",adres2(&a_tcp->addr, " -> "), url);
	    }
	    if(strncmp(server->data,"POST ",5) == 0){
		post++;
		fprintf(stdout, "###HTTP POST: %s url(%s)\n",adres2(&a_tcp->addr, " -> "), url);
	    }
	}
	if(client->count_new){
	    if(strncmp(client->data, "HTTP", 4) == 0){
		char resp_code[4];
		STRNCPY(resp_code, client->data+9, 3);
		fprintf(stdout, "###HTTP RESP: %s resp(%s)\n", adres2(&a_tcp->addr, " <- "),resp_code);
		//针对http 1.0 在同一连接中只有一个请求
		//而在http 1.1 在同一连接中会有很多请求
		//a_tcp->client.collect--;
		//a_tcp->server.collect--;
	    }
	}
	return 0;
    }
    if(a_tcp->nids_state == NIDS_CLOSE ||
	    a_tcp->nids_state == NIDS_RESET||
	    a_tcp->nids_state == NIDS_EXITING){
	return 0;
    }
    return 0;
}
