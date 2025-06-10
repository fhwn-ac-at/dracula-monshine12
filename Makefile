CC = clang
CFLAGS += -Wall -Wextra -Werror --std=c17

main: main.c libs/logger.c libs/game_board.c libs/config_manager.c libs/sim.c

clean:
	rm -f main *.o

.PHONY: clean
