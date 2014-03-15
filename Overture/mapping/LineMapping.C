#include "LineMapping.h"
#include "MappingInformation.h"

LineMapping::
LineMapping(const real xa_, 
	    const real xb_, 
	    const int numberOfGridPoints )
: Mapping(1,1,parameterSpace,cartesianSpace)   
//===========================================================================
/// \details  Build a mapping for a line in 1D.
/// \param xa_, xb_ (input) : End points of the interval.
//===========================================================================
{ 
  LineMapping::className="LineMapping";
  setName( Mapping::mappingName,"line");
  setGridDimensions( axis1,numberOfGridPoints );
  xa=xa_; ya=0.; za=0.; 
  xb=xb_; yb=0.; zb=0.; 
  
  setBasicInverseOption(canInvert);  // basicInverse is available
  inverseIsDistributed=false;

  setMappingCoordinateSystem( rectangular );  // for optimizing derivatives
  mappingHasChanged();
}


LineMapping::
LineMapping(const real xa_,const real ya_, 
	    const real xb_,const real yb_,
	    const int numberOfGridPoints)
: Mapping(1,2,parameterSpace,cartesianSpace)   
//===========================================================================
/// \details  Build a mapping for a line in 2D.
/// \param xa_, ya_, xb_, yb_ (input) : End points of the line.
//===========================================================================
{
  LineMapping::className="LineMapping";
  setName( Mapping::mappingName,"line");
  setGridDimensions( axis1,numberOfGridPoints );
  xa=xa_; ya=ya_; za=0.;
  xb=xb_; yb=yb_; zb=0.;
// **** add this setBasicInverseOption(canInvert);  // basicInverse is available
  setMappingCoordinateSystem( rectangular );  // for optimizing derivatives
  mappingHasChanged();
}

LineMapping::
LineMapping(const real xa_,const real ya_,const real za_, 
	    const real xb_,const real yb_,const real zb_,
	    const int numberOfGridPoints)
: Mapping(1,3,parameterSpace,cartesianSpace)   
//===========================================================================
/// \details  Build a mapping for a line in 3D.
/// \param xa_, ya_,za_,  xb_, yb_,zb_ (input) : End points of the line.
//===========================================================================
{
  LineMapping::className="LineMapping";
  setName( Mapping::mappingName,"line");
  setGridDimensions( axis1,numberOfGridPoints );
  xa=xa_; ya=ya_; za=za_;
  xb=xb_; yb=yb_; zb=zb_;
// **** add this   setBasicInverseOption(canInvert);  // basicInverse is available
  setMappingCoordinateSystem( rectangular );  // for optimizing derivatives
  mappingHasChanged();
}


// Copy constructor is deep by default
LineMapping::
LineMapping( const LineMapping & map, const CopyType copyType )
{
  LineMapping::className="LineMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "LineMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

LineMapping::
~LineMapping()
{ if( debug & 4 )
  cout << " LineMapping::Destructor called for " << getName(mappingName) << endl;
}

LineMapping & LineMapping::
operator =( const LineMapping & X0 )
{
  if( LineMapping::className != X0.getClassName() )
  {
    cout << "LineMapping::operator= ERROR trying to set a LineMapping = to a" 
      << " mapping of type " << X0.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X0);            // call = for derivee class
  LineMapping & X = (LineMapping&) X0;  // cast to a Line mapping
  xa=X.xa;
  ya=X.ya;
  za=X.za;
  xb=X.xb;
  yb=X.yb;
  zb=X.zb;
  return *this;
}

int LineMapping::
getPoints( real & xa_, real & xb_ ) const
//===========================================================================
/// \details  Get the end points of the line.
/// \param xa_, xb_ (output) : End points of the line.
//===========================================================================
{
  xa_=xa;
  xb_=xb;
  return 0;
}

int LineMapping::
getPoints( real & xa_, real & ya_,
	   real & xb_, real & yb_ ) const
//===========================================================================
/// \details  Get the end points of the line.
/// \param xa_, ya_, xb_, yb_ (output) : End points of the line.
//===========================================================================
{
  xa_=xa;
  xb_=xb;
  ya_=ya;
  yb_=yb;
  return 0;
}

int LineMapping::
getPoints( real & xa_, real & ya_, real & za_, 
	   real & xb_, real & yb_, real & zb_ ) const
//===========================================================================
/// \details  Get the end points of the line.
/// \param xa_, ya_,za_,  xb_, yb_,zb_ (output) : End points of the line.
//===========================================================================
{
  xa_=xa;
  xb_=xb;
  ya_=ya;
  yb_=yb;
  za_=za;
  zb_=zb;
  return 0;
}



int LineMapping::
setPoints( const real & xa_, const real & xb_ )
//===========================================================================
/// \details  Specify the end points for a line in 1D.
/// \param xa_, xb_ (input) : End points of the interval.
//===========================================================================
{
  rangeDimension=1;
  xa=xa_;
  xb=xb_;
  mappingHasChanged();
  return 0;
}

int LineMapping:: 
setPoints( const real & xa_, const real & ya_,
	   const real & xb_, const real & yb_ )
//===========================================================================
/// \details  Specify the end points for a line in 2D.
/// \param xa_, ya_, xb_, yb_ (input) : End points of the line.
//===========================================================================
{
  rangeDimension=2;
  xa=xa_;
  xb=xb_;
  ya=ya_;
  yb=yb_;
  mappingHasChanged();
  return 0;
}

int LineMapping:: 
setPoints( const real & xa_, const real & ya_, const real & za_, 
           const real & xb_, const real & yb_, const real & zb_ )
//===========================================================================
/// \details  Specify the end points for a line in 3D.
/// \param xa_, ya_,za_,  xb_, yb_,zb_ (input) : End points of the line.
//===========================================================================
{
  rangeDimension=3;
  xa=xa_;
  xb=xb_;
  ya=ya_;
  yb=yb_;
  za=za_;
  zb=zb_;
  mappingHasChanged();
  return 0;
}



void LineMapping::
map(const realArray & r, 
    realArray & x, 
    realArray & xr,
    MappingParameters & params )
// ===========================================================================================
// /Description:
//    Here is the mapping for a line
// The line is defined as 
//      x = xa + (xb-xa) * r
// ===========================================================================================
{
  if( params.coordinateType != cartesian )
    cerr << "LineMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    x(I,axis1)=(xb-xa)*r(I,axis1)+xa; 
    if( rangeDimension > 1 )
      x(I,axis2)=(yb-ya)*r(I,axis1)+ya; 
    if( rangeDimension > 2 )
      x(I,axis3)=(zb-za)*r(I,axis1)+za; 
  }
  if( computeMapDerivative )
  {
    xr(I,axis1,axis1)=xb-xa;
    if( rangeDimension > 1 )
      xr(I,axis2,axis1)=yb-ya;
    if( rangeDimension > 2 )
      xr(I,axis3,axis1)=zb-za;
  }
}

//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//=================================================================================
void LineMapping::
basicInverse( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
// ==========================================================================================
// The line is defined as 
//      x = xa + (xb-xa) * r
// The inverse is
//    r= (x-xa).(xb-xa) / \| xb-xa \|^2 
// ==========================================================================================
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    if( rangeDimension==1 )
      r(I,axis1)=(x(I,axis1)-xa)/(xb-xa); 
    else if( rangeDimension==2 )
      r(I,axis1)=( (x(I,axis1)-xa)*(xb-xa)+(x(I,axis2)-ya)*(yb-ya) )*(1./( SQR(xb-xa)+SQR(yb-ya) ));
    else
      r(I,axis1)=( (x(I,axis1)-xa)*(xb-xa)+(x(I,axis2)-ya)*(yb-ya)+(x(I,axis3)-za)*(zb-za) )*
                    (1./( SQR(xb-xa)+SQR(yb-ya)+SQR(zb-za) ));
    periodicShift(r,I);   // shift r in any periodic directions
  }
  if( computeMapDerivative )
  {
    const real eps=10.*REAL_MIN;
    rx(I,axis1,axis1)=1./max(eps,xb-xa);
    if( rangeDimension > 1 )
      rx(I,axis1,axis2)=1./max(eps,yb-ya);
    if( rangeDimension > 2 )
      rx(I,axis1,axis3)=1./max(eps,zb-za);
  }
}
  
void LineMapping::
mapS(const RealArray & r, 
    RealArray & x, 
    RealArray & xr,
    MappingParameters & params )
// ===========================================================================================
// /Description:
//    Here is the mapping for a line
// The line is defined as 
//      x = xa + (xb-xa) * r
// ===========================================================================================
{
  if( params.coordinateType != cartesian )
    cerr << "LineMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    x(I,axis1)=(xb-xa)*r(I,axis1)+xa; 
    if( rangeDimension > 1 )
      x(I,axis2)=(yb-ya)*r(I,axis1)+ya; 
    if( rangeDimension > 2 )
      x(I,axis3)=(zb-za)*r(I,axis1)+za; 
  }
  if( computeMapDerivative )
  {
    xr(I,axis1,axis1)=xb-xa;
    if( rangeDimension > 1 )
      xr(I,axis2,axis1)=yb-ya;
    if( rangeDimension > 2 )
      xr(I,axis3,axis1)=zb-za;
  }
}

//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//=================================================================================
void LineMapping::
basicInverseS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
// ==========================================================================================
// The line is defined as 
//      x = xa + (xb-xa) * r
// The inverse is
//    r= (x-xa).(xb-xa) / \| xb-xa \|^2 
// ==========================================================================================
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    if( rangeDimension==1 )
      r(I,axis1)=(x(I,axis1)-xa)/(xb-xa); 
    else if( rangeDimension==2 )
      r(I,axis1)=( (x(I,axis1)-xa)*(xb-xa)+(x(I,axis2)-ya)*(yb-ya) )*(1./( SQR(xb-xa)+SQR(yb-ya) ));
    else
      r(I,axis1)=( (x(I,axis1)-xa)*(xb-xa)+(x(I,axis2)-ya)*(yb-ya)+(x(I,axis3)-za)*(zb-za) )*
                    (1./( SQR(xb-xa)+SQR(yb-ya)+SQR(zb-za) ));
    periodicShift(r,I);   // shift r in any periodic directions
  }
  if( computeMapDerivative )
  {
    const real eps=10.*REAL_MIN;
    rx(I,axis1,axis1)=1./max(eps,xb-xa);
    if( rangeDimension > 1 )
      rx(I,axis1,axis2)=1./max(eps,yb-ya);
    if( rangeDimension > 2 )
      rx(I,axis1,axis3)=1./max(eps,zb-za);
  }
}
  

//=================================================================================
// get a mapping from the database
//=================================================================================
int LineMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering LineMapping::get" << endl;

  subDir.get( LineMapping::className,"className" ); 
  if( LineMapping::className != "LineMapping" )
  {
    cout << "LineMapping::get ERROR in className!" << endl;
  }
  subDir.get( xa,"xa" );
  subDir.get( ya,"ya" );
  subDir.get( za,"za" );
  subDir.get( xb,"xb" );
  subDir.get( yb,"yb" );
  subDir.get( zb,"zb" );
  Mapping::get( subDir, "Mapping" );
  mappingHasChanged();
  delete &subDir;
  return 0;
}

int LineMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( LineMapping::className,"className" );

  subDir.put( xa,"xa" );
  subDir.put( ya,"ya" );
  subDir.put( za,"za" );
  subDir.put( xb,"xb" );
  subDir.put( yb,"yb" );
  subDir.put( zb,"zb" );
  Mapping::put( subDir, "Mapping" );

  delete &subDir;
  return 0;
}

Mapping *LineMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=NULL;
  if( mappingClassName==LineMapping::className )
    retval = new LineMapping();
  return retval;
}


//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int LineMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!LineMapping",
      "set end points",
      "number of dimensions",
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
      "set end points : Specify the end points of the line",
      "number of dimensions: the line is in 1D, 2D or 3D",
      " ",
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

  gi.appendToTheDefaultPrompt("Line>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="set end points" ) 
    {
      if( getRangeDimension()==1 )
      {
	gi.inputString(line,sPrintF(buff,"Enter xa,xb (default=(%e,%e)): ",xa,xb));
	if( line!="" ) sScanF(line,"%e %e ",&xa,&xb);
      }
      else if( getRangeDimension()==2 )
      {
	gi.inputString(line,sPrintF(buff,"Enter xa,xb, ya,yb (default=(%e,%e,%e,%e)): ",
				     xa,xb,ya,yb));
	if( line!="" ) sScanF(line,"%e %e %e %e ",&xa,&xb,&ya,&yb);
      }
      else 
      {
	gi.inputString(line,sPrintF(buff,"Enter xa,xb, ya,yb, za,zb (default=(%e,%e,%e,%e,%e,%e)): ",
				     xa,xb,ya,yb,za,zb));
	if( line!="" ) sScanF(line,"%e %e %e %e %e %e",&xa,&xb,&ya,&yb,&za,&zb);
      }
      mappingHasChanged();
    }
    else if( answer=="specify end points" ) 
    {
      if( getRangeDimension()==1 )
      {
	gi.inputString(line,sPrintF(buff,"Enter xa,xb (default=(%e,%e)): ",xa,xb));
	if( line!="" ) sScanF(line,"%e %e ",&xa,&xb);
      }
      else if( getRangeDimension()==2 )
      {
	gi.inputString(line,sPrintF(buff,"Enter xa,ya,xb,yb (default=(%e,%e,%e,%e)): ",
				     xa,ya,xb,yb));
	if( line!="" ) sScanF(line,"%e %e %e %e ",&xa,&ya,&xb,&yb);
      }
      else 
      {
	gi.inputString(line,sPrintF(buff,"Enter xa,ya,za,xb,yb,zb (default=(%e,%e,%e,%e,%e,%e)): ",
				     xa,ya,za,xb,yb,zb));
	if( line!="" ) sScanF(line,"%e %e %e %e %e %e",&xa,&ya,&za,&xb,&yb,&zb);
      }
      mappingHasChanged();
    }
    else if( answer=="number of dimensions" ) 
    {
      int nd=getRangeDimension();
      gi.inputString(line,sPrintF(buff,"Enter number of dimensions (default=(%i)): ",nd));
      if( line!="" )
      {
        sScanF(line,"%i ",&nd);
	setRangeDimension(nd);
      }
      mappingHasChanged();
    }
    else if( answer=="show parameters" )
    {
      if( rangeDimension==1 )
	printf(" (xa)=(%e) -> (xb)=(%e)\n",xa,xb);
      else if( rangeDimension==2 )
	printf(" (xa,ya)=(%e,%e) -> (xb,yb)=(%e,%e)\n",xa,ya,xb,yb);
      else     
	printf(" (xa,ya,za)=(%e,%e,%e) -> (xb,yb,zb)=(%e,%e,%e)\n",xa,ya,za, xb,yb,zb);

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

