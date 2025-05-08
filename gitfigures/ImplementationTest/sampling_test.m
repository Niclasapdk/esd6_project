clc; close all; clear;

% Read data from CSV file
data = readmatrix('samplingtest.csv');  % Replace with your actual file name
time = data(:, 1);    % Time (seconds)
voltage = data(:, 2); % Voltage (volts)

% Plot the signal
figure;
plot(time, voltage, 'b', 'LineWidth', 1.5);
xlim([0 1e-4]);
xlabel('Time (s)');
ylabel('Voltage (V)');
title('ADC Sampling');
grid on;
