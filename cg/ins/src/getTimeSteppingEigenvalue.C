// ==============================================================================
//    Get the time stepping eigenvalues (that determine dt) 
//    for the INS and related equations 
// ==============================================================================

#include "Cgins.h"
#include "Parameters.h"
#include "MappedGridOperators.h"
#include "display.h"
#include "ParallelUtility.h"

#include "turbulenceModels.h"
#include "turbulenceParameters.h"

#include "GridMaterialProperties.h"

#define insdts EXTERN_C_NAME(insdts)
extern "C"
{
 void insdts(const int&nd,
      const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
      const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
      const int&nd4a,const int&nd4b,
      const int&mask, const real& xy, const real& rx, const real& u, const real& uu, const real&gv,  
      const real & dw, const real & divDamp, const real & dtVar,
      const int & ndMatProp, const int& matIndex, const real& matValpc, const real& matVal,
      const int&bc, const int&ipar, const real&rpar, const DataBase *pdb, const int&ierr );
}


//\begin{>>MappedGridSolverInclude.tex}{\subsection{getTimeSteppingEigenvalue}} 
void Cgins::
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
  const InsParameters::PDEModel & pdeModel = parameters.dbase.get<InsParameters::PDEModel>("pdeModel");

  #ifdef USE_PPP
    realSerialArray u0Local; getLocalArrayWithGhostBoundaries(u0,u0Local);
  #else  
    const realSerialArray & u0Local = u0;
  #endif

  Index I1,I2,I3;
  // getIndex( mg.extendedIndexRange(),I1,I2,I3);

  getIndex(mg.gridIndexRange(),I1,I2,I3);  
  bool ok = ParallelUtility::getLocalArrayBounds(u0,u0Local,I1,I2,I3);  

  if( ok ) // there are pts on this processor
  {

    MappedGridOperators & op = *u0.getOperators();

    const int isRectangular=op.isRectangular(); // trouble when moving ??
    // const int isRectangular=mg.isRectangular(); 
  
    real nu = parameters.dbase.get<real >("nu");
    // real kThermal = parameters.dbase.get<real >("kThermal");

    if( parameters.dbase.get<bool >("advectPassiveScalar") ) 
      nu=max(nu,parameters.dbase.get<real >("nuPassiveScalar"));   // could do better than this
//     if( pdeModel==InsParameters::BoussinesqModel )
//     {
//       nu=max(nu,kThermal);   // could do better than this
//     }
    

    // only apply fourth-order AD here if it is explicit
    const bool useFourthOrderArtificialDiffusion = parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") &&
      !parameters.dbase.get<bool >("useImplicitFourthArtificialDiffusion");

    const real adcPassiveScalar=1.; // coeff or linear artificial diffusion for the passive scalar ** add to params

    const bool & gridIsMoving = parameters.gridIsMoving(grid);
    const int gridIsImplicit=parameters.getGridIsImplicit(grid);

    int useWhereMask=true;
    real dx[3]={1.,1.,1.};
    real xab[2][3]={0.,1.,0.,1.,0.,1.};
    if( isRectangular )
      mg.getRectangularGridParameters( dx, xab );
   
    //kkc 100216 fix for testing compact ops    const int orderOfAccuracy=parameters.dbase.get<int >("orderOfAccuracy");
    const int orderOfAccuracy=min(4,parameters.dbase.get<int >("orderOfAccuracy"));
    const int gridType= isRectangular ? 0 : 1;


    // For non-moving grids u==uu, otherwise uu is a temp space to hold (u-gv)
    realSerialArray uu;
    if( parameters.gridIsMoving(grid) )
    {
      // fix this : uu only needs to be dimensioned to Ru -- needed to pass uu bounds to insdts
      // uu.redim(u0Local.dimension(0),u0Local.dimension(1),u0Local.dimension(2),parameters.dbase.get<Range >("Ru")); 
      uu.redim(u0Local.dimension(0),u0Local.dimension(1),u0Local.dimension(2),u0Local.dimension(3)); 
    }
#ifdef USE_PPP
    const real *pu = u0Local.getDataPointer();
    const real *puu = parameters.gridIsMoving(grid) ? uu.getDataPointer() : pu;
    const real *pdw = parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")==NULL ? pu : 
      ((*parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"))[grid]).getLocalArray().getDataPointer();

    const real *pDivDamp = divDampingWeight[grid].getLocalArray().getDataPointer();
    const real *pVariableDt = pdtVar !=NULL ? (*pdtVar)[grid].getLocalArray().getDataPointer() : pDivDamp;
  
    // For now we need the center array for the axisymmetric case:
    const real *pxy = parameters.isAxisymmetric() ? mg.center().getLocalArray().getDataPointer() : pu;
    const real *prsxy = isRectangular ? pu :  mg.inverseVertexDerivative().getLocalArray().getDataPointer();
    const int *pmask = mg.mask().getLocalArray().getDataPointer();
    const real *pgv = gridIsMoving ? gridVelocity.getLocalArray().getDataPointer() : pu;
#else  
    const real *pu = u0.getDataPointer();
    const real *puu = parameters.gridIsMoving(grid) ? uu.getDataPointer() : pu;
  
    const real *pdw = parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")==NULL ? pu : 
      ((*parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"))[grid]).getDataPointer();
  
    const real *pDivDamp = divDampingWeight[grid].getDataPointer();
    const real *pVariableDt = pdtVar !=NULL ? (*pdtVar)[grid].getDataPointer() : pDivDamp;
  
    // For now we need the center array for the axisymmetric case:
    if( parameters.isAxisymmetric() ) 
    {
      assert( mg.center().getLength(0)>0 );
    }
    const real *pxy = parameters.isAxisymmetric() ? mg.center().getDataPointer() : pu;
    const real *prsxy = isRectangular ? pu :  mg.inverseVertexDerivative().getDataPointer();
    const int *pmask = mg.mask().getDataPointer();
    const real *pgv = gridIsMoving ? gridVelocity.getDataPointer() : pu;
#endif

    // --- Variable material properies ---
    GridMaterialProperties::MaterialFormatEnum materialFormat = GridMaterialProperties::constantMaterialProperties;
    int ndMatProp=1;  // for piecewise constant materials, this is the leading dimension of the matVal array
    int *matIndexPtr=(int*)pmask;   // if not used, point to mask
    real*matValPtr=(real*)pu;       // if not used, point to u
    if( parameters.dbase.get<int>("variableMaterialPropertiesOption")!=0 )
    {
      // Material properties do vary 

      // printF("Cgins::getTimeSteppingEigenvalue: Material properties do vary\n");

      std::vector<GridMaterialProperties> & materialProperties = 
	parameters.dbase.get<std::vector<GridMaterialProperties> >("materialProperties");

      GridMaterialProperties & matProp = materialProperties[grid];
      materialFormat = matProp.getMaterialFormat();
      
      if( materialFormat==GridMaterialProperties::piecewiseConstantMaterialProperties )
      {
	IntegerArray & matIndex = matProp.getMaterialIndexArray();
        matIndexPtr = matIndex.getDataPointer();
      }
      
      RealArray & matVal = matProp.getMaterialValuesArray();
      matValPtr = matVal.getDataPointer();
      ndMatProp = matVal.getLength(0);  

      // ::display(matVal,"Cgins::getUt: matVal");
    }

    // declare and lookup visco-plastic parameters (macro)
    // declareViscoPlasticParameters;

    int i1a=mg.gridIndexRange(0,0);
    int i2a=mg.gridIndexRange(0,1);
    int i3a=mg.gridIndexRange(0,2);
    int ipar[] ={parameters.dbase.get<int >("pc"),                // 0 
		 parameters.dbase.get<int >("uc"),
		 parameters.dbase.get<int >("vc"),
		 parameters.dbase.get<int >("wc"),
		 parameters.dbase.get<int >("kc"),
		 parameters.dbase.get<int >("sc"),
		 grid,
		 orderOfAccuracy,
		 (int)parameters.gridIsMoving(grid),
		 useWhereMask,
		 (int)gridIsImplicit, // parameters.gridIsImplicit(grid),        // 10
		 (int)parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod"),
		 (int)parameters.dbase.get<Parameters::ImplicitOption >("implicitOption"),
		 (int)parameters.isAxisymmetric(),
		 (int)parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion"),
		 (int)useFourthOrderArtificialDiffusion,
		 (int)parameters.dbase.get<bool >("advectPassiveScalar"),
		 gridType,
		 parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel"),
		 (int)parameters.dbase.get<int >("useLocalTimeStepping"),
		 i1a,i2a,i3a,                 // 20,21,22
		 (int)parameters.dbase.get<InsParameters::PDEModel >("pdeModel"),
                 (int)parameters.dbase.get<InsParameters::ImplicitVariation>("implicitVariation"),
                 parameters.dbase.get<int >("rc"),  // ipar[25]
                 materialFormat
    };

    reLambda=0.;
    imLambda=0.;

    const real yEps=sqrt(REAL_EPSILON);  // tol for axisymmetric *** fix this ***
    real rpar[]={mg.gridSpacing(0),mg.gridSpacing(1),mg.gridSpacing(2),
		 dx[0],dx[1],dx[2],   // 3,4,5
		 nu,
		 parameters.dbase.get<real >("ad21"),
		 parameters.dbase.get<real >("ad22"),
		 parameters.dbase.get<real >("ad41"),
		 parameters.dbase.get<real >("ad42"),
		 parameters.dbase.get<real >("nuPassiveScalar"),  // 11
		 adcPassiveScalar,
		 reLambda,
		 imLambda,
		 parameters.dbase.get<real >("cDt"),
		 parameters.dbase.get<real >("cdv"),
		 parameters.dbase.get<real >("dtMax"),
		 parameters.dbase.get<real >("ad21n"),
		 parameters.dbase.get<real >("ad22n"),
		 parameters.dbase.get<real >("ad41n"),  // 20
		 parameters.dbase.get<real >("ad42n"),
		 xab[0][0],xab[0][1],xab[0][2],yEps 
                 };

    int ierr=0;
    DataBase *pdb = &parameters.dbase;
    
    insdts(mg.numberOfDimensions(),
	   I1.getBase(),I1.getBound(),
	   I2.getBase(),I2.getBound(),
	   I3.getBase(),I3.getBound(),
	   u0Local.getBase(0),u0Local.getBound(0),u0Local.getBase(1),u0Local.getBound(1),
	   u0Local.getBase(2),u0Local.getBound(2),u0Local.getBase(3),u0Local.getBound(3),
	   *pmask, *pxy, *prsxy,
	   *pu, *puu, *pgv, *pdw, *pDivDamp, *pVariableDt,
           ndMatProp,*matIndexPtr,*matValPtr,*matValPtr,
	   mg.boundaryCondition(0,0), ipar[0], rpar[0], pdb, ierr );

    reLambda=rpar[13];
    imLambda=rpar[14];
  }
  else
  {
    reLambda=0.;
    imLambda=0.;
  }
    
  if( debug() & 4 ) 
  {
    printF(">>>>>>>>>>>>Cgins::insdts: NEW: (reLambda,imLambda)=(%9.3e,%9.3e) (p=%i) hMin=%e\n",
	   reLambda,imLambda,parameters.dbase.get<int >("myid"),hMin[grid]); 
     
//      ::display(divergenceDampingWeight(),sPrintF(" insdts: divergenceDampingWeight() grid=%i ",grid),
// 	       parameters.dbase.get<FILE* >("debugFile"));
     
  }
    
  return;

}


