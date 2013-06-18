CFLAGS = -Ivendor/libuv/include 
LDFLAGS = -Lvendor/libuv -luv -ldl -lrt -pthread

all: 
	$(CC) $(CFLAGS) filesystem.c -o filesystem $(LDFLAGS) 
