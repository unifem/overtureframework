#
# make a grid for a box
# usage: ogen [noplot] box -nx=<num> -order=[2/4/6/8] -bc=[d|p]
#   -bc : d=dirichlet, p=periodic boundary conditions
# 
# Examples:
#
#     ogen noplot box -nx=5 
#     ogen noplot box -nx=10
#     ogen noplot box -nx=20 
#     ogen noplot box -nx=40 
#
#     ogen noplot box -bc=p -nx=10 
#     ogen noplot box -bc=p -nx=20 
# 
#     ogen noplot box -order=2 -nx=10 
#     ogen noplot box -order=2 -nx=20 
#     ogen noplot box -order=2 -nx=40 
#     ogen noplot box -order=2 -nx=80 
#     ogen noplot box -order=2 -nx=160
#
#     ogen noplot box -order=4 -nx=5 
#     ogen noplot box -order=4 -nx=10
#     ogen noplot box -order=4 -nx=20 
#     ogen noplot box -order=4 -nx=40 
#     ogen noplot box -order=4 -nx=80 
#     ogen noplot box -order=4 -nx=160
#
#     ogen noplot box -order=2 -nx=8
#     ogen noplot box -order=2 -nx=16
#     ogen noplot box -order=2 -nx=32
#     ogen noplot box -order=2 -nx=64
#     ogen noplot box -order=2 -nx=256
#
#     ogen noplot box -order=4 -nx=8
#     ogen noplot box -order=4 -nx=16
#     ogen noplot box -order=4 -nx=32
#     ogen noplot box -order=4 -nx=64
#     ogen noplot box -order=4 -nx=128
#     ogen noplot box -order=4 -nx=256
# 
# -- save current values of parameters so this script can be called by CG scripts
$orderBox=$order; $orderOfAccuracyBox=$orderOfAccuracy; $nxBox=$nx; 
#
$order=2; $nx=11; # default values
$orderOfAccuracy = "second order"; $ng=2; $bc="d"; 
# 
# get command line arguments
Getopt::Long::Configure("prefix_pattern=(--box|--|-)");
GetOptions( "order=i"=>\$order,"nx=i"=> \$n,"bc=s"=> \$bc);
$nx=$n+1;
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
# 
$lines = $nx;
$suffix = ".order$order"; 
if( $bc eq "p" ){ $suffix .= "p"; } # periodic
# 
$cells = $lines -1;
$name = "box" . "$cells" . $suffix;
$nge = $ng+1;
#
create mappings
  Box
    lines
      $lines $lines $lines
    boundary conditions
      if( $bc eq "d" ){ $cmd = "1 2 3 4 5 6"; }else{ $cmd="-1 -1 -1 -1 -1 -1"; }
      $cmd
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
# reset variables
$order=$orderBox; $orderOfAccuracy=$orderOfAccuracyBox; $nx=$nxBox; 
exit
