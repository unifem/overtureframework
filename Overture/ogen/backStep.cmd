*
$ds=.1; # grid spacing 
*
create mappings
*
  rectangle
    set corners
      0. 10. 0 3.
    lines
      101 31
    share
      0 0 0 1
    mappingName
      mainChannel
    exit
  rectangle
    set corners
     $xa=-3.; $xb=0.; # $ds;
     $xa $xb 1. 3.
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $nx 21
    share
      0 0 0 1
    mappingName
      inlet
    exit
  exit this menu
  generate an overlapping grid
    mainChannel
    inlet
    change parameters
      prevent hole cutting
       all
       all
      done
*
      mixed boundary
        mainChannel
          left   (side=0,axis=0)
          inlet
            done
        inlet
          right  (side=1,axis=0)
          mainChannel
            done
        done
*     trouble with explicit interp
*    interpolation type
*      explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
* 
*  display intermediate results
  compute overlap
*  pause
  exit
*
save an overlapping grid
backStep.hdf
backStep
exit

