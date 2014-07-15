#include "BoundaryData.h"

// Here is a class used to hold the boundary data for boundary conditions


// ========================================================================================
// \brief Default constructor.
// ========================================================================================
BoundaryData::
BoundaryData()
{
  // printf("BoundaryData constructor called\n");
  for( int side=0; side<=1; side++ )
  {
    for( int axis=0; axis<3; axis++ )
    {
      boundaryData[side][axis]=NULL;
      pHasVariableCoefficientBoundaryCondition[(side)+2*(axis)]=0;
    }
  }
}

// ========================================================================================
// \brief Copy constructor.
// ========================================================================================
BoundaryData::
BoundaryData(const BoundaryData & x)
{
  // printf("BoundaryData copy constructor called\n");
  for( int side=0; side<=1; side++ ) 
  {
    for( int axis=0; axis<3; axis++ )
    {
      boundaryData[side][axis]=NULL;
      pHasVariableCoefficientBoundaryCondition[(side)+2*(axis)]=0;
    }
  }
  
  *this=x;
}

// ========================================================================================
// \brief Equals operator (shallow copy of any arrays).
// ========================================================================================
BoundaryData & BoundaryData::
operator=(const BoundaryData & x)
{
  for( int side=0; side<=1; side++ ) 
  {
    for( int axis=0; axis<3; axis++ )
    {
      if( boundaryData[side][axis]!=NULL )
      {
        boundaryData[side][axis]->decrementReferenceCount();
        if( boundaryData[side][axis]->getReferenceCount()==0 )
	{
          // printf("+++++++++ BoundaryData::= Delete the array for (side,axis)=(%i,%i)\n",side,axis);
          delete boundaryData[side][axis];
	}
      }
      
      boundaryData[side][axis]=x.boundaryData[side][axis];
      if( boundaryData[side][axis]!=NULL )
        boundaryData[side][axis]->incrementReferenceCount();
      
      pHasVariableCoefficientBoundaryCondition[(side)+2*(axis)]=
	x.hasVariableCoefficientBoundaryCondition(side,axis);
      
      // What should we do about arrays stored in the dbase ? -> make a reference: 
      if( hasVariableCoefficientBoundaryCondition(side,axis) & variableCoefficientTemperatureBC )
      {
	printF("BoundaryData::=operator: hasVariableCoefficientBoundaryCondition(side,axis)=%i\n",
               hasVariableCoefficientBoundaryCondition(side,axis));
	
        // delete the existing array
	RealArray **varCoeff = dbase.get<RealArray*[6]>("variableTemperatureCoefficients");
        varCoeff[(side)+2*(axis)]->decrementReferenceCount();
        if( varCoeff[(side)+2*(axis)]->getReferenceCount()==0 )
	{
	  delete varCoeff[(side)+2*(axis)];
	}
      }
      
      if( x.hasVariableCoefficientBoundaryCondition(side,axis) & variableCoefficientTemperatureBC )
      {
	printF("BoundaryData::=operator: x.hasVariableCoefficientBoundaryCondition(side,axis)=%i\n",
               x.hasVariableCoefficientBoundaryCondition(side,axis));

        // reference the array in "x"
	RealArray* const *xVarCoeff = x.dbase.get<RealArray*[6]>("variableTemperatureCoefficients");

	if( !dbase.has_key("variableTemperatureCoefficients") )
	{
          dbase.put<RealArray*[6]>("variableTemperatureCoefficients"); 
	}
	RealArray **varCoeff = dbase.get<RealArray*[6]>("variableTemperatureCoefficients");

	varCoeff[(side)+2*(axis)]=(RealArray*)(xVarCoeff[(side)+2*(axis)]);
	if( varCoeff[(side)+2*(axis)]!=NULL )
	  varCoeff[(side)+2*(axis)]->incrementReferenceCount();

	pHasVariableCoefficientBoundaryCondition[(side)+2*(axis)] |= variableCoefficientTemperatureBC;

      }
      else
      {
        pHasVariableCoefficientBoundaryCondition[(side)+2*(axis)] &= ~variableCoefficientTemperatureBC;

      }
      

    } // end for axis 
  } // end for side
  
  return *this;
}


// ========================================================================================
// \brief Destructor.
// ========================================================================================
BoundaryData::
~BoundaryData()
{
  for( int side=0; side<=1; side++ ) 
  {
    for( int axis=0; axis<3; axis++ )
    {
      if( boundaryData[side][axis]!=NULL )
      {
        boundaryData[side][axis]->decrementReferenceCount();
        if( boundaryData[side][axis]->getReferenceCount()==0 )
	{
          // printf("********* ~BoundaryData() Delete the array for (side,axis)=(%i,%i)\n",side,axis);
          delete boundaryData[side][axis];
	}
      }

      // delete any variable coefficient BC arrays
      if( hasVariableCoefficientBoundaryCondition(side,axis) & variableCoefficientTemperatureBC )
      {
        RealArray **varCoeff = dbase.get<RealArray*[6]>("variableTemperatureCoefficients");
        varCoeff[(side)+2*(axis)]->decrementReferenceCount();
        if( varCoeff[(side)+2*(axis)]->getReferenceCount()==0 )
	{
	  delete varCoeff[(side)+2*(axis)];
	}
	
      }
      

    }
  }
  
}

// ================================================================================================
//
/// \brief Return the array that holds the variable coefficients in a given type of boundary
///   condition. This routine will create (but not dimension) the array if has not already been allocated.
/// \param option (input) : specifies which coefficient array to return
/// \param side, axis (input) : return the array for this face.
/// 
/// \Note: This routine will also set hasVariableCoefficientBoundaryCondition(side,axis) to include option.
//
// ================================================================================================
RealArray& BoundaryData::
getVariableCoefficientBoundaryConditionArray( VariableCoefficientBoundaryConditionEnum option, int side, int axis )
{
  assert( side>=0 && side<=1 && axis>=0 && axis<=2 );

  if( option == variableCoefficientTemperatureBC )
  {
    if( !dbase.has_key("variableTemperatureCoefficients") )
    {
      // -- allocate the array of pointers to the arrays that hold the coefficients ---
      dbase.put<RealArray*[6]>("variableTemperatureCoefficients");
      RealArray **varCoeff = dbase.get<RealArray*[6]>("variableTemperatureCoefficients");
      for( int i=0; i<6; i++ )
	varCoeff[i]=NULL;
    }

    RealArray **varCoeff = dbase.get<RealArray*[6]>("variableTemperatureCoefficients");
    if( varCoeff[(side)+2*(axis)]==NULL )
    {
      varCoeff[(side)+2*(axis)]= new RealArray;
      varCoeff[(side)+2*(axis)]-> incrementReferenceCount();
    }

    pHasVariableCoefficientBoundaryCondition[(side)+2*(axis)] |= variableCoefficientTemperatureBC;

    return *varCoeff[(side)+2*(axis)];

  }


  OV_ABORT("BoundaryData::getVariableCoefficientBoundaryConditionArray:ERROR: unexpected option");
  return Overture::nullRealArray();
}
