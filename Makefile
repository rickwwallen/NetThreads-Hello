 TARGET=hello
 include ../bench.mk
 hello: hello.o pktbuff.o memcpy.o arp.o dev.o
