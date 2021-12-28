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
 * @file    util.h
 * @author  Alessio Pepe         (a.pepe108@studenti.unisa.it)
 * @author  Paolo Mansi          (p.mansi5@studenti.unisa.it)
 * @author  Teresa Tortorella    (t.tortorella3@studenti.unisa.it)
 * @brief   This file contains utility function prototypes used to work with 
 *          random array and the declaration of the type you want use in the 
 *          counting sort algorithm.  
 * @version 1.0.0
 * @date 2021-11-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _INC_TYPEH_
#define _INC_TYPEH_


#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

/**
 * @brief Use start_time with an non used id to start measure time in that point of the code.
 * 
 */
#define STARTTIME(id)                             \
   struct timeval start_time_##id, end_time_##id; \
   gettimeofday(&start_time_##id, NULL);

/**
 * @brief Use end_tipe with a previous used id to stop measure time in that point of the code.
 *        The value of time will be saved in x.
 * 
 */
#define ENDTIME(id, x)                 \
   gettimeofday(&end_time_##id, NULL); \
   x = ((end_time_##id.tv_sec  - start_time_##id.tv_sec) * 1000000u +  end_time_##id.tv_usec - start_time_##id.tv_usec) / 1.e6;


void generate_rand_vector(const char *file_name, size_t len, long min_value, long max_value);


#endif /*_INC_TYPEH_*/
