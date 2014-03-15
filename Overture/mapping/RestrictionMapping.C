#include "RestrictionMapping.h"
#include "MappingInformation.h"

RestrictionMapping::
RestrictionMapping(const real ra_  /* =0. */,
		   const real rb_  /* =1. */,
		   const real sa_  /* =0. */,
		   const real sb_  /* =1. */,
		   const real ta_  /* =0. */,
		   const real tb_  /* =1. */,
		   const int dimension /* =2  */,
		   Mapping *restrictedMapping /* =NULL */)
//===========================================================================
/// \brief  Default Constructor
///  The restriction is a Mapping from {\tt parameter} space to {\tt parameter} space
///   defined by 
///  \begin{align*}
///      x(I,axis1) &= (rb-ra) r(I,axis1)+ra \\
///      x(I,axis2) &= (sb-sa) r(I,axis2)+sa \\
///      x(I,axis3) &= (tb-ta) r(I,axis3)+ta  
///  \end{align*}
/// \param ra_,rb_,sa_,sb_,ta_,tb_ (input): Parameters in the definition of the 
///    {\tt RestrictionMapping}.
/// \param dimension (input): define the domain and range dimension (which are equal).
/// \param restrictedMapping (input) : optionally pass the Mapping being restricted. 
///   This Mapping is used to set spaceIsPeriodic.
//===========================================================================
: Mapping(dimension,dimension,parameterSpace,parameterSpace)   
{ 
  RestrictionMapping::className="RestrictionMapping";
  setName( Mapping::mappingName,"restriction");
  for( int axis=0; axis<dimension; axis++ )
    setGridDimensions( axis,11 );
  ra=ra_;
  rb=rb_;
  sa=sa_;
  sb=sb_;
  ta=ta_;
  tb=tb_;
  spaceIsPeriodic[0]=spaceIsPeriodic[1]=spaceIsPeriodic[2]=0;
  if( restrictedMapping!=NULL )
  {
    for( int axis=0; axis<domainDimension; axis++ )
    {
      if( restrictedMapping->getIsPeriodic(axis)==functionPeriodic )
	spaceIsPeriodic[axis]=true;
    }
  }
  setBasicInverseOption(canInvert);  // basicInverse is available
  inverseIsDistributed=false;
  setMappingCoordinateSystem( rectangular );  // for optimizing derivatives
  mappingHasChanged();

}

// Copy constructor is deep by default
RestrictionMapping::
RestrictionMapping( const RestrictionMapping & map, const CopyType copyType )
{
  RestrictionMapping::className="RestrictionMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    printF("RestrictionMapping:: sorry no shallow copy constructor, doing a deep! \n");
    *this=map;
  }
}

RestrictionMapping::
~RestrictionMapping()
{ 
  if( debug & 4 )
    printF(" RestrictionMapping::Destructor called\n");
}

RestrictionMapping & RestrictionMapping::
operator=( const RestrictionMapping & X )
{
  if( RestrictionMapping::className != X.getClassName() )
  {
    printF("RestrictionMapping::operator= ERROR trying to set a RestrictionMapping = to a" 
           " mapping of type %s\n",(const char*)X.getClassName());
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  ra=X.ra;
  rb=X.rb;
  sa=X.sa;
  sb=X.sb;
  ta=X.ta;
  tb=X.tb;
  spaceIsPeriodic[0]=X.spaceIsPeriodic[0];
  spaceIsPeriodic[1]=X.spaceIsPeriodic[1];
  spaceIsPeriodic[2]=X.spaceIsPeriodic[2];

  return *this;
}

int RestrictionMapping::
scaleBounds(const real ra_, /* =0. */
	    const real rb_, /* =1. */ 
	    const real sa_, /* =0. */
	    const real sb_, /* =1. */
	    const real ta_, /* =0. */
	    const real tb_  /* =1. */ )
//===========================================================================
/// \brief  
///     Scale the current bounds. Define a sub-rectangle of the current restriction.
///   These parameters apply to the current restriction as if it were the entire unit square
///   or unit cube. For example for the "r" variable the transformation from old values of
///    (ra,rb) to new values of (ra,rb) is defined by:
///   \begin{align*}
///      rba &= rb-ra \\
///      rb &= ra+rb\_ ~rba \\
///      ra &= ra+ra\_ ~rba
///   \end{align*}
///    
/// \param ra_,rb_,sa_,sb_,ta_,tb_ (input): These parameters define a 
///  sub-rectangle of the current restriction.
//===========================================================================
{
  real rba=rb-ra, sba=sb-sa, tba=tb-ta;

  rb=ra+rb_*rba;
  ra=ra+ra_*rba;
  sb=sa+sb_*sba;
  sa=sa+sa_*sba;
  tb=ta+tb_*tba;
  ta=ta+ta_*tba;
  mappingHasChanged();
  return 0;
}

int RestrictionMapping::
getBounds(real & ra_, real & rb_, real & sa_, real & sb_, real & ta_, real & tb_ ) const
// ==========================================================================================
/// \details 
///   Get the bounds for a restriction mapping.
///    {\tt RestrictionMapping} for further details.
/// \param ra_,rb_,sa_,sb_,ta_,tb_ (output): 
// ==========================================================================================
{
  ra_=ra;
  rb_=rb;
  sa_=sa;
  sb_=sb;
  ta_=ta;
  tb_=tb;
  return 0;
}

int RestrictionMapping::
setBounds(const real ra_, /* =0. */ 
	  const real rb_, /* =1. */ 
	  const real sa_, /* =0. */
	  const real sb_, /* =1. */
	  const real ta_, /* =0. */
	  const real tb_  /* =1. */ )
//===========================================================================
/// \brief  
///   Set absolute bounds for the restriction.
/// \param ra_,rb_,sa_,sb_,ta_,tb_ (input): Parameters in the definition of the 
///    {\tt RestrictionMapping}.
//===========================================================================
{
  ra=ra_;
  rb=rb_;
  sa=sa_;
  sb=sb_;
  ta=ta_;
  tb=tb_;
  mappingHasChanged();
  return 0;
}

int RestrictionMapping::
setSpaceIsPeriodic( int axis, bool trueOrFalse /* = true */ )
// =================================================================================
///  Description:
///     Indicate whether the space being restricted is periodic. For example if you
///  restrict an AnnulusMapping then you should set periodic1=true since the Annulus
///  is periodic along axis1
// =================================================================================
{
  spaceIsPeriodic[axis]=trueOrFalse;

  return 0;
}


void RestrictionMapping::
map( const realArray & r, realArray & x, realArray & xr,
                         MappingParameters & params )
{
  if( params.coordinateType != cartesian )
    cerr << "RestrictionMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    x(I,axis1)=(rb-ra)*r(I,axis1)+ra; 
    if( domainDimension>1 )
      x(I,axis2)=(sb-sa)*r(I,axis2)+sa; 
    if( domainDimension>2 )
      x(I,axis3)=(tb-ta)*r(I,axis3)+ta; 
  }
  if( computeMapDerivative )
  {
    xr=0.;
    xr(I,axis1,axis1)=rb-ra;
    if( domainDimension>1 )
      xr(I,axis2,axis2)=sb-sa;
    if( domainDimension>2)
      xr(I,axis3,axis3)=tb-ta;
  }
}

//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//=================================================================================
void RestrictionMapping::
basicInverse( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    if( (bool)spaceIsPeriodic[axis1] || (bool)getIsPeriodic(axis1) )  // ********* clean this up -- loop ra[axis] ...
    {
      // shift the inverse to that it is the closest pt to the midpoint of the interval [ra,rb]
      real c=(ra+rb)*.5;  // midpoint.
      r(I,axis1) = fmod(x(I,axis1)+(1.5-c),1.)+(c-.5);
      r(I,axis1)=(r(I,axis1)-ra)*(1./(rb-ra)); 
    }
    else    
      r(I,axis1)=(x(I,axis1)-ra)*(1./(rb-ra)); 

    if( domainDimension>1 )
    {
      if( (bool)spaceIsPeriodic[axis2] || (bool)getIsPeriodic(axis2) )
      {
	// shift the inverse to that it is the closest pt to the midpoint of the interval [sa,sb]
	real c=(sa+sb)*.5;  // midpoint.
	r(I,axis2) = fmod(x(I,axis2)+(1.5-c),1.)+(c-.5);
	r(I,axis2)=(r(I,axis2)-sa)*(1./(sb-sa)); 
      }
      else    
	r(I,axis2)=(x(I,axis2)-sa)*(1./(sb-sa)); 
    }
    //  r(I,axis2)=(x(I,axis2)-sa)*(1./(sb-sa)); 
    if( domainDimension>2 )
    {
      if( (bool)spaceIsPeriodic[axis3] || (bool)getIsPeriodic(axis3) )
      {
	// shift the inverse to that it is the closest pt to the midpoint of the interval [ta,tb]
	real c=(ta+tb)*.5;  // midpoint.
	r(I,axis3) = fmod(x(I,axis3)+(1.5-c),1.)+(c-.5);
	r(I,axis3)=(r(I,axis3)-ta)*(1./(tb-ta)); 
      }
      else    
	r(I,axis3)=(x(I,axis3)-ta)*(1./(tb-ta)); 
    }
    //  r(I,axis3)=(x(I,axis3)-ta)*(1./(tb-ta)); 

  }
  if( computeMapDerivative )
  {
    rx=0.;
    rx(I,axis1,axis1)=1./(rb-ra);
    if( domainDimension>1 )
      rx(I,axis2,axis2)=1./(sb-sa);
    if( domainDimension>2 )
      rx(I,axis3,axis3)=1./(tb-ta);
  }
}
  

void RestrictionMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr,
                         MappingParameters & params )
{
  if( params.coordinateType != cartesian )
    cerr << "RestrictionMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    x(I,axis1)=(rb-ra)*r(I,axis1)+ra; 
    if( domainDimension>1 )
      x(I,axis2)=(sb-sa)*r(I,axis2)+sa; 
    if( domainDimension>2 )
      x(I,axis3)=(tb-ta)*r(I,axis3)+ta; 
  }
  if( computeMapDerivative )
  {
    xr=0.;
    xr(I,axis1,axis1)=rb-ra;
    if( domainDimension>1 )
      xr(I,axis2,axis2)=sb-sa;
    if( domainDimension>2)
      xr(I,axis3,axis3)=tb-ta;
  }
}

//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//=================================================================================
void RestrictionMapping::
basicInverseS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    if( (bool)spaceIsPeriodic[axis1] || (bool)getIsPeriodic(axis1) )  // ********* clean this up -- loop ra[axis] ...
    {
      // shift the inverse to that it is the closest pt to the midpoint of the interval [ra,rb]
      real c=(ra+rb)*.5;  // midpoint.
      r(I,axis1) = fmod(x(I,axis1)+(1.5-c),1.)+(c-.5);
      r(I,axis1)=(r(I,axis1)-ra)*(1./(rb-ra)); 
    }
    else    
      r(I,axis1)=(x(I,axis1)-ra)*(1./(rb-ra)); 

    if( domainDimension>1 )
    {
      if( (bool)spaceIsPeriodic[axis2] || (bool)getIsPeriodic(axis2) )
      {
	// shift the inverse to that it is the closest pt to the midpoint of the interval [sa,sb]
	real c=(sa+sb)*.5;  // midpoint.
	r(I,axis2) = fmod(x(I,axis2)+(1.5-c),1.)+(c-.5);
	r(I,axis2)=(r(I,axis2)-sa)*(1./(sb-sa)); 
      }
      else    
	r(I,axis2)=(x(I,axis2)-sa)*(1./(sb-sa)); 
    }
    //  r(I,axis2)=(x(I,axis2)-sa)*(1./(sb-sa)); 
    if( domainDimension>2 )
    {
      if( (bool)spaceIsPeriodic[axis3] || (bool)getIsPeriodic(axis3) )
      {
	// shift the inverse to that it is the closest pt to the midpoint of the interval [ta,tb]
	real c=(ta+tb)*.5;  // midpoint.
	r(I,axis3) = fmod(x(I,axis3)+(1.5-c),1.)+(c-.5);
	r(I,axis3)=(r(I,axis3)-ta)*(1./(tb-ta)); 
      }
      else    
	r(I,axis3)=(x(I,axis3)-ta)*(1./(tb-ta)); 
    }
    //  r(I,axis3)=(x(I,axis3)-ta)*(1./(tb-ta)); 

  }
  if( computeMapDerivative )
  {
    rx=0.;
    rx(I,axis1,axis1)=1./(rb-ra);
    if( domainDimension>1 )
      rx(I,axis2,axis2)=1./(sb-sa);
    if( domainDimension>2 )
      rx(I,axis3,axis3)=1./(tb-ta);
  }
}
  

//=================================================================================
// get a mapping from the database
//=================================================================================
int RestrictionMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase *subDir = dir.virtualConstructor();
  dir.find(*subDir,name,"Mapping");
  if( debug & 4 )
    printF("Entering RestrictionMapping::get\n");

  subDir->get( RestrictionMapping::className,"className" ); 
  if( RestrictionMapping::className != "RestrictionMapping" )
  {
    printF("RestrictionMapping::get ERROR in className!\n");
  }
  subDir->get( ra,"ra" );
  subDir->get( rb,"rb" );
  subDir->get( sa,"sa" );
  subDir->get( sb,"sb" );
  subDir->get( ta,"ta" );
  subDir->get( tb,"tb" );
  subDir->get( spaceIsPeriodic,"spaceIsPeriodic",3 );
  Mapping::get( *subDir, "Mapping" );
  delete subDir;
  mappingHasChanged();
  return 0;
}
int RestrictionMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase *subDir = dir.virtualConstructor();      // create a derived data-base object
  dir.create(*subDir,name,"Mapping");                      // create a sub-directory 

  subDir->put( RestrictionMapping::className,"className" );
  subDir->put( ra,"ra" );
  subDir->put( rb,"rb" );
  subDir->put( sa,"sa" );
  subDir->put( sb,"sb" );
  subDir->put( ta,"ta" );
  subDir->put( tb,"tb" );
  subDir->put( spaceIsPeriodic,"spaceIsPeriodic",3 );
  Mapping::put( *subDir, "Mapping" );
  delete subDir;
  return 0;
}

Mapping *RestrictionMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==RestrictionMapping::className )
    retval = new RestrictionMapping();
  return retval;
}

    

//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int RestrictionMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      // "specify corners", 
      "!RestictionMapping",
      "set corners",
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
      "set corners        : Specify the corners of the restriction",
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

  gi.appendToTheDefaultPrompt("Restriction>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="set corners" ) 
    {
      if( domainDimension==1 )
      {
        gi.inputString(line,sPrintF(buff,"Enter ra,rb (default=(%e,%e)): ",ra,rb));
        if( line!="" ) sScanF(line,"%e %e ",&ra,&rb);
      }
      else if( domainDimension==2 )
      {
        gi.inputString(line,sPrintF(buff,"Enter ra,rb, sa,sb (default=[%e,%e]x[%e,%e]): ",
          ra,rb,sa,sb));
        if( line!="" ) sScanF(line,"%e %e %e %e ",&ra,&rb,&sa,&sb);
      }
      else
      {
        gi.inputString(line,sPrintF(buff,"Enter ra,rb, sa,sb, ta,tb (default=[%e,%e]x[%e,%e]x[%e,%e]): ",
          ra,rb,sa,sb,ta,tb));
        if( line!="" ) sScanF(line,"%e %e %e %e %e %e",&ra,&rb,&sa,&sb,&ta,&tb);
      }
      mappingHasChanged();
    }
    else if( answer=="specify corners" )  // this is kept for compatibility
    {
      if( domainDimension==1 )
      {
        gi.inputString(line,sPrintF(buff,"Enter ra,rb (default=(%e,%e)): ",ra,rb));
        if( line!="" ) sScanF(line,"%e %e ",&ra,&rb);
      }
      else if( domainDimension==2 )
      {
        gi.inputString(line,sPrintF(buff,"Enter ra,sa,rb,sb (default=(%e,%e,%e,%e)): ",
          ra,sa,rb,sb));
        if( line!="" ) sScanF(line,"%e %e %e %e ",&ra,&sa,&rb,&sb);
      }
      else
      {
        gi.inputString(line,sPrintF(buff,"Enter ra,sa,ta,rb,sb,tb (default=(%e,%e,%e,%e,%e,%e)): ",
          ra,sa,ta,rb,sb,tb));
        if( line!="" ) sScanF(line,"%e %e %e %e %e %e",&ra,&sa,&ta,&rb,&sb,&tb);
      }
      mappingHasChanged();
    }
    else if( answer=="show parameters" )
    {
      printf(" (ra,sa,ta,rb,sb,tb)=(%e,%e,%e,%e,%e,%e)\n",ra,sa,ta,rb,sb,tb);
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
