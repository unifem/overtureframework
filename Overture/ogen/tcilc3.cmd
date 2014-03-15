*
* two circles in a long channel
*
***************************************************************************
* scale number of grid points in each direction by the following factor
$factor=2;
*
* Define a subroutine to convert the number of grid points
sub getGridPoints\
{ local($n1,$n2)=@_; \
  $nx=int(($n1-1)*$factor+1.5); \
  $ny=int(($n2-1)*$factor+1.5); \
}
*
**************************************************************************
*
create mappings
  rectangle
    set corners
    -2.5  7.5  -2.5 2.5
    lines
      getGridPoints(321,161);
      $nx $ny
    boundary conditions
      1 1 1 1
    mappingName
    square
    exit
*
  Annulus
    inner radius
      .5
    $radius = .5 + .375;
    outer radius
      $radius
    centre for annulus
      -.6 .6
    lines
     * 169 17  169 33  85 17
     getGridPoints(169,17); 
      $nx $ny
    boundary conditions
    -1 -1 1 0
    mappingName
      unstretched-annulus1
    exit
  * stretch the annulus *********
  *
  * Stretch coordinates
  stretch coordinates
    transform which mapping?
      unstretched-annulus1
    Stretch r2:itanh
      $dx = .006/$factor;
      STP:stretch r2 itanh: position and min dx 0 $dx
    stretch grid
*
    mappingName
    annulus1
    exit
  *
*
  Annulus
    inner radius
    .5
    outer radius
      $radius
    centre for annulus
      +.6 -.6
    lines
     * 169 17  169 33  85 17
     getGridPoints(169,17);
      $nx $ny
    boundary conditions
    -1 -1 1 0
    mappingName
      unstretched-annulus2
    exit
  * stretch the annulus *********
  *
  * Stretch coordinates
  stretch coordinates
    transform which mapping?
    unstretched-annulus2
    Stretch r2:itanh
      STP:stretch r2 itanh: position and min dx 0 $dx
    stretch grid
    mappingName
    annulus2
    exit
  *
  exit
  generate an overlapping grid
    square
    annulus1
    annulus2
    done
    change parameters
      * interpolation type
      *  explicit for all grids
      ghost points
        all
        2 2 2 2 2 2
      exit
    compute overlap
* pause
    exit
  save an overlapping grid
  tcilc3.hdf
  tcilc3
  exit


