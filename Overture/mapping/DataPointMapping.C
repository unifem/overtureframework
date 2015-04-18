#include "DataPointMapping.h"
#include "MappingInformation.h"
#include "arrayGetIndex.h"
#include <string.h>
#include "conversion.h"
#include "display.h"
#include "DataFormats.h"
#include "Inverse.h"
#include "ParallelUtility.h"
#include "NurbsMapping.h"

// #define CANINVERT canInvert
#define CANINVERT canInvertWithGoodGuess


//=====================================================================
//       Define a Grid from a set of Data Points
//       ---------------------------------------
//
// NOTES:
//  (1) The 2 or 3 dimensional grid is defined by a set of points.
//  (2) The grid values and derivatives are defined at intermediate
//      points by linear (iord=2) or cubic (iord=4) interpolation.
//      This value is specified when the grid is read in.
//  (3) In the case of linear interpolation the jacobian derivatives are *** no ***
//      determined by linear interpolation of second-order differences.
//      For cubic interpolation the jacobian derivatives are the partial
//      derivatives of the bi-cubic interpolant. Derivative values at the
//      boundaries are obtained by extrapolation of the grid points.
//
// Who to blame:
//   Bill Henshaw
//
//=====================================================================


#define DPM1    EXTERN_C_NAME(dpm1)
#define DPM2    EXTERN_C_NAME(dpm2)
#define OPPLT3D  EXTERN_C_NAME(opplt3d)
#define RDPLT3D  EXTERN_C_NAME(rdplt3d)
#define CLOSEPLT3D EXTERN_C_NAME(closeplt3d)

extern "C"
{

void DPM1(char filename[], int & idata, int & nd, int & ndrsab, int & nrsab, int & bc, int & share, 
	  int & per, int & ndr, const int & fileform, const int & dataform, char errmes[], int & ierr,
	  const int len_filename, const int len_errmes );

void DPM2(int & ndra, int & ndrb, int & ndsa, int & ndsb, int & ndta, int & ndtb, int & ndr, 
	  int & ndrsab, int & nrsab, int & nd, real & xy, int & per, int & idata, char errmes[], 
	  const int & fileform, const int & dataform, int & ierr,
	  const int len_errmes );
  
void OPPLT3D(char  filename[], int & iunit,int & fileFormat,int & ngd, int & ng,
	     int & nx,int & ny,int & nz, const int len_filename);

void RDPLT3D(int & fileFormat,int & iunit, const int & grid, int & nx, int & ny, int & nz,
	     int & nd, int & ndra, int & ndrb, int & ndsa, int & ndsb, int & ndta, int & ndtb, real & xy,
	     int & ierr );

void CLOSEPLT3D(const int & iunit);
}

// *************************************
//  tri-linear interpolant: 
//    INT_2D means the domain dimension is 2
// *************************************
#define INT_1D_ORDER_2(dr,x111,x211)		\
( (1.-dr)*(x111)+dr*(x211) )

#define INT_1D_ORDER_2_R(dr,x111,x211)		\
( delta[0]*( (x211)-(x111) ) )

#define INT_2D_ORDER_2(dr,ds,x111,x211,x121,x221)			\
( (1.-ds)*((1.-dr)*(x111)+dr*(x211))+ds*((1.-dr)*(x121)+dr*(x221)) )

#define INT_2D_ORDER_2_R(dr,ds,x111,x211,x121,x221)		\
( ((1.-ds)*( (x211)-(x111) ) +ds*( (x221)-(x121) ))*delta[0] )

#define INT_2D_ORDER_2_S(dr,ds,x111,x211,x121,x221)		\
( ((1.-dr)*( (x121)-(x111) ) +dr*( (x221)-(x211) ))*delta[1] )

#define INT_3D_ORDER_2(dr,ds,dt,x111,x211,x121,x221,x112,x212,x122,x222) \
(									\
  (1.-dt)*((1.-ds)*((1.-dr)*(x111)+dr*(x211))+ds*((1.-dr)*(x121)+dr*(x221))) \
  +   dt *((1.-ds)*((1.-dr)*(x112)+dr*(x212))+ds*((1.-dr)*(x122)+dr*(x222))) )

#define INT_3D_ORDER_2_R(dr,ds,dt,x111,x211,x121,x221,x112,x212,x122,x222) \
(									\
  ( (1.-dt)*((1.-ds)*((x211)-(x111))+ds*((x221)-(x121)))		\
    +       dt *((1.-ds)*((x212)-(x112))+ds*((x222)-(x122))) )*delta[0])

#define INT_3D_ORDER_2_S(dr,ds,dt,x111,x211,x121,x221,x112,x212,x122,x222) \
(									\
  ( (1.-dt)*((1.-dr)*((x121)-(x111))+dr*((x221)-(x211)))		\
    +       dt *((1.-dr)*((x122)-(x112))+dr*((x222)-(x212))) )*delta[1] )

#define INT_3D_ORDER_2_T(dr,ds,dt,x111,x211,x121,x221,x112,x212,x122,x222) \
(									\
  ( (1.-dr)*((1.-ds)*((x112)-(x111))+ds*((x122)-(x121)))		\
    +        dr *((1.-ds)*((x212)-(x211))+ds*((x222)-(x221))) )*delta[2] )

/* ---

   define INT_1D_ORDER_2(dr,x111,x211)  \
   ( (1.-dr)*(x111)+dr*(x211) )

   define INT_1D_ORDER_2_R(dr,x111,x211)  \
   ( delta[0]*( (x211)-(x111) ) )

   define INT_2D_ORDER_2(dr,ds,x111,x211,x121,x221)  \
   ( (1.-ds)*(1.-dr)*(x111)+(1.-ds)*dr*(x211)+ds*(1.-dr)*(x121)+ds*dr*(x221) )

   define INT_2D_ORDER_2_R(dr,ds,x111,x211,x121,x221)  \
   ( (1.-ds)*delta[0]*( (x211)-(x111) ) +ds*delta[0]*( (x221)-(x121) ) )

   define INT_2D_ORDER_2_S(dr,ds,x111,x211,x121,x221)  \
   ( (1.-dr)*delta[1]*( (x121)-(x111) ) +dr*delta[1]*( (x221)-(x211) ) )

   define INT_3D_ORDER_2(dr,ds,dt,x111,x211,x121,x221,x112,x212,x122,x222)  \
   (                                                    \
   (1.-dt)*(1.-ds)*(1.-dr)*(x111)+(1.-dt)*(1.-ds)*dr*(x211)+(1.-dt)*ds*(1.-dr)*(x121)+(1.-dt)*ds*dr*(x221) \
   +   dt*(1.-ds)*(1.-dr)*(x112)+dt*(1.-ds)*dr*(x212)+dt*ds*(1.-dr)*(x122)+dt*ds*dr*(x222) )

   define INT_3D_ORDER_2_R(dr,ds,dt,x111,x211,x121,x221,x112,x212,x122,x222)  \
   (                                                    \
   delta[0]*(1.-dt)*(1.-ds)*((x211)-(x111))+delta[0]*(1.-dt)*ds*((x221)-(x121)) \
   + delta[0]*    dt* (1.-ds)*((x212)-(x112))+delta[0]*    dt *ds*((x222)-(x122)) )

   define INT_3D_ORDER_2_S(dr,ds,dt,x111,x211,x121,x221,x112,x212,x122,x222)  \
   (  \
   delta[1]*(1.-dt)*(1.-dr)*((x121)-(x111))+delta[1]*(1.-dt)*dr*((x221)-(x211)) \
   + delta[1]*    dt *(1.-dr)*((x122)-(x112))+delta[1]*    dt *dr*((x222)-(x212)) )

   define INT_3D_ORDER_2_T(dr,ds,dt,x111,x211,x121,x221,x112,x212,x122,x222)  \
   (                                                    \
   delta[2]*(1.-dr)*(1.-ds)*((x112)-(x111))+ delta[2]*(1.-dr)*ds*((x122)-(x121)) \
   + delta[2]*    dr *(1.-ds)*((x212)-(x211))+ delta[2]*    dr *ds*((x222)-(x221)) )

   ----- */
// // ***************************************
// // Define jacobian entries by differencing
// // ***************************************
// #define XYRS2(i1,i2,i3,axis,dir) \
//            (xy(i1+i1d(dir),i2+i2d(dir),i3,axis)            \
//            -xy(i1-i1d(dir),i2-i2d(dir),i3,axis))*deltaByTwo[dir] 

// #define XYRS3(i1,i2,i3,axis,dir) \
//            (xy(i1+i1d(dir),i2+i2d(dir),i3+i3d(dir),axis)            \
//            -xy(i1-i1d(dir),i2-i2d(dir),i3-i3d(dir),axis))*deltaByTwo[dir] 

// // ******************
// // cubic interpolant:
// // ******************
// #define q03(z)  (-oneSixth  *((z)-1.)*((z)-2.)*((z)-3.))
// #define q13(z)  ( .5*(z)       *((z)-2.)*((z)-3.))
// #define q23(z)  (-.5*(z)*((z)-1.)       *((z)-3.))
// #define q33(z)  ( oneSixth*(z)*((z)-1.)*((z)-2.))
// #define q03d(z) ( -oneSixth*(11.+(z)*(-12.+3.*(z))))
// #define q13d(z) ( 3.+(z)*(-5.+1.5*(z)))
// #define q23d(z) ( -1.5+(z)*(4.-1.5*(z)))
// #define q33d(z) (oneSixth*(2.+(z)*(-6.+3.*(z))))

// #define q1x(i1,i2,i3,axis)  \
//       (  a0(I)* xyl(i1  ,i2  ,i3  ,axis) \
//         +a1(I)* xyl(i1+1,i2  ,i3  ,axis) \
//         +a2(I)* xyl(i1+2,i2  ,i3  ,axis) \
//         +a3(I)* xyl(i1+3,i2  ,i3  ,axis) )
// #define q2x(i1,i2,i3,axis)  \
//       (  b0(I)*q1x(i1  ,i2  ,i3  ,axis) \
//         +b1(I)*q1x(i1  ,i2+1,i3  ,axis) \
//         +b2(I)*q1x(i1  ,i2+2,i3  ,axis) \
//         +b3(I)*q1x(i1  ,i2+3,i3  ,axis) )
// #define q3x(i1,i2,i3,axis)  \
//       (  c0(I)*q2x(i1  ,i2  ,i3  ,axis) \
//         +c1(I)*q2x(i1  ,i2  ,i3+1,axis) \
//         +c2(I)*q2x(i1  ,i2  ,i3+2,axis) \
//         +c3(I)*q2x(i1  ,i2  ,i3+3,axis) )
// #define q1xr(i1,i2,i3,axis)   \
//                    (    a0r(I)*xyl(i1  ,i2  ,i3  ,axis)   \
//                        +a1r(I)*xyl(i1+1,i2  ,i3  ,axis)   \
//                        +a2r(I)*xyl(i1+2,i2  ,i3  ,axis)   \
//                        +a3r(I)*xyl(i1+3,i2  ,i3  ,axis) )
// #define q2xr(i1,i2,i3,axis)   \
//                     (   b0(I)*q1xr(i1  ,i2  ,i3  ,axis)   \
//                        +b1(I)*q1xr(i1  ,i2+1,i3  ,axis)   \
//                        +b2(I)*q1xr(i1  ,i2+2,i3  ,axis)   \
//                        +b3(I)*q1xr(i1  ,i2+3,i3  ,axis) )
// #define q3xr(i1,i2,i3,axis)   \
//                      (  c0(I)*q2xr(i1  ,i2  ,i3  ,axis)   \
//                        +c1(I)*q2xr(i1  ,i2  ,i3+1,axis)   \
//                        +c2(I)*q2xr(i1  ,i2  ,i3+2,axis)   \
//                        +c3(I)*q2xr(i1  ,i2  ,i3+3,axis)  ) 

// #define q1xs(i1,i2,i3,axis)   \
//                    (    a0(I)*  xyl(i1  ,i2  ,i3  ,axis)   \
//                        +a1(I)*  xyl(i1+1,i2  ,i3  ,axis)   \
//                        +a2(I)*  xyl(i1+2,i2  ,i3  ,axis)   \
//                        +a3(I)*  xyl(i1+3,i2  ,i3  ,axis)  ) 
// #define q2xs(i1,i2,i3,axis)   \
//                      (  b0r(I)*q1xs(i1  ,i2  ,i3  ,axis)   \
//                        +b1r(I)*q1xs(i1  ,i2+1,i3  ,axis)   \
//                        +b2r(I)*q1xs(i1  ,i2+2,i3  ,axis)   \
//                        +b3r(I)*q1xs(i1  ,i2+3,i3  ,axis))   
// #define q3xs(i1,i2,i3,axis)   \
//                      (  c0(I)*q2xs(i1  ,i2  ,i3  ,axis)   \
//                        +c1(I)*q2xs(i1  ,i2  ,i3+1,axis)   \
//                        +c2(I)*q2xs(i1  ,i2  ,i3+2,axis)   \
//                        +c3(I)*q2xs(i1  ,i2  ,i3+3,axis) )  

// #define q1xt(i1,i2,i3,axis)   \
//                      (  a0(I)*  xyl(i1  ,i2  ,i3  ,axis)   \
//                        +a1(I)*  xyl(i1+1,i2  ,i3  ,axis)   \
//                        +a2(I)*  xyl(i1+2,i2  ,i3  ,axis)   \
//                        +a3(I)*  xyl(i1+3,i2  ,i3  ,axis) )  
// #define q2xt(i1,i2,i3,axis)   \
//                     (   b0(I)*q1xt(i1  ,i2  ,i3  ,axis)   \
//                        +b1(I)*q1xt(i1  ,i2+1,i3  ,axis)   \
//                        +b2(I)*q1xt(i1  ,i2+2,i3  ,axis)   \
//                        +b3(I)*q1xt(i1  ,i2+3,i3  ,axis) )  
// #define q3xt(i1,i2,i3,axis)   \
//                      (  c0r(I)*q2xt(i1  ,i2  ,i3  ,axis)   \
//                        +c1r(I)*q2xt(i1  ,i2  ,i3+1,axis)   \
//                        +c2r(I)*q2xt(i1  ,i2  ,i3+2,axis)   \
//                        +c3r(I)*q2xt(i1  ,i2  ,i3+3,axis) )  




DataPointMapping::
DataPointMapping() 
  : Mapping(2,2,parameterSpace,cartesianSpace) 
//===========================================================================
/// \brief  Default Constructor. 
//===========================================================================
{ 
  DataPointMapping::className="DataPointMapping";
  setName( Mapping::mappingName,"DataPointMapping");
  orderOfInterpolation=2;
  mappingInitialized=FALSE;  
  for( int axis=0; axis<3; axis++ )
  {
    for(int side=0; side<=1; side++ )
    {
      setNumberOfGhostPoints(side,axis,2);  
    }
  }
  
//   #ifdef USE_PPP
//     mapIsDistributed=true;  
//     inverseIsDistributed=true;
//   #else
//     mapIsDistributed=false;  
//     inverseIsDistributed=false;
//   #endif
  
  // We should always set this to be true: (in case the grid is created in serial but read back in parallel)
  mapIsDistributed=true;  
  inverseIsDistributed=true;
  
  setNumberOfDistributedGhostLines( 2 );   // in parallel we want two ghost lines for 4th order interpolation

  useScalarIndexing=true;

  evalAsNurbs=false; // if true, fit the DPM to a Nurbs and eval the Nurbs.
  nurbsDegree=3;     // degree of the NURBS
  nurbsOutOfDate=true;

  mappingHasChanged();
}



// Copy constructor is deep by default
DataPointMapping::
DataPointMapping( const DataPointMapping & map, const CopyType copyType )
{
  DataPointMapping::className="DataPointMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "DataPointMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }

  printF("DPM::Copy constructor for mapping %s, numGhost=[%i,%i]\n",
	 (const char*)getName(mappingName),numberOfGhostPoints(0,0),numberOfGhostPoints(1,0));
}

DataPointMapping::~DataPointMapping()
{ 
  if( debug & 4 )
    printF(" --DPM-- DataPointMapping::Destructor called\n");

  if( dbase.has_key("nurbs") )
  {
    delete dbase.get<NurbsMapping*>("nurbs");
  }
}

// ===========================================================================================
/// \brief Internally convert DPM to a Nurbs for evaluation
// ===========================================================================================
int DataPointMapping::
useNurbsToEvaluate( bool trueOrFalse )
{
  evalAsNurbs=trueOrFalse;
  if( evalAsNurbs )
  {
    mapIsDistributed=false;    // for now the NurbsMapping is NOT distributed
    inverseIsDistributed=false;    
    // printF("---DPM--INFO: setting evalAsNurbs=true : internally convert the DPM to a Nurbs for evaluation.\n");  
  }
  else
  {
    // printF("---DPM--INFO: setting evalAsNurbs=false\n");
    
    mapIsDistributed=true;  
    inverseIsDistributed=true;
  }
  return 0;
}

// ===========================================================================================
/// \brief Set the degree of the Nurbs used to (optionally) evaluate the DPM. 
// ===========================================================================================
int DataPointMapping::
setDegreeOfNurbs( int degree )
{
  nurbsDegree=degree;
  return 0;
}



const realArray& DataPointMapping::
getGrid(MappingParameters & params /* =Overture::nullMappingParameters() */,
        bool includeGhost /* =false */ )
// ==========================================================================
//  /Purpose:  Return the grid that can be used for plotting the mapping
//      or for the inverse.
// ==========================================================================
{
//   printF("DPM::getGrid for mapping %s, includeGhost=%i numGhost=[%i,%i]\n",
//             (const char*)getName(mappingName),includeGhost,numberOfGhostPoints(0,0),numberOfGhostPoints(1,0));
  
  if( !gridIsValid() )
  {
    // determine the sign of the jacobian if domainDimension==rangeDimension
    getSignForJacobian();
  }

  int sameDimensions =TRUE;
  for( int axis=0; axis<domainDimension; axis++ )
  {
    sameDimensions= sameDimensions && 
      ( gridIndexRange(End,axis)-gridIndexRange(Start,axis) == getGridDimensions(axis)-1 );
    if( !sameDimensions )
      break;
  }
  if( sameDimensions )
  {
    if( Mapping::debug & 4 )
      cout << "DataPointMapping::getGrid(): using existing grid \n";
    setGridIsValid();
    if( !includeGhost )
    {
      Range I1(gridIndexRange(0,0),gridIndexRange(1,0));
      Range I2(gridIndexRange(0,1),gridIndexRange(1,1));
      Range I3(gridIndexRange(0,2),gridIndexRange(1,2));
      Range Rx=rangeDimension;
      grid.reference(xy(I1,I2,I3,Rx));
      return grid;
    }
    else
    {
      
      return xy;  // *wdh* April 1, 2007 -- return xy including ghost points 
    }
  }
  else
  {
    if( Mapping::debug & 4 )
      cout << "DataPointMapping::getGrid(): sameDimensions=FALSE \n";
    grid.redim(0);
    return Mapping::getGrid(params,includeGhost);  // compute if number of grid points has changed
  }
  setGridIsValid();

}

const realArray& DataPointMapping::
getDataPoints()
//===========================================================================
/// \details 
///   Return the array of data points. It will not be the same array as was given to
///     setDataPoints since ghostlines will have been added. Use getGridIndexRange
///     to determine the index positions for the grid boundaries.
/// \return  array of data points, xy(I1,I2,I3,0:r-1), r=rangeDimension
//===========================================================================
{
  return xy;
}

const IntegerArray & DataPointMapping::
getGridIndexRange()
//===========================================================================
/// \details 
///   Return the gridIndexRange array for the data points. These values indicate
///   the index positions for the grid boundaries.
/// \return  The gridIndexRange(0:1,0:2).
//===========================================================================
{
  return gridIndexRange;
}

const IntegerArray & DataPointMapping::
getDimension()
//===========================================================================
/// \details 
///   Return the dimension array for the data points. These values indicate
///   the index positions for the array dimensions.
/// \return  The dimension(0:1,0:2).
//===========================================================================
{
  return dimension;
}


#define ForBoundary(side,axis)   for( axis=0; axis<domainDimension; axis++ ) \
for( side=0; side<=1; side++ )

int DataPointMapping::
setDataPoints(const realArray & xd,
              const int positionOfCoordinates /* =3 */, 
              const int domainDimension_ /* =-1 */,
              const int numberOfGhostLinesInData /* = 0 */,
              const IntegerArray & xGridIndexRange /* = Overture::nullIntArray() */ ) 
//===========================================================================
/// \brief  
///    Supply data points as
///     <ol>
///       <li>  xd(0:r-1,I,J,K) if positionOfCoordinates==0 $\rightarrow$ domainDimension=domainDimension\_
///       <li>  xd(I,0:r-1)     if positionOfCoordinates==1 $\rightarrow$ domainDimension=1
///       <li>  xd(I,J,0:r-1)   if positionOfCoordinates==2 $\rightarrow$ domainDimension=2
///       <li>  xd(I,J,K,0:r-1) if positionOfCoordinates==3 $\rightarrow$ domainDimension=domainDimension\_
///     </ul>
///    where r=number of dimensions (range dimension)
///
/// \param xd (input): An array of values defining the coordinates of a grid of points. This routine make a COPY
///    of this array.
///
/// \param positionOfCoordinates (input): indicates the "shape" of the input array xd.
// 
/// \param domainDimension_ (input): As indicated above this parameter defines the domainDimension when
///     positionOfCoordinates is 0 or 3.
///
/// \param numberOfGhostLinesInData (input) : The data includes the coordinates of this many ghost lines (for all sides).
///     NOTE: these values are NOT USED if the xGridIndexRange arguement is provided. 
///
/// \param xGridIndexRange (input): If this array is not null and size (2,0:r-1) then these values indicate the 
///     points in the array xd that represent the boundary points on the grid. OTHER values of the xd array
///     will be used as ghost points
///
/// \param Remarks:
///    Note that by default the DataPointMapping will have the properties
///    <ul>
///      <li> domainSpace = parameterSpace
///      <li> rangeSpace = cartesianSpace
///      <li> not periodic
///      <li> boundary conditions all 1
///    </ul>
///    You will have to change the above properties as appropriate.  
///    NOTE: you should set the periodicity of this mapping before supplying data points.      
//===========================================================================
{
  int ngid[2][3];
  for( int axis=0; axis<3; axis++ )
    for(int side=0; side<=1; side++ )
      ngid[side][axis]=numberOfGhostLinesInData;
  return setDataPoints(xd,positionOfCoordinates,domainDimension_,ngid,xGridIndexRange);
}

int DataPointMapping::
setDataPoints(const realArray & xd, 
	      const int positionOfCoordinates, 
	      const int domainDimension_,
	      const int numberOfGhostLinesInData[2][3],
	      const IntegerArray & xGridIndexRange /* = Overture::nullIntArray() */ )
// ===========================================================================================
/// \details 
///     Supply data points: Same as above routine except that the numberOfGhostLinesInData can
///   be defined as separate values for each face.
/// \param numberOfGhostLinesInData[side][axis] : specify the number of ghostlines in the input data
///     for each face.
// ===========================================================================================
{


  assert( positionOfCoordinates>-1 && positionOfCoordinates<4 );
  
  int nd = xd.getLength(positionOfCoordinates);
  if( nd<1 || nd > 3 )
  {
    cout << "DataPointMapping: error: array xd being passed to setDataPoints has positionOfCoodinates = " <<
      positionOfCoordinates << "\n but the length of that position in xd is not 1,2,3, xd.getLength(positionOfCoordinates)="
	 << xd.getLength(positionOfCoordinates) << endl;
    Overture::abort("error");
  }
  setRangeDimension(nd);
  if( positionOfCoordinates==0 || positionOfCoordinates==3 )
  {
    if( domainDimension_<1 || domainDimension_ > 3 )
    {
      cout << "DataPointMapping:setDataPoints: ERROR: argument domainDimension = " << domainDimension_ <<endl;
      cout << "Should be 1,2, or 3 \n";
      cout << "positionOfCoordinates = " << positionOfCoordinates << endl;
      Overture::abort("error");
    }
    setDomainDimension(domainDimension_);
  }
  else 
    setDomainDimension(positionOfCoordinates);

//  if( orderOfInterpolation==2 && (
//    (domainDimension==2 && rangeDimension==2) || (domainDimension==3 && rangeDimension==3)) )

  if( domainDimension==2 || domainDimension==3 )
  {
    // printf(" **** DPM basicInverse available ******\n");
    setBasicInverseOption(CANINVERT);  // basicInverse is available with a good guess
  }
  else
    setBasicInverseOption(canDoNothing);
  
  dimension.redim(2,3);      dimension=0;
  gridIndexRange.redim(2,3);  gridIndexRange=0;
  delta[0]=delta[1]=delta[2]=1.;
  deltaByTwo[0]=deltaByTwo[1]=deltaByTwo[2]=.5;

  // we may need to build extra ghost lines if the grid is highly stretched near the boundary. This
  // will ensure that the inverse will still work for points slightly outside the domain (points that
  // may look far away compared to the boundary spacing)



// numberOfGhostLines=2;  // always create 2, in case user changes from 2nd-order to fourth-order
// **  int numberOfGhostLines=4;  // always create 2, in case user changes from 2nd-order to fourth-order

  // ngid(side,axis) : number of valid ghost points supplied in the data
  //                 : we do NOT to obtain these points by extrapolation
  IndexRangeType ngid;
  for( int axis=0; axis<3; axis++ ) for( int side=0; side<=1; side++ ) ngid(side,axis)=0;

  int offset = positionOfCoordinates == 0 ? 1 : 0;
  if( xGridIndexRange.getLength(0)>=2 && xGridIndexRange.getLength(1)>=domainDimension )
  {
    // -- If xGridIndexRange is provided then we use any existing ghost points in the xd array ---
    for( int axis=axis1; axis<domainDimension; axis++ )
    {
      ngid(0,axis)=xGridIndexRange(0,axis)-xd.getBase(axis+offset);
      ngid(1,axis)=xd.getBound(axis+offset)-xGridIndexRange(1,axis);
    }
  }
  else
  {
    for( int axis=axis1; axis<domainDimension; axis++ )
    {
      ngid(0,axis)=numberOfGhostLinesInData[0][axis];
      ngid(1,axis)=numberOfGhostLinesInData[1][axis];
    }
  }
  for( int axis=axis1; axis<domainDimension; axis++ )
  {
    setGridDimensions(axis,xd.getLength(axis+offset)-ngid(0,axis)-ngid(1,axis));
  }
  
//     printF("DPM:setDataPoints: num-ghost-in-data: ngid=[%i,%i][%i,%i][%i,%i]\n",ngid(0,0),ngid(1,0),
// 	   ngid(0,1),ngid(1,1),ngid(0,2),ngid(1,2));
  

  int xdBase1=xd.getBase(axis1+offset)+ngid(0,0);
  int xdBase2=xd.getBase(axis2+offset)+ngid(0,1);
  int xdBase3=xd.getBase(axis3+offset)+ngid(0,2);
  int xdBase[3] = { xdBase1,xdBase2,xdBase3 };

  const int defaultNumberOfGhostLines=2;  // fix me 

  IndexRangeType numberOfGhostLinesOld; // *wdh* 2012/03/21 -- increase ghost lines to match ngid
  numberOfGhostLinesOld=0;
  for( int axis=axis1; axis<domainDimension; axis++ )
  {
    gridIndexRange(Start,axis)=0; // *** alway keep this = 0, assumed for plotting ?? ***
    gridIndexRange(End  ,axis)=xd.getBound(axis+offset)-ngid(1,axis) -xdBase[axis];
    
    // dimension(Start,axis)=gridIndexRange(Start,axis)-numberOfGhostPoints(Start,axis);
    // dimension(End  ,axis)=gridIndexRange(End  ,axis)+numberOfGhostPoints(End,axis);

    // *wdh* 2012/03/21 -- dimension to include all provided ghost points 
    int nga = max(defaultNumberOfGhostLines,ngid(Start,axis));
    int ngb = max(defaultNumberOfGhostLines,ngid(End  ,axis));
    if( orderOfInterpolation==2 )
    {
      // When the orderOfInterpolation==2 the last ghost line is not used in evaluating the mapping (so that the
      //   derivative of the mapping (which does use the last line) is consistent. (I think)
      // When the user provides 2 or more ghost lines of data we add an additional ghost line --
      // this is used for grids from hype that specify a large boundary offset. (we don't always do this
      // for backward compatibility)
      if( ngid(Start,axis)>=2 )
	nga = max(defaultNumberOfGhostLines,ngid(Start,axis)+1);
      if( ngid(End  ,axis)>=2 )
        ngb = max(defaultNumberOfGhostLines,ngid(End  ,axis)+1);
    }
    

    dimension(Start,axis)=gridIndexRange(Start,axis)-nga;
    dimension(End  ,axis)=gridIndexRange(End  ,axis)+ngb;

    numberOfGhostLinesOld(0,axis)=ngid(0,axis);
    numberOfGhostLinesOld(1,axis)=ngid(1,axis);
    

    delta[axis]=gridIndexRange(End,axis)-gridIndexRange(Start,axis);
    deltaByTwo[axis]=.5*max(1.,delta[axis]);
  }

//   printF("DPM:setDataPoints: gridIndexRange=[%i,%i][%i,%i][%i,%i]\n",gridIndexRange(0,0),gridIndexRange(1,0),
// 	   gridIndexRange(0,1),gridIndexRange(1,1),gridIndexRange(0,2),gridIndexRange(1,2));
//   printF("DPM:setDataPoints: dimension=[%i,%i][%i,%i][%i,%i]\n",dimension(0,0),dimension(1,0),
// 	   dimension(0,1),dimension(1,1),dimension(0,2),dimension(1,2));

  // save points in the local array
  xy.partition(partition);
  xy.redim(Range(dimension(Start,0),dimension(End,0)),
           Range(dimension(Start,1),dimension(End,1)),
           Range(dimension(Start,2),dimension(End,2)),
           rangeDimension);
#ifdef USE_PPP
  realSerialArray xyLocal; getLocalArrayWithGhostBoundaries(xy,xyLocal);
  realSerialArray xdLocal; getLocalArrayWithGhostBoundaries(xd,xdLocal);
#else
  realSerialArray & xyLocal = xy;
  const realSerialArray & xdLocal = xd;
#endif
  
  xyLocal=0.; // to avoid UMRs when extrapolating ghost points.
  
  Index I1,I2,I3, R(0,rangeDimension);
  I1=dimension(0,0);  // set defaults *wdh* 081116
  I2=dimension(0,1);
  I3=dimension(0,2);
  if( positionOfCoordinates==0 )
  {
    for( int axis=0; axis<rangeDimension; axis++ )
      for( int i3=gridIndexRange(Start,axis3)-ngid(0,2); i3<=gridIndexRange(End,axis3)+ngid(1,2); i3++ )
	for( int i2=gridIndexRange(Start,axis2)-ngid(0,1); i2<=gridIndexRange(End,axis2)+ngid(1,1); i2++ )
	  for( int i1=gridIndexRange(Start,axis1)-ngid(0,0); i1<=gridIndexRange(End,axis1)+ngid(1,0); i1++ )
	    xyLocal(i1,i2,i3,axis)=xdLocal(axis,i1+xdBase1,i2+xdBase2,i3+xdBase3);
  }
  else if( positionOfCoordinates==1 )
  {
    for( int axis=0; axis<rangeDimension; axis++ )
    {
      // for( int i1=gridIndexRange(Start,axis1)-ngid(0,0); i1<=gridIndexRange(End,axis1)+ngid(1,0); i1++ )
      I1=Range(max(xy.getBase(0),gridIndexRange(Start,axis1)-ngid(0,0)),
	       min(xy.getBound(0),gridIndexRange(End,axis1)+ngid(1,0)));
      bool ok=ParallelUtility::getLocalArrayBounds(xy,xyLocal, I1,I2,I3, 1);//include parallel ghost
      if(ok)
	xyLocal(I1,0,0,axis)=xdLocal(I1+xdBase1,axis);
    }
  }
  else if( positionOfCoordinates==2 )
  {
    for( int axis=0; axis<rangeDimension; axis++ )
    {
      // for( int i2=gridIndexRange(Start,axis2)-ngid(0,1); i2<=gridIndexRange(End,axis2)+ngid(1,1); i2++ )
      // for( int i1=gridIndexRange(Start,axis1)-ngid(0,0); i1<=gridIndexRange(End,axis1)+ngid(1,0); i1++ )
      I1=Range(max(xy.getBase(0),gridIndexRange(Start,axis1)-ngid(0,0)),
	       min(xy.getBound(0),gridIndexRange(End,axis1)+ngid(1,0)));
      I2=Range(max(xy.getBase(1),gridIndexRange(Start,axis2)-ngid(0,1)),
	       min(xy.getBound(1),gridIndexRange(End,axis2)+ngid(1,1)));
      I3 = xy.getBase(2);
      bool ok=ParallelUtility::getLocalArrayBounds(xy,xyLocal, I1,I2,I3, 1);//include parallel ghost
      if(ok)
	xyLocal(I1,I2,0,axis)=xdLocal(I1+xdBase1,I2+xdBase2,axis);
    }
  }
  else if( positionOfCoordinates==3 )
  {
    I1=Range(max(xy.getBase(0),gridIndexRange(Start,axis1)-ngid(0,0)),
             min(xy.getBound(0),gridIndexRange(End,axis1)+ngid(1,0)));
    I2=Range(max(xy.getBase(1),gridIndexRange(Start,axis2)-ngid(0,1)),
             min(xy.getBound(1),gridIndexRange(End,axis2)+ngid(1,1)));
    I3=Range(max(xy.getBase(2),gridIndexRange(Start,axis3)-ngid(0,2)),
             min(xy.getBound(2),gridIndexRange(End,axis3)+ngid(1,2)));
    bool ok=ParallelUtility::getLocalArrayBounds(xy,xyLocal, I1,I2,I3, 1);//include parallel ghost
    if(ok)
      xyLocal(I1,I2,I3,R)=xdLocal(I1+xdBase1,I2+xdBase2,I3+xdBase3,R);
  }
  
  // set bounds on the range
  I1=Range(gridIndexRange(Start,axis1),gridIndexRange(End,axis1));
  I2=Range(gridIndexRange(Start,axis2),gridIndexRange(End,axis2));
  I3=Range(gridIndexRange(Start,axis3),gridIndexRange(End,axis3));
  for( int axis=0; axis<rangeDimension; axis++ )
  {
    setRangeBound(Start,axis,min(xy(I1,I2,I3,axis))); // *** FIX ME FOR PARALLEL ***
    setRangeBound(End  ,axis,max(xy(I1,I2,I3,axis)));
  }

  // xy.display("xy before extrap");
  
//   printF(" KKKKKKK dpm:setDataPoints:            dimension=[%i,%i][%i,%i][%i,%i]\n",dimension(0,0),dimension(1,0),
//  	 dimension(0,1),dimension(1,1),dimension(0,2),dimension(1,2));
//   printF(" KKKKKKK dpm:setDataPoints:       gridIndexRange=[%i,%i][%i,%i][%i,%i]\n",
//          gridIndexRange(0,0),gridIndexRange(1,0),
//  	 gridIndexRange(0,1),gridIndexRange(1,1),gridIndexRange(0,2),gridIndexRange(1,2));
//   printF(" KKKKKKK dpm:setDataPoints:                 ngid=[%i,%i][%i,%i][%i,%i]\n",ngid(0,0),ngid(1,0),
//  	 ngid(0,1),ngid(1,1),ngid(0,2),ngid(1,2));
//   printF(" KKKKKKK dpm:setDataPoints:  numberOfGhostPoints=[%i,%i][%i,%i][%i,%i]\n",
//          numberOfGhostPoints(0,0),numberOfGhostPoints(1,0),
//  	 numberOfGhostPoints(0,1),numberOfGhostPoints(1,1),numberOfGhostPoints(0,2),numberOfGhostPoints(1,2));

//   if( true )
//   {
  
//     printF("DPM:Before ghost: xy=[%i,%i][%i,%i][%i,%i]\n",xy.getBase(0),xy.getBound(0),xy.getBase(1),xy.getBound(1),
//            xy.getBase(2),xy.getBound(2));
    
//     int i1=4, i2=-3, i3=-1;
//     if( i2>=xy.getBase(1) )
//     {
//       printF("DPM:Before ghost: xy(%i,%i,%i)=(%e,%e,%e)\n",i1,i2,i3,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2));
//     }
//   }

  //   Determine values at ghost points that have not been user set:  extrapolate or use periodicity
  // computeGhostPoints( ngid,numberOfGhostPoints );
  computeGhostPoints( numberOfGhostLinesOld,numberOfGhostPoints );

  // xy.display("xy after extrap");

//   int side, is[3],ip[3];
//   ForBoundary(side,axis)
//   {
//     for( int dir=0; dir<3; dir++ )
//     {
//       is[dir] = dir!=axis ? 0 : ( side==Start ? +1 : -1 );
//       ip[dir] = (gridIndexRange(End,dir)-gridIndexRange(Start,dir))*is[dir];
//     }
//     for( int ghost=1+ngid[side][axis]; ghost<=numberOfGhostLines[side][axis]; ghost++ )
//     {
//       getGhostIndex(gridIndexRange,side,axis,I1,I2,I3,ghost,numberOfGhostLines[side][axis]);
//       if( getIsPeriodic(axis)==functionPeriodic )
//         xy(I1,I2,I3,R)=xy(I1+ip[0],I2+ip[1],I3+ip[2],R);
//       else 
//       {
//         // xy(I1,I2,I3,R)=2.*xy(I1+is[0],I2+is[1],I3+is[2],R)-xy(I1+2*is[0],I2+2*is[1],I3+2*is[2],R);
//         // extrapolate in a different way that work better on stretched grids so that the grid is reflected.
//         const int m=ghost, m2=2*m;
//         xy(I1,I2,I3,R)=2.*xy(I1+m*is[0],I2+m*is[1],I3+m*is[2],R)-xy(I1+m2*is[0],I2+m2*is[1],I3+m2*is[2],R);
	
//       }
      
//     }
//   }


  reinitialize();  // *wdh* we have to re-initialize the inverse 000503
   
//   if( true )
//   {
//     int i1=4, i2=-3, i3=-1;
//     if( i2>=xy.getBase(1) )
//     {
//       printF("DPM:After ghost: xy(%i,%i,%i)=(%e,%e,%e)\n",i1,i2,i3,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2));
//     }
//   }

//  xy.display("\n\n xy after extrap");
  mappingInitialized=TRUE;
  mappingHasChanged();
  return 0;
}


int DataPointMapping::
setDataPoints(const realSerialArray & x, 
              const int domainDimension_, const int rangeDimension_,
              const IntegerArray & xDimension,
	      const IntegerArray & xGridIndexRange  )
// ===========================================================================================
/// /brief: (Parallel version)  Supply data points in a serial array (for ech processor).
///
/// /param x(I1,I2,I3,0:rangeDimension-1) (input) : serial array holding (at least)
///      the data points on this processor include parallel ghost.
/// 
/// /param domainDimension (input): 
/// /param rangeDimension (input): 
/// /param xDimension(2,3) (input) : global dimension array (includes ghost points)
/// /param xGridIndexRange(2,3) (input) : gridIndexRange
// ===========================================================================================
{

  int positionOfCoordinates=3;
  
  setDomainDimension(domainDimension_);
  setRangeDimension(rangeDimension_);

  if( domainDimension==2 || domainDimension==3 )
  {
    // printf(" **** DPM basicInverse available ******\n");
    setBasicInverseOption(CANINVERT);  // basicInverse is available with a good guess
  }
  else
    setBasicInverseOption(canDoNothing);
  
  dimension.redim(2,3);       dimension=xDimension;
  gridIndexRange.redim(2,3);  gridIndexRange=xGridIndexRange;
  delta[0]=delta[1]=delta[2]=1.;
  deltaByTwo[0]=deltaByTwo[1]=deltaByTwo[2]=.5;

  IndexRangeType ngid; // number of ghost in data
  for( int axis=0; axis<3; axis++ ) for( int side=0; side<=1; side++ ) ngid(side,axis)=0;

  int minGhost=INT_MAX, maxGhost=-1;
  for( int axis=0; axis<domainDimension; axis++ )
  {
    ngid(0,axis)=gridIndexRange(0,axis)-dimension(0,axis);
    ngid(1,axis)=dimension(1,axis)-gridIndexRange(1,axis);

    minGhost=min(minGhost,ngid(0,axis),ngid(1,axis));
    maxGhost=max(maxGhost,ngid(0,axis),ngid(1,axis));
  }

  for( int axis=0; axis<domainDimension; axis++ )
  {
    setGridDimensions(axis,gridIndexRange(1,axis)-gridIndexRange(0,axis)+1);
  }
  
//     printF("DPM:setDataPoints: num-ghost-in-data: ngid=[%i,%i][%i,%i][%i,%i]\n",ngid(0,0),ngid(1,0),
// 	   ngid(0,1),ngid(1,1),ngid(0,2),ngid(1,2));
  

  // *** NOTE: WE MAY NEED TO INCREASE THE NUMBER OF GHOST FROM THOSE SUPPLIED **** -- see comments in above routine
  // *** FIX ME ***

  const int defaultNumberOfGhostLines=2; 

  IndexRangeType numberOfGhostLinesOld; // *wdh* 2012/03/21 -- increase ghost lines to match ngid
  numberOfGhostLinesOld=0;
  for( int axis=axis1; axis<domainDimension; axis++ )
  {
    numberOfGhostLinesOld(0,axis)=ngid(0,axis); 
    numberOfGhostLinesOld(1,axis)=ngid(1,axis);
    
    // --- make sure there are at least defaultNumberOfGhostLines ---
    int nga = max(defaultNumberOfGhostLines,ngid(Start,axis));
    int ngb = max(defaultNumberOfGhostLines,ngid(End  ,axis));
    if( orderOfInterpolation==2 )
    {
      // When the orderOfInterpolation==2 the last ghost line is not used in evaluating the mapping (so that the
      //   derivative of the mapping (which does use the last line) is consistent. (I think)
      // When the user provides 2 or more ghost lines of data we add an additional ghost line --
      // this is used for grids from hype that specify a large boundary offset. (we don't always do this
      // for backward compatibility)
      if( ngid(Start,axis)>=2 )
	nga = max(defaultNumberOfGhostLines,ngid(Start,axis)+1);
      if( ngid(End  ,axis)>=2 )
        ngb = max(defaultNumberOfGhostLines,ngid(End  ,axis)+1);
    }
    dimension(Start,axis)=gridIndexRange(Start,axis)-nga;  
    dimension(End  ,axis)=gridIndexRange(End  ,axis)+ngb;

    delta[axis]=gridIndexRange(End,axis)-gridIndexRange(Start,axis);
    deltaByTwo[axis]=.5*max(1.,delta[axis]);
  }

  if( true )
  {
    printF("DPM:setDataPoints: domainDimension=%i, rangeDimension=%i\n",domainDimension,rangeDimension);
    printF("DPM:setDataPoints: gridIndexRange=[%i,%i][%i,%i][%i,%i]\n",gridIndexRange(0,0),gridIndexRange(1,0),
 	   gridIndexRange(0,1),gridIndexRange(1,1),gridIndexRange(0,2),gridIndexRange(1,2));
    printF("DPM:setDataPoints: dimension=[%i,%i][%i,%i][%i,%i]\n",dimension(0,0),dimension(1,0),
 	   dimension(0,1),dimension(1,1),dimension(0,2),dimension(1,2));
  }
  
  if( evalAsNurbs )
  {
    // -- If we evaluate as a Nurbs there is no need to save the grid points
    //    in the "xy" array -- we can just generate the NurbsMapping directly 

    // **** NOTE*** THIS MAY NOT BE CONSISTENT WITH OLD WAY SINCE WE DO NOT GENERATE EXTRA GHOST 


    xy.redim(0);

    if( !dbase.has_key("nurbs") )
    {
      dbase.put<NurbsMapping*>("nurbs")=new NurbsMapping;
    }
    NurbsMapping & nurbs = *dbase.get<NurbsMapping*>("nurbs");
    
    // printF("--DPM-- setDataPoints -- minGhost=%i, maxGhost=%i\n",minGhost,maxGhost);

    // -- what about extra ghost ??
    // here is the new interpolate: 
    int xDegree[3] ={ nurbsDegree,nurbsDegree,nurbsDegree };  // 
    nurbs.interpolate(x,domainDimension,rangeDimension,xDimension,xGridIndexRange,
		      NurbsMapping::parameterizeByIndex,xDegree);
    
    reinitialize();            // we have to re-initialize the inverse -- is this needed with nurbs?
    mappingInitialized=true;
    mappingHasChanged();

    nurbsOutOfDate=false;
  }
  else
  {
    // --- Create the xy array ---


    // save points in the local array
    initializePartition();
    xy.partition(partition);
    xy.redim(Range(dimension(Start,0),dimension(End,0)),
	     Range(dimension(Start,1),dimension(End,1)),
	     Range(dimension(Start,2),dimension(End,2)),
	     rangeDimension);

    OV_GET_SERIAL_ARRAY(real,xy,xyLocal);
  
    xyLocal=0.; // to avoid UMRs when extrapolating ghost points.
  
    // --- copy all points from input array "x" ---
    Index I1,I2,I3, R(0,rangeDimension);
    ::getIndex(xDimension,I1,I2,I3);
    bool ok=ParallelUtility::getLocalArrayBounds(xy,xyLocal, I1,I2,I3, 1);//include parallel ghost
    if(ok)
    {
      xyLocal(I1,I2,I3,R)=x(I1,I2,I3,R);
    }
  
    if( false )
    {
      ::display(x,"--DPM-- x","%5.2f ");
      ::display(xyLocal,"--DPM-- xyLocal","%5.2f ");
    }
  
    // --- set bounds on the range ---
    ::getIndex(gridIndexRange,I1,I2,I3);
    ok=ParallelUtility::getLocalArrayBounds(xy,xyLocal, I1,I2,I3,0 ); // no parallel ghost
    real bigValue=.1*REAL_MAX;
    real xMin[3]={bigValue,bigValue,bigValue}, xMax[3]={-bigValue,-bigValue,-bigValue};
    if( ok )
    {
      for( int axis=0; axis<rangeDimension; axis++ )
      {
	xMin[axis]=min(xyLocal(I1,I2,I3,axis));
	xMax[axis]=max(xyLocal(I1,I2,I3,axis));
      }
    }
  
    ParallelUtility::getMinValues(xMin,xMin,rangeDimension);
    ParallelUtility::getMaxValues(xMax,xMax,rangeDimension);
  
    for( int axis=0; axis<rangeDimension; axis++ )
    {   
      setRangeBound(Start,axis,xMin[axis]);
      setRangeBound(End  ,axis,xMax[axis]);
    }

    //   Determine values at ghost points that have not been user set:  extrapolate or use periodicity
    // computeGhostPoints( ngid,numberOfGhostPoints );
    computeGhostPoints( numberOfGhostLinesOld,numberOfGhostPoints );

    if( true )
      xy.updateGhostBoundaries();

    if( debug & 8 )
    {
      ::display(xy,"--DPM-- xy","%7.4f ");
    }
  

    reinitialize();  // we have to re-initialize the inverse
   
    mappingInitialized=true;
    mappingHasChanged();

  } // end create the xy array

  
  return 0;
}



int DataPointMapping::
computeGhostPoints( IndexRangeType & numberOfGhostLinesOld, 
                    IndexRangeType & numberOfGhostLinesNew )
// =================================================================================================
/// \param Access Level: protected
/// \details 
///    Determine values at ghost points that have not been user set:  extrapolate or use periodicity
///    Ghost lines on sides with boundaryCondition>0 are extrapolated with a stretchingFactor (see below)
///    so that the grid lines get
///    further apart. This is useful for highly stretched grids so that the ghost points move away from
///    the boundary.
// ===========================================================================================
{
  // printF("\n CCCCCCCCCCCC DataPointMapping::computeGhostPoints CCCCCCC\n");

  bool numberOfGhostLinesHasChanged=FALSE;
  int side,axis;
  for( axis=0; axis<domainDimension; axis++ )
  {
    for( side=0; side<=1; side++ )
    {
      if( numberOfGhostLinesNew(side,axis)!= numberOfGhostLinesOld(side,axis) )
	numberOfGhostLinesHasChanged=true;
    }
  }
  if( !numberOfGhostLinesHasChanged )
    return 0;

  // *wdh* 080419 -- added xyLocal following pmb
#ifdef USE_PPP
  realSerialArray xyLocal;
  getLocalArrayWithGhostBoundaries(xy, xyLocal);
#else
  realSerialArray & xyLocal = xy;
#endif

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];
  Index Ipv[3], &Ip1=Ipv[0], &Ip2=Ipv[1], &Ip3=Ipv[2];
  Range Rx=rangeDimension;
  int is[3],ip[3];
  ForBoundary(side,axis)
  {
    int dir;
    for( dir=0; dir<3; dir++ )
    {
      is[dir] = dir!=axis ? 0 : ( side==Start ? +1 : -1 );
      ip[dir] = (gridIndexRange(End,dir)-gridIndexRange(Start,dir))*is[dir];
    }
    // only need to assign ghostline that are new.
    // printF("DPM:computeGhost: (side,axis)=(%i,%i) numberOfGhostLinesOld=%i numberOfGhostLinesNew=%i\n",
    // 	   side,axis,numberOfGhostLinesOld(side,axis),numberOfGhostLinesNew(side,axis));
    
    // We assign all ghost lines that appear in the xy *wdh* 2012/04/13
    int lastGhost = side==0 ? gridIndexRange(side,axis)-xy.getBase(axis) : xy.getBound(axis)-gridIndexRange(side,axis);
    for( int ghost=1+numberOfGhostLinesOld(side,axis); ghost<=lastGhost; ghost++ )
    // for( int ghost=1+numberOfGhostLinesOld(side,axis); ghost<=numberOfGhostLinesNew(side,axis); ghost++ )
    {
      getGhostIndex(gridIndexRange,side,axis,I1,I2,I3,ghost);      // line x(-g) : g'th line outside
      getGhostIndex(gridIndexRange,side,axis,Ib1,Ib2,Ib3,0);       // line x(0)  : boundary
      getGhostIndex(gridIndexRange,side,axis,Ip1,Ip2,Ip3,-ghost);  // line x(g)  : g'th line inside

      // *wdh* 070427 -- make sure that Ipv[axis] is within bounds for grids with few grid lines
      if( (side == 0 && Ipv[axis].getBase() > xy.getBound(axis) ) ||
          (side == 1 && Ipv[axis].getBase() < xy.getBase(axis) ) )
      {
	Ipv[axis] = side==0 ? xy.getBound(axis) : xy.getBase(axis);
      }
      
      for( dir=0; dir<domainDimension; dir++ )
      { // include ghost lines in tangential direction.
	if( dir!=axis )
	{
          // Assign all ghost points in the tangential direction *wdh* 2012/04/13 
          Iv[dir]=xy.dimension(dir); 
	  // Iv[dir]=Range(Iv[dir].getBase() -numberOfGhostLinesNew(0,dir),  // old
	  //		Iv[dir].getBound()+numberOfGhostLinesNew(1,dir));
          Ibv[dir]=Iv[dir];
          Ipv[dir]=Iv[dir];
	}
      }
      bool ok = ParallelUtility::getLocalArrayBounds(xy, xyLocal, I1,I2,I3, 1);//include parallel ghost

      if(ok)
      {
        ok = ParallelUtility::getLocalArrayBounds(xy, xyLocal, Ib1,Ib2,Ib3, 1); assert( ok );
        ok = ParallelUtility::getLocalArrayBounds(xy, xyLocal, Ip1,Ip2,Ip3, 1); assert( ok );

	if( getIsPeriodic(axis)==functionPeriodic )
	{
	  xyLocal(I1,I2,I3,Rx)=xyLocal(I1+ip[0],I2+ip[1],I3+ip[2],Rx);
	}
	else if( getIsPeriodic(axis)==derivativePeriodic )
	{
	  // *wdh* 050818 -- add a shift for derivative periodic
	  // printf(" DPM: assign ghost values for deriv periodic, side,axis,ghost=%i,%i,%i\n",side,axis,ghost);
	
	  xyLocal(I1,I2,I3,Rx)=xyLocal(I1+ip[0],I2+ip[1],I3+ip[2],Rx) +
	    (xyLocal(Ib1,Ib2,Ib3,Rx) - xyLocal(Ib1+ip[0],Ib2+ip[1],Ib3+ip[2],Rx));
	
	}
	else 
	{
	  // if( true ) // seems to be needed by filletTwoCyl
	  if( false &&  // *wdh* 040502 this leads to poor derivatives near the boundary!! as pointed out by Petri.
	      ghost>1 ) // *wdh* turn off for tdpm test 
	  {
	    // x(-1) = x(0) + stretchFactor*(x(0)-x(1))
	    const real stretchFactor= getBoundaryCondition(side,axis)>0 ? 1.4 : 1.;
	    const real alpha=1.+stretchFactor, beta=-stretchFactor;
	  
	    xyLocal(I1,I2,I3,Rx)=alpha*xyLocal(I1+is[0],I2+is[1],I3+is[2],Rx)+beta*xyLocal(I1+2*is[0],I2+2*is[1],I3+2*is[2],Rx);
	  }
	  else
	  {
	    // *wdh* 000926
	    // extrapolate in a different way that work better on stretched grids so that the grid is reflected.
	    // x(-g) = x(0) + (x(0)-x(g))
	    xyLocal(I1,I2,I3,Rx)=2.*xyLocal(Ib1,Ib2,Ib3,Rx)-xyLocal(Ip1,Ip2,Ip3,Rx);
	    // if( true )
	    // {
	    //   printF("DPM:computeGhost ghost=%i (side,axis)=(%i,%i) [%i,%i][%i,%i][%i,%i]\n",ghost,side,axis,
            //          I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
	    //   // ::display(xyLocal(I1,I2,I3,Rx),"x","%5.2f ");
	    // }
	    

	  }
	
	}
      } // end if ok
      
    }
  }

  for( axis=0; axis<domainDimension; axis++ )
  {
    for( side=0; side<=1; side++ )
    {
      numberOfGhostPoints(side,axis)=numberOfGhostLinesNew(side,axis);  
    }
  }

  mappingHasChanged();
  
  return 0;
}




int DataPointMapping::
setNumberOfGhostLines( IndexRangeType & numberOfGhostLinesNew )
// ===========================================================================================
/// \details 
///     Specify the number of ghost lines.
/// \param numberOfGhostLinesNew(side,axis) : specify the number of ghostlines.
// ===========================================================================================
{
  printF("\n WWWWWWWWWW DataPointMapping::setNumberOfGhostLines WWWW\n");

  bool numberOfGhostLinesHasChanged=FALSE;
  int side,axis;
  for( axis=0; axis<domainDimension; axis++ )
  {
    for( side=0; side<=1; side++ )
    {
      if( numberOfGhostLinesNew(side,axis)!= numberOfGhostPoints(side,axis) )
	numberOfGhostLinesHasChanged=TRUE;
    }
  }
  if( !mappingInitialized && numberOfGhostLinesHasChanged )
  {
    for( axis=0; axis<domainDimension; axis++ )
    {
      for( side=0; side<=1; side++ )
      {
        numberOfGhostPoints(side,axis)=numberOfGhostLinesNew(side,axis);
      }
    }
    mappingHasChanged();
  }
  else if( mappingInitialized && numberOfGhostLinesHasChanged )
  {
    // compute new ghost lines
    if( Mapping::debug & 2 )
      printf("DataPointMapping::recompute ghost lines since the number has changed.\n");
    

    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
    for( axis=domainDimension; axis<3; axis++ )
    {
      dimension(Start,axis)=gridIndexRange(Start,axis);
      dimension(End  ,axis)=gridIndexRange(End  ,axis);
    }
    ::getIndex(dimension,I1,I2,I3);  // old bounds

    for( axis=axis1; axis<domainDimension; axis++ )
    {
      dimension(Start,axis)=gridIndexRange(Start,axis)-numberOfGhostLinesNew(Start,axis);
      dimension(End  ,axis)=gridIndexRange(End  ,axis)+numberOfGhostLinesNew(End,axis);
    }

    ::getIndex(dimension,J1,J2,J3); // new bounds
    realArray xyz(J1,J2,J3,rangeDimension);
    xyz=0; // to avoid UMR's
    
    Range Rx=rangeDimension;
    for( axis=0; axis<3; axis++ )
      Iv[axis]=Range(max(Iv[axis].getBase(),Jv[axis].getBase()),min(Iv[axis].getBound(),Jv[axis].getBound()));
    
    xyz(I1,I2,I3,Rx)=xy(I1,I2,I3,Rx);

    xy.redim(0);
    xy=xyz;
    xyz.redim(0);

    //   Determine values at ghost points that have not been user set:  extrapolate or use periodicity
    computeGhostPoints( numberOfGhostPoints,numberOfGhostLinesNew );
    

//     int is[3],ip[3];
//     ForBoundary(side,axis)
//     {
//       int dir;
//       for( dir=0; dir<3; dir++ )
//       {
// 	is[dir] = dir!=axis ? 0 : ( side==Start ? +1 : -1 );
// 	ip[dir] = (gridIndexRange(End,dir)-gridIndexRange(Start,dir))*is[dir];
//       }
//       // only need to assign ghostline that are new.
//       for( int ghost=1+numberOfGhostLines[side][axis]; ghost<=numberOfGhostLines_[side][axis]; ghost++ )
//       {
// 	// getGhostIndex(gridIndexRange,side,axis,I1,I2,I3,ghost,numberOfGhostLines_[side][axis]);
// 	getGhostIndex(gridIndexRange,side,axis,I1,I2,I3,ghost);
// 	for( dir=0; dir<domainDimension; dir++ )
// 	{ // include ghost lines in tangential direction.
//           if( dir!=axis )
// 	    Iv[dir]=Range(Iv[dir].getBase() -numberOfGhostLines_[0][dir],
//                           Iv[dir].getBound()+numberOfGhostLines_[1][dir]);
// 	}
// 	if( getIsPeriodic(axis)==functionPeriodic )
// 	  xy(I1,I2,I3,Rx)=xy(I1+ip[0],I2+ip[1],I3+ip[2],Rx);
// 	else 
// 	{
//           // x(-1) = x(0) + stretchFactor*(x(0)-x(1))
//           const real stretchFactor=1.4, alpha=1.+stretchFactor, beta=-stretchFactor;
	  
// 	  xy(I1,I2,I3,Rx)=alpha*xy(I1+is[0],I2+is[1],I3+is[2],Rx)+beta*xy(I1+2*is[0],I2+2*is[1],I3+2*is[2],Rx);
// 	  // extrapolate in a different way that work better on stretched grids so that the grid is reflected.

// /* ------
// 	  const int m=ghost, m2=2*ghost;
//           printf("setNumberOfGhostLines: m=%i, m2=%i, I1=(%i,%i)\n",m,m2,I1.getBase(),I1.getBound());
	  
// 	  xy(I1,I2,I3,Rx)=2.*xy(I1+m*is[0],I2+m*is[1],I3+m*is[2],Rx)-xy(I1+m2*is[0],I2+m2*is[1],I3+m2*is[2],Rx);
	
//           ::display(xy(I1,I2,I3,Rx),"xy at ghost");
// ------ */

// 	}
      
//       }
//     }

    
  }
  
//   for( axis=0; axis<domainDimension; axis++ )
//   {
//     for( side=0; side<=1; side++ )
//     {
//       if( numberOfGhostLines_[side][axis]!= numberOfGhostLines[side][axis] )
// 	numberOfGhostLinesHasChanged=TRUE;
//       numberOfGhostLines[side][axis]=numberOfGhostLines_[side][axis];
//     }
//   }
//   mappingHasChanged();
  
  return 0;
}


int DataPointMapping::
projectGhostPoints(MappingInformation & mapInfo )
//===========================================================================
/// \details 
///    Project the ghost points on physical boundaries onto the closest mapping
///   found in a list of Mapping's
/// 
/// \param mapInfo (input):  Project onto the closest mapping found in mapInfo.mappingList.
/// 
// =========================================================================
{
  if( domainDimension!=2 && domainDimension!=3 )
  {  
    printf("DataPointMapping::projectGhostPoints:ERROR: can only project a Mapping with domainDimenion = 2 or 3\n");
    return -1;
  }
  
  const int numberOfMappings=mapInfo.mappingList.getLength();

  if( !mappingInitialized || numberOfMappings<=0 )
  {
    if( numberOfMappings<=0 )
      printf("DataPointMapping::projectGhostPoints:WARNING: There are no Mapping's in the list to project onto\n");
    return -1;
  }

  int numberProjected=0;
  for( int axis=0; axis<domainDimension; axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      const int shareFlag = share[side][axis];
      if( getBoundaryCondition(side,axis) > 0 &&  shareFlag!=0 )
      {
	
	Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
	Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
	Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];

	getBoundaryIndex(dimension,side,axis,Ib1,Ib2,Ib3);  
	Range Rx=getRangeDimension();

	for( int dir=0; dir<domainDimension; dir++ )
	{
	  I1=Ib1; I2=Ib2; I3=Ib3;
	  if( dir!=axis )
	  {
	    Iv[axis]=gridIndexRange(side,axis);
	    for( int side2=Start; side2<=End; side2++ )
	    {
	      Iv[dir]=side2==0 ? 
		Range(dimension(0,dir),gridIndexRange(0,dir)-1) :     // all ghost points
		Range(gridIndexRange(1,dir)+1,dimension(1,dir));


	      int num=I1.getLength()*I2.getLength()*I3.getLength();
              if( num<=0 )
                continue;
	      Range R=num;
	      realArray x(I1,I2,I3,Rx),r(R,Rx),xNew(R,Rx),drOpt(R);

              // drMax: only project points if |dr(normal direction)| < drMax. We allow quite a big value to 
              //        allow for highly stretched grids.
              const real drMax=1.;
              drOpt=drMax;  // holds dr for the best correction so far
	      x(I1,I2,I3,Rx)=xy(I1,I2,I3,Rx); 
	      x.reshape(R,Rx);
              xNew=x;

	      // try to project the points x(I1,I2,I3)

	      // **** should probably project onto the same Mapping that the the boundary points lie on *****

	      for( int m=0; m<numberOfMappings; m++ )
	      {
		Mapping & map = mapInfo.mappingList[m].getMapping();
		if( &map == this || 
                    map.getRangeDimension()!=rangeDimension ||
                    map.getDomainDimension()!=domainDimension )
		  continue;
	      
		for( int dirm=0; dirm<map.getDomainDimension(); dirm++ )
		{
		  for( int sidem=Start; sidem<=End; sidem++ )
		  {
		    if( map.getShare(sidem,dirm)==shareFlag && map.getBoundaryCondition(sidem,dirm)>0 &&
                        intersects( map, side,axis,sidem,dirm,.1 ) )
		    {
		      // face (side,axis) of "this" shares a face and intersects with (sidem,dirm) of map
                      if( TRUE || debug & 4 )
  		        printf(" (side,axis,share)=(%i,%i,%i) (side2,dir)=(%i,%i) shares with "
                           "(sidem,dirm,map)=(%i,%i,%s)\n",
			       side,axis,shareFlag,side2,dir,sidem,dirm,(const char*)map.getName(mappingName));

		      r=-1.;
		      map.inverseMap(x,r);
	              const int dirmp1 = (dirm+1) % map.getDomainDimension(); // tangential direction
	              const int dirmp2 = (dirm+2) % map.getDomainDimension(); // tangential direction
   		      intArray mask;
                      if( domainDimension==2 )
  		        mask=  fabs(r(R,dirmp1)-.5)<=.5;  // inside in the tangential direction
                      else
                        mask=  fabs(r(R,dirmp1)-.5)<=.5 && fabs(r(R,dirmp2)-.5)<=.5;
		      
		      int numOk=sum(mask);
		      if( numOk > 0 ) 
		      {
			// ::display(x,"Here are ghost points x");
			// ::display(r,"Here is r");
		    
			// inverse should be inside in the tangential directions

		        // choose the best possible answer from the list of mappings.
                
		        // check the normal

		        // project onto the boundary of "map"
                        realArray rp(R,Rx),xp(R,Rx);
                        rp=r;
			rp(R,dirm)=(real)sidem;
			map.mapGrid(rp,xp);
                        if( debug & 4 )
			{
			  for( int i=0; i<num; i++ )
			  {
			    if( mask(i) )
			    {
			      printf(" i=%i, point x=(%8.2e,%8.2e) projected to x=(%8.2e,%8.2e)"
				     " (r=(%8.2e,%8.2e)->rp=(%8.2e,%8.2e))\n",
				     i,x(i,0),x(i,1),xp(i,0),xp(i,1),r(i,0),r(i,1),rp(i,0),rp(i,1));
			    }
			  }
			}
			
			where( mask && fabs(r(R,dirm)-rp(R,dirm))< drOpt )
			{
			  drOpt=fabs(r(R,dirm)-rp(R,dirm));
			  for( int ax=0; ax<rangeDimension; ax++ )
			    xNew(R,ax)=xp(R,ax);
			}
		      
		      }
                      
		      // xy(I1,I2,I3,Rx)=x(I1,I2,I3,Rx);
		      
		    } // if share
		  } // for sidem
		} // for dirm

	      } // for m
              numberProjected+=sum(drOpt<drMax);
              if( debug & 4 )
	      {
		printf("Points projected on (side,axis,name)=(%i,%i,%s) for edge (side2,dir)=(%i,%i)\n",
		       side,axis,(const char*)getName(mappingName),side2,dir);
		for( int i=0; i<num; i++ )
		{
		  if( drOpt(i)<drMax )
		  {
		    printf(" *final* i=%i, point x=(%8.2e,%8.2e) projected to x=(%8.2e,%8.2e) (drOpt=%8.2e)\n",
			   i,x(i,0),x(i,1),xNew(i,0),xNew(i,1),drOpt(i));
		  }
		}
	      }
	      
	      xNew-=x;  // xNew= dx
              xNew.reshape(I1,I2,I3,Rx);
              // xy(I1,I2,I3,Rx)+=xNew(I1,I2,I3,Rx);
	      
              // now adjust all points in the normal direction.
              // adjust points off the boundary by a smaller amount: use a factor delta
              // We should be careful that the grid points don't overlap each other, this might
              // happen if dx is larger than the grid spacing and we change delta too rapidly
              J1=I1, J2=I2, J3=I3;
              const real dr0=1./max(1,gridIndexRange(End,axis)-gridIndexRange(Start,axis));
	      for( int i=dimension(Start,axis); i<=dimension(End,axis); i++ )
	      {
		Jv[axis]=i;
		real deltar = (i-gridIndexRange(side,axis))*dr0; 
		deltar = 1.- SQR(deltar);                        // delta=1 on the bndry, 0 on opposite bndry
                deltar =max(0.,min(1.,deltar));
                xy(J1,J2,J3,Rx)+=deltar*xNew;
              }
	      
	    } // for side2
	  }
	} // for dir

      } // if bc > 0
    }  // end for side 
  } // end for axis
  
  if( numberProjected )
  {
    printf("DataPointMapping::projectGhostPoints: number of pts projected=%i\n",numberProjected);
    mappingHasChanged();
  }

  return 0;
}



/* -----
int DataPointMapping::
setDataPoints( const aString & fileName )
//===========================================================================
/// \details 
///    Assign the  data points from a file of data. By default this routine will
///     attempt to automaticall determine the format of the file.
/// \param fileName (input) : name of an existing file of data (such as a plot3d file)
///  
//===========================================================================
{
  mappingHasChanged();
  return 0;
}
---- */

int DataPointMapping::
setMapping( Mapping & map )             
//===========================================================================
/// \details 
///     Build a data point mapping from grids points obtained by evaluating a 
///   mapping. 
/// \param map (input) : Mapping to get data points from.
///  
//===========================================================================
{
  setDomainDimension(map.getDomainDimension());
  setRangeDimension(map.getRangeDimension());

  if( domainDimension==2 || domainDimension==3 )
    setBasicInverseOption(canInvertWithGoodGuess);  // basicInverse is available
  else
    setBasicInverseOption(canDoNothing);

  for( int axis=0; axis<3; axis++ )for( int side=0; side<=1; side++ ) 
    Mapping::setGridIndexRange(side,axis,0);
  for( int axis=axis1; axis<domainDimension; axis++ )
  {
    // keep the default number of ghost points for the DPM

    setIsPeriodic(axis,map.getIsPeriodic(axis));
    for( int side=Start; side<=End; side++ )
    {
      setBoundaryCondition(side,axis,map.getBoundaryCondition(side,axis));
      setShare(side,axis,map.getShare(side,axis));
      setTypeOfCoordinateSingularity( side,axis,map.getTypeOfCoordinateSingularity(side,axis) );
    }
  }

  // evaulate the mapping, including ghostlines

  // Assign the gridIndexRange and dimension arrays:
  dimension.redim(2,3); dimension=0;
  gridIndexRange.redim(2,3); gridIndexRange=0;
  delta[0]=delta[1]=delta[2]=1.;
  deltaByTwo[0]=deltaByTwo[1]=deltaByTwo[2]=.5;
  for( int axis=axis1; axis<domainDimension; axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      Mapping::setGridIndexRange(side,axis, map.getGridIndexRange(side,axis));
      gridIndexRange(side,axis)=map.getGridIndexRange(side,axis);
    }
    
    dimension(Start,axis)= gridIndexRange(0,axis)-numberOfGhostPoints(Start,axis);
    dimension(End  ,axis)= gridIndexRange(1,axis)+numberOfGhostPoints(End,axis);

    delta[axis]=gridIndexRange(End,axis)-gridIndexRange(Start,axis);
    deltaByTwo[axis]=.5*max(1.,delta[axis]);
  }
 
//   printf("\n WWW dpm:setMapping gridIndexRange=[%i,%i][%i,%i][%i,%i] dimension=[%i,%i][%i,%i][%i,%i]\n",
//          gridIndexRange(0,0),gridIndexRange(1,0),
//          gridIndexRange(0,1),gridIndexRange(1,1),
//          gridIndexRange(0,2),gridIndexRange(1,2),
//          dimension(0,0),dimension(1,0),
//          dimension(0,1),dimension(1,1),
//          dimension(0,2),dimension(1,2));

  Range R1(dimension(Start,axis1),dimension(End,axis1));
  Range R2(dimension(Start,axis2),dimension(End,axis2));
  Range R3(dimension(Start,axis3),dimension(End,axis3));

  // The number of parallel ghost lines should be at least two for fourth-order interpolation
  initializePartition();
  xy.partition(partition);
  xy.redim(R1,R2,R3,rangeDimension);

  real dr[3];
  for( int axis=0; axis<3; axis++ )
    dr[axis]=1./max(gridIndexRange(1,axis)-gridIndexRange(0,axis),1);

  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  J1=R1; J2=R2; J3=R3;

  #ifdef USE_PPP
    realSerialArray xyLocal; getLocalArrayWithGhostBoundaries(xy,xyLocal); 
    bool ok = ParallelUtility::getLocalArrayBounds(xy,xyLocal,J1,J2,J3,1); // include parallel ghost
    realSerialArray r(xyLocal.dimension(0),xyLocal.dimension(1),xyLocal.dimension(2),domainDimension);
    r=0.;  // avoid UMR's in valgrind
  #else
    realSerialArray r(J1,J2,J3,domainDimension);
    realSerialArray & xyLocal = xy;
  #endif
  for( int axis=0; axis<domainDimension; axis++ ) 
  {
    Index Jaxis = Jv[axis];
    for( int k=r.getBase(axis); k<=r.getBound(axis); k++ ) 
    {
      Jv[axis] = k;
      real rval = dr[axis]*(k - gridIndexRange(0,axis));
      // Make sure the last point gets parameter value 1 (roundoff can affect this)
      if( k==gridIndexRange(1,axis) ) rval=1.;
      // evaluate function-periodic points for r>=1 at the periodic values so that
      // these values will be the same (?)
      if( rval>=1. && map.getIsPeriodic(axis)==Mapping::functionPeriodic )
	rval = dr[axis]*(k - gridIndexRange(1,axis));
	 
      r(J1,J2,J3,axis) = rval;
    } // end for
    Jv[axis] = Jaxis; // reset
  } // end for

  #ifdef USE_PPP  
    map.mapGridS(r,xyLocal);
  #else
    map.mapGrid(r,xyLocal);
  #endif

//   // for testing try this to be backward compatible:
//   int ngid[2][3] = {0,0,0,0,0,0};
//   computeGhostPoints( ngid,numberOfGhostLines );

  reinitialize();  
  mappingInitialized=true;
  mappingHasChanged();

  setGrid( xy,gridIndexRange ); // supply the grid 

  signForJacobian=map.getSignForJacobian();

  return 0;
}

/* -----------
int DataPointMapping::
setMapping( Mapping & map )             
//===========================================================================
/// \details 
///     Build a data point mapping from grids points obtained by evaluating a 
///   mapping. 
/// \param map (input) : Mapping to get data points from.
///  
//===========================================================================
{
  setDomainDimension(map.getDomainDimension());
  setRangeDimension(map.getRangeDimension());

  // evaulate the mapping, including ghostlines

  // first assign the indexRange and dimension arrays:
  IntegerArray dim(2,3);
  dim=0;
  int axis;
  for( axis=axis1; axis<domainDimension; axis++ )
  {
    dim(Start,axis)= 0-numberOfGhostLines[Start][axis];
    dim(End  ,axis)= map.getGridDimensions(axis)-1+numberOfGhostLines[End][axis];
  }
 
  Range R1(dim(Start,axis1),dim(End,axis1));
  Range R2(dim(Start,axis2),dim(End,axis2));
  Range R3(dim(Start,axis3),dim(End,axis3));

  realArray gridPoints( R1,R2,R3,rangeDimension);
  realArray r(R1,R2,R3,domainDimension);

  real deltaR[3]={1.,1.,1.};
  for( axis=axis1; axis<domainDimension; axis++ )
    deltaR[axis]=1./max(map.getGridDimensions(axis)-1,1);

  int i1,i2,i3;
  for( i1=gridPoints.getBase(axis1); i1<=gridPoints.getBound(axis1); i1++ )
    r(i1,R2,R3,0)=i1*deltaR[axis1];
  if( domainDimension>1 )
  {
    for( i2=gridPoints.getBase(axis2); i2<=gridPoints.getBound(axis2); i2++ )
      r(R1,i2,R3,1)=i2*deltaR[axis2];
  }
  if( domainDimension>2 )
  {
    for( i3=gridPoints.getBase(axis3); i3<=gridPoints.getBound(axis3); i3++ )
      r(R1,R2,i3,2)=i3*deltaR[axis3];
  }

// *wdh* 030918* replaced by above code which is faster
//    for( i3=gridPoints.getBase(axis3); i3<=gridPoints.getBound(axis3); i3++ )
//    {
//      for( i2=gridPoints.getBase(axis2); i2<=gridPoints.getBound(axis2); i2++ )
//        r(R1,i2,i3,0).seqAdd(-numberOfGhostLines[Start][axis1]*deltaR[axis1],deltaR[axis1]);
//      if( domainDimension>1 )
//      {
//        for( i1=gridPoints.getBase(axis1); i1<=gridPoints.getBound(axis1); i1++ )
//  	r(i1,R2,i3,1).seqAdd(-numberOfGhostLines[Start][axis2]*deltaR[axis2],deltaR[axis2]);
//      }
//    }
//    if( domainDimension>2 )
//    {
//      for( i2=gridPoints.getBase(axis2); i2<=gridPoints.getBound(axis2); i2++ )
//        for( i1=gridPoints.getBase(axis1); i1<=gridPoints.getBound(axis1); i1++ )
//  	r(i1,i2,R3,2).seqAdd(-numberOfGhostLines[Start][axis3]*deltaR[axis3),deltaR[axis3]);
//    }

  map.mapGrid(r,gridPoints);

  setDataPoints(gridPoints,3,map.getDomainDimension(),numberOfGhostLines);
  for( axis=axis1; axis<domainDimension; axis++ )
  {
    setIsPeriodic(axis,map.getIsPeriodic(axis));
    for( int side=Start; side<=End; side++ )
    {
      setBoundaryCondition(side,axis,map.getBoundaryCondition(side,axis));
      setShare(side,axis,map.getShare(side,axis));
      setTypeOfCoordinateSingularity( side,axis,map.getTypeOfCoordinateSingularity(side,axis) );
    }
  }

  signForJacobian=map.getSignForJacobian();

  return 0;
}
---------- */

void DataPointMapping::
setOrderOfInterpolation( const int order )
//===========================================================================
/// \brief  
///    Set the order of interpolation, 2 or 4.
/// \param order (input) : A value of 2 or 4.
//===========================================================================
{
  if( order!=2 && order!=4 )
  {
    printf("DataPointMapping::setOrderOfInterpolation:ERROR: not implemented for orderOfInterpoaltion=%i\n",
	   orderOfInterpolation);
    return;
  }
  
  orderOfInterpolation=order;

  if( orderOfInterpolation==2 && (
      (domainDimension==2 && rangeDimension==2) || (domainDimension==3 && rangeDimension==3)) )
    setBasicInverseOption(CANINVERT);  // basicInverse is available
  else
    setBasicInverseOption(canDoNothing);
}

int DataPointMapping::
getOrderOfInterpolation()
//===========================================================================
/// \brief  
///    Get the order of interpolation.
/// \return  The order of interpolation.
//===========================================================================
{
  return orderOfInterpolation;
}

void  DataPointMapping::
useScalarArrayIndexing(const bool & trueOrFalse /* =FALSE */) 
//===========================================================================
/// \brief  
///     Turn on or off the use of scalar indexing. Scalar indexing for array
///  operations can be faster when the length of arrays are smaller.
/// \param trueOrFalse (input) : TRUE means turn on scalra indexing.
//===========================================================================
{
  useScalarIndexing=trueOrFalse;
}



DataPointMapping & DataPointMapping::
operator =( const DataPointMapping & X )
{
  if( DataPointMapping::className != X.getClassName() )
  {
    cout << "DataPointMapping::operator= ERROR trying to set a DataPointMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class

  useScalarIndexing=X.useScalarIndexing;  // *wdh* 030918
  orderOfInterpolation=X.orderOfInterpolation;

  // Is this what we should do?
  xy.redim(0); 
  if( X.xy.elementCount()>0 )
  {
    xy.partition(X.xy.getPartition()); xy.redim(X.xy);
    // xy=X.xy;
    // *wdh* 2014/08/18 -- copy local arrays
    OV_GET_SERIAL_ARRAY(real,xy,xyLocal);
    OV_GET_SERIAL_ARRAY_CONST(real,X.xy,XxyLocal);
    
    xyLocal=XxyLocal;
  }
  
  dimension=X.dimension;
  gridIndexRange=X.gridIndexRange;

  delta[0]=X.delta[0]; delta[1]=X.delta[1]; delta[2]=X.delta[2];
  deltaByTwo[0]=X.deltaByTwo[0]; deltaByTwo[1]=X.deltaByTwo[1]; deltaByTwo[2]=X.deltaByTwo[2];

  mapIsDistributed=X.mapIsDistributed;
  inverseIsDistributed=X.inverseIsDistributed;
  evalAsNurbs=X.evalAsNurbs;
  nurbsDegree=X.nurbsDegree;
  nurbsOutOfDate=X.nurbsOutOfDate;
 
  bool nurbsExists = X.dbase.has_key("nurbs");
  if( nurbsExists &&  X.dbase.get<NurbsMapping*>("nurbs")!=NULL )
  {
    // --- copy the NurbsMapping from "X" ---
    if( !dbase.has_key("nurbs") )
    {
      dbase.put<NurbsMapping*>("nurbs")=NULL;
    }
    NurbsMapping *& pNurbs = dbase.get<NurbsMapping*>("nurbs");
    if( pNurbs==NULL )
       pNurbs =new NurbsMapping;
    NurbsMapping & nurbs = *pNurbs;
    nurbs=*(X.dbase.get<NurbsMapping*>("nurbs"));
  }
  else
  {
    // "X" has no NurbsMapping -- delete any existing NurbsMapping
    if( dbase.has_key("nurbs") )
    {
      NurbsMapping *&pNurbs=dbase.get<NurbsMapping*>("nurbs");
      delete pNurbs;
      pNurbs=NULL;
    }
    
  }
  

  // bool nurbsExists = dbase.has_key("nurbs");
  // NurbsMapping *pNurbs=dbase.get<NurbsMapping*>("nurbs");
  // nurbsExists = nurbsExists && pNurbs!=NULL;
  // subDir.put( nurbsExists,"nurbsExists" );
  // if( nurbsExists )
  // {
  //   pNurbs->put(subDir,"Nurbs");
  // }
  
  printF("DPM::operator = for mapping %s, numGhost=[%i,%i]\n",
            (const char*)getName(mappingName),numberOfGhostPoints(0,0),numberOfGhostPoints(1,0));

  return *this;
}

// ====================================================================================================
/// \brief Mark the Mapping as out of date.
// ===================================================================================================
int DataPointMapping::
mappingHasChanged()
{
  nurbsOutOfDate=true;
  return Mapping::mappingHasChanged();
}

// ====================================================================================================
/// \brief Generate the Nurbs (if it is out of date) that will be used to evaluate the
///   DataPointMapping and it's inverse.
// ===================================================================================================
int DataPointMapping::
generateNurbs()
{
  if( nurbsOutOfDate )
  {
    if( !dbase.has_key("nurbs") )
    {
      dbase.put<NurbsMapping*>("nurbs")=new NurbsMapping;
    }
    NurbsMapping & nurbs = *dbase.get<NurbsMapping*>("nurbs");
    
    printF("--DPM-- generateNurbs\n");

    bool evalAsNurbsSave=evalAsNurbs;
    evalAsNurbs=false; // this must be off while we interpolate this mapping (otherwise recursive loop)

    // --- do this for now : we could be more efficient to use the xy array -- also fix for parallel --
    int numberOfGhostPoint=2;
    nurbs.interpolate(*this,nurbsDegree,NurbsMapping::parameterizeByIndex,numberOfGhostPoint);

    evalAsNurbs=evalAsNurbsSave; // reset 
    nurbsOutOfDate=false;
  }
  return 0;
}


void DataPointMapping::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
// ====================================================================================================
// /Description:
//    Evaluate the Mapping.
// ===================================================================================================
{

  if( evalAsNurbs )
  {
    // printF("--DPM-- map called evalAsNurbs=%i\n",(int)evalAsNurbs);

    // --- Use a Nurbs to evaluate the mapping ---
    if( nurbsOutOfDate )
      generateNurbs();

    if( false )
      printF("--DPM-- map : eval as a NurbsMapping\n");

    NurbsMapping & nurbs = *dbase.get<NurbsMapping*>("nurbs");
    nurbs.map(r,x,xr,params);
    return;
  }


  if( params.coordinateType != cartesian )
    cerr << "DataPointMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  // Now we always use the optimised scalar version
  // printf("\n %%%%% DPM:mapScalar: orderOfInterpolation=%i, base=%i bound=%i\n",orderOfInterpolation,base,bound);

  #ifndef USE_PPP
    mapScalar( r,x,xr,params,base,bound,computeMap,computeMapDerivative );
  #else
    Overture::abort(" DataPointMapping::map: ERROR: map should not be called in parallel\n");
  #endif
  return;

//    if( !useScalarIndexing )
//    {
//      printf("\n %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
//             "  DPM: WARNING: useScalarIndexing=FALSE (orderOfInterpolation=%i)\n"
//             " %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
//             ,orderOfInterpolation);
//    }

//    if( true || (useScalarIndexing && orderOfInterpolation!=4) )
//    {
//      // printf("\n %%%%% DPM:mapScalar: orderOfInterpolation=%i, base=%i bound=%i\n",orderOfInterpolation,base,bound);
    
//      mapScalar( r,x,xr,params,I );
//      // mapScalarA(r,x,xr,params,I ); // old version dpmScalarA.C
//      return;
//    }
//    else
//    {
//      printf("\n %%%%% DPM:mapVector: orderOfInterpolation=%i\n",orderOfInterpolation);
//      // Overture::abort("mapVector has a bug! cf. conical shock. Fix this Bill.");

//      // *** warning: this does not compute correct derivatives for points on the ends where one-sided is needed
//      mapVector( r,x,xr,params,I ); // this is faster with the preprocessor version.
//       return;
//    }
  
}

// void DataPointMapping::
// mapS( const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params )
// // ====================================================================================================
// // /Description:
// //    Evaluate the Mapping.
// // ===================================================================================================
// {
//   if( params.coordinateType != cartesian )
//     cerr << "DataPointMapping::map - coordinateType != cartesian " << endl;

//   Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

//   // Now we always use the optimised scalar version
//   // printf("\n %%%%% DPM:mapScalar: orderOfInterpolation=%i, base=%i bound=%i\n",orderOfInterpolation,base,bound);

//   mapScalar( r,x,xr,params,base,bound,computeMap,computeMapDerivative );
//   return;
// }


//=================================================================================
// get a mapping from the database
//=================================================================================
int DataPointMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering DataPointMapping::get" << endl;

  subDir.get( DataPointMapping::className,"className" ); 
  if( DataPointMapping::className != "DataPointMapping" )
  {
    cout << "DataPointMapping::get ERROR in className!" << endl;
  }
  subDir.get( orderOfInterpolation,"orderOfInterpolation" );

  // The DPM data points are given the same distribution as the Mapping grid *wdh* 110812
  initializePartition();
  xy.partition(partition);

#ifdef USE_PPP
  if( true )
  {
    const intSerialArray & processorSet = partition.getProcessorSet();
    printF("DataPointMapping::get: xy array -> processors=[%i,%i]\n",
           processorSet(processorSet.getBase(0)),processorSet(processorSet.getBound(0)));
  }
#endif  

  subDir.getDistributed( xy,"xy" );
  subDir.get( dimension,"dimension" );
  subDir.get( gridIndexRange,"gridIndexRange" );
  subDir.get( delta,"delta",3 );
  subDir.get( deltaByTwo,"deltaByTwo",3 );
  subDir.get( evalAsNurbs,"evalAsNurbs" );
  subDir.get( nurbsDegree,"nurbsDegree" );
  subDir.get( nurbsOutOfDate,"nurbsOutOfDate" );

  bool nurbsExists=false;
  subDir.get( nurbsExists,"nurbsExists" );

  // printF("--DPM-- get: nurbsExists=%i nurbsOutOfDate=%i\n",(int)nurbsExists,(int)nurbsOutOfDate);
  

  if( nurbsExists )
  {
    if( !dbase.has_key("nurbs") )
    {
      dbase.put<NurbsMapping*>("nurbs")=new NurbsMapping;
    }
    NurbsMapping & nurbs = *dbase.get<NurbsMapping*>("nurbs");

    nurbs.get(subDir,"Nurbs");
  }

  Mapping::get( subDir, "Mapping" );
  delete &subDir;

  // *wdh* 2011/10/01 -- temp fix -- put this here 
  mapIsDistributed=true;  
  inverseIsDistributed=true;

  mappingHasChanged();

  nurbsOutOfDate=!nurbsExists;
  
  return 0;
}
int DataPointMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( DataPointMapping::className,"className" );
  subDir.put( orderOfInterpolation,"orderOfInterpolation" );
  subDir.putDistributed( xy,"xy" );
  subDir.put( dimension,"dimension" );
  subDir.put( gridIndexRange,"gridIndexRange" );
  subDir.put( delta,"delta",3 );
  subDir.put( deltaByTwo,"deltaByTwo",3 );
  subDir.put( evalAsNurbs,"evalAsNurbs" );
  subDir.put( nurbsDegree,"nurbsDegree" );
  subDir.put( nurbsOutOfDate,"nurbsOutOfDate" );

  bool nurbsExists = dbase.has_key("nurbs") && dbase.get<NurbsMapping*>("nurbs")!=NULL;
  subDir.put( nurbsExists,"nurbsExists" );
  if( nurbsExists )
  {
    NurbsMapping & nurbs = *dbase.get<NurbsMapping*>("nurbs");
    nurbs.put(subDir,"Nurbs");
  }

  
  
  Mapping::put( subDir, "Mapping" );
  delete & subDir;
  return 0;
}

Mapping *DataPointMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=NULL;
  if( mappingClassName==DataPointMapping::className )
    retval = new DataPointMapping();
  return retval;
}

int DataPointMapping::
specifyTopology(GenericGraphicsInterface & gi, GraphicsParameters & parameters )
//===========================================================================
// /Description:
//   Specify the topology parameters interactively
// 
//\end{DataPointMapping.tex}
//===========================================================================
{
  gi.appendToTheDefaultPrompt("topology>"); // set the default prompt
  aString menu[] = 
    {
      ">c-grid",
        "determine c-grid automatically",
        "specify c-grid matching tolerance",
//        "specify c-grid points manually",
      "<done",
      "exit",
      ""
    };
  char buff[80];
  
  real cGridTolerance = REAL_EPSILON*1000; // c-grid points should match this well (relative to xBound).
  bool plotPartialPeriodicPoints=FALSE;

  aString answer;
  for( ;; )
  {
    gi.getMenuItem(menu,answer);
    if( answer=="done" || answer=="exit" )
    {
      break;
    }
    else if( answer=="determine c-grid automatically" )
    {
      setTopologyMask(cGridTolerance);

      plotPartialPeriodicPoints=TRUE;
    }
    else if( answer=="specify c-grid matching tolerance" )
    {
      // printf("The c-grid matching tolerance is scaled by xBound=%9.2e\n",xBound);
      gi.inputString(answer,sPrintF(buff,"Enter the c-grid matching tolerance (current=%9.2e)",cGridTolerance));
      if( answer!="" )
	sScanF(answer,"%e",&cGridTolerance);
    }
    else if( answer=="specify c-grid points manually" )
    {
      int cSide=-1,cAxis=1,index1,index2;
      gi.inputString(answer,sPrintF(buff,"Enter the c-grid boundary: side,axis"));
      sScanF(answer,"%i %i",&cSide,&cAxis);
      gi.inputString(answer,sPrintF(buff,"Enter the index numbers of the trailing edge point"));
      if( answer!="" )
	sScanF(answer,"%i %i",&index1,&index2);

      plotPartialPeriodicPoints=TRUE;
    }
    else
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }


    parameters.set(GI_TOP_LABEL,getName(mappingName));
    gi.erase();
    PlotIt::plot(gi,*this,parameters);

#ifndef USE_PPP
    if( plotPartialPeriodicPoints )
    {
      parameters.set(GI_USE_PLOT_BOUNDS,TRUE); 

      for( int axis=0; axis<domainDimension; axis++ )
      {
	for( int side=0; side<=1; side++ )
	{
	  if( getTopology(side,axis)==topologyIsPartiallyPeriodic )
	  {
	    const intArray & mask = topologyMask();
	    const realArray & x = getGrid();
	    Index I1,I2,I3;
	    getBoundaryIndex(gridIndexRange,side,axis,I1,I2,I3);

	    realArray points(I1,I2,I3,rangeDimension);
	    Range Rx(0,rangeDimension-1);
	      
	    points=REAL_MAX;
	    where( mask(I1,I2,I3)!=0 )
	    {
	      for( int dir=0; dir<rangeDimension; dir++ )
		points(I1,I2,I3,dir)=x(I1,I2,I3,dir);
	    }
	      
	    int numberOfPoints=I1.getLength()*I2.getLength()*I3.getLength();
	    points.reshape(numberOfPoints,rangeDimension);
	    realArray pt(numberOfPoints,3); pt=.1;  // raise up off 2d plot
	    int j=0;
	    for( int i=0; i<numberOfPoints; i++ )
	    {
	      if( points(i,0)<REAL_MAX )
	      {
		pt(j,Rx)=points(i,Rx);
		j++;
	      }
	    }
	    numberOfPoints=j;
	    if( numberOfPoints>0 )
	    {
	      pt.resize(numberOfPoints,3);

	      parameters.set(GI_POINT_SIZE,(real)6.);  // size in pixels

	      gi.plotPoints(pt,parameters);
	    }
	      
	  }
	}
      }
      parameters.set(GI_USE_PLOT_BOUNDS,FALSE); 

    }
#endif
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
}


int DataPointMapping::
setTopologyMask(real cGridTolerance /* = -1. */ )
//===========================================================================
// /Description:
//   Build the topology mask for a c-grid.
// 
// /cGridTolerance (input) : relative tolerance for matching points on a c-grid boundary. If a value
// is given that is less than zero, this routine will choose a value.
//\end{DataPointMapping.tex}
//===========================================================================
{
  if( cGridTolerance<0. )
    cGridTolerance = REAL_EPSILON*1000; // c-grid points should match to this relative tolerance (relative to xBound).

  // grid needs to be made

  const realArray & x = getGrid();
  Index I1,I2,I3;
  Range Rx(0,getRangeDimension()-1);

  // first determine a length scale for the range space.
  real xBound =0.;
  int axis;
  for( axis=0; axis<getRangeDimension(); axis++ )
  {
    if( getRangeBound(Start,axis).isFinite() && getRangeBound(End,axis).isFinite() )
      xBound=max(xBound,(real)(getRangeBound(End,axis)-getRangeBound(Start,axis)));

  }
  if( xBound==0. )
    xBound=1.;

  printf("setTopologyMask: xBound = %e\n",xBound);

  int side,cSide,cAxis=-1,i1,i2,i3=0;
  for( axis=0; axis<domainDimension; axis++ )
  {
    for( side=0; side<=1; side++ )
    {
      getBoundaryIndex(gridIndexRange,side,axis,I1,I2,I3);
      if( max(fabs(x(I1.getBase(),I2.getBase(),i3,Rx)-x(I1.getBound(),I2.getBound(),i3,Rx))) < cGridTolerance*xBound )
      {
	cAxis=axis;
        cSide=side;
	break;
      }
    }
  }
  if( cAxis==-1 )
  {
    printf("Mapping::setTopologyMask: ERROR: unable to find the c-grid direction. No end points match\n");
    cAxis=1; // do this for now
    cSide=0;
    getBoundaryIndex(gridIndexRange,side,axis,I1,I2,I3);
  }

  setTopology(cSide,cAxis,topologyIsPartiallyPeriodic);
  approximateGlobalInverse->useRobustInverse();

  if( topologyMaskPointer==0 )
    topologyMaskPointer=new intArray;

  intArray & mask = *topologyMaskPointer;
  
  mask.redim(I1,I2,I3);
  mask=0;
  
  if( cAxis==axis2 )
  {
    int i1a=I1.getBase(), i1b=I1.getBound(), id=i1b-i1a;
    i2=I2.getBase();
    for( i1=0; i1<=id; i1++ )
    {
      real maxDiff=max(fabs(x(i1a+i1,i2,i3,Rx)-x(i1b-i1,i2,i3,Rx)));
      printf(" i1=%4i maxDiff=%9.2e, cGridTolerance*xBound=%9.2e \n",i1,maxDiff,cGridTolerance*xBound);
      
      if( maxDiff < cGridTolerance*xBound )
      {
	mask(i1a+i1,i2,i3)=1;
	mask(i1b-i1,i2,i3)=1;
      }
      else
	break;
    }
  }
  else if( cAxis==axis1 )
  {
    i1=I1.getBase();
    int i2a=I2.getBase(), i2b=I2.getBound(), id=i2b-i2a;
    for( i2=0; i2<=id; i2++ )
    {
      real maxDiff=max(fabs(x(i1,i2a+i2,i3,Rx)-x(i1,i2b-i2,i3,Rx)));
      if( maxDiff < cGridTolerance*xBound )
      {
	mask(i1,i2a+i2,i3)=1;
	mask(i1,i2b-i2,i3)=1;
      }
      else
	break;
    }
  }
  // mask.display("Here is the topology mask");
  return 0;
  
}


real DataPointMapping::
sizeOf(FILE *file /* = NULL */ ) const
// =======================================================================================
/// \details 
///    Return size of this object  
// =======================================================================================
{
  real size=Mapping::sizeOf(file);

  return size;
}


int DataPointMapping::
update( MappingInformation & mapInfo ) 
//===========================================================================
/// \brief  
///    Interactively change parameters describing the Mapping.
///    The user may choose to read in data points from a file. The current supported
///   file formats are
///    <ul>
///      <li> plot3d 
///    </ul>
///          
//===========================================================================
{

  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  aString menu[] = 
    {
      "!DataPointMapping",
      "read file",
      ">file format",
        "plot3d (default)",
      "<interp order",
      "enter points",
      "build from a mapping",
      "change plot parameters",
      "number of ghost lines",
      "boundary offset",
      "polar singularity",
      "project ghost points",
      "set data points to current grid",
      "use old inverse",
      "use new inverse",
      "point check",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
//      "c-grid",
      "use robust inverse",
      "do not use robust inverse",
      "check",
      "check inverse",
      "eval as nurbs",
      "degree of nurbs",
      "show parameters",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "read file   : read in the data from the file",
      "file format : specify info about the data file format",
      "plot3d      : plot3d format (default, single grid or multiple grid, formatted or unformated)",
      "interp order: choose the order of interpolation (2=default)",
      "build from a mapping : build a DataPointMapping from the grid points of another mapping.",
      "number of ghost lines : change the number of ghost points (default=2)",
      "boundary offset : shift location of the boundary in index space.",
      "polar singularity : specify the location of a polar (or spherical polar) singularity",
      "project ghost lines : project ghost lines onto other Mapping(s)",
      "set data points to current grid: ",
      "change plot parameters : replot and change plotting options."
      " ",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
//      "c-grid             : indicate that this is a C grid",
      "check              : check properties of this mapping",
      "check inverse      : input points to check the inverse",
      "eval as nurbs      : internally convert DPM to a Nurbs for evaluation",
      "show parameters    : print current values for parameters",
      "help        : Print this list",
      "exit        : Finished with parameters, construct grid",
      "" 
    };
  aString fileFormatMenu[] = 
    {
//      "cmpgrd",
//      "cmpgrd_1",
      "plot3d (single grid)",
      "plot3d_multiple (multiple grids)",
//      "plot3d_2",
//      "formatted",
//      "unformatted",
//      "x...y...z...",
//      "xyz...",
      "help",
      "exit", 
      "" 
     };
  aString fileFormatHelp[] = 
    {
//      "cmpgrd      : cmpgrd format (all x, then all y, [then all z))",
//      "cmpgrd_1    : cmpgrd format (xyz, xyz,...)",
      "plot3d      : plot3d format (single grid, no iblank)",
      "plot3d_1    : plot3d format (multiple grids, no iblank)",
      "plot3d_2    : like plot3d_1 but a 2D grid stored as 3D",
//      "formatted   : data file is formatted (readable) ",
//      "unformatted : data file is unformatted (binary) ",
      "help        : Print this list",
      "exit        : Finished with parameters, construct grid",
      "" 
    };
  aString nullMenu[] = { "" };
  aString yesNoMenu[] = { "yes","no","" };

  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  aString answer,line; 

  gi.outputString( ">>>DataPointMapping: Grid Defined from Data Points<<<" );

  enum fileFormats
  {
    unformatted=0,
    formatted=1
  };

  aString fileName;
  // fileFormats fileFormat=formatted;
  char buff[180];
  int i;
  bool plotObject=TRUE;
  // bool newFileToRead=FALSE;

  // int expectedNumberOfSpaceDimensions=3;
  bool useOldInverse=FALSE;

  
  gi.appendToTheDefaultPrompt("DataPoint>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject && mappingInitialized )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);

    if( answer=="read file" )
    {
      int returnValue=DataFormats::readPlot3d(*this);
      if( returnValue==0 )
	mappingInitialized=TRUE;

      // gi.inputString(fileName,"Enter the file name to read");
      // newFileToRead=TRUE;
      mappingHasChanged();
    }
    else if( answer=="interp order")
    {
      gi.outputString(sPrintF("Enter the order of interpolation (2 or 4, current=%i)...",orderOfInterpolation) );
      gi.inputString(line,"Enter the order of interpolation");
      if( line!="" ) sScanF( line,"%i",&orderOfInterpolation );
      if( orderOfInterpolation!=2 && orderOfInterpolation!=4 )
      {
	printf("Sorry, inavlid orderOfInterpolation=%i, only orders 2 or 4 are available.\n",orderOfInterpolation);
	orderOfInterpolation=2;
      }
      
    }
    else if( answer=="number of ghost lines" )
    {
      printF("numberOfGhostLines[side][axis]=[%i,%i],[%i,%i],[%i,%i]\n"
             "(must be at least orderOfInterpolation/2=%i)\n",
	     numberOfGhostPoints(0,0),numberOfGhostPoints(1,0),
	     numberOfGhostPoints(0,1),numberOfGhostPoints(1,1),
	     numberOfGhostPoints(0,2),numberOfGhostPoints(1,2),
             orderOfInterpolation/2);
      gi.inputString(line,"Enter number of ghost lines for each face, 6 values");
      if( line!="" )
      {
        IndexRangeType ngl;
        sScanF( line,"%i %i %i %i %i %i",
                &ngl(0,0),&ngl(1,0),
		&ngl(0,1),&ngl(1,1),
		&ngl(0,2),&ngl(1,2));
        for( int axis=0; axis<3; axis++ )
	  for( int side=0; side<=1; side++ )
	    ngl(side,axis)=max(ngl(side,axis),orderOfInterpolation/2);
	printf("numberOfGhostPoints(side,axis)=[%i,%i],[%i,%i],[%i,%i]\n",
	       ngl(0,0),ngl(1,0),
	       ngl(0,1),ngl(1,1),
	       ngl(0,2),ngl(1,2));

        setNumberOfGhostLines(ngl);
      }
    }
    else if( answer=="boundary offset" )
    {
      printf("Currently: gridIndexRange=[%i,%i]x[%i,%i]x[%i,%i] dimension=[%i,%i]x[%i,%i]x[%i,%i]\n",
             gridIndexRange(0,0),gridIndexRange(1,0),gridIndexRange(0,1),gridIndexRange(1,1),
             gridIndexRange(0,2),gridIndexRange(1,2),
             dimension(0,0),dimension(1,0),dimension(0,1),dimension(1,1),
             dimension(0,2),dimension(1,2) );
      printf("Enter a boundary offset for each side in order to shift the location of the boundary\n"
             "An offset=1 will shift the boundary inward by 1 (increasing the number of ghost lines).\n");
      
      gi.inputString(line,"Enter values for the offset, each side, each direction");
      int offset[2][3]={0,0,0,0,0,0};
      sScanF( line,"%i %i %i %i %i %i",
                &offset[0][0],&offset[1][0],
		&offset[0][1],&offset[1][1],
		&offset[0][2],&offset[1][2]);
      
      for( int axis=0; axis<domainDimension; axis++ )
      {
	dimension(0,axis)-=offset[0][axis];  // shift dimension so gridIndexRange(0,axis) remains zero
	dimension(1,axis)-=offset[0][axis];  
	// gridIndexRange(0,axis)+=offset[0][axis];
	gridIndexRange(1,axis)+=-offset[1][axis]-offset[0][axis];


        setGridDimensions(axis,gridIndexRange(1,axis)-gridIndexRange(0,axis)+1);

	delta[axis]=gridIndexRange(End,axis)-gridIndexRange(Start,axis);
	deltaByTwo[axis]=.5*max(1.,delta[axis]);
      }
      xy.reshape(Range(dimension(0,0),dimension(1,0)),
		 Range(dimension(0,1),dimension(1,1)),
		 Range(dimension(0,2),dimension(1,2)),xy.dimension(3));

      printf("New values: gridIndexRange=[%i,%i]x[%i,%i]x[%i,%i] dimension=[%i,%i]x[%i,%i]x[%i,%i]\n",
             gridIndexRange(0,0),gridIndexRange(1,0),gridIndexRange(0,1),gridIndexRange(1,1),
             gridIndexRange(0,2),gridIndexRange(1,2),
             dimension(0,0),dimension(1,0),dimension(0,1),dimension(1,1),
             dimension(0,2),dimension(1,2) );

      reinitialize();  // *wdh* we have to re-initialize the inverse
      mappingHasChanged();
      plotObject=TRUE;
    }
    else if( answer=="polar singularity" )
    {
      int side=-1, axis=-1;
      gi.inputString(line,"Enter side,axis for the end with a polar singularity.");
      sScanF( line,"%i %i",&side,&axis);
      if( side>=0 && side<=1 && axis>=0 && axis<=1 )
      {
	printf("Setting side=%i axis=%i to be a polar singularity\n",side,axis);
        setTypeOfCoordinateSingularity( side,axis,polarSingularity );

//         printf(" ***INFO**** using old inverse since the mapping is singular (*fix* this Bill) \n");
//         setBasicInverseOption(canDoNothing);
        printf(" ***INFO**** robust inverse since the mapping is singular \n");
        approximateGlobalInverse->useRobustInverse(TRUE);
      }
      else
      {
	printf("Invalid values for side=%i or axis=%i. Nothing changed.\n",side,axis);
      }
      
    }
    else if( answer=="enter points" )
    { 
      intArray nx(3);
      nx=1;
      realArray xyz;
      gi.inputString(line,sPrintF(buff,"Enter the domain and range dimensions (current=%i,%i)",
           domainDimension,rangeDimension));
      if( line!="" ) sScanF(line,"%i %i ",&domainDimension,&rangeDimension);
      if( domainDimension<1 || domainDimension>3 )
      {
	gi.outputString(sPrintF(buff,"Invalid domainDimension = %i",domainDimension));
	domainDimension=max(1,min(3,domainDimension));
	gi.outputString(sPrintF(buff,"setting to %i",domainDimension));
      }
      if( rangeDimension<domainDimension || rangeDimension>3 )
      {
	gi.outputString(sPrintF(buff,"Invalid rangeDimension = %i",rangeDimension));
	rangeDimension=max(domainDimension,min(3,rangeDimension));
	gi.outputString(sPrintF(buff,"setting to %i",rangeDimension));
      }
      gi.inputString(line,sPrintF(buff,"Enter the number of points in each direction: (%i integers)",domainDimension));
      if( line!="" )
      {
	if( domainDimension==1 )
          sScanF(line,"%i ",&nx(0));
        else if( domainDimension==2 )
          sScanF(line,"%i %i ",&nx(0),&nx(1));
        else
          sScanF(line,"%i %i %i",&nx(0),&nx(1),&nx(2));
      }
      nx(0)=max(nx(0),1);
      nx(1)=max(nx(1),1);
      nx(2)=max(nx(2),1);
      
      xyz.redim(nx(0),nx(1),nx(2),rangeDimension);
      for( int i3=0; i3<nx(2); i3++ )
      for( int i2=0; i2<nx(1); i2++ )
      for( int i1=0; i1<nx(0); i1++ )
      {
        if( domainDimension==1 )
          gi.inputString(line,sPrintF(buff,"Enter point (%i) (%i real numbers):",i1,rangeDimension));
        else if( domainDimension==2 )
          gi.inputString(line,sPrintF(buff,"Enter point (%i,%i) (%i real numbers):",i1,i2,rangeDimension));
        else 
          gi.inputString(line,sPrintF(buff,"Enter point (%i,%i,%i) (%i real numbers):",i1,i2,i3,rangeDimension));

        if( rangeDimension==1 )
          sScanF(line,"%e ",&xyz(i1,i2,i3,0));
        else if( rangeDimension==2 )
          sScanF(line,"%e %e ",&xyz(i1,i2,i3,0),&xyz(i1,i2,i3,1));
        else 
          sScanF(line,"%e %e %e ",&xyz(i1,i2,i3,0),&xyz(i1,i2,i3,1),&xyz(i1,i2,i3,2));

      }
      // xyz.display("Here are the grid points xyz after resize");
      xyz.display("here is xyz");
      setDataPoints(xyz,3,domainDimension);
      mappingInitialized=TRUE;
      plotObject=TRUE;
      mappingHasChanged();
    }
    else if( answer=="build from a mapping" )
    {
      // Make a menu with the Mapping names
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+1];
      int j=0;
      for( int i=0; i<num; i++ )
      {
        if( &(mapInfo.mappingList[i].getMapping()) != this )
	{
          menu2[j]=mapInfo.mappingList[i].getName(mappingName);
          j++;
	}
      }
      menu2[j]="";   // null string terminates the menu
      for( ;; )
      {
	int mapNumber = gi.getMenuItem(menu2,line);
        if( mapNumber<0 )
	{
	  printf("DataPointMapping::ERROR:unknown mapping to turn into a DataPointMapping!\n");
	  gi.stopReadingCommandFile();
	}
	else if( mapInfo.mappingList[mapNumber].mapPointer==this )
	{
	  cout << "DataPointMapping::ERROR: you cannot use this mapping, this would be recursive!\n";
	  continue;
	}
	else
	{
          setMapping( *mapInfo.mappingList[mapNumber].mapPointer );
          break;
	}
      }
      delete [] menu2;
    }
    else if( answer=="project ghost points" )
    {
      printf("Ghost points on this mapping will be projected onto other existing Mappings found\n"
             "in the mapInfo.mappingList. Points will only be projected on faces with boundaryCondtion>0\n"
             "and share flag not equal to zero. A face on another mapping will only be considered if\n"
             " the boundaryCondition>0 and the share flag is the same.\n");


      // Make a menu containing the names of all the Mapping's
      int numberOfMappings=mapInfo.mappingList.getLength();
      int numberOfMenuItems0=5;  // this many entries in menu if there are no mappings
      const int maximumNumberOfEntriesInMenu=numberOfMenuItems0+numberOfMappings+2;
      aString *menu = new aString [maximumNumberOfEntriesInMenu];
      int i=0;
      menu[i++]="!project onto mappings";
      const int mappingListStart=i;
      int j;
      for( j=0; j<numberOfMappings; j++ )
	menu[i++]=mapInfo.mappingList[j].getName(Mapping::mappingName);

      const int mappingListEnd=i-1;
      // add extra menu items
      int extra=i;
      menu[extra++]="choose all";
      menu[extra++]="choose none";
      menu[extra++]="done";   
      menu[extra++]="";   // null string terminates the menu
      assert( extra<maximumNumberOfEntriesInMenu );

      // replace menu with a new cascading menu if there are too many items.
      gi.buildCascadingMenu( menu,mappingListStart,mappingListEnd );

      int numberChosen=0;
      MappingInformation mapInfoForProjection;
      for( ;; )
      {
	int map = gi.getMenuItem(menu,answer,"project onto which mappings?");
	if( map>=mappingListStart && map<=mappingListEnd )
	{
	  mapInfoForProjection.mappingList.addElement(mapInfo.mappingList[map-mappingListStart]);
          numberChosen++;
	}
	else if( answer=="choose all" )
	{
          for( map=0; map<numberOfMappings; map++ )
            mapInfoForProjection.mappingList.addElement(mapInfo.mappingList[map]); 
          break;
	}
	else if( answer=="choose none" )
	{
          numberChosen=0;
	  break;
	}
	else if( answer=="done" || answer=="exit" )
	{
          break;
	}
      }
      if( numberChosen>0 )
      {
	for( int map=0; map<numberChosen; map++ )
	  printf("project onto mapping %s\n",(const char*) mapInfoForProjection.mappingList[map].getName(mappingName));
	  
        projectGhostPoints(mapInfoForProjection);
      }
      else if( answer!="choose none" )
      {
        printf("No projection performed. There were no Mapping's chosen\n");
      }
    }
    else if( answer=="set data points to current grid" )
    {
      int sameDimensions =TRUE;
      for( int axis=0; axis<domainDimension; axis++ )
      {
	sameDimensions= sameDimensions && 
	  ( gridIndexRange(End,axis)-gridIndexRange(Start,axis) == getGridDimensions(axis)-1 );
	if( !sameDimensions )
	  break;
      }
      if( !sameDimensions )
      {
        realArray x; x = getGrid(); // assumes getGrid() has no ghost points
#ifndef USE_PPP      
        setDataPoints(x,3,domainDimension);
#else
        OV_ABORT("error");
#endif
      }
      else
      {
        printf("DataPointMapping::INFO: data points already match the current grid\n");
	
      }
    }
    else if( answer=="change plot parameters" )
    {
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      PlotIt::plot(gi,*this,parameters);
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

    }
    else if( answer=="eval as nurbs" )
    {
      useNurbsToEvaluate( true );
    }
    else if( answer=="degree of nurbs" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the degree of the nurbs (current=%i)",nurbsDegree));
      if( line!="" ) sScanF(line,"%i",&nurbsDegree);
      printF("--DPM-- setting nurbsDegree=%i\n",nurbsDegree);
      setDegreeOfNurbs(nurbsDegree);
    }
    else if( answer=="use old inverse" )
    {
      useOldInverse=true;
      printf("Use old inverse\n");
      setBasicInverseOption(canDoNothing);
    }
    else if( answer=="use new inverse" )
    {
      useOldInverse=false;
      printf("Use new inverse\n");
      setBasicInverseOption(CANINVERT);  // basicInverse is available
    }
    else if( answer=="point check" )
    {
      realArray x(1,3),r(1,3),xr(1,3,3),x2(1,3);
      x=0.;
      r=0.;
    
      for( ;; )
      {
	if( getRangeDimension()==2 )
	{
	  printf("enter a point (x,y) to invert\n");
	  cin >> x(0,0) >> x(0,1);
	}
	else
	{
	  printf("enter a point (x,y,z) to invert\n");
	  cin >> x(0,0) >> x(0,1) >> x(0,2);
	}
      
	// dpm.inverseMap(x,r,xr);
	inverseMap(x,r);
	map(r,x2);
	printf(" x=(%6.2e,%6.2e,%6.2e) r=(%6.2e,%6.2e,%6.2e) map(r)=(%6.2e,%6.2e,%6.2e)\n",
	       x(0,0),x(0,1),x(0,2),r(0,0),r(0,1),r(0,2), x2(0,0),x2(0,1),x2(0,2));
      }
    }
    else if( answer=="use robust inverse" )
    {
      approximateGlobalInverse->useRobustInverse(TRUE);
    }
    else if( answer=="do not use robust inverse" )
    {
      approximateGlobalInverse->useRobustInverse(FALSE);
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity"  ||
             answer=="check"        ||
             answer=="check inverse" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
    }
    else if( answer=="c-grid" )
    {
      specifyTopology(gi,parameters);
    }
    else if( answer=="show parameters" )
    {
      printf(" orderOfInterpolation =%i \n",orderOfInterpolation);
      display();
    }
    else if( answer=="help" )
    {
      for( i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
    }
    else if( answer=="2D grid" )
    {
      // expectedNumberOfSpaceDimensions=2;
    }
    else if( answer=="exit" )
     break;
    else if( answer=="plotObject" )
    {
    }
    else
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }


    if( plotObject && mappingInitialized )
    {
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters);


    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;  

}
