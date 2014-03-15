*
* filament mapping in a channel
*
create mappings
*
rectangle
  set corners
    -2. 1. -.5 .5 
  lines
    151 51 
  boundary conditions
    1 1 1 1
  mappingName
    background
exit
*
filamentMapping
    Hyperbolic grid generator
      points on initial curve 101
      distance to march 0.1 
      lines to march 11
      spacing: geometric
      geometric stretch factor 1.1 
      generate
      exit
  mappingName
    filament
exit
*
exit
generate an overlapping grid
    background
    filament
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
*  display intermediate results
  compute overlap
  exit
*
save an overlapping grid
filament.hdf
filament
exit
