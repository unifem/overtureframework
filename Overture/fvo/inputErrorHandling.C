#include  "MappedGridFiniteVolumeOperators.h"
 
void MappedGridFiniteVolumeOperators::
	inputParameterErrorHandler (
		const functionNames  functionName,
		const int p0,				// = 0
		const int p1,				// = 0
		const int p2,				// = 0
		const int p3,				// = 0
		const int p4,				// = 0
		const int p5,				// = 0
		const int p6,				// = 0
		const int p7,				// = 0
		const int p8,				// = 0
		const int p9				// = 0
	      )

{
  int index,side,axis,component,i;
  int side0, side1, axis0, axis1, s, a;
  bool result;
  Range Side, Axis;

  switch (functionName) {

 // =================================
    case setBoundaryConditionCheck: 
 // =================================
      index = p0;
      side  = p1;
      axis  = p2;
      result = TRUE;
      result = result && (side == forAll || (side >=0 && side <=1));
      result = result && (axis == forAll || (axis >=0 && axis <=2));
      if (!result)
      {
	cout << "MappedGridFiniteVolumeOperators::setBoundaryCondition:Error invalid values for (side,axis) = ("
	     << side << "," << axis << ")\n";
	throw "MappedGridFiniteVolumeOperators::setBoundaryCondition:Error";
      }

      side0 = side==forAll ? Start : side;
      side1 = side==forAll ? End   : side;
      axis0 = axis==forAll ? 0     : axis;
      axis1 = axis==forAll ? 2     : axis;

      for (s=side0; s<=side1; s++){
	for (a=axis0; a<=axis1; a++){
	  if (index<0 || index>numberOfBoundaryConditions(s,a))
	  {  
	    cout << "MappedGridFiniteVolumeOperators::setBoundaryCondition:Error invalid value for index=" << index << endl;
	    cout << " This value should be >=0 and <=numberOfBoundaryConditions(side=" << s      
	      << ",axis=" << a << ")=" << numberOfBoundaryConditions(side,axis) << endl;
		throw "MappedGridFiniteVolumeOperators::setBoundaryCondition:Error";
	  }
        }
      }
      break;

 // =================================
    case setBoundaryConditionValueCheck: 
 // =================================
      index 	= p0;
      side  	= p1;
      axis  	= p2;
      component 	= p3;
      result = TRUE;
      result = result && (side == forAll || (side >=0 && side <=1));
      result = result && (axis == forAll || (axis >=0 && axis <=2));
      if (!result)
      {
	cout << "MappedGridFiniteVolumeOperators::setBoundaryConditionValue:Error invalid values for (side,axis) = ("
	     << side << "," << axis << ")\n";
	throw "MappedGridFiniteVolumeOperators::setBoundaryConditionValue:Error";
      }

      result = FALSE;
      result = result || index<0;
      Side = side==forAll ? Range(0,1) : Range(side,side);
      Axis = axis==forAll ? Range(0,2) : Range(axis,axis);
      for (s=Side.getBase(); s<=Side.getBound(); s++){
	for (a=Axis.getBase(); a<=Axis.getBound(); a++)
	  result = result || index>numberOfBoundaryConditions(s,a);
      }
      if (result)
      {  
	cout << "MappedGridFiniteVolumeOperators::setBoundaryConditionValue:Error invalid value for index=" << index << endl;
	cout << " This value should be >=0 and <=numberOfBoundaryConditions(side=" << side      
	  << ",axis=" << axis << ")=" << numberOfBoundaryConditions(side,axis) << endl;
	    throw "MappedGridFiniteVolumeOperators::setBoundaryConditionValue:Error";
      }
      if (component > numberOfComponents)
      {
	cout << "MappedGridFiniteVolumeOperators::setBoundaryConditionValue:Error, component > numberOfComponents" << endl;
	throw "MappedGridFiniteVolumeOperators::setBoundaryConditionValue:Error";
      }
      break;

 // =================================
    case applyBoundaryConditionsCheck:
 // =================================

      if (p0 == defaultValue) {
	if (numberOfDimensions == 0)
	{
	  cout <<"MappedGridFiniteVolumeOperators::applyBoundaryConditions:Error: you must assign a MappedGrid before "
	       <<"applyBoundaryConditions! \n" << endl;
	       throw "MappedGridFiniteVolumeOperators::applyBoundaryConditionValue:Error";
	}
      } else {
	side = p0;
	axis = p1;
	i    = p2;
	if (boundaryCondition (side,axis,i) == -1)
	{
	  cout << "WARNING: MappedGridFiniteVolumeOperators::applyBoundaryConditions: the boundary condition for"
		" side=" << side << ", axis=" << axis << ", index= " << i << endl;
	  cout << "has not been assigned a boundaryConditionType! \n";
	} else {
	  cout << "WARNING: applyBoundaryConditions: unknown boundary condition = "
	       << boundaryCondition(side,axis,i) << endl;
	}
      }
      break;

 // =================================
    case ApplyBoundaryConditionsCheck:
 // =================================

      if (p0 == defaultValue) {
	if (numberOfDimensions == 0)
	{
	  cout <<"MappedGridFiniteVolumeOperators::ApplyBoundaryConditions:Error: you must assign a MappedGrid before "
	       <<"ApplyBoundaryConditions! \n" << endl;
	       throw "MappedGridFiniteVolumeOperators::ApplyBoundaryConditionValue:Error";
	  }

	} else {

	side = p0;
	axis = p1;
	i    = p2;
	if (boundaryCondition (side,axis,i) == -1)
	{
	  cout << "MappedGridFiniteVolumeOperators::ApplyBoundaryConditions: the boundary condition for"
		" side=" << side << ", axis=" << axis << ", index= " << i << endl;
	  cout << "has not been assigned a boundaryConditionType! \n";
	} else {
	  cout << "ApplyBoundaryConditions: unknown boundary condition = "
	       << boundaryCondition(side,axis,i) << endl;
	}
	throw "MappedGridFiniteVolumeOperators::ApplyBoundaryConditions: fatal error! \n";
	
      } 

      break;

 // =================================
    default:
 // =================================

      break;
  };
}
