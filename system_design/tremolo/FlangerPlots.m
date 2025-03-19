%{
fs = 44100; % Sampling rate
t = 0:1/fs:1; % 1 second time vector
f = 100; % Frequency of the sine wave
input_signal = sin(2 * pi * f * t); % Generate sine wave

% Flanger parameters
mix = 0.5;       % Mix ratio (wet/dry)
delay = 2;       % Base delay in ms
width = 2;       % Modulation width in ms
rate = 2;     % LFO frequency in Hz

% Apply flanger effect
output_signal = flanger(input_signal, mix, delay, width, rate, fs);

% Generate the LFO for visualization
fsn = fs * 4; % Upsampled frequency
n = (1:length(input_signal) * 4)'; % Sample index for upsampled signal
LFO = sin(2 * pi * rate / fsn * n); % LFO modulation

% Time vectors
t_lfo = (0:length(LFO)-1) / fsn * 1000; % LFO time in ms
t_out = (0:length(output_signal)-1) / fs * 1000; % Output time in ms

% Plot results
figure;
subplot(3,1,1);
plot(t*1000, input_signal);
title('Input Signal', 'FontSize', 14);
xlabel('Time (ms)', 'FontSize', 12);
ylabel('Amplitude', 'FontSize', 12);
grid on;

subplot(3,1,2);
plot(t_lfo, LFO);
title('LFO Modulation (Flanger Delay)', 'FontSize', 14);
xlabel('Time (ms)', 'FontSize', 12);
ylabel('LFO Amplitude', 'FontSize', 12);
grid on;

subplot(3,1,3);
plot(t_out, output_signal);
title('Output Signal (Flanged)', 'FontSize', 14);
xlabel('Time (ms)', 'FontSize', 12);
ylabel('Amplitude', 'FontSize', 12);
grid on;
%}
% MATLAB code to generate frequency response of a flanger effect

% Parameters
fs = 44100; % Sampling frequency (Hz)
duration = 1; % Duration of the signal in seconds
f = 1000; % Frequency of a test signal (Hz)
delay_min = 0.001; % Minimum delay (1 ms)
delay_max = 0.005; % Maximum delay (5 ms)
modulation_rate = 0.25; % Rate of delay modulation (Hz)
depth = 1; % Modulation depth (fraction of maximum delay)
t = 0:1/fs:duration-1/fs; % Time vector

% Generate a test signal (sine wave)
x = sin(2*pi*f*t);

% Modulate the delay time
modulated_delay = delay_min + (depth * (delay_max - delay_min) * (sin(2*pi*modulation_rate*t) + 1) / 2);

% Initialize the output signal (flanger effect)
y = zeros(size(x));

% Apply flanger effect
for i = 1:length(x)
    delay_samples = round(modulated_delay(i) * fs); % Delay in samples
    if i - delay_samples > 0
        y(i) = x(i) + x(i - delay_samples); % Mix original signal with delayed signal
    else
        y(i) = x(i); % If there's no valid previous sample, just pass the original signal
    end
end

% Calculate the frequency response of the flanger
[H, f_response] = freqz(y, 1, 1024, fs);

% Plot the frequency response
figure;
plot(f_response, 20*log10(abs(H)));
xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
title('Frequency Response of a Flanger');
grid on;
xlim([0 fs/2]); % Focus on the audible frequency range (0 - Nyquist)
