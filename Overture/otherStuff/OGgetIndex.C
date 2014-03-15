#include "OGgetIndex.h"
#include "MappedGridFunction.h"
#include "UnstructuredMapping.h" // kkc need this for unstructured grid ranges
//=================================================================================================
//  Define some useful functions for getting A++ indicies for Overture
//=================================================================================================


static Range R[3];   // local version for this file

//---------------------------------------------------------------------------------------------
//             Get Index's for a gridFunction
//    -- base the Index's on the cell-centered-ness of component ---
//  Input
//    u,component,extra
//  Output
//    I1,I2,I3
//  Notes:
//   o Index's are based on indexRange
//   o if the grid and grid function are centred in the same way just use indexRange
//   o if the grid is cell centred but the gridFunction is vertex centred then add one point 
//   o if the grid is vertex centred but the gridFunction is cell centred then remove one point
//   o kkc - 031223 - if the grid is based on an UnstructuredMapping the above 4 points are ignored
//                    and the indices are taken from the centering info passed into getIndex
//   o kkc - 031223 - current extra[123] have are ignored for unstructured grids
//
//---------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------
// ------- This version is called by the versions for <float,double,int>MappedGridFunctions ------
//------------------------------------------------------------------------------------------------
void 
getIndex(const MappedGrid & c,                       
         int positionOfFaceCentering, 
         const IntegerArray & positionOfComponent,
         const IntegerArray & isCellCentered,
	 int component, 
	 Index & I1, 
	 Index & I2, 
	 Index & I3, 
	 int extra1, /* =0 */
	 int extra2, /* =OGgetIndexDefaultValue */
	 int extra3  /* =OGgetIndexDefaultValue */ )
{

  IntegerArray e(3);
  e(axis1)= extra1;
  e(axis2)= extra2==OGgetIndexDefaultValue ? extra1 : extra2;  // extra2=extra0 by default
  e(axis3)= extra3==OGgetIndexDefaultValue ? extra1 : extra3;  // extra3=extra0 by default

  IntegerArray c0(8); 
  for( int i=0; i<3; i++ )
    c0(positionOfComponent(i))=isCellCentered.getBase(i+1);   // default values

  if( positionOfFaceCentering>=0 )  
    c0(positionOfFaceCentering)=max(0,min(component,c.numberOfDimensions()-1)); // use this if GF is face centered:
  else
    c0(positionOfComponent(0))=component;    // use this one otherwise

  // kkc 031223 added isStructured to get around index offsets (for centering) when using an unstructured grid
  //            ??? should this be taken care of by the centering info in the MappedGrid and mappedGridFunction?
  bool isStructured = c.getGridType()==MappedGrid::structuredGrid;

  for( int axis=0; axis<c.numberOfDimensions(); axis++ )
  {
    if( c.isCellCentered(axis) && isStructured &&
        !isCellCentered(axis,c0(positionOfComponent(0)),c0(positionOfComponent(1)),c0(positionOfComponent(2))) )
    {
      // grid function is vertex, grid is cell
      R[axis]=Range(c.indexRange(Start,axis)-e(axis),c.indexRange(End,axis)+1+e(axis)); 
    }
    else if( !c.isCellCentered(axis) && isStructured &&
        isCellCentered(axis,c0(positionOfComponent(0)),c0(positionOfComponent(1)),c0(positionOfComponent(2))) )
    {
      // grid function is cell, grid is vertex
      R[axis]=Range(c.indexRange(Start,axis)-e(axis),c.indexRange(End,axis)-1+e(axis)); 
    }
    else if ( isStructured )
    { // grid and gridFunction have same centredness:
      R[axis]=Range(c.indexRange(Start,axis)-e(axis),c.indexRange(End,axis)+e(axis)); 
    }
    else if ( axis==0 )
      { // we have to actually check what kind of centering was passed in and set the ranges accordingly
	UnstructuredMapping &umap = (UnstructuredMapping &)c.mapping().getMapping(); 
	if ( positionOfFaceCentering>=0 )
	  {
	    R[axis] = c.numberOfDimensions()==2 ? umap.size(UnstructuredMapping::Edge) : umap.size(UnstructuredMapping::Face);
	  }
	else if ( isCellCentered(axis) )
	  {
	    R[axis] = c.numberOfDimensions()==1 ? umap.size(UnstructuredMapping::Edge) :
	      c.numberOfDimensions()==2 ? umap.size(UnstructuredMapping::Face) : umap.size(UnstructuredMapping::Region);
	  }
	else // assume vertex centred, but HEY, what about edge centered data? kkc
	  R[axis] = umap.size(UnstructuredMapping::Vertex);
	  
      }
    else
      R[axis] = 1;
    /* ----

    if( isCellCentered(axis,c0(positionOfComponent(0)),c0(positionOfComponent(1)),
                            c0(positionOfComponent(2)))==c.isCellCentered(axis) )
    { // grid and gridFunction have same centredness:
      R[axis]=Range(c.indexRange(Start,axis)-e(axis),c.indexRange(End,axis)+e(axis)); 
    }
    else   // grid and grid functions have different centred-ness
    {
      if( c.isCellCentered(axis) )  // grid function is vertex, grid is cell
        R[axis]=Range(c.indexRange(Start,axis)-e(axis),c.indexRange(End,axis)+1+e(axis)); 
      else                         // grid function is cell, grid is vertex
        R[axis]=Range(c.indexRange(Start,axis)-e(axis),c.indexRange(End,axis)-1+e(axis)); 
    }
    ---- */
  }
  if( c.numberOfDimensions()==2 )
    R[2]=Range(c.indexRange(Start,axis3),c.indexRange(End,axis3));

  I1=R[0];
  I2=R[1];
  I3=R[2];
}

//----------------------------------------------------------------------------------------------------
// Get Index's for this grid function -- base the Index's on the cell-centered-ness of component 0
//----------------------------------------------------------------------------------------------------
//\begin{>>OGgetIndexInclude.tex}{\subsection{getIndex from a \{float,double,int\}MappedGridFunction}} 
void 
getIndex(const floatMappedGridFunction & u, 
	 Index & I1, 
	 Index & I2, 
	 Index & I3,
	 int extra1,
	 int extra2,
	 int extra3 )
//=========================================================================================
// /Description:
// /u (input): Base the Index's on the {\tt indexRange} and cell-centredness associated   
//     with this grid function. Use the "first" component of the grid function, that is
//     use the base value for each component. To get Index's for a {\tt faceCenteredAll} grid function
//     use the {\tt getIndex} function described next.
// /I1,I2,I3 (output): Index values for the region
//  /extra1,extra2,extra3 (input): increase region by this many lines, by default extra1=0, while 
//                         extra2 and extra3 default to extra1 (so that if you only set extra1=1
//                         then by default extra2=extra3=1)
// /Author: WDH
//\end{OGgetIndexInclude.tex}
//=========================================================================================
{
  getIndex( u,0,I1,I2,I3,extra1,extra2,extra3 );
}
void 
getIndex(const doubleMappedGridFunction & u, 
	 Index & I1, 
	 Index & I2, 
	 Index & I3,
	 int extra1,
	 int extra2,
	 int extra3 )
{
  getIndex( u,0,I1,I2,I3,extra1,extra2,extra3 );
}
void 
getIndex(const intMappedGridFunction & u, 
	 Index & I1, 
	 Index & I2, 
	 Index & I3,
	 int extra1,
	 int extra2,
	 int extra3 )
{
  getIndex( u,0,I1,I2,I3,extra1,extra2,extra3 );
}

//\begin{>>OGgetIndexInclude.tex}{}
void 
getIndex(const floatMappedGridFunction & u, 
	 int component, 
	 Index & I1, 
	 Index & I2, 
	 Index & I3, 
	 int extra1,
	 int extra2,
	 int extra3 )
//=========================================================================================
// /Description:
// /u (input): Base the Index's on the {\tt indexRange} and cell-centredness associated   
//     with this grid function
// /component (input): use this component of the grid function, UNLESS the grid function 
//    is {\tt faceCenteredAll} in which case component =0,1 or 2 will indicate whether to return
//   Index's for the {\tt faceCenteredAxis1}, {\tt faceCenteredAxis2} or the {\tt faceCenteredAxis3} components.
// /I1,I2,I3 (output): Index values for the region
//  /extra1,extra2,extra3 (input): increase region by this many lines, by default extra1=0, while 
//                         extra2 and extra3 default to extra1 (so that if you only set extra1=1
//                         then by default extra2=extra3=1)
// /Author: WDH
//\end{OGgetIndexInclude.tex}
//=========================================================================================
{
  IntegerArray positionOfComponent(floatMappedGridFunction::maximumNumberOfIndicies);
  for( int i=0; i<floatMappedGridFunction::maximumNumberOfIndicies; i++ )
    positionOfComponent(i)=u.positionOfComponent(i);
  getIndex(*(u.mappedGrid),u.positionOfFaceCentering(),positionOfComponent,u.isCellCentered,
           component,I1,I2,I3,extra1,extra2,extra3);
}  
void 
getIndex(const doubleMappedGridFunction & u, 
	 int component, 
	 Index & I1, 
	 Index & I2, 
	 Index & I3, 
	 int extra1,
	 int extra2,
	 int extra3 )
{
  IntegerArray positionOfComponent(floatMappedGridFunction::maximumNumberOfIndicies);
  for( int i=0; i<floatMappedGridFunction::maximumNumberOfIndicies; i++ )
    positionOfComponent(i)=u.positionOfComponent(i);
  getIndex(*(u.mappedGrid),u.positionOfFaceCentering(),positionOfComponent,u.isCellCentered,
           component,I1,I2,I3,extra1,extra2,extra3);
}  
void 
getIndex(const intMappedGridFunction & u, 
	 int component, 
	 Index & I1, 
	 Index & I2, 
	 Index & I3, 
	 int extra1,
	 int extra2,
	 int extra3 )
{
  IntegerArray positionOfComponent(floatMappedGridFunction::maximumNumberOfIndicies);
  for( int i=0; i<floatMappedGridFunction::maximumNumberOfIndicies; i++ )
    positionOfComponent(i)=u.positionOfComponent(i);
  getIndex(*(u.mappedGrid),u.positionOfFaceCentering(),positionOfComponent,u.isCellCentered,
           component,I1,I2,I3,extra1,extra2,extra3);
}  



//\begin{>>OGgetIndexInclude.tex}{\subsection{getBoundaryIndex from a \{float,double,int\}MappedGridFunction}} 
void 
getBoundaryIndex(const floatMappedGridFunction & u, 
		 int side, 
		 int axis, 
		 Index & Ib1, 
                 Index & Ib2, 
		 Index & Ib3, 
		 int extra1, /* =0 */
		 int extra2, /* =OGgetIndexDefaultValue */
		 int extra3  /* =OGgetIndexDefaultValue */ )
//=========================================================================================
// /Description:
// /u (input): Base the Index's on the {\tt indexRange} and cell-centredness associated   
//     with this grid function. Use the "first" component of the grid function, that is
//     use the base value for each component.
// /side,axis (input): defines which side=0,1 and axis=0,1,2
// /I1,I2,I3 (output): Index values for the region
//  /extra1,extra2,extra3 (input): increase region by this many lines, by default extra1=0, while 
//                         extra2 and extra3 default to extra1 (so that if you only set extra1=1
//                         then by default extra2=extra3=1)
// /Author: WDH
//\end{OGgetIndexInclude.tex}
//=========================================================================================
{
  getGhostIndex( u,side,axis,Ib1,Ib2,Ib3,0,extra1,extra2,extra3 ); // for now *****
}
void 
getBoundaryIndex(const doubleMappedGridFunction & u, 
		 int side, 
		 int axis, 
		 Index & Ib1, 
                 Index & Ib2, 
		 Index & Ib3, 
		 int extra1,
		 int extra2,
		 int extra3 )
{
  getGhostIndex( u,side,axis,Ib1,Ib2,Ib3,0,extra1,extra2,extra3 ); // for now *****
}
void 
getBoundaryIndex(const intMappedGridFunction & u, 
		 int side, 
		 int axis, 
		 Index & Ib1, 
                 Index & Ib2, 
		 Index & Ib3, 
		 int extra1,
		 int extra2,
		 int extra3 )
{
  getGhostIndex( u,side,axis,Ib1,Ib2,Ib3,0,extra1,extra2,extra3 ); // for now *****
}

//\begin{>>OGgetIndexInclude.tex}{\subsection{getBoundaryIndex from a \{float,double,int\}MappedGridFunction}} 
void 
getBoundaryIndex(const intMappedGridFunction & u, 
                 int component,
		 int side, 
		 int axis, 
		 Index & Ib1, 
		 Index & Ib2, 
		 Index & Ib3, 
		 int extra1,
		 int extra2,
		 int extra3
		 )
//---------------------------------------------------------------------------------------------
// /Description:
//   return Index objects for a side of the region defined by indexArray
//
// /u (input): Base the Index's on the {\tt indexRange} and cell-centredness associated   
//     with this grid function. 
// /component (input): use this component of the grid function, UNLESS the grid function 
//    is {\tt faceCenteredAll} in which case component =0,1 or 2 will indicate whether to return
//   Index's for the {\tt faceCenteredAxis1}, {\tt faceCenteredAxis2} or the {\tt faceCenteredAxis3} components.
//  /side,axis (input): defines which side=0,1 and axis=0,1,2
//  /Ib1,Ib2,Ib3 (output): Index values for the given boundary of the region
//  /extra1,extra2,extra3 (input): increase region by this many lines, by default extra1=0, while 
//                         extra2 and extra3 default to extra1 (so that if you only set extra1=1
//                         then by default extra2=extra3=1)
//\end{OGgetIndexInclude.tex} 
//---------------------------------------------------------------------------------------------
{
  getGhostIndex( u,component,side,axis,Ib1,Ib2,Ib3,0,extra1,extra2,extra3 ); // for now *****
}
void 
getBoundaryIndex(const doubleMappedGridFunction & u, 
                 int component,
		 int side, 
		 int axis, 
		 Index & Ib1, 
		 Index & Ib2, 
		 Index & Ib3, 
		 int extra1,
		 int extra2,
		 int extra3
		 )
{
  getGhostIndex( u,component,side,axis,Ib1,Ib2,Ib3,0,extra1,extra2,extra3 ); // for now *****
}
void 
getBoundaryIndex(const floatMappedGridFunction & u, 
                 int component,
		 int side, 
		 int axis, 
		 Index & Ib1, 
		 Index & Ib2, 
		 Index & Ib3, 
		 int extra1,
		 int extra2,
		 int extra3
		 )
{
  getGhostIndex( u,component,side,axis,Ib1,Ib2,Ib3,0,extra1,extra2,extra3 ); // for now *****
}


//-------------------------------------------------------------------------------------------
//----Get the Index's for a ghost line based on the centred-ness of a grid function----------
//-------------------------------------------------------------------------------------------
void 
getGhostIndex(const MappedGrid & c,                       
              int positionOfFaceCentering, 
              const IntegerArray & positionOfComponent,
              const IntegerArray & isCellCentered,
	      int component,
	      int side, 
	      int axis0, 
	      Index & Ig1, 
	      Index & Ig2, 
	      Index & Ig3, 
	      int ghostLine, 
	      int extra1,
	      int extra2,
	      int extra3 )
{
  IntegerArray e(3);
  e(axis1)= extra1;
  e(axis2)= extra2==OGgetIndexDefaultValue ? extra1 : extra2;  // extra2=extra0 by default
  e(axis3)= extra3==OGgetIndexDefaultValue ? extra1 : extra3;  // extra3=extra0 by default

  IntegerArray c0(8); 
  for( int i=0; i<3; i++ )
    c0(positionOfComponent(i))=isCellCentered.getBase(i+1);   // default values

  if( positionOfFaceCentering>=0 )
    c0(positionOfFaceCentering)=component;   // use this component if the GF is face centered
  else
    c0(positionOfComponent(0))=component;    // use this one otherwise

  for( int axis=0; axis<c.numberOfDimensions(); axis++ )
  {
    if( c.isCellCentered(axis) && 
        !isCellCentered(axis,c0(positionOfComponent(0)),c0(positionOfComponent(1)),c0(positionOfComponent(2))) )
    {
      // grid function is vertex, grid is cell
      R[axis]=Range(c.indexRange(Start,axis)-e(axis),c.indexRange(End,axis)+1+e(axis)); 
    }
    else if( !c.isCellCentered(axis) && 
        isCellCentered(axis,c0(positionOfComponent(0)),c0(positionOfComponent(1)),c0(positionOfComponent(2))) )
    {
      // grid function is cell, grid is vertex
      R[axis]=Range(c.indexRange(Start,axis)-e(axis),c.indexRange(End,axis)-1+e(axis)); 
    }
    else
    { // grid and gridFunction have same centredness:
      R[axis]=Range(c.indexRange(Start,axis)-e(axis),c.indexRange(End,axis)+e(axis)); 
    }
    /* ----
    if( isCellCentered(axis,c0(positionOfComponent(0)),c0(positionOfComponent(1)),
                            c0(positionOfComponent(2)))==c.isCellCentered(axis) )
      R[axis]=Range(c.indexRange(Start,axis)-e(axis),c.indexRange(End,axis)+e(axis)); 
    else 
    {
      if( c.isCellCentered(axis) )  // grid function is vertex, grid is cell
        R[axis]=Range(c.indexRange(Start,axis)-e(axis),c.indexRange(End,axis)+1+e(axis)); 
      else                         // grid function is cell, grid is vertex
        R[axis]=Range(c.indexRange(Start,axis)-e(axis),c.indexRange(End,axis)-1+e(axis)); 
    }
    --- */
  }
  if( c.numberOfDimensions()==2 )
    R[2]=Range(c.indexRange(Start,axis3),c.indexRange(End,axis3));
 

  // now fix axis0

  if( c.isCellCentered(axis0) && 
      !isCellCentered(axis0,c0(positionOfComponent(0)),c0(positionOfComponent(1)),c0(positionOfComponent(2))) )
  {
    // grid function is vertex, grid is cell
    R[axis0]= side==0? Range(c.indexRange(Start,axis0)  -(ghostLine),c.indexRange(Start,axis0)  -(ghostLine)):  
                       Range(c.indexRange(End  ,axis0)+1+(ghostLine),c.indexRange(End  ,axis0)+1+(ghostLine));  
  }
  else if( !c.isCellCentered(axis0) && 
      isCellCentered(axis0,c0(positionOfComponent(0)),c0(positionOfComponent(1)),c0(positionOfComponent(2))) )
  {
    // grid function is cell, grid is vertex
    R[axis0]= side==0? Range(c.indexRange(Start,axis0)  -(ghostLine),c.indexRange(Start,axis0)  -(ghostLine)):  
                       Range(c.indexRange(End  ,axis0)-1+(ghostLine),c.indexRange(End  ,axis0)-1+(ghostLine));  
  }
  else
  { // grid and gridFunction have same centredness:
    R[axis0]= side==0? Range(c.indexRange(Start,axis0)-(ghostLine),c.indexRange(Start,axis0)-(ghostLine)):  
                       Range(c.indexRange(End  ,axis0)+(ghostLine),c.indexRange(End  ,axis0)+(ghostLine));  
  }
  Ig1=R[0];   
  Ig2=R[1];   
  Ig3=R[2];  
}

//\begin{>>OGgetIndexInclude.tex}{\subsection{getGhostIndex from a \{float,double,int\}MappedGridFunction}} 
void 
getGhostIndex(const floatMappedGridFunction & u, 
	      int side, 
	      int axis0, 
	      Index & Ig1, 
	      Index & Ig2, 
	      Index & Ig3, 
	      int ghostLine, /* =1 */
	      int extra1, /* =0 */
	      int extra2, /* =OGgetIndexDefaultValue */
	      int extra3  /* =OGgetIndexDefaultValue */ )
//=========================================================================================
// /Description:
//    Get Index's corresponding to a given ghost-line of region defined by a grid function.
// /u (input): Base the Index's on the {\tt indexRange} and cell-centredness associated   
//     with this grid function. Use the "first" component of the grid function, that is
//     use the base value for each component.
// /side,axis (input): defines which side=0,1 and axis=0,1,2
// /Ig1,Ig2,Ig3 (output): Index values for the given ghostline on the given side
// /ghostline (input): get Index's for this ghost line, can be positive, negative or zero.
//        A value of zero would give the boundary, a value of 1 would give the first
//        line outside and a value of -1 would give the first line inside.
//  /extra1,extra2,extra3 (input): increase region by this many lines, by default extra1=0, while 
//                         extra2 and extra3 default to extra1 (so that if you only set extra1=1
//                         then by default extra2=extra3=1)
// /Author: WDH
//\end{OGgetIndexInclude.tex}
//=========================================================================================
{
  getGhostIndex( u,0,side,axis0,Ig1,Ig2,Ig3,ghostLine,extra1,extra2,extra3 );
}
void 
getGhostIndex(const doubleMappedGridFunction & u, 
	      int side, 
	      int axis0, 
	      Index & Ig1, 
	      Index & Ig2, 
	      Index & Ig3, 
	      int ghostLine, 
	      int extra1,
	      int extra2,
	      int extra3 )
{
  getGhostIndex( u,0,side,axis0,Ig1,Ig2,Ig3,ghostLine,extra1,extra2,extra3 );
}
void 
getGhostIndex(const intMappedGridFunction & u, 
	      int side, 
	      int axis0, 
	      Index & Ig1, 
	      Index & Ig2, 
	      Index & Ig3, 
	      int ghostLine, 
	      int extra1,
	      int extra2,
	      int extra3 )
{
  getGhostIndex( u,0,side,axis0,Ig1,Ig2,Ig3,ghostLine,extra1,extra2,extra3 );
}

//\begin{>>OGgetIndexInclude.tex}{}
void 
getGhostIndex(const floatMappedGridFunction & u, 
	      int component,
	      int side, 
	      int axis, 
	      Index & Ig1, 
	      Index & Ig2, 
	      Index & Ig3, 
	      int ghostLine, /* =1 */
	      int extra1, /* =0 */
	      int extra2, /* =OGgetIndexDefaultValue */
	      int extra3  /* =OGgetIndexDefaultValue */ )
//=========================================================================================
// /Description:
//    Get Index's corresponding to a given ghost-line of region defined by a grid function.
// /u (input): Base the Index's on the {\tt indexRange} and cell-centredness associated   
//     with this grid function
// /component (input): use this component of the grid function, UNLESS the grid function 
//    is {\tt faceCenteredAll } in which case component =0,1 or 2 will indicate whether to return
//   Index's for the {\tt faceCenteredAxis1}, {\tt faceCenteredAxis2} or the {\tt faceCenteredAxis3} components.
// /side,axis (input): defines which side=0,1 and axis=0,1,2
// /Ig1,Ig2,Ig3 (output): Index values for the given ghostline on the given side
// /ghostline (input): get Index's for this ghost line, can be positive, negative or zero.
//        A value of zero would give the boundary, a value of 1 would give the first
//        line outside and a value of -1 would give the first line inside.
//  /extra1,extra2,extra3 (input): increase region by this many lines, by default extra1=0, while 
//                         extra2 and extra3 default to extra1 (so that if you only set extra1=1
//                         then by default extra2=extra3=1)
// /Author: WDH
//\end{OGgetIndexInclude.tex}
//=========================================================================================
{
  IntegerArray positionOfComponent(floatMappedGridFunction::maximumNumberOfIndicies);
  for( int i=0; i<floatMappedGridFunction::maximumNumberOfIndicies; i++ )
    positionOfComponent(i)=u.positionOfComponent(i);
  getGhostIndex(*(u.mappedGrid),u.positionOfFaceCentering(),positionOfComponent,u.isCellCentered,
                component,side,axis,Ig1,Ig2,Ig3,ghostLine,extra1,extra2,extra3);
}

void 
getGhostIndex(const doubleMappedGridFunction & u, 
	      int component,
	      int side, 
	      int axis, 
	      Index & Ig1, 
	      Index & Ig2, 
	      Index & Ig3, 
	      int ghostLine, 
	      int extra1,
	      int extra2,
	      int extra3 )
{
  IntegerArray positionOfComponent(floatMappedGridFunction::maximumNumberOfIndicies);
  for( int i=0; i<floatMappedGridFunction::maximumNumberOfIndicies; i++ )
    positionOfComponent(i)=u.positionOfComponent(i);
  getGhostIndex(*(u.mappedGrid),u.positionOfFaceCentering(),positionOfComponent,u.isCellCentered,
                component,side,axis,Ig1,Ig2,Ig3,ghostLine,extra1,extra2,extra3);
}

void 
getGhostIndex(const intMappedGridFunction & u, 
	      int component,
	      int side, 
	      int axis, 
	      Index & Ig1, 
	      Index & Ig2, 
	      Index & Ig3, 
	      int ghostLine, 
	      int extra1,
	      int extra2,
	      int extra3 )
{
  IntegerArray positionOfComponent(floatMappedGridFunction::maximumNumberOfIndicies);
  for( int i=0; i<floatMappedGridFunction::maximumNumberOfIndicies; i++ )
    positionOfComponent(i)=u.positionOfComponent(i);
  getGhostIndex(*(u.mappedGrid),u.positionOfFaceCentering(),positionOfComponent,u.isCellCentered,
                component,side,axis,Ig1,Ig2,Ig3,ghostLine,extra1,extra2,extra3);
}

//\begin{>>OGgetIndexInclude.tex}{extendedGridIndexRange}
IntegerArray
extendedGridIndexRange(const MappedGrid & mg)
// ==================================================================================
// /Description:
//   Return the extendedGridIndexRange which is equal to mg.gridIndexRange except on
//  interpolation boundaries where it is equal to mg.extendedIndexRange (i.e. it includes
//  the ghost points). NOTE: Does not include ghost points on mixed physical/interpolation bounadries
// /Author: WDH
//\end{OGgetIndexInclude.tex}
// ==================================================================================
{
  IntegerArray extendedGridIndexRange(2,3);
  extendedGridIndexRange=mg.gridIndexRange();
  for( int side=Start; side<=End; side++ )
  {
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      if( mg.boundaryCondition(side,axis)==0 )
      {
	extendedGridIndexRange(side,axis)=mg.extendedIndexRange(side,axis);
      }
    }
  }
  return extendedGridIndexRange;
}

//\begin{>>OGgetIndexInclude.tex}{extendedGridRange}
IntegerArray
extendedGridRange(const MappedGrid & mg)
// ==================================================================================
// /Description:
//   Return the extendedGridRange which is equal to mg.gridIndexRange except on
//  interpolation boundaries AND mixedPhyscialInterpolation boundaries
//  where it is equal to mg.extendedIndexRange (i.e. it includes
//  the ghost points). 
// /Author: WDH
//\end{OGgetIndexInclude.tex}
// ==================================================================================
{
  IntegerArray extendedGridRange(2,3);
  extendedGridRange=mg.gridIndexRange();
  for( int side=Start; side<=End; side++ )
  {
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      if( mg.boundaryCondition(side,axis)==0 || 
          mg.boundaryFlag(side,axis)==MappedGrid::mixedPhysicalInterpolationBoundary )
      {
	extendedGridRange(side,axis)=mg.extendedRange(side,axis);
      }
    }
  }
  return extendedGridRange;
}



/* -----

//-------------------------------------------------------------------------------------------------
// Get Index's for this grid function -- base the Index's on the cell-centered-ness of component
// and determine the Index range from the Index's (I1From,I2From,I3From) used with uFrom
//-------------------------------------------------------------------------------------------------
void 
getIndex(const floatMappedGridFunction & u, 
	 int component, 
	 Index & I1, 
	 Index & I2, 
	 Index & I3, 
         const floatMappedGridFunction & uFrom,   // determine Index's from this u
	 const Index & I1From,                    // and these Index's
	 const Index & I2From, 
	 const Index & I3From,
	 int extra1,
	 int extra2,
	 int extra3 )
{
  IntegerArray indexRange(2,3);  // create an indexRange from (I1From,I2From,I3From)
  indexRange(Start,axis1)=I1From.getBase();  indexRange(End,axis1)=I1From.getBound();
  indexRange(Start,axis2)=I2From.getBase();  indexRange(End,axis2)=I2From.getBound();
  indexRange(Start,axis3)=I3From.getBase();  indexRange(End,axis3)=I3From.getBound();
  
  IntegerArray e(3);
  e(axis1)= extra1;
  e(axis2)= extra2==OGgetIndexDefaultValue ? extra1 : extra2;  // extra2=extra0 by default
  e(axis3)= extra3==OGgetIndexDefaultValue ? extra1 : extra3;  // extra3=extra0 by default

  if( u.mappedGrid!=uFrom.mappedGrid )
  {
    cerr << "getIndex: ERROR: u and uFrom belong to different MappedGrids ! ";
  }

  const MappedGrid & c = *(u.mappedGrid);
  for( int axis=0; axis<u.numberOfDimensions(); axis++ )
  {
  if( u.isCellCentered(axis,component)==uFrom.isCellCentered(axis,component) )  // *** don't use == 
      R[axis]=Range(indexRange(Start,axis)-e(axis),indexRange(End,axis)+e(axis)); 
    else   // uFrom and u have different centred-ness
    {
      if( uFrom.isCellCentered(axis,component) )  // u is vertex, uFrom is cell
        R[axis]=Range(indexRange(Start,axis)-e(axis),indexRange(End,axis)+1+e(axis)); 
      else                         // u is cell, uFrom is vertex
        R[axis]=Range(indexRange(Start,axis)-e(axis),indexRange(End,axis)-1+e(axis)); 
    }
  }
  if( u.numberOfDimensions()==2 )
    R[2]=Range(indexRange(Start,axis3),indexRange(End,axis3));

  I1=R[0];
  I2=R[1];
  I3=R[2];
}
void 
getIndex(const doubleMappedGridFunction & u, 
	 int component, 
	 Index & I1, 
	 Index & I2, 
	 Index & I3, 
         const floatMappedGridFunction & uFrom,   // determine Index's from this u
	 const Index & I1From,                    // and these Index's
	 const Index & I2From, 
	 const Index & I3From,
	 int extra1,
	 int extra2,
	 int extra3 )
{
  IntegerArray indexRange(2,3);  // create an indexRange from (I1From,I2From,I3From)
  indexRange(Start,axis1)=I1From.getBase();  indexRange(End,axis1)=I1From.getBound();
  indexRange(Start,axis2)=I2From.getBase();  indexRange(End,axis2)=I2From.getBound();
  indexRange(Start,axis3)=I3From.getBase();  indexRange(End,axis3)=I3From.getBound();
  
  IntegerArray e(3);
  e(axis1)= extra1;
  e(axis2)= extra2==OGgetIndexDefaultValue ? extra1 : extra2;  // extra2=extra0 by default
  e(axis3)= extra3==OGgetIndexDefaultValue ? extra1 : extra3;  // extra3=extra0 by default

  if( u.mappedGrid!=uFrom.mappedGrid )
  {
    cerr << "getIndex: ERROR: u and uFrom belong to different MappedGrids ! ";
  }

  const MappedGrid & c = *(u.mappedGrid);
  for( int axis=0; axis<u.numberOfDimensions(); axis++ )
  {
    if( u.isCellCentered(axis,component)==uFrom.isCellCentered(axis,component) )
      R[axis]=Range(indexRange(Start,axis)-e(axis),indexRange(End,axis)+e(axis)); 
    else   // uFrom and u have different centred-ness
    {
      if( uFrom.isCellCentered(axis,component) )  // u is vertex, uFrom is cell
        R[axis]=Range(indexRange(Start,axis)-e(axis),indexRange(End,axis)+1+e(axis)); 
      else                         // u is cell, uFrom is vertex
        R[axis]=Range(indexRange(Start,axis)-e(axis),indexRange(End,axis)-1+e(axis)); 
    }
  }
  if( u.numberOfDimensions()==2 )
    R[2]=Range(indexRange(Start,axis3),indexRange(End,axis3));

  I1=R[0];
  I2=R[1];
  I3=R[2];
}
void 
getIndex(const intMappedGridFunction & u, 
	 int component, 
	 Index & I1, 
	 Index & I2, 
	 Index & I3, 
         const floatMappedGridFunction & uFrom,   // determine Index's from this u
	 const Index & I1From,                    // and these Index's
	 const Index & I2From, 
	 const Index & I3From,
	 int extra1,
	 int extra2,
	 int extra3 )
{
  IntegerArray indexRange(2,3);  // create an indexRange from (I1From,I2From,I3From)
  indexRange(Start,axis1)=I1From.getBase();  indexRange(End,axis1)=I1From.getBound();
  indexRange(Start,axis2)=I2From.getBase();  indexRange(End,axis2)=I2From.getBound();
  indexRange(Start,axis3)=I3From.getBase();  indexRange(End,axis3)=I3From.getBound();
  
  IntegerArray e(3);
  e(axis1)= extra1;
  e(axis2)= extra2==OGgetIndexDefaultValue ? extra1 : extra2;  // extra2=extra0 by default
  e(axis3)= extra3==OGgetIndexDefaultValue ? extra1 : extra3;  // extra3=extra0 by default

  if( u.mappedGrid!=uFrom.mappedGrid )
  {
    cerr << "getIndex: ERROR: u and uFrom belong to different MappedGrids ! ";
  }

  const MappedGrid & c = *(u.mappedGrid);
  for( int axis=0; axis<u.numberOfDimensions(); axis++ )
  {
    if( u.isCellCentered(axis,component)==uFrom.isCellCentered(axis,component) )
      R[axis]=Range(indexRange(Start,axis)-e(axis),indexRange(End,axis)+e(axis)); 
    else   // uFrom and u have different centred-ness
    {
      if( uFrom.isCellCentered(axis,component) )  // u is vertex, uFrom is cell
        R[axis]=Range(indexRange(Start,axis)-e(axis),indexRange(End,axis)+1+e(axis)); 
      else                         // u is cell, uFrom is vertex
        R[axis]=Range(indexRange(Start,axis)-e(axis),indexRange(End,axis)-1+e(axis)); 
    }
  }
  if( u.numberOfDimensions()==2 )
    R[2]=Range(indexRange(Start,axis3),indexRange(End,axis3));

  I1=R[0];
  I2=R[1];
  I3=R[2];
}


void 
getIndex(const floatMappedGridFunction & u, 
	 Index & I1, 
	 Index & I2, 
	 Index & I3, 
         const floatMappedGridFunction & uFrom,   // determine Index's from this u
	 const Index & I1From,                    // and these Index's
	 const Index & I2From, 
	 const Index & I3From,
	 int extra1,
	 int extra2,
	 int extra3 )
{
  getIndex( u,0,I1,I2,I3,uFrom,I1From,I2From,I3From,extra1,extra2,extra3 );
}
void 
getIndex(const doubleMappedGridFunction & u, 
	 Index & I1, 
	 Index & I2, 
	 Index & I3, 
         const floatMappedGridFunction & uFrom,   // determine Index's from this u
	 const Index & I1From,                    // and these Index's
	 const Index & I2From, 
	 const Index & I3From,
	 int extra1,
	 int extra2,
	 int extra3 )
{
  getIndex( u,0,I1,I2,I3,uFrom,I1From,I2From,I3From,extra1,extra2,extra3 );
}
void 
getIndex(const intMappedGridFunction & u, 
	 Index & I1, 
	 Index & I2, 
	 Index & I3, 
         const floatMappedGridFunction & uFrom,   // determine Index's from this u
	 const Index & I1From,                    // and these Index's
	 const Index & I2From, 
	 const Index & I3From,
	 int extra1,
	 int extra2,
	 int extra3 )
{
  getIndex( u,0,I1,I2,I3,uFrom,I1From,I2From,I3From,extra1,extra2,extra3 );
}

--- */
