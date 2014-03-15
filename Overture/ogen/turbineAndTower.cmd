#*********************************************************************************************************
#
#  Grids for a wind turbine and tower
#
# usage: ogen [noplot] turbineAndTower -factor=<num> -order=[2/4/6/8] -interp=[e/i] -numBlades=[0|1|2|3] -theta0=[angle] -checki=[0|1]
# 
# -checki=1 : double check interpolation for parallel runs
#
# examples:
#     ogen -noplot turbineAndTower -order=2 -ds0=.075 -factor=1  [OK 
#     ogen -noplot turbineAndTower -order=2 -ds0=.075 -factor=1 -ml=1
#     ogen -noplot turbineAndTower -order=2 -interp=e -ds0=.075 -factor=1 -ml=1
#
# One blade and tower:
#     ogen -noplot turbineAndTower -order=2 -interp=e -numBlades=1 -factor=2 -ml=1
#     ogen -noplot turbineAndTower -order=2 -interp=e -numBlades=1 -factor=2 -ml=2
#     ogen -noplot turbineAndTower -order=2 -interp=e -numBlades=1 -factor=4 -ml=2
#
# Three blades:
#     ogen -noplot turbineAndTower -order=2 -interp=e -numBlades=3 -factor=2 -ml=1 -ya=-2. -yb=1.5 -xa=-3. -xb=3.5
#
#     ogen -noplot turbineAndTower -order=2 -interp=e -numBlades=3 -factor=2 -ml=1
#     ogen -noplot turbineAndTower -order=2 -interp=e -numBlades=3 -factor=2 -ml=2
#     ogen -noplot turbineAndTower -order=2 -interp=e -numBlades=3 -factor=4 -ml=2
#
# One blade only:
#     ogen -noplot turbineAndTower -order=2 -interp=e -numTowers=0 -numBlades=1 -factor=2 -ml=1
#
# Fourth-order:
#     Note: offset blade more from tower to avoid backup pts -- why??
#     ogen -noplot turbineAndTower -order=4 -interp=i -numBlades=1 -bladeOffsetFromTower=.9 -factor=2 -ml=1 [OK
#     ogen -noplot turbineAndTower -order=4 -interp=i -numBlades=3 -bladeOffsetFromTower=.9 -factor=2 -ml=1 [OK
#     ogen -noplot turbineAndTower -order=4 -interp=e -numBlades=1 -bladeOffsetFromTower=.9 -factor=3 -ml=1 [backup
#    ogen -noplot turbineAndTower -order=4 -interp=e -numBlades=1 -bladeOffsetFromTower=.9 -factor=4 -ml=1  [ ONE blade
#    ogen -noplot turbineAndTower -order=4 -interp=e -numBlades=2 -bladeOffsetFromTower=.9 -bladeAxisOffset=1.25 -factor=4 -ml=1  [ TWO blades
#    ogen -noplot turbineAndTower -order=4 -interp=e -numBlades=3 -bladeOffsetFromTower=.9 -bladeAxisOffset=1.25 -factor=4 -ml=1  [ THREE blades
#    ogen -noplot turbineAndTower -order=4 -interp=i -numBlades=3 -factor=4 -ml=2  [ THREE blades
#
#
# - less strtetching for cgcns:
#     ogen -noplot turbineAndTower -order=2 -interp=e -factor=2 -blf=2  NOTE: something funny happens with blf=1 ??
# -- parallel 
#   mpirun -np 2 $ogenp -noplot turbineAndTower -order=2 -interp=e -factor=2 -ml=2  [OK
#   mpirun -np 2 ./ogen -noplot turbineAndTower -order=4 -interp=i  -numBlades=3 -factor=2 -ml=1 -numParallelGhost=4
#   srun -N1 -n4 -ppdebug $ogenp -noplot turbineAndTower -order=2 -interp=e -factor=2 -ml=2
#   srun -N1 -n4 -ppdebug $ogenp -noplot turbineAndTower -order=2 -interp=e -factor=4 -ml=2
# 
# -- This fails:
#   mpirun -np 2 $ogenp -noplot turbineAndTower -order=2 -ds0=.075 -factor=1 -ml=1 -theta0=5.
#*********************************************************************************************************
#
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $checki=0;
$name=""; $extra=0; $numBlades=1; $numTowers=1; $zScale=4.; $ml=0; 
$theta0=0.;  # initial rotation angle (from base angles) in degrees
$xa=-4.; $xb=4.; $ya=-2.5; $yb=3.; $za=0.; $zb=7.5; 
$blf=4; # boundary layer factor - boundary spacing is this many times smaller than the target
$bladeOffsetFromTower=.7; # offset blades from the tower 
$bladeAxisOffset=.5; 
# 
$ds0=.1; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "za=f"=> \$za,"zb=f"=> \$zb,"zScale=f"=> \$zScale,"interp=s"=> \$interp,"checki=i"=>\$checki,\
            "numTowers=i"=> \$numTowers,"curve=s"=> \$curve,"ds0=f"=> \$ds0,"ml=i"=>\$ml,\
            "numBlades=i"=> \$numBlades,"theta0=f"=> \$theta0,"blf=f"=>\$blf, "bladeOffsetFromTower=f"=>\$bladeOffsetFromTower,\
            "bladeAxisOffset=f"=>\$bladeAxisOffset );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; $extra=$order; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; $extra=$order; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; $extra=$order; }
## if( $interp eq "e" ){ $interpType = "explicit for all grids"; $extra=$order+1; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# if( $interp eq "e" && $order>2 ){ $extra=$extra+3; }
if( $interp eq "e" && $order>2 ){ $extra=$extra+3; }
## if( $factor > 4 ){ $extra=$extra+8; }  # make interface grids a bit wider for higher resolution cases
# 
$suffix = ".order$order"; 
if( $blf ne 4 ){ $suffix .= ".s$blf"; }
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
$prefix="turbineAndTower"; 
if( $numTowers eq 0 ){ $prefix="turbine"; }
if( $name eq "" ){ $name = "$prefix$numBlades". "Blades" . "$interp$factor" . $suffix . ".hdf";}
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
# -- tower and blade parameters --
$towerRadius=.3; # radius of the tower
$towerStart=0.; $towerEnd=4.;
$towerBC=7;  $towerShare=52;    # shared boundary flag for tower
$bladeBC=8; 
$towerBottomBC=5;  # BC at bottom of tower, set to zero if we have a tower base
$groundBC=5; $groundShare=5;    # share for ground level
$rDist=($nr-1)*$ds;  # normal distance for grids  
# $rDist=($nr-3)*$ds;  # normal distance for grids  
# 
#
create mappings 
# 
$xTowerShift=1.;  $yTowerShift=0;  $zTowerShift=0.;
#
  $numberOfBlades=0; # counts total number of blades
  $gridNames="*";     # list of grid names
  $numberfTowers=0;  # counts number of towers
# 
## $capExtent=.3; # make cap a bit bigger
$capExtent=.3 + ($order-2)*.05; # make cap a bit bigger
$capExtent=.3 + ($order-2)*.075; # make cap a bit bigger
include towerWithCap.h
# 
#
# $radius=.25; # radius of the wire
# $cylStart=-1.5; $cylEnd=1.5; 
$radius=.3; # radius of the wire
$cylStart=-1.25; $cylEnd=1.25; 
$wireShare=5;    # shared boundary flag for the wire surface
* 
include cylWithCaps.h
#
# -- flatten and rotate to make a blade
  $bladeAngle=90.-25.; $bladeFlatten=.5; $bladeShare=7;
  $hubRadius=$radius*$bladeAxisOffset; # offset of the blade from the axis 
  # exp: $hubRadius=$radius*.95; 
  $xHubShift = $hubRadius - $cylStart;  # shift so the axis is (0,0,0)
  $xBladeShift=0.; $yBladeShift=0.; $zBladeShift=0.; 
# 
# $bladeOffsetFromTower=.7; # offset blades from the tower 
# exp $bladeOffsetFromTower=.8; # offset blades from the tower -- make at least .75 for interp=e, factor=1
$bladeHeight=$towerEnd-.25;        # height of blades 
#
#  ============== tower ======================
$xTowerShift=0.;  $yTowerShift=0;  $zTowerShift=0.;
$bladeTheta=$theta0; 
$xBladeShift=$xTowerShift; $yBladeShift=$yTowerShift-$bladeOffsetFromTower; $zBladeShift=$zTowerShift+$bladeHeight;
# 
if( $numTowers > 0 ){ $cmd="include tower.h"; }else{ $cmd="#"; }
$cmd
# ************ Blade 1 *******************
if( $numBlades > 0 ){ $cmd="include turbineBlade.h"; }else{ $cmd="#"; }
$cmd
# ************ Blade 2 *******************
  $bladeTheta=$bladeTheta+120.; $bladeShare=$bladeShare+1;
if( $numBlades > 1 ){ $cmd="include turbineBlade.h"; }else{ $cmd="#"; }
$cmd
# ************ Blade 3 *******************
  $bladeTheta=$bladeTheta+120.; $bladeShare=$bladeShare+1;
if( $numBlades > 2 ){ $cmd="include turbineBlade.h"; }else{ $cmd="#"; }
$cmd
#
# -- tower base that joins to the ground ---
## include towerBase.h
#   -----------   background grid ------------
# 
  $xar=$xa; $xbr=$xb; $yar=$ya; $ybr=$yb; $zar=$zMin+2*$ds; $zbr=$zb; 
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
      1 2 3 4 5 0 
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
# change the plot
  #open graphics
#    debug
#      3
#    compute overlap
#    continue
#    continue
#    continue
#    continue
#    continue
#    continue
#  
# 
  compute overlap
# 
  exit
#
save an overlapping grid
$name
turbineAndTower
exit

