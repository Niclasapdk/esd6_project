clc
[in,Fs]=audioread('deltaclick.wav');
 %Limitamos a los 15 primeros segundos, para ficheros de larga duraci�n 
if(length(in)>10*Fs) 
    in=in(1:10*Fs); 
end
mix = 0.7; % dont set mix = 1
delay = 5;
width = 2.5;
rate= 1;
voices = 5;
out = flanger(in,mix,delay,width,rate,Fs,voices);
%sound(in,Fs);
sound(out,Fs);