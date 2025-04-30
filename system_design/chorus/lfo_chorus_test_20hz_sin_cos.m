close all
clear
% 
folderPath = pwd;
files = dir(fullfile(folderPath, 'lfo_chorus_20hz_cos_sin_test.txt'));
data = cell(length(files), 1);
filePath = fullfile(folderPath, files.name);

x = readtable(filePath);

v = (1:length(x.Var1))/4410; %44100/10 = 4410 :))
[peak_vals, peak_locs] = findpeaks(x.Var1,v);
[peak_vals1, peak_locs1] = findpeaks(x.Var2,v);

figH = figure;
p = plot(v,x.Var1/32766);
hold on
p1 = plot(v,x.Var2/32766);
frequency = 1/peak_locs(1,1);
frequency1 = 1/(peak_locs1(3)-peak_locs1(2));
title(['Chorus LFO frequency sine and cosine ',num2str(frequency),'Hz']);
hold on
p2 = plot(peak_locs(1,1), peak_vals/32766,'o');  % Then this
p3 = plot(peak_locs1(1,1), peak_vals1/32766,'o');
xlabel('Time [s]');
ylabel('Normalized Amplitude Q15');
hold on
legend([p,p1,p3(1),p2(3)], ...
    {['Sine'],['Cosine'],['Time period = ', num2str(peak_locs1(1,1)), 's'], ...
     ['Time period = ', num2str(peak_locs(1,1)), 's']}, ...
    'Location', 'southwest');
xlim([0 0.1]);
ylim([-1.1 1.1]);
phase = (peak_locs(1)-peak_locs1(1))*20*360;
annotation('textbox', [0.78, 0.148, 0.12, 0.15], 'String', {"Phase ", "difference:",num2str(phase)+"$^\circ$"},'Interpreter','latex');% trust me bro ;)
grid on

saveFolder = fullfile(pwd,'..','..','gitfigures/system_design/ChorusFlanger');
filePath = fullfile(saveFolder, sprintf('%s.png', 'chorus_lfo_test_20Hz_sin_cos'));
exportgraphics(figH, filePath, 'Resolution', 300);