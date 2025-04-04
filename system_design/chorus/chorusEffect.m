function outputSound = chorusEffect(inputSound, Fs, mix, delay, width, rate, numVoices)
    % Chorus: A music effect with multiple delayed copies of the signal
    x = inputSound; % Input matrix
    y = zeros(size(x)); % Output matrix

    sDelay = round(delay * Fs); % Center delay converted from ms to samples
    sWidth = round(width * Fs); % Delay variation converted from ms to samples
    modFreq = rate / Fs; % Modulation frequency in samples/sec
    maxDelay = sDelay + sWidth; % Max delay possible in samples
    delayLine = zeros(maxDelay, 1); % Memory allocation for max delay

    % Generate independent phase shifts for each voice
    phaseOffsets = linspace(0, 2*pi, numVoices + 1); %Creates vector from 0 to 2pi (numVoices + 1 is the steps)
    phaseOffsets = phaseOffsets(1:end-1); % Remove the last point to avoid duplication

    for n = 1:length(x)
        y(n) = x(n); % Start with the original signal

        % Apply chorus effect with multiple voices
        for v = 1:numVoices
            mod = abs(sin(modFreq * n * 2 * pi + phaseOffsets(v))); % LFO with phase shift
            tap = round(sDelay + sWidth * mod); % delay for the current sample (varies depending on the index)
            y(n) = y(n) + mix * delayLine(tap); % Copies are created from original sound (y(n)) and the delay
        end

        % Update delay line
        delayLine = [x(n); delayLine(1:maxDelay-1)];
    end

    % Normalize output to avoid clipping
    %y(n) = y(n) / (1 + gain * numVoices);
    ymax = max(abs(y));
    y = y/ymax;

    outputSound = y;
end
