#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        write(2, "usage: ", 7);
        write(2, argv[0], strlen(argv[0]));
        write(2, " [files].", 9);
    }
}
