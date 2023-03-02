.PHONY: s21_grep test clean check
CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra
SOURCES=s21_grep.c

s21_grep: clean $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o s21_grep

test: s21_grep
	bash s21_grep_test.sh

clean:
	rm -rf s21_grep

check:
	clang-format -n s21_grep.c
	clang-format -n s21_grep.h