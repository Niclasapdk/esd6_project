close all
clear

folder_path = pwd;
files = {'tremolo_depth_0.5_test.csv','tremolo_depth_1_test.csv'};
depth = [0.5 1];
data = cellfun(@readtable, files, 'UniformOutput', false);
for i = 1:length(data)
    figH(i) = figure(i);
    p = plot(data{i}.Time_s_,data{i}.Channel1_V_);
    grid on
    xlabel('Time [s]');
    ylabel('Magnitude [V]');
    title(['Tremolo Depth ',num2str(depth(i))]);
    xlim([0.2 0.65])
end


saveFolder = fullfile(pwd,'..','..','gitfigures/system_design/Tremolo/');
filePath = fullfile(saveFolder, sprintf('%s.png','tremolo_depth_0.5_test'));
exportgraphics(figH(1), filePath, 'Resolution', 300);
filePath = fullfile(saveFolder, sprintf('%s.png','tremolo_depth_1_test'));
exportgraphics(figH(2), filePath, 'Resolution', 300);