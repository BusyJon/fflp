inc:=-I../../Util -I../../lp
plugindir:=../../lp/plugin

all:killtcp.so
killtcp.so:main.c
	gcc -shared -g -fPIC -Wall -o $(plugindir)/$@ $^ $(inc)
