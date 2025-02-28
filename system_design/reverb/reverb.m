function [output] = reverb(input,fs,reverb_gain,reverb_time)
%reverb
%early decay
tap_delay = [0 0.0199 0.0354 0.0389 0.0414 0.0699 0.0796];
tap_gain = [1 1.02 0.818 0.635 0.719 0.267 0.242];
%tap_delay = [0 0.0043 0.0215 0.0225 0.0268 0.0270 0.0298 0.0458 0.0485 0.0572 0.0587 0.0595 0.0612 0.0707 0.0707 0.0708 0.0726 0.0741 0.0753 0.0797];
%tap_gain = [1 0.841 0.504 0.491 0.379 0.380 0.346 0.289 0.272 0.192 0.193 0.217 0.181 0.180 0181 0176 0.142 0.142 0.167 0.134];

%convert ms delay to sample delay
tap_delay_samples = ceil(tap_delay * fs); %convert sec to sample

N = length(input);%lenght of input
early_decay_out = zeros(1, N);%output
taps = length(tap_gain);%number of taps
early_delay_line = zeros(max(tap_delay_samples), 1); % mem for delayline early decay

%TDL y[n] = a0*x[n]+a1*x[n-1] . . . aN*x[n-M_N]
for n=1:N
    for k = 1:taps
        d = tap_delay_samples(k);  %delay in samples for kth tap
        if (n - d) >= 1
            early_decay_out(n) = early_decay_out(n)+tap_gain(k)*input(early_delay_line);
        end
        early_delay_line = [input(n);early_delay_line(1:max(tap_delay_samples)-1)];
        %if (n - d) < 1, we assume x(n-d) = 0 (causal filter)
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%late decay
comb_in = input;
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
gain = 0.7;
allpass_in = comb_out;
M_all = ceil(all_delay_ms *10^-3 * fs);%convert delay from ms to samples
allpass_out = zeros(length(allpass_in), 1); % set output to 0
output_delayline = zeros(M_all, 1); % mem for delayline output
input_delayline = zeros(M_all, 1); % mem for delayline input
%y[n]=-g*x[n]+x[n-m]+g*y[n-m]
for n=1:length(allpass_in)
    allpass_out(n) = -gain*allpass_in(n)+input_delayline(M_all)+gain*output_delayline(M_all);
    output_delayline = [allpass_out(n);output_delayline(1:M_all-1)];
    input_delayline = [allpass_in(n);input_delayline(1:M_all-1)];


%remember z^-d and should be a delay adjusts the last pulse from FIR
%section to roughly match in time with the first pulse to exit form the IIR
%section
late_decay = allpass_out;

output = (1-reverb_gain)*input+reverb_gain*(late_decay+early_decay_out);
end