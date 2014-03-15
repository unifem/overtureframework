#**************************************************************************
#
#  Grid for a knife edge
# usage: ogen [noplot] knifeEdge -factor=<num> -order=[2/4/6/8] -interp=[e/i] -yTop=<> -name=<>
#
#  ogen -noplot knifeEdge -interp=e -factor=2 -order=4 -yTop=1.1 -name="knifeSlit2.order4.hdf"
#  ogen -noplot knifeEdge -interp=e -factor=2 -order=4 -yTop=1.1 -name="knifeSlit4.order4.hdf"
#
#**************************************************************************
## OLD: 
## $orderOfAccuracy = "second order";
## * scale number of grid points in each direction by the following factor
## * $factor=1; $name = "knifeEdge.hdf";
## * $factor=2; $name = "knifeEdge2.hdf"; 
## * $factor=4; $name = "knifeEdge4.hdf"; 
## * $factor=8; $name = "knifeEdge8.hdf"; 
## * $factor=16; $name = "knifeEdge16.hdf"; 
## *
## * -- fourth-order accurate ---
## * $factor=2; $name = "knifeEdge2.order4.hdf";  $orderOfAccuracy = "fourth order";
## * $factor=4; $name = "knifeEdge4.order4.hdf";  $orderOfAccuracy = "fourth order";
## * $factor=8; $name = "knifeEdge8.order4.hdf";  $orderOfAccuracy = "fourth order";
## *
## *  ---- 4th-order "slit" (has a narrower gap at the top) -----
## $factor=2; $name = "knifeSlit2.order4.hdf";  $orderOfAccuracy = "fourth order"; $yTop=1.1; 
## ** $factor=4; $name = "knifeSlit4.order4.hdf";  $orderOfAccuracy = "fourth order"; $yTop=1.1; 
#
$yTop=2.; 
$prefix="knifeEdge"; 
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; 
#
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"yTop=f"=> \$yTop );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# printf(" factor=$factor\n");
#
# Define a subroutine to convert the number of grid points
sub getGridPoints\
{ local($n1,$n2)=@_; \
  $nx=int(($n1-1)*$factor+1.5); $ny=int(($n2-1)*$factor+1.5); \
}
#
#**************************************************************************
#
#
#  yb  X------------------------------X
#      |                              |
#      |            ye2               |
#      |            /\                |
#      |        ye1|  |               |
#      |           |  |               |
#      |           |  |               |
#      |           |  |               |
#      |           |  |               |
#      |           |  |               |
#      |           |  |               |
#   ya X------------  ----------------X
#      xa        xe1  xe2            xb
#*
# domain parameters:  
#
# bigger domain
   $xa=-1.00; $xb=1.0; 
   $ya= 0.;   $yb=$yTop; 
   $xe1=-.05; $xe2=.05; 
   $ye1=.90;  $ye2=1.; 
#
#
create mappings
#
# here is the bottom boundary
#
  smoothedPolygon
    $dn = -.05/$factor; # normal distance for boundary grids
    $xem=($xe1+$xe2)*.5;
    vertices
    5
    $xe2 $ya
    $xe2 $ye1
    $xem $ye2
    $xe1 $ye1
    $xe1 $ya
    n-dist
    fixed normal distance
      $dn 
    t-stretch
      0 50
      .1 5
      .1 5
      .1 5
      0 50
    n-stretch
      1 3 0
    sharpness
      20.
      20.
      20.
      20.
      20.
    lines
      $nx0=int(($ye2-$ya)*70*2+1.5);
      $nyn=7; # fix lines in normal direction
      getGridPoints($nx0,$nyn);
      $nx $nyn
    boundary conditions
      1 2 3 0
    correct corners
    share
      1 1 0 0
    mappingName
      knife
# pause
    exit
#
#
#  here is the background grid
#
  rectangle
    set corners
     $xa $xb $ya $yb 
    lines
      $nx0=int( ($xb-$xa)*95+1.5 );
      $ny0=int( ($yb-$ya)*95+1.5 );
      getGridPoints($nx0,$ny0);
      $nx $ny
    boundary conditions
      1 2 3 4
    share
      0 0 1 0
    mappingName
      backGround
  exit
#
  exit this menu
#
generate an overlapping grid
  backGround
  knife
  change parameters
    ghost points
      all
      2 2 2 2 2 2
    order of accuracy
     $orderOfAccuracy
  exit
# pause
  compute overlap
# pause
  exit
#
save an overlapping grid
  $name
  knifeEdge
exit
