*
* Create a grid for ice forming on a circle.
* Use this grid with the primer/deform.C program
*
*
create mappings
  annulus
    inner and outer radii
      .5 .8
    start and end angles
     -.3333 .3333
    lines
      61 9 
    boundary conditions
      0 0 1 0
    mappingName
    annulus
    share
    0 0 1 0
    exit
*
  rectangle
    set corners
    -2. 2. -2. 2.
    lines
     81 81 
    mappingName
     backGround
    exit
*
  nurbs (curve)
    enter points
      13
    $rad=.5; $Pi=4.*atan2(1.,1.);
    $x0=0.; $y0=1.*$rad;
    $theta=$Pi* 5./180.; $x1=-$rad*sin($theta); $y1=$rad*cos($theta);
    $theta=$Pi*10./180.; $x2=-$rad*sin($theta); $y2=$rad*cos($theta);
    $theta=$Pi*15./180.; $x3=-$rad*sin($theta); $y3=$rad*cos($theta);
    $theta=$Pi*20./180.; $x4=-$rad*sin($theta); $y4=$rad*cos($theta);
    $x0 $y0
    $x1 $y1
    $x2 $y2
    $x3 $y3
    $x4 $y4
    -.7  .5
    -.6  0.
    -.7 -.5
    $y0=-$y0; $y1=-$y1; $y2=-$y2; $y3=-$y3; $y4=-$y4;
    $x4 $y4
    $x3 $y3
    $x2 $y2
    $x1 $y1
    $x0 $y0
    lines
      31
    exit
  hyperbolic
    distance to march .25
    lines to march 9
    generate
    mapping parameters
    Share Value: bottom  1
    close mapping dialog
    name ice
    exit
  exit this menu
*
generate an overlapping grid
  backGround
  annulus
  ice
  done choosing mappings
  change parameters
    shared sides may cut holes
      ice
      annulus
      done
    ghost points
      all
      2 2 2 2 2 2
    exit
*
  compute overlap
exit
*
save an overlapping grid
  iceCircle.hdf
  iceCircle
exit



  change parameters
    shared boundary tolerances
      annulus
        bottom (side=0,axis=1)
      ice
        bottom (side=0,axis=1)
      r matching tolerance
      .01
      done
      done
    exit
  compute overlap


