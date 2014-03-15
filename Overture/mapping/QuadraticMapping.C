#include "QuadraticMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include <float.h>

QuadraticMapping::
QuadraticMapping() : Mapping(2,3,parameterSpace,cartesianSpace)   
//===========================================================================
/// \details  
///      Define a quadrtic curve or surface (parabola or hyperbola)
/// 
//===========================================================================
{ 
  QuadraticMapping::className="QuadraticMapping";
  setName( Mapping::mappingName,"quadratic");

  setGridDimensions( axis1,11 );
  setGridDimensions( axis2,11 );

  quadraticOption=parabola;
  signForHyperbola=1.;
  
  a[0][0]=a[1][0]=a[2][0]=a[0][1]=a[0][2]=a[1][1]=a[2][1]=a[1][2]=a[2][2]=0;
  
  a[2][0]=1.;
  a[0][2]=1.;
  
  c[0][0]=c[0][1]=-1.; 
  c[1][0]=c[1][1]=2.; 
  
  mappingHasChanged();
}

QuadraticMapping::
QuadraticMapping( const QuadraticMapping & map, const CopyType copyType )
// Copy constructor. ( is deep by default)
{
  QuadraticMapping::className="QuadraticMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "QuadraticMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

QuadraticMapping::
~QuadraticMapping()
{ if( debug & 4 )
  cout << " QuadraticMapping::Desctructor called" << endl;
}

QuadraticMapping & QuadraticMapping::
operator=( const QuadraticMapping & X )
{
  if( QuadraticMapping::className != X.getClassName() )
  {
    cout << "QuadraticMapping::operator= ERROR trying to set a QuadraticMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  quadraticOption=X.quadraticOption;
  signForHyperbola=X.signForHyperbola;

  int i,j;
  for( i=0; i<3; i++ )
  for( j=0; j<3; j++ )
    a[i][j]=X.a[i][j];
  for( i=0; i<2; i++ )
  for( j=0; j<2; j++ )
    c[i][j]=X.c[i][j];

  return *this;
}



int QuadraticMapping::
chooseQuadratic( QuadraticOption option,
                 int rangeDimension_ /* =2 */ )
//===========================================================================
/// \details 
///     Specify the parameters for a quadratic function:
/// 
/// \param option (input): An option from the enum QuadraticOption: {\tt parabola} or {\tt hyperbola}.
/// \param rangeDimension_ (input): 2 or 3 
//===========================================================================
{
  quadraticOption=option;
  assert( rangeDimension_>=2 && rangeDimension_ <=3 );
  
  setDomainDimension(rangeDimension_-1);
  setRangeDimension(rangeDimension_);
  return 0;
}


int QuadraticMapping::
setParameters(real c0x, 
              real c1x,
              real c0y,
              real c1y,
	      real a00,
	      real a10, 
	      real a01, 
	      real a20, 
	      real a11, 
	      real a02,
	      real signForHyperbola_ /* = 1. */ )
//===========================================================================
/// \details 
///     Specify the parameters for a quadratic function:
/// 
///  A parabola (curve in 2D) is defined by
///  \begin{align*}
///     x_0 &= c_{0x} + c_{1x}*r_0 \\
///     x_1 &= a_{00} + a_{10} x_0 + a_{20} x_0^2
///  \end{align*}
///  A 3d paraboloid (surface) is defined by
///  \begin{align*}
///     x_0 &= c_{0x} + c_{1x}*r_0 \\
///     x_1 &= c_{0y} + c_{1y}*r_1 \\
///     x_2 &= a_{00} + a_{10} x_0 + a_{01} x_1 + a_{20} x_0^2 + a_{11}x_0 x_1 + a_{02} x_1^2
///  \end{align*}
///  
///  A hyperbola (2d curve) is defined by
///  \begin{align*}
///     x_0 &= c_{0x} + c_{1x}*r_0 \\
///     x_1 &= \pm (a_{00} + a_{10} x_0 + a_{20} x_0^2 )^{1/2}
///  \end{align*}
/// 
///  A 3d hyperboloid (surface) is defined by
///  \begin{align*}
///     x_0 &= c_{0x} + c_{1x}*r_0 \\
///     x_1 &= c_{0y} + c_{1y}*r_1 \\
///     x_2 &= \pm (a_{00} + a_{10} x_0 + a_{01} x_1 + a_{20} x_0^2 + a_{11}x_0 x_1 + a_{02} x_1^2)^{1/2}
///  \end{align*}
///  
/// \param a00_, a10_,... (input): parameters in above formula.
//===========================================================================
{
  a[0][0]=a00;
  a[1][0]=a10;
  a[0][1]=a01;
  a[2][0]=a20;
  a[1][1]=a11;
  a[0][2]=a02;

  c[0][0]=c0x;
  c[1][0]=c1x;
  c[0][1]=c0y;
  c[1][1]=c1y;
  
  signForHyperbola=signForHyperbola_;
  
  return 0;
}


void QuadraticMapping::
map( const realArray & r, realArray & x0, realArray & xr, MappingParameters & params )
{
  if( params.coordinateType != cartesian )
    cerr << "QuadraticMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x0,xr,base,bound,computeMap,computeMapDerivative );

  Range D=domainDimension;

  realArray x1;
  realArray & x = computeMap ? x0 : x1;
  if( !computeMap )
    x1.redim(I,rangeDimension);
    

  int axis;
  for( axis=0; axis<domainDimension; axis++ )
  {
   
    x(I,axis)=c[0][axis] + c[1][axis]*r(I,axis);
    if( computeMapDerivative )
    {
      xr(I,axis,axis) = c[1][axis];
      if( domainDimension==2 )
        xr(I,axis,(axis+1)%2)=0.;
    }
  }
  
  if( quadraticOption==parabola )
  {
    if( domainDimension==1 )
    {
      x(I,1) = a[0][0] + x(I,0)*(a[1][0]+a[2][0]*x(I,0));
      if( computeMapDerivative )
        xr(I,1,0)=(a[1][0]+(2.*a[2][0])*x(I,0))*c[1][0];
    }
    else
    {
      x(I,axis3) = a[0][0] + x(I,0)*(a[1][0]+a[2][0]*x(I,0)+a[1][1]*x(I,1)) + x(I,1)*(a[0][1]+a[0][2]*x(I,1));
      if( computeMapDerivative )
      {
        xr(I,2,0)=(a[1][0]+(2.*a[2][0])*x(I,0)+a[1][1]*x(I,1))*c[1][0];
        xr(I,2,1)=(a[0][1]+(2.*a[0][2])*x(I,1)+a[1][1]*x(I,0))*c[1][1];
      }
    }
  }
  else if( quadraticOption==hyperbola )
  {
    realArray arg;
    const real eps=100.*REAL_MIN;
    if( domainDimension==1 )
    {
      arg=max(eps,  a[0][0] + x(I,0)*(a[1][0]+a[2][0]*x(I,0)) );
      
      x(I,1) = signForHyperbola*SQRT( arg );
      if( computeMapDerivative )
        xr(I,1,0)=((.5*a[1][0]+a[2][0]*x(I,0))/x(I,1))*c[1][0];
    }
    else
    {
      arg=max(eps,  a[0][0] + x(I,0)*(a[1][0]+a[2][0]*x(I,0)+a[1][1]*x(I,1)) + x(I,1)*(a[0][1]+a[0][2]*x(I,1)));
      x(I,2) = signForHyperbola*SQRT( arg );
      if( computeMapDerivative )
      {
        xr(I,2,0)=((.5*a[1][0]+(a[2][0])*x(I,0)+(.5*a[1][1])*x(I,1))/x(I,2))*c[1][0];
        xr(I,2,1)=((.5*a[0][1]+(a[0][2])*x(I,1)+(.5*a[1][1])*x(I,0))/x(I,2))*c[1][1];
      }
    }
  }
  else
  {
    printf("QuadraticMapping:ERROR: unknown quadraticOption=%i\n",quadraticOption);
    throw "error";
  }
  
}


int QuadraticMapping::
get( const GenericDataBase & dir, const aString & name)
//=================================================================================
// Get a mapping from a database.
//=================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering QuadraticMapping::get" << endl;

  subDir.get( QuadraticMapping::className,"className" ); 
  if( QuadraticMapping::className != "QuadraticMapping" )
  {
    cout << "QuadraticMapping::get ERROR in className!" << endl;
  }
  int temp;
  subDir.get( temp,"quadraticOption" ); quadraticOption=(QuadraticOption)temp;
  subDir.get( signForHyperbola,"signForHyperbola");
  subDir.get( (real*)c,"c",4 );
  subDir.get( (real*)a,"a",9 );


  Mapping::get( subDir, "Mapping" );
  delete &subDir;
  mappingHasChanged();
  return 0;
}

int QuadraticMapping::
put( GenericDataBase & dir, const aString & name) const
//=================================================================================
// Save a mapping in a database.
//=================================================================================
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( QuadraticMapping::className,"className" );

  subDir.put( (int)quadraticOption,"quadraticOption" );
  subDir.put( signForHyperbola,"signForHyperbola");
  subDir.put( (real*)c,"c",4 );
  subDir.put( (real*)a,"a",9 );

  Mapping::put( subDir, "Mapping" );
  delete &subDir;
  return 0;
}

Mapping *QuadraticMapping::
make( const aString & mappingClassName )
// Make a new mapping if the mappingClassName is the name of this Class
{
  Mapping *retval=0;
  if( mappingClassName==QuadraticMapping::className )
    retval = new QuadraticMapping();
  return retval;
}

    

int QuadraticMapping::
update( MappingInformation & mapInfo ) 
//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sPrintF
  aString menu[] = 
    {
      "!QuadraticMapping",
      "parabola (2d)",
      "parabola (3d)",
      "hyperbola (2d)",
      "hyperbola (3d)",
      "parameters",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "show parameters",
      "check",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "parabola (2d)",
      "parabola (3d)",
      "hyperbola (2d)",
      "hyperbola (3d)",
      "parameters"
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
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  gi.appendToTheDefaultPrompt("Quadratic>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="parabola (2d)" )
    { 
      quadraticOption=parabola;
      setDomainDimension(1);
      setRangeDimension(2);
      mappingHasChanged();
    }
    else if( answer=="parabola (3d)" )
    { 
      quadraticOption=parabola;
      setDomainDimension(2);
      setRangeDimension(3);
      mappingHasChanged();
    }
    else if( answer=="hyperbola (2d)" )
    { 
      
      quadraticOption=hyperbola;
      setDomainDimension(1);
      setRangeDimension(2);
      if( a[0][0]==0. )
        a[0][0]=.1;
      mappingHasChanged();
    }
    else if( answer=="hyperbola (3d)" )
    { 
      quadraticOption=hyperbola;
      setDomainDimension(2);
      setRangeDimension(3);
      if( a[0][0]==0. )
        a[0][0]=.1;
      mappingHasChanged();
    }
    else if( answer=="parameters" )
    {
      char buff[100];
      if( quadraticOption==parabola )
      {
        if( domainDimension==1 )
	{
          printf("parabola: x=c0+c1*r0, y=a0+a1*x+a2*x*x\n");
          printf("Current: c0=%e, c1=%e, a0=%e, a1=%e, a2=%e\n",c[0][0],c[1][0],a[0][0],a[1][0],a[2][0]);
	  gi.inputString(answer,sPrintF(buff,"Enter c0,c1, a0,a1,a2"));
          if( answer!="" )
	  {
	    sScanF(answer,"%e %e %e %e %e",&c[0][0],&c[1][0],&a[0][0],&a[1][0],&a[2][0]);
	  }
	}
	else
	{
	  printf("parabola: x=c0x+c1x*r0, y=c0y+c1y*r1, z=a00+a10*x+a01*y+a11*x*y+a20*x*x+a02*y*y\n");
	  printf("Current: c0x=%e, c1x=%e, c0y=%e, c1y=%e\n"
		 "a00=%e, a10=%e, a01=%e, a11=%e, a20=%e, a02=%e\n",
                 c[0][0],c[1][0],c[0][1],c[1][1],
		 a[0][0],a[1][0],a[0][1], a[1][1],a[2][0],a[0][2]);
	    gi.inputString(answer,sPrintF(buff,"Enter c0x,c1x,c0y,c1y, a00,a10,a01,a11,a20,a02"));
          if( answer!="" )
	    sScanF(answer,"%e %e %e %e %e %e %e %e %e %e",&c[0][0],&c[1][0],&c[0][1],&c[1][1],
		   &a[0][0],&a[1][0],&a[0][1], &a[1][1],&a[2][0],&a[0][2]);
	}
      }
      else
      {
        if( domainDimension==1 )
	{
          printf("hyperbola: x=c0+c1*r0, y=sqrt( a0+a1*x+a2*x*x )\n");
          printf("Current: c0=%e, c1=%e, a0=%e, a1=%e, a2=%e\n",c[0][0],c[1][0],a[0][0],a[1][0],a[2][0]);
	  gi.inputString(answer,sPrintF(buff,"Enter c0,c1, a0,a1,a2"));
          if( answer!="" )
	  {
	    sScanF(answer,"%e %e %e %e %e",&c[0][0],&c[1][0],&a[0][0],&a[1][0],&a[2][0]);
	  }
	}
	else
	{
	  printf("parabola: x=c0x+c1x*r0, y=c0y+c1y*r1, z=sqrt(a00+a10*x+a01*y+a11*x*y+a20*x*x+a02*y*y)\n");
	  printf("Current: c0x=%e, c1x=%e, c0y=%e, c1y=%e\n"
		 "a00=%e, a10=%e, a01=%e, a11=%e, a20=%e, a02=%e\n",
                 c[0][0],c[1][0],c[0][1],c[1][1],
		 a[0][0],a[1][0],a[0][1], a[1][1],a[2][0],a[0][2]);
	    gi.inputString(answer,sPrintF(buff,"Enter c0x,c1x,c0y,c1y, a00,a10,a01,a11,a20,a02"));
          if( answer!="" )
	    sScanF(answer,"%e %e %e %e %e %e %e %e %e %e",&c[0][0],&c[1][0],&c[0][1],&c[1][1],
		   &a[0][0],&a[1][0],&a[0][1], &a[1][1],&a[2][0],&a[0][2]);
	}
      }
      mappingHasChanged();
	
    }
    else if( answer=="show parameters" )
    {
      if( quadraticOption==parabola )
      {
        if( domainDimension==1 )
	{
          printf("parabola: x=c0+c1*r0, y=a0+a1*x+a2*x*x\n");
          printf("Current: c0=%e, c1=%e, a0=%e, a1=%e, a2=%e\n",c[0][0],c[1][0],a[0][0],a[1][0],a[2][0]);
	  
	}
	else
	{
	  printf("parabola: x=c0x+c1x*r0, y=c0y+c1y*r1, z=a00+a10*x+a01*y+a11*x*y+a20*x*x+a02*y*y\n");
	  printf("Current: c0x=%e, c1x=%e, c0y=%e, c1y=%e\n"
		 "a00=%e, a10=%e, a01=%e, a11=%e, a20=%e, a02=%e\n",
		 c[0][0],c[1][0],c[0][1],c[1][1],
		 a[0][0],a[1][0],a[0][1], a[1][1],a[2][0],a[0][2]);
	}
      }
      else
      {
	if( domainDimension==1 )
	{
	  printf("hyperbola: x=c0+c1*r0, y=sqrt( a0+a1*x+a2*x*x )\n");
	  printf("Current: c0=%e, c1=%e, a0=%e, a1=%e, a2=%e\n",c[0][0],c[1][0],a[0][0],a[1][0],a[2][0]);
	}
	else
	{
	  printf("parabola: x=c0x+c1x*r0, y=c0y+c1y*r1, z=sqrt(a00+a10*x+a01*y+a11*x*y+a20*x*x+a02*y*y)\n");
	  printf("Current: c0x=%e, c1x=%e, c0y=%e, c1y=%e\n"
		 "a00=%e, a10=%e, a01=%e, a11=%e, a20=%e, a02=%e\n",
		 c[0][0],c[1][0],c[0][1],c[1][1],
		 a[0][0],a[1][0],a[0][1], a[1][1],a[2][0],a[0][2]);
	}
      }
      Mapping::display();
    }
    else if( answer=="check" )
    {
      checkMapping();
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
      // Here we plot the QuadraticMapping.
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters);  

    }
  }
  if( !gridIsValid() )
    getGrid();
  
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset the default prompt
  return 0;
  
}
