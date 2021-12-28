#include <stdlib.h>
#include <stdio.h>


int main(int argc, char const *argv[])
{
    FILE *fptr;
    int A[atoi(argv[1])];

    fptr = fopen("prova", "rb");
    fread(A, sizeof(int), atoi(argv[1]), fptr);

    fclose(fptr);

    for (size_t i = 0; i < atoi(argv[1]); i++)
    {
        printf("%d ", A[i]);
    }
    printf("\n");
}
