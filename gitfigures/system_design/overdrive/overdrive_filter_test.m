files = {'FIR_AA_measurement.csv','IIR_HP_200_measurement.csv','IIR_HP_500_measurement.csv','IIR_HP_3000_measurement.csv','IIR_LP_4000_measurement.csv','IIR_LP_7000_measurement.csv','IIR_LP_10000_measurement.csv'};
data = cellfun(@readtable, files, 'UniformOutput', false);

folder_path = pwd;
fig_if = figure;
fig_elseif = figure;
fig_else = figure;
for i = 1:length(data)
    if i == 1
        figure(fig_if); 
        plot(data{i}.Frequency_Hz_*1E-3,data{i}.Channel2Magnitude_dB_);
        xlabel('Frequency [kHz]');
        ylabel('Magnitude [dB]');
        title('Anti-Aliasing Frequency Response');
        hold on;
        grid on;
        [M, I] = max(data{i}.Channel2Magnitude_dB_);
        yline(M-3,'--');
        xlim([0 22]);
    elseif i <= 4
        figure(fig_elseif);
        semilogx(data{i}.Frequency_Hz_*1E-3,data{i}.Channel2Magnitude_dB_);
        xlabel('Frequency [kHz]');
        ylabel('Magnitude [dB]');
        title('Frequency Response IIR High Pass');
        hold on;
        grid on;
        [M, I] = max(data{i}.Channel2Magnitude_dB_);
        yline(M-3,'--');
        xlim([0 22]);
    else
        figure(fig_else);
        semilogx(data{i}.Frequency_Hz_*1E-3,data{i}.Channel2Magnitude_dB_);
        xlabel('Frequency [kHz]');
        ylabel('Magnitude [dB]');
        title('Frequency Response IIR Low Pass');
        hold on;
        grid on;
        [M, I] = max(data{i}.Channel2Magnitude_dB_);
        yline(M-3,'--');
        xlim([0 22]);
    end
end

saveFolder = fullfile(pwd);
filePath = fullfile(saveFolder, sprintf('%s.png','overdrive_aa_filter_test'));
exportgraphics(fig_if, filePath, 'Resolution', 300);
filePath = fullfile(saveFolder, sprintf('%s.png','overdrive_hp_filter_test'));
exportgraphics(fig_elseif, filePath, 'Resolution', 300);
filePath = fullfile(saveFolder, sprintf('%s.png','overdrive_lp_filter_test'));
exportgraphics(fig_else, filePath, 'Resolution', 300);
