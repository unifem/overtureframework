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
    create surface grid... 
      choose edge curve 39 6.688847e+00 -1.726413e+00 -5.462293e+00 
      done 
      surface grid options... 
      Start curve parameter bounds .13 .87 
      forward and backward 
      points on initial curve 41 
      target grid spacing -1, .102 (tang,normal, <0 : use default) 
      lines to march 8 20 (forward,backward) 
      generate 
      name frontRightWheelSurface 
      GST:stretch r2 0 0.5 10 0.24 (id,weight,exponent,position) 
      set view:0 0.28173 0.121778 0 13.8599 0.301134 0.0398856 -0.952747 0.173179 0.980223 0.0957723 0.937725 -0.193836 0.288271 
      GST:stretch r2 1 0.5 10 0.72 (id,weight,exponent,position) 
      GST:stretch grid 
      *  pause 
      exit 
    * 
    create volume grid... 
      spacing: geometric 
      target grid spacing .06 
      lines to march 12 11 
      generate 
      name frontRightWheel 
      pause 
      exit 
    * 
    create surface grid... 
      choose edge curve 22 8.920199e+00 -2.122245e-01 -4.824351e+00 
      choose edge curve 38 8.920214e+00 -2.121750e-01 -5.428710e+00 
      choose edge curve 21 6.592132e+00 -2.297470e-01 -4.242349e+00 
      choose edge curve 20 4.365240e+00 -2.466528e-01 -4.832491e+00 
      choose edge curve 40 4.363637e+00 -2.429893e-01 -5.467756e+00 
      choose edge curve 47 6.691912e+00 -2.249962e-01 -5.474531e+00 
      done 
      backward 
      picking:choose interior matching curve 
      choose edge curve 36 6.654088e+00 -1.726922e+00 -4.235143e+00 
      done 
      0 
      7 
      choose edge curve 36 6.654088e+00 -1.726922e+00 -4.235143e+00 
      done 
      1 
      7 
      choose edge curve 39 6.688847e+00 -1.726413e+00 -5.462293e+00 
      done 
      0 
      7 
      choose edge curve 39 6.688847e+00 -1.726413e+00 -5.462293e+00 
      done 
      1 
      7 
      * 
      points on initial curve 92 
      equidistribution 1 (in [0,1]) 
      volume smooths 0 
      uniform dissipation 0.2 
      boundary offset 0, 0, 0, 1 (l r b t) 
      distance to march 0.9 
      lines to march 27 
      generate 
      * pause 
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
      name frontRightWheelJoinSurface 
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
      * pause 
      GSM:BC: front smoothed 
      GSM:BC: top smoothed 
      GSM:number of iterations 20 
      GSM:smooth grid 
      GST:stretch r3 2 .5 10. 0. (id,weight,exponent,position) 
      GST:stretch grid 
      name frontRightWheelJoin 
      exit 
    * 
    assign BC and share values 
      set BC and share 1 0 1 1 1 
      set BC and share 1 0 2 11 11 
      set BC and share 0 0 2 11 11 
      exit 
    save grids to a file... 
    file name: frontRightWheel.hdf 
    save file 
    exit 
    exit 
  exit 
