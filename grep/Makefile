CC=gcc
CFLAGS=-Wall -Werror -Wextra
SOURCES=s21_grep.c s21_grep.h
EXECUTABLE=s21_grep
TEST = diff_grep.sh
SUDO=sudo bash
FORMAT=clang-format -i
MINIVERTER=../../materials/build/run.sh

build:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

test:
	$(SUDO) $(TEST)

format:
	$(FORMAT) $(SOURCES)

verter:
	$(SUDO) $(MINIVERTER) 