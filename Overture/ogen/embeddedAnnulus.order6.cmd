*
* embedded annulus, for fourth order accuracy
*
create mappings
* $name = "embeddedAnnulus.order6.hdf";  $ds = 1./30.; 
$name = "embeddedAnnulus2.order6.hdf";  $ds = 1./30./2.; 
*
rectangle
  set corners
    -2. 2. -2. 2.
  lines
   $nx = int( 4./$ds + 1.5); $ny=$nx;
     $nx $ny
  boundary conditions
    -1 -1 -1 -1 
  mappingName
  square
exit
*
Annulus
  outer radius
    1.
  lines
    $pi = 3.141592653;
    $nx = int( 2.*$pi*.75/$ds + 1.5); $ny=int( .5/$ds + 1.5);
     $nx $ny
  boundary conditions
    -1 -1 0 0
exit
*
exit
generate an overlapping grid
    square
    Annulus
  done
  change parameters
    * choose implicit or explicit interpolation
    * interpolation type
    *  explicit for all grids
    ghost points
      all
      3 3 3 3
    order of accuracy
      sixth order
  exit
  compute overlap
  exit
*
save an overlapping grid
$name
embeddedAnnulus
exit

