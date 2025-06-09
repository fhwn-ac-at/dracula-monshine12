CC = clang
CFLAGS += -Wall -Wextra -Werror --std=c17

main: main.c

clean:
	rm -f main *.o

.PHONY: clean
