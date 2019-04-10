objects:=pack.o protocal.o mf_info.o file_opt.o dev_info.o 
all:$(objects)
CFLAGS=-g -Wall
CC=g++
vpath %.h ../h_src
$(objects): %.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@
.PHONY: clean
clean:
	rm -f *.o
