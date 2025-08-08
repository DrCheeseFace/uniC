CC = gcc

CFLAGS_DEBUG = -Wall -Wextra -Werror -Wpointer-arith -Wcast-align \
         -Wstrict-prototypes -Wwrite-strings -Waggregate-return \
         -Wswitch-default -Wswitch-enum -Wunreachable-code \
	 -Wunused-parameter -Wuninitialized -Winit-self -Wpedantic \
         -pedantic-errors -Wbad-function-cast -Wcast-align\
	 -Wformat=2 -Wlogical-op   -Wmissing-include-dirs \
         -Wredundant-decls -Wsequence-point -Wshadow \
	 -Wswitch -Wundef -Wunused-but-set-parameter \
	 -Wcast-qual  -Wfloat-equal -Wnested-externs \
	 -O0 -std=c11 -g \
	 -fsanitize=address \
	 
CFLAGS = -Wall -Wextra -Werror \
	 -O2 -std=c11\

LINKS = 

MAIN_TARGET = main.out
MAIN_SRC = src/*.c

.PHONY: all build run clean format format-check bear debug build-mahc-header build-mahc

all: whodoyouthinkyouareiam

whodoyouthinkyouareiam:  build-mahc-header build-mahc build run

build:
	$(CC) $(CFLAGS) -o $(MAIN_TARGET) $(MAIN_SRC) $(LINKS)

run:
	./$(MAIN_TARGET)

clean:
	-rm -f $(MAIN_TARGET)

format: 
	find ./src -name '*.h' -o -iname '*.c' | xargs clang-format -i --verbose

format-check:
	find ./src -name '*.h' -o -iname '*.c' | xargs clang-format --dry-run --Werror

bear: # this is for creating the compile_commands.json file
	rm -f compile_commands.json && bear -- make build

debug: build-mahc-header build-mahc
	$(CC) $(CFLAGS_DEBUG) -o $(MAIN_TARGET) $(MAIN_SRC) $(LINKS)

check: format-check debug format


