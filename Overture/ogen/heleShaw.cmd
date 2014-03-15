*
* heleShaw.cmd
*
create mappings
  Annulus
    boundary conditions
    -1 -1 0 1
    inner radius
    .4
    lines
    41 11
    exit
  spline
    periodicity
    2
    enter spline points
    9
    .4 0.
    .3 .3
    0. .5
    -.3 .3
    -.4 0.
    -.3 -.3
    0. -.4
    .3 -.3
    .4 0.
    exit
  mapping from normals
    extend normals from which mapping?
    splineMapping
    normal distance
      .3
    lines
     45 7
    exit
  exit this menu
  generate an overlapping grid
    Annulus
    normal-splineMapping
    done choosing mappings
    compute overlap
    pause
    exit
  save an overlapping grid
    heleShaw.hdf
    heleShaw
  exit

