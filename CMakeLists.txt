# Course:           High Performance Computing
# A.Y:              2021/22
# Lecturer:         Francesco Moscato           fmoscato@unisa.it

# Team:
# Alessio Pepe          0622701463      a.pepe108@studenti.unisa.it
# Teresa Tortorella     0622701507      t.tortorella3@studenti.unisa.it
# Paolo Mansi           0622701542      p.mansi5@studenti.unisa.it

# Copyright (C) 2021 - All Rights Reserved

# This file is part of Counting_Sort.

# Counting_Sort is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# Counting_Sort is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with Counting_Sort.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.16.3)
project(Counting_Sort VERSION 1.0.0)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/source
)

find_package(MPI REQUIRED)

# util lib
add_library(util include/util.h source/util.c)

# counting sort lib
add_library(counting_sort include/counting_sort.h source/counting_sort.c)

add_library(counting_sort_mpi include/counting_sort_mpi.h source/counting_sort_mpi.c)
target_link_libraries(counting_sort_mpi PUBLIC MPI::MPI_C)

# generate file
add_executable(generate source/generate.c)
target_link_libraries(generate PRIVATE util)

# read file
add_executable(read_file source/read_file.c)

# main measures
add_executable(main_measures_O2 source/main_measures.c)
target_compile_options(main_measures_O2 PRIVATE -O2)
target_link_libraries(main_measures_O2 PRIVATE util counting_sort_mpi)
target_link_libraries(main_measures_O2 PUBLIC MPI::MPI_C)

add_executable(main_measures_seq_O2 source/main_measures_sequential.c)
target_compile_options(main_measures_seq_O2 PRIVATE -O2)
target_link_libraries(main_measures_seq_O2 PRIVATE util counting_sort)


# add_executable(main_measures_O0 source/main_measure.c)
# target_link_libraries(main_measures_O0 PUBLIC OpenMP::OpenMP_C)
# target_compile_options(main_measures_O0 PRIVATE -O0)
# target_link_libraries(main_measures_O0 PRIVATE util counting_sort)

# add_executable(main_measures_seq_O0 source/main_measure.c)
# target_compile_options(main_measures_seq_O0 PRIVATE -O0)
# target_link_libraries(main_measures_seq_O0 PRIVATE util_seq counting_sort_seq)

# add_executable(main_measures_O1 source/main_measure.c)
# target_link_libraries(main_measures_O1 PUBLIC OpenMP::OpenMP_C)
# target_compile_options(main_measures_O1 PRIVATE -O1)
# target_link_libraries(main_measures_O1 PRIVATE util counting_sort)

# add_executable(main_measures_seq_O1 source/main_measure.c)
# target_compile_options(main_measures_seq_O1 PRIVATE -O1)
# target_link_libraries(main_measures_seq_O1 PRIVATE util_seq counting_sort_seq)

# add_executable(main_measures_O2 source/main_measure.c)
# target_link_libraries(main_measures_O2 PUBLIC OpenMP::OpenMP_C)
# target_compile_options(main_measures_O2 PRIVATE -O2)
# target_link_libraries(main_measures_O2 PRIVATE util counting_sort)

# add_executable(main_measures_seq_O2 source/main_measure.c)
# target_compile_options(main_measures_seq_O2 PRIVATE -O2)
# target_link_libraries(main_measures_seq_O2 PRIVATE util_seq counting_sort_seq)

# add_executable(main_measures_O3 source/main_measure.c)
# target_link_libraries(main_measures_O3 PUBLIC OpenMP::OpenMP_C)
# target_compile_options(main_measures_O3 PRIVATE -O3)
# target_link_libraries(main_measures_O3 PRIVATE util counting_sort)

# add_executable(main_measures_seq_O3 source/main_measure.c)
# target_compile_options(main_measures_seq_O3 PRIVATE -O3)
# target_link_libraries(main_measures_seq_O3 PRIVATE util_seq counting_sort_seq)

# ---------------------------------------- TEST -----------------------------------------

enable_testing()

add_executable(counting_sort_test test/counting_sort_test.c)
target_link_libraries(counting_sort_test PRIVATE util counting_sort counting_sort_mpi)
target_link_libraries(counting_sort_test PUBLIC MPI::MPI_C)
add_test(NAME counting_sort_test COMMAND counting_sort_test)


# # # ----------------------------- DOCS -------------------------------------
# option(BUILD_DOC "Build documentation" ON)

# # check if Doxygen is installed
# find_package(Doxygen REQUIRED dot)
# if (DOXYGEN_FOUND)
#     # set input and output files
#     set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
#     set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs)

#     # request to configure the file
#     configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
#     message("Doxygen build started")

#     # note the option ALL which allows to build the docs together with the application
#     add_custom_target( doc_doxygen ALL
#         COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
#         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#         COMMENT "Generating API documentation with Doxygen"
#         VERBATIM )
# endif()
