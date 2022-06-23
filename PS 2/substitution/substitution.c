#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int argv_lenght, plaintext_lenght;
string key_string, plaintext;

int main(int argc, string argv[])
{
    char alphalower[26];
    char alphaupper[26];
    if (argc != 2)
    {
        printf("./substution key\n");
        return 1;
    }

    // what's the argv[1] string lenght?
    argv_lenght = strlen(argv[1]);

    // must coint the 26 digits
    if (argv_lenght != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    //makes argv[1]
    key_string = argv[1];

    // cheking key
    for (int i = 0 ; i < argv_lenght; i++)
    {
        //is key alpha?
        if (!isalpha(key_string[i]))
        {
            printf("Key must only contain alphabetc characters.\n");
            return 1;
        }
        //cheking if is duplicated
        for (int c = i + 1 ; c < argv_lenght ; c++)
        {
            if (key_string[i] == key_string[c])
            {
                printf("Key must not contain repeated characters.\n");
                return 1;
            }
        }
    }
    // what's plaintext
    plaintext = get_string("plaintext: ");
    // plaintext lenght
    plaintext_lenght = strlen(plaintext);
    // making alpha lower
    for (char c = 'a' ; c <= 'z' ; c++)
    {
        alphalower[c - 'a'] = c;
    }
    // making alpha upper
    for (char c = 'A' ; c <= 'Z' ; c++)
    {
        alphaupper[c - 'A'] = c;
    }
    //making ciphertext
    char ciphertext[plaintext_lenght];

    for (int i = 0 ; i <= plaintext_lenght ; i++)
    {   
        int printed = 0;
        for (int c = 0 ; c <= 26 && printed == 0 ; c++)
        {
            if (isupper(plaintext[i]))
            {
                if (plaintext[i] == alphaupper[c])
                {
                    ciphertext[i] = toupper(key_string[c]);
                    printed = 1;
                }

            }
            else if (islower(plaintext[i]))
            {
                if (plaintext[i] == alphalower[c])
                {
                    ciphertext[i] = tolower(key_string[c]);
                    printed = 1;
                }

            }
            else if (!isalpha(plaintext[i]))
            {
                ciphertext[i] = plaintext[i];
                printed = 1;
            }

        }



    }
    printf("ciphertext: %s\n", ciphertext);


}