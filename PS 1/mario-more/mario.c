#include <cs50.h>
#include <stdio.h>

int height;
int r;
int c;
int s;
int main(void)
{


    do

    {

        //Ask the user for the height
        height = get_int("What's the height? \n");

    }
    //loop for height between 1 to 8
    while (height > 8 || height < 1);
    //loop for rows
    for (r = 0 ; r < height ; r++)
    {
        //loop for spaces
        for (s = 0 ; s < height - r - 1; s++)
        {
            printf(" ");
        }
        //loop for colums
        for (c = 0 ; c <= r ; c++)
        {
            printf("#");
        }


        printf("  ");


        //loop for colums
        for (c = 0 ; c <= r ; c++)
        {
            printf("#");
        }


        printf("\n");


    }


}
