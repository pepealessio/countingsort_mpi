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
 * @file    util.c
 * @author  Alessio Pepe         (a.pepe108@studenti.unisa.it)
 * @author  Paolo Mansi          (p.mansi5@studenti.unisa.it)
 * @author  Teresa Tortorella    (t.tortorella3@studenti.unisa.it)
 * @brief   This file contains utility function used to work with random array.  
 * @version 1.0.0
 * @date 2022-01-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "util.h"

/**
 * @brief Comment this line if you want generate every time a pseudo-random array.
 * 
 */
#define FIXED_ARRAY

/**
 * @brief   Initialize an array of a certain lenght with random integers in a 
 *          certain range. 
 * 
 * @param file_name     The file name into write the array.
 * @param len           The desidered lenght of the array. 
 * @param min_value     The minimum value inside the array.  
 * @param max_value     The maximum value inside the array.
 */
void generate_rand_vector(const char *file_name, size_t len, long min_value, long max_value)
{
    int tmp;
    FILE *fptr;

    fptr = fopen(file_name, "wb");

    #ifdef FIXED_ARRAY
    srand(1256765);
    #endif

    for (size_t i = 0; i < len; i++)
    {
        tmp = min_value + (rand() % (max_value - min_value + 1));
        //tmp = i;
        fwrite(&tmp, sizeof(int), 1, fptr);
    }

    fclose(fptr);
}
