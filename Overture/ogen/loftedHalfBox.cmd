# loftedHalfBox: make grids for the exterior of a half-box with rounded corners
#
# Examples:
#
#  ogen -noplot loftedHalfBox.cmd -factor=2 
#
#  ogen -noplot loftedHalfBox.cmd -interp=e -factor=2 
#  ogen -noplot loftedHalfBox.cmd -interp=e -factor=4 
#
# - sharper corners:
#  ogen -noplot loftedHalfBox -factor=4 -interp=e -sharpnessLB=120. 
#  
# - MG:
#  ogen -noplot loftedHalfBox.cmd -interp=e -factor=2 -ml=2   [loftedHalfBoxe2.order2.ml2.hdf
#  ogen -noplot loftedHalfBox.cmd -interp=e -factor=4 -ml=3   [
#
# - order 4:
#  ogen -noplot loftedHalfBox.cmd -interp=e -order=4 -factor=2 -ml=2 
#  ogen -noplot loftedHalfBox.cmd -interp=e -order=4 -factor=4 -ml=2 -xb=2. 
#
$sharpnessLB=40.;                     # corner sharpness
$widthX=1.; $widthY=1.; $widthZ=1.;   # box size 
$rotateX=0.; $rotateY=0.; $rotateZ=0.; # rotation (degrees) about X followed by Y followed by Z axis
#
$order=2; $factor=1; $interp = "i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$factorNurbs=1.; # factor for the Nurbs representation 
$name=""; 
$xa=-2.; $xb=4.; $ya=0.; $yb=2.; $za=-2.; $zb=2.; # bounding box 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"sharpnessLB=f"=> \$sharpnessLB,\
            "widthX=f"=> \$widthX,"widthY=f"=> \$widthY,"widthZ=f"=> \$widthZ,\
            "xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,"za=f"=> \$za,"zb=f"=> \$zb );
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = "loftedHalfBox" . "$interp$factor" . $suffix . ".hdf";}
* 
#
# NOTE: x-bounds and y-bounds should be centered around 0: 
$xalb=-$widthX*.5; $xblb=$widthX*.5; $yalb=-$widthY*.5; $yblb=$widthY*.5; $zalb=0; $zblb=$widthZ;   # lofted box bounds 
#
$ds=.1/$factor;
$dsn = .1/$factorNurbs; # build Nurbs representation with this grid spacing 
$dsBL = $ds/5.; # boundary layer spacing (spacing in the normal direction)
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
$Pi = 4.*atan2(1.,1.);
#
# Boundary conditions:
$inflowBC=3; $outflowBC=4; $wallBC=1; $slipWallBC=2; 
$bottomShare=2; # share flag for the bottom
#
# nr = number of lines in normal directions to boundaries
$nr = max( 5 + $ng + 2*($order-2), 2**($ml+2) );
$nr = intmg( $nr );
# 
$nDist= ($nr-2)*$ds;
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
    loftedHalfBoxSurface
  exit
#
  builder
    create surface grid...
      Start curve:loftedHalfBoxSurface
      plot options...
      plot boundary lines on reference surface 1
      close plot options
      picking:choose initial curve
      surface grid options...
      initial curve:points on surface
      $delta=($ng-1)*$ds; # reduce cap width since ghost lines are added
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
      name loftedHalfBoxFrontSurface
    exit
  exit
#
# Remove the singular end from the main box surface
#
  reparameterize
    transform which mapping?
      loftedHalfBoxSurface
    restrict parameter space
      set corners
        .0 .9 0. 1. 
      exit
    lines
      31 61
    mappingName
      loftedHalfBoxSurfaceNoEnds
    exit    
#
# Volume grid for the box body 
#
  mapping from normals
    extend normals from which mapping?
      loftedHalfBoxSurfaceNoEnds
    normal distance
      -$nDist
    boundary conditions
      3 0 -1 -1 7 0
    share
      3 0 0 0 7 0
    $nTheta = intmg( ( 2.*($xblb-$xalb) + 2*($yblb-$yalb) )/$ds +1.5 ); 
    $ns = intmg( ( ($zblb-$zalb) + ($yblb-$yalb) )/$ds +1.5 ); 
    lines 
       $ns $nTheta $nr 
    mappingName
      loftedHalfBoxBodyUnstretched
    exit
#
# Volume grid for the front cap
#
  mapping from normals
    extend normals from which mapping?
      loftedHalfBoxFrontSurface
    normal distance
      $nDist
    boundary conditions
      0 0 0 0 7 0
    share
      0 0 0 0 7 0
    lines
      $nx = intmg( $capWidthFraction*($xblb-$xalb)/$ds +1.5 ); 
      $ny = intmg( $capWidthFraction*($yblb-$yalb)/$ds +1.5 ); 
      $nx $ny $nr
    mappingName
      loftedHalfBoxFrontUnstretched
    exit
#
#  Stretch grid lines in the normal direction
#
  stretch coordinates
    transform which mapping?
      loftedHalfBoxBodyUnstretched
    STRT:multigrid levels $ml 
    Stretch r3:exp to linear
    STP:stretch r3 expl: min dx, max dx $dsBL $ds
    STRT:name loftedHalfBoxBody
  exit
#
#
  stretch coordinates
    transform which mapping?
      loftedHalfBoxFrontUnstretched
    STRT:multigrid levels $ml 
    Stretch r3:exp to linear
    STP:stretch r3 expl: min dx, max dx $dsBL $ds
    STRT:name loftedHalfBoxFront
  exit
# open graphics
#
#
# -- convert grids to Nurbs and perform rotation and shift: 
#
  $angle=-90.; $rotationAxis=0; 
  $xShift=0.; $yShift=0.; $zShift=0.; 
#
* Define a subroutine to convert a Mapping to a Nurbs Mapping
sub convertToNurbs\
{ local($old,$new,$angle,$rotationAxis,$xShift,$yShift,$zShift)=@_; \
  $cmds = "nurbs \n" . \
   "interpolate from mapping with options\n" . \
   " $old \n" . \
   " parameterize by index (uniform)\n" . \
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
convertToNurbs(loftedHalfBoxBody,loftedHalfBoxBodyNurbs,$angle,$rotationAxis,$xShift,$yShift,$zShift);
$cmds
convertToNurbs(loftedHalfBoxFront,loftedHalfBoxFrontNurbs,$angle,$rotationAxis,$xShift,$yShift,$zShift);
$cmds
#
#  Background
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
    0 0 3 0 0 0 
  mappingName
    backGround
  exit
#
# Make the overlapping grid
#
exit
generate an overlapping grid
  backGround
  loftedHalfBoxBodyNurbs
  loftedHalfBoxFrontNurbs
#
  done
  change parameters
    * choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng
  exit
*  display intermediate results
#  change the plot
# open graphics
  compute overlap
**  display computed geometry
  exit
*
* save an overlapping grid
save a grid (compressed)
$name
loftedHalfBox
exit


