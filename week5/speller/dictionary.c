// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 300007;
// read that ideal is roughly 2x no. of values for minimal collisions.
// based on some theoretical .58 result of things beyond me.
// also, ideally not a power of 2 for more reasons beyond me
// picked a prime number because why not that too
// my hash function as is ends up with minimum 30k collisions - 
// I'm sure this can be improved, and I have fun ideas
// about how to create a visualization of hash table density
// which may help in trying to understand how the hash function 
// under-targets certain regions of indecis
// but effectively this table flattens the linear searches to very near
// constant time, as the lists are mostly 3 at most deep

// Hash table
node *table[N];

// Cache the dictionary size globally
int dictionary_size = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int hash_check = hash(word);

    node *traveller = table[hash_check];
    while (traveller != NULL)
    {
        // printf("do we get here");
        string s = traveller->word;
        // printf("checking %s \n", s);
        if (strcasecmp(s, word) == 0)
        {
            return true;
        }
        traveller = traveller->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // what happens with overflows?
    unsigned long runningHash = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        // mult by a constant prime above 26 (I think this is good?)
        runningHash = 29*runningHash + toupper(word[i]);
    }

    // printf("%s hashes to %li \n", word, (runningHash % N));
    return runningHash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");

    int index = 0, words = 0;
    char word[LENGTH + 1];

    // Add each word in dictionary
    // I discovered fscanf %s input while debugging
    // it would have been a simpler implementation in a few ways
    // the below read method (stolen from the speller.c)
    // is overkill as the dictionary words being valid is a given
    // but this works and is more robust in the end
    char c;
    while (fread(&c, sizeof(char), 1, file))
    {
        // Allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // Append character to word
            word[index] = c;
            index++;

            // Ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // Consume remainder of alphabetical string
                while (fread(&c, sizeof(char), 1, file) && c != '\n');

                // Prepare for new word
                index = 0;
            }
        }

        // Ignore words with numbers (like MS Word can)
        else if (isdigit(c))
        {
            // Consume remainder of alphanumeric string
            while (fread(&c, sizeof(char), 1, file) && c != '\n');

            // Prepare for new word
            index = 0;
        }

        else if (c == '\n')
        {
            // terminate word
            word[index] = '\0';
            // printf("we are adding: %s\n", word);
            // Hash 
            unsigned int newHash = hash(word);

            // malloc new node
            node *newWord = malloc(sizeof(node));
            if (newWord == NULL)
            {
                return false;
            }

            // addword to new node
            strcpy(newWord->word, word);

            // make new node's next the existing node at this hash
            // either NULL or a previously added word (collision)
            newWord->next = table[newHash];

            // make this point in table the new word node
            table[newHash] = newWord;
            words++;
            // printf("we have added: %s\n", table[newHash]->word);

            // Prepare for new word
            index = 0;
        }
    }

    fclose(file);
    // printf("Words: %i\n", words);
    dictionary_size = words;

    // density check
    // unsigned int cellsFilled = 0;
    // for (int i = 0; i < N; i++)
    // {
    //     if (table[i] != NULL)
    //     {
    //         cellsFilled++;
    //     }
    // }
    // printf("Cells Filled: %i \nMimimum Collisions: %i \n", cellsFilled,  ((N-cellsFilled) - (N-words)));

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dictionary_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *traveller = table[i]->next;
            free(table[i]);
            table[i] = traveller;
        }
    }
    return true;
}
