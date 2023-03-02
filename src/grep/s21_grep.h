#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <glob.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct GrepSettings {
  int i_option;
  int v_option;
  int c_option;
  int l_option;
  int n_option;
  int h_option;
  int s_option;
  int o_option;
};

typedef struct GrepSettings GrepSettings;

void s21_finder(GrepSettings grep_set, regex_t *regex_arr, int len, int argc,
                char **argv, int curr_pos);
regex_t *s21_combine(regex_t *reg1, regex_t *reg2, int e_n, int f_n);
regex_t *s21_parse_part_II(int argc, char *argv[], int *e_n, int *err,
                           GrepSettings grep_set);
regex_t *s21_parse_part_III(int argc, char *argv[], int *f_n, int *err,
                            GrepSettings grep_set);
regex_t *s21_parse_part_IV(int *curr_pos, int argc, char *argv[],
                           regex_t *regex_arr, int *len, GrepSettings grep_set);
regex_t *s21_get_regex_from_file(char **file_names, int len, int *f_n,
                                 int *error, GrepSettings grep_set);
void s21_parse_part_I(int n, char *argv[], int *err, GrepSettings *set);
char *s21_string_filter(char *str);

#endif  // SRC_GREP_S21_GREP_H_
