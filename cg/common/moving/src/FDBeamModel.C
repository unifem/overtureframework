#include "FDBeamModel.h"
#include "display.h"
#include "MappedGridOperators.h"
#include "TridiagonalSolver.h"


//static member
int FDBeamModel::
FDBeamCounter=0;



FDBeamModel::
FDBeamModel()
  :BeamModel()
{
  beamType = "FDBeamModel";
  FDBeamCounter++;
  printF("-- BM%i -- construct an %s\n",getBeamID(),beamType.c_str());

  
  //set some default parameters for FDBeamModel
  dbase.get<int>("numberOfGhostPoints")=2; // numOfGhost on each side
  dbase.get<bool>("isCubicHermiteFEM")=false;  // Hermite FEM needs x derivatives, this controls the type of solution structure

  dbase.put<int>("stencilSize")=5; 
  const int &stencilSize = dbase.get<int>("stencilSize");
  dbase.put<RealArray*>("coefficientI")  =new RealArray(stencilSize);  // holds finite difference coefficients of operator I at a point
  dbase.put<RealArray*>("coefficientK") =new RealArray(stencilSize);  // holds finite difference coefficients of operator K at a point
  dbase.put<RealArray*>("coefficientB") =new RealArray(stencilSize);  //holds finite difference coefficients of operator B at a point
  
  
  
  //we need an mappedGridOperator to evaluate derivatives for FDBeamModel
  dbase.put<MappedGridOperators*>("operator")=NULL;

}

FDBeamModel::
~FDBeamModel()
{

  delete dbase.get<RealArray*>("coefficientI");
  delete dbase.get<RealArray*>("coefficientK");
  delete dbase.get<RealArray*>("coefficientB");

  
  MappedGridOperators*& op = dbase.get<MappedGridOperators*>("operator");
  if(op!=NULL)
    delete op;
}



// Longfei 20160621: old
// =================================================================================================
/// /brief Assign the force on the beam
/// /param x0 (input) : array of (undeformed) locations on the beam surface
/// /param traction (input) : traction on the deformed surface
/// /param normal (input) : normal to the deformed surface 
/// /param Ib1,Ib2,Ib3 (input) : index of points on the boundary.
// =================================================================================================
// void FDBeamModel::
// addForce(const real & tf, const RealArray & x0, const RealArray & traction, const RealArray & normal,  
// 	 const Index & Ib1, const Index & Ib2,  const Index & Ib3 )
// {
//   const int & current = dbase.get<int>("current"); 
//   std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force
//   RealArray & fc = f[current];  // force at current time
  
//   // compute the surfaceForce load vector
//   setSurfaceForce(tf, x0, traction, normal, Ib1, Ib2, Ib3 );
  
//   RealArray & sf = dbase.get<RealArray>("surfaceForce"); // this is a load vector

//   //project the load vector to nodal values
//   const int & numElem = dbase.get<int>("numElem");
//   solveBlockTridiagonal(sf, sf, "massMatrixSolverNoBC" );
//   Index I1 = Range(0,numElem);
//   Index J1=2*I1;  
//   fc(I1,0,0,0) = sf(J1,0,0,0); // get the surfaceForce nodal values 


//   if( false )
//     {
//       const RealArray &time=dbase.get<RealArray>("time");
//       ::display(fc,sPrintF("-- BM -- addForce : fc after projected to nodal values, t=%9.3e\n",time(current)),"%9.2e ");
//     }
  

//   return;
  
// }

// Longfei 20160621: new addForce() to add the surface force to the current force
void FDBeamModel::
addForce( )
{
  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force
  RealArray & fc = f[current];  // force at current time
  
  RealArray & sf = dbase.get<RealArray>("surfaceForce"); // this is a load vector

  //project the load vector to nodal values
  const int & numElem = dbase.get<int>("numElem");
  solveBlockTridiagonal(sf, sf, "massMatrixSolverNoBC" );
  Index I1 = Range(0,numElem);
  Index J1=2*I1;  
  fc(I1,0,0,0) = sf(J1,0,0,0); // get the surfaceForce nodal values 


  if( false )
    {
      const RealArray &time=dbase.get<RealArray>("time");
      ::display(fc,sPrintF("-- BM -- addForce : fc after projected to nodal values, t=%9.3e\n",time(current)),"%9.2e ");
    }
  

  return;
  
}



// ======================================================================================
/// \brief initialize the beam model
/// the FDBeamModel version calls base version initialize + initialize mappedGridOperator
// ======================================================================================
void FDBeamModel::
initialize()
{
  BeamModel::initialize();

   
  MappedGrid &mg  = dbase.get<MappedGrid>("beamGrid");
  MappedGridOperators *&pOperator = dbase.get<MappedGridOperators*>("operator");

  // initialize operator
  if(pOperator==NULL)
    {
      pOperator = new MappedGridOperators;
      pOperator->updateToMatchGrid(mg); // associate operator to MappedGrid
      pOperator->setOrderOfAccuracy(2);  // second order for now
    }


  
  // get physical parameters
  const real & Abar = dbase.get<real>("massPerUnitLength");
  const real & K0 = dbase.get<real>("K0");
  const real & T = dbase.get<real>("tension");
  const real & EI = dbase.get<real>("EI");
  const real & Kt = dbase.get<real>("Kt");
  const real & Kxxt = dbase.get<real>("Kxxt");



  const TimeSteppingMethodEnum & predictorMethod = dbase.get<TimeSteppingMethodEnum>("predictorMethod");
  const TimeSteppingMethodEnum & correctorMethod = dbase.get<TimeSteppingMethodEnum>("correctorMethod");
  // initialize finite difference coefficients.
  // initialize the coefficient matrices for the implicit methods only
  if(predictorMethod==newmark2Implicit || correctorMethod==newmarkCorrector)
    {
      const int &stencilSize = dbase.get<int>("stencilSize");
      RealArray & coefficientI = *dbase.get<RealArray*>("coefficientI");
      RealArray & coefficientK = *dbase.get<RealArray*>("coefficientK");
      RealArray & coefficientB = *dbase.get<RealArray*>("coefficientB");

      const real & dx = dbase.get<real>("elementLength");
      real dx2=dx*dx;
      real dx4=dx2*dx2;


      const bool & useSameStencilSize = dbase.get<bool>("useSameStencilSize");
  

      // finite-difference coefficients:
      RealArray Dss2(stencilSize),Dss4(stencilSize), Dssss(stencilSize);

      //coefficientI=[0, 0, 1, 0, 0]
      coefficientI = 0.;
      coefficientI(2)=1.;

      //fourth order scheme for second derivative
      //Dss4=[-1/12, 4/3, -5/2, 4/3, -1/12]/dx^2
      Dss4=0.;
      Dss4(0)=-1./12.;
      Dss4(1)=4./3.;
      Dss4(2)=-5./2.;
      Dss4(3)=4./3.;
      Dss4(4)=-1./12.;
      Dss4 /= dx2;

      //second order schemes for second derivative
      //Dss2=[0, 1,-2, 1, 0] /dx^2
      Dss2=0.;
      Dss2(1)=1.;
      Dss2(2)=-2;
      Dss2(3)=1;
      Dss2 /= dx2;
 

      //Dssss=[1,-4, 6,-4, 1] /dx^4
      Dssss=0.;
      Dssss(0)=1.;
      Dssss(1)=-4.;
      Dssss(2)=6.;
      Dssss(3)=-4.;
      Dssss(4)=1.;
      Dssss /= dx4;
  
      if(useSameStencilSize)
	{
	  // scheme for the beam operator K:
	  coefficientK= K0*coefficientI-T*Dss4+EI*Dssss;  
	}
      else
	{
	  // same order:
	  // scheme for the beam operator K:
	  coefficientK= K0*coefficientI-T*Dss2+EI*Dssss;  
	}
	  // scheme for the damping operator B:
      coefficientB= Kt*coefficientI-Kxxt*Dss2; // this is the scheme described in the paper	
	
    }
}


// =======================================================================================
/// /brief  Compute the internal force in the beam, f = -B(v) -K(u); 
/// where K(u) = K0*u-T*uxx+EI*uxxxx,  B(v) = Kt*v-Kxxt*vxx
/// /param u (input) : position of the beam 
/// /param v (input) : velocity of the beam
/// /param f (output) :internal force [out]
// =======================================================================================
void FDBeamModel::
computeInternalForce(const RealArray& u, const RealArray& v, RealArray& f) 
{
  const real & K0 = dbase.get<real>("K0");
  const real & T = dbase.get<real>("tension");
  const real & EI = dbase.get<real>("EI");
  const real & Kt = dbase.get<real>("Kt");
  const real & Kxxt = dbase.get<real>("Kxxt");

  const int & numElem = dbase.get<int>("numElem");
  const int & numOfGhost = dbase.get<int>("numberOfGhostPoints");
  assert(numOfGhost==2); // Longfei: we have 2 ghost points for now, we might need more for higher order method

  const BoundaryConditionEnum * boundaryConditions = dbase.get<BoundaryConditionEnum[2]>("boundaryConditions");
  const BoundaryConditionEnum & bcLeft =  boundaryConditions[0];
  const BoundaryConditionEnum & bcRight =  boundaryConditions[1];

  MappedGrid & mg= dbase.get<MappedGrid>("beamGrid");
  MappedGridOperators &op = *dbase.get<MappedGridOperators*>("operator");
  Index I1,I2,I3;
  getIndex(mg.dimension(),I1,I2,I3);  // interior,boundary and ghost points

  RealArray uxx(I1,I2,I3,1), uxxxx(I1,I2,I3,1);
  op.derivative( MappedGridOperators::xxDerivative,u,uxx,I1,I2,I3,0); 
  op.derivative( MappedGridOperators::xxDerivative,uxx,uxxxx,I1,I2,I3,0);  // note that only values on interior and boundary points are valid


  const bool & useSameStencilSize = dbase.get<bool>("useSameStencilSize");
  if(useSameStencilSize)
    {
      // recompute uxx using 4th order (5 point) scheme
      op.setOrderOfAccuracy(4);
      op.derivative( MappedGridOperators::xxDerivative,u,uxx,I1,I2,I3,0);  // use 5 point stencil for uxx
      op.setOrderOfAccuracy(2); //reset
    }
  
  f = 0.0;
  f = -K0*u+T*uxx-EI*uxxxx;


  if( Kt!=0. || Kxxt!=0. )
    {
      // add damping terms to internal force
      // reuse uxx for vxx to save space
      op.derivative( MappedGridOperators::xxDerivative,v,uxx,I1,I2,I3,0); 
      f += -Kt*v+Kxxt*uxx;

    }
  
 const bool isPeriodic = bcLeft==periodic;
 if(isPeriodic)
   { 
     assert(bcRight==periodic);

     // -- assign values on the right boundary node
     u(numElem,0,0,0) = u(0,0,0,0);
	      
     // -- assign values on ghost points
     f(-1,0,0,0) = f(numElem-1,0,0,0);
     f(-2,0,0,0) = f(numElem-2,0,0,0);
     f(numElem+1,0,0,0) = f(1,0,0,0);
     f(numElem+2,0,0,0) = f(2,0,0,0);
   }
 
 // Longfei 20170106: try directly compute fx and use that to fill in ghost points
 if(bcRight==internalForceBC)
   {
     assert(bcLeft==internalForceBC);
     f(-2,0,0,0)=0.;
     f(numElem+2,0,0,0) =0;
   }


 
}



//================================================================================================
/// \brief Compute the acceleration of the beam. (Longfei 20160210: new)
///
/// \param u (input):               current beam position (For Newmark this is un + dt*vn + .5*dt^2*(1-2*beta)*an )
/// \param  (input)v:               current beam velocity (NOT USED CURRENTLY)
/// \param f (input):                external force on the beam
/// \param a (input):               beam acceleration [out]
/// \param linAcceleration (input): acceleration of the CoM of the beam (for free motion) [out]
/// \param omegadd (input):         angular acceleration of the beam (for free motion) [out]
/// \param dt (input):              time step
/// \param solverName (input) :  solverName can be "explicitSolver" or "implicitNewmarkSolver"
//
//================================================================================================
void FDBeamModel::
computeAcceleration(const real t,
		    const RealArray& u, const RealArray& v, 
		    const RealArray& f,
		    RealArray& a,
		    real linAcceleration[2],
		    real& omegadd,
		    real dt,
                    const aString & solverName )
{
  //const real & dx = dbase.get<real>("elementLength"); // seems dx is not needed here
  //const real & L = dbase.get<real>("length");
  //const real & T = dbase.get<real>("tension");
  //const real & Kxxt = dbase.get<real>("Kxxt");

  const int & numElem = dbase.get<int>("numElem");
  const BoundaryConditionEnum * boundaryConditions = dbase.get<BoundaryConditionEnum[2]>("boundaryConditions");
  const BoundaryConditionEnum & bcLeft =  boundaryConditions[0];
  const BoundaryConditionEnum & bcRight =  boundaryConditions[1];
  const bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");
  const real & Abar = dbase.get<real>("massPerUnitLength");
  
  // Longfei 20160809: these following parameters are needed by compatibility BC
  const real & EI = dbase.get<real>("EI");
  const real & K0 = dbase.get<real>("K0");
  const real & T = dbase.get<real>("tension");
  const real & Kt = dbase.get<real>("Kt");
  const real & Kxxt = dbase.get<real>("Kxxt");
  
  if( debug() & 2 )
    printF("-- BM%i -- BeamModel::computeAcceleration, t=%8.2e, dt=%8.2e\n",getBeamID(),t,dt);
  

  RealArray rhs=u;  //same size as u

  // Compute:   rhs = -B*v -K*u 
  computeInternalForce(u, v, rhs);


  if(debug() & 2 )
    {
      ::display(rhs,"-- BM -- computeAcceleration: rhs after computeInternalForce","%9.2e ");
      ::display(f,"-- BM -- computeAcceleration: f ","%9.2e ");
    }

  rhs += f;

  real accelerationScaleFactor=1.;
  if( solverName=="explicitSolver")
    {
      // explicitSolver do not invert anything
      // it  is solving for Abar utt (not utt )
      accelerationScaleFactor=Abar;
    }


  // apply bc for rhs (Longfei 20160810: for implicit solvers only)
  if( !allowsFreeMotion && solverName!="explicitSolver" )
    {
      // --- Apply boundary conditions to rhs  ----

      // --- If the boundary degrees of freedom are given  (e.g. w(0,t)=g0(t) or wx(0,t)=h0(t))
      //     then we eliminate the corresponding equation from the matrix equation (by setting it to the indentity)

      // Get two time derivatives of the boundary functions for "acceleration BC"
      RealArray gtt;
      getBoundaryValues( t, gtt, 2 );

      //Longfei 20160809: new this is needed by compatibility conditions
      RealArray g,gt,gttt,gtttt;
      getBoundaryValues( t, g,  0);
      getBoundaryValues( t, gt,  1);
      getBoundaryValues( t, gttt,  3);
      getBoundaryValues( t, gtttt, 4 );
      RealArray ftt;
      getBoundaryForces(t,ftt,2);


      // getBoundaryValues( t, g );
      
    
      for( int side=0; side<=1; side++ )
	{
	  BoundaryConditionEnum bc = side==0 ? bcLeft : bcRight;
	  const int ib = side==0 ? 0 : numElem;              // boundary node
	  const int is = 1-2*side;   // ib-is is the first ghost line, ib-2*is is the second ghost line
	  
	  
	  // Special case when EI=0 : (we only have 1 BC for clamped instead of 2)
	  if( bc==clamped && EI==0. ) bc=pinned;
	  
	  // real x = side==0 ? 0 : L;
	  if( bc == clamped ) 
	    {
	      // First two equations in the matrix are
	      //       w_tt = given
	      //       wx_tt = given 
	      if( false )
		{
		  printF("-- BM%i -- side=%i set clamped BC gtt=%e, gttx=%e, accelerationScaleFactor=%8.2e\n",
			 getBeamID(),gtt(0,side),gtt(1,side),accelerationScaleFactor);
		}
	
	      rhs(ib,0,0,0)=gtt(0,side)*accelerationScaleFactor;   // a=w_tt is given
	      rhs(ib-is,0,0,0)=gtt(1,side)*accelerationScaleFactor;   // ax is given, this determines the first ghost line
	      //rhs(ib-2*is,0,0,0) = 0;                                 // use extrapolation for the second ghost line, so rhs=0
	      //Longfei 20160809: use compatibility BC: EI*uttxxxx-T*uttxx = -rhos*bs*gtttt-K0*gtt -Kt*gttt+Kxxt*vttxx+ftt ignore the damping "Kxxt*vttxx" for now.. FIX ME
	      rhs(ib-2*is,0,0,0) =(-Abar*gtttt(0,side)-K0*gtt(0,side)-Kt*gttt(0,side)+ftt(side))*accelerationScaleFactor;
	    }
	  else if( bc==pinned )
	    {
	      //       w_tt = given
	      //       EI*wxx_tt = given 
	      rhs(ib,0,0,0)=gtt(0,side)*accelerationScaleFactor;   // a=w_tt is given
	      rhs(ib-is,0,0,0) = 0.;                // extrapolate the first ghost line
	      rhs(ib-2*is,0,0,0) = 0.;              // extrapolate the second ghost line
	      if(  EI != 0.) 
		{
		  //  E*I*w_xx = given => a_xx = given
		  if( debug() & 1 )	
		    printF("-- BM%i -- set rhs for pinned BC axx = gtt(2,side)=%8.2e, EI=%g\n",getBeamID(),gtt(2,side),EI);
		  rhs(ib-is,0,0,0) = gtt(2,side)*accelerationScaleFactor;   // axx is given, this determines the first ghost line
		  //Longfei 20160809: use compatibility BC:  uttxxxx = (-rhos*bs*gtttt-K0*gtt+T*gtt2-Kt*gttt+Kxxt*gttt2+ftt)/EI
		  rhs(ib-2*is,0,0,0) =((-Abar*gtttt(0,side)-K0*gtt(0,side)+T*gtt(2,side) -Kt*gttt(0,side)+Kxxt*gttt(2,side) +ftt(side))/EI)*accelerationScaleFactor;
		}
	    }
	  else if( bc==slideBC )
	    {
	      // ---- slide BC ---
	      //  wx_tt = given
	      //  EI*wxxx_tt=given 
	      rhs(ib-is,0,0,0)=gtt(1,side)*accelerationScaleFactor; // ax is given, this determines the first ghost line   
	      if(EI!=0)
		{
		  
		  rhs(ib-2*is,0,0,0 ) =  gtt(3,side)*accelerationScaleFactor;// axxx is given, this determines the second ghost line   
		}
	      else
		{
		 
		  rhs(ib-2*is,0,0,0) = 0.; // extrapolate the second ghost line
		}
	    }
	  else if( bc==freeBC )
	    {
	      // Free BC: EI*wxx=given,EI* w_xxx= given
    	      if(  EI==0. )
    		{
    		  printF("-- BM%i -- ERROR: A `free' BC is not allowed with the string model for a beam, EI=0\n",getBeamID());
    		  OV_ABORT("ERROR");
    		}

	      rhs(ib-is,0,0,0) =  gtt(2,side)*accelerationScaleFactor;         //  axx given, this determines the first ghost line
	      rhs(ib-2*is,0,0,0) = gtt(3,side)*accelerationScaleFactor;        //  axxx given, this determines the second ghost line

	    }
	  else if( bc==internalForceBC )
	    {
	      // do not need accelerationScaleFactor if computing internalForce
	      accelerationScaleFactor=1.;
	    }
      

      
	}

    }

  if( allowsFreeMotion ) 
    {
      OV_ABORT("Error: FDBeamModel does not support free motion yet....finish me");
    }
  // end if allows free motion
  

  if( debug() & 2 )
    {
      ::display(rhs,"-- BM -- computeAcceleration: rhs before solve Ma=rhs","%11.4e ");
    }

  // Solve M a = rhs, M is determined by  tridiagonalSolver.
  solveTridiagonal(rhs, a, solverName );
  if(solverName=="explicitSolver")  // M= I, bc for a is not implemented
    {
      a/=accelerationScaleFactor; // explicitSolver solves Abar*utt
      RealArray utemp=u,vtemp=v; // use the size of u,v
      assignBoundaryConditions(t,utemp,vtemp,a,f ); 	
    }


  if( debug() & 2 )
    {
      ::display(a,"-- BM -- computeAcceleration: solution a after solve","%11.4e ");
    }

  // solveBlockTridiagonal(A, rhs, a, bcLeft,bcRight,allowsFreeMotion);
  
}





// ================================================================================
/// \brief Solve A u = f, A is associated with the solver
/// \param f (input): rhs of the system
/// \param u (output): solution
/// \param tridiagonalSolverName (input) : name of the tridiagonal solver
//
// ================================================================================
void FDBeamModel::
solveTridiagonal(const RealArray& f, RealArray& u, const aString & tridiagonalSolverName )
{

  const real & EI = dbase.get<real>("EI");
  const int & numElem = dbase.get<int>("numElem");
  const int & numOfGhost = dbase.get<int>("numberOfGhostPoints");

  const BoundaryConditionEnum * boundaryConditions = dbase.get<BoundaryConditionEnum[2]>("boundaryConditions");
  const BoundaryConditionEnum & bcLeft = boundaryConditions[0];
  const BoundaryConditionEnum & bcRight = boundaryConditions[1];
  const bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");

  const bool isPeriodic = bcLeft==periodic;


  if(tridiagonalSolverName=="implicitNewmarkSolver")
    {
      factorTridiagonalSolver(tridiagonalSolverName);  // factor the solver if refactor==true	    
      // TridiagonalSolver *& pTri = dbase.get<TridiagonalSolver*>("tridiagonalSolver");
      TridiagonalSolver *& pTri = dbase.get<TridiagonalSolver*>(tridiagonalSolverName);
      assert( pTri!=NULL );
      
      TridiagonalSolver & tri = *pTri;
      
      // -- rhs --
      Index I1,I2,I3;   
      if( isPeriodic )
	I1=Range(0,numElem-1);
      else
	I1=Range(-numOfGhost,numElem+numOfGhost);
      I2=0,I3=0;
      
      
      RealArray xTri(I1,I2,I3);
      xTri = f(I1,I2,I3,0); 
      // solve the block tridiagonal system: 
      tri.solve(xTri);
      // assign the solution 
      u(I1,I2,I3,0) = xTri;
      
    }
  else if(tridiagonalSolverName=="explicitSolver")
    {
      //mass matrix is identity for FDBeamModel.

      //solve Au=f, where A=I, so no need to invert a matrix
      u = f;
    }
  else
    {
      printF("unkown solverName \"%s\" for FDBeamModel::solveTridiagonal()",(const char*)tridiagonalSolverName);
      OV_ABORT("--BM-- Error!!!")
    }
  

  if( isPeriodic )
    { 
      // -- assign values on the right boundary node
      u(numElem,0,0,0) = u(0,0,0,0);
	      
      // -- assign values on ghost points
      u(-1,0,0,0) = u(numElem-1,0,0,0);
      u(-2,0,0,0) = u(numElem-2,0,0,0);
      u(numElem+1,0,0,0) = u(1,0,0,0);
      u(numElem+2,0,0,0) = u(2,0,0,0);
    }


}






//Longfei 20160216: new function that factors the tridiangonal solvers
//================================================================================
///\brief factor the tridiangnal solver 
/// \param tridiagonalSolverName (input) : name of the tridiagonal solver
// output: factor the solver with matrix 
// if(tridiagonalSolverName==implicitNewmarkSolver)  A =  rhs*bs*I+alpha*K+alphB*B
//================================================================================
int FDBeamModel::
factorTridiagonalSolver( const aString & tridiagonalSolverName)
{

  TridiagonalSolver *& pTri = dbase.get<TridiagonalSolver*>(tridiagonalSolverName);
  bool & refactor = dbase.get<bool>("refactor");
  if( pTri==NULL )
    {
      pTri = new TridiagonalSolver();
      refactor=true;
      printF("-- BM%i -- construct TridiagonalSolver=[%s]\n",getBeamID(),(const char*)tridiagonalSolverName);
    }
  if(!refactor) return 0;

  assert( pTri!=NULL );
  TridiagonalSolver & tri = *pTri;


  const BoundaryConditionEnum * boundaryConditions = dbase.get<BoundaryConditionEnum[2]>("boundaryConditions");
  const BoundaryConditionEnum & bcLeft =  boundaryConditions[0];
  const BoundaryConditionEnum & bcRight =  boundaryConditions[1];
 
  const bool isPeriodic = bcLeft==periodic;
  const bool addDampingMatrix = dbase.get<real>("Kt")!=0. || dbase.get<real>("Kxxt")!=0.;
  const int & numElem = dbase.get<int>("numElem");
  const int & numOfGhost = dbase.get<int>("numberOfGhostPoints");
  const real & dx = dbase.get<real>("elementLength");
  const bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");


  const real & Abar = dbase.get<real>("massPerUnitLength");
  const real & EI = dbase.get<real>("EI");
  const real & T = dbase.get<real>("tension"); // Longfei 20160809: T is needed for compatibility condition

 
  if( isPeriodic ) 
    { // consistency check:
      assert( bcRight==periodic );
    }

  RealArray A;
  if(tridiagonalSolverName=="implicitNewmarkSolver") // solve (Abar*I+alpha*K+alphB*B) a = f
    {
      const RealArray & coefficientI = *dbase.get<RealArray*>("coefficientI");
      const RealArray & coefficientK = *dbase.get<RealArray*>("coefficientK");
      const RealArray & coefficientB = *dbase.get<RealArray*>("coefficientB");
      const real & newmarkBeta = dbase.get<real>("newmarkBeta");
      const real & newmarkGamma = dbase.get<real>("newmarkGamma");
      const real & dt = dbase.get<real>("dt");
      
      
      real alpha =newmarkBeta*dt*dt;  // coeff of K in A
      real alphaB=newmarkGamma*dt;    // coeff of B in A
      
      A = Abar*coefficientI+alpha*coefficientK;

      if( addDampingMatrix )
	{ // add damping matrix B 
	  A += alphaB*coefficientB;
	}

      if(false)
	{
	  ::display(A,"A","%9.2e");
	  ::display(coefficientI,"I","%10.2e");
	  ::display(coefficientK,"K","%10.2e");
	  ::display(coefficientB,"B","%10.2e");
	  //OV_ABORT("TEST");
	}
      
    }
  else
    {
      printF("-- BM%i --  tridiagonalSolverName=[%s]\n",getBeamID(),(const char*)tridiagonalSolverName);
      OV_ABORT("Error: only implicitNewmarkSolver needs to use a Tridiagonal solver for FDBeamModel");
    }
 
  Index I1,I2,I3;   
  if( isPeriodic )
    I1=Range(0,numElem-1);
  else
    I1=Range(-numOfGhost,numElem+numOfGhost);

  I2=0,I3=0;

  //pentadiagonal vectors: 
  RealArray at(I1,I2,I3), bt(I1,I2,I3), ct(I1,I2,I3), dt(I1,I2,I3), et(I1,I2,I3);    
  at = A(0);
  bt = A(1);
  ct = A(2);
  dt = A(3);
  et = A(4);
  
  // TridiagonalSolver::periodic
  // due to some boundary conditions or extrapolations, we use extended systemType
  const TridiagonalSolver::SystemType systemType = isPeriodic ? TridiagonalSolver::periodic : TridiagonalSolver::extended;
  
  

  if( true || debug() & 1 )
    printF("-- BM%i -- solveBlockTridiagonal : name=[%s] form block tridiagonal system and factor, isPeriodic=%i\n",
	   getBeamID(),(const char*)tridiagonalSolverName, (int)isPeriodic);
  
      
  // Longfei 20160628:
  // note this is the matrix for acceleration equations.
  const bool & useSameStencilSize = dbase.get<bool>("useSameStencilSize");
  real delta = useSameStencilSize?1.:0.;
  if( !allowsFreeMotion && !isPeriodic )  // skip this for periodic bc
    {
      // --- Boundary conditions ---
      // Adjust the matrix for essential BC's -- these will set the DOF's at boundaries
      for( int side=0; side<=1; side++ )
    	{
    	  BoundaryConditionEnum bc = side==0 ? bcLeft : bcRight;
    	  const int ib = side==0 ? 0 : numElem;    // boundary node
	  const int is = 1-2*side;        // ib-is is the first ghost line, ib-2*is is the second ghost line

    	  // Special case when EI=0 : (we only have 1 BC for clamped instead of 2)
    	  // const real EI = elasticModulus*areaMomentOfInertia;
    	  if( bc==clamped && EI==0. ) bc=pinned;

	  // fix up eqn on the boundary nodes
	  // w_tt = given
	  if(bc == clamped || bc == pinned)
	    {
	      // Replace eqn on boundary with  identity
	      at(ib,0,0)=0.;
	      bt(ib,0,0)=0.;
	      ct(ib,0,0)=1.;
	      dt(ib,0,0)=0.;
	      et(ib,0,0)=0.; 
	    }
	  
	  // fix up eqn on the first ghost line
	  // wx_tt = given
	  if(bc == clamped || bc == slideBC)
	    {
	      // Replace eqn on first ghost line with  wx_tt = given
	      if(side==0)
		{
		  bt(ib-is,0,0)=(delta/12.)/dx;
		  ct(ib-is,0,0)=-(.5+delta/6.)/dx;
		  dt(ib-is,0,0)=0.;
		  et(ib-is,0,0)=(.5+delta/6.)/dx;
		  at(ib-is,0,0)=-(delta/12.)/dx;
		 
		}
	      else
		{
		  et(ib-is,0,0)=(delta/12.)/dx;
		  at(ib-is,0,0)=-(.5+delta/6.)/dx;
		  bt(ib-is,0,0)=0.;
		  ct(ib-is,0,0)=(.5+delta/6.)/dx;
		  dt(ib-is,0,0)=-(delta/12.)/dx;
		    
		}
	    }
	  // EI*wxx_tt = given
	  if(bc==pinned || bc==freeBC)
	    {
	      if(EI!=0)
		{
		  real dx2=dx*dx;
		  // Replace eqn on first ghost line with  wxx_tt = given
		  if(side==0)
		    {
		      // 2nd order
		      bt(ib-is,0,0)=-(delta/12.)/dx2;
		      ct(ib-is,0,0)=(1.+delta/3.)/dx2;
		      dt(ib-is,0,0)=-(2.+delta/2.)/dx2;
		      et(ib-is,0,0)=(1.+delta/3.)/dx2;
		      at(ib-is,0,0)=-(delta/12.)/dx2;
		    }
		  else 
		    {
		      // 2nd order
		      et(ib-is,0,0)=-(delta/12.)/dx2;
		      at(ib-is,0,0)=(1.+delta/3.)/dx2;
		      bt(ib-is,0,0)=-(2.+delta/2.)/dx2;
		      ct(ib-is,0,0)=(1.+delta/3.)/dx2;
		      dt(ib-is,0,0)=-(delta/12.)/dx2;
		    }
		}
	      else
		{
		  // EI = 0, use extrapolation to fill the first ghost line
		  // Replace eqn on first ghost line with extrapolation
		  modifyMatrixForExtrapolation(at,bt,ct,dt,et,ib-is,side);
		  
		}
	    }

	  // fix up eqn on the second ghost line
	  // EI*wxxx_tt = given
	  if(bc==freeBC || bc== slideBC)
	    {
	      real dx3=dx*dx*dx;
	      // Replace eqn on second ghost line with  wxxx_tt = given
	      if(side==0)
		{
		  ct(ib-2*is,0,0)=-.5/dx3;
		  dt(ib-2*is,0,0)=1./dx3;
		  et(ib-2*is,0,0)=0.;
		  at(ib-2*is,0,0)=-1./dx3;
		  bt(ib-2*is,0,0)=.5/dx3;
		}
	      else
		{
		  dt(ib-2*is,0,0)=-.5/dx3;
		  et(ib-2*is,0,0)=1./dx3;
		  at(ib-2*is,0,0)=0.;
		  bt(ib-2*is,0,0)=-1./dx3;
		  ct(ib-2*is,0,0)=.5/dx3;
		}
	    }
	  else if(bc==pinned )
	    {
	      // Longfei 20160809:
	      if(EI!=0)
		{
		  //compactbility BC:  EI*uxxxx = -rhos*bs*gtt-K0*g+T*g2-Kt*gt+Kxxt*gt2+f		  
		  // Replace eqn on second ghost line with  wxxxx_tt = 0
		  real dx4=dx*dx*dx*dx;
		  if(side==0)
		    {
		      ct(ib-2*is,0,0)=1./dx4;
		      dt(ib-2*is,0,0)=-4./dx4;
		      et(ib-2*is,0,0)=6./dx4;
		      at(ib-2*is,0,0)=-4./dx4;
		      bt(ib-2*is,0,0)=1./dx4;
		    }
		  else
		    {
		      dt(ib-2*is,0,0)=1./dx4;
		      et(ib-2*is,0,0)=-4./dx4;
		      at(ib-2*is,0,0)=6./dx4;
		      bt(ib-2*is,0,0)=-4./dx4;
		      ct(ib-2*is,0,0)=1./dx4;
		    }
		}
	      else
		{
		  //fill in second ghost with extrapolation. This value will not be used
		  modifyMatrixForExtrapolation(at,bt,ct,dt,et,ib-2*is,side);
		}
	    }
	  else if(bc==clamped )
	    {
	      // Longfei 20160809:
	      if(EI!=0)
		{
		  //compactbility BC:
		  // EI*uxxxx-T*uxx = -rhos*bs*gtt-K0*g-Kt*gt+Kxxt*vxx+f
		  // Replace eqn on second ghost line with  wxxxx_tt = 0
		  real dx4=dx*dx*dx*dx, dx2=dx*dx;
		  if(side==0)
		    {
		      ct(ib-2*is,0,0)=EI*1./dx4+T*(delta/12.)/dx2;
		      dt(ib-2*is,0,0)=-EI*4./dx4-T*(1.+delta/3.)/dx2;
		      et(ib-2*is,0,0)=EI*6./dx4+T*(2.+delta/2.)/dx2;
		      at(ib-2*is,0,0)=-EI*4./dx4-T*(1.+delta/3.)/dx2;
		      bt(ib-2*is,0,0)=EI*1./dx4+T*(delta/12.)/dx2;
		    }
		  else
		    {
		      dt(ib-2*is,0,0)=EI*1./dx4+T*(delta/12.)/dx2;
		      et(ib-2*is,0,0)=-EI*4./dx4-T*(1.+delta/3.)/dx2;
		      at(ib-2*is,0,0)=EI*6./dx4+T*(2.+delta/2.)/dx2;
		      bt(ib-2*is,0,0)=-EI*4./dx4-T*(1.+delta/3.)/dx2;
		      ct(ib-2*is,0,0)=EI*1./dx4+T*(delta/12.)/dx2;
		    }
		  
		}
	      else
		{
		  //fill in second ghost with extrapolation. This value will not be used
		  modifyMatrixForExtrapolation(at,bt,ct,dt,et,ib-2*is,side);
		}
	    }	  
	  else
	    {
	      OV_ABORT("Error: wrong BC conditions");
	    }

   

    	}
    }

  if(debug() &2)
    {
      printF("Pentadiagonal matrix: a,b,c,d,e\n");
      ::display(at,"a","%10.2e");
      ::display(bt,"b","%10.2e");
      ::display(ct,"c","%10.2e");
      ::display(dt,"d","%10.2e");
      ::display(et,"e","%10.2e");
    }

  // Factor the tridiagonal system:
  tri.factor(at,bt,ct,dt,et,systemType,axis1);
 
  refactor=false;

}


//================================================================================================
/// \brief modify the tridiagonal(pentadiag) matrix for extrapolation
///
/// \param at,...,et (input/output): diag vectors of a penta diag matrix
/// \param ie: equation number
/// \param side: indicates the direction of extrapolation.
///                    0: extrapolation use info from the right side
///                    1: extrapolation use info from the left side
//
//================================================================================================
void FDBeamModel::
modifyMatrixForExtrapolation(RealArray & at,RealArray & bt,RealArray & ct,RealArray & dt,RealArray &et, int ie, int side)
{
  
  if(side==0)
    {
      at(ie,0,0)=-1.;
      bt(ie,0,0)=0.;
      ct(ie,0,0)=1.;
      dt(ie,0,0)=-3.;
      et(ie,0,0)=3.;
    }
  else if(side==1)
    {
      at(ie,0,0)=3.;
      bt(ie,0,0)=-3.;
      ct(ie,0,0)=1.;
      dt(ie,0,0)=0.;
      et(ie,0,0)=-1.;
    }
  else
    {
      OV_ABORT("Error: side==0 or 1");
    }
}





//  =========================================================================================
/// \brief Assign boundary conditions
/// forcing is passed in for compatibility boundary condition
//  =========================================================================================
int FDBeamModel::
assignBoundaryConditions( real t, RealArray & u, RealArray & v, RealArray & a,const RealArray & f )
{  
  const real & L = dbase.get<real>("length");
  const int & numElem = dbase.get<int>("numElem");
  const BoundaryConditionEnum * boundaryConditions = dbase.get<BoundaryConditionEnum[2]>("boundaryConditions");
  const BoundaryConditionEnum & bcLeft =  boundaryConditions[0];
  const BoundaryConditionEnum & bcRight =  boundaryConditions[1];
  const bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");
  const bool & useExactSolution =  dbase.get<bool>("useExactSolution");
  const real & dx = dbase.get<real>("elementLength");
  const real & dx2= dx*dx;
  const real & dx3= dx2*dx;
  const real & dx4= dx3*dx;

  // physical parameters
  const real & Abar = dbase.get<real>("massPerUnitLength");
  const real & K0 = dbase.get<real>("K0");
  const real & T = dbase.get<real>("tension");
  const real & EI = dbase.get<real>("EI");
  const real & Kt = dbase.get<real>("Kt");
  const real & Kxxt = dbase.get<real>("Kxxt");

  const bool & useSameStencilSize = dbase.get<bool>("useSameStencilSize");



  if( !allowsFreeMotion )
    {
      RealArray g,gt,gtt;
      getBoundaryValues( t, g,  0 );
      getBoundaryValues( t, gt, 1 );
      getBoundaryValues( t, gtt,2 );

      for( int side=0; side<=1; side++ )
	{
	  BoundaryConditionEnum bc = side==0 ? bcLeft : bcRight;
    	  const int ib = side==0 ? 0 : numElem;    // boundary node
	  const int is = 1-2*side;        // ib-is is the first ghost line, ib-2*is is the second ghost line

	  	  
	  // Special case when EI=0 : (we only have 1 BC for clamped instead of 2)
	  if( bc==clamped && EI==0. ) bc=pinned;
	  
	  if( bc == clamped )
	    {//old:
	      //       w = given
	      //       wx = given 
	      u(ib,0,0,0) =  g(0,side);          // u is given
	      v(ib,0,0,0) =  gt(0,side);         // v is given
	      a(ib,0,0,0) =  gtt(0,side);        // a is given

	      if(useSameStencilSize)
		{
		  // same stencil for all
		  //-----ghost points for u --------------------
		  // use u.x=given and compatibility BC to determine the first and second ghost line for u
		  // D1*u = g1, where D1 = D0(I-h^2/6(D+D-))
		  // EI*D4*u-T*D2*u = -rhos*bs*gtt-K0*g-Kt*gt+Kxxt*(D+D-)v+f, where D4=(D+D-)^2, D2=(D+D-) - h^2/12(D+D-)^2
		  real rhs1 = -Abar*gtt(0,side)-K0*g(0,side)-Kt*gt(0,side)+Kxxt*(v(ib-is,0,0,0)-2.*v(ib,0,0,0)+v(ib+is,0,0,0))/dx2 + f(ib,0,0,0);
		  real rhs2 = 12.*is*dx*g(1,side);
		  //=====================================================================================================
		  //              EI*D4= [     EI*1./dx4       -EI*4./dx4        EI*6./dx4        -EI*4./dx4           EI/dx4]
		  //               T*D2= [ −T/(12.*dx2)      4.*T/(3.*dx2)   −5.*T/(2.*dx2)   4.*T/(3.*dx2)	 −T/(12.*dx2)]
		  //  =====> define
		  real am2=EI*1./dx4+T/(12.*dx2);
		  real am1= -EI*4./dx4- 4.*T/(3.*dx2);
		  real a0 =  EI*6./dx4+5.*T/(2.*dx2);
		  real ap1 = -EI*4./dx4-4.*T/(3.*dx2);
		  real ap2 = EI/dx4+T/(12.*dx2);
		  // ======> 
		  //  (EI*D4- T*D2)u=[ am2, am1, a0, ap1, ap2 ]*u=rhs1   --------- (1)
		  //                  D1u=[     1,    -8,  0,     8,    -1 ]*u=rhs2   -------- (2)
		  //=====================================================================================================
		  // solve eqn (1) and (2) for 
		  u(ib-is,0,0,0) = (rhs1-am2*rhs2-a0*u(ib,0,0,0)-(ap1-8*am2)*u(ib+is,0,0,0)-(ap2+am2)*u(ib+2*is,0,0,0))/(am1+8.*am2);
		  u(ib-2*is,0,0,0) = 8.*u(ib-is,0,0,0)-8.*u(ib+is,0,0,0)+ u(ib+2*is,0,0,0)+rhs2;

		  //-----ghost points for v --------------------
		  rhs1 = 0.;
		  rhs2 = 12.*is*dx*gt(1,side);
		  //=====================================================================================================
		  //           D+^3 v =[1,-3, 3,-1,0 ]v =rhs1;
		  //               D1v =[1,-8, 0, 8,-1]v =rhs2   
		  v(ib-is,0,0,0)=(rhs1-rhs2-3.*v(ib,0,0,0)+9.*v(ib+is,0,0,0)-v(ib+2*is,0,0,0))/5.;
		  v(ib-2*is,0,0,0)= 8.*v(ib-is,0,0,0)-8.*v(ib+is,0,0,0)+ v(ib+2*is,0,0,0)+rhs2;	
	      
		  //-----ghost points for a --------------------
		  rhs1 = 0.;
		  rhs2 = 12.*is*dx*gtt(1,side);
		  a(ib-is,0,0,0)=(rhs1-rhs2-3.*a(ib,0,0,0)+9.*a(ib+is,0,0,0)-a(ib+2*is,0,0,0))/5.;
		  a(ib-2*is,0,0,0)= 8.*a(ib-is,0,0,0)-8.*a(ib+is,0,0,0)+ a(ib+2*is,0,0,0)+rhs2;	
		}
	      else
		{
		  // 2nd order
		  u(ib-is,0,0,0) = u(ib+is)-is*2.*dx*g(1,side);   // ux is given, this determines the first ghost line
		  v(ib-is,0,0,0) = v(ib+is)-is*2.*dx*gt(1,side);  // vx is given, this determines the first ghost line
		  a(ib-is,0,0,0) = a(ib+is)-is*2.*dx*gtt(1,side); // ax is given, this determines the first ghost line

		  // use compatiblitiy condition for the second ghost line or u
		  real uxxxx = -Abar*gtt(0,side)-K0*g(0,side)+T*(u(ib-is,0,0,0)-2.*u(ib,0,0,0)+u(ib+is,0,0,0))/dx2-Kt*gt(0,side)+Kxxt*(v(ib-is,0,0,0)-2.*v(ib,0,0,0)+v(ib+is,0,0,0))/dx2 + f(ib,0,0,0);
		  u(ib-2*is) = 4.*u(ib-is)-6.*u(ib)+4.*u(ib+is)-u(ib+2*is) + uxxxx*dx4;
	  
		  // extrapolate the second ghost line for v and a. These values will not be used by the scheme
		  v(ib-2*is,0,0,0)=3.*v(ib-is,0,0,0)-3.*v(ib,0,0,0)+v(ib+is,0,0,0); 
		  a(ib-2*is,0,0,0)=3.*a(ib-is,0,0,0)-3.*a(ib,0,0,0)+a(ib+is,0,0,0);
		}
	    }
	 else if(bc==pinned)
	    {
	      // w=given
	      // EI*wxx given
	      u(ib,0,0,0) =g(0,side);
	      v(ib,0,0,0) =gt(0,side); 
	      a(ib,0,0,0) =gtt(0,side);
	      if(  EI != 0.) 
		{
		  //Longfei 20160303: we could do better for the ghost of v and a. But it is fine to do this since no forth derivatives need to be evalutated for v and a
		  v(ib-is,0,0,0)=2.*v(ib,0,0,0)-v(ib+is,0,0,0)+dx2*gt(2,side);   // vxx is given, this determines the first ghost line
		  a(ib-is,0,0,0)=2.*a(ib,0,0,0)-a(ib+is,0,0,0)+dx2*gtt(2,side);   // axx is given, this determines the first ghost line   

		  // Get ghost points for u:
		  //compactbility BC:  EI*uxxxx = -rhos*bs*gtt-K0*g+T*g2-Kt*gt+Kxxt*gt2+f
		  real uxxxx;
		  uxxxx = (-Abar*gtt(0,side)-K0*g(0,side)+T*g(2,side) -Kt*gt(0,side)+Kxxt*gt(2,side) + f(ib,0,0,0))/EI;
		  if(useSameStencilSize)
		    {
		      // use higher order u.xx and compatibility BC to determine the first and second ghost line for u
		      //get the first ghost line for u using D+D-u = g2+h^2/12*uxxxx
		      u(ib-is,0,0,0)=2.*u(ib,0,0,0)-u(ib+is,0,0,0)+dx2*(g(2,side)+dx2*uxxxx/12);   
		    }
		  else
		    {
		      // use 2nd order scheme for D+D- u=g2
		      u(ib-is,0,0,0)=2.*u(ib,0,0,0)-u(ib+is,0,0,0)+dx2*g(2,side);         // uxx is given, this determines the first ghost lin. This produce wiggles in a
		    }
		  // get the second ghost line for u from compatibility condition
		  u(ib-2*is) = 4.*u(ib-is)-6.*u(ib)+4.*u(ib+is)-u(ib+2*is) + uxxxx*dx4;
		}
	      else
		{
		  // extrapolate the first ghost line
		  u(ib-is,0,0,0)=3.*u(ib,0,0,0)-3.*u(ib+is,0,0,0)+u(ib+2*is,0,0,0);
		  v(ib-is,0,0,0)=3.*v(ib,0,0,0)-3.*v(ib+is,0,0,0)+v(ib+2*is,0,0,0);
		  a(ib-is,0,0,0)=3.*a(ib,0,0,0)-3.*a(ib+is,0,0,0)+a(ib+2*is,0,0,0);

		  // extrapolate the second ghost line for u
		  u(ib-2*is,0,0,0)=3.*u(ib-is,0,0,0)-3.*u(ib,0,0,0)+u(ib+is,0,0,0);		  
		}

	      // extrapolate the second ghost line for v and a. These values will not be used by the scheme
	      v(ib-2*is,0,0,0)=3.*v(ib-is,0,0,0)-3.*v(ib,0,0,0)+v(ib+is,0,0,0); 
	      a(ib-2*is,0,0,0)=3.*a(ib-is,0,0,0)-3.*a(ib,0,0,0)+a(ib+is,0,0,0);
	    }
	 else if( bc==slideBC )
	   {
	     // ---- slide BC ---
	     //  wx = given
	     //  EI*wxxx=given 


	     u(ib-is,0,0,0) = u(ib+is,0,0,0)-is*2.*dx*g(1,side);  // ux is given, this determines the first ghost line
	     v(ib-is,0,0,0) = v(ib+is,0,0,0)-is*2.*dx*gt(1,side);  // vx is given, this determines the first ghost line
	     a(ib-is,0,0,0) = a(ib+is,0,0,0)-is*2.*dx*gtt(1,side);  // ax is given, this determines the first ghost line

	     if(EI!=0)
	       {
		 if(useSameStencilSize)
		   {

		     // use 5 point stencil for w.x = D0(I-h^2/6(D+D-))*u = D0*u-h^2/6D0*((D+D-))u=g1     
		     // sinde D0*((D+D-))u = g3, we have u(ib-is,0,0,0) = u(ib+is,0,0,0)-is*2*dx*(g(1,side)+h^2/6*g(3,side));
		     // so correct u(ib-is) with -is*2*dx*dx2/6*g(3,side)
		     // add higher order correcto to u(ib-is)
		     u(ib-is,0,0,0) +=   -is*2.*dx*dx2/6.*g(3,side);
		     //  add higher order correcto to v(ib-is),a(ib-is) as well
		     v(ib-is,0,0,0) +=   -is*2.*dx*dx2/6.*gt(3,side);
		     a(ib-is,0,0,0) +=   -is*2.*dx*dx2/6.*gtt(3,side);
		   
		   }
		 // uxxx =D0(D+D-)u is given, this determines the second ghost line   
		 u(ib-2*is,0,0,0)=2.*u(ib-is,0,0,0)-2.*u(ib+is,0,0,0)+u(ib+2*is,0,0,0)-is*2.*dx3*g(3,side);
		 v(ib-2*is,0,0,0)=2.*v(ib-is,0,0,0)-2.*v(ib+is,0,0,0)+v(ib+2*is,0,0,0)-is*2.*dx3*gt(3,side);
		 a(ib-2*is,0,0,0)=2.*a(ib-is,0,0,0)-2.*a(ib+is,0,0,0)+a(ib+2*is,0,0,0)-is*2.*dx3*gtt(3,side);
		 
	       }
	     else
	       {
		 // extrapolate the second ghost line.
		 u(ib-2*is,0,0,0)=3.*u(ib-is,0,0,0)-3.*u(ib,0,0,0)+u(ib+is,0,0,0); 
		 v(ib-2*is,0,0,0)=3.*v(ib-is,0,0,0)-3.*v(ib,0,0,0)+v(ib+is,0,0,0); 
		 a(ib-2*is,0,0,0)=3.*a(ib-is,0,0,0)-3.*a(ib,0,0,0)+a(ib+is,0,0,0);
	       }
	   }
	 else if( bc==freeBC )
	   {
	     // Free BC: EI*wxx=given,EI* w_xxx= given

	     assert(EI!=0);

	     // use same stencil schemes for bc
	     if(useSameStencilSize)
	       {
		 // use (D+D-)D0 u=g3 and (D+D-)u-h^2/12*(D+D-)^2 u = g2 to dertermine first and second ghost line for u
		 // (D+D-)D0 u=g3                       =>  -u(ib-2*is)+  2.*u(ib-is)              - 2.*u(ib+is)+u(ib+2*is) = 2*is*dx3*g(3,side);
		 // (D+D-)u-h^2/12*(D+D-)^2 u= g2=>  -u(ib-2*is)+16.*u(ib-is)-30.*u(ib)+16.*u(ib+is)-u(ib+2*is) = 12*dx2*g(2,side);
		 // solve for u(ib-2*is) and u(ib-is):
		 u(ib-is,0,0,0)    = (30.*u(ib,0,0,0)-18.*u(ib+is,0,0,0)+2.*u(ib+2*is,0,0,0)+12.*dx2*g(2,side)-2.*is*dx3*g(3,side))/14.;
		 u(ib-2*is,0,0,0) = (18.*u(ib-is,0,0,0)-30.*u(ib,0,0,0)+14.*u(ib+is,0,0,0) -12.*dx2*g(2,side)-2.*is*dx3*g(3,side))/2.;

		 // same for v and a
		 v(ib-is,0,0,0)    = (30.*v(ib,0,0,0)-18.*v(ib+is,0,0,0)+2.*v(ib+2*is,0,0,0)+12.*dx2*gt(2,side)-2.*is*dx3*gt(3,side))/14.;
		 v(ib-2*is,0,0,0) = (18.*v(ib-is,0,0,0)-30.*v(ib,0,0,0)+14.*v(ib+is,0,0,0) -12.*dx2*gt(2,side)-2.*is*dx3*gt(3,side))/2.;
		 a(ib-is,0,0,0)    = (30.*a(ib,0,0,0)-18.*a(ib+is,0,0,0)+2.*a(ib+2*is,0,0,0)+12.*dx2*gtt(2,side)-2.*is*dx3*gtt(3,side))/14.;
		 a(ib-2*is,0,0,0) = (18.*a(ib-is,0,0,0)-30.*a(ib,0,0,0)+14.*a(ib+is,0,0,0) -12.*dx2*gtt(2,side)-2.*is*dx3*gtt(3,side))/2.;
	       }
	     else
	       {
	     
		 // use same order schemes for bc
		 u(ib-is,0,0,0)=2.*u(ib,0,0,0)-u(ib+is,0,0,0)+dx2*g(2,side);         /// uxx is given, this determines the first ghost line, this produces wiggles for a
		 v(ib-is,0,0,0)=2.*v(ib,0,0,0)-v(ib+is,0,0,0)+dx2*gt(2,side);         /// vxx is given, this determines the first ghost line
		 a(ib-is,0,0,0)=2.*a(ib,0,0,0)-a(ib+is,0,0,0)+dx2*gtt(2,side);         /// axx is given, this determines the first ghost line

		 u(ib-2*is,0,0,0)=2.*u(ib-is,0,0,0)-2.*u(ib+is,0,0,0)+u(ib+2*is,0,0,0)-is*2.*dx3*g(3,side); // uxxx is given, this determines the second ghost line   
		 v(ib-2*is,0,0,0)=2.*v(ib-is,0,0,0)-2.*v(ib+is,0,0,0)+v(ib+2*is,0,0,0)-is*2.*dx3*gt(3,side); // uxxx is given, this determines the second ghost line   
		 a(ib-2*is,0,0,0)=2.*a(ib-is,0,0,0)-2.*a(ib+is,0,0,0)+a(ib+2*is,0,0,0)-is*2.*dx3*gtt(3,side); // uxxx is given, this determines the second ghost line
	     
	       }

	   }
	 else if( bc==internalForceBC )
	   {
	     // Longfei 20170106: fill in the ghost point using extrapolation for internalForce.
	     //           the ghost point value is needed as we are using hermite interpolant to evalute internalForce
	     //           interpolateSolution(internalForce, elemNum, eta, DDdisplacement, DDslope); where nodal values and slopes are needed.
	     //           for FDBeamModel, we use centered finite difference to evaluate nodal slope, so at the beam ends, valid ghost values are needed.
	     // NOTE: In this case, "a" represents the internalForce f

	     //NOTE: this is better than direct extrapolation. But still not good enough! I will try directly evaluate fx inside of FDBeamModel::computeInternalForce()
	     // We first extrapolate fx at the boudary and then compute the ghost from fx
	     real fxm1, fx0,fx1,fx2,fx3;
	     // (delta/12.*X(i-2,0,0,0)-(.5+delta/6.)*X(i-1,0,0,0)+(.5+delta/6.)*X(i+1,0,0,0)-(delta/12.)*X(i+2,0,0,0))/(is*dx);
	     fx1 =( -a(ib,0,0,0)+a(ib+2*is,0,0,0))/(is*2.*dx);
	     fx2 =( -a(ib+1*is,0,0,0)+a(ib+3*is,0,0,0))/(is*2.*dx);
	     fx3 =( -a(ib+2*is,0,0,0)+a(ib+4*is,0,0,0))/(is*2.*dx);

	     fx0= 3.*fx1-3.*fx2+fx3;
	     fxm1= 3.*fx0-3.*fx1+fx2;
	     
	     a(ib-is,0,0,0) = a(ib+is,0,0,0)-is*2.*dx*fx0;  // ax is approximated by fx0, this determines the first ghost line
	     a(ib-2*is,0,0,0) = a(ib,0,0,0)-is*2.*dx*fxm1;

	     // CAN WE DO BETTER HERE???????????
	     
	     //TRIED THESE. NOTE: Direct Extrapolation for ghost was not good enough. 
	     //a(ib-is,0,0,0)=3.*a(ib,0,0,0)-3.*a(ib+is,0,0,0)+a(ib+2*is,0,0,0);
	     //a(ib-2*is,0,0,0)=3.*a(ib-is,0,0,0)-3.*a(ib,0,0,0)+a(ib+is,0,0,0);  
	     
	   }

	      
	  if(false)
	    {
	      // check if the ghost points are computed correctly
	      RealArray  ue,ve,ae;
	      getExactSolution(t,ue,ve,ae);
	      real errug0,errug1,errug2,errvg0, errvg1,errvg2,errag0, errag1,errag2;
	      errug0=abs( u(ib,0,0,0)- ue(ib,0,0,0));
	      errug1=abs( u(ib-is,0,0,0)- ue(ib-is,0,0,0));
	      errug2=abs( u(ib-2*is,0,0,0)- ue(ib-2*is,0,0,0));
	      errvg0=abs( v(ib,0,0,0)- ve(ib,0,0,0));
	      errvg1=abs( v(ib-is,0,0,0)- ve(ib-is,0,0,0));
	      errvg2=abs( v(ib-2*is,0,0,0)- ve(ib-2*is,0,0,0));
	      errag0=abs( a(ib,0,0,0)- ae(ib,0,0,0));
	      errag1=abs( a(ib-is,0,0,0)- ae(ib-is,0,0,0));
	      errag2=abs( a(ib-2*is,0,0,0)- ae(ib-2*is,0,0,0));
	      printF("t=%5.2e,errug0=%8.2e, errug1=%8.2e,errug2=%8.2e,errvg0=%8.2e,errvg1=%8.2e,errvg2=%8.2e,errag0=%8.2e,errag1=%8.2e,errag2=%8.2e\n",t,errug0,errug1,errug2,errvg0,errvg1,errvg2,errag0,errag1,errag2);
	    }

	  
	  
	}

    }


  return 0;
}



// ====================================================================================
/// \brief Smooth the Finite Difference solution with a 4th/6th-order filter.
/// \param t (input) : current time
/// \param w (input/output) : finite difference solution to smooth (u, v or a)
/// \param label (input) : label for debug output.
// ====================================================================================
void FDBeamModel::
smooth( const real t, RealArray & w, const aString & label )
{
  const int & numElem = dbase.get<int>("numElem");
  const bool & smoothSolution = dbase.get<bool>("smoothSolution");
  const BoundaryConditionEnum * bc = dbase.get<BoundaryConditionEnum[2]>("boundaryConditions");
  const BoundaryConditionEnum & bcLeft =  bc[0];
  const BoundaryConditionEnum & bcRight =  bc[1];

  if( !smoothSolution )
    return;
  
  const int & numberOfSmooths = dbase.get<int>("numberOfSmooths");
  const int & smoothOrder     = dbase.get<int>("smoothOrder");

  // add ghost points so we add apply filter up to boundary if needed
  int numberOfGhost = smoothOrder/2;    // 2 for 4th-order, 3 for 6th order filter
  int base =0, bound = numElem;

  Index I1,I2,I3,C;
  getSolutionArrayIndex(I1,I2,I3,C);

  
  Index Iw1;
  Iw1=Range(base-numberOfGhost,bound+numberOfGhost);  
  RealArray w1(Iw1,I2,I3,C);
  // -- copy input into w1
  w1(I1,I2,I3,C)=w(I1,I2,I3,C);

  //????Longfei: how to fill the 3rd ghost?
  
  const bool isPeriodic = bcLeft==periodic;
  
  const int orderOfExtrapolation=smoothOrder+1; 

  // we need at least this many elements to apply the smoother:
  assert( numElem >= orderOfExtrapolation );

  const real & dt = dbase.get<real>("dt"); 
  const real omega= dbase.get<real>("smoothOmega");  // parameter in smoother (=1 : kill plus minus mode)
  if( t < 3.*dt )
  {
    printF("-- BM%i -- smooth %s, numberOfSmooths=%i (%ith order filter), omega=%9.3e isPeriodic=%i t=%8.2e.\n",
	   getBeamID(),(const char*)label,numberOfSmooths,smoothOrder,omega,(int)isPeriodic,t );
  }


//  omega *=dt ;  /// **TRY THIS **

  //const int bc[2]={bcLeft,bcRight};  // 

  // I : smooth these points for u, v or a . Keep the boundary points fixed, except for
  //  periodic 
  //  slide 
  // int freeEnd = freeBC;
  // ???Longfei: what is this?
  //  int freeEnd = -10; // turn off smooth on the boundary pts for a free end 

  // const int i1a= (isPeriodic || bc[0]==freeBC || bc[0]==slideBC ) ? base  : base+1;
  // const int i1b= (isPeriodic || bc[1]==freeBC || bc[1]==slideBC ) ? bound : bound-1;
  const int i1a= base+1;
  const int i1b=bound-1;
 
  Index I=Range(i1a,i1b);

  // smoothBoundaryConditions( w1, base, bound, numberOfGhost,orderOfExtrapolation );

  for( int smooth=0; smooth<numberOfSmooths; smooth++ )
  {
    // smooth interior pts (and boundary pts sometimes): 

    if( smoothOrder==4 )
    {
      // 4th order filter: 
      w1(I,I2,I3,C)= w1(I,I2,I3,C) + (omega/16.)*(-w1(I-2,I2,I3,C) + 4.*w1(I-1,I2,I3,C) -6.*w1(I,I2,I3,C) + 4.*w1(I+1,I2,I3,C) -w1(I+2,I2,I3,C) );
      
    }
    else if( smoothOrder==6 )
    {
      // 6th order filter: 
      // 1 4 6 4 1 
      // 1 5 10 10 5 1
      // 1 6 15 20 15 6 1 
      w1(I)= w1(I) + (omega/64.)*(w1(I-3) - 6.*w1(I-2) +15.*w1(I-1) -20.*w1(I)
				      + 15.*w1(I+1) -6.*w1(I+2) + w1(I+3) );
    }
    else
    {
      printF("BeamModel::smooth:ERROR: not implemented for smoothOrder=%i.\n",smoothOrder);
      OV_ABORT("error");
    }

    // copy smoothed solution back to w
    w(I,I2,I3,C)=w1(I,I2,I3,C); // only copy back the points that are smoothed

    if(isPeriodic)
      {
    	// -- assign values on the right boundary node
    	w(numElem,0,0,0) = w(0,0,0,0);
      
    	// -- assign values on ghost points
    	w(-1,0,0,0) = w(numElem-1,0,0,0);
    	w(-2,0,0,0) = w(numElem-2,0,0,0);
    	w(numElem+1,0,0,0) = w(1,0,0,0);
    	w(numElem+2,0,0,0) = w(2,0,0,0);
      }
    //smoothBoundaryConditions( w1, base, bound, numberOfGhost,orderOfExtrapolation );

  } // end smooths



}



// =========================================================================================
/// \brief Add internal forces such as buoyancy and TZ forces
///
/// Compute the element force vectors 
// =========================================================================================
void FDBeamModel::
addInternalForces( const real t, RealArray & f )
{
  //Longfei 20160214: new way

  //call the base version to get nodal values of internal forces;
  RealArray fi=f; 
  BeamModel::addInternalForces( t,  fi ); // get the internalforces such as buoyancy and TZ forces

  f+=fi; // add interalforces to total force

}




// Longfei 20160622: new function for FDBeamModel.
//                  
// ===================================================================================================
/// \brief  Return the nodal force values on the beam reference line.
///         The force of FDBeamModel is already stored in nodal value, so just return the current force on the grids exclude ghost points
// ====================================================================================================
void FDBeamModel::
getForceOnBeam( const real t, RealArray & force )
{

  const int & current = dbase.get<int>("current"); 
  const int & numElem = dbase.get<int>("numElem");

  RealArray & time = dbase.get<RealArray>("time");
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force in nodal values 

  RealArray & fc = f[current];  // force at current time
  if( fabs(time(current)-t) > 1.e-10*(1.+t) )
    {
      printF("-- BM%i -- BeamModel::getForceOnBeam:ERROR: t=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
	     getBeamID(),t,time(current),current);
      OV_ABORT("ERROR");
    }

  force.redim(numElem+1);
  Index I2,I3,C;
  I2=I3=C=0;
  force=fc(Range(0,numElem),I2,I3,C);  // return values on interior and boundary grids
  
  if( false )
    {
      ::display(force,sPrintF("-- BM -- getForceOnBeam: force at t=%9.3e",t),"%8.2e ");
    }
  
}





//Longfei 20160809: new function that computes the time derivatives of the external forcing on boundaries.
// This is needed by compartibiltiy boundary conditions
//  =========================================================================================
/// \brief  Return the time derivatives of the forces on the boundary
/// \param ntd (input) number of time derivatives 
/// \param f(0:1) (output) : f(side), side=0,1 (left or right)
/// /Note: Only some vaues apply, depending on the BC
//  =========================================================================================
int FDBeamModel::
getBoundaryForces( const real t, RealArray & f, const int ntd /* = 0 */   )
{
  if( f.getLength(0)==0 ) f.redim(2); 
  f=0.;

  const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  const  int & current = dbase.get<int>("current");
  const  RealArray & time = dbase.get<RealArray>("time");
  
   if( fabs(time(current)-t) > 1.e-10*(1.+t) )
    {
      printF("-- BM%i -- BeamModel::getBoundaryForces:ERROR: t=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
	     getBeamID(),t,time(current),current);
      OV_ABORT("ERROR");
    }

   const int prev  = (current-1+numberOfTimeLevels)%numberOfTimeLevels;
   const int prev2= ( prev -1 + numberOfTimeLevels)%numberOfTimeLevels;

   const real dt = time(current)-time(prev);
   const real dtPrev = time(prev)-time(prev2);

   const std::vector<RealArray> & forces= dbase.get<std::vector<RealArray> >("f"); // force
   const RealArray & f1 = forces[prev2];
   const RealArray & f2 = forces[prev];
   const RealArray & f3 = forces[current];


   const int & numElem = dbase.get<int>("numElem");

   if(ntd==0)
     {
       f(0)=f3(0,0,0,0);
       f(1)=f3(numElem,0,0,0);       
     }
   else if(ntd==2)
     {
       if( t >= 1.5*dt && dt>0)  
	  {
	    // -- we have 2 old forces available: f(t-dt) and f(t-dt-dtPrev) --
	    // use backward finite difference formula to evaluate second time derivative of the current force
	    f(0)= (dt/dtPrev*f3(0,0,0,0)-(dt+dtPrev)/dt*f2(0,0,0,0)+f1(0,0,0,0))/(0.5*dtPrev*(dt+dtPrev));
	    f(1)= (dt/dtPrev*f3(numElem,0,0,0)-(dt+dtPrev)/dt*f2(numElem,0,0,0)+f1(numElem,0,0,0))/(0.5*dtPrev*(dt+dtPrev));
	  }
	else
	  {
	    // just set it to be zero
	    f(0)=0.;
	    f(1)=0.;    
	  }
     }
   

   
   
   
  
   return 0;
}
