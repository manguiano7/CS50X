//recursively adds a second number to a first number, for fun
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include<math.h>

int adder(int i, int j);
int main(void)
{

//the two numbers, the second number is added recursively to the first number
int i = 99;
//works if j >= 0
int j = 100;
int sum = adder(i,j);
printf("sum is %i\n", sum);
}

int adder(int i, int j)
    {
        if (j == 0)
        {
            return i;
        }
        else
        {
            return 1 + adder(i,j-1);
        }
    }


