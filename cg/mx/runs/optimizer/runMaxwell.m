%
%  Run the Maxwell Solver CgMx and return the requested results
%
% Usage:
% 
% Parameters:
%   caseName    (input) : ['cyl'|'block'|'blockWidth'|'lens'],
%   tFinal      (input) : final time
%   probeType   (input) : ['point','transmission'] 
%   gridFactor  (input) : =1,2,4,8 - grid is this much finer than coarsest grid available. Usually dx=1/(10*gridFactor)
%   infolevel   (input) : 
%   plotOption  (input) : 
%   par(1:)     (input) : input parameters
% 
%   values      (output) : array of output values
% 
% Examples
%
% 
function [ values ]  = runMaxwell( caseName,tFinal,probeType,gridFactor,infoLevel,plotOption, par )

 % Define some global variables to avoid pass so many args to runMaxwell
 globalDeclarations

 iteration = iteration+1; % counts the number of times runMaxwell is called

% Overture = getenv('Overture')
 % Here is cgmx:  *fix me* 
 cgmx = '/Users/henshaw/cg.g/mx/bin/cgmx';
 % Here is Ogen
 ogen = '/Users/henshaw/Overture.g/bin/ogen';

 % Here is plotStuff
 plotStuff = '/Users/henshaw/Overture.g/bin/plotStuff';

 fontSize=14;  lineWidth=2;  markerSize=5;   % for plots
 gridName='none'; 
 showFileName='optimizer.show'; 
 
 % OLD pointProbe=0; transmissionProbe=1; % probe types 

  values(1)=0; values(2)=0; 

  if( strcmp(caseName,'block') )

    % ----------------------------------------------------
    % ---- BLOCK: scattering from a dielectric block -----
    % ----------------------------------------------------

    kx=par(1);
    eps1=par(2); 

    fprintf('runMaxwell: it=%d, caseName=%s, tFinal=%9.3e probeType=%s, eps1=%g, kx=%g plotOption=%d\n',iteration,caseName,tFinal,probeType,eps1,kx,plotOption);


    titleLabel=sprintf('Block: eps1=%g, kx=%i',eps1,kx);
    
    cgmxCommand = sprintf('%s -noplot dielectricBodies -g=dielectricBlockGrid2de%d.order4 -backGround=leftBackGround -rbc=rbcNonLocal -kx=%i -eps1=%g -eps2=1. -diss=2 -tf=%g -tp=.1 -probeFileName=OptProbe -go=go >! cgmxOptimzer.out ',cgmx,gridFactor,kx,eps1,tFinal);

    titleLabel=sprintf('Dielectric block: kx=%i, eps1=%g',kx,eps1);

    if( strncmp(probeType,'transmission',length('transmission')) )
      % reflection/transmission probes:
      probeDataFile = 'OptProbe.dat';
    else
      % point probes:
      probeDataFile = 'leftOptProbe.dat';
    end;

  elseif( strcmp(caseName,'blockWidth') ) 

    % ----------------------------------------------------------------------------
    % ---- BLOCKWIDTH: scattering from a dielectric block that changes width -----
    % ----------------------------------------------------------------------------

    kx=par(1);
    eps1=par(2); 
    blockWidth=par(3);

    fprintf('runMaxwell: caseName=%s, RENGERATE THE GRID blockWidth=%g\n',caseName,blockWidth);
    % Note: new name chosen for grid: 
    ogenCommand = sprintf('%s -noplot dielectricBlockGrid2d -prefix=dieBlockOpt -interp=e -order=4 -width=%g -factor=%d >&! ogenOpt.out',...
                          ogen,blockWidth,gridFactor);
    if( infoLevel>0 )  fprintf('Run ogen: %s\n',ogenCommand); end; 
    system(ogenCommand);
    if( rt ~= 0 )
      fprintf('runMaxwell:ERROR return from ogen: rt=[%d]\n',rt);
      pause; pause; pause;
    end
    if( infoLevel>0 ) fprintf('..done ogen\n'); end; 

    fprintf('runMaxwell: caseName=%s, tFinal=%9.3e probeType=%s, eps1=%g, kx=%g blockWidth=%g \n',...
             caseName,tFinal,probeType,eps1,kx,blockWidth);


    titleLabel=sprintf('Block: eps1=%g, kx=%i, width=%g',eps1,kx,blockWidth);
    
    cgmxCommand = sprintf('%s -noplot dielectricBodies -g=dieBlockOpte%d.order4 -backGround=leftBackGround -rbc=rbcNonLocal -kx=%i -eps1=%g -eps2=1. -diss=2 -tf=%g -tp=.1 -probeFileName=OptProbe -go=go >! cgmxOptimzer.out ',cgmx,gridFactor,kx,eps1,tFinal);

    titleLabel=sprintf('Dielectric block: kx=%i, eps1=%g, width=%g',kx,eps1,blockWidth);

    if( strncmp(probeType,'transmission',length('transmission')) )
      % reflection/transmission probes:
      probeDataFile = 'OptProbe.dat';
    else
      % point probes:
      probeDataFile = 'leftOptProbe.dat';
    end;
     
  elseif( strcmp(caseName,'cyl') )

    % ----------------------------------------------------
    % CYL: ---- scattering from a PEC cylinder ---
    % ----------------------------------------------------

    kx=par(1);
    eps1=par(2); 
    titleLabel=sprintf('cylScat: kx=%i',kx);

    fprintf('runMaxwell: caseName=%s, tFinal=%9.3e probeType=%s, eps1=%g, kx=%g \n',caseName,tFinal,probeType,eps1,kx);

    cgmxCommand = sprintf('%s -noplot cylOpt -g=cice%d.order4.hdf -probeFileName=OptProbe -tf=%g -tp=.1 -kx=%g -go=go >! cgmxOptimzer.out ',...
         cgmx,gridFactor,tFinal,kx);
 
    probeDataFile = 'rightOptProbe.dat';


  elseif( strcmp(caseName,'lens') )

    % ----------------------------------------------------
    % LENS: ---- adjust the shape of a lens  -------------
    % ----------------------------------------------------

    kx=par(1);
    eps1=par(2); 
    dxLeft=par(3);   % shift left control point
    dxRight=par(4);  % shift right control point 
    titleLabel=sprintf('Lens: kx=%i, eps1=%g, dxLeft=%g, dxRight=%g',kx,eps1,dxLeft,dxRight);

    fprintf('runMaxwell: caseName=%s, RENGERATE THE GRID dxLeft=%g, dxRight=%g\n',caseName,dxLeft,dxRight);
    % NOTE: for now we just shift one control point at the center of the left and right sides.
    % Note: new name chosen for grid: 
    ogenCommand = sprintf('%s -noplot curvedBlockGrid2d -prefix=lensOptGrid -order=4 -interp=e -width=.25 -interfaceGridWidth=.4 -dxLeft=0 0 0 %g 0 0 0 -dxRight=0 0 0 %g 0 0 0 -factor=%d >&! ogenOpt.out',...
                          ogen,dxLeft,dxRight,gridFactor);
    if( infoLevel>0 )  fprintf('Run ogen: %s\n',ogenCommand); end; 
    rt = system(ogenCommand);
    if( rt ~= 0 )
      fprintf('runMaxwell:ERROR return from ogen: rt=[%d]\n',rt);
      pause; pause; pause;
    end
    if( infoLevel>0 ) fprintf('..done ogen\n'); end; 


    if( infoLevel>0 ) fprintf('runMaxwell: caseName=%s, tFinal=%9.3e probeType=%s, eps1=%g, kx=%g dxLeft=%g dxRight=%g\n',caseName,tFinal,probeType,eps1,kx,dxLeft,dxRight); end; 

    gridName = sprintf('lensOptGride%d.order4.hdf',gridFactor); 
    cgmxCommand = sprintf('%s -noplot dielectricBodies -g=%s -probeFileName=OptProbe -tf=%g -tp=.5 -kx=%g -backGround=leftBackGround -rbc=rbcNonLocal -eps1=%g -eps2=1. -diss=2 -xb=-1. -show=%s -go=go >! cgmxOptimzer.out ',...
         cgmx,gridName,tFinal,kx,eps1,showFileName);
 
    probeDataFile = 'OptProbe.dat';

  else
    fprintf('Unknown caseName=[%s]\n',caseName)
    pause; 
  end;


  
  % ----------------------------------------
  % ----------- RUN CGMX -------------------
  % ----------------------------------------
  if( infoLevel>0 )
    fprintf('Run cgmx...\n');
    fprintf('>> %s\n',cgmxCommand);
  end;
  rt = system(cgmxCommand);
  if( rt ~= 0 )
    fprintf('runMaxwell:ERROR return from cgmx: rt=[%d]\n',rt);
    pause; pause; pause;
  end

%     system(sprintf('/Users/henshaw/cg.g/mx/bin/cgmx -noplot cylOpt -g=cice4.order4.hdf -probeFileName=OptProbe -tf=1 -tp=.1 -kx=%g -go=go >! cgmxOptimizer.out ',kx)); 

  if( infoLevel>0 )
    fprintf('...done\n');
  end;

  % ----------------------------------------------------
  % ----------- Optionally plot the grid ---------------
  % ----------------------------------------------------

  if( plotGrid==1 && strcmp(caseName,'lens') )
    fprintf('Plot the current grid=[%s]...\n',gridName);
    plotName=sprintf('lensGridIteration%d.ps',iteration); 
    system(sprintf('%s plotCurrentGrid.cmd -show=%s -plotName=%s>! plotGrid.out',plotStuff,gridName,plotName)); 
  end;

  if( plotSolution==1 && strcmp(caseName,'lens') )
    fprintf('Plot the solution, showFileName=[%s]...\n',showFileName);
    system(sprintf('%s plotSolution.cmd -show=%s >! plotSolution.out',plotStuff,showFileName)); 
  end;


  % fileName='/Users/henshaw/runs/mx/optimizer/leftOptProbe.dat';
  % fileName='rightOptProbe.dat';
  % fileName='leftOptProbe.dat';


  figure(1); 
  if( strncmp(probeType,'point',length('point')) )

    % --- PLOT POINT PROBE RESULTS ----

    if( infoLevel>0 )
      fprintf('POINT-PROBE: Read probe file = [%s]\n',probeDataFile)
    end; 
    referenceFile=0; 
    [ t, Ex, Ey, Hz ] = getCgMxProbeData( probeDataFile, referenceFile,infoLevel  );

    fprintf('Plot probe data...\n')
    plot(t,Ex,'r-', t,Ey,'g-', t,Hz,'b-','LineWidth',lineWidth );
    title(titleLabel); 
    legend('E_x','E_y','H_z' ); set(gca,'FontSize',fontSize);
    xlabel('t');
    grid on;

  elseif( strncmp(probeType,'transmission',length('transmission')) )

    % --- PLOT REFLECTION/TRANSMISSION PROBE RESULTS ----

    if( infoLevel>0 )
      fprintf('REFLECTION/TRANSMISSION-PROBE: Read probe file = [%s]\n',probeDataFile)
    end; 
    [ t, Rr, Ri, Tr, Ti ] = getCgMxProbeReflectionTransmissionData( probeDataFile, infoLevel  );

    Rnorm = sqrt( Rr.^2 + Ri.^2 );
    Tnorm = sqrt( Tr.^2 + Ti.^2 );
    rtNorm = Rnorm.^2 + Tnorm.^2; 

    % google 'matlab colrs rgb' --> CSS3 color names
    % myColours = [rgb('Crimson'); rgb('Red'); rgb('Orange'); rgb('Blue'); rgb('DodgerBlue'); rgb('Turquoise')];
    % myColours = [rgb('Red'); rgb('OrangeRed'); rgb('Orange'); rgb('Blue'); rgb('DodgerBlue'); rgb('Turquoise')];
    % set(gcf,'DefaultAxesColorOrder',myColours); 

    plot(t,Rr,'-.',t,Ri,':',t,Rnorm,'-', ...
         t,Tr,'-.',t,Ti,':',t,Tnorm,'-', ...
         t,rtNorm,'k-', ...
         'LineWidth',lineWidth,'MarkerSize',markerSize);

    legend('R_r','R_i','|R|', 'T_r','T_i','|T|','|R|^2+|T|^2','Location','NorthWest');

    set(gca,'FontSize',fontSize);
    xlab =xlabel('time');                      % add axis labels and plot title
    set(xlab, 'Units', 'Normalized', 'Position', [.5, -0.05, 0]); % shifty x label upward

    title(titleLabel);
    grid on;
    drawnow;
    if( plotOption>0 )
      plotFileName=sprintf('%sReflectionTransmission.eps',caseName);
      if( infoLevel>0 ) fprintf('runMaxwell: save plot: %s\n',plotFileName); end; 
      print('-depsc2',plotFileName); % save as an eps file
    end;   
    if( infoLevel>0 )
      fprintf('t=%9.3e: R=(%12.5e,%12.5e) |R|=%12.5e, T=(%12.5e,%12.5e) |T|=%12.5e, \n',...
             t(end), ...
            Rr(end),Ri(end),Rnorm(end), ...
            Tr(end),Ti(end),Tnorm(end) );
    end; 

    % ------- DEFINE THE OBJECTIVE -----------
    if( strcmp(objective,'minimizeReflection') || strcmp(objective,'none') )

      % -- Objective: minimize the reflection 
      values(1)=Rnorm(end); % reflection coefficient
      values(2)=Tnorm(end); % transmission coefficient

   elseif( strcmp(objective,'targetTransmission') )

      % -- Objective: minimize the error between the transmission coeff and the target transmission
      [ tTarget, RrTarget, RiTarget, TrTarget, TiTarget ] = getCgMxProbeReflectionTransmissionData( targetFile, infoLevel  );

      fprintf('runMaxwell: T=[%g,%g] : target: T=[%g,%g]\n',Tr(end),Ti(end),TrTarget(end),TiTarget(end)); 

      Rdiff = sqrt( (Rr(end)-RrTarget(end))^2 + (Ri(end)-RiTarget(end))^2 );
      Tdiff = sqrt( (Tr(end)-TrTarget(end))^2 + (Ti(end)-TiTarget(end))^2 );

      values(1)=Rdiff; 
      values(2)=Tdiff; 


   else
      fprintf('runMaxwell: ERROR: unknown objective =[%s]\n',objective);
      pause; pause;
   end;

  else
    fprintf('ERROR: unknown probeType=[%s]\n',probeType )
  end; 
  if( infoLevel>0 )
    fprintf('...done runMaxwell\n'); 
  end;
  
end

% --- Utility functions ---

% ---------------  Read and Extract REFLECTION/TRANSMISSON Probe data from a CgMx probe file -----
% Parameters:
%   fileName (input) : name of the reflection/transmisson probe file
%   referenceFile (input) : (optional) name of the "reference file data" to be subtracted
%                           to get reflected field from total field
%  infoLevel (input) : > 0 : output extra info 
% 
%  t (output) : array of timne values 
%  Rr,Ri (output) : eral and imaginary parts of the reflection coefficient
%  Tr,Ti (output) : eral and imaginary parts of the transmission coefficient
% --------------------------------------------------------------------------------------
function [ t, Rr, Ri, Tr, Ti ] = getCgMxProbeReflectionTransmissionData( fileName, infoLevel  )


%  Read data:

% reflecton data: 
reflectionFileName=' ';
reflectionFileName=sprintf('reflection%s',fileName);
if( infoLevel>0 )
  fprintf('ReflectionTransmissionProbe: Read file=[%s]\n',reflectionFileName);
end;
[headers,labels,t,qr] = readProbeFile(reflectionFileName,infoLevel);

% transmission data: 
transmissionFileName=sprintf('transmission%s',fileName);
if( infoLevel>0 )
  fprintf('ReflectionTransmissionProbe: Read file=[%s]\n',transmissionFileName);
end; 
[headers,labels,t,qt] = readProbeFile(transmissionFileName,infoLevel);

[numHeaders,headerLen] = size(headers);
if( infoLevel>0 )
  fprintf('Header comments:\n');
  for i=1:numHeaders
    fprintf('%s\n',headers(i,:));  
  end;
end;

% labels

[numColumns,columnLen] = size(labels);
numVars=numColumns-1;   % t is not counted

if( infoLevel>0 )
  fprintf(1,'ReflectionTransmissionProbe: There are %d solution variables in the data.\n',numVars);
end;

cStart=4; % first component 

numToPlot=numVars-j+1;

cr=cStart;
ci=cStart+1;

Rr = qr(:,cr);
Ri = qr(:,ci);

Tr = qt(:,cr);
Ti = qt(:,ci);

% Rnorm = sqrt( qr(:,cr).^2 + qr(:,ci).^2 );
% Tnorm = sqrt( qt(:,cr).^2 + qt(:,ci).^2 );
% rtNorm = Rnorm.^2 + Tnorm.^2;

end





% ---------------  Read and Extract Probe data from a CgMx probe file -----
% Parameters:
%   fileName (input) : name of the probe file
%   referenceFile (input) : (optional) name of the "reference file data" to be subtracted
%                           to get reflected field from total field
%  infoLevel (input) : > 0 : output extra info 
% 
%  t (output) : array of timne values 
%  Ex, Ey, Hz (output) : time sequence probe data
% --------------------------------------------------------------------------------------
function [ t, Ex, Ey, Hz ] = getCgMxProbeData( fileName, referenceFile, infoLevel  )

  
  %  Read data: 
  [headers,labels,t,q] = readProbeFile(fileName,infoLevel);
  
  [numHeaders,headerLen] = size(headers);
  fprintf('Header comments:\n');
  if( infoLevel >0 )
    for i=1:numHeaders
      fprintf('%s\n',headers(i,:));
    end;
  end;
  
  % labels
  
  if( referenceFile ~= 0 )
    fprintf('Reading the reference file=[%s]\n',referenceFile);
    [headersRef,labelsRef,tRef,qRef] = readProbeFile(referenceFile);
  
    tDiff = max(abs(t-tRef));
    fprintf('Checking consistency of reference: |t-tRef| = %9.2e\n',tDiff);
  
    % Subtract off the reference solution (but not from x,y,z)
    q(:,4:end) = q(:,4:end)- qRef(:,4:end);
    
  end
  
  
  
  [numColumns,columnLen] = size(labels);
  numVars=numColumns-1;   % t is not counted
  fprintf(1,'There are %d solution variables in the data.\n',numVars);
  
  x = q(:,1);
  y = q(:,2);
  z = q(:,3);
  
  xMin = min(x);  xMax = max(x);
  yMin = min(y);  yMax = max(y);
  zMin = min(z);  zMax = max(z);
  
  fixedPosition=0;
  if xMin==xMax && yMin==yMax && zMin==zMax 
    fixedPosition=1;
    fprintf(1,' Probe is located at the fixed position (x,y,z)=(%9.3e,%9.3e,%9.3e)\n',xMin,yMin,zMin);
  end;
  
  j=1;
  if fixedPosition==1 
    j=j+3;  % do not plot (x,y,z) in this case 
  end;
  cStart=j; % first component 
  
  numToPlot=numVars-j+1;
  
  exc=cStart+0;
  eyc=cStart+1; 
  hzc=cStart+2; 
  
  Ex = q(:,exc); 
  Ey = q(:,eyc); 
  Hz = q(:,hzc); 


end
