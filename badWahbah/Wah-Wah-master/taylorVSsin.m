clc; clear; close all;

% Define x from 0 to pi/2
x = linspace(0, pi/2, 100); 

% Normalize x according to Q15 input scaling
x_norm = x/pi;  % Since input is expected in (-1,1) range

% True sine function
y_sin = sin(x);

% Standard Taylor approximation (5th order)
%y_taylor = x - (x.^3)/factorial(3) + (x.^5)/factorial(5);

% Given Q15 coefficients (assumed in floating-point)
c1 = 3.140625;
c2 = 0.02026367;
c3 = -5.3251;
c4 = 0.5446778;
c5 = 1.800293;

% Compute the sine approximation using rescaled coefficients
y_custom_scaled = c1*x_norm + c2*x_norm.^2 + c3*x_norm.^3 + c4*x_norm.^4 + c5*x_norm.^5;

% Define frequency and sampling rate
f = 1;          % Example frequency
fs = 44100;     % Sampling frequency
k = (2 * pi * f) / fs;

% Initialize matrices
x1 = 1;  % Initial conditions for x1
x2 = 0;  % Initial conditions for x2

y1 = zeros(size(x));
y2 = zeros(size(x));

for i = 1:length(x)
    % Matrix computation
    y1(i) = (1 - (k^2)/2) * x1 + k * x2;
    y2(i) = -k * x1 + (1 - (k^2)/2) * x2;
    
    % Update for next iteration
    x1 = y1(i);
    x2 = y2(i);
end

% Plot results
figure;
hold on;
plot(x, y_sin, 'k-', 'LineWidth', 2); % True sine wave
%plot(x, y_taylor, 'r--', 'LineWidth', 2); % Standard Taylor approximation
plot(x, y_custom_scaled, 'b-.', 'LineWidth', 2); % Rescaled custom approximation
plot(x, y1, 'g:', 'LineWidth', 2); % Matrix-based approximation
hold off;

% Labels and legend
xlabel('x (radians)');
ylabel('sin(x) and approximations');
title('Sine Wave vs. Approximations');
legend('True sin(x)', 'Custom Approximation', 'Matrix-Based Approximation', 'FontSize', 14);
grid on;
