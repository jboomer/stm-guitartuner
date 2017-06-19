#! /usr/bin/env python3

from functools import partial
import numpy as np
import argparse
import matplotlib.pyplot as plt

ADC_DIV = 3.3 / 4095

def main():

    parser = argparse.ArgumentParser()
    parser.add_argument('FILENAME')
    args = parser.parse_args()

    plot_binary_file(args.FILENAME)

def plot_binary_file(filename):
    with open(filename, 'rb') as f:
        values = [int.from_bytes(bVal, byteorder='little') * ADC_DIV
            for bVal in iter(partial(f.read, 2), b'') ]        

    t = (np.arange(len(values)) / 10e3)
    plt.plot(t, values)
    plt.show()

if __name__ == '__main__':
    main()
