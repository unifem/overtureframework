*
* Ellipsoid in a box
*
create mappings
*
  Box
    set corners
      -3.5 3.5 -2. 2. -2. 2. 
    lines
      57 33 33 
    exit
*
  CrossSection
    mappingName
      ellipsoidWithSingularity
    ellipse
    a,b,c for ellipse
      1. 1. 2.
*    inner radius is at 1
    outer radius
     1.25
    lines
      33 33 7 33 33 9 33 41 9   19 25 5
    share
      0 0 0 0 1 0
* pause
    exit
* 
  rotate/scale/shift
    transform which mapping
      ellipsoidWithSingularity
    rotate
     90 1
     0 0 0
    mappingName
     ellipsoid
* pause
    exit
*
  reparameterize
    orthographic
    specify sa,sb
      .5  .5
    exit
    mappingName
      north-pole
    lines
      9 9 7 9 9 9   17 17 9   11 11 5
    share
      0 0 0 0 1 0
* pause
    exit
*
  reparameterize
    mappingName
      south-pole
    orthographic
      choose north or south pole
        -1
      specify sa,sb
        .5 .5
      exit
    lines
      9 9 7 9 9 9   17 17 9 11 11 5
    share
      0 0 0 0 1 0
    exit
*
* now remove the singularity since ogen has trouble interpolating there
*
*  reparameterize 
*    transform which mapping
*     ellipsoidWithSingularityRotated
*    restrict parameter space
*      set corners
*        0. 1. .05 .95 0. 1.
*    exit
*    lines
*      33 33 9
*    mappingName
*     ellipsoid
* pause
*   exit
exit
*
  generate an overlapping grid
    box
    ellipsoid
    north-pole
    south-pole
  done
  change parameters
*  interpolation type
*    explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
*     change the plot
*       toggle grid 0 0
*       set view:0 0 0 0 1 0.79395 0.218432 -0.56739 0.243403 0.740984 0.625857 0.557134 -0.635003 0.535138
*  
*     exit this menu
* 
*   bigger:0
*  bigger:0
*  query a point
*    interpolate point 1
*    pt: grid,i1,i2,i3: 0 13 15 12
*
exit
*
* save an overlapping grid
save a grid (compressed)
ellipsoid.bbmg.hdf
ellipsoid
exit    
