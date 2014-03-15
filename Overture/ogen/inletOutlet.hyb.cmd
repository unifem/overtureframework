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
     2. .65 
     2.25 .65 
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
     25 11 
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
     2. .35 
     2.25 .35 
   lines
     25 11 
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
      -.35 .05  .3 .7
    lines
      15 15
    mappingName
     outlet
    boundary conditions
      1 0 1 1
  exit
  * now look at the mappings
  view mappings
    backGroundGrid
    annulus
    inlet-top
    inlet-bottom
    outlet
    *
    * The grid is plotted with boundaries coloured
    *  by the boundary condition number. Here we 
    * should check that all interpolation boundaries 
    * are 0 (blue), all physical boundaries are positive 
    * and periodic boundaries are black
    * pause
    *
    * now we plot the boundaries by share value
    * The sides that correspond to the same boundary 
    * should be the same colour
    colour boundaries by share value 
    pause
    erase and exit
  exit
generate a hybrid mesh
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
  exit
*  display intermediate
*  set debug parameter
*    31
  compute overlap
  exit
  set plotting frequency (<1 for never)
  -1
  continue generation
  exit 
  save grid in ingrid format
  inletOutlet.hyb.msh
exit
*

