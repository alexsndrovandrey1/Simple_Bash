CC=gcc
CFLAGS=-Wall -Werror -Wextra
SOURCES=s21_cat.c s21_cat.h
EXECUTABLE=s21_cat
TEST = test_cat.sh
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
