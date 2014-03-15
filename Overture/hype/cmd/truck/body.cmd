  open a data-base 
  /home/henshaw/Overture/hype/cmd/truck/truckCabNoWheels.hdf 
  open an old file read-only 
  get all mappings from the data-base 
  close the data-base 
  * 
  * 
  builder 
    * 
    * 
    *   define the target grid spacings 
    target grid spacing .3 .03 (tang,norm)((<0 : use default) 
    * 
    build curve on surface 
      * 
      *      **** cut the cab to form a boundary curve 
      * 
      plane point 1 23 -5.68201 -10.1416 
      plane point 2 23 22.2989 -10.1416 
      plane point 3 23 -5.68201 10.1794 
      cut with plane 
      exit 
    * 
    * 
    *  cab bottom 
    * 
    *       change the position of the branch cut 
    edit intersection curve 
      restrict the domain 
      .5 1.5 
      exit 
    * 
    create surface grid... 
      choose boundary curve 0 2.300000e+01 3.868764e+00 5.701078e+00 
      done 
      Start curve parameter bounds 0.105, 0.57  0.1, 0.57 
      * NOTE: increase spaing in both directions or else trouble can occur. 
      initial spacing 0.25 
      points on initial curve 131  94 141 
* new      equidistribution .2 (in [0,1]) 
      equidistribution .25 (in [0,1]) 
      * curvature weight 1 (for equidistribution) 
      stop on negative cells 0 
      *      distance to march 19.5 
      march along normals 1 
      lines to march 86 
pause
      generate 
      pause 
      GST:stretch r1 0 0.50 20 0.12 (id,weight,exponent,position) 
      GST:stretch r1 1 0.25 20 0.24 (id,weight,exponent,position) 
      GST:stretch r1 2 0.25 20 0.28 (id,weight,exponent,position) 
      GST:stretch r1 3 0.25 20 0.72 (id,weight,exponent,position) 
      GST:stretch r1 4 0.25 20 0.76 (id,weight,exponent,position) 
      GST:stretch r1 5 0.50 20 0.88 (id,weight,exponent,position) 
      GST:stretch r2 6 .5 20 .35 (id,weight,exponent,position) 
      GST:stretch r2 7 .5 10 .625 (id,weight,exponent,position) 
      GST:stretch grid 
      * pause 
      * 
      GSM:number of iterations 2 
      GSM:BC: top smoothed 
      GSM:BC: bottom smoothed 
      GSM:smooth grid 
      * pause 
      GSM:project smoothed grid onto reference surface 0 
      GSM:smoothing offset 8 8 0 13 0 0 (l r b t b f) 
      GSM:project smoothed grid onto reference surface 0 
      GSM:number of iterations 6 4 
      GSM:smooth grid 
      * 
      *      lines 
      *        101 86 
      name bodySurface 
      pause 
      exit 
    * 
    create volume grid... 
      spacing: geometric 
      dissipation transition 3  (>0 : use boundary dissipation) 
* new      boundary dissipation 0.1 
      lines 
        101 86 22 
      boundary dissipation 0.075
      uniform dissipation 0.2 
      volume smooths 20 
      lines to march 21 
      generate 
pause
      name body 
      pause 
      exit 
    * 
    save grids to a file... 
    file name: body.hdf 
    save file 
    exit 
    exit 
  exit 
