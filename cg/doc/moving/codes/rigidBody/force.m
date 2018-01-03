%
% Netwon Euler force
%
%
function f = force(t,vv,omegav,ea)

globalDeclarations;  % declare global variables here 

if addedMass==1
  [A11 , A12 , A21, A22 ] = getAddedMassMatrices( t );
end;

if itest==polynomialForcing
  f = [pf(1,1) + t*(pf(1,2) + t*(pf(1,3))); pf(2,1) + t*(pf(2,2) + t*(pf(2,3))); pf(3,1) + t*(pf(3,2) + t*(pf(3,3))) ];

elseif itest==freeRotation
  f = [0; 0; 0];

elseif itest==twilightZoneSolution
  % twilightzone forcing:

  % evaluate the time derivative of the exact solution:
  [xvDotExact, vvDotExact, omegavDotExact ] = getExactSolution( 1, t ); 

  f = mass*vvDotExact; 

  if addedMass==1 
    [xvExact, vvExact, omegavExact ] = getExactSolution( 0, t ); 
    f = f + A11*vvExact + A12*omegavExact;

  end;

else
  fprintf('torque:ERROR: invalid itest=%d\n',itest);
  pause;
end;


if addedMass==1 & implicitSolver==0
  % include added mass matrices (if the solver is explicit)
  %fprintf('force .. added mass\n');

  f = f - A11*vv - A12*omegav;

end;

