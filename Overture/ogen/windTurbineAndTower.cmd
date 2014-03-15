#
#   turbine windTurbineAndTower
#
# usage: ogen [noplot] windTurbineAndTower -factor=<num> -order=[2/4/6/8] -interp=[e/i] -nrExtra=<> -ml=<>
#
#  -nrExtra: extra lines to add in the radial direction on the sphere grids 
#  -ml = number of (extra) multigrid levels to support
# 
# examples:
#     ogen noplot windTurbineAndTower -factor=1 -order=2
#
$xa=-2.; $xb=2.; $ya=-2.; $yb=2.; $za=-2.; $zb=2.; $nrExtra=2; $loadBalance=0; $ml=0; 
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"nrExtra=i"=> \$nrExtra,"interp=s"=> \$interp,\
            "loadBalance=i"=>\$loadBalance,"ml=i"=>\$ml);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
$name = "windTurbineAndTower" . "$interp$factor" . $suffix . ".hdf";
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-1)/$ml2)*$ml2+1; return $n; }
#
# 
$ds=.05/$factor;
$pi = 4.*atan2(1.,1.);
#
$bladeLength=6.; 
# $bladeLength=6.; 
#
create mappings
# 
# create the turbine blade and tip:
#
include bladeInclude.cmd
#
#  Create the hub and join for the wind turbine
#
include hubInclude.cmd
#
# -- create grids for blade 1 ---
#
  rotate/scale/shift
    transform which mapping?
      wing
    shift
      # .3 = radius of cylinder on the end of the blade
      # .75 = shift blade outside the hub .. should be $hubRadius + delta 
      -.3 0 .75
    # rotate blade to set angle of attack:
    rotate
      $angleOfAttack=10;
      $angle = 90. - $angleOfAttack; 
      $angle 2
     0 0 0
    boundary conditions
      0 0 -1 -1 2 0
    share
      0 0 0 0 2 0
    mappingName
      blade1
    exit
# 
  rotate/scale/shift
    transform which mapping?
      wingTip
    shift
      -.3 0 .75
    # rotate blade to set angle of attack:
    rotate
      $angle 2
     0 0 0
    boundary conditions
      0 0 0 0 2 0
    share
      0 0 0 0 2 0
    mappingName
      bladeTip1
    exit
#
# -- create grids for blade 2 ---
#
$hubAngle=120.; 
$wingShare = $wingShare +1;
  rotate/scale/shift
    transform which mapping?
      wing
    shift
      -.3 0 .75
    # rotate blade to set angle of attack:
    rotate
      $angleOfAttack=20;
      $angle = 90. - $angleOfAttack; 
      $angle 2
        0 0 0
    # rotate blade around the hub:
    rotate
      $hubAngle 0
      0 0 0
    boundary conditions
      0 0 -1 -1 2 0
    share
      0 0 0 0 $wingShare 0
    mappingName
      blade2
    exit
# 
  rotate/scale/shift
    transform which mapping?
      wingTip
    shift
      -.3 0 .75
    # rotate blade to set angle of attack:
    rotate
      $angle 2
      0 0 0
    # rotate blade around the hub:
    rotate
      $hubAngle 0
      0 0 0
    boundary conditions
      0 0 0 0 2 0
    share
      0 0 0 0 $wingShare 0
    mappingName
      bladeTip2
    exit
# 
  rotate/scale/shift
    transform which mapping?
      hubWingJoin
    rotate
      $hubAngle 0
      0 0 0
    boundary conditions
      -1 -1 5 0 2 0
    share
      0 0 $hubShare 0 $wingShare 0
    mappingName
      hubWingJoin2
    exit
#
# -- create grids for blade 3 ---
#
$hubAngle=240.; 
$wingShare = $wingShare +1;
  rotate/scale/shift
    transform which mapping?
      wing
    shift
      -.3 0 .75
    # rotate blade to set angle of attack:
    rotate
      $angleOfAttack=-10;
      $angle = 90. - $angleOfAttack; 
      $angle 2
        0 0 0
    # rotate blade around the hub:
    rotate
      $hubAngle 0
      0 0 0
    boundary conditions
      0 0 -1 -1 2 0
    share
      0 0 0 0 $wingShare 0
    mappingName
      blade3
    exit
# 
  rotate/scale/shift
    transform which mapping?
      wingTip
    shift
      -.3 0 .75
    # rotate blade to set angle of attack:
    rotate
      $angle 2
      0 0 0
    # rotate blade around the hub:
    rotate
      $hubAngle 0
      0 0 0
    boundary conditions
      0 0 0 0 2 0
    share
      0 0 0 0 $wingShare 0
    mappingName
      bladeTip3
    exit
# 
  rotate/scale/shift
    transform which mapping?
      hubWingJoin
    rotate
      $hubAngle 0
      0 0 0
    boundary conditions
      -1 -1 5 0 2 0
    share
      0 0 $hubShare 0 $wingShare 0
    mappingName
      hubWingJoin3
    exit
#
#  -- include the nacelle and tower
include towerAndNacelleInclude.cmd
#
# Here is the box around the hub and blades 
#
Box
# $xa=-1.75; $xb=1.5; $ya=-1.75; $yb=1.75; $za=-($bladeLength+2.); $zb=$bladeLength+2;
  $xa=-1.75; $xb=4.; $ya=-($bladeLength+2.); $yb=$bladeLength+2; $za=-($bladeLength+2.); $zb=$bladeLength+2;
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5);
    $ny = int( ($yb-$ya)/$ds +1.5);
    $nz = int( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
  boundary conditions
    5 6 $groundShare 4 3 4 
  share
    0 0 $groundShare 0 0 0
  mappingName
    backGround
  exit
exit
#
generate an overlapping grid
   backGround
   hubBody
   hubFrontCap
   hubBackCap
#
   blade1
   bladeTip1
   hubWingJoin
#
   blade2
   bladeTip2
   hubWingJoin2
#
   blade3
   bladeTip3
   hubWingJoin3
#
   tower
   nacelleNurbs
   nacelleCapBackNurbs
   nacelleCapFrontNurbs
   towerNacelleJoinNurbs
# 
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
# open graphics
#  set view:0 0.0512 -0.00464048 0 2.0015 1.45669e-16 -0.766044 -0.642788 0.766044 0.413176 -0.492404 0.642788 -0.492404 0.586824
 compute overlap
exit
# save an overlapping grid
save a grid (compressed)
$name
hub
exit
