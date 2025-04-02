%Tremolo
clear; clc;

%Parameter declaration
[input, fs] = audioread('pen15.wav');   % Input and samplingfreq
modFreq = 5;                            % LFO frequency
depth = 0.3;                            % Tremolo depth (0 to 1)
initVal = 1;                            % Initial Value 

function output = tremolo_effect(input, fs, modFreq, depth, initVal)
t = (0:length(input)-1)/fs;             % Create index for lfo
t = t(:);
%lfo = sin(2*pi*modFreq*t);              % Sin wave (lfo)
lfo = sawtooth(2*pi*modFreq*t,.5);      % Triangle wave (lfo)
%lfo = square(2*pi*modFreq*t);           % Square wave (lfo)
tremolo = initVal + depth * lfo;          
output = input .* tremolo;              % AM modulate input with lfo

% Normalize the output based of max value of in and out
overall_gain = max(abs(input(:))) / max(abs(output(:)));
output = output * overall_gain;
end

% Outputs
output_signal = tremolo_effect(input, fs, modFreq, depth, initVal);
sound(output_signal, fs);               % Play the processed sound  