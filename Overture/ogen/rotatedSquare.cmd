* rotated square
#
# ogen noplot rotatedSquare -n=8
# ogen noplot rotatedSquare -n=16
# ogen noplot rotatedSquare -n=32
# ogen noplot rotatedSquare -n=64
# ogen noplot rotatedSquare -n=128
# ogen noplot rotatedSquare -n=256
# 
# ogen noplot rotatedSquare -order=4 -n=16
# ogen noplot rotatedSquare -order=4 -n=32
* 
# ogen noplot rotatedSquare -n=20
# ogen noplot rotatedSquare -n=40
# ogen noplot rotatedSquare -n=80
* 
# ogen noplot rotatedSquare -angle=90 -n=20
# ogen noplot rotatedSquare -angle=90 -n=40
* 
$order=2; $n=10; # default values
$angle=45; 
$orderOfAccuracy = "second order"; $ng=2;
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"n=i"=> \$n,"angle=f"=> \$angle );
$nx=$n+1;
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
* 
$lines = $nx;
$suffix = ".order$order"; 
if( $angle ne 45. ){ $suffix=".angle$angle" . $suffix; } 
$name = "rotatedSquare" . "$n" . $suffix;
*
* 
create mappings
  rectangle
    mappingName
      square
    set corners
      0. 1. 0. 1.
    lines
      $nx $nx 
    boundary conditions
      1 2 3 4 
    mappingName
     rectangularSquare
  exit
*
  rotate/scale/shift
    rotate
      $angle
      0. 0. 0.
    mappingName
      square
  exit
exit
*
generate an overlapping grid
  square
  done
  change parameters
    ghost points
      all
      $ngp = $ng+1;
      $ng $ng $ng $ngp $ng $ng
    order of accuracy
      $orderOfAccuracy
  exit
  compute overlap
exit
*
* save an overlapping grid
save a grid (compressed)
  $name.hdf
  rotatedSquare
exit














*
* rotated square in a square
*
create mappings
  rectangle
    set corners
      0. 1. 0. 1. -1. 1. -1. 1. 
    lines
      21 21  21 31 21 21 
    mappingName
      outer-square
    exit
*
  rotate/scale/shift
    rotate
      45.
      0. 0. 0.
    mappingName
      square
  exit
exit
*
generate an overlapping grid
  square
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
save an overlapping grid
rotatedSquare.hdf
rotatedSquare
exit

