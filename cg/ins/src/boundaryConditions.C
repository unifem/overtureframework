#include "Cgins.h"
#include "App.h"
#include "ParallelUtility.h"
#include "DeformingBodyMotion.h"
#include "BeamFluidInterfaceData.h"

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

  OV_GET_SERIAL_ARRAY(real,f,fLocal);
  OV_GET_SERIAL_ARRAY(real,gridVelocity,gridVelocityLocal);
#ifdef USE_PPP
  realSerialArray & normal = c.vertexBoundaryNormalArray(side,axis);
  // realSerialArray gridVelocityLocal; getLocalArrayWithGhostBoundaries(gridVelocity,gridVelocityLocal);
  // realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f,fLocal);
#else
  realSerialArray & normal = c.vertexBoundaryNormal(side,axis);
  // realSerialArray & gridVelocityLocal=gridVelocity;
  // realArray & fLocal = f;
#endif

  const int numberOfDimensions = c.numberOfDimensions();
  const int numberOfComponentGrids = gf0.cg.numberOfComponentGrids();
  const int & pc = parameters.dbase.get<int >("pc");

  Index I1,I2,I3;
  Index I1g,I2g,I3g;
  getGhostIndex( c.extendedIndexRange(),side,axis,I1 ,I2 ,I3 ,0);     // boundary line
  getGhostIndex( c.extendedIndexRange(),side,axis,I1g,I2g,I3g,1);  // first ghost line


  if( parameters.gridIsMoving(grid) 
      && parameters.dbase.get<real >("advectionCoefficient")!=0. )  // this is needed by project for some reason? )
    {

      // -- get the grid acceleration from the MovingGrid's class ---

      std::vector<BoundaryData> & boundaryDataArray =parameters.dbase.get<std::vector<BoundaryData> >("boundaryData");
      BoundaryData & bd = boundaryDataArray[grid];

      const bool & useAddedMassAlgorithm = parameters.dbase.get<bool>("useAddedMassAlgorithm");
      if( useAddedMassAlgorithm )
	{
	  // For the added-mass (beam) pressure BC, we scaled by rhos*As/rho 
	  const int (&deformingBodyNumber)[2][3] = bd.dbase.get<int[2][3]>("deformingBodyNumber");

	  // mixedNormalCoeff(pc,side,axis,grid)=beamMassPerUnitLength[side][axis]/fluidDensity;
	  const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
	  RealArray & bcData = parameters.dbase.get<RealArray>("bcData");      
	  if( deformingBodyNumber[side][axis]>=0 &&   // -- this is an interface --
	      mixedCoeff(pc,side,axis,grid)==1. )  
	    {
	      if( debug() & 4 && t0 <= dt )
		printF("--INS-- gridAccelerationBC: t=%8.2e, scale pressure BC by rhos*As/rho =%8.2e grid=%i, (side,axis)=(%i,%i:  mixedCoeff=%8.2e )\n",
		       t0,mixedNormalCoeff(pc,side,axis,grid),grid,side,axis, mixedCoeff(pc,side,axis,grid));
	      f(I1g,I2g,I3g) *= mixedNormalCoeff(pc,side,axis,grid);
	    }
      
	}
    
      // -- Get the grid acceleration term:
      MovingGrids & movingGrids = parameters.dbase.get<MovingGrids >("movingGrids");
      movingGrids.gridAccelerationBC(grid,side,axis,t0,c,u,f,gridVelocity,normal,I1,I2,I3,I1g,I2g,I3g);

      const int numberOfDeformingBodies= movingGrids.getNumberOfDeformingBodies();
      if( useAddedMassAlgorithm && numberOfDeformingBodies>0 )
	{


	  const bool & useApproximateAMPcondition = parameters.dbase.get<bool>("useApproximateAMPcondition");
	  if( useApproximateAMPcondition )
	    {
	      // ------------------------------------
	      // --- APPROXIMATE AMP PRESSURE BC ----
	      // ------------------------------------
	      // DeformingBodyMotion & deformingBody = movingGrids.getDeformingBody(body);
	      const int (&deformingBodyNumber)[2][3] = bd.dbase.get<int[2][3]>("deformingBodyNumber");

	      const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
	      RealArray & bcData = parameters.dbase.get<RealArray>("bcData");      
	      const int & pc = parameters.dbase.get<int >("pc");
	      if( deformingBodyNumber[side][axis]>=0 &&   // -- this is an interface --
		  mixedCoeff(pc,side,axis,grid)==1. )
		{
		  if( t0 < 5.*dt )
		    printF("--INS-- Approx. AMP BC: Add on traction term from AMP BC t=%9.3e\n",t0);

		  // Add on traction term  from AMP BC
		  // For two sided beams there are contributions from both sides
		  // f +=  [ nv^T( tau ) nv]_+^- 

		  // --- compute the forces on the surface ---
		  int ipar[] = {grid,side,axis,gf0.form}; // 
		  real rpar[] = { t0 }; // 
		  RealArray stressLocal(I1,I2,I3,numberOfDimensions);
		  parameters.getNormalForce( gf0.u,stressLocal,ipar,rpar );

		  const real fluidDensity = parameters.dbase.get<real>("fluidDensity")!=0. ? parameters.dbase.get<real>("fluidDensity") : 1.;
		  OV_GET_SERIAL_ARRAY(real,u,uLocal);

		  if( numberOfDimensions==2 )
		    {
		      // ADD viscous traction term (remove the pressure component)
		      // AMP:   p + (rhos*hs)/rho p.n = L   + n^T tau n    (plus sign)
		      if( t0 < 10.*dt  )
			{
			  RealArray nTaun(I1,I2,I3);
			  nTaun=(normal(I1,I2,I3,0)*stressLocal(I1,I2,I3,0) +  
				 normal(I1,I2,I3,1)*stressLocal(I1,I2,I3,1) ) 
			    -fluidDensity*uLocal(I1,I2,I3,pc);

			  printF("--INS-- Approx. AMP BC: (side,axis,grid)=(%i,%i,%i) t=%9.3e |n.taun|=%8.2e\n",side,axis,grid,t0,max(fabs(nTaun)));
			}
	  
		      fLocal(I1g,I2g,I3g) += (normal(I1,I2,I3,0)*stressLocal(I1,I2,I3,0) +  
					      normal(I1,I2,I3,1)*stressLocal(I1,I2,I3,1) ) 
			-fluidDensity*uLocal(I1,I2,I3,pc);

		      //Longfei 20160328: test adding the traction from the other side of the beam
		      // For two sided beams we need to adjust the opposite side 
		      if( true ) 
			{
			  // *NEW WAY*

			  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
			  int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];

			  MappedGrid & mg = gf0.cg[grid];
			  const IntegerArray & gid = mg.gridIndexRange();
			  for( int dir=0; dir<3; dir++ ){ iv[dir]=gid(0,dir); } //
			  const int axisp1= (axis +1) % numberOfDimensions;
	  
			  for( int body=0; body<numberOfDeformingBodies; body++ ) // *FIX* We know the body number from above ***
			    {
			      DeformingBodyMotion & deformingBody = movingGrids.getDeformingBody(body);
			      if( !deformingBody.beamModelHasFluidOnTwoSides() )
				{ // this is NOT a beam model with fluid on two sides.
				  continue;   
				}

			      DataBase & deformingBodyDataBase = deformingBody.deformingBodyDataBase;
			      const int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
			      const IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");

			      // --- beam-fluid interface data is stored here:
			      BeamFluidInterfaceData &  beamFluidInterfaceData = 
				deformingBodyDataBase.get<BeamFluidInterfaceData>("beamFluidInterfaceData");
			      IntegerArray *& donorInfoArray = beamFluidInterfaceData.dbase.get<IntegerArray*>("donorInfoArray");

			      is1=is2=is3=0;
			      isv[axis]=1-2*side;

			      for( int face=0; face<numberOfFaces; face++ )
				{
				  const int side0=boundaryFaces(0,face);
				  const int axis0=boundaryFaces(1,face);
				  const int grid0=boundaryFaces(2,face); 
				  if( grid0==grid && side0==side && axis0==axis )  // beam face is found
				    {
				      const IntegerArray & donorInfo= donorInfoArray[face]; 
				      Range I0=donorInfo.dimension(0);
				      for( int i=I0.getBase(); i<=I0.getBound(); i++ )  // NOTE: loop index i is incremented below
					{
					  // Here is the donor on the opposite face of the beam:
					  const int grid1 = donorInfo(i,0), side1=donorInfo(i,1), axis1=donorInfo(i,2);

					  if( grid1<0 )  // This means there is do opposite grid point -- could be the end of the beam
					    continue;

					  assert( grid1>=0 && grid1<numberOfComponentGrids );
		  
					  const realArray & u1 = gf0.u[grid1];
					  for( ; i<=I0.getBound(); i++ ) // NOTE: this increments "i" from the outer loop
					    {
					      if( donorInfo(i,0)!=grid1 )
						{
						  i--;
						  break;
						}
					      // closest grid pt on opposite side:
					      const int j1=donorInfo(i,3), j2=donorInfo(i,4), j3=donorInfo(i,5); 

					      iv[axisp1]=i+gid(0,axisp1); // index that varies along the interface of grid
					      int i1m=i1-is1, i2m=i2-is2, i3m=i3-is3; //  ghost point is (i1m,i2m,i3m)

					      f(i1m,i2m,i3m) += -(normal(j1,j2,j3,0)*stressLocal(j1,j2,j3,0) +  
								  normal(j1,j2,j3,1)*stressLocal(j1,j2,j3,1) ) 
						+fluidDensity*uLocal(j1,j2,j3,pc);
					    }
					}
				    }
				}
			    }
	  
			}




		    }
		  else
		    {
		      fLocal(I1g,I2g,I3g) += (normal(I1,I2,I3,0)*stressLocal(I1,I2,I3,0) +  
					      normal(I1,I2,I3,1)*stressLocal(I1,I2,I3,1) + 
					      normal(I1,I2,I3,2)*stressLocal(I1,I2,I3,2) ) 
			-fluidDensity*uLocal(I1,I2,I3,pc);
		    }
	
	
		}

	    }
	  else
	    {
	      // ------------------------------------
	      // --- ADJUSTED AMP PRESSURE BC ----
	      // ------------------------------------

	      // subtract off the current guess for sigma*n : *wdh* 2015/01/04 
	      const int (&deformingBodyNumber)[2][3] = bd.dbase.get<int[2][3]>("deformingBodyNumber");

	      // mixedNormalCoeff(pc,side,axis,grid)=beamMassPerUnitLength[side][axis]/fluidDensity;
	      const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
	      RealArray & bcData = parameters.dbase.get<RealArray>("bcData");      
	      const int & pc = parameters.dbase.get<int >("pc");
	      if( TRUE &&
		  deformingBodyNumber[side][axis]>=0 &&   // -- this is an interface --
		  mixedCoeff(pc,side,axis,grid)==1. )
		{
		  if( (true || debug() & 4) && t0 <= dt )
		    printF("--INS-- gridAccelerationBC: t=%8.2e, ADD p to grid acceleration RHS: grid=%i, (side,axis)=(%i,%i)\n",
			   t0,grid,side,axis);
		  if( (false ||  debug() & 4) && t0 <= dt )
		    {
		      ::display(u(I1,I2,I3,pc),sPrintF("--INS-- gridAccelerationBC: pressure p at t=%g (grid,side,axis)=(%i,%i,%i)",t0,grid,side,axis),"%9.2e ");
		      ::display(f(I1g,I2g,I3g),sPrintF("--INS-- gridAccelerationBC: f=gDot at t=%g (grid,side,axis)=(%i,%i,%i)",t0,grid,side,axis),"%9.2e "); 
		    }
	
		  f(I1g,I2g,I3g) += u(I1,I2,I3,pc) ; 

		  // For two sided beams we need to adjust the opposite side 
		  if( true ) 
		    {
		      // *NEW WAY*

		      int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
		      int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];

		      MappedGrid & mg = gf0.cg[grid];
		      const IntegerArray & gid = mg.gridIndexRange();
		      for( int dir=0; dir<3; dir++ ){ iv[dir]=gid(0,dir); } //
		      const int axisp1= (axis +1) % numberOfDimensions;
	  
		      for( int body=0; body<numberOfDeformingBodies; body++ ) // *FIX* We know the body number from above ***
			{
			  DeformingBodyMotion & deformingBody = movingGrids.getDeformingBody(body);
			  if( !deformingBody.beamModelHasFluidOnTwoSides() )
			    { // this is NOT a beam model with fluid on two sides.
			      continue;   
			    }

			  DataBase & deformingBodyDataBase = deformingBody.deformingBodyDataBase;
			  const int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
			  const IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");

			  // --- beam-fluid interface data is stored here:
			  BeamFluidInterfaceData &  beamFluidInterfaceData = 
			    deformingBodyDataBase.get<BeamFluidInterfaceData>("beamFluidInterfaceData");
			  IntegerArray *& donorInfoArray = beamFluidInterfaceData.dbase.get<IntegerArray*>("donorInfoArray");

			  is1=is2=is3=0;
			  isv[axis]=1-2*side;

			  for( int face=0; face<numberOfFaces; face++ )
			    {
			      const int side0=boundaryFaces(0,face);
			      const int axis0=boundaryFaces(1,face);
			      const int grid0=boundaryFaces(2,face); 
			      if( grid0==grid && side0==side && axis0==axis )  // beam face is found
				{
				  const IntegerArray & donorInfo= donorInfoArray[face]; 
				  Range I0=donorInfo.dimension(0);
				  for( int i=I0.getBase(); i<=I0.getBound(); i++ )  // NOTE: loop index i is incremented below
				    {
				      // Here is the donor on the opposite face of the beam:
				      const int grid1 = donorInfo(i,0), side1=donorInfo(i,1), axis1=donorInfo(i,2);

				      if( grid1<0 )  // This means there is do opposite grid point -- could be the end of the beam
					continue;

				      assert( grid1>=0 && grid1<numberOfComponentGrids );
		  
				      const realArray & u1 = gf0.u[grid1];
				      for( ; i<=I0.getBound(); i++ ) // NOTE: this increments "i" from the outer loop
					{
					  if( donorInfo(i,0)!=grid1 )
					    {
					      i--;
					      break;
					    }
					  // closest grid pt on opposite side:
					  const int j1=donorInfo(i,3), j2=donorInfo(i,4), j3=donorInfo(i,5); 

					  iv[axisp1]=i+gid(0,axisp1); // index that varies along the interface of grid
					  int i1m=i1-is1, i2m=i2-is2, i3m=i3-is3; //  ghost point is (i1m,i2m,i3m)

					  f(i1m,i2m,i3m) -= u1(j1,j2,j3,pc) ;
					}
				    }
				}
			    }
			}
	  
		    }
		  else
		    {
		      const int bodyNumber=deformingBodyNumber[side][axis];
		      for( int grid2=0; grid2<numberOfComponentGrids; grid2++ )
			{
			  if( grid2!=grid && parameters.gridIsMoving(grid2) )
			    {
			      // **FIX ME** this will only work for deforming bodies.
			      BoundaryData & bd2 = boundaryDataArray[grid2];
			      const int (&deformingBodyNumber2)[2][3] = bd2.dbase.get<int[2][3]>("deformingBodyNumber");
			      for( int dir2=0; dir2<numberOfDimensions; dir2++ )
				{
				  for( int side2=0; side2<=1; side2++ )
				    {
				      if( deformingBodyNumber2[side2][dir2]==bodyNumber )
					{
					  realMappedGridFunction & u2 = gf0.u[grid2];
					  // Here we assume that the gridlines match *FIX ME*
					  Index Jb1,Jb2,Jb3;
					  getGhostIndex( gf0.cg[grid2].extendedIndexRange(),side2,dir2,Jb1,Jb2,Jb3,0);     // boundary line
					  if( !( I1==Jb1 && I2==Jb2 && I3==Jb3 ) )
					    {
					      OV_ABORT("finish me");
					    }
		   
					  f(I1g,I2g,I3g) -= u2(Jb1,Jb2,Jb3,pc) ;

					  if( debug() & 4 || t0 <= dt )
					    printF("--INS-- gridAccelerationBC: t=%8.2e, SUBTRACT p on grid2=%i from grid acceleration RHS: grid=%i, (side,axis)=(%i,%i)\n",
						   t0,grid2,grid,side,axis);
              
					}
				    }
				}
			    }
			}
		    }
		}
      
	    } // end else adjusted AMP
      
	} // end if useAddedMass
    
    
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
