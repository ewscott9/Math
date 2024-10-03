#ifndef MATH_H_
#define MATH_H_

#include <stdbool.h>

void prompt_test(int a, int b, char op, const char* eq);
bool test_add(int a, int b);
bool test_sub(int a, int b);
bool test_mul(int a, int b);
int gcd(int a, int b);
bool test_gcd(int a, int b);
int sign(int a);
int div_reduce(int a, int b);
int div_remainder(int a, int b);
int div_proper(int a, int b);
enum div_options;
bool test_div(int a, int b, enum div_options option);
void save_stats(char* stats, char* filename);
void math_drill(
	int op_mask, int question_types, char* name, 
	int left_min, int left_max, int right_min, 
	int right_max, int questions_max, int show_stats,
	int div_options
);
void init_string(char* string, int string_size);
void menu();

#endif
