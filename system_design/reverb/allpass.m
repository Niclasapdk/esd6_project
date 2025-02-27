function [output] = allpass(input, fs, delay_ms, gain)
M = ceil(delay_ms *10^-3 * fs);%convert delay from ms to samples
output = zeros(length(input), 1); % set output to 0
output_delayline = zeros(M, 1); % mem for delayline output
input_delayline = zeros(M, 1); % mem for delayline input

%y[n]=-g*x[n]+x[n-m]+g*y[n-m]
for n=1:length(input)
    output(n)=-gain*input(n)+input_delayline(M)+gain*output_delayline(M);
    output_delayline = [output(n);output_delayline(1:M-1)];
    input_delayline = [input(n);input_delayline(1:M-1)];
end