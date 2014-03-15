*
* two circles in a channel
*
create mappings
*
rectangle
  set corners
    -2. 2. -2. 2.
  lines
    2401 2401 1201 1201   601 601 301 301 601 601 481 481 121 121 481 481 121 121 61 61  241 241 121 121  61 61  32 32 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  centre for annulus
    .25 .65
  outer radius
    .65
  lines
    2561 97  1281 49 641 25  161 7 641  33  161 9  81 5     
  boundary conditions
    -1 -1 1 0
  mappingName
    annulus1
exit
Annulus
  centre for annulus
    .75 -.65
  outer radius
    .65
  lines
    2561 97  1281 49 641 25 321 13 641 25  161 7 641  33  161 9  81 5     
  boundary conditions
    -1 -1 1 0
  mappingName
    annulus2
exit
*
exit
generate an overlapping grid
    square
    annulus1
    annulus2
  done
  change parameters
    * choose implicit or explicit interpolation
    interpolation type
      explicit for all grids
    ghost points
      all
      2 2 2 3
  exit
  compute overlap
* pause
  exit
*
save an overlapping grid
twoCircles.hdf
cic4
exit

