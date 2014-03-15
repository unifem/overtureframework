create mappings
  rectangle
    set corners
    0,5,.1,1.1
    lines
    151,31
    mappingName
    square
    share
    1 2 3 4
    exit
  rectangle
    set corners
    2.3,2.7,.1,.3
    lines
    21,11
    boundary conditions
    0 0 2 0
    share
    0 0 3 0
    mappingName
   inflow 
    exit
  rectangle
    set corners
    0,.2,.7,.9
    lines
    11,11
    boundary conditions
    3 0 0 0
    share
    1 0 0 0
    mappingName
    outflow_1 
    exit
  rectangle
    set corners
    4.8,5,.7,.9
    lines
    11,11
    boundary conditions
    0 3 0 0
    share
    0 2 0 0 
    mappingName
    outflow_2
    exit
exit this menu
generate an overlapping grid
square
inflow
outflow_1
outflow_2
done
change parameters
    discretization width
    all
    5 5
    ghost points
      all
      2 2 2 3
  exit
  compute overlap
exit
save a grid
inflow_outflow.hdf
grid
exit
