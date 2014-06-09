%
% Compute the time derivative of the Quaternion
%
function qDot = getQdot( omegav, qv )

  qDot = zeros(4,1);    
  qDot(1) = - .5*(omegav(1)*qv(2)+omegav(2)*qv(3)+omegav(3)*qv(4));
  qDot(2) = .5*( qv(1)*omegav(1) + omegav(2)*qv(4)-omegav(3)*qv(3) );
  qDot(3) = .5*( qv(1)*omegav(2) + omegav(3)*qv(2)-omegav(1)*qv(4) );
  qDot(4) = .5*( qv(1)*omegav(3) + omegav(1)*qv(3)-omegav(2)*qv(2) );
  % qDot=qDot'; 

  % fprintf('getQdot: qDot:\n');
  % qDot
  % pause;

  % size(qDot)
  % size(qv)
  % pause

  qDot = qDot - (qDot'*qv)*qv; % qDot.qv should be zero