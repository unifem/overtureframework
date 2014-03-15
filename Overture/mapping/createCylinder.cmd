  *
  read iges file
  cat2.igs
    * cylinder : 74 and 75 are the little slices that go in
    * the overhang of the port on the cylinder
    choose a list
      74 75 202 203 204 205 238 239 240 75
    done
    create CompositeSurface
      add all
      mappingName
        cylinder
      * pause
      determine topology
    exit
    save CompositeSurface
      catCylinder.hdf
    exit
  exit this menu
