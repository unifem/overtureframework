*
*  one valve for a moving grid computation
*
***************************************************************************
* scale number of grid points in each direction by the following factor
*   ***NOTE: restore file to factor=1 for regression tests *****
$factor=1; $name = "oneValve.hdf";
* $factor=2; $name = "oneValve2.hdf"; 
*  $factor=4; $name = "oneValve4.hdf"; 
* $factor=8; $name = "oneValve8.hdf"; 
* $factor=16; $name = "oneValve16.hdf"; 
printf(" factor=$factor\n");
*
* Define a subroutine to convert the number of grid points
sub getGridPoints\
{ local($n1,$n2)=@_; \
  $nx=int(($n1-1)*$factor+1.5); $ny=int(($n2-1)*$factor+1.5); \
}
*
***************************************************************************
*
create mappings
*************************************
*    --valve head--
*************************************
  SmoothedPolygon
    mappingName
      valve-head
    vertices
    7
    -.1 .3
    -.7 .3
    -1.1 0.
    0. 0.
    1.1 0.
    .7 .3
    .1 .3
    n-dist
    fixed normal distance
      $dist=-.25/($factor); 
      $dist
**    -.2   * keep fixed for now
    t-stretch
    .1  50.
    .1   5.
    .1  30.
    .0  1.
    .1  30.
    .1  5.
    .1  50.
    lines
*
    getGridPoints(101,13); $ny=13;
      $nx $ny 
*
    n-stretch
    1. 8. 0.
    sharpness
    30.
    30.
    30
    30.
    30
    30.
    30.
    boundary conditions
      1 1 1 0
    share
     1 1 2 0 
* pause
    exit
  *
*************************************
*    --lower valve shaft left--
*************************************
  SmoothedPolygon
    mappingName
      lower-valve-shaft-left
    vertices
    2
      -.1 .3
      -.1 .7
    n-dist
    fixed normal distance
      $dist=.25/($factor); 
      $dist
    lines
*
    getGridPoints(9,9); $ny=9;
      $nx $ny 
*
    n-stretch
      1. 8. 0.
    boundary conditions
      0 0 1 0 
    share
      0 0 1 0 
* pause
    exit
  *
*************************************
*    --upper valve shaft left--
*************************************
  SmoothedPolygon
    mappingName
      upper-valve-shaft-left
    vertices
    2
      -.1 .7 
      -.1 1.5
    n-dist
    fixed normal distance
      $dist=.25/($factor); 
      $dist
    lines
*
    getGridPoints(13,9); $ny=9;
      $nx $ny 
*
    n-stretch
      1. 8. 0.
    boundary conditions
      0 1 1 0 
    share
      0 3 1 0
* pause
    exit
*************************************
*    --lower valve shaft right--
*************************************
  SmoothedPolygon
    mappingName
      lower-valve-shaft-right
    vertices
    2
       .1 .3
       .1 .7
    n-dist
    fixed normal distance
      $dist=-.25/($factor); 
      $dist
    lines
*
    getGridPoints(9,9); $ny=9;
      $nx $ny 
*
    n-stretch
      1. 8. 0.
    boundary conditions
      0 0 1 0 
    share
      0 0 1 0 
* pause
    exit
  *
*************************************
*    --upper valve shaft right--
*************************************
  SmoothedPolygon
    mappingName
      upper-valve-shaft-right
    vertices
    2
       .1 .7 
       .1 1.5 
    n-dist
    fixed normal distance
      $dist=-.25/($factor); 
      $dist
    lines
*
    getGridPoints(13,9); $ny=9;
      $nx $ny 
*
    n-stretch
      1. 8. 0.
    boundary conditions
      0 1 1 0 
    share
      0 3 1 0
* pause
    exit
*********************************************
*
*********************************************
  SmoothedPolygon
    mappingName
      valve-seat-left
    vertices
      4
      -.7 1.5
      -.7 .8
      -1.1 .5
      -2.0 .5
    n-dist
    fixed normal distance
     *  .375
     $dist=.25/($factor); 
     $dist
    t-stretch
      0. 50
      .15 30.
      .15 30.
      0. 50.
    n-stretch
      1.  8. 0.
    lines
*
    getGridPoints(45,9); $ny=9;
      $nx $ny 
*
    boundary conditions
      1 1 1 0
    share
      3 5 4 0
    exit
*********************************
*
*********************************
  SmoothedPolygon
    mappingName
      valve-seat-right
    vertices
      4
       .7 1.5
       .7 .8
       1.1 .5
       2.0 .5
    n-dist
    fixed normal distance
      $dist=-.25/($factor); 
      $dist
    t-stretch
      0. 50
      .15 30.
      .15 30.
      0. 50.
    n-stretch
      1.  8. 0.
    lines
*
    getGridPoints(45,9); $ny=9;
      $nx $ny 
*
    boundary conditions
      1 1 1 0
    share
      3 6 4 0
    exit
*
*  ***** core of the valve stem ****
*
  rectangle
    mappingName
      left-valve-stem-core
    set corners
      $ya = .5 - 1./15/$factor;  $xb=-.1 - 2./15/$factor;
      -1.1 $xb $ya 1.5 
    lines
*
    getGridPoints(17,17);
      $nx $ny 
*
    boundary conditions
      0 0 0 1 
    share
      0 0 0 3 
    exit
*
*  ***** core of the valve stem ****
*
  rectangle
    mappingName
      right-valve-stem-core
    set corners
      $ya = .5 - 1./15/$factor;   $xa=.1 + 2./15/$factor;
      $xa 1.1 $ya 1.5 
    lines
*
    getGridPoints(17,17);
      $nx $ny 
*
    boundary conditions
      0 0 0 1 
    share
      0 0 0 3 
    exit
**
*  ** main background grid represents the cylinder ****
  rectangle
    mappingName
      cylinder
    set corners
      -2. 2. -2. .5
    lines
*
    getGridPoints(61,37);
      $nx $ny 
*
    boundary conditions
      1 1 1 0
    share
      5 6 0 0
    exit
*
*
  exit this menu
*
  generate an overlapping grid
    cylinder
    left-valve-stem-core
    right-valve-stem-core
    lower-valve-shaft-left
    upper-valve-shaft-left
    lower-valve-shaft-right
    upper-valve-shaft-right
    valve-seat-left
    valve-seat-right
    valve-head
    done
    *change parameters
     * prevent hole cutting
     *   cylinder
     *     all
     *   done
    *exit
    change parameters
      ghost points
        all
        2 2 2 2 2 2
    exit
*   display intermediate results
    compute overlap
*   pause
  exit
*
save an overlapping grid
$name
oneValve
exit

