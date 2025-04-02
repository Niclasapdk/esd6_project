#include "chorus.h"

/*Get random delay for chorus*/
extern void rand16init(void);               /* Initialize random number generator PDF 118*/
extern ushort rand16(DATA *r, ushort nr);   /* Generate a random number generator PDF 116*/

// Function to generate a random delay using rand16()
void getRandomDelay(Int16 DelayMin, Int16 DelayMax, Int16 Voices, ushort Delays) {
    DATA rand_nums[Voices];                 // Store the random number [0,..,65535]
    rand16(rand_num, Voices);               // Generate x(Voices) amount of random numbers from data
    for (Int16 i = 0; i < Voices; i++) {
        Delays[i] = DelayMin + (rand_nums[i] % (DelayMax - DelayMin + 1));  // scale to [10,...,25]
    }
}

/* lav til Q15*/
void depthChorusSet(Int16 Depth, Int16 *InvDepth){
    static Int16 Min = 0.001, Max = 0.999, port = 1;        // min = 0.1 % and Max 99.9 %
    parameterSet(Min,Max,port,Depth);                       // Map ADC value for port1 to Depth
    *InvDepth = 1-Depth;                                    // Get InvDepth for x (input)
}

/* lav til Q15 */
void rateChorusSet(Int16 Rate){
    static Int16 Min = 0.1, Max = 20, port = 2;             // Min 0.1 Hz and Max 20 Hz
    parameterSet(Min,Max,port,Rate);                        // Map ADC value for port2 to Rate
}

/* lav til Q15 */
void delayChorusSet(Int16 DelayMin, Int16 *DelayMax){
    static Int16 Min = 5, Max = 15, port = 3;               // Min 5 Hz and Max 15 Hz for DelayMin
    parameterSet(Min,Max,port,DelayMin);                    // Map ADC value for port3 to DelayMin
    *DelayMax = (EPM(DelayMin,-2.5))+ 62.5;                 // Linear equation y=mx+b for DelayMax Mapping
    /*Result in the linear equation for DelayMax
    DelayMin: 5  -> DelayMax: 50.00
    DelayMin: 6  -> DelayMax: 47.50
    DelayMin: 7  -> DelayMax: 45.00
    DelayMin: 8  -> DelayMax: 42.50
    DelayMin: 9  -> DelayMax: 40.00
    DelayMin: 10 -> DelayMax: 37.50
    DelayMin: 11 -> DelayMax: 35.00
    DelayMin: 12 -> DelayMax: 32.50
    DelayMin: 13 -> DelayMax: 30.00
    DelayMin: 14 -> DelayMax: 27.50
    DelayMin: 15 -> DelayMax: 25.00
    */
   /* Måske for indsat MaxDelaySamp og MinDelaySamp op her, så den kun skal udregne min og max en gang*/
}


Int16 Chorus(Int16 x) {
/*Needs to implemented using potensmeters*/
/*Needs to be in Q15 everything*/

/*Parameters declaration*/
static Int16 Fs = 44100, Voices = 4;             
static Int16 DelayMin = 327680, DelayMax = 819200;          //327680 = 10ms, 819200 = 25ms, Q(15) 
static Int16 Depth = 22938, Rate = 32767, Delay = 327670;     //22938 = 0.7, 32767 = 1, 327670 = 10, Q(15) 
static Int16 invDepth = 32767 - Depth;                  //32767 = 1, Q(15)
static Int16 LFOIndex = 100, LFOValue = 0;
static Int16 AbsDelayMax = 50;               

static Int16 absolutMaxSamp = ((Int32)AbsDelayMax * (Fs / 1000));
static Int16 maxDelaySamp = ((Int32)DelayMax * (Fs / 1000));  // Convert ms to samples
static Int16 minDelaySamp = ((Int32)DelayMin * (Fs / 1000));  // Convert ms to samples
static Int16 DelayLine[absolutMaxSamp], DelaySize[Voices], DelayBack[Voices];
static Int16 DelayIndex = 0, ModSignal = 0;

// parameter update
depthChorusSet(Depth, invDepth);
delayChorusSet(DelayMin, DelayMax);

// LFO calculation
if (LFOIndex >= 9){                 // How many times it runs with the same Rate
    rateChorusSet(Rate);            // Update lfo rate
    LFOValue = lfo(Rate);           // When used the same Rate for 9 times read new Rate
    LFOIndex = 0;
} else {
    LFOIndex++;                     // Increment until reach 9 then reset
}
/*Delay calculation for the modulations*/
DelayLine[DelayIndex] = x;		    /*Store 1st element in the delayline to be input*/
DelayIndex++;					    /*Increment, so next input is loaded to next index*/
if(DelayIndex >= maxDelaySamp) {	/*When the MaxDelay is recieved reset */
    DelayIndex = 0;
}
getRandomDelays(DelayMin, DelayMax, Voices, DelaySize);         // Get delays for each voice stor in delay[]
for (Int16 Voice=0; Voice < Voices; Voice++) {                   /*Make sure the LFO oscilates in 0 to 1 for this to work*/
    /* This line cant find out how to mulitiply this many numbers with Int32 or (EPM)*/
DelaySize[Voice] = (((Int32)DelaySize[Voice] * Depth * (0.5 * LFOValue + 0.5))>>15); // Calculate delay used based on lfo
DelayBack[Voice] = DelayIndex - DelaySize[Voice];							    // Indicates which index the delay is at
                                                                                // EX: 0 - 25 then look at -25 sample
if (DelayBack[Voice] < 0) {															
    DelayBack[Voice] += maxDelaySamp;											// Wrap around if negative 
                                                                                // EX: -25 + Max = DelayBack
}
/* Combine the modulations (voices)*/
ModSignal+=(((Int32)Depth * DelayLine[DelayBack[Voice]])>>15)/Voices;
}
/* Mix the modulation effect and the input*/
x = (((Int32)(1-Depth)*x)>>15) + ModSignal;
return x;
}