// --------------------------------------------------------------------------------------------------------
// Version 1.0  : Summer 1996 - Eugene Sy
//         1.1  : 961129 - Bill Henshaw
//                o Changes for new Mapping format (neww array ordering in calles to map, inverseMap)
//         1.2  : 971030 - Bill Henshaw. 
//
// --------------------------------------------------------------------------------------------------------

#include "EllipticTransform.h"
#include "EllipticGridGenerator.h"
#include "TridiagonalSolver.h"
#include "ComposeMapping.h"
#include "DataPointMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "SquareMapping.h"

EllipticTransform::
EllipticTransform() 
: Mapping(2,2,parameterSpace,cartesianSpace) 
//===========================================================================
/// \brief  
///     Create a mapping that can be used to generate an elliptic grid
///    from an existing grid. This can be useful to smooth out an existing Mapping.
///  
//===========================================================================
{ 
  EllipticTransform::className="EllipticTransform";
  setName( Mapping::mappingName,"ellipticTransform");
  userMap=NULL;
  compose=FALSE;
  dpm=NULL;
  ellipticGridDefined=FALSE;
  ellipticGridGenerator=NULL;
  
  setGridDimensions( axis1,11 );
  setGridDimensions( axis2,11 );

  mappingHasChanged();
}

// Copy constructor is deep by default
EllipticTransform::
EllipticTransform( const EllipticTransform& map, const CopyType copyType )
{
  EllipticTransform::className="EllipticTransform";
  userMap=NULL;
  compose=FALSE;
  dpm=NULL;
  ellipticGridGenerator=NULL;
  
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "EllipticTransform:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

EllipticTransform::
~EllipticTransform()
{ 
  if( debug & 4 )
    cout << " EllipticTransform::Destructor called" << endl;

  if( dpm!=NULL && dpm->decrementReferenceCount()==0 )
    delete dpm;
  delete ellipticGridGenerator;
  
}

EllipticTransform & EllipticTransform::
operator=( const EllipticTransform & X )
{
  if( EllipticTransform::className != X.getClassName() )
  {
    cout << "EllipticTransform::operator= ERROR trying to set a EllipticTransform= to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  // *********** FINISH THIS ****************
  ellipticGridDefined=X.ellipticGridDefined;
  ellipticGridGenerator=X.ellipticGridGenerator;

  userMap=X.userMap;     // **** we only copy the pointer here **** fix
  compose=X.compose;
  ellipticGridDefined=X.ellipticGridDefined;
  ellipticGridGenerator=X.ellipticGridGenerator;  // copies pointer only!
  
  if( dpm!=NULL && dpm->decrementReferenceCount()==0 )
    delete dpm; 
  dpm=X.dpm;
  if( dpm!=NULL )
    dpm->incrementReferenceCount();

  this->Mapping::operator=(X);            // call = for base class
  return *this;
}

int EllipticTransform::
get( const GenericDataBase & dir, const aString & name)
//===========================================================================
/// \details 
///     Get a mapping from the database.
/// \param dir (input): get the Mapping from a sub-directory of this directory.
/// \param name (input) : name of the sub-directory to look for the Mapping in.
//===========================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering EllipticTransform::get" << endl;

  subDir.get( EllipticTransform::className,"className" ); 
  if( EllipticTransform::className != "EllipticTransform" )
  {
    cout << "EllipticTransform::get ERROR in className!" << endl;
  }
  subDir.get(ellipticGridDefined,"ellipticGridDefined");

  // ****** NOTE **** we should be saving all the other arrays that define the elliptic transformation *********

  aString userMapClassName;
  subDir.get(userMapClassName,"userMapClassName");  
  userMap = Mapping::makeMapping( userMapClassName ); // ***** this does a new -- who will delete? ***
  if( userMap==NULL )
  {
    cout << "EllipticTransform::get:ERROR: reading in the userMap with className=" << userMapClassName << endl;
    return 1;
  }
  userMap->get(subDir,"userMap");
  userMap->incrementReferenceCount();
  
  if( dpm==NULL )
    dpm = new DataPointMapping;
  dpm->get(subDir,"DataPointMapping");
  dpm->incrementReferenceCount();

  Mapping::get( subDir, "Mapping" );
  mappingHasChanged();
  delete &subDir;
  return 0;
}

int EllipticTransform::
put( GenericDataBase & dir, const aString & name) const
//===========================================================================
/// \details 
///     Save a mapping into a database.
/// \param dir (input): put the Mapping into a sub-directory of this directory.
/// \param name (input) : name of the sub-directory to save the Mapping in.
//===========================================================================
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 
  
  subDir.put( EllipticTransform::className,"className" );
  subDir.put(ellipticGridDefined,"ellipticGridDefined");
  subDir.put(userMap->getClassName(),"userMapClassName");  // save the class name so we can "make" this type in get
  userMap->put(subDir,"userMap");
  dpm->put(subDir,"DataPointMapping");

  Mapping::put( subDir, "Mapping" );
  delete & subDir;
  return 0;
}


Mapping *EllipticTransform::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==EllipticTransform::className )
    retval = new EllipticTransform();
  return retval;
}


void EllipticTransform::
map(const realArray & r, 
    realArray & x, 
    realArray & xr /* = Overture::nullRealDistributedArray() */,
    MappingParameters & params /* =Overture::nullMappingParameters() */ )
{
  if( ellipticGridDefined )
    dpm->map(r,x,xr,params);
  else if( userMap!=NULL )
    userMap->map(r,x,xr,params);
  else
    cout << "EllipticTransform::map:ERROR: no mapping defined yet! \n";
}

void EllipticTransform::
inverseMap(const realArray & x, 
	   realArray & r, 
	   realArray & rx /* = Overture::nullRealDistributedArray() */,
	   MappingParameters & params /* =Overture::nullMappingParameters() */ )
{
  if( ellipticGridDefined )
    dpm->inverseMap(x,r,rx,params);
  else if( userMap!=NULL )
    userMap->inverseMap(x,r,rx,params);
  else
    cout << "EllipticTransform::inverseMap:ERROR: no mapping defined yet! \n";
}

int EllipticTransform::
setup()
{
  // set the domain and range for userMap 

  setName(mappingName,aString("elliptic-")+userMap->getName(mappingName));
  setDomainDimension(userMap->getDomainDimension());
  setRangeDimension(userMap->getRangeDimension());

  if( dpm==NULL )
  {
    dpm = new DataPointMapping;
    dpm->incrementReferenceCount();
  }
  

  // get grid resolution from userMap:
  int axis;
  for( axis=axis1; axis<domainDimension; axis++ )
    setGridDimensions(axis,userMap->getGridDimensions(axis));

  dpm->setRangeSpace(parameterSpace);
  dpm->setDomainDimension(userMap->getDomainDimension());
  dpm->setRangeDimension(userMap->getRangeDimension());
  ellipticGridDefined=FALSE;
  for( axis=0; axis<domainDimension; axis++)
  {
    for( int side=Start; side<=End; side++ )
    {
      setBoundaryCondition(side,axis,userMap->getBoundaryCondition(side,axis));
      setShare(side,axis,userMap->getShare(side,axis));
      dpm->setBoundaryCondition(side,axis,userMap->getBoundaryCondition(side,axis));
      dpm->setShare(side,axis,userMap->getShare(side,axis));
    }		
    setIsPeriodic(axis,userMap->getIsPeriodic(axis));
    dpm->setIsPeriodic(axis,userMap->getIsPeriodic(axis));
  }

  mappingHasChanged();
  return 0;
}


void EllipticTransform::
generateGrid(GenericGraphicsInterface *gi /* = NULL */, 
             GraphicsParameters & parameters /* =Overture::nullMappingParameters() */ )
//===========================================================================
/// \details 
///     This function performs the iterations to solve the elliptic grid equations.
/// \param gi (input) : supply a graphics interface if you want to see the grid as it
///     is being computed.
/// \param parameters (input) : optional parameters used by the graphics interface.
//===========================================================================
{
  ellipticGridDefined=TRUE;

  assert( ellipticGridGenerator!=NULL );
  EllipticGridGenerator & gridGenerator = *ellipticGridGenerator;
  

  Range Rr(0,domainDimension-1);
  Range Rx(0,rangeDimension-1);

  //Get grid index (interior + boundary without ghost points)
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  I1=I2=I3=0;
  for( int axis=0; axis<domainDimension; axis++ )
    Iv[axis]=Range(0,getGridDimensions(axis)-1);
    

  if( xe.getLength(0)<I1.getBound() || xe.getLength(1)<I2.getBound() || xe.getLength(2)<I3.getBound() )
    xe.redim(I1,I2,I3,Rx);

  
  gridGenerator.update(*dpm,gi,parameters);

  // **** fix this *****
/* --
  xe.reshape(I1,I2,I3,Rx);
  xe(I1,I2,I3,Rx)=gridGenerator.solution()(I1,I2,I3,Rx);  // **** DPM should use ghost points from elliptic
--- */

/* --
   //Reset DataPointMapping and project the boundary, and plot every few iterations
  if( rangeDimension==2 )
    xe.reshape(xe.dimension(0),xe.dimension(1),xe.dimension(3));
  resetDataPointMapping(xe,I1,I2,I3);
--- */

  gi->erase();
  fflush(stdout);
  printf("\n");
  PlotIt::plot(*gi,*this,parameters);   // *** recompute every time ?? ***
  gi->redraw(TRUE);   // force a redraw

}
	
void EllipticTransform::
resetDataPointMapping( realArray & x, Index Ig, Index Jg, Index Kg )
// Protected function.
// Change the data point mapping to be consistent with this x
// Project the boundary back onto the original mapping
{
/* -----
  if( project )
  {
    x.reshape(iDim*jDim,rangeDimension);
    rTilde.reshape(iDim*jDim,domainDimension);

    userMap->inverseMap(x,rTilde);

    rTilde.reshape(iDim,jDim,domainDimension);
    // project boundaries of rTilde

//  for( int axis=axis1; axis<domainDimension; axis++ )
//    for( int side=Start; side<=End; side++ )

    Range I(0,iDim-1);
    Range J(0,jDim-1);
    rTilde(I , 0,axis2)=0.;
    rTilde(I ,jb,axis2)=1.;
    rTilde(0 ,J ,axis1)=0.;
    rTilde(ib,J ,axis1)=1.;
  
    dpm->setDataPoints(rTilde,domainDimension,domainDimension);

    rTilde.reshape(iDim*jDim,domainDimension);
    userMap->map(rTilde,x);

    int klength=Kg.getBound()-Kg.getBase()+1;
    rTilde.reshape(iDim,jDim,domainDimension);
    x.reshape(iDim,jDim,klength,rangeDimension);
  }
  else
  {
    // no projection so we just make a dpm for the given grid points
    dpm->setDataPoints(x,domainDimension,domainDimension);
  }
---- */

  dpm->setDataPoints(x,domainDimension,domainDimension);
  mappingHasChanged();
}


void EllipticTransform::
initialize()
{

}


//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int EllipticTransform::
update( MappingInformation & mapInfo ) 
{

  Mapping *previousMapPointer;
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!EllipticTransform",
      "transform which mapping?",
      "elliptic smoothing",
//      "change resolution for elliptic grid",
      "reset elliptic transform",
      "set order of interpolation",
      "test weight function",
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
      "    Transform a Mapping by Elliptic Grid Generation",
      "transform which mapping? : 		choose the mapping to transform",
      "elliptic smoothing : 			smooth out grid with elliptic transform",
//      "change resolution for elliptic grid:	change iDim,jDim for elliptic solver",
      "reset elliptic transform                 start iterations from scratch",
      "set order of interpolation               order of interpolation for data point mapping",
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
  bool plotObject=FALSE;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  bool mappingChosen= userMap!=NULL;		


  // By default transform the last mapping in the list (if this mapping is uninitialized, mappingChosen==FALSE)
  if( !mappingChosen )
  {
    mappingHasChanged();
    int number= mapInfo.mappingList.getLength();
    for( int i=number-2; i>=0; i-- )
    {
      Mapping *mapPointer=mapInfo.mappingList[i].mapPointer;
      if( mapPointer->getDomainDimension()>=2 && mapPointer->getRangeDimension()>=2 )
      {
        userMap=mapPointer;   // use this one
	previousMapPointer=mapPointer;
        mappingChosen=TRUE;
        setup();
  	initialize();		
        mappingHasChanged();
        plotObject=TRUE;
	break; 
      }
    }
  }
  if( !mappingChosen )
  {
    cout << "EllipticTransfrom:ERROR: no mappings to transform!! \n";
    return 1;
  }

  if( ellipticGridGenerator==NULL )
    ellipticGridGenerator = new EllipticGridGenerator;
  EllipticGridGenerator & gridGenerator = *ellipticGridGenerator;

  gridGenerator.setup(*userMap);
  gridGenerator.startingGrid(userMap->getGrid());  // here are the initial conditions for the grid generator

//This section is the part dealing with menu choices.
  gi.appendToTheDefaultPrompt("EllipticTransform>"); // set the default prompt

  for( int it=0;; it++ )
  {

    if( it==0 && plotObject )
      answer="plotObject";  // plot first time through
    else
      gi.getMenuItem(menu,answer);
    
 
    if( answer=="transform which mapping?" )
    { 
      // make a list of all potential Mappings:
    
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int j=0;
      for( int i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
        if( map.getDomainDimension()>=2 && map.getRangeDimension()>=2 )
	{
	  subListNumbering(j)=i;
          menu2[j++]=map.getName(mappingName);
	}
      }
      menu2[j]="";   // null string terminates the menu
      int mapNumber = gi.getMenuItem(menu2,answer2);
      delete [] menu2;

      if( mapNumber<0 )
        gi.outputString("Error: unknown mapping to transform!");
      else
      {
        mapNumber=subListNumbering(mapNumber);
	if( mapInfo.mappingList[mapNumber].mapPointer==this )
	{
	  cout << "EllipticTransform::ERROR: you cannot transform this mapping, "
	    "this would be recursive!\n";
	  continue;
      	}
      
	// define the mappings to be smoothed:

	userMap = mapInfo.mappingList[mapNumber].mapPointer;
        previousMapPointer=userMap;
        setup();
	
        gridGenerator.setup(*userMap);
        gridGenerator.startingGrid(userMap->getGrid());

        mappingHasChanged();
	mappingChosen=TRUE;
        plotObject=TRUE;
      }
    }
    else if (it ==0) 
      previousMapPointer=userMap;
    else if( answer=="elliptic smoothing" ) 
    {
      if (!mappingChosen) 
	 gi.outputString("Error: Mapping not yet chosen.");
      else 
      {
        generateGrid(&gi,parameters);
        mappingHasChanged();
      }
    }
/* -----
    else if( answer=="change resolution for elliptic grid")
    {
      gi.inputString(line,sPrintF(buff,"Enter i-dimension,j-dimension (default=(%d,%d)): ",
		     iDim,jDim));
      if (line!="")	
      {
      	sscanf(line,"%d %d",&iDim,&jDim);
	userMap->setGridDimensions(axis1, iDim);
	userMap->setGridDimensions(axis2, jDim);
        int idimtmp=iDim-1;
        int jdimtmp=jDim-1;
        while (((idimtmp%2)==0)&&((jdimtmp%2)==0)) {
           (multigridSolver.nlevel)++;
           idimtmp /= 2, jdimtmp /=2;
        }
        if ((idimtmp==1)||(jdimtmp==1)) (multigridSolver.nlevel)--;
        if (idimtmp>jdimtmp) idimtmp=jdimtmp;
        nlevelmax=multigridSolver.nlevel;
        
        {
         realArray r;
         real a;
         Index I1, I2, I3;
         int i, j, k;

         I1=Range(-1, iDim);
         I2=Range(-1, jDim);
         I3=Range(0,0);

         r.redim(I1,I2,I3,domainDimension);
         a=1.0/(real(userMap->getGridDimensions(axis1)-1));
         for (k=I3.getBase();k<=I3.getBound(); k += I3.getStride())
           for (j=I2.getBase();j<=I2.getBound(); j += I2.getStride())
             r(I1,j,k,0).seqAdd(-a,a);
 
           if (rangeDimension>1){
             a=1.0/(real(userMap->getGridDimensions(axis2)-1));
             for (k=I3.getBase();k<=I3.getBound();k+=I3.getStride())
               for (i=I1.getBase();i<=I1.getBound(); i += I1.getStride())
                 r(i,I2,k,1).seqAdd(-a,a);
           }
 
           if (rangeDimension>2){
             a=1.0/(real(userMap->getGridDimensions(axis3)-1));
             for (i=I1.getBase();i<=I1.getBound(); i += I1.getStride())
               for (j=I2.getBase();j<=I2.getBound(); j += I2.getStride())
                 r(i,j,I3,2).seqAdd(-a,a);
           }
 
        }


	setup();
	mappingHasChanged();
	mappingChosen=TRUE;
	plotObject=TRUE;
      }
    }
----- */
    else if( answer=="reset elliptic transform" )
    {
      xe.redim(0);   // this will reset the iterations to scratch
      userMap=previousMapPointer;
      //printf("iDim=%i\t thisiiDim=%i\n",iDim,userMap->getGridDimensions(axis1));
      setup();
	
      //mappingHasChanged();
      mappingChosen=TRUE;
      mappingHasChanged();
      plotObject=TRUE;
      //ellipticGridDefined=TRUE;
    }
    else if( answer=="set order of interpolation" )
    {
      aString menu2[] = { "2nd order",
                         "4th order",
                         "no change", 
                         "" };
      int response=gi.getMenuItem(menu2,answer2);
      if( response>=0 && response<=1 )
      { 
	dpm->setOrderOfInterpolation(2+response*2);   // 2 or 4
      }
    }
    else if( answer=="show parameters" )
    {
      //don't run this until the thing is set!!
      display();
    }
//     else if( answer=="test weight function" )
//     {
//       RealMappedGridFunction weight;
//       gridGenerator.weightFunction(weight);
//       if (!mappingChosen) 
// 	 gi.outputString("Error: Mapping not yet chosen.");
//       else 
//       {
//         generateGrid(&gi,parameters);
//         mappingHasChanged();
//       }
//     }
    else if( answer=="plot" )
    {
      if( !mappingChosen )
      {
	gi.outputString("you must first choose a mapping to transform");
	continue;
      }
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
      gi.stopReadingCommandFile();
    }

    if( plotObject && mappingChosen )
    {
      gi.erase();
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      PlotIt::plot(gi,*this,parameters);  
    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
  
}

