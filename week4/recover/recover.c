#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// number of bytes in a file system block
// 512 in the case of FAT32
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("usage: ./recover <forensic raw>\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    uint8_t block_buffer[BLOCK_SIZE];
    int image_counter = 0;

    FILE *output_image = NULL;
    char *output_filename = malloc(8);

    while (fread(block_buffer, 1, BLOCK_SIZE, input) == BLOCK_SIZE)
    {
        // this is the 'fingerprint' of a jpg file as specified
        // single & is a 'bitwise operator'
        // we use it to 'mask' the end of the byte
        // 1110 xxxx &
        // 1111 0000 =
        // 1110 0000
        // so check if result equal 1110 0000 means if first 4 bits were 1110
        if (block_buffer[0] == 0xff && block_buffer[1] == 0xd8 && block_buffer[2] == 0xff && (block_buffer[3] & 0xf0) == 0xe0)
        {
            // formatted string from counter for filename requiring 3 digits (so) "xxx.jpg\0"
            // apparently has to start at 000!
            sprintf(output_filename, "%03i.jpg", image_counter);
            output_image = fopen(output_filename, "w");
            image_counter++;
        }

        // if output image has been initialized, write chunks to it
        if (output_image != NULL)
        {
            fwrite(block_buffer, 1, 512, output_image);
        }

        // this loop assumes there's no garbage chunks between images
    }

    // cleanup
    fclose(output_image);
    fclose(input);
    free(output_filename);

    return 0;
}