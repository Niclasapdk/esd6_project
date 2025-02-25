%----------------------Par Class------------------------
classdef par < Adaptor % the class for series 3-port adaptors
properties
WD % this is the down-going wave at the adapted port
WU % this is the up-going wave at the adapted port
leftReflect
end
methods
function obj = par(KidLeft,KidRight) % constructor function
obj.KidLeft = KidLeft; % connect the left ’child’
obj.KidRight = KidRight; % connect the right ’child’
lr = KidLeft.PortRes;
rr = KidRight.PortRes;
obj.PortRes = lr*rr/(lr+rr); % adapt. port
obj.leftReflect = Conductance(obj.KidLeft)/Conductance(obj);
end
function WU = WaveUp(obj) % the up-going wave at the adapted port
WaveUp(obj.KidLeft);
WaveUp(obj.KidRight);
bdiff = obj.KidRight.WU - obj.KidLeft.WU;
WU = obj.KidRight.WU - obj.leftReflect * bdiff;
obj.WU = WU;
end
function set.WD(obj,WaveFromParent) % sets the down-going wave
% set the waves to the ’children’ according to the scattering rules
bdiff = obj.KidRight.WU - obj.KidLeft.WU;
b2 = obj.WU - obj.KidRight.WU + WaveFromParent;
b1 = b2 + bdiff;
set(obj.KidLeft,'WD',b1);
set(obj.KidRight,'WD',b2);
obj.WD = WaveFromParent; % set the down-going wave for the adaptor
end
end
end