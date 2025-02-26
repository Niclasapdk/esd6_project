%flanger program

[filename,pathname] = uigetfile('*.*','Select input audio file');

[x, Fs] = audioread(num2str(filename));

n = length(x);

tn = n/Fs;

f = 1; %LFO frequency

t = linspace(0, tn, n);

d = 100; %delay factor

modsin = sin(2*pi*f*t);

modsin1 = round(d.*modsin')+d; %variable delay

y = zeros(n+d,1); %output signal

a = 0.8; %gain 

xn = padarray(x, [d,0], 0, 'pre');

for i = (d+1):1:n

    y(i-d,1) = x(i) + a*xn(i-modsin1(i-d));
end

audiowrite('guitar_flanged.wav', y, Fs);