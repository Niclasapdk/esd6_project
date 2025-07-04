close all
clear
folderPath = pwd;
% List of CSV filenames
files = {'wah_low_frequency_12db.csv','wah_mid_frequency_12db.csv','wah_high_frequency_12db.csv'};
figH = figure;
hold on;
set(gca, 'XScale', 'log');
for i = 1:length(files)
    % Read CSV file
    data = readtable(files{i});  % or use readtable if CSV has headers

    % Extract x and y from your CSV structure
    x = data(:,1);   % assuming first column is x
    y = data(:,3);   % assuming second column is y
    [maxval idx] = max(y.Channel2Magnitude_dB_);
    

    string = ['Frequency: ', num2str(x.Frequency_Hz_(idx),'%.1f'),'Hz'];
    xline(x.Frequency_Hz_(idx), 'r--', string, ...
    'LabelOrientation', 'aligned', ...
    'LabelVerticalAlignment', 'bottom', ...
    'FontWeight', 'bold');
    
    p(i) = plot(x.Frequency_Hz_, y.Channel2Magnitude_dB_);
    title('Wah Frequency Response With Varying Center Frequency');
    xlabel('Frequency [Hz]');
    ylabel('Magnitude [dB]');
    grid on;
end
xlim([20 20e3]);
saveFolder = fullfile(pwd,'..','..','gitfigures/system_design/Wah/');
filePath = fullfile(saveFolder, sprintf('%s.png', 'wah_fc_sweep_test'));
exportgraphics(figH, filePath, 'Resolution', 300);