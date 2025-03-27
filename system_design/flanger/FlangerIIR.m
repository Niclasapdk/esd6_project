[filename, pathname] = uigetfile('*.*', 'Select input audio file');

[x1, fs] = audioread(num2str(filename));

function [outputSound] = IIRFlanger(inputSound, Fs, gain, delay, width, rate)
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

    x = inputSound;     % Ensure column vector
    n = length(x);

    % Convert time-based parameters to samples
    sDelay = round(delay * Fs);   % Center delay in samples
    sWidth  = round(width * Fs);   % Delay variation in samples
    modFreq   = rate / Fs;           % LFO cycles per sample

    % Max possible delay (in samples) and allocate memory
    maxDelay = sDelay + sWidth;
    delayLine = zeros(maxDelay+1, 1);  % Buffer for delayed *output* samples

    % Allocate output
    y = zeros(n + maxDelay, 1); 

    for n = 1:(length(x))
        
        % Compute time-varying delay via sinusoidal LFO
        mod = sin(2*pi*modFreq*n);
        currentDelay = round(sDelay + sWidth * mod);
        currentDelay = max(1, min(maxDelay, currentDelay));

        % Read the delayed output sample
        delayedSample = delayLine(currentDelay+1);

        % Combine input with the delayed sample 
        % -- "gain" sets how much of the delayed signal is added back
        % -- some designs include an additional scaling on x(n) if needed
        y(n) = (1-gain)*x(n) + gain * delayedSample;

        % Update delay line (push new output sample into the front)
        delayLine = [y(n); delayLine(1:end-1)];
    end

    outputSound = y;
end

y1 = IIRFlanger(x1(:,1), fs, 0.7, 0.015, 0.004, 0.5);

audiowrite('guitar_flanged_IIR.wav', y1, fs);
