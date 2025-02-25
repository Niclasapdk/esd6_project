%% General parameters
Fs = 44100; % sample rate (Hz)
N = Fs*10; % number of samples to simulate
gain = 0.7; % input signal gain parameter
f0 = 100; % excitation frequency (Hz)
t = 0:N-1; % time vector for the excitation
tstart = 0; % for sound clip
input = sin(2*pi*f0/Fs.*t); % the excitation signal
[input, Fs] = audioread("2. Raw Guitar Line No Amp.mp3", [tstart*Fs+1 tstart*Fs+N+1]);
input = input .* gain;
output = zeros(1,length(input));

%% Diode parameters
V_T = 26e-3;
I_S = 1e-12;

%% input subcircuit (red)
C_2 = C(1e-6,Fs);
R_A = R(220);
V_bias = V(4.5, 10e3);
inp_ser = ser(C_2, ser(R_A, V_bias));

%% Lowpass subcircuit (green)
C_3 = C(0.047e-6,Fs);
R_4 = R(4.7e3);
lp_ser = ser(C_3, R_4);

%% Diode subcircuit (blue)
I_f = I(0, 51e3 + 250e3); % start at P1 = 250k (0-500k)
C_4 = C(51e-12, Fs);
clip_par = par(I_f, C_4);
r_is = clip_par.PortRes*I_S;
r_is_over_vt = r_is/V_T;

%% Traces
V_plus = zeros(1, length(input));
I_f_value = zeros(1, length(input));
output = zeros(1, length(input));
dbg1 = zeros(1, length(input));
dbg2 = zeros(1, length(input));
t1 = zeros(1, length(input));
t2 = zeros(1, length(input));
t3 = zeros(1, length(input));

%% Warm up
tic();
for n = 1:10000
    WaveUp(inp_ser);
    inp_ser.WD = 0;

    WaveUp(lp_ser);
    lp_ser.WD = 0;

    WaveUp(clip_par);
    clip_par.WD = 0;
end
warmup_time = toc()
%% The simulation loop:
bar = waitbar(0, "starting");
tic();
for n = 1:N % run each time sample until N
    if mod(n, 10000) == 0
        waitbar(n/N, bar, sprintf('Progress: %d of %d', n, N));
    end
    tim = tic();
    % input subcircuit
    WaveUp(inp_ser);
    t1(n) = toc(tim);tim = tic();
    inp_ser.WD = input(n);
    V_plus(n) = (V_bias.WU + V_bias.WD)/2;
    t2(n) = toc(tim);

    % Lowpass subcircuit
    WaveUp(lp_ser);
    lp_ser.WD = 2*V_plus(n) - lp_ser.WU;
    I_f_value(n) = (R_4.WD - R_4.WU)/(2*R_4.PortRes);

    tim = tic();
    % Diode subcircuit
    I_f.E = I_f_value(n);
    a = WaveUp(clip_par);
    lambda = sign(a);
    wright = wrightOmegaq(log(r_is_over_vt) + (lambda * a)/V_T + r_is_over_vt);
    % wright = fast_omega(log(r_is_over_vt) + (lambda * a)/V_T + r_is_over_vt);
    b = a + 2 * lambda * (r_is - V_T * wright);
    clip_par.WD = b;
    t3(n) = toc(tim);
    V_diode = (a + b)/2;
    output(n) = V_plus(n) + V_diode - 4.5;
end
sim_time = toc()
sim_time_per_second = sim_time / (N/Fs)
t1_sim_time_per_second = sum(t1) / (N/Fs)
t2_sim_time_per_second = sum(t2) / (N/Fs)
t3_sim_time_per_second = sum(t3) / (N/Fs)
%% Plot the results
% figure;
t = (0:length(input)-1)./Fs; % create a time vector for the figure
hold on;
% plot(t,input,'--'); hold on; % plot the input signal, keep figure open
% plot(t,V_plus); hold off; % plot output signal, prevent further plots
% figure;
% plot(t,I_f_value); hold off;
plot(t, output, "r");
% plot(t, dbg1, "g");
% plot (t, dbg2, "y");
% ylim([-1 1]);
% xlim([0.001 0.004]);