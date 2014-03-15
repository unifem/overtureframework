#*********************************************************************************************************
#
#  Grid for a rounded blade
#
# usage: ogen [noplot] roundedBladeGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i] -numBlades=[0|1|2|3] -theta0=[angle] -checki=[0|1]
# 
# -checki=1 : double check interpolation for parallel runs
#
# examples:
#
#   ogen -noplot roundedBladeGrid -order=2 -interp=e -factor=2 -ml=1 [OK
#   ogen -noplot roundedBladeGrid -order=2 -interp=e -factor=2 -ml=2
#   ogen -noplot roundedBladeGrid -order=2 -interp=e -factor=4 -ml=2
#
# Fourth-order:
#    ogen -noplot roundedBladeGrid -order=4 -interp=e -factor=4 -ml=1 [ok
#    ogen -noplot roundedBladeGrid -order=4 -interp=e -factor=8 -ml=2 [Ok 60M pts
#    ogen -noplot roundedBladeGrid -order=4 -interp=e -factor=8 -ml=3 [Ok 60M pts
#
#*********************************************************************************************************
#
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $checki=0;
$name=""; $extra=0; $numBlades=1; $numTowers=1; $zScale=4.; $ml=0; 
$theta0=0.;  # initial rotation angle (from base angles) in degrees
$xa=-2.5; $xb=2.5; $ya=-1.5; $yb=3.; $za=-2.5; $zb=2.5; 
$blf=4; # boundary layer factor - boundary spacing is this many times smaller than the target
$ds0=.1; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "za=f"=> \$za,"zb=f"=> \$zb,"zScale=f"=> \$zScale,"interp=s"=> \$interp,"checki=i"=>\$checki,\
            "numTowers=i"=> \$numTowers,"curve=s"=> \$curve,"ds0=f"=> \$ds0,"ml=i"=>\$ml,\
            "numBlades=i"=> \$numBlades,"theta0=f"=> \$theta0,"blf=f"=>\$blf);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; $extra=$order; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; $extra=$order; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; $extra=$order; }
if( $interp eq "e" ){ $interpType = "explicit for all grids";  }
## if( $factor > 4 ){ $extra=$extra+8; }  # make interface grids a bit wider for higher resolution cases
# 
$suffix = ".order$order"; 
if( $blf ne 4 ){ $suffix .= ".s$blf"; }
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
$prefix="roundedBladeGrid"; 
if( $name eq "" ){ $name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
# 
# domain parameters:  
$ds = $ds0/$factor; # target grid spacing
$dsBL = $ds/$blf; # boundary layer spacing (spacing in the normal direction)
#
$pi =4.*atan2(1.,1.);
## $nr=intmg( 5+$extra );       # number of lines in the radial direction 
$nr=intmg( 9+$extra );       # number of lines in the radial direction 
#
create mappings 
#
# -- blade parameters --
$bladeBC=8; $bladeShare=8; 
$rDist=($nr-1)*$ds;  # normal distance for grids  
# 
#
create mappings 
# 
#
  $numberOfBlades=0; # counts total number of blades
  $gridNames="*";     # list of grid names
# 
# $capExtent=.3 + ($order-2)*.075; # make cap a bit bigger
$capExtent=.3 + ($order-2)*.05; # make cap a bit bigger
# 
#
$radius=.3; # radius of the wire
$cylStart=-1.25; $cylEnd=1.25; 
$wireShare=$bladeShare;    # shared boundary flag for the wire surface
* 
include cylWithCaps.h
#
# -- flatten and rotate to make a blade (convert to NURBS)
  # $bladeAngle=90.-25.; 
  $bladeAngle=90.; 
  $bladeFlatten=.5; 
  $hubRadius=$radius*.5; # offset of the blade from the axis 
  # exp: $hubRadius=$radius*.95; 
  ### $xHubShift = $hubRadius - $cylStart;  # shift so the axis is (0,0,0)
  $xHubShift = 0.;
  $xBladeShift=0.; $yBladeShift=0.; $zBladeShift=0.; 
# 
#  ============== Blade ======================
$bladeTheta=$theta0; 
include turbineBlade.h
#
#   -----------   background grid ------------
# 
Box
    mappingName
      backGround
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5);
    $ny = intmg( ($yb-$ya)/$ds +1.5);
    $nz = intmg( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
    boundary conditions
      1 2 3 4 5 6
    share
      0 0 0 0 0 0 
  exit
#
exit
#
generate an overlapping grid 
  backGround
  $gridNames
  done 
#
  change parameters
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
    exit
# 
  if( $checki == 1 ){ $cmds="double check interpolation"; }else{ $cmds="#"; }
  $cmds
#  open graphics
#
  compute overlap
# 
  exit
#
save an overlapping grid
$name
roundedBladeGrid
exit

