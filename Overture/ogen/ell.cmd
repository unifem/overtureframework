*
* Ellipsoid in a box
*
create mappings
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
    boundary conditions
      -1 -1 1 2 3 4
    share
      0 0 0 0 1 0
    exit
* 
  rotate/scale/shift
    transform which mapping
      ellipsoidWithSingularity
    rotate
     90 1
     0 0 0
    mappingName
     ellipsoidWithSingularityRotated
    exit
*
* now remove the singularity since ogen has trouble interpolating there
*
  reparameterize 
    transform which mapping
     ellipsoidWithSingularityRotated
    restrict parameter space
      set corners
        0. 1. .05 .95 0. 1.
    exit
    lines
      9 9 7  17 17 7 33 33 9
    mappingName
     ellipsoid
* pause
   exit
exit
*
  generate an overlapping grid
    ellipsoid
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
pause
*   change the plot
*     toggle grid 0 0
*     set view:0 0 0 0 1 0.79395 0.218432 -0.56739 0.243403 0.740984 0.625857 0.557134 -0.635003 0.535138
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
save an overlapping grid
ell.bbmg.hdf
ellipsoid
exit    

















*
* Ellipsoid in a box
*
create mappings
*
*
  CrossSection
    mappingName
      ellipsoidWithSingularity
    ellipse
    a,b,c for ellipse
      1. 1. 2.
    lines
      33 33 9 33 41 9   19 25 5
    boundary conditions
     -1 -1 1 2 3 4
    share
      0 0 0 0 1 0
    exit
* 
  rotate/scale/shift
    transform which mapping
      ellipsoidWithSingularity
    rotate
     90 1
     0 0 0
    mappingName
     ellipsoidWithSingularityRotated
    exit
*
* now remove the singularity since ogen has trouble interpolating there
*
  reparameterize 
    transform which mapping
     ellipsoidWithSingularityRotated
    restrict parameter space
      set corners
        0. 1. .02 .98 0. 1.
    exit
    lines
      7 7 7  11 11 7 33 33 9
    mappingName
     ellipsoid
  pause
   exit
exit
*
  generate an overlapping grid
    ellipsoid
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
pause
exit
*
save an overlapping grid
ell.hdf
ellipsoid
exit    
























create mappings
*
  CrossSection
    mappingName
      ellipsoid
    choose type
    ellipse
    a,b,c for ellipse
      1. 1. 2
    lines
      11 21 5
    boundary condition
      1 1 -1 -1 1 2
    share
      0 0 0 0 1 2
    exit
*
  reparameterize
    mappingName
      north-pole
    lines
      11 11 5
    boundary condition
      0 0 0 0 1 2
    share
      0 0 0 0 1 2
    exit
*
  reparameterize
    mappingName
      south-pole
    orthographic
      choose north or south pole
        -1
      exit
    lines
      11 11 5
    boundary condition
      0 0 0 0 1 2
    share
      0 0 0 0 1 2
    exit
   pause
exit
*
  generate an overlapping grid
    ellipsoid
    north-pole
    south-pole
