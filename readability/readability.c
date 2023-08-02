#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include<math.h>
int count_letters (string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");
    double avg_lett_words, avg_sent_words;
    int num_words = 1, num_sent = 0, num_letters = 0, index;

    num_letters = count_letters(text);
    num_words = count_words(text);
    num_sent = count_sentences(text);

    /*  printf("%i\n", num_letters);
    printf("%i\n", num_words);
    printf("%i\n", num_sent);*/
    avg_lett_words = (double) num_letters/num_words*100.0;
    avg_sent_words = (double) num_sent/num_words*100.0;
    index = round(0.0588 * avg_lett_words - (0.296 * avg_sent_words) - 15.8);
    if (index <1)
    {
        printf("Before Grade 1\n");
    }
    else if (index>15)
    {
        printf("Grade 16+\n");
    }
     else
     {

        printf("Grade %i\n", index);
     }
}

int count_letters (string text)
{
    int i=0, num_letters=0;
do
    {
        if (isalpha((int) text[i])) /* checks if a letter*/
        {
            num_letters +=1;
        }
    i += 1;
    }
    while (text[i] != '\0');
    return num_letters;
}

int count_words(string text)
{
    int i=0,num_words=1; /*initializes to one to account for last word*/
  do
    {

        if (text[i] == ' ')
        {
            num_words +=1;
        }
    i += 1;
    }
    while (text[i] != '\0');
    return num_words;
}

int count_sentences(string text)
{
    int i=0, num_sent=0;
    do
    {

        if (text[i]=='.' || text[i]=='!' || text[i]=='?')
        {
            num_sent +=1;
        }

    i += 1;
    }
    while (text[i] != '\0');
    return num_sent;
}