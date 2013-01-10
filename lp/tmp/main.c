#if 0

#define MAX_PACKET_LEN 2048

void truncate_packet(plugin *proto, u_char **pkt, int *length) {
    *length = *length - proto->get_header_length();
    *pkt = *pkt + proto->get_header_length();
}


plugin* identify_upper_proto(plugin* proto, const u_char *pkt, int length) {
    vector<plugin>* proto_vec = plugin_graph.getChildlist(*proto);
    vector<plugin>::iterator begin = proto_vec->begin();
    for (; begin != proto_vec->end(); begin++) {
	if (begin->identify(pkt, length)) {
	    return &(*begin);
	}
    }
    return NULL;
}

void processPacket(u_char *arg, const pcap_pkthdr *pkthdr, const u_char *packet) {
    int *counter = (int *) arg;
    //打印基本信息
    printf("###Basic Info:\n");
    printf("Pcaket Count: %d\n", ++(*counter));
    printf("Captured Packet Size: %d\n", pkthdr->caplen);
    printf("Real Packet Size: %d\n",pkthdr->len);
    check_length(pkthdr);
    printf("Timeval: %ld %ld\n", pkthdr->ts.tv_sec, pkthdr->ts.tv_usec);
    printf("###Payload:\n");
    print_data(packet, pkthdr->caplen,16);

    //协议识别
    printf("###Identify:\n");
    const u_char *pkt = packet;
    int length = pkthdr->caplen;
    print_data(pkt,length,16);
    printf("\n------------------------>>");

    /*
    //首先有一个起始协议
    printf(">> identifed by eth\n");
    plugin *proto = plugin_graph.getHead();
    plugin *upper_proto=NULL;
    while(true){
    //协议处理
    void *header = proto->init_header(pkt, pkthdr->caplen);
    proto->print_header(header);
    proto->destroy_header(header);
    //协议识别，如能识别，继续，不能识别，结束
    upper_proto = identify_upper_proto(proto, pkt, length);
    if(upper_proto==NULL)
    break;
    //截断包数据
    length = length - proto->get_header_length();
    pkt = pkt + proto->get_header_length();
    proto=upper_proto;
    }
    printf("------------------------------------------------------------->>\n");
    */
    return;
}

int main() {
    int count = 0;
    pcap_t *descr = NULL; 
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);
    char *device = NULL;

    device = pcap_lookupdev(errbuf);
    null_exit_str(device, "###ERROR: can't open network interface device!!!!"); 
    printf("###NOTE: open device %s\n", device);

    //寻找ip和mask
    u_int32_t net;
    u_int32_t mask;
    int ret=pcap_lookupnet("wlan0", &net, &mask, errbuf);
    lt_exit_str(ret, 0, errbuf);

    /*
    //register all plugins
    register_all();
    init_plugin_graph();
    show_plugin_graph();
    */

    //	设置过滤器
    //	struct bpf_program fcode;
    //	const char *filter_str="http";
    //	if(pcap_compile(descr,&fcode,filter_str,0,net)<0)
    //	    printf("###%s\n",errbuf);
    //	if(pcap_setfilter(descr,&fcode)<0)
    //	    printf("###%s\n",errbuf);

    //第二个参数表示最大可以接收的数据。
    //以混杂模式打开网卡
    //512是超时时间timeout
    descr = pcap_open_live(device, MAX_BYTES_FOR_CAPTURE, 1, 512, errbuf);
    pcap_loop(descr, -1, processPacket, (u_char *) &count);

    /*
    //unregister all plugins
    unregister_all();
    */
    return 0;
}

//手工配置处理路径,本应由插件系统自动调用实现
int num=1;
void proc_ip_gre_ppp(struct ip *ip){
    printf("package %d\n",num++);
    //ip
    u_long ip_pkg_len=ntohs(ip->ip_len);
    u_long ip_hdr_len=ip->ip_hl*4;
    u_long ip_pld_len=ip_pkg_len-ip_hdr_len;
    print_ip(ip,ip_pkg_len);
    //gre
    u_char *gre=(u_char*)ip+ip_hdr_len;
    u_long gre_hdr_len=get_gre_hdr_len((struct gre*)gre,ip_pld_len);
    u_long gre_pld_len=get_gre_pld_len((struct gre*)gre);
    print_gre((struct gre*)gre,ip_pld_len);
    //ppp
    u_char *ppp=gre+gre_hdr_len;
    print_ppp(ppp,gre_pld_len);
    printf("\n");
}
#endif

