#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdio.h>
#include <string.h>

#define NUMBER_OF_FLAGS 12
#define MAX_LENGHT_OF_FLAGS 18
#define NMAX 30

enum true_false { FALSE, TRUE };

static const char possible_flags[NUMBER_OF_FLAGS][MAX_LENGHT_OF_FLAGS] = {
    "-b", "--number-nonblank", "-e", "-E", "-n",   "--number",
    "-s", "--squeeze-blank",   "-t", "-T", "NULL", "-v"};

typedef struct Flags {
  int b;
  int s;
  int n;
  int e;
  int E;
  int T;
  int t;
  int v;
} Flags;

typedef struct Data {
  char all_flags[NMAX][NMAX];
  char all_text_files_array[NMAX][NMAX];
  int number_of_files;
  char newline;
  char tabulation;
} Data;

void print_result(Flags const *flags, Data const *data);
void flags_processing(int *current_character, int *next_character,
                      int *previous_newline, int *ordinal, Data const *data,
                      Flags const *flags, FILE *file);
int is_newline(const int current_character, Data const *data);
int is_unprintable(const int current_character, Data const *data);
int is_tabulation(const int current_character, Data const *data);
void initialization_of_flags(Flags *flags);
void initialization_of_data(Data *data);
void flags_to_structure(Flags *flags, Data const *data);
void flag_b(const int current_character, const int previous_newline,
            Data const *data, int *ordinal);
void flag_s(const int current_character, int *next_character,
            int previous_newline, Data const *data, FILE *file);
void flag_n(int *ordinal);
void flag_e();
void flag_t(int *current_character, int *next_character, FILE *file,
            Data const *data);
void flag_v(int *current_character, int *next_character, FILE *file,
            Data const *data);
int check_start_programm(const int argc, char *argv[], Data *data);
int parsing(const int argc, char *argv[], Data *data);
int check_flags_valid(const int counter_for_flags, Data const *data);
int check_file_exist(const int filename_index, Data const *data);

#endif  // SRC_CAT_S21_CAT_H_