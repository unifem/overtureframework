*
* Make a grid for a ship propeller
*
create mappings
  * read grids from the chalmesh overlapping grid:
  read overlapping grid file
  ../xCogToOverture/p4119
*
* The edge mapping needs to use the robust inverse
*
  change a mapping
  edge1
    use robust inverse
    * **********************************
    *    boundary condition
    *      0 0 0 0 4 0
    share
      0 0 1 1 4 0
*    check inverse
*     enter a point
*      -2.731380e-01,1.769806e-01,-1.359098e-01 
*    pause
    exit
*
  reparameterize
    transform which mapping?
      edge1
    restrict parameter space
      set corners
        0. 1. .85 1. 0. 1.
      exit
    mappingName
      edge1r
*    check inverse
* pause
    exit
*
  change a mapping
  upper1
    share
      0 0 1 0 4 0
    exit
*
*
  reparameterize
    transform which mapping?
      upper1
    restrict parameter space
      set corners
        .75 1. 0. .25 0. 1.
      exit
    mappingName
      upper1r
    exit
exit
*
generate an overlapping grid
*  lower1
  upper1r
  edge1r
  done
*
  display intermediate
  debug
   7
  compute overlap
    change the plot
      toggle grids on and off
      1 : edge1r is (on)
      exit this menu
      set view 0.151429 0.337143 0 3.80435 1 0 0 0 1 0 0 0 1
pause



generate an overlapping grid
  cylinder2
  lower1
  upper1
  edge1
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
  exit
  display intermediate
  compute overlap
  continue
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



