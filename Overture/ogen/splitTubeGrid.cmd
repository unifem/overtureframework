#
# 3D cylindrical tube with two overlapping cylinders
#
#
# usage: ogen [noplot] splitTubeGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i] -name= -sa= -sb= -outerRad=<f> -prefix=<s> -orient=[x|y|z]
# 
#  factor : grid resolution factor 
# 
# examples:
#     ogen -noplot splitTubeGrid -factor=2 -order=2 -interp=e
#     ogen -noplot splitTubeGrid -factor=4 -order=2 -interp=e
#     ogen -noplot splitTubeGrid -factor=4 -order=2 -interp=e -sa=-1. -sb=1. 
# 
# -- tube parallel to z-axis for cgmx eigenmodes
#     ogen -noplot splitTubeGrid -interp=e -prefix=tubeGrid -outerRad=1. -orient=z -order=2 -factor=2 
#     
# -- set default parameter values:
$outerRad=.5; 
$sa=-.5; $sb=.5;
$order=2; $factor=1; $interp="i"; $name="";
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$prefix="splitTubeGrid"; $orient="x"; 
$numGhost=-1;  # if this value is set, then use this number of ghost points
# 
# get command line arguments
GetOptions("order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp,"outerRad=f"=> \$outerRad,"sa=f"=> \$sa,"sb=f"=> \$sb,\
           "name=s"=>\$name,"prefix=s"=>\$prefix,"orient=s"=>\$orient,"numGhost=i"=>\$numGhost);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
if( $numGhost ne -1 ){ $ng = $numGhost; } # overide number of ghost
if( $name eq "" ){ $name = $prefix . "$interp$factor" . $suffix . ".hdf"; }
# parallel ghost lines: for ogen we need at least:
#       .5*( iw -1 )   : implicit interpolation 
#       .5*( iw+dw-2 ) : explicit interpolation
$dw = $order+1; $iw=$order+1; 
$parallelGhost=($iw-1)/2;
if( $interp eq "e" ){  $parallelGhost=($iw+$dw-2)/2; }
if( $parallelGhost<1 ){ $parallelGhost=1; } 
minimum number of distributed ghost lines
  $parallelGhost
# 
$ds=.05/$factor;
$pi = 4.*atan2(1.,1.); 
#
#
# Make a cylinder in a box
#
create mappings
#
  Cylinder
    mappingName
      cylinderLeft
    bounds on the radial variable
 # cylinder is a fixed number of lines in the radial direction: 
      $nr=5+$order; 
      $deltaRad=($nr-1)*$ds; 
      $innerRad = $outerRad - $deltaRad; 
      $innerRad $outerRad
    bounds on the axial variable
      $extra=2*$ds + ($ng-1)*$ds;      # add extra to the overlap
      $sm= .5*($sa+$sb) + $extra; # increase overlap 
      $sa $sm
    orientation
      # choose axial direction to be the the x, y or z direction.
      if( $orient eq "x" ){ $cmd = "1 2 0"; }elsif( $orient eq "y" ){ $cmd="2 0 1"; }else{ $cmd="0 1 2"; }
      $cmd
      # 1 2 0 
    lines
      $nt = int( 2.*$pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
      $ns = int( ($sm-$sa)/$ds + 1.5 );
      $nt $ns $nr 
    boundary conditions
     # theta axial radial
     -1 -1   2 0   0 1 
    share
      0 0 2 0 0 1
  exit
#
  Cylinder
    mappingName
      cylinderRight
    bounds on the radial variable
 # cylinder is a fixed number of lines in the radial direction: 
      $deltaRad=($nr-1)*$ds; 
      $innerRad = $outerRad - $deltaRad; 
      $innerRad $outerRad
    bounds on the axial variable
      $sm= .5*($sa+$sb) - $extra; # increase overlap 
      $sm $sb
    orientation
      # choose axial direction to be the the x, y or z direction.
      if( $orient eq "x" ){ $cmd = "1 2 0"; }elsif( $orient eq "y" ){ $cmd="2 0 1"; }else{ $cmd="0 1 2"; }
      $cmd
      # 1 2 0 
    lines
      $nt = int( 2.*$pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
      $ns = int( ($sb-$sm)/$ds + 1.5 );
      $nt $ns $nr 
    boundary conditions
 # theta axial radial
     -1 -1   0 3   0 1 
    share
      0 0 0 3 0 1
  exit
# 
  Box
    mappingName
      box
  set corners
  lines
    if( $orient eq "x" ){ $xa=$sa; $xb=$sb; $ya=-$innerRad-$extra-1*$ds; $yb=-$ya; $za=$ya; $zb=$yb; }
    if( $orient eq "y" ){ $ya=$sa; $yb=$sb; $za=-$innerRad-$extra-1*$ds; $zb=-$za; $xa=$za; $xb=$zb; }
    if( $orient eq "z" ){ $za=$sa; $zb=$sb; $xa=-$innerRad-$extra-1*$ds; $xb=-$xa; $ya=$xa; $yb=$xb; }
    set corners
      $xa $xb $ya $yb $za $zb
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $ny = int( ($yb-$ya)/$ds + 1.5 );
      $nz = int( ($zb-$za)/$ds + 1.5 );
      $nx $ny $nz
    if( $orient eq "x" ){ $cmd="2 3 0 0 0 0"; }
    if( $orient eq "y" ){ $cmd="0 0 2 3 0 0"; }
    if( $orient eq "z" ){ $cmd="0 0 0 0 2 3"; }
    boundary conditions
      $cmd
    share
      $cmd
  exit
exit
#
#
generate an overlapping grid
    box
    cylinderLeft
    cylinderRight
  done
  change parameters
    order of accuracy
      $orderOfAccuracy
    interpolation type
      $interpType
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
#
#  display intermediate results
# pause
# 
  compute overlap
  exit
#
save an overlapping grid
$name
tube
exit


