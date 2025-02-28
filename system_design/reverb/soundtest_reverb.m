%sound test of reverb
[input, fs] = audioread('../soundclips/guitar.wav');
%normalize sound because dododododo
input_sound = input / max(abs(input));

%reverb parameters
gain = 0.5;
reverb_time = 1;

%add reverb
output_signal = reverb(input_sound,fs,gain,reverb_time);

%play sound
%sound(output_signal, fs);

%save sound
audiowrite('reverb_v0.wav', output_signal, fs);