  *  create mappings 
  read iges file 
    /usr/casc/overture/Overture/sampleMappings/pinvertedcoupler2.igs 
    continue 
    choose some 
    0 -1 
    mappingName 
    slac2 
    CSUP:determine topology 
    *      deltaS 2 
    *  make the triangulation a bit finer to make it easier to grow a grid around a corner. 
    deltaS 1. 
*
    improve triangulation 1
*
    build edge curves 
    merge edge curves 
    triangulate 
* pause
    exit 
    exit 
  * 
  builder 
    build a box grid 
      x bounds: -9.16e+01, 2.96e-04 
      y bounds: -9.01e+01, 0. 
      z bounds: 9, 5.47e+01 
      lines: 45 45 30 
      box details... 
        boundary conditions 
        0 2 0 3 0 1 
        share 
        0 2 0 3 0 1 
        exit 
      exit 
    * 
    build a box grid 
      x bounds: -9.60e+01, -3.0e+01 
      y bounds: -1.8e+01, 0. 
      z bounds: 0, 1.5e+01 
      lines: 33 9 8 
      box details... 
        boundary conditions 
        0 0 0 3 4 0 
        share 
        0 0 0 3 4 0 
        exit 
      exit 
    * 
    build a box grid 
      x bounds: -1.8e+01, 0. 
      y bounds: -9.60e+01, -3.0e+01 
      z bounds: 0., 1.5e+01 
      lines:  9 33  8 
      box details... 
        boundary conditions 
        0 2 0 0 5 0 
        share 
        0 2 0 0 5 0 
        exit 
      exit 
    * 
    * inner core 
    * 
    build a box grid 
      x bounds: -6.e+00, 0. 
      y bounds: -6.e+00, 0. 
      z bounds: 0., 5.47e+01 
      lines: 10, 10, 31 
      box details... 
        boundary conditions 
        0 2 0 3 7 1 
        share 
        0 2 0 3 7 1 
        exit 
      exit 
    * 
    * neck grid 
    * 
    create surface grid... 
      picking:create boundary curve 
      choose edge curve 22 2.960852e-04 -8.325296e+00 1.757500e+01 
      choose edge curve 23 2.960852e-04 -1.076418e+01 3.710078e+01 
      choose edge curve 27 2.960852e-04 -1.582853e+01 2.207522e+01 
      done 
      choose edge curve 20 -8.325000e+00 0.000000e+00 1.757500e+01 
      choose edge curve 25 -9.886119e+00 -2.421398e-15 3.710078e+01 
      choose edge curve 29 -1.560928e+01 -3.823163e-15 2.289239e+01 
      done 
      picking:choose initial curve 
      choose edge curve 16 -5.570087e+00 -6.187276e+00 -1.626686e-15 
      done 
      * backward 
      project points onto reference surface 1 
      distance to march 47 46 47 38 
      lines to march 31 27 31  21 
      points on initial curve 31 
      * 
      Boundary Condition: bottom  1 
      Boundary Condition: top     0 
      adjust for corners when marching 0 
      *  stretch near the corner 
      spacing: stretch Mapping 
        reset:0 
        layers 
        1 
        .35 20. .775 
        exit 
      * 
      generate 
      plot reference surface 0 
      set view:0 -0.655151 -0.582598 0 5.73438 1 0 0 0 0.939693 -0.34202 0 0.34202 0.939693 
      y+r:0 
      pause 
*      continue
      exit 
    * 
    * neck volume 
    * 
    create volume grid... 
      forward 
      marching options... 
      BC: right fix y, float x and z 
      BC: left fix x, float y and z 
      distance to march 4. 3.5  3. 
      lines to march 9. 7 
      uniform dissipation .01 
      volume smooths 4 
      * 
      Boundary Condition: left    2 
      Share Value: left    2 
      Boundary Condition: right   3 
      Share Value: right   3 
      Boundary Condition: bottom  7 
      Share Value: bottom  7 
      Boundary Condition: back 6 
      Share Value: back 6 
      * 
      generate 
      pause 
      * continue
      exit 
    * 
    * core surface grid 
    * 
    create surface grid... 
      picking:create boundary curve 
      choose edge curve 23 2.960852e-04 -1.076418e+01 3.710078e+01 
      choose edge curve 27 2.960852e-04 -1.582853e+01 2.207522e+01 
      choose edge curve 34 2.960852e-04 -2.143492e+01 6.343842e+00 
      done 
      choose edge curve 25 -9.886119e+00 -2.421398e-15 3.710078e+01 
      choose edge curve 29 -1.560928e+01 -3.823163e-15 2.289239e+01 
      choose edge curve 32 -2.143463e+01 -5.249838e-15 6.343842e+00 
      done 
      picking:choose initial curve 
      choose edge curve 26 -9.021743e+00 -8.394561e+00 3.515000e+01 
      done 
      backward 
      project points onto reference surface 1 
      distance to march 37 46 44  42 
      lines to march 25 31 
      points on initial curve 25 
      generate 
      pause 
      * continue
      exit 
    * 
    * core volume 
    * 
    create volume grid... 
      marching options... 
      BC: right fix x, float y and z 
      BC: left fix y, float x and z 
      BC: bottom fix z, float x and y 
      distance to march 6. 5.  4 
      lines to march 7 
      backward 
      * 
      Boundary Condition: left    3 
      Share Value: left    3 
      Boundary Condition: right   2 
      Share Value: right   2 
      Boundary Condition: back 6 
      Share Value: back 6 
      pause 
      * continue
      * 
      generate 
      exit 
    * 
    *  middle surface 
    * 
    create surface grid... 
      picking:create boundary curve 
      choose edge curve 1 -1.341403e+01 -3.516532e+01 4.999944e+00 
      choose edge curve 7 -1.995676e+01 -4.868542e+01 5.003790e+00 
      choose edge curve 8 -2.000004e+01 -6.550215e+01 7.817688e+00 
      choose edge curve 9 -1.460103e+01 -9.367065e+01 2.948796e+01 
      done 
      choose edge curve 3 -3.516547e+01 -1.341405e+01 4.999944e+00 
      choose edge curve 15 -4.933883e+01 -1.998908e+01 5.015305e+00 
      choose edge curve 14 -6.619641e+01 -2.000000e+01 8.056715e+00 
      choose edge curve 13 -9.391014e+01 -1.437037e+01 2.983435e+01 
      done 
      forward and backward 
      picking:choose initial curve 
      choose edge curve 2 -3.650256e+01 -3.703438e+01 4.999955e+00 
      done 
      points on initial curve 35  25 
      distance to march 44 30 (forward,backward) 
      lines to march 22 15 (forward,backward) 
      project points onto reference surface 1 
      *      all boundaries are interpolation -- this will 
      *      make the last line generated the ghost point. 
      Boundary Condition: left    0 
      Boundary Condition: right   0 
      Boundary Condition: bottom  0 
      Boundary Condition: top     0 
      generate 
      pause 
      * continue
      exit 
    * 
    *   middle volume 
    * 
    create volume grid... 
      distance to march 20 
      * 
      Boundary Condition: back 6 
      Share Value: back 6 
      * 
      generate 
      pause 
      * continue
      exit 
    * 
    * first coupler surface 
    * 
    create surface grid... 
      picking:create boundary curve 
      choose edge curve 53 -3.000000e+01 0.000000e+00 1.666646e+00 
      choose edge curve 4 -2.846368e+01 3.485716e-15 4.999937e+00 
      choose edge curve 32 -2.143463e+01 -5.249838e-15 6.343842e+00 
      done 
      choose edge curve 51 -1.000000e+02 0.000000e+00 2.097884e+01 
      choose edge curve 12 -1.013133e+02 -2.655856e-14 4.888047e+01 
      done 
      * 
      picking:choose initial curve 
      choose edge curve 70 -3.616144e+01 -1.443933e+01 -1.658183e-14 
      choose edge curve 65 -6.500002e+01 -2.000000e+01 -2.775558e-14 
      choose edge curve 74 -9.414214e+01 -1.414214e+01 -3.290186e-14 
      done 
      * 
      backward 
      *      * pause 
      *      picking:hide sub-surface 
      *      hide surface 9 
      *        exit 
      Boundary Condition: bottom  1 
      * 
      *      stop on negative cells 0 
      distance to march 19. 
      *       initially build a finer grid to get around the corner 
      lines to march 23 51 23  19 
      points on initial curve 51 
      generate 
      *       reduce lines to 23 
      *      lines 
      *        51 23 
      pause 
      * continue
      exit 
    * 
    * first coupler volume 
    * 
    create volume grid... 
      lines to march 3 
      backward 
      generate 
      BC: bottom fix z, float x and y 
      BC: left fix y, float x and z 
      BC: right fix y, float x and z 
      distance to march 10 
      lines to march 9 
      * 
      Boundary Condition: left    3 
      Share Value: left    3 
      Boundary Condition: right   3 
      Share Value: right   3 
      Boundary Condition: bottom  4 
      Share Value: bottom  4 
      Boundary Condition: back 6 
      Share Value: back 6 
      * 
      generate 
      pause 
      * continue
      exit 
    * 
    * make second coupler surface 
    * 
    create surface grid... 
      picking:create boundary curve 
      choose edge curve 48 2.960852e-04 -1.000021e+02 2.098227e+01 
      choose edge curve 10 2.960852e-04 -1.011896e+02 4.790384e+01 
      done 
      choose edge curve 38 2.960852e-04 -2.999970e+01 1.666646e+00 
      choose edge curve 0 2.960852e-04 -2.692746e+01 4.999936e+00 
      choose edge curve 34 2.960852e-04 -2.143492e+01 6.343842e+00 
      done 
      picking:choose initial curve 
      choose edge curve 90 -1.404977e+01 -9.423657e+01 -1.205690e-14 
      choose edge curve 80 -2.000004e+01 -6.565364e+01 -1.387779e-14 
      choose edge curve 85 -1.383770e+01 -3.556130e+01 -1.199202e-14 
      done 
      backward 
      stop on negative cells 0 
      distance to march 19 
      lines to march 23 19 
      points on initial curve 51 
      Boundary Condition: bottom  1 
      generate 
      pause 
      * continue
      exit 
    * 
    * 2nd coupler volume 
    * 
    create volume grid... 
      Start curve:slac1-surface2 
      distance to march 10 
      lines to march 9 
      backward 
      marching options... 
      BC: bottom fix z, float x and y 
      BC: left fix x, float y and z 
      BC: right fix x, float y and z 
      * 
      Boundary Condition: left    2 
      Share Value: left    2 
      Boundary Condition: right   2 
      Share Value: right   2 
      Boundary Condition: bottom  5 
      Share Value: bottom  5 
      Boundary Condition: back 6 
      Share Value: back 6 
      * 
      generate 
      * pause 
      exit 
    * 
    * 
    *  build curved surface 
    * 
    create surface grid... 
      picking:create boundary curve 
      choose edge curve 10 2.960852e-04 -1.011896e+02 4.790384e+01 
      choose edge curve 48 2.960852e-04 -1.000021e+02 2.098227e+01 
      done 
      choose edge curve 12 -1.013133e+02 -2.655856e-14 4.888047e+01 
      choose edge curve 51 -1.000000e+02 0.000000e+00 2.097884e+01 
      done 
      picking:choose initial curve 
      choose edge curve 11 -7.187711e+01 -7.187740e+01 5.465000e+01 
      done 
      backward 
      project points onto reference surface 1 
      lines to march 21 
      distance to march 42 
      points on initial curve 51 
      Boundary Condition: bottom  1 
      generate 
      pause 
      * continue
      exit 
    * 
    * big curved volume grid 
    * 
    create volume grid... 
      backward 
      distance to march 15 
      marching options... 
      BC: left fix x, float y and z 
      BC: right fix y, float x and z 
      BC: bottom fix z, float x and y 
      * 
      Boundary Condition: left    2 
      Share Value: left    2 
      Boundary Condition: right   3 
      Share Value: right   3 
      Boundary Condition: bottom  1 
      Share Value: bottom  1 
      Boundary Condition: back 6 
      Share Value: back 6 
      * 
      generate 
      *  pause 
      exit 
    * 
    * 
    * extra refinement grid 
    * 
    build a box grid 
      x bounds: -20., 0.   -13., 0. 
      y bounds: -20., 0. -13., 0. 
      z bounds: 20, 5.47e+01  35, 5.47e+01 
      lines: 25 25 25   15, 15, 15 
      box details... 
        boundary conditions 
        0 2 0 3 0 1 
        share 
        0 2 0 3 0 1 
        exit 
      exit 
    * 
    * 
    save grids to a file... 
    file name: slac2Grids.hdf 
    save file 
    exit 
    exit 
  exit 
