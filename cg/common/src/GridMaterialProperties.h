#ifndef GRID_MATERIAL_PROPERTIES
#define GRID_MATERIAL_PROPERTIES

// =======================================================================================
// This class holds variable material properties for a component grid.
//
//  The material properties such as lambda, mu, Cp, k, mu etc. can be
//  stored in different formats:
//    (1) constantMaterialProperties: the material properties are constant on the grid
//    (2) piecewiseConstantMaterialProperties : the material properties are constant
//        over regions, which means they can be stored in a more compact fashion.
//    (3) variableMaterialProperties : the material properties vary from grid
//        point to grid point.
//
// Notes: 
//   We could have multiple instances of this class if we wish to store some material
//  parameters as piecewise-constant and some as variable.
// ======================================================================================

#include "Overture.h"
#define KK_DEBUG
#include "DBase.hh"
using namespace DBase;


class GridMaterialProperties
{
  
public:

enum MaterialFormatEnum
{
  constantMaterialProperties=0,
  piecewiseConstantMaterialProperties,
  variableMaterialProperties
};


GridMaterialProperties();
~GridMaterialProperties();

// Return the material format for this grid. 
MaterialFormatEnum getMaterialFormat() const;

// Set the material format
void
setMaterialFormat(const MaterialFormatEnum materialFormat );

// Set the number of properties that we will store in the material property arrays.
void
setNumberOfMaterialProperties( const int numberOfMaterialProperties );

// Return the number of material properties that we will store in the material property arrays.
int 
getNumberOfMaterialProperties() const;

// Set the name of material property "m". 
void
setMaterialName( const int m, const aString & materialName );

// Get the name of material property "m".
const aString &
getMaterialName( const int m ) const;

// Return the index array, materialIndex(i1,i2,i3) that holds the material index at a grid point (i1,i2,i3)
IntegerArray & 
getMaterialIndexArray();

// Return the array that holds the different material values by index (for piecewise constant materials)
RealArray & 
getMaterialValuesArray();

RealArray & 
getVariableMaterialValuesArray();


protected:

// This database contains the parameters and data for material properties.
DataBase dbase;


};



#endif
  
