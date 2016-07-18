#
# Make a grid for a rectangular region used in the "slider" added-damping tests 
# 
#  For the "shear-block" exact solution you should choose the height of the grid
#  to satsify:
#              H = Pi - atan2(bodyMass,rho*L)
# 
# usage: ogen [noplot] slider -factor=<> -order=[2/4/6/8] -periodic=[|p|np|pn] -numGhost=[]
# 
#  ogen -noplot sliderGrid -factor=1
#  ogen -noplot sliderGrid -factor=2
#  ogen -noplot sliderGrid -factor=4
#  ogen -noplot sliderGrid -factor=8
#  ogen -noplot sliderGrid -factor=16
#  ogen -noplot sliderGrid -factor=32
#  ogen -noplot sliderGrid -factor=64
#
# -- shear block grid: -- 3 lines in the x-direction
#  ogen -noplot sliderGrid -prefix=shearBlockGrid -nx=3 -factor=1
#  ogen -noplot sliderGrid -prefix=shearBlockGrid -nx=3 -factor=2
#  ogen -noplot sliderGrid -prefix=shearBlockGrid -nx=3 -factor=4
#  ogen -noplot sliderGrid -prefix=shearBlockGrid -nx=3 -factor=8
#
# OLD*******
#    bodyMass=.01, rho*L=1 -> H=Pi - atan2(bodyMass,rho*L) = 3.1315929869031280
#  ogen -noplot sliderGrid -prefix=shearBlockGrid -bodyMass=.01 -ny=33
#  ogen -noplot sliderGrid -prefix=shearBlockGrid -bodyMass=.01 -ny=65
#  ogen -noplot sliderGrid -prefix=shearBlockGrid -bodyMass=.01 -ny=129
#  ogen -noplot sliderGrid -prefix=shearBlockGrid -bodyMass=.01 -ny=257
#
$prefix="sliderGrid"; $order=2; $factor=1; # default values
$orderOfAccuracy = "second order"; $ng=2; $periodic=""; 
$xa=0.; $xb=1.; $ya=0; $yb=1; 
$bodyMass=""; $ny=""; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"periodic=s"=>\$periodic,"prefix=s"=>\$prefix,\
            "bodyMass=f"=>\$bodyMass,"nx=i"=> \$nx,"ny=i"=> \$ny );
if( $ny ne "" ){ $factor=$ny-1; } 
if( $nx eq "" ){ $nx=$n+1; }
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
# 
$pi=4.*atan2(1.,1.);
## OLD if( $bodyMass ne "" ){ $length=1; $height=$pi-atan2($bodyMass,$length); $yb=$height; printf("height=$height\n"); }
# 
$suffix = ".order$order"; 
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
$dsx=.2; # coarse grid spacing in x-direction
# 
create mappings
 rectangle
  set corners
    $xa $xb $ya $yb
  lines
    if( $nx eq "" ){ $nx = int( ($xb-$xa)/$dsx +1.5 ); }
    if( $ny eq "" ){ $ny = int( ($yb-$ya)/$ds +1.5 ); }
    $nx $ny
  boundary conditions
    -1 -1 3 4
  share
     # share=100 marks the interface
     0  0 100 0
  mappingName
    slider
 exit
exit
#
generate an overlapping grid
  slider
  done
  change parameters
    ghost points
      all
      $ng $ng $ng $ng $ng $ng
    order of accuracy
      $orderOfAccuracy
  exit
  compute overlap
  # display computed geometry
exit
#
save an overlapping grid
  $name
  slider
exit

