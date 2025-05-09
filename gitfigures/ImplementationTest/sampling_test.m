clc; close all; clear;

% Read data from CSV file
data = readmatrix('samplingtest.csv');  % Replace with your actual file name
time = data(:, 1);    % Time (seconds)
voltage = data(:, 2); % Voltage (volts)

% Detect local maxima
isPeak = islocalmax(voltage, 'FlatSelection', 'first', 'MinProminence', 0.6);
peakTimes = time(isPeak);
peakVoltages = voltage(isPeak);

% Filter to only peaks occurring after t > 0
validIdx = peakTimes > -0.1*10^-4;
peakTimes = peakTimes(validIdx);
peakVoltages = peakVoltages(validIdx);

% Select the first two valid peaks
peakTimes = peakTimes(5:6);
peakVoltages = peakVoltages(5:6);

% Plot the signal and just the two peaks
figure;
plot(time, voltage, 'b', 'LineWidth', 1.5);
hold on;
plot(peakTimes, peakVoltages, 'ro', 'MarkerSize', 6, 'LineWidth', 1.5);  % mark two peaks

% Draw line between the peaks
line(peakTimes, peakVoltages, 'Color', 'red', 'LineStyle', '--');

% Compute time difference and frequency
dt = abs(peakTimes(2) - peakTimes(1));
f = 1 / dt;

% Add legend with time and frequency info
legendStr = sprintf('\\Deltat = %.2e s, f = %.2f Hz', dt, f);
legend('Signal', legendStr, 'Location', 'southwest', 'Fontsize', 12);

xlim([7e-5 12e-5]);
xlabel('Time (s)', 'FontSize', 14);
ylabel('Voltage (V)', 'FontSize', 14);

title('ADC Sampling PIN29', 'FontSize', 16);
grid on;
