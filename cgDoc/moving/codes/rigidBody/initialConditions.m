%
%  Setup and assign initial conditions for the rigidBody equations
%

% Polynomial forcing: 
% force: f(j) = pf(j,1) + pf(j,2)*t + pf(j,3)*t^2 
pf = [ -.25 -.5   .125 ; ...  % constants 
       .2   -.1   .3   ; ...  % coeff's of t 
       .15   .2  -.15  ];     % coeff's of t^2
% Torque: 
pg = [ -.35  .4   .25  ; ...
        .4   -.2   -.22; ...
       .25   .1   .35  ];

% Twilightzone solutions:

% Trigonometric:
%     vv = [ amp(1,1)*cos( freq(1,1)*( t-tOffset(1,1) ); ...
%            amp(2,1)*cos( freq(2,1)*( t-tOffset(2,1) ); ...
%            amp(3,1)*cos( freq(3,1)*( t-tOffset(3,1) ) ];

amp = zeros(3,2); freq=zeros(3,2); tOffset=zeros(3,2);
% v : 
amp(1,1)=1.;  freq(1,1)=pi*2.5;  tOffset(1,1)=0.;
amp(2,1)=1.5; freq(2,1)=pi*1.75; tOffset(2,1)=.25;
amp(3,1)=.75; freq(3,1)=pi*1.5;  tOffset(3,1)=.5;
% omegav:
amp(1,2)=.8;  freq(1,2)=pi*1.5;  tOffset(1,2)=.25;
amp(2,2)=1.2; freq(2,2)=pi*2.0;  tOffset(2,2)=.75;
amp(3,2)=.90; freq(3,2)=pi*1.2;  tOffset(3,2)=.35; 



I1=3.; I2=2.; I3=1.; % moments of inertia
Lambda = [ I1 0. 0.; 0. I2 0. ; 0. 0. I3];

ea0 = [ 1. 0. 0.; 0. 1. 0.; 0. 0. 1.];  % E(0)


% initial conditions
xv0 = [0; 0; 0];
vv0 = [0; 0; 0];
hv0 = [0; 0; 0];
omegav0 = [0; 0; 0];


R0 = [ 1. 0. 0.; 0. 1. 0.; 0. 0. 1.];  % R(0)
% qv0 = matrixToQuaternion( R0 );
qv0 = [1; 0; 0; 0 ];  % initial conditions for the quaternion

if itest==freeRotation
   % free rotation test 
   % If I1=I2 or I2=I3 or I3=I1 we can compute the exact solution for omegaHat = E^T omegav
   I1=1.; I2=1.; I3=1.;
   if freeRotationAxis==1 
     I1=2;
   elseif freeRotationAxis==2
     I2=2.;
   else
     I3=2.;
   end;
   Lambda = [ I1 0. 0.; 0. I2 0. ; 0. 0. I3];
   omegav0 = [0.; 0.; 0.];
   i3 = freeRotationAxis;
   i1= mod(i3,3)+1;
   i2= mod(i1,3)+1;
   omegav0(i1)=1.; omegav0(i2)=2.; omegav0(i3)=3.; 


   A0 = ea0*Lambda*ea0'; 
   hv0 = A0*omegav0;

end;

if itest==twilightZoneSolution

  % evaluate the initial conditions from the exact solution:
  [xv0, vv0, omegav0 ] = getExactSolution( 0, 0. ); 
  A0 = ea0*Lambda*ea0'; 
  hv0 = A0*omegav0;

end;

% state: w = [ x1; x2; x3; v1; v2; v3; h1; h2; h3; e11; e12; e13; e21; ... e33; omega1; omega2; omega3; q1; q2; q3 ];

mx=1; mv=mx+3; mh=mv+3; me=mh+3; mo=me+9; mq=mo+3; % index positions in wv of h, e, omega, q

wv0 = stateToArray( xv0, vv0, hv0, ea0, omegav0, qv0 );


