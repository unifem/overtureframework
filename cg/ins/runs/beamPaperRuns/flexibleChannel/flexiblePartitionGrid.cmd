#
# Grid for a flexible partition between two fluid domains -- e.g. cgins + beam model
#
#
#   yd  ---------------------------------------------
#       |                                           |
#       |                                           |
#       |                fluid                      |
#       |                                           |
#   yc  ---------------------------------------------
#       |                beam                       |
#   yb  ---------------------------------------------
#       |                                           |
#       |                fluid                      |
#       |                                           |
#       |                                           |
#   ya  ---------------------------------------------
#       xa                                         xb 
#
# usage: ogen [noplot] flexiblePartitionGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i] -per=[0|1] ...
#                                         -option=[middle|top|bottom] -extraTop=[i]
# Options:
#   -option : middle =beam is in the middle betwee two-fluid domains, 
#             top    =beam is on top of one fluid domain, 
#             bottom =beam is on the bottom of a fluid domain
#   -per = 0 = no-periodic in x,  1=periodic in x
#   -extraTop=n -- add n grid points in the x-direction to the grids on top
# 
# examples:
#     ogen -noplot flexiblePartitionGrid -interp=e -factor=1
#     ogen -noplot flexiblePartitionGrid -interp=e -factor=2 
#     ogen -noplot flexiblePartitionGrid -interp=e -factor=4
#     ogen -noplot flexiblePartitionGrid -interp=e -factor=8 
#     ogen -noplot flexiblePartitionGrid -interp=e -factor=16
#     ogen -noplot flexiblePartitionGrid -interp=e -factor=32
#
#  - beam on top
#     ogen -noplot flexiblePartitionGrid -option=top -interp=e -factor=1
#     ogen -noplot flexiblePartitionGrid -option=top -interp=e -factor=2 
#     ogen -noplot flexiblePartitionGrid -option=top -interp=e -factor=4
#     ogen -noplot flexiblePartitionGrid -option=top -interp=e -factor=8 
#  -- very thin beam on top:
#     ogen -noplot flexiblePartitionGrid -option=top -interp=e -thickness=1.e-6 -prefix="thinBeamOnTop" -factor=1
# 
#    -- coarse grid in x
#     ogen -noplot flexiblePartitionGrid -option=top -interp=e -dsx=.5 -factor=1
# -- periodic in x:
#    ogen -noplot flexiblePartitionGrid -factor=4 -per=1
#
# - change number of points on top for testing AMP scheme
#    ogen -noplot flexiblePartitionGrid -interp=e -factor=1 -extraTop=5
#
$order=2; $factor=1; $extraTop=0; $interp="i";  # default values
$orderOfAccuracy = "second order"; $ng=2; 
$name=""; $t=0; $per=0; $option="middle"; 
$thickness=.1; # beam thickness
$heightLower=.5; $heightUpper=.5; 
$xa=-1.; $xb=1.;
$dsx=-1.; 
$prefix=""; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "t=f"=> \$t,"interp=s"=> \$interp,"name=s"=> \$name,"option=s"=> \$option,"per=i"=>\$per,\
            "dsx=f"=> \$dsx,"thickness=f"=> \$thickness,"extraTop=i"=>\$extraTop,"prefix=s"=> \$prefix );
#
$thb2=$thickness*.5; 
$ya=-$heightLower-$thb2; $yb=-$thb2; $yc=$thb2; $yd=$heightUpper+$thb2; 
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $per eq 1 ){ $suffix .= "p"; }
if( ($prefix eq "") && ($option eq "top") ){ $prefix="beamOnTopGrid"; }
if( ($prefix eq "") && ($option eq "bottom") ){ $prefix="beamOnBottomGrid"; }
if( $prefix eq "" ){ $prefix="flexiblePartitionGrid"; }
if( $dsx > 0. ){ $prefix .= ".dsx"; }
if( $extraTop > 0 ){ $prefix .= "ExtraTop$extraTop"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
if( $dsx<0 ){ $dsx=$ds; }
# 
$dw = $order+1; $iw=$order+1; 
#
$bcInterface=100;  # bc for interfaces
$shareInterface=100;        # share value for interfaces
#
#
create mappings
#
# --- build a spline for the lower boundary of the beam
# 
$degree=3;
$nx = int( ($xb-$xa)/$dsx +1.5 ); 
$n=$nx; $h=($xb-$xa)/($n-1);
$cmd0="";
for($i=0; $i<$n; $i++){$x=$xa+$i*$h; $y=$yb; $cmd0=$cmd0 . "$x $y\n";}
# 
  nurbs (curve)
    parameterize by index (uniform)
    enter points
      $n $degree
    $cmd0
    lines
      $nx
    mappingName
      beamBottom
    exit
#
# fluid interface grid next to the bottom of the beam
#
  hyperbolic
    Start curve:beamBottom
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
    name lowerFluidInterface
    # open graphics
  exit
# Background grid for the lower fluid
 rectangle
  set corners
    $yb0=$yb+.5*($yb-$ya); # heighten background to allow for surface motion
    $xa $xb $ya $yb0 
  lines
    $nx = int( ($xb -$xa)/$dsx +1.5 ); 
    $ny = int( ($yb0-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
   if( $per eq 0 ){ $cmd="1 2 3 0 "; }else{ $cmd="-1 -1 3 0"; }
   $cmd
  share 
    1 2 3 0 
  mappingName
    lowerFluidBackGround
  exit
#
#
# --- build a spline for the upper boundary of the beam
# 
$degree=3;
$nx = int( ($xb-$xa)/$dsx +1.5 + $extraTop ); 
$n=$nx; $h=($xb-$xa)/($n-1);
$cmd0="";
for($i=0; $i<$n; $i++){$x=$xa+$i*$h; $y=$yc; $cmd0=$cmd0 . "$x $y\n";}
  nurbs (curve)
    parameterize by index (uniform)
    enter points
      $n $degree
    $cmd0
    lines
      $nx
    mappingName
      beamTop
    exit
#
# fluid interface grid next to the upper surface of the beam
#
  hyperbolic
    Start curve:beamTop
    $nr =5; 
    $dist= $ds*($nr-1);
    backward
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
    name upperFluidInterface
    # open graphics
  exit
# Background grid for the upper fluid
 rectangle
  set corners
    $yc0=$yc-.5*($yd-$yc); # lower bottom to allow for surface motion
    $xa $xb $yc0 $yd 
  lines
    $nx = int( ($xb -$xa )/$dsx +1.5  + $extraTop); 
    $ny = int( ($yd -$yc0)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
   if( $per eq 0 ){ $cmd="1 2 0 4 "; }else{ $cmd="-1 -1 0 4"; }
   $cmd
  share 
    1 2 0 4
  mappingName
    upperFluidBackGround
  exit
#
exit this menu
#
generate an overlapping grid
  if( $option eq "middle" ){ $grids="lowerFluidBackGround\n upperFluidBackGround\n lowerFluidInterface\n upperFluidInterface"; }
  if( $option eq "top"    ){ $grids="lowerFluidBackGround\n lowerFluidInterface"; }
  if( $option eq "bottom" ){ $grids="upperFluidBackGround\n upperFluidInterface"; }
  $grids 
  done choosing mappings
#
  change parameters
    # choose implicit or explicit interpolation
    interpolation type
      $interpType
#     specify a domain
#       lowerDomain
#       lowerFluidBackGround
#       lowerFluidInterface
#       done
#     specify a domain
#       upperDomain
#       upperFluidBackGround
#       upperFluidInterface
#       done
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
flexiblePartitionGrid
exit

