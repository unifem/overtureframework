create mappings
*
  spline (3D)
    enter spline points
     13
    0. 0. 0.
    1. 0.  10.
    5. 0.  25.
    15. 0.  32.
    25. 0.  30.
    30.75 0.  20.
    34. 0.  10.
    35. 0. 0.
    32.5 0. -10.
    22. 0. -15.
    13.5 0. -12.  -11.95
    5. 0. -7.5
    0. 0. 0.
    periodicity
      2
    lines
      121  91
    mappingName
     candleSweepCurveInitial
    exit
*
   rotate/scale/shift
    scale
     1.4 1. 1.2
    shift 
      10. 0. 0.
    mappingName
     candleSweepCurve
   exit
*
* make a sweep rectangle to enclose the other sweep mappings
* 
  rectangle
    set corners
    -5. 14. 0 12
    lines
    77 48
    share
    0 0 2 0
    exit
*
  sweep
    set view:0 0 0 0 1 0.98555 -0.0213304 0.168037 -0.0579492 0.889718 0.452817 -0.159164 -0.456011 0.875626
    orientation 
      -1
    specify center
      5. 0. 0. 
    boundary conditions
     1 1 1 1 -1 -1 
    share
     0 0 2 0 0 0
    lines
     21 11 151 35 25 151  77 48 121
    mappingName
      sweepBox







