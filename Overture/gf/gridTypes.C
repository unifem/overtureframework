


enum gridFunctionType
{
  general,
  vertexCenter,
  cellCenter,
  faceCenterAll,
  faceCenterAxis1,
  faceCenterAxis2,
  faceCenterAxis3
};


enum gridFunctionTypeWithComponents
{
  generalWith1Component,
  generalWith2Components,
  generalWith3Components,
  generalWith4Components,
  generalWith5Components,

  vertexCenterWith1Component,
  vertexCenterWith2Components,
  vertexCenterWith3Components,
  vertexCenterWith4Components,
  vertexCenterWith5Components,

  cellCenterWith1Component,
  cellCenterWith2Components,
  cellCenterWith3Components,
  cellCenterWith4Components,
  cellCenterWith5Components,

  faceCenterAllWith1Component,
  faceCenterAllWith2Components,
  faceCenterAllWith3Components,
  faceCenterAllWith4Components,
  faceCenterAllWith5Components,

  faceCenterAxis1With1Component,
  faceCenterAxis1With2Components,
  faceCenterAxis1With3Components,
  faceCenterAxis1With4Components,
  faceCenterAxis1With5Components,

  faceCenterAxis2With1Component,
  faceCenterAxis2With2Components,
  faceCenterAxis2With3Components,
  faceCenterAxis2With4Components,
  faceCenterAxis2With5Components,

  faceCenterAxis3With1Component,
  faceCenterAxis3With2Components,
  faceCenterAxis3With3Components,
  faceCenterAxis3With4Components,
  faceCenterAxis3With5Components
 };


  
// Constructor

mappedGridFunction(MappedGrid & mg, 
                   const GridFunctionType type, 
                   const Range Component0=nullRange,       // defaults to Range(0,0)
                   const Range Component1=nullRange,
                   const Range Component2=nullRange,
                   const Range Component3=nullRange,
                   const Range Component4=nullRange );


// return the type of the grid function
gridFunctionType getGridFunctionType() const;

gridFunctionTypeWithComponents getGridFunctionTypeWithComponents() const;

int getNumberOfComponents() const;
