#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define jpeg_size 512
#define filenamesize 8
typedef uint8_t BYTE;
int isjpeg(BYTE buffer []);

int main(int argc, char *argv[])
{
    //runs if only 2 args
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    //let's open the memory card
    FILE *input = fopen(argv[1], "r");

    //it was oppened?
    if (input == NULL)
    {
        printf("%s cannot be open\n", argv[1]);
        return 1;
    }

    //define pointer of output images
    FILE *output = NULL;

    //counter to track number of images generated
    int count = 0;

    //file name
    char name[filenamesize];

    //let's find the beggining of a JPEG
    BYTE buffer[jpeg_size];

    while (fread(buffer, jpeg_size, 1, input))
    {
        //checks first if is a JPEG file
        if (isjpeg(buffer) == 1)
        {
            //is it the first jpeg?? if is not, close it the previous
            if (count != 0)
            {
                fclose(output);
            }
            //creates a name for the .jpg file
            sprintf(name, "%03i.jpg", count);

            //opens new file
            output = fopen(name, "w");

            //writes in the .jpg file
            fwrite(buffer, jpeg_size, 1, output);

            count++;
        }
        else if (count > 0)
        {
            fwrite(buffer, jpeg_size, 1, output);
        }
    }

}

int isjpeg(BYTE buffer [])
{
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
    {
        return 1;
    }
    return 0;
}