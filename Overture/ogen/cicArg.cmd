#
# Circle in a channel (taking arguments)
#
#
# usage: ogen [-noplot] cicArg -factor=<num> -order=[2/4/6/8] -interp=[e/i] -blf=<num> -ml=<>  -rgd=[fixed|var] ...
#                             -xa=<> -xb=<> -ya=<> -yb=<> -cx=<> -cy=<> -numGhost=<i>
# 
#  -blf : boundary-layer-factor : blf>1 : make grid lines near boundary this many times smaller
#  -ml = number of (extra) multigrid levels to support
#  -rgd : var=variable : decrease radial grid distance as grids are refined. fixed=fix radial grid distance
#  -xa, -xb, -ya, -yb : bounds on the back ground grid
#  -cx, -cy : center for the annulus
# 
# examples:
#     ogen -noplot cicArg -order=2 -factor=1
#     ogen -noplot cicArg -order=4 -factor=1
#     ogen -noplot cicArg -order=4 -factor=2
# 
#     ogen -noplot cicArg -order=2 -interp=e -factor=2   ( creates cice2.order2.hdf)
#     ogen -noplot cicArg -order=2 -interp=e -factor=3   ( creates cice3.order2.hdf)
#     ogen -noplot cicArg -order=2 -interp=e -factor=4   ( creates cice4.order2.hdf)
#     ogen -noplot cicArg -order=2 -interp=e -factor=16    
#     ogen -noplot cicArg -order=2 -interp=e -factor=32    
#     ogen -noplot cicArg -order=2 -interp=e -factor=64   (6.2M pts)
#     ogen -noplot cicArg -order=2 -interp=e -factor=128  (25M pts)
# 
#     ogen -noplot cicArg -order=4 -interp=e -factor=2 
#     ogen -noplot cicArg -order=4 -interp=e -factor=3 
#     ogen -noplot cicArg -order=4 -interp=e -factor=4 
#     ogen -noplot cicArg -order=4 -interp=e -factor=16    
#     ogen -noplot cicArg -order=4 -interp=e -factor=32    
#     ogen -noplot cicArg -order=4 -interp=e -factor=64 
# 
#   --periodic square
#     ogen -noplot cicArg -order=2 -interp=e -factor=2 -bcSquare=p
#     ogen -noplot cicArg -order=2 -interp=e -factor=4 -bcSquare=p
# 
#   -- fixed radial distance
#     ogen -noplot cicArg -order=2 -interp=e -rgd=fixed -factor=1
#     ogen -noplot cicArg -order=2 -interp=e -rgd=fixed -factor=2  
#     ogen -noplot cicArg -order=2 -interp=e -rgd=fixed -factor=4 
#     ogen -noplot cicArg -order=2 -interp=e -rgd=fixed -factor=8 
#     ogen -noplot cicArg -order=2 -interp=e -rgd=fixed -factor=16
#     ogen -noplot cicArg -order=2 -interp=e -rgd=fixed -factor=32
#     ogen -noplot cicArg -order=2 -interp=e -rgd=fixed -factor=64
# 
#   -- fixed radial distance + order 4
#     ogen -noplot cicArg -order=4 -interp=e -rgd=fixed -factor=2  
#     ogen -noplot cicArg -order=4 -interp=e -rgd=fixed -factor=4 
#     ogen -noplot cicArg -order=4 -interp=e -rgd=fixed -factor=8 
#     ogen -noplot cicArg -order=4 -interp=e -rgd=fixed -factor=16
#     ogen -noplot cicArg -order=4 -interp=e -rgd=fixed -factor=32
#     ogen -noplot cicArg -order=4 -interp=e -rgd=fixed -factor=64
# 
#   -- multigrid
#     ogen -noplot cicArg -order=2 -interp=e -ml=2 -factor=1
#     ogen -noplot cicArg -order=2 -interp=e -ml=2 -factor=2 
#     ogen -noplot cicArg -order=2 -interp=e -ml=3 -factor=4 
#     ogen -noplot cicArg -order=2 -interp=e -ml=3 -factor=8 
#     ogen -noplot cicArg -order=2 -interp=e -ml=4 -factor=16
#     ogen -noplot cicArg -order=2 -interp=e -ml=4 -factor=32  (1.7M)
#     ogen -noplot cicArg -order=2 -interp=e -ml=5 -factor=64  (6.8M)
#     ogen -noplot cicArg -order=2 -interp=e -ml=5 -factor=128 (26M)
# 
#     ogen -noplot cicArg -order=4 -interp=e -ml=2 -factor=2 
#     ogen -noplot cicArg -order=4 -interp=e -ml=3 -factor=4 
#     ogen -noplot cicArg -order=4 -interp=e -ml=3 -factor=8 
#     ogen -noplot cicArg -order=4 -interp=e -ml=4 -factor=16
#     ogen -noplot cicArg -order=4 -interp=e -ml=4 -factor=32
#     ogen -noplot cicArg -order=4 -interp=e -ml=5 -factor=64
#     ogen -noplot cicArg -order=4 -interp=e -ml=6 -factor=64
#     ogen -noplot cicArg -order=4 -interp=e -ml=6 -factor=128
# 
#   -- multigrid and stretched
#     ogen -noplot cicArg -order=4 -interp=e -ml=2 -blf=2 -factor=2 
#     ogen -noplot cicArg -order=4 -interp=e -ml=3 -blf=2 -factor=4 
#     ogen -noplot cicArg -order=4 -interp=e -ml=3 -blf=2 -factor=8 
#     ogen -noplot cicArg -order=4 -interp=e -ml=4 -blf=2 -factor=16
#     ogen -noplot cicArg -order=4 -interp=e -ml=4 -blf=2 -factor=32
#     ogen -noplot cicArg -order=4 -interp=e -ml=5 -blf=2 -factor=64
#     ogen -noplot cicArg -order=4 -interp=e -ml=6 -blf=2 -factor=128
# 
#   -- multigrid and fixed radial distance
#     ogen -noplot cicArg -order=4 -interp=e -ml=1 -rgd=fixed -factor=2  
#     ogen -noplot cicArg -order=4 -interp=e -ml=2 -rgd=fixed -factor=4 
#     ogen -noplot cicArg -order=4 -interp=e -ml=3 -rgd=fixed -factor=8 
#     ogen -noplot cicArg -order=4 -interp=e -ml=4 -rgd=fixed -factor=16
#     ogen -noplot cicArg -order=4 -interp=e -ml=5 -rgd=fixed -factor=32
#     ogen -noplot cicArg -order=4 -interp=e -ml=6 -rgd=fixed -factor=64
# 
#     ogen -noplot cicArg -factor=2 -order=2 -interp=e -xa=-4. -xb=4. -name="cice2L8.hdf"
#     ogen -noplot cicArg -factor=4 -order=2 -interp=e -xa=-4. -xb=4. -name="cice4L8.hdf"
#
#   -- long channel for flow past a cylinder (with multigrid levels and boundary layer stretching)
#     ogen -noplot cicArg -order=2 -interp=e -xa=-2. -xb=10. -ya=-3.0 -yb=3.0 -ml=2 -factor=2  -blf=4 -name="cicLongChannele2.hdf"
#     ogen -noplot cicArg -order=2 -interp=e -xa=-2. -xb=10. -ya=-3.0 -yb=3.0 -ml=3 -factor=4  -blf=4 -name="cicLongChannele4.hdf"
#     ogen -noplot cicArg -order=2 -interp=e -xa=-2. -xb=10. -ya=-3.0 -yb=3.0 -ml=4 -factor=8  -blf=4 -name="cicLongChannele8.hdf"
#     ogen -noplot cicArg -order=2 -interp=e -xa=-2. -xb=10. -ya=-3.0 -yb=3.0 -ml=4 -factor=16 -blf=4 -name="cicLongChannele16.hdf"
# 
#     -- for diffraction problem
#     ogen -noplot cicArg -order=2 -interp=e -xa=-3. -xb=3. -ya=-3. -yb=3. -factor=2 -name="cicex3y3f2.hdf"
#     ogen -noplot cicArg -order=2 -interp=e -xa=-3. -xb=3. -ya=-3. -yb=3. -factor=4 -name="cicex3y3f4.hdf"
#     ogen -noplot cicArg -order=2 -interp=e -xa=-3. -xb=3. -ya=-3. -yb=3. -factor=8 -name="cicex3y3f8.hdf"
#     ogen -noplot cicArg -order=2 -interp=e -xa=-3. -xb=3. -ya=-3. -yb=3. -factor=16 -name="cicex3y3f16.hdf"
#     ogen -noplot cicArg -order=2 -interp=e -xa=-3. -xb=3. -ya=-3. -yb=3. -factor=32 -name="cicex3y3f32.hdf"
#     ogen -noplot cicArg -order=2 -interp=e -xa=-3. -xb=3. -ya=-3. -yb=3. -factor=64 -name="cicex3y3f64.hdf"
#     ogen -noplot cicArg -order=2 -interp=e -xa=-3. -xb=3. -ya=-3. -yb=3. -factor=128 -name="cicex3y3f128.hdf"
# 
# -- more ghost
#     ogen -noplot cicArg -order=4 -interp=e -numGhost=3 -factor=2 
#     ogen -noplot cicArg -order=4 -interp=e -numGhost=3 -factor=4 
#     ogen -noplot cicArg -order=4 -interp=e -numGhost=3 -factor=8 
#     -- fixed radial
#     ogen -noplot cicArg -order=4 -interp=e -numGhost=3 -rgd=fixed -factor=2 
#     ogen -noplot cicArg -order=4 -interp=e -numGhost=3 -rgd=fixed -factor=4
# 
#     ogen -noplot cicArg -order=6 -interp=e -numGhost=4 -factor=2 
#     ogen -noplot cicArg -order=6 -interp=e -numGhost=4 -factor=4 
#     ogen -noplot cicArg -order=6 -interp=e -numGhost=4 -factor=8 
#
# -- parallel bug: holeWidth large: (np=1 ok, np=2 BAD, np=4 ok)
#  mpirun -np 2 $ogenp -noplot cicArg -order=2 -interp=e -ml=2 -factor=2 -cx=-.5 -cy=.0
#
#  mpirun -np 2 ./ogen --noplot cicArg -order=2 -interp=e -factor=8
#
# srun -N 1 -n 4 -ppdebug $ogenp -noplot cicArg -order=2 -interp=e -factor=100 (16M)
# srun -N 12 -n 64 -ppdebug $ogenp -noplot cicArg -order=2 -interp=e -factor=500 (400M, 500M)
# srun -N 12 -n 64 -ppdebug $ogenp -noplot cicArg -order=2 -interp=e -factor=700 (784M, 560M)
# srun -N 12 -n 96 -ppdebug $ogenp -noplot cicArg -order=2 -interp=e -factor=900 (1.3B, 650M)
# srun -N 12 -n 96 -ppdebug $ogenp -noplot cicArg -order=2 -interp=e -factor=1000 (1.6B, 800M)
# srun -N 12 -n 96 -ppdebug $ogenp -noplot cicArg -order=2 -interp=e -factor=1500 (all=3.6B, ave=1G, mx=2G ?, 83s)
#
$prefix="cic";  $rgd="var"; $bcSquare="d"; 
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; 
$cx=0.; $cy=0.;  # center for the annulus
$blf=1;  # this means no stretching
$deltaRadius0=.3; # radius for rgd fixed
$numGhost=-1;  # if this value is set, then use this number of ghost points
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf, "prefix=s"=> \$prefix,\
            "cx=f"=>\$cx,"cy=f"=>\$cy,"rgd=s"=> \$rgd,"bcSquare=s"=>\$bcSquare,"numGhost=i"=>\$numGhost );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=3; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=4; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; }
if( $bcSquare eq "p" ){ $prefix = $prefix . "p"; }
$suffix = ".order$order"; 
if( $numGhost ne -1 ){ $ng = $numGhost; } # overide number of ghost
if( $numGhost ne -1 ){ $suffix .= ".ng$numGhost"; } 
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
    $sbc="1 2 3 4";
    if( $bcSquare eq "p" ){ $sbc = "-1 -1 3 4"; }
    $sbc
  mappingName
  square
exit
#
# if 
if( $blf>1 ){ $annulusName="AnnulusUnStretched"; $stretchAnnulusName="Annulus"; }else{ $annulusName="Annulus"; $stretchAnnulusName="AnnulusStretched"; }
Annulus
  # Make sure there are at least 4 points on the coarsest MG level
  $nr = max( 5+ $ng + 2*($order-2), 2**($ml+2) );
  $nr = intmg( $nr );
  $innerRad=.5; $outerRad = $innerRad + ($nr-1)*$ds;
  if( $rgd eq "fixed" ){ $outerRad = $innerRad + $deltaRadius0; $nr=intmg( $deltaRadius0/$ds + 2.5 ); }
  center: $cx $cy
  inner and outer radii
    $innerRad $outerRad
  lines
    if( $blf>1 ){ $nr = $nr + 4; } # extra grid lines to account for stretching
    $nTheta = intmg( 2.*$pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
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
  #Stretch r2:itanh
  #STP:stretch r2 itanh: position and min dx 0 $dxMin
  #stretch grid
  STRT:name $stretchAnnulusName
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
      # $ngp = $ng+1;
      $ngp = $ng;
      $ng $ng $ng $ngp $ng $ng
  exit
#  display intermediate results
  compute overlap
# plot
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

