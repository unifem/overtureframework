#*********************************************************************************************************
#
#  Grids for a wind farm and terrain
#
# usage: ogen [noplot] windFarm -factor=<num> -order=[2/4/6/8] -interp=[e/i] -numTowers[0|1|2|4|6|10]
#                -zScale=<> -zb=<>
# -zScale : scale factor for amplitude of ground variation in the z direction
# -zb : height of background
# 
# examples:
#     ogen -noplot windFarm -order=2 -numTowers=0 -zb=10. (builds a single tower, no blades)
#     ogen -noplot windFarm -order=2 -numTowers=1 
#     ogen -noplot windFarm -order=2 -interp=e -numTowers=1 -factor=2  *works*  $yb=3.5
#     ogen -noplot windFarm -order=2 -interp=e -numTowers=1 -factor=4  -- try $yb=4.5  
# 
#     ogen -noplot windFarm -order=2 -numTowers=2  [OK
#     ogen -noplot windFarm -order=2 -numTowers=4 -zScale=6. -zb=11. [ 1 backup 
#     ogen -noplot windFarm -order=2 -numTowers=4 -zScale=6. -zb=11. -ds0=.08 [ backup
#     ogen -noplot windFarm -order=2 -numTowers=6
#     ogen -noplot windFarm -order=2 -numTowers=10 -zScale=8. -zb=12.
#
# -multigrid:
#     ogen -noplot windFarm -order=2 -numTowers=1 -ml=2 [OK
#     ogen -noplot windFarm -order=2 -numTowers=1 -factor=2 -ml=1 [OK
#
#     ogen -noplot windFarm -order=2 -numTowers=2 -ml=1 [backup 
#     ogen -noplot windFarm -order=2 -numTowers=2 -ml=2 [OK
#     ogen -noplot windFarm -order=2 -numTowers=2 -factor=2 -ml=1  [OK now (use explicit hole cutters)
#
#     ogen -noplot windFarm -order=2 -numTowers=4 -zScale=6. -zb=11. -ml=1 [OK, backup 
#     ogen -noplot windFarm -order=2 -numTowers=4 -zScale=6. -zb=11. -interp=e -factor=2 -ml=1 [TROUBLE
#
# srun -N1 -n4 -ppdebug $ogenp -noplot windFarm -order=2 -numTowers=2  --> TROUBLE : finish JoinMap for parallel
# 
#*********************************************************************************************************
#
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $extra=0; $numTowers=1; $zScale=4.;  $ml=0; $ds0=.1; 
$xa=-4.5; $xb=4.5; $ya=-4.5; $yb=4.5; $za=0.; $zb=9.5; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "za=f"=> \$za,"zb=f"=> \$zb,"zScale=f"=> \$zScale,"interp=s"=> \$interp,\
            "numTowers=i"=> \$numTowers,"curve=s"=> \$curve,"ml=i"=>\$ml,"ds0=f"=>\$ds0);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $extra=$order+1; }
if( $factor > 4 ){ $extra=$extra+8; }  # make interface grids a bit wider for higher resolution cases
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){ $name = "windFarm$numTowers". "Towers" . "$interp$factor" . $suffix . ".hdf";}
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
# 
# domain parameters:  
$ds = $ds0/$factor; # target grid spacing
$dsBL = $ds/2.; # boundary layer spacing (spacing in the normal direction)
#
$pi =4.*atan2(1.,1.);
$nr=intmg(5+$extra);       # number of lines in the radial direction 
#
if( $numTowers == 0 ){ $xa=-1.5; $xb=1.5; $ya=-1.5; $yb=1.5; $za=0.; }\
elsif( $numTowers == 1 ){ $xa=-3.5; $xb=4.5; $ya=-2.5; $yb=3.5; $za=0.; }\
elsif( $numTowers == 2 ){ $xa=-5.0; $xb=5.5; $ya=-5.0; $yb=5.0; $za=0.; }\
elsif( $numTowers == 4 ){ $xa=-7.0; $xb=9.; $ya=-4.5; $yb=5.0; $za=0.; }\
elsif( $numTowers == 6 ){ $xa=-7.0; $xb=10.; $ya=-4.5; $yb=8.0; $za=0.; }\
elsif( $numTowers ==10 ){ $xa=-7.0; $xb=17.; $ya=-5.0; $yb=12.0; $za=0.; }
#
# ----perl subroutine ---------------------------------------------------------------------
#  bump(x,y, amp,x0,y0,w0) 
#    amp : amplitude of the bump 
#    x0,y0 : center of the bump
#    w0 = width of the bump
sub bump\
{ local($x,$y,$amp,$x0,$y0,$w0)=@_; \
  $rr= sqrt( (($x-$x0)/$w0)**2 + (($y-$y0)/$w0)**2 );\
  if( $rr < -.5 || $rr > .5 ){ return 0.; }else{ return $amp*(-1.-cos(2.*$pi*$rr));}\
}
# -----------------------------------------------------------------------------------------
$xScale=($xb-$xa)*.75; if( $xScale < 5. ){ $xScale=5.; }
$amp1=-.20*$zScale; $x1= 0.*$xScale; $y1= 0.*$xScale; $w1=1.5*$xScale; 
$amp2=-.10*$zScale; $x2=-.4*$xScale; $y2=-.2*$xScale; $w2= .6*$xScale; 
$amp3=-.08*$zScale; $x3=+.5*$xScale; $y3=+.1*$xScale; $w3= .7*$xScale; 
# ----perl subroutine ---------------------------------------------------------------------
#  groundLevel(x,y) 
sub groundLevel\
{ \
local($x,$y)=@_; \
$z  = bump($x,$y, $amp1,$x1,$y1,$w1); \
$z += bump($x,$y, $amp2,$x2,$y2,$w2); \
$z += bump($x,$y, $amp3,$x3,$y3,$w3); \
return $z; \
}
# -----------------------------------------------------------------------------------------
#
create mappings 
#
#* -- Define the Nurbs for the volume grid next to the ground --
# 
 $cmd="";
$nz=$nr;     # number of grid lines in the z direction 
$nzNurbs=3;  # number of z levels in the Nurbs (at least 3 needed for degree 3)
$deltaZ=($nz-1)*$ds/($nzNurbs-1); 
$nxNurbs=11; $nyNurbs=11;    # the number of points defining the nurbs 
$hx=($xb-$xa)/($nxNurbs-1); $hy=($yb-$ya)/($nyNurbs-1);
$zMin=100.; $zMax=-$zMin; 
for( $k=0; $k<$nzNurbs; $k++){ $z1=$k*$deltaZ; for( $j=0; $j<$nyNurbs; $j++){for( $i=0; $i<$nxNurbs; $i++){ \
$x=$xa + $hx*$i; $y=$ya + $hy*$j; \
$z = $z1 + groundLevel($x,$y); \
if( $z<$zMin ){ $zMin=$z; } if( $z>$zMax ){ $zMax=$z; }\
$cmd=$cmd . "$x $y $z\n"; }}}
#
#
# 
# -- tower and blade parameters --
$towerRadius=.3; # radius of the tower
$towerStart=0.; $towerEnd=4.;
$towerBC=7;  
# shared boundary flag for tower: (must not overlap with share flag for blades)
$towerShare=52+$numTowers*3;    
$bladeBC=8; 
$towerBottomBC=0;  # BC at bottom of tower, set to zero if we have a tower base
$groundBC=5; $groundShare=5;    # share for ground level
$rDist=($nr-1)*$ds;  # normal distance for grids  
# 
#
create mappings 
# 
#* -- Define the Nurbs for the volume grid next to the ground --
# 
nurbs
$degree=3;
set domain dimension
  3
set range dimension
  3 
enter points 
  $nxNurbs $nyNurbs $nzNurbs
$cmd
lines
  $nx = intmg( ($xb-$xa)/$ds +1.5);
  $ny = intmg( ($yb-$ya)/$ds +1.5);
  $nx $ny $nz 
boundary conditions
 1 2 3 4 $groundBC 0
share
 1 2 3 4 $groundShare 0
mappingName
  ground
exit
# 
$xTowerShift=1.;  $yTowerShift=0;  $zTowerShift=0.;
#
  $numberOfBlades=0; # counts total number of blades
  $gridNames="#";     # list of grid names
  $numberfTowers=0;  # counts number of towers
  $explicitHoleCutterNames="#"; # list of hole cutters
# 
include towerWithCap.h
# 
#
$radius=.25; # radius of the wire
$cylStart=-1.5; $cylEnd=1.5; 
$wireShare=10;   # shared boundary flag for the wire surface
* 
include cylWithCaps.h
#
# -- flatten and rotate to make a blade
  $bladeAngle=90.-25.; $bladeFlatten=.5; $bladeShare=$wireShare;
  $hubRadius=$radius*.5; # offset of the blade from the axis 
  # exp: $hubRadius=$radius*.95; 
  $xHubShift = $hubRadius - $cylStart;  # shift so the axis is (0,0,0)
  $xBladeShift=0.; $yBladeShift=0.; $zBladeShift=0.; 
# 
$bladeOffsetFromTower=.7; # offset blades from the tower 
# exp $bladeOffsetFromTower=.8; # offset blades from the tower -- make at least .75 for interp=e, factor=1
$bladeHeight=$towerEnd-.25;        # height of blades 
#
if( $numTowers == 0 ){ $cmds = "include buildTowers0.h"; }\
elsif( $numTowers == 1 ){ $cmds = "include buildTowers1.h"; }\
elsif( $numTowers == 2 ){ $cmds = "include buildTowers2.h"; }\
elsif( $numTowers == 4 ){ $cmds = "include buildTowers4.h"; }\
elsif( $numTowers == 6 ){ $cmds = "include buildTowers6.h"; }\
elsif( $numTowers == 10 ){ $cmds = "include buildTowers10.h"; }
$cmds
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
      1 2 3 4 0 6
    share
      1 2 3 4 0 0 
  exit
#
exit
#
generate an overlapping grid 
  backGround
  ground
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
    # define mappings that act as explicit hole cutters
    define explicit hole cutters
      $explicitHoleCutterNames
    done
  exit
# 
# change the plot
#   set debug parameter
#   3
#   compute overlap
#   continue
#   continue
#   continue
open graphics


# 
# 	
  compute overlap
# 
  exit
#
save an overlapping grid
$name
windFarm
exit











  mapping from normals
    extend normals from which mapping?
    interfaceCurve
    $nr=7+$extra; 
    normal distance
      $dist=-($nr-2)*$ds; 
      $dist 
    lines
      $length=($xb-$xa) + $amp*2.; 
      $nx = int( $length/$ds + 1.5 );
      $nz=$nx; 
      $nx $nz $nr 
    boundary conditions
      1 2 5 6 3 0 
    share
      1 2 5 6 3 0 
    mappingName
     terrain
# pause
# 
  exit







# -- tower ---
  cylinder
    orientation
    2 0 1
    centre for cylinder
      -.1 0 .2 
    bounds on the radial variable
      .05 .15
    bounds on the axial variable
      0 .5 
    boundary conditions
      -1 -1 3 4 7 0
    lines
      $nTheta=21; $nAxial=21; $nr=5;
      $nTheta $nAxial $nr
    mappingName
      tower
    exit
  join
    choose curves
    tower 
    interfaceCurve
    compute join
    boundary conditions
      -1 -1 3 4 7 0 
    share  
       0 0 3 4 7 0 
    mappingName
      tower1
# open graphics
    exit
# 
  $xar=$xa; $xbr=$xb; $yar=$yMin; $ybr=$yb; $zar=$za; $zbr=$zb; 
Box
    mappingName
      backGround
  set corners
    $xar $xbr $yar $ybr $zar $zbr
  lines
    $nx = int( ($xb-$xa)/$ds +1.5);
    $ny = int( ($yb-$ya)/$ds +1.5);
    $nz = int( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
    boundary conditions
      1 2 0 4 5 6
    share
      1 2 0 4 5 6
  exit
#
exit
#
generate an overlapping grid 
  backGround
  terrain
  tower1
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
  change the plot
  open graphics

  

  compute overlap
# 
  exit
#
save an overlapping grid
$name
windFarm
exit
