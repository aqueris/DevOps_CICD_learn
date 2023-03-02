.PHONY: s21_cat test clean check
CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra
SOURCES=s21_cat.c

s21_cat: clean $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o s21_cat

test: s21_cat
	bash s21_cat_test.sh

clean:
	rm -rf s21_cat

check:
	clang-format -n s21_cat.c
	clang-format -n s21_cat.h