#include "my_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

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
	enum { READ_SIZE = 20};

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

