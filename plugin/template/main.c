
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "plugin.h"
#include "header.h"
#include "network.h"
#include "str.h"


/****************************
 * 定义一个返回plugin_t类型的函数
 * 包含init，disect，description三个量，供插件系统注册时使用
 * init函数在插件被注册到系统中去时，被调用，整个系统运作周期内仅被调用一次
 * disect函数是协议处理函数，入口不同，则形参不同
 * discription为插件描述字串，给出插件处理的说明
 *  *************************/

plugin_t plugin_temp();
void init_temp();
int IP_ENTRY();
int TCP_ENTRY();
int UDP_ENTRY();

plugin_t plugin_temp(){
    plugin_t temp={
	.init=init_temp,
	.disect=IP_ENTRY,//TCP_ENTRY, UDP_ENTRY
	.description="temp",
    };
    return temp;
}


/**********************
 * 插件私有数据定义后，在可init中初始化
 * init函数仅在插件被调用时被调用一次
 * *******************/

int num;
char str[10];
void init_temp(){
    num=1;
    memset(str,0,10);
    fprintf("###NOTE: I am initialing !!!! \n");
}

/***********************
 * 协议处理函数，有三种形参形式，分别对应ip，tcp，udp
 * 与libnids的三种供注册的回调函数形参形式对应（保证代码移植方面）
 *********************/
int IP_ENTRY(struct ip_header *iphdr, int len){

}
int TCP_ENTRY(struct tcp_stream* a_tcp, void *data){
    
}
int UDP_ENTRY(struct tuple4 *addr, char *buf, int len, struct ip_header *iphdr){

}



