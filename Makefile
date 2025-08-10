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

CFLAGS_TEST = -Wall -Wextra -Werror -Wpointer-arith -Wcast-align \
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
TEST_TARGET = test.out

MAIN_SRC = $(wildcard src/*.c)
TEST_SRC = $(filter-out src/main.c, $(wildcard src/*.c)) $(filter-out test/testdata/%, $(wildcard test/*.c))
LIB_SRC = lib/mr_utils/*.c 

.PHONY: all build run clean format format-check bear debug test check

all: whodoyouthinkyouareiam

whodoyouthinkyouareiam: build run

build:
	$(CC) $(CFLAGS) -o $(MAIN_TARGET) $(MAIN_SRC) $(LIB_SRC) $(LINKS)

build-test:
	$(CC) $(CFLAGS_TEST) -o $(TEST_TARGET) $(TEST_SRC) $(LIB_SRC)

run:
	./$(MAIN_TARGET)

run-test:
	./$(TEST_TARGET)

clean:
	-rm -f $(MAIN_TARGET) $(TEST_TARGET)

format:
	find ./src ./test -not -path './test/testdata/*' \( -name '*.h' -o -iname '*.c' \) | xargs clang-format -i --verbose

format-check:
	find ./src ./test -not -path './test/testdata/*' \( -name '*.h' -o -iname '*.c' \) | xargs clang-format --dry-run --Werror --verbose

bear-debug: # this is for creating the compile_commands.json file
	rm -f compile_commands.json && bear -- make build

bear-test: # this is for creating the compile_commands.json file
	rm -f compile_commands.json && bear -- make build-test

debug: 
	$(CC) $(CFLAGS_DEBUG) -o $(MAIN_TARGET) $(MAIN_SRC) $(LIB_SRC) $(LINKS)

check: format-check debug test

test: build-test run-test


