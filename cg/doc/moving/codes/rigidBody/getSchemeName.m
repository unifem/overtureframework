function solverName = getSchemeName()

globalDeclarations;  % declare global variables here 

if solver==ode45Solver
  solverName='ode45';
elseif solver==rk4Solver
  solverName='rk4';
elseif solver==leapfrogTrapPC 
  solverName='leapfrogTrapPC';
elseif solver==AMIleapfrogTrapPC
  solverName='AMIleapfrogTrapPC';
  implicitSolver=1;
elseif solver==DIRK
  solverName=sprintf('DIRK%d',dirkDefaultOrder);
  implicitSolver=1;
else
  solverName='unknown'; 
end;
