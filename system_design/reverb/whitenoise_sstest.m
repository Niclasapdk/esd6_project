clc, clear, close all;

% Resolution
fs = 44100;
N = fs*2;%value in seconds
duration_white_noise = 0.5; % White noise duration in seconds
samples_white_noise = duration_white_noise * fs;

% Create input signal (white noise for 1 second, then silence)
input_signal = zeros(N,1);
%input_signal(1:samples_white_noise) = 2 * rand(samples_white_noise, 1) - 1; % White noise in [-1,1]
%input_signal(1:samples_white_noise) = randn(samples_white_noise, 1); % White noise in [-1,1]
input_signal(1:samples_white_noise) = wgn(samples_white_noise,1,0);

% Output vector (Impulse Response)
ir = zeros(1,N);

% Filter parameters
gain = 1; 
reverbtime = 1;

% Apply reverb
ir = reverb(input_signal, fs, gain, reverbtime);
%ir = reverb_with_input_delayline(input_signal, fs, gain, reverbtime);

% Time vector in seconds
t = (0:N-1) / fs;

% Convert input and output to dB
input_dB = 20 * log10(abs(input_signal) + eps);
output_dB = 20 * log10(abs(ir) + eps); % Avoid log(0)

% Plot both input and output in dB
figure;
plot(t, output_dB, 'k', 'DisplayName', 'Reverb Response'); hold on;
plot(t, input_dB, 'b', 'DisplayName', 'Input (White Noise → Silence)');

% Set x-axis tick interval (e.g., every 0.5 seconds)
xticks(0:0.5:max(t));

% Labels and grid
xlabel('Time (seconds)');
ylabel('Magnitude (dB)');
title('Input (White Noise → Silence) and Reverb Response');
grid on;

% Mark when the white noise stops
xline(duration_white_noise, '--r', 'White Noise Stops', 'LabelVerticalAlignment', 'middle', 'HandleVisibility', 'off');
yline(-5, '--r', '-5dB', 'HandleVisibility', 'off');
yline(-65, '--r', '-65dB', 'HandleVisibility', 'off');




legend('Location', 'SouthWest');
