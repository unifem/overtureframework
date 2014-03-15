  open a data-base 
  truckCabNoWheels.hdf 
  open an old file read-only 
  get all mappings from the data-base 
  close the data-base 
  * 
  builder 
    target grid spacing .3 .03 (tang,norm)((<0 : use default) 
    * 
    build curve on surface 
      plane point 1 2.175197e+01,  5.5, -6.5 
      plane point 2 25,            5.5, -6.5 
      plane point 3 22.5,          5.5, -3. 
      cut with plane 
      exit 
    create surface grid... 
      choose boundary curve 0 2.437642e+01 5.500000e+00 -5.734934e+00 
      done 
      forward 
      equidistribution 0.5 (in [0,1]) 
      volume smooths 0 
      stop on negative cells 0 
      target grid spacing .3 .15 
      points on initial curve 32 
      lines to march 22 
      SC:stretch r1 0 1 5 0.5 (id,weight,exponent,position) 
      stretch start curve 
      generate 
      pause 
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
      name rightCabCornerSurface 
      pause 
      exit 
    * 
    create volume grid... 
      backward 
      lines to march 23 
      uniform dissipation 0.15
      generate 
      name rightCabCorner 
      pause 
      exit 
    * 
    save grids to a file... 
    file name: rightCabCorner.hdf 
    save file 
    exit 
    exit 
  exit 
