CFLAGS = -Ivendor/libuv/include 
LDFLAGS = -Lvendor/libuv -luv -ldl -lrt -pthread

all: 
	$(CC) $(CFLAGS) main.c -o main $(LDFLAGS) 
