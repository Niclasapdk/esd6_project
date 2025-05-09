%----------------------WDF Class------------------------
classdef WDF < hgsetget % the WDF element superclass
properties
PortRes % the WDF port resistance
end
methods
function Volts = Voltage(obj) % the voltage (V) over a WDF element
Volts = (obj.WU+obj.WD)/2; % as defined in the WDF literature
end
function G = Conductance(obj)
    G = 1/obj.PortRes;
end
end
end