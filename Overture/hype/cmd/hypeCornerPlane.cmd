* Make a composite surface with a corner to test
* surface grid generator
  plane or rhombus
    mappingName
      plane1
    lines
      11 11
    exit
  plane or rhombus
    specify plane or rhombus by three points
     * 1 -.125 0   1 1.125 0   1.0 -.125 -1.    * 90 degree down turn
     * 1 -.125 0   1 1.125 0   1.0 -.125 +1.    * 90 degree up turn
       1 -.125 0   1 1.125 0    .25  -.125 -1.    * very sharp turn
    lines
      11 16
    mappingName
      plane2
    exit
*
  composite surface
    add a mapping plane1
    add a mapping plane2
    determine topology
*
      deltaS .1
      build edge curves
      merge edge curves
      triangulate
      exit
    exit
  builder
    set view:0 0 0 0 1 0.991658 -0.122583 0.0398514 0.123693 0.81802 -0.561733 0.0362596 0.561976 0.826358
    create surface grid...
      surface grid options...
      initial curve:coordinate line 0
      mogl-select:0 1 
            87 1169718016 1170171264  
      mogl-coordinates 5.072886e-01 5.072886e-01 1.169718e+09 4.924853e-01 5.052780e-01 6.929314e-03
      set view:0 0 0 0 1 0.946497 -0.193354 0.258374 0.293815 0.847465 -0.442128 -0.133476 0.494387 0.858933
      backward
      distance to march .25
      lines to march 7
      points on initial curve 11
      generate





    plot normals (toggle)
    flip normals (toggle)
    mappingName
      cornerPlane
    exit
  hyperbolic
    mappingName
      cornerPlane
    choose the initial curve
    create a curve from the surface
    coordinate line
      0
      axis1=.5
    distance to march
      1.
    lines to march
      12
    reset
    x-r 90
    y-r 45
    x+r 45
    generate


