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
 * @file    main_measures_sequential.c
 * @author  Alessio Pepe         (a.pepe108@studenti.unisa.it)
 * @author  Paolo Mansi          (p.mansi5@studenti.unisa.it)
 * @author  Teresa Tortorella    (t.tortorella3@studenti.unisa.it)
 * @brief   Main to measure the execution time of the counting sort sequential version algorithm.
 * @version 1.0.0
 * @date 2021-12-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include "counting_sort.h"
#include "util.h"


int main(int argc, char *argv[])
{
    // Check parameter usage
    if (argc != 3)
    {
        printf("USAGE: %s file_name arrayLen\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Read parameters from argv
    const char *fileName = argv[1];
    size_t arrayLen = atoi(argv[2]);
    double time_algo=0.0;

    STARTTIME(0); 
    counting_sort(fileName, arrayLen);
    ENDTIME(0, time_algo);

    // Expected in output:
    // t_algo 
    printf("%f\n", time_algo);

    return EXIT_SUCCESS;
}
