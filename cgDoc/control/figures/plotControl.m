%
%  plot time series results from control problems
%

clear;
clf;
set(gca,'FontSize',16);

lineWidth=2; 

icase=1; 
icase=2; 
%icase=3; 
%icase=4; 

if icase==1
  name = 'Flow Past a Cylinder, K_i=1'; 
  plotName = 'FlowCylGain1.eps';
  % file from runs/cgins/control: 
  controlCicGain1;
  t1 = x0;
  Tcontrol1 = Tcontrol0;
  Tbar1 = Tbar0;
  Tset1 = Tset0;
elseif icase==2

  name = 'Flow Past a Cylinder, K_i=2'; 
  plotName = 'FlowCylGain2.eps';
  % file from runs/cgins/control: 
  controlCicGain2;
  t1 = x0;
  Tcontrol1 = Tcontrol0;
  Tbar1 = Tbar0;
  Tset1 = Tset0;

elseif icase==3

  name = 'Flow Past a Cylinder, K_p=1, K_i=1'; 
  plotName = 'FlowCylGainKp1Ki1.eps';
  % file from runs/cgins/control: 
  controlKp1Ki1
  t1 = x0;
  Tcontrol1 = control0;
  Tbar1 = sensor0;
  Tset1 = setPoint0;

elseif icase==4

  name = 'Heated Room'; 
  plotName = 'HeatedRoom2d4.eps';
  % file from runs/cgins/control: 
  controlHR4
  t1 = x0;
  Tcontrol1 = control0;
  Tbar1 = sensor0;
  Tset1 = setPoint0;

end;


plot(t1,Tcontrol1,'r-',t1,Tbar1,'g-',t1,Tset1,'b-','LineWidth',lineWidth);
legend('T_{inflow}','T_{ave}','T_{set}');
title(sprintf('Control Variables, %s',name));
grid on
axis tight
xlabel('t');
% ylim([5e-4,1.]);

fprintf('Saving file=[%s]\n',plotName);
print('-depsc2',plotName);


% Uncomment the next lines to create a plot
% plot(x0,Tcontrol0,'r-o',x0,Tbar0,'g-x',x0,Tset0,'b-s');
% legend('Tcontrol','Tbar','Tset');