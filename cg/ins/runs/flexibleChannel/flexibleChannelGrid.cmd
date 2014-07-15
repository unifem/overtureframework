#
# Grid for a flexible channel -- e.g. cgins + beam model
#
#   yb  ---------------------------------------------
#       |                                           |
#       |                fluid                      |
#       |                                           |
#       |                                           |
#   ya  ---------------------------------------------
#       xa                                         xb 
#
# usage: ogen [noplot] flexibleChannel -factor=<num> -order=[2/4/6/8] -interp=[e/i] -per=[0|1]
# Options:
#   -per = 0 = no-periodic in x,  1=periodic in x
# 
# examples:
#     ogen -noplot flexibleChannelGrid -factor=4 
#     ogen -noplot flexibleChannelGrid -factor=8 
#     ogen -noplot flexibleChannelGrid -factor=16
#     ogen -noplot flexibleChannelGrid -factor=32
#
# -- periodic in x:
#    ogen -noplot flexibleChannelGrid -factor=4 -per=1
#
$order=2; $factor=1; $interp="e"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $t=0; $per=0; 
$xa=0.; $xb=6.; $ya=0.; $yb=.5; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "t=f"=> \$t,"interp=s"=> \$interp,"name=s"=> \$name,"per=i"=>\$per );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $per eq 1 ){ $suffix .= "p"; }
if( $name eq "" ){$name = "flexibleChannelGrid" . "$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
# 
$dw = $order+1; $iw=$order+1; 
#
$bcInterface=100;  # bc for interfaces
$shareInterface=100;        # share value for interfaces
#
$nx = int( ($xb-$xa)/$ds +1.5 ); 
#
$degree=3;
$n=$nx;      # *** For now this much match the number of grid points
$h=($xb-$a)/($n-1);
#
$cmd0="";
$cmd1="";
for($i=0; $i<$n; $i++){$x=$i*$h; $y=$yb; $cmd0=$cmd0 . "$x $y\n";}
#
create mappings
  $ny = int( ($height)/$ds +1.5 ); 
  nurbs (curve)
    parameterize by index (uniform)
    enter points
    $n $degree
    $cmd0
    lines
      $nx
    mappingName
      topBoundary
    exit
#
# Interface grid for the fluid
#
  hyperbolic
    Start curve:topBoundary
    $nr =5; 
    $dist= $ds*($nr-1);
    forward
    distance to march $dist
    lines to march $nr
    points on initial curve $nx
    BC: left fix x, float y and z
    BC: right fix x, float y and z
    generate
    # use fourth order interpolant to define the mapping:
    # fourth order
    boundary conditions
      if( $per eq 0 ){ $cmd="1 2 $bcInterface 0"; }else{ $cmd="-1 -1 $bcInterface 0"; }
      $cmd
    share
     1 2 $shareInterface 0
    name fluidInterface
    # open graphics
  exit
# Background grid for the fluid
 rectangle
  set corners
    $xa $xb $ya $yb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5 ); 
    $ny = int( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
   if( $per eq 0 ){ $cmd="1 2 3 0 "; }else{ $cmd="-1 -1 3 0"; }
   $cmd
  share 
    1 2 3 0 
  mappingName
    fluidBackGround
  exit
exit this menu
#
generate an overlapping grid
  fluidBackGround
  fluidInterface
  done choosing mappings
  change parameters
    ghost points
      all
      2 2 2 2 2 2
    exit
  # open graphics
  compute overlap
exit
#
# save an overlapping grid
save a grid (compressed)
$name
flexibleChannelGrid
exit

