*
* create a grid for testing the mismatch at corners
*
create mappings
  spline
    mappingName
      bottom
    enter spline points
      5
      0. 0.
      .25 .1
      .5 .2
      .75 .1
      1. 0.
    exit
  spline
    mappingName
      left
    enter spline points
      5
       0. 0
       0.05 .25
      -.05 .5
      -.2 .75
      -.3 1.
    exit
  spline
    mappingName
      top
    enter spline points
      5
      -.3 1.
      -.1  1.1
      .3  1.2
      .6 1.1
      1. .9
    exit
  spline
    mappingName
      right
    enter spline points
      5
      1. 0.
      1.1 .25
      1.2 .5
      1.1 .75
      1. .9
    exit
  tfi
    mappingName
      baseGrid
    choose bottom curve (r_2=0)
      bottom
    choose top curve    (r_2=1)
      top
    choose left curve   (r_1=0)
      left
    choose right curve  (r_1=1)
      right
    lines
      8 8 
    share
      1 0 2 0
   exit
*
  reparameterize
    mappingName
      refinement0
    restrict parameter space
      specify corners
       0. 0. .5 .5
      exit
    lines
      7 7 
    boundary conditions
       1 0 1 0    
    share
      1 0 2 0
    exit
* Now shift the refinement a bit so it doesn't match
  rotate/scale/shift
    mappingName
      refinement
    shift
       -.0075 -.005
    exit
  exit
*
  generate an overlapping grid
    baseGrid
    refinement
    done
    compute overlap
  exit
*
save an overlapping grid
mismatch.hdf
mismatch
exit
