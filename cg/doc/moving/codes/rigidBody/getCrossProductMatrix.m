%
% Compute the matrix correspoding to the cross product of a vector 
%       Omega = [ omegav X ]
%
function Omega = getCrossProductMatrix( omegav )

Omega = [ 0. -omegav(3) omegav(2); omegav(3) 0. -omegav(1); -omegav(2) omegav(1) 0. ];