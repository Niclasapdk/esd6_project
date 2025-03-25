// Int16 lfo(Int16 Rate) {
/* Get it all set to Q15, so it fits to fixed point*/

/* Set parameters to intialize the lfo*/
// static Int16 x1 = 0, x2=0;                      // Set prevoius inputs to 0 for first iteration
// static Int16 Normalize = 23170 ;                // 3 dB, 1/\sqrt(2) in Q15 0.707*32767 = 23170 

/* Kan jeg sætte dem her til static også så de kunne bliver regnet engang iforhold til hver rate*/
// Int16 k=0.00014247585 * Rate;                   // 2PI * 44100 = 0.00014247585 Table 10.2
// Int16 k_in_2 = (EPM(k,k));                      // K^2
// Int16 K_thing = 1-k_in_2/2;                     // 1-K^2/2

// Matrix setup 
// Int16 y1 = (EPM(K_thing,x1))+(EPM(k,x2));       // Line 1 in matrix to calculate output
// Int16 y2 = -(EPM(k,x2))+(EPM(K_thing,x1));      // Line 2 in matrix
// Calculate signal power
// Int16 P = (EPM(y1,y1))+(EPM(y2,y2));            // Signal power table 10.2
//Gain calculation 
// G= 3/2-P;                                       // Correction gain table 10.2
// Set output to right gain
// y1 = (EPM(y1,G));                               
// y2 = (EPM(y2,G));
// Save prevoius output to next generation
//x1 = y1;
// x2 = y2;
// Normalize the output to [-1,1] range
// Int16 lfo = EPM(y1,Normalize)>>15;              // mutiply 2 Q15 = Q30 then shift 15, = Q15
// return lfo;
// }