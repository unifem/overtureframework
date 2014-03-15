# ================================================================================================ 
# Ogen: generate a grid for a Ship Hull 
# 
# ================================================================================================ 
# 
# 
# Ship dimensions: 
$shipLength=2.; $shipDepth=.25; $shipBreadth=.25; 
$ml=0; 
$ds=.02; 
# 
# -- convert a number so that it is a power of 2 plus 1 -- 
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; } 
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } } 
# 
# 
create mappings 
  # -- Define the surface of the ship hull -- 
  #  NOTE: this surface includes the symmetric part above the water line so 
  #   that we can create a nice symmetric grid. 
  # 
  lofted surface 
    ship length: $shipLength 
    ship depth: $shipDepth 
    ship breadth: $shipBreadth 
    ship hull sections 
    0 0 
    flat double tip profile 
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
      $sa=1.; $sb=1; 
      $sa $sv 
      exit 
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
      $sa=1.; $sb=1; 
      $sa $sv 
      exit 
    mappingName 
    bowSurfaceStart 
    exit 
  # -- turn the stern and bow surfaces into NURBS --- 
  #    This is needed since the LoftedSurface does not compute derivatives 
  #    properly in cylindrical coordinates 
  nurbs (surface) 
    interpolate from a mapping 
    sternSurfaceStart 
    mappingName 
    sternSurfaceNurbsFull 
    exit 
  # 
  nurbs (surface) 
    interpolate from a mapping 
    bowSurfaceStart 
    mappingName 
    bowSurfaceNurbsFull 
    exit 
  # 
  # -- take the lower half of the bow and stern 
  reparameterize 
    transform which mapping? 
    bowSurfaceNurbsFull 
    set corners 
    0. 1. .5 1. 
    mappingName 
    bowSurfaceNurbs 
    # pause 
    exit 
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
  #  --- build a hyperbolic surface grid on the stern --- 
  builder 
    Start curve:sternSurfaceNurbs 
    create surface grid... 
      initial curve:coordinate line 0 
      choose point on surface 0 0. 6.793312e-03 3.564729e-06 .5 5.059769e-01 
      done 
      forward and backward 
      lines to march 15 15 (forward,backward) 
      distance to march .2 .2 (forward,backward) 
      equidistribution 0.3 (in [0,1]) 
      BC: right (forward) fix y, float x and z 
      BC: right (backward) fix y, float x and z 
      BC: left (forward) outward splay 
      BC: left (backward) outward splay 
      boundary offset 1, 0, 1, 1 (l r b t) 
      generate 
      name sternSurfaceHype 
      # pause 
      exit 
    exit 
  # 
  #  --- build a hyperbolic surface grid on the bow --- 
  builder 
    Start curve:bowSurfaceNurbs 
    create surface grid... 
      initial curve:coordinate line 0 
      choose point on surface 0 0. -8.744275e-02 1.998805e+00 .5 1.575091e-01 
      done 
      forward and backward 
      lines to march 15 15 (forward,backward) 
      distance to march .2 .2 (forward,backward) 
      equidistribution 0.3 (in [0,1]) 
      BC: left (forward) fix y, float x and z 
      BC: left (backward) fix y, float x and z 
      BC: right (forward) outward splay 
      BC: right (backward) outward splay 
      boundary offset 0, 1, 1, 1 (l r b t) 
      generate 
      name bowSurfaceHype 
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
    .15 .85 .5 1. 
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
    target grid spacing $ds $ds (tang,norm)((<0 : use default) 
    add surface grid 
    hullSurfaceNoEnds 
    # 
    Start curve:hullSurfaceNoEnds 
    create volume grid... 
      points on initial curve 57, 26 
      backward 
      lines to march 11 
      BC: bottom fix y, float x and z 
      BC: top fix y, float x and z 
      generate 
      boundary conditions 
      0 0 $bcSurface $bcSurface $bcHull 0 
      share 
      0 0 $shareSurface $shareSurface $shareHull 0 
      name hullVolume 
      # pause 
      exit 
    # 
    #   -- volume grid on the stern 
    # 
    add surface grid 
    sternSurfaceHype 
    Start curve:sternSurfaceHype 
    create volume grid... 
      lines to march 11 
      points on initial curve 20, 29 
      BC: right fix y, float x and z 
      generate 
      name sternVolume 
      boundary conditions 
      0 $bcSurface 0 0 $bcHull 0 
      share 
      0 $shareSurface 0 0 $shareHull 0 
      # pause 
      exit 
    # 
    # 
    #   -- volume grid on the bow 
    add surface grid 
    bowSurfaceHype 
    Start curve:bowSurfaceHype 
    create volume grid... 
      lines to march 11 
      points on initial curve 20, 29 
      BC: left fix y, float x and z 
      generate 
      name bowVolume 
      boundary conditions 
      $bcSurface 0 0 0 $bcHull 0 
      share 
      $shareSurface 0 0 0 $shareHull 0 
      # pause 
      exit 
    # 
    # exit builder: 
    exit 
  # 
  # Here is background grid 
  # 
  Box 
    $xa=-1.; $xb=1.; $ya=-.75; $yb=0.; $za=-1.; $zb=3.; 
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
  open graphics 
  compute overlap
  change the plot
    toggle grid 0 0
    y+r:0
    x-r:0
    y+r:0
    y+r:0
    set view:0 0.00302115 -0.081571 0 4.47297 0.866928 -0.0301537 -0.497521 -0.0593912 0.984808 -0.163176 0.494883 0.17101 0.851966
    y+r:0
    y+r:0
    smaller:0
    smaller:0
    smaller:0
    x+r:0
    x+r:0
    x+r:0
    x+r:0
    x-r:0
    x-r:0
    x-r:0
    x-r:0
    x-r:0
    x-r:0
    x-r:0
    x+r:0
    y-r:0
    y-r:0
    y-r:0
    bigger:0
    bigger:0
    bigger:0
    x-:0
    x-:0
    x-:0
    hardcopy file name:0 loftedShipGrid.ps
    hardcopy save:0
    pick to colour grids
    pick colour...
    PIC:brass
    grid colour 1 BRASS
    grid colour 2 BRASS
    grid colour 3 BRASS
    close colour choices
    plot grid lines 0
    pick to toggle boundaries
    toggle boundary 1 1 1 0
    toggle boundary 0 0 3 0
    toggle boundary 0 1 1 0
    toggle boundary 1 0 2 0
    x+r:0
    x+r:0
    x+r:0
    x+r:0
    y-r:0
    x+r:0
    x-r:0
    x-r:0
    x-r:0
    x-r:0
    x-r:0
    reset:0
    set view:0 -0.0151057 -0.126888 0 6.36538 1 0 0 0 1 0 0 0 1
    x-r:0
    x-r:0
    x-r:0
    plot grid lines 1
    reset:0
    x+r:0
    x+r:0
    x+r:0
    hardcopy close dialog:0
    set view:0 -0.0422961 -0.081571 0 3.28553 1 0 0 0 0.866025 0.5 0 -0.5 0.866025
    x+r:0
    x+r:0
