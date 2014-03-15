  open a data-base 
  /home/henshaw/Overture/hype/cmd/truck/cabWithoutWheels.hdf 
  open an old file read-only 
  get all mappings from the data-base 
  close the data-base 
  * 
  * 
  builder 
    target grid spacing .3 .03 (tang,norm)((<0 : use default) 
    **    *** main grid over the tender ***** 
    build curve on surface 
      plane point 1 24.5 -4.29758 -10.0314 
      plane point 2 24.5 22.1887 -10.0314 
      plane point 3 24.5 -4.29758 10.0692 
      cut with plane 
    *  pause 
      exit 
    * 
    build curve on surface 
      plane point 1 40.2 -4.29758 -10.0314 
      plane point 2 40.2 22.1887 -10.0314 
      plane point 3 40.2 -4.29758 10.0692 
   *   pause 
      cut with plane 
      exit 
    *   plot reference surface 0 
    build surface patch from curves... 
      left curve is extra boundary curve 0 
      right curve is extra boundary curve 1 
      project patch onto surface 0 
      remove twist from periodic patch 
      .1133 
      *      lines 35 105 
      lines 45 121 
      build patch 
      pause 
      explicit ghost lines 1 1 0 0 (l r b t)(green red blue yellow) 
      stretching... 
      GST:pick to stretch 
        GST:default exponent 30 (used with picking) 
        continue 
      GST:stretch r2 0 0.125 30 0.35 (id,weight,exponent,position) 
      GST:stretch r2 1 0.125 30 0.00 (id,weight,exponent,position) 
      GST:stretch r2 2 0.125 30 0.49 (id,weight,exponent,position) 
      GST:stretch r2 3 0.125 30 0.86 (id,weight,exponent,position) 
      GST:stretch r1 4 0.25 10 0. (id,weight,exponent,position) 
      GST:stretch r1 5 1. 2. .8 (id,weight,exponent,position) 
      GST:stretch grid 
*       .new: smooth the grid one iteration
      GSM:number of iterations 1
      GSM:project smoothed grid onto reference surface 0
      GSM:smooth grid
      name tenderSurface 
   pause 
      exit 
    * 
    create volume grid... 
      backward 
      spacing: geometric 
      lines to march 21 
      generate 
      name tender 
      pause 
      exit 
    * 
    save grids to a file... 
    file name: tender.hdf 
    save file 
    exit 
    exit 
  exit 
