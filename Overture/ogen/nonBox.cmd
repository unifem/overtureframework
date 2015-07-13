#
# make a grid for a nonBox
# usage: ogen [noplot] nonBox -nx=<num> -order=[2/4/6/8] -bc=[d|p]
#   -bc : d=dirichlet, p=periodic boundary conditions
# 
# Examples:
#
#     ogen noplot nonBox -nx=10 
#     ogen noplot nonBox -nx=20 
#     ogen noplot nonBox -nx=40 
#
#     ogen noplot nonBox -bc=p -nx=10 
#     ogen noplot nonBox -bc=p -nx=20 
# 
#     ogen noplot nonBox -order=2 -nx=8
#     ogen noplot nonBox -order=2 -nx=16
#     ogen noplot nonBox -order=2 -nx=32
#     ogen noplot nonBox -order=2 -nx=64
# 
#     ogen noplot nonBox -order=2 -nx=32 -bc=pdd
# 
#     ogen noplot nonBox -order=2 -nx=10 
#     ogen noplot nonBox -order=2 -nx=20 
#     ogen noplot nonBox -order=2 -nx=40 
#     ogen noplot nonBox -order=2 -nx=80 
#     ogen noplot nonBox -order=2 -nx=160
#
#     ogen noplot nonBox -order=4 -nx=10 
#     ogen noplot nonBox -order=4 -nx=20 
#     ogen noplot nonBox -order=4 -nx=40 
#     ogen noplot nonBox -order=4 -nx=80 
#     ogen noplot nonBox -order=4 -nx=160
#
#     ogen noplot nonBox -order=4 -nx=8
#     ogen noplot nonBox -order=4 -nx=16
#     ogen noplot nonBox -order=4 -nx=32
#     ogen noplot nonBox -order=4 -nx=64
#     ogen noplot nonBox -order=4 -nx=128
#     ogen noplot nonBox -order=4 -nx=256
# 
$order=2; $nx=10; # default values
$orderOfAccuracy = "second order"; $ng=2; $bc="d"; 
$numGhost=-1;  # if this value is set, then use this number of ghost points
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"nx=i"=> \$nx,"bc=s"=> \$bc,"numGhost=i"=>\$numGhost);
$nx=$nx+1; 
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
# 
$lines = $nx;
$nge = $ng+1;
$suffix = ".order$order"; 
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
if( $numGhost ne -1 ){ $ng = $numGhost; $nge=$ng; } # overide number of ghost
if( $bc ne "d" ){ $suffix .= $bc; } # periodic
# 
$cells = $lines -1;
$name = "nonBox" . "$cells" . $suffix;
#
create mappings
  Box
    lines
      $lines $lines $lines
    boundary conditions
      if( $bc eq "d" ){ $cmd = "1 2 3 4 5 6"; }elsif( $bc eq "pdd" ){ $cmd="-1 -1 3 4 5 6"; }else{ $cmd="-1 -1 -1 -1 -1 -1"; }
      $cmd
    mappingName
      box-analytic
  exit
  rotate/scale/shift
    mappingName
    box
  exit
exit
#
generate an overlapping grid
  box
  done
  change parameters
    ghost points
      all
      $ng $ng $ng $ng $ng $nge
    order of accuracy
      $orderOfAccuracy
  exit
  compute overlap
  display computed geometry
exit
#
save an overlapping grid
  $name.hdf
  $name
exit

