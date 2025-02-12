close all
clear
folderPath = pwd;
files = dir(fullfile(folderPath, '*.csv'));
saveFolder = fullfile(pwd,'..','..','gitfigures/guitar_measurement_fig/');
data = cell(length(files), 1);
names = {'Bridge 10','Bridge 5', 'Middle 10','Neck 10','Neck 5'};
for i = 1:length(files)
    filePath = fullfile(folderPath, files(i).name);
    data{i} = readtable(filePath);
    
end

figH = figure;
for k = 1:length(files)
    title('Impedance measurement of the guitar');
    semilogx(data{k}{:,1}/1E3,data{k}{:,3}/1E3)
    xlabel('Frequency [kHz]');
    ylabel('Impedance |Z|[k\Omega]');
    grid on
    xlim([0 18.6])
    legend(names,'Location','northwest')
    hold on
end
filePath = fullfile(saveFolder, sprintf('%s.png', 'guitar_impedance_measurement'));
exportgraphics(figH, filePath, 'Resolution', 300);