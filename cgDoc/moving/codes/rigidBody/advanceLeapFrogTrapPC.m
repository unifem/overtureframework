%
% Solve the Newton Euler equations using the Leapfrog predictor, trapezoidal corrector algorithm from Overture 
%
% -- this file is used in rigidBody.m

  if dt0 > 0.
    dt=dt0;
  else
    % Guess dt: err = K T dt^2
    dt = 5.*cfl*(rtol/tf)^(1./2.); 
  end;
  % number of steps:
  n = round( tf/dt + 1.5 ) +1;
  if dt0 <0. 
    dt = tf/(n-1); 
  end;

  fprintf('LeapfrogTrap: n=%d, dt=%8.2e\n',n,dt);

  % allocate arrays to hold the solution
  numberOfComponents = size(wv0,1);
  tv = zeros(n,1);
  wv = zeros(n,numberOfComponents);

  t=0.; 
  yv = wv0;
  wv(1,1:numberOfComponents)=yv; tv(1)=t; % save solution 

  % -- first compute an approximation to w(-dt) using RK4
  yvm = rungeKutta4( 'wdot1', yv, t, -dt );

  for i=2:n
  
    % predictor:
    wDotn = feval( 'wdot1', t,yv );  % dw(t)/dt 
    yvp = yvm + (2.*dt)*wDotn;       % Leapfrog 

    yvm=yv; % update old time solution
    % Corrector
    wDotp = feval( 'wdot1', t+dt, yvp ); % dw(t+dt)/dt
    yv = yv + (.5*dt)*( wDotp + wDotn);  % trapezodial

    t = t+dt;
  
    tv(i)=t;  wv(i,1:numberOfComponents)=yv; % save solution 

    % normalize E and the Quaternion:
    normalizeRotation;
  
  end;
  fprintf('LeapfrogTrap: done. t=%9.3e\n',t);
