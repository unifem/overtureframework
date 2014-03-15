create mappings
spline
enter spline points
9
0 4e5 
5e4 1.4e5
1e5 5e4
2e5 1e4
3e5 0
4e5 0
6e5 0
7e5 0
9e5 0
mappingName
spline1
exit
spline
enter spline points
9
5e5 4e5
5.5e5 3e5
6.5e5 1.5e5
7.5e5 1e5
8e5 1e5
8.2e5 1e5
8.5e5 1e5
8.8e5 1e5
9e5 1e5
mappingName
spline2
exit
tfi
choose curves for sides
spline1
spline2
lines
30 30
exit
body of revolution
revolve which mapping?
TFIMapping
choose a point on the line to revolve about
9.5e5 0 0
lines
x+r
x+r
30 30 50
boundary conditions
1 0 2 2 -1 -1
x+r
x+r
mappingName
coonrev
exit
Box
reset
specify corners
8e5 0 -1.5e5 1.1e6 1e5 1.5e5
lines
40 30 40
boundary conditions
0 0 1 1 0 0
exit
exit this menu
create mappings
change a mapping
box
specify corners
7.5e5 0 -2e5 1.15e6 1e5 2e5
exit
change a mapping
box
lines
50 30 50
exit
exit this menu
create mappings
change a mapping
coonrev
lines
40 40 50
revolve which mapping?
TFIMapping
boundary conditions
1 0 2 2 -1 -1
mappingName
coonrev
exit
pause
exit this menu
make an overlapping grid
box
coonrev
Done choosing Mappings
Plot the CompositeGrid
Done specifying the CompositeGrid
exit
