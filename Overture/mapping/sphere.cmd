*
* Make a sphere
*
Sphere
  mappingName
    sphere
    inner and outer radii
      1. 1.5
    bounds on phi (latitude)
    .0001 .9999
    surface or volume (toggle)
    lines
      101 101 
pause
  exit
  unstructured
    build from a mapping
    sphere
   exit
  get geometric properties
  unstructuredMapping