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
 * @file    counting_sort_mpi.h
 * @author  Alessio Pepe         (a.pepe108@studenti.unisa.it)
 * @author  Paolo Mansi          (p.mansi5@studenti.unisa.it)
 * @author  Teresa Tortorella    (t.tortorella3@studenti.unisa.it)
 * @brief   Counting sort alghorithm in mpi version.  
 * @version 1.0.0
 * @date 2022-01-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _DEF_COUNTINGSORT_MPI_H_
#define _DEF_COUNTINGSORT_MPI_H_

#define TIME_MEASURES

#ifdef TIME_MEASURES
    void counting_sort_mpi(const char *fileName, size_t arrayLen, double* t1, double* t2, double* t3,double* t4,double* t5,double* t6);
    void counting_sort_mpi2(const char *fileName, size_t arrayLen, double* t1, double* t2, double* t3,double* t4,double* t5,double* t6);
#else
    void counting_sort_mpi(const char *fileName, size_t arrayLen);
    void counting_sort_mpi2(const char *fileName, size_t arrayLen);
#endif

#endif /*_DEF_COUNTINGSORT_MPI_H_*/
