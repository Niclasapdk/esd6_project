close all;
clear
folderPath = pwd;
files = dir(fullfile(folderPath, '*.txt'));
data = cell(length(files), 1);
filePath = fullfile(folderPath, files.name);
data = readtable(filePath);
figH = figure;
yyaxis left
s = semilogx(data.Var1*1E-3, data.Var2);
xlabel('Frequency [kHz]','FontSize',12);
ylabel('Magnitude [dB]','FontSize',12);
s(1).LineWidth = 1.8;
hold on
p = semilogx(22e3*1E-3,data.Var2(1,1)-3,'o-','MarkerFaceColor','red')
hold on
p1 = semilogx(1e3*1E-3,data.Var2(1,1),'o-','MarkerFaceColor','green')
hold on
yyaxis right
s1 = semilogx(data.Var1*1E-3, data.Var3)
ylabel('Phase [deg]','FontSize',12);
s1(1).LineWidth = 1.8;
grid on;
hold on
legend([p1 p],{'-7.85dB' '-10.85dB'},'Location','southwest');
title('Frequency Response')
saveFolder = fullfile(pwd,'..','..','gitfigures/system_design/analog_frontend');
filePath = fullfile(saveFolder, sprintf('%s.png', 'freqresp_frontend'));
exportgraphics(figH, filePath, 'Resolution', 300);