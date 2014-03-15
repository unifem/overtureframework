//
//  Define the Base Class Mapping for defining curves, surfaces and 
//  volumes
//
#include "GenericDataBase.h"
#include "Mapping.h"
#include "MappingRC.h"
#include "MappingInformation.h"
#include "MappingProjectionParameters.h"
#include "ReferenceCountingList.h"
#include "ParallelUtility.h"
#include "display.h"
#include "Inverse.h"           // defines global and local inverses
#include "DistributedInverse.h"
#include "App.h"

// Uncomment the next line to get globalID info printed every time a Mapping is constructed or destructed
//#define MAPPING_CONSTRUCTOR_DESTRUCTOR_INFO
// extern Mapping::LinkedList *pStaticMappingList;  // list of Mappings for makeMapping
MappingLinkedList & Mapping::staticMapList() { return Overture::staticMappingList(); }

FILE *Mapping::debugFile=NULL, *Mapping::pDebugFile=NULL;

void initOvertureGlobalVariables();

int Mapping::debug=0;     // variable used for debugging
int Mapping::useInitialGuessForInverse=TRUE;
int Mapping::defaultNumberOfGhostPoints=0;

// realArray 
// floor2( const realArray & r )  // ***** until the A+= floor get fixed
// {


//   return realArray(fmod(r+2.,1.));  // map back to [0,1], add 2 to make positive, kludge**
 
// }



MappingLinkedList::
MappingLinkedList()
{ start=NULL; 
  end=NULL; 
}

MappingLinkedList:: 
~MappingLinkedList()
{ // delete Items allocated by add
  MappingItem *ptr1, *ptr2;
  ptr1=start;
  while( ptr1 )
  { 
    ptr2=ptr1;
    ptr1=ptr1->next;
    delete ptr2;
  }
}

// Add an item to the list, ie. add a pointer to the Mapping.
// Only add an item if it is not already in the list *** fix this ***
void MappingLinkedList:: 
add( Mapping *val )
{ MappingItem *ptr = new MappingItem( val );
  if( start==NULL )
  { start=ptr; end=ptr;
  }
  else
  { end->next=ptr;
    end=ptr;
  }
}

// Remove an item from the list (ie. remove the pointer to the Mapping)
// return 1 if deleted, 0 if not found
int MappingLinkedList:: 
remove( Mapping *val )
{ 
  if( start==NULL )
    return 0;
  // Since this is a singly linked list we must keep track of the
  // previous entry so we can delete an entry
  MappingItem *ptr,*ptr1;
  ptr=start;
  // see if the first item in the list points to the same Mapping
  if( ptr->val==val )
  {
    start=ptr->next;
    delete ptr;
    return 1;
  }    
  // Now check the rest of the list
  bool found=FALSE;
  while( ptr )
  {
    if( ptr->next->val==val )
    { // found! remove from the list and delete
      ptr1=ptr->next;      
      ptr->next=ptr->next->next;
      delete ptr1;
      found=TRUE;
      break;
    }
    ptr=ptr->next;
  }
  if( found )
    return 1;
  else
    return 0;
}



const static char* mappingItemString[Mapping::numberOfMappingItemNames] = 
{
  "mappingName",      
  "domainName",       
  "rangeName",
  "domainAxis1Name", 
  "domainAxis2Name", 
  "domainAxis3Name", 
  "rangeAxis1Name",  
  "rangeAxis2Name", 
  "rangeAxis3Name"
};


// Here are the default names for the axes for each coordinate system
const static char* axisName[6][3]
          = { { "x1","x2","x3"},        // cartesian
              { "phi","theta","r"},     // spherical
              { "theta","s","r"},       // cylindrical
              { "r","theta","s"},       // polar
              { "theta1","theta2","r"}  // toroidal
            };

Mapping::
Mapping(int domainDimension_ /* =3 */, 
	int rangeDimension_ /* =3 */, 
	mappingSpace domainSpace_ /* =parameterSpace */,
	mappingSpace rangeSpace_ /* =cartesianSpace */,
	coordinateSystem domainCoordinateSystem_ /* =cartesian */,
	coordinateSystem rangeCoordinateSystem_ /* =cartesian */ )
// =====================================================================================
/// \details  Default Constructor.
///  
/// \param domainDimension_ (input): 
/// \param rangeDimension_ (input):
/// \param domainSpace_ (input):
/// \param rangeSpace_ (input):
/// \param domainCoordinateSystem_ (input):
/// \param rangeCoordinateSystem_ (input):
// =====================================================================================
{ 
  className="Mapping";   // assign name for this Class
  setID();
#ifdef MAPPING_CONSTRUCTOR_DESTRUCTOR_INFO
  cout << " Mapping::Constructor called, globalID=" << getGlobalID() << endl;
#endif
  domainDimension=domainDimension_;
  rangeDimension=rangeDimension_; 

  basicInverseOption=canDoNothing;
  
  approximateGlobalInverse = NULL;
  exactLocalInverse = NULL;

  // mapIsDistrubuted=false : means the mapS function can be evaluated with no communication.  
  //        "false" is a good default. The DataPointMapping would need to change this to true.
  mapIsDistributed=false;

  // inverseIsDistributed=true :means that in parallel the special parallel inverseMap function in inverseMap.C will
  //      be used. However, if a basicInverse is supplied then it will be used instead. Thus it is best
  //      to assume "true" by default since most Mapping's that don't supply a basicInverse will use a
  //      distributed grid for the inverse.
  inverseIsDistributed=true; // we need to set this to true even in serial in case the mapping is read in parallel

  distributedInverse = NULL;
  partitionInitialized=false;
  numberOfDistributedGhostLines=1; 
  
  signForJacobian=0.; // 0 means it has not been computed yet
  arcLength=-1.; // holds the arcLength for curves, a negative value means it has not been computed yet.

  mappingHasChanged();  // for function getGrid
  
  //  Set default values:
  invertible=FALSE;
  int axis;
  for( axis=0; axis<3; axis++ )
  {
    isPeriodic[axis]=notPeriodic;
    for( int dir=0; dir<3; dir++ )
    {
      periodVector[axis][dir]=0;
    }
    for( int side=Start; side<=End; side++ )
    {
       bc[side][axis]=1;
       share[side][axis]=0;
       typeOfCoordinateSingularity[side][axis]=noCoordinateSingularity;

       numberOfGhostPoints(side,axis)=defaultNumberOfGhostPoints;
    }

    // default number of points to use when building the "grid" for this Mapping (to use for plotting etc.)
    gridIndexRange(0,axis)=0;
    gridIndexRange(1,axis)=9;

  }
  

  domainSpace=domainSpace_;
  setDefaultMappingBounds( domainSpace,domainBound );
  rangeSpace=rangeSpace_;
  setDefaultMappingBounds( rangeSpace,rangeBound );
  
  domainCoordinateSystem=domainCoordinateSystem_;
  setDefaultCoordinateSystemBounds( domainCoordinateSystem,domainCoordinateSystemBound );

  rangeCoordinateSystem=rangeCoordinateSystem_;
  setDefaultCoordinateSystemBounds( rangeCoordinateSystem,rangeCoordinateSystemBound );

  mappingCoordinateSystem0=general;

  setName( mappingName, "mapping" );

  //  Assign names to the coordinate axes
  switch ( domainSpace ){
    case parameterSpace:
      setName( domainName, "parameterSpace" );  
      setName( domainAxis1Name , axisName[domainCoordinateSystem][axis1] );
      setName( domainAxis2Name , axisName[domainCoordinateSystem][axis2] );
      setName( domainAxis3Name , axisName[domainCoordinateSystem][axis3] );
      break;
    case cartesianSpace:  default:
      setName( domainName, "cartesianSpace" );
      setName( domainAxis1Name , "x1" );
      setName( domainAxis2Name , "x2" );
      setName( domainAxis3Name , "x3" );
      break;
    }
  switch ( rangeSpace ){
    case parameterSpace:
      setName( rangeName, "parameterSpace" );  
      setName( rangeAxis1Name , axisName[rangeCoordinateSystem][axis1] );
      setName( rangeAxis2Name , axisName[rangeCoordinateSystem][axis2] );
      setName( rangeAxis3Name , axisName[rangeCoordinateSystem][axis3] );
      break;
    case cartesianSpace:  default:
      setName( rangeName, "cartesianSpace" );
      setName( rangeAxis1Name , "x1" );
      setName( rangeAxis2Name , "x2" );
      setName( rangeAxis3Name , "x3" );
      break;
    }

  // typeOfCoordinateSingularity.redim(2,3); typeOfCoordinateSingularity=noCoordinateSingularity;
  // By default, the map function is defined for a cartesian coordinate system
  // coordinateEvaluationType.redim(numberOfCoordinateSystems);
  for( int i=0; i<numberOfCoordinateSystems; i++ )
    coordinateEvaluationType[i]=FALSE;
  coordinateEvaluationType[cartesian]=TRUE;
    
  topologyMaskPointer=0;
  for( axis=0; axis<3; axis++)
    for( int side=0; side<=1; side++)
      topology[axis][side]= topologyIsNotPeriodic;

// Define the inverse functions for inverseMap:
  approximateGlobalInverse=new ApproximateGlobalInverse( *this );
  assert( approximateGlobalInverse != 0 );
  exactLocalInverse=new ExactLocalInverse( *this );
  assert( exactLocalInverse != 0 );
  
}

//-----------------------------------------------------------------------------------------
// Copy Constructor (Deep copy)
//-----------------------------------------------------------------------------------------
Mapping::
Mapping( const Mapping & X, const CopyType copyType )
{
  approximateGlobalInverse=NULL;
  exactLocalInverse=NULL;
  distributedInverse=NULL;
  topologyMaskPointer=0;
  
  if( copyType==DEEP )
    (*this)=X;  // deep copy with assigment
  else
    reference( X );
}

// Destructor
Mapping::~Mapping()
{
  if( debug & 4 )
  { cout << " Mapping::Destructor called, mappingName=" 
	 << getName(mappingName) << endl;
  }
#ifdef MAPPING_CONSTRUCTOR_DESTRUCTOR_INFO
  cout << " Mapping::Destructor called, globalID=" << getGlobalID() << endl;
#endif
  delete topologyMaskPointer;
  delete approximateGlobalInverse;
  delete exactLocalInverse;
  delete distributedInverse;
  
}

void Mapping::
openDebugFiles()
// =====================================================================================
/// \details 
///     Open the Mapping debug files.
// =====================================================================================
{
  if( debugFile==NULL )
  {
#ifdef USE_PPP
    const int np= max(1,Communication_Manager::numberOfProcessors());
    const int myid=max(0,Communication_Manager::My_Process_Number);
    debugFile = fopen(sPrintF("mappingNP%i.debug",np),"w" ); 
    pDebugFile= fopen(sPrintF("mappingNP%i.%i.debug",np,myid),"w" ); 
    fprintf(pDebugFile,
	    " ********************************************************************************** \n"
	    " *********** Mapping debug file, processor=%i, number of processors=%i ************ \n"
	    " ********************************************************************************** \n\n",
	    myid,np);
#else
    debugFile = fopen("mapping.debug","w" ); 
    pDebugFile=debugFile;
#endif
  }
}

void Mapping::
closeDebugFiles()
// =====================================================================================
/// \details 
///     Close the Mapping debug files.
// =====================================================================================
{
  if( debugFile!=NULL )
  {
    fclose(debugFile); debugFile=NULL;
  }
#ifdef USE_PPP
  if( pDebugFile!=NULL )
  {
    fclose(pDebugFile); pDebugFile=NULL;
  }
#endif
}


void Mapping::
basicInverse(const realArray & x, 
	     realArray & r, 
	     realArray & rx /* =nullDistributedArray */, 
	     MappingParameters & params /* =Overture::nullMappingParameters() */ ) 
// =====================================================================================
/// \details 
///     A derived class may optionally define this function it the class knows how
///   to rapidly compute the inverse of the mapping (by an analytic formula for example).
// =====================================================================================
{
  cout << "Mapping::basicInverse: WARNING !! This function should not be called \n";
  cout << "mmapingName = " << getName(mappingName) << endl;
  Overture::abort("error");
}

// *serial-array version*
void Mapping::
basicInverseS(const RealArray & x, 
	      RealArray & r,
	      RealArray & rx /* =Overture::nullRealArray() */,
	      MappingParameters & params /* =Overture::nullMappingParameters() */)
// =====================================================================================
// /Description:
//    A derived class may optionally define this function it the class knows how
//  to rapidly compute the inverse of the mapping (by an analytic formula for example).
//\end{MappingInclude.tex}
// =====================================================================================
{
  cout << "Mapping::basicInverseS: WARNING !! This function should not be called \n";
  cout << "mappingName = " << getName(mappingName) << endl;
  Overture::abort("error");
}


void Mapping::
breakReference()
{
  cout << "Mapping::ERROR breakReference function called! " << endl;
}

//-----------------------------------------------------------------------------------------
//  assignment with = is a deep copy
//-----------------------------------------------------------------------------------------
Mapping & Mapping::
operator=( const Mapping & x )
{
  dataBaseID                 = 0; //    x.dataBaseID;

  basicInverseOption         = x.basicInverseOption;
  className                  = x.className;
  int i;
  for( i=0; i<numberOfCoordinateSystems; i++ )
    coordinateEvaluationType[i]= x.coordinateEvaluationType[i];
  domainCoordinateSystem     = x.domainCoordinateSystem;
  domainDimension            = x.domainDimension;
  domainSpace                = x.domainSpace;
  invertible                 = x.invertible;
  mappingCoordinateSystem0   = x.mappingCoordinateSystem0;
  rangeCoordinateSystem      = x.rangeCoordinateSystem;
  rangeDimension             = x.rangeDimension;
  rangeSpace                 = x.rangeSpace;
  periodicityOfSpace         = x.periodicityOfSpace;
  arcLength                  = x.arcLength;
  inverseIsDistributed       =x.inverseIsDistributed;
  partitionInitialized       =x.partitionInitialized;
  partition                  =x.partition;
  mapIsDistributed           =x.mapIsDistributed;
  
  signForJacobian            = x.signForJacobian;
  numberOfDistributedGhostLines=x.numberOfDistributedGhostLines;
  
  int axis;
  for( axis=0; axis<3; axis++)
  {
    isPeriodic[axis]     = x.isPeriodic[axis];
    for( int side=Start; side<=End; side++ )
    { 
      gridIndexRange(side,axis)=x.gridIndexRange(side,axis);
      numberOfGhostPoints(side,axis)=x.numberOfGhostPoints(side,axis);
      
      bc[side][axis]=x.bc[side][axis];
      share[side][axis] = x.share[side][axis];
      typeOfCoordinateSingularity[side][axis]= x.typeOfCoordinateSingularity[side][axis];
      
      domainBound[axis][side] = x.domainBound[axis][side];
      rangeBound[axis][side] = x.rangeBound[axis][side];
      domainCoordinateSystemBound[axis][side] = x.domainCoordinateSystemBound[axis][side];
      rangeCoordinateSystemBound[axis][side] = x.rangeCoordinateSystemBound[axis][side];
      topology[axis][side]= x.topology[axis][side];
    }
    for( int dir=0; dir<3; dir++ )
    {
      periodVector[axis][dir]=x.periodVector[axis][dir];
    }
    
  }
  for( i=0; i<numberOfMappingItemNames; i++)
    setName( mappingItemName(i), x.namestr[i] );

  
  grid.redim(0);
  #ifdef USE_PPP
  if( x.grid.elementCount()>0 )
  {
    // grid.partition(x.grid.getPartition());
    initializePartition();
    grid.partition(partition);
    grid.redim(x.grid);
    assign(grid,x.grid);
  }
  #else
    grid=x.grid;
  #endif  
  
  // *wdh* 070727 -- set remakeGrid, remakeGridSerial and assign gridSerial
  remakeGrid=x.remakeGrid; 
  remakeGridSerial=x.remakeGridSerial;   
  #ifdef USE_PPP
    gridSerial.redim(0);  // this grid is used for the Inverse in parallel
    if( usesDistributedInverse() )
    { // if the inverse is distributed then gridSerial equals the local part of the grid array.
      getLocalArrayWithGhostBoundaries(grid,gridSerial);
    }
    else
    { // otherwise gridSerial holds the entire grid on each process
      gridSerial=x.gridSerial;
    }
  #endif

  // create the inverse objects -- we should use the = operator when these are
  // defined for the inverse classes **********************************************
  delete approximateGlobalInverse;
  approximateGlobalInverse=new ApproximateGlobalInverse( *this );
  assert( approximateGlobalInverse != 0 );
  delete exactLocalInverse;
  exactLocalInverse=new ExactLocalInverse( *this );
  assert( exactLocalInverse != 0 );

  // partition = x.partition; // do not copy the partition (?)

  if( x.topologyMaskPointer!=0 )
  {
    if( topologyMaskPointer==0 )
      topologyMaskPointer=new intArray;
    topologyMaskPointer->redim(0);
    *topologyMaskPointer=*x.topologyMaskPointer;
  }
  else
  {
    delete topologyMaskPointer;
    topologyMaskPointer=0;
  }

  return *this;
}


aString Mapping::
getClassName() const
{
  return className;
}


real Mapping::
epsilon()
// =====================================================================================
/// \details 
///     Return the tolerance used by the Mappings.
// =====================================================================================
{
#ifdef OV_USE_DOUBLE
  return REAL_EPSILON*10000.;
#else
  return REAL_EPSILON*100.;
#endif
}


void Mapping::
secondOrderDerivative(const Index & I, 
		      const realArray & r, 
		      realArray & xrr, 
		      const int axis,
		      const int & rAxis )
//=================================================================================
/// \details  compute second derivatives of the mapping by finite differences
/// \param I (input) : 
/// \param r (input) : evaulate at these points, r(I,0:domainDimension-1).
/// \param xrr (output):
/// \param axis (input): compute the derivative of x(axis,I)
/// \param rAxis (input): compute the second derivative along the direction rAxis.
//=================================================================================
{
  realArray rShift, x(I,getRangeDimension());
  
  real h,coeff[5];

  int orderOfAccuracy=4; 

  if( orderOfAccuracy==2 )
  {
    // optimal h for 2nd order differences ,  eps/h^2 = h^2
    h=min(1./128,pow(REAL_EPSILON/20.,1./4.)); 
    coeff[0]=1./SQR(h); coeff[1]=-2./SQR(h); coeff[2]=1./SQR(h);
  }
  else
  {
    h=pow(REAL_EPSILON/20.,1./6.); 
    coeff[0]=-1./(12.*SQR(h)); coeff[1]=16./(12.*SQR(h)); coeff[2]=-30./(12.*SQR(h));
    coeff[3]=coeff[1]; coeff[4]=coeff[0];
  }
  
  rShift=r;
  for( int i=0; i<=orderOfAccuracy; i++ )
  {
    rShift(I,rAxis)=r(I,rAxis)+(i-(orderOfAccuracy/2))*h;
    map(rShift,x);
    if( i==0 )
    {
      xrr(I,axis)=x(I,axis)*coeff[i];
    }
    else
    {
      xrr(I,axis)+=x(I,axis)*coeff[i];
    }
    
  }

//    printf("***second derivative, order of accuracy = %i\n",orderOfAccuracy);
//  xrr(I,axis).display("***mapDerivative: Here is xrr");

}

#ifdef USE_PPP
void Mapping::
secondOrderDerivative(const Index & I, 
		      const RealArray & r, 
		      RealArray & xrr, 
		      const int axis,
		      const int & rAxis )
//=================================================================================
// /Description: compute second derivatives of the mapping by finite differences
//  /I (input) : 
//  /r (input) : evaulate at these points, r(I,0:domainDimension-1).
// /xrr (output):
// /axis (input): compute the derivative of x(axis,I)
// /rAxis (input): compute the second derivative along the direction rAxis.
//=================================================================================
{
  RealArray rShift, x(I,getRangeDimension());
  
  real h,coeff[5];

  int orderOfAccuracy=4; 

  if( orderOfAccuracy==2 )
  {
    // optimal h for 2nd order differences ,  eps/h^2 = h^2
    h=min(1./128,pow(REAL_EPSILON/20.,1./4.)); 
    coeff[0]=1./SQR(h); coeff[1]=-2./SQR(h); coeff[2]=1./SQR(h);
  }
  else
  {
    h=pow(REAL_EPSILON/20.,1./6.); 
    coeff[0]=-1./(12.*SQR(h)); coeff[1]=16./(12.*SQR(h)); coeff[2]=-30./(12.*SQR(h));
    coeff[3]=coeff[1]; coeff[4]=coeff[0];
  }
  
  rShift=r;
  for( int i=0; i<=orderOfAccuracy; i++ )
  {
    rShift(I,rAxis)=r(I,rAxis)+(i-(orderOfAccuracy/2))*h;
    mapS(rShift,x);
    if( i==0 )
    {
      xrr(I,axis)=x(I,axis)*coeff[i];
    }
    else
    {
      xrr(I,axis)+=x(I,axis)*coeff[i];
    }
    
  }

//    printf("***second derivative, order of accuracy = %i\n",orderOfAccuracy);
//  xrr(I,axis).display("***mapDerivative: Here is xrr");

}
#endif


void Mapping::
display( const aString & label ) const
// =====================================================================================
/// \details 
///    Write the values of the Mapping parameters to standard output.
///  
// =====================================================================================
{ // Display information about the mapping

  printF("--------------------------------------------------------------------------------------\n");
  printF("%s\n",(const char*)label);
  printF("domainDimension = %i, rangeDimension =%i\n",domainDimension,rangeDimension);

  switch( domainSpace )
  {
    case parameterSpace: 
      printF("domain is parameterSpace"); break;
    case cartesianSpace: 
      printF("domain is cartesianSpace"); 
  };
  switch( rangeSpace )
  {
    case parameterSpace: 
      printF(", range is parameterSpace\n"); break;
    case cartesianSpace: 
      printF(", range is cartesianSpace\n"); break;
  };
  switch( domainCoordinateSystem )
  {
  case cartesian: 
    printF("domain coordinate system is cartesian\n"); break;
  case spherical: 
    printF("domain coordinate system is spherical\n"); break;
  case cylindrical: 
    printF("domain coordinate system is cylindrical\n"); break;
  case polar: 
    printF("domain coordinate system is polar\n"); break;
  case toroidal: 
    printF("domain coordinate system is toroidal\n"); break;
  default:
    printF("Unknown domainCoordinateSystem!! \n");
  };
  switch( rangeCoordinateSystem )
  {
    case cartesian: 
      printF("range coordinate system is cartesian\n"); break;
    case spherical: 
      printF("range coordinate system is spherical\n"); break;
    case cylindrical: 
      printF("range coordinate system is cylindrical\n"); break;
    case polar: 
      printF("range coordinate system is polar\n"); break;
    case toroidal: 
      printF("range coordinate system is toroidal\n"); break;
  default:
    printF("Unknown rangeCoordinateSystem!! \n");
  };
  int side,axis;
  for( axis=axis1; axis<domainDimension; axis++ )
    printF("isPeriodic(%i)=%s, ",axis,(isPeriodic[axis]==notPeriodic ? "not periodic" : 
             isPeriodic[axis]==functionPeriodic ? "function periodic" : "derivative periodic" ));
  printF("\n");

  for( axis=axis1; axis<domainDimension; axis++ )
    printF("gridIndexRange(0:1,%i) = [%i,%i]\n",axis,gridIndexRange(0,axis),gridIndexRange(1,axis));
  for( axis=axis1; axis<domainDimension; axis++ )
    printF("numberOfGhostPoints(0:1,%i) = [%i,%i]\n",axis,numberOfGhostPoints(0,axis),numberOfGhostPoints(1,axis));

  for( axis=axis1; axis<domainDimension; axis++ )
  {
    for( side=Start; side<=End; side++ )
      printF("bc(%i,%i)=%i, ",side,axis,bc[side][axis]);
    printF("\n");
  }
  

  for( axis=axis1; axis<domainDimension; axis++ )
  {
    for( side=Start; side<=End; side++ )
      printF("share(%i,%i)=%i, ",side,axis,share[side][axis]);
    printF("\n");
  }

  for( axis=axis1; axis<domainDimension; axis++ )
  {
    for( side=Start; side<=End; side++ )
      printF("coordinateSingularity(%i,%i)=%i, ",side,axis,typeOfCoordinateSingularity[side][axis]);
    printF("\n");
  }
  
  for( axis=axis1; axis<domainDimension; axis++ )
    for( side=Start; side<=End; side++ )
      switch( typeOfCoordinateSingularity[side][axis] )
      {
      case polarSingularity:
	printf("Mapping has a polar coordinate singularity on side=%i, axis=%i \n",side,axis); break;
      default:
	break;
      };
  
//  printF("className = " << className << endl;

  printF("basicInverseOption = %s \n",basicInverseOption==canDoNothing ? "canDoNothing" : 
	 basicInverseOption==canInvert ? "canInvert" : "canDetermineOutside");

  for( int i=0; i<numberOfMappingItemNames; i++ )
    printF("%s = %s\n",(const char*)mappingItemString[i],(const char*)getName(mappingItemName(i)));
  printF("------------------------------------------------------\n");

  
}

Index Mapping::
getIndex(const realArray & r, 
	 realArray & x, 
	 const realArray &xr,
	 int & base0, 
	 int & bound0, 
	 int & computeMap0, 
	 int & computeMapDerivative0 )
// =====================================================================================
/// \details 
///     Return an Index operator for loops in the map and inverseMap functions
///   Also compute the members:
/// \param computeMapping : TRUE or FALSE
/// \param computeMappingDerivative : TRUE or FALSE
/// \param base : base for Index
/// \param bound : bound for the Index
/// 
/// \param NOTE: do note make x "const" so we check that this routine is called
///        correctly from map and inverseMap
// =====================================================================================
{
//  base and bound are defined by the r array:
  base=r.getBase(0);
  bound=r.getBound(0);

// Only compute x if there is space
  computeMap = 
        ( // (x.getLength(axis1) >= rangeDimension) &&
          (x.getBase(0) <= base) &&
          (x.getBound(0) >= bound) );

// Only compute xr if there is space
  computeMapDerivative = 
       ( (xr.getBase(0) <= base) &&
         (xr.getBound(0) >= bound) );

  if( !computeMap && !computeMapDerivative )
  {
    cout << "Mapping::getIndex WARNING - both computeMap and " 
         << "computeMapDerivative are FALSE! " << endl;
    cout << "         mappingName=" << getName(mappingName) << endl;
    printf("domainDimension=%i, rangeDimension=%i, r.getBase(0)=%i, r.getBound(0)=%i\n",
           domainDimension,rangeDimension,base,bound);
    printf("x.getLength(0)=%i, x.getBase(0)=%i, x.getBound(0)=%i, \n xr.getLength(1)=%i, "
           "xr.getLength(2)=%i, xr.getBase(0)=%i, xr.getBound(0)=%i \n",
	   x.getLength(0),x.getBase(0),x.getBound(0),xr.getLength(1),xr.getLength(2),
           xr.getBase(0),xr.getBound(0));

    if( Mapping::debug >0 )
    {
      Overture::abort("error");
    }
    
  }
  
  base0=base;
  bound0=bound;
  computeMap0=computeMap;
  computeMapDerivative0=computeMapDerivative;

 return Index(base,bound-base+1);
}

#ifdef USE_PPP
Index Mapping::
getIndex(const RealArray & r, 
	 RealArray & x, 
	 const RealArray &xr,
	 int & base0, 
	 int & bound0, 
	 int & computeMap0, 
	 int & computeMapDerivative0 )
// =====================================================================================
// /Description:
//    Return an Index operator for loops in the map and inverseMap functions
//  Also compute the members:
//  /computeMapping : TRUE or FALSE
//  /computeMappingDerivative : TRUE or FALSE
//  /base : base for Index
//  /bound : bound for the Index
//
// /NOTE: do note make x "const" so we check that this routine is called
//       correctly from map and inverseMap
//\end{MappingInclude.tex}
// =====================================================================================
{
//  base and bound are defined by the r array:
  base=r.getBase(0);
  bound=r.getBound(0);

// Only compute x if there is space
  computeMap = 
        ( // (x.getLength(axis1) >= rangeDimension) &&
          (x.getBase(0) <= base) &&
          (x.getBound(0) >= bound) );

// Only compute xr if there is space
  computeMapDerivative = 
       ( (xr.getBase(0) <= base) &&
         (xr.getBound(0) >= bound) );

  if( !computeMap && !computeMapDerivative )
  {
    cout << "Mapping::getIndex WARNING - both computeMap and " 
         << "computeMapDerivative are FALSE! " << endl;
    cout << "         mappingName=" << getName(mappingName) << endl;
    printf("domainDimension=%i, rangeDimension=%i, r.getBase(0)=%i, r.getBound(0)=%i\n",
           domainDimension,rangeDimension,base,bound);
    printf("x.getLength(0)=%i, x.getBase(0)=%i, x.getBound(0)=%i, \n xr.getLength(1)=%i, "
           "xr.getLength(2)=%i, xr.getBase(0)=%i, xr.getBound(0)=%i \n",
	   x.getLength(0),x.getBase(0),x.getBound(0),xr.getLength(1),xr.getLength(2),
           xr.getBase(0),xr.getBound(0));

    if( Mapping::debug >0 )
    {
      Overture::abort("error");
    }
  }
  
  base0=base;
  bound0=bound;
  computeMap0=computeMap;
  computeMapDerivative0=computeMapDerivative;

 return Index(base,bound-base+1);
}
#endif

int Mapping::
get( const GenericDataBase & dir, const aString & name)
// =====================================================================================
/// \details 
///     Get this object from a sub-directory called "name"
// =====================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  subDir.setMode(GenericDataBase::streamInputMode);

  subDir.get( className,"className" ); 
  // subDir.get( dataBaseID,"dataBaseID");
  dataBaseID=dir.getID();
  
  GenericDataBase & dir2 = (GenericDataBase &)dir;  // cast away const
  if( dir2.getList()!=NULL )
    dir2.getList()->add(this,dataBaseID);
  
  int temp;
  subDir.get( temp,"basicInverseOption" ); basicInverseOption=basicInverseOptions(temp);
  subDir.get( (int*)bc,"bc",6 );
  if( className != "Mapping" )
  {
    cout << "Mapping::get ERROR in className!" << endl;
    cout << "className for database = `" << className << "'" << endl;
  }
  subDir.get( coordinateEvaluationType,"coordinateEvaluationType",numberOfCoordinateSystems );
  subDir.get( temp,"domainCoordinateSystem" ); domainCoordinateSystem=coordinateSystem( temp );
  subDir.get( domainDimension,"domainDimension" );
  assert( domainDimension>0 && domainDimension<4 );
  subDir.get( temp,"domainSpace" );  domainSpace = mappingSpace( temp );
  assert( domainSpace==parameterSpace || domainSpace==cartesianSpace );
  int ta6[6];
  subDir.get(ta6,"gridIndexRange",6);
  for( int axis=0; axis<3; axis++)for( int side=0; side<=1; side++ ) gridIndexRange(side,axis)=ta6[side+2*axis];
  subDir.get(ta6,"numberOfGhostPoints",6);
  for( int axis=0; axis<3; axis++)for( int side=0; side<=1; side++ ) numberOfGhostPoints(side,axis)=ta6[side+2*axis];

  subDir.get( invertible,"invertible" );
  int ta3[3];
  subDir.get( ta3,"isPeriodic",3 );
  for( int axis=0; axis<3; axis++)
    isPeriodic[axis]=(periodicType)ta3[axis];
  
  subDir.get( temp,"mappingCoordinateSystem0"); mappingCoordinateSystem0=mappingCoordinateSystem(temp);

  subDir.get( (real*)periodVector,"periodVector",9 );
  subDir.get( periodicityOfSpace,"periodicityOfSpace");

  subDir.get( signForJacobian,"signForJacobian");
  subDir.get( numberOfDistributedGhostLines,"numberOfDistributedGhostLines");
  subDir.get( arcLength,"arcLength" );
  subDir.get( inverseIsDistributed,"inverseIsDistributed" );
  subDir.get( mapIsDistributed,"mapIsDistributed" );

  subDir.get( temp,"rangeCoordinateSystem" ); rangeCoordinateSystem=coordinateSystem(temp);
  subDir.get( rangeDimension,"rangeDimension" );
  assert( rangeDimension>0 && rangeDimension<4 );
  subDir.get( temp,"rangeSpace" );   rangeSpace = mappingSpace( temp );
  subDir.get( remakeGrid,"remakeGrid" );
  subDir.get( (int*)share,"share",6 );
  int ta23[2][3];
  subDir.get( (int*)ta23,"typeOfCoordinateSingularity",6 ); 
  for( int axis=0; axis<3; axis++)
    for( int side=0; side<=1; side++ )
      typeOfCoordinateSingularity[side][axis]=(coordinateSingularity)ta23[side][axis];
  
  const int bufLen = 80;
  static char buf[bufLen];
  int i;
  for( i=0; i<3; i++ )
    for( int side=Start; side<=End; side++ )
    { 
      sPrintF(buf,"domainBound[%i][%i]",i,side);
      domainBound[i][side].get( subDir,buf );
      sPrintF(buf,"rangeBound[%i][%i]",i,side);
      rangeBound[i][side].get( subDir,buf );
      sPrintF(buf,"domainCoordinateSystemBound[%i][%i]",i,side);
      domainCoordinateSystemBound[i][side].get( subDir,buf );
      sPrintF(buf,"rangeCoordinateSystemBound[%i][%i]",i,side);
      rangeCoordinateSystemBound[i][side].get( subDir,buf );
    }
  aString itemName;
  for( i=0; i<numberOfMappingItemNames; i++ )
  { 
    subDir.get( itemName,mappingItemString[i] );
    if( debug & 64 )
      cout << "Mapping::get: itemName= " << itemName << endl;
    setName( mappingItemName(i),itemName );
  }

  approximateGlobalInverse->get(subDir,"approximateGlobalInverse");
  exactLocalInverse->get(subDir,"exactLocalInverse");

  
  delete & subDir;
  return TRUE;
}

int Mapping::
getID() const 
// =====================================================================================
/// \details 
///    Get the current value for the Mapping identifier, a unique number to use when
///   saving the Mapping in a database file. This value is used to avoid having
///  multiple copies of a Mapping saved in a data base file.  
// =====================================================================================
{ 
  return dataBaseID;
}

void Mapping::
setID() 
// =====================================================================================
/// \details 
///    Set a new value for the Mapping identifier, a unique number to use when
///   saving the Mapping in a database file. This value is used to avoid having
///  multiple copies of a Mapping saved in a data base file.  
// =====================================================================================
{ 
  dataBaseID=0; // getGlobalID();  // do this for now
}




Mapping::basicInverseOptions Mapping::
getBasicInverseOption() const
// =====================================================================================
/// \details 
// =====================================================================================
{
  return basicInverseOption;
}


int Mapping::
getBoundaryCondition( const int side, const int axis ) const 
// =====================================================================================
/// \details 
///    Return the boundary condition code for a side of the mapping.
///    A positive value denotes a physical boundary, 0 an interpolation boundary
///    and a negative value a periodic direction.
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{ 
  int retval=0;
  if( validSide( side ) && (axis>=0 && axis<rangeDimension) )
    retval = bc[side][axis];
  else
    printF(" Mapping::getBoundaryCondition: Invalid arguments \n");
  return retval;
}

//------Access Functions 

RealArray Mapping::
getBoundingBox(const int & side /* = -1 */, 
               const int & axis /* = -1 */) const 
// =====================================================================================
/// \details 
///    Return the bounding box for the Mapping (if side<0 and axis<0) or the bounding
///    box for a particular side.
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{ 
  //if( true  )
  //  printF("Mapping::getBoundingBox usesDistributedInverse=%i (%s)\n",(int)usesDistributedInverse(),
  //         (const char*)getName(Mapping::mappingName));

#ifndef USE_PPP
  // *wdh* 090705 -- do not use the ditributed inverse to compute the bounding box in serial
  //                 This was causing trouble with moving grids (drops)
  if( true ) 
#else
  if( !usesDistributedInverse() )
#endif
  {
    // In this case the bounding boxes can be computed in serial

    assert( approximateGlobalInverse!=0 );
    approximateGlobalInverse->initialize();
    if( side<0 || axis<0 )
    {
      return approximateGlobalInverse->boundingBox;
    }
  
    if( !validSide( side ) || !validAxis( axis ) )
    {
      printF(" Mapping::getBoundingBox: Invalid arguments \n");
      Overture::abort("error");
    }
    return approximateGlobalInverse->boundingBoxTree[side][axis].getRangeBound();
  }
  else
  {
    // In this case the bounding boxes are computed in parallel since the grid is distributed

    if( distributedInverse==NULL )
    {
      distributedInverse = new DistributedInverse((Mapping&)(*this));
      distributedInverse->computeBoundingBoxes();
    }
    if( side<0 || axis<0 )
    {
      return distributedInverse->getBoundingBox();
    }
  
    if( !validSide( side ) || !validAxis( axis ) )
    {
      printF(" Mapping::getBoundingBox: Invalid arguments \n");
      Overture::abort("error");
    }
    return distributedInverse->getBoundingBoxTree(side,axis).getRangeBound();
      
  }
}

int Mapping::
getBoundingBox( const IntegerArray & indexRange, const IntegerArray & gridIndexRange_,
                RealArray & xBounds, bool local /* = false */ ) const
// =====================================================================================
/// \details 
///    Return the bounding box, xBounds, for the set of grid points spanned by 
///    indexRange. 
/// 
/// \param indexRange(0:1,0:2) (input) : range of indicies, i_m=indexRange(0,m),...,indexRange(1,m)
/// \param gridIndexRange_(0:1,0:2) (input) : Normally these should match the gridIndexRange of the Mapping.
///     This argument is used to double check that this is true.
/// \param xBounds(0:1,0:2) : bounds
/// \param local (input) : if local=true then only compute the min and max over points on this processor, otherwise
///                   compute the min and max over all points on all processors
/// 
/// \return  0=success, 1=indexRange values are invalid, 2=cannot compute bounds with local=true since
///    the indexRange values do not lie within the local array.
// =====================================================================================
{
  if( !gridIsValid() )
  {
    ((Mapping*)(this))->getGrid();
  }
  
  const IntegerArray & gid = gridIndexRange_;
  Range R[3];
  for( int axis=0; axis<3; axis++ )
  {
    if( indexRange(0,axis)<grid.getBase(axis) || indexRange(1,axis)>grid.getBound(axis) )
    {
      printf("Mapping::getBoundingBox:ERROR: indexRange(0:1,%i)=[%i,%i] are out of bounds\n"
             "                           grid.getBase(%i)=%i grid.getBound(%i)=%i\n",
	     axis,indexRange(0,axis),indexRange(1,axis),axis,grid.getBase(axis),axis,grid.getBound(axis));

      Overture::abort("error");
      // return 1;
    }
    for( int side=0; side<=1; side++ )
    {
      if( axis<domainDimension && gid(side,axis)!=gridIndexRange(side,axis) )
      {
	printf("Mapping::getBoundingBox:ERROR: gridIndexRange_(0:1,%i)=[%i,%i] does NOT match\n"
	       "                  the Mapping::gridIndexRange_(0:1,%i)=[%i,%i]. name=%s\n",
	       axis,gridIndexRange(0,axis),gridIndexRange(1,axis),axis,gid(0,axis),gid(1,axis),
               (const char*)getName(mappingName));
	Overture::abort("error");

      }
    }
    
    R[axis]= Range(indexRange(0,axis),indexRange(1,axis));
  }
   
  #ifdef USE_PPP
  if( local )
  { // If we compute local bounds then the indexRange values must lie in the local grid array:
    realSerialArray gridLocal;  getLocalArrayWithGhostBoundaries(grid,gridLocal);
    bool localBoundsOk=true;
    if( gridLocal.elementCount()>0 )  // local grid is not empty
    {
      for( int axis=0; axis<3; axis++ )
      {
	if( R[axis].getBase() <gridLocal.getBase(axis) || 
	    R[axis].getBound()>gridLocal.getBound(axis) )
	{
	  localBoundsOk=false;
	  break;
	}
      }
      if( !localBoundsOk )
	return 2;
    }
    // if( true ) return 2;  // * for testing *
    
  }
  #endif


  // set Range bounds
  real xMin[3], xMax[3];
  getGridMinAndMax(grid,R[0],R[1],R[2],xMin,xMax,local);
  for( int axis=0; axis<rangeDimension; axis++ )
  {
    xBounds(0,axis)=xMin[axis];
    xBounds(1,axis)=xMax[axis];
  }
  if( Mapping::debug & 4 )
  {
    printF("Mapping::getBoundingBox(indexRange,xBounds): called for %s\n",(const char*)getName(mappingName));
    printF("Mapping::getBoundingBox: indexRange=[%i,%i][%i,%i][%i,%i] xBounds=[%g,%g][%g,%g][%g,%g]\n",
	   indexRange(0,0),indexRange(1,0),indexRange(0,1),indexRange(1,1),indexRange(0,2),indexRange(1,2),
	   xBounds(0,0),xBounds(1,0),xBounds(0,1),xBounds(1,1),xBounds(0,2),xBounds(1,2));
  }
  
  return 0;
}


int Mapping::
getBoundingBox( const RealArray & rBounds, RealArray & xBounds ) const
// =====================================================================================
// /Description:
//   Return the bounding box, xBounds, for the range space that corresponds to the
//   bounding box, rBounds, in the domain space. 
//\end{MappingInclude.tex}
// =====================================================================================
{
  // do this for now:
  printF("Mapping:getBoundingBox(rBounds,...) WARNING : not implemented yet!\n");
  xBounds=getBoundingBox();
  return 0;
}


const BoundingBox & Mapping::   
getBoundingBoxTree( const int & side, 
                    const int & axis ) const
// =====================================================================================
/// \details 
///    Return the BoundingBox (tree) for a side of a Mapping.
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{
  assert( approximateGlobalInverse!=0 );
  approximateGlobalInverse->initialize();
  if( !validSide( side ) || !validAxis( axis ) )
  {
    cout << " Mapping::getBoundingBoxTree: Invalid arguments " << endl;
    Overture::abort("error");
  }
  return approximateGlobalInverse->boundingBoxTree[side][axis];
}


int Mapping::
getCoordinateEvaluationType( const coordinateSystem type ) const 
// =====================================================================================
/// \details 
// =====================================================================================
{ 
  return coordinateEvaluationType[type]; 
}

Bound Mapping::
getDomainBound( const int side, const int axis ) const
// =====================================================================================
/// \details 
///     
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{ 
  if( validSide( side ) && validAxis( axis ) )
    return domainBound[axis][side];
  else
    { cout << " Mapping::getDomainBound: Invalid arguments " << endl;
      return 0;
    }
}


Mapping::coordinateSystem Mapping::
getDomainCoordinateSystem() const 
// =====================================================================================
/// \details 
// =====================================================================================
{ return domainCoordinateSystem; }

Bound Mapping::
getDomainCoordinateSystemBound( const int side, const int axis ) const
// =====================================================================================
/// \details 
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{ 
  if( validSide( side ) && validAxis( axis ) )
    return domainCoordinateSystemBound[axis][side];
  else
    { cout << " Mapping::getDomainCoordinateSystemBound: Invalid arguments, (side,axis)=(" << side 
           << "," << axis << ")\n";
      return 0;
    }
}

int Mapping::
getDomainDimension() const 
// =====================================================================================
/// \details 
// =====================================================================================
{ 
return domainDimension; 
}

Mapping::mappingSpace Mapping::
getDomainSpace() const 
// =====================================================================================
/// \details 
// =====================================================================================
{ return domainSpace; }

int Mapping::
getGridDimensions( const int axis ) const
// =====================================================================================
/// \details 
/// \param axis (input): axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{ 
  int retval=0;
  if( validAxis( axis ) )
    retval = gridIndexRange(1,axis)-gridIndexRange(0,axis)+1; 
  else
    printF("Mapping::getGridDimensions: Invalid arguments: axis=%i. (domainDimension=%i)\n",axis,domainDimension);
  return retval;
}

const realArray& Mapping::
getGrid(MappingParameters & params /* =Overture::nullMappingParameters() */,
        bool includeGhost /* =false */ )
// ================================================================================
/// \details 
///    Return an array that holds the values of this Mapping evaluated on
///   an array of equally spaced points. Note that this array may or may not
///   contain ghost points. If $x$ denotes the array that is returned, then
///   the values that are guaranteed to be there are
///     \[
///          x(0:n_0,0:n_1,0:n_2,0:rangeDimension-1) 
///     \]
///    where $n_i = getGridDimensions(i)-1$. Thus the valid values will always start
///    with base 0 in the array. The array x may have ghost points in which case the
///    base will be less than $0$ and the bound greater than $n_i$.
/// \return  An array x 
/// \param Note: For efficiency the array is returned by reference. Thus {\bf you should not
///     alter the array that is returned by this routine}.
// ================================================================================
{

  if( remakeGrid || grid.elementCount()==0 )
  {
    if( Mapping::debug & 4 )
      printF("Mapping:getGrid called, remake grid for mapping %s\n",(const char*)getName(mappingName));

    // first assign the indexRange and dimension arrays:
    IntegerArray dimension(2,3);
    dimension=0;
    int axis;
    for( axis=axis1; axis<domainDimension; axis++ )
    {
      dimension(0,axis)=gridIndexRange(0,axis)-numberOfGhostPoints(0,axis);
      dimension(1,axis)=gridIndexRange(1,axis)+numberOfGhostPoints(1,axis);
    }
    
  if( false )
  {
    for( int axis=axis1; axis<domainDimension; axis++ )for( int side=0; side<=1; side++ )
    {
      int nMin = ParallelUtility::getMinValue(dimension(side,axis));
      int nMax = ParallelUtility::getMaxValue(dimension(side,axis));
      if( nMin!=nMax )
      {
	int myid=max(0,Communication_Manager::My_Process_Number);
	printF("Mapping:getGrid: mapping=%s : dimension's don't match on different processors!\n",
	       (const char*)getName(mappingName));
	printf(" myid=%i map.getGridDimensions=[%i,%i,%i]\n",
	       myid,getGridDimensions(0),getGridDimensions(1),getGridDimensions(2));
	printf(" myid=%i dimension=[%i,%i][%i,%i][%i,%i]\n",
	       myid,dimension(0,0),dimension(1,0),dimension(0,1),dimension(1,1),dimension(0,2),dimension(1,2));
	printf(" myid=%i numberOfGhostPoints=[%i,%i][%i,%i][%i,%i]\n",
	       myid,numberOfGhostPoints(0,0),numberOfGhostPoints(1,0),
               numberOfGhostPoints(0,1),numberOfGhostPoints(1,1),numberOfGhostPoints(0,2),numberOfGhostPoints(1,2));
	
	fflush(0);
	Overture::abort("error");
      }
    }
  }

    Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];

    J1=Range(dimension(Start,axis1),dimension(End,axis1));
    J2=Range(dimension(Start,axis2),dimension(End,axis2));
    J3=Range(dimension(Start,axis3),dimension(End,axis3));
  
    initializePartition();
    grid.partition(partition);
    grid.redim( J1,J2,J3,rangeDimension);

    real dr[3];
    for( axis=axis1; axis<=axis3; axis++ )
      dr[axis]=1./max(gridIndexRange(1,axis)-gridIndexRange(0,axis),1);


    #ifdef USE_PPP
      realSerialArray gridLocal;  getLocalArrayWithGhostBoundaries(grid,gridLocal);  
      if( usesDistributedInverse() )
      { // if the inverse is distributed then gridSerial equals the local part of the grid array.
        gridSerial.reference(gridLocal);
      }
      J1=gridLocal.dimension(0); J2=gridLocal.dimension(1); J3=gridLocal.dimension(2);
      realSerialArray r(J1,J2,J3,domainDimension);
      r=0.;  // avoid UMR's in valgrind
    #else
      realSerialArray r(J1,J2,J3,domainDimension);
      realSerialArray & gridLocal = grid;
    #endif
    for( axis=0; axis<domainDimension; axis++ ) 
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
        if( rval>=1. && getIsPeriodic(axis)==Mapping::functionPeriodic )
          rval = dr[axis]*(k - gridIndexRange(1,axis));
	 
	r(J1,J2,J3,axis) = rval;
      } // end for
      Jv[axis] = Jaxis; // reset
    } // end for

    #ifdef USE_PPP  
      mapGridS(r,gridLocal,Overture::nullRealArray(),params);
    #else
      mapGrid(r,grid,Overture::nullRealDistributedArray(),params);
    #endif


    // enforce periodicity exactly
    // ***** better --> just change rVal above !
//     #ifdef USE_PPP
//       printF("Mapping::getGrid: enforce periodicity exactly NOT implemented yet! fix me Bill!\n");
//     #else

//     // fix for ghost points too !
//     Range xAxes(0,rangeDimension-1);
//     Index I1=Range(gridIndexRange(0,0),gridIndexRange(1,0));
//     Index I2=Range(gridIndexRange(0,1),gridIndexRange(1,1));
//     Index I3=Range(gridIndexRange(0,2),gridIndexRange(1,2));
    
//     if( getIsPeriodic(axis1)==Mapping::functionPeriodic )
//       grid(I1.getBound(),I2,I3,xAxes)=grid(I1.getBase(),I2,I3,xAxes);
//     if( domainDimension>1 &&  getIsPeriodic(axis2)==Mapping::functionPeriodic )
//       grid(I1,I2.getBound(),I3,xAxes)=grid(I1,I2.getBase(),I3,xAxes);
//     if( domainDimension>2 &&  getIsPeriodic(axis3)==Mapping::functionPeriodic )
//       grid(I1,I2,I3.getBound(),xAxes)=grid(I1,I2,I3.getBase(),xAxes);
//     #endif

    // set Range bounds
//     for( axis=0; axis<rangeDimension; axis++ )
//     {
//       setRangeBound(Start,axis,min(grid(J1,J2,J3,axis)));
//       setRangeBound(End  ,axis,max(grid(J1,J2,J3,axis)));
//     }
    real xMin[3], xMax[3];
    getGridMinAndMax(grid,J1,J2,J3,xMin,xMax);
    for( int axis=0; axis<rangeDimension; axis++ )
    {
      setRangeBound(Start,axis,xMin[axis]);
      setRangeBound(End  ,axis,xMax[axis]);
    }

    // determine the sign of the jacobian if domainDimension==rangeDimension
    getSignForJacobian();

    remakeGrid=false;
    remakeGridSerial=false;
    
  }
  else
  {
    if( Mapping::debug & 4 )
      printF("Mapping:getGrid called, use existing grid\n");
  }
  
  if( false )
  {
    for( int axis=axis1; axis<domainDimension; axis++ )
    {
      int nMin = ParallelUtility::getMinValue(grid.getLength(axis));
      int nMax = ParallelUtility::getMaxValue(grid.getLength(axis));
      if( nMin!=nMax )
      {
	int myid=max(0,Communication_Manager::My_Process_Number);
	printF("Mapping:getGrid: mapping=%s : dimension's don't match on different processors!\n",
	       (const char*)getName(mappingName));
	printf(" myid=%i map.getGridDimensions=[%i,%i,%i]\n",
	       myid,getGridDimensions(0),getGridDimensions(1),getGridDimensions(2));
	printf(" myid=%i grid=[%i,%i][%i,%i][%i,%i]\n",
	       myid,
	       grid.getBase(0),grid.getBound(0),
	       grid.getBase(1),grid.getBound(1),
	       grid.getBase(2),grid.getBound(2));
	
	fflush(0);
	Overture::abort("error");
      }
    }
  }
//   bool returnView=false;
//   for( axis=0; axis<domainDimension; axis++ )
//   {
//     if( grid.getBase(axis)!=0 || grid.getBound(axis)!=(gridDimensions[axis]-1) )
//     {
//       returnView=true;
//       break;
//     }
//   }
//   if( !returnView )
//     return grid;
//   else
//   {
//     // return a view if the grid has ghost points
//     Range R[3]={0,0,0}; // 
//     for( axis=0; axis<domainDimension; axis++ )
//       R[axis]=gridDimensions[axis];
//     Range Rx=rangeDimension;
//     return grid(R[0],R[1],R[2],Rx);
//   }

  return grid;
  
}


const RealArray& Mapping::
getGridSerial(MappingParameters & params /* =Overture::nullMappingParameters() */,
              bool includeGhost /* =false */ )
// ================================================================================
/// \details 
///    Return an array that holds the values of this Mapping evaluated on
///   an array of equally spaced points. Note that this array may or may not
///   contain ghost points. If $x$ denotes the array that is returned, then
///   the values that are guaranteed to be there are
///     \[
///          x(0:n_0,0:n_1,0:n_2,0:rangeDimension-1) 
///     \]
///    where $n_i = getGridDimensions(i)-1$. Thus the valid values will always start
///    with base 0 in the array. The array x may have ghost points in which case the
///    base will be less than $0$ and the bound greater than $n_i$.
/// \return  An array x 
/// \param Note: gridSerial is used for the parallel inverse. If the inverse is distributed then
///    gridSerial will equal the local-array-with-ghost-boundaries of grid. Otherwise grid-serial
///    will hold the entire grid on each processor. In a serial computation, gridSerial is the same as grid.
/// \param Note: For efficiency the array is returned by reference. Thus {\bf you should not
///     alter the array that is returned by this routine}.
// ================================================================================
{
#ifndef USE_PPP
  return getGrid(params,includeGhost);
  
#else
  // *** parallel version ***

  if( usesDistributedInverse() )
  {
    // make the parallel grid (gridSerial will hold the local-array portion of grid)
    getGrid(params,includeGhost);
    // return the local array version:
    return gridSerial;
  }
  else if( remakeGridSerial || gridSerial.elementCount()==0 )
  {
    // *** gridSerial will hold the entire grid on each processor ***
    //  ---  gridSerial is used by the parallel inverse ---

    if( Mapping::debug & 4 )
    {
      printF("Mapping:getGridSerial:remake gridSerial for mapping %s. (remakeGridSerial=%i, elementCount=%i)\n",
	     (const char*)getName(mappingName),remakeGridSerial,gridSerial.elementCount());
      printF("Mapping:getGridSerial: gridIndexRange=[%i,%i][%i,%i][%i,%i]\n",
             gridIndexRange(0,0),gridIndexRange(1,0),gridIndexRange(0,1),gridIndexRange(1,1),
	     gridIndexRange(0,2),gridIndexRange(1,2));

    }
      
    // first assign the indexRange and dimension arrays:
    IntegerArray dimension(2,3);
    dimension=0;
    int axis;
    for( axis=axis1; axis<domainDimension; axis++ )
    {
      // *wdh* 2012/03/01 - this fix made for AMR grids which can have a non-zero base (c.f. cgcns wing2d.cmd in parallel + amr)
      // dimension(End,axis)= getGridDimensions(axis)-1;
      dimension(Start,axis)=gridIndexRange(0,axis);
      dimension(End  ,axis)=gridIndexRange(0,axis) + getGridDimensions(axis)-1;
    }
    
    Range R1(dimension(Start,axis1),dimension(End,axis1));
    Range R2(dimension(Start,axis2),dimension(End,axis2));
    Range R3(dimension(Start,axis3),dimension(End,axis3));
  
    gridSerial.redim( R1,R2,R3,rangeDimension);
    RealArray r(R1,R2,R3,domainDimension);

    real dr[3];

    for( axis=axis1; axis<=axis3; axis++ )
      dr[axis]=1./max(gridSerial.getBound(axis)-gridSerial.getBase(axis),1);

    // ************** here we assume there are no ghost points on the grid ******* 
    int i1,i2,i3;
    if( domainDimension==1 )
    {
      i2=gridSerial.getBase(axis2), i3=gridSerial.getBase(axis3);
//      r(R1,i2,i3,0).seqAdd(0.,dr[axis1]); // roundoff problems
      for( i1=gridSerial.getBase(axis1); i1<=gridSerial.getBound(axis1); i1++ )
	r(i1,i2,i3,0)=(i1-gridSerial.getBase(axis1))*dr[axis1];
// AP: Make sure the last point gets parameter value 1 (roundoff can affect this)
      r(R1.getBound(),i2,i3,0) = 1.;
    }
    else
    {
      for( i1=gridSerial.getBase(axis1); i1<=gridSerial.getBound(axis1); i1++ )
	r(i1,R2,R3,0)=(i1-gridSerial.getBase(axis1))*dr[axis1];
      for( i2=gridSerial.getBase(axis2); i2<=gridSerial.getBound(axis2); i2++ )
	r(R1,i2,R3,1)=(i2-gridSerial.getBase(axis2))*dr[axis2];
      if( domainDimension>2 )
      {
	for( i3=gridSerial.getBase(axis3); i3<=gridSerial.getBound(axis3); i3++ )
	  r(R1,R2,i3,2)=(i3-gridSerial.getBase(axis3))*dr[axis3];
      }
    }

    mapGridS(r,gridSerial,Overture::nullRealArray(),params);

    if( Mapping::debug & 16 )
    {
      ::display(r,"getGridSerial: r","%5.2f ");
      ::display(gridSerial,"getGridSerial: gridSerial","%5.2f ");
    }
    
    // enforce periodicity exactly
    Range xAxes(0,rangeDimension-1);
    if( getIsPeriodic(axis1)==Mapping::functionPeriodic )
      gridSerial(R1.getBound(),R2,R3,xAxes)=gridSerial(R1.getBase(),R2,R3,xAxes);
    if( domainDimension>1 &&  getIsPeriodic(axis2)==Mapping::functionPeriodic )
      gridSerial(R1,R2.getBound(),R3,xAxes)=gridSerial(R1,R2.getBase(),R3,xAxes);
    if( domainDimension>2 &&  getIsPeriodic(axis3)==Mapping::functionPeriodic )
      gridSerial(R1,R2,R3.getBound(),xAxes)=gridSerial(R1,R2,R3.getBase(),xAxes);

//     for( axis=0; axis<rangeDimension; axis++ )
//     {
//       setRangeBound(Start,axis,min(gridSerial(R1,R2,R3,axis)));
//       setRangeBound(End  ,axis,max(gridSerial(R1,R2,R3,axis)));
//     }
    real xMin[3], xMax[3];
    getGridMinAndMax(gridSerial,R1,R2,R3,xMin,xMax);
    for( int axis=0; axis<rangeDimension; axis++ )
    {
      setRangeBound(Start,axis,xMin[axis]);
      setRangeBound(End  ,axis,xMax[axis]);
    }

    // determine the sign of the jacobian if domainDimension==rangeDimension
    getSignForJacobian();
    remakeGridSerial=false;
  }
  else
  {
    if( Mapping::debug & 4 )
      printF("Mapping:getGridSerial called, use existing grid\n");
  }
  
  return gridSerial;
#endif
}


void Mapping::
setMinimumNumberOfDistributedGhostLines( int numGhost )
// ==========================================================================
/// \details 
///  On Parallel machines always add at least this many parallel ghost lines on the grid array.
//==========================================================================
{
  minimumNumberOfDistributedGhostLines=numGhost;
}

int Mapping::
setNumberOfGhostLines( IndexRangeType & numberOfGhostLinesNew )
// ===========================================================================================
/// \details 
///     Specify the number of ghost lines.
/// \param numberOfGhostLinesNew(side,axis) : specify the number of ghostlines.
// ===========================================================================================
{
  bool numberOfGhostLinesHasChanged=false;
  int side,axis;
  for( axis=0; axis<domainDimension; axis++ )
  {
    for( side=0; side<=1; side++ )
    {
      if( numberOfGhostLinesNew(side,axis)!= numberOfGhostPoints(side,axis) )
	numberOfGhostLinesHasChanged=TRUE;
    }
  }
  if( numberOfGhostLinesHasChanged )
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
  return 0;
}


void Mapping::
setNumberOfDistributedGhostLines( int numGhost )
// ==========================================================================
/// \details 
///  Specify the number of parallel ghost lines to use for this mapping (on the "grid" array)
//==========================================================================
{
  numberOfDistributedGhostLines=numGhost;
}



void Mapping::
initializePartition()
// =======================================================================================
// /Description:
//   Initialize the partition object.
// =======================================================================================
{
  int debug=0;
  if( !partitionInitialized )
  {
    partitionInitialized=true;

    const int myid=max(0,Communication_Manager::My_Process_Number);
    if( debug & 1 )
      printf("***** myid=%i Mapping:: initialize the partition with (internal) address %d ***** \n",
              myid,partition.getInternalPartitioningObject());

    partition.SpecifyDecompositionAxes(domainDimension);
    for( int axis=0; axis<domainDimension; axis++)
    {
      int numGhost=max(Mapping::minimumNumberOfDistributedGhostLines,numberOfDistributedGhostLines);
      // set partition axes and number of ghost line boundaries
      if( debug & 1 )
        printf("****Mapping::initializePartition(): myid=%i, numGhost=%i ***\n",myid,numGhost);
      
      partition.partitionAlongAxis(axis, true, numGhost ); 
    }
    for( int axis=domainDimension; axis<MAX_ARRAY_DIMENSION; axis++)
      partition.partitionAlongAxis(axis, false, 0);
  }

  
}



void Mapping::
setGrid(realArray & grid_, IntegerArray & gridIndexRange_ )
// ================================================================================
/// \details 
///     Provide the grid to be used for plotting and the inverse. The MappedGrid, for example,
///   can provide the vertex array to use and thus this array can be shared (to save space). 
///  
/// \param grid_ (input) : grid points with optional ghost points.
/// \param gridIndexRange_(0:1,0:2) : grid index range -- this array indicates the index values
///    in the array grid that corresponding to the domai boundaries. Values outside of these
///  index bounds are this ghost points. For now gridIndexRange\_(0,0:2) should always be zero.
///  
// ================================================================================
{
  if( true )
  {
    if( false || Mapping::debug & 4 ) 
      printF("Mapping:setGrid called for mapping %s\n",(const char*)getName(mappingName));

   partition=grid_.getPartition();
   partitionInitialized=true;

   grid.reference(grid_);

   remakeGridSerial=false;
   #ifdef USE_PPP 
   if( usesDistributedInverse() )
   { // if the inverse is distributed then gridSerial becomes the local array:
     getLocalArrayWithGhostBoundaries(grid,gridSerial);  
   }
   else
   {
     remakeGridSerial=true; // *wdh* 220630 -- we need to re-make the serial grid which is used for the parallel inverse
     // getGridSerial(); // only do this on demand
   }
   #endif
   Range R[3];
   for( int axis=0; axis<3; axis++ )
   {
     for( int side=0; side<=1; side++ )
     {
       gridIndexRange(side,axis)=gridIndexRange_(side,axis);
     }

     // assert( gridIndexRange(0,axis)==0 );  // enforce this for now
  
     numberOfGhostPoints(0,axis)=gridIndexRange(0,axis)-grid.getBase(axis);
     numberOfGhostPoints(1,axis)=grid.getBound(axis)-gridIndexRange(1,axis);
     assert( numberOfGhostPoints(0,axis)>=0 );
     assert( numberOfGhostPoints(1,axis)>=0 );
  
     R[axis]= Range(gridIndexRange(0,axis),gridIndexRange(1,axis));
   }
   
   if( false )
     printf("YYYYY Mapping:setGrid called for mapping %s numGhost=[%i,%i]\n",(const char*)getName(mappingName),
	    numberOfGhostPoints(0,0),numberOfGhostPoints(1,0) );

   // set Range bounds
   real xMin[3], xMax[3];
   getGridMinAndMax(grid,R[0],R[1],R[2],xMin,xMax);

   if( false )
   {
     printF("Mapping::setGrid: grid x-bounds =[%8.2e,%8.2e][%8.2e,%8.2e][%8.2e,%8.2e]\n",xMin[0],xMax[0],xMin[1],xMax[1],xMin[2],xMax[2]);
   }
   
   for( int axis=0; axis<rangeDimension; axis++ )
   {
//     setRangeBound(Start,axis,min(grid(R[0],R[1],R[2],axis)));
//     setRangeBound(End  ,axis,max(grid(R[0],R[1],R[2],axis)));
     setRangeBound(Start,axis,xMin[axis]);
     setRangeBound(End  ,axis,xMax[axis]);
   }

   getSignForJacobian();
   remakeGrid=false;

   // *wdh* 091130 The AGI is based on the grid so we need to reinitialize (needed for ogmg "valve" for e.g.)
   if (approximateGlobalInverse != NULL )
   {
     //      printF("GGG Mapping::setGrid for %s gid=[%i,%i][%i,%i]\n",
     // 	    (const char*)getName(mappingName),gridIndexRange(0,0),gridIndexRange(1,0),
     //             (domainDimension>1 ? gridIndexRange(0,1) : 0),
     //             (domainDimension>1 ? gridIndexRange(1,1) : 0));
     approximateGlobalInverse->reinitialize();
   }
   
  }
  
}


int Mapping::
getInvertible() const 
// =====================================================================================
/// \details 
// =====================================================================================
{ return invertible; }

Mapping::periodicType Mapping::
getIsPeriodic( const int axis ) const
// =====================================================================================
/// \details 
/// \param axis (input): axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{ 
  int retval=0;
  if( validAxis( axis ) )
    retval = isPeriodic[axis];
  else
  {
    printF(" Mapping::getIsPeriodic: Invalid arguments: axis=%i\n",axis);
    Overture::abort("error");
  }
  return periodicType(retval);
}

Mapping::mappingCoordinateSystem Mapping::
getMappingCoordinateSystem() const
// =====================================================================================
/// \details 
// =====================================================================================
{
  return mappingCoordinateSystem0;
}

aString Mapping::
getName( const mappingItemName item ) const
// =====================================================================================
/// \details 
///     Return a name from enum mappingItemName:
///   <ul>
///     <li> <B>mappingName</B> : mapping name
///     <li> <B>domainName</B> : domain name
///     <li> <B>rangeName</B> :
///     <li> <B>domainAxis1Name</B> : names for coordinate axes in domain
///     <li> <B>domainAxis2Name</B> : 
///     <li> <B>domainAxis3Name</B> : 
///     <li> <B>rangeAxis1Name</B> : names for coordinate axes in range
///     <li> <B>rangeAxis2Name</B> : 
///     <li> <B>rangeAxis3Name</B> :
///   </ul>
/// \param item (input): return the name of this item.
// =====================================================================================
{
  return namestr[item] ;
}

real Mapping::
getParameter( const MappingParameters::realParameter & param ) const
// =====================================================================================
/// \details 
///    Return the value of a parameter used by the Mapping or the ApproximateGlobalInverse or the ExactLocalInverse.
///  
///  <ul>
///   <li> <B>THEnonConvergenceValue</B> : value given to "r" value of the inverse when there is no convergence. This is
///               currently equal to 10. and cannot be changed.
///   <li> <B>THEnewtonToleranceFactor</B> : convergence tolerance is this times the machine epsilon. Default=100. ?
///   <li> <B>THEnewtonDivergenceValue</B> : newton is deemed to have diverged if the r value is this much outside [0,1].
///       The default value is .1 and so Newton is deemed to have diverged when the r value is outside the range 
///       [-.1,1.1]
///   <li> <B>THEnewtonL2Factor</B> : extra factor for finding the closest point to a curve or surface, default=.1.
///           This factor allows a less strict convergence factor if the target point is far from the mapping.
///           Decrease this value if you want a more accurate answer. You may also have to decrease this value
///           for mappings that have poor parameterizations. 
///   <li> <B>THEboundingBoxExtensionFactor</B> : relative amount to increase the bounding box each direction. The bounding
///      box can be increased in size to allow the inverse function to still converge for nearby points. The default
///      value is $.01$. ***Actually*** only the bounding boxes for the highest leaves in the bounding box tree
///      are extended by this factor. The bounding boxes for all other nodes (and the root) are just computed
///      from the size of the bounding boxes of the two leaves of the node.
///   <li> <B>THEstencilWalkBoundingBoxExtensionFactor</B> : The stencil walk routine that finds the closest point
///      before inversion by Newton's method will only find the closest point if the point lies in a box
///      that is equal to the bounding box extended by this factor in each direction. Default =.2
///  </ul>
/// 
// =====================================================================================
{ 
  real returnValue;
  switch (param)
  {
  case MappingParameters::THEnonConvergenceValue:    // value given to inverse when there is no convergence
  case MappingParameters::THEnewtonToleranceFactor:  // convergence tolerance is this times the machine epsilon
  case MappingParameters::THEnewtonL2Factor: 
  case MappingParameters::THEnewtonDivergenceValue:  // newton is deemed to have diverged if the r value is this much outside [0,1]
    assert( exactLocalInverse!=NULL );
    returnValue=exactLocalInverse->getParameter(param);
    break;
  case MappingParameters::THEboundingBoxExtensionFactor:
    assert( approximateGlobalInverse!=NULL );
    returnValue=approximateGlobalInverse->getParameter(param);
    break;
  case  MappingParameters::THEstencilWalkBoundingBoxExtensionFactor:
    assert( approximateGlobalInverse!=NULL );
    returnValue=approximateGlobalInverse->getParameter(param);
  default:
    cout << " Mapping::getParameter: fatal error, unkown value for realParameter\n";
    Overture::abort("error");
  }
  return returnValue;
}

int Mapping::
getParameter( const MappingParameters::intParameter & param ) const
// =====================================================================================
/// \details 
///    Set the value of a parameter used by the Mapping or the ApproximateGlobalInverse or the ExactLocalInverse.
///  
///  <ul>
///   <li> <B>THEfindBestGuess</B> : if true, always find the closest point, even if the point to be inverted
///     is outside the bounding box. Default value is false.
///  </ul>
/// 
///  
// =====================================================================================
{ 
  int returnValue=0;
  switch (param)
  {
  case  MappingParameters::THEfindBestGuess:
    assert( approximateGlobalInverse!=NULL );
    returnValue = approximateGlobalInverse->getParameter(param);
    break;
  default:
    cout << " Mapping::getParameter: fatal error, unknown value for realParameter\n";
    Overture::abort("error");
  }
  return returnValue;
}

real Mapping::
getPeriodVector(const int axis, const int direction ) const
// =====================================================================================
/// \details 
///   For a mapping with getIsPeriodic(direction)==derivativePeriodic this routine returns
///  the vector that determines the shift from the `left' edge to the `right' edge.
/// \param axis (input): axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<rangeDimension$, 
///      are the components of the vector.
/// \param direction (input) : direction =0,1,...,domainDimension
// =====================================================================================
{ 
  real retval = 0.;
  if( axis >= 0 && axis < rangeDimension 
     && direction >= 0 && direction < domainDimension )
    retval =periodVector[axis][direction];
  else
    cout << " Mapping::getPeriodVector: Invalid arguments " << endl;
  return retval;
}

Bound Mapping::
getRangeBound( const int side, const int axis ) const
// =====================================================================================
/// \details 
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{ 
  if( validSide( side ) && axis>=0 && axis<rangeDimension )
    return rangeBound[axis][side];
  else
  { 
    cout << " Mapping::getRangeBound: Invalid arguments, (side,axis)=(" << side << "," << axis << ")\n";
    return 0;
  }
}

Mapping::coordinateSystem Mapping::
getRangeCoordinateSystem() const 
// =====================================================================================
/// \details 
// =====================================================================================
{ return rangeCoordinateSystem; }


Bound Mapping::
getRangeCoordinateSystemBound( const int side, const int axis ) const
// =====================================================================================
/// \details 
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{ 
  if( validSide( side ) && axis>=0 && axis<rangeDimension )
    return rangeCoordinateSystemBound[axis][side];
  else
    { cout << " Mapping::getRangeCoordinateSystemBound: Invalid arguments, (side,axis)=(" << side 
           << "," << axis << ")\n";
      return 0;
    }
}

int Mapping::
getRangeDimension() const 
// =====================================================================================
/// \details 
// =====================================================================================
{ return rangeDimension; }    



Mapping::mappingSpace Mapping::
getRangeSpace() const 
// =====================================================================================
/// \details 
// =====================================================================================
{ return rangeSpace; }



int Mapping::
getShare( const int side, const int axis ) const
// =====================================================================================
/// \details 
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{ 
  int retval=0;
  if( validSide( side ) && (axis>=0 && axis<rangeDimension) )
    retval = share[side][axis];
  else
    cout << " Mapping::getShare: Invalid arguments " << endl;
  return retval;
}



real Mapping::
getSignForJacobian() const
// =====================================================================================
/// \details 
///    Return the sign of the jacobian, 1 (right handed coordinate system) or -1 (left handed).
///   This may only make sense for some mappings.
// =====================================================================================
{
  // if( remakeGrid )
  if( signForJacobian==0. ) // *wdh* 030710
  {
    if( domainDimension==rangeDimension && domainDimension>1 )
    {
      RealArray rr(1,3),xx(1,3),xr(1,3,3);

      rr=.5;  // check this point
      real & sign = (real&)signForJacobian; // cast away const
      for( int it=0; it<2; it++ )
      {
        #ifdef USE_PPP
  	  ((Mapping*) this)->mapS(rr,xx,xr);  // cast away const
        #else
  	  ((Mapping*) this)->map(rr,xx,xr);  // cast away const
        #endif

        sign=0.;
	if( domainDimension==2 )
	  sign=xr(0,0,0)*xr(0,1,1)-xr(0,1,0)*xr(0,0,1);
	else
	  sign=( (xr(0,0,0)*xr(0,1,1)-xr(0,0,1)*xr(0,1,0))*xr(0,2,2) +
		 (xr(0,0,1)*xr(0,1,2)-xr(0,0,2)*xr(0,1,1))*xr(0,2,0) +
		 (xr(0,0,2)*xr(0,1,0)-xr(0,0,0)*xr(0,1,2))*xr(0,2,1) );
        // printf(" Mapping::getSignForJacobian: BEFORE: sign=%e (p=%i)\n",sign,myid);

	if( sign==0. )
	{
          if( it==0 )
	  {
	    printf("Mapping::WARNING:Mapping=%s The determinant of the jacobian at r=(%8.2e,%8.2e,%8.2e) is zero\n"
		   "     when computing signForJacobian. Will try another point...\n",
                        (const char*)getName(mappingName),rr(0,0),rr(0,1),rr(0,2));
	    rr(0,0)=.25;
	    rr(0,1)=.3;
	    rr(0,2)=.4;
	  }
	  else
	  {
	    printf("     ..,Mapping=%s The det of the jacobian at r=(%8.2e,%8.2e,%8.2e) "
		   " is STILL zero. Will choose sign=+1\n",
                        (const char*)getName(mappingName),rr(0,0),rr(0,1),rr(0,2));
	  }
	  
	}
        else
	{
          if( it>0 )
	  {
	    printf("    ...Mapping=%s The det of the jacobian at r=(%8.2e,%8.2e,%8.2e) = %8.2e is not zero!\n",
		   (const char*)getName(mappingName),rr(0,0),rr(0,1),rr(0,2),sign);
             
	  }
	  break;
	}
      }

      sign= sign<0. ? -1. : 1.;
    }
  }
  return signForJacobian;
}

// old way:
// real Mapping::
// getSignForJacobian() const
// // =====================================================================================
// // /Description:
// //   Return the sign of the jacobian, 1 (right handed coordinate system) or -1 (left handed).
// //  This may only make sense for some mappings.
// //\end{MappingInclude.tex}
// // =====================================================================================
// {
//   // if( remakeGrid )
//   if( signForJacobian==0. ) // *wdh* 030710
//   {
//     if( domainDimension==rangeDimension && domainDimension>1 )
//     {
//       const int myid=max(0,Communication_Manager::My_Process_Number);
//       const int processor=0;  // only compute the sign on this processor
//       realArray rr,xx,xr;
//       #ifdef USE_PPP
//         Partitioning_Type partition(Range(processor,processor)); 
//         partition.SpecifyDecompositionAxes(0);
// 	rr.partition(partition);
// 	xx.partition(partition);
//         xr.partition(partition);
//       #endif
//       rr.redim(1,3), xx.redim(1,3), xr.redim(1,3,3);
//       #ifdef USE_PPP
//         const realSerialArray & xrLocal=xr.getLocalArray();
//       #else
//         realSerialArray & xrLocal=xr; 
//       #endif

//       rr=.5;  // check this point
//       real & sign = (real&)signForJacobian; // cast away const
//       for( int it=0; it<2; it++ )
//       {
// 	((Mapping*) this)->map(rr,xx,xr);  // cast away const

//         sign=0.;
//         if( myid==processor )
// 	{
// 	  if( domainDimension==2 )
// 	    sign=xrLocal(0,0,0)*xrLocal(0,1,1)-xrLocal(0,1,0)*xrLocal(0,0,1);
// 	  else
// 	    sign=( (xrLocal(0,0,0)*xrLocal(0,1,1)-xrLocal(0,0,1)*xrLocal(0,1,0))*xrLocal(0,2,2) +
// 		   (xrLocal(0,0,1)*xrLocal(0,1,2)-xrLocal(0,0,2)*xrLocal(0,1,1))*xrLocal(0,2,0) +
// 		   (xrLocal(0,0,2)*xrLocal(0,1,0)-xrLocal(0,0,0)*xrLocal(0,1,2))*xrLocal(0,2,1) );
// 	}
//         // printf(" Mapping::getSignForJacobian: BEFORE: sign=%e (p=%i)\n",sign,myid);

// 	#ifdef USE_PPP  
// 	// sign=ParallelUtility::getSum(sign); // should do a broadcast here instead
//         // int MPI_Bcast ( void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm )
//   	  double value=sign;
//   	  MPI_Bcast ( &value, 1, MPI_DOUBLE, processor, MPI_COMM_WORLD );
//           sign=value;
//         #endif
// 	// printf(" Mapping::getSignForJacobian: AFTER: sign=%e (p=%i)\n",sign,myid);
	

// 	if( sign==0. )
// 	{
//           if( it==0 )
// 	  {
// 	    printf("Mapping::WARNING:Mapping=%s The determinant of the jacobian at r=(%8.2e,%8.2e,%8.2e) is zero\n"
// 		   "     when computing signForJacobian. Will try another point...\n",
//                         (const char*)getName(mappingName),rr(0,0),rr(0,1),rr(0,2));
// 	    rr(0,0)=.25;
// 	    rr(0,1)=.3;
// 	    rr(0,2)=.4;
// 	  }
// 	  else
// 	  {
// 	    printf("     ..,Mapping=%s The det of the jacobian at r=(%8.2e,%8.2e,%8.2e) "
// 		   " is STILL zero. Will choose sign=+1\n",
//                         (const char*)getName(mappingName),rr(0,0),rr(0,1),rr(0,2));
// 	  }
	  
// 	}
//         else
// 	{
//           if( it>0 )
// 	  {
// 	    printf("    ...Mapping=%s The det of the jacobian at r=(%8.2e,%8.2e,%8.2e) = %8.2e is not zero!\n",
// 		   (const char*)getName(mappingName),rr(0,0),rr(0,1),rr(0,2),sign);
             
// 	  }
// 	  break;
// 	}
//       }

//       sign= sign<0. ? -1. : 1.;
//     }
//   }
//   return signForJacobian;
// }



Mapping::coordinateSingularity Mapping::
getTypeOfCoordinateSingularity( const int side, const int axis  ) const
// =====================================================================================
/// \details 
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{
  if( validSide( side ) && validAxis( axis ) )
    return coordinateSingularity(typeOfCoordinateSingularity[side][axis]);
  else
  { cout << " Mapping::getTypeOfCoordinateSingularity: Invalid arguments " << endl;
    return noCoordinateSingularity;
  }
}





int Mapping::
hasACoordinateSingularity() const
// =====================================================================================
/// \details  return true if the Mapping has a coordinate singularity
// =====================================================================================
{
  for( int axis=0; axis<domainDimension; axis++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      if( typeOfCoordinateSingularity[side][axis]!=noCoordinateSingularity )
        return TRUE;
    }
  }
  return FALSE;
//  return max(abs(typeOfCoordinateSingularity(Range(0,1),Range(0,domainDimension-1))
//                               -Mapping::noCoordinateSingularity))!=0;
}

int Mapping::
intersects(Mapping & map2, 
	   const int & side1 /* =-1 */, 
	   const int & axis1 /* =-1 */,
	   const int & side2 /* =-1 */, 
	   const int & axis2 /* =-1 */,
           const real & tol /* = 0. */  ) const
//==========================================================================================
/// \details 
///     Determine one mapping (or a face of the mapping) intersects another mapping (or the face of another
///      mapping.
/// 
/// \param map2 (input):  check intersect with this Mapping.
/// \param side1,axis1 (input): Check this face of this mapping (by default check all faces).
/// \param side2,axis2 (input): Check this face of map2 (by default check all faces).
/// \param tol (input) : increase the the size of the bounding boxes by tol*(box size) when determining 
///    whether the mappings intersect. Thus choosing a value of $.1$ will cause the Mappings
///     to intersect provided they are close to each other while a value of -.1 will cause the
///     mappings to intersect only if they overlap sufficiently.
///  Return value : TRUE if the face (side,axis) of map intersects this mapping.
//===========================================================================================
{
  if( false )
  {
    // old way

      // make sure the bounding boxes are initialized
  approximateGlobalInverse->initialize();
  map2.approximateGlobalInverse->initialize();
  
  assert( rangeDimension==map2.getRangeDimension() );
  
  if( side1!=-1 && ( side1<0 || side1 > 1 ) )
  {
    printf("Mapping::intersects:ERROR: invalid value for side1=%i\n",side1);
    Overture::abort("error");
  }
  if( axis1!=-1 && ( axis1<0 || axis1 > getDomainDimension() ) )
  {
    printf("Mapping::intersects:ERROR: invalid value for axis1=%i\n",axis1);
    Overture::abort("error");
  }
  if( side2!=-1 && ( side2<0 || side2 > 1 ) )
  {
    printf("Mapping::intersects:ERROR: invalid value for side2=%i\n",side2);
    Overture::abort("error");
  }
  if( axis2!=-1 && ( axis2<0 || axis2 > map2.getDomainDimension() ) )
  {
    printf("Mapping::intersects:ERROR: invalid value for axis2=%i\n",axis2);
    Overture::abort("error");
  }
  
  real delta1_[6], delta2_[6];
#define delta(i,j) delta_[(i)+2*(j)]
#define delta1(i,j) delta1_[(i)+2*(j)]
#define delta2(i,j) delta2_[(i)+2*(j)]

  if( tol!=0. )
  { // increase the size of the box, assign the delta1, delta2 arrays to hold the amount to increase by,
    // these will be added to the box rangeBound's
    for( int i=0; i<=1; i++ )
    {
      RealArray & xBound = i==0 ? approximateGlobalInverse->boundingBox 
                                : map2.approximateGlobalInverse->boundingBox;
      real *delta_ = i==0 ? delta1_ : delta2_;
      for( int dir=0; dir<rangeDimension; dir++ )
      {
	real eps=(xBound(End,dir)-xBound(Start,dir))*tol;
	delta(Start,dir)=-eps;
	delta(End  ,dir)=+eps;
      }
    }
  }
  else
    {
      //kkc initialize delta to zero in case a tolerance is not specified
      for( int i=0; i<=1; i++ )
	{
	  real *delta_ = i==0 ? delta1_ : delta2_;
	  for( int dir=0; dir<rangeDimension; dir++ )
	    {
	      delta(Start,dir)=0.;
	      delta(End  ,dir)=0.;
	    }
	}
    }


  BoundingBox box1, box2;
  box1.setDimensions(domainDimension,rangeDimension);
  Range I2(0,1), R(0,rangeDimension-1), R2(0,map2.getRangeDimension()-1);
  if( axis1==-1 )
  { // make a box for the entire mapping
    assert( side1==-1 );
    for( int axis=0; axis<rangeDimension; axis++)
      for( int side=0; side<=1; side++ )
	box1.rangeBound(side,axis)=approximateGlobalInverse->boundingBox(side,axis)+delta1(side,axis);
  }
  box2.setDimensions(map2.getDomainDimension(),map2.getRangeDimension());
  if( axis2==-1 )
  { // make a box for the entire mapping
    assert( side2==-1 );
    // box2.setRangeBound(evaluate(map2.approximateGlobalInverse->boundingBox(I2,R2)+delta2));
    for( int axis=0; axis<rangeDimension; axis++)
      for( int side=0; side<=1; side++ )
	box2.rangeBound(side,axis)=map2.approximateGlobalInverse->boundingBox(side,axis)+delta2(side,axis);
  }
  for( int a1=axis1; a1<=axis1; a1++ )
  {
    for( int s1=side1; s1<=side1; s1++ )
    {
      if( axis1!=-1 )
      {
        //box1.setRangeBound(approximateGlobalInverse->boundingBoxTree[s1][a1].getRangeBound()(I2,R)+delta1);  
	for( int axis=0; axis<rangeDimension; axis++)
	  for( int side=0; side<=1; side++ )
	    box1.rangeBound(side,axis)=
                  approximateGlobalInverse->boundingBoxTree[s1][a1].rangeBound(side,axis)+delta1(side,axis);
      }
      for( int a2=axis2; a2<=axis2; a2++ )
      {
	for( int s2=side2; s2<=side2; s2++ )
	{
          if( axis2!=-1 )
	  {
//             box2.setRangeBound(evaluate(
//                 map2.approximateGlobalInverse->boundingBoxTree[s2][a2].getRangeBound()(I2,R2)+delta2));  
	    for( int axis=0; axis<rangeDimension; axis++)
	      for( int side=0; side<=1; side++ )
		box2.rangeBound(side,axis)=
                  map2.approximateGlobalInverse->boundingBoxTree[s2][a2].rangeBound(side,axis)+delta2(side,axis);
	  }
          if( box1.intersects( box2 ) )
	    return TRUE;
	}
      }
    }
  }
  return FALSE;
  }
  else
  {
    // new way

  // make sure the bounding boxes are initialized
//   approximateGlobalInverse->initialize();
//   map2.approximateGlobalInverse->initialize();
  
  assert( rangeDimension==map2.getRangeDimension() );
  
  if( side1!=-1 && ( side1<0 || side1 > 1 ) )
  {
    printf("Mapping::intersects:ERROR: invalid value for side1=%i\n",side1);
    Overture::abort("error");
  }
  if( axis1!=-1 && ( axis1<0 || axis1 > getDomainDimension() ) )
  {
    printf("Mapping::intersects:ERROR: invalid value for axis1=%i\n",axis1);
    Overture::abort("error");
  }
  if( side2!=-1 && ( side2<0 || side2 > 1 ) )
  {
    printf("Mapping::intersects:ERROR: invalid value for side2=%i\n",side2);
    Overture::abort("error");
  }
  if( axis2!=-1 && ( axis2<0 || axis2 > map2.getDomainDimension() ) )
  {
    printf("Mapping::intersects:ERROR: invalid value for axis2=%i\n",axis2);
    Overture::abort("error");
  }
  
  real delta1_[6], delta2_[6];
#define delta(i,j) delta_[(i)+2*(j)]
#define delta1(i,j) delta1_[(i)+2*(j)]
#define delta2(i,j) delta2_[(i)+2*(j)]

  if( tol!=0. )
  { // increase the size of the box, assign the delta1, delta2 arrays to hold the amount to increase by,
    // these will be added to the box rangeBound's
    RealArray xBound;
    for( int i=0; i<=1; i++ )
    {
      // RealArray & xBound = i==0 ? approximateGlobalInverse->boundingBox : 
      //                        map2.approximateGlobalInverse->boundingBox;
      if( i==0 )
	xBound = getBoundingBox();
      else
        xBound = map2.getBoundingBox();
      
      real *delta_ = i==0 ? delta1_ : delta2_;
      for( int dir=0; dir<rangeDimension; dir++ )
      {
	real eps=(xBound(End,dir)-xBound(Start,dir))*tol;
	delta(Start,dir)=-eps;
	delta(End  ,dir)=+eps;
      }
    }
  }

  RealArray bb1, bb2;
  bb1=getBoundingBox(side1,axis1);
  bb2=map2.getBoundingBox(side2,axis2);
  if( tol!=0. )
  {
    for( int axis=0; axis<rangeDimension; axis++)
    {
      for( int side=0; side<=1; side++ )
      {
	bb1(side,axis)+=delta1(side,axis);
	bb2(side,axis)+=delta2(side,axis);
      }
    }
  }
  
  bool boxesIntersect=false;
  switch (rangeDimension)
  {
  case 1:
    boxesIntersect= bb1(1,0)>=bb2(0,0) && bb2(1,0)>=bb1(0,0);
    break;
  case 2:
    boxesIntersect= (bb1(1,0)>=bb2(0,0) && bb2(1,0)>=bb1(0,0) &&
		     bb1(1,1)>=bb2(0,1) && bb2(1,1)>=bb1(0,1));
    break;
  case 3:
    boxesIntersect= (bb1(1,0)>=bb2(0,0) && bb2(1,0)>=bb1(0,0) &&
		     bb1(1,1)>=bb2(0,1) && bb2(1,1)>=bb1(0,1) &&
		     bb1(1,2)>=bb2(0,2) && bb2(1,2)>=bb1(0,2));
    break;
  default:
    cout << "Mapping::intersects: invalid rangeDimension = " << rangeDimension << endl;
  }
  return boxesIntersect;
    
  
  }
  

//   BoundingBox box1, box2;
//   box1.setDimensions(domainDimension,rangeDimension);
//   box2.setDimensions(map2.getDomainDimension(),map2.getRangeDimension());
//   Range I2(0,1), R(0,rangeDimension-1), R2(0,map2.getRangeDimension()-1);
//   if( axis1==-1 )
//   { // make a box for the entire mapping
//     assert( side1==-1 );
//     for( int axis=0; axis<rangeDimension; axis++)
//       for( int side=0; side<=1; side++ )
// 	box1.rangeBound(side,axis)=approximateGlobalInverse->boundingBox(side,axis)+delta1(side,axis);
//   }
//   if( axis2==-1 )
//   { // make a box for the entire mapping
//     assert( side2==-1 );
//     // box2.setRangeBound(evaluate(map2.approximateGlobalInverse->boundingBox(I2,R2)+delta2));
//     for( int axis=0; axis<rangeDimension; axis++)
//       for( int side=0; side<=1; side++ )
// 	box2.rangeBound(side,axis)=map2.approximateGlobalInverse->boundingBox(side,axis)+delta2(side,axis);
//   }
//   for( int a1=axis1; a1<=axis1; a1++ )
//   {
//     for( int s1=side1; s1<=side1; s1++ )
//     {
//       if( axis1!=-1 )
//       {
//         //box1.setRangeBound(approximateGlobalInverse->boundingBoxTree[s1][a1].getRangeBound()(I2,R)+delta1);  
// 	for( int axis=0; axis<rangeDimension; axis++)
// 	  for( int side=0; side<=1; side++ )
// 	    box1.rangeBound(side,axis)=
//                   approximateGlobalInverse->boundingBoxTree[s1][a1].rangeBound(side,axis)+delta1(side,axis);
//       }
//       for( int a2=axis2; a2<=axis2; a2++ )
//       {
// 	for( int s2=side2; s2<=side2; s2++ )
// 	{
//           if( axis2!=-1 )
// 	  {
// //             box2.setRangeBound(evaluate(
// //                 map2.approximateGlobalInverse->boundingBoxTree[s2][a2].getRangeBound()(I2,R2)+delta2));  
// 	    for( int axis=0; axis<rangeDimension; axis++)
// 	      for( int side=0; side<=1; side++ )
// 		box2.rangeBound(side,axis)=
//                   map2.approximateGlobalInverse->boundingBoxTree[s2][a2].rangeBound(side,axis)+delta2(side,axis);
// 	  }
//           if( box1.intersects( box2 ) )
// 	    return TRUE;
// 	}
//       }
//     }
//   }
//   return FALSE;

}
#undef delta
#undef delta1
#undef delta2


void Mapping::
inverseMap(const realArray & x_, 
	   realArray & r_, 
	   realArray & rx_ /* =nullDistributedArray */,
	   MappingParameters & params  /* =Overture::nullMappingParameters() */)
// =====================================================================================
/// \details 
///   --- Here is the generic inverse ----
/// 
/// \param x (input) : invert these points. The dimensions of this array will determine which
///      points are inverted.
/// \param r (input/output) : On input this is an initial guess. If you know a reasonable initial
///    guess then supply it, If you don't know an initial guess
///     then set r=-1. for those points that you do not know a guess. If you do not know a guess
///      then do NOT specify some valid value like .5 since this will probably be slower than allowing
///      the value to be automatically generated.
/// \param rx (output): the derivatives of the inverse mapping.
/// \param params (input) :
///    <ul>
///      <li>[params.computeGlobalInverse] : TRUE means compute a full global inverse,
///        FALSE means only compute a local inverse using the initial guess supplied in r
///      <li>[params.periodicityOfSpace] : 
///      <li>[params.periodVector] : 
///    </ul>
//=================================================================================
{

  if( debug & 8 )
    cout << getName(mappingName) << ": inverseMap - params.computeGlobalMap=" 
         << params.computeGlobalInverse << endl;

  #ifdef USE_PPP
    RealArray x;  getLocalArrayWithGhostBoundaries(x_,x);
    RealArray r;  getLocalArrayWithGhostBoundaries(r_,r);
    RealArray rx; getLocalArrayWithGhostBoundaries(rx_,rx);
  #else
    const RealArray & x =x_;
    RealArray & r =r_;
    RealArray & rx =rx_;
  #endif

  if( basicInverseOption==canInvert && params.periodicityOfSpace==0 )
  {
    #ifndef USE_PPP
      basicInverse( x,r,rx,params );  // can use user supplied inverse
    #else
      basicInverseS( x,r,rx,params );
    #endif
    return;
  }


  MappingWorkSpace workSpace; 
  if( params.computeGlobalInverse )
  {
    // first get the initial guess
    approximateGlobalInverse->inverse( x,r,rx,workSpace,params );

    // Now do Newton to Invert:
    exactLocalInverse->inverse( x,r,rx,workSpace,TRUE );   // TRUE means use results found in the
                                                           // workSpace
  }
  else
  {
    exactLocalInverse->inverse( x,r,rx,workSpace,FALSE );
  }

  
}

// **************** moved to inverseMap.C **************
// // *serial-array version*
// void Mapping::
// inverseMapS( const RealArray & x, 
//              RealArray & r, 
// 	     RealArray & rx /* =Overture::nullRealArray() */,
// 	     MappingParameters & params /* =Overture::nullMappingParameters() */ )
// // =====================================================================================
// // /Description:
// //  --- Here is the generic inverse ----
// //
// // /x (input) : invert these points. The dimensions of this array will determine which
// //     points are inverted.
// // /r (input/output) : On input this is an initial guess. If you know a reasonable initial
// //   guess then supply it, If you don't know an initial guess
// //    then set r=-1. for those points that you do not know a guess. If you do not know a guess
// //     then do NOT specify some valid value like .5 since this will probably be slower than allowing
// //     the value to be automatically generated.
// // /rx (output): the derivatives of the inverse mapping.
// //  /params (input) :
// //   \begin{description}
// //     \item[params.computeGlobalInverse] : TRUE means compute a full global inverse,
// //       FALSE means only compute a local inverse using the initial guess supplied in r
// //     \item[params.periodicityOfSpace] : 
// //     \item[params.periodVector] : 
// //   \end{description}
// //\end{MappingInclude.tex}
// //=================================================================================
// {
// 
//   if( debug & 8 )
//     cout << getName(mappingName) << ": inverseMapS - params.computeGlobalMap=" 
//          << params.computeGlobalInverse << endl;
// 
//   if( basicInverseOption==canInvert && params.periodicityOfSpace==0 )
//   {
//     basicInverseS( x,r,rx,params );  // can use user supplied inverse
//     return;
//   }
// 
//   MappingWorkSpace workSpace; 
//   if( params.computeGlobalInverse )
//   {
//     // first get the initial guess
//     approximateGlobalInverse->inverse( x,r,rx,workSpace,params );
// 
//     // Now do Newton to Invert:
//     exactLocalInverse->inverse( x,r,rx,workSpace,TRUE );   // TRUE means use results found in the
//                                                            // workSpace
//   }
//   else
//   {
//     exactLocalInverse->inverse( x,r,rx,workSpace,FALSE );
//   }
// 
// }
// 
// ******************************************** 

void Mapping::
inverseMapC(const realArray & x, 
	    const realArray & r, 
	    const realArray & rx /* =nullDistributedArray */, 
	    MappingParameters & params /* =Overture::nullMappingParameters() */)
// =====================================================================================
/// \details 
///    This version of inverseMap defines x and xr to be const (even though they really aren't).
///   It can be used for some compilers (IBM:xlC) that don't like passing
///    views of arrays to non-const references, as in mapping.inverseMapC(r(I),x(I),xr(I))
// =====================================================================================
{
  inverseMap(x,(realArray&)r,(realArray&)rx,params); // cast away const
}


// *serial-array version*
void Mapping::
inverseMapCS(const RealArray & x, 
	    const RealArray & r, 
	    const RealArray & rx /* =nullRealArray */, 
	    MappingParameters & params /* =Overture::nullMappingParameters() */)
// =====================================================================================
// /Description:
//   This version of inverseMap defines x and xr to be const (even though they really aren't).
//  It can be used for some compilers (IBM:xlC) that don't like passing
//   views of arrays to non-const references, as in mapping.inverseMapC(r(I),x(I),xr(I))
//\end{MappingInclude.tex}
// =====================================================================================
{
  inverseMapS(x,(RealArray&)r,(RealArray&)rx,params); // cast away const
}


void Mapping::
inverseMapGrid(const realArray & x, 
	       realArray & r, 
	       realArray & rx /* =nullDistributedArray */,
	       MappingParameters & params /* =Overture::nullMappingParameters() */  )
// =====================================================================================
/// \details  inverseMap a grid of points.
///  
///  This version of inverseMap assumes that the input array is of the form of a grid of points:
///  \begin{verbatim}
///    if rangeDimension==1 then x can be of the form
///         x(a1:a2,0:d-1)             
///         x(a1:a2,0:0,0:d-1)        
///         x(a1:a2,0:0,0:0,0:d-1)   
///    if rangeDimension==2 then x can be of the form
///         x(a1:a2,b1:b2,0:d-1)             
///         x(a1:a2,b1:b2,0:0,0:d-1)      
///    if rangeDimension==3 then x can be of the form
///         x(a1:a2,b1:b2,c1:c2,0:d-1)      
/// 
///  \end{verbatim}
///  The output is in a similar form
/// 
/// \param x (input) : evaluate the inverse mapping at these points, where
/// \param r  (input/output) : if r has enough space, then compute the inverse mapping. You must supply an initial guess.
///        Choose r=-1. if you don't know a good guess. 
/// \param rx (output) : if rx has enough space, then  compute the derivatives of the inverse mapping.
/// \param params (input/output) : holds parameters for the mapping.
// =====================================================================================
{
  int positionOfDomainDimension=max(1,r.numberOfDimensions()-1);
  int positionOfRangeDimension =max(1,x.numberOfDimensions()-1); 

  int axis;
  if( x.getLength(positionOfRangeDimension)!=rangeDimension )
  {
    positionOfRangeDimension=-1;
    for( axis=rangeDimension+1; axis<4; axis++ )
      if( x.getLength(axis)==rangeDimension )
      {
        positionOfRangeDimension=axis;
        break;
      }
  }
  if( positionOfRangeDimension==-1 )
  {
    cout << "Mapping::inverseMapGrid:ERROR: array x is not of the expected shape \n";
    printf(" mappingName=%s, domainDimension=%i, rangeDimension=%i\n",(const char*)getName(mappingName),
	   domainDimension,rangeDimension);
    printf("dimensions of x are [%i,%i]x[%i,%i]x[%i,%i]x[%i,%i]\n",
	   x.getBase(0),x.getBound(0),x.getBase(1),x.getBound(1),
           x.getBase(2),x.getBound(2),x.getBase(3),x.getBound(3));
    printf("I was expecting x to have a dimension [0,domainDimension-1] = [0,%i]\n",domainDimension-1);
    
    Overture::abort("error");
  }

  getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );


  Range Rx[4], Rr[4], Rrx[4];
  int dim[3] = {1,1,1};
  for( axis=0; axis<positionOfRangeDimension; axis++ )
    dim[axis]=x.getLength(axis);

  for( axis=0; axis<4; axis++ )
  {
    Rx [axis]=Range( x.getBase(axis), x.getBound(axis));
    Rr [axis]=Range( r.getBase(axis), r.getBound(axis));
    Rrx[axis]=Range(rx.getBase(axis),rx.getBound(axis));
  }
  
  realArray & xx = (realArray &)x; // cast away const so we can reshape

  xx.reshape(dim[0]*dim[1]*dim[2],rangeDimension);
  if( computeMap )
    r.reshape(dim[0]*dim[1]*dim[2],Rr[positionOfDomainDimension].length());
  if( computeMapDerivative )
    rx.reshape(dim[0]*dim[1]*dim[2],domainDimension,rangeDimension);

  inverseMap(x,r,rx,params); // evaluate the inverse mapping

  xx.reshape(Rx[0],Rx[1],Rx[2],Rx[3]);
  if( computeMap )
    r.reshape(Rr[0],Rr[1],Rr[2],Rr[3]);
  if( computeMapDerivative )
    rx.reshape(Rrx[0],Rrx[1],Rrx[2],Rrx[3]);
}


// *serial-array version*
void Mapping::
inverseMapGridS(const RealArray & x, 
		RealArray & r, 
		RealArray & rx /* =Overture::nullRealArray() */,
		MappingParameters & params /* =Overture::nullMappingParameters() */ )
{
  Overture::abort("Mapping:inverseMapGridS:ERROR - finish this");
}



Mapping *Mapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the className is the name
  // of this Class
  Mapping *retval=0;
  if( mappingClassName==className )
  {
    retval = new Mapping();
    assert( retval != 0 );
  }
  return retval;
}


void Mapping::
map(const realArray & r, 
    realArray & x, 
    realArray & xr /* =nullDistributedArray */,
    MappingParameters & params /* =Overture::nullMappingParameters() */)
// =====================================================================================
/// \details 
///  Here is the transformation that defines the mapping.
/// 
/// \param r (input): r(base:bound,0:d) - evaluate the mapping at these points, where
///        d=domainDimension-1
/// \param x (output) : - if x has enough space, x(base:bound,0:r), then compute
///        the mapping. Here r=rangeDimension-1. Do not compute the mapping 
///        if x is not large enough
/// \param xr (output) : - if xr has enough space, xr(base:bound,0:r,0:d), then
///        compute the derivatives of the mapping.
/// \param params (input): - holds parameters for the mapping.
// =====================================================================================
{
  // Map the parameter space r to the physical space x
  printF("Mapping::map(r,x,xr) - ERROR base class function called! name=%s\n",
	 (const char*)getName(mappingName));
  Overture::abort("error");
}

// *serial-array version*
void Mapping::
mapS( const RealArray & r, 
      RealArray & x, 
      RealArray &xr /* = Overture::nullRealArray() */,
      MappingParameters & params /* =Overture::nullMappingParameters() */)
// =====================================================================================
// /Description:
// Here is the transformation that defines the mapping.
//
// /r (input): r(base:bound,0:d) - evaluate the mapping at these points, where
//       d=domainDimension-1
// /x (output) : - if x has enough space, x(base:bound,0:r), then compute
//       the mapping. Here r=rangeDimension-1. Do not compute the mapping 
//       if x is not large enough
// /xr (output) : - if xr has enough space, xr(base:bound,0:r,0:d), then
//       compute the derivatives of the mapping.
// /params (input): - holds parameters for the mapping.
//\end{MappingInclude.tex}
// =====================================================================================
{
  // Map the parameter space r to the physical space x
 #ifndef USE_PPP
  map(r,x,xr,params);
 #else
  printF("Mapping::mapS(r,x,xr) - ERROR base class function called! name=%s\n"
         "   The parallel version of this Mapping has likely not been implemented (fix me Bill!)\n"
         "   You may be able to convert this Mapping into a NurbsMapping which does work in parallel\n",
	 (const char*)getName(mappingName));
  OV_ABORT("error");
 #endif
}

void Mapping::
mapC(const realArray & r, 
     const realArray & x, 
     const realArray &xr /* =nullDistributedArray */, 
     MappingParameters & params /* =Overture::nullMappingParameters() */ )
// =====================================================================================
/// \details 
///    This version of map defines x and xr to be const (even though they really aren't).
///   It can be used for some compilers (IBM:xlC) that don't like passing
///    views of arrays to non-const references, as in mapping.mapC(r(I),x(I),xr(I))
// =====================================================================================
{
  map(r,(realArray&)x,(realArray&)xr,params); // cast away const
}


//  *serial-array version*
void Mapping::
mapCS(const RealArray & r, 
      const RealArray & x, 
      const RealArray &xr /* =nullRealArray */, 
      MappingParameters & params /* =Overture::nullMappingParameters() */ )
// =====================================================================================
// /Description:
//   This version of map defines x and xr to be const (even though they really aren't).
//  It can be used for some compilers (IBM:xlC) that don't like passing
//   views of arrays to non-const references, as in mapping.mapC(r(I),x(I),xr(I))
//\end{MappingInclude.tex}
// =====================================================================================
{
  mapS(r,(RealArray&)x,(RealArray&)xr,params); // cast away const
}



void Mapping::
mapGrid(const realArray & r, 
	realArray & x, 
	realArray & xr /* =nullDistributedArray */,
	MappingParameters & params /* =Overture::nullMappingParameters() */ )
// =====================================================================================
/// \details  Map a grid of points.
///  
///  This version of map assumes that the input array is of the form of a grid of points:
///  \begin{verbatim}
///    if domainDimension==1 then r can be of the form
///         r(a1:a2,0:d-1)             
///         r(a1:a2,0:0,0:d-1)        
///         r(a1:a2,0:0,0:0,0:d-1)   
///    if domainDimension==2 then r can be of the form
///         r(a1:a2,b1:b2,0:d-1)             
///         r(a1:a2,b1:b2,0:0,0:d-1)      
///    if domainDimension==3 then r can be of the form
///         r(a1:a2,b1:b2,c1:c2,0:d-1)      
/// 
///  \end{verbatim}
///  The output is in a similar form
/// 
/// \param r (input) : evaluate the mapping at these points, where
/// \param x  (output) : if x has enough space, then compute the mapping. 
/// \param xr (output) : if xr has enough space, then  compute the derivatives of the mapping.
/// \param params (input/output) : holds parameters for the mapping.
// =====================================================================================
{
  int positionOfDomainDimension=max(1,r.numberOfDimensions()-1);
  int positionOfRangeDimension =max(1,x.numberOfDimensions()-1); 

  int axis;
  if( r.getLength(positionOfDomainDimension)!=domainDimension )
  {
    positionOfDomainDimension=-1;
    for( axis=domainDimension+1; axis<4; axis++ )
      if( r.getLength(axis)==domainDimension )
      {
        positionOfDomainDimension=axis;
        break;
      }
  }
  if( positionOfDomainDimension==-1 )
  {
    cout << "Mapping::mapGrid:ERROR: array r is not of the expected shape \n";
    Overture::abort("error");
  }

/* ----
  if( x.getLength(positionOfRangeDimension)!=rangeDimension )
  {
    positionOfRangeDimension=-1;
    for( axis=rangeDimension+1; axis<4; axis++ )
      if( x.getLength(axis)==rangeDimension )
      {
        positionOfRangeDimension=axis;
        break;
      }
  }
  if( positionOfRangeDimension==-1 )
  {
    cout << "Mapping::mapGrid:ERROR: array x is not of the expected shape \n";
    Overture::abort("error");
  }
 --- */
  
  getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );


  
#ifndef USE_PPP
  Range R[4], Rx[4], Rxr[4];
  int dim[3] = {1,1,1};
  for( axis=0; axis<positionOfDomainDimension; axis++ )
    dim[axis]=r.getLength(axis);

  for( axis=0; axis<4; axis++ )
  {
    R  [axis]=Range( r.getBase(axis), r.getBound(axis));
    Rx [axis]=Range( x.getBase(axis), x.getBound(axis));
    Rxr[axis]=Range(xr.getBase(axis),xr.getBound(axis));
  }

  realArray & rr = (realArray &)r; // cast away const so we can reshape

  rr.reshape(dim[0]*dim[1]*dim[2],domainDimension);
  if( computeMap )
    x.reshape(dim[0]*dim[1]*dim[2],Rx[positionOfRangeDimension].length());
  if( computeMapDerivative )
    xr.reshape(dim[0]*dim[1]*dim[2],rangeDimension,domainDimension);

  map(r,x,xr,params); // evaluate the mapping, xr needed for normals

  rr.reshape(R[0],R[1],R[2],R[3]);
  if( computeMap )
    x.reshape(Rx[0],Rx[1],Rx[2],Rx[3]);
  if( computeMapDerivative )
    xr.reshape(Rxr[0],Rxr[1],Rxr[2],Rxr[3]);
  
#else

  if( true )
  {
    // new way:
    realSerialArray rLocal; getLocalArrayWithGhostBoundaries(r,rLocal);
    realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
    realSerialArray xrLocal; getLocalArrayWithGhostBoundaries(xr,xrLocal);
    // *wdh* 100517 mapGridS(rLocal,xLocal,xrLocal,params);

    //  *wdh* 100517 do this to avoid a leak in array id's (since getLocalArrayWithGhostBoundaries 
    //     uses adopt. -- eventually we need to fix this properly) 
    realSerialArray rLocal2;  rLocal2.reference(rLocal);
    realSerialArray xLocal2;  xLocal2.reference(xLocal);
    realSerialArray xrLocal2; xrLocal2.reference(xrLocal);
    
    mapGridS(rLocal2,xLocal2,xrLocal2,params);


  }
  else
  {
    // old way:

    // ** for P++ we cannot easily reshape without a lot of message passing

    Partitioning_Type partition1d;
    partition1d.SpecifyDecompositionAxes(1);
    partition1d.SpecifyDefaultInternalGhostBoundaryWidths(0,0,0,0);
    partition1d.SpecifyInternalGhostBoundaryWidths(0,0,0,0);

    const realSerialArray & rs = r.getLocalArray();
    const real * rsp = rs.Array_Descriptor.Array_View_Pointer3;
    const int rsDim0=rs.getRawDataSize(0);
    const int rsDim1=rs.getRawDataSize(1);
    const int rsDim2=rs.getRawDataSize(2);
#undef R
#define R(i0,i1,i2,i3) rsp[i0+rsDim0*(i1+rsDim1*(i2+rsDim2*(i3)))]

    const int myid=Communication_Manager::My_Process_Number;

    // We choose the number of points for r0 so that each processor will have at least as
    // many points as there are in the local arrays of rs
    int numToComputeLocal=rs.getLength(0)*rs.getLength(1)*rs.getLength(2);
    // printf("**** Mapping::mapGrid: node=%i numToComputeLocal=%i\n",myid,numToComputeLocal);

    int maxNumToComputeLocal;

    MPI_Allreduce(&numToComputeLocal, &maxNumToComputeLocal, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    // printf("**** Mapping::mapGrid: node=%i maxNumToComputeLocal=%i\n",myid,maxNumToComputeLocal);
  

    // *** we should really use the processors over which r is distributed
    Range Rm=maxNumToComputeLocal*Communication_Manager::numberOfProcessors();

    realArray r0; r0.partition(partition1d); r0.redim(Rm,domainDimension); 
    realArray x0, xr0;

    const realSerialArray & r0s = r0.getLocalArray();
    real * r0sp = r0s.Array_Descriptor.Array_View_Pointer1;
    const int r0sDim0=r0s.getRawDataSize(0);
    const int r0sBase=r0s.getBase(0);  // **** we need to add this on, why??? ****************************

#undef R0
#define R0(i0,i1) r0sp[r0sBase+ (i0)+r0sDim0*(i1)]

    // double check that we have it right:
    assert( r0.getLength(0) >= rs.getLength(0)*rs.getLength(1)*rs.getLength(2) );


    int i1,i2,i3;
    const int i1Base =rs.getBase(0);
    const int i2Base =rs.getBase(1);
    const int i3Base =rs.getBase(2);
    const int i1Bound=rs.getBound(0);
    const int i2Bound=rs.getBound(1);
    const int i3Bound=rs.getBound(2);
    const int i1Dim=i1Bound-i1Base+1;
    const int i2Dim=i2Bound-i2Base+1;
    const int i3Dim=i3Bound-i3Base+1;
  
    // ** copy the local values of r to the local values of r0
    // r0s=.5; // ****  for testing
    for( axis=0; axis<domainDimension; axis++ )
      for( i3=i3Base; i3<=i3Bound; i3++ )
	for( i2=i2Base; i2<=i2Bound; i2++ )
	  for( i1=i1Base; i1<=i1Bound; i1++ )
	  {
	    // int k=i1-i1Base+i1Dim*(i2-i2Base+i2Dim*(i3-i3Base));
	    // if( k<0 || k>r0s.getLength(0) )
	    // {
	    //   printf("ERROR: k=%i out of range [%i,%i], (i1,i2,i3)=(%i,%i,%i) i1Base=(%i,%i,%i) \n",k,
	    //           r0s.getBase(0),r0s.getBound(0),i1,i2,i3,
	    //             i1Base,i2Base,i3Base);
	    //   Overture::abort("error");
	    // }
	    // printf(" node=%i k=%i (i1,i2,i3)=(%i,%i,%i) r0s=(%8.2e,%8.2e)\n",myid,k,i1,i2,i3,R0(k,0),R0(k,1));
	    
	    R0(i1-i1Base+i1Dim*(i2-i2Base+i2Dim*(i3-i3Base)),axis)=R(i1,i2,i3,axis);
	  }
    
    // fill in any extra values with zero.
    const int ia=rs.getLength(0)*rs.getLength(1)*rs.getLength(2);
    const int ib=r0s.getLength(0)-1;
    for( axis=0; axis<domainDimension; axis++ )
      for( i1=ia; i1<=ib; i1++ )
	R0(i1,axis)=0.;

  

    if( computeMap )
    {
      x0.partition(partition1d); x0.redim(Rm,rangeDimension);
    }
    if( computeMapDerivative )
    {
      xr0.partition(partition1d); xr0.redim(Rm,rangeDimension,domainDimension);
    }
  
    // r0.display(" **** Mapping::mapGrid: r0 **** ");
  
    map(r0,x0,xr0,params); // *** evaluate the Mapping ***

    if( computeMap )
    {
      const realSerialArray & xs = x.getLocalArray();
      real * xsp = xs.Array_Descriptor.Array_View_Pointer3;
      const int xsDim0=xs.getRawDataSize(0);
      const int xsDim1=xs.getRawDataSize(1);
      const int xsDim2=xs.getRawDataSize(2);
#undef X
#define X(i0,i1,i2,i3) xsp[i0+xsDim0*(i1+xsDim1*(i2+xsDim2*(i3)))]

      const realSerialArray & x0s = x0.getLocalArray();
      real * x0sp = x0s.Array_Descriptor.Array_View_Pointer1;
      const int x0sDim0=x0s.getRawDataSize(0);
      const int x0sBase=x0s.getBase(0);
#undef X0
#define X0(i0,i1) x0sp[x0sBase+(i0)+x0sDim0*(i1)]

      // x0s=2.;
    
      // copy the computed values back to the input arrays
      for( axis=0; axis<domainDimension; axis++ )
	for( i3=i3Base; i3<=i3Bound; i3++ )
	  for( i2=i2Base; i2<=i2Bound; i2++ )
	    for( i1=i1Base; i1<=i1Bound; i1++ )
	    {
	      // int k=i1-i1Base+i1Dim*(i2-i2Base+i2Dim*(i3-i3Base));
	      // printf(" node=%i k=%i (i1,i2,i3)=(%i,%i,%i) x0s=(%8.2e,%8.2e)\n",myid,k,i1,i2,i3,X0(k,0),X0(k,1));

	      X(i1,i2,i3,axis)=X0(i1-i1Base+i1Dim*(i2-i2Base+i2Dim*(i3-i3Base)),axis);
	    }
    
	
    }
    if( computeMapDerivative )
    {
      const realSerialArray & xrs = xr.getLocalArray();
      real * xrsp = xrs.Array_Descriptor.Array_View_Pointer3;
      const int xrsDim0=xrs.getRawDataSize(0);
      const int xrsDim1=xrs.getRawDataSize(1);
      const int xrsDim2=xrs.getRawDataSize(2);
#undef XR
#define XR(i0,i1,i2,i3) xrsp[i0+xrsDim0*(i1+xrsDim1*(i2+xrsDim2*(i3)))]

      const realSerialArray & xr0s = xr0.getLocalArray();
      real * xr0sp = xr0s.Array_Descriptor.Array_View_Pointer2;
      const int xr0sDim0=xr0s.getRawDataSize(0);
      const int xr0sDim1=xr0s.getRawDataSize(1);
      const int xr0sBase=xr0s.getBase(0);
#undef XR0
#define XR0(i0,i1,i2) xr0sp[xr0sBase+(i0)+xr0sDim0*(i1+xr0sDim1*(i2))]

      for( axis=0; axis<rangeDimension; axis++ )
	for( int dir=0; dir<domainDimension; dir++ )
	  for( i3=i3Base; i3<=i3Bound; i3++ )
	    for( i2=i2Base; i2<=i2Bound; i2++ )
	      for( i1=i1Base; i1<=i1Bound; i1++ )
		XR(i1,i2,i3,axis+rangeDimension*dir)=XR0(i1-i1Base+i1Dim*(i2-i2Base+i2Dim*(i3-i3Base)),axis,dir);
    }
  
    Communication_Manager::Sync();
  }
  
#endif

}

// *serial-array version*
void Mapping::
mapGridS(const RealArray & r, 
	 RealArray & x, 
	 RealArray & xr /* =Overture::nullRealArray() */,
	 MappingParameters & params /* =Overture::nullMappingParameters() */ )
{
  int positionOfDomainDimension=max(1,r.numberOfDimensions()-1);
  int positionOfRangeDimension =max(1,x.numberOfDimensions()-1); 

  int axis;
  if( r.elementCount()>0 )
  {
    if( r.getLength(positionOfDomainDimension)!=domainDimension )
    {
      positionOfDomainDimension=-1;
      for( axis=domainDimension+1; axis<4; axis++ )
	if( r.getLength(axis)==domainDimension )
	{
	  positionOfDomainDimension=axis;
	  break;
	}
    }
    if( positionOfDomainDimension==-1 )
    {
      cout << "Mapping::mapGridS:ERROR: array r is not of the expected shape \n";
      printf(" bounds on r = [%i,%i][%i,%i] \n",r.getBase(0),r.getBound(0),r.getBase(1),r.getBound(1));
    
      Overture::abort("error");
    }
  }
  
  getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );


  Range R[4], Rx[4], Rxr[4];
  int dim[3] = {1,1,1};
  for( axis=0; axis<positionOfDomainDimension; axis++ )
    dim[axis]=r.getLength(axis);

  for( axis=0; axis<4; axis++ )
  {
    R  [axis]=Range( r.getBase(axis), r.getBound(axis));
    Rx [axis]=Range( x.getBase(axis), x.getBound(axis));
    Rxr[axis]=Range(xr.getBase(axis),xr.getBound(axis));
  }

  RealArray & rr = (RealArray &)r; // cast away const so we can reshape

  rr.reshape(dim[0]*dim[1]*dim[2],domainDimension);
  if( computeMap )
    x.reshape(dim[0]*dim[1]*dim[2],Rx[positionOfRangeDimension].length());
  if( computeMapDerivative )
    xr.reshape(dim[0]*dim[1]*dim[2],rangeDimension,domainDimension);

  mapS(r,x,xr,params); // evaluate the mapping, xr needed for normals

  rr.reshape(R[0],R[1],R[2],R[3]);
  if( computeMap )
    x.reshape(Rx[0],Rx[1],Rx[2],Rx[3]);
  if( computeMapDerivative )
    xr.reshape(Rxr[0],Rxr[1],Rxr[2],Rxr[3]);
}




int Mapping::
mappingHasChanged()
// ======================================================================================
/// \param Access: protected
/// \details 
///     Call this function when the mapping has changed 
// ======================================================================================
{
  if( Mapping::debug & 16 )
    printf("mappingHasChanged called for %s\n",(const char*)getName(mappingName));

  remakeGrid=1;
  remakeGridSerial=1;
  
  arcLength=-1.;   // this is no longer valid
  signForJacobian=0.;  // this is no longer valid

  if (approximateGlobalInverse != NULL )approximateGlobalInverse->reinitialize();
  // **** we also need to get a new dataBaseID ***********
  dataBaseID=0; // getGlobalID();  // do this for now
  return 0;
}

bool  Mapping::
gridIsValid() const
// ==========================================================================
/// \details 
///    Return true if remakeGrid=false
// ==========================================================================
{
  return remakeGrid==false;
}

void Mapping::
setGridIsValid() 
// ==========================================================================
/// \details 
///    Indicate that the grid is valid.
// ==========================================================================
{
  remakeGrid=false;
}



void Mapping::
periodicShift( realArray & r, const Index & I )
// =====================================================================================
/// \details 
///    Shift r into the interval [0.,1] if the mapping is periodic (derivative or function)
// =====================================================================================
{
  for( int axis=axis1; axis < domainDimension; axis++ )
    if( getIsPeriodic(axis) )
    {
       r(I,axis)=fmod(r(I,axis)+1.,1.);  // map back to [0,1]
       // r(I,axis)-=floor2(r(I,axis));  
    }
}

#ifdef USE_PPP
void Mapping::
periodicShift( RealArray & r, const Index & I )
// =====================================================================================
// /Description:
//   Shift r into the interval [0.,1] if the mapping is periodic (derivative or function)
// =====================================================================================
{
  for( int axis=axis1; axis < domainDimension; axis++ )
    if( getIsPeriodic(axis) )
    {
       r(I,axis)=fmod(r(I,axis)+1.,1.);  // map back to [0,1]
       // r(I,axis)-=floor2(r(I,axis));  
    }
}
#endif

// finish me: 
// #ifdef USE_PPP
// int Mapping:: 
// project( RealArray & x, 
// 	 MappingProjectionParameters & mpParams )
// {
//   OV_ABORT("Mapping:: project:ERROR: finish me for parallel");

//   return 0;
// }
// #endif


int Mapping:: 
project( realArray & x, 
	 MappingProjectionParameters & mpParams )
//===========================================================================
/// \brief  
///    Project the points x(i,0:2) onto the Mapping. This is normally used
///  to project points onto a curve in 2D or surface in 3D (i.e. domainDimension=rangeDimension-1,
///     aka a hyperspace of co-dimension 1).
/// 
/// \param x (input) : project these points.
/// \param mpParams (input) : This class holds parameters used by the projection algorithm.
///     
/// \param Notes:
///  The inverse unit square coordinates will be held in the array mpParams.getRealArray(r). If you 
///   have a good guess for these values then you should supply this array.
/// 
///  If you want the derivatives you should dimension mpParams.getRealArray(xr) to be big enough
///  and then they will be computed.
/// 
/// \param Note: If you want the normal (or tangent to a curve) you should dimension mpParams.getRealArray(normal) 
///  to be big enough. For curves (domainDimension==1) the normal is actually the tangent to the curve.
///  Otherwise the normal will only make sense if the Mapping is a curve in 2D or a surface in 3D, i.e.
///  domainDimension=rangeDimension-1.
//===========================================================================
{
  assert( rangeDimension==2 || rangeDimension==3 );
  
  typedef MappingProjectionParameters MPP;

  realArray & r  = mpParams.getRealArray(MPP::r);  // cannot getLocalArray here since we redim!!
  realArray & xr = mpParams.getRealArray(MPP::xr);
  realArray & normal = mpParams.getRealArray(MPP::normal);
  
  if( r.getBase(0)>x.getBase(0) || r.getBound(0)<x.getBound(0) )
  {
    // printf("project: initializing r to -1\n");
    r.redim(x.dimension(0),domainDimension);
    r=-1.;
  }
  const bool computeNormals= normal.getLength(0)>0;
  // we need to compute xr if we need to compute the normals.
  if( computeNormals && (xr.getBase(0)>x.getBase(0) || xr.getBound(0)<x.getBound(0) ) )
    xr.redim(x.dimension(0),rangeDimension,domainDimension);

//   realArray & r  = r0.getLocalArray();  // *** note: can't redim these
//   realArray & xr  = xr0.getLocalArray();
//   const realArray & normal  = normal0.getLocalArray();


// tmp
  realArray xOriginal;
  xOriginal = x;
  
  inverseMap(x,r);
  // r.display("r coordinates before map");
  
  map(r,x,xr);
  // r.display("r coordinates after map");

//    xOriginal.display("Original x,y,z coordinates");
//    x.display("Projected x,y,z, coordinates");

  if( false ) // **************** 081101 
    printF(" ****Mapping::project: map = %s, computeNormals=%i \n",(const char*) getName(mappingName),computeNormals);

  if( computeNormals )
  {
    Range I(r.getBase(0),r.getBound(0));
    realArray norm(I);
    if( domainDimension==1 )
    {
      // compute tangents
      for( int dir=0; dir<rangeDimension; dir++ ) 
	normal(I,dir)=xr(I,dir,0);

      // normal.display("project: normal before normalizing");
      if( rangeDimension==2 )
      {
	norm = 1./max( REAL_MIN*10., SQRT( SQR(normal(I,0))+SQR(normal(I,1)) ) );
      }
      else
      {
	norm = 1./max( REAL_MIN*10., SQRT( SQR(normal(I,0))+SQR(normal(I,1))+SQR(normal(I,2))  ) );
      }
      
    }
    else
    {
      // compute the normal
      if( rangeDimension==2 )
      {
	normal(I,axis1)= xr(I,1,0);
	normal(I,axis2)=-xr(I,0,0);
	norm = SQRT( SQR(normal(I,0))+SQR(normal(I,1)) ) ;
      }
      else
      {
	normal(I,axis1)=xr(I,1,0)*xr(I,2,1)-xr(I,2,0)*xr(I,1,1);
	normal(I,axis2)=xr(I,2,0)*xr(I,0,1)-xr(I,0,0)*xr(I,2,1);
	normal(I,axis3)=xr(I,0,0)*xr(I,1,1)-xr(I,1,0)*xr(I,0,1);
	norm = SQRT( SQR(normal(I,0))+SQR(normal(I,1))+SQR(normal(I,2))  ) ;
      }
      // We need to check for zero length normals at singular points.
      bool mappingHasACoordinateSingularity = FALSE;
      for( int axis=0; axis<domainDimension && !mappingHasACoordinateSingularity ; axis++ )
      {
	for( int side=Start; side<=End; side++ )
	{
	  if( getTypeOfCoordinateSingularity(side,axis)==polarSingularity )
	  {
	    mappingHasACoordinateSingularity = TRUE;
	    break;
	  }
	}
      }
      if( mappingHasACoordinateSingularity )
      {
	const int rBase=r.getBase(0), rBound=r.getBound(0);
	Range Rx(0,rangeDimension-1);
	for( int j=rBase; j<=rBound; j++ )
	{
	  if( norm(j)==0. )
	  {
	    // this must be a singularity, choose a nearby normal
	    int jn = j < rBound ? j+1 : j-1;
	    for( int i=rBase; i<=rBound; i++ )  // look for a nearby non-zero normal to use
	    {
	      if( norm(jn)!=0. )
		break;
	      jn++;  // check the next neighbour
	      if( jn>rBound )
		jn=rBase; // wrap around
	    }
	    norm(j)=norm(jn);
	    normal(j,Rx)=normal(jn,Rx);
	    if( norm(j)==0. )
	    {
	      printf("Mapping::project::ERROR: all normals are zero! something is wrong here. \n");
	      norm.display("Here are the normals");
	      Overture::abort("error");
	    }
	  }
	}
      }
      else
      {
	norm=max(REAL_MIN,norm);
      }
      norm=1./norm;
    }
    
    int dir;
    for( dir=0; dir<rangeDimension; dir++ ) 
      normal(I,dir)*=norm;

    if( Mapping::debug & 4 ) // **************** 081101 
    {
      printF(" ****Mapping::project: compute normals for map = %s\n",(const char*) getName(mappingName));
      for( int i=I.getBase(); i<=I.getBound(); i++ )
      {
	printF(" i=%i x0=(%5.2f,%5.2f,%5.2f) -> x=(%5.2f,%5.2f,%5.2f) r=(%5.2f,%5.2f) normal=(%5.2f,%5.2f,%5.2f)\n",i,
	       xOriginal(i,0),xOriginal(i,1),xOriginal(i,2),x(i,0),x(i,1),x(i,2),r(i,0),r(i,1),normal(i,0),normal(i,1),
                (rangeDimension==2 ? 0. : normal(i,2)));
      }
    }
    
  }
  
  return 0;
}


// *serial-array version*
int Mapping::
projectS( RealArray & x, MappingProjectionParameters & mpParams )
{
  OV_ABORT("error - finish this");
  return 0;
}


int Mapping:: 
put( GenericDataBase & dir, const aString & name) const
// =====================================================================================
/// \details 
///  save this object to a sub-directory called "name"
// =====================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.setMode(GenericDataBase::streamOutputMode);

  subDir.put( className,"className" );
  ((Mapping*)this)->dataBaseID=dir.getID();
  // subDir.put( getID(),"dataBaseID");
  if( dir.getList()!=NULL )
    dir.getList()->add((Mapping*)this,dataBaseID);

  subDir.put( (int)basicInverseOption,"basicInverseOption" ); 
  subDir.put( (int*)bc,"bc",6 );
  subDir.put( coordinateEvaluationType,"coordinateEvaluationType",numberOfCoordinateSystems );
  subDir.put( (int)domainCoordinateSystem,"domainCoordinateSystem" );
  subDir.put( domainDimension,"domainDimension" );
  subDir.put( (int)domainSpace,"domainSpace" );
  subDir.put( &gridIndexRange(0,0),"gridIndexRange",6);
  subDir.put( &numberOfGhostPoints(0,0),"numberOfGhostPoints",6);
  
  subDir.put( invertible,"invertible" );
  int ta3[3];
  int axis;
  for( axis=0; axis<3; axis++)
    ta3[axis]=isPeriodic[axis];
  subDir.put( ta3,"isPeriodic",3 );
  subDir.put( (int)mappingCoordinateSystem0,"mappingCoordinateSystem0");

  subDir.put( (real*)periodVector,"periodVector",9 );
  subDir.put( periodicityOfSpace,"periodicityOfSpace");

  subDir.put( signForJacobian,"signForJacobian");
  subDir.put( numberOfDistributedGhostLines,"numberOfDistributedGhostLines");
  subDir.put( arcLength,"arcLength" );
  subDir.put( inverseIsDistributed,"inverseIsDistributed" );
  subDir.put( mapIsDistributed,"mapIsDistributed" );

  subDir.put( (int)rangeCoordinateSystem,"rangeCoordinateSystem" );
  subDir.put( rangeDimension,"rangeDimension" );
  subDir.put( (int)rangeSpace,"rangeSpace" );
  subDir.put( remakeGrid,"remakeGrid" );
  subDir.put( (int*)share,"share",6 );
  int ta23[2][3];
  for( axis=0; axis<3; axis++)
    for( int side=0; side<=1; side++ )
      ta23[side][axis]=typeOfCoordinateSingularity[side][axis];
  subDir.put( (int*)ta23,"typeOfCoordinateSingularity",6 );
  //  For the arrays of bounds we save as different names
  const int bufLen = 80;
  static char buf[bufLen];
  int i;
  for( i=0; i<3; i++ )
    for( int side=Start; side<=End; side++ )
    { 
      sPrintF(buf,"domainBound[%i][%i]",i,side);
      domainBound[i][side].put( subDir,buf );
      sPrintF(buf,"rangeBound[%i][%i]",i,side);
      rangeBound[i][side].put( subDir,buf );
      sPrintF(buf,"domainCoordinateSystemBound[%i][%i]",i,side);
      domainCoordinateSystemBound[i][side].put( subDir,buf );
      sPrintF(buf,"rangeCoordinateSystemBound[%i][%i]",i,side);
      rangeCoordinateSystemBound[i][side].put( subDir,buf );
    }
  // Save names
  for( i=0; i<numberOfMappingItemNames; i++ )
  {
    if( debug & 64 )
      cout << "put: " << mappingItemString[i] << " = " 
	   << getName( mappingItemName(i) ) << endl;
    subDir.put( getName( mappingItemName(i) ),mappingItemString[i] );
  }

  approximateGlobalInverse->put(subDir,"approximateGlobalInverse");
  exactLocalInverse->put(subDir,"exactLocalInverse");

  delete &subDir;
  return TRUE;
}

int Mapping::
getGridIndexRange(int side, int axis )
// =====================================================================================
/// \details 
///      Get the number of getGridIndexRange
// =====================================================================================
{
  if( axis>=0 && axis<3 && side>=0 && side<=1 )
  {
    return gridIndexRange(side,axis);
  }
  else
  {
    printF("Mapping::getGridIndexRange:ERROR: invalid side=%i or axis=%i\n",side,axis);
    return 0;
  }
  
}

void Mapping::
setGridIndexRange(int side, int axis, int num )
// =====================================================================================
/// \details 
///      Set the getGridIndexRange
// =====================================================================================
{
  if( axis>=0 && axis<3 && side>=0 && side<=1 )
  {
    gridIndexRange(side,axis)=num;
  }
  else
  {
    printF("Mapping::setGridIndexRange:ERROR: invalid side=%i or axis=%i\n",side,axis);
  }
  
}

int Mapping::
getNumberOfGhostPoints(int side, int axis )
// =====================================================================================
/// \details 
///      Get the number of ghost points to use on the grid associated with the mapping.
// =====================================================================================
{
  if( axis>=0 && axis<3 && side>=0 && side<=1 )
  {
    return numberOfGhostPoints(side,axis);
  }
  else
  {
    printF("Mapping::getNumberOfGhostPoints:ERROR: invalid side=%i or axis=%i\n",side,axis);
    return 0;
  }
  
}

void Mapping::
setNumberOfGhostPoints(int side, int axis, int numGhost )
// =====================================================================================
/// \details 
///      Set the number of ghost points to use on the grid associated with the mapping.
// =====================================================================================
{
  if( axis>=0 && axis<3 && side>=0 && side<=1 )
  {
    numberOfGhostPoints(side,axis)=numGhost;
  }
  else
  {
    printF("Mapping::setNumberOfGhostPoints:ERROR: invalid side=%i or axis=%i\n",side,axis);
  }
  
}

void Mapping::
reinitialize()
// =====================================================================================
/// \details 
///    Re-initialize a mapping that has changed (this will re-initialize the inverse)
// =====================================================================================
{
  if( approximateGlobalInverse!=NULL )
    approximateGlobalInverse->reinitialize(); // this should rebuild the bounding box trees
  if( exactLocalInverse!=NULL )
    exactLocalInverse->reinitialize();

/* ----
  delete approximateGlobalInverse;
  approximateGlobalInverse=new ApproximateGlobalInverse( *this );
  assert( approximateGlobalInverse != 0 );
  delete exactLocalInverse;
  exactLocalInverse=new ExactLocalInverse( *this );
  assert( exactLocalInverse != 0 );
  mappingHasChanged();
---- */

/* ----  this will not destroy the bounding boxes!!, so do above
  if( approximateGlobalInverse!=NULL )
    approximateGlobalInverse->reinitialize();
  if( exactLocalInverse!=NULL )
    exactLocalInverse->reinitialize();
-- */
}



void Mapping::
setDefaultMappingBounds( const mappingSpace ms, Bound mappingBound[3][2] )
{
 int i, side;
 switch( ms ){
  case parameterSpace :
    // *wdh* 031108 for( i=0; i<domainDimension; i++ )
    for( i=0; i<3; i++ )  // initialize all bounds
      for( side=0; side<=1 ; side++ )
        mappingBound[i][side].set( side);     // [0,1]
    break;
  case cartesianSpace :
    // *wdh* 031108 for( i=0; i<domainDimension; i++ )
    for( i=0; i<3; i++ )  // initialize all bounds
      for( side=0; side<=1 ; side++ )
        mappingBound[i][side].set( 2*side-1,0 );  //  [-infinity,infinity]
    break;
  default:
    cout << "Mapping:Error, unknown domain space" << endl;
    break;
  }
}

void Mapping::
setDefaultCoordinateSystemBounds( const coordinateSystem cs, Bound csBound[3][2] )
{
  int i,side;
  switch( cs ) {
    case cartesian :
    default:
    for( i=0; i<3; i++ )
      for( side=0; side<=1 ; side++ )
        csBound[i][side].set( side);
  }
}



void Mapping::
setName( const mappingItemName item, const aString & itemName )
// =====================================================================================
/// \details 
///     Assign a name from enum mappingItemName:
///   <ul>
///     <li> <B>mappingName</B> : mapping name
///     <li> <B>domainName</B> : domain name
///     <li> <B>rangeName</B> :
///     <li> <B>domainAxis1Name</B> : names for coordinate axes in domain
///     <li> <B>domainAxis2Name</B> : 
///     <li> <B>domainAxis3Name</B> : 
///     <li> <B>rangeAxis1Name</B> : names for coordinate axes in range
///     <li> <B>rangeAxis2Name</B> : 
///     <li> <B>rangeAxis3Name</B> :
///   </ul>
/// \param item (input): assign this item.
/// \param itemName (input) : name to give the item.
// =====================================================================================
{
  namestr[item]=itemName;
}







void Mapping::
reference( const Mapping & map )
{
  cout << "Mapping::ERROR reference function called! " << endl;
  map.display("Here is the mapping that was passed to the reference");

}



//=======================================================================
//       outside
//
//  Is x outside the grid
//  return TRUE if you are sure, return FALSE if it isn't or if you don't know
//
//=======================================================================
int Mapping::
outside( const realArray & x )
{ 
  if( debug & 16 )
    cout << "Mapping::outside - x = " << x(axis1) << endl;
  return FALSE;   // by default we do not know
}


int Mapping::
validSide( const int side ) const
{  // utiliity routine for checking a "side" argument
  if( side>= 0 && side <= 1 )
    return TRUE;
  else
    return FALSE;
}

int Mapping::
validAxis( const int axis ) const
{  // utiliity routine for checking a "axis" argument
  if( axis>= 0 && axis < domainDimension )
    return TRUE;
  else
    return FALSE;
}


void Mapping::
mappingError( const aString & subName,  const int side, const int axis ) const
{ cout << ">>Mapping: Invalid arguments to function `" << subName 
       <<  "', for mapping " << getName(mappingName) << endl;
  if( !validSide( side ) )
    cout << ">> The argument `side' is invalid, side = "  << side << endl;
  if( !validAxis( axis ) )
    cout << ">> The argument `axis' is invalid, axis = "  << axis << endl;
}

void Mapping::
setCoordinateEvaluationType( const coordinateSystem type, const int trueOrFalse )
// =====================================================================================
/// \details 
// =====================================================================================
{
  coordinateEvaluationType[type]=trueOrFalse;
}

void Mapping::
setTypeOfCoordinateSingularity( const int side, const int axis, 
                                              const coordinateSingularity type )
// =====================================================================================
/// \details 
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{
  if( validSide( side ) && validAxis( axis ) )
    typeOfCoordinateSingularity[side][axis]=type;
  else
    mappingError( "setTypeOfCoordinateSingularity", side,axis );
}

intArray & Mapping::
topologyMask()
// =====================================================================================
/// \details 
///      Return the mask that represents a partial periodicity, such as a C-grid.
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{
  if( topologyMaskPointer!=0 )
    return *topologyMaskPointer;
  else
    return Overture::nullIntegerDistributedArray();
}



Mapping::topologyEnum Mapping::
getTopology( const int side, const int axis) const
// =====================================================================================
/// \details 
///      Return the topology. This is primarily used to represent C-grids.
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{
  if( !validSide( side ) || !validAxis( axis ) )
    mappingError( "getTopology", side,axis );
   
  return topology[axis][side];
}

void Mapping::
setTopology( const int side, const int axis, const topologyEnum topo )
// =====================================================================================
/// \details 
///  Specify the topology. This is primarily used to represent C-grids.
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{
  if( validSide( side ) && validAxis( axis ) )
    topology[axis][side]=topo;
  else
    mappingError( "setTopology", side,axis );
}


void Mapping::
setDomainDimension( const int domainDimension0 )
// =====================================================================================
/// \details 
// =====================================================================================
{ if( domainDimension0 >= 0 && domainDimension0 <= 3 )
    domainDimension=domainDimension0;
  else
    mappingError( "setDomainDimension", 0,0 );
}

void Mapping::
setRangeDimension( const int rangeDimension0 )
// =====================================================================================
/// \details 
// =====================================================================================
{ if( rangeDimension0 >= 0 && rangeDimension0 <= 3 )
    rangeDimension=rangeDimension0;
  else
    mappingError( "setRangeDimension", 0,0 );
}

void Mapping::
setBasicInverseOption( const basicInverseOptions option )
// =====================================================================================
/// \details 
// =====================================================================================
{
  basicInverseOption=option;
  // *wdh* 2012/04/22 : this is not correct: invertible is unrelated to the basic inverse option!
  // *wdh* invertible= basicInverseOption==canInvert; 
}


void Mapping::
setBoundaryCondition( const int side, const int axis, const int bc0 )
// =====================================================================================
/// \details 
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{
  if( validSide( side ) && (axis>=0 && axis<rangeDimension) ) // *wdh* 070301 validAxis( axis ) )
    bc[side][axis]=bc0;
  else
    mappingError( "setBoundaryCondition", side,axis );
}

void Mapping::
setShare( const int side, const int axis, const int share0 )
// =====================================================================================
/// \details 
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{
  if( validSide( side ) && (axis>=0 && axis<rangeDimension) ) // *wdh* 070301 -- validAxis( axis ) )
    share[side][axis]=share0;
  else
    mappingError( "setShare", side,axis );
}

void Mapping::
setSignForJacobian( const real signForJac )
// =====================================================================================
/// \details 
///    Set the sign of the jacobian, 1 (right handed coordinate system) or -1 (left handed).
///   This may only make sense for some mappings.
/// \param signForJac (input) : should be 1. or -1.
// =====================================================================================
{
  assert( fabs(signForJac)==1. );
  signForJacobian=signForJac;
}

void Mapping::
setMappingCoordinateSystem( const mappingCoordinateSystem mappingCoordinateSystem1 )
// =====================================================================================
/// \details 
// =====================================================================================
{
  mappingCoordinateSystem0=mappingCoordinateSystem1;
}

void Mapping::
setIsPeriodic( const int axis, const periodicType isPeriodic0 )
// =====================================================================================
/// \details 
/// \param axis (input): axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
/// \param Notes:
///     This routine has some side effects. It will change the boundaryConditions to be consistent
///   with the periodicity (if necessary).
// =====================================================================================
{
  if( validAxis( axis ) )
  {
    isPeriodic[axis]=isPeriodic0;
    // set bc to be consistent
    if( isPeriodic[axis]!=notPeriodic )
    {
      if( bc[Start][axis]>=0 )
	bc[Start][axis]=-1;
      if( bc[End][axis]>=0 )
	bc[End][axis]=-1;
    }
    else // *wdh* 001025
    {
      if( bc[Start][axis]<0 )  bc[Start][axis]=0;
      if( bc[End  ][axis]<0 )  bc[End  ][axis]=0;
    }
    
  }
  else
    mappingError( "setIsPeriodic", 0,axis );
}

void Mapping::
setGridDimensions( const int axis, const int dim )
// =====================================================================================
/// \details 
/// \param axis (input): axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{
  if( validAxis( axis ) )
  {
    if( dim!=getGridDimensions(axis) )
    {
      mappingHasChanged();    
      gridIndexRange(1,axis)=gridIndexRange(0,axis) + dim-1;  // new way

#ifdef USE_PPP
  if( false )
  {
    int nMin = ParallelUtility::getMinValue(getGridDimensions(axis));
    int nMax = ParallelUtility::getMaxValue(getGridDimensions(axis));
    if( nMin!=nMax )
    {
      int myid=max(0,Communication_Manager::My_Process_Number);
      printF("Mapping:setGridDimensionsERROR: mapping=%s : gridDimensions don't match on different processors!\n",
	     (const char*)getName(mappingName));
      printF(" usesDistributedInverse=%i\n",(int)usesDistributedInverse());
      printf(" myid=%i map.getGridDimensions=[%i,%i,%i]\n",
	     myid,getGridDimensions(0),getGridDimensions(1),getGridDimensions(2));
      fflush(0);
      Overture::abort("error");
    }
  }
#endif 

    }
  }
  else
    mappingError( "setGridDimensions", 0,axis );
}

void Mapping::
setInvertible( const int invertible0 )
// =====================================================================================
/// \details 
// =====================================================================================
{ 
  invertible=invertible0;
}

/* **
   void Mapping::
   setPeriodicityOfSpace( const int periodicityOfSpace0 )
   { if( periodicityOfSpace0 >= 0 && periodicityOfSpace0 < domainDimension )
   periodicityOfSpace=periodicityOfSpace0;
   else
   mappingError( "setPeriodicityOfSpace", 0,0 );
   }
   ** */

void Mapping::
setParameter( const MappingParameters::realParameter & param, const real & value ) 
// =====================================================================================
/// \details 
///    Set the value of a parameter used by the Mapping or the ApproximateGlobalInverse or the ExactLocalInverse.
///  
///  <ul>
///   <li> <B>THEnonConvergenceValue</B> : value given to "r" value of the inverse when there is no convergence. This is
///               currently equal to 10. and cannot be changed.
///   <li> <B>THEnewtonToleranceFactor</B> : convergence tolerance is this times the machine epsilon. Default=100. ?
///   <li> <B>THEnewtonDivergenceValue</B> : newton is deemed to have diverged if the r value is this much outside [0,1].
///       The default value is .1 and so Newton is deemed to have diverged when the r value is outside the range 
///       [-.1,1.1]
///   <li> <B>THEnewtonL2Factor</B> : extra factor for finding the closest point to a curve or surface, default=.1.
///           This factor allows a less strict convergence factor if the target point is far from the mapping.
///           Decrease this value if you want a more accurate answer. You may also have to decrease this value
///           for mappings that have poor parameterizations. 
///   <li> <B>THEboundingBoxExtensionFactor</B> : relative amount to increase the bounding box each direction. The bounding
///      box can be increased in size to allow the inverse function to still converge for nearby points. The default
///      value is $.01$. ***Actually*** only the bounding boxes for the highest leaves in the bounding box tree
///      are extended by this factor. The bounding boxes for all other nodes (and the root) are just computed
///      from the size of the bounding boxes of the two leaves of the node.
///   <li> <B>THEstencilWalkBoundingBoxExtensionFactor</B> : The stencil walk routine that finds the closest point
///      before inversion by Newton's method will only find the closest point if the point lies in a box
///      that is equal to the bounding box extended by this factor in each direction. Default =.2
///  </ul>
/// 
///  
// =====================================================================================
{ 
  switch (param)
  {
  case MappingParameters::THEnonConvergenceValue:    // value given to inverse when there is no convergence
  case MappingParameters::THEnewtonToleranceFactor:  // convergence tolerance is this times the machine epsilon
  case MappingParameters::THEnewtonL2Factor:
  case MappingParameters::THEnewtonDivergenceValue:  // newton is deemed to have diverged if the r value is this much outside [0,1]
  case MappingParameters::THEboundingBoxExtensionFactor:
  case  MappingParameters::THEstencilWalkBoundingBoxExtensionFactor:
    assert( approximateGlobalInverse!=NULL );
    approximateGlobalInverse->setParameter(param,value);
    break;
  default:
    cout << " Mapping::setParameter: fatal error, unknown value for realParameter\n";
    Overture::abort("error");
  }
}

void Mapping::
setParameter( const MappingParameters::intParameter & param, const int & value ) 
// =====================================================================================
/// \details 
///    Set the value of a parameter used by the Mapping or the ApproximateGlobalInverse or the ExactLocalInverse.
///  
///  <ul>
///   <li> <B>THEfindBestGuess</B> : if true, always find the closest point, even if the point to be inverted
///     is outside the bounding box. Default value is false.
///  </ul>
/// 
///  
// =====================================================================================
{ 
  switch (param)
  {
  case  MappingParameters::THEfindBestGuess:
    assert( approximateGlobalInverse!=NULL );
    approximateGlobalInverse->setParameter(param,value);
    break;
  default:
    cout << " Mapping::setParameter: fatal error, unknown value for intParameter\n";
    Overture::abort("error");
  }
}


void Mapping::
setPeriodVector( const int axis, const int direction, const real periodVectorComponent  )
// =====================================================================================
///   For a mapping with getIsPeriodic(direction)==derivativePeriodic this routine sets
///  the vector that determines the shift from the `left' edge to the `right' edge.
/// \param axis (input): axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<rangeDimension$
///       are the components of the vector 
/// \param direction (input) : direction =0,1,...,domainDimension
// =====================================================================================
{
  if( axis >= 0 && axis < rangeDimension 
      && direction >= 0 && direction < domainDimension )
    periodVector[axis][direction]=periodVectorComponent;
  else
    mappingError( "setPeriodVector", 0, axis );
}



void Mapping::
setPartition( Partitioning_Type & partition_ )
// ====================================================================================
/// \details 
///    Supply a parallel partition to use when building the "grid" array
// ====================================================================================
{
  partition = partition_;
  partitionInitialized=true;
  
#ifdef USE_PPP
  if( false )
  {
    const intSerialArray & processorSet = partition.getProcessorSet();
    printF("Mapping::setPartition: partition(COPY) : processors=[%i,%i]\n",processorSet.getBase(0),processorSet.getBound(0));
  }
#endif 

}


void Mapping::
setDomainSpace( const mappingSpace domainSpace0 )
// =====================================================================================
/// \details 
// =====================================================================================
{ domainSpace=domainSpace0; }

void Mapping::
setRangeSpace( const mappingSpace rangeSpace0 )
// =====================================================================================
/// \details 
// =====================================================================================
{ rangeSpace=rangeSpace0; }

void Mapping::
setDomainCoordinateSystem( const coordinateSystem domainCoordinateSystem0 )
// =====================================================================================
/// \details 
// =====================================================================================
{ domainCoordinateSystem=domainCoordinateSystem0; }

void Mapping::
setRangeCoordinateSystem( const coordinateSystem rangeCoordinateSystem0 )
// =====================================================================================
/// \details 
// =====================================================================================
{ rangeCoordinateSystem=rangeCoordinateSystem0; }

void Mapping::
setDomainBound( const int side, const int axis, const Bound domainBound0 )
// =====================================================================================
/// \details 
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{ 
  if( validSide( side ) && validAxis( axis ) )
    domainBound[axis][side]=domainBound0; 
  else
    mappingError( "setDomainBound", side,axis );
}

void Mapping::
setRangeBound( const int side, const int axis, const Bound rangeBound0 )
// =====================================================================================
/// \details 
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{ 
  if( validSide( side ) && axis>=0 && axis<rangeDimension )
    rangeBound[axis][side]=rangeBound0; 
  else
    mappingError( "setRangeBound", side,axis );
}

void Mapping::
setDomainCoordinateSystemBound(const int side, 
			       const int axis, 
			       const Bound domainCoordinateSystemBound0 )
// =====================================================================================
/// \details 
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{ 
  if( validSide( side ) && validAxis( axis ) )
    domainCoordinateSystemBound[axis][side]=domainCoordinateSystemBound0; 
  else
    mappingError( "setDomainCoordinateSystemBound", side,axis );
}

void Mapping::
setRangeCoordinateSystemBound(const int side, 
			      const int axis, 
			      const Bound rangeCoordinateSystemBound0 )
// =====================================================================================
/// \details 
/// \param side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
///      and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{ 
  if( validSide( side ) && axis>=0 && axis<rangeDimension )
    rangeCoordinateSystemBound[axis][side]=rangeCoordinateSystemBound0; 
  else
    mappingError( "setRangeCoordinateSystemBound", side,axis );
}



void Mapping::
useRobustInverse(const bool trueOrFalse /* =TRUE */ )
// =======================================================================================
/// \details 
///     Use the robust form of the inverse.
// =======================================================================================
{
  if( approximateGlobalInverse!=NULL )
    approximateGlobalInverse->useRobustInverse(trueOrFalse);
  else
  {
    printf("Mapping::useRobustInverse:WARNING: no approximateGlobalInverse exists for this Mapping\n");

  }
  if( exactLocalInverse!=NULL )
  {
    exactLocalInverse->useRobustInverse(trueOrFalse);
  }
  
}

//! Return true if the robust inverse is used.
bool Mapping::
usingRobustInverse() const
{
  if( approximateGlobalInverse!=NULL )
    return approximateGlobalInverse->usingRobustInverse();
  else
  {
    printf("Mapping::useRobustInverse:WARNING: no approximateGlobalInverse exists for this Mapping\n");
    return false;
  }
}

bool Mapping::
usesDistributedInverse() const
// ==============================================================================================
/// \details  Indicate whether in parallel this Mapping uses the distributed array, grid, for the inverse
///   or whether the Mapping uses the serial array, gridSerial, (duplicated across all processors) for the inverse.
///   If true, then the inverseMap function is a distributed (i.e. collective) parallel operation.
///  
///   Normally only Mapping's with a small amount of data would duplicate the grid across all processors. 
///  
/// \return 
///    If true, then the inverseMap function is a distributed (collective) operation.
///    The Mapping uses the distributed parallel array, grid, for the inverse.
///    In this case, the array gridSerial points to the local array with ghost boundaries of grid.
///  
/// 
///    If false then the inverseMap function is a serial (non-collective) operation with no communication. 
///    The Mapping uses the serial array, gridSerial, for the inverse and this
///    grid is duplicated across all processors. In this case the array, grid, will remain empty.
///  
// ==============================================================================================
{
  return inverseIsDistributed;
}

bool Mapping::
usesDistributedMap() const
// ==============================================================================================
/// \details  Indicate whether in parallel the map function is a distributed (i.e. collective)
///    parallel operation.
///  
///   Most Mapping's the map function is NOT distributed. The DataPointMapping, however, will normally
///  use a distributed map function since the data-points that define the mapping are usually stored
///  in a distributed array.
///  
/// \return 
///    If true, then the map function is a distributed (collective) operation.
///    If false, then the map function can be evaluated with no communication. 
///  
// ==============================================================================================
{
  return mapIsDistributed;
}

real Mapping::
sizeOf(FILE *file /* = NULL */ ) const
// =======================================================================================
/// \details 
///    Return size of this object  
// =======================================================================================
{
  real size=sizeof(*this);

  size+=grid.elementCount()*sizeof(real);

  if( topologyMaskPointer!=NULL )
    size+=topologyMaskPointer->elementCount()*sizeof(int);

  // size+=workSpace.sizeOf(); 
  if( approximateGlobalInverse!=NULL )
    size+=approximateGlobalInverse->sizeOf();
  if( exactLocalInverse!=NULL )
    size+=exactLocalInverse->sizeOf();

  return size;
}

static void 
updateMappingDialogData( Mapping *map, DialogData &dialog )
{
  aString buff;
  int i;
  int domainDimension = map->getDomainDimension();

  dialog.setTextLabel(0,map->getName(Mapping::mappingName));

  if ( domainDimension==1 )
  {
    sPrintF(buff,"%i",map->getGridDimensions(0));
  }
  else if ( domainDimension==2 )
  {
    sPrintF(buff,"%i %i",map->getGridDimensions(0), map->getGridDimensions(1));
  }
  else if ( domainDimension==3 )
  {
    sPrintF(buff,"%i %i %i",map->getGridDimensions(0), map->getGridDimensions(1), 
	    map->getGridDimensions(2));
  }

  dialog.setTextLabel(1,buff);
  
  if ( map->approximateGlobalInverse==NULL ) 
    dialog.setSensitive(false, DialogData::toggleButtonWidget, 0);
  else
  {
    dialog.setSensitive(true, DialogData::toggleButtonWidget, 0);
    int inverseOnOff = map->approximateGlobalInverse->usingRobustInverse();
    dialog.setToggleState(0, inverseOnOff);
  }

  buff="";

  int offset=2;
  int axis,side;
  for ( i=offset; i<offset+2*map->getDomainDimension(); i++ )
  {
    axis = (i-offset)/2;
    side = (i-offset)%2;
    sPrintF(buff, "%i", map->getBoundaryCondition(side,axis));
    dialog.setTextLabel(i,buff);
    buff="";
  }

  offset = offset+2*map->getDomainDimension();
  for ( i=offset; i<offset+2*map->getDomainDimension(); i++ )
  {
    axis = (i-offset)/2;
    side = (i-offset)%2;
    sPrintF(buff, "%i", map->getShare(side,axis));
    dialog.setTextLabel(i, buff);
    buff="";
  }

//   offset = offset+2*map->getDomainDimension();

//   for ( i=offset; i<offset+map->getDomainDimension(); i++ )
//     {
//       axis = (i-offset);
//       sPrintF(buff, "%i", map->getIsPeriodic(axis));
//       dialog.setTextLabel(i, buff);
//       buff="";
//     }

  for( axis=0; axis<map->getDomainDimension(); axis++ )
    dialog.getOptionMenu(axis).setCurrentChoice((int)map->getIsPeriodic(axis));
}

static void 
buildMappingDialog(Mapping *map, DialogData & dialog)
// ==========================================================================================
// /Description:
//    Build the dialog defining the parameters generic to all Mappings.
// ==========================================================================================
{
  dialog.setWindowTitle("Mapping Parameters");

  aString pbCommands[] = { "show parameters",
			   // "plot",
			   "check",
			   "check inverse",
			   "" };

  aString pbLabels[] = { "Show Parameters",
			 //  "Plot",
			 "Check Mapping",
			 "Check Inverse",
			 "" };

  dialog.setPushButtons(pbCommands, pbLabels, 2);

  dialog.setExitCommand("close mapping dialog", "Close");

  aString tbInverseCommand[] = {"robust inverse", ""};
  aString tbInverseLabel[] = {"Robust Inverse", ""};
  int inverseOnOff = map->approximateGlobalInverse==NULL ? 0 : map->approximateGlobalInverse->usingRobustInverse();
  int tbState[] = { inverseOnOff };

  dialog.setToggleButtons(tbInverseCommand, tbInverseLabel, tbState,1);

  int offset = 2;
  int numberOfTextBoxes = offset+5*map->getDomainDimension();

  aString *textCommands = new aString[numberOfTextBoxes+1];
  aString *textLabels = new aString[numberOfTextBoxes+1];
  aString *textStrings = new aString[numberOfTextBoxes];

  textCommands[0] = "mappingName";
  textLabels[0] = "Mapping Name";
  textStrings[0] = " ";

  textCommands[1] = "lines";
  textLabels[1] = "Lines";
  textStrings[1] = " ";

  int i;
  int axis, side;
  const char *sides[] = { "left  ", 
		    "right ", 
		    "bottom", 
		    "top   ", 
		    "back  ", 
		    "front " };



  aString periodicLabel[] = {"not periodic",
			     "derivative periodic",
			     "function periodic",
                             ""}; 
  aString periodicCommand[4]={"","","",""};

  dialog.setOptionMenuColumns(1);
  aString buff;
  for( axis=0; axis<map->getDomainDimension(); axis++ )
  {
    aString prefix=sPrintF(buff,"periodicity: axis %i ",axis);
    GUIState::addPrefix(periodicLabel,prefix,periodicCommand,4);
    dialog.addOptionMenu(prefix, periodicCommand,periodicLabel,(int)map->getIsPeriodic(axis));
  }
  
  for ( i=offset; i<(offset+2*map->getDomainDimension()); i++ )
  {
    axis = (i-offset)/2;
    side = (i-offset)%2;
    sPrintF(textCommands[i], "Boundary Condition: %s ", sides[i-offset]);
    textLabels[i] = textCommands[i];
    textStrings[i] = " ";
  }


  for ( i=offset+2*map->getDomainDimension(); i<(offset+4*map->getDomainDimension()); i++ )
  {
    axis = (i-(offset+2*map->getDomainDimension()))/2;
    side = (i-(offset+2*map->getDomainDimension()))%2;
    sPrintF(textCommands[i], "Share Value: %s ", sides[i-(offset+2*map->getDomainDimension())]);
    textLabels[i] = textCommands[i];
    textStrings[i] = " ";
  }

//   for ( i=(offset+4*map->getDomainDimension()); i<numberOfTextBoxes; i++ )
//     {
//       axis = i-(offset+4*map->getDomainDimension());
//       sPrintF(textCommands[i], "Periodicity: axis %d ", axis);
//       textLabels[i] = textCommands[i];
//       textStrings[i] = " ";
//     }

  textCommands[numberOfTextBoxes] = textLabels[numberOfTextBoxes] = "";

  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  updateMappingDialogData(map, dialog);

  delete [] textCommands;
  delete [] textLabels;
  delete [] textStrings;
}

bool 
Mapping::
updateWithCommand(MappingInformation &mapInfo, const aString & command) 
// ===========================================================================================
/// \details 
///    Update one of the parameters common to all Mappings. This function is usually called by the update
///  function for the derived class. 
/// \param returns : true if the command was understood, false otherwise
// ===========================================================================================

{
  bool result = false; // if the command was not understood
  
  aString possibleCommands[] = { // original commands
    "lines",
    "boundary conditions",
    "mappingName",
    "share",
    "periodicity",
    "check",
    "use robust inverse",
    "do not use robust inverse",
    "robust inverse",
    "check inverse",
    // commands for new interface
    "build mapping dialog",
    "open mapping dialog",
    "mapping parameters", // same as open mapping dialog *wdh* 010805
    "update mapping dialog",
    "close mapping dialog",
    "Boundary Condition",
    "Periodicity",
    "Share Value",
    "" };

  int ncmd=0;
  for ( ncmd=0; possibleCommands[ncmd]!="" && !result; ncmd++ )
    result = possibleCommands[ncmd]==command(0,possibleCommands[ncmd].length()-1);

  if (result)
  {
    MappingInformation::commandOptions oldOption = mapInfo.commandOption;
    mapInfo.commandOption = MappingInformation::readOneCommand;
    mapInfo.command = (aString *)&command;
    Mapping::update(mapInfo);
    mapInfo.commandOption = oldOption;
  }

  return result;
				 
}

int Mapping::
update( MappingInformation & mapInfo ) 
// ===========================================================================================
/// \details 
///    Update parameters common to all Mappings. This function is usually called by the update
///  function for the derived class. 
// ===========================================================================================
{
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  aString option="";
  if( mapInfo.commandOption==MappingInformation::readOneCommand && mapInfo.command )
  {
    option=mapInfo.command[0];
  }

  aString line;  
  char buff[180];
  int len=0;
  bool getAnswersInteractively=false;
  GUIState gui;
  if ( option=="mapping parameters" && mapInfo.interface==NULL )
  {
    // build a new dialog (rather than a sibling).
    getAnswersInteractively=true;
    buildMappingDialog(this, gui); 
    gi.pushGUI(gui);

    if( !gi.readingFromCommandFile() )
    {
      gi.outputString("Make changes to mapping parameters. Close dialog to update the plot.");
    }
  }
  for( ;; )  // loop looking for answers -- if getAnswersInteractively==false we break after one pass.
  {
    if( getAnswersInteractively )
    {
      gi.getAnswer(option,"");
    }

    if ( option == "build mapping dialog" )
    {
      assert( mapInfo.interface != NULL );
      DialogData & interface = * mapInfo.interface;

      buildMappingDialog(this, interface);  
    }
    else if ( option=="mapping parameters" || option=="open mapping dialog" )
    {
      assert( mapInfo.interface!=NULL );
      // build a sibling interface for specifying parameters
      DialogData & interface = * mapInfo.interface;

      updateMappingDialogData( this, interface );
      interface.showSibling();
      interface.setSensitive(1);

    }
    else if ( option=="update mapping dialog" )
    {
      assert( mapInfo.interface != NULL );
      DialogData & interface = * mapInfo.interface;

      updateMappingDialogData( this, interface );
    }
    else if ( option == "close mapping dialog" )
    {
      if( !getAnswersInteractively )
      {
	assert( mapInfo.interface !=NULL );
	mapInfo.interface->hideSibling();
      }
      else
      {
        gi.popGUI();      
	getAnswersInteractively=false;   // this will cause us to break out of the interactive loop.
      }
      
    }
    else if( option=="lines" )
    {
      mappingHasChanged();  // could be more selective
      int gd[3];
      if( domainDimension==1 )
      {
	gi.inputString(line,sPrintF(buff,"Enter number of grid lines (default=%i, -1=no change)): ",getGridDimensions(0)));
	if( line!="" )
	{
	  sScanF( line,"%i",&gd[0]);
	  if( gd[0]>0 ) setGridDimensions(0,gd[0]);
	}
	
      }
      else if( domainDimension==2 )
      {
	gi.inputString(line,sPrintF(buff,"Enter number of grid lines (default=(%i,%i, -1=no change)): ",
				    getGridDimensions(0),getGridDimensions(1)));
	if( line!="" ) 
	{
	  sScanF( line,"%i %i",&gd[0],&gd[1]);
          if( gd[0]>0 ) setGridDimensions(0,gd[0]);
          if( gd[1]>0 ) setGridDimensions(1,gd[1]);
	}
	
      }
      else if( domainDimension==3 )
      {
	gi.inputString(line,sPrintF(buff,"Enter number of grid lines (default=(%i,%i,%i), -1=no change): ",
				    getGridDimensions(0),getGridDimensions(1),getGridDimensions(2)));
	if( line!="" )
	{
	  sScanF( line,"%i %i %i",&gd[0],&gd[1],&gd[2]);
          if( gd[0]>0 ) setGridDimensions(0,gd[0]);
          if( gd[1]>0 ) setGridDimensions(1,gd[1]);
          if( gd[2]>0 ) setGridDimensions(2,gd[2]);
	}
      }
      else
      {
	cout << "Mapping::update: unknown domain dimension = " << domainDimension << endl;
	gi.stopReadingCommandFile();
      }
    }
    else if( len=option.matches("lines") )
    {
      mappingHasChanged();
      int gd[3];
      if ( domainDimension==1 )
      {
	sScanF(option(len,option.length()-1), "%i", &gd[0]);
	if( gd[0]>0 ) setGridDimensions(0,gd[0]);
      }
      else if ( domainDimension==2 )
      {
	sScanF(option(len,option.length()-1), "%i %i", &gd[0], &gd[1]);
	if( gd[0]>0 ) setGridDimensions(0,gd[0]);
	if( gd[1]>0 ) setGridDimensions(1,gd[1]);
      }
      else if ( domainDimension==3 )
      {
	sScanF(option(len,option.length()-1), "%i %i %i", &gd[0], &gd[1], &gd[2]);
	if( gd[0]>0 ) setGridDimensions(0,gd[0]);
	if( gd[1]>0 ) setGridDimensions(1,gd[1]);
	if( gd[2]>0 ) setGridDimensions(2,gd[2]);
      }
      else 
      {
	cout << "Mapping::update: unknown domain dimension = " << domainDimension << endl;
	gi.stopReadingCommandFile();
      }
    } 
    else if( option=="boundary conditions" )
    {
      gi.outputString("Boundary conditions: positive=physical boundary, negative=periodic, 0=interpolation");
      gi.outputString("        bc(0,0)=left side, bc(1,0)=right side");
      if( domainDimension>1 )
	gi.outputString("        bc(0,1)=bottom,    bc(1,1)=top       ");
      if( domainDimension>2 )
	gi.outputString("        bc(0,2)=back,      bc(1,2)=front     ");
      if( domainDimension==1 )
      {
	gi.inputString(line,sPrintF(buff,"Current=(%i,%i), Enter bc for left, right ",bc[0][0],bc[1][0]));
	if( line!="" ) sScanF( line,"%i %i",&bc[0][0],&bc[1][0]);
      }
      else if( domainDimension==2 )
      {
	gi.inputString(line,sPrintF(buff,"Current=(%i,%i, %i,%i) Enter bc for left,right, bottom,top",
				    bc[0][0],bc[1][0],bc[0][1],bc[1][1] ));
	if( line!="" ) sScanF( line,"%i %i %i %i",&bc[0][0],&bc[1][0],&bc[0][1],&bc[1][1]);
      }
      else if( domainDimension==3 )
      {
	gi.inputString(line,sPrintF(buff,"Current=(%i,%i, %i,%i, %i,%i) Enter bc for left,right, bottom,top, back,front",
				    bc[0][0],bc[1][0],bc[0][1],bc[1][1],bc[0][2],bc[1][2] ));
	if( line!="" ) sScanF(line,"%i %i %i %i %i %i",&bc[0][0],&bc[1][0],&bc[0][1],&bc[1][1],
			      &bc[0][2],&bc[1][2]);
      }
      else
      {
	cout << "Mapping::update: unknown domain dimension = " << domainDimension << endl;
	gi.stopReadingCommandFile();
      }
      for( int axis=0; axis<domainDimension; axis++ )
      {
	if( bc[Start][axis]<0 && bc[End][axis]<0 )
	{
	  if( isPeriodic[axis]==0 )
	  {
	    // do a simple test to guess if this Mapping is "derivativePeriodic" or "functionPeriodic"
	    realArray r(2,3),x(2,3);
	    r=0.; x=0.;
	    r(1,axis)=1.;
	    map(r,x);
	    Range Rx(0,rangeDimension-1);
	    if( max(fabs(x(1,Rx)-x(0,Rx))) <= max(x)*REAL_EPSILON*100. )
	    {
	      printf("Mapping:info: setting periodicity=functionPeriodic for axis=%i \n",axis);
	      setIsPeriodic(axis,functionPeriodic);
	      mappingHasChanged(); 
	    }
	    else
	    {
	      printf("Mapping:info: setting periodicity=derivativePeriodic for axis=%i \n",axis);
	      setIsPeriodic(axis,derivativePeriodic);
	      mappingHasChanged(); 
	    }
	  }
	}
	else if( bc[Start][axis]<0 || bc[End][axis]<0 )
	{
	  printf("Mapping:Error: bc(Start,axis=%i)=%i and bc(End,axis=%i)=%i",
		 axis,bc[Start][axis],axis,bc[End][axis]);
	  cout << "Boundary conditions must be both non-positive or both non-negative \n";
	  cout << "Try again! \n";
	  bc[End][axis]=bc[Start][axis];
	}
	if( bc[Start][axis]>=0 && bc[End][axis]>=0 && getIsPeriodic(axis)!=notPeriodic )
	{
	  printf("Mapping: bc's are now non-negative on axis=%i, setting periodicity to notPeriodic \n",axis);
	  setIsPeriodic(axis,notPeriodic);
	  mappingHasChanged(); 
	}
      }
    
    }
    else if ( option(0,17) == "Boundary Condition" )
    {
      if ( option(20,23)=="left" )
      {
	sScanF(option(24,option.length()-1), "%i", &bc[0][0]);
	if (bc[1][0]<0 || bc[0][0]<0) bc[1][0] = bc[0][0];
      }
      else if ( option(20,24)=="right" )
      {
	sScanF(option(25,option.length()-1), "%i", &bc[1][0]);
	if (bc[1][0]<0 || bc[0][0]<0 ) bc[0][0] = bc[1][0];
      }
      else if ( option(20,25)=="bottom" )
      {
	sScanF(option(26,option.length()-1), "%i", &bc[0][1]);
	if ( bc[0][1]<0 || bc[1][1]<0 ) bc[1][1]=bc[0][1];
      }
      else if ( option(20,22)=="top" )
      {
	sScanF(option(23,option.length()-1), "%i", &bc[1][1]);
	if ( bc[0][1]<0 || bc[1][1]<0 ) bc[0][1] = bc[1][1];
      }
      else if ( option(20,23)=="back" )
      {
	sScanF(option(24,option.length()-1), "%i", &bc[0][2]);
	if ( bc[1][2]<0 ||bc[0][2]<0 ) bc[1][2]=bc[0][2];
      }
      else if ( option(20,24)=="front" )
      {
	sScanF(option(25,option.length()-1), "%i", &bc[1][2]);
	if ( bc[1][2]<0 || bc[0][2]<0) bc[0][2]=bc[1][2];
      }
      else
      {
	gi.outputString(aString("Mapping :: invalid boundary condition side ")+
			option(19,option.length()-1));
      }
      // the following lines were copped from the previous option=="boundary condition" 
      //     else statement
      for( int axis=0; axis<domainDimension; axis++ )
      {
	if( bc[Start][axis]<0 && bc[End][axis]<0 )
	{
	  if( isPeriodic[axis]==0 )
	  {
	    // do a simple test to guess if this Mapping is "derivativePeriodic" or "functionPeriodic"
	    realArray r(2,3),x(2,3);
	    r=0.; x=0.;
	    r(1,axis)=1.;
	    map(r,x);
	    Range Rx(0,rangeDimension-1);
	    if( max(fabs(x(1,Rx)-x(0,Rx))) <= max(x)*REAL_EPSILON*100. )
	    {
	      printf("Mapping:info: setting periodicity=functionPeriodic for axis=%i \n",axis);
	      setIsPeriodic(axis,functionPeriodic);
	      mappingHasChanged(); 
	    }
	    else
	    {
	      printf("Mapping:info: setting periodicity=derivativePeriodic for axis=%i \n",axis);
	      setIsPeriodic(axis,derivativePeriodic);
	      mappingHasChanged(); 
	    }
	  }
	}
	else if( bc[Start][axis]<0 || bc[End][axis]<0 )
	{
	  printf("Mapping:Error: bc(Start,axis=%i)=%i and bc(End,axis=%i)=%i",
		 axis,bc[Start][axis],axis,bc[End][axis]);
	  cout << "Boundary conditions must be both non-positive or both non-negative \n";
	  cout << "Try again! \n";
	  bc[End][axis]=bc[Start][axis];
	}
	if( bc[Start][axis]>=0 && bc[End][axis]>=0 && getIsPeriodic(axis)!=notPeriodic )
	{
	  printf("Mapping: bc's are now non-negative on axis=%i, setting periodicity to notPeriodic \n",axis);
	  setIsPeriodic(axis,notPeriodic);
	  mappingHasChanged(); 
	}
      }
    }
    else if( option=="mappingName" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the mapping name (default=%s)",(const char*)getName(mappingName)));
      if( line!="" ) setName(mappingName,line);
    }
    else if ( option(0,10)=="mappingName" )
    {
      line = option(12,option.length()-1);
      if ( line!="") setName(mappingName, line);
    }
    else if( option=="share" )
    {
      gi.outputString("The shared boundary flag should be set to a positive integer for a physical boundary.");
      gi.outputString("If the sides of two grids have the same shared flag value then the side are assummed");
      gi.outputString("to belong to the same boundary curve. This allows interpolation on boundaries.");
      gi.outputString("        share(0,0)=left side, share(1,0)=right side");
      if( domainDimension>1 )
	gi.outputString("        share(0,1)=bottom,    share(1,1)=top       ");
      if( domainDimension>2 )
	gi.outputString("        share(0,2)=back,      share(1,2)=front     ");
      if( domainDimension==1 )
      {
	gi.inputString(line,sPrintF(buff,"Current=(%i,%i), Enter share for left, right ",share[0][0],share[1][0]));
	if( line!="" ) sScanF( line,"%i %i",&share[0][0],&share[1][0]);
      }
      else if( domainDimension==2 )
      {
	gi.inputString(line,sPrintF(buff,"Current=(%i,%i, %i,%i) Enter share for left,right, bottom,top",
				    share[0][0],share[1][0],share[0][1],share[1][1] ));
	if( line!="" ) sScanF( line,"%i %i %i %i",&share[0][0],&share[1][0],&share[0][1],&share[1][1]);
      }
      else if( domainDimension==3 )
      {
	gi.inputString(line,sPrintF(buff,"Current=(%i,%i, %i,%i, %i,%i) Enter share for left,right, bottom,top, back,front",
				    share[0][0],share[1][0],share[0][1],share[1][1],share[0][2],share[1][2] ));
	if( line!="" ) sScanF(line,"%i %i %i %i %i %i",&share[0][0],&share[1][0],&share[0][1],&share[1][1],
			      &share[0][2],&share[1][2]);
      }
      else
      {
	cout << "Mapping::update: unknown domain dimension = " << domainDimension << endl;
	gi.stopReadingCommandFile();
      }
    }
    else if ( option(0,10) == "Share Value" )
    {
      if ( option(13,16)=="left" )
      {
	sScanF(option(17,option.length()-1), "%i", &share[0][0]);
      }
      else if ( option(13,17)=="right" )
      {
	sScanF(option(18,option.length()-1), "%i", &share[1][0]);
      }
      else if ( option(13,18)=="bottom" )
      {
	sScanF(option(19,option.length()-1), "%i", &share[0][1]);
      }
      else if ( option(13,15)=="top" )
      {
	sScanF(option(16,option.length()-1), "%i", &share[1][1]);
      }
      else if ( option(13,16)=="back" )
      {
	sScanF(option(17,option.length()-1), "%i", &share[0][2]);
      }
      else if ( option(13,17)=="front" )
      {
	sScanF(option(18,option.length()-1), "%i", &share[1][2]);
      }
      else
      {
	gi.outputString(aString("Mapping :: invalid share side ")+
			option(12,option.length()-1));
      }
    }
    else if( option.matches("periodicity") )
    {

      int periodic[3]={isPeriodic[0],isPeriodic[1],isPeriodic[2]}; //
      printF("Periodic boundaries are labeled with 0,1 or 2: \n"
	     " 0=not periodic, 1=derivative periodic, 2=function(=branch cut)\n"
	     "examples: An an annulus will have a branch cut, isPeriodic=2 \n"
	     "          A square may have derivative periodic, isPeriodic=1, in one or both directions\n"
	     " A periodic boundary will also have a boundary condition value of -1 (set automatically here).  \n\n");
      int axis;
      if( option=="periodicity" )
      {
	// old style
	printF(" current values for isPeriodic are: ");

	for( axis=0; axis<domainDimension; axis++ )    
	  printF("%i ",isPeriodic[axis]);
	printF("\n");
    
	if( domainDimension==1 )
	{
	  gi.inputString(line,sPrintF(buff,"Enter isPeriodic, current=%i (0=not, 1=derivative, 2=function(=branch cut))"
				      ,getIsPeriodic(0)));
	  if( line!="" ) sScanF( line,"%i",&periodic[0]);
	}
	else if( domainDimension==2 )
	{
	  gi.inputString(line,sPrintF(buff,"Enter isPeriodic, 2 integers, current=(%i,%i)"
				      " (0=not,1=derivative,2=function)",periodic[0],periodic[1]));
	  if( line!="" ) sScanF( line,"%i %i",&periodic[0],&periodic[1]);
	}
	else if( domainDimension==3 )
	{
	  gi.inputString(line,sPrintF(buff,"Enter isPeriodic, 3 integers, current=(%i,%i,%i) "
				      " (0=not,1=derivative,2=function)",periodic[0],periodic[1],periodic[2]));
	  if( line!="" ) sScanF( line,"%i %i %i",&periodic[0],&periodic[1],&periodic[2]);
	}
	else
	{
	  cout << "Mapping::update: unknown domain dimension = " << domainDimension << endl;
   	  gi.stopReadingCommandFile();
	}
      }
      else
      {
        cout << "option=[" << option << "]\n";
	int len=option.matches("periodicity: axis");
        int axis;
	sScanF(option(len,option.length()-1),"%i",&axis);
        if( axis>=0 && axis<domainDimension )
	{
          len+=2;
	  while( len<option.length()-2 )
	  {
	    if( option(len,len+2)=="not" )
	    {
	      periodic[axis]=(int)notPeriodic;
              break;
	    }
	    else if( option(len,len+2)=="der" )
	    {
              periodic[axis]=(int)derivativePeriodic;
              break;
	    }
	    else if( option(len,len+2)=="funt" )
	    {
              periodic[axis]=(int)functionPeriodic;
              break;
	    }
            len++;
	  }
          if( len>option.length()-2 )
	  {
	    gi.outputString(sPrintF(buff,"Unknown command: %s",(const char*)option));
  	    gi.stopReadingCommandFile();
	  }
	}
	else
	{
	  gi.outputString(sPrintF(buff,"Periodicity: Invalid value for axis=%i",axis));
	  gi.stopReadingCommandFile();
	}
      }
      
      mappingHasChanged();   
      for( axis=0; axis<domainDimension; axis++ )
      {
	if( periodic[axis] < 0 || periodic[axis] > 2 )
	{
	  cout << "Mapping::update: Error setting isPeriodic(" << axis << ")=" << isPeriodic[axis] 
	       << "??, answer must be in the range [0,2]\n ...setting isPeriodic(" << axis << ")=0\n";
	  isPeriodic[axis]=notPeriodic;
	}
	else
	{
	  isPeriodic[axis]=(periodicType)periodic[axis];
	}
      
	// Make sure the BC array is consistent
	if( isPeriodic[axis]>0 )
	{
	  if( bc[Start][axis]>=0 )
	    bc[Start][axis]=-1;
	  if( bc[End][axis]>=0 )
	    bc[End][axis]=-1;
	}
	else 
	{
	  if( bc[Start][axis]<0 )
	    bc[Start][axis]=1;
	  if( bc[End][axis]<0 )
	    bc[End][axis]=1;
	}
      }
    }
//     else if (option(0,10) == "Periodicity" )
//     {
//       int axis,per;
//       sScanF(option(17,option.length()-1),"%i %i",&axis, &per);
//       if ( axis<0 || axis>=domainDimension )
//       {
// 	gi.outputString(aString("invalid axis specification for Periodicity change : ")
// 			+option(11,option.length()-1));
//       }
//       else
//       { 
// 	if ( per<0 || per>2 )
// 	{
// 	  cout << "Mapping::update: Error setting isPeriodic(" << axis << ")=" << isPeriodic[axis] 
// 	       << "??, answer must be in the range [0,2]\n ...setting isPeriodic(" << axis << ")=0\n";
// 	  isPeriodic[axis]=notPeriodic;
// 	}
// 	else
// 	{
// 	  isPeriodic[axis] = periodicType(per);
// 	}
// 	// Make sure the BC array is consistent
// 	if( isPeriodic[axis]>0 )
// 	{
// 	  if( bc[Start][axis]>=0 )
// 	    bc[Start][axis]=-1;
// 	  if( bc[End][axis]>=0 )
// 	    bc[End][axis]=-1;
// 	}
// 	else 
// 	{
// 	  if( bc[Start][axis]<0 )
// 	    bc[Start][axis]=1;
// 	  if( bc[End][axis]<0 )
// 	    bc[End][axis]=1;
	    
// 	}
//       }
//     }
    else if( option=="check" )
    {
      checkMapping();
    }
    else if( option=="use robust inverse" )
    {
      if( approximateGlobalInverse!=0 )
	approximateGlobalInverse->useRobustInverse(TRUE);
    }
    else if( option=="do not use robust inverse" )
    {
      if( approximateGlobalInverse!=0 )
	approximateGlobalInverse->useRobustInverse(FALSE);
    }
    else if ( option(0,13)=="robust inverse" )
    {
      if ( approximateGlobalInverse!=0 )
	approximateGlobalInverse->useRobustInverse( ! approximateGlobalInverse->usingRobustInverse() );
    }
    else if( option=="check derivative" )
    {
      realArray x(2,3),r(1,3),xx(4,3),xr(1,3,3);
      realArray t4(4,3), dx(1,3,3);
      Range Axes=domainDimension;
      Range xAxes=rangeDimension;

      const real epsilon=REAL_EPSILON;
      const real hmax=1./128.;
      const real h=min(hmax,pow(epsilon/20.,1./5.)); // optimal h for 4th order differences
      const real d=SQR(h);
      
      Range Rx(0,rangeDimension-1);
      aString answer;
      aString menu[]=
      {
	"enter an r value",
	"done",
	""
      };
      int axis;
      for( int i=0;;i++ )
      {
	gi.getMenuItem(menu,answer,"choose an option");
	
	if( answer=="done" )
	  break;
	else if( answer=="enter an r value" )
	{
	  gi.inputString(answer,"Enter a point r");

	  sScanF(answer,"%e %e %e",&r(0,0),&r(0,1),&r(0,2));
	  mapC(r,x,xr);

	  for( int ii=0; ii<4; ii++ )
	    t4(ii,Axes)=r(0,Axes);

	  real anorm=0.;
	  for( axis=axis1; axis<domainDimension; axis++ )
	  {
	    t4(0,axis)=r(0,axis)-1.5*h;
	    t4(1,axis)=r(0,axis)-0.5*h;
	    t4(2,axis)=r(0,axis)+0.5*h;
	    t4(3,axis)=r(0,axis)+1.5*h;
	    mapC( t4,xx );	  
	    t4(Range(0,3),axis)=r(0,axis);
	    

	    // 4th order accurate approximation to the first derivative:
	    dx(0,xAxes,axis)=(27.*(xx(2,xAxes)-xx(1,xAxes))-(xx(3,xAxes)-xx(0,xAxes)))/(24.*h);

	    anorm+=sum(SQR(dx(0,xAxes,axis)));
	  }
          printf(" r=(%6.3f,%6.3f,%6.3f) (h for difference approx = %10.2e) \n",
                   r(0,axis1),r(0,axis2),domainDimension==2 ? 0. : r(0,axis3),h);
	  
	  anorm=SQRT(anorm);
	  for( axis=0; axis<domainDimension; axis++ )
	  {
	    for( int dir=0; dir<rangeDimension; dir++ )
	    {
	      if( fabs(dx(0,dir,axis)-xr(0,dir,axis)) > d*anorm )
	      {
		printf("Warning: xr(%i,%i)=%12.4e is inaccurate, from differences xr=%12.4e, diff=%10.2e \n",
		       dir,axis,xr(0,dir,axis),dx(0,dir,axis),fabs(xr(0,dir,axis)-dx(0,dir,axis)));
	      }
	      else
	      {
                printf("  xr(%i,%i)=%15.7e  : from differences: xr(%i,%i)=%15.7e, diff=%10.2e\n",
                       dir,axis,xr(0,dir,axis),dir,axis,dx(0,dir,axis),fabs(xr(0,dir,axis)-dx(0,dir,axis)));
	      }
	      
	    }
	  }
	}
	else
	{
	  printf("Unknown response: %s \n",(const char *)answer);
	  gi.stopReadingCommandFile();
	}

      } // end for i
      
    }
    else if( option=="check inverse" )
    {
      const int debugOld=Mapping::debug;
      Mapping::debug=3;
      GraphicsParameters parameters;
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

      RealArray x(2,3),r(1,3),xx(1,3),xr(1,3,3);
      r=-1.;
      x=.1;  // raise up to plot
      Range Rx(0,rangeDimension-1);
      aString answer;
      aString menu[]=
      {
	"enter a point",
	"enter multiple points",
	"enter an r value",
	"use robust inverse",
	"do not use robust inverse",
        "debug",
	"done",
	""
      };
      for( int i=0;;i++ )
      {
	gi.getMenuItem(menu,answer,"choose an option");
	
	if( answer=="done" )
	  break;
	else if( answer=="debug" )
	{
          gi.inputString(answer,"Enter debug (e.g. 15)");
          sScanF(answer,"%i",&debug);
	  printF("Setting debug=%i\n",debug);
	}
	else if( answer=="enter a point" )
	{
	  gi.inputString(answer,"Enter a point (x,y,z) to invert");

	  sScanF(answer,"%e %e %e",&x(0,0),&x(0,1),&x(0,2));
	  r=-1.;
          #ifdef USE_PPP
 	   inverseMapS(x(0,Rx),r);
	   mapS(r,xx);
          #else
	   inverseMap(x(0,Rx),r);
	   map(r,xx);
          #endif
	  x(1,Rx)=xx(0,Rx);
	  printf("checkInverse: x=(%12.8e,%12.8e,%12.8e), r=(%12.8e,%12.8e,%12.8e), projected x=(%12.8e,%12.8e,%12.8e)\n",
		 x(0,0),x(0,1),(rangeDimension==2  ? 0. : x(0,2)), 
		 r(0,0),r(0,1),(domainDimension==2 ? 0. : r(0,2)), 
		 x(1,0),x(1,1),(rangeDimension==2  ? 0. : x(1,2)));

	  gi.erase();
	  PlotIt::plot(gi, *this, parameters);  
	  parameters.set(GI_USE_PLOT_BOUNDS,TRUE);
	  parameters.set(GI_POINT_SIZE,(real)6.);
	  gi.plotPoints(x,parameters);
	  parameters.set(GI_USE_PLOT_BOUNDS,FALSE);
	}
	else if( answer=="enter multiple points" )
	{
          RealArray r,x(10,3),xx;
          r=-1;
          int i, num=1;
	  for( i=0; i<10; i++ )
	  {
            answer="";
   	    gi.inputString(answer,"Enter a point (x,y,z) to invert. (enter 'done' finish)");
   	    sScanF(answer,"%e %e %e",&x(i,0),&x(i,1),&x(i,2));
 
            if( answer=="" || answer=="done" ) 
	    {
	      num=i;
	      break;
	    }
	  }
          if( num==0 ) continue;
          x.resize(num,Rx);
	  r.redim(num,Rx);
          xx.redim(num,Rx);
	  r=-1.;
          #ifdef USE_PPP
	    inverseMapS(x,r);
	    mapS(r,xx);
          #else
	    inverseMap(x,r);
	    map(r,xx);
          #endif

          for( i=0; i<num; i++ )
	  {
	    printF("i=%i x=(%12.8e,%12.8e,%12.8e), r=(%12.8e,%12.8e,%12.8e), projected x=(%12.8e,%12.8e,%12.8e)\n",i,
		   x(i,0),x(i,1),(rangeDimension==2  ? 0. : x(i,2)), 
		   r(i,0),r(i,1),(domainDimension==2 ? 0. : r(i,2)), 
		   xx(i,0),xx(i,1),(rangeDimension==2  ? 0. : xx(i,2)));
	  }

	  gi.erase();
	  PlotIt::plot(gi, *this, parameters);  
	  parameters.set(GI_USE_PLOT_BOUNDS,TRUE);
	  parameters.set(GI_POINT_SIZE,(real)6.);
	  gi.plotPoints(x,parameters);
	  gi.plotPoints(xx,parameters);
	  parameters.set(GI_USE_PLOT_BOUNDS,FALSE);

	}
	else if( answer=="enter an r value" )
	{
	  gi.inputString(answer,"Enter a point r");

	  sScanF(answer,"%e %e %e",&r(0,0),&r(0,1),&r(0,2));
          #ifdef USE_PPP
  	    mapCS(r,x(0,Rx),xr);
          #else
   	    mapC(r,x(0,Rx),xr);
          #endif
          RealArray rr(1,3);
          rr=-1.;
          #ifdef USE_PPP
  	    inverseMapS(x(0,Rx),rr);
          #else
	    inverseMapC(x(0,Rx),rr);
          #endif
	  
	  printf(" r=(%12.8e,%12.8e,%12.8e), x=(%12.8e,%12.8e,%12.8e) -> inverseMap rr=(%12.8e,%12.8e,%12.8e)\n",
		 r(0,0),r(0,1),(domainDimension==2 ? 0. : r(0,2)), 
		 x(0,0),x(0,1),(rangeDimension==2  ? 0. : x(0,2)),
                 rr(0,0),rr(0,1),(domainDimension==2 ? 0. : rr(0,2)));


	  if( domainDimension==2 && rangeDimension==3 )
	  {
	    real nv[3];
	    nv[0] = xr(0,1,0)*xr(0,2,1)-xr(0,2,0)*xr(0,1,1);
	    nv[1] = xr(0,2,0)*xr(0,0,1)-xr(0,0,0)*xr(0,2,1);
	    nv[2] = xr(0,0,0)*xr(0,1,1)-xr(0,1,0)*xr(0,0,1);
	    real anorm =max(REAL_MIN*100.,sqrt(SQR(nv[0])+SQR(nv[1])+SQR(nv[2]))); 
	    nv[0]/=anorm; nv[1]/=anorm; nv[2]/=anorm;

	    printf(" xr = (%8.2e,%8.2e,%8.2e), xs=(%8.2e,%8.2e,%8.2e), normal=(%8.2e,%8.2e,%8.2e)\n",
                   xr(0,0,0),xr(0,1,0),xr(0,2,0),
                   xr(0,0,1),xr(0,1,1),xr(0,2,1),nv[0],nv[1],nv[2]);
	  }
	  

          if( domainDimension==3 && rangeDimension==3 )
	  {
            real tripleProduct=( (xr(0,0,0)*xr(0,1,1)-xr(0,0,1)*xr(0,1,0))*xr(0,2,2) +
				 (xr(0,0,1)*xr(0,1,2)-xr(0,0,2)*xr(0,1,1))*xr(0,2,0) +
				 (xr(0,0,2)*xr(0,1,0)-xr(0,0,0)*xr(0,1,2))*xr(0,2,1) );

            printF(" xr = (%8.2e,%8.2e,%8.2e), xs=(%8.2e,%8.2e,%8.2e) xt=(%8.2e,%8.2e,%8.2e)\n"
                   " xr X xs o xt = %8.2e  signForJacobian = %f \n",
                   xr(0,0,0),xr(0,1,0),xr(0,2,0),
                   xr(0,0,1),xr(0,1,1),xr(0,2,1),
                   xr(0,0,2),xr(0,1,2),xr(0,2,2),tripleProduct,getSignForJacobian());
	  }

	  gi.erase();
	  PlotIt::plot(gi, *this, parameters);  
	  parameters.set(GI_USE_PLOT_BOUNDS,TRUE);
	  parameters.set(GI_POINT_SIZE,(real)6.);
	  gi.plotPoints(x,parameters);
	  parameters.set(GI_USE_PLOT_BOUNDS,FALSE);
	}
      
	else if( answer=="use robust inverse" )
	{
	  if( approximateGlobalInverse!=0 )
	    approximateGlobalInverse->useRobustInverse(TRUE);
	}
	else if( answer=="do not use robust inverse" )
	{
	  if( approximateGlobalInverse!=0 )
	    approximateGlobalInverse->useRobustInverse(FALSE);
	}
	else
	{
	  printf("Unknown response: %s \n",(const char *)answer);
	  gi.stopReadingCommandFile();
	}
      }

      Mapping::debug=debugOld;
    }
    else
    {
      cout << "Mapping::update: unknown option : [" << (const char*) option << "]\n";
      cout << "The base class is being called for Mapping [" << getClassName() << "]\n";
    }
    
    if( !getAnswersInteractively ) break;
    updateMappingDialogData( this, gui );
  }
  
  return 0;
}


int Mapping::
interactiveUpdate( GenericGraphicsInterface & gi )
// ===========================================================================================
/// \details 
///    Update Mapping parameters. This virtual function will call the update function for the
///  derived Mapping. Use this function if you don't need to pass other Mapping information.
/// \param gi (input) : use this graphics interface.
// ===========================================================================================
{
  MappingInformation mapInfo;
  mapInfo.graphXInterface=&gi;
  return update( mapInfo );
}


// ==========================================================================================
// \brief Build the dialog defining the parameters generic to all Mappings.
// ==========================================================================================
int Mapping::
buildMappingParametersDialog(DialogData & dialog)
{
  Mapping & map = *this;

  const int domainDimension = map.getDomainDimension();
  const int rangeDimension = map.getRangeDimension();
  
  dialog.setWindowTitle("Mapping Parameters");
  dialog.setExitCommand("close mapping parameters", "close");


  // option menus
  // dialog.setOptionMenuColumns(1);

  // **** finish periodicity: reading answers, also adjust BC and periodicity text boxes to be consistent *******


//   aString opCommand1[4];
//   aString opLabel1[] = {"not periodic",
// 			"derivative periodic",
// 			"function periodic",
// 			""};
//   for( int axis=0; axis<domainDimension; axis++ )
//   {
//     sPrintF(opCommand1[0],"periodicity: axis %i not periodic"       ,axis);
//     sPrintF(opCommand1[1],"periodicity: axis %i derivative periodic",axis);
//     sPrintF(opCommand1[2],"periodicity: axis %i function periodic"  ,axis);
//     opCommand1[3]="";
//     dialog.addOptionMenu(sPrintF("periodicity: axis %i",axis), opCommand1, opLabel1, (int)map.getIsPeriodic(axis1));
//   }
  
  aString pbCommands[] = { "show parameters",
			   "check",
			   "check inverse",
			   "" };

  int numRows=2;
  dialog.setPushButtons(pbCommands, pbCommands, numRows);

  // --- toggle buttons --
  aString tbCommands[] = {"use robust inverse",
 			  ""};
  bool useRobustInverse=map.approximateGlobalInverse!=0 ? map.approximateGlobalInverse->usingRobustInverse() : false;
  int tbState[10];
  tbState[0] = useRobustInverse;
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);


  const int numberOfTextStrings=20;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];


  int nt=0;
  textLabels[nt] = "lines:";  
  if( domainDimension==1 )
    sPrintF(textStrings[nt],"%i",map.getGridDimensions(0));
  else if( domainDimension==2 )
    sPrintF(textStrings[nt],"%i %i",map.getGridDimensions(0),map.getGridDimensions(1));
  else
    sPrintF(textStrings[nt],"%i %i %i",map.getGridDimensions(0),map.getGridDimensions(1),map.getGridDimensions(2));
  nt++;
  
  textLabels[nt] = "boundary conditions:";  
  if( domainDimension==1 )
    sPrintF(textStrings[nt],"%i %i (left,right)",
            map.getBoundaryCondition(0,0),map.getBoundaryCondition(1,0));
  else if( domainDimension==2 )
    sPrintF(textStrings[nt],"%i %i %i %i (left,right,bot,top)",
	    map.getBoundaryCondition(0,0),map.getBoundaryCondition(1,0),
	    map.getBoundaryCondition(0,1),map.getBoundaryCondition(1,1));
  else
    sPrintF(textStrings[nt],"%i %i %i %i %i %i (left,right,bot,top,back,front)",
	    map.getBoundaryCondition(0,0),map.getBoundaryCondition(1,0),
	    map.getBoundaryCondition(0,1),map.getBoundaryCondition(1,1),
	    map.getBoundaryCondition(0,2),map.getBoundaryCondition(1,2));
  nt++;
  
  textLabels[nt] = "shared boundary flag:";  
  if( domainDimension==1 )
    sPrintF(textStrings[nt],"%i %i (left,right)",
            map.getShare(0,0),map.getShare(1,0));
  else if( domainDimension==2 )
    sPrintF(textStrings[nt],"%i %i %i %i (left,right,bot,top)",
	    map.getShare(0,0),map.getShare(1,0),
	    map.getShare(0,1),map.getShare(1,1));
  else
    sPrintF(textStrings[nt],"%i %i %i %i %i %i (left,right,bot,top,back,front)",
	    map.getShare(0,0),map.getShare(1,0),
	    map.getShare(0,1),map.getShare(1,1),
	    map.getShare(0,2),map.getShare(1,2));
  nt++;
  
  textLabels[nt] = "periodicity:";  
  if( domainDimension==1 )
    sPrintF(textStrings[nt],"%i (0=not,1=derivative,2=function)",(int)map.getIsPeriodic(0));
  else if( domainDimension==2 )
    sPrintF(textStrings[nt],"%i %i (0=not,1=derivative,2=function)",(int)map.getIsPeriodic(0),
            (int)map.getIsPeriodic(1));
  else
    sPrintF(textStrings[nt],"%i %i %i (0=not,1=derivative,2=function)",(int)map.getIsPeriodic(0),
            (int)map.getIsPeriodic(1),(int)map.getIsPeriodic(2));
  nt++;
  


  textLabels[nt] = "name:";  textStrings[nt]=map.getName(Mapping::mappingName); nt++;

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  return 0;

}

// ==========================================================================================
// \brief Update the Mapping Parameters dialog to be consistent with the current parameters.
// ==========================================================================================
int Mapping::
updateMappingParametersDialog(DialogData & dialog)
{
  Mapping & map =*this;
  
  aString line;

  if( domainDimension==1 )
    sPrintF(line,"%i",map.getGridDimensions(0));
  else if( domainDimension==2 )
    sPrintF(line,"%i %i",map.getGridDimensions(0),map.getGridDimensions(1));
  else
    sPrintF(line,"%i %i %i",map.getGridDimensions(0),map.getGridDimensions(1),map.getGridDimensions(2));

  dialog.setTextLabel("lines:",line);


  if( domainDimension==1 )
    sPrintF(line,"%i %i (left,right)",
	    map.getBoundaryCondition(0,0),map.getBoundaryCondition(1,0));
  else if( domainDimension==2 )
    sPrintF(line,"%i %i %i %i (left,right,bot,top)",
	    map.getBoundaryCondition(0,0),map.getBoundaryCondition(1,0),
	    map.getBoundaryCondition(0,1),map.getBoundaryCondition(1,1));
  else
    sPrintF(line,"%i %i %i %i %i %i (left,right,bot,top,back,front)",
	    map.getBoundaryCondition(0,0),map.getBoundaryCondition(1,0),
	    map.getBoundaryCondition(0,1),map.getBoundaryCondition(1,1),
	    map.getBoundaryCondition(0,2),map.getBoundaryCondition(1,2));
  dialog.setTextLabel("boundary conditions:",line);

  if( domainDimension==1 )
    sPrintF(line,"%i (0=not,1=derivative,2=function)",(int)map.getIsPeriodic(0));
  else if( domainDimension==2 )
    sPrintF(line,"%i %i (0=not,1=derivative,2=function)",(int)map.getIsPeriodic(0),
	    (int)map.getIsPeriodic(1));
  else
    sPrintF(line,"%i %i %i (0=not,1=derivative,2=function)",(int)map.getIsPeriodic(0),
	    (int)map.getIsPeriodic(1),(int)map.getIsPeriodic(2));

  dialog.setTextLabel("periodicity:",line);

  dialog.setTextLabel("name:",sPrintF(line, "%s", (const char*)map.getName(Mapping::mappingName))); 

  return 0;
}




//================================================================================
/// \brief: Look for a change to one of the mapping parameters.
///
/// \param answer (input) : check this command 
///
/// \return return 1 if the command was found, 0 otherwise.
//====================================================================
int Mapping::
getMappingParametersOption( const aString & answer, 
                            DialogData & dialog,
                            GenericGraphicsInterface & gi  )
{
  Mapping & map = *this;

  int found=true; 
  bool useRobustInverse=false;
  bool updateBoundaryConditionText=false;  // set to true if the BC text box needs to be updated
  bool updatePeriodicText=false;  // set to true if the periodic text box needs to be updated

  const int domainDimension = map.getDomainDimension();
  const int rangeDimension = map.getRangeDimension();
  aString line;
  int len=0;

  if( answer=="mapping parameters..." )
  {
    printF(" mappingParametersDialog.showSibling()...\n");
    dialog.showSibling();
  }
  else if( answer=="close mapping parameters" )
  {
    dialog.hideSibling();  // close sibling dialog
  }
  else if( len=answer.matches("lines:") )
  {
    int n[3]={-1,-1,-1}; 
    sScanF(answer(len+1,answer.length()-1),"%i %i %i",&n[0],&n[1],&n[2]);
    for( int axis=0; axis<domainDimension; axis++ )
    {
      if( n[axis]>0 )
        map.setGridDimensions(axis,n[axis]);
      else if( n[axis]!=-1 ) // -1 : use current value
      {
	printF("getMappingParametersOption:ERROR: Invalid number of lines =%i for axis %i. Ignoring.\n",
	       n[axis],axis);
      }
      
    }
    if( domainDimension==1 )
      sPrintF(line,"%i",map.getGridDimensions(0));
    else if( domainDimension==2 )
      sPrintF(line,"%i %i",map.getGridDimensions(0),map.getGridDimensions(1));
    else
      sPrintF(line,"%i %i %i",map.getGridDimensions(0),map.getGridDimensions(1),map.getGridDimensions(2));

    dialog.setTextLabel("lines:",line);

    // map.mappingHasChanged();
  }
  else if( len=answer.matches("boundary conditions:") )
  {
    gi.outputString("Boundary conditions: positive=physical boundary, negative=periodic, 0=interpolation");
    gi.outputString("        bc(0,0)=left side, bc(1,0)=right side");
    if( domainDimension>1 )
      gi.outputString("        bc(0,1)=bottom,    bc(1,1)=top       ");
    if( domainDimension>2 )
      gi.outputString("        bc(0,2)=back,      bc(1,2)=front     ");

    int pbbc[6]={0,0,0,0,0,0}; 
    #define bbc(side,axis) pbbc[(side)+2*(axis)]
    sScanF(answer(len+1,answer.length()-1),"%i %i %i %i %i %i",&bbc(0,0),&bbc(1,0),&bbc(0,1),&bbc(1,1),
          &bbc(0,2),&bbc(1,2));
    for( int side=0; side<=1; side++ )for( int axis=0; axis<domainDimension; axis++ )
    {
      // map.setBoundaryCondition(side,axis,bc(side,axis));
      bc[side][axis]=bbc(side,axis);
    }
    #undef bbc

    for( int axis=0; axis<domainDimension; axis++ )
    {
      if( bc[Start][axis]<0 && bc[End][axis]<0 )
      {
	if( isPeriodic[axis]==0 )
	{
	  // do a simple test to guess if this Mapping is "derivativePeriodic" or "functionPeriodic"
	  RealArray r(2,3),x(2,3);
	  r=0.; x=0.;
	  r(1,axis)=1.;
          #ifdef USE_PPP
  	    map.mapS(r,x);
          #else
  	    map.map(r,x);
          #endif
	  Range Rx(0,rangeDimension-1);
	  if( max(fabs(x(1,Rx)-x(0,Rx))) <= max(x)*REAL_EPSILON*100. )
	  {
	    printF("Mapping:info: setting periodicity=functionPeriodic for axis=%i \n",axis);
	    setIsPeriodic(axis,functionPeriodic);
	    mappingHasChanged(); 
	  }
	  else
	  {
	    printF("Mapping:info: setting periodicity=derivativePeriodic for axis=%i \n",axis);
	    setIsPeriodic(axis,derivativePeriodic);
	    mappingHasChanged(); 
	  }
	}
      }
      else if( bc[Start][axis]<0 || bc[End][axis]<0 )
      {
	printF("Mapping:Error: bc(Start,axis=%i)=%i and bc(End,axis=%i)=%i",
	       axis,bc[Start][axis],axis,bc[End][axis]);
	printF("Boundary conditions must be both non-positive or both non-negative \n"
	       "Try again! \n");
	bc[End][axis]=bc[Start][axis];
      }
      if( bc[Start][axis]>=0 && bc[End][axis]>=0 && getIsPeriodic(axis)!=notPeriodic )
      {
	printF("Mapping: bc's are now non-negative on axis=%i, setting periodicity to notPeriodic \n",axis);
	setIsPeriodic(axis,notPeriodic);
	mappingHasChanged(); 
      }
    }
    updateBoundaryConditionText=true;
    updatePeriodicText=true;

  }
  else if( len=answer.matches("shared boundary flag:") )
  {
    int pshare[6]={0,0,0,0,0,0}; 
    #define share(side,axis) pshare[(side)+2*(axis)]
    sScanF(answer(len+1,answer.length()-1),"%i %i %i %i %i %i",&share(0,0),&share(1,0),&share(0,1),&share(1,1),
           &share(0,2),&share(1,2));
    for( int side=0; side<=1; side++ )for( int axis=0; axis<domainDimension; axis++ )
    {
      map.setShare(side,axis,share(side,axis));
    }
    #undef share

    if( domainDimension==1 )
      sPrintF(line,"%i %i (left,right)",
	      map.getShare(0,0),map.getShare(1,0));
    else if( domainDimension==2 )
      sPrintF(line,"%i %i %i %i (left,right,bot,top)",
	      map.getShare(0,0),map.getShare(1,0),
	      map.getShare(0,1),map.getShare(1,1));
    else
      sPrintF(line,"%i %i %i %i %i %i (left,right,bot,top,back,front)",
	      map.getShare(0,0),map.getShare(1,0),
	      map.getShare(0,1),map.getShare(1,1),
	      map.getShare(0,2),map.getShare(1,2));
    // if( !gi.isGraphicsWindowOpen() )
    dialog.setTextLabel("shared boundary flag:",line);


  }
  else if( len=answer.matches("periodicity:") )
  {
    printF("Periodic boundaries are labeled with 0,1 or 2: \n"
	   " 0=not periodic, 1=derivative periodic, 2=function(=branch cut)\n"
	   "examples: An an annulus will have a branch cut, isPeriodic=2 \n"
	   "          A square may have derivative periodic, isPeriodic=1, in one or both directions\n"
	   " A periodic boundary will also have a boundary condition value of -1 (set automatically here).  \n\n");

    int pper[3]={0,0,0}; 
    #define per(axis) pper[(axis)]
    sScanF(answer(len+1,answer.length()-1),"%i %i %i",&per(0),&per(1),&per(2));
    for( int axis=0; axis<domainDimension; axis++ )
    {
      map.setIsPeriodic(axis,Mapping::periodicType(per(axis)));
    }

    mappingHasChanged();   
    for( int axis=0; axis<domainDimension; axis++ )
    {
      if( per(axis) < 0 || per(axis) > 2 )
      {
	printF("Mapping::update: Error setting isPeriodic(%i)=%i ??, answer must be in the range [0,2]\n"
               " ...setting isPeriodic(%i)=0 (not periodic)\n",axis,isPeriodic[axis],axis);
	
	isPeriodic[axis]=notPeriodic;
      }
      else
      {
	isPeriodic[axis]=(periodicType)per(axis);
      }
      #undef per
      
      // Make sure the BC array is consistent
      if( isPeriodic[axis]>0 )
      {
	if( bc[Start][axis]>=0 )
	  bc[Start][axis]=-1;
	if( bc[End][axis]>=0 )
	  bc[End][axis]=-1;
      }
      else 
      {
	if( bc[Start][axis]<0 )
	  bc[Start][axis]=1;
	if( bc[End][axis]<0 )
	  bc[End][axis]=1;
      }
    }

    updateBoundaryConditionText=true;
    updatePeriodicText=true;
  }
  else if( len=answer.matches("name:") )
  {
    map.setName(Mapping::mappingName,answer(len,answer.length()-1));
    dialog.setTextLabel("name:",sPrintF(line, "%s", (const char*)map.getName(Mapping::mappingName))); 
  }
  else if( dialog.getToggleValue(answer,"use robust inverse",useRobustInverse) )
  {
    if( map.approximateGlobalInverse!=0 )
      map.approximateGlobalInverse->useRobustInverse(useRobustInverse);
  } 
  // *** finish me: ***
//   else if( len=answer.matches("periodicity: axis") 
//   {
//   }
  else
  {
    found=false;
  }
  
  if( updateBoundaryConditionText )
  {
    if( domainDimension==1 )
      sPrintF(line,"%i %i (left,right)",
	      map.getBoundaryCondition(0,0),map.getBoundaryCondition(1,0));
    else if( domainDimension==2 )
      sPrintF(line,"%i %i %i %i (left,right,bot,top)",
	      map.getBoundaryCondition(0,0),map.getBoundaryCondition(1,0),
	      map.getBoundaryCondition(0,1),map.getBoundaryCondition(1,1));
    else
      sPrintF(line,"%i %i %i %i %i %i (left,right,bot,top,back,front)",
	      map.getBoundaryCondition(0,0),map.getBoundaryCondition(1,0),
	      map.getBoundaryCondition(0,1),map.getBoundaryCondition(1,1),
	      map.getBoundaryCondition(0,2),map.getBoundaryCondition(1,2));
    // if( !gi.isGraphicsWindowOpen() )
    dialog.setTextLabel("boundary conditions:",line);
  }
  if( updatePeriodicText )
  {
    if( domainDimension==1 )
      sPrintF(line,"%i (0=not,1=derivative,2=function)",(int)map.getIsPeriodic(0));
    else if( domainDimension==2 )
      sPrintF(line,"%i %i (0=not,1=derivative,2=function)",(int)map.getIsPeriodic(0),
	      (int)map.getIsPeriodic(1));
    else
      sPrintF(line,"%i %i %i (0=not,1=derivative,2=function)",(int)map.getIsPeriodic(0),
	      (int)map.getIsPeriodic(1),(int)map.getIsPeriodic(2));

    dialog.setTextLabel("periodicity:",line);
    
  }
  
  return found;
  
}

// Declare and define base and bounds, perform loop
#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base=I1.getBase(), I2Base=I2.getBase(), I3Base=I3.getBase();\
  int I1Bound=I1.getBound(), I2Bound=I2.getBound(), I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

static inline 
double
tetVolume6(real *p1, real*p2, real *p3, real *p4 )
{
  // Rteurn 6 times the volume of the tetrahedra
  // (p2-p1)x(p3-p1) points in the direction of p4 ( p1,p2,p3 are counter clockwise viewed from p4 )
  // 6 vol = (p4-p1) . ( (p2-p1)x(p3-p1) )
  return  ( (p4[0]-p1[0])*( (p2[1]-p1[1])*(p3[2]-p1[2]) - (p2[2]-p1[2])*(p3[1]-p1[1]) ) -
	    (p4[1]-p1[1])*( (p2[0]-p1[0])*(p3[2]-p1[2]) - (p2[2]-p1[2])*(p3[0]-p1[0]) ) +
	    (p4[2]-p1[2])*( (p2[0]-p1[0])*(p3[1]-p1[1]) - (p2[1]-p1[1])*(p3[0]-p1[0]) ) ) ;
	  
}

static inline 
real
hexVolume( real *v000, real *v100, real *v010, real *v110, real *v001, real *v101, 
           real *v011, real *v111 )
// =====================================================================================================
// Return true if the hex defined by the vertices v000,v100,... has any tetrahedra that are negative.
// =====================================================================================================
{
  return (tetVolume6(v000,v100,v010, v001)+
	  tetVolume6(v110,v010,v100, v111)+
	  tetVolume6(v101,v001,v111, v100)+
	  tetVolume6(v011,v111,v001, v010)+
	  tetVolume6(v100,v010,v001, v111));
}

void Mapping::
gridStatistics( Mapping & map, RealArray & gridStats, FILE *file /* =stdout */ )
// =================================================================================================
/// \brief Compute statistics about the grid
///  
///     - min,ave,max volumes: gridStats(0:2)
///     - min,ave,max grid spacing along each axis : 
///                  gridStats(3:5) : axis1 min,ave,max
///                  gridStats(6:8) : axis2 min,ave,max
///                  gridStats(9:11): axis3 min,ave,max
///     - number of negative volumes: gridStats(12)
/// \param file (input):   Print statistics about the grid to this file. If NULL do not print.
// =================================================================================================
{
  gridStats.redim(20);
  gridStats=-1.;

  real volMin=REAL_MAX,volAve=0.,volMax=0.;
  real numberOfNegativeVolumes=0;
  real numberOfGridPoints=0.;
  
  const int domainDimension = map.getDomainDimension();
  const int rangeDimension = map.getRangeDimension();

  // (we could use getGridSerial here -- BUT this is sometimes the entire grid)
  const realArray & xd = map.getGrid(); // grids points 
  OV_GET_SERIAL_ARRAY_CONST(real,xd,x); // x = grid points local to this processor

  if( false )
  {
    ::display(xd," gridStatistics: xd","%7.3f ");
    ::display(x," gridStatistics: x (local)","%7.3f ");
  }

  const IntegerArray gid(2,3); 

  for( int axis=0; axis<3; axis++ )
  {
    gid(0,axis)=xd.getBase(axis);
    gid(1,axis)=xd.getBound(axis);
  }

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  ::getIndex(gid,I1,I2,I3); 

  int includeGhost=0;
  bool ok=ParallelUtility::getLocalArrayBounds(xd,x, I1,I2,I3,includeGhost);

  for( int axis=0; axis<domainDimension; axis++ )
    Iv[axis]=Range(Iv[axis].getBase(),Iv[axis].getBound()-1);  // only compute at cell centres.

  const real orientation = map.getSignForJacobian();

  int i1,i2,i3;
  if( domainDimension==2 && rangeDimension==2 )
  {
    if( ok )
    {
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	// area of a polygon = (1/2) sum{ x_i y_{i+1} - x_{i+1} y_i
	real vol = ( x(i1  ,i2  ,i3,0)*x(i1+1,i2  ,i3,1)-x(i1+1,i2  ,i3,0)*x(i1  ,i2  ,i3,1) +  // (i1,i2)
		     x(i1+1,i2  ,i3,0)*x(i1+1,i2+1,i3,1)-x(i1+1,i2+1,i3,0)*x(i1+1,i2  ,i3,1) + 
		     x(i1+1,i2+1,i3,0)*x(i1  ,i2+1,i3,1)-x(i1  ,i2+1,i3,0)*x(i1+1,i2+1,i3,1) + 
		     x(i1  ,i2+1,i3,0)*x(i1  ,i2  ,i3,1)-x(i1  ,i2  ,i3,0)*x(i1  ,i2+1,i3,1) );
    
	vol*=.5*orientation;
	volMin=min(volMin,vol);
	volMax=max(volMax,vol);
	volAve+=vol;
	numberOfGridPoints++;
	if( vol<=0. ) numberOfNegativeVolumes++;
      
      }
    }
    
    volMin = ParallelUtility::getMinValue(volMin);
    volMax = ParallelUtility::getMaxValue(volMax);
    volAve = ParallelUtility::getSum(volAve);
    numberOfGridPoints = ParallelUtility::getSum(numberOfGridPoints);
    numberOfNegativeVolumes = ParallelUtility::getSum(numberOfNegativeVolumes);

    volAve/=max(1,numberOfGridPoints);
    
    
  }
  else if( domainDimension==2 && rangeDimension==3 )
  {
    printF("printGridStatistics: not implemented yet for surface grids.\n");
  }
  else if( domainDimension==3 )
  {
    // ************ 3D ***********************

    real v[2][2][2][3];
    if( ok )
    {
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	for( int axis=0; axis<3; axis++ )
	{
	  v[0][0][0][axis]=x(i1  ,i2  ,i3  ,axis);
	  v[1][0][0][axis]=x(i1+1,i2  ,i3  ,axis);
	  v[0][1][0][axis]=x(i1  ,i2+1,i3  ,axis);
	  v[1][1][0][axis]=x(i1+1,i2+1,i3  ,axis);
	  v[0][0][1][axis]=x(i1  ,i2  ,i3+1,axis);
	  v[1][0][1][axis]=x(i1+1,i2  ,i3+1,axis);
	  v[0][1][1][axis]=x(i1  ,i2+1,i3+1,axis);
	  v[1][1][1][axis]=x(i1+1,i2+1,i3+1,axis);
	}

	real vol=hexVolume(v[0][0][0],v[1][0][0],v[0][1][0],v[1][1][0],
			   v[0][0][1],v[1][0][1],v[0][1][1],v[1][1][1])*orientation;
      
	volMin=min(volMin,vol);
	volMax=max(volMax,vol);
	volAve+=vol;
	numberOfGridPoints++;
	if( vol<=0. ) numberOfNegativeVolumes++;
      
      }
    }
    
    volMin = ParallelUtility::getMinValue(volMin);
    volMax = ParallelUtility::getMaxValue(volMax);
    volAve = ParallelUtility::getSum(volAve);
    numberOfGridPoints = ParallelUtility::getSum(numberOfGridPoints);
    numberOfNegativeVolumes = ParallelUtility::getSum(numberOfNegativeVolumes);

    volAve/=max(1,numberOfGridPoints);
    
  }

  // Compute grid line spacing
  for( int axis=0; axis<domainDimension; axis++ )
  {
    real dsMin=REAL_MAX,dsAve=0.,dsMax=0.;

    int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
    isv[0]=isv[1]=isv[2]=0;
    isv[axis]=1;
      
    ::getIndex(gid,I1,I2,I3);  // do all points
    Iv[axis]=Range(Iv[axis].getBase(),Iv[axis].getBound()-1);

    includeGhost=0;
    ok=ParallelUtility::getLocalArrayBounds(xd,x, I1,I2,I3,includeGhost);

    real numDs=0.;
    if( ok )
    {
      if( rangeDimension==1 )
      {
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  real ds= sqrt( SQR(x(i1+is1,i2,i3,0)-x(i1,i2,i3,0)) );
	  dsMin=min(dsMin,ds);
	  dsMax=max(dsMax,ds);
	  dsAve+=ds;
	  numDs++;
	}
      }
      else if( rangeDimension==2 )
      {
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  real ds= sqrt( SQR(x(i1+is1,i2+is2,i3,0)-x(i1,i2,i3,0))+
			 SQR(x(i1+is1,i2+is2,i3,1)-x(i1,i2,i3,1)) );
	  dsMin=min(dsMin,ds);
	  dsMax=max(dsMax,ds);
	  dsAve+=ds;
	  numDs++;
	}
      }
      else
      {
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  real ds= sqrt( SQR(x(i1+is1,i2+is2,i3+is3,0)-x(i1,i2,i3,0))+
			 SQR(x(i1+is1,i2+is2,i3+is3,1)-x(i1,i2,i3,1))+   
			 SQR(x(i1+is1,i2+is2,i3+is3,2)-x(i1,i2,i3,2)) );
	  dsMin=min(dsMin,ds);
	  dsMax=max(dsMax,ds);
	  dsAve+=ds;
	  numDs++;
	}
      }
    }
    
    dsMin = ParallelUtility::getMinValue(dsMin);
    dsMax = ParallelUtility::getMaxValue(dsMax);
    dsAve = ParallelUtility::getSum(dsAve);
    numDs = ParallelUtility::getSum(numDs);

    dsAve/=max(1,numDs);

    gridStats(3+3*axis+0)=dsMin;
    gridStats(3+3*axis+1)=dsAve;
    gridStats(3+3*axis+2)=dsMax;
      
  }

  gridStats(0)=volMin;
  gridStats(1)=volAve;
  gridStats(2)=volMax;
  gridStats(12)=numberOfNegativeVolumes;
  
  if( file!=NULL )
  {
    fPrintF(file,
	    " -----------------------------------------------------------------------\n"
	    "         Grid Statistics name=%s. \n"
	    " grid lines  : [%i:%i,%i:%i,%i:%i], total points = %g\n"
	    " cell volumes: [%8.2e,%8.2e,%8.2e] [min,ave,max] \n"
	    " number of negative volumes = %g \n"
	    " -----------------------------------------------------------------------\n"
	    ,(const char*)map.getName(Mapping::mappingName),
	    gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
	    numberOfGridPoints,
	    volMin,volAve,volMax,numberOfNegativeVolumes
      );
    
    for( int axis=0; axis<domainDimension; axis++ )
    {
      fPrintF(file,
	      " grid spacing: [%8.2e,%8.2e,%8.2e] [min,ave,max] (axis=%i) \n",
	      gridStats(3+3*axis+0),gridStats(3+3*axis+1),gridStats(3+3*axis+2),axis);
    }
    fPrintF(file,
	    " -----------------------------------------------------------------------\n");
  }
  
}


// ---------------------------
#if 0 
#ifdef USE_PPP 
// here are versions that take distributed arrays
void Mapping::
mapD( const realArray & r, realArray & x, realArray &xr /* = Overture::nullRealDistributedArray() */,
     MappingParameters & params /* =Overture::nullMappingParameters() */)
{
  // evalaute the local array, include ghost boundaries so we don't have to communicate them.
  map( r.getLocalArray(),
       x.getLocalArray(),
       xr.getLocalArray(),
       params );
}

void Mapping::
inverseMapD( const realArray & x, realArray & r, realArray & rx /* =Overture::nullRealDistributedArray() */,
	    MappingParameters & params  /* =Overture::nullMappingParameters() */)
{
  inverseMap( x.getLocalArray(),
	      r.getLocalArray(),
	      rx.getLocalArray(),
	      params );
}

void Mapping::
basicInverseD(const realArray & x, 
	     realArray & r,
	     realArray & rx  /* =nullRealistributedArray */,
	     MappingParameters & params  /* =Overture::nullMappingParameters() */)
{
  basicInverse( x.getLocalArray(),
		r.getLocalArray(),
		rx.getLocalArray(),
		params );
}

void Mapping::
mapCD( const realArray & r, const realArray & x, const realArray &xr  /* = Overture::nullRealDistributedArray() */,
      MappingParameters & params  /* =Overture::nullMappingParameters() */)
{
  mapD(r,(realArray&)x,(realArray&)xr,params); // cast away const
}

void Mapping::
inverseMapCD( const realArray & x, const realArray & r, const realArray & rx  /* =Overture::nullRealDistributedArray() */,
	     MappingParameters & params  /* =Overture::nullMappingParameters() */)
{
  inverseMapD(x,(realArray&)r,(realArray&)rx,params); // cast away const
}

void Mapping::
mapGridD(const realArray & r, 
	realArray & x, 
	realArray & xr  /* =nullRealistributedArray */ ,
	MappingParameters & params /* =Overture::nullMappingParameters() */ )
{
  mapGrid( r,
	   x,
	   xr,
	   params );
}

void Mapping::
inverseMapGridD(const realArray & x, 
	realArray & r, 
	realArray & rx  /* =nullRealistributedArray */ ,
	MappingParameters & params /* =Overture::nullMappingParameters() */ )
{
  inverseMapGrid( x,
		  r,
		  rx,
		  params );
}

int  Mapping::
projectD( realArray & x, MappingProjectionParameters & mpParams )
{
  return project(x,mpParams);
}


#else
//   Define these in the serial case

void Mapping::
mapD( const realArray & r, realArray & x, realArray &xr /* = Overture::nullRealDistributedArray() */,
     MappingParameters & params /* =Overture::nullMappingParameters() */)
{
  // evalaute the local array, include ghost boundaries so we don't have to communicate them.
  map(r,x,xr,params);
}

void Mapping::
inverseMapD( const realArray & x, realArray & r, realArray & rx /* =Overture::nullRealDistributedArray() */,
	    MappingParameters & params  /* =Overture::nullMappingParameters() */)
{
  inverseMap(x,r,rx,params);
}

void Mapping::
basicInverseD(const realArray & x, 
	     realArray & r,
	     realArray & rx  /* =nullRealistributedArray */,
	     MappingParameters & params  /* =Overture::nullMappingParameters() */)
{
  basicInverse(x,r,rx,params);
}

void Mapping::
mapCD( const realArray & r, const realArray & x, const realArray &xr  /* = Overture::nullRealDistributedArray() */,
      MappingParameters & params  /* =Overture::nullMappingParameters() */)
{
  map(r,(realArray&)x,(realArray&)xr,params); // cast away const
}

void Mapping::
inverseMapCD( const realArray & x, const realArray & r, const realArray & rx  /* =Overture::nullRealDistributedArray() */,
	     MappingParameters & params  /* =Overture::nullMappingParameters() */)
{
  inverseMap(x,(realArray&)r,(realArray&)rx,params); // cast away const
}

void Mapping::
mapGridD(const realArray & r, 
	realArray & x, 
	realArray & xr  /* =nullRealistributedArray */ ,
	MappingParameters & params /* =Overture::nullMappingParameters() */ )
{
  mapGrid(r,x,xr,params);
}

void Mapping::
inverseMapGridD(const realArray & x, 
	realArray & r, 
	realArray & rx  /* =nullRealistributedArray */ ,
	MappingParameters & params /* =Overture::nullMappingParameters() */ )
{
  inverseMapGrid( x,r,rx,params );
}

int  Mapping::
projectD( realArray & x, MappingProjectionParameters & mpParams )
{
  return project((RealArray&)x,mpParams);
}

#endif

#endif
