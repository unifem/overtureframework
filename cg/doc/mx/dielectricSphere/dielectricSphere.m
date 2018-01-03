%
% Plot convergence rates for the dielectric sphere
%  Extract results from
%    solidSphere.order4.out
%
clear;
clf;
set(gca,'FontSize',18);

hh = [0.05  0.025 0.0125 ];

% ***************** CGFD2 *******************
% solidSphereInABoxe2.order2.diss11.out
% -->t=1.0000e+00 dt=2.0e-02 maxNorm errors:[2.3310e-01,5.7381e-01,1.9767e-01,], maxNorm (u):[1.55e+00,3.73e+00,7.11e-01,]
% -->t=1.0000e+00 dt=2.0e-02 l1-norm errors:[8.0259e-03,3.0265e-02,6.1653e-03,], l1-norm (u):[8.50e-02,5.74e-01,5.30e-02,]

% solidSphereInABoxe4.order2.diss11.out
% -->t=1.0000e+00 dt=1.0e-02 maxNorm errors:[4.8566e-02,1.3528e-01,4.5779e-02,], maxNorm (u):[1.72e+00,4.30e+00,8.59e-01,]
% -->t=1.0000e+00 dt=1.0e-02 l1-norm errors:[1.8386e-03,7.0820e-03,1.4537e-03,], l1-norm (u):[9.17e-02,5.94e-01,5.91e-02,]

% solidSphereInABoxe8.order2.diss11.out  : numberOfStepsTaken =      180
% advance.............................  1.01e+02    5.63e-01    1.47e-08    79.657   1.016e+02   1.012e+02
% -->t=1.0000e+00 dt=5.6e-03 maxNorm errors:[1.2892e-02,3.6382e-02,1.2181e-02,], maxNorm (u):[1.75e+00,4.41e+00,8.87e-01,]
% -->t=1.0000e+00 dt=5.6e-03 l1-norm errors:[4.6548e-04,1.7997e-03,3.5791e-04,], l1-norm (u):[9.28e-02,6.02e-01,5.84e-02,]

E2mx = [5.7381e-01 1.3528e-01 3.6382e-02 ];
E2l1 = [3.0265e-02 7.0820e-03 1.7997e-03 ];

% ***************** CGFD4 *******************

% solidSphereInABoxe2.order4.diss48.out
% -->t=1.0000e+00 dt=2.6e-03 maxNorm errors:[6.6218e-01,9.4795e-01,3.5353e-01,], maxNorm (u):[1.15e+00,3.34e+00,5.45e-01,]
% -->t=1.0000e+00 dt=2.6e-03 l1-norm errors:[1.6881e-02,5.5788e-02,1.2213e-02,], l1-norm (u):[7.64e-02,5.63e-01,4.83e-02,]
%   Timings:         (ave-sec/proc:)   seconds    sec/step   sec/step/pt     %     [max-s/proc] [min-s/proc]
% advance.............................  2.64e+01    6.76e-02    9.29e-08    94.862   2.639e+01   2.633e+01

% solidSphereInABoxe4.order4.diss48.out  : 
% -->t=1.0000e+00 dt=2.5e-03 maxNorm errors:[3.6915e-02,7.0869e-02,2.7603e-02,], maxNorm (u):[1.73e+00,4.36e+00,8.75e-01,]
% -->t=1.0000e+00 dt=2.5e-03 l1-norm errors:[8.5634e-04,2.0773e-03,7.0198e-04,], l1-norm (u):[9.24e-02,5.98e-01,6.02e-02,]

% solidSphereInABoxe8.order4.diss48.out : gridpts =3.88029e+07 , numberOfStepsTaken =      450
% ***** getTimeStep: Correct for art. dissipation: new dt=3.468e-03 (old = 1.035e-02, new/old=0.33)
% -->t=1.0000e+00 dt=2.2e-03 maxNorm errors:[1.7460e-03,3.4487e-03,1.1624e-03,], maxNorm (u):[1.76e+00,4.44e+00,8.96e-01,]
% -->t=1.0000e+00 dt=2.2e-03 l1-norm errors:[3.9052e-05,8.5734e-05,2.9994e-05,], l1-norm (u):[9.36e-02,6.00e-01,6.15e-02,]
% advance.............................  6.32e+02    1.40e+00    3.62e-08    95.836   6.322e+02   6.314e+02

% old E4mx = [9.4795e-01 7.0869e-02 3.4487e-03 ];
% old E4l1 = [5.5788e-02 2.0773e-03 8.5734e-05 ];


% ***************** CGFD4 dissOrder=8 with lower cfl *******************

% solidSphereInABoxe2.order4.dissOrder8new.cfl0p5.out
% -->t=1.0000e+00 dt=1.3e-02 maxNorm errors:[3.0799e-02,4.3991e-02,2.0117e-02,], maxNorm (u):[1.74e+00,4.33e+00,8.73e-01,]
% -->t=1.0000e+00 dt=1.3e-02 l1-norm errors:[5.0031e-04,1.4992e-03,2.9228e-04,], l1-norm (u):[9.11e-02,6.04e-01,5.92e-02,]

% solidSphereInABoxe4nrMin15.order4.dissOrder8new.out
% -->t=1.0000e+00 dt=5.6e-03 maxNorm errors:[1.7667e-03,2.2152e-03,1.5612e-03,], maxNorm (u):[1.74e+00,4.43e+00,8.97e-01,]
% -->t=1.0000e+00 dt=5.6e-03 l1-norm errors:[2.9740e-05,8.6697e-05,2.1925e-05,], l1-norm (u):[8.81e-02,6.10e-01,5.35e-02,]

% solidSphereInABoxe8nrMin15.order4.dissOrder8new.cfl0p33.out
% -->t=1.0000e+00 dt=2.4e-03 maxNorm errors:[2.3741e-04,3.2209e-04,2.0709e-04,], maxNorm (u):[1.76e+00,4.45e+00,8.97e-01,]
% -->t=1.0000e+00 dt=2.4e-03 l1-norm errors:[5.1305e-06,5.6745e-06,3.7575e-06,], l1-norm (u):[9.25e-02,6.05e-01,5.77e-02,]

E4mx = [4.3991e-02 2.2152e-03 3.2209e-04];
E4l1 = [1.4992e-03 8.6697e-05 5.6745e-06];

% ***************** YEE *******************
% solidSphereInABoxe2.Yee.out
% -->t=1.0000e+00 dt=2.0e-02 maxNorm errors:[3.2271e-01,1.8389e-01,1.5882e-01,1.5824e-01,7.8629e-02,2.7641e-01,], maxNorm (u):[1.78e+00,4.36e+00,8.91e-01,1.42e+00,1.59e+00,6.42e+00,]
% -->t=1.0000e+00 dt=2.0e-02 l1-norm errors:[3.0282e-03,1.0944e-02,1.7026e-03,3.4667e-03,1.8557e-03,1.2662e-02,], l1-norm (u):[1.17e-01,5.99e-01,6.71e-02,2.05e-01,1.02e-01,6.66e-01,]

% solidSphereInABoxe4.Yee.out
% -->t=1.0000e+00 dt=1.1e-02 maxNorm errors:[2.7325e-01,1.6915e-01,1.4274e-01,7.1422e-02,6.0727e-02,1.2607e-01,], maxNorm (u):[1.77e+00,4.44e+00,9.02e-01,1.43e+00,1.56e+00,6.40e+00,]
% -->t=1.0000e+00 dt=1.1e-02 l1-norm errors:[1.2817e-03,2.9829e-03,8.2458e-04,1.0651e-03,8.4835e-04,3.3860e-03,], l1-norm (u):[1.17e-01,6.03e-01,6.79e-02,2.09e-01,1.04e-01,6.63e-01,]
% solidSphereInABoxe8.Yee.out : gridpts =3.43281e+07  numberOfStepsTaken =      180
% -->t=1.0000e+00 dt=5.6e-03 maxNorm errors:[2.9071e-01,1.7522e-01,1.5764e-01,4.5429e-02,6.3941e-02,9.2176e-02,], maxNorm (u):[1.77e+00,4.45e+00,9.32e-01,1.44e+00,1.58e+00,6.40e+00,]
% -->t=1.0000e+00 dt=5.6e-03 l1-norm errors:[6.5864e-04,1.0016e-03,4.6018e-04,4.5332e-04,4.7968e-04,1.1149e-03,], l1-norm (u):[1.17e-01,6.05e-01,6.82e-02,2.11e-01,1.04e-01,6.68e-01,]
% advance.............................  5.81e+01    3.23e-01    9.40e-09    52.296   5.831e+01   5.775e+01

EYmx = [3.2271e-01 2.7325e-01 2.9071e-01 ];
EYl1 = [1.0944e-02 2.9829e-03 1.0016e-03 ];

loglog(hh,EYmx,'r-x',hh,E2mx,'b-+',hh,E4mx,'g-x');
legend('E (Yee)','E (CGDF2)','E (CGFD4)','Location','SouthEast');


title('Dielectric Sphere Max Norm');
ylabel('Errors');
xlabel('h');
% set(gca,'XLim',[0.00625,0.025]);
set(gca,'XLim',[0.01,0.065]);
set(gca,'XTick',[0.0125 0.025 0.05 ]);

hl=zeros(2,1); el2=zeros(2,1);  el4=zeros(2,1);
hl(1)=.03; hl(2)=hl(1)/1.5; 
el2(1)=4.e-2; el2(2)=el2(1)*(hl(2)/hl(1))^2; 
el4(1)=4.e-2; el4(2)=el4(1)*(hl(2)/hl(1))^4; 

hold on;

loglog(hl,el2,'k-',hl,el4,'k-','LineWidth',2);

xt=hl(2)*.95; yt=el2(2)*1.3;
text(xt,yt,'2','FontSize',18);
xt=hl(2)*.95; yt=el4(2)*.75;
text(xt,yt,'4','FontSize',18);
grid on;
hold off;

% print('-depsc2','dielectricSphereConvergenceRatesMaxNorm.eps');
pause;

% ==================== L1 ===================

loglog(hh,EYl1,'r-x',hh,E2l1,'b-+',hh,E4l1,'g-x');
legend('E (Yee)','E (CGDF2)','E (CGFD4)','Location','SouthEast');

title('Dielectric Sphere L1 Norm');
ylabel('Errors');
xlabel('h');
% set(gca,'XLim',[0.00625,0.025]);
set(gca,'XLim',[0.01,0.065]);
set(gca,'YLim',[1.e-6,.1]);
set(gca,'XTick',[0.0125 0.025 0.05 ]);

hl=zeros(2,1); el2=zeros(2,1);  el4=zeros(2,1);
hl(1)=.03; hl(2)=hl(1)/1.5; 
el2(1)=1.e-3; el2(2)=el2(1)*(hl(2)/hl(1))^2; 
el4(1)=1.e-3; el4(2)=el4(1)*(hl(2)/hl(1))^4; 

hold on;

loglog(hl,el2,'k-',hl,el4,'k-','LineWidth',2);

xt=hl(2)*.925; yt=el2(2)*1.25;
text(xt,yt,'2','FontSize',18);
xt=hl(2)*.925; yt=el4(2)*.75;
text(xt,yt,'4','FontSize',18);

grid on;
hold off;
% print('-depsc2','dielectricSphereConvergenceRatesL1Norm.eps');
