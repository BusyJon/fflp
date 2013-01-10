inc:=-I../../Util -I../../lp
plugindir:=../../lp/plugin
util:=../../Util

all:udpwatch.so
udpwatch.so:main.c $(util)/network.c $(util)/str.c 
	gcc -shared -fPIC -g -Wall -o $(plugindir)/$@ $^ $(inc)
