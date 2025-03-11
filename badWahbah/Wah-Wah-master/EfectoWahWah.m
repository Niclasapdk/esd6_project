function y=EfectoWahWah(damp,minf,maxf,Fs,Fw, x,Fc,wahType)
% Wah type = 1, is the triangle wave
% Wah type = 2, is the constant Fc

% Fc needs to be in the range if not then end
if Fc > maxf | Fc < minf 
        disp('No work Fc not in value !!!!!!!!!!!!!!!!!!!!!!!!!');
        return;
end

switch wahType
    case 1                      % triangle wave / wahwah effect, making the wah wah sound
        Fc=minf;                % Set cutoff to min frequency
    % tunning coeffiecients from eq 2.8 (56 PDF DAFX)
    Q1 = 2*damp;                %Q coefficient
    F1 = 2*sin((pi*Fc)/Fs);     %F coefficient
    y=zeros(size(x));           %Vector full 0's for output
    % first element in the vector
    y(1) = F1*x(1);             % First output eq 2.7 (56 PDF DAFX), as NO priovious output yet
    yl = F1*y(1);               % First Lowpass coefficient eq 2.7 (56 PDF DAFX) no priovious
    yaux = y(1);                % First output
    delta = Fw/Fs;              % Define step size based upon how large a frequency range it sweeps per sec
    % Loop to create the triangle wave going from minf-maxf-minf and so on
            for n=2:length(x),  
        if Fc + delta > maxf | Fc + delta < minf % Change if it goes up or down when hiting maxf or minf
            delta =  -delta;
        end
        Fc = Fc+ delta;                          % increase Fc by stepsize
        F1 = 2*sin((pi*Fc)/Fs);                  % Calculate F1 coefficient for each Fc
        yh = x(n)-yl-Q1*yaux;                    % highpass coefficient
        y(n) = F1*yh+yaux;                       % bandpass coefficient
        yl=F1*y(n)+yl;                           % lowpass coeficient
        yaux=y(n);                               % set the output = bandpass
            end
    case 2                           % Constant Fc
    delta = Fw/Fs;                   % step size
    freq=minf:delta:maxf;            % vector with all possible frequencies
    [~, idx] = min(abs(freq - Fc));  % index of the closest value
    Fc = freq(idx);                  % Set Fc to closest value
    y=zeros(size(x));                % Vector full 0's for output
    % tunning coeffiecients and start Fc
    Q1 = 2*damp;                     % Q coefficient
    F1 = 2*sin((pi*Fc)/Fs);          % F coefficient
    % first element in the vector
    y(1) = F1*x(1);                  % First output as NO priovious output
    yl = F1*y(1);                    % First Lowpass coefficient No priovious
    yaux = y(1);                     % First output
    % Loop to create the triangle wave going from minf-maxf-minf and so on
        for n=2:length(x),
    % all equations from eq 2.7 (56 PDF DAFX)
    yh = x(n)-yl-Q1*yaux;            % highpass coefficient
    y(n) = F1*yh+yaux;               % bandpass coefficient
    yl=F1*y(n)+yl;                   % lowpass coefficient
    yaux=y(n);                       % set the output = bandpass
        end
    otherwise
    error('Invalid wahType! Must be 1 or 2.');
end
         % needs to do both
    % normalize the values in the vector of y
    maxy = max(abs(y));
    y = y/maxy;
end

% kig lige på det her slider lort https://www.youtube.com/watch?v=wGaPLzaHIps 