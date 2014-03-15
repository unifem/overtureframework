  * front wheels 
  * 
  read iges file 
    /home/chand/iges/truck/STD_TRUCK_ASSY.igs 
    continue 
    choose a list 
      230 231 232 233 234 235 236 237 268 269 
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
    * 
    * 
    * **************** left front wheel ****************************** 
    * 
    create surface grid... 
      choose edge curve 12 6.652768e+00 -1.716155e+00 5.456003e+00 
      done 
      Start curve parameter bounds .13 .87  .15 .85 
      forward and backward 
      marching spacing... 
      points on initial curve 41 
      *     wheel is about 1.24 units wide 
      target grid spacing .3 .102 
      lines to march 8 20 7 19 (forward,backward) 
      generate 
      name frontLeftWheelSurface 
      pause 
      GST:pick to stretch 
        GST:pick to stretch in direction 2 
        GST:stretch r2 0 0.5 10 0.24 (id,weight,exponent,position) 
        GST:stretch r2 1 0.5 10 0.72 (id,weight,exponent,position) 
        exit 
      GST:stretch grid 
      pause 
      exit 
    * 
    create volume grid... 
      spacing: geometric 
      target grid spacing .06 
      lines to march 13 12 11 
      generate 
      name frontLeftWheel 
      pause 
      exit 
    * 
    create surface grid... 
      choose edge curve 4 6.591462e+00 -2.263083e-01 5.457075e+00 
      choose edge curve 11 4.364778e+00 -2.455501e-01 5.436581e+00 
      choose edge curve 15 4.365129e+00 -2.463658e-01 4.839182e+00 
      choose edge curve 31 6.693723e+00 -2.278638e-01 4.241512e+00 
      choose edge curve 17 8.921127e+00 -2.100171e-01 4.820538e+00 
      choose edge curve 5 8.921286e+00 -2.096229e-01 5.431183e+00 
      done 
      Pick rotation point:0 
        4.61713 -0.467698 4.23422 
      set view:0 0.470176 -0.163717 0 3.90202 0.984808 0.0593912 -0.163176 -2.12651e-17 -0.939693 -0.34202 -0.173648 0.336824 -0.925417 
      y+r:0 
      backward 
      picking:choose interior matching curve 
      choose edge curve 16 6.655075e+00 -1.716140e+00 4.241112e+00 
      done 
      1 
      7 
      choose edge curve 16 6.655075e+00 -1.716140e+00 4.241112e+00 
      done 
      0 
      x+r:0 
      x+r:0 
      7 
      choose edge curve 12 6.652768e+00 -1.716155e+00 5.456003e+00 
      done 
      0 
      7 
      choose edge curve 12 6.652768e+00 -1.716155e+00 5.456003e+00 
      done 
      1 
      7 
      * pause 
      equidistribution 1 (in [0,1]) 
      volume smooths 0 
      uniform dissipation 0.2 
      points on initial curve 92 
      distance to march 0.9 
      lines to march 27 
      boundary offset 0 0 0 1 0 0 
      generate 
      GSM:BC: top smoothed 
      GSM:number of iterations 10 
      GSM:smooth grid 
      GST:stretch r1 0 0.25 20 0.0 (id,weight,exponent,position) 
      GST:stretch r1 1 0.25 20 .104 (id,weight,exponent,position) 
      GST:stretch r1 2 0.25 20 0.5 (id,weight,exponent,position) 
      GST:stretch r1 3 0.25 20 0.603 (id,weight,exponent,position) 
      *      GST:stretch r2 1 1 5 0 (id,weight,exponent,position) 
      GST:stretch grid 
      lines 
      92 11 
      name frontLeftWheelJoinSurface 
      exit 
    target grid spacing .3 .03 
    * 
    create volume grid... 
      backward 
      *     BC: top free floating 
      outward splay .1 .1 .1 .05 
      BC: bottom match to a mapping 
      truck1 
      target grid spacing -1, 0.1 (tang,normal, <0 : use default) 
      lines to march 12 
      normal blending 3 3 15 3 
      uniform dissipation 0.2 
      volume smooths 40 
      generate 
      GSM:BC: front smoothed 
      GSM:BC: top smoothed 
      GSM:number of iterations 20 
      GSM:smooth grid 
      GST:stretch r3 2 .5 10. 0. (id,weight,exponent,position) 
      GST:stretch grid 
      name frontLeftWheelJoin 
      pause 
      exit 
    assign BC and share values 
      set BC and share 1 0 1 1 1 
      boundary condition: 10 
      shared boundary flag: 10 
      set BC and share 1 0 2 10 10 
      set BC and share 0 0 2 10 10 
      pause 
      exit 
    * 
    save grids to a file... 
    file name: frontLeftWheel.hdf 
    save file 
    exit 
    exit 
  exit 
