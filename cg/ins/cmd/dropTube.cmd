*
* This example shows a sphere dropping in a cylindrical tube
*
*
$solver = "choose best iterative solver";
* $solver = "multigrid";
$tolerance = "1.e-7";
$nu =.1;
* $hybrid = "use hybrid grid for surface integrals";
$hybrid = "do not use hybrid grid for surface integrals";
$tFinal=3.;
$g = -981.; # cm/s^2   -9.81 acceleration due to gravity standard value: 9.80665 m/s^2.
$vIn = 1.7;
$density = 1.04;
$radius=.2;
$tPrint=.025; 
$dtMax = 1.;
*
* $gridName = "dropTube.hdf"; $show="dropTube.show"; $vIn=0.; $tFinal=.5; $nu=.2; $density = 1.14; $radius=.2;
* $gridName = "dropTube0.hdf"; $show="dropTube0.show"; $vIn=0; $tFinal=.5; $nu=.2; $density = 1.14; $radius=.2;
* $gridName = "dropTube2.hdf"; $show="dropTube2.show"; $vIn=0; $tFinal=.5; $nu=.2; $density = 1.14; $radius=.2;
* $gridName = "dropTube2.hdf"; $show="dropTube2i.show"; $vIn=0; $tFinal=.5; $nu=.2; $density = 1.14; $radius=.2; $solver = "multigrid"; $dtMax=1.e-3;
$gridName = "dropTube1.hdf"; $show="dropTube2v.show"; $vIn=1.673; $tFinal=.5; $nu=.2; $density = 1.14; $radius=.2; $solver = "multigrid"; $dtMax=1.e-3; $tPrint=.05; 
* $gridName = "dropTube2.hdf"; $show="dropTube2v.show"; $vIn=1.673; $tFinal=.5; $nu=.2; $density = 1.14; $radius=.2; $solver = "multigrid"; $dtMax=1.e-3; $tPrint=.05; 
** $gridName = "dropTube2.hdf"; $show="dropTube2v.show"; $vIn=1.673; $tFinal=.5; $nu=.2; $density = 1.14; $radius=.2; $solver = "choose best iterative solver"; $dtMax=1.e-3; $tPrint=.05; 
* $gridName = "dropTube3.hdf"; $show="dropTube3.show"; $vIn=1.673; $tFinal=.25; $nu=.2; $density = 1.14; $radius=.2; $solver = "multigrid"; $dtMax=5.e-4; $tPrint=.05; 
* $gridName = "dropTube4.hdf"; $show="dropTube4.show"; $vIn=1.673; $tFinal=.25; $nu=.2; $density = 1.14; $radius=.2; $solver = "multigrid"; $dtMax=2.e-4; $tPrint=.05; 
* $gridName = "dropTube2.hdf"; $show="dropTubeRestart.show"; $vIn=1.7; $tFinal=.5; $nu=.2; $density = 1.14; $radius=.2; $solver = "multigrid"; $dtMax=1.e-3;
*
*
  $gridName
  incompressible Navier Stokes
  exit
*
  show file options
  * un-comment the next two lines to save a show file
   open
      $show
    frequency to flush
      1
  exit  
  turn off twilight zone
*
  dtMax $dtMax
*
  recompute dt every 10 *  1 * 2 * 5 * 10 * 1 
*
****
  turn on moving grids
*   detect collisions 1
****
  $pi =3.141592653;
  $volume = 4./3.*$pi*$radius**3; $mass=$volume*$density;
  specify grids to move
      $hybrid
      rigid body
        density
          $density
        mass
           $mass
         moments of inertia
           * $momentOfInertia=2./5.*$mass*$radius**2;
           $momentOfInertia=10.; # do this for now
           $momentOfInertia $momentOfInertia $momentOfInertia
         initial centre of mass
            0. 0. 0. * 0. .5 0.   0. 0. 0.   -.25 -.25 -.75
         done
 	sphere1-north-pole
	sphere1-south-pole
      done
  done
**********
*
  implicit
  choose grids for implicit
    all=implicit
  done
*
  pde parameters
    nu
      $nu 
*   -- specify the fluid density
   fluid density
     1.
*  turn on gravity
    gravity
       0. $g 0.
    *** OBPDE:second-order artificial diffusion 1
    OBPDE:second-order artificial diffusion 0 
    OBPDE:ad21,ad22  2,2 
   done
  boundary conditions
    all=noSlipWall, uniform(v=$vIn)
    cylinder(0,1)=inflowWithVelocityGiven,  uniform(p=1,v=$vIn)
    cylinder-core(0,1)=inflowWithVelocityGiven,  uniform(p=1,v=$vIn)
    cylinder(1,1)=outflow , pressure(1.*p+0.0*p.n=0.)
    cylinder-core(1,1)=outflow , pressure(1.*p+0.0*p.n=0.)
    done
*
  initial conditions
    uniform flow
     p=1., v=$vIn
* ---------
*    read from a show file
*     dropTube2v.show
*       5
  exit
*
  project initial conditions
*
  final time $tFinal 
  times to plot $tPrint
*
  plot and always wait
**  no plotting
*
*  initial conditions
*     read from a show file
*      twoDropNew2.show
*       24 48  -1 35 -1 1 4 -1 10 18  -1
*  exit
*
***************************************************
   pressure solver options
     $solver 
     relative tolerance
       $tolerance
     absolute tolerance
       $tolerance
      multigrid parameters
      *  alternating
      * cycles: 1=V, 2=W
      * -- coarse grid solver parameters ---
        Oges parameters
        fillin ratio
          55
        choose best iterative solver
        exit
      * ---
      number of cycles
        2
      residual tolerance
         $tolerance
      error tolerance
         $tolerance
      maximum number of iterations
        50
      maximum number of interpolation iterations
         8 30 10
 *      maximum number of levels
 *        5 4 3 2
 *      maximum number of extra levels
 *        4 3 2 1
      debug
        1 3 7 3 1 0 3
      exit
    exit
*
*******************************************************************
   implicit time step solver options
     $solver 
     relative tolerance
       $tolerance
     absolute tolerance
       $tolerance
      multigrid parameters
      *  alternating
      * cycles: 1=V, 2=W
      * -- coarse grid solver parameters ---
        Oges parameters
        fillin ratio
          55
        choose best iterative solver
        exit
      * ---
      number of cycles
        2
      residual tolerance
         $tolerance
      error tolerance
         $tolerance
      maximum number of iterations
        50
      maximum number of interpolation iterations
         8 30 10
 *      maximum number of levels
 *        5 4 3 2
 *      maximum number of extra levels
 *        4 3 2 1
      debug
        1 3 7 3 1 0 3
      exit
    exit
********************************************************
*  debug
*     63
  continue
*

* ----------- plot solution of sphere surface
    plot:v
    contour
      plot contours on grid boundaries
      (2,0,2) = (sphere1-north-pole,side,axis) (off)
      (3,0,2) = (sphere1-south-pole,side,axis) (off)
      exit
*      set view:0 0 0 0 1 0.827404 -0.160413 0.538211 -0.114096 0.89034 0.440768 -0.549896 -0.426101 0.718368
    exit
*-----------------------
*

movie mode
finish





   grid
*      raise the grid by this amount (2D) 1
*      raise the grid by this amount (2D) 2
    toggle shaded surfaces 0 0
    exit this menu
    plot:v

* output resolution
*  512
*movie and save
*  twoDrop
