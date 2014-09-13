#
# Make a cylinder in a channel 
#   ogen [-noplot] cylinderInAChannel -factor=<> -interp=[e|i] -order=[2|4|6|8] -zper=[0|1] -blf=<>
#
# -zper : 1= periodic in z-direction (axial direction of the cylinder)
# -blf  : factor to decrease the boundary layer grid spacing by
#
# Examples:
#  ogen -noplot cylinderInAChannel -factor=1
#  ogen -noplot cylinderInAChannel -factor=2
#
#  ogen -noplot cylinderInAChannel -zper=1 -factor=1       [ periodic
#  ogen -noplot cylinderInAChannel -zper=1 -factor=1 -ml=2 [ periodic
#
# - fourth-order
#  ogen -noplot cylinderInAChannel -order=4 -factor=2
#  ogen -noplot cylinderInAChannel -zper=1 -order=4 -factor=1 -ml=2 [ periodic
#
#  ogen -noplot cylinderInAChannel -order=4 -factor=2 -ml=2
# 
# - short channel and narrow cylinder for testing:
#  ogen -noplot cylinderInAChannel -order=2 -factor=1 -xb=1. -za=-0.075 -zb=.075 -name=cylinderInAShortChannele1.order2.hdf
#
#  ogen -noplot cylinderInAChannel -order=4 -factor=2 -xb=1. -za=-0.075 -zb=.075 -name=cylinderInAShortChannele2.order4.hdf
#  -- MG
#   ogen -noplot cylinderInAChannel -order=4 -factor=1 -ml=1 
#   ogen -noplot cylinderInAChannel -order=4 -factor=2 -ml=2 -blf=5.
#   ogen -noplot cylinderInAChannel -order=4 -factor=4 -ml=3 -blf=8.  [9M pts
#   ogen -noplot cylinderInAChannel -order=4 -factor=8 -ml=3 -blf=10.
#
#  ogen -noplot cylinderInAChannel -order=2 -factor=1 -ml=2 -xb=1. -za=-0.075 -zb=.075 -name=cylinderInAShortChannele1.order2.ml2.hdf
#  ogen -noplot cylinderInAChannel -order=2 -factor=2 -ml=2 -xb=1. -za=-0.075 -zb=.075 -name=cylinderInAShortChannele2.order2.ml2.hdf
# 
#  ogen -noplot cylinderInAChannel -order=4 -factor=1 -ml=2 -xb=1. -za=-0.2 -zb=.2 -name=cylinderInAShortChannele1.order4.ml2.hdf
#  ogen -noplot cylinderInAChannel -order=4 -factor=2 -ml=2 -xb=1. -za=-0.075 -zb=.075 -name=cylinderInAShortChannele2.order4.ml2.hdf
#
# -- OLD:
# $factor=1; $gridName = "cylinderInAChannel$factor.hdf"; 
# $factor=2; $gridName = "cylinderInAChannel$factor.hdf"; 
# $factor=4; $gridName = "cylinderInAChannel$factor.hdf"; 
#
# $gridName = "cylinderInAShortChannel1.hdf"; $factor=1.; $xa=-.75; $xb=.75; $ya=-.75; $yb=.75; $za=-.075; $zb=.075; $deltaRadius=.25; 
# $gridName = "cylinderInAShortChannel2.hdf"; $factor=2.; $xa=-.75; $xb=.75; $ya=-.75; $yb=.75; $za=-.075; $zb=.075; $deltaRadius=.25; 
# 
# $gridName = "cylinderInAShortChannel.order4.hdf"; $factor=1.75; $order="fourth order"; $xa=-.75; $xb=.75; $ya=-.75; $yb=.75; $za=-.075; $zb=.075; $deltaRadius=.5; 
# $gridName = "cylinderInAChannel2.order4.hdf"; $factor=2.; $order="fourth order"; $za=-.075; $zb=.075; $deltaRadius=.5;
# 
#
$order=2; $factor=1; $interp="e";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-1.; $xb=1.; $ya=-1.; $yb=1.;
$xa=-1.; $xb=3.; $ya=-1.; $yb=1.; $za=-1.; $zb=1.; $deltaRadius=.25; 
$zper=0; # 1= periodic in z direction
$blf=2.;  # 1 = no stretching, 2=boundary layer spacing is a factor of 2 times smaller.
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=>\$factor,"xa=f"=>\$xa,"xb=f"=> \$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "za=f"=>\$za,"zb=f"=>\$zb,"interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"zper=i"=>\$zper,"blf=f"=> \$blf,);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $zper ne 0 ){ $suffix .= "p"; }
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = "cylinderInAChannel" . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
# 
$dw = $order+1; $iw=$order+1; 
#
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
#
$ds= (1./20.)/$factor;
$pi= 4.*atan2(1.,1.);
#
create mappings
  Box
    mappingName
      box
    set corners
      $xa $xb $ya $yb $za $zb 
    lines
      $nx = intmg( ($xb-$xa)/$ds +1.5 );
      $ny = intmg( ($yb-$ya)/$ds +1.5 );
      $nz = intmg( ($zb-$za)/$ds +1.5 );
#
      $nx $ny $nz 
    boundary conditions
      if( $zper eq 0 ){ $bc = "1 2 3 4 5 6"; }else{ $bc="1 2 3 4 -1 -1"; }
      $bc
    share
      0 0 0 0 1 2
  exit
#
  Cylinder
    mappingName
      unstretched-cylinder
    bounds on the radial variable
      $nr = intmg( 7 + $order ); # number of lines in the radial direction
      $radius = ($nr-1)*$ds; 
      $ra=.25; $rb=$ra+$radius;
      $ra $rb 
    bounds on the axial variable
      $za $zb  
    lines
      $nTheta = intmg( 2.*$pi*($ra+$rb)/2./$ds +1.5 );
      $nz = intmg( ($zb-$za)/$ds +1.5 );
      $nr = intmg( ($rb-$ra)/$ds +1.5 ); # 7; 
# 
      $nTheta $nz $nr 
    boundary conditions
     if( $zper eq 0 ){ $bc = "-1 -1 5 6 7 0"; }else{ $bc="-1 -1 -1 -1 7 0"; }
     $bc
    share
      0  0 1 2 0 0
  exit
#
  stretch coordinates
    Stretch r3:exp to linear
    STRT:multigrid levels $ml
    $dsMin=$ds/$blf; # normal grid spacing on the cylinder
    STP:stretch r3 expl: min dx, max dx $dsMin $ds
    STRT:name cylinder
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
 # choose implicit or explicit interpolation
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
# pause
  exit
#
save an overlapping grid
$name
cylinderInAChannel
exit


