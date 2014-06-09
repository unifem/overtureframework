% baldwinLomax
% 
%  Plot some of the functions from the BL model
%
clear;
set(gca,'FontSize',16);


delta=2.;
ckleb=.3; 
kappa=.4;
alpha=.0168;
A0=26;
ccp=1.6;
cwk=1.;  % or .25 
udif=1.; 
beta=1.;  % y+ = beta*y 

wmax=10.;
scaleo=50; 
scalef=.5/wmax;

n=100;
ya=0.;
yb=10.;
dy=(yb-ya)/(n-1);

y = ya:dy:yb;

fmax=0.;
ymax=0.;

f = zeros(n,1);
fkleb = zeros(n,1);
w = zeros(n,1);
nuTi = zeros(n,1);
nuTo = zeros(n,1);
for i=1:n
  w(i)= wmax*exp(-y(i));
  f(i)= y(i)*w(i)*( 1.-exp(-y(i)*beta) );
  nuTi(i) = w(i)*(kappa*y(i)*( 1.-exp(-y(i)*beta) ))^2;
  if f(i)>fmax 
    fmax=f(i); ymax=y(i); 
  end 
end

delta=ymax/ckleb;
fwake=min(ymax*fmax,cwk*ymax*udif^2/fmax);
for i=1:n
  fkleb(i) = .5/(1. + 5.5*(y(i)/delta)^6 );
  nuTo(i) = scaleo*alpha*ccp*fwake*fkleb(i);
end

plot(y,scalef*f,'r-o',y,w*(.2/wmax),'b-o',y,nuTi,'g-o',y,nuTo,'c-o');
legend('F(y)','w(y)','nuT(inner)','nuT(outer)');
grid on
xlabel('y');
title('Baldwin-Lomax: F(y)','FontSize',18);

% print -depsc2 baldwinLomaxF.eps

