CFLAGS = -I../vendor/libuv/include 
LDFLAGS = -L../vendor/libuv -luv -lrt -ldl -pthread

all: 
	$(CC) $(CFLAGS) main.c -o main $(LDFLAGS) 
