close all
clear


x = readtable('reverb_noise_dB.csv');
env = abs(hilbert(x.Channel1_V_));
env(env < 1e-12) = 1e-12;  % avoid log(0)
env_db = 20 * log10(env);

env1 = abs(hilbert(x.Channel2_V_));
env1(env1 < 1e-12) = 1e-12;  % avoid log(0)
env_db1 = 20 * log10(env1);



plot(x.Time_s_,x.Channel1_V_);
hold on
plot(x.Time_s_, x.Channel2_V_);
xlabel('Time (s)');
ylabel('Amplitude (dB)');
title('Signal dB Loss Over Time');