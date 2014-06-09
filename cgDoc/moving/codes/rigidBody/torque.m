%
% Netwon Euler torque
%
%
function g = torque(t,vv,omegav,ea)

globalDeclarations;  % declare global variables here 

if addedMass==1
  [A11 , A12 , A21, A22 ] = getAddedMassMatrices( t );
end;

if itest==polynomialForcing

  g = [pg(1,1) + t*(pg(1,2) + t*(pg(1,3))); pg(2,1) + t*(pg(2,2) + t*(pg(2,3))); pg(3,1) + t*(pg(3,2) + t*(pg(3,3))) ];

elseif itest==freeRotation
  
  g = [.0; 0.; .0];

elseif itest==twilightZoneSolution
  % twilightzone forcing:

  % evaluate the exact solution and it's time derivative
  [xvExact, vvExact, omegavExact ] = getExactSolution( 0, t ); 
  [xvDotExact, vvDotExact, omegavDotExact ] = getExactSolution( 1, t ); 

  % hvExact = A*omegavExact  : we use computed A with exact omega
  A = ea*Lambda*ea';
  % Omega = [ 0. -omegav(3) omegav(2); omegav(3) 0. -omegav(1); -omegav(2) omegav(1) 0. ];
  Omega = getCrossProductMatrix( omegavExact );
  % A' = Omega*A - A*Omega  (assume Omega*omegav=0 : omegav X omegav = 0 )
  hvDotExact = A*omegavDotExact + Omega*A*omegavExact; 

  g = hvDotExact; 

  if addedMass==1 
    g = g + A21*vvExact + A22*omegavExact;
  end;

else
  fprintf('torque:ERROR: invalid itest=%d\n',itest);
  pause;
end;


if addedMass==1 & implicitSolver==0
  % include added mass matrices (if the solver is explicit)

  g = g - A21*vv - A22*omegav;

end;