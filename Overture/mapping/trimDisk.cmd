  plane or rhombus
    lines
      11 11
    mappingName
      plane1
    exit
  Circle or ellipse
    specify centre
      .5 .5
    specify radius of the circle
      .45
    exit
  trimmed mapping
    specify mappings
    plane
    circle
    done
    mappingName
      disk
    exit
  plane or rhombus
    specify plane or rhombus by three points
      .5 .5 -.5  1. .5 -.5  .5 .5 .5
    mappingName
      plane2
    exit
  debug
    7
  intersection
    choose mappings to intersect
    disk
    plane2

    intersect

