%
%
function [ numErr, errorName, errv ] = rigidBody( itest_,tf,solver_,cfl,plotOption,debug_,...
                                                  checkErrors,normalize_,rtol,freeRotationAxis,...
                                                  addedMass_,twilightZone_ ,mass_, dirkOrder, dt0 )
%
% Solve the Newton-Euler equations of rigid body motion.
% 
% Here are the ODEs we solve: 
%
%    x' = v
%    m v' = f 
%    h' = g
%    E' = Omega*E
%    A w' = - Omega A w + g 
%    q' = omega x q          (Quaternion equation)
%
% where
%    h = A*w 
%    A = E Lambda E^T
% 
% Note that we solve both the equation for E and q (only one is really
%  needed so that we can compare the results).
% 
% -- see the rigid body notes rb.tex for more details on the equations
%
%
% Arguments:
%  itest = test case
%          0 : polynomial forcing (with exact solution)
%          1 : free rotation (see freeRotationAxis)
%          2 : twilight zone
%  tf = final time
%  solver = 0 : matlab's ode45
%         = 1 : runge-kutta order 4
%         = 2 : leapfrog - trapezoidal predictor-corrector scheme from Overture.
%         = 3 : Added-Mass Implicit LF-TRAP, AMI-LF-TRAP
%         = 4 : DIRK (Diagonal implicit Runge Kutta)
% 
%  cfl = cfl number (unless dt0>0 is input)
%  plotOption = 0 : no plotting
%               1 : plot solution versus t
%               2 : plot errors versus t 
%               4 : plot moving ellipsoid 
%  debug = bit flag 
%  checkErrors = 1 : check errors
%  normalize = 1 : normalize the rotations
%  rtol = relative tolerance for solvers
%  freeRotationAxis : 1, 2, or 3 - 
%  addedMass = 0 : turn off added mass matrices 
%            = 1 : turn on added mass matrices 
%  twilightZone = 0 : off
%                 1 : polynomial
%                 2 : trigonometric
% dirkOrder : order of accuracy for the DIRK method
% dt0 : use this fixed dt if dt0>0 
%
globalDeclarations;  % declare global variables here 

debug=debug_;
debugFlag=debugFlag;
normalize=normalize_;

polynomialTwilightZone=1;
trigonometricTwilightZone=2;
twilightZone=twilightZone_;
if twilightZone==0
  twilightZoneName='off';
elseif twilightZone==polynomialTwilightZone
  twilightZoneName='polynomial';
elseif twilightZone==trigonometricTwilightZone
  twilightZoneName='trigonometric';
else
  twilightZoneName='unknown';
end;

% itest options (enums:)
polynomialForcing=0;    % polynomial f and g 
freeRotation=1;         % free rotation exact solution 
twilightZoneSolution=2; % TZ


itest=itest_; % this is a global variable
if itest==polynomialForcing
  testName='poly force';
elseif itest==freeRotation
  testName=sprintf('free rotation(%d)',freeRotationAxis);
elseif itest==twilightZoneSolution
  testName=sprintf('TZ=%s',twilightZoneName);
else
  testName='unknown';
end;

% solver options:
ode45Solver = 0; % used matlab's ode45
rk4Solver = 1;   % use rungeKutta4 solver 
leapfrogTrapPC =2; % leap-frog predictor, trapezoidal corrector
AMIleapfrogTrapPC=3; % added-mass-implicit leap-frog predictor, trapezoidal corrector
DIRK=4;              % Runge-Kutta DIRK 

%% solver=ode45Solver; solverName='ode45';
% solver=rk4Solver; solverName='rk4';

dirkDefaultOrder=dirkOrder; % order of accuracy for the DIRK method (1=BE)


implicitSolver=0; % set to 1 if this is an implicit solver
solver=solver_;
solverName = getSchemeName();
% if solver==ode45Solver
%   solverName='ode45';
% elseif solver==rk4Solver
%   solverName='rk4';
% elseif solver==leapfrogTrapPC 
%   solverName='leapfrogTrapPC';
% elseif solver==AMIleapfrogTrapPC
%   solverName='AMIleapfrogTrapPC';
%   implicitSolver=1;
% elseif solver==DIRK
%   solverName=sprintf('DIRK%d',dirkOrder);
%   implicitSolver=1;
% else
%   solverName='unknown'; 
% end;

mass=mass_; % mass of the body 


addedMass=addedMass_;


fprintf('rigidBody: itest=%d [%s], tf=%8.2e, solver=%d [%s], freeRotationAxis=%d, checkErrors=%d, normalize=%d',...
            itest,testName,tf,solver,solverName,freeRotationAxis,checkErrors,normalize);
fprintf(' rtol=%8.2e, plotOption=%d, addedMass=%d, tz=%s, mass=%f, debug=%d\n',rtol,plotOption,addedMass,...
        twilightZoneName,mass,debug);

errorName  = [ '123456789012345' ; ...
               '123456789012345' ; ...
              ];

%% tf=2.; % final time
tp=.1; % output times
%% cfl=.25; % used to compute the time step for RK4

%% itest = polynomialForcing;  % just run with some force and torque
% itest = freeRotation; % Exact solution -- free rotation 
%% freeRotationAxis=2; % option for free rotation: axis to rotate about: 1, 2 or 3 

%% normalize=1; % normalize=1 : normalize rotations
%% debug=1; 
%% checkErrors=1;


% -- error tolerances: 
% rtol=1.e-10; 
%% rtol=1.e-5; 
% rtol=1.e-3;

atol=rtol*.1;

% -- setup and assign initial conditions --
initialConditions; 

% 
if solver==ode45Solver

  % ------ Use matlab ode45 -----
  options = odeset('RelTol',rtol,'AbsTol',atol);
  tInterval = [0 tf]; % integrate over this time interval
  [tv, wv] = ode45('wdot1',tInterval,wv0, options);

elseif solver==rk4Solver

  % --- Use RK4 routine ------

  advanceRK4

elseif solver==leapfrogTrapPC

  % --- solve using the Leapfrog predictor, trapezoidal corrector algorithm from Overture ------

  advanceLeapFrogTrapPC


elseif solver==AMIleapfrogTrapPC

  % --- Added-mass-implicit Leapfrog predictor, trapezoidal corrector  ------
  %  *new* solver that treats added mass matricies implicitly

  advanceImplicitLeapFrogTrapPC


elseif solver==DIRK
  % -- Diagonally implicit Runge Kutta --

  advanceDIRK

else
  fprintf('ERROR: unknown solver=%d\n',solver);

end;

n = size(tv,1); 

% ------- plot results ---------

if mod(plotOption,2) == 1
  titleLabel = sprintf('Rigid body motion, %s, %s',testName,solverName);

  plot( tv,wv(:,1),'r-x',tv,wv(:,2),'r-+',tv,wv(:,3),'r-o', tv,wv(:,4),'g-x',tv,wv(:,5),'g-+', tv,wv(:,6),'g-o');
  % plot( tv,wv(:,1),'r-x',tv,wv(:,2),'r-+',tv,wv(:,3),'r-o', tv,wv(:,4),'g-x',tv,wv(:,5),'g-+', tv,wv(:,6),'g-o');
  title(titleLabel);
  legend('x_1','x_2','x_3','v_1','v_2','v_3');
  xlim([0 tf]);
  xlabel('t');
  
  pause;
  
  hv = wv(:,7:9);
  omegav = wv(:,19:21); 
  
  plot( tv,hv(:,1),'r-x',tv,hv(:,2),'r-+',tv,hv(:,3),'r-o', tv,omegav(:,1),'b-x',tv,omegav(:,2),'b-+',tv,omegav(:,3),'b-o');
  title(titleLabel);
  legend('h_1','h_2','h_3','\omega_1','\omega_2','\omega_3');
  xlim([0 tf]);
  xlabel('t');
  pause;
end;


if debug > 1
  pause;
end;


fprintf('RigidBody: test=%s, solver=%s, tz=%s, tf=%8.2e, cfl=%5.2f, steps=%d, mass=%f, addedMass=%d, rtol=%8.2e, normalize=%d\n',...
        testName,solverName,twilightZoneName,tf,cfl,n,mass,addedMass,rtol,normalize);

% -- compute errors ---
computeErrors

% ---------- plot errors over time ----
if mod(floor(plotOption/2),2) == 1

  titleLabel = sprintf('Errors, %s, %s',testName,solverName);
  if numErr==1 

    plot( tv,errv(:,1),'r-x');
    title(titleLabel);
    legend(errorName(1,:));
    xlim([0 tf]);
    xlabel('t');

  elseif numErr==2

    plot( tv,errv(:,1),'r-x',tv,errv(:,2),'g-s');
    title(titleLabel);
    legend(errorName(1,:),errorName(2,:));
    xlim([0 tf]);
    xlabel('t');

  elseif numErr>0 
    fprintf('ERROR: plot errors not implemented for numErr=%d\n',numErr);

  end;
  pause; 
end;


% ---- plot the dynamics over time with an ellpsoid ---

if mod(floor(plotOption/4),2) == 1

  pause; 

  % [ x,y,z] = ellipsoid(xc,yc,zc,xr,yr,zr,n)
  
  % [x, y, z] = ellipsoid(0,0,0,5.9,3.25,3.25,30);
  % surfl(x, y, z)
  % colormap copper
  % axis equal
  
  [x, y, z] = ellipsoid(0.,0.,0., I1, I2, I3, 20);

  stride=1;
  for i=1:stride:n

    xv = [ wv(i,mx);  wv(i,mx+1); wv(i,mx+2) ];

    % Apply the current rotation to (x,y,z)
    qv = [ wv(i,mq);  wv(i,mq+1); wv(i,mq+2); wv(i,mq+3) ];  % Quaternion
    R = quaternionToMatrix(qv);

    x1 = R(1,1)*x+R(1,2)*y + R(1,3)*z + xv(1);
    y1 = R(2,1)*x+R(2,2)*y + R(2,3)*z + xv(2);
    z1 = R(3,1)*x+R(3,2)*y + R(3,3)*z + xv(3);
    
    surf(x1, y1, z1,'FaceColor','interp','FaceLighting','phong');
    title(sprintf('t=%9.3e',tv(i)));
    daspect([1 1 1]);
    colormap(jet);

    xlim([-5,5]);
    ylim([-5,5]);
    zlim([-5,5]);
    xlabel('x');
    ylabel('y');
    zlabel('z');

    drawnow
    % pause

  end;

end;
