%
% Examples:
%   runRigidBody -itest=0 -tf=2. -solver=0
%   runRigidBody -itest=1 -tf=2. -solver=1 
%   runRigidBody -itest=1 -tf=2. -solver=1 -plotOption=0 -freeRotationAxis=1
%   runRigidBody -itest=1 -tf=2. -solver=1 -plotOption=0 -freeRotationAxis=2
%
%   runRigidBody -itest=2 -twilightZone=2 -tf=2. -solver=0   [ trig twilightZone
%   runRigidBody -itest=2 -twilightZone=2 -tf=2. -solver=2   [ trig twilightZone
% 
% -- Leapfrog trapezoidal scheme:
%   runRigidBody -itest=0 -tf=2. -solver=2 -plotOption=0 -cfl=.25
%   runRigidBody -itest=0 -tf=2. -solver=2 -plotOption=0 -cfl=.5
% 
%   runRigidBody -itest=1 -tf=2. -solver=2 -freeRotationAxis=2 -plotOption=1
% 
% -- Added Mass:
%   runRigidBody -itest=2 -twilightZone=2 -tf=2. -solver=1 -addedMass=1 -plotOption=3
%   runRigidBody -itest=2 -twilightZone=2 -tf=2. -solver=2 -addedMass=1 -plotOption=3
%
% -- Compute convergence rates:
%   -- TZ: 
%   runRigidBody -itest=2 -twilightZone=2 -tf=1. -addedMass=1 -conv=1 -numResolutions=4 -solver=1
%   runRigidBody -itest=2 -twilightZone=2 -tf=1. -addedMass=1 -conv=1 -numResolutions=4 -solver=2
%   runRigidBody -itest=2 -twilightZone=2 -tf=1. -addedMass=1 -conv=1 -numResolutions=4 -solver=3
%   runRigidBody -itest=2 -twilightZone=2 -tf=1. -addedMass=1 -conv=1 -numResolutions=4 -solver=4 -dirkOrder=1
%   runRigidBody -itest=2 -twilightZone=2 -tf=1. -addedMass=1 -conv=1 -numResolutions=4 -solver=4 -dirkOrder=2
%   runRigidBody -itest=2 -twilightZone=2 -tf=1. -addedMass=1 -conv=1 -numResolutions=4 -solver=4 -dirkOrder=3
%   runRigidBody -itest=2 -twilightZone=2 -tf=1. -addedMass=1 -conv=1 -numResolutions=4 -solver=4 -dirkOrder=4
% 
%   -- free rotation
%   runRigidBody -itest=2 -itest=1 -freeRotationAxis=1 -tf=1. -conv=1 -numResolutions=5 -solver=1 
%   runRigidBody -itest=2 -itest=1 -freeRotationAxis=1 -tf=1. -conv=1 -numResolutions=5 -solver=2
%   runRigidBody -itest=2 -itest=1 -freeRotationAxis=1 -tf=1. -conv=1 -numResolutions=5 -solver=4 -dirkOrder=2
%
function runWave(varargin)
%%


%clear;
%clf;
set(gca,'FontSize',14);

conv=0; % set to 1 to compute convergence rates

itest=0;
tf=2.;
mass=1.; 
solver=0;
freeRotationAxis=3;
checkErrors=1;
normalize=1;
rtol=1.e-5;
debug=0;
plotOption=1; 
cfl=.25; 
dt0=-1; % use this dt if dt0>0 
addedMass=0; 
twilightZone=0; 
dirkOrder=1; % Backward Euler
numResolutions=2; % number of times to halve the time step for convergence studies

 % --- read command line args ---
 for i = 1 : nargin
   % fprintf ( 1, 'fsi: argument %d is [%s]\n', i, varargin{i} );
   line = varargin{i};
   if( strncmp(line,'-itest=',7) )
     itest = sscanf(varargin{i},'-itest=%d'); 
   elseif( strncmp(line,'-tf=',4) )
     tf = sscanf(varargin{i},'-tf=%e'); 
   elseif( strncmp(line,'-debug=',7) )
     debug = sscanf(varargin{i},'-debug=%d'); 
   elseif( strncmp(line,'-plotOption=',12) )
     plotOption = sscanf(varargin{i},'-plotOption=%d'); 
   elseif( strncmp(line,'-solver=',8) )
     solver = sscanf(varargin{i},'-solver=%d'); 
   %                     12345678901234567890
   elseif( strncmp(line,'-freeRotationAxis=',18) )
     freeRotationAxis = sscanf(varargin{i},'-freeRotationAxis=%d'); 
   elseif( strncmp(line,'-normalize=',11) )
     normalize = sscanf(varargin{i},'-normalize=%d'); 
   elseif( strncmp(line,'-rtol=',6) )
     rtol = sscanf(varargin{i},'-rtol=%e'); 
   elseif( strncmp(line,'-mass=',6) )
     mass = sscanf(varargin{i},'-mass=%e'); 
   elseif( strncmp(line,'-cfl=',5) )
     cfl = sscanf(varargin{i},'-cfl=%e'); 
   elseif( strncmp(line,'-addedMass=',11) )
     addedMass = sscanf(varargin{i},'-addedMass=%d'); 
   elseif( strncmp(line,'-twilightZone=',14) )
     twilightZone = sscanf(varargin{i},'-twilightZone=%d'); 
   elseif( strncmp(line,'-dirkOrder=',11) )
     dirkOrder = sscanf(varargin{i},'-dirkOrder=%d'); 
   elseif( strncmp(line,'-conv=',6) )
     conv = sscanf(varargin{i},'-conv=%d'); 
   elseif( strncmp(line,'-dt0=',5) )
     dt0 = sscanf(varargin{i},'-dt0=%e'); 
   %                     12345678901234567890
   elseif( strncmp(line,'-numResolutions=',16) )
     numResolutions = sscanf(varargin{i},'-numResolutions=%d'); 
   end;
 end

if conv==0 
  rigidBody( itest,tf,solver,cfl,plotOption,debug,checkErrors,normalize,rtol,freeRotationAxis,...
             addedMass,twilightZone,mass,dirkOrder,dt0 );
else
  % compute convergence rates:

  cfl=1.; 
  dtInitial=.1; 
  plotOption=0;
  maxNumErr=10; % maximum number of different errors 
  maxErr = zeros(maxNumErr,numResolutions);
  dtv = zeros(numResolutions);
  for i=1:numResolutions

    dt0 = dtInitial/(2^(i-1)); 
    fprintf('Solve equations with dt=%f\n',dt0);
    [ numErr, errorName, errv ] = rigidBody( itest,tf,solver,cfl,plotOption,debug,checkErrors,...
                                             normalize,rtol,freeRotationAxis,...
                                             addedMass,twilightZone,mass,dirkOrder,dt0 );
    dtv(i)=dt0;
    for j=1:numErr
      maxErr(j,i)=max(abs(errv(:,j)));
    end;

  end;

  solverName = getSchemeName();

  testName='Unknown test';
  if itest==1
    testName=sprintf('FR%d',freeRotationAxis);
  elseif itest==2
    testName=sprintf('TZTrig');
  end;

  if 0==1
  % Output results
  fprintf('\n -------------------------------\n');
  fprintf('Results for %s\n',solverName);
  for i=1:numResolutions
    for j=1:numErr

      ratio = maxErr(j,max(1,i-1))/maxErr(j,i); % ratio==1 for i=1
      fprintf('dt=%6.3f, %s : max-err = %8.2e, ratio=%5.2f\n',dtv(i),errorName(j,:),maxErr(j,i),ratio);
    end;

  end;
  end;


% Output results as a Latex table

fprintf('\\begin{figure}[hbt]\\tableFont %% you should set \\tableFont to \\footnotesize or other size\n');
fprintf('\\begin{center}\n');
fprintf('\\begin{tabular}{|l|');
for j=1:numErr
  fprintf('c|c|');
end
fprintf('} \\hline \n');
fprintf('\\multicolumn{%d}{|c|}{Rigid body, %s, %s}     \\\\ \\hline\n',1+2*numErr,solverName,testName);
% \dt  & %s  & r & %s  & r & %s  & r \\ \hline
fprintf('$\\dt$    ');
for j=1:numErr
  fprintf('& %s &   r   ',errorName(j,:));
end
fprintf('\\\\ \\hline\n'); 
for i=1:numResolutions
  % dt  & err & ratio  & 4.0{e-4} & ratio    \\ \hline
  fprintf('%8.6f ',dtv(i));
  for j=1:numErr
    ratio = maxErr(j,max(1,i-1))/maxErr(j,i); % ratio==1 for i=1
    if i==1
      fprintf('&    %8.2e     &       ',maxErr(j,i));
    else
      fprintf('&    %8.2e     & %4.1f  ',maxErr(j,i),ratio);
    end;
  end
  fprintf('  \\\\ \\hline\n');
end;
%   rate  &   $1.99$  &       & $2.01$   &        \\ \hline
fprintf(' rate    ');
for j=1:numErr
  % dtv(1:numResolutions)
  % maxErr(j,1:numResolutions)
  p = polyfit(log(dtv(1:numResolutions)),log(maxErr(j,1:numResolutions)),1);
  rate = p(1);
  fprintf('&   %5.2f         &       ',rate);
end;
fprintf('   \\\\ \\hline\n');
fprintf('\\end{tabular}\n');
fprintf('\\caption{Newton-Euler Equations: Scheme=%s, test=%s, Max-norm errors at $t=%4.1f$. AddedMass=%d }\n',solverName,testName,tf,addedMass);
fprintf('\\label{tab:Test%s_Scheme%s}\n',testName,solverName);
fprintf('\\end{center}\n');
fprintf('\\end{figure} \n'); 
  

end;