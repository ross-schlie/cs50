#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>
#include <string.h>

typedef uint8_t BYTE;

typedef struct
{
    BYTE   first;
    BYTE   second;
    BYTE   third;
    BYTE   fourth;
}
JPGFILEHEADER;

int readSize = 512;
JPGFILEHEADER checkJpgSuspect(char buffer[readSize]);

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open input file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    // read 512 of them at a time into a buffer for efficiency’s sake.
    // Thanks to FAT, you can trust that JPEGs’ signatures will be “block-aligned.”
    // That is, you need only look for those signatures in a block’s first four bytes.
    char buffer[readSize];

    //output file
    int imagesFound = 0;
    char  outfile[8];
    sprintf(outfile, "%03i.jpg", imagesFound);

    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(file);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    bool foundFirstHeader = false;
    bool foundNextHeader = false;

    //read 512 bytes at a time (header + contents)
    int numRead = fread(&buffer, readSize, 1, file);
    do
    {
        // the first three bytes of JPEGs are
        // 0xff 0xd8 0xff
        // from first byte to third byte, left to right
        //  the fourth byte’s first four bits are 1110
        // start of a JPEG usually demarks the end of another
        JPGFILEHEADER jpgSuspect = checkJpgSuspect(buffer);
        // printf("DEBUG START OF block: %i %i %i %i \n", jpgSuspect.first, jpgSuspect.second, jpgSuspect.third, jpgSuspect.fourth);
        if (jpgSuspect.first == 0xff && jpgSuspect.second == 0xd8 && jpgSuspect.third == 0xff && (jpgSuspect.fourth >= 0xe0
                && jpgSuspect.fourth <= 0xef))
        {
            // printf("DEBUG probably a jpg!\n");
            //if we found a header, then read 512 bytes at a time into buffer
            //until we find a new header. then close and write buffer as new image
            if (foundFirstHeader)
            {
                //close previous
                fclose(outptr);
            }
            else
            {
                foundFirstHeader = true;
            }

            // Open (new) output file
            sprintf(outfile, "%03i.jpg", imagesFound);
            outptr = fopen(outfile, "w");
            if (outptr == NULL)
            {
                fclose(file);
                fprintf(stderr, "Could not create %s.\n", outfile);
                return 4;
            }

            imagesFound++;
        }

        if (foundFirstHeader)
        {
            fwrite(&buffer, readSize, 1, outptr);
        }

        //read next
        numRead = fread(&buffer, readSize, 1, file);
    }
    while (numRead == 1);
    //read 512 bytes at a time (header + contents)
    //If an error occurs, or the end of the file is reached, the return value is a short item count (or zero).

    //cleanup
    fclose(outptr);
    fclose(file);
    return 0;
}

JPGFILEHEADER checkJpgSuspect(char buffer[readSize])
{
    JPGFILEHEADER jpgSuspect;
    memcpy(&jpgSuspect, buffer, sizeof(JPGFILEHEADER));
    return jpgSuspect;
}