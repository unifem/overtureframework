*
*open a data-base
*  rightPort.hdf
*open an old file read-only
*get from the data-base
*  rightPort
*
  *
  read iges file
  cat2.igs
  continue
    * front port
    *
    choose a list
      68  72  73  75  76  77  78  79  80  81 
      82 122 123 124 125 126 127 128 129
      * here are the joining pieces
*      93 94 95 96 97 98 99
    done
    recompute bounds
    mappingName
      rightPort
      determine topology
    exit
  builder
    x+r:0
pause
    create surface grid...
      mogl-select:0 2 
            208 1162536704 1162552320  213 1162532096 1162547840  
      mogl-coordinates 5.223880e-01 2.369146e-01 1.162532e+09 6.921650e+01 8.522908e+00 1.620601e+02
pause
      forward and backward
      distance to march 5 5 (forward,backward) 
      lines to march 12, 12  11, 11
      generate
pause
      step
      step
      exit
pause
    create volume grid...
      uniform dissipation 0.3
      distance to march 4.
      lines to march 11
      generate
      x+r:0 30
      bigger:0
