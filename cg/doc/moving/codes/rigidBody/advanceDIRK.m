%
% Solve the Newton Euler equations using a diagonally implicit Runge-Kutta scheme
%
% -- this file is used in rigidBody.m


  if dt0 > 0.
    dt=dt0;
  else 
    % Guess dt: err = K T dt^2
    dt = 5.*cfl*(rtol/tf)^(1./2.); 
    if dirkOrder==3
     dt = cfl*(rtol/tf)^(1./dirkOrder); 
    elseif dirkOrder==4
     dt = .5*cfl*(rtol/tf)^(1./dirkOrder); 
    elseif dirkOrder>4 
     dt = .5*cfl*(rtol/tf)^(1./dirkOrder); 
    end; 
  end;
  % number of steps:
  n = round( tf/dt + 1.5 );
  if dt0 <0. 
    dt = tf/(n-1); 
  end;

  fprintf('DIRK: dirkOrder=%d, n=%d, dt=%8.2e\n',dirkOrder,n,dt);

  % allocate arrays to hold the solution
  numberOfComponents = size(wv0,1);
  tv = zeros(n,1);
  wv = zeros(n,numberOfComponents);

  t=0.; 
  yv = wv0;
  wv(1,1:numberOfComponents)=yv; tv(1)=t; % save solution 

  eak=zeros(3,3);
  % yvn=zeros(numberOfComponents,1);

  for i=2:n
  
    yv = timeStepDirk( yv, t, dt, dirkOrder );
    t = t+dt;
    tv(i)=t;  wv(i,1:numberOfComponents)=yv; % save solution 

  end;

  fprintf('DIRK: dirkOrder=%d, n=%d, dt=%8.2e, ... DONE.\n',dirkOrder,n,dt);