#include <cs50.h>
#include <stdio.h>
/*
    Give a number in a do while loop:
    -must be positive
    -long will take care of letters
    need to take secound to last digit then times two


*/
long cardnumber, go1, go2, n, firstnumbers;
int lds, ld, sld, m2, firstd, secoundd, firstsum, sumhigher, sumoftwo, secoundsum, sumofthelast, totalsum, modulo, cardigits, i,
    firstnumber;
//long go1;
int main(void)
{

    do
    {
        cardnumber = get_long("What's the card number?\n");
    }
    while (cardnumber <= 0);

    n = cardnumber;

    //cardnumber digit counter
    while (n > 1)
    {
        n = n / 10;
        i++;
    }

    //save cardigits before
    cardigits = i;

    //whats the first numbers??
    for (firstnumbers = cardnumber; i > 2 ; i--)
    {
        firstnumbers = firstnumbers / 10;
    }

    //visa first number
    if ((firstnumbers >= 40) && (firstnumbers <= 49))
    {
        firstnumber = firstnumbers / 10;
    }

    do
    {
        //get the digits of the cardnumber
        lds = cardnumber % 100;

        // last digit of the last digits
        ld = lds % 10;

        // first digit of the last digits
        sld = lds / 10;

        //sum of the last digits
        sumofthelast = ld + sumofthelast;

        //times two
        m2 = sld * 2;

        // is the product higher then 9???
        if (m2 > 9)
        {
            //
            firstd = m2 / 10;
            secoundd = m2 % 10 ;
            sumhigher = firstd + secoundd;
            firstsum = sumhigher + firstsum;

            //move to secound to last digit
            cardnumber = cardnumber / 100;

        }
        else
        {

            //sum of digits
            firstsum = m2 + firstsum;

            //move to secound to last digit
            cardnumber = cardnumber / 100;
        }
    }
    while (cardnumber > 0);

    //total sum
    totalsum = sumofthelast + firstsum;

    //last digit
    modulo = totalsum % 10;

    //check firstnumber and how many digits
    if (modulo == 0)
    {
        if (((firstnumbers >= 51) && (firstnumbers <= 55)) && cardigits == 16)
        {
            printf("MASTERCARD\n");
        }
        else if (firstnumber == 4 && (cardigits == 13 || cardigits == 16))
        {
            printf("VISA\n");
        }
        else if ((firstnumbers == 34 || firstnumbers == 37) && cardigits == 15)
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
