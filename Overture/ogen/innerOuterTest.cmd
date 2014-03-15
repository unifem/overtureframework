*
*  Builds grids for regions exterior and interior to a boundary
*
create mappings 
*
  rectangle 
    mappingName
      innerSquare
    set corners 
*      -.3 .3 -.3 .3  
      -.2 .2 -.2 .2 
    lines 
      11 11 
    boundary conditions
      0 0 0 0
    exit 
*
  annulus 
    mappingName 
      innerAnnulus 
    boundary conditions 
    -1 -1 0 1 
    share
    0 0 0 0
    inner and outer radii 
    0.2 0.4
    lines 
    21 5 
    exit 
*
  rectangle 
    mappingName
      outerSquare
    set corners 
    -1 1 -1 1 
    lines 
       31 31 
    exit 
*
  annulus 
    mappingName 
      outerAnnulus 
    inner and outer radii 
    0.4 0.7 
    share
    0 0 0 0 
    lines 
    41 7 
    boundary conditions 
    -1 -1 2 0 
    exit 
  exit this menu 
*
generate an overlapping grid 
  outerSquare
  outerAnnulus 
  innerSquare
  innerAnnulus 
  done 
*
  change parameters 
*   We must prevent hole cutting and interpolation between
*   the inner and outer grids.  
    prevent hole cutting
      all
      all
    done
    allow hole cutting
      innerAnnulus
      innerSquare
      outerAnnulus
      outerSquare
    done
    prevent interpolation
      all
      all
    done
    allow interpolation 
      innerAnnulus
      innerSquare
      innerSquare
      innerAnnulus
      outerAnnulus
      outerSquare
      outerSquare
      outerAnnulus
    done
    ghost points 
      all 
      2 2 2 2 
    exit 
*    display intermediate results
    compute overlap
  exit
save a grid
innerOuterTest.hdf
innerOuter
exit
