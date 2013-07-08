CFLAGS = -I../vendor/libuv/include 
LDFLAGS = -L../vendor/libuv -luv -lrt -ldl -pthread

all: 
	$(CC) $(CFLAGS) client.c -o client $(LDFLAGS) 
	$(CC) $(CFLAGS) server.c -o server $(LDFLAGS) 
