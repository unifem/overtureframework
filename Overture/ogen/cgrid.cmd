*
* make a c-grid
*
create mappings
*
$factor=2; $grid = "cgrid.hdf";
* 
$ds=.025/$factor;
*
  spline
    enter spline points
      15
      1.    1.e-4
      .9    1.e-4
      .8    1.e-4
      .7    1.e-4
      .6    1.e-4
      .5    1.e-4
      .25   -.1
*      .005  -.03
      0.    0.
*      .005 +.03
      .25  +.1
      .5    -1.e-4
      .6    -1.e-4
      .7    -1.e-4
      .8    -1.e-4
      .9    -1.e-4
      1.    -1.e-4
    shape preserving (toggle)
    lines 
     $nx = int( 3./$ds + 1.5 );
     $nx
    curvature weight
     2.
    mappingName
      c-surface
    exit
*
  hyperbolic
    distance to march
      $dist = .2/$factor;
      $dist
    lines to march 11
    uniform dissipation coefficient
      .02
    grow grid in opposite direction
    geometric stretching, specified ratio
      1.1
    generate
* 
    mappingName
      c-grid
    * use robust inverse
    boundary conditions
      0 0 1 0
    share
      0 0 1 0
    exit
*
  rectangle
*
    mappingName
      backGround
    set corners
      -.5 1.5 -.5 .5
    lines
     $nx = int( 2./$ds + 1.5);
     $ny = int( 1./$ds + 1.5);
      $nx $ny
    exit
*
exit
generate an overlapping grid
    backGround
     c-grid
  done
  * display intermediate results
  change parameters
    mixed boundary
      c-grid
      bottom (side=0,axis=1)
      c-grid
       r matching tolerance
         .01
      * determine
       done
    done
    ghost points
      all
      2 2 2 2 2 2
  exit
  * display intermediate
  * set debug
  *    15
  compute overlap
  exit
*
save an overlapping grid
cgrid.hdf
cgrid
exit


