
#include "GenericGraphicsInterface.h"
#include "AdvancingFront.h"
#include "PlotIt.h"

int PlotIt::
plotAdvancingFront(GenericGraphicsInterface &gi, const AdvancingFront & front_,
		   GraphicsParameters & par, const int plotOptions)

// ===========================================================================
// /Description:
//  Utility routine for plotting an unstructured grid.
// ===========================================================================
{
  if( !gi.graphicsIsOn() ) return 0;

  AdvancingFront & front = (AdvancingFront &)front_;
  const AdvancingFrontParameters &advfpar = front.getParameters();
  
  bool plotFrontNodes = advfpar.state(AdvancingFrontParameters::frontNodes);
  bool plotFrontEdges = advfpar.state(AdvancingFrontParameters::frontEdges);
  bool plotFrontFaces = advfpar.state(AdvancingFrontParameters::frontFaces);
  bool plotMeshFaces = advfpar.state(AdvancingFrontParameters::meshFaces);
  bool plotMeshEdges = advfpar.state(AdvancingFrontParameters::meshEdges);

  int highlightedFace = advfpar.highlightFace();

  // bool plotNodes = plotOptions & 1;
  // bool plotFaces = plotOptions & 2;
  // bool plotEdges = plotOptions & 4;
  //  const bool & plotWireFrame = par.plotWireFrame;
  //  const real & surfaceOffset = par.surfaceOffset;

  const int domainDimension=front.getDomainDimension();
  const int rangeDimension=front.getRangeDimension();
  const realArray & x = front.getVertices();
      
  const int numberOfNodes =front.getNumberOfVertices();
  const vector<Face *> & faces = front.getFaces();
  const PriorityQueue & frnt    = front.getFront();
  const vector< vector<int> > & elements = front.getElements();

  // plot all the faces in the current state of the mesh
  
  gi.setColour(par.mappingColour);
  
  if ( domainDimension==2)
    {
      //      if (plotFaces)
      if ( rangeDimension==2 )
	{
//  <<<<<<< plotAdvancingFront.C
//  	  gi.setColour("black"); 
//  	  glBegin(GL_LINES);
//  	  for (vector<Face *>::const_iterator face=faces.begin(); face!=faces.end(); face++)
//  =======
	  if ( plotMeshFaces || plotMeshEdges )
//>>>>>>> 1.18
	    {
//  <<<<<<< plotAdvancingFront.C
//  	      if ( *face != NULL )
//  		{
//  		  if ( (*face)->getID()==highlightedFace ) gi.setColour("blue");
//  		  glVertex3(x((*face)->getVertex(0),0),x((*face)->getVertex(0),1),0.);
//  		  glVertex3(x((*face)->getVertex(1),0),x((*face)->getVertex(1),1),0.);
//  		  if ( (*face)->getID()==highlightedFace ) gi.setColour("black");
//  		}
//  =======
	      gi.setColour("black"); 
	      glBegin(GL_LINES);
	      for (vector<Face *>::const_iterator face=faces.begin(); face!=faces.end(); face++)
		{
		  if ( *face != NULL )
		    {
		      if ( (*face)->getID()==highlightedFace ) gi.setColour("blue");
		      glVertex3(x((*face)->getVertex(0),0),x((*face)->getVertex(0),1),0.);
		      glVertex3(x((*face)->getVertex(1),0),x((*face)->getVertex(1),1),0.);
		      if ( (*face)->getID()==highlightedFace ) gi.setColour("black");
		    }
		}
	      glEnd();
	    }
	  
	  // plot the faces on the front in a thicker line
	  
	  if ( plotFrontFaces || plotFrontEdges )
	    {
	      gi.setColour(par.mappingColour);
	      glLineWidth( par.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor()*2);
	      glBegin(GL_LINES);
	      for (PriorityQueue::const_iterator ff=frnt.begin(); ff!=frnt.end(); ff++)
		{
		  
		  int f = (*ff)->getID();
		  if ( f==highlightedFace ) gi.setColour("blue");
		  glVertex3(x(faces[f]->getVertex(0),0),x(faces[f]->getVertex(0),1), 0.);
		  glVertex3(x(faces[f]->getVertex(1),0),x(faces[f]->getVertex(1),1), 0.);
		  if ( f==highlightedFace ) gi.setColour(par.mappingColour);
		  
		}
	      glEnd();
	      glLineWidth( par.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor());
//>>>>>>> 1.18
	    }
	}
      else
	{
//  <<<<<<< plotAdvancingFront.C
//  	  gi.setColour(par.mappingColour);
//  	  glLineWidth( par.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor()*2);
//  	  glBegin(GL_LINES);
//  	  for (PriorityQueue::const_iterator ff=frnt.begin(); ff!=frnt.end(); ff++)
//  =======
	  
	  if ( plotMeshEdges || plotFrontEdges )
//>>>>>>> 1.18
	    {
//  <<<<<<< plotAdvancingFront.C
	      
//  	      int f = (*ff)->getID();
//  	      if ( f==highlightedFace ) gi.setColour("blue");
//  	      glVertex3(x(faces[f]->getVertex(0),0),x(faces[f]->getVertex(0),1), 0.);
//  	      glVertex3(x(faces[f]->getVertex(1),0),x(faces[f]->getVertex(1),1), 0.);
//  	      if ( f==highlightedFace ) gi.setColour(par.mappingColour);
//  =======
	      glEnable(GL_POLYGON_OFFSET_FILL);
	      glPolygonOffset(1.,par.surfaceOffset*OFFSET_FACTOR);  
	    }

	  if ( plotMeshEdges )
	    {
	      gi.setColour("black"); 
	      glBegin(GL_LINES);
	      for (vector<Face *>::const_iterator face=faces.begin(); face!=faces.end(); face++)
		{
		  if ( *face != NULL )
		    {
		      if ( (*face)->getID()==highlightedFace ) gi.setColour("blue");
		      glVertex3(x((*face)->getVertex(0),0),x((*face)->getVertex(0),1),x((*face)->getVertex(0),2));
		      glVertex3(x((*face)->getVertex(1),0),x((*face)->getVertex(1),1),x((*face)->getVertex(1),2));
		      if ( (*face)->getID()==highlightedFace ) gi.setColour("black");
		    }
		}
	      glEnd();
	  
	    }

	  // plot the faces on the front in a thicker line
	  if ( plotFrontEdges )
	    {
	      gi.setColour(par.mappingColour);
	      glLineWidth( par.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor()*2);
	      glBegin(GL_LINES);
	      for (PriorityQueue::const_iterator ff=frnt.begin(); ff!=frnt.end(); ff++)
		{
		  if ( *ff != NULL )
		    {
		      int f = (*ff)->getID();
		      if ( f==highlightedFace ) gi.setColour("blue");
		      glVertex3(x(faces[f]->getVertex(0),0),x(faces[f]->getVertex(0),1), x(faces[f]->getVertex(0),2));
		      glVertex3(x(faces[f]->getVertex(1),0),x(faces[f]->getVertex(1),1), x(faces[f]->getVertex(1),2));
		      if ( f==highlightedFace ) gi.setColour(par.mappingColour);
		    }
		  
		}
	      glEnd();
	      glLineWidth( par.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor());
	    }	

	  if ( plotMeshFaces )
	    {
	      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	      glShadeModel(GL_SMOOTH);
//>>>>>>> 1.18
	      
	      real normal[3];

	      gi.setColour("red");
	      const realArray & faceNormals = front.getFaceNormals();

	      for (vector< vector<int> >::const_iterator elem=elements.begin(); elem!=elements.end(); elem++ )
		{
		  const vector<int> &elFaces = *elem;

		  if ( elFaces.size()==3 )
		    {
		      int v1 = faces[elFaces[0]]->getVertex(0);
		      int v2 = faces[elFaces[0]]->getVertex(1);
		      int v3 = (faces[elFaces[1]]->getVertex(0)==v1 || faces[elFaces[1]]->getVertex(0)==v2) ?
			faces[elFaces[1]]->getVertex(1) : faces[elFaces[1]]->getVertex(0);
		      
		      normal[0] = (x(v2,1)-x(v1,1))*(x(v3,2)-x(v1,2)) - (x(v2,2)-x(v1,2))*(x(v3,1)-x(v1,1));
		      normal[1] =-(x(v2,0)-x(v1,0))*(x(v3,2)-x(v1,2)) - (x(v2,2)-x(v1,2))*(x(v3,0)-x(v1,0));
		      normal[2] = (x(v2,0)-x(v1,0))*(x(v3,1)-x(v1,1)) - (x(v2,1)-x(v1,1))*(x(v3,0)-x(v1,0));
		      
		      glBegin(GL_POLYGON);
		      // AP: Nowadays, lighting is enabled when the display list is created
		      //		  if( lighting[currentWindow] )
		      glNormal3v(normal);
		      glVertex3(x(v1,0),x(v1,1),x(v1,2));
		      //		  if( lighting[currentWindow] )
		      glNormal3v(normal);
		      glVertex3(x(v2,0),x(v2,1),x(v2,2));
		      //		  if( lighting[currentWindow] )
		      glNormal3v(normal);
		      glVertex3(x(v3,0),x(v3,1),x(v3,2));
		      glEnd();
		    }

		}
	    }
//<<<<<<< plotAdvancingFront.C
//  	  glEnd();
//  	  glLineWidth( par.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor());
//  =======
//  >>>>>>> 1.18
	}
    } else {
#if 0
      // plot the mesh in wireframe and the front as a shaded surface
      if( !lighting[currentWindow] && rangeDimension==3 ) // turn on lighting by default in 3D
	lightsOn(currentWindow);
	//turnOnLighting();
#endif

      int n;
      real normal[3];

      if ( plotFrontEdges || plotMeshEdges )
	{
	  glEnable(GL_POLYGON_OFFSET_FILL);
	  glPolygonOffset(1.,par.surfaceOffset*OFFSET_FACTOR);  
	}
      
      if (plotMeshEdges)
	{
	  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	  gi.setColour("black");
	  for (vector<Face *>::const_iterator face=faces.begin(); face!=faces.end(); face++)
	    {
	      if ( *face != NULL )
		{
		  if ( (*face)->getZ2ID()!=-1 )
		    {
		      glBegin(GL_POLYGON);
		      for ( n=0; n<(*face)->getNumberOfVertices(); n++ )
			{
			  if ( (*face)->getVertex(n)!=-1 )
			    glVertex3(x((*face)->getVertex(n),0), x((*face)->getVertex(n),1), x((*face)->getVertex(n),2));
			}
		      glEnd();
		    }
		}
	    }
	}

      if (plotFrontEdges)
	{
	  // now plot the faces on the front as shaded surfaces
	  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	  gi.setColour("black");
	  for (PriorityQueue::const_iterator ff=frnt.begin(); ff!=frnt.end(); ff++) 
	    {
	      int f = (*ff)->getID();
	      int nnod = faces[f]->getNumberOfVertices();
	      glBegin(GL_POLYGON);
	      for ( n=0; n<faces[f]->getNumberOfVertices(); n++ )
		{	
// AP		  if( lighting[currentWindow] )
		    glNormal3v(normal);
		  if ( faces[f]->getVertex(n)!=-1 )
		    glVertex3(x(faces[f]->getVertex(n),0), x(faces[f]->getVertex(n),1), x(faces[f]->getVertex(n),2));
		}
	      glEnd();
	    }
	}

      if ( plotFrontFaces || plotMeshFaces )
	{
	  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	  glShadeModel(GL_SMOOTH);
	}

      if (plotMeshFaces)
	{
	  gi.setColour("darkslategrey");
	  const realArray & faceNormals = front.getFaceNormals();
	  for (vector<Face *>::const_iterator face=faces.begin(); face!=faces.end(); face++)
	    {
	      if ( *face !=NULL )
		{
		  if ( (*face)->getID()==highlightedFace ) gi.setColour("blue");
		  if ( (*face)->getZ2ID()!=-1 )
		    {
		      normal[0] = faceNormals((*face)->getID(),0);
		      normal[1] = faceNormals((*face)->getID(),1);
		      normal[2] = faceNormals((*face)->getID(),2);
// *AP*
//  		      normal[0] = front.faceNormals((*face)->getID(),0);
//  		      normal[1] = front.faceNormals((*face)->getID(),1);
//  		      normal[2] = front.faceNormals((*face)->getID(),2);
		      glBegin(GL_POLYGON);
		      for ( n=0; n<(*face)->getNumberOfVertices(); n++ )
			{
			  if ( (*face)->getVertex(n)!=-1 )
			    glVertex3(x((*face)->getVertex(n),0), x((*face)->getVertex(n),1), x((*face)->getVertex(n),2));
// AP			  if( lighting[currentWindow] )
			    glNormal3v(normal);
			}
		      glEnd();
		    }
		  if ( (*face)->getID()==highlightedFace ) gi.setColour("darkslategrey");
		}
	    }
	}

      if (plotFrontFaces)
	{
	  // now plot the faces on the front as shaded surfaces
	  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	  glShadeModel(GL_SMOOTH);
	  gi.setColour(par.mappingColour);
// *AP*
	  const realArray & faceNormals = front.getFaceNormals();
	  for (PriorityQueue::const_iterator ff=frnt.begin(); ff!=frnt.end(); ff++) 
	    {
	      int f = (*ff)->getID();
	      //cout<<"plotting front face "<<f<<" with "<<faces[f]->getNumberOfVertices()<<" vertices "<<endl;
	      if ( f==highlightedFace ) gi.setColour("blue");
	      int nnod = faces[f]->getNumberOfVertices();
	      normal[0] = faceNormals(f,0);
	      normal[1] = faceNormals(f,1);
	      normal[2] = faceNormals(f,2);
//  	      normal[0] = front.faceNormals(f,0);
//  	      normal[1] = front.faceNormals(f,1);
//  	      normal[2] = front.faceNormals(f,2);

	      glBegin(GL_POLYGON);
	      for ( n=0; n<faces[f]->getNumberOfVertices(); n++ )
		{	
// AP		  if( lighting[currentWindow] )
		    glNormal3v(normal);
		  if ( faces[f]->getVertex(n)!=-1 )
		    glVertex3(x(faces[f]->getVertex(n),0), x(faces[f]->getVertex(n),1), x(faces[f]->getVertex(n),2));
		  //cout <<"vertex "<<x(faces[f]->getVertex(n),0)<<"  "<<x(faces[f]->getVertex(n),1)<<"  "<<x(faces[f]->getVertex(n),2)<<endl;
		}
	      glEnd();
	      if ( f==highlightedFace ) gi.setColour(par.mappingColour);

	    }
	    
	}
    }

  // plot the vertices on the front
  //int i;
  glColor3(0.,0.,0.);
  glPointSize(5.*gi.getLineWidthScaleFactor());   
  glBegin(GL_POINTS);  
  for ( int xi=0; xi<front.getNumberOfVertices(); xi++ )
    {
      if ( plotFrontNodes && ((AdvancingFront &)front).vertexIsOnFront(xi) )
	{
	  if( rangeDimension==2 ) 
	    glVertex3(x(xi,0),x(xi,1), 0.);
	  else
	    glVertex3(x(xi,0),x(xi,1), x(xi,2));
	}
    }
  
  glEnd();

  return 0;
}


//\begin{>>PlotItInclude.tex}{\subsection{Plot an AdvancingFront}} 
void PlotIt:: 
plot(GenericGraphicsInterface & gi, AdvancingFront & front,
     GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */)
//----------------------------------------------------------------------
// /Description:
//   Plot an AdvancingFront.
//
// /front (input): AdvancingFront to plot
// /parameters (input/output): supply optional parameters to change
//    plotting characteristics.
// /Return Values: none.
//
//  /Author: WDH \& AP \& KKC
//\end{PlotItInclude.tex} 
//----------------------------------------------------------------------
{
  //cout <<"inside plot"<<endl;

  if( !gi.isGraphicsWindowOpen() )
    return;

  AdvancingFrontParameters &advFrontParams = front.getParameters();

// save the current window number 
  int startWindow = gi.getCurrentWindow();

  GUIState interface;
  interface.setWindowTitle("AdvancingFront Plotter");
  interface.setExitCommand("exit","Exit");

  aString tbCommands[] = { "plot front nodes",
			   "plot front edges",
			   "plot front faces",
			   "plot all edges",
			   "plot all faces",
			   "" };

  aString tbLabels[] = { "Front Nodes",
			 "Front Edges",
			 "Front Faces",
			 "Mesh Edges",
			 "Mesh Faces",
			 "" };

  int tbState[] = {advFrontParams.state(AdvancingFrontParameters::frontNodes),
		   advFrontParams.state(AdvancingFrontParameters::frontEdges),
		   advFrontParams.state(AdvancingFrontParameters::frontFaces),
		   advFrontParams.state(AdvancingFrontParameters::meshEdges),
		   advFrontParams.state(AdvancingFrontParameters::meshFaces) };

  interface.setToggleButtons(tbCommands,tbLabels,tbState);

  aString menu[] = 
  {
    "plot control grid",
    "plot control function",
    "plot nodes (toggle)",
    "plot edges (toggle)",
    "plot number labels (toggle)",
    "plot",
    "set colour",
    "plotAxes (toggle)",
    "erase",
    "erase and exit",
    "exit",
    "" };

  interface.buildPopup(menu);

  int unlitlist=0, lightList=0;
  if( gi.isGraphicsWindowOpen() )
  {
    unlitlist = gi.generateNewDisplayList(0); // get a new (unlit) display list to use
    assert(unlitlist!=0);
    lightList = gi.generateNewDisplayList(1); // get a new (lit) display list to use
    assert(lightList!=0);
  }
  GraphicsParameters & par = parameters;

  bool   plotObject               = par.plotObject;

  gi.setAxesDimension(front.getRangeDimension());
  gi.setPlotTheAxes( par.plotTheAxes );

  bool & plotWireFrame            = par.plotWireFrame;

  // bool labelsPlottedLocally = false;
  
  bool labelGridsAndBoundaries = false;
  bool plotNodes=false;
  bool plotFaces=true;
  bool plotEdges=true;

  bool plotControlGrid = false;
  bool plotControlFunction = false;
  // do not plot ghost lines:  This can take too long and is usually unnecessary 
  // on Trimmed surfaces.

  const int domainDimension=front.getDomainDimension();
  const int rangeDimension=front.getRangeDimension();

  int axis;
  // first determine bounds on the mapping
  Bound b;
  RealArray xBound(2,3); xBound=0.;
  const realArray & xyz = front.getVertices();

  Range R(xyz.getBase(0), xyz.getBound(0));

  for( axis=0; axis<rangeDimension; axis++ )
    {
      xBound(0,axis)   = min(xyz(R, axis));
      xBound(1, axis)  = max(xyz(R, axis));
    }

  // set default prompt
  //  appendToTheDefaultPrompt("plot(AdvancingFront)>");

  if( !parameters.plotObjectAndExit )
    gi.pushGUI(interface);

  aString answer;

  // **** Plotting loop *****
  for(int it=0;;it++)
  {
    if( it==0 && plotObject )
      answer="plot";               // plot first time through if plotObject==true
    else if( it==1 && par.plotObjectAndExit )
      answer="exit";
    else
      gi.getAnswer(answer,"");

    //      getMenuItem(menu,answer);

// make sure that the currentWindow is the same as startWindow! (It might get changed 
// interactively by the user)
    if (gi.getCurrentWindow() != startWindow)
      gi.setCurrentWindow(startWindow);

    if( answer=="plotAxes (toggle)" )
    {
      gi.setPlotTheAxes(!gi.getPlotTheAxes());
    }
    else if( answer=="plot wire frame (toggle)" )
    {
      plotWireFrame= !plotWireFrame;
    }
    else if ( answer=="plot edges (toggle)")
    {
      plotEdges = !plotEdges;
    }
    else if ( answer=="plot faces (toggle)" )
    {
      plotFaces = !plotFaces;
    }
    else if( answer=="plot nodes (toggle)" )
    {
      plotNodes=!plotNodes;
    }
    else if( answer=="plot number labels (toggle)" )
    {
      labelGridsAndBoundaries=!labelGridsAndBoundaries;
      gi.setPlotTheColouredSquares(labelGridsAndBoundaries); // turn them on/off
    }
    else if ( answer=="plot control grid" )
      {
	plotControlGrid = !plotControlGrid;
      }
    else if ( answer=="plot control function" )
      {
	plotControlFunction = !plotControlFunction;
      }
    else if (answer(0,15)=="plot front nodes")
      {
	advFrontParams.toggle(AdvancingFrontParameters::frontNodes);
	interface.setToggleState(AdvancingFrontParameters::frontNodes, 
				 advFrontParams.state(AdvancingFrontParameters::frontNodes));
      }
    else if (answer(0,15)=="plot front edges")
      {
	advFrontParams.toggle(AdvancingFrontParameters::frontEdges);
	interface.setToggleState(AdvancingFrontParameters::frontEdges, 
				 advFrontParams.state(AdvancingFrontParameters::frontEdges));
      }
    else if (answer(0,15)=="plot front faces")
      {
	advFrontParams.toggle(AdvancingFrontParameters::frontFaces);
	interface.setToggleState(AdvancingFrontParameters::frontFaces, 
				 advFrontParams.state(AdvancingFrontParameters::frontFaces));
      }
    else if (answer(0,13)=="plot all edges")
      {
	advFrontParams.toggle(AdvancingFrontParameters::meshEdges);
	interface.setToggleState(AdvancingFrontParameters::meshEdges, 
				 advFrontParams.state(AdvancingFrontParameters::meshEdges));
      }
    else if (answer(0,13)=="plot all faces")
      {
	advFrontParams.toggle(AdvancingFrontParameters::meshFaces);
	interface.setToggleState(AdvancingFrontParameters::meshFaces, 
				 advFrontParams.state(AdvancingFrontParameters::meshFaces));
      }
    else if( answer=="erase" )
    {
      plotObject=false;
      glDeleteLists(unlitlist,1);
      glDeleteLists(lightList,1);
      gi.redraw(true);
    }
    else if( answer=="erase and exit" )
    {
      plotObject=false;
      glDeleteLists(unlitlist,1);
      glDeleteLists(lightList,1);
      gi.redraw();
      break;
    }
    else if( answer=="set colour" )
    {
      aString answer2 = gi.chooseAColour();
      if( answer2!="no change" )
        par.mappingColour = answer2;
    }
    else if( answer=="exit" )
    {
      break;
    }
    else if( answer=="plot" )
    {
      plotObject=true;
    }
    else
    {
      char buff[100];
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
      
    }
    if( plotObject )
    {

      gi.erase();
      gi.setGlobalBound(xBound);

      glNewList(lightList,GL_COMPILE);

      int plotOption=0;

      plotAdvancingFront(gi, front, par, plotOption );

      glEndList();
	  
      if ( plotControlFunction )
	contour(gi, front.getControlFunction(), parameters);

      // plot labels on top and bottom
// AP: This is now taken care of inside the GUI 
//        if( par.plotTitleLabels && (!labelsPlotted[currentWindow] || labelsPlottedLocally) )
//        {
//  	plotLabels( par );
//  	labelsPlottedLocally=true;
//        }

      gi.redraw();

    }
  }

  if( !parameters.plotObjectAndExit )
    gi.popGUI();
  //unAppendTheDefaultPrompt(); // reset defaultPrompt
}
