#
# 2D circular disk inside a larger disk -- put two grids on the inner disk for testing AMP scheme
#
#
# usage: ogen [-noplot] splitDiskInDiskGrid.cmd -factor=<num> -order=[2/4/6/8] -interp=[e/i] -blf=<num> -ml=<>  -rgd=[fixed|var] ...
#                             -xa=<> -xb=<> -ya=<> -yb=<> -cx=<> -cy=<> -numGhost=<i>
# 
#  -blf : boundary-layer-factor : blf>1 : make grid lines near boundary this many times smaller
#  -ml = number of (extra) multigrid levels to support
#  -rgd : var=variable : decrease radial grid distance as grids are refined. fixed=fix radial grid distance
#  -xa, -xb, -ya, -yb : bounds on the back ground grid
#  -cx, -cy : center for the annulus
# 
# examples:
#     ogen -noplot splitDiskInDiskGrid -order=2 -interp=e -factor=1
#     ogen -noplot splitDiskInDiskGrid -order=2 -interp=e -factor=2
#     ogen -noplot splitDiskInDiskGrid -order=2 -interp=e -factor=4
#     ogen -noplot splitDiskInDiskGrid -order=2 -interp=e -factor=8
#     ogen -noplot splitDiskInDiskGrid -order=2 -interp=e -factor=16
#
# -- fixed radius of annular grids
#     ogen -noplot splitDiskInDiskGrid -order=2 -interp=e -rgd=fixed -factor=2
#     ogen -noplot splitDiskInDiskGrid -order=2 -interp=e -rgd=fixed -factor=4
#     ogen -noplot splitDiskInDiskGrid -order=2 -interp=e -rgd=fixed -factor=8
#     ogen -noplot splitDiskInDiskGrid -order=2 -interp=e -rgd=fixed -factor=16
#
# -- order=4
#     ogen -noplot splitDiskInDiskGrid -order=4 -interp=e -rgd=fixed -factor=4
#     ogen -noplot splitDiskInDiskGrid -order=4 -interp=e -rgd=fixed -factor=8
# 
# -- Grid using only annular grids (no background)
#     ogen -noplot splitDiskInDiskGrid -order=2 -interp=e -rgd=half -factor=2
# 
# Outer radius=3 
#     ogen -noplot splitDiskInDiskGrid -order=2 -interp=e -outerRadius=3 -prefix=splitDiskInDiskGridRad3 -factor=2
#     ogen -noplot splitDiskInDiskGrid -order=2 -interp=e -outerRadius=3 -prefix=splitDiskInDiskGridRad3 -rgd=fixed -factor=4
#
$prefix="splitDiskInDiskGrid";  $rgd="var"; $bcSquare="d"; 
$innerRadius=1.; $outerRadius=2.; 
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; 
$cx=0.; $cy=0.;  # center for the annulus
$blf=1;  # this means no stretching
$deltaRadius0=.3; # radius for rgd fixed
$numGhost=-1;  # if this value is set, then use this number of ghost points
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf, "prefix=s"=> \$prefix,\
            "cx=f"=>\$cx,"cy=f"=>\$cy,"rgd=s"=> \$rgd,"bcSquare=s"=>\$bcSquare,"numGhost=i"=>\$numGhost,\
            "innerRadius=f"=>\$innerRadius,"outerRadius=f"=>\$outerRadius, );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=3; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=4; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; }
if( $rgd eq "half" ){ $prefix = $prefix . "Half"; }
if( $rgd eq "half" ){ $deltaRadius0 = .5*($outerRadius-$innerRadius)+$ds*($ng-1);}
# 
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
# ********************* SQUARE BACK GROUND *************************
#
rectangle
  $xa=-$outerRadius; $xb=-$xa; $ya=$xa; $yb=$xb; 
  set corners
    $xa $xb $ya $yb
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    0 0 0 0 
  mappingName
   backGround
exit
#
# ********************* INNER ANNULUS GRID TOP *************************
#
if( $blf>1 ){ $annulusNameTop="diskUnStretchedTop"; $stretchdiskNameTop="innerDiskTop"; }else{ $annulusNameTop="innerDiskTop"; $stretchdiskNameTop="diskStretchedTop"; }
Annulus
  # Make sure there are at least 4 points on the coarsest MG level
  $nr = max( 5+ $ng + 2*($order-2), 2**($ml+2) );
  $nr = intmg( $nr );
  $innerRad=$innerRadius; $outerRad = $innerRad + ($nr-1)*$ds;
  if( $rgd eq "fixed" || $rgd eq "half" ){ $outerRad = $innerRad + $deltaRadius0; $nr=intmg( 1.1*$deltaRadius0/$ds + 2.5 ); }
  center: $cx $cy
  $endAngle = .5 + .05; 
  angles: 0, $endAngle
  inner and outer radii
    $innerRad $outerRad
  lines
    if( $blf>1 ){ $nr = $nr + 4; } # extra grid lines to account for stretching
    $nTheta = intmg( $pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    0 0  5 0
  share
     0  0 5 0
  mappingName
   $annulusNameTop
exit
#
# optionally stretch the grid lines next to the cylinder
# 
 stretch coordinates 
  transform which mapping? 
    $annulusNameTop 
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
  STRT:name $stretchdiskNameTop
 exit
 $innerDiskTop=$annulusNameTop;
#
#
# ********************* INNER ANNULUS GRID BOTTOM *************************
#
if( $blf>1 ){ $annulusNameBot="diskUnStretchedBot"; $stretchdiskNameBot="innerDiskBot"; }else{ $annulusNameBot="innerDiskBot"; $stretchdiskNameBot="diskStretchedBot"; }
Annulus
  # Make sure there are at least 4 points on the coarsest MG level
  $nr = max( 5+ $ng + 2*($order-2), 2**($ml+2) );
  $nr = intmg( $nr );
  $innerRad=$innerRadius; $outerRad = $innerRad + ($nr-1)*$ds;
  if( $rgd eq "fixed" || $rgd eq "half" ){ $outerRad = $innerRad + $deltaRadius0; $nr=intmg( 1.1*$deltaRadius0/$ds + 2.5 ); }
  center: $cx $cy
  $endAngle = 1. + .05;
  angles: .5 $endAngle
  inner and outer radii
    $innerRad $outerRad
  lines
    if( $blf>1 ){ $nr = $nr + 4; } # extra grid lines to account for stretching
    $nTheta = intmg( $pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    0 0  5 0
  share
     0  0 5 0
  mappingName
   $annulusNameBot
exit
#
# optionally stretch the grid lines next to the cylinder
# 
 stretch coordinates 
  transform which mapping? 
    $annulusNameBot 
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
  STRT:name $stretchdiskNameBot
 exit
 $innerDiskBot=$annulusNameBot;
#
#
#
# ********************* OUTER ANNULUS *************************
#
if( $blf>1 ){ $annulusName="outerDiskUnStretched"; $stretchouterDiskName="outerDisk"; }else{ $annulusName="outerDisk"; $stretchouterDiskName="outerDiskStretched"; }
Annulus
  # Make sure there are at least 4 points on the coarsest MG level
  $nr = max( 5+ $ng + 2*($order-2), 2**($ml+2) );
  $nr = intmg( $nr );
  $outerRad = $outerRadius; $innerRad=$outerRad - ($nr-1)*$ds; 
  if( $rgd eq "fixed" || $rgd eq "half" ){ $innerRad = $outerRad - $deltaRadius0; $nr=intmg( 1.1*$deltaRadius0/$ds + 4.5 ); }
  center: $cx $cy
  inner and outer radii
    $innerRad $outerRad
  lines
    if( $blf>1 ){ $nr = $nr + 4; } # extra grid lines to account for stretching
    $nTheta = intmg( 2.*$pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 0 6 
  share
     0  0 0 6 
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
  STRT:name $stretchouterDiskName
 exit
 $outerDisk=$annulusName;
#
#
exit
generate an overlapping grid
    if( $rgd eq "half" ){ $cmd="#"; }else{ $cmd="backGround"; }
    $cmd
    $innerDiskTop
    $innerDiskBot
    $outerDisk
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
# open graphics
#   display intermediate results
  compute overlap
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
# printf(" name=$name\n");
$name
cic
exit

