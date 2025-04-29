close all
clear

folderPath = pwd;
files = dir(fullfile(folderPath, 'lfo_tremolo_10hz_test.txt'));
data = cell(length(files), 1);
filePath = fullfile(folderPath, files.name);

x = readtable(filePath);
v = (1:length(x.Var1))/4410; %44100/10 = 4410 :))
[peak_vals, peak_locs] = findpeaks(x.Var1,v);

figH = figure;
p = plot(v,x.Var1/32766);
frequency = 1/peak_locs(1,1)
title(['LFO frequency ',num2str(frequency),'Hz']);
hold on
p1 = plot(peak_locs(1,1), peak_vals/32766,'o');  % Then this
xlabel('Time [s]');
ylabel('Normalized Amplitude Q15');
hold on
legend(p1, ['Time period = ',num2str(peak_locs(1,1)),'s'],'Location','southwest');
xlim([0 0.2]);
ylim([-1.1 1.1]);
grid on

saveFolder = fullfile(pwd,'..','..','gitfigures/system_design/Tremolo');
filePath = fullfile(saveFolder, sprintf('%s.png', 'tremolo_lfo_test_10Hz'));
exportgraphics(figH, filePath, 'Resolution', 300);