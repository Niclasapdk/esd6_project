clc; close all; clear;
%dont look at comments

% Read data from CSV file
data = readmatrix('output_dsp_test.csv');  % Replace with your actual file name
time = data(:, 1);       % Frequency in Hz
voltage_1 = data(:, 2);  % Phase in radians
voltage_2 = data(:, 3);  % Magnitude of impedance
max_vol1 = max(voltage_1);
max_vol2 = max(voltage_2);


% Plot impedance magnitude and phase vs. frequency
figure;
input = plot(time, voltage_1, 'b');
hold on;
output = plot(time, voltage_2, 'r');  % e.g., peak points
xlim([-10e-3 -2e-3]);
i = yline(max_vol1, '--', 'LineWidth', 1.5, 'Color', 'blue');
o = yline(max_vol2, '--', 'LineWidth', 1.5, 'Color', 'r');
xlabel('Time (s)', 'FontSize', 14);
ylabel('Voltage (V)', 'FontSize', 14);
title('Output of DSP', 'FontSize', 16);
legend([input, output, i, o], ...
    'Input', ...
    'Output', ...
    ['Max Input Voltage = ' num2str(max_vol1) ' V'], ...
    ['Max Output Voltage = ' num2str(max_vol2) ' V'], ...
    'Location', 'southwest', 'Fontsize', 12);
grid on;
