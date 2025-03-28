/* Tremolo demolo semolo boom!!!!!*/
#include "tremolo.h"

// Tremolo = Input * (i_val + depth * LFO) 

// Rate for LFO
void rateTremSet(Int16 rate){
    static Int16 min = 100, max = 10000, port = 1; // min = 0.1Hz, max = 10Hz       
    parameterSet(min, max, port, rate);
} 

// Tremolo Depth
void depthTremSet(Int16 depth){
    static Int16 min = 3277, max = 32439, port = 2; // min = 3277 = 0.1, max = 32439 = 0.99 Q(15)
    parameterSet(min, max, port, depth);
}

// Function for Tremolo
Int16 tremolo(Int16 xn){

static int initVal = 32767;         // initialValue = 32767 = 1 Q(15)   

//update rate & depth
static Int16 depth = 1, rate = 1;   // Initialise variables
depthTremSet(depth);

// LFO calculation
if (LFOIndex >= 9){                 // How many times it runs with the same Rate
    rateTremSet(rate);              // Update lfo rate
    LFOValue = lfo(rate);           // When used the same Rate for 9 times read new Rate
    LFOIndex = 0;
} else {
    LFOIndex++;                     // Increment until reach 9 then reset
}

//Tremolo = Input * (i_val + depth * LFO) 
xn = xn * (initVal + depth * LFOValue);

return xn;

}