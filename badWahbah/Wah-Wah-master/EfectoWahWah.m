function y=EfectoWahWah(damp,minf,maxf,Fs,Fw, x,Fc,wahType)
% Wah type = 1, is the triangle wave
% Wah type = 2, is the constant Fc

% Fc needs to be in the range if not then end
if Fc > maxf | Fc < minf 
        disp('No work Fc not in value !!!!!!!!!!!!!!!!!!!!!!!!!');
        disp('You fucking dumbasss');
        return;
end

switch wahType
    case 1 % triangle wave / wahwah effect, making the wah wah sound
        Fc=minf;
    % tunning coeffiecients and start Fc
    Q1 = 2*damp;
    F1 = 2*sin((pi*Fc)/Fs);
    % first element in the vector
    y(1) = F1*x(1);
    yl = F1*y(1);
    yaux = y(1); 
    delta = Fw/Fs; % step size
            for n=2:length(x),
        if Fc + delta > maxf | Fc + delta < minf 
            delta =  -delta;
        end
        Fc = Fc+ delta;
        F1 = 2*sin((pi*Fc)/Fs);
        yh = x(n)-yl-Q1*yaux; % highpass
        y(n) = F1*yh+yaux; % bandpass
        yl=F1*y(n)+yl; % lowpass
        yaux=y(n); 
            end
    case 2 % for constant Fc same loop as shown above for triangle

    delta = Fw/Fs; % step size
    % vector with all possible frequencies
    freq=minf:delta:maxf;
    % find the closest frequency to Fc
    [~, idx] = min(abs(freq - Fc));  % idx gives the index of the closest value
    Fc = freq(idx);  % This is the closest frequency to Fc (finite length)

    % tunning coeffiecients and start Fc
    Q1 = 2*damp;
    F1 = 2*sin((pi*Fc)/Fs);
    % first element in the vector
    y(1) = F1*x(1);
    yl = F1*y(1);
    yaux = y(1); 
        for n=2:length(x),
    yh = x(n)-yl-Q1*yaux; % highpass
    y(n) = F1*yh+yaux; % bandpass
    yl=F1*y(n)+yl; % lowpass
    yaux=y(n); 
        end
end
         % needs to do both
    % normalize the values in the vector of y
    maxy = max(abs(y));
    y = y/maxy;
end

% kig lige på det her slider lort https://www.youtube.com/watch?v=wGaPLzaHIps 