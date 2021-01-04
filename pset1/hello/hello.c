#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //original print out of hello
    //printf("hello, world\n");

    //get users name
    string name = get_string("What is your name?\n");

    //print out helllo, <users name>
    printf("hello, %s\n", name);
}