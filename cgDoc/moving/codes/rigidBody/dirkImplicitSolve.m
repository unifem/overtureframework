%
% Solve the implicit DIRK equation for kv 
% 
% --- Solve:
%      M*(yv-yv0) - aii*dt*f(yv,tc) = 0 
%
%   kv = (yv-yv0)/(aii*dt) 
% Input:
%   aii, tc : diagonal weight and time
%   yv = initial guess 
%   yv0 
function kv = dirkImplicitSolve( dt,aii,tc,yv,yv0 )

  globalDeclarations;  % declare global variables here 

  rkTol=1.e-5;  % Note: if converging quadratically then next correction will be rkTol^2   **FIX ME**

  [ xvn, vvn, hvn, ean, omegavn, qvn ] = arrayToState( yv  );  % initial guess
  [ xv0, vv0, hv0, ea0, omegav0, qv0 ] = arrayToState( yv0 );  

  [A11 , A12 , A21, A22 ] = getAddedMassMatrices( tc );

  % Initial guess:
  vvk = vvn;  omegavk = omegavn; eak = ean;

  fvnp1 = force(tc,vvn,omegavn,ean);  % get force
  gvnp1 = torque(tc,vvn,omegavn,ean); % get torque
  % For TZ the torque uses the computed A -- we need to adjust the Jacobian for this
  %   g = hvDotExact = A*omegavDotExact + Omega*A*omegavExact; 
  if itest==twilightZoneSolution
    [xvExact, vvExact, omegavExact ] = getExactSolution( 0, tc ); 
    [xvDotExact, vvDotExact, omegavDotExact ] = getExactSolution( 1, tc ); 
    OmegaExact = getCrossProductMatrix( omegavExact );  % 
  end


  adt = aii*dt; 

  m = 3 + 3 + 9;    % number of unknowns
  rk = zeros(m,1);  % holds residual 
  Jk = zeros(m,m);  % Jacobian

  maxIterations=20;
  for k=1:maxIterations

    Ak = eak*Lambda*eak'; % inertia matrix 
    % Omegak = [ 0. -omegavk(3) omegavk(2); omegavk(3) 0. -omegavk(1); -omegavk(2) omegavk(1) 0. ];
    Omegak = getCrossProductMatrix( omegavk );  % matrix for [ omegavk X ]

    if itest==twilightZoneSolution
      % for TZ, torque uses Ak from eak -- this slows down Newton
      gvnp1 = torque(tc,vvn,omegavk,eak); % get torque
    end;

    % residual
    % v: 
    rk(1:3) = mass*(vvk-vv0) - adt*( -A11*vvk - A12*omegavk + fvnp1 );
    % omega: 
    rk(4:6) = Ak*(omegavk-omegav0) - adt*( -Omegak*Ak*omegavk -A21*vvk - A22*omegavk + gvnp1 );
    % E:   
    mr=7;
    for j=1:3
      rk(mr:mr+2) = eak(1:3,j)-ea0(1:3,j) - adt*( Omegak*eak(1:3,j) );
      mr=mr+3; 
    end;

    % fprintf('BE: residual: k=%d, max(|resid|)=%8.2e\n',k,max(abs(rk))); 
    % rk'
    % pause

    
    % Fill the Jacobian
    % v: 
    Jk(1:3,1:3) = mass*eye(3,3) + adt*A11;   % d(fv)/d(v)
    Jk(1:3,4:6) = adt*A12;                   % d(fv)/d(omega)

    % omega:
    hk = Ak*omegavk;
    hkStar = getCrossProductMatrix( hk );  % matrix for [ hk X ]
    Jk(4:6,1:3) = adt*A21;                            % d(fomega)/d(v)
    Jk(4:6,4:6) = Ak + adt*(A22+Omegak*Ak-hkStar);    % d(fomega)/d(omega)
    
    dOmega1 = omegavk-omegav0;
    if itest==twilightZoneSolution
     dOmega1 = dOmega1 - adt*omegavDotExact;
    end;

    mr=7;
    for j=1:3
      % Here is d(fomega)/d(ev(j))
      Jk(4:6,mr:mr+2) = Lambda(j,j)*( (eak(1:3,j)'*dOmega1)*eye(3,3) + eak(1:3,j)*dOmega1' ) ...
                        + (Lambda(j,j)*adt)*Omegak*( (eak(1:3,j)'*omegavk)*eye(3,3) + eak(1:3,j)*omegavk' );
      if itest==twilightZoneSolution
        Jk(4:6,mr:mr+2) = Jk(4:6,mr:mr+2) - ...
                 (Lambda(j,j)*adt)*OmegaExact*( (eak(1:3,j)'*omegavExact)*eye(3,3) + eak(1:3,j)*omegavExact' );
      end;
      mr=mr+3; 
    end;

    mr=7;
    for j=1:3
      eaStar = getCrossProductMatrix( eak(1:3,j) );  % matrix for [ eak(1:3,j) X ]
      Jk(mr:mr+2,4:6    ) = adt*eaStar;               % d(fea)/d(omega)
      Jk(mr:mr+2,mr:mr+2) = eye(3,3) - adt*Omegak;    % d(fea(i))/d(ea(i))
      mr=mr+3; 
    end;

    % fprintf('DIRK: Jacobian: k=%d\n',k); 
    % Jk
    % pause

    % Solve:
    dy = Jk\rk;
    
    maxCorrection = max(abs(dy));
    if mod(floor(debugFlag/2),2)==1 
      fprintf('DIRK: correction: dy: k=%d  max(|dy|)=%8.2e\n',k,maxCorrection); 
    end;
    % dy'
    % pause

    % update the current solution
    vvk = vvk - dy(1:3);  
    omegavk = omegavk - dy(4:6);
    mr=7;
    for j=1:3
      % correction should be orthogonal to ean *but* this messes up Newton convergence
      % dy(mr:mr+2) = dy(mr:mr+2) - (dy(mr:mr+2)'*ean(1:3,j))*ean(1:3,j);
      eak(1:3,j) = eak(1:3,j) - dy(mr:mr+2);
      mr=mr+3; 
    end;

    % fprintf('DIRK: eak*eak^T - I : k=%d\n',k);
    % eak*eak' - eye(3,3)
    % pause;

    if( maxCorrection<rkTol ) 
      break;
    end;

  end; % for k 

  % pause

  if maxCorrection>rkTol
    fprintf('DIRK:WARNING: No convergence in Newton after %d iterations,. maxCorrection=%8.2e, tol=%8.2e\n',...
          maxIterations,maxCorrection,rkTol);
    pause;
  else
    if mod(debugFlag,2)==1 
      fprintf('++DIRK: %i Newton iterations required.\n',k);
    end;
  end;


  % kv holds [ xvn, vvn, hvn, ean, omegavn, qvn ] 
  numberOfComponents = 3+3+3 + 9 + 3 + 4;
  kv = zeros(numberOfComponents,1);

  
  kv(1:3)=vvk;            % x' = v   => k(v) = v
  kv(4:6)=(vvk-vv0)/adt;
  kv(7:9)=0.; % k(h) : ignore for now 
  mr=10;
  for j=1:3
    kv(mr:mr+2) = (eak(1:3,j)-ea0(1:3,j))/adt;
    mr=mr+3; 
  end;
  kv(19:21)=(omegavk-omegav0)/adt;
  
  qDotk = getQdot( omegavk, qvn );   % **** do this for now ****XS
  kv(22:25)=qDotk; 