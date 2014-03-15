#
# airfoil.cmd : Ogen command file. Generate a grid for an airfoil using the hyperbolic grid generator.
#
# usage: ogen [-noplot] airfoil -factor=<num> -order=[2/4/6/8] -interp=[e/i] --camber=<> -angle=<degrees> ...
#             xa=<> -xb=<> -ya=<> -yb=<> -name=<over-ride file name>
# 
#  -xa, -xb, -ya, -yb : bounds on the back ground grid
#
# Examples:
#   ogen -noplot airfoil -order=2 -interp=e -factor=1
#   ogen -noplot airfoil -order=2 -interp=e -factor=1 -xb=4. -name=airfoile1big.order2.hdf
# 
#   ogen -noplot airfoil -order=2 -interp=e -factor=2 -xb=3.
#
#
$prefix="airfoil"; $camber=0.; $angle=-2.; $order=2; $ml=0; 
$blf=1.; $interp="e";
$xa=-1.5; $xb=2.5; $ya=-1.5; $yb=1.5; 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf, "prefix=s"=> \$prefix,\
            "camber=f"=>\$camber,"angle=f"=>\$angle );
# 
if( $order eq 2 ){ $orderOfAccuracy="second order"; $ng=2; }\
elsif( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $blf ne 1 ){ $suffix .= ".s$blf"; }
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# --- OLD: 
# $factor="1"; $grid="airfoil1.hdf"; 
# $factor="2"; $grid="airfoil2.hdf"; 
# $factor="4"; $grid="airfoil4.hdf"; 
# $factor="8"; $grid="airfoil8.hdf"; 
# $factor="16"; $grid="airfoil16.hdf"; 
# $factor="32"; $grid="airfoil32.hdf"; 
#
# $factor="1"; $grid="airfoilWithCamber1.hdf"; $camber=.1; $angle=2.; 
# $factor="2"; $grid="airfoilWithCamber2.hdf"; $camber=.1; $angle=2.; 
# 
$ds=3./45./$factor; 
#
create mappings
 #
 # First make a back-ground grid  
 #
  rectangle
    mappingName
      backGround
    set corners
      $xa $xb $ya $yb
#      -1.5 2.5  -1.5 1.5 
    lines
     $nx = int( ($xb-$xa)/$ds+1.5 );
     $ny = int( ($yb-$ya)/$ds+1.5 );
     $nx $ny 
#     61 45 
  exit
 # make the NACA airfoil (curve)
  Airfoil
    airfoil type
      naca
    camber 
      $camber
    lines
     $arcLength=2.5 + 3./$factor; # include more points on coarser grids
     $ns = int( $arcLength/$ds+1.5 );
     $ns 
#    91
    mappingName
      airfoil-curve
    exit
#   -- rotate the airfoil curve 
  rotate/scale/shift
    transform which mapping?
    airfoil-curve
    rotate
      $angle
      0 0
    mappingName
      airfoil-curve-rotated
    exit
#
  hyperbolic
#
    BC: left trailing edge
    BC: right trailing edge
#
    $radialDist=.35/$factor; $nr = 21;
    lines to march $nr
    distance to march $radialDist
#   lines to march 21
#   distance to march .45
#    uniform dissipation coefficient .01
    geometric stretch factor 1.15
    generate
    smoothing...
    GSM:BC: left periodic
    GSM:BC: right periodic
    GSM:BC: top smoothed
    GSM:number of iterations 5
    GSM:smooth grid
    GSM:smooth grid
# 
# pause
    mappingName
     airfoil_dpm
    boundary conditions
      -1 -1 1 0
    exit
* Define a subroutine to convert a Mapping to a Nurbs Mapping
sub convertToNurbs\
{ local($old,$new,$angle)=@_; \
  $commands = "nurbs\n" . \
              "interpolate from mapping with options\n" . "$old\n" . \
              "choose degree\n 2 \n" . \
              "parameterize by index (uniform)\n" . "done\n" . \
              "rotate\n" . "$angle 1\n" . "0 0 0\n" . \
              "mappingName\n" . "$new\n" . "exit\n"; \
}
*
convertToNurbs("airfoil_dpm","airfoil",0.);
$commands
* 
 exit
#
# make an overlapping grid
#
generate an overlapping grid
    backGround
    airfoil
  done
  change parameters
    # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      # $ngp = $ng+1; # causes trouble with parallel AMR
      $ngp = $ng;
      $ng $ng $ng $ngp $ng $ng
  exit
 # pause
  compute overlap
 # pause
exit
#
save an overlapping grid
$name
naca
exit
