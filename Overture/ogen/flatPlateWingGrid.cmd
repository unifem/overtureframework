#
#   Make a grid for a flat plate wing 
#
# usage: ogen [noplot] flatPlateWingGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<> -useNurbs=[0|1]
# 
# examples:
#     ogen -noplot flatPlateWingGrid -interp=e -factor=2 [ OK - NOTE: use degreeNurbs=1 or trouble with cgins at corners
#     ogen -noplot flatPlateWingGrid -interp=e -factor=4 [ OK
#
# Multigrid:
#     ogen -noplot flatPlateWingGrid -interp=e -factor=2 -ml=1 [OK
#     ogen -noplot flatPlateWingGrid -interp=e -factor=4 -ml=2 [OK
#     ogen -noplot flatPlateWingGrid -interp=e -factor=8 -ml=3 [OK
# 
# -- Do not use nurbs grids:
#    ogen -noplot flatPlateWingGrid -interp=e -useNurbs=0 -factor=2 -ml=1 
#    ogen -noplot flatPlateWingGrid -interp=e -useNurbs=0 -factor=4 -ml=2
#
$order=2; $factor=1; $interp = "i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$name=""; $xa=-1.; $xb=1.; $ya=-1.; $yb=1.;
$useNurbs = 1;   # 1 : build Nurbs mappings for wing grids, 0: use hyperbolic grids
# 
$plateBC   =10; # BC value for plate surface
$plateShare=10; # share value for plate surface
#-----------------------------------------------------------------
$chord=1.; 
# $span=50./15.; 
$span=2.;
$AspectRatio=$span/$chord;
$halfSpan=$AspectRatio/2; #Assume chord length = 1
#-----------------------------------------------------------------
$Fore_Angle=0;
$FL_xdist=0.0;  # Distance from origin (x axis)  #LEFT SIDE
$FL_ydist=0;    # Distance from origin (y-axis)  #LEFT SIDE
$FL_zdist=0; # Space between body to root of wing (z axis)   #LEFT SIDE
#
#-----------------------------------------------------------------
$thick=0.025; #Half of wing's thickness
$marchdist=0.25; #Normal march distance for body, wings, and caps
#-----------------------------------------------------------------
$gsf = 1.1; # geometric stretch factor for stretching grid lines near boundaries and edges
# 
$saCap=1.; $sbCap=.8; # orthographic patch extent
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"useNurbs=i"=>\$useNurbs );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $useNurbs ne 1 ){ $suffix .= ".hype"; }
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = "flatPlateWingGrid" . "$interp$factor" . $suffix . ".hdf";}
# 
$ds0=.1;
$ds=$ds0/$factor;
$dsNormal=$ds*.25; # make grid spacing in normal direction this amount
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
$dw = $order+1; $iw=$order+1;
# parallel ghost lines: for ogen we need at least:
# #       .5*( iw -1 )   : implicit interpolation 
# #       .5*( iw+dw-2 ) : explicit interpolation
$parallelGhost=($iw-1)/2;
if( $interp eq "e" ){  $parallelGhost=($iw+$dw-2)/2; }
if( $parallelGhost<1 ){ $parallelGhost=1; }
minimum number of distributed ghost lines
$parallelGhost
#
create mappings 
#
# -- define the cross section for the plate ---
 smoothedPolygon 
  make 3d (toggle) 
      0 
  curve or area (toggle) 
  vertices 
    6
     0  -$thick 
   -.5  -$thick 
   -.5   $thick 
    .5   $thick 
    .5  -$thick 
     0  -$thick
  lines 
    81 
  sharpness 
     80 
     80 
     80 
     80 
     80 
     80
  t-stretch
    .1 .1
    .1 40.
    .1 40.
    .1 40.
    .1 40.
    .1 .1
    mappingName 
      FLcrsp 
    exit 
#
# -- make three cross-sections ---
#
  rotate/scale/shift 
    rotate 
      -$Fore_Angle 2
    0 0 0
    shift 
      $x=$FL_xdist;
      $y=$FL_ydist;
      $z=$FL_zdist;
      $x $y $z 
    mappingName 
      FLcrsp0 
    exit 
*
  rotate/scale/shift  
    shift   
      0 0 -$halfSpan
    mappingName 
      FLcrsp1 
  exit 
* 
  rotate/scale/shift 
    transform which mapping? 
      FLcrsp0 
    shift 
      0 0 $halfSpan
    mappingName 
      FLcrsp2 
    exit 
#
# -- cross section mapping for the plate plus ends --
#
  crossSection 
    general 
      3 
      FLcrsp1 
      FLcrsp0 
      FLcrsp2 
    polar singularity at start 
    polar singularity at end 
    lines 
      $stretchFactor=1.25; # add extra lines in theta direction for stretching
      $nTheta = intmg( $stretchFactor*2.*($chord+$thick)/$ds + 1.5 );
      $ns = intmg( $AspectRatio/$ds +1.5 );
      $nTheta $ns
    mappingName 
      FLwingSurf 
    share 
      0 0 0 0 $plateShare 0 
# pause
    exit 
#
#  --- Make the north pole cap ----
#
  reparameterize 
    orthographic 
      mappingName 
        CapN 
      choose north or south pole 
        1 
      specify sa,sb 
        $saCap $sbCap
      lines 
        200 200 
    exit 
 #- pause
  exit 
  * 
  builder 
#    
    target grid spacing $ds $dsNormal (tang,norm)((<0 : use default)
  #
  # --- surface grid for the north pole cap ---
  #
    create surface grid... 
#      -- cluster the grid lines near the edge to have a finer grid spacing:
      $dsEdge=$ds/4.; 
      target grid spacing $ds, $dsEdge (tang,normal, <0 : use default)
      geometric stretch factor $gsf
#
      surface grid options... 
      initial curve:coordinate line 0 
      close surface grid options 
      choose point on surface 0 -2.061222e-01 -3.201652e-03 4.980608e-01 5.011077e-01 5.440648e-01
      done 
      forward and backward 
      # 
      BC: left (forward) outward splay
      BC: right (forward) outward splay
      BC: left (backward) outward splay
      BC: right (backward) outward splay
      #
      # The boundary offset says how many ghost lines are included in the grid -- for surface grids
      # we want the ghost points to lie on the surface (and not be extrapolated)
      boundary offset 1, 1, 1, 1 (l r b t)
      #
      # We wish to march until the grid spacing increases from $dsEdge to $ds 
      $linesForCap = log($ds/$dsEdge)/log($gsf) -1.;
      # Here is approximately how far we march: (sum of a geometric series)
      #    L = a*( r^n -1 )/( r -1 )
      $distForCap = $dsEdge*( $gsf**($linesForCap) -1. )/( $gsf-1. ); 
      # But we should march far enough:
      $minDistForCap=.15;  # grow the cap grid at least this far
      $maxDistForCap=.25;   # grow the cap grid at most this far
      $distForCap = min( $maxDistForCap, max( $distForCap, $minDistForCap ) );
      # Note 1: We march one extra point since the final grid point in the marching direction is a ghost point,
      #         thus we add 1 to linesForCap below.
      # Note 2: The number of lines to march is one less that the usual number of grid points,
      #         thus we subtract 1 from linesForCap below.
      $linesForCap = intmg( log( $distForCap*($gsf-1.)/$dsEdge+1. )/log($gsf)  + .5 ) -1 + 1; 
      lines to march $linesForCap, $linesForCap (forward,backward) 
      equidistribution .2 (in [0,1])
      $capWidth=1.3; # Fix ME : we should be able to set the number of MG levels in Hype and builder
      $nsCap = intmg( $capWidth/$ds + 1.5 ) + 2 ;  # add 2 for the boundary offsets
      points on initial curve $nsCap
      generate 
# pause
      exit 
#
    # --- volume grid for the north pole cap ---
    create volume grid... 
#
      spacing: geometric
      geometric stretch factor $gsf
#
      share 
        0 0 0 0 $plateShare 0 
      boundary conditions 
        0 0 0 0 $plateBC 0 
      # -- note the surface grid boundary offset already made the ghost lines in tangential directions to
      #    lie on the surface.
      boundary offset 0, 0, 0, 0, 0, 1 (l r b t b f)
      forward 
      # We wish to march until the grid spacing increases from $dsNormal to $ds 
      #   $dsNormal*($gsf)^n = $ds 
      $nr = intmg( log($ds/$dsNormal)/log($gsf) +.5  ) -1 + 1 ;  # NOTE -1 for lines to march, +1 for b.o.
      lines to march $nr
      generate 
      mappingName 
       FLNorth-cap
## pause
   exit 
 exit 
#
#  --- Make the south pole cap ----
#
  reparameterize 
    transform which mapping? 
      FLwingSurf 
    orthographic 
      choose north or south pole 
        -1 
     specify sa,sb 
        $saCap $sbCap
     lines 
        200 200 
      exit 
    mappingName 
      FLCapS 
    exit 
#
#
 builder 
  target grid spacing $ds $dsNormal (tang,norm)((<0 : use default)
  #
  # --- surface grid for the south pole cap ---
  #
  create surface grid... 
    # -- cluster the grid lines near the edge to have a finer grid spacing:
      target grid spacing $ds, $dsEdge (tang,normal, <0 : use default)
      geometric stretch factor $gsf
    #
    surface grid options... 
    initial curve:coordinate line 0
    choose point on surface 0 -2.869185e-02 -1.905771e-04 -9.998884e-01 4.940180e-01 4.955250e-01
    done 
    forward and backward 
    # 
    BC: left (forward) outward splay
    BC: right (forward) outward splay
    BC: left (backward) outward splay
    BC: right (backward) outward splay
    # The boundary offset says how many ghost lines are included in the grid -- for surface grids
    # we want the ghost points to lie on the surface (and not be extrapolated)
    boundary offset 1, 1, 1, 1 (l r b t)
    #
    lines to march $linesForCap, $linesForCap (forward,backward) 
    equidistribution .2 (in [0,1])
    points on initial curve $nsCap
    generate 
##pause
   exit 
#
  # --- volume grid for the south pole cap ---
    create volume grid... 
      spacing: geometric
      geometric stretch factor $gsf
      share 
        0 0 0 0 $plateShare 0 
      boundary conditions 
        0 0 0 0 $plateBC 0 
      # -- note the surface grid boundary offset already made the ghost lines in tangential directions to
      #    lie on the surface.
      boundary offset 0, 0, 0, 0, 0, 1 (l r b t b f)
      forward 
      lines to march $nr
      generate 
      mappingName 
        FLSouth-cap
      exit 
##pause
    exit 
*
#   --- remove the singular ends from the plate ---
  reparameterize 
    transform which mapping? 
      FLwingSurf 
    set corners 
      0 1 .05 .95 0 1 
    lines 
      $stretchFactor=1.25; # add extra lines in theta direction for stretching
      $nTheta = intmg( $stretchFactor*2.*($chord+$thick)/$ds + 1.5 );
      $ns = intmg( .9*$AspectRatio/$ds +1.5 ) +2; # add 2 for boundary offset
      $nTheta $ns
#
    mappingName 
      FLwingSurfcut 
## pause
    exit 
  * 
  hyperbolic
    target grid spacing $ds $dsNormal (tang,norm)((<0 : use default)
    spacing: geometric
    geometric stretch factor $gsf
    share 
      0 0 0 0 $plateShare 0 
    boundary conditions 
      -1 -1 0 0 $plateBC 0 
    # 
    boundary offset 0, 0, 1, 1, 0, 1 (l r b t b f)
    backward
    lines to march $nr
    generate
    mappingName
      FLWing
#pause
   exit
#
*
*----------------create Nurbs for faster evaluation --------------------------- 
*
 $nurbsDegree=1; # default is 3 but this may cause wiggles if grid is too coarse
     nurbs (surface)
      interpolate from mapping with options
        FLWing
        choose degree
         $nurbsDegree
        parameterize by index (uniform)
      done
      mappingName
        FLWing-nurb
      exit
*
    nurbs (surface)
      interpolate from mapping with options
        FLSouth-cap
        choose degree
         $nurbsDegree
        parameterize by index (uniform)
      done
      mappingName
        FLSouth-cap-nurb
      exit
*
    nurbs (surface)
      interpolate from mapping with options
        FLNorth-cap
        choose degree
         $nurbsDegree
        parameterize by index (uniform)
      done
      mappingName
        FLNorth-cap-nurb
      exit
#
#  Refinement grid near the wing
#
Box
  # $extraWidth = 1./$factor;  
  # $extraWidth = .25 + .5/$factor;  # Is this ok?
  $extraWidth = .125 + .75/$factor;  # Is this ok?
  $downStreamWidth=$extraWidth; 
  $xad=-$chord*.5-$extraWidth; $xbd=$chord*.5+$downStreamWidth; 
  $yad=-$thick-$extraWidth;    $ybd=$thick+$extraWidth; 
  $zad=-$halfSpan-$extraWidth; $zbd=$halfSpan+$extraWidth;
  set corners
    $xad $xbd $yad $ybd $zad $zbd
  lines
    $nx = intmg( ($xbd-$xad)/$ds +1.5 ); 
    $ny = intmg( ($ybd-$yad)/$ds +1.5 ); 
    $nz = intmg( ($zbd-$zad)/$ds +1.5 ); 
    $nx $ny $nz
  boundary conditions
    0 0 0 0 0 0 
  mappingName
    wingBox
  exit
#
#  Coarser background grid
#
Box
  $dsBox=2.*$ds; # increase grid spacing by a factor of 2
  # $extraWidth = 4./$factor;  
  $sideWidth = 1.0; # extra widths on sides
  $upStreamDist=1.5; 
  $downStreamDist=2.;
  $xad=-$chord*.5-$upStreamDist; $xbd=$chord*.5+$downStreamDist; 
  $yad=-$thick   -$sideWidth; $ybd=$thick   +$sideWidth; 
  $zad=-$halfSpan-$sideWidth; $zbd=$halfSpan+$sideWidth;
  set corners
    $xad $xbd $yad $ybd $zad $zbd
  lines
    $nx = intmg( ($xbd-$xad)/$dsBox +1.5 ); 
    $ny = intmg( ($ybd-$yad)/$dsBox +1.5 ); 
    $nz = intmg( ($zbd-$zad)/$dsBox +1.5 ); 
    $nx $ny $nz
  boundary conditions
    1 2 3 4 5 6 
  share
    1 2 3 4 5 6 
  mappingName
    backGround
  exit
#
# Make the overlapping grid
#
exit
generate an overlapping grid
  backGround
  wingBox
#
  if( $useNurbs eq 1 ){ $wingGrids = "FLWing-nurb\n FLSouth-cap-nurb\n FLNorth-cap-nurb"; }\
                  else{ $wingGrids = "FLWing     \n FLSouth-cap     \n FLNorth-cap     "; }
  $wingGrids
#
  done
  change parameters
 # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
#  display intermediate results
#  open graphics
#
  compute overlap
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
flatPlateWing
exit
