CC = clang
CFLAGS += -Wall -Wextra -Werror --std=c17 -g

main: main.c libs/logger.c libs/game_board.c libs/config_manager.c libs/sim.c

clean:
	rm -f main *.o

.PHONY: clean
