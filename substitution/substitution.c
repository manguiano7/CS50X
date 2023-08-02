#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
int validkey(string key, int key_len);
int main(int argc, string argv[])
{


    if (argc != 2)
    {
        printf("Please provide only a single key containing 26 alphabetic characters.\n");
        return 1; /*wrong number of arguments returns 1 to indicate error */
    }
    int key_len = strlen(argv[1]);
    string key = argv[1];

    int i = 0;
    int keyerror = validkey(key,key_len);

    if (keyerror == 0)
    {
        string text = get_string("plaintext: ");
        int text_len = strlen(text);
        printf("ciphertext: ");
        for (i = 0; i < text_len; i++)
        {
            if (isupper(text[i]))
            {
                printf("%c", toupper(key[(int) text[i] - 65]));//returns upper keyed value if text is upper
            }
            else if (islower(text[i]))
            {
                printf("%c", tolower(key[(int) text[i] - 97]));
            }
            else
            {
                printf("%c", text[i]);
            }
        }
        printf("\n");
        return 0;

    }
    else if (keyerror == 1)
    {
        printf("Key must contain exactly 26 non-repeating alphabetic characters.\n");
        return 1;
    }
}

int validkey(string key, int key_len)
{
    int i = 0, j = 0;
    if (key_len != 26)
    {
        return 1;
    }
printf("key_len is %i\n",key_len);
    for (i=0; i < key_len; i++)
    {
        if (!isalpha((int) key[i])) // checks if a letter
            {

                return 1; //1 if character is not alpha
            }

        for (j = 0; j < i; j++)
        {
            if (tolower(key[i]) == tolower(key[j]))
            {

                return 1; //if non unique
            }
        }
    }
    return 0; //if everything works well, no if statements above were triggered.
}