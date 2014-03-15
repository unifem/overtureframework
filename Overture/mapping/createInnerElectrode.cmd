* I think these are surfaces 0-45
  read iges file
  electrode3.igs
    choose a list
      21 23 24 25 
    done
    create CompositeSurface
      add all mappings
      * pause
      determine topology
pause
      mappingName
        innerElectrode
      exit
    save CompositeSurface
      innerElectrode.hdf
    exit
  exit this menu
