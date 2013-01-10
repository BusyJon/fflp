#ifndef PROTO_BASE_H_
#define PROTO_BASE_H_

/* xxx
 * 各个协议的协议处理函数
 * 协议处理函数的项目有
 * {
 * 	协议字段描述结构体
 * 	端口-线程识别(当有传输层协议存在)
 * }
 */

/*
 * xxx
 * 每个协议都不知道它可以承载哪些协议，但知道它自身可以承载到哪些协议下
 * 这相当于在协议转换图中，每个协议都不知道他将有哪些子节点，但知道他可以有哪些父节点。
 */


/* xxx
 * 没有必要使用这个函数
 * 先定义一个结构体类型，表示协议，包含协议名，协议处理函数，报文数据和其大小
 * 为每个协议定义一个这种类型的变量
 * 因为可以直接在各个协议处理函数里调用对应的协议处理函数
 */
/*
 * 通过协议名返回协议处理函数
 * 个人定义
 * 	map型函数为通过传入某一个类型变量或字符串或整形
 * 	通过switch/case结构返回另一个与传入值相对应的某类型的值的函数
 */

/*
proto_op_t get_proto_op_by_name(const char *proto_name){
	switch(proto_name){
	}
}
*/


void lp_cant_dissect();//默认的协议处理函数，同时也是无法识别的协议处理函数

/* xxx
 * 无法识别时的协议处理函数
 * 是否需要将无法识别的报文记录到log中
 */
void lp_cant_dissect(const unsigned char* pkt,int length){
	printf("###warning: can't  dissect the protocol message");
	lp_print_data(pkt,length,16);
	return;
}


#endif
