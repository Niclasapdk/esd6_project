function [outputSound] = FlangerIIR(inputSound, Fs, gain, delay, width, rate)
    % FlangerIIR: A basic IIR flanger effect
    %   inputSound: Single-channel input signal
    %   Fs: Sampling frequency
    %   gain: Feedback gain for delayed signal (0 < gain < 1)
    %   delay: Average delay (in seconds)
    %   width: Delay modulation range (in seconds)
    %   rate: LFO (low-frequency oscillator) rate (in Hz)
    %
    % The output will have an IIR-like feedback structure, i.e. some portion 
    % of the effect output is fed back into the delay line.

    x = inputSound(:);     % Ensure column vector
    nSamples = length(x);

    % Convert time-based parameters to samples
    baseDelay = round(delay * Fs);   % Center delay in samples
    modWidth  = round(width * Fs);   % Delay variation in samples
    modFreq   = rate / Fs;           % LFO cycles per sample

    % Max possible delay (in samples) and allocate memory
    maxDelay = baseDelay + modWidth;
    delayLine = zeros(maxDelay, 1);  % Buffer for delayed *output* samples

    % Allocate output
    y = zeros(nSamples, 1);

    for n = 1:nSamples
        
        % Compute time-varying delay via sinusoidal LFO
        modFactor = sin(2*pi*modFreq * n);
        currentDelay = round(baseDelay + modWidth * modFactor);

        % Read the delayed output sample
        delayedSample = delayLine(currentDelay);

        % Combine input with the delayed sample 
        % -- "gain" sets how much of the delayed signal is added back
        % -- some designs include an additional scaling on x(n) if needed
        y(n) = x(n) + gain * delayedSample;

        % Update delay line (push new output sample into the front)
        delayLine = [y(n); delayLine(1:end-1)];
    end

    outputSound = y;
end
