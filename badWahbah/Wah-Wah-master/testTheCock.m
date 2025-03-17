% Sampling frequency
Fs = 44100;    

% Define filter parameters for two filters
Fc1 = 350;    % First center frequency (Hz)
damp1 = 0.05;  % First damping factor
Fc2 = 400;    % Second center frequency (Hz)
damp2 = 0.1;   % Second damping factor
Fc3= 1000;      % Third center frequency (Hz)
damp3 = 0.05;  % Third damping factor
Fc4= 1200;      % Third center frequency (Hz)
damp4 = 0.1;  % Third damping factor
% maybe lige test damp i forhold til Fc

% Generate impulse input (delta function)
N = 1024; 
x = zeros(N, 1);
x(1) = 1; % Impulse at n=1

% ------------ Apply Bandpass Filter for First Set of Parameters ------------
y1 = zeros(size(x)); % Initialize output
Q1 = 2 * damp1;                    % Q coefficient
F1 = 2 * sin((pi * Fc1) / Fs);      % Frequency coefficient

% First sample
y1(1) = F1 * x(1);                  
yl = F1 * y1(1);                   
yaux = y1(1);                       

% Apply filter loop
for n = 2:length(x)
    yh = x(n) - yl - Q1 * yaux;  % Highpass coefficient
    y1(n) = F1 * yh + yaux;      % Bandpass coefficient
    yl = F1 * y1(n) + yl;        % Lowpass coefficient
    yaux = y1(n);                % Update
end

% Compute frequency response for first filter
[H1, w] = freqz(y1, 1, N, Fs);
H1_dB = 20 * log10(abs(H1));

% ------------ Apply Bandpass Filter for Second Set of Parameters ------------
y2 = zeros(size(x)); % Initialize output
Q2 = 2 * damp2;                    % Q coefficient
F2 = 2 * sin((pi * Fc2) / Fs);      % Frequency coefficient

% First sample
y2(1) = F2 * x(1);                  
yl = F2 * y2(1);                   
yaux = y2(1);                       

% Apply filter loop
for n = 2:length(x)
    yh = x(n) - yl - Q2 * yaux;  % Highpass coefficient
    y2(n) = F2 * yh + yaux;      % Bandpass coefficient
    yl = F2 * y2(n) + yl;        % Lowpass coefficient
    yaux = y2(n);                % Update
end

% Compute frequency response for second filter
[H2, ~] = freqz(y2, 1, N, Fs);
H2_dB = 20 * log10(abs(H2));

% ------------ Apply Bandpass Filter for Third Set of Parameters ------------
y3 = zeros(size(x)); % Initialize output
Q3 = 2 * damp3;                    % Q coefficient
F3 = 2 * sin((pi * Fc3) / Fs);      % Frequency coefficient

% First sample
y3(1) = F3 * x(1);                  
yl = F3 * y3(1);                   
yaux = y3(1);                       

% Apply filter loop
for n = 2:length(x)
    yh = x(n) - yl - Q3 * yaux;  % Highpass coefficient
    y3(n) = F3 * yh + yaux;      % Bandpass coefficient
    yl = F3 * y3(n) + yl;        % Lowpass coefficient
    yaux = y3(n);                % Update
end

% Compute frequency response for second filter
[H3, ~] = freqz(y3, 1, N, Fs);
H3_dB = 20 * log10(abs(H3));

% ------------ Apply Bandpass Filter for Fourth Set of Parameters ------------
y4 = zeros(size(x)); % Initialize output
Q4 = 2 * damp4;                    % Q coefficient
F4 = 2 * sin((pi * Fc4) / Fs);      % Frequency coefficient

% First sample
y4(1) = F4 * x(1);                  
yl = F4 * y4(1);                   
yaux = y4(1);                       

% Apply filter loop
for n = 2:length(x)
    yh = x(n) - yl - Q4 * yaux;  % Highpass coefficient
    y4(n) = F4 * yh + yaux;      % Bandpass coefficient
    yl = F4 * y4(n) + yl;        % Lowpass coefficient
    yaux = y4(n);                % Update
end

% Compute frequency response for second filter
[H4, ~] = freqz(y4, 1, N, Fs);
H4_dB = 20 * log10(abs(H4));

% --------------------- Plot Frequency Responses ----------------------

figure;

% Plot magnitude responses
subplot(2,1,1);
semilogx(w, H1_dB, 'r', 'LineWidth', 1.5); hold on;
semilogx(w, H2_dB, 'b', 'LineWidth', 1.5); hold on;
semilogx(w, H3_dB, 'g', 'LineWidth', 1.5); hold on;
semilogx(w, H4_dB, 'm', 'LineWidth', 1.5); 
xline(Fc1, '--r', ['Fc1 = ' num2str(Fc1) ' Hz'], 'LineWidth', 1.2, 'LabelVerticalAlignment', 'bottom'); % Red dashed line
xline(Fc2, '--b', ['Fc2 = ' num2str(Fc2) ' Hz'], 'LineWidth', 1.2, 'LabelVerticalAlignment', 'bottom'); % Blue dashed line
xline(Fc3, '--g', ['Fc3 = ' num2str(Fc3) ' Hz'], 'LineWidth', 1.2, 'LabelVerticalAlignment', 'bottom'); % Red dashed line
xline(Fc4, '--m', ['Fc4 = ' num2str(Fc4) ' Hz'], 'LineWidth', 1.2, 'LabelVerticalAlignment', 'bottom'); % Blue dashed line
xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
title('Bandpass (wah) Filter Magnitude Response');
legend(['Fc = ' num2str(Fc1) ' Hz, Damping = ' num2str(damp1)], ...
       ['Fc = ' num2str(Fc2) ' Hz, Damping = ' num2str(damp2)],...
       ['Fc = ' num2str(Fc3) ' Hz, Damping = ' num2str(damp3)],....
       ['Fc = ' num2str(Fc4) ' Hz, Damping = ' num2str(damp4)]);
grid on;

% Plot phase responses
subplot(2,1,2);
semilogx(w, angle(H1), 'r', 'LineWidth', 1.5); hold on;
semilogx(w, angle(H2), 'b', 'LineWidth', 1.5); hold on;
semilogx(w, angle(H3), 'g', 'LineWidth', 1.5); hold on;
semilogx(w, angle(H4), 'm', 'LineWidth', 1.5); 
xline(Fc1, '--r', [num2str(Fc1) ' Hz'], 'LineWidth', 1.2); % Red dashed line
xline(Fc2, '--b', [num2str(Fc2) ' Hz'], 'LineWidth', 1.2); % Blue dashed line
xline(Fc3, '--g', [num2str(Fc3) ' Hz'], 'LineWidth', 1.2); % Red dashed line
xline(Fc4, '--m', [num2str(Fc4) ' Hz'], 'LineWidth', 1.2); % Blue dashed line
xlabel('Frequency (Hz)');
ylabel('Phase (radians)');
title('Bandpass (wah) Filter Phase Response');
legend(['Fc = ' num2str(Fc1) ' Hz, Damping = ' num2str(damp1)], ...
       ['Fc = ' num2str(Fc2) ' Hz, Damping = ' num2str(damp2)],...
       ['Fc = ' num2str(Fc3) ' Hz, Damping = ' num2str(damp3)],....
       ['Fc = ' num2str(Fc4) ' Hz, Damping = ' num2str(damp4)]);
grid on;
