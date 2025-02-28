function [output] = reverb(input,fs,reverb_gain,reverb_time)
%reverb
comb_in = input;


%late decay
%combfilters
combfilter_delay = [50 56 61 68 72 78];
combfilter_gain = [0.46 0.48 0.50 0.52 0.53 0.55];
comb1 = combfilter(comb_in,fs,combfilter_delay(1),combfilter_gain(1),reverb_time);
comb2 = combfilter(comb_in,fs,combfilter_delay(2),combfilter_gain(2),reverb_time);
comb3 = combfilter(comb_in,fs,combfilter_delay(3),combfilter_gain(3),reverb_time);
comb4 = combfilter(comb_in,fs,combfilter_delay(4),combfilter_gain(4),reverb_time);
comb5 = combfilter(comb_in,fs,combfilter_delay(5),combfilter_gain(5),reverb_time);
comb6 = combfilter(comb_in,fs,combfilter_delay(6),combfilter_gain(6),reverb_time);

comb_out = (comb1+comb2+comb3+comb4+comb5+comb6)*(1/6);

%allpass
all_delay_ms = 6;
gain=0.7;
allpass_in=comb_out;
M = ceil(all_delay_ms *10^-3 * fs);%convert delay from ms to samples
allpass_out = zeros(length(allpass_in), 1); % set output to 0
output_delayline = zeros(M, 1); % mem for delayline output
input_delayline = zeros(M, 1); % mem for delayline input
%y[n]=-g*x[n]+x[n-m]+g*y[n-m]
for n=1:length(allpass_in)
    allpass_out(n)=-gain*allpass_in(n)+input_delayline(M)+gain*output_delayline(M);
    output_delayline = [allpass_out(n);output_delayline(1:M-1)];
    input_delayline = [allpass_in(n);input_delayline(1:M-1)];


%remember z^-d and should be a delay adjusts the last pulse from FIR
%section to roughly match in time with the first pulse to exit form the IIR
%section
late_decay = allpass_out;

output = (1-reverb_gain)*input+reverb_gain*late_decay;
end