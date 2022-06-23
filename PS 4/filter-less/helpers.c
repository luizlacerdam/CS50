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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int r = 0; r < height; r++)
    {
        for (int p = 0; p < width; p++)
        {
            int nsepiaRed = round((.393 * image[r][p].rgbtRed) + (.769 * image[r][p].rgbtGreen) + (.189 * image[r][p].rgbtBlue));
            int nsepiaGreen = round((.349 * image[r][p].rgbtRed) + (.686 * image[r][p].rgbtGreen) + (.168 * image[r][p].rgbtBlue));
            int nsepiaBlue = round((.272 * image[r][p].rgbtRed) + (.534 * image[r][p].rgbtGreen) + (.131 * image[r][p].rgbtBlue));

            if (nsepiaRed > 255)
            {
                nsepiaRed = 255;
            }
            if (nsepiaGreen > 255)
            {
                nsepiaGreen = 255;
            }
            if (nsepiaBlue > 255)
            {
                nsepiaBlue = 255;
            }
            image[r][p].rgbtRed = nsepiaRed;
            image[r][p].rgbtGreen = nsepiaGreen;
            image[r][p].rgbtBlue = nsepiaBlue;
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

    for (int r = 0; r < height; r++)
    {
        for (int p = 0; p < width; p++)
        {

            //corner pixels
            //left top
            if ((r == 0 && p == 0))
            {
                int avgred = round((tempimage[0][0].rgbtRed + tempimage[0][1].rgbtRed + tempimage[1][0].rgbtRed + tempimage[1][1].rgbtRed) / 4.00);
                int avggreen = round((tempimage[0][0].rgbtGreen + tempimage[0][1].rgbtGreen + tempimage[1][0].rgbtGreen +
                                      tempimage[1][1].rgbtGreen) / 4.00);
                int avgblue = round((tempimage[0][0].rgbtBlue + tempimage[0][1].rgbtBlue + tempimage[1][0].rgbtBlue + tempimage[1][1].rgbtBlue)
                                    / 4.00);
                image[r][p].rgbtRed = avgred;
                image[r][p].rgbtGreen = avggreen;
                image[r][p].rgbtBlue = avgblue;

            }
            //right top
            if ((r == 0 && p == width - 1))
            {
                int avgred = round((tempimage[0][width - 1].rgbtRed + tempimage[0][width - 2].rgbtRed + tempimage[1][width - 2].rgbtRed +
                                    tempimage[1][width - 1].rgbtRed) / 4.00);
                int avggreen = round((tempimage[0][width - 1].rgbtGreen + tempimage[0][width - 2].rgbtGreen + tempimage[1][width - 2].rgbtGreen +
                                      tempimage[1][width - 1].rgbtGreen) / 4.00);
                int avgblue = round((tempimage[0][width - 1].rgbtBlue + tempimage[0][width - 2].rgbtBlue + tempimage[1][width - 2].rgbtBlue +
                                     tempimage[1][width - 1].rgbtBlue) / 4.00);
                image[r][p].rgbtRed = avgred;
                image[r][p].rgbtGreen = avggreen;
                image[r][p].rgbtBlue = avgblue;

            }
            //left bottom
            if ((r == height - 1 && p == 0))
            {
                int avgred = round((tempimage[height - 1][0].rgbtRed + tempimage[height - 2][0].rgbtRed + tempimage[height - 2][1].rgbtRed +
                                    tempimage[height - 1][1].rgbtRed) / 4.00);
                int avggreen = round((tempimage[height - 1][0].rgbtGreen + tempimage[height - 2][0].rgbtGreen + tempimage[height - 2][1].rgbtGreen +
                                      tempimage[height - 1][1].rgbtGreen) / 4.00);
                int avgblue = round((tempimage[height - 1][0].rgbtBlue + tempimage[height - 2][0].rgbtBlue + tempimage[height - 2][1].rgbtBlue +
                                     tempimage[height - 1][1].rgbtBlue) / 4.00);
                image[r][p].rgbtRed = avgred;
                image[r][p].rgbtGreen = avggreen;
                image[r][p].rgbtBlue = avgblue;

            }
            //right bottom
            if ((r == height - 1 && p == width - 1))
            {
                int avgred = round((tempimage[height - 1][width - 1].rgbtRed + tempimage[height - 2][width - 1].rgbtRed + tempimage[height -
                                    2][width - 2].rgbtRed + tempimage[height - 1][width - 2].rgbtRed) / 4.00);
                int avggreen = round((tempimage[height - 1][width - 1].rgbtGreen + tempimage[height - 2][width - 1].rgbtGreen + tempimage[height -
                                      2][width - 2].rgbtGreen + tempimage[height - 1][width - 2].rgbtGreen) / 4.00);
                int avgblue = round((tempimage[height - 1][width - 1].rgbtBlue + tempimage[height - 2][width - 1].rgbtBlue + tempimage[height -
                                     2][width - 2].rgbtBlue + tempimage[height - 1][width - 2].rgbtBlue) / 4.00);
                image[r][p].rgbtRed = avgred;
                image[r][p].rgbtGreen = avggreen;
                image[r][p].rgbtBlue = avgblue;

            }
            //side pixels
            //left side
            if ((r > 0 && r < height - 1 && p == 0))
            {
                //red vars
                int rce = tempimage[r][0].rgbtRed;
                int rup = tempimage[r - 1][0].rgbtRed;
                int rur = tempimage[r - 1][1].rgbtRed;
                int rrs = tempimage[r][1].rgbtRed;
                int rdr = tempimage[r + 1][1].rgbtRed;
                int rdw = tempimage[r + 1][0].rgbtRed;
                //green vars
                int gce = tempimage[r][0].rgbtGreen;
                int gup = tempimage[r - 1][0].rgbtGreen;
                int gur = tempimage[r - 1][1].rgbtGreen;
                int grs = tempimage[r][1].rgbtGreen;
                int gdr = tempimage[r + 1][1].rgbtGreen;
                int gdw = tempimage[r + 1][0].rgbtGreen;
                //blue vars
                int bce = tempimage[r][0].rgbtBlue;
                int bup = tempimage[r - 1][0].rgbtBlue;
                int bur = tempimage[r - 1][1].rgbtBlue;
                int brs = tempimage[r][1].rgbtBlue;
                int bdr = tempimage[r + 1][1].rgbtBlue;
                int bdw = tempimage[r + 1][0].rgbtBlue;

                int avgred = round((rce + rup + rur + rrs + rdr + rdw) / 6.00);
                int avggreen = round((gce + gup + gur + grs + gdr + gdw) / 6.00);
                int avgblue = round((bce + bup + bur + brs + bdr + bdw) / 6.00);
                image[r][p].rgbtRed = avgred;
                image[r][p].rgbtGreen = avggreen;
                image[r][p].rgbtBlue = avgblue;
            }
            //right side
            if ((r > 0 && r < height - 1 && p == width - 1))
            {
                //red vars
                int rce = tempimage[r][p].rgbtRed;
                int rup = tempimage[r - 1][p].rgbtRed;
                int rul = tempimage[r - 1][p - 1].rgbtRed;
                int rlf = tempimage[r][p - 1].rgbtRed;
                int rdl = tempimage[r + 1][p - 1].rgbtRed;
                int rdw = tempimage[r + 1][p].rgbtRed;
                //green vars
                int gce = tempimage[r][p].rgbtGreen;
                int gup = tempimage[r - 1][p].rgbtGreen;
                int gul = tempimage[r - 1][p - 1].rgbtGreen;
                int glf = tempimage[r][p - 1].rgbtGreen;
                int gdl = tempimage[r + 1][p - 1].rgbtGreen;
                int gdw = tempimage[r + 1][p].rgbtGreen;
                //blue vars
                int bce = tempimage[r][p].rgbtBlue;
                int bup = tempimage[r - 1][p].rgbtBlue;
                int bul = tempimage[r - 1][p - 1].rgbtBlue;
                int blf = tempimage[r][p - 1].rgbtBlue;
                int bdl = tempimage[r + 1][p - 1].rgbtBlue;
                int bdw = tempimage[r + 1][p].rgbtBlue;

                int avgred = round((rce + rup + rul + rlf + rdl + rdw) / 6.00);
                int avggreen = round((gce + gup + gul + glf + gdl + gdw) / 6.00);
                int avgblue = round((bce + bup + bul + blf + bdl + bdw) / 6.00);
                image[r][p].rgbtRed = avgred;
                image[r][p].rgbtGreen = avggreen;
                image[r][p].rgbtBlue = avgblue;

            }
            //up side
            if ((r == 0 && p > 0 && p < width - 1))
            {
                //red vars
                int rce = tempimage[0][p].rgbtRed;
                int rls = tempimage[0][p - 1].rgbtRed;
                int rdl = tempimage[r + 1][p - 1].rgbtRed;
                int rds = tempimage[r + 1][p].rgbtRed;
                int rdr = tempimage[r + 1][p + 1].rgbtRed;
                int rrs = tempimage[0][p + 1].rgbtRed;
                //green vars
                int gce = tempimage[0][p].rgbtGreen;
                int gls = tempimage[0][p - 1].rgbtGreen;
                int gdl = tempimage[r + 1][p - 1].rgbtGreen;
                int gds = tempimage[r + 1][p].rgbtGreen;
                int gdr = tempimage[r + 1][p + 1].rgbtGreen;
                int grs = tempimage[0][p + 1].rgbtGreen;
                //blue vars
                int bce = tempimage[0][p].rgbtBlue;
                int bls = tempimage[0][p - 1].rgbtBlue;
                int bdl = tempimage[r + 1][p - 1].rgbtBlue;
                int bds = tempimage[r + 1][p].rgbtBlue;
                int bdr = tempimage[r + 1][p + 1].rgbtBlue;
                int brs = tempimage[0][p + 1].rgbtBlue;

                int avgred = round((rce + rls + rdl + rds + rdr + rrs) / 6.00);
                int avggreen = round((gce + gls + gdl + gds + gdr + grs) / 6.00);
                int avgblue = round((bce + bls + bdl + bds + bdr + brs) / 6.00);
                image[r][p].rgbtRed = avgred;
                image[r][p].rgbtGreen = avggreen;
                image[r][p].rgbtBlue = avgblue;

            }
            //down side
            if ((r == height - 1 && p > 0 && p < width - 1))
            {
                //red vars
                int rce = tempimage[r][p].rgbtRed;
                int rls = tempimage[r][p - 1].rgbtRed;
                int rdl = tempimage[r - 1][p - 1].rgbtRed;
                int rup = tempimage[r - 1][p].rgbtRed;
                int rdr = tempimage[r - 1][p + 1].rgbtRed;
                int rrs = tempimage[r][p + 1].rgbtRed;
                //green vars
                int gce = tempimage[r][p].rgbtGreen;
                int gls = tempimage[r][p - 1].rgbtGreen;
                int gdl = tempimage[r - 1][p - 1].rgbtGreen;
                int gup = tempimage[r - 1][p].rgbtGreen;
                int gdr = tempimage[r - 1][p + 1].rgbtGreen;
                int grs = tempimage[r][p + 1].rgbtGreen;
                //blue vars
                int bce = tempimage[r][p].rgbtBlue;
                int bls = tempimage[r][p - 1].rgbtBlue;
                int bdl = tempimage[r - 1][p - 1].rgbtBlue;
                int bup = tempimage[r - 1][p].rgbtBlue;
                int bdr = tempimage[r - 1][p + 1].rgbtBlue;
                int brs = tempimage[r][p + 1].rgbtBlue;

                int avgred = round((rce + rls + rdl + rup + rdr + rrs) / 6.00);
                int avggreen = round((gce + gls + gdl + gup + gdr + grs) / 6.00);
                int avgblue = round((bce + bls + bdl + bup + bdr + brs) / 6.00);
                image[r][p].rgbtRed = avgred;
                image[r][p].rgbtGreen = avggreen;
                image[r][p].rgbtBlue = avgblue;

            }
            //MIDDLE CASE
            if (r > 0  && r < height - 1 && p > 0 && p < width - 1)
            {
                //red vars
                int rce = tempimage[r][p].rgbtRed;
                int rls = tempimage[r][p - 1].rgbtRed;
                int rdl = tempimage[r - 1][p - 1].rgbtRed;
                int rup = tempimage[r - 1][p].rgbtRed;
                int rdr = tempimage[r - 1][p + 1].rgbtRed;
                int rrs = tempimage[r][p + 1].rgbtRed;
                int rrd = tempimage[r + 1][p + 1].rgbtRed;
                int rdw = tempimage[r + 1][p].rgbtRed;
                int rld = tempimage[r + 1][p - 1].rgbtRed;
                //green vars
                int gce = tempimage[r][p].rgbtGreen;
                int gls = tempimage[r][p - 1].rgbtGreen;
                int gdl = tempimage[r - 1][p - 1].rgbtGreen;
                int gup = tempimage[r - 1][p].rgbtGreen;
                int gdr = tempimage[r - 1][p + 1].rgbtGreen;
                int grs = tempimage[r][p + 1].rgbtGreen;
                int grd = tempimage[r + 1][p + 1].rgbtGreen;
                int gdw = tempimage[r + 1][p].rgbtGreen;
                int gld = tempimage[r + 1][p - 1].rgbtGreen;
                //blue vars
                int bce = tempimage[r][p].rgbtBlue;
                int bls = tempimage[r][p - 1].rgbtBlue;
                int bdl = tempimage[r - 1][p - 1].rgbtBlue;
                int bup = tempimage[r - 1][p].rgbtBlue;
                int bdr = tempimage[r - 1][p + 1].rgbtBlue;
                int brs = tempimage[r][p + 1].rgbtBlue;
                int brd = tempimage[r + 1][p + 1].rgbtBlue;
                int bdw = tempimage[r + 1][p].rgbtBlue;
                int bld = tempimage[r + 1][p - 1].rgbtBlue;

                int avgred = round((rce + rls + rdl + rup + rdr + rrs + rrd + rdw + rld) / 9.00);
                int avggreen = round((gce + gls + gdl + gup + gdr + grs + grd + gdw + gld) / 9.00);
                int avgblue = round((bce + bls + bdl + bup + bdr + brs + brd + bdw + bld) / 9.00);
                image[r][p].rgbtRed = avgred;
                image[r][p].rgbtGreen = avggreen;
                image[r][p].rgbtBlue = avgblue;

            }

        }
    }
    return;
}
