CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L -Iinclude
.PHONY: all clean run genesis
all: out/cubalc
out/cubalc: src/cubalc_main.c src/cubalc_core.c include/cubalc.h include/cubalc_law.h
	@mkdir -p out
	$(CC) $(CFLAGS) -o $@ src/cubalc_main.c src/cubalc_core.c -lm
clean:
	rm -f out/cubalc
run: all
	./out/cubalc genesis
genesis: run
