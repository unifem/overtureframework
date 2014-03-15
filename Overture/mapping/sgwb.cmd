*
* make a hyperbolic surface grid
*
read plot3d file
wingbody.dat
read plot3d file
lines.dat
*
change a mapping
wingbody.dat-grid0
lines
21 11
x+r
x+r
x+r
x+r
exit
change a mapping
lines.dat-grid1
lines
51
exit
change a mapping
wingbody.dat-grid1
lines
21 101
exit
hyperbolic surface
choose the reference surface
wingbody.dat-grid1
x+r
x+r
x+r
number of lines in marching
2
far field distance
.1




