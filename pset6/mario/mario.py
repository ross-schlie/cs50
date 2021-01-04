from cs50 import get_int

def main():
    while(True):
        height = get_int("Height: ")
        if height > 0 and height < 9:
            break

    drawRow(height, height);

def drawRow(maxHeight, height):
    if height > 1:
        drawRow(maxHeight, height - 1)

    drawFiller(maxHeight - height)
    drawCol(maxHeight, height, True)

    # spacer
    print("  ", end="")

    # draw right side of the pyramid
    drawCol(maxHeight, height, False)

    # newline
    print("")

def drawCol(maxWidth, width, isLeft):
    if width > 1:
        drawCol(maxWidth, width - 1, isLeft)

    print("#", end="")

def drawFiller(width):
    for i in range(width):
        print(" ", end="")

main()