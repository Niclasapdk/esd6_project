% Parameters
Fs = 1000;      % Sampling frequency (Hz)
T = 1;          % Duration of the signal (seconds)
f_signal = 2;   % Frequency of the base signal (Hz)
f_mod = 5;      % Frequency of modulation (Hz)

% Time vector
t = 0:1/Fs:T-1/Fs;

% Base signal (e.g., a sine wave)
signal = sin(2*pi*f_signal*t);

% Carrier signals (sine, triangle, and square waves)
carrier_sin = sin(2*pi*f_mod*t);
carrier_triangle = sawtooth(2*pi*f_mod*t, 0.5);  % Triangle wave
carrier_square = square(2*pi*f_mod*t);           % Square wave

% Modulated signals (tremolo effect)
modulated_sin = signal .* (1 + 0.5 * carrier_sin);          % 50% depth
modulated_triangle = signal .* (1 + 0.5 * carrier_triangle); % 50% depth
modulated_square = signal .* (1 + 0.5 * carrier_square);     % 50% depth

% Plotting the results in the desired layout (3 rows, 3 columns)
figure;

% First row: Original signal
subplot(3,3,[1 2 3]);  % Use the entire first row
plot(t, signal, 'k', 'LineWidth', 2, 'DisplayName', 'Original Signal');
title('Original Signal (Sine Wave)', 'FontSize', 14);
xlabel('Time [s]', 'FontSize', 12);
ylabel('Amplitude', 'FontSize', 12);
legend off;
grid on;

% Second row: Carrier signals (sine, triangle, square)
subplot(3,3,4);
plot(t, carrier_sin, 'b', 'LineWidth', 1, 'DisplayName', 'Sine Carrier');
title('Sine Carrier', 'FontSize', 14);
xlabel('Time [s]','FontSize', 12);
ylabel('Amplitude','FontSize', 12);
legend off;
grid on;

subplot(3,3,5);
plot(t, carrier_triangle, 'r', 'LineWidth', 1, 'DisplayName', 'Triangle Carrier');
title('Triangle Carrier', 'FontSize', 14);
xlabel('Time [s]','FontSize', 12);
ylabel('Amplitude','FontSize', 12);
legend off;
grid on;

subplot(3,3,6);
plot(t, carrier_square, 'g', 'LineWidth', 1, 'DisplayName', 'Square Carrier');
title('Square Carrier', 'FontSize', 14);
xlabel('Time [s]','FontSize', 12);
ylabel('Amplitude','FontSize', 12);
legend off;
grid on;

% Third row: Modulated signals (using each carrier)
subplot(3,3,7);
plot(t, modulated_sin, 'b', 'LineWidth', 1, 'DisplayName', 'Modulated with Sine');
title('Modulated with Sine', 'FontSize', 14);
xlabel('Time [s]','FontSize', 12);
ylabel('Amplitude','FontSize', 12);
legend off;
grid on;

subplot(3,3,8);
plot(t, modulated_triangle, 'r', 'LineWidth', 1, 'DisplayName', 'Modulated with Triangle');
title('Modulated with Triangle', 'FontSize', 14);
xlabel('Time [s]','FontSize', 12);
ylabel('Amplitude','FontSize', 12);
legend off;
grid on;

subplot(3,3,9);
plot(t, modulated_square, 'g', 'LineWidth', 1, 'DisplayName', 'Modulated with Square');
title('Modulated with Square', 'FontSize', 14);
xlabel('Time [s]','FontSize', 12);
ylabel('Amplitude','FontSize', 12);
legend off;
grid on;
