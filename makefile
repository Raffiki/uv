CFLAGS = -Ivendor/libuv/include 
LDFLAGS = -Lvendor/libuv -luv -ldl -pthread

all: 
	$(CC) $(CFLAGS) filesystem.c -o filesystem $(LDFLAGS) -pthread -framework CoreFoundation -framework CoreServices
