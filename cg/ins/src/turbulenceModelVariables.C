// ====================================================================================================================
//  This file contains functions that evaluate turbulence model variables
//
// ====================================================================================================================

#include "Cgins.h"
#include "InsParameters.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"

#define getBaldwinLomaxViscosity EXTERN_C_NAME(getbaldwinlomaxviscosity)
#define getKEpsilonViscosity EXTERN_C_NAME(getkepsilonviscosity)
#define getLargeEddySimulationViscosity EXTERN_C_NAME(getlargeeddysimulationviscosity)

#define insLineSetupNew EXTERN_C_NAME(inslinesetupnew)
#define insLineSolveBC EXTERN_C_NAME(inslinesolvebc)
#define computeResidual EXTERN_C_NAME(computeresidual)
#define computeResidualNew EXTERN_C_NAME(computeresidualnew)
extern "C"
{
void getBaldwinLomaxViscosity(const int&nd,
		  const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
		  const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,
                  const int&nd3b,const int&nd4a,const int&nd4b,
		  const int&mask,const real&rx, const real&xy,  const real&u, const real&v, const real&dw,
                  const int&bc, const int&boundaryCondition, 
                  const int&ipar, const real&rpar, const DataBase *pdb, const int&ierr );
void getKEpsilonViscosity(const int&nd,
		  const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
		  const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,
                  const int&nd3b,const int&nd4a,const int&nd4b,
		  const int&mask,const real&rx, const real&xy,  const real&u, const real&v, const real&dw,
                  const int&bc, const int&boundaryCondition, 
                  const int&ipar, const real&rpar, const DataBase *pdb, const int&ierr );

void getLargeEddySimulationViscosity(const int&nd,
		  const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
		  const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,
                  const int&nd3b,const int&nd4a,const int&nd4b,
		  const int&mask,const real&rx, const real&xy,  const real&u, const real&v, const real&dw,
                  const int&bc, const int&boundaryCondition, 
                  const int&ipar, const real&rpar, const DataBase *pdb, const int&ierr );
}

// void
// getLocalBoundsAndBoundaryConditions( const realMappedGridFunction & a, 
//                                      IntegerArray & gidLocal, 
//                                      IntegerArray & dimensionLocal, 
//                                      IntegerArray & bcLocal );



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


// ===================================================================================================================
/// \brief Define variables for turbulence models
/// \details This routine is used to compute the coefficient of viscosity some turbulence models
/// \param name (input) : evaluate this quantity: "viscosity"
/// \param cgf (input) : use this solution 
/// \param r (output) : save results here.
/// \param component (input) : save results in this component of "r".
// ==================================================================================================================
int InsParameters::
getTurbulenceModelVariables( const aString & name, const GridFunction & cgf, realCompositeGridFunction & r, 
                             const int component )
{
  const Parameters::TurbulenceModel & turbulenceModel = dbase.get<Parameters::TurbulenceModel >("turbulenceModel");

  const CompositeGrid & cg = cgf.cg;
  const realCompositeGridFunction & u = cgf.u;
  
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    getTurbulenceModelVariables( name,u[grid],r[grid],grid,component,cgf.t );
  }

  // we optionally interpolate the variable here in case we need values at interpolation points.
  if( name=="viscosity" )
  {
    // we could smooth the viscosity here 

    
    // No need to interp the k-Epsilon nuT since it is an algebaric expression and well defined at interp points.
    const bool allPointsEvaluated = (turbulenceModel==Parameters::kEpsilon ||
				     turbulenceModel==Parameters::kOmega  );
    if( !allPointsEvaluated )
    {
      r.interpolate(Range(component,component));
    }
    
  }
  

  return 0;
}

int InsParameters::
getTurbulenceModelVariables( const aString & name, const realMappedGridFunction & u, realMappedGridFunction & v,
                             const int grid,
                             const int component, 
                             const real t )
{
  InsParameters & parameters = *this;
  
  const Parameters::TurbulenceModel & turbulenceModel = 
                 parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel");

  // evaluate variables for the visco-plastic model

  MappedGrid & mg = *v.getMappedGrid();

#ifdef USE_PPP
  intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
  realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v,vLocal);
  realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
#else
  const intSerialArray & maskLocal = mg.mask();
  realSerialArray & vLocal = v;
  const realSerialArray & uLocal = u;
#endif


//   const int uc=dbase.get<int >("uc");
//   const int vc=dbase.get<int >("vc");
//   const int wc=dbase.get<int >("wc");
//   const int tc=dbase.get<int >("tc");

  Index I1,I2,I3;
  int extra=0;  
  // We can assign all points directly for kEpsilon since the viscosity is an algebraic expression.
  const bool evaluateAllPoints = (turbulenceModel==Parameters::kEpsilon ||
				  turbulenceModel==Parameters::kOmega );
  if( evaluateAllPoints )
  {
    getIndex(mg.dimension(),I1,I2,I3,extra); 
  }
  else
    getIndex(mg.gridIndexRange(),I1,I2,I3,extra); 
      
  const int includeGhost=1;
  bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);

  const int isRectangular=mg.isRectangular();
  int useWhereMask=true;
  real dx[3]={1.,1.,1.};
  real xab[2][3]={0.,1.,0.,1.,0.,1.};
  if( isRectangular )
    mg.getRectangularGridParameters( dx, xab );
   
  const int orderOfAccuracy=parameters.dbase.get<int >("orderOfAccuracy");
  const int gridType= isRectangular ? 0 : 1;
  bool & twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");

  Range all;
  int n=component;  // put result into this component
  if( name=="viscosity" )
  {
    if( ok )
    {
      const IntegerArray & bc = mg.boundaryCondition();
      IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
      ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( u,gidLocal,dimLocal,bcLocal );  

      real *pu=uLocal.getDataPointer();
      const realArray & xy = twilightZoneFlow ? mg.center() : u;
#ifdef USE_PPP
      const real *pxy = xy.getLocalArray().getDataPointer();
      const real *pdw = parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")==NULL ? pu : 
	((*parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"))[grid]).getLocalArray().getDataPointer();
      const real *prsxy = isRectangular ? pu :  mg.inverseVertexDerivative().getLocalArray().getDataPointer();
#else
      const real *pxy = xy.getDataPointer();
      const real *pdw = parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")==NULL ? pu : 
	((*parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"))[grid]).getDataPointer();
      const real *prsxy = isRectangular ? pu :  mg.inverseVertexDerivative().getDataPointer();
#endif

      DataBase *pdb = &parameters.dbase;

      int ipar[60];
      ipar[0]=n;
      ipar[1]=grid;
      ipar[2]=gridType;
      ipar[3]=orderOfAccuracy;
      ipar[4]=useWhereMask;
      ipar[5]=(int)turbulenceModel;
      ipar[6]=(int)twilightZoneFlow;
      ipar[7]=(int)parameters.dbase.get<InsParameters::PDEModel >("pdeModel");

      real rpar[]={dx[0],
                   dx[1],
                   dx[2],
                   mg.gridSpacing(0),
                   mg.gridSpacing(1),
                   mg.gridSpacing(2),
                   t };


      if( turbulenceModel==Parameters::BaldwinLomax )
      {
        assert( parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")!=NULL );

	const int nTrip=50;
	ipar[nTrip]=ipar[nTrip+1]=ipar[nTrip+2]=-1; // turbulence trip location, i,j,k
        IntegerArray & turbulenceTripPoint = parameters.dbase.get<IntegerArray >("turbulenceTripPoint");
	if( turbulenceTripPoint.getLength(0) )
	{
	  for ( int i=0; i<mg.numberOfDimensions(); i++ )
	    ipar[nTrip+i] = turbulenceTripPoint(i+1);
	}
	

	// note: results are saved in v(i1,i2,i3,n) 
	int ierr=0;
	getBaldwinLomaxViscosity(
	  mg.numberOfDimensions(),
	  I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
	  uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1), 
	  uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3), 
	  *maskLocal.getDataPointer(), *prsxy, *pxy, *pu, *vLocal.getDataPointer(), *pdw,
	  bc(0,0), bcLocal(0,0),ipar[0],rpar[0], pdb, ierr );
      
      }
      else if( turbulenceModel==Parameters::kEpsilon )
      {
	// note: results are saved in v(i1,i2,i3,n) 
        // Range all; 
	// vLocal(all,all,all,Range(n,n))=1.;  // initialize all values -- to remove UMR's
	

	int ierr=0;
	getKEpsilonViscosity(
	  mg.numberOfDimensions(),
	  I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
	  uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1), 
	  uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3), 
	  *maskLocal.getDataPointer(), *prsxy, *pxy, *pu, *vLocal.getDataPointer(), *pdw,
	  bc(0,0), bcLocal(0,0),ipar[0],rpar[0], pdb, ierr );
      
	// ::display(v(I1,I2,I3,n),"viscosity, After getKEpsilonViscosity","%5.2f ");
	
      }
      else if( turbulenceModel==Parameters::LargeEddySimulation )
      {
 	// note: results are saved in v(i1,i2,i3,n) 
	int ierr=0;
	getLargeEddySimulationViscosity(
	  mg.numberOfDimensions(),
	  I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
	  uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1), 
	  uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3), 
	  *maskLocal.getDataPointer(), *prsxy, *pxy, *pu, *vLocal.getDataPointer(), *pdw,
	  bc(0,0), bcLocal(0,0),ipar[0],rpar[0], pdb, ierr );
      }
      else
      {
	printF("getTurbulenceModelVariables:ERROR: unknown turbulenceModel=%i\n",turbulenceModel);
	Overture::abort("error");
      }

    } // end if ok 
    
    // now get values on ghost points by extrapolation
    if( !evaluateAllPoints )
    {
      BoundaryConditionParameters extrapParams;
      extrapParams.orderOfExtrapolation=2;
      real t=0.;
      v.applyBoundaryCondition(Range(n,n),BCTypes::extrapolate,BCTypes::allBoundaries,0.,t,extrapParams);

      if( parameters.dbase.get<int >("orderOfAccuracy")==4 )
      {
        // extrap second ghost line too
	extrapParams.ghostLineToAssign=2;
	v.applyBoundaryCondition(Range(n,n),BCTypes::extrapolate,BCTypes::allBoundaries,0.,t,extrapParams);
      }
      else if( parameters.dbase.get<int >("orderOfAccuracy")!=2 )
      {
        printF("getTurbulenceModelVariables:ERROR: finish me for orderOfAccuracy=%i\n",
	       parameters.dbase.get<int >("orderOfAccuracy"));
	OV_ABORT("error");
      }

      v.finishBoundaryConditions(extrapParams,Range(n,n));  // get corners
      
    }

    n++;  // increment for next component to be saved
  }
    

  return 0;
}
