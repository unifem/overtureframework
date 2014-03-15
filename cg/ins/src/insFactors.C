#include "InsParameters.h"
#include "TridiagonalSolver.h"
#include "insFactors.h"
#include "CompositeGrid.h"
#include "GridFunction.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "PlotIt.h"
#include "AdamsPCData.h"

#include "kkcdefs.h"

                                    
// Macros that wrap fortran subroutine names
#define ins_rfactor EXTERN_C_NAME(ins_rfactor)
#define ins_rrfactor EXTERN_C_NAME(ins_rrfactor)
#define ins_diagfactor EXTERN_C_NAME(ins_diagfactor)
#define ins_mfactor EXTERN_C_NAME(ins_mfactor)
#define ins_mfactor_opt EXTERN_C_NAME(ins_mfactor_opt)
#define ins_fscoeff EXTERN_C_NAME(ins_fscoeff)
#define ins_evalux EXTERN_C_NAME(ins_evalux)

// Macros and an extern "C" block that declare the fortran subroutines
#define DEFINE_AF_FACTOR_SUBROUTINE(NAME) void NAME(const int &nd, \
						    const int &nd1a,const int &nd1b,\
						    const int &nd2a,const int &nd2b,\
						    const int &nd3a,const int &nd3b,\
						    const int &nd4a,const int &nd4b,\
						    const int &mask, \
						    const real &rsxy, const real &u, const real &ul, const real &gv,  \
						    const int &bc, const int &boundaryCondition, \
						    const int &ndbcd1a,const int &ndbcd1b,\
						    const int &ndbcd2a,const int &ndbcd2b,\
						    const int &ndbcd3a,const int &ndbcd3b,\
						    const int &ndbcd4a,const int &ndbcd4b,\
						    const real &bcData, \
						    const int &ipar, const real &rpar, DataBase *pdb, \
						    const int &mode, const int&dir, const int &component,\
						    real &a, real &b, real &c, real &d, real &e, real &rhs, int &ierr )

#define SETUP_PARAMETER_ARRAYS 	\
          ArraySimpleFixed<int,60,1,1,1>  ipar; \
	  ArraySimpleFixed<real,60,1,1,1> rpar; \
          \
  ipar[0] = I1.getBase()+extra[0];	  \
  ipar[1] = I1.getBound()-extra[0];	  \
  ipar[2] = I2.getBase()+extra[1];	  \
  ipar[3] = I2.getBound()-extra[1];	  \
  ipar[4] = I3.getBase()+extra[2];	  \
  ipar[5] = I3.getBound()-extra[2];	  \
          \
	  ipar[6] = parameters->dbase.get<int >("pc");\
	  ipar[7] = parameters->dbase.get<int>("uc");\
	  ipar[8] = parameters->dbase.get<int>("vc");\
	  ipar[9] = parameters->dbase.get<int>("wc");\
	  ipar[10]= parameters->dbase.get<int >("kc");\
	  ipar[11]= parameters->dbase.get<int >("sc");\
	  ipar[12]= parameters->dbase.get<int >("tc");\
          \
	  ipar[13]= grid;\
	  ipar[14]= parameters->dbase.get<int >("orderOfAccuracy");\
	  ipar[15]= parameters->gridIsMoving(grid);\
	  ipar[16]= discrete_approximation;\
	  ipar[26]= isRectangular ? 1 : 0;\
          ipar[27]= isPeriodic;\
          ipar[38]=I1.getBase();\
          ipar[39]=I1.getBound();\
          ipar[40]=I2.getBase();\
          ipar[41]=I2.getBound();\
          ipar[42]=I3.getBase();\
          ipar[43]=I3.getBound();\
	  ipar[48]=parameters->dbase.get<int >("debug");\
          ipar[49]=(int)parameters->dbase.get<InsParameters::PDEModel >("pdeModel");\
          ipar[50]=parameters->dbase.get<bool >("useSecondOrderArtificialDiffusion"); \
          ipar[51]=parameters->dbase.get<bool >("useFourthOrderArtificialDiffusion"); \
          ipar[52]=isParallel;\
          ipar[53]=parameters->dbase.get<bool >("useBoundaryDissipationInAFScheme");\
          ipar[54]=mg.getMinimumNumberOfDistributedGhostLines();\
          ipar[55]=rhsOnly;\
          ipar[56]=isParallel ? Communication_Manager::My_Process_Number : -1;\
          \
	  rpar[0] = mg.gridSpacing(0);\
	  rpar[1] = mg.gridSpacing(1);\
	  rpar[2] = mg.gridSpacing(2);\
	  rpar[3] = dx[0];\
	  rpar[4] = dx[1];\
	  rpar[5] = dx[2];\
	  rpar[6] = dt;\
	  rpar[8] = parameters->dbase.get<real >("nu");\
	  rpar[9]= parameters->dbase.get<real >("kThermal");\
          \
	  ArraySimpleFixed<real,3,1,1,1> & gravity = parameters->dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");\
	  rpar[20]=gravity[0];\
	  rpar[21]=gravity[1];\
	  rpar[22]=gravity[2];\
	  real thermalExpansivity=1.;\
	  parameters->dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("thermalExpansivity",thermalExpansivity);\
	  rpar[23]=thermalExpansivity;\
	  rpar[24]=parameters->dbase.get<real>("advectionCoefficient");\
          rpar[25]=parameters->dbase.get<real>("ad21");\
          rpar[26]=parameters->dbase.get<real>("ad22");\
          rpar[27]=parameters->dbase.get<real>("ad41");\
          rpar[28]=parameters->dbase.get<real>("ad42");\
           /*  pass disspation for T equation : wdh 2013/02/02 */  \
          real adcBoussinesq=0.; \
          parameters->dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("adcBoussinesq",adcBoussinesq);\
          rpar[29]=adcBoussinesq;\
	  DataBase *pdb = &parameters->dbase;
// END SETUP PARAMETER ARRAYS

#define CALL_INS_FACTOR_ROUTINE(NAME) NAME(numberOfDimensions, \
			       ug_starLocal.getBase(0), ug_starLocal.getBound(0),\
			       ug_starLocal.getBase(1), ug_starLocal.getBound(1),\
			       ug_starLocal.getBase(2), ug_starLocal.getBound(2),\
			       ug_starLocal.getBase(3), ug_starLocal.getBound(3),\
			       *maskLocal.getDataPointer(),\
			       *rsxyLocal.getDataPointer(), *ugLocal.getDataPointer(), *ug_starLocal.getDataPointer(), \
			       *gridVelocityLocal.getDataPointer(),\
			       bcLocal(0,0), mg.boundaryCondition(0,0), \
			       bcData.getBase(0),bcData.getBound(0),\
			       bcData.getBase(1),bcData.getBound(1),\
			       bcData.getBase(2),bcData.getBound(2),\
			       bcData.getBase(3),bcData.getBound(3),\
			       *bcData.getDataPointer(),\
			       ipar[0],rpar[0],pdb,\
			       mode, dir, component_to_solve_for,\
			       *dl2.getDataPointer(), *dl1.getDataPointer(), *d.getDataPointer(), \
			       *du1.getDataPointer(), *du2.getDataPointer(), \
			       *rhsLocal.getDataPointer(), ierr);
// END CALL_INS_FACTOR_ROUTINE


#define PRINT_MATRIX_ARRAYS(NAME) \
if ( parameters->dbase.get<int >("debug")>=4 ) \
		{ \
		  cout<<"INS_FACTORS " #NAME " debug :: after ins_factor["<<type<<"], dir="<<getDirection()<<", cc="<<component_to_solve_for<<" : "<<endl; \
		  if ( isPenta) dl2.display("dl2"); \
		  dl1.display("dl1"); \
		  d.display("d"); \
		  du1.display("du1"); \
		  if ( isPenta ) du2.display("du2"); \
		  rhsLocal.display("rhs"); \
		} \
  // END PRINT_MATRIX_ARRAYS

extern "C" {

  DEFINE_AF_FACTOR_SUBROUTINE(ins_rfactor);
  DEFINE_AF_FACTOR_SUBROUTINE(ins_rrfactor);
  DEFINE_AF_FACTOR_SUBROUTINE(ins_diagfactor);
  DEFINE_AF_FACTOR_SUBROUTINE(ins_mfactor);
  DEFINE_AF_FACTOR_SUBROUTINE(ins_mfactor_opt);
  DEFINE_AF_FACTOR_SUBROUTINE(ins_fscoeff);
  DEFINE_AF_FACTOR_SUBROUTINE(ins_evalux);

  typedef DEFINE_AF_FACTOR_SUBROUTINE((*INSFactorSubroutine));
}

#define IS_DIRICHLET(SIDE,AXIS) ( bcLocal(SIDE,AXIS)==Parameters::dirichletBoundaryCondition || bcLocal(SIDE,AXIS)==Parameters::noSlipWall || bcLocal(SIDE,AXIS)==InsParameters::inflowWithVelocityGiven || bcLocal(SIDE,AXIS)==InsParameters::outflow)


// here is the new, less messy, GET_BOUNDS_INFO MACRO
/* There are basically 5 cases:
   1) nonperiodic in the dir axis, serial
   2) periodic in the dir axis, serial
   3) nonperiodic in the dir axis, parallel split along dir
   4) periodic in the dir axis, parallel split along dir
   5) periodic in the dir axis, parallel but NOT split along dir
   Cases 1, 3 and 4 are basically treated the same, except we need to use the gridIndexRange in 4.
   Cases 2 and 5 are also the same.

   Note that on parallel boundaries we extend the "internal" grid points by nParallelGhost-nGhost points to overlap computations
     between processors.

   Note that I1,I2,I3 (referencing II[0:2]) hold the grid index range INCLUDING the ghost points along the dir axis.
   In the case of a periodic boundary (cases 2 and 5) we DO NOT include the ghost points because we can actually use the periodicity.
   In the directions normal to dir (grid lines parallel to dir) we need to include parts of the parallel ghost boundaries on which the internal 
     discretization overlaps between processors.
   The Diagonal_Factor operates on all dimensions, so we need to get the extra width in all directions, not just dir.
*/

//////// BEGIN GET_BOUNDS_INFO_MACRO /////////////
#define GET_BOUNDS_INFO(ARRAY,LOCAL_ARRAY,EXTRA_FACTOR)	                                                                                 			\
  Index II[3],&I1=II[0],&I2=II[1],&I3=II[2];				                                                                                        \
  int nParallelGhost =  cg[grid].getMinimumNumberOfDistributedGhostLines();                                                                                     \
  int nGhost = afParallelGhostWidth;                                                                                                                            \
  int extra[] = {0,0,0};						                                                                                        \
  const IntegerArray &dim = mg.dimension();                                                                                      				\
  /* first lets just get the index range and see if we are on a periodic boundary */                                                                            \
  getIndex(mg.indexRange(),I1,I2,I3,extra[0],extra[1],extra[2]);	                                                                                        \
  int lb1s,lb1e,lb2s,lb2e,lb3s,lb3e;					                                                                                        \
  bool have_local_points = ParallelUtility::getLocalArrayBounds(ARRAY,LOCAL_ARRAY,I1,I2,I3,                                                                     \
								lb1s,lb1e,lb2s,lb2e,lb3s,lb3e,parallel_array_bounds_option);                                    \
  IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);		                                                                                        \
  ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( ARRAY,gidLocal,dimLocal,bcLocal,CG_ApproximateFactorization::parallelBC );                     \
  /* check to see if we gotta use the gridIndexRange in the case because we have case 4 above */                                                                \
  bool isPeriodic = isReallyPeriodic(mg,dim,dimLocal,dir);                                                                                                      \
  extra[dir] = isPeriodic ? 0 : nGhost; /* now we can set the extra width */                                                                                    \
  if ( type==Diagonal_Factor ) { for ( int ida=0; ida<cg.numberOfDimensions(); ida++ )                                                                          \
     { extra[ida] = isReallyPeriodic(mg,dim,dimLocal,ida) ? 0 : nGhost;                                                                                         \
       isPeriodic = isPeriodic || isReallyPeriodic(mg,dim,dimLocal,ida);  \
     }}	                                                                                         								\
  for ( int a=0; a<cg.numberOfDimensions(); a++ ) { extra[a] *= EXTRA_FACTOR; }                                                                                 \
  const IntegerArray &indexRangeToUse = isPeriodic ? mg.indexRange() : mg.gridIndexRange();                                                                     \
  getIndex(indexRangeToUse,I1,I2,I3,extra[0],extra[1],extra[2]);	                                                                                        \
  /* do all this over again to get the actual bounds we want */                                                                                                 \
  have_local_points = ParallelUtility::getLocalArrayBounds(ARRAY,LOCAL_ARRAY,I1,I2,I3,                                                                          \
								lb1s,lb1e,lb2s,lb2e,lb3s,lb3e,parallel_array_bounds_option);                                    \
  ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( ARRAY,gidLocal,dimLocal,bcLocal,CG_ApproximateFactorization::parallelBC );                     \
  /* At this point I1,I2,I3 have the index range + ghost points everywhere except on parallel boundaries where they include all the parallel ghost points. */   \
  /* In directions normal to dir (except in Diagonal_Factor) we need to remove the extra nGhost points we don't need. */                                        \
  /* In the case of Diagonal_Factor, this has already been accounted for by setting extra on all directions equal to EXTRA_FACTOR*nGhost. */                    \
  if ( type!=Diagonal_Factor ) {                                                                                                                                \
    for ( int axis=1; axis<cg.numberOfDimensions(); axis++ )                                                                                                    \
      {                                                                                                                                                         \
	int na = (dir+axis)%cg.numberOfDimensions();                                                                                                            \
	if ( bcLocal(0,na)==CG_ApproximateFactorization::parallelBC ) { II[na] = Range(II[na].getBase()+nGhost,II[na].getBound());}                             \
	if ( bcLocal(1,na)==CG_ApproximateFactorization::parallelBC ) { II[na] = Range(II[na].getBase(),II[na].getBound()-nGhost);}                             \
      }                                                                                                                                                         \
  }                        								                                                                 	\
  else /*if(false)*/ {							\
    for ( int na=0; na<cg.numberOfDimensions(); na++ )                                                                               {                          \
	if ( bcLocal(0,na)==CG_ApproximateFactorization::parallelBC ) { II[na] = Range(II[na].getBase()+nGhost,II[na].getBound());}                             \
	if ( bcLocal(1,na)==CG_ApproximateFactorization::parallelBC ) { II[na] = Range(II[na].getBase(),II[na].getBound()-nGhost);}                             \
}}	


//////// END GET_BOUNDS_INFO_MACRO /////////////



#define ZERO_GHOST(LOCAL_ARRAY) \
  for ( int gl=1; !isPeriodic && axis==getDirection() && gl<=cg[grid].numberOfGhostPoints(side,axis); gl++ ) \
 {\  
  Index Ig1,Ig2,Ig3;\
  getGhostIndex(gidLocal,side,axis,Ig1,Ig2,Ig3,gl); \
  LOCAL_ARRAY(Ig1,Ig2,Ig3) = 0.0; \
 } 

// The following macro is used to determine if we need to rebuild and refactor the tridiagonal matrices.
// If we are solving for a velocity component then we only need to build and factor once for each direction.
// For the temperature (and passive scalars with their own dissipation) we need to rebuild and refactor.
#define REBUILD_AND_REFACTOR(CC) (((CC)==uc) || (tc>wc && CC>=tc))

using namespace CGINS_ApproximateFactorization;
using namespace std;

//#define EXTRAP_NORMAL_DIRS
#define USE_MFACTOR_OPT

namespace {
  

#ifdef USE_COMBINED_FACTORS
  bool use_merged_factors = true;
#else
  bool use_merged_factors = false;
#endif

  bool forceNonRectangular = false;

  bool implicit_freestream_correction = false;

  INSFactorSubroutine ins_factor[] = { ins_rfactor,
				       ins_rrfactor,
				       ins_diagfactor,
#ifdef USE_MFACTOR_OPT
				       ins_mfactor_opt
#else
                                       ins_mfactor
#endif
  };

  int parallel_array_bounds_option = 1;
  Range all;

#ifndef USE_PPP
  int isParallel = 0;
#else
  int isParallel = 1;
#endif

  inline bool isReallyPeriodic(const MappedGrid &mg, const IntegerArray &dim, const IntegerArray &dimLocal, const int &dir )
  {
    return ( mg.isPeriodic(dir)!=Mapping::notPeriodic &&	
      dim(0,dir)==dimLocal(0,dir) && dim(1,dir)==dimLocal(1,dir) ); 
  }
}

CGINS_ApproximateFactorization::
INS_Factor::
INS_Factor(const int dir, const CGINS_ApproximateFactorization::FactorTypes t, const InsParameters &parameters_) : Factor(dir), type(t), parameters(&parameters_) 
{
  if ( type==R_Factor ) 
    name = "r factor";
  else if ( type==RR_Factor )
    name = "rr factor";
  else if ( type==Diagonal_Factor )
    name = "diagonal factor";
  else if ( type==Merged_Factor )
    name = "merged factor";
}

CGINS_ApproximateFactorization::
INS_Factor::
~INS_Factor(){}

void
CGINS_ApproximateFactorization::
INS_Factor::
solveRightHandSide(const real &dt, const GridFunction &u, GridFunction &u_star)
{
  // We need to solve something that looks like:
  // U^{**} = (I-A)U^{*}
  // and then set
  // U^{*} <-- U^{**} 
  // for the next factor.  Note that A is a function of the argument u.

  InsFactorModes mode = solveRHS;
  const int dir = getDirection();

  const CompositeGrid &cgs = u_star.cg;
  const CompositeGrid &cg = u.cg;
  int numberOfDimensions = cg.numberOfDimensions();
  int rhsOnly = 0;

  InsParameters::DiscretizationOptions discrete_approximation = parameters->dbase.get<InsParameters::DiscretizationOptions>("discretizationOption");

  int afParallelGhostWidth = parameters->dbase.get<int>("AFparallelGhostWidth");

  int uc = parameters->dbase.get<int>("uc");
  int vc = parameters->dbase.get<int>("vc");
  int wc = parameters->dbase.get<int>("wc");
  int tc = parameters->dbase.get<int>("tc");
  if ( cg.numberOfDimensions()==2 ) wc=vc;
  if (  parameters->dbase.get<InsParameters::PDEModel >("pdeModel")!=InsParameters::BoussinesqModel ) tc = wc;

  if ( parameters->dbase.get<int >("debug")>=64 )
    {
      u.u.display("solveRHS : u");
      u_star.u.display("solveRHS : u_star");
    }

  for ( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      TridiagonalSolver tri;
      MappedGrid &mg = (MappedGrid &)cg[grid];
      MappedGrid &mgs= (MappedGrid &)cgs[grid];
      realMappedGridFunction &ug = u.u[grid];
      realMappedGridFunction &ug_star = u_star.u[grid];
      
      //      realMappedGridFunction rhs(mg,all,all,all);
      //      rhs.updateToMatchGridFunction(ug_star);
      realMappedGridFunction &gridVelocity = parameters->gridIsMoving(grid) ? ((GridFunction &)u).getGridVelocity(grid) : u.u[grid];

      const bool isRectangular = mg.isRectangular() && !forceNonRectangular;
      real dx[3]={1.,1.,1.};
      if( isRectangular )
	mg.getDeltaX(dx);
      else
	((MappedGrid &)mg).update(MappedGrid::THEinverseVertexDerivative);
      
      OV_GET_LOCAL_ARRAY(real,ug);
      OV_GET_LOCAL_ARRAY(real,ug_star); // get the local array for ug_star...
      // ...and check to see if there are points on this processor

      GET_BOUNDS_INFO(ug_star,ug_starLocal,1);

      if ( have_local_points ) 
	{

	  // adjust the indices so that they are appropriate for the boundary conditions
#ifndef USE_COMBINED_FACTORS
	  int extra[] = {0,0,0};
	  if (!isPeriodic && !use_merged_factors) extra[getDirection()] = 1;
	  getIndex(mg.indexRange(),I1,I2,I3,extra[0],extra[1],extra[2]); /// ???!!!! would this affect getLocalArrayBounds ???!!!
#endif

	  //x	  if ( bcLocal(0,dir)==Parameters::interpolation || bcLocal(1,dir)==CG_ApproximateFactorization::parallelBC ) II[dir] = Range(II[dir].getBase()-nGhost,II[dir].getBound());
	  //x	  if ( bcLocal(1,dir)==Parameters::interpolation || bcLocal(1,dir)==CG_ApproximateFactorization::parallelBC ) II[dir] = Range(II[dir].getBase(),II[dir].getBound()+nGhost);

	  // get local views of the rest of the stuff we need
	  RealArray rhsLocal(I1,I2,I3),fsCoeff;
	  rhsLocal(I1,I2,I3) = 0.;
	  // *wdh* 100227 OV_GET_LOCAL_ARRAY(real,rhs);
	  OV_GET_LOCAL_ARRAY(real,gridVelocity);
	  OV_GET_LOCAL_ARRAY_FROM(int,mask,mgs.mask());  
	  OV_GET_LOCAL_ARRAY_CONDITIONAL(real,rsxy,isRectangular,ugLocal,mg.inverseVertexDerivative());
	  const RealArray & bcData = parameters->dbase.get<RealArray>("bcData");
  
	  RealArray dl1(I1,I2,I3);
	  RealArray   d(I1,I2,I3);
	  RealArray du1(I1,I2,I3);
	  RealArray dl2,du2;
	  //	  RealArray dl1(I1,I2,I3),d(I1,I2,I3),du1(I1,I2,I3),dl2,du2;
	  bool isPenta = ( (parameters->dbase.get<int >("orderOfAccuracy")==4 && discrete_approximation==InsParameters::standardFiniteDifference) ||
			   (parameters->dbase.get<int >("orderOfAccuracy")>4 && discrete_approximation==InsParameters::compactDifference && !use_merged_factors) ||
			   (parameters->dbase.get<int >("orderOfAccuracy")>2 && discrete_approximation==InsParameters::compactDifference && use_merged_factors));
	  if ( isPenta )
	    {
	      dl2.redim(I1,I2,I3);
	      du2.redim(I1,I2,I3);
	    }
	  else
	    {
	      dl2.reference(dl1);
	      du2.reference(du1);
	    }

	  if ( parameters->dbase.get<int >("debug")>=4 )
	    {
	      cout<<"INS_FACTORS solveRHS debug :: ug_starLocal before solve"<<endl;
	      ug_starLocal.display("u_starLocal");
	    }

	  if ( implicit_freestream_correction && type==Diagonal_Factor )
	    {
	      fsCoeff.redim(I1,I2,I3);
	      if ( !isRectangular ) 
		{
		  fsCoeff(I1,I2,I3) = 0;
		  for ( int dir=0; dir<cg.numberOfDimensions(); dir++ ) // !!! local version of variable dir!!! 
		    {
		      int component_to_solve_for = -1;
		      SETUP_PARAMETER_ARRAYS;

		      int ierr=0;
#define ug_starLocal ugLocal
		      CALL_INS_FACTOR_ROUTINE(ins_fscoeff);
#undef ug_starLocal
		  
		      if ( parameters->dbase.get<int >("debug")>=4 )
			{
			  cout<<"INS_FACTORS getRHS debug :: after freestream correction ins_factor["<<type<<"], dir="<<dir<<", cc="<<component_to_solve_for<<" : "<<endl;
			  if ( isPenta) dl2.display("dl2");
			  dl1.display("dl1");
			  d.display("d");
			  du1.display("du1");
			  if ( isPenta ) du2.display("du2");
			  rhsLocal.display("rhs");
			}
		      
		      TridiagonalSolver tri;
		      TridiagonalSolver::SystemType tri_type = isPeriodic ? TridiagonalSolver::periodic : TridiagonalSolver::extended;
		      if ( isPenta )
			tri.factor(dl2,dl1,d,du1,du2,tri_type,dir);
		      else
			tri.factor(dl1,d,du1,tri_type,dir);
		      
		      tri.solve(rhsLocal);
		      if ( parameters->dbase.get<int >("debug")>=4 )
			{
			  cout<<"INS_FACTORS getRHS debug :: after solve for freestream correction, ins_factor["<<type<<"], dir="<<dir<<", cc="<<component_to_solve_for<<" : "<<endl;
			  rhsLocal.display("rhs");
			}
		      fsCoeff(I1,I2,I3) += rhsLocal(I1,I2,I3);

		  //	      cout<<"max fscoeff["<<dir<<"] = "<<max(fabs(fsCoeff))<<endl;
		      rhsLocal(I1,I2,I3) = 0.;
		    }// dir loop

		  for (int axis=0; axis<mg.numberOfDimensions(); axis++ )
		    for ( int side=0; side<2; side++ )
		      {
			if ( IS_DIRICHLET(side,axis) || bcLocal(side,axis)==Parameters::neumannBoundaryCondition )
			  {
			    Index Ib1,Ib2,Ib3,Ig1,Ig2,Ig3;
			    getBoundaryIndex(gidLocal,side,axis,Ib1,Ib2,Ib3);
			    fsCoeff(Ib1,Ib2,Ib3) = 0;
			  }

		      }
		}
	      else
		{
		  fsCoeff(I1,I2,I3) = 0.;
		}
	    }

	  //!!! in the following loop we assume the components are ordered pc,uc,vc,wc,tc with:
	  //    tc = wc if not boussinesq
	  //    wc = vc if 2D
	  for ( int component_to_solve_for=uc; component_to_solve_for<=tc; component_to_solve_for++ )
	    {

	      if ( type!=Diagonal_Factor || !implicit_freestream_correction )
		{
		  // the following line is a macro defined at the top of this file
		  rhsOnly = !(REBUILD_AND_REFACTOR(component_to_solve_for));
		  SETUP_PARAMETER_ARRAYS;

		  // call the fortran subroutine that fills in the diagonals
		  int ierr = 0;
		  CALL_INS_FACTOR_ROUTINE(ins_factor[type]);

		  if ( parameters->dbase.get<int >("debug")>=4 )
		    {
		      cout<<"INS_FACTORS solveRHS debug :: after ins_factor["<<type<<"], dir="<<getDirection()<<", cc="<<component_to_solve_for<<" : "<<endl;
		      if ( isPenta ) dl2.display("dl2");
		      dl1.display("dl1");
		      d.display("d");
		      du1.display("du1");
		      if (isPenta ) du2.display("du2");
		      rhsLocal.display("rhs");
		      
		    }

		  if ( discrete_approximation==InsParameters::standardFiniteDifference )
		    {
		      // then the system is trivial and we can do the division here
		      
		      //		  		  rhsLocal(I1,I2,I3) = rhsLocal(I1,I2,I3)/d(I1,I2,I3);
#if 1
		      OV_APP_TO_PTR_3D(real,rhsLocal, rp);
		      OV_APP_TO_PTR_3D(real,d,dp);
		      for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
			for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
			  for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
			    {
			      A_3D(rp,i1,i2,i3) = A_3D(rp,i1,i2,i3)/A_3D(dp,i1,i2,i3);
			    }
#endif
		    }
		  else if ( discrete_approximation==InsParameters::compactDifference )
		    { 
		      // the system is tri or penta diagonal so we must actually solve it
		      if ( REBUILD_AND_REFACTOR(component_to_solve_for) )
			{
			  //		      TridiagonalSolver tri;
			  TridiagonalSolver::SystemType tri_type = isPeriodic ? TridiagonalSolver::periodic : TridiagonalSolver::extended;
			  if ( isPenta )
			    tri.factor(dl2,dl1,d,du1,du2,tri_type,dir);
			  else
			    tri.factor(dl1,d,du1,tri_type,dir);
			}

		      tri.solve(rhsLocal);
		    }
		  else
		    {
		      Overture::abort("unknown discrete approximation option given to INS_ApproximateFactorization::INS_Factor");
		    }
		} 
	      else if ( implicit_freestream_correction )
		{
		  real dto2 = dt/2.;
		  OV_APP_TO_PTR_3D(real,rhsLocal, rp);
		  OV_APP_TO_PTR_3D(real,fsCoeff, fsp);
		  OV_APP_TO_PTR_4D(real,ug_starLocal, up);
		  for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
		    for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
		      for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
			A_3D(rp,i1,i2,i3) = (1.+dto2*A_3D(fsp,i1,i2,i3))*A_4D(up,i1,i2,i3,component_to_solve_for);
		}

	      if ( parameters->dbase.get<int >("debug")>=4 )
		{
		  cout<<"INS_FACTORS solveRHS debug :: after solve"<<endl;
		  rhsLocal.display("solution (rhsLocal array)");
		}

	      OV_APP_TO_PTR_3D(real,rhsLocal, rp);
	      OV_APP_TO_PTR_4D(real,ug_starLocal, uslp);
	      for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
		for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
		  for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
		    A_4D(uslp,i1,i2,i3,component_to_solve_for) = A_3D(rp,i1,i2,i3);
	    } // for each component to solve for
	} // if have_local_points

      // extrapolate the temporary variable into the ghosts
      for ( int d=1; d<cg.numberOfDimensions(); d++ )
	{
	  int td = (getDirection()+d)%cg.numberOfDimensions();
	  Range C(uc,tc);
	  BoundaryConditionParameters extrapParams;
	  extrapParams.orderOfExtrapolation=parameters->dbase.get<int >("orderOfAccuracy")+1;
	  extrapParams.ghostLineToAssign=1;
	  #ifdef EXTRAP_NORMAL_DIRS
	  ug_star.applyBoundaryCondition(C,BCTypes::extrapolate,BCTypes::allBoundaries,0.,u.t,extrapParams);
	  extrapParams.ghostLineToAssign=2;
	  ug_star.applyBoundaryCondition(C,BCTypes::extrapolate,BCTypes::allBoundaries,0.,u.t,extrapParams);
	  #endif
	}

      ug_star.periodicUpdate();
      ug_star.finishBoundaryConditions();
      //      PlotIt::contour(*Overture::getGraphicsInterface(),ug_star);
    } // for each grid
}

void
CGINS_ApproximateFactorization::
INS_Factor::
solveLeftHandSide(const real &dt, const GridFunction &u, GridFunction &u_star)
{
  // We need to solve something that looks like:
  // (I+A)U^{**} = U^{*}
  // and then set
  // U^{*} <-- U^{**} 
  // for the next factor.  Note that A is a function of the argument u.
  InsFactorModes mode = solveLHS;
  const int dir = getDirection();

  CompositeGrid &cgs = u_star.cg;
  CompositeGrid &cg = u_star.cg;
  int numberOfDimensions = cg.numberOfDimensions();
  int rhsOnly = 0;

  InsParameters::DiscretizationOptions discrete_approximation = parameters->dbase.get<InsParameters::DiscretizationOptions>("discretizationOption");

  int afParallelGhostWidth = parameters->dbase.get<int>("AFparallelGhostWidth");
  int uc = parameters->dbase.get<int>("uc");
  int vc = parameters->dbase.get<int>("vc");
  int wc = parameters->dbase.get<int>("wc");
  int tc = parameters->dbase.get<int>("tc");
  if ( cg.numberOfDimensions()==2 ) wc=vc;
  if (  parameters->dbase.get<InsParameters::PDEModel >("pdeModel")!=InsParameters::BoussinesqModel ) tc = wc;

  if ( parameters->dbase.get<int >("debug")>=64 )
    {
      u.u.display("solveLHS : u");
      u_star.u.display("solveLHS : u_star");
    }

  for ( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      TridiagonalSolver tri;
      MappedGrid &mg = cg[grid];
      MappedGrid &mgs= (MappedGrid &)cgs[grid];

      realMappedGridFunction &ug = u.u[grid];
      realMappedGridFunction &ug_star = u_star.u[grid];
      
      //      realMappedGridFunction rhs(mg,all,all,all);
      //      rhs.updateToMatchGridFunction(ug_star);
      realMappedGridFunction &gridVelocity = parameters->gridIsMoving(grid) ? ((GridFunction &)u_star).getGridVelocity(grid) : u.u[grid];

      const bool isRectangular = mg.isRectangular() && !forceNonRectangular;
      real dx[3]={1.,1.,1.};
      if( isRectangular )
	mg.getDeltaX(dx);
      else
	mgs.update(MappedGrid::THEinverseVertexDerivative);
      
      OV_GET_LOCAL_ARRAY(real,ug);
      OV_GET_LOCAL_ARRAY(real,ug_star); // get the local array for ug_star...
      // ...and check to see if there are points on this processor
      GET_BOUNDS_INFO(ug_star,ug_starLocal,1);

      if ( have_local_points ) 
	{

	  // adjust the indices so that they are appropriate for the boundary conditions
#ifndef USE_COMBINED_FACTORS
	  int extra[] = {0,0,0};
	  if (!isPeriodic && !use_merged_factors) extra[getDirection()] = 1;
	  getIndex(mg.indexRange(),I1,I2,I3,extra[0],extra[1],extra[2]); /// ???!!!! would this affect getLocalArrayBounds ???!!!
#endif

	  //x	  if ( bcLocal(0,dir)==Parameters::interpolation || bcLocal(0,dir)==CG_ApproximateFactorization::parallelBC ) II[dir] = Range(II[dir].getBase()-nGhost,II[dir].getBound());
	  //x	  if ( bcLocal(1,dir)==Parameters::interpolation || bcLocal(1,dir)==CG_ApproximateFactorization::parallelBC ) II[dir] = Range(II[dir].getBase(),II[dir].getBound()+nGhost);

	  // get local views of the rest of the stuff we need
	  RealArray rhsLocal(I1,I2,I3),fsCoeff;
	  rhsLocal(I1,I2,I3) = 0.;
	  // *wdh* 100227 OV_GET_LOCAL_ARRAY(real,rhs);
	  OV_GET_LOCAL_ARRAY(real,gridVelocity);
	  OV_GET_LOCAL_ARRAY_FROM(int,mask,mgs.mask());
	  OV_GET_LOCAL_ARRAY_CONDITIONAL(real,rsxy,isRectangular,ugLocal,mgs.inverseVertexDerivative());
	  const RealArray & bcData = parameters->dbase.get<RealArray>("bcData");
// 	  IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
// 	  // NOTE: bcLocal(side,axis) == -1 for internal boundaries between processors
// 	  ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( ug_star,gidLocal,dimLocal,bcLocal ); // call this to get bcLocal

// 	  // the following isPeriodic check should probably sit in some parallel utility
// 	  const IntegerArray &dim = mg.dimension();
// 	  bool isPeriodic = false;
// 	  isPeriodic = ( mg.isPeriodic(dir)!=Mapping::notPeriodic && 
// 			 dim(0,dir)==dimLocal(0,dir) && dim(1,dir)==dimLocal(1,dir) ); //!!! this will need to change for lines split by parallelism
  
	  RealArray dl1(I1,I2,I3);
	  RealArray   d(I1,I2,I3);
	  RealArray du1(I1,I2,I3);
	  RealArray dl2,du2;

	  bool isPenta = ( (parameters->dbase.get<int >("orderOfAccuracy")==4 && discrete_approximation==InsParameters::standardFiniteDifference) ||
			   (parameters->dbase.get<int >("orderOfAccuracy")>4 && discrete_approximation==InsParameters::compactDifference && !use_merged_factors) ||
			   (parameters->dbase.get<int >("orderOfAccuracy")>2 && discrete_approximation==InsParameters::compactDifference && use_merged_factors));

	  if ( isPenta )
	    {
	      dl2.redim(I1,I2,I3);
	      du2.redim(I1,I2,I3);
	    }
	  else
	    {
	      dl2.reference(dl1);
	      du2.reference(du1);
	    }

	  if ( parameters->dbase.get<int >("debug")>=4 )
	    {
	      cout<<"INS_FACTORS solveLHS debug :: ug_starLocal before solve"<<endl;
	      ug_starLocal.display("u_starLocal");
	    }

	  if ( implicit_freestream_correction && type==Diagonal_Factor )
	    {
	      fsCoeff.redim(I1,I2,I3);
	      if ( !isRectangular ) 
		{
		  fsCoeff(I1,I2,I3) = 0;
		  for ( int dir=0; dir<cg.numberOfDimensions(); dir++ ) // !!! local version of variable dir!!! 
		    {
		      int component_to_solve_for = -1;
		      SETUP_PARAMETER_ARRAYS;

		      int ierr=0;
#define ug_starLocal ugLocal
		      CALL_INS_FACTOR_ROUTINE(ins_fscoeff);
#undef ug_starLocal
		  
		      if ( parameters->dbase.get<int >("debug")>=4 )
			{
			  cout<<"INS_FACTORS getLHS debug :: after freestream correction ins_factor["<<type<<"], dir="<<getDirection()<<", cc="<<component_to_solve_for<<" : "<<endl;
			  if ( isPenta) dl2.display("dl2");
			  dl1.display("dl1");
			  d.display("d");
			  du1.display("du1");
			  if ( isPenta ) du2.display("du2");
			  rhsLocal.display("rhs");
			}
		      
		      TridiagonalSolver tri;
		      TridiagonalSolver::SystemType tri_type = isPeriodic ? TridiagonalSolver::periodic : TridiagonalSolver::extended;
		      if ( isPenta )
			tri.factor(dl2,dl1,d,du1,du2,tri_type,dir);
		      else
			tri.factor(dl1,d,du1,tri_type,dir);
		      
		      tri.solve(rhsLocal);
		      if ( parameters->dbase.get<int >("debug")>=4 )
			{
			  cout<<"INS_FACTORS solveLHS debug :: after solve for freestream correction, ins_factor["<<type<<"], dir="<<getDirection()<<", cc="<<component_to_solve_for<<" : "<<endl;
			  rhsLocal.display("rhs");
			}
		      fsCoeff(I1,I2,I3) += rhsLocal(I1,I2,I3);

		  //	      cout<<"max fscoeff["<<dir<<"] = "<<max(fabs(fsCoeff))<<endl;
		      rhsLocal(I1,I2,I3) = 0.;
		    }// dir loop
		  for (int axis=0; axis<mg.numberOfDimensions(); axis++ )
		    for ( int side=0; side<2; side++ )
		      {
			if ( IS_DIRICHLET(side,axis) || bcLocal(side,axis)==Parameters::neumannBoundaryCondition )
			  {
			    Index Ib1,Ib2,Ib3,Ig1,Ig2,Ig3;
			    getBoundaryIndex(gidLocal,side,axis,Ib1,Ib2,Ib3);
			    fsCoeff(Ib1,Ib2,Ib3) = 0;
			  }

		      }
		}
	      else
		{
		  fsCoeff(I1,I2,I3) = 0.;
		}
              #ifndef USE_PPP
// 	      if ( false )
// 		{
// 		  cout<<"PLOTTING IN LHS, "<<type<<", "<<dir<<endl;
// 		  realMappedGridFunction tmp(mg);
// 		  tmp(I1,I2,I3) = fsCoeff(I1,I2,I3);
// 		  tmp.periodicUpdate();
// 		  PlotIt::contour(*Overture::getGraphicsInterface(),tmp);
// 		}
              #endif
	    }

	  //!!! in the following loop we assume the components are ordered pc,uc,vc,wc,tc with:
	  //    tc = wc if not boussinesq
	  //    wc = vc if 2D
	  for ( int component_to_solve_for=uc; component_to_solve_for<=tc; component_to_solve_for++ )
	    {
	      if ( type!=Diagonal_Factor || !implicit_freestream_correction )
		{
		  // the following line is a macro defined at the top of this file
		  rhsOnly = !(REBUILD_AND_REFACTOR(component_to_solve_for));
		  SETUP_PARAMETER_ARRAYS;
		  
		  // call the fortran subroutine that fills in the diagonals
		  int ierr = 0;
		  CALL_INS_FACTOR_ROUTINE(ins_factor[type]);
		  
		  if ( parameters->dbase.get<int >("debug")>=4 )
		    {
		      cout<<"INS_FACTORS solveLHS debug :: after ins_factor["<<type<<"], dir="<<getDirection()<<", cc="<<component_to_solve_for<<" : "<<endl;
		      if (isPenta ) display(dl2,"dl2","%9.2e ");
		      display(dl1,"dl1","%9.2e ");
		      display(d,"d","%9.2e ");
		      display(du1,"du1","%9.2e ");
		      if (isPenta ) display(du2,"du2","%9.2e ");
		      display(rhsLocal,"rhs","%9.2e ");
		    }
		  
		  if ( type!=Diagonal_Factor)
		    {
		      if ( REBUILD_AND_REFACTOR(component_to_solve_for) )
			{
			  //		      TridiagonalSolver tri;
			  TridiagonalSolver::SystemType tri_type = isPeriodic ? TridiagonalSolver::periodic : TridiagonalSolver::extended;
			  if ( isPenta )
			    tri.factor(dl2,dl1,d,du1,du2,tri_type,dir);
			  else
			    tri.factor(dl1,d,du1,tri_type,dir);
			}

		      tri.solve(rhsLocal);
		    }
		  else
		    { // diagonal factors are easy to invert... :-)
		      
		      //rhsLocal(I1,I2,I3) = rhsLocal(I1,I2,I3)/b(I1,I2,I3);
		      OV_APP_TO_PTR_3D(real,rhsLocal, rp);
		      OV_APP_TO_PTR_3D(real,d,dp);
		      for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
			for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
			  for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
			    A_3D(rp,i1,i2,i3) = A_3D(rp,i1,i2,i3)/A_3D(dp,i1,i2,i3);
		    }
		}
	      else if ( implicit_freestream_correction )
		{
		  real dto2 = dt/2.;
		  OV_APP_TO_PTR_3D(real,rhsLocal, rp);
		  OV_APP_TO_PTR_3D(real,fsCoeff, fsp);
		  OV_APP_TO_PTR_4D(real,ug_starLocal, up);
		  for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
		    for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
		      for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
			{
			  A_3D(rp,i1,i2,i3) = A_4D(up,i1,i2,i3,component_to_solve_for)/(1.-dto2*A_3D(fsp,i1,i2,i3));
			}
		}

	      if ( parameters->dbase.get<int >("debug")>=4 )
		{
		  cout<<"INS_FACTORS solveLHS debug :: after solve"<<endl;
		  rhsLocal.display("solution (rhsLocal array)");
		}

	      OV_APP_TO_PTR_3D(real,rhsLocal, rp);
	      OV_APP_TO_PTR_4D(real,ug_starLocal, uslp);
	      for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
		for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
		  for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
		    {
		      A_4D(uslp,i1,i2,i3,component_to_solve_for) = A_3D(rp,i1,i2,i3);
		    }

	      if ( parameters->dbase.get<int >("debug")>=4 )
		{
		  cout<<"INS_FACTORS solveLHS debug :: after solve"<<endl;
		  rhsLocal.display("rhsLocal");
		  ug_starLocal.display("u_starLocal");
		}

	    } // for each component to solve for
	} // if have_local_points

      // extrapolate the temporary variable into the ghosts
      for ( int d=1; d<cg.numberOfDimensions(); d++ )
	{
	  int td = (getDirection()+d)%cg.numberOfDimensions();
	  Range C(uc,tc);
	  BoundaryConditionParameters extrapParams;
	  extrapParams.orderOfExtrapolation=parameters->dbase.get<int >("orderOfAccuracy")+1;
	  extrapParams.ghostLineToAssign=1;
	  #ifdef EXTRAP_NORMAL_DIRS
	  ug_star.applyBoundaryCondition(C,BCTypes::extrapolate,BCTypes::allBoundaries,0.,u.t,extrapParams);
	  extrapParams.ghostLineToAssign=2;
	  ug_star.applyBoundaryCondition(C,BCTypes::extrapolate,BCTypes::allBoundaries,0.,u.t,extrapParams);
	  #endif
	}
      ug_star.periodicUpdate();
      ug_star.finishBoundaryConditions();


    } // for each grid
}

void
CGINS_ApproximateFactorization::
INS_Factor::
addExplicitContribution(const real &dt, const GridFunction &u, realCompositeGridFunction &f)
{
  InsFactorModes mode = addExplicit;
  const int dir = getDirection();

  CompositeGrid &cg = *u.u.getCompositeGrid();//*f.getCompositeGrid();
  CompositeGrid &cgs =*f.getCompositeGrid();//cg;
  int numberOfDimensions = cg.numberOfDimensions();
  int rhsOnly = 0;

  InsParameters::DiscretizationOptions discrete_approximation = parameters->dbase.get<InsParameters::DiscretizationOptions>("discretizationOption");

  int afParallelGhostWidth = parameters->dbase.get<int>("AFparallelGhostWidth");
  int uc = parameters->dbase.get<int>("uc");
  int vc = parameters->dbase.get<int>("vc");
  int wc = parameters->dbase.get<int>("wc");
  int tc = parameters->dbase.get<int>("tc");
  if ( cg.numberOfDimensions()==2 ) wc=vc;
  if (  parameters->dbase.get<InsParameters::PDEModel >("pdeModel")!=InsParameters::BoussinesqModel ) tc = wc;

  if ( parameters->dbase.get<int >("debug")>=64 )
    {
      u.u.display("getExplicit : u");
      f.display("getExplicit : f");
    }

  for ( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid &mg = cg[grid];
      MappedGrid &mgs= (MappedGrid &)cgs[grid];

      realMappedGridFunction &ug = u.u[grid];
      realMappedGridFunction &fg = f[grid];//u.u[grid];
      
      //      realMappedGridFunction rhs(mg,all,all,all);
      //      rhs.updateToMatchGridFunction(fg);
      realMappedGridFunction &gridVelocity = parameters->gridIsMoving(grid) ? ((GridFunction &)u).getGridVelocity(grid) : u.u[grid];

      const bool isRectangular = mg.isRectangular() && !forceNonRectangular;
      real dx[3]={1.,1.,1.};
      if( isRectangular )
	mg.getDeltaX(dx);
      else
	mg.update(MappedGrid::THEinverseVertexDerivative);
      
      OV_GET_LOCAL_ARRAY(real,ug);
      OV_GET_LOCAL_ARRAY(real,fg); // get the local array for fg...
      // ...and check to see if there are points on this processor
      GET_BOUNDS_INFO(ug,ugLocal,1);

      if ( have_local_points ) 
	{

	   // adjust the indices so that they are appropriate for the boundary conditions
#ifndef USE_COMBINED_FACTORS
	  int extra[] = {0,0,0};
	  if (!isPeriodic && !use_merged_factors) extra[getDirection()] = 1;
	  getIndex(mg.indexRange(),I1,I2,I3,extra[0],extra[1],extra[2]); /// ???!!!! would this affect getLocalArrayBounds ???!!!
#endif

	  //	  int nGhost = parameters->dbase.get<int>("orderOfAccuracy")==2 ? 1 : 2;
	  //	  if ( bcLocal(0,dir)==Parameters::interpolation ) II[dir] = Range(II[dir].getBase()-nGhost,II[dir].getBound());
	  //	  if ( bcLocal(1,dir)==Parameters::interpolation ) II[dir] = Range(II[dir].getBase(),II[dir].getBound()+nGhost);

	  // get local views of the rest of the stuff we need
	  RealArray rhsLocal(I1,I2,I3),fsCoeff;
	  rhsLocal(I1,I2,I3) = 0.;
	  // *wdh* 100227 OV_GET_LOCAL_ARRAY(real,rhs);
	  OV_GET_LOCAL_ARRAY(real,gridVelocity);
	  OV_GET_LOCAL_ARRAY_FROM(int,mask,mgs.mask());
	  OV_GET_LOCAL_ARRAY_CONDITIONAL(real,rsxy,isRectangular,ugLocal,mg.inverseVertexDerivative() );
	  const RealArray & bcData = parameters->dbase.get<RealArray>("bcData");
// 	  IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
// 	  // NOTE: bcLocal(side,axis) == -1 for internal boundaries between processors
// 	  ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( fg,gidLocal,dimLocal,bcLocal ); // call this to get bcLocal

// 	  // the following isPeriodic check should probably sit in some parallel utility
// 	  const IntegerArray &dim = mg.dimension();
// 	  bool isPeriodic = false;
// 	  isPeriodic = ( mg.isPeriodic(dir)!=Mapping::notPeriodic && 
// 			 dim(0,dir)==dimLocal(0,dir) && dim(1,dir)==dimLocal(1,dir) ); //!!! this will need to change for lines split by parallelism
  
	  RealArray dl1(I1,I2,I3);
	  RealArray   d(I1,I2,I3);
	  RealArray du1(I1,I2,I3);
	  RealArray dl2,du2;

	  //	  RealArray dl1(I1,I2,I3),d(I1,I2,I3),du1(I1,I2,I3),dl2,du2;
	  bool isPenta = ( (parameters->dbase.get<int >("orderOfAccuracy")==4 && discrete_approximation==InsParameters::standardFiniteDifference) ||
			   (parameters->dbase.get<int >("orderOfAccuracy")>4 && discrete_approximation==InsParameters::compactDifference && !use_merged_factors) ||
			   (parameters->dbase.get<int >("orderOfAccuracy")>2 && discrete_approximation==InsParameters::compactDifference && use_merged_factors));
	  if ( isPenta )
	    {
	      dl2.redim(I1,I2,I3);
	      du2.redim(I1,I2,I3);
	    }
	  else
	    {
	      dl2.reference(dl1);
	      du2.reference(du1);
	    }

	  rhsLocal(I1,I2,I3) = 0.0;
	  
	  // compute the freestream coefficients if we need them
	  if ( !isRectangular && type!=Diagonal_Factor && !implicit_freestream_correction )
	    {
	      int component_to_solve_for = -1;
	      SETUP_PARAMETER_ARRAYS;

	      int ierr=0;
#define ug_starLocal ugLocal
	      CALL_INS_FACTOR_ROUTINE(ins_fscoeff);
#undef ug_starLocal

	      if ( parameters->dbase.get<int >("debug")>=4 )
		{
		  cout<<"INS_FACTORS getExplicit debug :: after freestream correction ins_factor["<<type<<"], dir="<<getDirection()<<", cc="<<component_to_solve_for<<" : "<<endl;
		  if ( isPenta) dl2.display("dl2");
		  dl1.display("dl1");
		  d.display("d");
		  du1.display("du1");
		  if ( isPenta ) du2.display("du2");
		  rhsLocal.display("rhs");
		}

	      TridiagonalSolver tri;
	      TridiagonalSolver::SystemType tri_type = isPeriodic ? TridiagonalSolver::periodic : TridiagonalSolver::extended;
	      if ( isPenta )
		tri.factor(dl2,dl1,d,du1,du2,tri_type,dir);
	      else
		tri.factor(dl1,d,du1,tri_type,dir);
	      
	      tri.solve(rhsLocal);
	      if ( parameters->dbase.get<int >("debug")>=4 )
		{
		  cout<<"INS_FACTORS getExplicit debug :: after solve for freestream correction, ins_factor["<<type<<"], dir="<<getDirection()<<", cc="<<component_to_solve_for<<" : "<<endl;
		  rhsLocal.display("rhs");
		}
	      fsCoeff.resize(I1,I2,I3);
	      fsCoeff(I1,I2,I3) = rhsLocal(I1,I2,I3);
	      for (int axis=0; axis<mg.numberOfDimensions(); axis++ )
		for ( int side=0; side<2; side++ )
		  {
		    if ( IS_DIRICHLET(side,axis) || bcLocal(side,axis)==Parameters::neumannBoundaryCondition )
		      {
			Index Ib1,Ib2,Ib3,Ig1,Ig2,Ig3;
			getBoundaryIndex(gidLocal,side,axis,Ib1,Ib2,Ib3);
			fsCoeff(Ib1,Ib2,Ib3) = 0;
		      }
		  }

	      //	      cout<<"max fscoeff["<<dir<<"] = "<<max(fabs(fsCoeff))<<endl;
	      rhsLocal(I1,I2,I3) = 0.;

              #ifndef USE_PPP
	      if ( false )
		{
		  cout<<"PLOTTING IN GetExplicit, "<<type<<", "<<dir<<endl;
		  realMappedGridFunction tmp(mg);
		  tmp(I1,I2,I3) = fsCoeff(I1,I2,I3);
		  tmp.periodicUpdate();
		  PlotIt::contour(*Overture::getGraphicsInterface(),tmp);
		}
              #endif

	    } // freestream correction coefficient block
	  else if ( implicit_freestream_correction )
	    {
	      fsCoeff.resize(I1,I2,I3);
	      fsCoeff(I1,I2,I3) = 0.;
	    }

	  //	  fsCoeff(I1,I2,I3) = 0.;
	  //!!! in the following loop we assume the components are ordered pc,uc,vc,wc,tc with:
	  //    tc = wc if not boussinesq
	  //    wc = vc if 2D
	  for ( int component_to_solve_for=uc; component_to_solve_for<=tc; component_to_solve_for++ )
	    {
	      // the following line is a macro defined at the top of this file
	      SETUP_PARAMETER_ARRAYS;

	      // call the fortran subroutine that fills in the diagonals
	      int ierr = 0;
	      if ( (type!=R_Factor && type!=Merged_Factor) || component_to_solve_for==uc )
		{
		  // - we do this conditional because the R_Factor actually needs to solve for the pressure gradient
		  // - each direction in the pressure gradient will contribution to the full gradient for each component
		  // - oh yeah, CALL_INS_FACTOR_ROUTINE expects a dummy variable named ug_starLocal, define it appropriately here
#define ug_starLocal fgLocal
		  CALL_INS_FACTOR_ROUTINE(ins_factor[type]);
#undef ug_starLocal
		}

	      if ( parameters->dbase.get<int >("debug")>=4 )
		{
		  cout<<"INS_FACTORS getExplicit debug :: after ins_factor["<<type<<"], dir="<<getDirection()<<", cc="<<component_to_solve_for<<" : "<<endl;
		  if ( isPenta) dl2.display("dl2");
		  dl1.display("dl1");
		  d.display("d");
		  du1.display("du1");
		  if ( isPenta ) du2.display("du2");
		  rhsLocal.display("rhs");
		}

	      if ( type==R_Factor || type==Merged_Factor) 
		{
		  if ( component_to_solve_for==uc && discrete_approximation==InsParameters::compactDifference )
		    { // only do the solve once and reuse the result for the other physical dimensions
		      // also, only the compact schemes need the linear solver
		      TridiagonalSolver tri;
		      TridiagonalSolver::SystemType tri_type = isPeriodic ? TridiagonalSolver::periodic : TridiagonalSolver::extended;
		      if ( isPenta )
			tri.factor(dl2,dl1,d,du1,du2,tri_type,dir);
		      else
			tri.factor(dl1,d,du1,tri_type,dir);

		      tri.solve(rhsLocal);
		      if ( parameters->dbase.get<int >("debug")>=4 )
			{
			  cout<<"INS_FACTORS getExplicit debug :: after solve, ins_factor["<<type<<"], dir="<<getDirection()<<", cc="<<component_to_solve_for<<" : "<<endl;
			  rhsLocal.display("rhs");
			  
			}
		    }

		  if ( component_to_solve_for==uc ) // i.e. only needs to be done the first time through because we reuse the data for the other components
		    {
		      //			int axis = dir;
		      for (int axis=0; axis<mg.numberOfDimensions(); axis++ )
			for ( int side=0; side<2; side++ )
			  {
			    if ( IS_DIRICHLET(side,axis) || (bcLocal(side,axis)==Parameters::neumannBoundaryCondition ))//&& dir==axis) )
			      {
				Index Ib1,Ib2,Ib3,Ig1,Ig2,Ig3;
				getGhostIndex(gidLocal,side,axis,Ig1,Ig2,Ig3);
				getBoundaryIndex(gidLocal,side,axis,Ib1,Ib2,Ib3);
				// shift contributions on dirichlet boundaries to the ghost
				// commented for extrap if ( axis==dir ) rhsLocal(Ig1,Ig2,Ig3) = rhsLocal(Ib1,Ib2,Ib3); // comment out if we put dirichlet everwhere
				rhsLocal(Ib1,Ib2,Ib3) = 0;
				//	if ( axis==dir) rhsLocal(Ig1,Ig2,Ig3) = 0;
			      }

			  }
		    }

		  OV_APP_TO_PTR_3D(real,rhsLocal, rp);
		  OV_APP_TO_PTR_4D(real,fgLocal, fp);
		  OV_RGF_TO_PTR_5D(real,rsxyLocal, rxp, mg.numberOfDimensions());
		  OV_APP_TO_PTR_3D(int,maskLocal,maskp);

		  int xdim = component_to_solve_for - uc;
		  if ( component_to_solve_for<tc || tc==wc )
		    { // add the pressure gradient and the free-stream corrections (if needed) to the momentum-eqs
		      if ( isRectangular && dir==xdim )
			{
			  real drdx = mg.gridSpacing(dir)/dx[xdim];
			  
			  for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
			    for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
			      for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
				{
				  int ii[] = {i1,i2,i3};
				  int dir=getDirection();
				  bool isGhost = (ii[dir]<(II[dir].getBase()+extra[dir]) || ii[dir]>(II[dir].getBound()-extra[dir]));

				  bool isSlip = (ii[dir]==(II[dir].getBase()+extra[dir]) && bcLocal(0,dir)==Parameters::slipWall)||(ii[dir]==(II[dir].getBound()-extra[dir]) && bcLocal(1,dir)==Parameters::slipWall);
				  if ( (A_3D(maskp,i1,i2,i3) & MappedGrid::ISdiscretizationPoint ) && !isGhost && !isSlip )
				    {
				      A_4D(fp,i1,i2,i3,component_to_solve_for) -= dt*drdx*A_3D(rp,i1,i2,i3); // we need -grad p hence the -
				    }
				}
			}
		      else if ( !isRectangular )
			{ // then we need to use rsxy, i.e. multiply by rsxy(i1,i2,i3,dir,xdim)
			  OV_APP_TO_PTR_3D(real,fsCoeff, fsp);
			  OV_APP_TO_PTR_4D(real,ugLocal, up);
			  for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
			    for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
			      for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
				{
				  int ii[] = {i1,i2,i3};
				  int dir=getDirection();
				  bool isGhost = (ii[dir]<(II[dir].getBase()+extra[dir]) || ii[dir]>(II[dir].getBound()-extra[dir]));

				  if ( (A_3D(maskp,i1,i2,i3) & MappedGrid::ISdiscretizationPoint ) && !isGhost )
				    {
				      A_4D(fp,i1,i2,i3,component_to_solve_for) -= dt*A_5D(rxp,i1,i2,i3,dir,xdim)*A_3D(rp,i1,i2,i3); // we need -grad p hence the -
				      A_4D(fp,i1,i2,i3,component_to_solve_for) += dt*A_4D(up,i1,i2,i3,component_to_solve_for)*A_3D(fsp,i1,i2,i3); // freestream correction
				    }
				}
			}

		    } //
		  else if (component_to_solve_for==tc && tc>wc && !isRectangular && !implicit_freestream_correction ) // just add the freestream corrections to the temperature equation
		    { // we need to compute the "freestream" corrections for the temperature equation because it has a different diffusion coefficient
		      int component_to_solve_for = tc;
		      SETUP_PARAMETER_ARRAYS;
		      RealArray fsCoeff;

		      int ierr=0;
#define ug_starLocal ugLocal
		      CALL_INS_FACTOR_ROUTINE(ins_fscoeff);
#undef ug_starLocal
		      
		      if ( parameters->dbase.get<int >("debug")>=4 )
			{
			  cout<<"INS_FACTORS getExplicit debug :: after T freestream correction ins_factor["<<type<<"], dir="<<getDirection()<<", cc="<<component_to_solve_for<<" : "<<endl;
			  if ( isPenta) dl2.display("dl2");
			  dl1.display("dl1");
			  d.display("d");
			  du1.display("du1");
			  if ( isPenta ) du2.display("du2");
			  rhsLocal.display("rhs");
			}
			  
		      TridiagonalSolver tri;
		      TridiagonalSolver::SystemType tri_type = isPeriodic ? TridiagonalSolver::periodic : TridiagonalSolver::extended;
		      if ( isPenta )
			tri.factor(dl2,dl1,d,du1,du2,tri_type,dir);
		      else
			tri.factor(dl1,d,du1,tri_type,dir);
		      
		      tri.solve(rhsLocal);
		      if ( parameters->dbase.get<int >("debug")>=4 )
			{
			  cout<<"INS_FACTORS getExplicit debug :: after solve for T freestream correction, ins_factor["<<type<<"], dir="<<getDirection()<<", cc="<<component_to_solve_for<<" : "<<endl;
			  rhsLocal.display("rhs");
			}
		      fsCoeff.resize(I1,I2,I3);
		      fsCoeff(I1,I2,I3) = rhsLocal(I1,I2,I3);
		      for (int axis=0; axis<mg.numberOfDimensions(); axis++ )
			for ( int side=0; side<2; side++ )
			  {
			    if ( IS_DIRICHLET(side,axis) || (bcLocal(side,axis)==Parameters::neumannBoundaryCondition) )// && dir==axis) )
			      {
				Index Ib1,Ib2,Ib3,Ig1,Ig2,Ig3;
				getBoundaryIndex(gidLocal,side,axis,Ib1,Ib2,Ib3);
				fsCoeff(Ib1,Ib2,Ib3) = 0;
			      }
			  }
		      
		      //	      cout<<"max fscoeff["<<dir<<"] = "<<max(fabs(fsCoeff))<<endl;
		      rhsLocal(I1,I2,I3) = 0.;
		      
		      OV_APP_TO_PTR_3D(real,fsCoeff, fsp);
		      OV_APP_TO_PTR_4D(real,ugLocal, up);
		      for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
			for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
			  for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
			    if ( A_3D(maskp,i1,i2,i3) & MappedGrid::ISdiscretizationPoint )
			      {
				A_4D(fp,i1,i2,i3,component_to_solve_for) += dt*A_4D(up,i1,i2,i3,component_to_solve_for)*A_3D(fsp,i1,i2,i3); // freestream correction
			      }
		    } // freestream correction coefficient block
		}
	      else if ( type==Diagonal_Factor )
		{
		  // here we add Boussinesq terms and curvilinear grid cross derivative terms for the Laplacian
		  //   we also add the extrapolation in time for the interpolation points

		  if ( !isRectangular )
		    { // evaluate and add the cross derivative terms for the Laplacian approximation
		      real nu_eq = ( (parameters->dbase.get<InsParameters::PDEModel >("pdeModel")==InsParameters::BoussinesqModel) && 
				     component_to_solve_for==tc ) ? parameters->dbase.get<real >("kThermal") : parameters->dbase.get<real >("nu");

		      RealArray uxy(Range(ugLocal.getBase(0),ugLocal.getBound(0)),
				    Range(ugLocal.getBase(1),ugLocal.getBound(1)),
				    Range(ugLocal.getBase(2),ugLocal.getBound(2)));
				    
		      GET_BOUNDS_INFO(ug,ugLocal,0);
			      
		      SETUP_PARAMETER_ARRAYS;
		      RealArray d(I1,I2,I3),dl1(I1,I2,I3),du1(I1,I2,I3);
		      RealArray dl2,du2;
		      RealArray rhsLocal(I1,I2,I3);

		      if ( isPenta )
			{
			  dl2.redim(I1,I2,I3);
			  du2.redim(I1,I2,I3);
			}
		      else
			{
			  dl2.reference(dl1);
			  du2.reference(du1);
			}
		      rhsLocal(I1,I2,I3) = 0.;

		      OV_APP_TO_PTR_4D(real,ugLocal, up);
		      OV_APP_TO_PTR_3D(real,uxy,uxyp);
		      OV_APP_TO_PTR_3D(real,rhsLocal, rp);
		      OV_APP_TO_PTR_4D(real,fgLocal, fp);
		      OV_RGF_TO_PTR_5D(real,rsxyLocal, rxp, mg.numberOfDimensions());
		      OV_APP_TO_PTR_3D(int,maskLocal,maskp);

		      for (int kd=0; kd<mg.numberOfDimensions()-1; kd++)
			{
			  for ( int od=kd+1; od<mg.numberOfDimensions(); od++ )
			    {
			      int dir = kd; 

			      bool isPeriodic = false;
			      isPeriodic = isReallyPeriodic(mg,dim,dimLocal,dir);

			      ipar[27]= isPeriodic;	
			      int ierr = 0;
			      for ( int i3=ugLocal.getBase(2); i3<=ugLocal.getBound(2); i3++ )
				for ( int i2=ugLocal.getBase(1); i2<=ugLocal.getBound(1); i2++ )
				  for ( int i1=ugLocal.getBase(0); i1<=ugLocal.getBound(0); i1++ )
				    {
				      A_3D(uxyp,i1,i2,i3) = A_4D(up,i1,i2,i3,component_to_solve_for);
				    }

#define ug_starLocal uxy
#define ugLocal uxy
			      { // put this extra scope here to isolate the temporary value of component_to_solve_for
				int component_to_solve_for = 0;
				CALL_INS_FACTOR_ROUTINE(ins_evalux);
			      }
			      PRINT_MATRIX_ARRAYS(LAPLACIAN_1);
			      TridiagonalSolver tri;
			      TridiagonalSolver::SystemType tri_type = isPeriodic ? TridiagonalSolver::periodic : TridiagonalSolver::extended;
			      if ( isPenta )
				tri.factor(dl2,dl1,d,du1,du2,tri_type,dir);
			      else
				tri.factor(dl1,d,du1,tri_type,dir);
			      
			      tri.solve(rhsLocal);
			      // rhsLocal should now have dU[component_to_solve_for]/dr[kd]
			      {
				OV_APP_TO_PTR_3D(real,rhsLocal, rp);

				for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
				  for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
				    for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
				      {
					A_3D(uxyp,i1,i2,i3) = A_3D(rp,i1,i2,i3);
				      }
			      }
			      
			      int ld = od;
			      dir = ld;
			      isPeriodic = false;
			      isPeriodic = isReallyPeriodic(mg,dim,dimLocal,dir);

			      ipar[27]= isPeriodic;	

			      if ( isPeriodic )
				{ // SIGH, manually do the periodic update of the ghost points
				  int nd = mg.numberOfDimensions();
				  int d1 = (ld+1)%nd;
				  int d2 = (ld+2)%nd;
				  for ( int g=uxy.getBase(ld); g<0; g++ )
				    {
				      if ( nd==2 )
					{
					  for ( int j1=rhsLocal.getBase(d1); j1<=rhsLocal.getBound(d1); j1++ )
					    {
					      if ( ld==0 )
						{
						  A_3D(uxyp,g,j1,I3.getBase()) = A_3D(uxyp,I1.getBound()+g,j1,I3.getBase());
						  A_3D(uxyp,I1.getBound()-g,j1,I3.getBase()) = A_3D(uxyp,I1.getBase()-g,j1,I3.getBase());
						}
					      else
						{
						  A_3D(uxyp,j1,g,I3.getBase()) = A_3D(uxyp,j1,I2.getBound()+g,I3.getBase());
						  A_3D(uxyp,j1,I2.getBound()-g,I3.getBase()) = A_3D(uxyp,j1,I2.getBase()-g,I3.getBase());
						}
					    }
					}
				      else
					{
					  for ( int j1=rhsLocal.getBase(d1); j1<=rhsLocal.getBound(d1); j1++ )
					    for ( int j2=rhsLocal.getBase(d2); j2<=rhsLocal.getBound(d2); j2++ )
					      {
						if ( ld==0 )
						  {
						    A_3D(uxyp,g,j1,j2) = A_3D(uxyp,I1.getBound()+g,j1,j2);
						    A_3D(uxyp,I1.getBound()-g,j1,j2) = A_3D(uxyp,I1.getBase()-g,j1,j2);
						  }
						else if (ld==1)
						  {
						    A_3D(uxyp,j2,g,j1) = A_3D(uxyp,j2,I2.getBound()+g,j1);
						    A_3D(uxyp,j2,I2.getBound()-g,j1) = A_3D(uxyp,j2,I2.getBase()-g,j1);
						  }
						else
						  {
						    A_3D(uxyp,j1,j2,g) = A_3D(uxyp,j1,j2,I3.getBound()+g);
						    A_3D(uxyp,j1,j2,I3.getBound()-g) = A_3D(uxyp,j1,j2,I3.getBase()-g);
						  }
					      }
					}
					
				    }

				}


			      PRINT_MATRIX_ARRAYS(BEFORE_LAPLACIAN_2);
			      { // put this extra scope here to isolate the temporary value of component_to_solve_for
				int component_to_solve_for = 0;
				CALL_INS_FACTOR_ROUTINE(ins_evalux);
			      }
			      PRINT_MATRIX_ARRAYS(LAPLACIAN_2);
			      
			      tri_type = isPeriodic ? TridiagonalSolver::periodic : TridiagonalSolver::extended;
			      if ( isPenta )
				tri.factor(dl2,dl1,d,du1,du2,tri_type,dir);
			      else
				tri.factor(dl1,d,du1,tri_type,dir);
				  
			      tri.solve(rhsLocal);

			      for (int axis=0; axis<mg.numberOfDimensions(); axis++ )
				for ( int side=0; side<2; side++ )
				  {
				    if ( IS_DIRICHLET(side,axis) || bcLocal(side,axis)==Parameters::neumannBoundaryCondition)
				      {
					Index Ib1,Ib2,Ib3,Ig1,Ig2,Ig3;
					getBoundaryIndex(gidLocal,side,axis,Ib1,Ib2,Ib3);
					rhsLocal(Ib1,Ib2,Ib3) = 0;
				      }
				  }
			      
			      // rhsLocal should now have d^2U[component_to_solve_for]/dr[kd]dr[ld]
			      //			      getIndex(mg.indexRange(),I1,I2,I3,extra[0],extra[1],extra[2]);
			      //			      int lb1s,lb1e,lb2s,lb2e,lb3s,lb3e;			
			      //			      ParallelUtility::getLocalArrayBounds(ug,ugLocal,I1,I2,I3, 
			      //								   lb1s,lb1e,lb2s,lb2e,lb3s,lb3e,parallel_array_bounds_option);

			      real fd = mg.numberOfDimensions()-1; // in 3D we have two of each of these terms
			      {
				//				OV_APP_TO_PTR_3D(real,rhsLocal, rp);

				for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
				  for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
				    for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
				      {
					int ii[] = {i1,i2,i3};
					bool isGhost = false;
					for ( int aa = 0; aa<cg.numberOfDimensions() && !isGhost; aa++ )
					  isGhost = (ii[aa]<(II[aa].getBase()+extra[aa]) || ii[aa]>(II[aa].getBound()-extra[aa]));
					//					if ( (A_3D(maskp,i1,i2,i3) & MappedGrid::ISdiscretizationPoint) && !isGhost ) //kkc 120408 & MappedGrid::ISdiscretizationPoint ) //&& !isGhost )
					if ( (A_3D(maskp,i1,i2,i3) & MappedGrid::ISdiscretizationPoint) && !isGhost ) //kkc 120408 & MappedGrid::ISdiscretizationPoint ) //&& !isGhost )
					  for ( int xdim=0; xdim<mg.numberOfDimensions(); xdim++ )
					    {
					      A_4D(fp,i1,i2,i3,component_to_solve_for) += fd*dt*nu_eq*A_5D(rxp,i1,i2,i3,kd,xdim)*A_5D(rxp,i1,i2,i3,ld,xdim)*A_3D(rp,i1,i2,i3);
					    } // xdim loop
				      }
			      }
			    } // ld loop
#undef ugLocal
#undef ug_starLocal
			} // kd loop

		    } // !isRectangular if statement

		  // add in interpolation point extrapolation and Boussinesq terms

		  // boussinesq stuff
		  if ( component_to_solve_for<tc && tc>wc ) 
		    {//this stuff only gets added to the momentum equation when we are using the Boussinesq model
		      // fill in the rhs array with the bouancy terms and then zero out the dirichlet bc
		      OV_APP_TO_PTR_3D(real,rhsLocal, rp);
		      OV_APP_TO_PTR_4D(real,fgLocal, fp);
		      OV_APP_TO_PTR_4D(real,ugLocal,ugp);
		      OV_APP_TO_PTR_3D(int,maskLocal,maskp);
		      
		      for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
			for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
			  for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
			    {
			      A_3D(rp,i1,i2,i3) = dt*thermalExpansivity*gravity[component_to_solve_for-uc]*A_4D(ugp,i1,i2,i3,tc);
			    }

		      for (int axis=0; axis<mg.numberOfDimensions(); axis++ )
			for ( int side=0; side<2; side++ )
			  {
			    Index Ib1,Ib2,Ib3,Ig1,Ig2,Ig3;
			    if ( IS_DIRICHLET(side,axis) || bcLocal(side,axis)==Parameters::neumannBoundaryCondition)
			      {
				getBoundaryIndex(gidLocal,side,axis,Ib1,Ib2,Ib3);
				rhsLocal(Ib1,Ib2,Ib3) = 0;
			      }
			    // *wdh* 2012/10/13 -- is this next line correct or should it use isReallyPeriodic ?
                            // if ( mg.isPeriodic(axis)==Mapping::notPeriodic )
#if 0
			    if ( !isReallyPeriodic(mg,dim,dimLocal,axis) ) // ? should we use this ?
			      {
				getGhostIndex(gidLocal,side,axis,Ig1,Ig2,Ig3);
				rhsLocal(Ig1,Ig2,Ig3) = 0;
				getGhostIndex(gidLocal,side,axis,Ig1,Ig2,Ig3,2);
				rhsLocal(Ig1,Ig2,Ig3) = 0;
			      }
#endif
			  }		    

#if 0
		      for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
			for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
			  for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
			    A_4D(fp,i1,i2,i3,component_to_solve_for) -= A_3D(rp,i1,i2,i3);
#endif

		      for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
			for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
			  for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
			  {
			    int ii[] = {i1,i2,i3};
			    bool isGhost = false;
			    for ( int aa = 0; aa<cg.numberOfDimensions() && !isGhost; aa++ )
			      isGhost = (ii[aa]<(II[aa].getBase()+extra[aa]) || ii[aa]>(II[aa].getBound()-extra[aa]));
			    if ( (A_3D(maskp,i1,i2,i3) & MappedGrid::ISdiscretizationPoint) && !isGhost ) //kkc 120408 & MappedGrid::ISdiscretizationPoint ) //&& !isGhost )
			      A_4D(fp,i1,i2,i3,component_to_solve_for) -= A_3D(rp,i1,i2,i3);
			  }
		      



			    
		      
		    }
		  if ( true ) 
		    {
		      OV_GET_LOCAL_ARRAY(real,ug);
		      OV_APP_TO_PTR_4D(real,fgLocal, fp);
		      OV_APP_TO_PTR_4D(real,ugLocal,ugp);
		      OV_APP_TO_PTR_3D(int,maskLocal,maskp);
		      
		      // interoplation point extrapolation
		      real dtb = parameters->dbase.get<DataBase >("modelData").get<AdamsPCData>("AdamsPCData").dtb;

		      real dt0 = dt>0 ? 0.5*(-2.*dtb+sqrt(4*dtb*dtb+8*dtb*dt)) : sqrt(-dt*2*dtb);
		      real odtf = dt<0 ? -dt0/dtb : dt0/dtb; //-1 : 1;

		      if ( dt<0 )
			{ // then this is uOld, and advanceFactored always calls that after calling it with uCur
			  for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
			    for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
			      for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
				{
				  int ii[] = {i1,i2,i3};
				  bool isGhost = false;
				  for ( int dir = 0; dir<cg.numberOfDimensions() && !isGhost; dir++ )
				    isGhost = (ii[dir]<(II[dir].getBase()+extra[dir]) || ii[dir]>(II[dir].getBound()-extra[dir]));
				  if ( A_3D(maskp,i1,i2,i3)<=0 || isGhost )
				    {
				      A_4D(fp,i1,i2,i3,component_to_solve_for) += odtf*A_4D(ugp,i1,i2,i3,component_to_solve_for);
				    }
				}
			}
		      else
			{ // then this is uCur, and we need to overwrite whatever was put here...
			  //			  cout<<"DBG BASE ("<<Communication_Manager::My_Process_Number<<")  : "<<I3.getBase()<<", "<<I2.getBase()<<", "<<I1.getBase()<<", "<<dir<<", "<<extra[dir]<<endl;
			  //			  cout<<"DBG BOUND("<<Communication_Manager::My_Process_Number<<") : "<<I3.getBound()<<", "<<I2.getBound()<<", "<<I1.getBound()<<", "<<dir<<", "<<extra[dir]<<endl;
			  for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
			    for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
			      for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
				{
				  int ii[] = {i1,i2,i3};
				  bool isGhost = false;
				  for ( int dir = 0; dir<cg.numberOfDimensions() && !isGhost; dir++ )
				    isGhost = (ii[dir]<(II[dir].getBase()+extra[dir]) || ii[dir]>(II[dir].getBound()-extra[dir]));
				  //				  if ( isGhost ) cout<<"DBG("<<Communication_Manager::My_Process_Number<<") : "<<i1<<", "<<i2<<endl;
				  if ( A_3D(maskp,i1,i2,i3)<=0 || isGhost )
				    {
				      
				      A_4D(fp,i1,i2,i3,component_to_solve_for) = odtf*A_4D(ugp,i1,i2,i3,component_to_solve_for);
				    }
				}
			}

		      // ghost point extrapolation
		      
		    }
		  
		} // end of Diagonal_Factor block
	      else // type!=R_Factor && type!=Merged_Factor && type!=Diagonal_Factor
		{
		  OV_APP_TO_PTR_3D(real,rhsLocal, rp);
		  OV_APP_TO_PTR_4D(real,fgLocal, fp);
		  for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
		    for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
		      for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
			A_4D(fp,i1,i2,i3,component_to_solve_for) += A_3D(rp,i1,i2,i3);
		}
	    } // for each component to solve for
	} // if have_local_points

      for ( int d=1; d<cg.numberOfDimensions(); d++ )
	{
	  int td = (getDirection()+d)%cg.numberOfDimensions();
	  Range C(uc,tc);
	  BoundaryConditionParameters extrapParams;
	}
      fg.periodicUpdate();
      fg.finishBoundaryConditions();

    } // for each grid

}

