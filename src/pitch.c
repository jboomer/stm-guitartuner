#include "stm32f4xx.h"
#include "pitch.h"
#include "arm_math.h"

#define SAMPLING_FREQUENCY 2e3
#define BUFFERSIZE         512
#define FFT_BIN_WIDTH      (SAMPLING_FREQUENCY / BUFFERSIZE)

typedef enum {INIT, RISING, FOUND} PeakDetectState;

bool pitch_autocorr_arm_math(uint16_t* buffer
                           , uint32_t bufferLen
                           , float* freq)

{
    q15_t outputbuffer[2 * bufferLen - 1];
    uint32_t maxIndex = 0;
    PeakDetectState state = INIT;

    arm_correlate_q15((q15_t*)buffer
                     , bufferLen
                     , (q15_t*)buffer
                     , bufferLen
                     , outputbuffer);

    /* State machine, finds first peak, starting at lag 0 */
    for (uint32_t i = bufferLen; i < 2 * bufferLen - 2; i++) {
        if (outputbuffer[i+1] > outputbuffer[i]) {
            state = RISING;
            continue;
        }

        if (state == RISING && outputbuffer[i+1] < outputbuffer[i]) {
            state = FOUND;
            maxIndex = i;
            break;
        }
    }

    if (state == FOUND){
        *freq = SAMPLING_FREQUENCY / ((float)(maxIndex - bufferLen));
        return true;
    } else {
        /* Did not find a peak, return error */
        return false;
    }
}

bool pitch_fft(uint16_t* buffer, uint32_t bufferlen, float* freq)
{
    arm_rfft_instance_q15 initfft;
    if (arm_rfft_init_q15(&initfft, bufferlen / 2, 0, 1) != ARM_MATH_SUCCESS) {
        return false;
    }

    q15_t outbuffer[BUFFERSIZE] = {0};
    q15_t magnitude[BUFFERSIZE/2] = {0};
    q15_t maxValue = 0;
    uint32_t maxIndex = 0;

    /* Perform FFT */
    arm_rfft_q15(&initfft, (q15_t*)buffer, outbuffer);

    /* Since bin 0 and bin N/2 is always real in rfft
     * outputbuffer 0, 1 is real[0], real[N/2], then
     * real[1], complex[1], real[2], complex[2] etc. */

    /* Find complex magnitude */
    arm_cmplx_mag_q15(outbuffer, magnitude, BUFFERSIZE / 2);

    /* Find the max (in the first half, positive frequencies)*/
    arm_max_q15(&magnitude[1], BUFFERSIZE / 2 - 1, &maxValue, &maxIndex);

    /* Return the frequency */
    *freq = (float)(maxIndex + 1) * FFT_BIN_WIDTH;

    return true;
}
