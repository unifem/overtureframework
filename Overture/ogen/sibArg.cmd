#
# Sphere in a Box (taking arguments)
#
# usage: ogen [noplot] sibArg -factor=<num> -order=[2/4/6/8] -interp=[e/i] -nrExtra=<> -ml=<> ...
#                             -numGhost=<i> -rgd=[fixed|var] -prefix=<string>
#
#  -nrExtra: extra lines to add in the radial direction on the sphere grids 
#  -ml = number of (extra) multigrid levels to support
#  -rgd : var=variable : decrease radial grid distance as grids are refined. fixed=fix radial grid distance
# 
# examples:
#     ogen -noplot sibArg -factor=1 -order=2
#     ogen -noplot sibArg -factor=2 -order=4 -interp=e -nrExtra=8    (for cgmx : add extra grid lines in the normal direction)
#     ogen -noplot sibArg -order=4 -interp=i -factor=1
#     ogen -noplot sibArg -order=4 -interp=e -factor=2
#     ogen -noplot sibArg -order=4 -interp=e -factor=4
# 
#     ogen -noplot sibArg -order=2 -interp=e -factor=1 
#     ogen -noplot sibArg -order=2 -interp=e -factor=2 -ml=2
#     ogen -noplot sibArg -order=2 -interp=e -factor=4 
#     ogen -noplot sibArg -order=2 -interp=e -factor=8 
#     ogen -noplot sibArg -order=2 -interp=e -factor=16
# 
#     ogen -noplot sibArg -order=2 -interp=e -factor=2 -ml=2
#     ogen -noplot sibArg -order=2 -interp=e -factor=4 -ml=3
#     ogen -noplot sibArg -order=2 -interp=e -factor=8 -ml=4
# 
#     ogen -noplot sibArg -order=4 -interp=i -factor=1 -ml=1
#     ogen -noplot sibArg -order=4 -interp=i -factor=1 -ml=2 
#     ogen -noplot sibArg -order=4 -interp=e -factor=1 -ml=2  [Backup
#     ogen -noplot sibArg -order=4 -interp=e -factor=2 -ml=2
#     ogen -noplot sibArg -order=4 -interp=e -factor=2 -ml=3
#     ogen -noplot sibArg -order=4 -interp=e -factor=4 -ml=3
#
#  -- more ghost for sosup
#     ogen -noplot sibArg -order=4 -interp=e -numGhost=3 -factor=2
# 
# -- Fixed radius grids
#     ogen -noplot sibArg -order=2 -interp=e -rgd=fixed -prefix=sibFixed -factor=2
#     ogen -noplot sibArg -order=2 -interp=e -rgd=fixed -prefix=sibFixed -factor=4
#
# parallel: 
# srun -N 1 -n 2 -ppdebug $ogenp -noplot sibArg -order=2 -interp=e -factor=4
# srun -N 1 -n 2 -ppdebug $ogenp -noplot sibArg -order=2 -interp=e -factor=16  (33M)
# srun -N 1 -n 2 -ppdebug $ogenp -noplot sibArg -order=2 -interp=e -factor=32  (264M)
# srun -N 8 -n 16 -ppdebug $ogenp -noplot sibArg -order=2 -interp=e -factor=64 (2.1B pts)
#
$xa=-2.; $xb=2.; $ya=-2.; $yb=2.; $za=-2.; $zb=2.; $nrExtra=2; $loadBalance=0; $ml=0; 
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
$numGhost=-1;  # if this value is set, then use this number of ghost points
$rgd="var"; $deltaRadius=.4; 
$prefix="sib";
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"nrExtra=i"=> \$nrExtra,"interp=s"=> \$interp,\
            "loadBalance=i"=>\$loadBalance,"ml=i"=>\$ml,"numGhost=i"=>\$numGhost,\
            "prefix=s"=> \$prefix,"rgd=s"=> \$rgd,"deltaRadius=f"=>\$deltaRadius );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
# 
$suffix = ".order$order"; 
if( $numGhost ne -1 ){ $ng = $numGhost; } # overide number of ghost
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
$name = $prefix . "$interp$factor" . $suffix . ".hdf";
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-1)/$ml2)*$ml2+1; return $n; }
#
# 
$ds=.2/$factor;
# 
# ---------------------------------------
#* turn off graphics
$loadBalanceCmd = $loadBalance ? "load balance" : "*";
$loadBalanceCmd
# ---------------------------------------
#
create mappings
# first make a sphere
Sphere
  $nr=3+$order; if( $interp eq "e" ){ $nr=$nr+$order; } 
  $innerRad=.5; $outerRad=$innerRad+($nr-1)*$ds;
  $nr=intmg($nr + $nrExtra); 
  # check for fixed radius 
  if( $rgd eq "fixed" ){ $outerRad = $innerRad + $deltaRadius; $nr=intmg( $deltaRadius/$ds + 1.5 + $nrExtra ); }
  inner and outer radii
    $innerRad $outerRad
exit
#
# now make a mapping for the north pole
#
reparameterize
  orthographic
 # sa=2 --> patches just match (not including ghost points)
    $sa = 2. + $order*$dse*$ds + ($order-2)*$ds*.5; $sb=$sa; 
    specify sa,sb
      $sa $sb
  exit
  lines
    $nTheta=intmg( 3.2*($innerRad+$outerRad)*.5/$ds +1.5 );    
    $nTheta $nTheta $nr
#    15 15 5
  boundary conditions
    0 0 0 0 7 0
  share
    0 0 0 0 7 0
  mappingName
    north-pole
exit
#
# now make a mapping for the south pole
#
reparameterize
  orthographic
    choose north or south pole
      -1
    specify sa,sb
      $sa $sb
  exit
  lines
    $nTheta $nTheta $nr
  boundary conditions
    0 0 0 0 7 0
  share
    0 0 0 0 7 0
  mappingName
    south-pole
exit
#
# Here is the box
#
Box
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5);
    $ny = intmg( ($yb-$ya)/$ds +1.5);
    $nz = intmg( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
  boundary conditions
    1 2 3 4 5 6 
  mappingName
    box
  exit
exit
#
generate an overlapping grid
  box
  north-pole
  south-pole
  done
  change parameters
 # improve quality of interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
 compute overlap
#
exit
# save an overlapping grid
save a grid (compressed)
$name
sib
exit
