#
# Box beside a Box (taking arguments)
#
# usage: ogen [noplot] boxBesideBox -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<>
#
#  -ml = number of (extra) multigrid levels to support
# 
# examples:
#     ogen noplot boxBesideBox -order=2 -factor=1 
#
#     ogen noplot boxBesideBox -order=2 -interp=e -factor=1 
#     ogen noplot boxBesideBox -order=2 -interp=e -factor=2
#     ogen noplot boxBesideBox -order=2 -interp=e -factor=4 
#
#     ogen noplot boxBesideBox -order=4 -interp=e -factor=1 
#
#     ogen noplot boxBesideBox -order=2 -interp=e -factor=1 -ml=2
#     ogen noplot boxBesideBox -order=2 -interp=e -factor=2 -ml=2
#     ogen noplot boxBesideBox -order=2 -interp=e -factor=4 -ml=3
#     ogen noplot boxBesideBox -order=2 -interp=e -factor=8 -ml=4
# 
#     ogen noplot boxBesideBox -order=4 -interp=e -factor=1 -ml=2
#     ogen noplot boxBesideBox -order=4 -interp=e -factor=2 -ml=2
# 
# parallel: 
# srun -N 1 -n 2 -ppdebug $ogenp noplot boxBesideBox -order=2 -interp=e -factor=4
# srun -N 1 -n 2 -ppdebug $ogenp noplot boxBesideBox -order=2 -interp=e -factor=16  (33M)
# srun -N 1 -n 2 -ppdebug $ogenp noplot boxBesideBox -order=2 -interp=e -factor=32  (264M)
# srun -N 4 -n 8 -ppdebug $ogenp noplot boxBesideBox -order=2 -interp=e -factor=64 
#
$xa=-1.; $xb=1.; $ya=-.5; $yb=.5; $za=-.5; $zb=.5; $loadBalance=0; $ml=0; 
$xai=-.5; $xbi=.5; $yai=-.5; $ybi=.5; $zai=-.5; $zbi=.5; # bounds on the inner box
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp,\
            "loadBalance=i"=>\$loadBalance,"ml=i"=>\$ml);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
$name = "boxBesideBox" . "$interp$factor" . $suffix . ".hdf";
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-1)/$ml2)*$ml2+1; return $n; }
#
# 
$ds=.1/$factor;
# 
# ---------------------------------------
#* turn off graphics
$loadBalanceCmd = $loadBalance ? "load balance" : "*";
$loadBalanceCmd
# ---------------------------------------
#
create mappings
#
# Left-box
#
 Box
  $xai=$xa; $xbi=$ds; $yai=$ya; $ybi=$yb; $zai=$za; $zbi=$zb; 
  set corners
    $xai $xbi $yai $ybi $zai $zbi
  lines
    $nx = intmg( ($xbi-$xai)/$ds +1.5);
    $ny = intmg( ($ybi-$yai)/$ds +1.5);
    $nz = intmg( ($zbi-$zai)/$ds +1.5);
    $nx $ny $nz
  boundary conditions
    1 0 3 4 5 6 
    share 
    0 0 3 4 5 6 
  mappingName
    left-box-rectangular
 exit
 rotate/scale/shift
    mappingName
    left-box
  exit
#
#  Right-box
#
 Box
  $xai=-$ds; $xbi=$xb; $yai=$ya; $ybi=$yb; $zai=$za; $zbi=$zb; 
  set corners
    $xai $xbi $yai $ybi $zai $zbi
  lines
    $nx = intmg( ($xbi-$xai)/$ds +1.5);
    $ny = intmg( ($ybi-$yai)/$ds +1.5);
    $nz = intmg( ($zbi-$zai)/$ds +1.5);
    $nx $ny $nz
  boundary conditions
    0 2 3 4 5 6 
  share 
    0 0 3 4 5 6 
  mappingName
    right-box-rectangular
 exit
 rotate/scale/shift
    mappingName
    right-box
  exit
exit
#
generate an overlapping grid
  left-box
  right-box
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
exit
# save an overlapping grid
save a grid (compressed)
$name
boxBesideBox
exit
