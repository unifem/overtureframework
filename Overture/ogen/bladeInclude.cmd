#
# Create a turbine blade and tip (included in hubAndBlades.cmd)
#
# Input: 
#    $ds 
#    $bladeLength
#
  if( $bladeLength <= 0. ){ $bladeLength=9.; } 
  lofted surface
    blade length: $bladeLength
    cylinder to Joukowsky sections
    wind turbine profile
    lines
      $length=$bladeLength*7./9.; $chord=2.; # fix me 
      $ns = int( $length/$ds + 1.5 );
      $nTheta = int( $chord*2./$ds + 1.5 );
     $ns $nTheta
     # 121 81   
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
      $nx = int( 1.5/$ds + 1.5 );
      $nx $nx 
      # 41 41  21 21 
    exit
#
#
  builder
    $dsTip = $ds*.5;  # fix me 
    # we need to march with a finer grid spacing and then coarsen after
    target grid spacing .02 .01 (tang,norm)((<0 : use default)
#    target grid spacing $dsTip $dsTip (tang,norm)((<0 : use default)
    add surface grid 
    wingTipSurfaceNurbs 
    create volume grid...
      backward
      uniform dissipation 0.05
      volume smooths 10
      lines to march 21
      generate
      lines
        $nx $nx 11
      boundary conditions
        0 0 0 0 2 0
      share
        0 0 0 0 2 0
      name wingTip
# pause
      exit
# 
    add surface grid
    wingSurfaceNoTip
    create volume grid...
      target grid spacing .02 .02
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
