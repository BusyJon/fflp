#ifndef PLUGIN_H_
#define PLUGIN_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dlfcn.h>

#include "option.h"

#include "klist.h"
#include "conf.h"
#include "debug.h"
#include "file.h"


//插件结构体
typedef struct plugin {
    void (*init)();
    //disect返回值为int，作为以后的扩展
    int (*disect)();
    const char *description;
}plugin_t;


/* XXX
 * 由插件配置文件中三元组来确定插件信息结构体plugin_info_t
 * <plugin_file, plugin_entry, plugin_func> 三元组
 * plugi_file是插件文件(.so)
 * plugin_entry是插件入口可以是ip，tcp，udp三种
 * plugin_func是插件注册函数，返回plugin结构体
 */
typedef struct plugin_info{
    //三元组
    char file[64];
    char entry[5];
    plugin_t (*func)();
    //由三元组操作得到
    void *handle;
    plugin_t plugin;
    //list
    list_head_t list;
}plugin_info_t;


extern list_head_t tuple_head;
extern list_head_t pi_head;

#if 0
//pi means plugin_info
extern list_head_t ip_pi_head;
extern list_head_t tcp_pi_head;
extern list_head_t udp_pi_head;
#endif

//it's declared in option.h
extern fflp_option_t opt;

void init_plugin();
void register_plugin();
void destroy_plugin();

#if 0
void read_plugin();
void take_plugin(int plugin_size);
#endif

#endif


