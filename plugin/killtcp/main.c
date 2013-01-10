#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "plugin.h"
#include "header.h"
#include "network.h"
#include "str.h"

/******************
 * 从libnids的源码移植而来
 * ****************/

plugin_t plugin_killtcp();
void init_killtcp();
int killtcp();

plugin_t plugin_killtcp(){
    plugin_t killtcp={
	.init=init_killtcp,
	.disect=killtcp,
	.description="do_killtcp",
    };
    return killtcp;
}

void init_killtcp(){
}

int killtcp(){
    return 0;
}

