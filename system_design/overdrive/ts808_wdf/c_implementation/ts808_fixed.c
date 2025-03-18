#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define TYPE float
#define CAP_R(C, Fs) (1/(2*C*Fs))
#define Fs (44100)
#define ACC_MASK (0xffffffffff) // 40-bit accumulator mask
#define ACC(x) (x&ACC_MASK)
#define DATADIR "traces/"

#include "omega.h"
#include "diode.h"

// Save n_samples samples from the array to a binary file.
void save_samples_int64(char *filename, int64_t *samples, size_t n_samples) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("Failed to open file for writing");
        return;
    }
    // Write the raw data: n_samples elements of type TYPE.
    if (fwrite(samples, sizeof(int64_t), n_samples, f) != n_samples) {
        perror("Failed to write all samples");
    }
    fclose(f);
}

// Save n_samples samples from the array to a binary file.
void save_samples_int16(char *filename, int16_t *samples, size_t n_samples) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("Failed to open file for writing");
        return;
    }
    // Write the raw data: n_samples elements of type TYPE.
    if (fwrite(samples, sizeof(int16_t), n_samples, f) != n_samples) {
        perror("Failed to write all samples");
    }
    fclose(f);
}

// Save n_samples samples from the array to a binary file.
void save_time_samples(char *filename, double *samples, size_t n_samples) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("Failed to open file for writing");
        return;
    }
    // Write the raw data: n_samples elements of type TYPE.
    if (fwrite(samples, sizeof(double), n_samples, f) != n_samples) {
        perror("Failed to write all samples");
    }
    fclose(f);
}

#define ERR_OPEN_FILE -1
// Reads a raw PCM file (pcm_s16le mono) into an array of floats in the range [-1, 1].
// filename: input raw PCM file name.
// array: destination array to store up to N float samples.
// N: maximum number of float samples to read.
// Returns 0 on success, or a negative error code if something fails.
int read_pcm(char *filename, int16_t *array, size_t N) {
    FILE *f = fopen(filename, "rb");
    if (!f)
        return ERR_OPEN_FILE;

    size_t count = 0;
    unsigned char sample_bytes[2];

    // Each sample is 2 bytes (16-bit)
    while (count < N)
    {
        size_t nread = fread(sample_bytes, 1, 2, f);
        if (nread < 2)
            break; // End-of-file reached

        // Reconstruct a signed 16-bit integer from little-endian bytes.
        int16_t sample = (int16_t)(sample_bytes[0] | (sample_bytes[1] << 8));

        // Map to float in range [-1, 1]. Note that -32768 maps to -1.
        array[count++] = sample;
    }

    fclose(f);
    return 0;
}

// Writes a raw PCM file (pcm_s16le mono) from an array of floats in the range [-1, 1].
// filename: output file name.
// array: source array of float samples.
// N: number of samples in the array.
// Returns 0 on success, or a negative error code if something fails.
int write_pcm(char *filename, int16_t *array, size_t N) {
    FILE *f = fopen(filename, "wb");
    if (!f)
        return ERR_OPEN_FILE;

    for (size_t i = 0; i < N; i++)
    {
        int16_t sample = array[i];
        // Write sample in little-endian order.
        unsigned char bytes[2];
        bytes[0] = sample & 0xFF;
        bytes[1] = (sample >> 8) & 0xFF;
        if (fwrite(bytes, 1, 2, f) != 2)
        {
            fclose(f);
            return -2; // error writing file
        }
    }

    fclose(f);
    return 0;
}

int signum (TYPE val) {
    return (0 < val) - (val < 0);
}

/// Processing
#define C_2_val (CAP_R(1e-6, Fs))
#define R_A_val (220)
#define V_bias_res_val (10e3)
// processes a sample x in part 1 circuit and outputs the voltage
int64_t part1_process(int16_t x) {
    static const int16_t inp_inner_ser_reflect = 32768*(V_bias_res_val/(R_A_val+V_bias_res_val));
    static const int16_t inp_ser_reflect = 32768*(C_2_val/(R_A_val+V_bias_res_val+C_2_val));
    return -x;
}

#define R_4_val (4.7e3)
#define C_3_val (CAP_R(0.047e-6, Fs))
// processes a sample x in part 2 circuit and outputs the current
int64_t part2_process(int16_t y1) {
    static const int16_t r = 32768*(C_3_val)/(C_3_val+R_4_val);
    int64_t ac0, ac1, ac2, ac3;
    static int16_t C3_a=0, C3_b;
    C3_b = C3_a;
    ac0 = y1*2; // q1.15
    ac1 = C3_b; // q0.15
    ac0 += ac1; // q2.15: a
    ac2 = ac0;  // store a in ac2
    ac0 += ac1; // q3.15
    ac0 *= -r;  // q0.30 since r is small
    ac1 += ac0>>15; // q15: C3_a
    C3_a = ac1;
    ac2 += ac1; // q3.15: -y2
    ac2 = - ac2; // q3.15: y2
    return ac2;
}

#define C_4_val (CAP_R(51e-12, Fs))
#define I_f_res_val (51e3)
#define V_T (26e-3)
#define I_S (1e-12)
double fp_diode(double a, int16_t drive_res) {
    TYPE par_g = (1/C_4_val + 1/(I_f_res_val+drive_res*1000.0));
    TYPE par_r = 1/par_g;
    TYPE R_Is = par_r * I_S;
    TYPE R_Is_over_Vt = R_Is / V_T;
    TYPE log_part = log(R_Is_over_Vt);
    int lambda = signum(a);
    TYPE other_part = (lambda * a)/V_T + R_Is_over_Vt;
    TYPE wright = omega4(log_part + other_part);
    TYPE b = a + 2 * lambda * (R_Is - V_T * wright);
    return b;
}

// circuit is parallel(I_f, C_4)
// processes a sample (current) in part 3 circuit and outputs the voltage
int64_t part3_process(int64_t x) {
    // FIXME THIS IS NOT IMPLEMENTED YET AND IS WRONG
    int16_t drive_resist = 100; // q9.0: 0-500 (k)
    static int32_t C4_b = 0; // q14.15: expensive
    int64_t ac0, ac1, ac2, ac3;
    ac3 = C4_b; // q14.15: load delayed sample
    ac0 = x;
    ac0 *= -11; // -If_static_scale = -5.4255 (in q3.1 --> -11)
    ac1 = drive_resist;
    ac2 = ac1 * 3486; // q6.15
    ac2 = ac2 >> 11; // q6.4
    ac0 *= ac2; // q12.20: -If_b
    ac0 = ac0 >> 5; // q12.15
    ac0 += ac3; // q12.15
    // calc reflect based on drive
    // TODO implement LUT
    double fuck = (double)(ac1+51)*1000;
    double lut_replacement = -(1/((fuck)*(4.498200e-06 + 1/fuck)));
    ac1 = (int64_t)((1<<8)*lut_replacement); // q8: If_static_res
    // weitergehen
    ac0 *= ac1; // q13.23
    ac0 = ac0 >> 8; // q13.15
    ac3 += ac0; // q15.15: a
    // model diode
    /////// FIXME implement LUT
    double a = (double)ac3/(1<<15);
    double b = fp_diode(a, drive_resist);
    ///////
    ac2 = b*(1<<15); // q8.15
    ac0 += ac2; // q14.15: C4_a
    C4_b = ac0; // save for later
    ac2 += ac3;
    ac2 /= 2;
    return ac2;
}

#define clock_to_sec(t) (((double)t)/CLOCKS_PER_SEC)
int main() {
// #define FUNCTION_GENERATOR
// #define DEBUG_PRINT
#ifndef FUNCTION_GENERATOR
    const int N = 44100*10;
#else
    const int N = 100;
#endif
    int16_t *input = calloc(sizeof(int16_t), N);
    int64_t *v_plus = calloc(sizeof(int64_t), N);
    int64_t *i_f = calloc(sizeof(int64_t), N);
    int64_t *cooktown = calloc(sizeof(int64_t), N);
    int16_t *output = calloc(sizeof(int16_t), N);
    double *part1_time = calloc(sizeof(double), N);
    double *part2_time = calloc(sizeof(double), N);
    double *part3_time = calloc(sizeof(double), N);
#ifndef FUNCTION_GENERATOR
    int err = read_pcm("../guitar.pcm", input, N);
    if (err != 0) {
        perror("file open failed");
        exit(ERR_OPEN_FILE);
    }
#endif
    // initialization
    // for (int i=0; i<100; i++) {
    //     part3_process(part2_process(part1_process(0)));
    // }
    // process loop
    clock_t t_start = clock();
    for (int t=0; t<N; t++) {
#ifdef FUNCTION_GENERATOR
        int16_t x = 32768*sinf(2*3.14f*1000*t/Fs); // Q15
#else
        int16_t x = input[t];
#endif
        clock_t t1 = clock();
        int64_t y1 = part1_process(x);
        clock_t t2 = clock();
        int64_t y2 = part2_process(y1);
        clock_t t3 = clock();
        int64_t y3 = part3_process(y2);
        clock_t t4 = clock();
#ifdef DEBUG_PRINT
        printf("x=%hd, y1=%ld, y2=%ld, y3=%ld\n", x, y1, y2, y3);
#endif
        // Save samples
#ifdef FUNCTION_GENERATOR
        input[t] = x;
#endif
        v_plus[t] = y1;
        i_f[t] = y2;
        cooktown[t] = y3;
        output[t] = (y1 + y3)>>1; // q15 filter with gain 0.5
        part1_time[t] = clock_to_sec(t2-t1);
        part2_time[t] = clock_to_sec(t3-t2);
        part3_time[t] = clock_to_sec(t4-t3);
    }
    clock_t t_stop = clock();
    double t_total = clock_to_sec(t_stop-t_start);
    printf("Calculated %i seconds (%i samples) in %lf seconds (%lf seconds per second of audio)\n", N/Fs, N, t_total, (t_total/N)*Fs);
    save_samples_int16(DATADIR"input.bin", input, N);
    save_samples_int64(DATADIR"v_plus.bin", v_plus, N);
    save_samples_int64(DATADIR"i_f.bin", i_f, N);
    save_samples_int64(DATADIR"cooktown.bin", cooktown, N);
    save_samples_int16(DATADIR"output.bin", output, N);
    save_time_samples(DATADIR"part1_time.bin", part1_time, N);
    save_time_samples(DATADIR"part2_time.bin", part2_time, N);
    save_time_samples(DATADIR"part3_time.bin", part3_time, N);
    write_pcm("../output_fixed.pcm", output, N);
}
