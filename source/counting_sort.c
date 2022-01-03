// Course:           High Performance Computing
// A.Y:              2021/22
// Lecturer:         Francesco Moscato           fmoscato@unisa.it

// Team:
// Alessio Pepe          0622701463      a.pepe108@studenti.unisa.it
// Teresa Tortorella     0622701507      t.tortorella3@studenti.unisa.it
// Paolo Mansi           0622701542      p.mansi5@studenti.unisa.it

// Copyright (C) 2021 - All Rights Reserved

// This file is part of Counting_Sort.

// Counting_Sort is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Counting_Sort is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with Counting_Sort.  If not, see <http://www.gnu.org/licenses/>.

/**
 * @file    counting_sort.c
 * @author  Alessio Pepe         (a.pepe108@studenti.unisa.it)
 * @author  Paolo Mansi          (p.mansi5@studenti.unisa.it)
 * @author  Teresa Tortorella    (t.tortorella3@studenti.unisa.it)
 * @brief   Counting sort alghorithm in serial version. That are inspired 
 *          at https://it.wikipedia.org/wiki/Counting_sort.   
 * @version 1.0.0
 * @date 2021-12-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include "counting_sort.h"
#include "util.h"


/**
 * @brief   Reorder the array using a counter-sort alghorithm.
 * 
 * @param file_name     The file name into write the array.
 * @param len           The lenght to the array to reorder.
 */
#ifdef TIME_MEASURES
void counting_sort(const char *file_name, size_t len, double* t1, double* t2, double* t3, double* t4,double* t5) 
#else
void counting_sort(const char *file_name, size_t len)
#endif
{
    // Empty or lenght=1 array
    if (len < 2) return;

    FILE *fptr;
    int *A;
    int min, max;
    size_t *C;
    size_t C_len, k;
    A = malloc(len * sizeof(int));
#ifdef TIME_MEASURES
    STARTTIME(1);
#endif
    // Read array from file
    fptr = fopen(file_name, "rb");
    fread(A, sizeof(int), len, fptr);
    fclose(fptr);
#ifdef TIME_MEASURES
    ENDTIME(1, *t1);
#endif

    //Compute max and min of v.
#ifdef TIME_MEASURES
    STARTTIME(2);
#endif
    min = A[0];
    max = A[0];

    for (size_t i = 1; i < len; i++)
    {
        if (A[i] < min)
        {
            min = A[i];
        }
        else if (A[i] > max)
        {
            max = A[i];
        }
    }
#ifdef TIME_MEASURES
    ENDTIME(2, *t2);
#endif

    // Construct a zeros array of lenght max-min+1.
#ifdef TIME_MEASURES
    STARTTIME(3);
#endif
    C_len = max - min + 1;
    C = (size_t *) calloc(C_len, sizeof(size_t));

    // Count the element frequency.
    for (size_t i = 0; i < len; i++)
    {
        C[A[i] - min]++;
    }
#ifdef TIME_MEASURES
    ENDTIME(3, *t3);
#endif


    // Ordering based on the frequency array.
#ifdef TIME_MEASURES
    STARTTIME(4);
#endif
    k = 0;

    for (size_t i = 0; i < C_len; i++)
    {
        for (size_t j = 0; j < C[i]; j++)
        {
            A[k++] = i + min;
        }
    }
    
    free(C);    // Dealloc the frequency vector.
#ifdef TIME_MEASURES
    ENDTIME(4, *t4);

    STARTTIME(5);
#endif
    fptr = fopen(file_name, "wb");
    fwrite(A, sizeof(int), len, fptr);
    fclose(fptr);
#ifdef TIME_MEASURES
    ENDTIME(5, *t5);
#endif
}

