#include "helpers.h"
#include "math.h"
#include <stdio.h>

int sobel (int Gx, int Gy);
int clamp (int num, int upper, int lower);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // over 3.0 to allow decimals so we can do more complex rounding. is there a better way?
            int avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.00);
            
            // set each color value to the average of the three
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        // only halfway left to right, as the swap is bi-directional
        // if width is odd, the middle pixel will be skipped using this setup
        // which is desired behavior (all other pixels pivot over the center)
        for (int j = 0; j < width/2; j++)
        {   
            // mirror the pixels
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // writing changes to a new array so pixels are not influenced by blurred pixels around them
    RGBTRIPLE blurred[height][width];
    // set the constant blur radius (not a radius I guess since it's a box blur but w/e)
    // Default (based on problem spec) is 1 but works any int of 0 up to the smaller image dimension
    // Bigger numbers hurt the computer brainz though
    int BLUR_RADIUS = 1;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // for each pixel, initialize RBG value totals and a count of pixels counted around them
            // floats to fix rounding issue?
            float blue_total = 0;
            float red_total = 0;
            float green_total = 0;
            float pixels = 0;

            // go through rows above through below the center pixel based on radius
            for (int k = 0 - BLUR_RADIUS; k <= BLUR_RADIUS; k++)
            {
                // skip the row if it's above or below the edge of the image
                if (i + k < 0 || i + k >= height)
                {
                    continue;
                }
                else
                {
                    // go over pixels left through right of the center pixel based on radius
                    for (int l = 0 - BLUR_RADIUS; l <= BLUR_RADIUS; l++)
                    {
                        // skip pixels to the left or right of the edge of the image
                        if (j + l < 0 || j + l >= width)
                        {
                            continue;
                        }
                        else
                        {
                            // add the pixel's R, G, and B values to the totals
                            blue_total += image[i + k][j + l].rgbtBlue;
                            red_total += image[i + k][j + l].rgbtRed;
                            green_total += image[i + k][j + l].rgbtGreen;
                            // and add to the pixels referenced count
                            pixels++;
                        }
                    }
                }
            }
            // use the averages of all surrounding pixels to set R, G, and B values
            blurred[i][j].rgbtBlue = round((blue_total / pixels));
            blurred[i][j].rgbtRed = round((red_total / pixels));
            blurred[i][j].rgbtGreen = round((green_total / pixels));
        }
    }
    
    // point the image pixels to the new blurred two dimension array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blurred[i][j];
        }
    }

    return;
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // write to a temp to prevent influencing outcome as we go
    RGBTRIPLE edged[height][width];

    // init kernels
    int Gx_KERNEL[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy_KERNEL[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // RGBTRIPLE Gx = { .rgbtBlue = 0, .rgbtRed = 0, .rgbtGreen = 0 };
            // RGBTRIPLE Gy = { .rgbtBlue = 0, .rgbtRed = 0, .rgbtGreen = 0 };
            // using this as an organizer was good for code legibility but because each value is only 1 byte
            // wont work because these totals run far above 255

            int Gx_blue = 0,
                Gx_red = 0,
                Gx_green = 0,
                Gy_blue = 0,
                Gy_red = 0,
                Gy_green = 0;
                
            for (int k = -1; k <= 1; k++)
            {
                if (i + k < 0 || i + k >= height)
                {
                    continue;
                }
                else
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        if (j + l < 0 || j + l >= width)
                        {
                            continue;
                        }   
                        Gx_blue += (image[i + k][j + l].rgbtBlue) * Gx_KERNEL[k + 1][l + 1];
                        Gx_red += (image[i + k][j + l].rgbtRed) * Gx_KERNEL[k + 1][l + 1];
                        Gx_green += (image[i + k][j + l].rgbtGreen) * Gx_KERNEL[k + 1][l + 1];
                        Gy_blue += (image[i + k][j + l].rgbtBlue) * Gy_KERNEL[k + 1][l + 1];
                        Gy_red += (image[i + k][j + l].rgbtRed) * Gy_KERNEL[k + 1][l + 1];
                        Gy_green += (image[i + k][j + l].rgbtGreen) * Gy_KERNEL[k + 1][l + 1];
                    }
                }
            }
            // process each into a final byte value
            edged[i][j].rgbtBlue = sobel(Gx_blue, Gy_blue);
            edged[i][j].rgbtRed = sobel(Gx_red, Gy_red);
            edged[i][j].rgbtGreen = sobel(Gx_green, Gy_green);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = edged[i][j];
        }
    }

    return;
}

// sobel algo, then clamp to byte limit 
int sobel (int Gx, int Gy)
{
    int result = round(sqrt(pow(Gx, 2) + pow(Gy, 2)));
    return clamp(result, 0, 255); 
}

// clamp an int num by an upper and lower bound
int clamp (int num, int lower, int upper)
{
    // not sure how to throw an error outside main? 
    // but to generalize the func it should error check lower > upper, etc
    if (num > upper)
    {
        num = upper;
    }
    if (num < lower)
    {
        num = lower;
    }
    return num;
}