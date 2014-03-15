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
* left port overhang
*
hyperbolic
    start from which curve/surface?
      lowerPortCylinder
    choose the initial curve
    create a curve from the surface
      specify active sub-surfaces
        4 5
        done
      choose an edge
        specify edge curves
          2 16
          done
        done
      exit
      edit initial curve
      * curve is parameterized clockwise when looked from on top
      * .69 1.4
      restrict the domain
         .36 .73 
        lines
         15
      exit
      distance to march
        6. 5.5  6. 
      lines to march
        7
      * grow grid in opposite direction
      boundary conditions
        0 0 1 0
      generate
      mappingName
       leftPortBaseOverhangSurface
      * pause
      exit
*
* right port base, overhang
*
hyperbolic
    start from which curve/surface?
      lowerPortCylinder
    choose the initial curve
    create a curve from the surface
      specify active sub-surfaces
        0 3
        done
      choose an edge
        specify edge curves
          2 8
          done
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
      * grow grid in opposite direction
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
* right port base, no overhang
*
hyperbolic
    start from which curve/surface?
      lowerPortCylinder
    choose the initial curve
    create a curve from the surface
      specify active sub-surfaces
        0 3
        done
      choose an edge
        specify edge curves
          2 8
          done
        done
      exit
      edit initial curve
      * curve is parameterized clockwise when looked from on top
      * remember: ghost points used when interpolating
      restrict the domain
         .44 1.13    .42 1.15  .44 1.13  
      exit
      distance to march
       10. 9.  10. 11.
      lines to march
       10  11  12
      * grow grid in opposite direction
      boundary conditions
        0 0 1 0
      generate
      x+r 90
      bigger
      bigger
      bigger
      bigger
    mappingName
     rightPortBaseSurface
     * pause
    exit
*
* left port base
*
hyperbolic
    start from which curve/surface?
      lowerPortCylinder
    choose the initial curve
    create a curve from the surface
      specify active sub-surfaces
        4 5
        done
      choose an edge
        specify edge curves
          2 16
          done
        done
      exit
      edit initial curve
      * curve is parameterized clockwise when looked from on top
      restrict the domain
        .69 1.38   .69 1.39  .685 1.40   .685 1.39      .68 1.41  .69 1.4   
      exit
      distance to march
       10. 9.  10. 11.
      lines to march
       10  11 12
      * grow grid in opposite direction
      boundary conditions
        0 0 1 0
      generate
      mappingName
       leftPortBaseSurface
      * pause
      exit
*
* volume grids
*
  hyperbolic
    reset
    start from which curve/surface?
     rightPortBaseSurface
    distance to march
      9. 7
    lines to march
      8
    grow grid in opposite direction
    boundary conditions for marching
      bottom (side=0,axis=1)
      fix y, float x and z
      exit
    uniform dissipation coefficient
      .4
    geometric stretching, specified ratio
      1.2
    generate
    mappingName
    rightPortBaseVolume
    * pause
    boundary conditions
      0 0 3 0 1 0
    share  
      0 0 3 0 1 0
    * pause
   exit
*
  hyperbolic
    reset
    start from which curve/surface?
     leftPortBaseSurface
    distance to march
      9. 7
    lines to march
      8
    grow grid in opposite direction
    boundary conditions for marching
      bottom (side=0,axis=1)
      fix y, float x and z
      exit
    uniform dissipation coefficient
      .4
    generate
    mappingName
     leftPortBaseVolume
    * pause
    boundary conditions
      0 0 2 0 1 0
    share  
      0 0 2 0 1 0
     * pause
   exit
*
  hyperbolic
    reset
    start from which curve/surface?
     leftPortBaseOverhangSurface
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
      leftPortBaseOverhangVolume
    boundary conditions
      0 0 2 0 1 0
    share  
      0 0 2 0 1 0
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
    * pause
   exit
*
* make the strip that joins the two ports
*
  hyperbolic
    start from which curve/surface?
      lowerPortCylinder
    choose the initial curve
    create a curve from the surface
    project a line
    choose end points
      39 0 129 72 0 108
    exit
    exit
    grow grid in both directions (toggle)
    distance to march
      5.   6
      5.   6
    lines to march
      6
      6
    generate
    * pause
    mappingName
      joinStripSurface
    exit
* make the volume grid for the strip
  hyperbolic
    distance to march
      4
    lines to march
      7
    grow grid in opposite direction
    generate
    mappingName
      joinStrip
    boundary conditions
      0 0 0 0 1 0
    share  
      0 0 0 0 1 0
    * pause
    exit
*
*
  open a data-base
    catPortCylinderEdgePort.hdf
  open a new file
  put to the data-base
    leftPortBaseVolume
  put to the data-base
    rightPortBaseVolume
  put to the data-base
    leftPortBaseOverhangVolume
  put to the data-base
    rightPortBaseOverhangVolume
  put to the data-base
    joinStrip
  close the data-base
  exit this menu

