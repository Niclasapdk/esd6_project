%this script takes the impulse response of the combfilter 
clc, clear, close all;

%resolution 
N = 2^14;
fs = 44100;

% Create an impulse
impulse = zeros(N,1);
impulse(1) = 1;

% Output vector
ir = zeros(1,N);

% Filter parameters
delay_ms = 78; 
gainLP = 0.5; 
reverbtime = 1;


%ir = combfilter(impulse, fs, delay_ms, gainLP, reverbtime);

%to test reverb
ir = reverb(impulse, fs, gainLP, reverbtime);

% Time vector in seconds
t = (0:N-1) / fs;

magnitude_dB = 20 * log10(abs(ir) + eps); % Add eps to avoid log(0)

% Plot the impulse response in dB
plot(t, magnitude_dB);
xlabel('Time (seconds)');
ylabel('Magnitude (dB)');
title('Impulse Response in dB');
grid on;

hold on;
yline(-60, '--r'); %adds line at -60 db
