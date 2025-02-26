clc; clear; close all;

% Parameters
fs = 44100;        % Sampling rate in Hz
duration = 2;      % Duration of the sweep in seconds
f_start = 20;      % Start frequency of sweep (Hz)
f_end = 20000;     % End frequency of sweep (Hz)
delay_ms = 20;     % Delay in milliseconds
gainLP = 0.46;      % Low-pass gain
reverbtime = 2.0;  % Reverberation time in seconds

% Generate logarithmic frequency sweep (chirp signal)
t = linspace(0, duration, duration * fs);
input = chirp(t, f_start, duration, f_end, 'linear');

% Apply comb filter
output = combfilter(input, fs, delay_ms, gainLP, reverbtime);

% Compute FFTs
NFFT = 2^nextpow2(length(input)); % Next power of 2 for efficiency
f = linspace(0, fs/2, NFFT/2);    % Frequency axis (one-sided)

X = abs(fft(input, NFFT));  % Magnitude spectrum of input
Y = abs(fft(output, NFFT)); % Magnitude spectrum of output

% Normalize to maximum value for better visualization
X = X / max(X);
Y = Y / max(Y);

% Plot Amplitude Response
figure;
semilogx(f, 20*log10(Y(1:NFFT/2)), 'r', 'LineWidth', 1.5); hold on;
semilogx(f, 20*log10(X(1:NFFT/2)), 'b--', 'LineWidth', 1);
xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
title('Amplitude Response of Comb Filter');
legend('Filtered Signal', 'Input Signal');
grid on;
axis([20 fs/2 -40 5]); % Limit y-axis to a reasonable range
