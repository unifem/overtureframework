$bladeLength=6.; 
$ds=.05; 
# 
create mappings
#
  lofted surface
    blade length: $bladeLength
    cylinder to Joukowsky sections
    wind turbine profile
    lines
      121 81   81 51
    mappingName
      wingSurface1
#pause
    exit
#
  reparameterize
    orthographic
      choose north or south pole
      -1
      specify sa,sb
        # the first direction follows the edge of the wing tip
        # .175 .125
        .25 .175
      exit
     lines
      61 121  61 61 31 21
   mappingName
     wingTipSurface
#pause
  exit
# 
  reparameterize
    transform which mapping?
    wingSurface1
    restrict parameter space
      set corners
        0. .975 0 1
      exit
    mappingName
     wingSurfaceNoTip
#pause
    exit
# -- convert to Nurbs since we do not have derivatives in cylindrical coords
  nurbs (surface)
    interpolate from a mapping
      wingTipSurface
    mappingName
      wingTipSurfaceNurbs
    lines
      41 41  21 21 
    exit
#
#
  builder
    target grid spacing .02 .02 (tang,norm)((<0 : use default)
    add surface grid 
    wingTipSurfaceNurbs 
    create volume grid...
      backward
      uniform dissipation 0.05
      volume smooths 10
      lines to march 11  
      generate
      boundary conditions
        0 0 0 0 2 0
      share
        0 0 0 0 2 0
      name wingTip
      exit
    add surface grid
    wingSurfaceNoTip
    create volume grid...
      Start curve:wingSurfaceNoTip
      backward
      uniform dissipation 0.05
      volume smooths 10
      lines to march 11  
      BC: left fix z, float x and y
      generate
      boundary conditions
        3 0 -1 -1 2 0
      share
        3 0  0  0 2 0
      name wing
      exit
   exit
#
# Here is the box
#
Box
  $xa=-.5; $xb=2.; $ya=-.75; $yb=.75; $za=0.; $zb=$bladeLength+2.;
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5);
    $ny = int( ($yb-$ya)/$ds +1.5);
    $nz = int( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
  boundary conditions
    5 6 4 4 3 4 
  share
    0 0 0 0 3 0 
  mappingName
    box
  exit
exit
#
generate an overlapping grid
  box
  wing
  wingTip
  done
  open graphics














    add surface grid
    wingTipSurfaceNurbs




    build curve on surface
      plane grid points 51 51
      cut with plane

      exit
    create surface grid...
      choose boundary curve 0
      done
#
      forward and backward
      BC: left (forward) outward splay
      BC: right (forward) outward splay
      BC: left (backward) outward splay
      BC: right (backward) outward splay
      equidistribution .75 (in [0,1])
      outward splay .25 .25 (left, right for outward splay BC)
      edit initial curve
        restrict the domain
          .3 .7
        exit
      generate
      outward splay 0.2, 0.2
      lines to march 41, 41 (forward,backward)  
      generate





# 
  reparameterize
    set corners
      0 1 .95 .99999
    lines
      101 101
    mappingName
     capSurface
pause
  exit
# -- convert to Nurbs since we have finished derivatives
  nurbs (surface)
    interpolate from a mapping
    capSurface
    set view:0 0 0 0 1 0.867492 0.166651 -0.468706 0.130427 0.833056 0.537594 0.480049 -0.52749 0.700933
    mappingName
     capNurbs
  exit
#
  reparameterize
    transform which mapping?
      capNurbs
    restrict parameter space
     set corners
      0. .5 .0 1.
      exit
    lines
      51 51
    mappingName
      capTop
pause
    exit
  reparameterize
    transform which mapping?
      capNurbs
    restrict parameter space
      set corners
      .5 1. .0 1.
      exit
    lines
      51 51
    mappingName
     capBottom
pause
    exit
  composite surface
    CSUP:add a mapping capTop
    CSUP:add a mapping capBottom
pause
    CSUP:determine topology
     deltaS 0.01
     maximum area 1.e-6
     compute topology
 pause
    exit
    CSUP:mappingName cap
    exit
