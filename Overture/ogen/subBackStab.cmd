*
* Create a 'submarine' grid.
* This version just adds a back stabilizer to the hull
*
create mappings
*
  Box
    mappingName
      backGround
    specify corners
      -2. -2. -2. 12. 2. 2.5
    lines
      112 32 36  98 28 32   84 24 27   71 21 24
    exit
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
  reparameterize
    orthographic
      specify sa,sb
        .4 .4
      exit
    lines
      11 11 11
    share
      0 0 0 0 1 0
    mappingName
      hull-bow-cap
    exit
*
  reparameterize
    orthographic
      specify sa,sb
        .3 .3
      choose north or south pole
        -1
      exit
    lines
      11 11 11
    share
      0 0 0 0 1
    mappingName
      hull-stern-cap
    exit
*
* ----- Build the stabilizer ----
*
  Circle or ellipse (3D)
    mappingName
      back-stab-ellipse0
    specify axes of the ellipse
      .35 .1 
    specify centre
      8. 0. .2
    exit
*
  Circle or ellipse (3D)
    mappingName
      back-stab-ellipse1
    specify axes of the ellipse
      .30 .1
    specify centre
      8.2 0. .7     8.2 0. .6
    exit
*
  Circle or ellipse (3D)
    mappingName
      back-stab-ellipse2
    specify axes of the ellipse
      .25 .1
    specify centre
      8.4 0. 1.2    8.4 0. 1.0
    exit
*
  CrossSection
    mappingName
      back-stabilizer-surface
    general
      3
    back-stab-ellipse0
    back-stab-ellipse1
    back-stab-ellipse2
    polar singularity at end
  exit
* orthographic patch for the cap of the stabilizer
  reparameterize  
    mappingName
      back-stab-cap-surface
  exit
* stretch lines on cap
  stretch coordinates
    mappingName
      stretched-back-stab-cap-surface
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
      back-stabilizer-cap
    normal distance
      .25
    extend normals from which mapping?
      stretched-back-stab-cap-surface
    lines
      15 15 9
    share
      0 0 0 0 2 0
    * pause
    exit
* volume mapping for the front stab body
  mapping from normals
    mappingName
      back-stabilizer-volume
    normal distance
      .25
    extend normals from which mapping?
      back-stabilizer-surface
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
      back-stabilizer
    end of join
     .9
    choose curves
      back-stabilizer-volume
    hull (side=0,axis=2)
    * pause
    lines
      31 11 7
    boundary conditions
      -1 -1 2 0 1 0
    share
       0  0 1 0 2 0
    compute join
   exit
*
*  ---- Now build actual front stabilizers by rotating and scaling
*       the basic stabilizer
*
  rotate/scale/shift
    transform which mapping?
      back-stabilizer
    rotate
      0 0
      0 0 0
    mappingName
      back-stabilizer1
    exit
  rotate/scale/shift
    transform which mapping?
      back-stabilizer-cap
    rotate
      0 0
      0 0 0
    mappingName
      back-stabilizer1-cap
    exit
*
  rotate/scale/shift
    transform which mapping?
      back-stabilizer
    rotate
      90 0
      0 0 0
    mappingName
      back-stabilizer2
    exit
  rotate/scale/shift
    transform which mapping?
      back-stabilizer-cap
    rotate
      90 0
      0 0 0
    mappingName
      back-stabilizer2-cap
    exit
*
  rotate/scale/shift
    transform which mapping?
      back-stabilizer
    rotate
      180 0
      0 0 0
    mappingName
      back-stabilizer3
    exit
  rotate/scale/shift
    transform which mapping?
      back-stabilizer-cap
    rotate
      180 0
      0 0 0
    mappingName
      back-stabilizer3-cap
    exit
*
   DataPointMapping
     build from a mapping
       back-stabilizer1
       * stabilizer
     mappingName
      back-stabilizer1-dp
  exit
*
   DataPointMapping
     build from a mapping
       back-stabilizer1-cap
       * stabilizer-cap
     mappingName
       back-stabilizer1-cap-dp
  exit
*
   DataPointMapping
     build from a mapping
       back-stabilizer2
       * stabilizer
     mappingName
      back-stabilizer2-dp
  exit
*
   DataPointMapping
     build from a mapping
       back-stabilizer2-cap
       * stabilizer-cap
     mappingName
       back-stabilizer2-cap-dp
  exit
exit
generate an overlapping grid
     backGround
     hull
     hull-bow-cap
     hull-stern-cap
*     back-stabilizer1-dp
*     back-stabilizer1-cap-dp
     back-stabilizer2-dp
     back-stabilizer2-cap-dp
  done choosing mappings
    change the plot
      toggle grids on and off
      0 : backGround is (on)
      exit this menu
      colour boundaries by share value (toggle)
      exit this menu
  pause
  compute overlap
  pause
exit
*
save an overlapping grid
sub.hdf
sub
exit

    