clc; close all; clear;

% Read data from CSV file
data = readmatrix('allfx_computationtime.csv');  % Replace with your actual file name
time = data(:, 1);       % Time (seconds)
voltage1 = data(:, 3);   % Signal 1
voltage2 = data(:, 2);   % Signal 2

% Define high threshold
threshold = 3.0;

% === Signal 1: Detect high durations ===
above1 = voltage1 > threshold;
risingEdges1 = find(diff([0; above1]) == 1);
fallingEdges1 = find(diff([above1; 0]) == -1);
numPulses1 = min(length(risingEdges1), length(fallingEdges1));
pulseDurations1 = time(fallingEdges1(1:numPulses1)) - time(risingEdges1(1:numPulses1));
avgHigh1 = mean(pulseDurations1);

% === Signal 2: Detect high durations ===
above2 = voltage2 > threshold;
risingEdges2 = find(diff([0; above2]) == 1);
fallingEdges2 = find(diff([above2; 0]) == -1);
numPulses2 = min(length(risingEdges2), length(fallingEdges2));
pulseDurations2 = time(fallingEdges2(1:numPulses2)) - time(risingEdges2(1:numPulses2));
avgHigh2 = mean(pulseDurations2);

% === Plot Signals ===
figH=figure;
h1=plot(time, voltage1, 'b', 'LineWidth', 1.5); hold on;
h2=plot(time, voltage2, 'g', 'LineWidth', 1.5);

% === Define visible window ===
t_min = 7e-5;
t_max = 15.5e-5;

% === Signal 1: Find first pulse within xlim ===
valid1 = time(risingEdges1) >= t_min & time(fallingEdges1) <= t_max;
firstIdx1 = find(valid1, 1);
if ~isempty(firstIdx1)
    t1_start = time(risingEdges1(firstIdx1));
    t1_end   = time(fallingEdges1(firstIdx1));
    y_level = 3.6;
    
    % Plot markers at rising and falling edges
    plot(t1_start, y_level, 'ko', 'MarkerFaceColor', 'k', 'MarkerSize', 6);
    plot(t1_end, y_level, 'ko', 'MarkerFaceColor', 'k', 'MarkerSize', 6);
    
    % Plot dashed line between rising and falling edges
    hPulse1=plot([t1_start t1_end], [y_level y_level], 'k--', 'LineWidth', 1);
end
% === Signal 2: Same logic ===
valid2 = time(risingEdges2) >= t_min & time(fallingEdges2) <= t_max;
firstIdx2 = find(valid2, 1);
if ~isempty(firstIdx2)
    t2_start = time(risingEdges2(firstIdx2+1));
    t2_end   = time(fallingEdges2(firstIdx2+1));
    y_level = 3.6;
    
    % Plot markers at rising and falling edges
    plot(t2_start, y_level, 'ro', 'MarkerFaceColor', 'r', 'MarkerSize', 6);
    plot(t2_end, y_level, 'ro', 'MarkerFaceColor', 'r', 'MarkerSize', 6);
    
    % Plot dashed line between rising and falling edges
    hPulse2=plot([t2_start t2_end], [y_level y_level], 'r--', 'LineWidth', 1);
end

%text(mid1, 3.7, sprintf('%.2e s', t1_end - t1_start), ...
%    'Color', 'black', 'FontSize', 10, 'HorizontalAlignment', 'center');

%text(mid2, 3.7, sprintf('%.2e s', t2_end - t2_start), ...
%    'Color', 'black', 'FontSize', 10, 'HorizontalAlignment', 'center');

% === Add Legend with high time info ===
legendStr1 = sprintf('Avg high time = %.2f μs, Pulse length = %.2f μs', avgHigh1*1e6, (t1_end - t1_start)*1e6);
legendStr2 = sprintf('Sample read: Pulse length = %.2f μs', (t2_end - t2_start)*1e6);
legend([h1, h2,hPulse1, hPulse2],'Computation Time', 'Time for sample read to sample read',legendStr1, legendStr2, ...
       'Location', 'southwest', 'FontSize', 10);

% === Labels and formatting ===
xlim([7e-5 15.5e-5]);
xlabel('Time (s)', 'FontSize', 14);
ylabel('Voltage (V)', 'FontSize', 14);
title('Computation Time For All Effect (Optimization 2)', 'FontSize', 16);
grid on;

saveFolder = fullfile(pwd,'..','..','gitfigures/ImplementationTest/');
filePath = fullfile(saveFolder, sprintf('%s.png','ComputationTime_O2ALLFx'));
exportgraphics(figH, filePath, 'Resolution', 300);
%filePath = fullfile(saveFolder, sprintf('%s.png','ComputationTime_noFx'));
%exportgraphics(figJ, filePath, 'Resolution', 300);