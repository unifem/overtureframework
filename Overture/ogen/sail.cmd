*
* Create a 3D Sail for Cheryl
*   First run 'ogen noplot sailVolumes'
*         and 'ogen noplot sailPerimeter'
*
create mappings
*
open a data-base
   sailVolumes.hdf
   open an old file read-only
   get all mappings from the data-base
*
open a data-base
   sailPerimeter.hdf
   open an old file read-only
   get all mappings from the data-base
  *
*
*
*  box : [-100,400]x[-50,100],[-50,1050]
*
  Box
   mappingName
    backGround
    * aspect ratios 
    set corners
      -100 400  -50 100  -50 1050 
    lines
      91 31 201 <-worked   101 31 221 <-worked   101 61 221  <-worked  50 15 110
   boundary conditions
     1 1 1 1 1 1
   exit
*
  exit this menu
*
  generate an overlapping grid
    backGround
    topWindWardVolume
    bottomWindWardVolume
    topLeeWardVolume
    bottomLeeWardVolume
    perimeterVolume
    capVolume
    change the plot
      toggle grids on and off
      0 : backGround is (on)
      exit this menu
      x-r 90
      exit this menu
    change parameters
      prevent hole cutting
        all
        all
        done
      allow hole cutting
        perimeterVolume
          backGround      
        perimeterVolume
          bottomWindWardVolume
        perimeterVolume
          bottomLeeWardVolume
        capVolume
          backGround
        capVolume
          topWindWardVolume
        capVolume
          topLeeWardVolume
        topWindWardVolume
          backGround
        bottomWindWardVolume
          backGround
        topLeeWardVolume
          backGround
        bottomLeeWardVolume
          backGround
        done
      exit
    * display intermediate
    * pause
    compute overlap
    * pause
  exit
*
save an overlapping grid
sail.hdf
sail
exit


    display intermediate results






  generate an overlapping grid
     bottomWindWardVolume
     bottomLeeWardVolume
     perimeterVolume
     topLeeWardVolume
     topWindWardVolume
     capVolume
   done
   x-r 90
   display intermediate results
  set debug parameter
     1
junk

   compute overlap
pause   


  reparameterize
    transform which mapping?
    perimeterVolume
    restrict parameter space
      specify corners
        0. .5 0. 1.
      exit
    mappingName
      topPerimeterVolume
    exit
view mappings
pause
*
  exit this menu
  generate an overlapping grid
    topPerimeterVolume
    bottomLeeWardVolume
    done choosing mappings
*    set debug parameter
*      7
*    compute overlap

  exit this menu
*
  generate an overlapping grid
    bottomWindWardVolume
    perimeterVolume
   done
   x-r 90


    topWindWardVolume
    topLeeWardVolume
    capVolume
   done

  generate an overlapping grid
    backGround
    topWindWardVolume
    bottomWindWardVolume
    topLeeWardVolume
    bottomLeeWardVolume
    perimeterVolume
    capVolume
    change the plot
      toggle grids on and off
      0 : backGround is (on)
      exit this menu
      x-r 90
      exit this menu
    change parameters
      prevent hole cutting
        all
        all
        done
      allow hole cutting
        perimeterVolume
        all
        capVolume
        all
        topWindWardVolume
        backGround
        bottomWindWardVolume
        backGround
        topLeeWardVolume
        backGround
        bottomLeeWardVolume
        backGround
        done
      exit
    display intermediate results




*
  reparameterize
    transform which mapping?
    perimeterVolume
    restrict parameter space
      set corners
        .0  1.0 0. 1.  0. 1. .525 .65 0. .5  0. 1.
      exit
    mappingName
      perimeterVolumeNew
*pause
    exit
*
  reparameterize
    transform which mapping?
    bottomWindWardVolume
    restrict parameter space
      set corners
        0. 1.  0. 1.  0. 1.   0. .15  0. .25 0. 1.
      exit
    mappingName
      bottomWindWardVolumeNew
*pause
    exit
