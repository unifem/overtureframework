%
%  Plot flat plate results
%
clear; clf;  
set(gca,'FontSize',14);

lineWidth=2; 

flatPlateOrder4Res4


plot(x0,u0,'r-', x0,100*u_err0,'b-', x0,100*v0,'g-',  x0,10000*v_err0,'k-','LineWidth',lineWidth );
% plot(x0,u0,'r-', x0,100*u_err0,'b-', x0,100*v0,'g-',  x0,10000*v_err0,'k-', x0,-vorticity0,'c-' );
grid on;
legend('u','uErr*10^2', 'v*10^2', 'vErr*10^4','Location','NorthWest' );
% legend('u','u-true', 'u-err*100', '100*v', '100*v-true', '10000*v-err' );
title('Flat plate boundary layer');
xlabel('y');

print('-depsc2','flatPlateProfilesOrder4Res4.eps'); % save as an eps file

% plot(x0,u0,'r-o',x0,v0,'g-x',x0,vorticity0,'b-s',x0,u-err0,'c-<',x0,v-err0,'m->',x0,p-err0,'r-+',x0,x00,'g-o',x0,x10,'b-x');
% legend('u','v','vorticity','u-err','v-err','p-err','x0','x1');
