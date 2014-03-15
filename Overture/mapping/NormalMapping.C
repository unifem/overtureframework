#include "NormalMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "display.h"

NormalMapping::
NormalMapping() : Mapping(2,2,parameterSpace,cartesianSpace)   
//===========================================================================
/// \details 
///     Starting from a reference surface build an offset surface in the direction
///  normal to each point. 
///  
//===========================================================================
{ 
  NormalMapping::className="NormalMapping";
  setName( Mapping::mappingName,"normalMapping");
  setGridDimensions( axis1,11 );
  setGridDimensions( axis2,11 );
  surface=NULL;
  normalDistance=.5;
  mappingHasChanged();
}

NormalMapping::
NormalMapping(Mapping & referenceSurface, const real & distance /* =.5 */ )
//===========================================================================
/// \details 
///     Starting from a reference surface build an offset surface in the direction
///  normal to each point. 
///  
/// \param referenceSurface (input) : start from this surface.
/// \param distance (input) : offset distance.
//===========================================================================
{
  normalDistance=distance;
  setReferenceSurface(referenceSurface);
}



// Copy constructor is deep by default
NormalMapping::
NormalMapping( const NormalMapping & map, const CopyType copyType )
{
  NormalMapping::className="NormalMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "NormalMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

NormalMapping::
~NormalMapping()
{ if( debug & 4 )
  cout << " NormalMapping::Desctructor called" << endl;

  // *wdh* 090525 -- check for uncountedReferencesMayExist
 if( surface!=NULL && !surface->uncountedReferencesMayExist() && surface->decrementReferenceCount()==0 )
   delete surface;
}

NormalMapping & NormalMapping::
operator=( const NormalMapping & X )
{
  if( NormalMapping::className != X.getClassName() )
  {
    cout << "NormalMapping::operator= ERROR trying to set a NormalMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  normalDistance=X.normalDistance;
  surface=X.surface;
  // *wdh* 090525 -- check for uncountedReferencesMayExist
  if( surface!=NULL && !surface->uncountedReferencesMayExist() )
    surface->incrementReferenceCount();
  
  return *this;
}

void NormalMapping::
setReferenceSurface( Mapping & referenceSurface )
//===========================================================================
/// \details 
///     Define the reference surface.
///  
/// \param referenceSurface (input) : start from this surface.
//===========================================================================
{
  surface = &referenceSurface;
  // *wdh* 090525 -- check for uncountedReferencesMayExist
  if( !surface->uncountedReferencesMayExist() ) 
    surface->incrementReferenceCount();
  initialize();
}

void NormalMapping::
setNormalDistance( real distance )
//===========================================================================
/// \details 
///     Starting from a reference surface build an offset surface in the direction
///  normal to each point. 
///  
/// \param referenceSurface (input) : start from this surface.
/// \param distance (input) : offset distance.
//===========================================================================
{
  normalDistance=distance;
  mappingHasChanged();
}

void NormalMapping::
initialize()
// ============================================================================================
// /Access: protected.
// /Description:
//    Initialize properties of the Mapping.
// ============================================================================================
{
  // Define properties of this mapping
  if( getName(mappingName)=="normalMapping")
    setName(mappingName,aString("normal-")+surface->getName(mappingName));
  setDomainDimension(surface->getDomainDimension()+1);
  setRangeDimension(surface->getRangeDimension());
  int axis;
  for( axis=0; axis<domainDimension-1; axis++ )
  {
    setGridDimensions(axis,surface->getGridDimensions(axis));
    setIsPeriodic(axis,surface->getIsPeriodic(axis));
    for( int side=Start; side<=End; side++ )
    {
      setBoundaryCondition(side,axis,surface->getBoundaryCondition(side,axis));
      setShare(side,axis,surface->getShare(side,axis));
    }
  }
  setGridDimensions(domainDimension-1,11); // grid lines in normal direction
  setBoundaryCondition(Start,domainDimension-1,1); // by default the outer normal face is...
  setBoundaryCondition(End  ,domainDimension-1,0); // ...an interpolation boundary, bc=0
  bool singular=FALSE;
  for( axis=0; axis<domainDimension-1; axis++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      if( surface->getTypeOfCoordinateSingularity(side,axis)==polarSingularity )
      {
	singular=TRUE;
	setTypeOfCoordinateSingularity(side,axis,polarSingularity);
	setBoundaryCondition(side,axis,0);
      }
    }
  }
  // *** this doesn't work yet ***
  if( singular )
  {
    // setCoordinateEvaluationType( cylindrical,TRUE );  // Mapping can be evaluated in cylindrical coordinates
  }
  else
  {
    // setCoordinateEvaluationType( cylindrical,FALSE ); 
  }

  mappingHasChanged();
}


void NormalMapping::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
// ============================================================================================
/// \brief:
///     Evaluate the normal mapping.
// ============================================================================================
{
  if( surface==NULL )
  {
    cout << "NormalMapping::map: Error: The surface mapping has not been defined yet!\n";
    exit(1);    
  }

  if( params.coordinateType != cartesian )
    cerr << "NormalMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  const real h=.1*pow(20.*REAL_EPSILON,1./5.);
  const real eps=sqrt(REAL_MIN);
  

  Range R(0,rangeDimension-1), D(0,domainDimension-1), R0(base,bound);
  realArray normal(R0,R);
  realArray norm(R0);
  realArray xt(R0,R,D);

  const int surfaceDomainDimension=surface->getDomainDimension();

//    printf("*** surfaceDomainDimension=%i rangeDimension=%i ****\n",surfaceDomainDimension,
//                  rangeDimension  );
  
  if( computeMap || computeMapDerivative )
  {
    surface->map(r,x,xt);

    int axis;
    if( surfaceDomainDimension==1 )
    { // normals were extended from a 2D curve 
      normal(I,0)= xt(I,axis2,axis1);
      normal(I,1)=-xt(I,axis1,axis1);
      norm(I)=normalDistance/max(eps,sqrt(pow(normal(I,0),2)+pow(normal(I,1),2)));
      if( rangeDimension==3 )
      {
	x(I,2)=0.;
        normal(I,2)=0.;
      }
    }
    else
    {
      normal(I,0)=xt(I,axis2,axis1)*xt(I,axis3,axis2)-xt(I,axis3,axis1)*xt(I,axis2,axis2);
      normal(I,1)=xt(I,axis3,axis1)*xt(I,axis1,axis2)-xt(I,axis1,axis1)*xt(I,axis3,axis2);
      normal(I,2)=xt(I,axis1,axis1)*xt(I,axis2,axis2)-xt(I,axis2,axis1)*xt(I,axis1,axis2);

      // If there is a polar singularity we average the normal at the singularity so all
      // the lines emanating from the singularity are co-incident.
      for( axis=0; axis<domainDimension-1; axis++ )
      {
	for( int side=Start; side<=End; side++ )
	{
	  if( surface->getTypeOfCoordinateSingularity(side,axis)==polarSingularity )
	  {
	    where( fabs(r(I,axis)-(real)side)<REAL_EPSILON*10. )
	    {
	      for( int dir=0; dir<rangeDimension; dir++ )
		normal(I,dir)=sum(normal(I,dir))/real(bound-base+1);
	    }
	  }
	}
      }
      norm(I)=normalDistance/max(eps,sqrt(pow(normal(I,0),2)+pow(normal(I,1),2)+pow(normal(I,2),2)));
    }
    for( axis=0; axis<rangeDimension; axis++ )
      normal(I,axis)*=norm(I);

    if( surfaceDomainDimension==1 || domainDimension==rangeDimension )
    {
      for( axis=0; axis<rangeDimension; axis++ )
	x(I,axis)+=normal(I,axis)*r(I,domainDimension-1);    //  x= surface + normal * r

      // ::display(normal,"normal from normal mapping","%5.2f ");
      // ::display(x,"x from normal mapping","%5.2f ");
    }
    else
    {
      // offset surface
      for( axis=0; axis<rangeDimension; axis++ )
	x(I,axis)+=normal(I,axis);
    }
    
  }   
  if( computeMapDerivative )
  {
    // compute the Jacobian derivatives -- these require second derivatives, get these
    // by differencing --

    if( surfaceDomainDimension==1 || domainDimension==rangeDimension )
      xr(I,R,domainDimension-1)=normal(I,R);

    realArray t(R0,D);
    realArray xx(R0,R);
    t(I,D)=r(I,D);

    // ......Determine the normal surface at points -1.5h, -.5h, .5h, 1.5h  and difference to get jacobian
    // Use the 4th order difference approximation:
    //	xr(I,axis,dir)=(27.*(xx(I,axis,2)-xx(I,axis,1))-(xx(I,axis,3)-xx(I,axis,0)))/(24.*h);
    real diffCoeff[4] = { +1./(24.*h),-27./(24.*h),+27./(24.*h),-1./(24.*h)};  

    xr(I,R,Range(0,domainDimension-2))=0.;  // we will accumulate a sum
    for( int dir=0; dir<domainDimension-1; dir++ )  // partial w.r.t axis_dir
    {
      for( int i=0; i<4; i++ )  // 4 point difference
      {
	t(I,dir)=r(I,dir)+.5*(2*i-3)*h; // evaluate surface at this point
	surface->map(t,xx,xt);  

        if( surfaceDomainDimension==1 )
	{
	  normal(I,0)= xt(I,axis2,axis1);
	  normal(I,1)=-xt(I,axis1,axis1);
	  norm(I)=r(I,axis2)*normalDistance/sqrt(SQR(normal(I,0))+SQR(normal(I,1)));
          if( rangeDimension==3 )
	  {
	    xx(I,2)=0.;
            normal(I,2)=0.;
	  }
	}
	else
	{
	  normal(I,0)=xt(I,axis2,axis1)*xt(I,axis3,axis2)-xt(I,axis3,axis1)*xt(I,axis2,axis2);
	  normal(I,1)=xt(I,axis3,axis1)*xt(I,axis1,axis2)-xt(I,axis1,axis1)*xt(I,axis3,axis2);
	  normal(I,2)=xt(I,axis1,axis1)*xt(I,axis2,axis2)-xt(I,axis2,axis1)*xt(I,axis1,axis2);
 	  norm(I)=r(I,axis3)*normalDistance/max(eps,sqrt(SQR(normal(I,0))+SQR(normal(I,1))+SQR(normal(I,2))));
	}
	for( int axis=0; axis<rangeDimension; axis++ )
	  xx(I,axis,0)+=normal(I,axis)*norm(I);

        xr(I,R,dir)+=diffCoeff[i]*xx(I,R,0);
      }
      t(I,dir)=r(I,dir);
    }
  }
}


void NormalMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params )
// ============================================================================================
// /Description:
//     Evaluate the normal mapping.
// ============================================================================================
{
  if( surface==NULL )
  {
    cout << "NormalMapping::map: Error: The surface mapping has not been defined yet!\n";
    exit(1);    
  }

  if( params.coordinateType != cartesian )
    cerr << "NormalMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  const real h=.1*pow(20.*REAL_EPSILON,1./5.);
  const real eps=sqrt(REAL_MIN);
  

  Range R(0,rangeDimension-1), D(0,domainDimension-1), R0(base,bound);
  RealArray normal(R0,R);
  RealArray norm(R0);
  RealArray xt(R0,R,D);

  const int surfaceDomainDimension=surface->getDomainDimension();

//    printf("*** surfaceDomainDimension=%i rangeDimension=%i ****\n",surfaceDomainDimension,
//                  rangeDimension  );
  
  if( computeMap || computeMapDerivative )
  {
    surface->mapS(r,x,xt);

    int axis;
    if( surfaceDomainDimension==1 )
    { // normals were extended from a 2D curve 
      normal(I,0)= xt(I,axis2,axis1);
      normal(I,1)=-xt(I,axis1,axis1);
      norm(I)=normalDistance/max(eps,sqrt(pow(normal(I,0),2)+pow(normal(I,1),2)));
      if( rangeDimension==3 )
      {
	x(I,2)=0.;
        normal(I,2)=0.;
      }
    }
    else
    {
      normal(I,0)=xt(I,axis2,axis1)*xt(I,axis3,axis2)-xt(I,axis3,axis1)*xt(I,axis2,axis2);
      normal(I,1)=xt(I,axis3,axis1)*xt(I,axis1,axis2)-xt(I,axis1,axis1)*xt(I,axis3,axis2);
      normal(I,2)=xt(I,axis1,axis1)*xt(I,axis2,axis2)-xt(I,axis2,axis1)*xt(I,axis1,axis2);

      // If there is a polar singularity we average the normal at the singularity so all
      // the lines emanating from the singularity are co-incident.
      for( axis=0; axis<domainDimension-1; axis++ )
      {
	for( int side=Start; side<=End; side++ )
	{
	  if( surface->getTypeOfCoordinateSingularity(side,axis)==polarSingularity )
	  {
	    where( fabs(r(I,axis)-(real)side)<REAL_EPSILON*10. )
	    {
	      for( int dir=0; dir<rangeDimension; dir++ )
		normal(I,dir)=sum(normal(I,dir))/real(bound-base+1);
	    }
	  }
	}
      }
      norm(I)=normalDistance/max(eps,sqrt(pow(normal(I,0),2)+pow(normal(I,1),2)+pow(normal(I,2),2)));
    }
    for( axis=0; axis<rangeDimension; axis++ )
      normal(I,axis)*=norm(I);

    if( surfaceDomainDimension==1 || domainDimension==rangeDimension )
    {
      for( axis=0; axis<rangeDimension; axis++ )
	x(I,axis)+=normal(I,axis)*r(I,domainDimension-1);    //  x= surface + normal * r

      // ::display(normal,"normal from normal mapping","%5.2f ");
      // ::display(x,"x from normal mapping","%5.2f ");
    }
    else
    {
      // offset surface
      for( axis=0; axis<rangeDimension; axis++ )
	x(I,axis)+=normal(I,axis);
    }
    
  }   
  if( computeMapDerivative )
  {
    // compute the Jacobian derivatives -- these require second derivatives, get these
    // by differencing --

    if( surfaceDomainDimension==1 || domainDimension==rangeDimension )
      xr(I,R,domainDimension-1)=normal(I,R);

    RealArray t(R0,D);
    RealArray xx(R0,R);
    t(I,D)=r(I,D);

    // ......Determine the normal surface at points -1.5h, -.5h, .5h, 1.5h  and difference to get jacobian
    // Use the 4th order difference approximation:
    //	xr(I,axis,dir)=(27.*(xx(I,axis,2)-xx(I,axis,1))-(xx(I,axis,3)-xx(I,axis,0)))/(24.*h);
    real diffCoeff[4] = { +1./(24.*h),-27./(24.*h),+27./(24.*h),-1./(24.*h)};  

    xr(I,R,Range(0,domainDimension-2))=0.;  // we will accumulate a sum
    for( int dir=0; dir<domainDimension-1; dir++ )  // partial w.r.t axis_dir
    {
      for( int i=0; i<4; i++ )  // 4 point difference
      {
	t(I,dir)=r(I,dir)+.5*(2*i-3)*h; // evaluate surface at this point
	surface->mapS(t,xx,xt);  

        if( surfaceDomainDimension==1 )
	{
	  normal(I,0)= xt(I,axis2,axis1);
	  normal(I,1)=-xt(I,axis1,axis1);
	  norm(I)=r(I,axis2)*normalDistance/sqrt(SQR(normal(I,0))+SQR(normal(I,1)));
          if( rangeDimension==3 )
	  {
	    xx(I,2)=0.;
            normal(I,2)=0.;
	  }
	}
	else
	{
	  normal(I,0)=xt(I,axis2,axis1)*xt(I,axis3,axis2)-xt(I,axis3,axis1)*xt(I,axis2,axis2);
	  normal(I,1)=xt(I,axis3,axis1)*xt(I,axis1,axis2)-xt(I,axis1,axis1)*xt(I,axis3,axis2);
	  normal(I,2)=xt(I,axis1,axis1)*xt(I,axis2,axis2)-xt(I,axis2,axis1)*xt(I,axis1,axis2);
 	  norm(I)=r(I,axis3)*normalDistance/max(eps,sqrt(SQR(normal(I,0))+SQR(normal(I,1))+SQR(normal(I,2))));
	}
	for( int axis=0; axis<rangeDimension; axis++ )
	  xx(I,axis,0)+=normal(I,axis)*norm(I);

        xr(I,R,dir)+=diffCoeff[i]*xx(I,R,0);
      }
      t(I,dir)=r(I,dir);
    }
  }
}


//=================================================================================
// get a mapping from the database
//=================================================================================
int NormalMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase *subDir = dir.virtualConstructor();
  dir.find(*subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering NormalMapping::get" << endl;

  subDir->get( NormalMapping::className,"className" ); 
  if( NormalMapping::className != "NormalMapping" )
  {
    cout << "NormalMapping::get ERROR in className!" << endl;
  }
  subDir->get( normalDistance,"normalDistance" );

  aString mappingClassName;
  subDir->get(mappingClassName,"surface.className");  
  surface = Mapping::makeMapping( mappingClassName );  // ***** this does a new -- who will delete? ***
  if( surface==NULL )
  {
    cout << "NormalMapping::get:ERROR unable to make the mapping with className = " 
      << mappingClassName << endl;
    return 1;
  }
  surface->incrementReferenceCount();
  surface->get( *subDir,"surface" ); 
  Mapping::get( *subDir, "Mapping" );
  delete subDir;
  mappingHasChanged();
  return 0;
}

int NormalMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase *subDir = dir.virtualConstructor();      // create a derived data-base object
  dir.create(*subDir,name,"Mapping");                      // create a sub-directory 

  subDir->put( NormalMapping::className,"className" );
  subDir->put( normalDistance,"normalDistance" );
  subDir->put( surface->getClassName(),"surface.className"); // save the class name so we can do a 
  // "makeMapping" in the get function
  surface->put( *subDir,"surface" );
  Mapping::put( *subDir, "Mapping" );
  delete subDir;
  return 0;
}

Mapping *NormalMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==NormalMapping::className )
    retval = new NormalMapping();
  return retval;
}

    

//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int NormalMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sPrintF
  aString menu[] = 
    {
      "!NormalMapping",
      "extend normals from which mapping?",
      "normal distance",
      "volume grid",
      "surface grid",
      "make 3d",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "check",
      "show parameters",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "extend normals from which mapping? : choose a curve or surface",
      "normal distance    : Specify the normal distance (positive or negative)",
      "volume grid        : undo the surface grid option",
      "surface grid       : turn a volume grid into the offset surface grid",
      "make 3d            : turn a 2D grid into a 3D grid",
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

  bool plotObject=surface!=NULL;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  gi.appendToTheDefaultPrompt("Normal>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="extend normals from which mapping?" )
    { // Make a menu with the Mapping names (only curves or surfaces!)
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int j=0;
      for( int i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( map.getDomainDimension()== (map.getRangeDimension()-1) && map.mapPointer!=this )
	{
	  subListNumbering(j)=i;
          menu2[j++]=map.getName(mappingName);
	}
      }
      menu2[j++]="none"; 
      menu2[j]="";   // null string terminates the menu
      int mapNumber = gi.getMenuItem(menu2,answer2);
      delete [] menu2;
      if( mapNumber<0 )
      {
	printf("NormalMapping::ERROR: unknown mapping to use\n");
	gi.stopReadingCommandFile();
	continue;
      }
      if( answer2=="none" )
        continue;
      mapNumber=subListNumbering(mapNumber);  // map number in the original list
      surface=mapInfo.mappingList[mapNumber].mapPointer;

      setReferenceSurface(*surface);

      
      plotObject=TRUE;
    }
    else if( answer=="normal distance" ) 
    {
      gi.inputString(line,sPrintF(buff,"Enter the normal distance (default=(%e)): ",
          normalDistance));
      if( line!="" ) sScanF(line,"%e ",&normalDistance);
      mappingHasChanged();
    }
    else if( answer=="volume grid" )
    {
      // volumeGrid=true;
      setDomainDimension(rangeDimension);
    }
    else if( answer=="surface grid" )
    {
      // volumeGrid=false;
      setDomainDimension(rangeDimension-1);
    }
    else if( answer=="make 3d" )
    {
      setRangeDimension(3);
      mappingHasChanged();
    }
    else if( answer=="show parameters" )
    {
      printf(" normalDistance = = %e\n",normalDistance);
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
             answer=="periodicity"  ||
             answer=="check" )
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
