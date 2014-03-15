//                                              -*- c++ -*-
// DeformingGridGenerationInformation:
//     contains params for HyperbolicGridGenerator

#ifndef DEFORMING_GRID_GENERATION_INFORMATION
#define DEFORMING_GRID_GENERATION_INFORMATION

class DeformingGridGenerationInformation {
public:
  int  rangeDimension;
  real distanceToMarch;
  real dissipation;
  int  linesInTheNormalDirection;
  int  *gridDimensions;
  int  *gridGeneratorDimensions;

  //..Constructor/Destructor
  DeformingGridGenerationInformation( const int rangeDim00=2 )
  {
    rangeDimension = rangeDim00; 
    assert( ( 1<=rangeDimension ) && ( rangeDimension<= 3 ));
    gridDimensions =           new int[rangeDimension];
    gridGeneratorDimensions =  new int[rangeDimension];
    // default dimensions 11 x 11 x 11
    for (int i=0; i<rangeDimension; i++) 
    {
      gridDimensions[i]          = 11;
      gridGeneratorDimensions[i] = gridDimensions[i];
    }

    distanceToMarch = 0.2;
    dissipation = 0.1;
    linesInTheNormalDirection = 11;
  };

  ~DeformingGridGenerationInformation() { delete gridDimensions; };

  int setGridDimensions( int n1, int n2 ) 
  {
    if (rangeDimension != 2) {
      cout << "ERROR DeformingGridGenerationInformation::setGridDimensions(n1,n2)\n";
      cout << "----- rangeDimension is not 2\n";
      return -1;
    }
    assert(gridDimensions != NULL );
    gridDimensions[0]=n1;
    gridDimensions[1]=n2;
    return 0;
  }

  int setGridDimensions( int n1, int n2, int n3) 
  {
    cout << "ERROR DeformingGridGenerationInformation::setGridDimensions(n1,n2,n3)\n";
    cout << "----- only 2 dims supported for now.\n";
    return -1;
  }

  int setGridGeneratorDimensions( int n1, int n2 ) 
  {
    if (rangeDimension != 2) {
      cout << "ERROR DeformingGridGenerationInformation::setGridGeneratorDimensions(n1,n2)\n";
      cout << "----- rangeDimension is not 2\n";
      return -1;
    }
    assert(gridGeneratorDimensions != NULL );
    gridGeneratorDimensions[0]=n1;
    gridGeneratorDimensions[1]=n2;
    return 0;
  }

  int setGridGeneratorDimensions( int n1, int n2, int n3) 
  {
    cout << "ERROR DeformingGridGenerationInformation::setGridGeneratorDimensions(n1,n2,n3)\n";
    cout << "----- only 2 dims supported for now.\n";
    return -1;
  }


};

#endif
