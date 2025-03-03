%This script takes the magnitude response of the combfilter
clc, clear, close all;
fs = 44100; % Sampling frequency
N = 2^16; % FFT size for frequency resolution
freqs = linspace(0, fs/2, N/2+1); % Frequency axis

% Generate an impulse signal
impulse = zeros(N,1);
impulse(1) = 1;

% Filter parameters
delay_ms = 10; 
gainLP = 0.7; 
reverbtime = 1;

% Apply the comb filter
%output = combfilter(impulse, fs, delay_ms, gainLP, reverbtime);

%to test reverb
output = reverb(impulse, fs, gainLP, reverbtime);

%to test eaerly dealy
%output = early_decay(impulse, fs);

% Compute magnitude response using FFT
H = fft(output, N);
H_mag = abs(H(1:N/2+1)); % Take only positive frequencies

% Plot the magnitude response
figure;
semilogx(freqs, 20*log10(H_mag)); 
grid on;
xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
title('Magnitude Response of Comb Filter');
axis([20 fs/2 -50 20]); % Adjust axis for visibility
