#
# Create an overlapping grid for a 2D valve
#
# usage: ogen [noplot] valveArg -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<>
# 
#  -ml = number of (extra) multigrid levels to support
# 
#  ogen noplot valveArg -factor=1
#  ogen noplot valveArg -interp=e -factor=2
#
# multigrid:
#  ogen noplot valveArg -ml=3 -factor=1
#  ogen noplot valveArg -ml=3 -interp=e -factor=1
#  ogen noplot valveArg -ml=3 -interp=e -factor=2
#  ogen noplot valveArg -ml=4 -interp=e -factor=4
#
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; 
$ml=0; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = "valve" . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=1./50./$factor;
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
#
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-1)/$ml2)*$ml2+1; return $n; }
#
create mappings
 #
 # First make a back-ground grid  
 #
  rectangle
    mappingName
      backGround
    set corners
      # 0 1.  0 1.
      $xa=0.; $xb=1.; $ya=0.; $yb=1.;
      $xa $xb $ya $yb
    lines
 # 41 41
 # 51 51
      # 49 49 
      $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
      $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
      $nx $ny
    share
      1 2 3 4
  exit
 #
 # Now make the valve  
 #
  SmoothedPolygon
    mappingName
      valve
    vertices
 # .4 .4 .65 .65  ok
 # .45 .45 .7 .7  ok
 # .47  .47  .72  .72  ok
 # .475 .475 .725 .725 no
 # .47  .47  .72  .72  last used, ok
     4
     0.47  0.
     0.47  .75
     0.72  .5
     0.72  0.
    n-dist
      fixed normal distance
 # .1
      # $nDist=.05; 
      # $nr = int( $rFactor*$nDist/$ds + 2.5 );
#       -- fix grid points in the normal direction
      $nr = intmg( 8+1.5);
      $nDist = ($nr-5)*$ds;  if( $nDist>.05 ){ $nDist=.05; }
      $nDist
    lines
 # 65 9
 # 75 9
      # 73 9 
      # $rFactor=2.0; # extra grid points in normal direction for stretching
      $length=1.7; # approx length of the curve
      $sFactor=1.1; 
      $ns = intmg( $sFactor*$length/$ds +1.5 );
      $ns $nr 
    boundary conditions
      1 1 1 0
    share
      3 3 0 0 
    sharpness
      15
      15
      15
      15
    t-stretch
      1. 0. 
      1. 3.
      1. 2.
      1. 0.
    n-stretch
      1. 4. 0.
  exit
 #
 # Here is the part of the boundary that 
 # the valve closes against  
 #
  SmoothedPolygon
    mappingName
      stopper
    vertices
      4
      1. .5
      0.75 .5
      0.5 .75
      0.5 1.
      n-dist
        fixed normal distance
 # .1
        # .05
        $nDist
      lines
 # 61 9
 # 61 9
        # 65 9
        $length=1.; # approx length of the curve
        $ns = intmg( $sFactor*$length/$ds +1.5 );
        $ns $nr 
      t-stretch
        1. 0. 
        1. 3.
        1. 3.
        1. 0.
      n-stretch
        1. 4. 0.
      boundary conditions
        1 1 1 0
      share
        2 4 0 0
  exit
exit
#
# Make the overlapping grid
#
generate an overlapping grid
    backGround
    stopper
    valve
  done
  change parameters
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
#  debug
#    7
#  display intermediate results
#- open graphics
#- display intermediate results
#- compute overlap
#- 
#- continue
#- continue
#- 
  compute overlap
#  pause
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
valve
exit
