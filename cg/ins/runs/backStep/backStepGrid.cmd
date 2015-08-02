#
# Grid for a back step
#
#   yb  ---------------------------------------------
#       |                                           |
#       |                fluid                      |
#   ys  +-------------+                             |
#                     |                             |
#   ya                +------------------------------
#       xa            xs                            xb 
#
# usage: ogen [noplot] backStepGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i] -name=[] -ml=<i> -blSpacingFactor=<f>
# 
# examples:
# 
#     ogen -noplot backStepGrid -interp=e -factor=1
#     ogen -noplot backStepGrid -interp=e -factor=2
#     ogen -noplot backStepGrid -interp=e -factor=4
# 
#     ogen -noplot backStepGrid -interp=e -order=4 -factor=2
#     ogen -noplot backStepGrid -interp=e -order=4 -factor=4
#     ogen -noplot backStepGrid -interp=e -order=4 -factor=8
# 
# -- multigrid ( Don't make too many levels on coarse grids for cgins -- ghost points? )
#     ogen -noplot backStepGrid -interp=e -order=4 -ml=1 -factor=4 
#     ogen -noplot backStepGrid -interp=e -order=4 -ml=2 -factor=8
#     ogen -noplot backStepGrid -interp=e -order=4 -ml=3 -factor=16
#
$order=2; $factor=1; $interp="i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$name=""; 
$xs=0.; $ys=1.; # corner of step 
# $xa=-3.; $xb=10.; $ya=0.; $yb=3.; 
$xa=-2.; $xb=8.; $ya=0.; $yb=3.;
$blSpacingFactor=5.; # spacing in the boundary layer is this many times finer. 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "xs=f"=>\$xs,"ys=f"=>\$ys,"interp=s"=> \$interp,"name=s"=> \$name,"per=i"=>\$per,"ml=i"=>\$ml,\
            "blSpacingFactor=f"=>\$blSpacingFactor );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $per eq 1 ){ $suffix .= "p"; }
if( $name eq "" ){$name = "backStepGrid" . "$interp$factor" . $suffix . ".hdf";}
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
# 
$ds=.1/$factor;
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
#   -- backGround --
  rectangle
    set corners
      $xs $xb $ya $yb
    lines
    $nx = intmg( ($xb-$xs)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
    #   101 31
    boundary conditions
      0 2 3 4
    share
      0 0 3 0 
    mappingName
      mainChannel
    exit
#   -- inlet grid 
  rectangle
    set corners
      $delta=.1*($ys-$ya); # the inlet grid is extended downward so there are grid points near the rounded corner
      $yai = $ys-$delta; 
      $xbi = $xs + $order*$ds; # add extra overlap for higher order 
      $xa $xbi $yai $yb
      # -3. 0. 1. 3.
    lines
      $nx = intmg( ($xs-$xa)/$ds +1.5 ); 
      $ny = intmg( ($yb-$ys)/$ds +1.5 ); 
      $nx $ny
      # 31 21
    boundary conditions
      1 0 0 4   
    share
      2 0 0 0
    mappingName
      inlet
    exit
 # 
  smoothedPolygon
    vertices
      3
      $xa $ys
      $xs $ys
      $xs $ya
 #      -3. 1.
 #      0 1
 #      0 0
    $length= $xs-$xa + $ys-$ya; 
    $nr = intmg(9+$order*2); # points in normal direction
    $nDist= $ds*($nr-4);  # account for stretching 
    lines
      $ns = intmg( 1.3*$length/$ds + 1.5 ); # 1.3 : account for stretching 
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
     .1 15.
#
    correct corners
#
    boundary conditions
      1 1 1 0
    share
      2 3 1 0
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
    STRT:name corner
    # open graphics
   exit 
#
  exit this menu
#
  generate an overlapping grid
    mainChannel
    inlet
    corner
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
  compute overlap
  # pause
  exit
#
save an overlapping grid
$name
backStepGrid
exit

