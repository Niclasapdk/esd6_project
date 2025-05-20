% Parameters
f = 1;              % Frequency in Hz
fs = 44100;         % Sampling frequency
duration = 2;       % seconds
N = fs * duration;  % Number of samples

% Time and angle vectors
t = (0:N-1)/fs;
theta = 2 * pi * f * t;

% True sine wave
y_sin = sin(theta);

% === Custom Polynomial Approximation ===
x_norm = mod(theta+pi, 2*pi); % Wrap angle
x_norm = (x_norm - pi) / pi;  % Normalize [-1, 1]

% Coefficients
c1 = 3.140625;
c2 = 0.02026367;
c3 = -5.3251;
c4 = 0.5446778;
c5 = 1.800293;

y_custom_scaled = c1*x_norm + c2*x_norm.^2 + c3*x_norm.^3 + ...
                  c4*x_norm.^4 + c5*x_norm.^5;

% === Matrix-Based Approximation ===
k = (2 * pi * f) / fs;

% Standard matrix oscillator
x1 = 0; x2 = 1;
y1 = zeros(1, N);

% Gain-corrected version
x1_corr = 1; x2_corr = 0;
y_corr = zeros(1, N);

for i = 1:N
    % Basic matrix oscillator
    y1(i) = (1 - k^2/2) * x1 + k * x2;
    x2_new = -k * x1 + (1 - k^2/2) * x2;
    x1 = y1(i);
    x2 = x2_new;
    
    % Gain-corrected oscillator
    y1_corr = (1 - k^2/2) * x1_corr + k * x2_corr;
    y2_corr = -k * x1_corr + (1 - k^2/2) * x2_corr;
    
    % Power & gain
    P = y1_corr^2 + y2_corr^2;
    G = 1.5 - P;
    
    x1_corr = G * y1_corr;
    x2_corr = G * y2_corr;
    
    y_corr(i) = y1_corr;
end

% === Plot All Approximations ===
plot_range = 1:round(fs/f/4);  % One cycle for clarity

figH=figure;
hold on;
plot(theta(plot_range), y_sin(plot_range), 'k-', 'LineWidth', 2);
plot(theta(plot_range), y_custom_scaled(plot_range), 'b-.', 'LineWidth', 4);
plot(theta(plot_range), y1(plot_range), 'g:', 'LineWidth', 3);

xlabel('\theta (radians)','FontSize',14);
ylabel('Amplitude','FontSize',14);
title('Sine Approximations via Matrix Oscillator','FontSize',16);
legend('True sin(x)', 'Custom Approximation', ...
       'Matrix-Based', 'FontSize', 14,'Location', 'southeast');
grid on;
hold off;

saveFolder = fullfile(pwd,'..','..','gitfigures/system_design/Wah/');
filePath = fullfile(saveFolder, sprintf('%s.png','SinVsTaylorVsMatrix'));
exportgraphics(figH, filePath, 'Resolution', 300);
%filePath = fullfile(saveFolder, sprintf('%s.png','flanger_delay_15ms_test'));
%exportgraphics(figJ, filePath, 'Resolution', 300);
