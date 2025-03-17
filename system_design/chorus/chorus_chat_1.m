% Parameters
fs = 44100;               % Sampling rate
num_voices = 4;           % Number of chorus voices (1-8)
delay_range = [10, 25];   % Delay range in milliseconds (10 - 40ms)
mod_depth = 1;            % Modulation depth (fraction of delay) (0-100%)
lfo_rate = 1;             % LFO frequency (max 1.5)
mix = 0.7;                % (0-100% dry/wet)

delay_range_samples = round(delay_range * fs / 1000);  % Convert delay range to samples
[input, fs] = audioread('pen15.wav'); % Input signal (replace with your actual audio)
duration = 5;  % Duration in seconds
numSamples = min(duration * fs, length(input)); % numSamples for x seconds
input = input(1:numSamples, :); % Keep only the first x seconds

output = zeros(size(input)); % Output buffer

for v = 1:num_voices                         % Process each voice
    base_delay = randi(delay_range_samples); % Random base delay for this voice (in samples)
    t = (0:length(input)-1)' / fs;           % Create LFO (sinusoidal modulation)
    %lfo = sawtooth(2*pi*lfo_rate*t+ pi/2 * (v-1),0.5); % another option for chorus
    %lfo = sin(2 * pi * lfo_rate * t + pi/2 * (v-1)); % Phase shifted 90deg lfo
    mod_delay = base_delay * mod_depth * (0.5*lfo + 0.5);  % Calculate varying delay in samples
    voice_signal = zeros(size(input));       %Buffer for modulated signal

    for n = 1:length(input)                  % Apply modulated delay to create the chorus voice
        delay_samples = round(mod_delay(n)); %
        if n - delay_samples > 0             % When signal > then delay samples
            voice_signal(n) = input(n - delay_samples);     % Apply delay
        else
            voice_signal(n) = 0;             % Zero padding if delay exceeds buffer
        end
    end
    output = output + voice_signal / num_voices; % Accumulate into output (average the voices)
end

% Combine dry and wet (optional)
%mix = 0.7;  % (0-100% dry/wet)
output = mix * output + (1 - mix) * input;

% Listen
%sound(input, fs);
sound(output, fs);

% Save to file if needed
% audiowrite('chorus_output.wav', output, fs);
