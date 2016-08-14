#include "Ogen.h"
#include "Overture.h"
#include "GenericGraphicsInterface.h"
#include "display.h"

// ======================================================================================================
/// \brief Check that there are enough parallel ghost lines for ogen.
/// \details For implicit interpolation we need (iw-1)/2 ghost lines. For explicit interpolation
///    we need (iw+dw-2)/2 ghost lines. 
// ======================================================================================================
int Ogen::
checkParallelGhostWidth( CompositeGrid & cg )
{
#ifdef USE_PPP

  Range Rx=cg.numberOfDimensions(), G=cg.numberOfComponentGrids();
  Range all;
  const int iw = max(cg.interpolationWidth(Rx,all,all,all));

  int parallelGhost = max( 1, (iw-1)/2 );
  int dw=0;
  // for explicit interpolation we need more parallel ghost lines 
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    if( max(cg.interpolationIsImplicit(grid,G,0)) == 0 )
    { // explicit interpolation
      dw = max( dw, max( cg[grid].discretizationWidth()(Rx)) );
      parallelGhost = max( parallelGhost, (iw+dw-2)/2 );
    }
  }
  if( parallelGhost > MappedGrid::getMinimumNumberOfDistributedGhostLines() )
  {
    if( dw==0 )
    {
      printF("Ogen:ERROR: MappedGrid::minimumNumberOfDistributedGhostLines = %i is too small.\n"
	     " For implicit interpolation, ogen needs at least (iw-1)/2 = %i parallel ghost lines, \n"
	     " where iw=%i is the interpolation width.\n"
	     " If you are running Ogen interactively you can use the ogen command line option -numberOfParallelGhost=%i, \n"
             "    OR you can set `minimum number of distributed ghost lines' to %i from the top level ogen menu.\n"
             " If you are running cgins, add the command line option `-numberOfParallelGhost=%i'.\n"
             " If you are running another program you should call"
             " MappedGrid::setMinimumNumberOfDistributedGhostLines(numParallelGhost);"
             " to set the number of parallel ghost to the required number\n",
	     MappedGrid::getMinimumNumberOfDistributedGhostLines(),parallelGhost,iw,parallelGhost,parallelGhost,parallelGhost);
    }
    else
    {
      printF("Ogen:ERROR: MappedGrid::minimumNumberOfDistributedGhostLines = %i is too small.\n"
	     " For explicit interpolation, ogen needs at least (iw+dw-2)/2 = %i parallel ghost lines, \n"
	     " where iw=%i is the interpolation width and dw=%i is the discretization width.\n"
	     " If you are running Ogen interactively you can use the ogen command line option -numberOfParallelGhost=%i, \n"
             "    OR you can set `minimum number of distributed ghost lines' to %i from the top level ogen menu.\n"
             " If you are running cgins, add the command line option `-numberOfParallelGhost=%i'.\n"
             " If you are running another program you should call"
             " MappedGrid::setMinimumNumberOfDistributedGhostLines(numParallelGhost);"
             " to set the number of parallel ghost to the required number\n",
	     MappedGrid::getMinimumNumberOfDistributedGhostLines(),parallelGhost,iw,dw,parallelGhost,parallelGhost,parallelGhost);
    }
    OV_ABORT("ERROR: add more parallel ghostlines");
  }
#endif
  return 0;
}


// ======================================================================================================
/// \brief Output composite grid parameters such as discretization width, interpolation width etc.
// ======================================================================================================
int Ogen::
displayCompositeGridParameters( CompositeGrid & cg, FILE *file /* =stdout */ )
{
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    fPrintF(file,"grid %3i : name=%s \n",grid,(const char *)cg[grid].mapping().getName(Mapping::mappingName));

    fPrintF(file,"         : centering = %s \n",cg[0].isAllCellCentered() ? "cell centered" : "vertex centered");

    fPrintF(file,"         : interpolationType = (");
    int gridi;
    for( gridi=0; gridi<cg.numberOfComponentGrids(); gridi++ )
      fPrintF(file,"%s,",cg.interpolationIsImplicit(grid,gridi,0) ? "i" : "e");
    fPrintF(file,"), i=implicit, e=explicit\n");
		 
    fPrintF(file,"         : discretizationWidth = (");
    int axis;
    for( axis=0; axis<cg.numberOfDimensions(); axis++ )
      fPrintF(file,"%i,",cg[grid].discretizationWidth(axis));
    fPrintF(file,")\n");

    fPrintF(file,"         : interpolationWidth = (");
    for( gridi=0; gridi<cg.numberOfComponentGrids(); gridi++ )
      if( cg.numberOfDimensions()==2 )
	fPrintF(file,"[%i,%i],",cg.interpolationWidth(0,grid,gridi,0),cg.interpolationWidth(1,grid,gridi,0));
      else if( cg.numberOfDimensions()==3 )
	fPrintF(file,"[%i,%i,%i],",cg.interpolationWidth(0,grid,gridi,0),cg.interpolationWidth(1,grid,gridi,0),
	       cg.interpolationWidth(2,grid,gridi,0));
      else 
	fPrintF(file,"%i,",cg.interpolationWidth(0,grid,gridi,0));
    fPrintF(file,")\n");

    fPrintF(file,"         : may cut holes =(");
    for( gridi=0; gridi<cg.numberOfComponentGrids(); gridi++ )
      fPrintF(file,"%s,",cg.mayCutHoles(grid,gridi) ? "y" : "n");
    fPrintF(file,"), y=yes, n=no\n");

    fPrintF(file,"         : boundary condition   (side,axis) =(");
    for( axis=0; axis<cg.numberOfDimensions(); axis++ )
      fPrintF(file,"%i,%i, ",cg[grid].boundaryCondition(Start,axis),cg[grid].boundaryCondition(End,axis));
    fPrintF(file,")\n");

    fPrintF(file,"         : shared boundary flag (side,axis) =(");
    for( axis=0; axis<cg.numberOfDimensions(); axis++ )
      fPrintF(file,"%i,%i, ",cg[grid].sharedBoundaryFlag(Start,axis),cg[grid].sharedBoundaryFlag(End,axis));
    fPrintF(file,")\n");

    fPrintF(file,"         : shared boundary tolerance (side,axis) =(");
    for( axis=0; axis<cg.numberOfDimensions(); axis++ )
      fPrintF(file,"%e,%e, ",cg[grid].sharedBoundaryTolerance(Start,axis),cg[grid].sharedBoundaryTolerance(End,axis));
    fPrintF(file,")\n");

    fPrintF(file,"         : ghost points (side,axis) =(");
    for( axis=0; axis<cg.numberOfDimensions(); axis++ )
      fPrintF(file,"%i,%i, ",cg[grid].numberOfGhostPoints(Start,axis),cg[grid].numberOfGhostPoints(End,axis));
    fPrintF(file,")\n");
  }

  return 0;
}

//\begin{>changeParametersInclude.tex}{\subsubsection{Change Parameters descriptions}}
//\no function header:
// 
// Choosing the ``{\tt change parameters}'' menu option from the main Ogen menu will 
// allow one to make changes to the various parameters that affect the overlapping grid.
// 
// \begin{description}
//  \item[interpolation type] : There are two types of interpolation,
//     {\bf explicit} and {\bf implicit}.  {\bf Explicit} interpolation means
//     that a point that is interpolated will only use values on other grids
//     that are not interpolation points themselves.  This means that will
//     the default 3 point interpolation the amount of overlap must be at
//     least $1.5$ grid cells wide. With explicit interpolation the
//     interpolation equations can be solved explicitly (and this faster).
//     
//     With {\bf implicit} interpolation the points used in the interpolation
//     stencil may themselves be interpolation points. This means  
//     that will the default 3 point interpolation the amount of overlap must be at least
//     $.5$ grid cells wide. Thus {\bf implicit interpolation is more likely to give a valid grid} since
//     it requires less overlap. With implicit interpolation the interpolation equations are a coupled
//     system that must be solved. This is a bit slower but the Overture interpolation function handles
//     this automatically.
//  \item[ghost points] : You can increase the number of ghost points on each grid. Some solvers require
//      more ghost points. This will have no effect on the overlapping grid.
//  \item[cell centering] : The grid can be made {\bf cell centered} in which case cell centers are
//      interpolated from other cell centers. By default the grid is {\bf vertex centered} whereby vertices
//      are interpolated from vertices.
//  \item[maximize overlap] Maximize the overlap between grids. The default is to minimize the overlap.
//  \item[minimize overlap] : minimize the overlap between grids. This is the default.
//  \item[minimum overlap] : specify the minimum allowable overlap between grids. By default this is .5
//       times a grid spacing. 
//  \item[mixed boundary] : define a boundary that is partly a physical boundary and partly an interpolation
//     boundary. use this option to define a 'c-grid' or an 'h-grid'.
//  \item[interpolate ghost] : interpolate ghost points on interpolation boundaries.
//  \item[do not interpolate ghost] : interpolate points on the boundary of interpolation boundaries.
//  \item[prevent hole cutting] : By default, the overlapping grid
//      generator will use any physical boundary (a side of a grid with a
//      positive {\tt boundaryCondition} to try and cut holes in any other
//      grid that lies near the physical boundary. Thus in the ``cylinder in a
//      channel example'' section (\ref{sec:cylinderInAChannel}) the inner
//      boundary of the annulus cuts a hole in the rectangular grid.
//      Sometimes, as in the ``inlet outlet'' example, section
//      (\ref{sec:inletOutlet}), one does not want this to happen. In this
//      case it is necessary to explicitly specify which grids are allowed to
//      cut holes in which other grids. 
//  \item[allow hole cutting] : specify which grids can have holes cut by a given grid.
//  \item[manual hole cutting] : specify a block of points to be cut as a hole. Usually used with
//      phantom hole cutting as described next.
//  \item[phantom hole cutting] : a boundary can be specified to be a phantom hole cutter. Use this 
//    option together with manual hole cutting. A phantom hole cutting boundary will proceed as if cutting
//    a hole but it will only mark the interpolation points at the hole boundary and not cut any holes.
//  \item[prevent interpolation] : prevent interpolation between grids. By default all grids may interpolate
//      from all others. 
//  \item[allow interpolation] : allow interpolation between grids.  By default all grids may interpolate
//      from all others. 
//  \item[allow holes to be cut]: specify which grids cut holes in a given grid.
// %\item[shared boundary tolerance] : The shared boundary flag (\ref{sec:share})
// %      is used to indicate when two different component grids share a common boundary. This allows
// %      a boundary point on one grid to interpolate from the boundary of the other grid
// %      even if the point is slightly outside the other grid. The {\bf shared boundary tolerance}
// %      is a relative measure of much outside the boundary a grid point is allowed to be.
// %      By default the value is $.1$ (unit square cooridnates) 
// %      which means that a point is allowed to deviate by $.1$ times
// %      the width in the normal direction of the boundary grid.
//  \item[shared boundary tolerances] : Specify the tolerances which determine when points interpolate
//    on shared boundaries. 
//  \item[specify shared boundaries] : explicitly specify where a portion of one boundary should
//       share a boundary with the side of another grid.
//  \item[maximum distance for hole cutting]: specify the maximum distance from a given face on a given
//         grid  from which holes can be cut (ony applies to physical boundaries). By default this
//       distance is $\infty$. You may have to specify this value for thin objects such as the sail
//       example to prevent hole points from being cut too far from the sail surface.
//  \item[non-cutting boundary points] : specify parts of physical boundaries that should not cut holes.
//            This option should be rarely used.
// %  \item[non-conforming] :
//  \item[order of accuracy] : Choose an order of accuracy, 2nd-order or fourth-order. This
//     option will then assign that {\tt interpolationWidth} and {\tt discretizationWidth}
//     to be $3$ for 2nd-order or $5$ for fourth-order. You can also explicitly change
//     the {\tt interpolationWidth} and {\tt discretizationWidth} instead of using this option.
//  \item[interpolation width] : By default the interpolation width is 3 which means that the
//     interpolation stencil is 3 points wide in each direction. The interpolation width may
//     be changed to any integer greater than or equal to 1.
//  \item[discretization width] : The discretization width is by default 3 and defines the width
//     of the expected discretization stencil used by a solver. The discretization width can
//     be an odd integer greater than or equal to 3. A fourth-order accurate solver may require
//     that the discretization width be incraeased to 5. In this case 2 lines of interpolation
//     points will be required.
//  \item[boundary discretization width] : The one-side discretization width used at a boundary
//      is by default 3. This means that a discretization point on the boundary will have 2 valid
//      interior points next to it in the normal direction.
//  \item[shared sides may cut holes] : cg.sharedSidesMayCutHoles(g1,g2)=false by default. 
//     Normally a physical boundary on grid g1 
//     with sharedBoundaryFlag=share1 will not cut holes in grid g2 if g2 has a sharedBoundaryFlag
//     equal to share1 (on any of it sides). In some cases (such as the end.cmd example) this
//     option should be set to true to allow a shared side to cut holes in places where the
//     boundaries are not the same.
//  \item[specify a domain] assign grids to a separate domain
//  \item[reset domains] assign all grids back to domain 0
//  \item[show parameter values] : The current parameter values will be printed.
// \end{description}
//\end{changeParametersInclude.tex}


// ===================================================================================
/// \brief Change Ogen parameters.
/// \pMapInfo (input) : pointer to the list of mappings (used to define explicit hole cutters)
// ===================================================================================
int Ogen::
changeParameters( CompositeGrid & cg, MappingInformation *pMapInfo /* =NULL */ )
{
  assert( ps!=NULL );
  
//   if( false )
//   {
//     ::display(cg.interpolationIsImplicit,"cg.interpolationIsImplicit BEFORE change parameters");
//     ::display(cg.interpolationWidth,"cg.interpolationWidth BEFORE change parameters");
//     ::display(cg.interpolationOverlap,"cg.interpolationOverlap BEFORE change parameters");
//   }


  const int numberOfBaseGrids = cg.numberOfBaseGrids();
  const int myid=max(0,Communication_Manager::My_Process_Number);
  
  int & qualityAlgorithm = dbase.get<int>("qualityAlgorithm");

  GenericGraphicsInterface & gi = *ps;
  gi.appendToTheDefaultPrompt("change parameters>");

  GUIState dialog;
  dialog.setWindowTitle("Change Ogen parameters");
  dialog.setExitCommand("exit", "exit");

  aString cmds[] = {"order of accuracy",
                    "interpolation type",
                    "ghost points",
                    "interpolation width",
                    "discretization width",
                    "create explicit hole cutter",
                    // "define explicit hole cutters",
		    ""};

  int numberOfPushButtons=6;  // number of entries in cmds
  int numRows=(numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 


  aString tbCommands[] = {"improve quality of interpolation",
			  ""};
  int tbState[10];
  tbState[0] = improveQualityOfInterpolation;
  

  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

  const int numberOfTextStrings=5;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "improve quality algorithm:";  sPrintF(textStrings[nt],"%i [0=old,1=new]",qualityAlgorithm);  nt++; 


  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);


  aString menu[] = 
    {
      "!change parameters",
      ">hole cutting options",
        "allow hole cutting",
        "allow holes to be cut",
        "prevent hole cutting",
        "manual hole cutting",
        "define explicit hole cutters",
        "phantom hole cutting",
        "allow interpolation",
        "prevent interpolation",
        "maximum distance for hole cutting",
        "non-cutting boundary points",
//        "pre-interpolate",
        "use new hole cutting algorithm",
        "use old hole cutting algorithm",
        "compute for overlapping grid",
        "compute for hybrid mesh",
        "use local bounding boxes",
        "do not use local bounding boxes",
      "<cell centering",
      "discretization width",
      "boundary discretization width",
      "ghost points",
      "interpolate ghost",
      "do not interpolate ghost",
      "interpolation type",
      "interpolation width",
      "set quality bound",
      "improve quality of interpolation",
      ">domains",
        "specify a domain",
        "reset domains",
      "<maximize overlap",
      "minimize overlap",
      "minimum overlap",
//      "non-conforming",
      ">mixed boundary",
        "mixed boundary",
//        "c-grid",
//        "h-grid",
      "<order of accuracy",
      "boundary conditions",
      ">share options",
        "shared boundary tolerances",
        "default shared boundary normal tolerance",
        "shared boundary flag",
        "shared sides may cut holes",
        "specify shared boundaries",
      "<show parameter values",
      "use backup rules if necessary",
      "do not use backup rules",
      "useBoundaryAdjustment",
//      "load balance",
      "maximum number of points to invert at a time",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "allow hole cutting   : specify which grids can be cut by a given grid",
      "allow holes to be cut: specify which grids cut holes in a given grid",
      "compute for overlapping grid : compute overlap and interpolation stencils for overlapping grids",
      "compute for hybrid mesh : cut holes for hybrid meshing",
      "manual hole cutting : specify a block of points to cut as a hole",
      "maximum distance for hole cutting: ",
      "cell centering       : change the cell centering of the grid",
      "discretization width : set the width of the discretiation stencil",
      "boundary discretization width : width og one-sided stencil at boundaries",
      "ghost points         : change the number of ghost points",
      "interpolate ghost    :  interpolate ghost points on interpolation boundaries",
      "interpolation type   : implicit interpolation requires less overlap than explicit",
      "interpolation width  : set the width of the interpolation stencil",
      "improve quality of interpolation: ",
      "domains             : domains are independent regions where one might solve different equations",
      "specify a domain    : assign grids to a separate domain",
      "reset domains       : assign all grids back to domain 0",
      "set quality bound   : set quality bound for interpolation. (ratio od cell sizes)",
      "specify shared boundaries : explicitly specify shared boundaries",
      "maximize overlap     : maximize the overlap between grids",
      "minimize overlap     : minimize the overlap between grids",
      "minimum overlap      : specify minimum allowable overlap" ,
//      "non-conforming       : specify that a grid is nonconforming",
      "mixed boundary       : specify a boundary that is partly physical and partly interpolation",
      "non-cutting boundary points : specify parts of boundaries that should not cut holes",
      "order of accuracy    : set parameters for 2nd or 4th order accuracy",
      "phantom hole cutting : mark interpolation points near the hole boundary but do not cut the holes",
      "prevent hole cutting : by default all physical boundaries (boundaryCondition>0) cut holes",
      "prevent interpolation: prevent interpolation of one grid from another grid",
      "boundary conditions  : change boundary conditions on grids",
      "shared boundary tolerances: set tolerances for matching between shared boundaries",
      "shared boundary flag : change the shared boundary flag for some grids",
      "shared sides may cut holes : by default shared sides do not cut holes in each other",
      "show parameter values: display current values for parameters",
      "maximum number of points to invert at a time : reducing this can save memory in the inverse routines",
      "help                 : Print this list",
      "exit                 : Finished with changes",
      "" 
    };


  dialog.buildPopup(menu);

  dialog.addInfoLabel("Choose more options from the popup menu.");


  gi.pushGUI(dialog);


  aString *gridMenu = new aString [cg.numberOfComponentGrids()+4];
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    gridMenu[grid]=cg[grid].mapping().getName(Mapping::mappingName);
  }
  gridMenu[cg.numberOfComponentGrids()]="all";
  gridMenu[cg.numberOfComponentGrids()+1]="none";
  gridMenu[cg.numberOfComponentGrids()+2]="done";
  gridMenu[cg.numberOfComponentGrids()+3]="";

  aString *gridMenuNoAll = new aString [cg.numberOfComponentGrids()+3];
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    gridMenuNoAll[grid]=cg[grid].mapping().getName(Mapping::mappingName);
  }
  gridMenuNoAll[cg.numberOfComponentGrids()  ]="none";
  gridMenuNoAll[cg.numberOfComponentGrids()+1]="done";
  gridMenuNoAll[cg.numberOfComponentGrids()+2]="";
  
  Range L(0,max(0,cg.numberOfMultigridLevels()-1));

  RealArray minimumOverlap(Range(3),cg.numberOfComponentGrids(),cg.numberOfComponentGrids(),L);
  minimumOverlap=.5;

  bool domainsHaveBeenAssigned=false; // set to true when a domain has been assigned

  int dir;
  aString answer,answer2,answer3;
  Range all;
  for(int it=0;; it++)
  {
    // gi.getMenuItem(menu,answer,"choose an option");
    gi.getAnswer(answer,"");
    
    if( answer=="interpolation type" )
    {
      aString menu[]=
      { 
	"implicit for all grids",
	"explicit for all grids",
	"set implicit for some grids",
	"set explicit for some grids",
        "no change",
	"" 
      };
      gi.appendToTheDefaultPrompt("interpolation>");
      gi.getMenuItem(menu,answer2,"Choose an option");
      
      if( answer2=="no change" )
	continue;
      else if( answer2=="implicit for all grids" )
        cg.interpolationIsImplicit(all,all,all)=true;
      else if( answer2=="explicit for all grids" )
        cg.interpolationIsImplicit(all,all,all)=false;
      else
      {
        // *wdh* 110509 - fixed for setting some explicit
        bool setImplicit=true;
	if( answer2=="set implicit for some grids" )
	  setImplicit=true;
	else if( answer2=="set explicit for some grids" )
          setImplicit=false;
	else
	{
	  printF("Ogen:changeParameters: un-expected answer=[%s]\n",(const char*)answer2);
	  OV_ABORT("error");
	}
	

	int interp,interpolee;
        for( ;; )
	{
          // "change the interpolation properties of which grid?"
	  interp = gi.getMenuItem(gridMenu,answer2,"change the way a grid interpolates from others"); 
	  if( interp<0 )
	  {
  	    printF("Unknown response=%s\n",(const char*)answer2);
            gi.stopReadingCommandFile();
	    break;
	  }
          if( answer2=="done" || answer=="none" )
            break;
         
	  if( answer2=="set implicit for some grids" )
	    interpolee = gi.getMenuItem(gridMenu,answer3,"interpolate implicitly from which grid(s)?"); 
	  else
	    interpolee = gi.getMenuItem(gridMenu,answer3,"interpolate explicitly from which grid(s)?"); 

	  if( interpolee<0 )
	  {
  	    printF("Unknown response=%s\n",(const char*)answer2);
            gi.stopReadingCommandFile();
	    break;
	  }
          if( answer2=="done" )
            break;

	  Range Rinterp, Rinterpolee;
	  if( interp<cg.numberOfComponentGrids() )
	    Rinterp=Range(interp,interp);
	  else 
	    Rinterp=Range(0,cg.numberOfComponentGrids()-1);   // change all  interp grids
	  
	  if( interpolee<cg.numberOfComponentGrids() )
	    Rinterpolee=Range(interpolee,interpolee);
	  else 
	    Rinterpolee=Range(0,cg.numberOfComponentGrids()-1);  // change all interpolee grids
	  
	  bool interpChoice=  answer3=="none" ? false : true;
          printF("interpChoice=%i (1) answer2=[%s], answer3=[%s]\n",interpChoice,(const char*)answer2,(const char*)answer3);
	  
	  if( !setImplicit )
	  { // set some explicit - flip choice:
	    interpChoice = !interpChoice;
	  }
	  
	  cg.interpolationIsImplicit(Rinterp,Rinterpolee,all)=interpChoice;

          display(cg.interpolationIsImplicit,"cg.interpolationIsImplicit");

	}
      }
      // display(cg.interpolationIsImplicit,"cg.interpolationIsImplicit");
      gi.unAppendTheDefaultPrompt();

    }
    else if( answer=="use new hole cutting algorithm" )
    {
      holeCuttingOption= 1;
      printF("Use new hole cutting algorithm\n");
    }
    else if( answer=="use old hole cutting algorithm" )
    {
      holeCuttingOption=0;
      printF("Use old hole cutting algorithm\n");
    }
    else if( answer=="compute for overlapping grid" )
    {
      classifyHolesForHybrid = false;
      printF("Compute an overlapping grid\n");
    }
    else if( answer=="compute for hybrid mesh" )
    {
      classifyHolesForHybrid = true;
      printF("Compute holes for a hybrid mesh\n");
    }
    else if( answer=="use backup rules if necessary" )
    {
      allowBackupRules=true;
      printF("Allow the use of backup rules\n");
    }
    else if( answer=="do not use backup rules" )
    {
      allowBackupRules=false;
      printF("Do not allow the use of backup rules\n");
    }
    else if( answer=="use local bounding boxes" )
    {
      useLocalBoundingBoxes=true;
      printF("use local bounding boxes\n");
    }
    else if( answer=="do not use local bounding boxes" )
    {
      useLocalBoundingBoxes=false;
      printF("Do not use local bounding boxes\n");
    }
    else if( answer=="useBoundaryAdjustment" )
    {
      useBoundaryAdjustment=!useBoundaryAdjustment;
      printF("useBoundaryAdjustment=%i\n",(int)useBoundaryAdjustment);
    }
    else if( answer=="order of accuracy" )
    {
      aString orderMenu[] = 
      {
	"second order",
        "fourth order",
        "sixth order",
        "eighth order",
        "tenth order",
        "twelfth order",
        "fourteenth order",
        "sixteenth order",
        "eighteenth order",
        ""
      };
      int order = gi.getMenuItem(orderMenu,answer2,"choose an order of accuracy");
      if( order<0 )
      {
	printF("Unknown response=%s\n",(const char*)answer2);
	gi.stopReadingCommandFile();
	break;
      }     
      const int width = 2*(order+1)+1; // ==0 ? 3 : order==1 ? 5 : order==2 : 7 : 9;
      const int numGhost = (width-1)/2;
      printF("Setting interpolationWidth=%i, discretizationWidth=%i, boundary disc. width=%i, number of ghost lines=%i\n",
	     width,width,width,numGhost);
      
      Range Rx(0,cg.numberOfDimensions()-1);
      cg.interpolationWidth(Rx,all,all,all)=width;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	cg[grid].discretizationWidth()(Rx)=width;
        cg[grid].numberOfGhostPoints()(Range(0,1),Range(0,2))=max(cg[grid].numberOfGhostPoints(),numGhost);

        // *wdh* 2012/05/08 -- this was missing!
	for( int dir=0; dir<cg.numberOfDimensions(); dir++ )for( int side=0; side<=1; side++ )
	{
	  cg[grid].setBoundaryDiscretizationWidth(side,dir, width);
	}
      }
    }
    else if( answer=="prevent hole cutting" || answer=="prevent interpolation" || 
             answer=="allow interpolation" || answer=="phantom hole cutting" )
    {
      enum
      {
	preventHoleCutting,
	preventInterpolation,
	allowInterpolation,
        phantomHoleCutting
      } option; 
      
      aString defaultPrompt,prompt,prompt2;
      if( answer=="prevent hole cutting" )
      {
        option=preventHoleCutting;
	defaultPrompt="prevent hole cut>";
        prompt="change cutting characteristics for which grid(s)?";
	prompt2="do NOT cut holes in which grid(s)?";
      }
      else if( answer=="prevent interpolation" )
      {
        option=preventInterpolation;
	defaultPrompt="prevent interpolation>";
        prompt="change interpolation characteristics for which grid(s)?";
	prompt2="do NOT interpolate from which grid(s)?";
      }
      else if( answer=="allow interpolation" )
      {
        option=allowInterpolation;
	defaultPrompt="allow interpolation>";
        prompt="change interpolation characteristics for which grid(s)?";
	prompt2="allow interpolation from which grid(s)?";
      }
      else
      {
        option=phantomHoleCutting;
	defaultPrompt="phantom hole cutting>";
        prompt="change cutting characteristics for which grid(s)?";
	prompt2="allow phantom hole cutting on which grids(s)?";
      }
      
      
      gi.appendToTheDefaultPrompt((const char*)defaultPrompt);
      for( ;; )
      {
	int cutter = gi.getMenuItem(gridMenu,answer2,prompt);
        if( cutter<0 )
	{
	  printF("Unknown response=%s\n",(const char*)answer2);
          gi.stopReadingCommandFile();
	  break;
	}
	if( answer2!="none" && answer2!="done" )
	{
	  int cuttee = gi.getMenuItem(gridMenu,answer3,prompt2);
          if( cuttee <0 )
	  {
	    printF("Unknown response=%s\n",(const char*)answer2);
            gi.stopReadingCommandFile();
            continue;
	  }

	  Range Rcut, Rcuttee;
	  if( cutter<cg.numberOfComponentGrids() )
	    Rcut=Range(cutter,cutter);
	  else 
	    Rcut=Range(0,cg.numberOfComponentGrids()-1);   // change all  cutter grids
	  
	  if( cuttee<cg.numberOfComponentGrids() )
	    Rcuttee=Range(cuttee,cuttee);
	  else 
	    Rcuttee=Range(0,cg.numberOfComponentGrids()-1);  // change all cuttee grids
	  
	  bool cuttingChoice =  answer3=="none" ? true : false;
	  if( option==preventHoleCutting )
	  {
	    cg.mayCutHoles(Rcut,Rcuttee)=cuttingChoice;
	    if( debug & 4 )
	      display(cg.mayCutHoles,"cg.mayCutHoles");
	  }
	  else if( option==preventInterpolation )
	  {
	    cg.mayInterpolate(Rcut,Rcuttee,all)=cuttingChoice;
	  }
	  else if( option== allowInterpolation )
	  {
	    cg.mayInterpolate(Rcut,Rcuttee,all)=!cuttingChoice;
	  }
	  else
	  {
	    cg.mayCutHoles(Rcut,Rcuttee)=2;  // this means phantom cutting.
	  }
	  
	}
        else
          break;
      }
      gi.unAppendTheDefaultPrompt();
    }
    else if( answer=="allow hole cutting" || answer=="allow holes to be cut" )
    {
      printF("By default all physical boundaries (bc>0) cut holes in all other grids\n");
      const bool allowHoleCutting= answer=="allow hole cutting";
      if( allowHoleCutting )
	printF("Specify which grids can have holes cut with a given grid\n");
      else
	printF("Specify which grids can cut holes in a given grid\n");
      
      
      gi.appendToTheDefaultPrompt("allow hole cut>");
      for( ;; )
      {
	int cutter=0, cuttee=0;
        if( allowHoleCutting )
          cutter = gi.getMenuItem(gridMenu,answer2,"Specify cutting characteristics for which grid(s)?");
        else
          cuttee = gi.getMenuItem(gridMenu,answer2,"allow holes to be cut in which grid(s)?");
          
        if( cutter<0 || cuttee<0 )
	{
	  printF("Unknown response=%s\n",(const char*)answer2);
          gi.stopReadingCommandFile();
	  break;
	}
	if( answer2!="none" && answer2!="done" )
	{
          if( allowHoleCutting )
            cuttee = gi.getMenuItem(gridMenu,answer3,"cut holes in which grid(s)?");
          else
            cutter = gi.getMenuItem(gridMenu,answer3,"cut holes with which grid(s)?");
	  
          if( cutter<0 || cuttee<0 )
	  {
	    printF("Unknown response=%s\n",(const char*)answer2);
            gi.stopReadingCommandFile();
            continue;
	  }
	  
	  Range Rcut, Rcuttee;
	  if( cutter<cg.numberOfComponentGrids() )
	    Rcut=Range(cutter,cutter);
	  else 
	    Rcut=Range(0,cg.numberOfComponentGrids()-1);   // change all  cutter grids
	  
	  if( cuttee<cg.numberOfComponentGrids() )
	    Rcuttee=Range(cuttee,cuttee);
	  else 
	    Rcuttee=Range(0,cg.numberOfComponentGrids()-1);  // change all cuttee grids
	  
	  bool cuttingChoice =  answer3=="none" ? false : true;
	  cg.mayCutHoles(Rcut,Rcuttee)=cuttingChoice;

	  if( debug & 4 ) 
            display(cg.mayCutHoles,"cg.mayCutHoles");

	}
        else
          break;
      }
      gi.unAppendTheDefaultPrompt();
    }
/* ---
    else if( answer=="non-conforming" )
    {
      gi.outputString("A non-conforming grid intersects the domain in a non-smooth manner");
      gi.outputString("This is called the <poor man's> intersection in the documentation");
      
      gi.appendToTheDefaultPrompt("non-conforming>");
      for( ;; )
      {
	int grid = gi.getMenuItem(gridMenu,answer2,"specify which grid is non-conforming");
	if( grid<0 )
	{
	  cout << "Unknown response=" << answer2 << endl;
	  gi.stopReadingCommandFile();
          break;
	}
	else if( answer2=="none" || answer2=="done" )
	{
	  break;
	}
	else 
	{
	  Range R;
	  if( grid<cg.numberOfComponentGrids() )
	    R=Range(grid,grid);
	  else 
	    R=Range(0,cg.numberOfComponentGrids()-1); 
	  cg.mayCutHoles(R,R)=100; // kludge *********
          if( debug & 1 )
  	    display(cg.mayCutHoles,"cg.mayCutHoles");
	}
      }
      gi.unAppendTheDefaultPrompt();
    }
---- */
    else if( answer=="manual hole cutting" )
    {
      gi.outputString("Specify a block of points [i1a,i1b]x[i2a,i2b]x[i3a,i3b] to mark as a hole in `grid'");

      gi.appendToTheDefaultPrompt("manual hole cutting");
      for( ;; )
      {
	int grid = gi.getMenuItem(gridMenuNoAll,answer2,"Manually cut holes in which grid?");
        if( grid<0 )
	{
	  printF("Unknown response=%s\n",(const char*)answer2);
          gi.stopReadingCommandFile();
	  break;
	}
	if( answer2=="none" || answer2=="done" )
	{
          break;
	}
	else
	{
	  if( grid<0 || grid>= cg.numberOfComponentGrids() )
	  {
	    printF("ERROR: Invalid value for grid=%i\n",grid);
	    gi.stopReadingCommandFile();
	    continue;
	  }
	  MappedGrid & c = cg[grid];
	  printF(" grid %s : array dimensions [%i,%i]x[%i,%i]x[%i,%i]\n"
		 " grid %s : gridIndexRange   [%i,%i]x[%i,%i]x[%i,%i]\n",
		 (const char*)c.getName(),c.dimension(0,0),c.dimension(1,0),
		 c.dimension(0,1),c.dimension(1,1),c.dimension(0,2),c.dimension(1,2),(const char*)c.getName(),
		 c.gridIndexRange(0,0),c.gridIndexRange(1,0),
		 c.gridIndexRange(0,1),c.gridIndexRange(1,1),c.gridIndexRange(0,2),c.gridIndexRange(1,2));

	  gi.inputString(answer2,"Enter i1a,i1b,i2a,i2b,i3a,i3b\n");
	  int i1a=0,i1b=0,i2a=0,i2b=0,i3a=0,i3b=0;
	  sScanF(answer2,"%i %i %i %i %i %i",&i1a,&i1b,&i2a,&i2b,&i3a,&i3b);
            
      
	  if( i1a>i1b || i1a< c.dimension(0,0) || i1b>c.dimension(1,0) ||
	      i2a>i2b || i2a< c.dimension(0,1) || i2b>c.dimension(1,1) ||
	      i3a>i3b || i3a< c.dimension(0,2) || i3b>c.dimension(1,2) )
	  {
	    printF(" ERROR: Invalid values [i1a,i1b]x[i2a,i2b]x[i3a,i3b]=[%i,%i]x[%i,%i]x[%i,%i]\n"
		   "        grid %s has dimensions [%i,%i]x[%i,%i]x[%i,%i]\n",
		   i1a,i1b,i2a,i2b,i3a,i3b, (const char*)c.getName(),c.dimension(0,0),c.dimension(1,0),
		   c.dimension(0,1),c.dimension(1,1),c.dimension(0,2),c.dimension(1,2));
	
	    gi.stopReadingCommandFile();
	    continue;
	  }
      
	  if( manualHole.getLength(0) <= numberOfManualHoles )
	  {
	    manualHole.resize(numberOfManualHoles+5,7);
	  }
	  manualHole(numberOfManualHoles,0)=grid;
	  manualHole(numberOfManualHoles,1)=i1a;
	  manualHole(numberOfManualHoles,2)=i1b;
	  manualHole(numberOfManualHoles,3)=i2a;
	  manualHole(numberOfManualHoles,4)=i2b;
	  manualHole(numberOfManualHoles,5)=i3a;
	  manualHole(numberOfManualHoles,6)=i3b;
      

	  numberOfManualHoles++;

	}
      }
      gi.unAppendTheDefaultPrompt();
      
    }
    else if( answer=="create explicit hole cutter" ) // ***NEW WAY***
    {
      // -- create a new hole cutter --- 
      explicitHoleCutter.push_back(ExplicitHoleCutter());
      ExplicitHoleCutter & holeCutter = explicitHoleCutter[explicitHoleCutter.size()-1];
      holeCutter.mayCutHoles.redim(cg.numberOfComponentGrids());
      holeCutter.mayCutHoles=true;  // by default hole cutters cut holes in all grids

      holeCutter.update( gi,*pMapInfo,cg );
    }
    else if( answer=="define explicit hole cutters" ) // ***OLD WAY***
    {
      printF("Ogen:INFO: define explicit hole cutters."
             " These are mappings that define regions where all points should be marked at holes.\n"
             " Explicit hole cutters are used in the rare cases when the default implicit hole cutting fails.\n"
             " Explicit hole cutting may be needed if part of a boundary grid lies outside the final domain.\n"
             " For example, a square region with half an annulus on the lower boundary with a square refinement grid\n"
             " over the annulus (see halfAnnulusRefinedGrid.cmd).\n");

      if( pMapInfo == NULL )
      {
	printF("ERROR: mapInfo was not passed to change parameters!\n");
	continue;
      }
      MappingInformation & mapInfo = *pMapInfo;
      

      int num=mapInfo.mappingList.getLength();

      const int maxMenuItems=num+7;
      aString *mapMenu = new aString[maxMenuItems];
      int mappingListStart=0, mappingListEnd=num-1;
      for( int i=0; i<num; i++ )
	mapMenu[i]=mapInfo.mappingList[i].getName(Mapping::mappingName);
      // add extra menu items
      int extra=num;
      mapMenu[extra++]="done";
      mapMenu[extra++]="";   // null string terminates the menu
      assert( extra<= maxMenuItems );

      // replace menu with a new cascading menu if there are too many items. (see viewMappings.C)
      gi.buildCascadingMenu( mapMenu,mappingListStart,mappingListEnd );
      gi.appendToTheDefaultPrompt("explicit hole cutters");
      for( ;; )
      {
	gi.getMenuItem(mapMenu,answer2,"Cut holes with which mapping?");
	int map=-1;
	for( int j=0; j<num; j++ )
	{
	  if( answer2==mapInfo.mappingList[j].getName(Mapping::mappingName) )
	  {
	    map=j;
	    break;
	  }
	}
	if( answer2=="done" || answer2=="exit" )
	{
	  break;
	}
	else if( map>=0 )
	{
	  MappingRC & mapping = mapInfo.mappingList[map];
	  printF("Adding mapping=[%s] as an explicit hole cutter.\n",
		   (const char*)mapping.getName(Mapping::mappingName));

          // -- create a new hole cutter --- 
          explicitHoleCutter.push_back(ExplicitHoleCutter());
          ExplicitHoleCutter & holeCutter = explicitHoleCutter[explicitHoleCutter.size()-1];
          // -- This hole cutter usings "mapping" : 
	  holeCutter.holeCutterMapping.reference(mapping);
          holeCutter.mayCutHoles.redim(cg.numberOfComponentGrids());
	  holeCutter.mayCutHoles=true;  // by default hole cutters cut holes in all grids

	}
	else
	{
	  printF("Unknown response=%s.\n",(const char*)answer2);
	  gi.stopReadingCommandFile();
	}
      }

      delete [] mapMenu;
      gi.unAppendTheDefaultPrompt();
    }

    else if( answer=="pre-interpolate" )
    {
//     **** This option not currently needed ***** 

//  \item[pre-interpolate] : If grid2 is basically a refinement of grid1 then one may pre-interpolate
//      grid1 from grid2. This may be necessary if a small feature would otherwise not cut holes properly
//      in grid1 (but does cut holes properly in grid2).
      gi.appendToTheDefaultPrompt("pre-interpolate>");
      for( ;; )
      {
	int grid1,grid2;
        grid1 = gi.getMenuItem(gridMenuNoAll,answer2,"pre-interpolate which grid?");
        if( answer2=="done" || answer2=="none" )
	{
	  break;
	}
	else if( grid1>=0 && grid1<cg.numberOfComponentGrids() )
	{
          grid2 = gi.getMenuItem(gridMenuNoAll,answer2,"...from which grid?");
          if( grid2>=0 && grid2<cg.numberOfComponentGrids() )
	  {
	    const int n=preInterpolate.getLength(0);
	    preInterpolate.resize(n+1,2);
	    preInterpolate(n,0)=grid1;
	    preInterpolate(n,1)=grid2;
	  }
	  else
	  {
	    printF("Invalid response: %s\n",(const char*)answer2);
            gi.stopReadingCommandFile();
	  }
	}
        else
	{
	  printF("Invalid response: %s\n",(const char*)answer2);
          gi.stopReadingCommandFile();
	}
      }
      gi.unAppendTheDefaultPrompt();
    }
    else if( answer=="maximize overlap" )
    {
      minimizeTheOverlap=false;
      printF("Overlap will be maximized\n");
    }
    else if( answer=="minimize overlap" )
    {
      minimizeTheOverlap=true;
      printF("Overlap will be minimized\n");
    }
    else if( answer=="minimum overlap" )
    {
      gi.appendToTheDefaultPrompt("minimum overlap>");
      for( ;; )
      {
	int grid = gi.getMenuItem(gridMenu,answer2,"set minimum overlap for interpolation points on which grid(s)?");
	if( grid<0 )
	{
	  printF("Unknown response=%s\n",(const char*)answer2);
          gi.stopReadingCommandFile();
	  break;
	}
	else if( answer2=="none" || answer2=="done" )
	{
          break;
	}
        else
	{
    	  int grid2 = gi.getMenuItem(gridMenu,answer2,"...from which grid(s)?");
	  if( grid2<0 )
	  {
	    printF("Unknown response=%s\n",(const char*)answer2);
	    gi.stopReadingCommandFile();
	    break;
	  }
	  else if( answer2=="none" || answer2=="done" )
	  {
            break;
	  }
	  else 
	  {
	    gi.inputString(answer3,sPrintF(buff,"Enter the minimum overlap, default=.5 (of a grid spacing)"));
	    real minOverlap=.5;
	    if( answer3!="" )
	    {
	      sScanF(answer3,"%e",&minOverlap);
	    }
	    if( minOverlap<=0. )
	    {
	      printF("changeParameters::ERROR: The minimum Overlap must be greater than 0.! \n");
	      printF("changeParameters:: No change has been made.");
	    }
	    else
	    {
              Range G1 = grid <cg.numberOfComponentGrids() ? Range(grid ,grid ) : Range(0,cg.numberOfComponentGrids()-1);
              Range G2 = grid2<cg.numberOfComponentGrids() ? Range(grid2,grid2) : Range(0,cg.numberOfComponentGrids()-1);
	      
//              cg.backupInterpolationConditionLimit(G1,G2,all)=minOverlap;   // this is used to hold the value *fix*
//              display(cg.backupInterpolationConditionLimit,"minimum overlap");

              minimumOverlap(all,G1,G2,all)=minOverlap;
	      
              if( grid >=cg.numberOfComponentGrids() && grid2 >=cg.numberOfComponentGrids() )
              {
		break;
	      }
	    }
	  }
	}
      }
      gi.unAppendTheDefaultPrompt();
    }
    else if( answer=="boundary conditions" )
    {
      gi.appendToTheDefaultPrompt("bc>");
      for( ;; )
      {
	int grid = gi.getMenuItem(gridMenu,answer2,"change bc's for which grid(s)?");
	if( grid<0 )
	{
	  printF("Unknown response=%s\n",(const char*)answer2);
          gi.stopReadingCommandFile();
	  break;
	}
	else if( answer2=="none" || answer2=="done" )
	{
          break;
	}
	else 
	{

          const int gStart = grid<cg.numberOfComponentGrids() ? grid : 0;
	  const int gEnd = grid<cg.numberOfComponentGrids() ? grid : cg.numberOfComponentGrids()-1;
	  for( int g=gStart; g<=gEnd; g++ )
	  {
            assert( g>=0 && g<cg.numberOfComponentGrids());
	    MappedGrid & mg = cg[g];
            IntegerArray bc; bc = mg.boundaryCondition();
            if( cg.numberOfDimensions()==2 )
	    {
              printF("Grid %s : boundary conditions are (%i,%i, %i,%i)\n",
                      (const char*)mg.mapping().getName(Mapping::mappingName),bc(0,0),bc(1,0),bc(0,1),bc(1,1));  
	      gi.inputString(answer3,sPrintF(buff,"Enter values for grid %s. Current=(%i,%i, %i,%i)",
			     (const char*)mg.mapping().getName(Mapping::mappingName),
					     bc(0,0),bc(1,0),bc(0,1),bc(1,1)));
              if( answer3!="" )
      	        sScanF(answer3,"%i %i %i %i",&bc(0,0),&bc(1,0),&bc(0,1),&bc(1,1));
	    }
            else
	    {
              printF("Grid %s : boundary conditions are (%i,%i, %i,%i, %i,%i)\n",
                      (const char*)mg.mapping().getName(Mapping::mappingName),
		     bc(0,0),bc(1,0),bc(0,1),bc(1,1),bc(0,2),bc(1,2));
	      gi.inputString(answer3,sPrintF(buff,"Enter values for grid %s. Current=(%i,%i, %i,%i, %i,%i)",
			     (const char*)mg.mapping().getName(Mapping::mappingName),
					     bc(0,0),bc(1,0),bc(0,1),bc(1,1),bc(0,2),bc(1,2)));
              if( answer3!="" )
		sScanF(answer3,"%i %i %i %i %i %i",&bc(0,0),&bc(1,0),&bc(0,1),&bc(1,1),&bc(0,2),&bc(1,2));
	    }
	    for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	    {
	      for( int side=Start; side<=End; side++ )
	      {
                if( bc(side,axis)!=mg.boundaryCondition(side,axis) && mg.boundaryCondition(side,axis)>=0 )
		{
		  mg.setBoundaryCondition(side,axis,bc(side,axis));
		}
		else if( mg.boundaryCondition(side,axis)<0 )
		{
                  printF("Sorry, you cannot change (side,axis)=(%i,%i) on grid %s since the bc<0\n",
			 side,axis,(const char*)mg.mapping().getName(Mapping::mappingName));
		}
	      }
	    }
	  }
	}
      }
      gi.unAppendTheDefaultPrompt();
    }
    else if( answer=="specify shared boundaries" )
    {
      printF("Explicitly specify a portion of a boundary that should share a boundary with the side of "
             "another grid\n");
      
      gi.appendToTheDefaultPrompt("shared boundary>");
      for( ;; )
      {
	int grid = gi.getMenuItem(gridMenuNoAll,answer2,"specify shared boundary for which grid?");
	if( answer2=="none" || answer2=="done" )
	{
          break;
	}
	else if( grid<0 || grid>cg.numberOfComponentGrids() )
	{
	  printF("Invalid response: %s\n",(const char*)answer2);
          gi.stopReadingCommandFile();
	  break;
	}

	MappedGrid & mg = cg[grid];

	int side1=-1, dir1=-1;
	chooseASide( mg,side1,dir1 );
	if( side1<0 )
	{
	  gi.stopReadingCommandFile();
	  break;
	}
	  
	Index I1,I2,I3;
        int extra = (mg.discretizationWidth(0)-3)/2;
	getBoundaryIndex(extendedGridIndexRange(mg),side1,dir1,I1,I2,I3,extra);
	printF("Enter a region of points i1a,i1b,i2a,i2b,i3a,i3b\n"
	       " face (%i,%i) of grid %s has dimensions [%i,%i]x[%i,%i]x[%i,%i]\n",
	       side1,dir1,(const char*)mg.getName(),
               I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
	  
	gi.inputString(answer2,"Enter i1a,i1b,i2a,i2b,i3a,i3b\n");
        int iva[3], &i1a=iva[0], &i2a=iva[1], &i3a=iva[2];
        int ivb[3], &i1b=ivb[0], &i2b=ivb[1], &i3b=ivb[2];
	
	i1a=0,i1b=0,i2a=0,i2b=0,i3a=0,i3b=0;
	sScanF(answer2,"%i %i %i %i %i %i",&i1a,&i1b,&i2a,&i2b,&i3a,&i3b);
      
	if( i1a>i1b || i1a< I1.getBase() || i1b>I1.getBound() ||
	    i2a>i2b || i2a< I2.getBase() || i2b>I2.getBound() ||
	    i3a>i3b || i3a< I3.getBase() || i3b>I3.getBound() )
	{
	  printF(" ERROR: Invalid values [i1a,i1b]x[i2a,i2b]x[i3a,i3b]=[%i,%i]x[%i,%i]x[%i,%i]\n"
		 "        face (%i,%i) of grid %s has dimensions [%i,%i]x[%i,%i]x[%i,%i]\n",
		 i1a,i1b,i2a,i2b,i3a,i3b, side1,dir1,(const char*)mg.getName(),
		 I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
	
	  gi.stopReadingCommandFile();
	  break;
	}
        else if( iva[dir1]!=mg.gridIndexRange(side1,dir1) || ivb[dir1]!=mg.gridIndexRange(side1,dir1)  )
	{
          printF("ERROR: Invalid values [i1a,i1b]x[i2a,i2b]x[i3a,i3b]=[%i,%i]x[%i,%i]x[%i,%i]\n"
                 "   Points do not lie on the face (side,axis)=(%i,%i) of grid %s \n",
		 i1a,i1b,i2a,i2b,i3a,i3b, side1,dir1,(const char*)mg.getName());
          printF(" i%ia=%i should equal %i and i%ib=%i should equal %i\n",
		 dir1+1,iva[dir1],mg.gridIndexRange(side1,dir1),
		 dir1+1,ivb[dir1],mg.gridIndexRange(side1,dir1));
	  gi.stopReadingCommandFile();
	  break;
	}
	


	int grid2 = gi.getMenuItem(gridMenuNoAll,answer2,"share a boundary with which grid?");
	if( answer2=="none" || answer2=="done" )
	{
	  break;
	}
	else if( grid2<0 || grid2>cg.numberOfComponentGrids()  )
	{
	  printF("Invalid response: %s\n",(const char*)answer2);
	  gi.stopReadingCommandFile();
	  break;
	}
	int side2=-1, dir2=-1;
	chooseASide( cg[grid2],side2,dir2 );
	if( side2<0 )
	{
	  gi.stopReadingCommandFile();
	  break;
	}

	real rtol,xtol,ntol;
    
        rtol=.8;
	xtol=REAL_MAX*.1;
	ntol=maximumAngleDifferenceForNormalsOnSharedBoundaries;
	
	if( manualSharedBoundary.getLength(0) <= numberOfManualSharedBoundaries )
	{
	  manualSharedBoundary.resize(numberOfManualSharedBoundaries+5,12);
	  manualSharedBoundaryValue.resize(numberOfManualSharedBoundaries+5,3);
	}
        const int n=numberOfManualSharedBoundaries;
	manualSharedBoundary(n,0)=grid;
	manualSharedBoundary(n,1)=side1;
	manualSharedBoundary(n,2)=dir1;
	manualSharedBoundary(n,3)=grid2;
	manualSharedBoundary(n,4)=side2;
        manualSharedBoundary(n,5)=dir2;
	manualSharedBoundary(n,6)=i1a;
	manualSharedBoundary(n,7)=i1b;
	manualSharedBoundary(n,8)=i2a;
	manualSharedBoundary(n,9)=i2b;
	manualSharedBoundary(n,10)=i3a;
	manualSharedBoundary(n,11)=i3b;
	  
	  
	manualSharedBoundaryValue(n,0)=rtol;
	manualSharedBoundaryValue(n,1)=xtol;
	manualSharedBoundaryValue(n,2)=ntol;
	numberOfManualSharedBoundaries++;

        printF("A shared boundary is determined by 3 tolerances: \n"
               "  rtol : tolerance in parameter space \n"
               "  xtol : tolerance in physical space \n"
               "  ntol : maximum angle between normals \n"
               "A point will be deemed part of the shared boundary if ALL conditions are met\n");
	
        gi.outputString("Enter a matching tolerance or choose `done' to continue");
	
	aString matchMenu[] =
	{
	  "r matching tolerance",
	  "x matching tolerance",
	  "normal matching angle",
	  "done",
	  ""
	};
      
        for( ;; )
	{
	  gi.getMenuItem(matchMenu,answer3,"specify a tolerance.");
          if( answer3=="done" )
	  {
	    break;
	  }
	  else if( answer3=="r matching tolerance" )
	  {
	    gi.inputString(answer3,sPrintF(buff,"Set r matching tolerance (default=%6.2e)",rtol));
	    sScanF(answer3,"%e",&rtol);
	    manualSharedBoundaryValue(n,0)=rtol;
	  }
	  else if( answer3=="x matching tolerance" )
	  {
	    gi.inputString(answer3,sPrintF(buff,"Set x matching tolerance (default=%6.2e)",xtol));
	    sScanF(answer3,"%e",&xtol);
	    manualSharedBoundaryValue(n,1)=xtol;
	  }
          else if( answer3=="normal matching angle" )
	  {
            real angle=acos(min(1.,max(0.,1.-ntol)))*360./twoPi;
	    gi.inputString(answer3,sPrintF(buff,"Enter the maximum angle between matching normals (degrees)=%5.2f)",
                   angle));
	    sScanF(answer3,"%e",&angle);
            ntol=1.-cos(angle*twoPi/360.);
	    manualSharedBoundaryValue(n,2)=ntol;
            printF(" angle=%f degree, (ntol=%9.3e) \n",angle,ntol);
	  }
	  else
	  {
	    printF("Unknown response=%s\n",(const char*)answer2);
	    gi.stopReadingCommandFile();
	    break;
	  }
	}
      }
      gi.unAppendTheDefaultPrompt();
    }
    else if( answer=="mixed boundary" )
    {
      // A mixed boundary is a physical bc (boundaryCondition(side,axis)>0 ) but may have
      // one or more intervals that interpolate from other grids
      printF("A mixed boundary is a physical boundary with boundaryCondition(side,axis)>0 \n"
	     "but has one or more intervals that interpolate from some other grid\n"
	     "A mixed boundary can be used to form a C-grid or an H-grid, for example.\n");


      gi.appendToTheDefaultPrompt("mixed bc>");
      for( ;; )
      {
	int grid = gi.getMenuItem(gridMenu,answer2,"Set mixed bc for which grid(s)?");
	if( grid<0 )
	{
	  printF("Unknown response=%s\n",(const char*)answer2);
          gi.stopReadingCommandFile();
	  break;
	}
	else if( answer2=="none" || answer2=="done" )
	{
          break;
	}
	else 
	{

          gi.appendToTheDefaultPrompt("side>");

          const int gStart = grid<cg.numberOfComponentGrids() ? grid : 0;
	  const int gEnd = grid<cg.numberOfComponentGrids() ? grid : cg.numberOfComponentGrids()-1;
	  for( int g=gStart; g<=gEnd; g++ )
	  {
            assert( g>=0 && g<cg.numberOfComponentGrids());
	    MappedGrid & mg = cg[g];
            IntegerArray bc; bc = mg.boundaryCondition();
	    int response=-1;
            if( cg.numberOfDimensions()==2 )
	    {
              aString sideMenu[]=
	      {
		"left   (side=0,axis=0)",
                "right  (side=1,axis=0)",
                "bottom (side=0,axis=1)",
                "top    (side=1,axis=1)",
                ""
	      };

              printF("Grid %s : boundary conditions are (%i,%i, %i,%i)\n",
                      (const char*)mg.mapping().getName(Mapping::mappingName),bc(0,0),bc(1,0),bc(0,1),bc(1,1));  

	      response=gi.getMenuItem(sideMenu,answer3,"Set which side?");
	    }
	    else
	    {
              aString sideMenu[]=
	      {
		"left   (side=0,axis=0)",
                "right  (side=1,axis=0)",
                "bottom (side=0,axis=1)",
                "top    (side=1,axis=1)",
                "front  (side=0,axis=2)",
                "back   (side=1,axis=2)",
                ""
	      };

              printF("Grid %s : boundary conditions are (%i,%i, %i,%i, %i,%i)\n",
                      (const char*)mg.mapping().getName(Mapping::mappingName),
		     bc(0,0),bc(1,0),bc(0,1),bc(1,1),bc(0,2),bc(1,2));
	      response=gi.getMenuItem(sideMenu,answer3,"Set which side?");
	    }
	    
	    if( response<0 || response> (2*cg.numberOfDimensions()-1) )
	    {
	      printF("Unknown response=%s\n",(const char*)answer2);
	      gi.stopReadingCommandFile();
	      break;
	    }
	    else
	    {
	      int side=response %2;
	      int axis = response/2;
			
              MappedGrid & mg = cg[grid];
	      mg.setBoundaryFlag(side,axis,MappedGridData::mixedPhysicalInterpolationBoundary);

	      int gridI = gi.getMenuItem(gridMenu,answer3,"Interpolate from which grid(s)?");

              if( answer=="done" || answer=="exit" )
	      {
		break;
	      }
	      else if( (gridI>=0 && gridI<cg.numberOfComponentGrids()) || answer3=="all" )
	      {
                gi.appendToTheDefaultPrompt("tol>");

                real rtol=1.e-3, xtol=REAL_EPSILON*10.;
                printF("A mixed boundary can be used to build a 'c-grid' or an 'h-grid' where part of the \n"
                       "boundary matches to another boundary. \n"
                       " \n"
                       "The mixed boundary interpolation points can be determined automatically or you can\n"
                       "specify explicity which points should be interpolated\n" 
                       " \n"
                       "  1. Automatic: for the automatic determination of the mixed boundary interpolation points\n"
                       "     You can specify the tolerance for matching in two possible ways: \n"
                       "     r matching tolerance : boundaries match if points are this close in unit square space\n"
                       "     x matching tolerance : boundaries match if points are this close in x space\n"
                       "     -- boundaries will match if either one of the above two matching conditions holds ---\n"
                       "     Default values: rTolerance=%8.2e, xTolerance=%8.2e\n"
                       "   NOTE: When a grid interpolates from itself and the points are being automatically \n"
                       "     determined then both sides of the branch cut are determined. On one side the \n"
                       "     boundary points will be interpolated and on the other the ghost points.\n"  
                       "  2. Non-automatic: specify a region on the face that should be interpolated\n"
                       "     (If there are multiple disjoint regions to interpolate, each one should be \n"
                       "      specified separately)\n"
                       "    NOTE: In non-automatic mode when you want to interpolate a grid from itself you must\n"
                       "       specify two sets of points; normally the boundary points on one side will be\n"
                       "       specified and the ghost points on the other side\n"
                       " When a grid is interpolated from itself (such as a c-grid) the grid is temporarily\n"
                       " split into two pieces so that the points on one half interpolate from the second half.\n"
                       " (We need to prevent a point from interpolating from itself.)\n"
                       " By default, for a mixed boundary on (side,axis) the grid is split at the halfway point\n"
                       " along `(axis+1) mod numberOfDimensions'. If this is not correct you should explicity\n"
                       " specify where to split the grid using the `specify split for self interpolation' option.\n"
                       "\n **Optionally choose `determine mixed boundary points' to see which points will be\n"
                       " interpolated with the current parameters **\n"
                        ,rtol,xtol);

                aString mixedMenu[] =
		{
		  "r matching tolerance",
                  "x matching tolerance",
                  "determine mixed boundary points",
                  "specify mixed boundary points",
                  "specify split for self interpolation",
                  "mark all boundary points as non-cutting",
                  "do not mark all boundary points as non-cutting",
                  "done",
                  ""
		};
		
                const int numberOfMixedBoundaryEntries=13;
		if( mixedBoundary.getLength(0)<=numberOfMixedBoundaries+5 )
		{
                  // Range all;
		  int n=numberOfMixedBoundaries+5;
		  mixedBoundary.resize(n,numberOfMixedBoundaryEntries);
		  mixedBoundary(Range(numberOfMixedBoundaries,n-1),all)=-1;
		    
		  mixedBoundaryValue.resize(n,2); 
		  mixedBoundaryValue(Range(numberOfMixedBoundaries,n-1),all)=-1;
		}
		int n=numberOfMixedBoundaries;
		numberOfMixedBoundaries+=1;
		  
		mixedBoundary(n,0)=grid;
		mixedBoundary(n,1)=side;
		mixedBoundary(n,2)=axis;
		mixedBoundary(n,3)=gridI>=0 && gridI<cg.numberOfComponentGrids() ? gridI : -1; // -1 means use any grid
                mixedBoundary(n,4)=-1;  // i1a
                mixedBoundary(n,5)=-2;  // i1b
                mixedBoundary(n,6)=-1;  // i2a
                mixedBoundary(n,7)=-2;  // i2b
                mixedBoundary(n,8)=-1;  // i3a
                mixedBoundary(n,9)=-2;  // i3b
		mixedBoundary(n,10)=-1; // splitAxis
		mixedBoundary(n,11)=-1; // splitIndex
                mixedBoundary(n,12)=1;   // mark user specied points as non-cutting boundary points.

                mixedBoundaryValue(n,0)=rtol;
		mixedBoundaryValue(n,1)=xtol;
		  
		if( grid==gridI )
		{
		  printF("*** use robust inverse for this 'c-grid'\n");
		  mg.mapping().useRobustInverse(true);
		  // mg.mapping().getMapping().reinitialize();
		}

                for( ;; )
		{
		  gi.getMenuItem(mixedMenu,answer3,"Set a tolerance or specify points.");
		  if( answer3=="r matching tolerance" )
		  {
		    gi.inputString(answer3,sPrintF(buff,"Set r matching tolerance (default=%6.2e)",rtol));
		    sScanF(answer3,"%e",&rtol);
                    mixedBoundaryValue(n,0)=rtol;
		  }
		  else if( answer3=="x matching tolerance" )
		  {
		    gi.inputString(answer3,sPrintF(buff,"Set x matching tolerance (default=%6.2e)",xtol));
		    sScanF(answer3,"%e",&xtol);
  		    mixedBoundaryValue(n,1)=xtol;
		  }
                  else if( answer3=="determine mixed boundary points" )
		  {
                    updateGeometry(cg,cg); // we need to build some geometry arrays
		    
		    interpolateMixedBoundary(cg,n);
                
                    // generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );

		    plot( "After interpolating a mixed boundary",cg,false);

		  }
                  else if( answer3=="do not mark all boundary points as non-cutting" )
		  {
                    printF("By default all boundary points of the user specified region are marked\n"
                           "as points that do not cut holes. This option has now been turned off.\n"
                           "Only boundary points next to ghost points that can interpolate will be \n"
                           "marked as non-cutting\n");
		    mixedBoundary(n,12)=0;
		  }
                  else if( answer3=="mark all boundary points as non-cutting" )
		  {
                    printF("By default all boundary points of the user specified region are marked\n"
                           "as points that do not cut holes. This option has now been turned on.\n");
		    mixedBoundary(n,12)=1;
		  }
                  else if( answer3=="specify mixed boundary points" )
		  {
                    int iva[3], ivb[3];
                    for( dir=0; dir<3; dir++ )
		    {
		      iva[dir]= mg.gridIndexRange(0,dir);
		      ivb[dir]= mg.gridIndexRange(1,dir);
		    }
		    iva[axis]=mg.gridIndexRange(side,axis);
		    ivb[axis]=iva[axis];
		    if( side==0 )
		      iva[axis]-=1;
		    else
                      ivb[axis]+=1;

		    if( mg.numberOfDimensions()==2 )
		    {
		      printF(" Specify a line of points in index space to interpolate, in [%i,%i][%i,%i].\n" 
                             " The index values along axis=%i should be specified as [%i,%i] for the boundary\n"
                             " or [%i,%i] for the ghost line.\n",
                             iva[0],ivb[0],iva[1],ivb[1],axis,
                             mg.gridIndexRange(side,axis),
                             mg.gridIndexRange(side,axis),
                             mg.gridIndexRange(side,axis)+2*side-1,
                             mg.gridIndexRange(side,axis)+2*side-1);
                      gi.inputString(answer3,sPrintF(buff,"Enter the interval (four int's in [%i,%i][%i,%i])",
						     iva[0],ivb[0],iva[1],ivb[1]));
		      sScanF(answer3,"%i %i %i %i",&iva[0],&ivb[0],&iva[1],&ivb[1]);
		    }
		    else 
		    {
		      printF(" Specify a rectangle of pts in index space to interpolate, in [%i,%i][%i,%i][%i,%i].\n" 
                             " The index values along axis=%i should be specified as [%i,%i] for the boundary\n"
                             " or [%i,%i] for the ghost line.\n",
                             iva[0],ivb[0],iva[1],ivb[1],iva[2],ivb[2],axis,
                             mg.gridIndexRange(side,axis),
                             mg.gridIndexRange(side,axis),
                             mg.gridIndexRange(side,axis)+2*side-1,
                             mg.gridIndexRange(side,axis)+2*side-1);

                      gi.inputString(answer3,sPrintF(buff,"Enter the interval (6 int's in [%i,%i][%i,%i][%i,%i])",
						     iva[0],ivb[0],iva[1],ivb[1],iva[2],ivb[2]));
		      sScanF(answer3,"%i %i %i %i %i %i",&iva[0],&ivb[0],&iva[1],&ivb[1],&iva[2],&ivb[2]);

		    }
                    if( iva[axis]!=ivb[axis] || ( iva[axis]!=mg.gridIndexRange(side,axis) &&
						  iva[axis]!=mg.gridIndexRange(side,axis)+2*side-1 ) )
		    {
                       printF("ERROR: choosing values for iva[%i]=%i or ivb[%i]=%i \n"
                              " These values should both be equal and equal to %i or %i\n"
                              " I will set the values to be both %i (interpolating the ghost line)\n",
			      axis,iva[axis],axis,ivb[axis], mg.gridIndexRange(side,axis),
			      mg.gridIndexRange(side,axis)+2*side-1,mg.gridIndexRange(side,axis)+2*side-1);
		       iva[axis]=mg.gridIndexRange(side,axis)+2*side-1;
		       ivb[axis]=iva[axis];
		    }
                    bool ok=true;
                    for( dir=1; dir<mg.numberOfDimensions(); dir++ )
		    {
		      int axisp=(axis+dir)%mg.numberOfDimensions();
		      if( iva[axisp]>ivb[axisp] || iva[axisp]<mg.gridIndexRange(0,axisp) ||
			  ivb[axisp]>mg.gridIndexRange(1,axisp) )
		      {
                        ok=false;
			printF("ERROR: Invalid values iva[%i]=%i ivb[%i]=%i, gridIndexRange=[%i,%i]. Values should \n"
                               "  satisfy  gridIndexRange(0,%i) <= iva[%i] <= ivb[%i] <= gridIndexRange(1,%i) \n",
                               axisp,iva[axisp],axisp,ivb[axisp],mg.gridIndexRange(0,axisp),mg.gridIndexRange(1,axisp),
			       axisp,axisp,axisp,axisp);
		      }
		    }
		    if( !ok )
		    {
		      gi.stopReadingCommandFile();
		      break;
		    }
		    
		    if( mg.numberOfDimensions()==2 )
		    {
		      printF("Choosing points [%i,%i][%i,%i] to interpolate\n",iva[0],ivb[0],iva[1],ivb[1]);
		    }
		    else
		    {
		      printF("Choosing pts [%i,%i][%i,%i][%i,%i] to interpolate\n",iva[0],ivb[0],iva[1],ivb[1],
			     iva[2],ivb[2]);
		    }
		    
		    mixedBoundary(n,4)=iva[0];
		    mixedBoundary(n,5)=ivb[0];
		    mixedBoundary(n,6)=iva[1];
		    mixedBoundary(n,7)=ivb[1];
		    mixedBoundary(n,8)=iva[2];
		    mixedBoundary(n,9)=ivb[2];
		  }
		  else if( answer3=="specify split for self interpolation" )
		  {
                    int splitAxis=(axis+1)%mg.numberOfDimensions();
                    int num=mg.gridIndexRange(1,splitAxis)-mg.gridIndexRange(0,splitAxis);
		    int splitIndex=(num-1)/2-1;
                    printF("Specify where to split the grid into two pieces to prevent a point of the\n"
                           "mixed boundary from interpolating from itself\n"
                           "The grid has index bounds: [%i,%i][%i,%i][%i,%i]\n",
                           mg.gridIndexRange(0,0),mg.gridIndexRange(1,0),
                           mg.gridIndexRange(0,1),mg.gridIndexRange(1,1),
                           mg.gridIndexRange(0,2),mg.gridIndexRange(1,2));
		    gi.inputString(answer3,sPrintF(buff,"Enter splitAxis,splitIndex (default=%i %i)",
                           splitAxis,splitIndex));
		    sScanF(answer3,"%i %i",&splitAxis,&splitIndex);
                    if( splitAxis==axis || splitAxis<0 || splitAxis>=mg.numberOfDimensions() )
		    {
		      printF("ERROR: Invalid value for splitAxis=%i. No changes made.\n",splitAxis);
		    }
		    else if( splitIndex<mg.gridIndexRange(0,splitAxis) || splitIndex>mg.gridIndexRange(1,splitAxis) )
		    {
		      printF("ERROR: splitIndex=%i should be in the range [%i,%i].No changes made.\n",
			     splitIndex,mg.gridIndexRange(0,splitAxis),mg.gridIndexRange(1,splitAxis));
		    }
		    else
		    {
		      printF("Setting splitAxis=%i and splitIndex=%i\n",splitAxis,splitIndex);
		      mixedBoundary(n,10)=splitAxis;
		      mixedBoundary(n,11)=splitIndex;
		    }

		  }
		  else if( answer3!="done" )
		  {
		    printF("Unknown response=%s\n",(const char*)answer3);
		    gi.stopReadingCommandFile();
		    break;
		  }
                  else
		  {
		    break;
		  }
		} // for( ;; ) set a tolerance
		
                gi.unAppendTheDefaultPrompt();

	      }
	      else if( answer3=="none" )
	      {
                printF("reseting boundary to NOT be a mixed boundary\n");
   	        mg.setBoundaryCondition(side,axis,mg.boundaryCondition(side,axis));

                int n=0;
		while( n<numberOfMixedBoundaries )
		{
		  if( mixedBoundary(n,0)==grid && 
                      mixedBoundary(n,1)==side &&
                      mixedBoundary(n,2)==axis  )
		  {
                    if( n< (numberOfMixedBoundaries-1) )
		    {
                      mixedBoundary(n,mixedBoundary.dimension(1))=mixedBoundary(n+1,mixedBoundary.dimension(1));
                      mixedBoundaryValue(n,mixedBoundaryValue.dimension(1))=
                              mixedBoundaryValue(n+1,mixedBoundaryValue.dimension(1));
		    }
                    numberOfMixedBoundaries--;
		  }
                  else
		  {
		    n++;
		  }
		}
	      }
	      else
	      {
                printF("Unknown response\n");
		gi.stopReadingCommandFile();
	      }

	    }
	  }
          gi.unAppendTheDefaultPrompt();

	}
      }
      gi.unAppendTheDefaultPrompt();
    }
    else if( answer=="non-cutting boundary points" )
    {
      gi.outputString("Specify portions of physical boundaries that should not cut holes");
      

      gi.appendToTheDefaultPrompt("non-cutting boundary>");
      for( ;; )
      {
	int grid = gi.getMenuItem(gridMenuNoAll,answer2,"specify a non-cutting boundary for which grid?");
	if( answer2=="none" || answer2=="done" )
	{
          break;
	}
	else if( grid<0 || grid>cg.numberOfComponentGrids() )
	{
	  printF("Invalid response: %s\n",(const char*)answer2);
          gi.stopReadingCommandFile();
	  break;
	}

	MappedGrid & mg = cg[grid];

	int side1=-1, dir1=-1;
	chooseASide( mg,side1,dir1 );
	if( side1<0 )
	{
	  gi.stopReadingCommandFile();
	  break;
	}
	  
	Index I1,I2,I3;
	getBoundaryIndex(extendedGridIndexRange(mg),side1,dir1,I1,I2,I3);
	printF("Enter a region of points i1a,i1b,i2a,i2b,i3a,i3b\n"
	       " face (%i,%i) of grid %s has dimensions [%i,%i]x[%i,%i]x[%i,%i]\n",
	       side1,dir1,(const char*)mg.getName(),
               I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
	  
	gi.inputString(answer2,"Enter i1a,i1b,i2a,i2b,i3a,i3b\n");
        int iva[3], &i1a=iva[0], &i2a=iva[1], &i3a=iva[2];
        int ivb[3], &i1b=ivb[0], &i2b=ivb[1], &i3b=ivb[2];
	
	i1a=0,i1b=0,i2a=0,i2b=0,i3a=0,i3b=0;
	sScanF(answer2,"%i %i %i %i %i %i",&i1a,&i1b,&i2a,&i2b,&i3a,&i3b);
      
	if( i1a>i1b || i1a< I1.getBase() || i1b>I1.getBound() ||
	    i2a>i2b || i2a< I2.getBase() || i2b>I2.getBound() ||
	    i3a>i3b || i3a< I3.getBase() || i3b>I3.getBound() )
	{
	  printF(" ERROR: Invalid values [i1a,i1b]x[i2a,i2b]x[i3a,i3b]=[%i,%i]x[%i,%i]x[%i,%i]\n"
		 "        face (%i,%i) of grid %s has dimensions [%i,%i]x[%i,%i]x[%i,%i]\n",
		 i1a,i1b,i2a,i2b,i3a,i3b, side1,dir1,(const char*)mg.getName(),
		 I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
	
	  gi.stopReadingCommandFile();
	  break;
	}
        else if( iva[dir1]!=mg.gridIndexRange(side1,dir1) || ivb[dir1]!=mg.gridIndexRange(side1,dir1)  )
	{
          printF("ERROR: Invalid values [i1a,i1b]x[i2a,i2b]x[i3a,i3b]=[%i,%i]x[%i,%i]x[%i,%i]\n"
                 "   Points do not lie on the face (side,axis)=(%i,%i) of grid %s \n",
		 i1a,i1b,i2a,i2b,i3a,i3b, side1,dir1,(const char*)mg.getName());
          printF(" i%ia=%i should equal %i and i%ib=%i should equal %i\n",
		 dir1+1,iva[dir1],mg.gridIndexRange(side1,dir1),
		 dir1+1,ivb[dir1],mg.gridIndexRange(side1,dir1));
	  gi.stopReadingCommandFile();
	  break;
	}

	if( nonCuttingBoundaryPoints.getLength(0)<=numberOfNonCuttingBoundaries+5 )
	{
	  const int n=numberOfNonCuttingBoundaries+5;
	  nonCuttingBoundaryPoints.resize(n,7);
	  nonCuttingBoundaryPoints(Range(numberOfNonCuttingBoundaries,n-1),all)=-1;
	}
	const int n=numberOfNonCuttingBoundaries;
	numberOfNonCuttingBoundaries++;
		  
        nonCuttingBoundaryPoints(n,0)=grid;
        nonCuttingBoundaryPoints(n,1)=i1a;
        nonCuttingBoundaryPoints(n,2)=i1b;
        nonCuttingBoundaryPoints(n,3)=i2a;
        nonCuttingBoundaryPoints(n,4)=i2b;
        nonCuttingBoundaryPoints(n,5)=i3a;
        nonCuttingBoundaryPoints(n,6)=i3b;
      }
      
      
    }
    else if( answer=="default shared boundary normal tolerance" )
    {
      printF("The shared boundary normal tolerance determines the allowable variance in 1-cos(angle) \n"
             " of the 'angle' between the normals on two shared sides. A value of 0. would allow no \n"
             " variance while a value of 1. would be complete variance. Typically choose a value in [.1,.75], \n"
             " choosing a larger value if the surface is not smoothly represented with the grids \n");

      gi.inputString(answer2,sPrintF(buff,"Enter the default shared boundary normal tolerance (current=%f)",
                     maximumAngleDifferenceForNormalsOnSharedBoundaries));
      if( answer2!="" )
	sScanF(answer2,"%e",&maximumAngleDifferenceForNormalsOnSharedBoundaries);
    }
    else if( answer=="shared boundary tolerances" )
    {
      printF("Specify tolerances to determine which points interpolate on shared boundaries\n");
      
      gi.appendToTheDefaultPrompt("shared boundary tolerances>");
      for( ;; )
      {
	int grid = gi.getMenuItem(gridMenuNoAll,answer2,"specify shared boundary tolerances for which grid?");
	if( answer2=="none" || answer2=="done" )
	{
          break;
	}
	else if( grid<0 || grid>=cg.numberOfComponentGrids() )
	{
	  printF("Invalid response: %s\n",(const char*)answer2);
          gi.stopReadingCommandFile();
	  break;
	}

	MappedGrid & mg = cg[grid];

	int side1=-1, dir1=-1;
	chooseASide( mg,side1,dir1 );
	if( side1<0 )
	{
	  gi.stopReadingCommandFile();
	  break;
	}
	  
	int grid2 = gi.getMenuItem(gridMenuNoAll,answer2,"sharing a boundary with which other grid?");
	if( answer2=="none" || answer2=="done" )
	{
	  break;
	}
	else if( grid2<0 || grid2>=cg.numberOfComponentGrids()  )
	{
	  printF("Invalid response: %s\n",(const char*)answer2);
	  gi.stopReadingCommandFile();
	  break;
	}
	int side2=-1, dir2=-1;
	chooseASide( cg[grid2],side2,dir2 );
	if( side2<0 )
	{
	  gi.stopReadingCommandFile();
	  break;
	}

	real rtol,xtol,ntol;
    
	// defaults -- these are also in getSharedBoundaryTolerances *fix this*
        rtol=.8;
	xtol=REAL_MAX*.1;
	ntol=maximumAngleDifferenceForNormalsOnSharedBoundaries;
	
	if( sharedBoundaryTolerances.getLength(0) <= numberOfSharedBoundaryTolerances )
	{
	  sharedBoundaryTolerances.resize(numberOfSharedBoundaryTolerances+5,6);
	  sharedBoundaryTolerancesValue.resize(numberOfSharedBoundaryTolerances+5,3);
	}
        const int n=numberOfSharedBoundaryTolerances;
	sharedBoundaryTolerances(n,0)=grid;
	sharedBoundaryTolerances(n,1)=side1;
	sharedBoundaryTolerances(n,2)=dir1;
	sharedBoundaryTolerances(n,3)=grid2;
	sharedBoundaryTolerances(n,4)=side2;
        sharedBoundaryTolerances(n,5)=dir2;
	  
	sharedBoundaryTolerancesValue(n,0)=rtol;
	sharedBoundaryTolerancesValue(n,1)=xtol;
	sharedBoundaryTolerancesValue(n,2)=ntol;
	numberOfSharedBoundaryTolerances++;

        printF("A shared boundary is determined by 3 tolerances: \n"
               "  rtol : tolerance in parameter space \n"
               "  xtol : tolerance in physical space \n"
               "  ntol : maximum angle between normals \n"
               "A point will be deemed part of the shared boundary if ALL conditions are met\n");
	
        gi.outputString("Enter a matching tolerance or choose `done' to continue");
	
	aString matchMenu[] =
	{
	  "r matching tolerance",
	  "x matching tolerance",
	  "normal matching angle",
	  "done",
	  ""
	};
      
        for( ;; )
	{
	  gi.getMenuItem(matchMenu,answer3,"specify a tolerance.");
          if( answer3=="done" )
	  {
	    break;
	  }
	  else if( answer3=="r matching tolerance" )
	  {
	    gi.inputString(answer3,sPrintF(buff,"Set r matching tolerance (default=%6.2e)",rtol));
	    sScanF(answer3,"%e",&rtol);
	    sharedBoundaryTolerancesValue(n,0)=rtol;
	  }
	  else if( answer3=="x matching tolerance" )
	  {
	    gi.inputString(answer3,sPrintF(buff,"Set x matching tolerance (default=%6.2e)",xtol));
	    sScanF(answer3,"%e",&xtol);
	    sharedBoundaryTolerancesValue(n,1)=xtol;
	  }
          else if( answer3=="normal matching angle" )
	  {
            real angle=acos(min(1.,max(0.,1.-ntol)))*360./twoPi;
	    gi.inputString(answer3,sPrintF(buff,"Enter the maximum angle between matching normals (degrees)=%5.2f)",
                   angle));
	    sScanF(answer3,"%e",&angle);
            ntol=1.-cos(angle*twoPi/360.);
	    sharedBoundaryTolerancesValue(n,2)=ntol;
            printF(" angle=%f degree, (ntol=%9.3e) \n",angle,ntol);
	  }
	  else
	  {
	    printF("Unknown response=%s\n",(const char*)answer2);
	    gi.stopReadingCommandFile();
	    break;
	  }
	}
      }
      gi.unAppendTheDefaultPrompt();

    }
//      else if( answer=="shared boundary tolerance" )
//      {
//        // **** I don't think this tolerance is currently used *****
//        gi.appendToTheDefaultPrompt("shared tol>");
//        for( ;; )
//        {
//  	int grid = gi.getMenuItem(gridMenu,answer2,"change shared boundary tolerance for which grid(s)?");
//  	if( grid<0 )
//  	{
//  	  cout << "Unknown response=" << answer2 << endl;
//            gi.stopReadingCommandFile();
//  	  break;
//  	}
//  	else if( answer2=="none" || answer2=="done" )
//  	{
//            break;
//  	}
//          else
//  	{
//  	  gi.inputString(answer3,sPrintF(buff,"Enter tolerance, default=.1 (of a grid spacing)"));
//  	  real spacing=.1;
//  	  if( answer3!="" )
//  	  {
//  	    sScanF(answer3,"%e",&spacing);
//  	  }
//            if(  grid<cg.numberOfComponentGrids() )
//              cg[grid]->sharedBoundaryTolerance=spacing;   // sharedBoundaryTolerance(side,axis)
//            else
//  	  {
//  	    for( int g=0; g<cg.numberOfComponentGrids(); g++ )
//                cg[g]->sharedBoundaryTolerance=spacing;   // sharedBoundaryTolerance(side,axis)

//              break;
//  	  }
//  	}
//        }
//        gi.unAppendTheDefaultPrompt();
//      }
    else if( answer=="shared boundary flag" )
    {
      gi.appendToTheDefaultPrompt("share flag>");
      for( ;; )
      {
	int grid = gi.getMenuItem(gridMenu,answer2,"change shared boundary flag for which grid(s)?");
	if( grid<0 )
	{
	  printF("Unknown response=%s\n",(const char*)answer2);
          gi.stopReadingCommandFile();
	  break;
	}
	else if( answer2=="none" || answer2=="done" )
	{
          break;
	}
	else 
	{

          const int gStart = grid<cg.numberOfComponentGrids() ? grid : 0;
	  const int gEnd = grid<cg.numberOfComponentGrids() ? grid : cg.numberOfComponentGrids()-1;
	  for( int g=gStart; g<=gEnd; g++ )
	  {
            assert( g>=0 && g<cg.numberOfComponentGrids());
	    MappedGrid & mg = cg[g];
            const IntegerArray & share = mg.sharedBoundaryFlag();
            const IntegerArray & bc = mg.boundaryCondition();
            if( cg.numberOfDimensions()==2 )
	    {
              printF("Grid %s : boundary conditions are (%i,%i, %i,%i)\n",
                      (const char*)mg.mapping().getName(Mapping::mappingName),bc(0,0),bc(1,0),bc(0,1),bc(1,1));  
	      gi.inputString(answer3,sPrintF(buff,"Enter values for grid %s. Current=(%i,%i, %i,%i)",
			     (const char*)mg.mapping().getName(Mapping::mappingName),
                             share(0,0),share(1,0),share(0,1),share(1,1)));
              if( answer3!="" )
      	        sScanF(answer3,"%i %i %i %i",&share(0,0),&share(1,0),&share(0,1),&share(1,1));
	    }
            else
	    {
              printF("Grid %s : boundary conditions are (%i,%i, %i,%i, %i,%i)\n",
                      (const char*)mg.mapping().getName(Mapping::mappingName),
                      bc(0,0),bc(1,0),bc(0,1),bc(1,1),bc(0,2),bc(1,2));  
	      gi.inputString(answer3,sPrintF(buff,"Enter values for grid %s. Current=(%i,%i, %i,%i, %i,%i)",
			     (const char*)mg.mapping().getName(Mapping::mappingName),
                             share(0,0),share(1,0),share(0,1),share(1,1),share(0,2),share(1,2)));
              if( answer3!="" )
	        sScanF(answer3,"%i %i %i %i %i %i",&share(0,0),&share(1,0),&share(0,1),&share(1,1),
                  &share(0,2),&share(1,2));
	    }
	    
	  }
	}
      }
      gi.unAppendTheDefaultPrompt();
    }
    else if( answer=="shared sides may cut holes" )
    {
      printF("By default, sides that share do not cut holes in each other.\n");
      printF("In some cases you will need to allow this.\n");
      
      gi.appendToTheDefaultPrompt("shared sides>");
      for( ;; )
      {
	int cutter = gi.getMenuItem(gridMenu,answer2,"change shared side cutting for which grid(s)? (Cutting grid)");
        if( cutter<0 )
	{
	  printF("Unknown response=%s\n",(const char*)answer2);
          gi.stopReadingCommandFile();
	  break;
	}
	if( answer2!="none" && answer2!="done" )
	{
	  int cuttee = gi.getMenuItem(gridMenu,answer3,"allow hole cutting in which grid(s)?");
          if( cuttee <0 )
	  {
	    printF("Unknown response=%s\n",(const char*)answer3);
            gi.stopReadingCommandFile();
            continue;
	  }

	  Range Rcut, Rcuttee;
	  if( cutter<cg.numberOfComponentGrids() )
	    Rcut=Range(cutter,cutter);
	  else 
	    Rcut=Range(0,cg.numberOfComponentGrids()-1);   // change all  cutter grids
	  
	  if( cuttee<cg.numberOfComponentGrids() )
	    Rcuttee=Range(cuttee,cuttee);
	  else 
	    Rcuttee=Range(0,cg.numberOfComponentGrids()-1);  // change all cuttee grids
	  
	  bool cuttingChoice =  answer3=="none" ? false : true;
	  cg.sharedSidesMayCutHoles(Rcut,Rcuttee)=cuttingChoice;

          if( debug & 4 )
	    display(cg.sharedSidesMayCutHoles,"cg.sharedSidesMayCutHoles");

	}
        else
          break;
      }
      gi.unAppendTheDefaultPrompt();
    }
    else if( answer=="ghost points" )
    {
      gi.appendToTheDefaultPrompt("ghost>");
      int grid = gi.getMenuItem(gridMenu,answer2,"change ghost points on which grid(s)");
      if( grid<0 )
      {
	printF("Unknown response=%s\n",(const char*)answer2);
        gi.stopReadingCommandFile();
      }
      else if( answer2!="none" && answer2!="done" )
      {
        Range R;
	if( grid<cg.numberOfComponentGrids() )
	  R=Range(grid,grid);
	else 
	  R=Range(0,cg.numberOfComponentGrids()-1); 

        gi.inputString(answer3,sPrintF(buff,"Enter the number of ghost points on each face (%i non-negative values)",
             cg.numberOfDimensions()*2));
        if( answer3!="" )
 	{
          int numberOfGhostPoints[3][2] = {1,1,1,1,1,1};
          if( cg.numberOfDimensions()==2 )
      	    sscanf(answer3,"%i %i %i %i",
                   &numberOfGhostPoints[0][0],&numberOfGhostPoints[0][1],
		   &numberOfGhostPoints[1][0],&numberOfGhostPoints[1][1]);

          else if( cg.numberOfDimensions()==3 )
      	    sscanf(answer3,"%i %i %i %i %i %i",
		   &numberOfGhostPoints[0][0],&numberOfGhostPoints[0][1],
		   &numberOfGhostPoints[1][0],&numberOfGhostPoints[1][1],
		   &numberOfGhostPoints[2][0],&numberOfGhostPoints[2][1] );
          else
      	    sscanf(answer3,"%i %i",&numberOfGhostPoints[0][0],&numberOfGhostPoints[0][1]);
          int grid;
          for( grid=R.getBase(); grid<=R.getBound(); grid++ )
	  {
            bool valuesHaveChanged=false;
            for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	    {
              for( int side=Start; side<=End; side++ )
	      {
                if( cg[grid].numberOfGhostPoints(side,axis)!=numberOfGhostPoints[axis][side] )
		{

		  cg[grid].numberOfGhostPoints()(side,axis)=numberOfGhostPoints[axis][side];
		  cg[grid].dimension()(side,axis)=cg[grid].gridIndexRange(side,axis)
		    +numberOfGhostPoints[axis][side]*(2*side-1);
		}
	      }
	    }
	    if( valuesHaveChanged )
              geometryNeedsUpdating(grid)=true;
	  }
          if( answer2=="all" )
	  {
            grid=0;
	    printF("numberOfGhostPoints: [%i,%i] [%i,%i] [%i,%i] on all grids \n",
		   cg[grid].numberOfGhostPoints(0,0),cg[grid].numberOfGhostPoints(1,0),
		   cg[grid].numberOfGhostPoints(0,1),cg[grid].numberOfGhostPoints(1,1),
		   cg[grid].numberOfGhostPoints(0,2),cg[grid].numberOfGhostPoints(1,2));
	  }
	  else
	  {
	    for( grid=R.getBase(); grid<=R.getBound(); grid++ )
	      printF("numberOfGhostPoints: [%i,%i] [%i,%i] [%i,%i] on grid %s\n",
		     cg[grid].numberOfGhostPoints(0,0),cg[grid].numberOfGhostPoints(1,0),
		     cg[grid].numberOfGhostPoints(0,1),cg[grid].numberOfGhostPoints(1,1),
		     cg[grid].numberOfGhostPoints(0,2),cg[grid].numberOfGhostPoints(1,2),
		     (const char*)cg[grid].getName());
	  }
	  
	}
      }
      gi.unAppendTheDefaultPrompt();
    }
    else if( answer=="interpolate ghost" ||
             answer=="do not interpolate ghost" )
    {
      const bool interpolateGhost = answer=="interpolate ghost";
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	cg[grid].setUseGhostPoints(interpolateGhost);
        printF("useGhostPoints = %s on grid: %s \n",cg[grid].useGhostPoints() ? "true " : "false",
               (const char *)cg[grid].mapping().getName(Mapping::mappingName) );
        geometryNeedsUpdating(grid)=true;
      }
    }
    else if( answer=="interpolation width" )
    {
      printF("The interpolation width can be changed for any pair of grids.\n"
             "First choose the `toGrid' and then the `fromGrid'\n"
             "To change the width for all grids choose `all' and 'all'\n");
      
      gi.appendToTheDefaultPrompt("interp width>");
      int grid = gi.getMenuItem(gridMenu,answer2,"change interpolation characteristics of which grid(s)");
      if( grid<0 )
      {
	printF("Unknown response=%s\n",(const char*)answer2);
        gi.stopReadingCommandFile();
      }
      else if( answer2!="none" && answer2!="done" )
      {
        Range R;
	if( grid<cg.numberOfComponentGrids() )
	  R=Range(grid,grid);
	else 
	  R=Range(0,cg.numberOfComponentGrids()-1); 

        int gridI = gi.getMenuItem(gridMenu,answer3,"change interpolation width from which grids?");
	if( gridI<0 )
	{
	  printF("Unknown response=%s\n",(const char*)answer2);
          gi.stopReadingCommandFile();
	}
        else
	{
	  Range RI;
	  if( gridI<cg.numberOfComponentGrids() )
	    RI=Range(gridI,gridI);
	  else 
	    RI=Range(0,cg.numberOfComponentGrids()-1); 


	  gi.inputString(answer3,sPrintF(buff,"Enter the width in each direction (%i values, >= 2, default=3)",
					 cg.numberOfDimensions()));
	  if( answer3!="" )
	  {
	    IntegerArray width(3); width=3;
	    if( cg.numberOfDimensions()==2 )
	      sscanf(answer3,"%i %i",&width(0),&width(1));
	    else if( cg.numberOfDimensions()==3 )
	      sscanf(answer3,"%i %i %i",&width(0),&width(1),&width(2));
	    else
	      sscanf(answer3,"%i",&width(0));

	    if( (cg.numberOfDimensions()>1 && width(0)!=width(1)) ||
	        (cg.numberOfDimensions()>2 && width(0)!=width(2)) )
	    {
	      printF("Ogen::changeParameters::ERROR: unequal interpolation widths are currently not supported.\n"
                     " I am setting all widths to %i\n",width(0));
              width(1)=width(0);
	      width(2)=width(1);
	    }
	    else if( width(0)<1 || width(1)<1 || width(2)<1 )
	    {
	      printF("Invalid values for the width=(%i,%i,%i) (should be >=1). Try again.\n",
		     width(0),width(1),width(2));
	    }
	    else
	    {
	      Range Rx(0,cg.numberOfDimensions()-1), all;
	      for( grid=R.getBase(); grid<=R.getBound(); grid++ )
		for( gridI=RI.getBase(); gridI<=RI.getBound(); gridI++ )
		  cg.interpolationWidth(Rx,grid,gridI,all)=width(Rx);

              if( debug & 2 && myid==0 )
	        cg.interpolationWidth.display(" here is cg.interpolationWidth(axis,toGrid,fromGrid,MG level)");

	    }
	  }
	}
      }
      gi.unAppendTheDefaultPrompt();
    }
    else if( answer=="maximum distance for hole cutting" )
    {
      gi.appendToTheDefaultPrompt("hole cut dist>");
      int grid = gi.getMenuItem(gridMenu,answer2,"change the maximum distance for hole cutting of which grid(s)");
      if( grid<0 )
      {
	printF("Unknown response=%s\n",(const char*)answer2);
        gi.stopReadingCommandFile();
      }
      else if( answer2!="none" && answer2!="done" )
      {
        Range R;
	if( grid<cg.numberOfComponentGrids() )
	  R=Range(grid,grid);
	else 
	  R=Range(0,cg.numberOfComponentGrids()-1); 

        gi.inputString(answer3,sPrintF(buff,"Enter the distance for each face (%i values, left,right,...)",
				       2*cg.numberOfDimensions()));
        if( answer3!="" )
 	{
          RealArray dist(2,3); //  = cg.maximumHoleCuttingDistance;
          if( cg.numberOfDimensions()==2 )
            sScanF(answer3,"%e %e %e %e",&dist(0,0),&dist(1,0),&dist(0,1),&dist(1,1));
          else if( cg.numberOfDimensions()==3 )
            sScanF(answer3,"%e %e %e %e %e %e",&dist(0,0),&dist(1,0),&dist(0,1),&dist(1,1),&dist(0,2),&dist(1,2));
          else
            sScanF(answer3,"%e %e",&dist(0,0),&dist(1,0));
	  Range Rx(0,cg.numberOfDimensions()-1), all;
	  for( grid=R.getBase(); grid<=R.getBound(); grid++ )
	  {
	    cg.maximumHoleCuttingDistance(all,Rx,grid)=dist(all,Rx);
            printF("max hole cutting distance = ");
            for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
              printF(" [%8.2e,%8.2e] ",cg.maximumHoleCuttingDistance(0,axis,grid),
                                       cg.maximumHoleCuttingDistance(1,axis,grid));
	    printF(" for grid %s\n",(const char *)cg[grid].mapping().getName(Mapping::mappingName));
	  }
	}
      }
      gi.unAppendTheDefaultPrompt();
    }
    else if( answer=="discretization width" )
    {
      gi.appendToTheDefaultPrompt("disc. width>");
      int grid = gi.getMenuItem(gridMenu,answer2,"change the discretization width of which grid(s)");
      if( grid<0 )
      {
	printF("Unknown response=%s\n",(const char*)answer2);
        gi.stopReadingCommandFile();
      }
      else if( answer2!="none" && answer2!="done" )
      {
        Range R;
	if( grid<cg.numberOfComponentGrids() )
	  R=Range(grid,grid);
	else 
	  R=Range(0,cg.numberOfComponentGrids()-1); 

        gi.inputString(answer3,sPrintF(buff,"Enter the width in each direction (%i values, >= 3, odd)",
				       cg.numberOfDimensions()));
        if( answer3!="" )
 	{
          IntegerArray width(3); width=3;
          if( cg.numberOfDimensions()==2 )
            sScanF(answer3,"%i %i",&width(0),&width(1));
          else if( cg.numberOfDimensions()==3 )
            sScanF(answer3,"%i %i %i",&width(0),&width(1),&width(2));
          else
            sScanF(answer3,"%i",&width(0));

	  if( (cg.numberOfDimensions()>1 && width(0)!=width(1)) ||
	      (cg.numberOfDimensions()>2 && width(0)!=width(2)) )
	  {
	    printF("Ogen::changeParameters::ERROR: unequal discretization widths are currently not supported.\n"
		   " I am setting all widths to %i\n",width(0));
	    width(1)=width(0);
	    width(2)=width(1);
	  }
          else if( width(0)<3 || width(1)<3 || width(2)<3 || width(0)%2==0 || width(1)%2==0 || width(2)%2==0 )
	  {
	    printF("Invalid values for the width (should be >=3 and odd). Try again.\n");
            gi.stopReadingCommandFile();
	  }
	  else
	  {
            Range R2(0,1), Rx(0,cg.numberOfDimensions()-1), all;
            for( grid=R.getBase(); grid<=R.getBound(); grid++ )
	    {
              cg[grid].discretizationWidth()(Rx)=width(Rx);
              if( cg.numberOfDimensions()==2 )
                printF("discretization widths = (%i,%i)",width(0),width(1));
	      else if( cg.numberOfDimensions()==3 )
                printF("discretization widths = (%i,%i,%i)",width(0),width(1),width(2));
	      else 
                printF("discretization width = %i",width(0));
              printF(" for grid %s\n",(const char *)cg[grid].mapping().getName(Mapping::mappingName));

              // *wdh* 080325 cg[grid].numberOfGhostPoints()(Range(0,1),Range(0,2))=max(cg[grid].numberOfGhostPoints(),2);
              int numGhost=max(2,max(width(Rx))/2);
              cg[grid].numberOfGhostPoints()(Range(0,1),Range(0,2))=max(cg[grid].numberOfGhostPoints(),numGhost);

              // *wdh* 080325  : We should set the boundaryDiscretizationWidth to be at least as large as the
              //                 discretization with 
              for( int dir=0; dir<cg.numberOfDimensions(); dir++ )for( int side=0; side<=1; side++ )
	      {
		cg[grid].setBoundaryDiscretizationWidth(side,dir, width(dir));
	      }
              // cg[grid].boundaryDiscretizationWidth().display("**** boundaryDiscretizationWidth ***");
	    }
            printF("I am setting number of ghost points to be at least dw/2. \n");
            printF("I am setting the boundary discretization width to match the discretization width.\n");
	  }
	}
      }
      gi.unAppendTheDefaultPrompt();
    }
    else if( answer=="boundary discretization width" )
    {
      gi.appendToTheDefaultPrompt("bndy-disc-width>");
      int grid = gi.getMenuItem(gridMenu,answer2,"change the boundary discretization width of which grid(s)");
      if( grid<0 )
      {
	printF("Unknown response=%s\n",(const char*)answer2);
        gi.stopReadingCommandFile();
      }
      else if( answer2!="none" && answer2!="done" )
      {
        Range R;
	if( grid<cg.numberOfComponentGrids() )
	  R=Range(grid,grid);
	else 
	  R=Range(0,cg.numberOfComponentGrids()-1); 

        gi.inputString(answer3,sPrintF(buff,"Enter the widths for each side (%i values)",
				       cg.numberOfDimensions()*2));
        if( answer3!="" )
 	{
          IntegerArray width(2,3); width=3;
          if( cg.numberOfDimensions()==1 )
            sScanF(answer3,"%i %i",&width(0,0),&width(1,0));
          else if( cg.numberOfDimensions()==2 )
            sScanF(answer3,"%i %i %i %i",&width(0,0),&width(1,0),&width(0,1),&width(1,1));
          else
            sScanF(answer3,"%i %i %i %i %i %i",&width(0,0),&width(1,0),&width(0,1),&width(1,1),
                      &width(0,2),&width(1,2));

	  Range R2(0,1), Rx(0,cg.numberOfDimensions()-1), all;
	  for( grid=R.getBase(); grid<=R.getBound(); grid++ )
	  {
	    cg[grid].boundaryDiscretizationWidth()(R2,Rx)=width(R2,Rx);
	    if( cg.numberOfDimensions()==2 )
	      printF("boundary discretization widths = (%i,%i) (%i,%i)",width(0,0),width(1,0),width(0,1),width(1,1));
	    else if( cg.numberOfDimensions()==3 )
	      printF("boundary discretization widths = (%i,%i) (%i,%i) (%i,%i)",width(0,0),width(1,0),width(0,1),width(1,1),
                     width(0,2),width(1,2));
	    else 
	      printF("boundary discretization width = (%i,%i)",width(0,0),width(1,0));
	    printF(" for grid %s\n",(const char *)cg[grid].mapping().getName(Mapping::mappingName));
	  }
	}
      }
      gi.unAppendTheDefaultPrompt();
    }
    else if( answer=="cell centering" )
    {
      gi.appendToTheDefaultPrompt("cell centering>");
      aString menu[]=
      { 
	"cell centered for all grids",
	"vertex centered for all grids",
        "no change",
	"" 
      }; 
      gi.getMenuItem(menu,answer2,"Choose an option");
      
      if( answer2=="cell centered for all grids" )
      {
        cg.changeToAllCellCentered();
        geometryNeedsUpdating=true;

        printF("The grid is now cell centered\n");
      }
      else if( answer2=="vertex centered for all grids" )
      {
        cg.changeToAllVertexCentered();
        geometryNeedsUpdating=true;
        printF("The grid is now vertex centered\n");
      }
      else
       printF("No changes made to cell centering\n");
      gi.unAppendTheDefaultPrompt();
    }
    else if( answer=="specify a domain" )
    {
      printF("Specify grids that belong to a domain.\n"
             " A domain is an independent region where one might solve a different equation.\n"
             " A grid may belong to only one domain.\n"
             " The grids in a domain are treated as a separate overlapping grid - grids in a domain\n"
             "  will interpolate from and cut holes in grids from the same domain only.\n");

      gi.appendToTheDefaultPrompt("specify domain>");

      // **** we probably want to assign a domain name --- keep count of domains using cg.numberOfDomains()
      //   add a "reset domains" to put all grids back into domain 0

      // The first time thru we will assign domain 0:
      int domain = !domainsHaveBeenAssigned ? 0 : cg.numberOfDomains(); // here is the new domain number

      aString domainName=sPrintF("domain%i",domain);
      gi.inputString(domainName,sPrintF(buff,"Enter the domain name (default=%s)",(const char*)domainName));
//       sScanF(answer2,"%i",&domain);
//       if( domain<0 )
//       {
// 	printF("Invalid value for domain=%i, this should be greater than or equal to 0\n",domain);
// 	gi.stopReadingCommandFile();
// 	break;
//       }
      for( ;; )
      {
	int grid = gi.getMenuItem(gridMenu,answer2,"Choose grids to add to this domain");
        if( answer2=="done" )
          break;
        Range G;  // range of grids to add to this domain
        if( answer2=="all" )
	{
	  G=cg.numberOfComponentGrids();
	}
        else if( grid<0 || grid>=cg.numberOfComponentGrids() )
	{
  	  printF("Unknown response=%s\n",(const char*)answer2);
          gi.stopReadingCommandFile();
	  break;
	}
	else
	{
	  G=Range(grid,grid);
	}
	
	assert( domain>=0 );
	cg.domainNumber(G)=domain;
        
      }
      if( domainsHaveBeenAssigned )
        cg->numberOfDomains++;  // increase the number of domains
      domainsHaveBeenAssigned=true;

      cg.setDomainName( domain,domainName );
      
      gi.unAppendTheDefaultPrompt();

      // now update parameters:
      //   prevent hole cutting in grids from other domains
      //   prevent interpolation from grids in other domains

      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	if( cg.domainNumber(grid)==domain )
	{
          for( int grid2=0; grid2<cg.numberOfComponentGrids(); grid2++ )
	  {
            if( cg.domainNumber(grid2)==domain )
	    { // grid and grid2 belong to the same domain

              // do not change the default since the user may have changed these for other reasons:
   	      // cg.mayCutHoles(grid,grid2)=true;         // grid may cut holes in grid2 (same domain)
   	      // cg.mayCutHoles(grid2,grid)=true;   
	      // cg.mayInterpolate(grid,grid2,all)=true;  // grid may interpolate from grid2
	      // cg.mayInterpolate(grid2,grid,all)=true; 
	    }
	    else
	    { // grid and grid2 belong to different domains
              if( debug & 4 )
		printF("Info: grids %s may NOT cut holes nor interpolate from grid %s"
		       " and vice-versa (different domains).\n",
		       (const char*)cg[grid].getName(),(const char*)cg[grid2].getName());
   	      cg.mayCutHoles(grid,grid2)=false;          // grid may NOT cut holes in grid2 (different domains)
   	      cg.mayCutHoles(grid2,grid)=false;    
	      cg.mayInterpolate(grid,grid2,all)=false;   // grid may NOT interpolate from grid 2
	      cg.mayInterpolate(grid2,grid,all)=false;
	    }
	  }
	}
      }
      if( true )
      {
	for( int d=0; d<cg.numberOfDomains(); d++ )
	{
	  printF("INFO: domain %i is named %s\n",d,(const char*)cg.getDomainName(d));
	}
      }
    }
    else if( answer=="reset domains" )
    {
      printF("Info: reseting all grids back to domain 0. \n"
             "      resetting `may cut holes' and `may interpolate' to true for all grids\n");

      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
        cg.domainNumber(grid)=0;      // all grids are in domain 0
      cg.mayCutHoles=true;
      cg.mayInterpolate=true;
    }

    else if( answer=="improve quality of interpolation" )// *OLD* WAY
    {
      improveQualityOfInterpolation=!improveQualityOfInterpolation;
      if( improveQualityOfInterpolation )
        printF("Improve quality of interpolation is now ON, qualityBound=%5.2e\n",qualityBound);
      else
        printF("Improve quality of interpolation is now OFF\n");
    }
    else if( dialog.getToggleValue(answer,"improve quality of interpolation",improveQualityOfInterpolation) )
    {
      if( improveQualityOfInterpolation )
        printF("Improve quality of interpolation is now ON, qualityBound=%5.2e\n",qualityBound);
      else
        printF("Improve quality of interpolation is now OFF\n");
    }

    else if( dialog.getTextValue(answer,"improve quality algorithm:","%i",qualityAlgorithm) )
    {
      printF("Setting qualityAlgorithm=%i. 0=relative-area (old), 1=area+distance-to-boundary (new).\n",
          qualityAlgorithm);
    }

    else if (answer=="set quality bound" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the quality bound (larger than 1.) (current=%5.2e)\n",
                     qualityBound));
      if( answer2!="" )
	sScanF(answer2,"%e",&qualityBound);
    }
    else if (answer=="maximum number of points to invert at a time" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the maximum number of points to invert at one time (current=%i)\n",
                     maximumNumberOfPointsToInvertAtOneTime));
      if( answer2!="" )
	sScanF(answer2,"%i",&maximumNumberOfPointsToInvertAtOneTime);
      printF("Setting: maximumNumberOfPointsToInvertAtOneTime=%i\n",maximumNumberOfPointsToInvertAtOneTime);
    }
    else if( answer=="show parameter values" )
    {
      displayCompositeGridParameters( cg );
      if( cg.numberOfDomains()>1 )
      {
        cg.update( CompositeGrid::THEdomain );
	for( int d=0; d<cg.numberOfDomains(); d++ )
	{
	  printF("\n ************ domain %i (%s) ***************\n",d,(const char*)cg.getDomainName(d));
	  displayCompositeGridParameters( cg.domain[d] );
	}
      }
    }
    else if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }
    

  }

  //  Check that there are enough parallel ghost lines for ogen: 
  checkParallelGhostWidth( cg );

  updateParameters(cg,-1,minimumOverlap);
  
//   if( true )
//   {
//     ::display(cg.interpolationIsImplicit,"cg.interpolationIsImplicit AFTER change parameters");
//     ::display(cg.interpolationWidth,"cg.interpolationWidth AFTER change parameters");
//     ::display(cg.interpolationOverlap,"cg.interpolationOverlap AFTER change parameters");
//   }
  
  gi.popGUI(); // restore the previous GUI

  delete [] gridMenu;
  delete [] gridMenuNoAll;
  gi.unAppendTheDefaultPrompt();
  return 0;
}


int Ogen::
updateParameters(CompositeGrid & cg, const int level /* = -1 */,
                 const RealArray & minimumOverlap /* =  Overture::nullRealArray() */ )
// ==============================================================================================
// /Description:
//   Recompute the parameters that depend on the user specified parameters
//
// /level (input): update parameters for this multigrid level. By default update all levels.
// /minimumOverlap (input): optionally specify the minimum overlap
//
// /NOTE: I use the cg.backupInterpolationConditionLimit to hold the minumum overlap if it was set
//       int changeParameters (if it is >0 )
// ==============================================================================================
{
  
  const int numberOfBaseGrids = cg.numberOfBaseGrids();

  const int startLevel = level==-1 ? 0 : level;
  const int endLevel   = level==-1 ? cg.numberOfMultigridLevels()-1 : level;
  Range all,L(startLevel,endLevel);
  
  if( minimumOverlap.getLength(0)>0 )
    cg.interpolationOverlap(all,all,all,L)= minimumOverlap(all,all,all,L);
  else
    cg.interpolationOverlap(all,all,all,L)=.5;              // default minimum overlap

//  cg.backupInterpolationOverlap(all,all,all,L)=.1;
//  cg.backupInterpolationIsImplicit(all,all,L)=true;
//  cg.backupInterpolationWidth(all,all,all,L)=cg.interpolationWidth(all,all,all,L);

  
  for( int l=startLevel; l<=endLevel; l++ )
  {
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      for( int grid2=0; grid2<cg.numberOfComponentGrids(); grid2++ )
      {
	MappedGrid & g2= cg[grid2];
      
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	{
	  int m21 = g2.extendedIndexRange(1,axis) - g2.extendedIndexRange(0,axis) + 1;
	  cg.interpolationWidth(axis,grid,grid2,l) = min(cg.interpolationWidth(axis,grid,grid2,l), m21);

          // if the user has specified a new min overlap, it is saved in cg.backupInterpolationConditionLimit
//	  if( cg.backupInterpolationConditionLimit(grid,grid2,l) > 0. )
//	    cg.interpolationOverlap(axis,grid,grid2,l)=cg.backupInterpolationConditionLimit(grid,grid2,l);

          int width;
          // *wdh* 000905 : amr grids are allowed to be implicit from each other
          // *wdh* 061102 : amr grids are allowed to be implicit from base grids too (since these are done first)
          if( cg.interpolationIsImplicit(grid,grid2,l) || cg.refinementLevelNumber(grid)>0 )
//              || ( cg.refinementLevelNumber(grid)>0 && cg.refinementLevelNumber(grid2)>0) ) 
	    width = max(cg.interpolationWidth(axis,grid,grid2,l),g2.discretizationWidth(axis)) - 2;
	  else
            width=cg.interpolationWidth(axis,grid,grid2,l) + g2.discretizationWidth(axis) - 3;
	    
  	  cg.interpolationOverlap(axis,grid,grid2,l) =
  	    max(cg.interpolationOverlap(axis,grid,grid2,l), .5*width );
	  // *wdh* I don't think a minimum value is needed -- it produced incorrect results with cicie
	  // *wdh* 000825 min(  max(cg.interpolationOverlap(axis,grid,grid2,l), .5*width ), .5*(m21 - 2));

	}
      }
    }
  }
  return 0;
}


//! Ogen utility function: Choose a side of a grid.
int Ogen::
chooseASide( MappedGrid & mg, int & side, int & axis )
{
  int returnValue=0;
  
  assert( ps!=NULL );
  
  GenericGraphicsInterface & gi = *ps;
  gi.appendToTheDefaultPrompt("choose side>");

  IntegerArray bc; bc = mg.boundaryCondition();
  aString answer;
  side=-1, axis=-1;

  int response=-1;
  if( mg.numberOfDimensions()==2 )
  {
    aString sideMenu[]=
    {
      "left   (side=0,axis=0)",
      "right  (side=1,axis=0)",
      "bottom (side=0,axis=1)",
      "top    (side=1,axis=1)",
      ""
    };

    printF("Grid %s : boundary conditions are (%i,%i, %i,%i)\n",
	   (const char*)mg.getName(),bc(0,0),bc(1,0),bc(0,1),bc(1,1));  

    response=gi.getMenuItem(sideMenu,answer,"Set which side?");
  }
  else
  {
    aString sideMenu[]=
    {
      "left   (side=0,axis=0)",
      "right  (side=1,axis=0)",
      "bottom (side=0,axis=1)",
      "top    (side=1,axis=1)",
      "front  (side=0,axis=2)",
      "back   (side=1,axis=2)",
      ""
    };
    printF("Grid %s : boundary conditions are (%i,%i, %i,%i, %i,%i)\n",
	   (const char*)mg.getName(),
	   bc(0,0),bc(1,0),bc(0,1),bc(1,1),bc(0,2),bc(1,2));
    response=gi.getMenuItem(sideMenu,answer,"Set which side?");
  }
  
  if( response<0 || response> (2*mg.numberOfDimensions()-1) )
  {
    printF("Unknown response=%s\n",(const char*)answer);
    returnValue=1;
  }
  else
  {
    side=response %2;
    axis = response/2;
  }
  gi.unAppendTheDefaultPrompt();

  return returnValue;
}

  
