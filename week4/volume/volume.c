// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    uint8_t* header_buffer[HEADER_SIZE];
    fread(header_buffer, HEADER_SIZE, 1, input);
    fwrite(header_buffer, HEADER_SIZE, 1, output);

    // TODO: Read samples from input file and write updated data to output file
    int16_t sample_buffer;
    while(fread(&sample_buffer, sizeof(int16_t), 1, input))
    {
        sample_buffer = (sample_buffer * factor);
        fwrite(&sample_buffer, sizeof(int16_t), 1, output);
    }
    // why didn't eoff work?
    // Close files
    fclose(input);
    fclose(output);
}
