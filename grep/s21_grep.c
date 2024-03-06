#include "s21_grep.h"

int main(int argc, char *argv[]) {
  Flags flags = {0};
  initialization_flags(&flags);
  Data data = {0};
  initialization_data(&data);
  if (check_start_conditions(argc, argv, &flags, &data)) {
    flags_to_structure(&flags, &data);
    print_result(&flags, &data);
  } else {
    print_error(&data, "Flags were not valid");
  }
  return 0;
}

void print_result(Flags const *flags, Data *data) {
  const int number_of_files = data->number_of_files;
  int run = TRUE;
  if (flags->f) {
    run = check_files_exist(flags, 0, data);
  }
  if (run) {
    int index_for_files = 0;
    for (; index_for_files < number_of_files; ++index_for_files) {
      if (check_files_exist(flags, index_for_files, data)) {
        FILE *file = fopen(data->all_filenames[index_for_files], "r");
        while (parse_line(file, data)) {
          int pattern_index = 0;
          data->line_should_printed = FALSE;
          while (pattern_index < data->pattern_index &&
                 !data->line_should_printed) {
            check_pattern(flags, data, pattern_index);
            ++pattern_index;
          }
          if (data->line_should_printed) {
            print_line(flags, data, pattern_index, index_for_files);
          }
          check_if_last_newline(data);
        }
        print_res_no_line(flags, data, index_for_files);
        print_newline(flags, data, index_for_files);
        reset_num(data);
      } else {
        if (!flags->s) {
          print_error(data, "File doesn't exist");
        }
      }
    }
  }
}

int check_start_conditions(const int argc, char *argv[], Flags *flags,
                           Data *data) {
  int conditions = FALSE;
  if (argc >= 4 && parse_flags_pattern_filename(argv, flags, data))
    conditions = TRUE;
  return conditions;
}

void parse_pattern(Data *data, char *argv[], int *element_index) {
  strcpy(data->pattern_array[data->pattern_index], argv[*element_index]);
  ++data->pattern_index;
  ++*element_index;
}

void parse_filenames(Data *data, char *argv[], int *element_index) {
  while (argv[*element_index]) {
    strcpy(data->all_filenames[data->number_of_files], argv[*element_index]);
    ++data->number_of_files;
    ++*element_index;
  }
}

void check_pattern(Flags const *flags, Data *data, const int pattern_index) {
  if (flags->i) {
    flag_i(flags, data, pattern_index);
  }
  flag_e(data, pattern_index);
  if (flags->v) {
    flag_v(data);
  }
  if (data->line_should_printed) {
    ++data->number_of_matching_lines;
  }
}

void parse_pattern_flags_f(Flags const *flags, Data *data) {
  FILE *file = NULL;
  if (!(file = fopen(data->pattern_file, "r"))) {
    print_error(data, "Pattern file doesn't exist");
  } else {
    char temp_pattern_array[MAX_LENGHT_OF_PATTERN];
    memset(temp_pattern_array, 0, MAX_LENGHT_OF_PATTERN * sizeof(char));
    int char_index = 0, pattern_array_index = 0, current_character = 0;
    pattern_array_index = flags->e ? 1 : 0;
    while (current_character != EOF) {
      current_character = fgetc(file);
      if (current_character != data->newline) {
        temp_pattern_array[char_index] = current_character;
        ++char_index;
      }
      if (current_character == data->newline) {
        strcpy(data->pattern_array[pattern_array_index], temp_pattern_array);
        memset(temp_pattern_array, 0, MAX_LENGHT_OF_PATTERN * sizeof(char));
        char_index = 0;
        ++pattern_array_index;
        ++data->pattern_index;
      }
    }
  }
  data->is_prefious_flag_f = FALSE;
  fclose(file);
}

void print_error(Data const *data, char *error_mes) {
  if (!data->error_mes_should_printed) {
    fprintf(stderr, "%s\n", error_mes);
  }
}

int parse_flags_pattern_filename(char *argv[], Flags *flags, Data *data) {
  int is_valid_input = TRUE;
  int count_for_flags = 0;
  int element_index = 1;
  parse_flags(argv, flags, data, &count_for_flags, &element_index,
              &is_valid_input);
  if (!data->is_prefious_flag_f) {
    if (data->pattern_array[0][0] == 0) {
      parse_pattern(data, argv, &element_index);
    }
  }
  parse_flags(argv, flags, data, &count_for_flags, &element_index,
              &is_valid_input);
  parse_filenames(data, argv, &element_index);
  if (flags->f) parse_pattern_flags_f(flags, data);
  return is_valid_input;
}

int check_flag_valid(char *flag_array) {
  int flags_are_valid = FALSE, index_possible_flags = 0;
  while (index_possible_flags < NUM_FLAGS && !flags_are_valid) {
    if (!strcmp(flag_array, possible_flags[index_possible_flags])) {
      flags_are_valid = TRUE;
    }
    ++index_possible_flags;
  }
  return flags_are_valid;
}

void parse_flags(char *argv[], Flags *flags, Data *data, int *counter_for_flags,
                 int *element_index, int *is_valid_input) {
  int letter_index = 0, is_previous_flag_e = FALSE, first_element = 0;
  char single_flag[MAX_LENGHT_OF_FLAGS];
  memset(single_flag, 0, MAX_LENGHT_OF_FLAGS * sizeof(char));
  while (argv[*element_index][letter_index] == '-') {
    ++letter_index;
    //перебираем буквы флагов если они написаны в одно слово
    while (argv[*element_index][letter_index] && is_valid_input) {
      single_flag[first_element] = argv[*element_index][letter_index];
      *is_valid_input = check_flag_valid(single_flag);
      strcpy(data->all_flags[*counter_for_flags], single_flag);
      ++*counter_for_flags;
      ++letter_index;
      if (single_flag[first_element] == 'e') {
        is_previous_flag_e = TRUE;
        flags->e = TRUE;
      }
      if (single_flag[first_element] == 'f') {
        data->is_prefious_flag_f = TRUE;
        flags->f = TRUE;
      }
    }
    ++*element_index;
    //паттер копи в массив если предыдущий флаг -е
    if (is_previous_flag_e) {
      parse_pattern(data, argv, element_index);
      is_previous_flag_e = FALSE;
    }
    if (data->is_prefious_flag_f) {
      ++*element_index;
      strcpy(data->pattern_file, argv[*element_index]);
    }
    letter_index = 0;
  }
}

void flag_i(Flags const *flags, Data *data, const int pattern_index) {
  int line_lenght = 0, pattern_lenght = 0;
  line_lenght = strlen(data->line_array);
  pattern_lenght = strlen(data->pattern_array[pattern_index]);
  memset(data->line_array_copy, 0, MAX_LENGHT_OF_LINE * sizeof(char));
  for (int index = 0; index <= line_lenght; ++index) {
    data->line_array_copy[index] = tolower(data->line_array[index]);
  }
  for (int index = 0; index <= pattern_lenght; ++index) {
    data->pattern_array[pattern_index][index] =
        tolower(data->pattern_array[pattern_index][index]);
  }
  if (flags->i && flags->o) {
    get_inverted_mathed_parts(data, pattern_index);
  }
}

void get_inverted_mathed_parts(Data *data, const int pattern_index) {
  int line_lenght = 0, pattern_lenght = 0, temp_array_index = 0,
      inverted_index = 0, maches = 0;
  char temp_array[MAX_LENGHT_OF_LINE];
  memset(temp_array, 0, MAX_LENGHT_OF_LINE * sizeof(char));
  line_lenght = strlen(data->line_array);
  pattern_lenght = strlen(data->pattern_array[pattern_index]);
  for (int l_index = 0; l_index < line_lenght; ++l_index) {
    for (int p_index = 0; p_index < pattern_lenght; ++p_index) {
      if (data->pattern_array[pattern_index][p_index] ==
          data->line_array_copy[l_index]) {
        temp_array[temp_array_index] = data->line_array[l_index];
        ++temp_array_index;
        ++maches;
        if (p_index != pattern_lenght - 1) {
          ++l_index;
        }
        if (p_index == pattern_lenght - 1 && maches >= pattern_lenght) {
          strcpy(data->inverted_matched_parts[inverted_index], temp_array);
          ++inverted_index;
        }
      } else {
        maches = 0;
      }
    }
    memset(temp_array, 0, MAX_LENGHT_OF_LINE * sizeof(char));
    temp_array_index = 0;
  }
}

void flag_o(Flags const *flags, Data *data, int pattern_index) {
  if (flags->o && !flags->v && !flags->i) {
    for (int index = 0; index < data->pattern_found_in_line; ++index) {
      printf("%s\n", data->pattern_array[pattern_index - 1]);
      data->flag_o_printed_newline = TRUE;
    }
  }
  if (flags->o && !flags->v && flags->i) {
    for (int index = 0; index < data->pattern_found_in_line; ++index) {
      printf("%s\n", data->inverted_matched_parts[index]);
      data->flag_o_printed_newline = TRUE;
    }
  }
  if (flags->o && flags->v && flags->i) {
    printf("%s", data->line_array);
  }
}

void flag_h(Flags const *flags, int *filenames_should_printed) {
  if (flags->h) {
    *filenames_should_printed = !*filenames_should_printed;
  }
}

void flag_v(Data *data) {
  data->line_should_printed = !data->line_should_printed;
  if (!data->pattern_found_in_file && data->line_should_printed) {
    data->pattern_found_in_file = TRUE;
  }
}

void flag_c(Flags const *flags, Data *data) {
  if (flags->c && !flags->l) {
    print_match_lines(data);
  }
}

void print_match_lines(Data const *data) {
  printf("%d\n", data->number_of_matching_lines);
}

void flag_l(Flags const *flags, const int index_for_files, Data const *data) {
  if (flags->l) {
    if (data->pattern_found_in_file) {
      print_filename(index_for_files, data, data->newline);
    }
  }
}

void print_filename(const int index_for_files, Data const *data, char symbol) {
  printf("%s%c", data->all_filenames[index_for_files], symbol);
}

void flag_n(Flags const *flags, const int line_number) {
  if (flags->n) {
    print_number_line(line_number);
  }
}

void print_number_line(const int line_number) { printf("%d:", line_number); }

void print_res_no_line(Flags const *flags, Data *data,
                       const int index_for_files) {
  if (flags->c || flags->l) {
    flag_h(flags, &data->filename_should_printed);
    if (!flags->h && !flags->l) {
      filename_should_printed(flags, data);
      if (data->filename_should_printed) {
        print_filename(index_for_files, data, data->colon);
      }
    }
    flag_c(flags, data);
    flag_l(flags, index_for_files, data);
  }
}

void check_if_last_newline(Data *data) {
  data->is_last_newline = data->line_array[0] == '\n' ? TRUE : FALSE;
}

void flag_e(Data *data, const int pattern_index) {
  data->line_should_printed = strings_comparison(data, pattern_index);
}

void print_line(Flags const *flags, Data *data, int pattern_index,
                int index_for_files) {
  if (!flags->c && !flags->l) {
    filename_should_printed(flags, data);
    if (data->filename_should_printed) {
      print_filename(index_for_files, data, data->colon);
    }
    flag_n(flags, data->number_of_line);
    flag_o(flags, data, pattern_index);
    print_line_array(flags, data);
  }
}

void print_newline(Flags const *flags, Data const *data,
                   const int index_for_files) {
  int last_file_index = data->number_of_files - 1;
  int is_last_file = index_for_files == last_file_index ? TRUE : FALSE;
  if (!flags->c && !flags->l) {
    if (!data->is_last_newline && !is_last_file && data->line_should_printed &&
        !data->flag_o_printed_newline) {
      printf("%c", data->newline);
    }
  }
  if (flags->v && !flags->c && !flags->l) {
    printf("\n");
  }
}

void print_line_array(Flags const *flags, Data const *data) {
  if (!flags->o) {
    printf("%s", data->line_array);
  }
}

void filename_should_printed(Flags const *flags, Data *data) {
  if (!flags->h) {
    data->filename_should_printed = data->number_of_files > 1 ? TRUE : FALSE;
  }
}

int strings_comparison(Data *data, const int pattern_index) {
  int are_equal = FALSE;
  regex_t reegex;
  regcomp(&reegex, data->pattern_array[pattern_index], 0);
  int compare_res = 0;
  compare_res = regexec(&reegex, data->line_array_copy, 0, NULL, 0);
  if (!compare_res) {
    are_equal = TRUE;
    if (!data->pattern_found_in_file) {
      data->pattern_found_in_file = TRUE;
    }
  }
  regfree(&reegex);
  pattern_in_line(data, pattern_index);
  return are_equal;
}

void pattern_in_line(Data *data, int pattern_index) {
  int number_of_occurrences = 0, i_two = 0;
  int line_lenght = (int)strlen(data->line_array_copy);
  int pattern_lenght = (int)strlen(data->pattern_array[pattern_index]);
  for (int i_one = 0; i_one <= line_lenght; ++i_one) {
    if (data->line_array_copy[i_one] ==
        data->pattern_array[pattern_index][i_two]) {
      while (data->line_array_copy[i_one] ==
                 data->pattern_array[pattern_index][i_two] &&
             i_two < pattern_lenght) {
        ++i_one;
        if (!(i_two == (line_lenght - 1))) {
          ++i_two;
        }
        if (data->line_array_copy[i_one] ==
                data->pattern_array[pattern_index][i_two] &&
            (i_two == pattern_lenght - 1)) {
          ++number_of_occurrences;
        }
      }
      i_two = 0;
    }
  }
  data->pattern_found_in_line = number_of_occurrences;
}

void reset_line(Data *data) {
  memset(data->line_array, 0, MAX_LENGHT_OF_LINE * sizeof(char));
  memset(data->line_array_copy, 0, MAX_LENGHT_OF_LINE * sizeof(char));
  memset(data->inverted_matched_parts, 0, sizeof(char));
}

void reset_num(Data *data) {
  data->number_of_line = 0;
  data->number_of_matching_lines = 0;
  data->pattern_found_in_file = FALSE;
  data->is_last_newline = FALSE;
  data->flag_o_printed_newline = FALSE;
}

int parse_line(FILE *file, Data *data) {
  reset_line(data);
  ++data->number_of_line;
  int can_be_parsed = TRUE, going_to_be_printed = FALSE, index = 0,
      current_character = 0;
  while (can_be_parsed && current_character != EOF) {
    current_character = fgetc(file);
    if (current_character == EOF || current_character == data->newline) {
      can_be_parsed = FALSE;
      if (current_character == data->newline) {
        data->line_array[index] = current_character;
        ++index;
      }
    } else {
      data->line_array[index] = current_character;
      ++index;
    }
  }
  if (index) {
    going_to_be_printed = TRUE;
  }
  memcpy(data->line_array_copy, data->line_array, index);
  return going_to_be_printed;
}

void flags_to_structure(Flags *flags, Data const *data) {
  int index = 0;
  while (strcmp(data->all_flags[index], "\0")) {
    if (!strcmp(data->all_flags[index], "-e") ||
        !strcmp(data->all_flags[index], "e")) {
      flags->e = TRUE;
    } else if (!strcmp(data->all_flags[index], "-i") ||
               !strcmp(data->all_flags[index], "i")) {
      flags->i = TRUE;
    } else if (!strcmp(data->all_flags[index], "-v") ||
               !strcmp(data->all_flags[index], "v")) {
      flags->v = TRUE;
    } else if (!strcmp(data->all_flags[index], "-c") ||
               !strcmp(data->all_flags[index], "c")) {
      flags->c = TRUE;
    } else if (!strcmp(data->all_flags[index], "-l") ||
               !strcmp(data->all_flags[index], "l")) {
      flags->l = TRUE;
    } else if (!strcmp(data->all_flags[index], "-n") ||
               !strcmp(data->all_flags[index], "n")) {
      flags->n = TRUE;
    } else if (!strcmp(data->all_flags[index], "-h") ||
               !strcmp(data->all_flags[index], "h")) {
      flags->h = TRUE;
    } else if (!strcmp(data->all_flags[index], "-s") ||
               !strcmp(data->all_flags[index], "s")) {
      flags->s = TRUE;
    } else if (!strcmp(data->all_flags[index], "-o") ||
               !strcmp(data->all_flags[index], "o")) {
      flags->o = TRUE;
    }
    ++index;
  }
}

int check_files_exist(Flags const *flags, const int filename_index,
                      Data const *data) {
  int files_exist = TRUE;
  FILE *file = NULL;
  if (flags->f) {
    if ((file = fopen(data->pattern_file, "r")) == NULL) {
      files_exist = FALSE;
    } else {
      fclose(file);
    }
  } else {
    if ((file = fopen(data->all_filenames[filename_index], "r")) == NULL) {
      files_exist = FALSE;
    } else {
      fclose(file);
    }
  }
  return files_exist;
}

void initialization_flags(Flags *flags) {
  flags->e = FALSE;
  flags->i = FALSE;
  flags->v = FALSE;
  flags->c = FALSE;
  flags->l = FALSE;
  flags->n = FALSE;
  flags->h = FALSE;
  flags->s = FALSE;
  flags->f = FALSE;
  flags->o = FALSE;
}

void initialization_data(Data *data) {
  memset(data->all_flags, 0, sizeof data->all_flags);
  memset(data->all_filenames, 0, sizeof data->all_filenames);
  memset(data->all_filenames, 0, sizeof data->all_filenames);  // -
  memset(data->pattern_file, 0, MAX_LENGHT_OF_FILENAME * sizeof(char));
  data->pattern_index = 0;
  memset(data->line_array, 0, MAX_LENGHT_OF_LINE * sizeof(char));
  memset(data->line_array_copy, 0, MAX_LENGHT_OF_LINE * sizeof(char));
  memset(data->inverted_matched_parts, 0, sizeof(data->inverted_matched_parts));
  data->number_of_files = 0;
  data->number_of_line = 0;
  data->flag_o_printed_newline = FALSE;
  data->newline = '\n';
  data->colon = ':';
  data->line_should_printed = FALSE;
  data->filename_should_printed = FALSE;
  data->number_of_matching_lines = 0;
  data->pattern_found_in_file = FALSE;
  data->pattern_found_in_line = 0;
  data->is_last_newline = FALSE;
  data->is_prefious_flag_f = FALSE;
}