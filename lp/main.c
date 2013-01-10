#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <pcap.h>
#include <nids.h>

#include "option.h"
#include "filter.h"
#include "plugin.h"
#include "filter.h"

//个人代码库
#include "util.h"
#include "debug.h"
#include "print.h"
#include "klist.h"
#include "str.h"
#include "file.h"
#include "conf.h"
#include "header.h"

int main() {
    init_fflp_option();
    if (!nids_init()) {
	printf("出现错误：%s\n", nids_errbuf);
	exit(-1);
    }
    
    DONOT_SUMCHK;
    init_plugin();
    DEBUG_LIST(pi_head,file,plugin_info_t,list);
    register_plugin();

    nids_run();
    return 0;
}

