close all
clear
folderPath = pwd;
files = dir(fullfile(folderPath, '*.csv'));
saveFolder = fullfile(pwd,'..','gitfigures/guitar_measurement_fig/');
data = cell(length(files), 1);
names = {'Bridge Hard','Bridge Normal', 'Middle Hard','Middle Normal','Neck Hard','Neck Middle'};
for i = 1:length(files)
    filePath = fullfile(folderPath, files(i).name);
    data{i} = readtable(filePath);
    
end
for k = 1:length(files)
    figH = figure;
    plot(data{k}{:,1},data{k}{:,2})
    ylabel('Amplitude [V]');
    xlabel('Time [s]');
    grid on
    xlim([0 0.1])
    legend(names(k))
    clean = strrep(names{k}, ' ', '_');
    filePath = fullfile(saveFolder, sprintf('%s.png', clean));
    exportgraphics(figH, filePath, 'Resolution', 300);
end