%
%  Convert the vector wv(1:3*4+9) into the state xv(1:3), vv(1:3), hv(1:3) ea(1:3,1:3), omegav(1:3) 
% 
function [ xv, vv, hv, ea, omegav, qv] = arrayToState( wv )

m=1;
xv = wv(m:m+2); m=m+3;
vv = wv(m:m+2); m=m+3;
hv = wv(m:m+2); m=m+3;

ea = zeros(3,3);

for j=1:3
  ea(1:3,j) = wv(m:m+2); m=m+3;
end;

omegav = wv(m:m+2); m=m+3;

qv = wv(m:m+3); m=m+4;