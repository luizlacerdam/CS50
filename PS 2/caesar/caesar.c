#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int i, lenght, key, is_digit, is_not_digit, plaintext_lenght;
string plaintext, key_string;
char  c;
char rotate(char ch, int n);

int main(int argc, string argv[])
{
    // argc > 2 ?
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    //  what's argv[1] lenght??
    lenght = strlen(argv[1]);

    // makes argv[1] constant
    key_string = argv[1];

    //  is it valid argv?
    for (i = 0; lenght > i ; i++)
    {
        if (!isdigit(key_string[i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    // converting the key
    key = atoi(argv[1]);

    // getting the plaintext
    plaintext = get_string("What's the plaintext? ");

    // Encipher
    plaintext_lenght = strlen(plaintext);

    printf("ciphertext: ");

    for (i = 0; i < plaintext_lenght; i++)
    {
        if (isupper(plaintext[i]))
        {
            c = (plaintext[i] + key - 65) % 26 + 65;
        }
        else if (islower(plaintext[i]))
        {
            c = (plaintext[i] + key - 97) % 26 + 97;
        }
        else
        {
            c = plaintext[i];
        }
        printf("%c", c);
    }

    printf("\n");

}
