// Very simple command line arithmatic drills
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

// constants used for file saves
enum IO_Const
{
	MAX_READ = 20,
	MAX_WRITE = 200
};

// clears stdin
// used after getting numbers from stdin, incase user accidentally entered non numeric characters after
void clear_stdin()
{
	char c = '\0';
	do c = getchar(); while (c != EOF && c != '\n');
}

// get a string from input
void get_string(char* out_string, int out_string_size)
{
	fgets(out_string, out_string_size, stdin);
	int len = (int) strlen(out_string);
	if (out_string[len - 1] == '\n') out_string[len - 1] = '\0';
	else clear_stdin();
}

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


// gets int value, and clears stdin afterwards
// if input starts with a number, reads input until READ_SIZE - 1 characters or a non number and returns the number or LONG_MIN or LONG_MAX if number was too large
// if input starts with a letter, returns INT_MIN.
int get_int()
{
	enum { READ_SIZE = MAX_READ };

	// grab characters from stdin, if you can't get them all in one go, throw the rest away.
	char text[READ_SIZE] = "";
	fgets(text, READ_SIZE, stdin);

	// if the text that was read in doesn't start with a number, just return INT_MIN
	int value = INT_MIN;
	bool negitive_number = false;
	bool positive_number = false;

	if (strlen(text) >= 2) negitive_number = (text[0] == '-' && text[1] >= '0' && text[1] <= '9');
	positive_number = text[0] >= '0' && text[0] <= '9';

	if (positive_number || negitive_number) {
		if (text[strlen(text) - 1] != '\n') clear_stdin();
		// if theres a number at the front of the string return that value
		char* pend;
		value = (int)strtol(text, &pend, 0);
		if (value == 0 && pend[0] != '\n') value = INT_MIN;
	}
	return value;
}

// prints a message, receives an input, and returns a number.
int prompt_int(const char* text)
{
	fputs(text, stdout);
	return get_int();
}

// prints a message, receives an input, and returns a number.
int prompt_int_ranged(const char* text, int min, int max)
{
	fputs(text, stdout);
	int value = get_int();
	while (value < min || value > max) {
		printf("(invalid input)\n");
		fputs(text, stdout);
		value = get_int();
	}
	return value;
}

// prints a message, receives an input, and returns a number.
void prompt_string(const char* prompt, char* out_string, int out_string_size)
{
	fputs(prompt, stdout);
	get_string(out_string, out_string_size);
}

void prompt_test(int a, int b, char op, char eq)
{
	if (b < 0) printf("%d %c (%d) %c ", a, op, b, eq);
	else printf("%d %c %d %c ", a, op, b, eq);
}
// addition test
bool test_add(int a, int b)
{
	prompt_test(a, b, '+', '=');
	return a + b == get_int();
}

// subtraction test
bool test_sub(int a, int b)
{
	prompt_test(a, b, '-', '=');
	return a - b == get_int();
}

// multiplication test
bool test_mul(int a, int b)
{
	prompt_test(a, b, '*', '=');
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
int sign(int a)
{
	return (a > 0) - (a < 0);
}

int div_reduce(int a, int b)
{
	bool x = true;

	printf("reduce");
	prompt_test(a, b, '/', ':');
	int sign_result = sign(a) * sign(b);
	a = abs(a);
	b = abs(b);
	
	int gcd_ab = gcd(a, b);
	x &= test_gcd(a, b);

	printf("numerator = ");
	int n = get_int();
	if (a % b != 0) {
		printf("denominator = ");
		int d = get_int();
		x &= a / gcd_ab == abs(n);
		x &= b / gcd_ab == abs(d);
		x &= sign_result == sign(n) * sign(d);
	} else x &= sign_result * a / gcd_ab == n;

	return x;
}

int div_remainder(int a, int b)
{
	printf("remainder = ");
	return abs(a) % abs(b) == get_int();
}

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

// divison test. Options (0: remainder, 1: proper, 2: improper)
bool test_div(int a, int b, int div_options)
{
	// keeps track of the correctness of the answer. 
	bool x = true;

	// if you devide by 0
	if (b == 0) {
		const char * ans = "undefined";
		enum {LEN = 10};
		char s[LEN];
		get_string(s, LEN);
		
		for (int i = 0; i < LEN; i++) x &= ans[i] == s[i];

	// if the reduce only flag is set
	} else if (div_options == 2) x = div_reduce(a, b);

	// if the remainder or proper flags are set.
	else {
		// get the whole part of the result
		prompt_test(a, b, '/', '=');
		printf("(whole part) ");
		x = (int) (a / b) == get_int();

		if ((x == 0) || (a % b == 0)) ; // do nothing
		else if (div_options == 0) x &= div_remainder(a, b);
		else if (div_options == 1) x &= div_proper(a, b);
	}
	return x;
}

// Generates a selection mask
int test_option()
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
void save_stats(char stats[], char filename[])
{
	FILE* file;
	file = fopen(filename, "a");
	if (file != NULL) {
		fputs(stats, file);
		fclose(file);
	}
}

// Tests users arithmetic skills.
void math_drill(
	int op_mask, int question_types, char* name, 
	int left_min, int left_max, int right_min, 
	int right_max, int questions_max, int show_stats,
	int div_options
)
{
	int c = 0;
	time_t t = time(NULL);
	time_t t_now = time(NULL);
	int t_delta = 1;
	srand((int)t);

	if (questions_max > 0) printf("\nPlease complete the next %d questions:\n", questions_max);
	else printf("\n");

	for (bool answer = true; answer == true && (questions_max == 0 || c != questions_max);) {
		int a = rand_range(left_min, left_max);
		int b = rand_range(right_min, right_max);

		//printf("%d, %d, %d, %d, a: %d, b: %d\n", left_min, left_max, right_min, right_max, a, b); // debug

		// randomly choose a test based on option selection
		int op = 0;
		do op = 1 << (rand() % question_types); 
		while ((op & op_mask) == 0);

		switch (op) {
		case 0b00001: answer = test_add(a, b); break;
		case 0b00010: answer = test_sub(a, b); break;
		case 0b00100: answer = test_mul(a, b); break;
		case 0b01000: answer = test_div(a, b, div_options); break;
		case 0b10000: answer = test_gcd(a, b); break;
		}

		if (answer) {
			c++;
			t_now = time(NULL);

			// Let the player know some stats every 10 questions
			t_delta = (int)(t_now - t) + 1;

			if (show_stats != 0 && c % show_stats == 0) 
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

int main()
{
	printf("Welcome to Math Drills!\n");
	int op_mask = 0b01111;
	int left_min = -10;
	int left_max = 10;
	int right_min = -10;
	int right_max = 10;
	int questions_max = 20;
	int options = 0;
	int show_stats = 10;
	int div_options = 1;
	enum { 
		NAME_SIZE = 50,
		QUESTION_TYPES = 5,
		QUIT = -1
	};
	char name[NAME_SIZE] = "";
	init_string(name, NAME_SIZE);
		
	prompt_string("Enter Your Name: ", name, NAME_SIZE);
	do {
		options = prompt_int(
			"(-1) Quit,	(1) Start,	(2) Test Options,\n"
			"(3) Min,	(4) Max,	(5) Question Limit,\n"
			"(6) Stats,	(7) Edit Name,	(8) Division Options: "
		);

		switch (options) {
		case QUIT: printf("Goodbye!\n"); break;
		case 1:	
			math_drill(
				op_mask, QUESTION_TYPES, name, 
				left_min, left_max, right_min, 
				right_max, questions_max, show_stats,
				div_options
			); 
			break;
		case 2: op_mask = test_option(); break;
		case 3: left_min = prompt_int("Left Min: "); right_min = prompt_int("Right Min: "); break;
		case 4: left_max = prompt_int("Left Max: "); right_max = prompt_int("Right Max: "); break;
		case 5: questions_max = prompt_int("Question Limit (0 to disable): "); break;
		case 6: show_stats = prompt_int("Show stats after every X questions (0 to disable): "); break;
		case 7: prompt_string("Enter Your Name: ", name, NAME_SIZE); break;
		case 8: 
			div_options = prompt_int_ranged(
				"(0) integer with remainder, (1) proper fraction, (2) reduce only: ",
				0,2
			);
			break;
		default: printf("(invalid input)\n");
		}
	} while (options != QUIT);
}
