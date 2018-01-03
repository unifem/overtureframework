%
% Solve the Newton Euler equations with a 4th order Runge Kutta
%
% -- this file is used in rigidBody.m


  if dt0 > 0.
    dt=dt0;
  else
    % Guess dt: err = K T dt^4 
    dt = cfl*(rtol/tf)^(1./4.); 
  end;

  n = round( tf/dt + 1.5 ) +1;
  if dt0 <0. 
    dt = tf/(n-1); 
  end;

  fprintf('RK4: n=%d, dt=%8.2e\n',n,dt);

  % allocate arrays to hold the solution
  numberOfComponents = size(wv0,1);
  tv = zeros(n,1);
  wv = zeros(n,numberOfComponents);

  t=0.; 
  yv = wv0;
  wv(1,1:numberOfComponents)=yv; tv(1)=t; % save solution 
  for i=2:n
  
    yv = rungeKutta4( 'wdot1', yv, t, dt );
    t = t+dt;
  
    tv(i)=t;  wv(i,1:numberOfComponents)=yv; % save solution 

    % normalize E and the Quaternion:
    normalizeRotation;
  
  end;
  fprintf('RK4: done. t=%9.3e\n',t);
