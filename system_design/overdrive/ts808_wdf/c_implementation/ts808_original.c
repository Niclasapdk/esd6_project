#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TYPE float
#define CAP_R(C, Fs) (1/(2*C*Fs))
#define Fs (44100)

#include "omega.h" // must be after definition of `TYPE`

// Save n_samples samples from the array to a binary file.
void save_samples(char *filename, TYPE *samples, size_t n_samples) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("Failed to open file for writing");
        return;
    }
    // Write the raw data: n_samples elements of type TYPE.
    if (fwrite(samples, sizeof(TYPE), n_samples, f) != n_samples) {
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
int read_pcm(char *filename, float *array, size_t N) {
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
        array[count++] = sample / 32768.0f;
    }

    fclose(f);
    return 0;
}

// Writes a raw PCM file (pcm_s16le mono) from an array of floats in the range [-1, 1].
// filename: output file name.
// array: source array of float samples.
// N: number of samples in the array.
// Returns 0 on success, or a negative error code if something fails.
int write_pcm(char *filename, float *array, size_t N) {
    FILE *f = fopen(filename, "wb");
    if (!f)
        return ERR_OPEN_FILE;

    for (size_t i = 0; i < N; i++)
    {
        float s = array[i];
        // Clamp to avoid overflow: ensure that 1.0 doesn't map to 32768.
        if (s > 0.999969f)
            s = 0.999969f;
        if (s < -1.0f)
            s = -1.0f;
        int16_t sample = (int16_t)(s * 32768.0f);
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
TYPE part1_process(TYPE x) {
    static const TYPE inp_inner_ser_reflect = V_bias_res_val/(R_A_val+V_bias_res_val);
    static const TYPE inp_ser_reflect = C_2_val/(R_A_val+V_bias_res_val+C_2_val);
    static const TYPE v_bias_b = 0; // FIXME why the fuck does this work with 0 when it should be 4.5??????????????
    // static TYPE C_2_a = 4.5, C_2_b = 4.5;
    static TYPE C_2_a = 0, C_2_b = 0; // FIXME same here wtf is going on
    C_2_b = C_2_a;
    C_2_a = C_2_b - inp_ser_reflect * (x + C_2_b - v_bias_b); // port1.b - reflect * (x + port1.b + port2.b) // port2.b is from the inner series circuit
    TYPE inner_ser_a = - (x + C_2_a); // b2 = - (x + b1)
    TYPE R_A_a = - inp_inner_ser_reflect * (inner_ser_a + 4.5); // port1.b - reflect * (x + port1.b + port2.b) // ser(R_A, V_bias)
    TYPE v_bias_a = - (x + R_A_a); // b2 = - (x + b1)
    return (v_bias_a + 4.5)/2; // voltage
}

#define R_4_val (4.7e3)
#define C_3_val (CAP_R(0.047e-6, Fs))
// processes a sample x in part 2 circuit and outputs the current
TYPE part2_process(TYPE x) {
    static const TYPE reflect = (C_3_val)/(C_3_val+R_4_val);
    static TYPE C_3_a = 0, C_3_b = 0;
    C_3_b = C_3_a;
    TYPE a = 2*x + C_3_b; // 2*x - reflected(series_connection)
    C_3_a = C_3_b - reflect * (a + C_3_b);
    TYPE b2 = - (C_3_a + a);
    return b2 / (2*R_4_val);
}

#define gain ((TYPE)2) // 0-10
#define C_4_val (CAP_R(51e-12, Fs))
#define I_f_res_val (51e3+(gain/10)*500e3)
#define V_T (26e-3)
#define I_S (1e-12)
// circuit is parallel(I_f, C_4)
// processes a sample (current) in part 3 circuit and outputs the voltage
TYPE part3_process(TYPE x) {
    static const TYPE par_g = (1/C_4_val + 1/I_f_res_val);
    static const TYPE par_r = 1/par_g;
    static const TYPE R_Is = par_r * I_S;
    static const TYPE R_Is_over_Vt = R_Is / V_T;
    static const TYPE log_R_Is_over_Vt = -12.222595194170855;
    static const TYPE reflect = (1/I_f_res_val)/par_g;
    static TYPE C_4_a = 0, C_4_b = 0;
    // get reflected waves for C_4 and I_f and parallel circuit
    TYPE I_f_b = x * I_f_res_val;
    C_4_b = C_4_a;
    TYPE bDiff = C_4_b - I_f_b;
    TYPE a = C_4_b - reflect * bDiff; // reflected from circuit (incident to diode)

    // Model diode
// #define DIODE_BETTER
#ifdef DIODE_BETTER
    int lambda = signum(a);
    TYPE other_part = (lambda * a)/V_T + R_Is_over_Vt;
    TYPE lambda_a_over_vt = lambda * a / V_T;
    TYPE b = a - 2 * V_T * lambda * (omega4(log_R_Is_over_Vt + lambda_a_over_vt) - omega4(log_R_Is_over_Vt - lambda_a_over_vt));
#else
    int lambda = signum(a);
    TYPE other_part = (lambda * a)/V_T + R_Is_over_Vt;
    TYPE wright = omega4(log_R_Is_over_Vt + other_part);
    TYPE b = a + 2 * lambda * (R_Is - V_T * wright);
#endif

    // Scatter incident to parallel circuit
    TYPE b2 = a - C_4_b + b; // reflected from circuit - port2->b + x
    C_4_a = b2;
    return (b+a)/2;
}

#define clock_to_sec(t) (((double)t)/CLOCKS_PER_SEC)
int main() {
    int N = 44100*10;
// #define FUNCTION_GENERATOR
// #define DEBUG_PRINT
    TYPE *input = calloc(sizeof(TYPE), N);
    TYPE *v_plus = calloc(sizeof(TYPE), N);
    TYPE *i_f = calloc(sizeof(TYPE), N);
    TYPE *output = calloc(sizeof(TYPE), N);
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
        TYPE x = sinf(2*3.14*100*t); // 100 Hz
#else
        TYPE x = input[t];
#endif
        clock_t t1 = clock();
        TYPE y1 = part1_process(x);
        clock_t t2 = clock();
        TYPE y2 = part2_process(y1);
        clock_t t3 = clock();
        TYPE y3 = part3_process(y2);
        clock_t t4 = clock();
#ifdef DEBUG_PRINT
        printf("x=%f, y=%f, out=%f\n", x, y, y-4.5);
#endif
        // Save samples
#ifdef FUNCTION_GENERATOR
        input[t] = x;
#endif
        v_plus[t] = y1;
        i_f[t] = y2;
        output[t] = y1 + y3 - 4.5;
        part1_time[t] = clock_to_sec(t2-t1);
        part2_time[t] = clock_to_sec(t3-t2);
        part3_time[t] = clock_to_sec(t4-t3);
    }
    clock_t t_stop = clock();
    double t_total = clock_to_sec(t_stop-t_start);
    printf("Calculated %i seconds (%i samples) in %lf seconds (%lf seconds per second of audio)\n", N/Fs, N, t_total, (t_total/N)*Fs);
    save_samples("v_plus.bin", v_plus, N);
    save_samples("input.bin", input, N);
    save_samples("i_f.bin", i_f, N);
    save_samples("output.bin", output, N);
    save_time_samples("part1_time.bin", part1_time, N);
    save_time_samples("part2_time.bin", part2_time, N);
    save_time_samples("part3_time.bin", part3_time, N);
    write_pcm("../output.pcm", output, N);
}
