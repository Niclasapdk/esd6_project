function out = flangerdog(in, mix, delay, width, rate, fs)

minDelaySamp = ceil(delay * fs / 1000);           % Convert to ms, then samples
maxDelaySamp = ceil((delay + width) * fs / 1000); % Convert to ms, then samples
n = (1:length(in) + maxDelaySamp)';               % Index for modulation signal
%LFO=sawtooth(2*pi*rate/(fsn)*n,.5);              %sawtooth lfo
LFO = sin(2 * pi * rate / fs * n);                % Sinusoidal LFO

% Vector for the delayed modulated signal
delayTimeSamples = (delay + width / 2 + width / 2 * LFO) * fs / 1000;

out = zeros(length(in) + minDelaySamp, 1);   % Create output vector
out(1:maxDelaySamp) = in(1:maxDelaySamp);   % Copy start input signal before min delay

for i = maxDelaySamp + 1:length(in)             % Start from next sample
    delaySamples = ceil(delayTimeSamples(i));   % Whole number of current delay
    out(i) = in(i) + mix * out(i - delaySamples); % Add input and delay signal
end

end
