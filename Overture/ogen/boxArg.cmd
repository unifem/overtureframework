#
#  Create a box grid with varying size
#
# usage: ogen [-noplot] boxArg -factor=<num> -order=[2/4/6/8] -xa=<> -xb=<> -ya=<> -yb=<> -za=<> -zb=<> -ml=<> -periodic=[|p|npp|pnp|ppn] -name=<> 
#
#  xa,xb, ya,yb, za,zb : bounds on the box
#  ml = number of (extra) multigrid levels to support
# -p : periodic in all directions
# -npp, -pnp, -ppn : periodic or not in each direction
#
# examples:
#  ogen -noplot boxArg -order=2 -xa=-1. -xb=1. -ya=-1. -yb=1. -za=-1. -zb=1. -factor=1 -name="boxLx2Ly2Lz2Factor1.order2.hdf"
#  ogen -noplot boxArg -order=2 -xa=-1. -xb=1. -ya=-1. -yb=1. -za=-1. -zb=1. -factor=1 -name="boxLx2Ly2Lz2Factor2.order2.hdf"
# 
# -- fourth-order: 
#  ogen -noplot boxArg -order=4 -xa=-1. -xb=1. -ya=-1. -yb=1. -za=-1. -zb=1. -factor=2 -name="boxLx2Ly2Lz2Factor2.order4.hdf"
#  ogen -noplot boxArg -order=4 -xa=-1. -xb=1. -ya=-1. -yb=1. -za=-1. -zb=1. -factor=4 -name="boxLx2Ly2Lz2Factor4.order4.hdf"
# 
# -- periodic in one or more directions
#   ogen -noplot boxArg -order=2 -periodic=npp -factor=2  
#   ogen -noplot boxArg -order=2 -periodic=npp -factor=3
#   ogen -noplot boxArg -order=2 -periodic=npp -factor=4
#
#   ogen -noplot boxArg -order=2 -periodic=ppn -factor=2  
#
#   ogen -noplot boxArg -order=4 -periodic=npp -factor=5    [ 50^3 grid 
#   ogen -noplot boxArg -order=4 -periodic=pnp -factor=5    [ 50^3 grid 
#   ogen -noplot boxArg -order=4 -periodic=ppn -factor=5    [ 50^3 grid 
#   ogen -noplot boxArg -order=4 -periodic=npp -factor=10   [ 100^3 grid 
#   ogen -noplot boxArg -order=4 -periodic=pnp -factor=10   [ 100^3 grid 
#   ogen -noplot boxArg -order=4 -periodic=ppn -factor=10   [ 100^3 grid 
#
#   ogen -noplot boxArg -order=4 -periodic=p -factor=1  
#   ogen -noplot boxArg -order=4 -periodic=p -factor=2      [ 20^2
#   ogen -noplot boxArg -order=4 -periodic=p -factor=4  
#   ogen -noplot boxArg -order=4 -periodic=p -factor=8
# 
#  -- add more ghost points for sosup
#   ogen -noplot boxArg -numGhost=3 -order=4 -factor=2 
#
$xa=0.; $xb=1.; $ya=0.; $yb=1.; $za=0.; $zb=1.; $name=""; 
$order=2; $factor=1; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $periodic="";
$numGhost=-1;  # if this value is set, then use this number of ghost points
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"name=s"=>\$name,"ml=i"=>\$ml,"numGhost=i"=>\$numGhost,\
            "xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,"za=f"=>\$za,"zb=f"=>\$zb,"periodic=s"=>\$periodic );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
# 
$prefix="box";
if( $periodic eq "p" ){ $suffix = "p"; }
if( $periodic eq "npp" ){ $suffix = "npp"; }
if( $periodic eq "pnp" ){ $suffix = "pnp"; }
if( $periodic eq "ppn" ){ $suffix = "ppn"; }
if( $periodic eq "nnp" ){ $suffix = "nnp"; }
$suffix .= ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
if( $numGhost ne -1 ){ $ng = $numGhost; } # overide number of ghost
if( $name eq "" ){ $name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
# 
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
#
#
create mappings
#
# Create the box grid
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
    $bc ="1 2 3 4 5 6";
    if( $periodic eq "p" ){ $bc ="-1 -1 -1 -1 -1 -1"; }
    if( $periodic eq "npp" ){ $bc ="1 2 -1 -1 -1 -1"; }
    if( $periodic eq "pnp" ){ $bc ="-1 -1 3 4 -1 -1"; }
    if( $periodic eq "ppn" ){ $bc ="-1 -1 -1 -1 5 6"; }
    if( $periodic eq "nnp" ){ $bc ="1 2 3 4 -1 -1"; }
    $bc
  mappingName
    box
  exit
#**********************************
exit
#
generate an overlapping grid
  box
  done
# 
  change parameters
# 
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      if( $periodic eq "p" ){ $ngp=$ng+1; }else{ $ngp=$ng; }
      $ng $ng $ng $ng $ng $ngp
  exit
#
  compute overlap
#
exit
# save an overlapping grid
save a grid (compressed)
$name
box
exit
