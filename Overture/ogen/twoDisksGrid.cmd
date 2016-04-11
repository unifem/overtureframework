#
# Two disks in a channel 
#
#
# usage: ogen [-noplot] twoDisksGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i] -blf=<num> -ml=<> ...
#                       -rgd=[fixed|var] -xa=<> -xb=<> -ya=<> -yb=<> -cx=<> -cy=<> -numGhost=<i>
# 
#  -blf : boundary-layer-factor : blf>1 : make grid lines near boundary this many times smaller
#  -ml = number of (extra) multigrid levels to support
#  -rgd : var=variable : decrease radial grid distance as grids are refined. fixed=fix radial grid distance
#  -xa, -xb, -ya, -yb : bounds on the back ground grid
#  -cx1, -cy1 : center for the annulus 1
#  -cx2, -cy2 : center for the annulus 2
# 
# examples:
#     ogen -noplot twoDisksGrid -order=2 -interp=e -factor=1
# 
#
$prefix="twoDisksGrid";  $rgd="var"; $bcSquare="d"; 
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; 
$cx1=-.7; $cy1=-.7;  # center for the annulus 1
$cx2= .7; $cy2=0.7;  # center for the annulus 2
$blf=1;  # this means no stretching
$deltaRadius0=.3; # radius for rgd fixed
$numGhost=-1;  # if this value is set, then use this number of ghost points
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf, "prefix=s"=> \$prefix,\
            "cx1=f"=>\$cx1,"cy1=f"=>\$cy1,"rgd=s"=> \$rgd,"bcSquare=s"=>\$bcSquare,"numGhost=i"=>\$numGhost );
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
# ------------------------------ Disk 1 -------------------------------
#
if( $blf>1 ){ $annulusName="AnnulusUnStretched"; $stretchAnnulusName="disk1"; }else{ $annulusName="disk1"; $stretchAnnulusName="AnnulusStretched"; }
Annulus
  # Make sure there are at least 4 points on the coarsest MG level
  $nr = max( 5+ $ng + 2*($order-2), 2**($ml+2) );
  $nr = intmg( $nr );
  $innerRad=.5; $outerRad = $innerRad + ($nr-1)*$ds;
  if( $rgd eq "fixed" ){ $outerRad = $innerRad + $deltaRadius0; $nr=intmg( $deltaRadius0/$ds + 2.5 ); }
  center: $cx1 $cy1
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
# ------------------------------ Disk 2 -------------------------------
#
if( $blf>1 ){ $annulusName2="AnnulusUnStretched2"; $stretchAnnulusName="disk2"; }else{ $annulusName2="disk2"; $stretchAnnulusName2="AnnulusStretched2"; }
Annulus
  # Make sure there are at least 4 points on the coarsest MG level
  $nr = max( 5+ $ng + 2*($order-2), 2**($ml+2) );
  $nr = intmg( $nr );
  $innerRad=.5; $outerRad = $innerRad + ($nr-1)*$ds;
  if( $rgd eq "fixed" ){ $outerRad = $innerRad + $deltaRadius0; $nr=intmg( $deltaRadius0/$ds + 2.5 ); }
  center: $cx2 $cy2
  inner and outer radii
    $innerRad $outerRad
  lines
    if( $blf>1 ){ $nr = $nr + 4; } # extra grid lines to account for stretching
    $nTheta = intmg( 2.*$pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 5 0
  share
     0  0 6 0
  mappingName
   $annulusName2
exit
#
# optionally stretch the grid lines next to the cylinder
# 
 stretch coordinates 
  transform which mapping? 
    $annulusName2
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
  STRT:name $stretchAnnulusName2
 exit
##
exit
generate an overlapping grid
    square
    disk1
    disk2
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
#   display intermediate results
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
# printf(" name=$name\n");
$name
twoDisks
exit
