#
# Grid for a channel with TWO component grids (for testing moving grids)
#
#
# usage: ogen [noplot] splitChannelGrid -factor=<num> -order=[2/4/6/8] -ml=<> -per=[0|1]
# 
#  -per : 1=periodic channel (default), 0=not periodic
#  -ml = number of (extra) multigrid levels to support
# 
# examples:
#     ogen -noplot splitChannelGrid -factor=1
#     ogen -noplot splitChannelGrid -factor=2
#     ogen -noplot splitChannelGrid -factor=4
#     ogen -noplot splitChannelGrid -factor=5
#     ogen -noplot splitChannelGrid -factor=10
#     ogen -noplot splitChannelGrid -factor=20
#     ogen -noplot splitChannelGrid -factor=40
#
# Non-periodic: (for pipe flow)
#     ogen -noplot splitChannelGrid -order=2 -length=2. -per=0 -factor=1
#     ogen -noplot splitChannelGrid -order=2 -length=2. -per=0 -factor=2
#						 								  
#     ogen -noplot splitChannelGrid -order=4 -length=2. -per=0 -factor=1
#     ogen -noplot splitChannelGrid -order=4 -length=2. -per=0 -factor=2
# 						 								  
# Fourth-order: (non-periodic)			 					  
#     ogen -noplot splitChannelGrid -order=4 -length=3. -per=0 -factor=1
#     ogen -noplot splitChannelGrid -order=4 -length=3. -per=0 -factor=2
# multigrid:
#     ogen -noplot splitChannelGrid -factor=2 -ml=2
#     ogen -noplot splitChannelGrid -factor=5 -ml=3 
#     ogen -noplot splitChannelGrid -factor=10 -ml=3
#     ogen -noplot splitChannelGrid -factor=20 -ml=3   (splitChannelGrid20.order2.ml3.hdf, 1.2M pts)
#     ogen -noplot splitChannelGrid -factor=40 -ml=4   (splitChannelGrid40.order2.ml4.hdf, 5M pts)
# 
#
$pi=4.*atan2(1.,1.); $per=1; $interp="e";
$order=2; $factor=1;  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
# 
$length=2.*$pi; # length of splitChannelGrid)/(2*pi)
$xa=0.; $xb=$length; 
$ya=-1.; $yb=1.; 
$bStretch=7.; $nyFactor=2.5; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=>\$factor,"ml=i"=>\$ml,"length=f"=>\$length,"name=s"=> \$name,\
            "interp=s"=> \$interp,"per=i"=>\$per );
$xb=$length; 
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
if( $interp eq "i" ){ $interpType = "implicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){ $name = "splitChannelGrid" . "$interp$factor" . $suffix . ".hdf"; }
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
# 
$ds=.1/$factor;
#
# 
create mappings
#
$ym=.5*($ya+$yb); 
#
rectangle
  set corners
    $ya0=$ya; $yb0=$yb;
    $xa $xb $ya0 $yb0
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 );
    $ny = intmg( ($yb0-$ya0)/$ds +1.5 );
    $nx $ny
  boundary conditions
    if( $per eq 1 ){ $cmd="-1 -1 3 4"; }else{ $cmd="1 2 3 4"; }
    $cmd
  share
    0 0 0 4
  mappingName
   lowerChannel
exit
# -- the upperChannel is a patch that abuts the upper surface --
#  It does not extend the entire width -- this means we can move it 
rectangle
  set corners
    $xa0=.25; $xb0=.75;
    $ya0=$yb-.5; $yb0=$yb; 
    $xa0 $xb0 $ya0 $yb0
  lines
    $nx = intmg( ($xb0-$xa0)/$ds +1.5 );
    $ny = intmg( ($yb0-$ya0)/$ds +1.5 );
    $nx $ny
  boundary conditions
    if( $per eq 1 ){ $cmd="0 0 0 5"; }else{ $cmd="0 0 0 5"; }
    $cmd
  share
    0 0 0 4
  mappingName
   upperChannel
exit
# 
#
exit
generate an overlapping grid
    lowerChannel
    upperChannel
  done
  change parameters
    interpolation type
      $interpType
    ghost points
      all
       $ng $ng $ng $ng $ng $ng 
    order of accuracy
      $orderOfAccuracy
  exit
#
  compute overlap
  exit
#
save a grid (compressed)
$name
splitChannelGrid
exit

