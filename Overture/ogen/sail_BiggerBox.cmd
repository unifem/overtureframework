*
*  Build a 3d sail
*    First run sailVolumes.cmd and sailPerimeter.cmd
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
     0 0 0 0 0 0
   exit
  Box
   mappingName
    BigBackground
    * aspect ratios 
    set corners
      -500 800  -400 500 -50 1450
    lines
 	64 32 128
   boundary conditions
     1 1 1 1 1 1
   exit
*
  exit this menu
*
  generate an overlapping grid
    BigBackground
    backGround
    topWindWardVolume
    bottomWindWardVolume
    topLeeWardVolume
    bottomLeeWardVolume
    perimeterVolume
    capVolume
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
        backGround
          BigBackground
        done
      exit
   pause
    compute overlap
    * pause
  exit
*
save an overlapping grid
sail.hdf
sail
exit

