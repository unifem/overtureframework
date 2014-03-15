*
*  Build a composite surface for the surface of a cylinder
*
*
  plane or rhombus
    specify plane or rhombus by three points
    -.75 -.75 0.  .75 -.75 0.  -.75 .75 0.
    mappingName
     plane1
    exit
*
*  -- build a trim curve ---
*
  circle or ellipse
    specify radius of the circle
     .333333333333333333333
    specify centre
     .5 .5
    lines
      101
    mappingName
     circleAnalytic
    exit
*  -- trim curve must be a nurbs so convert to a nurbs
  nurbs (curve)
    interpolate from a mapping
      circleAnalytic
    mappingName
     circle
    exit
*
* Make a trimmed mapping for a disk
*
  trimmed mapping
  specify mappings
    plane1
    circle
  done
  mappingName
    bottom
  exit
*
  plane or rhombus
    specify plane or rhombus by three points
    -.75 -.75 1.  .75 -.75 1.  -.75 .75 1.
    mappingName
     plane2
    exit
*
* Make a trimmed mapping for a disk
*
  trimmed mapping
  specify mappings
    plane2
    circle
  done
  mappingName
    top  
  exit
*
*  -- the topology routine does not like periodic surfaces so make 2 half cylinders
*
  Cylinder 
    surface or volume (toggle) 
    bounds on the radial variable
      .5 1.
    bounds on the axial variable
      0. 1.
    bounds on theta
      0. .5 
    mappingName
      cylinder1
    exit
*
  Cylinder 
    surface or volume (toggle) 
    bounds on the radial variable
      .5 1.
    bounds on the axial variable
      0. 1.
    bounds on theta
      .5 1. 
    mappingName
      cylinder2
    exit
*
  composite surface
    CSUP:add a mapping bottom
    CSUP:add a mapping top
    CSUP:add a mapping cylinder1
    CSUP:add a mapping cylinder2
    CSUP:determine topology
    deltaS 0.05
    maximum area .0025
    compute topology
    exit
    mappingName
     compositeSurface
  exit
*
  open a data-base
  cylinderCS.hdf
  open a new file
  put to the data-base
  compositeSurface