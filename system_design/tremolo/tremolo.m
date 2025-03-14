%Tremolo

clear; clc;
%Parameter declaration
[input, fs] = audioread('input.wav');   %Input and samplingfreq
Modfreq = 5;                            %LFO frequency
depth = 3;                              %tremolo depth
function output=tremolo(Modfreq,depth,fs,input)

%signal modulation
t = (0:length(input)-1)/fs;                 %create index for lfo
lfo = (1 - depth)+depth*sin(2*pi*Modfreq*t);          %creates lfo with freq and depth
%lfo = (1 - depth)+depth*sawtooth(2*pi*Modfreq*t,.5)  %Triangle wave
%lfo = (1 - depth)+depth*square(2*pi*Modfreq*t)       %Square wave
output = input .* lfo;                      %AM modulate input with lfo

% Normalize the output based of max value of in and out
overall_gain = max(abs(input(:))) / max(abs(output(:)));
output = output * overall_gain;

%outputs
sound(output, fs); % Play the processed sound
audiowrite('output_tremolo.wav', output, fs); %save file
end