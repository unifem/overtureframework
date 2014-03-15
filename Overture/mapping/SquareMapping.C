#include "SquareMapping.h"
#include "MappingInformation.h"
#include "ParallelUtility.h"

SquareMapping::
SquareMapping( const real xa_, const real xb_, const real ya_, const real yb_ ) 
: Mapping(2,2,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief  Build a mapping for a square with given bounds.
/// \param xa_, xb_, ya_, yb_ (input) : The square is [xa_,xb_]$\times$[ya_,yb_].
//===========================================================================
{ 
  SquareMapping::className="SquareMapping";
  setName( Mapping::mappingName,"square");
  setGridDimensions( axis1,11 );
  setGridDimensions( axis2,11 );
  setVertices(xa_,xb_,ya_,yb_,0.);
  setBasicInverseOption(canInvert);  // basicInverse is available
  inverseIsDistributed=false;
  setMappingCoordinateSystem( rectangular );  // for optimizing derivatives
  mappingHasChanged();

}

// Copy constructor is deep by default
SquareMapping::
SquareMapping( const SquareMapping & map, const CopyType copyType )
{
  SquareMapping::className="SquareMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "SquareMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

SquareMapping::
~SquareMapping()
{ if( debug & 4 )
  cout << " SquareMapping::Destructor called" << endl;
}

SquareMapping & SquareMapping::
operator=( const SquareMapping & x )
{
  if( SquareMapping::className != x.getClassName() )
  {
    cout << "SquareMapping::operator= ERROR trying to set a SquareMapping = to a" 
      << " mapping of type " << x.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(x);            // call = for derivee class
  xa=x.xa;
  ya=x.ya;
  xb=x.xb;
  yb=x.yb;
  z=x.z;
  return *this;
}

RealArray SquareMapping::
getBoundingBox( const int & side /* =-1 */, const int & axis /* =-1 */ ) const
// =====================================================================================
// 
// Specialized version. Supplying this functions means we can avoid building the grid in
// order to determine the bounding box for intersect etc.
// 
// /Description:
//   Return the bounding box for the Mapping (if side<0 and axis<0) or the bounding
//   box for a particular side.
//   /side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
//     and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{
   RealArray bb(2,3);

   bb(0,0)=xa; bb(1,0)=xb; 
   bb(0,1)=ya; bb(1,1)=yb; 

  if( side<0 && axis<0 )
  {
    return bb;
  }
  
  if( !validSide( side ) || !validAxis( axis ) )
  {
    cout << " SquareMapping::getBoundingBox: Invalid arguments " << endl;
    Overture::abort("error");
  }

  bb(0,axis)=bb(side,axis);
  bb(1,axis)=bb(side,axis);
  return bb;

}

int SquareMapping::
getBoundingBox( const IntegerArray & indexRange, const IntegerArray & gridIndexRange_,
                RealArray & xBounds, bool local /* =false */ ) const
// =====================================================================================
// 
// Specialized version. Supplying this functions means we can avoid building the grid in
// order to determine the bounding box for intersect etc.
// 
// /Description:
//   Return the bounding box, xBounds, for the set of grid points spanned by 
//   indexRange. 
//
// /indexRange(0:1,0:2) (input) : range of indicies, i\_m=indexRange(0,m),...,indexRange(1,m)
// /gridIndexRange\_(0:1,0:2) (input) : Normally these should match the gridIndexRange of the Mapping.
//    This argument is used to double check that this is true.
// /xBounds(0:1,0:2) : bounds
// /local (input) : if local=true then only compute the min and max over points on this processor, otherwise
//                  compute the min and max over all points on all processors
//
// /Return values: 0=success, 1=indexRange values are invalid.
// =====================================================================================
{
  real xya[2]={xa,ya}, xyb[2]={xb,yb}; // 
  for( int dir=0; dir<2; dir++ )
  {
    real dx = (xyb[dir]-xya[dir])/max(1,gridIndexRange_(1,dir)-gridIndexRange_(0,dir));
    xBounds(0,dir)=xya[dir]+ (indexRange(0,dir)-gridIndexRange_(0,dir))*dx;
    xBounds(1,dir)=xyb[dir]+ (indexRange(1,dir)-gridIndexRange_(1,dir))*dx;
  }  
  xBounds(0,2)=0.;
  xBounds(1,2)=0.;
  return 0;
}

int SquareMapping::
getBoundingBox( const RealArray & rBounds, RealArray & xBounds ) const
// =====================================================================================
// 
// Specialized version. Supplying this functions means we can avoid building the grid in
// order to determine the bounding box for intersect etc.
// 
// /Description:
//   Return the bounding box, xBounds, for the range space that corresponds to the
//   bounding box, rBounds, in the domain space. 
// =====================================================================================
{
  return Mapping::getBoundingBox(rBounds,xBounds);
}

void SquareMapping::
map( const realArray & r_, realArray & x_, realArray & xr_, MappingParameters & params )
{
  if( params.coordinateType != cartesian )
    cerr << "SquareMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r_,x_,xr_,base,bound,computeMap,computeMapDerivative );

  #ifdef USE_PPP
    realSerialArray x;  getLocalArrayWithGhostBoundaries(x_,x);
    realSerialArray r;  getLocalArrayWithGhostBoundaries(r_,r);
    realSerialArray xr; getLocalArrayWithGhostBoundaries(xr_,xr);

    int n1a = max(I.getBase() , r.getBase(0)+r_.getGhostBoundaryWidth(0));
    int n1b = min(I.getBound(),r.getBound(0)-r_.getGhostBoundaryWidth(0));
    bool ok = n1b>=n1a;
    if( !ok ) return;
    
    I=Range(n1a,n1b);
    
  #else
    realSerialArray & x = x_;
    const realSerialArray & r = r_;
    realSerialArray & xr = xr_;
  #endif

  if( computeMap )
  {
    x(I,axis1)=(xb-xa)*r(I,axis1)+xa; 
    x(I,axis2)=(yb-ya)*r(I,axis2)+ya; 
    if( rangeDimension==3 )
      x(I,axis3)=z;
  }
  if( computeMapDerivative )
  {
    xr(I,axis1,axis1)=xb-xa;
    xr(I,axis1,axis2)=0.;
    xr(I,axis2,axis1)=0.;
    xr(I,axis2,axis2)=yb-ya;
    if( rangeDimension==3 )
      xr(I,axis3,Range(0,1))=0.;
  }
}

//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//=================================================================================
void SquareMapping::
basicInverse( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    r(I,axis1)=(x(I,axis1)-xa)/(xb-xa); 
    r(I,axis2)=(x(I,axis2)-ya)/(yb-ya); 
    periodicShift(r,I);   // shift r in any periodic directions
  }
  if( computeMapDerivative )
  {
    rx(I,axis1,axis1)=1./(xb-xa);
    rx(I,axis1,axis2)=0.;
    rx(I,axis2,axis1)=0.;
    rx(I,axis2,axis2)=1./(yb-ya);
    if( rangeDimension==3 )
      rx(I,Range(0,1),axis3)=0.;
  }
}
  
void SquareMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr,
                         MappingParameters & params )
{
  if( params.coordinateType != cartesian )
    cerr << "SquareMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    x(I,axis1)=(xb-xa)*r(I,axis1)+xa; 
    x(I,axis2)=(yb-ya)*r(I,axis2)+ya; 
    if( rangeDimension==3 )
      x(I,axis3)=z;
  }
  if( computeMapDerivative )
  {
    xr(I,axis1,axis1)=xb-xa;
    xr(I,axis1,axis2)=0.;
    xr(I,axis2,axis1)=0.;
    xr(I,axis2,axis2)=yb-ya;
    if( rangeDimension==3 )
      xr(I,axis3,Range(0,1))=0.;
  }
}

//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//=================================================================================
void SquareMapping::
basicInverseS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  if( computeMap )
  {
    r(I,axis1)=(x(I,axis1)-xa)/(xb-xa); 
    r(I,axis2)=(x(I,axis2)-ya)/(yb-ya); 
    periodicShift(r,I);   // shift r in any periodic directions
  }
  if( computeMapDerivative )
  {
    rx(I,axis1,axis1)=1./(xb-xa);
    rx(I,axis1,axis2)=0.;
    rx(I,axis2,axis1)=0.;
    rx(I,axis2,axis2)=1./(yb-ya);
    if( rangeDimension==3 )
      rx(I,Range(0,1),axis3)=0.;
  }
}
  

//=================================================================================
// get a mapping from the database
//=================================================================================
int SquareMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");

  subDir.get( SquareMapping::className,"className" ); 
  if( SquareMapping::className != "SquareMapping" )
  {
    cout << "SquareMapping::get ERROR in className!" << endl;
  }
  subDir.get( xa,"xa" );
  subDir.get( ya,"ya" );
  subDir.get( xb,"xb" );
  subDir.get( yb,"yb" );
  subDir.get( z ,"z" );
  Mapping::get( subDir, "Mapping" );
  delete &subDir;
  mappingHasChanged();
  return 0; 
}

int SquareMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( SquareMapping::className,"className" );
  subDir.put( xa,"xa" );
  subDir.put( ya,"ya" );            
  subDir.put( xb,"xb" );
  subDir.put( yb,"yb" );            
  subDir.put( z ,"z" );
  Mapping::put( subDir, "Mapping" );
  delete &subDir;
  return 0;
}

Mapping *SquareMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==SquareMapping::className )
    retval = new SquareMapping();
  return retval;
}

real SquareMapping::
getVertices(real & xa_ , real & xb_ , real & ya_ , real & yb_  ) const
//===========================================================================
/// \details  return the vertices of the square.
/// \param xa_, xb_, ya_, yb_ (output) : The square is [xa_,xb_]$\times$[ya_,yb_].
/// \return  is the z-level 
//===========================================================================
{
  xa_=xa;
  xb_=xb;
  ya_=ya;
  yb_=yb;
  return z;
}



void SquareMapping::
setVertices(const real xa_ /* =0. */, 
	    const real xb_ /* =1. */, 
	    const real ya_ /* =0. */,
	    const real yb_ /* =1. */,
            const real z_  /* =0. */ )
//===========================================================================
/// \brief  Build a mapping for a square with given corners.
/// \param xa_, xb_, ya_, yb_ (input) : The square is [xa_,xb_]$\times$[ya_,yb_].
/// \param z_ : z level if the rangeDimension is 3.
//===========================================================================
{
  xa=xa_;
  xb=xb_;
  ya=ya_;
  yb=yb_;
  z=z_;
  setPeriodVector(0,axis1, xb-xa);  // in case we have derivativePeriodic
  setPeriodVector(1,axis1, 0.   ); 
  setPeriodVector(0,axis2, 0.   );
  setPeriodVector(1,axis2, yb-ya);
  
}

// ********* Add to Mapping
static void 
updateMappingParametersDialog( Mapping *map, DialogData &dialog )
{
  aString buff;
  int i;
  int domainDimension = map->getDomainDimension();

  dialog.setTextLabel(0,map->getName(Mapping::mappingName));

  if ( domainDimension==1 )
  {
    sPrintF(buff,"%i",map->getGridDimensions(0));
  }
  else if ( domainDimension==2 )
  {
    sPrintF(buff,"%i %i",map->getGridDimensions(0), map->getGridDimensions(1));
  }
  else if ( domainDimension==3 )
  {
    sPrintF(buff,"%i %i %i",map->getGridDimensions(0), map->getGridDimensions(1), 
	    map->getGridDimensions(2));
  }

  dialog.setTextLabel(1,buff);
  
  if ( map->approximateGlobalInverse==NULL ) 
    dialog.setSensitive(false, DialogData::toggleButtonWidget, 0);
  else
  {
    dialog.setSensitive(true, DialogData::toggleButtonWidget, 0);
    int inverseOnOff = map->approximateGlobalInverse->usingRobustInverse();
    dialog.setToggleState(0, inverseOnOff);
  }

  buff="";

  int offset=2;
  int axis,side;
  for ( i=offset; i<offset+2*map->getDomainDimension(); i++ )
  {
    axis = (i-offset)/2;
    side = (i-offset)%2;
    sPrintF(buff, "%i", map->getBoundaryCondition(side,axis));
    dialog.setTextLabel(i,buff);
    buff="";
  }

  offset = offset+2*map->getDomainDimension();
  for ( i=offset; i<offset+2*map->getDomainDimension(); i++ )
  {
    axis = (i-offset)/2;
    side = (i-offset)%2;
    sPrintF(buff, "%i", map->getShare(side,axis));
    dialog.setTextLabel(i, buff);
    buff="";
  }

//   offset = offset+2*map->getDomainDimension();

//   for ( i=offset; i<offset+map->getDomainDimension(); i++ )
//     {
//       axis = (i-offset);
//       sPrintF(buff, "%i", map->getIsPeriodic(axis));
//       dialog.setTextLabel(i, buff);
//       buff="";
//     }

  for( axis=0; axis<map->getDomainDimension(); axis++ )
    dialog.getOptionMenu(axis).setCurrentChoice((int)map->getIsPeriodic(axis));
}

    


//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int SquareMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  // Here is the old popup menu
  aString menu[] = 
    {
      "!SquareMapping",
      "set corners",
      "make 3d (toggle)",
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
      "set corners        : Specify the corners of the square",
      "make 3d (toggle)   : turn the square into a surface in 3D",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "mapping parameters : change lines, boundary conditions etc.",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  bool makeThreeDimensional=rangeDimension==3;

  GUIState dialog;

  dialog.setWindowTitle("Square Mapping");
  dialog.setExitCommand("exit", "exit");

  // option menus
//     dialog.setOptionMenuColumns(1);

//     aString opCommand1[] = {"unit square",
// 			    "helical wire",
//                             "fillet for two cylinders",
//                             "blade",
// 			    ""};
    
//     dialog.addOptionMenu( "type:", opCommand1, opCommand1, mappingType); 


  aString colourBoundaryCommands[] = { "colour by bc",
			               "colour by share",
			               "" };
  // dialog.addRadioBox("boundaries:",colourBoundaryCommands, colourBoundaryCommands, 0 );
  dialog.addOptionMenu("boundaries:",colourBoundaryCommands, colourBoundaryCommands, 0 );

  aString cmds[] = {"mapping parameters...",
		    "show parameters",
                    "plot",
		    "help",
		    ""};
  int numberOfPushButtons=3;  // number of entries in cmds
  int numRows=numberOfPushButtons; // (numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

  aString tbCommands[] = {"three dimensional",
 			  ""};
  int tbState[10];

  tbState[0] = makeThreeDimensional;
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

  const int numberOfTextStrings=7;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];


  int nt=0;
  textLabels[nt] = "set corners:";  sPrintF(textStrings[nt],"%g,%g, %g,%g",xa,xb,ya,yb);  nt++; 

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);


  // make a dialog sibling for setting general mapping parameters
  DialogData & mappingParametersDialog = dialog.getDialogSibling();
  buildMappingParametersDialog( mappingParametersDialog );

  dialog.buildPopup(menu);
  gi.pushGUI(dialog);

  int len=0;
  aString answer,line; 

  bool plotObject=true;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
  parameters.set(GI_LABEL_GRIDS_AND_BOUNDARIES,true); // turn on plotting of colourd squares

  gi.appendToTheDefaultPrompt("Square>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getAnswer(answer,"");  // gi.getMenuItem(menu,answer);
 

    if( len=answer.matches("set corners:") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e %e",&xa,&xb,&ya,&yb);
      if( !gi.isGraphicsWindowOpen() )
        dialog.setTextLabel("set corners:",sPrintF(answer,"%g,%g, %g,%g",xa,xb,ya,yb));

      setVertices(xa,xb,ya,yb);
      mappingHasChanged(); 
    }
    else if( dialog.getToggleValue(answer,"three dimensional",makeThreeDimensional) )
    {
      rangeDimension = makeThreeDimensional ? 3 : 2;
      mappingHasChanged();
    }
    else if( getMappingParametersOption(answer,mappingParametersDialog,gi ) )
    {
      // Changes were made to generic mapping parameters such as lines, BC's, share, periodicity
      printF("Answer=%s found in getMappingParametersOption\n",(const char*)answer);
    }
    else if( answer=="colour by bc" || 
             answer=="colour by share" )
    {
      if( answer=="colour by bc" )
      {
        parameters.set(GI_BOUNDARY_COLOUR_OPTION,GraphicsParameters::colourByBoundaryCondition);
        // dialog.getRadioBox(0).setCurrentChoice(0);
        dialog.getOptionMenu("boundaries:").setCurrentChoice(0);
      }
      else if( answer=="colour by share" )
      {
        parameters.set(GI_BOUNDARY_COLOUR_OPTION,GraphicsParameters::colourByShare);
        // dialog.getRadioBox(0).setCurrentChoice(1);
        dialog.getOptionMenu("boundaries:").setCurrentChoice(1);
      }
    }
    else if( answer=="set corners" ) 
    {
      gi.inputString(line,sPrintF(buff,"Enter xa,xb, ya,yb (default=[%e,%e]x[%e,%e]): ",
          xa,xb,ya,yb));
      if( line!="" ) sScanF(line,"%e %e %e %e ",&xa,&xb,&ya,&yb);
      setVertices(xa,xb,ya,yb);
      
      mappingHasChanged(); 
    }
    else if( answer=="specify corners" ) // old way
    {
      gi.inputString(line,sPrintF(buff,"Enter xa,ya,xb,yb (default=(%e,%e,%e,%e)): ",
          xa,ya,xb,yb));
      if( line!="" ) sScanF(line,"%e %e %e %e ",&xa,&ya,&xb,&yb);
      setVertices(xa,xb,ya,yb);
      
      mappingHasChanged(); 
    }
    else if( answer=="make 3d (toggle)" )
    {
      if( rangeDimension==2 )
      {
        rangeDimension=3;
        gi.inputString(line,sPrintF(buff,"Enter the z value for the grid (default=%e): ",z));
        if( line!="" ) sScanF( line,"%e",&z);
      }
      else
      {
        rangeDimension=2;
      }
      mappingHasChanged();
    }
    else if( updateWithCommand(mapInfo, answer) )
    {
      // *older way* changes were made to generic mapping parameters such as lines, BC's, share, periodicity
    }
    else if( answer=="show parameters" )
    {
      printf(" (xa,ya,xb,yb)=(%e,%e,%e,%e) \n",xa,ya,xb,yb);
      if( rangeDimension==3 )
        printf(" z = %e \n",z);
      display();
    }
    else if( answer=="plot" )
    {
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters); 
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
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
      plotObject=true;
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

  gi.popGUI(); // restore the previous GUI

  return 0;
}
