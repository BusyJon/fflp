inc:=-I/home/ocean/project/Util
util:=/home/ocean/project/Util
libs:=-lnids -lpcap -ldl -lpthread -lpcre

srcs:=main.c
srcs+=plugin.c filter.c option.c 
srcs+=$(util)/util.c $(util)/print.c $(util)/debug.c 
srcs+=$(util)/str.c  $(util)/file.c  $(util)/klist.c
srcs+=$(util)/conf.c 

all:test
test:$(srcs)
	gcc -Wall -O0 -o $@ $^ $(libs) $(inc)

clean:
	rm *.o test
.PHONY:all lp clean
