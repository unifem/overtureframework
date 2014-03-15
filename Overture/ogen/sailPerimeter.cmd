create mappings
*
open a data-base
   sailSurfaces.hdf
   open an old file read-only
   get all mappings from the data-base
* pause
*
*
  nurbs (surface)
    read NURBS data, format cheryl
    ../mapping/PerimeterSurf.nurbs
    lines
     61 351
    periodicity
     0 2 
    * pause
    mappingName
    perimeterReferenceSurface
    exit
*
   DataPointMapping
     build from a mapping
       perimeterReferenceSurface
     mappingName
       perimeterReferenceSurface-dp
     * pause
  exit
  *
  composite surface
    add a mapping
      windWardSurface
    add a mapping
      leeWardSurface
    add a mapping
       perimeterReferenceSurface-dp
    plot normals (toggle)
    change the sign of a normal
      1
*    change the sign of a normal
*      2
    * pause
    mappingName
      sailComposite
    exit
*
*  Build the grid for the cap
*
  hyperbolic
    start from which curve/surface?
      sailComposite
    * perimeterReferenceSurface-dp
    mappingName
      capSurface
    choose the initial curve
      create a curve from the surface
        reparameterized coordinate line
          2
          axis1=.5
    edit initial curve
      restrict the domain
        -.05 .05   -.016 .016  .02 .98
      lines
        21 31  41 21  15
      exit
    grow grid in both directions
    lines to march
      7 
      7 
    distance to march
      7. 
      7. 
    geometric stretching, specified ratio
      1.1
    uniform dissipation coefficient
      .3
    generate
    pause
  exit
*
  hyperbolic
    mappingName
      capVolume
    lines to march
     7 6
    distance to march
      15 10.
    grow grid in opposite direction
    implicit coefficient
       0.
    uniform dissipation coefficient
      .03
    generate
    * pause
    boundary conditions
     0 0 0 0 1 0
    share
     0 0 0 0 1 0
  exit
*
*  Main perimeter grid
*
  hyperbolic
    start from which curve/surface?
      sailComposite
    choose the initial curve
      create a curve from the surface
        reparameterized coordinate line
          2
          axis1=.5
    edit initial curve
      restrict the domain
       .05 .95 .1  .9      .05 .95
      lines
        151
      exit
    grow grid in both directions
    lines to march
      11
      11
    distance to march
      30.
      30.
    uniform dissipation coefficient
      .4
    geometric stretching, specified ratio
     1.15
    generate
    * pause
    mappingName
      perimeterSurface
   exit
*
*   volume grid
*
  hyperbolic
    start from which curve/surface?
      perimeterSurface
    mappingName
      perimeterVolume
    lines to march
      7
    distance to march
      20.
    grow grid in opposite direction
    implicit coefficient
       0.
    uniform dissipation coefficient
      .01
    geometric stretching, specified ratio
      1.15
    generate
    * pause
    boundary conditions
     0 0 0 0 1 0
    share
     0 0 0 0 1 0
  exit
*
  open a data-base
   sailPerimeter.hdf
     open a new file
   put to the data-base
     perimeterVolume
   put to the data-base
     capVolume
   close the data-base
exit
*
exit