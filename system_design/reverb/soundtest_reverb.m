%sound test of reverb
[input, fs] = audioread('../soundclips/fcpremix.wav');

starttime = 0.5;
desiredLength = 10;                        % Seconds
t = starttime * fs;
L = fs * desiredLength + t;                   % Samples/sec * sec = Samples
trimmed_input = input(t:L,:);

%normalize sound because dododododo
input_sound = trimmed_input / max(abs(trimmed_input));

%reverb parameters
gain = 0;
reverb_time = 1;

%add reverb
start = tic();
output_signal = reverb(input_sound,fs,gain,reverb_time);
toc(start)

%play sound
%sound(output_signal, fs);

%save sound
audiowrite('reverb_tap7_0_1.wav', output_signal/max(abs(output_signal)), fs);