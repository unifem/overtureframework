*
* circle in a channel
*
create mappings
*
* $factor=1; $name="multiCyl.hdf";
$factor=2; $name="multiCyl2.hdf";
*$factor=4; $name="multiCyl4.hdf";
*
* Define a subroutine to convert the number of grid points
sub getGridPoints\
{ local($n1,$n2,$n3)=@_; \
  $nx=int(($n1-1)*$factor+1.5); $ny=int(($n2-1)*$factor+1.5); $nz=int(($n3-1)*$factor+1.5);\
}
*
rectangle
  set corners
    -2. 2. -1.5 1.5
  lines
    * 129 97  65 49 
    getGridPoints(129,97);
    $nx $ny
  boundary conditions
    1 1 1 1
  mappingName
    backGround
exit
* --------------------------------
Annulus
  lines
    $outerRadius=.25+.2/$factor;
    $ny0=8; # fixed number of points in the radial direction
    getGridPoints(73,$ny0);
     $nx $ny0
  inner and outer radii
    .25 $outerRadius
  centre
     -.5 0. 
  boundary conditions
    -1 -1 1 0
  mappingName
   annulus1
exit
* --------------------------------
Annulus
  lines
    $nx $ny0 
  inner and outer radii
    .25 $outerRadius
  centre
     .5 .5 
  boundary conditions
    -1 -1 1 0
  mappingName
   annulus2
exit
* --------------------------------
Annulus
  lines
    $nx $ny0
  inner and outer radii
    .25 $outerRadius
  centre
     1. -.5
  boundary conditions
    -1 -1 1 0
  mappingName
   annulus3
exit
*
exit
generate an overlapping grid
    backGround
    annulus1
    annulus2
    annulus3
  done
  change parameters
    * choose implicit or explicit interpolation
    * interpolation type
    *   implicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
*  display intermediate results
  compute overlap
* display computed geometry
  exit
*
save an overlapping grid
$name
multiCyl
exit

