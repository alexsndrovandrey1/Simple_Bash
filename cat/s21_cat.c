#include "s21_cat.h"

int main(int argc, char *argv[]) {
  Flags flags = {0};
  initialization_of_flags(&flags);
  Data data = {0};
  initialization_of_data(&data);
  if (check_start_programm(argc, argv, &data)) {
    flags_to_structure(&flags, &data);
    print_result(&flags, &data);
  } else {
    fprintf(stderr, "Flags were not valid\n");
  }
  return 0;
}

void print_result(Flags const *flags, Data const *data) {
  for (int index_for_files = 0; index_for_files < data->number_of_files;
       ++index_for_files) {
    if (check_file_exist(index_for_files, data)) {
      int is_previous_newline = TRUE, ordinal = 1;
      int current_character = 0, next_character = 0;
      FILE *file = fopen(data->all_text_files_array[index_for_files], "r");
      next_character = fgetc(file);
      current_character = next_character;
      next_character = fgetc(file);
      while (current_character != EOF) {
        flags_processing(&current_character, &next_character,
                         &is_previous_newline, &ordinal, data, flags, file);
      }
      fclose(file);
    } else {
      fprintf(stderr, "File doesn't exist\n");
    }
  }
}

void flags_processing(int *current_character, int *next_character,
                      int *previous_newline, int *ordinal, Data const *data,
                      Flags const *flags, FILE *file) {
  if (flags->b && *previous_newline) {
    flag_b(*current_character, *previous_newline, data, ordinal);
  }
  if (flags->s && is_newline(*current_character, data) && *previous_newline) {
    flag_s(*current_character, next_character, *previous_newline, data, file);
  }
  // PRICOL
  if (flags->n && !flags->b && *previous_newline) {
    flag_n(ordinal);
  }
  if ((flags->e && is_newline(*current_character, data)) ||
      (flags->e && is_unprintable(*current_character, data))) {
    if (flags->e && is_newline(*current_character, data)) {
      flag_e();
    }
    if (is_unprintable(*current_character, data)) {
      flag_v(current_character, next_character, file, data);
    }
  }
  if (flags->E && is_newline(*current_character, data)) {
    flag_e();
  }
  if (flags->T && is_tabulation(*current_character, data)) {
    flag_t(current_character, next_character, file, data);
  }
  if ((flags->t && is_tabulation(*current_character, data)) ||
      (flags->t && is_unprintable(*current_character, data))) {
    flag_t(current_character, next_character, file, data);
    if (is_unprintable(*current_character, data)) {
      flag_v(current_character, next_character, file, data);
    }
  }
  if (flags->v && is_unprintable(*current_character, data)) {
    flag_v(current_character, next_character, file, data);
  }
  if ((!flags->t || !flags->T) && !is_tabulation(*current_character, data)) {
    printf("%c", *current_character);
  }
  if ((!flags->t && !flags->T) && is_tabulation(*current_character, data)) {
    printf("%c", *current_character);
  }
  *previous_newline = *current_character == '\n' ? TRUE : FALSE;
  *current_character = *next_character;
  *next_character = fgetc(file);
}

int is_newline(const int current_character, Data const *data) {
  int is_newline = FALSE;
  if (current_character == data->newline) {
    is_newline = TRUE;
  }
  return is_newline;
}

int is_tabulation(const int current_character, Data const *data) {
  int is_tabulation = FALSE;
  if (current_character == data->tabulation) {
    is_tabulation = TRUE;
  }
  return is_tabulation;
}

int is_unprintable(const int current_character, Data const *data) {
  int is_unprintable = FALSE;
  if (((current_character >= 0 && current_character < 31) &&
       !is_newline(current_character, data) &&
       !is_tabulation(current_character, data)) ||
      current_character == 127) {
    is_unprintable = TRUE;
  }
  return is_unprintable;
}

void initialization_of_flags(Flags *flags) {
  flags->b = FALSE;
  flags->s = FALSE;
  flags->n = FALSE;
  flags->e = FALSE;
  flags->E = FALSE;
  flags->T = FALSE;
  flags->t = FALSE;
  flags->v = FALSE;
}

void initialization_of_data(Data *data) {
  memset(data->all_flags, '\0', sizeof(char));
  memset(data->all_text_files_array, '\0', sizeof(char));
  data->number_of_files = 0;
  data->newline = '\n';
  data->tabulation = '\t';
}

void flags_to_structure(Flags *flags, Data const *data) {
  int index = 0;
  while (strcmp(data->all_flags[index], "\0")) {
    if (!strcmp(data->all_flags[index], "-b") ||
        !strcmp(data->all_flags[index], "--number-nonblank")) {
      flags->b = TRUE;
    } else if (!strcmp(data->all_flags[index], "-e")) {
      flags->e = TRUE;
    } else if (!strcmp(data->all_flags[index], "-E")) {
      flags->e = TRUE;
    } else if (!strcmp(data->all_flags[index], "-n") ||
               !strcmp(data->all_flags[index], "--number")) {
      flags->n = TRUE;
    } else if (!strcmp(data->all_flags[index], "-s") ||
               !strcmp(data->all_flags[index], "--squeeze-blank")) {
      flags->s = TRUE;
    } else if (!strcmp(data->all_flags[index], "-t")) {
      flags->t = TRUE;
    } else if (!strcmp(data->all_flags[index], "-T")) {
      flags->T = TRUE;
    } else if (!strcmp(data->all_flags[index], "-v")) {
      flags->v = TRUE;
    }
    ++index;
  }
}

void flag_b(const int current_character, const int previous_newline,
            Data const *data, int *ordinal) {
  if (previous_newline && !is_newline(current_character, data)) {
    printf("%6d\t", *ordinal);
    ++*ordinal;
  }
}

void flag_s(const int current_character, int *next_character,
            int previous_newline, Data const *data, FILE *file) {
  while (is_newline(current_character, data) && previous_newline &&
         is_newline(*next_character, data)) {
    previous_newline = current_character == '\n' ? TRUE : FALSE;
    *next_character = fgetc(file);
  }
}

void flag_n(int *ordinal) {
  printf("%6d\t", *ordinal);
  ++*ordinal;
}

void flag_e() { printf("$"); }

void flag_t(int *current_character, int *next_character, FILE *file,
            Data const *data) {
  while (is_tabulation(*current_character, data) ||
         is_tabulation(*next_character, data)) {
    printf("^I");
    if (!is_tabulation(*next_character, data)) {
      return;
    }
    *current_character = *next_character;
    *next_character = fgetc(file);
  }
}

void flag_v(int *current_character, int *next_character, FILE *file,
            Data const *data) {
  while (is_unprintable(*current_character, data)) {
    if (*current_character == 127) {
      *current_character = '?';
    } else {
      *current_character += 64;
    }
    printf("^%c", *current_character);
    if (is_newline(*current_character, data) || *next_character == EOF) {
      return;
    }
    *current_character = *next_character;
    *next_character = fgetc(file);
  }
}

int check_start_programm(const int argc, char *argv[], Data *data) {
  int condition = FALSE;
  if (argc >= 2 && parsing(argc, argv, data)) {
    condition = TRUE;
  }
  return condition;
}

int parsing(const int argc, char *argv[], Data *data) {
  int valid_input = FALSE, letter_index = 0;
  int counter_for_flags = 0;
  for (int element_index = 1; element_index < argc; ++element_index) {
    if (argv[element_index][letter_index] == '-') {
      strcpy(data->all_flags[counter_for_flags], argv[element_index]);
      ++counter_for_flags;
    } else {
      strcpy(data->all_text_files_array[data->number_of_files],
             argv[element_index]);
      ++data->number_of_files;
    }
  }
  if (check_flags_valid(counter_for_flags, data)) {
    valid_input = TRUE;
  }
  return valid_input;
}

int check_flags_valid(const int counter_for_flags, Data const *data) {
  int valid_flags = FALSE, index_all_flags = 0, number_of_valid_flags = 0;
  while (index_all_flags != counter_for_flags) {
    for (int index_possible_flags = 0; index_possible_flags < NUMBER_OF_FLAGS;
         ++index_possible_flags) {
      if (!strcmp(data->all_flags[index_all_flags],
                  possible_flags[index_possible_flags])) {
        ++number_of_valid_flags;
      }
    }
    ++index_all_flags;
  }
  if (counter_for_flags == number_of_valid_flags) {
    valid_flags = TRUE;
  }
  return valid_flags;
}

int check_file_exist(const int filename_index, Data const *data) {
  int files_exist = TRUE;
  FILE *file = NULL;
  if ((file = fopen(data->all_text_files_array[filename_index], "r")) == NULL) {
    files_exist = FALSE;
  } else {
    fclose(file);
  }
  return files_exist;
}
