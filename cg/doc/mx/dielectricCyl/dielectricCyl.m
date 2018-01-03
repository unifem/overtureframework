%
% Plot convergence rates for the dielectric cylinder
%  Extract results from
%       dielectricCylNFDTDOrder2maxNorm.m
%       dielectricCylNFDTDOrder4maxNorm.m
%       dielectricCylYeeOrder2maxNorm.m
%
clear;
clf;
set(gca,'FontSize',18);

hh = [0.025 0.0125 0.00625 ];
Ex2 = [4.04e-02 1.12e-02 2.88e-03 ];
Ey2 = [3.78e-02 9.84e-03 2.54e-03 ];
Hz2 = [6.64e-02 1.80e-02 4.65e-03 ];
divE2 = [1.38e-02 3.73e-03 1.03e-03 ];

Ex4 = [1.17e-04 4.63e-06 5.18e-07 ];
Ey4 = [1.48e-04 1.07e-05 6.16e-07 ];
Hz4 = [1.62e-04 1.14e-05 9.81e-07 ];
divE4 = [1.10e-04 9.46e-06 1.38e-06 ];

ExY= [5.75e-01 5.58e-01 5.93e-01 ];
EyY = [5.07e-01 4.74e-01 5.43e-01 ];
HzY = [1.36e-01 6.01e-02 6.61e-02 ];
divEY = [1.63e+00 1.95e+00 1.80e+00 ];

E2 = max(Ex2,Ey2);
E4 = max(Ex4,Ey4);
EY = max(ExY,EyY);

%loglog(hh,Ex2,'r-+',hh,Ey2,'g-+',hh,Hz2,'b-+',hh,divE2,'c-+',...
%       hh,Ex4,'r-x',hh,Ey4,'g-x',hh,Hz4,'b-x',hh,divE4,'c-x');
%legend('Ex (2)','Ey (2)','Hz (2)','divE (2)','Ex (4)','Ey (4)','Hz (4)','divE (4)','Location','NorthWest');

loglog(hh,EY,'r-x',hh,E2,'b-+',hh,E4,'g-o');
legend('E (Yee)','E (CGDF2)','E (CGFD4)','Location','SouthEast');


title('Dielectric Cylinder Max Norm');
ylabel('Errors');
xlabel('h');
% set(gca,'XLim',[0.00625,0.025]);
set(gca,'XLim',[0.005,0.04]);
set(gca,'YLim',[0.2e-6,1.]);
set(gca,'XTick',[0.00625 0.0125 0.025 ]);

hl=zeros(2,1); el2=zeros(2,1);  el4=zeros(2,1);
hl(1)=.015; hl(2)=hl(1)/1.5; 
el2(1)=1.e-4; el2(2)=el2(1)*(hl(2)/hl(1))^2; 
el4(1)=1.e-4; el4(2)=el4(1)*(hl(2)/hl(1))^4; 

hold on;

loglog(hl,el2,'k-',hl,el4,'k-','LineWidth',2);

xt=hl(2)*.95; yt=el2(2)*1.5;
text(xt,yt,'2','FontSize',18);
xt=hl(2)*.95; yt=el4(2)*.75;
text(xt,yt,'4','FontSize',18);
grid on;
hold off;

% print('-depsc2','dielectricCylConvergenceRatesMaxNorm.eps');
pause;

% ==================== L1 ===================

Ex2 = [5.00e-03 1.33e-03 3.35e-04 ];
Ey2 = [7.49e-03 1.87e-03 4.64e-04 ];
Hz2 = [9.19e-03 2.29e-03 5.68e-04 ];
divE2 = [1.38e-02 3.73e-03 1.03e-03 ];

Ex4 = [1.01e-05 6.83e-07 7.06e-08 ];
Ey4 = [2.56e-05 2.04e-06 1.72e-07 ];
Hz4 = [2.34e-05 1.80e-06 1.61e-07 ];
divE4 = [1.10e-04 9.46e-06 1.38e-06 ];

ExY = [9.21e-03 4.95e-03 2.97e-03 ];
EyY = [1.07e-02 5.32e-03 3.15e-03 ];
HzY = [1.24e-02 5.90e-03 3.57e-03 ];
divEY = [1.63e+00 1.95e+00 1.80e+00 ];

E2 = max(Ex2,Ey2);
E4 = max(Ex4,Ey4);
EY = max(ExY,EyY);

loglog(hh,EY,'r-x',hh,E2,'b-+',hh,E4,'g-o');
legend('E (Yee)','E (CGDF2)','E (CGFD4)','Location','SouthEast');

title('Dielectric Cylinder L1 Norm');
ylabel('Errors');
xlabel('h');
% set(gca,'XLim',[0.00625,0.025]);
set(gca,'XLim',[0.005,0.04]);
set(gca,'YLim',[.2e-7,.05]);
set(gca,'XTick',[0.00625 0.0125 0.025 ]);

hl=zeros(2,1); el2=zeros(2,1);  el4=zeros(2,1);
hl(1)=.015; hl(2)=hl(1)/1.5; 
el2(1)=1.e-4; el2(2)=el2(1)*(hl(2)/hl(1))^2; 
el4(1)=1.e-4; el4(2)=el4(1)*(hl(2)/hl(1))^4; 

hold on;

loglog(hl,el2,'k-',hl,el4,'k-','LineWidth',2);

xt=hl(2)*.925; yt=el2(2)*1.5;
text(xt,yt,'2','FontSize',18);
xt=hl(2)*.925; yt=el4(2)*.75;
text(xt,yt,'4','FontSize',18);

grid on;
hold off;
% print('-depsc2','dielectricCylConvergenceRatesL1Norm.eps');
