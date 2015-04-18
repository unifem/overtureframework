#
# Create the initial grid for a deforming circle.
# Use this grid with cg/cns/cmd circleDeform.cmd 
#
# Examples:
#   ogen -noplot circleDeformGrid -interp=e -factor=1
#   ogen -noplot circleDeformGrid -interp=e -factor=2
#   ogen -noplot circleDeformGrid -interp=e -factor=4
#   ogen -noplot circleDeformGrid -interp=e -ya=-3 -yb=3 -xb=8 -factor=5
#
#
#
$prefix="circleDeformGrid"; 
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
$ds0 = 1./20.; 
# target grid spacing:
$ds = $ds0/$factor;
#
create mappings
  annulus
    $width=.25; 
    $innerRadius=.5; $outerRadius=$innerRadius + $width/$factor; 
    $averageRadius=($innerRadius+$outerRadius)/2.; 
    inner and outer radii
      $innerRadius $outerRadius
    start and end angles
     -.3333 .3333
    $nr = int( 2.*$pi*$averageRadius*(2./3.)/$ds+1.5 ); 
    $ns = int( $width/$ds0 +1.5 );
    lines
      $nr $ns
    boundary conditions
      0 0 5 0
    share
      0 0 5 0
    mappingName
      annulus
    exit
#
  rectangle
    $nx=int( ($xb-$xa)/$ds+1.5 ); 
    $ny=int( ($yb-$ya)/$ds+1.5 ); 
    set corners
      $xa $xb $ya $yb 
    lines
      $nx $ny 
    boundary conditions
      1 2 3 4 
    mappingName
     backGround
    exit
#
  nurbs (curve)
    enter points
      13
    $rad=.5; 
    $x0=0.; $y0=1.*$rad;
    $theta=$pi* 5./180.; $x1=-$rad*sin($theta); $y1=$rad*cos($theta);
    $theta=$pi*10./180.; $x2=-$rad*sin($theta); $y2=$rad*cos($theta);
    $theta=$pi*15./180.; $x3=-$rad*sin($theta); $y3=$rad*cos($theta);
    $theta=$pi*20./180.; $x4=-$rad*sin($theta); $y4=$rad*cos($theta);
    $theta=$pi*50./180.; $x5=-$rad*sin($theta); $y5=$rad*cos($theta);
    $theta=$pi*90./180.; $x6=-$rad*sin($theta); $y6=$rad*cos($theta);
    $x0 $y0
    $x1 $y1
    $x2 $y2
    $x3 $y3
    $x4 $y4
    $x5 $y5
    $x6 $y6
    $y0=-$y0; $y1=-$y1; $y2=-$y2; $y3=-$y3; $y4=-$y4; $y5=-$y5;
    $x5 $y5
    $x4 $y4
    $x3 $y3
    $x2 $y2
    $x1 $y1
    $x0 $y0
    lines
      31
# pause
    exit
  hyperbolic
 # add a few extra points as the boundary deforms it gets longer
    $stretchFactor=1.25; 
    $nr = int( 2.*$pi*$stretchFactor*$averageRadius*(.5)/$ds+1.5 ); 
    $dist = $width/$factor;     
    distance to march $dist 
    lines to march $ns
    points on initial curve $nr
    generate
    boundary conditions
      0 0 5 0
    share
      0 0 5 0
    name ice
    exit
  exit this menu
#
generate an overlapping grid
  backGround
  annulus
  ice
  done choosing mappings
  change parameters
    shared sides may cut holes
      ice
      annulus
      done
    ghost points
      all
      2 2 2 2 2 2
    exit
#
  compute overlap
#
exit
#
save an overlapping grid
  $name
  circleDeformGrid
exit
