// Implements a dictionary's functionality

#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;
int cw = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hash_number = hash(word);
    node *arrow = table[hash_number];
    while (arrow != NULL)
    {
        if (strcasecmp(arrow->word, word) == 0)
        {
            return true;
        }
        arrow = arrow->next;
    }

    // TODO
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //opens file
    FILE *file = fopen(dictionary, "r");

    // checks if file is NULL
    if (file == NULL)
    {
        printf("%s cannot be open\n", dictionary);
        return false;
    }

    char typed[LENGTH + 1];

    while (fscanf(file, "%s", typed) != EOF)
    {
        //allocates memory to node
        node *n = malloc(sizeof(node));
        //checks if node is nul
        if (n == NULL)
        {
            return false;
        }
        //copy string typed to n in word data structure
        strcpy(n->word, typed);

        int hash_number = hash(typed);

        if (table[hash_number] == NULL)
        {
            n->next = NULL;
        }
        else
        {
            n->next = table[hash_number];
        }

        table[hash_number] = n;

        cw += 1;
        //n->next = NULL;
    }
    //close file
    fclose(file);

    return true;

}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return cw;
}
void free_node(node *n)
{
    if (n->next != NULL)
    {
        free_node(n->next);
    }
    free(n);
}
// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            free_node(table[i]);
        }
    }

    return true;
}
