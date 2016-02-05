#
#    Grid for a lofted quarter-box in a channel
#
# Usage:
#    ogen [-noplot] quarterBoxGrid.cmd -factor=<> -interp=[e|i] -order=<> -ml=<> -widthX=<> -widthY=<> -widthZ=<> ...
#           -blf=<> -xa=<> -xb=<> -ya=<> -yb=<> -za=<> -zb=<> -refinementBox=[0|1] -prefix=<s>
# Options:
#   -xa, -xb, -ya, -yb, -za, -zb : bounding box
#   -blf : boundary layer stretching factor
#   -refinementBox : 1=add a refinement box and coarsen the backGround grid
#
# Examples:
#
#  ogen -noplot quarterBoxGrid.cmd -interp=e -factor=2 
# 
#  ogen -noplot quarterBoxGrid.cmd -interp=e -order=4 -factor=2 
#
# -- multigrid:
#  ogen -noplot quarterBoxGrid.cmd -interp=e -factor=2 -ml=1
#  ogen -noplot quarterBoxGrid.cmd -interp=e -factor=4 -ml=1
#
#  ogen -noplot quarterBoxGrid.cmd -interp=e -order=4 -factor=2 -ml=1
#  ogen -noplot quarterBoxGrid.cmd -interp=e -order=4 -factor=4 -ml=1
#
#
# ******************** from loftedHalfBox:
#  ogen -noplot quarterBoxGrid.cmd -factor=2 
#  ogen -noplot quarterBoxGrid.cmd -factor=2 -widthX=2.
#  ogen -noplot quarterBoxGrid.cmd -factor=2 -widthZ=2.
# 
# -- multigrid:
#  ogen -noplot quarterBoxGrid.cmd -factor=2 -ml=1
# 
#  ogen -noplot quarterBoxGrid.cmd -interp=e -xa=-1. -xb=2.5 -factor=2 -ml=1
#  ogen -noplot quarterBoxGrid.cmd -interp=e -xa=-1. -xb=2.5 -xbc=5. -order=2 -factor=8 -ml=2
#  ogen -noplot quarterBoxGrid.cmd -interp=e -xa=-1. -xb=2.5 -xbc=5. -order=2 -factor=8 -ml=3
# -- fourth-order
#  ogen -noplot quarterBoxGrid.cmd -order=4 -interp=e -ml=1 -factor=2
#
#
#  ogen -noplot quarterBoxGrid.cmd -interp=i -xa=-1. -xb=2.5 -xbc=5. -order=4 -factor=2 -ml=1 
#  ogen -noplot quarterBoxGrid.cmd -interp=e -xa=-1. -xb=2.5 -xbc=5. -order=4 -factor=3 -ml=1 
#  ogen -noplot quarterBoxGrid.cmd -interp=e -xa=-1. -xb=2.5 -xbc=5. -order=4 -factor=3 -ml=2
#  ogen -noplot quarterBoxGrid.cmd -interp=e -xa=-1. -xb=2.5 -xbc=5. -order=4 -factor=4 -ml=2 
#  ogen -noplot quarterBoxGrid.cmd -interp=e -xa=-1. -xb=2.5 -xbc=5. -order=4 -factor=4 -ml=3
#  ogen -noplot quarterBoxGrid.cmd -interp=e -xa=-1. -xb=2.5 -xbc=5. -order=4 -factor=8 -ml=2
#  ogen -noplot quarterBoxGrid.cmd -interp=e -xa=-1. -xb=2.5 -xbc=5. -order=4 -factor=8 -ml=3
#
# TEST for valid grid:
#  ogen -noplot quarterBoxGrid.cmd -interp=e -refinementBox=0 -xa=-1. -xb=1.5 -xbc=1.5 -order=4 -factor=3 -ml=1 
#  ogen -noplot quarterBoxGrid.cmd -interp=e -refinementBox=0 -xa=-1. -xb=1.5 -xbc=1.5 -order=4 -factor=3 -ml=2
#  ogen -noplot quarterBoxGrid.cmd -interp=e -refinementBox=0 -xa=-1. -xb=1.5 -xac=-1.5 -yac=-1.5 -ybc=1.5 -xbc=1.5 -zac=-1.5 zbc=1.5 -order=4 -factor=3 -ml=2
# TESTS for Ogmg:
#   ogen -noplot quarterBoxGrid.cmd -interp=e -xac=-1.5 -xbc=1.5 -yac=-1.5 -ybc=1.5 -zac=-1.5 -zbc=1.5 -factor=2 -ml=2
# 
#  -- no refinement box:
#   ogen -noplot quarterBoxGrid.cmd -interp=e -refinementBox=0 -xac=-1.5 -xbc=1.5 -yac=-1.5 -ybc=1.5 -zac=-1.5 -zbc=1.5 -factor=2 -ml=2
#
# - sharper corners:
#  ogen quarterBoxGrid -factor=4 -sharpnessLB=120. 
#  
# Options:
#   - set box dimensions, corner sharpness
#   - full box, partial box 
#
#
$prefix="quarterBoxGrid";     # prefix for grid name 
#
$sharpnessLB=40.;                     # corner sharpness
$widthX=1.; $widthY=1.; $widthZ=1.;   # box size 
$blf=5.; # boundary layer stretching factor
$rotateX=0.; $rotateY=0.; $rotateZ=0.; # rotation (degrees) about X followed by Y followed by Z axis
#
# refinement box: 
$xar=0.; $xbr=3.; $yar=0; $ybr=.8; $zar=-1.25; $zbr=1.25; 
# Coarser background grid box:
$xac=0.; $xbc=4.; $yac=0.; $ybc=2.; $zac=-2.; $zbc=2.; 
#  -- wall patch in wake with stretched grid 
$xaw=0.; $xbw=2.; $yaw=0.; $ybw=.2; $zaw=-1.; $zbw=1.;  # $ybw is changed below
#
$order=2; $factor=1; $interp = "i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$factorNurbs=1.; # factor for the Nurbs representation 
$name=""; 
* 
* get command line arguments
GetOptions( "prefix=s"=>\$prefix,"order=i"=>\$order,"factor=f"=> \$factor,"blf=f"=>\$blf,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"sharpnessLB=f"=> \$sharpnessLB,\
            "widthX=f"=> \$widthX,"widthY=f"=> \$widthY,"widthZ=f"=> \$widthZ,\
            "xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,"za=f"=>\$za,"zb=f"=>\$zb,\
            "xac=f"=>\$xac,"xbc=f"=>\$xbc,"yac=f"=>\$yac,"ybc=f"=>\$ybc,"zac=f"=>\$zac,"zbc=f"=>\$zbc );
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=3; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=4; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = "quarterBoxGrid" . "$interp$factor" . $suffix . ".hdf";}
# 
#
# NOTE: x-bounds and y-bounds should be centered around 0: 
$xalb=-$widthX*.5; $xblb=$widthX*.5; $yalb=-$widthY*.5; $yblb=$widthY*.5; $zalb=0; $zblb=$widthZ*.5;   # lofted box bounds 
#
$ds=.1/$factor;
$dsn = .1/$factorNurbs; # build Nurbs representation with this grid spacing 
$dsBL = $ds/$blf; # boundary layer spacing (spacing in the normal direction)
* 
$dw = $order+1; $iw=$order+1; 
*
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
sub min{ local($n,$m)=@_; if( $n<$m ){ return $n; }else{ return $m; } }
#
$pi = 4.*atan2(1.,1.);
#
# nr = number of lines in normal directions to boundaries
$nr = max( 5 + $ng + 2*($order-2), 2**($ml+2) );
$nr = intmg( $nr );
#
$wallBC=7;   # BC for walls of the box 
$wallShare=7; 
# 
$stretchFactor=.75; # scale nDist to account for stretching
$nDist= $stretchFactor*($nr-2)*$ds;
if( $order eq 4 ){ $nDist=$nDist+2.*$ds; }
#
create mappings
#
# Define the box as a lofted surface
#
  lofted surface 
#   - cross-sections will be smoothed polygons:
    smoothed polygon sections
#
    $tStretchaLB=.05;  $tStretchbLB=$sharpnessLB*.5;  # stretching at corners in tangential directions
#
#   -- define the profile (also a smoothed-polygon)
#
    flat tip profile
#
#    set the profile:  (shape of box in y-z plane)
#
    edit profile
      show parameters
    # NOTE: I think the profile height needs to be 1 since it multiplies the section shape
    vertices
      4
      $x0=$zalb;    $y0=.5;
      $x1=$zblb;    $y1=$y0;   
      $x2=$x1;      $y2=-.5; 
      $x3=$x0;      $y3=$y2;
#
      $x0 $y0
      $x1 $y1
      $x2 $y2
      $x3 $y3
#
    sharpness
      $sharpnessLB
      $sharpnessLB
      $sharpnessLB
      $sharpnessLB
     t-stretch
      .0 $tStretchbLB
      $tStretchaLB $tStretchbLB
      $tStretchaLB $tStretchbLB
      .0 $tStretchbLB
    exit
    #
    #  Define the cross-section : shape in the x-y plane
    #
    edit section
    #
    vertices
      6
      $x0=($xalb+$xblb)*.5;   $y0=$yalb;
      $x1=$xblb;              $y1=$y0;
      $x2=$x1;                $y2=$yblb;
      $x3=$xalb;              $y3=$y2; 
      $x4=$x3;                $y4=$y0;
      $x5=$x0;                $y5=$y0; 
      #
      $x0 $y0
      $x1 $y1
      $x2 $y2
      $x3 $y3
      $x4 $y4
      $x5 $y5
    sharpness
      $sharpnessLB
      $sharpnessLB
      $sharpnessLB
      $sharpnessLB
      $sharpnessLB
      $sharpnessLB
     t-stretch
      .0 $tStretchLB
      $tsa = 2.*$tStretchaLB; # increase t-stretch weighting since there are twice as many corners
      $tsa $tStretchbLB
      $tsa $tStretchbLB
      $tsa $tStretchbLB
      $tsa $tStretchbLB
      .0 $tStretchLB
     periodicity
        2
    exit
#
  mappingName
    loftedHalfBoxGridSurface
   # open graphics
  exit
#
#  Generate a hyperbolic grid over the end face of the box where 
#  the lofted mapping has a singularity
  builder
    create surface grid...
      Start curve:quarterBoxGridSurface
      plot options...
      plot boundary lines on reference surface 1
      close plot options
      picking:choose initial curve
      surface grid options...
      initial curve:points on surface
#      choose point on surface 0 -.35 -.6 .75 
#      choose point on surface 0   0  -.6 .75 
#      choose point on surface 0  .35 -.6 .75 
      $delta=($ng-2)*$ds; # reduce cap width since ghost lines are added
      # increase cap width for fourth order and coarser grids -- ghost points are on surface anyway:
      if( $order eq 4 && $factor <4 ){ $delta=$delta-2.5*$ds; } 
#
      $capWidthFraction=.8;   # approx fraction of the end covered by the face grids 
#
      $x1=$capWidthFraction*$xblb-$delta; $y1=$capWidthFraction*$yalb+$delta; $z1=$zblb; 
      $y1a=0.; ####
      choose point on surface 0 -$x1 $y1a $z1
      choose point on surface 0   0  $y1a $z1
      choose point on surface 0  $x1 $y1a $z1 
      done
      # NOTE: We may want to choose avoid evaluating the surface on the singular point ??
      $ns = intmg( 2.*$x1/$ds +1.5 ); 
      $ns = $ns + ($ns % 2);  # Make ns even so we avoid the singularity
      points on initial curve $ns
      BC: left (forward) fix x, float y and z
      BC: right (forward) fix x, float y and z
      ## $dist=-2.*$y1 -$ds;   # ******************************* NOTE - $ds 
      $dist=-$y1; 
      distance to march $dist
      $nMarch = intmg( $dist/$ds +1.5 );
      lines to march $nMarch
      generate
      fourth order
      # Ensure that the ghost points lie on the surface:
      $numGhost = int( $order/2 + .5 ); 
      boundary offset $numGhost $numGhost 0 $numGhost  (l r b t)
      name quarterBoxGridFrontSurface
      # open graphics
    exit
  exit
#
#
# Split main box in half and Remove the singular ends from the main box surface
#
  reparameterize
    transform which mapping?
      loftedHalfBoxGridSurface
    restrict parameter space
      set corners
        $thetaStart=.25; $thetaEnd=.75; 
        .0 .92 $thetaStart $thetaEnd
      exit
    $stretchFactor=1.25;
    $nTheta = intmg( $stretchFactor*( 2.*($xblb-$xalb) + 2*($yblb-$yalb) )/$ds +1.5 ); 
    lines 
       $ns $nTheta 
    mappingName
      quarterBoxGridSurfaceNoEnds
    exit    
#
# Volume grid for the box body 
#
  hyperbolic
    Start curve:quarterBoxGridSurfaceNoEnds
    # target grid spacing .05 .05 (tang,normal, <0 : use default)
    backward
    distance to march $nDist
    $linesToMarch = $nr-1; 
    lines to march $linesToMarch
    BC: left fix z, float x and y
    BC: bottom fix y, float x and z
    BC: top fix y, float x and z
#
    generate
#
    fourth order
    # boundary offset $numGhost $numGhost 0 0 0 0 (l r b t b f)
    boundary offset 0 $numGhost 0 0 0 0 (l r b t b f)
    boundary conditions
      1 0 3 3 $wallBC 0
    share
      1 0 3 3 $wallShare 0
    lines 
       $ns = intmg( ( ($zblb-$zalb) + .5*($yblb-$yalb) )/$ds +1.5 ); 
       $ns $nTheta $nr 
    name quarterBoxGridBodyUnstretched
    # open graphics 
 exit
#
# Volume grid for the front cap
#
  hyperbolic
    Start curve:quarterBoxGridFrontSurface
    # target grid spacing .05 .05 (tang,normal, <0 : use default)
    distance to march $nDist
    $linesToMarch = $nr-1; 
    lines to march $linesToMarch
    BC: bottom fix y, float x and z
    generate
    fourth order
    boundary offset $numGhost $numGhost 0 $numGhost 0 0 (l r b t b f)
    lines
     $nx = intmg( $capWidthFraction*($xblb-$xalb)/$ds +1.5 ); 
     $ny = intmg( .5*$capWidthFraction*($yblb-$yalb)/$ds +1.5 ); 
     $nx $ny $nr    
    boundary conditions
      0 0 3 0 $wallBC 0
    share
      0 0 3 0 $wallShare 0
    name quarterBoxGridFrontUnstretched
    # open graphics
 exit
#
#  Stretch grid lines in the normal direction
#
  stretch coordinates
    transform which mapping?
      quarterBoxGridBodyUnstretched
    STRT:multigrid levels $ml 
    Stretch r3:exp to linear
    STP:stretch r3 expl: min dx, max dx $dsBL $ds
    #
    # cluster lines near the wall:
    $bStretch = $blf*2.5;  # guess this
    Stretch r2:itanh
    STP:stretch r2 itanh: layer 0 0.15 $bStretch 0. (id>=0,weight,exponent,position)
    STP:stretch r2 itanh: layer 1 0.15 $bStretch 1. (id>=0,weight,exponent,position)
    #
    STRT:name quarterBoxGridBody
  exit
#
  stretch coordinates
    transform which mapping?
      quarterBoxGridFrontUnstretched
    STRT:multigrid levels $ml 
    Stretch r3:exp to linear
    STP:stretch r3 expl: min dx, max dx $dsBL $ds
    Stretch r2:exp to linear
    STP:stretch r2 expl: min dx, max dx $dsBL $ds
    STRT:name quarterBoxGridFront
  exit
#
#
# -- convert grids to Nurbs and perform rotation and shift: 
#
  $angle=90.; $rotationAxis=1; 
  $xShift=0.; $yShift=0.; $zShift=0.; 
#
* Define a subroutine to convert a Mapping to a Nurbs Mapping
sub convertToNurbs\
{ local($old,$new,$angle,$rotationAxis,$xShift,$yShift,$zShift)=@_; \
  $cmds = "nurbs \n" . \
   "interpolate from mapping with options\n" . \
   " $old \n" . \
   " parameterize by index (uniform)\n" . \
   " number of ghost points to include\n $numGhost\n" . \
   " choose degree\n" . \
   "  3 \n" . \
   " # number of points to interpolate\n" . \
   " #  11 21 5 \n" . \
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
$numGhost=$ng+1; # N.B. to avoid negative volumes in the ghost points interpolate ghost too in Nurbs.
convertToNurbs(quarterBoxGridBody,quarterBoxGridBodyNurbs,$angle,$rotationAxis,$xShift,$yShift,$zShift);
$cmds
convertToNurbs(quarterBoxGridFront,quarterBoxGridFrontNurbs,$angle,$rotationAxis,$xShift,$yShift,$zShift);
$cmds
#
#  WALL-PATCH : Refined and stretched patch near wall near the box 
#
Box
  set corners
    $ybw =($nr-3)*$ds; 
    $xaw $xbw $yaw $ybw $zaw $zbw
  lines
    $nx = intmg( ($xbw-$xaw)/$ds +1.5 ); 
    $ny = intmg( ($ybw-$yaw)/$ds +1.5 ); 
    $nz = intmg( ($zbw-$zaw)/$ds +1.5 ); 
    $nx $ny $nz
  boundary conditions
    # 1 2 3 4 5 6 
    1 0 3 0 0 0 
  share
    1 0 3 0 0 0 
  mappingName
    wallPatchUnstretched
  exit
#
#  STRETCHED WALL-PATCH :  Stretch grid lines in the normal direction
#
  stretch coordinates
    transform which mapping?
      wallPatchUnstretched
    STRT:multigrid levels $ml 
    Stretch r2:exp to linear
    STP:stretch r2 expl: min dx, max dx $dsBL $ds
    STRT:name wallPatch
  exit
#
#  Refined patch near the box
#
Box
  set corners
    # $yap=$yar+5.*$ds; # raise up a bit 
    $xar $xbr $yar $ybr $zar $zbr
  lines
    $nx = intmg( ($xbr-$xar)/$ds +1.5 ); 
    $ny = intmg( ($ybr-$yar)/$ds +1.5 ); 
    $nz = intmg( ($zbr-$zar)/$ds +1.5 ); 
    $nx $ny $nz
  boundary conditions
    # 1 2 3 4 5 6 
    1 0 3 0 0 0 
  share
    1 0 3 0 0 0 
  mappingName
    refinementPatch
  exit
#
#  Coarser Background
#
Box
  set corners
    $xac $xbc $yac $ybc $zac $zbc
  lines
    $dsc=$ds*2.; 
    $nx = intmg( ($xbc-$xac)/$dsc +1.5 ); 
    $ny = intmg( ($ybc-$yac)/$dsc +1.5 ); 
    $nz = intmg( ($zbc-$zac)/$dsc +1.5 ); 
    $nx $ny $nz
  boundary conditions
    1 2 3 4 5 6 
  share
    1 0 3 0 0 0 
  mappingName
    backGround
  exit
#
#  Box for an explicit hole cutter
#    The cutter just needs to cover some portion of the interior of the quarter box
#    to cut some holes in the refinement box which otherwise could interpolate 
#    from the backGround grid
Box
  set corners
    $xaBox=-.1*$widthX;  $xbBox=.4*$widthX; $yaBox=-.1*$widthY; $ybBox=.4*$widthY; $zaBox=-.4*$widthZ; $zbBox=.4*$widthZ; 
    $xaBox $xbBox $yaBox $ybBox $zaBox $zbBox
  lines
    11 11 21
  boundary conditions
    1 2 3 4 5 6 
  mappingName
    boxCutter
  exit
# 
exit
#
# Make the overlapping grid
#
generate an overlapping grid
  backGround
  refinementPatch
  wallPatch
#  quarterBoxGridBody
#  quarterBoxGridFront
# -- use nurbs versions
  quarterBoxGridBodyNurbs
  quarterBoxGridFrontNurbs
#
  done
  change parameters
    create explicit hole cutter
      Hole cutter:boxCutter
      name: boxCutter
    exit
    * choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
  #
  # display intermediate results
  # open graphics
  compute overlap
  #  open graphics
  **  display computed geometry
  exit
*
* save an overlapping grid
save a grid (compressed)
$name
quarterBoxGrid
exit


