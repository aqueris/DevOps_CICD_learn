#include "s21_grep.h"

int main(int argc, char *argv[]) {
  GrepSettings grep_set = {.i_option = 0,
                           .v_option = 0,
                           .c_option = 0,
                           .l_option = 0,
                           .n_option = 0,
                           .h_option = 0,
                           .s_option = 0,
                           .o_option = 0};
  int err = (argc < 3);
  int e_n = 0, f_n = 0;
  int curr_pos = 1;
  s21_parse_part_I(argc, argv, &err, &grep_set);
  regex_t *regex_arr =
      s21_parse_part_II(argc, argv, &e_n, &err, grep_set);  // FREE
  regex_t *regex_from_file =
      s21_parse_part_III(argc, argv, &f_n, &err, grep_set);  // FREE
  int len = e_n + f_n;
  regex_arr = s21_combine(regex_arr, regex_from_file, e_n, f_n);
  if (!e_n && !f_n) {
    regex_arr =
        s21_parse_part_IV(&curr_pos, argc, argv, regex_arr, &len, grep_set);
  }
  if (!err) {
    s21_finder(grep_set, regex_arr, len, argc, argv, curr_pos);
  }
  if (regex_arr != NULL) {
    free(regex_arr);
  }

  return 0;
}

regex_t *s21_parse_part_IV(int *curr_pos, int argc, char *argv[],
                           regex_t *regex_arr, int *len,
                           GrepSettings grep_set) {
  regex_t *ret_reg = regex_arr;
  while (*curr_pos < argc - 1) {
    if (argv[*curr_pos][0] != '-') {
      char *tmp_str = malloc((strlen(argv[*curr_pos]) + 1) * sizeof(char));
      strcpy(tmp_str, argv[*curr_pos]);
      tmp_str = s21_string_filter(tmp_str);
      (*len)++;
      if (*len == 1) {
        ret_reg = malloc((*len) * sizeof(regex_t));
      } else {
        ret_reg = realloc(ret_reg, *len * sizeof(regex_t));
      }
      if (grep_set.i_option) {
        regcomp(ret_reg + (*len - 1), tmp_str, REG_ICASE);
      } else {
        regcomp(ret_reg + (*len - 1), tmp_str, 0);
      }
      free(tmp_str);
      *curr_pos = *curr_pos + 1;
      break;
    }
    *curr_pos = *curr_pos + 1;
  }
  return ret_reg;
}

void s21_finder(GrepSettings grep_set, regex_t *regex_arr, int len, int argc,
                char **argv, int curr_pos) {
  int mul_files = 0;

  for (int i = curr_pos; i < argc; i++) {
    if (strchr(argv[i], '-') != NULL) {
      if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "-e")) {
        i++;
      }
      continue;
    }
    mul_files++;
  }

  for (int i = curr_pos; i < argc; i++) {
    if (strchr(argv[i], '-') != NULL) {
      if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "-e")) {
        i++;
      }
      continue;
    }
    FILE *f = fopen(argv[i], "r");
    if (f != NULL) {
      size_t nmatch = 2;
      regmatch_t pmatch[2];
      char c;
      int line_len = 0;
      int line_num = 0;
      int match_counter = 0;
      char *line = malloc(1);
      int on = 1;
      int matched = 0;
      while (on) {
        int scanning = fscanf(f, "%c", &c);
        if (c == '\n') {
          c = '\0';
        } else if (scanning == -1) {
          on = 0;
          c = '\0';
        }
        line_len++;
        char *tmp = realloc(line, line_len * sizeof(char));
        if (tmp != NULL) {
          line = tmp;
          *(line + line_len - 1) = c;
        }
        if ((on && c == '\0') || (!on && *line != '\0')) {
          line_num++;
          matched = 0;
          for (int k = 0; k < len; k++) {
            int ret = regexec(regex_arr + k, line, nmatch, pmatch, 0);
            if (!ret) {
              matched = 1;
              break;
            }
          }
          if (grep_set.v_option) {
            matched = matched ? 0 : 1;
          }
          if (matched) {
            match_counter++;
            if (grep_set.l_option) {
              break;
            }
            if (!grep_set.c_option) {
              if (mul_files > 1 && !grep_set.h_option) {
                printf("%s:", argv[i]);
              }
              if (grep_set.n_option) {
                printf("%d:", line_num);
              }
              if (grep_set.o_option && !grep_set.v_option) {
                int appender = pmatch[0].rm_eo;
                printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                       &line[pmatch[0].rm_so]);
                for (int k = 0; k < len; k++) {
                  while (!regexec(regex_arr + k, line + appender, nmatch,
                                  pmatch, REG_NOTBOL) &&
                         pmatch[0].rm_eo != pmatch[0].rm_so) {
                    printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                           &line[appender + pmatch[0].rm_so]);
                    appender += pmatch[0].rm_eo;
                  }
                }
              } else {
                printf("%s\n", line);
              }
            }
          }

          free(line);
          line_len = 0;
          line = malloc(0);
        }
      }
      if (grep_set.c_option) {
        if (mul_files > 1 && !grep_set.h_option) {
          printf("%s:", argv[i]);
        }
        printf("%d\n", match_counter);
      }
      if (grep_set.l_option && matched) {
        printf("%s\n", argv[i]);
      }
      free(line);
    } else {
      printf("s21_grep:No such file or directory:%s\n", argv[i]);
    }
  }
  for (int k = 0; k < len; k++) {
    regfree(regex_arr + k);
  }
}

regex_t *s21_combine(regex_t *reg1, regex_t *reg2, int e_n, int f_n) {
  if (reg1 == NULL) {
    reg1 = malloc(0);
  }
  reg1 = realloc(reg1, (e_n + f_n) * sizeof(regex_t));
  for (int i = 0; i < f_n; i++) {
    reg1[e_n + i] = reg2[i];
  }
  if (reg2 != NULL) {
    free(reg2);
  }

  return reg1;
}

char *s21_string_filter(char *str) {
  char *ret = NULL;
  if (str != NULL) {
    int n = strlen(str);
    if (*str == '\'' && *(str + n - 1) == '\'' && n > 1) {
      ret = malloc(n - 1);
      strncpy(ret, str + 1, n - 2);
      free(str);
    } else if (*str == '\"' && *(str + n - 1) == '\"' && n > 1) {
      ret = malloc(n - 1);
      strncpy(ret, str + 1, n - 2);
      free(str);
    } else {
      ret = str;
    }
  }

  return ret;
}

regex_t *s21_parse_part_II(int argc, char *argv[], int *e_n, int *err,
                           GrepSettings grep_set) {
  int counter = 0;
  regex_t *regex_arr = NULL;
  int i = 1;
  while (i < argc && !(*err)) {
    if (!strcmp(argv[i], "-e")) {
      counter++;
      i++;
      if (regex_arr == NULL) {
        regex_arr = malloc(0);
      }
      regex_t *tmp = realloc(regex_arr, counter * sizeof(regex_t));  // FREE
      if (tmp != NULL) {
        regex_arr = tmp;
      }
      char *tmp_str = malloc((strlen(argv[i]) + 1) * sizeof(char));
      strcpy(tmp_str, argv[i]);
      tmp_str = s21_string_filter(tmp_str);
      if (grep_set.i_option) {
        regcomp(regex_arr + (counter - 1), tmp_str, REG_ICASE);
      } else {
        regcomp(regex_arr + (counter - 1), tmp_str, 0);
      }
      free(tmp_str);
    }
    i++;
  }
  *e_n = counter;

  return regex_arr;
}

regex_t *s21_parse_part_III(int argc, char *argv[], int *f_n, int *err,
                            GrepSettings grep_set) {
  int i = 1;
  int counter = 0;
  char **file_names = NULL;  // FREE

  while (i < argc && !(*err)) {
    if (!strcmp(argv[i], "-f") && i != argc - 1) {
      i++;
      counter++;
      if (file_names == NULL) {
        file_names = malloc(counter * sizeof(char *));
      } else {
        char **tmp = realloc(file_names, counter * sizeof(char *));
        if (tmp != NULL) {
          file_names = tmp;
        }
      }
      *(file_names + counter - 1) =
          malloc((strlen(argv[i]) + 1) * sizeof(char));
      strcpy(*(file_names + counter - 1), argv[i]);
    }
    i++;
  }
  int file_len = counter;
  regex_t *ret =
      s21_get_regex_from_file(file_names, file_len, f_n, err, grep_set);

  for (int i = 0; i < counter; i++) {
    free(*(file_names + i));
  }
  if (file_names != NULL) {
    free(file_names);
  }

  return ret;
}

regex_t *s21_get_regex_from_file(char **file_names, int len, int *f_n,
                                 int *error, GrepSettings grep_set) {
  regex_t *regex_arr = NULL;  // FREE

  for (int i = 0; i < len && !(*error); i++) {
    FILE *f = fopen(file_names[i], "r");
    if (f == NULL && grep_set.s_option) {
      *error = 1;
      printf("No such file or directory:%s\n", file_names[i]);
    } else {
      char c;
      int line_len = 0;
      char *line = malloc(1);
      int on = 1;
      while (on) {
        c = fgetc(f);
        if (c == '\n') {
          c = '\0';
        } else if (c == EOF) {
          on = 0;
          c = '\0';
        }
        line_len++;
        char *tmp = realloc(line, line_len * sizeof(char));
        if (tmp != NULL) {
          line = tmp;
          *(line + line_len - 1) = c;
        }
        if (c == '\0') {
          line = s21_string_filter(line);
          (*f_n)++;
          if (regex_arr == NULL) {
            regex_arr = malloc(0);
          }
          regex_t *tmp = realloc(regex_arr, (*f_n) * sizeof(regex_t));
          if (tmp != NULL) {
            regex_arr = tmp;
          }
          if (grep_set.i_option) {
            regcomp(regex_arr + *f_n - 1, line, REG_ICASE);
          } else {
            regcomp(regex_arr + (*f_n) - 1, line, 0);
          }
          free(line);
          line_len = 0;
          line = malloc(0);
        }
      }
      free(line);
    }
    fclose(f);
  }

  return regex_arr;
}

void s21_parse_part_I(int n, char *argv[], int *err, GrepSettings *set) {
  for (int i = 1; i < n && !(*err); i++) {
    if (*(argv[i]) == '-' && strcmp(argv[i], "-e") && strcmp(argv[i], "-f")) {
      if (strlen(argv[i]) == 1) {
        break;
      }

      for (size_t k = 1; k < strlen(argv[i]) && i != -1; k++) {
        switch (*(argv[i] + k)) {
          case 'i':
            set->i_option = 1;
            break;
          case 'v':
            set->v_option = 1;
            break;
          case 'c':
            set->c_option = 1;
            break;
          case 'l':
            set->l_option = 1;
            break;
          case 'n':
            set->n_option = 1;
            break;
          case 'h':
            set->h_option = 1;
            break;
          case 's':
            set->s_option = 1;
            break;
          case 'o':
            set->o_option = 1;
            break;

          default:
            printf("s21_grep: illegal option -- %s\n", (argv[i] + 1));
            printf("usage: ./s21_grep [-eivclnhsfo] [file ...]");
            *err = 1;
            break;
        }
      }
    }
  }
}
