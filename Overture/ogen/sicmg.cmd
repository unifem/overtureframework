* 
* Create a "square in a circle" with MG levels
*
create mappings
* first make an annulus
  Annulus
    inner radius
      .4
    outer radius
      1.
    lines
      41 13 
    mappingName
      annulus
    boundary conditions
      -1 -1 0 1
    exit
*  make a rectangle to fill in the inside of the annulus
  rectangle
    specify corners
      -.7 -.7 .7 .7
    lines
      21 21 
    boundary conditions
      0 0 0 0
    mappingName
      inner-square
    exit
  exit
*
generate an overlapping grid
  specify number of multigrid levels
    3
  inner-square
  annulus
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  * pause
  compute overlap
exit
save an overlapping grid
sicmg.hdf
sicmg
exit

