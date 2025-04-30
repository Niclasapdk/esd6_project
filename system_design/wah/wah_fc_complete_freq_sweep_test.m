close all
clear
folderPath = pwd;
% List of CSV filenames
files = {'wah_fc_350hz_700mV_rightshift_test.csv','wah_fc_1000hz_700mV_rightshift_test','wah_fc_1500hz_700mV_rightshift_test','wah_fc_2250hz_700mV_rightshift_test'};
figH = figure;
hold on;
set(gca, 'XScale', 'log');
for i = 1:length(files)
    % Read CSV file
    data = readtable(files{i});  % or use readtable if CSV has headers

    % Extract x and y from your CSV structure
    x = data(:,1);   % assuming first column is x
    y = data(:,2);   % assuming second column is y
    [maxval idx] = max(y.Channel1Magnitude_dB_);
    

    string = ['Frequency: ', num2str(x.Frequency_Hz_(idx),'%.1f'),'Hz'];
    xline(x.Frequency_Hz_(idx), 'r--', string, ...
    'LabelOrientation', 'aligned', ...
    'LabelVerticalAlignment', 'bottom', ...
    'FontWeight', 'bold');
    
    p(i) = plot(x.Frequency_Hz_, y.Channel1Magnitude_dB_);
    title('Wah Frequency Response With Varying Center Frequency');
    xlabel('Frequency [Hz]');
    ylabel('Magnitude [dB]');
    grid on;
end
xlim([20 20e3]);
saveFolder = fullfile(pwd,'..','..','gitfigures/system_design/Wah/');
filePath = fullfile(saveFolder, sprintf('%s.png', 'wah_fc_sweep_test'));
exportgraphics(figH, filePath, 'Resolution', 300);