close all
clear

folderPath = pwd;
x = readtable('wah_fc_1000hz_50mV_test.csv');
x = renamevars(x,["Channel1Magnitude_dB_","Frequency_Hz_"],["mag","freq"]);
y = readtable('wah_fc_1000hz_500mV_overflow_test.csv');
y = renamevars(y,["Channel1Magnitude_dB_","Frequency_Hz_"],["mag","freq"]);

figH = figure;
[maxval idx] = max(x.mag);

string = {['Fc = ' num2str(x.freq(idx)),'Hz'], ['Max Magnitude = ',num2str(maxval),'dB']};

p = semilogx(x.freq,x.mag);
hold on
xline(x.freq(idx), 'r--', string, ...
    'LabelOrientation', 'horizontal', ...
    'LabelVerticalAlignment', 'bottom', ...
    'FontWeight', 'bold');
hold on
p1 = semilogx(y.freq,y.mag);

title('Wah Frequency Response With 1kHz Center Frequency');

ylabel('Magnitude [dB]');
xlabel('Frequency [Hz]');

legend([p p1],{['Input Signal Amplitude 50mV'],['Input Signal Amplitude 500mV']},'Location','northwest');
xlim([20 20e3]);
grid on

saveFolder = fullfile(pwd,'..','..','gitfigures/system_design/Wah/');
filePath = fullfile(saveFolder, sprintf('%s.png', 'wah_fc_test_500mv_50mv_noshift'));
exportgraphics(figH, filePath, 'Resolution', 300);