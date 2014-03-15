#
# Make a cylinder in a box
#
# usage: ogen [noplot] cylBox -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<>
#
#  -ml = number of (extra) multigrid levels to support
# 
# examples:
#     ogen noplot cylBox -order=2 -factor=1 
#     ogen noplot cylBox -order=2 -factor=2 -ml=2
#     ogen noplot cylBox -order=4 -factor=1
# 
#     ogen noplot cylBox -order=2 -interp=e -factor=1 
#     ogen noplot cylBox -order=2 -interp=e -factor=2 -ml=2
#     ogen noplot cylBox -order=2 -interp=e -factor=4 
#     ogen noplot cylBox -order=2 -interp=e -factor=8 
#     ogen noplot cylBox -order=2 -interp=e -factor=16
# 
#
$xa=-.5; $xb=.5; $ya=-.5; $yb=.5; $za=-.5; $zb=.5; $nrExtra=2; $loadBalance=0; $ml=0; 
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
$name = "cylBox" . "$interp$factor" . $suffix . ".hdf";
#
$ds=.04/$factor;
$pi = 4.*atan2(1.,1.);
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-1)/$ml2)*$ml2+1; return $n; }
#
create mappings
  Box
    mappingName
      box
   set corners
    $xa $xb $ya $yb $za $zb
   lines
    $nx = intmg( ($xb-$xa)/$ds +1.5);
    $ny = intmg( ($yb-$ya)/$ds +1.5);
    $nz = intmg( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
   boundary conditions
     1 1 2 2 3 3 
   share
     0 0 0 0 1 2
  exit
#
  Cylinder
    mappingName
      cylinder
    bounds on the radial variable
      $deltaRad=5.*$ds; 
      $ra=.2; $rb=$ra+$deltaRad; 
      $ra $rb
    bounds on the axial variable
      $za $zb
    lines
      $nTheta = intmg( 2.*$pi*($ra+$rb)*.5/$ds+.5 );
      $nr = intmg( $deltaRad/$ds+1.5 );
      $nTheta $nz $nr
    boundary conditions
     -1 -1 1 1 1 0   
    share
      0 0 1 2 0 0
  exit
exit
#
#
generate an overlapping grid
    box
    cylinder
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
#  display intermediate results
# pause
  compute overlap
  exit
#
save an overlapping grid
$name
cylBox
exit


