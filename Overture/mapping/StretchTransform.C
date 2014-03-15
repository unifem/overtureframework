#include "StretchTransform.h"
#include "MappingInformation.h"
#include "MappingRC.h"


StretchTransform::
StretchTransform() : ComposeMapping()
{ 
  stretchedSquare=NULL;

  StretchTransform::className="StretchTransform";
  setName( Mapping::mappingName,"stretchTransform");
  setGridDimensions( axis1,11 );
  setGridDimensions( axis2,11 );
  domainDimension=getDomainDimension();
  rangeDimension=getRangeDimension();
  mappingHasChanged();
}

// Copy constructor is deep by default
StretchTransform::
StretchTransform( const StretchTransform & map, const CopyType copyType )
{
  StretchTransform::className="StretchTransform";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "StretchTransform:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

StretchTransform::
~StretchTransform()
{ 
  if( debug & 4 )
    cout << " StretchTransform::Desctructor called" << endl;
}

StretchTransform & StretchTransform::
operator=( const StretchTransform & X )
{
  if( StretchTransform::className != X.getClassName() )
  {
    cout << "StretchTransform::operator= ERROR trying to set a StretchTransform = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->ComposeMapping::operator=(X);            // call = for derivee class
  stretchedSquare=(StretchedSquare*)map1.mapPointer;
  return *this;
}

void StretchTransform::
setMapping( Mapping & map )
// set the mapping for the stretch tranform
{

  bool stretchedMappingwasNewed=false;
  if( stretchedSquare==NULL )
  {
    stretchedMappingwasNewed=true;
    stretchedSquare=new StretchedSquare;
    stretchedSquare->incrementReferenceCount();
  }

  // set the domain and range for the StretchedSquare
  stretchedSquare->setDomainDimension(map.getDomainDimension());
  stretchedSquare->setRangeDimension(map.getDomainDimension());
  // stretching should have the same periodicity as this mapping
  for( int axis=0; axis<map.getDomainDimension(); axis++)
    stretchedSquare->setIsPeriodic(axis,map.getIsPeriodic(axis));

  setMappings(*stretchedSquare,map);
  setMappingProperties(); // set additional mapping properties

  if( stretchedMappingwasNewed )
    stretchedSquare->decrementReferenceCount();

  mappingHasChanged(); 
}

StretchedSquare & StretchTransform::
getStretchedSquare()
{
  if( stretchedSquare!=NULL )
    return *stretchedSquare;
  else
  {
    printf("StretchTransform::ERROR: The stretchedSquare has not be created yet\n");
    throw "error";
  }
  
}

void StretchTransform::
reinitialize()
  // call this function if you change the StretchedSquare
{
  mappingHasChanged();
}



//=================================================================================
// get a mapping from the database
//=================================================================================
int StretchTransform::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase *subDir = dir.virtualConstructor();
  dir.find(*subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering StretchTransform::get" << endl;

  subDir->get( StretchTransform::className,"className" ); 
  if( StretchTransform::className != "StretchTransform" )
  {
    cout << "StretchTransform::get ERROR in className!" << endl;
  }
  ComposeMapping::get( *subDir, "ComposeMapping" );
  stretchedSquare =(StretchedSquare*) map1.mapPointer;
  mappingHasChanged();
  delete subDir;

  if( debug & 8 )
    printF("StretchTransform:get: name=%s, usesDistributedInverse=%i usesDistributedMap=%i\n",
	   (const char*)getName(mappingName),
           (int)usesDistributedInverse(),(int)usesDistributedMap());

  return 0;
}

int StretchTransform::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase *subDir = dir.virtualConstructor();      // create a derived data-base object
  dir.create(*subDir,name,"Mapping");                      // create a sub-directory 
  
  subDir->put( StretchTransform::className,"className" );
  ComposeMapping::put( *subDir, "ComposeMapping" );

  if( debug & 8 )
    printF("StretchTransform:put name=%s, usesDistributedInverse=%i usesDistributedMap=%i\n",
	   (const char*)getName(mappingName),
           (int)usesDistributedInverse(),(int)usesDistributedMap());

  delete subDir;
  return 0;
}

Mapping *StretchTransform::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==StretchTransform::className )
    retval = new StretchTransform();
  return retval;
}

int StretchTransform::
setMappingProperties()
// set additional mapping properties
{
  if( getName(mappingName)=="stretchTransform" )
    setName(mappingName,aString("stretched-")+map2.getName(mappingName));
  
  for( int axis=0; axis<domainDimension; axis++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      setTypeOfCoordinateSingularity(side,axis,map2.getTypeOfCoordinateSingularity(side,axis));
    }
  }
  return 0;
}

//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int StretchTransform::
update( MappingInformation & mapInfo ) 
{
  return update( mapInfo,nullString,NULL );


//    assert(mapInfo.graphXInterface!=NULL);
//    GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
//    char buff[180];  // buffer for sprintf
//    aString menu[] = 
//      {
//        "!StretchTransform",
//        "transform which mapping?",
//        "stretch",
//        "stretch new",
//        "edit unstretched mapping",
//        " ",
//        "lines",
//        "boundary conditions",
//        "share",
//        "mappingName",
//        "periodicity",
//        "show parameters",
//        "plot",
//        "help",
//        "exit", 
//        "" 
//       };
//    aString help[] = 
//      {
//        "    Transform a Mapping by stretching along the Coordinate directions",
//        "transform which mapping? : choose the mapping to transform",
//        "stretch            : define stretching in each coordinate direction",
//        "edit unstretched mapping: make changes to the unstretched mapping",
//        " ",
//        "lines              : specify number of grid lines",
//        "boundary conditions: specify boundary conditions",
//        "share              : specify share values for sides",
//        "mappingName        : specify the name of this mapping",
//        "periodicity        : specify periodicity in each direction",
//        "show parameters    : print current values for parameters",
//        "plot               : enter plot menu (for changing ploting options)",
//        "help               : Print this list",
//        "exit               : Finished with changes",
//        "" 
//      };

//    aString answer,line,answer2; 
//    bool plotObject=TRUE;
//    GraphicsParameters parameters;
//    parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
//    bool mappingChosen= stretchedSquare!=NULL;

//    // By default transform the last mapping in the list (if this mapping is unitialized)
//    if( !mappingChosen )
//    {
//      if( mapInfo.mappingList.getLength()>0 )
//      {
//        // define the mappings to be composed:
//        Mapping & map = *mapInfo.mappingList[mapInfo.mappingList.getLength()-1].mapPointer;
//        setMapping(map);

//  /* ----
//        // set the domain and range for the StretchedSquare
//        if( stretchedSquare==NULL )
//        {
//  	stretchedSquare=new StretchedSquare;
//  	stretchedSquare->incrementReferenceCount();
//        }
      
//        stretchedSquare->setDomainDimension(map.getDomainDimension());
//        stretchedSquare->setRangeDimension(map.getDomainDimension());
//        // stretching should have the same periodicity as this mapping
//        for( int axis=0; axis<map.getDomainDimension(); axis++)
//  	stretchedSquare->setIsPeriodic(axis,map.getIsPeriodic(axis));

//        setMappings(*stretchedSquare,map);
//        setMappingProperties();

//        stretchedSquare->decrementReferenceCount();
//  ---- */

//        mappingChosen=TRUE;
//      }
//      else
//      {
//        cout << "StretchTransfrom:ERROR: no mappings to transform!! \n";
//        return 1;
//      }
//    }
  
//    gi.appendToTheDefaultPrompt("StretchTransform>"); // set the default prompt

//    for( int it=0;; it++ )
//    {
//      if( it==0 && plotObject )
//        answer="plotObject";
//      else
//        gi.getMenuItem(menu,answer);
 
//      if( answer=="transform which mapping?" )
//      { // Make a menu with the Mapping names
//        int num=mapInfo.mappingList.getLength();
//        aString *menu2 = new aString[num+1];
//        for( int i=0; i<num; i++ )
//          menu2[i]=mapInfo.mappingList[i].getName(mappingName);

//        menu2[num]="";   // null string terminates the menu
//        int mapNumber = gi.getMenuItem(menu2,answer2);
//        delete [] menu2;
//        if( mapNumber<0 )
//        {
//          gi.outputString("Error: unknown mapping to revolve!");
//          gi.stopReadingCommandFile();
//        }
//        else
//        {
//  	if( mapInfo.mappingList[mapNumber].mapPointer==this )
//  	{
//  	  cout << "MatrixTransform::ERROR: you cannot transform this mapping, "
//                    "this would be recursive!\n";
//  	  continue;
//  	}
//  	// define the mappings to be composed:
//  	Mapping & map = *mapInfo.mappingList[mapNumber].mapPointer;

//          setMapping( map );
//  /* ---	
//  	// set the domain and range for the StretchedSquare
//  	stretchedSquare->setDomainDimension(map.getDomainDimension());
//  	stretchedSquare->setRangeDimension(map.getDomainDimension());
//  	// stretching should have the same periodicity as this mapping
//  	for( int axis=0; axis<map.getDomainDimension(); axis++)
//  	  stretchedSquare->setIsPeriodic(axis,map.getIsPeriodic(axis));

//  	setMappings(*stretchedSquare,map);
//          setMappingProperties(); // set additional mapping properties
//  --- */

//  	mappingChosen=TRUE;
//  	plotObject=TRUE;
//        }
//      }
//      else if( answer=="stretch" ) 
//      {
//        stretchedSquare->setName(mappingName,"Stretched Unit Square");
//        for( int axis=0; axis<domainDimension; axis++ )
//        { // label stretchedSquare axes
//          stretchedSquare->setName(mappingItemName(domainAxis1Name+axis),map2.getName(mappingItemName(domainAxis1Name+axis)));
//          stretchedSquare->setName(mappingItemName(rangeAxis1Name +axis),map2.getName(mappingItemName(domainAxis1Name+axis)));
//        }
//        stretchedSquare->update(mapInfo);
//        mappingHasChanged(); 
//      }
//      else if( answer=="stretch new" ) 
//      {
//        stretchedSquare->setName(mappingName,"Stretched Unit Square");
//        for( int axis=0; axis<domainDimension; axis++ )
//        { // label stretchedSquare axes
//          stretchedSquare->setName(mappingItemName(domainAxis1Name+axis),
//                                   map2.getName(mappingItemName(domainAxis1Name+axis)));
//          stretchedSquare->setName(mappingItemName(rangeAxis1Name +axis),
//                                   map2.getName(mappingItemName(domainAxis1Name+axis)));
//        }

//        update( mapInfo,nullString,NULL );

//        mappingHasChanged(); 
//      }
//      else if( answer=="edit unstretched mapping" )
//      {
//        assert( stretchedSquare!=NULL );
//        if( mappingChosen )
//        {
//  	map2.update(mapInfo);
//        }
//        else
//        {
//          printf("Sorry: cannot edit mapping before one is chosen\n");
//        }
//      }
//      else if( answer=="show parameters" )
//      {
//        display(" ****** Parameters for the stretched mapping ******* ");
//        stretchedSquare->display(" ***** Here are the parameters for the unit square stretching ***** ");
//      }
//      else if( answer=="plot" )
//      {
//        if( !mappingChosen )
//        {
//  	gi.outputString("you must first choose a mapping to transform");
//  	continue;
//        }
//        parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
//        gi.erase();
//        PlotIt::plot(gi,*this,parameters); 
//        parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
//      }
//      else if( answer=="help" )
//      {
//        for( int i=0; help[i]!=""; i++ )
//          gi.outputString(help[i]);
//      }
//      else if( answer=="lines"  ||
//               answer=="boundary conditions"  ||
//               answer=="share"  ||
//               answer=="mappingName"  ||
//               answer=="periodicity" )
//      { // call the base class to change these parameters:
//        mapInfo.commandOption=MappingInformation::readOneCommand;
//        mapInfo.command=&answer;
//        Mapping::update(mapInfo); 
//        mapInfo.commandOption=MappingInformation::interactive;
//      }
//      else if( answer=="exit" )
//        break;
//      else if( answer=="plotObject" )
//        plotObject=TRUE;
//      else 
//      {
//        gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
//        printf("Unknown response=%s",(const char*)answer);
//        gi.stopReadingCommandFile();
//      }

//      if( plotObject && mappingChosen )
//      {
//        parameters.set(GI_TOP_LABEL,getName(mappingName));
//        gi.erase();
//        PlotIt::plot(gi,*this,parameters);   // *** recompute every time ?? ***

//      }
//    }
//    gi.erase();
//    gi.unAppendTheDefaultPrompt();  // reset
//    return 0;
  
}
