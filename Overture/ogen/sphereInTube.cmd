#
#  Grid for the region outside a sphere and inside a cylindrical tube.
#
# usage: ogen [noplot] sphereInTube -factor=<num> -order=[2/4/6/8] -interp=[e/i] -nrExtra=<> -rgd=[fixed|var] -ml=<> ...
#                                   -stretch=<> -box=<>
#
#  nrExtra: extra lines to add in the radial direction on the sphere grids 
#  rgd : var=variable : decrease radial grid distance as grids are refined. fixed=fix radial grid distance
#  ml = number of (extra) multigrid levels to support
#  box :  if non zero on input then set -xa=xb=-ya=yb=-za=zb=box
#
# examples:
#
#     ogen -noplot sphereInTube -order=2 -interp=e -factor=3
#     ogen -noplot sphereInTube -order=2 -interp=e -factor=4
#     
# -- longer tube:
#      ogen -noplot sphereInTube -order=2 -ya=-1. -yb=2. -suffix=L3 -interp=e -ml=1 -factor=4
#      ogen -noplot sphereInTube -order=2 -ya=-1. -yb=3. -suffix=L4 -interp=e -ml=1 -factor=4
#
#
# ========= OLD: from sphereInABox.cmd:
#     ogen noplot sphereInTube -order=2 -interp=e -factor=1 
#     ogen noplot sphereInTube -order=2 -interp=e -factor=2
#     ogen noplot sphereInTube -order=2 -factor=4
#
#     ogen noplot sphereInTube -order=4 -factor=1 
#     ogen noplot sphereInTube -order=4 -factor=2   (2M pts)
#     ogen noplot sphereInTube -order=4 -factor=4 
# 
#   -- longer in the x direction
#     ogen noplot sphereInTube -order=2 -interp=e -xb=4. -factor=1 
#     ogen noplot sphereInTube -order=2 -interp=e -xb=4. -factor=2 
#     ogen noplot sphereInTube -order=2 -interp=e -xb=4. -factor=4 
#     ogen noplot sphereInTube -order=2 -interp=e -xb=4. -factor=8 
# 
#
#     -- fixed radial distance: 
#     ogen noplot sphereInTube -order=2 -interp=e -rgd=fixed -factor=1 
# 
#     ogen noplot sphereInTube -order=2 -interp=e -factor=4 -nrMin=15 -name="sphereInTubee4nrMin15.order2.hdf"
#
#  -- multigrid
#     ogen noplot sphereInTube -order=2 -interp=e -factor=1 -ml=1
#     ogen noplot sphereInTube -order=2 -interp=e -factor=2 -ml=2
#     ogen noplot sphereInTube -order=2 -interp=e -factor=4 -ml=3
# 
#     ogen noplot sphereInTube -order=4 -interp=e -factor=1 -ml=1
#     ogen noplot sphereInTube -order=4 -interp=e -factor=1 -ml=2
#     ogen noplot sphereInTube -order=4 -interp=e -factor=2 -ml=2
#     ogen noplot sphereInTube -order=4 -interp=e -factor=2 -ml=3
#     ogen noplot sphereInTube -order=4 -interp=e -factor=4 -ml=3 [**NOTE improper interpolation step very slow????
# 
#     ogen noplot sphereInTube -order=4 -interp=e -factor=1 -ml=1 -stretchFactor=1.
#     ogen noplot sphereInTube -order=4 -interp=e -factor=2 -ml=2 -stretchFactor=1.
#     ogen noplot sphereInTube -order=4 -interp=e -factor=4 -ml=3 -stretchFactor=1.
# 
#     ogen noplot sphereInTube -order=4 -interp=e -factor=1 -ml=1 -stretchFactor=4.
#     ogen noplot sphereInTube -order=4 -interp=e -factor=2 -ml=2 -stretchFactor=4.
#     ogen noplot sphereInTube -order=4 -interp=e -factor=4 -ml=3 -stretchFactor=4.
# 
#     ogen noplot sphereInTube -order=4 -interp=e -factor=2 -ml=2
#
#  -- multigrid + fixed radial distance + no stretching
#     ogen noplot sphereInTube -order=4 -interp=e -ml=1 -rgd=fixed -stretchFactor=1 -box=2. -deltaRadius0=.75 -factor=1
#     ogen noplot sphereInTube -order=4 -interp=e -ml=2 -rgd=fixed -stretchFactor=1 -box=2. -deltaRadius0=.5 -factor=2
#     ogen noplot sphereInTube -order=4 -interp=e -ml=3 -rgd=fixed -stretchFactor=1 -box=2. -deltaRadius0=.5 -factor=4
#     ogen noplot sphereInTube -order=4 -interp=e -ml=4 -rgd=fixed -stretchFactor=1 -box=2. -deltaRadius0=.5 -factor=8
#
#  -- Long box:
#     ogen noplot sphereInTube -order=2 -interp=i -xb=6 -suffix="L6" -factor=1 -ml=2
#     ogen noplot sphereInTube -order=2 -interp=e -xb=6 -suffix="L6" -factor=2 -ml=2
#     ogen noplot sphereInTube -order=2 -interp=e -xb=6 -suffix="L6" -factor=4 -ml=3
# 
# Sphere parameters:
$sphereRadius=.2; $radiusDir=1; 
$xSphere=0.; $ySphere=0.; $zSphere=0.; 
# Cylinder parameters:
$ya=-.5; $yb=1.; $cylRadius=.5; 
#
$nrMin=5; $nrExtra=0; $rgd="var"; $name=""; 
$box=0.; # if non zero on input then set -xa=xb=-ya=yb=-za=zb=box
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
$stretchFactor=4.; # stretch grid lines by this factor at the sphere boundary
$deltaRadius0=.2; # do not make larger than .3 or troubles with cgmx
$prefix="sphereInTube";
$suffix=""; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"nrExtra=i"=>\$nrExtra,"nrMin=i"=>\$nrMin,\
            "interp=s"=> \$interp,"rgd=s"=> \$rgd,"deltaRadius0=f"=>\$deltaRadius0,"name=s"=>\$name,\
            "xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,"za=f"=>\$za,"zb=f"=>\$zb,"ml=i"=>\$ml,\
            "stretchFactor=f"=>\$stretchFactor,"box=f"=>\$box,"prefix=s"=>\$prefix ,"suffix=s"=>\$suffix);
# 
if( $box ne 0 ){ $xa=-$box; $xb=$box; $ya=-$box; $yb=$box; $za=-$box; $zb=$box; }
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; $nrExtra=$order; }
# 
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; $sphereWidth=$deltaRadius0; }else{ $sphereWidth=-1.; }
$suffix .= ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){ $name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
# 
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
#
#
# Here is the radial width of the spherical grids -- this will be fixed if rgd=fixed
# matching interface grids should be given distinct share values for now for cgmx -- fix me -- cgmp is different
# ---------------------------------------
# turn off graphics
# ---------------------------------------
$dw = $order+1;
$iw = $dw;
$parallelGhost=($dw+1)/2;
if( $interp eq "e" ){ $parallelGhost=($dw+$iw-2)/2; }
minimum number of distributed ghost lines
  $parallelGhost
#
# ---------------------- create mappings -----------------------
#
create mappings
$pi=4.*atan2(1.,1.);
# number of points to use in the radial direction : $nrExtra is used for stretching 
$nr=$nrMin + $order; if( $interp eq "e" ){ $nr=$nr+$nrExtra; } 
#
$gridNames="*"; 
# 
#
#   ******** Sphere ********
$sphereName="sphere1"; 
$northPoleName="northPole1";
$southPoleName="southPole1"; 
$sphereBC=7; 
$sphereShare=1;   # reset this so the inner sphere has the same corresponding share values
$phiStart=.15; $phiEnd=1. - $phiStart;  # note phiStart=.2 was too big for explicit interp (with moving sphere)
# 
include sphereThreePatch.h
#
#  -- stretch grid lines in the normal direction to the sphere patches --
#
# make grid lines a factor "$stretchFactor" smaller at the wall
$nDist = $ds/$stretchFactor; 
$stretchCmds = "STP:stretch r3 itanh: position and min dx 0 $nDist";  # stretch by specifying the spacing
$stretchCmds = "STP:stretch r3 itanh: layer 0 1. 7.05 0. (id>=0,weight,exponent,position)"; 
# 
  stretch coordinates
    transform which mapping?
      $sphereName
    Stretch r3:itanh
    $stretchCmds
    # stretch grid
    STRT:name sphere
  exit
  stretch coordinates
    transform which mapping?
      $northPoleName
    Stretch r3:itanh
    $stretchCmds
    # stretch grid
    STRT:name northPole
  exit
  stretch coordinates
    transform which mapping?
      $southPoleName
    Stretch r3:itanh
    $stretchCmds
    # stretch grid
    STRT:name southPole
  exit
  # Only use stretched grids if stretchFactor>1 :
  if( $stretchFactor ne 1 ){ $sphereName="sphere"; $northPoleName="northPole"; $southPoleName="southPole"; }
*
* Here is the cylinder
*
  * main cylinder
     cylinder
    mappingName
      cylinder
    * orient the cylinder so y-axis is axial direction
    orientation
      2 0 1
    bounds on the radial variable
      $cylInnerRad = $cylRadius - ($nr-2)*$ds;
      $cylInnerRad $cylRadius
    bounds on the axial variable
      $ya $yb
    lines
      $ns = intmg( ($yb-$ya)/$ds + 1.5 );
      $nTheta = intmg( 2.*$pi*( $cylInnerRad+$cylRadius)*.5/$ds + 1.5 );
      $nTheta $ns $nr
    boundary conditions
      -1 -1 3 4 0 6
    share
      0 0 3 4 0 0 
  exit
#
# core of the main cylinder
#
  Box
    mappingName
      cylinderCore
    $xb = $cylRadius - ($nr -$order-$nrExtra)*$ds;
    $xa=-$xb; $za=-$xb; $zb=$xb;
   set corners
    $xa $xb $ya $yb $za $zb
   lines
    $nx = intmg( ($xb-$xa)/$ds +1.5);
    $ny = intmg( ($yb-$ya)/$ds +1.5);
    $nz = intmg( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
    boundary conditions
      0 0 3 4 0 0 
    share
      0 0 3 4 0 0
  exit
#**********************************
exit
#
generate an overlapping grid
  cylinderCore
  cylinder
  $sphereName
  $southPoleName
  $northPoleName
  done
# 
  change parameters
# 
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
#
  compute overlap
#
exit
# save an overlapping grid
save a grid (compressed)
$name
sphereInTube
exit
