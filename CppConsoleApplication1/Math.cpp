// Very simple command line arithmatic drills

#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

void clear_stdin() 
{
    int c;
    do 
    {
        c = getchar();
    } while (c != EOF && c != '\n');
}

int get_int() 
{
    int value;
    scanf_s("%i", &value);
    clear_stdin();
    return value;
}

int get_int(int min, int max) 
{
    int value;
    scanf_s("%i", &value);
    clear_stdin();
    if (value < min) value = min;
    if (value > max) value = max;
    return value;
}

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

int menu(const char *text) 
{
    printf("%s", text);
    int a = get_int();
    return a;
}

// Generates a selection mask
int test_option() 
{
    int mask = 0;
    while (mask == 0) 
    {
        printf("Test Addition (1, 0): ");
        mask = get_int(0, 1);

        printf("Test Subtraction (1, 0): ");
        mask = mask + get_int(0, 1) * 0b0010;

        printf("Test Multiplication (1, 0): ");
        mask = mask + get_int(0, 1) * 0b0100;

        printf("Test Division (1, 0): ");
        mask = mask + get_int(0, 1) * 0b1000;

        if (mask == 0) printf("(please select at least one test)\n");
    }
    return mask;
}

int number_option(const char *text) 
{
    printf("%s", text);
    return get_int();
}

void math_drill(int op_mask, int left_min, int left_max, int right_min, int right_max)
{
    
    srand(time(NULL));
    int c = 0;
    int t = time(NULL);
    for (bool answer = true; answer == true;)
    {
        int a = rand() % (left_max - 1) + left_min;
        int b = rand() % (right_max - 1) + right_min;

        // randomly choose a test based on option selection
        int op = 0;
        while ((op & op_mask) == 0) {
            op = 0b1000 >> (rand() % 4);
        }

        switch (op)
        {
        case 0b0001: answer = test_add(a, b); break;
        case 0b0010: answer = test_sub(a, b); break;
        case 0b0100: answer = test_mul(a, b); break;
        case 0b1000: answer = test_div(a, b); break;
        }

        if (answer)
        {
            c++;
        }
    }

    t = time(NULL) - t + 1;
    printf("%i correct in %i seconds. %i answers per minute\n", c, t, 60 * c / t);
}

int main()
{

    int op_mask = 0b1111;
    int left_min = 2;
    int left_max = 9;
    int right_min = 2;
    int right_max = 9;
    for (int options = 1;  options != 0;)
    {
        options = menu("Quit (0), Start (1), Test Options (2), Number Min (3), Number Max (4): ");
        switch (options) 
        {
        case 0: break;
        case 1: math_drill(op_mask, left_min, left_max, right_min, right_max); break;
        case 2: op_mask = test_option(); break;
        case 3:
            left_min = number_option("Left Min: ");
            right_min = number_option("Right Min: ");
            break;
        case 4: 
            left_max = number_option("Left Max: ");
            right_max = number_option("Right Max: ");
            break;
        default: printf("(invalid input)\n");
        } 
    } 
}


