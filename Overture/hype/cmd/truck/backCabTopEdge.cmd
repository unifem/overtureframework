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
    *  cab top 
    * 
    create surface grid... 
      choose boundary curve 0 2.300000e+01 1.387375e+01 -5.951819e+00 
      done 
      surface grid options... 
      forward 
      Start curve parameter bounds 0.02, 0.6525 
      target grid spacing 0.3, 0.2 (tang,normal, <0 : use default) 
      lines to march 16 
      stop on negative cells 0 
      equidistribution weight .1 
      generate 
      pause 
      * 
      GSM:BC: top smoothed 
      GSM:number of iterations 10 
      GSM:smooth grid 
      * 
      GST:stretch r1 0 .25 20 0.35 (id,weight,exponent,position) 
      GST:stretch r1 1 0.25 20 0.65 (id,weight,exponent,position) 
      GST:stretch r2 2 0.25 10 0.36 (id,weight,exponent,position) 
      GST:stretch r1 3 .25 10 0.0 (id,weight,exponent,position) 
      GST:stretch r1 4 .25 10 1.0 (id,weight,exponent,position) 
      GST:stretch grid 
      name backCabTopEdgeSurface 
      pause 
      exit 
    * 
    create volume grid... 
      backward 
      spacing: geometric 
      lines to march 21 
      generate 
      name backCabEdge 
      pause 
      exit 
    * 
    save grids to a file... 
    file name: backCabTopEdge.hdf 
    save file 
    exit 
    exit 
  exit 
