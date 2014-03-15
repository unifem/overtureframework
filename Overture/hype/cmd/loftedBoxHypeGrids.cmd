# loftedBox: make grids for the exterior of a box with rounded corners
# Usage:
#  hype [-noplot] loftedBoxHypeGrids.cmd -factor=<> -interp=[e|i] -order=<> -ml=<> -widthX=<> -widthY=<> -widthZ=<> ...
#           -blf=<> -xa=<> -xb=<> -ya=<> -yb=<> -za=<> -zb=<> -refinementBox=[0|1]
# Options:
#   -xa, -xb, -ya, -yb, -za, -zb : bounding box
#   -blf : boundary layer stretching factor
#   -refinementBox : 1=add a refinement box and coarsen the backGround grid
#
# Examples:
#
#  ../hype -noplot loftedBoxHypeGrids.cmd 
#
$sharpnessLB=40.;                     # corner sharpness
$widthX=1.; $widthY=1.; $widthZ=1.;   # box size 
$blf=5.; # boundary layer stretching factor
$rotateX=0.; $rotateY=0.; $rotateZ=0.; # rotation (degrees) about X followed by Y followed by Z axis
#
# refinement box: 
$refinementBox=1; # 1=add refinement box 
$xa=""; $xb=""; $ya=""; $yb=""; $za=""; $zb=""; 
# Coarser background grid  bo:
$xac=-2.; $xbc=4.; $yac=-2.; $ybc=2.; $zac=-2.; $zbc=2.; 
#
$order=2; $factor=2; $interp = "i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$factorNurbs=1.; # factor for the Nurbs representation 
$name=""; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"blf=f"=>\$blf,"refinementBox=i"=>\$refinementBox,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"sharpnessLB=f"=> \$sharpnessLB,\
            "widthX=f"=> \$widthX,"widthY=f"=> \$widthY,"widthZ=f"=> \$widthZ,\
            "xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,"za=f"=>\$za,"zb=f"=>\$zb,\
            "xac=f"=>\$xac,"xbc=f"=>\$xbc,"yac=f"=>\$yac,"ybc=f"=>\$ybc,"zac=f"=>\$zac,"zbc=f"=>\$zbc );
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $refinementBox eq 0 ){ $suffix .= "noRefine"; }
if( $name eq "" ){$name = "loftedBox" . "$interp$factor" . $suffix . ".hdf";}
* 
#
# NOTE: x-bounds and y-bounds should be centered around 0: 
$xalb=-$widthX*.5; $xblb=$widthX*.5; $yalb=-$widthY*.5; $yblb=$widthY*.5; $zalb=-$widthZ*.5; $zblb=$widthZ*.5;   # lofted box bounds 
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
    flat double tip profile
#
#    set the profile:  (shape of box in y-z plane)
#
    edit profile
      show parameters
    # NOTE: I think the profile height needs to be 1 since it multiplies the section shape
    vertices
      6
      $x0=$zalb;    $y0=0.;
      $x1=$x0;      $y1=.5;
      $x2=$zblb;    $y2=$y1;   
      $x3=$x2;      $y3=-.5; 
      $x4=$x0;      $y4=$y3;
      $x5=$x0;      $y5=$y0; 
#
      $x0 $y0
      $x1 $y1
      $x2 $y2
      $x3 $y3
      $x4 $y4
      $x5 $y5
#-            0. 0. 
#-             0. .5 
#-             1. .5 
#-             1. -.5 
#-             0. -.5 
#-             0. 0. 
    sharpness
      $sharpnessLB
      $sharpnessLB
      $sharpnessLB
      $sharpnessLB
      $sharpnessLB
      $sharpnessLB
     t-stretch
      .0 $tStretchbLB
      $tStretchaLB $tStretchbLB
      $tStretchaLB $tStretchbLB
      $tStretchaLB $tStretchbLB
      $tStretchaLB $tStretchbLB
      .0 $tStretchbLB
     periodicity
        2
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
    loftedBoxSurface
 # open graphics
  exit
#
#  Generate a hyperbolic grid over the end face of the box where 
#  the lofted mapping has a singularity
  builder
    create surface grid...
      Start curve:loftedBoxSurface
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
      choose point on surface 0 -$x1 $y1 $z1
      choose point on surface 0   0  $y1 $z1
      choose point on surface 0  $x1 $y1 $z1 
      done
      # NOTE: We may want to choose avoid evaluating the surface on the singular point ??
      $ns = intmg( 2.*$x1/$ds +1.5 ); 
      $ns = $ns + ($ns % 2);  # Make ns even so we avoid the singularity
      points on initial curve $ns
      BC: left (forward) fix x, float y and z
      BC: right (forward) fix x, float y and z
      $dist=-2.*$y1 -$ds;   # ******************************* NOTE - $ds 
      distance to march $dist
      $nMarch = intmg( $dist/$ds +1.5 );
      lines to march $nMarch
      generate
      fourth order
      # Ensure that the ghost points lie on the surface:
      $numGhost = int( $order/2 + .5 ); 
      ## boundary offset $numGhost $numGhost $numGhost $numGhost  (l r b t)
      name loftedBoxFrontSurface
    exit
#
    create surface grid...
      Start curve:loftedBoxSurface
      plot options...
      plot boundary lines on reference surface 1
      close plot options
      picking:choose initial curve
      surface grid options...
      initial curve:points on surface
#      choose point on surface 0 -.35 -.6 .75 
#      choose point on surface 0   0  -.6 .75 
#      choose point on surface 0  .35 -.6 .75 
      $x1=$capWidthFraction*$xblb-$delta; $y1=$capWidthFraction*$yalb+$delta; $z1=$zalb; 
      choose point on surface 0  $x1 $y1 $z1
      choose point on surface 0   0  $y1 $z1
      choose point on surface 0 -$x1 $y1 $z1 
      done
      points on initial curve $ns
      BC: left (forward) fix x, float y and z
      BC: right (forward) fix x, float y and z
      distance to march $dist
      $nMarch = intmg( $dist/$ds +1.5 );
      lines to march $nMarch
      generate
      fourth order
      # Ensure that the ghost points lie on the surface:
      ### boundary offset $numGhost $numGhost $numGhost $numGhost  (l r b t)
      name loftedBoxBackSurface
    exit
  exit
#
# Remove the singular ends from the main box surface
#
  reparameterize
    transform which mapping?
      loftedBoxSurface
    restrict parameter space
      set corners
        .08 .92 0. 1. 
      exit
    $nTheta = intmg( ( 2.*($xblb-$xalb) + 2*($yblb-$yalb) )/$ds +1.5 ); 
    $ns = intmg( ( ($zblb-$zalb) + ($yblb-$yalb) )/$ds +1.5 ); 
    lines 
       $ns $nTheta 
    mappingName
      loftedBoxSurfaceNoEnds
    exit    
#
# Volume grid for the box body 
#
  hyperbolic
    Start curve:loftedBoxSurfaceNoEnds
    # target grid spacing .05 .05 (tang,normal, <0 : use default)
    backward
    distance to march $nDist
    $linesToMarch = $nr-1; 
    lines to march $linesToMarch
    generate
    fourth order
    boundary offset $numGhost $numGhost 0 0 0 0 (l r b t b f)
    boundary conditions
      0 0 -1 -1 $wallBC 0
    share
      0 0 0 0 $wallShare 0
    lines 
       $ns $nTheta $nr 
    name loftedBoxBodyUnstretched
 exit
#
#-   mapping from normals
#-     extend normals from which mapping?
#-       loftedBoxSurfaceNoEnds
#-     normal distance
#-       -$nDist
#-     boundary conditions
#-       0 0 -1 -1 $wallBC 0
#-     share
#-       0 0 0 0 $wallShare 0
#-     $nTheta = intmg( ( 2.*($xblb-$xalb) + 2*($yblb-$yalb) )/$ds +1.5 ); 
#-     $ns = intmg( ( ($zblb-$zalb) + ($yblb-$yalb) )/$ds +1.5 ); 
#-     lines 
#-        $ns $nTheta $nr 
#-     mappingName
#-       loftedBoxBodyUnstretched
#-     exit
#
# Volume grid for the back cap
#
  hyperbolic
    Start curve:loftedBoxBackSurface
    # target grid spacing .05 .05 (tang,normal, <0 : use default)
    distance to march $nDist
    $linesToMarch = $nr-1; 
    lines to march $linesToMarch
    generate
    fourth order
    boundary offset $numGhost $numGhost $numGhost $numGhost 0 0 (l r b t b f)
    lines
     $nx = intmg( $capWidthFraction*($xblb-$xalb)/$ds +1.5 ); 
     $ny = intmg( $capWidthFraction*($yblb-$yalb)/$ds +1.5 ); 
     $nx $ny $nr    
    boundary conditions
      0 0 0 0 $wallBC 0
    share
      0 0 0 0 $wallShare 0
    name loftedBoxBackUnstretched
 exit
#  NOTE: There was trouble using the normal mapping with ogmg --
#  The normal mapping uses derivatives of the surface and these may get amplified
#  far from the body.
# 
#-  mapping from normals
#-    extend normals from which mapping?
#-      loftedBoxBackSurface
#-    normal distance
#-      $nDist
#-    boundary conditions
#-      0 0 0 0 $wallBC 0
#-    share
#-      0 0 0 0 $wallShare 0
#-    lines
#-      $nx = intmg( $capWidthFraction*($xblb-$xalb)/$ds +1.5 ); 
#-      $ny = intmg( $capWidthFraction*($yblb-$yalb)/$ds +1.5 ); 
#-      $nx $ny $nr
#-    mappingName
#-      loftedBoxBackUnstretched
#-    exit
#
# Volume grid for the front cap
#
  hyperbolic
    Start curve:loftedBoxFrontSurface
    # target grid spacing .05 .05 (tang,normal, <0 : use default)
    distance to march $nDist
    $linesToMarch = $nr-1; 
    lines to march $linesToMarch
    generate
    fourth order
    boundary offset $numGhost $numGhost $numGhost $numGhost 0 0 (l r b t b f)
    lines
     $nx $ny $nr    
    boundary conditions
      0 0 0 0 $wallBC 0
    share
      0 0 0 0 $wallShare 0
    name loftedBoxFrontUnstretched
 exit
exit
exit


