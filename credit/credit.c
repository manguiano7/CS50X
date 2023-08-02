#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long long int cc_num = get_long("Number: ");
    long long int cc_num_copy = cc_num;
    int sum =0;
    int dummy;
    int first_two;
    int num_digits =0;

do
{
    if (cc_num > 9)
    {
        num_digits = num_digits + 2;
    } else  {
        num_digits = num_digits + 1;
    }
    sum =  sum + (cc_num % 10);
    cc_num = cc_num / 10;
    dummy = (cc_num % 10) * 2;
    if (dummy <10 ) /*if dummy is double digits, we need to split it into single digits*/
    {
    sum = sum + dummy;
    }
    else
    {
     /* splits into single digits*/
    sum = sum + (dummy % 10);
    dummy = dummy / 10;
    sum = sum + dummy;
    }
    if (cc_num > 100 && cc_num < 1000)
    {
        first_two = cc_num / 10; /* first two digits of cc_num */
    } else if (cc_num >9) {
        first_two = cc_num;
    }

    cc_num = cc_num / 10;

} while (cc_num>0);

/*
printf("sum is %i\n", sum);
printf("first two is %i\n", first_two);
printf("num_digits is %i\n", num_digits);
*/
if (sum % 10 != 0)
{
printf("INVALID\n");
} else if (first_two >=51 && first_two <= 55 && num_digits ==16)
{
printf("MASTERCARD\n");
} else if ((first_two == 34 || first_two == 37) && num_digits ==15)
{
printf("AMEX\n");
} else if (first_two >=40 && first_two <=49 && ( num_digits ==13 ||  num_digits ==16))
{
printf("VISA\n");
} else {
 printf("INVALID\n");
}
}