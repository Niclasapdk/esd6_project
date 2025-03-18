%this script takes the impulse response of the combfilter 
clc, clear, close all;

%resolution 
fs = 44100;
N = fs*0.2;%value in seconds
before_0 = 0.01*fs;


% Create an impulse
impulse = zeros(N+before_0,1);
impulse(before_0+1) = 1;

% Output vector
ir = zeros(1,N+before_0);

gain = 0; 
reverbtime = 1;

%to test reverb
ir = reverb(impulse, fs, gain, reverbtime);


% Time vector in seconds
t = ((0:N+before_0-1)-before_0) / fs;

% Plot the impulse response in dB
plot(t,ir);
xlabel('Time (seconds)');
ylabel('Magnitude');
title('Impulse Response');
xline(0.0796, '--', {'Last early','reflection'})
grid on;

