#
# Grid for a curved dielectric block in a channel
#
#
# usage: ogen [noplot] curvedBlockGrid2d -factor=<num> -order=[2/4/6/8] -interp=[e/i] -prefix=<s> ...
#                             -xa=<> -xb=<> -ya=<> -yb=<> -width=<f> -interfaceGridWidth=<f>
#                             -dxLeft= f f f .. -dxRight= f f f ...
# 
#  -width = width of undeformed block 
#  -interfaceGridWidth = width of curvilinear grids near the interface (must be larger than the dxLeft and dxRight)
#  -dxLeft = list of delta-x shifts of left control points 
# 
#  -dxRight = list of delta-x shifts of right control points 
# 
#  -ml = number of (extra) multigrid levels to support
#  -xa, -xb, -ya, -yb : bounds on the back ground grid
#
#                       periodic 
#           yb  +-----------------------+
#               |        |     |        |
#               |        |     |        |
#               |       /       \       |
#               |      |         |      |
#               |       \       /       |
#               |        |     |        |
#               |        |width|        |
#           ya  +-----------------------+
#               xa      periodic       xb
# 
# examples:
#     ogen -noplot curvedBlockGrid2d -order=2 -interp=e -width=.5 -deltaLeft=.05 -deltaRight=.05 -factor=4
#
#
#
$prefix="curvedBlockGrid2d";  $rgd="var"; $angle=0.; 
$numGhost=-1;  # if this value is set, then use this number of ghost points
$width=.5;  # width of the block
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-1.5; $xb=1.5; $ya=-.5; $yb=.5; 
$cx=0.; $cy=0.;  # center of the block 
# 
# Grid near interface is this wide:
$interfaceGridWidth=.5; 
#  --- Define delta's in the control points ---
@dxLeft = ();  @dyLeft = (); @dxRight = ();  @dyRight = (); # these must be null for GetOptions to work, defaults are given below
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml, "prefix=s"=> \$prefix,"numGhost=i"=> \$numGhost,\
            "cx=f"=>\$cx,"cy=f"=>\$cy,"width=f"=>\$width,"interfaceGridWidth=f"=>\$interfaceGridWidth, \
	    "dxLeft=f{1,}"=>\@dxLeft,   "dyLeft=f{1,}"=>\@dyLeft, \
	    "dxRight=f{1,}"=>\@dxRight, "dyRight=f{1,}"=>\@dyRight );
#
# Give defaults here for array arguments: 
if( $dxLeft[0] eq "" ){ @dxLeft=(0,0,0,0); }
if( $dyLeft[0] eq "" ){ @dyLeft=(0,0,0,0); }
if( $dxRight[0] eq "" ){ @dxRight=(0,0,0,0); }
if( $dyRight[0] eq "" ){ @dyRight=(0,0,0,0); }
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
if( $numStir eq 1 ){ $prefix = $prefix . "1"; }
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; }
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $numGhost ne -1 ){ $ng = $numGhost; } # overide number of ghost
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
$pi = 4.*atan2(1.,1.);
# 
$dw = $order+1; $iw=$order+1; 
# parallel ghost lines: for ogen we need at least:
#       .5*( iw -1 )   : implicit interpolation 
#       .5*( iw+dw-2 ) : explicit interpolation
$parallelGhost=($iw-1)/2;
if( $interp eq "e" ){  $parallelGhost=($iw+$dw-2)/2; }
if( $parallelGhost<1 ){ $parallelGhost=1; } 
minimum number of distributed ghost lines
  $parallelGhost
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
#
create mappings
#
# Create a curve for the right side of the middle block
#
 $x0=$width*.5; 
 # place y-points on a straight vertical line -- cluster near ends to
 # make nearly vertical for periodicity in y 
 $yba=$yb-$ya; # vertical height 
 $y0=$ya+ 0*$yba; 
 $y1=$ya+.1*$yba;
 $y2=$ya+.2*$yba;
 $y3=$ya+.5*$yba;
 $y4=$ya+.8*$yba;
 $y5=$ya+.9*$yba;
 $y6=$ya+1.*$yba;
#
 $y0=$ya+ 0 *$yba; 
 $y1=$ya+.05*$yba;
 $y2=$ya+.1 *$yba;
 $y3=$ya+.5 *$yba;
 $y4=$ya+.9 *$yba;
 $y5=$ya+.95*$yba;
 $y6=$ya+1. *$yba;
 nurbs (curve)
  # First define a flat interface (changed below)
  enter points
    7 3
    $x0  $y0
    $x0  $y1
    $x0  $y2
    $x0  $y3
    $x0  $y4
    $x0  $y5
    $x0  $y6
  lines
      51
  boundary conditions
       -1 -1
  plot control points 1
  # ---- CHANGE CONTROL POINTS ----
  # **NOTE FOR NOW JUST CHANGE THE MIDDLE CONTROL POINT
  # **NOTE y-vales for other control points are NOT equal to $y2, $y4 etc.
  # **NOTE: We should add an option to add shifts to the control points
  change control points
    $x2 = $x0 + $dxRight[2]; 
    $x3 = $x0 + $dxRight[3]; 
    $x4 = $x0 + $dxRight[4]; 
 #    2
 #     $x2 $y2 $weight
    3
     $x3 $y3 $weight
 #    4
 #     $x4 $y4 $weight
   done
  mappingName
    rightCurve
  # pause
 exit
#
# Create a curve for the left side of the middle block
#
 $x0=-$width*.5;
 nurbs (curve)
  # First define a flat interface (changed below)
  enter points
    7 3
    $x0 $y0
    $x0 $y1
    $x0 $y2
    $x0 $y3
    $x0 $y4
    $x0 $y5
    $x0 $y6
  lines
      51
  boundary conditions
       -1 -1
  plot control points 1
  # ---- CHANGE CONTROL POINTS ----
  change control points
    $x2 = $x0 + $dxLeft[2]; 
    $x3 = $x0 + $dxLeft[3]; 
    $x4 = $x0 + $dxLeft[4]; 
#    2
#      $x2 $y2 $weight
    3
     $x3 $y3 $weight
#     4
#      $x4 $y4 $weight
   done
  mappingName
    leftCurve
  #   pause
 exit
#
#  TFI mapping for the block
#
  tfi
    choose left curve   (r_1=0)
      leftCurve
    choose right curve  (r_1=1)
      rightCurve
    lines
      $scaleFactor=1.2; # make grid a bit finer in x
      $lensWidth = $width + max(0,$dxRight[3]-$dxLeft[3]); # estimate lens width 
      $nx = intmg( ($scaleFactor*$lensWidth)/$ds +1.5 ); 
      $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
      $nx $ny
    boundary conditions
      100 101 -1 -1
    share
      100 101 0 0
    mappingName
      middleBlock
 exit
#
# Create a straight curve for the left side of the left interface grid 
#
 $x0=-.5*$width -$interfaceGridWidth;
 $x1=$x0;
 nurbs (curve)
  enter points
    7 3
    $x0  $y0
    $x0  $y1
    $x0  $y2
    $x1  $y3
    $x0  $y4
    $x0  $y5
    $x0  $y6
  lines
      51
  boundary conditions
       -1 -1
  # plot control points 1
  mappingName
    leftInterfaceCurve
  # pause
 exit
#
# TFI mapping for left interface
  tfi
    choose left curve   (r_1=0)
      leftInterfaceCurve
    choose right curve  (r_1=1)
      leftCurve
    lines
      $nx = intmg( ($interfaceGridWidth)/$ds +1.5 ); 
      $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
      $nx $ny
    boundary conditions
      0 100 -1 -1
    share 
      0 100 0 0
    mappingName
      leftInterface
 exit
#
# Create a straight curve for the right side of the right interface grid 
#
 $x0=$width*.5+$interfaceGridWidth;
 $x1=$x0;
 nurbs (curve)
  enter points
    7 3
    $x0  $y0
    $x0  $y1
    $x0  $y2
    $x1  $y3
    $x0  $y4
    $x0  $y5
    $x0  $y6
  lines
      51
  boundary conditions
       -1 -1
  # plot control points 1
  mappingName
    rightInterfaceCurve
  # pause
 exit
#
# TFI mapping for right interface
# 
  tfi
    choose left curve   (r_1=0)
      rightCurve
    choose right curve  (r_1=1)
      rightInterfaceCurve
    lines
      $nx = intmg( ($interfaceGridWidth)/$ds +1.5 ); 
      $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
      $nx $ny
    boundary conditions
      101 0  -1 -1
    share 
      101 0 0 0
    mappingName
      rightInterface
    # pause
 exit
#
# ------- Left background grid -----
#
rectangle
  set corners
    $xal=$xa; $xbl=$cx -$width*.5; 
    $xal $xbl $ya $yb
  lines
    $nx = intmg( ($xbl-$xal)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 0 -1 -1 
  share 
    0 0  0 0
  mappingName
   leftBackGround
exit
#
# ------- Right background grid -----
#
rectangle
  set corners
    $xal=$cx + $width*.5; $xbl= $xb;
    $xal $xbl $ya $yb
  lines
    $nx = intmg( ($xbl-$xal)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    0 2 -1 -1 
  share 
    0 0 0 0
  mappingName
   rightBackGround
exit
#
exit
#
#  --- generate the overlapping grid ---
#
generate an overlapping grid
    leftBackGround
    leftInterface
#
    middleBlock
#
    rightBackGround
    rightInterface
  done
  change parameters
    specify a domain
      innerDomain 
        middleBlock
    done
    specify a domain
      outerDomain
        leftBackGround
        leftInterface
        rightBackGround
        rightInterface
    done
    # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ngp = $ng+1;
      $ng $ng $ng $ngp $ng $ng
  exit
  # open graphics
  compute overlap
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
dielectricBlock
exit


