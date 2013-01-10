
#include "option.h"

//默认选项
fflp_option_t opt={
    .mode="LIVE",
    .device="eth0",
    .pcap_filter="",
    .plugin_dir="./plugin/",
    .plugin_conf="./PLUGIN.conf",
};

LIST_HEAD(option_head);

void init_fflp_option(){
    FILE *fd=open_conf2("./FFLP.conf",&option_head,'=');
    get_value(&option_head, "mode",opt.mode);
    get_value(&option_head, "device", opt.device);
    get_value(&option_head, "pcap_filter", opt.pcap_filter);
    get_value(&option_head, "plugin_dir", opt.plugin_dir);
    get_value(&option_head, "plugin_conf", opt.plugin_conf);
    close_conf2(fd,&option_head);
    //设置nids参数
    if(strcmp(opt.mode, "LIVE") == 0){
	nids_params.device=opt.device;
    }else{
	nids_params.filename=opt.device;
    }
    nids_params.pcap_filter=opt.pcap_filter;
}


