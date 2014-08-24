//#define BOUNDS_CHECK

#include "GL_GraphicsInterface.h"
#include "UnstructuredMapping.h"
#include "PlotIt.h"

void PlotIt:: 
plotUM(GenericGraphicsInterface &gi, UnstructuredMapping & map,
       GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */,
       int dList /* =0 */ ,
       bool lit /* =FALSE */)
// ==============================================================================================
// ==============================================================================================
{
  if( !gi.isGraphicsWindowOpen() )
    return;

  GUIState gui;

  glDisable(GL_POLYGON_OFFSET_FILL);
  // printf("plotUM: map=%s, dList=%i lit=%i\n",(const char*)map.getName(Mapping::mappingName),dList,lit);

// save the current window number 
  int startWindow = gi.getCurrentWindow();

  const realArray & x0 = map.getNodes();
  GraphicsParameters localParameters(TRUE);  // TRUE means this gets default values
  GraphicsParameters & par = parameters.isDefault() ? localParameters : parameters;

  bool plotObjectAndExit = par.plotObjectAndExit;
  bool plotObject        = par.plotObject;

  bool & plotWireFrame            = par.plotWireFrame; // not used any more (always off)
  bool & plotNormals              = par.plotMappingNormals;
  int & numberOfGhostLinesToPlot  = par.numberOfGhostLinesToPlot;
  bool & plotShadedMappingBoundaries   = par.plotShadedMappingBoundaries;
  bool & plotLinesOnMappingBoundaries= par.plotLinesOnMappingBoundaries;
  bool & plotGridPointsOnCurves   = par.plotGridPointsOnCurves;
  const real & surfaceOffset            = par.surfaceOffset;
  bool & labelGridsAndBoundaries  = par.labelGridsAndBoundaries;

  bool & plotNodes = par.plotUnsNodes;
  bool & plotFaces = par.plotUnsFaces;
  bool & plotEdges = par.plotUnsEdges;
  bool & plotBoundaryEdges = par.plotUnsBoundaryEdges;
  bool & useCutPlane = par.useUnsCutplane;

  bool plotGhosts = par.numberOfGhostLinesToPlot;

  RealArray & cutplaneVertex = par.unsCutplaneVertex; 
  //parameters.get(GI_UNS_CUTPLANE_VERTEX,cutplaneVertex);
  RealArray & cutplaneNormal = par.unsCutplaneNormal;
  //parameters.get(GI_UNS_CUTPLANE_VERTEX,cutplaneNormal);

  aString answer;
  aString menu[] = 
  {
    "!Unstructured mapping plotter",
    "erase and exit",
    " ",
    "plot nodes",
    "do not plot nodes",
    "plot 3D faces",
    "do not plot 3D faces",
    "plot boundary edges",
    "do not plot boundary edges",
    "plot 3D edges",
    "do not plot 3D edges",
    "plot normals (toggle)",
//                      "colour boundaries by boundary condition number (toggle)",
//                      "colour boundaries by share value (toggle)",
//    "plot shaded surfaces (3D) toggle",
//                      "plot grid points on curves (toggle)",
//    "plot grid lines on boundaries (3D) toggle",
    //        "plot ghosts",
//    "plot",
    "set colour",
//    "erase",
//    "exit",
    "" };


  enum tbEnum {
    pNodes=0,
    pFaces,
    p3DFaces,
    pBdyFaces,
    p3DEdges,
    pNormals,
    meshCut,
    pGhost,
    numberOftbEnums};

  aString tbLabels[] = { "Plot Nodes",
                         "plot faces",
			 "Plot 3D Faces",
			 "Plot Bndry Edges",
			 "Plot 3D Edges",
			 "Plot Normals",
			 "Mesh Cut Plane",
			 "Plot Ghost Entities",
			 "" };

  aString tbCommands[] = {"plot nodes (toggle)",
                          "plot faces",
			  "plot 3D faces (toggle)",
			  "plot bdy edges (toggle)",
			  "plot 3D edges (toggle)",
			  "plot normals (toggle)",
			  "mesh cut plane",
			  "Plot Ghost Entities",
			  ""};

  int tbStateInit[] = { plotNodes,
			plotFaces,
			plotBoundaryEdges,
			plotEdges,
			plotNormals,
			plotGhosts,
			useCutPlane };

  UnstructuredMapping::EntityTypeEnum highlightType = UnstructuredMapping::Vertex;
  int highlightIDX = -1;

  enum txtEnum {
    cutPlaneVert = 0,
    cutPlaneNorm,
    highlight,
    nTxt
  };

  aString txtBoxes[] = { "Cut Plane Vertex",
			 "Cut Plane Normal",
			 "Highlight Entity",
			 ""};
  aString txtInit[4];

// Radio box
  enum plotStyleEnum { plotAll=0, plotOneElement, plotOneFace };
  static plotStyleEnum plotStyle=plotAll;

  aString rbCommands[] = {"plot option all elements", "plot option one element",
			  "plot option one face", ""};
  aString rbLabel[] = {"All Elements", "One Element", "One Face", "" };
  gui.addRadioBox( "Plotting option", rbCommands, rbLabel, plotStyle, 3); // 3 columns

  if( !par.plotObjectAndExit )
  {
    gui.buildPopup(menu);
    gui.setExitCommand("exit","Exit");

    gui.setToggleButtons(tbCommands,tbLabels,tbStateInit,2);

    sPrintF(txtInit[0],"%f, %f, %f",cutplaneVertex(0),cutplaneVertex(1),cutplaneVertex(2));
    sPrintF(txtInit[1],"%f, %f, %f",cutplaneNormal(0),cutplaneNormal(1),cutplaneNormal(2));
    sPrintF(txtInit[2],"%s %d",UnstructuredMapping::EntityTypeStrings[highlightType].c_str(),highlightIDX);

    gui.setTextBoxes(txtBoxes,txtBoxes,txtInit);

    gi.pushGUI(gui);
  }

  gi.setKeepAspectRatio(true); 

  // do not plot ghost lines:  This can take too long and is usually unnecessary 
  // on Trimmed surfaces.
  const int oldNumberOfGhostLinesToPlot= numberOfGhostLinesToPlot;
  numberOfGhostLinesToPlot=0;

  const int domainDimension=map.getDomainDimension();
  const int rangeDimension=map.getRangeDimension();

  int axis;
  // first determine bounds on the mapping
  Bound b;
  RealArray xBound(2,3); xBound=0.;
  for( axis=0; axis<rangeDimension; axis++ )
  {
    b = map.getRangeBound(Start,axis);
    if( b.isFinite() )
      xBound(Start,axis)=(real)b;
    b = map.getRangeBound(End,axis);
    if( b.isFinite() )
      xBound(End,axis)=(real)b;
  }
  // xBound.display(" plotAListOfMappings: **** xBound **** ");
  gi.setGlobalBound(xBound);

  // erase();

  // set default prompt
  gi.appendToTheDefaultPrompt("plot(Unstructured)>");

  int len;
  // **** Plotting loop *****
  for(int it=0;;it++)
  {

    if ( useCutPlane )
      {
	gui.setSensitive(true,DialogData::textBoxWidget,0);
	gui.setSensitive(true,DialogData::textBoxWidget,1);
      }
    else
      {
	gui.setSensitive(false,DialogData::textBoxWidget,0);
	gui.setSensitive(false,DialogData::textBoxWidget,1);
      }


    if( it==0 && plotObject )
      answer="plot";               // plot first time through if plotObject==TRUE
    else if( it==1 && par.plotObjectAndExit )
      answer="exit";
    else
      gi.getAnswer(answer,"");

// make sure that the currentWindow is the same as startWindow! (It might get changed 
// interactively by the user)
    if (gi.getCurrentWindow() != startWindow)
      gi.setCurrentWindow(startWindow);

    if ( gui.getToggleValue(answer, tbLabels[pGhost], plotGhosts) ) 
      {	
	map.eraseUnstructuredMapping(gi); // this makes things gets new display lists
      }
    else if( answer.matches("plot normals (toggle)") )
    {
      plotNormals= !plotNormals;
    }
    else if( answer.matches("plot nodes (toggle)") )
    {
      plotNodes = !plotNodes;
    }
    else if( answer=="plot nodes" )
    {
      plotNodes=TRUE;
    }
    else if( answer=="do not plot nodes" )
    {
      plotNodes=FALSE;
    }
    else if( (len=answer.matches("plot faces")) )
    {
      int value=plotFaces;
      sScanF(answer(len,answer.length()-1),"%i",&value);
      plotFaces = value;
      printf(" plot faces=%i\n",plotFaces);
      
    }
    else if( answer.matches("plot 3D faces (toggle)") )
    {
      if (rangeDimension == 3)
	plotFaces = !plotFaces;
    }
    else if( answer=="plot 3D faces" )
    {
      if (rangeDimension == 3)
	plotFaces=TRUE;
    }
    else if( answer=="do not plot 3D faces" )
    {
      if (rangeDimension == 3)
	plotFaces=FALSE;
    } 
    else if( answer.matches("plot 3D edges (toggle)") )
    {
      plotEdges = !plotEdges;
    }
    else if( answer=="plot 3D edges" )
    {
      plotEdges=TRUE;
    }
    else if( answer=="do not plot 3D edges" )
    {
      plotEdges=FALSE;
    }
    else if( answer.matches("plot boundary edges (toggle)") )
    {
      if (rangeDimension == 3)
	plotBoundaryEdges=!plotBoundaryEdges;
      else
	plotFaces=!plotFaces;

    }
    else if( answer.matches("plot bdy edges (toggle)") )
    {
      if (rangeDimension == 3)
	plotBoundaryEdges=!plotBoundaryEdges;
      else
	plotFaces=!plotFaces;
    }
    else if( answer=="plot boundary edges" )
    {
      if (rangeDimension == 3)
	plotBoundaryEdges=TRUE;
      else
	plotFaces=TRUE;
    }
    else if( answer=="do not plot boundary edges" )
    {
      if (rangeDimension == 3)
	plotBoundaryEdges=FALSE;
      else
	plotFaces=FALSE;
    }
//      else if ( answer.matches("plot wire frame (toggle)") )
//        {
//  	plotWireFrame = !plotWireFrame;
//        }
    else if ( answer.matches("mesh cut plane") )
      {
	useCutPlane = !useCutPlane;

	if ( useCutPlane ) map.eraseUnstructuredMapping(gi);
      }
    else if ( (len=answer.matches("Cut Plane Vertex")) )
      {
	sScanF(answer(len,answer.length()-1),"%e %e %e",&cutplaneVertex(0),&cutplaneVertex(1),&cutplaneVertex(2));
	cutplaneVertex.display("cutplaneVertex");
	if ( useCutPlane ) map.eraseUnstructuredMapping(gi);
      }
    else if ( (len=answer.matches("Cut Plane Normal")) )
      {
	sScanF(answer(len,answer.length()-1),"%e %e %e",&cutplaneNormal(0),&cutplaneNormal(1),&cutplaneNormal(2));
	cutplaneNormal.display("cutplaneNormal");
	if ( useCutPlane ) map.eraseUnstructuredMapping(gi);
      }
    else if ( (len=answer.matches(txtBoxes[highlight]) ) )
      {
	aString spec = answer(len+1,answer.length()-1);
	len = 0;
	bool found = false;
	for ( int i=0; i<=UnstructuredMapping::Region; i++ )
	  {
	    if ( (len=spec.matches(UnstructuredMapping::EntityTypeStrings[i]) ) )
	      {
		highlightType = UnstructuredMapping::EntityTypeEnum(i);
		sScanF(spec(len,spec.length()-1),"%d",&highlightIDX);
		found = true;
		break;
	      }
	  }
	if ( !found )
	  {
	    gi.createMessageDialog("could not find entity "+spec,errorDialog);
	  }
	else if ( highlightIDX>map.size(highlightType) )
	  {
	    gi.createMessageDialog("no entity with the specified id",errorDialog);
	    highlightIDX=-1;
	  }
      }
    else if( answer=="erase" )
    {
      plotObject=FALSE;
      map.eraseUnstructuredMapping(gi);
    }
    else if( answer=="erase and exit" )
    {
      plotObject=FALSE;
      map.eraseUnstructuredMapping(gi);
      break;
    }
    else if( answer=="set colour" )
    {
      aString answer2 = gi.chooseAColour();
      if( answer2!="no change" )
      {
	map.setColour(answer2);
        par.mappingColour = answer2;
	map.eraseUnstructuredMapping(gi);
	plotObject=true;
      }
      
    }
    else if( answer=="plot shaded surfaces (3D) toggle" )
    {
      plotShadedMappingBoundaries = !plotShadedMappingBoundaries;   
    }
    else if( answer=="plot grid lines on boundaries (3D) toggle" )
    {
      plotLinesOnMappingBoundaries= !plotLinesOnMappingBoundaries;  
    }
    else if( answer=="plot grid points on curves (toggle)" )
    {
      plotGridPointsOnCurves= !plotGridPointsOnCurves;
    }
    else if( answer=="exit" )
    {
      break;
    }
    else if( answer=="plot" )
    {
      plotObject=TRUE;
    }
    else
    {
      char buff[100];
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
      
    }
    if( plotObject && gi.isGraphicsWindowOpen() )
    {
      gi.setAxesDimension( map.getRangeDimension() );

      if (dList == 0) // display list mgmt taken care of here
      {
	int list;
	par.set(GI_PLOT_THE_OBJECT_AND_EXIT,true); // this is for the mapping plotter that will be called below

	int plotShadedSurface, plotTitleLabels, localSquares, pTA; 
      
	par.get(GI_PLOT_SHADED_MAPPING_BOUNDARIES, plotShadedSurface);

	par.get(GI_PLOT_LABELS, plotTitleLabels);
	par.get(GI_LABEL_GRIDS_AND_BOUNDARIES, localSquares);
      
// don't plot labels or squares below
	par.set(GI_PLOT_LABELS, FALSE);
	par.set(GI_LABEL_GRIDS_AND_BOUNDARIES, FALSE);
      
// save the current settings
	int localPlotNodes = par.plotUnsNodes;
	int localPlotEdges = par.plotUnsEdges;
	int localPlotBoundaryEdges = par.plotUnsBoundaryEdges;
	int localPlotFaces = par.plotUnsFaces;
	int localPlotNormals = par.plotMappingNormals;
	int localNumberOfGhostLinesToPlot = par.numberOfGhostLinesToPlot;

	par.numberOfGhostLinesToPlot = plotGhosts ? 1 : 0;

//#define AP_DEBUG

// nodes
	if (par.plotUnsNodes)
	{
	  par.plotUnsNodes = true;
	  par.plotUnsEdges = false;
	  par.plotUnsBoundaryEdges = false;
	  par.plotUnsFaces = false;
	  par.plotMappingNormals = false;
	  
	  if ( (list=map.getDisplayList(UnstructuredMapping::nodeDL)) > 0 && glIsList(list) )
	  {
	    gi.setPlotDL(list, true);
#ifdef AP_DEBUG
	    printf("Turned on display list %i for nodes\n", list);
#endif	    
	  }
	  else 
	  {// get a new list which is unlit, plotted, hideable and interactive
	    list = gi.generateNewDisplayList(false, true, true, true);
	    map.setDisplayList(UnstructuredMapping::nodeDL, list);
	    
#ifdef AP_DEBUG
	    printf("Made new display list %i for nodes\n", list);
#endif	    
	    glNewList(list, GL_COMPILE);
	    glPushName(map.getGlobalID()); // assign a name for picking

	    plotUnstructured(gi, map, par, list, false);
	    glPopName();
	    glEndList();
	  }
	  par.plotUnsNodes = localPlotNodes;
	  par.plotUnsEdges = localPlotEdges;
	  par.plotUnsBoundaryEdges = localPlotBoundaryEdges;
	  par.plotUnsFaces = localPlotFaces;
	  par.plotMappingNormals = localPlotNormals;
	} // end nodes
	else
// toggle off the nodes display list
	{
	  gi.setPlotDL(map.getDisplayList(UnstructuredMapping::nodeDL), false);
	}
      
// edges
	if (par.plotUnsEdges)
	{
	  par.plotUnsNodes = false;
	  par.plotUnsEdges = true;
	  par.plotUnsBoundaryEdges = false;
	  par.plotUnsFaces = false;
	  par.plotMappingNormals = false;

	  if ( (list=map.getDisplayList(UnstructuredMapping::edgeDL)) > 0 && glIsList(list) )
	  {
	    gi.setPlotDL(list, true);
#ifdef AP_DEBUG
	    printf("Turned on display list %i for edges\n", list);
#endif	    
	  }
	  else 
	  {// get a new list which is unlit, plotted, hideable and interactive
	    list = gi.generateNewDisplayList(false, true, true, true);
	    map.setDisplayList(UnstructuredMapping::edgeDL, list);
#ifdef AP_DEBUG
	    printf("Made new display list %i for edges\n", list);
#endif	    
	    
	    glNewList(list, GL_COMPILE);
	    glPushName(map.getGlobalID()); // assign a name for picking

	    plotUnstructured(gi, map, par, list, false);
	    glPopName();
	    glEndList();
	  }
// reset graphics parameters
	  par.plotUnsNodes = localPlotNodes;
	  par.plotUnsEdges = localPlotEdges;
	  par.plotUnsBoundaryEdges = localPlotBoundaryEdges;
	  par.plotUnsFaces = localPlotFaces;
	  par.plotMappingNormals = localPlotNormals;
	} // end edges
	else
// toggle off the edges display list
	{
	  gi.setPlotDL(map.getDisplayList(UnstructuredMapping::edgeDL), false);
	}
	

// boundaryEdges
	if (par.plotUnsBoundaryEdges)
	{
	  par.plotUnsNodes = false;
	  par.plotUnsEdges = false;
	  par.plotUnsBoundaryEdges = true;
	  par.plotUnsFaces = false;
	  par.plotMappingNormals = false;
	  if ( (list=map.getDisplayList(UnstructuredMapping::boundaryEdgeDL)) > 0 && glIsList(list) )
	  {
	    gi.setPlotDL(list, true);
#ifdef AP_DEBUG
	    printf("Turned on display list %i for boundary edges\n", list);
#endif	    
	  }
	  else 
	  {// get a new list which is unlit, plotted, hideable and interactive
	    list = gi.generateNewDisplayList(false, true, true, true);
	    map.setDisplayList(UnstructuredMapping::boundaryEdgeDL, list);
#ifdef AP_DEBUG
	    printf("Made new display list %i for boundary edges\n", list);
#endif	    
	    
	    glNewList(list, GL_COMPILE);
	    glPushName(map.getGlobalID()); // assign a name for picking

	    plotUnstructured(gi, map, par, list, false);
	    glPopName();
	    glEndList();
	  }
// reset graphics parameters
	  par.plotUnsNodes = localPlotNodes;
	  par.plotUnsEdges = localPlotEdges;
	  par.plotUnsBoundaryEdges = localPlotBoundaryEdges;
	  par.plotUnsFaces = localPlotFaces;
	  par.plotMappingNormals = localPlotNormals;
	} // end boundaryEdges
	else
// toggle off the boundaryEdges display list
	{
	  gi.setPlotDL(map.getDisplayList(UnstructuredMapping::boundaryEdgeDL), false);
	}
	

// faces
	if (par.plotUnsFaces)
	{
	  par.plotUnsNodes = false;
	  par.plotUnsEdges = false;
	  par.plotUnsBoundaryEdges = false;
	  par.plotUnsFaces = true;
	  par.plotMappingNormals = false;
	  if ( (list=map.getDisplayList(UnstructuredMapping::faceDL)) > 0 && glIsList(list) )
	  {
	    gi.setPlotDL(list, true);
#ifdef AP_DEBUG
	    printf("Turned on display list %i for faces\n", list);
#endif	    
	  }
	  else 
	  {// get a new list which is lit, plotted, hideable but non-interactive
	    list = gi.generateNewDisplayList(true, true, true, false);
	    map.setDisplayList(UnstructuredMapping::faceDL, list);
#ifdef AP_DEBUG
	    printf("Made new display list %i for faces\n", list);
#endif	    
	    
	    glNewList(list, GL_COMPILE);
	    glPushName(map.getGlobalID()); // assign a name for picking

	    plotUnstructured(gi, map, par, list, true);
	    glPopName();
	    glEndList();
	  }
// reset graphics parameters
	  par.plotUnsNodes = localPlotNodes;
	  par.plotUnsEdges = localPlotEdges;
	  par.plotUnsBoundaryEdges = localPlotBoundaryEdges;
	  par.plotUnsFaces = localPlotFaces;
	  par.plotMappingNormals = localPlotNormals;
	} // end faces
	else
// toggle off the faces display list
	{
	  gi.setPlotDL(map.getDisplayList(UnstructuredMapping::faceDL), false);
	}
	
// normals
	if (par.plotMappingNormals)
	{
	  par.plotUnsNodes = false;
	  par.plotUnsEdges = false;
	  par.plotUnsBoundaryEdges = false;
	  par.plotUnsFaces = false;
	  par.plotMappingNormals = true;
	  if ( (list=map.getDisplayList(UnstructuredMapping::faceNormalDL)) > 0 && glIsList(list) )
	  {
	    gi.setPlotDL(list, true);
#ifdef AP_DEBUG
	    printf("Turned on display list %i for normals\n", list);
#endif	    
	  }
	  else 
	  {// get a new list which is unlit, plotted, hideable and interactive
	    list = gi.generateNewDisplayList(false, true, true, true);
	    map.setDisplayList(UnstructuredMapping::faceNormalDL, list);
#ifdef AP_DEBUG
	    printf("Made new display list %i for normals\n", list);
#endif	    
	    
	    glNewList(list, GL_COMPILE);
	    glPushName(map.getGlobalID()); // assign a name for picking

	    plotUnstructured(gi, map, par, list, false);
	    glPopName();
	    glEndList();
	  }
// reset graphics parameters
	  par.plotUnsNodes = localPlotNodes;
	  par.plotUnsEdges = localPlotEdges;
	  par.plotUnsBoundaryEdges = localPlotBoundaryEdges;
	  par.plotUnsFaces = localPlotFaces;
	  par.plotMappingNormals = localPlotNormals;
	} // end normals
	else
// toggle off the normals display list
	{
	  gi.setPlotDL(map.getDisplayList(UnstructuredMapping::faceNormalDL), false);
	}

// don't use the plot bounds in graphicsParameters after this ( neccessary?)
//	par.set(GI_USE_PLOT_BOUNDS, false);

	par.set(GI_PLOT_THE_OBJECT_AND_EXIT, plotObjectAndExit);
	par.numberOfGhostLinesToPlot = localNumberOfGhostLinesToPlot;
      
      }
      else // the calling routine is assumed to take care of display list management
      {
	glPushName(map.getGlobalID()); // assign a name for picking
	plotUnstructured(gi, map, par,dList,lit);
	glPopName();
      }
          
      // plot labels on top and bottom
      if( par.plotTitleLabels )
      {
	gi.plotLabels( par );
      }

      if ( highlightIDX>-1 )
	{
	  GraphicsParameters hgp;
	  hgp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
	  real lw;
	  hgp.get(GraphicsParameters::curveLineWidth,lw);
	  hgp.set(GraphicsParameters::curveLineWidth,2*lw);

	  if ( highlightType==UnstructuredMapping::Vertex )
	    {
	      realArray vert(1,map.getRangeDimension());
	      for ( int a=0; a<map.getRangeDimension(); a++ )
		vert(0,a) = x0(highlightIDX,a);
	      gi.plotPoints(vert,hgp);
	    }
	  else 
	    {
	      const intArray &edges = map.getEntities(UnstructuredMapping::Edge);
	      UnstructuredMappingAdjacencyIterator e,e_end;
	      e_end = map.adjacency_end(highlightType, highlightIDX, UnstructuredMapping::Edge);
	      e = map.adjacency_begin(highlightType, highlightIDX, UnstructuredMapping::Edge);
	      int ne = highlightType==UnstructuredMapping::Edge ? 1 : e.nAdjacent();
	      realArray lines(ne,map.getRangeDimension(),2);
	      ne = 0;
	      while ( e!=e_end )
		{
		  for ( int a=0; a<map.getRangeDimension(); a++ )
		    {
		      lines(ne,a,0) = x0(edges(*e,0),a);
		      lines(ne,a,1) = x0(edges(*e,1),a);
		    }
		  ne++;
		  e++;
		}
	      gi.plotLines(lines,hgp);

	      e_end = map.adjacency_end(highlightType, highlightIDX, UnstructuredMapping::Vertex);
	      e = map.adjacency_begin(highlightType, highlightIDX, UnstructuredMapping::Vertex);
	      cout<<"Entity : "<<UnstructuredMapping::EntityTypeStrings[highlightType]<<" # "<<highlightIDX<<" : ";
	      for ( ; e!=e_end; e++ )
		cout<<"  "<<*e;
	      cout<<endl;
	    }
	}

      // -------------------------Label Colours------------------------------------
      // Draw a coloured square with the number inside it for each of the colours
      // shown on the plot 
      if( labelGridsAndBoundaries )
      {
	IntegerArray numberList(1); numberList=0;
	gi.drawColouredSquares(numberList, par);
      }

      gi.redraw();
  
    } // end if plotObject
  }
  

  numberOfGhostLinesToPlot=oldNumberOfGhostLinesToPlot;  // reset

  if( !parameters.plotObjectAndExit )
  {
    gi.popGUI();
  }
  
  gi.unAppendTheDefaultPrompt(); // reset defaultPrompt

}

//#define OLD_PUM

#ifndef OLD_PUM

namespace {

  void computeFaceNormal( const int f, const intArray &faces, const realArray &x, real *norm )
  {

    int nNode=0;
    int maxn = faces.getLength(1);
    for ( int i=0; i<maxn && faces(f,i)>-1; i++ ) nNode++;

    real x1[3],x2[3];

    if (nNode==3)
      {
	int m1=faces(f,0), m2=faces(f,1), m3=faces(f,2);
	x1[0]=x(m2,0)-x(m1,0);
	x1[1]=x(m2,1)-x(m1,1);
	x1[2]=x(m2,2)-x(m1,2);
	x2[0]=x(m3,0)-x(m1,0);
	x2[1]=x(m3,1)-x(m1,1);
	x2[2]=x(m3,2)-x(m1,2);
	  
	norm[0]=x1[1]*x2[2]-x1[2]*x2[1];
	norm[1]=x1[2]*x2[0]-x1[0]*x2[2];
	norm[2]=x1[0]*x2[1]-x1[1]*x2[0];
	real normInverse = 1./max(REAL_MIN,SQRT( norm[0]*norm[0]+norm[1]*norm[1]+norm[2]*norm[2] ));
	norm[0]*=normInverse;
	norm[1]*=normInverse;
	norm[2]*=normInverse;
      }
    else 
      { // loop through each "side" and compute an average normal
	real zc[3]={0.,0.,0.}; //zone center
	for (int n=0; n<nNode; n++) 
	  {
	    zc[0] += x(faces(f,n),0);
	    zc[1] += x(faces(f,n),1);
	    zc[2] += x(faces(f,n),2);
	  }

	zc[0] = zc[0]/real(nNode);
	zc[1] = zc[1]/real(nNode);
	zc[2] = zc[2]/real(nNode);

	real normalContrib[3];
	norm[0] = norm[1] = norm[2] = 0.0;
	for (int p=0; p<nNode; p++) 
	  {
	    int p1 = faces(f,p);
	    int p2 = faces(f,(p+1)%nNode);
	    x1[0] = x(p1,0) - zc[0];
	    x1[1] = x(p1,1) - zc[1];
	    x1[2] = x(p1,2) - zc[2];
	    x2[0] = x(p2,0) - zc[0];
	    x2[1] = x(p2,1) - zc[1];
	    x2[2] = x(p2,2) - zc[2];
	    normalContrib[0]=x1[1]*x2[2]-x1[2]*x2[1];
	    normalContrib[1]=x1[2]*x2[0]-x1[0]*x2[2];
	    normalContrib[2]=x1[0]*x2[1]-x1[1]*x2[0];
	    real normInverse = 1./max(REAL_MIN,
				      SQRT( normalContrib[0]*normalContrib[0]+
					    normalContrib[1]*normalContrib[1]+
					    normalContrib[2]*normalContrib[2] ));
	    norm[0]+=normalContrib[0]*normInverse;
	    norm[1]+=normalContrib[1]*normInverse;
	    norm[2]+=normalContrib[2]*normInverse;
	  }		    
	  norm[0]/=real(nNode);
	  norm[1]/=real(nNode);
	  norm[2]/=real(nNode);    
      }
    
  }

}

int PlotIt::
plotUnstructured(GenericGraphicsInterface &gi, const UnstructuredMapping & um_, GraphicsParameters & par,
		 int dList /* =0 */ ,
		 bool lit /* =FALSE */ )
// ===========================================================================
// /Description:
//  Utility routine for plotting an unstructured grid.
//
//  050118 KKC This member function has been rewritten to to use the new connectivity
//           API and data structures.  It also handles ghost entities.
// ===========================================================================
{
  UnstructuredMapping &um = (UnstructuredMapping &)um_;

  bool drawLit = lit || dList==0;
  bool drawUnlit = !lit || dList==0;

  bool plotNodes = par.plotUnsNodes;
  bool plotFaces = par.plotUnsFaces;
  bool plotEdges = par.plotUnsEdges;
  int plotBlockBoundaries = 0;
  par.get(GI_PLOT_BLOCK_BOUNDARIES,plotBlockBoundaries);
  bool plotBoundaryEdges = par.plotUnsBoundaryEdges;
  bool useCutPlane = par.useUnsCutplane;
  bool flatShading = par.useUnsFlatShading;
  bool plotNormals = par.plotMappingNormals;
  bool plotGhost = par.numberOfGhostLinesToPlot;
  real zLev2D = par.zLevelFor2DGrids;

  bool bcColor = false;
  if( par.blockBoundaryColourOption==GraphicsParameters::colourByBoundaryCondition ||
      par.blockBoundaryColourOption==GraphicsParameters::defaultColour )
    bcColor=true;

//   cout<<"plotNodes : "<<plotNodes <<endl; 
//   cout<<"plotFaces : "<<plotFaces <<endl; 
//   cout<<"plotEdges : " <<plotEdges <<endl; 
//   cout<<"plotBoundaryEdges : "<<plotBoundaryEdges <<endl; 
//   cout<<"plotBlockBoundaries : "<<plotBlockBoundaries <<endl; 
//   cout<<"useCutPlane : "<<useCutPlane <<endl; 
//   cout<<"flatShading : "<<flatShading <<endl; 
//   cout<<"plotNormal : "<<plotNormals <<endl; 
//   cout<<"plotGhost : "<<plotGhost<<endl;


  RealArray & cutPlaneVertex = par.unsCutplaneVertex; 
  RealArray & cutPlaneNormal = par.unsCutplaneNormal;

  const bool plotWireFrame = false;

  const real & surfaceOffset = par.surfaceOffset;

  const int domainDimension=um.getDomainDimension();
  const int rangeDimension=um.getRangeDimension();

  // adjust the booleans to take into account conflicting states/options
  plotEdges = plotEdges /*|| plotBoundaryEdges*/ || (domainDimension==2 && rangeDimension==2 && plotFaces);
  plotFaces = rangeDimension==3 && plotFaces;
  plotNormals = plotNormals & (rangeDimension==3 && domainDimension==2);
  useCutPlane = useCutPlane && ( rangeDimension==3 && domainDimension==3 );

  const realArray &x = um.getNodes();
  int nNodes = um.size(UnstructuredMapping::Vertex);

  UnstructuredMappingIterator iter,iter_end;
  UnstructuredMappingAdjacencyIterator aiter,aiter_end;

  // // // // // // 
  // VERTEX PLOTTING
  // // // // // //  
  if( drawUnlit && plotNodes )
  {
    gi.setColour(GenericGraphicsInterface::textColour);
    glPointSize(5.*gi.getLineWidthScaleFactor());   
    glBegin(GL_POINTS);  

    if( rangeDimension==2 )
    {
      for( int i=0; i<nNodes; i++ )
	if ( plotGhost || !um.isGhost(UnstructuredMapping::Vertex,i) ) glVertex3(x(i,0),x(i,1),zLev2D);
    }
    else if ( rangeDimension==3 )
    {
      for( int i=0; i<nNodes; i++ )
	if ( plotGhost || !um.isGhost(UnstructuredMapping::Vertex,i) ) glVertex3(x(i,0),x(i,1),x(i,2));
    }
    
    glEnd();
  }

  // // // // // // 
  // EDGE PLOTTING
  // // // // // //  
  if ( drawUnlit && plotEdges )
    {
      const intArray &edges = um.getEntities(UnstructuredMapping::Edge);

      if ( rangeDimension==2 && par.gridLineColourOption==GraphicsParameters::defaultColour )
	gi.setColour(par.mappingColour);
      else if( par.gridLineColourOption==GraphicsParameters::defaultColour || 
	       par.gridLineColourOption==GraphicsParameters::colourByGrid  ) 
	gi.setColour(GenericGraphicsInterface::textColour);
      else if( par.gridLineColourOption==GraphicsParameters::colourByGrid )
	gi.setColour(um.getColour());
      else if( par.gridLineColourOption==GraphicsParameters::colourByValue )
	{
	  gi.setColour(gi.getColourName(min(max(0,par.gridLineColourValue),
					    GenericGraphicsInterface::numberOfColourNames-1)));
	}
      else 
	{
	  gi.setColour(GenericGraphicsInterface::textColour);
	}
      
      glLineWidth( par.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor());
      
      glBegin(GL_LINES);

      iter_end = um.end(UnstructuredMapping::Edge, !plotGhost );
      for ( iter=um.begin(UnstructuredMapping::Edge,!plotGhost); iter!=iter_end; iter++ )
	{
	  int e=*iter;
	  real z0 = zLev2D;
	  real z1 = zLev2D;

	  if ( plotGhost || !um.isGhost(UnstructuredMapping::Edge,e) )
	    {
	      const int e0 = edges(e,0);
	      const int e1 = edges(e,1);

	      bool plotable = true;
	      if ( rangeDimension==3 )
		{
		  z0 = x(e0,2);
		  z1 = x(e1,2);
		  if ( useCutPlane )
		    {

		      real dot1 = 0.;
		      real dot2 = 0.;
		      for ( int a=0; a<3; a++ )
			{
			  dot1 += (x(e0,a)-cutPlaneVertex(a))*cutPlaneNormal(a);
			  dot2 += (x(e1,a)-cutPlaneVertex(a))*cutPlaneNormal(a);
			}
		      plotable = dot1<0. && dot2<0.;
		    }
		}

	      if ( plotable )
		{
		  glVertex3(x(e0,0),x(e0,1),z0);
		  glVertex3(x(e1,0),x(e1,1),z1);
		}
	    }
	}

      glEnd();
    }

  // // // // // // // // //
  // BOUNDARY EDGE PLOTTING
  // // // // // // // // // 
  if ( drawUnlit && (plotBoundaryEdges || plotBlockBoundaries) ) // kkc : why was this only available for surface and volume grids?
    { // in the old version this only worked for volume and surface grids

      // the edges are by default coloured black (kkc 050118, they were hardcoded to always be blue; no more)
      if( par.blockBoundaryColourOption==GraphicsParameters::colourBlack ||
	  par.blockBoundaryColourOption==GraphicsParameters::defaultColour )
	gi.setColour("black");
      else if( par.blockBoundaryColourOption==GraphicsParameters::colourByGrid )
	gi.setColour(um.getColour());

      // Make the boundary edges 2 times wider than an interior edge (same as the structured plotter)
      glLineWidth(par.size(GraphicsParameters::lineWidth)*2.*gi.getLineWidthScaleFactor());

      const intArray &edges = um.getEntities(UnstructuredMapping::Edge);

      UnstructuredMapping::tag_entity_iterator tagit, tagit_end;

      std::string tag = "boundary "+UnstructuredMapping::EntityTypeStrings[UnstructuredMapping::Edge];

      tagit_end = um.tag_entity_end(tag);
      glBegin(GL_LINES);

      for ( tagit=um.tag_entity_begin(tag); tagit!=tagit_end; tagit++ )
	{
	  const int e=tagit->e;
	  const int m1=edges(e,0), m2=edges(e,1);
	  const real z0 = rangeDimension==3 ? x(m1,2) : zLev2D;
	  const real z1 = rangeDimension==3 ? x(m2,2) : zLev2D;
	  if ( bcColor )
	  {
	    if ( !plotFaces && um.hasBC(UnstructuredMapping::Vertex,m1) )
	      gi.setColour(gi.getColourName(min(max((long)0,um.getBC(UnstructuredMapping::Vertex,m1)),
						GenericGraphicsInterface::numberOfColourNames-1)));
	    else
	      gi.setColour("black");
	  }
	  
	  glVertex3(x(m1,0),x(m1,1),z0);
	  glVertex3(x(m2,0),x(m2,1),z1);
	}

      glEnd();
    }

  // // // // // //
  // FACE PLOTTING
  // // // // // //
  if ( drawLit && plotFaces )
    {
      const intArray &faces = um.getEntities(UnstructuredMapping::Face);
      
      // if there is a cut-plane active we will plot all the faces
      // if the cut-plane is inactive then only the boundary faces will be plotted

      glEnable(GL_POLYGON_OFFSET_FILL);
      glPolygonOffset(1.,surfaceOffset*OFFSET_FACTOR);  

      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      if (flatShading)
	glShadeModel(GL_FLAT);     // flat shading -> more revealing of unsmooth triangulation
      else
	glShadeModel(GL_SMOOTH);   // interpolate colours between vertices

      if( par.boundaryColourOption==GraphicsParameters::colourByGrid )
	gi.setColour(par.mappingColour);
      else //if ( par.boundaryColourOption==GraphicsParameters::defaultColour )
	gi.setColour(um.getColour());
    
      realArray vertexNormals;

      if ( !flatShading )
	{
	  vertexNormals.redim( um.size(UnstructuredMapping::Vertex), 3 );
	  vertexNormals = 0.;
	  
	  intArray numAdjFaces(um.size(UnstructuredMapping::Vertex));
	  numAdjFaces = 0;
	  
	  iter_end = um.end(UnstructuredMapping::Face);
	  for ( iter=um.begin(UnstructuredMapping::Face); iter!=iter_end; iter++ )
	    {
	      const int f = *iter;
	      real norm[3];
	      computeFaceNormal(f, faces, x, norm);
	      for ( int n=0; n<um.maxVerticesInEntity(UnstructuredMapping::Face) && faces(f,n)>-1; n++ )
		{
		  vertexNormals(faces(f,n),0) += norm[0];
	  	  vertexNormals(faces(f,n),1) += norm[1];
		  vertexNormals(faces(f,n),2) += norm[2];
		  numAdjFaces(faces(f,n))++;
		}
	    }

	  for ( int v=0; v<um.size(UnstructuredMapping::Vertex); v++ )
	    for ( int a=0; a<3; a++ )
	      vertexNormals(v,a) /= real(numAdjFaces(v)); // should we normalize this normal?

	}

      if ( !useCutPlane && domainDimension==3 )
	{
	  UnstructuredMapping::tag_entity_iterator tagit, tagit_end;
	  
	  std::string tag = "boundary "+UnstructuredMapping::EntityTypeStrings[UnstructuredMapping::Face];
	  
	  tagit_end = um.tag_entity_end(tag);
	  
	  for ( tagit=um.tag_entity_begin(tag); tagit!=tagit_end; tagit++ )
	    {
	      const int f=tagit->e;
	      
	      glBegin(GL_POLYGON);

	      if ( bcColor )
		if ( um.hasBC(UnstructuredMapping::Vertex,faces(f,0)) )
		  gi.setColour(gi.getColourName(min(max((long)0,um.getBC(UnstructuredMapping::Vertex,faces(f,0))),
						    GenericGraphicsInterface::numberOfColourNames-1)));

	      for ( int i=0; i<um.maxVerticesInEntity(UnstructuredMapping::Face) && faces(f,i)>-1; i++ )
		{
		  const int vi=faces(f,i);
		  const real z0 = rangeDimension==3 ? x(vi,2) : 0.;
		  if ( !flatShading )
		    {
		      real normal[3];
		      //		      computeFaceNormal(f,faces,x,normal);
		      for ( int a=0; a<3; a++ )
			normal[a] = vertexNormals(vi,a);
		      glNormal3v(normal);
		    }

		  glVertex3(x(vi,0),x(vi,1),z0);
		}

	      glEnd();
	    }
	}
      else
	{

	  iter_end = um.end(UnstructuredMapping::Face, !plotGhost );
	  for ( iter=um.begin(UnstructuredMapping::Face, !plotGhost ); iter!=iter_end; iter++ )
	    {
	      const int f = *iter;
	      bool plotable = true;
	      for ( int i=0; useCutPlane && i<um.maxVerticesInEntity(UnstructuredMapping::Face) && faces(f,i)>-1 && plotable; i++ )
		{
		  // a face is plotable if all it's vertices are behind the plane
		  real dotp=0.;
		  for ( int a=0; a<rangeDimension; a++ )
		    dotp += (x(faces(f,i),a)-cutPlaneVertex(a))*cutPlaneNormal(a);
		  plotable = dotp<0;
		}

	      if ( plotable )
		{
		  glBegin(GL_POLYGON);

		  for ( int i=0; i<um.maxVerticesInEntity(UnstructuredMapping::Face) && faces(f,i)>-1; i++ )
		    {
		      const int vi=faces(f,i);
		      const real z0 = rangeDimension==3 ? x(vi,2) : 0.;

		      if ( !flatShading )
			{
			  real normal[3];
			  //			  computeFaceNormal(f,faces,x,normal);
			  for ( int a=0; a<3; a++ )
			    normal[a] = vertexNormals(vi,a);
			  glNormal3v(normal);
			}

		      glVertex3(x(vi,0),x(vi,1),z0);
		    }

		  glEnd();
		}
	    }
	}
    }

  // // // // // // // // // //
  // SURFACE NORMAL PLOTTING
  // // // // // // // // // //
  if ( drawUnlit && plotNormals )
    {
      const intArray &faces = um.getEntities(UnstructuredMapping::Face);
      
      gi.setColour(GenericGraphicsInterface::textColour);
      
      glLineWidth(par.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor());

      glBegin(GL_LINES);

      iter_end = um.end(UnstructuredMapping::Face, !plotGhost);
      for ( iter=um.begin(UnstructuredMapping::Face, !plotGhost ); iter!=iter_end; iter++ )
	{

	  const int f = *iter;

	  real normal[3];
	  computeFaceNormal(f,faces,x,normal);
	  int nNode=0;
	  real zc[3] = {0.,0.,0.};

	  for ( int i=0; i<um.maxVerticesInEntity(UnstructuredMapping::Face) && faces(f,i)>-1; i++ )
	    {
	      const int vi=faces(f,i);

	      for ( int a=0; a<3; a++ )
		zc[a] += x(vi,a);
	      
	      nNode++;
	    }

	  for ( int a=0; a<3; a++ )
	    zc[a] /= real(nNode);

	  real nl = 0.2;
	  glVertex3(zc[0],zc[1],zc[2]);
	  glVertex3(zc[0]+nl*normal[0],zc[1]+nl*normal[1],zc[2]+nl*normal[2]);

	}

      glEnd();

    }

  return 0;
}

#else

int PlotIt::
plotUnstructured(GenericGraphicsInterface &gi, const UnstructuredMapping & map, GraphicsParameters & par,
		 int dList /* =0 */ ,
		 bool lit /* =FALSE */ )
// ===========================================================================
// /Description:
//  Utility routine for plotting an unstructured grid.
// ===========================================================================
{
  bool drawLit = lit || dList==0;
  bool drawUnlit = !lit || dList==0;

  bool plotNodes = par.plotUnsNodes;
  bool plotFaces = par.plotUnsFaces;
  bool plotEdges = par.plotUnsEdges;
  bool plotBoundaryEdges = par.plotUnsBoundaryEdges;
  bool useCutPlane = par.useUnsCutplane;
  bool flatShading = par.useUnsFlatShading;
  bool plotNormals = par.plotMappingNormals;

  RealArray & cutplaneVertex = par.unsCutplaneVertex; 
  //parameters.get(GI_UNS_CUTPLANE_VERTEX,cutplaneVertex);
  RealArray & cutplaneNormal = par.unsCutplaneNormal;
  //parameters.get(GI_UNS_CUTPLANE_VERTEX,cutplaneNormal);

  bool someEdgesPlotted=plotEdges || plotBoundaryEdges;

  const bool plotWireFrame = false;
  const real & surfaceOffset = par.surfaceOffset;

  const int domainDimension=map.getDomainDimension();
  const int rangeDimension=map.getRangeDimension();

  const realArray & x0 = map.getNodes();
      
  const real *xp = x0.Array_Descriptor.Array_View_Pointer1;
  const int xDim0=x0.getRawDataSize(0);
#define x(i0,i1) xp[(i0)+xDim0*(i1)]

  const int numberOfNodes = map.getNumberOfNodes();
  const int numberOfElements = map.getNumberOfElements();
  const int numberOfEdges    = map.getNumberOfEdges();
  const intArray & element0 = map.getElements();
  //  const intArray & edge0    = map.getEdges();
  bool hasNewC = ((UnstructuredMapping &)map).buildEntity(UnstructuredMapping::Edge,false);
  const intArray & edge0    = hasNewC ? ((UnstructuredMapping &)map).getEntities(UnstructuredMapping::Edge) : map.getEdges();
  const intArray & face0    = map.getFaces();
  const int maxNumberOfNodesPerElement = map.getMaxNumberOfNodesPerElement();

  const int *elementp = element0.Array_Descriptor.Array_View_Pointer1;
  const int elementDim0=element0.getRawDataSize(0);
#define element(i0,i1) elementp[(i0)+elementDim0*(i1)]
  const int *edgep = edge0.Array_Descriptor.Array_View_Pointer1;
  const int edgeDim0=edge0.getRawDataSize(0);
#define edge(i0,i1) edgep[(i0)+edgeDim0*(i1)]
  const int *facep = face0.Array_Descriptor.Array_View_Pointer1;
  const int faceDim0=face0.getRawDataSize(0);
#define face(i0,i1) facep[(i0)+faceDim0*(i1)]



  int i;
  if( drawUnlit && plotNodes )
  {
    gi.setColour(GenericGraphicsInterface::textColour);
    glPointSize(5.*gi.getLineWidthScaleFactor());   
    glBegin(GL_POINTS);  

    for( i=0; i<numberOfNodes; i++ )
    {
      if( rangeDimension==2 )
	glVertex3(x(i,0),x(i,1),zLev2D);
      else
	glVertex3(x(i,0),x(i,1),x(i,2));
    }
    glEnd();
  }
  if( drawUnlit && (plotFaces || plotEdges)  && domainDimension==2 && rangeDimension==2 )
  {

    // plot edges in 2D
    gi.setColour(par.mappingColour);

    glBegin(GL_LINES);
    int e;
//      for (e=0; e<numberOfEdges; e++)
//      {
//        glVertex3(x(edge(e,0),0),x(edge(e,0),1),0.);
//        glVertex3(x(edge(e,1),0),x(edge(e,1),1),0.);
//      }

    UnstructuredMappingIterator iter;
    for( iter=map.begin(UnstructuredMapping::Edge,true); iter!=map.end(UnstructuredMapping::Edge,true); iter++ ) // kkc true skips ghost entities
    {
      e=*iter;
      glVertex3(x(edge(e,0),0),x(edge(e,0),1),zLev2D);
      glVertex3(x(edge(e,1),0),x(edge(e,1),1),zLev2D);
    }
    glEnd();

//     int n,m;
//     int numberOfNodesPerElement=3;
//     for( i=0; i<numberOfElements; i++ )
//     {
//       glBegin(GL_LINE_STRIP);
//       for( n=0; n<numberOfNodesPerElement; n++ )
//       {
// 	m=element(i,n);
// 	// printf(" i=%i n=%i m=%i\n",i,n,m);
	    
// 	assert( m>=0 && m<numberOfNodes );
// 	glVertex3(x(m,0),x(m,1),0.);
//       }
//       m=element(i,0);
//       glVertex3(x(m,0),x(m,1),0.);
//       glEnd();     // GL_LINES_STRIP
//     }
  }
  if( drawUnlit && !plotWireFrame && plotEdges && domainDimension>=2 && rangeDimension==3 )
  {
    // *wdh* 010202 setColour(textColour);
    if( par.gridLineColourOption==GraphicsParameters::defaultColour || 
	par.gridLineColourOption==GraphicsParameters::colourByGrid  ) // hard to see edges if plotted with the same colour as the shaded faces
      gi.setColour(GenericGraphicsInterface::textColour);
    else if( par.gridLineColourOption==GraphicsParameters::colourByGrid )
      gi.setColour(map.getColour());
    else if( par.gridLineColourOption==GraphicsParameters::colourByValue )
    {
      gi.setColour(gi.getColourName(min(max(0,par.gridLineColourValue),
					GenericGraphicsInterface::numberOfColourNames-1)));
    }
    else 
    {
      gi.setColour(GenericGraphicsInterface::textColour);
    }

    glLineWidth( par.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor());
    
    glBegin(GL_LINES);
    int e;
    cutplaneVertex.reshape(1,3);
    cutplaneNormal.reshape(1,3);
    Range AXES(3);
    
    if ( useCutPlane )
      {
	for (e=0; e<numberOfEdges; e++)
	  {
	    bool plotable=true;
	
            real dot1=0., dot2=0.;
            for( int axis=0; axis<3; axis++ )
	    {
	      dot1+=(x(edge(e,0),axis)-cutplaneVertex(0,axis))*cutplaneNormal(0,axis);
	      dot2+=(x(edge(e,1),axis)-cutplaneVertex(0,axis))*cutplaneNormal(0,axis);
	    }
	    plotable = dot1<0. && dot2<0.;
	    
// 	    plotable = // *wdh* changed for P++
// 	      sum( (x(edge(e,0),AXES)-cutplaneVertex(0,AXES))*cutplaneNormal(0,AXES) )<0 &&
// 	      sum( (x(edge(e,1),AXES)-cutplaneVertex(0,AXES))*cutplaneNormal(0,AXES) )<0;
	    
	    if ( plotable )
	      {
		glVertex3(x(edge(e,0),0),x(edge(e,0),1),x(edge(e,0),2));
		glVertex3(x(edge(e,1),0),x(edge(e,1),1),x(edge(e,1),2));
	      }
	  }
      }
    else
    {
      if (numberOfEdges > 0)
      {
	for (e=0; e<numberOfEdges; e++)
	{
	  glVertex3(x(edge(e,0),0),x(edge(e,0),1),x(edge(e,0),2));
	  glVertex3(x(edge(e,1),0),x(edge(e,1),1),x(edge(e,1),2));
	}
      }
    }
    
    glEnd();

    if (numberOfEdges == 0) // plot the edges using element info if the connectivity hasn't been computed
    {
      int n, m;
      if (domainDimension==2)
      {
	for (e=0; e<numberOfElements; e++)
	{
	  glBegin(GL_LINE_LOOP);
	  for( n=0; n<maxNumberOfNodesPerElement; n++ )
	  {
	    if ((m=element(e,n))>=0)
	      glVertex3(x(m,0),x(m,1),x(m,2));
	    else
	      break;
	  }
	  glEnd();
	}
      }
      else
      {
	for (e=0; e<numberOfElements; e++)
	{
	  if (map.getElementType(e) == UnstructuredMapping::hexahedron)
	  {
	    glBegin(GL_LINE_LOOP);
	    for( n=0; n<4; n++ )
	    {
	      m=element(e,n);
	      glVertex3(x(m,0),x(m,1),x(m,2));
	    }
	    glEnd();
	    glBegin(GL_LINE_LOOP);
	    for( n=4; n<8; n++ )
	    {
	      m=element(e,n);
	      glVertex3(x(m,0),x(m,1),x(m,2));
	    }
	    glEnd();
	    glBegin(GL_LINES);
// 0-4
	    m=element(e,0);
	    glVertex3(x(m,0),x(m,1),x(m,2));
	    m=element(e,4);
	    glVertex3(x(m,0),x(m,1),x(m,2));
// 1-5
	    m=element(e,1);
	    glVertex3(x(m,0),x(m,1),x(m,2));
	    m=element(e,5);
	    glVertex3(x(m,0),x(m,1),x(m,2));
// 2-6
	    m=element(e,2);
	    glVertex3(x(m,0),x(m,1),x(m,2));
	    m=element(e,6);
	    glVertex3(x(m,0),x(m,1),x(m,2));
// 3-7
	    m=element(e,3);
	    glVertex3(x(m,0),x(m,1),x(m,2));
	    m=element(e,7);
	    glVertex3(x(m,0),x(m,1),x(m,2));

	    glEnd();
	  } // end hexahedron
	}
      } // end domainDimension == 3
    } // end no edge info available
    
// AP: tmp plot element numbers
//      real zc[3]; // zone center
//      aString buf;
//      int qq;

//      for( i=0; i<numberOfElements; i++ )
//      {
//        zc[0] = zc[1] = zc[2] = 0.0;
//        for (int n=0; n<3; n++) 
//  	for (qq=0; qq<3; qq++)
//  	  zc[qq] += x(element(i,n),qq);
//        for (qq=0; qq<3; qq++)
//  	zc[qq] = zc[qq]/3.;
//        gi.xLabel(sPrintF(buf,"%i", i), zc,0.01);
//      }
    
// end tmp	  

  }
  if( drawUnlit && !plotWireFrame && plotBoundaryEdges && domainDimension>=2 && rangeDimension==3 )
  {
// the edges are by default coloured blue
    if( par.blockBoundaryColourOption==GraphicsParameters::defaultColour || TRUE )
      gi.setColour("blue");
// according to the grid colour    
    else if( par.blockBoundaryColourOption==GraphicsParameters::colourByGrid )
      gi.setColour(map.getColour());

// Make the boundary edges 2 times wider than an interior edge (same as the structured plotter)
    glLineWidth(par.size(GraphicsParameters::lineWidth)*2.*gi.getLineWidthScaleFactor());

    int i;
// AP: There seems to be a bug in Mesa that prevents glPushName/ glPopName from working
// properly if there only is one long list of vertices for GL_LINES. Chopping this 
// list into several pieces seems to circumvent the problem
    glBegin(GL_LINES);
    for( i=0; i<map.getNumberOfBoundaryFaces(); i++ )
    {
      const int f = map.getBoundaryFace(i);
      const int numberOfNodes = map.getNumberOfNodesThisFace(f);
      int e;
      for (e=0; e<numberOfNodes-1; e++)
      {
        int m1=face(f,e), m2=face(f,e+1);
	glVertex3(x(m1,0),x(m1,1),x(m1,2));
	glVertex3(x(m2,0),x(m2,1),x(m2,2));
      }
    }
    glEnd();

  }

  if( drawUnlit && plotNormals && domainDimension==2 && rangeDimension==3 )
  {
    gi.setColour(GenericGraphicsInterface::textColour);

    glLineWidth(par.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor());

// Draw element normals
    real center[3], nl=0.2, normal[3], x1[3], x2[3];
    int n;
    for( i=0; i<numberOfElements; i++ )
    {
      glBegin(GL_LINES);
      
      if (map.getElementType(i) == UnstructuredMapping::triangle) 
      {
	int m1=element(i,0), m2=element(i,1), m3=element(i,2);
	x1[0]=x(m2,0)-x(m1,0);
	x1[1]=x(m2,1)-x(m1,1);
	x1[2]=x(m2,2)-x(m1,2);
	x2[0]=x(m3,0)-x(m1,0);
	x2[1]=x(m3,1)-x(m1,1);
	x2[2]=x(m3,2)-x(m1,2);
	  
	normal[0]=x1[1]*x2[2]-x1[2]*x2[1];
	normal[1]=x1[2]*x2[0]-x1[0]*x2[2];
	normal[2]=x1[0]*x2[1]-x1[1]*x2[0];
	real normInverse = 1./max(REAL_MIN,SQRT( normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2] ));
	normal[0]*=normInverse;
	normal[1]*=normInverse;
	normal[2]*=normInverse;

	for (n=0; n<3; n++)
	  center[n] = (x(m1,n) + x(m2,n) + x(m3,n))/3.;

	glNormal3v(normal);
	glVertex3(center[0],center[1],center[2]);
	glVertex3(center[0]+nl*normal[0],center[1]+nl*normal[1],center[2]+nl*normal[2]);
	
      }
      
      else 
      { // loop through each "side" and compute an average normal
	const int nnod = map.getNumberOfNodesThisElement(i);

	real zc[3]={0.,0.,0.}; //zone center
	for (int n=0; n<nnod; n++) 
	{
	  zc[0] += x(element(i,n),0);
	  zc[1] += x(element(i,n),1);
	  zc[2] += x(element(i,n),2);
	}
	zc[0] = zc[0]/real(nnod);
	zc[1] = zc[1]/real(nnod);
	zc[2] = zc[2]/real(nnod);

	real normalContrib[3];
	normal[0] = normal[1] = normal[2] = 0.0;
	for (int p=0; p<nnod; p++) 
	{
	  int p1 = element(i,p);
	  int p2 = element(i,(p+1)%nnod);
	  x1[0] = x(p1,0) - zc[0];
	  x1[1] = x(p1,1) - zc[1];
	  x1[2] = x(p1,2) - zc[2];
	  x2[0] = x(p2,0) - zc[0];
	  x2[1] = x(p2,1) - zc[1];
	  x2[2] = x(p2,2) - zc[2];
	  normalContrib[0]=x1[1]*x2[2]-x1[2]*x2[1];
	  normalContrib[1]=x1[2]*x2[0]-x1[0]*x2[2];
	  normalContrib[2]=x1[0]*x2[1]-x1[1]*x2[0];
	  real normInverse = 1./max(REAL_MIN,
				    SQRT( normalContrib[0]*normalContrib[0]+
					  normalContrib[1]*normalContrib[1]+
					  normalContrib[2]*normalContrib[2] ));
	  normal[0]+=normalContrib[0]*normInverse;
	  normal[1]+=normalContrib[1]*normInverse;
	  normal[2]+=normalContrib[2]*normInverse;
	}		    
	normal[0]/=real(nnod);
	normal[1]/=real(nnod);
	normal[2]/=real(nnod);    

	glNormal3v(normal);
	glVertex3(zc[0],zc[1],zc[2]);
	glVertex3(zc[0]+nl*normal[0],zc[1]+nl*normal[1],zc[2]+nl*normal[2]);
      }
// done drawing element normals
      glEnd();
    }

  }

  if( drawLit && plotFaces && domainDimension==2 && rangeDimension==3 )
  {
    if( !plotWireFrame )
    {
      glEnable(GL_POLYGON_OFFSET_FILL);
      glPolygonOffset(1.,surfaceOffset*OFFSET_FACTOR);  
//      printf("4:POLYGON_OFFSET_FACTOR=%f\n", surfaceOffset);

      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      if (flatShading)
	glShadeModel(GL_FLAT);     // flat shading -> more revealing of unsmooth triangulation
      else
	glShadeModel(GL_SMOOTH);   // interpolate colours between vertices
    }
    else
    {
      glDisable(GL_POLYGON_OFFSET_FILL);
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);  // plot lines on surface for a wireframe
    }
    
    // plot shaded triangles in 3D
    if( par.boundaryColourOption==GraphicsParameters::colourByGrid )
      gi.setColour(par.mappingColour);
    else if( par.boundaryColourOption==GraphicsParameters::defaultColour || TRUE )
      gi.setColour(map.getColour());

    int i,n,m;
    real x1[3],x2[3], normal[3];
      
    realArray vertexNormals(numberOfNodes,rangeDimension);
    intArray numAdjFaces(numberOfNodes);
    vertexNormals = 0.;
    numAdjFaces = 0;

// compute vertex normals by averaging
    if (!flatShading)
    {
      for( i=0; i<numberOfElements; i++ )
      {
	if (map.getElementType(i) == UnstructuredMapping::triangle) 
	{
	  int m1=element(i,0), m2=element(i,1), m3=element(i,2);
	  x1[0]=x(m2,0)-x(m1,0);
	  x1[1]=x(m2,1)-x(m1,1);
	  x1[2]=x(m2,2)-x(m1,2);
	  x2[0]=x(m3,0)-x(m1,0);
	  x2[1]=x(m3,1)-x(m1,1);
	  x2[2]=x(m3,2)-x(m1,2);
	  
	  normal[0]=x1[1]*x2[2]-x1[2]*x2[1];
	  normal[1]=x1[2]*x2[0]-x1[0]*x2[2];
	  normal[2]=x1[0]*x2[1]-x1[1]*x2[0];
	  real normInverse = 1./max(REAL_MIN,SQRT( normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2] ));
	  normal[0]*=normInverse;
	  normal[1]*=normInverse;
	  normal[2]*=normInverse;

	}
	else 
	{ // loop through each "side" and compute an average normal
	  const int nnod = map.getNumberOfNodesThisElement(i);
	  real zc[3]={0.,0.,0.}; //zone center
	  for (int n=0; n<nnod; n++) 
	  {
	    zc[0] += x(element(i,n),0);
	    zc[1] += x(element(i,n),1);
	    zc[2] += x(element(i,n),2);
	  }
	  zc[0] = zc[0]/real(nnod);
	  zc[1] = zc[1]/real(nnod);
	  zc[2] = zc[2]/real(nnod);

	  real normalContrib[3];
	  normal[0] = normal[1] = normal[2] = 0.0;
	  for (int p=0; p<nnod; p++) 
	  {
	    int p1 = element(i,p);
	    int p2 = element(i,(p+1)%nnod);
	    x1[0] = x(p1,0) - zc[0];
	    x1[1] = x(p1,1) - zc[1];
	    x1[2] = x(p1,2) - zc[2];
	    x2[0] = x(p2,0) - zc[0];
	    x2[1] = x(p2,1) - zc[1];
	    x2[2] = x(p2,2) - zc[2];
	    normalContrib[0]=x1[1]*x2[2]-x1[2]*x2[1];
	    normalContrib[1]=x1[2]*x2[0]-x1[0]*x2[2];
	    normalContrib[2]=x1[0]*x2[1]-x1[1]*x2[0];
	    real normInverse = 1./max(REAL_MIN,
				      SQRT( normalContrib[0]*normalContrib[0]+
					    normalContrib[1]*normalContrib[1]+
					    normalContrib[2]*normalContrib[2] ));
	    normal[0]+=normalContrib[0]*normInverse;
	    normal[1]+=normalContrib[1]*normInverse;
	    normal[2]+=normalContrib[2]*normInverse;
	  }		    
	  normal[0]/=real(nnod);
	  normal[1]/=real(nnod);
	  normal[2]/=real(nnod);    

	}
      
	for ( n=0; n<map.getNumberOfNodesThisElement(i); n++ )
	{
	  vertexNormals(element(i,n),0) += normal[0];
	  vertexNormals(element(i,n),1) += normal[1];
	  vertexNormals(element(i,n),2) += normal[2];
	  numAdjFaces(element(i,n))++;
	}
      }
    }
    
// draw the polygons
    for ( i=0; i<numberOfElements; i++ )
      {

	glBegin(GL_POLYGON);  // draw shaded filled polygons

	if (flatShading) // compute the element normal
	{
	  if (map.getElementType(i) == UnstructuredMapping::triangle) 
	  {
	    int m1=element(i,0), m2=element(i,1), m3=element(i,2);
	    x1[0]=x(m2,0)-x(m1,0);
	    x1[1]=x(m2,1)-x(m1,1);
	    x1[2]=x(m2,2)-x(m1,2);
	    x2[0]=x(m3,0)-x(m1,0);
	    x2[1]=x(m3,1)-x(m1,1);
	    x2[2]=x(m3,2)-x(m1,2);
	  
	    normal[0]=x1[1]*x2[2]-x1[2]*x2[1];
	    normal[1]=x1[2]*x2[0]-x1[0]*x2[2];
	    normal[2]=x1[0]*x2[1]-x1[1]*x2[0];
	    real normInverse = 1./max(REAL_MIN,SQRT( normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2] ));
	    normal[0]*=normInverse;
	    normal[1]*=normInverse;
	    normal[2]*=normInverse;
	  }
	  else
	  { // loop through each "side" and compute an average normal
	    const int nnod = map.getNumberOfNodesThisElement(i);
	    real zc[3]={0.,0.,0.}; //zone center
	    for (int n=0; n<nnod; n++) 
	    {
	      zc[0] += x(element(i,n),0);
	      zc[1] += x(element(i,n),1);
	      zc[2] += x(element(i,n),2);
	    }
	    zc[0] = zc[0]/real(nnod);
	    zc[1] = zc[1]/real(nnod);
	    zc[2] = zc[2]/real(nnod);

	    real normalContrib[3];
	    normal[0] = normal[1] = normal[2] = 0.0;
	    for (int p=0; p<nnod; p++) 
	    {
	      int p1 = element(i,p);
	      int p2 = element(i,(p+1)%nnod);
	      x1[0] = x(p1,0) - zc[0];
	      x1[1] = x(p1,1) - zc[1];
	      x1[2] = x(p1,2) - zc[2];
	      x2[0] = x(p2,0) - zc[0];
	      x2[1] = x(p2,1) - zc[1];
	      x2[2] = x(p2,2) - zc[2];
	      normalContrib[0]=x1[1]*x2[2]-x1[2]*x2[1];
	      normalContrib[1]=x1[2]*x2[0]-x1[0]*x2[2];
	      normalContrib[2]=x1[0]*x2[1]-x1[1]*x2[0];
	      real normInverse = 1./max(REAL_MIN,
					SQRT( normalContrib[0]*normalContrib[0]+
					      normalContrib[1]*normalContrib[1]+
					      normalContrib[2]*normalContrib[2] ));
	      normal[0]+=normalContrib[0]*normInverse;
	      normal[1]+=normalContrib[1]*normInverse;
	      normal[2]+=normalContrib[2]*normInverse;
	    }		    
	    normal[0]/=real(nnod);
	    normal[1]/=real(nnod);
	    normal[2]/=real(nnod);    
	  }
// assign the normal for the element
	  glNormal3v(normal);
	} // end if flatShading
	
	for( n=0; n<map.getNumberOfNodesThisElement(i); n++ )
	{
	  m=element(i,n);
	  // printf(" i=%i n=%i m=%i\n",i,n,m);
	    
	  if (!flatShading)
	  {
	    assert( m>=0 && m<numberOfNodes && numAdjFaces(m)>0);

  	    normal[0] = vertexNormals(m,0)/real(numAdjFaces(m));
  	    normal[1] = vertexNormals(m,1)/real(numAdjFaces(m));
  	    normal[2] = vertexNormals(m,2)/real(numAdjFaces(m));

	    glNormal3v(normal);
	  }
	    
	  glVertex3(x(m,0),x(m,1),x(m,2));
	  }
	// m=element(0,i);
	// glVertex3(x(m,0),x(m,1),0.);
	glEnd();    
      } // end for i=0,...,numberOfElements
    
    
    if( !plotWireFrame /* && someEdgesPlotted */ )
    {
      glDisable(GL_POLYGON_OFFSET_FILL);
    }
  } // end plotFaces, dDim=2, rDim=3  
  

  if( drawLit && plotFaces && domainDimension==3 && rangeDimension==3 )
  {
    if( !plotWireFrame )
    {
      glEnable(GL_POLYGON_OFFSET_FILL);
      glPolygonOffset(1.,surfaceOffset*OFFSET_FACTOR);  
//      printf("5:POLYGON_OFFSET_FACTOR=%f\n", surfaceOffset);

      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glShadeModel(GL_SMOOTH);     // interpolate colours between vertices
    }
    else
    {
      glDisable(GL_POLYGON_OFFSET_FILL);
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);  // plot lines on surface for a wireframe
    }
    

    // plot shaded triangles in 3D
    gi.setColour(par.mappingColour);

    int i,n,m;
    //int numberOfNodesPerElement=3;
    real x1[3],x2[3], normal[3];
    realArray vertexNormals(numberOfNodes,rangeDimension);
    intArray numAdjFaces(numberOfNodes);
    vertexNormals = 0.;
    numAdjFaces = 0;

    if ( !useCutPlane )
      {
	// in 3D, only draw faces on the boundary of the domain

	for( i=0; i<map.getNumberOfBoundaryFaces(); i++ )
	  {
	    int f = map.getBoundaryFace(i);
      
	    if (map.getNumberOfNodesThisFace(f)==3)
	    {
	      int m1=face(f,0), m2=face(f,1), m3=face(f,2);
	      x1[0]=x(m2,0)-x(m1,0);
	      x1[1]=x(m2,1)-x(m1,1);
	      x1[2]=x(m2,2)-x(m1,2);
	      x2[0]=x(m3,0)-x(m1,0);
	      x2[1]=x(m3,1)-x(m1,1);
	      x2[2]=x(m3,2)-x(m1,2);
	  
	      normal[0]=x1[1]*x2[2]-x1[2]*x2[1];
	      normal[1]=x1[2]*x2[0]-x1[0]*x2[2];
	      normal[2]=x1[0]*x2[1]-x1[1]*x2[0];
	      real normInverse = 1./max(REAL_MIN,SQRT( normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2] ));
	      normal[0]*=normInverse;
	      normal[1]*=normInverse;
	      normal[2]*=normInverse;
	    } 
	    else 
	    { // loop through each "side" and compute an average normal
	  
	      const int nnod = map.getNumberOfNodesThisFace(f);
	      real zc[3]={0.,0.,0.}; //zone center
	      for (int n=0; n<nnod; n++) 
	      {
		zc[0] += x(face(f,n),0);
		zc[1] += x(face(f,n),1);
		zc[2] += x(face(f,n),2);
	      }
	      zc[0] = zc[0]/real(nnod);
	      zc[1] = zc[1]/real(nnod);
	      zc[2] = zc[2]/real(nnod);

	      real normalContrib[3];
	      normal[0] = normal[1] = normal[2] = 0.0;
	      normalContrib[0] = normalContrib[1] = normalContrib[2] = 0.0;
	      for (int p=0; p<nnod; p++) {
		int p1 = face(f,p);
		int p2 = face(f,(p+1)%nnod);
		x1[0] = x(p1,0) - zc[0];
		x1[1] = x(p1,1) - zc[1];
		x1[2] = x(p1,2) - zc[2];
		x2[0] = x(p2,0) - zc[0];
		x2[1] = x(p2,1) - zc[1];
		x2[2] = x(p2,2) - zc[2];
		normalContrib[0]=x1[1]*x2[2]-x1[2]*x2[1];
		normalContrib[1]=x1[2]*x2[0]-x1[0]*x2[2];
		normalContrib[2]=x1[0]*x2[1]-x1[1]*x2[0];
		real normInverse = 1./max(REAL_MIN,SQRT( normalContrib[0]*normalContrib[0]+
							 normalContrib[1]*normalContrib[1]+
							 normalContrib[2]*normalContrib[2] ));
		normal[0]+=normalContrib[0]*normInverse;
		normal[1]+=normalContrib[1]*normInverse;
		normal[2]+=normalContrib[2]*normInverse;
	      }		    
	      normal[0]/=real(nnod);
	      normal[1]/=real(nnod);
	      normal[2]/=real(nnod);    
	    }
	    for ( n=0; n<map.getNumberOfNodesThisFace(f); n++ )
	    {
	      vertexNormals(face(f,n),0) += normal[0];
	      vertexNormals(face(f,n),1) += normal[1];
	      vertexNormals(face(f,n),2) += normal[2];
	      numAdjFaces(face(f,n))++;
	    }
	    
	  }
	for ( i=0; i<map.getNumberOfBoundaryFaces(); i++ )
	  {
	    int f = map.getBoundaryFace(i);
	    glBegin(GL_POLYGON);  // draw shaded filled polygons
	    for( n=0; n<map.getNumberOfNodesThisFace(f); n++ )
	      {
		m=face(f,n);
		// printf(" i=%i n=%i m=%i\n",i,n,m);
		assert( m>=0 && m<numberOfNodes && numAdjFaces(m)>0);
		normal[0] = vertexNormals(m,0)/real(numAdjFaces(m));
		normal[1] = vertexNormals(m,1)/real(numAdjFaces(m));
		normal[2] = vertexNormals(m,2)/real(numAdjFaces(m));
	    
		glNormal3v(normal);
		glVertex3(x(m,0),x(m,1),x(m,2));
	      }
	    //m=element(0,i);
	    //glVertex3(x(m,0),x(m,1),0.);
	    glEnd();    
	  }
      }
    else
      {
	cutplaneVertex.reshape(1,3);
	cutplaneNormal.reshape(1,3);
	Range AXES(3);

	for( int f=0; f<map.getNumberOfFaces(); f++ )
	  {
	    bool plotable=true;
	    int nnod = map.getNumberOfNodesThisFace(f);
            int n;
	    for ( n=0; plotable && n<nnod; n++)
	      {
		real dot=0.;
		for( int axis=0; axis<3; axis++ )
		  dot+=(x(face(f,n),axis)-cutplaneVertex(0,axis))*cutplaneNormal(0,axis);

		plotable = dot<0.;

		// plotable = sum( (x(face(f,n),AXES)-cutplaneVertex(0,AXES))*cutplaneNormal(0,AXES))<0;
	      }

	    if ( plotable )
	      {
		//cout<<"face "<<f<<" is plotable"<<endl;
		if (map.getNumberOfNodesThisFace(f)==3)
		{
		  int m1=face(f,0), m2=face(f,1), m3=face(f,2);
		  x1[0]=x(m2,0)-x(m1,0);
		  x1[1]=x(m2,1)-x(m1,1);
		  x1[2]=x(m2,2)-x(m1,2);
		  x2[0]=x(m3,0)-x(m1,0);
		  x2[1]=x(m3,1)-x(m1,1);
		  x2[2]=x(m3,2)-x(m1,2);
	  
		  normal[0]=x1[1]*x2[2]-x1[2]*x2[1];
		  normal[1]=x1[2]*x2[0]-x1[0]*x2[2];
		  normal[2]=x1[0]*x2[1]-x1[1]*x2[0];
		  real normInverse = 1./max(REAL_MIN,SQRT( normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2] ));
		  normal[0]*=normInverse;
		  normal[1]*=normInverse;
		  normal[2]*=normInverse;
		} 
		else 
		{ // loop through each "side" and compute an average normal
	  
		  const int nnod = map.getNumberOfNodesThisFace(f);
		  real zc[3]={0.,0.,0.}; //zone center
		  for (int n=0; n<nnod; n++) 
		  {
		    zc[0] += x(face(f,n),0);
		    zc[1] += x(face(f,n),1);
		    zc[2] += x(face(f,n),2);
		  }
		  zc[0] = zc[0]/real(nnod);
		  zc[1] = zc[1]/real(nnod);
		  zc[2] = zc[2]/real(nnod);

		  real normalContrib[3];
		  normal[0] = normal[1] = normal[2] = 0.0;
		  normalContrib[0] = normalContrib[1] = normalContrib[2] = 0.0;
		  for (int p=0; p<nnod; p++) {
		    int p1 = face(f,p);
		    int p2 = face(f,(p+1)%nnod);
		    x1[0] = x(p1,0) - zc[0];
		    x1[1] = x(p1,1) - zc[1];
		    x1[2] = x(p1,2) - zc[2];
		    x2[0] = x(p2,0) - zc[0];
		    x2[1] = x(p2,1) - zc[1];
		    x2[2] = x(p2,2) - zc[2];
		    normalContrib[0]=x1[1]*x2[2]-x1[2]*x2[1];
		    normalContrib[1]=x1[2]*x2[0]-x1[0]*x2[2];
		    normalContrib[2]=x1[0]*x2[1]-x1[1]*x2[0];
		    real normInverse = 1./max(REAL_MIN,SQRT( normalContrib[0]*normalContrib[0]+
							     normalContrib[1]*normalContrib[1]+
							     normalContrib[2]*normalContrib[2] ));
		    normal[0]+=normalContrib[0]*normInverse;
		    normal[1]+=normalContrib[1]*normInverse;
		    normal[2]+=normalContrib[2]*normInverse;
		  }		    
		  normal[0]/=real(nnod);
		  normal[1]/=real(nnod);
		  normal[2]/=real(nnod);    
		}
	      
		glBegin(GL_POLYGON);  // draw shaded filled polygons
		for( n=0; n<map.getNumberOfNodesThisFace(f); n++ )
		  {
		    m=face(f,n);
		    // printf(" i=%i n=%i m=%i\n",i,n,m);
		    //assert( m>=0 && m<numberOfNodes && numAdjFaces(m)>0);
	    
		    glNormal3v(normal);
		    glVertex3(x(m,0),x(m,1),x(m,2));
		  }
		//m=element(0,i);
		//glVertex3(x(m,0),x(m,1),0.);
		glEnd();    
	      }
	    
	  }
	cutplaneVertex.reshape(3);
	cutplaneNormal.reshape(3);

      }
  	
	cutplaneVertex.reshape(3);
	cutplaneNormal.reshape(3);

    if( !plotWireFrame ) //always push back the polygons...
    {
      glDisable(GL_POLYGON_OFFSET_FILL);
    }
  }
  
  return 0;
}

#undef x
#undef element
#undef face
#undef edge

#endif
