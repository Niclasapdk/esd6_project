% Parameters
fs = 44100;               % Sampling rate
num_voices = 4;           % Number of chorus voices
delay_range = [10, 25];    % Delay range in milliseconds
mod_depth = 0.4;          % Modulation depth (fraction of delay)

% Convert delay range to samples
delay_range_samples = round(delay_range * fs / 1000);

% Input signal (replace with your actual audio)
[input, fs] = audioread('fcpremix.wav');
%input = input(:, 1);  % Use mono for simplicity
duration = 15;  % Duration in seconds
% Extract first x seconds (ensure it does not exceed the file length)
numSamples = min(duration * fs, length(input));
input = input(1:numSamples, :); % Keep only the first x seconds

% Output buffer
output = zeros(size(input));

% Process each voice
for v = 1:num_voices
    % Random base delay for this voice (in samples)
    base_delay = randi(delay_range_samples);

    % Random LFO rate (Hz) for pitch modulation
    lfo_rate = 0.5;
    %lfo_rate = 0.1 + (0.4 * rand);  % 0.1 to 0.5 Hz

    % Create LFO (sinusoidal modulation)
    t = (0:length(input)-1)' / fs;
    lfo = sin(2 * pi * lfo_rate * t);
    %lfo = sawtooth(2*pi*lfo_rate*t,0.5);

    % Calculate varying delay in samples
    %mod_delay = base_delay * (1 + mod_depth * lfo);  % Delay modulated by LFO
    mod_delay = 10 + 15 * mod_depth * (0.5*lfo + 0.5);
    
    % Apply modulated delay to create the chorus voice
    voice_signal = zeros(size(input));

    for n = 1:length(input)
        delay_samples = round(mod_delay(n));
      
        if n - delay_samples > 0
            voice_signal(n) = input(n - delay_samples);
        else
            voice_signal(n) = 0; % Zero padding if delay exceeds buffer
        end
    end

    % Accumulate into output (average the voices)
    output = output + voice_signal / num_voices;
end

% Combine dry and wet (optional)
mix = 0.7;  % 50% dry, 50% wet
output = mix * output + (1 - mix) * input;

% Listen
%sound(input, fs);
sound(output, fs);

% Save to file if needed
% audiowrite('chorus_output.wav', output, fs);
