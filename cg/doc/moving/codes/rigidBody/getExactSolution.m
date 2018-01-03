function [ xv, vv, omegav ] = getExactSolution( deriv, t )
%
% Arguments:
%   deriv = 0, 1, 2 compute this time derivative of the exact solution
%

globalDeclarations;  % declare global variables here 

if itest==twilightZoneSolution

  if twilightZone==polynomialTwilightZone
  
    fprintf('getExactSolution:ERROR: polynomialTwilightZone -- finish me!\n');
    pause;
  
  elseif twilightZone==trigonometricTwilightZone
  
    if deriv==0  
      xv = [ amp(1,1)/freq(1,1)*sin( freq(1,1)*( t-tOffset(1,1) )); ...
             amp(2,1)/freq(2,1)*sin( freq(2,1)*( t-tOffset(2,1) )); ...
             amp(3,1)/freq(3,1)*sin( freq(3,1)*( t-tOffset(3,1) )) ];
    
      vv = [ amp(1,1)*cos( freq(1,1)*( t-tOffset(1,1) )); ...
             amp(2,1)*cos( freq(2,1)*( t-tOffset(2,1) )); ...
             amp(3,1)*cos( freq(3,1)*( t-tOffset(3,1) )) ];
    
      omegav = [ amp(1,2)*cos( freq(1,2)*( t-tOffset(1,2) )); ...
                 amp(2,2)*cos( freq(2,2)*( t-tOffset(2,2) )); ...
                 amp(3,2)*cos( freq(3,2)*( t-tOffset(3,2) )) ];
  
    elseif deriv==1 
  
      xv = [ amp(1,1)*cos( freq(1,1)*( t-tOffset(1,1) )); ...
             amp(2,1)*cos( freq(2,1)*( t-tOffset(2,1) )); ...
             amp(3,1)*cos( freq(3,1)*( t-tOffset(3,1) )) ];

      vv = -[ amp(1,1)*freq(1,1)*sin( freq(1,1)*( t-tOffset(1,1) )); ...
              amp(2,1)*freq(2,1)*sin( freq(2,1)*( t-tOffset(2,1) )); ...
              amp(3,1)*freq(3,1)*sin( freq(3,1)*( t-tOffset(3,1) )) ];
    
      omegav = -[ amp(1,2)*freq(1,2)*sin( freq(1,2)*( t-tOffset(1,2) )); ...
                  amp(2,2)*freq(2,2)*sin( freq(2,2)*( t-tOffset(2,2) )); ...
                  amp(3,2)*freq(3,2)*sin( freq(3,2)*( t-tOffset(3,2) )) ];
    else
      fprintf('getExactSolution:ERROR:trigonometricTwilightZone: deriv=%d\n',deriv);
      pause;
    end;
    
  else 
  
    fprintf('getExactSolution:ERROR: unknown option: twilightZone=%d\n',twilightZone);
    pause;
  
  end;

else

  fprintf('getExactSolution:ERROR: unknown itest=%d\n',itest);
  pause;

end;
