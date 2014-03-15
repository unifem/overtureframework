#include "Mapping.h"
#include "mathutil.h"
#include <float.h>
#include "Inverse.h"
#include "ParallelUtility.h"

int useNewNonSquareInverse=true;

RealArray floor2( const RealArray & r );   // ***** until the A+= floor get fixed


void initStaticMappingVariables();    

const real Mapping::bogus=10.;   // Bogus value to indicate no convergence
const real ApproximateGlobalInverse::bogus=10.;   // Bogus value to indicate no convergence

real ApproximateGlobalInverse::timeForApproximateInverse=0.;

// extern IntegerArray status;
// extern RealArray yStatic[maximumNumberOfRecursionLevels],
//           yrStatic[maximumNumberOfRecursionLevels],
//           dr,dy,
//           ry,det, 
//           r2Static[maximumNumberOfRecursionLevels],
//           yr2Static[maximumNumberOfRecursionLevels],
//           yrr,rBogus;

static int recursionLevel=-1;  // keeps track of how deeply nested we are
static real drMax[3];          // holds maximum dr for Newton

static int numberOfNoConvergenceMessages=0;
static int maximumNumberOfNoConvergenceMessages=10;

static int numberOfSlowConvergenceMessages=0;
static int maximumNumberOfSlowConvergenceMessages=10;

real MappingWorkSpace::
sizeOf(FILE *file /* = NULL */ ) const
{
  real size=sizeof(*this);
  size+=(x0.elementCount()+r0.elementCount()+rx0.elementCount())*sizeof(real);
  size+=index0.elementCount()*sizeof(int);
  
  return size;
}

//===========================================================================
//  Routines to Define define an Approximate Global Inverse
//  and an Exact Local Inverse.
//
//===========================================================================

ApproximateGlobalInverse::
ApproximateGlobalInverse( Mapping & map0 )
// ====================================================================================
/// \details 
///    Build an approximate inverse to go with a given mapping.
// ===================================================================================
#ifndef USE_PPP
  : grid(map0.grid)
#else
  : grid(map0.gridSerial)
#endif
{
  map=&map0;

  uninitialized=true;     // initialize on first call to inverse
  gridDefined=false;      // no grid defined for inverse
  boundingBoxExtensionFactor=0.01;
  stencilWalkBoundingBoxExtensionFactor=map->getDomainDimension()==1 ? .5 : .2;
  useRobustApproximateInverse=false;
  findBestGuess=false;

  serialBoundingBox=NULL;
  
}

void ApproximateGlobalInverse::
reinitialize()
// ====================================================================================
// /Description:
//   This will mark ApproximateGlobalInverse as being in need of initialization.
//   The actual call to initialize will occur when the inverse is actually used.
// ===================================================================================
{
  if( Mapping::debug & 8 )
    cout << "ApproximateGlobalInverse::reinitialize \n";
  uninitialized=true;
  gridDefined=false;
  // **wdh**** 990325  initialize();
}

//====================================================================================
//    initialize
//
//  Initialization routine for the bounding box and search routines
//  This needs to be completed....
//===================================================================================
void ApproximateGlobalInverse::
initialize()
{
  if( !uninitialized )
    return;
  
  domainDimension=map->getDomainDimension();  
  rangeDimension=map->getRangeDimension();  

  Axes = Index(axis1,domainDimension);
  xAxes  = Index(axis1,rangeDimension);

  if( Mapping::debug & 8 )
    cout << "ApproximateGlobalInverse: initialize..." << endl;
  
  if( !gridDefined )
    constructGrid();  // Make a grid to use if none is given with setGrid


  // Determine the binary tree of BoundingBox's
  if(Mapping::debug & 8 )
  {
    printF("AGI::initialize: initializeBoundingBoxes for %s\n",(const char*)map->getName(Mapping::mappingName));
    // Range all;
    // printF("AGI: grid x-bounds = [%8.2e,%8.2e][%8.2e,%8.2e]\n",min(grid(all,all,all,0)),max(grid(all,all,all,0)),
    //	   min(grid(all,all,all,1)),max(grid(all,all,all,1)));
  }
  initializeBoundingBoxTrees();
  initializeStencilWalk();


  #ifdef USE_PPP

  if( map->usesDistributedInverse() )
  {
    // If the inverseMap uses a distributed grid, we need to compute the
    // bounding boxes (serialBoundingBox[p]) for the portion of the grid on processor p.
    // This is used in inverseMapS (inverseMap.C)

    const MPI_Comm & OV_COMM = Overture::OV_COMM;
  
    const int np= max(1,Communication_Manager::numberOfProcessors());
    const int myid=max(0,Communication_Manager::My_Process_Number);
  
    serialBoundingBox = new BoundingBox [np];
  
    // --- send my bounding box to all other processors ---

    real *pbb = new real [6*np];
#define bb(side,axis,p) pbb[(side)+2*((axis)+3*(p))]
    for( int axis=0; axis<3; axis++ )for( int side=0; side<=1; side++ )
    {
      bb(side,axis,myid)=boundingBox(side,axis);
    }

    MPI_Status status;
    const int tag0=110328;
    for(int p=0; p<np; p++ )
    {
      int tags=tag0+p, tagr=tag0+myid;
      if( p!=myid )
      {
	MPI_Sendrecv(&bb(0,0,myid), 6, MPI_Real, p, tags, 
		     &bb(0,0,p)   , 6, MPI_Real, p, tagr, OV_COMM, &status ); 
      }
    }

    for(int p=0; p<np; p++ )
    {
      for( int axis=0; axis<3; axis++ )for( int side=0; side<=1; side++ )
      {
	serialBoundingBox[p].rangeBound(side,axis)=bb(side,axis,p);
      }
    }
    delete [] pbb;
  }
  
  #endif



  uninitialized=false;
  
  // Assign xOrigin and xTangent vectors used for periodicityOfSpace computations
  // xOrigin : a point on each face of the bounding box
  // xTangents : tangent vector(s) for the face (length=length of side)

  xOrigin.redim(rangeDimension,2,rangeDimension);
  if( rangeDimension>1 )
  {
    xTangent.redim(rangeDimension,rangeDimension-1,2,rangeDimension);
    xTangent=0.;
  }
  
  for( int dir=axis1; dir<rangeDimension; dir++ )
  for( int side=Start; side<=End; side++ )
  {
    for( int axis=axis1; axis<rangeDimension; axis++)
      xOrigin(axis,side,dir)=boundingBox(Start,axis);
    xOrigin(dir,side,dir)=boundingBox(side,dir);
    for( int i=axis1; i<rangeDimension-1; i++)
    {
      int dirPlus=(dir+i+1) % rangeDimension;  
      xTangent(dirPlus,i,side,dir)=boundingBox(End,dirPlus)-boundingBox(Start,dirPlus);
    }
  }
  if( Mapping::debug & 64 )
  {
    xOrigin.display("Here is xOrigin");  
    xTangent.display("Here is xTangent");  
  }

  uninitialized=false;
}

ApproximateGlobalInverse::
~ApproximateGlobalInverse()
{
  // remember: bounding box trees are properly deleted (recursively)
  // when the destructors for boundingBoxTree[2][3] are called.
  // Each box deletes it's children which in turn delete their children

  delete [] serialBoundingBox;
  
}

// void ApproximateGlobalInverse::
// setGrid( const RealArray & grid0, const IntegerArray & gridIndexRange )
// //=======================================================================
// //  /Description:
// //  Give a grid that can be used for global search routines
// //  The grid is assumed to have been assigned with values of the
// //  mapping. The grid is assumed to be always declared as a 
// //  four-dimensional A++ array, grid(axis1,axis2,axis3,rangeDimension).
// //
// //  /grid0 (input) : use this grid.
// //  /gridIndexRange : index bounds for the sides of the grids
// //
// //=======================================================================
// {
//   grid.redim(0);  
//   grid=grid0;       // **** why not use a reference? ***********************
//   indexRange.redim(0);
//   indexRange=gridIndexRange;
    
    
//   dimension.redim(2,3);
//   for( int axis=axis1; axis<3; axis++ )
//   {
//     dimension(Start,axis)=grid.getBase(axis);
//     dimension(End  ,axis)=grid.getBound(axis);
//   }

//   gridDefined=true;
//   uninitialized=true;   // must initialize again if the grid has changed
    
// }  

const RealArray & ApproximateGlobalInverse::
getGrid() const
//=======================================================================
/// \details 
///  return the grid used for the inverse
//=======================================================================
{
  return grid;
}

const RealArray & ApproximateGlobalInverse::
getBoundingBox() const
// =====================================================================================
/// \details 
///    Return the bounding box for the entire mapping.
// =====================================================================================
{
  return boundingBox;
}

const BoundingBox & ApproximateGlobalInverse::
getBoundingBoxTree(int side, int axis) const
// =====================================================================================
/// \details 
///    Return the bounding box tree for a given boundary of the mapping.
// =====================================================================================
{
  return boundingBoxTree[side][axis];
}


real ApproximateGlobalInverse::
getParameter( const MappingParameters::realParameter & param ) const
// =====================================================================================
/// \details 
///    Return the value of a parameter.
/// \param param (input) : One of {\tt MappingParameters::THEboundingBoxExtensionFactor}
///      or {\tt MappingParameters::THEstencilWalkBoundingBoxExtensionFactor}.
///  
// =====================================================================================
{ 
  real returnValue;
  switch (param)
  {
  case MappingParameters::THEboundingBoxExtensionFactor: 
    returnValue=boundingBoxExtensionFactor;
    break;
  case MappingParameters::THEstencilWalkBoundingBoxExtensionFactor:
    returnValue=stencilWalkBoundingBoxExtensionFactor;
    break;
  default:
    cout << " ApproximateGlobalInverse::getParameter: fatal error, unknown value for realParameter\n";
    {throw "error";}
  }
  return returnValue;
}

int ApproximateGlobalInverse::
getParameter( const MappingParameters::intParameter & param ) const
// =====================================================================================
/// \details 
///    Return the value of a parameter.
/// \param param (input) : One of {\tt MappingParameters::THEfindBestGuess}
///  
// =====================================================================================
{ 
  int returnValue;
  switch (param)
  {
  case MappingParameters::THEfindBestGuess:
    returnValue=findBestGuess;
  default:
    cout << " ApproximateGlobalInverse::getParameter: fatal error, unknown value for realParameter\n";
    {throw "error";}
  }
  return returnValue;
}

void ApproximateGlobalInverse::
setParameter( const MappingParameters::realParameter & param, const real & value ) 
// =====================================================================================
/// \details 
///    Set the value of a parameter.
/// \param param (input) : One of {\tt MappingParameters::THEboundingBoxExtensionFactor}
///      or {\tt MappingParameters::THEstencilWalkBoundingBoxExtensionFactor}.
/// \param value (input) : value for the parameter.
///  
// =====================================================================================
{ 

  switch (param)
  {
  case MappingParameters::THEboundingBoxExtensionFactor:
    boundingBoxExtensionFactor=value;
    break;
  case MappingParameters::THEstencilWalkBoundingBoxExtensionFactor:
    stencilWalkBoundingBoxExtensionFactor=value;
    break;
  default:
    cout << " ApproximateGlobalInverse:: fatal error, unknown value for realParameter\n";
    {throw "error";}
  }
}

void ApproximateGlobalInverse::
setParameter( const MappingParameters::intParameter & param, const int & value ) 
// =====================================================================================
/// \details 
///    Set the value of a parameter.
/// \param param (input) : One of {\tt MappingParameters::THEboundingBoxExtensionFactor}
///      or {\tt MappingParameters::THEstencilWalkBoundingBoxExtensionFactor}.
/// \param value (input) : value for the parameter.
///  
// =====================================================================================
{ 

  switch (param)
  {
  case MappingParameters::THEfindBestGuess:
    findBestGuess=value;
    break;
  default:
    cout << " ApproximateGlobalInverse:: fatal error, unknown value for realParameter\n";
    {throw "error";}
  }
}

void ApproximateGlobalInverse::
useRobustInverse(const bool trueOrFalse /* =true */ )
// =====================================================================================
/// \details 
///     If true use the more robust approximate inverse that will work with highly
///  stretched grids where the closest grid point x, to a given point may be many cells
///  away from the cell containing the point x.
// =====================================================================================
{
  useRobustApproximateInverse=trueOrFalse;
}

bool ApproximateGlobalInverse::
usingRobustInverse() const 
// =====================================================================================
/// \details 
///     Return true if using the more robust approximate inverse that will work with highly
///  stretched grids where the closest grid point x, to a given point may be many cells
///  away from the cell containing the point x.
// =====================================================================================
{
  return useRobustApproximateInverse;
}

real ApproximateGlobalInverse::
sizeOf(FILE *file /* = NULL */ ) const
// =======================================================================================
/// \details 
///    Return size of this object  
// =======================================================================================
{
  real size=sizeof(*this);

  // size+=grid.elementCount()*sizeof(real);   // grid is now just a C reference to the one in map

  return size;
}


int ApproximateGlobalInverse::
get( const GenericDataBase & dir, const aString & name)
// =====================================================================================
/// \details 
///     Get this object from a sub-directory called "name"
// =====================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"ApproximateGlobalInverse");

  subDir.get( boundingBoxExtensionFactor,"boundingBoxExtensionFactor" );
  delete & subDir;
  return true;
}

int ApproximateGlobalInverse:: 
put( GenericDataBase & dir, const aString & name) const
// =====================================================================================
/// \details 
///  save this object to a sub-directory called "name"
// =====================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"ApproximateGlobalInverse");                      // create a sub-directory 

  subDir.put( boundingBoxExtensionFactor,"boundingBoxExtensionFactor" );
  delete &subDir;
  return true;
}


void ApproximateGlobalInverse::
inverse(const RealArray & x, 
	RealArray & r, 
	RealArray & rx,
	MappingWorkSpace & workSpace, 
	MappingParameters & params )
//===================================================================================
/// \brief 
///    Find an approximate inverse of the mapping; this approximate inverse
///    should be good enough so that Newton will converge
/// 
/// \param Method: 
///  <ol>
///    <li> If space is periodic (e.g. if the grids all live on a background square which has
///       one or more periodic edges) then we need to worry about values of x that are outside 
///       the basic periodic region. These points may have periodic images that lie inside the
///       periodic region. We thus add new points to the list that are the periodic images that
///       lie inside the basic square. ***NOTE*** space periodic rarley occurs and probably hasn't
///       been tested enough.
///  \begin{verbatim}
///        --------------------
///        |                  |
///        | x                |   X
///        | periodic         |   initial point to invert
///        | image            |
///        |                  |
///        |                  |
///        |                  |
///        --------------------
///  \end{verbatim}
///   <li> For all points to invert, find the closest point on the reference grid that goes with
///     the mapping. This grid is usually just the grid that is used when plotting the mapping.
///      This step is performed by the function {\tt findNearestGridPoint}
///  </ul>
/// 
/// \param Notes:
///    The results produced by this routine are saved in the object workSpace.
/// \param workSpace.x0 (output) : list of points to invert with possible extra points if space is periodic.
/// \param workSpace.r0 (output) : unit square coordinates of the closest point.
/// \param workSpace.I0 (output) : Index object that demarks the active points in x0 and r0.
/// \param workSpace.index0 (output) : indirect addressing array that points back to the original r array; used
///      when there are extra points added for periodicity in space.
/// \param workSpace.index0IsSequential (output) : if true then space is periodic and the index0 indirect addressing
///      array should be used when storing results back in the user arrays r and rx.
/// 
//===================================================================================
{ 
  real time0=getCPU();

  if( uninitialized || domainDimension != map->getDomainDimension()
                    || rangeDimension  != map->getRangeDimension() )
    initialize();  // Initialize first time (do here to allow for calls to setGrid)

  RealArray & x0 = workSpace.x0;
  RealArray & r0 = workSpace.r0;
  Index & I0 = workSpace.I0;
  IntegerArray & index0 = workSpace.index0;
  
  int computeMap, computeMapDerivative;
  Index I = map->getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  int i;

  base0=base;          // set global variables
  bound0=bound;

  
  x0.redim(x); x0=x;  // Initialize x0 and r0
  r0.redim(I,domainDimension); // *wdh* 980104 
  index0.redim(I); 

  // ---If space is periodic determine the correct x to use by choosing an x that sits
  //    in the bounding Box (there may be more that one, so add all possibilities)

  // *** add changes here for "blocked" grids ***

  if( params.periodicityOfSpace > 0 && domainDimension==rangeDimension )
  {
    if( Mapping::debug & 4 ) 
      cout << "ApproximateGlobalInverse:inverse: Space is periodic..." << endl;
  
    // Make a list of the points that could be inside the grid
    int j,nI; 
    RealArray xI;
    for( i=base, j=base-1; i<=bound; i++ )
    { 
      // Make a list of the possible periodic images that lie in the range
      getPeriodicImages( evaluate(x(i,xAxes)),xI,nI,params.periodicityOfSpace,
                         params.periodicityVector );
      if( j+nI > bound0 )
      {
        // Increase the size of the arrays
        bound0=bound0+10+bound0/10;   // Increase size by this amount?
        x0.resize(I,rangeDimension);  
        x0.resize(I,rangeDimension);  
        r0.resize(I,domainDimension); 
        index0.resize(I);             
      }
      if( nI==0 )
        r(i,Axes)=bogus;  // bogus value if cannot invert
      
      for( int image=0; image<nI; image++)  
      {
        j++;
        x0(j,xAxes)=xI(image,xAxes);
        r0(j,Axes)=r(i,Axes);
        index0(j)=i;
      }
    }
    
    if( bound0 != j )  // number of points have changed
    {
      // Resize the r0,x0 and index0 arrays, r0 must be the correct size for calls to map
      bound0=j;  
      r0.resize(I,domainDimension);
      x0.resize(I,rangeDimension); 
      index0.resize(I);            
    }

    workSpace.index0IsSequential=false;
  }
  else  // not periodic in space:
  {
    // index0.seqAdd(base,1);   // set index0(i)=i     *** is this needed ?? ***
    // for( i=base; i<=bound; i++ )
    //  index0(i)=i;
    // index0.display("index0");

    workSpace.index0IsSequential=true;
  }

  I0 = Index(base0,bound0-base0+1);  // *** is this used?? ***

  if( Mapping::debug & 16 ) 
    cout << " ... base0,bound0 = " << base0 << ", " << bound0 << endl;

  if( map->getBasicInverseOption()!=Mapping::canInvert )
  {
    if( Mapping::useInitialGuessForInverse && computeMap )
      r0=r(I,Axes);
    else
      r0=-1.;  // causes next routine to use it's own guess  ***** is this what we want ? ******

    findNearestGridPoint( base0, bound0, x0, r0 );
  }

  if( Mapping::debug & 8 )
    for( i=base0; i<=bound0; i++ )
    {
      if( i==base0 ) cout << "ApproximateGlobalInverse:inverse: x0=periodic x, r0=nearest point" << endl;
      if( workSpace.index0IsSequential )
      {
        printf(" i=%i x=(%9.2e,%9.2e,%9.2e) x0=(%9.2e,%9.2e,%9.2e) r0=(%9.2e,%9.2e,%9.2e)\n",i,
               x(i,0),(rangeDimension>1 ? x(i,1) : 0.),(rangeDimension>2 ? x(i,2) : 0.), 
               x0(i,0),(rangeDimension>1 ? x0(i,1) : 0.),(rangeDimension>2 ? x0(i,2) : 0.), 
               r0(i,0),(rangeDimension>1 ? r0(i,1) : 0.),(rangeDimension>2 ? r0(i,2) : 0.) );
      }
      else
      {
	cout << "   i = " << i << ", index0 = " << index0(i) 
	     << ", x  = (" <<  x(index0(i),axis1) << "," 
	     << (rangeDimension>1 ? x(index0(i),axis2) : 0.) << ","
	     << (rangeDimension==3 ? x(index0(i),axis3) : 0.) << ")" 
	     << ", x0 = (" << x0(i,axis1) << "," 
	     << (rangeDimension>1 ? x0(i,axis2) : 0. ) << ","
	     << (rangeDimension==3 ? x0(i,axis3) : 0.) << ")" 
	     << ", r0 = (" << r0(i,axis1) << "," 
	     << (domainDimension>1 ? r0(i,axis2) : 0.)  << ","
	     << (domainDimension==3 ? r0(i,axis3) : 0.) <<  ")" << endl;
      }
      
    }

  timeForApproximateInverse+=getCPU()-time0;

}

//====================================================================================
//    getPeriodicImages
//
//  Get a list of the possible periodic images of x that lie in the range of
//  the mapping. If the periodicityOfSpace > 0 then we must check to see
//  if there are integers i1,i2,i3 such that the point
//          x + i1*periodicityVector(.,axis1)+i2*periodicityVector(.,axis2)+..
//  lies in the bounding box.
//
// Input -
//  x : target point
// 
// Output
//  nI : number of periodic Images
//  xI : periodic images
//===================================================================================
void ApproximateGlobalInverse::
getPeriodicImages( const RealArray & x, RealArray & xI, int & nI, const int & periodicityOfSpace, 
                   const RealArray & periodicityVector )
{
  if( Mapping::debug & 8 ) 
    cout << "getPeriodicImages:  x = (" << x(axis1) << "," << x(axis2) << ")" 
      << " periodicityOfSpace = " << periodicityOfSpace << endl;

  int max=int(10+2*pow(3,periodicityOfSpace));  // max number of images, fix this ***
  xI.redim(max,rangeDimension);

  nI=0;
  switch (periodicityOfSpace)
  {
  case 1:
    intersectLine( x, nI, xI, periodicityVector, xOrigin, xTangent );
    break;
  case 2:
    intersectPlane( x, nI, xI, periodicityVector, xOrigin, xTangent );
    break;
  case 3:
    intersectCube( x, nI, xI, periodicityVector, xOrigin, xTangent );
    break;
  default:
    cerr << "getPeriodicImages::Error periodicityOfSpace = " << periodicityOfSpace << endl;
    exit(1);
  }
  if( Mapping::debug & 8 )
  {
    cout << "getPeriodicImages: nI =" << nI << endl;
    xI.display("getPeriodicImages: xI");
  }
}

//===================================================================================
//              intersectLine
//
//  Find all points
//             xI=x+s*vector,   s an integer,
//   that lie inside the box.
//
// Input -
//  x : target point (find periodic images of this point)
//  vector : look for integers, s, with  x+s*vector inside the box
//  nI : save points in xI(.,nI), xI(.,nI+1), ...
// Output -
//  xI : image points that we found
//  nI : points to the last point in the list
//====================================================================================
void ApproximateGlobalInverse::
intersectLine( const RealArray & x, int & nI, RealArray & xI, 
               const RealArray & vector, const RealArray & xOrigin0, const RealArray & xTangent0 )
{
  RealArray xa(3),xba(3),xca(3),sp(3),xma(3);
  real det,s1,s2,s;  

  // Get intersections of the line y(s) = x + s*periodicityVector with
  // the faces (edges) of the boundingBox
  int intersection=0;
  for( int dir=axis1; dir<domainDimension; dir++ )
  {
    for( int side=Start; side<=End; side++ )
    { 
      // intersect the line with the plane (line) defined by
      //    xa+s1*xba+s2*xca
      xa(xAxes)=xOrigin0(xAxes,side,dir);
      xba(xAxes)=xTangent0(xAxes,axis1,side,dir);

      if( Mapping::debug & 64 )
      {
        xa.display("Here is xa");
        xba.display("Here is xba");
      }
    
      switch (rangeDimension)
      {
      case 2:
        // Solve :    [ -vector(0) xba(0) ][ s ] = [ x(0)-xa(0) ]
        //            [ -vector(1) xba(1) ][ s1] = [ x(1)-xa(1) ]
        det=-vector(axis1)*xba(axis2)+vector(axis2)*xba(axis1);
        if( Mapping::debug & 64 ) cout << " det =" << det << endl;
        if( det!=0. )
        {  // for now assume parallel lines don't intersect
        
          s1=(-vector(axis1)*(x(0,axis2)-xa(axis2))+vector(axis2)*(x(0,axis1)-xa(axis1)))/det;
          if( Mapping::debug & 64 ) 
            cout << " dir = " << dir << ", side=" << side << ", s1=" << s1 << endl;
       
          if( s1>=0. && s1<=1. )
          {
            s=((x(0,axis1)-xa(axis1))*xba(axis2)-(x(0,axis2)-xa(axis2))*xba(axis1))/det;
            sp(intersection++)=s;
          }
	}
	break;
      case 3:
        // Solve :    [ -vector(0) xba(0) xca(0) ][ s ] = [ x(0)-xa(0) ]
        //            [ -vector(1) xba(1) xca(1) ][ s1] = [ x(1)-xa(1) ]
        //            [ -vector(2) xba(2) xca(2) ][ s2] = [ x(2)-xa(2) ]
        xca(xAxes)=xTangent0(xAxes,axis2,side,dir);
        det=-vector(axis1)*( xba(axis2)*xca(axis3)-xba(axis3)*xca(axis2) )
            +vector(axis2)*( xba(axis1)*xca(axis3)-xba(axis3)*xca(axis1) )
            -vector(axis3)*( xba(axis1)*xca(axis2)-xba(axis2)*xca(axis1) );
        // if( Mapping::debug & 64 ) cout << " det =" << det << endl;
        if( det!=0. )
        {  // for now assume parallel lines don't intersect
        
          for( int axis=0; axis<rangeDimension; axis++ )
            xma(axis)=x(0,axis)-xa(axis);
          s1=(-vector(axis1)*( xma(axis2)*xca(axis3)-xma(axis3)*xca(axis2) )
              +vector(axis2)*( xma(axis1)*xca(axis3)-xma(axis3)*xca(axis1) )
              -vector(axis3)*( xma(axis1)*xca(axis2)-xma(axis2)*xca(axis1) ))/det;
          // cout << " dir = " << dir << ", side=" << side << ", s1=" << s1 << endl;
       
          if( s1>=0. && s1<=1. )
          {
          s2=(-vector(axis1)*( xba(axis2)*xma(axis3)-xba(axis3)*xma(axis2) )
              +vector(axis2)*( xba(axis1)*xma(axis3)-xba(axis3)*xma(axis1) )
              -vector(axis3)*( xba(axis1)*xma(axis2)-xba(axis2)*xma(axis1) ))/det;
            // cout << " dir = " << dir << ", side=" << side << ", s2=" << s2 << endl;
            if( s2>=0. && s2<=1. )
            {
              s=(xma(axis1)*( xba(axis2)*xca(axis3)-xba(axis3)*xca(axis2) )
                -xma(axis2)*( xba(axis1)*xca(axis3)-xba(axis3)*xca(axis1) )
                +xma(axis3)*( xba(axis1)*xca(axis2)-xba(axis2)*xca(axis1) ))/det;
              sp(intersection++)=s;
	    }
          }
	}
	
        break;
      default:
        cerr << " intersectLine::ERROR rangeDimension =" << rangeDimension << endl;
      } // end switch
    }
  }

  int xIBound = xI.getBound(axis2);  // maximum entry in xI
  
  if( intersection > 0 )
  { // find all integral mutiples of p that lie between the points
    // of intersection ** Here we assume the bounding box is slightly larger ***
    Index I(axis1,intersection);  // there may be 2,4 or 6 intersections
    if( Mapping::debug & 64 ) 
      cout << "intersection=" << intersection 
           << " sp(0) = " << sp(0) << ", sp(1) =" << sp(1) << endl;
    for( int i = int(ceil(min(sp(I)))); i<= int(floor(max(sp(I)))); i++ )
    {
      if( nI+1 > xIBound )
      {
	cerr << "intersectLine:Error not enough space in xI ! " << endl;
	exit(1);
      }
      // cout << " $$$$ before $$$" << endl;
      // xI(xAxes,nI++)=x(xAxes)+i*vector(xAxes);    This didn't work ****
      // cout << " $$$$ after $$$" << endl;
      for( int axis=0; axis<rangeDimension; axis++ )
        xI(nI++,axis)=x(0,axis)+i*vector(axis,axis1);
    }
  }
}

//===================================================================================
//              intersectPlane
//
//  Use this routine when the periodicityOfSpace==2 and rangeDimension==2
//  in order to compute the perioidc image points of the point x that lie
//  in the bounding box
//  
//
// Method:
//  Image points are of the form
//        x+ alpha1*vector1 + alpha2*vector2
//
//  (1) Determine the range of possible alpha2 by computing the alpha2 corresponding
//      to all corners of the box, giving  alpha2Min <= alpha2 <= alpha2Max
//  (2) For each integer i2 with alpha2Min <= i2 <= alpha2MAx, intersect the line
//           y(alpha1) = x + alpha1*vector1 + i2*vector2 
//      with the bounding box.
//
//==================================================================================
void ApproximateGlobalInverse::
intersectPlane( const RealArray & x, int & nI, RealArray & xI, 
                const RealArray & vector, const RealArray & xOrigin0, const RealArray & xTangent0 )
{
  // Get period vector coordinates of all corners (xOrigin0)
  // Solve  x+alpha1*vector1+alpha2*vector2 = xCorner
  //  -> a*alpha = xCorner-x
  //  ...we really on use alpha2

  RealArray a(2,2);

  a(axis1,axis1)=vector(axis1,axis1)*vector(axis1,axis1)   // vector1^T vector1
                +vector(axis2,axis1)*vector(axis2,axis1);
  a(axis1,axis2)=vector(axis1,axis1)*vector(axis1,axis2)   // vector1^T vector2
                +vector(axis2,axis1)*vector(axis2,axis2);
  a(axis2,axis1)=a(axis1,axis2);                           // vector2^T vector1
  a(axis2,axis2)=vector(axis1,axis2)*vector(axis1,axis2)   // vector1^T vector1
                +vector(axis2,axis2)*vector(axis2,axis2);
  real det=a(axis1,axis1)*a(axis2,axis2)-a(axis1,axis2)*a(axis2,axis1);
  if( det==0. )
  {
    cerr << "intersectPlane::Error det(vector1,vector2)=0 !" << endl;
    exit(1);
  }
  
  real alpha2Min=1.e10;      // ****
  real alpha2Max=-alpha2Min;
  RealArray x0(3);
  
  for( int dir=axis1; dir<domainDimension; dir++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      for( int axis=0; axis<rangeDimension; axis++ )
        x0(axis)=xOrigin0(axis,side,dir)-x(0,axis);
      //      alpha(axis1,side,dir)=
      //	(x0(axis1)*a(axis2,axis2)-x0(axis2)*a(axis1,axis2))/det;
      real alpha2=(a(axis1,axis1)*x0(axis2)-a(axis2,axis1)*x0(axis1))/det;
      alpha2Min=min(alpha2Min,alpha2);
      alpha2Max=max(alpha2Max,alpha2);
    }
  }

  // for lines parallel to vector(.,axis2) find points where they
  // intersect the bounding box
  RealArray x2(1,3);
  
  for( int i2=int(ceil(alpha2Min)); i2<=int(floor(alpha2Max)); i2++ )
  {
    for( int axis=0; axis<rangeDimension; axis++ )
      x2(0,axis)=x(0,axis)+i2*vector(axis,axis2);
    intersectLine( x2, nI, xI, vector, xOrigin0, xTangent0 );
  }
}

//===================================================================================
//              intersectCube
//
//  Use this routine when the periodicityOfSpace==3 and rangeDimension==3
//  in order to compute the perioidic image points of the point x that lie
//  in the bounding box
//  
//
// Method:
//  Image points are of the form
//        x+ alpha1*vector1 + alpha2*vector2 + alpha3*vector3
//
//  (1) Determine the range of possible alpha2[3] by computing the alpha2[3] corresponding
//      to all corners of the box, giving  alpha2[3]Min <= alpha2[3] <= alpha2[3]Max
//  (2) For each integer i2[3] with alpha2[3]Min <= i2[3] <= alpha2[3]MAx, intersect the line
//           y(alpha1) = x + alpha1*vector1 + i2*vector2 + i3*vector3
//      with the bounding box.
//
//==================================================================================
void ApproximateGlobalInverse::
intersectCube( const RealArray & x, int & nI, RealArray & xI, 
               const RealArray & vector, const RealArray & xOrigin0, const RealArray & xTangent0 )
{
  // Get period vector coordinates of all corners (xOrigin0)
  // Solve  x+alpha1*vector1+alpha2*vector2 = xCorner
  //  -> a*alpha = xCorner-x
  //  ...we really on use alpha2

  RealArray a(3,3);

  for( int i2=axis1; i2<=axis3; i2++ )
  {
    for( int i1=axis1; i1<=axis3; i1++ )
    {
      a(i1,i2)=vector(axis1,i1)*vector(axis1,i2)   // vector(i1)^T vector(i2)
              +vector(axis2,i1)*vector(axis2,i2)
              +vector(axis3,i1)*vector(axis3,i2);
    }
  }
  
  real det=a(axis1,axis1)*(a(axis2,axis2)*a(axis3,axis3)-a(axis3,axis2)*a(axis2,axis3))
          +a(axis2,axis1)*(a(axis3,axis2)*a(axis1,axis3)-a(axis1,axis2)*a(axis3,axis3))
	  +a(axis3,axis1)*(a(axis1,axis2)*a(axis2,axis3)-a(axis2,axis2)*a(axis1,axis3));
  
  if( det==0. )
  {
    cerr << "intersectCube::Error det(vector1,vector2)=0 !" << endl;
    exit(1);
  }
  
  real alpha2,alpha3;
  
  real alpha2Min=REAL_MAX;
  real alpha2Max=-alpha2Min;
  real alpha3Min=REAL_MAX;
  real alpha3Max=-alpha3Min;
  RealArray x0(3);
  
  for( int dir=axis1; dir<domainDimension; dir++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      for( int axis=0; axis<rangeDimension; axis++ )
        x0(axis)=xOrigin0(axis,side,dir)-x(0,axis);
      //      alpha(axis1,side,dir)=
      //	(x0(axis1)*a(axis2,axis2)-x0(axis2)*a(axis1,axis2))/det;
      alpha2=(a(axis1,axis1)*(x0(axis2)*a(axis3,axis3)-x0(axis3)*a(axis2,axis3))
             +a(axis2,axis1)*(x0(axis3)*a(axis1,axis3)-x0(axis1)*a(axis3,axis3))
	     +a(axis3,axis1)*(x0(axis1)*a(axis2,axis3)-x0(axis2)*a(axis1,axis3)))/det;
      alpha3=(a(axis1,axis1)*(a(axis2,axis2)*x0(axis3)-a(axis3,axis2)*x0(axis2))
             +a(axis2,axis1)*(a(axis3,axis2)*x0(axis1)-a(axis1,axis2)*x0(axis3))
             +a(axis3,axis1)*(a(axis1,axis2)*x0(axis2)-a(axis2,axis2)*x0(axis1)))/det;
      alpha2Min=min(alpha2Min,alpha2);
      alpha2Max=max(alpha2Max,alpha2);
      alpha3Min=min(alpha3Min,alpha3);
      alpha3Max=max(alpha3Max,alpha3);
    }
  }

  RealArray x2(1,3);
  
//  Intersect the line  x2(alpha1)=x+alpha1*vector1+i2*vector2+i3*vector3
//  with the bounding box

  for( int i3=int(ceil(alpha3Min)); i3<=int(floor(alpha3Max)); i3++ )
  {
    for( int i2=int(ceil(alpha2Min)); i2<=int(floor(alpha2Max)); i2++ )
    {
      for( int axis=0; axis<rangeDimension; axis++ )
        x2(axis)=x(0,axis)+i2*vector(axis,axis2)+i3*vector(axis,axis3);
      intersectLine( x2, nI, xI, vector, xOrigin0, xTangent0 );
    }
  }
}

//========================================================================================
//         constructGrid
//
// Make a grid to use if none is given
// Use a grid size defined from the mapping
//========================================================================================
void ApproximateGlobalInverse::
constructGrid( )
{
  map->getGridSerial();   // Note that ApproximateGlobalInverse::grid is a C reference to map->grid
  //  assign the indexRange and dimension arrays:
  indexRange.redim(2,3);
  indexRange=0;
  int axis;
  for( axis=axis1; axis<domainDimension; axis++ )
  {
    for( int side=0; side<=1; side++) 
      indexRange(side,axis)= map->gridIndexRange(side,axis);
  }
  
  #ifdef USE_PPP
  if( map->usesDistributedInverse() )
  {
    // In parallel, restrict bounds to the local serial array
    // *but* include a ghost line on one side of the parallel boundary so that the bounding boxes
    // cover the entire grid (otherwise there can be a gap)

    const realArray & gridd = map->getGrid();  // here is the distributed array
    for( axis=axis1; axis<domainDimension; axis++ )
    {
      indexRange(0,axis)=max(indexRange(0,axis),grid.getBase(axis) +gridd.getGhostBoundaryWidth(axis));
      indexRange(1,axis)=min(indexRange(1,axis),grid.getBound(axis)-gridd.getGhostBoundaryWidth(axis));
      if( indexRange(0,axis)>map->gridIndexRange(0,axis) && gridd.getGhostBoundaryWidth(axis)>0 )
      {
        // add an extra point to the "left" side of an internal parallel boundary
	indexRange(0,axis)-=1;
      }
    }
    if( Mapping::debug & 2  ) 
    {
      const int myid=max(0,Communication_Manager::My_Process_Number);
      printf("AGI:constructGrid: myid=%i map=%s distributedInverse=%i indexRange=[%i,%i][%i,%i][%i,%i]\n",
	     myid,(const char*)map->getName(Mapping::mappingName),
             int(map->usesDistributedInverse()),
             indexRange(0,0),indexRange(1,0),indexRange(0,1),indexRange(1,1),
	     indexRange(0,2),indexRange(1,2));
    }
    
  }
  #endif

  // *wdh* 020526 : do not include singularity fixes in dimension
  dimension.redim(2,3);
  dimension=indexRange;
  
  for( axis=axis1; axis<domainDimension; axis++ )
  {
    // if the grid has a polar singularity, move the boundary 1 grid line away so that
    // the closest point will be determined properly.
    for( int side=0; side<=1; side++ )
    {
      if( map->getTypeOfCoordinateSingularity(side,axis)==Mapping::polarSingularity )
      {
        // don't adjust if we are really at an internal parallel boundary 
	if( (side==0 && indexRange(side,axis)==0 ) || 
            (side==1 && indexRange(side,axis)==(map->getGridDimensions(axis)-1)) )
	{
	  indexRange(side,axis)+=1-2*side;
	}
      }
    }
  }
  

  gridDefined=true;

  if( false ) 
  {
    printf("AGI::constructGrid: name=%s\n"
           "       grid=[%i,%i][%i,%i][%i,%i][%i,%i]\n"
           " indexRange=[%i,%i][%i,%i][%i,%i]\n",
           (const char*)map->getName(Mapping::mappingName),
	   grid.getBase(0),grid.getBound(0),   
	   grid.getBase(1),grid.getBound(1),   
	   grid.getBase(2),grid.getBound(2),   
	   grid.getBase(3),grid.getBound(3),   
           indexRange(0,0),indexRange(1,0),
           indexRange(0,1),indexRange(1,1),
           indexRange(0,2),indexRange(1,2));
  }
  

  if( Mapping::debug & 128 )
    grid.display("ApproximateGlobalInverse:Here is grid:");
}


//=============================================================================
//  Print statistics for the use of ApproximateGlobalInverse and Exact Inverse
//=============================================================================
void ApproximateGlobalInverse::
printStatistics()
{
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int myid=max(0,Communication_Manager::My_Process_Number);

  real total=timeForApproximateInverse + ExactLocalInverse::timeForExactInverse;
  
  real maxTimeForApproximateInverse = ParallelUtility::getMaxValue(timeForApproximateInverse);
  real maxTimeForFindNearestGridPoint = ParallelUtility::getMaxValue(timeForFindNearestGridPoint);
  real maxTimeForBinarySearchOverBoundary = ParallelUtility::getMaxValue(timeForBinarySearchOverBoundary);
  real maxTimeForBinarySearchOnLeaves = ParallelUtility::getMaxValue(timeForBinarySearchOnLeaves);
  real maxTimeForExactInverse = ParallelUtility::getMaxValue(ExactLocalInverse::timeForExactInverse);
  int  maxNumberOfStencilSearches = ParallelUtility::getMaxValue(numberOfStencilSearches);
  int  maxNumberOfStencilWalks = ParallelUtility::getMaxValue(numberOfStencilWalks);
  int  maxNumberOfBinarySearches = ParallelUtility::getMaxValue(numberOfBinarySearches);
  int  maxNumberOfBoxesChecked = ParallelUtility::getMaxValue(numberOfBoxesChecked);
  int  maxNumberOfBoundingBoxes = ParallelUtility::getMaxValue(numberOfBoundingBoxes);
  int  maxNumberOfNewtonSteps = ParallelUtility::getMaxValue(ExactLocalInverse::numberOfNewtonSteps);
  int  maxNumberOfNewtonInversions = ParallelUtility::getMaxValue(ExactLocalInverse::numberOfNewtonInversions);
  
  real minTimeForApproximateInverse = ParallelUtility::getMinValue(timeForApproximateInverse);
  real minTimeForFindNearestGridPoint = ParallelUtility::getMinValue(timeForFindNearestGridPoint);
  real minTimeForBinarySearchOverBoundary = ParallelUtility::getMinValue(timeForBinarySearchOverBoundary);
  real minTimeForBinarySearchOnLeaves = ParallelUtility::getMinValue(timeForBinarySearchOnLeaves);
  real minTimeForExactInverse = ParallelUtility::getMinValue(ExactLocalInverse::timeForExactInverse);
  int  minNumberOfStencilSearches = ParallelUtility::getMinValue(numberOfStencilSearches);
  int  minNumberOfStencilWalks = ParallelUtility::getMinValue(numberOfStencilWalks);
  int  minNumberOfBinarySearches = ParallelUtility::getMinValue(numberOfBinarySearches);
  int  minNumberOfBoxesChecked = ParallelUtility::getMinValue(numberOfBoxesChecked);
  int  minNumberOfBoundingBoxes = ParallelUtility::getMinValue(numberOfBoundingBoxes);
  int  minNumberOfNewtonSteps = ParallelUtility::getMinValue(ExactLocalInverse::numberOfNewtonSteps);
  int  minNumberOfNewtonInversions = ParallelUtility::getMinValue(ExactLocalInverse::numberOfNewtonInversions);
  

  printF("Mapping Inverse, Statistics: ( %i processors)   \n"
         "---------------------------------------------     time      \n"
         "                                          max/proc          min/proc    \n"   
         " timeForApproximateInverse...........%8.2e %5.1f%%   %8.2e %5.1f%% \n"
         "   timeForFindNearestGridPoint.......%8.2e %5.1f%%   %8.2e %5.1f%% \n"
         "   timeForBinarySearchOverBoundary...%8.2e %5.1f%%   %8.2e %5.1f%% \n"
         "   timeForBinarySearchOnLeaves.......%8.2e %5.1f%%   %8.2e %5.1f%% \n"
         " timeForExactInverse.................%8.2e %5.1f%%   %8.2e %5.1f%% \n"
         "                                             max/proc     min/proc \n"
         " average number of stencil searches/walk... %8.2f    %8.2f \n"
         " number of binary searches................. %8i    %8i   \n"
         " number of boxes checked/( binary search).. %8.2f    %8.2f \n"
         " total number of bounding boxes............ %8i    %8i   \n"
         " number of Newton inversions............... %8i    %8i\n"
         " average number of Newton iterations....... %8.2f    %8.2f \n",
         np,
         maxTimeForApproximateInverse,100.*maxTimeForApproximateInverse/total,
         minTimeForApproximateInverse,100.*minTimeForApproximateInverse/total,
         maxTimeForFindNearestGridPoint,100.*maxTimeForFindNearestGridPoint/total,
         minTimeForFindNearestGridPoint,100.*minTimeForFindNearestGridPoint/total,
         maxTimeForBinarySearchOverBoundary,100.*maxTimeForBinarySearchOverBoundary/total,
         minTimeForBinarySearchOverBoundary,100.*minTimeForBinarySearchOverBoundary/total,
         maxTimeForBinarySearchOnLeaves,100.*maxTimeForBinarySearchOnLeaves/total,
         minTimeForBinarySearchOnLeaves,100.*minTimeForBinarySearchOnLeaves/total,
         maxTimeForExactInverse,100.*maxTimeForExactInverse/total,
         minTimeForExactInverse,100.*maxTimeForExactInverse/total,
         maxNumberOfStencilSearches/real(max(1,maxNumberOfStencilWalks)),
         minNumberOfStencilSearches/real(max(1,minNumberOfStencilWalks)),
         maxNumberOfBinarySearches,
         minNumberOfBinarySearches,
         maxNumberOfBoxesChecked/real(max(1,maxNumberOfBinarySearches)),
         minNumberOfBoxesChecked/real(max(1,minNumberOfBinarySearches)),
         maxNumberOfBoundingBoxes,
         minNumberOfBoundingBoxes,
         maxNumberOfNewtonInversions,
         minNumberOfNewtonInversions,
         maxNumberOfNewtonSteps/real(max(1,maxNumberOfNewtonInversions)),
         minNumberOfNewtonSteps/real(max(1,minNumberOfNewtonInversions))
         );
}  











//===============================================================================
//            ExactLocalInverse
//
// Here we define the routines for the Exact Local Inverse
//===============================================================================

real ExactLocalInverse::timeForExactInverse=0.;
int ExactLocalInverse::numberOfNewtonInversions=0;
int ExactLocalInverse::numberOfNewtonSteps=0;

ExactLocalInverse::
ExactLocalInverse( Mapping & map0 )
// =====================================================================================
/// \details 
///     Build an ExactLocalInverse from a Mapping.
// =====================================================================================
{
  map = &map0;
  uninitialized=true;              // initialize on first call to inverse
  initStaticMappingVariables();    

  nonConvergenceValue=10.;     // value given to inverse when there is no convergence
  newtonToleranceFactor=100.;  // convergence tolerance is this times the machine epsilon
  newtonL2Factor=.1;           // extra factor used in inverting the closest point to a curve or surface
  
  if( REAL_EPSILON == DBL_EPSILON )
    newtonToleranceFactor=1.e4;   // require fewer digits in double precision
  // newtonDivergenceValue : newton is deemed to have diverged if the r value is this much outside [0,1]
  // this may be changed in the initialize() member function.
  newtonDivergenceValue = map->getDomainDimension()==1 ? .5 : .1;  

  useRobustExactLocalInverse=false;
}  

real ExactLocalInverse::
getParameter( const MappingParameters::realParameter & param ) const
// =====================================================================================
/// \details 
///    Return the value of a parameter.
/// \param param (input) : one of {\tt THEnonConvergenceValue}, {\tt THEnewtonToleranceFactor},
///      or {\tt THEnewtonDivergenceValue} or {\tt newtonL2Factor} from the enum {\tt MappingParameters}.
///  
// =====================================================================================
{ 
  real returnValue;
  switch (param)
  {
  case MappingParameters::THEnonConvergenceValue:    // value given to inverse when there is no convergence
    returnValue=nonConvergenceValue;
    break;
  case MappingParameters::THEnewtonToleranceFactor:  // convergence tolerance is this times the machine epsilon
    returnValue=newtonToleranceFactor;
    break;
  case MappingParameters::THEnewtonDivergenceValue:  // newton is deemed to have diverged if the r value is this much outside [0,1]
    returnValue=newtonDivergenceValue;
    break;
  case MappingParameters::THEnewtonL2Factor:
    returnValue=newtonL2Factor;
    break;
  default:
    cout << " ExactLocalInverse::getParameter: fatal error, unkown value for realParameter\n";
    {throw "error";}
  }
  return returnValue;
}

void ExactLocalInverse::
setParameter( const MappingParameters::realParameter & param, const real & value ) 
// =====================================================================================
/// \details 
///    Set the vaule of a parameter.
/// \param param (input) : one of {\tt THEnonConvergenceValue}, {\tt THEnewtonToleranceFactor},
///      or {\tt THEnewtonDivergenceValue} or {\tt THEnewtonL2Factor}  from the enum {\tt MappingParameters}.
/// \param value (input) : value to assign.
///  
// =====================================================================================
{ 

  switch (param)
  {
  case MappingParameters::THEnonConvergenceValue:    // value given to inverse when there is no convergence
    cout << "ExactLocalInverse::setParameter: sorry, you cannot change THEnonConvergenceValue\n";
    // nonConvergenceValue=value;
    break;
  case MappingParameters::THEnewtonToleranceFactor:  // convergence tolerance is this times the machine epsilon
    newtonToleranceFactor=value;
    break;
  case MappingParameters::THEnewtonDivergenceValue:  // newton is deemed to have diverged if the r value is this much outside [0,1]
    newtonDivergenceValue=value;
    break;
  case MappingParameters::THEnewtonL2Factor: 
    newtonL2Factor=value;
    break;
  default:
    cout << " ExactLocalInverse:: fatal error, unkown value for realParameter\n";
    {throw "error";}
  }
}

//! Use a more robust exact local inverse
void ExactLocalInverse::
useRobustInverse(const bool trueOrFalse )
{
  useRobustExactLocalInverse=trueOrFalse;
}



real ExactLocalInverse::
sizeOf(FILE *file /* = NULL */ ) const
// =======================================================================================
/// \details 
///    Return size of this object  
// =======================================================================================
{
  real size=sizeof(*this);

  return size;
}

void ExactLocalInverse::
reinitialize()
// ====================================================================================
/// \details 
///    This will mark ExactLocalInverse as being in need of initialization.
///    The actual call to initialize will occur when the inverse is actually used.
// ===================================================================================
{
  uninitialized=true;
  // ***wdh**** 990325 initialize();
}


int ExactLocalInverse::
get( const GenericDataBase & dir, const aString & name)
// =====================================================================================
/// \details 
///     Get this object from a sub-directory called "name"
// =====================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"ExactLocalInverse");

  subDir.get( nonConvergenceValue,"nonConvergenceValue" );
  subDir.get( newtonToleranceFactor,"newtonToleranceFactor" );
  subDir.get( newtonDivergenceValue,"newtonDivergenceValue" );
  subDir.get( newtonL2Factor,"newtonL2Factor" );

  delete & subDir;
  return true;
}

int ExactLocalInverse:: 
put( GenericDataBase & dir, const aString & name) const
// =====================================================================================
/// \details 
///  save this object to a sub-directory called "name"
// =====================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"ExactLocalInverse");                      // create a sub-directory 

  subDir.put( nonConvergenceValue,"nonConvergenceValue" );
  subDir.put( newtonToleranceFactor,"newtonToleranceFactor" );
  subDir.put( newtonDivergenceValue,"newtonDivergenceValue" );
  subDir.put( newtonL2Factor,"newtonL2Factor" );

  delete &subDir;
  return true;
}

void ExactLocalInverse::
initialize()
// =====================================================================================
/// \details 
///    Initialize.
// =====================================================================================
{
  uninitialized=false;
  if( Mapping::debug & 8 )
    cout << "ExactLocalInverse -- initialize" << endl;  
  domainDimension=map->getDomainDimension();
  rangeDimension=map->getRangeDimension();
  Axes = Index(axis1,domainDimension);
  xAxes = Index(axis1,rangeDimension);

  periodVector.redim(3,3);
  int axis;
  for( axis=axis1; axis<rangeDimension; axis++)
    for( int i=axis1; i<domainDimension; i++ )
      periodVector(axis,i)=map->getPeriodVector(axis,i);

  // If the mapping has a coordinate singularity we may have to use least squares to invert
  mappingHasACoordinateSingularity=map->hasACoordinateSingularity();

  // newtonDivergenceValue : newton is deemed to have diverged if the r value is this much outside [0,1]
  // since we are expected to converge on ghost values, extend the convergence region outside [0,1]
  // by 2.5 ghostlines.
  for( axis=0; axis<map->getDomainDimension(); axis++ )
    newtonDivergenceValue=max(newtonDivergenceValue,2.5/(max(1.,map->getGridDimensions(axis)-1.)));
  if( Mapping::debug & 4 )
    printf("ExactLocalInverse:initialize: The mapping named %s : newtonDivergenceValue=%e \n",
           (const char *)map->getName(Mapping::mappingName),newtonDivergenceValue);

  uninitialized=false;
}

ExactLocalInverse::
~ExactLocalInverse()
{ }
  
int ExactLocalInverse::
compressConvergedPoints(Index & I,
                        RealArray & x, 
			RealArray & r, 
			RealArray & ry, 
			RealArray & det, 
                        IntegerArray & status,
			const RealArray & x1, 
			RealArray & r1, 
			RealArray & rx1, 
			MappingWorkSpace & workSpace,
			const int computeGlobalInverse )
// ===============================================================================
/// \details 
///    Remove points that have converged or diverged so that we will only iterate
///   on the smaller number of points that haven't converged,
// ==============================================================================
{
  // printf("reducing the list\n");
  
  // first save the converged/diverged points
  // Copy r,rx -> r1,rx1 
  int computeMap, computeMapDerivative;
  int base1, bound1;
  Index I1 = map->getIndex( x1,r1,rx1,base1,bound1,computeMap,computeMapDerivative );

  IntegerArray & index0 = workSpace.index0;
  
  if( !workSpace.index0IsSequential )
  {
    if( computeMap )
    {
      // bug here is r1 is a view of an array with a non-zero base -- see bug31.C
      Range I1=r1.dimension(0);
      RealArray r02(I1,Axes); r02=r1(I1,Axes);
      where( status(I)==0 ) 
      {
	for( int axis=axis1; axis<domainDimension; axis++ )
	  r02(index0(I),axis)= r(I,axis);     
      }
      elsewhere( status(I)<0 )
      {
	for( int axis=axis1; axis<domainDimension; axis++ )
	  r02(index0(I),axis)= ApproximateGlobalInverse::bogus; 
      }
      r1(I1,Axes)=r02;
    }
    if( computeMapDerivative )  
      if( domainDimension==rangeDimension )
      {
        where( status(I)==0 )
        {
	  for( int d=0; d<domainDimension; d++ )
	    for( int r=0; r<domainDimension; r++ )
	      rx1(index0(I),d,r)=ry(I,d,r)/det(I); 
	}
      }
  }
  else
  {
    if( computeMap )
    {
      where( status(I)==0 )  // converged
      {
	for( int axis=axis1; axis<domainDimension; axis++ )
	  r1(I,axis)= r(I,axis);     
      }
      elsewhere( status(I)<0 )
      {
	for( int axis=axis1; axis<domainDimension; axis++ )
	  r1(I,axis)= ApproximateGlobalInverse::bogus;  
      }
    }
    if( computeMapDerivative )  
    {
      if( domainDimension==rangeDimension )
      {
        where( status(I)==0 )
	{
          for( int r=axis1; r<rangeDimension; r++ )
            for( int d=axis1; d<domainDimension; d++ )
	      rx1(I,d,r)=ry(I,d,r)/det(I); 
	}
      }
    }
  }
  if( workSpace.index0IsSequential )
  {
    workSpace.index0.redim(I);
    workSpace.index0.seqAdd(base,1);  // we need to build the indirect addressing array.
    workSpace.index0IsSequential=false;
  }
  
  // make a new list of points that remain, save converged/diverged points.
  int j=base;
  for( int i=base; i<=bound; i++ )
  {
    if( status(i)==1 )
    {
      index0(j)=index0(i);  // points into the original arrays r1,x1
      x(j,xAxes)=x(i,xAxes);
      r(j,Axes)=r(i,Axes);   
      j++;
    }
  }
  bound=j-1;
  I=Range(base,bound);
  status(I)=1;

  r.resize(I,domainDimension);

  return 0;
}

#define DISTANCE1(y) ( fabs( y(0,0)-x(i,0) ) )
#define DISTANCE2(y) ( SQR( y(0,0)-x(i,0) ) + SQR( y(0,1)-x(i,1) ) )
#define DISTANCE3(y) ( SQR( y(0,0)-x(i,0) ) + SQR( y(0,1)-x(i,1) ) + SQR( y(0,2)-x(i,2) ) )


void ExactLocalInverse::
inverse(const RealArray & x1, 
	RealArray & r1, 
	RealArray & rx1, 
	MappingWorkSpace & workSpace,
	const int computeGlobalInverse )
//===============================================================================
/// \details 
///    Compute the inverse of the mapping using Newton's method.
///    The initial guess must be good enough for Newton to converge
/// 
/// \param x1,r1,rx1 (input/output) : 
/// \param workSpace (input) :
/// \param computeGlobalInverse (input): 
///      true means that the approximateGlobal inverse routine was called previous
///      to this call. In this case we look for information in the workSpace.
///      false means that the approximateGlobalInverse was not called before this
///      call.
//===============================================================================
{ 
  real time0=getCPU();
  int i;
  
  if( Mapping::debug>0 )
    Mapping::openDebugFiles();

  FILE *&pDebugFile = Mapping::pDebugFile;
  
  if( uninitialized )
    initialize();

  recursionLevel++;   // keeps track of how deeply nested we are

  const bool usesDistributedMap = map->usesDistributedMap();

  int computeMap, computeMapDerivative;

  if( Mapping::debug & 8 )
    fprintf(pDebugFile,"Newton: computeGlobalInverse =%i\n",computeGlobalInverse);


  RealArray & x = computeGlobalInverse ? workSpace.x0 : (RealArray&)x1;
  //   RealArray & r = computeGlobalInverse ? workSpace.r0 :  r1;   
  RealArray r; 
  if( computeGlobalInverse )
    r.reference(workSpace.r0);
  else
    r=r1;   // we need to make a copy in this case since we may redim below when we compress

  RealArray & rx= computeGlobalInverse ? workSpace.rx0 : rx1;

  // Use the basic inverse if the mapping can invert itself
  // For multiple periodic images use the image that it closest to the
  // centre of the parameter space.
  if( map->getBasicInverseOption()==Mapping::canInvert ||
      map->getBasicInverseOption()==Mapping::canInvertWithGoodGuess )
  {
    if( workSpace.index0IsSequential )
    {
      #ifndef USE_PPP
        map->basicInverse( x1,r,rx1 );  // can use user supplied inverse  *wdh* 070329 use r instead of r1
      #else  
        map->basicInverseS( x1,r,rx1 );  // can use user supplied inverse
      #endif

      Range I = x1.dimension(0);
      r1(I,Axes)=r(I,Axes);  // copy back the solution *wdh* 070329
    }
    else
    {
      // space must be periodic 
      map->getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );
      #ifndef USE_PPP
        map->basicInverse( x,r,rx );  // can use user supplied inverse
      #else
        map->basicInverseS( x,r,rx );  // can use user supplied inverse
      #endif

      int i0;
      IntegerArray & index0 = workSpace.index0;
      for( i=base; i<=bound; )
      {
	if( computeMap )
	  r1(index0(i),Axes)= r(i,Axes); 
	i0=i;
	while( i<bound && index0(i+1)==index0(i) )  // multiple periodic images!
	{
	  i++;
	  if( max(fabs(r(i,Axes)-.5)) < max(fabs(r(i0,Axes)-.5)) )
	  {
	    r1(index0(i),Axes)=r(i,Axes); i0=i;
	  }
	}
	if( computeMapDerivative )  
	  if( domainDimension==rangeDimension )
	    rx1(index0(i0),Axes,xAxes)=rx(i0,Axes,xAxes); 
	  else
	    rx1(index0(i0),Axes,xAxes)=0.;  // what to return in this case??

	i++;
      }
    }
    return;
  }
  
  Index I;
  if( !computeGlobalInverse ) // don't use work space in this case
  {
    I = map->getIndex( x1,r1,rx1,base,bound,computeMap,computeMapDerivative );
  }
  else
  {
    I = workSpace.I0;
    base=I.getBase();    // sets global values
    bound=I.getBound();
  }


  RealArray y(I,rangeDimension), yr(I,rangeDimension,domainDimension); 
  RealArray dy(I,rangeDimension),dr(I,rangeDimension),ry(I,domainDimension,rangeDimension),det(I);
  IntegerArray status(I);

  //    status(i) = 0 : Newton converged
  //              = 1 : Newton hasn't converged (yet)
  //              =-1 : Newton diverged
  //              =-2 : really diverged
  status(I)=1;
  where( r(I,axis1)==ApproximateGlobalInverse::bogus )
  { // The stencilWalk may put a bogus value in if the point is a lot outside the boundingbox.
    // we need to put a smaller value in the r array so that some mappings don't choke.
    status(I)=-1;
    for( int axis=0; axis<domainDimension; axis++ )
      r(I,axis)=.5; // *wdh* 960817 --- put non bogus value
  }

  // convergence tolerance:  (note that the convergence test is checking |dr| for the last
  //  iteration BUT the error is really more like |dr|*|dr| for quadratic convergence )
  real eps=REAL_EPSILON*newtonToleranceFactor; // REAL_EPSILON*100.; 
  real goodEnoughEps=FLT_EPSILON*100.;  // this is good enough if we cannot make the former tolerance

  // ** for debugging *** goodEnoughEps=.01;  // *****
  
  int maxit;
  maxit=int( log(FLT_EPSILON*newtonToleranceFactor)/log(.5) );  // this should be good enough even in double precision.
  if( FLT_EPSILON!=REAL_EPSILON )
    maxit+=2;   // add a few in more in double precision.
  
  // restrict a Newton step to be at most one grid spacing:

// ********
  // newtonDivergenceValue=.25;
  newtonDivergenceValue=1.;   // *wdh* 990703 -- larger value might be ok with more robust hole cutting
  // After some iterations, max dr correction should be less than this,  *wdh* 2012/07/08 
  // or else we assume the iteration is not converging
  const real drDivergenceValue=.1;  
// ********
  
  int axis;
  for( axis=axis1; axis<domainDimension; axis++ )
  {
    // ** drMax[axis]=min(.2,1./max(1,map->getGridDimensions(axis)));
    // drMax[axis]=.2;  //  ***wdh*** 990224
    drMax[axis]=max(.2,1.1*newtonDivergenceValue);  
  }  
  
  if( Mapping::debug & 8 )
  {
    fprintf(pDebugFile,"Newton: eps=%g, maxit =%i\n",eps,maxit);
    for( i=base; i<=bound; i++ )
      fprintf(pDebugFile," i=%i, r =(%g,%g,%g)\n",i,r(i,axis1),
              (domainDimension > 1 ? r(i,axis2) : 0.),
              (domainDimension > 2 ? r(i,axis3) : 0.));
  }
  const int iterationToStartDamping= 4;
//  const int iterationToAllowOutsideConvergence = domainDimension>1 ? iterationToStartDamping+2 : maxit;
  const int iterationToAllowOutsideConvergence = iterationToStartDamping+2;
  const int iterationToStartWarning = iterationToStartDamping+ ( FLT_EPSILON==REAL_EPSILON ? 2 : 4 );

  numberOfNewtonInversions+=(bound-base+1);
  if( false && useRobustExactLocalInverse && domainDimension==1 )
  {
    // *** I am not sure if this works ?? ***
    // peform a bisection type algorithm
    dr(I,Axes)=1./(map->getGridDimensions(axis1)-1);
    minimizeByBisection(r,x,dr,status,eps );
  }
  else  if( domainDimension == rangeDimension )
  {
    // --- Square Jacobian : do some Newton iterations ---
    int iter;
    for( iter=1; iter<=maxit; iter++ )
    {
      numberOfNewtonSteps+=(bound-base+1);

      #ifndef USE_PPP
        map->map( r,y,yr );
      #else
        map->mapS( r,y,yr );
      #endif

      dy(I,xAxes)=x(I,xAxes)-y(I,xAxes);
      invert( yr,dy,det,ry,dr,status);   // get dr=yr^(-1)*dy
      if( iter>iterationToStartDamping )
      { // damp the correction:
        where( dr(I,Axes)>goodEnoughEps*100. )
  	  dr(I,Axes)*=.5;
      }
      r(I,Axes)=r(I,Axes)+dr(I,Axes);
      periodicShift( r,I );   // shift r into [0,1] if the mapping is functionPeriodic

      if( (Mapping::debug & 2 && iter > iterationToStartWarning) )
      {
        fprintf(pDebugFile,"Newton: iter=%i, max(fabs(dr(Axes,I)))=%g min(fabs(det(I)))=%g"
                " base=%i, bound=%i newtonDivergenceValue=%g \n",iter,max(fabs(dr(I,Axes))),
                min(fabs(det(I))),base,bound,newtonDivergenceValue);
        if( domainDimension==3 )
	{
	  for( int i=base; i<=bound; i++ )
	    fprintf(pDebugFile," r(.,%i) = (%e,%e,%e), y=(%6.2e,%6.2e,%6.2e) x=(%6.2e,%6.2e,%6.2e) "
		   "max(dr)=%e\n",
		   i,r(i,0),r(i,1),r(i,2),y(i,0),y(i,1),y(i,2),
		   x(i,0),x(i,1),x(i,2),max(dr(i,Axes)));
	}
        else if( domainDimension==2 )
	{
	  for( int i=base; i<=bound; i++ )
            if( status(i)==1 )  
  	      fprintf(pDebugFile," r(.,%i)=(%6.2e,%6.2e), mapS(r)=(%6.2e,%6.2e) x=(%6.2e,%6.2e) dr=(%6.2e,%6.2e)\n",
		   i,r(i,0),r(i,1),y(i,0),y(i,1),x(i,0),x(i,1),dr(i,0),dr(i,1));
	}
      }
      for( int i=base; i<=bound; i++)  // Check for convergence or divergence
      {
        // *wdh* 2011/06/12 -- for testing check for nan's
	if( false )
	{
	  if( domainDimension==2 )
	  {
	    if( r(i,0)!=r(i,0) || r(i,1)!=r(i,1) )
	    {
              if( true || Mapping::debug & 4 )
	      {
		printf("ExactLocalInverse::inverse:Newton: ERROR: r=(%e,%e) nan's ?? map=%s\n",r(i,0),r(i,1),
		       (const char*)map->getName(Mapping::mappingName));
	      }
              r(i,0)=r(i,1)=Mapping::bogus;
	      status(i)=-1;   // iteration not converging
	    }
	  }
	  else if( domainDimension==3 )
	  {
	    if( r(i,0)!=r(i,0) || r(i,1)!=r(i,1) || r(i,2)!=r(i,2) )
	    {
	      if( true || Mapping::debug & 4 )
	      {
		printf("ExactLocalInverse::inverse:Newton: ERROR: r=(%e,%e,%e) nan's ?? map=%s\n",r(i,0),r(i,1),r(i,2),
		       (const char*)map->getName(Mapping::mappingName));
	      }
	      
              r(i,0)=r(i,1)=r(i,2)=Mapping::bogus;
	      status(i)=-1;   // iteration not converging
	    }
	  }
	}
	    
        if( status(i)==1 )  
	{
	  const real rDelta = max(fabs(r(i,Axes)-.5));
          const real maxDr = max(fabs(dr(i,Axes)));
          if( rDelta > .5+newtonDivergenceValue )    // iteration diverged
	  {
	    status(i)=-1;
	    if( maxDr>drDivergenceValue )
	    {
	      for( int dir=0; dir<domainDimension; dir++ )
	        r(i,dir)=Mapping::bogus;    // *wdh* 2012/07/08
	    }
	  }
  	  else if( maxDr < eps )        // iteration converged
	     status(i)=0;                 
          else if( iter>iterationToAllowOutsideConvergence )
	  {
	    // *wdh* 2012/07/08 if( rDelta > .5+maxDr*2. )   // define convergence if outside the grid and dr is small
            // define convergence if outside the grid and dr is small:
	    if( (rDelta > .5+maxDr*2.) && (maxDr<drDivergenceValue) )  
	      status(i)=0;                              // this prevents many iterations for some mappings such as
            // else if( maxDr>newtonDivergenceValue )
            else if( maxDr>drDivergenceValue )
	    {
              status(i)=-1;   // iteration not converging
	      for( int dir=0; dir<domainDimension; dir++ )
		r(i,dir)=Mapping::bogus;    // *wdh* 2012/07/08
	    }
	  }
	}                                             // points outside the data point mapping.
      }
	
      // printf(" it=%2i, pts left=%i\n",iter,sum(status(I)==1));

      //  Check for convergence or divergence at all points
      int numberLeft = sum(status(I)==1);
      #ifdef USE_PPP
        // If the map.map function uses communication then we must iterate the same number of
        // times on all processors 
        if( usesDistributedMap )
            numberLeft=ParallelUtility::getMaxValue(numberLeft);
      #endif
      if( numberLeft==0 )
      {
        if( Mapping::debug & 8 || 
           (iter > iterationToStartWarning && numberOfSlowConvergenceMessages<maximumNumberOfSlowConvergenceMessages) )
          cout << "*******Warning:mapping:Inverse:Newton: iterations = " << iter << endl;
        break;   // exit iterations
      }
      /* --
      if( false && numberLeft < I.getLength()*.5 &&  I.getLength()>5 )
      {
        printf("compress points, numberLeft=%i\n",numberLeft);
        compressConvergedPoints(I,x,r, status,x1,r1,rx1,workSpace,computeGlobalInverse );
      }
      --- */
    }
    // 
    if( iter > iterationToStartWarning && goodEnoughEps>eps )
    {
      // **double precision** Allow a lesser convergence tolerance since the Mapping may
      // be defined with single precision numbers (plot3d file for example)
      if( numberOfSlowConvergenceMessages<maximumNumberOfSlowConvergenceMessages )
      {
	numberOfSlowConvergenceMessages++;
	printf("Mapping::Inverse:Newton:WARNING:slow convergence in double precision for mapping %s. \n"
	       "                                Using larger convergence tolerance=%6.3e instead of tolerance=%6.3e\n",
	       (const char*)map->getName(Mapping::mappingName),goodEnoughEps,eps);
	if( numberOfSlowConvergenceMessages==(maximumNumberOfSlowConvergenceMessages-1) )
	{
	  printf("\n **** Mapping::Newton Inverse: too many `slow convergence' warnings. "
		 "I will not print any-more ***\n\n");
	
	}      
      }
      
      for( int i=base; i<=bound; i++)  // Check for convergence or divergence
      {
	if( status(i)==1 )  
	{
          if( Mapping::debug & 4 )
	  {
	    if( domainDimension==3 )
	      fprintf(pDebugFile,"Mapping::Newton:WARNING: poor convergence i=%i, r=(%e,%e,%e), dr=(%e,%e,%e), goodEnoughEps=%e \n",
		     i,r(i,axis1),r(i,axis2),r(i,axis3),dr(i,axis1),dr(i,axis2),dr(i,axis3),goodEnoughEps); 
	    else if( domainDimension==2 )
	      fprintf(pDebugFile,"Mapping::Newton:WARNING: poor convergence i=%i, r=(%e,%e), dr=(%e,%e), goodEnoughEps=%e \n",
		     i,r(i,axis1),r(i,axis2),dr(i,axis1),dr(i,axis2),goodEnoughEps); 
	    else 
	      fprintf(pDebugFile,"Mapping::Newton:WARNING: poor convergence i=%i, r=%e, dr=%e, goodEnoughEps=%e \n",
		     i,r(i,axis1),dr(i,axis1),goodEnoughEps); 
	  }
	  if( max(fabs(dr(i,Axes))) < goodEnoughEps )      
	    status(i)=0;                 // iteration converged
	}
      }
    }
  }
  else if( useNewNonSquareInverse )
  {
    maxit=20; // *wdh* 010831 was =5;

    RealArray r2(I,domainDimension);

    real deltaR=0.;
    for( axis=0; axis<domainDimension; axis++ )
      deltaR=max(deltaR,1./(map->getGridDimensions(axis)-1));
    deltaR*=.5;

    // extra tolerance used if we are far from the surface
    // epsL2: measures a distance in r space : if we are off the surface by this much
    //        then we use a bigger tolerance
    //  For very coarse grids we should decrease this factor.
    // .005 -> if the grid has more than 20 pts then use newtonL2Factor
    const real epsL2=min(newtonL2Factor,.005/(2.*deltaR));  

    RealArray dyNorm(I), a00(I),a01(I),a11(I),b0(I),b1(I),detInverse(I);

    if( domainDimension == 1 && rangeDimension==2 )
    {
      // for curves we adjust the initial guess -- **this is needed for curves with sharp corners**
      // 
      #ifndef USE_PPP
        map->map( r,y );
      #else
        map->mapS( r,y );
      #endif
      dyNorm=SQR(y(I,0)-x(I,0))+SQR(y(I,1)-x(I,1));
      
      for( int side=0; side<=1; side++ )
      {
        real delta = side==0 ? -.5*deltaR : .5*deltaR;
        #ifndef USE_PPP
  	  map->map( r+delta,y );
        #else
    	  map->mapS( r+delta,y );
        #endif
	a00=SQR(y(I,0)-x(I,0))+SQR(y(I,1)-x(I,1));
	where( a00 < dyNorm )
	{
	  r+=delta;
	  dyNorm=a00;
	}
      }
      
    }

    for( int iter=1; iter<=maxit; iter++ )
    {
      numberOfNewtonSteps+=(bound-base+1);

      #ifndef USE_PPP
        map->map( r,y,yr );
      #else
        map->mapS( r,y,yr );
      #endif

      if( Mapping::debug & 16 )
      {
	int outIndex;
	outIndex = base;
// tmp
	outIndex = 88;
	
	if( rangeDimension==3 )
	{
	  real j00,j01,j11,det;
	  j00 = yr(outIndex,0,0)*yr(outIndex,0,0)+yr(outIndex,1,0)*yr(outIndex,1,0)+yr(outIndex,2,0)*yr(outIndex,2,0);
	  j01 = yr(outIndex,0,0)*yr(outIndex,0,1)+yr(outIndex,1,0)*yr(outIndex,1,1)+yr(outIndex,2,0)*yr(outIndex,2,1);
	  j11 = yr(outIndex,0,1)*yr(outIndex,0,1)+yr(outIndex,1,1)*yr(outIndex,1,1)+yr(outIndex,2,1)*yr(outIndex,2,1);
	  det = j00*j11-j01*j01;
	  
	  printf(" it=%2i, x=(%8.2e,%8.2e,%8.2e), y=(%8.2e,%8.2e,%8.2e), r=(%8.2e,%8.2e), det=%8.2e, status=%i\n"
		 "\n",iter,
		 x(outIndex,0), x(outIndex,1), x(outIndex,2), y(outIndex,0), y(outIndex,1), y(outIndex,2), 
		 r(outIndex,0), r(outIndex,1), det, status(outIndex));
	}
      }

      dy(I,xAxes)=x(I,xAxes)-y(I,xAxes);

      // Solve the least squares problem -- just use the Normal equations. QR not needed?
      //    A dr = dy
      // A = yr^T yr
      if( domainDimension == 1 )
      {
        if( rangeDimension==3 )
	{
	  detInverse=1./max(REAL_MIN,yr(I,0,0)*yr(I,0,0)+yr(I,1,0)*yr(I,1,0)+yr(I,2,0)*yr(I,2,0));
	  dr(I,axis1)=(yr(I,0,0)*dy(I,0)+yr(I,1,0)*dy(I,1)+yr(I,2,0)*dy(I,2))*detInverse;

          dyNorm=SQRT( (SQR(dy(I,0))+SQR(dy(I,1))+SQR(dy(I,2)) )*detInverse );
	}
	else if( rangeDimension==2 )
	{
	  detInverse=1./max(REAL_MIN,yr(I,0,0)*yr(I,0,0)+yr(I,1,0)*yr(I,1,0));
	  dr(I,axis1)=(yr(I,0,0)*dy(I,0)+yr(I,1,0)*dy(I,1))*detInverse;

          dyNorm=SQRT( (SQR(dy(I,0))+SQR(dy(I,1)) )*detInverse );
	}
	else
	{
	  detInverse=1./max(REAL_MIN,yr(I,0,0)*yr(I,0,0));
	  dr(I,axis1)=(yr(I,0,0)*dy(I,0))*detInverse;

          dyNorm=SQRT( SQR(dy(I,0))*detInverse );
	}
      }
      else if( domainDimension==2 )
      {
        if( rangeDimension==3 )
	{
	  a00 = yr(I,0,0)*yr(I,0,0)+yr(I,1,0)*yr(I,1,0)+yr(I,2,0)*yr(I,2,0);
	  a01 = yr(I,0,0)*yr(I,0,1)+yr(I,1,0)*yr(I,1,1)+yr(I,2,0)*yr(I,2,1);
	  a11 = yr(I,0,1)*yr(I,0,1)+yr(I,1,1)*yr(I,1,1)+yr(I,2,1)*yr(I,2,1);
	  b0  = yr(I,0,0)*dy(I,0)  +yr(I,1,0)*dy(I,1)  +yr(I,2,0)*dy(I,2);
	  b1  = yr(I,0,1)*dy(I,0)  +yr(I,1,1)*dy(I,1)  +yr(I,2,1)*dy(I,2);
	  detInverse = a00*a11-a01*a01;
	  where (fabs(detInverse) > 10*REAL_MIN)
	  {
	    dr(I,0)=(b0*a11-b1*a01)/detInverse;
	    dr(I,1)=(b1*a00-b0*a01)/detInverse;
	  }
	  otherwise()
	  {
	    status(I) = -1;
	    dr(I,0)=0;
	    dr(I,1)=0;
	  }
	  
          dyNorm=SQRT( (SQR(dy(I,0))+SQR(dy(I,1))+SQR(dy(I,2)) )/(a00+a11) );
	}
	else if( rangeDimension==2 )
	{
	  a00 = yr(I,0,0)*yr(I,0,0)+yr(I,1,0)*yr(I,1,0);
	  a01 = yr(I,0,0)*yr(I,0,1)+yr(I,1,0)*yr(I,1,1);
	  a11 = yr(I,0,1)*yr(I,0,1)+yr(I,1,1)*yr(I,1,1);
	  b0  = yr(I,0,0)*dy(I,0)  +yr(I,1,0)*dy(I,1);
	  b1  = yr(I,0,1)*dy(I,0)  +yr(I,1,1)*dy(I,1);
	  detInverse=1./(a00*a11-a01*a01);
	  dr(I,0)=(b0*a11-b1*a01)*detInverse;
	  dr(I,1)=(b1*a00-b0*a01)*detInverse;

          dyNorm=SQRT( (SQR(dy(I,0))+SQR(dy(I,1)) )/(a00+a11) );
	}
        else
	{
	  a00 = yr(I,0,0)*yr(I,0,0);
	  a01 = yr(I,0,0)*yr(I,0,1);
	  a11 = yr(I,0,1)*yr(I,0,1);
	  b0  = yr(I,0,0)*dy(I,0);
	  b1  = yr(I,0,1)*dy(I,0);
	  detInverse=1./(a00*a11-a01*a01);
	  dr(I,0)=(b0*a11-b1*a01)*detInverse;
	  dr(I,1)=(b1*a00-b0*a01)*detInverse;
          
          dyNorm=fabs(dy(I,0))/( fabs(yr(I,0,0))+fabs(yr(I,0,1)) );
	}
      }
      
      if( Mapping::debug & 16 )
      {
	int outIndex;
	outIndex = base;
// tmp
	outIndex = 88;
	
	if( domainDimension==2 )
	  fprintf(pDebugFile," it=%2i, r=(%8.2e,%8.2e) dr=(%8.1e,%8.1e) |dy|=%6.2e, pts left=%i\n",iter,r(outIndex,0),
		 r(outIndex,1),dr(outIndex,0),dr(outIndex,1), dyNorm(outIndex),sum(status(I)==1));
	else if( domainDimension==1 )
	  cout << " r = (" << r(outIndex,axis1) << "), ";
	if( rangeDimension==2 )
	{
	  cout << " y,x,dy = (" 
	       <<  " (" << y(outIndex,axis1) << "," << y(outIndex,axis2) << "), "
	       <<  " (" << x(outIndex,axis1) << "," << x(outIndex,axis2) << "), "
	       <<  " (" <<dy(outIndex,axis1) << "," <<dy(outIndex,axis2) << "), " << endl;
	}
      }
      
      if( iter<iterationToStartDamping && max(fabs(dr(I,Axes)))< deltaR )
      {
	r(I,Axes)+=dr(I,Axes);
      }
      else
      {
	// damp the correction:
	where( fabs(dr(I,Axes))>goodEnoughEps*100. )
	  dr(I,Axes)*=.5;        // damp the iteration (need if the 2nd derivative is discontinuous)
	r(I,Axes)+=dr(I,Axes);
      }
      
      periodicShift( r,I );   // shift r into [0,1] if the mapping is functionPeriodic

      // compute actual change in r, taking into account getting stuck at the end points and periodicity

      r(I,Axes)=min(1.05,max(r(I,Axes),-.05));   // allow answer to be slightly outside wdh: 970502

      r2(I,Axes)=fabs(dr(I,Axes));
      if( iter>iterationToAllowOutsideConvergence )
      {
        // allow points to converge if they lie outside [-.04,1.04]
	where( fabs(r(I,Axes)-.5) > .54 )  // *wdh* 010901
	  r2(I,Axes)=0.;
      }
      
      
     
      for( int i=base; i<=bound; i++)  // Check for convergence or divergence
      {
        // dyNorm = || dy || / || dy/dr || : estimate of how far we are from the curve in r-coordinates
        //                                 : allow a less strict convergence tolerance if this is large

        // if( status(i)==1 && ( (max(r2(i,Axes)) < (eps+epsL2*dyNorm(i))) ) )
        // *wdh* 010416 if( status(i)==1 && ( (max(r2(i,Axes)) < (eps+epsL2*dyNorm(i))) || dyNorm(i)>.2 ) ) 
        // *wdh* 010416 .2 was too small for curves with very few grid points)
        if( status(i)==1 && ( (max(r2(i,Axes)) < (eps+epsL2*dyNorm(i))) || (iter>3 && dyNorm(i)>.25) ) ) 
	{
	  status(i)=0;                  // iteration converged
	}
      }
      
      if( iter>15 || Mapping::debug & 4 )
      {
        if( iter==16 )
	{
	  printf("WARNING:Newton(L2): more than 15 iterations required\n");
	}
        if( Mapping::debug & 4 )
	{
	  int axs2=min(1,domainDimension-1);
	  for( int i=base; i<=bound; i++)
	  {
	    if( max(r2(i,Axes)) > eps )
	    {
	      if( domainDimension==2 )
		printf("Newton(L2): it=%i i=%i status=%i dr=(%6.0e,%6.0e) r=(%8.2e,%8.2e) 1/det=%7.1e dyNorm=%7.1e",
		       iter,i,status(i),dr(i,0),dr(i,axs2),r(i,0),r(i,axs2),detInverse(i),dyNorm(i));
	      else
		printf("Newton(L2): it=%i i=%i status=%i dr=%6.0e r=%8.2e 1/det=%7.1e dyNorm=%7.1e",
		       iter,i,status(i),dr(i,0),r(i,0),detInverse(i),dyNorm(i));
	      if( rangeDimension==1 )
		printf(" x=%8.2e",x(i,axis1));
	      else if( rangeDimension==2 )
		printf(" x=(%8.2e,%8.2e)",x(i,axis1),x(i,axis2));
	      else 
		printf(" x=(%8.2e,%8.2e,%8.2e)",x(i,axis1),x(i,axis2),x(i,axis3));
	      if( rangeDimension==1 )
		printf(" y=%8.2e ",y(i,axis1));
	      else if( rangeDimension==2 )
		printf(" y=(%8.2e,%8.2e) ",y(i,axis1),y(i,axis2));
	      else 
		printf(" y=(%8.2e,%8.2e,%8.2e) ",y(i,axis1),y(i,axis2),y(i,axis3));
	      printf("\n");
	    }
	  }
	}
      }
      //  Check for convergence.
      int numberLeft = sum(status(I)==1);
      #ifdef USE_PPP
      if( usesDistributedMap ) // *wdh* 2011/10/02
      { // if the mapS() function requires communication we cannot finish until all processors are done.
        // we could do better here to save computations?
        numberLeft = ParallelUtility::getMaxValue(numberLeft);
      }
      #endif
      
      if( numberLeft==0 )
        break;
      

    }  // end for iter

    if( goodEnoughEps>eps )
    {
      // **double precision** Allow a lesser convergence tolerance since the Mapping may
      // be defined with single precision numbers (plot3d file for example)
      for( int i=base; i<=bound; i++)  // Check for convergence or divergence
      {
	if( status(i)==1 )  
	{
          if( Mapping::debug & 2 )
	  {
	    printf("Mapping::Newton:WARNING: poor convergence i=%i, ",i);
	    if( domainDimension==2 )
	      printf("r=(%6.1e,%6.1e), dr=(%6.1e,%6.1e), goodEnoughEps=%6.1e \n",
		     r(i,axis1),r(i,axis2),r2(i,axis1),r2(i,axis2),goodEnoughEps); 
	    else
	      printf("r=%6.1e, dr=%6.1e, goodEnoughEps=%e \n",
		     r(i,axis1),r2(i,axis1),goodEnoughEps); 
	  }
	  // if( max(fabs(r2(i,Axes))) < goodEnoughEps )   // *wdh* 990113   
          // we assume we always converge, unless we are at a "saddle-pt" in which case we have one answer
          // of more than one possible answer
	  status(i)=0;                 // iteration converged
	}
      }
    }

    
  }
  else // !useNewNonSquareInverse // AP: The following is obsolete code
  {
    // --- NonSquare Jacobian : minimize the L2 norm distance ---
    // maxit+=10; // may need more
    // eps*=10.;   // less severe convergence for minimum distance !!

    // There should always be a solution to this problem
    // Method:
    //   minimize : g(r) = (y(r)-x)^T(y(r)-x)
    //      h_i(r) = g_{r_i} = 2 y_{r_i}^T (y-x)
    //   Apply Newton to Solve h_i(r) =0   
    
    RealArray r2(I,domainDimension),yr2(I,rangeDimension,domainDimension),
              yrr(I,rangeDimension,domainDimension,domainDimension);  

    real h = sqrt(eps*10.);   // for computing yrr ****
    real hInverse=1./h;
//    bool useRobustInverse=false; // *****
    real deltaR=0.;
    for( axis=0; axis<domainDimension; axis++ )
      deltaR=max(deltaR,1./(map->getGridDimensions(axis)-1));
    deltaR*=.5;

    for( int iter=1; iter<=maxit; iter++ )
    {
      numberOfNewtonSteps+=(bound-base+1);

      if( useRobustExactLocalInverse && domainDimension==1 )
      {
	// peform a bisection type algorithm
        r2(I,Axes)=r(I,Axes);  // save old value
        dr(I,Axes)=1./(map->getGridDimensions(axis1)-1);
	minimizeByBisection(r,x,dr,status,eps );
      }
      else
      {
        #ifndef USE_PPP
          map->map( r,y,yr );
        #else
          map->mapS( r,y,yr );
        #endif
	if( domainDimension == 1 )
	{ // compute second derivatives by differences
	  r2(I,Axes)=r(I,Axes)+h;

          #ifndef USE_PPP
	    map->map( r2(I,Axes),Overture::nullRealArray(),yr2 );
          #else
  	    map->mapS( r2(I,Axes),Overture::nullRealArray(),yr2 );
          #endif

	  yrr(I,xAxes,axis1,axis1)=(yr2(I,xAxes,axis1)-yr(I,xAxes,axis1))*hInverse;
	}
	else
	{  
          if( iter==1 ) // only compute second derivatives first time. We only need a first order approximation.
	  {
	    for( int axis=axis1; axis<=axis2; axis++ )
	    { // compute second derivatives by differences
	      r2(I,Axes)=r(I,Axes);
	      r2(I,axis)+=h;

              #ifndef USE_PPP
	        map->map( r2(I,Axes),Overture::nullRealArray(),yr2 );
              #else
  	        map->mapS( r2(I,Axes),Overture::nullRealArray(),yr2 );
              #endif

	      yrr(I,xAxes,axis1,axis)=(yr2(I,xAxes,axis1)-yr(I,xAxes,axis1))*hInverse;
	      yrr(I,xAxes,axis2,axis)=(yr2(I,xAxes,axis2)-yr(I,xAxes,axis2))*hInverse;
	    }
	  }
/* --------
          else if( iter==6 ) 
	  {
            // 2nd derivative may be discontinuous -- set to zero if we are taking too many iterations
	    for( int axis=axis1; axis<=axis2; axis++ )
	    {
	      yrr(I,xAxes,axis1,axis)=0.;
	      yrr(I,xAxes,axis2,axis)=0.;
	    }
	  }
---------- */
	}
	dy(I,xAxes)=x(I,xAxes)-y(I,xAxes);

	if( Mapping::debug & 16 )
	{
	  if( domainDimension==2 )
	    cout << " r = (" << r(base,axis1) << "," << r(base,axis2) << "), ";
	  else if( domainDimension==1 )
	    cout << " r = (" << r(base,axis1) << "), ";
	  if( rangeDimension==2 )
	  {
	    cout << " y,x,dy = (" 
		 <<  " (" << y(base,axis1) << "," << y(base,axis2) << "), "
		 <<  " (" << x(base,axis1) << "," << x(base,axis2) << "), "
		 <<  " (" <<dy(base,axis1) << "," <<dy(base,axis2) << "), " << endl;
	  }
	}
      
	invertL2(yr,dy,det,yr2,yrr,ry,dr,status);   // get dr

	// dr(I,Axes)=max(-.025,min(.025,dr(I,Axes)));   // limit correction

	r2(I,Axes)=r(I,Axes);  // save old value

//	if( domainDimension>1 || (max(dr(I,Axes))< deltaR &&  iter<maxit-2) ) // *wdh* 971023
	if( domainDimension>1 || (max(fabs(dr(I,Axes)))< deltaR &&  iter<iterationToStartDamping) )
	{
	  r(I,Axes)+=dr(I,Axes);
	}
	else
	{
	  if( domainDimension==1 )
	  {
	    // peform a bisection type algorithm for slowly converging cases
	    // it could be that the 1st or 2nd derivative are not continuous
	    minimizeByBisection(r,x,dr,status,eps );
	  }
	  else
	  {
	    // damp the correction:
	    where( fabs(dr(I,Axes))>goodEnoughEps*100. )
	      dr(I,Axes)*=.5;        // damp the iteration (need if the 2nd derivative is discontinuous)
            r(I,Axes)+=dr(I,Axes);
	  }
	}
      }
      
      periodicShift( r,I );   // shift r into [0,1] if the mapping is functionPeriodic

      // compute actual change in r, taking into account getting stuck at the end points and periodicity

      r(I,Axes)=max(r(I,Axes),-.05);   // allow answer to be slightly outside wdh: 970502
      r(I,Axes)=min(r(I,Axes),1.05);

      r2(I,Axes)=min(fabs(r2(I,Axes)-r(I,Axes)),fabs(dr(I,Axes)));

      for( int i=base; i<=bound; i++)  // Check for convergence or divergence
        if( status(i)==1 && max(r2(i,Axes)) < eps ) 
	  status(i)=0;                  // iteration converged

      if( iter>10 || Mapping::debug & 4 )
      {
        int axs2=min(1,domainDimension-1);
	for( int i=base; i<=bound; i++)
	{
          if( max(r2(i,Axes)) > eps )
	  {
            printf("Newton(L2): iter=%i, i=%i, dr=(%e,%e), r2=(%e,%e), ",
                 iter,i,dr(i,0),dr(i,axs2),r2(i,0),r2(i,axs2));
	    if( rangeDimension==1 )
	      printf(" x=%8.2e ",x(i,axis1));
	    else if( rangeDimension==2 )
	      printf(" x=(%8.2e,%8.2e) ",x(i,axis1),x(i,axis2));
	    else 
	      printf(" x=(%8.2e,%8.2e,%8.2e) ",x(i,axis1),x(i,axis2),x(i,axis3));
	    if( rangeDimension==1 )
	      printf(" y=%8.2e ",y(i,axis1));
	    else if( rangeDimension==2 )
	      printf(" y=(%8.2e,%8.2e) ",y(i,axis1),y(i,axis2));
	    else 
	      printf(" y=(%8.2e,%8.2e,%8.2e) ",y(i,axis1),y(i,axis2),y(i,axis3));
            printf("\n");
	  }
	}
      }
      //  Check for convergence.
      if( max(status(I)) <= 0 )
      {
        break;
      }
    }
    if( goodEnoughEps>eps )
    {
      // **double precision** Allow a lesser convergence tolerance since the Mapping may
      // be defined with single precision numbers (plot3d file for example)
      for( int i=base; i<=bound; i++)  // Check for convergence or divergence
      {
	if( status(i)==1 )  
	{
          if( Mapping::debug & 2 )
	  {
	    printf("Mapping::Newton:WARNING: poor convergence i=%i, ",i);
	    if( domainDimension==2 )
	      printf("r=(%e,%e), dr=(%e,%e), goodEnoughEps=%e \n",
		     r(i,axis1),r(i,axis2),r2(i,axis1),r2(i,axis2),goodEnoughEps); 
	    else
	      printf("r=%e, dr=%e, goodEnoughEps=%e \n",
		     r(i,axis1),r2(i,axis1),goodEnoughEps); 
	  }
	  // if( max(fabs(r2(i,Axes))) < goodEnoughEps )   // *wdh* 990113   
          // we assume we always converge, unless we are at a "saddle-pt" in which case we have one answer
          // of more than one possible answer
	  status(i)=0;                 // iteration converged
	}
      }
    }
//    real sqrtEps=SQRT(eps);
//    for( int i=base; i<=bound; i++)
//      if( status(i)==1 && max(r2(i,Axes)) < sqrtEps ) 
//	status(i)=0;                  // iteration converged
      
    
  } // end obsolete code
  
  
  map->periodicShift(r,I);  // shift for function or derivative periodic

  // Copy r,rx -> r1,rx1 
  int base1, bound1;
  Index I1 = map->getIndex( x1,r1,rx1,base1,bound1,computeMap,computeMapDerivative );

  
  // if( computeGlobalInverse && !workSpace.index0IsSequential )
  if( !workSpace.index0IsSequential )
  {
    // Note: if we compress points then it could be computeGlobalInverse==false as well
    // if( !computeGlobalInverse )
    //   printf("***** WARNING computeGlobalInverse=false ****\n");   
    
    // if( workSpace.index0(bound)!=bound ) printf("***** WARNING: workSpace.index0(bound)!=bound\n");
    
    // printf(" base=%i, bound=%i, base1=%i, bound1=%i \n",base,bound,base1,bound1);

    if( computeMap )
    {
/* ------
      if( false )
      {
        RealArray r2,r3,r4;
        r2=r;
        r3=r;
	r4=r;
	
	where( status(I)>=0 )  // *wdh* 990129
	{
	  for( int axis=axis1; axis<domainDimension; axis++ )
	    r1(workSpace.index0(I),axis)= r3(I,axis);          // ***** bug here in A++ if I.base !=0 ******
	}
	elsewhere( status(I)<0 )
	{
	  for( int axis=axis1; axis<domainDimension; axis++ )
	    r1(workSpace.index0(I),axis)= ApproximateGlobalInverse::bogus; 
	}
        // r1=r4;
	
	for( int i=base; i<=bound; i++ )
	{
	  if( status(i)>=0 )
	    r2(workSpace.index0(i),Axes)= r3(i,Axes);       
          else if( status(i)<0 )
            r2(workSpace.index0(i),Axes)= ApproximateGlobalInverse::bogus; 
	}
        if( max(fabs(r1-r2)) >0. )
	{
	  printf(" >>>>>>>>>>>>>>>>>>> Error in r2 is %e \n",max(fabs(r1-r2)));
          I.display("I");
          status.display("status");
	  workSpace.index0.display("workSpace.index0");
	  workSpace.index0(I).display("workSpace.index0(I)");
          r1.display("r1");
	  r2.display("r2");
	  r3.display("r3 is the original r");
	  
	  throw "error";
	}
        r1=r2;
	
      }
-------------- */
      if( true )
      {
        // bug here is r1 is a view of an array with a non-zero base -- see bug31.C
        Range I1=r1.dimension(0);
        RealArray r02(I1,Axes);
        r02=r1(I1,Axes);
	where( status(I)>=0 )  // *wdh* 990129
	{
	  for( int axis=axis1; axis<domainDimension; axis++ )
	    r02(workSpace.index0(I),axis)= r(I,axis);          // ***** bug here in A++ if I.base !=0 ******
	}
	// elsewhere  // *wdh* 990129
	elsewhere( status(I)<0 )
	{
	  for( int axis=axis1; axis<domainDimension; axis++ )
	    r02(workSpace.index0(I),axis)= ApproximateGlobalInverse::bogus; 
	}
        r1(I1,Axes)=r02;
      }
      /* ---
      else
      {
	where( status(I)>=0 )  // *wdh* 990129
	{
	  for( int axis=axis1; axis<domainDimension; axis++ )
	    r1(workSpace.index0(I),axis)= r(I,axis);          // ***** bug here in A++ if I.base !=0 ******
	}
	// elsewhere  // *wdh* 990129
	elsewhere( status(I)<0 )
	{
	  for( int axis=axis1; axis<domainDimension; axis++ )
	    r1(workSpace.index0(I),axis)= ApproximateGlobalInverse::bogus; 
	}
      }
      -- */
    }
    if( computeMapDerivative )  
      if( domainDimension==rangeDimension )
      {
        where( status(I)==0 )
        {
	  for( int d=0; d<domainDimension; d++ )
	    for( int r=0; r<domainDimension; r++ )
	      rx1(workSpace.index0(I),d,r)=ry(I,d,r)/det(I); 
	}
      }
      else
        rx1(I1,Axes,xAxes)=0.;  // what to return in this case??
  }
  else // workspace is sequential
  {
    if( computeMap )
    {
      where( status(I)>=0 )  // *wdh* 990129
      {
	for( int axis=axis1; axis<domainDimension; axis++ )
	  r1(I,axis)= r(I,axis);     
      }
      // elsewhere  // *wdh* 990129
      elsewhere( status(I)<0 )
      {
	for( int axis=axis1; axis<domainDimension; axis++ )
	  r1(I,axis)= ApproximateGlobalInverse::bogus;  
      }
    }
    if( computeMapDerivative )  
    {
      if( domainDimension==rangeDimension )
      {
        where( status(I)==0 )
	{
          for( int r=axis1; r<rangeDimension; r++ )
            for( int d=axis1; d<domainDimension; d++ )
	      rx1(I,d,r)=ry(I,d,r)/det(I); 
	}
      }
      else
        rx1(I,Axes,xAxes)=0.;  // what to return in this case??
    }
  }

  // check for points that didn't converge
  for( i=base; i<=bound; i++ )
  {
    if( numberOfNoConvergenceMessages<maximumNumberOfNoConvergenceMessages && status(i)==1 )
    {
      numberOfNoConvergenceMessages++;
      printf("Mapping::Newton Inverse: WARNING - no convergence for ");
      if( rangeDimension==1 )
	printf(" x=%7.3e ",x(i,axis1));
      else if( rangeDimension==2 )
	printf(" x=(%7.3e,%7.3e) ",x(i,axis1),x(i,axis2));
      else 
	printf(" x=(%7.3e,%7.3e,%7.3e) ",x(i,axis1),x(i,axis2),x(i,axis3));
      if( domainDimension==1 )
	printf(" r=%7.3e, dr=%7.3e, eps=%7.3e, ",r(i,axis1),dr(i,axis1),eps);             
      else if( domainDimension==2 )
	printf(" r=(%7.3e,%7.3e), dr=(%7.3e,%7.3e), eps=%7.3e, ",r(i,axis1),r(i,axis2),
	       dr(i,axis1),dr(i,axis2),eps);             
      else 
	printf(" r=(%7.3e,%7.3e,%7.3e), dr=(%7.3e,%7.3e,%7.3e), eps=%7.3e,",r(i,axis1),r(i,axis2),r(i,axis3),
	       dr(i,axis1),dr(i,axis2),dr(i,axis3),eps);             
      printf(" maxit=%i, mapping=%s \n",maxit,(const char*)map->getName(Mapping::mappingName));
      if( numberOfNoConvergenceMessages==(maximumNumberOfNoConvergenceMessages-1) )
      {
	printf("\n **** Mapping::Newton Inverse: too many `no convergence' warnings. "
               "I will not print any-more ***\n\n");
	
      }
	  
    }
  }

  recursionLevel--;   // keeps track of how deeply nested we are
  workSpace.index0IsSequential=true;
  
  timeForExactInverse+=getCPU()-time0;
}


void ExactLocalInverse:: 
minimizeByBisection(RealArray & r, RealArray & x, RealArray & dr, IntegerArray & status, real & eps )
{
  // printf(" **** minimizeByBisection *** \n");
  // domainDimension==1 and rangeDimension>1 only
  assert( domainDimension==1 );

  real dL,dM,dR;
  
  RealArray rM(1,1),rL(1,1),rR(1,1),xD(1,3),rr(11,1),xx(11,3);
  real distance,minDistance;
  int j,jMin=-1;

  for( int i=base; i<=bound; i++)  // Check for convergence or divergence
  {
    if( status(i)==1 ) // not converged
    {
      // first find an interval  rL < rM < rR such that the distance at rM is less
      // than the distance at rL and at rR, the minimum distance is then assumed
      // to lie in [rL,rR]
      real delta;
      rM(0,0)=r(i,0);
      #ifndef USE_PPP
        map->map( rM,xD );
      #else
        map->mapS( rM,xD );
      #endif
      dM=rangeDimension==2 ? DISTANCE2(xD) : rangeDimension==3 ? DISTANCE3(xD) : DISTANCE1(xD);
      

      for( j=1; j<10; j+=2 )
      {
	// first time check nearby points to see if we already have an answer.
        // *wdh* 990215 delta= j==1 ? .9*eps : dr(i,0)*(j-1)*.5; 
	delta= j==1 ? .9*eps : fabs(dr(i,0))*(j-1)*.5; 

	rL=rM-delta;
        #ifndef USE_PPP
          map->map( rL,xD );
        #else
          map->mapS( rL,xD );
        #endif
	dL=rangeDimension==2 ? DISTANCE2(xD) : rangeDimension==3 ? DISTANCE3(xD) : DISTANCE1(xD);

	rR=rM+delta;
        #ifndef USE_PPP
          map->map( rR,xD );
        #else
          map->mapS( rR,xD );
        #endif
	dR=rangeDimension==2 ? DISTANCE2(xD) : rangeDimension==3 ? DISTANCE3(xD) : DISTANCE1(xD);
      
	if( dM <= dL && dM <= dR )
          break;
      }
      if( delta<eps )
      {
        if( Mapping::debug & 4 )
  	  printf("Bisection: Initial point is at a minimum distance. dL=%e, dM=%e, dR=%e eps=%e\n",dL,dM,dR,eps);
	 // converged
      }
      else
      {
	if( dM > dL || dM > dR )
	{
	  printf(" minimizeByBisection:WARNING dM is not in the middle. dL=%e, dM=%e, dR=%e \n",dL,dM,dR);
	}
      
        const int maxIt=25;
	real dn,rMin,r0,r1,rr2;
	for( int it=0; it<maxIt; it++ )
	{
	  //real rL0=rL(0,0), rR0=rR(0,0);
	  //for( j=0; j<=10; j++ )
	  //  rr(j,0)=rL0*(1.-.1*j) + rR0*( j*.1 );

	  if( true )
	  {
	    r0=rL(0,0); r1=rM(0,0); rr2=rR(0,0);
	    dn = (rr2-r1)*dL -(rr2-r0)*dM+(r1-r0)*dR;
	    if( dn!=0. )
	    {
	      rMin=.5*( (rr2*rr2-r1*r1)*dL-(rr2*rr2-r0*r0)*dM+(r1*r1-r0*r0)*dR )/dn;
	      if( rMin<r0 || rMin>rr2 )
		rMin=r1;
	    }
	    else
	      rMin=r1;
	    if( Mapping::debug & 4 )
	      printf("bisection: r0=%e, r1=%e, rr2=%e, rMin=%e, delta=%e \n",r0,r1,rr2,rMin,delta);

	    // cluster points near the estimated minimum
	    rr(0,0)=r0;
	    rr(1,0)=.5*(rMin+r0);
	    rr(2,0)=.7 *rMin+.3 *r0;
	    rr(3,0)=.8 *rMin+.2 *r0;
	    rr(4,0)=.9*rMin +.1*r0;
	    rr(5,0)=rMin;
	    rr(6,0)=.9*rMin+.1*rr2;     // rMin+delta*delta*2.;
	    rr(7,0)=.8 *rMin+.2 *rr2;
	    rr(8,0)=.7 *rMin+.3 *rr2;
	    rr(9,0)=.5*(rM(0,0)+rr2);
	    rr(10,0)=rr2;

	  }
	
          #ifndef USE_PPP
	    map->map( rr,xx );
          #else
	    map->mapS( rr,xx );
          #endif
	  // now find point with minimum distance
	  minDistance=REAL_MAX;
	  for( j=1; j<10; j++ )  
	  {
	    distance= rangeDimension==2 ? SQR( xx(j,0)-x(i,0) ) + SQR( xx(j,1)-x(i,1) )
	      : rangeDimension==3 ? SQR( xx(j,0)-x(i,0) ) + SQR( xx(j,1)-x(i,1) ) + SQR( xx(j,2)-x(i,2) ) :
	      fabs( xx(j,0)-x(i,0) )  ;
	  
	    if( distance<minDistance )
	    {
	      minDistance=distance;
	      jMin=j;
	    }
	  }
	  assert( jMin>0 && jMin<10 );

	  rL=rr(jMin-1,0);
	  rM=rr(jMin  ,0);
	  rR=rr(jMin+1,0);
      
	  if( Mapping::debug & 4 )
	    printf("minimize by bisection: r=%e, x=(%e,%e), minDistance=%e, it=%i, eps=%e \n",
		   rM(0,0),x(i,0),x(i,1),minDistance,it,eps);

	  delta=max(rM(0,0)-rL(0,0),rR(0,0)-rM(0,0));   // /=10.;
	  if( delta<eps )
	    break;
	}
      }
      
      r(i,0)=rM(0,0);
      dr(i,0)=delta;
      status(i)=0;  // converged
      
    }
  }
}



//=======================================================================================
//      periodicShift
//   *** shift if functionPeriodic ****
//=======================================================================================
inline void ExactLocalInverse::
periodicShift( RealArray & r, const Index & I )
{
  for( int axis=axis1; axis < domainDimension; axis++ )
    if( map->getIsPeriodic(axis) == Mapping::functionPeriodic )
    {
      r(I,axis)=fmod(r(I,axis)+1.,1.);  // map back to [0,1]
      // r(I,axis)-=floor2(r(I,axis));   // shift y into the interval [0.,1]
    }
}

//===================================================================================
//    invert
//
//  Invert the Jacobian matrix and compute the correction
//
// Input -
//  status(i) =  0 : Newton has already converged
//            =  1 : Newton still converging
//            = -1 : Newton didn't converge
//====================================================================================

inline void ExactLocalInverse::
invert(RealArray & yr, RealArray & dy, RealArray & det, RealArray & ry, RealArray & dr, IntegerArray & status)
{ // Input yr,dy
  // Output : ry = yr^(-1)*det   (unnormalized inverse)
  //          dr = yr^(-1)*dy
  //
//   #ifdef USE_PPP
//     bool useOpt=true;  // for now just use new version in parallel
//   #else
//     bool useOpt=false;
//   #endif

  bool useOpt=true;
  if( useOpt )
  {
    real *yrp = yr.Array_Descriptor.Array_View_Pointer2;
    const int yrDim0=yr.getRawDataSize(0);
    const int yrDim1=yr.getRawDataSize(1);
    #undef YR
    #define YR(i0,i1,i2) yrp[i0+yrDim0*(i1+yrDim1*(i2))]

    real *ryp = ry.Array_Descriptor.Array_View_Pointer2;
    const int ryDim0=ry.getRawDataSize(0);
    const int ryDim1=ry.getRawDataSize(1);
    #undef RY
    #define RY(i0,i1,i2) ryp[i0+ryDim0*(i1+ryDim1*(i2))]

    real *drp = dr.Array_Descriptor.Array_View_Pointer1;
    const int drDim0=dr.getRawDataSize(0);
    #undef DR
    #define DR(i0,i1) drp[i0+drDim0*(i1)]

    real *dyp = dy.Array_Descriptor.Array_View_Pointer1;
    const int dyDim0=dy.getRawDataSize(0);
    #undef DY
    #define DY(i0,i1) dyp[i0+dyDim0*(i1)]

    real *detp = det.Array_Descriptor.Array_View_Pointer0;
    #undef DET
    #define DET(i0) detp[i0]


    if( domainDimension==1 )
    {
      for( int i=base; i<=bound; i++ )
      {
	if( status(i)==1 )  // only do points that are still converging
	{
	  DET(i)=1.;
	  RY(i,axis1,axis1)=1./YR(i,axis1,axis1);
	  DR(i,axis1)=RY(i,axis1,axis1)*DY(i,axis1);
	}
	else
	{
	  DR(i,axis1)=0.;
	}
      }
    }
    else if( domainDimension==2 )
    {
      for( int i=base; i<=bound; i++ )
      {
	if( status(i)==1 )  // only do points that are still converging
	{
	  DET(i)=YR(i,axis1,axis1)*YR(i,axis2,axis2)-YR(i,axis1,axis2)*YR(i,axis2,axis1);
	  RY(i,axis1,axis1)= YR(i,axis2,axis2);
	  RY(i,axis1,axis2)=-YR(i,axis1,axis2);
	  RY(i,axis2,axis1)=-YR(i,axis2,axis1);
	  RY(i,axis2,axis2)= YR(i,axis1,axis1);

	  DR(i,axis1)=RY(i,axis1,axis1)*DY(i,axis1)+RY(i,axis1,axis2)*DY(i,axis2);
	  DR(i,axis2)=RY(i,axis2,axis1)*DY(i,axis1)+RY(i,axis2,axis2)*DY(i,axis2);
	}
	else
	{
	  DR(i,axis1)=0.;
	  DR(i,axis2)=0.;
	}
      }
      //  If there is a coordinate singularity call underdetermined least squares inverter
      if( mappingHasACoordinateSingularity )
      {
	for( int i=base; i<=bound; i++ )
	{
	  if( status(i)==1 &&  max(fabs(DR(i,axis1)),fabs(DR(i,axis2))) >= fabs(DET(i)) )
	  {
	    //call least squares solver
	    if( Mapping::debug & 16 )
	      cout << "Newton: call (underdetermined) least squares solver..." << endl;
	    underdeterminedLS( yr(i,xAxes,Axes), ry(i,Axes,xAxes), dy(i,xAxes), dr(i,Axes), det(i) );
	    DET(i)=1.;
	  }
	}
      }
      // keep dr from being too big
      for( int i=base; i<=bound; i++ )
      {
	if( status(i)==1 )
	{
	  DR(i,axis1)=min(drMax[axis1],max(-drMax[axis1],DR(i,axis1)/DET(i)));
	  DR(i,axis2)=min(drMax[axis2],max(-drMax[axis2],DR(i,axis2)/DET(i)));
	}
      }
      
    }
    else if( domainDimension==3 )
    {
      for( int i=base; i<=bound; i++ )
      {
	if( status(i)==1 )  // only do points that are still converging
	{
	  DET(i)=
	    (YR(i,axis1,axis2)*YR(i,axis2,axis3)-YR(i,axis2,axis2)*YR(i,axis1,axis3))*YR(i,axis3,axis1)
	    +(YR(i,axis1,axis3)*YR(i,axis2,axis1)-YR(i,axis2,axis3)*YR(i,axis1,axis1))*YR(i,axis3,axis2)
	    +(YR(i,axis1,axis1)*YR(i,axis2,axis2)-YR(i,axis2,axis1)*YR(i,axis1,axis2))*YR(i,axis3,axis3);
	  // or should these values just be scalars:
	  RY(i,axis1,axis1)=YR(i,axis2,axis2)*YR(i,axis3,axis3)-YR(i,axis2,axis3)*YR(i,axis3,axis2);
	  RY(i,axis2,axis1)=YR(i,axis2,axis3)*YR(i,axis3,axis1)-YR(i,axis2,axis1)*YR(i,axis3,axis3);
	  RY(i,axis3,axis1)=YR(i,axis2,axis1)*YR(i,axis3,axis2)-YR(i,axis2,axis2)*YR(i,axis3,axis1);
	  RY(i,axis1,axis2)=YR(i,axis3,axis2)*YR(i,axis1,axis3)-YR(i,axis3,axis3)*YR(i,axis1,axis2);
	  RY(i,axis2,axis2)=YR(i,axis3,axis3)*YR(i,axis1,axis1)-YR(i,axis3,axis1)*YR(i,axis1,axis3);
	  RY(i,axis3,axis2)=YR(i,axis3,axis1)*YR(i,axis1,axis2)-YR(i,axis3,axis2)*YR(i,axis1,axis1);
	  RY(i,axis1,axis3)=YR(i,axis1,axis2)*YR(i,axis2,axis3)-YR(i,axis1,axis3)*YR(i,axis2,axis2);
	  RY(i,axis2,axis3)=YR(i,axis1,axis3)*YR(i,axis2,axis1)-YR(i,axis1,axis1)*YR(i,axis2,axis3);
	  RY(i,axis3,axis3)=YR(i,axis1,axis1)*YR(i,axis2,axis2)-YR(i,axis1,axis2)*YR(i,axis2,axis1);
	  DR(i,axis1)=RY(i,axis1,axis1)*DY(i,axis1)+RY(i,axis1,axis2)*DY(i,axis2)
	    +RY(i,axis1,axis3)*DY(i,axis3);
	  DR(i,axis2)=RY(i,axis2,axis1)*DY(i,axis1)+RY(i,axis2,axis2)*DY(i,axis2)
	    +RY(i,axis2,axis3)*DY(i,axis3);
	  DR(i,axis3)=RY(i,axis3,axis1)*DY(i,axis1)+RY(i,axis3,axis2)*DY(i,axis2)
	    +RY(i,axis3,axis3)*DY(i,axis3);
	}
	else
	{
	  DR(i,axis1)=0.;
	  DR(i,axis2)=0.;
	  DR(i,axis3)=0.;
	}
      }
      
      //  If there is a coordinate singularity call underdetermined least squares inverter
      if( mappingHasACoordinateSingularity )
      {
	for( int i=base; i<=bound; i++ )
	{
	  // const int singularAxis=axis2;
	  // if( status(i)==1 && fabs(r(i,singularAxis)-singularEnd) < .01 ) 
	  if( status(i)==1 && max(fabs(dr(i,Axes))) >= .25*fabs(det(i)) ) // This is not a good check.
	  {
	    if( Mapping::debug & 4 )
	    {
	      cout << "Newton: call (underdetermined) least squares solver..." << endl;
	      printf("invert: 3D: max(|dr|)=%6.2e is > .25*det=%6.2e \n",max(fabs(dr(i,Axes))),.25*fabs(DET(i)));
	    }
	    underdeterminedLS( yr(i,xAxes,Axes), ry(i,Axes,xAxes), dy(i,xAxes), dr(i,Axes), det(i) );
	    DET(i)=1.;
	  }
	}
      }
      // now divide through by the determinant
      for( int i=base; i<=bound; i++ )
      {
	if( status(i)==1 )
	{
	  DR(i,axis1)=min(drMax[axis1],max(-drMax[axis1],DR(i,axis1)/DET(i)));
	  DR(i,axis2)=min(drMax[axis2],max(-drMax[axis2],DR(i,axis2)/DET(i)));
	  DR(i,axis3)=min(drMax[axis3],max(-drMax[axis3],DR(i,axis3)/DET(i)));
	}
      }
      
    }
    else
    {
      cerr << "ExactLocalInverse: invert - ERROR domainDimension= " << domainDimension
	   << ", rangeDimension = " << rangeDimension << endl;
      exit(1);
    }
    
#undef YR
#undef RY
#undef DR
#undef DY
#undef DET

  }
  else
  {
    // old way -- trouble in parallel (?)
    Index I=Range(base,bound);

    switch( domainDimension )
    {
    case 1:   // R^1->R^1
      where( status(I)==1 )  // only do points that are still converging
      {
	det(I)=1.;
	ry(I,axis1,axis1)=1./yr(I,axis1,axis1);
	dr(I,axis1)=ry(I,axis1,axis1)*dy(I,axis1);
      }
      otherwise()
	dr(I,axis1)=0.;
      break; 
    case 2:  // R^2->R^2
      where( status(I)==1 )  // only do points that are still converging
      {
	det(I)=yr(I,axis1,axis1)*yr(I,axis2,axis2)-yr(I,axis1,axis2)*yr(I,axis2,axis1);
	ry(I,axis1,axis1)= yr(I,axis2,axis2);
	ry(I,axis1,axis2)=-yr(I,axis1,axis2);
	ry(I,axis2,axis1)=-yr(I,axis2,axis1);
	ry(I,axis2,axis2)= yr(I,axis1,axis1);

	dr(I,axis1)=ry(I,axis1,axis1)*dy(I,axis1)+ry(I,axis1,axis2)*dy(I,axis2);
	dr(I,axis2)=ry(I,axis2,axis1)*dy(I,axis1)+ry(I,axis2,axis2)*dy(I,axis2);
      }
      otherwise()
      {
	dr(I,axis1)=0.;
	dr(I,axis2)=0.;
      }
      //  If there is a coordinate singularity call underdetermined least squares inverter
      if( mappingHasACoordinateSingularity )
      {
	for( int i=base; i<=bound; i++ )
	{
	  if( status(i)==1 &&  max(fabs(dr(i,axis1)),fabs(dr(i,axis2))) >= fabs(det(i)) )
	  {
	    //call least squares solver
	    if( Mapping::debug & 16 )
	      cout << "Newton: call (underdetermined) least squares solver..." << endl;
	    underdeterminedLS( yr(i,xAxes,Axes), ry(i,Axes,xAxes), dy(i,xAxes), dr(i,Axes), det(i) );
	    det(i)=1.;
	  }
	}
      }
      // keep dr from being too big
      where( status(I)==1 )
      {
	dr(I,axis1)=min(drMax[axis1],max(-drMax[axis1],dr(I,axis1)/det(I)));
	dr(I,axis2)=min(drMax[axis2],max(-drMax[axis2],dr(I,axis2)/det(I)));
      }
      break; 

    case 3:  // R^3->R^3
      where( status(I)==1 )  // only do points that are still converging
      {
	det(I)=
	  (yr(I,axis1,axis2)*yr(I,axis2,axis3)-yr(I,axis2,axis2)*yr(I,axis1,axis3))*yr(I,axis3,axis1)
	  +(yr(I,axis1,axis3)*yr(I,axis2,axis1)-yr(I,axis2,axis3)*yr(I,axis1,axis1))*yr(I,axis3,axis2)
	  +(yr(I,axis1,axis1)*yr(I,axis2,axis2)-yr(I,axis2,axis1)*yr(I,axis1,axis2))*yr(I,axis3,axis3);
	// or should these values just be scalars:
	ry(I,axis1,axis1)=yr(I,axis2,axis2)*yr(I,axis3,axis3)-yr(I,axis2,axis3)*yr(I,axis3,axis2);
	ry(I,axis2,axis1)=yr(I,axis2,axis3)*yr(I,axis3,axis1)-yr(I,axis2,axis1)*yr(I,axis3,axis3);
	ry(I,axis3,axis1)=yr(I,axis2,axis1)*yr(I,axis3,axis2)-yr(I,axis2,axis2)*yr(I,axis3,axis1);
	ry(I,axis1,axis2)=yr(I,axis3,axis2)*yr(I,axis1,axis3)-yr(I,axis3,axis3)*yr(I,axis1,axis2);
	ry(I,axis2,axis2)=yr(I,axis3,axis3)*yr(I,axis1,axis1)-yr(I,axis3,axis1)*yr(I,axis1,axis3);
	ry(I,axis3,axis2)=yr(I,axis3,axis1)*yr(I,axis1,axis2)-yr(I,axis3,axis2)*yr(I,axis1,axis1);
	ry(I,axis1,axis3)=yr(I,axis1,axis2)*yr(I,axis2,axis3)-yr(I,axis1,axis3)*yr(I,axis2,axis2);
	ry(I,axis2,axis3)=yr(I,axis1,axis3)*yr(I,axis2,axis1)-yr(I,axis1,axis1)*yr(I,axis2,axis3);
	ry(I,axis3,axis3)=yr(I,axis1,axis1)*yr(I,axis2,axis2)-yr(I,axis1,axis2)*yr(I,axis2,axis1);
	dr(I,axis1)=ry(I,axis1,axis1)*dy(I,axis1)+ry(I,axis1,axis2)*dy(I,axis2)
	  +ry(I,axis1,axis3)*dy(I,axis3);
	dr(I,axis2)=ry(I,axis2,axis1)*dy(I,axis1)+ry(I,axis2,axis2)*dy(I,axis2)
	  +ry(I,axis2,axis3)*dy(I,axis3);
	dr(I,axis3)=ry(I,axis3,axis1)*dy(I,axis1)+ry(I,axis3,axis2)*dy(I,axis2)
	  +ry(I,axis3,axis3)*dy(I,axis3);
      }
      otherwise()
      {
	dr(I,axis1)=0.;
	dr(I,axis2)=0.;
	dr(I,axis3)=0.;
      }
      //  If there is a coordinate singularity call underdetermined least squares inverter
      if( mappingHasACoordinateSingularity )
      {
	for( int i=base; i<=bound; i++ )
	{
	  // const int singularAxis=axis2;
	  // if( status(i)==1 && fabs(r(i,singularAxis)-singularEnd) < .01 ) 
	  if( status(i)==1 && max(fabs(dr(i,Axes))) >= .25*fabs(det(i)) ) // This is not a good check.
	  {
	    if( Mapping::debug & 4 )
	    {
	      cout << "Newton: call (underdetermined) least squares solver..." << endl;
	      printf("invert: 3D: max(|dr|)=%6.2e is > .25*det=%6.2e \n",max(fabs(dr(i,Axes))),.25*fabs(det(i)));
	    }
	    underdeterminedLS( yr(i,xAxes,Axes), ry(i,Axes,xAxes), dy(i,xAxes), dr(i,Axes), det(i) );
	    det(i)=1.;
	  }
	}
      }
      // now divide through by the determinant
      where( status(I)==1 )
      {
	dr(I,axis1)=min(drMax[axis1],max(-drMax[axis1],dr(I,axis1)/det(I)));
	dr(I,axis2)=min(drMax[axis2],max(-drMax[axis2],dr(I,axis2)/det(I)));
	dr(I,axis3)=min(drMax[axis3],max(-drMax[axis3],dr(I,axis3)/det(I)));
      }
      break;
    default:
      cerr << "ExactLocalInverse: invert - ERROR domainDimension= " << domainDimension
	   << ", rangeDimension = " << rangeDimension << endl;
      exit(1);
    }
  }
  
}

/* ----
inline real 
g( const int i, const int i1, const int i2, const RealArray & yr, 
               const RealArray & dy, const RealArray & yrr)
{ // matrix for inverseL2 for R^2 -> R^3
  return
    yr(i,axis1,i1)*yr(i,axis1,i2)
   +yr(i,axis2,i1)*yr(i,axis2,i2)
   +yr(i,axis3,i1)*yr(i,axis3,i2)
   -yrr(i,axis1,i1,i2)*dy(i,axis1)
   -yrr(i,axis2,i1,i2)*dy(i,axis2)
   -yrr(i,axis3,i1,i2)*dy(i,axis3);
}
--- */

//===================================================================================
//    invertL2
//
//  Invert the Jacobian matrix and compute the correction in the case when
//  we are minimizing the L2 norm distance
//
//====================================================================================

inline void ExactLocalInverse::
invertL2(RealArray & yr, RealArray & dy, RealArray & det, RealArray & yr2, RealArray & yrr, 
         RealArray & ry, RealArray & dr, IntegerArray & status)
{ // Input yr,dy
  // Output : ry = yr^(-1)*det   (unnormalized inverse)
  //          dr = yr^(-1)*dy
  //
  int i1,i2;
//real g11,g12,g21,g22,h1,h2;
  Range I(base,bound);

  switch(  rangeDimension+10*domainDimension )
  {
  case 12:   // R^1->R^2 : 
    dr(I,axis1)=(yr(I,axis1,axis1)*dy(I,axis1)+yr(I,axis2,axis1)*dy(I,axis2))/
      (SQR(yr(I,axis1,axis1))+SQR(yr(I,axis2,axis1))
       -yrr(I,axis1,axis1,axis1)*dy(I,axis1)
       -yrr(I,axis2,axis1,axis1)*dy(I,axis2)  );

/* ----
    for( i=base; i<= bound; i++ )    // ************************** fix this ********************
    { 
      dr(i,axis1)=(yr(i,axis1,axis1)*dy(i,axis1)+yr(i,axis2,axis1)*dy(i,axis2))/
                  (pow(yr(i,axis1,axis1),2)+pow(yr(i,axis2,axis1),2)
                   -yrr(i,axis1,axis1,axis1)*dy(i,axis1)
                   -yrr(i,axis2,axis1,axis1)*dy(i,axis2)  );
      if( Mapping::debug & 16 )
	cout << "invertL2: i = " << i << ", dr = " << dr(i,axis1) 
             << ", yr=(" << yr(i,axis1,axis1) << "," << yr(i,axis2,axis1) << ")" 
             << ", yrr=(" << yrr(i,axis1,axis1) << "," << yrr(i,axis2,axis1) << ")" << endl;
	
    }
---- */
    break; 
  case 13:   // R^1->R^3 : 
    dr(I,axis1)=(yr(I,axis1,axis1)*dy(I,axis1)
		 +yr(I,axis2,axis1)*dy(I,axis2)
		 +yr(I,axis3,axis1)*dy(I,axis3))/
      (SQR(yr(I,axis1,axis1))+SQR(yr(I,axis2,axis1))+SQR(yr(I,axis3,axis1))
       -yrr(I,axis1,axis1,axis1)*dy(I,axis1)
       -yrr(I,axis2,axis1,axis1)*dy(I,axis2) 
       -yrr(I,axis3,axis1,axis1)*dy(I,axis3)  );
/* ---
    for( i=base; i<= bound; i++ )
    { 
      dr(i,axis1)=(yr(i,axis1,axis1)*dy(i,axis1)
                  +yr(i,axis2,axis1)*dy(i,axis2)
                  +yr(i,axis3,axis1)*dy(i,axis3))/
        (pow(yr(i,axis1,axis1),2)+pow(yr(i,axis2,axis1),2)+pow(yr(i,axis3,axis1),2)
                   -yrr(i,axis1,axis1,axis1)*dy(i,axis1)
                   -yrr(i,axis2,axis1,axis1)*dy(i,axis2) 
                   -yrr(i,axis3,axis1,axis1)*dy(i,axis3)  );
    }
 ----- */
    break; 

  case 23:  // R^2->R^3  : 
    // save rhs vector (h1(I),h2(I)) in yr2:
    yr2(I,2,0)=yr(I,axis1,axis1)*dy(I,axis1)   // h1
              +yr(I,axis2,axis1)*dy(I,axis2)
	      +yr(I,axis3,axis1)*dy(I,axis3);
    yr2(I,2,1)=yr(I,axis1,axis2)*dy(I,axis1)   // h2
              +yr(I,axis2,axis2)*dy(I,axis2)
	      +yr(I,axis3,axis2)*dy(I,axis3);

    // compute matrix elements and save in yr2
    // only compute g00, g01, g22  (g10==g01)
    for( i1=axis1; i1<=axis2; i1++ )
    {
      for( i2=i1; i2<=axis2; i2++ )  // note starting index
      {
	yr2(I,i1,i2)=( yr(I,axis1,i1)    *yr(I,axis1,i2)
		      +yr(I,axis2,i1)    *yr(I,axis2,i2)
		      +yr(I,axis3,i1)    *yr(I,axis3,i2)
		      -yrr(I,axis1,i1,i2)*dy(I,axis1)
		      -yrr(I,axis2,i1,i2)*dy(I,axis2)
		      -yrr(I,axis3,i1,i2)*dy(I,axis3));
      }
    }
    det(I)=1./( yr2(I,0,0)*yr2(I,1,1)-yr2(I,0,1)*yr2(I,0,1) ); // ***** check for det==0
    
    dr(I,axis1)=( yr2(I,1,1)*yr2(I,2,0)-yr2(I,0,1)*yr2(I,2,1))*det(I);
    dr(I,axis2)=(-yr2(I,0,1)*yr2(I,2,0)+yr2(I,0,0)*yr2(I,2,1))*det(I);
    
    
/* ----
    for( i=base; i<= bound; i++ )
    { 
      h1=yr(i,axis1,axis1)*dy(i,axis1)   // rhs : (h1,h2)^T
        +yr(i,axis2,axis1)*dy(i,axis2)
	+yr(i,axis3,axis1)*dy(i,axis3);
      h2=yr(i,axis1,axis2)*dy(i,axis1)
        +yr(i,axis2,axis2)*dy(i,axis2)
	+yr(i,axis3,axis2)*dy(i,axis3);
      
      g11=g(i,axis1,axis1, yr,dy,yrr );  // matrix for Newton
      g12=g(i,axis1,axis2, yr,dy,yrr );
      g21=g12;
      g22=g(i,axis2,axis2, yr,dy,yrr );
 
      det(i)=g11*g22-g12*g21;

      dr(i,axis1)=( g22*h1-g12*h2)/det(i);   // check for det=0 *****
      dr(i,axis2)=(-g21*h1+g11*h2)/det(i);

    }
----- */

    break;
  default:
    cerr << "ExactLocalInverse: invertL2 - ERROR domainDimension= " << domainDimension
         << ", rangeDimension = " << rangeDimension << endl;
    exit(1);
  }
}

//======================================================================================
//  Solve the underdetermined least Squares problem
//    This is needed at the singularity of a coordinate system (centre of a polar
//    grid, or at the pole of a sphere) when one variable (the angle) is undetermined
//
// This code is taken from Geoff's fortran subroutine, it could be cleaned up
//======================================================================================
void ExactLocalInverse::
underdeterminedLS( const RealArray & xt, 
		   const RealArray & tx,   // *** should not be const -- do for IBM compiler
		  const RealArray & dyi,
		  const RealArray & dri,  // *** should not be const -- do for IBM compiler
		  real & deti )
{
  RealArray xtk(rangeDimension);
  RealArray ata(rangeDimension,rangeDimension);
  RealArray atai(rangeDimension,rangeDimension);

  IntegerArray ip(rangeDimension);
  int i,j,k,irank,ij;
  real odet,xtk1,xtk2;
  
  const real eps = FLT_EPSILON*50.;  // tolerance for rank difficiency

  int xtb0=xt.getBase(0);
  int txb0=tx.getBase(0);
  int drib0=dri.getBase(0);
  int dyib0=dyi.getBase(0);

  int ok=true;

  // Compute column norms of xt.
  for( j=axis1; j<domainDimension; j++ )
  {
    xtk(j)=0.;
    for( i=axis1; i<rangeDimension; i++ )
    {
      xtk(j)=xtk(j)+xt(xtb0,i,j)*xt(xtb0,i,j);
      tx(txb0,j,i)=0.;
    }
    if( j==axis1 )
    {
      xtk1=xtk(j);
      xtk2=xtk(j);
    }
    else
    {
      xtk1=min(xtk1,xtk(j));
      xtk2=max(xtk2,xtk(j));
    }
  }

  if( Mapping::debug & 16 )
    cout << "Underdetermined: column norms: min = xtk1 = " 
         << xtk1 << ", max = xtk2 = " << xtk2 << endl;
  

  if( xtk2==0. )
  {
    //  Rank=0.
    ok=false;
    dri(drib0,Axes)=ApproximateGlobalInverse::bogus;    // Bogus value
  }
  else if( xtk1 < eps*xtk2 )
  {
    //  0<Rank<n.
    //  Find the non-zero columns of xt.
    irank=0; 
    for( k=axis1; k<rangeDimension; k++ ) 
    {
      if( xtk(k) > eps*xtk2 )
      {
        ip(irank++)=k;
      }
    }
    if( Mapping::debug & 16 )
    {
      printf("Underdetermined:: rank = %i, nonzero columns:",irank);
      for( int k=0; k<irank; k++ )
        printf("column=%i, ",ip(k));
      printf("\n");
    }
    
    //  Skip over the zero columns of xt and
    //  form the normal matrix A = xt' * xt.
    for( k=axis1; k<irank; k++ )  
    {
      for( j=axis1; j<irank; j++ )
      {
        ata(j,k)=0.;
        for( i=axis1; i<rangeDimension; i++ )
	{
          ata(j,k)=ata(j,k)+xt(xtb0,i,ip(j))*xt(xtb0,i,ip(k));
	}
      }
    }
    // Invert the normal matrix A using Cramer's rule.
    if( irank==1 )
    {
      atai(axis1,axis1)=1./ata(axis1,axis1);
    }
    else
    {
      // Rank=2
      deti=ata(axis1,axis1)*ata(axis2,axis2)-ata(axis1,axis2)*ata(axis2,axis1);
      if( fabs(deti) < eps*xtk2 )
      {
        ok=false;   //       return
        dri(drib0,Axes)=ApproximateGlobalInverse::bogus;    // Bogus value
      }
      else
      {
        odet=1./deti;
        atai(axis1,axis1)= ata(axis2,axis2)*odet;
        atai(axis1,axis2)=-ata(axis1,axis2)*odet;
        atai(axis2,axis1)=-ata(axis2,axis1)*odet;
        atai(axis2,axis2)= ata(axis1,axis1)*odet;
      }
    }
    //  Solve the normal equations A * tx = xt:  tx = A \ xt.
    if( ok )
    {
      for( k=axis1; k<rangeDimension; k++ )
      {
       for( j=axis1; j<irank; j++ ) 
       {
         ij=ip(j);
         tx(txb0,ij,k)=0.;
         for( i=axis1; i<irank; i++ )
 	 {
            tx(txb0,ij,k)=tx(txb0,ij,k)+atai(j,i)*xt(xtb0,k,ip(i));
  	 }
       }
      }
      k=0;
      for( i=axis1; i<domainDimension; i++)  // solve for dri
      {
	dri(drib0,i)=0.;
	for( j=axis1; j<rangeDimension; j++ )
	  dri(drib0,i)+=tx(txb0,i,j)*dyi(dyib0,j);
      }
      
    }
  }
  else
  {
    // Rank=n.
    // This should never be needed, but here it is for completeness.
    //  Solve using Cramer's rule.
    if( Mapping::debug & 4 )
      cout << "ExactLocalInverse::underdeterminedLS:WARNING LeastSquares called but full rank! \n";
    
    if( rangeDimension==1 )
    {
      tx(txb0,axis1,axis1)=1./xt(xtb0,axis1,axis1);
    }
    else if( rangeDimension==2 )
    {
      deti=xt(xtb0,axis1,axis1)*xt(xtb0,axis2,axis2)-xt(xtb0,axis2,axis1)*xt(xtb0,axis1,axis2);
      if( fabs(deti) < eps*xtk2 )
        ok=false;  // return
      else
      {
        odet=1./deti;
        tx(txb0,axis1,axis1)= xt(xtb0,axis2,axis2)*odet;
        tx(txb0,axis1,axis2)=-xt(xtb0,axis1,axis2)*odet;
        tx(txb0,axis2,axis1)=-xt(xtb0,axis2,axis1)*odet;
        tx(txb0,axis2,axis2)= xt(xtb0,axis1,axis1)*odet;
      }
    }
    else
    {
      //  n=3.
      deti=(xt(xtb0,axis1,axis2)*xt(xtb0,axis2,axis3)-xt(xtb0,axis2,axis2)*xt(xtb0,axis1,axis3))*xt(xtb0,axis3,axis1)
          +(xt(xtb0,axis1,axis3)*xt(xtb0,axis2,axis1)-xt(xtb0,axis2,axis3)*xt(xtb0,axis1,axis1))*xt(xtb0,axis3,axis2)
          +(xt(xtb0,axis1,axis1)*xt(xtb0,axis2,axis2)-xt(xtb0,axis2,axis1)*xt(xtb0,axis1,axis2))*xt(xtb0,axis3,axis3);
      if( fabs(deti) < eps*pow(xtk2,1.5) )
      {
	ok=false;     // return 
        dri(drib0,Axes)=ApproximateGlobalInverse::bogus;  // bogus value
      }
      else
      {
        odet=1./deti;
        tx(txb0,axis1,axis1)=(xt(xtb0,axis2,axis2)*xt(xtb0,axis3,axis3)-xt(xtb0,axis2,axis3)*xt(xtb0,axis3,axis2))*odet;
        tx(txb0,axis2,axis1)=(xt(xtb0,axis2,axis3)*xt(xtb0,axis3,axis1)-xt(xtb0,axis2,axis1)*xt(xtb0,axis3,axis3))*odet;
        tx(txb0,axis3,axis1)=(xt(xtb0,axis2,axis1)*xt(xtb0,axis3,axis2)-xt(xtb0,axis2,axis2)*xt(xtb0,axis3,axis1))*odet;
        tx(txb0,axis1,axis2)=(xt(xtb0,axis3,axis2)*xt(xtb0,axis1,axis3)-xt(xtb0,axis3,axis3)*xt(xtb0,axis1,axis2))*odet;
        tx(txb0,axis2,axis2)=(xt(xtb0,axis3,axis3)*xt(xtb0,axis1,axis1)-xt(xtb0,axis3,axis1)*xt(xtb0,axis1,axis3))*odet;
        tx(txb0,axis3,axis2)=(xt(xtb0,axis3,axis1)*xt(xtb0,axis1,axis2)-xt(xtb0,axis3,axis2)*xt(xtb0,axis1,axis1))*odet;
        tx(txb0,axis1,axis3)=(xt(xtb0,axis1,axis2)*xt(xtb0,axis2,axis3)-xt(xtb0,axis1,axis3)*xt(xtb0,axis2,axis2))*odet;
        tx(txb0,axis2,axis3)=(xt(xtb0,axis1,axis3)*xt(xtb0,axis2,axis1)-xt(xtb0,axis1,axis1)*xt(xtb0,axis2,axis3))*odet;
        tx(txb0,axis3,axis3)=(xt(xtb0,axis1,axis1)*xt(xtb0,axis2,axis2)-xt(xtb0,axis1,axis2)*xt(xtb0,axis2,axis1))*odet;
      }
    }
    if( ok )
    {
      for( i=axis1; i<domainDimension; i++)  // solve for dri
      {
        dri(drib0,i)=0.;
        for( j=axis1; j<rangeDimension; j++ )
          dri(drib0,i)+=tx(txb0,i,j)*dyi(dyib0,j);
      }
    }
    
  }
  deti=1.;  
}


