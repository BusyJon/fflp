
#ifndef OPTION_H_
#define OPTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "nids.h"
#include "klist.h"

typedef struct fflp_option{
    char mode[10];
    char device[64];
    char pcap_filter[128];
    char plugin_dir[64];
    char plugin_conf[64];
}fflp_option_t;


extern fflp_option_t opt;
extern list_head_t option_head;

void init_fflp_option();

#endif
