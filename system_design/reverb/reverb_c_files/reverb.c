//remember to write modulo very bad do idx instead. bad because too much time
#include <stdio.h>
#include <ezdsp5535.h>
//**************************************//
//			functions for				// 
//			setting variables			//
//			and other stof				//
//**************************************//
static Int16 reverbCombGain = 0, reverbMix = 0;

//LUT for g in the g = 1-(0.366 / reverb_time) calculations with step size of 0.1sec from 0.5 to 4 sec
#define reverbGainLUTlen 36
static const Int16 reverbGainLUT[reverbGainLUTlen] = 
{
     8782,  12780,  15635,  17777,  19442,  20775,  21865,  22774,
    23543,  24202,  24773,  25272,  25713,  26105,  26456,  26771,
    27057,  27317,  27554,  27771,  27971,  28155,  28326,  28485,
    28632,  28770,  28899,  29020,  29134,  29241,  29341,  29437,
    29527,  29612,  29693,  29770
};

void setReverbTime(Int16 adcVal)//value from adc 10 bit 
{
	//calculate the idx for the LUT
	Int16 idx = ((Int32)adcVal * (Int32)(reverbGainLUTlen)) >> 10;//10 divide to approx 1024 and 15 to truncate
    
    //set g value
    reverbCombGain = reverbGainLUT[idx];
}

void setReverbMix(Int16 adcVal)//take Q15 value to set mix in reverb
{
	reverbMix = adcVal;
}
//**************************************//
//			Reverb Blocks				//
//**************************************//
//early reflectionz
Int16 early(Int16 x)
{
	Int16 y=0, i;
	Int32 sum = 0;
    #define tapN 6//length of early reflection vector
    #define earlyM 3511//0.0796*fs
    static Int16 tapGain[tapN] = {32767, 26804, 20808, 23560, 8749, 7930};//Q15: signed int = 32767*gain factor--first gain set to 1 instead of 1.02
    static Int16 tapDelay[tapN] = {878, 1562, 1716, 1826, 3083, 3511};//tapDelay * fs
    static Int16 earlyDelayline[earlyM] = {0};
    static Int16 earlyDelayTmp[tapN] = {0};
    static Uint16 idx = 0;
    
    for (i = 0; i < 6; i++)
    {
		earlyDelayTmp[i] = idx - tapDelay[i];
 		if (earlyDelayTmp[i] < 0) earlyDelayTmp[i] = earlyDelayTmp[i] + (earlyM-1);
 	}
    
    sum = ((Int32)tapGain[0] * earlyDelayline[earlyDelayTmp[0]]);//they divide by 2 ie << 1
    for (i = 1; i < tapN; i++)
    {
    	sum += ((Int16)tapGain[i] * earlyDelayline[earlyDelayTmp[i]]);
    }
    
    y = (sum >> 15) + x;
    earlyDelayline[idx] = x;
    idx++;
    if (idx >= earlyM) idx = 0;
    return y;
}

// combfilter
Int16 combfilters(Int16 x) {
	const Int16 g1[] = {15073, 15729, 16384, 17039, 17367, 18022};//moorergain*32767 -- gain Q15
	static Int16 g2[] = {11219, 10803, 10388, 9972, 9764, 9349}; // g2 calc from 1sec reverbtime
	#define comb1M 2205
	#define comb2M 2470
	#define comb3M 2691
	#define comb4M 2999
	#define comb5M 3176
	#define comb6M 3440
	const Int16 combMArray[] = {comb1M, comb2M, comb3M, comb4M, comb5M, comb6M};
	
	// A bunch of arrayz or smth
	#define DLINE(name, len) static Int16 name[len]={0} // macro for defining delaylinez
	// input delaylinez
	Int16 inpDelayline = 0;//the 1 sample input delay
	// output delaylinez
	DLINE(yLine1, comb1M); DLINE(yLine2, comb2M); DLINE(yLine3, comb3M);
	DLINE(yLine4, comb4M); DLINE(yLine5, comb5M); DLINE(yLine6, comb6M);
	// common shit arrayz of pointerz to the delaycolalinez on the sablez
	static Int16 *outDelayline[6] = {yLine1, yLine2, yLine3, yLine4, yLine5, yLine6};
	
	// Some indicez
	static Uint16 idxY[6] = {0}, idxY1[6];
	
	Int16 y, sum=0, i, g2Tmp;
	
	
	//calc g2 from g and g1
	for (i=0; i<6; i++)
	{
		g2Tmp = (((Int32)reverbCombGain*((Int32)32767-(Int32)g1[i])) >> 15);
		g2[i] = (((Int32)32767*(Int32)g2Tmp) >> 15);
	}
	
	// Dab on all them comb filterz
	for (i=0; i<6; i++) 
	{
		// Calculate indices while taking edge cases into account
		// y[n-m]: y[idxY]
		// y[n-1]: y[idxY-1]
		if ((Int16)idxY[i]-1 < 0) { // if we reach idx 0 and beyond of delayline
			idxY1[i] = combMArray[i]-1; // go to end of delayline
		} else {
			idxY1[i] = idxY[i]-1; // calculate index normally (linearly)
		}
	
		// Calculate output
		// y[n]=x[n-m]-g1*x[n-m-1]+g1*y[n-1]+g2*y[n-m] this shit big doo doo, use too much brain
		//y[n] = x[n] - g1 * x[n-1] + g1 * y[n-1] + g2 * y[n-m] this much better use small brain
		y = x + ((-(Int32)g1[i] * (Int32)inpDelayline + (Int32)g1[i] * (Int32)outDelayline[i][idxY1[i]] + 
		(Int32)g2[i] * (Int32)outDelayline[i][idxY[i]]) >> 15);
		sum += ((Int32)y*(Int32)5461 >> 15);//q15 1/6 is 5461 me think this right
	
		// Update delaylinez
		//inpDelayline[i][idx_x[i]] = x;
		inpDelayline = x;
		outDelayline[i][idxY[i]] = y;
	
		// Update indiciez
		//idx_x[i]++;
		idxY[i]++;
		//if (idx_x[i] >= combMArray[i]+1) idx_x[i] = 0;
		if (idxY[i] >= combMArray[i])   idxY[i] = 0;
	}
	
	return sum;
}

//Allpazz
//y[n]=x[n-m] - g*x[n] + g*y[n-m]
Int16 allpass(Int16 x)
{
    #define allM 265
    const Int16 gain = 22938;//65535/2*0.7 -- gain = 0.7
    static Int16 inpDelayline[allM] = {0};
    static Int16 outDelayline[allM] = {0};
    static Uint16 idx = 0;

    Int16 y = inpDelayline[idx] + ((-(Int32)gain*(Int32)x + 
    (Int32)gain*(Int32)outDelayline[idx]) >> 15);
    inpDelayline[idx] = x;
    outDelayline[idx] = y;

    //update circ buffer idx
    idx++;
    if (idx >= allM) idx = 0;
    return y;
}

//late delay reverberator
Int16 lateDelay(Int16 x)
{
    #define lateM 1172 //max tap delay 3511  / 3 = 1172
    static Int16 outDelayline[lateM] = {0};
    static Uint16 idx = 0;

    Int16 y = outDelayline[idx];

    outDelayline[idx] = x;
    idx++;
    if (idx >= lateM) idx = 0;
    return y;
}

//final reverb function
Int16 reverb(Int16 x)
{
	//inzet the functionzz zo zurkuz can be happy
	Int16 y=0, reverbEffectTmp=0;
	Int16 earlyTmp = early(x);
	Int16 lateTmp = combfilters(earlyTmp);
	lateTmp = allpass(lateTmp);
	lateTmp = lateDelay(lateTmp);
	reverbEffectTmp = earlyTmp + lateTmp; 
	y = ((((Int32)32767-(Int32)reverbMix) * (Int32)x + (Int32)reverbMix * (Int32)reverbEffectTmp) >> 15);
	return y;
}
