#include "A++.h"
/*
   Solve the wave equation as a test for P++
    WDH

Examples:

  srun -N1 -n4 -ppdebug pppWave


*/

typedef double real;
typedef doubleArray realArray;
typedef doubleSerialArray realSerialArray;

#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base,I2Base,I3Base;\
  int I1Bound,I2Bound,I3Bound;\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )


int 
main(int argc, char** argv)
{
  int optLevel=2; // 0=P++ , 1=A++, 2=C 
  
  ios::sync_with_stdio();

  int Number_of_Processors=0;
  
  Optimization_Manager::Initialize_Virtual_Machine("",Number_of_Processors,argc,argv);
  double sTime = MPI_Wtime(), maxTime;

  Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(1,1);

  Index::setBoundsCheck(off);
  // Do this to avoid unecessary communication:
  Optimization_Manager::setForceVSG_Update(Off);

  int myid = Communication_Manager::My_Process_Number;

  if( myid==0 )
  {
    printf("Usage: dmpirun -np N wave [optLevel]\n"
           "   optLevel: 0=P++ , 1=A++, 2=opt-C \n");
    
    for( int i=0; i<argc; i++ )
    {
      printf(" argv[%i]=%s\n",i,argv[i]);
    }
  }
  if( argc>1 )
  {
    sscanf(argv[1],"%i",&optLevel);
    printf("optLevel=%i (0=P++ , 1=A++, 2=C)\n",optLevel);
  }
  

   const int nx=1000, ny=1000;
//  const int nx=2000, ny=2000;
//  const int nx=10000, ny=10000;

  Index all;

  realArray ua[2], x(nx,ny), y(nx,ny);
  ua[0].redim(nx,ny);
  ua[1].redim(nx,ny);
  
  const real dx=1./(nx-1), dy=1./(ny-1);

  for (int i=0; i<nx; i++) x(i,all) = i*dx;
  for (int j=0; j<ny; j++) y(all,j) = j*dy;

  real c=1., pi=atan2(1.,1.)*4.;

  real dt = 1./(c*sqrt( 1./(dx*dx) + 1./(dy*dy) ));
  real cdt=c*dt;
  real t=0.;

#define EXACT(x,y,t) sin(pi*((x)-c*(t)))
  // initial conditions
  int current=0, old=1;
  ua[current] = EXACT(x,y,t);
  ua[old]     = EXACT(x,y,t-dt);

  sTime = MPI_Wtime() - sTime;
  MPI_Reduce(&sTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  if (myid == 0)  
  {
    printf("-----wave: starting computation: time for setup=%8.2e(s)\n"
           "      nx=%i, dx=%8.2e, dt=%8.2e, pi=%10.8f\n",maxTime,nx,dx,dt,pi);
  }
  

  sTime = MPI_Wtime();

  
  realSerialArray uaLocal[2];
  uaLocal[0].reference(ua[0].getLocalArrayWithGhostBoundaries());
  uaLocal[1].reference(ua[1].getLocalArrayWithGhostBoundaries());

  const realSerialArray & xLocal = x.getLocalArrayWithGhostBoundaries();
  const realSerialArray & yLocal = y.getLocalArrayWithGhostBoundaries();
    
  int numberOfSteps = 100;
  for( int step=0; step<numberOfSteps; step++ )
  {
    

    if( optLevel==0 )
    {
      // Use P++ arrays directly

      Index I1(1,nx-2), I2(1,ny-2);

      realArray & u = ua[current];
      realArray & uOld = ua[old];
      realArray & uNew = ua[old];
    
      uNew(I1,I2) = 2.*u(I1,I2) - uOld(I1,I2) + (cdt*cdt/(dx*dx))*( u(I1+1,I2)-2.*u(I1,I2)+u(I1-1,I2) )
	                                      + (cdt*cdt/(dy*dy))*( u(I1,I2+1)-2.*u(I1,I2)+u(I1,I2-1) );

      // Boundary conditions
      I1=Range(0,nx-1), I2=Range(0,ny-1);

      if( true )
      {
	uNew(0   ,I2  )=EXACT(x(   0,I2  ),y(   0,I2),t+dt);
	uNew(nx-1,I2  )=EXACT(x(nx-1,I2  ),y(nx-1,I2),t+dt);
	uNew(I1  , 0  )=EXACT(x(I1  , 0  ),y(I1  , 0  ),t+dt);
	uNew(I1  ,ny-1)=EXACT(x(I1  ,ny-1),y(I1  ,ny-1),t+dt);
      }
      

    }
    else if( optLevel==1 )
    {
      // Use local arrays and update ghost boundaries explicitly

      const realSerialArray & u = uaLocal[current];
      const realSerialArray & uOld= uaLocal[old];
      const realSerialArray & uNew = uOld;
      
      Range I1(max(1,u.getBase(0)),min(nx-2,u.getBound(0)));
      Range I2(max(1,u.getBase(1)),min(ny-2,u.getBound(1)));

      if( step==0 )
      {
	printf("myid=%i: optLevel=1 local u: [%i,%i][%i,%i] I1=[%i,%i] I2=[%i,%i]\n",myid,
               u.getBase(0),u.getBound(0),
               u.getBase(1),u.getBound(1),I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound());
      }
      
	
      uNew(I1,I2) = 2.*u(I1,I2) - uOld(I1,I2) + (cdt*cdt/(dx*dx))*( u(I1+1,I2)-2.*u(I1,I2)+u(I1-1,I2) )
	                                      + (cdt*cdt/(dy*dy))*( u(I1,I2+1)-2.*u(I1,I2)+u(I1,I2-1) );


      // Boundary conditions
      I1 = Range(max(0,u.getBase(0)),min(nx-1,u.getBound(0)));
      I2 = Range(max(0,u.getBase(1)),min(ny-1,u.getBound(1)));

      if( u.getBase(0)<=0 )
        uNew(0   ,I2  )=EXACT(xLocal(   0,I2  ),yLocal(   0,I2),t+dt);
      if( u.getBound(0)>=(nx-1) )
        uNew(nx-1,I2  )=EXACT(xLocal(nx-1,I2  ),yLocal(nx-1,I2),t+dt);
      if( u.getBase(1)<=0 )
        uNew(I1  , 0  )=EXACT(xLocal(I1  , 0  ),yLocal(I1  , 0  ),t+dt);
      if( u.getBound(1)>=(ny-1) )
        uNew(I1  ,ny-1)=EXACT(xLocal(I1  ,ny-1),yLocal(I1  ,ny-1),t+dt);

      ua[old].updateGhostBoundaries();
      
      //
    }
    else if( optLevel==2 )
    {
      // use local arrays with C-indexing

      const realSerialArray & u = uaLocal[current];
      const realSerialArray & uOld= uaLocal[old];
      const realSerialArray & uNew = uOld;

      const real * up = u.Array_Descriptor.Array_View_Pointer2;
      const int uDim0=u.getRawDataSize(0);
      const int uDim1=u.getRawDataSize(1);
#define U(i0,i1,i2) up[i0+uDim0*(i1+uDim1*(i2))]	
    
      const real * uOldp = uOld.Array_Descriptor.Array_View_Pointer2;
      const int uOldDim0=uOld.getRawDataSize(0);
      const int uOldDim1=uOld.getRawDataSize(1);
#define UOLD(i0,i1,i2) uOldp[i0+uOldDim0*(i1+uOldDim1*(i2))]	

      real * uNewp = uNew.Array_Descriptor.Array_View_Pointer2;
      const int uNewDim0=uNew.getRawDataSize(0);
      const int uNewDim1=uNew.getRawDataSize(1);
#define UNEW(i0,i1,i2) uNewp[i0+uNewDim0*(i1+uNewDim1*(i2))]	
      
      int i1,i2,i3;
      
      int hw = 1;  // stencil half-width

      Range I1(max(1,u.getBase(0)+hw),min(nx-2,u.getBound(0)-hw));
      Range I2(max(1,u.getBase(1)+hw),min(ny-2,u.getBound(1)-hw));
      Range I3(0,0);

      if( step==0 )
      {
	printf("myid=%i: optLevel=2 local u: [%i,%i][%i,%i] I1=[%i,%i] I2=[%i,%i]\n",myid,
               u.getBase(0),u.getBound(0),
               u.getBase(1),u.getBound(1),I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound());
      }
      
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	UNEW(i1,i2,i3) = 2.*U(i1,i2,i3) - UOLD(i1,i2,i3) 
	  + ( (cdt*cdt/(dx*dx))*( U(i1+1,i2,i3)-2.*U(i1,i2,i3)+U(i1-1,i2,i3) ) +
	      (cdt*cdt/(dy*dy))*( U(i1,i2+1,i3)-2.*U(i1,i2,i3)+U(i1,i2-1,i3) ) );
      }
      

      // Boundary conditions
      I1 = Range(max(0,u.getBase(0)),min(nx-1,u.getBound(0)));
      I2 = Range(max(0,u.getBase(1)),min(ny-1,u.getBound(1)));

      if( u.getBase(0)<=0 )
        uNew(0   ,I2  )=EXACT(xLocal(   0,I2  ),yLocal(   0,I2),t+dt);
      if( u.getBound(0)>=(nx-1) )
        uNew(nx-1,I2  )=EXACT(xLocal(nx-1,I2  ),yLocal(nx-1,I2),t+dt);
      if( u.getBase(1)<=0 )
        uNew(I1  , 0  )=EXACT(xLocal(I1  , 0  ),yLocal(I1  , 0  ),t+dt);
      if( u.getBound(1)>=(ny-1) )
        uNew(I1  ,ny-1)=EXACT(xLocal(I1  ,ny-1),yLocal(I1  ,ny-1),t+dt);

      ua[old].updateGhostBoundaries();


    }
    else
    {
      throw "error";
    }
    
    
    
    t+=dt;

    old=current;
    current = (current+1) % 2;
  }

  sTime = MPI_Wtime() - sTime;
  MPI_Reduce(&sTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  // compute the max error
  real err,errMax;
  if( true || optLevel==0 )
  {
    Index I1(0,nx), I2(0,ny);
    realArray & u = ua[current];

    errMax=max(fabs(u(I1,I2)-EXACT(x(I1,I2),y(I1,I2),t)));
  }
  if( myid==0 )
  {
    printf("\n *** wave: max error at t=%8.2e is %8.2e \n",t,errMax);
  }


  
  if( myid==0 )
  {
    printf(" number of messages sent=%i\n",Diagnostic_Manager::getNumberOfMessagesSent());
    printf(" number of messages received=%i\n",Diagnostic_Manager::getNumberOfMessagesReceived());
    printf("\n------wave: nx=%6i, steps=%i, np=%i,  cpu=%8.2e (s) \n\n",nx,numberOfSteps,
	   Communication_Manager::numberOfProcessors(),maxTime);
  }
  
  Optimization_Manager::Exit_Virtual_Machine();

  return 0;
}
