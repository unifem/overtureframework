create mappings
  rectangle
    mappingName
      bottom
    specify corners
      0 0 1. .5
    lines
      21 11
    share
      0 1 0 2
    exit
  rectangle
    mappingName
      right
    specify corners
      .5 .25 1. 1.
    lines
      11 21
    boundary conditions
      1 1 0 1
    share
      2 1 0 0
    exit
  fillet
    choose curves
    bottom (side=1,axis=1)
    right (side=0,axis=0)
    compute fillet
    mappingName
      fillet-curve
    exit
  hyperbolic
    mappingName
      fillet
    hypgen
    distance to march (ZREG)
      .3
    generate
    exit
    lines
      25 11
    share
      0 0 2 0
    exit
  exit this menu
  generate an overlapping grid
    right
    bottom
    fillet
    done choosing mappings
    change parameters
      prevent hole cutting
        right
        all
        bottom
        all
        done
      useBoundaryAdjustment
    exit
      