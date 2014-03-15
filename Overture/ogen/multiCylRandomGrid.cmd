#
#   Random cylinders in a channel
#
# Examples:
#   ogen -noplot multiCylRandomGrid -factor=2 -name=multiCylRandom2.order2.hdf
#   ogen -noplot multiCylRandomGrid -factor=4 -name=multiCylRandom4.order2.hdf
#   ogen -noplot multiCylRandomGrid -factor=8 -name=multiCylRandom8.order2.hdf
#
create mappings
#
# OLD: 
# $factor=4; $name="multiCylRandom.hdf";
# $factor=8; $name="multiCylRandom8.hdf";
# 
GetOptions( "factor=f"=> \$factor,"name=s"=> \$name );
#
# Define a subroutine to convert the number of grid points
sub getGridPoints\
{ local($n1,$n2,$n3)=@_; \
  $nx=int(($n1-1)*$factor+1.5); $ny=int(($n2-1)*$factor+1.5); $nz=int(($n3-1)*$factor+1.5);\
}
#
rectangle
  set corners
    -2. 2. -1.5 1.5
  lines
 # 161  121  * 129 97  65 49 
    getGridPoints(161,121);
    $nx $ny
  boundary conditions
    1 1 1 1
  mappingName
    backGround
exit
# ======================================================
# Define a function to build and AnnulusMapping 
# usage:
#   makeAnnulus(radius,xCenter,yCenter,name)
# =====================================================
sub makeAnnulus\
{ local($radius,$xc,$yc,$name)=@_; \
  $outerRadius=$radius+.15/$factor;\
  $nxr = int($nx*$radius/.25+.5); \
  $annulusMappingNames = $annulusMappingNames . "   $name\n"; \
  $commands = \
  "Annulus\n" . \
  "lines\n" . \
  "  $nxr $ny\n" . \
  "inner and outer radii\n" . \
  "  $radius $outerRadius\n" . \
  "centre\n" . \
  "   $xc $yc\n" .   \
  "boundary conditions\n" . \
  "  -1 -1 1 0\n" . \
  "mappingName\n" . \
  " $name\n" .  \
  "exit\n"; \
}
#
# $nx=81; $ny=7; # for the annulus
  getGridPoints(81,7);
  $ny=7; # fix lines in the normal direction since we reduce the radius
#
#
makeAnnulus(.125,-1.2,0.2,annulus1);
$commands
#
makeAnnulus(.1,-.8,-.5,annulus2);
$commands
#
makeAnnulus(.0625,-.45,.45,annulus3);
$commands
#
#
makeAnnulus(.25,-.2,-.2,annulus4);
$commands
#
makeAnnulus(.2, .5,.5,annulus5);
$commands
#
makeAnnulus(.0625, .55, -.125,annulus6);
$commands
#
makeAnnulus(.175, .45, -.6,annulus7);
$commands
#
makeAnnulus(.15,-.75,0.0,annulus8);
$commands
#
makeAnnulus(.125,-.9,.75,annulus9);
$commands
#
makeAnnulus(.13,-.4,-.9,annulus10);
$commands
#
makeAnnulus(.19,-.1,.8,annulus11);
$commands
#
#
exit
generate an overlapping grid
    backGround
    $annulusMappingNames
  done
  change parameters
 # choose implicit or explicit interpolation
 # interpolation type
 #   implicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
#  display intermediate results
  compute overlap
# display computed geometry
  exit
#
save an overlapping grid
$name
multiCylRandom
exit

