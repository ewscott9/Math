// Very simple command line arithmatic drills
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

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

void get_string(char* out_string, int out_string_size)
{
    fgets(out_string, out_string_size, stdin);
    int len = (int) strlen(out_string);
    if (out_string[len - 1] == '\n') out_string[len - 1] = '\0';
    else clear_stdin();
}


// gets int value, and clears stdin afterwards
// if input starts with a number, reads input until READ_SIZE - 1 characters or a non number and returns the number or LONG_MIN or LONG_MAX if number was too large
// if input starts with a letter, returns LONG_MIN.
int get_int()
{
    enum { READ_SIZE = MAX_READ };

    // grab characters from stdin, if you can't get them all in one go, throw the rest away.
    char text[READ_SIZE] = "";
    fgets(text, READ_SIZE, stdin);

    // if the text that was read in doesn't start with a number, just return LONG_MIN
    int value = INT_MIN;
    if (text[0] >= '0' && text[0] <= '9')
    {
        if (text[strlen(text) - 1] != '\n') clear_stdin();
        // if theres a number at the front of the string return that value
        char* pend;
        value = (int)strtol(text, &pend, 0);
        if (value == 0 && pend[0] != '\n') value = INT_MIN;
        //printf("%i ", value);
    }
    return value;
}

// gets int value and can set max and min value that int can be.
int get_int_ranged(int min, int max)
{
    int value = get_int();
    if (value < min) value = min;
    if (value > max) value = max;
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
    return get_int_ranged(min, max);
}

// prints a message, receives an input, and returns a number.
void prompt_string(const char* prompt, char* out_string, int out_string_size)
{
    fputs(prompt, stdout);
    get_string(out_string, out_string_size);
}

// Following functions test arithmetic add, sub, mul, div
// they return true or false depending on the correctness of the user input
bool test_add(int a, int b)
{
    printf("%i + %i = ", a, b);
    return a + b == get_int();
}

bool test_sub(int a, int b)
{
    if (a < b)
    {
        int t = a;
        a = b;
        b = t;
    }
    printf("%i - %i = ", a, b);
    return a - b == get_int();
}

// calculate the greatest common divisor
int gcd(int a, int b)
{
    if (a < b)
    {
        int t = b;
        b = a;
        a = t;
    }

    while (b > 0)
    {
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

bool test_gcd(int a, int b)
{
    printf("GCD(%i, %i) = ", a, b);
    return gcd(a, b) == get_int();
}

bool test_mul(int a, int b)
{
    printf("%i * %i = ", a, b);
    return a * b == get_int();
}

bool test_div(int a, int b)
{
    bool x = false;
    if (a < b)
    {
        int t = a;
        a = b;
        b = t;
    }

    printf("%i / %i = ", a, b);
    x = (a / b == get_int());

    if (x && a % b != 0)
    {
        printf("%i mod %i = ", a, b);;
        x &= (a % b == get_int());
    }
    return x;
}

// Generates a selection mask
int test_option()
{
    int mask = 0;
    do
    {
        mask = prompt_int_ranged("Test Addition (1, 0): ", 0, 1);
        mask += prompt_int_ranged("Test Subtraction (1, 0): ", 0, 1) << 1;
        mask += prompt_int_ranged("Test Multiplication (1, 0): ", 0, 1) << 2;
        mask += prompt_int_ranged("Test Division (1, 0): ", 0, 1) << 3;
        mask += prompt_int_ranged("Test GCD (1, 0): ", 0, 1) << 4;
        if (mask == 0) printf("(please select at least one test)\n");
    } while (mask == 0);
    return mask;
}

#pragma warning (disable : 4996)
void save_stats(char stats[], char filename[])
{
    FILE* file;
    file = fopen(filename, "a");
    if (file != NULL)
    {
        fputs(stats, file);
        fclose(file);
    }
}

// Tests users arithmetic skills.
void math_drill(int op_mask, int question_types, char* name, int left_min, int left_max, int right_min, int right_max, int questions_max)
{
    int c = 0;
    time_t t = time(NULL);
    time_t t_now = time(NULL);
    int t_delta = 1;
    srand((int)t);
    if (questions_max != -1) printf("\nPlease complete the next %i questions:\n", questions_max);
    for (bool answer = true; answer == true && c != questions_max;)
    {
        int a = rand() % (left_max - 1) + left_min;
        int b = rand() % (right_max - 1) + right_min;

        // randomly choose a test based on option selection
        int op = 0;
        do op = 1 << (rand() % question_types); while ((op & op_mask) == 0);

        switch (op)
        {
        case 0b00001: answer = test_add(a, b); break;
        case 0b00010: answer = test_sub(a, b); break;
        case 0b00100: answer = test_mul(a, b); break;
        case 0b01000: answer = test_div(a, b); break;
        case 0b10000: answer = test_gcd(a, b); break;
        }

        if (answer)
        {
            c++;
            t_now = time(NULL);

            // Let the player know some stats every 10 questions
            t_delta = (int)(t_now - t) + 1;
            if (c % 10 == 0) printf("\n%i correct in %i\' %i\". %i answers per minute.\n", c, t_delta / 60, t_delta % 60, 60 * c / t_delta);
        }
    }

    // Create a timestamp for today's date
    struct tm* timeinfo = localtime(&t_now);
    char* timestamp = asctime(timeinfo);
    if (timestamp != NULL) timestamp[strlen(timestamp) - 1] = '\0';

    // Save stats to file and print it to cmd
    enum { STATS_SIZE = 400 };
    char stats[STATS_SIZE];
    snprintf(stats, STATS_SIZE, "%s: %s: %i correct in %i\' %i\". %i answers per minute.\n", name, timestamp, c, t_delta / 60, t_delta % 60, 60 * c / t_delta);
    save_stats(stats, "stats.txt");
    fputs(stats, stdout);
}

void init_string(char* string, int string_size) {
    for (int i = 0; i < string_size; i++) {
        string[i] = ' ';
    }
    string[string_size - 1] = '\0';
}

int main()
{
    printf("Welcome to Math Drills!\n");
    int op_mask = 0b01111;
    int left_min = 2;
    int left_max = 9;
    int right_min = 2;
    int right_max = 9;
    int questions_max = 100;
    int options = 0;
    enum { NAME_SIZE = 50 };
    char name[NAME_SIZE] = "";
    enum { QUESTION_TYPES = 5 };
    init_string(name, NAME_SIZE);

    prompt_string("Enter Your Name: ", name, NAME_SIZE);
    do
    {
        options = prompt_int("Quit (0), Start (1), Test Options (2), Number Min (3), Number Max (4), Question Limit (5), Change Name (6): ");
        switch (options)
        {
        case 0: printf("Goodbye!\n"); break;
        case 1: math_drill(op_mask, QUESTION_TYPES, name, left_min, left_max, right_min, right_max, questions_max); break;
        case 2: op_mask = test_option(); break;
        case 3: left_min = prompt_int("Left Min: "); right_min = prompt_int("Right Min: "); break;
        case 4: left_max = prompt_int("Left Max: "); right_max = prompt_int("Right Max: "); break;
        case 5: questions_max = prompt_int("Question Limit (-1 to disable): "); break;
        case 6: prompt_string("Enter Your Name: ", name, NAME_SIZE); break;
        default: printf("(invalid input)\n");
        }
    } while (options != 0);
}
