// ==========================================================================
//   Add forcinging to the advection-diffusion equations
// ==========================================================================

#include "Cgad.h"
#include "Parameters.h"
#include "display.h"
#include "ParallelUtility.h"
#include "EquationDomain.h"


//\begin{>>MappedGridSolverInclude.tex}{\subsection{addForcing}}
void Cgad::
addForcing(realMappedGridFunction & dvdt, const realMappedGridFunction & u, 
	   int iparam[], real rparam[],
	   realMappedGridFunction & dvdtImplicit /* = Overture::nullRealMappedGridFunction() */,
           realMappedGridFunction *referenceFrameVelocity /* =NULL */ )
//========================================================================================================
// /Description:
//   Add the forcing term to $u_t$ on a component grid for the advection-diffusion equations
// Here is where we added the analytic derivatives for twilight-zone flow.
//
// /mg (input) : grid
// /dvdt (intput/output) : return $u_t$ in this grid function.
// /t (input) : current time.
// /grid (input) : the component grid number if this MappedGrid is part of a GridCollection or CompositeGrid.
// /dvdtImplicit (input) : for implicit time stepping, the time derivative is split into two parts,
//     $u_t=u_t^E + u_t^I$. The explicit part, $u_t^E$, is returned in dvdt while the implicit part, $u_t^I$,
//   is returned in dvdtImplicit. This splitting does NOT depend on whether we are using backward Euler or
//   Crank-Nicolson since this weighting is applied elsewhere. 
// /tImplicit (input) : for implicit time stepping, apply forcing for the implicit part at his 
//     time.
// 
// Implicit time-stepping notes: 
// ----------------------------
//    Suppose we are solving the PDE:
//           u_t = f(u,x,t)  + F(x,t)
//   that we have split into an explicit part, fe(u),  and implicit part, A*u:
//           u_t = fe(u) + A u  + F(x,t)
//
//   Let the Twilight-zone function be ue(t). 
//   If the time stepping method is implicit then we compute
//           dvdt += F(x,t) - fe(ue(t))
//   When implicitOption==computeImplicitTermsSeparately we also compute:
//           dvdtImplicit +=  -[ alpha*A*ue(tImplicit) + (1-alpha)*A*ue(t) ] 
//   where alpha is the implicit factor (= .5 for Crank-Nicolson)
//   (if implicitOption==doNotComputeImplicitTerms then do not change dvdtImplicit).
// 
//
//\end{MappedGridSolverInclude.tex}  
//=======================================================================================================
{
  real cpu0=getCPU();

  if( !parameters.dbase.get<bool >("twilightZoneFlow") && !parameters.dbase.get<bool >("turnOnBodyForcing") )
  {
    // No forcing to add 
    return;
  }

 
  if( debug() & 8 )
  {
    printF("Cgad::addForcing: ...\n");
  }

  // *wdh* 081207 MappedGrid & mg = *dvdt.getMappedGrid();
  MappedGrid & mg = *u.getMappedGrid();   // use this for moving grid cases since dvdt may not have the correct grid
  
  const real & t0=rparam[0];
  real t         =rparam[1];          // this is realy tForce
  const real & tImplicit=rparam[2];
  const int & grid = iparam[0];
  const int level=iparam[1];
  const int numberOfStepsTaken = iparam[2];
  const bool gridIsMoving = parameters.gridIsMoving(grid);

  // The TZ forcing includes terms for a moving reference frame. **finish me ***
  const Parameters::ReferenceFrameEnum referenceFrame = parameters.getReferenceFrame();
  const bool adjustForReferenceFrame = gridIsMoving && referenceFrame==Parameters::rigidBodyReferenceFrame;
  if( adjustForReferenceFrame )
  {
    assert( referenceFrameVelocity!=NULL );
  }
  
  const int numberOfDimensions=mg.numberOfDimensions();
  const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents")-parameters.dbase.get<int >("numberOfExtraVariables");

  FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
  FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

  realArray & ut = dvdt;
  OV_GET_SERIAL_ARRAY(real,ut,utLocal);

  Index I1,I2,I3;
  // --- Add on any user defined forcing ---
  if( parameters.dbase.get<bool >("turnOnBodyForcing") )
  {
    // The body forcing will have already been computed.

    assert( parameters.dbase.get<realCompositeGridFunction* >("bodyForce")!=NULL );
    realCompositeGridFunction & bodyForce = *(parameters.dbase.get<realCompositeGridFunction* >("bodyForce"));

    // Add the user defined force onto dvdt:
    OV_GET_SERIAL_ARRAY(real,bodyForce[grid],bodyForceLocal);

    // printF("addForcing: add body force: (min,max)=(%g,%g)\n",min(bodyForceLocal),max(bodyForceLocal));

    getIndex(mg.gridIndexRange(),I1,I2,I3);
    bool ok = ParallelUtility::getLocalArrayBounds(ut,utLocal,I1,I2,I3); 
    
    const Range & Rt = parameters.dbase.get<Range >("Rt");       // time dependent components

    utLocal(I1,I2,I3,Rt) += bodyForceLocal(I1,I2,I3,Rt);

  }
  



  if( parameters.dbase.get<bool >("twilightZoneFlow") )
  {
    // ---add forcing for twlight-zone flow---

    const Parameters::ImplicitOption & implicitOption = parameters.dbase.get<Parameters::ImplicitOption >("implicitOption");
    const real implicitFactor = parameters.dbase.get<real >("implicitFactor");
    
    realArray & uti = dvdtImplicit;

    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));

    Index I1,I2,I3;

    #ifdef USE_PPP
      realSerialArray utiLocal; getLocalArrayWithGhostBoundaries(uti,utiLocal);
      // intSerialArray mask; getLocalArrayWithGhostBoundaries(mg.mask(),mask);
      realSerialArray rfLocal; 
      if( adjustForReferenceFrame ) getLocalArrayWithGhostBoundaries(*referenceFrameVelocity,rfLocal);
    #else
      const realSerialArray & utiLocal = uti;
      const realSerialArray & rfLocal = adjustForReferenceFrame ? *referenceFrameVelocity : utLocal; 
    #endif  

    const bool isRectangular = false; // ** do this for now ** mg.isRectangular();

    const bool vertexNeeded = !isRectangular || parameters.isAxisymmetric();
    if( vertexNeeded )
      mg.update(MappedGrid::THEcenter);

    realArray & x= mg.center();
    #ifdef USE_PPP
      realSerialArray xLocal;  if( vertexNeeded ) getLocalArrayWithGhostBoundaries(x,xLocal);
    #else
      const realSerialArray & xLocal = x;
    #endif


    int extra=1;
    getIndex(extendedGridIndexRange(mg),I1,I2,I3,extra); 
    bool ok = ParallelUtility::getLocalArrayBounds(ut,utLocal,I1,I2,I3); 

    if( ok )
    {
      std::vector<real> & kappa = parameters.dbase.get<std::vector<real> >("kappa");
      std::vector<real> & a = parameters.dbase.get<std::vector<real> >("a");
      std::vector<real> & b = parameters.dbase.get<std::vector<real> >("b");
      std::vector<real> & c = parameters.dbase.get<std::vector<real> >("c");    
      bool firstDerivNeeded=adjustForReferenceFrame || parameters.isAxisymmetric();
      for( int m=0; m<numberOfComponents && !firstDerivNeeded; m++ )
      {
	firstDerivNeeded= firstDerivNeeded || a[m]!=0. || b[m]!=0. || c[m]!=0.;
      }

      realSerialArray ut(I1,I2,I3), ux,uy,uz;
      realSerialArray uxx(I1,I2,I3),uyy(I1,I2,I3),uzz;

      if( firstDerivNeeded )
      {
	ux.redim(I1,I2,I3); uy.redim(I1,I2,I3); 
      }
    
      if( mg.numberOfDimensions()==2 )
      {

        RealArray radiusInverse;
	if( parameters.isAxisymmetric() )
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
	      if( mg.boundaryCondition(side,axis)==Parameters::axisymmetric )  // we should use bcLocal here 
	      {
		getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
		bool ok = ParallelUtility::getLocalArrayBounds(dvdt,utLocal,Ib1,Ib2,Ib3);
		if( ok )
		{
		  radiusInverse(Ib1,Ib2,Ib3)=0.;
		}
	      }
	    }
	  }
	}
	
	if( firstDerivNeeded )
	{
	  for( int m=0; m<numberOfComponents; m++ )
	  {
	    e.gd( ut ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,m,t);
	    e.gd( ux ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,m,t);
	    e.gd( uy ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,m,t);

	    e.gd( uxx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,m,t);
	    e.gd( uyy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,m,t);

	    utLocal(I1,I2,I3,m)+= (ut(I1,I2,I3) + a[m]*ux(I1,I2,I3)+b[m]*uy(I1,I2,I3) 
				   -kappa[m]*(uxx(I1,I2,I3)+uyy(I1,I2,I3)) );

	    if( adjustForReferenceFrame )
	    {
	      // ::display(rfLocal,"addForcing: reference frame velocity","%5.2f ");
	    
	      utLocal(I1,I2,I3,m)+= rfLocal(I1,I2,I3,0)*ux(I1,I2,I3)+rfLocal(I1,I2,I3,1)*uy(I1,I2,I3);
	    }
            if( parameters.isAxisymmetric() )
	    {
              utLocal(I1,I2,I3,m)+= -kappa[m]*( uy(I1,I2,I3)*radiusInverse(I1,I2,I3) );
              where( radiusInverse==0. )
	      {
                utLocal(I1,I2,I3,m)+= -kappa[m]*( uyy(I1,I2,I3) );
	      }
	    }
	    
	  }
	}
	else
	{
	  for( int m=0; m<numberOfComponents; m++ )
	  {
	    e.gd( ut ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,m,t);
	    e.gd( uxx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,m,t);
	    e.gd( uyy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,m,t);

	    utLocal(I1,I2,I3,m)+=ut(I1,I2,I3) - kappa[m]*(uxx(I1,I2,I3)+uyy(I1,I2,I3));

	  }
	}
      
      }
      else // 3D 
      {
	uzz.redim(I1,I2,I3);
	if( firstDerivNeeded )
	{
	  uz.redim(I1,I2,I3);

	  for( int m=0; m<numberOfComponents; m++ )
	  {
	    e.gd( ut ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,m,t);
	    e.gd( ux ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,m,t);
	    e.gd( uy ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,m,t);
	    e.gd( uz ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,m,t);

	    e.gd( uxx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,m,t);
	    e.gd( uyy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,m,t);
	    e.gd( uzz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,m,t);

	    utLocal(I1,I2,I3,m)+= ( ut(I1,I2,I3) + a[m]*ux(I1,I2,I3)+b[m]*uy(I1,I2,I3)+c[m]*uz(I1,I2,I3)-
				    kappa[m]*(uxx(I1,I2,I3)+uyy(I1,I2,I3)+uzz(I1,I2,I3)) );

	    if( adjustForReferenceFrame )
	    {
	      utLocal(I1,I2,I3,m)+= (rfLocal(I1,I2,I3,0)*ux(I1,I2,I3)+
				     rfLocal(I1,I2,I3,1)*uy(I1,I2,I3)+
				     rfLocal(I1,I2,I3,2)*uz(I1,I2,I3));
	    }
	  }
	}
	else
	{
	  for( int m=0; m<numberOfComponents; m++ )
	  {
	    e.gd( ut ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,m,t);
	    e.gd( uxx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,m,t);
	    e.gd( uyy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,m,t);
	    e.gd( uzz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,m,t);

	    utLocal(I1,I2,I3,m)+=ut(I1,I2,I3) - kappa[m]*(uxx(I1,I2,I3)+uyy(I1,I2,I3)+uzz(I1,I2,I3));

	  }
	}
      
      }

    }

    
  } //end if twilightZone
  
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForForcing"))+=getCPU()-cpu0;
}
