#
# Circle in a channel (with fixed radius of the cylinder)
#
#
# usage: ogen [noplot] cicFixed -factor=<num> -order=[2/4/6/8] -interp=[e/i]
# 
# examples:
#     ogen noplot cicFixed -order=2 -interp=e -factor=2    ( creates cicFixede2.order2.hdf)
# 
#
#
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; 
$numGhost=-1;  # if this value is set, then use this number of ghost points
$radiusFixed = .25;   # fixed radius
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"numGhost=i"=>\$numGhost,"radiusFixed=f"=> \$radiusFixed );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $numGhost ne -1 ){ $ng = $numGhost; } # overide number of ghost
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
if( $name eq "" ){$name = "cicFixed" . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
# 
$dw = $order+1; $iw=$order+1; 
# parallel ghost lines: for ogen we need at least:
#       .5*( iw -1 )   : implicit interpolation 
#       .5*( iw+dw-2 ) : explicit interpolation
$parallelGhost=($iw-1)/2;
if( $interp eq "e" ){  $parallelGhost=($iw+$dw-2)/2; }
if( $parallelGhost<1 ){ $parallelGhost=1; } 
minimum number of distributed ghost lines
  $parallelGhost
create mappings
#
rectangle
  set corners
    $xa $xb $ya $yb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5 ); 
    $ny = int( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
#
Annulus
  $innerRad=.5; 
  # $outerRad = $innerRad + ($nr-1)*$ds;
  $outerRad = $innerRad + $radiusFixed;
  inner and outer radii
    $innerRad $outerRad
  lines
    $nTheta = int( 2.*3.1415*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nr = int( ($outerRad-$innerRad)/$ds + 2.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 1 0
exit
#
exit
generate an overlapping grid
    square
    Annulus
  done
  change parameters
 # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ngp=$ng+1;
      $ng $ng $ng $ngp $ng $ng 
  exit
#  display intermediate results
  compute overlap
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
cic
exit

