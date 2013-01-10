inc:=-I../../Util -I../../lp
plugindir:=../../lp/plugin
util:=$(HOME)/project/Util
srcs:=main.c mppcdecode.c mppc/mppcd.c mppc/mppcc.c 
srcs+=$(util)/network.c $(util)/print.c

all: pptp_mppc.so
pptp_mppc.so:$(srcs) 
	gcc -fPIC -Wall -g -shared $(inc) -o $(plugindir)/$@ $^
clear:
	rm *o -f

.PHONY:all clear pptp_mppc
