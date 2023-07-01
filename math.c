// Very simple command line arithmatic drills

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

// clears stdin 
// used after getting numbers from stdin, incase user accidentally entered non numeric characters after
void clear_stdin()
{
    char c = '\0';
    do c = getchar(); while (c != EOF && c != '\n');
}

// gets int value, and clears stdin afterwards
// if input starts with a number, reads input until SIZE - 1 characters or a non number and returns the number or LONG_MIN or LONG_MAX if number was too large
// if input starts with a letter, returns LONG_MIN.
int get_int()
{
    enum { SIZE = 20 };

    // grab characters from stdin, if you can't get them all in one go, throw the rest away.
    char text[SIZE];
    fgets(text, SIZE, stdin);
    if (text[strlen(text) - 1] != '\n') clear_stdin();

    // if theres a number at the front of the string return that value
    char* pend;
    int value = strtol(text, &pend, 0);
    if (value == 0 && pend[0] != '\n') value = LONG_MIN;

    //printf("%i ", value);

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
        if (mask == 0) printf("(please select at least one test)\n");
    } while (mask == 0);
    return mask;
}

#pragma warning (disable : 4996) // required because ms dislikes fopen for some reason.
void save_stats(char stats[], char filename[]) {
    FILE* file;
    file = fopen(filename, "a");
    if (file != NULL) {
        fputs(stats, file);
        fclose(file);
    }
}

// Tests users arithmetic skills.
void math_drill(int op_mask, int left_min, int left_max, int right_min, int right_max)
{
    int c = 0;
    time_t t = time(NULL);
    srand(t);
    for (bool answer = true; answer == true;)
    {
        int a = rand() % (left_max - 1) + left_min;
        int b = rand() % (right_max - 1) + right_min;

        // randomly choose a test based on option selection
        int op = 0;
        do op = 1 << (rand() % 4); while ((op & op_mask) == 0);

        switch (op)
        {
        case 0b0001: answer = test_add(a, b); break;
        case 0b0010: answer = test_sub(a, b); break;
        case 0b0100: answer = test_mul(a, b); break;
        case 0b1000: answer = test_div(a, b); break;
        }

        if (answer) c++;
    }

    // calculate, save, and print out some stat's
    time_t t_now = time(NULL);
    t = t_now - t + 1; //round to the next second. makes more sense, and prevents div by 0 later

    // Create a timestamp for today's date
    struct tm* timeinfo = localtime(&t_now);
    char* timestamp = asctime(timeinfo);
    if (timestamp != NULL) timestamp[strlen(timestamp) - 1] = '\0';

    // Save stats to 
    enum{ SIZE = 200 };
    char stats[SIZE];
    snprintf(stats, SIZE, "%s: %i correct in %i seconds. %i answers per minute.\n", timestamp, c, (int) t, 60 * c / (int) t);
    save_stats(stats, "stats.txt");
    fputs(stats, stdout);
}

int main()
{
    printf("Welcome to Math Drills!\n");
    int op_mask = 0b1111;
    int left_min = 2;
    int left_max = 9;
    int right_min = 2;
    int right_max = 9;
    int options = 0;
    do
    {
        options = prompt_int("Quit (0), Start (1), Test Options (2), Number Min (3), Number Max (4): ");
        switch (options)
        {
        case 0: printf("Goodbye!\n"); break;
        case 1: math_drill(op_mask, left_min, left_max, right_min, right_max); break;
        case 2: op_mask = test_option(); break;
        case 3: left_min = prompt_int("Left Min: "); right_min = prompt_int("Right Min: "); break;
        case 4: left_max = prompt_int("Left Max: "); right_max = prompt_int("Right Max: "); break;
        default: printf("(invalid input)\n");
        }
    } while (options != 0);
}


