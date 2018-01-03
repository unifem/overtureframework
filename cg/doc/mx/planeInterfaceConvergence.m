%
% Plot errors and convergence rate for scattering from a plane interface
% 
clear;
clf;
set(gca,'FontSize',18);


n=3; % number of grid resolutions
h=zeros(n,1);
ep=zeros(n,1);
eu=zeros(n,1);
ev=zeros(n,1);
ew=zeros(n,1);
eT=zeros(n,1);
ed=zeros(n,1);
eTs=zeros(n,1);
h(1)=.1/2; h(2)=.1/4; h(3)=.1/8; 

 % -- 
 % errors in solid displacement 
 ex(1)=8.0e-2; ex(2)=2.2e-2; ex(3)=5.6e-3; 
 ey(1)=2.4e-1; ey(2)=6.5e-2; ey(3)=1.7e-2; 
 ez(1)=7.7e-3; ez(2)=2.0e-3; ez(3)=4.9e-4; 

hl=zeros(2,1); el=zeros(2,1); el2=zeros(2,1);
hl(1)=.15/4; hl(2)=hl(1)/2.; 
el2(1)=.3; el2(2)=el2(1)*(hl(2)/hl(1))^2; 


loglog(h,ex,'b-+',h,ey,'r-o',h,ez,'g-x');
title(sprintf('Scattering from a plane interface: maximum errors'));
legend('E_x','E_y','E_z','Location','SouthEast');
xlabel('\Delta s');

hold on;

loglog(hl,el,'k-',hl,el2,'k-','LineWidth',2);

xt=sqrt(hl(1)*hl(2)); yt=1.25*sqrt(el2(1)*el2(2));
ht = text(xt,yt,'Slope 2','FontSize',18,'HorizontalAlignment','right');

set(gca,'XLim',[.01,.06]);
% set(gca,'YLim',[2.e-4,.02]);
grid on;
hold off;

% print('-depsc2','planeInterfaceConvergence.eps');

