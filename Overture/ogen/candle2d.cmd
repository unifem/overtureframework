create mappings 
  * 
  open a data-base 
  /home/henshaw/Overture/ogen/candleGrids2d.hdf
    open an old file read-only 
    get all mappings from the data-base 
  close the data-base
*
  rectangle
    set corners
     -5. 15. 0 12.   -10 30 0. 20.    -15. 65. 0 20. 
    boundary conditions
       2 5 1 4 
    share
       0 0 2 0 0 0
    lines
      89 45   181 45 181  241 61 241 
    mappingName
      backGround
    exit
exit this menu
*
*
generate an overlapping grid
backGround
candleGridA
candleGridB
done
*
*  change the plot
*    toggle grid 0 0
*    bigger:0
*   exit this menu
*
  change parameters
*     
 *      interpolation type
 *       explicit for all grids
   ghost points
      all
      2 2 2 2 2 2
    boundary discretization width
       all
       5 5 5 5 5 5
    exit
*  display intermediate results
* pause
   compute overlap
 pause
 exit
*
save an overlapping grid
candle2d.hdf
candle
exit 












*
* 
create mappings
  spline
    enter spline points
    23
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
    91
    mappingName
    crossSectionA
    curvature weight
    1.
    exit
*
  body of revolution
    choose a point on the line to revolve about
      25. 0 0
    lines
      91 91
    mappingName
    surfaceA
    exit
*
  hyperbolic
    target grid spacing -1, 0.075 (tang,normal, <0 : use default)
    lines to march 25  
    uniform dissipation 0.2
    volume smooths 40
    generate
    BC: left fix y, float x and z
    BC: right float y, fix x and z
    boundary conditions
     2 1 -1 -1 1 0
    share
     2 1 0 0 0 0
    name candleVolumeA
pause
    exit
*
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
    31
    curvature weight
    1.
    mappingName
     crossSectionB
* pause
    exit
*
  body of revolution
    choose a point on the line to revolve about
      25. 0 0
    lines
      31 91
    mappingName
    surfaceB
* pause
    exit
*
  hyperbolic
    target grid spacing -1, 0.075 (tang,normal, <0 : use default)
    lines to march 25  
    BC: right fix y, float x and z
    BC: left float y, fix x and z
    generate
    boundary conditions
     1 2 -1 -1 1 0
    share
     1 2 0 0 0 0
    name candleVolumeA
pause
    exit
  builder
    plot reference surface 0
    plot surface grids 0
    colour boundaries by BC number
    save grids to a file...
    file name: candleGrids.hdf
    save file
    exit

   builder


    exit














create mappings
  spline
    enter spline points
    30
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
    2.5 6.325
    4. 5.65
    5.5 4.975
    7 4.3      **
    7    3.6      ****
    5.625 3.825
    4.25 4.05
    2.875 4.275
    1.5  4.5    ****
    1.5  3.95
    1.5    3.4   ****
    3.825  2.75
    6.15   2.1
    8.475  1.45
    10.8   .8     *****
    11. 0.
    shape preserving (toggle)
    lines
    91
    mappingName
    crossSectionA
    exit


  body of revolution
    choose a point on the line to revolve about
    22. 0. 0.
    x+r:0
    lines
    91 91
    x+r:0
    mappingName
    candleSurface
    choose a point on the line to revolve about
    25. 0. 0.
    exit
  builder
    create volume grid...
    exit
  hyperbolic
    set view:0 0.486405 -0.0151057 0 3.42574 1 0 0 0 0.939693 0.34202 0 -0.34202 0.939693
    x-r:0
    distance to march .1 