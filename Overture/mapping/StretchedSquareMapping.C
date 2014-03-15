#include "StretchedSquare.h"
#include "MappingInformation.h"
#include "ParallelUtility.h"

StretchedSquare::
StretchedSquare(const int & domainDimension_ /* =2  */ )
// ========================================================================
// 
// ========================================================================
: Mapping(domainDimension_,domainDimension_,parameterSpace,parameterSpace)   
{ 
  StretchedSquare::className="StretchedSquare";
  setName( Mapping::mappingName,"StretchedUnitSquare");
  for( int axis=0; axis<domainDimension; axis++ )
    setGridDimensions( axis,11 );
  setBasicInverseOption(canInvert);           // basicInverse is available
  inverseIsDistributed=false;
  // setMappingCoordinateSystem( rectangular );  // for optimizing derivatives
  mappingHasChanged();

}

// Copy constructor is deep by default
StretchedSquare::
StretchedSquare( const StretchedSquare & map, const CopyType copyType )
{
  StretchedSquare::className="StretchedSquare";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "StretchedSquare:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

StretchedSquare::
~StretchedSquare()
{ if( debug & 4 )
  cout << " StretchedSquare::Desctructor called" << endl;
}

StretchedSquare & StretchedSquare::
operator=( const StretchedSquare & X0 )
{
  if( StretchedSquare::className != X0.getClassName() )
  {
    cout << "StretchedSquare::operator= ERROR trying to set a StretchedSquare = to a" 
      << " mapping of type " << X0.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X0);            // call = for derivee class
  StretchedSquare & X = (StretchedSquare&) X0;  // cast to a Square mapping
  for( int axis=0; axis<3; axis++)
    stretch[axis]=X.stretch[axis];

  return *this;
}

StretchMapping &  StretchedSquare::
stretchFunction( const int & axis /* = 0 */ )
// =================================================================================
// /Description:
//   Return a reference to the stretching function along a given coordinate direction
// =================================================================================
{
  assert( axis>=0 && axis<3 );
  return stretch[axis];
}




void StretchedSquare::
map( const realArray & r, realArray & x, realArray & xr,
                         MappingParameters & params )
{
  #ifdef USE_PPP
    RealArray rLocal; getLocalArrayWithGhostBoundaries(r,rLocal);
    RealArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
    RealArray xrLocal; getLocalArrayWithGhostBoundaries(xr,xrLocal);
    mapS(rLocal,xLocal,xrLocal,params);
    return;
  #else
    mapS(r,x,xr,params);
    return;
  #endif
}

//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//=================================================================================
void StretchedSquare::
basicInverse( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
{
  #ifdef USE_PPP
    RealArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
    RealArray rLocal; getLocalArrayWithGhostBoundaries(r,rLocal);
    RealArray rxLocal; getLocalArrayWithGhostBoundaries(rx,rxLocal);
    basicInverseS(xLocal,rLocal,rxLocal,params);
    return;
  #else
    basicInverseS(x,r,rx,params);
    return;
  #endif
}
  


// version for serial arrays
void StretchedSquare::
mapS( const RealArray & r, RealArray & x, RealArray & xr,
                         MappingParameters & params )
{
  if( params.coordinateType != cartesian )
    cerr << "StretchedSquare::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );


  RealArray r0(I,1), x0(I,1), xr0(I,1,1);
  if( computeMap && computeMapDerivative )
  {
    xr=0.; // set off diagonal entries
    for( int axis=0; axis<domainDimension; axis++)
    {
      stretch[axis].mapCS(r(I,axis),x(I,axis),xr(I,axis,axis)); // use mapC to pass a view (needed by IBM xlC)
    }
  }
  else if( computeMap )
  {
    for( int axis=0; axis<domainDimension; axis++)
    {
      stretch[axis].mapCS(r(I,axis),x(I,axis));
    }
  }
  else if( computeMapDerivative )
  {
    xr=0.;
    for( int axis=0; axis<domainDimension; axis++)
    {
      stretch[axis].mapCS(r(I,axis),Overture::nullRealArray(),xr(I,axis,axis));
    }
  }
}

//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//=================================================================================
void StretchedSquare::
basicInverseS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

//  RealArray r0(I,1), x0(I,1), rx0(I,1,1);
  if( computeMap && computeMapDerivative )
  {
    rx=0.;
    for( int axis=0; axis<domainDimension; axis++)
    {
      if( axis==0 )
        stretch[axis].inverseMapCS(x(I,axis),r(I,axis),rx(I,axis,axis));
      else
      { // set the base to zero for the inverse routine
	RealArray & r0 = (RealArray &)r;   const int rBase=r.getBase(1); r0.setBase(-axis,1);
	RealArray & x0 = (RealArray &)x;   const int xBase=x.getBase(1); x0.setBase(-axis,1);
	RealArray & rx0 = (RealArray &)rx; const int rxBase=rx.getBase(1); rx0.setBase(-axis,1); rx0.setBase(-axis,2);

        stretch[axis].inverseMapCS(x0(I,0),r0(I,0),rx0(I,0,0));

	r0.setBase(rBase,1); x0.setBase(xBase,1); rx0.setBase(rxBase,1); rx0.setBase(rxBase,2);

      }
    }
  }
  else if( computeMap )
    for( int axis=0; axis<domainDimension; axis++)
    {
      if( axis==0 )
        stretch[axis].inverseMapCS(x(I,axis),r(I,axis));
      else
      {
	RealArray & r0 = (RealArray &)r;   const int rBase=r.getBase(1); r0.setBase(-axis,1);
	RealArray & x0 = (RealArray &)x;   const int xBase=x.getBase(1); x0.setBase(-axis,1);

        stretch[axis].inverseMapCS(x0(I,0),r0(I,0));

	r0.setBase(rBase,1); x0.setBase(xBase,1); 
      }
    }
  else if( computeMapDerivative )
  {
    rx=0.;
    for( int axis=0; axis<domainDimension; axis++)
    {
      if( axis==0 )
        stretch[axis].inverseMapCS(x(I,axis),Overture::nullRealArray(),rx(I,axis,axis));
      else
      {
	RealArray & x0 = (RealArray &)x;   const int xBase=x.getBase(1); x0.setBase(-axis,1);
	RealArray & rx0 = (RealArray &)rx; const int rxBase=rx.getBase(1); rx0.setBase(-axis,1); rx0.setBase(-axis,2);

        stretch[axis].inverseMapCS(x0(I,0),Overture::nullRealArray(),rx0(I,0,0));

	x0.setBase(xBase,1); rx0.setBase(rxBase,1); rx0.setBase(rxBase,2);
      }
    }
  }
}
  

//=================================================================================
// get a mapping from the database
//=================================================================================
int StretchedSquare::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase *subDir = dir.virtualConstructor();
  dir.find(*subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering StretchedSquare::get" << endl;

  subDir->get( StretchedSquare::className,"className" ); 
  if( StretchedSquare::className != "StretchedSquare" )
  {
    cout << "StretchedSquare::get ERROR in className!" << endl;
  }
  stretch[0].get( *subDir,"stretch[0]");
  stretch[1].get( *subDir,"stretch[1]");
  stretch[2].get( *subDir,"stretch[2]");
  Mapping::get( *subDir, "Mapping" );
  delete subDir;
  mappingHasChanged();
  return 0;
}

int StretchedSquare::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase *subDir = dir.virtualConstructor();      // create a derived data-base object
  dir.create(*subDir,name,"Mapping");                      // create a sub-directory 

  subDir->put( StretchedSquare::className,"className" );
  stretch[0].put( *subDir,"stretch[0]");
  stretch[1].put( *subDir,"stretch[1]");
  stretch[2].put( *subDir,"stretch[2]");
  Mapping::put( *subDir, "Mapping" );
  delete subDir;
  return 0;
}

Mapping *StretchedSquare::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==StretchedSquare::className )
    retval = new StretchedSquare();
  return retval;
}

    

//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int StretchedSquare::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  const int sizeOfMenu0=14;
  const int menuForStretchAxis0=1;
  aString menu0[sizeOfMenu0] = 
    {
      "!StretchedSquareMapping",
      "specify stretching along axis=0",
      "specify stretching along axis=1",
      "specify stretching along axis=2",
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
      "specify stretching along axis=0",
      "specify stretching along axis=1",
      "specify stretching along axis=2",
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

  aString answer,line;
  aString axisName[3];
  aString menu[sizeOfMenu0];
  // add domain axis labels if there exist
  int i,j=0;
  for( i=0; i<menuForStretchAxis0; i++ )
  {
    menu[j]=menu0[i];
    j++;
  }
  for( i=0; i<domainDimension; i++ )
  {
    axisName[i]=getName(mappingItemName(rangeAxis1Name+i));   // domainName == rangeName
    if( axisName[i]!="" )
      menu[j]=menu0[j]+aString(" (")+axisName[i]+aString(")");
    else
      menu[j]=menu0[j];
    j++;
  }
  for( i=menuForStretchAxis0+3; i<sizeOfMenu0; i++ )
  {
    menu[j]=menu0[i];
    j++;
  }
  menu[sizeOfMenu0-1]="";

  bool plotObject=TRUE;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  // parameters.set(GI_TOP_LABEL,"Stretched Unit Square");
  // set periodicity of the stretching mappings
  for( int axis=0; axis<domainDimension; axis++)
    stretch[axis].setIsPeriodic(getIsPeriodic(axis));

  gi.appendToTheDefaultPrompt("StretchedSquare>"); // set the default prompt

  int item;
  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
    {
      answer="plotObject";
      item=9;
    }
    else
      item=gi.getMenuItem(menu,answer)-menuForStretchAxis0;
 

    if( item>=0 && item < domainDimension ) // answer=="specify stretching along axis=0,1,2"
    {
      if( axisName[item]!="" )
	stretch[item].setName(Mapping::mappingName,
			      sPrintF(buff,"stretching function for axis=%s",(const char*)axisName[item]));
      else
	stretch[item].setName(Mapping::mappingName,sPrintF(buff,"stretching function for axis=%i",item));
      stretch[item].update(mapInfo);
      mappingHasChanged(); 
    }
    else if( answer=="show parameters" )
    {
      display();
    }
    else if( answer=="plot" )
    {
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
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
      if( answer=="periodicity" )
        for( int axis=0; axis<domainDimension; axis++)
          stretch[axis].setIsPeriodic(getIsPeriodic(axis));
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=TRUE;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
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
