%
% Plot the dropping sphere results
%
clear;
clf;
set(gca,'FontSize',14);
lineWidth=2.; 


sphereTubeRho2Nu0p1

t = x0;
v1 = v10;
v2 = v20;
v3 = v30;

w1 = w10;
w2 = w20;
w3 = w30;

f1 = f10;
f2 = f20;
f3 = f30;



plot(t,v2,'-r');
legend('v_2');
title('Falling sphere in a tube');
grid on;
print('-depsc2','sphereTubeRho2Nu0p1V2.eps');
pause;

plot(t,v1,'-r', t,v3,'-g');
legend('v_1','v_3');
grid on;

pause;

plot(t,w1,'-r', t,w2,'-g', t,w3,'-b');
legend('w_1','w_2','w_3');
ylim([-1 1]);
grid on;

pause;

plot(t,f1,'-r', t,f2,'-g', t,f3,'-b');
legend('f_1','f_2','f_3');
ylim([-1 1]);
grid on;


% Uncomment the next lines to create a plot
% plot(x0,x10,'r-o',x0,x20,'g-x',x0,x30,'b-s',x0,v10,'c-<',x0,v20,'m->',x0,v30,'r-+',x0,w10,'g-o',x0,w20,'b-x',x0,w30,'c-s',x0,f10,'m-<',x0,f20,'r->',x0,f30,'r-o',x0,g10,'g-x',x0,g20,'b-s',x0,g30,'c-<');
% legend('x1','x2','x3','v1','v2','v3','w1','w2','w3','f1','f2','f3','g1','g2','g3');


% print('-depsc2','sosup2dStabilityCurves.eps');