*
create mappings
*
  read iges file 
*    /home/henshaw/Overture/mapping/LANLAOT5.igs 
    /home/henshaw/iges/LANLAOT5.igs 
    continue 
    choose some 
    0 -1 
    CSUP:determine topology 
    deltaS 2 
    maximum area 4
    build edge curves 
    merge edge curves 
    triangulate 
* pause
    exit 
    mappingName
      compositeSurface
    exit 
  open a data-base
  slac1.hdf
  open a new file
  put to the data-base
    compositeSurface
  close the data-base


  exit this menu
