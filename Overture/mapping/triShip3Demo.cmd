*
* convert a cylinder into an unstructured mapping
*
  read iges file
    ship3.igs
    continue
   choose a list
    15 16 17 18 19 20 21 22 23 102 109 110
   done
   x+r:0 120
   y+r:0 20
   plot grid lines on boundaries (3D) toggle 0
   pause
   mappingName
     propHull
   exit
*
  unstructured
    element density tolerance
      .025 .01  .025  .05   .025
    stitching tolerance
      .01 .025  .01
    absolute stitching tolerance
      .05 .1
    stitch debug
      0
    build from a mapping
    propHull
    mappingName
     triShip3
    pause
  exit
  builder
    create surface grid...
      mogl-select:0 2 
            175 1173687040 1173859328  148 1173701248 1173935488  
      mogl-coordinates 1.761194e-01 5.592287e-01 1.173687e+09 6.026624e+00 7.712531e+00 3.805740e+00
pause
      points on initial curve 21
      lines to march 27
      generate
      step
      step
      exit
pause
    create surface grid...
      mogl-select:0 2 
            189 1178696832 1178849920  148 1178691200 1178908544  
      mogl-coordinates 8.567164e-01 5.730028e-01 1.178691e+09 1.449567e+01 8.686126e+00 4.458348e+00
pause
      distance to march 5.
      lines to march 27
      generate
      x+r:0
pause
      exit
    create surface grid...
      mogl-select:0 2 
            198 1188232704 1188608640  148 1188120192 1188608640  
      mogl-coordinates 3.850746e-01 3.443526e-01 1.188120e+09 8.942968e+00 6.487210e+00 7.260759e+00
pause
      surface grid options...
      edit initial curve
        restrict the domain
        .5 1.
        exit
      marching options...
      BC: left  free floating
      points on initial curve 11
pause
      generate
      exit
pause
* volume grids
    active grid:triShip3-surface1
    create volume grid...
      distance to march 2
      marching options...
      uniform dissipation 0.2
      generate
      exit
pause
    active grid:triShip3-surface2
    create volume grid...
      distance to march 2
      generate
      exit
    active grid:triShip3-surface3
    create volume grid...
      generate
      exit


