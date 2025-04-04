%Chat cooking test
% chorusEffect(inputSound, Fs, mix, delay, width, rate, numVoices)

%Audioread
[inputSignal, Fs] = audioread('pen15.wav');  % Load audio file
duration = 10;  % Duration in seconds
% Extract first x seconds (ensure it does not exceed the file length)
numSamples = min(duration * Fs, length(inputSignal));
inputSignal = inputSignal(1:numSamples, :); % Keep only the first x seconds

%Chorus Parameters
mix = 0.2;
delay = 25e-3;
width = 5e-3;
rate = 0.5;
numVoices = 4;

% chorusEffect(inputSound, Fs, gain, delay, width, rate, numVoices)
outputSignal = chorusEffect(inputSignal, Fs, mix, delay, width, rate, numVoices);  % Apply chorus

%sound(inputSignal, Fs);
sound(outputSignal, Fs);  % Play output
%audiowrite('output_chorus.wav', outputSignal, Fs);  % Save output
