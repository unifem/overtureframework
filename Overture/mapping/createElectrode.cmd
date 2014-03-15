* I think these are surfaces 0-45
  read iges file
  electrode3.igs
  continue



    choose some
      0 45 
    done
    create CompositeSurface
      add all mappings
      * pause
      determine topology
pause
      mappingName
        electrode
      exit
    save CompositeSurface
      electrode.hdf
    exit
  exit this menu
