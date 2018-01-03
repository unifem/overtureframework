%
% Plot the dropping cylinder results
%
clear;
clf;
set(gca,'FontSize',14);
lineWidth=2.; 


% nu=.1 
cylDropNu0p1L5


t = x0;
v1 = v10;
v2 = v20;

w3 = w30;

f1 = f10;
f2 = f20;


plot(t,v2,'-r');
legend('v_2');
title('Falling cylinder in a channel');
xlabel('t');
grid on;
print('-depsc2','cylDropNu0p1.eps');
pause;

plot(t,v1,'-r',t,w3,'-g');
legend('v_1','w_3');
xlabel('t');
grid on;

pause;

plot( t,f2,'-g');
legend('f_2');
ylim([-1 1]);
xlabel('t');
grid on;

pause;

plot( t,f1,'-g');
legend('f_1');
% ylim([-1 1]);
xlabel('t');
grid on;


% Uncomment the next lines to create a plot
% plot(x0,x10,'r-o',x0,x20,'g-x',x0,x30,'b-s',x0,v10,'c-<',x0,v20,'m->',x0,v30,'r-+',x0,w10,'g-o',x0,w20,'b-x',x0,w30,'c-s',x0,f10,'m-<',x0,f20,'r->',x0,f30,'r-o',x0,g10,'g-x',x0,g20,'b-s',x0,g30,'c-<');
% legend('x1','x2','x3','v1','v2','v3','w1','w2','w3','f1','f2','f3','g1','g2','g3');


