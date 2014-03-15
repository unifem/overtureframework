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
    *       change the position of the branch cut 
    edit intersection curve 
      restrict the domain 
      .5 1.5 
      exit 
    * 
    * 
    *  back edge of cab on bottom 
    * 
    create surface grid... 
      choose boundary curve 0 2.300000e+01 3.868764e+00 5.701078e+00 
      done 
      surface grid options... 
      forward 
      target grid spacing 0.3, 0.3 (tang,normal, <0 : use default) 
      Start curve parameter bounds 0.169, 0.505 
      generate 
      post stretching... 
      GST:stretch r1 2 0.125 30 0. (id,weight,exponent,position) 
      GST:stretch r1 0  .125 30 0.2 (id,weight,exponent,position) 
      GST:stretch r1 1 0.125 30 0.8 (id,weight,exponent,position) 
      GST:stretch r1 3 0.125 30 1 (id,weight,exponent,position) 
      GST:stretch grid 
      pause 
      name backCabBottomEdge 
      exit 
    * 
    create volume grid... 
      backward 
      spacing: geometric 
      lines to march 21 
      generate 
      name backCabBottomEdge 
      pause 
      exit 
    save grids to a file... 
    file name: backCabBottomEdge.hdf 
    save file 
    exit 
    exit 
  exit 
