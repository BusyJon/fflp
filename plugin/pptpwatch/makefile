inc:=-I../../Util -I../../lp
plugindir:=../../lp/plugin
util:=../../Util
srcs:=main.c
srcs+=$(util)/util.c $(util)/debug.c $(util)/network.c 
srcs+=$(util)/str.c $(util)/print.c 


all:pptpwatch.so
pptpwatch.so:$(srcs)
	gcc -shared -fPIC -g -Wall -o $(plugindir)/$@ $^ $(inc)
