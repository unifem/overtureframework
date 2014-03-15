  Cylinder
    orientation
      1 2 0
    bounds on the axial variable
      -1. 1.
    bounds on the radial variable
      .5 .75
    boundary conditions
      -1 -1 1 2 3 0
    mappingName
      main-cylinder
    lines
      31 21 6
    exit
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
      -1 -1 0 2 3 0
    lines
      25 15 5
    exit
*
  fillet
   * define more lines for computing the fillet   
   lines
     81 41 41 21
    Start curve 1:main-cylinder (side=0,axis=2)
    Start curve 2:top-cylinder (side=0,axis=2)
    orient curve 1+ to curve 2-
    compute fillet
*    pause
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

    lines
      31 12 6
    mappingName
      cylinderFillet
    share
      0 0 0 0 0 0

    exit




   choose curves
     main-cylinder (side=0,axis=2)
     top-cylinder (side=0,axis=2)
   orient curve 1- to curve 2+
   compute
   * reduce the lines for actual fillet
*   lines
*     31 12
