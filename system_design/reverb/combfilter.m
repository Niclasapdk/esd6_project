function [output] = combfilter(input, fs, delay_ms, gain_LP, reverb_time)
%gains for combfilter
g = 1-(0.366/reverb_time);
g2 = g*(1-gain_LP);

input_length = length(input);%length of input
M = ceil(delay_ms *10^-3 * fs);%convert delay from ms to samples
output = zeros(length(input), 1); % set output to 0
output_delayline = zeros(M, 1); % mem for delayline output
input_delayline = zeros(M+1, 1); % mem for delayline input

%circ buffer bookkeeping
start_output_offset = 0; %comb_offset
start_input_offset = 0;

%y[n]=x[n-m]-g1*x[n-m-1]+g1*y[n-1]+g2*y[n-m]
%rememberinput delayline n-m-1 m+1 length
for n=1:input_length
    %output(n)=input(n)-gain_LP*input_delay+gain_LP*output_delayline(ringbuf_idx(start_output_offset, 1, M))+g2*output_delayline(ringbuf_idx(start_output_offset, M, M));%-1 pga matlab
    output(n)=input_delayline(ringbuf_idx(start_input_offset,M,M+1))-...
        gain_LP*input_delayline(ringbuf_idx(start_input_offset,M+1,M+1))+...
        gain_LP*output_delayline(ringbuf_idx(start_output_offset, 1, M))+...
        g2*output_delayline(ringbuf_idx(start_output_offset, M, M));
    
    start_output_offset = mod(start_output_offset-1,M);%update offset
    output_delayline(start_output_offset+1) = output(n);%+1 kun pgamatalb  and  updates dealyline
    
    start_input_offset = mod(start_input_offset-1,M+1);%update offset
    input_delayline(start_input_offset+1) = input(n);%+1 kun pgamatalb  and  updates dealyline
end
