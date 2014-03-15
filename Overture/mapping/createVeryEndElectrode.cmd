  read iges file
  electrode3.igs
    choose some
      1 5
    create CompositeSurface
      add all mappings
      x+r
      x+r
      x+r
      delete sub-surfaces with mouse
      mogl-select 1 
            126 864121792 880716096  
      mogl-pick
      mogl-select 1 
            170 872546688 873963648  
      mogl-pick
      done
      determine topology
      mappingName
        veryEndElectrode
      exit
    save CompositeSurface
      veryEndElectrode.hdf
    exit
  exit this menu

