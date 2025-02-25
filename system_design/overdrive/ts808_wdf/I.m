%----------------------I Class------------------------
classdef I < OnePort % class for the WDF current source (and par. res.)
properties
E % this is the source current
end
methods
function obj = I(E,PortRes) % constructor function
obj.E = E; % set the source current
obj.PortRes = PortRes; % set the port resistance
obj.WD = 0; % initial value for the incoming wave
end
function WU = WaveUp(obj) % evaluate the up-going wave
WU = obj.E * obj.PortRes; % from the def. of the WDF current source
obj.WU = WU;
end
end
end