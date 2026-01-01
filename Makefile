CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

SRC := $(wildcard src/**/*.c src/*.c)

lab4:
	$(CC) $(CFLAGS) $(SRC) -o lab4_vm

clean:
	rm -f lab4_vm
