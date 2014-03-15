  *
  read iges file
  cat2.igs
  continue
    *
    * back port
    *
    choose a list
      83  84  85  86  87  88  89  90  91  92 
      93  94  95  96  97  98  99 100 103 130
     131 132 133 134 135 136 137 143 144
    *
    * cylinder : 74 and 75 are the little slices that go in
    *            the overhang of the port on the cylinder
      74 202 203 204 205 238 239 240
    *
    * front port
    *
      68  72  73  75  76  77  78  79  80  81 
      82 122 123 124 125 126 127 128 129
     done
      mappingName
        portCylinder
      pause
      determine topology
    exit


    save CompositeSurface
      portCylinder.hdf
    exit
  exit this menu
