#include "ReductionMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include <float.h>

ReductionMapping::
ReductionMapping() : Mapping(2,2,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief  Default Constructor
//===========================================================================
{ 
  ReductionMapping::className="ReductionMapping";
  setName( Mapping::mappingName,"reductionMapping");
  originalMap=NULL;
  inActiveAxisValue[0]=0.;
  inActiveAxisValue[1]=-1.;
  inActiveAxisValue[2]=-1.;
  mappingHasChanged();
  // *wdh* 080627 basicInverseOption=canInvert;

}


ReductionMapping::
ReductionMapping(Mapping & mapToReduce, 
                 const real & inactiveAxis1Value /* =0. */,
                 const real & inactiveAxis2Value /* =-1. */,
                 const real & inactiveAxis3Value /* =-1. */ )
: Mapping(2,2,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief  Create a reduction mapping.
/// 
/// \param mapToReduce (input): reduce the domain dimension of this mapping.
/// \param inactiveAxis1Value (input): if this value is between [0,1] then the r value
///       for axis1 will be fixed to this value and axis1 will become an in-active axis;
///        otherwise axis1 will remain active.
/// \param inactiveAxis2Value (input): fix an r value for axis2. See comments for inactiveAxis1Value.
/// \param inactiveAxis3Value (input): fix an r value for axis3. See comments for inactiveAxis1Value.
/// 
//===========================================================================
{ 
  ReductionMapping::className="ReductionMapping";
  setName( Mapping::mappingName,"reductionMapping");
  originalMap=&mapToReduce;
  if( !originalMap->uncountedReferencesMayExist() )
    originalMap->incrementReferenceCount();  // *wdh* 052601
  // *wdh* 080627 basicInverseOption=canInvert;
  setInActiveAxes( inactiveAxis1Value,inactiveAxis2Value,inactiveAxis3Value );
  mappingHasChanged();
}

ReductionMapping::
ReductionMapping(Mapping & mapToReduce, 
                 const int & inactiveAxis,
                 const real & inactiveAxisValue )
: Mapping(2,2,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief  Create a reduction mapping.
/// 
/// \param mapToReduce (input): reduce the domain dimension of this mapping.
/// \param inactiveAxis (input): This is the inactive axis.
/// \param inactiveAxisValue (input): This is the value of the inactive axis in [0,1].
/// 
//===========================================================================
{ 
  ReductionMapping::className="ReductionMapping";
  setName( Mapping::mappingName,"reductionMapping");
  originalMap=&mapToReduce;
  if( !originalMap->uncountedReferencesMayExist() )
    originalMap->incrementReferenceCount();  // *wdh* 052601
  
  real value[3] = {-1.,-1.,-1.};
  assert( inactiveAxis>=0 && inactiveAxis<mapToReduce.getDomainDimension() );
  value[inactiveAxis]=inactiveAxisValue;
  // *wdh* 080627 basicInverseOption=canInvert;
  setInActiveAxes( value[0],value[1],value[2] );
  mappingHasChanged();
}

int ReductionMapping:: 
set(Mapping & mapToReduce, 
    const real & inactiveAxis1Value /* =0.  */ ,
    const real & inactiveAxis2Value /* =-1. */,
    const real & inactiveAxis3Value /* =-1. */ )
//===========================================================================
/// \brief  Set parameters for a reduction mapping.
/// 
/// \param mapToReduce (input): reduce the domain dimension of this mapping.
/// \param inactiveAxis1Value (input): if this value is between [0,1] then the r value
///       for axis1 will be fixed to this value and axis1 will become an in-active axis;
///        otherwise axis1 will remain active.
/// \param inactiveAxis2Value (input): fix an r value for axis2. See comments for inactiveAxis1Value.
/// \param inactiveAxis3Value (input): fix an r value for axis3. See comments for inactiveAxis1Value.
/// 
//===========================================================================
{
  if( originalMap!=NULL && originalMap->decrementReferenceCount()==0 )
  {
    delete originalMap;
  }
  originalMap=&mapToReduce;
  if( !originalMap->uncountedReferencesMayExist() )
    originalMap->incrementReferenceCount();  // *wdh* 052601
  
  setInActiveAxes( inactiveAxis1Value,inactiveAxis2Value,inactiveAxis3Value );
  mappingHasChanged();
  return 0;
}

int ReductionMapping:: 
set(Mapping & mapToReduce,
    const int & inactiveAxis,
    const real & inactiveAxisValue ) 
//===========================================================================
/// \brief  Set parameters for a reduction mapping.
/// 
/// \param mapToReduce (input): reduce the domain dimension of this mapping.
/// \param inactiveAxis (input): This is the inactive axis.
/// \param inactiveAxisValue (input): This is the value of the inactive axis in [0,1].
/// 
//===========================================================================
{
  if( originalMap!=NULL && originalMap->decrementReferenceCount()==0 )
  {
    delete originalMap;
  }
  originalMap=&mapToReduce;
  if( !originalMap->uncountedReferencesMayExist() )
    originalMap->incrementReferenceCount();  // *wdh* 052601
  
  real value[3] = {-1.,-1.,-1.};
  assert( inactiveAxis>=0 && inactiveAxis<mapToReduce.getDomainDimension() );
  value[inactiveAxis]=inactiveAxisValue;

  setInActiveAxes( value[0],value[1],value[2] );

  mappingHasChanged();
  return 0;
}

int ReductionMapping::
setInActiveAxes( const real & inactiveAxis1Value /* =0. */,
                 const real & inactiveAxis2Value /* =-1. */,
                 const real & inactiveAxis3Value /* =-1. */ )
//===========================================================================
/// \brief  Specify the in-active axes.
/// 
/// \param inactiveAxis1Value (input): if this value is between [0,1] then the r value
///       for axis1 will be fixed to this value and axis1 will become an in-active axis;
///        otherwise axis1 will remain active.
/// \param inactiveAxis2Value (input): fix an r value for axis2. See comments for inactiveAxis1Value.
/// \param inactiveAxis3Value (input): fix an r value for axis3. See comments for inactiveAxis1Value.
//===========================================================================
{
  real value[3] = { inactiveAxis1Value,inactiveAxis2Value,inactiveAxis3Value };
  return setInActiveAxes( value );    
}

int ReductionMapping::
setInActiveAxes(const int & inactiveAxis,
                const real & inactiveAxisValue ) 
//===========================================================================
/// \brief  Set parameters for a reduction mapping.
/// 
/// \param inactiveAxis (input): This is the inactive axis.
/// \param inactiveAxisValue (input): This is the value of the inactive axis in [0,1].
/// 
//===========================================================================
{
  assert( inactiveAxis>=0 && inactiveAxis<getDomainDimension() );
  real value[3] = {-1.,-1.,-1.};
  value[inactiveAxis]=inactiveAxisValue;

  setInActiveAxes( value[0],value[1],value[2] );
  mappingHasChanged();
  return 0;
}


int ReductionMapping::
setInActiveAxes( const real value[3] )
// private routine to set the inactive axes.
// this routine will also call setMappingProperties()
{
  assert( originalMap!=NULL );

  int axis;

  for( axis=0; axis<originalMap->getDomainDimension(); axis++ )
    {
      inActiveAxis[axis] = -1;
      activeAxis[axis] = -1;
    }

  numberOfInActiveAxes=0;
  int numberOfActiveAxes=0;
  for( axis=0; axis<originalMap->getDomainDimension(); axis++ )
  {
    if( value[axis]>=0. && value[axis]<=1. )
    {
      inActiveAxis[numberOfInActiveAxes]=axis;
      inActiveAxisValue[numberOfInActiveAxes]=value[axis];
      numberOfInActiveAxes++;
    }
    else
    {
      activeAxis[numberOfActiveAxes]=axis;
      numberOfActiveAxes++;
    }
  }
  setMappingProperties();
  return 0;
}

    
int ReductionMapping::
setMappingProperties()
// private routine to set properties of the mapping.
{
  assert( originalMap!=NULL );
  
  setDomainDimension(originalMap->getDomainDimension()-numberOfInActiveAxes);
  setRangeDimension(originalMap->getRangeDimension());
  for( int axis=0; axis<domainDimension; axis++ )
  {
    setGridDimensions(axis,originalMap->getGridDimensions(activeAxis[axis]));
    setIsPeriodic(axis,originalMap->getIsPeriodic(activeAxis[axis]));
    for( int side=Start; side<=End; side++ )
    {
      setBoundaryCondition(side,axis,originalMap->getBoundaryCondition(side,activeAxis[axis]));

      setShare(side,axis,originalMap->getShare(side,activeAxis[axis]));
      setTypeOfCoordinateSingularity(side,axis,getTypeOfCoordinateSingularity(side,axis));
    }
  }
  // *wdh* 070301 --
  // 
  //  If the ReductionMapping corresponds to one end-face of the original mapping,
  //  set the boundaryCondition and share flag on the "extra" axis (axis=domainDimension)
  // to match the values from the inactive-axis on originalMap,
  // 
  //  This share value can be used for surface grids in order to check whether the grids share the
  //  same surface.
  if( numberOfInActiveAxes==1 )
  {
    const real eps = 5.*REAL_EPSILON;
    const int dir=inActiveAxis[0];
    if( fabs(inActiveAxisValue[0])<eps || fabs(1.-inActiveAxisValue[0])<eps )
    {
      const int originalSide= fabs(inActiveAxisValue[0])<eps ? 0 : 1;
      for( int side=Start; side<=End; side++ )
      {
	setBoundaryCondition(side,domainDimension,originalMap->getBoundaryCondition(originalSide,dir));
	setShare(side,domainDimension,originalMap->getShare(originalSide,dir));
      }
    }
  }
  
  // *wdh* 2011/10/01 : The DPM has a distributed map
  // printf(" ReductionMapping: domainDimension=%i, rangeDimension=%i\n",domainDimension,rangeDimension);
  if( false )
  {
    printF(" ReductionMapping: originalMap=%s originalMap->usesDistributedMap()=%i\n",
	   (const char*)originalMap->getName(mappingName),(int)originalMap->usesDistributedMap());
  }
  
  inverseIsDistributed = originalMap->usesDistributedInverse();
  mapIsDistributed = originalMap->usesDistributedMap();
  
  return 0;
}


// Copy constructor is deep by default
ReductionMapping::
ReductionMapping( const ReductionMapping & map, const CopyType copyType )
{
  // kkc 011001 initialize basic class information following default constructor first
  ReductionMapping::className="ReductionMapping";
  setName( Mapping::mappingName,"reductionMapping");
  originalMap=NULL;
  inActiveAxisValue[0]=0.;
  inActiveAxisValue[1]=-1.;
  inActiveAxisValue[2]=-1.;

  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "ReductionMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

ReductionMapping::
~ReductionMapping()
{ 
  if( debug & 4 )
    cout << " ReductionMapping::Destructor called" << endl;

  if( originalMap->uncountedReferencesMayExist() )
    return;
  
  if( originalMap!=NULL && originalMap->decrementReferenceCount()==0 )
  {
    delete originalMap;
  }
  

}

ReductionMapping & ReductionMapping::
operator=( const ReductionMapping & X )
{
  if( ReductionMapping::className != X.getClassName() )
  {
    cout << "ReductionMapping::operator= ERROR trying to set a ReductionMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  if( originalMap!=NULL && originalMap->decrementReferenceCount()==0 )
  {
    delete originalMap;
  }
  originalMap=X.originalMap;
  if( originalMap!=NULL )
    originalMap->incrementReferenceCount();
  
  numberOfInActiveAxes=X.numberOfInActiveAxes;
  for( int axis=0; axis<3; axis++ )
  {
    activeAxis[axis]=X.activeAxis[axis];
    inActiveAxis[axis]=X.inActiveAxis[axis];
    inActiveAxisValue[axis]=X.inActiveAxisValue[axis];          // The fixed r-value for the inActiveAxis
  }
  
  return *this;
}

void ReductionMapping::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
{
#ifndef USE_PPP
  mapS(r,x,xr,params);
#else
  Overture::abort("finish this for parallel");
#endif
}


void ReductionMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params )
{
  if( originalMap==NULL )
  {
    cout << "ReductionMapping::map: Error: The mapping to reduce has not been defined yet!\n";
    exit(1);    
  }

  if( params.coordinateType != cartesian )
    cerr << "ReductionMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  Range R(0,rangeDimension-1);
  RealArray r2,xr2;
  const int originalDomainDimension=originalMap->getDomainDimension();
  assert( originalDomainDimension>=0 && originalDomainDimension<=3 );
  
  r2.redim(r.dimension(0),originalDomainDimension);
  
  int i;
  for( i=0; i<domainDimension; i++ )
  {
    assert( activeAxis[i]<originalDomainDimension );
    r2(I,activeAxis[i])=r(I,i);
  }
  for( i=0; i<numberOfInActiveAxes; i++ )
   r2(I,inActiveAxis[i])=inActiveAxisValue[i];

  if( computeMapDerivative )
    xr2.redim(I,R,originalMap->getDomainDimension());
  if( computeMap && computeMapDerivative )
  {
    #ifdef USE_PPP
    originalMap->mapS(r2,x,xr2 );
    #else
    originalMap->map(r2,x,xr2 );
    #endif
    for( int i=0; i<domainDimension; i++ )
      xr(I,R,i)=xr2(I,R,activeAxis[i]);
  }
  else if( computeMap )
  {
    #ifdef USE_PPP
    originalMap->mapS(r2,x);
    #else
    originalMap->map(r2,x);
    #endif
  }
  else
  {
    #ifdef USE_PPP
      originalMap->mapS(r2,Overture::nullRealArray(),xr2 );
    #else
      originalMap->map(r2,Overture::nullRealArray(),xr2 );
    #endif
    for( int i=0; i<domainDimension; i++ )
      xr(I,R,i)=xr2(I,R,activeAxis[i]);
  }

}

void 
ReductionMapping::
basicInverse(const realArray & x, 
	     realArray & r,
	     realArray & rx /*=Overture::nullRealDistributedArray()*/,
	     MappingParameters & params /* =Overture::nullMappingParameters()*/)
{
  Overture::abort("ERROR:ReductionMapping::basicInverse -- this function is not valid");

#ifndef USE_PPP
  basicInverseS(x,r,rx,params);
#else
  Overture::abort("finish this for parallel");
#endif
}



void 
ReductionMapping::
basicInverseS(const RealArray & x, 
	      RealArray & r,
	      RealArray & rx /*=Overture::nullRealArray()*/,
	      MappingParameters & params /* =Overture::nullMappingParameters()*/)
{

  // *wdh* 080627 -- this inverse is not correct : we need to use generic the L2 inverse 

  Overture::abort("ERROR:ReductionMapping::basicInverse -- this function is not valid");

  if( originalMap==NULL )
  {
    cout << "ReductionMapping::map: Error: The mapping to reduce has not been defined yet!\n";
    exit(1);    
  }

  Index X = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  Range R(0,rangeDimension-1);
  RealArray x2,r2,rx2;
  const int originalDomainDimension=originalMap->getDomainDimension();
  assert( originalDomainDimension>=0 && originalDomainDimension<=3 );
  Range DO(0,originalDomainDimension-1);

  x2.redim(X,R);
  x2(X,R) = x(X,R);
  r2.redim(X, DO);
  int a;
  for ( a=0; a<domainDimension; a++ )
  {
    assert(activeAxis[a]<originalDomainDimension);
    r2(X,activeAxis[a]) = r(X,a);
  }
  for (a=0; a<numberOfInActiveAxes; a++)
  {
    r2(X,inActiveAxis[a]) = inActiveAxisValue[a];
  }

  if ( computeMapDerivative )
  {
    rx2.redim(X.getLength(), originalDomainDimension, rangeDimension);
  }
  else
    rx2 = Overture::nullRealArray();

  #ifdef USE_PPP
    originalMap->inverseMapS(x2,r2,rx2,params); 
  #else
    originalMap->inverseMap(x2,r2,rx2,params); 
  #endif
  for ( a=0; a<domainDimension; a++ )
  {
    assert(activeAxis[a]<originalDomainDimension);
    r(X,a) = r2(X,activeAxis[a]);
  }

  if ( computeMapDerivative )
  {
    for ( a=0; a<domainDimension; a++ )
    {
      rx(X,a,R) = rx2(X,activeAxis[a],R);
    }
  }

}

//=================================================================================
// get a mapping from the database
//=================================================================================
int ReductionMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering ReductionMapping::get" << endl;

  subDir.get( ReductionMapping::className,"className" ); 
  if( ReductionMapping::className != "ReductionMapping" )
  {
    cout << "ReductionMapping::get ERROR in className!" << endl;
  }

  subDir.get(numberOfInActiveAxes ,"numberOfInActiveAxes" );
  subDir.get(activeAxis ,"activeAxis",3);
  subDir.get(inActiveAxis ,"inActiveAxis",3);
  subDir.get(inActiveAxisValue ,"inActiveAxisValue",3);

  aString mappingClassName;
  subDir.get(mappingClassName,"originalMap.className");  
  originalMap = Mapping::makeMapping( mappingClassName );  // ***** this does a new -- who will delete? ***
  if( originalMap==NULL )
  {
    cout << "ReductionMapping::get:ERROR unable to make the mapping with className = " 
      << mappingClassName << endl;
    return 1;
  }
  originalMap->get( subDir,"originalMap" ); 

  Mapping::get( subDir, "Mapping" );
  delete &subDir;
  mappingHasChanged();
  return 0;
}

int ReductionMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( ReductionMapping::className,"className" );

  subDir.put(numberOfInActiveAxes ,"numberOfInActiveAxes" );
  subDir.put(activeAxis ,"activeAxis",3);
  subDir.put(inActiveAxis ,"inActiveAxis",3);
  subDir.put(inActiveAxisValue ,"inActiveAxisValue",3);

  subDir.put( originalMap->getClassName(),"originalMap.className"); // save the class name so we can do a 
  // "makeMapping" in the get function
  originalMap->put( subDir,"originalMap" );

  Mapping::put( subDir, "Mapping" );
  delete &subDir;
  return 0;
}

Mapping *ReductionMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==ReductionMapping::className )
    retval = new ReductionMapping();
  return retval;
}

    

//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int ReductionMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sPrintF
  aString menu[] = 
    {
      "!ReductionMapping",
      "reduce the domain dimension of which mapping?",
      "choose the in-active axes",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "show parameters",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "reduce the domain dimension of which mapping? : choose a mapping",
      "choose the in-active axes: specify values to fix some coordinate directions",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  aString answer,line,answer2; 

  bool plotObject=FALSE;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  gi.appendToTheDefaultPrompt("Reduction>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="reduce the domain dimension of which mapping?" )
    { // Make a menu with the Mapping names
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      for( int i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
        menu2[i]=map.getName(mappingName);
      }
      menu2[num]="none"; 
      menu2[num+1]="";   // null string terminates the menu
      int mapNumber = gi.getMenuItem(menu2,answer2);
      delete [] menu2;
      if( answer2=="none" )
        continue;
      if( mapInfo.mappingList[mapNumber].mapPointer==this )
      {
	cout << "ReductionMapping::ERROR: you cannot transform this mapping, this would be recursive!\n";
        continue;
      }
      if( originalMap!=NULL && originalMap->decrementReferenceCount()==0 )
        delete originalMap;
      originalMap=mapInfo.mappingList[mapNumber].mapPointer;
      if( !originalMap->uncountedReferencesMayExist() )
	originalMap->incrementReferenceCount();  // *wdh* 052601

      // Define properties of this mapping
      setName(mappingName,aString("reduction-")+originalMap->getName(mappingName));
      setInActiveAxes(inActiveAxisValue);  // default inactive axes, r[axis1]=0.

      mappingHasChanged();
      plotObject=TRUE;
    }
    else if( answer=="choose the in-active axes" ) 
    {
      if( originalMap==NULL )
      {
	printf("You must first choose a mapping to reduce the domain of.\n");
      }
      else
      {
	printf("Any coordinate direction may be fixed to a given value or r between 0. and 1.\n");
	printf("Such a coordinate direction will be called an in-active axis\n");
	printf("Enter `active' if you want a given axis to remain active over [0,1]\n");
	real value[3] = {-1.,-1.,-1.};
	for( int axis=0; axis<originalMap->getDomainDimension(); axis++ )
	{
	  gi.inputString(line,sPrintF(buff,"enter a value for r for axis %i, or enter `active' to keep active",
				      axis));
	  if( line!="active" && line!="" )
	  {
	    sScanF(line,"%e ",&value[axis]);
	    if( value[axis]<0. || value[axis]>1. )
	    {
	      value[axis]=max(0.,min(1.,value[axis]));
	      printf("ERROR: the value for the in-active axis should be in [0,1] \n");
	      printf("       I am setting the value to %e \n",value[axis]);
	    }
	  }
	}
	setInActiveAxes( value );

	mappingHasChanged();
      }
    }
    else if( answer=="show parameters" )
    {
      for( int axis=0; axis<numberOfInActiveAxes; axis++ )
	printf("Axis %i is inactive with a fixed value of %e \n",inActiveAxis[axis],inActiveAxisValue[axis]);
	
      display();
    }
    else if( answer=="plot" )
    {
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters); 
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=TRUE;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
    }

    if( plotObject )
    {
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters);   // *** recompute every time ?? ***

    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
  
}
