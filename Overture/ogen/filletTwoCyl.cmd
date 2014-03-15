*
* Create a grid exterior to two intersecting pipes
*
create mappings
*
* Here is the box
*
  Box
    specify corners
      -1. -1. -1. 1. 1. 1.
    lines
      32 32 32  
    mappingName
      box
    share
      1 1 0 2 0 0
    exit
*
  Cylinder
    mappingName
      main-cylinder
    orientation
      1 2 0
    bounds on the axial variable
      -1. 1.
    bounds on the radial variable
      .5 .75
    boundary conditions
      -1 -1 1 2 3 0
    lines
      31 21 6
    share
      0 0 1 1 3 0
    exit
*
  Cylinder
    mappingName
      top-cylinder
    orientation
      2 0 1
    bounds on the axial variable
      .25 1.
    bounds on the radial variable
      .3 .6
    boundary conditions
      -1 -1 0 2 4 0
    lines
      25 15 5
    share
      0 0 0 2 4 0
    exit
*
  fillet
   choose curves
     main-cylinder (side=0,axis=2)
     top-cylinder (side=0,axis=2)
*   orient curve 1- to curve 2+
   orient curve 1+ to curve 2-
   * define more lines for computing the fillet   
   width .325  .25
   overlap .25
   lines
     61 23
   compute fillet
* pause
   * reduce the lines for actual fillet
   lines
     31 12
   exit
* build a volume grid around the fillet
  hyperbolic
   grow grid in opposite direction
    distance to march .2
    points on initial curve 31 12
    lines to march 7
    uniform dissipation .1
    outward splay .25 .25 .25 .25 (left,right,bottom,top for outward splay BC)
    * show parameters
    BC: bottom outward splay
    BC: top outward splay
    generate
* pause
    mappingName
      cylinderFillet
    share
      0 0 0 0 0 0
    exit
  exit
generate an overlapping grid
  box
  top-cylinder
  main-cylinder
  cylinderFillet
  done
  change parameters
    * OLD way using shared sides:
    * turn on hole cutting for a shared side
*   shared sides may cut holes
*   top-cylinder
*   main-cylinder
*   main-cylinder
*   top-cylinder
*   done
*
*    prevent hole cutting
*     cylinderFillet
*     all
*    done
*
*   NEW way using manual shared sides.
    specify shared boundaries
      cylinderFillet
        front  (side=0,axis=2)
      0 30 -1 5 0 0 
      main-cylinder
        front  (side=0,axis=2)
      normal matching angle
        30.
      done
      cylinderFillet
         front  (side=0,axis=2)
       0 30 6 12 0 0
       top-cylinder
         front  (side=0,axis=2)
       done
      main-cylinder
        front  (side=0,axis=2)
      0 30 0 20 0 0
      cylinderFillet
        front  (side=0,axis=2)
      done
      top-cylinder
        front  (side=0,axis=2)
      0 24 -1 14 0 0
      cylinderFillet
        front  (side=0,axis=2)
      done
      done
    ghost points
      all
      2 2 2 2 2 2
  exit
*
  change the plot
    toggle grid 0 0
    exit this menu
  x+r:0 30
  y+r:0
*  display intermediate results
  compute overlap
*  pause
exit
*
save an overlapping grid
filletTwoCyl.hdf
filletTwoCyl
exit
