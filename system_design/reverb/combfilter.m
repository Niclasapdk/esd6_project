function [output] = combfilter(input, fs, delay_ms, gain_LP, reverb_time)
%gains for combfilter
g = 1-(0.366/reverb_time);
g2 = g*(1-gain_LP);

M = ceil(delay_ms *10^-3 * fs);%convert delay from ms to samples
output = zeros(length(input), 1); % set output to 0
output_delayline = zeros(M, 1); % mem for delayline output
input_delayline = zeros(M, 1); % mem for delayline input

%y[n]=x[n]-g1*x[n-1]+g1*y[n-1]+g2*y[n-m]
for n=1:length(input)
    output(n)=input(n)-gain_LP*input_delayline(1)+gain_LP*output_delayline(1)+g2*output_delayline(M);
    output_delayline = [output(n);output_delayline(1:M-1)];
    input_delayline = [input(n);input_delayline(1:M-1)];
end
