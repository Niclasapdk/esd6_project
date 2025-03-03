%sound test of reverb
[input, fs] = audioread('../soundclips/fcpremix.wav');

starttime = 0.5;
desiredLength = 10;                        % Seconds
t = starttime * fs;
L = fs * desiredLength + t;                   % Samples/sec * sec = Samples
trimmed_input = input(t:L,:);

%normalize sound because dododododo
input_sound = trimmed_input / max(abs(trimmed_input));

%try sinus
%fs = 44100;
%duration = 0.2;
%f = 100;
%t = 0:1/fs:duration;
%input_sound = sin(2*pi*f*t);

%tryimpulse
%impulse = zeros(N,1);
%impulse(1) = 1;

%reverb parameters
gain = 0.3;
reverb_time = 0.2;

%add reverb
start = tic();
output_signal = reverb(input_sound,fs,gain,reverb_time);
toc(start)
%play sound
%sound(output_signal, fs);

%save sound
audiowrite('sound_tests/reverb_test3.wav', output_signal/max(abs(output_signal)), fs);