clc; close all; clear;

% Read data from CSV file
data = readmatrix('input_impedance.csv');  % Replace with your actual file name
frequency = data(:, 1);   % Frequency in Hz
phaseZ = data(:, 2);      % Phase in radians
magnitudeZ = data(:, 3);  % Magnitude of impedance
magnitude_kOhm = magnitudeZ / 1000;

% Plot impedance magnitude and phase vs. frequency
figure;

%subplot(2,1,1);
semilogx(frequency, magnitude_kOhm, 'b', 'LineWidth', 1.5);
xlabel('Frequency (Hz)');
ylabel('|Z| (k\Omega)');
title('Impedance Magnitude');
grid on;

%{
subplot(2,1,2);
semilogx(frequency, rad2deg(phaseZ), 'r', 'LineWidth', 1.5);
xlabel('Frequency (Hz)');
ylabel('Phase (degrees)');
title('Impedance Phase');
grid on;
%}
