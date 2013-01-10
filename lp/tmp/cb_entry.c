
#include "cb_entry.h"

#if 0
void ip_packet_entry(struct iphdr *data, int len){
    list_head_t *node=ip_pi_head.next;
    plugin_info_t *pi;
    plugin_t p;
    int ret=PASS;
    while(ret == PASS && node != &ip_pi_head){
	pi=container_of(node, plugin_info_t, list);
	p=pi->plugin;
	p.init();
	ret=p.disect(data,len);
	node=node->next;
    }
}

void udp_packet_entry(struct tuple4 *addr, char *buf, int len, struct iphdr *iph){
    plugin_info_t *pi;
    plugin_t p;
    list_head_t *node=udp_pi_head.next;
    int ret=PASS;
    while(ret == PASS && node != &udp_pi_head){
	pi=container_of(node, plugin_info_t, list);
	p=pi->plugin;
	p.init();
	ret=p.disect(addr, buf, len, iph);
	node=node->next;
    }
}

void tcp_packet_entry(struct tcp_stream *a_tcp, void **param){
    list_head_t *node=tcp_pi_head.next;
    plugin_info_t *pi;
    plugin_t p;
    int ret=PASS;
    while(ret == PASS && node != &ip_pi_head){
	pi=container_of(node, plugin_info_t, list);
	p=pi->plugin;
	p.init();
	ret=p.disect(a_tcp, param);
	node=node->next;
    }
}

#endif


#if 0
/* XXX
 * 废弃，如果让插件系统操作tcp_stream，则插件失去更多机会去操作tcp流
 * 将更多tcp_stream直接给插件，让插件决定读取多少数据和缓存的控制
 * 而不是有插件系统保留得到所有数据后再给插件
 */

char tcp_client_data[65536];
char tcp_server_data[65536];
char tcp_client_urgdata[65536];
char tcp_server_urgdata[65536];
u_int32_t tcp_client_len=0;
u_int32_t tcp_server_len=0;
u_int32_t tcp_client_urglen=0;
u_int32_t tcp_server_urglen=0;

void tcp_packet_entry(struct tcp_stream *ns,void **param){
    struct half_stream *client=&ns->client;
    struct half_stream *server=&ns->server;
    switch(ns->nids_state){
	case NIDS_JUST_EST:
	    DEBUG("===========================================>");
	    DEBUG("establish\n");
	    client->collect++;
	    client->collect_urg++;
	    server->collect++;
	    server->collect_urg++;
	    break;
	case NIDS_DATA:
	    DEBUG("===========================================>");
	    DEBUG("client data len: offset(%d) + new(%d) = all(%d)", 
		    client->offset, client->count_new, client->count);
	    DEBUG("server data len: offset(%d) + new(%d) = all(%d)",
		    server->offset, server->count_new, server->count);
	    if(client->count_new){
		//防止缓冲区溢出，这种情况极少，暂时这样处理
		if(tcp_client_len + client->count_new > 65536){
		    DEBUG("===========================================>");
		    DEBUG("too much data !!!!");
		    memcpy(tcp_client_data+tcp_client_len, client->data, 65536-tcp_client_len);
		    tcp_client_len=65536;
		    goto exiting;
		}
		memcpy(tcp_client_data+client->offset, client->data, client->count_new);
		tcp_client_len+=client->count_new;
	    }
	    if(server->count_new){
		//防止缓冲区溢出，这种情况极少，暂时这样处理
		if(tcp_server_len + server->count_new > 65536){
		    DEBUG("===========================================>");
		    DEBUG("too much data !!!!");
		    memcpy(tcp_server_data+tcp_server_len, server->data, 65536-tcp_server_len);
		    tcp_server_len=65536;
		    goto exiting;
		}
		memcpy(tcp_server_data+server->offset, server->data, server->count_new);
		tcp_server_len+=server->count_new;
	    }
	    break;
	case NIDS_RESET:
	    DEBUG("===========================================>");
	    DEBUG("reseting");
	    goto exiting; 
	case NIDS_CLOSE:
	    DEBUG("===========================================>");
	    DEBUG("closing");
	    goto exiting;
	case NIDS_EXITING:
	    DEBUG("===========================================>");
	    DEBUG("exiting");
exiting:
#ifdef ENABLE_DEBUG 
	    printf("%s\n",tcp_client_data);
	    //print_data((const u_char *)tcp_client_data,tcp_client_len,16);
	    printf("---------------------------\n");
	    printf("%s\n",tcp_server_data);
	    //print_data((const u_char *)tcp_server_data,tcp_server_len,16);
	    break;
#endif
	    /* XXX
	     * tcp segment stream结束时的处理块
	     */
	    {
		list_head_t *node=tcp_pi_head.next;
		plugin_info_t *pi;
		plugin_t p;
		int ret=PASS;
		//client data operate callback
		while(ret == PASS && node != &tcp_pi_head){
		    pi=container_of(node, plugin_info_t, list);
		    p=pi->plugin;
		    //p.init();
		    ret=p.disect(ns->addr, tcp_client_data, tcp_client_len);
		    node=node->next;
		}
		//server data operate callback
		node=tcp_pi_head.next;
		ret=PASS;
		while(ret == PASS && node != &tcp_pi_head){
		    pi=container_of(node, plugin_info_t, list);
		    p=pi->plugin;
		    //p.init();
		    ret=p.disect(ns->addr, tcp_server_data, tcp_server_len);
		    node=node->next;
		}
	    }
	    break;
	default:
	    break;
    }
}

#endif
