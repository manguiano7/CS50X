#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    int i=0, sum=0, int_char;

    do
    {
        int_char = (int) word[i]; /* the integer value of the ith character in word*/
        if (islower(word[i])) /* checks if is lowercase alphabet (also checks if alphabet) */
        {
            sum += POINTS[int_char-97]; /*ASCII "a" is 97*/
        }
        else if (isupper(word[i])) /*checks if uppercase, also checks if alphabet*/
        {
            sum += POINTS[int_char-65];/*ASCII "A" is 65*/
        }
        /*above if statements are true only if alphabet, if not, sum doesn't get added to*/
        i += 1;
    }
    while (word[i]!= '\0'); /*end of line character*/
    return sum;
}
