/*-
 * Copyright (c) 2002-2004 Jan Dubiec <jdx@slackware.pl>
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
 * MPPC compression library.
 * Version 1.0
 *
 * Please note that Hi/Fn (http://www.hifn.com) holds US patents on some 
 * implementation critical aspects of MPPC so you should check if those 
 * patents are valid in your country in order to avoid legal problems.
 */

//#include <sys/systm.h>
//#include <net/mppc.h>
#include <sys/param.h>
#include <inttypes.h>
#include <netinet/in_systm.h>

#include "mppc.h"

#define HASH(x)		(((40543*(((((x)[0]<<4)^(x)[1])<<4)^(x)[2]))>>4) & 0x1fff)

struct MPPC_comp_state;

/* inserts 1 to 8 bits into the output buffer */
static void __inline putbits8(uint8_t *buf, uint32_t val, const uint32_t n,
		uint32_t *i, uint32_t *l) {
	buf += *i;
	if (*l >= n) {
		*l = (*l) - n;
		val <<= *l;
		*buf = *buf | (val & 0xff);
		if (*l == 0) {
			*l = 8;
			(*i)++;
			*(++buf) = 0;
		}
	} else {
		(*i)++;
		*l = 8 - n + (*l);
		val <<= *l;
		*buf = *buf | ((val >> 8) & 0xff);
		*(++buf) = val & 0xff;
	}
}

/* inserts 9 to 16 bits into the output buffer */
static void __inline putbits16(uint8_t *buf, uint32_t val, const uint32_t n,
		uint32_t *i, uint32_t *l) {
	buf += *i;
	if (*l >= n - 8) {
		(*i)++;
		*l = 8 - n + (*l);
		val <<= *l;
		*buf = *buf | ((val >> 8) & 0xff);
		*(++buf) = val & 0xff;
		if (*l == 0) {
			*l = 8;
			(*i)++;
			*(++buf) = 0;
		}
	} else {
		(*i)++;
		(*i)++;
		*l = 16 - n + (*l);
		val <<= *l;
		*buf = *buf | ((val >> 16) & 0xff);
		*(++buf) = (val >> 8) & 0xff;
		*(++buf) = val & 0xff;
	}
}

/* inserts 17 to 24 bits into the output buffer */
static void __inline putbits24(uint8_t *buf, uint32_t val, const uint32_t n,
		uint32_t *i, uint32_t *l) {
	buf += *i;
	if (*l >= n - 16) {
		(*i)++;
		(*i)++;
		*l = 16 - n + (*l);
		val <<= *l;
		*buf = *buf | ((val >> 16) & 0xff);
		*(++buf) = (val >> 8) & 0xff;
		*(++buf) = val & 0xff;
		if (*l == 0) {
			*l = 8;
			(*i)++;
			*(++buf) = 0;
		}
	} else {
		(*i)++;
		(*i)++;
		(*i)++;
		*l = 24 - n + (*l);
		val <<= *l;
		*buf = *buf | ((val >> 24) & 0xff);
		*(++buf) = (val >> 16) & 0xff;
		*(++buf) = (val >> 8) & 0xff;
		*(++buf) = val & 0xff;
	}
}

size_t MPPC_SizeOfCompressionHistory(void) {
	return sizeof(struct MPPC_comp_state);
}

void MPPC_InitCompressionHistory(char *history) {
	struct MPPC_comp_state *state = (struct MPPC_comp_state*) history;
	memset(history, 0, sizeof(struct MPPC_comp_state));
	state->histptr = MPPE_HIST_LEN;
}

//--该算法实现定义了一个两个连续的历史记录缓冲区，使用时把后半段的放到前半段去
//--state->histptr从后半段开始随包的压缩而增加
//--所以变化范围是后半段，当不足以容纳包时重置state->histptr到后半段起点,并把后半段记录复制到前半段
//--olen在putbitsxx函数中不断增加，olen因该为output sbuf的开始点
//--对压缩函数传入restart-history标志，则在压缩的最后会重置历史记录缓冲
int MPPC_Compress(u_char **src, u_char **dst, u_long *srcCnt, u_long *dstCnt,
		char *history, int flags, int undef) {
	struct MPPC_comp_state *state = (struct MPPC_comp_state*) history;
	uint32_t olen, off, len, idx, i, l;
	uint8_t *hist, *sbuf, *p, *q, *r, *s;
	int rtn = MPPC_OK;

	/*
	 At this point, to avoid possible buffer overflow caused by packet
	 expansion during/after compression, we should make sure we have
	 space for the worst case.

	 Maximum MPPC packet expansion is 12.5%. This is the worst case when
	 all octets in the input buffer are >= 0x80 and we cannot find any
	 repeated tokens.
	 */
	if (*dstCnt < (*srcCnt * 9 / 8 + 2)) {
		rtn &= ~MPPC_OK;
		return rtn;
	}

	/* We can't compress more then MPPE_HIST_LEN bytes in a call */
	if (*srcCnt > MPPE_HIST_LEN) {
		rtn &= ~MPPC_OK;
		return rtn;
	}

	//stat->hist是双缓冲，hist设置state->hist的后半段起点
	hist = state->hist + MPPE_HIST_LEN;
	/* check if there is enough room at the end of the history */
	if (state->histptr + *srcCnt >= 2 * MPPE_HIST_LEN) {
		rtn |= MPPC_RESTART_HISTORY;
		state->histptr = MPPE_HIST_LEN;
		//把后半段缓冲复制到前半段缓冲
		memcpy(state->hist, hist, MPPE_HIST_LEN);
	}
	/* add packet to the history */
	sbuf = state->hist + state->histptr;
	memcpy(sbuf, *src, *srcCnt);
	state->histptr += *srcCnt;

	/* compress data */
	r = sbuf + *srcCnt;
	**dst = olen = i = 0;
	l = 8;
	while (i < *srcCnt - 2) {
		s = q = sbuf + i;

		/* Prognose matching position using hash function */
		idx = HASH(s);
		p = hist + state->hash[idx];
		state->hash[idx] = (uint16_t) (s - hist);
		if (p > s) /* It was before MPPC_RESTART_HISTORY */
			p -= MPPE_HIST_LEN; /* try previous history buffer */
		off = s - p;

		/* Check our prognosis */
		if (off > MPPE_HIST_LEN - 1 || off < 1 || *p++ != *s++ || *p++ != *s++
				|| *p++ != *s++) {
			/* no match found; encode literal byte */
			if ((*src)[i] < 0x80) { /* literal byte < 0x80 */
				putbits8(*dst, (uint32_t) (*src)[i], 8, &olen, &l);
			} else { /* literal byte >= 0x80 */
				putbits16(*dst, (uint32_t) (0x100 | ((*src)[i] & 0x7f)), 9,
						&olen, &l);
			}
			++i;
			continue;
		}

		/* Find length of the matching fragment */
#if defined(__amd64__) || defined(__i386__)
		/* Optimization for CPUs without strict data aligning requirements */
		while ((*((uint32_t*) p) == *((uint32_t*) s)) && (s < (r - 3))) {
			p += 4;
			s += 4;
		}
#endif
		while ((*p++ == *s++) && (s <= r))
			;
		len = s - q - 1;
		i += len;

		/* at least 3 character match found; code data */
		/* encode offset */
		if (off < 64) { /* 10-bit offset; 0 <= offset < 64 */
			putbits16(*dst, 0x3c0 | off, 10, &olen, &l);
		} else if (off < 320) { /* 12-bit offset; 64 <= offset < 320 */
			putbits16(*dst, 0xe00 | (off - 64), 12, &olen, &l);
		} else if (off < 8192) { /* 16-bit offset; 320 <= offset < 8192 */
			putbits16(*dst, 0xc000 | (off - 320), 16, &olen, &l);
		} else {
			/* This shouldn't happen. */
			rtn &= ~MPPC_OK;
			return rtn;
		}

		/* encode length of match */
		if (len < 4) { /* length = 3 */
			putbits8(*dst, 0, 1, &olen, &l);
		} else if (len < 8) { /* 4 <= length < 8 */
			putbits8(*dst, 0x08 | (len & 0x03), 4, &olen, &l);
		} else if (len < 16) { /* 8 <= length < 16 */
			putbits8(*dst, 0x30 | (len & 0x07), 6, &olen, &l);
		} else if (len < 32) { /* 16 <= length < 32 */
			putbits8(*dst, 0xe0 | (len & 0x0f), 8, &olen, &l);
		} else if (len < 64) { /* 32 <= length < 64 */
			putbits16(*dst, 0x3c0 | (len & 0x1f), 10, &olen, &l);
		} else if (len < 128) { /* 64 <= length < 128 */
			putbits16(*dst, 0xf80 | (len & 0x3f), 12, &olen, &l);
		} else if (len < 256) { /* 128 <= length < 256 */
			putbits16(*dst, 0x3f00 | (len & 0x7f), 14, &olen, &l);
		} else if (len < 512) { /* 256 <= length < 512 */
			putbits16(*dst, 0xfe00 | (len & 0xff), 16, &olen, &l);
		} else if (len < 1024) { /* 512 <= length < 1024 */
			putbits24(*dst, 0x3fc00 | (len & 0x1ff), 18, &olen, &l);
		} else if (len < 2048) { /* 1024 <= length < 2048 */
			putbits24(*dst, 0xff800 | (len & 0x3ff), 20, &olen, &l);
		} else if (len < 4096) { /* 2048 <= length < 4096 */
			putbits24(*dst, 0x3ff000 | (len & 0x7ff), 22, &olen, &l);
		} else if (len < 8192) { /* 4096 <= length < 8192 */
			putbits24(*dst, 0xffe000 | (len & 0xfff), 24, &olen, &l);
		} else {
			/* This shouldn't happen. */
			rtn &= ~MPPC_OK;
			return rtn;
		}
	}

	/* Add remaining octets to the output */
	while (*srcCnt - i > 0) {
		if ((*src)[i] < 0x80) { /* literal byte < 0x80 */
			putbits8(*dst, (uint32_t) (*src)[i++], 8, &olen, &l);
		} else { /* literal byte >= 0x80 */
			putbits16(*dst, (uint32_t) (0x100 | ((*src)[i++] & 0x7f)), 9, &olen,
					&l);
		}
	}

	/* Reset unused bits of the last output octet */
	if ((l != 0) && (l != 8)) {
		putbits8(*dst, 0, l, &olen, &l);
	}

	/* If result is bigger then original, set flag and flush history */
	if ((*srcCnt < olen) || ((flags & MPPC_SAVE_HISTORY) == 0)) {
		if (*srcCnt < olen)
			rtn |= MPPC_EXPANDED;
		bzero(history, sizeof(struct MPPC_comp_state));
		state->histptr = MPPE_HIST_LEN;
	}

	*src += *srcCnt;
	*srcCnt = 0;
	*dst += olen;
	*dstCnt -= olen;

	return rtn;
}
