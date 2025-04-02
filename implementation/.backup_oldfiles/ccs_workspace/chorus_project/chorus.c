#include "chorus.h"

Int16 rate = 1000; 			// range(100 - 20000mHz)
Int16 depth = 10000; 		// range (3277 - 32439) Q15 (3277 = 0.1, 32439 = 0.99) 
Int16 invDepth = 10000; 	// 1 - depth
Int16 delayMin = 441;    	// delayMin/1000 * fs 
Int16 delayMax = 1103;   	// delayMax/1000 * fs

// lfo values
Int16 lfoIndex = 100;
Int16 lfoValue = 0;

//Get random Int16 number
//extern void rand16init(void);
//extern ushort randomData = rand16(DATA *r, ushort nr);

/*
void getRandomDelay(ushort delay){
	DATA rand_nums;  		 // random delay
	rand16(rand_nums, 1);
	
	Int16 delayMin = 441;    // delayMin/1000 * fs 
	Int16 delayMax = 1103;   // delayMax/1000 * fs
	 	
	delay = delayMin + (rand_nums * (delayMax - delayMin + 1));    
}
*/

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


}










