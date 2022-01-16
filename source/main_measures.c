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
 * @file    main_measures.c
 * @author  Alessio Pepe         (a.pepe108@studenti.unisa.it)
 * @author  Paolo Mansi          (p.mansi5@studenti.unisa.it)
 * @author  Teresa Tortorella    (t.tortorella3@studenti.unisa.it)
 * @brief   Main to measure the execution time of the counting sort mpi version algorithm.
 * @version 1.0.0
 * @date 2022-01-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "counting_sort_mpi.h"
#include "util.h"


int main(int argc, char *argv[])
{
    // Check parameter usage
    if (argc != 4)
    {
        printf("USAGE: %s file_name arrayLen algo_version\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Read parameters from argv
    const char *fileName = argv[1];
    size_t arrayLen = atoi(argv[2]);
    int algoVersion = atoi(argv[3]);
    double timeAlgo, locTimeAlgo;
    double t1, t2, t3, t4, t5, t6;
    double loc_t1, loc_t2, loc_t3, loc_t4, loc_t5, loc_t6;

    // Init MPI environment
    MPI_Init(NULL, NULL);   

    int worldRank, worldSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    STARTTIME(0); 
#ifdef TIME_MEASURES
    if (algoVersion == 1)
    {
        counting_sort_mpi(fileName, arrayLen, &loc_t1, &loc_t2, &loc_t3, &loc_t4, &loc_t5, &loc_t6);
    }
    else if (algoVersion == 2) 
    {
        counting_sort_mpi2(fileName, arrayLen, &loc_t1, &loc_t2, &loc_t3, &loc_t4, &loc_t5, &loc_t6);
    }
#else
    if (algoVersion == 1)
    {
        counting_sort_mpi(fileName, arrayLen);
    }
    else if (algoVersion == 2) 
    {
        counting_sort_mpi2(fileName, arrayLen);
    }
#endif
    ENDTIME(0, locTimeAlgo);

    MPI_Reduce(&locTimeAlgo, &timeAlgo, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
#ifdef TIME_MEASURES
    MPI_Reduce(&loc_t1, &t1, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&loc_t2, &t2, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&loc_t3, &t3, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&loc_t4, &t4, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&loc_t5, &t5, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&loc_t6, &t6, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
#endif
    if (worldRank == 0)
    {
        // Expected in output: t_algo\n
    #ifdef TIME_MEASURES
        printf("%f,%f,%f,%f,%f,%f,%f\n", timeAlgo, t1, t2 ,t3 ,t4 ,t5 ,t6);
    #else
        printf("%f\n", timeAlgo);
    #endif
    }

    // End MPI environment 
    MPI_Finalize();

    return EXIT_SUCCESS;
}
