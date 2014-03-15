  read iges file 
    /home/henshaw/iges/STD_TRUCK_ASSY.igs 
    continue 
    choose a list 
      99 116 117 118 119 120 164 165 166 167 168 172 173 174 276 277 278 291 292 293 
      294 296 297 300 352 
      done 
    CSUP:determine topology 
    merge tolerance 0.01 
    deltaS 0.1 
    build edge curves 
    merge edge curves 
    triangulate 
    exit 
    CSUP:mappingName corner 
    exit 
  * 
  builder 
    target grid spacing .3 .03 (tang,norm)((<0 : use default) 
    * 
    build curve on surface 
      plane point 1 2.175197e+01,5.5,6.5 
      plane point 2 25,5.5,6.5 
      plane point 3 22.5,5.5,3. 
      cut with plane 
      exit 
    create surface grid... 
      choose boundary curve 18 2.435692e+01 5.500000e+00 5.715941e+00 
      done 
      equidistribution 0.5 (in [0,1]) 
      volume smooths 0 
      stop on negative cells 0 
      target grid spacing .3 .15 
      points on initial curve 32 
      lines to march 22 
      SC:stretch r1 0 1 5 0.5 (id,weight,exponent,position) 
      stretch start curve 
      generate 
      smoothing... 
      GSM:BC: right smoothed 
      GSM:project smoothed grid onto reference surface 0 
      GSM:number of iterations 1 
      GSM:smooth grid 
      GSM:smoothing offset 5 5 5 5 (l r b t b f) 
      GSM:smooth grid 
      GSM:smooth grid 
      GSM:smooth grid 
      GSM:smooth grid 
      GSM:smooth grid 
      GSM:smooth grid 
      GSM:smooth grid 
      GSM:smooth grid 
      name leftCabCornerSurface 
      pause 
      exit 
    * 
    create volume grid... 
      lines to march 23 
      uniform dissipation 0.15
      generate 
      name leftCabCorner 
      pause 
      exit 
    * 
    save grids to a file... 
    file name: leftCabCorner.hdf 
    save file 
    exit 
    exit 
  exit 
