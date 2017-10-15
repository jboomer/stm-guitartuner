#include <stdint.h>
#include <stdbool.h>

/**
 * Use autocorrelation to detect the pitch
 *
 * @param buffer ADC input buffer
 * @param bufferLen Length of the input buffer
 * @param freq Return of the fundamental frequency
 *
 * @return True for success, false for failure
 */
bool pitch_autocorr_arm_math(uint16_t* buffer, uint32_t bufferLen, float* freq);

/**
 * Use the FFT to detect the pitch
 *
 * @param buffer ADC input buffer
 * @param bufferLen Length of the input buffer
 * @param freq Return of the fundamental frequency
 *
 * @return True for success, false for failure
 */

bool pitch_fft(uint16_t* buffer, uint32_t bufferlen, float* freq);
