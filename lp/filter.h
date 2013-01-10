

#ifndef FILTER_H_
#define FILTER_H_

#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

void check_length(const struct pcap_pkthdr *hdr);

//对任意的网络都不检验
//libnids高版本校验函数可能存在bug，以后调试
#define DONOT_SUMCHK	\
	struct nids_chksum_ctl nochksumchk; \
    nochksumchk.netaddr = 0;	\
    nochksumchk.mask = 0;   \
    nochksumchk.action = NIDS_DONT_CHKSUM;  \
    nids_register_chksum_ctl(&nochksumchk, 1);


#endif
