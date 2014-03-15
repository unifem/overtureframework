create mappings
Sphere
centre for sphere
1e6 0 0
inner radius
4e5
outer radius
1e6
bounds on phi (latitude)
.5
lines
30 30 15
boundary conditions
3 3 -1 -1 0 2
x+r
x+r
x-r
x-r
y+r
y+r
y+r
y+r
y+r
mappingName
sphere1
exit
Box
reset
specify corners
5e5 -5e5 -5e5 1.5e6 5e5 0
lines
15 15 15
boundary conditions
0 0 0 0 0 3
boundary conditions
0 0 0 0 0 5
exit
pause
exit this menu
make an overlapping grid
box
sphere1
Done choosing Mappings
  Specify new CompositeGrid parameters
    interpolationIsImplicit
    * Explicit
    Implicit
    Repeat
  Done
  pause
Plot the CompositeGrid
Done specifying the CompositeGrid
# Bill, here, I get a warning message that one of the component
# grids does not have any points remaining.
exit
