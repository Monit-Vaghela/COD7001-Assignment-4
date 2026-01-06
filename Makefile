CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

SRC := $(wildcard src/**/*.c src/*.c)

lab4:
	$(CC) $(CFLAGS) $(SRC) -o VM

clean:
	rm -f VM
	find . -name "*.bin" -type f -delete
