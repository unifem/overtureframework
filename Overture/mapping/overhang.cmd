*
*  make the grids that go around the base of the ports 
* but NOT around the nasty corners.
*  
* 
open a data-base
  lowerPortCylinder.hdf
open an old file read-only
get from the data-base
  lowerPortCylinder
*
* right port base, overhang
*
hyperbolic
    start from which curve/surface?
      lowerPortCylinder
    choose the initial curve
    create a curve from the surface
      specify active sub-surfaces
        0
        3
        -1
      choose an edge
        specify an edge curve
          2 8
        done
      exit
      edit initial curve
      * curve is parameterized clockwise when looked from on top
      * .44 1.13 
      restrict the domain
        .09 .48
        lines
          15
      exit
      distance to march
       6. 5.75 6.
      lines to march
       7
      grow grid in opposite direction
      boundary conditions
        0 0 1 0
      generate
      x+r 90
      bigger
      bigger
      bigger
      bigger
    mappingName
     rightPortBaseOverhangSurface
     * pause
    exit
*
  hyperbolic
    reset
    start from which curve/surface?
     rightPortBaseOverhangSurface
    distance to march
      9. 7
    lines to march
      8
    grow grid in opposite direction
    boundary conditions for marching
      bottom (side=0,axis=1)
      fix y, float x and z
      * top    (side=1,axis=1)
      * fix y, float x and z
      exit
    uniform dissipation coefficient
      .4
    generate
    mappingName
      rightPortBaseOverhangVolume
    boundary conditions
      0 0 3 0 1 0
    share  
      0 0 3 0 1 0
    pause
   exit
