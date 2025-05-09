close all
clear

file_path = pwd;
file = {'output_impedance.csv'};

data = cellfun(@readtable, file, 'UniformOutput', false);
mag = db2mag(data{1}.Channel2Magnitude_dB_);
res = 1e3;
zd_abs = ((res-(mag*res))/(mag));
x = zd_abs(:,118);

figH = figure;
semilogx(data{1}.Frequency_Hz_,x*1e-3,'LineWidth',2);
title('Output Impedance Sweep for Output Driver');
xlabel('Frequency [Hz]');
ylabel('|Z| [k\Omega]');
xlim([0 20e3]);
grid on

saveFolder = fullfile(pwd);
filePath = fullfile(saveFolder, sprintf('%s.png','output_impedance_test_driver'));
exportgraphics(figH(1), filePath, 'Resolution', 300);