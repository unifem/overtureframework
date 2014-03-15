#
# make a stirring stick
#     ogen -noplot stir.cmd -interp=i
#     ogen -noplot stir.cmd -interp=e
#
$order=2; $orderOfAccuracy = "second order"; 
$ng=2;   # number of ghost points 
$interp="i"; 
$sharp=40.; 
$nStretch=4.; 
#
GetOptions( "order=i"=>\$order,"interp=s"=> \$interp,"sharp=f"=>\$sharp,"nStretch=f"=>\$nStretch );
*
$name="stir"; 
if( $order eq "4" ){ $orderOfAccuracy = "fourth order"; }
if( $interp eq "i" ){ $interpType="implicit for all grids"; }
if( $interp eq "e" ){ $interpType="explicit for all grids"; }
if( $interp eq "e" ){ $name="stire"; }
* 
* 
create mappings
  rectangle
    set corners
      -.5 .5 -.5 .5
    lines
      35 35
*     31 31
*     41 41 
    boundary conditions
      1 1 1 1
    mappingName
      backGround
  exit
*
SmoothedPolygon
* start on a side so that the polygon is symmetric
  vertices 
    6
    -.05  .00
    -.05  .25
     .05  .25
     .05 -.25
    -.05 -.25
    -.05  .00
  n-stretch
   1. $nStretch 0.
  n-dist
    fixed normal distance
*   .125
    .1
  periodicity
    2
  lines
*    61 7
    69 8
  t-stretch
    0. 1.
    1. 9.
    1. 9.
    1. 9.
    1. 9.
    0. 1.
  # set sharpness of corners
  sharpness
    $sharp
    $sharp
    $sharp
    $sharp
    $sharp
    $sharp
  boundary conditions
    -1 -1 1 0
  share
     0 0 5 0
  mappingName
    stir
  exit
  * pause
exit
*
* now make an overlapping grid
*
generate an overlapping grid
  backGround
  stir
  done
* 
  change parameters
   order of accuracy
    $orderOfAccuracy
   interpolation type
     $interpType
   ghost points
     all
     $ng $ng $ng $ng $ng $ng
  exit
  *   display intermediate
  compute overlap
  *   continue
  *   pause
exit
save an overlapping grid
$name.hdf
stir
exit
