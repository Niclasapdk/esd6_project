%% 1) Define symbolic variables
syms s z real
syms Rl Rf Rr Rs Cs Cz real
syms Rz real                % If needed
syms fs real                % Sample rate
T = 1/fs;                   % Sampling period

%% 2) Define intermediate terms from the paper
%   Y    = (Rl + Rr) * (Rz + parallel(Rl, Rr));
%   W    = Y / (Rl*Rf + Y);
%   X    = (Rr/(Rl + Rr)) * (1 / ((Rz + parallel(Rl,Rr))*Cz));
%
%   but we need a small helper for parallel(...) = (a*b)/(a+b).
%   Also define the pole/zero frequencies if you wish.

parallelRlRr = (Rl*Rr)/(Rl + Rr);
Y  = (Rl + Rr)*(Rz + parallelRlRr);
W  = Y/(Rl*Rf + Y);
X  = (Rr/(Rl + Rr)) * (1/((Rz + parallelRlRr)*Cz));

%  The paper’s formula suggests something like:
%  H(s) = ((Rl*Rf + Y)/(Y*Rs*Cs)) * (s + W*omega_z) / ((s + omega_p)*(s + omega_z) + X*s)
%  but you might also define omega_z, omega_p as 1/(Cz(...)), etc. 
%  For clarity, let's define them too:

syms omega_z omega_p real
% Typically:
% omega_z = 1 / ( Cz*( Rz + parallel(Rl,Rr)) );
% omega_p = 1 / ( Cs*parallel(Rs, Ri) );  (Ri might also be a param)

% For now we assume you've already defined them or treat them as symbolic:
H_s_num = (Rl*Rf + Y)/(Y*Rs*Cs) * ( s + W*omega_z );
H_s_den = (s + omega_p)*(s + omega_z) + X*s;

H_s = simplify( H_s_num / H_s_den );

%% 3) Apply the bilinear transform:  s -> (2/T)*((1 - z^-1)/(1 + z^-1))
s_bilin = (2/T)*((1 - z^(-1)) / (1 + z^(-1)));

H_z = simplify( subs(H_s, s, s_bilin) );

%% 4) Convert H(z) into a rational function of z^-1 and read off difference eq.
% Multiply top and bottom so we have polynomials in z^-1 only:

[num_z, den_z] = numden(H_z);           % symbolic numerator & denominator
% Clear out any common factors, then collect terms in z^-1:
num_z_collected = collect(num_z, z^(-1));
den_z_collected = collect(den_z, z^(-1));

%% (Optional) Get a polynomial form in z^-1
% so that H(z) = num_z_collected / den_z_collected
% Then a difference equation is:
% den_z_collected * y[n] = num_z_collected * x[n]
%
% For example, if den_z_collected = a0 + a1 z^-1 + a2 z^-2 + ...
% and num_z_collected = b0 + b1 z^-1 + b2 z^-2 + ...
% then the time-domain difference equation is:
%
% a0*y[n] + a1*y[n-1] + a2*y[n-2] + ... = b0*x[n] + b1*x[n-1] + ...

disp('H(z) numerator (collected in z^-1):');
disp(num_z_collected);
disp('H(z) denominator (collected in z^-1):');
disp(den_z_collected);
