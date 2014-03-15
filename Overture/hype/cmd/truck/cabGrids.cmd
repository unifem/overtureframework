  * 
  *  Read in the grids for the truck cab 
  * 
  open a data-base 
  cabTop.hdf 
  open an old file read-only 
  get all mappings from the data-base 
  close the data-base 
  * 
  open a data-base 
  hood.hdf 
  open an old file read-only 
  get all mappings from the data-base 
  close the data-base 
  * 
  open a data-base 
  front.hdf 
  open an old file read-only 
  get all mappings from the data-base 
  close the data-base 
  * 
  open a data-base 
  windshield.hdf 
  open an old file read-only 
  get all mappings from the data-base 
  close the data-base 
  * 
  open a data-base 
  body.hdf 
  open an old file read-only 
  get all mappings from the data-base 
  close the data-base 
  * 
  open a data-base 
  tender.hdf 
  open an old file read-only 
  get all mappings from the data-base 
  close the data-base 
  * 
  open a data-base 
  backTender.hdf 
  open an old file read-only 
  get all mappings from the data-base 
  close the data-base 
  * 
  open a data-base 
  backCabTopEdge.hdf 
  open an old file read-only 
  get all mappings from the data-base 
  close the data-base 
  * 
  open a data-base 
  backCabBottomEdge.hdf 
  open an old file read-only 
  get all mappings from the data-base 
  close the data-base 
  * 
  open a data-base 
  backCabMiddleEdge.hdf 
  open an old file read-only 
  get all mappings from the data-base 
  close the data-base 
  * 
  open a data-base 
  leftCabCorner.hdf 
  open an old file read-only 
  get all mappings from the data-base 
  close the data-base 
  * 
  open a data-base 
  rightCabCorner.hdf 
  open an old file read-only 
  get all mappings from the data-base 
  close the data-base 
  * 
  builder 
    target grid spacing .4 .03 
    plot lines on non-physical 0 
    * 
    build a box grid 
      set x min 2.430487e+01 
      set y min 5.293258e+00 
      set y max 1.738408e+01 
      set z min -5.099023e+00 
      set z max 5.129990e+00 
      set x max 2.684642e+01 
      lines: 15, 31, 27 
      GST:stretch r1 0 .5 10. 0. (id,weight,exponent,position) 
      GST:stretch grid 
      name boxBehindCab 
      exit 
    * 
    build a box grid 
      reset:0 
      set x min 4.026665e+01 
      set z min -5.376200e+00 
      set z max 5.386814e+00 
      set y min 5.326247e-01 
      set y max 3.811778e+00 
      lines: 15, 15 41 
      GST:stretch r1 0 1 10 0. (id,weight,exponent,position) 
      GST:stretch grid 
      name boxBehindTender 
      exit 
    * 
    assign BC and share values 
      boundary condition: 0 
      shared boundary flag: 0 
      set BC and share 13 1 2 0 0 
      set BC and share 13 0 2 0 0 
      set BC and share 13 1 0 0 0 
      set BC and share 13 1 1 0 0 
      set BC and share 13 0 1 0 0 
      set BC and share 12 1 2 0 0 
      set BC and share 12 0 2 0 0 
      set BC and share 12 1 0 0 0 
      set BC and share 12 0 1 0 0 
      set BC and share 12 1 1 0 0 
      boundary condition: 1 
      shared boundary flag: 1 
      set BC and share 13 0 0 1 1 
      set BC and share 6 0 2 1 1 
      set BC and share 5 0 2 1 1 
      set BC and share 12 0 0 1 1 
      set BC and share 9 0 2 1 1 
      set BC and share 7 0 2 1 1 
      set BC and share 10 0 2 1 1 
      set BC and share 11 0 2 1 1 
      set BC and share 8 0 2 1 1 
      set BC and share 4 0 2 1 1 
      set BC and share 0 0 2 1 1 
      set BC and share 3 0 2 1 1 
      set BC and share 1 0 2 1 1 
      set BC and share 2 0 2 1 1 
      exit 
    * 
    build a box grid 
      x bounds: -1.73277, 41.7523 
      y bounds: 18.7626, 25 
      z bounds: -7.94166, 7.95945 
      name topBox 
      bc 0 0 0 5 0 0 (l r b t b f) 
      share 0 0 0 5 0 0  (l r b t b f) 
      exit 
    * 
    build a box grid 
      x bounds: -8, 3. 
      y bounds: -3, 25 
      z bounds: -14, 14 
      *      lines: 18, 71, 71 
      name frontBox 
      bc 2 0 4 5 6 7 (l r b t b f) 
      share 2 0 4 5 6 7  (l r b t b f) 
      exit 
    * 
    build a box grid 
      *      x bounds: -1.60196, 50 
      x bounds: -1.60196, 60 
      y bounds: -3, 25 
      z bounds: 6.04869, 14 
      *      lines: 130, 71, 21 
      name leftBox 
      bc 0 3 4 5 0 7 (l r b t b f) 
      share 0 3 4 5 0 7 (l r b t b f) 
      exit 
    * 
    build a box grid 
      *      x bounds: -1.60196, 50 
      x bounds: -1.60196, 60 
      y bounds: -3, 25 
      z bounds: -14 -6.04869 
      *      lines: 130, 71, 21 
      name rightBox 
      bc 0 3 4 5 6 0 (l r b t b f) 
      share 0 3 4 5 6 0 (l r b t b f) 
      exit 
    * 
    build a box grid 
      *      x bounds: 41.3667, 50 
      x bounds: 41.3667, 60 
      y bounds: -3, 25 
      z bounds: -6.67829, 6.93138 
      *      lines: 23, 71, 35 
      bc 0 3 4 5 0 0 (l r b t b f) 
      share 0 3 4 5 0 0   (l r b t b f) 
      name backBox 
      exit 
    * 
    build a box grid 
      x bounds: 24, 42.3145 
      y bounds: 5.29326, 19. 
      z bounds: -6.25, 6.25 
      *      lines: 41, 31, 27 
      bc 0 0 0 0 0 0 (l r b t b f) 
      share 0 0 0 0 0 0  (l r b t b f) 
      name tenderBox 
      exit 
    * 
    build a box grid 
      x bounds: -1.60196, 42.0862 
      y bounds: -3, -0.797693 
      z bounds: -6.83352, 7.00661 
      *      lines: 110, 7, 36 
      name bottomBox 
      bc 0 0 4 0 0 0 (l r b t b f) 
      share 0 0 4 0 0 0 
      exit 
    * 
    build a box grid 
      x bounds: -1.43782, 13. 
      *      y bounds: 8.10544, 19.3939 
      y bounds: 7.25, 19.3939 
      z bounds: -7.94166, 7.95945 
      *     lines: 33, 29, 41 
      bc 0 0 0 0 0 0 (l r b t b f) 
      share 0 0 0 0 0 0 (l r b t b f) 
      name hoodBox 
      exit 
    * 
    save grids to a file... 
    file name: cabTenderGrids.hdf 
    save file 
    exit 
    exit 
  exit 
