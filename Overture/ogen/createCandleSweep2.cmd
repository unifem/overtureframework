*    noSlipWall=1,
*    inflowWithVelocityGiven=2,
*    inflowWithPressureAndTangentialVelocityGiven=3,
*    slipWall=4,
*    outflow=5,
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
  line 
    number of dimensions
    2
    set end points
     -5 7   9.7  4.3      1 7  7 4.3
    mappingName
      line1
    exit
*  line 
*    number of dimensions
*    2
 *   set end points
 *    -.6 2.6   5.5 8.5        1 2.6  7 8.5
*    mappingName
*      line2
*    exit
  spline
    enter spline points
     14
    0 0
    0 2
    0 4
    0 5.8
    -1. 6.4
    -2 7
    -2 8
   -.875 8.25
   .25 8.5
   1.375 8.75
    2.5 9
    2.6 8.5
    1.8 7.75
    1  7       **
    shape preserving (toggle)
    lines
      121   161  89
    mappingName
    crossSectionA
    curvature weight
    1.
    exit
*
  stretch coordinates
    stretch
      specify stretching along axis=0 (x1)
        layers
        3
        .25 20. .45
        .125 30 .82
        .125 20 1.
        exit
    exit
    mappingName
    stretched-crossSectionA
* pause
  exit
*
*
  hyperbolic
    backward
    marching spacing...
    spacing: geometric
    target grid spacing -1, .025 (tang,normal, <0 : use default)
    lines to march 21 17
    BC: right match to a mapping
    line1
    normal blending 9 9 (lines: left, right)
    generate
*  The smoothing currently removes the normal BC at r=1
*   GSM:BC: top smoothed
*    GSM:smooth grid
* pause        
    lines
     73 9  53 7   65 9    93  9
    boundary conditions
     1 1 1 0
    share
     2 3 0 0
 *  fourth order
    name candleGridA
*    pause
    exit
*
*
  sweep
    orientation 
      -1
    choose reference
      candleGridA
    choose sweep curve
      candleSweepCurve
*    use center of sweep curve
    specify center
      5. 0. 0. 
   lines
     73 9 161  53 7 121  65 9 121   93 9 93   93 9 181
    boundary conditions
     1 1 1 0 -1 -1 
    share
     2 4 0 0 0 0
    mappingName
      candleVolumeA
pause
  exit
*
  spline
    enter spline points
     10
    1  7       **
    2.5 6.325
    4. 5.65
    5.5 4.975
    7 4.3      **
    7    3.6      ****
    5.625 3.825
    4.25 4.05
    2.875 4.275
    1.5  4.5    ****
    shape preserving (toggle)
    lines
      81 
    mappingName
    crossSectionB
    curvature weight
    1.
    exit
*
  stretch coordinates
    stretch
      specify stretching along axis=0 (x1)
        layers
        1
        .5 8. .5 
        exit
      exit
    mappingName
    stretched-crossSectionB
*   pause
    exit
* 
  hyperbolic
    backward
    BC: right fix x, float y and z
*     BC: left match to a mapping
*      line2
   normal blending 9 9 (lines: left, right)
    target grid spacing -1, 0.03 (tang,normal, <0 : use default)
    spacing: geometric
    lines to march 18 17 14 16
    generate
* pause
    GSM:BC: top smoothed
    GSM:smooth grid
* pause
    boundary conditions
     0 1 1 0
    share
     0 3 4 0
    lines
      49 9   37 7
 *  fourth order
    name candleGridB
*  pause
    exit
*
  sweep
    orientation 
      -1
    choose reference
      candleGridB
    choose sweep curve
      candleSweepCurve
*    use center of sweep curve
    specify center
      5. 0. 0. 
   lines
     49 9 161  37 7 121   73 9 93  181
    boundary conditions
     1 1 1 0 -1 -1 
    share
     1 3 4 0  0 0
    mappingName
      candleVolumeB
 pause
  exit
*
create mappings
  spline
    enter spline points
    6
    1.5    3.4   ****
    3.825  2.75
    6.15   2.1
    8.475  1.45
    10.8   .8     *****
    11.25 0.
    shape preserving (toggle)
    lines
      65 33
    curvature weight
      .75
    mappingName
     crossSectionC
* pause
    exit
*
  hyperbolic
    backward
    BC: right fix y, float x and z
    BC: left float y, fix x and z
    target grid spacing -1, 0.03 (tang,normal, <0 : use default)
    spacing: geometric
    lines to march 18 17 14 16
    generate
    GSM:BC: top smoothed
    GSM:smooth grid
    boundary conditions
     1 1 1 0
    share
     3 2 0 0
    lines
      37 9   29 7  53 9
 *  fourth order
    name candleGridC
* pause
    exit
*
*
  sweep
    orientation 
      -1
    choose reference
      candleGridC
    choose sweep curve
      candleSweepCurve
*    use center of sweep curve
    specify center
      5. 0. 0. 
    boundary conditions
     1 1 1 0 -1 -1 
    share
     3 2 0 0 0 0
    lines
      37 9 161  29 7 121  53 9 93  
    mappingName
      candleVolumeC
pause
   exit
*
*
* make a sweep rectangle to enclose the other sweep mappings
* 
  rectangle
    set corners
      -7. 16. 0 14
    lines
      57 37  49 29 
    share
      0 0 2 0
    mappingName
     sweepRectangle
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
      57 37 161  49 29 121  77 48 121
    mappingName
      sweepBox
pause
    exit
*
  open a data-base
  candleSweepGrids2d.hdf
  open a new file
  put to the data-base
  candleGridA
  put to the data-base
  candleGridB
  put to the data-base
  candleGridC
  put to the data-base
    candleGridC
  put to the data-base
    sweepRectangle
  close the data-base
*
  builder
    plot reference surface 0
    plot surface grids 0
    colour boundaries by BC number
    save grids to a file...
    file name: candleSweepGrids.hdf
    save file
    exit









