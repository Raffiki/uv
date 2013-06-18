CFLAGS = -I../vendor/libuv/include 
LDFLAGS = -L../vendor/libuv -luv -ldl -pthread

all: 
	$(CC) $(CFLAGS) main.c -o main $(LDFLAGS) -pthread -framework CoreFoundation -framework CoreServices
