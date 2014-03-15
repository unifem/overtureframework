* 
* command file to create a hybrid grid for a sphere in a box 
* 
create mappings 
  * first make a sphere 
  Sphere 
    exit 
  * 
  * now make a mapping for the north pole 
  * 
  reparameterize 
    orthographic 
      specify sa,sb 
      2.5 2.5 
      exit 
    lines 
    15 15 5 
    boundary conditions 
    0 0 0 0 1 0 
    share 
    0 0 0 0 1 0 
    mappingName 
    north-pole 
    exit 
  * 
  * now make a mapping for the south pole 
  * 
  reparameterize 
    orthographic 
      choose north or south pole 
      -1 
      specify sa,sb 
      2.5 2.5 
      exit 
    lines 
    7 7 5 
    boundary conditions 
    0 0 0 0 1 0 
    share 
    0 0 0 0 1 0 
    mappingName 
    south-pole 
    exit 
  * 
  * Here is the box 
  * 
  Box 
    set corners 
    -2 2 -2 2 -2 2 
    lines 
    11 11 11 
    mappingName 
    box 
    exit 
  exit 
* 
generate a hybrid mesh 
  box 
  north-pole 
  south-pole 
  done 
  compute overlap 
  exit
  continue generation
  exit
  continue generation
*  pause
  exit
  save an overlapping grid
    sib.hyb.hdf
    sib
exit
