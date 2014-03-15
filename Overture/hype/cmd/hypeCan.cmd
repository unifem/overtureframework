* build a grid around the corner of a soup can
*
* first build the soup can
*
  Annulus
    make 3d (toggle)
    1.
    exit
  Cylinder
    surface or volume (toggle)
    exit
  composite surface
    CSUP:add a mapping Annulus
    CSUP:add a mapping Cylinder


    exit
  *
  * now the surface grid
  hyperbolic
    choose the initial curve
      create a curve from the surface
        specify active sub-surfaces
          0
          done
        choose an edge
          specify edge curves
            3
            done
          done
        exit
    distance to march
    .3
    lines to march
    7
    grow grid in both directions
    x-r 45
    plot shaded boundaries on reference surface (toggle)
    smaller
*     save postscript
*       hypeCan.rf.ps
    pause
    generate
    plot reference surface (toggle)
*     save postscript
*       hypeCan.surf.ps
    pause
    exit
* volume grid
  hyperbolic
    distance to march
      .3
    lines to march
      6
    grow grid in opposite direction
    generate
    change plot parameters
      plotAxes (toggle)
      plot grid lines on coordinate planes
        0 0
        0 5
        0 10
        0 15
        -1
      plot non-physical boundaries (toggle)
      reset
      x-r 90
      y-r 20
      x+r 30
      exit
    bigger
*     save postscript
*       hypeCan.vol.ps
    pause
 exit
exit
