#
# Circle in a channel with two grids on the annulus (taking arguments)
#
#
# usage: ogen [noplot] cicSplit -factor=<num> -order=[2/4/6/8] -interp=[e/i] -blf=<num> -ml=<>  -rgd=[fixed|var] ...
#                             -xa=<> -xb=<> -ya=<> -yb=<> -cx=<> -cy=<>
# 
#  -blf : boundary-layer-factor : blf>1 : make grid lines near boundary this many times smaller
#  -ml = number of (extra) multigrid levels to support
#  -rgd : var=variable : decrease radial grid distance as grids are refined. fixed=fix radial grid distance
#  -xa, -xb, -ya, -yb : bounds on the back ground grid
#  -cx, -cy : center for the annulus
# 
# examples:
#     ogen noplot cicSplit -order=2 -factor=1
#     ogen noplot cicSplit -order=4 -factor=1
#     ogen noplot cicSplit -order=4 -factor=2
# 
#     ogen noplot cicSplit -order=2 -interp=e -factor=2   ( creates cice2.order2.hdf)
#     ogen noplot cicSplit -order=2 -interp=e -factor=3   ( creates cice3.order2.hdf)
#     ogen noplot cicSplit -order=2 -interp=e -factor=4   ( creates cice4.order2.hdf)
#     ogen noplot cicSplit -order=2 -interp=e -factor=16    
#     ogen noplot cicSplit -order=2 -interp=e -factor=32    
#     ogen noplot cicSplit -order=2 -interp=e -factor=64   (6.2M pts)
#     ogen noplot cicSplit -order=2 -interp=e -factor=128  (25M pts)
# 
#     ogen noplot cicSplit -order=4 -interp=e -factor=2 
#     ogen noplot cicSplit -order=4 -interp=e -factor=3 
#     ogen noplot cicSplit -order=4 -interp=e -factor=4 
#     ogen noplot cicSplit -order=4 -interp=e -factor=16    
#     ogen noplot cicSplit -order=4 -interp=e -factor=32    
#     ogen noplot cicSplit -order=4 -interp=e -factor=64 
# 
#   -- fixed radial distance
#     ogen noplot cicSplit -order=2 -interp=e -rgd=fixed -factor=1
#     ogen noplot cicSplit -order=2 -interp=e -rgd=fixed -factor=2  
#     ogen noplot cicSplit -order=2 -interp=e -rgd=fixed -factor=4 
#     ogen noplot cicSplit -order=2 -interp=e -rgd=fixed -factor=8 
#     ogen noplot cicSplit -order=2 -interp=e -rgd=fixed -factor=16
#     ogen noplot cicSplit -order=2 -interp=e -rgd=fixed -factor=32
#     ogen noplot cicSplit -order=2 -interp=e -rgd=fixed -factor=64
# 
#   -- fixed radial distance + order 4
#     ogen noplot cicSplit -order=4 -interp=e -rgd=fixed -factor=2  
#     ogen noplot cicSplit -order=4 -interp=e -rgd=fixed -factor=4 
#     ogen noplot cicSplit -order=4 -interp=e -rgd=fixed -factor=8 
#     ogen noplot cicSplit -order=4 -interp=e -rgd=fixed -factor=16
#     ogen noplot cicSplit -order=4 -interp=e -rgd=fixed -factor=32
#     ogen noplot cicSplit -order=4 -interp=e -rgd=fixed -factor=64
# 
#   -- multigrid
#     ogen noplot cicSplit -order=2 -interp=e -ml=2 -factor=1
#     ogen noplot cicSplit -order=2 -interp=e -ml=2 -factor=2 
#     ogen noplot cicSplit -order=2 -interp=e -ml=3 -factor=4 
#     ogen noplot cicSplit -order=2 -interp=e -ml=3 -factor=8 
#     ogen noplot cicSplit -order=2 -interp=e -ml=4 -factor=16
#     ogen noplot cicSplit -order=2 -interp=e -ml=4 -factor=32  (1.7M)
#     ogen noplot cicSplit -order=2 -interp=e -ml=5 -factor=64  (6.8M)
#     ogen noplot cicSplit -order=2 -interp=e -ml=5 -factor=128 (26M)
# 
#     ogen noplot cicSplit -order=4 -interp=e -ml=2 -factor=2 
#     ogen noplot cicSplit -order=4 -interp=e -ml=3 -factor=4 
#     ogen noplot cicSplit -order=4 -interp=e -ml=3 -factor=8 
#     ogen noplot cicSplit -order=4 -interp=e -ml=4 -factor=16
#     ogen noplot cicSplit -order=4 -interp=e -ml=4 -factor=32
#     ogen noplot cicSplit -order=4 -interp=e -ml=5 -factor=64
#     ogen noplot cicSplit -order=4 -interp=e -ml=6 -factor=128
# 
#   -- multigrid and stretched
#     ogen noplot cicSplit -order=4 -interp=e -ml=2 -blf=2 -factor=2 
#     ogen noplot cicSplit -order=4 -interp=e -ml=3 -blf=2 -factor=4 
#     ogen noplot cicSplit -order=4 -interp=e -ml=3 -blf=2 -factor=8 
#     ogen noplot cicSplit -order=4 -interp=e -ml=4 -blf=2 -factor=16
#     ogen noplot cicSplit -order=4 -interp=e -ml=4 -blf=2 -factor=32
#     ogen noplot cicSplit -order=4 -interp=e -ml=5 -blf=2 -factor=64
#     ogen noplot cicSplit -order=4 -interp=e -ml=6 -blf=2 -factor=128
# 
#   -- multigrid and fixed radial distance
#     ogen noplot cicSplit -order=4 -interp=e -ml=1 -rgd=fixed -factor=2  
#     ogen noplot cicSplit -order=4 -interp=e -ml=2 -rgd=fixed -factor=4 
#     ogen noplot cicSplit -order=4 -interp=e -ml=3 -rgd=fixed -factor=8 
#     ogen noplot cicSplit -order=4 -interp=e -ml=4 -rgd=fixed -factor=16
#     ogen noplot cicSplit -order=4 -interp=e -ml=5 -rgd=fixed -factor=32
#     ogen noplot cicSplit -order=4 -interp=e -ml=6 -rgd=fixed -factor=64
# 
#     ogen noplot cicSplit -factor=2 -order=2 -interp=e -xa=-4. -xb=4. -name="cice2L8.hdf"
#     ogen noplot cicSplit -factor=4 -order=2 -interp=e -xa=-4. -xb=4. -name="cice4L8.hdf"
#
#   -- long channel for flow past a cylinder (with multigrid levels and boundary layer stretching)
#     ogen noplot cicSplit -order=2 -interp=e -xa=-2. -xb=10. -ya=-3.0 -yb=3.0 -ml=2 -factor=2  -blf=4 -name="cicLongChannele2.hdf"
#     ogen noplot cicSplit -order=2 -interp=e -xa=-2. -xb=10. -ya=-3.0 -yb=3.0 -ml=3 -factor=4  -blf=4 -name="cicLongChannele4.hdf"
#     ogen noplot cicSplit -order=2 -interp=e -xa=-2. -xb=10. -ya=-3.0 -yb=3.0 -ml=4 -factor=8  -blf=4 -name="cicLongChannele8.hdf"
#     ogen noplot cicSplit -order=2 -interp=e -xa=-2. -xb=10. -ya=-3.0 -yb=3.0 -ml=4 -factor=16 -blf=4 -name="cicLongChannele16.hdf"
# 
#     -- for diffraction problem
#     ogen noplot cicSplit -order=2 -interp=e -xa=-3. -xb=3. -ya=-3. -yb=3. -factor=2 -name="cicex3y3f2.hdf"
#     ogen noplot cicSplit -order=2 -interp=e -xa=-3. -xb=3. -ya=-3. -yb=3. -factor=4 -name="cicex3y3f4.hdf"
#     ogen noplot cicSplit -order=2 -interp=e -xa=-3. -xb=3. -ya=-3. -yb=3. -factor=8 -name="cicex3y3f8.hdf"
#     ogen noplot cicSplit -order=2 -interp=e -xa=-3. -xb=3. -ya=-3. -yb=3. -factor=16 -name="cicex3y3f16.hdf"
#     ogen noplot cicSplit -order=2 -interp=e -xa=-3. -xb=3. -ya=-3. -yb=3. -factor=32 -name="cicex3y3f32.hdf"
#     ogen noplot cicSplit -order=2 -interp=e -xa=-3. -xb=3. -ya=-3. -yb=3. -factor=64 -name="cicex3y3f64.hdf"
#     ogen noplot cicSplit -order=2 -interp=e -xa=-3. -xb=3. -ya=-3. -yb=3. -factor=128 -name="cicex3y3f128.hdf"
# 
# -- parallel bug: holeWidth large: (np=1 ok, np=2 BAD, np=4 ok)
#  mpirun -np 2 $ogenp noplot cicSplit -order=2 -interp=e -ml=2 -factor=2 -cx=-.5 -cy=.0
#
# srun -N 1 -n 4 -ppdebug $ogenp noplot cicSplit -order=2 -interp=e -factor=100 (16M)
# srun -N 12 -n 64 -ppdebug $ogenp noplot cicSplit -order=2 -interp=e -factor=500 (400M, 500M)
# srun -N 12 -n 64 -ppdebug $ogenp noplot cicSplit -order=2 -interp=e -factor=700 (784M, 560M)
# srun -N 12 -n 96 -ppdebug $ogenp noplot cicSplit -order=2 -interp=e -factor=900 (1.3B, 650M)
# srun -N 12 -n 96 -ppdebug $ogenp noplot cicSplit -order=2 -interp=e -factor=1000 (1.6B, 800M)
# srun -N 12 -n 96 -ppdebug $ogenp noplot cicSplit -order=2 -interp=e -factor=1500 (all=3.6B, ave=1G, mx=2G ?, 83s)
#
$prefix="cicSplit";  $rgd="var";
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; 
$cx=0.; $cy=0.;  # center for the annulus
$blf=1;  # this means no stretching
$deltaRadius0=.3; # radius for rgd fixed
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf, "prefix=s"=> \$prefix,\
            "cx=f"=>\$cx,"cy=f"=>\$cy,"rgd=s"=> \$rgd );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; }
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
Annulus
  # Make sure there are at least 4 points on the coarsest MG level
  $nr = max( 5+ $ng + 2*($order-2), 2**($ml+2) );
  $nr = intmg( $nr );
  $innerRad=.5; $outerRad = $innerRad + ($nr-1)*$ds;
  if( $rgd eq "fixed" ){ $outerRad = $innerRad + $deltaRadius0; $nr=intmg( $deltaRadius0/$ds + 2.5 ); }
  center: $cx $cy
  inner and outer radii
    $innerRad $outerRad
  start and end angles
    -.05 .55
  lines
    if( $blf>1 ){ $nr = $nr + 4; } # extra grid lines to account for stretching
    $nTheta = intmg( .6*2.*$pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    0 0 5 0
  mappingName
   annulus1
exit
Annulus
  # Make sure there are at least 4 points on the coarsest MG level
  $nr = max( 5+ $ng + 2*($order-2), 2**($ml+2) );
  $nr = intmg( $nr );
  $innerRad=.5; $outerRad = $innerRad + ($nr-1)*$ds;
  if( $rgd eq "fixed" ){ $outerRad = $innerRad + $deltaRadius0; $nr=intmg( $deltaRadius0/$ds + 2.5 ); }
  center: $cx $cy
  inner and outer radii
    $innerRad $outerRad
  start and end angles
    .5 1.
  lines
    if( $blf>1 ){ $nr = $nr + 4; } # extra grid lines to account for stretching
    $nTheta = intmg( .6*2.*$pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    0 0 5 0
  mappingName
   annulus2
exit
#
exit
generate an overlapping grid
    square
    annulus1
    annulus2
  done
  change parameters
    # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ngp = $ng+1;
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
cicSplit
exit







*
* check an annulus on a fine grid -- for hole cutting
*
create mappings
*
rectangle
  set corners
    -2. 2. -2. 2.
  lines
    25 25 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  start and end angles
    -.05 .55
  lines
    17 9
  boundary conditions
    0 0  1 0
  share
     0 0 1 0
  mappingName
   left
exit
*
Annulus
  start and end angles
    .5 1.
  lines
    17 9
  boundary conditions
    0 0  1 0
  share
     0 0 1 0
  mappingName
   right
exit
*
exit
generate an overlapping grid
    square
    left
    right
  done
  change parameters
    * choose implicit or explicit interpolation
    * interpolation type
    *   implicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
*  display intermediate results
  compute overlap
  exit
*
save an overlapping grid
cicSplit.hdf
cic
exit



  display intermediate results
*  debug 
*    7
  compute overlap
  continue

