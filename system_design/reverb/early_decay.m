function [early_decay_out] = early_decay(input,fs)
%reverb
%early decay
tap_delay = [0 0.0199 0.0354 0.0389 0.0414 0.0699 0.0796]';
tap_gain = [1 1.02 0.818 0.635 0.719 0.267 0.242]';
%tap_delay = [0 0.0043 0.0215 0.0225 0.0268 0.0270 0.0298 0.0458 0.0485 0.0572 0.0587 0.0595 0.0612 0.0707 0.0707 0.0708 0.0726 0.0741 0.0753 0.0797];
%tap_gain = [1 0.841 0.504 0.491 0.379 0.380 0.346 0.289 0.272 0.192 0.193 0.217 0.181 0.180 0181 0176 0.142 0.142 0.167 0.134];

taps = zeros(length(tap_delay),1);

%convert ms delay to sample delay
tap_delay_samples = ceil(tap_delay * fs); %convert sec to sample
tap_delay_samples(1,1) = 1; %for at det første delay ikke er nul 

early_delayline = zeros(max(tap_delay_samples),1); 
early_decay_out = zeros(size(input));%vector of early deacy output

%TDL y[n] = a0*x[n]+a1*x[n-1] . . . aN*x[n-M_N] ??early_decay_out(n) = early_decay_out(n)+tap_gain(k)*early_delayline(tap_delay_samples(k));
for n=1:length(input)
    for k = 1:length(taps)
        taps(k) = early_delayline(tap_delay_samples(k));
    end
        early_decay_out(n) = sum(taps.*tap_gain);
        early_delayline = [input(n);early_delayline(1:max(tap_delay_samples)-1)];
end

end