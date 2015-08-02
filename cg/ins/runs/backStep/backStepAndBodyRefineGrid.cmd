#
# Grid for a back step  and body  with refinements near the stepa and wake 
#
#  ybf  +
#       |               far field grid 
#       |
#       |
#   yb  ---------------------------------------------
#       |                                           |
#       |                                           |
#       |                fluid                      |
#       |                                           |
#   yrb |      +------------------------ +          |   far field grid 
#       |      |       refinememt        |          |
#   ys  +------+------+     +--+         |          |
#                     |     |  | body    |          |
#                     |     +--+         |          |
#   ya                +------------------+---------------------------------------+
#       xa     xra    xs                xrb         xb                           xbf (farfield)
#
# usage: ogen [noplot] backStepAndBodyRefineGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i] -name=[] -ml=<i> -blSpacingFactor=<f>
# 
# examples:
# 
#     ogen -noplot backStepAndBodyRefineGrid -interp=e -factor=1
#     ogen -noplot backStepAndBodyRefineGrid -interp=e -factor=2
#     ogen -noplot backStepAndBodyRefineGrid -interp=e -factor=4
# 
#     ogen -noplot backStepAndBodyRefineGrid -interp=e -order=4 -factor=2
#     ogen -noplot backStepAndBodyRefineGrid -interp=e -order=4 -factor=4
#     ogen -noplot backStepAndBodyRefineGrid -interp=e -order=4 -factor=8
# 
# -- multigrid ( Don't make too many levels on coarse grids for cgins -- ghost points? )
#     ogen -noplot backStepAndBodyRefineGrid -interp=e -order=4 -ml=1 -factor=4 
#     ogen -noplot backStepAndBodyRefineGrid -interp=e -order=4 -ml=2 -factor=8
#     ogen -noplot backStepAndBodyRefineGrid -interp=e -order=4 -ml=3 -factor=16
#
$order=2; $factor=1; $interp="i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$name=""; 
$xs=0.; $ys=1.; # corner of step 
# $xa=-3.; $xb=10.; $ya=0.; $yb=3.; 
$xa=-2.; $xb=10.; $ya=0.; $yb=3.;
$xra=-.5; $xrb=5.;  $yrb=2.;   # refinement region
$xbf=20.; # far field distance in x
$ybf=6.; # far field distance in y 
#
$innerRad=.3; $cx=3.; $cy=1.; # raduis and center of annulus body 1
#
$includeTopFarField=1;  # trouble with MG solver with this grid 
# 
$blSpacingFactor=5.; # spacing in the boundary layer is this many times finer. 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "xs=f"=>\$xs,"ys=f"=>\$ys,"interp=s"=> \$interp,"name=s"=> \$name,"per=i"=>\$per,"ml=i"=>\$ml,\
            "blSpacingFactor=f"=>\$blSpacingFactor,"xbf=f"=>\$xbf,"ybf=f"=>\$ybf );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $per eq 1 ){ $suffix .= "p"; }
if( $name eq "" ){$name = "backStepAndBodyRefineGrid" . "$interp$factor" . $suffix . ".hdf";}
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
# 
$ds=.1/$factor;
$dsc=$ds*2.; # coarser spacing 
$pi=4.*atan2(1.,1.);
# 
#
$dw = $order+1; $iw=$order+1;
# parallel ghost lines: for ogen we need at least:
# #       .5*( iw -1 )   : implicit interpolation 
# #       .5*( iw+dw-2 ) : explicit interpolation
$parallelGhost=($iw-1)/2;
if( $interp eq "e" ){  $parallelGhost=($iw+$dw-2)/2; }
if( $parallelGhost<1 ){ $parallelGhost=1; }
minimum number of distributed ghost lines
$parallelGhost
create mappings
#
#   -- middle backGround --
#
$overlap=($order-2)*$dsc+$dsc;  # overlap grids so we can interpolate explicitly
$overlapFine=($order-2)*$ds+$ds;  # overlap grids so we can interpolate explicitly
#
  rectangle
    set corners
      $xa1=$xra-$overlap; $xb1=$xb; $ya1=$yrb-$overlap; $yb1=$yb; 
      if( $includeTopFarField eq 1 ){ $yb1=$yb+$overlap; }
      $xa1 $xb1 $ya1 $yb1
    lines
    $nx = intmg( ($xb1-$xa1)/$dsc +1.5 ); 
    $ny = intmg( ($yb1-$ya1)/$dsc +1.5 ); 
    $nx $ny
    #   101 31
    boundary conditions
      if( $includeTopFarField eq 1 ){ $bcCmd = "0 0 0 0"; }else{ $bcCmd="0 0 0 4"; }
      $bcCmd
    share
      $bcCmd
    mappingName
      middleBackground
    exit
#
#
#   -- coarse inlet grid --
#
  rectangle
    set corners
      $xa1=$xa; $xb1=$xra+$overlap; $ya1=$ys; $yb1=$yb;
      if( $includeTopFarField eq 1 ){  $yb1=$yb+$overlap; }
      $xa1 $xb1 $ya1 $yb1
    lines
    $nx = intmg( ($xb1-$xa1)/$dsc +1.5 ); 
    $ny = intmg( ($yb1-$ya1)/$dsc +1.5 ); 
    $nx $ny
    #   101 31
    boundary conditions
      if( $includeTopFarField eq 1 ){ $bcCmd = "1 0 5 0"; }else{ $bcCmd="1 0 5 4"; }
      $bcCmd
    share
      $bcCmd
    mappingName
      inletUnstretched
    exit
# -- stretch the grid lines on inlet
  stretch coordinates
    transform which mapping?
     inletUnstretched
    Stretch r2:exp to linear
    STP:stretch r2 expl: linear weight 100
    STRT:multigrid levels $ml 
    $dsMin = $dsc/$blSpacingFactor; # grid spacing in the normal direction 
    STP:stretch r2 expl: min dx, max dx $dsMin $ds
    STRT:name inletStretched
    # open graphics
   exit 
#
#   -- coarse mid-stream
#
  rectangle
    set corners
      $xa1=$xrb-$overlap; $xb1=$xb; $ya1=$ya; $yb1=$yrb+$overlap;
      $xa1 $xb1 $ya1 $yb1
    lines
    $nx = intmg( ($xb1-$xa1)/$dsc +1.5 ); 
    $ny = intmg( ($yb1-$ya1)/$dsc +1.5 ); 
    $nx $ny
    #   101 31
    boundary conditions
      0 0 3 0
    share
      0 0 3 0
    mappingName
      midStream
    exit
#
#   -- refined grid near bottom wall 
# 
  rectangle
    set corners
      $nr = intmg(9); # points in normal direction
      $nDist=($nr-3)*$ds; 
      $ybBottomWall=$nDist; # top of bottom wall grid 
      $xa1=$xs; $xb1=$xrb+$overlap; $ya1=$ya; $yb1=$ya + $nDist+$overlapFine;
      $xa1 $xb1 $ya1 $yb1
    lines
    $nx = intmg( ($xb1-$xa1)/$ds +1.5 ); 
    $ny = intmg( ($yb1-$ya1)/$ds +1.5 ); 
    $nx $ny
    #   101 31
    boundary conditions
      5 0 3 0
    share
      5 0 3 0
    mappingName
      bottomWallUnstretched
    exit
#
# -- stretch the grid lines on bottomWall
#
  stretch coordinates
    transform which mapping?
     bottomWallUnstretched
    Stretch r2:exp to linear
    STRT:multigrid levels $ml 
    $dsMin = $ds/$blSpacingFactor; # grid spacing in the normal direction 
    STP:stretch r2 expl: min dx, max dx $dsMin $ds
    # stretch in x-direction next to step -- increase linear weight so we get to linear spacing sooner
    Stretch r1:exp to linear
    STP:stretch r1 expl: linear weight 100
    STP:stretch r1 expl: min dx, max dx $dsMin $ds
    STRT:name bottomWallStretched
    # open graphics
   exit 
#
#   -- refined grid
# 
  rectangle
    set corners
      $xa1=$xra-$overlap; $xb1=$xrb+$overlap; $ya1=$ybBottomWall-$overlapFine; $yb1=$yrb+$overlap;
      $xa1 $xb1 $ya1 $yb1
    lines
    $nx = intmg( ($xb1-$xa1)/$ds +1.5 ); 
    $ny = intmg( ($yb1-$ya1)/$ds +1.5 ); 
    $nx $ny
    #   101 31
    boundary conditions
      0 0 0 0
    share
      0 0 0 0
    mappingName
      refine
    exit
#
#   -- farfield outlet 
# 
  rectangle
    $dsOutlet=$dsc*2.; 
    set corners
      $xa1=$xb-$overlap; $xb1=$xbf; $ya1=$ya; $yb1=$yb;
      if( $includeTopFarField eq 1 ){  $yb1=$yb+$overlap; }
      $xa1 $xb1 $ya1 $yb1
    lines
    $nx = intmg( ($xb1-$xa1)/$dsOutlet +1.5 ); 
    $ny = intmg( ($yb1-$ya1)/$dsOutlet +1.5 ); 
    $nx $ny
    #   101 31
    boundary conditions
      if( $includeTopFarField eq 1 ){ $bcCmd = " 0 2 3 0"; }else{ $bcCmd="0 2 3 4"; }
      $bcCmd
    share
      $bcCmd
    mappingName
      outletUnstretched
    exit
#
# -- stretch the grid lines on outlet
#
  stretch coordinates
    transform which mapping?
     outletUnstretched
    Stretch r1:exp to linear
    STRT:multigrid levels $ml 
    $dsMin = $dsc; # grid spacing in the normal direction 
    $farFieldSpacingFactor=4.; # WHAT SHOULD THIS BE ? propto distance ?
    $dsMax = $farFieldSpacingFactor*$dsOutlet; # spacing at far-field boundary
    STP:stretch r1 expl: linear weight 2
    STP:stretch r1 expl: min dx, max dx $dsMin $dsMax
    STRT:name outletStretched
    # open graphics
   exit 
#
#   -- farfield upper 
# 
  rectangle
    set corners
      $xa1=$xa; $xb1=$xbf; $ya1=$yb-$overlap; $yb1=$ybf;
      $xa1 $xb1 $ya1 $yb1
    lines
    $nx = intmg( ($xb1-$xa1)/$dsOutlet +1.5 ); 
    $ny = intmg( ($yb1-$ya1)/$dsOutlet +1.5 ); 
    # $nx = intmg( ($xb1-$xa1)/$dsc +1.5 ); 
    # $ny = intmg( ($yb1-$ya1)/$dsc +1.5 ); 
    $nx $ny
    #   101 31
    boundary conditions
      1 2 0 4
    share
      1 2 0 0 
    mappingName
      farfieldUpperUnstretched
      # farfieldUpper
    exit
#
# -- stretch the grid lines
#
  stretch coordinates
    transform which mapping?
     farfieldUpperUnstretched
     # farfieldUpper
    $dsMin = $dsc; # grid spacing in the normal direction 
    $farFieldSpacingFactor=4.; # WHAT SHOULD THIS BE ? propto distance ?
    $dsMax = $farFieldSpacingFactor*$dsOutlet; # spacing at far-field boundary
    STRT:multigrid levels $ml 
    # Stretch r1:exp to linear
    # STP:stretch r1 expl: linear weight 2
    # STP:stretch r1 expl: min dx, max dx $dsMin $dsMax
    #
    Stretch r2:exp to linear
    STP:stretch r2 expl: linear weight 2
    STP:stretch r2 expl: min dx, max dx $dsMin $dsMax
    # STRT:name farfieldUpperStretched
    STRT:name farfieldUpperStretched
    # open graphics
   exit 
#
#   -- corner ---
#
  smoothedPolygon
    vertices
      3
      $yBot = $ybBottomWall-$overlapFine;
      $xra $ys
      $xs  $ys
      $xs  $yBot
 #      -3. 1.
 #      0 1
 #      0 0
    $length= $xs-$xra + $ys-$ya; 
    $nr = intmg(9+$order*2); # points in normal direction
    $nDist= $ds*($nr-4);  # account for stretching 
    lines
      $stretchFactor=1.1; 
      $ns = intmg( $stretchFactor*$length/$ds + 1.5 ); # 1.3 : account for stretching 
      $ns $nr
      # 51 8  
    n-dist
    fixed normal distance
      $nDist
     # .3
#
    sharpness
      30.
      30.
      30.
#
    n-stretch
     1. 2. 0 
    t-stretch
      0 50
     .15 10.
     .0 15.
#
    correct corners
#
    boundary conditions
      0 0 5 0
    share
      0 0 5 0
    mappingName
      unstretchedCorner
    exit
# -- stretch the grid lines on the corner grid 
  stretch coordinates
    transform which mapping?
     unstretchedCorner
    Stretch r2:exp to linear
    STRT:multigrid levels $ml 
    $dsMin = $ds/$blSpacingFactor; # grid spacing in the normal direction 
    STP:stretch r2 expl: min dx, max dx $dsMin $ds
    # Stretch to get fine grid near the base of the step (xs,ya)
    # Stretch r1:exp to linear
    # STRT:multigrid levels $ml 
    # STP:stretch r1 expl: position 1
    # STP:stretch r1 expl: min dx, max dx $dsMin $ds
    STRT:name cornerStretched
    # open graphics
   exit 
#
# ---- BODY ----
#
#
Annulus
  # Make sure there are at least 4 points on the coarsest MG level
  $nr = max( 5+ $ng + 2*($order-2), 2**($ml+2) );
  $nr = intmg( $nr );
  $outerRad = $innerRad + ($nr-1)*$ds;
  # if( $rgd eq "fixed" ){ $outerRad = $innerRad + $deltaRadius0; $nr=intmg( $deltaRadius0/$ds + 2.5 ); }
  center: $cx $cy
  inner and outer radii
    $innerRad $outerRad
  lines
    if( $blSpacingFactor>1 ){ $nr = $nr + 4; } # extra grid lines to account for stretching
    $nTheta = intmg( 2.*$pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 11 0
  share
     0  0 11 0
  mappingName
   annulusUnstretched
exit
#
# optionally stretch the grid lines next to the cylinder
# 
 stretch coordinates 
  transform which mapping? 
    annulusUnstretched
  multigrid levels $ml
  # add extra resolution in the stretching direction: 
  stretch resolution factor 2.
  # exponential to linear stretching: 
   Stretch r2:exp to linear
   STP:stretch r2 expl: position 0
   $dxMin = $ds/$blSpacingFactor; 
   STP:stretch r2 expl: min dx, max dx $dxMin $ds
  STRT:name body1
 exit
#
#
# Define a subroutine to convert a Mapping to a Nurbs Mapping
sub convertToNurbs\
{ local($old,$new,$angle)=@_; \
  $commands = "nurbs (surface)\n" . \
              "interpolate from mapping with options\n" . "$old\n" . "parameterize by index (uniform)\n" . "done\n" . \
              "rotate\n" . "$angle 1\n" . "0 0 0\n" . \
              "mappingName\n" . "$new\n" . "exit\n"; \
}
#
# -- convert to Nurbs for parallel ---
convertToNurbs("cornerStretched","corner",0.);
$commands
convertToNurbs("inletStretched","inlet",0.);
$commands
convertToNurbs("bottomWallStretched","bottomWall",0.);
$commands
convertToNurbs("outletStretched","outlet",0.);
$commands
convertToNurbs("farfieldUpperStretched","farfieldUpper",0.);
$commands
#
#
  exit this menu
#
  generate an overlapping grid
    outlet
    if( $includeTopFarField eq 1 ){  $cmd="farfieldUpper"; }else{ $cmd="#"; }
    $cmd
    middleBackground
    inlet
    midStream
    refine
    corner
    bottomWall
    body1
    done
    change parameters
      # choose implicit or explicit interpolation
      interpolation type
        $interpType
      order of accuracy 
        $orderOfAccuracy
      ghost points
        all
        2 2 2 2 2 2
    exit
  # display intermediate results
  # open graphics
  compute overlap
  # pause
  exit
#
save an overlapping grid
$name
backStepAndBodyRefineGrid
exit

