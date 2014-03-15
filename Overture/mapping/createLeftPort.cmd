  * 
  read iges file 
    cat2.igs 
    continue 
    * 
    * back port plus connector pieces 
    * 
    choose a list 
      83  84  85  86  87  88  89  90  91  92 
      93  94  95  96  97  98  99 100 103 130 
      131 132 133 134 135 136 137 143 144 
      done 
    mappingName
      leftPort
    pause
    determine topology
   exit
*
  open a data-base
    leftPort.hdf
  open a new file
  put to the data-base
  leftPort
  close the data-base
  exit this menu


pause

  *
  read iges file
  cat2.igs
  continue
    *
    * back port plus connector pieces
    *
    choose a list
      83  84  85  86  87  88  89  90  91  92 
      93  94  95  96  97  98  99 100 103 130
     131 132 133 134 135 136 137 143 144
     done
pause
    create CompositeSurface
      add all
      mappingName
        leftPort
      determine topology
      * pause
    exit
    save CompositeSurface
      leftPort.hdf
    exit
  exit this menu
