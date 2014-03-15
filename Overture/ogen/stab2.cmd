*
* Create a 'submarine' grid.
*
create mappings
  *
  * First create the outline of the hull.
  * Go all the away round so the shape is
  * symmetric at the back and front
  spline
    mappingName
    hull-profile
    enter spline points
    29
    0. 0.
    .10718 .4
    .4 .69282
    .8 .8
    2. .8
    3. .8
    4. .8
    5. .75
    6. .6
    7. .5
    8. .425
    8.6 .4
    8.8 .34641
    8.94641 .2
    9. 0.
    *
    8.94641 -.2
    8.8 -.34641
    8.6 -.4
    8. -.425
    7. -.5
    6. -.6
    5. -.75
    4. -.8
    3. -.8
    2. -.8
    .8 -.8
    .4 -.69282
    .10718 -.4
    0. 0.
    lines
    51
    * pause
    periodicity
    2
    exit
  *
  *  Take the top half of the curve hull
  *
  reparameterize
    restrict parameter space
      specify corners
      0. .5
      exit
    * pause
    exit
  * Stretch the grid lines
  stretch coordinates
    transform which mapping?
    reparameterized-hull-profile
    stretch
      specify stretching along axis=0 (x1)
        layers
        2
        1. 5. 0
        1. 7. 1.
        exit
      exit
    exit
  *
  mapping from normals
    extend normals from which mapping?
    stretched-reparameterized-hull-profile
    normal distance
    -.75
    * pause
    exit
  *
  body of revolution
    mappingName
    hull
    tangent of line to revolve about
    1. 0. 0.
    lines
    51 31 11
    boundary conditions
    0  0 -1 -1 1 0
    share
    0 0 0 0 1 0
    exit
  *
  * ----- Build the front stabilizer ----
  *
  CrossSection
    a,b,c for ellipse
      .35 .15 .5
    a,b,c for ellipse
      .3 .1 .75
    start axial
      .3
    outer radius
      3.
    mappingName
      stab-volume
    centre for ellipse
      1. .25 +.5
    exit
  view mappings
    hull
    stab-volume
    plot non-physical boundaries (toggle)

