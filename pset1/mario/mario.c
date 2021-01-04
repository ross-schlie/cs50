#include <stdio.h>
#include <cs50.h>

void drawRow(int maxHeight, int height);
void drawCol(int maxwidth, int width, bool isLeft);
void drawFiller(int width);

int main(void)
{
    //intialize to 0 and while/do condition
    int height = 0;
    bool isLess = true;
    bool isMore = true;

    //until user enters a int between 1 and 8, prompt them
    do
    {
        //ask the user the pyramids height
        height = get_int("How high (1-8)? ");

        isLess = (height <= 8);
        isMore = (height >= 1);

        //debug the int the user entered
        //printf("is less than specified: %i\n", isLess);
        //printf("is more than specified: %i\n", isMore);
    }
    while (!isLess || !isMore);

    //feedback on height requested (seemed like this was requested, but checker rejects it)
    //printf("Height: %i\n", height);

    //recursevily print out each row (rows if more than 1)
    drawRow(height, height);

}

//draw a row of the pyramid, if this is not the last row, draw other rows first;
//otherwise draw the left colums, spacer, then right columns, finaly a newline
void drawRow(int maxHeight, int height)
{
    //recurse first since we want the pyramid to go from thin to wide
    if (height > 1)
    {
        drawRow(maxHeight, height - 1);
    }

    //draw the left side of the pyramid
    //we need to draw space to indent the left
    drawFiller(maxHeight - height);
    drawCol(maxHeight, height, true);

    //spacer
    printf("  ");

    //draw right side of the pyramid
    drawCol(maxHeight, height, false);

    //newline
    printf("\n");
}

//draw the columns to make up a row of the pyramid, ???
void drawCol(int maxWidth, int width, bool isLeft)
{
    //recurse since we want the pyramid to go from thin to wide
    if (width > 1)
    {
        drawCol(maxWidth, width - 1, isLeft);
    }

    printf("#");
}

//draw space which appear before or after the #
void drawFiller(int width)
{
    //debug to check the spaces drawn
    //printf("%i", width);

    //draw spaces!
    for (int i = 0; i < width; i++)
    {
        printf(" ");
    }
}