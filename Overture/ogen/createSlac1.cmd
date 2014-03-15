*
* Create component grids for the SLAC geometry
*
*  This file is included in other files (slac1.cmd, slac1.order4.cmd)
*   This script uses:  
*  getGridPoints : A perl function to compute the number of grid points
*  $numGhost : number of ghost points, 1 for 2nd order, 2= fourth order
*
create mappings
*
  read iges file 
    /home/henshaw/Overture/mapping/LANLAOT5.igs 
    continue 
    choose some 
    0 -1 
    mappingName 
    slac1 
pause
    CSUP:determine topology 
    deltaS 2 
    build edge curves 
pause
    merge edge curves 
pause
    triangulate 
pause
    exit 
    exit 
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
      choose edge curve 9 -1.082827e-14 -2.117347e+01 2.787662e+01 
      choose edge curve 14 -1.223744e-14 -2.884488e+01 4.052989e+01 
      done
      choose edge curve 4 8.325000e+00 0.000000e+00 3.648409e+01 
      choose edge curve 7 1.354208e+01 -3.316744e-15 1.955315e+01 
      choose edge curve 10 2.064701e+01 -5.056892e-15 2.591185e+01 
      choose edge curve 13 2.884488e+01 -7.064726e-15 4.052989e+01 
      done
      picking:choose initial curve
      choose edge curve 0 5.190755e+00 -6.508705e+00 4.966500e+01 
      done
      backward
pause
      getGridPoints(21,21,21);
      points on initial curve $nx 
**      points on initial curve 21  16
      distance to march 60 * 55.
      getGridPoints(60,60,60);
      lines to march $nx 
*      lines to march 60  * 55  
      boundary offset 0, 0, 0, $numGhost (l r b t)
*
      uniform dissipation 0.01
*
      generate
      GSM:smooth grid
*
      fourth order
*
       pause
      exit
    * 
    create volume grid... 
      marching options... 
      BC: left fix x, float y and z 
      BC: right fix y, float x and z 
      BC: bottom fix z, float x and y 
      distance to march 3. 2.5 
      getGridPoints(6,6,6);
      lines to march $nx
**    lines to march 6  7 
      generate 
      GSM:smooth grid
      boundary conditions 
      1 2 3 0 4 0 
      share 
      1 2 3 0 4 0 
*
      fourth order
*
      pause 
    exit 
    ***********************************************************************
    *   big grid, surface 
    ***********************************************************************
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
      getGridPoints(75,1,1);
      lines to march  $nx
**      lines to march  75 53  27
      getGridPoints(61,1,1);
      points on initial curve $nx
**      points on initial curve  61  31
*  ---- are these still needed?  -- yes --- this should be automatic ??
      marching options...
       boundary offset 0 0 0 $numGhost (l r b t)
      close marching options
pause
      generate
      GSM:smooth grid
*
      fourth order
*
pause
      exit
    * 
    *  big grid, volume 
    * 
    create volume grid... 
      Start curve:slac1-surface2 
      BC: left fix x, float y and z 
      BC: right fix y, float x and z 
      BC: bottom fix z, float x and y 
      backward 
      getGridPoints(6,1,1);
      lines to march $nx
**      lines to march 6  7 
*      points on initial curve 21 35 
      distance to march 3.5  4.  5.  7
      generate 
      GSM:smooth grid
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
*
      fourth order
*
   pause 
  exit 
  exit 
*********************************************************
  * 
  Box 
    mappingName 
    mainBox 
    set corners 
     0 100 -100 0 0 45  * 0 90  -90 0 0 35 
    lines 
    getGridPoints(91,91,41);
    $nx $ny $nz
*    75 75 33  45 45 17 
    boundary conditions 
    1 0 0 2 5 0 
    share 
    1 0 0 2 5 0 
    exit 
  * 
  Box 
    mappingName 
    coreBox 
    set corners 
      0 7 -7 0 0 49.665   [49.6,49.7] 
    lines 
    getGridPoints(8,8,49);
    $nx $ny $nz
*       8 8 49  * 9 9 29 
    boundary conditions 
      1 0 0 2 5 3 
    share 
      1 0 0 2 5 3 
    exit 
  * 
  exit this menu
