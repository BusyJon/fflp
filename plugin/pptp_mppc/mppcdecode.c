
#include "mppcdecode.h"
#include "print.h"
#include "util.h"
#include "debug.h"


void mppcd_ppp_data(u_char *ppp_data, u_long data_len, u_char *buf, u_long *buflen) {
    u_char *mppcd = ppp_data + 2;
    ulong mppcd_len = data_len - 2;
    int ret = mppc_uncomp_one(mppcd, mppcd_len, buf, buflen);
    DEBUG("PPP MPPCD: ret(%d) len(%ld --> %ld)\n", ret, mppcd_len, *buflen);
    print_data(buf, *buflen, 16);
}


/*
 * return 1 for find ip pkg and 0 for not find
 * if find ip pkg then change buf and buflen
 */
int find_ip_in_mppcd(u_char **buf,u_long *buflen){
   // print_data(*buf,8,16);
    int pos=0;
    for(;pos < 5 && pos < *buflen; pos++){
	if((*buf)[pos] == 0x00 && (*buf)[pos+1]== 0x21){
	    *buf += (pos+2);
	    *buflen -= (pos+2);    
	    return 1;
	}
    }
    return 0;
}


/* MPPC */
int mppc_comp_one(u_char *src, u_long srclen, u_char *buf, u_long *buflen){
	ulong destlen = *buflen;
	u_char *dest = buf;
	memset(dest, 0, *buflen);
	int flags = 0; //MPPC_RESTART_HISTORY;
	char history[MPPC_SizeOfCompressionHistory()];
	memset(history, 0, MPPC_SizeOfCompressionHistory());

	MPPC_InitCompressionHistory(history);
	int ret = MPPC_Compress(&src, &dest, &srclen, &destlen, history, flags, 0);
	*buflen -= destlen;
	return ret;
}



int mppc_uncomp_one(u_char *src,u_long srclen, u_char *buf, u_long *buflen) {
	ulong destlen = *buflen;
	u_char *dest = buf;
	int flags = 0; //MPPC_RESTART_HISTORY;
	char history[MPPC_SizeOfCompressionHistory()];
	memset(history, 0, MPPC_SizeOfCompressionHistory());

	MPPC_InitDecompressionHistory(history);
	int ret = MPPC_Decompress((u_char**) &src, (u_char**) &dest, &srclen, &destlen, history, flags);
	*buflen -= destlen;
	return ret;
}



