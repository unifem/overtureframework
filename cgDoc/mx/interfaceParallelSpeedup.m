%
% Plot the parallel speedup: Cgmx Parallel Scaling : 3D Interface Problem.
% 
clear;
clf;
set(gca,'FontSize',18);

n=8;
p=zeros(n,1);
for i=1:n
  p(i)=2^(i-1);
end;

t=zeros(n,1);

n0=1;  % start here np=2
n1=7;  % end here   np=64

t(1)=30.8; 
t(2)=14.1;
t(3)=10.6;
t(4)=5.2;
t(5)=7.8;
t(6)=1.3;
t(7)=.79;

% compute the speed up and parallel scaling factor
speedUp=zeros(n,1); psf=zeros(n,1);
for i=n0:n1
  speedUp(i)=t(i)/t(n0);
  psf(i)=(t(n0)*p(n0))/(t(i)*p(i));
end;



plot(p(n0:n1),speedUp(n0:n1),'b-o',p(n0:n1),psf(n0:n1),'r-x','MarkerSize',15);
title(sprintf('Cgmx Parallel Scaling : 3D Interface Problem'));
legend('Speedup','Scale factor','Location','NorthEast');
xlabel('Number of processors');
set(gca,'XLim',[0,65]);
set(gca,'YLim',[0,1.2]);
grid on;

print('-depsc2','cgmxInterfaceParallelSpeedup.eps');
