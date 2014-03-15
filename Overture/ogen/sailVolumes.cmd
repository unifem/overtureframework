*
*
  create mappings
*
  nurbs (surface)
    read NURBS data, format cheryl
    ../mapping/WindwardSurf.nurbs
    lines
      61 31      41 21 21 11  51 21  15 15
    mappingName
      windWardReferenceSurface
    exit
*
  elliptic
    transform which mapping?
      windWardReferenceSurface
    elliptic smoothing
      maximum number of iterations
        5
      number of multigrid levels
        2
      boundary conditions
        left   (side=0,axis=0)
        free floating
        right  (side=1,axis=0)
        free floating
        exit
      * debug
      *   7
      x-r 90
      generate grid
      exit
    mappingName
      windWardSurface
  exit
*
*
  nurbs (surface)
    read NURBS data, format cheryl
    ../mapping/LeewardSurf.nurbs
    lines
      61 31    41 21 21 11  51 21  15 15
    mappingName
      leeWardReferenceSurface
    exit
*
  elliptic
    transform which mapping?
      leeWardReferenceSurface
    elliptic smoothing
      maximum number of iterations
        5
      number of multigrid levels
        2
      boundary conditions
        left   (side=0,axis=0)
        free floating
        right  (side=1,axis=0)
        free floating
        exit
      * debug
      *   7
      generate grid
      exit
    mappingName
      leeWardSurface
  exit
* --- top ----
  nurbs (surface)
    read NURBS data, format cheryl
    ../mapping/WindwardSurf.nurbs
    lines
      25 11 61 31 41 21 21 11  51 21  15 15
    restrict the domain
      .89 1. 0. 1.
    mappingName
      topWindWardReferenceSurface
    exit
*
  elliptic
    transform which mapping?
      topWindWardReferenceSurface
    elliptic smoothing
      maximum number of iterations
        5
      number of multigrid levels
        2
      boundary conditions
        left   (side=0,axis=0)
        free floating
        right  (side=1,axis=0)
        free floating
        exit
      * debug
      *   7
      generate grid
      exit
    mappingName
      topWindWardSurface
  exit
*
* --- bottom ----
  nurbs (surface)
    read NURBS data, format cheryl
    ../mapping/WindwardSurf.nurbs
    lines
      61 31 41 21 21 11  51 21  15 15
    restrict the domain
      0.025 .90 0.05 .95   0.025 .975   0. .90 0. 1.    
    mappingName
      bottomWindWardReferenceSurface
    exit
*
  elliptic
    transform which mapping?
      bottomWindWardReferenceSurface
    elliptic smoothing
      maximum number of iterations
        5
      number of multigrid levels
        1
      boundary conditions
        left   (side=0,axis=0)
        free floating
        right  (side=1,axis=0)
        free floating
        exit
      * debug
      *   7
      generate grid
      exit
    mappingName
      bottomWindWardSurface
  exit
*
* --- top ----
  nurbs (surface)
    read NURBS data, format cheryl
    ../mapping/LeewardSurf.nurbs
    lines
      25 11 61 31 41 21 21 11  51 21  15 15
    restrict the domain
      .89 1. 0. 1.
    mappingName
      topLeeWardReferenceSurface
    exit
*
  elliptic
    transform which mapping?
      topLeeWardReferenceSurface
    elliptic smoothing
      maximum number of iterations
        5
      number of multigrid levels
        2
      boundary conditions
        left   (side=0,axis=0)
        free floating
        right  (side=1,axis=0)
        free floating
        exit
      * debug
      *   7
      generate grid
      exit
    mappingName
      topLeeWardSurface
  exit
*
* --- bottom ----
  nurbs (surface)
    read NURBS data, format cheryl
    ../mapping/LeewardSurf.nurbs
    lines
      61 31 41 21 21 11  51 21  15 15
    restrict the domain
      0.025 .90 0.05 .95   0.025 .975   0. .90 0. 1.    
    mappingName
      bottomLeeWardReferenceSurface
    exit
*
  elliptic
    transform which mapping?
      bottomLeeWardReferenceSurface
    elliptic smoothing
      maximum number of iterations
        5
      number of multigrid levels
        1
      boundary conditions
        left   (side=0,axis=0)
        free floating
        right  (side=1,axis=0)
        free floating
        exit
      * debug
      *   7
      generate grid
      exit
    mappingName
      bottomLeeWardSurface
  exit
*
  hyperbolic
    start from which curve/surface?
     topLeeWardSurface
    mappingName
      topLeeWardVolume
    lines to march
     7
    distance to march
      15 12
    implicit coefficient
      0.
    uniform dissipation coefficient
     .4
    grow grid in opposite direction
    generate
    * pause
    boundary conditions
     0 0 0 0 1 0
    share
     0 0 0 0 1 0
  exit
*
*
  hyperbolic
    start from which curve/surface?
     bottomLeeWardSurface
    mappingName
      bottomLeeWardVolume
    lines to march
     7
    distance to march
      15
    implicit coefficient
      0.
    uniform dissipation coefficient
     .4
    grow grid in opposite direction
    generate
    boundary conditions
     0 0 0 0 1 0
    share
     0 0 0 0 1 0
  exit
*
*
  hyperbolic
    start from which curve/surface?
     topWindWardSurface
    mappingName
      topWindWardVolume
    lines to march
     7
    distance to march
      15 12
    implicit coefficient
      0.
    uniform dissipation coefficient
     .4
    generate
    * pause
    boundary conditions
     0 0 0 0 1 0
    share
     0 0 0 0 1 0
  exit
*
*
  hyperbolic
    start from which curve/surface?
     bottomWindWardSurface
    mappingName
      bottomWindWardVolume
    lines to march
     7
    distance to march
      15
    implicit coefficient
      0.
    uniform dissipation coefficient
     .4
    generate
    * pause
    boundary conditions
     0 0 0 0 1 0
    share
     0 0 0 0 1 0
  exit
*
*
  open a data-base
   sailSurfaces.hdf
     open a new file
   put to the data-base
     windWardSurface
   put to the data-base
     topWindWardSurface
   put to the data-base
     bottomWindWardSurface
   put to the data-base
     leeWardSurface
   put to the data-base
     topLeeWardSurface
   put to the data-base
     bottomLeeWardSurface
   close the data-base
*
  open a data-base
   sailVolumes.hdf
     open a new file
   put to the data-base
     topWindWardVolume
   put to the data-base
     bottomWindWardVolume
   put to the data-base
     topLeeWardVolume
   put to the data-base
     bottomLeeWardVolume
   close the data-base
exit
exit

