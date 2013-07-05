CFLAGS = -I../vendor/libuv/include 
LDFLAGS = -L../vendor/libuv -luv -lrt -ldl -pthread

all: 
	$(CC) $(CFLAGS) server.c -o server $(LDFLAGS) 
