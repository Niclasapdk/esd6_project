function [early_decay_out] = early_decay(input,fs)
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
        early_decay_out(n) = early_decay_out(n)+tap_gain(k)*input(early_delay_line);
    end
        early_delay_line = [input(n);early_delay_line(1:max(tap_delay_samples)-1)];
        %if (n - d) < 1, we assume x(n-d) = 0 (causal filter)
end
end