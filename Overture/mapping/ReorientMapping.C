#include "ReorientMapping.h"
#include "MappingInformation.h"

ReorientMapping::
ReorientMapping(const int dir1_ /* =0 */, const int dir2_ /* =1 */, const int dir3_ /* =2 */,
                const int dimension /* =2 */ )
//===========================================================================
/// \brief  Default Constructor
///  The Reorient is a Mapping from {\tt parameter} space to {\tt parameter} space
///  that can be used to reorder the domain space variables $(r_1,r_2,r_3)$ and is defined by 
///  \begin{align*}
///      x(I,axis1) &= r(I,dir1) \\
///      x(I,axis2) &= r(I,dir2) \\
///      x(I,axis3) &= r(I,dir3)  
///  \end{align*}
/// \param dir1_,dir2_,dir3_ (input): 3 integers that are a permutation of (0,1,2)
/// \param dimension (input): define the domain and range dimension (which are equal).
/// 
//===========================================================================
: Mapping(dimension,dimension,parameterSpace,parameterSpace)   
{ 
  ReorientMapping::className="ReorientMapping";
  setName( Mapping::mappingName,"reorient");

  setOrientation(dir1_,dir2_,dir3_);
  
  for( int axis=0; axis<dimension; axis++ )
    setGridDimensions( axis,11 );

  setBasicInverseOption(canInvert);  // basicInverse is available
  inverseIsDistributed=false;

  setMappingCoordinateSystem( rectangular );  // for optimizing derivatives
  mappingHasChanged();

}

// Copy constructor is deep by default
ReorientMapping::
ReorientMapping( const ReorientMapping & map, const CopyType copyType )
{
  ReorientMapping::className="ReorientMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "ReorientMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

ReorientMapping::
~ReorientMapping()
{ if( debug & 4 )
  cout << " ReorientMapping::Destructor called" << endl;
}

ReorientMapping & ReorientMapping::
operator=( const ReorientMapping & X )
{
  if( ReorientMapping::className != X.getClassName() )
  {
    cout << "ReorientMapping::operator= ERROR trying to set a ReorientMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  dir1=X.dir1;
  dir2=X.dir2;
  dir3=X.dir3;

  return *this;
}

int ReorientMapping::
setOrientation(const int dir1_, const int dir2_, const int dir3_ /* =-1 */)
//===========================================================================
/// \brief  
///    Define the reordering of the domain space variables.
/// \param dir1_,dir2_,dir3_ (input): 3 integers that are a permutation of (0,1,2)
//===========================================================================
{
  dir1=dir1_;
  dir2=dir2_;
  dir3=dir3_;
  bool ok=true;
  if( domainDimension==2 )
  {
    if( (dir1==0 && dir2==1) || (dir1==1 && dir2==0 ) )
    {
      // ok
    }
    else
    {
      ok=false;
      printf("ReorientMapping::setOrientation:ERROR invalid values for (dir1,dir2)=(%i,%i)\n",dir1,dir2);
    }
  }
  else if( domainDimension==3 )
  {
    if( dir1>=0 && dir1<=2 && dir2>=0 && dir2<=2 && dir3>=0 && dir3<=2 && 
        dir1!=dir2 && dir1!=dir3 && dir2!=dir3 )
    {
      // ok
    }
    else
    {
      ok=false;
      printf("ReorientMapping::setOrientation:ERROR invalid values for (dir1,dir2,dir3)=(%i,%i,%i)\n",dir1,dir2,dir3);
    }
  }
  else
  {
    if( dir1!=0 )
      ok=false;
  }
  if( !ok )
  {
    dir1=0;
    dir2=1;
    dir3=2;
  }
  
  mappingHasChanged();
  return 0;
}

int ReorientMapping::
getOrientation(int & dir1_, int & dir2_, int & dir3_) const
//===========================================================================
/// \brief  
///     Return the current ordering.
/// \param dir1_,dir2_,dir3_ (output): 3 integers that are a permutation of (0,1,2)
//===========================================================================
{
  dir1_=dir1;
  dir2_=dir2;
  dir3_=dir3;
  return 0;
}


void ReorientMapping::
map( const realArray & r, realArray & x, realArray & xr,
                         MappingParameters & params )
{
  if( params.coordinateType != cartesian )
    cerr << "ReorientMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    x(I,axis1)=r(I,dir1); 
    if( domainDimension>1 )
      x(I,axis2)=r(I,dir2); 
    if( domainDimension>2 )
      x(I,axis3)=r(I,dir3); 
  }
  if( computeMapDerivative )
  {
    xr=0.;
    xr(I,axis1,dir1)=1.;
    if( domainDimension>1 )
      xr(I,axis2,dir2)=1.;
    if( domainDimension>2)
      xr(I,axis3,dir3)=1.;
  }
}

//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//=================================================================================
void ReorientMapping::
basicInverse( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    r(I,dir1)=x(I,axis1);
    if( domainDimension>1 )
      r(I,dir2)=x(I,axis2); 
    if( domainDimension>2 )
      r(I,dir3)=x(I,axis3); 
  }
  if( computeMapDerivative )
  {
    rx=0.;
    rx(I,dir1,axis1)=1.;
    if( domainDimension>1 )
      rx(I,dir2,axis2)=1.;
    if( domainDimension>2 )
      rx(I,dir3,axis3)=1.;
  }
}
  
void ReorientMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr,
                         MappingParameters & params )
{
  if( params.coordinateType != cartesian )
    cerr << "ReorientMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    x(I,axis1)=r(I,dir1); 
    if( domainDimension>1 )
      x(I,axis2)=r(I,dir2); 
    if( domainDimension>2 )
      x(I,axis3)=r(I,dir3); 
  }
  if( computeMapDerivative )
  {
    xr=0.;
    xr(I,axis1,dir1)=1.;
    if( domainDimension>1 )
      xr(I,axis2,dir2)=1.;
    if( domainDimension>2)
      xr(I,axis3,dir3)=1.;
  }
}

//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//=================================================================================
void ReorientMapping::
basicInverseS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    r(I,dir1)=x(I,axis1);
    if( domainDimension>1 )
      r(I,dir2)=x(I,axis2); 
    if( domainDimension>2 )
      r(I,dir3)=x(I,axis3); 
  }
  if( computeMapDerivative )
  {
    rx=0.;
    rx(I,dir1,axis1)=1.;
    if( domainDimension>1 )
      rx(I,dir2,axis2)=1.;
    if( domainDimension>2 )
      rx(I,dir3,axis3)=1.;
  }
}
  

//=================================================================================
// get a mapping from the database
//=================================================================================
int ReorientMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering ReorientMapping::get" << endl;

  subDir.get( ReorientMapping::className,"className" ); 
  if( ReorientMapping::className != "ReorientMapping" )
  {
    cout << "ReorientMapping::get ERROR in className!" << endl;
  }
  subDir.get( dir1,"dir1" );
  subDir.get( dir2,"dir2" );
  subDir.get( dir3,"dir3" );

  Mapping::get( subDir, "Mapping" );
  delete &subDir;
  mappingHasChanged();
  return 0;
}
int ReorientMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( ReorientMapping::className,"className" );
  subDir.put( dir1,"dir1" );
  subDir.put( dir2,"dir2" );
  subDir.put( dir3,"dir3" );
  Mapping::put( subDir, "Mapping" );
  delete &subDir;
  return 0;
}

Mapping *ReorientMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==ReorientMapping::className )
    retval = new ReorientMapping();
  return retval;
}

    

//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int ReorientMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      // "specify corners", 
      "!RestictionMapping",
      "set orientation",
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
      "set orientation    : specify a new orientation",
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
  parameters.set(GI_TOP_LABEL,"Restricted Unit Square");

  gi.appendToTheDefaultPrompt("Reorient>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="set orientation" ) 
    {
      if( domainDimension==2 )
      {
        gi.inputString(line,sPrintF(buff,"Enter dir1,dir2 (current=(%i,%i)): ",dir1,dir2));
        if( line!="" ) sScanF(line,"%i %i",&dir1,&dir2);
      }
      else
      {
        gi.inputString(line,sPrintF(buff,"Enter dir1,dir2,dir3 (current=(%i,%i,%i)): ",dir1,dir2,dir3));
        if( line!="" ) sScanF(line,"%i %i %i",&dir1,&dir2,&dir3);
      }
      setOrientation(dir1,dir2,dir3);
      mappingHasChanged();
    }
    else if( answer=="show parameters" )
    {
      printf(" (dir1,dir2,dir3)=(%i,%i,%i)\n",dir1,dir2,dir3);
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
