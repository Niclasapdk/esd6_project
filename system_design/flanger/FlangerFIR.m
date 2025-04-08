[filename,pathname] = uigetfile('*.*','Select input audio file');

[x1, fs] = audioread(num2str(filename));

function [outputSound] = Flanger(inputSound, Fs, gain, delay, width, rate)
    %Flanger: A music effect
    x = inputSound; %Input matrix
    n = length(x);

    sDelay = round(delay*Fs); %Center delay in samples
    sWidth = round(width*Fs); %Delay variation in samples
    modFreq = rate/Fs; %Modulation frequency in samples/sec
    maxDelay = sDelay + sWidth; %Max delay possible
    delayLine = zeros(maxDelay, 1); %Memory allocation for max delay
    y = zeros(n+maxDelay,1); %Output matrix

    for n = 1:(length(x))
        mod = sin(modFreq*n*2*pi);
        tap = round(sDelay+sWidth*mod);
        y(n) = (1-gain)*x(n)+gain*delayLine(tap-1);
        delayLine=[x(n);delayLine(1:maxDelay-1)];
    end
    outputSound = y;
end

y1 = Flanger(x1(:,1), fs, 0.7, 0.015, 0.004, 1);

audiowrite('guitar_flanged_fir.wav', y1, fs);