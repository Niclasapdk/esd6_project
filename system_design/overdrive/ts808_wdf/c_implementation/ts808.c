#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define TYPE float
#define CAP_R(C, Fs) (1/(2*C*Fs))

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
OnePort_t C_2; // = C(1e-6,Fs);
OnePort_t R_A; // = R(220);
OnePort_t V_bias; // V(4.5, 10e3);
TYPE inp_ser_reflect;
TYPE inp_inner_ser_reflect;

void part1_init() {
    // initOnePort(&C_2, CAP_R(1e-6, 44.1e3), 4.5, 4.5); // init to 4.5V
    initOnePort(&C_2, CAP_R(1e-6, 44.1e3), 0, 0);
    initOnePort(&R_A, 220, 0, 0);
    initOnePort(&V_bias, 10e3, 0, 0);
    TYPE R_inner = R_A.R+V_bias.R;
    inp_inner_ser_reflect = V_bias.R/R_inner;
    TYPE R = R_inner+C_2.R;
    inp_ser_reflect = C_2.R/R;
}

// processes a sample x in part 1 circuit and outputs the voltage
TYPE part1_process(TYPE x) {
    C_2.b = C_2.a;
    TYPE b1 = C_2.b - inp_ser_reflect * (x + C_2.b + V_bias.b);
    C_2.a = b1;
    TYPE inner_ser_a = - (x + b1);
    TYPE R_A_a = - inp_inner_ser_reflect * (inner_ser_a + 4.5);
    TYPE v_bias_a = - (x + R_A_a);
    return (v_bias_a + 4.5)/2; // voltage
}

int main() {
    // int N = 44100*10;
// #define DEBUG_PRINT
    int N = 20;
    TYPE *input = calloc(sizeof(TYPE), N);
    TYPE *v_plus = calloc(sizeof(TYPE), N);
    part1_init();
    for (int t=0; t<N; t++) {
        TYPE x = sinf(2*3.14*100*t); // 100 Hz
        TYPE y = part1_process(x);
#ifdef DEBUG_PRINT
        printf("x=%f, y=%f, out=%f\n", x, y, y-4.5);
#endif
        // Save samples
        input[t] = x;
        v_plus[t] = y;
    }
    save_samples("v_plus.bin", v_plus, N);
    save_samples("input.bin", input, N);
}
