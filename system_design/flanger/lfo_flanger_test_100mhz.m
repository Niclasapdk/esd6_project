close all
clear

folderPath = pwd;
files = dir(fullfile(folderPath, 'lfo_flanger_100mhz_test.txt'));
data = cell(length(files), 1);
filePath = fullfile(folderPath, files.name);

x = readtable(filePath);
v = (1:length(x.Var1))/4410; %44100/10 = 4410 :))

Fs = 4410;
N = length(x.Var1);
X = fft(x.Var1);
f = (0:N-1)*(Fs/N);
mag = abs(X);
[~, idx] = max(mag);    % Index of max magnitude
dominant_freq = f(idx); % Corresponding frequency
[peak_vals, peak_locs] = findpeaks(x.Var1,v);

figH = figure;
p = plot(v,x.Var1/32766);
frequency = 1/peak_locs;
title(['LFO frequency ',num2str(frequency),'Hz']);
hold on
p1 = plot(peak_locs, peak_vals/32766,'o');  % Then this
xlabel('Time [s]');
ylabel('Normalized Amplitude Q15');
hold on
legend(p1, ['Time period = ',num2str(peak_locs),'s'],'Location','southwest');
xlim([0 11]);
ylim([-1.1 1.1]);
grid on

saveFolder = fullfile(pwd,'..','..','gitfigures/system_design/ChorusFlanger');
filePath = fullfile(saveFolder, sprintf('%s.png', 'flanger_lfo_test_100mHz'));
exportgraphics(figH, filePath, 'Resolution', 300);