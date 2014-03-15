*
* circle in a long channel
*
create mappings
rectangle
specify corners
-2.5 -2.5 7.5 2.5  
lines
101 51 
boundary conditions
1 1 1 1
mappingName
square
exit
Annulus
inner
.5 
outer
1.25
lines
85 17 
boundary conditions
-1 -1 1 0
exit
* stretch the annulus *********
*
* Stretch coordinates
stretch coordinates
transform which mapping?
Annulus
stretch
specify stretching along axis=1
layers
1
1. 9. 0.
exit
exit
mappingName
annulus
exit
*
exit
generate an overlapping grid
  specify number of multigrid levels
    2
  square
  annulus
  done
  change parameters
    interpolation type
      explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
save an overlapping grid
cilc.mg.hdf
cilc.mg
exit