#include "DomainSolver.h"
#include "Ogshow.h"
#include "Ogen.h"
#include "Ogmg.h"
#include "LineSolve.h"


// =======================================================================================
//
// Some utility routines for the DomainSolver
//
// =======================================================================================





//\begin{>>CompositeGridSolverInclude.tex}{\subsection{sizeOf}} 
real DomainSolver::
sizeOf(FILE *file /* = NULL */ ) const
// =======================================================================================
// /Description:
//   Return size of this object  
//\end{CompositeGridSolverInclude.tex}  
// =======================================================================================
{
  real size=sizeof(*this);
  const real megaByte=1024.*1024;

  int grid;
//   if( mappedGridSolver!=NULL )
//   {
//     real wSize=0.;
//     for( grid=0; grid<solution.u.numberOfComponentGrids(); grid++ )
//     {
//       if( mappedGridSolver[grid]!=NULL )
//         wSize+=  mappedGridSolver[grid]->sizeOf();
//     }
//     fprintf(file,"*** OB_CompositeGridSolver::size of mappedGridSolver's = %16.2f Mbytes\n",wSize/megaByte);
//     size+=wSize;
//   }

  // Don't count solution since it is referenced to another gf[]
  // size+=solution.sizeOf();    // This grid function holds the current solution

  int i;
  for( i=0; i<maximumNumberOfGridFunctionsToUse; i++ )
    size+=gf[i].sizeOf()-gf[i].cg.sizeOf();   // do not count the cg 

  for( i=0; i<4; i++ )
   size+=fn[i].sizeOf();

  size+=poissonCoefficients.sizeOf();
  if( poisson!=NULL )
    size+=poisson->sizeOf();

  for( i=0; i<numberOfImplicitSolvers; i++ )
    size+=implicitSolver[i].sizeOf();
  size+=coeff.sizeOf();
  
  size+=pressureRightHandSide.sizeOf();
  if( pp!=NULL )
    size+=p().sizeOf();
  if( ppx!=NULL )
    size+=px().sizeOf();
  if( prL!=NULL )
    size+=rL().sizeOf();
  if( ppL!=NULL )
    size+=pL().sizeOf();
  if( prho!=NULL )
    size+=rho().sizeOf();
  if( pgam!=NULL )
    size+=gam().sizeOf();
  
  
//   if( pWorkSpace!=NULL )
//   {
//     real wSize=0.;
//     // only count unique work spaces
//     for( grid=0; grid<solution.u.numberOfComponentGrids(); grid++)
//     {
//       if( grid==0 || min(abs( workSpaceIndex(grid)-workSpaceIndex(Range(0,grid-1)))) !=0 )
//       {
// 	wSize+=workSpace(grid).sizeOf();
//         if( debug() & 2 )
//           fprintf(file,"*** OB_CompositeGridSolver::size of workspace(%i) = %16.2f Mbytes\n",grid,
//             workSpace(grid).sizeOf()/megaByte);
//       }
//     }
//     // display(workSpaceIndex,"workSpaceIndex","%3i");
    
//     if( debug() & 2 ) fprintf(file,"*** OB_CompositeGridSolver::size of workspace = %16.2f Mbytes\n",wSize/megaByte);
//     size+=wSize;
//   }

 if( pLineSolve!=NULL ) 
   size+=pLineSolve->sizeOf();

  return size;
}




//\begin{>>CompositeGridSolverInclude.tex}{\subsection{interpolate}} 
int DomainSolver::
interpolate( GridFunction & cgf, const Range & R /* = all */ )
//==============================================================================
// /Description:
// This interpolate function is simple used as a wrapper so that we time all
// interpolations. 
// /Notes: 
//    For conservative schemes we interpolate the conservative variables.
//\end{CompositeGridSolverInclude.tex}  
//==============================================================================
{
  real time=getCPU();
  const int & myid = parameters.dbase.get<int >("myid");
  FILE *& debugFile = parameters.dbase.get<FILE* >("debugFile");
  FILE *& pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");
  const Parameters::InterpolationTypeEnum & interpolationType = 
                 parameters.dbase.get<Parameters::InterpolationTypeEnum >("interpolationType");
  

  realCompositeGridFunction & u = cgf.u;
  
  if( interpolationType==Parameters::interpolatePrimitiveVariables )
  {
    cgf.conservativeToPrimitive();
    // printf(" *** interpolate u (primitive variables)\n");
  }
  else if( interpolationType==Parameters::interpolatePrimitiveAndPressure )
  {
    // In this case we interpolate the primitive variables plus pressure -- this is used in the
    // case of the multi-component fluid where it is better to interpolate the velocities and pressure
    // at a contact discontinuity since these will be smooth.
    cgf.conservativeToPrimitive();
    parameters.getDerivedFunction("pressure",cgf.u,cgf.u,parameters.dbase.get<int >("tc"),cgf.t,parameters);

    // printf(" *** interpolate u (primitive variables and pressure)\n");
  }
  else if( interpolationType==Parameters::interpolateConservativeVariables )
  {
    cgf.primitiveToConservative();
    // printf(" *** interpolate u (conservative variables)\n");
  }

  if( parameters.dbase.get<int >("maximumNumberOfIterationsForImplicitInterpolation")>0 )
    u.getInterpolant()->setMaximumNumberOfIterations(parameters.dbase.get<int >("maximumNumberOfIterationsForImplicitInterpolation"));

  u.interpolate(R);


  if( interpolationType==Parameters::interpolatePrimitiveAndPressure )
  {
    // convert pressure back T
    parameters.getDerivedFunction("temperature-from-pressure",cgf.u,cgf.u,parameters.dbase.get<int >("tc"),cgf.t,parameters);
  }



  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForInterpolate"))+=getCPU()-time;
  return 0;
}


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{fixupUnusedPoints}} 
int DomainSolver::
fixupUnusedPoints( realCompositeGridFunction & u )
//==============================================================================
// /Description:
// Fixup values at unused points
//\end{CompositeGridSolverInclude.tex}  
//==============================================================================
{
  if( debug() & 4 ) fPrintF(parameters.dbase.get<FILE* >("debugFile"),"\n ******** fixupUnusedPoints *****\n");
  
  RealArray values(parameters.dbase.get<int >("numberOfComponents"));
  values=0.;
  int numberOfGhostLines=2;  // we use this many ghost lines in computations.
  
  // We look for a vector that provides values for unused point
  if( parameters.dbase.get<DataBase >("modelParameters").has_key("unusedValue") )
  {
    // printf("++++DomainSolver::fixupUnusedPoints: using modelParameters unusedValue. \n");
    typedef vector<real> realVector;
    realVector & unusedValue = parameters.dbase.get<DataBase >("modelParameters").get<realVector>("unusedValue");
    for( int n=0; n<parameters.dbase.get<int >("numberOfComponents"); n++ )
    {
      values(n)=unusedValue[n];
    }
  }
  
  u.fixupUnusedPoints(values,numberOfGhostLines);
  
  // printf("<<fixupUnusedPoints: parameters.dbase.get<int >("globalStepNumber")=%i\n",parameters.dbase.get<int >("globalStepNumber"));
/* ---
  if( TRUE )
  {
    fprintf(parameters.dbase.get<FILE* >("debugFile")," ***After fixupUnusedPoints****\n");
    outputSolution( u,0. );
  }
--- */

  
  return 0;
}

// =============================================================================
/// \brief Extrapolate interpolation neighbours.
// \param gf (input/output) : apply to this grid function.
// \param C (input) : apply to these components.
// =============================================================================
void DomainSolver::
extrapolateInterpolationNeighbours( GridFunction & gf, const Range & C )
{
  BoundaryConditionParameters extrapParams;
  extrapParams.orderOfExtrapolation=parameters.dbase.get<int >("orderOfExtrapolationForInterpolationNeighbours");
  for( int grid=0; grid<gf.cg.numberOfComponentGrids(); grid++ )
  {
    realMappedGridFunction & u = gf.u[grid];
    u.applyBoundaryCondition(C,BCTypes::extrapolateInterpolationNeighbours,BCTypes::allBoundaries,0.,
                             gf.t,extrapParams);
  }
}
