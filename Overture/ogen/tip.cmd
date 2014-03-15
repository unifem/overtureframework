create mappings
  SmoothedPolygon
    vertices
      4
      -.004 0.
      -.004 1
       .004 1.
       .004 .1
    n-dist
    fixed normal distance
      .1
    lines
      52 31
    n-stretch
      5. 120. 0.
    share
      0 0 1 2
    mappingName
     tipper
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
     tipper
    line
pause
  exit
*
  reparameterize
    restrict parameter space
      set corners
        .25 .7 0. 1. 0. 1.
      exit
    mappingName
      tip
    exit
*
  reparameterize
    restrict parameter space
      set corners
        .6 1. 0. 1. 0. 1.
      exit
      lines
        31 26
    mappingName
      right
    exit
*
    DataPointMapping
      build from a mapping
      tip
      use robust inverse
      mappingName
        tip-dp
      exit
*
    DataPointMapping
      build from a mapping
      right
      mappingName
        right-dp
      exit
  exit this menu
*
  generate an overlapping grid
    right-dp
    tip-dp
    done choosing mappings
    display intermediate results
    compute overlap

