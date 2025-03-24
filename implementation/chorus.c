
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
/*Needs to be in Q15 everything*/

/*Parameters declaration*/
static Int16 Fs=44100, Voices = 4, DelayMin = 10, DelayMax = 25,DelayMin = 10, Depth=0.7, Rate=1;
static Int16 maxDelaySamp=(((Int32)(DelayMax)*Fs)>>15)/1000; 	   //Convert to ms, then samples
static Int16 minDelaySamp=(((Int32)(DelayMin)*Fs)>>15)/1000; 	   //Convert to ms, then samples
static Int16 DelayLine[maxDelaySamp], DelaySize[Voices], DelayBack[Voices];
static Int16 DelayIndex = 0, ModSignal = 0;

/*Delay calculation for the modulations*/
DelayLine[DelayIndex] = x;		    /*Store 1st element in the delayline to be input*/
DelayIndex++;					    /*Increment, so next input is loaded to next index*/
if(DelayIndex >= maxDelaySamp) {	/*When the MaxDelay is recieved reset */
    DelayIndex = 0;
}
getRandomDelays(DelayMin, DelayMax, Voices, DelaySize);         // Get delays for each voice stor in delay[]
for (Int16 Voice=0; Voice < Voices; Voice++) {                   /*Make sure the LFO oscilates in 0 to 1 for this to work*/
    /*Insert LFO's here*/
DelaySize[Voice] = (((Int32)DelaySize[Voice] * Depth * (0.5 LFOValue[Voice]+0.5))>>15); // Calculate delay used based on lfo
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