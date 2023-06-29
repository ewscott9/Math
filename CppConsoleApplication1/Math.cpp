// Very simple command line arithmatic drills

#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

// clears stdin 
// used after getting numbers from stdin, incase user accidentally entered non numeric characters after
void clear_stdin()
{
    char c = NULL;
    do c = getchar(); while (c != EOF && c != '\n');
}

// gets int value, and clears stdin afterwards
int get_int2() 
{
    const int size = 10;
    char text[size];
    char* pend;
    fgets(text, size, stdin);
    int value = strtod(text, &pend);
    if ('0' < *pend || *pend > '9') value = LONG_MIN;
    printf("%i ", value);
    printf("%s ", pend);
    printf("%s\n", &text);
    clear_stdin();
    return value;
}

// gets int value, and clears stdin afterwards, can set max and min value that int can be.
int get_int()
{
    int value = 0;
    scanf_s("%i", &value);
    clear_stdin();
    return value;
}

// gets int value, and clears stdin afterwards, can set max and min value that int can be.
int get_int(int min, int max) 
{
    int value = 0;
    scanf_s("%i", &value);
    clear_stdin();
    if (value < min) value = min;
    if (value > max) value = max;
    return value;
}

// Following functions test arithmetic add, sub, mul, div
// they return true or false depending on the correctness of the user input
bool test_add(int a, int b)
{
    int x = 0;
    printf("%i + %i = ", a, b);
    x = get_int();
    return a + b == x;
}

bool test_sub(int a, int b)
{
    int x = 0;
    if (a < b) 
    {
        int t = a;
        a = b;
        b = t;
    }
    printf("%i - %i = ", a, b);
    x = get_int();
    return a - b == x;
}

bool test_mul(int a, int b) 
{
    int x = 0;
    printf("%i * %i = ", a, b);
    x = get_int();
    return a * b == x;
}

bool test_div(int a, int b) 
{
    int x = 0;
    int r = 0;
    if (a < b)
    {
        int t = a;
        a = b;
        b = t;
    }
    printf("%i / %i = ", a, b);
    x = get_int();
    if (a % b != 0) 
    {
        printf("%i mod %i = ", a, b);;
        r = get_int();
    }
    return a / b == x && a % b == r;
}

// asks for an input, and returns the value entered.
int menu(const char *text) 
{
    printf("%s", text);
    return get_int();
}

// Generates a selection mask
int test_option() 
{
    int mask = 0;
    do 
    {
        printf("Test Addition (1, 0): ");
        mask = get_int(0, 1);

        printf("Test Subtraction (1, 0): ");
        mask += get_int(0, 1) << 1;

        printf("Test Multiplication (1, 0): ");
        mask += get_int(0, 1) << 2;

        printf("Test Division (1, 0): ");
        mask += get_int(0, 1) << 3;

        if (mask == 0) printf("(please select at least one test)\n");
        printf("%i\n", mask);
    } while (mask == 0);
    return mask;
}

void math_drill(int op_mask, int left_min, int left_max, int right_min, int right_max)
{
    int c = 0;
    srand(time(NULL));
    int t = time(NULL) + 1; // +1 so fraction of second is rounded up. prevents div by 0, and makes more sense.
    for (bool answer = true; answer == true;)
    {
        int a = rand() % (left_max - 1) + left_min;
        int b = rand() % (right_max - 1) + right_min;

        // randomly choose a test based on option selection
        int op = 0;
        do op = 0b1000 >> (rand() % 4); while ((op & op_mask) == 0);

        switch (op)
        {
        case 0b0001: answer = test_add(a, b); break;
        case 0b0010: answer = test_sub(a, b); break;
        case 0b0100: answer = test_mul(a, b); break;
        case 0b1000: answer = test_div(a, b); break;
        }

        if (answer) c++;
    }

    // print out some stat's
    t = time(NULL) - t; 
    printf("%i correct in %i seconds. %i answers per minute\n", c, t, 60 * c / t);
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
        options = menu("Quit (0), Start (1), Test Options (2), Number Min (3), Number Max (4): ");
        switch (options) 
        {
        case 0: break;
        case 1: math_drill(op_mask, left_min, left_max, right_min, right_max); break;
        case 2: op_mask = test_option(); break;
        case 3:
            left_min = menu("Left Min: ");
            right_min = menu("Right Min: ");
            break;
        case 4: 
            left_max = menu("Left Max: ");
            right_max = menu("Right Max: ");
            break;
        default: printf("(invalid input)\n");
        } 
    } while (options != 0);
    printf("Goodbye!\n");
}


