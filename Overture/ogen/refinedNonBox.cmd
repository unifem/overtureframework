#
# make a grid for a refinedNonBox
# usage: ogen [noplot] refinedNonBox -nx=<num> -order=[2/4/6/8] -bc=[d|p]
#   -bc : d=dirichlet, p=periodic boundary conditions
# 
# Examples:
#
#     ogen noplot refinedNonBox -nx=10 
#     ogen noplot refinedNonBox -nx=20 
#     ogen noplot refinedNonBox -nx=40 
#
#     ogen noplot refinedNonBox -order=2 -nx=10 
#     ogen noplot refinedNonBox -order=2 -nx=20 
#     ogen noplot refinedNonBox -order=2 -nx=40 
#     ogen noplot refinedNonBox -order=2 -nx=80 
#     ogen noplot refinedNonBox -order=2 -nx=160
#
#     ogen noplot refinedNonBox -order=4 -nx=10 
#     ogen noplot refinedNonBox -order=4 -nx=20 
#     ogen noplot refinedNonBox -order=4 -nx=40 
#     ogen noplot refinedNonBox -order=4 -nx=80 
#     ogen noplot refinedNonBox -order=4 -nx=160
#
# 
$order=2; $nx=10; # default values
$orderOfAccuracy = "second order"; $ng=2; $bc="d"; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"nx=i"=> \$nx,"bc=s"=> \$bc);
$nx=$nx+1; 
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
$name = "refinedNonBox" . "$cells" . $suffix;
$nge = $ng+1;
#
create mappings
  Box
    lines
      $lines $lines $lines
    boundary conditions
      if( $bc eq "d" ){ $cmd = "1 2 3 4 5 6"; }else{ $cmd="-1 -1 -1 -1 -1 -1"; }
      $cmd
    share 
      0 0 1 0 0 0 
    mappingName
      box-analytic
  exit
  rotate/scale/shift
    mappingName
    box
  exit
# refinement: 
  Box
    set corners
      .25 .75 .0 .5 .25 .75
    lines
      $lines $lines $lines
    boundary conditions
      0 0 1 0 0 0 
    share 
      0 0 1 0 0 0 
    mappingName
      refinedBox-analytic
  exit
  rotate/scale/shift
    mappingName
    refinedBox
  exit
exit
#
generate an overlapping grid
  box
  refinedBox
  done
  change parameters
    ghost points
      all
      $ng $ng $ng $ng $ng $nge
    order of accuracy
      $orderOfAccuracy
  exit
  compute overlap
#  display computed geometry
exit
#
save an overlapping grid
  $name.hdf
  $name
exit

