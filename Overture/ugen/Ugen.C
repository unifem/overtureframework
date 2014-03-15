//#define OV_DEBUG
//#define BOUNDS_CHECK
//kkc 081124 #include <iostream.h>
#include <iostream>

#include "Overture.h"
#include "OvertureDefine.h"
#include "Ugen.h"
#include "AssertException.h"
#include "UnstructuredMapping.h"
#include "HyperbolicMapping.h"
#include "ReductionMapping.h"
#include "SquareMapping.h"
#include "BoxMapping.h"
#include "MappedGridFunction.h"
#include "CompositeGridFunction.h"
#include "CompositeGridOperators.h"
#include "CompositeSurface.h"
#include "interpPoints.h"
#include "MeshQuality.h"

#include "optMesh.h"
#include "Geom.h"

using namespace std;

//extern void optimize(UnstructuredMapping &umap, RealCompositeGridFunction *cf);
 void optimize(UnstructuredMapping &umap, RealCompositeGridFunction *cf)
 { 
   if (cf)
     {
       MetricCGFunctionEvaluator me(cf);
       optimize(umap, me);
     }
   else
     {
       IdentityMetricEvaluator me;
       optimize(umap,me); 
     }
 }
 

const int debug_Ugen = true; // set to false to turn many of the AssertExceptions off

static void errorReport(GenericGraphicsInterface &ps, const aString &msg)
{
  // note casting away of const, these two methods should not change the string!
  aString & msg_out = (aString &) msg;
  ps.outputString(msg_out);
  ps.createMessageDialog(msg_out, errorDialog);
}

static void createBackgroundMappings(const CompositeGrid &cg, MappingInformation &mapInfo_in)
{
  // create a list of surface mappings from the composite grid.
  //   in some cases, like the hyperbolic mapping, look for an underlying mapping so
  //   we do not have to rely on a DatapointMapping.
#if 1
  CompositeSurface *cs = new CompositeSurface();
  MappingInformation mapInfo;
#endif

  for ( int grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      const MappingRC & map = cg[grid].mapping();
      map.mapPointer->getBoundingBox();
      if ( map.getClassName()=="HyperbolicMapping" )
	{
	  //look for an underlying surface
	  // ( does the HyperbolicMapping class always have a surface underneath? )
	  // XXX should we check for duplicate mappings being inserted into mapInfo ?
	  const HyperbolicMapping &hmap = *((const HyperbolicMapping *)map.mapPointer);
	  if ( hmap.getSurface()!=NULL )
	    mapInfo.mappingList.addElement( MappingRC((Mapping&)*(hmap.getSurface())) );
	  else 
	    mapInfo.mappingList.addElement(map);
	}
      if ( map.getClassName()=="ReductionMapping" )
	{
	  // same issue, but it may be a HyperbolicMapping underneath...
	  const ReductionMapping & rmap = (const ReductionMapping &)(*map.mapPointer);
	  const Mapping *omap = (rmap.getOriginalMapping());
	  
	  if ( omap->getClassName()=="HyperbolicMapping" )
	    {
	      cout<<"reduced hyperbolic mapping "<<endl;
	      if ( rmap.getInActiveAxis(2)==2 && rmap.getInActiveAxisValue(2)==0. )
		{
		  cout<<"reduced to original surface"<<endl;
		  const HyperbolicMapping &hmap = *((const HyperbolicMapping *)omap);
		  if ( hmap.getSurface()!=NULL )
		    mapInfo.mappingList.addElement( MappingRC((Mapping&)*(hmap.getSurface())) );
		  else 
		    mapInfo.mappingList.addElement(map);
		}
	      else
		mapInfo.mappingList.addElement(map);
	    }
	  else
      {
	    mapInfo.mappingList.addElement(map);
      }
	}
      else
	mapInfo.mappingList.addElement(map);
      
      cout<<"mapping for grid "<<grid<<" is a "<<mapInfo.mappingList[mapInfo.mappingList.getLength()-1].getClassName()<<endl;
    }
  
  cout<<"INFO:: found "<<mapInfo.mappingList.getLength()<<" background mappings"<<endl;
#if 1
  for ( int m=0; m<mapInfo.mappingList.getLength(); m++ )
    {
      mapInfo.mappingList[m].mapPointer->incrementReferenceCount();
      cs->add(*(mapInfo.mappingList[m].mapPointer));
      Mapping &map = (*cs)[m];
      if ( map.getClassName()=="ReductionMapping" )
	   {
	      ReductionMapping & rmap = (ReductionMapping &)(map);

          if ( rmap.getInActiveAxisValue(0)>.9 ) 
              cs->getSignForNormal(m) = -1; 
          else
              cs->getSignForNormal(m) = 1; 

          if ( rmap.getInActiveAxis(0)==1 ) cs->getSignForNormal(m) *= -1; 
       }

      cout<<"sign for normal for surface "<<m<<" = "<<cs->getSignForNormal(m)<<endl;

    }
  
  cs->recomputeBoundingBox();
  cs->incrementReferenceCount();
  mapInfo_in.mappingList.addElement(*cs);
  if ( (cs->decrementReferenceCount()==0) ) delete cs;
#endif
}

static void buildUgenInterfaces(GUIState &basicInterface, GUIState &abortInterface)
{

  aString basicButtons[][2] = { {"continue generation","Generate"},
				{"enlarge hole", "Enlarge Hole"},
				{"enlarge front", "Expand Front"},
				{"reset hole", "Reset Hole"},
				//{"status", "Status"},
				{"exit", "Exit"},
				{"" , ""} };

  aString abortButtons[][2] = {  {"reset hole","Reset"},
				 {"exit", "Exit"},
				 {"", ""} };

  basicInterface.setUserButtons(basicButtons);
  abortInterface.setUserButtons(abortButtons);

  basicInterface.setOptionMenuColumns(1);
  aString optionMenu[] = { "Triangle...", "AdvancingFront...", "" };
  aString optionMenuCom[] = { "use triangle", "use advancing front", ""};

  basicInterface.addOptionMenu("Algorithm", optionMenuCom, optionMenu, 1);

  basicInterface.setWindowTitle("Hybrid Mesh Generator");

  // toggle buttons
  aString toggleCommands[] = { "plot component grids (toggle)", "plot triangle", "plot advancing front", ""};
  aString toggleLabels[] = { "Plot Component Grids", "Plot Delaunay Triangulation", "Plot Advancing Front","" };
  int toggleState[3];
  toggleState[0] = 0;
  toggleState[1] = 0;
  toggleState[2] = 1;
  basicInterface.setToggleButtons( toggleCommands, toggleLabels, toggleState );
  abortInterface.setToggleButtons( toggleCommands, toggleLabels, toggleState );

  // pulldown menu
  aString pulldown[] = {"Component Grids...",
			"Advancing Front...",
			"Delaunay Mesh...",
			""};

  basicInterface.setUserMenu(pulldown,"Change Plot");
  abortInterface.setUserMenu(pulldown,"Change Plot");

  aString textCommands[] = {"Enlarge Grid Holes",""};
  aString textStrings[] = {"",""};
  basicInterface.setTextBoxes(textCommands,textCommands,textStrings);

  // exit stuff
  basicInterface.setExitCommand("exit", "Exit");
  abortInterface.setExitCommand("exit", "Exit");
}

static void setupAdvFrontDialog( AdvancingFrontParameters & params, DialogData &dia )
{
  aString textCommands[] = {"maximum neighbor angle", "edge growth factor", "number of advances", "quality tolerance", "print face",""};
  aString textLabels[] = {"Maximum neighbor Angle", "Edge Growth Factor (<0 for no edge stretching)", "Number of Advances (<0 for no plotting)", "Quality Tolerance", "Print Face Verts",""};
  aString textStrings[5];
  sPrintF(textStrings[0], "%f", params.getMaxNeighborAngle());
  sPrintF(textStrings[1], "%f", params.getEdgeGrowthFactor());
  sPrintF(textStrings[2], "%i", params.getNumberOfAdvances());
  sPrintF(textStrings[3], "%f", params.getQualityTolerance());
  sPrintF(textStrings[4], "%i", -1);
  dia.setTextBoxes(textCommands, textLabels, textStrings);

}

static void setupTriangleWrapperDialog( TriangleWrapperParameters & params, DialogData &dia )
{
  aString textCommands[] = {"maximum area", "minimum angle", ""};
  aString textLabels[] = {"Maximum Area (<0 for auto)", "Minimum Angle (<0 for auto)", ""};

  aString textStrings[2];
  sPrintF(textStrings[0], "%f", params.getMaximumArea());
  sPrintF(textStrings[1], "%f", params.getMinimumAngle());

  dia.setTextBoxes(textCommands, textLabels, textStrings);
}

// this class should really be just a bunch of methods in a namespace ..?

//\begin{>Ugen.tex}{\subsection{Default Constructor}}
Ugen::
Ugen() : delaunayMesh(2,2, Mapping::parameterSpace, Mapping::cartesianSpace)
// /Purpose : build and initialize a {\tt Ugen}, make the {\tt GenericGraphicsInterface} pointer NULL
//\end{Ugen.tex}
{

  ps = NULL;
  initialize();

}

//\begin{>>Ugen.tex}{\subsection{Constructor}}
Ugen::
Ugen(GenericGraphicsInterface & ps_) : delaunayMesh(2,2, Mapping::parameterSpace, Mapping::cartesianSpace)
// /Purpose : build and initialize a {\tt Ugen} given an instance of a {\tt GenericGraphicsInterface}
// /ps\_ (input) : reference to the {\tt GenericGraphicsInterface} that the {\tt Ugen} should use for plots
//\end{Ugen.tex}
{
  ps = &ps_;
  initialize();
}

Ugen::
~Ugen()
{
}

//\begin{>>Ugen.tex}{\subsection{updateHybrid}}
void 
Ugen::
updateHybrid(CompositeGrid & cg, 
	     MappingInformation & mapInfo)
// /Purpose : generate or a update hybrid mesh given an overlapping {\tt CompositeGrid}
// /cg (input) : {\tt CompositeGrid} on which to construct a hybrid mesh
// /mapInfo (input) : currently only provides a {\tt GenericGraphicsInterface} instance for plotting
// /Description : {\tt updateHybrid} performs the following tasks, primarily through private and protected methods
// \begin{itemize}
//  \item strips away the overlap in {\tt cg}
//  \item determines the faces comprising the initial holes
//  \item initializes an {\tt AdvancingFront} with the faces determined above
//  \item optionally generates or destroys mesh stretching influences on the {\tt AdvancingFront}
//  \item provides the {\tt GenericGraphicsInterface}/command line interface for plotting and interactive mesh generation
// \end{itemize}
// Currently, this method catches all exceptions arising from failures in the {\tt AdvancingFront}'s
// mesh generation algorithms.  Low level data structure errors are, in general, not caught and are
//  thrown through to the calling scope. It is assumed that if a low level error occurs (say in 
// a {\tt GeometricADT} or, worse, an {\tt NTreeNode}, that the state is corrupt enough 
// that {\tt updateHybrid} cannot recover.  Generally, {\tt updateHybrid} will recover from a mesh
// generation error by plotting the current state of the hybrid mesh and allowing a limited set of
//  {\tt Ugen} manipulations.
//\end{Ugen.tex}
{

  // initialize default generator type
  GeneratorType genWith = AdvFront;

  //
  // these are the parameters for the mesh generators
  //

  AdvancingFrontParameters & advFrontParameters = advancingFront.getParameters();
  TriangleWrapperParameters & triangleWrapperParameters = (TriangleWrapperParameters &)triangleWrapper.getParameters();

  //
  // get and set up the graphics interface
  //
  if ( ps!=NULL )
    {
      assert(mapInfo.graphXInterface!=NULL);
      
      ps = mapInfo.graphXInterface;
    }

  GenericGraphicsInterface &gi = *ps;
  gi.appendToTheDefaultPrompt("ugen>");

  GUIState basicInterface,  // default interface, when everything is ok
           abortInterface;  // use if the mesh generation algorithm fails

  // build the popup menu and user buttons for the basic and abort interface
  buildUgenInterfaces(basicInterface, abortInterface);

  // initialize dialog siblings for the mesh generation parameters
  DialogData & advFrontDialog = basicInterface.getDialogSibling();
  advFrontDialog.setWindowTitle("AdvancingFront Parameters");
  setupAdvFrontDialog(advFrontParameters, advFrontDialog);
  advFrontDialog.setExitCommand("close advancing front dialog", "Close");

  DialogData & triangleWrapperDialog = basicInterface.getDialogSibling();
  triangleWrapperDialog.setWindowTitle("TriangleWrapper Parameters");
  setupTriangleWrapperDialog(triangleWrapperParameters, triangleWrapperDialog);
  triangleWrapperDialog.setExitCommand("close triangle dialog", "Close");

  // keep track dialogs that are open
  bool algorithmDialogOpen = false;
  // ok, the interface is ready so push it onto the stack
  bool aborted = false;
  gi.pushGUI(basicInterface);

  preprocessCompositeGridMasks(cg);

  if ( debug_Ugen ) cout<<"preprocessing of masks completed"<<endl;

  // the following 4 arrays should ultimately reside as mapping arrays in the composite grid
  intArray *vertexIndex = new intArray[ cg.numberOfComponentGrids() ];
  intArray numberOfVertices( cg.numberOfComponentGrids() );
  intArray vertexGridIndexMap;
  intArray *gridIndexVertexMap;
  gridIndexVertexMap = new intArray[cg.numberOfComponentGrids()];
  intArray *vertexIDMap = new intArray[cg.numberOfComponentGrids() ];
  realArray xyz_initial;

  intArray initialFaceList; // list of the faces on the hole boundary
  intArray initialFaceZones;// list of the structured grid zones adjacent to the faces on the hole

  int rangeDimension = cg.numberOfDimensions();
  int domainDimension = cg.numberOfGrids()>0 ? cg[0].domainDimension() : rangeDimension;

  aString answer;

  try {
    initializeGeneration(cg, vertexIndex, numberOfVertices, vertexIDMap, vertexGridIndexMap, gridIndexVertexMap, initialFaceList, initialFaceZones, xyz_initial);
  } catch (AbstractException &e) {
    e.debug_print();
    errorReport(gi, "ERROR: could not initialize hybrid mesh generator");
    answer = "exit";
  }

  int advanceNum = 100;
  bool plotObject = true;
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT, TRUE);
  
  if( domainDimension!=rangeDimension )
  {
    // For surface grids, plot unstructured grid by default *wdh* 070223
    advFrontParameters.toggle(AdvancingFrontParameters::meshEdges);
    advFrontParameters.toggle(AdvancingFrontParameters::meshFaces);
  }
  
  char buff[180]; // buffer for error message
  bool plotComponentGrids = false;
  bool plotTriangle = false;
  bool plotAdvFront = true;
  int len;
  for (int it=0;; it++)
    {

      if (it==0 && answer!="exit")
	answer = "plot object";
      else if(answer!="exit")
	gi.getAnswer(answer, "");


      if (answer == "set plotting frequency (<1 for never)") 
	{
	  gi.inputString(answer, "Enter the number of front advances between plots (<1 for plot when finished)");
	  if (answer!="")
	    {
	      sScanF(answer, "%i", &advanceNum);
	      advFrontParameters.setNumberOfAdvances(advanceNum);
	    }
	}
      else if (answer == "continue generation")
	{

	  try {
	    if ( genWith==AdvFront )
	      generateWithAdvancingFront();
	    else if ( genWith==TriWrap )
	      {
		if ( rangeDimension==2 )
		  generateWithTriangle();
		else
		  errorReport(gi, "ERROR: triangle only works with 2D");
	      }
	  } 
	  catch ( AbstractException &e ) {
	    e.debug_print();
	    gi.popGUI();
	    aborted = true;
	    gi.pushGUI(abortInterface);
	  }
	  // any other errors are considered unknown causes of fatality

	  plotObject = true;
	  
	}
      else if (answer == "enlarge hole")
	{
	  enlargeHole(cg, vertexGridIndexMap);
	  advancingFront.destroyFront();
	  initializeGeneration(cg, vertexIndex, numberOfVertices, vertexIDMap, vertexGridIndexMap, gridIndexVertexMap, initialFaceList, initialFaceZones, xyz_initial);
	  plotObject = true;
	}
      else if ( int len=answer.matches("Enlarge Grid Hole") )
	{
	  int egrd;
	  int nread = sScanF(&answer[len],"%d",&egrd);
	  enlargeHole(cg,vertexGridIndexMap,egrd);
	  advancingFront.destroyFront();
	  initializeGeneration(cg, vertexIndex, numberOfVertices, vertexIDMap, vertexGridIndexMap, gridIndexVertexMap, initialFaceList, initialFaceZones, xyz_initial);
	  plotObject = true;
	}
      else if ( answer=="enlarge front")
	{
	  advancingFront.expandFront();
	  plotObject = true;
	}
      else if (answer == "reset hole")
	{
	  advancingFront.destroyFront();
	  initializeGeneration(cg, vertexIndex, numberOfVertices, vertexIDMap, vertexGridIndexMap, gridIndexVertexMap, initialFaceList, initialFaceZones, xyz_initial);
	  plotObject = true;
	  if ( aborted ) 
	    {
	      gi.popGUI();
	      gi.pushGUI(basicInterface);
	      aborted = false;
	    }
	  //menu = basicMenu;
	}
      else if (answer(0,28) == "plot component grids (toggle)") 
	{
	  plotComponentGrids = !plotComponentGrids;
	  plotObject = true;
	}
      //      else if ( answer(0,29) == "plot control function (toggle)" )
      //	{
      //	  plotControlFunction = !plotControlFunction;
      //	  plotObject = true;
      //	}
      else if (answer(0,12) == "plot triangle")
	{
	  plotTriangle = !plotTriangle;
	  plotObject = true;
	}
      else if (answer(0,19) == "plot advancing front")
	{
	  plotAdvFront = !plotAdvFront;
	  plotObject = true;
	}
      else if( answer=="open graphics" )  // *wdh*
      {
        if( !gi.graphicsIsOn() )
          gi.createWindow("ugen: Hybrid Grid Generator");
      }
      else if (answer == "plot object")
	{
	  plotObject = true;
	}
      else if (answer(0,11) == "use triangle")
	{
	  if ( algorithmDialogOpen && genWith!=TriWrap) advFrontDialog.hideSibling();
	  basicInterface.getOptionMenu(0).setCurrentChoice(0);
	  genWith = TriWrap;
	  algorithmDialogOpen = true;
	  triangleWrapperDialog.showSibling();
	  triangleWrapperDialog.setSensitive(1);
	  if ( rangeDimension != 2 ) // only works in 2D
	    {
	      triangleWrapperDialog.setSensitive(0);
	      errorReport(gi,"ERROR: triangle only works in 2D");
	    }
	      
	}
      else if (answer(0,18) == "use advancing front")
	{
	  if ( algorithmDialogOpen && genWith!=AdvFront) triangleWrapperDialog.hideSibling();
	  basicInterface.getOptionMenu(0).setCurrentChoice(1);
	  genWith = AdvFront;
	  algorithmDialogOpen = true;
	  advFrontDialog.showSibling();
	}
      else if (answer(0,11) == "maximum area")
	{
	  real newMaxArea;
	  aString buff;
	  int nRead = sScanF(&answer[12], "%f", &newMaxArea);
	  if ( nRead < 1 )
	    {
	      errorReport(gi, "ERROR: invalid input");
	      sPrintF(buff, "%f", triangleWrapperParameters.getMaximumArea());
	      triangleWrapperDialog.setTextLabel(0, buff);
	    }
	  else
	    {
	      triangleWrapperParameters.setMaximumArea(newMaxArea);
	      sPrintF(buff, "%f", newMaxArea);
	      triangleWrapperDialog.setTextLabel(0, buff);
	    }
	}
      else if (answer(0,12) == "minimum angle")
	{
	  real newMinAngle;
	  aString buff;
	  int nRead = sScanF(&answer[13], "%f", &newMinAngle);
	  if ( nRead < 1 )
	    {
	      errorReport(gi, "ERROR: invalid input");
	      sPrintF(buff, "%f", triangleWrapperParameters.getMinimumAngle());
	    }
	  else
	    {
	      triangleWrapperParameters.setMinimumAngle(newMinAngle);
	      sPrintF(buff, "%f", newMinAngle);
	    }
	  triangleWrapperDialog.setTextLabel(1, buff);
	}
      else if (answer(0,21) == "maximum neighbor angle")
	{
	  real val;
	  aString buff;
	  int nRead = sScanF(&answer[22], "%f", &val);
	  if ( nRead<1 )
	    {
	      errorReport(gi, "ERROR: invalid input");
	      sPrintF(buff, "%f", advFrontParameters.getMaxNeighborAngle());
	    }
	  else
	    {
	      advFrontParameters.setMaxNeighborAngle(val);
	      sPrintF(buff, "%f", val);
	    }
	  advFrontDialog.setTextLabel(0,buff);    
	} 
      else if (answer(0,17) == "edge growth factor")
	{
	  real val;
	  aString buff;
	  int nRead = sScanF(&(answer[18]), "%f", &val);
	  if ( nRead<1 )
	    {
	      errorReport(gi, "ERROR: invalid input");
	      sPrintF(buff, "%f", advFrontParameters.getEdgeGrowthFactor());
	    }
	  else
	    {
	      advFrontParameters.setEdgeGrowthFactor(val);
	      sPrintF(buff, "%f", val);
	    }
	  advFrontDialog.setTextLabel(1,buff);    
	}
      else if (answer(0,17) == "number of advances")
	{
	  int val;
	  aString buff;
	  int nRead = sScanF(&answer[18], "%i", &val);
	  if ( nRead<1 )
	    {
	      errorReport(gi,"ERROR: invalid input");
	      sPrintF(buff, "%i", advFrontParameters.getNumberOfAdvances());
	    }
	  else
	    {
	      advFrontParameters.setNumberOfAdvances(val);
	      sPrintF(buff, "%i", val);
	    }
	  advFrontDialog.setTextLabel(2,buff); 
	}
      else if ( (len=answer.matches("quality tolerance")) )
	{
	  real val;
	  aString buff;
	  int nRead = sScanF(&answer[len], "%f", &val);
	  advFrontParameters.setQualityTolerance(val);
	  sPrintF(buff,"%f",val);
	  advFrontDialog.setTextLabel(3,buff);
	}
      else if ( answer(0,9)=="print face" )
	{
	  int val;
	  aString buff;
	  int nRead = sScanF(&answer[10], "%i", &val);
	  if ( nRead<1 )
	    {
	      errorReport(gi,"ERROR: invalid input");
	      sPrintF(buff, "%i", -1);
	      val = -1;
	    }
	  else
	    {
	      const vector<Face *> & fs = advancingFront.getFaces();
	      if ( val>=0 && val<advancingFront.getNumberOfFaces() )
		{
		  if ( fs[val]->getNumberOfVertices()==4 )
		    sPrintF(buff,"%i, %i, %i, %i", fs[val]->getVertex(0),
			    fs[val]->getVertex(1),
			    fs[val]->getVertex(2),
			    fs[val]->getVertex(3));
		  else if ( fs[val]->getNumberOfVertices()==3 )
		    sPrintF(buff,"%i, %i, %i", fs[val]->getVertex(0),
			    fs[val]->getVertex(1),
			    fs[val]->getVertex(2));
		  else
		    sPrintF(buff,"%i, %i", fs[val]->getVertex(0),
			    fs[val]->getVertex(1));
		  
		  gi.outputString(buff);
		  buff="";
		}
	      advFrontParameters.highlightFace(val);
	    }
	  sPrintF(buff, "%i", val);
	  advFrontDialog.setTextLabel(4,buff); 
	  plotObject = true;
	  
	}
      else if (answer == "status")
	{
	}
      else if (answer == "close advancing front dialog")
	{
	  advFrontDialog.hideSibling();
	}
      else if (answer == "close triangle dialog")
	{
	  triangleWrapperDialog.hideSibling();
	}
      else if ( answer=="Component Grids..." )
	{
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
	  PlotIt::plot(gi,cg,psp);
	}
      else if (answer=="Advancing Front...")
	{
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
	  PlotIt::plot(gi,advancingFront, psp);
	}
      else if (answer=="Delaunay Mesh...")
	{
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
	  PlotIt::plot(gi,delaunayMesh, psp);
	}
      else if (answer == "exit")
	{
	  advFrontDialog.hideSibling();
	  triangleWrapperDialog.hideSibling();
	  break;
	}
      else
	{
	  gi.outputString( sPrintF(buff, "unknown response=%s", (const char *) answer) );
	}
	      
      if (plotObject)
	{
	 plot("Hybrid Mesh Generator", cg, plotComponentGrids, plotTriangle, plotAdvFront);
	 plotObject = false;
	}

    }

  
  if (advancingFront.getNumberOfElements()!=0)  // *wdh* 001004
    buildHybridInterfaceMappings(mapInfo, 
				 cg,
				 genWith,
				 gridIndexVertexMap, 
				 vertexGridIndexMap, 
				 vertexIndex, 
				 initialFaceZones);

  delete [] vertexIDMap;

  gi.erase();
  gi.popGUI();
  gi.unAppendTheDefaultPrompt();

}

void 
Ugen::
updateHybrid(CompositeGrid & cg)
{

#if 0
  MappingInformation mapInfo;

  mapInfo.graphXInterface = Overture::getGraphicsInterface();

  for ( int grid=0; grid<cg.numberOfGrids(); grid++ )
    mapInfo.mappingList.addElement(*(cg[grid].mapping().mapPointer));

  updateHybrid(cg,mapInfo);
#else

    // initialize default generator type
  GeneratorType genWith = AdvFront;

  //
  // these are the parameters for the mesh generators
  //
  AdvancingFrontParameters & advFrontParameters = advancingFront.getParameters();
  TriangleWrapperParameters & triangleWrapperParameters = (TriangleWrapperParameters &)triangleWrapper.getParameters();

  preprocessCompositeGridMasks(cg);

  if ( debug_Ugen ) cout<<"preprocessing of masks completed"<<endl;

  // the following 4 arrays should ultimately reside as mapping arrays in the composite grid
  intArray *vertexIndex = new intArray[ cg.numberOfComponentGrids() ];
  intArray numberOfVertices( cg.numberOfComponentGrids() );
  intArray vertexGridIndexMap;
  intArray *gridIndexVertexMap;
  gridIndexVertexMap = new intArray[cg.numberOfComponentGrids()];
  intArray *vertexIDMap = new intArray[cg.numberOfComponentGrids() ];
  realArray xyz_initial;

  intArray initialFaceList; // list of the faces on the hole boundary
  intArray initialFaceZones;// list of the structured grid zones adjacent to the faces on the hole

  int rangeDimension = cg.numberOfDimensions();
  int domainDimension = cg.numberOfGrids()>0 ? cg[0].domainDimension() : rangeDimension;

  try {
    initializeGeneration(cg, vertexIndex, numberOfVertices, vertexIDMap, vertexGridIndexMap, gridIndexVertexMap, initialFaceList, initialFaceZones, xyz_initial);
  } catch (AbstractException &e) {
    throw e;
  }

  if ( initialFaceList.getLength(0) && genWith==AdvFront )
    {
      try {
	generateWithAdvancingFront();
      } catch ( AbstractException &e ) {
	MappingInformation mapInfo;
	mapInfo.graphXInterface = Overture::getGraphicsInterface();
	updateHybrid(cg,mapInfo);
      }
    }

  // *wdh* added 04016 
  if (advancingFront.getNumberOfElements()!=0)
  {
    MappingInformation mapInfo;
    mapInfo.graphXInterface = Overture::getGraphicsInterface();
    buildHybridInterfaceMappings(mapInfo, 
				 cg,
				 genWith,
				 gridIndexVertexMap, 
				 vertexGridIndexMap, 
				 vertexIndex, 
				 initialFaceZones);
  }
  
    
  delete [] vertexIndex;
  delete [] gridIndexVertexMap;
  delete [] vertexIDMap;
#endif


}

int
Ugen::
initialize()
{
return 0;
}

void 
Ugen::
buildHybridInterfaceMappings(MappingInformation &mapInfo, 
			     CompositeGrid &cg,
			     Ugen::GeneratorType genWith,
			     intArray * & gridIndex2UnstructuredVertex,
			     intArray   & unstructuredVertex2GridIndex,
			     intArray * & gridVertex2UnstructuredVertex,
			     intArray   & initialFaceZones)
{
  intArray elementList;
  realArray xyz;
  intArray UnstructuredElement2StructuredZone;

  UnstructuredElement2StructuredZone.redim(initialFaceZones.getLength(0),5);
  UnstructuredElement2StructuredZone = -1;

  UnstructuredMapping *um = new UnstructuredMapping(2,2,
						    Mapping::parameterSpace,
						    Mapping::cartesianSpace);

  Range AXES(0, cg.numberOfDimensions()-1);

  if ( genWith == AdvFront )
    {
      
      elementList = advancingFront.generateElementList();
      
      Range R(0,advancingFront.getNumberOfVertices()-1);
      
      xyz = advancingFront.getVertices()(R, AXES);
      const vector<Face *> & faces = advancingFront.getFaces();

      for ( int f=0; f<initialFaceZones.getLength(0); f++ )
	{
	  UnstructuredElement2StructuredZone(f,0) = faces[f]->getZ2ID();
	  for ( int i=0; i<4; i++ )
	    UnstructuredElement2StructuredZone(f,i+1) = initialFaceZones(f,i);
	}
    }
  else if ( genWith == TriWrap )
    {
      elementList = triangleWrapper.generateElementList();
      xyz = triangleWrapper.getPoints();
      const intArray & initialFaceElements = triangleWrapper.getInitialFaces2TriangleMapping();
      for ( int f=0; f<initialFaceZones.getLength(0); f++ )
	{
	  UnstructuredElement2StructuredZone(f,0) = initialFaceElements(f);
	  for ( int i=0; i<4; i++ )
	    UnstructuredElement2StructuredZone(f,i+1) = initialFaceZones(f,i);
	}
    }

  // 031124  um->setNodesAndConnectivity(xyz, elementList, cg.numberOfDimensions());
  um->setNodesAndConnectivity(xyz, elementList, advancingFront.getDomainDimension());

  //um->incrementReferenceCount();
  mapInfo.mappingList.addElement(*um);//, mapInfo.mappingList.getLength());

  //um->incrementReferenceCount();
  cg.add(*um);
  cg[cg.numberOfComponentGrids()-1].update(MappedGrid::THEmask);
  int ugrid = elementList.getLength(0)==0 ? -1 : cg.numberOfComponentGrids()-1;
  cg.setHybridConnectivity(ugrid, 
			   gridIndex2UnstructuredVertex,
			   unstructuredVertex2GridIndex,
			   gridVertex2UnstructuredVertex,
			   UnstructuredElement2StructuredZone);

  FILE *checkfile=0;
  // ok this is bad, ogen.check Ogen probably still has an open file pointer to this file!!! but hey...
  checkfile = fopen("ogen.check","a");
  if ( !checkfile ) return;
  fprintf(checkfile,"*************************************\n");
  fprintf(checkfile,"************ugen check file**********\n");
  fprintf(checkfile,"*************************************\n");
  fprintf(checkfile,"generated with %s\n",genWith==AdvFront ? "advancing front" : "triangle");

  for ( int d=0; d<=int(UnstructuredMapping::Region); d++ )
    fprintf(checkfile,"number of %ss : %i\n",(char *)UnstructuredMapping::EntityTypeStrings[d].c_str(),
                um->size(UnstructuredMapping::EntityTypeEnum(d)));

  fclose(checkfile);
}

void 
Ugen::
preprocessCompositeGridMasks(CompositeGrid &cg)
{

  int grid;
  
  // remove interpolation/overlap masks leaving just a big hole
  cout << "updating masks";
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & c = cg[grid];
      intArray & mask = c.mask();

      where( mask<0 )
	{
	  mask=0;
	}
      cout <<".";
    }
  cout << " "<< cg.numberOfComponentGrids()<<" grids"<<endl;
  cout<<endl;
  cout << "initial update to mask arrays complete"<<endl;

  // find the hole points on each grid
  //IntegerArray *vertexIndex = new IntegerArray[ cg.numberOfComponentGrids() ];
  //IntegerArray numberOfVertices(cg.numberOfComponentGrids());
  //generateHoleLists(cg, vertexIndex, numberOfVertices);

  // the above process may leave faces sticking out into the holes, these should be removed
  //removeHangingFaces(cg);//, vertexIndex, numberOfVertices);

  //delete [] vertexIndex;
}

static void recurs_updateNeighbors(intArray &mask, intArray &vertexRefCount, 
				   const int &i1, const int &i2, const int &i3, 
				   const int *const & ibase, const int *const & ibounds,
				   const int &minRef)
{

  int i1p = i1+1;
  int i1m = i1-1;
  int i2p = i2+1;
  int i2m = i2-1;
  int i3p = i3+1;
  int i3m = i3-1;

  if (i1p<=ibounds[0] && mask(i1p,i2,i3)>0)
    {
      vertexRefCount(i1p, i2, i3)--;
      if (vertexRefCount(i1p,i2,i3)<minRef)
	{
	  mask(i1p, i2, i3) = 0;
	  recurs_updateNeighbors(mask, vertexRefCount, i1p, i2, i3, ibase, ibounds, minRef);
	}
    }
  if (i1m>=ibase[0] && mask(i1m,i2,i3)>0)
    {
      vertexRefCount(i1m, i2, i3)--;
      if (vertexRefCount(i1m,i2,i3)<minRef)
	{
	  mask(i1m, i2, i3) = 0;
	  recurs_updateNeighbors(mask, vertexRefCount, i1m, i2, i3, ibase, ibounds, minRef);
	}
    }

  if (i2p<=ibounds[1] && mask(i1,i2p,i3)>0)
    {
      vertexRefCount(i1, i2p, i3)--;
      if (vertexRefCount(i1,i2p,i3)<minRef)
	{
	  mask(i1, i2p, i3) = 0;
	  recurs_updateNeighbors(mask, vertexRefCount, i1, i2p, i3, ibase, ibounds, minRef);
	}
    }
  if (i2m>=ibase[1] && mask(i1,i2m,i3)>0)
    {
      vertexRefCount(i1, i2m, i3)--;
      if (vertexRefCount(i1,i2m,i3)<minRef)
	{
	  mask(i1, i2m, i3) = 0;
	  recurs_updateNeighbors(mask, vertexRefCount, i1, i2m, i3, ibase, ibounds, minRef);
	}
    }

  if (i3p<=ibounds[2] && mask(i1,i2,i3p)>0)
    {
      vertexRefCount(i1, i2, i3p)--;
      if (vertexRefCount(i1,i2,i3p)<minRef)
	{
	  mask(i1, i2, i3p) = 0;
	  recurs_updateNeighbors(mask, vertexRefCount, i1, i2, i3p, ibase, ibounds, minRef);
	}
    }
  if (i3m>=ibase[2] && mask(i1,i2,i3m)>0)
    {
      vertexRefCount(i1, i2, i3m)--;
      if (vertexRefCount(i1,i2,i3m)<minRef)
	{
	  mask(i1, i2, i3m) = 0;
	  recurs_updateNeighbors(mask, vertexRefCount, i1, i2, i3m, ibase, ibounds, minRef);
	}
    }


}

void
Ugen::
removeHangingFaces(CompositeGrid &cg)
{

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  
  int minRef = (cg[0].domainDimension()==2) ? 2 : 3;
  
  for (int grid=0; grid<cg.numberOfComponentGrids(); grid++)
    {
      MappedGrid & mappedGrid = cg[grid];
      intArray & mask = mappedGrid.mask();
      getIndex(mappedGrid.gridIndexRange(),I1,I2,I3);
      intArray vertexRefCount(I1,I2,I3);
      vertexRefCount = 0;

      int ibounds[3];
      ibounds[0] = I1.getBound();
      ibounds[1] = I2.getBound();
      ibounds[2] = I3.getBound();
      
      int ibase[3];
      ibase[0] = I1.getBase();
      ibase[1] = I2.getBase();
      ibase[2] = I3.getBase();
      
      if (I3.getBase()==I3.getBound()) ibase[2]=ibounds[2]=I3.getBound();
      //      for (int axis=0; axis<cg.numberOfDimensions(); axis++)
//        for (int axis=0; axis<3; axis++)
//  	if (ibounds[axis]<ibase[axis]) {
//  	  ibounds[axis] = Iv[axis].getBound();
//  	  ibase[axis] = Iv[axis].getBase();
//  	}

      // first count the number of unmasked adjacent vertices for each interior vertex
      int i1,i2,i3;
      for ( i3=ibase[2]; i3<=ibounds[2]; i3++ )
	{
	  for ( i2=ibase[1]; i2<=ibounds[1]; i2++ )
	    {
	      for ( i1=ibase[0]; i1<=ibounds[0]; i1++ )
		{

		  if (mask(i1+1, i2, i3) > 0) vertexRefCount(i1,i2,i3)++;
		  if (mask(i1-1, i2, i3) > 0) vertexRefCount(i1,i2,i3)++;
		  if (mask(i1, i2+1, i3) > 0) vertexRefCount(i1,i2,i3)++;
		  if (mask(i1, i2-1, i3) > 0) vertexRefCount(i1,i2,i3)++;

		  if (cg[grid].domainDimension() == 3) 
		    {
		      //if ( i3!=ibounds[2] && mask(i1 , i2, i3+1) > 0) 
		      if ( mask(i1 , i2, i3+1) > 0 )
			vertexRefCount(i1,i2,i3)++;
			//if ( i3!=ibase[2]   && mask(i1 , i2, i3-1) > 0) 
		      if ( mask(i1 , i2, i3-1) > 0 ) 
			vertexRefCount(i1,i2,i3)++;

		      // adjust for boundaries
#if 0
		      if ( i1==ibounds[0] && mask(i1+1, i2, i3)>0 )  vertexRefCount(i1,i2,i3)--;
		      if ( i1==ibase[0] && mask(i1-1, i2, i3)>0 )  vertexRefCount(i1,i2,i3)--;
		      if ( i2==ibounds[1] && mask(i1, i2+1, i3)>0 )  vertexRefCount(i1,i2,i3)--;
		      if ( i2==ibase[1] && mask(i1, i2-1, i3)>0 )  vertexRefCount(i1,i2,i3)--;
#endif
		      
		    }
		} // i1
	    } // i2
	} // i3

      // now loop through the vertices again, removing any that have too few connections
      //   as a vertex is removed/masked out, visit the neighboring vertices and update thier 
      //   vertexRefCount entry and mask if neccessary
      for ( i3=ibase[2]; i3<=ibounds[2]; i3++ )
	{
	  for ( i2=ibase[1]; i2<=ibounds[1]; i2++ )
	    {
	      for ( i1=ibase[0]; i1<=ibounds[0]; i1++ )
		{

		  if (vertexRefCount(i1,i2,i3) < minRef && mask(i1,i2,i3)>0)
		    {
		      //cout<<"fixing hanging vertex "<<endl;
		      mask(i1,i2,i3) = 0;
		      //vertexRefCount(i1,i2,i3)--;
		      // the next block of ifs check for periodic boundaries
		      if ( mappedGrid.isPeriodic(0) && i1==ibase[0] )
			{
			  mask(ibounds[0],i2,i3) = 0;
			  // mask out ghost vertices
			  mask(ibounds[0]+1,i2,i3) = mask(ibase[0]-1,i2,i3) = 0;
			  vertexRefCount(ibounds[0],i2,i3)--;
			  vertexRefCount(i1,i2,i3)--;
			  recurs_updateNeighbors(mask, vertexRefCount, 
						 ibounds[0], i2, i3, ibase, ibounds, minRef);
			}
		      else if ( mappedGrid.isPeriodic(1) && i2==ibase[1] )
			{
			  mask(i1,ibounds[1],i3) = 0;
			  // mask out ghost vertices
			  mask(i1, ibounds[1]+1, i3) = mask(i1, ibase[1]-1, i3) = 0;
			  vertexRefCount(i1,ibounds[1],i3)--;
			  vertexRefCount(i1,i2,i3)--;
			  recurs_updateNeighbors(mask, vertexRefCount,
						 i1, ibounds[1], i3, ibase, ibounds, minRef);
			}
		      else if ( mappedGrid.isPeriodic(2) && i3==ibase[2] && (ibase[2]!=ibounds[2]) ) 
			{
			  mask(i1,i2,ibounds[2]) = 0;
			  // mask out ghost vertices
			  mask(i1, i2, ibounds[2]+1) = mask(i1, i2, ibase[2]-1) = 0;
			  vertexRefCount(i1,i2,ibounds[2])--;
			  vertexRefCount(i1,i2,i3)--; 
			  recurs_updateNeighbors(mask, vertexRefCount,
						 i1, i2, ibounds[2], ibase, ibounds, minRef);
			}
		      
		      // update the neighbors, recurse if needed
		      recurs_updateNeighbors(mask, vertexRefCount, i1,i2,i3, ibase, ibounds, minRef); 
		    }
		} // i1
	    } // i2
	} // i3

      for ( i3=ibase[2]; i3<=ibounds[2]; i3++ )
	{
	  for ( i2=ibase[1]; i2<=ibounds[1]; i2++ )
	    {
	      for ( i1=ibase[0]; i1<=ibounds[0]; i1++ )
		{
		  
		  if (vertexRefCount(i1,i2,i3) ==0 && mask(i1,i2,i3)>0)
		    {
		      cout<<"fixing hanging vertex "<<endl;
		      mask(i1,i2,i3) = 0;
		    }
		}
	    }
	}
    } // grid

}



void
Ugen::
buildHybridVertexMappings(CompositeGrid &cg, intArray *vertexIndex, 
			  intArray &numberOfVertices, intArray *vertexIDMap, 
			  intArray &vertexGridIndexMap, intArray *gridIndexVertexMap, realArray &xyz_initial)
{
  //IntegerArray *vertexIndex = new IntegerArray[ cg.numberOfComponentGrids() ];
  //IntegerArray numberOfVertices(cg.numberOfComponentGrids());
  generateHoleLists(cg, vertexIndex, numberOfVertices);

  int rangeDimension = cg.numberOfDimensions();
  int domainDimension = cg[0].domainDimension();

  int numberOfInitialVertices = sum(numberOfVertices); // the vertices will be packed into xyz_initial
  xyz_initial.redim(numberOfInitialVertices, rangeDimension);

  Range AXES(rangeDimension);

  //domainDimension = cg.numberOfDimensions();
  //xyz_initial.redim(numberOfInitialVertices, domainDimension);

  // the following two should be stored someplace like the CompositeGrid cg.
  // vertexIDMap - maps a grid and index into vertexIndex[grid] to the unstructured vertex id
  // vertexGridIndexMap - maps an unstructured mesh vertex into the corresponding grid and ijk index
  // these two arrays, in addition to vertexIndex and numberOfVertices will aid the interface between structured and unstructured regions
  //IntegerArray *vertexIDMap = new IntegerArray[ cg.numberOfComponentGrids() ];
  vertexGridIndexMap.redim(numberOfInitialVertices, 4); // 0 - grid ; 1,2,3 - i,j,k
  
  // fill the list of vertices on the boundary of the holes and create a mapping back to the original grids
  int grid;
  int vertexID = 0;
  for ( grid = 0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mappedGrid = cg[grid];
      MappingRC &map = mappedGrid.mapping();
      Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
      getIndex(mappedGrid.gridIndexRange(),I1,I2,I3);
      cg[grid].update(MappedGrid::THEvertex);

      realArray & cvertices = cg[grid].vertex();
      intArray & iv = vertexIndex[grid];
      intArray & ivmap = vertexIDMap[grid];
      intArray & gvimap = gridIndexVertexMap[grid];
      ivmap.redim(numberOfVertices(grid));
      gvimap.redim(I1,I2,I3);
      gvimap = -1;

      for ( int v=0; v<numberOfVertices(grid); v++)
	{
	  bool treatPolarAtBase = false;
	  bool treatPolarAtBound = false;
	  if ( map.getDomainDimension()==3 ) 
	    {
	      treatPolarAtBase = ( (map.getTypeOfCoordinateSingularity(0,0)==Mapping::polarSingularity && 
				    iv(v,0)==I1.getBase()) ||
				   (map.getTypeOfCoordinateSingularity(0,1)==Mapping::polarSingularity && 
				    iv(v,1)==I2.getBase()) ||
				   (map.getTypeOfCoordinateSingularity(0,2)==Mapping::polarSingularity && 
				    iv(v,2)==I3.getBase()) );
	      treatPolarAtBound = ( (map.getTypeOfCoordinateSingularity(1,0)==Mapping::polarSingularity && 
				     iv(v,0)==I1.getBound()) ||
				    (map.getTypeOfCoordinateSingularity(1,1)==Mapping::polarSingularity && 
				     iv(v,1)==I2.getBound()) ||
				    (map.getTypeOfCoordinateSingularity(1,2)==Mapping::polarSingularity && 
				     iv(v,2)==I3.getBound()) );
	    }
	      

	  if ( treatPolarAtBase )
	    {
	      if ( gvimap(0,0,0)==-1 ) 
		{
		  for (int axis =AXES.getBase(); axis<=AXES.getBound(); axis++)
		    xyz_initial(vertexID, axis) = cvertices(iv(v,0),iv(v,1),iv(v,2), axis);
		  
		  ivmap(v) = vertexID;
		  vertexGridIndexMap(vertexID, 0) = grid;
		  vertexGridIndexMap(vertexID, 1) = iv(v,0);
		  vertexGridIndexMap(vertexID, 2) = iv(v,1);
		  vertexGridIndexMap(vertexID, 3) = iv(v,2);
		  gvimap(0, 0, 0) = gvimap(iv(v,0), iv(v,1), iv(v,2)) = vertexID;
		  vertexID++;
		}
	      else
		{
		  for (int axis =AXES.getBase(); axis<=AXES.getBound(); axis++)
		    xyz_initial(vertexID, axis) = cvertices(iv(v,0),iv(v,1),iv(v,2), axis);
		  vertexGridIndexMap(vertexID, 0) = grid;
		  vertexGridIndexMap(vertexID, 1) = iv(v,0);
		  vertexGridIndexMap(vertexID, 2) = iv(v,1);
		  vertexGridIndexMap(vertexID, 3) = iv(v,2);
		  gvimap(iv(v,0), iv(v,1), iv(v,2)) = gvimap(0, 0, 0);
		  ivmap(v) = gvimap(0, 0, 0);
		  vertexID++;
		}
	    }
	  else if ( treatPolarAtBound )
	    {
	      if ( gvimap(I1.getBound(),I2.getBound(),I3.getBound())==-1 ) 
		{
		  for (int axis =AXES.getBase(); axis<=AXES.getBound(); axis++)
		    xyz_initial(vertexID, axis) = cvertices(iv(v,0),iv(v,1),iv(v,2), axis);
		  
		  ivmap(v) = vertexID;
		  vertexGridIndexMap(vertexID, 0) = grid;
		  vertexGridIndexMap(vertexID, 1) = iv(v,0);
		  vertexGridIndexMap(vertexID, 2) = iv(v,1);
		  vertexGridIndexMap(vertexID, 3) = iv(v,2);
		  gvimap(I1.getBound(),I2.getBound(),I3.getBound()) = gvimap(iv(v,0), iv(v,1), iv(v,2)) = vertexID;
		  vertexID++;
		}
	      else
		{
		  for (int axis =AXES.getBase(); axis<=AXES.getBound(); axis++)
		    xyz_initial(vertexID, axis) = cvertices(iv(v,0),iv(v,1),iv(v,2), axis);
		  vertexGridIndexMap(vertexID, 0) = grid;
		  vertexGridIndexMap(vertexID, 1) = iv(v,0);
		  vertexGridIndexMap(vertexID, 2) = iv(v,1);
		  vertexGridIndexMap(vertexID, 3) = iv(v,2);
		  gvimap(iv(v,0), iv(v,1), iv(v,2)) = gvimap(I1.getBound(),I2.getBound(),I3.getBound());
		  ivmap(v) = gvimap(I1.getBound(),I2.getBound(),I3.getBound());
		  vertexID++;
		}
	    }
	  else
	    {
	      for (int axis =AXES.getBase(); axis<=AXES.getBound(); axis++)
		xyz_initial(vertexID, axis) = cvertices(iv(v,0),iv(v,1),iv(v,2), axis);
	      
	      ivmap(v) = vertexID;
	      vertexGridIndexMap(vertexID, 0) = grid;
	      vertexGridIndexMap(vertexID, 1) = iv(v,0);
	      vertexGridIndexMap(vertexID, 2) = iv(v,1);
	      vertexGridIndexMap(vertexID, 3) = iv(v,2);
	      gvimap(iv(v,0), iv(v,1), iv(v,2)) = vertexID;
	      vertexID++;
	    }
	}
    }
}

static inline void computeVertexBounds(const MappedGrid &mg, int *ibounds)
{
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  getIndex(mg.gridIndexRange(),I1,I2,I3);
  
  ibounds[0] = I1.getBound();
  ibounds[1] = I2.getBound();
  ibounds[2] = I3.getBound();
  
  for (int axis=0; axis<3; axis++)
    if (mg.isPeriodic(axis)) {
      ibounds[axis] = ibounds[axis]-1;
    }  
}

static void extractStructuredFace(const MappedGrid &mg,  
				  const intArray &gi2vID, 
				  const int &i1, const int &i2, const int&i3, 
				  const int &axis, const int &side, 
				  const intArray &face)
{

  //
  // this function pulls the unstructured vertex id's out of gi2vID corresponding to a requested
  // face on a structured zone.  The Orientation of the face points the face normal outside the 
  // zone.  This function could probably be simplified by stuffing the zone vertices into an 
  // UnstructuredMapping style zone template and using the appropriate face templates to extract 
  // the faces.
  //

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  getIndex(mg.gridIndexRange(),I1,I2,I3);

//   real signForJacobian = mg.mapping().getMapping().getClassName()=="ReductionMapping" ?
//     ((ReductionMapping &)mg.mapping().getMapping()).getOriginalMapping()->getSignForJacobian() : 
//     mg.mapping().getMapping().getSignForJacobian();

  real signForJacobian = mg.mapping().getMapping().getSignForJacobian();

  int numberOfDimensions = mg.domainDimension();
  int facebase = face.getBase(0);
  int ibounds[3];
  computeVertexBounds(mg, ibounds);

  Range FACEV(0, face.getLength(1)-1);
  intArray faceTemp(FACEV);
  faceTemp = -1;

  // 
  // check to make sure the requested axis does not exceed the number of dimensions
  //
  if (numberOfDimensions == 2) AssertException(!debug_Ugen || axis<2, PreProcessingError());
  AssertException(!debug_Ugen || (side==0 || side==1), PreProcessingError());

  int i1u=i1+1, i2u=i2+1, i3u=i3+1;
  if (i1u==(ibounds[0]+1) && mg.isPeriodic(0)) i1u = I1.getBase();
  if (i2u==(ibounds[1]+1) && mg.isPeriodic(1)) i2u = I2.getBase();
  if (i3u==(ibounds[2]+1) && mg.isPeriodic(2)) i3u = I3.getBase();

  if (numberOfDimensions==2) // construction of an oriented 2D face 
    {
      switch(axis) {
      case 0:
	i1u = i1+side;
	// adjust for periodicity
	if (i1+side == (ibounds[0]+1) && mg.isPeriodic(0)) i1u = I1.getBase();
	AssertException(gi2vID(i1u, i2  , i3)!=-1 && gi2vID(i1u, i2u, i3)!=-1, PreProcessingError());
	faceTemp(0) = gi2vID(i1u, i2 , i3);
	faceTemp(1) = gi2vID(i1u, i2u, i3);
	break;
      case 1:
	i2u = i2+side;
	// adjust for periodicity
	if (i2+side == (ibounds[1]+1) && mg.isPeriodic(1)) i2u = I2.getBase();
	AssertException(gi2vID(i1u, i2u, i3)!=-1 && gi2vID(i1, i2u, i3)!=-1, PreProcessingError());
	faceTemp(0) = gi2vID(i1u , i2u, i3);
	faceTemp(1) = gi2vID(i1  , i2u, i3);
	break;
      default:
	throw PreProcessingError();
      }
      // assign face to faceTemp, making sure the orientation is correct
      if ( (side==1 && signForJacobian>0) || ( side==0 && signForJacobian<0) ) 
	{
	  face(facebase, 0) = faceTemp(1);
	  face(facebase, 1) = faceTemp(0);
	} else {
	  face(facebase, FACEV) = faceTemp.reshape(1,FACEV.getLength());
	}
    }
  else if (numberOfDimensions==3) // construction of an oriented 3D face
    {
      switch(axis) {
      case 0:
	i1u = i1+side;
	if (i1u == (ibounds[0]+1) && mg.isPeriodic(0)) i1u = I1.getBase();
	AssertException(gi2vID(i1u, i2 , i3 )!=-1 &&
			gi2vID(i1u, i2 , i3u)!=-1 &&
			gi2vID(i1u, i2u, i3u)!=-1 &&
			gi2vID(i1u, i2 , i3 )!=-1, PreProcessingError());
	faceTemp(0) = gi2vID(i1u, i2 , i3 );
	faceTemp(1) = gi2vID(i1u, i2 , i3u);
	faceTemp(2) = gi2vID(i1u, i2u, i3u);
	faceTemp(3) = gi2vID(i1u, i2u , i3 );
	break;
      case 1:
	i2u = i2+side;
	if (i2u == (ibounds[1]+1) && mg.isPeriodic(1)) i2u = I2.getBase();
	AssertException(gi2vID(i1 , i2u, i3 )!=-1 &&
			gi2vID(i1u, i2u, i3 )!=-1 &&
			gi2vID(i1u, i2u, i3u)!=-1 &&
			gi2vID(i1 , i2u, i3u)!=-1, PreProcessingError());
	faceTemp(0) = gi2vID(i1 , i2u, i3 );
	faceTemp(1) = gi2vID(i1u, i2u, i3 );
	faceTemp(2) = gi2vID(i1u, i2u, i3u);
	faceTemp(3) = gi2vID(i1 , i2u, i3u);
	break;
      case 2:
	i3u = i3+side;
	if (i3u == (ibounds[2]+1) && mg.isPeriodic(2)) i3u = I3.getBase();
	AssertException(gi2vID(i1 , i2 , i3u)!=-1 &&
			gi2vID(i1 , i2u, i3u)!=-1 &&
			gi2vID(i1u, i2u, i3u)!=-1 &&
			gi2vID(i1u, i2 , i3u)!=-1, PreProcessingError());
	faceTemp(0) = gi2vID(i1 , i2 , i3u);
	faceTemp(1) = gi2vID(i1 , i2u, i3u);
	faceTemp(2) = gi2vID(i1u, i2u, i3u);
	faceTemp(3) = gi2vID(i1u, i2 , i3u);
	break;
      default:
	throw PreProcessingError();
      }

      
      // assign face to faceTemp, making sure the orientation is correct
      if ( (side==1 && signForJacobian>0) || ( side==0 && signForJacobian<0) ) 
	{
	  for (int v=0; v<FACEV.getLength(); v++)
	    face(facebase, FACEV.getLength()-1-v) = faceTemp(v);
	} else {
	  face(facebase, FACEV) = faceTemp.reshape(1,FACEV.getLength());
	}
      // now check for degeneracies
      faceTemp.reshape(1,FACEV.getLength());
      faceTemp = face(facebase,FACEV);
      for (int v0=0; v0<FACEV.getLength()-1; v0++)
	for (int v1=v0+1; v1<FACEV.getLength(); v1++)
	  if ( face(facebase, v0) == face(facebase, v1) ) 
	    {
	      faceTemp(0,v0) = -1;
	      break;
	    }
      face(facebase,FACEV) = -1;
      int fvv = 0;
      for ( int fv=0; fv<FACEV.getLength(); fv++ )
	if ( faceTemp(0,fv)!=-1 )
	  {
	    face(facebase, fvv) = faceTemp(0,fv);
	    fvv++;
	  }
    }
  else
    throw PreProcessingError();
					       
}

// a little helper function used in generateInitialFaceList
static inline void assignFaceZone(intArray &fz, const int &f, const int &grid, 
				  const int &i1, const int &i2, const int &i3)
{

  fz(f,0) = grid;
  fz(f,1) = i1;
  fz(f,2) = i2;
  fz(f,3) = i3;

}

void
Ugen::
generateInitialFaceList(CompositeGrid &cg, intArray *vertexIndex, 
			intArray &numberOfVertices, intArray *vertexIDMap, 
			intArray &vertexGridIndexMap, intArray *gridIndexVertexMap, 
			intArray &initialFaces, intArray &initialFaceZones)
{ 
  
  
  // Basic Idea : loop through all the zones in a mesh and determine whether they are on the boundary 
  //              of a hole in the composite grid. If a zone is on a boundary then it will contribute
  //              a face or faces to the initial faces given to the unstructured mesh generator.

  // ISSUES : 
  //          ** 1. the code does not detect coordinate singularities and hence will
  //          **    generate 3D triangular faces as quadrilaterals. ** FIXED in extractStructuredFace  **
  //

  // create zone based masks for each grid in cg that will tell us if a given 
  // zone has been cut out by the hole cutting algorithm
  intArray *zoneMasks = NULL;
  intArray numberOfMaskedZones;
  computeZoneMasks(cg, zoneMasks, numberOfMaskedZones);  // WARNING, computeZoneMasks allocates zoneMasks!, deallocate it below

  int numberOfDimensions = cg.numberOfDimensions();
  int maxNumberOfVerticesOnFace = ( numberOfDimensions == 2 ) ? 2 : 4;
  
  Range FACEV(0, maxNumberOfVerticesOnFace-1);

  int numberOfInitialVertices = sum(numberOfVertices); // the vertices will be packed into xyz_initial

  // let guess the maximum number of faces that could occur in the front
  // how is this done : 
  // maxNumberOfFaces = (number of masked out zones)*(max number of faces exposed by a masked zone) 
  int maxNumberOfFaces = sum(numberOfMaskedZones)*(numberOfDimensions*2 - 1);
  
  // now size initialFaces by the estimate above
  initialFaces.redim(maxNumberOfFaces, maxNumberOfVerticesOnFace);
  initialFaces = -1;

  // initialFaceZones keeps track of which structured grid zones each initial face came from, this will
  // be usefull later when creating the hybrid grid interface data structures
  initialFaceZones.redim(maxNumberOfFaces, 4); // structured zone corresponding to a face 0 - grid; 1 - i1; 2 - i2; 3 - i3; 

  int grid;

  int faceID = 0;
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  int i1,i2,i3;

  for (grid = 0; grid<cg.numberOfComponentGrids(); grid++)
    {

      MappedGrid &mappedGrid = cg[grid];
      getIndex(mappedGrid.gridIndexRange(),I1,I2,I3);

#if 0
      real signForJacobian = mappedGrid.mapping().getMapping().getClassName()=="ReductionMapping" ?
	((ReductionMapping &)mappedGrid.mapping().getMapping()).getOriginalMapping()->getSignForJacobian() : 
	mappedGrid.mapping().getMapping().getSignForJacobian();
#else
      real signForJacobian = mappedGrid.mapping().getMapping().getSignForJacobian();
#endif

      cout<<"Ugen::generateInitialFaceList: grid sign for jacobian "<<grid<<" "<<signForJacobian<<endl;

      intArray &zoneMask = zoneMasks[grid];

      intArray & gridIndex2VertexID = gridIndexVertexMap[grid];

      int ibounds[3];
      ibounds[0] = I1.getBound()-1;
      ibounds[1] = I2.getBound()-1;
      ibounds[2] = I3.getBound()-1;
      
      int gridOffset[] = { 1, 1, 1 };

      int numberOfDimensions = cg.numberOfDimensions();

      // reset the bounds and offset in case we have a plane in coordinate space
      for (int axis=0; axis<3; axis++)
	if (Iv[axis].getLength()==1) 
	  {
	    ibounds[axis] = Iv[axis].getBound();
	    gridOffset[axis] = 0;
	  }

      // loop through each zone adding faces to initialFaces as they are found
      for (i3=I3.getBase(); i3<=ibounds[2]; i3++)
	{
	  for (i2=I2.getBase(); i2<=ibounds[1]; i2++)
	    {
	      for (i1=I1.getBase(); i1<=ibounds[0]; i1++)
		{
		  
		  int i1m1 = i1-gridOffset[0];
		  int i1p1 = i1+gridOffset[0];
		  int i2m1 = i2-gridOffset[1];
		  int i2p1 = i2+gridOffset[1];
		  int i3m1 = i3-gridOffset[2];
		  int i3p1 = i3+gridOffset[2];
		  
		  // check and adjust indices for periodicity
		  if (i1 == ibounds[0] && mappedGrid.isPeriodic(0))
		    i1p1 = I1.getBase();
		  
		  if (i1 == I1.getBase() && mappedGrid.isPeriodic(0)) 
		    i1m1 = ibounds[0];
		  
		  if (i2 == ibounds[1] && mappedGrid.isPeriodic(1)) 
		    i2p1 = I2.getBase();
		  
		  if (i2 == I2.getBase() && mappedGrid.isPeriodic(1)) 
		    i2m1 = ibounds[1];

		  if (i3 == ibounds[2] && mappedGrid.isPeriodic(2)) 
		    i3p1 = I3.getBase();
		  
		  if (i3 == I3.getBase() && mappedGrid.isPeriodic(2)) 
		    i3m1 = ibounds[2];
		  
		  // if this zone is not masked, check to see if any of it's neighbors are
		  if (zoneMask(i1,i2,i3) > 0) 
		    {
		      // check the neighbors, if a neighbors is masked out, then the face between the
		      //   two zones is on the initial hole front. Add this face to the list of initial faces
		      // Note that a zone can contribute more than one face to the front
		      
		      // a check is performed on the mapping between the grid and its indices for each point
		      // and the data in gridIndexVertexMap for this grid.  If gridIndexVertexMap for the point
		      // on a candidate face is -1 then something is screwed up since these points should lie
		      // on the boundary of the hole.

		      // i1 + 1
		      if (zoneMask(i1p1, i2, i3) == 0)
			{
			  extractStructuredFace(mappedGrid, gridIndex2VertexID, i1,i2,i3, 0, 1, initialFaces(faceID, FACEV));
			  assignFaceZone(initialFaceZones, faceID, grid, i1, i2, i3);
			  faceID++;
			}

		      // i1 - 1
		      if (zoneMask(i1m1, i2, i3) == 0)
			{
			  extractStructuredFace(mappedGrid, gridIndex2VertexID, i1,i2,i3, 0, 0, initialFaces(faceID, FACEV));
			  assignFaceZone(initialFaceZones, faceID, grid, i1, i2, i3);
			  faceID++;
			}

		      // i2 + 1
		      if (zoneMask(i1, i2p1, i3) == 0)
			{
			  extractStructuredFace(mappedGrid, gridIndex2VertexID, i1,i2,i3, 1, 1, initialFaces(faceID, FACEV));
			  assignFaceZone(initialFaceZones, faceID, grid, i1, i2, i3);
			  faceID++;
			}

		      // i2 - 1
		      if (zoneMask(i1, i2m1, i3) == 0)
			{
			  extractStructuredFace(mappedGrid, gridIndex2VertexID, i1,i2,i3, 1, 0, initialFaces(faceID, FACEV));
			  assignFaceZone(initialFaceZones, faceID, grid, i1, i2, i3);
			  faceID++;
			}

		      // i3 + 1
		      if (zoneMask(i1, i2, i3p1) == 0)
			{
			  extractStructuredFace(mappedGrid, gridIndex2VertexID, i1,i2,i3, 2, 1, initialFaces(faceID, FACEV));
			  assignFaceZone(initialFaceZones, faceID, grid, i1, i2, i3);
			  faceID++;
			}

		      // i3 - 1
		      if (zoneMask(i1, i2, i3m1) == 0)
			{
			  extractStructuredFace(mappedGrid, gridIndex2VertexID, i1,i2,i3, 2, 0, initialFaces(faceID, FACEV));
			  assignFaceZone(initialFaceZones, faceID, grid, i1, i2, i3);
			  faceID++;
			}

		    } else {
		      // check boundaries 
		      if ( i1==I1.getBase() && zoneMask(i1m1,i2,i3)>0 ) {  
		      
			// boundary face on axis 0, side 0
			extractStructuredFace(mappedGrid, gridIndex2VertexID, i1m1,i2,i3, 0,1, initialFaces(faceID, FACEV));
			assignFaceZone(initialFaceZones, faceID, grid, i1,i2,i3);
			faceID++;

		      } 
		      if ( i1==ibounds[0] && zoneMask(i1p1,i2,i3)>0 && !mappedGrid.isPeriodic(0) ) {
			
			// boundary face on axis 0, side 1
			extractStructuredFace(mappedGrid, gridIndex2VertexID, i1p1,i2,i3, 0,0, initialFaces(faceID, FACEV));
			assignFaceZone(initialFaceZones, faceID, grid, i1,i2,i3);
			faceID++;
			
		      } 
		      if ( i2==I2.getBase() && zoneMask(i1,i2m1,i3)>0 ) {  
			
			// boundary face on axis 1, side 0
			extractStructuredFace(mappedGrid, gridIndex2VertexID, i1,i2m1,i3, 1,1, initialFaces(faceID, FACEV));
			assignFaceZone(initialFaceZones, faceID, grid, i1,i2,i3);
			faceID++;
			
		      } 
		      if ( i2==ibounds[1] && zoneMask(i1,i2p1,i3)>0 && !mappedGrid.isPeriodic(1) ) {  
			
			// boundary face on axis 1, side 1
			extractStructuredFace(mappedGrid, gridIndex2VertexID, i1,i2p1,i3, 1,0, initialFaces(faceID, FACEV));
			assignFaceZone(initialFaceZones, faceID, grid, i1,i2,i3);
			faceID++;
			
		      } 
		      if ( i3==I3.getBase() && zoneMask(i1,i2,i3m1)>0 && I3.getBase()!=ibounds[2] ) {  
			
			// boundary face on axis 2, side 0 (except don't check if we only have a coordinate plane)
			extractStructuredFace(mappedGrid, gridIndex2VertexID, i1,i2,i3m1, 2,1, initialFaces(faceID, FACEV));
			assignFaceZone(initialFaceZones, faceID, grid, i1,i2,i3);
			faceID++;
			
		      } 
		      if ( i3==ibounds[2] && zoneMask(i1,i2,i3p1)>0 && I3.getBase()!=ibounds[2] && !mappedGrid.isPeriodic(2)) { 
			
			// boundary face on axis 2, side 1 (except don't check if we only have a coordinate plane)
			extractStructuredFace(mappedGrid, gridIndex2VertexID, i1,i2,i3p1, 2,0, initialFaces(faceID, FACEV));
			assignFaceZone(initialFaceZones, faceID, grid, i1,i2,i3);
			faceID++;
			
		      } 
		      if (zoneMask(i1,i2,i3) != 1 && zoneMask(i1,i2,i3)!=0) {
			// a last check to make sure zoneMask is not corrupt
			throw PreProcessingError();
		      }
		    }
		} // i1
	    } // i2
	} // i3
    }

  initialFaces.resize(faceID, maxNumberOfVerticesOnFace);
  initialFaceZones.resize(faceID, initialFaceZones.getLength(1));

  cout<<"Ugen::generateInitialFaceList: number of faces found in Ugen "<<faceID<<endl;

  delete [] zoneMasks;

}

//\begin{>>Ugen.tex}{\subsection{computeZoneMasks}}
void
Ugen::
computeZoneMasks(CompositeGrid &cg, intArray * &zoneMasks, intArray &numberOfMaskedZones)
// /Purpose : construct a mask array for the zones in each grid in a composite grid
// /cg (input/output) : the composite grid containing the mapped grids requiring zone masks, vertex masks may be adjusted
// /zoneMasks (output) : an array of IntegerArray's for each grid containing the mask for each zone  (<1 means a zone is masked out)
// /numberOfMaskedZones : an IntegerArray of the number of zones in each grid in cg that were masked out
// /Description : a zone will be considered masked out if any one of its vertices have {\tt MappedGrid::mask} value <=0 or
// if all of the surrounding zone meet this criteria.  One layer of ghost zones is included. \\
// WARNING :: if a zone is found to be floating in a sea of masked zones (ie it is isolated) 
// then it and its constituent vertices are masked out as well; this adjusts the mapping's vertex mask array.
//\end{Ugen.tex}
{
  // construct a mask array for the zones in each grid in cg
  // a zone will be considered masked out if any one of its vertices
  // have a MappedGrid::mask value <=0 or if all of the surrounding zones
  // meet this criteria
  // one layer of ghost zones is included

  // WARNING :: if a zone is found to be floating in a  sea of masked zones (ie it is isolated)
  //            then it and its surrounding vertices are blanked out as well
  //            This adjusts the mapping's vertex mask array !!

  int ng = cg.numberOfComponentGrids();
  if (zoneMasks==NULL) 
    zoneMasks = new intArray[ ng ];
  else
    throw PreProcessingError();

  numberOfMaskedZones.redim(cg.numberOfComponentGrids());
  numberOfMaskedZones = 0;

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  int i1,i2,i3;

  int grid;

  for (grid = 0; grid<cg.numberOfComponentGrids(); grid++)
    {

      if ( debug_Ugen ) cout<<"creating zone masks for grid "<<grid<<endl; 
      MappedGrid &mappedGrid = cg[grid];
      getIndex(mappedGrid.gridIndexRange(),I1,I2,I3);


      int ibounds[3];
      ibounds[0] = I1.getBound();
      ibounds[1] = I2.getBound();
      ibounds[2] = I3.getBound();
      // ilen contains the length of a grid's zone array including one layer of ghost zones
      int ilen[] = { I1.getLength()+1, I2.getLength()+1, I3.getLength()+1 };

      int gridOffset[] = { 1, 1, 1 };

      int numberOfDimensions = cg.numberOfDimensions();

      for (int axis=0; axis<3; axis++)
	if (Iv[axis].getLength()==1) 
	  {
	    ibounds[axis] = Iv[axis].getBound();
	    gridOffset[axis] = 0;
	    ilen[axis] = 1;
	  }

      intArray &vertexMask = mappedGrid.mask();
      intArray &zoneMask = zoneMasks[grid];
      Range I1Z(I1.getBase()-gridOffset[0], ilen[0]);
      Range I2Z(I2.getBase()-gridOffset[1], ilen[1]);
      Range I3Z(I3.getBase()-gridOffset[2], ilen[2]);
      zoneMask.redim(I1Z, I2Z, I3Z);
      zoneMask = 1;

      for (i3=I3Z.getBase(); i3<=ibounds[2]; i3++)
	{
	  for (i2=I2Z.getBase(); i2<=ibounds[1]; i2++)
	    {
	      for (i1=I1Z.getBase(); i1<=ibounds[0]; i1++)
		{
		  // check to see if any vertices in the zone are masked out
		  // if any of the vertices are masked out then mask out this zone

		  if ( vertexMask(i1,i2,i3)<=0 || vertexMask(i1+gridOffset[0], i2, i3)<=0 ||
		       vertexMask(i1,i2+gridOffset[1], i3)<=0 || 
		       vertexMask(i1+gridOffset[0],i2+gridOffset[1], i3)<=0 ||

		       vertexMask(i1, i2,i3+gridOffset[2])<=0 || 
		       vertexMask(i1+gridOffset[0], i2, i3+gridOffset[2])<=0 ||
		       vertexMask(i1,i2+gridOffset[1], i3+gridOffset[2])<=0 || 
		       vertexMask(i1+gridOffset[0],i2+gridOffset[1], i3+gridOffset[2])<=0 )
		    {
		      zoneMask(i1,i2,i3) = 0;
		      numberOfMaskedZones(grid)++;
		    }
		  

		} // i1
	    } // i2
	} // i3
      //cout << "number of masked zones for grid "<<grid<<" "<< numberOfMaskedZones(grid)<<endl;

      // now check and mask out orphaned zones
      for (i3=I3.getBase(); i3<=ibounds[2]; i3++)
	{
	  for (i2=I2.getBase(); i2<=ibounds[1]; i2++)
	    {
	      for (i1=I1.getBase(); i1<=ibounds[0]; i1++)
		{

		  if (zoneMask(i1,i2,i3)>0)
		    { // check to see if this zone is all alone in a hole
		      // if it is, blank it and it's vertices out
		      if (cg.numberOfDimensions()==2)
			{
			  if ( zoneMask(i1+gridOffset[0],i2,i3)<=0 &&
			       zoneMask(i1-gridOffset[0],i2,i3)<=0 &&
			       zoneMask(i1,i2+gridOffset[1],i3)<=0 &&
			       zoneMask(i1,i2-gridOffset[1],i3)<=0 )
			    {
			      zoneMask(i1,i2,i3) = 0;
			      numberOfMaskedZones(grid)++;
			      vertexMask(i1  , i2, i3) = 0;
			      vertexMask(i1+1, i2, i3) = 0;
			      vertexMask(i1  , i2+1, i3) = 0;
			      vertexMask(i1+1, i2+1, i3) = 0;
			    }
			} else if (cg.numberOfDimensions()==3) {
			  if ( zoneMask(i1+gridOffset[0],i2,i3)<=0 &&
			       zoneMask(i1-gridOffset[0],i2,i3)<=0 &&
			       zoneMask(i1,i2+gridOffset[1],i3)<=0 &&
			       zoneMask(i1,i2-gridOffset[1],i3)<=0 &&
			       zoneMask(i1,i2,i3+gridOffset[2])<=0 &&
			       zoneMask(i1,i2,i3-gridOffset[2])<=0 )
			    {
			      zoneMask(i1,i2,i3) = 0;
			      numberOfMaskedZones(grid)++;
			      vertexMask(i1  , i2, i3) = 0;
			      vertexMask(i1+1, i2, i3) = 0;
			      vertexMask(i1  , i2+1, i3) = 0;
			      vertexMask(i1+1, i2+1, i3) = 0;
			      vertexMask(i1  , i2, i3+1) = 0;
			      vertexMask(i1+1, i2, i3+1) = 0;
			      vertexMask(i1  , i2+1, i3+1) = 0;
			      vertexMask(i1+1, i2+1, i3+1) = 0;
			    }
			}
		    }
		      		      
		} // i1
	    } // i2
	} // i3
      
    }
}



void
Ugen::
generateHoleLists(CompositeGrid &cg, intArray * vertexIndex, intArray & numberOfVertices)
{
#if 1
  AssertException (vertexIndex != NULL,PreProcessingError());
  AssertException (numberOfVertices.getLength(0)==cg.numberOfComponentGrids(),
		   PreProcessingError());
#else
  AssertException<PreProcessingError> (vertexIndex != NULL);
  AssertException<PreProcessingError> (numberOfVertices.getLength(0) == cg.numberOfComponentGrids());
#endif
  //assert(vertexIndex != NULL);
  //assert(numberOfVertices.getLength(0) == cg.numberOfComponentGrids());

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  { 

    MappedGrid & mappedGrid = cg[grid];
    //Mapping &map = mappedGrid.mapping();

    intArray & mask = mappedGrid.mask();
    getIndex(mappedGrid.gridIndexRange(),I1,I2,I3);

    int ibounds[3];
    ibounds[0] = I1.getBound();
    ibounds[1] = I2.getBound();
    ibounds[2] = I3.getBound();
    
//     int ibase[3];
//     ibase[0] = I1.getBase();
//     ibase[1] = I2.getBase();
//     ibase[2] = I3.getBase();

    //for (int axis=0; axis<cg.numberOfDimensions(); axis++)
    for (int axis=0; axis<mappedGrid.domainDimension(); axis++)
      {
	if (mappedGrid.isPeriodic(axis)) {
	  ibounds[axis] = ibounds[axis]-1;
	}
      }


    intArray & ia = vertexIndex[grid];
    ia.redim(I1.length()*I2.length()*I3.length(),3);

    int i=0;
    int ii[3], &i1=ii[0], &i2=ii[1], &i3=ii[2];
    int ip1[3], &i1p1=ip1[0], &i2p1=ip1[1], &i3p1=ip1[2];
    int im1[3], &i1m1=im1[0], &i2m1=im1[1], &i3m1=im1[2];

	  
    for( i3=I3.getBase(); i3<=ibounds[2]; i3++ )
    {
      for( i2=I2.getBase(); i2<=ibounds[1]; i2++ )
      {
	for( i1=I1.getBase(); i1<=ibounds[0]; i1++ )
	{
	  ip1[2] = im1[2] = i3;
	  // check and adjust indices for periodicity
	  for (int axis=0; axis<mappedGrid.domainDimension(); axis++)
	    {
	      ip1[axis] = ii[axis]+1;
	      im1[axis] = ii[axis]-1;
	      if ( mappedGrid.isPeriodic(axis) && ii[axis]==ibounds[axis] )ip1[axis]=Iv[axis].getBase();
	      if ( mappedGrid.isPeriodic(axis) && ii[axis]==Iv[axis].getBase() )im1[axis]=ibounds[axis];
	    }
	  
	  if( mask(i1,i2,i3)!=0 && 
	      ( mask(i1m1,i2m1,i3)==0 || mask(i1  ,i2m1,i3)==0 || mask(i1p1,i2m1,i3)==0 ||
	        mask(i1m1,i2  ,i3)==0 ||                          mask(i1p1,i2  ,i3)==0 ||
	        mask(i1m1,i2p1,i3)==0 || mask(i1  ,i2p1,i3)==0 || mask(i1p1,i2p1,i3)==0 ||

		mask(i1m1,i2m1,i3p1)==0 || mask(i1  ,i2m1,i3p1)==0 || mask(i1p1,i2m1,i3p1)==0 ||
	        mask(i1m1,i2  ,i3p1)==0 || mask(i1,i2,i3p1)==0     || mask(i1p1,i2  ,i3p1)==0 ||
	        mask(i1m1,i2p1,i3p1)==0 || mask(i1  ,i2p1,i3p1)==0 || mask(i1p1,i2p1,i3p1)==0 ||

		mask(i1m1,i2m1,i3m1)==0 || mask(i1  ,i2m1,i3m1)==0 || mask(i1p1,i2m1,i3m1)==0 ||
	        mask(i1m1,i2  ,i3m1)==0 || mask(i1,i2,i3m1)==0     || mask(i1p1,i2  ,i3m1)==0 ||
	        mask(i1m1,i2p1,i3m1)==0 || mask(i1  ,i2p1,i3m1)==0 || mask(i1p1,i2p1,i3m1)==0 ) )
	    {
	      ia(i,0)=i1;
	      ia(i,1)=i2;
	      ia(i,2)=i3;
	      
	      i++;
	    }
	}
      }
    }
    numberOfVertices(grid)=i;
    ia.resize( numberOfVertices(grid), ia.getLength(1));
    if (debug_Ugen) cout<<"number of vertices from grid "<<grid<<" : "<<numberOfVertices(grid)<<endl;
  }

  //  numberOfVertices.display();
}

void
Ugen::
plot(const aString & title, CompositeGrid &cg, bool plotComponentGrids, bool plotTriangle, bool plotAdvFront)
{

  GenericGraphicsInterface & gi = *ps;

  gi.erase();
  
  psp.set(GI_TOP_LABEL, title);

  if ( plotAdvFront )
    {
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT, TRUE);
      PlotIt::plot(gi,advancingFront, psp);
    }

  if ( plotTriangle )
    {
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT, TRUE);
      PlotIt::plot(gi,delaunayMesh, psp);
    }

  if (plotComponentGrids) {
    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT, TRUE);
    PlotIt::plot(gi,cg, psp);
  }


}



void 
Ugen::
initializeGeneration(CompositeGrid &cg, intArray *vertexIndex, 
		     intArray &numberOfVertices, intArray *vertexIDMap, 
		     intArray &vertexGridIndexMap, intArray *gridIndexVertexMap, 
		     intArray &initialFaces, intArray &initialFaceZones,
		     realArray & xyz_initial)
{

  int rangeDimension = cg.numberOfDimensions();
  int domainDimension = cg.numberOfGrids()>0 ? cg[0].domainDimension() : rangeDimension;

  removeHangingFaces(cg);

  // build the mapping arrays that keep track of where the front came from
  buildHybridVertexMappings(cg, vertexIndex, numberOfVertices, vertexIDMap, vertexGridIndexMap, gridIndexVertexMap, xyz_initial);

  if ( debug_Ugen ) cout <<"hybrid vertex mappings complete"<<endl;

  // generate the initial face data structures
  generateInitialFaceList(cg, vertexIndex, numberOfVertices, vertexIDMap, vertexGridIndexMap, gridIndexVertexMap, initialFaces, initialFaceZones);

  if ( !initialFaces.getLength(0) )
    {
      cout<<"no hole was found for the hybrid mesh generator!"<<endl;
      return;
    }

  // if this will be a surface grid, create a list of background mappings from the composite grid
  //   (pass it to the advancing front when the time is right)
  MappingInformation mapInfo;
  if ( rangeDimension==3 && domainDimension==2 )
    {
      createBackgroundMappings(cg,mapInfo);
      //GraphicsParameters csgp;
      
      //mapInfo.graphXInterface = ps;
      //mapInfo.mappingList[0].mapPointer->update(mapInfo);

      //      ps->plot(*(mapInfo.mappingList[0].mapPointer),csgp);
    }
  // try to create a mesh spacing control function, if that fails, then simply leave the
  // advancingFront without any spacing control, give a warning and let the user decide whether to go on 
  
  if ( rangeDimension==domainDimension)
    generateSpacingControlMesh(cg, initialFaceZones, xyz_initial);
  else if ( !advancingFront.getControlFunction().numberOfGrids() )
    generateSpacingControlMeshForSurface(initialFaces, xyz_initial);
  
  // look for any holes in the initial front, try to seal them
  // NOTE :: any new faces and/or vertices will be appened to initialFaces and xyz_initial
  intArray initialFaceSurfaceMapping(initialFaces.getLength(0));
  int nInitFaces = initialFaces.getLength(0);
  initialFaceSurfaceMapping = -1;
  sealHoles(cg, gridIndexVertexMap, initialFaces, xyz_initial, initialFaceSurfaceMapping);

  // fill in the rest of the initalFaceSurfaceMapping if this is a surface mesh
  if ( rangeDimension==3 && domainDimension==2 )
    {
      for ( int f=0; f<nInitFaces; f++ )
	{

	  int v1 = initialFaces(f,0);
	  int v2 = initialFaces(f,1);
	  
	  initialFaceSurfaceMapping(f) = max(vertexGridIndexMap(v1,0),vertexGridIndexMap(v2,0));
	}
    }
  

  // now lets try to initialize the advancingFront instance so we can generate a mesh
  // this will fail if the advancingFront gets confused.  

  try {
    if ( rangeDimension==3 && domainDimension==2 )
      advancingFront.initialize(initialFaces, xyz_initial, &mapInfo, initialFaceSurfaceMapping);
    else
      advancingFront.initialize(initialFaces, xyz_initial);

    if ( debug_Ugen ) cout <<"front initialized"<<endl;
  }
//   catch (DimensionError &e) {
//     e.debug_print();
//     errorReport(*ps, "FATAL Error : unstructured meshing is currently only supported in 2D");
//     throw e;
//   }
  catch (AdvancingFrontError &e) {
    e.debug_print();
    errorReport(*ps,"Problems were encountered trying to initialize the advancing front data structures\n"
		"Try plotting the composite grid and check to make sure the holes contain no overlaps"
		" exitting Ugen, returning to Ogen...");
    throw e;
  }

  // try to initialize the delaunay triangluator
  if ( cg.numberOfDimensions()==2 )
    {
      int ndim = cg.numberOfDimensions();
      Index AXES(0,ndim);
      
      realArray holePt(initialFaceZones.getLength(0), ndim);
      IntegerArray center(1,1,1,cg.numberOfDimensions());
      int nholes = 0;
      for ( int element = 0; element<initialFaceZones.getLength(0); element++ )
	{
	  int grid = initialFaceZones(element,0);
	  int i1   = initialFaceZones(element,1);
	  int i2   = initialFaceZones(element,2);
	  int i3   = initialFaceZones(element,3);
	  
	  const realArray & vertices = cg[grid].vertex();
	  //const realArray & centers  = cg[grid].center();
	  
	  if ( (i1>0 && i1<cg[grid].dimension(1,0)-2) &&
	       (i2>0 && i2<cg[grid].dimension(1,1)-2) )
	    {
	      int indices[3];
	      indices[2] = 0;
	      // compute the cell in the controlGrid that contains cg[grid]'s element center
	      realArray center(1,1,1,ndim);
	      // compute cell center
	      center = ( vertices(i1,i2,i3,AXES) + vertices(i1+1,i2,i3,AXES) + 
			 vertices(i1+1,i2+1,i3,AXES) + vertices(i1, i2+1,i3,AXES) )/4.0;
	      
	      //if ( domainDimension == 3 )
	      //  center = center/2.0 + ( vertices(i1,i2,i3+1,AXES) + vertices(i1+1,i2,i3+1,AXES) + 
	      //		    vertices(i1+1,i2+1,i3+1,AXES) + vertices(i1, i2+1,i3+1,AXES) )/8.0;
	      
	      center.reshape(1,ndim);
	      holePt(nholes++,AXES) = center(0,AXES);
	    }
	}
      
      
      holePt.resize(nholes,AXES.getLength());

      try {
	triangleWrapper.initialize(initialFaces, xyz_initial);
	triangleWrapper.setHoles(holePt);
	if ( !triangleWrapper.getParameters().getVoronoi() ) triangleWrapper.getParameters().toggleVoronoi();
	if ( !triangleWrapper.getParameters().getFreezeSegments() ) triangleWrapper.getParameters().toggleFreezeSegments();
	if ( debug_Ugen ) cout<<"triangle wrapper initialized"<<endl;
      } 
      catch (AbstractException &e) {
	e.debug_print();
	errorReport(*ps, "Problems were encountered attempting to initialize the delaunay mesh generator");
	throw e;
      }
    }

}

void
Ugen::
enlargeHole(CompositeGrid &cg, intArray &vertexGridIndexMap,int egrd)
{
  for ( int v=vertexGridIndexMap.getBase(0); v<=vertexGridIndexMap.getBound(0); v++ )
    {
      if ( vertexGridIndexMap(v,0)==egrd || egrd==-1 )
	{
	  MappedGrid & mappedGrid = cg[vertexGridIndexMap(v,0)];
	  intArray & mask = mappedGrid.mask();
	  int i1 = vertexGridIndexMap(v,1);
	  int i2 = vertexGridIndexMap(v,2);
	  int i3 = vertexGridIndexMap(v,3);
	  
	  Index Iv[3];
	  getIndex(mappedGrid.gridIndexRange(), Iv[0], Iv[1], Iv[2]);
	  // mask out the vertex if it is not on the boundary of the original structured grid
	  int nBdy = 0;
	  bool notPeriodic = false;
	  bool noSing = true;
	  int nPer = 0;
	  if ( mappedGrid.isPeriodic(0) ) nPer++;
	  if ( mappedGrid.isPeriodic(1) ) nPer++;
	  if ( mappedGrid.isPeriodic(2) ) nPer++;
	  
	  Mapping & map = mappedGrid.mapping().getMapping();
	  
	  if ( i1==Iv[0].getBase() || i1==Iv[0].getBound() ) 
	    {
	      nBdy++;
	      notPeriodic = notPeriodic || !(mappedGrid.isPeriodic(1) || mappedGrid.isPeriodic(2));
	      noSing = !( ( map.getTypeOfCoordinateSingularity(0,0)==Mapping::polarSingularity && i1==Iv[0].getBase() ) || 
			  ( map.getTypeOfCoordinateSingularity(1,0)==Mapping::polarSingularity && i1==Iv[0].getBound()) );
	    }
	  //	  cout<<"- "<<notPeriodic<<endl;
	  if ( i2==Iv[1].getBase() || i2==Iv[1].getBound() ) 
	    {
	      nBdy++;
	      notPeriodic = notPeriodic || !(mappedGrid.isPeriodic(0) || mappedGrid.isPeriodic(2));
	      noSing = noSing && !( ( map.getTypeOfCoordinateSingularity(0,1)==Mapping::polarSingularity && i2==Iv[1].getBase() ) || 
				    ( map.getTypeOfCoordinateSingularity(1,1)==Mapping::polarSingularity && i2==Iv[1].getBound()) );
	    }
	  //	  cout<<"  "<<notPeriodic<<endl;
	  if ( (i3==Iv[2].getBase() || i3==Iv[2].getBound()) && cg.numberOfDimensions()!=2) 
	    {
	      nBdy++;
	      notPeriodic = notPeriodic || !(mappedGrid.isPeriodic(0) || mappedGrid.isPeriodic(1));
	      noSing = noSing && !( ( map.getTypeOfCoordinateSingularity(0,2)==Mapping::polarSingularity && i3==Iv[2].getBase() ) || 
				    ( map.getTypeOfCoordinateSingularity(1,2)==Mapping::polarSingularity && i3==Iv[2].getBound()) );
	    }
	  
	  //	  cout<<nBdy<<" "<<nPer<<"  "<<notPeriodic<<"  "<<noSing<<endl;
	  if ( nBdy==0 )//|| (nBdy==1 && cg.numberOfDimensions()==3) )
	    mask(i1,i2,i3) = 0;
	  else if ( nBdy==1 )
	    { // maintain masks along periodic boundaries
	      // assumption, along periodic boundaries, only the base vertices are kept (not the bound)
	      if ( i1==Iv[0].getBase() && mappedGrid.isPeriodic(0) ) 
		mask(Iv[0].getBound(), i2, i3) = mask(i1,i2,i3) = 0;
	      else if ( i2==Iv[1].getBase() && mappedGrid.isPeriodic(1) ) 
		mask(i1, Iv[1].getBound(), i3) = mask(i1,i2,i3) = 0;
	      else if ( i3==Iv[2].getBase() && mappedGrid.isPeriodic(2) && cg.numberOfDimensions()>2 ) 
		mask(i1, i2, Iv[2].getBound()) = mask(i1,i2,i3) = 0;
	      else if ( mappedGrid.domainDimension()==3 )//&& notPeriodic )//&& ( notPeriodic || (noSing && !notPeriodic)) )
		{
		  //		  cout<<"blanking "<<vertexGridIndexMap(v,0)<<" : "<<i1<<" "<<i2<<" "<<i3<<endl;
		  mask(i1,i2,i3) = 0;
		}
	    }
	  else if ( nBdy==2 && mappedGrid.domainDimension()==3 )//&& !notPeriodic )
	    {
	      //	      cout<<"  checking edge boundary vertex "<<" : "<<i1<<" "<<i2<<" "<<i3<<endl;
	      //	      cout<<"  periodicity "<<mappedGrid.isPeriodic(0)<<"  "<<mappedGrid.isPeriodic(1)<<"  "<<mappedGrid.isPeriodic(2)<<endl;
	      
#if 0
	      if ( i1==Iv[0].getBase() && mappedGrid.isPeriodic(0) ) 
		mask(Iv[0].getBound(), i2, i3) = mask(i1,i2,i3) = 0;
	      if ( i2==Iv[1].getBase() && mappedGrid.isPeriodic(1) ) 
		mask(i1, Iv[1].getBound(), i3) = mask(i1,i2,i3) = 0;
	      if ( i3==Iv[2].getBase() && mappedGrid.isPeriodic(2) && cg.numberOfDimensions()>2 ) 
		mask(i1, i2, Iv[2].getBound()) = mask(i1,i2,i3) = 0;
#else
	      if ( nPer && !noSing )
	    mask(i1,i2,i3)=0;
#endif
	      //	      if ( mask(i1,i2,i3)==0 ) 
	      //		cout<<"masked out on boundary edge"<<" : "<<i1<<" "<<i2<<" "<<i3<<endl;
	    }
//       else if ( mappedGrid.domainDimension()==3 )//&& ( notPeriodic || (noSing && !notPeriodic)) )
// 	{
// 	  cout<<"blanking n "<<vertexGridIndexMap(v,0)<<" : "<<i1<<" "<<i2<<" "<<i3<<endl;
// 	  mask(i1,i2,i3) = 0;
// 	}
//       else if ( mappedGrid.domainDimension()==3 && !noSing )
// 	{
// 	  cout<<"blanking n bdy "<<vertexGridIndexMap(v,0)<<" : "<<i1<<" "<<i2<<" "<<i3<<endl;
// 	  if ( i1==Iv[0].getBase() && mappedGrid.isPeriodic(0) ) 
// 	    mask(Iv[0].getBound(), i2, i3) = mask(i1,i2,i3) = 0;
// 	  else if ( i2==Iv[1].getBase() && mappedGrid.isPeriodic(1) ) 
// 	    mask(i1, Iv[1].getBound(), i3) = mask(i1,i2,i3) = 0;
// 	  else if ( i3==Iv[2].getBase() && mappedGrid.isPeriodic(2) && cg.numberOfDimensions()>2 ) 
// 	    mask(i1, i2, Iv[2].getBound()) = mask(i1,i2,i3) = 0;
// 	}
	
	}
    }
}

void 
Ugen::
generateSpacingControlMeshForSurface(const intArray & initialFaces, const realArray &initial_vertices)
{
  // find the min and max of the initial vertices
  int rangeDimension=3; 
  Range VERTICES(initial_vertices.getBase(0), initial_vertices.getBound(0));
  Range AXES(0,rangeDimension-1); 
  realArray mins(1,rangeDimension);
  realArray maxs(1,rangeDimension);

  mins = REAL_MAX;
  maxs = -REAL_MAX;

  for ( int v=VERTICES.getBase(); v<VERTICES.getBound(); v++)
    {
      mins = min(mins, initial_vertices(v,AXES));
      maxs = max(maxs, initial_vertices(v,AXES));
    }

  mins.reshape(rangeDimension);
  maxs.reshape(rangeDimension);

  int axis;
  for ( axis=0; axis<rangeDimension; axis++ )
    {
      real disp = 0.15*(maxs(axis)-mins(axis));
      mins(axis) -= disp; 
      maxs(axis) += disp;
    }

  real avgLen = 0;
  for ( int f=0; f<initialFaces.getLength(0); f++ )
  {
      ArraySimpleFixed<real,3,1,1,1> edge,mid;
      for ( int a=0; a<3; a++ )
      {
          edge[a] = initial_vertices(initialFaces(f,1),a)-initial_vertices(initialFaces(f,0),a);
          mid[a] = .5*(initial_vertices(initialFaces(f,1),a)+initial_vertices(initialFaces(f,0),a));
      }
      real len = sqrt(ASmag2(edge));
      avgLen += len; 
  }
  avgLen /= real(initialFaces.getLength(0));
  real controlGridDelta = 2.*avgLen;
    // generate the control grid
  CompositeGrid  *controlGridPtr = new CompositeGrid(rangeDimension,1);
  CompositeGrid & controlGrid = *controlGridPtr;

  Mapping *controlMapping;

  if ( rangeDimension==2 )
    controlMapping = (Mapping *) new SquareMapping( mins(0), maxs(0), mins(1), maxs(1) );
  else
    controlMapping = (Mapping *) new BoxMapping( mins(0), maxs(0), mins(1), maxs(1), mins(2), maxs(2) );

  int controlGridDims[3];
  for ( axis=0; axis<rangeDimension; axis++ )
    {
      controlGridDims[axis] = max(4,int((maxs(axis)-mins(axis))/controlGridDelta)+1);
      controlMapping->setGridDimensions(axis, controlGridDims[axis]);
    }
  controlMapping->reinitialize();
  controlMapping->incrementReferenceCount();
  MappedGrid mappedGrid(*controlMapping);
  controlMapping->decrementReferenceCount();
  mappedGrid.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
  controlGrid[0].reference(mappedGrid);
  controlGrid.updateReferences();

  controlGridDelta  =(maxs(0) - mins(0))/(controlGridDims[0]-1);
  Range all;
  realCompositeGridFunction controlFunction(controlGrid, all, all, all, AXES, AXES);

  controlFunction = 0.;
  Index I1,I2,I3;
  getIndex(controlGrid[0].gridIndexRange(),I1,I2,I3);
  for ( int a=0; a<rangeDimension; a++ )
        controlFunction[0](I1,I2,I3,a,a) = 1./avgLen;
  advancingFront.setControlFunction(controlGrid, controlFunction);
}

void 
Ugen::
generateSpacingControlMesh(CompositeGrid &cg, const intArray & initialFaceZones, const realArray &initial_vertices)

{

  int rangeDimension = cg.numberOfDimensions();
  int domainDimension = cg[0].domainDimension();

  Range AXES(0,rangeDimension-1);

  // just use one MappedGrid and one MappedGridFunction for now

  // find the min and max of the initial vertices
  Range VERTICES(initial_vertices.getBase(0), initial_vertices.getBound(0));
  
  realArray mins(1,rangeDimension);
  realArray maxs(1,rangeDimension);

  mins = REAL_MAX;
  maxs = -REAL_MAX;

  for ( int v=VERTICES.getBase(); v<VERTICES.getBound(); v++)
    {
      mins = min(mins, initial_vertices(v,AXES));
      maxs = max(maxs, initial_vertices(v,AXES));
    }

  // adjust mins and maxes expanding the bounding box by, oh say, 15% on each side
  // XXX !this will not work for highly stretched grids
  mins.reshape(rangeDimension);
  maxs.reshape(rangeDimension);
  int axis;
  for ( axis=0; axis<rangeDimension; axis++ )
    {
      real disp = 0.15*(maxs(axis)-mins(axis));
      mins(axis) -= disp; 
      maxs(axis) += disp;
    }

  // determine sizing of the control grid
  //real controlGridDelta = (domainDimension==2) ? advancingFront.getAverageFaceSize() : sqrt(advancingFront.getAverageFaceSize());
  //  real controlGridDelta = 2*advancingFront.getAverageFaceSize();
  // XXX - don't really want to ask the advancingFront anything right now, it may not be initialized
  //       compute control grid size from magnitudes of the cell centered jacobians...
  // real controlGridDelta = advancingFront.getAverageFaceSize();

  int element;
  real controlGridDelta = 0;
  int cgDeltaCounter = 0;
  int i3inc = (domainDimension==3) ? 1 : 0;

  for ( element = 0; element<initialFaceZones.getLength(0); element++ )
    {
      int grid = initialFaceZones(element,0);
      int i1   = initialFaceZones(element,1);
      int i2   = initialFaceZones(element,2);
      int i3   = initialFaceZones(element,3);

      const realArray & vertices = cg[grid].vertex();
      int signForJac = int(cg[grid].mapping().getSignForJacobian());
      //const realArray & centers  = cg[grid].center();

      int indices[3];
      indices[2] = 0;

      // compute the transformation matrix, adding it into the appropriate vertices in the control grid
      realArray bases(rangeDimension, rangeDimension);

      // first find the bases of the transformation by computing the vectors between the centers
      // of opposing faces.
      for ( axis=0; axis<rangeDimension; axis++ ){
	bases(0, axis) = 0.25 * ( vertices(i1+1, i2  , i3,axis) - vertices(i1,i2,i3,axis) +
				  vertices(i1+1, i2+1, i3,axis) - vertices(i1,i2+1,i3,axis) +
				  vertices(i1+1, i2  , i3+i3inc,axis) - vertices(i1,i2,i3+i3inc,axis) +
				  vertices(i1+1, i2+1, i3+i3inc,axis) - vertices(i1,i2+1,i3+i3inc,axis) );

	bases(1, axis) = 0.25 * ( vertices(i1  , i2+1, i3,axis) - vertices(i1,i2,i3,axis) +
				  vertices(i1+1, i2+1, i3,axis) - vertices(i1+1,i2,i3,axis) +
				  vertices(i1  , i2+1, i3+i3inc,axis) - vertices(i1,i2,i3+i3inc,axis) +
				  vertices(i1+1, i2+1, i3+i3inc,axis) - vertices(i1+1,i2,i3+i3inc,axis) );
	
	if ( rangeDimension == 3 )
	  bases(2, axis) = 0.25 * ( vertices(i1  , i2 , i3+i3inc,axis) - vertices(i1,i2,i3,axis) +
				    vertices(i1+1, i2  , i3+i3inc,axis) - vertices(i1+1,i2,i3,axis) +
				    vertices(i1+1, i2+1, i3+i3inc,axis) - vertices(i1+1,i2+1,i3,axis) +
				    vertices(i1  , i2+1, i3+i3inc,axis) - vertices(i1,i2+1,i3,axis) );

      }

      controlGridDelta += sqrt(sum(pow(bases(0,AXES),2))) + sqrt(sum(pow(bases(1,AXES),2)));
      if ( rangeDimension==3 )
	controlGridDelta += sqrt(sum(pow(bases(2,AXES),2)));
					 
      cgDeltaCounter += rangeDimension;
    }

  controlGridDelta /= real(cgDeltaCounter);
  controlGridDelta *= 2.;

  cout<<"cg delta "<<controlGridDelta<<endl;

  // generate the control grid
  CompositeGrid  *controlGridPtr = new CompositeGrid(rangeDimension,1);
  CompositeGrid & controlGrid = *controlGridPtr;

  Mapping *controlMapping;

  if ( rangeDimension==2 )
    controlMapping = (Mapping *) new SquareMapping( mins(0), maxs(0), mins(1), maxs(1) );
  else
    controlMapping = (Mapping *) new BoxMapping( mins(0), maxs(0), mins(1), maxs(1), mins(2), maxs(2) );

  int controlGridDims[3];
  for ( axis=0; axis<rangeDimension; axis++ )
    {
      controlGridDims[axis] = max(4,int((maxs(axis)-mins(axis))/controlGridDelta)+1);
      controlMapping->setGridDimensions(axis, controlGridDims[axis]);
    }
  controlMapping->reinitialize();
  controlMapping->incrementReferenceCount();
  MappedGrid mappedGrid(*controlMapping);
  controlMapping->decrementReferenceCount();
  mappedGrid.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
  controlGrid[0].reference(mappedGrid);
  controlGrid.updateReferences();

  controlGridDelta  =(maxs(0) - mins(0))/(controlGridDims[0]-1);
  Range all;
  realCompositeGridFunction controlFunction(controlGrid, all, all, all, AXES, AXES);

  realCompositeGridFunction vertexCounterGF(controlGrid, all, all, all);
  realArray & vertexCounter = vertexCounterGF[0];

  realCompositeGridFunction theOriginalJacobians(cg, all,all,all, AXES,AXES);

  Index I1,I2,I3;
  getIndex(controlGrid[0].gridIndexRange(),I1,I2,I3);
  
  controlFunction = 0;
  real thedelta = controlGridDelta;
#if 0
  for ( axis=0; axis<rangeDimension; axis++ )
    controlFunction[0](I1,I2,I3,axis,axis) = 1.0/thedelta;
  vertexCounter = 1.0;
#else
  vertexCounter=0.0;
  cout<<"THEDELTA "<<2.0/thedelta<<endl;
#endif

#if 0
  // compute stretchings along the boundary of the hole and interpolate onto the control grid
  int grid;
  for ( grid =0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      cg[grid].update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEvertexDerivative);
      Index I1,I2,I3;
      getIndex(cg[grid].gridIndexRange(),I1,I2,I3);
      for (int ti=0; ti<rangeDimension; ti++)
	for ( int tj=0; tj<rangeDimension; tj++)
	  theOriginalJacobians[grid](I1,I2,I3,ti,tj) = cg[grid].vertexDerivative()(I1,I2,I3,ti,tj);
    }

  //  interpolateAllPoints(theOriginalJacobians, controlFunction);
#else

  // compute the geometry we need on the original composite grid
  int grid;
  for ( grid =0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      cout<<"sign for jacobian for grid "<<grid<<" "<<cg[grid].mapping().getSignForJacobian()<<endl;
      cg[grid].update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEvertexDerivative);
    }

#if 0
  
  for ( grid =0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      const realArray & vertices = cg[grid].vertex();
      const intArray & mask = cg[grid].mask();

      Index I1,I2,I3;
      getIndex(cg[grid].gridIndexRange(),I1,I2,I3);
      int signForJac = int(cg[grid].mapping().getSignForJacobian());

      int io1=1,io2=1,io3=1;
      if ( I1.getBase()==I1.getBound() ) io1=0;
      if ( I2.getBase()==I2.getBound() ) io2=0;
      if ( I3.getBase()==I3.getBound() ) io3=0;

     
      for ( int i1=I1.getBase(); i1<=I1.getBound()-io1; i1++ )
	for ( int i2=I2.getBase(); i2<=I2.getBound()-io2; i2++ )
	  for ( int i3=I3.getBase(); i3<=I3.getBound()-io3; i3++ )
	    {
	      int indices[3];
	      indices[2] = 0;
	      // compute the cell in the controlGrid that contains cg[grid]'s element center
	      realArray center(1,1,1,rangeDimension);
	      // compute cell center
	      center = ( vertices(i1,i2,i3,AXES) + vertices(i1+1,i2,i3,AXES) + 
			 vertices(i1+1,i2+1,i3,AXES) + vertices(i1, i2+1,i3,AXES) )/4.0;

	      if ( rangeDimension == 3 )
		center = center/2.0 + ( vertices(i1,i2,i3+1,AXES) + vertices(i1+1,i2,i3+1,AXES) + 
					vertices(i1+1,i2+1,i3+1,AXES) + vertices(i1, i2+1,i3+1,AXES) )/8.0;

	      center.reshape(rangeDimension);


	      bool inbox = false;

	      inbox = center(0)<=maxs(0) && center(0)>=mins(0) &&
		center(1)<=maxs(1) && center(1)>=mins(1);
		
	      if ( rangeDimension==3 )
		inbox = inbox && center(2)<=maxs(2) && center(2)>=mins(2);
	      
	      //center.display("center");
	      if ( inbox )
		{
		  //cout<<"inside box!"<<endl;
		  for ( axis=0; axis<rangeDimension; axis++ )
		    indices[axis] = int((center(axis) - mins(axis))/((maxs(axis)-mins(axis))/real(controlGridDims[axis]-1)));

      // compute the transformation matrix, adding it into the appropriate vertices in the control grid
		  realArray bases(rangeDimension, rangeDimension);
		  realArray scales(rangeDimension);

      // first find the bases of the transformation by computing the vectors between the centers
      // of opposing faces.
		  for ( axis=0; axis<rangeDimension; axis++ ){
		    bases(0, axis) = 0.25 * ( vertices(i1+1, i2  , i3,axis) - vertices(i1,i2,i3,axis) +
					      vertices(i1+1, i2+1, i3,axis) - vertices(i1,i2+1,i3,axis) +
					      vertices(i1+1, i2  , i3+i3inc,axis) - vertices(i1,i2,i3+i3inc,axis) +
					      vertices(i1+1, i2+1, i3+i3inc,axis) - vertices(i1,i2+1,i3+i3inc,axis) );

		    bases(1, axis) = 0.25 * ( vertices(i1  , i2+1, i3,axis) - vertices(i1,i2,i3,axis) +
					      vertices(i1+1, i2+1, i3,axis) - vertices(i1+1,i2,i3,axis) +
					      vertices(i1  , i2+1, i3+i3inc,axis) - vertices(i1,i2,i3+i3inc,axis) +
					      vertices(i1+1, i2+1, i3+i3inc,axis) - vertices(i1+1,i2,i3+i3inc,axis) );
	
		    if ( rangeDimension == 3 )
		      bases(2, axis) = 0.25 * ( vertices(i1  , i2 , i3+i3inc,axis) - vertices(i1,i2,i3,axis) +
						vertices(i1+1, i2  , i3+i3inc,axis) - vertices(i1+1,i2,i3,axis) +
						vertices(i1+1, i2+1, i3+i3inc,axis) - vertices(i1+1,i2+1,i3,axis) +
						vertices(i1  , i2+1, i3+i3inc,axis) - vertices(i1,i2+1,i3,axis) );
		  }
      
		  // find the stretching scale determined by the cell bases above and normalize the bases
#if 1
		  real maxscale = -REAL_MAX;
		  int maxsloc = -1;
		  for ( axis=0; axis<rangeDimension; axis++ )
		    {
		      scales(axis) = sqrt(sum(pow(bases(axis,AXES),2))); 
		      if ( scales(axis)>maxscale )
			{
			  maxsloc = axis;
			  maxscale = scales(axis);
			}
		    }

#if 1
		  for ( axis=0; axis<rangeDimension; axis++ )
		    {
		      if ( axis!=maxsloc )
			{ // project out the component of the basis vector that is parallel to the longest stretching direction
			  bases(axis,AXES) = bases(axis,AXES) - 
			    sum(bases(maxsloc,AXES)*bases(axis,AXES))*bases(maxsloc,AXES)/(scales(maxsloc)*scales(maxsloc));
			  scales(axis) = sqrt(sum(pow(bases(axis,AXES),2)));
			  bases(axis,AXES) = bases(axis, AXES)/scales(axis);
			  //scales(axis)/=2.; // divide by 2 so pyramids make nice elements
			}
	  
		      //cout<<"grid, i1,i2,i3 "<<grid<<" "<<i1<<" "<<i2<<" "<<i3<<endl;
		      //bases(axis,AXES).display("bases");
		    }
      
		  bases(maxsloc,AXES) = bases(maxsloc,AXES)/scales(maxsloc);
#endif
		  //scales(maxsloc)/=2.; // divide by 2 so pyramids make nice elements
#else
		  for ( axis=0; axis<rangeDimension; axis++ )
		    {
		      scales(axis) = sqrt(sum(pow(bases(axis,AXES),2)));
		      bases(axis,AXES) = bases(axis, AXES)/scales(axis);
		    }
#endif

		  // compute the transformation matrix and add it to the vertices in the control mesh for later averaging
		  //scales.display("scales");
		  if (domainDimension==2)
		    for ( axis=0; axis<rangeDimension; axis++ )
		      for ( int ti=0; ti<rangeDimension; ti++ )
			for ( int tj=0; tj<rangeDimension; tj++ )
			  {
		
			    real comp = bases(axis, ti) * bases(axis,tj)/scales(axis);
			    //if ( mask(i1,i2,i3)>0 ) comp *=2.;
			    controlFunction[0](indices[0]  ,indices[1],indices[2],ti,tj) += comp;
			    controlFunction[0](indices[0]+1,indices[1],indices[2],ti,tj) += comp;
			    controlFunction[0](indices[0]+1,indices[1]+1,indices[2],ti,tj) += comp;
			    controlFunction[0](indices[0]  ,indices[1]+1,indices[2],ti,tj) += comp;
			  }
		  else
		    for ( axis=0; axis<rangeDimension; axis++ )
		      for ( int ti=0; ti<rangeDimension; ti++ )
			for ( int tj=0; tj<rangeDimension; tj++ )
			  {
			    real comp = bases(axis, ti) * bases(axis,tj)/scales(axis);
			    //if ( mask(i1,i2,i3)>0 ) comp *=2.;
			    controlFunction[0](indices[0]  ,indices[1],indices[2],ti,tj) += comp;
			    controlFunction[0](indices[0]+1,indices[1],indices[2],ti,tj) += comp;
			    controlFunction[0](indices[0]+1,indices[1]+1,indices[2],ti,tj) += comp;
			    controlFunction[0](indices[0]  ,indices[1]+1,indices[2],ti,tj) += comp;
			    controlFunction[0](indices[0]  ,indices[1],indices[2]+1,ti,tj) += comp;
			    controlFunction[0](indices[0]+1,indices[1],indices[2]+1,ti,tj) += comp;
			    controlFunction[0](indices[0]+1,indices[1]+1,indices[2]+1,ti,tj) += comp;
			    controlFunction[0](indices[0]  ,indices[1]+1,indices[2]+1,ti,tj) += comp;
			  }
      
		  // increment the affected control grid vertex's vertexCounter for later averaging
		  if (domainDimension==2)
		    { 
		      int incr = 1;
		      //if ( mask(i1,i2,i3)>0 ) incr=2;
		      vertexCounter(indices[0],   indices[1],   indices[2])+=incr;
		      vertexCounter(indices[0]+1, indices[1],   indices[2])+=incr;
		      vertexCounter(indices[0]+1, indices[1]+1, indices[2])+=incr;
		      vertexCounter(indices[0],   indices[1]+1, indices[2])+=incr;
		    }
		  else
		    {      
		      int incr = 1;
		      //if ( mask(i1,i2,i3)>0 ) incr=2;
		      vertexCounter(indices[0],   indices[1],   indices[2])+=incr;
		      vertexCounter(indices[0]+1, indices[1],   indices[2])+=incr;
		      vertexCounter(indices[0]+1, indices[1]+1, indices[2])+=incr;
		      vertexCounter(indices[0],   indices[1]+1, indices[2])+=incr;
		      vertexCounter(indices[0],   indices[1],   indices[2]+1)+=incr;
		      vertexCounter(indices[0]+1, indices[1],   indices[2]+1)+=incr;
		      vertexCounter(indices[0]+1, indices[1]+1, indices[2]+1)+=incr;
		      vertexCounter(indices[0],   indices[1]+1, indices[2]+1)+=incr;
		    }
		}
	      
	    }
    }
      
#else

  for ( element = 0; element<initialFaceZones.getLength(0); element++ )
    {
      int grid = initialFaceZones(element,0);
      int i1   = initialFaceZones(element,1);
      int i2   = initialFaceZones(element,2);
      int i3   = initialFaceZones(element,3);

      const realArray & vertices = cg[grid].vertex();
      int signForJac = int(cg[grid].mapping().getSignForJacobian());
      //const realArray & centers  = cg[grid].center();

      int indices[3];
      indices[2] = 0;
      // compute the cell in the controlGrid that contains cg[grid]'s element center
      realArray center(1,1,1,rangeDimension);
      // compute cell center
      center = ( vertices(i1,i2,i3,AXES) + vertices(i1+1,i2,i3,AXES) + 
		 vertices(i1+1,i2+1,i3,AXES) + vertices(i1, i2+1,i3,AXES) )/4.0;

      if ( rangeDimension == 3 )
	center = center/2.0 + ( vertices(i1,i2,i3+1,AXES) + vertices(i1+1,i2,i3+1,AXES) + 
				vertices(i1+1,i2+1,i3+1,AXES) + vertices(i1, i2+1,i3+1,AXES) )/8.0;

      center.reshape(rangeDimension);

      for ( axis=0; axis<rangeDimension; axis++ )
	indices[axis] = int((center(axis) - mins(axis))/((maxs(axis)-mins(axis))/real(controlGridDims[axis]-1)));

      // compute the transformation matrix, adding it into the appropriate vertices in the control grid
      realArray bases(rangeDimension, rangeDimension);
      realArray scales(rangeDimension);

      // first find the bases of the transformation by computing the vectors between the centers
      // of opposing faces.
      for ( axis=0; axis<rangeDimension; axis++ ){
	bases(0, axis) = 0.25 * ( vertices(i1+1, i2  , i3,axis) - vertices(i1,i2,i3,axis) +
				  vertices(i1+1, i2+1, i3,axis) - vertices(i1,i2+1,i3,axis) +
				  vertices(i1+1, i2  , i3+i3inc,axis) - vertices(i1,i2,i3+i3inc,axis) +
				  vertices(i1+1, i2+1, i3+i3inc,axis) - vertices(i1,i2+1,i3+i3inc,axis) );

	bases(1, axis) = 0.25 * ( vertices(i1  , i2+1, i3,axis) - vertices(i1,i2,i3,axis) +
				  vertices(i1+1, i2+1, i3,axis) - vertices(i1+1,i2,i3,axis) +
				  vertices(i1  , i2+1, i3+i3inc,axis) - vertices(i1,i2,i3+i3inc,axis) +
				  vertices(i1+1, i2+1, i3+i3inc,axis) - vertices(i1+1,i2,i3+i3inc,axis) );
	
	if ( rangeDimension == 3 )
	  bases(2, axis) = 0.25 * ( vertices(i1  , i2 , i3+i3inc,axis) - vertices(i1,i2,i3,axis) +
				    vertices(i1+1, i2  , i3+i3inc,axis) - vertices(i1+1,i2,i3,axis) +
				    vertices(i1+1, i2+1, i3+i3inc,axis) - vertices(i1+1,i2+1,i3,axis) +
				    vertices(i1  , i2+1, i3+i3inc,axis) - vertices(i1,i2+1,i3,axis) );
      }
      
      // find the stretching scale determined by the cell bases above and normalize the bases
#if 1
      real maxscale = -REAL_MAX;
      int maxsloc = -1;
      for ( axis=0; axis<rangeDimension; axis++ )
	{
	  scales(axis) = sqrt(sum(pow(bases(axis,AXES),2))); 
	  if ( scales(axis)>maxscale )
	    {
	      maxsloc = axis;
	      maxscale = scales(axis);
	    }
	}

#if 1
      for ( axis=0; axis<rangeDimension; axis++ )
	{
	  if ( axis!=maxsloc )
	    { // project out the component of the basis vector that is parallel to the longest stretching direction
	      bases(axis,AXES) = bases(axis,AXES) - 
		sum(bases(maxsloc,AXES)*bases(axis,AXES))*bases(maxsloc,AXES)/(scales(maxsloc)*scales(maxsloc));
	      scales(axis) = sqrt(sum(pow(bases(axis,AXES),2)));
	      bases(axis,AXES) = bases(axis, AXES)/scales(axis);
	      //scales(axis)/=2.; // divide by 2 so pyramids make nice elements
	    }
	  
	  //cout<<"grid, i1,i2,i3 "<<grid<<" "<<i1<<" "<<i2<<" "<<i3<<endl;
	  //bases(axis,AXES).display("bases");
	}
      
      bases(maxsloc,AXES) = bases(maxsloc,AXES)/scales(maxsloc);
#endif
      //scales(maxsloc)/=2.; // divide by 2 so pyramids make nice elements
#else
      for ( axis=0; axis<rangeDimension; axis++ )
	{
	  scales(axis) = sqrt(sum(pow(bases(axis,AXES),2)));
	  bases(axis,AXES) = bases(axis, AXES)/scales(axis);
	}
#endif

      // compute the transformation matrix and add it to the vertices in the control mesh for later averaging
      if (domainDimension==2)
	for ( axis=0; axis<rangeDimension; axis++ )
	  for ( int ti=0; ti<rangeDimension; ti++ )
	    for ( int tj=0; tj<rangeDimension; tj++ )
	      {
		
		real comp = bases(axis, ti) * bases(axis,tj)/scales(axis);
		controlFunction[0](indices[0]  ,indices[1],indices[2],ti,tj) += comp;
		controlFunction[0](indices[0]+1,indices[1],indices[2],ti,tj) += comp;
		controlFunction[0](indices[0]+1,indices[1]+1,indices[2],ti,tj) += comp;
		controlFunction[0](indices[0]  ,indices[1]+1,indices[2],ti,tj) += comp;
	      }
      else
	for ( axis=0; axis<rangeDimension; axis++ )
	  for ( int ti=0; ti<rangeDimension; ti++ )
	    for ( int tj=0; tj<rangeDimension; tj++ )
	      {
		real comp = bases(axis, ti) * bases(axis,tj)/scales(axis);
		controlFunction[0](indices[0]  ,indices[1],indices[2],ti,tj) += comp;
		controlFunction[0](indices[0]+1,indices[1],indices[2],ti,tj) += comp;
		controlFunction[0](indices[0]+1,indices[1]+1,indices[2],ti,tj) += comp;
		controlFunction[0](indices[0]  ,indices[1]+1,indices[2],ti,tj) += comp;
		controlFunction[0](indices[0]  ,indices[1],indices[2]+1,ti,tj) += comp;
		controlFunction[0](indices[0]+1,indices[1],indices[2]+1,ti,tj) += comp;
		controlFunction[0](indices[0]+1,indices[1]+1,indices[2]+1,ti,tj) += comp;
		controlFunction[0](indices[0]  ,indices[1]+1,indices[2]+1,ti,tj) += comp;
	      }
      
      // increment the affected control grid vertex's vertexCounter for later averaging
      if (domainDimension==2)
	{ 
	  vertexCounter(indices[0],   indices[1],   indices[2])+=1;
	  vertexCounter(indices[0]+1, indices[1],   indices[2])+=1;
	  vertexCounter(indices[0]+1, indices[1]+1, indices[2])+=1;
	  vertexCounter(indices[0],   indices[1]+1, indices[2])+=1;
	}
      else
	{      
	  vertexCounter(indices[0],   indices[1],   indices[2])++;
	  vertexCounter(indices[0]+1, indices[1],   indices[2])++;
	  vertexCounter(indices[0]+1, indices[1]+1, indices[2])++;
	  vertexCounter(indices[0],   indices[1]+1, indices[2])++;
	  vertexCounter(indices[0],   indices[1],   indices[2]+1)++;
	  vertexCounter(indices[0]+1, indices[1],   indices[2]+1)++;
	  vertexCounter(indices[0]+1, indices[1]+1, indices[2]+1)++;
	  vertexCounter(indices[0],   indices[1]+1, indices[2]+1)++;
	}
      
    }

#endif
  
  // now complete the averaging of the transformation on the control mesh
  //RealArray & controlFunctionArray = ( RealArray &) controlFunction[0];

  intArray originalMask = vertexCounter(I1,I2,I3)>1;
#if 0
  where ( originalMask(I1,I2,I3) )
    { 
      for ( int ti=0; ti<domainDimension; ti++ )
	for ( int tj=0; tj<domainDimension; tj++ )
	  controlFunction[0](I1,I2,I3,ti,tj) = 
	    controlFunction[0](I1,I2,I3,ti,tj)/vertexCounter[0](I1,I2,I3);
    }
#else
  for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
    for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	if ( vertexCounter(i1,i2,i3)>0 )
	  {
	    //cout<<"vertex counter "<<i1<<" "<<i2<<" "<<i3<<" = "<<vertexCounter(i1,i2,i3)<<endl;
	    for ( int ti=0; ti<rangeDimension; ti++ )
	      for ( int tj=0; tj<rangeDimension; tj++ )
		controlFunction[0](i1,i2,i3,ti,tj) = 
		  controlFunction[0](i1,i2,i3,ti,tj)/vertexCounter(i1,i2,i3);
	  }
	else
	  for ( int ti=0; ti<rangeDimension; ti++ )
	    for ( int tj=0; tj<rangeDimension; tj++ )
	      if ( ti==tj )
		controlFunction[0](i1,i2,i3,ti,tj) = 2./thedelta;
	      else
		controlFunction[0](i1,i2,i3,ti,tj) = 0.;
	
  //else
  //cout<<"vertex counter -- void "<<i1<<" "<<i2<<" "<<i3<<" = "<<vertexCounter(i1,i2,i3)<<endl;
  
#endif

  controlFunction.periodicUpdate();

  // smooth the control grid
  CompositeGridOperators op(controlGrid);
  controlFunction.setOperators(op);

  realCompositeGridFunction originalControlFunction(controlGrid, all, all, all, AXES,AXES);
  originalControlFunction[0] = controlFunction[0];
  //  for ( int ti=0; ti<domainDimension; ti++ )
  // for ( int tj=0; tj<domainDimension; tj++ )  
  //   originalControlFunction[0](I1,I2,I3,ti,tj) = controlFunction[0](I1,I2,I3,ti,tj);

  int nIterations = 2;//*max(I1.getLength(),I2.getLength(),I3.getLength());//100;

  cout<<"NITERATIONS "<<nIterations<<endl;
  //nIterations = 2;
#if 0
  RealArray & cf = controlFunction[0];
  RealArray &ocf = originalControlFunction[0];
#else
  RealMappedGridFunction &cf = controlFunction[0];
  RealMappedGridFunction &ocf= originalControlFunction[0];
#endif

  if ( rangeDimension == 2 )
    for ( int it = 0; it<nIterations; it++ )
      {
	// relax each component in the transformation using a jacobi iteration
	// there is some redundant work done since the matrix is symmetric
	int i1,i2,i3;
	i3 = 0;
#if 1
	for ( int ti=0; ti<rangeDimension; ti++ )
	  for ( int tj=0; tj<rangeDimension; tj++ )
	    cf(I1,I2,I3,ti,tj) = ( cf(I1-1,I2,I3,ti,tj) +
				   cf(I1,I2-1,I3,ti,tj) +
				   cf(I1+1,I2,I3,ti,tj) +
				   cf(I1,I2+1,I3,ti,tj))/4.0;
#else
	for ( int tt=0; tt<domainDimension*domainDimension; tt++ )
	  cf(I1,I2,I3,tt) = ( cf(I1-1,I2,I3,tt) +
			      cf(I1,I2-1,I3,tt) +
			      cf(I1+1,I2,I3,tt) +
			      cf(I1,I2+1,I3,tt))/4.0;
#endif
	// now reset the original locations
#if 0
#if 0
	where ( originalMask(I1,I2,I3) )
	  { 
	    for ( int ti=0; ti<domainDimension; ti++ )
	      for ( int tj=0; tj<domainDimension; tj++ )
		controlFunction[0](I1,I2,I3,ti,tj) = originalControlFunction[0](I1,I2,I3,ti,tj);
	  }
#else
	for ( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  for ( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	    for ( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	      if ( vertexCounter(i1,i2,i3)>0 )
		for ( int tt=0; tt<domainDimension*domainDimension; tt++ )
		  cf(i1,i2,i3,tt) = ocf(i1,i2,i3,tt);
#endif
#endif
	controlFunction.periodicUpdate();
	
      }
  else
    for ( int it = 0; it<nIterations; it++ )
      {
	// relax each component in the transformation using a jacobi iteration
	// there is some redundant work done since the matrix is symmetric

	int i1,i2,i3;

	for ( int ti=0; ti<rangeDimension; ti++ )
	  for ( int tj=0; tj<rangeDimension; tj++ )
	    controlFunction[0](I1,I2,I3,ti,tj) = ( controlFunction[0](I1-1,I2,I3,ti,tj) +
						   controlFunction[0](I1,I2-1,I3,ti,tj) +
						   controlFunction[0](I1+1,I2,I3,ti,tj) +
						   controlFunction[0](I1,I2+1,I3,ti,tj) +
						   controlFunction[0](I1,I2,I3-1,ti,tj) +
						   controlFunction[0](I1,I2,I3+1,ti,tj)
						   )/6.0;
	
	// now reset the original locations
#if 0
#if 0
	where ( originalMask(I1,I2,I3) )
	  { 
	    for ( int ti=0; ti<domainDimension; ti++ )
	      for ( int tj=0; tj<domainDimension; tj++ )
		controlFunction[0](I1,I2,I3,ti,tj) = originalControlFunction[0](I1,I2,I3,ti,tj);
	  }
#else
	for ( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  for ( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	    for ( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	      if ( vertexCounter(i1,i2,i3)>0 )
		for ( int tt=0; tt<rangeDimension*rangeDimension; tt++ )
		  cf(i1,i2,i3,tt) = ocf(i1,i2,i3,tt);
#endif
#endif

	controlFunction.periodicUpdate();
	
      }
#endif

  //  controlFunction[0].display("cf");
  advancingFront.setControlFunction(controlGrid, controlFunction);

}

void
Ugen::
generateWithAdvancingFront()
{
  real t0 = getCPU();
  
  try {

    advancingFront.advanceFront();

    real dt = getCPU()-t0;
    int nFaces = advancingFront.getNumberOfFaces();
    cout << "number of faces : "<<nFaces<<endl;
    cout << "number of elements : "<<advancingFront.getNumberOfElements()<<endl;
    cout << "generation time    : "<<dt<<endl;
    cout << "generation time/nFaces, generation time/nFace^2    : "<<dt/nFaces<<" "<<dt/(nFaces*nFaces)<<endl;
    
    if ( advancingFront.getRangeDimension()==advancingFront.getDomainDimension() && 
	 advancingFront.isFrontEmpty() )
      {
	intArray elementList = advancingFront.generateElementList();
	
	Range R(0,advancingFront.getNumberOfVertices()-1);
	Range AXES(0,advancingFront.getRangeDimension()-1);
	
	realArray &xyz = (realArray &)advancingFront.getVertices();
	realArray xyzt(R,AXES);
	xyzt(R,AXES) = xyz(R,AXES);
	UnstructuredMapping umap;
	umap.setNodesAndConnectivity(xyzt,elementList, advancingFront.getDomainDimension());
	
	cout<<"optimizing..."<<endl;
	optimize(umap, (RealCompositeGridFunction *)&advancingFront.getControlFunction());
	//	optimize(umap,NULL);
	cout<<" done!"<<endl;
	
	xyz(R,AXES) = umap.getNodes()(R,AXES);

	MeshQualityMetrics mq(umap);
 	MetricCGFunctionEvaluator metricEval((RealCompositeGridFunction *)&advancingFront.getControlFunction());
 	//	mq.setReferenceTransformation((RealCompositeGridFunction *)&advancingFront.getControlFunction());
 	mq.setReferenceTransformation(&metricEval);

	mq.outputHistogram();
      }
	
  }
  catch(AdvancingFrontError &e)
    {
      e.debug_print();
      cerr<<endl;
      errorReport(*ps,"** Unstructured Mesh Front Advancement FAILED **"
		  "A weakness in the algorithm may have been exploited, including one of the following : \n"
		  "-- Holes in the initial front \n"
		  "-- Edge intersection and parallelism checks ran afoul of thier tolerances\n"
		  "-- Any of several other things, please report this as a problem..."
		  "plotting remnants of the mesh");
      throw e;
    }
  catch(GeometricADTError &e)
    {
      e.debug_print();
      cerr<<endl;
      errorReport(*ps, "A low-level error occured while generating the mesh");
      throw e;
    }
  catch(AbstractException &e)
    {
      e.debug_print();
      throw e;
    }
}

void
Ugen::
generateWithTriangle()
{

  triangleWrapper.generate();

  const realArray &points = triangleWrapper.getPoints(); 

  const intArray &elems = triangleWrapper.generateElementList();

  delaunayMesh.setNodesAndConnectivity(points, elems);

}
