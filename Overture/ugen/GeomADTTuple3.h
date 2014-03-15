#ifndef GEOM_ADT_TUPLE3_H
#define GEOM_ADT_TUPLE3_H

// This class expects a typedef for "real" to be float or double

#ifndef processedWithDT
#undef GeomADTTuple
#define GeomADTTuple GeomADTTuple3
#undef dimension2
#define dimension2 (dimension*2)
#endif


template<class dataT>
class GeomADTTuple // helper class to be used in the GeometricADT
{
 public:
  GeomADTTuple() 
  { 
    dimension=0;
    // boundingBox=NULL; coords=NULL;  
  }
  GeomADTTuple(GeomADTTuple &x) 
    {
      for( int axis=0; axis<dimension2; axis++ )
        boundingBox[axis] = x.boundingBox[axis];
      data = x.data;
    }
  GeomADTTuple(int dimension_, const real *boundingBox_, const real *coords_, dataT i) 
    {
      dimension=dimension_;
      int axis;
      for( axis=0; axis<dimension2; axis++ )
        boundingBox[axis] = boundingBox_[axis];
      for( axis=0; axis<dimension; axis++ )
	coords[axis] = coords_[axis];
      data = i;
    }
  GeomADTTuple & operator=(GeomADTTuple &x) 
    {
      if( dimension==0 )
      {
        dimension=x.dimension;
	// boundingBox=new real[dimension2]; coords=new real[dimension];
      }
      int axis;
      for( axis=0; axis<dimension2; axis++ )
        boundingBox[axis] = x.boundingBox[axis];
      for( axis=0; axis<dimension; axis++ )
	coords[axis] = x.coords[axis];
      data = x.data;
      return *this;
    }
  ~GeomADTTuple() 
   { 
      // delete [] boundingBox; delete [] coords; 
   }

  void setData(int dimension_, const real *boundingBox_, const real *coords_, dataT i) 
    {
      if( dimension==0 )
      {
        dimension=dimension_; // fix this **
	// boundingBox=new real[dimension2]; coords=new real[dimension];
      }
      int axis;
      for( axis=0; axis<dimension2; axis++ )
        boundingBox[axis] = boundingBox_[axis];
      for( axis=0; axis<dimension; axis++ )
	coords[axis] = coords_[axis];
      data = i;
    }
  
  int dimension;
//  real boundingBox[dimension2];
//  real coords[dimension];
//  real *boundingBox;
//  real *coords;
  real boundingBox[12];
  real coords[6];
  dataT data;
};

#endif
