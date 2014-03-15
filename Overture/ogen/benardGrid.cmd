#
# Make grids for the Rayleigh Benard problem
# 
# usage: ogen [-noplot] benardGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<>  ...
#                             -xa=<> -xb=<> -ya=<> -yb=<> -prefix=<> -numGhost=<i> -periodic=[n|p]
# 
#  -ml = number of (extra) multigrid levels to support
#  -xa, -xb, -ya, -yb : bounds on the back ground grid
#  -periodic=y : make periodic in x.
# 
# Examples:
#     ogen -noplot benardGrid -order=2 -interp=e -factor=1
#     ogen -noplot benardGrid -order=2 -interp=e -factor=2
#     ogen -noplot benardGrid -order=2 -interp=e -factor=4
#     ogen -noplot benardGrid -order=2 -interp=e -factor=8
#
# -- fourth-order
#     ogen -noplot benardGrid -order=4 -interp=e -factor=4 
#     ogen -noplot benardGrid -order=4 -interp=e -factor=8 
#  - periodic:
#     ogen -noplot benardGrid -order=4 -interp=e -periodic=p -factor=4
#     ogen -noplot benardGrid -order=4 -interp=e -periodic=p -factor=8
#     ogen -noplot benardGrid -order=4 -interp=e -periodic=p -factor=16
#     ogen -noplot benardGrid -order=4 -interp=e -periodic=p -factor=32
#
$xa=0.;  $xb=4.0; 
$ya=0.;  $yb=1.0; 
$prefix="benardGrid";  $blf=1; $periodic="n"; 
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name="";
$numGhost=-1;  # if this value is set, then use this number of ghost points
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"prefix=s"=> \$prefix,"ml=i"=>\$ml,"blf=f"=> \$blf, "prefix=s"=> \$prefix,\
            "numGhost=i"=>\$numGhost,"periodic=s"=>\$periodic );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=3; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=5; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $numGhost ne -1 ){ $ng = $numGhost; } # overide number of ghost
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
if( $periodic eq "p" ){ $suffix .= "p"; }
if( $blf ne 1 ){ $suffix .= ".s$blf"; }
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
#
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
create mappings 
#
  rectangle 
    mappingName
      backGround
    set corners
     $xa $xb $ya $yb 
    lines
      $nx=intmg( ($xb-$xa)/$ds+1.5 );
      $ny=intmg( ($yb-$ya)/$ds+1.5 );
      $nx $ny
    boundary conditions
     if( $periodic eq "p" ){ $bc ="-1 -1 3 4 "; }else{ $bc="1 2 3 4"; }
     $bc    
    exit 
  exit this menu 
#
generate an overlapping grid
  backGround
  done
  change parameters
    # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      # $ngp = $ng+1;
      $ngp = $ng;
      $ng $ng $ng $ngp $ng $ng
  exit
  compute overlap
exit
#
save an overlapping grid
  $name
  benardGrid
exit