#include "Cgsm.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "UnstructuredMapping.h"


#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)


void Cgsm::
getEnergy( int current, real t, real dt )
// ============================================================================================================
//   Determine the energy (or determine the symmetry of the operator)
// ============================================================================================================
{
  bool checkSymmetry=false;
  // printF(" **** getEnergy computeEnergy=%i\n",computeEnergy);

  if( !computeEnergy && !checkSymmetry )
    return;

  real time0=getCPU();
  int & debug = parameters.dbase.get<int >("debug");
  
  const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;
  const int next = (current+1) % numberOfTimeLevels;
  
  FILE *& debugFile  =parameters.dbase.get<FILE* >("debugFile");
  FILE *& logFile    =parameters.dbase.get<FILE* >("logFile");
  FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

  const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  const int & uc =  parameters.dbase.get<int >("uc");
  const int & vc =  parameters.dbase.get<int >("vc");
  const int & wc =  parameters.dbase.get<int >("wc");
  const int & rc =  parameters.dbase.get<int >("rc");
  const int & tc =  parameters.dbase.get<int >("tc");
  const int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
  const int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");

  real & rho=parameters.dbase.get<real>("rho");
  real & mu = parameters.dbase.get<real>("mu");
  real & lambda = parameters.dbase.get<real>("lambda");
  RealArray & muGrid = parameters.dbase.get<RealArray>("muGrid");
  RealArray & lambdaGrid = parameters.dbase.get<RealArray>("lambdaGrid");

  const int numberOfComponentGrids = cg.numberOfComponentGrids();
  const int numberOfDimensions = cg.numberOfDimensions();

  totalEnergy=0.;
  
  for( int grid=0; grid<numberOfComponentGrids; grid++ )
  {
    MappedGrid & mg = cg[grid];
    const intArray & mask = mg.mask();

    lambda = lambdaGrid(grid);
    mu = muGrid(grid);
    c1=(mu+lambda)/rho, c2= mu/rho;

    real c=c1;  // fix this

    real energy=0.;  // energy on this grid

    // E = \| u-um \|^2/dt^2 - (u,A um)
    // this is not really correct:

    realMappedGridFunction &u  = gf[current].u[grid];
    realMappedGridFunction &um = gf[prev].u[grid];
	    
    mg.update(MappedGrid::THEcenterJacobian);
    const realArray & centerJacobian = mg.centerJacobian();

    // Compute the "energy" in the following component:
    int sc=uc;

    real e1,e2;

    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];

    getIndex(mg.indexRange(),I1,I2,I3); // use indexRange -- avoid periodic images
    getIndex(mg.indexRange(),J1,J2,J3,1); 

    // Are these needed: (?)
    //         um.periodicUpdate(); // ************************************
    // 	u.periodicUpdate();

    // mgop.useConservativeApproximations(true);  // ****************

    // Could use work-space here
    realArray uma(u.dimension(0),u.dimension(1),u.dimension(2),u.dimension(3));
    realArray ua(u.dimension(0),u.dimension(1),u.dimension(2),u.dimension(3));

    bool zeroBoundaryValues=false;
    if( !zeroBoundaryValues )
    {
      ua=u;
      uma=um;
    }
    else
    {
      // for testing symmetry -- set boundary values to zero.
      // Index I1,I2,I3;
      int extra=-2;
      getIndex(mg.gridIndexRange(),I1,I2,I3,extra); 

      ua=0.;
      ua(I1,I2,I3,sc)=u(I1,I2,I3,sc);
      uma=0.;
      uma(I1,I2,I3,sc)=um(I1,I2,I3,sc);

// 	  ua=0.;
// 	  ua(J1,J2,J3,sc)=u(J1,J2,J3,sc);
// 	  uma=0.;
// 	  uma(J1,J2,J3,sc)=um(J1,J2,J3,sc);

    }


    realArray uDot(I1,I2,I3);
    uDot = u(I1,I2,I3,sc)-um(I1,I2,I3,sc);

    realArray lap(u.dimension(0),u.dimension(1),u.dimension(2),u.dimension(3));
    realArray lap2,lapSq; 

    MappedGridOperators & mgop = (*cgop)[grid];
        

    if( false && t<dt )
    {
      ::display(mg.inverseVertexDerivative(),"rx",pDebugFile,"%10.8f ");
      ::display(mg.centerJacobian(),"centerJacobian",pDebugFile,"%10.8f ");
    }

    mgop.setOrderOfAccuracy(orderOfAccuracyInSpace);
    mgop.derivative(MappedGridOperators::laplacianOperator,uma,lap,I1,I2,I3);
    if( orderOfAccuracyInTime==4 )
    { // add correction for 4th-order modified equation
      lap2.redim(u.dimension(0),u.dimension(1),u.dimension(2),u.dimension(3));
      lapSq.redim(u.dimension(0),u.dimension(1),u.dimension(2),u.dimension(3));
	  
      mgop.setOrderOfAccuracy(orderOfAccuracyInSpace-2);
	  
      mgop.derivative(MappedGridOperators::laplacianOperator,uma,lap2,J1,J2,J3);  
      mgop.derivative(MappedGridOperators::laplacianOperator,lap2,lapSq,I1,I2,I3);
      mgop.setOrderOfAccuracy(orderOfAccuracyInSpace);
      lap += lapSq*(c*c*dt*dt/12.); 
    }
	
    real e3=0.;
    if( cg.numberOfComponentGrids()==1 )
    {
      e1 =  sum(uDot*uDot*centerJacobian(I1,I2,I3))/(dt*dt);

      e2 = c*c*sum( ua(I1,I2,I3,sc)*lap(I1,I2,I3,sc)*centerJacobian(I1,I2,I3) );

      if( checkSymmetry )
      {
	// here we check if the operator is symmetric

	mgop.derivative(MappedGridOperators::laplacianOperator,ua,lap,I1,I2,I3);
	if( orderOfAccuracyInTime==4 )
	{
	  mgop.setOrderOfAccuracy(orderOfAccuracyInSpace-2);
	  mgop.derivative(MappedGridOperators::laplacianOperator,ua,lap2,J1,J2,J3);  
	  mgop.derivative(MappedGridOperators::laplacianOperator,lap2,lapSq,I1,I2,I3);
	  mgop.setOrderOfAccuracy(orderOfAccuracyInSpace);
	  lap += lapSq*(c*c*dt*dt/12.); 
	}

	e3=c*c*sum( uma(I1,I2,I3,sc)*lap(I1,I2,I3,sc)*centerJacobian(I1,I2,I3) );

	printF("  check symmetry:    e1=%e e2=(u,Lum)=%e e3=(um,Lu)=%e diff=e2-e3=%e\n",e1,e2,e3,e2-e3);
      }
    }
    else
    {
      where( mask(I1,I2,I3)!=0 )
      {
	e1 =  sum(uDot*uDot*centerJacobian(I1,I2,I3))/(dt*dt);
	e2 = c*c*sum(u(I1,I2,I3,sc)*lap(I1,I2,I3,sc)*centerJacobian(I1,I2,I3));
      }
    }
	  
    energy=.5*(e1-e2)*mg.gridSpacing(0)*mg.gridSpacing(1);
    real energy2=.5*(e1-e3)*mg.gridSpacing(0)*mg.gridSpacing(1);


    if( debug & 4 )
      printF(" ==> t=%9.3e, grid=%i: energy (e1-e2) =%16.10e, energy (e1-e3)=%16.10e \n",t,grid,energy,energy2);

    // energy=.5*dx[0]*dx[1]*sum( SQR(u(I1,I2,I3,hz))+SQR(u(I1,I2,I3,ex))+SQR(u(I1,I2,I3,ey)) );

    totalEnergy+=energy;
  
  } // end for grid
  
  if( initialTotalEnergy<0. ) initialTotalEnergy=totalEnergy;

  if( myid==0 )
  {
    for( int fileio=0; fileio<2; fileio++ )
    {
      FILE *output = fileio==0 ? logFile : stdout;

      fPrintF(output,"-->t=%10.4e dt=%7.1e (approx) energy=%13.6e, energy/energy(0)=%9.3e, "
              "energy-energy(0)=%9.3e\n",
              t,dt,totalEnergy,totalEnergy/initialTotalEnergy,totalEnergy-initialTotalEnergy);

    }
  }
  
//  timing(SmParameters::timeForGetEnergy)+=getCPU()-time0;
}

