%Tremolo

clear; clc;

[filename,pathname] = uigetfile('*.*','Select input audio file');

[x1, fs] = audioread(num2str(filename));

t = [0:length(x1)-1]/fs;
t = t(:);

f = 5; %LFO frequency
td = 3; %tremolo depth

lfo = 1+td*sin(2*pi*f*t); %creates lfo with modification from depth

out = x1 .* lfo; %AM modulation

audiowrite('guitar_tremolo.wav', out/max(abs(out)), fs);