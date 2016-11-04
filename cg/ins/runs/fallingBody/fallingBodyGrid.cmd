#
# Grid for a 2D dropping body in a box
# 
#
#    y=yb ---------------------------------------
#         |                                     |
#         |                                     |
#         |           ------------              | 
#         |           |          |              |
#         |           |   angle  |  height      |  -- cy (center of body)
#         |           ------------              | 
#         |              width                  |
#         |                                     |
#     ya  ---------------------------------------
#        xa                |                    xb
#                          cx
#
# Usage:
#   ogen [-noplot] fallingBodyGrid interp=[i|e] -order=[2|4|6] -factor=<i> -rgd=[fixed|var] ...
#        -shape=[rectangle|trapezoid] -addBottomRefinement=[0|1] -addTopRefinement=[0|1] -improveQuality=[0|1]
#
# Examples:
#    ogen -noplot fallingBodyGrid -cy=-.25 -interp=e -order=2 -factor=2
#    ogen -noplot fallingBodyGrid -cy=-.25 -interp=e -order=2 -factor=4
#    ogen -noplot fallingBodyGrid -cy=-.25 -interp=e -order=2 -factor=8
#    ogen -noplot fallingBodyGrid -cy=-.25 -interp=e -order=2 -factor=16
#
#
#
#  Top -refinement:
#     ogen -noplot fallingBodyGrid -addBottomRefinement=0 -addTopRefinement=1 -prefix=risingBodyGrid -interp=e -order=2 -factor=2
# 
# -- fixed with grids:
#    ogen -noplot fallingBodyGrid -addBottomRefinement=0 -addTopRefinement=1 -prefix=risingBodyGrid  -rgd=fixed -interp=e -order=2 -factor=2
# 
#  Rotated:
#    ogen -noplot fallingBodyGrid -interp=e -order=2 -angle=45 -prefix=fallingBodyGridAngle45 -factor=2 
#    
#  Trapezoid, no bottom refinement: 
#   ogen -noplot fallingBodyGrid -interp=e -order=2 -addBottomRefinement=0 -shape=trapezoid -factor=2 
#    -- rotated by 90 degrees:
#   ogen -noplot fallingBodyGrid -interp=e -order=2 -addBottomRefinement=0 -angle=90 -shape=trapezoid -prefix=fallingTrapezoidGridAngle90 -factor=2 
# 
$prefix=""; $shape="rectangle"; $addBottomRefinement=1; $addTopRefinement=0; $rgd="var";
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$xa =-1.; $xb=1.; $ya=-1.; $yb=1.; 
$name=""; $improveQuality=0; 
$width=1.; $height=.5; 
$cx=0.; $cy=0.;  # center for the body
$angle=0.;  # angle of rotation (degrees)
#
$blf=4;  # grid lines are this much finer near the boundary and bottom wall
$bottomHeight=.2; # height of stretch grid at bottom wall
$topHeight=.2; # height of stretch grid at top wall
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf, "prefix=s"=> \$prefix,\
            "cx=f"=>\$cx,"cy=f"=>\$cy,"rgd=s"=> \$rgd,"angle=f"=>\$angle,"shape=s"=> \$shape,\
            "addBottomRefinement=i"=>\$addBottomRefinement,"addTopRefinement=i"=>\$addTopRefinement,\
            "improveQuality=i"=>\$improveQuality );
#
if( $prefix eq "" && $shape eq "trapezoid" ){ $prefix = "fallingTrapezoidGrid"; }
if( $prefix eq "" ){ $prefix="fallingBodyGrid"; }
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; }
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
# 
$ds=.1/$factor;
$pi = 4.*atan2(1.,1.);
#
create mappings
#
rectangle
  set corners
    if( $addBottomRefinement eq 1 ){ $yac=$ya+$bottomHeight-.5*$ng*$ds; }else{ $yac=$ya; }
    if( $addTopRefinement eq 1 ){ $ybc=$yb-$topHeight+.5*$ng*$ds; }else{ $ybc=$yb; }
    $xa $xb $yac $ybc 
  lines
    $nx = intmg( ($xb-$xa )/$ds+1.5);  
    $ny = intmg( ($ybc-$yac)/$ds+1.5);
    $nx $ny 
  boundary conditions
    if( $addBottomRefinement eq 1 ){ $bcBot=0; }else{ $bcBot=3; } 
    if( $addTopRefinement eq 1 ){ $bcTop=0; }else{ $bcTop=4; } 
    1 2 $bcBot $bcTop
  share
    if( ($addBottomRefinement eq 1) || ($addTopRefinement eq 1 ) ){ $share="1 2 0 0"; }else{ $share="0 0 0 0"; } 
    $share
  mappingName
   channel
exit
#
#
# Stretched grid near bottom wall:
rectangle
  set corners
    $ybw=$ya+$bottomHeight; 
    $xa $xb $ya $ybw 
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
    $ny = intmg( ($ybw-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 2 3 0 
  share
    1 2 0 0 
  mappingName
   unstretchedBottomWallGrid
exit
#
# Stretch bottom wall grid
# 
 stretch coordinates 
  transform which mapping? 
    unstretchedBottomWallGrid
  multigrid levels $ml
  # add extra resolution in the stretching direction: 
  stretch resolution factor  $stretchResolution
  # exponential to linear stretching: 
   Stretch r2:exp to linear
   STP:stretch r2 expl: position 0
   $dxMin = $ds/$blf; 
   STP:stretch r2 expl: min dx, max dx $dxMin $ds
  STRT:name bottomWallGrid
 exit
#
#
#
# Stretched grid near top wall:
rectangle
  set corners
    $yaw=$yb-($topHeight); 
    $xa $xb $yaw $yb 
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
    $ny = intmg( ($ybw-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 2 0 4
  share
    1 2 0 0 
  mappingName
   unstretchedTopWallGrid
exit
#
# Stretched top wall grid
# 
 stretch coordinates 
  transform which mapping? 
    unstretchedTopWallGrid
  multigrid levels $ml
  # add extra resolution in the stretching direction: 
  stretch resolution factor  $stretchResolution
  # exponential to linear stretching: 
   Stretch r2:exp to linear
   STP:stretch r2 expl: position 1.
   $dxMin = $ds/$blf; 
   STP:stretch r2 expl: min dx, max dx $dxMin $ds
  STRT:name topWallGrid
 exit
#
#
#  -- rectangular or trapezoidal body ---
#
#
#                              .......  yrb
#                       .......       |
#                 ......              |
#    ylb  +.......                    |
#         |                           |
#         |                           |
#    yla  +.......                    |
#         xl      ......              |
#                        ......       |
#                               ......+ yra
#                                    xr 
#               
#
$nr = 9+$order;
# $nr = 12+$order;
#
$trapFactor=2.0; # right height is this factor larger than left height 
if( $shape eq "trapezoid" ){ $heightRight=$height*$trapFactor; }else{ $heightRight=$height; }
$xl=$cx-.5*$width; $xr=$xl+$width; 
$yla=$cy-.5*$height; $ylb=$cy+.5*$height;
$yra=$cy-.5*$heightRight; $yrb=$cy+.5*$heightRight; 
# $ybb=$cy+.5*$height; $yba=$ybb-$height; # corners of embedded body
# 
SmoothedPolygon
  # start on a side so that the polygon is symmetric
  vertices 
    $xm=.5*($xl+$xr);   # mid-point on bottom face
    $ym=.5*($yla+$yra); # mid-point on bottom face
    6
    # --- start curve on bottom in middle if a wide body ---
    $xm   $ym
    $xr   $yra
    $xr   $yrb
    $xl   $ylb
    $xl   $yla
    $xm   $ym
  n-stretch
   $nStretch=$blf;  # what should his be ?
   1. $nStretch 0.
  n-dist
    fixed normal distance
    $nDist = ($nr-5)*$ds; 
    if( $rgd eq "fixed" ){ $nDist=.15; $nr=intmg( $nDist/$ds + 4.5 ); }
    -$nDist
  periodicity
    2
  lines
    # $stretchFactor=1.7; # add more lines in the tangential direction due to stretching at corners
    $stretchFactor=1.2; # add more lines in the tangential direction due to stretching at corners
    $length=$ylb-$yla + $yrb-$yra + 2.*sqrt( ($xr-$xl)**2 + ($yra-$yla)**2 ); # perimeter length 
    printf("Body Perimeter: length=$length\n"); 
    #
    $nTheta = int( $stretchFactor*$length/$ds +1.5 ); 
    $nTheta $nr
  $tStretch=7.; 
  t-stretch
    0. 1.
    .2   $tStretch
    .2   $tStretch
    .2   $tStretch
    .2   $tStretch
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
    -1 -1 7 0
  share 
     0  0 0 0
  mappingName
    rectangularBodyUnRotated
  exit
#
  rotate/scale/shift
    transform which mapping?
     rectangularBodyUnRotated
    rotate
     $angle
     $cx $cy 0
    mappingName
     fallingBody
  exit
#
exit
generate an overlapping grid
    channel
    if( $addBottomRefinement eq 1 ){ $cmd="bottomWallGrid"; }else{ $cmd="#"; }
    $cmd
    if( $addTopRefinement eq 1 ){ $cmd="topWallGrid"; }else{ $cmd="#"; }
    $cmd
    fallingBody
  done
  change parameters
 # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ngp = $ng+1;
      $ng $ng $ng $ngp $ng $ng
    if( $improveQuality eq 1 ){ $cmd="improve quality of interpolation"; }else{ $cmd="#"; }
    $cmd
    improve quality algorithm: 1 [0=old,1=new]
    # improve quality of interpolation
    # set quality bound
    #  2.
  exit
  #  display intermediate results
  # open graphics
  compute overlap
#  pause
  exit
#
save an overlapping grid
$name
fallingBodyGrid
exit

