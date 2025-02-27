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

inline int signum (TYPE val) {
    return (0 < val) - (val < 0);
}

typedef struct  {
    TYPE R; // port resistance
    TYPE G; // port conductance (1/R)
    TYPE a; // incident
    TYPE b; // reflected
} OnePort_t;

typedef struct {
    TYPE R;
    TYPE G;
    TYPE port1Reflect;
    TYPE bDiff; // used for parallel
    TYPE a; // incident
    TYPE b; // reflected
    OnePort_t *p1, *p2;
} Adapter_t;

inline TYPE voltage(OnePort_t *op) {
    return (op->a+op->b)/2;
}

inline TYPE current(OnePort_t *op) {
    return (op->a - op->b)/(2*op->R);
}

void initParallel(Adapter_t *par, OnePort_t *p1, OnePort_t *p2) {
    par->R = p1->R*p1->R/(p1->R+p2->R);
    par->G = 1/par->R;
    par->port1Reflect = p1->G/par->G;
    par->p1 = p1;
    par->p2 = p2;
}

void initSeries(Adapter_t *ser, OnePort_t *p1, OnePort_t *p2) {
    ser->R = p1->R + p2->R;
    ser->G = 1/ser->R;
    ser->port1Reflect = p1->R/ser->R;
    ser->p1 = p1;
    ser->p2 = p2;
}

void initOnePort(OnePort_t *op, TYPE R, TYPE a, TYPE b) {
    op->R = R;
    op->G = 1/R;
    op->a = a;
    op->b = b;
}

TYPE reflectedParallel(Adapter_t *par) {
    par->bDiff = par->p2->b - par->p1->b;
    TYPE b = par->p2->b - par->port1Reflect * par->bDiff;
    return b;
}

inline TYPE reflectedSerial(Adapter_t *ser) {
    return -(ser->p1->b + ser->p2->b);
}

void incidentParallel(Adapter_t *par, TYPE x) {
    TYPE b2 = par->b - par->p2->b + x;
    par->p2->a = b2;
    par->p1->a = b2 + par->bDiff;
    par->a = x;
}

// void incidentSeries(Adapter_t *ser, TYPE x) {
//     TYPE b1 = ser->p1->b - ser->port1Reflect * (x + ser->p1->b + ser->p2->b);
//     ser->p1->a = b1;
//     ser->p2->a = - (x + b1);
//     ser->a = x;
// }

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

#define gain ((TYPE)5) // 0-10
#define C_4_val (CAP_R(51e-12, Fs))
#define I_f_res_val (51e3+(gain/10)*500e3)
#define V_T (26e-3)
#define I_S (1e-12)
// circuit is parallel(C_4, I_f)
// processes a sample (current) in part 3 circuit and outputs the voltage
TYPE part3_process(TYPE x) {
    static const TYPE par_g = (1/C_4_val + 1/I_f_res_val);
    static const TYPE par_r = 1/par_g;
    static const TYPE R_Is = par_r * I_S;
    static const TYPE R_Is_over_Vt = R_Is / V_T;
    static const TYPE reflect = (1/C_4_val)/par_g;
    static TYPE C_4_a = 0, C_4_b = 0;
    // get reflected waves for C_4 and I_f and parallel circuit
    TYPE I_f_b = x * I_f_res_val;
    C_4_b = C_4_a;
    TYPE bDiff = C_4_b - I_f_b;
    TYPE b = C_4_b - reflect * bDiff;

    // Model diode
    int lambda = signum(b);
    TYPE log_part = log_approx(R_Is_over_Vt);
    TYPE other_part = (lambda * b)/V_T + R_Is_over_Vt;
    TYPE wright = omega4(log_part + other_part);
    TYPE a = b + 2 * lambda * (R_Is - V_T * wright);
    TYPE b2 = a - I_f_b + a; // b - port2->b + x
    C_4_a = b2 + bDiff;
    return (a+b)/2;
}

#define clock_to_sec(t) (((double)t)/CLOCKS_PER_SEC)
int main() {
    int N = 44100*10;
#define FUNCTION_GENERATOR
// #define DEBUG_PRINT
    TYPE *input = calloc(sizeof(TYPE), N);
    TYPE *v_plus = calloc(sizeof(TYPE), N);
    TYPE *i_f = calloc(sizeof(TYPE), N);
    TYPE *output = calloc(sizeof(TYPE), N);
    double *part1_time = calloc(sizeof(double), N);
    double *part2_time = calloc(sizeof(double), N);
    double *part3_time = calloc(sizeof(double), N);
#ifndef FUNCTION_GENERATOR
    // read_wav("../guitar.wav", input, sizeof(input), 0);
#endif
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
    // write_wav("../output.wav", output, sizeof(output));
}
