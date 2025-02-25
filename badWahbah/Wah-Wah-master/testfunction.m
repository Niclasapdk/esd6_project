clc
[x,Fs]=audioread('guitar.wav'); 
 %Limitamos a los 15 primeros segundos, para ficheros de larga duración 
if(length(x)>15*Fs) 
    x=x(1:15*Fs); 
end
disp(size(x));
% constants
damp = 0.1; 
minf=350; 
maxf=2250; 
Fw = 1000;
Fc = 2000;
wahType=1;
% center frequency (Fc) needs to be a variable (sensor)
% function call
y=EfectoWahWah(damp,minf,maxf,Fs,Fw, x,Fc,wahType); 
sound(y,Fs)