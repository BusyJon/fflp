#ifndef _MPPCDECODE_H_
#define _MPPCDECODE_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include "mppc/mppc.h"

int find_ip_in_mppcd(u_char **buf,u_long *buflen);
void mppcd_ppp_data(u_char *ppp_data, u_long data_len, u_char *buf, u_long *buflen);

/* MPPC */
int mppc_comp_one(u_char *src, u_long srclen, u_char *buf, u_long *buflen);
int mppc_uncomp_one(u_char *src,u_long srclen, u_char *buf, u_long *buflen);

#endif

