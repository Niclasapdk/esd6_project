function [output] = combfilter(input, fs, delay_ms, gainLP, reverbtime)
%gains for combfilter
g = 1-(0.366/reverbtime);
g2 = g*(1-gainLP);

M = ceil(delay_ms *10^-3 * fs);%convert delay from ms to samples
output = zeros(length(input), 1); % set output to 0
output_delayline = zeros(M, 1); % mem for delayline output
input_delayline = zeros(M, 1); % mem for delayline input

%y[n]=x[n]-g1*x[n-1]+g1*y[n-1]+g2*y[n-m]
for n=1:length(input)
    output(n)=input(n)-gainLP*input_delayline(1)+gainLP*output_delayline(1)+g2*output_delayline(M);
    output_delayline = [output(n);output_delayline(1:M-1)];
    input_delayline = [input(n);input_delayline(1:M-1)];
end
