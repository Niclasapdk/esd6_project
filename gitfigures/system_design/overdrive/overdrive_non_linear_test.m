clc; close all; clear;

% Read data
data = readmatrix('overdrive_non_linear_test.csv');
time = data(:, 1);
input = data(:, 2);
output = data(:, 3);

% Step 1: Find intersection point (where input ≈ output)
diff_signal = abs(input - output);
[~, idx_intersect] = min(diff_signal);   % Index of minimum difference
t_intersect = time(idx_intersect);       % Time at intersection

% Step 2: Shift time so that intersection point is at t = 0
time_shifted = time - t_intersect;

% Max values
max_vol1 = max(input);
max_vol2 = max(output);

% Plot
figH = figure;
fig_1 = plot(time_shifted, input, 'b', 'LineWidth', 1);
hold on;
fig_2 = plot(time_shifted, output, 'r', 'LineWidth', 1);
xlim([-0.00025+4.3972e-6 0.00025+4.3972e-6]);
i = yline(max_vol1, '--', 'LineWidth', 1.5, 'Color', 'blue');
o = yline(max_vol2, '--', 'LineWidth', 1.5, 'Color', 'r');
xline(0, 'LineWidth', 1.5, ...
    'Label', '0', ...
    'LabelOrientation','horizontal', ...
    'LabelHorizontalAlignment', 'right', ...
    'LabelVerticalAlignment', 'top', 'FontSize', 12);

xline(0.00025 * 1/3, 'LineWidth', 1.5, ...
    'Label', '1/3', ...
    'LabelOrientation','horizontal', ...
    'LabelHorizontalAlignment', 'right', ...
    'LabelVerticalAlignment', 'top', 'FontSize', 12);

xline(0.00025 * 2/3, 'LineWidth', 1.5, ...
    'Label', '2/3', ...
    'LabelOrientation','horizontal', ...
    'LabelHorizontalAlignment', 'right', ...
    'LabelVerticalAlignment', 'top', 'FontSize', 12);


xlabel('Time (s)', 'FontSize', 14);
ylabel('Voltage (V)', 'FontSize', 14);
title('Test of Overdrive Nonlinearity', 'FontSize', 16);

legend([fig_1, fig_2, i, o], ...
    'Input', ...
    'Output', ...
    ['Max Input Voltage = ' num2str(max_vol1, '%.3f') ' V'], ...
    ['Max Output Voltage = ' num2str(max_vol2, '%.4f') ' V'], ...
    'Location', 'southwest', 'Fontsize', 10);
grid on;

saveFolder = fullfile(pwd);
filePath = fullfile(saveFolder, sprintf('%s.png','overdrive_nonlinearity_test_fig1'));
exportgraphics(figH, filePath, 'Resolution', 300);

% figH = figure;
% title('Test of Overdrive Nonlinearity');
% plot(data.Time_s_,data.Channel1_V_);
% hold on
% plot(data.Time_s_,data.Channel2_V_);
% xlim([-1e-3 1e-3]);
% grid on
% legend('Input Signal','Output Signal');
% ylabel('Amplitude [V]');
% xlabel('Time [s]');
% 
% saveFolder = fullfile(pwd);
% filePath = fullfile(saveFolder, sprintf('%s.png','overdrive_nonlinearity_test_fig'));
% exportgraphics(figH, filePath, 'Resolution', 300);