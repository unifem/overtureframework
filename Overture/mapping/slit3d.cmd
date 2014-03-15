  SmoothedPolygon
    vertices
      5
      -.01 .5
       .01 .5
       .01 -.5
      -.01 -.5
      -.01 .5
    n-dist
    fixed normal distance
      .1
    lines
      16 7
    mappingName
      slit
    use robust inverse
    check inverse
      enter a point
        .02 .1
pause
      enter a point
        .01 .1
    done
    exit
*
* now build a 3d version
*
  line (3D)
    set end points
      0 0 0 0 0 1
    exit
*
  sweep
    use center of sweep curve
    choose sweep surface/curve
    slit
    line
    check inverse
      use robust inverse
      enter a point
        .02 .1 .5
      enter a point
        .01 .1 .5
