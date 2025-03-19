#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define SAMPLE_RATE 44100     // Sampling frequency (Hz)
#define FLANGER_RATE 1     // LFO frequency (Hz)
#define MIN_DELAY 5           // Minimum delay in samples
#define MAX_DELAY 100          // Maximum delay in samples
#define DELAY_BUFFER_SIZE 1024 // Circular buffer size
#define Q15_SCALE (1 << 15)   // Q15 scaling factor

int16_t delayBuffer[DELAY_BUFFER_SIZE]; // Circular delay buffer
int writeIndex = 0;  // Write index for circular buffer

// CORDIC Sine Function (Fixed Point)
void cordic_fixed_point(int16_t theta, int16_t *cos_val, int16_t *sin_val) {
    int16_t x = 0x26DD; // Q15 scaling factor (≈ 0.607252)
    int16_t y = 0;
    int16_t z = theta;

    static const int16_t atan_table[16] = {
        0x3243, 0x1DAC, 0x0FAD, 0x07F5, 0x03FE, 0x01FF, 0x00FF, 0x007F,
        0x003F, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000
    };

    for (int i = 0; i < 16; i++) {
        int16_t x_new;
        if (z >= 0) {
            x_new = x - (y >> i);
            y = y + (x >> i);
            z -= atan_table[i];
        } else {
            x_new = x + (y >> i);
            y = y - (x >> i);
            z += atan_table[i];
        }
        x = x_new;
    }

    *cos_val = x;
    *sin_val = y;
}

// Convert floating-point radians to Q15 fixed-point
int16_t rad_to_q15(float radians) {
    return (int16_t)(radians * Q15_SCALE);
}

// Flanger effect function
int16_t flanger_process(int16_t input, uint32_t *phase_acc) {
    static uint32_t phase_step = (uint32_t)(2.0 * M_PI * FLANGER_RATE * Q15_SCALE / SAMPLE_RATE);
    
    // Generate sine wave for LFO
    int16_t cos_val, lfo;
    cordic_fixed_point((*phase_acc) >> 16, &cos_val, &lfo);
    
    // Increment phase accumulator
    *phase_acc += phase_step;
    if (*phase_acc > (uint32_t)(2 * M_PI * Q15_SCALE)) {
        *phase_acc -= (uint32_t)(2 * M_PI * Q15_SCALE);
    }

    // Convert LFO output to delay range
    int delay_samples = MIN_DELAY + ((lfo + Q15_SCALE) * (MAX_DELAY - MIN_DELAY) / (2 * Q15_SCALE));

    // Compute read index for delay buffer
    int readIndex = writeIndex - delay_samples;
    if (readIndex < 0) readIndex += DELAY_BUFFER_SIZE;

    // Get delayed sample
    int16_t delayedSample = delayBuffer[readIndex];

    // Store current sample in delay buffer
    delayBuffer[writeIndex] = input;
    writeIndex = (writeIndex + 1) % DELAY_BUFFER_SIZE;

    // Mix dry and wet signals (50% mix)
    return (input + delayedSample) / 2;
}

// Main function for testing
int main() {
    int16_t inputSignal[SAMPLE_RATE]; // Example input buffer (1 second)
    int16_t outputSignal[SAMPLE_RATE]; // Processed output buffer

    uint32_t phase_acc = 0; // LFO phase accumulator

    // Example test: Fill input with a 440Hz sine wave
    for (int i = 0; i < SAMPLE_RATE; i++) {
        inputSignal[i] = (int16_t)(32767 * sin(2.0 * M_PI * 440 * i / SAMPLE_RATE));
    }

    // Process flanger effect
    for (int i = 0; i < SAMPLE_RATE; i++) {
        outputSignal[i] = flanger_process(inputSignal[i], &phase_acc);
        printf("%d, %d\n", i, outputSignal[i]); // Print values for plotting
    }

    return 0;
}
