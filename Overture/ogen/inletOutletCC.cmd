*
* create a grid to demonstrate various features
*
create mappings
  * make a back ground grid
  rectangle
    set corners
      0 2.  0 1.
    lines
      61 31  
    mappingName
     backGroundGrid
    share
      1 2 3 4
  exit
  * make an annulus
  Annulus
    centre for annulus
      1. .5
    inner radius
     .2
    outer radius
     .4
    lines
      41 9
    mappingName
      annulus
    boundary conditions
      -1 -1 1 0
  exit
  * the inlet (on the right) will consist of two 
  * smoothed polygons
  SmoothedPolygon
    mappingName
      inlet-top
    vertices
     3
     2. .85
     2. .625 
     2.25 .625 
   n-dist
     fixed normal distance
     -.175  .2
   sharpness
     10.
     10.
     10.
   t-stretch
     0. 10.
     1. 10.
     0. 10.
   lines
     25 13 
   boundary conditions
     0 1 1 0
   * One boundary here should match one boundary of 
   * the backGroundGrid, while another boundary 
   * should match a boundary on the inlet-bottom.
   * Set share flag to match corresponding share values
   share
     0 5 2 0
   exit
* 
  SmoothedPolygon
    mappingName
      inlet-bottom
    vertices
     3
     2. .15 
     2. .375 
     2.25 .375 
   lines
     25 13 
   n-dist
     fixed normal distance
      .175  .2
   sharpness
     10.
     10.
     10.
   t-stretch
     0. 10.
     1. 10.
     0. 10.
   boundary conditions
     0 1 1 0
   * One boundary here should match one boundary 
   * of the backGroundGrid, while another boundary 
   * should match a bounbdary on the inlet-bottom.
   * Set share flag to match corresponding share values
   share
     0 5 2 0
   exit
  * here is an outlet grid made in the poor man's way
  rectangle
    set corners
     * trouble:  -.35 .05  .3  .7
      -.35 .1  .3 .7
    lines
      15 15
    mappingName
     outlet
    boundary conditions
      1 0 1 1
  exit
  * now look at the mappings
  exit
generate an overlapping grid
* put the nonconforming grid first to be a lower 
* priority than the back-ground
    outlet
    backGroundGrid
    annulus
    inlet-top
    inlet-bottom
  done
  change parameters
    prevent hole cutting
      backGroundGrid
        all
      outlet
        all
      done
    cell centering
      cell centered for all grids
    exit
    *set debug parameter
    * 7
    compute overlap
  exit
*
save an overlapping grid
  inletOutletCC.hdf
  inletOutlet
exit

