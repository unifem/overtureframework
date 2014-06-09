%
% Plot errors and convergence rate for various cases:
%      1. dielectric sphere 
% 
clear;
clf;
set(gca,'FontSize',18);

% grid  & N &  $E_x$ &  $E_y$ & $E_z$ & $\grad\cdot\Ev/\grad\Ev$\\ \hline 
%   solidSphereInABox1 &     1 & ~$1.5\times10^{ -1}$~ & ~$6.1\times10^{ -1}$~ & ~$9.1\times10^{ -2}$~ & ~$2.2\times10^{ -1}$~  \\ \hline
%   solidSphereInABox2 &     2 & ~$4.5\times10^{ -2}$~ & ~$1.8\times10^{ -1}$~ & ~$2.9\times10^{ -2}$~ & ~$8.5\times10^{ -2}$~  \\ \hline
%   solidSphereInABox4 &     4 & ~$1.2\times10^{ -2}$~ & ~$4.8\times10^{ -2}$~ & ~$8.3\times10^{ -3}$~ & ~$2.4\times10^{ -2}$~  \\ \hline
%     rate             &       &       $1.81$          &       $1.83$          &       $1.72$          &       $1.61$           \\ \hline

n=3;
h=zeros(n,1);
ex=zeros(n,1);
ey=zeros(n,1);
ex=zeros(n,1);
ed=zeros(n,1);
h(1)=.15; h(2)=h(1)/2.; h(3)=h(2)/2.;

ex(1)=1.5e-1; ex(2)=4.5e-2; ex(3)=1.2e-2; 
ey(1)=6.1e-1; ey(2)=1.8e-1; ey(3)=4.8e-2; 
ez(1)=9.1e-2; ez(2)=2.9e-2; ez(3)=8.3e-3; 
ed(1)=2.2e-1; ed(2)=8.5e-2; ed(3)=2.4e-2; 


hl=zeros(2,1); el=zeros(2,1);
hl(1)=1./15.; hl(2)=hl(1)/1.5; 
el(1)=.1; el(2)=el(1)*(hl(2)/hl(1))^2; 


loglog(h,ex,'b-+',h,ey,'r-o',h,ez,'g-*',h,ed,'c-x','MarkerSize',15);
title(sprintf('Scattering from a dielectric sphere : errors'));
legend('E_x error','E_y error','E_z error','div error','Location','NorthWest');
xlabel('\Delta x');
ylabel('Maximum error');

hold on;

loglog(hl,el,'k-','LineWidth',2);

xt=.045; yt=.1;
text(xt,yt,'Slope 2','FontSize',18);

set(gca,'XLim',[h(3)/1.25,h(1)*1.25]);
set(gca,'YLim',[5.e-3,.75]);
grid on;
hold off;

print('-depsc2','dielectricSphereConvergence.eps');

