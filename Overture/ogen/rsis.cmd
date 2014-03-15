#
# rotated square in a square
#
#
# usage: ogen [noplot] rsis -factor=<num> -order=[2/4/6/8] -interp=[e/i] -bc=[d|p]
#   -bc : d=dirichlet, p=periodic boundary conditions
# 
# examples:
#     ogen -noplot rsis -order=2 -interp=i -factor=1
#     ogen -noplot rsis -order=2 -interp=e -factor=2
#     ogen -noplot rsis -order=2 -interp=e -factor=4
#     ogen -noplot rsis -order=2 -interp=e -factor=8
#     ogen -noplot rsis -order=2 -interp=e -factor=16
#
#     ogen -noplot rsis -order=4 -interp=e -factor=2
#     ogen -noplot rsis -order=4 -interp=e -factor=4
#     ogen -noplot rsis -order=4 -interp=e -factor=8
# 
#     ogen -noplot rsis -factor=4 -order=6 -interp=e
#
# -- periodic:
#     ogen -noplot rsis -order=2 -interp=e -bc=p -factor=2
#     ogen -noplot rsis -order=4 -interp=e -bc=p -factor=2
# 
# -- multigrid
#     ogen -noplot rsis -order=2 -interp=e -factor=1 -ml=2
#     ogen -noplot rsis -order=2 -interp=e -factor=2 -ml=2
#     ogen -noplot rsis -order=2 -interp=e -factor=2 -ml=3
#     ogen -noplot rsis -order=2 -interp=e -factor=4 -ml=3 -angle=45.
#
#     ogen -noplot rsis -order=4 -interp=e -factor=1  -ml=2
#     ogen -noplot rsis -order=4 -interp=e -factor=4  -ml=2
#     ogen -noplot rsis -order=4 -interp=e -factor=8  -ml=3
#     ogen -noplot rsis -order=4 -interp=e -factor=16 -ml=4
#     ogen -noplot rsis -order=4 -interp=e -factor=32 -ml=5
#
$order=2; $factor=1; $interp="i"; $bc="d";  $ml=0; $angle=30.; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp,"bc=s"=> \$bc,"ml=i"=>\$ml,"angle=f"=>\$angle);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=3; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=5; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $bc eq "p" ){ $suffix .= "p"; } # periodic
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
$name = "rsis" . "$interp$factor" . $suffix . ".hdf";
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
# 
$ds=.1/$factor;
# 
create mappings
  rectangle
    set corners
     $xa=-1.; $xb=1.; $ya=-1.; $yb=1.; 
     $xa $xb $ya $yb 
    lines
      $nx=intmg( ($xb-$xa)/$ds+1.5 );
      $ny=intmg( ($yb-$ya)/$ds+1.5 );
      $nx $ny
    boundary conditions
      if( $bc eq "d" ){ $cmd = "1 1 1 1"; }else{ $cmd="-1 -1 -1 -1"; }
      $cmd
    mappingName
      outer-square
    exit
#
  rectangle
    set corners
     $xa=-.4; $xb=.4; $ya=-.4; $yb=.4; 
     $xa $xb $ya $yb 
    lines
      $nx=intmg( ($xb-$xa)/$ds+1.5 );
      $ny=intmg( ($yb-$ya)/$ds+1.5 );
      $nx $ny
    boundary conditions
      0 0 0 0
    mappingName
      inner-square-unrotated
  exit
  rotate/scale/shift
    transform
      inner-square-unrotated
    rotate
      $angle
      0. 0. 0.
    mappingName
    inner-square
  exit
exit
#
generate an overlapping grid
  outer-square
  inner-square
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
    order of accuracy
      $orderOfAccuracy
# -- test:
# open graphics
#     interpolation width
#       all
#       all
#       2 2 2
# ---
    interpolation type
      $interpType
  exit
#   debug 
#     7 
#  open graphics
# 
  compute overlap
exit
save an overlapping grid
$name
rsis
exit

