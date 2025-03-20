#include <math.h>

#define ADC_MAX_VALUE 65536  // Example: 16-bit ADC (0-65536)
//#define Pot_Port 1           // Port reading the pot
//#define Fs 44100             // Sampling frequency
//#define portInput 3          // input port for voltage

double readADC(int port) {
    return analogRead(port);  // Example for Arduino
}

double CalcF1(int port, double minf,double maxf, double Fs) {
    double ADC_Value = readADC(port);                                   // read ADC
    double Fc = (ADC_Value * (maxf-minf)+minf) / (double)ADC_MAX_VALUE; // calc FC
    // calculate tunning coefficient based on Fc
    double F1 = 2 * sin((3.1415926535897931 * Fc)/Fs);     
    return F1;
}   

/* The WahWah effect, which should change Fc based on the pedal adjustments*/
double WahWahBahBah(double damp, double minf, double maxf, double Fs, double x, int Pot_Port, int portInput)
{
    // Set first the coeffiecient equal 0 first element
    static double yl = 0;                          // Set first low pass coefficient
    static double yaux = 0;                        // Set first output coefficient
    // tuning parameters being set 
    double Q1 = damp * 2;                          // set Q factor based on damp factor
    double F1 = CalcF1(Pot_Port,minf,maxf,Fs);     // set F1 factor basd on Fc
    // Maybe make pause between each sample
    // check input signal
    double x = readADC(portInput); 
    // Difference equation
    double yh = x - yl - Q1 * yaux;                // calculate the higher pass filter, should be x in first iteration
    double y = F1 * yh + yaux;                     // calculate output, should be F1 * x in first iteration
    yl = F1 * y + yl;                              // lowpass filter, should be F1 * F1* x in first iteration
    yaux = y;                                      // Saves last output
    return yaux;                                   // output
    }