%
% Solve the Newton Euler equations using the:
% --- Added-mass-implicit Leapfrog predictor, trapezoidal corrector  ------
%  *new* solver that treats added mass matricies implicitly
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

  fprintf('AMILeapfrogTrap: n=%d, dt=%8.2e\n',n,dt);

  % allocate arrays to hold the solution
  numberOfComponents = size(wv0,1);
  tv = zeros(n,1);
  wv = zeros(n,numberOfComponents);

  t=0.; 
  yv = wv0;
  wv(1,1:numberOfComponents)=yv; tv(1)=t; % save solution 

  % -- first compute an approximation to w(-dt) using RK4   **FIX ME**
  implicitSolverSave=implicitSolver; implicitSolver=0;  % set solver to explicit
  yvm = rungeKutta4( 'wdot1', yv, t, -dt );
  implicitSolver=implicitSolverSave; % reset

  for i=2:n
  
    % ---- predictor: -----
    %  Note: vm = v(t-dt)

    [ xvm, vvm, hvm, eam, omegavm, qvm ] = arrayToState( yvm );  % solution at t-dt
    [ xvn, vvn, hvn, ean, omegavn, qvn ] = arrayToState( yv  );  % solution at t

    % get added mass matrices at time t
    [A11n , A12n , A21n, A22n ] = getAddedMassMatrices( t );

    fvn = force(t,vvn,omegavn,ean);  % get force
    gvn = torque(t,vvn,omegavn,ean); % get torque

    % predictor for x:
    xvp = xvm + (2.*dt)*vvn;  

    % predictor for h:
    hvp = hvm + (2.*dt)*(gvn -A21n*vvn -A22n*omegavn); % NOTE: for implicit solvers gvn does not include mass matrices

    % predictor for E and q:
    Omegan = [ 0. -omegavn(3) omegavn(2); omegavn(3) 0. -omegavn(1); -omegavn(2) omegavn(1) 0. ];
    eap = eam + (2.*dt)*( Omegan*ean );

    % fprintf('qvn:\n');
    % qvn
    %  pause

    qDotn = getQdot( omegavn, qvn );
    qvp = qvm + (2.*dt)*( qDotn );
    % fprintf('qvp:\n');
    % qvp
    %  pause

    % -- predictor for v and omega:
    %   m ( vp - vm )/( 2*dt ) = - A11( vp+vm )/2 - A12*( omegap + omegam)/2 + fn
    %  An*( omegap - omegam)/( 2*dt ) = - A21( vp+vm )/2 - A22*( omegap + omegam)/2 + gn

    An = ean*Lambda*ean'; % inertia matrix A(t)

    % Matrix B holds the coupled system matrix for v and omega
    B = zeros( 6, 6);
    B(1:3,1:3) = mass*eye(3,3) + dt*A11n; B(1:3,4:6) = dt*A12n;
    B(4:6,1:3) = dt*A21n;                 B(4:6,4:6) = An + dt*A22n;

    rhs = zeros(6,1);
    rhs(1:3) = mass*vvm   - dt*( A11n*vvm + A12n*omegavm ) + (2.*dt)*fvn;
    rhs(4:6) = An*omegavm - dt*( A21n*vvm + A22n*omegavm ) + (2.*dt)*(gvn -Omegan*An*omegavn);

    % fprintf('rhs:\n');
    % rhs
    % pause;

    % Solve the implicit system:
    soln = zeros(6,1); % solution  B*soln = rhs
    soln = B\rhs;

    % fprintf('soln:\n');
    % soln
    % pause;

    vvp=zeros(3,1); omegavp=zeros(3,1);
    vvp(1:3)= soln(1:3);
    omegavp(1:3)= soln(4:6);
    
    % fprintf('vvp:\n');
    % vvp
    % pause;

    % --- corrector (Trapezodial rule)----
    %   m ( vc - vn )/dt = - A11h( vc+vn )/2 - A12h*( omegac + omegan)/2 + .5*(fp+fn)
    %  Ah*( omegac - omegan)/( dt ) = - A21h( vc+vn )/2 - A22h*( omegac + omegan)/2 + .5*(gp+gn)
    %  where
    %      Ah = .5*( Ap+an) , A11h=.5*( A11p +A11n)
    
if 1==1
    % get added mass matrices at time t+dt 
    [A11p, A12p, A21p, A22p ] = getAddedMassMatrices( t+dt );

    A11h = .5*( A11p + A11n );  A12h = .5*( A12p + A12n );
    A21h = .5*( A21p + A21n );  A22h = .5*( A22p + A22n );

    fvp = force(t+dt,vvp,omegavp,eap);  % get force
    gvp = torque(t+dt,vvp,omegavp,eap); % get torque

    % Note -- save corrector values in predictor variables (so we can iterate if need be)

    % corrector for x:
    xvp = xvn + (.5*dt)*(vvp + vvn);  

    % corrector for h:
    hvp = hvn + (.5*dt)*(gvp -A21p*vvp -A22p*omegavp + gvn -A21n*vvn -A22n*omegavn);  

    % Corrector for E and q:  
    Omegap = [ 0. -omegavp(3) omegavp(2); omegavp(3) 0. -omegavp(1); -omegavp(2) omegavp(1) 0. ];
    eap = ean + (.5*dt)*( Omegap*eap + Omegan*ean );

    qDotp = getQdot( omegavp, qvp );
    qvp = qvn + (.5*dt)*( qDotp + qDotn );

    % NOTE: these next use the corrected values for omegavp and eap:
    Ap = eap*Lambda*eap'; 
    Ah = .5*( Ap + An );

    dth = .5*dt;
    B(1:3,1:3) = mass*eye(3,3) + dth*A11h;   B(1:3,4:6) = dth*A12h;
    B(4:6,1:3) = dth*A21h;                   B(4:6,4:6) = Ah + dth*A22h;


    rhs(1:3) = mass*vvn   - dth*( A11h*vvn + A12h*omegavn ) + dth*(fvp+fvn);
    rhs(4:6) = Ah*omegavn - dth*( A21h*vvn + A22h*omegavn ) + dth*(gvp+gvn -Omegap*Ap*omegavp -Omegan*An*omegavn );

    % Solve the implicit system:
    soln = B\rhs;
    vvp(1:3)= soln(1:3);
    omegavp(1:3)= soln(4:6);
    

end; 


    % Update the current state:
    yvm=yv;
    yv = stateToArray( xvp, vvp, hvp, eap, omegavp, qvp );
    t = t+dt;
  
    tv(i)=t;  wv(i,1:numberOfComponents)=yv; % save solution 

    % normalize E and the Quaternion:
    normalizeRotation;
  
  end;
  fprintf('AMI-LeapfrogTrap: done. t=%9.3e\n',t);
