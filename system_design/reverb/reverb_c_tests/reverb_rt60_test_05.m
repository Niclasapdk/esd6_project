close all
clear
clc

data = readtable('scope_8_05s.csv');
%data_noise = readtable('scope_6_noisefloor_w_reverb.csv');

ch1_dB = 20 * log10(abs(data.ch1) + eps);
ch2_dB = 20 * log10(abs(data.ch2) + eps); % Avoid log(0)
%noise_dB = 20 * log10(abs(data_noise.ch2) + eps);

MaxVAL=max(ch2_dB);

x_min = 0;
x_max = 0.9;

% Logical index for x range
idx = data.xaxis >= x_min & data.xaxis <= x_max;

figb = figure;
s2 = plot(data.xaxis,ch2_dB, 'LineWidth',1);
hold on;
s1 = plot(data.xaxis(idx),ch1_dB(idx),':');
%hold on;
%plot(data.xaxis,noise_dB,'-','LineWidth',3);
title('RT20 Test For 0.5s Reverb Time', FontSize=14);
xlim([0.5 1.5]);
ylim([-40 5]);

xline(0.817, '--', '0.817s', 'LineWidth', 1.5, 'Color', 'r');
xline(0.905, '--', '0.905s', 'LineWidth', 1.5, 'Color', 'r');

yline(MaxVAL-5,'--', '-5dB', 'LineWidth',1.5,'color','black');
yline(MaxVAL-25,'--', '-25dB', 'LineWidth',1.5,'color','black');

legend([s2, s1], {'Output', 'Input'}, 'Location', 'SouthWest', FontSize=12);
xlabel('Time (seconds)', FontSize=14);
ylabel('Magnitude (dB)', FontSize=14);

saveFolder = fullfile(pwd,'..','..','..','gitfigures/system_design/reverb/');
filePath = fullfile(saveFolder, sprintf('%s.png','reverb_rt60_test_0.5'));
exportgraphics(figb(1), filePath, 'Resolution', 300);