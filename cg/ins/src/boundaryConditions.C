#include "Cgins.h"
#include "App.h"
#include "ParallelUtility.h"

#define mixedRHS(component,side,axis,grid)         bcData(component+numberOfComponents*(0),side,axis,grid)
#define mixedCoeff(component,side,axis,grid)       bcData(component+numberOfComponents*(1),side,axis,grid)
#define mixedNormalCoeff(component,side,axis,grid) bcData(component+numberOfComponents*(2),side,axis,grid)

// void Cgins::
// gridAccelerationBC(const int & grid,
// 		   const real & t0,
// 		   MappedGrid & c,
// 		   realMappedGridFunction & u ,
// 		   realMappedGridFunction & f ,
// 		   realMappedGridFunction & gridVelocity ,
// 		   realSerialArray & normal,
// 		   const Index & I1,
// 		   const Index & I2,
// 		   const Index & I3,
// 		   const Index & I1g,
// 		   const Index & I2g,
// 		   const Index & I3g,
// 		   int side,
//                    int axis   )
//\begin{>>MappedGridSolverInclude.tex}{\subsection{gridAccelerationBC}} 
void Cgins::
gridAccelerationBC(const int & grid,
		   const real & t0,
		   GridFunction & gf0, 
		   realCompositeGridFunction & f0,
		   int side,
                   int axis   )
//=================================================================================
// /Description:
// Add the grid acceleration in the normal direction, -n.x_tt,  to the function f
//   f is normally the function that holds the rhs for the pressure eqn
// /I1,I2,I3 (input) : Index's on boundary
// /I1g,I2g,I3g (input) : Index's on ghost line, fill in f(I1g,I2g,I3g)
// /side,axis (input) : fill in this face.
//\end{MappedGridSolverInclude.tex}  
//=================================================================================
{
  MappedGrid & c = gf0.cg[grid];
  realMappedGridFunction & u = gf0.u[grid];
  realMappedGridFunction & f = f0[grid];
  realMappedGridFunction & gridVelocity = gf0.getGridVelocity(grid);

  #ifdef USE_PPP
    realSerialArray & normal = c.vertexBoundaryNormalArray(side,axis);
    realSerialArray gridVelocityLocal; getLocalArrayWithGhostBoundaries(gridVelocity,gridVelocityLocal);
    realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f,fLocal);
  #else
    realSerialArray & normal = c.vertexBoundaryNormal(side,axis);
    realSerialArray & gridVelocityLocal=gridVelocity;
    realArray & fLocal = f;
  #endif

    Index I1,I2,I3;
    Index I1g,I2g,I3g;

  getGhostIndex( c.extendedIndexRange(),side,axis,I1 ,I2 ,I3 ,0);     // boundary line
  getGhostIndex( c.extendedIndexRange(),side,axis,I1g,I2g,I3g,1);  // first ghost line


  if( parameters.gridIsMoving(grid) 
      && parameters.dbase.get<real >("advectionCoefficient")!=0. )  // this is needed by project for some reason? )
  {

    // -- get the grid acceleration from the MovingGrid's class ---

    const bool & useAddedMassAlgorithm = parameters.dbase.get<bool>("useAddedMassAlgorithm");
    if( useAddedMassAlgorithm )
    {
      // For the added-mass (beam) pressure BC, we scaled by rhos*As/rho 

      // mixedNormalCoeff(pc,side,axis,grid)=beamMassPerUnitLength[side][axis]/fluidDensity;
      const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
      RealArray & bcData = parameters.dbase.get<RealArray>("bcData");      
      const int & pc = parameters.dbase.get<int >("pc");
      if( mixedCoeff(pc,side,axis,grid)==1. )  // FIX ME 
      {
	printF("--INS-- gridAccelerationBC: t=%8.2e, scale pressure BC by rhos*As/rho =%8.2e grid=%i, (side,axis)=(%i,%i)\n",
               t0,mixedNormalCoeff(pc,side,axis,grid),grid,side,axis);
	f(I1g,I2g,I3g) *= mixedNormalCoeff(pc,side,axis,grid);
      }
      
    }
    
    // -- Note: For the added-mass case the "acceleration" of does not include the sigma*n term 
    MovingGrids & movingGrids = parameters.dbase.get<MovingGrids >("movingGrids");
    movingGrids.gridAccelerationBC(grid,side,axis,t0,c,u,f,gridVelocity,normal,I1,I2,I3,I1g,I2g,I3g);


  }

  // if( parameters.dbase.get< >("timeDependentBoundaryConditions") ) // ********** fix this *****
  if( parameters.bcIsTimeDependent(side,axis,grid) )
  {
    // add grid acceleration from time dependent boundary conditions (e.g. user defined)
    // if( getTimeDependentBoundaryConditions(t0,grid,side,axis,computeTimeDerivativeOfForcing) )
    if( getTimeDerivativeOfBoundaryValues( gf0, t0, grid,side,axis) )
    {
      // realArray & bd = gridVelocity; // ************************** fix this
      const int uc=parameters.dbase.get<int >("uc");
      const int vc=parameters.dbase.get<int >("vc");
      const int wc=parameters.dbase.get<int >("wc");
      // printf("gridAccelerationBC: add acceleration BC to grid=%i, side=%i, axis=%i\n",grid,side,axis);
      if( parameters.dbase.get<IntegerArray>("variableBoundaryData")(grid) )
      {
        RealArray & bd = parameters.getBoundaryData(side,axis,grid,c); 
	fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*bd(I1,I2,I3,uc)+
			      normal(I1,I2,I3,1)*bd(I1,I2,I3,vc));
	if( parameters.dbase.get<int >("numberOfDimensions")==3 )
	{
	  fLocal(I1g,I2g,I3g)-=normal(I1,I2,I3,2)*bd(I1,I2,I3,wc);
	}
      }
      else
      {
        const RealArray & bcData  = parameters.dbase.get<RealArray>("bcData");
	
	fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*bcData(uc,side,axis,grid)+
			      normal(I1,I2,I3,1)*bcData(vc,side,axis,grid));
	if( parameters.dbase.get<int >("numberOfDimensions")==3 )
	{
	  fLocal(I1g,I2g,I3g)-=normal(I1,I2,I3,2)*bcData(wc,side,axis,grid);
	}
      }
    }
  }
  
}
