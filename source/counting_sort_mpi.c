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
 * @file    counting_sort_mpi.c
 * @author  Alessio Pepe         (a.pepe108@studenti.unisa.it)
 * @author  Paolo Mansi          (p.mansi5@studenti.unisa.it)
 * @author  Teresa Tortorella    (t.tortorella3@studenti.unisa.it)
 * @brief   Counting sort alghorithm in mpi version.  
 * @version 1.0.0
 * @date 2021-12-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "counting_sort_mpi.h"
#include "util.h"


void minmax_mpi(int *V, size_t len, int *min, int *max);


/**
 * @brief   Implementation of counting sort using MPI. Order an array writed in a byte file. 
 * 
 * @param fileName      The path to the file containing an array of integert to order (in bytes).
 * @param arrayLen      The lenght of the array in the file.
 */
void counting_sort_mpi(const char *fileName, size_t arrayLen) {
    // Empty or lenght=1 array
    if (arrayLen < 2) return;

    int locCDFLen, locCDFOffset, locWriteSize, max, min, realLocCDFLen, worldRank, worldSize;
    int *locArray, *locCDFLens, *locCDFDisplacement, *locOut;
    size_t locArrayLen, locArrayStartOffset, locArrayEndOffset, CLen;
    size_t *locC, *C;
    MPI_File fh;  

    // ------------------- Init MPI environment ----------------------
    // Get worldRank and worldSize of global communicator
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
    MPI_Status status;
    // ---------------------------------------------------------------
    
    // ------------------- Read array from file ----------------------
    // Open file in read and write mode.
    MPI_File_open(MPI_COMM_WORLD, fileName, MPI_MODE_RDWR, MPI_INFO_NULL, &fh);

    // Compute the offset to read file
    locArrayLen = arrayLen / worldSize;  // Divide array in equal part
    locArrayStartOffset = locArrayLen * worldRank;  // Offset start based on thread rank
    if (worldRank == worldSize - 1)
    {
        locArrayLen += arrayLen % worldSize;  // Last thread manage the last element of the file
    }

    // Allocate the local array who contains the part of the array for that rank 
    locArray = malloc(locArrayLen * sizeof(int));

    // Each thread read a part of the vector
    MPI_File_seek(fh, locArrayStartOffset * sizeof(int), MPI_SEEK_SET);
    MPI_File_read(fh, locArray, locArrayLen, MPI_INT, MPI_STATUS_IGNORE);
    // ---------------------------------------------------------------

    // ------------------- Compute min & max -------------------------
    // Find global min and max and broadcast that to all thread
    minmax_mpi(locArray, locArrayLen, &min, &max);
    // ---------------------------------------------------------------

    // ------------------- Compute the frequency vector --------------
    // Compute the lenght of the pmf array
    CLen = max - min + 1;

    // In version 1 just the master have the complete pmf array. That compute the CDF array anf
    // distribute that to the various thread. 
    // In version 2 all thread receive the pmf array and each one compute the needed part of CDF 
    // array.
#if VERSION == 1
    // Just master thread allocate the global pmf array (plus one because we want transform that in a CDF array with a starting 0).
    if (worldRank == 0) 
    {
        C = (size_t *) calloc(CLen+1, sizeof(size_t));
    }
#endif

    // Each thread allocate a local C array (plus one element for same reason)
#if VERSION == 1
    locC = (size_t *) calloc(CLen+1, sizeof(size_t));
#elif VERSION == 2
    locC = (size_t *) calloc(CLen, sizeof(size_t));
#endif
    
    // Count the local pmf array.
    for (size_t i = 0; i < locArrayLen; i++)
    {
        locC[locArray[i] - min]++;
    }

    // Dealloc no more useful local array
    free(locArray);

#if VERSION == 1
    // Reduce to global pmf array (starting from the second element of C because we need the first equal to one).
    MPI_Reduce(locC, C+1, CLen, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
#elif VERSION == 2
    C = (size_t *) calloc(CLen, sizeof(size_t));
    MPI_Allreduce(locC, C, CLen, MPI_LONG, MPI_SUM, MPI_COMM_WORLD);
    free(locC);
#endif
    // ---------------------------------------------------------------

    // ------------------- Compute the CDF vector --------------------
#if VERSION == 1
    // Master thread compute the CDF array
    if (worldRank == 0) 
    {
        for (size_t i = 1; i < CLen+1; i++)
        {
            C[i] += C[i-1];
        }
        
        // Allocate locCDFLen and locCDFDisplacement array used below for the scatterv.
        locCDFLens = malloc(worldSize * sizeof(int));
        locCDFDisplacement = malloc(worldSize * sizeof(int));

        // Compute the lenght and the displacement for the scatter v with an overlap of the last sample.
        // e.g., if there are 3 threads and a cdf array like the following: 0 1 4 5 6 10 14 16 18 18 19 21 
        //       we want the following situation for each thread:
        //        - thread 0:   0  1  4  5 
        //        - thread 1:   5  6 10 14
        //        - thread 2:  14 16 18 18 19 21
        int increment = 1;
        realLocCDFLen = CLen / worldSize;

        for(int processID = 0; processID < worldSize; processID++)
        {
            locCDFDisplacement[processID] = increment - 1;  // Minus one because we need the first element equal to the last of the previous thread
            locCDFLens[processID] = realLocCDFLen + 1;  // Plus one for the same reason
            increment += realLocCDFLen;
        }
        locCDFLens[worldSize-1] += CLen % worldSize;  // last thread manage the last element non prefectly divided 
    }
    // ---------------------------------------------------------------

    // ------------------- Split & send CDF array --------------------
    realLocCDFLen = CLen / worldSize;

    // lenght of the CDF array to each thread (lenght can be different because last thread have problably more element).
    if (worldRank == worldSize-1)
    {
        locCDFLen = realLocCDFLen + 1 + CLen % worldSize;
    }
    else
    {
        locCDFLen = realLocCDFLen + 1;
    }

#elif VERSION == 2
    realLocCDFLen = CLen / worldSize;

    if (worldRank == worldSize-1)
    {
        locCDFLen = realLocCDFLen + CLen % worldSize;
    }
    else
    {
        locCDFLen = realLocCDFLen;
    }

    for (size_t i = 1; i < worldRank * realLocCDFLen + locCDFLen; i++)
    {
        C[i] += C[i-1];
    }
#endif

#if VERSION == 1
    // Divide the CDF array on the different threads
    MPI_Scatterv(C, locCDFLens, locCDFDisplacement,  MPI_LONG, locC, locCDFLen, MPI_LONG, 0, MPI_COMM_WORLD); 
    
    // Dealloc CDF array needed to send and the C array
    if (worldRank == 0) 
    {
        free(locCDFLens);
        free(locCDFDisplacement);
        free(C);
    }
#endif
    // ---------------------------------------------------------------

    // ----------------- Write the correct result --------------------

    // Each thread compute the part of the array to write
#if VERSION == 1
    locWriteSize = (locC[locCDFLen-1] - locC[0]);

    locOut = malloc( locWriteSize * sizeof(int));

    int k = 0;
    for (size_t i = 1; i < locCDFLen; i++)
    {
        for (size_t j = locC[i-1]; j < locC[i]; j++)
        {
            locOut[k++] = worldRank * realLocCDFLen + min + i - 1;
        }
    }

    // Each thread compute the offset to write the ordered elements in the file based on the local CDF array
    MPI_File_seek(fh, locC[0] * sizeof(int), MPI_SEEK_SET);

#elif VERSION == 2
    if (worldRank == 0)
    {
        locWriteSize = C[worldRank*realLocCDFLen+locCDFLen-1] - 0;
    }
    else
    {
        locWriteSize = C[worldRank*realLocCDFLen+locCDFLen-1] - C[worldRank*realLocCDFLen-1];
    }

    locOut = malloc(locWriteSize * sizeof(int));

    int k = 0;
    for (size_t i = worldRank*realLocCDFLen; i < worldRank*realLocCDFLen+locCDFLen; i++)
    {
        for (size_t j = (i != 0 ? C[i-1] : 0); j < C[i]; j++)
        {
            locOut[k++] = min + i;
        }
    }

    if (worldRank == 0)
    {
        MPI_File_seek(fh, 0, MPI_SEEK_SET);
    }
    else
    {
        MPI_File_seek(fh, C[worldRank*realLocCDFLen-1] * sizeof(int), MPI_SEEK_SET);
    }
#endif

    // Write on file
    MPI_File_write_all(fh, locOut, locWriteSize, MPI_INT, MPI_STATUS_IGNORE);

    // Close the file.
    MPI_File_close(&fh);

    // Dealloc the writed element
    free(locOut);

#if VERSION == 1
    // Each thread dealloc their local C
    free(locC);
#endif

#if VERSION == 2
    free(C);
#endif
}


void minmax_mpi(int *V, size_t len, int *min, int *max){
    int lmin = V[0];
    int lmax = V[0];

    for (size_t i = 1; i < len; i++)
    {
        if (V[i] < lmin)
        {
            lmin = V[i];  // update min
        } 
        else if (V[i] > lmax)
        {
            lmax = V[i];  // update max
        }
    }
    
    MPI_Allreduce(&lmin, min, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&lmax, max, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
}
