* A channel for a detonation computation
create mappings
  rectangle
    set corners
      -1. .5 0. 1.   -2. 1. 0. 1. 
    lines
      301 201  601 201   151 55  71 11   
    boundary conditions
      1 1 1 1
    mappingName
      channel
    exit
exit
* 
generate an overlapping grid
  channel
  done
  change parameters
    ghost points
      all
      2 2 2 2
    exit
  compute overlap
  exit
save a grid
detChannel.hdf
detChannel
exit
