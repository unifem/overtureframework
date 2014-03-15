*
* Make a grid for a ship propeller
*
create mappings
  * read grids from the chalmesh overlapping grid:
  read overlapping grid file
  /home/henshaw/res/xCogToOverture/p4119
*
  change a mapping
  cylinder
    share
      0 1 0 0 2 3
    exit
*
  Cylinder
    orientation
      1 2 0
    bounds on the axial variable
      -.6 .6
    bounds on the radial variable
      .2 1.4  .4 1.4
    boundary conditions
       -1 -1 3 4 1 2  -1 -1 3 4 0 2
    share
      0 0 2 3 1 0 
    lines
      141 31 41  
    mappingName
     cylinder2
    exit
*
*  change a mapping
*  huban
*    share
*      1 0 0 0 2 3
*    exit
*
*
* The edge mapping needs to use the robust inverse?
*
  change a mapping
  edge1
   use robust inverse
    share
      0 0 1 1 4 0
    exit
*
  change a mapping
  upper1
    share
      0 0 1 0 4 0
    exit
*
  change a mapping
  lower1
    share
      1 0 0 0 4 0
    exit
*
  change a mapping
  edge2
    use robust inverse
    share
      0 0 1 1 5 0
    exit
*
  change a mapping
  upper2
    share
      0 0 1 0 5 0
    exit
*
  change a mapping
  lower2
    share
      1 0 0 0 5 0
    exit
*
  change a mapping
  edge3
    use robust inverse
    share
      0 0 1 1 6 0
    exit
*
  change a mapping
  upper3
    share
      0 0 1 0 6 0
    exit
*
  change a mapping
  lower3
    share
      1 0 0 0 6 0
    exit
exit
*
generate an overlapping grid
  cylinder2
*  lower1
*  upper1
*  edge1
*  lower2
*  upper2
*  edge2
  lower3
  upper3
  edge3
  done
*
  change parameters
   prevent hole cutting
    all
    all
    done
   allow holes to be cut
    cylinder2
     all
   done
   prevent hole cutting
    cylinder2
    all
    done
    maximum distance for hole cutting
      all
        .1 .1 .1 .1 .1 .1
  exit
*  display intermediate
   change the plot
     toggle grid 0 0 
   exit

  compute overlap

  exit
*
save an overlapping grid
prop.hdf
prop
exit





generate an overlapping grid
*  lower1
  upper1
  edge1
  done
*
  display intermediate
*  debug
*   7
  compute overlap
    change the plot
      toggle grids on and off
      1 : edge1 is (on)
      exit this menu
pause






generate an overlapping grid
    cylinder
    * hub:
    huban
    * blade 1: twist1 is a helper grid to put more resolution near the blade
    twist1
    lower1
    upper1
    edge1
    * blade 2:
    *    twist2
    *    lower2
    *    upper2
    *    edge2
  done
*
  change parameters
   prevent hole cutting
    all
    all
    done
   allow holes to be cut
    cylinder
     all
    huban
     all
    twist1
     all
    done
  exit
*   debug
*    7
  display intermediate
  compute overlap
  continue
pause






generate an overlapping grid
    cylinder
    lower1
    upper1
*    edge1
  done
*
  change parameters
*   we have to prevent one side of the blade from cutting holes
*   in the other side 
   prevent hole cutting
    all
    all
    done
   allow holes to be cut
    cylinder
     all
    done
  exit
  debug
   7
  compute overlap
  continue
pause



