#
# Two adjacent boxes (with one addtional grid)
#
# usage: ogen [noplot] plug3d -factor=<num> -order=[2/4/6/8] -interp=[e/i]
# 
# examples:
#  ogen -noplot plug3d -interp=e -factor=1 
#  ogen -noplot plug3d -interp=e -factor=2 
#  ogen -noplot plug3d -interp=e -prefix=plugAnnulus -annulus=1 -factor=1
#  					       				       
#
$xa=-0.75; $xb=1.5; $ya=0; $yb=1; $za=0; $zb=1;
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$prefix="plug3d";
$deltaRadius0=0.2;
$annulus=0;
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp,"prefix=s"=>\$prefix,\
            "xa=f"=>\$xa,"xb=f"=>\$xb,"ml=i"=>\$ml,"annulus=i"=>\$annulus);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=3; }\
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
$name = $prefix . "$interp$factor" . $suffix . ".hdf";
# 
$ds=.1/$factor;
$pi=4.*atan2(1.,1.);
# 
create mappings
  Box
    set corners
     $xa $xb $ya $yb $za $zb
    lines
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ny=int( ($yb-$ya)/$ds+1.5 );
      $nz=int( ($zb-$za)/$ds+1.5 );
      $nx $ny $nz
    boundary conditions
      0 2 3 4 5 6
    share
      0 0 2 3 4 5
    mappingName
      box
    exit
# 
  Box
    set corners
     $xa=0; $xb=0.5;
     $xa $xb $ya $yb $za $zb
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $nx $ny $nz
    boundary conditions
    100 0 3 4 5 6
    share
    100 0 2 3 4 5
    mappingName
      plug
    exit 
#  
  cylinder
    mappingName 
      innerAnnulus 
    orientation
      1 2 0
    centre for cylinder 
    0 0.5 0.5
    bounds on the radial variable
    $deltaRadius=$deltaRadius0;
    $outerRadius=.4; 
    $innerRadius=$outerRadius-$deltaRadius;
    $innerRadius $outerRadius
    bounds on the axial variable
    $xb=$xb*.8;
    $xa $xb
    boundary conditions
    -1 -1 100 0 0 0
    share
    0 0 100 0 0 0
    lines 
      $ntheta=int( (2.*$pi*$outerRadius)/$ds+1.5 );
      $nr=int( ($deltaRadius)/$ds+2.5 );
      $nx=int( ($xb-$xa)/$ds+1.5 );
      $ntheta $nx $nr
    exit
#
exit
#
generate an overlapping grid
  box
  plug
  if($annulus eq 1){$cmds=innerAnnulus;}else{$cmds="#";}
  $cmds
  done
  change parameters
    ghost points
      all
       $ng $ng $ng $ng $ng $ng 
    order of accuracy
      $orderOfAccuracy
    interpolation type
      $interpType
  exit
  compute overlap
exit
save a grid (compressed)
$name
splitTwoBoxesPlug
exit
