%This script takes the magnitude response of the combfilter
clc, clear, close all;
fs = 44100; % Sampling frequency
N = 2^14; % FFT size for frequency resolution
freqs = linspace(0, fs/2, N/2+1); % Frequency axis

% Generate an impulse signal
impulse = zeros(N,1);
impulse(1) = 1;

% Filter parameters
delay_ms = 1; 
gainLP = 0.7; 
reverbtime = 1;

% Apply the comb filter
output = combfilter(impulse, fs, delay_ms, gainLP, reverbtime);
output_w = combfilter_with_input_delayline(impulse, fs, delay_ms, gainLP, reverbtime);

%to test reverb
%output = reverb(impulse, fs, gainLP, reverbtime);

%to test eaerly dealy
%output = early_decay(impulse, fs);

% Compute magnitude response using FFT
H = fft(output, N);
H_mag = abs(H(1:N/2+1)); % Take only positive frequencies

H_w = fft(output_w, N);
H_mag_w = abs(H_w(1:N/2+1)); % Take only positive frequencies

% Plot the magnitude response
figure;
semilogx(freqs, 20*log10(H_mag), 'b', 'DisplayName', 'Without Input Delayline'); 
hold on;
semilogx(freqs, 20*log10(H_mag_w), 'r--', 'DisplayName', 'With Input Delayline'); 
grid on;
xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
title('Magnitude Response of Comb Filter');
legend;
axis([20 fs/2 -7 15]); % Adjust axis for visibility
