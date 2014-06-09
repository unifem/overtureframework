%
% Take one time step of a diagonally implicit Runge-Kutta scheme
%
% NOTE:
%  The 2nd-order implicit mid point rule is a sympletic integrator
%  and thus the rotation matrix should remain orthogonal ??
% 
function yvn = timeStepDIRK( yv, t, dt, dirkOrder );

  globalDeclarations;  % declare global variables here 

  [ xvn, vvn, hvn, ean, omegavn, qvn ] = arrayToState( yv  );  % solution at t

  if dirkOrder==1 

    % ---- DIRK order 1 : Back Euler: -----
    %     y' = f(y,t)
    %     k1 = f(y(n)+dt*k1,t+dt) : implicit equation for k1
    %     y(n+1) = y(n) + dt*k1

    % To solve k1 = f(y(n)+dt*k1,t+dt), let y=y(n)+dt*k1, then
    %      y + dt*f(y,t+dt) = y(n) , k1=(y-yn)/dt 

    aii=1.; ci=1.; b1=1.; % Backward Euler
    yv0=yv;  % yv0=yn + dt*sum_j=0^{i-1} k_j
    kv1 = dirkImplicitSolve( dt,aii,t+ci*dt,yv,yv0);  % yv=initial guess

    % kv holds [ xvn, vvn, hvn, ean, omegavn, qvn ] 

    yvn = yv + dt*b1*( kv1 );

  elseif dirkOrder==2

    % ---- DIRK order 2 : Implicit mid-point --

    aii=.5; ci=.5; b1=1.; % Implicit mid-point

    yv0=yv;  % yv0=yn + dt*sum_j=0^{i-1} k_j
    kv1 = dirkImplicitSolve( dt,aii,t+ci*dt,yv,yv0);  % yv=initial guess

    yvn = yv + dt*b1*( kv1 );

  elseif dirkOrder==3

    % ---- DIRK two-stage order 3, A-stable 
    %   Formula from Crouzeix, cf. Alexander 1977 SIAM J. Anal. 14, no 6. 

    sqi3 = 1./sqrt(3.);

    a11=.5*(1.+sqi3);                    c1=.5*(1.+sqi3); b1=.5; 
    a21=-sqi3;        a22=.5*(1.+sqi3);  c2=.5*(1.-sqi3); b2=.5; 

    yv0=yv;  % yv0=yn + dt*sum_j=0^{i-1} k_j
    kv1 = dirkImplicitSolve( dt,a11,t+c1*dt,yv,yv0);  % yv=initial guess

    yv0=yv + dt*( a21*kv1 );
    kv2 = dirkImplicitSolve( dt,a22,t+c2*dt,yv,yv0);  % yv=initial guess  *FIX ME*

    yvn = yv + dt*( b1*kv1 + b2*kv2 );

  elseif dirkOrder==4

    % ---- DIRK 4-stage order 4, A0-stable 
    %   Formula from Jackson and Norsett (1990)   -- See: for A and L stable methods? 
    % Iserles, A. and Nørsett, S. P. (1990). On the Theory of Parallel Runge-Kutta Methods. IMA
    %   Journal of Numerical Analysis 10: 463-488.
 

    a11=1.;                                        c1=1.;     b1=11./72.; 
    a21=0.;       a22=3./5.;                       c2=3./5.;  b2=25./72.;
    a31=171./44.; a32=-215/44.; a33=1.;            c3=0.;     b3=11./72.;
    a41=-43./20.; a42=39./20.;  a43=0.; a44=3./5.; c4=2./5.;  b4=25./72.;

    yv0=yv;  % yv0=yn + dt*sum_j=0^{i-1} k_j
    kv1 = dirkImplicitSolve( dt,a11,t+c1*dt,yv,yv0);  % yv=initial guess

    yv0=yv + dt*( a21*kv1 );
    kv2 = dirkImplicitSolve( dt,a22,t+c2*dt,yv,yv0);  % yv=initial guess  *FIX ME*

    yv0=yv + dt*( a31*kv1 + a32*kv2 );
    kv3 = dirkImplicitSolve( dt,a33,t+c3*dt,yv,yv0);  % yv=initial guess  *FIX ME*

    yv0=yv + dt*( a41*kv1 + a42*kv2 + a43*kv3 );
    kv4 = dirkImplicitSolve( dt,a44,t+c4*dt,yv,yv0);  % yv=initial guess  *FIX ME*

    yvn = yv + dt*( b1*kv1 + b2*kv2 + b3*kv3 + b4*kv4 );

  else
    fprintf('advanceDIRK: ERROR: dirkOrder=%d NOT implemented\n');
    pause;
  end;

  if normalize==1 
    % Normalize the rotation matrix and the quaternion
    yvn = normalizeEandQ( yvn );
  end

  % Compute h directly from h = A*omega

  omegavk = yvn(mo:mo+2);
  m=me;
  for j=1:3
    eak(1:3,j) = yvn(m:m+2); m=m+3;
  end;

  Ak = eak*Lambda*eak'; % inertia matrix 
  % NOT: E will be normalized after this --FIX ME--
  hvk = Ak*omegavk; 
  % Update the current state:
  yvn(mh:mh+2)=hvk;
  


