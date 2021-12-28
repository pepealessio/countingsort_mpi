#include <stdio.h>
#include <stdlib.h>
#include "util.h"


int main(int argc, char const *argv[])
{
    if (argc != 5)
    {
        printf("USAGE: %s file_name len min_value max_value\n", argv[0]);
        return (EXIT_FAILURE);
    }

    const char *file_name = argv[1];
    size_t len = atoi(argv[2]);
    int min_value = atoi(argv[3]);
    int max_value = atoi(argv[4]);

    generate_rand_vector(file_name, len, min_value, max_value);    

    return EXIT_SUCCESS;
}
