
#include "filter.h"

//初始时校验长度
//退出时计算包的接受数和丢弃数

void check_length(const struct pcap_pkthdr *hdr){
    if(hdr->len!=hdr->caplen)
	printf("###capen != len, there is some data losed");
    return;
}

