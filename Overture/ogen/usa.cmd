*
* read a plot3d file for the usa
* The file usa.plot3d was created with Hypgen
*
create mappings
  DataPointMapping
    mappingName
      usa
  * file format is a plot3d file (single grid)  
  plot3d (single grid)
  read file
  usa.plot3d
  exit
exit
make an overlapping grid
  usa
  Done
  Plot
  pause
Done
*
save an overlapping grid
  usa.hdf
  usa
exit

