# About

This is an effort to make a guitar tuner using the STM32F4 Discovery board.

# Theory

Guitars in standard tuning have the following frequencies for the strings:

| String  | Freq (Hz)   |
|---------|-------------|
| E       | 82.41       |
| A       | 110.00      |
| D       | 146.80      |
| G       | 196.00      |
| B       | 246.90      |
| e       | 329.60      |

So to tune, we have to select a string, compare an input signals frequency it to this frequency and output either "higher" or "lower".

There are several techniques to get the frequency of a sampled signal. Some of the simpler ones are explained below.

## Zero crossing

## Fourier Transform

## Autocorrelation

# Hardware
To sample the guitar signal, which usually is a signal of +/- a few mV, we have to bring it into the ADC range of 0-3.3V.

To do this the following circuit is used:

![Circuit](hw/circuit.png?raw=true "Circuit"){ width=50% }

# Code

# Results

# TODO

- [ ] Faster autocorrelation
- [ ] Try zero crossing counting
- [ ] Using microphone input
- [ ] Output on LCD screen
- [ ] Port to HAL/CMSIS/ChibiOSHAL (std. peripheral libs deprecated)
