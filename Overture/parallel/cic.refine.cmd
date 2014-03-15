* 
choose a grid
  cice
add a refinement
  * grid level ...  rr
  1 1 2 8 1 7  2 
add a refinement
  0 1 18 25 18 25   2 
*add a refinement
*  0 1 1 4 1 6   2 
debug
  7
do not check validity
update refinements
* 

save the grid
cice.refine.hdf
cice
exit

output check file

exit




choose a grid
  cice
add a refinement
  1 1 0 5 0 5 
add a refinement
  1 1 28 32 1 6
* 
update refinements
*
* 

test interpolate





* --------- refinements hit the branch cut ----------
choose a grid
  annulus1
 add a refinement
   0 1 0 5 1 4 
 add a refinement
   0 1 26 30 1 4
update refinements
*
* test interpolate refinements
test interpolateRefinementBoundaries



* --------- refinements hit the branch cut ----------
choose a grid
  annulus1
add a refinement
  0 1 0 5 1 4 
add a refinement
  0 1 26 30 1 4
update refinements
*
* test interpolate refinements
test interpolateRefinementBoundaries

* --------- refinements do NOT hit the branch cut ----------
choose a grid
  annulus1
add a refinement
  0 1 10 15 1 4 
add a refinement
  0 1 15 20 1 4
update refinements
*
test interpolate refinements
* 







choose a grid
  cice
add a refinement
  1 1 0 5 0 5 
add a refinement
  1 1 28 32 1 6
* 
update refinements
*
* 
test interpolate refinements


** test interpolate
* 
* is it allowable anymore to have a refinement grid cross the branch cut? 
add a refinement
  * grid level ...  rr
  1 1 2 6 2 7  2 
add a refinement
  0 1 19 25 19 25   2 
debug
  7
update refinements


*
test interpolate
* 
exit


choose a grid
  cic
* is it allowable anymore to have a refinement grid cross the branch cut? 
add a refinement
  1 1 -5 5 0 7
debug
  7
update refinements

solve with oges
erase
contour plot

