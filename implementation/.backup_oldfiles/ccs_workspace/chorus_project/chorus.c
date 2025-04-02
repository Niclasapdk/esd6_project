#include "chorus.h"
// kan man godt bare gør sådan her
#include rand16.asm
// link til hvor rand16.asm er fundt https://www.ti.com/lit/pdf/spru518
// Get random Int16 number
extern void rand16init(void);
extern short rand16(DATA *x, ushort nx);


Int16 rate = 1000; 			// range(100 - 20000mHz)
Int16 depth = 10000; 		// range (3277 - 32439) Q15 (3277 = 0.1, 32439 = 0.99) 
Int16 invDepth = 10000; 	// 1 - depth
Int16 delayMin = 441;    	// delayMin/1000 * fs 
Int16 delayMax = 1103;   	// delayMax/1000 * fs
Int16 DelayIndex = 0;		// Index for delayLine
Int16 Modsignal = 0;		// Modulated signal set 0 so far
Int16 DelayLine[2204];		// 50 ms DelayLine, absolut max
Int16 DelaySize = 0;	    // The amount of delay it has to look back
Int16 DelayBack = 0;	    // Which delay nr is should look back at

// lfo values
Int16 lfoIndex = 100;
Int16 lfoValue = 0;

// test this function
void getRandomDelay(ushort *delay) {  
    DATA rand_nums = 0;  // Ensure it's initialized

    rand16(&rand_nums, 1);  // Pass by reference if expected

    Int16 delayMin = 441;
    Int16 delayMax = 1103;

    *delay = delayMin + (rand_nums % (delayMax - delayMin + 1));  
}

void setRateChorus(Int16 adcVal){
	rate = 100 + (((Int32)adcVal * (20000-100)) >> 10); 	  //Q15.0 (100-20000mHz)
}

void setDepthChorus(Int16 adcVal2){
	depth = 3277 + (((Int32)adcVal2 * (32439 - 3277)) >> 10); //Q0.15 (0.1-0.99)
	invDepth = 32767 - depth;
}

void setDelayChorus(Int16 adcVal3){
	delayMin = 221 + (((Int32)adcVal3 * (662 - 221)) >> 10); //Determine delayMin in range(5-15ms) Q0.15
	//Maps delayMin to delayMax:
	//delayMin: 5ms  -> delayMax: 50ms
	//delayMin: 15ms -> delayMax: 25ms
	delayMax = ((-25 * delayMin + 27575) * 205) >> 11; 		 //delayMax = ((-25 * delayMin) + 27575) / 10 (range(25-50ms))   	
}

Int16 chorus(Int16 x){

//Update parameters
setDepthChorus(adcVal2);

//Update lfo
if(lfoIndex >= 9){
	setRateChorus(adcVal);
	lfoValue = lfo(); //Change to lfo name.	
	lfoIndex = 0;
} else {
	lfoIndex++;
}

/*Delay calculation for the modulations*/
DelayLine[DelayIndex] = x;		    /*Store 1st element in the delayline to be input*/
DelayIndex++;					    /*Increment, so next input is loaded to next index*/
if(DelayIndex >= maxDelaySamp) {	/*When the MaxDelay is recieved reset */
    DelayIndex = 0;
}

// Modulated all the voices and add the together
for (Int16 Voice=0; Voice < 3; Voice++) {                   /*Make sure the LFO oscilates in 0 to 1 for this to work*/
getRandomDelays(&DelaySize);         						// Get random delay for each voice
// Some calculations for delaysize
Delaysize = (((Int32)DelaySize * Depth)>>15);				// Control depth of the Delay Q15
lfoValue = 16384 + (((Int32)16384 * lfoValue)>>15);			// lfoValue calc in Q15, (0.5+(0.5*lfoValue))
// Delaysize calculation
Delaysize = ((Int32)Delaysize * lfoValue)>>15;				// calculate the actual Delaysize
DelayBack = DelayIndex - DelaySize;						    // Indicates which index the delay is at
                                                            // EX: 0 - 25 then look at -25 sample
if (DelayBack < 0) {															
    DelayBack += maxDelaySamp;						// Wrap around if negative 
                                                            // EX: -25 + Max = DelayBack
}
/* Combine the modulations (voices)*/
Int16 Temp = (((Int32)Depth * DelayLine[DelayBack])>>15);	// Q15, Depth*Delay = output mod
Temp = Temp>>2;												// >>2 = /4		cause 4 voices
ModSignal+= Temp;											// Add all the delays together				
}
/* Mix the modulation effect and the input*/
x = (((Int32)invDepth*x)>>15) + ModSignal;					// Add input and modulation together
return x;
}










