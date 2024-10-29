#ifndef MATH_H_
#define MATH_H_

#include <stdbool.h>

enum div_options { REMAINDER, PROPER, REDUCE };

struct settings
{
	int op_mask; // used to mask specific test types
	int left_min;
	int left_max;
	int right_min;
	int right_max;
	int questions_max;
	int options;
	int show_stats; // show test stats every "show_stats" questions
	int div_options; // default divison test type is proper fractions
	int question_types;
};

int rand_range(int a, int b);
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
bool test_div(int a, int b, enum div_options option);
void save_stats(char* stats, char* filename);
void math_drill(struct settings s, char* name);
void init_string(char* string, int string_size);
void menu();

#endif
