  open a data-base 
  truckCabNoWheels.hdf 
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
      plane point 1 -1. -1. 0.1 
      plane point 2  2. -1. 0.1 
      plane point 3 -1.  8. 0.1 
      cut with plane 
      pause 
      exit 
    * 
    create surface grid... 
      choose boundary curve 0 1.680575e-01 3.313073e+00 1.000000e-01 
      done 
      * pause 
      points on initial curve 51 
      forward and backward 
      initial spacing 0.2 (<0 means choose default) 
      BC: left (backward) outward splay 
      BC: right (backward) outward splay 
      BC: right (forward) outward splay 
      BC: left (forward) outward splay 
      equidistribution .5 (in [0,1]) 
      *      lines to march 34 34  (forward,backward) 
      lines to march 36 36  (forward,backward) 
      generate 
      pause 
      * 
      GSM:BC: bottom smoothed 
      GSM:BC: top smoothed 
      GSM:BC: right smoothed 
      GSM:BC: left smoothed 
      *      GSM:smooth new grid 
      * 
      GST:stretch r1 0 0.5 15 0.85 (id,weight,exponent,position) 
      GST:stretch r1 1 0.5 15 0.15 (id,weight,exponent,position) 
      GST:stretch r1 2 0.5 30 0.45 (id,weight,exponent,position) 
      GST:stretch r1 3 0.5 30 0.57(id,weight,exponent,position) 
      * 
      GST:stretch r2 4 0.5 15 0.08   (id,weight,exponent,position) 
      GST:stretch r2 5 0.5 15 0.25  (id,weight,exponent,position) 
      GST:stretch r2 6 0.5 15 0.75  (id,weight,exponent,position) 
      GST:stretch r2 7 0.5 15 0.92  (id,weight,exponent,position) 
      GST:stretch grid 
      * 
      GSM:smoothing offset 21 19 8 8 0 0 (l r b t b f) 
      GSM:project smoothed grid onto reference surface 0 
      GSM:number of iterations 5 
      GSM:smooth grid 
      name frontSurface 
      pause 
      exit 
    * 
    create volume grid... 
      backward 
      lines to march 21 
      spacing: geometric 
      pause 
      generate 
      name front 
      exit 
    * 
    save grids to a file... 
    file name: front.hdf 
    save file 
    exit 
    exit 
  exit 
