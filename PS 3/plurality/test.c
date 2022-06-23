#include <cs50.h>
#include <stdio.h>
#include <string.h>

bool vote(string name);
string name1;
int main (void)
{
     name1 = "A";
    string name = get_string("Vote: ");
    vote(name);
}

bool vote(string name)
{
    if (strcmp(name1, name) == 0)
    {
        return true;
        
    }


    return false;
}