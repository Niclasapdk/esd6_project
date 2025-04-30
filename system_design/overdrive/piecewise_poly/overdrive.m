function y = overdrive(x)
% OVERDRIVE  Sample-by-sample DSP guitar overdrive
%    y = OVERDRIVE(x) applies:
%      1) FIR anti-alias LP
%      2) IIR high-pass (rumble removal)
%      3) piecewise nonlinearity
%      4) IIR low-pass (output smoothing)
%
%   Call with a vector of samples like:
%     y = arrayfun(@overdrive, x_in);
%
persistent fir_taps fir_buf fir_idx
persistent hp_b hp_a hp_xz hp_yz
persistent lp_b lp_a lp_xz lp_yz
persistent init
fs = 44100;        % sampling frequency
hp_fc   = 200;     % high-pass corner freq (Hz)
lp_fc   = 3000;    % low-pass corner freq (Hz)
hp_ord  = 2;       % HP filter order
lp_ord  = 2;       % LP filter order
gain = 40; % between 1 and 50
level = 1; % between 0 and 1

if isempty(init)
    %% 1) Anti-alias FIR LP taps
    fir_taps = [0.0030129264015200906 0.02102474903640835 0.0521123744290017 0.06226466448821925 0.012792216446345376 -0.06731840417038246 -0.06969755244058952 0.08102727944068425 0.3048380521496307 0.41302310351443294 0.3048380521496307 0.08102727944068425 -0.06969755244058952 -0.06731840417038246 0.012792216446345376 0.06226466448821925 0.0521123744290017 0.02102474903640835 0.0030129264015200906];
    N = numel(fir_taps);
    fir_buf = zeros(1,N);
    fir_idx = 1;

    %% 2) IIR high-pass (2nd order) coefficients (manual)
    % assume fs, hp_fc and hp_ord (=2) are already defined
    Wn_hp = tan(pi*hp_fc/fs);
    D_hp  = 1 + sqrt(2)*Wn_hp + Wn_hp^2;
    hp_b  = [ 1, -2,  1 ] ./ D_hp;
    hp_a  = [ 1,  2*(Wn_hp^2-1),  (1 - sqrt(2)*Wn_hp + Wn_hp^2) ] ./ D_hp;

    hp_xz = zeros(1,2);   % x[n-1], x[n-2]
    hp_yz = zeros(1,2);   % y[n-1], y[n-2]

    %% 4) IIR low-pass (2nd order) coefficients (manual)
    % assume fs, lp_fc and lp_ord (=2) are already defined
    Wn_lp = tan(pi*lp_fc/fs);
    D_lp  = 1 + sqrt(2)*Wn_lp + Wn_lp^2;
    lp_b  = [ Wn_lp^2,  2*Wn_lp^2,  Wn_lp^2 ] ./ D_lp;
    lp_a  = [ 1,  2*(Wn_lp^2-1),  (1 - sqrt(2)*Wn_lp + Wn_lp^2) ] ./ D_lp;

    lp_xz = zeros(1,2);   % x[n-1], x[n-2]
    lp_yz = zeros(1,2);   % y[n-1], y[n-2]


    init = true;
end

%% 1) FIR anti-aliasing LP (circular buffer)
fir_buf(fir_idx) = x;
y1 = 0;
for k = 1:numel(fir_taps)
    idx = fir_idx - k + 1;
    if idx < 1, idx = idx + numel(fir_taps); end
    y1 = y1 + fir_taps(k) * fir_buf(idx);
end
fir_idx = mod(fir_idx, numel(fir_taps)) + 1;
% y1 = x; % dummy no fir filter dry run

%% 2) IIR high-pass
y2 = hp_b(1)*y1 + hp_b(2)*hp_xz(1) + hp_b(3)*hp_xz(2) ...
     - hp_a(2)*hp_yz(1) - hp_a(3)*hp_yz(2);
% shift delays
hp_xz(2) = hp_xz(1);  hp_xz(1) = y1;
hp_yz(2) = hp_yz(1);  hp_yz(1) = y2;

%% 3) Nonlinear piecewise distortion
% TODO: replace this with your piecewise mapping on abs(y2)
%   e.g. if a<1/3, f(a)=...; elseif a<2/3, f(a)=...; else f(a)=...
a = gain*y2;
sign_a = sign(a);
u = abs(a);
if u < 1/3
    v = 2*a;
elseif u < 2/3
    v = sign_a * (3 - (2 - 3*u) * (2 - 3*u))/3;
else
    v = sign_a;
end
y3 = v;

%% 4) IIR low-pass
y4 = lp_b(1)*y3 + lp_b(2)*lp_xz(1) + lp_b(3)*lp_xz(2) ...
     - lp_a(2)*lp_yz(1) - lp_a(3)*lp_yz(2);
% shift delays
lp_xz(2) = lp_xz(1);  lp_xz(1) = y3;
lp_yz(2) = lp_yz(1);  lp_yz(1) = y4;

y = level*y4;
end