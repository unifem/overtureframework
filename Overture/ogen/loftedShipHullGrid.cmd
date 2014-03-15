# ================================================================================================
# Ogen: generate a grid for a Ship Hull 
#  
# Usage:
#   ogen [-noplot] loftedShipHullGrid.cmd -order=<i> -interp=[i|e] -factor=<i> -ml=<i> ...
#              -shipLength=<f> -shipDepth=<f> -shipBreadth=<f> -hullSharpeness=<f> -profileSharpness=<f>
#              -xa=<f> -xb=<f> -ya=<f> -yb=<f> -za=<f> -zb=<f> ...
# Options:
#  -shipLength, -shipDepth, -shipBreadth : ship dimensions
#  -hullSharpeness : default=.025, to make the hull stern/bow sharper, make this smaller, but too small a value may 
#           cause trouble in grid generation. (Finer grids may support sharper hulls)
#  -xa, -xb, -ya, -yb, -za, -zb : bounds on the background grid
#
# Examples:
#   ogen -noplot loftedShipHullGrid -order=2 -interp=e -factor=4  [
#   ogen -noplot loftedShipHullGrid -order=2 -interp=e -factor=6  [
#   ogen -noplot loftedShipHullGrid -order=2 -interp=e -factor=8  [
#
# -- fourth-order grids
#   ogen -noplot loftedShipHullGrid -order=4 -interp=e -factor=4
#
# -- Multigrid levels:
#   ogen -noplot loftedShipHullGrid -order=2 -interp=e -factor=4 -ml=1
#
# -- wider and deeper ship:
#   ogen -noplot loftedShipHullGrid -order=2 -interp=e -factor=4 -shipDepth=.125 -shipBreadth=.4 
#
# ================================================================================================
#
#
# Ship dimensions:
$shipLength=2.; $shipDepth=.0625; $shipBreadth=.2; 
$hullSharpness=.02; # to make the hull bow/stern sharper make this smaller (but a smaller value may cause trouble in grid generation)
$profileSharpness=50.;  # Increase sharpness exponent to make a sharper corner between the bow and keel
# 
$ml=0; 
$xa=-.4; $xb=.4; $ya=-.2; $yb=0.; $za=-.4; $zb=$shipLength+.4;  # bounds on background grid
$nrMin=5; $name=""; 
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $dse=0.; 
$stretchFactor=5.; # stretch grid lines by this factor at the boundary
$suffix=""; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"nrExtra=i"=>\$nrExtra,"nrMin=i"=>\$nrMin,\
            "interp=s"=> \$interp,"name=s"=>\$name,\
            "shipLength=f"=>\$shipLength,"shipDepth=f"=>\$shipDepth,"shipBreadth=f"=>\$shipBreadth,\
            "hullSharpness=f"=>\$hullSharpness,"profileSharpness=f"=>\$profileSharpness,\
            "xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,"za=f"=>\$za,"zb=f"=>\$zb,"ml=i"=>\$ml,\
            "stretchFactor=f"=>\$stretchFactor,"box=f"=>\$box,"suffix=s"=>\$suffix );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; $dse=1.; }
# 
$prefix="loftedShipHullGrid";
$suffix .= ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){ $name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.05/$factor;
$dsn=$ds/$stretchFactor; # spacing in normal direction on the hull
$numGhost=$order/2; # number of ghost points we need (boundary offset)
# 
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub min{ local($n,$m)=@_; if( $n<$m ){ return $n; }else{ return $m; } }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
#
# 
$nr = intmg( 13 );  # lines to march in the normal direction
#
create mappings
#
#   -----------------------------------------
#   -- Define the surface of the ship hull --
#   -----------------------------------------
#
#  NOTE: this surface includes the symmetric part above the water line so
#   that we can create a nice symmetric grid.
#
  lofted surface
    ship length: $shipLength
    ship depth: $shipDepth
    ship breadth: $shipBreadth
    hull sharpness: $hullSharpness
    ship hull sections
      0 0 
    flat double tip profile
    # 
    # -- Edit the profile:
    #   Increase sharpness exponent to make a sharper corner between the bow and keel
    edit profile
      sharpness
      $profileSharpness
      $profileSharpness
      $profileSharpness
      $profileSharpness
      $profileSharpness
      $profileSharpness
    exit
    lines
      # Note: add 2*numGhost since we remove it later (boundaryOffset)
      $nsHull = intmg( 1.3*($shipLength+$shipBreadth)/$ds + 1.5 ) + 2*$numGhost;
      $nThetaHull = intmg( 3.5*($shipDepth+$shipBreadth)/$ds + 1.5 );
      $nsHull $nThetaHull
    mappingName
     shipHullSurfaceStart

    # pause
    exit
#
#   Create a patch on the stern and bow to remove the polar singularity
#
  reparameterize
    transform which mapping?
      shipHullSurfaceStart 
    orthographic
     choose north or south pole
        1
      specify sa,sb
        # $sa and $sb define the extent of the orthographic patch
        # $sa=.8+ ($order-2)*.1; 
        $sa=.7+ ($order-2)*.1; 
        $sb=$sa; 
        $sa $sb
      exit
      lines
        41 41 
      mappingName
        sternSurfaceStart
    exit
# 
  reparameterize
    transform which mapping?
      shipHullSurfaceStart 
    orthographic
     choose north or south pole
        -1
      specify sa,sb
        $sa $sb
      exit
      lines
        41 41 
      mappingName
        bowSurfaceStart
    exit
# -- turn the stern and bow surfaces into NURBS ---
#    This is needed since the LoftedSurface does not compute derivatives 
#    properly in cylindrical coordinates
  nurbs (surface) 
    interpolate from mapping with options
      sternSurfaceStart
      choose degree
       2
    done
    mappingName
      sternSurfaceNurbsFull
    exit
#
  nurbs (surface) 
    interpolate from mapping with options
      bowSurfaceStart
      choose degree
       2
    done
    mappingName
      bowSurfaceNurbsFull
    exit
#
# -- take the lower half of the bow
#
  reparameterize
    transform which mapping?
    bowSurfaceNurbsFull
    set corners
       0. 1. .5 1. 
    #
    mappingName
      bowSurfaceNurbs
    # pause
    exit
# 
# -- take the lower half of the stern 
#
  reparameterize
    transform which mapping?
    sternSurfaceNurbsFull
    set corners
       0. 1. 0. .5
    set view:0 0 0 0 1 -0.661525 -0.128304 0.738866 0.0421495 0.977337 0.207451 -0.748738 0.168377 -0.641124
    mappingName
    sternSurfaceNurbs
    # pause
    exit
# 
#  --- build a hyperbolic surface grid on the bow ---
#
#  NOTE: make the stern/bow surface grids with a bit finer resolution
#
  $dsSurf=min(.006,$ds*.65); # make the surface grids with a reasonably fine resolution
# 
  $distToMarch=.04;  # march this distance from bow along hull (in both directions)
  $linesToMarch = intmg( 2.5*$distToMarch/$dsSurf + .5 );  # add more lines for stretching later
  $prowFactor = 2. + ($order-2)/4.; # what should this be ?
  $nProw = intmg( $prowFactor*$shipDepth/$dsSurf + 1.5 ) + $numGhost;  # add numGhost since we remove later (boundaryOffset)
  builder 
    Start curve:bowSurfaceNurbs 
    create surface grid... 
      initial curve:coordinate line 0 
      choose point on surface 0 0. -8.744275e-02 1.998805e+00 .5 1.575091e-01
      done 
      forward and backward 
      lines to march $linesToMarch $linesToMarch (forward,backward) 
      distance to march $distToMarch $distToMarch (forward,backward) 
      points on initial curve $nProw
      equidistribution 0.3 (in [0,1]) 
      volume smooths 100
      outward splay .25 .25 (left, right for outward splay BC)
      BC: left (forward) fix y, float x and z
      BC: left (backward) fix y, float x and z
      BC: right (forward) outward splay
      BC: right (backward) outward splay
      # -- do NOT set the boundary offset on surface grids --
      boundary offset 0 0 0 0  (l r b t)
      generate
      name bowSurfaceHype
      # open graphics
      # pause
      exit
    exit
# 
#  --- build a hyperbolic surface grid on the stern ---
#
  builder 
    Start curve:sternSurfaceNurbs 
    create surface grid... 
      initial curve:coordinate line 0 
      choose point on surface 0 0. 6.793312e-03 3.564729e-06 .5 5.059769e-01 
      done 
      forward and backward 
      distance to march $distToMarch $distToMarch (forward,backward) 
      #
      lines to march $linesToMarch $linesToMarch (forward,backward) 
      points on initial curve $nProw
#
      equidistribution 0.3 (in [0,1]) 
      volume smooths 100
      outward splay .25 .25 (left, right for outward splay BC)
      BC: right (forward) fix y, float x and z
      BC: right (backward) fix y, float x and z
      BC: left (forward) outward splay
      BC: left (backward) outward splay
      # -- do NOT set the boundary offset on surface grids --
      boundary offset 0 0 0 0  (l r b t)
      generate
      name sternSurfaceHype
      # pause
      exit
    exit
#
# -- remove the polar singularities from the hull surface --
#
  reparameterize
    transform which mapping?
     shipHullSurfaceStart
     set corners
      $startHull=.175-$ds;   
      $endHull=1.-$startHull;
      $startHull $endHull .5 1.
    mappingName
      hullSurfaceNoEnds
  # pause
    exit
#
#  -- build a volume grid over the hull ---
#
$bcSurface=4; $shareSurface=4;
$bcHull=7; $shareHull=7; 
  builder
    # target grid spacing $ds $ds (tang,norm)((<0 : use default)
    add surface grid
      hullSurfaceNoEnds
#
    # We initially march with more lines so we get a better quality grid:
    $nrHype = $nr*2; 
    $normalDistanceToMarch = ($nr-2.5)*$ds;  
#
    Start curve:hullSurfaceNoEnds
    create volume grid...
      # Note: add 2*numGhost since we remove it later (boundaryOffset)
      $nsHull = intmg( 1.1*($shipLength+$shipBreadth)/$ds + 1.5 ) + 2*$numGhost;
      $nThetaHull = intmg( 2.*($shipDepth+$shipBreadth)/$ds + 1.5 );
      points on initial curve $nsHull, $nThetaHull
      backward
      lines to march $nrHype
      # NOTE: normalDistanceToMarch is a bit smaller on the hull to avoid "hanging cells"
      # where the hull grid meets the stern (or bow) 
      distance to march $normalDistanceToMarch
      BC: bottom fix y, float x and z
      BC: top fix y, float x and z
      # -- DO set the boundary offset on interp boundaries of volume grids --
      boundary offset $numGhost, $numGhost, 0,  0, 0, 0 (l r b t b f)
      # open graphics
      generate
      boundary conditions
         0 0 $bcSurface $bcSurface $bcHull 0
      share 
         0 0 $shareSurface $shareSurface $shareHull 0 
      ghost lines to plot: $numGhost
      # pause
      name hullVolumeUnstretched
    # pause
   exit
# 
#   -- volume grid on the stern
#
    $normalDistanceToMarch = ($nr-1.)*$ds;   # this is slightly bigger than for thr hull
#
    add surface grid
      sternSurfaceHype
    Start curve:sternSurfaceHype
    create volume grid...
      lines to march $nrHype
      distance to march $normalDistanceToMarch
      BC: right fix y, float x and z
      # decrease volume smooths for convex corners:
      volume smooths 10
      # -- DO set the boundary offset on interp boundaries of volume grids --
      boundary offset $numGhost, 0, $numGhost, $numGhost,  0, 0 (l r b t b f)
      # open graphics
      generate
      name sternVolumeUnstretched
      boundary conditions
         0 $bcSurface 0 0 $bcHull 0
      share 
         0 $shareSurface 0 0 $shareHull 0       
      ghost lines to plot: $numGhost
      #
      # now set the number of lines to the actual value:
      # lines
      #   -1 -1 $nr
     # pause
    exit
#
#
#   -- volume grid on the bow 
    add surface grid
      bowSurfaceHype
    Start curve:bowSurfaceHype
    create volume grid...
      lines to march $nrHype
      distance to march $normalDistanceToMarch
      BC: left fix y, float x and z
      # decrease volume smooths for convex corners:
      volume smooths 10
      # -- DO set the boundary offset on interp boundaries of volume grids --
      boundary offset 0, $numGhost, $numGhost, $numGhost,  0, 0 (l r b t b f)
      generate
      name bowVolumeUnstretched
      boundary conditions
        $bcSurface 0 0 0 $bcHull 0
      share 
        $shareSurface 0 0 0 $shareHull 0       
      # lines
      #   -1 -1 $nr
     # open graphics
   # pause
     exit
  #
  # exit builder:
  exit 
#
#  ---------------------------------
#  -- add stretching to the grids --
#  ---------------------------------
  stretch coordinates
    transform which mapping?
      hullVolumeUnstretched
    STRT:multigrid levels $ml
    Stretch r3:exp to linear
    STP:stretch r3 expl: min dx, max dx $dsn $ds
    STRT:name hullVolume1
  exit
#
  stretch coordinates
    transform which mapping?
      sternVolumeUnstretched
    STRT:multigrid levels $ml
    Stretch r3:exp to linear
    # -- reduce linear weight (or else too few grid lines)
    STP:stretch r3 expl: linear weight 2
    # On COARSE grids decrease the target spacing to match better (for some reason)
    $dsnfactor=1./(1+ (1.5/$factor)**2 );  # 
    $dsnStern=$dsn*$dsnfactor;
    $dsStern=$ds*$dsnfactor; 
    STP:stretch r3 expl: min dx, max dx $dsnStern $dsStern
    Stretch r2:itanh
    # cluster points near the sharp edge   
    STP:stretch r2 itanh: layer 0 .25 8. .5 (id>=0,weight,exponent,position)
    # cluster points near the water line 
    Stretch r1:itanh
    STP:stretch r1 itanh: layer 0 0.25 8. 1. (id>=0,weight,exponent,position)
    #
    STRT:name sternVolume1
  exit
#
  stretch coordinates
    transform which mapping?
      bowVolumeUnstretched
    STRT:multigrid levels $ml
    Stretch r3:exp to linear
    # -- reduce linear weight (or else too few grid lines)
    STP:stretch r3 expl: linear weight 2
    # decrease normal stretching to be consistent with the hull
    STP:stretch r3 expl: min dx, max dx $dsnStern $dsStern
    Stretch r2:itanh
    # cluster points near the sharp edge   
    STP:stretch r2 itanh: layer 0 .25 8. .5 (id>=0,weight,exponent,position)
    # cluster points near the water line 
    Stretch r1:itanh
    STP:stretch r1 itanh: layer 0 0.25 8. 0. (id>=0,weight,exponent,position)
    #
    STRT:name bowVolume1
  exit
#
# Define a subroutine to convert a Mapping to a Nurbs Mapping
#
sub convertToNurbs\
{ local($old,$new,$angle)=@_; \
  $commands .= "nurbs (surface)\n" . \
              "interpolate from mapping with options\n" . "$old\n" . "parameterize by index (uniform)\n" . "done\n" . \
              "rotate\n" . "$angle 1\n" . "0 0 0\n" . \
              "mappingName\n" . "$new\n" . "exit\n"; \
}
*
# -- Convert the hull grids to NURBS : this should be faster to evaluate and smoother too --
#  NOTE: we could scale and rotate here!
$commands="";
convertToNurbs("hullVolume1","hullVolume",0.);
convertToNurbs("sternVolume1","sternVolume",0.);
convertToNurbs("bowVolume1","bowVolume",0.);
$commands
#
# Here is background grid 
#
Box
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5);
    $ny = intmg( ($yb-$ya)/$ds +1.5);
    $nz = intmg( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
  boundary conditions
    1 2 3 $bcSurface 5 6
  share
    0 0 0 $shareSurface 0 0
  mappingName
    backGround
  exit
#**********************************
exit
#
generate an overlapping grid
  backGround
  hullVolume
  sternVolume
  bowVolume
  done
#
  change parameters
# 
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
#
# 
#  open graphics
  compute overlap
#
exit
# save an overlapping grid
save a grid (compressed)
$name
shipHull
exit
