%
% Netwon Euler version 1 : 
%   Advance [ x, v, h, E ]
%      w = A^{-1} h 
%
%
function wdot = wdot1(t,wv)

globalDeclarations;  % declare global variables here 


[ xv, vv, hv, ea, omegav, qv ] = arrayToState( wv );

% Omega is the matrix form of "omegav X"
Omega = [ 0. -omegav(3) omegav(2); omegav(3) 0. -omegav(1); -omegav(2) omegav(1) 0. ];


% A = E*Lambda*E^T
A = ea*Lambda*ea'; 
Ai = inv(A); 

fv = force(t,vv,omegav,ea); % get force
gv = torque(t,vv,omegav,ea); % get torque

xp = vv;
vp = fv/mass;

hp = gv;

eap  = Omega*ea;
% for i=1:3
%   eap(1:3,i) = eap(1:3,i) - (eap(1:3,i)'*ea(1:3,i))*ea(1:3,i); % 
% end;


% hv = A*omegav 
% omegap  = Ai*( -Omega*hv + gv );
omegap  = Ai*( -Omega*A*omegav + gv );

wdot = zeros(25,1);

% Quaternion:  [s1,v1] x [s2,v2] = [s1*s2-v1.v2, s1*v2+s2*v1+v1Xv2]
% q' = .5*[ 0, omegav] x qv 

qp(1) = - .5*(omegav(1)*qv(2)+omegav(2)*qv(3)+omegav(3)*qv(4));
% qp(2:4) = .5*( qv(1)*omegav(1:3) + ...
%           [ omegav(2)*qv(4)-omegav(3)*qv(3); omegav(3)*qv(2)-omegav(1)*qv(4); omegav(1)*qv(3)-omegav(2)*qv(2)] );

qp(2) = .5*( qv(1)*omegav(1) + omegav(2)*qv(4)-omegav(3)*qv(3) );
qp(3) = .5*( qv(1)*omegav(2) + omegav(3)*qv(2)-omegav(1)*qv(4) );
qp(4) = .5*( qv(1)*omegav(3) + omegav(1)*qv(3)-omegav(2)*qv(2) );

qp=qp'; 

qp = qp - (qp'*qv)*qv; % qp.qv should be zero


% fprintf('wdot:\n');
% xp
% vp
% hp

wdot = stateToArray( xp, vp, hp, eap, omegap, qp );