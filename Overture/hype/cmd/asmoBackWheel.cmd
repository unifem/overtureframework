  * mbuilder command file 
  * 
  *   Build grids for the back wheel of the asmo automobile 
  * 
  * Asmo notes:  car bottom is at z=0 
  *              wheel bottom is a z=-33.558 
  * 
  read iges file 
    asmo.igs 
    continue 
    choose a list 
      4 5 6 7 13 14 15 16 17 18 19 20 
      97 98 107 116 125 134 135 
      124 115 96 106 
      done 
    * 
    *  The trimmed surfaces here do not match very well 
    *    It is a bit tricky to fix up the geometry 
    * 
    x-r:0 180 
    CSUP:determine topology 
    * pause 
    merge tolerance .65 
    deltaS 2. 
    build edge curves 
    split tolerance factor 1.71
    merge edge curves 
pause
    * 
    triangulate 
    exit 
    exit 
  * 
  builder 
    * surface grids near the back wheel 
    set view:0 0.016085 -0.0413849 0 1.17117 0.885446 -0.189178 0.424495 0.301741 -0.460682 -0.8347 0.353464 0.86717 -0.350826 
    pause 
    create surface grid... 
      surface grid options... 
      * 
      *     choose a start curve 
      *      points on initial curve 93 
      edge curve tolerance   0.001 
      choose edge curve -20 6.735582e+02 -1.227705e+02 2.523541e+00 
      choose edge curve -97 6.502912e+02 -1.229584e+02 3.558778e-01 
      choose edge curve -99 6.460684e+02 -1.216126e+02 8.846857e-02 
      choose edge curve -31 6.444722e+02 -1.088584e+02 -5.374061e-02 
      choose edge curve -40 6.461212e+02 -9.828455e+01 3.039209e-01 
      choose edge curve -101 6.509754e+02 -9.700111e+01 5.689136e-01 
      choose edge curve -16 6.745141e+02 -9.701961e+01 2.756941e+00 
      choose edge curve -106 7.137900e+02 -9.712460e+01 8.822817e+00 
      choose edge curve -107 7.344397e+02 -9.969220e+01 1.182997e+01 
      choose edge curve -108 7.350625e+02 -1.106961e+02 1.181201e+01 
      choose edge curve -50 7.348928e+02 -1.179575e+02 1.207633e+01 
      choose edge curve -109 7.331440e+02 -1.208926e+02 1.202596e+01 
      choose edge curve -5 7.119289e+02 -1.222748e+02 8.679488e+00 
      done 
      backward 
      points on initial curve 53 
*      distance to march 45 
      distance to march 32
      lines to march 13 
      * 
      *     set the bc's -1=periodic, 1=physical boundary 0=interpolation 
      boundary conditions 
      -1 -1 1 0 
      generate 
      * rear wheel surface grid 
      pause 
      exit 
    * 
    create volume grid... 
      name backWheelJoin 
      marching options... 
      BC: bottom match to a mapping 
      asmo.igs.compositeSurface 
      uniform dissipation 0.1 
      volume smooths 40 
      distance to march 35 
      lines to march 13 
      normal blending 6 6 6 6 (lines, left,right,bottom,top)
      Boundary Condition: bottom 2
      Share Value: bottom  2 
      Boundary Condition: back  4
      Share Value: back  4
      generate 
      * rear wheel volume grid 
      pause 
      exit 
    * 
    * surface grid near base of wheel 
    * 
    create surface grid... 
      choose boundary curve 1 6.670898e+02 -1.010557e+02 -3.350000e+01 
      done 
      backward 
      distance to march 27 
      lines to march 11 
      points on initial curve 81 
      generate 
      * rear wheel joining surface 
      pause 
      exit 
    * 
    create volume grid... 
      name backWheelBase 
      marching options... 
      BC: bottom fix z, float x and y 
      normal blending 20 20 20 20 (lines, left,right,bottom,top) 
      uniform dissipation 0.05 
      volume smooths 40 
      uniform dissipation 0.1 
      BC: top outward splay 
      outward splay .3 .3 .3 .3 .3 .3 
      volume smooths 60 
      dissipation transition 5 
      boundary dissipation 0.02 
      lines to march 25 
      distance to march 27 
      Boundary Condition: back  4 
      Share Value: back  4 
      Boundary Condition: bottom 3 
      Share Value: bottom 3 
      generate 
      * rear wheel joining volume grid 
      pause 
      exit 
    * 
pause
    save grids to a file... 
    file name: asmoBackWheel.hdf 
    save file 
    exit 
    exit 
  exit 
