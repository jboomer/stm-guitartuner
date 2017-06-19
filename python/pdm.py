#! /usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt


def main():
    # Create a sine wave, of 100Hz, sampled at 16kHz, 1 second
    x = np.arange(1600)
    t = x / 16e3
    y = np.sin(2 * np.pi * 100 * t)

    # Convert each to a 64bit number
    yadc = ((y + 1) ) * (2**63 - 1)
    yadc.round
    yadc = np.uint64(yadc)
    pdm = np.unpackbits(yadc.view(np.uint8))


    plt.plot(pdm)
    plt.xlabel('sample')
    plt.ylabel('Amplitude')
    plt.show()


if __name__ == '__main__':
    main()
