#
#  solid pill-box in a channel
# 
# Usage:
#    ogen [-noplot] solidPillBoxGrid.cmd -factor=<> -interp=[e|i] -order=<> -ml=<> -widthX=<> -widthY=<> -widthZ=<> ...
#           -blf=<> -xa=<> -xb=<> -ya=<> -yb=<> -za=<> -zb=<> -refinementBox=[0|1]
# Options:
#   -xa, -xb, -ya, -yb, -za, -zb : bounding box
#   -blf : boundary layer stretching factor
#
#
#                   +----------------+
#                   |                |
#                   |                |
#                   +----------------+
#
# Examples:
#
#  ogen -noplot solidPillBoxGrid.cmd -interp=e -factor=4 
#
#
#
$thickness=1./3.;  # thickness of the pill-box 
#
$sharpnessLB=15.;                     # corner sharpness
$tStretch=2.; 
$nrMin=5; 
$widthX=1.; $widthY=1.; $widthZ=1.;   # box size 
$blf=5.; # boundary layer stretching factor
$rotateX=0.; $rotateY=0.; $rotateZ=0.; # rotation (degrees) about X followed by Y followed by Z axis
#
# Background grid:
$xa=-2.; $xb=2.; $ya=-.75; $yb=.75; $za=-.75; $zb=.75; 
# Refinement patch 
## $xar=-.9; $xbr=1.5; $yar=-.9; $ybr=.9; $zar=-.9; $zbr=.9; 
#
$order=2; $factor=1; $interp = "i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$extra=0;  # extra overlap needed for higher order
$factorNurbs=1.; # factor for the Nurbs representation 
$refinementBox=0; 
$name=""; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"blf=f"=>\$blf,"refinementBox=i"=>\$refinementBox,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"sharpnessLB=f"=> \$sharpnessLB,\
            "widthX=f"=> \$widthX,"widthY=f"=> \$widthY,"widthZ=f"=> \$widthZ,\
            "xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,"za=f"=>\$za,"zb=f"=>\$zb,\
            "xac=f"=>\$xac,"xbc=f"=>\$xbc,"yac=f"=>\$yac,"ybc=f"=>\$ybc,"zac=f"=>\$zac,"zbc=f"=>\$zbc,\
            "thickness=f"=>\$thickness,"nrMin=i"=>\$nrMin  );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
$extra=$order-2;
if( $interp eq "e" ){ $extra=$extra+2; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = "solidPillBoxGrid" . "$interp$factor" . $suffix . ".hdf";}
# 
#
# NOTE: x-bounds and y-bounds should be centered around 0: 
$xalb=-$widthX*.5; $xblb=$widthX*.5; $yalb=-$widthY*.5; $yblb=$widthY*.5; $zalb=-$widthZ*.5; $zblb=$widthZ*.5;   # lofted box bounds 
#
$ds=.1/$factor;
$dsn = .1/$factorNurbs; # build Nurbs representation with this grid spacing 
$dsBL = $ds/$blf; # boundary layer spacing (spacing in the normal direction)
# 
$dw = $order+1; $iw=$order+1; 
#
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
sub min{ local($n,$m)=@_; if( $n<$m ){ return $n; }else{ return $m; } }
#
$pi = 4.*atan2(1.,1.);
#
# NOTE: do NOT extend the grid too far since the core region needs to background grid in it.
# nr = number of lines in normal directions to boundaries
$nr = max( 7 + $ng + 2*($order-2), 2**($ml) );
$nr = intmg( $nr );
#
$wallBC=7;   # BC for walls of the box 
$wallShare=7; 
# 
create mappings
#
#  Cross-section for the pill
#
  smoothedPolygon
    vertices
      6
      # -- rb = outer radius of pill-box
      #    ra = edge grid goes to r=ra on the top and bottom
      $ra=.25-$extra*$ds; $rb=.5;   # inner and outer radius of curved edge grid of the pill
      $r1=$ra+.5*($rb-$ra); 
      $halfThickness=.5*$thickness; 
      $ra -$halfThickness
      $r1 -$halfThickness
      $rb -$halfThickness
      $rb  $halfThickness
      $r1  $halfThickness
      $ra  $halfThickness
#     n-dist
#       # 
#       fixed normal distance
#         -$nDist
    curve or area (toggle)
    lines
      $stretchFactor=1.5; # add more lines in the tangential direction due to stretching at corners
      $length=2.*($rb-$ra)+$thickness; # approx. perimeter length 
      $ns = intmg( $stretchFactor*$length/$ds +1.5 ); 
      $ns $nr
    sharpness
      $sharpnessLB
      $sharpnessLB
      $sharpnessLB
      $sharpnessLB
      $sharpnessLB
      $sharpnessLB
    n-stretch
      # note -- normal stretching is done below
      $nStretch=4.; 
      0. $nStretch 0.
    t-stretch
      0. 1.
      0. 1.
      .25  $tStretch
      .25  $tStretch
      0. 1.
      0. 1.
#
    mappingName
      pillCrossSection
    exit
#
#   Grid around the edge
#
  body of revolution
    choose a point on the line to revolve about
      0 0 0
    lines
      $rad = $ra;
      $nTheta = intmg( 2.*$pi*$rad/$ds +1.5 );
      $ns $nTheta 
    boundary conditions
      7 0 -1 -1
    share
      7 0 0 0
    mappingName
      pillEdgeSurface
    exit
#
# -- create grids around the outer ring --
$nr = intmg( max($nrMin,5) );
$nDist = ($nr- 1)*$ds; 
#
  builder...
    add surface grid
      pillEdgeSurface
    #
    target grid spacing $ds $ds (tang,norm)((<0 : use default)
    #   --- outer ring grid ---
    create volume grid...
      backward
      lines to march $nr
      generate
      boundary conditions
        0 0 -1 -1 7   0 
      share
        0 0  0  0 100 0 
      name outerPillEdge0
     exit
    #   --- inner ring grid ---
    create volume grid...
      forward
      lines to march $nr
      generate
      boundary conditions
        0 0 -1 -1 7   0 
      share
        0 0  0  0 100 0 
      name innerPillEdge0
    exit
  exit
#   
#  --- make box grids that sit on the center of the pill-box --- 
#
  box
    set corners
      $xbc=$ra+$extra*$ds; $xac=-$xbc; $yac=$halfThickness; $ybc=$yac+$nDist; 
      $xac $xbc $yac $ybc $xac $xbc
    lines
      # 13 6 13
      $nx = intmg( ($xbc-$xac)/$ds + 1.5 );
      $ny = intmg( ($ybc-$yac)/$ds + 1.5 );
      $nx $ny $nx 
    boundary conditions
      0 0 7 0 0 0
    share
      0 0 100 0 0 0
    mappingName
      outerTop0
    exit
#
  box
    set corners
      $xbc=$ra+$extra*$ds; $xac=-$xbc; $ybc=$halfThickness; $yac=$ybc-$nDist; 
      $xac $xbc $yac $ybc $xac $xbc
    lines
      # 13 6 13
      $nx = intmg( ($xbc-$xac)/$ds + 1.5 );
      $ny = intmg( ($ybc-$yac)/$ds + 1.5 );
      $nx $ny $nx 
    boundary conditions
      0 0 0 7 0 0
    share
      0 0 0 100 0 0
    mappingName
      innerTop0
    exit
#
  box
    set corners
      $ybc=-$halfThickness;   $yac=$ybc-$nDist; 
      $xac $xbc $yac $ybc $xac $xbc
    lines
      $nx $ny $nx
    boundary conditions
      0 0 0 7 0 0
    share
      0 0 0 100 0 0
    mappingName
      outerBottom0
    exit
#
  box
    set corners
      $yac=-$halfThickness;   $ybc=$ybc+$nDist; 
      $xac $xbc $yac $ybc $xac $xbc
    lines
      $nx $ny $nx
    boundary conditions
      0 0 7 0 0 0
    share
      0 0 100 0 0 0
    mappingName
      innerBottom0
    exit
#
#  Background grid 
#
Box
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nz = intmg( ($zb-$za)/$ds +1.5 ); 
    $nx $ny $nz
  boundary conditions
    1 2 3 4 5 6 
  share
    0 0 0 0 0 0 
  mappingName
    backGround
  exit
#
#  -- inner background ---
#
  box
    set corners
      $xbc=$rb-2.*$ds; $xac=-$xbc;
      $yac=-$halfThickness+2*$ds;   $ybc=$halfThickness-2*$ds;
      $xac $xbc $yac $ybc $xac $xbc
    lines
      $nx $ny $nx
    boundary conditions
      0 0 0 0 0 0
    share
      0 0 0 0 0 0
    mappingName
      innerBackGround0
    exit
#
#
#  --- Convert to Nurbs and Rotate --- 
#
# Define a subroutine to convert a Mapping to a Nurbs Mapping
sub convertToNurbs\
{ local($old,$new,$angle,$rotationAxis,$xShift,$yShift,$zShift)=@_; \
  $cmds = "nurbs \n" . \
   "interpolate from mapping with options\n" . \
   " $old \n" . \
   " parameterize by index (uniform)\n" . \
   " number of ghost points to include\n $numGhost\n" . \
   " choose degree\n" . \
   "  3 \n" . \
   "done\n" . \
   "rotate \n" . \
   " $angle $rotationAxis \n" . \
   " 0. 0. 0.\n" . \
   "shift\n" . \
   " $xShift $yShift $zShift\n" . \
   "mappingName\n" . \
   " $new\n" . \
   "exit"; \
}
#
$numGhost=$ng; 
$angle=90.; $rotationAxis=2; $xShift=0.; $yShift=0.; $zShift=0.; 
convertToNurbs(outerTop0,outerTop,$angle,$rotationAxis,$xShift,$yShift,$zShift);
$cmds
convertToNurbs(outerBottom0,outerBottom,$angle,$rotationAxis,$xShift,$yShift,$zShift);
$cmds
convertToNurbs(outerPillEdge0,outerPillEdge,$angle,$rotationAxis,$xShift,$yShift,$zShift);
$cmds
convertToNurbs(innerTop0,innerTop,$angle,$rotationAxis,$xShift,$yShift,$zShift);
$cmds
convertToNurbs(innerBottom0,innerBottom,$angle,$rotationAxis,$xShift,$yShift,$zShift);
$cmds
convertToNurbs(innerPillEdge0,innerPillEdge,$angle,$rotationAxis,$xShift,$yShift,$zShift);
$cmds
convertToNurbs(innerBackGround0,innerBackGround,$angle,$rotationAxis,$xShift,$yShift,$zShift);
$cmds
#
exit 
#
# Make the overlapping grid
#
generate an overlapping grid
  backGround
  outerTop
  outerBottom
  outerPillEdge
#
  innerBackGround
  innerTop
  innerBottom
  innerPillEdge
 done
 change parameters
   specify a domain
    outerDomain 
      # grids in the domain:
      backGround
      outerTop
      outerBottom
     outerPillEdge
    done
    specify a domain
      innerDomain 
      # grids in the domain:
       innerBackGround
       innerTop
       innerBottom
       innerPillEdge
    done
    # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
  # open graphics
  compute overlap
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
solidPillBoxGrid
exit







# ************* OLD STUFF ***********
* 
* #
* # Make the overlapping grid
* #
* generate an overlapping grid
*   innerBackGround
*   innerTop
*   innerBottom
*   innerPillEdge
*  done
*  change parameters
*     # choose implicit or explicit interpolation
*     interpolation type
*       $interpType
*     order of accuracy 
*       $orderOfAccuracy
*     ghost points
*       all
*       $ng $ng $ng $ng $ng $ng 
*   exit
*  open graphics
* 
* 
* 
* 
* 
* # 
*   stretch coordinates
*     transform which mapping?
*     pistonBottomPatch_unstretched
*     STRT:multigrid levels $ml
*     Stretch r2:exp to linear
*     STP:stretch r2 expl: position 1.
*     STP:stretch r2 expl: min dx, max dx $dsBL $ds
*     STRT:name pistonBottomPatch
*   exit
* #
* # Define a subroutine to convert a Mapping to a Nurbs Mapping
* sub convertToNurbs\
* { local($old,$new,$angle,$rotationAxis,$xShift,$yShift,$zShift)=@_; \
*   $cmds = "nurbs \n" . \
*    "interpolate from mapping with options\n" . \
*    " $old \n" . \
*    " parameterize by index (uniform)\n" . \
*    " number of ghost points to include\n $numGhost\n" . \
*    " choose degree\n" . \
*    "  3 \n" . \
*    " # number of points to interpolate\n" . \
*    " #  11 21 5 \n" . \
*    "done\n" . \
*    "rotate \n" . \
*    " $angle $rotationAxis \n" . \
*    " 0. 0. 0.\n" . \
*    "shift\n" . \
*    " $xShift $yShift $zShift\n" . \
*    "mappingName\n" . \
*    " $new\n" . \
*    "exit"; \
* }
* #
* $angle=0.; $rotationAxis=2; $xShift=0.; $yShift=0.; $zShift=0.; 
* # convertToNurbs(flattenedTorusReoriented,flattenedTorusNurbs,$angle,$rotationAxis,$xShift,$yShift,$zShift);
* convertToNurbs(pistonOuterRing,pistonOuterRingNurbs,$angle,$rotationAxis,$xShift,$yShift,$zShift);
* $cmds
* convertToNurbs(pistonTopPatch,pistonTopPatchNurbs,$angle,$rotationAxis,$xShift,$yShift,$zShift);
* $cmds
* convertToNurbs(pistonBottomPatch,pistonBottomPatchNurbs,$angle,$rotationAxis,$xShift,$yShift,$zShift);
* $cmds
* #- #
* #- #  Refinement box 
* #- #
* #- Box
* #-   set corners
* #-     $xar $xbr $yar $ybr $zar $zbr
* #-   lines
* #-     $nx = intmg( ($xbr-$xar)/$ds +1.5 ); 
* #-     $ny = intmg( ($ybr-$yar)/$ds +1.5 ); 
* #-     $nz = intmg( ($zbr-$zar)/$ds +1.5 ); 
* #-     $nx $ny $nz
* #-   boundary conditions
* #-     0 0 0 0 0 0 
* #-   share
* #-     0 0 0 0 0 0 
* #-   mappingName
* #-     refinementPatch
* #-   exit
* #- #
* #- #  Background grid 
* #- #
* Box
*   set corners
*     $xa $xb $ya $yb $za $zb
*   lines
*     if( $refinementBox eq 1 ){ $dsc=$ds*2.; }else{ $dsc=$ds; }
*     $nx = intmg( ($xb-$xa)/$dsc +1.5 ); 
*     $ny = intmg( ($yb-$ya)/$dsc +1.5 ); 
*     $nz = intmg( ($zb-$za)/$dsc +1.5 ); 
*     $nx $ny $nz
*   boundary conditions
*     1 2 3 4 5 6 
*   share
*     0 0 0 0 0 0 
*   mappingName
*     backGround
*   exit
* exit
* 
