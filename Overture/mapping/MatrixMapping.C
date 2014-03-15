#include "MatrixMapping.h"
#include "ParallelUtility.h"



//-------------------------------------------------------------
//
// Matrix Mapping: transformation represented by a 3x4 matrix
// can be used for scaling, rotation, shifts etc.
//     x(0:2) <- Matrix(0:2,0:2)*t(0:2) + matrix(0:2,3)
//
//------------------------------------------------------------

MatrixMapping::
MatrixMapping(int domainDimension_ /* = 3 */, 
              int rangeDimension_ /* = 3 */ )
     : Mapping(domainDimension_,rangeDimension_,cartesianSpace,cartesianSpace )
//===========================================================================
/// \brief  Build a matrix mapping. This is normally used with the {\tt MatrixTransform}
///    to rotate, scale, or translate an existing mapping.
/// \param domainDimension_, rangeDimension_ (input) : domain and range dimension.
/// 
//===========================================================================
{
  MatrixMapping::className="MatrixMapping";         
  domainDimension=domainDimension_;
  rangeDimension=rangeDimension_;
  setName( Mapping::mappingName,"matrix");
  matrix.redim(4,4); matrixInverse.redim(4,4);
  setInvertible( TRUE );             // **** ? ***
  setBasicInverseOption(canInvert);  // basicInverse is available
  inverseIsDistributed=false;        // *wdh* 2011/06/29
  matrix=0;                          // initialize to the indentity
  for( int i=0; i<4; i++ )
    matrix(i,i)=1;
  matrixInverse=matrix;

}


// Copy constructor is deep by default
MatrixMapping::
MatrixMapping( const MatrixMapping & map, const CopyType copyType )
{
  MatrixMapping::className="MatrixMapping";         
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "MatrixMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

MatrixMapping::
~MatrixMapping()
{ if( (Mapping::debug/4) % 2 )
    cout << " MatrixMapping::Desctructor called" << endl;
}

MatrixMapping & MatrixMapping::
operator =( const MatrixMapping & X )
{
  if( MatrixMapping::className != X.getClassName() )
  {
    cout << "MatrixMapping::operator= ERROR trying to set a MatrixMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  matrix=X.matrix;
  matrixInverse=X.matrixInverse;
  return *this;
}

void MatrixMapping::
rotate( const int axis, const real theta )
//===========================================================================
/// \brief  Perform a rotation about a given axis. This rotation is applied
///    after any existing transformations. Use the reset function first if you
///    want to remove any existing transformations.
/// \param axis (input) : axis to rotate about (0,1,2)
/// \param theta (input) : angle in radians to rotate by.
//===========================================================================
{
  // form rotation matrix and compose with matrix
  RealArray rot(4,4);
  rot=0.; 
  int i1 = (axis+1) % 3;
  int i2 = (axis+2) % 3;
  if( (Mapping::debug/4) % 2 )
    cout << "MatrixMapping::rotate axis =" << axis 
         << ", i1 =" << i1 
         << ", i2 =" << i2 << ", theta= " << theta << endl;
  rot(i1,i1)=cos(theta); rot(i1,i2)=-sin(theta);
  rot(i2,i1)=sin(theta); rot(i2,i2)= cos(theta);
  rot(axis,axis)=1.;
  rot(3,3)=1.;
  matrixMatrixProduct( matrix, rot,matrix );
  if( getInvertible() )
    matrixInversion( matrixInverse,matrix );
}

void MatrixMapping::
rotate( const RealArray & rotate, bool incremental /* =false */ )
//===========================================================================
/// \brief  Perform a "rotation" using a $3\times3$ matrix. This does not really have to
///   be a rotation. 
/// \param rotate (input): If incremental=false then the upper $3\times3$ portion of the $4\times4$ transformation
///     matrix will be replaced by the matrix {\tt rotate(0:2,0:2)}. Otherwise this rotation matrix
///     will mutliply the existing transformation.
/// \param incremental (input) : if true apply this rotation to the existing transformation,
///     otherwise replace the existing rotation.
//===========================================================================
{
  // reset(); no wdh 990923
  Range R(0,2);
  if( !incremental )
  {
    matrix(R,R)=rotate(R,R);
  }
  else
  {
    // apply an incremental rotation
    RealArray rot(4,4);
    rot=0.; 
    rot(3,3)=1.;
    rot(R,R)=rotate(R,R);
    matrixMatrixProduct( matrix, rot,matrix );
  }
  if( matrixInversion( matrixInverse,matrix )==0 )
  {
    setInvertible( TRUE );             // **** ? ***
    setBasicInverseOption(canInvert);  // basicInverse is available
  }
  else
  {
    setInvertible( FALSE );  
    setBasicInverseOption(canDoNothing);  // basicInverse is not available
  } 
}

void MatrixMapping::
scale( const real scalex /* =1. */,
       const real scaley /* =1. */, 
       const real scalez /* =1. */, 
       bool incremental  /* =true */  )
//===========================================================================
/// \brief  Perform a scaling 
/// \param scalex, scaley, scalez (input): Scale factors along each axis.
/// \param incremental (input) : if true then incrementally transform the 
///        existing mapping, other transform the original mapping.
//===========================================================================
{
  // scale by factors of scalex in the x-direction, etc.
  RealArray scale(4,4);
  scale=0.;
  scale(xAxis,xAxis)=scalex;
  scale(yAxis,yAxis)=scaley;
  scale(zAxis,zAxis)=scalez;
  scale(3,3)=1.;
  if( incremental )
    matrixMatrixProduct( matrix, scale,matrix );
  else
    matrix=scale;
  if( scalex==0 || scaley==0 || scalez==0 )
    setInvertible( FALSE );
  if( getInvertible() )
    matrixInversion( matrixInverse,matrix );

}

void MatrixMapping::
shift( const real shiftx /* =0. */ , 
       const real shifty /* =0. */ ,
       const real shiftz /* =0. */, 
       bool incremental  /* =true */  )
//===========================================================================
/// \brief  Perform a shift.
/// \param shitx, shity, shitz (input): shifts along each axis.
/// \param incremental (input) : if true then incrementally transform the 
///        existing mapping, other transform the original mapping.
//===========================================================================
{
  // shift by shiftx in the x-direction, etc.
  RealArray shift(4,4);
  shift=0.;
  shift(xAxis,xAxis)=1.; shift(xAxis,3)=shiftx;
  shift(yAxis,yAxis)=1.; shift(yAxis,3)=shifty;
  shift(zAxis,zAxis)=1.; shift(zAxis,3)=shiftz;
  shift(3,3)=1.;
  if( incremental )
    matrixMatrixProduct( matrix, shift,matrix );
  else
    matrix=shift;
  if( getInvertible() )
    matrixInversion( matrixInverse,matrix );
}

void MatrixMapping::
reset()
//===========================================================================
/// \brief  reset the matrix to the identity.
//===========================================================================
{
  matrix=0;                          // initialize to the indentity
  for( int i=0; i<4; i++ )
    matrix(i,i)=1;
  matrixInverse=matrix;
}


// Here is the matrix mapping
void MatrixMapping::
map( const realArray & r, realArray & x, realArray & xr,
                         MappingParameters & params )
{ 
  #ifdef USE_PPP
    realSerialArray rLocal;  getLocalArrayWithGhostBoundaries(r,rLocal);
    realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(x,xLocal);
    realSerialArray xrLocal; getLocalArrayWithGhostBoundaries(xr,xrLocal);
    mapS(rLocal,xLocal,xrLocal,params);
    return;
  #endif

  if( debug & 4  && params.coordinateType != cartesian )
    cerr << "MatrixMapping::map:INFO: - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );
  
  
  if( computeMap )
  {
    for( int i=axis1; i<rangeDimension; i++ )
    {
      x(I,i)=matrix(i,3);              // holds shift
      for( int j=axis1; j<domainDimension; j++)
      {
         x(I,i)=x(I,i)+matrix(i,j)*r(I,j);
      }
    }
  }
  
  if( computeMapDerivative )
  {
    for( int i=axis1; i<rangeDimension; i++ )
    {
      for( int j=axis1; j<domainDimension; j++)
      {
         xr(I,i,j)=matrix(i,j);
      }
    }
  }
  
  if( (Mapping::debug/16) % 2 ==1  )
  {
    matrix.display("MatrixMapping: matrix=");
    r.display("MatrixMapping: r=");
    x.display("MatrixMapping: x=");
  }
    
}

// Here is the inverse matrix mapping
void MatrixMapping::
inverseMap( const realArray & x, realArray & r, realArray & rx,
			  MappingParameters & params )
{ 
  #ifdef USE_PPP
    realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(x,xLocal);
    realSerialArray rLocal;  getLocalArrayWithGhostBoundaries(r,rLocal);
    realSerialArray rxLocal; getLocalArrayWithGhostBoundaries(rx,rxLocal);
    inverseMapS(xLocal,rLocal,rxLocal,params);
    return;
  #endif

  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  if( (Mapping::debug/64) % 2 ==1  )
    cout << "MatrixMapping::inverseMap - params.isNull =" << params.isNull << endl;
  
  if( computeMap )
    for( int i=axis1; i<domainDimension; i++ )
      {
      r(I,i)=matrixInverse(i,3);              // holds shift
      for( int j=axis1; j<rangeDimension; j++)
        {
        r(I,i)=r(I,i)+matrixInverse(i,j)*x(I,j);
      }
    }

  if( computeMapDerivative )
    for( int i=axis1; i<domainDimension; i++ )
      {
      for( int j=axis1; j<rangeDimension; j++)
        {
        rx(I,i,j)=matrixInverse(i,j);
      }
    }

 }

// define a basic inverse too
void MatrixMapping::
basicInverse( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
{ 
  inverseMap( x,r,rx,params );
}

// Here is the matrix mapping
void MatrixMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr,
                         MappingParameters & params )
{ 
  if( debug & 4  && params.coordinateType != cartesian )
    cerr << "MatrixMapping::map:INFO: - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );
  
  
  if( computeMap )
  {
    for( int i=axis1; i<rangeDimension; i++ )
    {
      x(I,i)=matrix(i,3);              // holds shift
      for( int j=axis1; j<domainDimension; j++)
      {
         x(I,i)=x(I,i)+matrix(i,j)*r(I,j);
      }
    }
  }
  
  if( computeMapDerivative )
  {
    for( int i=axis1; i<rangeDimension; i++ )
    {
      for( int j=axis1; j<domainDimension; j++)
      {
         xr(I,i,j)=matrix(i,j);
      }
    }
  }
  
  if( (Mapping::debug/16) % 2 ==1  )
  {
    matrix.display("MatrixMapping: matrix=");
    r.display("MatrixMapping: r=");
    x.display("MatrixMapping: x=");
  }
    
}

// Here is the inverse matrix mapping
void MatrixMapping::
inverseMapS( const RealArray & x, RealArray & r, RealArray & rx,
			  MappingParameters & params )
{ 
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  if( (Mapping::debug/64) % 2 ==1  )
    cout << "MatrixMapping::inverseMap - params.isNull =" << params.isNull << endl;
  
  if( computeMap )
    for( int i=axis1; i<domainDimension; i++ )
      {
      r(I,i)=matrixInverse(i,3);              // holds shift
      for( int j=axis1; j<rangeDimension; j++)
        {
        r(I,i)=r(I,i)+matrixInverse(i,j)*x(I,j);
      }
    }

  if( computeMapDerivative )
    for( int i=axis1; i<domainDimension; i++ )
      {
      for( int j=axis1; j<rangeDimension; j++)
        {
        rx(I,i,j)=matrixInverse(i,j);
      }
    }

 }

// define a basic inverse too
void MatrixMapping::
basicInverseS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
{ 
  inverseMapS( x,r,rx,params );
}

// ---get a mapping from the database---
int MatrixMapping::
get( const GenericDataBase & dir, const aString & name)
{
  if( (Mapping::debug/4) % 2 )
    cout << "Entering MatrixMapping::get" << endl;

  GenericDataBase *subDir = dir.virtualConstructor();
  dir.find(*subDir,name,"Mapping");

  subDir->get( MatrixMapping::className,"className" ); 
  if( MatrixMapping::className != "MatrixMapping" )
  {
    cout << "MatrixMapping::get ERROR in className!" << endl;
  }
  subDir->get( matrix,"matrix" );
  subDir->get( matrixInverse,"matrixInverse" );
  Mapping::get( *subDir, "Mapping" );

  delete subDir;
  return 0;
}

int MatrixMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase *subDir = dir.virtualConstructor();      // create a derived data-base object
  dir.create(*subDir,name,"Mapping");                      // create a sub-directory 

  subDir->put( MatrixMapping::className,"className" );
  subDir->put( matrix,"matrix" );
  subDir->put( matrixInverse,"matrixInverse" );
  Mapping::put( *subDir, "Mapping" );

  delete subDir;
  return 0;
}

Mapping* MatrixMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=NULL;
  if( mappingClassName==MatrixMapping::className )
  {
    retval = new MatrixMapping();
    assert( retval != 0 );
  }
  return retval;
}


//-------------------------------------------------------------------------
//  Utility routines for matrix operations
//------------------------------------------------------------------------

void MatrixMapping::
matrixMatrixProduct( RealArray & m1, const RealArray & m2, const RealArray & m3 )
//===========================================================================
/// \brief  
///    Multiply two 4x4 matrices together. This is a utility routine (a static member function that
///  can be called without a MatrixMapping object using MatrixMapping::matrixMatrixProduct(...)).
///  \begin{verbatim}
///      m1 <- m2*m3
///  \end{verbatim}
//===========================================================================
{
 RealArray m0(4,4);
 for( int i=0; i<4; i++ )
   for( int j=0; j<4; j++ )
    {
     real t=0;
     for( int k=0; k<4; k++ )
       t=t+m2(i,k)*m3(k,j);
      m0(i,j)=t;
    }
 m1=m0;
}

void MatrixMapping:: 
matrixVectorProduct( RealArray & v1, const RealArray & m2, const RealArray & v3 )
//===========================================================================
/// \brief  
///    Multiply a 4x4 matrix times a vector. This is a utility routine (a static member function).
///  \begin{verbatim}
///      v1 <- m2*v3
///  \end{verbatim}
//===========================================================================
{
 RealArray v0(4);
 for( int i=0; i<4; i++ )
  {
   real t=0;
   for( int k=0; k<4; k++ )
     t=t+m2(i,k)*v3(k);
   v0(i)=t;
  }
 v1=v0;
}

int MatrixMapping:: 
matrixInversion( RealArray & m1Inverse, const RealArray & m1 )
//===========================================================================
/// \brief  
///    Invert a 4x4 matrix. This is a utility routine (a static member function).
///    This only works for matrices used in transforming
///  3D vectors which look like:
///  \begin{verbatim}
///       [ a00 a01 a02 a03 ]
///       [ a10 a11 a12 a13 ]
///       [ a20 a21 a22 a23 ]
///       [  0   0   0   1  ]
///  \end{verbatim}
/// \return  0=success, 1=matrix is not invertible
//===========================================================================
{
 RealArray m0(4,4);
 m0=0;
 
 real det,deti;

 det = m1(0,0)*(m1(1,1)*m1(2,2)-m1(2,1)*m1(1,2))
      +m1(1,0)*(m1(2,1)*m1(0,2)-m1(0,1)*m1(2,2))
      +m1(2,0)*(m1(0,1)*m1(1,2)-m1(1,1)*m1(0,2));

 if( det != 0. )
   deti=1./det;
 else
 {
   // cout << "MatrixMapping::matrixInversion: ERROR det=0" << endl;
   m1Inverse=0.;
   return 1;
 }
  
 m0(0,0)= (m1(1,1)*m1(2,2)-m1(2,1)*m1(1,2))*deti;
 m0(0,1)= (m1(2,1)*m1(0,2)-m1(0,1)*m1(2,2))*deti;
 m0(0,2)= (m1(0,1)*m1(1,2)-m1(1,1)*m1(0,2))*deti;

 m0(1,0)= (m1(1,2)*m1(2,0)-m1(2,2)*m1(1,0))*deti;
 m0(1,1)= (m1(2,2)*m1(0,0)-m1(0,2)*m1(2,0))*deti;
 m0(1,2)= (m1(0,2)*m1(1,0)-m1(1,2)*m1(0,0))*deti;

 m0(2,0)= (m1(1,0)*m1(2,1)-m1(2,0)*m1(1,1))*deti;
 m0(2,1)= (m1(2,0)*m1(0,1)-m1(0,0)*m1(2,1))*deti;
 m0(2,2)= (m1(0,0)*m1(1,1)-m1(1,0)*m1(0,1))*deti;

 m0(3,3)=1.;
 m0(0,3)=-( m0(0,0)*m1(0,3)+m0(0,1)*m1(1,3)+m0(0,2)*m1(2,3)) ;
 m0(1,3)=-( m0(1,0)*m1(0,3)+m0(1,1)*m1(1,3)+m0(1,2)*m1(2,3)) ;
 m0(2,3)=-( m0(2,0)*m1(0,3)+m0(2,1)*m1(1,3)+m0(2,2)*m1(2,3)) ;

// ---
// m1.display(" matrixInversion: Here is m1 ");
// m0.display(" matrixInversion: Here is m0 (m1**(-1)) ");
// realArray m2(4,4);
// matrixMatrixProduct( m2,m1,m0 );
// m2.display(" matrixInversion: Here is m1*m1Inverse ");
// ---
 m1Inverse=m0;
 
 return 0;
}
		     

