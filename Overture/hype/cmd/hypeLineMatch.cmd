  line (2D)
    specify end points
*      1 0 2 1
      1 0 .5 1
    mappingName
      boundary
    exit
  line (2D)
    lines
      21
    exit
*
  hyperbolic 
    BC: right match to a mapping
    boundary
    lines to march 3 
    distance to march .1
    backward
    generate
