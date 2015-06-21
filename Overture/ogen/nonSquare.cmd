# make a square that is not considered to be rectangular in order
# to build a simple test for codes that apply different methods for
# rectangular and curvilinear grids
#
# ogen noplot nonSquare -n=5
# ogen noplot nonSquare -n=10
# ogen noplot nonSquare -n=20
# ogen noplot nonSquare -n=40
# 
# ogen noplot nonSquare -n=16
# ogen noplot nonSquare -n=32
# ogen noplot nonSquare -n=64
# ogen noplot nonSquare -n=128
# ogen noplot nonSquare -n=256
# 
# ogen noplot nonSquare -order=4 -n=10
# ogen noplot nonSquare -order=4 -n=20
# ogen noplot nonSquare -order=4 -n=40
# ogen noplot nonSquare -order=4 -n=80
#
# ogen noplot nonSquare -order=4 -n=8
# ogen noplot nonSquare -order=4 -n=16
# ogen noplot nonSquare -order=4 -n=20
# ogen noplot nonSquare -order=4 -n=32
# ogen noplot nonSquare -order=4 -n=64
# ogen noplot nonSquare -order=4 -n=128
# ogen noplot nonSquare -order=4 -n=256
# ogen noplot nonSquare -order=4 -n=512
# ogen noplot nonSquare -order=4 -n=1024
#
# -periodic
#  ogen noplot nonSquare -n=16 -order=4 -periodic=pn
# 
# 
$order=2; $n=10; $periodic=""; # default values
$orderOfAccuracy = "second order"; $ng=2;
$numGhost=-1;  # if this value is set, then use this number of ghost points
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"n=i"=> \$n,"periodic=s"=>\$periodic,"numGhost=i"=> \$numGhost );
$nx=$n+1;
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
# 
$lines = $nx;
suffix= "";
if( $periodic eq "p" ){ $suffix = "p"; }
if( $periodic eq "np" ){ $suffix = "np"; }
if( $periodic eq "pn" ){ $suffix = "pn"; }
$suffix .= ".order$order"; 
if( $numGhost ne -1 ){ $ng = $numGhost; } # overide number of ghost
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
$name = "nonSquare" . "$n" . $suffix;
#
# 
create mappings
  rectangle
    mappingName
      square
    set corners
      0. 1. 0. 1.
    lines
      $nx $nx 
    boundary conditions
     if( $periodic eq "p" ){ $bc ="-1 -1 -1 -1"; }\
     elsif( $periodic eq "np" ){ $bc ="1 2 -1 -1"; }\
     elsif( $periodic eq "pn" ){ $bc ="-1 -1 3 4"; }else{ $bc="1 2 3 4"; }
     $bc
    mappingName
     rectangularSquare
  exit
  rotate/scale/shift
    mappingName
    square
    exit
exit
#
generate an overlapping grid
  square
  done
  change parameters
    ghost points
      all
      $ngp = $ng+1;
      $ng $ng $ng $ngp $ng $ng
    order of accuracy
      $orderOfAccuracy
  exit
  compute overlap
exit
#
# save an overlapping grid
save a grid (compressed)
  $name.hdf
  nonSquare
exit
