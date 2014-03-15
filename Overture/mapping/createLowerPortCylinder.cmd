  * lower ports plus cylinder
  *
  read iges file
  cat2.igs
    * 74 and 75 are the little slices that go in
    * the overhang of the port on the cylinder
    choose a list
      73 74 75 79 87 92 93 94 95 96 
      97 98 99 202 203 204 205 
      done
    create CompositeSurface
      add all
      mappingName
        lowerPortCylinder
      determine topology
      flip normals
    exit
    save CompositeSurface
      lowerPortCylinder.hdf
    exit
  exit this menu

