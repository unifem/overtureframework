************************************
*  Parallel ogmg test
*
* mpirun -np 1 ogmgt ps.cmd
* mpirun -np 2 ogmgt -degreex=2 ps.cmd
* mpirun -tv -np 2 ogmgt ps.cmd
* totalview srun -a -N2 -n2 -ppdebug ogmgt ps.cmd
* mpirun -np 2 -dbg=valgrindebug ogmgt ps.cmd
***********************************
$debug = 3; $smooth="2 1"; $maxIt=9; $smoother="red black jacobi"; $interpolationIterations=4;
$rtol=1.e-10; $atol=1.e-10; 
*  number of cycles: 1=V 2=W
$cycles=1; 
$trig = "turn on trigonometric"; $poly="turn on polynomial"; $tz=$trig; 
*
* $grid="square5"; $debug = 15; $smooth="1 0"; $maxIt=5; $tz=$poly; $smoother="jacobi"; 
* $grid="square5"; $debug = 15; $tz=$poly; 
* $grid="nonSquare5"; $debug = 15;  $tz=$poly;
* $grid="square8"; $debug = 3;  $tz=$poly; 
* $grid="/home/henshaw.0/Overture/ogmg/cmd/nonSquare8mg"; $debug =15;  $tz=$poly;
* $grid="square16"; $debug = 3;  $tz=$poly; 
* $grid="square32"; $debug = 3;  $tz=$poly;
* $grid="square256"; $debug = 3;  $tz=$poly; 
$grid="square1024"; $debug = 3;  $tz=$poly; # $smoother="red black";
* $grid="square2048"; $debug = 3;  $tz=$poly; 
* $grid="square4096"; $debug = 3;  $tz=$poly; 
* square16
* square10
* square32
* $grid="square64"; $debug = 3;  $tz=$poly; 
* $grid="square16.order4";  $debug = 3; $tz=$poly;
* square64.order4
* square128.order4
* square256.order4
* $grid="sis2mg.hdf"; $debug = 3;  $tz=$poly; 
* --> read a grid that already has MG levels:
* $grid="sis2mgmg.hdf"; $debug = 3;  $tz=$poly;
* $grid="sbs1mgmg.hdf"; $debug = 7;  $tz=$poly;
* $grid="sis2mg.hdf"; $tz=$poly; $name="sis2mgmg.hdf"; $maxIt=5; $debug=15;
* $grid="sis2mg.hdf"; $tz=$poly; $name="sis2mgmg.hdf"; 
* cic3.order4
*
* $grid="/home/henshaw.0/Overture/ogmg/cmd/cic0mg.hdf"; $debug =3;  $tz=$poly;
* $grid="/home/henshaw.0/Overture/ogmg/cmd/cic3mg.hdf"; $debug =3;  $tz=$poly; $interpolationIterations=4;
* $grid="/home/henshaw.0/Overture/ogmg/cmd/cic5mg.hdf"; $debug =3;  $tz=$poly; $interpolationIterations=4;
** $grid="/home/henshaw.0/Overture/ogmg/cmd/cic6mg.hdf"; $debug =3;  $tz=$poly; $smoother="red black";
* $grid="/home/henshaw.0/Overture/ogmg/cmd/cic7mg.hdf"; $debug =3;  $tz=$poly; $interpolationIterations=2;
* about 1M pts
*cic4.order4
*
* $grid="box16"; $debug = 3;  $tz=$poly; 
* $grid="box64"; $debug = 3;  $tz=$poly; 
* $grid="box128"; $debug = 3;  $tz=$poly; 
* $grid="box256"; $debug = 3;  $tz=$poly; 
* $grid="box512"; $debug = 3;  $tz=$poly; 
*
$grid
* 
* 
laplace (predefined)
* heat equation (predefined)
** test smoother
$tz
 set trigonometric frequencies
   2. 2. 2.
* turn off twilight zone
* dirichlet=1 neumann=2 mixed=3
*  bc(0,0,0)=2
*  bc(1,0,0)=3
*  bc(0,1,0)=2
*  bc(1,1,0)=2
*
*==================CHANGE PARAMETERS =======================================
change parameters
* show smoothing rates
* -- specify the grid with the MG levels in it: 
* read the multigrid composite grid
*   $name
* ----------
* show smoothing rates
use new red-black smoother
******
Oges parameters
*   PETScNew
 choose best iterative solver
* parallel bi-conjugate gradient stabilized
 parallel gmres
 relative tolerance
   $rtol
 absolute tolerance
   $atol
exit
* 
* iterate on coarse grid
******
* +++++++++++++++++++++++++++++++++++++++++++++++++++++ ADD this:
use new fine to coarse BC
* ++++++++++++++++++++++++++++++++++++++++++++++++++++++
*  minimum number of initial smooths
*   100 20 10 50 5 10 100 50 100 10 50 
*
*do not use locally optimal omega
*omega red-black
* 1.10
*  1.064 1.08 1.0875 1.09 1.085 1.1 1.  1.05 1.0 1. 1.05 1.025  1.1 1.09  1.1 1.07
* omega line-zebra
*  1.85 1.75 1.8 1.65 1.75 1.7 1.6  05 .95  1.0
** jacobi
* gauss-seidel
* alternate smoothing directions
***********
* do not use split step line solver
***********
$smoother
* alternating jacobi
*** alternating zebra
* line zebra direction 1
* line zebra direction 2
* line jacobi direction 2
* smoother(0,0)=alternating
* do not use automatic sub-smooth determination
*
* do not average coarse grid equations
*
residual tolerance
  1.e-14 1.e-13 1.e-14  1.e-15 1.e-14 1.e-13 1.e-12
error tolerance
  1.e-13
maximum number of iterations
  $maxIt
*
* *NOTE* use a W-cycle for alternating-zebra
number of smooths
   $smooth
number of cycles 
  $cycles
***********************
*  use an F cycle
************************
*
** show smoothing rates
* do not use optimized version
* boundary averaging option
*   0 0 dirichlet
*   3 3 partial
*  5 5  lumped
* ghost line averaging option
*   1 6 * default, impose-Neumann
*
maximum number of interpolation iterations
  $interpolationIterations
* 
output a matlab file
exit
 debug
   $debug
exit

