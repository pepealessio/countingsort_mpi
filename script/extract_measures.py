#!/usr/bin/env python3
# -*- coding: utf-8 -*-

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

__author__ = ["Alessio Pepe", "Teresa Tortorella", "Paolo Mansi"]
__copyright__ = "Copyright 2021"
__license__ = "GNU-GPL v3"
__version__ = "1.0.0"
# ------------- Don't edit up here -------------

# --------- You can edit this fields -----------
ARRAY_RC=[1000, 100000, 10000000] #, 10000000]
ARRAY_RANGE=[1000, 10000] # , 100000]
ARRAY_THS=[0, 1, 2, 4, 8]
ARRAY_OPT=[0, 1, 2, 3]
ARRAY_VERSION=[1, 2]
TITLE=''
# ----------------------------------------------

# ------------- Don't edit below here ----------

RAW_FIELDS = ['size', 'range', 'n_th', 't_algo', 'read','minmax','pmf','cdf','split','write'] 


import matplotlib.pyplot as plt
import os
import shutil
import numpy as np


def reject_outliers(data, index=0):
    """This function remove the outside value from a numpy array considering a gaussian distribution. Remove data 
    points that are not in [mean - std; mean + std] (about 68% of the data).
    
    :param data:    an numpy array representing the data points;
    :param index:   the column to evaluate in order to have all the data filtered. If there's just one column leave 0 as default;
    
    :return:        the filtered data."""

    # Compute mean and std. deviation of the data
    u = np.mean(data[:, index])
    s = np.std(data[:, index])

    # Filter samples out of [mean - std; mean + std]
    filtered = [row for row in data if (u - s < float(row[index]) < u + s)]

    # Return the filtered array
    return np.array(filtered, dtype=np.float64)


def compute_speedup(tser, tpar):
    """Compute speedup"""
    return tser / tpar


def compute_efficiency(tser, tpar, nth):
    """Compute the efficiency"""
    return compute_speedup(tser, tpar) / (nth if nth != 0 else 1)


# ---------------- Data processing --------------------------------------------------

# Check if RAW data are present. If not, exit the script.
if not( os.path.exists("measures/raw") ) :
    print(f"Raw measures not present. Generate it with 'bash script/measures.bash'")
    exit(1)

# Create the folder containing all the processed data
if os.path.exists("measures/processed"):
    shutil.rmtree("measures/processed")
os.mkdir("measures/processed")

# Create the folder containing all the plots
if os.path.exists("measures/plots"):
    shutil.rmtree("measures/plots")
os.mkdir("measures/plots")


measure_on_index = 3
for version_i, version in enumerate(ARRAY_VERSION):
    for opt_i, opt in enumerate(ARRAY_OPT):
        for size_i, size in enumerate(ARRAY_RC):
            for range_i, rng in enumerate(ARRAY_RANGE):

                # Print help
                print(f"Elaborating measured for version={version}, opt={opt}, size={size} and range={rng}")

                # Compute refined measure for current size
                measures = np.zeros(shape=(len(ARRAY_THS), len(RAW_FIELDS)+2))

                for nth_i, nth in enumerate(ARRAY_THS):
                    # Now open files and evaluate results
                    m = np.genfromtxt(f"measures/raw/version_{version}/opt_{opt}_size_{size}_range_{rng}_threads_{nth}.csv", delimiter=',', invalid_raise=False)
                    m = reject_outliers(m[1:, :], measure_on_index)
                    m = np.sum(m[:, :], axis=0) / (m.shape[0])
                    
                    measures[nth_i, :-2] = m

                    # Compute speedup and efficiency for the current size
                    measures[nth_i, -2] = compute_speedup(measures[0, measure_on_index], measures[nth_i, measure_on_index])
                    measures[nth_i, -1] = compute_efficiency(measures[0, measure_on_index], measures[nth_i, measure_on_index], nth)

                # Save processed measures
                np.savetxt(f"measures/processed/version_{version}/{RAW_FIELDS[measure_on_index].upper()}_opt_{opt}_size_{size}_range_{rng}.csv", measures, delimiter=',', header=','.join([*RAW_FIELDS, "speedup", "efficiency"]))


# Loop for each (version, size, optimization) tuple
for version_i, version in enumerate(ARRAY_VERSION):
    for opt_i, opt in enumerate(ARRAY_OPT):
        for size_i, size in enumerate(ARRAY_RC):

            # Print help
            print(f"Plotting measured for opt={opt}, size={size}")

            t = ARRAY_THS[1:]

            fig, ax1 = plt.subplots()
            ax2 = ax1.twinx()

            ax1.plot(t, t, '-', label="Ideal Sp.")
            ax2.plot(1,1)

            for range_i, rng in enumerate(ARRAY_RANGE):

                measures = np.genfromtxt(f"measures/processed/version_{version}/{RAW_FIELDS[measure_on_index].upper()}_opt_{opt}_size_{size}_range_{rng}.csv", delimiter=',', invalid_raise=False)
                
                ax1.plot(t, measures[:, -2][1:], '^-', label=f"r. {rng}")
                ax2.plot(t, measures[:, -1][1:], '.--')

            fig.suptitle(f'{RAW_FIELDS[measure_on_index].upper()} ({TITLE}) -  Size:{size} Opt:O{opt}', fontsize=15)
            ax1.set_xlabel("#CPU")
            ax1.set_ylabel("Speedup")

            ax2.set_xlabel("#CPU")
            ax2.set_ylabel("Efficiency", color="green")
            ax1.grid(ls='--')
            ax2.tick_params(axis='y', colors="green")

            ax1.legend(loc='upper center', fontsize=8)
            ax1.label_outer()
            fig.tight_layout()

            plt.savefig(f"measures/plots/version_{version}/{RAW_FIELDS[measure_on_index].upper()}_opt_{opt}_size_{size}.png")
            plt.close()

print("Script ended successfully.")
