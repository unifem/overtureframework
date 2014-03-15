*
* two boxes for Jerry and Barry
create mappings
Box
specify corners
* 8e5 -1e6 0 2e6 1e6 2e3
  .8 -1. 0. 2. 1. 2.
lines
25 41 9
boundary conditions
0 1 2 2 3 3
share
0 0 2 2 3 3
mappingName
box1
exit
Box
specify corners
* 0 -1e6 0 1.2e6 1e6 2e3
0 -1 0 1.2 1 2
lines
37 81 9
boundary conditions
1 0 2 2 3 3
share
0 0 2 2 3 3
mappingName
box2
exit
stretch coordinates
stretch
specify stretching along axis=0 (x1)
layers
1
1.5 2 0
exit
exit
exit
view mappings
box1
stretched-box2
erase and exit
exit this menu
make an overlapping grid
box1
stretched-box2
Done choosing Mappings
Specify new MappedGrid Parameters
numberOfGhostPoints
2 2 2 2 2 2
2 2 2 2 2 2
Done specifying MappedGrid parameters
Specify new CompositeGrid parameters
mayCutHoles
No
Repeat
Done specifying CompositeGrid parameters
Done specifying the CompositeGrid
save an overlapping grid
twoBox2.hdf
compbox3
exit
