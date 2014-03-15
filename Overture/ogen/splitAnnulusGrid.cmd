#
# Annulus covered by two patches
#
#
# usage: ogen [noplot] splitAnnulusGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<>
# 
#  -ml = number of (extra) multigrid levels to support
# 
# examples:
#     ogen noplot splitAnnulusGrid -order=2 -factor=1 
#     ogen noplot splitAnnulusGrid -order=2 -factor=2 
#     ogen noplot splitAnnulusGrid -order=2 -factor=4 
#     ogen noplot splitAnnulusGrid -order=2 -factor=8
# 
#     ogen noplot splitAnnulusGrid -order=4 -factor=2 
#     ogen noplot splitAnnulusGrid -order=4 -factor=4 
#     ogen noplot splitAnnulusGrid -order=4 -factor=8
# 
#     ogen noplot splitAnnulusGrid -order=2 -factor=2 -ml=2
#     ogen noplot splitAnnulusGrid -order=2 -factor=4 -ml=3
# 
#     ogen noplot splitAnnulusGrid -order=4 -factor=4 -ml=2
#     ogen noplot splitAnnulusGrid -order=4 -factor=8 -ml=3
#     ogen noplot splitAnnulusGrid -order=4 -factor=16 -ml=4
# 
$order=2; $factor=1; $interp="e";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "explicit for all grids";
$name=""; 
$innerRad=.5; $outerRad = 1.;
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = "splitAnnulusGrid" . "$interp$factor" . $suffix . ".hdf";}
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
$pi=4.*atan2(1.,1.); 
#
# 
$ds=.1/$factor;
# 
create mappings
#
$midRad = .5*( $innerRad+$outerRad);
# 
Annulus
  # 
  $ra=$innerRad=.5; $rb = $midRad + $ds*($order-2);
  inner and outer radii
    $ra $rb
  lines
    $nTheta = intmg( 2.*$pi*($ra+$rb)*.5/$ds + 1.5 );
    $nr = intmg( ($rb-$ra)/$ds + 2.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 1 0
  mappingName
    innerAnnulus
exit
# 
Annulus
  # 
  $ra=$midRad - $ds*($order-2); $rb = $outerRad;
  inner and outer radii
    $ra $rb
  lines
    $nTheta = intmg( 2.*$pi*($ra+$rb)*.5/$ds + 1.5 );
    $nr = intmg( ($rb-$ra)/$ds + 2.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 0 2 
  mappingName
    outerAnnulus
exit
#
exit
generate an overlapping grid
    innerAnnulus
    outerAnnulus
  done
  change parameters
 # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      # add an extra ghost for singular problems: 
      if( $order ne "2" ){ $ngp=$ng+1; }else{ $ngp=$ng; }
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
splitAnnulusGrid
exit

