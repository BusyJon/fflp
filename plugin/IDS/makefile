#inc:=../../lp/src/plugin.h
plugindir:=../../lp/plugin
#plugindir:=../../test/dl/plugin

all:ids.so
ids.so:main.c
	gcc -shared -fPIC -o $(plugindir)/$@ $^
