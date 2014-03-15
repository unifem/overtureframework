% profile.m
% 
%  ZND profile for Mie-Gruneisen EOS (plus other EOS)
%
%  For most of this file the profile is assumed to move from right to left
% so that U is negative
%
%                       +-----------------
%                  <- U |
%                       |
%     -------------------
%      r0, u0, T0, p0
%
% For plotting and output,  near the end, the profile is reversed and travels from left to right since
% this is how profiles usually move in cgcns.
%
% This matlab program also uses:
%    hugoniot.m, velocityCJ.m, volumeCJ.m, getComputedValues.m
%
clear;
clf;

set(gca,'FontSize',14);

global a1 a2 p0 v0 a3 Q m Y g G1 gr alpha beta vCJ uCJ kappa options

% ************************* assign testCase ******************************************
testCase=2;  % 0=ideal, 1=MG, 2=MG with kappa!=1
gg=2; % 1=coarse grid, 2=fine grid

n=2000; % 1000;  % 200 was too small

kappa=1.; % kappa : Cp = Cv + kappa*R 
% ************************************************************************************


if testCase==0
  alpha=0.; beta=0.;
  g=1.4; % gamma
elseif testCase==1
  alpha=.5;  beta=.5;
  g=4./3.;
else 
  alpha=.5;  beta=.5;
  g=4./3.;
  kappa=1.5;
end;


options = optimset('TolX',1e-15);

% hg = inline('(a1*v+a2)*(p0 + m*m*(v0 - v))+(a3*v-c1)+Q*Y','v','Y');


% Mie_Gruneisen parmeters
G0=0.;
G1=0.; 


gr= (g+1)/(g-1);

Rg= 1.; % /(g-1);


r0=1;
T0=.9325;
v0=1./r0;
u0=0;

F0= 0; 
p0= r0*Rg*kappa*T0+F0; 


Y0=0;
Y1=1;


% Determine the CJ velocity and volume fractions for this value of Q

Q=-4.;
% Here is a guess for U and m for this Q
U=-3.1816;  
vv0=u0-U;
m=r0*vv0;

% *** we need G1 here which depends on vCJ !

% First solve for the volume fraction
%vCJ = v0/gr + (1/(m*gr))*sqrt( v0*( (p0*gr*gr-1) -2*(gr/v0)*( Q*Y1 ) ) );
% if alpha and beta are non-zero then we need to solve:
%[vCJ,fval] = fzero(@volumeCJ,vCJ);

% Solve for the uCJ and vCJ --- this determines U and m so that we have a CJ detonation for this value of Q
vCJ=.6;
uCJ=-U;

[uCJ,fval] = fzero(@velocityCJ,-U,options);

fprintf(1,' **** vCJ = %8.5f (fval=%8.2e) , uCJ = %10.7f,  (r0*uCJ=%8.5f, m(guess)=%8.5f)\n',vCJ,fval,uCJ,r0*uCJ,m);

% Here is the new m:
U=-uCJ;
m=r0*uCJ;




% a1*p*v + a2*p + a3*v = gr

a1=g/(g-1)-.5;
a2= .5/r0;
a3= .5*p0; 


eps=.075;
sigma0=1.;
sigma=sigma0*eps/((g-1.)*abs(Q));

vMin=v0/gr; 
a=vMin +.05;
b=v0+p0/m^2.;  % value of v where Rayleigh line hits zero
h=(b-a)/(n-1);


x = zeros(n,1);
y = zeros(n,1);
r = zeros(n,1);
v = zeros(n,1);
p = zeros(n,1);
T = zeros(n,1);
u = zeros(n,1);
R = zeros(n,1);  % reaction rate term

pmg = zeros(n,1);
pmg1 = zeros(n,1);
rayleigh = zeros(n,1);
for i=1:n
  v(i)=a+h*(i-1);
  p(i)= p0*(gr*v0-v(i))/(gr*v(i)-v0);

  p1(i)=( p0*(gr*v0-v(i)) -2.*(Q*Y1)  )/(gr*v(i)-v0);

%   G = alpha/2.*(v(i)-v0)^2 + beta/3.*(v(i)-v0)^3  - v(i)/(g-1)*( alpha*(v(i)-v0)+beta*(v(i)-v0)^2 );
  % *wdh* 050108 -- Ec should be Ec/rho
  G = v(i)*eosFn1(v(i)) - v(i)/(g-1)*( eosFn2(v(i)) );

  pmg(i)= ( p0*(gr*v0-v(i)) -2.*(G-G0) )/(gr*v(i)-v0); 
  pmg1(i)=( p0*(gr*v0-v(i)) -2.*(Q*Y1 + G-G0) )/(gr*v(i)-v0);

  v2(i) = a + h*(i-1);
  rayleigh(i) = p0 + m*m*(v0 - v2(i));   % rho*v^2 + p = const
end

hold on;
plot(v,p,'c-', v,pmg,'g-', v,pmg1,'r-' ,v2,rayleigh,'b-');
label3=sprintf('Hugoniot (MieG, Y=1)');
legend('Hugoniot (ideal, Y=0)','Hugoniot (MieG, Y=0)',label3,'Rayleigh');
grid on
xlabel('v');
ylabel('p');

myLabel=sprintf('Q=%3.1f, U=%3.2f,\n \\gamma=%3.2f, \\alpha=%3.2f, \\beta=%3.2f \\kappa=%3.2f',Q,-U,g,alpha,beta,kappa);
text(a,.75,myLabel,'HorizontalAlignment','left','FontSize',14);
title('ZND Profile, Analytic Solution','FontSize',14);

myTitle=sprintf('ZND profile, p0=%4.3f, Q=%3.1f, U=%3.2f, \\gamma=%3.2f, \\alpha=%3.2f, \\beta=%3.2f',p0,Q,-U,g,alpha,beta);
% title(myTitle,'FontSize',14);

plot(v0,p0,'b-o');
% text(v0,p0,'X','FontSize',16);
text(v0,p0+.3,'Initial state','HorizontalAlignment','left','FontSize',14);


v1=vCJ;
p1=p0 + m*m*(v0 - v1); 

plot(v1,p1,'b-o');
text(v1,p1+.3,'Final state','HorizontalAlignment','left','FontSize',14);

% text(t(3),defect(3),' \leftarrow W[1,1], CR=.122, ECR=.62','HorizontalAlignment','left','FontSize',16);

% set(gca,'XLim',[0 1.4]);
set(gca,'YLim',[0 10]);

hold off;

pause
% print -depsc2 Hugoniot.eps
clf;


% f = inline('1.+x*y','x','y');

% hg = inline('(a1*v+a2)*(p0 + m*m*(v0 - v))+(a3*v-gr)+Q*Y','v','Y');

yEps=1.e-5;  % =.1 *wdh* 050108
% dy=1./(n+yEps);
Ya=0.;
Yb=1.-yEps;
dy=(Yb-Ya)/(n-1);

% dy=1./(n+yEps);

vStart=.25;  % *** here is a guess -- could do better
delta=.1;


for i=1:n

 Y = Ya + (i-1)*dy;  % becomes more difficult at Y-> 1  -- we should cluster points near Y=1

%  [vv,fval] = fzero(@hugoniot,vStart);
 % vStarta = max(1.,vStart-delta);
 
 [vv,fval] = fminbnd(@hugoniot,vStart-delta,vStart+delta,options);
 if abs(fval)>1.e-10 
  fprintf('*** Warning computing V from Y : fval=%9.3e for Y=%9.3e (you may want to increase delta )\n',fval,Y);
 end;

 vStart=vv;
 delta=(5+10*Y)/n;   % decrease the interval we look in (but not too small at Y->1)

 y(i)=Y;
 v(i)=vv;
 r(i)=1./v(i);
 p(i)=p0 + m*m*(v0 - v(i)); 

% Fi = alpha*(v(i)-v0)+beta*(v(i)-v0)^2;
 Fi = eosFn2(v(i)); 

 T(i)=( p(i) - Fi )/(kappa*Rg*r(i));  % kappa : Cp=Cv +kappa*R 

 u(i)=-( m/r(i)+U );  % flip u so the profile moves in the opposite direction

 R(i) = (r(i)/m)*(1.-y(i))*sigma*exp((1-1/T(i))/eps);

 % fprintf(1,' Y=%6.3f v=%6.3f p=%6.3f T=%6.4f (fval=%8.2e)\n',Y,v(i),p(i),T(i),fval);

end;

fprintf(1,'eps=%10.3e, sigma=%10.3e\n',eps,sigma);

if 1
  plot(y,r,'b-',y,p,'r-+',y,T,'g-',y,R/100,'c-');
  legend('rho','p','T','R/100');
  xlabel('Y');
  title('rho, p, V computed from Y');  

  pause;
  clf;
end;

% *************** Compute x from the species equation ******

x(1)=0.;
for i=2:n
  x(i)=x(i-1)+.5*( 1./R(i) + 1./R(i-1) )*( y(i)-y(i-1) );
end;


% ****** add on the pre-shock state to the curve for plotting
m=20;
nn=m+n;

xa = zeros(nn,1);
ya = zeros(nn,1);
ra = zeros(nn,1);
ua = zeros(nn,1);
pa = zeros(nn,1);
Ta = zeros(nn,1);

pScale=2;
dx =.05;
for i=1:m
  j=nn-i+1;
  xa(j)=dx*(m-i)/(m-1);
  ya(j)=0;
  ra(j)=r0;
  pa(j)=p0;
  Ta(j)=T0;
  ua(j)=u0;
end;
for i=m+1:nn
  j=nn-i+1;
  xa(j)=-x(i-m);
  ya(j)=y(i-m);
  ra(j)=r(i-m);
  pa(j)=p(i-m);
  Ta(j)=T(i-m);
  ua(j)=u(i-m);


end;
  
j=1;
fprintf(' Solution at $x2=%9.3e; $r2=%12.6e; $u2=%12.6e; $T2=%12.6e; $Y2=%12.6e;\n', ...
        xa(j),ra(j),ua(j)+U,Ta(j),ya(j));

plot(xa,ya,'r-', xa,ra,'g-',xa,pa/pScale,'b-',xa,Ta,'c-',xa,ua,'m-');
legend('Y','\rho',sprintf('p/%d',pScale),'T','u');
xlabel('x');
% title(myTitle,'FontSize',14);
text(xa(1),3.5,myLabel,'HorizontalAlignment','left','FontSize',14);
title('ZND Profile, Analytic Solution','FontSize',14);

grid on;
% set(gca,'XLim',[0 100]);
% set(gca,'XLim',[-.1 .025]);
if kappa==1.5
  set(gca,'XLim',[-.15 .05]);
else
  set(gca,'XLim',[-.05 .02]);
end;

myLabel=sprintf('Q=%3.1f, U=%3.2f,\n \\alpha=%3.2f, \\beta=%3.2f,\n \\gamma=%3.2f, \\kappa=%3.2f',Q,-U,alpha,beta,g,kappa);
if testCase==2 
 text(-.14,3.5,myLabel,'HorizontalAlignment','left','FontSize',14);
else
 text(-.045,3.5,myLabel,'HorizontalAlignment','left','FontSize',14);
end


% print -depsc2 profileIdealOneStep.eps
% print -depsc2 profileMieGruneisen.eps


pause


%
% +++ Now create a profile.dat file for OverBlown +++
%
if testCase==0
  name = 'oneStepIdealProfile.data';
elseif testCase==1
  name = 'oneStepMieGruneisenProfile.data';
else
  name = 'oneStepMieGruneisenKappa1p5Profile.data';
end;

fid=fopen(name,'w');

nc=5;  % number of components

% first line: number of points, number of components
fprintf(fid,' %d %d\n',nn,nc);

% second line: xShock  rho1  u1 ...   set solution to (rho1,u1,...) for x>xShock

xFront=0; % position of the detonation front in the data
xTarget=.5;      % this is where we want the front to be
xShift=-xFront+xTarget;   % shift x values by this amount to put the front at x=xTarget

uShift=-U;   %  shift in u to adjust the relative speed

xShock=xTarget; 
fprintf(fid,' %e  %e %e %e %e %e\n',xShock,r0,u0-uShift,0.,T0,Y0);
for i=1:nn

  fprintf(fid,' %e  %e %e %e %e %e \n',xa(i)+xShift,ra(i),ua(i)-uShift,0.,Ta(i),ya(i));

end


fclose(fid);
fprintf('wrote OverBlown profile.dat file:  %s\n',name);

% pause


% compare to the computed solution
hold on;

% for gg=1:2

% ==== read the data from the 3-level amr run: =========
[xc,rhoc,uc,Tc,pc,lambdac] = getComputedValues(testCase,gg);

levels=1;
if gg==1 
  if testCase==1
    dxl3=5.e-3/16; % grid spacing on level 3
    % xc=xc-.5+3.*dxl3; % coarse grid at time?
    % xc=xc-.5+4.*dxl3; 
    xc=xc-.5; 
    levels=2;
  else
    % ideal case
    xc=xc-.5;
    levels=2;
  end;
else
  if testCase==1
    % here are results from the 4-level
    % xc=xc-.9965+.001; 
    dxl4=5.e-3/64; % grid spacing on level 3
    % xc=xc-.5 + .0025; 
    xc=xc-.5; 
    levels=3;
    if kappa==1
      xc=xc+.00075; 
    else
      xc=xc;
    end;
  elseif testCase==2
    % xc=xc-.5+.005;  % shift for small domain solution, speed off
    % xc=xc-1.5; % for long domain case, [0,2]
    xc=xc-.5; % for long domain case, [0,2]
    levels=3; 
  else
    % ideal case
    xc=xc-.5;
    levels=3;
  end;
end;

% Uncomment the next lines to create a plot
plot(xc,lambdac,'r--',xc,rhoc,'g--',xc,pc/2,'b--',xc,Tc,'c--',xc,uc+uCJ,'m--');
% legend('rho','u','T','p','lambda');
%
set(gca,'XLim',[-.06 .04]);
myLabel=sprintf('Q=%3.1f, U=%3.2f,\n \\alpha=%3.2f, \\beta=%3.2f,\n \\gamma=%3.2f, \\kappa=%3.2f',Q,-U,alpha,beta,g,kappa);
if testCase==2 
 text(.005,2.5,myLabel,'HorizontalAlignment','left','FontSize',14);
end
title(sprintf('ZND Profile, Comparison to Computed Solution, Levels=%d',levels),'FontSize',14);

% end % for gg 

% print -depsc2 profileMGvsComputedLevel2.eps
% print -depsc2 profileMGvsComputedLevel3.eps

% print -depsc2 profileIdealvsComputedLevel2.eps
% print -depsc2 profileIdealvsComputedLevel3.eps

% print -depsc2 profileMGvsComputedKappa1p5Level2.eps
% print -depsc2 profileMGvsComputedKappa1p5Level3.eps

hold off;
pause
% *************** plot diagnostics ******************p

Rayleigh = zeros(nn,1);
Hugoniot = zeros(nn,1);
massFlux = zeros(nn,1);
eFlux    = zeros(nn,1);
pu       = zeros(nn,1);

pu0 = p0+r0*(u0+U)^2;
ee0 = 1./((g-1)*r0)*( p0-eosFn2(v0) ) + eosFn1(v0)/r0;
eFlux0 = ee0 + p0/r0 + .5*(u0+U)^2 + Q*Y0; 

for i=1:nn
  mf=ra(i)*(ua(i)+U);
  massFlux(i)=mf;
  Rayleigh(i) = pa(i)-p0 - mf*mf*(v0-1./ra(i));
  vv=1./ra(i);

%   G= alpha/2.*(vv-v0)^2 + (beta/3.)*(vv-v0)^3  - vv/(g-1)*( alpha*(vv-v0)+beta*(vv-v0)^2 );
  G = vv*eosFn1(vv) - vv/(g-1)*( eosFn2(vv) );

  Hugoniot(i)=(g/(g-1))*(pa(i)/ra(i)-p0*v0) - .5*(pa(i)-p0)*(v0+1./ra(i)) + Q*ya(i) + G;
  
  pu(i)= pa(i)+ra(i)*(ua(i)+U)^2 - pu0;  % p + rho*v^2 should be const

  ee = 1./((g-1)*ra(i))*( pa(i)-eosFn2(vv) ) + eosFn1(vv)/ra(i);
  eFlux(i) = ee + pa(i)/ra(i) + .5*(ua(i)+U)^2 + Q*ya(i) - eFlux0; 
end;

m0=r0*(u0+U);
plot(xa,Rayleigh,'r-',xa,Hugoniot,'g-',xa,massFlux-m0,'b-',xa,pu,'c-',xa,eFlux,'b+');
legend('Rayleigh','Hugoniot','m-m0','p+r*u^2','e+p/r+...');

title('ZND Profile, Analytic Results','FontSize',14);
xlabel('x');
grid on;
if kappa==1.5 
  text(-1.,-2.e-8,myLabel,'HorizontalAlignment','left','FontSize',14);
end;

pause
% print -depsc2 profileMGconserved.eps



% [xc,rhoc,uc,Tc,pc,lambdac] =

mm=size(xc,2);

Rayleighc = zeros(mm,1);
Hugoniotc = zeros(mm,1);
massFluxc = zeros(mm,1);
puc = zeros(mm,1);
eFluxc = zeros(mm,1);

mMid=floor( (mm+1)/2);
xLeft=-.1; xRight=.025; % show this interval
ma=1;
for i=1:mMid
  if xc(i)>xLeft 
    ma=i; 
    break;
  end 
end 
mb=mm;
for i=mm:-1:mMid
  if xc(i)<xRight
    mb=i; 
    break;
  end 
end 

% ma=mMid-200;
% mb=mMid+200;
pu0 = pc(mb)+rhoc(mb)*(uc(mb))^2;
ee0 = 1./((g-1)*rhoc(mb))*( pc(mb)-eosFn2(1./rhoc(mb)) ) + eosFn1(1./rhoc(mb))/rhoc(mb);
eFlux0 = ee0 + pc(mb)/rhoc(mb) + .5*(uc(mb))^2 + Q*lambdac(mb); 

for i=ma:mb
  mf=rhoc(i)*(uc(i));
  massFluxc(i)=mf;
  Rayleighc(i) = pc(i)-p0 - mf*mf*(v0-1./rhoc(i));
  vv=1./rhoc(i);

%  G= alpha/2.*(vv-v0)^2 + (beta/3.)*(vv-v0)^3  - vv/(g-1)*( alpha*(vv-v0)+beta*(vv-v0)^2 );
  G = vv*eosFn1(vv) - vv/(g-1)*( eosFn2(vv) );

  Hugoniotc(i)=(g/(g-1))*(pc(i)/rhoc(i)-p0*v0) - .5*(pc(i)-p0)*(v0+1./rhoc(i)) + Q*lambdac(i) + G;
  
  puc(i)= pc(i)+rhoc(i)*(uc(i))^2 - pu0;  % p + rho*v^2 should be const

  ee = 1./((g-1)*rhoc(i))*( pc(i)-eosFn2(vv) ) + eosFn1(vv)/rhoc(i);
  eFluxc(i) = ee + pc(i)/rhoc(i) + .5*(uc(i))^2 + Q*lambdac(i) - eFlux0; 

end;

plot(xc(ma:mb),Rayleighc(ma:mb),'r-',xc(ma:mb),Hugoniotc(ma:mb),'g-',xc(ma:mb),massFluxc(ma:mb)-m0,'b-',...
                   xc(ma:mb),puc(ma:mb),'c-',xc(ma:mb),eFluxc(ma:mb),'b-+');
legend('Rayleigh','Hugoniot','m-m0','p+r*u^2','e+p/r+...');

title(sprintf('ZND Profile, Computed Results, Levels=%d',levels),'FontSize',14);
grid on;

% set(gca,'XLim',[-2. .04]);
% set(gca,'YLim',[-1. 1.]);

if kappa==1.5 
  set(gca,'YLim',[-.02 .02]);
  text(-.095,.015,myLabel,'HorizontalAlignment','left','FontSize',14);
else
  set(gca,'YLim',[-.1 .1]);
end;

% print -depsc2 profileMGconervedComputed.eps
