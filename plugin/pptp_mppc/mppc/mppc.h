/*-
 * Copyright (c) 2007 Alexander Motin <mav@freebsd.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * MPPC compression/decompression library.
 * Version 1.0
 *
 * Please note that Hi/Fn (http://www.hifn.com) holds US patents on some 
 * implementation critical aspects of MPPC so you should check if those 
 * patents are valid in your country in order to avoid legal problems.
 */

#include <sys/types.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _NET_MPPC_H_
#define	_NET_MPPC_H_

#define MPPC_MANDATORY_COMPRESS_FLAGS 0
#define MPPC_MANDATORY_DECOMPRESS_FLAGS 0

#define MPPC_SAVE_HISTORY 1

#define MPPC_OK 5
#define MPPC_EXPANDED 8
#define MPPC_RESTART_HISTORY 16
#define MPPC_DEST_EXHAUSTED 32

#define MPPE_HIST_LEN          8192
struct MPPC_comp_state {
	uint8_t hist[2 * MPPE_HIST_LEN];
	uint16_t histptr;
	uint16_t hash[MPPE_HIST_LEN];
};

struct MPPC_decomp_state {
    uint8_t	hist[2*MPPE_HIST_LEN];
    uint16_t	histptr;
};


size_t MPPC_SizeOfCompressionHistory(void);
size_t MPPC_SizeOfDecompressionHistory(void);

void MPPC_InitCompressionHistory(char *history);
void MPPC_InitDecompressionHistory(char *history);

int MPPC_Compress(u_char **src, u_char **dst, u_long *srcCnt,
		u_long *dstCnt, char *history, int flags, int undef);
int MPPC_Decompress(u_char **src, u_char **dst, u_long *srcCnt,
		u_long *dstCnt, char *history, int flags);

#endif
