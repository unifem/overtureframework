#=================================================================================
#
# grid for a sphere dropping in a channel
#
#  Examples: 
#   ogen -noplot drop3d -factor=1 -ml=1
#   ogen -noplot drop3d -factor=2 -ml=1
#   ogen -noplot drop3d -factor=4 -ml=2
# 
#   ogen -noplot drop3d -interp=e -factor=2 -ml=1
#
#=================================================================================
# scale number of grid points in each direction by the following factor
# $factor=1; $name = "drop3d.hdf";   $ml=1;
# $factor=2; $name = "drop3d2.hdf";   $ml=1;
# $factor=4; $name = "drop3d4.hdf";   $ml=2;
#
#
$prefix="drop3d";  $rgd="var";
$innerRadius=.25; $deltaRadius=.225; $dse=0.; 
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$saveWeights=1;   # 1 = save integration weights
$xa=-1.; $xb=1.; $ya=-3.; $yb=1.; $za=$xa; $zb=$xb; 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"ml=i"=>\$ml,"name=s"=> \$name,"saveWeights=i"=>\$saveWeights);
# 
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
# printf(" factor=$factor, ml=$ml\n");
#
#-----
$ds=.1/$factor;
$pi=4.*atan2(1.,1.);
#-----
#
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
#**************************************************************************
#
create mappings
#
box
  set corners
    $xa $xb $ya $yb $za $zb 
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nz = intmg( ($zb-$za)/$ds +1.5 ); 
    $nx $ny $nz
  boundary conditions
    1 2 3 4 5 6
  mappingName
   channel
exit
#
# reduce the oudter radius of the sphere as the grid is defined.
# we need to keep enough multigrid levels though.
# case 1:  
#   $nx0=21; $ny0=21; $nz0 = 7; $innerRadius=.35; $deltaRadius=.3;  
# case 2:
#   $nx0=17; $ny0=17; $nz0 = 7; $innerRadius=.125; $deltaRadius=.2;
# case 3:  
##$nx0=13; $ny0=13; $nz0 = 5; $innerRadius=.25; $deltaRadius=.225;  
#
##  $nzMG =int( int(($nz0+$mgFactor-1)/$mgFactor)*$mgFactor+1.5);
##$outerRadius = $innerRadius + $deltaRadius*$nzMG/$nz0/$factor;
#
##  getGridPoints($nx0,$ny0,$nz0);
#
Sphere
  $nr = 7;
  $nr = intmg( $nr );
  $outerRadius = $innerRadius + ($nr-2)*$ds;
  inner and outer radii
    $innerRadius $outerRadius
  centre for sphere
    0. 0. 0.
  mappingName
   sphere1
exit
#
# 
# north pole of sphere 1
reparameterize
  orthographic
    $sa = 2. + ($order-2)*$ds*.5 + $order*$dse*$ds; $sb=$sa; 
    specify sa,sb
      $sa $sa
  exit
  lines
   $nTheta=intmg( 3.2*($innerRadius+$outerRadius)*.5/$ds +1.5 );  
   $nTheta $nTheta $nr
  boundary conditions
    0 0 0 0 1 0
  share
   0 0 0 0 1 0
  mappingName
    sphere1-north-pole
exit
# south pole of sphere 1
reparameterize
  orthographic
    choose north or south pole
      -1
    specify sa,sb
      $sa $sa
  exit
  lines
   $nTheta $nTheta $nr
  boundary conditions
    0 0 0 0 1 0
  share
    0 0 0 0 1 0
  mappingName
    sphere1-south-pole
exit
#
exit
generate an overlapping grid
    channel
    sphere1-north-pole
    sphere1-south-pole
  done
  change parameters
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
#  display intermediate results
  compute overlap
#  x-r 90
#   change the plot
#   toggle shaded surfaces 0 0
#   exit
#   pause
  exit
#
save integration weights $saveWeights
save an overlapping grid
$name
drop3d
exit

