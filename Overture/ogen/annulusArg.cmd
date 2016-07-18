#
# Annulus (taking arguments)
#
#
# usage: ogen [-noplot] annulusArg -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<>
# 
#  -ml = number of (extra) multigrid levels to support
# 
# examples:
#     ogen -noplot annulusArg -order=2 -factor=1 
#     ogen -noplot annulusArg -order=2 -factor=2 
#     ogen -noplot annulusArg -order=2 -factor=4 
#     ogen -noplot annulusArg -order=2 -factor=8
# 
#     ogen -noplot annulusArg -order=4 -factor=1 
#     ogen -noplot annulusArg -order=4 -factor=2 
#     ogen -noplot annulusArg -order=4 -factor=4 
#     ogen -noplot annulusArg -order=4 -factor=8
# 
#     ogen -noplot annulusArg -order=2 -factor=2 -ml=2
#     ogen -noplot annulusArg -order=2 -factor=4 -ml=3
# 
#     ogen -noplot annulusArg -order=4 -factor=4 -ml=2
#     ogen -noplot annulusArg -order=4 -factor=8 -ml=3
#     ogen -noplot annulusArg -order=4 -factor=16 -ml=4
#
#  Annulus with innerRad=1 outerRad=2
#     ogen -noplot annulusArg -prefix=annulusGrid -innerRad=1 -outerRad=2 -order=2 -factor=0.5
#     ogen -noplot annulusArg -prefix=annulusGrid -innerRad=1 -outerRad=2 -order=2 -factor=1
#     ogen -noplot annulusArg -prefix=annulusGrid -innerRad=1 -outerRad=2 -order=2 -factor=2
# 
$order=2; $factor=1; $interp="e";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "explicit for all grids";
$prefix="annulus"; $name=""; 
# 
$innerRad=.5; $outerRad = 1.;
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,\
            "innerRad=f"=> \$innerRad,"outerRad=f"=> \$outerRad,"prefix=s"=> \$prefix );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$factor" . $suffix . ".hdf";}
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
#
# 
$ds=.1/$factor;
$pi = 4.*atan2(1.,1.);
# 
create mappings
#
Annulus
  inner and outer radii
    $innerRad $outerRad
  lines
    $nTheta = intmg( 2.*$pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
    # $nr = intmg( ($outerRad-$innerRad)/$ds + 2.5 ); # *wdh* July 7, 2016
    $nr = intmg( ($outerRad-$innerRad)/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 1 2
  mappingName
    annulus
exit
#
exit
generate an overlapping grid
    annulus
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
annulus
exit

