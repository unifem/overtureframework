* two squares side by side
create mappings
rectangle
specify corners
0. 0. .65 1.
lines
9 7
boundary conditions
1 0 1 1
mappingName
leftSquare
exit
rectangle
specify corners
.35 0. 1. 1.
lines
9 7
boundary conditions
0 1 1 1
mappingName
rightSquare
exit
change a mapping
SquareMapping : square
mappingName
leftSquare
exit
make an overlapping grid
  2
  leftSquare
  rightSquare
  GRPAR
    isCellCentered
    CELL
    REPEAT
  EXIT
  PLOT
EXIT
save an overlapping grid
twosqcc.hdf
twosq
exit
