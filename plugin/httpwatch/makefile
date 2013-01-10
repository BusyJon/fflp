inc:=-I../../lp -I../../Util
plugindir:=../../lp/plugin
util:=$(HOME)/project/Util

all:httpwatch.so
httpwatch.so:main.c $(util)/network.c $(util)/str.c 
	gcc -shared -fPIC -g -Wall $(inc) -o $(plugindir)/$@ $^ 
