% lawOfTheWall.m 
% 
%  Plot quantities related to the law of the wall
%
%
clear;
clf;
myFontSize=18; 
set(gca,'FontSize',myFontSize);

kappa=.41;

E=9.;

% ------------------- Plot the turbulent boundary layer

if 0 == 1 

xa=1; xb=1000.;

n=201;
dx=(xb-xa)/(n-1);
x=(xa:dx:xb);

xav=1.; xbv=20; 
nv=101;
dxv=(xbv-xav)/(nv-1);
xv = (xav:dxv:xbv);

%xm =10; % matching point: y+=xm :    cv*xm = (1/kappa)*log(E*xm)
%cv = (1/kappa)*log(E*xm)/xm;
cv=1.; 
yv = cv*xv;  % viscous sub-layer


ylog = (1/kappa)*log(E*x);



plot(xv,yv,'r-',x,ylog,'b-');
legend('u^+=y^+','u^+ = (1/\kappa) ln(E y^+)');
ylabel('u+');
xlabel('y+');
title('Turbulent Boundary Layer');
set(gca,'XScale','log');

% set(gca,'XLim',[0 .5]);

pause
% print('-depsc2','turbulentBoundaryLayer.eps');

end;

% --------------------  plot without a log scale ----------------
if 0 == 1 

xa=1; xb=100.;

n=201;
dx=(xb-xa)/(n-1);
x=(xa:dx:xb);

xav=1.; xbv=20; 
nv=101;
dxv=(xbv-xav)/(nv-1);
xv = (xav:dxv:xbv);

%xm =10; % matching point: y+=xm :    cv*xm = (1/kappa)*log(E*xm)
%cv = (1/kappa)*log(E*xm)/xm;
cv=1.; 
yv = cv*xv;  % viscous sub-layer


ylog = (1/kappa)*log(E*x);

plot(xv,yv,'r-',x,ylog,'b-');
legend('u^+=y^+','u^+ = (1/\kappa) ln(E y^+)');
ylabel('u+');
xlabel('y+');
title('Turbulent Boundary Layer');
% set(gca,'XScale','log');

% set(gca,'XLim',[0 .5]);

pause
% print('-depsc2','turbulentBoundaryLayerNoLog.eps');

end;



% ----------------------------------------------------------------
% Plot the wall of the law function for uTau = z 

% yp = M 
M=30;             % y+ = 30 
Re=10e5;          % Reynold's number 
A= E*M*sqrt(Re);  % A = E*yp/nu 

uTauScale = 5./Re^.25;

za=1./A; zb=uTauScale; nz=11; dz=(zb-za)/(nz-1);
z=(za:dz:zb);
g = zeros(nz,1);
g2 = zeros(nz,1);
for i=1:nz
 g(i) = (1./kappa)*z(i)*log(A*z(i));
 g2(i) = (1./kappa)*z(i)*log(A);
end;

% fit a curve to the function g 
zf =zeros(2,1);
gf = zeros(2,1);

zf(1)=za; zf(2)=zb;
gf(1)=g(1); gf(2)=g(nz);


plot(z,g,'r-', zf,gf,'b-',z,g2,'g-');
ylabel('U_p');
xlabel('u_\tau');
title('U_p = (1/\kappa) u_\tau ln(A u_\tau), A=E y^+/\nu');
legend('U_p','linear-fit','u_\tau ln(A)/\kappa',2);
set(gca,'XLim',[za zb]);

% print('-depsc2','lawOfTheWallFunction.eps');
