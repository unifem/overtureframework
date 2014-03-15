#
# This file is included in site300Grid.cmd
#
#
  builder
    add surface grid
    surface
    # target grid spacing $ds $ds (tang,norm)((<0 : use default)
    create volume grid...
     BC: left fix x, float y and z
     BC: right fix x, float y and z
     BC: bottom fix y, float x and z
     BC: top fix y, float x and z
     # -- GHOST points go bad at lower-right corner, this seems to help:
     apply boundary conditions to start curve 1
     # 
     $terrainFactor=1.25; # account for extra domain size that includes the terrain
     $nx = intmg( $terrainFactor*($xb-$xa)/$ds + 1.5 );
     $ny = intmg( $terrainFactor*($yb-$ya)/$ds + 1.5 );
     # points on initial curve 183, 145
     points on initial curve $nx, $ny
     boundary conditions
       1 2 3 4 5 0
     share
       1 2 3 4 0 0
     # We cannot use the boundary offset to shift the ghost points
     # since the resulting boundary faces will not flat. 
     boundary offset 0 0 0 0 0 1 (l r b t b f)
     # ---New 2011/10/03
     normal blending 7, 7, 7, 7 (lines, left,right,bottom,top)
     # I think we need to increase the number of volume smooths as we make the grid finer
     # ---New 2011/10/03
     $volSmooths=100*$factor; 
     volume smooths $volSmooths
     ## volume smooths 200
     $linesToMarch = intmg( 17 )-1 +1;
     lines to march $linesToMarch
     # $dist = 75.*$scale;
     # make the grid spacing finer in the normal direction -- we will later adjust this
     # when we stretch the grid
     $dist = .5*$linesToMarch*$ds; 
     distance to march $dist
     generate     
     # ---New 2011/10/03
     ## fourth order
     mappingName terrain-unstretched
   # pause
   exit
#
  exit
#
# -- stretch the grid lines in the normal direction.
#
  stretch coordinates
    transform which mapping?
#
    terrain-unstretched
    #- terrain-nurbs
#
    Stretch r3:exp to linear
    STRT:multigrid levels $ml
    # Transition the grid spacing to the outer box spacing:
    STP:stretch r3 expl: min dx, max dx $dsNormal $ds
    # stretch grid
    STRT:name terrain-stretched 
  #  pause
  exit
#
# -- convert to a nurbs 
#
   nurbs 
     interpolate from mapping with options
     terrain-stretched
     parameterize by index (uniform)
     choose degree
       2
     done
     mappingName
      terrain
  exit
