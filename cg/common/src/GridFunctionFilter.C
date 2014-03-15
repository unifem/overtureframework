#include "GridFunctionFilter.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "PlotStuff.h"


#define cgFilter EXTERN_C_NAME(cgfilter)

extern "C"
{
 void cgFilter(const int&nd,
      const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
      const int & gridIndexRange, const real & u, const real & d, 
      const int&mask, const int&boundaryCondition, const int&ipar, const real&rpar, int&ierr );
}


// fourth order dissipation 2D: (D+D-)^2 *********
#define FD4A_2D(u,i1,i2,i3,c) \
      (     ( u(i1-2,i2,i3,c)+u(i1+2,i2,i3,c)+u(i1,i2-2,i3,c)+u(i1,i2+2,i3,c) )   \
        -4.*( u(i1-1,i2,i3,c)+u(i1+1,i2,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2+1,i3,c) ) \
       +12.*u(i1,i2,i3,c) )

// fourth order dissipation 3D:
#define FD4A_3D(u,i1,i2,i3,c) \
      (     ( u(i1-2,i2,i3,c)+u(i1+2,i2,i3,c)+u(i1,i2-2,i3,c)+u(i1,i2+2,i3,c)+u(i1,i2,i3-2,c)+u(i1,i2,i3+2,c) )   \
        -4.*( u(i1-1,i2,i3,c)+u(i1+1,i2,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2+1,i3,c)+u(i1,i2,i3-1,c)+u(i1,i2,i3+1,c) ) \
       +18.*u(i1,i2,i3,c) )


// fourth order dissipation 2D: 
#define FD4V_2D(u,i1,i2,i3,c) \
      (     ax*( u(i1-2,i2,i3,c)+u(i1+2,i2,i3,c) )  \
           +ay*( u(i1,i2-2,i3,c)+u(i1,i2+2,i3,c) )   \
          -ax4*( u(i1-1,i2,i3,c)+u(i1+1,i2,i3,c) ) \
          -ay4*( u(i1,i2-1,i3,c)+u(i1,i2+1,i3,c) ) \
       + axy12*u(i1,i2,i3,c) )

// fourth order dissipation 3D:
#define FD4V_3D(u,i1,i2,i3,c) \
      (     ax*( u(i1-2,i2,i3,c)+u(i1+2,i2,i3,c) )\
           +ay*( u(i1,i2-2,i3,c)+u(i1,i2+2,i3,c) )\
           +az*( u(i1,i2,i3-2,c)+u(i1,i2,i3+2,c) ) \
          -ax4*( u(i1-1,i2,i3,c)+u(i1+1,i2,i3,c) ) \
          -ay4*( u(i1,i2-1,i3,c)+u(i1,i2+1,i3,c) ) \
          -az4*( u(i1,i2,i3-1,c)+u(i1,i2,i3+1,c) ) \
          +axyz18*u(i1,i2,i3,c) )

int GridFunctionFilter::debug=0;

// ==================================================================================
// Constructor for the grid function filter class.
// ==================================================================================
GridFunctionFilter::
GridFunctionFilter()
{
  filterType=explicitFilter;

  orderOfFilter=4;
  filterFrequency=1;
  numberOfFilterIterations=1;

  filterCoefficient=1.;
  numberOfFilterStages=1;
  
  numberOfFilterApplications=0;  // counts the total number of applications of the filter
}

// ==================================================================================
// Destructor for the grid function filter class.
// ==================================================================================
GridFunctionFilter::
~GridFunctionFilter()
{
}


// =====================================================================================
//  Apply the higher-order filter that requires two steps since the stencil is so wide.
//
//  NOTE: Add to v at the START of the step.
// =====================================================================================
int GridFunctionFilter::
applyFilter( realCompositeGridFunction & v,
             Range & C,
             realCompositeGridFunction & w // work space
           )
{
  const int myid=max(0,Communication_Manager::My_Process_Number);

  if( numberOfFilterApplications==0 )
    printF("applyFilter: orderOfFilter=%i, frequency=%i, numberOfFilterIterations=%i, filterCoefficient=%g\n",
            orderOfFilter,filterFrequency,numberOfFilterIterations,filterCoefficient);

  if( filterType==implicitFilter )
  {
    printF("GridFunctionFilter::applyFilter:ERROR: The implicit filter is not yet implemented\n");
    OV_ABORT("error");
  }

  //  real time0=getCPU();
  
  CompositeGrid & cg= *v.getCompositeGrid();
  const int numberOfDimensions = cg.numberOfDimensions();

  Index I1,I2,I3;

  realCompositeGridFunction & cgdiss = w;


  // Stage I
  //    D4 =  alpha*(D+xD-x)^2 + beta*(D+yD-y)^2
  //    D4^2 = alpha^2 (D+xD-x)^4 + 2*alpha*beta(D+xD-x)^2*(D+yD-y)^2 + beta^2 *(D+yD-y)^2
  //
  // We choose alpha^2 =1 for weak instabilities and alpha^2=1/dx for strong 


  IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
  const bool useOpt=true;

  for( int it=0; it<numberOfFilterIterations; it++ )
  {

    int option=0;  // Stage I 

    const int init = int(numberOfFilterApplications==0);
    int ipar[] = {option, C.getBase(),C.getBound(), orderOfFilter,debug,myid,init,numberOfFilterStages  };
    real ad=1.;   // changed below 
    real rpar[] = { ad }; // 
    int ierr=0;
  
    if( numberOfFilterStages>1 )
    {
      option=0;   // Stage I 
      ipar[0]=option;
    
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[grid];
  
	realMappedGridFunction & fieldCurrent = v[grid];
	realArray & u = fieldCurrent;
	realArray & d = cgdiss[grid];
	const intArray & mask = mg.mask();

#ifdef USE_PPP
	realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
	realSerialArray dLocal; getLocalArrayWithGhostBoundaries(d,dLocal);
	intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
#else
	realSerialArray & uLocal = u;
	realSerialArray & dLocal = d;
	const intSerialArray & maskLocal = mask;
#endif    
    
        // **************************************************************
        // fieldCurrent.updateGhostBoundaries();  
        // **************************************************************

	getIndex(mg.gridIndexRange(),I1,I2,I3);
	bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3);

	dLocal=0.;


	if( useOpt )
	{
	  ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( fieldCurrent,gidLocal,dimLocal,bcLocal );
	  if( ok )
	  {
	    cgFilter( numberOfDimensions,
		      uLocal.getBase(0),uLocal.getBound(0),
		      uLocal.getBase(1),uLocal.getBound(1),
		      uLocal.getBase(2),uLocal.getBound(2),
		      gidLocal(0,0),*uLocal.getDataPointer(),*dLocal.getDataPointer(),*maskLocal.getDataPointer(),
		      bcLocal(0,0), ipar[0],rpar[0],ierr );
	  }
	}
	else
	{
	  getIndex(mg.gridIndexRange(),I1,I2,I3);
	  where( mask(I1,I2,I3)>0 )
	  {
	    for( int n=C.getBase(); n<=C.getBound(); n++ )
	    {
	      if( numberOfDimensions==2 )
	      {
		d(I1,I2,I3,n)=FD4A_2D(u,I1,I2,I3,n);
		// d(I1,I2,I3,n)=FD4V_2D(u,I1,I2,I3,n);
	      }
	      else
	      {
		d(I1,I2,I3,n)=FD4A_3D(u,I1,I2,I3,n);
	      }
	    }
      
	  }
	}
    
	cgdiss[grid].periodicUpdate();  
	cgdiss[grid].updateGhostBoundaries();  
    
	// display(u,sPrintF("u on grid %i",grid),"%5.2f ");
	// display(cgdiss[grid],sPrintF("d on grid %i",grid),"%7.1e ");
	

      }
    
      // cgdiss.interpolate();
    }
  
     
    // -----------------------------------------------
    // ------------- STAGE II ------------------------
    // -----------------------------------------------

    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
  
      realMappedGridFunction & fieldCurrent = v[grid];
      realArray & u = fieldCurrent;

      realArray & d = cgdiss[grid];
      const intArray & mask = mg.mask();

#ifdef USE_PPP
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
      realSerialArray dLocal; getLocalArrayWithGhostBoundaries(d,dLocal);
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
#else
      realSerialArray & uLocal = u;
      realSerialArray & dLocal = d;
      const intSerialArray & maskLocal = mask;
#endif  

      getIndex(mg.gridIndexRange(),I1,I2,I3);
      bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3);

      const bool isRectangular=mg.isRectangular();

      // -------------------------------------------------------------------------
      // choose dissipation so that the mode (-1)^i * (-1)^j * (-1)^k  has a damping factor of zero: 
      // -------------------------------------------------------------------------

      if( orderOfFilter==8 )
      {
	if( numberOfFilterStages==2 )
	{
	  // Diss =[ (D+xD-x)^2 + (D+yD-y)^2 +(D+zD-z)^2 ]^2 
	  //  (D+D-)^2 (-1)^i = 16 * (-1)^i 
	  ad =  -filterCoefficient/SQR(16.*numberOfDimensions); 
	}
	else if( numberOfFilterStages==1 )
	{
          // Diss =[ (D+xD-x)^4 + (D+yD-y)^4 +(D+zD-z)^4 ]
          ad =  -filterCoefficient/( 4.*4.*4.*4.*numberOfDimensions );
	}
	else
	{
	  OV_ABORT("applyFilter:Error : numberOfFilterStages: finish me");
	}
	
      }
      else if( orderOfFilter==6 )
      {
        if( numberOfFilterStages==2 )
	{ // two stages: Order 4 followed by order 2 
   	  // Diss =[ (D+xD-x) + (D+yD-y) +(D+zD-z) ]*[ (D+xD-x)^2 + (D+yD-y)^2 +(D+zD-z)^2 ]
	  ad =  -filterCoefficient/( 4.*numberOfDimensions * 4.*4.*numberOfDimensions ); 
	}
	else if( numberOfFilterStages==1 )
	{
          // Diss =[ (D+xD-x)^3 + (D+yD-y)^3 +(D+zD-z)^3 ]
          ad =  -filterCoefficient/( 4.*4.*4.*numberOfDimensions );
	}
	else
	{
	  OV_ABORT("applyFilter:Error : numberOfFilterStages: finish me");
	}
      }
      else if( orderOfFilter==4 )
      {
	// Diss =[ (D+xD-x) + (D+yD-y) +(D+zD-z) ]^2 
	//  (D+D-) (-1)^i = 4 * (-1)^i 
        if( numberOfFilterStages==2 )
	{
	  ad =  -filterCoefficient/SQR(4.*numberOfDimensions); 
	}
	else if( numberOfFilterStages==1 )
	{
          // Diss =[ (D+xD-x)^2 + (D+yD-y)^2 +(D+zD-z)^2 ]
          ad =  -filterCoefficient/( 4.*4.*numberOfDimensions );
	}
	else
	{
	  OV_ABORT("applyFilter:Error : numberOfFilterStages: finish me");
	}
      }
      else
      {
	OV_ABORT("applyFilter:Error : orderOfFilter: finish me");
      }
    


      if( numberOfFilterApplications==0 )
	printF("applyFilter: it=%i : grid=%i ad=%8.2e \n",it,grid,ad );
    
      if( useOpt )
      {
	if( ok )
	{
	  ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( fieldCurrent,gidLocal,dimLocal,bcLocal );
	  option =1;  // stage II
	  ipar[0] = option;
	  rpar[0] = ad;  // coefficient of the dissipation
	  cgFilter( numberOfDimensions,
		    uLocal.getBase(0),uLocal.getBound(0),
		    uLocal.getBase(1),uLocal.getBound(1),
		    uLocal.getBase(2),uLocal.getBound(2),
		    gidLocal(0,0),*uLocal.getDataPointer(),*dLocal.getDataPointer(),*maskLocal.getDataPointer(),
		    bcLocal(0,0), ipar[0],rpar[0],ierr );
	  
	}
      }
      else
      {
	int extra=0;
	getIndex(mg.gridIndexRange(),I1,I2,I3,extra);

	where( mask(I1,I2,I3)>0 )
	{
	  for( int n=C.getBase(); n<=C.getBound(); n++ )
	  {
	    if( numberOfDimensions==2 )
	    {
	      if( orderOfFilter==8 )
	      {
		u(I1,I2,I3,n) +=  ad * FD4A_2D(d,I1,I2,I3,n);

		// implicit diagonal term:
		// u(I1,I2,I3,n) = (u(I1,I2,I3,n) + ad8 * FD4A_2D(d,I1,I2,I3,n) - ad8d*u(I1,I2,I3,n))/( 1.-ad8d );

		// u(I1,I2,I3,n) +=  ad8 * FD4V_2D(d,I1,I2,I3,n);
		// u(I1,I2,I3,n) +=  ad4 * d(I1,I2,I3,n);
	      }
	      else 
	      {
		OV_ABORT("applyFilter:Error : finish me");
	      }
	
	    }
	    else
	    {
	      if( orderOfFilter==8 )
	      {
		u(I1,I2,I3,n) +=  ad * FD4A_3D(d,I1,I2,I3,n);
	      }
	      else
	      {
		OV_ABORT("applyFilter:Error : finish me");
	      }
	 
	    }
	  }
	}
      }
    
      fieldCurrent.periodicUpdate();  
      fieldCurrent.updateGhostBoundaries();  
    
      // ::display(u," u ","%6.2f ");
    }

    // ********************************************************
    // if( numberOfFilterIterations>1 )
    // {
    //   v.interpolate();
    // }
    
  } // end for filter iterations

  numberOfFilterApplications++;
  // timing(timeForDissipation)+=getCPU()-time0;
}



int GridFunctionFilter::
update( GenericGraphicsInterface & gi )
{


  GUIState gui;

  DialogData & dialog=gui;

  dialog.setWindowTitle("GridFunctionFilter");
  dialog.setExitCommand("exit", "exit");

  dialog.setOptionMenuColumns(1);

  // ************** OPTION MENUS *****************

  aString filterTypeCommands[] = {"explicit filter", "implicit filter", "" };
  dialog.addOptionMenu("Type:", filterTypeCommands, filterTypeCommands, (int)filterType );


  // ************** PUSH BUTTONS *****************
//   aString pushButtonCommands[] = {"specify probes",
//                                   "show file options...",
// 				  ""};
//   int numRows=3;
//   dialog.setPushButtons(pushButtonCommands,  pushButtonCommands, numRows ); 


  // ************** TOGGLE BUTTONS *****************
//   aString tbCommands[] = {"plot errors",
//                           "plot divergence",
//  			  ""};
//   int tbState[15];
//   tbState[0] = plotErrors;
//   tbState[1] = plotDivergence;
//   int numColumns=2;
//   dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 



  // ----- Text strings ------
  const int numberOfTextStrings=30;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textCommands[nt] = "filter order";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i",orderOfFilter);  nt++; 

  textCommands[nt] = "filter frequency";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",filterFrequency); nt++; 

  textCommands[nt] = "filter iterations";
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",numberOfFilterIterations); nt++; 

  textCommands[nt] = "filter coefficient";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%f",filterCoefficient); nt++; 

  textCommands[nt] = "filter stages";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i",numberOfFilterStages);  nt++; 

  textCommands[nt] = "debug";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i",debug);  nt++; 


  // null strings terminal list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  gi.pushGUI(gui);
  aString answer,line;
  int len=0;
  for(;;) 
  {
    gi.getAnswer(answer,"");      
    // printF("Start: answer=[%s]\n",(const char*) answer);
    
    if( answer=="continue" || answer=="exit" )
    {
      break;
    }
    else if( answer=="explicit filter" || answer=="implicit filter" )
    {
      filterType = answer=="explicit filter" ? explicitFilter : implicitFilter;
    }
    else if( dialog.getTextValue(answer,"filter order","%i",orderOfFilter) ){}//
    else if( dialog.getTextValue(answer,"filter frequency","%i",filterFrequency) ){}//
    else if( dialog.getTextValue(answer,"filter iterations","%i",numberOfFilterIterations) ){}//
    else if( dialog.getTextValue(answer,"filter coefficient","%e",filterCoefficient) ){}//
    else if( dialog.getTextValue(answer,"debug","%i",debug) ){}//
    else if( dialog.getTextValue(answer,"filter stages","%i",numberOfFilterStages) ){}//
    else
    {
      printF("GridFunctionFilter::update: Unknown command = [%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }  
  }

  gi.popGUI();  // pop dialog

  return 0;
}
