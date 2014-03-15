#
# Ellipse in a channel
#
#
# usage: ogen [noplot] ellipseArg -factor=<num> -order=[2/4/6/8] -interp=[e/i] -blf=<num> -ml=<>  -rgd=[fixed|var] ...
#                             -radX=<> -radY=<> -angle=<> -xa=<> -xb=<> -ya=<> -yb=<> -cx=<> -cy=<>
# 
#  -blf : boundary-layer-factor : blf>1 : make grid lines near boundary this many times smaller
#  -ml = number of (extra) multigrid levels to support
#  -rgd : var=variable : decrease radial grid distance as grids are refined. fixed=fix radial grid distance
#  -radX, -radY : radii of the ellipse in the x and y directions
#  -angle : angle of rotation (degrees)
#  -xa, -xb, -ya, -yb : bounds on the back ground grid
#  -cx, -cy : center for the ellipse
# 
# Examples:
# 
#     ogen noplot ellipseArg -interp=e -blf=1.5 -factor=2
#     ogen noplot ellipseArg -interp=e -blf=1.5 -factor=4
# 
#  -- rotated ellipse:
#     ogen -noplot ellipseArg -interp=e -angle=45 -blf=1.5 -factor=2 
#     ogen -noplot ellipseArg -interp=e -angle=45 -blf=1.5 -factor=4
#     ogen -noplot ellipseArg -interp=e -angle=45 -blf=1.5 -factor=8
#     ogen -noplot ellipseArg -interp=e -angle=45 -blf=1.5 -factor=16
#     ogen -noplot ellipseArg -interp=e -angle=45 -blf=1.5 -factor=32
#
#  -- rotated ellipse with branch cut moved:
#    ogen -noplot ellipseArg -interp=e -angle=45 -blf=1.5 -branch=1 -factor=2 
#    ogen -noplot ellipseArg -interp=e -angle=45 -blf=1.5 -branch=1 -factor=4 
#    ogen -noplot ellipseArg -interp=e -angle=45 -blf=1.5 -branch=1 -factor=8 
#    ogen -noplot ellipseArg -interp=e -angle=45 -blf=1.5 -branch=1 -factor=16
#    ogen -noplot ellipseArg -interp=e -angle=45 -blf=1.5 -branch=1 -factor=32
#
#  -- bigger domain:
#    ogen -noplot ellipseArg -interp=e -angle=45 -blf=1.5 -branch=1 -factor=8 -xa=-1.25 -xb=4. -ya=-2.5 -yb=2.5 -name=ellipseAngle45BigDomaine8.order2
#    ogen -noplot ellipseArg -interp=e -angle=45 -blf=1.5 -branch=1 -factor=16 -xa=-1.25 -xb=4. -ya=-2.5 -yb=2.5 -name=ellipseAngle45BigDomaine16.order2
# 
#  -- near circular case:
#     ogen -noplot ellipseArg -interp=e -angle=45 -radX=.475 -radY=.525 -blf=1.5 -factor=2 -name=ellipse525Angle45e2.order2.s1.5.hdf
#
# OLD: from annulusArg
#
#     ogen noplot ellipseArg -order=2 -interp=e -factor=2   ( creates cice2.order2.hdf)
#     ogen noplot ellipseArg -order=2 -interp=e -factor=3   ( creates cice3.order2.hdf)
#     ogen noplot ellipseArg -order=2 -interp=e -factor=4   ( creates cice4.order2.hdf)
#     ogen noplot ellipseArg -order=2 -interp=e -factor=16    
#     ogen noplot ellipseArg -order=2 -interp=e -factor=32    
#     ogen noplot ellipseArg -order=2 -interp=e -factor=64   (6.2M pts)
#     ogen noplot ellipseArg -order=2 -interp=e -factor=128  (25M pts)
# 
#     ogen noplot ellipseArg -order=4 -interp=e -factor=2 
#     ogen noplot ellipseArg -order=4 -interp=e -factor=3 
#     ogen noplot ellipseArg -order=4 -interp=e -factor=4 
#     ogen noplot ellipseArg -order=4 -interp=e -factor=16    
#     ogen noplot ellipseArg -order=4 -interp=e -factor=32    
#     ogen noplot ellipseArg -order=4 -interp=e -factor=64 
# 
#   -- fixed radial distance
#     ogen noplot ellipseArg -order=2 -interp=e -rgd=fixed -factor=1
#     ogen noplot ellipseArg -order=2 -interp=e -rgd=fixed -factor=2  
# 
#   -- fixed radial distance + order 4
#     ogen noplot ellipseArg -order=4 -interp=e -rgd=fixed -factor=2  
#     ogen noplot ellipseArg -order=4 -interp=e -rgd=fixed -factor=4 
# 
#   -- multigrid
#     ogen noplot ellipseArg -order=2 -interp=e -ml=2 -factor=1
#     ogen noplot ellipseArg -order=2 -interp=e -ml=2 -factor=2 
# 
#   -- multigrid and stretched
#     ogen noplot ellipseArg -order=4 -interp=e -ml=2 -blf=2 -factor=2 
#     ogen noplot ellipseArg -order=4 -interp=e -ml=3 -blf=2 -factor=4 
# 
#   -- multigrid and fixed radial distance
#     ogen noplot ellipseArg -order=4 -interp=e -ml=1 -rgd=fixed -factor=2  
#     ogen noplot ellipseArg -order=4 -interp=e -ml=2 -rgd=fixed -factor=4 
#
#   -- long channel for flow past a cylinder (with multigrid levels and boundary layer stretching)
#     ogen noplot ellipseArg -order=2 -interp=e -xa=-2. -xb=10. -ya=-3.0 -yb=3.0 -ml=2 -factor=2  -blf=4 -name="cicLongChannele2.hdf"
#     ogen noplot ellipseArg -order=2 -interp=e -xa=-2. -xb=10. -ya=-3.0 -yb=3.0 -ml=3 -factor=4  -blf=4 -name="cicLongChannele4.hdf"
# 
#
$prefix="ellipse";  $rgd="var"; $angle=0.; $branch=0; 
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; 
$radX=.7; $radY=.35; # radii of the ellipse in the x and y directions
$cx=0.; $cy=0.;  # center for the ellipse
$blf=1;  # this means no stretching
$deltaRadius0=.3; # radius for rgd fixed
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf, "prefix=s"=> \$prefix,\
            "cx=f"=>\$cx,"cy=f"=>\$cy,"rgd=s"=> \$rgd,"radX=f"=>\$radX,"radY=f"=>\$radY,"angle=f"=>\$angle,\
            "branch=i"=>\$branch );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; }
if( $branch ne 0 ){ $prefix = $prefix . "Branch"; }
if( $angle ne 0 ){ $prefix = $prefix . "Angle$angle"; }
$suffix = ".order$order"; 
if( $blf ne 1 ){ $suffix .= ".s$blf"; }
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
$pi = 4.*atan2(1.,1.);
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
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
#
create mappings
#
rectangle
  set corners
    $xa $xb $ya $yb
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 2 3 4 
  mappingName
  square
exit
#
# if 
if( $blf>1 ){ $annulusName="AnnulusUnStretched"; $stretchAnnulusName="Annulus"; }else{ $annulusName="Annulus"; $stretchAnnulusName="AnnulusStretched"; }
 # --- make the ellipse 
   Circle or ellipse
     mappingName
       ellipseBoundary
     specify centre
      $cx $cy
     specify axes of the ellipse
       $radX $radY
    # optionally move the branch cut to the bottom for testing
    if( $branch ne 0 ){ $cmd="specify start/end angles\n  -.25 .75"; }else{ $cmd="#"; }
    $cmd
   exit
# ---
 # Make sure there are at least 4 points on the coarsest MG level
 $nr = max( 6 + $ng + 2*($order-2), 2**($ml+2) );
 $nr = intmg( $nr );
 mapping from normals
  extend normals from which mapping?
    ellipseBoundary
  $nDist = ($nr-1)*$ds;
  if( $rgd eq "fixed" ){ $nDist=$deltaRadius0; $nr=intmg( $deltaRadius0/$ds + 2.5 ); }
  normal distance
    $nDist
  lines
    if( $blf>1 ){ $nr = $nr + 4; } # extra grid lines to account for stretching
    # Arclength: use Ramanujan's formula:
    $a = $radX+.5*$nDist; $b=$radY+.5*$nDist;
    $arcLength = $pi*( 3.*($a+$b)-sqrt( 10.*$a*$b + 3.*( $a*$a+$b*$b) ) );
    $nTheta = intmg( $arcLength/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 5 0
  share
     0  0 5 0
  mappingName
   $annulusName
  exit
#
# optionally stretch the grid lines next to the cylinder
# 
 stretch coordinates 
  transform which mapping? 
    $annulusName 
  multigrid levels $ml
  # add extra resolution in the stretching direction: 
  stretch resolution factor 2.
  # exponential to linear stretching: 
   Stretch r2:exp to linear
   STP:stretch r2 expl: position 0
   $dxMin = $ds/$blf; 
   STP:stretch r2 expl: min dx, max dx $dxMin $ds
  STRT:name $stretchAnnulusName
 exit
#
# Convert to a nurbs and rotate
#
sub convertToNurbs\
{ local($old,$new,$angle)=@_; \
  $commands = "nurbs (surface)\n" . \
              "interpolate from mapping with options\n" . "$old\n" . "parameterize by index (uniform)\n" . "done\n" . \
              "rotate\n" . "$angle 1\n" . "0 0 0\n" . \
              "mappingName\n" . "$new\n" . "exit\n"; \
}
convertToNurbs("Annulus","ellipse",$angle);
$commands
#
#
exit
generate an overlapping grid
    square
    ellipse
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
#  display intermediate results
# open graphics
  compute overlap
  plot
#   query a point 
#     interpolate point 1
#     check interpolation coords 1
#     pt: grid,i1,i2,i3: 1 5 6 0
# 
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
cic
exit

