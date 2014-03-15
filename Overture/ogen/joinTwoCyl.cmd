*
* Create a grid exterior to two intersecting pipes
* Use the JoinMapping to reparameterize the top cylinder
* so that exactly matches the main cylinder.
*
create mappings
*
* Here is the box
*
  Box
    set corners
      -1. 1. -1. 1. -1. 1.
    lines
      32 32 32   
    mappingName
      box
    share
      1 1 0 2 0 0
    exit
*
  Cylinder
    mappingName
      main-cylinder
    orientation
      1 2 0
    bounds on the axial variable
      -1. 1.
    bounds on the radial variable
      .5 .75
    boundary conditions
      -1 -1 1 2 3 0
    lines
      39 25 6   31 21 6
    share
      0 0 1 1 3 0
    exit
*
  Cylinder
    mappingName
      top-cylinder
    orientation
      2 0 1
    bounds on the axial variable
      .25 1.
    bounds on the radial variable
      .25 .425
    boundary conditions
      -1 -1 0 2 3 0
    lines
      25 15 5
    share
      0 0 0 2 3 0
    exit
*
  join
   mappingName
     cylinderJoin
   choose curves
     top-cylinder
     main-cylinder (side=0,axis=2)
   * pause
   compute join
   lines
    25 11 6   31 15 7
   boundary conditions
     -1 -1 1 1 1 0
   share
      0 0  3 2 0 0
  exit
*
   DataPointMapping
     build from a mapping
       cylinderJoin
     mappingName
       cylinderJoindp
     * pause
  exit
exit
generate an overlapping grid
  box
  * top-cylinder
  main-cylinder
  cylinderJoindp
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
   change the plot
     toggle grid 0 0
     x+r
     y+r
     y+r
     x+r
* pause
   exit this menu
  compute overlap
  exit
*
save an overlapping grid
joinTwoCyl.hdf
joinTwoCyl
exit
