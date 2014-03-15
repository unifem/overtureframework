#include "arrayGetIndex.h"
#include "OvertureInit.h"

void 
OGcheckIndex( const IntegerArray & indexArray, const aString & functionName )
{
  if( indexArray.getLength(0)<2 || indexArray.getLength(1)<3 )
  {
    printf("%s: ERROR: the indexArray is not dimensioned large enough. Should be (0:1,0:2)\n",
	   (const char *)functionName);
    Overture::abort("error");
  }
}
  


//\begin{>OGgetIndexInclude.tex}{\subsection{getIndex from an index array}}   
void 
getIndex(const IntegerArray & indexArray, 
	 Index & I1, 
	 Index & I2, 
	 Index & I3, 
	 int extra1, /* =0 */
	 int extra2, /* =OGgetIndexDefaultValue */
	 int extra3  /* =OGgetIndexDefaultValue */ )
//---------------------------------------------------------------------------------------------
// /Description:
//   Return Index objects for the region defined by indexArray
//
//  /indexArray(0\collon1,0\collon2) (input): defines a region
//  /I1,I2,I3 (output): Index values for the region
//  /extra1,extra2,extra3 (input): increase region by this many lines, by default extra1=0, while 
//                         extra2 and extra3 default to extra1 (so that if you only set extra1=1
//                         then by default extra2=extra3=1)
// /Author: WDH
//\end{OGgetIndexInclude.tex}  
//---------------------------------------------------------------------------------------------
{
  OGcheckIndex( indexArray,"getIndex" );
  
  int e1= extra1;
  int e2= extra2==arrayGetIndexDefaultValue ? extra1 : extra2;  // extra2=extra0 by default
  int e3= extra3==arrayGetIndexDefaultValue ? extra1 : extra3;  // extra3=extra0 by default
  
      
  I1=Range(indexArray(Start,axis1)-e1,indexArray(End,axis1)+e1);
  I2=(indexArray(Start,axis2)!=indexArray(End,axis2))?
	Range(indexArray(Start,axis2)-e2,indexArray(End,axis2)+e2):
	Range(indexArray(Start,axis2)   ,indexArray(End,axis2)   );
  I3=(indexArray(Start,axis3)!=indexArray(End,axis3))?
	Range(indexArray(Start,axis3)-e3,indexArray(End,axis3)+e3):
	Range(indexArray(Start,axis3)   ,indexArray(End,axis3)   );
}

//\begin{>>OGgetIndexInclude.tex}{\subsection{getBoundaryIndex from an index array}} 
void 
getBoundaryIndex(const IntegerArray & indexArray, 
		 int side, 
		 int axis, 
		 Index & Ib1, 
                 Index & Ib2, 
		 Index & Ib3, 
		 int extra1, /* =0 */
		 int extra2, /* =arrayGetIndexDefaultValue */
		 int extra3  /* =arrayGetIndexDefaultValue */ )
//---------------------------------------------------------------------------------------------
// /Description:
//   return Index objects for a side of the region defined by indexArray
//
//  /indexArray(0\collon1,0\collon2) (input): defines a region
//  /side,axis (input): defines which side=0,1 and axis=0,1,2
//  /Ib1,Ib2,Ib3 (output): Index values for the given boundary of the region
//  /extra1,extra2,extra3 (input): increase region by this many lines, by default extra1=0, while 
//                         extra2 and extra3 default to extra1 (so that if you only set extra1=1
//                         then by default extra2=extra3=1)
//\end{OGgetIndexInclude.tex} 
//---------------------------------------------------------------------------------------------
{
  OGcheckIndex( indexArray,"getBoundaryIndex" );
  int e1= extra1;
  int e2= extra2==arrayGetIndexDefaultValue ? extra1 : extra2;  // extra2=extra0 by default
  int e3= extra3==arrayGetIndexDefaultValue ? extra1 : extra3;  // extra3=extra0 by default

  Range R[3];   
  R[0]=Range(indexArray(Start,axis1)-e1,indexArray(End,axis1)+e1); 
  R[1]=(indexArray(Start,axis2)!=indexArray(End,axis2))?
	Range(indexArray(Start,axis2)-e2,indexArray(End,axis2)+e2):
	Range(indexArray(Start,axis2)   ,indexArray(End,axis2)   );
  R[2]=(indexArray(Start,axis3)!=indexArray(End,axis3))?
	Range(indexArray(Start,axis3)-e3,indexArray(End,axis3)+e3):
	Range(indexArray(Start,axis3)   ,indexArray(End,axis3)   );
  R[axis]= side==0? Range(indexArray(Start,axis),indexArray(Start,axis)):
  Range(indexArray(End  ,axis),indexArray(End  ,axis));
  Ib1=R[0];  
  Ib2=R[1];  
  Ib3=R[2];   
}

//\begin{>>OGgetIndexInclude.tex}{\subsection{getGhostIndex from an index array}} 
void 
getGhostIndex(const IntegerArray & indexArray, 
	      int side, 
	      int axis, 
	      Index & Ig1, 
	      Index & Ig2, 
	      Index & Ig3, 
	      int ghostLine, /* =1 */
	      int extra1,    /* =0 */
	      int extra2,    /* =arrayGetIndexDefaultValue */
	      int extra3     /* =arrayGetIndexDefaultValue */ )
//---------------------------------------------------------------------------------------------
// /Description:
//    Get Index's corresponding to a given ghost-line
//
//  /indexArray(0\collon1,0\collon2) (input): defines a region
//  /side,axis (input): defines which side=0,1 and axis=0,1,2
//  /Ig1,Ig2,Ig3 (output): Index values for the given ghostline of the region
//  /ghostline (input): get Index's for this ghost line, can be positive, negative or zero.
//        A value of zero would give the boundary, a value of 1 would give the first
//        line outside and a value of -1 would give the first line inside.
//  /extra1,extra2,extra3 (input): increase region by this many lines, by default extra1=0, while 
//                         extra2 and extra3 default to extra1 (so that if you only set extra1=1
//                         then by default extra2=extra3=1)
//\end{OGgetIndexInclude.tex} 
//---------------------------------------------------------------------------------------------
{
  OGcheckIndex( indexArray,"getGhostIndex" );
  int e1= extra1;
  int e2= extra2==arrayGetIndexDefaultValue ? extra1 : extra2;  // extra2=extra0 by default
  int e3= extra3==arrayGetIndexDefaultValue ? extra1 : extra3;  // extra3=extra0 by default

  Range R[3];   
  R[0]=Range(indexArray(Start,axis1)-e1,indexArray(End,axis1)+e1); 
  R[1]=(indexArray(Start,axis2)!=indexArray(End,axis2))?
	Range(indexArray(Start,axis2)-e2,indexArray(End,axis2)+e2):
	Range(indexArray(Start,axis2)   ,indexArray(End,axis2)      );
  R[2]=(indexArray(Start,axis3)!=indexArray(End,axis3))?
	Range(indexArray(Start,axis3)-e3,indexArray(End,axis3)+e3):
	Range(indexArray(Start,axis3)   ,indexArray(End,axis3)      );
  R[axis]= side==0? Range(indexArray(Start,axis)-(ghostLine),indexArray(Start,axis)-(ghostLine)):  
                    Range(indexArray(End  ,axis)+(ghostLine),indexArray(End  ,axis)+(ghostLine));  
  Ig1=R[0];   
  Ig2=R[1];   
  Ig3=R[2];  
}



