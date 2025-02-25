% wah_wah.m   state variable band pass
%
% BP filter with narrow pass band, Fc oscillates up and
% down the spectrum
% Difference equation taken from DAFX chapter 2
%
% Changing this from a BP to a BR/BS (notch instead of a bandpass) converts
%  this effect to a phaser
%
% yl(n) = F1*yb(n) + yl(n-1)
% yb(n) = F1*yh(n) + yb(n-1)
% yh(n) = x(n) - yl(n-1) - Q1*yb(n-1)
%
% vary Fc from 500 to 5000 Hz
infile = 'guitar.wav';
% read in wav sample
[ x, Fs ] = audioread(infile);
if(length(x)>15*Fs) 
    x=x(1:15*Fs); 
end
 %%%%%%% EFFECT COEFFICIENTS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% damping factor
% lower the damping factor the smaller the pass band ( 54 PDF DAFX)
damp = 0.1;
% min and m
% min and max centre cutoff frequency of variable bandpass filter
minf=350;
maxf=2200;
% wah frequency, how many Hz per second are cycled through
% determines how fast the frequency moves up and down over time
Fw = 2000;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% change in centre frequency per sample (Hz), step size
delta = Fw/Fs;

% create triangle wave of centre frequency values
% example
%Fc = [1 2 3] % Initial sequence
%Fc = [1 2 3 3 2 1] % Appended decreasing part
%Fc = [1 2 3 3 2 1 1 2 3] % Appended increasing part
% can be seen it is a triangle wave increasing and decreasing and so on
% does this until it has the length of (x).
Fc=minf:delta:maxf;

while(length(Fc) < length(x) )
    Fc= [ Fc (maxf:-delta:minf) ];
    Fc= [ Fc (minf:delta:maxf) ];
end
% trim tri wave to size of input, removes the values above length(x) from
% FC generated from the triangle wave
Fc = Fc(1:length(x));

%difference equation coefficients
% must be recalculated each time Fc changes (56 PDF DAFX)
% Fc(1) first element of the Fc triangle wave vector.
F1 = 2*sin((pi*Fc(1))/Fs);
% this dictates size of the pass bands (56 PDF DAFX)
Q1 = 2*damp;
% general rule for stability F1 < 2-Q1 (56 PDF DAFX)


%yh=zeros(size(x));     %High pass     
% create emptly out vectors 
yb=zeros(size(x));  %Bandpass
%yl=zeros(size(x));  %Low pass
% first sample, to avoid referencing of negative signals
%yh(1) = x(1); %High pass
yb(1) = F1*x(1); %Band pass
%yl(1) = F1*yb(1); %Lowpass - Kig lige på den anden benytter den her:
% x(1) first element of input signal, F1= tunning coefficient


% apply difference equation to the sample (56 PDF DAFX)
fc = minf; % inicializacion de fc

% start at n=2 cause n=1 already computed
for n=2:length(x),
    %yh(n) = x(n) - yl(n-1) - Q1*yb(n-1);
    yb(n) = F1*x(n)-F1*Q1*yb(n-1)+yb(n-1)-F1*F1*sum(yb(1:n-1));
    %yl(n) = F1*yb(n) + yl(n-1);
%(56 PDF DAFX) version of the difference equation
% yb(n)=F1*yh(n)+yb(n-1) % bandpass
% yh(n)= F1*x(n)-F1*Q1*yb(n-1) % mangler yl(n-1) % highpass
% +yb(n-1) %The other part of bandpass difference equation
% yl(n) = F1*yb(n) + yl(n-1) %lowpass 
%isolate => F1*yb(n) = yl(n)-yl(n-1)
% F1*F1*yb(n) = F1*(yl(n)-yl(n-1)) % is the sum part to get the lowpass
% part 
    if fc + delta > maxf | fc + delta < minf 
        delta =  -delta;
% | = or gate
% if maxf or minf is exceeded
% then delta must be in wrong direction so switch sign
    end
% for loop fc change with delta step size
   fc = fc+ delta;
    F1 = 2*sin((pi*fc)/Fs);
end

% make the max value output from the difference equation absolut
maxyb = max(abs(yb));
% To normalize the difference equation outputs, so it ranges from [-1,1]
yb = yb/maxyb;

%write output wav files
%wavwrite(yb, Fs, N, 'out_wah.wav');
audiowrite('out_wah.wav',yb, Fs);
figure(1)
hold on
% original input signal
plot(x,'r');
% wah difference equation output signal
plot(yb,'b');
title('Wah-wah and original Signal');
sound(yb,Fs);