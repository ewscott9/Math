// Very simple and slightly broken math drill game for cmd

#include <iostream>
#include <cmath>
#include <ctime>
using namespace std;

bool test_add(int a, int b)
{
    int x = 0;
    printf("%i + %i = ", a, b);
    scanf_s("%i", &x);
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
    scanf_s("%i", &x);
    return a - b == x;
}

bool test_mul(int a, int b) 
{
    int x = 0;
    printf("%i * %i = ", a, b);
    scanf_s("%i", &x);
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
    scanf_s("%i", &x);
    if (a % b != 0) 
    {
        printf("%i mod %i = ", a, b);;
        scanf_s("%i", &r);
    }
    return a / b == x && a % b == r;
}

int menu() 
{
    int a;
    printf("Quit (0), Start (1), Test Options (2), Number Options (3): ");
    scanf_s("%i", &a);
    return a;
}

// Generates a selection mask
int test_option() {
    int x = 0;
    int m = 0;
    printf("Test Addition (1, 0): ");
    scanf_s("%i", &x);

    printf("Test Subtraction (1, 0): ");
    scanf_s("%i", &m);
    x = x + m * 0b0010;

    printf("Test Multiplication (1, 0): ");
    scanf_s("%i", &m);
    x = x + m * 0b0100;

    printf("Test Division (1, 0): ");
    scanf_s("%i", &m);
    x = x + m * 0b1000;

    return x;
}

int number_option(const char* text) 
{
    int x = 0;
    printf("%s", text);
    scanf_s("%i", &x);
    return x;
}

void mathdrill(int op_mask, int left_max, int right_max)
{
    srand(time(NULL));
    int c = 0;
    int t = time(NULL);
    bool answer = true;

    while (answer)
    {
        int a = rand() % (left_max - 1) + 2;
        int b = rand() % (right_max - 1) + 2;

        // randomly choose a test based on option selection
        int op = 0;
        while ((op & op_mask) == 0) {
            op = 0b1000 >> (rand() % 4);
        }

        switch (op & op_mask)
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

    t = time(NULL) - t;
    printf("%i correct in %i seconds. %i answers per minute\n", c, t, 60 * c / t);
}

int main()
{
    int options = 1;
    int op_mask = 0b1111;
    int left_max = 9;
    int right_max = 9;
    while (options != 0)
    {
        options = menu();
        switch (options) 
        {
        case 0: break;
        case 2: op_mask = test_option(); break;
        case 3: 
            left_max = number_option("Left Max: ");
            right_max = number_option("Right Max: ");
            break;
        default: mathdrill(op_mask, left_max, right_max);
        }
    }
}


