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
      * pause 
      exit 
    * 
    * 
    *  cab top 
    * 
    create surface grid... 
      choose boundary curve 0 2.300000e+01 1.387375e+01 -5.951819e+00 
      done 
      surface grid options... 
      Start curve parameter bounds 0.05, 0.625 
      points on initial curve 71 61 
      target grid spacing .3 .35 
      lines to march 45 
      equidistribution .5 (in [0,1]) 
      generate 
      GST:stretch r1 0 0.25 20 0.33 (id,weight,exponent,position) 
      GST:stretch r1 1 0.25 20 0.66 (id,weight,exponent,position) 
      GST:stretch r2 2  .5 10 .53 (id,weight,exponent,position) 
      GST:stretch grid 
      name cabTopSurface 
      pause 
      exit 
    create volume grid... 
      lines to march 21 
      marching spacing... 
      spacing: geometric 
      generate 
      name cabTop 
      * pause 
      exit 
    * 
    save grids to a file... 
    file name: cabTop.hdf 
    save file 
    exit 
    exit 
  exit 
