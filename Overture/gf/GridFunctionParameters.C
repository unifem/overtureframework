#include "GridFunctionParameters.h"

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{Public enumerators}} 
//\no function header:
// 
// Here are the public enumerators:
// /GridFunctionType: Use these values to construct grid function of a given type.
//   Query the grid function to see what type it is with {\ff getGridFunctionType}.    
//  {\footnotesize
//  \begin{verbatim} 
//    enum GridFunctionType
//    {
//      general,
//      vertexCentered,
//      cellCentered,
//      faceCenteredAll,
//      faceCenteredAxis1,
//      faceCenteredAxis2,
//      faceCenteredAxis3
//      };
//  \end{verbatim} 
//  }
//  
// /GridFunctionTypeWithComponents: 
//   Query the grid function to see what type it is and how many components there are
//     with {\ff getGridFunctionTypeWithComponents}.
//  {\footnotesize
//  \begin{verbatim} 
//    enum GridFunctionTypeWithComponents
//    {
//      generalWith0Components,
//      generalWith1Component,
//      generalWith2Components,
//      generalWith3Components,
//      generalWith4Components,
//      generalWith5Components,
//  
//      vertexCenteredWith0Components,
//      vertexCenteredWith1Component,
//      vertexCenteredWith2Components,
//      vertexCenteredWith3Components,
//      vertexCenteredWith4Components,
//      vertexCenteredWith5Components,
//  
//      cellCenteredWith0Components,
//      cellCenteredWith1Component,
//      cellCenteredWith2Components,
//      cellCenteredWith3Components,
//      cellCenteredWith4Components,
//      cellCenteredWith5Components,
//  
//      faceCenteredAllWith0Components,
//      faceCenteredAllWith1Component,
//      faceCenteredAllWith2Components,
//      faceCenteredAllWith3Components,
//      faceCenteredAllWith4Components,
//      faceCenteredAllWith5Components,
//  
//      faceCenteredAxis1With0Components,
//      faceCenteredAxis1With1Component,
//      faceCenteredAxis1With2Components,
//      faceCenteredAxis1With3Components,
//      faceCenteredAxis1With4Components,
//      faceCenteredAxis1With5Components,
//  
//      faceCenteredAxis2With0Components,
//      faceCenteredAxis2With1Component,
//      faceCenteredAxis2With2Components,
//      faceCenteredAxis2With3Components,
//      faceCenteredAxis2With4Components,
//      faceCenteredAxis2With5Components,
//  
//      faceCenteredAxis3With0Components,
//      faceCenteredAxis3With1Component,
//      faceCenteredAxis3With2Components,
//      faceCenteredAxis3With3Components,
//      faceCenteredAxis3With4Components,
//      faceCenteredAxis3With5Components
//      };
//  \end{verbatim} 
//  }
//  
// /faceCenteringType:
//   Here are some standard types of face centred grid functions. Use {\ff setFaceCentering}
//   and {\ff getFaceCentering}.    
//  {\footnotesize
//  \begin{verbatim} 
//    enum faceCenteringType   
//    { 
//      none=-1,                // not face centred
//      direction0=0,           // all components are face centred along direction (i.e. axis) = 0
//      direction1=1,           // all components are face centred along direction (i.e. axis) = 1
//      direction2=2,           // all components are face centred along direction (i.e. axis) = 2
//      all=-2                  // components are face centred in all directions, positionOfFaceCentering determines
//    };                        // the Index position that is used for the "directions"
//  \end{verbatim} 
//  }
//  
//  
//\end{MappedGridFunctionInclude.tex}


GridFunctionParameters:: 
GridFunctionParameters() 
{ 
  inputType=defaultCentering; 
  outputType=defaultCentering; 
}

#define CASTING_CONSTRUCTOR(GFT) \
GridFunctionParameters::                                    \
GridFunctionParameters(const GFT & type)  \
{                                                           \
  inputType=defaultCentering;                              \
  outputType=(GridFunctionType)type;                         \
}

CASTING_CONSTRUCTOR(GridFunctionType)

GridFunctionParameters::
~GridFunctionParameters()
{
}


#define CAST_OPERATOR(GFTYPE) \
GridFunctionParameters:: \
operator GridFunctionParameters::GFTYPE () const  \
{  \
  return (GFTYPE)outputType; \
}

CAST_OPERATOR(GridFunctionType)

