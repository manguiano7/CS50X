// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table

const unsigned int N = 5099;

// Hash table
node *table[N];

//dummy nodes
node *current_node;
node *next_node;
//counts words in dictionary
//initialized here so its scope is global, we can count as we load the dictionary
int num_words = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //gets hash value from hash function
    int hash_value = hash(word);

    //for loop to traverse nodes in ith table
    for (current_node = table[hash_value]; current_node != NULL; current_node = current_node -> next)
    {
        if (!strcasecmp(word, current_node -> word)) // true if strings are equal
        {
            return true;
        }

    }

    return false; //false if never true above
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int hash_sum = 0, i;
    for (i = 0; i < strlen(word); i++)
    {
        //some polynomial in letters, 7 is arbitrary
        hash_sum += toupper(word[i]) * toupper(word[i]) * power_hash(7, i + 1);
        //toupper(word[i])
        //printf("hash sum is %i\n", hash_sum);
    }

    // TODO: Improve this hash function
    //makes sure hash sum is between 0 and N
    return hash_sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *input = fopen(dictionary, "r");
    int i = 0, hash_value;
    /*
    int numcollisions[N];
    for (i = 0; i < N; i++)
    {
        numcollisions[i] = 0;
    }
    */
   //LENGTH + 2 since sometimes there is \n and \0
    char word[LENGTH + 2];
    bool word_added;
    //reads words in dictionary
    while (fgets(word, sizeof(word), input) != NULL)
    {
        //counts words in dictionary
        num_words += 1;
        //printf("char is %c duh\n", word[0]);
        if (word[strlen(word) - 1] == '\n') //if newline character exists
        {
            word[strlen(word) - 1] = '\0'; // removes newline \n character
        }
        //printf("word is %s\n", word);
        hash_value = hash(word);
        //numcollisions[hash_value] += 1;
        //printf("final hash value is %i\n", hash_value);
        //if table value is NULL there is nothing there, word can be readily added there
        if (table[hash_value] == NULL)
        {
            //calloc initializes node to something, to prevent valgrind error
            table[hash_value] = calloc(1, sizeof(node));
            //checks if malloc failed
            if (table[hash_value] == NULL)
            {
                return false;
            }

            //sets current node to first hashed table element
            current_node = table[hash_value];

            //strcpy adds word to first hashed table element
            strcpy(current_node -> word, word);
            //makes sure next node is NULL
            //current_node -> next = NULL;
            //printf("word added is %s\n", word);

        }
        else
        {
            word_added = false;
            //sets current node to first hashed table element
            current_node = table[hash_value];
            //while loops until word is added
            while (!word_added)
            {
                if (current_node -> next != NULL) //true if next node is not null (which means current node is full)
                {
                    // if (isalpha(current_node.word[0]))
                    current_node = current_node -> next; // goes to the next node
                }
                else // if next node is null (which means current node word is empty)
                {
                    //mallocs space for next node
                    current_node -> next = malloc(sizeof(node));
                    //next_node = malloc(sizeof(node));
                    //checks if malloc failed
                    if (current_node -> next == NULL)
                    {
                        return false;
                    }
                    //adds word to next node using strcpy
                    strcpy(current_node -> next -> word, word);
                    //makes sure next node is NULL
                    current_node -> next -> next = NULL;
                    //makes current node point to same location as next node
                    //i.e., completion of adding the word
                    // current_node -> next = next_node;
                    word_added = true;
                    //printf("word added is %s\n", word);
                }
            }
        }
    }
    /*
    for (i = 0; i < N; i++)
    {
        printf("Numcollisions[%i] is %i\n", i, numcollisions[i]);
    }*/
    fclose(input);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return num_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        current_node = table[i];
        if (current_node == NULL)
        {
            free(current_node);
        }
        else
        {
            while (current_node != NULL)
            {
                next_node = current_node -> next;
                free(current_node);
                current_node = next_node;
            }
        }
    }

    if (current_node == NULL) //if all nodes emptied
    {
        return true;
    }
    else
    {
        return false;
    }
}

//used for hash function
//pow() is used for doubles, this function was created to handle pure integers
int power_hash(int m, int n)
{
    int product = m;
    if (n == 1)
    {
        return m;
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            m = m * product % N;
        }
        return m;
    }
}