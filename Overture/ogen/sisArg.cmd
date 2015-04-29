*
* Square in a square
*
*
* usage: ogen [noplot] sisArg -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<>
#  -ml = number of (extra) multigrid levels to support
* 
* examples:
*     ogen -noplot sisArg -order=2 -interp=i -factor=1
*     ogen -noplot sisArg -order=2 -interp=e -factor=2
*     ogen -noplot sisArg -order=4 -interp=e -factor=2
*     ogen -noplot sisArg -order=4 -interp=e -factor=2 -ml=2
* 
*     ogen -noplot sisArg -order=2 -interp=e -factor=1 -ml=2
*     ogen -noplot sisArg -order=2 -interp=e -factor=1 -ml=3
*     ogen -noplot sisArg -order=2 -interp=e -factor=2 -ml=2
*
*     ogen -noplot sisArg -order=4 -interp=e -factor=4 -ml=2
*     ogen -noplot sisArg -order=4 -interp=e -factor=8 -ml=3
*     ogen -noplot sisArg -order=4 -interp=e -factor=16 -ml=4
* 
# Extra ghost:
#     ogen -noplot sisArg -numGhost=3 -order=4 -interp=e -factor=2
#
$order=2; $factor=1; $interp="i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$numGhost=-1;  # if this value is set, then use this number of ghost points
$xa=-1.; $xb=1.; $ya=-1.; $yb=1.;  # bounds on outer square
$xai=-.5; $xbi=.5; $yai=-.5; $ybi=.5;   # bounds on inner square
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp,"ml=i"=>\$ml,"numGhost=i"=>\$numGhost);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=3; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=5; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $numGhost ne -1 ){ $ng = $numGhost; } # overide number of ghost
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
$name = "sis" . "$interp$factor" . $suffix . ".hdf";
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
* 
$ds=.1/$factor;
* 
create mappings
  rectangle
    set corners
     $xa $xb $ya $yb 
    lines
      $nx=intmg( ($xb-$xa)/$ds+1.5 );
      $ny=intmg( ($yb-$ya)/$ds+1.5 );
      $nx $ny
    mappingName
      outer-square
    exit
*
  rectangle
    set corners
     $xai $xbi $yai $ybi 
    lines
      $nx=intmg( ($xbi-$xai)/$ds+1.5 );
      $ny=intmg( ($ybi-$yai)/$ds+1.5 );
      $nx $ny
    boundary conditions
      0 0 0 0
    mappingName
      inner-square
    exit
  exit
*
generate an overlapping grid
  outer-square
  inner-square
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
sis
exit
