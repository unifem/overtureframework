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
* ----- Build the stabilizer ----
*
  Circle or ellipse (3D)
    mappingName
      stab-ellipse0
    specify axes of the ellipse
      .35 .1 
    specify centre
      1. 0. .4
    exit
*
  Circle or ellipse (3D)
    mappingName
      stab-ellipse1
    specify axes of the ellipse
      .35 .1
    specify centre
      1. 0. 1.
    exit
*
  Circle or ellipse (3D)
    mappingName
      stab-ellipse2
    specify axes of the ellipse
      .35 .1
    specify centre
      1. 0. 1.4
    exit
*
  CrossSection
    mappingName
      stabilizer-surface
    general
      3
    stab-ellipse0
    stab-ellipse1
    stab-ellipse2
    polar singularity at end
    * pause
  exit
* orthographic patch for the cap of the stabilizer
  reparameterize  
    mappingName
      stab-cap-surface
  exit
* stretch lines on cap
  stretch coordinates
    mappingName
      stretched-stab-cap-surface
    stretch
      specify stretching along axis=0 (x1)
        layers
          1
          1. 5. .5
        exit
      exit
    exit
*  volume mapping for the cap
  mapping from normals
    mappingName
      stab-cap
    normal distance
      .35
    extend normals from which mapping?
      stretched-stab-cap-surface
    lines
      15 15 9
    share
      0 0 0 0 1 0
    * pause
    exit
* volume mapping for the front stab body
  mapping from normals
    mappingName
      stabilizer-volume
    normal distance
      .35
    extend normals from which mapping?
      stabilizer-surface
    lines
      31 15 5
    boundary conditions
      -1 -1 0 0 1 2
    share
      0 0 0 0 2 0
    * pause
    exit
*
  join
    mappingName
      stabilizer
    end of join
     .9
    choose curves
      stabilizer-volume
    hull (side=0,axis=2)
    * pause
    lines
      31 11 7
    boundary conditions
      -1 -1 2 0 1 0
    share
       0  0 1 0 2 0
    compute join
    * pause
   exit

