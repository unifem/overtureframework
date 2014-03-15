*
* OverBlown command file for a square
*
$cfl=.9; $degreeX=2; $degreeT=2;  $debug=0; $bcOption=3; $ad=5.; $ghostErr=0; 
$bc00 = "square(0,0)=slipWall";
$bc01 = "square(0,1)=slipWall";
$bca = "all=slipWall"; 
$bcd="all=dirichletBoundaryCondition"; 
$bc=$bc00;
$tzShape="square";
*
*  square5.hdf
*  square8.hdf
$gridName = "square10.hdf";  $tFinal=.01; $tPlot=.005; $bcOption=4; $degreeX=2; $degreeT=1; $debug=3; $ad=0.;
* $gridName = "squareReverse10.hdf";  $tFinal=.25; $tPlot=.005; $bcOption=4; $degreeX=2; $degreeT=1; $debug=1; $bc=$bc01;
* $gridName = "square20.hdf";  $tFinal=.5; $tPlot=.1; $bcOption=4; $degreeX=2; $degreeT=1; $debug=1; $ad=0.; $bc=$bca;
* $gridName = "square20.hdf";  $tFinal=.5; $tPlot=.1; $bcOption=4; $degreeX=2; $degreeT=1; $debug=1; $ad=0.; $bc=$bcd; 
* $gridName = "rotatedSquare20.hdf";  $tFinal=.1; $tPlot=.1; $bcOption=4; $degreeX=2; $degreeT=2; $debug=1; $ad=0.; $bc=$bca;
* $gridName = "stretchedSquare80.hdf";  $tFinal=.5; $tPlot=.1; $bcOption=4; $degreeX=2; $degreeT=2; $debug=1; $ad=2.; $bc=$bca;
* $gridName = "square40.hdf";  $tFinal=.25; $tPlot=.05; $bcOption=4; $degreeX=2; $degreeT=2; $debug=1; 
* $gridName = "rotatedSquare10.hdf";  $tFinal=.25; $tPlot=.05; $bcOption=4; $degreeX=1; $degreeT=1; $debug=1; 
* $gridName = "square20.hdf";  $tFinal=.25; $tPlot=.05; $bcOption=4; $degreeX=2; $degreeT=0; $debug=3; 
* $gridName = "nonSquare10.hdf";  $tFinal=.02; $tPlot=.01; 
* $gridName = "nonSquare5.hdf";  $tFinal=.05; $tPlot=.01; 
* $gridName = "rotatedSquare.hdf";  $tFinal=.05; $tPlot=.01; 
* $gridName = "quarterAnnulus.hdf";  $tFinal=.5; $tPlot=.1;  $bcOption=4; $degreeX=2; $degreeT=1; $debug=1; 
* $gridName = "quarterAnnulus2.hdf";  $tFinal=.5; $tPlot=.1;  $bcOption=4; $degreeX=1; $degreeT=1; $debug=1; 
* $gridName = "annulus0.hdf";  $tFinal=.05; $tPlot=.01; $bcOption=4; $degreeX=1; $degreeT=1; $debug=3;  $ad=2.;
* $gridName = "annulus1.hdf";  $tFinal=.2; $tPlot=.05; 
* $gridName = "annulus2.hdf";  $tFinal=.5; $tPlot=.1;  $bcOption=4; $degreeX=1; $degreeT=1; $debug=1;  $ad=2.; 
*
* -- 3d --
* $gridName = "box10.hdf";  $tFinal=.5; $tPlot=.1; $bcOption=4; $degreeX=2; $degreeT=1; $debug=1; $ad=0.; $bc=$bcd; 
* $gridName = "boxsbse.hdf";  $tFinal=.5; $tPlot=.1; $bcOption=4; $degreeX=2; $degreeT=1; $debug=1; $ad=0.; $bc=$bcd; 
* $gridName = "rbibe.hdf";  $tFinal=.5; $tPlot=.1; $bcOption=4; $degreeX=2; $degreeT=1; $debug=1; $ad=0.; $bc=$bcd; 
* $gridName = "qsib.hdf";  $tFinal=.1; $tPlot=.01; $bcOption=4; $degreeX=2; $degreeT=1; $debug=1; $ad=0.; $bc=$bcd; 
*  -- reverse means swap r <-> s
* $gridName = "annulusReverse3.hdf";  $tFinal=.2; $tPlot=.1;  $bcOption=4; $degreeX=2; $degreeT=2; $debug=1;  $ad=0.; $bc=$bca; $tzShape="circle";
* 
* $gridName = "annulus3.hdf";  $tFinal=.05; $tPlot=.001; 
* $gridName = "annulus4.hdf";  $tFinal=.05; $tPlot=.001; 
*  square16.hdf
* $gridName = "square32.hdf";  $tFinal=.1; $tPlot=.02; 
* $gridName = "halfCylinder.hdf";  $tFinal=.01; $tPlot=.005; 
* nonSquare8.hdf
* $gridName = "nonSquare20.hdf";  $tFinal=.1; $tPlot=.05; 
*
$gridName
*
   compressible Navier Stokes (Godunov)
**   compressible Navier Stokes (non-conservative) 
   exit
*  
  turn on polynomial
  degree in time $degreeT
  degree in space $degreeX
  ** turn on trig
*
*
  final time (tf=)
    $tFinal
  times to plot (tp=)
    $tPlot
  plot and always wait
*  no plotting
  cfl
    $cfl
  debug
    $debug
* 
  check error on ghost
    $ghostErr
*
  pde parameters
    mu
     0.0
    kThermal
     0.0
    heat release
      0.
    rate constant
      0.
   reciprocal activation energy
     1.
  done
*
  $rc=0; $uc=1; $vc=2; $tc=3; 
* 
* default coefficients for poly TZ:  space coeff: $rmn : x^n y^n ,  time coeff: $rm : t^m 
  $r00=1.; $r10=0.; $r01=0.; $r20=0.; $r11=0.; $r02=0.;   $r0=1.; $r1=0.; $r2=0.; 
  $u00=0.; $u10=0.; $u01=0.; $u20=0.; $u11=0.; $u02=0.;   $u0=1.; $u1=0.; $u2=0.; 
  $v00=0.; $v10=0.; $v01=0.; $v20=0.; $v11=0.; $v02=0.;   $v0=1.; $v1=0.; $v2=0.; 
  $t00=0.; $t10=0.; $t01=0.; $t20=0.; $t11=0.; $t02=0.;   $t0=1.; $t1=0.; $t2=0.; 
* 
*
  if( $degreeX>=1 ){ $r00=1.;  $t00=5.; $t10=.25; $t01=.125; }
*
  if( $degreeX>=2 ){ $r00=1.; $r20=.2; $r02=.3;  $t20=.125; $t02=.0625; }
*
*    (u,v)=(-y,x):
  if( $tzShape eq "circle" && $degreeX==1 )\
  { $u01=-1.; $v10=1.; $t00=1.; }
*    (-y,x)*( 1+.5*(x-y) ):
  if( $tzShape eq "circle"  && $degreeX==2 )\
  { $u01=-1.; $u11=-.5; $u02=.5;  $v10=1.; $v11=-.5; $v20=.5; }
* 
  if( $tzShape eq "square" && $degreeX==1 )\
  { $u10=1.; $v00=4.; $v10=1./3.; $v01=1./6.; }
*
*   (u,v) = (x(1-x),y(1-y))
*   r = 1 + .25*(x-.5)^2 + .25*(y-.5)^2 
*   T = 2 +  .5*(x-.5)^2 +  .5*(y-.5)^2 
  if( $tzShape eq "square"  && $degreeX==2 )\
  { $u10=1; $u20=-1; $v01=1; $v02=-1; \
    $r00=1.+2.*.25*.25; $r10=-.25; $r20=.25; $r01=-.25; $r02=.25; \
    $t00=2.+2.*.5*.25; $t10=-.5; $t20=.5; $t01=-.5; $t02=.5;   }
*
  if( $degreeT >= 1 ){ $r1=.5; $u1=.5; $v1=.5;  $t1=.5;  }
  if( $degreeT >= 2 ){ $r2=1./3.; $u2=1./3.; $v2=1./3.; $t2=1./3.; }
* 
  $commands="cx(0,0,0,$rc)=$r00\n cx(0,0,0,$uc)=$u00\n cx(0,0,0,$vc)=$v00\n cx(0,0,0,$tc)=$t00\n" . \
            "cx(1,0,0,$rc)=$r10\n cx(1,0,0,$uc)=$u10\n cx(1,0,0,$vc)=$v10\n cx(1,0,0,$tc)=$t10\n" . \
            "cx(0,1,0,$rc)=$r01\n cx(0,1,0,$uc)=$u01\n cx(0,1,0,$vc)=$v01\n cx(0,1,0,$tc)=$t01\n" . \
            "cx(2,0,0,$rc)=$r20\n cx(2,0,0,$uc)=$u20\n cx(2,0,0,$vc)=$v20\n cx(2,0,0,$tc)=$t20\n" . \
            "cx(1,1,0,$rc)=$r11\n cx(1,1,0,$uc)=$u11\n cx(1,1,0,$vc)=$v11\n cx(1,1,0,$tc)=$t11\n" . \
            "cx(0,2,0,$rc)=$r02\n cx(0,2,0,$uc)=$u02\n cx(0,2,0,$vc)=$v02\n cx(0,2,0,$tc)=$t02\n" . \
            "    ct(0,$rc)=$r0 \n     ct(0,$uc)=$u0 \n     ct(0,$vc)=$v0 \n     ct(0,$tc)=$t0 \n" . \
            "    ct(1,$rc)=$r1 \n     ct(1,$uc)=$u1 \n     ct(1,$vc)=$v1 \n     ct(1,$tc)=$t1 \n" . \
            "    ct(2,$rc)=$r2 \n     ct(2,$uc)=$u2 \n     ct(2,$vc)=$v2 \n     ct(2,$tc)=$t2 \n";
*
  OBTZ:assign polynomial coefficients
*    degreeX=1: u=-y, v=x
   $commands
  done
*
  boundary conditions
    all=slipWall
*    all=dirichletBoundaryCondition
*    $bc
*   square(0,1)=slipWall
*     square(1,0)=slipWall
*     square(0,1)=slipWall
*
*    quarter(0,1)=slipWall
*    Annulus(0,1)=slipWall
*    Annulus(0,0)=slipWall
   done
*********************************************************
*   choose the new slip wall BC
*      1=slipWallPressureEntropySymmetry
*      2=slipWallTaylor
*      3=slipWallCharacteristic
**  OBPDE:slip wall boundary condition option 1
*   OBPDE:slip wall boundary condition option 2
**  OBPDE:slip wall boundary condition option $bcOption
**  OBPDE:artificial diffusion $ad $ad $ad $ad $ad $ad $ad
*********************************************************
continue
*
continue

movie mode 
finish










