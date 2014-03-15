  read iges file 
    /home/henshaw/Overture/mapping/LANLAOT5.igs 
    continue 
    choose some 
    0 -1 
    mappingName 
    slac1 
    CSUP:determine topology 
    deltaS 2 
    build edge curves 
    merge edge curves 
    triangulate 
    exit 
    exit 
*
*
*  
*
  debug
    0
*
  builder 
*
*    Build grid on the boundary of the "core"
*
    create surface grid... 
      set view:0 0.688822 -0.567976 0 4.30864 0.984808 0.0868241 0.150384 -0.0593912 0.98221 -0.178148 -0.163176 0.16651 0.972444 
      picking:create boundary curve 
      choose edge curve 5 -8.468121e-15 -8.325000e+00 3.868091e+01 
      choose edge curve 6 -9.426452e-15 -1.354208e+01 1.955315e+01 
      done 
      choose edge curve 4 8.325000e+00 0.000000e+00 3.648409e+01 
      choose edge curve 7 1.354208e+01 -3.316744e-15 1.955315e+01 
      done 
      picking:choose initial curve 
      choose edge curve 0 5.190755e+00 -6.508705e+00 4.966500e+01 
      done 
      backward 
      distance to march 26 30 
      lines to march 31  35 
      project points onto reference surface 1 
      boundary offset 0, 0, 0, 1 (l r b t) 
      generate 
      pause 
      exit 
    * 
    create volume grid... 
      marching options... 
      BC: left fix x, float y and z 
      BC: right fix y, float x and z 
      BC: bottom fix z, float x and y 
      distance to march 3. 2.5 
      lines to march 7 
      generate 
      boundary conditions 
      1 2 3 0 4 0 
      share 
      1 2 3 0 4 0 
      pause 
      exit 
    * 
    *   big grid, surface 
    * 
    create surface grid... 
      reset:0 
      x+r:0 20 
      y+r:0 20 
      picking:create boundary curve
      choose edge curve 20 -2.309779e-14 -8.796782e+01 3.218457e+01 
      choose edge curve 17 -4.427322e-15 -4.900922e+01 4.466500e+01 
      choose edge curve 14 -1.223744e-14 -2.884488e+01 4.052989e+01 
      done
      choose edge curve 19 8.918457e+01 -2.184320e-14 3.096782e+01 
      choose edge curve 16 4.900922e+01 7.162021e-15 4.466500e+01 
      choose edge curve 13 2.884488e+01 -7.064726e-15 4.052989e+01 
      done
      picking:choose initial curve
      x+r 45
      x-r 90
      choose edge curve 18 7.187684e+01 -7.187684e+01 -3.967049e-14 
      done
      forward
*
      distance to march 102
      lines to march 27
      points on initial curve 31
*  ---- are these still needed?  -- yes --- this should be automatic ??
      marching options...
       boundary offset 0 0 0 1 (l r b t)
      close marching options
      generate
      exit
    * 
    *  big grid, volume 
    * 
    create volume grid... 
      Start curve:slac1-surface2 
      BC: left fix x, float y and z 
      BC: right fix y, float x and z 
      BC: bottom fix z, float x and y 
      lines to march 5 
      backward 
      lines to march 11 
      points on initial curve 21 35 
      distance to march 14. 13.   12 
      generate 
      mapping parameters 
      Boundary Condition: left    1 
      Boundary Condition: right   2 
      Boundary Condition: bottom  5 
      Boundary Condition: back    4 
      Share Value: left    1 
      Share Value: right   2 
      Share Value: bottom  5 
      Share Value: back    4 
      close mapping dialog 
    pause 
      exit 
    * 
    * -- middle section, surface grid 
    * 
    create surface grid... 
      picking:create boundary curve 
      choose edge curve 6 -9.426452e-15 -1.354208e+01 1.955315e+01 
      choose edge curve 9 -1.082827e-14 -2.117347e+01 2.787662e+01 
      choose edge curve 14 -1.223744e-14 -2.884488e+01 4.052989e+01 
      done 
      choose edge curve 7 1.354208e+01 -3.316744e-15 1.955315e+01 
      choose edge curve 10 2.064701e+01 -5.056892e-15 2.591185e+01 
      choose edge curve 13 2.884488e+01 -7.064726e-15 4.052989e+01 
      done 
      picking:choose initial curve 
      choose edge curve 3 5.150980e+00 -6.539307e+00 2.550000e+01 
      done 
      backward 
      lines to march 35 
      points on initial curve 31 
      distance to march 31 
      project points onto reference surface 1 
      generate 
      pause 
      exit 
    * 
    * -- middle section volume grid 
    create volume grid... 
      Start curve:slac1-surface3 
      distance to march 5. 4.5 
      lines to march  9 
      BC: left fix x, float y and z 
      BC: right fix y, float x and z 
      mapping parameters 
      Boundary Condition: left    1 
      Boundary Condition: right   2 
      Boundary Condition: back    4 
      Share Value: left    1 
      Share Value: right   2 
      Share Value: back    4 
      close mapping dialog 
      generate 
      pause 
      exit 
    * 
    pause 
    * 
    exit 
  open a data-base 
  slac1.hdf 
  open a new file 
  put to the data-base 
  slac1-volume1 
  put to the data-base 
  slac1-volume2 
  put to the data-base 
  slac1-volume3 
  close the data-base 
  exit 
