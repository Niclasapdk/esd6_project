[input, fs] = audioread('../../soundclips/lead.wav');

starttime = 0.5;
desiredLength = 5;                        % Seconds
t = starttime * fs;
L = fs * desiredLength + t;                % Samples/sec * sec = Samples
trimmed_input = input(t:L,:);

%add reverb
start = tic();
output_signal = arrayfun(@overdrive, trimmed_input);
toc(start)

%play sound
sound(output_signal, fs);

%save sound
%audiowrite('reverb_test_2_1_7tap.wav', output_signal/max(abs(output_signal)), fs);
