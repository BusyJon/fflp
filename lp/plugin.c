
#include "plugin.h"

LIST_HEAD(tuple_head);
LIST_HEAD(pi_head);

#if 0
LIST_HEAD(ip_pi_head);
LIST_HEAD(tcp_pi_head);
LIST_HEAD(udp_pi_head);
#endif

/* XXX
 * 关键函数：初始化插件(ip, tcp, udp)
 */
void init_plugin(){
    FILE *fd=open_conf3(opt.plugin_conf, &tuple_head, ':');
    //the read plugin_path is plugin_dir + pi->file
    char plugin_path[FIELDLEN*2];
    char func[FIELDLEN];
    plugin_info_t *pi;
    list_head_t *node=tuple_head.next;
    for(; node != &tuple_head; node=node->next){
	//分配堆内存给插件
	pi=(plugin_info_t *)malloc(sizeof(plugin_info_t));
	//给插件赋值
	get_tuple3(node, pi->file, pi->entry, func);
	strcpy(plugin_path,opt.plugin_dir);
	strncat(plugin_path,pi->file,FIELDLEN-strlen(plugin_path));
	if(!is_file_exist(plugin_path)){
	    printf("###NOTE: the plugin of %s is't exist!!!!\n",plugin_path);
	    continue;
	}
	void *handle=dlopen(plugin_path, RTLD_LAZY);
	if(handle == NULL){
	    printf("###ERROR: failed to open plugin %s\n",plugin_path);
	    exit(-1);
	}
	pi->handle=handle;
	pi->func=(plugin_t (*)())dlsym(handle,func);
	pi->plugin=pi->func();
	//链接到插件队列上
	list_add_tail(&pi->list, &pi_head);
#if 0
	//根据入口entry链接到三个链表
	if(strcmp(pi->entry, "ip") == 0){
	    list_add_tail(&pi->list, &ip_pi_head);
	}else if(strcmp(pi->entry, "tcp") == 0){
	    list_add_tail(&pi->list, &tcp_pi_head);
	}else if(strcmp(pi->entry, "udp") == 0){
	    list_add_tail(&pi->list, &udp_pi_head);
	}else{
	    fprintf(stderr,"###ERROR: entry of plugin(%s) is't defined !!!! \n", pi->file);
	}
#endif
    }
    close_conf3(fd,&tuple_head);
}


void register_plugin(){
    list_head_t *node=pi_head.next;
    plugin_info_t *pi;
    plugin_t *p;
    while(node != &pi_head){
	pi=container_of(node, plugin_info_t, list);
	p=&pi->plugin;
	//根据入口entry链接到三个入口函数中
	if(strcmp(pi->entry, "ip") == 0){
	    nids_register_ip_frag(p->disect);
	}else if(strcmp(pi->entry, "tcp") == 0){
	    nids_register_tcp(p->disect);
	}else if(strcmp(pi->entry, "udp") == 0){
	    nids_register_udp(p->disect);
	}else{
	    fprintf(stderr,"###ERROR: entry of plugin(%s) is't defined !!!! \n", pi->file);
	}
	//回调插件内置的初始化函数
	p->init();
	node=node->next;
    }
}


static inline void __close_plugin_handle(plugin_info_t *pi){
    dlclose(pi->handle);
}


void destroy_plugin(){
    //关闭打开的插件(.so)
    list_for_each(&pi_head, plugin_info_t, list, __close_plugin_handle);
    //释放动态分配的插件的堆空间
    list_for_each_del(&pi_head, plugin_info_t, list);
#if 0
    //关闭打开的插件(.so)
    list_for_each(&ip_pi_head, plugin_info_t, list, __close_plugin_handle);
    list_for_each(&tcp_pi_head, plugin_info_t, list,  __close_plugin_handle);
    list_for_each(&udp_pi_head, plugin_info_t, list,  __close_plugin_handle);
    //释放动态分配的插件的堆空间
    list_for_each_del(&ip_pi_head, plugin_info_t, list);
    list_for_each_del(&tcp_pi_head, plugin_info_t, list);
    list_for_each_del(&udp_pi_head, plugin_info_t, list);
#endif
}


#if 0
void read_plugin(){
    FILE *f=fopen(plugin_conf,"rt");          
    null_exit_str(f,"###ERROR: can't open plugin config file\n");
    const char *plugindir="./plugin/";
#define BUFLEN 1024
    char line[BUFLEN];
    int len=0,i=0;
    while((len=getline2(f, line, BUFLEN)) != EOF){
	if(strcmp(line,"")==0 || *line == '#')
	    continue;
	char *_line=line;
	int index=0;
	char field[BUFLEN];

	//plugin_file
	index=STRCHR(_line,':');
	STRNMERGE(field,plugindir, strlen(plugindir), _line, index);
	strcpy(plugins[i].file,field);
	DEBUG("%s",field);
	plugins[i].handle=dlopen(field,RTLD_LAZY);

	//plugin_entry
	_line+=(index+1);
	index=STRCHR(_line,':');
	STRNCPY(field,_line,index);
	DEBUG("%s",field);
	strcpy(plugins[i].entry,field);

	//plugin_func
	_line+=(index+1);
	STRNCPY(field,_line,strlen(_line));
	DEBUG("%s",field);
	plugins[i].func=(plugin_t (*)())dlsym(plugins[i].handle,field);
	i++;
    }
}
#endif


#if 0
void take_plugin(int plugin_size){
    int i=0;
    plugin_info_t *pi;
    for(i=0; i<plugin_size; i++){
	pi=plugins+i;
	pi->plugin=pi->func();
	if(strcmp(pi->entry,"ip")==0){
	    list_add(&pi->plugin.list,&ip_head);
	}else if(strcmp(pi->entry,"tcp")==0){
	    list_add(&pi->plugin.list,&tcp_head);
	}else if(strcmp(pi->entry,"udp")==0){
	    list_add(&pi->plugin.list,&udp_head);
	}
	DEBUG("%s",pi->entry);
    } 
}
#endif
