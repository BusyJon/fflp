#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../lp/plugin.h" 

/****************
 * XXX 计划中!!!!
 * 对snort的特征库进行移植
 * 模拟snort的入侵检测机制
 ***************/

plugin_t plugin_ids();
void init_IDS();
int IDS();

plugin_t plugin_ids(){
    plugin_t ids={
	.init = init_IDS,
	.disect = IDS,
	.description = "ids working",
    };
    return ids;
}

void init_IDS(){
}

int IDS(){
    return PASS;
}

