#include "CircleMapping.h"
#include "MappingInformation.h"

CircleMapping:: 
CircleMapping(const real & x_ /* =0. */, 
	      const real & y_ /* =0. */, 
	      const real & a_ /* =1. */, 
	      const real & b_ /* =a_ */,
	      const real & startTheta_ /* =0. */, 
	      const real & endTheta_ /* =1. */) 
   : Mapping(1,2,parameterSpace,cartesianSpace)   
//===========================================================================
/// \details  
///    Define a circle or ellipse (or an arc there-of) in 2D, 
///    semi-axes a, and b, angle from startTheta*twoPi to endTheta*twoPi
/// 
///  \begin{verbatim}
///     x(I,axis1)=a*cos(thetaFactor*(r(I,axis1)-startTheta))+xa; 
///     x(I,axis2)=b*sin(thetaFactor*(r(I,axis1)-startTheta))+ya;
///  \end{verbatim}
/// 
/// \param x_ (input) : x coordinate of center
/// \param y_ (input) : y coordinate of center
/// \param a_ (input) : length of semi axis along x (radius for a circle)
/// \param b_ (input) : length of semi axis along y (radius for a circle)
/// \param startTheta_ (input): starting angle (in units of radians/(2 pi))
/// \param endTheta_ (input): ending angle (in units of radians/(2 pi))
//===========================================================================
{
  initialize(x_,y_,0.,a_,b_,startTheta_,endTheta_);
}

CircleMapping:: 
CircleMapping(const real & x_, 
	      const real & y_, 
	      const real & z_, 
	      const real & a_, 
	      const real & b_, 
	      const real & startTheta_, 
	      const real & endTheta_ )
   : Mapping(1,3,parameterSpace,cartesianSpace)   
//===========================================================================
/// \details  
///    Define a circle or ellipse (or an arc there-of) in 3D (constant z), 
///    semi-axes a, and b, angle from startTheta*twoPi to endTheta*twoPi
/// 
///  \begin{verbatim}
///     x(I,axis1)=a*cos(thetaFactor*(r(I,axis1)-startTheta))+xa; 
///     x(I,axis2)=b*sin(thetaFactor*(r(I,axis1)-startTheta))+ya;
///     x(I,axis3)=za;
///  \end{verbatim}
/// 
/// \param x_ (input) : x coordinate of center
/// \param y_ (input) : y coordinate of center
/// \param z_ (input) : z coordinate of center
/// \param a_ (input) : length of semi axis along x (radius for a circle)
/// \param b_ (input) : length of semi axis along y (radius for a circle)
/// \param startTheta_ (input): starting angle (in units of radians/(2 pi))
/// \param endTheta_ (input): ending angle (in units of radians/(2 pi))
//===========================================================================
{
  initialize(x_,y_,z_,a_,b_,startTheta_,endTheta_);
}

void CircleMapping:: 
initialize(const real & x_, 
	   const real & y_, 
	   const real & z_, 
	   const real & a_, 
	   const real & b_, 
	   const real & startTheta_, 
	   const real & endTheta_ )
{
  CircleMapping::className="CircleMapping";
  setName( Mapping::mappingName,"circle");
  setGridDimensions( axis1,21 );  // gridlines for plotting

  xa=x_; ya=y_; za=z_;
  a=a_;
  b= b_==-1. ? a : b_; // by default b=a;
  if( a==0. || b==0. )
  {
    cout << "CircleMapping::constructor:Error: and b must be non-zero, a=" << a << ", b=" << b << endl;
    cout << "CircleMapping::constructor:changing value to 1\n";
    if( a==0. )
      a=1.;
    if( b==0. )
      b=1.;
  }
  startTheta=startTheta_;
  endTheta=endTheta_;
  
  if( fabs(endTheta-startTheta)>1.+ REAL_EPSILON*10. )
  {
    cout << "CircleMapping::ERROR(?) endTheta-startTheta > 1 so the circle overlaps on itself \n";
    cout << "endTheta and startTheta are in units of radians/(2 pi) \n";
  }
  

  setBasicInverseOption( canInvert );    // inverse gives the closest point
  inverseIsDistributed=false;

  if( fabs(endTheta-startTheta-1.)<REAL_EPSILON*10. ) 
  {
    setIsPeriodic(axis1, functionPeriodic );  
    setBoundaryCondition( Start,axis1,-1 );
    setBoundaryCondition( End  ,axis1,-1 );
  }
  mappingHasChanged();
}

  // Copy constructor is deep by default
CircleMapping:: 
CircleMapping( const CircleMapping & map, const CopyType copyType )
{
  CircleMapping::className="CircleMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "CircleMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

CircleMapping:: 
~CircleMapping()
{ if( (debug/4) % 2 )
    cout << " CircleMapping::Desctructor called" << endl;
}

CircleMapping & CircleMapping:: 
operator =( const CircleMapping & X )
{
  if( CircleMapping::className != X.getClassName() )
  {
    cout << "CircleMapping::operator= ERROR trying to set a CircleMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  a=X.a;
  b=X.b;
  xa=X.xa;
  ya=X.ya;
  return *this;
}

void CircleMapping:: 
map(const realArray & r, realArray & x, realArray & xr, MappingParameters & params)
{
  if( params.coordinateType != cartesian )
    cerr << "CircleMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  real thetaFactor=twoPi*(endTheta-startTheta);

  const realArray & cosTheta=evaluate(cos(thetaFactor*r(I,axis1)+(twoPi*startTheta)));
  const realArray & sinTheta=evaluate(sin(thetaFactor*r(I,axis1)+(twoPi*startTheta)));
  
  if( computeMap )
  {
    x(I,axis1)=a*cosTheta+xa; 
    x(I,axis2)=b*sinTheta+ya;
    if( rangeDimension==3 )
      x(I,axis3)=za;
  }
  if( computeMapDerivative )
  {
    xr(I,axis1,axis1)=-a*thetaFactor*sinTheta;
    xr(I,axis2,axis1)= b*thetaFactor*cosTheta;
    if( rangeDimension==3 )
      xr(I,axis3,axis1)=0.;
  }
}

//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//=================================================================================
void CircleMapping::
basicInverse( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  assert( fabs(endTheta-startTheta)>0. );

  real theta0=twoPi*startTheta, theta1=twoPi*endTheta;

  real inverseScale=1./(theta1-theta0);

  assert( a!=0. && b!=0. );
  real aInverse=1./a;
  real bInverse=1./b;
  
  if( computeMap )
  {
    if( getIsPeriodic(axis1) )
    {
      // ***NOTE evaluate atan2(-y/-x) gives theta +/- pi
      r(I,axis1)=atan2(evaluate((ya-x(I,axis2))*bInverse),
                       evaluate((xa-x(I,axis1))*aInverse));  // **NOTE** (-y,-x) : result in [-pi,pi]
      r(I,axis1)=( r(I,axis1)+(Pi-theta0) )*inverseScale;
      r(I,axis1)=fmod(r(I,axis1)+1.,1.);  // map back to [0,1]
    }
    else
    {
      // if we are not periodic then we shift the angle to [theta0-delta,theta1+delta] where
      // delta is the gap in the angle between the start and end of the annulus
      r(I,axis1)=atan2(evaluate((x(I,axis2)-ya)*bInverse),
                       evaluate((x(I,axis1)-xa)*aInverse));  // **NOTE** +theta : result in [-pi,pi]
      real delta = (1.-(endTheta-startTheta))*Pi;
      where ( r(I,axis1) < theta0 - delta )
      {
	r(I,axis1)+=twoPi;    // now the angle in is the range [theta0-delta,theta1+delta]
      }
      r(I,axis1)=(r(I,axis1)-theta0)*inverseScale;
    }
//     r(I,axis1)=( atan2(evaluate((ya-x(I,axis2))*bInverse),evaluate((xa-x(I,axis1))*aInverse))
//                +(Pi-twoPi*startTheta) )*inverseScale;
  }
  if( computeMapDerivative )
  {
    rx(I,axis1,axis1)= (inverseScale*b*aInverse)*(ya-x(I,axis2));
    rx(I,axis1,axis2)= (inverseScale*a*bInverse)/(x(I,axis1)-xa);
    if( rangeDimension==3 )
      rx(I,axis1,axis3)=0.;
  }
}

// get a mapping from the database
int CircleMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase *subDir = dir.virtualConstructor();
  dir.find(*subDir,name,"Mapping");
  if( (debug/4) % 2 )
    cout << "Entering CircleMapping::get" << endl;

  subDir->get( CircleMapping::className,"className" ); 
  if( CircleMapping::className != "CircleMapping" )
  {
    cout << "CircleMapping::get ERROR in className!" << endl;
    cout << "className from the database = " << CircleMapping::className << endl;
  }
  
  subDir->get( a,"a" );
  subDir->get( b,"b" );
  subDir->get( xa,"xa" );
  subDir->get( ya,"ya" );
  subDir->get( za,"za" );
  subDir->get( startTheta,"startTheta" );
  subDir->get( endTheta,"endTheta" );

  Mapping::get( *subDir, "Mapping" );
  mappingHasChanged();
  delete subDir;
  return 0;
}

int CircleMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase *subDir = dir.virtualConstructor();      // create a derived data-base object
  dir.create(*subDir,name,"Mapping");                      // create a sub-directory 

  subDir->put( CircleMapping::className,"className" );
  subDir->put( a,"a" );
  subDir->put( b,"b" );
  subDir->put( xa,"xa" );
  subDir->put( ya,"ya" );            
  subDir->put( za,"za" );
  subDir->put( startTheta,"startTheta" );
  subDir->put( endTheta,"endTheta" );
  Mapping::put( *subDir, "Mapping" );
  delete subDir;
  return 0;
}

Mapping* CircleMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==CircleMapping::className )
    retval = new CircleMapping();
  return retval;
}

aString CircleMapping::
getClassName() const
{
  return CircleMapping::className;
}


//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int CircleMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!CircleMapping",
      "specify radius of the circle",
      "specify axes of the ellipse",
      "specify centre",
      "specify start/end angles",
      "set range dimension (2 or 3)",
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
      "specify corners    : Specify the corners of the square",
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

  gi.appendToTheDefaultPrompt("Circle>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 
    if( answer=="specify radius of the circle" ) 
    {
      gi.inputString(line,sPrintF(buff,"Enter the radius of the circle (default=%e): ",a));
      if( line!="" ) sScanF(line,"%e",&a);
      b=a;
      mappingHasChanged();
    }
    else if( answer=="specify axes of the ellipse" )
    {
      gi.inputString(line,sPrintF(buff,"Enter a,b for ellipse (default=(%e,%e)): ",
          a,b));
      if( line!="" ) sScanF(line,"%e %e ",&a,&b);
      mappingHasChanged();
    }
    else if( answer=="set range dimension (2 or 3)" )
    {
      gi.inputString(line,sPrintF(buff,"Enter range dimension (current(%i)): ",rangeDimension));
      if( line!="" ) sScanF(line,"%i",&rangeDimension);
      rangeDimension=max(2,min(3,rangeDimension));
      mappingHasChanged();
    }
    else if( answer=="specify centre" )
    {
      if( rangeDimension==2 )
      {
        gi.inputString(line,sPrintF(buff,"Enter centre (default=(%e,%e)): ",xa,ya));
        if( line!="" ) sScanF(line,"%e %e",&xa,&ya);
      }
      else
      {
        gi.inputString(line,sPrintF(buff,"Enter centre (default=(%e,%e,%e)): ",xa,ya,za));
        if( line!="" ) sScanF(line,"%e %e %e",&xa,&ya,&za);
      }
      mappingHasChanged();
    }
    else if( answer=="specify start/end angles" )
    {
      gi.inputString(line,sPrintF(buff,"Enter startTheta, endTheta (default=(%e,%e)): ",
          startTheta, endTheta));
      if( line!="" )
      {
	sScanF(line,"%e %e ",&startTheta,&endTheta);
	if( fabs(fabs(endTheta-startTheta)-1.)<REAL_EPSILON*10. ) 
	{
	  setIsPeriodic(axis1, functionPeriodic );  
	  setBoundaryCondition( Start,axis1,-1 );
	  setBoundaryCondition( End  ,axis1,-1 );
	}
        else if( getIsPeriodic(axis1)!=notPeriodic )
	{
	  setIsPeriodic(axis1, notPeriodic );  
	  setBoundaryCondition( Start,axis1,1 );
	  setBoundaryCondition( End  ,axis1,1 );
	}
      }
      mappingHasChanged();
    }
    else if( answer=="show parameters" )
    {
      if( a==b )
        printf(" radius of circle = %e\n",a);
      else
        printf(" axes of ellipse, (a,b)=(%e,%e)\n",a,b);
      if( rangeDimension==2 )
        printf(" centre: (xa,ya)=(%e,%e)\n",xa,ya);
      else
        printf(" centre: (xa,ya,za)=(%e,%e,%e)\n",xa,ya,za);
      printf(" startTheta=%e, endTheta=%e \n",startTheta,endTheta);
      
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
