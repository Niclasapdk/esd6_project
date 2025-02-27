%This script takes the magnitude response of the allpass
clc, clear, close all;
fs = 44100; % Sampling frequency
N = 2^14; % FFT size for frequency resolution
freqs = linspace(0, fs/2, N/2+1); % Frequency axis

% Generate an impulse signal
impulse = zeros(N,1);
impulse(1) = 1;

% Filter parameters
delay_ms = 6; 
gain = 0.7; 

% Apply the comb filter
output = allpass(impulse, fs, delay_ms, gain);

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
axis([20 fs/2 -0.1 0.1]); % Adjust axis for visibility