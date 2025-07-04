close all;
clear;
clc;

% --- Load impulse response data ---
data = readtable('scope_8_05s.csv');
%data = readtable('scope_5_4s.csv');
t = data.xaxis;                      % Time vector (seconds)
h = data.ch2;                        % Impulse response


% --- Schroeder backward integration ---
h_squared = abs(h).^2;
energy_decay = flip(cumsum(flip(h_squared)));
energy_decay = energy_decay / max(energy_decay);  % Normalize
decay_dB = 10 * log10(energy_decay + eps);        % Add eps to avoid log(0)

% --- Define analysis range (-5 dB to -30 dB) ---
peak_dB = max(decay_dB);
start_dB = peak_dB - 5;              % -5 dB start point
end_dB = peak_dB - 25;               % -30 dB end point (your noise floor)

% Find first continuous decay region
valid_idx = find(decay_dB <= start_dB & decay_dB >= end_dB);
start_idx = valid_idx(1);
end_idx = valid_idx(end);

% Extract linear region
t_fit = t(start_idx:end_idx);
decay_fit = decay_dB(start_idx:end_idx);

% --- Linear regression ---
[p, ~] = polyfit(t_fit, decay_fit, 1);
slope = p(1);                        % dB/s

% --- RT calculation ---
RT25 = (start_dB - end_dB)/abs(slope);  % Actual measured decay time
RT60 = RT25 * (60/20);                  % Extrapolate to 60 dB

% --- Plotting ---
fig05s = figure;
s2 = plot(t, decay_dB, 'b-', 'LineWidth', 1.5, 'DisplayName', 'Schroeder Decay');
hold on;
s1 = plot(t_fit, polyval(p, t_fit), 'r--', 'LineWidth', 2, 'DisplayName', 'Linear Fit');
yline(start_dB, '--', '-5 dB', 'Color', 'k');
yline(end_dB, '--', '-25 dB', 'Color', 'k');
xline(0.5825, '--', 'Start regression 0.5825s', 'Color', 'k', 'LabelVerticalAlignment', 'bottom');
xline(0.92, '--', 'End regression 0.92s', 'Color', 'k', 'LabelVerticalAlignment', 'middle');
title(sprintf('RT20 = %.2fs, RT60 = %.2fs', RT25, RT60));
xlabel('Time (s)', FontSize=14);
ylabel('Level (dB)', FontSize=14);
legend([s2, s1], {'Schroeder integration', 'Linear regression'},'Location', 'northeast', FontSize=12);
grid on;
set(gca, 'YLim', [end_dB-5 peak_dB+2]);

saveFolder = fullfile(pwd,'..','..','..','gitfigures/system_design/reverb/');
filePath = fullfile(saveFolder, sprintf('%s.png','reverb_schroeder_rt60_05s'));
exportgraphics(fig05s(1), filePath, 'Resolution', 300);