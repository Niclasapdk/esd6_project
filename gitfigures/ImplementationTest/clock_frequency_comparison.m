close all
clear

files = {"acope_0wy12.csv","acope_0wy13.csv"};

data = cellfun(@readtable, files, 'UniformOutput', false);
figH = figure;
s = plot(1E6*data{1}.Var1,data{1}.Var2);
[pks,locs] = findpeaks(data{1}.Var2,2e9,'NPeaks',2,'MinPeakHeight',3.5);

ylabel('Amplitude [V]');
xlabel('Time [\mus]','Interpreter','tex');

legend(s,{['Clock Frequency: ' num2str(1E-6 * 1/(locs(2,:)-locs(1,:))) 'Mhz']});
grid on;

figD = figure;
s = plot(1E6*data{2}.Var1,data{2}.Var2);
legend(s,'Clock Frequency: 100Mhz');

ylabel('Amplitude [V]');
xlabel('Time [\mus]','Interpreter','tex');
grid on;


saveFolder = fullfile(pwd);
filePath = fullfile(saveFolder, sprintf('%s.png','12.2Mhz_clock_plot'));
exportgraphics(figH, filePath, 'Resolution', 300);
filePath = fullfile(saveFolder, sprintf('%s.png','100Mhz_clock_plot'));
exportgraphics(figD, filePath, 'Resolution', 300);