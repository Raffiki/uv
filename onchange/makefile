CFLAGS = -I../vendor/libuv/include 
LDFLAGS = -L../vendor/libuv -luv -ldl -lrt -pthread

all: 
	$(CC) $(CFLAGS) main.c -o main $(LDFLAGS)
