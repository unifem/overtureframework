#include "PolynomialMapping.h"
#include "MappingInformation.h"


PolynomialMapping::
PolynomialMapping( int numberOfDimensions /* =2 */ )
: Mapping(numberOfDimensions,numberOfDimensions,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief  Build a mapping defined by a polynomial -- this can be used for
///   testing PDE solvers since the Mapping derivatives can be computed exactly
///   if the scheme is accurate enough.
//===========================================================================
{ 
  PolynomialMapping::className="PolynomialMapping";
  setName( Mapping::mappingName,"polynomial");
  setGridDimensions( axis1,11 );
  setGridDimensions( axis2,11 );

  pc.redim(3,3,2);
  pc=0.;
  
  pc(1,0,0)=.75;  // x= .75*r + .25*r^2
  pc(2,0,0)=.25;

  pc(0,1,1)=.75;  // y= .75*s + .25*s^2
  pc(0,2,1)=.25;

  mappingHasChanged();

}

// Copy constructor is deep by default
PolynomialMapping::
PolynomialMapping( const PolynomialMapping & map, const CopyType copyType )
{
  PolynomialMapping::className="PolynomialMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "PolynomialMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

PolynomialMapping::
~PolynomialMapping()
{ if( debug & 4 )
  cout << " PolynomialMapping::Destructor called" << endl;
}

PolynomialMapping & PolynomialMapping::
operator=( const PolynomialMapping & x )
{
  if( PolynomialMapping::className != x.getClassName() )
  {
    cout << "PolynomialMapping::operator= ERROR trying to set a PolynomialMapping = to a" 
      << " mapping of type " << x.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(x);            // call = for derivee class
  pc=x.pc;
  return *this;
}

void PolynomialMapping::
map( const realArray & r, realArray & x, realArray & xr,
                         MappingParameters & params )
{
  if( params.coordinateType != cartesian )
    cerr << "PolynomialMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    x(I,axis1)=pc(0,0,0)+r(I,axis1)*(pc(1,0,0)+r(I,axis1)*pc(2,0,0));
    x(I,axis2)=pc(0,0,1)+r(I,axis2)*(pc(0,1,1)+r(I,axis2)*pc(0,2,1));
    if( rangeDimension==3 )
      x(I,axis3)=r(I,axis3);
  }
  if( computeMapDerivative )
  {
    xr(I,axis1,axis1)=pc(1,0,0)+r(I,axis1)*(2.*pc(2,0,0));
    xr(I,axis1,axis2)=0.;
    xr(I,axis2,axis1)=0.;
    xr(I,axis2,axis2)=pc(0,1,1)+r(I,axis2)*(2.*pc(0,2,1));
    if( rangeDimension==3 )
      xr(I,axis3,Range(0,1))=1.;
  }
}


//=================================================================================
// get a mapping from the database
//=================================================================================
int PolynomialMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");

  subDir.get( PolynomialMapping::className,"className" ); 
  if( PolynomialMapping::className != "PolynomialMapping" )
  {
    cout << "PolynomialMapping::get ERROR in className!" << endl;
  }
  subDir.get( pc,"pc" );
  Mapping::get( subDir, "Mapping" );
  delete &subDir;
  mappingHasChanged();
  return 0; 
}

int PolynomialMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( PolynomialMapping::className,"className" );
  subDir.put( pc,"pc" );
  Mapping::put( subDir, "Mapping" );
  delete &subDir;
  return 0;
}

Mapping *PolynomialMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==PolynomialMapping::className )
    retval = new PolynomialMapping();
  return retval;
}

void PolynomialMapping::
getCoefficients(RealArray & coeff) const
//===========================================================================
/// \details  return the coeff's of the polynomial.
//===========================================================================
{
  coeff.redim(pc);
  coeff=pc;
}

void PolynomialMapping::
setCoefficients(const RealArray & coeff) 
//===========================================================================
/// \details  set the coeff's of the polynomial.
//===========================================================================
{
  pc.redim(coeff);
  pc=coeff;
}


    

//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int PolynomialMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!PolynomialMapping",
//      "set corners",
      " ",
      "mapping parameters",
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
//      "set corners        : Specify the corners of the polynomial",
      "mapping parameters : change lines, boundary conditions etc.",
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

  gi.appendToTheDefaultPrompt("Polynomial>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="set corners" ) 
    {
//       gi.inputString(line,sPrintF(buff,"Enter xa,xb, ya,yb (default=[%e,%e]x[%e,%e]): ",
//           xa,xb,ya,yb));
//       if( line!="" ) sScanF(line,"%e %e %e %e ",&xa,&xb,&ya,&yb);
//       setVertices(xa,xb,ya,yb);
      
//       mappingHasChanged(); 
    }
    else if( updateWithCommand(mapInfo, answer) )
    {
      // changes were made to generic mapping parameters such as lines, BC's, share, periodicity
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
    else if( answer=="show parameters" )
    {
      pc.display("pc");
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
