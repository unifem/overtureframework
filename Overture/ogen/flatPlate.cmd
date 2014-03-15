#
# flatPlate : make a channel geometry for test turbulence models.
#
# Usage:
#   ogen [-noplot] flatPlate -order=[2|4|6|8] -factor=<> -dsMin=<> -name=<> -xa=<> -xb=<> -ya=<> -yb=<>
#
#   -dsMin = grid spacing in normal direction at plate surface 
#
# Examples:
#   ogen -noplot flatPlate -order=2 -factor=1 -dsMin=.01  [ creates flatPlate1.order2.dy.01.hdf
#   ogen -noplot flatPlate -order=2 -factor=2 -dsMin=.01  [ creates flatPlate2.order2.dy.01.hdf
#   ogen -noplot flatPlate -order=2 -factor=2 -dsMin=.001  [ creates flatPlate2.order2.dy.001.hdf
#   ogen -noplot flatPlate -order=2 -factor=4 -dsMin=.005  [ creates flatPlate4.order2.dy.005.hdf
#
# Multigrid:
#   ogen -noplot flatPlate -order=2 -factor=16 -dsMin=.001 -ml=3
#
$prefix="flatPlate";  
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2;
$name=""; $xa=0; $xb=5.; $ya=0.; $yb=1.; 
$dsMin=.01;  # grid spacing in normal direction to the boundary
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "name=s"=>\$name,"ml=i"=>\$ml,"dsMin=f"=>\$dsMin );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order.dy$dsMin"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$factor" . $suffix . ".hdf";}
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
  rectangle
  set corners
    $xa $xb $ya $yb
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 2 3 4 
  mappingName
    rectangle
  exit
#
#
# -- stretch the grid lines in the normal direction.
#
  stretch coordinates
    transform which mapping?
      rectangle
    Stretch r2:exp to linear
    STRT:multigrid levels $ml
    # Transition the grid spacing to the outer box spacing:
    STP:stretch r2 expl: min dx, max dx $dsMin $ds
    # Doing this command again seems to give a more accurate dsMin
    STP:stretch r2 expl: min dx, max dx $dsMin $ds 
    # stretch grid
    STRT:name plate
    # open graphics
  exit
exit
*
generate an overlapping grid
  plate
  done
  change parameters
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng
  exit
  compute overlap
* pause
exit
*
save an overlapping grid
  $name
  flatPlate
exit
