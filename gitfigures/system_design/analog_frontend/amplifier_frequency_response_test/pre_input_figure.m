clc; close all; clear;

% Read the data
data = readmatrix('pre_input.csv');
freq = data(:, 2);   % Frequency in Hz
gain = data(:, 4);   % Gain in dB

% --- Find gain at 20 Hz (initial reference point) ---
[~, idx_20Hz] = min(abs(freq - 20));
initial_gain_20Hz = gain(idx_20Hz);

% --- Cutoff frequency: 3 dB down from 7.85 dB ---
target_dB = -7.85 - 3;

% Find the frequency where the gain is closest to the target
[~, idx_target] = min(abs(gain - target_dB));
target_freq = freq(idx_target);

% --- Find gain at 1 kHz ---
[~, idx_1kHz] = min(abs(freq - 1000));
gain_1kHz = gain(idx_1kHz);
freq_1kHz = freq(idx_1kHz);

% --- Plotting ---
figure;
g = semilogx(freq, gain, 'b-', 'DisplayName', 'Frequency Response');
hold on;

% Plot cutoff frequency point
p = semilogx(target_freq, target_dB, 'ro', 'MarkerSize', 6, 'MarkerFaceColor', 'red', 'DisplayName', 'Cutoff Frequency');

% Plot gain at 1 kHz
h = semilogx(freq_1kHz, gain_1kHz, 'go', 'MarkerSize', 6, 'MarkerFaceColor', 'green', 'DisplayName', 'Passband Gain');

% Axes and labels
xlabel('Frequency (Hz)', 'FontSize', 14);
ylabel('Gain (dB)', 'FontSize', 14);
title('Frequency Response for Output Stage', 'FontSize', 16);
xlim([20 100000]);
%ylim([-8 10]);
grid on;

% Reference line at 20 kHz
xline(20000, '--black', '20 kHz', ...
    'LabelOrientation', 'horizontal', ...
    'LabelVerticalAlignment', 'bottom', ...
    'LabelHorizontalAlignment', 'right');

% Legend
legend([g, p, h], 'Location', 'southwest', 'FontSize', 12);

% Label for cutoff frequency
target_freq_kHz = target_freq / 1000;
label_text = sprintf('%.2f kHz,\n%.2f dB', target_freq_kHz, target_dB);
text(target_freq, target_dB, label_text, 'VerticalAlignment', 'bottom', 'Color', 'black');

% Label for 1 kHz point
label_1kHz = sprintf('1.00 kHz,\n%.2f dB', gain_1kHz);
text(freq_1kHz, gain_1kHz, label_1kHz, 'VerticalAlignment', 'bottom', 'Color', 'black');
