#include <stdlib.h>
#include <stdio.h>


int main(int argc, char const *argv[])
{
    // Check parameter usage
    if (argc != 3)
    {
        printf("USAGE: %s fileName arrayLen\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *fptr;
    const char *fileName = argv[1];
    size_t len = atoi(argv[2]);
    int *A;

    A = (int *) malloc(len * sizeof(int));

    fptr = fopen(fileName, "rb");
    fread(A, sizeof(int), len, fptr);

    fclose(fptr);

    for (size_t i = 0; i < len; i++)
    {
        printf("%d ", A[i]);
    }
    printf("\n");
}
