close all
clear

folderpath = pwd;


filenames = {'flanger_delay_0ms_test.csv', 'flanger_delay_15ms_test.csv'};
files = cellfun(@readtable, filenames, 'UniformOutput', false);
figH = figure;
p = plot(files{1}.Time_s_ ,files{1}.Channel1_V_);
hold on
p1 = plot(files{1}.Time_s_ ,files{1}.Channel2_V_);
grid on
title("Flanger Delay Test 0ms");
legend;
xlim([-0.05 0.05]);
legend('Flanger delayed signal','Input signal');
xlabel('Time [s]');
ylabel('Amplitude [V]')
hold off;

figJ = figure;
threshold = prctile(files{2}.Channel1_V_, 0.15);
lowValIdx = find(files{2}.Channel1_V_ < threshold);

p2 = plot(files{2}.Time_s_ ,files{2}.Channel1_V_);
minIdx = find(islocalmin(files{2}.Channel1_V_));
hold on

p3 = plot(files{2}.Time_s_ ,files{2}.Channel2_V_);
hold on

xline(files{2}.Time_s_([2886 4083]),'r--');
hold on

x_norm = rescale(files{2}.Time_s_,0,1);
y_norm = rescale(files{2}.Channel1_V_,0,1);

time_difference = (files{2}.Time_s_(4083) - files{2}.Time_s_(2886))*1E3;

str = {['Time:'],[num2str(time_difference,'%.2f'),'ms']};
annotation('doublearrow',[x_norm(3300),x_norm(4200)], [0.6,0.6]);

text(files{2}.Time_s_(3000),0.45,str);

hold on


plot(files{2}.Time_s_(10),files{2}.Channel1_V_(10),'ro');
grid on

title("Flanger Delay Test 15ms");
xlim([-0.05 0.05]);
legend('Flanger delayed signal','Input signal');
xlabel('Time [s]');
ylabel('Amplitude [V]')
hold off;


saveFolder = fullfile(pwd,'..','..','gitfigures/system_design/ChorusFlanger/');
filePath = fullfile(saveFolder, sprintf('%s.png','flanger_delay_0ms_test'));
exportgraphics(figH, filePath, 'Resolution', 300);
filePath = fullfile(saveFolder, sprintf('%s.png','flanger_delay_15ms_test'));
exportgraphics(figJ, filePath, 'Resolution', 300);