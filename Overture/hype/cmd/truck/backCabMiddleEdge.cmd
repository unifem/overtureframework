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
    create surface grid... 
      choose edge curve 425 2.438092e+01 4.302071e+00 8.530798e-02 
      choose edge curve 424 2.437327e+01 4.326533e+00 4.758937e+00 
      choose edge curve 423 2.437273e+01 4.331041e+00 5.082868e+00 
      choose edge curve 426 2.438846e+01 4.310290e+00 -4.637613e+00 
      choose edge curve 411 2.438886e+01 4.328932e+00 -4.975080e+00 
      done 
      forward and backward 
      lines to march 7 7 (forward,backward) 
      generate 
      name backCabMiddleEdgeSurface 
      pause 
      exit 
    * 
    create volume grid... 
      backward 
      lines to march 21 
      generate 
      name backCabMiddleEdge 
      pause 
      exit 
    save grids to a file... 
    file name: backCabMiddleEdge.hdf 
    save file 
    exit 
    exit 
  exit 
