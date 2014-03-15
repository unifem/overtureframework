create mappings
  spline
    pick spline points
    -1 0 -.5 .5
         -0.0778718 -0.0706006 0 
         -0.162701 0.050429 0 
         -0.223294 0.155322 0 
         -0.275808 0.119013 0 
         -0.3364 -0.00201714 0 
         -0.437388 0.0363089 0 
         -0.627244 0.195665 0 
         -0.708034 0.0262231 0 
         -0.837299 0.0100858 0 
         -0.938286 -0.143218 0 
      done
    mappingName
    newSquigly
    exit
  filamentMapping
    user defined
    newSquigly
    Number of filament points
    35
    Number of thick spline points
    50
    Number of thick spline points
    90
    lines
    200 7
    Hyperbolic grid generator
      distance to march
      .03
      generate
      set view:0 0.169776 -0.31903 0 7.51316 1 0 0 0 1 0 0 0 1
      reset:0
      distance to march
      .07
      generate
      set view:0 0.277985 -0.628731 0 17.2903 1 0 0 0 1 0 0 0 1
      reset:0
      exit
    Number of filament points
    70
    Number of thick spline points
    150
    set view:0 0.158582 -0.214552 0 18.4828 1 0 0 0 1 0 0 0 1
    reset:0
    lines
    300 11
    Hyperbolic grid generator
      generate
      set view:0 0.121269 0.223881 0 9.84483 1 0 0 0 1 0 0 0 1
      reset:0
      exit
    exit
  spline
    pick spline points
    -1 0 -.5 .5
         -0.0545913 -0.0878768 0 
         -0.162493 0.0276184 0 
         -0.28796 0.143114 0 
         -0.330619 0.115495 0 
         -0.44354 0.110474 0 
         -0.584063 0.107963 0 
         -0.767245 0.143114 0 
         -0.845035 0.0477045 0 
      done
    mappingName
    anotherSquigly
    exit
  filamentMapping
    mappingName
    Filament2
    user defined
    anotherSquigly
    Number of filament points
    50
    Number of thick spline points
    120
    set view:0 -0.617537 0.190299 0 5.94792 1 0 0 0 1 0 0 0 1
    reset:0
    Number of end points
    7
    set view:0 -0.673507 0.205224 0 8.39706 1 0 0 0 1 0 0 0 1
    reset:0
    Hyperbolic grid generator
      distance to march
      .1
      generate
      distance to march
      .06
      generate
      set view:0 -0.289179 -0.0671642 0 8.12121 1 0 0 0 1 0 0 0 1
      reset:0
      exit
    exit
  rectangle
    set corners
    -1.5 0.5 -1 1
    lines
    50 50
    exit
  view mappings
    square
    Filament2
    set view:0 -0.242537 0.0410448 0 4.78571 1 0 0 0 1 0 0 0 1
    exit
  change a mapping
  square
    lines
    150 150
    exit
  view mappings
    square
    Filament2
    set view:0 -0.354812 -0.000668318 0 8.35551 1 0 0 0 1 0 0 0 1
    exit
  change a mapping
  Filament2
    reset:0
    set view:0 -0.470149 -0.143657 0 6.96104 1 0 0 0 1 0 0 0 1
    lines
    162 5
    reset:0
    set view:0 -0.326493 -0.0802239 0 8.15714 1 0 0 0 1 0 0 0 1
    reset:0
    exit
  exit this menu
  generate an overlapping grid
    square
    Filament2
    done choosing mappings
    change parameters
      ghost points
        all
        2 2 2 2 2 2
    exit
    compute overlap
    set view:0 -0.180421 -0.0873762 0 17.9927 1 0 0 0 1 0 0 0 1
    reset:0
    set view:0 -0.324627 0.0485075 0 5.65346 1 0 0 0 1 0 0 0 1
    reset:0
    exit
  save a grid
  userDefinedCenterLineFilamGrid.hdf
  testingCenterLine
  exit
