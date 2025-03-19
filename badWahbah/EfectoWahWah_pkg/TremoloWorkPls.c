#include <math.h>

#define ADC_MAX_VALUE 65536  // Example: 16-bit ADC (0-65536)
//#define Pot_Port 1           // Port reading the pot
//#define Fs 44100             // Sampling frequency
//#define portInput 3          // input port for voltage
//#define minf=0.1             // minumum hz for the lfo
//#define maxf=20              // maximum hz for the lfo
//#define minDepth=0.01        // minumum depth 1 %
//#define maxDepth=1           // Maximum depth 100%
double readADC(int port) {
    return analogRead(port);  // Example for Arduino
}

double Modfreqcalc(int port, double minf,double maxf, double Fs) {
    double ADC_Value = readADC(port);                                   // read ADC
    double Freq = (ADC_Value * (maxf-minf)+minf) / (double)ADC_MAX_VALUE; // calc FC   
    return Freq;
}
double DepthCalc(int port, double minDepth,double maxDepth, double Fs) {
    double ADC_Value = readADC(port);                                   // read ADC
    double Depth = (ADC_Value * (maxDepth-minDepth)+minDepth) / (double)ADC_MAX_VALUE; // calc FC   
    return Depth;
}


double TremoloMyDingDong(double Fs, int PortDepth,double minDepth, double maxDepth, int PortModFreq,double minf, double maxf, int portInput) {
// Parameters that are adjustable
double Modfreq = DepthCalc(PortDepth,minDepth,maxDepth,Fs);
double Depth = DepthCalc(PortModFreq,minf,maxf,Fs);
// Read the input
double x = readADC(portInput); 
// Create the lfo and then output the modulated Tremolo signal
// don't know how to implement the time inxed (t)
double lfo = (1 - Depth) + Depth * sin(2 * 3.1415926535897931 * Modfreq * t / Fs);
double output = x * lfo;

return output;
}