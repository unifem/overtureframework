*
* ogen command file to create a plot3d file for a Mapping
*
  create mappings
*
  cylinder
    bounds on theta
      0. .75
    mappingName
      partialCylinder
  exit
*
  save plot3d file
  partialCylinder
  cylinder.plot3d
    save file
    exit
  exit this menu
