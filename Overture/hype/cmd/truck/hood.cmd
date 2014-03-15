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
      plane point 1 3.5 5.5 -10.1416 
      plane point 2 2.5 9 -10.1416 
      plane point 3 3.5 5.5 10.1794 
      cut with plane 
      exit 
    create surface grid... 
      choose boundary curve 0 2.880498e+00 7.668258e+00 -8.403455e-02 
      done 
      forward and backward 
      stop on negative cells 0 
      lines to march 11, 21 (forward,backward) 
      generate 
      pause 
      set view:0 0.441835 0.344863 0 3.20225 0.735913 0.503404 -0.452787 -0.192748 0.79683 0.572635 0.649061 -0.334136 0.683428 
      post stretching... 
      GST:stretch r1 0 0.5 15. .16  (id,weight,exponent,position) 
      GST:stretch r1 1 0.5 15 0.84  (id,weight,exponent,position) 
      GST:stretch r2 2 0.5 10. .25 (id,weight,exponent,position) 
      GST:stretch r2 3 0.5 10 0.9 (id,weight,exponent,position) 
      GST:stretch grid 
      pause 
      GSM:BC: bottom smoothed 
      GSM:BC: top smoothed 
      GSM:number of iterations 2 
      GSM:smooth grid 
      GSM:smoothing offset 3 3 4 20 0 0 (l r b t b f) 
      *      GSM:do not project 0, 2 36 4 11 (id, l r b t) 
      GSM:project smoothed grid onto reference surface 0 
      GSM:smooth grid 
      name hoodSurface 
      pause 
      exit 
    * 
    create volume grid... 
      backward 
      spacing: geometric 
      lines to march 21 
      generate 
      name hood 
      * pause 
      exit 
    save grids to a file... 
    file name: hood.hdf 
    save file 
    exit 
    exit 
  exit 
