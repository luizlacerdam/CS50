#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int r = 0; r < height; r++)
    {
        for (int p = 0; p < width; p++)
        {
            float red = image[r][p].rgbtRed;
            float g = image[r][p].rgbtGreen;
            float b = image[r][p].rgbtBlue;
            int avarage = round((red + g + b) / 3.00);
            image[r][p].rgbtRed = avarage;
            image[r][p].rgbtGreen = avarage;
            image[r][p].rgbtBlue = avarage;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //row loop
    for (int r = 0; r < height; r++)
    {
        //colum loop
        for (int c = 0; c < width / 2; c++)
        {
            //temporary var to recieve before swap
            int red = image[r][c].rgbtRed;
            int green = image[r][c].rgbtGreen;
            int blue = image[r][c].rgbtBlue;
            //swap
            image[r][c].rgbtRed = image[r][width - 1 - c].rgbtRed;
            image[r][c].rgbtGreen = image[r][width - 1 - c].rgbtGreen;
            image[r][c].rgbtBlue = image[r][width - 1 - c].rgbtBlue;
            //swap
            image[r][width - 1 - c].rgbtRed = red;
            image[r][width - 1 - c].rgbtGreen = green;
            image[r][width - 1 - c].rgbtBlue = blue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //copy of the original image to temp
    RGBTRIPLE tempimage[height][width];
    for (int r = 0; r < height; r++)
    {
        for (int p = 0; p < width; p++)
        {
            tempimage[r][p].rgbtRed = image[r][p].rgbtRed;
            tempimage[r][p].rgbtGreen = image[r][p].rgbtGreen;
            tempimage[r][p].rgbtBlue = image[r][p].rgbtBlue;
        }
    }
    //start the of the code

    for (int r = 0; r < height; r++)
    {
        for (int p = 0; p < width; p++)
        {
            int reds, greens, blues;
            reds = greens = blues = 0;
            float c = 0.00;
            //pixels around 9 p
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2;y++)
                {
                    int relpx = r + x;
                    int relpy = p + y;
                    if (relpx < 0 || relpx > height -1 || relpy < 0 || relpy > width -1)
                    {
                        continue;
                    }
                    //image value
                    reds += tempimage[relpx][relpy].rgbtRed;
                    greens += tempimage[relpx][relpy].rgbtGreen;
                    blues += tempimage[relpx][relpy].rgbtBlue;
                    c++;
                }
                //avarage
                int avgRed = round(reds/c);
                int avgGreen = round(greens/c);
                int avgBlue = round(blues/c);
                image[r][p].rgbtRed = avgRed;
                image[r][p].rgbtGreen = avgGreen;
                image[r][p].rgbtBlue = avgBlue;
            }
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //copy of the original image to temp
    RGBTRIPLE tempimage[height][width];
    for (int r = 0; r < height; r++)
    {
        for (int p = 0; p < width; p++)
        {
            tempimage[r][p].rgbtRed = image[r][p].rgbtRed;
            tempimage[r][p].rgbtGreen = image[r][p].rgbtGreen;
            tempimage[r][p].rgbtBlue = image[r][p].rgbtBlue;
        }
    }
    //start the of the code

    for (int r = 0; r < height; r++)
    {
        for (int p = 0; p < width; p++)
        {
            int reds, greens, blues;
            reds = greens = blues = 0;
            float c = 0.00;
            //pixels around 9 p
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2;y++)
                {
                    int relpx = r + x;
                    int relpy = p + y;
                    if (relpx < 0 || relpx > height -1 || relpy < 0 || relpy > width -1)
                    {
                        continue;
                    }
                    //image value
                    reds += tempimage[relpx][relpy].rgbtRed;
                    greens += tempimage[relpx][relpy].rgbtGreen;
                    blues += tempimage[relpx][relpy].rgbtBlue;
                    c++;
                }
                //avarage
                int avgRed = round(reds/c);
                int avgGreen = round(greens/c);
                int avgBlue = round(blues/c);

                image[r][p].rgbtRed = avgRed;
                image[r][p].rgbtGreen = avgGreen;
                image[r][p].rgbtBlue = avgBlue;
            }
        }
    }
    return;
}
