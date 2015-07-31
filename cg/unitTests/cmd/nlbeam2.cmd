#
# cmd file for tbm : test beam models
#    -- compare to cgsm case
#  lambda=mu=rho=0 
#    E = 2.5 , nu=.25,
#     h=.04 --> period T=8.7 (Euler Bernoulli)
#     h=.02 --> period=17.43
$tf=11.; $tp=.1; $thick=0.02; $cfl=1.; 
$density=1.; 
# 
GetOptions( "tf=f"=>\$tf,"tp=f"=>\$tp,"thick=f"=>\$thick,"cfl=f"=>\$cfl );
#
nonlinear beam model
# 
tFinal: $tf
tPlot: $tp
cfl: $cfl
# 
change beam parameters
#
use exact solution 1
#
elastic modulus: 2.5
density: $density
nu: 0.25
#
structure omega: 1.
# 
number of elements: 21
# number of elements: 20
# number of elements: 2
thickness: $thick
length: 1
# 
bc left:pinned
bc right:pinned
#
build beam
exit

solve
