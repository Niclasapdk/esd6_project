#include <stdio.h>
#include <stdint.h>

//Set parameters for boundaries
#define RISING 32767
#define FALLING 0

// Take mHz as input
int triangleLFO(int r){
    // The phase deviates in range [-1, 1] Q15

    int stepsize;          
    static int lfostat = RISING;         //Stat if rising or falling set to rise 1st
    static int phase = -32767;           //Phase of the triangle lfo

    r = r >> 10;                         // Convert from Mhz to Hz, Around r/1000   (1024)


    //Q15 calc stepsize,  (r/44100) 
    stepsize = ((long)r*48693) >>16;     // r/44100
    stepsize = stepsize << 1;            // *2 as the range is [-1,1] instead of [0,1]
    stepsize = stepsize * 10;            // cause is only updates ever 10 iteration

    if( lfostat == RISING){              // increment phase until reach max    
        phase += stepsize;            
        if(phase >= 32767){
            phase = 32767;
            lfostat = FALLING;
        }
    }
    if(lfostat == FALLING){              // Decrease phase until reach 0
        phase -= stepsize;
    } if(phase <= -32767){
        phase = -32767;
        lfostat = RISING;
    }
    return phase;
}

// Take mHz as input
int SquareLFO(int r){
    // The phase deviates in range [-1, 1] Q15
    int samplesTime; 
    static int counter = 0;         
    static int lfostat = RISING;         //Stat if rising or falling set to rise 1st
    static int phase = 32767;            //Phase of the triangle lfo
    r = r >> 10;                           // Convert from Mhz to Hz, Around r/1000
    // Find way to remove divide
    samplesTime = 44100/r;    // convert to sample r/44100


    // when counter exceed rate in samples switch stat between [-1,1]
    if(counter >= samplesTime){
        if(lfostat == RISING){
            phase = 32767;
            lfostat = FALLING;

        }
        else {
            phase = -32767;
            lfostat = RISING;
        }
        counter = 0;
    }
    counter++;
    return phase;
}




int main() {
    FILE *f = fopen("triangleLFO_output.txt", "w");  // Open file for writing

    if (f == NULL) {
        printf("Error opening file for writing.\n");
        return -1;  // Exit if file can't be opened
    }

    int r = 4;  // Rate (change as necessary)
    int i;
    for (i = 0; i < 1000; i++) {  // Test for 1000 iterations, adjust as needed
        int lfo_value = triangleLFO(r);  // Get LFO value
        fprintf(f, "%d\n", lfo_value);  // Write value to file
    }

    fclose(f);  // Close the file after writing

    printf("Triangle LFO values written to triangleLFO_output.txt\n");

    return 0;
}