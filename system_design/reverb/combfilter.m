function [output] = combfilter(input, fs, delay_ms, gain_LP, reverb_time)
%gains for combfilter
g = 1-(0.366/reverb_time);
g2 = g*(1-gain_LP);

input_length = length(input);%length of input
M = ceil(delay_ms *10^-3 * fs);%convert delay from ms to samples
output = zeros(length(input), 1); % set output to 0
output_delayline = zeros(M, 1); % mem for delayline output
input_delay = 1; % input delay for n-1

%circ buffer bookkeeping
start_offset = 0; %comb_offset

%y[n]=x[n]-g1*x[n-1]+g1*y[n-1]+g2*y[n-m]
for n=1:input_length
    output(n)=input(n)-gain_LP*input_delay+gain_LP*output_delayline(ringbuf_idx(start_offset, 1, M))+g2*output_delayline(ringbuf_idx(start_offset, M, M));%-1 pga matlab
    
    start_offset = mod(start_offset-1,M);%update offset
    output_delayline(start_offset+1) = output(n);%+1 kun pgamatalb  and  updates dealyline
    input_delay = input(n);% updates the input n-1 delay
end
