%sound test of reverb
[input, fs] = audioread('../soundclips/guitar.wav');
%normalize sound because dododododo
input_sound = input / max(abs(input));

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
gain = 0.5;
reverb_time = 0.1;

%add reverb
output_signal = reverb(input_sound,fs,gain,reverb_time);

%play sound
%sound(output_signal, fs);

%save sound
audiowrite('reverb_v0.wav', output_signal, fs);