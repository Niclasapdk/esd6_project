%----------------------Ser Class------------------------
classdef ser < Adaptor % the class for series 3-port adaptors
properties
WD % this is the down-going wave at the adapted port
WU % this is the up-going wave at the adapted port
leftReflect
end
methods
function obj = ser(KidLeft,KidRight) % constructor function
obj.KidLeft = KidLeft; % connect the left ’child’
obj.KidRight = KidRight; % connect the right ’child’
obj.PortRes = KidLeft.PortRes+KidRight.PortRes; % adapt. port
obj.leftReflect = KidLeft.PortRes/obj.PortRes;
end
function WU = WaveUp(obj) % the up-going wave at the adapted port
WU = -(WaveUp(obj.KidLeft)+WaveUp(obj.KidRight)); % wave up
obj.WU = WU;
end
function set.WD(obj,WaveFromParent) % sets the down-going wave
% set the waves to the ’children’ according to the scattering rules
b1 = obj.KidLeft.WU - obj.leftReflect * (WaveFromParent + obj.KidLeft.WU + obj.KidRight.WU);
b2 = - (WaveFromParent + b1);
set(obj.KidLeft,'WD',b1);
set(obj.KidRight,'WD',b2);
obj.WD = WaveFromParent; % set the down-going wave for the adaptor
end
end
end
