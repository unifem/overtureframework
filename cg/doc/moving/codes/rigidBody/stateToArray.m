%
%  Convert the state xv(1:3), vv(1:3), hv(1:3) ea(1:3,1:3), omegav(1:3) qv(1:4) into a vector wv(1:3*4+9+4)
% 
function wv = stateToArray( xv, vv, hv, ea, omegav, qv )

% fprintf('stateToArray:\n');
% xv
% vv
% hv

wv = [ xv; vv; hv; ea(1:3,1); ea(1:3,2); ea(1:3,3); omegav; qv ];
