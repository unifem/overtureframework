*
* Two half cylinders in a channel -- you should use twoBumpArg.cmd instead 
*
$factor=1; $grid="twoBump.hdf"; $interpType="implicit for all grids";
* $factor=1; $grid="twoBumpe.hdf"; $interpType="explicit for all grids";
* $factor=2; $grid="twoBump2e.hdf"; $interpType="explicit for all grids";
*
create mappings
*
rectangle
  specify corners
    -1.5 -0. 4. 3.
  lines
    $nx = int( $factor*82 + 1.5);
    $ny = int( $factor*44 + 1.5 );
    $nx $ny
*     83 45     
    * 165 90     
    * 330 180
  boundary conditions
    1 1 1 1
  mappingName
   channel
exit
*
Annulus
  mappingName
    bottomAnnulus
  lines
    $nx = int( $factor*44 + 1.5);
    $ny = int( $factor*8  + 1.5 );
    $nx $ny
    * 45 9
    * 90 18 
    * 180 36
  inner and outer radii
    .5 1.
  start and end angles
    0. .5
  centre for annulus
    .5 0.
  boundary conditions
    1 1 1 0
exit
Annulus
  mappingName
    topAnnulus
  lines
    $nx $ny
    * 90 18 
    * 180 36
  inner and outer radii
    .5 1.
  start and end angles
    .5 1. 
  centre for annulus
    1. 3.
  boundary conditions
    1 1 1 0
exit
*
exit
generate an overlapping grid
    channel
    bottomAnnulus
    topAnnulus
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
    interpolation type
      $interpType
  exit
  compute overlap
  exit
*
save a grid (compressed)
$grid
twoBump
exit

