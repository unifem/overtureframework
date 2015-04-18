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
#     ogen -noplot flexibleChannelGrid -interp=e -factor=2 
#     ogen -noplot flexibleChannelGrid -interp=e -factor=4
#     ogen -noplot flexibleChannelGrid -interp=e -factor=8 
#     ogen -noplot flexibleChannelGrid -interp=e -factor=16
#     ogen -noplot flexibleChannelGrid -interp=e -factor=32
#
# -- fixed width grid near surface 
#     ogen -noplot flexibleChannelGrid -interp=e -width=.1 -prefix=flexibleChannelGridFixed -factor=2 
#     ogen -noplot flexibleChannelGrid -interp=e -width=.1 -prefix=flexibleChannelGridFixed -factor=4 
#
# -- fixed width grid near surface, use fourth-order DPM interpolation 
#     ogen -noplot flexibleChannelGrid -interp=e -width=.1 -prefix=flexibleChannelGridDpm4Fixed -dpm=4 -factor=1
#     ogen -noplot flexibleChannelGrid -interp=e -width=.1 -prefix=flexibleChannelGridDpm4Fixed -dpm=4 -factor=2 
#
# -- grid stretching:
#    ogen -noplot flexibleChannelGrid -interp=e -width=.1 -stretch=1.1 -prefix=flexibleChannelGridStretched -factor=8
# 
# -- periodic in x:
#    ogen -noplot flexibleChannelGrid -factor=4 -per=1
#
# -- shorter channel for testing:
#    ogen -noplot flexibleChannelGrid -xb=1 -interp=e -factor=2  -name=shortFlexibleChannelGrid.hdf
#
$order=2; $factor=1; $interp="i";  # default values
$orderOfAccuracy = "second order"; $ng=2; 
$name=""; $t=0; $per=0; 
$xa=0.; $xb=6.; $ya=0.; $yb=.5;
$width=-1.;  $dpm=2; 
$stretch=-1; 
$prefix ="flexibleChannelGrid";
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "t=f"=> \$t,"interp=s"=> \$interp,"name=s"=> \$name,"per=i"=>\$per,"width=f"=>\$width,\
            "prefix=s"=> \$prefix,"stretch=f"=> \$stretch,"dpm=f"=> \$dpm );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $per eq 1 ){ $suffix .= "p"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
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
$h=($xb-$xa)/($n-1);
#
$cmd0="";
$cmd1="";
for($i=0; $i<$n; $i++){$x=$xa+ $i*$h; $y=$yb; $cmd0=$cmd0 . "$x $y\n";}
#
create mappings
  $ny = int( ($xb-$xa)/$ds +1.5 ); 
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
    $nr =5;  $dist= $ds*($nr-1);
    if( $width > 0. ){ $dist=$width; $nr =int( $dist/$ds+1.5 ); }
    forward
    distance to march $dist
    # optionally stretch grid lines -- increase number of lines to account for stretching
    if( $stretch>0. ){ $nr= int( $nr*1.75 ); $cmd="spacing: geometric\n  geometric stretch factor $stretch"; }else{ $cmd="#"; }
    $cmd
    lines to march $nr
    points on initial curve $nx
    BC: left fix x, float y and z
    BC: right fix x, float y and z
    generate
    # use fourth order interpolant to define the mapping:
    if( $dpm eq 4 ){ $cmd="fourth order"; }else{ $cmd="#"; }
    $cmd
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
    $ybb=$yb+.25*($yb-$ya); # heighten backgroud to allow for surface motion
    $xa $xb $ya $ybb 
  lines
    $nx = int( ($xb -$xa)/$ds +1.5 ); 
    $ny = int( ($ybb-$ya)/$ds +1.5 ); 
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
    # choose implicit or explicit interpolation
    interpolation type
      $interpType
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

