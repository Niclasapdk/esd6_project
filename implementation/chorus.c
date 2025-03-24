
/*Get random delay for chorus*/



extern void rand16init(void);               /* Initialize random number generator PDF 118*/
extern ushort rand16(DATA *r, ushort nr);   /* Generate a random number generator PDF 116*/

// Function to generate a random delay using rand16()
void getRandomDelay(Int16 DelayMin, Int16 DelayMax,Int16 Voices,ushort Delays) {
    DATA rand_nums[Voices];                 // Store the random number [0,..,65535]
    rand16(rand_num, Voices);               // Generate x(Voices) amount of random numbers from data
    for (Int16 i = 0; i < Voices; i++) {
        Delays[i] = DelayMin + (rand_nums[i] % (DelayMax - DelayMin + 1));  // scale to [10,...,25]
    }
}


Int16 Chorus(Int16 x) {
/*Needs to implemented using potensmeters*/
static Int16 Fs=44100, Voices = 4, DelayMin = 10, DelayMax = 25, Depth=0.7,Rate=1;
/*Convert from Delay Max to ms to samples*/
Int16 Time = (DelayMax + 10)*Fs/1000;                 // Time saved of older samples
/*Create array with the random delays */
ushort Delays[Voices];                                // Array to store delays for each voice
getRandomDelays(DelayMin, DelayMax, Voices, Delays);  // Get delays for each voice stor in delay[]

/*Make the modulation delays*/
for (Int16 i=0; i < Voices; i++) {
    /* Insert LFO here*/
    Int16 lfo = 0;
    /*Skal laves om til 2D array så t (tidindex) og voice index(i) begge er der når lfo implementeres*/
    Delays[i] =  Delays[i] * Depth *(0.5*lfo+0.5);

    /*Something seem wrong with Delay[j], dont know if just me, burde være 2D array*/
    for (Int16 j=0; j < Time; j++) {
        Int16 DelaySamples=Delays[j];             // declare the delay
        if (j - DelaySamples >= 0) {
            ushort ModSignal[j] = input[j - DelaySamples];  // Apply delay
        } else {
            ModSignal[j] = 0;                        // Zero padding for out-of-bounds
        }
    }
    /*IDK skal implementeres med tidsindex (t)*/
    ushort Output = Output + Modsignal[t] /Voices;
}
/* Combine the modulation (output) and input signal*/
Output = Output + Input;
}