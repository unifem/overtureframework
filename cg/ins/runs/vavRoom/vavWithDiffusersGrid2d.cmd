#
# Grid for a 2d cross section of 3D VAV room with diffusers : Purdue's VAV room
#
# usage: ogen [noplot] vavWithDiffusersGrid2d -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<> -clouds=[0|1]
# 
# -clouds=1 :  add cloud grids
#
# NOTE: watch out at inlet/outlet : the background grid may retain a short section of wall where it shouldn't
#       if there is not enough overlap
#
# examples:
# 
# -- second-order
#  ogen -noplot vavWithDiffusersGrid2d -interp=e -factor=1 -ml=1 
#  ogen -noplot vavWithDiffusersGrid2d -interp=e -factor=2 -ml=1
#  ogen -noplot vavWithDiffusersGrid2d -interp=e -factor=4 -ml=2
#  ogen -noplot vavWithDiffusersGrid2d -interp=e -factor=8 -ml=2
#
# -- fourth-order
#  ogen -noplot vavWithDiffusersGrid2d -interp=e -order=4 -factor=1 -ml=1 
#  ogen -noplot vavWithDiffusersGrid2d -interp=e -order=4 -factor=2 -ml=1
#  ogen -noplot vavWithDiffusersGrid2d -interp=e -order=4 -factor=4 -ml=2
#  ogen -noplot vavWithDiffusersGrid2d -interp=e -order=4 -factor=8 -ml=3
#
# 
#
$order=2; $factor=1; $interp = "i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$name=""; $xa=-1.; $xb=1.; $ya=-1.; $yb=1.;		
$clouds=0; 
# 
$f2m = .3048; # feet to meters conversion (exact)
# $f2m = 1.; # work in units of feet
#
# -- Room dimensions:
# 
$xaRoom=0.; $xbRoom=32.*$f2m; $yaRoom=0.; $ybRoom=14.5*$f2m; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"clouds=i"=>\$clouds);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
# 
$prefix ="vavWithDiffusersGrid2d";
if( $clouds eq 1 ){ $prefix .= "WithClouds"; }
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds0=.5*$f2m;
$ds=$ds0/$factor;
$pi = 4.*atan2(1.,1.);
# 
$dw = $order+1; $iw=$order+1; 
#
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
sub min{ local($n,$m)=@_; if( $n<$m ){ return $n; }else{ return $m; } }
#
$Pi = 4.*atan2(1.,1.);
#
#
#
#
create mappings
#
$bcOutflow=12;   # outflow BC
#
#  Main room grid:
#
rectangle
  $xad=$xaRoom; $xbd=$xbRoom; $yad=$yaRoom; $ybd=$ybRoom;
  set corners
    $xad $xbd $yad $ybd
  lines
    $nx = intmg( ($xbd-$xad)/$ds +1.5 ); 
    $ny = intmg( ($ybd-$yad)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 2 3 4
  share
    1 2 3 4	 
  mappingName
    roomBackGround
  exit
#
#  Outlet grid (poor man's)
#
rectangle
  $nyOutlet = intmg( 9 ); # outlet grid extends this far into the room
#
#      # $xao=8.*$f2m; $xbo=12.*$f2m; $zao=8.*$f2m; $zbo=12.*$f2m; $yao=$ybRoom-$nyOutlet*$ds; $ybo=$ybRoom; 
#  $xao=12.5*$f2m; $xbo=16.*$f2m; $zao=11.*$f2m; $zbo=12.5*$f2m; $yao=$ybRoom-$nyOutlet*$ds; $ybo=$ybRoom; 
#
# -- For 2D
  $xao=$xbRoom-$nyOutlet*$ds; $xbo=$xbRoom; $yao=1.5*$f2m; $ybo=4.5*$f2m;
#
  set corners
    $xao $xbo $yao $ybo
  lines
    $nx = intmg( ($xbo-$xao)/$ds +1.5 ); 
    $ny = intmg( ($ybo-$yao)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    0 $bcOutflow 0 0 
  share
    0 4 0 0 
  mappingName
    outlet
  exit
# Here is the diffuser -- use an annulus
  $xWidthd=2.*$f2m;  $yWidthd=2.*$f2m;  # width of diffusers
  $xa=8.*$f2m; $xb=$xa+$xWidthd; $ya=9.*$f2m; $yb=9.5*$f2m; 
 $nr = intmg( 7 );
Annulus
  $innerRad=$xWidthd*.5; $outerRad = $innerRad + ($nr-1)*$ds;
  $xShift = .2*$f2m;  # shift diffuser away from the end a bit 
  $cx = .5*($xa+$xb) - $xShift; $cy=$ya+$xWidthd*.5; 
  center: $cx $cy
  inner and outer radii
    $innerRad $outerRad
  lines
    $nTheta = intmg( 2.*$pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 10 0
  share
     0  0 10 0
  mappingName
   diffuser1
 exit
# -- diffuser 2 
Annulus
  $cx = $cx + 14.*$f2m + 2.*$xShift;
  center: $cx $cy
  inner and outer radii
    $innerRad $outerRad
  lines
    $nTheta $nr
  boundary conditions
    -1 -1 11 0
  share
     0  0 11 0
  mappingName
   diffuser2
 exit
#
exit
#
#
# Make the overlapping grid
#
generate an overlapping grid
  roomBackGround
  outlet
  diffuser1
  diffuser2
#
  done
  plot
  change parameters
 # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng 
  exit
# open graphics
  compute overlap
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
vavWithDiffusersGrid2d
exit

