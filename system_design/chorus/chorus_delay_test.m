close all
clear

folder_path = pwd;
files = {'chorus_delay_10ms_test.csv','chorus_delay_25ms_test.csv'};
delay_len = [10 25];
data = cellfun(@readtable, files, 'UniformOutput', false);
for i = 1:length(data)
    figH(i) = figure(i);
    p = plot(data{i}.Time_s_,data{i}.Channel2_V_,data{i}.Time_s_,data{i}.Channel1_V_);
    grid on
    xlabel('Time [s]');
    ylabel('Magnitude [V]');
    xlim([0.98 1.03]);
    title(['Chorus Delay of Max ',num2str(delay_len(i)),'ms']);
    TF2 = islocalmin(data{i}.Channel1_V_,'MinProminence',0.1);
    test = data{i}.Time_s_(TF2);
    result = zeros(3,2);
    result(:,1) = test(1);
    result(1:end, 2) = test(2:4);
    time = (result(:,2) - result(1,1))*1E3;
    strings = string(test)+"s";
    p1 = xline(data{i}.Time_s_(TF2),'r--',strings);
    hold on
    for k = 1:length(result)
        ypart = 0.06*(k)*ones(3,2);
        meow(k) = line(result(k,:),ypart(k,:));
    end
    hold off
    legend([p(:);meow(:)],'Input Impulse', ...
        'Output Impulse', ...
        ['\Delta',num2str(time(1)),'ms'], ...
        ['\Delta',num2str(time(2)),'ms'], ...
        ['\Delta',num2str(time(3)),'ms'],'Location','northwest');
end

saveFolder = fullfile(pwd,'..','..','gitfigures/system_design/ChorusFlanger/');
filePath = fullfile(saveFolder, sprintf('%s.png','chorus_delay_10_ms_test'));
exportgraphics(figH(1), filePath, 'Resolution', 300);
filePath = fullfile(saveFolder, sprintf('%s.png','chorus_delay_25_ms_test'));
exportgraphics(figH(2), filePath, 'Resolution', 300);