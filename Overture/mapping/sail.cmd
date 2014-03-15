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
* view mappings
* choose all
* pause
*
*
*  box : [-100,400]x[-50,100],[-50,1050]
*
  Box
   mappingName
    backGround
    * aspect ratios 
    specify corners
      -100 -50 -50 400 100 1050 
    lines
      101 31 221   50 15 110
   boundary conditions
     1 1 1 1 1 1
   exit
