*
* This grid tests the case when the hole cutting alogrithm
* finds some interpolation points that eventually are found
* to be outside the domain.
*
create mappings
*
  rectangle
    lines
      21 21
    mappingName
      background
    boundary conditions
      1 0 0 1
    exit
*
  rectangle
    mappingName
      right
    specify corners
      .6 .2 .9 1.
    lines
      9 15
    boundary conditions
      0 1 0 1
    share
      0 1 0 0
    exit
*
  rectangle
    mappingName
      bottom
    specify corners
      0. .4 1. 1.
    specify corners
      0. .4 1. .7
    lines
      15 6
    boundary conditions
      1 0 1 0
    share
      0 0 1 0
    exit
*
  fillet
    choose curves
    bottom (side=0,axis=1)
    right (side=1,axis=0)
    orient curve 1+ to curve 2-
    compute fillet
    exit
*
  hyperbolic
    mappingName
      fillet
    distance to march 
     .25
    uniform dissipation coefficient
     .2
    grow grid in opposite direction
    generate
    share
      0 0 1 0
    exit
  exit this menu
*
  generate an overlapping grid
    background
    right
    bottom
    fillet
    done choosing mappings
    * display intermediate results
    change parameters
      * turn on hole cutting for a shared side
      shared sides may cut holes
        fillet
        all
      done
    done
#   debug 
#      3
    compute overlap
#  continue
#  continue
#  continue
#  continue
#  continue
#  continue
*    pause
  exit
*
save an overlapping grid
filletThree.hdf
filletThree
exit
