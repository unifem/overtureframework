#include "ComposeMapping.h"
#include "MappingInformation.h"
#include "display.h"
#include "ParallelUtility.h"

//----------------------------------------------------------------
// ComposeMapping
//       Compose two Mapping functions
//       -----------------------------
// Constructor example:
//   ComposeMapping mapc( mapa,mapb );
// ...means
//     mapc <-  (mapb o mapa)
//  mapc means to apply mapa followed by mapb
//----------------------------------------------------------------

ComposeMapping::
ComposeMapping( )
{
  ComposeMapping::className="ComposeMapping";         
  setName( Mapping::mappingName,"compose");
//approximateGlobalInverse=NULL;
//exactLocalInverse=NULL;
  useDefaultInverse=FALSE;
  mappingHasChanged();
}


// Copy constructor is deep by default
ComposeMapping::
ComposeMapping( const ComposeMapping & map, const CopyType copyType )
{
  ComposeMapping::className="ComposeMapping";         
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "ComposeMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

ComposeMapping::
ComposeMapping( Mapping & mapa, Mapping & mapb )
{
  ComposeMapping::className="ComposeMapping";         
  setName( Mapping::mappingName,"compose");
//approximateGlobalInverse=NULL;
//exactLocalInverse=NULL;
  useDefaultInverse=FALSE;
  setup(mapa,mapb);
}

const realArray& ComposeMapping::
getGrid(MappingParameters & params /* =Overture::nullMappingParameters() */,
        bool includeGhost /* =false */ )
// ================================================================================
//   Use this grid for plotting the mapping and/or for the inverse
// ================================================================================
{
  // I don't know if the composed mappings have changed
  // *wdh* 070320 mappingHasChanged();  -- force the user to indicate this 
  return Mapping::getGrid(params,includeGhost);  // could try to be smarter here
}



void ComposeMapping::
setMappings( Mapping & mapa, Mapping & mapb )
{
  setup(mapa,mapb);
}

void ComposeMapping::
setup( Mapping & mapa, Mapping & mapb )
{
  ComposeMapping::className="ComposeMapping";
  map1.reference(mapa);
  map2.reference(mapb);
  if( mapa.getRangeSpace() != mapb.getDomainSpace() )
  {
    cout << "ERROR in ComposeMapping - cannot compose these mappings because the \n"
      " range space of mapa is not the same as the domain space of mapb\n";
    return;
  }
  if( mapa.getRangeDimension() != mapb.getDomainDimension() )
  {
    cout << "ERROR in ComposeMapping - cannot compose these mappings because the \n"
      " range dimension of mapa (=" << mapa.getRangeDimension() << ") is not the same as "
      "the domain dimension of mapb (=" << mapb.getDomainDimension() << ")\n";
    return;
  }
  
  if( Mapping::debug & 4 )
  {
    cout << "Composing two mappings, mapa=" << mapa.getName(mappingName)
         << ", mapb =" << mapb.getName(mappingName) << endl;
  }
  
  setDomainDimension( mapa.getDomainDimension() );
  setRangeDimension( mapb.getRangeDimension() );

  // Choose the BC's share etc from one of the mappings, if possible choose the
  // mapping that maps from parameterSpace to cartesianSpace
  Mapping *mapPointer;
  if( mapa.getDomainSpace()==parameterSpace && mapa.getRangeSpace()==cartesianSpace )
    mapPointer=&mapa;
  else
    mapPointer=&mapb;
    
  if( getName(mappingName)=="compose" )
    setName(mappingName,mapPointer->getName(mappingName));
  int axis;
  for( axis=axis1; axis<getDomainDimension(); axis++ )
  { 
    setGridDimensions( axis,mapPointer->getGridDimensions(axis) );
    if(  mapa.getDomainSpace()==parameterSpace )
      setIsPeriodic( axis,mapa.getIsPeriodic(axis) );
    for( int side=Start; side<=End ; side++ )
    { 
      setBoundaryCondition( side,axis,mapPointer->getBoundaryCondition(side,axis) );
      setShare( side,axis,mapPointer->getShare(side,axis) );

      // domain bounds are taken from mapa
      setDomainBound( side,axis,mapa.getDomainBound(side,axis) );
      setDomainCoordinateSystemBound(side,axis,mapa.getDomainCoordinateSystemBound(side,axis) );
    }
  }

  // check for the basic inverse
  if( mapa.getBasicInverseOption()==canInvert &&
      mapb.getBasicInverseOption()==canInvert )
    setBasicInverseOption(canInvert);
  else
    setBasicInverseOption(canDoNothing);

  // *wdh* 2011/06/29
  inverseIsDistributed = mapa.usesDistributedInverse() || mapb.usesDistributedInverse();
  // *wdh* 2011/10/01 : The DPM has a distributed map
  mapIsDistributed = mapa.usesDistributedMap() || mapb.usesDistributedMap();

  if( debug & 8 )
    printF("ComposeMapping: name=%s, mapa.usesDistributedInverse()=%i mapb.usesDistributedInverse()=%i\n",
	   (const char*)getName(mappingName),(int)mapa.usesDistributedInverse(),(int)mapb.usesDistributedInverse());

  for( axis=axis1; axis< getRangeDimension(); axis++ )
  {
    for( int side=Start; side<=End ; side++ )
    { 
      // range bounds are taken from mapb
      setRangeBound( side,axis,mapb.getRangeBound(side,axis) );
      setRangeCoordinateSystemBound( side,axis,
         mapb.getRangeCoordinateSystemBound(side,axis) );
    }
  }
  setDomainSpace( mapa.getDomainSpace() );
  setRangeSpace( mapb.getRangeSpace() );  
  setDomainCoordinateSystem( mapa.getDomainCoordinateSystem() ); 
  setRangeCoordinateSystem( mapa.getRangeCoordinateSystem() ); 

/* ----- *wdh* 000922
  // Define the inverse functions for inverseMap:
  if( approximateGlobalInverse ) 
    delete approximateGlobalInverse;
  approximateGlobalInverse=new ApproximateGlobalInverse( *this );
  assert( approximateGlobalInverse != 0 );
  if( exactLocalInverse ) 
    delete exactLocalInverse;
  exactLocalInverse=new ExactLocalInverse( *this );
  assert( exactLocalInverse != 0 );
---- */ 

  // define some Ranges ** this would be wrong if mappings change! ***
  Rr=Range(0,mapa.getDomainDimension()-1);  
  Ry=Range(0,mapa.getRangeDimension()-1);  
  Rx=Range(0,mapb.getRangeDimension()-1);  
  mappingHasChanged();

}

ComposeMapping::
~ComposeMapping()
{ 
  if( Mapping::debug & 4 )
     cout << " ComposeMapping::Destructor called" << endl;
//delete approximateGlobalInverse;
//delete exactLocalInverse;
}

ComposeMapping & ComposeMapping::
// =============================================================================
//  /Description: deep copy, The mappings that are composed are copied
// =============================================================================
operator =( const ComposeMapping & x )
{
  map1=x.map1;
  map2=x.map2;
  if( map1.mapPointer && map2.mapPointer )
    setMappings(*map1.mapPointer,*map2.mapPointer);

  useDefaultInverse=x.useDefaultInverse;

  this->Mapping::operator=(x);            // call = for base class
  return *this;
}

  // Use default Mapping inverse instead of optimized inverse defined by this mapping
int  ComposeMapping::
useDefaultMappingInverse(bool trueOrFalse /* = TRUE */ )
{
  useDefaultInverse=trueOrFalse;
  if( useDefaultInverse ) 
    setBasicInverseOption( Mapping::canDoNothing );  // turn off inverse if it exists
  else
  {
    if( map1.getBasicInverseOption()==canInvert &&
	map2.getBasicInverseOption()==canInvert )
      setBasicInverseOption(canInvert);
    else
      setBasicInverseOption(canDoNothing);
  }
  
  return 0;
}

void ComposeMapping::
useRobustInverse(const bool trueOrFalse /* =TRUE */ )
// =======================================================================================
// /Description:
//    Use the robust form of the inverse.
// =======================================================================================
{
  map1.useRobustInverse(trueOrFalse);
  map2.useRobustInverse(trueOrFalse);

}


// ---get a mapping from the database---
int ComposeMapping::
get( const GenericDataBase & dir, const aString & name)
{
  if( Mapping::debug & 4 )
    cout << "Entering ComposeMapping::get" << endl;

  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");

  subDir.get( ComposeMapping::className,"className" ); 
  if( ComposeMapping::className != "ComposeMapping" )
  {
    cout << "ComposeMapping::get ERROR in className!" << endl;
  }

  subDir.get( useDefaultInverse,"useDefaultInverse");

  map1.get( subDir, "map1" );
  map2.get( subDir, "map2" );

  setup( *map1.mapPointer,*map2.mapPointer );  // *** is this ok *** why reference again
  Mapping::get( subDir, "Mapping" );

  if( debug & 8 )
    printF("ComposeMapping:get name=%s, usesDistributedInverse=%i map1.usesDistributedInverse()=%i "
           "map2.usesDistributedInverse()=%i\n"
           " usesDistributedMap=%i map1.usesDistributedMap()=%i map2.usesDistributedMap()=%i\n",
	   (const char*)getName(mappingName),
           (int)usesDistributedInverse(),
           (int)map1.getMapping().usesDistributedInverse(),(int)map2.getMapping().usesDistributedInverse(),
           (int)usesDistributedMap(),
           (int)map1.getMapping().usesDistributedMap(),(int)map2.getMapping().usesDistributedMap()
            );

  mappingHasChanged();
  delete &subDir;
  return 0;
}

int ComposeMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( ComposeMapping::className,"className" );
  subDir.put( useDefaultInverse,"useDefaultInverse");

  map1.put( subDir, "map1" );
  map2.put( subDir, "map2" );
  Mapping::put( subDir, "Mapping" );

  delete &subDir;
  return 0;
}

Mapping* ComposeMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=NULL;
  if( mappingClassName==ComposeMapping::className )
  {
    retval = new ComposeMapping();
    assert( retval != 0 );
  }
  return retval;
}

void ComposeMapping::
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

  // Should check consistency of dimensions here
  int i,j,k;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );
  Range R(base,bound);

  realArray y(R,Ry), yr,xy;
  if( computeMapDerivative )
  {
    yr.redim(R,Ry,Rr);
    xy.redim(R,Rx,Ry);
  }
  

  map1.map( r,y,yr,params ); 

  map2.map( y,x,xy,params );

  if( computeMapDerivative )
  {
    for( i=axis1; i < getRangeDimension(); i++ )
      for( j=axis1; j < getDomainDimension() ; j++ )
      {
	xr(I,i,j)=0.;
	for( k=axis1; k< map2.getDomainDimension(); k++ )
	  xr(I,i,j)=xr(I,i,j)+xy(I,i,k)*yr(I,k,j);
      }
  }
  
}
//=================================================================================
//          inverseMap
//  Invert the mapping (map2 o map1)
//=================================================================================
void ComposeMapping::
inverseMap( const realArray & x, realArray & r, realArray & rx,
			  MappingParameters & params )
{
#ifdef USE_PPP
  realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(x,xLocal);
  realSerialArray rLocal;  getLocalArrayWithGhostBoundaries(r,rLocal);
  realSerialArray rxLocal; getLocalArrayWithGhostBoundaries(rx,rxLocal);
  inverseMapS(xLocal,rLocal,rxLocal,params);
  return;
  
#else

  if( useDefaultInverse )
  {
    // use default inverse from base class (this is NOT the default and probably slower)
    Mapping::inverseMap(x,r,rx,params );
    return;
  }
  // Use user supplied inverse's if they are available (and space is not periodic)
  if( getBasicInverseOption()==canInvert && params.periodicityOfSpace==0 )
  {
    basicInverse(x,r,rx,params);
    return;
  }
  
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  realArray yi(I,Ry), ry,yx;     

  if( computeMapDerivative )
  {
    ry.redim(I,Ry,Rr);
    yx.redim(I,Rx,Ry);
  }
  if( map2.getBasicInverseOption()==canInvert && params.periodicityOfSpace==0 )
  { // map2 can be inverted fast but not map 1
    map2.basicInverse( x,yi,yx ); // Solve y=map2^(-1)(x) (basic inverse)
    map1.inverseMap( yi,r,ry ); // Global Inverse needed!
    if( computeMapDerivative )
    {
      for( int m=axis1; m < getDomainDimension(); m++ )
      {
	for( int n=axis1; n < getRangeDimension(); n++ )
	{
	  rx(I,m,n)=0.;
	  for( int k=axis1; k< map2.getDomainDimension(); k++ )
	    rx(I,m,n)+=ry(I,m,k)*yx(I,k,n);
	}
      }
    }
  }
  else
  {
    // We need to solve  x = map2(map1(r))  for x given r
    //    first get the closest point, r0,  for the whole mapping:
    //                  x ~=  map2(map1(r0))   (r0=closest point)
    //     then invert  x = map2(y)   for y given the initial guess y=map1(r0)
    //     finally invert  y=map1(r) for r

    // first get the initial guess based on the composed mapping
    if( Mapping::debug & 4 )
      cout << " ComposeMapping::call approximateGlobalInverse..." << endl;

    // **** consider case when one basicInverse exists ****

    // MappingWorkSpace & ws = workSpace;
    MappingWorkSpace ws;
    approximateGlobalInverse->inverse( x,r,rx,ws,params );  // -> result : ws.x0,ws.r0,ws.I0,ws.index0


    Index Axes(0,domainDimension);
    Index xAxes(0,rangeDimension);

    // Here we remove "bogus" points from the list of invertible points
    int base0=ws.I0.getBase(); 
    int bound0=ws.I0.getBound();
    int j=base0-1;
    if( ws.index0IsSequential ) // we need to build an indirect address array
    {
      ws.index0.redim(ws.I0);
      ws.index0.seqAdd(base0,1);
    }
    for( int i=base0; i<=bound0; i++ )
    {
      if( ws.r0(i,0)==Mapping::bogus )
	r(i,Axes)=Mapping::bogus;   // assign final result as bogus for this point.
      else
      {
        j++;
	if( j!=i )
	{
          ws.index0(j)=ws.index0(i);
	  ws.r0(j,Axes)=ws.r0(i,Axes);
	  ws.x0(j,xAxes)=ws.x0(i,xAxes);
	}
      }
    }
    if( j!=bound0 )
    {
      // printf("ComposeMapping::inverseMap: compressing ws.r0 after approximate inverse. old =%i, new=%i\n",
      //     bound0-base0+1,j-base0+1);
      if( j<base0 )
        return;
      bound0=j;
      ws.I0=Range(base0,bound0);
      ws.index0IsSequential=FALSE;
      ws.r0.resize(ws.I0,Axes);
      ws.x0.resize(ws.I0,xAxes);
      yi.redim(ws.I0,Ry);
    }


    Range R(base0,bound0);

    MappingParameters localParams = params;       // (Use this for doing local inverse only)
    localParams.computeGlobalInverse=FALSE;  


    map1.map( ws.r0,yi,Overture::nullRealDistributedArray(),params );    // Initial guess for map2^(-1) is  y=map1(r0):
    // maybe map2^(-1) doesn't need a guess ****
    
    if( Mapping::debug & 4 )
      cout << " ComposeMapping::call map2 inverse..." << endl;

    map2.inverseMap( ws.x0,yi,yx,localParams ); // Solve y=map2^(-1)(x0) (local inverse)
    
    if( Mapping::debug & 4 )
    {
      yi.display("ComposeMapping, y=map2^(-1)(x0):");
      cout << " ComposeMapping::call map1 inverse..." << endl;
    }
    
    // *** yi must be exactly the right size here ***
    map1.inverseMap( yi,ws.r0,ry ); // Global Inverse needed!

    if(  Mapping::debug & 4 )
    {
      ws.r0.display("ComposeMapping, r0=map1^(-1)(y):");
    }

    //  copy r0 --> r
    
    if( computeMap )
    {
      if( ws.index0IsSequential ) // remember that some bogus points may have been removed.
        r(R,Axes)=ws.r0(R,Axes);
      else
      {
	for( int i=base0; i<=bound0; i++ )
	  r(ws.index0(i),Axes)=ws.r0(i,Axes);   // **** use status! ****
      }
    }
    if( computeMapDerivative )
    {
      for( int m=axis1; m < getDomainDimension(); m++ )
      {
	for( int n=axis1; n < getRangeDimension(); n++ )
	{
	  rx(I,m,n)=0.;
	  for( int k=axis1; k< map2.getDomainDimension(); k++ )
	  {
	    if( TRUE || ws.index0IsSequential )  // **wdh* 990525
	    {
              rx(R,m,n)+=ry(R,m,k)*yx(R,k,n);
	    }
	    else
	    {
	      for( int i=base0; i<=bound0; i++ )
		rx(ws.index0(i),m,n)+=ry(i,m,k)*yx(i,k,n);
	    }
	  }
	}
      }
    }
  }  
#endif

}

//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//       Invert the mapping (map2 o map1)
//=================================================================================
void ComposeMapping::
basicInverse( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
{
#ifdef USE_PPP
  realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(x,xLocal);
  realSerialArray rLocal;  getLocalArrayWithGhostBoundaries(r,rLocal);
  realSerialArray rxLocal; getLocalArrayWithGhostBoundaries(rx,rxLocal);
  basicInverseS(xLocal,rLocal,rxLocal,params);
  return;
#else

  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );
  // --- allocate more work space if needed --- 
  // 
  realArray yi(I,Ry), ry,yx;     

  if( computeMapDerivative )
  {
    ry.redim(I,Rr,Ry);
    yx.redim(I,Ry,Rx);
  }

  yi=-1.;  // initial guess is not known
  if( computeMap && computeMapDerivative )
  {
    map2.basicInverse( x,yi,yx,params );  
    map1.basicInverse( yi,r,ry,params );       // Note: yi must be the correct size here!
  }
  else if( computeMap )
  {
    map2.basicInverse( x,yi,Overture::nullRealDistributedArray(),params );  
    map1.basicInverse( yi,r,Overture::nullRealDistributedArray(),params ); 
    if( false )
    {
      printf(" ComposeMapping: map1: %s, map2: %s\n",(const char*)map1.getClassName(),
                                                     (const char*)map2.getClassName());
      ::display(x,"ComposeMapping:basicInverse: x","%5.2f ");
      ::display(yi,"ComposeMapping:basicInverse: yi (from map2.basicInverse","%5.2f ");
      ::display(r,"ComposeMapping:basicInverse: r (from map1.basicInverse","%5.2f ");
    }
  }
  else if( computeMapDerivative )
  {
    map2.basicInverse( x,yi,yx,params );  
    map1.basicInverse( yi,Overture::nullRealDistributedArray(),ry,params ); 
  }
  if( computeMapDerivative )
  {
    Index I = Range(base,bound);
    for( int m=axis1; m < getDomainDimension(); m++ )
    {
      for( int n=axis1; n < getRangeDimension(); n++ )
      {
	rx(I,m,n)=0.;
	for( int k=axis1; k< map2.getDomainDimension(); k++ )
          rx(I,m,n)+=ry(I,m,k)*yx(I,k,n);
      }
    }
  }
  return;
#endif

}


void ComposeMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr,
                          MappingParameters & params )
{
  // Should check consistency of dimensions here
  int i,j,k;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );
  Range R(base,bound);

  RealArray y(R,Ry), yr,xy;
  if( computeMapDerivative )
  {
    yr.redim(R,Ry,Rr);
    xy.redim(R,Rx,Ry);
  }
  

  map1.mapS( r,y,yr,params ); 

  map2.mapS( y,x,xy,params );

  if( computeMapDerivative )
  {
     for( i=axis1; i < getRangeDimension(); i++ )
	for( j=axis1; j < getDomainDimension() ; j++ )
	{
	  xr(I,i,j)=0.;
	  for( k=axis1; k< map2.getDomainDimension(); k++ )
	     xr(I,i,j)=xr(I,i,j)+xy(I,i,k)*yr(I,k,j);
	}
  }
  
}
//=================================================================================
//          inverseMap
//  Invert the mapping (map2 o map1)
//=================================================================================
void ComposeMapping::
inverseMapS( const RealArray & x, RealArray & r, RealArray & rx,
			  MappingParameters & params )
{

  if( useDefaultInverse )
  {
    // use default inverse from base class (this is NOT the default and probably slower)
    Mapping::inverseMapS(x,r,rx,params );
    return;
  }
  // Use user supplied inverse's if they are available (and space is not periodic)
  if( getBasicInverseOption()==canInvert && params.periodicityOfSpace==0 )
  {
    basicInverseS(x,r,rx,params);
    return;
  }
  
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  RealArray yi(I,Ry), ry,yx;     

  if( computeMapDerivative )
  {
    ry.redim(I,Ry,Rr);
    yx.redim(I,Rx,Ry);
  }
  if( map2.getBasicInverseOption()==canInvert && params.periodicityOfSpace==0 )
  { 
    // map2 can be inverted fast but not map 1

    // printF("ComposeMapping::inverseMapS: name=%s, use map2.basicInverseS followed by map1.inverseMapS\n",(const char*)getName(mappingName));
    // printF("ComposeMapping::inverseIsDistributed=%i\n",(int)inverseIsDistributed);
    
    map2.basicInverseS( x,yi,yx ); // Solve y=map2^(-1)(x) (basic inverse)
    map1.inverseMapS( yi,r,ry ); // Global Inverse needed!
    if( computeMapDerivative )
    {
      for( int m=axis1; m < getDomainDimension(); m++ )
      {
	for( int n=axis1; n < getRangeDimension(); n++ )
	{
	  rx(I,m,n)=0.;
	  for( int k=axis1; k< map2.getDomainDimension(); k++ )
	    rx(I,m,n)+=ry(I,m,k)*yx(I,k,n);
	}
      }
    }
  }
  else
  {
    // We need to solve  x = map2(map1(r))  for x given r
    //    first get the closest point, r0,  for the whole mapping:
    //                  x ~=  map2(map1(r0))   (r0=closest point)
    //     then invert  x = map2(y)   for y given the initial guess y=map1(r0)
    //     finally invert  y=map1(r) for r

    // first get the initial guess based on the composed mapping
    if( Mapping::debug & 4 )
      printF(" ComposeMapping::inverseMapS: call approximateGlobalInverse...\n");

    // **** consider case when one basicInverse exists ****

    // MappingWorkSpace & ws = workSpace;
    MappingWorkSpace ws;
    approximateGlobalInverse->inverse( x,r,rx,ws,params );  // -> result : ws.x0,ws.r0,ws.I0,ws.index0

    Index Axes(0,domainDimension);
    Index xAxes(0,rangeDimension);

    // Here we remove "bogus" points from the list of invertible points
    int base0=ws.I0.getBase(); 
    int bound0=ws.I0.getBound();
    int j=base0-1;
    if( ws.index0IsSequential ) // we need to build an indirect address array
    {
      ws.index0.redim(ws.I0);
      ws.index0.seqAdd(base0,1);
    }
    for( int i=base0; i<=bound0; i++ )
    {
      if( ws.r0(i,0)==Mapping::bogus )
	r(i,Axes)=Mapping::bogus;   // assign final result as bogus for this point.
      else
      {
        j++;
	if( j!=i )
	{
          ws.index0(j)=ws.index0(i);
	  ws.r0(j,Axes)=ws.r0(i,Axes);
	  ws.x0(j,xAxes)=ws.x0(i,xAxes);
	}
      }
    }
    if( j!=bound0 )
    {
      // printf("ComposeMapping::inverseMap: compressing ws.r0 after approximate inverse. old =%i, new=%i\n",
      //     bound0-base0+1,j-base0+1);
      if( j<base0 )
        return;
      bound0=j;
      ws.I0=Range(base0,bound0);
      ws.index0IsSequential=FALSE;
      ws.r0.resize(ws.I0,Axes);
      ws.x0.resize(ws.I0,xAxes);
      yi.redim(ws.I0,Ry);
    }


    Range R(base0,bound0);

    MappingParameters localParams = params;       // (Use this for doing local inverse only)
    localParams.computeGlobalInverse=FALSE;  


    map1.mapS( ws.r0,yi,Overture::nullRealArray(),params );    // Initial guess for map2^(-1) is  y=map1(r0):
    // maybe map2^(-1) doesn't need a guess ****
    
    if( Mapping::debug & 4 )
      printF(" ComposeMapping::call map2 inverse... \n");

    map2.inverseMapS( ws.x0,yi,yx,localParams ); // Solve y=map2^(-1)(x0) (local inverse)
    
    if( Mapping::debug & 4 )
    {
      aString buff;
      ::display(yi,sPrintF(buff,"ComposeMapping: Stage I: y=map2^(-1)(x0) (map2=%s):",(const char*)map2.getName(mappingName)));
      printF(" ComposeMapping::call map1 inverse...\n");
    }
    
    // *** yi must be exactly the right size here ***
    map1.inverseMapS( yi,ws.r0,ry ); // Global Inverse needed!

    if( Mapping::debug & 4 )
    {
      aString buff;
      ::display(ws.r0,sPrintF(buff,"ComposeMapping: Stage II: r0=map1^(-1)(y) (map1=%s):",(const char*)map1.getName(mappingName)));
    }

    //  copy r0 --> r
    
    if( computeMap )
    {
      if( ws.index0IsSequential ) // remember that some bogus points may have been removed.
        r(R,Axes)=ws.r0(R,Axes);
      else
      {
	for( int i=base0; i<=bound0; i++ )
	  r(ws.index0(i),Axes)=ws.r0(i,Axes);   // **** use status! ****
      }
      if( Mapping::debug & 4 )
      {
	aString buff;
	::display(r,"ComposeMapping: r at end");
      }
    }
    if( computeMapDerivative )
    {
      for( int m=axis1; m < getDomainDimension(); m++ )
      {
	for( int n=axis1; n < getRangeDimension(); n++ )
	{
	  rx(I,m,n)=0.;
	  for( int k=axis1; k< map2.getDomainDimension(); k++ )
	  {
	    if( TRUE || ws.index0IsSequential )  // **wdh* 990525
	    {
              rx(R,m,n)+=ry(R,m,k)*yx(R,k,n);
	    }
	    else
	    {
	      for( int i=base0; i<=bound0; i++ )
		rx(ws.index0(i),m,n)+=ry(i,m,k)*yx(i,k,n);
	    }
	  }
	}
      }
    }
  }
  

}

//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//       Invert the mapping (map2 o map1)
//=================================================================================
void ComposeMapping::
basicInverseS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );
  // --- allocate more work space if needed --- 
  // 
  RealArray yi(I,Ry), ry,yx;     

  if( computeMapDerivative )
  {
    ry.redim(I,Rr,Ry);
    yx.redim(I,Ry,Rx);
  }

  yi=-1.;  // initial guess is not known
  if( computeMap && computeMapDerivative )
  {
    map2.basicInverseS( x,yi,yx,params );  
    map1.basicInverseS( yi,r,ry,params );       // Note: yi must be the correct size here!
  }
  else if( computeMap )
  {
    map2.basicInverseS( x,yi,Overture::nullRealArray(),params );  
    map1.basicInverseS( yi,r,Overture::nullRealArray(),params ); 
  }
  else if( computeMapDerivative )
  {
    map2.basicInverseS( x,yi,yx,params );  
    map1.basicInverseS( yi,Overture::nullRealArray(),ry,params ); 
  }
  if( computeMapDerivative )
  {
    Index I = Range(base,bound);
    for( int m=axis1; m < getDomainDimension(); m++ )
    {
      for( int n=axis1; n < getRangeDimension(); n++ )
      {
	rx(I,m,n)=0.;
	for( int k=axis1; k< map2.getDomainDimension(); k++ )
          rx(I,m,n)+=ry(I,m,k)*yx(I,k,n);
      }
    }
  }
  return;
}


real ComposeMapping::
sizeOf(FILE *file /* = NULL */ ) const
// =======================================================================================
/// \details 
///    Return size of this object  
// =======================================================================================
{
  real size=Mapping::sizeOf(file);

  size+=map1.sizeOf(file);
  size+=map2.sizeOf(file);

  return size;
}



//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int ComposeMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!ComposeMapping",
      "edit first map",
      "edit second map",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "check",
      "check inverse",
      "show parameters",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "revolve which mapping?",
      "edit first map     : edit the first Mapping in the composition",
      "edit second map    : edit the second Mapping in the composition",
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

  bool plotObject=TRUE;

  bool mappingChosen=map1.getMapping().getClassName()!="Mapping" &&
                     map2.getMapping().getClassName()!="Mapping";

  // By default transform the last mapping in the list (if this mapping is unitialized, mappingChosen==FALSE)
//   if( !mappingChosen )
//   {
//     int number= mapInfo.mappingList.getLength();
//     for( int i=number-1; i>=0; i-- )
//     {
//       Mapping *mapPointer=mapInfo.mappingList[i].mapPointer;
//       if( (mapPointer->getDomainDimension()==1 || mapPointer->getDomainDimension()==2) && 
//            mapPointer->getRangeDimension()==2  || mapPointer->getRangeDimension()==3 )
//       {
//         mappingHasChanged();
// 	break; 
//       }
//     }
//   }

  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  gi.appendToTheDefaultPrompt("Revolution>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="edit first map" )
    { 
      gi.erase();
      map1.update(mapInfo);
    }
    else if( answer=="edit second map" )
    {
      gi.erase();
      map2.update(mapInfo);
    }
    else if( answer=="show parameters" )
    {
      display();
      continue;
    }
    else if( answer=="plot" )
    {
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi, *this,parameters); 
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
      continue;
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity" ||
             answer=="check" ||
             answer=="check inverse" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
      if( answer=="mappingName" )
        continue;
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
      PlotIt::plot(gi,*this,parameters);   
    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
  
}
