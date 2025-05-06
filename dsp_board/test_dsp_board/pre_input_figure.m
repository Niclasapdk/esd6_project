clc;close all; clear;

% Read the data
data = readmatrix('pre_output.csv');
freq = data(:, 2);   % Frequency in Hz
gain = data(:, 4);   % Gain in dB

% Find the gain at 20 Hz (or the closest point to 20 Hz)
[~, idx_20Hz] = min(abs(freq - 20));
initial_gain_20Hz = gain(idx_20Hz);

% Calculate the target gain (3 dB lower)
target_dB = initial_gain_20Hz - 3;

% Find the frequency where the gain is equal to target_dB
[~, idx_target] = min(abs(gain - target_dB));
target_freq = freq(idx_target);

% Plot the main graph
figure;
semilogx(freq, gain, 'b-');
hold on;

% Plot the dot at the target frequency and gain
semilogx(target_freq, target_dB, 'ro', 'MarkerSize', 8, 'DisplayName', 'Cut-off frequency');

% Axes and labels
xlabel('Frequency (Hz)');
ylabel('Gain (dB)');
title('Frequency Response for Output Stage');
grid on;
xticks([20 1e3 2e4]);
xticklabels({'20 Hz', '1 kHz', '20 kHz'});
%legend show;

% Convert target frequency to kHz for labeling
target_freq_kHz = target_freq / 1000;

% Create the combined label with dB and kHz
label_text = sprintf('%.2f kHz, \n %.2f dB', target_freq_kHz, target_dB);

% Add the label at the point
text(target_freq, target_dB, label_text, 'VerticalAlignment', 'bottom', 'Color', 'red');
