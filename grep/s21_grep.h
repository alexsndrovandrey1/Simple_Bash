#ifndef SRC_GREP_S21_GREP_H
#define SRC_GREP_S21_GREP_H

#include <ctype.h>  //для работы с символами(регистр)
#include <regex.h>  //для работы с регулярными выражениями
#include <stdio.h>
#include <string.h>

#define NUM_FLAGS 11
#define MAX_LENGHT_OF_FLAGS 5
#define NUM_FILENAME 100
#define MAX_LENGHT_OF_FILENAME 200
#define MAX_NUM_OF_PATTERNS 100
#define MAX_LENGHT_OF_PATTERN 100
#define MAX_LENGHT_OF_LINE 500
#define MAX_LINES_IN_FILE 500

enum true_false { FALSE, TRUE };

static const char possible_flags[NUM_FLAGS][MAX_LENGHT_OF_FLAGS] = {
    "e", "i", "v", "c", "l", "n", "h", "s", "f", "o", "NULL"};

typedef struct Flags {
  int e;  // шаблон
  int i;  // игнорим регистр
  int v;  // перевернутое совпаение
  int c;  // выводим только кол-во совпадающих строк
  int l;  // выводим только соответствующие файлы
  int n;  // перед каждым совпадением ставится номер строки
  int h;  // выводим совпадающие строки без предшествующих им имен файловн
  int s;  // не выводи ошибки если они есть
  int f;  // регулярки из файла
  int o;  // выводим совпадающие части совпадающей строки
} Flags;

typedef struct Data {
  // Массив, чтобы сохранить все флаги действительными и недействительными,
  // чтобы отсортировать их позже
  char all_flags[NUM_FLAGS][MAX_LENGHT_OF_FLAGS];
  // Массив для имен файлов действительный и недействительный
  char all_filenames[NUM_FILENAME][MAX_LENGHT_OF_FILENAME];
  // Массив для хранени имени файла паттерна для -f
  char pattern_file[MAX_LENGHT_OF_FILENAME];
  int is_prefious_flag_f;
  // Массив для хранения шаблонов для -e и -f
  char pattern_array[MAX_NUM_OF_PATTERNS][MAX_LENGHT_OF_PATTERN];
  int pattern_index;
  // Массив для хранения текущей строки
  char line_array[MAX_LENGHT_OF_LINE];
  char line_array_copy[MAX_LENGHT_OF_LINE];
  char inverted_matched_parts[MAX_LENGHT_OF_LINE][MAX_LINES_IN_FILE];
  int number_of_files;
  int line_should_printed;
  int filename_should_printed;
  int error_mes_should_printed;
  int number_of_matching_lines;
  int number_of_line;
  int flag_o_printed_newline;
  int pattern_found_in_file;
  int pattern_found_in_line;
  int is_last_newline;
  char newline, colon;
} Data;

void initialization_flags(Flags *flags);
void initialization_data(Data *data);
void print_result(Flags const *flags, Data *data);
int check_start_conditions(const int argc, char *argv[], Flags *flags,
                           Data *data);
void parse_pattern(Data *data, char *argv[], int *element_index);
void parse_filenames(Data *data, char *argv[], int *element_index);
void check_pattern(Flags const *flags, Data *data, const int pattern_index);
void parse_pattern_flags_f(Flags const *flags, Data *data);
void print_error(Data const *data, char *error_mes);
int parse_flags_pattern_filename(char *argv[], Flags *flags, Data *data);
int check_flag_valid(char *flag_array);
void parse_flags(char *argv[], Flags *flags, Data *data, int *counter_for_flags,
                 int *element_index, int *is_valid_input);
void flag_i(Flags const *flags, Data *data, const int pattern_index);
void get_inverted_mathed_parts(Data *data, const int pattern_index);
void flag_o(Flags const *flags, Data *data, int pattern_index);
void flag_h(Flags const *flags, int *filenames_should_printed);
void flag_v(Data *data);
void flag_c(Flags const *flags, Data *data);
void print_match_lines(Data const *data);
void flag_l(Flags const *flags, const int index_for_files, Data const *data);
void print_filename(const int index_for_files, Data const *data, char symbol);
void flag_n(Flags const *flags, const int line_number);
void print_number_line(const int line_number);
void print_res_no_line(Flags const *flags, Data *data,
                       const int index_for_files);
void check_if_last_newline(Data *data);
void flag_e(Data *data, const int pattern_index);
void print_line(Flags const *flags, Data *data, int pattern_index,
                int index_for_files);
void print_newline(Flags const *flags, Data const *data,
                   const int index_for_files);
void print_line_array(Flags const *flags, Data const *data);
void filename_should_printed(Flags const *flags, Data *data);
int strings_comparison(Data *data, const int pattern_index);
void pattern_in_line(Data *data, int pattern_index);
void reset_line(Data *data);
void reset_num(Data *data);
int parse_line(FILE *file, Data *data);
void flags_to_structure(Flags *flags, Data const *data);
int check_files_exist(Flags const *flags, const int filename_index,
                      Data const *data);

#endif