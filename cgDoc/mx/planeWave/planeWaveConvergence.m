%
% Plot convergence rates for the plane wave in free space.
%
clear;
clf;
set(gca,'FontSize',18);

hh = [0.05  0.025 0.0125 ];

% ***************** CGFD2 *******************


E2mx = [.52 .13 .026];

% ***************** CGFD4 *******************


E4mx = [.085 .0067 .00043];

% ***************** YEE *******************


EYmx = [1.3 .28 .066];

loglog(hh,EYmx,'r-x',hh,E2mx,'b-+',hh,E4mx,'g-o');
legend('E (Yee)','E (CGDF2)','E (CGFD4)','Location','SouthEast');


title('Plane Wave in Free Space, Max Norm');
ylabel('Errors');
xlabel('h');
% set(gca,'XLim',[0.00625,0.025]);
set(gca,'XLim',[0.01,0.065]);
set(gca,'YLim',[.1e-3 2.]);

set(gca,'XTick',[0.0125 0.025 0.05 ]);

hl=zeros(2,1); el2=zeros(2,1);  el4=zeros(2,1);
hl(1)=.03; hl(2)=hl(1)/1.5; 
el2(1)=4.e-2; el2(2)=el2(1)*(hl(2)/hl(1))^2; 
el4(1)=4.e-2; el4(2)=el4(1)*(hl(2)/hl(1))^4; 

hold on;

loglog(hl,el2,'k-',hl,el4,'k-','LineWidth',2);

xt=hl(2)*.95; yt=el2(2)*1.0;
text(xt,yt,'2','FontSize',18);
xt=hl(2)*.95; yt=el4(2)*.75;
text(xt,yt,'4','FontSize',18);
grid on;
hold off;

% print('-depsc2','planeWaveConvergenceRatesMaxNorm.eps');

