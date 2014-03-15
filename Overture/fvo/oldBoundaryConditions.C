//==============================================================================
//\begin{>>boundaryConditions.tex}{\subsection{setNumberOfBoundaryConditions}} 
void MappedGridFiniteVolumeOperators::
setNumberOfBoundaryConditions (
			const int & number, 
			const int & side, 	// = forAll
			const int & axis)	// = forAll

//
// /Purpose:
// Indicate how many boundary conditions are to be applied on a given side
//
// /number:	number of boundary conditions
// /side:	side that this applies to. if = MappedGridFiniteVolumeOperators::forAll,
//		  then this BC applies to all sides
// /axis:	axis that this applies to. if = MappedGridFiniteVolumeOperators::forAll,
//			  then this BC applies to all axes
//
//\end{boundaryConditions.tex} 
//========================================
{
  Range Side = side==forAll ? Range(0,1) : Range(side,side);
  Range Axes = axis==forAll ? Range(0,2) : Range(axis,axis);

  numberOfBoundaryConditions (Side,Axes) = number;
  if (debug)  boundaryConditionDisplay.display (numberOfBoundaryConditions, "Here is the numberOfBoundaryConditions array:");
}

//==============================================================================
//\begin{>>boundaryConditions.tex}{\subsection{setBoundaryCondition for one component}} 
void MappedGridFiniteVolumeOperators::
setBoundaryCondition ( 	const int & index, 
			const int & side, 
			const int & axis,
			const boundaryConditionTypes & boundaryConditionType,
			const int & component)					// = 0



//
// /Purpose:
//   assign the index'th boundary condition for particular component of
//   a REALMappedGridFunction and for a particular (side,axis) of a
//   MappedGrid. This function specifies how the values in the ghost cells adjacent
//   to a boundary will be set. It does not actually set those values; that is done
//   by the routine {\bf applyBoundaryConditions}. This function (or one of its alternate versions) must
//   be called prior to a call to {\bf applyBoundaryConditions}. This routine is appropriate
//   for specifying boundary conditions that involve only one component of the solution.
//   Use {\bf setBoundaryCondition(int,int,int,boundaryConditionTypes,intArray)} to specify
//   boundary conditions that involve more than one component (such as {\bf normalComponent}
//   boundary conditions.  
//
// /index:	index (ie which) BC to set
// /side:	which side this applies to (if = MappedGridFiniteVolumeOperators::forALL,
//				then this applies to all sides
// /axis:	which axis this applies to (if = MappedGridFiniteVolumeOperators::forALL,
//				then this applies to all axes
//
// /boundaryConditionType:
//	  select from among
//        \begin{itemize} 
//	    \item{MappedGridFiniteVolumeOperators::dirichlet:}        
//                                   specify a value or a function along a side; use
//                                   {\bf setBoundaryConditionValue} or {setBoundaryConditionRightHandSide}
//                                   to specify the boundary value(s).  
//	    \item{MappedGridFiniteVolumeOperators::neumann:}          
//                                   set normal derivative along a side (typically to zero); use 
//                                   {\bf setBoundaryConditionValue} or {setBoundaryConditionRightHandSide}
//                                   to specify the value of the normal derivative  
//	    \item{MappedGridFiniteVolumeOperators::extrapolate:}      
//                                   extrapolate the ghost cell values 
//                                   from the interior by setting the third undivided difference
//                                   of the solution equal zero.
//         \end{itemize} 
//
// /component:	which component of your REALMappedGridFunction this BC will be applied to. 
//
// /Author:				D.L.Brown
// /Date Documentation Last Modified:	951130
//\end{boundaryConditions.tex}  
	//========================================
{
  inputParameterErrorHandler (setBoundaryConditionCheck, index, side, axis);

  Range Side, Axis;
  Side = side==forAll ? Range(Start,End) 		: Range(side,side);
  Axis = axis==forAll ? Range(0,numberOfDimensions-1) 	: Range(axis,axis);

  boundaryCondition(Side,Axis,index) = (int) boundaryConditionType;
  componentForBoundaryCondition(Side,Axis,index,0) = component;

  if (debug) boundaryConditionDisplay.display (componentForBoundaryCondition, "componentForBoundaryCondition set to :");
  if(debug) boundaryConditionDisplay.display (boundaryCondition, "boundaryCondition set to :");
}

//==============================================================================
//\begin{>>boundaryConditions.tex}{\subsection{setBoundaryCondition for many components}} 
void MappedGridFiniteVolumeOperators::
setBoundaryCondition ( 	const int & index, 
			const int & side, 
			const int & axis,
			const boundaryConditionTypes & boundaryConditionType,
			const intArray & component)				// = 0
//
// /Purpose:
//   assign the index'th boundary condition 
//   a particular (side,axis) of a
//   MappedGrid.  This function is used to specify boundary conditions involving
//   multiple components of the solution, such as the {\bf normalComponent} boundary
//   condition.
//   This function specifies how the values in the ghost cells adjacent
//   to a boundary will be set. It does not actually set those values; that is done
//   by the routine {\bf applyBoundaryConditions}. This function (or one of its alternate versions) must
//   be called prior to a call to {\bf applyBoundaryConditions}.
//
// /index:	index (ie which) BC to set
// /side:	which side this applies to (if = MappedGridFiniteVolumeOperators::forALL,
//				then this applies to all sides
// /axis:	which axis this applies to (if = MappedGridFiniteVolumeOperators::forALL,
//				then this applies to all axes
//
// /boundaryConditionType:
//	  select from among
//        \begin{itemize} 
//	    \item{normalComponent:}  set the normal component of a vector MappedGridFunction 
//                 (typically a velocity) to a value.
//         \end{itemize} 
//
// /component:	which components of your REALMappedGridFunction constitute the components of
//              the velocity-like function. For example, if a three-dimensional velocity
//              is stored in the 2,3,4-components of a REALMappedGridFunction, then set
//              component(0) = 2; component(1) = 3; component(2) = 4;
//
// /Author: D. L. Brown
// /Date documentation last modified: 951130
//\end{boundaryConditions.tex}  
	//========================================
{
  inputParameterErrorHandler (setBoundaryConditionCheck, index, side, axis);

  Range Side, Axis;
  Side = side==forAll ? Range(Start,End) 		: Range(side,side);
  Axis = axis==forAll ? Range(0,numberOfDimensions-1) 	: Range(axis,axis);

  boundaryCondition(Side,Axis,index) = (int) boundaryConditionType;
  for (int i=0; i<=component.getBound(0); i++)
    componentForBoundaryCondition(Side,Axis,index,i) = component(i);

    boundaryConditionDisplay.display (componentForBoundaryCondition, "componentForBoundaryCondition set to :");
    if (debug) boundaryConditionDisplay.display (boundaryCondition, "boundaryCondition set to :");
}

//==============================================================================
//\begin{>>boundaryConditions.tex}{\subsection{setBoundaryCondition for all sides and components}} 
void MappedGridFiniteVolumeOperators::
setBoundaryCondition (const boundaryConditionTypes & boundaryConditionType)
//
// /Purpose:
//   specify the boundary condition for all sides, axes and components to be the
//   same.
//
// /boundaryConditionType:
//	  select from among
//        \begin{itemize} 
//	    \item{dirichlet:}        specify a value or a function along a side; use
//                                   {\bf setBoundaryConditionValue} or {setBoundaryConditionRightHandSide}
//                                   to specify the boundary value(s).  
//	    \item{neumann:}          set normal derivative along a side (typically to zero); use 
//                                   {\bf setBoundaryConditionValue} or {setBoundaryConditionRightHandSide}
//                                   to specify the value of the normal derivative  
//	    \item{extrapolate:}      extrapolate the ghost cell values 
//                                   from the interior by setting the third undivided difference
//                                   of the solution equal zero.
//	    \item{normalComponent:}  set the normal component of a vector MappedGridFunction to a value.
//         \end{itemize} 
//
// /Author: D. L. Brown
// /Date documentation last modified:
//\end{boundaryConditions.tex}  
	//========================================
{
  for (int side=Start; side<=End; side++)
    for (int axis=0; axis<3; axis++)
      for (int index=0; index<numberOfBoundaryConditions(side,axis); index++)
      {
	boundaryCondition(side,axis,index) = (int) boundaryConditionType;
	for (int component=0; component<numberOfComponents; component++)
        // 950823 I don't understand why componentForBoundaryCondition has 4 indexes
	//
	//  componentForBoundaryCondition(side,axis,index,component) = component;
          componentForBoundaryCondition(side,axis,index,component) = index;
      }
  if (debug) boundaryConditionDisplay.display (componentForBoundaryCondition, "componentForBoundaryCondition is set to:");
}
//==============================================================================
//\begin{>>boundaryConditions.tex}{\subsection{setBoundaryConditionValue}} 
void MappedGridFiniteVolumeOperators::
setBoundaryConditionValue (const real & value,
			const int & component,
			const int & index,
			const int & side,		// = forAll
			const int & axis,		// = forAll
			const bool & trueOrFalse)	// = TRUE
//
// /Purpose:
//  This function specifies a constant forcing value for a boundary condition.
//  It is called prior to a the function {\bf applyBoundaryConditions}. 
//
// /value:		forcing value for BC
// /component:	this is forcing value for which component
// /index:		this is forcing value for index'th boundary condition
// /side:		this is forcing value for which side
// /axis:		this is forcing value for boundaries along which axis
// /trueOrFalse:	if == TRUE, use this value; if == FALSE, don't set a value;
//
//
// /Status and Warnings:
// \begin{itemize}
//   \item{0:} 
//   Requirement to specify both component and index is unnecessary, but included
//   for compatibility with the interface for the MappedGridFunctionOperators class. 
//    \item{1:} 
//   If you specify the wrong
//   component (different from what was specified in setBoundaryCondition), the
//   value specified will not be used currently.
//   \item{2:}
//     it is not currently possible to specify a value for a {\bf normalComponent} or 
//     {\bf extrapolation} boundary conditions. 
//  \end{itemize} 
//
// /Author: D. L. Brown
// /Date documentation last modified: 951111
//\end{boundaryConditions.tex}  
	//========================================
{
  inputParameterErrorHandler (setBoundaryConditionValueCheck, index, side, axis, component);

  Range Side, Axis;
  Side = side==forAll ? Range(Start,End) : Range(side,side);
  Axis = axis==forAll ? Range(0,numberOfDimensions-1) : Range(axis,axis);
  
  boundaryConditionValueGiven (Side, Axis, index, component) = trueOrFalse;
  if (trueOrFalse)
    boundaryConditionValue (Side, Axis, index, component) = value;
// *wdh  if (trueOrFalse)
// *wdh no display for floatSerialArray (RealArray)
// *wdh   if (debug) boundaryConditionDisplay.display (boundaryConditionValue, "boundaryConditionValue set to:");
}
//==============================================================================
//\begin{>>boundaryConditions.tex}{\subsection{setBoundaryConditionRightHandSide}}   
void MappedGridFiniteVolumeOperators::
setBoundaryConditionRightHandSide (const REALMappedGridFunction & boundaryConditionRightHandSide)
//
// /Purpose:
//   specify a gridFunction to be used as right-hand-side of boundary
//   conditions. The forcing for boundary conditions should be placed
//   in the row corresponding to the ghost values that are being changed.
//   For a typical problem in which one row of ghosts cells is being, set,
//   the values therefore go in the first row of ghost cells. The values
//   specified by this function are actually applied when the function
//   {\bf applyBoundaryConditions} is called. 
//
//  /boundaryConditionRightHandSide (mg, cellCentered, numberOfBoundaryConditions):
//				this array (currently a full MappedGridFunction)
//				contains the forcing for the BCs.
// 
// /Author:				D.L.Brown
// /Date Documentation Last Modified:	951108
//\end{boundaryConditions.tex} 
//========================================
{
  boundaryData = TRUE;
  boundaryRHS.reference (boundaryConditionRightHandSide);
}
		      
//==============================================================================
//\begin{>>boundaryConditions.tex}{\subsection{applyBoundaryConditions}} 
void MappedGridFiniteVolumeOperators::
applyBoundaryConditions (REALMappedGridFunction & u,
			const REAL & t)			// = 0.
//
// /Purpose:
//	apply boundary conditions to a REALMappedGridFunction. This function is
//      called after calls to functions that set the boundary condition types and
//      values. It actually applies the boundary conditions, and so is typically
//      called at each timestep.
//
// /t(input):			value of "t" for which boundary
//					conditions are to be applied.
//					For constant boundary conditions, this
//					value is irrelevant.
//
// /u (input/output):	function to which boundary conditions
//				are to be applied;
//	                        changes boundary values of u  in order to enforce selected boundary 
//	                        conditions
//
// /Author: D. L. Brown
// /Date documentation last changed: 951111
//\end{boundaryConditions.tex}  
	//========================================
{
  
  inputParameterErrorHandler (applyBoundaryConditionsCheck);

  int side,axis;
  int i,n,n1,n2,n3;		// Bill says its a bad idea to declare stuff within a switch
  Index I1,  I2,  I3;           // first cell inside boundary
  Index I1m, I2m, I3m;          // first cell outside boundary
  Index I1b, I2b, I3b;          // index for faceCentered objects on the boundary itself
  
  bool uDotNUpdated = FALSE;
  // REAL HALF = 0.5, TWO = 2.0, ONE = 1.0;
  REAL HALF = 0.5, TWO = 2.0;

  MappedGrid &c = mappedGrid;

  OGFunction *e = twilightZoneFlowFunction;
  if (twilightZoneFlow) assert (twilightZoneFlowFunction != NULL);

 // ========================================
 // Loop over Boundaries; assign values
 // ========================================

 ForBoundary (side, axis)
 {
    if (c.boundaryCondition()(side,axis) > 0)	// If this is a real boundary...
    {
      getBoundaryIndex (c.indexRange(), side, axis, I1, I2, I3);
      getGhostIndex    (c.indexRange(), side, axis, I1m, I2m, I3m, +1);  //first ghost line or surface
      getGhostIndex    (c.indexRange(), side, axis, I1b, I2b, I3b, side);

      for (i=0; i<numberOfBoundaryConditions(side,axis); i++ )
      {
	switch ( (boundaryConditionTypes) boundaryCondition (side,axis,i) )
	{

     // ====================
	case dirichlet:
     // ====================

	  n = componentForBoundaryCondition(side,axis,i,0);

	  if (twilightZoneFlow)
	  {
	    //u(I1m,I2m,I3m,n) = (*e)(c,I1m,I2m,I3m,n,t);
	    u(I1m,I2m,I3m,n) = -u(I1,I2,I3,n) + TWO*((*e)(c,I1m,I2m,I3m,n,t) + (*e)(c,I1,I2,I3,n,t));
	    if (debug) boundaryConditionDisplay.display (u, "array after Dirichlet TWF BC applied");
	  } 
	  else if (boundaryConditionValueGiven(side,axis,i,n) )		//user-supplied const value
	    u(I1m,I2m,I3m,n) = -u(I1,I2,I3,n) + TWO*boundaryConditionValue(side,axis,i,n);
	  else if (boundaryData)
	    u(I1m,I2m,I3m,n) = -u(I1,I2,I3,n) + TWO*boundaryRHS(I1m,I2m,I3m,i);			//user-supplied values
	  else
	    u(I1m,I2m,I3m,n) = -u(I1,I2,I3,n);						//default is dirichlet value of zero

	  break;

     // ====================
	case neumann:  //(note, not yet implemented for nonorthogonal grids)
     // ====================

	  n = componentForBoundaryCondition(side,axis,i,0);
	  
	  if ( boundaryConditionValueGiven (side,axis,i,n) )
	    u(I1m,I2m,I3m,n) = u(I1,I2,I3,n) - boundaryConditionValue(side,axis,i,n);	
	  else if ( boundaryData )
	    u(I1m,I2m,I3m,n) = u(I1,I2,I3,n) - boundaryRHS(I1m,I2m,I3m,i);
	  else 
	    u(I1m,I2m,I3m,n) = u(I1,I2,I3,n);		// *** fix this ***
	  if (twilightZoneFlow)
	    u(I1m,I2m,I3m,n) += (*e)(c,I1m,I2m,I3m,n,t) - (*e)(c,I1,I2,I3,n,t);

	  break;

     // ====================
	case extrapolate:
     // ====================

	  n = componentForBoundaryCondition(side,axis,i,0);

	  u(I1m,I2m,I3m,n) = u(I1,I2,I3,n);
	  if (twilightZoneFlow)
	    u(I1m,I2m,I3m,n) += (*e)(c,I1m,I2m,I3m,n,t) + (*e)(c,I1,I2,I3,n,t);
	  
	  break;
     // ====================
	case normalComponent:
     // for this boundary condition to work, you need to set the ghost value of u first, say,
     // by extrapolating. 
     // ====================

	  if (!uDotNUpdated)
	  {
	    uDotN.updateToMatchGrid (mappedGrid);
	    uDotNUpdated = TRUE;
	  }


	  n1 = componentForBoundaryCondition (side, axis, i, 0);
	  n2 = componentForBoundaryCondition (side, axis, i, 1);
	if ( numberOfDimensions == 3 )
	  n3 = componentForBoundaryCondition (side, axis, i, 2);

//...first compute the unscaled normal component at the cell faces

	  if ( numberOfDimensions == 2 )
	    uDotN(I1,I2,I3) = HALF*(
		(u(I1,I2,I3,n1) + u(I1m,I2m,I3m,n1))*faceNormal(I1b,I2b,I3b,xAxis,axis)
	      + (u(I1,I2,I3,n2) + u(I1m,I2m,I3m,n2))*faceNormal(I1b,I2b,I3b,yAxis,axis));
	  
	  else 
	    uDotN(I1,I2,I3) = HALF*(
		(u(I1,I2,I3,n1) + u(I1m,I2m,I3m,n1))*faceNormal(I1b,I2b,I3b,xAxis,axis)
	      + (u(I1,I2,I3,n2) + u(I1m,I2m,I3m,n2))*faceNormal(I1b,I2b,I3b,yAxis,axis)
	      + (u(I1,I2,I3,n3) + u(I1m,I2m,I3m,n3))*faceNormal(I1b,I2b,I3b,zAxis,axis));

	  if ( twilightZoneFlow )
	  {
	    if ( numberOfDimensions == 2 )
	      uDotN(I1,I2,I3) -= HALF*(
		((*e)(c,I1,I2,I3,n1,t) + (*e)(c,I1m,I2m,I3m,n1,t))*faceNormal(I1b,I2b,I3b,xAxis,axis)
	      + ((*e)(c,I1,I2,I3,n2,t) + (*e)(c,I1m,I2m,I3m,n2,t))*faceNormal(I1b,I2b,I3b,yAxis,axis));
	    else
	      uDotN(I1,I2,I3) -= HALF*(
		((*e)(c,I1,I2,I3,n1,t) + (*e)(c,I1m,I2m,I3m,n1,t))*faceNormal(I1b,I2b,I3b,xAxis,axis)
	      + ((*e)(c,I1,I2,I3,n2,t) + (*e)(c,I1m,I2m,I3m,n2,t))*faceNormal(I1b,I2b,I3b,yAxis,axis)
	      + ((*e)(c,I1,I2,I3,n3,t) + (*e)(c,I1m,I2m,I3m,n2,t))*faceNormal(I1b,I2b,I3b,zAxis,axis));
	  }

//...now subtract out the normal component  so that u will have normal component = 0. scaling done at this point
	   
	  if (numberOfDimensions == 2)
	  {
	    u(I1m,I2m,I3m,n1) +=  -TWO*uDotN(I1,I2,I3)*faceNormal(I1b,I2b,I3b,xAxis,axis)/
	                         (  faceNormal(I1b,I2b,I3b,xAxis,axis)*faceNormal(I1b,I2b,I3b,xAxis,axis)
				  + faceNormal(I1b,I2b,I3b,yAxis,axis)*faceNormal(I1b,I2b,I3b,yAxis,axis));
	    u(I1m,I2m,I3m,n2) += -TWO*uDotN(I1,I2,I3)*faceNormal(I1b,I2b,I3b,yAxis,axis)/
	                         (  faceNormal(I1b,I2b,I3b,xAxis,axis)*faceNormal(I1b,I2b,I3b,xAxis,axis)
				  + faceNormal(I1b,I2b,I3b,yAxis,axis)*faceNormal(I1b,I2b,I3b,yAxis,axis));
	  }

	  if (numberOfDimensions == 3)
	  {
	    u(I1m,I2m,I3m,n1) += -TWO*uDotN(I1,I2,I3)*faceNormal(I1b,I2b,I3b,xAxis,axis)/
	                         (  faceNormal(I1b,I2b,I3b,xAxis,axis)*faceNormal(I1b,I2b,I3b,xAxis,axis)
				  + faceNormal(I1b,I2b,I3b,yAxis,axis)*faceNormal(I1b,I2b,I3b,yAxis,axis)
				  + faceNormal(I1b,I2b,I3b,zAxis,axis)*faceNormal(I1b,I2b,I3b,zAxis,axis));
	    u(I1m,I2m,I3m,n2) += -TWO*uDotN(I1,I2,I3)*faceNormal(I1b,I2b,I3b,yAxis,axis)/
                                 (  faceNormal(I1b,I2b,I3b,xAxis,axis)*faceNormal(I1b,I2b,I3b,xAxis,axis)
				  + faceNormal(I1b,I2b,I3b,yAxis,axis)*faceNormal(I1b,I2b,I3b,yAxis,axis)
				  + faceNormal(I1b,I2b,I3b,zAxis,axis)*faceNormal(I1b,I2b,I3b,zAxis,axis));
	    u(I1m,I2m,I3m,n3) += -TWO*uDotN(I1,I2,I3)*faceNormal(I1b,I2b,I3b,zAxis,axis)/
                                 (  faceNormal(I1b,I2b,I3b,xAxis,axis)*faceNormal(I1b,I2b,I3b,xAxis,axis)
				  + faceNormal(I1b,I2b,I3b,yAxis,axis)*faceNormal(I1b,I2b,I3b,yAxis,axis)
				  + faceNormal(I1b,I2b,I3b,zAxis,axis)*faceNormal(I1b,I2b,I3b,zAxis,axis));
	  }
	  
	  break;
	
	default:

	  inputParameterErrorHandler (applyBoundaryConditionsCheck, side, axis, i);
	  break;
	}
      }	   
    }
  }

  fixBoundaryCorners (u);			// fix periodicity, extrapolate corners, or whatever

}

#define UX3(n1,n2,n3,i1,i2,i3,n)              \
     + 3.*u(i1+  (n1),i2+  (n2),i3+  (n3),n)  \
     - 3.*u(i1+2*(n1),i2+2*(n2),i3+2*(n3),n)  \
     +    u(i1+3*(n1),i2+3*(n2),i3+3*(n3),n)

//==============================================================================
//\begin{>>boundaryConditions.tex}{\subsection{applyRightHandSideBoundaryConditions}} 
void MappedGridFiniteVolumeOperators::
applyRightHandSideBoundaryConditions (realMappedGridFunction & rightHandSide,
				      const REAL & t)                            // = 0.

//
// /Purpose:
//  apply right-hand-side boundary conditions for implicit operators.
//  The right-hand side of the boundary conditions is applied  according to
//  what was specified using the function {\bf setBoundaryConditionRightHandSide}. 
//
//   /rightHandSide(input/output):   the RHS array that needs boundary conditions
//   /t(input):                      time (optional).
//   
//
// /Author: D. L. Brown
// /Date documentation last modified: 951111
//\end{boundaryConditions.tex}  
//==============================================================================

{
  MappedGrid &c = mappedGrid;

  OGFunction *e = twilightZoneFlowFunction;
  if (twilightZoneFlow) assert (twilightZoneFlowFunction != NULL);

 // ========================================
 // Loop over Boundaries; assign values
 // ========================================
  int i, n, side, axis; 
  Index I1, I2, I3;
  Index I1m, I2m, I3m;
  REAL HALF = 0.5;
  
  
  ForBoundary (side, axis)
  {
    if (c.boundaryCondition()(side,axis) > 0)	// If this is a real boundary...
    {
      getBoundaryIndex (c.indexRange(), side, axis, I1, I2, I3);
      getGhostIndex    (c.indexRange(), side, axis, I1m, I2m, I3m, +1);  //first ghost line or surface

      for (i=0; i<numberOfBoundaryConditions(side,axis); i++ )
      {
	switch ( (boundaryConditionTypes) boundaryCondition (side,axis,i) )
	{

     // ====================
	case dirichlet:
     // ====================
          n = componentForBoundaryCondition (side,axis,i,0);
     
          if (twilightZoneFlow)
            rightHandSide(I1m,I2m,I3m) = HALF*((*e)(c,I1m,I2m,I3m,n,t) + (*e)(c,I1,I2,I3,n,t));
	  else if (boundaryConditionValueGiven(side,axis,i,n))
            rightHandSide(I1m,I2m,I3m) = boundaryConditionValue(side,axis,i,n);
          else if (boundaryData)
            rightHandSide(I1m,I2m,I3m) = boundaryRHS(I1m,I2m,I3m,i);
          else
	    rightHandSide(I1m,I2m,I3m) = 0.;
   
          break;

     // ====================
	case neumann:  //(note, not yet implemented for nonorthogonal grids)
     // ====================
      
          n = componentForBoundaryCondition (side,axis,i,0);
          
          if ( twilightZoneFlow )
            rightHandSide(I1m,I2m,I3m) = -(*e)(c,I1m,I2m,I3m,n,t) + (*e)(c,I1,I2,I3,n,t);
          else if ( boundaryData )
            rightHandSide(I1m,I2m,I3m) = boundaryRHS(I1m,I2m,I3m,i);
          else if ( boundaryConditionValueGiven(side,axis,i,n))
            rightHandSide(I1m,I2m,I3m) = boundaryConditionValue(side,axis,i,n);
          else
            rightHandSide(I1m,I2m,I3m) = 0.;
     
          break;
     // ====================
	case extrapolate:
     // ====================

	  n = componentForBoundaryCondition(side,axis,i,n);
          
          rightHandSide(I1m,I2m,I3m) = 0.;
          if ( twilightZoneFlow )    
            rightHandSide(I1m,I2m,I3m) = -(*e)(c,I1m,I2m,I3m,n,t) + (*e)(c,I1,I2,I3,n,t);
          
          break;
     // ====================
	case normalComponent:
     // ====================
           cout << "applyRightHandSideBoundaryCondition: WARNING: normalComponent boundary condition" << endl;
           cout << "not yet implemented " << endl;

	  break;
     	
     // ====================
        default:
     // ====================
	  break;
     
        }
      }
    }
  }
 
}

/* THIS has been moved to the base class in Overture.v1 */
/*
//
//==============================================================================
void MappedGridFiniteVolumeOperators::
fixBoundaryCorners( realMappedGridFunction & u )
//
//  ****** This is just copied from Bill's MappedGridOperators class; it should go into the
//  ****** GenericMappedGrid class so that it wouldn't have to be included
{

  MappedGrid & c = mappedGrid;

  //     ---Fix periodic edges
  u.periodicUpdate();
  

  //     ---when two (or more) adjacent faces have boundary conditions
  //        we set the values on the fictitous line (or vertex)
  //        that is outside both faces ( points marked + below)
  //        We set values on all ghost points that lie outside the corner
  //
  //                + +                + +
  //                + +                + +
  //                    --------------
  //                    |            |
  //                    |            |
  //

  int side1,side2,side3,is1,is2,is3,i1,i2,i3,n;
  

  Index I1=Range(c.indexRange()(Start,axis1),c.indexRange()(End,axis1));
  Index I2=Range(c.indexRange()(Start,axis2),c.indexRange()(End,axis2));
  Index I3=Range(c.indexRange()(Start,axis3),c.indexRange()(End,axis3));
  Index N =Range(u.getComponentBase(0),u.getComponentBound(0));   // ********* Is this ok ?? *************

  //         ---extrapolate edges---
  if( !c.isPeriodic()(axis1) && !c.isPeriodic()(axis2) )
  {
    //       ...Do the four edges parallel to i3
    for( side1=Start; side1<=End; side1++ )
    {
      is1=1-2*side1;
      // * i1=c.indexRange()(side1,axis1);
      // loop over all ghost points along i1:
      for( i1=c.indexRange()(side1,axis1); i1!=c.dimension()(side1,axis1); i1-=is1 )
      for( side2=Start; side2<=End; side2++ )
      {
        is2=1-2*side2;
        // * i2=c.indexRange()(side2,axis2);
        // loop over all ghost points along i2:
        for( i2=c.indexRange()(side2,axis2); i2!=c.dimension()(side2,axis2); i2-=is2 )
        // ***        u(i1-is1,i2-is2,I3,N)=UX3(is1,is2,0,i1-is1,i2-is2,I3,N);
        for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
        for( n=N.getBase(); n<=N.getBound(); n++ )
          u(i1-is1,i2-is2,i3,n)=UX3(is1,is2,0,i1-is1,i2-is2,i3,n);
      }
    }
  }
 
  if( numberOfDimensions==2 ) return;

  if( !c.isPeriodic()(axis1) && !c.isPeriodic()(axis3) )
  {
    //       ...Do the four edges parallel to i2
    for( side1=Start; side1<=End; side1++ )
    {
      is1=1-2*side1;
      // * i1=c.indexRange()(side1,axis1);
      for( i1=c.indexRange()(side1,axis1); i1!=c.dimension()(side1,axis1); i1-=is1 )
      for( side3=Start; side3<=End; side3++ )
      {
        is3=1-2*side3;
        // * i3=c.indexRange()(side3,axis3);
        for( i3=c.indexRange()(side3,axis3); i3!=c.dimension()(side3,axis3); i3-=is3 )
          u(i1-is1,I2,i3-is3,N)=UX3(is1,0,is3,i1-is1,I2,i3-is3,N);
      }
    }
  }
  if( !c.isPeriodic()(axis1) && !c.isPeriodic()(axis3) )
  {
    //       ...Do the four edges parallel to i1
    for( side2=Start; side2<=End; side2++ )
    {
      is2=1-2*side2;
      // * i2=c.indexRange()(side2,axis2);
      for( i2=c.indexRange()(side2,axis2); i2!=c.dimension()(side2,axis2); i2-=is2 )
      for( side3=Start; side3<=End; side3++ )
      {
        is3=1-2*side3;
        // * i3=c.indexRange()(side3,axis3);
        for( i3=c.indexRange()(side3,axis3); i3!=c.dimension()(side3,axis3); i3-=is3 )
          u(I1,i2-is2,i3-is3,N)=UX3(0,is2,is3,I1,i2-is2,i3-is3,N);
      }
    }
  }

  if( !c.isPeriodic()(axis1) && !c.isPeriodic()(axis2) )
  {
    //       ...Do the four edges parallel to i3
    for( side1=Start; side1<=End; side1++ )
    {
      is1=1-2*side1;
      // * i1=c.indexRange()(side1,axis1);
      for( i1=c.indexRange()(side1,axis1); i1!=c.dimension()(side1,axis1); i1-=is1 )
      for( side2=Start; side2<=End; side2++ )
      {
        is2=1-2*side2;
        // * i2=c.indexRange()(side2,axis2);
        for( i2=c.indexRange()(side2,axis2); i2!=c.dimension()(side2,axis2); i2-=is2 )
        for( side3=Start; side3<=End; side3++ )
        {
          is3=1-2*side3;
          // * i3=c.indexRange()(side3,axis3);
          for( i3=c.indexRange()(side3,axis3); i3!=c.dimension()(side3,axis3); i3-=is3 )
            u(i1-is1,i2-is2,i3-is3,N)=UX3(is1,is2,is3,i1-is1,i2-is2,i3-is3,N);
	}
      }
    }
  }

}
*/




//==============================================================================
void MappedGridFiniteVolumeOperators::
ApplyBoundaryConditions ( REALMappedGridFunction & coeff,
		          const real & t)				// = 0.

//==============================================================================
{
  cout << "The routine ApplyBoundaryConditions is no longer supported" << endl;
  cout << "Use routine applyBoundaryConditionsToCoefficients instead " << endl;
  assert (FALSE);
}

//==============================================================================
//\begin{>>boundaryConditions.tex}{\subsection{applyBoundaryConditionsToCoefficients}} 
void MappedGridFiniteVolumeOperators::
applyBoundaryConditionsToCoefficients( REALMappedGridFunction & coeff,
				       const real & t)                  // = 0.
//
// /Purpose:
//	Apply Boundary Conditions to an inverse operator 
//	(i.e. set edge values of the coefficient arrays)
//
// /coeff (input/output):	The coefficient array
// /t (input):			time
//
//
// /Status and Warnings:
// \begin{itemize} 
//	\item{950713:} neumann implemented only for orthogonal grids;
//		normalComponent not implemented at all
//	\item{950717:} corners not set; I have assumed that the Oges solver
//		will do the extrapolation for you.
// \end{itemize} 
//  
// /Author: D. L. Brown
// /Date documentation last modified: 951111
//\end{boundaryConditions.tex}  
	//========================================
{
#undef inside
#define inside(axis,side,i) inc(axis,i)*(1-2*side)

  inputParameterErrorHandler (ApplyBoundaryConditionsCheck);
	      
  int side,axis;
  int i;
  int arg1,arg2,coefficient;
  Index I1,  I2,  I3;
  Index I1m, I2m, I3m;
  REAL HALF = 0.5, ZERO = 0.0, ONE = 1.0;
  int ghostCoefficientLocation;
  int boundaryCoefficientLocation;
  
  MappedGrid &c = mappedGrid;

  Index M(0,9);

 // ========================================
 // Loop over Boundaries; assign values
 // ========================================

 ForBoundary (side, axis)
 {
    if (c.boundaryCondition()(side,axis) > 0)	// If this is a real boundary...
    {
      getBoundaryIndex (c.indexRange(), side, axis, I1, I2, I3);
      getGhostIndex    (c.indexRange(), side, axis, I1m, I2m, I3m, +1);  //first ghost line


      for (i=0; i<numberOfBoundaryConditions(side,axis); i++ )
      {
	switch ( (boundaryConditionTypes) boundaryCondition (side,axis,i) )
	{

     // ====================
	case dirichlet:
     // ====================

	  coeff(M,        I1m,I2m,I3m) = ZERO;
	  if (twilightZoneFlow && FALSE) {

	    coeff(n3n3(1,1),I1m,I2m,I3m) = ONE;		//for TwilightZoneFlow, set the ghost point rather than average		

	  } else {

	    coeff(n3n3(1,1),I1m,I2m,I3m) = HALF;
//	    coeff(n3n3(1+inside(axis,side,rAxis),1+inside(axis,side,sAxis)),I1m,I2m,I3m) = HALF;
            arg1 = 1+inside(axis,side,rAxis);
	    arg2 = 1+inside(axis,side,sAxis);
	    coefficient    = n3n3(arg1,arg2);
	    coeff(coefficient,I1m,I2m,I3m) = HALF;
	  }

	  break;

     // ====================
	case neumann:		//FAKE NEUMANN
     //   ghost cell always -1, boundary cell always 1: this simplifies logic
     // ====================

          arg1 = 1+inside(axis,side,rAxis);
	  arg2 = 1+inside(axis,side,sAxis);
          ghostCoefficientLocation    = n3n3(1,1);
	  boundaryCoefficientLocation = n3n3(arg1,arg2);
     
	  coeff(M,        I1m,I2m,I3m) = ZERO;
	  coeff(ghostCoefficientLocation    ,I1m,I2m,I3m) = -ONE; // (2*side-1)
          coeff (boundaryCoefficientLocation,I1m,I2m,I3m) =  ONE; // -(2*side-1)

	  break;
      
     // ====================
	case extrapolate:		
     // ====================

	  coeff(M,        I1m,I2m,I3m) = ZERO;
	  coeff(n3n3(1,1),I1m,I2m,I3m) = (2*side-1);
          arg1 = 1+inside(axis,side,rAxis);
	  arg2 = 1+inside(axis,side,sAxis);
	  coefficient    = n3n3(arg1,arg2);
      //  coeff(n3n3(1+inside(axis,side,rAxis),1+inside(axis,side,sAxis)),I1m,I2m,I3m) = -(2*side-1);
	  coeff(coefficient,I1m,I2m,I3m) =  -(2*side-1);

	  break;

     // ====================
	case normalComponent:		
     // ====================

	  break;

     // ====================
	default:
     // ====================

	  inputParameterErrorHandler (ApplyBoundaryConditionsCheck, side, axis, i);
	  break;

	  }
	}
      }
  }
	  
}

