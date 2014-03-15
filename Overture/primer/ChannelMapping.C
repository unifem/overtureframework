#include "ChannelMapping.h"
#include "MappingInformation.h"

ChannelMapping::
ChannelMapping( const real xa0, const real xb0, const real ya0, const real yb0 ) 
// ====================================================================================
// Constructor for the Channel Mapping.
//   A ChannelMapping is a Mapping from R^2 -> R^2 from
//   parameterSpace to cartesianSpace
// ====================================================================================
: Mapping(2,2,parameterSpace,cartesianSpace)   
{ 
  ChannelMapping::className="ChannelMapping";
  setName( Mapping::mappingName,"channel");
  setGridDimensions( axis1,11 );
  setGridDimensions( axis2,11 );
  xa=xa0;
  xb=xb0;
  ya=ya0;
  yb=yb0;
}

ChannelMapping::
ChannelMapping( const ChannelMapping & map, const CopyType copyType )
// ====================================================================================
// Copy constructor is a deep copy by default
// The copy constructor is called when passing by a mapping by value.
// ====================================================================================
{
  ChannelMapping::className="ChannelMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "ChannelMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

ChannelMapping::
~ChannelMapping()
// ====================================================================================
// Destructor
// ====================================================================================
{ if( debug & 4 )
  cout << " ChannelMapping::Desctructor called" << endl;
}

ChannelMapping & ChannelMapping::
operator=( const ChannelMapping & X )
// ==========================================================================
// Equals operator, set Mapping X equal to this mapping
// ==========================================================================
{
  if( ChannelMapping::className != X.getClassName() )
  {
    cout << "ChannelMapping::operator= ERROR trying to set a ChannelMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  xa=X.xa;
  ya=X.ya;
  xb=X.xb;
  yb=X.yb;
  return *this;
}

void ChannelMapping::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
// ==========================================================================
// Here is the transformation that defines the mapping.
//
//   r : (input) r(0:1,base:bound) - evaluate the mapping at these points
//   x : (output) - if x has enough space, x(0:1,base:bound), then compute
//       the mapping. Do not compute the mapping if x is not large enough
//   xr : (output) - if xr has enough space, xr(0:1,0:1,base:bound), then
//       compute the derivatives of the mapping.
//   params : (input) - holds parameters for the mapping.
// ==========================================================================
{
  if( params.coordinateType != cartesian )
    cerr << "ChannelMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    x(I,axis1)=(xb-xa)*r(I,axis1)+xa; 
    x(I,axis2)=(yb-ya)*r(I,axis2)+ya; 
  }
  if( computeMapDerivative )
  {
    xr(I,axis1,axis1)=xb-xa;
    xr(I,axis1,axis2)=0.;
    xr(I,axis2,axis1)=0.;
    xr(I,axis2,axis2)=yb-ya;
  }
}


int ChannelMapping::
get( const GenericDataBase & dir, const aString & name )
//=================================================================================
// get a mapping from a database
// 
//  dir : (input) - dataBase directory
//  name : (input) - get the mapping from a sub-directory called name. If name="."
//               get the mapping in dir. 
//=================================================================================
{
  // Use directory dir if name="."
  if( debug & 4 )
    cout << "Entering ChannelMapping::get" << endl;

  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");

  subDir.get( ChannelMapping::className,"className" ); 
  if( ChannelMapping::className != "ChannelMapping" )
  {
    cout << "ChannelMapping::get ERROR in className!" << endl;
  }
  subDir.get( xa,"xa" );
  subDir.get( ya,"ya" );
  subDir.get( xb,"xb" );
  subDir.get( yb,"yb" );
  Mapping::get( subDir, "Mapping" );

  delete &subDir;
  return 0;
}

int ChannelMapping::
put( GenericDataBase & dir, const aString & name ) const
//=================================================================================
// put a mapping into the database
//
//  dir : (input) - dataBase directory
//  name : (input) - save the mapping in a sub-directory called name. If name="."
//               save the mapping in dir. This sub-directory will be created.
//=================================================================================
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( ChannelMapping::className,"className" );
  subDir.put( xa,"xa" );
  subDir.put( ya,"ya" );            
  subDir.put( xb,"xb" );
  subDir.put( yb,"yb" );            
  Mapping::put( subDir, "Mapping" );
  delete &subDir;
  
  return 0;
}

Mapping *ChannelMapping::
make( const aString & mappingClassName )
//========================================================================
// Make a new mapping if the mappingClassName is the name of this Class
// This function can be used to create Mappings from a data-base without
// knowing the exact mapping class.
// ========================================================================
{
  Mapping *retval=0;
  if( mappingClassName==ChannelMapping::className )
    retval = new ChannelMapping();
  return retval;
}

    

int ChannelMapping::
update( MappingInformation & mapInfo ) 
//=============================================================================
//   Prompt for changes to parameters
//   Interact with the user through a graphics interface
//
// mapInfo : (input) - contains a graphics interface to use.
//=============================================================================
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "specify corners",
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
      "specify corners    : Specify the corners of the channel",
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

  bool plotObject=TRUE;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="specify corners" ) 
    {
      gi.inputString(line,sPrintF(buff,"Enter xa,ya,xb,yb (default=(%e,%e,%e,%e)): ",xa,ya,xb,yb));
      if( line!="" ) sscanf(line,"%e %e %e %e ",&xa,&ya,&xb,&yb);
    }
    else if( answer=="show parameters" )
    {
      printf(" (xa,ya,xb,yb)=(%e,%e,%e,%e)\n",xa,ya,xb,yb);
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
      gi.erase();
      PlotIt::plot(gi,*this,parameters);   // *** recompute every time ?? ***

    }
  }
  gi.erase();
  return 0;
}
