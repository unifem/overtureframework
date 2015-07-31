#
# cmd file for tbm : test beam models
# 
nonlinear beam model
# 
tFinal: 0.4
tPlot: 0.01
cfl: 1.
# 
change beam parameters
#
use exact solution 1
#
elastic modulus: 1.666e6
$density=1000.; 
# $density=1000./4; 
density: $density
nu: 0.4
#
structure omega: 1.
# 
number of elements: 10
# number of elements: 20
# number of elements: 2
thickness: 0.05
# $thick=.05/4; 
# $thick=.05/16; 
# $thick=.001; 
thickness: $thick
length: 1
# 
bc left:pinned
bc right:pinned
#
build beam
exit

solve
