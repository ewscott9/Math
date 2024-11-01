// Very simple command line arithmatic drills
#include "my_io.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

// returns a random value between a and b 
int rand_range(int a, int b)
{
	if (a > b) {
		int t = a;
		a = b;
		b = t;
	}
	return (rand() % (b - a + 1)) + a;
}

// a prompt for each test. checks negative numbers on b, and puts "()" around them.
void prompt_test(int a, int b, char op, const char* eq)
{
	if (b < 0) printf("%d %c (%d)%s", a, op, b, eq);
	else printf("%d %c %d%s", a, op, b, eq);
}

// addition test
bool test_add(int a, int b)
{
	prompt_test(a, b, '+', " = ");
	return a + b == get_int();
}

// subtraction test
bool test_sub(int a, int b)
{
	prompt_test(a, b, '-', " = ");
	return a - b == get_int();
}

// multiplication test
bool test_mul(int a, int b)
{
	prompt_test(a, b, '*', " = ");
	return a * b == get_int();
}

// calculate the greatest common divisor
int gcd(int a, int b)
{
	a = abs(a);
	b = abs(b);
	if (a < b) {
		int t = a;
		a = b;
		b = t;
	}

	while (b > 0) {
		int r = a % b;
		a = b;
		b = r; 
	}
	return a;
}

// gcd test
bool test_gcd(int a, int b)
{
	printf("gcd(%d, %d) = ", a, b);
	return gcd(a, b) == get_int();
}

// returns thesign (1, -1, or 0) of "a"
int sign(int x)
{
	return (x > 0) - (x < 0);
}

// tests ability to reduce a fraction
int div_reduce(int a, int b)
{
	bool x = true;

	printf("reduce ");
	prompt_test(a, b, '/', ":\n");

	int sign_result = sign(a) * sign(b);
	a = abs(a);
	b = abs(b);
	
	int gcd_ab = gcd(a, b);
	x &= test_gcd(a, b);

	int n;
	if (x) {
		printf("numerator = ");
		n = get_int();
	}

	if (a % b != 0) {
		x &= a / gcd_ab == abs(n);
		int d;
		if (x) {
			printf("denominator = ");
			d = get_int();
		}
		x &= b / gcd_ab == abs(d);
		x &= sign_result == sign(n) * sign(d);

	} else {
		x &= sign_result * a / gcd_ab == n;
	}

	return x;
}

// tests ability to find the remainder of integer division
int div_remainder(int a, int b)
{
	printf("remainder = ");
	return abs(a) % abs(b) == get_int();
}

// tests the ability to find the proper fraction form of a division operation.
int div_proper(int a, int b)
{
	bool x = true;
	int sign_result = sign(a) * sign(b);
	a = abs(a);
	b = abs(b);
	int r = a % b;
	x &= div_remainder(a, b);
	if (a < b) x &= div_reduce(sign_result * r, b);
	else x &= div_reduce(r, b);
	return x;
}

// divison test. Options (0: REMAINDER, 1: PROPER, 2: IMPROPER)
bool test_div(int a, int b, enum div_options option)
{
	// keeps track of the correctness of the answer. 
	bool x = true;

	// if you devide by 0
	if (b == 0) {
		const char* ans = "undefined";
		enum {LEN = 10};
		char s[LEN];
		get_string(s, LEN);
		
		for (int i = 0; i < LEN; i++) x &= ans[i] == s[i];

	// if the reduce only flag is set
	} else if (option == REDUCE) x = div_reduce(a, b);

	// if the remainder or proper flags are set.
	else {
		// get the whole part of the result
		prompt_test(a, b, '/', " (whole part) = ");
		x = (int) (a / b) == get_int();

		if ((x == 0) || (a % b == 0)) ; // do nothing
		else if (option == REMAINDER) x &= div_remainder(a, b);
		else if (option == PROPER) x &= div_proper(a, b);
	}
	return x;
}

// Generates a selection mask
int test_option_mask()
{
	int mask = 0;
	do {
		mask = prompt_int_ranged("Test Addition (1, 0): ", 0, 1);
		mask += prompt_int_ranged("Test Subtraction (1, 0): ", 0, 1) << 1;
		mask += prompt_int_ranged("Test Multiplication (1, 0): ", 0, 1) << 2;
		mask += prompt_int_ranged("Test Division (1, 0): ", 0, 1) << 3;
		mask += prompt_int_ranged("Test GCD (1, 0): ", 0, 1) << 4;
		if (mask == 0) printf("(please select at least one test)\n");
	} while (mask == 0);
	return mask;
}

// saves stats to file
#pragma warning (disable : 4996)
void save_stats(char* stats, char* filename)
{
	FILE* file;
	file = fopen(filename, "a");
	if (file != NULL) {
		fputs(stats, file);
		fclose(file);
	}
}

// Tests users arithmetic skills.
void math_drill(struct settings s, char* name)
{
	int c = 0;
	time_t t = time(NULL);
	time_t t_now = time(NULL);
	int t_delta = 1;
	srand((int)t);

	if (s.questions_max > 0) printf("\nPlease complete the next %d questions:\n", s.questions_max);
	else printf("\n");

	for (bool answer = true; answer == true && (s.questions_max == 0 || c != s.questions_max);) {
		int a = rand_range(s.left_min, s.left_max);
		int b = rand_range(s.right_min, s.right_max);

		//printf("%d, %d, %d, %d, a: %d, b: %d\n", left_min, left_max, right_min, right_max, a, b); // debug

		// randomly choose a test based on option selection
		int op = 0;
		do op = 1 << (rand() % s.question_types); 
		while ((op & s.op_mask) == 0);

		// select the test based on the binary rep of op
		switch (op) {
		case 0b00001: answer = test_add(a, b); break;
		case 0b00010: answer = test_sub(a, b); break;
		case 0b00100: answer = test_mul(a, b); break;
		case 0b01000: answer = test_div(a, b, s.div_options); break;
		case 0b10000: answer = test_gcd(a, b); break;
		}

		if (answer) {
			c++;
			t_now = time(NULL);

			// Let the player know some stats every 10 questions
			t_delta = (int)(t_now - t) + 1;

			if (s.show_stats != 0 && c % s.show_stats == 0) 
				printf(
					"\n%d correct in %d\' %d\". %d answers per minute.\n", 
					c, t_delta / 60, t_delta % 60, 60 * c / t_delta
				);
		}
	}

	// Create a timestamp for today's date
	struct tm* timeinfo = localtime(&t_now);
	char* timestamp = asctime(timeinfo);
	if (timestamp != NULL) timestamp[strlen(timestamp) - 1] = '\0';

	// Save stats to file and print it to cmd
	enum { STATS_SIZE = 400 };
	char stats[STATS_SIZE];
	snprintf(
		stats, STATS_SIZE, 
		"%s: %s: %d correct in %d\' %d\". %d answers per minute.\n", 
		name, timestamp, c, t_delta / 60, t_delta % 60, 60 * c / t_delta
	);
	save_stats(stats, "stats.txt");
	printf("\n");
	fputs(stats, stdout);
}

void init_string(char* string, int string_size) 
{
	for (int i = 0; i < string_size; i++) {
		string[i] = ' ';
	}
	string[string_size - 1] = '\0';
}




void menu() 
{
	printf("Welcome to Math Drills!\n");

	struct settings s;
	s.op_mask = 0b01111; // used to mask specific test types
	s.left_min = -10;
	s.left_max = 10;
	s.right_min = -10;
	s.right_max = 10;
	s.questions_max = 20;
	s.options = 0;
	s.show_stats = 10; // show test stats every "show_stats" questions
	s.div_options = REMAINDER; // default divison test type is proper fractions
	s.question_types = 5;

	enum { NAME_SIZE = 50 };
	char name[NAME_SIZE] = "";
	init_string(name, NAME_SIZE);
	prompt_string("Enter Your Name: ", name, NAME_SIZE);

	enum { QUIT = -1 };
	do {
		printf("(%d) Quit,\t", QUIT);
		s.options = prompt_int(
			"(1) Start,\t(2) Test Options,\n"
			"(3) Min,\t(4) Max,\t(5) Question Limit,\n"
			"(6) Stats,\t(7) Edit Name,\t(8) Division Options: "
		);

		switch (s.options) {
		case QUIT: printf("Goodbye!\n"); break;
		case 1:	
			math_drill(s, name); 
			break;
		case 2: s.op_mask = test_option_mask(); break;
		case 3: s.left_min = prompt_int("Left Min: "); s.right_min = prompt_int("Right Min: "); break;
		case 4: s.left_max = prompt_int("Left Max: "); s.right_max = prompt_int("Right Max: "); break;
		case 5: s.questions_max = prompt_int("Question Limit (0 to disable): "); break;
		case 6: s.show_stats = prompt_int("Show stats after every X questions (0 to disable): "); break;
		case 7: prompt_string("Enter Your Name: ", name, NAME_SIZE); break;
		case 8: 
			s.div_options = prompt_int_ranged(
				"(0) integer with remainder, (1) proper fraction, (2) reduce only: ",
				REMAINDER, REDUCE
			);
			break;
		default: printf("(invalid input)\n");
		}
	} while (s.options != QUIT);
}

int main()
{
	menu();
	return 0;
}

