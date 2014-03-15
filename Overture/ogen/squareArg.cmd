#
# make a grid for a square:
# usage: ogen [noplot] square -nx=<num> -order=[2/4/6/8] -periodic=[|p|np|pn] -numGhost=[]
# 
# -p : periodic in both directions
# -pn : periodic in direction 1, not periodic in direction 2
# -np : not periodic in direction 1, periodic in direction 2
# 
# examples:
#
#     ogen -noplot squareArg -nx=5
#     ogen -noplot squareArg -nx=10 
#     ogen -noplot squareArg -nx=20 
#     ogen -noplot squareArg -nx=40 
#     ogen -noplot squareArg -nx=80
#     ogen -noplot squareArg -nx=1024
#     ogen -noplot squareArg -nx=2048
# 
# Periodic:
#     ogen -noplot squareArg -periodic=p -nx=16 
#     ogen -noplot squareArg -periodic=p -nx=32 
#     ogen -noplot squareArg -periodic=p -nx=64 
#     ogen -noplot squareArg -periodic=p -nx=128
#
#     ogen -noplot squareArg -periodic=p  -order=4 -nx=32 
#     ogen -noplot squareArg -periodic=p  -order=6 -nx=32 
# 
#     ogen -noplot squareArg -periodic=p  -order=4 -nx=10
#     ogen -noplot squareArg -periodic=p  -order=4 -nx=20
#
# Semi-periodic
#     ogen -noplot squareArg -periodic=np -order=2 -nx=32
#     ogen -noplot squareArg -periodic=np -order=2 -nx=128
#     ogen -noplot squareArg -periodic=np -order=4 -nx=128
#
#     ogen -noplot squareArg -periodic=pn -order=4 -nx=32 -numGhost=3
#     ogen -noplot squareArg -periodic=pn -order=4 -nx=128
#
#     ogen -noplot squareArg -nx=9  
#     ogen -noplot squareArg -nx=17 
#     ogen -noplot squareArg -nx=33 
# 
#     ogen -noplot squareArg -nx=11 -order=2
#     ogen -noplot squareArg -nx=8  
#     ogen -noplot squareArg -nx=16
#     ogen -noplot squareArg -nx=32 
#     ogen -noplot squareArg -nx=64
#     ogen -noplot squareArg -nx=128
#     ogen -noplot squareArg -nx=256
# 
#     ogen -noplot squareArg -order=4 -nx=5
#     ogen -noplot squareArg -order=4 -nx=8
#     ogen -noplot squareArg -order=4 -nx=10 
#     ogen -noplot squareArg -order=4 -nx=16 
#     ogen -noplot squareArg -order=4 -nx=32 
#     ogen -noplot squareArg -order=4 -nx=64
#     ogen -noplot squareArg -order=4 -nx=128
#     ogen -noplot squareArg -order=4 -nx=256
#     ogen -noplot squareArg -order=4 -nx=512
#     ogen -noplot squareArg -order=4 -nx=1024    (2^10)
#     ogen -noplot squareArg -order=4 -nx=2048    (2^11)
#     ogen -noplot squareArg -order=4 -nx=4096    (2^12 = 16.8M)
#     ogen -noplot squareArg -order=4 -nx=8192    (2^13 = 67M)
#     srun -N1 -n8 -ppdebug $ogenp -noplot squareArg -order=4 -nx=16384   (2^14 = 268M
#     srun -N2 -n8 -ppdebug $ogenp -noplot squareArg -order=4 -nx=32768   (2^15 = 1.1B
#     srun -N8 -n16 -ppdebug $ogenp -noplot squareArg -order=4 -nx=65536  (2^16 = 4.3B
#     ogen -noplot squareArg -order=4 -nx=131072  (2^17 
#     ogen -noplot squareArg -order=4 -nx=262,144 (2^18
#     ogen -noplot squareArg -order=4 -nx=
# 
#     ogen -noplot squareArg -order=4 -nx=10 
#     ogen -noplot squareArg -order=4 -nx=20 
#     ogen -noplot squareArg -order=4 -nx=40 
#     ogen -noplot squareArg -order=4 -nx=80 
#     ogen -noplot squareArg -order=4 -nx=160
#     ogen -noplot squareArg -order=4 -nx=160
#
# add more ghost points:
# 
#     ogen -noplot squareArg -order=4 -nx=20 -numGhost=3
#     ogen -noplot squareArg -order=4 -nx=40 -numGhost=3
#     ogen -noplot squareArg -order=6 -nx=40 -numGhost=4
#
# see also the script buildSquares
#
$order=2; $n=10; # default values
$orderOfAccuracy = "second order"; $ng=2; $periodic=""; 
$numGhost=-1;  # if this value is set, then use this number of ghost points
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"nx=i"=> \$n,"periodic=s"=>\$periodic,"numGhost=i"=> \$numGhost );
$nx=$n+1; 
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
# 
$lines = $nx;
$suffix="";
if( $periodic eq "p" ){ $suffix = "p"; }
if( $periodic eq "np" ){ $suffix = "np"; }
if( $periodic eq "pn" ){ $suffix = "pn"; }
$suffix .= ".order$order"; 
if( $numGhost ne -1 ){ $ng = $numGhost; } # overide number of ghost
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
# 
# 
$cells = $lines -1;
$name = "square" . "$cells" . $suffix;
#
create mappings
  rectangle
    mappingName
      square
    lines
      $lines $lines
    boundary conditions
     if( $periodic eq "p" ){ $bc ="-1 -1 -1 -1"; }\
     elsif( $periodic eq "np" ){ $bc ="1 2 -1 -1"; }\
     elsif( $periodic eq "pn" ){ $bc ="-1 -1 3 4"; }else{ $bc="1 2 3 4"; }
     $bc
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
      if( $periodic eq "" ){ $ngp = $ng; } # do not do this for afs scheme for now
      $ng $ng $ng $ngp $ng $ng
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

