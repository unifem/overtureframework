// ========================================================================================================
/// \class GridFunction
/// \brief An enhanced grid-function (field-variable) used by DomainSolver's.
/// \details This enhanced "grid-function" includes information such as whether the
///      values are in conservative or primitive form, the time at which the GF lives,
///      and the grid velocity.
///
// ========================================================================================================

#include "GridFunction.h"
#include "MatrixTransform.h"
#include "ParallelUtility.h"
#include "MappedGridOperators.h"

// ===================================================================================================================
/// \brief Constructor to build an extended grid function.
///
/// \param pParameters (input) : pointer to a Parameters object.
// ==================================================================================================================
GridFunction::
GridFunction(Parameters *pParameters /* =NULL */ )
{
  
  t=0.;
  gridVelocityTime=-REAL_MAX;
  transform=NULL;
  numberOfTransformMappings=0;
  form=primitiveVariables;

  sizeOfGridVelocityArray=0;
  gridVelocity=NULL;
  pParams=pParameters;
}

// ===================================================================================================================
/// \brief destructor.
// ==================================================================================================================
GridFunction::
~GridFunction()
{
  if( transform!=NULL )
  {
    for( int grid=0; grid<numberOfTransformMappings; grid++ )
    {
      if( transform[grid]!=0 && transform[grid]->decrementReferenceCount()==0 )
        delete transform[grid];
    }
    delete [] transform;
  }
  if( gridVelocity!=NULL )
  {
    for( int grid=0; grid<sizeOfGridVelocityArray; grid++ )
    {
      delete gridVelocity[grid];
    }
    delete [] gridVelocity;

    sizeOfGridVelocityArray=0;
  }
  
}

// ===================================================================================================================
/// \brief return the size of this object in bytes.
///
/// \param file (input) : output any summary info to this file.
// ==================================================================================================================
real GridFunction::
sizeOf(FILE *file /* = NULL */ ) const
{
  real size=sizeof(*this);
  size+=u.sizeOf();
  size+=cg.sizeOf();
  
  if( gridVelocity!=NULL )
  {
    for( int grid=0; grid<sizeOfGridVelocityArray; grid++ )
    {
      if( gridVelocity[grid]!=NULL )
	size+=gridVelocity[grid]->sizeOf();
    }
  }
  
  return size;
}

// ===================================================================================================================
/// \brief Build a grid function to hold the grid velocities.
///
/// \param grid (input) : grid number.
// ==================================================================================================================
realMappedGridFunction & GridFunction::
createGridVelocity(int grid )
{
  if( gridVelocity==NULL || grid>=sizeOfGridVelocityArray )
  {
    if( gridVelocity==NULL )
    {
      sizeOfGridVelocityArray=cg.numberOfComponentGrids();
      gridVelocity=new realMappedGridFunction * [cg.numberOfComponentGrids()];
      for( int g=0; g<cg.numberOfComponentGrids(); g++ )
	gridVelocity[g]=NULL;
    }
    else
    {
      // The number of grids has changed.
      realMappedGridFunction **temp = new realMappedGridFunction * [cg.numberOfComponentGrids()];
      int g;
      for( g=cg.numberOfComponentGrids(); g<sizeOfGridVelocityArray; g++ )
        delete gridVelocity[g];   // remove any excess grid-functions
      
      for( int g=0; g<min(sizeOfGridVelocityArray,cg.numberOfComponentGrids()); g++ )
	temp[g]=gridVelocity[g];  // save existing grid functions

      for( g=sizeOfGridVelocityArray; g<cg.numberOfComponentGrids(); g++ )
	temp[g]=NULL;

      delete [] gridVelocity;
      gridVelocity=temp;
    }
    
  }
  if( gridVelocity[grid]==NULL )
  {
    Range all;
    gridVelocity[grid] = new realMappedGridFunction(cg[grid],all,all,all,cg.numberOfDimensions());
  }
  return *gridVelocity[grid];
}

// ===================================================================================================================
/// \brief Update the grid velocity arrays -- use this function if the number of grids changes.
///
// ==================================================================================================================
void GridFunction::
updateGridVelocityArrays()
{
  if( gridVelocity==NULL ) return;
  
  assert( pParams!=NULL );
  Parameters & parameters = *pParams;

  if( false )
    printf(" **** OB_CGF:updateGridVelocityArrays: sizeOfGridVelocityArray=%i cg.numberOfComponentGrids()=%i\n",
	   sizeOfGridVelocityArray,cg.numberOfComponentGrids());
  
  if( sizeOfGridVelocityArray!=cg.numberOfComponentGrids() || parameters.isAdaptiveGridProblem() )
  {
    // For moving AMR grids we need to deal with the case when a refinement grid changes from being
    // a moving grid to non-moving and vice versa

    realMappedGridFunction **temp = gridVelocity;
    gridVelocity=new realMappedGridFunction * [cg.numberOfComponentGrids()];
    int num=min(sizeOfGridVelocityArray,cg.numberOfComponentGrids());
   
    int grid;
    Range all;
    if( true )
    {
      // ** new way **
      int gg=0;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        if( parameters.gridIsMoving(grid) )
	{
	  while( gg<num && temp[gg]==NULL ){ gg++; }  // find a grid velocity to use, if any.
          if( gg<num )
  	    gridVelocity[grid]=temp[gg];
          else
	    gridVelocity[grid]=new realMappedGridFunction(cg[grid],all,all,all,cg.numberOfDimensions());
	  gg++;
	}
	else
          gridVelocity[grid]=NULL;
      }
      
    }
    else
    {
      for( grid=0; grid<num; grid++ )
      {
	gridVelocity[grid]=temp[grid];
      }
      
      for( grid=num; grid<cg.numberOfComponentGrids(); grid++ )
      {
	gridVelocity[grid]=NULL;
      }

      delete [] temp;

      // Now build grid velocity's for any AMR child grids that lie on a moving base grid
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	if( cg.refinementLevelNumber(grid)>0 && gridVelocity[grid]==NULL )
	{
	  // refinement grids are moved in the same way as their parent
	  int base = cg.baseGridNumber(grid);
	  if( gridVelocity[base]!=NULL )
	  {
	    gridVelocity[grid] = new realMappedGridFunction(cg[grid],all,all,all,cg.numberOfDimensions());
	  }
	}
      }
    }
    
    sizeOfGridVelocityArray=cg.numberOfComponentGrids();
  }
  // Now update to match any new grid sizes
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    if( gridVelocity[grid]!=NULL )
    {
      assert( parameters.gridIsMoving(grid) );
      
      gridVelocity[grid]->updateToMatchGrid(cg[grid]);
      if( false )
      {
        printf(" GridFunction::updateGridVelocityArrays: t=%8.2e update grid=%i "
               " dimension=[%i,%i][%i,%i]\n",
	       t,grid,gridVelocity[grid]->getBase(0),gridVelocity[grid]->getBound(0),
	       gridVelocity[grid]->getBase(1),gridVelocity[grid]->getBound(1));
      }
    }
    else
    {
      if( false ) 
        printf(" GridFunction::updateGridVelocityArrays: t=%8.2e do NOT update grid=%i\n",t,grid);
 
    }
    
  }
  
}


 
// **** this is temporary ****
realCompositeGridFunction & GridFunction::
getGridVelocity()
{
  if( true )
  {
    printF("GridFunction::getGridVelocity(): ERROR: this function should not be called!\n");
    Overture::abort("ERROR");
  }
  return u;
}



// ===================================================================================================================
/// \brief Return the gridVelocity (if it is there, otherwise return a null grid function).
/// \param grid (input) : grid number.
// ==================================================================================================================
realMappedGridFunction & GridFunction::
getGridVelocity(int grid) 
{
  if( gridVelocity!=NULL && (grid>=sizeOfGridVelocityArray) )
  {
    updateGridVelocityArrays();
  }
  assert( gridVelocity==NULL || (grid<sizeOfGridVelocityArray) );
  
  if( gridVelocity!=NULL && gridVelocity[grid]!=NULL )
    return *gridVelocity[grid];

  return Overture::nullRealMappedGridFunction();
}

// ===================================================================================================================
/// \brief Reference this gridVelocity to that of another grid function.
/// \param gf (input) : 
// ==================================================================================================================
int GridFunction::
referenceGridVelocity(GridFunction & gf)
{
  if( gridVelocity==NULL )
    return 0;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    assert( grid<sizeOfGridVelocityArray );
    if( gridVelocity[grid]!=NULL )
      gridVelocity[grid]->reference(gf.getGridVelocity(grid));
  }
  return 0;
}

// ===================================================================================================================
/// \brief Provide parameters.
/// \param parameters (input) : 
// ==================================================================================================================
void GridFunction::
setParameters(Parameters & parameters)
{
  pParams = &parameters;
}


// ===================================================================================================================
/// \brief update to match a new grid.
//   \details this function assumes that u and gridVelocity are already defined.
/// \param cg0 (input) : new grid to match to.
// ==================================================================================================================
int GridFunction::
updateToMatchGrid(CompositeGrid & cg0)
// this function assumes that u and gridVelocity are already defined
{
  cg.reference(cg0);

  return 0;
}



// //\begin{>>CompositeGridFunctionInclude.tex}{\subsection{update}}
// int GridFunction::
// update( const Forms & form0 )
// // ===========================================================================
// // /Description:
// // match component index values to PDE
// //\end{CompositeGridFunctionInclude.tex}  
// // ===========================================================================
// {
//   assert( pParams!=NULL );
//   Parameters & parameters = *pParams;

//   form=form0;
//   componentTimes.redim(parameters.dbase.get<int >("numberOfComponents"));
//   componentTimes=t;

//   return 0;
// }

// ===================================================================================================================
/// \brief Convert primitive variables to conservative
//  \details 
/// primitive : rho, u,v,w, T, species \n 
/// conservative rho, (rho*u), (rho*v), (rho*w), E, (rho*species)
///
/// \param gridToConvert (input) : by default (grid==-1) convert all grids, otherwise convert this grid.
/// \param fixupUnsedPoints (input) : if true fixup unused points
// ==================================================================================================================
int GridFunction::
primitiveToConservative(int gridToConvert  /* =-1 */, 
                        int fixupUnsedPoints /* =false */)
{
  assert( pParams!=NULL );
  Parameters & parameters = *pParams;
  return parameters.primitiveToConservative(*this,gridToConvert,fixupUnsedPoints);
}

//\begin{>>CompositeGridFunctionInclude.tex}{\subsection{conservativeToPrimitive}} 
int GridFunction::
conservativeToPrimitive(int gridToConvert  /* =-1 */, 
                        int fixupUnsedPoints /* =false */ )
// ==================================================================================
// /Description:
//   Convert conservative variables to primitive.
// primitive : rho, u,v,w, T. species
// conservative rho, (rho*u), (rho*v), (rho*w), E, (rho*species)
//
// /gridToConvert (input) : by default (grid==-1) convert all grids, otherwise convert this grid.
// /fixUnsedPoints (input) : if true fixup unused points
//
//\end{CompositeGridFunctionInclude.tex}  
// =========================================================================================
{
  assert( pParams!=NULL );
  Parameters & parameters = *pParams;
  return parameters.conservativeToPrimitive(*this,gridToConvert,fixupUnsedPoints);
}



//\begin{>>CompositeGridFunctionInclude.tex}{\subsubsection{get}}
int GridFunction::
get( const GenericDataBase & dir, const aString & name)
//==================================================================================
// /Description:
//   Get from a database file.
// /dir (input): get from this directory of the database.
// /name (input): the name of the grid function on the database.
// /NOTE: This get function will not set the pointer to the MappedGrid associated
//    with this grid function. You should call updateToMatchGrid(...) to set
//    the grid BEFORE using this function. 
//\end{CompositeGridFunctionInclude.tex} 
//==================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"CompositeGridFunction");

//  subDir.setMode(GenericDataBase::streamInputMode);

  subDir.get( t,"t" );
  cg.get(subDir,"cg");
  u.updateToMatchGrid(cg);
  u.get( subDir,"u" );
  char buff[100];
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    int gridVelocityExists=false;
    subDir.get(gridVelocityExists,sPrintF(buff,"gridVelocityExists[%i]",grid));
    if( gridVelocityExists )
    {
      assert( gridVelocity[grid]!=NULL );
      gridVelocity[grid]->get(subDir,sPrintF(buff,"gridVelocity[%i]",grid));
    }
  }
  
  return 0;
}




//\begin{>>CompositeGridFunctionInclude.tex}{\subsubsection{put}}
int GridFunction::
put( GenericDataBase & dir, const aString & name) const
//==================================================================================
// /Description:
// /dir (input): put onto this directory of the database.
// /name (input): the name of the grid function on the database.
//\end{CompositeGridFunctionInclude.tex} 
//==================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"CompositeGridFunction");        // create a sub-directory 

//  subDir.setMode(GenericDataBase::streamOutputMode);

  subDir.put(t,"t");
  cg.put(subDir,"cg");
  u.put(subDir,"u");

  char buff[100];
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    int gridVelocityExists=gridVelocity!=NULL && gridVelocity[grid]!=NULL;
    subDir.put(gridVelocityExists,sPrintF(buff,"gridVelocityExists[%i]",grid));
    if( gridVelocityExists )
    {
      gridVelocity[grid]->put(subDir,sPrintF(buff,"gridVelocity[%i]",grid));
    }
  }

  return 0;
}

