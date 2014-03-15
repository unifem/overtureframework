// ==============================================================================
//    Get the time stepping eigenvalues (that determine dt) 
// ==============================================================================

#include "Cgad.h"
#include "Parameters.h"
#include "MappedGridOperators.h"
#include "display.h"
#include "ParallelUtility.h"
#include "EquationDomain.h"


//\begin{>>MappedGridSolverInclude.tex}{\subsection{getTimeSteppingEigenvalue}} 
void Cgad::
getTimeSteppingEigenvalue(MappedGrid & mg,
			  realMappedGridFunction & u0, 
			  realMappedGridFunction & gridVelocity,  
			  real & reLambda,
			  real & imLambda,
			  const int & grid)
//=====================================================================================================
// /Description:
//   Determine the real and imaginary parts of the eigenvalues for time stepping.
//
// /Author: WDH
//
//\end{MappedGridSolverInclude.tex}  
// ===================================================================================================
{

  assert( grid>=0 && grid<realPartOfEigenvalue.size() && grid<imaginaryPartOfEigenvalue.size() );
  real & realPartOfTimeSteppingEigenvalue      = realPartOfEigenvalue[grid];
  real & imaginaryPartOfTimeSteppingEigenvalue = imaginaryPartOfEigenvalue[grid];


  reLambda=0.;
  imLambda=0.;
  
  ListOfShowFileParameters *pPar=&parameters.dbase.get<ListOfShowFileParameters >("pdeParameters");
  if( parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
  {
    // use parameters from this equationDomain 
    ListOfEquationDomains & equationDomainList = *(parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"));
    const int numberOfEquationDomains=equationDomainList.size();
    const int equationDomainNumber=equationDomainList.gridDomainNumberList[grid];
    assert( equationDomainNumber>=0 && equationDomainNumber<numberOfEquationDomains );
    EquationDomain & equationDomain = equationDomainList[equationDomainNumber];
    pPar= &equationDomain.pdeParameters;
  }
  ListOfShowFileParameters & pdeParameters = *pPar;

  bool gridIsImplicit = parameters.getGridIsImplicit(grid);
  const bool & gridIsMoving = parameters.gridIsMoving(grid);
  const Parameters::ReferenceFrameEnum referenceFrame = parameters.getReferenceFrame();
  const bool adjustForMovingGrids = gridIsMoving && referenceFrame==Parameters::fixedReferenceFrame;

  const bool variableDiffusivity = parameters.dbase.get<bool >("variableDiffusivity");
  const bool variableAdvection = parameters.dbase.get<bool >("variableAdvection");

  std::vector<real> & kappav = parameters.dbase.get<std::vector<real> >("kappa");
  std::vector<real> & av = parameters.dbase.get<std::vector<real> >("a");
  std::vector<real> & bv = parameters.dbase.get<std::vector<real> >("b");
  std::vector<real> & cv = parameters.dbase.get<std::vector<real> >("c");    

  // Base the time step on the worst case for any component -- this is too restrictive in general --
  real kappa=0.,a=0.,b=0.,c=0.;
  for( int m=0; m<parameters.dbase.get<int >("numberOfComponents"); m++ )
  {
    kappa=max(kappa,kappav[m]); 
    a=max(a,fabs(av[m])); 
    b=max(b,fabs(bv[m])); 
    c=max(c,fabs(cv[m])); 
  }

  MappedGridOperators & op = *u0.getOperators();

  #ifdef USE_PPP
    realSerialArray gvLocal; if( adjustForMovingGrids ) getLocalArrayWithGhostBoundaries(gridVelocity,gvLocal);
  #else
    const realSerialArray & gvLocal = gridVelocity;
  #endif

  // --- look for variable diffusion coefficients ---
  realCompositeGridFunction*& pKappaVar= parameters.dbase.get<realCompositeGridFunction*>("kappaVar");
  if( variableDiffusivity && pKappaVar==NULL )
  {
    OV_ABORT(" Cgad::getTimeSteppingEigenvalue:ERROR:kappaVar not created! ");
  }
  realArray & kappaVar = variableDiffusivity ? (*pKappaVar)[grid] : Overture::nullRealDistributedArray(); 
  OV_GET_SERIAL_ARRAY_CONDITIONAL(real,kappaVar,kappaVarLocal,variableDiffusivity);

  // -- look for variable advection velocity ---
  realCompositeGridFunction*& pAdvectVar= parameters.dbase.get<realCompositeGridFunction*>("advectVar");
  if( variableAdvection && pAdvectVar==NULL )
  {
    OV_ABORT(" Cgad::getTimeSteppingEigenvalue:ERROR:advectVar not created! ");
  }
  realArray & advectVar = variableDiffusivity ? (*pAdvectVar)[grid] : Overture::nullRealDistributedArray(); 
  OV_GET_SERIAL_ARRAY_CONDITIONAL(real,advectVar,advectVarLocal,variableAdvection);


  Index I1,I2,I3;
  getIndex( mg.indexRange(),I1,I2,I3); // Get Index's for the interior+boundary points
  OV_GET_SERIAL_ARRAY(real,u0,u0Local);
  bool ok=ParallelUtility::getLocalArrayBounds(u0,u0Local,I1,I2,I3); // get bounds local to this processor
  if( ok )
  {
    if( mg.numberOfDimensions()==2 )
    {
      const real axiCoeff = parameters.isAxisymmetric() ? 2. : 1.; 

      if( mg.isRectangular() )
      {
	// ***** rectangular grid *****
	real dx[3];
	mg.getDeltaX(dx);
    
	if( variableAdvection )
	  imLambda = (max(fabs(advectVarLocal(I1,I2,I3,0)))*(1./dx[0])+
		      max(fabs(advectVarLocal(I1,I2,I3,1)))*(1./dx[1]) );
	else
	  imLambda = fabs(a)*(1./(dx[0]))+fabs(b)*(1./dx[1]);

	if( !gridIsImplicit )
	{
	  if( variableDiffusivity ) // assume kappaVar is positive!
	    reLambda = max(kappaVarLocal(I1,I2,I3))*(4./(dx[0]*dx[0])+4.*axiCoeff/(dx[1]*dx[1]));
	  else
	    reLambda = kappa *(4./(dx[0]*dx[0])+4.*axiCoeff/(dx[1]*dx[1]));
	}
      
      }
      else
      {
	// ***** non-rectangular grid *****

	mg.update(MappedGrid::THEinverseVertexDerivative);  // make sure the jacobian derivatives are built
	// define an alias:
	realMappedGridFunction & rxd = mg.inverseVertexDerivative();
	OV_GET_SERIAL_ARRAY(real,rxd,rx);

	const int nd=mg.numberOfDimensions();
        #define MN(m,n) ((m)+nd*(n))
        #define RX(m,n) rx(I1,I2,I3,MN(m,n))

	realSerialArray a1(I1,I2,I3), b1(I1,I2,I3);

	// Grid spacings on unit square:
	real dr1 = mg.gridSpacing(axis1);
	real dr2 = mg.gridSpacing(axis2);
	if( !adjustForMovingGrids )
	{
	  if( variableAdvection )
	  {
	    a1 = advectVarLocal(I1,I2,I3,0)*RX(0,0) + advectVarLocal(I1,I2,I3,1)*RX(0,1);
	    b1 = advectVarLocal(I1,I2,I3,0)*RX(1,0) + advectVarLocal(I1,I2,I3,1)*RX(1,1);
	  }
	  else
	  {
	    a1 = a*RX(0,0) + b*RX(0,1);
	    b1 = a*RX(1,0) + b*RX(1,1);
	  }
	  
	}
	else
	{
	  if( variableAdvection )
	  {
	    a1 = (advectVarLocal(I1,I2,I3,0)-gvLocal(I1,I2,I3,0))*RX(0,0) + 
	      (advectVarLocal(I1,I2,I3,1)-gvLocal(I1,I2,I3,1))*RX(0,1);
	    b1 = (advectVarLocal(I1,I2,I3,0)-gvLocal(I1,I2,I3,0))*RX(1,0) + 
	      (advectVarLocal(I1,I2,I3,1)-gvLocal(I1,I2,I3,1))*RX(1,1);
	  }
	  else
	  {
	    a1 = (a-gvLocal(I1,I2,I3,0))*RX(0,0) + (b-gvLocal(I1,I2,I3,1))*RX(0,1);
	    b1 = (a-gvLocal(I1,I2,I3,0))*RX(1,0) + (b-gvLocal(I1,I2,I3,1))*RX(1,1);
	  }
	  
	}

	if( (kappa>0. || variableDiffusivity ) && !gridIsImplicit )
	{
	  realSerialArray kappa11(I1,I2,I3), kappa12(I1,I2,I3), kappa22(I1,I2,I3);

          realSerialArray & rxx=kappa11; // re-use these arrays 
          realSerialArray & ryy=kappa22;
	  op.derivative(MappedGridOperators::xDerivative,rx,rxx,I1,I2,I3,MN(0,0));  // rxx
	  // printf("getDt: max err in rxx=%8.2e\n",max(fabs(kappa11-rx.x(I1,I2,I3,0,0)(I1,I2,I3,0,0))));
	  op.derivative(MappedGridOperators::yDerivative,rx,ryy,I1,I2,I3,MN(0,1));  // ryy
	  // printf("getDt: max err in rxy=%8.2e\n",max(fabs(kappa22-rx.y(I1,I2,I3,0,1)(I1,I2,I3,0,1))));
	  // a1   = a*RX(0,0) + b*RX(0,1) - kappa*( kappa11+kappa22 );

	  if( variableDiffusivity )
	    a1 -= kappaVarLocal(I1,I2,I3)*( rxx+ryy ); // *wdh* 071129 - this extra term was missing 
	  else
	    a1 -= kappa*( rxx+ryy ); // *wdh* 071129 - this extra term was missing 
	  
          realSerialArray & sxx=kappa11; // re-use these arrays 
          realSerialArray & syy=kappa22;
	  op.derivative(MappedGridOperators::xDerivative,rx,sxx,I1,I2,I3,MN(1,0));  // sxx
	  // printf("getDt: max err in sxx=%8.2e\n",max(fabs(kappa11-rx.x(I1,I2,I3,1,0)(I1,I2,I3,1,0))));
	  op.derivative(MappedGridOperators::yDerivative,rx,syy,I1,I2,I3,MN(1,1));  // syy
	  // printf("getDt: max err in syy=%8.2e\n",max(fabs(kappa22-rx.y(I1,I2,I3,1,1)(I1,I2,I3,1,1))));
	  // b1   = a*RX(1,0) + b*RX(1,1) - kappa*( kappa11+kappa22 );
	  if( variableDiffusivity )
	    b1 -= kappaVarLocal(I1,I2,I3)*( sxx+syy ); // *wdh* 071129 - this extra term was missing 
	  else
	    b1 -= kappa*( sxx+syy ); // *wdh* 071129 - this extra term was missing 
	  
	  // kappa11 = kappa*( r1.x*r1.x + r1.y*r1.y )
	  // kappa12 = kappa*( r1.x*r2.x + r1.y*r2.y )*2 
	  // kappa22 = kappa*( r2.x*r2.x + r2.y*r2.y ) 
	  if( variableDiffusivity )
	  {
	    kappa11 = kappaVarLocal(I1,I2,I3)*( RX(0,0)*RX(0,0) + axiCoeff*RX(0,1)*RX(0,1) );
	    kappa12 = kappaVarLocal(I1,I2,I3)*( RX(0,0)*RX(1,0) + axiCoeff*RX(0,1)*RX(1,1) )*2.;
	    kappa22 = kappaVarLocal(I1,I2,I3)*( RX(1,0)*RX(1,0) + axiCoeff*RX(1,1)*RX(1,1) );
	  }
	  else
	  {
	    kappa11 = kappa*( RX(0,0)*RX(0,0) + axiCoeff*RX(0,1)*RX(0,1) );
	    kappa12 = kappa*( RX(0,0)*RX(1,0) + axiCoeff*RX(0,1)*RX(1,1) )*2.;
	    kappa22 = kappa*( RX(1,0)*RX(1,0) + axiCoeff*RX(1,1)*RX(1,1) );
	  }
	  
          imLambda = max( abs(a1)*(1./dr1)+abs(b1)*(1./dr2) );
          reLambda = max(  kappa11 *(4./(dr1*dr1)) 
			   +abs(kappa12)*(1./(dr1*dr2))
			   +kappa22 *(4./(dr2*dr2)) );

	}
	else
	{
          imLambda = max( abs(a1)*(1./dr1)+abs(b1)*(1./dr2) );
          reLambda =0.;

	}
      } // end curvilinear grid
    }
    else // =================== 3D ============================
    {
      if( mg.isRectangular() )
      {
	// ***** 3D rectangular grid *****
	real dx[3];
	mg.getDeltaX(dx);
    
	if( variableAdvection )
	  imLambda = (max(fabs(advectVarLocal(I1,I2,I3,0)))*(1./dx[0])+
		      max(fabs(advectVarLocal(I1,I2,I3,1)))*(1./dx[1])+
		      max(fabs(advectVarLocal(I1,I2,I3,2)))*(1./dx[2]) );
	else
	  imLambda = fabs(a)*(1./(dx[0])) + fabs(b)*(1./dx[1]) + fabs(c)*(1./dx[2]);

	if( !gridIsImplicit )
	{
	  if( variableDiffusivity )
	    reLambda = max(kappaVarLocal(I1,I2,I3))*( 4./(dx[0]*dx[0])+4./(dx[1]*dx[1])+4./(dx[2]*dx[2]) );
	  else
	    reLambda = kappa *( 4./(dx[0]*dx[0])+4./(dx[1]*dx[1])+4./(dx[2]*dx[2]) );
	}

      }
      else
      {
	// ***** 3D curvilinear grid *****
	mg.update(MappedGrid::THEinverseVertexDerivative);  // make sure the jacobian derivatives are built
	// define an alias:
	realMappedGridFunction & rxd = mg.inverseVertexDerivative();
	OV_GET_SERIAL_ARRAY(real,rxd,rx);

	const int nd=mg.numberOfDimensions();
        #undef MN
        #define MN(m,n) ((m)+nd*(n))
        #undef RX
        #define RX(m,n) rx(I1,I2,I3,MN(m,n))

	// Grid spacings on unit square:
	real dr1 = mg.gridSpacing(axis1);
	real dr2 = mg.gridSpacing(axis2);
	real dr3 = mg.gridSpacing(axis3);

        realSerialArray a1(I1,I2,I3),b1(I1,I2,I3),c1(I1,I2,I3);
	if( !adjustForMovingGrids )
	{
	  if( variableAdvection )
	  {
	    a1 = advectVarLocal(I1,I2,I3,0)*RX(0,0) + advectVarLocal(I1,I2,I3,1)*RX(0,1) + advectVarLocal(I1,I2,I3,2)*RX(0,2);
	    b1 = advectVarLocal(I1,I2,I3,0)*RX(1,0) + advectVarLocal(I1,I2,I3,1)*RX(1,1) + advectVarLocal(I1,I2,I3,2)*RX(1,2);
	    c1 = advectVarLocal(I1,I2,I3,0)*RX(2,0) + advectVarLocal(I1,I2,I3,1)*RX(2,1) + advectVarLocal(I1,I2,I3,2)*RX(2,2);
	  }
	  else
	  {
	    a1 = a*RX(0,0) + b*RX(0,1) + c*RX(0,2);
	    b1 = a*RX(1,0) + b*RX(1,1) + c*RX(1,2);
	    c1 = a*RX(2,0) + b*RX(2,1) + c*RX(2,2);
	  }
		  
	}
	else // adjust for moving grids
	{
	  if( variableAdvection )
	  {
	    a1 = (advectVarLocal(I1,I2,I3,0)-gvLocal(I1,I2,I3,0))*RX(0,0) + 
	      (advectVarLocal(I1,I2,I3,1)-gvLocal(I1,I2,I3,1))*RX(0,1)+ 
	      (advectVarLocal(I1,I2,I3,2)-gvLocal(I1,I2,I3,2))*RX(0,2);
	    b1 = (advectVarLocal(I1,I2,I3,0)-gvLocal(I1,I2,I3,0))*RX(1,0) + 
	      (advectVarLocal(I1,I2,I3,1)-gvLocal(I1,I2,I3,1))*RX(1,1)+ 
	      (advectVarLocal(I1,I2,I3,2)-gvLocal(I1,I2,I3,2))*RX(1,2);
	    c1 = (advectVarLocal(I1,I2,I3,0)-gvLocal(I1,I2,I3,0))*RX(2,0) + 
	      (advectVarLocal(I1,I2,I3,1)-gvLocal(I1,I2,I3,1))*RX(2,1)+
	      (advectVarLocal(I1,I2,I3,2)-gvLocal(I1,I2,I3,2))*RX(2,2);
	  }
	  else
	  {
	    a1 = (a-gvLocal(I1,I2,I3,0))*RX(0,0) + (b-gvLocal(I1,I2,I3,1))*RX(0,1)+ (c-gvLocal(I1,I2,I3,2))*RX(0,2);
	    b1 = (a-gvLocal(I1,I2,I3,0))*RX(1,0) + (b-gvLocal(I1,I2,I3,1))*RX(1,1)+ (c-gvLocal(I1,I2,I3,2))*RX(1,2);
	    c1 = (a-gvLocal(I1,I2,I3,0))*RX(2,0) + (b-gvLocal(I1,I2,I3,1))*RX(2,1)+ (c-gvLocal(I1,I2,I3,2))*RX(2,2);
	  }
	  
	}

	if((kappa>0. || variableDiffusivity ) && !gridIsImplicit )
	{
	  realSerialArray rxx(I1,I2,I3), ryy(I1,I2,I3), rzz(I1,I2,I3);
	  realSerialArray sxx(I1,I2,I3), syy(I1,I2,I3), szz(I1,I2,I3);
	  realSerialArray txx(I1,I2,I3), tyy(I1,I2,I3), tzz(I1,I2,I3);

	  op.derivative(MappedGridOperators::xDerivative,rx,rxx,I1,I2,I3,MN(0,0));
	  op.derivative(MappedGridOperators::yDerivative,rx,ryy,I1,I2,I3,MN(0,1));
	  op.derivative(MappedGridOperators::zDerivative,rx,rzz,I1,I2,I3,MN(0,2));

	  op.derivative(MappedGridOperators::xDerivative,rx,sxx,I1,I2,I3,MN(1,0));
	  op.derivative(MappedGridOperators::yDerivative,rx,syy,I1,I2,I3,MN(1,1));
	  op.derivative(MappedGridOperators::zDerivative,rx,szz,I1,I2,I3,MN(1,2));

	  op.derivative(MappedGridOperators::xDerivative,rx,txx,I1,I2,I3,MN(2,0));
	  op.derivative(MappedGridOperators::yDerivative,rx,tyy,I1,I2,I3,MN(2,1));
	  op.derivative(MappedGridOperators::zDerivative,rx,tzz,I1,I2,I3,MN(2,2));

	  if( variableDiffusivity )
	  {
	    imLambda=max(abs(a1 - kappaVarLocal(I1,I2,I3)*( rxx+ryy+rzz ) )*(1./(dr1)) +
			 abs(b1 - kappaVarLocal(I1,I2,I3)*( sxx+syy+szz ) )*(1./(dr2)) +
			 abs(c1 - kappaVarLocal(I1,I2,I3)*( txx+tyy+tzz ) )*(1./(dr3)) );

	    reLambda=max( (
			    ( RX(0,0)*RX(0,0)+
			      RX(0,1)*RX(0,1)+
			      RX(0,2)*RX(0,2) )*(4./(dr1*dr1)) +
			    ( RX(1,0)*RX(1,0)+
			      RX(1,1)*RX(1,1)+
			      RX(1,2)*RX(1,2) )*(4./(dr2*dr2)) +
			    ( RX(2,0)*RX(2,0)+
			      RX(2,1)*RX(2,1)+
			      RX(2,2)*RX(2,2) )*(4./(dr3*dr3)) +
			    abs( RX(0,0)*RX(1,0)+
				 RX(0,1)*RX(1,1)+
				 RX(0,2)*RX(1,2) )*(2.*(1./(dr1*dr2)))+
			    abs( RX(0,0)*RX(2,0)+
				 RX(0,1)*RX(2,1)+
				 RX(0,2)*RX(2,2) )*(2.*(1./(dr1*dr3))) +
			    abs( RX(1,0)*RX(2,0)+
				 RX(1,1)*RX(2,1)+
				 RX(1,2)*RX(2,2) )*(2.*(1./(dr2*dr3)))
			    )*kappaVarLocal(I1,I2,I3)
	      );
	  }
	  else
	  {
	    imLambda=max(abs(a1 - kappa*( rxx+ryy+rzz ) )*(1./(dr1)) +
			 abs(b1 - kappa*( sxx+syy+szz ) )*(1./(dr2)) +
			 abs(c1 - kappa*( txx+tyy+tzz ) )*(1./(dr3)) );

	    reLambda=max( ( RX(0,0)*RX(0,0)+
			    RX(0,1)*RX(0,1)+
			    RX(0,2)*RX(0,2) )*(kappa*4./(dr1*dr1)) +
			  ( RX(1,0)*RX(1,0)+
			    RX(1,1)*RX(1,1)+
			    RX(1,2)*RX(1,2) )*(kappa*4./(dr2*dr2)) +
			  ( RX(2,0)*RX(2,0)+
			    RX(2,1)*RX(2,1)+
			    RX(2,2)*RX(2,2) )*(kappa*4./(dr3*dr3)) +
			  abs( RX(0,0)*RX(1,0)+
			       RX(0,1)*RX(1,1)+
			       RX(0,2)*RX(1,2) )*(kappa*2.*(1./(dr1*dr2)))+
			  abs( RX(0,0)*RX(2,0)+
			       RX(0,1)*RX(2,1)+
			       RX(0,2)*RX(2,2) )*(kappa*2.*(1./(dr1*dr3))) +
			  abs( RX(1,0)*RX(2,0)+
			       RX(1,1)*RX(2,1)+
			       RX(1,2)*RX(2,2) )*(kappa*2.*(1./(dr2*dr3))) );
	  }
	  
	}
	
	else
	{
	  imLambda=max(abs(a1)*(1./(dr1)) +
		       abs(b1)*(1./(dr2)) +
		       abs(c1)*(1./(dr3)) );
	    
	  reLambda=0.;
	}
	
    
      } // end curvilinear grid

    } // end 3D
    
  } // end if ok 
  
  imLambda= ParallelUtility::getMaxValue(imLambda); 
  reLambda= ParallelUtility::getMaxValue(reLambda); 

  // these are used for AMR:
  realPartOfTimeSteppingEigenvalue      = reLambda;
  imaginaryPartOfTimeSteppingEigenvalue = imLambda;

  if( debug() & 4 )
    printF(" ***** Cgad: getTimeSteppingEigenvalue: grid=%i (im,re)=(%9.3e,%9.3e) ****\n",grid,imLambda,reLambda);
  
}


