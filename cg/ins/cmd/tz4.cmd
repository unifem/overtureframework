*
* cgins - test fourth order accurate
*
$tFinal=1.; $tPrint=.1;  $degreeSpace=4; $degreeTime=3; $debug=0; 
* np: axis1=not periodic, axis2=periodic
* square64.order4
* square32.order4
* $gridName = "square6.order4.hdf"; $tFinal=1.; $tPrint=.001; $degreeSpace=4; $degreeTime=4; $debug=0;
*   -- exact for (x,T)=(4,3) but not (X,T)=(4,4)
* $gridName = "square16.order4.hdf"; $tFinal=1.; $tPrint=.1; $degreeSpace=4; $degreeTime=4; 
$gridName = "square16.order4.hdf"; $tFinal=1.; $tPrint=.1; $degreeSpace=4; $degreeTime=3; 
* square8.order4
* sis.order4
* sis2.order4
* sis3.order4
* sbs1.order4
* sbs.order4
* sbs2.order4
* square16np.order4
* square8np.order4
* square8.order4
* square16np.order4
* square32np.order4
* square16p.order4
* square32p.order4
* square40
* square40p
* square8p.order4
* square8p
* square4.order4
* box8.order4
* box8p.order4
* box16p.order4
* bib.order4
*
$gridName
*
  incompressible Navier Stokes
  exit
*
 fourth order accurate
************************
   adams PC order 4
************************
*
  turn on polynomial
  degree in space $degreeSpace
* 
  degree in time $degreeTime
*
* turn on trig
* frequencies (x,y,z,t) 2 2 2 2
*
*  turn off twilight zone 
*
  final time $tFinal
  times to plot $tPrint
*  ***** need to fix cfl
*    cfl=.75 needed for PC44, cfl=.9 ok for PC34
  cfl 
    .9
*
  plot and always wait
**  no plotting
*  OBPDE:divergence damping 0.
*
OBPDE:use new fourth order boundary conditions 1
*
  pde parameters
    nu
     .1
    done
*
*    pressure solver options
*     debug
*      63
*   exit
  boundary conditions
*    all=dirichletBoundaryCondition
     all=noSlipWall
*     square(0,0)=noSlipWall
*     all=slipWall
*     square(0,0)=inflowWithVelocityGiven , uniform(p=1.,u=1.)
*     square(1,0)=outflow 
*     square(0,1)=slipWall
*     square(1,1)=slipWall
*    square(1,0)=outflow , pressure(.1*p+1.*p.n=0.)
*     square(0,0)=inflowWithVelocityGiven , parabolic(d=.2,p=1.,u=1.), oscillate(t0=.3,omega=2.5)
   done
*
*  initial conditions
*    uniform flow
*    p=1., u=1.
*  exit
*  project initial conditions
  debug
    $debug 
  check error on ghost 
    2
 continue
 movie mode
 finish

 contour
  ghost lines 2
  smaller
  exit
