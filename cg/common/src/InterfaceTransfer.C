// This file automatically generated from InterfaceTransfer.bC with bpp.
// -------------------------------------------------------------------------------------
// Class InterfaceTransfer : used to transfer information across an interface between
// two composite grids.
// -------------------------------------------------------------------------------------


#include "InterfaceTransfer.h"
#include "DomainSolver.h"
#include "Interface.h"
#include "ParallelUtility.h"
#include "InterpolatePointsOnAGrid.h"

// Default interpolation widths to use when an InterfaceTransfer object is constructed: 
int InterfaceTransfer::defaultInterpolationWidth[2]={2,2};  


// void
// getLocalBoundsAndBoundaryConditions( const realMappedGridFunction & a, 
//                                      IntegerArray & gidLocal, 
//                                      IntegerArray & dimensionLocal, 
//                                      IntegerArray & bcLocal );


#define FOR_3D(i1,i2,i3,I1,I2,I3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)


// -------------------------------------------------------------------------------------------
/// \brief: Constructor for the class used to transfer information across an interface between
// two composite grids.
// -------------------------------------------------------------------------------------------
InterfaceTransfer::
InterfaceTransfer()
{
    initialized=false;
    interpolatePointsOnAGrid=NULL;
    indirectionArray=NULL;                     // holds indicies (i1,i2,i3,grid) of points on a interface
    interpolationWidth[0]=defaultInterpolationWidth[0];   // interpolation width (one for each transfer direction)
    interpolationWidth[1]=defaultInterpolationWidth[1];
}

// -------------------------------------------------------------------------------------------
/// \brief: Destructor for the class used to transfer information across an interface between
// two composite grids.
// -------------------------------------------------------------------------------------------
InterfaceTransfer::
~InterfaceTransfer()
{
    delete [] interpolatePointsOnAGrid;
    delete indirectionArray;
}

// -------------------------------------------------------------------------------------------
/// \brief: Initialize the interface transfer.
//
/// \param interfaceDescriptor (input): defines the interface.
/// \param domainSolver (input) : holds PDE solvers for each domain
/// \param parameters (input) : parameters from Cgmp
///
/// \details This function will query the interface and build the transfer function that
/// can transfer (interpolate) the interface values from one side of the interface to the other.
/// Currently this is implemented by interpolating points from one side to the other. 
/// In the future we may want options to transfer data so that certain quantities are
/// conserved. 
// -------------------------------------------------------------------------------------------
int InterfaceTransfer::
initialize( InterfaceDescriptor & interfaceDescriptor,
                        std::vector<DomainSolver*> domainSolver,
                        std::vector<int> & gfIndex,
                        Parameters & parameters )
{

    initialized=true;

    int debug = 3;

    if( debug & 2 )
        printF("\n ************** InterfaceTransfer::initialize domain1=%i domain2=%i *************************\n",
                      interfaceDescriptor.domain1,interfaceDescriptor.domain2 );

    const int np= max(1,Communication_Manager::numberOfProcessors());
    const int & myid = parameters.dbase.get<int>("myid");
    
    const int numberOfDomains=domainSolver.size();
    int numberOfInterfaceHistoryValuesToSave = parameters.dbase.get<int>("numberOfInterfaceHistoryValuesToSave");
    int numberOfInterfaceIterateValuesToSave = parameters.dbase.get<int>("numberOfInterfaceIterateValuesToSave");
    FILE *& interfaceFile =parameters.dbase.get<FILE* >("interfaceFile");
    
    Index Iv[4], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2], &I4=Iv[3];
    Index Jv[4], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2], &J4=Jv[3];
    
    Index Iav[4], &Ia1=Iav[0], &Ia2=Iav[1], &Ia3=Iav[2], &Ia4=Iav[3];
    Index Jav[4], &Ja1=Jav[0], &Ja2=Jav[1], &Ja3=Jav[2], &Ja4=Jav[3];
    
    int domain[2];
    domain[0]=interfaceDescriptor.domain1;
    domain[1]=interfaceDescriptor.domain2;
    assert( domain[0]>=0 && domain[0]<numberOfDomains && domain[1]>=0 && domain[1]<numberOfDomains );

    int iv[4], &i1=iv[0], &i2=iv[1], &i3=iv[2];

    RealArray xid[2];    // xi[d](k,0:2) holds the x-coords of point k on the interface of domain[d]
  // indirectionArray[d](k,0:3) holds the index and grid coords (i1,i2,i3,grid) for point k
    if( indirectionArray==NULL )
        indirectionArray = new IntegerArray [2];

    const int orderOfAccuracyInSpace=2;  // **** do this for now ***

  // There may be multiple grid faces that lie on the interface:     
    for( int interfaceSide=0; interfaceSide<2; interfaceSide++ )
    {
    // const int d = domain[interfaceSide];       // interpolate points on this domain...

        const int domainTarget = domain[interfaceSide]; 
        const int domainSource = domain[(interfaceSide+1)%2]; //   ... from this domain

        GridFunction & gf = domainSolver[domainTarget]->gf[gfIndex[domainTarget]];
        CompositeGrid & cg = gf.cg;
        const int numberOfDimensions = cg.numberOfDimensions();

        RealArray & xi    = xid[interfaceSide];
        IntegerArray & ia = indirectionArray[interfaceSide];

    // --- there be multiple faces on this side of the interface ---
        GridList & gridList = interfaceSide==0 ? interfaceDescriptor.gridListSide1 : interfaceDescriptor.gridListSide2;
    // --- PASS 1: count the number of points on all faces of the interface
        int numberOfInterfacePoints=0;
        for( int face=0; face<gridList.size(); face++ )
        {
            GridFaceDescriptor & gridDescriptor = gridList[face];
            const int dd=gridDescriptor.domain, grid=gridDescriptor.grid, side=gridDescriptor.side, dir=gridDescriptor.axis;
            assert( dd==domainTarget );

            assert( grid>=0 && grid<cg.numberOfComponentGrids());
            MappedGrid & mg = cg[grid];
            const intArray & mask = mg.mask();

            OV_GET_SERIAL_ARRAY_CONST(int,mask,maskLocal);

            const int extra=0; // orderOfAccuracyInSpace/2;
            getBoundaryIndex(mg.gridIndexRange(),side,dir,I1,I2,I3,extra);
    
            int includeGhost=0;  // is this right ? 
            bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);
            if( ok )
            {
        // *NOTE* we could check the mask !! *FIX ME*

      	numberOfInterfacePoints += I1.length()*I2.length()*I3.length();
            }
        
        } // end for face 
    
    // --- PASS 2: fill in the interface data (positions of points)
        if( numberOfInterfacePoints>0 )
        {
            xi.redim(numberOfInterfacePoints,numberOfDimensions);
            ia.redim(numberOfInterfacePoints,4);
            
            int nip=0; // counts interface points as we fill them in 
            for( int face=0; face<gridList.size(); face++ )
            {
      	GridFaceDescriptor & gridDescriptor = gridList[face];
            
      	const int dd=gridDescriptor.domain, grid=gridDescriptor.grid, side=gridDescriptor.side, dir=gridDescriptor.axis;
      	assert( dd==domainTarget );

      	assert( grid>=0 && grid<cg.numberOfComponentGrids());
      	MappedGrid & mg = cg[grid];
      	const intArray & mask = mg.mask();
                OV_GET_SERIAL_ARRAY_CONST(int,mask,maskLocal);

      	const int extra=0; // orderOfAccuracyInSpace/2;
      	getBoundaryIndex(mg.gridIndexRange(),side,dir,I1,I2,I3,extra);
    
      	int includeGhost=0;  // is this right ? 
      	bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);

	// -- *FIX ME* optimize for Cartesian
      	mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );
                OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),xLocal);

      	if( ok )
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3)
        	  {
          	    for( int axis=0; axis<numberOfDimensions; axis++ )
          	    {
            	      xi(nip,axis)=xLocal(i1,i2,i3,axis);
          	    }
          	    for( int axis=0; axis<3; axis++ ) // for now save 3 values even for 2D
          	    {
            	      ia(nip,axis)=iv[axis];
          	    }
          	    ia(nip,3)=grid; 
          	    nip++;
        	  }
      	}
        
            } // end for face 
            assert( nip==numberOfInterfacePoints ); // sanity check 
            
        } // end if numberOfInterfacePoints>0
        

    // Option I:
    //   Use InterpolatePointsOnAGrid: 


        if( interpolatePointsOnAGrid==NULL )
            interpolatePointsOnAGrid = new InterpolatePointsOnAGrid [2];
        
        InterpolatePointsOnAGrid & interp = interpolatePointsOnAGrid[interfaceSide];

        interp.setInterpolationWidth( interpolationWidth[interfaceSide] );
        
    // *wdh* 100603 -- use explicit interpolation by default 
        interp.setInterpolationType( InterpolatePointsOnAGrid::explicitInterpolation );

        if( debug & 1 )
            printF("--- InterfaceTransfer::initialize: interfaceSide=%i numberOfInterfacePoints=%i ---\n",
           	     interfaceSide,numberOfInterfacePoints);
        if( debug & 8 )
        {
            for( int i=0; i<numberOfInterfacePoints; i++ )
            {
      	printF(" i=%i xi=(%8.2e,%8.2e) ia=(%i,%i,%i,%i) \n",i,xi(i,0),xi(i,1),ia(i,0),ia(i,1),ia(i,2),ia(i,3));
            }
        }
        
    // ::display(x2i,"x2i");
        
    // determine how to interpolate points on the interface of mg2 from cg1 : 
        CompositeGrid & cgSource = domainSolver[domainSource]->gf[gfIndex[domainSource]].cg;
        
        interp.buildInterpolationInfo( xi,cgSource );

        if( debug & 8 )
        {
            IntegerArray indexValues,interpoleeGrid;

            interp.getInterpolationInfo(cgSource, indexValues, interpoleeGrid);

            printF(" --- InterfaceTransfer::initialize: interpolate points on domain=%i from domain=%i ---\n",
           	     domainTarget,domainSource);
            for( int i=indexValues.getBase(0); i<=indexValues.getBound(0); i++ )
            {
      	printF(" i=%i xi=(%8.2e,%8.2e) il=(%i,%i) donor=%i\n",i,xi(i,0),xi(i,1),indexValues(i,0),indexValues(i,1),interpoleeGrid(i));
            }
        }
        
    // Here we access the internal data from InterpolatePointsOnAGrid: 

    //IntegerArray & numberOfInterpolationPoints = interp.numberOfInterpolationPoints;
    //IntegerArray *& indirection = interp.indirection;
    //IntegerArray *& interpolationLocation = interp.interpolationLocation;
    //RealArray *& interpolationCoordinates = interp.interpolationCoordinates;
        

        if( debug & 2 )
        {
            printF("InterfaceTransfer::initialize: target domain=%i source domain=%i interfaceSide=%i\n",
                          domainTarget,domainSource,interfaceSide);
            printF(" interp.maxInterpolationWidth=%i\n",interp.maxInterpolationWidth);
        }
        
    // --- ensure that the donor locations lie on the interface ---  **FINISH ME**

    // -- first define:
    //         gridIsOnInterface(grid) = true   if the source grid is on the interface
    //                                 = false  otherwise
        int *pGridIsOnInterface = new int [cgSource.numberOfComponentGrids()];
        #define gridIsOnInterface(grid) pGridIsOnInterface[grid]
        for( int grid=0; grid<cgSource.numberOfComponentGrids(); grid++ )
            gridIsOnInterface(grid)=false;
        GridList & gridListSource = interfaceSide==0 ? interfaceDescriptor.gridListSide2 : interfaceDescriptor.gridListSide1;
        for( int face=0; face<gridListSource.size(); face++ )
        {
            GridFaceDescriptor & gridDescriptor = gridListSource[face];
            const int dd=gridDescriptor.domain, grid=gridDescriptor.grid, side=gridDescriptor.side, dir=gridDescriptor.axis;
            assert( dd==domainSource );
            gridIsOnInterface(grid)=true;
        }
        

        int isOnFace[3];
        const real eps = 1.e-2;  // tolerance for checking whether we on the face of a grid 
        for( int grid=0; grid<cgSource.numberOfComponentGrids(); grid++ )
        {
            int nil=interp.numberOfInterpolationPoints(grid);
            if( nil<=0 ) continue;
            
            if( !gridIsOnInterface(grid) )
            {
      	printF("InterfaceTransfer::initialize:ERROR: The donor grid=%i is not on the interface! *FIX ME Bill*\n");
      	OV_ABORT("error");
            }

            if( debug & 2 )
      	printF(" donor grid =%i, numberOfInterpolationPoints=%i \n",grid,interp.numberOfInterpolationPoints(grid));
            const IntegerArray & ip = interp.interpolationLocation[grid];       
            const IntegerArray & ia = interp.indirection[grid];       
            const RealArray & ci = interp.interpolationCoordinates[grid];
            for( int i=0; i<nil; i++ )
            {
      	if( debug & 8 )
        	  printF(" i=%i ia=%i il=(%i,%i) ci=(%5.3f,%5.3f)\n",i,ia(i),ip(i,0),ip(i,1),ci(i,0),ci(i,1));

        // numberOfFaces : number of faces the interpolation point lies on
      	int numberOfFaces=0;  
      	for( int axis=0; axis<numberOfDimensions; axis++ )
      	{
                    isOnFace[axis]=0;
        	  if( fabs(ci(i,axis))<eps )
        	  {
                        isOnFace[axis]=1; numberOfFaces++;
        	  }
                    else if( fabs(ci(i,axis)-1.)<eps )
        	  {
                        isOnFace[axis]=2; numberOfFaces++;
        	  }
        	  else
          	    isOnFace[axis]=0;
      	}
        // we could project onto the interface
      	if( numberOfFaces==1 )
      	{
      	}
      	else if( numberOfFaces>1 )
      	{

      	}
      	else if( numberOfFaces==0 )
      	{
                    int iai = ia(i);
        	  printF("InterfaceTransfer::initialize:ERROR: A donor location for interpolation is not on the interface!\n"
                                  "  domainTarget=%i domainSource=%i x=(%5.3f,%5.3f,%5.3f) donor=%i, il=(%i,%i,%i) "
                                  "ci=(%5.3f,%5.3f,%5.3f)\n"
                                  " The tolerance was eps=%8.2e\n",
                                  domainTarget,domainSource,
                                  xi(iai,0),xi(iai,1),(numberOfDimensions==2 ? 0 : xi(iai,2)),
             		 grid,ip(i,0),ip(i,1),(numberOfDimensions==2 ? 0 : ip(i,2)),
                                  ci(i,0),ci(i,1),(numberOfDimensions==2 ? 0. : ci(i,2)),eps);
        	  OV_ABORT("error");
      	}
      	
            } // end for i
            
        } // end for grid

        delete []  pGridIsOnInterface;

    }  // end for interfaceSide

    return 0;
}

// ------------------------------------------------------------------------------------------------------------------
/// \brief: Set the default interface transfer interpolation widths. These values will apply when
/// new InterfaceTransfer objects are built. This is a static function that can be called before any
/// class objects are constructed.
///
/// \param width (input) : set the interpolation width (positive integer)
/// \param interfaceSide (input) : if interfaceSide=-1 then set both transfer directions. If interfaceSide=dir
///    set the width for transfer direction "dir", dir=0 or dir=1.
// ------------------------------------------------------------------------------------------------------------------
int InterfaceTransfer::
setDefaultInterpolationWidth( int width, int interfaceSide /* =-1 */ )
{
    if( width<=0 || width>20 )
    {
        printF("InterfaceTransfer::setDefaultInterpolationWidth:ERROR: invalid interpolation width requested width=%i.\n"
                      "  The current interpolation widths = %i, %i will not be changed\n",
                      width,defaultInterpolationWidth[0],defaultInterpolationWidth[1]);
    }
    
    if( interfaceSide==0 || interfaceSide==1 )
    {
        defaultInterpolationWidth[interfaceSide]=width;
    }
    else
    {
        defaultInterpolationWidth[0]=width;
        defaultInterpolationWidth[1]=width;
    }
    
    return 0;
}

// ------------------------------------------------------------------------------------------------------------------
/// \brief: Set the interface transfer interpolation widths
///
/// \param width (input) : set the interpolation width (positive integer)
/// \param interfaceSide (input) : if interfaceSide=-1 then set both transfer directions. If interfaceSide=dir
///    set the width for transfer direction "dir", dir=0 or dir=1.
// ------------------------------------------------------------------------------------------------------------------
int InterfaceTransfer::
setInterpolationWidth( int width, int interfaceSide /* =-1 */ )
{
    if( width<=0 || width>20 )
    {
        printF("InterfaceTransfer::setInterpolationWidth:ERROR: invalid interpolation width requested width=%i.\n"
                      "  The current interpolation widths = %i, %i will not be changed\n",
                      width,interpolationWidth[0],interpolationWidth[1]);
    }
    
    if( interfaceSide==0 || interfaceSide==1 )
    {
        interpolationWidth[interfaceSide]=width;
    }
    else
    {
        interpolationWidth[0]=width;
        interpolationWidth[1]=width;
    }
    
    return 0;
}


// ------------------------------------------------------------------------------------------------------------------------
/// \brief: Transfer the interface data from one domain to another.
//
/// \param domainSource, domainTarget (input) :  domain identifiers for the source domain and target domain.
/// \param targetDataArray[grid] (input) : pointer to an array of data on the interface from domainTarget (if non-null)
/// \param Ct (input) : target components to fill in.
/// \param sourceDataArray[grid] : pointer to an array of data on the interface from domainSource (if non-null)
/// \param Cs (input) : source components to fill in.
/// \param interfaceDescriptor (input): defines the interface.
/// \param domainSolver (input) : holds PDE solvers for each domain
/// \param parameters (input) : parameters from Cgmp
///
/// \details This function will query the interface and build the transfer function that
/// can transfer (interpolate) the inteface values from one side of the interface to the other.
///
// -----------------------------------------------------------------------------------------------------------------------
int InterfaceTransfer::
transferData( int domainSource, int domainTarget, 
                            RealArray **sourceDataArray, Range & Cs,
                            RealArray **targetDataArray, Range & Ct,
            	      InterfaceDescriptor & interfaceDescriptor,
            	      std::vector<DomainSolver*> domainSolver,
            	      std::vector<int> & gfIndex,
            	      Parameters & parameters )
{

    int debug = 3;

    if( !initialized )
    {
        initialize( interfaceDescriptor,domainSolver,gfIndex,parameters );
        assert( initialized );
    }
    
    assert( interpolatePointsOnAGrid!=NULL );
    const int interfaceSide = domainTarget==interfaceDescriptor.domain1 ? 0 : 1;
    InterpolatePointsOnAGrid & interp = interpolatePointsOnAGrid[interfaceSide];


    CompositeGrid & cgSource = domainSolver[domainSource]->gf[gfIndex[domainSource]].cg;

  // We need a new interp routine: (see pogip.bC)
  // NOTE: source arrays are multi-dimensional arrays so we need to know which points to fill in !

    const int numberOfDomains=domainSolver.size();
    int domain[2];
    domain[0]=interfaceDescriptor.domain1;
    domain[1]=interfaceDescriptor.domain2;
    assert( domain[0]>=0 && domain[0]<numberOfDomains && domain[1]>=0 && domain[1]<numberOfDomains );

    if( debug & 8 )
    {
        printF("InterfaceTransfer::transferData: domainSource=%i domainTarget=%i interfaceSide=%i\n",
         	   domainSource,domainTarget,interfaceSide);
        printF(" interp.maxInterpolationWidth=%i\n",interp.maxInterpolationWidth);
        for( int grid=0; grid<cgSource.numberOfComponentGrids(); grid++ )
        {
            int nil=interp.numberOfInterpolationPoints(grid);
            printF(" donor grid =%i, numberOfInterpolationPoints=%i \n",grid,interp.numberOfInterpolationPoints(grid));
            const IntegerArray & ip = interp.interpolationLocation[grid];       
            const RealArray & ci = interp.interpolationCoordinates[grid];
            for( int i=0; i<nil; i++ )
            {
                printF(" i=%i il=(%i,%i) ci=(%5.3f,%5.3f)\n",i,ip(i,0),ip(i,1),ci(i,0),ci(i,1));
            }
            
            if( sourceDataArray[grid]!=NULL )
            {
      	::display(*sourceDataArray[grid],sPrintF("Source data values on domain=%i grid=%i\n",
                                     						 domainSource,grid));
            }
        }
    }



    internalInterpolate( sourceDataArray, Cs, targetDataArray, Ct, 
                                              cgSource, interfaceSide );

    if( debug & 8 )
    {
          CompositeGrid & cg = domainSolver[domainTarget]->gf[gfIndex[domainTarget]].cg;
          for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
          {
              if( targetDataArray[grid]!=NULL )
              {
                  ::display(*targetDataArray[grid],sPrintF("Interpolated interface values on domain=%i grid=%i",
                                      						  domainTarget,grid));
              }
          }
    }
    
    return 0;
}


//  **FINISH ME**

// =====================================================================================
/// \brief Make changes to the interface transfer parameters.
// =====================================================================================
// int InterfaceTransfer::
// update( InterfaceDescriptor & interfaceDescriptor,
//         std::vector<DomainSolver*> domainSolver,
//         Parameters & parameters,
//         const aString & command /* = nullString */,
// 	DialogData *interface /* =NULL */ )
// {
//   int returnValue=0;

//   assert(  parameters.dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
//   GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");

//   aString prefix = "INTR:"; // prefix for commands to make them unique.

//   // ** Here we only look for commands that have the proper prefix ****
//   const bool executeCommand = command!=nullString;
//   if( executeCommand && command(0,prefix.length()-1)!=prefix && command!="build dialog" )
//     return 1;

//   aString answer;
//   char buff[100];
    

//   GUIState gui;
//   gui.setExitCommand("done", "continue");
//   DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;

//   if( interface==NULL || command=="build dialog" )
//   {
//     const int maxCommands=40;
//     aString cmd[maxCommands];
//     dialog.setWindowTitle("Interface Transfer Parameters");

//     aString pbLabels[] = {"show parameters",
// 			    ""};
//     addPrefix(pbLabels,prefix,cmd,maxCommands);

//     int numRows=2;
//     dialog.setPushButtons( cmd, pbLabels, numRows ); 


// //     aString tbLabels[] = {"solve coupled interface equations",
// //                           "use mixed interface conditions",
// //                           "extrapolate initial interface values",
// //                           "use new interface transfer",
// // 			    ""};
// //     int tbState[10];
// //     tbState[0] = dbase.get<bool>("solveCoupledInterfaceEquations");
// //     tbState[1] = dbase.get<bool>("useMixedInterfaceConditions");
// //     tbState[2] = dbase.get<bool>("extrapolateInitialInterfaceValues");
// //     tbState[3] = dbase.get<bool>("useNewInterfaceTransfer");
        
// //     int numColumns=1;
// //     addPrefix(tbLabels,prefix,cmd,maxCommands);
// //     dialog.setToggleButtons(cmd, tbLabels, tbState, numColumns); 


//     const int numberOfTextStrings=10;
//     aString textLabels[numberOfTextStrings];
//     aString textStrings[numberOfTextStrings];

//     int nt=0;
//     textLabels[nt] = "active interface";  sPrintF(textStrings[nt], "%i (-1=all, for params)", 
//                       dbase.get<int >("activeInterface"));  nt++; 
  
//     textLabels[nt] = "interface tolerance";  sPrintF(textStrings[nt], "%g", 
//                       dbase.get<real >("interfaceTolerance"));  nt++; 
  
//     textLabels[nt] = "interface omega";  sPrintF(textStrings[nt], "%g", 
//                       dbase.get<real >("interfaceOmega"));  nt++; 
  
//     textLabels[nt] = "domain order"; textStrings[nt]="";
//     std::vector<int> & domainOrder = dbase.get<std::vector<int> >("domainOrder");
//     for( int d=0; d<domainOrder.size(); d++ )
//     {
//       textStrings[nt] += sPrintF("%i ",domainOrder[d]);
//     }
//     nt++;
        
//     // textLabels[nt] = "dtMax";  sPrintF(textStrings[nt], "%g",dbase.get<real >("dtMax"));  nt++; 
  
//     // null strings terminal list
//     textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
//     addPrefix(textLabels,prefix,cmd,maxCommands);
//     dialog.setTextBoxes(cmd, textLabels, textStrings);

//     if( false && gi.graphicsIsOn() )
//       dialog.openDialog(0);   // open the dialog here so we can reset the parameter values below

//     updatePDEparameters();

//     if( executeCommand ) return 0;
//   }
    
//   if( !executeCommand  )
//   {
//     gi.pushGUI(gui);
//     gi.appendToTheDefaultPrompt("pde parameters>");  
//   }

//   InterfaceList & interfaceList = dbase.get<InterfaceList>("interfaceList");
//   int & activeInterface = dbase.get<int>("activeInterface");
    
//   int len;
//   for(int it=0; ; it++)
//   {
//     if( !executeCommand )
//     {
//       gi.getAnswer(answer,"");
//     }
//     else
//     {
//       if( it==0 ) 
//         answer=command;
//       else
//         break;
//     }
    
//     if( answer(0,prefix.length()-1)==prefix )
//       answer=answer(prefix.length(),answer.length()-1);   // strip off the prefix

//     printF("setPdeParameters: answer=[%s]\n",(const char*)answer);
        

//     if( answer=="done" )
//       break;
//     else if( dialog.getTextValue(answer,"active interface","%i", activeInterface) )
//     {
//       printF("Setting the active interface to %i (for setting parameters, -1=all)\n",activeInterface);
//     }
//     else if( dialog.getTextValue(answer,"interface tolerance","%e", dbase.get<real>("interfaceTolerance")) )
//     {
//       if( activeInterface==-1 )
// 	printF("Setting the interfaceTolerance=%8.2e for all %i interfaces\n",dbase.get<real>("interfaceTolerance"),
// 	       interfaceList.size());
//       else
//         printF("Setting the interfaceTolerance=%8.2e for interface %i.\n",dbase.get<real>("interfaceTolerance"),
//                 activeInterface);
//       for( int inter=0; inter < interfaceList.size(); inter++ )
//       {
//         if( activeInterface==-1 || activeInterface==inter )
// 	{
// 	  InterfaceDescriptor & interfaceDescriptor = interfaceList[inter]; 
// 	  interfaceDescriptor.interfaceTolerance=dbase.get<real>("interfaceTolerance");
// 	}
//       }
//     }
//     else if( dialog.getTextValue(answer,"interface omega","%e", dbase.get<real>("interfaceOmega")) )
//     {
//       if( activeInterface==-1 )
// 	printF("Setting the interfaceOmega=%8.2e for all %i interfaces\n",dbase.get<real>("interfaceOmega"),
// 	       interfaceList.size());
//       else
// 	printF("Setting the interfaceOmega=%8.2e for interface %i.\n",dbase.get<real>("interfaceOmega"),
// 	       activeInterface);

//       for( int inter=0; inter < interfaceList.size(); inter++ )
//       {
//         if( activeInterface==-1 || activeInterface==inter )
// 	{
// 	  InterfaceDescriptor & interfaceDescriptor = interfaceList[inter]; 
// 	  interfaceDescriptor.interfaceOmega=dbase.get<real>("interfaceOmega");
// 	}
//       }
//     }
//     else if( dialog.getToggleValue(answer,"solve coupled interface equations",
//                                    dbase.get<bool>("solveCoupledInterfaceEquations")) ){} //
//     else if( dialog.getToggleValue(answer,"use mixed interface conditions",
//                                    dbase.get<bool>("useMixedInterfaceConditions")) ){} //
//     else if( dialog.getToggleValue(answer,"extrapolate initial interface values",
//                                    dbase.get<bool>("extrapolateInitialInterfaceValues")) ){} //
//     else if( dialog.getToggleValue(answer,"use new interface transfer",
//                                    dbase.get<bool>("useNewInterfaceTransfer")) ){} //
//     // else if( dialog.getTextValue(answer,"dtMax","%e",dbase.get<real >("dtMax")) ){} //
//     else if( len=answer.matches("domain order") )
//     {
//       std::vector<int> & domainOrder = dbase.get<std::vector<int> >("domainOrder");
//       bool ok=true;
//       for( int d=0; d<domainOrder.size(); d++ )
//       {
//         int domain=-1;
//         const int length = answer.length();
// 	while( len<length && answer[len]==' ' ) len++;  // skip blanks
// 	int numChars = sScanF(answer(len,answer.length()-1),"%i",&domain);
// 	len+=numChars;
// 	if( domain>=0 && domain<domainOrder.size() )
// 	{
//           domainOrder[d]=domain;
// 	}
// 	else
// 	{
//           printF("Error: invalid domain number=%i\n",domain);
// 	  ok=false;
//           gi.stopReadingCommandFile();
// 	  break;
// 	}
      	
//       }
//       if( !ok )
//       {
// 	printF("I am reseting the domain order to the default.\n");
// 	for( int d=0; d<domainOrder.size(); d++ ) domainOrder[d]=d;
//       }
//       else
//       {
//         // We should also check that all domains are accounted for
// 	printF(" New domainOrder = ");
// 	for( int d=0; d<domainOrder.size(); d++ )
// 	{
// 	  printF("%i ",domainOrder[d]);
// 	}
// 	printF("\n");
//       }
//     }
//     else if( answer == "show interface parameters" )
//     {
//       displayInterfaceInfo();
//     }
//     else if(  dbase.get<ListOfShowFileParameters >("pdeParameters").matchAndSetValue( answer ) )
//     {
//       printF("*** answer=[%s] was found as a user defined parameter\n",(const char*)answer);
            
//     }
//     else
//     {
//       if( executeCommand )
//       {
// 	returnValue= 1;  // when executing  dbase.get<real >("a") single command, return 1 if the command was not recognised.
//         break;
//       }
//       else
//       {
// 	printF("Unknown response=[%s]\n",(const char*)answer);
// 	gi.stopReadingCommandFile();
//       }
              
//     }

//   }

//   if( !executeCommand  )
//   {
//     gi.popGUI();
//     gi.unAppendTheDefaultPrompt();
//   }

//  return returnValue;

// }




// ******************** THIS NEXT ROUTINE IS COPIED FROM pogip.bc , WE SHOULD MERGE ****************************

#undef dr
#undef ir
#undef isCC
#undef coeffg


#define q10(x) 1
#define q20(x) -(x)+1
#define q21(x) (x)
#define q30(x) ((x)-1)*((x)-2)/2.
#define q31(x) -(x)*((x)-2)
#define q32(x) (x)*((x)-1)/2.
#define q40(x) -((x)-1)*((x)-2)*((x)-3)/6.
#define q41(x) (x)*((x)-2)*((x)-3)/2.
#define q42(x) -(x)*((x)-1)*((x)-3)/2.
#define q43(x) (x)*((x)-1)*((x)-2)/6.
#define q50(x) ((x)-1)*((x)-2)*((x)-3)*((x)-4)/24.
#define q51(x) -(x)*((x)-2)*((x)-3)*((x)-4)/6.
#define q52(x) (x)*((x)-1)*((x)-3)*((x)-4)/4.
#define q53(x) -(x)*((x)-1)*((x)-2)*((x)-4)/6.
#define q54(x) (x)*((x)-1)*((x)-2)*((x)-3)/24.
#define q60(x) -((x)-1)*((x)-2)*((x)-3)*((x)-4)*((x)-5)/120.
#define q61(x) (x)*((x)-2)*((x)-3)*((x)-4)*((x)-5)/24.
#define q62(x) -(x)*((x)-1)*((x)-3)*((x)-4)*((x)-5)/12.
#define q63(x) (x)*((x)-1)*((x)-2)*((x)-4)*((x)-5)/12.
#define q64(x) -(x)*((x)-1)*((x)-2)*((x)-3)*((x)-5)/24.
#define q65(x) (x)*((x)-1)*((x)-2)*((x)-3)*((x)-4)/120.
#define q70(x) ((x)-1)*((x)-2)*((x)-3)*((x)-4)*((x)-5)*((x)-6)/720.
#define q71(x) -(x)*((x)-2)*((x)-3)*((x)-4)*((x)-5)*((x)-6)/120.
#define q72(x) (x)*((x)-1)*((x)-3)*((x)-4)*((x)-5)*((x)-6)/48.
#define q73(x) -(x)*((x)-1)*((x)-2)*((x)-4)*((x)-5)*((x)-6)/36.
#define q74(x) (x)*((x)-1)*((x)-2)*((x)-3)*((x)-5)*((x)-6)/48.
#define q75(x) -(x)*((x)-1)*((x)-2)*((x)-3)*((x)-4)*((x)-6)/120.
#define q76(x) (x)*((x)-1)*((x)-2)*((x)-3)*((x)-4)*((x)-5)/720.
#define q80(x) -((x)-1)*((x)-2)*((x)-3)*((x)-4)*((x)-5)*((x)-6)*((x)-7)/5040.
#define q81(x) (x)*((x)-2)*((x)-3)*((x)-4)*((x)-5)*((x)-6)*((x)-7)/720.
#define q82(x) -(x)*((x)-1)*((x)-3)*((x)-4)*((x)-5)*((x)-6)*((x)-7)/240.
#define q83(x) (x)*((x)-1)*((x)-2)*((x)-4)*((x)-5)*((x)-6)*((x)-7)/144.
#define q84(x) -(x)*((x)-1)*((x)-2)*((x)-3)*((x)-5)*((x)-6)*((x)-7)/144.
#define q85(x) (x)*((x)-1)*((x)-2)*((x)-3)*((x)-4)*((x)-6)*((x)-7)/240.
#define q86(x) -(x)*((x)-1)*((x)-2)*((x)-3)*((x)-4)*((x)-5)*((x)-7)/720.
#define q87(x) (x)*((x)-1)*((x)-2)*((x)-3)*((x)-4)*((x)-5)*((x)-6)/5040.
#define q90(x) ((x)-1)*((x)-2)*((x)-3)*((x)-4)*((x)-5)*((x)-6)*((x)-7)*((x)-8)/40320.
#define q91(x) -(x)*((x)-2)*((x)-3)*((x)-4)*((x)-5)*((x)-6)*((x)-7)*((x)-8)/5040.
#define q92(x) (x)*((x)-1)*((x)-3)*((x)-4)*((x)-5)*((x)-6)*((x)-7)*((x)-8)/1440.
#define q93(x) -(x)*((x)-1)*((x)-2)*((x)-4)*((x)-5)*((x)-6)*((x)-7)*((x)-8)/720.
#define q94(x) (x)*((x)-1)*((x)-2)*((x)-3)*((x)-5)*((x)-6)*((x)-7)*((x)-8)/576.
#define q95(x) -(x)*((x)-1)*((x)-2)*((x)-3)*((x)-4)*((x)-6)*((x)-7)*((x)-8)/720.
#define q96(x) (x)*((x)-1)*((x)-2)*((x)-3)*((x)-4)*((x)-5)*((x)-7)*((x)-8)/1440.
#define q97(x) -(x)*((x)-1)*((x)-2)*((x)-3)*((x)-4)*((x)-5)*((x)-6)*((x)-8)/5040.
#define q98(x) (x)*((x)-1)*((x)-2)*((x)-3)*((x)-4)*((x)-5)*((x)-6)*((x)-7)/40320.


// --------------------- 2D interp macros -----------------------------
// These formulae are from higherOrderInterp.h











// -------------- 3D interp macros -----------------










// =======================================================================
// Double check that the interpolation is valid by checking that 
// the mask is non-zero at all donor points
// =======================================================================


// **************** SERIAL VERSION **********************

#undef VIW
#undef IL1
#undef IL2
#undef IL3

int InterfaceTransfer::
internalInterpolate( RealArray **sourceDataArray, Range & Cs,
                                          RealArray **targetDataArray, Range & Ct,
                                          CompositeGrid & cg, int interfaceSide )
{
      
  //  if( u.getCompositeGrid()->numberOfBaseGrids()<=0 )
  //   return 0;

    #ifdef USE_PPP
        OV_ABORT("InterfaceTransfer::internalInterpolate:ERROR: This function should not be called in parallel.");
    #endif

    int debug=3; // *****************

    if( debug & 2 )
        printF("**** InterfaceTransfer::internalInterpolate debug=%i ****\n",debug);
    

    

    double time0=getCPU();
    const int myid = max(0,Communication_Manager::My_Process_Number);


    assert( indirectionArray!=NULL );
    assert( interfaceSide==0 || interfaceSide==1 );
    
    IntegerArray & ia = indirectionArray[interfaceSide];

  // fill in values ia(k,0:3) : targetDataArray[grid](i1,i2,i3) = Interp 

    InterpolatePointsOnAGrid & interp = interpolatePointsOnAGrid[interfaceSide];
    
    
    const int c1Base=Cs.getBase(), c1Bound=Cs.getBound();
    const int c2Base=Ct.getBase(), c2Bound=Ct.getBound();
    const int numberOfComponents=c1Bound-c1Base+1;
    assert( numberOfComponents==(c2Bound-c2Base+1) );

    const int numberOfComponentGrids = cg.numberOfComponentGrids();
    const int numberOfDimensions = cg.numberOfDimensions();

    bool checkForValidInterpolation=debug>0;  // ****
    int numberOfInvalidPoints=0;

    const int & maxInterpolationWidth = interp.maxInterpolationWidth;

    if( debug & 4 )
        printF("    internalInterp: interfaceSide=%i, Cs=[%i,%i] Ct=[%i,%i] maxInterpWidth=%i ****\n",
         	   interfaceSide,c1Base,c1Bound,c2Base,c2Bound,maxInterpolationWidth);

    int grid;
    int i,j,axis;

    int width[3]={1,1,1};
    for( int axis=0; axis<numberOfDimensions; axis++ )
        width[axis]=maxInterpolationWidth;
    int m1,m2,m3;
    int k=0;  // note used in serial but appears in the loops
    int nid=0;
    const int p=0;
    
  // RealArray ui;  // do this for now 

    const InterpolatePointsOnAGrid::ExplicitInterpolationStorageOptionEnum & explicitInterpolationStorageOption = 
                                                                                        interp.explicitInterpolationStorageOption;

    const InterpolatePointsOnAGrid::ExplicitInterpolationStorageOptionEnum & precomputeAllCoefficients =
        InterpolatePointsOnAGrid::precomputeAllCoefficients;
    const InterpolatePointsOnAGrid::ExplicitInterpolationStorageOptionEnum & precomputeNoCoefficients =
        InterpolatePointsOnAGrid::precomputeNoCoefficients;

  // We must check the highest priority grid first since this was the order the points were generated.
  // A point may be extrapolated on a higher priority grid but then interpolated on a lower priority grid.
    for( int grid=numberOfComponentGrids-1; grid>=0; grid-- )  // check highest priority grid first
    {

    // interpolate the points from donor=grid 
        const int nil=interp.numberOfInterpolationPoints(grid);
        if( nil==0 ) continue; // no points to interpolate from this donor
        
        if( debug & 2  )
            printF(" -- interpolate %i pts from donor grid=%i\n",nil,grid);
        
        assert( sourceDataArray[grid]!=NULL );
        
        const realSerialArray & us = *sourceDataArray[grid];

    // For now we build a source array for the whole grid and copy the
    // interface values into it -- could do better here
        Index I1,I2,I3;
        getIndex(cg[grid].dimension(),I1,I2,I3);
        realSerialArray vs(I1,I2,I3,Cs);
        vs=0.;
        I1=us.dimension(0); I2=us.dimension(1);  I3=us.dimension(2); 
        vs(I1,I2,I3,Cs)=us(I1,I2,I3,Cs);

    // ::display(vs," source array on whole grid","%5.1f ");

        nid+=nil;
      	
        real cr0,cr1,cr2,cr3,cr4,cr5,cr6,cr7,cr8;
        real cs0,cs1,cs2,cs3,cs4,cs5,cs6,cs7,cs8;
        real ct0,ct1,ct2,ct3,ct4,ct5,ct6,ct7,ct8;
        int i1,i2,i3,iw;
        int ja,j1,j2,j3,gj;

        IntegerArray & viw = interp.variableInterpolationWidth[grid];

        const IntegerArray & ip = interp.interpolationLocation[grid];
        const int *ipp = ip.Array_Descriptor.Array_View_Pointer1;
        const int ipDim0=ip.getRawDataSize(0);
        #define IL(i0,i1) ipp[i0+ipDim0*(i1)]

    // define macros for the interp width and components of IL
        #undef VIW
        #define VIW(i) viw(i)
        #define IL1(i) IL(i,0)
        #define IL2(i) IL(i,1)
        #define IL3(i) IL(i,2)

        const RealArray & ci = interp.interpolationCoordinates[grid];
        real *cip = ci.Array_Descriptor.Array_View_Pointer1;
        const int ciDim0=ci.getRawDataSize(0);
        #define CI(i0,i1) cip[i0+ciDim0*(i1)]


//     real *uip = ui.Array_Descriptor.Array_View_Pointer1;
//     const int uiDim0=ui.getRawDataSize(0);
//     #define UI(i0,i1) uip[i0+uiDim0*(i1)]

        const IntegerArray & ia = interp.indirection[grid];
        const int *iap = ia.Array_Descriptor.Array_View_Pointer0;
        #define IA(i0) iap[(i0)]

        real *coeffap = interp.coeffa(p,grid);
        #undef cf
        #undef cfs
        #define cf(i,m1,m2)  coeffap[i+nil*((m1)+width[0]*((m2)))]
        #define cfs(i,m1)  coeffap[i+nil*(m1)]

        const IntegerArray & id = indirectionArray[interfaceSide];
        const int *idp = id.Array_Descriptor.Array_View_Pointer1;
        const int idDim0=id.getRawDataSize(0);
        #define ID(i0,i1) idp[i0+idDim0*(i1)]
        
        if( checkForValidInterpolation )
        {
      // *** double check that the interpolation is valid ***
              MappedGrid & mg= cg[grid];
              #ifdef USE_PPP
                  intSerialArray mask; getLocalArrayWithGhostBoundaries(mg.mask(),mask);
              #else
                  const intSerialArray & mask = mg.mask();
              #endif
              const int *maskp = mask.Array_Descriptor.Array_View_Pointer2;
              const int maskDim0=mask.getRawDataSize(0);
              const int maskDim1=mask.getRawDataSize(1);
              #define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]
              int i3a=0, i3b=0;
              for( int j=0; j<nil; j++ )
              {
                  int iw=VIW(j);
                  int i1a=IL1(j),   i2a=IL2(j);
                  int i1b=i1a+iw-1, i2b=i2a+iw-1;
                  if( numberOfDimensions==3 )
                  {
                      i3a=IL3(j); i3b=i3a+iw-1;
                  }
                  bool ok=true;
                  for( int i3=i3a; i3<=i3b; i3++ )
                  for( int i2=i2a; i2<=i2b; i2++ )
                  for( int i1=i1a; i1<=i1b; i1++ )
                  {
                      if( MASK(i1,i2,i3)==0 )
                      {
                  	ok=false;
                  	break;
                      }
                  }
                  if( !ok )
                  {
                      numberOfInvalidPoints++;
                      printf("InterfaceTransfer::internalInterpolate:ERROR donor stencil is INVALID. mask==0 at some pts.\n"
                                    "    : myid=%i, grid=%i, j=%i, width=%i, il=(%i,%i,%i)\n",myid,grid,j,width,i1a,i2a,i3a);
                  }
              }
        } 



        if( numberOfDimensions==2 )
        {
            const int i3=vs.getBase(2);

      // *wdh* 105031 bug fixed -- cannot define VS as a 2d array with Array_View_Pointer2 
      //  when the array vs has a non-zero component base
            const real *vsp = vs.Array_Descriptor.Array_View_Pointer3;
            const int vsDim0=vs.getRawDataSize(0);
            const int vsDim1=vs.getRawDataSize(1);
            const int vsDim2=vs.getRawDataSize(2);
#undef VS
#define VS(i0,i1,i2,i3) vsp[i0+vsDim0*(i1+vsDim1*(i2+vsDim2*(i3)))]

            if( maxInterpolationWidth==3 )
            {
      	if( explicitInterpolationStorageOption==precomputeAllCoefficients )
      	{
          // ja = IA(j);
          // ja = IA(j); j1 = ID(ja,0); j2=ID(ja,1); j3=ID(ja,2); gj = ID(ja,3); 
          // targetDataArray[gj](j1,j2,j3,c2)=


        //   if( false && c1Base==c1Bound )
        //   {
        //     for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
        //     for( j=0; j<nil; j++ )
        //     {
        //       int i1=IL1(j), i2=IL2(j);
        //       (*targetDataArray[gj])(j1,j2,j3,c2)=(cf(j,0,0)*VS(i1,i2,i3,c1)+cf(j,1,0)*VS(i1+1,i2,i3,c1)+cf(j,2,0)*VS(i1+2,i2,i3,c1)+			cf(j,0,1)*VS(i1,i2+1,i3,c1)+cf(j,1,1)*VS(i1+1,i2+1,i3,c1)+cf(j,2,1)*VS(i1+2,i2+1,i3,c1)+			cf(j,0,2)*VS(i1,i2+2,i3,c1)+cf(j,1,2)*VS(i1+1,i2+2,i3,c1)+cf(j,2,2)*VS(i1+2,i2+2,i3,c1));
        //       k++;
        //     }
        //   }
                for( j=0; j<nil; j++ )
                {
                  int i1=IL1(j), i2=IL2(j);
                  ja = IA(j); j1=ID(ja,0); j2=ID(ja,1); j3=ID(ja,2); gj=ID(ja,3); 
                  for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                  {
                      (*targetDataArray[gj])(j1,j2,j3,c2)=(cf(j,0,0)*VS(i1,i2,i3,c1)+cf(j,1,0)*VS(i1+1,i2,i3,c1)+cf(j,2,0)*VS(i1+2,i2,i3,c1)+			cf(j,0,1)*VS(i1,i2+1,i3,c1)+cf(j,1,1)*VS(i1+1,i2+1,i3,c1)+cf(j,2,1)*VS(i1+2,i2+1,i3,c1)+			cf(j,0,2)*VS(i1,i2+2,i3,c1)+cf(j,1,2)*VS(i1+1,i2+2,i3,c1)+cf(j,2,2)*VS(i1+2,i2+2,i3,c1));
                      k++;
                  }
                }
      	}
      	else
      	{
                for( j=0; j<nil; j++ )
                {
                    iw=VIW(j); i1=IL1(j); i2=IL2(j);
                    ja = IA(j); j1=ID(ja,0); j2=ID(ja,1); j3=ID(ja,2); gj=ID(ja,3); 
          //   printf(" Interp pt j=%i : (%i,%i,%i) grid=%i from i=(%i,%i) iw=%i\n",j,j1,j2,j3,gj,i1,i2,iw);
          //   printf("    v(i1,i2,i3,c1Base=%i)=%e \n",VS(i1  ,i2  ,i3,c1Base),c1Base);
                    if( iw==3 )
                    {
                        cr0 = q30(cfs(j,0));
                        cs0 = q30(cfs(j,1));
                        cr1 = q31(cfs(j,0));
                        cs1 = q31(cfs(j,1));
                        cr2 = q32(cfs(j,0));
                        cs2 = q32(cfs(j,1));
                        for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                        {
                            (*targetDataArray[gj])(j1,j2,j3,c2) = cs0*(cr0*VS(i1  ,i2  ,i3,c1)+cr1*VS(i1+1,i2  ,i3,c1)+cr2*VS(i1+2,i2  ,i3,c1))+cs1*(cr0*VS(i1  ,i2+1,i3,c1)+cr1*VS(i1+1,i2+1,i3,c1)+cr2*VS(i1+2,i2+1,i3,c1))+cs2*(cr0*VS(i1  ,i2+2,i3,c1)+cr1*VS(i1+1,i2+2,i3,c1)+cr2*VS(i1+2,i2+2,i3,c1));
                            k++;
                        }
                    }
                    else if( iw==2 )
                    {
                        cr0 = q20(cfs(j,0));
                        cs0 = q20(cfs(j,1));
                        cr1 = q21(cfs(j,0));
                        cs1 = q21(cfs(j,1));
                        for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                        {
                            (*targetDataArray[gj])(j1,j2,j3,c2) = cs0*(cr0*VS(i1  ,i2  ,i3,c1)+cr1*VS(i1+1,i2  ,i3,c1))+cs1*(cr0*VS(i1  ,i2+1,i3,c1)+cr1*VS(i1+1,i2+1,i3,c1));
                            k++; 
                        }
                    }
                    else if( iw==1 )
                    {
                        for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                        {
                            (*targetDataArray[gj])(j1,j2,j3,c2) = VS(i1  ,i2  ,i3,c1);
                            k++; 
                        }
                    }
                    else
                    {
                        Overture::abort("ERROR: unexpected interp width");
                    }
                }
      	}
            }
            else if( maxInterpolationWidth==2 )
            {
      	if( explicitInterpolationStorageOption==precomputeAllCoefficients )
      	{
        //   if( false && c1Base==c1Bound )
        //   {
        //     for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
        //     for( j=0; j<nil; j++ )
        //     {
        //       int i1=IL1(j), i2=IL2(j);
        //       (*targetDataArray[gj])(j1,j2,j3,c2)=(cf(j,0,0)*VS(i1,i2,i3,c1)+cf(j,1,0)*VS(i1+1,i2,i3,c1)+			cf(j,0,1)*VS(i1,i2+1,i3,c1)+cf(j,1,1)*VS(i1+1,i2+1,i3,c1));
        //       k++;
        //     }
        //   }
                for( j=0; j<nil; j++ )
                {
                  int i1=IL1(j), i2=IL2(j);
                  ja = IA(j); j1=ID(ja,0); j2=ID(ja,1); j3=ID(ja,2); gj=ID(ja,3); 
                  for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                  {
                      (*targetDataArray[gj])(j1,j2,j3,c2)=(cf(j,0,0)*VS(i1,i2,i3,c1)+cf(j,1,0)*VS(i1+1,i2,i3,c1)+			cf(j,0,1)*VS(i1,i2+1,i3,c1)+cf(j,1,1)*VS(i1+1,i2+1,i3,c1));
                      k++;
                  }
                }
      	}
      	else
      	{
                for( j=0; j<nil; j++ )
                {
                    iw=VIW(j); i1=IL1(j); i2=IL2(j);
                    ja = IA(j); j1=ID(ja,0); j2=ID(ja,1); j3=ID(ja,2); gj = ID(ja,3); 
                    if( iw==2 )
                    {
                        cr0 = q20(cfs(j,0));
                        cs0 = q20(cfs(j,1));
                        cr1 = q21(cfs(j,0));
                        cs1 = q21(cfs(j,1));
                        for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                        {
                            (*targetDataArray[gj])(j1,j2,j3,c2) = cs0*(cr0*VS(i1  ,i2  ,i3,c1)+cr1*VS(i1+1,i2  ,i3,c1))+cs1*(cr0*VS(i1  ,i2+1,i3,c1)+cr1*VS(i1+1,i2+1,i3,c1));
                            k++; 
                        }
                    }
                    else if( iw==1 )
                    {
                        for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                        {
                            (*targetDataArray[gj])(j1,j2,j3,c2) = VS(i1  ,i2  ,i3,c1);
                            k++; 
                        }
                    }
                    else
                    {
                        Overture::abort("ERROR: unexpected interp width");
                    }
                }
      	}
            }
            else if( maxInterpolationWidth==1 )
            {
      //   if( false && c1Base==c1Bound )
      //   {
      //     for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
      //     for( j=0; j<nil; j++ )
      //     {
      //       int i1=IL1(j), i2=IL2(j);
      //       (*targetDataArray[gj])(j1,j2,j3,c2)=VS(i1,i2,i3,c1);
      //       k++;
      //     }
      //   }
            for( j=0; j<nil; j++ )
            {
              int i1=IL1(j), i2=IL2(j);
              ja = IA(j); j1=ID(ja,0); j2=ID(ja,1); j3=ID(ja,2); gj=ID(ja,3); 
              for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
              {
                  (*targetDataArray[gj])(j1,j2,j3,c2)=VS(i1,i2,i3,c1);
                  k++;
              }
            }
            }
            else if( maxInterpolationWidth==5 || 
                              (explicitInterpolationStorageOption==precomputeNoCoefficients && maxInterpolationWidth<=5 ) ) 
            {
        // we can do maxInterpolationWidth==4 here for the sparse storage option
#define IW5(m1) (cf(j,0,m1)*VS(i1  ,i2+m1,i3,c1)+cf(j,1,m1)*VS(i1+1,i2+m1,i3,c1)+cf(j,2,m1)*VS(i1+2,i2+m1,i3,c1)+cf(j,3,m1)*VS(i1+3,i2+m1,i3,c1)+cf(j,4,m1)*VS(i1+4,i2+m1,i3,c1))

      	if( explicitInterpolationStorageOption==precomputeAllCoefficients )
      	{
        //   if( false && c1Base==c1Bound )
        //   {
        //     for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
        //     for( j=0; j<nil; j++ )
        //     {
        //       int i1=IL1(j), i2=IL2(j);
        //       (*targetDataArray[gj])(j1,j2,j3,c2)=IW5(0)+IW5(1)+IW5(2)+IW5(3)+IW5(4);
        //       k++;
        //     }
        //   }
                for( j=0; j<nil; j++ )
                {
                  int i1=IL1(j), i2=IL2(j);
                  ja = IA(j); j1=ID(ja,0); j2=ID(ja,1); j3=ID(ja,2); gj=ID(ja,3); 
                  for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                  {
                      (*targetDataArray[gj])(j1,j2,j3,c2)=IW5(0)+IW5(1)+IW5(2)+IW5(3)+IW5(4);
                      k++;
                  }
                }
      	}
      	else
      	{
                for( j=0; j<nil; j++ )
                {
                    iw=VIW(j); i1=IL1(j); i2=IL2(j);
                    ja = IA(j); j1=ID(ja,0); j2=ID(ja,1); j3=ID(ja,2); gj=ID(ja,3); 
                    if( iw==5 )
                    {
                        cr0 = q50(cfs(j,0));
                        cs0 = q50(cfs(j,1));
                        cr1 = q51(cfs(j,0));
                        cs1 = q51(cfs(j,1));
                        cr2 = q52(cfs(j,0));
                        cs2 = q52(cfs(j,1));
                        cr3 = q53(cfs(j,0));
                        cs3 = q53(cfs(j,1));
                        cr4 = q54(cfs(j,0));
                        cs4 = q54(cfs(j,1));
                        for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                        {
                            (*targetDataArray[gj])(j1,j2,j3,c2) = cs0*(cr0*VS(i1  ,i2  ,i3,c1)+cr1*VS(i1+1,i2  ,i3,c1)+cr2*VS(i1+2,i2  ,i3,c1)+cr3*VS(i1+3,i2  ,i3,c1)+cr4*VS(i1+4,i2  ,i3,c1))+cs1*(cr0*VS(i1  ,i2+1,i3,c1)+cr1*VS(i1+1,i2+1,i3,c1)+cr2*VS(i1+2,i2+1,i3,c1)+cr3*VS(i1+3,i2+1,i3,c1)+cr4*VS(i1+4,i2+1,i3,c1))+cs2*(cr0*VS(i1  ,i2+2,i3,c1)+cr1*VS(i1+1,i2+2,i3,c1)+cr2*VS(i1+2,i2+2,i3,c1)+cr3*VS(i1+3,i2+2,i3,c1)+cr4*VS(i1+4,i2+2,i3,c1))+cs3*(cr0*VS(i1  ,i2+3,i3,c1)+cr1*VS(i1+1,i2+3,i3,c1)+cr2*VS(i1+2,i2+3,i3,c1)+cr3*VS(i1+3,i2+3,i3,c1)+cr4*VS(i1+4,i2+3,i3,c1))+cs4*(cr0*VS(i1  ,i2+4,i3,c1)+cr1*VS(i1+1,i2+4,i3,c1)+cr2*VS(i1+2,i2+4,i3,c1)+cr3*VS(i1+3,i2+4,i3,c1)+cr4*VS(i1+4,i2+4,i3,c1));
                            k++;
                        }
                    }
                    else if( iw==4 )
                    {
                        cr0 = q40(cfs(j,0));
                        cs0 = q40(cfs(j,1));
                        cr1 = q41(cfs(j,0));
                        cs1 = q41(cfs(j,1));
                        cr2 = q42(cfs(j,0));
                        cs2 = q42(cfs(j,1));
                        cr3 = q43(cfs(j,0));
                        cs3 = q43(cfs(j,1));
                        for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                        {
                            (*targetDataArray[gj])(j1,j2,j3,c2) = cs0*(cr0*VS(i1  ,i2  ,i3,c1)+cr1*VS(i1+1,i2  ,i3,c1)+cr2*VS(i1+2,i2  ,i3,c1)+cr3*VS(i1+3,i2  ,i3,c1))+cs1*(cr0*VS(i1  ,i2+1,i3,c1)+cr1*VS(i1+1,i2+1,i3,c1)+cr2*VS(i1+2,i2+1,i3,c1)+cr3*VS(i1+3,i2+1,i3,c1))+cs2*(cr0*VS(i1  ,i2+2,i3,c1)+cr1*VS(i1+1,i2+2,i3,c1)+cr2*VS(i1+2,i2+2,i3,c1)+cr3*VS(i1+3,i2+2,i3,c1))+cs3*(cr0*VS(i1  ,i2+3,i3,c1)+cr1*VS(i1+1,i2+3,i3,c1)+cr2*VS(i1+2,i2+3,i3,c1)+cr3*VS(i1+3,i2+3,i3,c1));
                            k++;
                        }
                    }
                    else if( iw==3 )
                    {
                        cr0 = q30(cfs(j,0));
                        cs0 = q30(cfs(j,1));
                        cr1 = q31(cfs(j,0));
                        cs1 = q31(cfs(j,1));
                        cr2 = q32(cfs(j,0));
                        cs2 = q32(cfs(j,1));
                        for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                        {
                            (*targetDataArray[gj])(j1,j2,j3,c2) = cs0*(cr0*VS(i1  ,i2  ,i3,c1)+cr1*VS(i1+1,i2  ,i3,c1)+cr2*VS(i1+2,i2  ,i3,c1))+cs1*(cr0*VS(i1  ,i2+1,i3,c1)+cr1*VS(i1+1,i2+1,i3,c1)+cr2*VS(i1+2,i2+1,i3,c1))+cs2*(cr0*VS(i1  ,i2+2,i3,c1)+cr1*VS(i1+1,i2+2,i3,c1)+cr2*VS(i1+2,i2+2,i3,c1));
                            k++;
                        }
                    }
                    else if( iw==2 )
                    {
                        cr0 = q20(cfs(j,0));
                        cs0 = q20(cfs(j,1));
                        cr1 = q21(cfs(j,0));
                        cs1 = q21(cfs(j,1));
                        for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                        {
                            (*targetDataArray[gj])(j1,j2,j3,c2) = cs0*(cr0*VS(i1  ,i2  ,i3,c1)+cr1*VS(i1+1,i2  ,i3,c1))+cs1*(cr0*VS(i1  ,i2+1,i3,c1)+cr1*VS(i1+1,i2+1,i3,c1));
                            k++; 
                        }
                    }
                    else if( iw==1 )
                    {
                        for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                        {
                            (*targetDataArray[gj])(j1,j2,j3,c2) = VS(i1  ,i2  ,i3,c1);
                            k++; 
                        }
                    }
                    else
                    {
                        Overture::abort("ERROR: unexpected interp width");
                    }
                }
      	}
#undef IW5
            }
            else
            {
	// general case
      	if( explicitInterpolationStorageOption!=precomputeAllCoefficients )
      	{
        	  OV_ABORT("ERROR: un-implemented interpolation width -- finish me!");
      	}
      	for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
        	  for( j=0; j<nil; j++ )
        	  {
          	    int i1=IL1(j), i2=IL2(j);
          	    real value=0.;
          	    for( m2=0; m2< width[axis2]; m2++ ) 
            	      for( m1=0; m1< width[axis1]; m1++ ) 
            		value+=cf(j,m1,m2)*VS(i1+m1,i2+m2,i3,c1);

          	    (*targetDataArray[gj])(j1,j2,j3,c2)=value;

          	    k++;
        	  }
            }
        	  
        }
        else if( numberOfDimensions==3 )
        {

#undef c
#define c(i,m1,m2,m3) coeffgp[m1+width[0]*(m2+width[1]*m3)][i]

            const real *vsp = vs.Array_Descriptor.Array_View_Pointer3;
            const int vsDim0=vs.getRawDataSize(0);
            const int vsDim1=vs.getRawDataSize(1);
            const int vsDim2=vs.getRawDataSize(2);
#undef VS
#define VS(i0,i1,i2,i3) vsp[i0+vsDim0*(i1+vsDim1*(i2+vsDim2*(i3)))]

      // *** new way
#undef c
#define c(i,m1,m2,m3)  coeffap[i+nil*((m1)+width[0]*((m2)+width[1]*(m3)))]

            if( maxInterpolationWidth==3 )
            {
      	if( explicitInterpolationStorageOption==precomputeAllCoefficients )
      	{
        //   if( false && c1Base==c1Bound )
        //   {
        //     for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
        //     for( j=0; j<nil; j++ )
        //     {
        //       int i1=IL1(j), i2=IL2(j), i3=IL3(j);
        //       (*targetDataArray[gj])(j1,j2,j3,c2)=		(c(j,0,0,0)*VS(i1,i2,i3,c1)+c(j,1,0,0)*VS(i1+1,i2,i3,c1)+c(j,2,0,0)*VS(i1+2,i2,i3,c1)+		c(j,0,1,0)*VS(i1,i2+1,i3,c1)+c(j,1,1,0)*VS(i1+1,i2+1,i3,c1)+c(j,2,1,0)*VS(i1+2,i2+1,i3,c1)+		c(j,0,2,0)*VS(i1,i2+2,i3,c1)+c(j,1,2,0)*VS(i1+1,i2+2,i3,c1)+c(j,2,2,0)*VS(i1+2,i2+2,i3,c1)+		c(j,0,0,1)*VS(i1,i2,i3+1,c1)+c(j,1,0,1)*VS(i1+1,i2,i3+1,c1)+c(j,2,0,1)*VS(i1+2,i2,i3+1,c1)+		c(j,0,1,1)*VS(i1,i2+1,i3+1,c1)+c(j,1,1,1)*VS(i1+1,i2+1,i3+1,c1)+c(j,2,1,1)*VS(i1+2,i2+1,i3+1,c1)+		c(j,0,2,1)*VS(i1,i2+2,i3+1,c1)+c(j,1,2,1)*VS(i1+1,i2+2,i3+1,c1)+c(j,2,2,1)*VS(i1+2,i2+2,i3+1,c1)+		c(j,0,0,2)*VS(i1,i2,i3+2,c1)+c(j,1,0,2)*VS(i1+1,i2,i3+2,c1)+c(j,2,0,2)*VS(i1+2,i2,i3+2,c1)+		c(j,0,1,2)*VS(i1,i2+1,i3+2,c1)+c(j,1,1,2)*VS(i1+1,i2+1,i3+2,c1)+c(j,2,1,2)*VS(i1+2,i2+1,i3+2,c1)+		c(j,0,2,2)*VS(i1,i2+2,i3+2,c1)+c(j,1,2,2)*VS(i1+1,i2+2,i3+2,c1)+c(j,2,2,2)*VS(i1+2,i2+2,i3+2,c1));
        //       k++;
        //     }
        //   }
                for( j=0; j<nil; j++ )
                {
                  int i1=IL1(j), i2=IL2(j), i3=IL3(j);
                  ja = IA(j); j1=ID(ja,0); j2=ID(ja,1); j3=ID(ja,2); gj=ID(ja,3); 
                  for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                  {
                      (*targetDataArray[gj])(j1,j2,j3,c2)=		(c(j,0,0,0)*VS(i1,i2,i3,c1)+c(j,1,0,0)*VS(i1+1,i2,i3,c1)+c(j,2,0,0)*VS(i1+2,i2,i3,c1)+		c(j,0,1,0)*VS(i1,i2+1,i3,c1)+c(j,1,1,0)*VS(i1+1,i2+1,i3,c1)+c(j,2,1,0)*VS(i1+2,i2+1,i3,c1)+		c(j,0,2,0)*VS(i1,i2+2,i3,c1)+c(j,1,2,0)*VS(i1+1,i2+2,i3,c1)+c(j,2,2,0)*VS(i1+2,i2+2,i3,c1)+		c(j,0,0,1)*VS(i1,i2,i3+1,c1)+c(j,1,0,1)*VS(i1+1,i2,i3+1,c1)+c(j,2,0,1)*VS(i1+2,i2,i3+1,c1)+		c(j,0,1,1)*VS(i1,i2+1,i3+1,c1)+c(j,1,1,1)*VS(i1+1,i2+1,i3+1,c1)+c(j,2,1,1)*VS(i1+2,i2+1,i3+1,c1)+		c(j,0,2,1)*VS(i1,i2+2,i3+1,c1)+c(j,1,2,1)*VS(i1+1,i2+2,i3+1,c1)+c(j,2,2,1)*VS(i1+2,i2+2,i3+1,c1)+		c(j,0,0,2)*VS(i1,i2,i3+2,c1)+c(j,1,0,2)*VS(i1+1,i2,i3+2,c1)+c(j,2,0,2)*VS(i1+2,i2,i3+2,c1)+		c(j,0,1,2)*VS(i1,i2+1,i3+2,c1)+c(j,1,1,2)*VS(i1+1,i2+1,i3+2,c1)+c(j,2,1,2)*VS(i1+2,i2+1,i3+2,c1)+		c(j,0,2,2)*VS(i1,i2+2,i3+2,c1)+c(j,1,2,2)*VS(i1+1,i2+2,i3+2,c1)+c(j,2,2,2)*VS(i1+2,i2+2,i3+2,c1));
                      k++;
                  }
                }

          	    }
      	else
      	{
                for( j=0; j<nil; j++ )
                {
                    iw=VIW(j); i1=IL1(j); i2=IL2(j); i3=IL3(j);
                    ja = IA(j); j1=ID(ja,0); j2=ID(ja,1); j3=ID(ja,2); gj=ID(ja,3); 
                    if( iw==3 )
                    {
                        cr0 = q30(cfs(j,0));
                        cs0 = q30(cfs(j,1));
                        ct0 = q30(cfs(j,2));
                        cr1 = q31(cfs(j,0));
                        cs1 = q31(cfs(j,1));
                        ct1 = q31(cfs(j,2));
                        cr2 = q32(cfs(j,0));
                        cs2 = q32(cfs(j,1));
                        ct2 = q32(cfs(j,2));
                        for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                        {
                            (*targetDataArray[gj])(j1,j2,j3,c2) = ct0*(	    cs0*(cr0*VS(i1,i2,i3+0,c1)+cr1*VS(i1+1,i2,i3+0,c1)+cr2*VS(i1+2,i2,i3+0,c1))	   +cs1*(cr0*VS(i1,i2+1,i3+0,c1)+cr1*VS(i1+1,i2+1,i3+0,c1)+cr2*VS(i1+2,i2+1,i3+0,c1))	   +cs2*(cr0*VS(i1,i2+2,i3+0,c1)+cr1*VS(i1+1,i2+2,i3+0,c1)+cr2*VS(i1+2,i2+2,i3+0,c1))	)+ct1*(	     cs0*(cr0*VS(i1,i2,i3+1,c1)+cr1*VS(i1+1,i2,i3+1,c1)+cr2*VS(i1+2,i2,i3+1,c1))	    +cs1*(cr0*VS(i1,i2+1,i3+1,c1)+cr1*VS(i1+1,i2+1,i3+1,c1)+cr2*VS(i1+2,i2+1,i3+1,c1))	    +cs2*(cr0*VS(i1,i2+2,i3+1,c1)+cr1*VS(i1+1,i2+2,i3+1,c1)+cr2*VS(i1+2,i2+2,i3+1,c1))	)+ct2*(	     cs0*(cr0*VS(i1,i2,i3+2,c1)+cr1*VS(i1+1,i2,i3+2,c1)+cr2*VS(i1+2,i2,i3+2,c1))	    +cs1*(cr0*VS(i1,i2+1,i3+2,c1)+cr1*VS(i1+1,i2+1,i3+2,c1)+cr2*VS(i1+2,i2+1,i3+2,c1))	    +cs2*(cr0*VS(i1,i2+2,i3+2,c1)+cr1*VS(i1+1,i2+2,i3+2,c1)+cr2*VS(i1+2,i2+2,i3+2,c1))	);
                            k++;
                        }
                    }
                    else if( iw==2 )
                    {
                        cr0 = q20(cfs(j,0)); 
                        cs0 = q20(cfs(j,1)); 
                        ct0 = q20(cfs(j,2)); 
                        cr1 = q21(cfs(j,0)); 
                        cs1 = q21(cfs(j,1)); 
                        ct1 = q21(cfs(j,2)); 
                        for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                        {
                            (*targetDataArray[gj])(j1,j2,j3,c2) = ct0*(	    cs0*(cr0*VS(i1,i2,i3+0,c1)+cr1*VS(i1+1,i2,i3+0,c1))	   +cs1*(cr0*VS(i1,i2+1,i3+0,c1)+cr1*VS(i1+1,i2+1,i3+0,c1))	)+ct1*(	     cs0*(cr0*VS(i1,i2,i3+1,c1)+cr1*VS(i1+1,i2,i3+1,c1))	    +cs1*(cr0*VS(i1,i2+1,i3+1,c1)+cr1*VS(i1+1,i2+1,i3+1,c1))	);
                            k++;
                        }
                    }
                    else if( iw==1 )
                    {
                          for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                          {
                              (*targetDataArray[gj])(j1,j2,j3,c2) = VS(i1  ,i2  ,i3,c1);
                              k++; 
                          }
                    }
                    else
                    {
                        Overture::abort("ERROR: unexpected interp width");
                    }
                }
      	}
            }
            else if( maxInterpolationWidth==2 )
            {
      	if( explicitInterpolationStorageOption==precomputeAllCoefficients )
      	{
        //   if( false && c1Base==c1Bound )
        //   {
        //     for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
        //     for( j=0; j<nil; j++ )
        //     {
        //       int i1=IL1(j), i2=IL2(j), i3=IL3(j);
        //       (*targetDataArray[gj])(j1,j2,j3,c2)=		(c(j,0,0,0)*VS(i1,i2,i3,c1)+c(j,1,0,0)*VS(i1+1,i2,i3,c1)+		c(j,0,1,0)*VS(i1,i2+1,i3,c1)+c(j,1,1,0)*VS(i1+1,i2+1,i3,c1)+		c(j,0,0,1)*VS(i1,i2,i3+1,c1)+c(j,1,0,1)*VS(i1+1,i2,i3+1,c1)+		c(j,0,1,1)*VS(i1,i2+1,i3+1,c1)+c(j,1,1,1)*VS(i1+1,i2+1,i3+1,c1));
        //       k++;
        //     }
        //   }
                for( j=0; j<nil; j++ )
                {
                  int i1=IL1(j), i2=IL2(j), i3=IL3(j);
                  ja = IA(j); j1=ID(ja,0); j2=ID(ja,1); j3=ID(ja,2); gj=ID(ja,3); 
                  for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                  {
                      (*targetDataArray[gj])(j1,j2,j3,c2)=		(c(j,0,0,0)*VS(i1,i2,i3,c1)+c(j,1,0,0)*VS(i1+1,i2,i3,c1)+		c(j,0,1,0)*VS(i1,i2+1,i3,c1)+c(j,1,1,0)*VS(i1+1,i2+1,i3,c1)+		c(j,0,0,1)*VS(i1,i2,i3+1,c1)+c(j,1,0,1)*VS(i1+1,i2,i3+1,c1)+		c(j,0,1,1)*VS(i1,i2+1,i3+1,c1)+c(j,1,1,1)*VS(i1+1,i2+1,i3+1,c1));
                      k++;
                  }
                }
      	}
      	else
      	{
                for( j=0; j<nil; j++ )
                {
                    iw=VIW(j); i1=IL1(j); i2=IL2(j); i3=IL3(j);
                    ja = IA(j); j1=ID(ja,0); j2=ID(ja,1); j3=ID(ja,2); gj=ID(ja,3); 
                    if( iw==2 )
                    {
                        cr0 = q20(cfs(j,0)); 
                        cs0 = q20(cfs(j,1)); 
                        ct0 = q20(cfs(j,2)); 
                        cr1 = q21(cfs(j,0)); 
                        cs1 = q21(cfs(j,1)); 
                        ct1 = q21(cfs(j,2)); 
                        for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                        {
                            (*targetDataArray[gj])(j1,j2,j3,c2) = ct0*(	    cs0*(cr0*VS(i1,i2,i3+0,c1)+cr1*VS(i1+1,i2,i3+0,c1))	   +cs1*(cr0*VS(i1,i2+1,i3+0,c1)+cr1*VS(i1+1,i2+1,i3+0,c1))	)+ct1*(	     cs0*(cr0*VS(i1,i2,i3+1,c1)+cr1*VS(i1+1,i2,i3+1,c1))	    +cs1*(cr0*VS(i1,i2+1,i3+1,c1)+cr1*VS(i1+1,i2+1,i3+1,c1))	);
                            k++;
                        }
                    }
                    else if( iw==1 )
                    {
                          for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                          {
                              (*targetDataArray[gj])(j1,j2,j3,c2) = VS(i1  ,i2  ,i3,c1);
                              k++; 
                          }
                    }
                    else
                    {
                        Overture::abort("ERROR: unexpected interp width");
                    }
                }
      	}
            }
            else if( maxInterpolationWidth==5 || 
                              (explicitInterpolationStorageOption==precomputeNoCoefficients && maxInterpolationWidth<=5 ) ) 
            {
        // we can do maxInterpolationWidth==4 here for the sparse storage option

#define IW5A(m1,m2) (c(j,0,m1,m2)*VS(i1  ,i2+m1,i3+m2,c1)+c(j,1,m1,m2)*VS(i1+1,i2+m1,i3+m2,c1)+c(j,2,m1,m2)*VS(i1+2,i2+m1,i3+m2,c1)+c(j,3,m1,m2)*VS(i1+3,i2+m1,i3+m2,c1)+c(j,4,m1,m2)*VS(i1+4,i2+m1,i3+m2,c1))
#define IW5(m2) (IW5A(0,m2)+IW5A(1,m2)+IW5A(2,m2)+IW5A(3,m2)+IW5A(4,m2))

      	if( explicitInterpolationStorageOption==precomputeAllCoefficients )
      	{
        //   if( false && c1Base==c1Bound )
        //   {
        //     for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
        //     for( j=0; j<nil; j++ )
        //     {
        //       int i1=IL1(j), i2=IL2(j), i3=IL3(j);
        //       (*targetDataArray[gj])(j1,j2,j3,c2)=IW5(0)+IW5(1)+IW5(2)+IW5(3)+IW5(4);
        //       k++;
        //     }
        //   }
                for( j=0; j<nil; j++ )
                {
                  int i1=IL1(j), i2=IL2(j), i3=IL3(j);
                  ja = IA(j); j1=ID(ja,0); j2=ID(ja,1); j3=ID(ja,2); gj=ID(ja,3); 
                  for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                  {
                      (*targetDataArray[gj])(j1,j2,j3,c2)=IW5(0)+IW5(1)+IW5(2)+IW5(3)+IW5(4);
                      k++;
                  }
                }
      	}
      	else
      	{
                for( j=0; j<nil; j++ )
                {
                    iw=VIW(j); i1=IL1(j); i2=IL2(j); i3=IL3(j);
                    ja = IA(j); j1=ID(ja,0); j2=ID(ja,1); j3=ID(ja,2); gj=ID(ja,3); 
                    if( iw==5 )
                    {
                            cr0 = q50(cfs(j,0));
                            cs0 = q50(cfs(j,1));
                            ct0 = q50(cfs(j,2));
                            cr1 = q51(cfs(j,0));
                            cs1 = q51(cfs(j,1));
                            ct1 = q51(cfs(j,2));
                            cr2 = q52(cfs(j,0));
                            cs2 = q52(cfs(j,1));
                            ct2 = q52(cfs(j,2));
                            cr3 = q53(cfs(j,0));
                            cs3 = q53(cfs(j,1));
                            ct3 = q53(cfs(j,2));
                            cr4 = q54(cfs(j,0));
                            cs4 = q54(cfs(j,1));
                            ct4 = q54(cfs(j,2));
                            for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                            {
                        (*targetDataArray[gj])(j1,j2,j3,c2) = ct0*(cs0*(cr0*VS(i1,i2  ,i3+0,c1)+cr1*VS(i1+1,i2  ,i3+0,c1)+cr2*VS(i1+2,i2  ,i3+0,c1)+cr3*VS(i1+3,i2  ,i3+0,c1)+cr4*VS(i1+4,i2  ,i3+0,c1))+cs1*(cr0*VS(i1,i2+1,i3+0,c1)+cr1*VS(i1+1,i2+1,i3+0,c1)+cr2*VS(i1+2,i2+1,i3+0,c1)+cr3*VS(i1+3,i2+1,i3+0,c1)+cr4*VS(i1+4,i2+1,i3+0,c1))+cs2*(cr0*VS(i1,i2+2,i3+0,c1)+cr1*VS(i1+1,i2+2,i3+0,c1)+cr2*VS(i1+2,i2+2,i3+0,c1)+cr3*VS(i1+3,i2+2,i3+0,c1)+cr4*VS(i1+4,i2+2,i3+0,c1))+cs3*(cr0*VS(i1,i2+3,i3+0,c1)+cr1*VS(i1+1,i2+3,i3+0,c1)+cr2*VS(i1+2,i2+3,i3+0,c1)+cr3*VS(i1+3,i2+3,i3+0,c1)+cr4*VS(i1+4,i2+3,i3+0,c1))+cs4*(cr0*VS(i1,i2+4,i3+0,c1)+cr1*VS(i1+1,i2+4,i3+0,c1)+cr2*VS(i1+2,i2+4,i3+0,c1)+cr3*VS(i1+3,i2+4,i3+0,c1)+cr4*VS(i1+4,i2+4,i3+0,c1)))+ct1*(cs0*(cr0*VS(i1,i2  ,i3+1,c1)+cr1*VS(i1+1,i2  ,i3+1,c1)+cr2*VS(i1+2,i2  ,i3+1,c1)+cr3*VS(i1+3,i2  ,i3+1,c1)+cr4*VS(i1+4,i2  ,i3+1,c1))+cs1*(cr0*VS(i1,i2+1,i3+1,c1)+cr1*VS(i1+1,i2+1,i3+1,c1)+cr2*VS(i1+2,i2+1,i3+1,c1)+cr3*VS(i1+3,i2+1,i3+1,c1)+cr4*VS(i1+4,i2+1,i3+1,c1))+cs2*(cr0*VS(i1,i2+2,i3+1,c1)+cr1*VS(i1+1,i2+2,i3+1,c1)+cr2*VS(i1+2,i2+2,i3+1,c1)+cr3*VS(i1+3,i2+2,i3+1,c1)+cr4*VS(i1+4,i2+2,i3+1,c1))+cs3*(cr0*VS(i1,i2+3,i3+1,c1)+cr1*VS(i1+1,i2+3,i3+1,c1)+cr2*VS(i1+2,i2+3,i3+1,c1)+cr3*VS(i1+3,i2+3,i3+1,c1)+cr4*VS(i1+4,i2+3,i3+1,c1))+cs4*(cr0*VS(i1,i2+4,i3+1,c1)+cr1*VS(i1+1,i2+4,i3+1,c1)+cr2*VS(i1+2,i2+4,i3+1,c1)+cr3*VS(i1+3,i2+4,i3+1,c1)+cr4*VS(i1+4,i2+4,i3+1,c1)))+ct2*(cs0*(cr0*VS(i1,i2  ,i3+2,c1)+cr1*VS(i1+1,i2  ,i3+2,c1)+cr2*VS(i1+2,i2  ,i3+2,c1)+cr3*VS(i1+3,i2  ,i3+2,c1)+cr4*VS(i1+4,i2  ,i3+2,c1))+cs1*(cr0*VS(i1,i2+1,i3+2,c1)+cr1*VS(i1+1,i2+1,i3+2,c1)+cr2*VS(i1+2,i2+1,i3+2,c1)+cr3*VS(i1+3,i2+1,i3+2,c1)+cr4*VS(i1+4,i2+1,i3+2,c1))+cs2*(cr0*VS(i1,i2+2,i3+2,c1)+cr1*VS(i1+1,i2+2,i3+2,c1)+cr2*VS(i1+2,i2+2,i3+2,c1)+cr3*VS(i1+3,i2+2,i3+2,c1)+cr4*VS(i1+4,i2+2,i3+2,c1))+cs3*(cr0*VS(i1,i2+3,i3+2,c1)+cr1*VS(i1+1,i2+3,i3+2,c1)+cr2*VS(i1+2,i2+3,i3+2,c1)+cr3*VS(i1+3,i2+3,i3+2,c1)+cr4*VS(i1+4,i2+3,i3+2,c1))+cs4*(cr0*VS(i1,i2+4,i3+2,c1)+cr1*VS(i1+1,i2+4,i3+2,c1)+cr2*VS(i1+2,i2+4,i3+2,c1)+cr3*VS(i1+3,i2+4,i3+2,c1)+cr4*VS(i1+4,i2+4,i3+2,c1)))+ct3*(cs0*(cr0*VS(i1,i2  ,i3+3,c1)+cr1*VS(i1+1,i2  ,i3+3,c1)+cr2*VS(i1+2,i2  ,i3+3,c1)+cr3*VS(i1+3,i2  ,i3+3,c1)+cr4*VS(i1+4,i2  ,i3+3,c1))+cs1*(cr0*VS(i1,i2+1,i3+3,c1)+cr1*VS(i1+1,i2+1,i3+3,c1)+cr2*VS(i1+2,i2+1,i3+3,c1)+cr3*VS(i1+3,i2+1,i3+3,c1)+cr4*VS(i1+4,i2+1,i3+3,c1))+cs2*(cr0*VS(i1,i2+2,i3+3,c1)+cr1*VS(i1+1,i2+2,i3+3,c1)+cr2*VS(i1+2,i2+2,i3+3,c1)+cr3*VS(i1+3,i2+2,i3+3,c1)+cr4*VS(i1+4,i2+2,i3+3,c1))+cs3*(cr0*VS(i1,i2+3,i3+3,c1)+cr1*VS(i1+1,i2+3,i3+3,c1)+cr2*VS(i1+2,i2+3,i3+3,c1)+cr3*VS(i1+3,i2+3,i3+3,c1)+cr4*VS(i1+4,i2+3,i3+3,c1))+cs4*(cr0*VS(i1,i2+4,i3+3,c1)+cr1*VS(i1+1,i2+4,i3+3,c1)+cr2*VS(i1+2,i2+4,i3+3,c1)+cr3*VS(i1+3,i2+4,i3+3,c1)+cr4*VS(i1+4,i2+4,i3+3,c1)))+ct4*(cs0*(cr0*VS(i1,i2  ,i3+4,c1)+cr1*VS(i1+1,i2  ,i3+4,c1)+cr2*VS(i1+2,i2  ,i3+4,c1)+cr3*VS(i1+3,i2  ,i3+4,c1)+cr4*VS(i1+4,i2  ,i3+4,c1))+cs1*(cr0*VS(i1,i2+1,i3+4,c1)+cr1*VS(i1+1,i2+1,i3+4,c1)+cr2*VS(i1+2,i2+1,i3+4,c1)+cr3*VS(i1+3,i2+1,i3+4,c1)+cr4*VS(i1+4,i2+1,i3+4,c1))+cs2*(cr0*VS(i1,i2+2,i3+4,c1)+cr1*VS(i1+1,i2+2,i3+4,c1)+cr2*VS(i1+2,i2+2,i3+4,c1)+cr3*VS(i1+3,i2+2,i3+4,c1)+cr4*VS(i1+4,i2+2,i3+4,c1))+cs3*(cr0*VS(i1,i2+3,i3+4,c1)+cr1*VS(i1+1,i2+3,i3+4,c1)+cr2*VS(i1+2,i2+3,i3+4,c1)+cr3*VS(i1+3,i2+3,i3+4,c1)+cr4*VS(i1+4,i2+3,i3+4,c1))+cs4*(cr0*VS(i1,i2+4,i3+4,c1)+cr1*VS(i1+1,i2+4,i3+4,c1)+cr2*VS(i1+2,i2+4,i3+4,c1)+cr3*VS(i1+3,i2+4,i3+4,c1)+cr4*VS(i1+4,i2+4,i3+4,c1)));
                              k++;
                            } // end for c1
                    }
                    else if( iw==4 )
                    {
                            cr0 = q40(cfs(j,0));
                            cs0 = q40(cfs(j,1));
                            ct0 = q40(cfs(j,2));
                            cr1 = q41(cfs(j,0));
                            cs1 = q41(cfs(j,1));
                            ct1 = q41(cfs(j,2));
                            cr2 = q42(cfs(j,0));
                            cs2 = q42(cfs(j,1));
                            ct2 = q42(cfs(j,2));
                            cr3 = q43(cfs(j,0));
                            cs3 = q43(cfs(j,1));
                            ct3 = q43(cfs(j,2));
                            for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                            {
                        (*targetDataArray[gj])(j1,j2,j3,c2) = ct0*(cs0*(cr0*VS(i1,i2  ,i3+0,c1)+cr1*VS(i1+1,i2  ,i3+0,c1)+cr2*VS(i1+2,i2  ,i3+0,c1)+cr3*VS(i1+3,i2  ,i3+0,c1))+cs1*(cr0*VS(i1,i2+1,i3+0,c1)+cr1*VS(i1+1,i2+1,i3+0,c1)+cr2*VS(i1+2,i2+1,i3+0,c1)+cr3*VS(i1+3,i2+1,i3+0,c1))+cs2*(cr0*VS(i1,i2+2,i3+0,c1)+cr1*VS(i1+1,i2+2,i3+0,c1)+cr2*VS(i1+2,i2+2,i3+0,c1)+cr3*VS(i1+3,i2+2,i3+0,c1))+cs3*(cr0*VS(i1,i2+3,i3+0,c1)+cr1*VS(i1+1,i2+3,i3+0,c1)+cr2*VS(i1+2,i2+3,i3+0,c1)+cr3*VS(i1+3,i2+3,i3+0,c1))+cs4*(cr0*VS(i1,i2+4,i3+0,c1)+cr1*VS(i1+1,i2+4,i3+0,c1)+cr2*VS(i1+2,i2+4,i3+0,c1)+cr3*VS(i1+3,i2+4,i3+0,c1)))+ct1*(cs0*(cr0*VS(i1,i2  ,i3+1,c1)+cr1*VS(i1+1,i2  ,i3+1,c1)+cr2*VS(i1+2,i2  ,i3+1,c1)+cr3*VS(i1+3,i2  ,i3+1,c1))+cs1*(cr0*VS(i1,i2+1,i3+1,c1)+cr1*VS(i1+1,i2+1,i3+1,c1)+cr2*VS(i1+2,i2+1,i3+1,c1)+cr3*VS(i1+3,i2+1,i3+1,c1))+cs2*(cr0*VS(i1,i2+2,i3+1,c1)+cr1*VS(i1+1,i2+2,i3+1,c1)+cr2*VS(i1+2,i2+2,i3+1,c1)+cr3*VS(i1+3,i2+2,i3+1,c1))+cs3*(cr0*VS(i1,i2+3,i3+1,c1)+cr1*VS(i1+1,i2+3,i3+1,c1)+cr2*VS(i1+2,i2+3,i3+1,c1)+cr3*VS(i1+3,i2+3,i3+1,c1))+cs4*(cr0*VS(i1,i2+4,i3+1,c1)+cr1*VS(i1+1,i2+4,i3+1,c1)+cr2*VS(i1+2,i2+4,i3+1,c1)+cr3*VS(i1+3,i2+4,i3+1,c1)))+ct2*(cs0*(cr0*VS(i1,i2  ,i3+2,c1)+cr1*VS(i1+1,i2  ,i3+2,c1)+cr2*VS(i1+2,i2  ,i3+2,c1)+cr3*VS(i1+3,i2  ,i3+2,c1))+cs1*(cr0*VS(i1,i2+1,i3+2,c1)+cr1*VS(i1+1,i2+1,i3+2,c1)+cr2*VS(i1+2,i2+1,i3+2,c1)+cr3*VS(i1+3,i2+1,i3+2,c1))+cs2*(cr0*VS(i1,i2+2,i3+2,c1)+cr1*VS(i1+1,i2+2,i3+2,c1)+cr2*VS(i1+2,i2+2,i3+2,c1)+cr3*VS(i1+3,i2+2,i3+2,c1))+cs3*(cr0*VS(i1,i2+3,i3+2,c1)+cr1*VS(i1+1,i2+3,i3+2,c1)+cr2*VS(i1+2,i2+3,i3+2,c1)+cr3*VS(i1+3,i2+3,i3+2,c1))+cs4*(cr0*VS(i1,i2+4,i3+2,c1)+cr1*VS(i1+1,i2+4,i3+2,c1)+cr2*VS(i1+2,i2+4,i3+2,c1)+cr3*VS(i1+3,i2+4,i3+2,c1)))+ct3*(cs0*(cr0*VS(i1,i2  ,i3+3,c1)+cr1*VS(i1+1,i2  ,i3+3,c1)+cr2*VS(i1+2,i2  ,i3+3,c1)+cr3*VS(i1+3,i2  ,i3+3,c1))+cs1*(cr0*VS(i1,i2+1,i3+3,c1)+cr1*VS(i1+1,i2+1,i3+3,c1)+cr2*VS(i1+2,i2+1,i3+3,c1)+cr3*VS(i1+3,i2+1,i3+3,c1))+cs2*(cr0*VS(i1,i2+2,i3+3,c1)+cr1*VS(i1+1,i2+2,i3+3,c1)+cr2*VS(i1+2,i2+2,i3+3,c1)+cr3*VS(i1+3,i2+2,i3+3,c1))+cs3*(cr0*VS(i1,i2+3,i3+3,c1)+cr1*VS(i1+1,i2+3,i3+3,c1)+cr2*VS(i1+2,i2+3,i3+3,c1)+cr3*VS(i1+3,i2+3,i3+3,c1))+cs4*(cr0*VS(i1,i2+4,i3+3,c1)+cr1*VS(i1+1,i2+4,i3+3,c1)+cr2*VS(i1+2,i2+4,i3+3,c1)+cr3*VS(i1+3,i2+4,i3+3,c1)));
                              k++;
                            } // end for c1
                    }
                    else if( iw==3 )
                    {
                        cr0 = q30(cfs(j,0));
                        cs0 = q30(cfs(j,1));
                        ct0 = q30(cfs(j,2));
                        cr1 = q31(cfs(j,0));
                        cs1 = q31(cfs(j,1));
                        ct1 = q31(cfs(j,2));
                        cr2 = q32(cfs(j,0));
                        cs2 = q32(cfs(j,1));
                        ct2 = q32(cfs(j,2));
                        for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                        {
                            (*targetDataArray[gj])(j1,j2,j3,c2) = ct0*(	    cs0*(cr0*VS(i1,i2,i3+0,c1)+cr1*VS(i1+1,i2,i3+0,c1)+cr2*VS(i1+2,i2,i3+0,c1))	   +cs1*(cr0*VS(i1,i2+1,i3+0,c1)+cr1*VS(i1+1,i2+1,i3+0,c1)+cr2*VS(i1+2,i2+1,i3+0,c1))	   +cs2*(cr0*VS(i1,i2+2,i3+0,c1)+cr1*VS(i1+1,i2+2,i3+0,c1)+cr2*VS(i1+2,i2+2,i3+0,c1))	)+ct1*(	     cs0*(cr0*VS(i1,i2,i3+1,c1)+cr1*VS(i1+1,i2,i3+1,c1)+cr2*VS(i1+2,i2,i3+1,c1))	    +cs1*(cr0*VS(i1,i2+1,i3+1,c1)+cr1*VS(i1+1,i2+1,i3+1,c1)+cr2*VS(i1+2,i2+1,i3+1,c1))	    +cs2*(cr0*VS(i1,i2+2,i3+1,c1)+cr1*VS(i1+1,i2+2,i3+1,c1)+cr2*VS(i1+2,i2+2,i3+1,c1))	)+ct2*(	     cs0*(cr0*VS(i1,i2,i3+2,c1)+cr1*VS(i1+1,i2,i3+2,c1)+cr2*VS(i1+2,i2,i3+2,c1))	    +cs1*(cr0*VS(i1,i2+1,i3+2,c1)+cr1*VS(i1+1,i2+1,i3+2,c1)+cr2*VS(i1+2,i2+1,i3+2,c1))	    +cs2*(cr0*VS(i1,i2+2,i3+2,c1)+cr1*VS(i1+1,i2+2,i3+2,c1)+cr2*VS(i1+2,i2+2,i3+2,c1))	);
                            k++;
                        }
                    }
                    else if( iw==2 )
                    {
                        cr0 = q20(cfs(j,0)); 
                        cs0 = q20(cfs(j,1)); 
                        ct0 = q20(cfs(j,2)); 
                        cr1 = q21(cfs(j,0)); 
                        cs1 = q21(cfs(j,1)); 
                        ct1 = q21(cfs(j,2)); 
                        for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                        {
                            (*targetDataArray[gj])(j1,j2,j3,c2) = ct0*(	    cs0*(cr0*VS(i1,i2,i3+0,c1)+cr1*VS(i1+1,i2,i3+0,c1))	   +cs1*(cr0*VS(i1,i2+1,i3+0,c1)+cr1*VS(i1+1,i2+1,i3+0,c1))	)+ct1*(	     cs0*(cr0*VS(i1,i2,i3+1,c1)+cr1*VS(i1+1,i2,i3+1,c1))	    +cs1*(cr0*VS(i1,i2+1,i3+1,c1)+cr1*VS(i1+1,i2+1,i3+1,c1))	);
                            k++;
                        }
                    }
                    else if( iw==1 )
                    {
                          for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
                          {
                              (*targetDataArray[gj])(j1,j2,j3,c2) = VS(i1  ,i2  ,i3,c1);
                              k++; 
                          }
                    }
                    else
                    {
                        Overture::abort("ERROR: unexpected interp width");
                    }
                }
      	}
#undef IW5
#undef IW55
            }
            else if( maxInterpolationWidth==1 )
            {
      //   if( false && c1Base==c1Bound )
      //   {
      //     for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
      //     for( j=0; j<nil; j++ )
      //     {
      //       int i1=IL1(j), i2=IL2(j), i3=IL3(j);
      //       (*targetDataArray[gj])(j1,j2,j3,c2)=VS(i1,i2,i3,c1);
      //       k++;
      //     }
      //   }
            for( j=0; j<nil; j++ )
            {
              int i1=IL1(j), i2=IL2(j), i3=IL3(j);
              ja = IA(j); j1=ID(ja,0); j2=ID(ja,1); j3=ID(ja,2); gj=ID(ja,3); 
              for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
              {
                  (*targetDataArray[gj])(j1,j2,j3,c2)=VS(i1,i2,i3,c1);
                  k++;
              }
            }
            }
            else
            {
	// general case
      	if( explicitInterpolationStorageOption!=precomputeAllCoefficients )
      	{
        	  OV_ABORT("ERROR: un-implemented interpolation width -- finish me!");
      	}
      	for( int c1=c1Base, c2=c2Base; c1<=c1Bound; c1++,c2++ )
        	  for( j=0; j<nil; j++ )
        	  {
          	    int i1=IL1(j), i2=IL2(j), i3=IL3(j);
          	    real value=0.;
          	    for( m3=0; m3< width[axis3]; m3++ ) 
            	      for( m2=0; m2< width[axis2]; m2++ ) 
            		for( m1=0; m1< width[axis1]; m1++ ) 
              		  value+=c(j,m1,m2,m3)*VS(i1+m1,i2+m2,i3+m3,c1);

          	    (*targetDataArray[gj])(j1,j2,j3,c2)=value;
          	    k++;
        	  }
            }

        	  
        }
        else
        {
            printf("InterfaceTransfer:internalInterpolate:ERROR: numberOfDimensions=%i\n",numberOfDimensions);
            OV_ABORT("ERROR");
        }
    } // for grid
    
    if( numberOfInvalidPoints>0 )
    {
        printf("InterfaceTransfer:internalInterpolate:ERROR: There were %i invalid interpolation points!\n", numberOfInvalidPoints);
    }
    else if( checkForValidInterpolation )
    {
        printf("InterfaceTransfer:internalInterpolate:INFO: All interpolation points were assigned.\n");
    }
    
    real time=getCPU()-time0;
    if( debug>0 ) 
    {
        printF(" >>>>>>>>> Time for InterfaceTransfer::interpolate =%8.2e  (%i interpolation pts)<<<<<<<\n",
         	   time,nid);
    }

    return 0;
}
