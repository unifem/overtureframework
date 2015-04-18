#include "Cgad.h"
#include "Parameters.h"
#include "MappedGridOperators.h"
#include "ParallelUtility.h"
#include "EquationDomain.h"

int Cgad::
getUt(const realMappedGridFunction & v,
      const realMappedGridFunction & gridVelocity_, 
      realMappedGridFunction & dvdt, 
      int iparam[], real rparam[],
      realMappedGridFunction & dvdtImplicit /* = Overture::nullRealMappedGridFunction() */,
      MappedGrid *pmg2 /* = NULL */,
      const realMappedGridFunction *pGridVelocity2 /* = NULL */ )
//===============================================================================================
//  /Description:
//    Return du/dt for for the advection diffusion equations
//
//  /tForce (input): apply the forcing at this time (by default apply at gf.t)
//  /pGridVelocity2 (input) : for moving grids only, supply the grid velocity at time t+dt for moving grids.
//
// /Notes:
//
// Implicit time-stepping notes: 
// ----------------------------
//    Suppose we are solving the PDE:
//           u_t = f(u,x,t)  + F(x,t)
//   that we have split into an explicit part, fe(u),  and implicit part, A*u:
//           u_t = fe(u) + A u  + F(x,t)
//
//   If the time stepping method is implicit then we compute
//           dvdt = fe(u)
//   When implicitOption==computeImplicitTermsSeparately we also compute:
//           dvdtImplicit = (1-alpha)*A*u 
//   where alpha is the implicit factor (= .5 for Crank-Nicolson). This is the part of the implicit
//   term that is treated explicitly. 
//   (if implicitOption==doNotComputeImplicitTerms then do not change dvdtImplicit).
// 
//===============================================================================================
{
  real cpu0=getCPU();

  FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
  FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

  assert( pmg2!=NULL );

  const real & t=rparam[0];
  real tForce   =rparam[1];
  const real & tImplicit=rparam[2];
  const int & grid = iparam[0];
  const int level=iparam[1];
  const int numberOfStepsTaken = iparam[2];
  const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents")-parameters.dbase.get<int >("numberOfExtraVariables");

  const int gridIsImplicit=parameters.getGridIsImplicit(grid);
  const Parameters::ImplicitOption & implicitOption = parameters.dbase.get<Parameters::ImplicitOption >("implicitOption");
  // const real implicitFactor = parameters.dbase.get<real >("implicitFactor");

  const bool & gridIsMoving = parameters.gridIsMoving(grid);
  const Parameters::ReferenceFrameEnum referenceFrame = parameters.getReferenceFrame();
  const bool adjustForMovingGrids = gridIsMoving && referenceFrame==Parameters::fixedReferenceFrame;

  const bool variableDiffusivity = parameters.dbase.get<bool >("variableDiffusivity");
  const bool variableAdvection = parameters.dbase.get<bool >("variableAdvection");

  const bool & implicitAdvection = parameters.dbase.get<bool >("implicitAdvection");

  MappedGrid & mg = *(v.getMappedGrid());
  MappedGridOperators & op = *(v.getOperators());
  
  // For now we need the center array for the axisymmetric case:
  const bool isAxisymmetric =  parameters.isAxisymmetric();
  const bool vertexNeeded=parameters.isAxisymmetric();
  const realArray & xy = vertexNeeded ? mg.center() : v;
  if( vertexNeeded ) 
  {
    assert( mg.center().getLength(0)>0 );
  }

  Index I1,I2,I3;
  getIndex(mg.extendedIndexRange(),I1,I2,I3);
  Index N(0,numberOfComponents);
  
  OV_GET_SERIAL_ARRAY_CONST(real,xy,xLocal);
  OV_GET_SERIAL_ARRAY_CONST(real,v,uLocal);
  OV_GET_SERIAL_ARRAY(real,dvdt,utLocal);
  OV_GET_SERIAL_ARRAY(real,dvdtImplicit,utiLocal);
  OV_GET_SERIAL_ARRAY_CONDITIONAL(real,(realArray&)gridVelocity_,gvLocal,adjustForMovingGrids);
  
  // --- look for variable diffusion coefficients ---
  realCompositeGridFunction*& pKappaVar= parameters.dbase.get<realCompositeGridFunction*>("kappaVar");
  if( variableDiffusivity && pKappaVar==NULL )
  {
    OV_ABORT(" Cgad::getUt:ERROR:kappaVar not created! ");
  }
  realArray & kappaVar = variableDiffusivity ? (*pKappaVar)[grid] : dvdt; 
  OV_GET_SERIAL_ARRAY_CONDITIONAL(real,kappaVar,kappaVarLocal,variableDiffusivity);

  // -- look for variable advection velocity ---
  realCompositeGridFunction*& pAdvectVar= parameters.dbase.get<realCompositeGridFunction*>("advectVar");
  if( variableAdvection && pAdvectVar==NULL )
  {
    OV_ABORT(" Cgad::getUt:ERROR:advectVar not created! ");
  }
  realArray & advectVar = variableAdvection ? (*pAdvectVar)[grid] : dvdt; 
  OV_GET_SERIAL_ARRAY_CONDITIONAL(real,advectVar,advectVarLocal,variableAdvection);
  
  if( debug() & 4 && adjustForMovingGrids )
  {
    char buff[120];
    ::display(gvLocal,sPrintF(buff,"getUt: gridVelocity: grid=%i, t=%e",grid,t),pDebugFile,"%5.2f ");
  }
  
  


  bool ok = ParallelUtility::getLocalArrayBounds(v,uLocal,I1,I2,I3);
  if( ok )
  {

    std::vector<real> & kappa = parameters.dbase.get<std::vector<real> >("kappa");
    std::vector<real> & a = parameters.dbase.get<std::vector<real> >("a");
    std::vector<real> & b = parameters.dbase.get<std::vector<real> >("b");
    std::vector<real> & c = parameters.dbase.get<std::vector<real> >("c");    
    bool firstDerivNeeded=adjustForMovingGrids; // false;
    for( int m=0; m<numberOfComponents && !firstDerivNeeded; m++ )
    {
      firstDerivNeeded= firstDerivNeeded || a[m]!=0. || b[m]!=0. || c[m]!=0.;
    }

    
    realSerialArray ux,uy,uz;
    realSerialArray uLap(I1,I2,I3,N);

    if( firstDerivNeeded )
    {
      ux.redim(I1,I2,I3,N); uy.redim(I1,I2,I3,N); 
      op.derivative(MappedGridOperators::xDerivative,uLocal,ux  ,I1,I2,I3,N);
      op.derivative(MappedGridOperators::yDerivative,uLocal,uy  ,I1,I2,I3,N);
    }
    
    if( variableDiffusivity )
    {
      // Variable diffusivity:
      if( true || t==0. ) printF("getUT:INFO: variable dissipation evaluated at t=%9.3e for grid=%i\n",t,grid);
      
      op.derivative(MappedGridOperators::divergenceScalarGradient,uLocal,kappaVarLocal,uLap,I1,I2,I3,N);
    }
    else
    {
      // constant diffusivity: 
      op.derivative(MappedGridOperators::laplacianOperator,uLocal,uLap,I1,I2,I3,N);
    }
    

    RealArray radiusInverse; 
    if( isAxisymmetric )
    {
      // add on the axis-symmetric correction
      //   Delta(u) = u_xx + u_yy + u_y/y   y>0 
      //            = u_xx + u_yy + u_yy    y=0 
      assert( mg.numberOfDimensions()==2 );
      radiusInverse.redim(I1,I2,I3);
      radiusInverse(I1,I2,I3) = 1./max(REAL_MIN,xLocal(I1,I2,I3,axis2));
      Index Ib1,Ib2,Ib3;
      for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
      {
	for( int side=0; side<=1; side++ )
	{
	  if( mg.boundaryCondition(side,axis)==Parameters::axisymmetric ) // we should use bcLocal here 
	  {
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
            bool ok = ParallelUtility::getLocalArrayBounds(v,uLocal,Ib1,Ib2,Ib3);
            if( ok )
	    {
  	      radiusInverse(Ib1,Ib2,Ib3)=0.;
              RealArray uyy(Ib1,Ib2,Ib3,N);
              op.derivative(MappedGridOperators::yyDerivative,uLocal,uyy,Ib1,Ib2,Ib3,N);

              uLap(Ib1,Ib2,Ib3,N)+=uyy(Ib1,Ib2,Ib3,N);
	    }
	  }
	}
      }
	
      if( debug() & 8 )
      {
	display(radiusInverse,sPrintF("Cgad::getUt: radiusInverse, grid=%i",grid),pDebugFile,"%8.5f ");
      }

      if( !firstDerivNeeded )
      {
	uy.redim(I1,I2,I3,N); 
	op.derivative(MappedGridOperators::yDerivative,uLocal,uy  ,I1,I2,I3,N);
      }
      for( int m=0; m<numberOfComponents; m++ )
       uLap(I1,I2,I3,m) += uy(I1,I2,I3,m)*radiusInverse;
      
    }
    if( !variableDiffusivity )
    { // For constant diffusivity, multiply by kappa
      for( int m=0; m<numberOfComponents; m++ )
	uLap(I1,I2,I3,m)*=kappa[m];
    }
    
    if( mg.numberOfDimensions()==2 )
    {
      if( firstDerivNeeded )
      {
	for( int m=0; m<numberOfComponents; m++ )
	{
	  // printf("getUt: convectionDiffusion: m=%i, a,b,kappa=%5.3f, %5.3f, %8.2e\n",m,a[m],b[m],kappa[m]);
	  if( !gridIsImplicit )
	  {
	    if( variableAdvection )
	    {
	      utLocal(I1,I2,I3,m)=uLap(I1,I2,I3,m) 
		- ( advectVarLocal(I1,I2,I3,0)*ux(I1,I2,I3,m) + advectVarLocal(I1,I2,I3,1)*uy(I1,I2,I3,m) );
	    }
            else
	    {
	      utLocal(I1,I2,I3,m)=(-a[m])*ux(I1,I2,I3,m)+(-b[m])*uy(I1,I2,I3,m)+uLap(I1,I2,I3,m);
	    }
	    
	  }
	  else 
	  {
	    // Here are the terms that we treat explicitly:
            // utLocal(I1,I2,I3,m)=(-a[m])*ux(I1,I2,I3,m)+(-b[m])*uy(I1,I2,I3,m);
	    if( !implicitAdvection )
	    {
	      // --- advection terms: explicit
              // --- diffusion terms: implicit
	      if( variableAdvection )
	      {
		utLocal(I1,I2,I3,m)=
		  - ( advectVarLocal(I1,I2,I3,0)*ux(I1,I2,I3,m) + advectVarLocal(I1,I2,I3,1)*uy(I1,I2,I3,m) );
	      }
	      else
	      {
		utLocal(I1,I2,I3,m)=(-a[m])*ux(I1,I2,I3,m)+(-b[m])*uy(I1,I2,I3,m);
	      }

	      if( implicitOption==Parameters::computeImplicitTermsSeparately )
	      {
		// Here are the terms for the part treated implicitly
              
		// real nuE = kappa[m]*(1.-implicitFactor);   // This is no longer done here *wdh* 0711122
		// utiLocal(I1,I2,I3,m)=nuE*uLap(I1,I2,I3,m);
		utiLocal(I1,I2,I3,m)=uLap(I1,I2,I3,m);
	      }
	    }
	    else
	    {
	      // --- advection terms: implicit
              // --- diffusion terms: implicit
	      utLocal(I1,I2,I3,m)=0.;  // no terms are entirely explicit 

	      if( implicitOption==Parameters::computeImplicitTermsSeparately )
	      {
		// Here are the terms for the part treated implicitly
              
		if( variableAdvection )
		{
		  utiLocal(I1,I2,I3,m)=
		    - ( advectVarLocal(I1,I2,I3,0)*ux(I1,I2,I3,m) + advectVarLocal(I1,I2,I3,1)*uy(I1,I2,I3,m) ) + uLap(I1,I2,I3,m);
		}
		else
		{
		  utiLocal(I1,I2,I3,m)=(-a[m])*ux(I1,I2,I3,m)+(-b[m])*uy(I1,I2,I3,m) + uLap(I1,I2,I3,m);
		}
	      }

	    }

	  }

          if( adjustForMovingGrids )
	  {
	    utLocal(I1,I2,I3,m) += gvLocal(I1,I2,I3,0)*ux(I1,I2,I3,m) + gvLocal(I1,I2,I3,1)*uy(I1,I2,I3,m);
	  }
	  
	}
      }
      else
      {
	for( int m=0; m<numberOfComponents; m++ )
	{
	  if( !gridIsImplicit )
	  {
	    utLocal(I1,I2,I3,m)=uLap(I1,I2,I3,m);
	  }
	  else 
	  {
	    // Here are the terms that we treat explicitly:
            utLocal(I1,I2,I3,m)=0.;
	    if( implicitOption==Parameters::computeImplicitTermsSeparately )
	    {
              // Here are the terms for the part treated implicitly
	      // real nuE = kappa[m]*(1.-implicitFactor);  // This is no longer done here *wdh* 0711122
	      // utiLocal(I1,I2,I3,m)=nuE*uLap(I1,I2,I3,m);
              utiLocal(I1,I2,I3,m)=uLap(I1,I2,I3,m);
	    }
	  }
	}
      }
    }
    else // *********** 3D ***************
    {
      if( firstDerivNeeded )
      {
	uz.redim(I1,I2,I3,N);
	op.derivative(MappedGridOperators::zDerivative,uLocal,uz ,I1,I2,I3,N);

	for( int m=0; m<numberOfComponents; m++ )
	{
	  if( !gridIsImplicit )
	  {
	    if( variableAdvection )
	    {
	      utLocal(I1,I2,I3,m)=uLap(I1,I2,I3,m) 
		- ( advectVarLocal(I1,I2,I3,0)*ux(I1,I2,I3,m) + 
                    advectVarLocal(I1,I2,I3,1)*uy(I1,I2,I3,m) + 
                    advectVarLocal(I1,I2,I3,2)*uz(I1,I2,I3,m) );
	    }
            else
	    {
	      utLocal(I1,I2,I3,m)=(-a[m])*ux(I1,I2,I3,m)+(-b[m])*uy(I1,I2,I3,m)+(-c[m])*uz(I1,I2,I3,m)
                    +uLap(I1,I2,I3,m);
	    }
	  }
	  else 
	  {
	    // Here are the terms that we treat explicitly:
            // utLocal(I1,I2,I3,m)=(-a[m])*ux(I1,I2,I3,m)+(-b[m])*uy(I1,I2,I3,m)+(-c[m])*uz(I1,I2,I3,m);
	    if( variableAdvection )
	    {
	      utLocal(I1,I2,I3,m)=
		- ( advectVarLocal(I1,I2,I3,0)*ux(I1,I2,I3,m) + 
                    advectVarLocal(I1,I2,I3,1)*uy(I1,I2,I3,m) + 
                    advectVarLocal(I1,I2,I3,2)*uz(I1,I2,I3,m) );
	    }
            else
	    {
	      utLocal(I1,I2,I3,m)=(-a[m])*ux(I1,I2,I3,m)+(-b[m])*uy(I1,I2,I3,m)+(-c[m])*uz(I1,I2,I3,m);
	    }

	    if( implicitOption==Parameters::computeImplicitTermsSeparately )
	    {
              // Here are the terms for the part treated implicitly
	      // real nuE = kappa[m]*(1.-implicitFactor);  // This is no longer done here *wdh* 0711122
	      // utiLocal(I1,I2,I3,m)=nuE*uLap(I1,I2,I3,m);
	      utiLocal(I1,I2,I3,m)=uLap(I1,I2,I3,m);
	    }
	  }
          if( adjustForMovingGrids )
	  {
	    utLocal(I1,I2,I3,m) += (gvLocal(I1,I2,I3,0)*ux(I1,I2,I3,m) + 
                                    gvLocal(I1,I2,I3,1)*uy(I1,I2,I3,m) +
                                    gvLocal(I1,I2,I3,2)*uz(I1,I2,I3,m));
	  }
	}
      }
      else
      {
	for( int m=0; m<numberOfComponents; m++ )
	{
	  if( !gridIsImplicit )
	  {
	    utLocal(I1,I2,I3,m)=uLap(I1,I2,I3,m);
	  }
	  else 
	  {
	    // Here are the terms that we treat explicitly:
            utLocal(I1,I2,I3,m)=0.;
	    if( implicitOption==Parameters::computeImplicitTermsSeparately )
	    {
              // Here are the terms for the part treated implicitly
	      // real nuE = kappa[m]*(1.-implicitFactor);  // This is no longer done here *wdh* 0711122
	      utiLocal(I1,I2,I3,m)=uLap(I1,I2,I3,m);
	    }
	  }
	}
      }
      
    }

  }
  
  if( debug() & 4 && adjustForMovingGrids )
  {
    char buff[120];
    ::display(dvdt,sPrintF(buff,"getUt: dvdt before add forcing: grid=%i, t=%e",grid,t),debugFile,"%5.2f ");
  }

  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForGetUt"))+=getCPU()-cpu0;
  addForcing(dvdt,v,iparam,rparam,dvdtImplicit,(realMappedGridFunction*)(&gridVelocity_));

  return 0;
}
