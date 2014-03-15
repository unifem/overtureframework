#ifndef GEOM_ADT_TUPLE_H
#define GEOM_ADT_TUPLE_H

// This class expects a typedef for "real" to be float or double

#ifndef processedWithDT
#undef GeomADTTuple
#define GeomADTTuple GeomADTTuple2
#undef dimension2
#define dimension2 (dimension*2)
#endif


template<class dataT, int dimension>
class GeomADTTuple // helper class to be used in the GeometricADT
{
 public:
  GeomADTTuple() { ; }
  GeomADTTuple(GeomADTTuple &x) 
    {
      for( int axis=0; axis<dimension2; axis++ )
        boundingBox[axis] = x.boundingBox[axis];
      data = x.data;
    }
  GeomADTTuple(const real *boundingBox_, const real *coords_, dataT i) 
    {
      int axis;
      for( axis=0; axis<dimension2; axis++ )
        boundingBox[axis] = boundingBox_[axis];
      for( axis=0; axis<dimension; axis++ )
	coords[axis] = coords_[axis];
      data = i;
    }
  GeomADTTuple & operator=(GeomADTTuple &x) 
    {
      int axis;
      for( axis=0; axis<dimension2; axis++ )
        boundingBox[axis] = x.boundingBox[axis];
      for( axis=0; axis<dimension; axis++ )
	coords[axis] = x.coords[axis];
      data = x.data;
      return *this;
    }
  ~GeomADTTuple() { ; }

  void setData(const real *boundingBox_, const real *coords_, dataT i) 
    {
      int axis;
      for( axis=0; axis<dimension2; axis++ )
        boundingBox[axis] = boundingBox_[axis];
      for( axis=0; axis<dimension; axis++ )
	coords[axis] = coords_[axis];
      data = i;
    }
  
  real boundingBox[dimension2];
  real coords[dimension];
  dataT data;
};

#endif
