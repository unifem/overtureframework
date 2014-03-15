#include "CompositeGridHybridConnectivity.h"

//\begin{>CompositeGridHybridConnectivityImp.tex}{\subsection{Default Constructor}}
CompositeGridHybridConnectivity::
CompositeGridHybridConnectivity()
// /Purpose : Default constructor that nullifies all pointers
//\end{CompositeGridHybridConnectivityImp.tex}
{

  gridIndex2UnstructuredVertex = NULL;
  gridVertex2UnstructuredVertex = NULL;
  grid = -1;

}

//\begin{>>CompositeGridHybridConnectivityImp.tex}{\subsection{Initialized constructor}}
CompositeGridHybridConnectivity::
CompositeGridHybridConnectivity(const int &grid_,
				intArray * gridIndex2UVertex_,
				intArray & uVertex2GridIndex_,
				intArray * gridVertex2UVertex_,
				intArray & boundaryFaceMapping_)
// /Purpose : This constructor takes existing connectivity data and constructs a new hybrid connectivity
// /grid (input) : the grid number in cg that contains the unstructured mesh referred to by this connectivity
// /gridIndex2UVertex\_ (input) : an array of IntegerArrays the length of the number of grids in cg\_; This data maps a structured vertex in a particular strutured grid into the corresponding unstructured vertex id, if the mapping exists (ie, if the structured vertex lies on the hybrid interface)
// /uVertex2GridIndex\_ (input) : maps an unstructured boundary vertex into a structured grid in cg\_ and the corresponding indices on that grid
// /gridVertex2UVertex\_ (input) : an array of IntegerArrays the length of the number of grids in cg\_; a condensation of vertexIDMapping, this contains a list of all the interface vertices for each grid and the unstructured vertices they map to
// /boundaryFaceMapping\_ (input) : for each boundary element in the unstructured part of the hybrid mesh this data structure contains the grid and indices for the adjacent structured zone.
// /Returns : void
// /Throws  : CompositeGridHybridConnectivityError is thrown through this method by setCompositeGridHybridConnectivity
//\end{CompositeGridHybridConnectivityImp.tex}
{

  setCompositeGridHybridConnectivity(grid_,
				     gridIndex2UVertex_,
				     uVertex2GridIndex_,
				     gridVertex2UVertex_,
				     boundaryFaceMapping_);

}

CompositeGridHybridConnectivity::
~CompositeGridHybridConnectivity()
{
  destroy();
}

//! return the size in bytes of this object.
real CompositeGridHybridConnectivity::
sizeOf(FILE *file /* = NULL */ ) const
{
  real size=sizeof(*this);
  // if( gridIndex2UnstructuredVertex!=NULL )  size+=gridIndex2UnstructuredVertex->elementCount()*sizeof(int);
  // if( gridVertex2UnstructuredVertex!=NULL )  size+=gridVertex2UnstructuredVertexelementCount()*sizeof(int);
  size+=unstructuredVertex2GridIndex.elementCount()*sizeof(int);
  size+=boundaryFaceMapping.elementCount()*sizeof(int);

  return size;
}


//\begin{>>CompositeGridHybridConnectivityImp.tex}{\subsection{setCompositeGridHybridConnectivity}}
void
CompositeGridHybridConnectivity::
setCompositeGridHybridConnectivity(const int &grid_,
				   intArray * gridIndex2UVertex_,
				   intArray & uVertex2GridIndex_,
				   intArray * gridVertex2UVertex_,
				   intArray & boundaryFaceMapping_)
// /Purpose : This method builds fills the {\tt CompositeGridHybridConnectivity} data structures
//            with externally computed index arrays.  Generally these arrays are computed 
//            during the generation of a hybrid mesh by methods in {\tt Ugen} and {\tt AdvancingFront}. 
// /grid (input) : the grid number in cg that contains the unstructured mesh referred to by this connectivity
// /gridIndex2UVertex\_ (input) : an array of IntegerArrays the length of the number of grids in cg\_; This data maps a structured vertex in a particular strutured grid into the corresponding unstructured vertex id, if the mapping exists (ie, if the structured vertex lies on the hybrid interface)
// /uVertex2GridIndex\_ (input) : maps an unstructured boundary vertex into a structured grid in cg\_ and the corresponding indices on that grid
// /gridVertex2UVertex\_ (input) : an array of IntegerArrays the length of the number of grids in cg\_; a condensation of vertexIDMapping, this contains a list of all the interface vertices for each grid and the unstructured vertices they map to
// /boundaryFaceMapping\_ (input) : for each boundary element in the unstructured part of the hybrid mesh this data structure contains the grid and indices for the adjacent structured zone.
// /Returns : void
// /Throws  : 
// \itemize
//  \item CompositeGridHybridConnectivityError
//\end{itemize}
//\end{CompositeGridHybridConnectivityImp.tex}
{
#if 0
  const MappedGrid &g = cgg[grid];

  if (if g.mapping().getClassName()!="UnstructuredMapping") throw CompositeGridHybridConnectivityError();

  if (cg_ == NULL) 
    {
      CompositeGridHybridConnectivityError err;
      err.message = "NULL CompositeGrid pointer in constructor";
      throw err;
    } else {
      cg = cg_;
    }
#endif

  grid = grid_;

  if (gridIndex2UVertex_ == NULL) 
    {
      CompositeGridHybridConnectivityError err;
      err.message = "NULL vertexIDMapping pointer in constructor";
      throw err;
    } else {
      gridIndex2UnstructuredVertex = gridIndex2UVertex_;
    }

  if (gridVertex2UVertex_ == NULL) 
    {
      CompositeGridHybridConnectivityError err;
      err.message = "NULL vertexIndex pointer in constructor";
      throw err;
    } else {
      gridVertex2UnstructuredVertex = gridVertex2UVertex_;
    }

  unstructuredVertex2GridIndex.redim(0);
  unstructuredVertex2GridIndex = uVertex2GridIndex_;
  boundaryFaceMapping.redim(0);
  boundaryFaceMapping = boundaryFaceMapping_;
}

//\begin{>>CompositeGridHybridConnectivityImp.tex}{\subsection{destroy}}
void 
CompositeGridHybridConnectivity::
destroy()
// /Purpose : destroy the hybrid connectivity data structures, calls delete on the allocated arrays of IntegerArrays
// /Throws : nothing
//\end{CompositeGridHybridConnectivityImp.tex}
{
  if( false ) // *wdh* turn this off for now -- 040808 -- there is a problem -- see testIntegrate
  {
    if (gridIndex2UnstructuredVertex!=NULL) 
    { 
      delete [] gridIndex2UnstructuredVertex;
      gridIndex2UnstructuredVertex = NULL;
    }
    if (gridVertex2UnstructuredVertex != NULL) 
    {
      delete [] gridVertex2UnstructuredVertex;
      gridVertex2UnstructuredVertex = NULL;
    }
  }
}

// get the vertex id mapping for a particular grid 
//\begin{>>CompositeGridHybridConnectivityImp.tex}{\subsection{getGridIndex2UVertex}}
const intArray & 
CompositeGridHybridConnectivity::
getGridIndex2UVertex(int grid_) const
// /Purpose : get the vertex id mapping for a particular grid
// /grid (input) : the requested grid 
// /Returns :  a const IntegerArray reference to the IntegerArray with grid's connectivity to the unstructured mesh
// /Throws : nothing (!) (but should perform a check on the validity of grid and throw an appropriate error)
//\end{CompositeGridHybridConnectivityImp.tex}
{
  return gridIndex2UnstructuredVertex[grid_];
}

// get vertexIndexMapping (inverse of vertexIDMapping)
//\begin{>>CompositeGridHybridConnectivityImp.tex}{\subsection{getUVertex2GridIndex}}
const intArray & 
CompositeGridHybridConnectivity::
getUVertex2GridIndex() const
// /Purpose : get the unstructured mesh vertex index mapping
// /Returns :  a const IntegerArray reference to the unstructred mesh's connectivity to vertices in the structured grids
// /Throws : nothing 
//\end{CompositeGridHybridConnectivityImp.tex}
{
  return unstructuredVertex2GridIndex;
}

// get the indices on a particular grid that are on the structured/unstructured boundary
//\begin{>>CompositeGridHybridConnectivityImp.tex}{\subsection{getGridVertex2UVertex}}
const intArray & 
CompositeGridHybridConnectivity::
getGridVertex2UVertex(int grid_) const
// /Purpose : get the indices on a particular grid that are on the structured/unstructured boundary
// /grid (input) : the requested grid 
// /Returns :  a const IntegerArray reference to the IntegerArray with grid's connectivity to the unstructured mesh, but only for those verticies sitting on the structured/unstructured interface
// /Throws : nothing (!) (but should perform a check on the validity of grid and throw an appropriate error)
//\end{CompositeGridHybridConnectivityImp.tex}
{
  return gridVertex2UnstructuredVertex[grid_];
}

// return the number of vertices on the structured/unstructred interface of a particular grid
//\begin{>>CompositeGridHybridConnectivityImp.tex}{\subsection{getNumberOfInterfaceVertices}}
int 
CompositeGridHybridConnectivity::
getNumberOfInterfaceVertices(int grid_) const
// /Purpose : get the number of vertices on the structured/unstructured interface of a particular structured grid; this method is need for iterations through the result of {\tt getVertexindex}
// /grid (input) : the requested grid 
// /Returns : int; the number of vertices on the hybrid structured/unstructured interface for {\tt grid}
// /Throws : nothing (!) (but should perform a check on the validity of grid and throw an appropriate error)
//\end{CompositeGridHybridConnectivityImp.tex}
{
  return gridVertex2UnstructuredVertex[grid_].getLength(0);
}

//\begin{>>CompositeGridHybridConnectivityImp.tex}{\subsection{getBoundaryFaceMapping}}
const intArray &
CompositeGridHybridConnectivity::
getBoundaryFaceMapping() const
// /Purpose : the an integerarray containing the structured faces on the boundary of the unstructured region
// /Returns : const IntegerArray \&; the boundary face mapping array, 2nd dimension is length 4 (0 - grid, 1 - i1, 2 - i2, 3 - i3)
//\end{CompositeGridHybridConnectivityImp.tex}
{
  return boundaryFaceMapping;
}
