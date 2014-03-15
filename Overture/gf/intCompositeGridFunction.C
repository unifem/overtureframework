#include "intGridCollectionFunction.h"
#include "GridCollection.h"
#include "Interpolant.h"
#include "CompositeGridOperators.h"

#include "intCompositeGridFunction.h"
#include "intCompositeGridFunction.h"

// Default constructor
intCompositeGridFunction::intCompositeGridFunction ()
{
  className="intCompositeGridFunction";
}

intCompositeGridFunction::
intCompositeGridFunction(CompositeGrid & grid, 
		      const Range & R0, 
		      const Range & R1,
		      const Range & R2,
		      const Range & R3,
		      const Range & R4,
		      const Range & R5,
		      const Range & R6,
		      const Range & R7 )
// don't do the following, otherwise the virtual function updateCollections is not called properly
// : intGridCollectionFunction( grid,R0,R1,R2,R3,R4,R5,R6,R7 )
{
  className="intCompositeGridFunction";
  updateToMatchGrid( grid,R0,R1,R2,R3,R4,R5,R6,R7 );
  // updateReferences();  // now done in update
}


intCompositeGridFunction::
intCompositeGridFunction(CompositeGridData & gcData, 
		      const Range & R0, 
		      const Range & R1,
		      const Range & R2,
		      const Range & R3,
		      const Range & R4,
		      const Range & R5,
		      const Range & R6,
		      const Range & R7 )
// : intGridCollectionFunction( gcData,R0,R1,R2,R3,R4,R5,R6,R7 )
{
  className="intCompositeGridFunction";
  updateToMatchGrid(gcData,R0,R1,R2,R3,R4,R5,R6,R7 );
  // updateReferences();
}
intCompositeGridFunction::
intCompositeGridFunction(CompositeGrid & grid, 
		      const int   & i0, 
		      const Range & R1,
		      const Range & R2,
		      const Range & R3,
		      const Range & R4,
		      const Range & R5,
		      const Range & R6,
		      const Range & R7 )
// : intGridCollectionFunction( grid,i0,R1,R2,R3,R4,R5,R6,R7 )
{
  className="intCompositeGridFunction";
  updateToMatchGrid( grid,i0,R1,R2,R3,R4,R5,R6,R7 );
  // updateReferences();
}


intCompositeGridFunction::
intCompositeGridFunction(CompositeGridData & gcData, 
		      const int   & i0, 
		      const Range & R1,
		      const Range & R2,
		      const Range & R3,
		      const Range & R4,
		      const Range & R5,
		      const Range & R6,
		      const Range & R7 )
// : intGridCollectionFunction( gcData,i0,R1,R2,R3,R4,R5,R6,R7 )
{
  className="intCompositeGridFunction";
  updateToMatchGrid( gcData,i0,R1,R2,R3,R4,R5,R6,R7 );
  // updateReferences();
}

//
// This constructor takes a GridFunctionType
// 
intCompositeGridFunction::
intCompositeGridFunction(CompositeGrid & gc, 
		      const GridFunctionParameters::GridFunctionType & type, 
		      const Range & component0,
		      const Range & component1,
		      const Range & component2,
		      const Range & component3,
		      const Range & component4)
// : intGridCollectionFunction( gc,type,component0,component1,component2,component3,component4)
{
  className="intCompositeGridFunction";
  updateToMatchGrid( gc,type,component0,component1,component2,component3,component4);
  // updateReferences();
}

intCompositeGridFunction::
intCompositeGridFunction(CompositeGrid & gc)
// : intGridCollectionFunction( gc )
{
  className="intCompositeGridFunction";
  updateToMatchGrid(gc);
  // updateReferences();
}

intCompositeGridFunction::
intCompositeGridFunction(CompositeGridData & cgData)
// : intGridCollectionFunction( cgData )
{
  className="intCompositeGridFunction";
  updateToMatchGrid( cgData );
  // updateReferences();
}

// Copy constructor, deep copy by default
intCompositeGridFunction::
intCompositeGridFunction(const intCompositeGridFunction & cgf, 
		      const CopyType copyType)
: intGridCollectionFunction(cgf,copyType)
{
  className="intCompositeGridFunction";
  updateReferences();
}

intCompositeGridFunction::
intCompositeGridFunction(const intGridCollectionFunction & gcf, 
		      const CopyType copyType)
: intGridCollectionFunction(gcf,copyType)
{
  className="intCompositeGridFunction";
  updateReferences();
}


// Destructor
intCompositeGridFunction::
~intCompositeGridFunction ()
{
}

intCompositeGridFunction & intCompositeGridFunction::
operator= ( const intCompositeGridFunction & cgf )
{
  if( gridCollection!=0 && cgf.gridCollection!=0 )
  {
    // SHALLOW COPY
    intGridCollectionFunction::operator=(cgf);         // use = from base class
  }
  else if( cgf.gridCollection!=0 )
  {
    // DEEP COPY
    intGridCollectionFunction::operator=(cgf);         // use = from base class

    // we must handle these lists separately since the base class has a different type of list
    // refinementLevel        =cgf.refinementLevel;
    // multigridLevel         =cgf.multigridLevel;
    updateCollections(); // *wdh* 980619
    updateReferences();
  }
  else
  {
    destroy();
  }
  className="intCompositeGridFunction";
  
  if( cgf.temporary )
  { // Delete cgf if it is a temporary
    intCompositeGridFunction *temp = (intCompositeGridFunction*) &cgf; 
    delete temp;                                                  
  }
  return *this;
}

intGridCollectionFunction & intCompositeGridFunction::
operator= ( const intGridCollectionFunction & cgf )
{
  // note that after this call the gridCollection in the result could be point to a GridCollection
  // and not a CompositeGrid
  intGridCollectionFunction::operator=(cgf);         // use = from base class
  // we cannot use the = operator as above to copy the refinementLevel and multigridLevel
  // since the lists are the wrong type. Thus we recreate the lists
  updateCollections();
  updateReferences();
  return *this;
}

intCompositeGridFunction intCompositeGridFunction::
operator()(const Range & component0,
	   const Range & component1 /* =nullRange */,
	   const Range & component2 /* =nullRange */,
	   const Range & component3 /* =nullRange */,
	   const Range & component4 /* =nullRange */ )
//==================================================================================
// /Description:
//   Return a link to some specfied components. 
// \end{verbatim}
//==================================================================================
{
  intCompositeGridFunction result;
  result.link(*this,component0,component1,component2,component3,component4);
  return intCompositeGridFunction(result,SHALLOW); // return a shallow copy so that we don't break the link
}

void intCompositeGridFunction::
link(const intCompositeGridFunction & gcf,
     const Range & R0,
     const Range & R1,
     const Range & R2,
     const Range & R3,
     const Range & R4 )
{
  intGridCollectionFunction::link(gcf,R0,R1,R2,R3,R4);
  updateReferences();
}

//-----------------------------------------------------------------------------------------------
// Old style link:
//-----------------------------------------------------------------------------------------------
void intCompositeGridFunction::
link(const intCompositeGridFunction & gcf, const int componentToLinkTo, const int numberOfComponents )
{
  Range R(componentToLinkTo,componentToLinkTo+numberOfComponents-1);
  link( gcf,R );
}

void intCompositeGridFunction::
updateReferences()
{
  // reference the list in the RC data to the one in the derived class
  rcData->refinementLevel.reference((ListOfIntGridCollectionFunction &)refinementLevel);
  // set the list in the base class as well
  intGridCollectionFunction::refinementLevel.reference((ListOfIntGridCollectionFunction &)refinementLevel);

  // reference the list in the RC data to the one in the derived class
  rcData->multigridLevel.reference((ListOfIntGridCollectionFunction &)multigridLevel);
  intGridCollectionFunction::multigridLevel.reference((ListOfIntGridCollectionFunction &)multigridLevel);
  
}

//\begin{>>CompositeGridFunctionInclude.tex}{\subsubsection{getInterpolant}}
Interpolant* intCompositeGridFunction::
getInterpolant(const bool abortIfNull /* =TRUE */) const
//==================================================================================
// /Description:
//   Return a pointer to the Interpolant. 
//==================================================================================
{
  if( abortIfNull && rcData->interpolant==0 && gridCollectionData->interpolant==0 )
  {
    cout << "intCompositeGridFunction:getInterpolant:ERROR: The pointer to the Interpolant is NULL \n";
    Overture::abort("error");
  }
  return rcData->interpolant!=0 ? rcData->interpolant : gridCollectionData->interpolant;
}

CompositeGridOperators* intCompositeGridFunction::
getOperators() const
//==================================================================================
// /Description:
//    get the operators used with this grid function. Return NULL if there are none.
//==================================================================================
{
  // the case below should safe as the base class cannot be made
  if( operators ) 
    return (CompositeGridOperators*)operators;
  else
    return NULL;
}

//\begin{>>CompositeGridFunctionInclude.tex}{\subsubsection{getClassName}}
aString intCompositeGridFunction::
getClassName() const 
//==================================================================================
// /Description:
//    Return the class name.
//
//\end{CompositeGridFunctionInclude.tex} 
//==================================================================================
{ 
  return className; 
}


//\begin{>>CompositeGridFunctionInclude.tex}{\subsubsection{getCompositeGrid}}
CompositeGrid* intCompositeGridFunction::
getCompositeGrid(const bool abortIfNull /* =TRUE */ ) const
//==================================================================================
// /Description:
//   Return a pointer to the CompositeGrid that this grid function is associaated with
//   By default this function will abort if the pointer is NULL.
// /Return values:
//   A pointer to a CompositeGrid or NULL
//\end{CompositeGridFunctionInclude.tex} 
//==================================================================================
{
  if( abortIfNull && gridCollection==NULL )
  {
    cout << "intCompositeGridFunction:getCompositeGrid:ERROR: The pointer to the gridCollection is NULL \n";
    Overture::abort("error");
  }
  return (CompositeGrid*) gridCollection;
}

void intCompositeGridFunction::
reference( const intCompositeGridFunction & cgf )
//-----------------------------------------------------------------------------------------
// /Description:
//   Use this function to reference one intCompositeGridFunction to another.
//   When two (or more) grid functions have been
//   referenced they share the same array data so that changes to one grid function
//   will change all the other referenced grid functions. 
//   Only the array data is referenced. Other properties of the grid function such
//   as cell-centredness can be changed in the referenced grid function. The "shape"
//   of the referenced grid function can also be changed without changing 
//   the referencee:{\ff cgf}.  
//  /Author: WDH
//\end{CompositeGridFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  intGridCollectionFunction::reference(cgf);
  
  multigridLevel .reference(cgf.multigridLevel); 
  refinementLevel.reference(cgf.refinementLevel); 
  rcData->multigridLevel.reference((ListOfIntGridCollectionFunction &)multigridLevel);
  rcData->refinementLevel.reference((ListOfIntGridCollectionFunction &)refinementLevel);
}

//\begin{>>CompositeGridFunctionInclude.tex}{\subsubsection{sizeOf}}
real intCompositeGridFunction::
sizeOf(FILE *file /* = NULL */ ) const
// ==========================================================================
// /Description: 
//   Return number of bytes allocated by this object; optionally print detailed info to a file
//
// /file (input) : optinally supply a file to write detailed info to. Choose file=stdout to
// write to standard output.
// /Return value: the number of bytes.
//\end{CompositeGridFunctionInclude.tex}
//==========================================================================
{
  real size=intGridCollectionFunction::sizeOf();
  return size;
}


/* -----
void intCompositeGridFunction::
setOperators(GenericCollectionOperators & operators0 )
//==================================================================================
// /Description:
//   Supply a derivative object to use for computing derivatives
//   on all component grids. This operator is used for the member functions
//   .x .y .z .xx .xy etc.
// /operators0: use these operators.
//==================================================================================
{
  operators=&operators0; 
  for( int grid=0; grid< numberOfGrids(); grid++ )
  {
    mappedGridFunctionList[grid].setOperators((*getOperators())[grid]);  // set operators on component grids 
  }
}
----- */

//------------------------------------------------------------------------------
//  Interpolate using default Interpolant or one found in the grid collection
//   return : >0 -- error, unable to interpolate
//------------------------------------------------------------------------------
int intCompositeGridFunction::
interpolate(const Range & C0 /* = nullRange */,
	    const Range & C1 /* = nullRange */,
	    const Range & C2 /* = nullRange */ )
{
  if( numberOfGrids()==0 )
    return 0;
  if( rcData->interpolant!=NULL )
    return interpolate( *(rcData->interpolant),C0,C1,C2 );  // interpolate
  else if( gridCollectionData->interpolant !=NULL  )
    return interpolate( *(gridCollectionData->interpolant),C0,C1,C2  );  // use interpolant from gridCollection
  else
  {
    cout << "intCompositeGridFunction::interpolate:ERROR: Sorry but I cannot interpolate\n";
    cout << "...since I cannot find an interpolant to use\n";
    cout << "...you might want to make an Interpolant\n";
    return 1;
  }
}


//------------------------------------------------------------------------------
//  Interpolate using an Interpolant
//
//  Notes:
//   o The lines //, // and  are treated by the gf.p perl script
//------------------------------------------------------------------------------
#ifndef OV_USE_DOUBLE
//  int intCompositeGridFunction::interpolate(Interpolant & interpolant,
//                                          const Range & C0,
//	                                  const Range & C1,
//                                          const Range & C2)
// int intCompositeGridFunction::interpolate(Interpolant &, const Range &, const Range &, const Range &  )
    int intCompositeGridFunction::interpolate(Interpolant &, const Range &, const Range &, const Range &  )
{
  // return interpolant.interpolate( *this,C0,C1,C2  );
  // cout << "intCompositeGridFunction::interpolate: sorry unable to interpolate\n"; return 1;
      cout << "intCompositeGridFunction::interpolate: sorry unable to interpolate\n"; return 1;
}
#else
// int intCompositeGridFunction::interpolate(Interpolant & interpolant,
//                                         const Range & C0,
//	                                  const Range & C1,
//                                         const Range & C2 )
//  int intCompositeGridFunction::interpolate(Interpolant &, const Range &, const Range &, const Range &  )
    int intCompositeGridFunction::interpolate(Interpolant &, const Range &, const Range &, const Range &  )
{
  // return interpolant.interpolate( *this,C0,C1,C2  );
  //  cout << "intCompositeGridFunction::interpolate: sorry unable to interpolate\n"; return 1;
      cout << "intCompositeGridFunction::interpolate: sorry unable to interpolate\n"; return 1;
}
#endif



// define all the update functions
intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGrid()
{
  updateReturnValue returnValue=intGridCollectionFunction::updateToMatchGrid(); 
  updateReferences();  // *wdh* 011127 These were added in all the update functions below
  return returnValue;
}

intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGrid(CompositeGridData & gridData, 
		  const Range & R0, 
		  const Range & R1,
		  const Range & R2,
		  const Range & R3,
		  const Range & R4,
		  const Range & R5,
		  const Range & R6,
		  const Range & R7 )
{
  updateReturnValue returnValue=intGridCollectionFunction::updateToMatchGrid(gridData,R0,R1,R2,R3,R4,R5,R6,R7);
  updateReferences();
  return returnValue;
}


intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGrid(CompositeGrid & grid, 
		  const Range & R0, 
		  const Range & R1,
		  const Range & R2,
		  const Range & R3,
		  const Range & R4,
		  const Range & R5,
		  const Range & R6,
		  const Range & R7 )
{
  updateReturnValue returnValue=intGridCollectionFunction::updateToMatchGrid(grid,R0,R1,R2,R3,R4,R5,R6,R7); 
  updateReferences();
  return returnValue;
}


// define this version to avoid overloading ambiguities
intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGrid(CompositeGrid & grid, 
		  const int  & i0, 
		  const Range & R1,
		  const Range & R2,
		  const Range & R3,
		  const Range & R4,
		  const Range & R5,
		  const Range & R6,
		  const Range & R7 )
{
  updateReturnValue returnValue=intGridCollectionFunction::updateToMatchGrid(grid,i0,R1,R2,R3,R4,R5,R6,R7); 
  updateReferences();
  return returnValue;
}


intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGrid(const Range & R0, 
		  const Range & R1,
		  const Range & R2,
		  const Range & R3,
		  const Range & R4,
		  const Range & R5,
		  const Range & R6,
		  const Range & R7 )
{
  updateReturnValue returnValue=intGridCollectionFunction::updateToMatchGrid(R0,R1,R2,R3,R4,R5,R6,R7); 
  updateReferences();
  return returnValue;
}


intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGrid(CompositeGrid & grid, 
		  const GridFunctionParameters::GridFunctionType & type, 
		  const Range & component0,    
		  const Range & component1,
		  const Range & component2,
		  const Range & component3,
		  const Range & component4 )
{
  updateReturnValue returnValue=intGridCollectionFunction::
            updateToMatchGrid(grid,type,component0,component1,component2,component3,component4);
  updateReferences();
  return returnValue;
}

intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGrid(const GridFunctionParameters::GridFunctionType & type, 
		  const Range & component0,    
		  const Range & component1,
		  const Range & component2,
		  const Range & component3,
		  const Range & component4 )
{
  updateReturnValue returnValue=intGridCollectionFunction::
            updateToMatchGrid(type,component0,component1,component2,component3,component4);
  updateReferences();
  return returnValue;
}
intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGrid(CompositeGrid & grid, 
		  const GridFunctionParameters::GridFunctionType & type)
{
  updateReturnValue returnValue=intGridCollectionFunction::updateToMatchGrid(grid,type);
  updateReferences();
  return returnValue;
}

intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGrid(const GridFunctionParameters::GridFunctionType & type)
{
  updateReturnValue returnValue=intGridCollectionFunction::updateToMatchGrid(type);
  updateReferences();
  return returnValue;
}



intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGrid(CompositeGrid & grid)
{
  updateReturnValue returnValue=intGridCollectionFunction::updateToMatchGrid(grid);
  updateReferences();
  return returnValue;
}

intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGrid(CompositeGridData & grid)
{
  updateReturnValue returnValue=intGridCollectionFunction::updateToMatchGrid(grid);
  updateReferences();
  return returnValue;
}



// update this grid function to match another grid function
intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGridFunction(const intCompositeGridFunction & cgf, 
			  const Range & R0, 
			  const Range & R1,
			  const Range & R2,
			  const Range & R3,
			  const Range & R4,
			  const Range & R5,
			  const Range & R6,
			  const Range & R7 )
{
  updateReturnValue returnValue=intGridCollectionFunction::updateToMatchGridFunction(cgf,R0,R1,R2,R3,R4,R5,R6,R7); 
  updateReferences();
  return returnValue;
}


intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGridFunction(const intCompositeGridFunction & cgf)
{
  updateReturnValue returnValue=intGridCollectionFunction::updateToMatchGridFunction(cgf);
  updateReferences();
  return returnValue;
}

// make sure the number of mappedGridFunction's is correct
intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchNumberOfGrids(CompositeGrid& gc)
{
  updateReturnValue returnValue=intGridCollectionFunction::updateToMatchNumberOfGrids(gc);
  updateReferences();
  return returnValue;
}

intCompositeGridFunction & 
abs( const intCompositeGridFunction & cgf ) 
{
  intCompositeGridFunction *cgfn;
  if( cgf.temporary )
    cgfn=(intCompositeGridFunction*)&cgf;  // cgfn points to the temporary (cast away const)
  else
  {
    cgfn= new intCompositeGridFunction();
    (*cgfn).temporary=TRUE;
    (*cgfn).updateToMatchGridFunction(cgf);
  }
  for( int grid=0; grid< cgf.numberOfGrids(); grid++ )
    (*cgfn)[grid]=abs(cgf[grid]);

  return *cgfn;
}


#define INT_COLLECTION_FUNCTION  
#define COLLECTION_FUNCTION intCompositeGridFunction
#define COLLECTION CompositeGrid
#undef INT_COLLECTION_FUNCTION
#define INT_COLLECTION_FUNCTION intCompositeGridFunction
#define QUOTES_COLLECTION_FUNCTION "intGridCollectionFunction"
#define INTEGRAL_TYPE int
#include "derivativeDefinitions.C"
#undef COLLECTION_FUNCTION
#undef INT_COLLECTION_FUNCTION
#undef COLLECTION 
#undef QUOTES_COLLECTION_FUNCTION 
#undef INTEGRAL_TYPE

//\begin{>>CompositeGridFunctionInclude.tex}{\subsubsection{evaluate}}
intCompositeGridFunction
evaluate( intCompositeGridFunction & cgf ) 
//==================================================================================
// /Description:
//   Due to the way that temporaries are handled it is necessary to use this function
//   on expressions involving grid collection functions that are passed as arguments to function.
//   Example:
//   \begin{verbatim}
//     realGridCollectionFunction u(gc),v(cg);
//     ...
//     myFunction(evaluate(u+v));
//     ...
//   \end{verbatim}
//   If the {\tt evaluate} function were not used there could be a possible memory leak.
// /cgf (input): If this grid function is a temporary,
// /Return value: A grid collection function equal to cgf which is not a temporary
//\end{CompositeGridFunctionInclude.tex} 
//==================================================================================
{
  intCompositeGridFunction cgfn;
  cgfn.reference(cgf);
  cgfn.temporary=FALSE;
  if( cgf.temporary )
  { // Delete cgf if it is a temporary
    intCompositeGridFunction *temp = (intCompositeGridFunction*) &cgf; 
    delete temp;                                                  
  }

  return cgfn;
}

void intCompositeGridFunction::
setOperators(GenericCollectionOperators & operators0 )
//==================================================================================
// NOTE: we need this here in addition to the one in the intGridCollectionFunction
// since the refinementLevel list is different.
// /Description:
//   Supply a derivative object to use for computing derivatives
//   on all component grids. This operator is used for the member functions
//   .x .y .z .xx .xy etc.
// /operators0: use these operators.
//==================================================================================
{
  if( gridCollection==0 )
  {
    printf("compositeGridFunction::setOperators:WARNING: setting operators for a null intGridCollectionFunction\n"
           "                                              This operation is being ignored. \n");
    return;
  }
  operators=&operators0; 
  int grid;
  for( grid=0; grid< numberOfGrids(); grid++ )
    mappedGridFunctionList[grid].setOperators((*getOperators())[grid]);  // set operators on component grids 

  CompositeGrid & gc = (CompositeGrid &)(*gridCollection);
  int l;
  if( gc.computedGeometry() & GridCollection::THErefinementLevel )
  {
    if( refinementLevel.getLength() != gc.numberOfRefinementLevels() )
    {
      printf("intGridCollectionFunction::setOperators:ERROR: consistency error. The number of refinement levels\n"
	     " in the grid =%i, but the number of levels in the grid function =%i. \n"
	     " Perhaps you need to call updateToMatchGrid for the grid function before calling setOperators\n",
	     gc.numberOfRefinementLevels(),refinementLevel.getLength());
      Overture::abort("error");
    }
    for( l=0; l<gc.numberOfRefinementLevels(); l++ )
    {
      GridCollection & rl =gc.refinementLevel[l];
      for( grid=0; grid< rl.numberOfGrids(); grid++ )
	refinementLevel[l][grid].setOperators((*getOperators())[rl.gridNumber(grid)]);  
    }
  }
  if(  gc.computedGeometry() & GridCollection::THEmultigridLevel )
  {
    if( multigridLevel.getLength() != gc.numberOfMultigridLevels() )
    {
      printf("intGridCollectionFunction::setOperators:ERROR: consistency error. The number of multigrid levels\n"
	     " in the grid =%i, but the number of levels in the grid function =%i. \n"
	     " Perhaps you need to call updateToMatchGrid for the grid function before calling setOperators\n",
	     gc.numberOfMultigridLevels(),multigridLevel.getLength());
      Overture::abort("error");
    }
    for( l=0; l<gc.numberOfMultigridLevels(); l++ )
    {
      GridCollection & rl =gc.multigridLevel[l];
      for( grid=0; grid< rl.numberOfGrids(); grid++ )
	multigridLevel[l][grid].setOperators((*getOperators())[rl.gridNumber(grid)]);  
    }
  }
  
}

void intCompositeGridFunction::
setIsACoefficientMatrix(const bool trueOrFalse, 
			const int stencilSize,
			const int numberOfGhostLines,
			const int numberOfComponentsForCoefficients,
                        const int offset0 )
//  This needs to be here as well as in the intGridCollectionFunction since we use the refinement and multigrid lists
//  which are overloaded.
{
  if( numberOfGrids()==0 )
  {
    cout << "intCompositeGridFunction::setIsACoefficientMatrix:Warning: there are no grids in this collection\n";
    cout << "This call will have no effect, you should call setIsACoefficientMatrix after the providing\n";
    cout << "the intCompositeGridFunction with a CompositeGrid \n";
  }
  
  GridCollection & gc = *gridCollection;
  int l,grid;
  for( l=0; l<gc.numberOfMultigridLevels(); l++ )
  {
    GridCollection & m =  gc.numberOfMultigridLevels()==1 ? gc : gc.multigridLevel[l];
    int offset=offset0;
    for( int g=0; g< m.numberOfComponentGrids(); g++ )
    {
      const int grid = gc.numberOfMultigridLevels()==1 ? g : m.gridNumber(g);
      if( !( m[g].isRectangular() && ( (dataAllocationOption & 1) || 
				    ((dataAllocationOption & 2) && l <gc.numberOfMultigridLevels()-1) )) )
      {
	mappedGridFunctionList[grid].setIsACoefficientMatrix(trueOrFalse,stencilSize,numberOfGhostLines,
							     numberOfComponentsForCoefficients,offset); 
      }
      // printf("intGridCollectionFunction::setIsACoefficientMatrix grid=%i, offset=%i\n",grid,offset);
      offset+=numberOfComponentsForCoefficients
	*(gc[grid].dimension()(End,axis1)-gc[grid].dimension()(Start,axis1)+1)
	*(gc[grid].dimension()(End,axis2)-gc[grid].dimension()(Start,axis2)+1)
	*(gc[grid].dimension()(End,axis3)-gc[grid].dimension()(Start,axis3)+1);
    }
  }

  // *** we now need to update the information in the refinement and multigrid lists.
  if( gc.computedGeometry() & GridCollection::THErefinementLevel )
  {
    if( refinementLevel.getLength() != gc.numberOfRefinementLevels() )
    {
      printf("intCompositeGridFunction::setIsACoefficientMatrix: consistency error. The number of refinement levels\n"
	     " in the grid =%i, but the number of levels in the grid function =%i. \n"
	     " Perhaps you need to call updateToMatchGrid for the grid function before calling setIsACoefficientMatrix\n",
	     gc.numberOfRefinementLevels(),refinementLevel.getLength());
      Overture::abort("error");
    }
    for( l=0; l<gc.numberOfRefinementLevels(); l++ )
    {
      GridCollection & rl =gc.refinementLevel[l];
      for( grid=0; grid< rl.numberOfGrids(); grid++ )
	refinementLevel[l][grid].setIsACoefficientMatrix(mappedGridFunctionList[rl.gridNumber(grid)].sparse);
    }
  }
  if(  gc.computedGeometry() & GridCollection::THEmultigridLevel )
  {
    if( multigridLevel.getLength() != gc.numberOfMultigridLevels() )
    {
      printf("intCompositeGridFunction::setIsACoefficientMatrix: consistency error. The number of multigrid levels\n"
	     " in the grid =%i, but the number of levels in the grid function =%i. \n"
	     " Perhaps you need to call updateToMatchGrid for the grid function before calling setIsACoefficientMatrix\n",
	     gc.numberOfMultigridLevels(),multigridLevel.getLength());
      Overture::abort("error");
    }
    for( l=0; l<gc.numberOfMultigridLevels(); l++ )
    {
      GridCollection & rl =gc.multigridLevel[l];
      for( grid=0; grid< rl.numberOfGrids(); grid++ )
	multigridLevel[l][grid].setIsACoefficientMatrix(mappedGridFunctionList[rl.gridNumber(grid)].sparse);
    }
  }


}

int intCompositeGridFunction::
updateCollections()
//==================================================================================
// /Description:
//    Update the refinementLevel and multigridLevel
//  This is a protected member.
//
//\end{CompositeGridFunctionInclude.tex} 
//==================================================================================
{
  if( gridCollection!=NULL )
  {
    CompositeGrid & gc = (CompositeGrid &)(*gridCollection);
    int level;
    // if( gc.computedGeometry() & GridCollection::THErefinementLevel )
    //   cout << "intCompositeGridFunction:update the refinementLevel\n";
    if( gc.computedGeometry() & GridCollection::THErefinementLevel )
    {  // update the refinement levels
      // cout << "intCompositeGridFunction: REALLY update the refinementLevel...\n";

      // keep track of which levels are changed
      IntegerArray wasChanged(max(1,gc.numberOfRefinementLevels()));
      wasChanged=FALSE;

      // refinementLevel[level] is a CompositeGridFunctionFunction representing the refinement level
      // make the list the correct length
      while( refinementLevel.getLength() < gc.numberOfRefinementLevels() )
      {
	level=refinementLevel.getLength();
        assert( level>=0 );
	wasChanged(level)=TRUE;
	refinementLevel.addElement();         // add an element to the end of the list
	// *wdh* 001121 refinementLevel[level].initialize();  // initialize the CompositeGridFunctionFunction
      }
      while( refinementLevel.getLength() > gc.numberOfRefinementLevels() )
        refinementLevel.deleteElement();        // delete the last element

      // Each grid in the grid collection belongs to a refinementLevel
      // We need to go through each grid in order, find which level it sits on and then add it to the correct level
      IntegerArray gridNumber(gc.numberOfRefinementLevels());
      gridNumber=0;
      int grid;
      for( grid=0; grid< gc.numberOfGrids(); grid++ )
      {
        level = gc.refinementLevelNumber(grid);
        assert( level>=0 && level < gc.numberOfRefinementLevels() );

        intCompositeGridFunction & gcf = refinementLevel[level];
        if( gcf.mappedGridFunctionList.getLength()<gridNumber(level)+1 )
	{
          wasChanged(level)=TRUE;
  	  gcf.mappedGridFunctionList.addElement(mappedGridFunctionList[grid],gridNumber(level));
	}
	else
	{ // reference mappedGridFunction if it is not already there (no need to update grid function)
          gcf[gridNumber(level)].reference(mappedGridFunctionList[grid]);
	}
        gridNumber(level)++;
      } 	

      int level;
      for( level=0; level< gc.numberOfRefinementLevels(); level++ )
      {  // remove entries from refinementLevel[leve] if it is too long
        while( refinementLevel[level].mappedGridFunctionList.getLength() > gc.refinementLevel[level].numberOfGrids() )
        {
          wasChanged(level)=TRUE;
	  refinementLevel[level].mappedGridFunctionList.deleteElement();
	}
      }

      Range *R = rcData->R;
      for( level=0; level< gc.numberOfRefinementLevels(); level++ )
      { // finish updating the grid collection if it was changed
	intCompositeGridFunction & gcf = refinementLevel[level];
        gcf.gridCollection     = &gc.refinementLevel[level];
	gcf.gridCollectionData =  gc.refinementLevel[level].rcData;
	if( wasChanged(level) )
	{
          // cout << "intCompositeGridFunction:updateCollections...refinement was changed\n";

	  gcf.constructor( gcf.gridCollection,gcf.gridCollection->rcData,
			   R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7],FALSE );
	}
      }
    }
    //if( gc.computedGeometry() & GridCollection::THEmultigridLevel )
    //  cout << "intCompositeGridFunction:update multigrid levels, numberOfMultigridLevels=" 
    //       << gc.numberOfMultigridLevels() << "...\n";
    if( gc.computedGeometry() & GridCollection::THEmultigridLevel ) // *wdh* build even if only 1 levl 991003
    {  
      //cout << "intCompositeGridFunction:REALLY update multigrid levels, numberOfMultigridLevels=" 
      //     << gc.numberOfMultigridLevels() << "...\n";

      // update the multigrid levels
      // keep track of which levels are changed
      IntegerArray wasChanged(max(1,gc.numberOfMultigridLevels()));
      wasChanged=FALSE;

      // multigridLevel[level] is a intCompositeGridFunction representing the multigridLevel level
      // make the list the correct length
      while( multigridLevel.getLength() < gc.numberOfMultigridLevels() )
      {
	level=multigridLevel.getLength();
	wasChanged(level)=TRUE;
	multigridLevel.addElement();         // add an element to the end of the list
	// *wdh* 001121 multigridLevel[level].initialize();  // initialize the intCompositeGridFunction
      }
      while( multigridLevel.getLength() > gc.numberOfMultigridLevels() )
        multigridLevel.deleteElement();        // delete the last element

      // Each grid in the grid collection belongs to a multigridLevel
      // We need to go through each grid in order, find which level it sits on and then add it to the correct level
      IntegerArray gridNumber(gc.numberOfMultigridLevels());
      gridNumber=0;
      int grid;
      for( grid=0; grid< gc.numberOfGrids(); grid++ )  // ****** gc.numberOfGrids() ****
      {
        level = gc.multigridLevelNumber(grid);
        assert( level>=0 && level < gc.numberOfMultigridLevels() );

        intCompositeGridFunction & gcf = multigridLevel[level];
        if( gcf.mappedGridFunctionList.getLength()<gridNumber(level)+1 )
	{
          wasChanged(level)=TRUE;
  	  gcf.mappedGridFunctionList.addElement(mappedGridFunctionList[grid],gridNumber(level));
	}
	else
	{ // reference mappedGridFunction if it is not already there (no need to update grid function)
          gcf[gridNumber(level)].reference(mappedGridFunctionList[grid]);
	}
        gridNumber(level)++;
      } 	

      int level;
      for( level=0; level< gc.numberOfMultigridLevels(); level++ )
      {  // remove entries from multigridLevel[level] if it is too long
        while( multigridLevel[level].mappedGridFunctionList.getLength() > gc.multigridLevel[level].numberOfGrids() )
        {
          wasChanged(level)=TRUE;
	  multigridLevel[level].mappedGridFunctionList.deleteElement();
	}
      }

      Range *R = rcData->R;
      for( level=0; level< gc.numberOfMultigridLevels(); level++ )
      { // finish updating the grid collection if it was changed
        intCompositeGridFunction & gcf = multigridLevel[level];
	gcf.gridCollection = & gc.multigridLevel[level];
	gcf.gridCollectionData = gc.multigridLevel[level].rcData;
	if( wasChanged(level) )
	{
          // cout << "intCompositeGridFunction:updateCollections...multigridLevel was changed\n";

	  gcf.gridCollection = & gc.multigridLevel[level];
	  gcf.constructor( gcf.gridCollection,gcf.gridCollection->rcData,
			   R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7],FALSE );
	}
      }
    }
  }

  intGridCollectionFunction::updateCollections();  // *wdh* 000628 to build collections in intGridCollectionFunction class
  

  return 0;
}


// --- functions needed to avoid compiler warnings.

void intCompositeGridFunction::
link(const intGridCollectionFunction & gcf, // *New*
	    const Range & R0,
	    const Range & R1 /* = nullRange */,
	    const Range & R2 /* = nullRange */,
	    const Range & R3 /* = nullRange */,
     const Range & R4 /* = nullRange */ )
{
  if( gcf.getClassName()=="intCompositeGridFunction" )
  {
    link( (intCompositeGridFunction&)gcf,R0,R1,R2,R3,R4);
  }
  else
  {
    printf("intCompositeGridFunction::ERROR:This function should not be called\n");
    Overture::abort("error");
  }
}


void intCompositeGridFunction:: 
link(const intGridCollectionFunction & gcf, 
     const int componentToLinkTo,
     const int numberOfComponents )
{
  if( gcf.getClassName()=="intCompositeGridFunction" )
  {
    link( (intCompositeGridFunction&)gcf,componentToLinkTo,numberOfComponents);
  }
  else
  {
    printf("intCompositeGridFunction::ERROR:This function should not be called\n");
    Overture::abort("error");
  }
}


void intCompositeGridFunction:: 
reference( const intGridCollectionFunction & cgf )
{
  if( cgf.getClassName()=="intCompositeGridFunction" )
  {
    reference( (CompositeGrid&)cgf );
  }
  else
  {
    printf("intCompositeGridFunction::ERROR:This function should not be called\n");
    Overture::abort("error");
  }
}

  
intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGrid(GridCollectionData & gridData, 
		  const Range & R0, 
		  const Range & R1 /* = nullRange */,
		  const Range & R2 /* = nullRange */,
		  const Range & R3 /* = nullRange */,
		  const Range & R4 /* = nullRange */,
		  const Range & R5 /* = nullRange */,
		  const Range & R6 /* = nullRange */,
		  const Range & R7 /* = nullRange */ )
{
  printf("intCompositeGridFunction::ERROR:This function should not be called\n");
  if( this!=NULL )
    Overture::abort("error");
  return updateNoChange;
}

  
intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGrid(GridCollection & gc, 
		  const Range & R0, 
		  const Range & R1 /* = nullRange */,
		  const Range & R2 /* = nullRange */,
		  const Range & R3 /* = nullRange */,
		  const Range & R4 /* = nullRange */,
		  const Range & R5 /* = nullRange */,
		  const Range & R6 /* = nullRange */,
		  const Range & R7 /* = nullRange */ )
{
  if( gc.getClassName()=="CompositeGrid" )
  {
    updateToMatchGrid( (CompositeGrid&)gc,R0,R1,R2,R3,R4,R5,R6,R7);
  }
  else
  {
    printf("intCompositeGridFunction::ERROR:This function should not be called\n");
    Overture::abort("error");
  }
  return updateNoChange;
}


intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGrid(GridCollection & gc, 
		  const int  & i0, 
		  const Range & R1 /* = nullRange */,
		  const Range & R2 /* = nullRange */,
		  const Range & R3 /* = nullRange */,
		  const Range & R4 /* = nullRange */,
		  const Range & R5 /* = nullRange */,
		  const Range & R6 /* = nullRange */,
		  const Range & R7 /* = nullRange */ )
{
  if( gc.getClassName()=="CompositeGrid" )
  {
    updateToMatchGrid( (CompositeGrid&)gc,i0,R1,R2,R3,R4,R5,R6,R7);
  }
  else
  {
    printf("intCompositeGridFunction::ERROR:This function should not be called\n");
    Overture::abort("error");
  }
  return updateNoChange;
}


  
intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGrid(GridCollection & gc, 
		  const GridFunctionParameters::GridFunctionType & type, 
		  const Range & component0,
		  const Range & component1 /* = nullRange */,
		  const Range & component2 /* = nullRange */,
		  const Range & component3 /* = nullRange */,
		  const Range & component4 /* = nullRange */ )
{
  if( gc.getClassName()=="CompositeGrid" )
  {
    updateToMatchGrid( (CompositeGrid&)gc,type,component0,component1,component2,component3,component4 );
  }
  else
  {
    printf("intCompositeGridFunction::ERROR:This function should not be called\n");
    Overture::abort("error");
  }
  return updateNoChange;
}


intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGrid(GridCollection & gc, 
		  const GridFunctionParameters::GridFunctionType & type)
{
  if( gc.getClassName()=="CompositeGrid" )
  {
    updateToMatchGrid( (CompositeGrid&)gc,type );
  }
  else
  {
    printf("intCompositeGridFunction::ERROR:This function should not be called\n");
    Overture::abort("error");
  }
  return updateNoChange;
}

intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGrid(GridCollection & gc )
{
  if( gc.getClassName()=="CompositeGrid" )
  {
    updateToMatchGrid( (CompositeGrid&)gc );
  }
  else
  {
    printf("intCompositeGridFunction::ERROR:This function should not be called\n");
    Overture::abort("error");
  }
  return updateNoChange;
}
  
intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGrid(GridCollectionData & grid)
{
  printf("intCompositeGridFunction::ERROR:This function should not be called\n");
  if( this!=NULL )
    Overture::abort("error");
  return updateNoChange;
}
  


// update this grid function to match another grid function
intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGridFunction(const intGridCollectionFunction & cgf, 
			  const Range & R0, 
			  const Range & R1 /* = nullRange */,
			  const Range & R2 /* = nullRange */,
			  const Range & R3 /* = nullRange */,
			  const Range & R4 /* = nullRange */,
			  const Range & R5 /* = nullRange */,
			  const Range & R6 /* = nullRange */,
			  const Range & R7 /* = nullRange */ )
{
  if( cgf.getClassName()=="intCompositeGridFunction" )
  {
    updateToMatchGridFunction( (intCompositeGridFunction&)cgf,R0,R1,R2,R3,R4,R5,R6,R7 );
  }
  else
  {
    printf("intCompositeGridFunction::ERROR:This function should not be called\n");
    Overture::abort("error");
  }
  return updateNoChange;
}
  


intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchGridFunction(const intGridCollectionFunction & gcf)
{
  if( gcf.getClassName()=="intCompositeGridFunction" )
  {
    updateToMatchGridFunction( (intCompositeGridFunction&)gcf );
  }
  else
  {
    printf("intCompositeGridFunction::ERROR:This function should not be called\n");
    Overture::abort("error");
  }
  return updateNoChange;
}
  

// make sure the number of mappedGridFunction's is correct
intCompositeGridFunction::updateReturnValue intCompositeGridFunction::
updateToMatchNumberOfGrids(GridCollection& gc)
{
  if( gc.getClassName()=="CompositeGrid" )
  {
    updateToMatchNumberOfGrids( (CompositeGrid&)gc );
  }
  else
  {
    printf("intCompositeGridFunction::ERROR:This function should not be called\n");
    Overture::abort("error");
  }
  return updateNoChange;
}
  







