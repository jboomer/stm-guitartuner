#! /usr/bin/env python3

from functools import partial
import numpy as np
import argparse
import matplotlib.pyplot as plt
from scipy.fftpack import fft, fftshift
from scipy.signal import correlate


# Constants
ADC_DIV = 3.3 / 4095
FS = 2e3

def main():

    parser = argparse.ArgumentParser()
    parser.add_argument('FILENAME')
    args = parser.parse_args()

    plot_time_series(read_time_series(args.FILENAME))

def read_time_series(filename, nbytes=2, byteorder='little'):
    with open(filename, 'rb') as f:
        y = [int.from_bytes(bVal, byteorder=byteorder) * ADC_DIV
            for bVal in iter(partial(f.read, nbytes), b'') ]        

        return y

def plot_time_series(y):
    n = len(y)

    print('Read {0} values'.format(n))

    k = np.arange(n)

    T = n / FS

    frq = k / T
    frq = frq[:n//2]

    t = np.arange(n) / FS

    plt.figure(1)
    plt.title('Sampled signal')
    plt.xlabel('time [sec]')
    plt.ylabel('V')
    plt.plot(t, y)

    # Remove DC component
    y = y - np.mean(y)
    
    f = fft(y)
    f = f[:n//2]

    print('Max fft freq {0} Hz'.format(frq[np.argmax(abs(f))]))

    plt.figure(2)
    plt.title('Fourier transform')
    plt.xlabel('f [Hz]')
    plt.ylabel('Magnitude')
    plt.plot(frq, abs(f))

    corr = correlate(y, y)
    corrpos = corr[n:]

    # Find first positive difference
    risePoint = 0
    for index in range(len(corrpos) - 1):
        if corrpos[index + 1] - corrpos[index] > 0:
            risePoint = index
            break

    # Find highest peak after rise point
    period = risePoint + np.argmax(corrpos[risePoint:])
    print('Highest autocorr peak {0} Hz'.format(FS / period))


    plt.figure(3)
    plt.title('Autocorrelation')
    plt.xlabel('lag')
    plt.ylabel('coeff')
    plt.plot(corrpos)

    plt.show()

if __name__ == '__main__':
    main()
