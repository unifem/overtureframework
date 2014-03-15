  Annulus
    make 3d (toggle)
      1.
    exit
*
  Cylinder
    surface or volume (toggle)
    exit
*
  composite surface
    CSUP:add a mapping Annulus
    CSUP:add a mapping Cylinder
    CSUP:determine topology
      compute topology
      exit
    exit
  Annulus
    mappingName
      startSurface
    inner radius
      1.
    outer radius 
      1.5
    make 3d (toggle)
      1. .75
    lines
      9 2 13 2 5 2 13 3  5 2 
    exit
*
  hyperbolic
    BC: bottom match to a mapping
    compositeSurface
    BC: top fix x, float y and z
    lines to march 3
    distance to march .4
    x-r:0 80
*    debug
*      3
* ***NOTE*** to make the grid grow symmetrically downward turn off the smoothing and volume smooths.
    generate


    boundary conditions for marching
      bottom (side=0,axis=1)
      match to a mapping
      compositeSurface
      exit
    distance to march
      .3
    lines to march
      4
    debug 
      3
    grow grid in opposite direction
    uniform dissipation coefficient
      0.1
    generate
      x-r 45
    plot boundary condition mappings (toggle)