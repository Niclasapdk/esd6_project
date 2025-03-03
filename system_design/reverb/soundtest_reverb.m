%sound test of reverb
[input, fs] = audioread('../soundclips/guitar.wav');

desiredLength = 0.05;                        % Seconds
L = fs * desiredLength;                   % Samples/sec * sec = Samples
trimmed_input = input(1:L,:);

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
gain = 0.75;
reverb_time = 1;

%add reverb
output_signal = reverb(input_sound,fs,gain,reverb_time);

%play sound
%sound(output_signal, fs);

%save sound
audiowrite('reverb.wav', output_signal/max(abs(output_signal)), fs);