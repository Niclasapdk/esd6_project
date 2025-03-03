syms Rs Cs Rz Rf Cz Ri T Rl Rr s;
fs = 44100;

wz = 1/(Cz*(Rz + Rl*Rr/(Rl+Rr)));
wp = 1/(Cs*Rs*Ri/(Rs+Ri));

X = (Rr/(Rl+Rr))*1/(Rz+Rl*(Rr/(Rl+Rr))*Cz);
Y = (Rl+Rr)*(Rz+Rl*Rr/(Rl+Rr));
W = Y/(Rl*Rf + Y);
% calc transfer func
H = ((Rl*Rf+Y)/(Y*Rs*Cs))*(s+W*wz)/((s+wp)*(s+wz)+X*s);
disp("Transfer function (s-domain)");
disp(collect(H, s));

% bilinear transform
% syms z;
% Hz = subs(H, s, 2*fs*(z-1)/(z+1));
% disp("Transfer function (z-domain)");
% disp(collect(Hz, z));

% component values
Rf_val = 1e3;
Rz_val = 220;
Cz_val = 0.22e-6;
Ri_val = 10e3;
Rs_val = 1e3;
Cs_val = 0.22e-6;
Rl_val = T*20e3; % left side of potentiometer
Rr_val = (1-T)*20e3; % right side of potentiometer

% Hz_withvals = subs(Hz, [Rl, Rr, Rf, Rz, Cz, Ri, Rs, Cs], ...
%                       [Rl_val, Rr_val, Rf_val, Rz_val, Cz_val, Ri_val, Rs_val, Cs_val]);
% 
% disp("Transfer function (z-domain with values)")
% disp(collect(simplify(Hz_withvals), z))

% plot on that thang
% syms w real
% T_vals = 0:(1/4):1;  % T values from 0 to 1 with a 0.2 step
% figure;
% hold on;
% for T_val = T_vals
%     % Substitute z = exp(1i*w) and T = T_val into Hz_withvals
%     H_ejw = subs(Hz_withvals, [z, T], [exp(1i*w), T_val]);
%     % Compute the magnitude response
%     H_mag = abs(H_ejw);
%     % Plot the amplitude response from 0 to pi
%     fplot(H_mag, [0, pi], 'DisplayName', sprintf('T = %.1f', T_val));
% end
% hold off;
% xlabel('w (rad/sample)')
% ylabel('|H(e^{jw})|')
% title('Amplitude Response for different T values')
% legend('show')
% grid on

H_withvals = subs(H, [Rl, Rr, Rf, Rz, Cz, Ri, Rs, Cs], ...
                      [Rl_val, Rr_val, Rf_val, Rz_val, Cz_val, Ri_val, Rs_val, Cs_val]);
% T_vals = 0:(1/4):1;  % T values from 0 to 1 with a 0.2 step
% figure;
% hold on;
% for T_val = T_vals
%     syms w real;
%     % Substitute z = exp(1i*w) and T = T_val into Hz_withvals
%     H_jw = subs(H_withvals, [s, T], [1i*w, T_val]);
%     % Compute the magnitude response
%     H_mag = abs(H_jw);
%     % Plot the amplitude response from 0 to pi
%     fplot(H_mag, [0, 44.1e3/2], 'DisplayName', sprintf('T = %.1f', T_val));
% end
% hold off;
% xlabel('Omega (Hz)')
% ylabel('|H(jw)|')
% title('Amplitude Response (continuous) for different T values')
% legend('show')
% grid on


% Get TF and SOS for different T vals
N = 100;
g_vals = zeros(N); % dc gain
T_vals = zeros(N);
for n=1:N
T_val = (n-1)/(N-1);
[H_tf, sos, g] = getFilterFromT(T_val, H_withvals, T);
% disp('T');
% disp(T_val);
% disp('Transfer Function:');
% disp(H_tf.Numerator);
% disp(H_tf.Denominator);
% disp('Second Order Sections (SOS):');
% disp(sos);
% disp('Overall gain:');
% disp(g);
T_vals(n) = T_val;
g_vals(n) = g;
end
plot(T_vals, g_vals);

%% Local function (place this at the end of your script file)
function [H_tf, sos, g] = getFilterFromT(T_val, H_expr, T_sym)
    % Substitute the numeric T value into the symbolic transfer function
    H_numeric = subs(H_expr, T_sym, T_val);
    H_numeric = simplify(expand(H_numeric));
    
    % Extract numerator and denominator polynomials in s
    [num_sym, den_sym] = numden(H_numeric);
    num_poly = sym2poly(num_sym);
    den_poly = sym2poly(den_sym);
    
    % Create a transfer function object
    H_tf = tf(num_poly, den_poly);
    
    % Convert the transfer function to second-order sections (SOS)
    [sos, g] = tf2sos(num_poly, den_poly);
end
