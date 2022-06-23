#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int i, text_lenght, grade;
float average_setences, average_letters, index, letters, setences, words;
int count_letters(string text);
int count_words(string text);
int count_setences(string text);
int main(void)
{

    //get text
    string text = get_string("What's the text? ");

    //text lenght
    text_lenght = strlen(text);

    //count letters
    letters = count_letters(text);

    //count words
    words = count_words(text) + 1;

    //count setences
    setences = count_setences(text);

    //average letters
    average_letters = letters / words * 100;


    // average setences
    average_setences = setences / words * 100;


    //Coleman Index
    index = (0.0588 * average_letters) - (0.296 * average_setences) - 15.8;

    //rounds the index
    grade = round(index);

    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

int count_letters(string text)
{
    for (i = 0; i < text_lenght  ; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
    }
    return letters;
}
int count_words(string text)
{
    for (i = 0; i < text_lenght  ; i++)
    {
        if (isspace(text[i]))
        {
            words++;
        }
    }
    return words;
}

int count_setences(string text)
{
    for (i = 0; i < text_lenght  ; i++)
    {
        if (text[i] == '!' || text[i] == '.' || text[i] == '?')
        {
            setences++;
        }
    }
    return setences;
}


