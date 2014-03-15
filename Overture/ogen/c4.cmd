*
* circle in a channel
*
create mappings
*
rectangle
specify corners
-2. -2. 2. 2.
lines
51 51
boundary conditions
1 1 1 1
periodicity
0 1
mappingName
square
exit
*
Annulus
lines
51 11 
boundary conditions
-1 -1 1 0
exit
*
exit
*
make an overlapping grid
square
Annulus
Done choosing Mappings
Specify new MappedGrid Parameters
discretizationWidth
5 5
5 5
Done specifying MappedGrid parameters
Specify new CompositeGrid parameters
interpolationWidth
5 5
repeat
interpolationIsImplicit
Implicit
Repeat
Done specifying CompositeGrid parameters
Plot the CompositeGrid

