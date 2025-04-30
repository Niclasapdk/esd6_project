N = 1024;  % FFT length
impulse = [1; zeros(N-1, 1)];  % Unit impulse
fs = 44100;  % Sampling rate (adjust if needed)

% Filter parameters
delay_ms = 1; 
gainLP = 0.7; 
reverbtime = 1;

% Get impulse responses
output = combfilter(impulse, fs, delay_ms, gainLP, reverbtime);
output_w = combfilter_with_input_delayline(impulse, fs, delay_ms, gainLP, reverbtime);

% Compute FFTs
H1 = fft(output, N);
H2 = fft(output_w, N);
f = (0:N-1)*(fs/N);  % Frequency axis

% Plot magnitude responses
figure;
plot(f, abs(H1), 'b', 'LineWidth', 1); hold on;
plot(f, abs(H2), 'r--', 'LineWidth', 1);
xlabel('Frequency (Hz)');
ylabel('Magnitude');
title('Magnitude Responses of combfilter vs combfilter\_with\_input\_delayline');
legend('combfilter', 'combfilter\_with\_input\_delayline');
xlim([0 fs/2]);  % Only up to Nyquist
grid on;
