function result = fast_omega(x)
% FAST_OMEGA Approximates the Wright Omega function using a fourth-order scheme.
%
%   result = FAST_OMEGA(x) returns the approximated value of the Wright Omega
%   function for input x.
%
%   This implementation uses Estrin's scheme for polynomial evaluation and
%   bit-level tricks to approximate log and exp.
%
%   Example:
%       y = fast_omega(2.5);

    result = omega4(x);
end

%--------------------------------------------------------------------------

function y = estrin(coeffs, x)
% ESTRIN Recursively evaluates a polynomial using Estrin's scheme.
%
%   coeffs is a vector of coefficients arranged as
%       [ a_n, a_(n-1), ..., a_1, a_0 ]
%   so that the polynomial is
%       P(x) = a_n*x^n + a_(n-1)*x^(n-1) + ... + a_1*x + a_0.
%
%   This function recursively reduces the polynomial order.

    order = length(coeffs) - 1;
    if order == 1
        y = coeffs(2) + coeffs(1) * x;
        return;
    end
    
    nTerms = floor(order/2) + 1;
    temp = zeros(nTerms, 1);
    
    if mod(order,2) == 0
        for n = order:-2:2
            idx = floor(n/2) + 1;
            temp(idx) = coeffs(n+1) + coeffs(n) * x;
        end
        temp(1) = coeffs(1);
    else
        for n = order:-2:1
            idx = floor(n/2) + 1;
            temp(idx) = coeffs(n+1) + coeffs(n) * x;
        end
    end
    
    y = estrin(temp, x.^2);
end

%--------------------------------------------------------------------------

function y = log2_approx(x)
% LOG2_APPROX Approximates log2(x) on the range [1,2] via a cubic polynomial.
%
%   Uses the coefficients:
%       [0.1640425613334452, -1.098865286222744, 3.148297929334117, -2.213475204444817].

    coeffs = [0.1640425613334452, -1.098865286222744, ...
              3.148297929334117, -2.213475204444817];
    y = estrin(coeffs, x);
end

%--------------------------------------------------------------------------

function y = log_approx(x)
% LOG_APPROX Approximates the natural logarithm using a bit-level trick.
%
%   This function extracts the exponent and mantissa (normalized) parts of x,
%   applies log2_approx to the mantissa, and then combines them.

    % Reinterpret the double as a 64-bit unsigned integer.
    v = typecast(x, 'uint64');
    % Extract exponent bits (mask: 0x7ff0000000000000).
    ex = bitand(v, hex2dec('7ff0000000000000'));
    % Shift right by 53 bits and subtract 510.
    e = double(bitshift(ex, -53)) - 510;
    % Remove exponent bits and set exponent to 0x3ff.
    v_new = bitor(v - ex, hex2dec('3ff0000000000000'));
    m = typecast(v_new, 'double');
    y = 0.693147180559945 * (e + log2_approx(m));
end

%--------------------------------------------------------------------------

function y = pow2_approx(x)
% POW2_APPROX Approximates 2^x on the range [0,1] using a cubic polynomial.
%
%   Uses the coefficients:
%       [0.07944154167983575, 0.2274112777602189, 0.6931471805599453, 1.0].

    coeffs = [0.07944154167983575, 0.2274112777602189, ...
              0.6931471805599453, 1.0];
    y = estrin(coeffs, x);
end

%--------------------------------------------------------------------------

function y = exp_approx(x)
% EXP_APPROX Approximates exp(x) using a scaling trick and pow2_approx.
%
%   The argument is first scaled so that bit-level operations can be used
%   to form the correct exponent bits.

    x = max(-126.0, 1.442695040888963 * x);
    l = fix(x);
    if isscalar(x)
        if x < 0
            l = l - 1;
        end
    else
        negIdx = x < 0;
        l(negIdx) = l(negIdx) - 1;
    end
    d = x - l;
    % Compute bit pattern for the exponent: (l+1023) shifted left by 52.
    expBits = bitshift(uint64(l + 1023), 52);
    v = typecast(expBits, 'double');
    y = v .* pow2_approx(d);
end

%--------------------------------------------------------------------------

function y = omega1(x)
% OMEGA1 First-order approximation of the Wright Omega function.
%
%   Simply returns max(x, 0).

    y = max(x, 0);
end

%--------------------------------------------------------------------------

function y = omega2(x)
% OMEGA2 Second-order approximation of the Wright Omega function.
%
%   For x < -3.684303659906469, returns 0.
%   For x > 1.972967391708859, returns x.
%   Otherwise, evaluates a cubic polynomial.

    x1 = -3.684303659906469;
    x2 = 1.972967391708859;
    a = 9.451797158780131e-3;
    b = 1.126446405111627e-1;
    c = 4.451353886588814e-1;
    d = 5.836596684310648e-1;
    
    y = zeros(size(x));
    y(x < x1) = 0;
    y(x > x2) = x(x > x2);
    
    idx = (x >= x1) & (x <= x2);
    if any(idx)
        y(idx) = estrin([a, b, c, d], x(idx));
    end
end

%--------------------------------------------------------------------------

function y = omega3(x)
% OMEGA3 Third-order approximation of the Wright Omega function.
%
%   For x < -3.341459552768620, returns 0.
%   For x < 8, returns a cubic polynomial.
%   For x >= 8, returns x - log_approx(x).

    x1 = -3.341459552768620;
    x2 = 8.0;
    a = -1.314293149877800e-3;
    b = 4.775931364975583e-2;
    c = 3.631952663804445e-1;
    d = 6.313183464296682e-1;
    
    y = zeros(size(x));
    y(x < x1) = 0;
    
    idx1 = (x >= x1) & (x < x2);
    if any(idx1)
        y(idx1) = estrin([a, b, c, d], x(idx1));
    end
    
    idx2 = x >= x2;
    if any(idx2)
        y(idx2) = x(idx2) - log_approx(x(idx2));
    end
end

%--------------------------------------------------------------------------

function y = omega4(x)
% OMEGA4 Fourth-order approximation of the Wright Omega function.
%
%   Improves on omega3 via one Newton iteration:
%       omega4 = y - (y - exp_approx(x-y))/(y+1)

    y_val = omega3(x);
    y = y_val - (y_val - exp_approx(x - y_val)) ./ (y_val + 1);
end