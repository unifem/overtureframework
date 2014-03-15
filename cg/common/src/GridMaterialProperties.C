#include "GridMaterialProperties.h"


// =============================================================================
/// \brief Construct the object that manages and stores the material properties
/// on a component grid. 
// ============================================================================
GridMaterialProperties::
GridMaterialProperties()
{

  if( !dbase.has_key("numberOfMaterialProperties") ) dbase.put<int>("numberOfMaterialProperties",0);
  if( !dbase.has_key("materialFormat") ) dbase.put<MaterialFormatEnum>("materialFormat",constantMaterialProperties);

}



GridMaterialProperties::
~GridMaterialProperties()
{
}

// =============================================================================
/// \brief  Return the material format for this grid.
// ============================================================================
GridMaterialProperties::MaterialFormatEnum GridMaterialProperties::
getMaterialFormat() const
{
  return dbase.get<MaterialFormatEnum>("materialFormat");
}


// =============================================================================
/// \brief  Set the material format for this grid.
// ============================================================================
void GridMaterialProperties::
setMaterialFormat(const MaterialFormatEnum materialFormat )
{
  dbase.get<MaterialFormatEnum>("materialFormat")=materialFormat;
}


// Set the number of properties that we will store in the material property arrays.
void GridMaterialProperties::
setNumberOfMaterialProperties( const int numberOfMaterialProperties )
{
  dbase.get<int>("numberOfMaterialProperties")=numberOfMaterialProperties;
}


// Return the number of material properties that we will store in the material property arrays.
int GridMaterialProperties::
getNumberOfMaterialProperties() const
{
  return dbase.get<int>("numberOfMaterialProperties");
}


// Set the name of material property "m". 
void GridMaterialProperties::
setMaterialName( const int m, const aString & materialName )
{
  // finish me 
}


// Get the name of material property "m".
const aString & GridMaterialProperties::
getMaterialName( const int m ) const
{
  aString name;
  name="finish me";
  return name;
  
}




// ==========================================================================================
/// \brief  Return the index array for piecewise constant materials. 
/// \details Suppose that the material property "m" (e.g. lambda, mu,...) takes on 3 values 
///   over the grid, materialValues(m,i), i=0,1,2.
///   The properties of material "m" at a grid point (i1,i2,i3) are accessed using
///   materialValue(m, materialIndex(i1,i2,i3)). The materialIndex array thus holds the
//   index into the different material values. 
///
// ===========================================================================================
IntegerArray & GridMaterialProperties::
getMaterialIndexArray()
{
  if( !dbase.has_key("materialIndex") ) dbase.put<IntegerArray>("materialIndex");
  return dbase.get<IntegerArray>("materialIndex");
}


RealArray & GridMaterialProperties::
getMaterialValuesArray()
{
  if( !dbase.has_key("materialValues") ) dbase.put<RealArray>("materialValues");
  return dbase.get<RealArray>("materialValues");
}


// ==========================================================================================
/// \brief  Return the array that contains the variable material properties. 
///
/// \details The material properties at a grid point (i1,i2,i3) are accessed using
///        materialValue(i1,i2,i3,m),  m=0,1,...,numberOfMaterialProperties-1
// ===========================================================================================
RealArray & GridMaterialProperties::
getVariableMaterialValuesArray()
{
  if( !dbase.has_key("materialValues") ) dbase.put<RealArray>("materialValues");
  return dbase.get<RealArray>("materialValues");
}


