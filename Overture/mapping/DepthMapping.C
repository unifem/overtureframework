#include "DepthMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include <float.h>

DepthMapping::
DepthMapping() : Mapping(3,3,parameterSpace,cartesianSpace)   
//===========================================================================
/// \details  
///   Define a 3D Mapping from 2D Mapping by extending in 
///   the z-direction by a variable amount
/// 
//===========================================================================
{ 
  DepthMapping::className="DepthMapping";
  setName( Mapping::mappingName,"depthMapping");
  setGridDimensions( axis1,11 );
  setGridDimensions( axis2,11 );
  setGridDimensions( axis2,11 );
  surface=NULL;
  depth=NULL;

  depthOption=constantDepth;

  zSurface=0.;  // default level for surface for all depth functions
  zDepth=-.5;   // default depth for constant depth.

  a00=-.5;
  a20=a02=.25;
  a10=a01=a11=0.;

// scale factors from x to r for the depthFunction:
  depthPar[0]=0.;
  depthPar[1]=1.;
  depthPar[2]=0.;
  depthPar[3]=1.;

  
  mappingHasChanged();
}

DepthMapping::
DepthMapping( const DepthMapping & map, const CopyType copyType )
// Copy constructor. ( is deep by default)
{
  DepthMapping::className="DepthMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "DepthMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

DepthMapping::
~DepthMapping()
{ if( debug & 4 )
  cout << " DepthMapping::Desctructor called" << endl;
}

DepthMapping & DepthMapping::
operator=( const DepthMapping & X )
{
  if( DepthMapping::className != X.getClassName() )
  {
    cout << "DepthMapping::operator= ERROR trying to set a DepthMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  depthOption=X.depthOption;
  zDepth     =X.zDepth;
  zSurface   =X.zSurface;
  surface    =X.surface;
  depth      =X.depth;
  a00        =X.a00;
  a10        =X.a10;
  a01        =X.a01;
  a20        =X.a20;
  a11        =X.a11;
  a02        =X.a02;
  for( int i=0; i<4; i++ )
    depthPar[i]=X.depthPar[i];
  
  return *this;
}

int DepthMapping::
initialize()
//  Initialize a DepthMapping when a new surface is defined
{
  assert( surface!=NULL );
  
  setName(mappingName,aString("depth-")+surface->getName(mappingName));

  for( int axis=0; axis<domainDimension-1; axis++ )
  {
    setGridDimensions(axis,surface->getGridDimensions(axis));
    setIsPeriodic(axis,surface->getIsPeriodic(axis));
    for( int side=Start; side<=End; side++ )
    {
      setBoundaryCondition(side,axis,surface->getBoundaryCondition(side,axis));
      setShare(side,axis,surface->getShare(side,axis));
    }
  }
  setGridDimensions(domainDimension-1,11); // grid lines in depth direction
  setBoundaryCondition(Start,domainDimension-1,1);
  setBoundaryCondition(End  ,domainDimension-1,1);

  return 0;
}

int DepthMapping::
setDepthFunction( Mapping & depth_ )
//===========================================================================
/// \details 
///     Supply a mapping that will define the depth.
/// \param depth_ (input) : Use this mapping for the depth, $z=depth(x_0,x_1)$.
//===========================================================================
{
  depth=&depth_;
  mappingHasChanged();
  return 0;
}

int DepthMapping::
setDepthFunctionParameters( real a0, real b0, real a1, real b1 )
//===========================================================================
/// \details 
///     Define the scaling parameters for the depth function.
/// \param a0,b0,a1,b1_ (input) : 
//===========================================================================
{
  assert( b0!=0. && b1!=0. );

  depthPar[0]=a0;
  depthPar[1]=1./b0;
  depthPar[2]=a1;
  depthPar[3]=1./b1;

  return 0;
}



int DepthMapping::
setSurface( Mapping & surface_ )
//===========================================================================
/// \details 
///     Supply a 2D mapping that will define the surface of the 3D domain.
/// \param surface_ (input) : 2D Mapping.
//===========================================================================
{
  surface=&surface_;
  initialize();
  mappingHasChanged();
  return 0;
}



int DepthMapping::
setQuadraticParameters(const real & a00_,
		       const real & a10_, 
		       const real & a01_, 
		       const real & a20_, 
		       const real & a11_, 
		       const real & a02_)
//===========================================================================
/// \details 
///     Specify the parameters for a quadratic depth function:
///  \[
///     z(x_0,x_1) =  a_{00} + a_{10} x_0 + a_{01} x_1 + a_{20} x_0^2 + a_{11}x_0 x_1 + a_{02} x_1^2
///  \]
///  
/// \param a00_, a10_,... (input): parameters in above formula.
//===========================================================================
{
  a00=a00_;
  a10=a10_;
  a01=a01_;
  a20=a20_;
  a11=a11_;
  a02=a02_;
  return 0;
}


void DepthMapping::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
{
  if( surface==NULL )
  {
    cout << "DepthMapping::map: Error: The surface mapping has not been defined yet!\n";
    exit(1);    
  }

  if( params.coordinateType != cartesian )
    cerr << "DepthMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  Range D2(0,1), D3(0,2);


  if( depthOption==constantDepth )
  {
    // constant depth case
    surface->map(r,x,xr);  
    if( computeMap )
      x(I,2)=zSurface + r(I,axis3)*zDepth;
    if( computeMapDerivative )
    {
      xr(I,D2,2)=0.;
      xr(I,2,D2)=0.;
      xr(I,2,2)=zDepth;
    }
  }
  else if(  depthOption==quadraticDepth )
  {
    // variable depth
    realArray x2;
    if( !computeMap )
    {
      x2.redim(I,D2);
    }
    realArray & xx = computeMap ? x : x2;  // we need to compute the surface points even if the user only wants
                                           // the derivatives
    
    // evaluate the 2D surface
    surface->map(r,xx,xr);  

    // evaluate the depth function
    realArray z(I,1), zx(I,1,2);
    // quadratic bottom profile
    z(I) = a00 + xx(I,0)*(a10+a20*xx(I,0)+a11*xx(I,1)) + xx(I,1)*(a01+a02*xx(I,1));
    if( computeMapDerivative )
    {
      zx(I,0,0) = a10+2.*a20*xx(I,0)+a11*xx(I,1);   // dz/dx_i
      zx(I,0,1) = a01+2.*a02*xx(I,1)+a11*xx(I,0);   // dz/dx_i
    }

    if( computeMap )
    {
      x(I,2) = zSurface + r(I,axis3)*z(I);
    }
    if( computeMapDerivative )
    {
      xr(I,D2,2)=0.;    
      for( int dir=0; dir<=1; dir++ )
	xr(I,2,dir) = r(I,axis3)*(zx(I,0,0)*xr(I,0,dir) + zx(I,0,1)*xr(I,1,dir));
      xr(I,2,2)=z(I);
    }
    
  }
  else if(  depthOption==depthFunction )
  {
    // use a depth function
    assert( depth!=NULL );

    realArray x3(I,3),r3(I,3),x4(I,3),xr3;
    
    if( computeMapDerivative )
    {
      xr3.redim(I,3,3);
    }
    
    // evaluate the 2D surface
    surface->map(r,x3,xr);  

    // evaluate the depth function

    // x3(I,Range(0,1)).display("evaluate 2D surface");
    
    // evaluate the depth function
    r3(I,0)=(x3(I,0)-depthPar[0])*depthPar[1];   // scale to unit square coordinates
    r3(I,1)=(x3(I,1)-depthPar[2])*depthPar[3];
    if( domainDimension==3 )
      r3(I,2)=r(I,2);
    
    if( max(fabs(r3(I,0)-.5))>.7 ||  max(fabs(r3(I,1)-.5))>.7 )
    {
      printf("DepthMapping:WARNING: depthFunction scaling parameters are probably wrong\n");
      printf(" max(fabs(r3(I,0)-.5))=%8.2e, max(fabs(r3(I,1)-.5))=%8.2e \n",
	     max(fabs(r3(I,0)-.5)),max(fabs(r3(I,1)-.5)));
      
    }
    
    depth->map(r3,x4,xr3);

    //r3.display("r3");
    //x3.display("re-evaluate surface");
    //printf("DepthMapping:map: min(r3)=%e max(r3)=%e \n",min(r3),max(r3));
    //printf("DepthMapping:map: min(x3)=%e max(x3)=%e \n",min(x3),max(x3));

    if( computeMap )
    {
      Range R2=Range(0,1);
      x(I,R2)=x3(I,R2);
      x(I,2)=x4(I,2);
    }
    if( computeMapDerivative )
    {
      Range R2=Range(0,1);
      if( domainDimension==3 )
      {
	xr(I,2,2)=xr3(I,2,2);
	xr(I,R2,2)=0.;
      }
      // d(z)/d(r) = chain rule
      for( int dir=0; dir<=1; dir++ )
	xr(I,2,dir)=xr3(I,2,0)*xr(I,0,dir)+xr3(I,2,1)*xr(I,1,dir);
    }
  }
  else
  {
    printf("DepthMapping:ERROR: unknown depthOption=%i\n",depthOption);
    throw "error";
  }
  
}


int DepthMapping::
get( const GenericDataBase & dir, const aString & name)
//=================================================================================
// Get a mapping from a database.
//=================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering DepthMapping::get" << endl;

  subDir.get( DepthMapping::className,"className" ); 
  if( DepthMapping::className != "DepthMapping" )
  {
    cout << "DepthMapping::get ERROR in className!" << endl;
  }
  int temp;
  subDir.get( temp,"depthOption" ); depthOption=(DepthOption)temp;
  subDir.get( zDepth,"zDepth" );
  subDir.get( a00,"a00" );
  subDir.get( a10,"a10" );
  subDir.get( a01,"a01" );
  subDir.get( a20,"a20" );
  subDir.get( a11,"a11" );
  subDir.get( a02,"a02" );

  subDir.get( zSurface,"zSurface" );

  subDir.get( depthPar,"depthPar",4 );

  aString mapClassName;

  char buff[40];
  for( int i=0; i<=1; i++ )  // *** only get 1 mapping for now.
  {
    Mapping *&mapPointer = i==0 ? surface : depth;
    int mappingExists=   mapPointer!=NULL ? 1 : 0;
    subDir.get(mappingExists,sPrintF(buff,"mapping%iExists",i));
    if( mappingExists )
    {
      aString mapClassName;
      subDir.get(mapClassName,sPrintF(buff,"mapping%iClassName",i));
      mapPointer = Mapping::makeMapping( mapClassName );
      if( mapPointer==NULL )
      {
	cout << "DepthMapping::get:ERROR unable to make the mapping with className = " 
	  << (const char *)mapClassName << endl;
        {throw "error";}
      }
      mapPointer->get( subDir,sPrintF(buff,"mapping%i",i) );
    }
  }


  Mapping::get( subDir, "Mapping" );
  delete &subDir;
  mappingHasChanged();
  return 0;
}

int DepthMapping::
put( GenericDataBase & dir, const aString & name) const
//=================================================================================
// Save a mapping in a database.
//=================================================================================
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( DepthMapping::className,"className" );

  subDir.put( (int)depthOption,"depthOption" );
  subDir.put( zDepth,"zDepth" );

  subDir.put( a00,"a00" );
  subDir.put( a10,"a10" );
  subDir.put( a01,"a01" );
  subDir.put( a20,"a20" );
  subDir.put( a11,"a11" );
  subDir.put( a02,"a02" );

  subDir.put( zSurface,"zSurface" );

  subDir.put( depthPar,"depthPar",4 );

  char buff[40];
  for( int i=0; i<=1; i++ )   
  {
    Mapping *mapPointer = i==0 ? surface : depth;

    int mappingExists=   mapPointer!=NULL ? 1 : 0;
    subDir.put(mappingExists,sPrintF(buff,"mapping%iExists",i));
    if( mappingExists )
    {
      subDir.put(mapPointer->getClassName(),sPrintF(buff,"mapping%iClassName",i));
      mapPointer->put( subDir,sPrintF(buff,"mapping%i",i) );
    }
  }


  Mapping::put( subDir, "Mapping" );
  delete &subDir;
  return 0;
}

Mapping *DepthMapping::
make( const aString & mappingClassName )
// Make a new mapping if the mappingClassName is the name of this Class
{
  Mapping *retval=0;
  if( mappingClassName==DepthMapping::className )
    retval = new DepthMapping();
  return retval;
}

    

int DepthMapping::
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
      "!DepthMapping",
      "extend depth from which mapping?",
      "constant depth",
      "quadratic depth",
      "depth function",
      "depth function parameters",
      "surface height",
      "check",
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
      "extend depth from which mapping? : choose a 2D Mapping",
      "constant depth     : Specify a constant depth (positive or negative)",
      "quadratic depth    : choose a quadratic depth profile",
      "depth function     : Specify a Mapping to define the depth",
      "surface height     : specify the surface height",
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

  bool mappingChosen=surface!=NULL;

  // By default transform the last appropriate mapping in the list 
  if( !mappingChosen )
  {
    int number= mapInfo.mappingList.getLength();
    for( int i=number-1; i>=0; i-- )
    {
      Mapping *mapPointer=mapInfo.mappingList[i].mapPointer;
      if( mapPointer->getDomainDimension()==2 && mapPointer->getRangeDimension()==2 )
      {
        surface=mapPointer;   // use this one
        initialize();
        mappingHasChanged();
	break; 
      }
    }
  }
  if( surface==NULL )
  {
    cout << "DepthMapping:ERROR: there are no 2D mappings that can be used! \n";
    cout << "A DepthMapping mapping is applied to a Mapping with domainDimension==rangeDimension==2 \n";
    return 1;
  }

  bool plotObject=TRUE;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  gi.appendToTheDefaultPrompt("Depth>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="extend depth from which mapping?" )
    { // Make a menu with the Mapping names (only Mappings R2->R2)
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int j=0;
      for( int i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( map.getDomainDimension()==2 &&  map.getRangeDimension()==2  )
	{
	  subListNumbering(j)=i;
          menu2[j++]=map.getName(mappingName);
	}
      }
      menu2[j++]="none"; 
      menu2[j]="";   // null string terminates the menu
      int mapNumber = gi.getMenuItem(menu2,answer2);
      delete [] menu2;
      if( answer2=="none" )
        continue;
      mapNumber=subListNumbering(mapNumber);  // map number in the original list
      if( mapInfo.mappingList[mapNumber].mapPointer==this )
      {
	cout << "DepthMapping::ERROR: you cannot transform this mapping, this would be recursive!\n";
        continue;
      }

      surface=mapInfo.mappingList[mapNumber].mapPointer;
      // Define properties of this mapping
      initialize();

      mappingHasChanged();
      plotObject=TRUE;
    }
    else if( answer=="constant depth" ) 
    {
      depthOption=constantDepth;
      gi.inputString(line,sPrintF(buff,"Enter the constant depth  (default=(%e)): ",
          zDepth));
      if( line!="" ) sScanF(line,"%e ",&zDepth);
      mappingHasChanged();
    }
    else if( answer=="quadratic depth" ) 
    {
      depthOption=quadraticDepth;
      printf("The quadratic depth function is z(x,y) = a00 + a10*x + a01*y + a20*x^2 + a11*x*y + a02*y^2");
      gi.inputString(line,sPrintF(buff,"Enter the parameters a00,a10,a01,a20,a11,a02 "
           "(default=(%7.2e,%7.2e,%7.2e,%7.2e,%7.2e,%7.2e)): ",a00,a10,a01,a20,a11,a02));
      if( line!="" ) sScanF(line,"%e %e %e %e %e %e",&a00,&a10,&a01,&a20,&a11,&a02);
      mappingHasChanged();
    }
    else if( answer=="depth function" )
    {
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int j=0;
      for( int i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( map.getDomainDimension()>=2 &&  map.getRangeDimension()==3 && map.mapPointer!=this )
	{
	  subListNumbering(j)=i;
          menu2[j++]=map.getName(mappingName);
	}
      }
      if( j==0 )
      {
	printf("DepthMapping:ERROR: There are no Mappings that can be used as a depth function.\n"
               "     A depth function should define a 3D volume,  R^3 -> R^3\n");
      }
      else
      {
	menu2[j++]="none"; 
	menu2[j]="";   // null string terminates the menu

	int mapNumber = gi.getMenuItem(menu2,answer2);
	if( answer2=="none" )
	  continue;
        else if( mapNumber<0 )
	{
          cout << "Unknown response: [" << answer2 << "]\n";
	  gi.stopReadingCommandFile();
	}
	else
	{
	  mapNumber=subListNumbering(mapNumber);  // map number in the original list
          depthOption=depthFunction;
	  depth=mapInfo.mappingList[mapNumber].mapPointer;
          setDomainDimension( depth->getDomainDimension() );
	}
      }
      delete [] menu2;
      mappingHasChanged();
    }
    else if( answer=="depth function parameters" )
    {
      real & a0=depthPar[0], b0=1./depthPar[1];
      real & a1=depthPar[2], b1=1./depthPar[3];
      
      gi.inputString(line,sPrintF(buff,"Enter a0,b0,a1,b1 (default=(%7.2e,%7.2e,%7.2e,%7.2e)): ",
            a0,b0,a1,b1));
      if( line!="" ) 
      {
        sScanF(line,"%e %e %e %e ",&a0,&b0,&a1,&b1);
        depthPar[1]=1./b0;
        depthPar[3]=1./b1;
      }
      mappingHasChanged();
    }
    else if( answer=="surface height" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the surface height  (default=(%e)): ",zSurface));
      if( line!="" ) sScanF(line,"%e ",&zSurface);
      mappingHasChanged();
    }
    else if( answer=="show parameters" )
    {
      printf(" depthOption = %s\n",depthOption==constantDepth ? "constant depth" : 
	     (depthOption==quadraticDepth ? "quadratic depth" : "depth function"));
      if( depthOption==constantDepth )
        printf(" constant depth : zDepth = %e\n",zDepth);
      else
      {
        if( depthOption==quadraticDepth )
          printf("a00=%e, a10=%e, a01=%e, a20=%e, a11=%e, a02=%e \n",a00,a10,a01,a20,a11,a02);
      }
      display();
    }
    else if( answer=="check" )
    {
      realArray r(1,3),x(1,3),xr(1,3,3);
      for( ;; )
      {
        cout << "Enter r1,r2,r3 \n";
	cin >> r(0,0) >> r(0,1) >> r(0,2);
	map(r,x);
	printf(" r=(%9.2e,%9.2e,%9.2e) x=(%9.2e,%9.2e,%9.2e)\n",
	       r(0,0),r(0,1),r(0,2),x(0,0),x(0,1),x(0,2));
      }
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
      // Here we plot the DepthMapping.
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters);  

    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset the default prompt
  return 0;
  
}
