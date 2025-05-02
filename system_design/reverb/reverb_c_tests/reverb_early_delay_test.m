close all
clear

data = readtable('reverb_early_delay_impulse.csv');

TF2 = islocalmin(data.Channel1_V_,'MinProminence',0.1);

figH = figure;
time = round(data.Time_s_(TF2),5);
time_diff = time(2:end,1) - time(1,1);
strings = "$\Delta$"+string(time*1E3)+"ms";
time_strings = ["Input Impulse";"Output Response";"$\Delta$"+string(time_diff*1E3)+'ms';];
p = plot(data.Time_s_,data.Channel2_V_,data.Time_s_,data.Channel1_V_);
p(1).Color = [114/255, 135/255, 253/255];
xH = xline(time,'r--');
hold on

for k = 2:length(time)
     ypart = 0.075*k*ones(length(time),2);  
     meow(k) = line([time(1) time(k)],ypart(k,:));
end

lgd = legend([p(:);meow(2:end).'],time_strings(:),'Interpreter','latex','Location','southoutside','Orientation','horizontal','NumColumns',4);
lgd.FontSize = 10;
xlim([-0.02 0.1]);
title('Early Delay Impulse Reponse');
xlabel('Time [s]');
ylabel('Magnitude [V]');
grid on;

saveFolder = fullfile(pwd,'..','..','..','gitfigures/system_design/reverb/');
filePath = fullfile(saveFolder, sprintf('%s.png','reverb_early_delay_test'));
exportgraphics(figH(1), filePath, 'Resolution', 300);