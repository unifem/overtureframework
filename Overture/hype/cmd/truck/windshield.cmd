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
      plane point 1 13 12 -10.1416 
      plane point 2 7 18 -10.1416 
      plane point 3 13 12 10.1794 
      cut with plane 
      exit 
    * 
    create surface grid... 
      choose boundary curve 0 1.023102e+01 1.476898e+01 -1.692767e-02 
      done 
      BC: left (backward) outward splay 
      BC: right (backward) outward splay 
      equidistribution .5 (in [0,1]) 
      lines to march 21  23 
      generate 
      post stretching... 
      GST:stretch r1 0 .5 15 .2 (id,weight,exponent,position) 
      GST:stretch r1 1 0.5 15 0.8 (id,weight,exponent,position) 
      GST:stretch grid 
      name windshieldSurface 
      pause 
      exit 
    create volume grid... 
      spacing: geometric 
      lines to march 21 
      generate 
      name windshield 
      pause 
      exit 
    save grids to a file... 
    file name: windshield.hdf 
    save file 
    exit 
    exit 
  exit this menu 
