*  inner electrode
open a data-base
  innerElectrode.hdf
open an old file read-only
get from the data-base
  innerElectrode
hyperbolic
    choose the initial curve
    create a curve from the surface
    turn on all sub-surfaces
    choose an edge
      specify edge curves
        2 12 7 
      done
    done
    exit
    grow grid in opposite direction
    edit initial curve
     * restrict the domain
     *  0. 1. .0001 .999 .001 .99  0. 1.  .0001 .9999
      curvature weight
        1.
      exit
    boundary conditions for marching
    left
    fix y, float x and z
    right
    fix y, float x and z
    bottom
      free
    exit
    project ghost points
      left   (side=0,axis=0)
      do not project ghost points
      right  (side=1,axis=0)
      do not project ghost points
      exit
    distance to march
      490. 350. 6.
    lines to march
      71 35 2
    curvature weight
      6.
    uniform dissipation coefficient
      .001
    y-r
    y-r
    generate
