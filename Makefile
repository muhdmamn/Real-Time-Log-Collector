CC = gcc
CFLAGS = -Wall -Wextra

./build/RTLOG:
	mkdir -p build
	$(CC) $(CFLAGS) ./src/main.c -o ./build/RTLOG

clean:
	rm -rf ./build/RTLOG

.PHONY: clean
