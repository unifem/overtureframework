  * 
  read iges file 
    /home/chand/iges/truck/STD_TRUCK_ASSY.igs 
    continue 
    choose a list 
      179 180 181 182 183 184 185 242 243 281 282 283 372 373 374 375 376 377 378 379 
      380 381 382 383 
      done 
    mappingName 
    truck1 
    * 
    CSUP:determine topology 
    merge tolerance 0.01 
    deltaS 0.1 
    build edge curves 
    merge edge curves 
    triangulate 
    exit 
    exit 
  * 
  * 
  builder 
    * 
    target grid spacing .3 .1 
    * 
    create surface grid... 
      choose edge curve 110 3.720013e+01 -1.481526e+00 5.270526e+00 
      done 
      surface grid options... 
      Start curve parameter bounds .15 .85 
      forward and backward 
      target grid spacing .2 .125 (tang,normal, <0 : use default) 
      lines to march 7, 33 (forward,backward) 
      generate 
      * 
      GST:stretch r2 0 0.5 10 0.16 (id,weight,exponent,position) 
      GST:stretch r2 1 0.5 10 0.84 (id,weight,exponent,position) 
      GST:stretch grid 
      name tenderLeftRear2WheelSurface 
      * pause 
      exit 
    * 
    create volume grid... 
      spacing: geometric 
      backward 
      generate 
      target grid spacing -1, 0.1 (tang,normal, <0 : use default) 
      lines to march 7 
      generate 
      name tenderLeftRear2Wheel 
      * pause 
      exit 
    * 
    * 
    create surface grid... 
      choose edge curve 44 3.719024e+01 1.616291e-02 5.258584e+00 
      choose edge curve 45 3.943239e+01 3.158933e-02 3.649490e+00 
      choose edge curve 46 3.719013e+01 1.155115e-02 2.098527e+00 
      choose edge curve 47 3.494798e+01 -3.880956e-03 3.707621e+00 
      done 
pause
      target grid spacing 0.3, 0.1 (tang,normal, <0 : use default) 
      picking:choose interior matching curve 
      choose edge curve 110 3.720013e+01 -1.481526e+00 5.270526e+00 
      done 
      0 
      7 
      choose edge curve 110 3.720013e+01 -1.481526e+00 5.270526e+00 
      done 
      1 
      7 
      choose edge curve 112 3.719981e+01 -1.481529e+00 2.110482e+00 
      done 
      0 
      7 
      choose edge curve 112 3.719981e+01 -1.481529e+00 2.110482e+00 
      done 
      1 
      7 
      marching options... 
      uniform dissipation 0.2 
      volume smooths 0 
      equidistribution 1 (in [0,1]) 
      boundary offset 0, 0, 0, 1 (l r b t) 
      lines to march 13 
      points on initial curve 51 
      generate 
      smoothing... 
      GSM:BC: top smoothed 
      GSM:number of iterations 20 
      GSM:smooth grid 
      * 
      GST:stretch r1 0 0.25 20 0.0 (id,weight,exponent,position) 
      GST:stretch r1 1 0.25 20 .2   (id,weight,exponent,position) 
      GST:stretch r1 2 0.25 20 0.5 (id,weight,exponent,position) 
      GST:stretch r1 3 0.25 20 0.7   (id,weight,exponent,position) 
      GST:stretch grid 
      name tenderLeftRear2WheelSurfaceJoin 
      pause 
      exit 
    * 
    * 
    create volume grid... 
      target grid spacing -1, 0.1 (tang,normal, <0 : use default) 
      normal blending 3, 3, 15, 3 (lines, left,right,bottom,top) 
      BC: top free floating 
      *      outward splay 0.1, 0.1, 0.1, 0.05 (left,right,bottom,top for outward splay BC) 
      BC: bottom match to a mapping 
      truck1 
      volume smooths 40 
      backward 
      lines to march 11 
      generate 
      pause 
      GSM:number of iterations 10 
      GSM:BC: top smoothed 
      GSM:BC: front smoothed 
      GSM:smooth grid 
      * 
      GST:stretch r3 2 .5 10. 0 (id,weight,exponent,position) 
      GST:stretch grid 
      name tenderLeftRear2WheelJoin 
      pause 
      exit 
    assign BC and share values 
      set BC and share 1 0 1 1 1 
      set BC and share 0 0 2 13 13 
      set BC and share 1 0 2 13 13 
      exit 
    * 
    save grids to a file... 
    file name: tenderRearWheels.hdf 
    save file 
    exit 
    exit 
  exit 
