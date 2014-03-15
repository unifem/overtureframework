*
* Test the boundary mismatch problem
*
create mappings
*
  Annulus
    mappingName
      mainAnnulus
    lines
      21 7
    share
      0 0 1 2
    exit
*
  Annulus
    mappingName
      refinement
    start and end angles
      0. .25
    lines
      30 15
    boundary conditions
      0 0 1 2
    share
      0 0 1 2
    exit
* build a discrete version of the mapping
  DataPointMapping
    mappingName
      mainAnnulusDP
    build from a mapping
    mainAnnulus
    exit
  DataPointMapping
    mappingName
      refinementDP
    build from a mapping
    refinement
    exit
  exit this menu
*
  generate an overlapping grid
    mainAnnulusDP
    refinementDP
*    mainAnnulus
*    refinement
    done
*    debug 
*       7
*     display intermediate results
    compute overlap
* 
*
  exit
*
exit
