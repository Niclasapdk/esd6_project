% WDFDiodeExample.m
% Authors: V¨alim¨aki, Bilbao, Smith, Abel, Pakarinen, Berners
Fs = 44100; % sample rate (Hz)
N = Fs/10; % number of samples to simulate
gain = 30; % input signal gain parameter
f0 = 100; % excitation frequency (Hz)
t = 0:N-1; % time vector for the excitation
input = gain.*sin(2*pi*f0/Fs.*t); % the excitation signal
output = zeros(1,length(input));
V1 = V(0,1); % create a source with 0 (initial) voltage and 1 Ohm ser. res.
R1 = R(80); % create an 80Ohm resistor
CapVal = 3.5e-5; % the capacitance value in Farads
C1 = C(1/(2*CapVal*Fs)); % create the capacitance
s1 = ser(V1,ser(C1,R1)); % create WDF tree as a ser. conn. of V1,C1, and R1
Vdiode = 0; % initial value for the voltage over the diode
%% The simulation loop:
for n = 1:N % run each time sample until N
V1.E = input(n); % read the input signal for the voltage source
WaveUp(s1); % get the waves up to the root
Rdiode = 125.56*exp(-0.036*Vdiode); % the nonlinear resist. of the diode
r = (Rdiode-s1.PortRes)/(Rdiode+s1.PortRes); % update scattering coeff.
s1.WD = r*s1.WU; % evaluate the wave leaving the diode (root element)
Vdiode = (s1.WD+s1.WU)/2; % update the diode voltage for next time sample
output(n) = Voltage(R1); % the output is the voltage over the resistor R1
end;
%% Plot the results
t = (1:length(input))./Fs; % create a time vector for the figure
hi = plot(t,input,'--'); hold on; % plot the input signal, keep figure open
ho = plot(t,output); hold off; % plot output signal, prevent further plots
grid on; % use the grid for clarity
xlabel('Time (s)'); ylabel('Voltage (V)'); % insert x- and y-axis labels
legend([hi ho],'Source voltage E','Voltage over R1'); % insert legend