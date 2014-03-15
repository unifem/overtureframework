#include "MappingBuilder.h"
#include "GUIState.h"
#include "MappingInformation.h"
#include "HyperbolicMapping.h"
#include "UnstructuredMapping.h"
#include "NurbsMapping.h"
#include "GenericGraphicsInterface.h"
#include "CompositeSurface.h"
#include "IntersectionMapping.h"
#include "PlaneMapping.h"
#include "BoxMapping.h"
#include "HDF_DataBase.h"
#include "CompositeTopology.h"
#include "TFIMapping.h"
#include "GridStretcher.h"
#include "GridSmoother.h"
#include "ReductionMapping.h"
#include "display.h"
#include "arrayGetIndex.h"
#include "StretchTransform.h"

MappingBuilder::
MappingBuilder()
{
  pSurface=NULL;
  numberOfExtraBoundaryCurves=0;
  maxNumberOfExtraBoundaryCurves=0;
  extraBoundaryCurve=NULL;
  plotEdgeCurves=false;
  numberOfBoundaryCurves=0;
  boundaryCurves=NULL;
  targetGridSpacing[0]=targetGridSpacing[1]=-1.;  // negative means the user has not set a target

  // Normally we do not plot ghost lines on CompositeSurfaces -- but we allow this for grids:
  surfaceGrids.plotGhostLines=true;
  volumeGrids.plotGhostLines=true;
}

MappingBuilder::
~MappingBuilder()
{
  for( int c=0; c<numberOfExtraBoundaryCurves; c++ )
  {
    if( extraBoundaryCurve[c]!=NULL && extraBoundaryCurve[c]->decrementReferenceCount()==0 )
      delete extraBoundaryCurve[c];
  }
  delete [] extraBoundaryCurve;
}

  
static void
updateActiveGridMenu(MappingInformation & mapInfo,
                     DialogData & dialog, 
                     CompositeSurface & surfaceGrids,
                     int active )
// ===================================================================================================
// ==================================================================================================
{
  const int numberOfSurfaceGrids = surfaceGrids.numberOfSubSurfaces();
  
  const int maxCommands=numberOfSurfaceGrids+2;
  aString *cmd = new aString [maxCommands];
  aString *label= new aString [maxCommands];
  int i;
  for(i=0; i<numberOfSurfaceGrids; i++ )
  {
    Mapping & map = surfaceGrids[i];
    label[i]=map.getName(Mapping::mappingName);
    cmd[i]="active grid:"+label[i];
  }
  label[numberOfSurfaceGrids]="none";
  cmd[numberOfSurfaceGrids]="none";
  label[numberOfSurfaceGrids+1]="";
  cmd[numberOfSurfaceGrids+1]="";

  dialog.changeOptionMenu(2,cmd,label,active);
  dialog.getOptionMenu(2).setCurrentChoice(active);
}


int MappingBuilder::
plot(MappingInformation & mapInfo )
// =============================================================================================
// /Description:
//     Plot reference surface, surface grids and volume grids.
// =============================================================================================
{
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  aString answer,line;
  
  //kkc   assert( pSurface!=NULL );
  Mapping *surface = pSurface;
  
  if ( surface )
    gi.setAxesDimension(surface->getRangeDimension());
  else
    gi.setAxesDimension(3);
  
  gi.erase();   // This will only hide the things in a CompositeSurface

  if( surface!=NULL && plotReferenceSurface )
  {
    referenceSurfaceParameters.set(GI_MAPPING_COLOUR,"blue");
    real oldCurveLineWidth;
    referenceSurfaceParameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
    referenceSurfaceParameters.set(GraphicsParameters::curveLineWidth,2.);

    PlotIt::plot(gi,*surface,referenceSurfaceParameters);  

    referenceSurfaceParameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
  }
      
  if( plotSurfaceGrids )
  {
    real defaultOffset;
    parameters.get(GI_SURFACE_OFFSET,defaultOffset);  
    parameters.set(GI_SURFACE_OFFSET,(real) 1.);  // shift surface mappings by a smaller amount
    parameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,TRUE );
    parameters.set(GI_MAPPING_COLOUR,"red");
    parameters.set(GI_NUMBER_OF_GHOST_LINES_TO_PLOT,(plotGhostPoints ? numberOfGhostLinesToPlot : 0)); 

    PlotIt::plot(gi,surfaceGrids,parameters);

    parameters.set(GI_NUMBER_OF_GHOST_LINES_TO_PLOT,0);
    parameters.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,FALSE);
    parameters.set(GI_SURFACE_OFFSET,defaultOffset);
    

  }
  if( plotVolumeGrids )
  {
    parameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,TRUE );
    parameters.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,plotNonPhysicalBoundaries);
    parameters.set(GI_PLOT_MAPPING_EDGES,plotBlockBoundaries);
    parameters.set(GI_PLOT_UNS_EDGES,plotBlockBoundaries);     
    parameters.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,plotGridLines);
    if( plotGhostPoints )
    {
      printF("plot %i ghost points on volume grids...\n",numberOfGhostLinesToPlot);
      parameters.set(GI_NUMBER_OF_GHOST_LINES_TO_PLOT,numberOfGhostLinesToPlot);
//      parameters.set(GI_NUMBER_OF_GHOST_LINES_TO_PLOT,(plotGhostPoints ? 1 : 0)); 
    }
    
    PlotIt::plot(gi,volumeGrids,parameters);

    parameters.set(GI_NUMBER_OF_GHOST_LINES_TO_PLOT,0);
    parameters.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,false);
  }
  if( plotBoundaryCurves && (numberOfBoundaryCurves>0 || numberOfExtraBoundaryCurves>0) )
  {
    real oldCurveLineWidth;
    parameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
    parameters.set(GraphicsParameters::curveLineWidth,3.);
    parameters.set(GI_MAPPING_COLOUR,"green");
    parameters.set(GI_PLOT_GRID_POINTS_ON_CURVES,false );

    int b;
    for( b=0; b<numberOfBoundaryCurves; b++ )
    {
      aString colour = gi.getColourName(b);
      if( colour=="BLUE" ) // skip this colour
	colour = gi.getColourName(b+1);
      parameters.set(GI_MAPPING_COLOUR,colour);
	
      PlotIt::plot(gi,*boundaryCurves[b],parameters);
    }
    for( b=0; b<numberOfExtraBoundaryCurves; b++ )
    {
      aString colour = gi.getColourName(b+numberOfBoundaryCurves+3);
      parameters.set(GI_MAPPING_COLOUR,colour);
	
      PlotIt::plot(gi,*extraBoundaryCurve[b],parameters);
    }
    parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
    parameters.set(GI_MAPPING_COLOUR,"red");
    parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	
  }
  
  if( surface!=NULL && plotEdgeCurves )
  {
    // **** plot edge curves ****

    const bool isCompositeSurface = surface->getClassName()=="CompositeSurface";
    if( isCompositeSurface && ((CompositeSurface*)surface)->getCompositeTopology()!=NULL )
    {
      CompositeSurface & cs = (CompositeSurface&)(*surface);
      CompositeTopology & compositeTopology = *cs.getCompositeTopology();

      aString edgeCurveColour="green";
	
      GraphicsParameters params;
      params.set(GraphicsParameters::curveLineWidth,2.);
      params.set(GI_MAPPING_COLOUR,edgeCurveColour);
      params.set(GI_PLOT_GRID_POINTS_ON_CURVES,false);
      params.set(GI_POINT_SIZE,(real)3.);
      params.set(GI_PLOT_END_POINTS_ON_CURVES,true);
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

      int numberOfEdgeCurves=compositeTopology.getNumberOfEdgeCurves();
      for( int e=0; e<numberOfEdgeCurves; e++ )
      {
	// printf(" edge=%i status=%i\n",e,int(compositeTopology.getEdgeCurveStatus(e)));
	// if( int(compositeTopology.getEdgeCurveStatus(e)) <= 2 )
	Mapping & edge = compositeTopology.getEdgeCurve(e);
	    
	if( compositeTopology.getEdgeCurveStatus(e)==CompositeTopology::edgeCurveIsMerged ||
            compositeTopology.getEdgeCurveStatus(e)==CompositeTopology::edgeCurveIsNotMerged )
	  PlotIt::plot(gi,compositeTopology.getEdgeCurve(e),params);
      }

    }
  }
    

  return 0;
}


int MappingBuilder::
buildBoxGrid( MappingInformation & mapInfo )
// ============================================================================================
// /Description:
//   Build a box grid. Use the mouse to choose the box location.
// ============================================================================================
{
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  plotVolumeGrids=true;  // this should be on so we see the volume grids
  
  aString answer,line;
  
  Mapping & bbMap = pSurface!=NULL ? *pSurface : volumeGrids;

  // first determine a global bounding box.

  RealArray boundingBox(2,3);
  int axis;
  for( axis=0; axis<bbMap.getRangeDimension(); axis++ )
  {
    if( !bbMap.getRangeBound(Start,axis).isFinite() || !bbMap.getRangeBound(End,axis).isFinite() )
    {
      printf("*** WARNING: rangeBound not finite! axis=%i [%e,%e]\n",axis,
	     (real)bbMap.getRangeBound(Start,axis),(real)bbMap.getRangeBound(End,axis) );
      bbMap.getGrid();
    }
    boundingBox(Start,axis)=bbMap.getRangeBound(Start,axis);
    boundingBox(End  ,axis)=bbMap.getRangeBound(End,axis);
	
  }

  real xab[2][3], &xa=xab[0][0], &xb=xab[1][0], &ya=xab[0][1], &yb=xab[1][1], &za=xab[0][2], &zb=xab[1][2];
  xa=boundingBox(0,0), xb=boundingBox(1,0);
  ya=boundingBox(0,1), yb=boundingBox(1,1);
  za=boundingBox(0,2), zb=boundingBox(1,2);
      

  BoxMapping & box = *new BoxMapping(xa,xb,ya,yb,za,zb); 
  Mapping *mapPointer=&box;  mapPointer->incrementReferenceCount();
  mapInfo.mappingList.addElement(*mapPointer);

  // add the box to the list of volume grids
  numberOfBoxGrids++;
  box.setName(Mapping::mappingName,sPrintF(line,"box%i",numberOfBoxGrids));
      
  const int boxNumber=volumeGrids.numberOfSubSurfaces();
  volumeGrids.add(box); 
  volumeGrids.setColour(numberOfVolumeGrids,gi.getColourName(numberOfVolumeGrids));
  numberOfVolumeGrids++;

  GUIState gui;
  GUIState & dialog = gui;
  dialog.setWindowTitle("Build a box grid");
  dialog.setExitCommand("exit", "exit");

  aString pickOptionCommands[] = { "x min","x max","y min","y max","z min","z max",""};
  enum PickOptionsEnum
  {
    pickXmin,
    pickXmax,
    pickYmin,
    pickYmax,
    pickZmin,
    pickZmax
  } pickOption=pickXmin;
      
  dialog.addRadioBox("Pick to choose:",pickOptionCommands, pickOptionCommands, (int)pickOption, 3); // 3 columns


  aString pbLabels[] = {"stretching...",
                        "box details...",
			""};
  int numRows=1;
  dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

  int projectToNearest=true;
  aString tbCommands[] = {"project pick to nearest object",
			  ""};
  int tbState[2];
  tbState[0] = projectToNearest==true;
  tbState[1] = 0;
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


  const int numberOfTextStrings=20;
  aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "x bounds:"; 
  sPrintF(textStrings[nt], "%.6g, %.6g",xa,xb); nt++; 
  textLabels[nt] = "y bounds:"; 
  sPrintF(textStrings[nt], "%.6g, %.6g",ya,yb); nt++; 
  textLabels[nt] = "z bounds:"; 
  sPrintF(textStrings[nt], "%.6g, %.6g",za,zb); nt++; 
  textLabels[nt] = "lines:"; 
  sPrintF(textStrings[nt], "%i, %i, %i",box.getGridDimensions(0),box.getGridDimensions(1),
	  box.getGridDimensions(2));  nt++; 

  textLabels[nt] = "name"; 
  sPrintF(textStrings[nt], "%s", (const char*)box.getName(Mapping::mappingName)); 
  nt++; 

  textLabels[nt] = "bc"; 
  sPrintF(textStrings[nt], "%i %i %i %i %i %i (l r b t b f)",
	  volumeGrids[numberOfVolumeGrids-1].getBoundaryCondition(0,0),
	  volumeGrids[numberOfVolumeGrids-1].getBoundaryCondition(1,0),
	  volumeGrids[numberOfVolumeGrids-1].getBoundaryCondition(0,1),
	  volumeGrids[numberOfVolumeGrids-1].getBoundaryCondition(1,1),
	  volumeGrids[numberOfVolumeGrids-1].getBoundaryCondition(0,2),
	  volumeGrids[numberOfVolumeGrids-1].getBoundaryCondition(1,2));
  nt++;

  textLabels[nt] = "share"; 
  sPrintF(textStrings[nt], "%i %i %i %i %i %i (l r b t b f)",
	  volumeGrids[numberOfVolumeGrids-1].getShare(0,0),
	  volumeGrids[numberOfVolumeGrids-1].getShare(1,0),
	  volumeGrids[numberOfVolumeGrids-1].getShare(0,1),
	  volumeGrids[numberOfVolumeGrids-1].getShare(1,1),
	  volumeGrids[numberOfVolumeGrids-1].getShare(0,2),
	  volumeGrids[numberOfVolumeGrids-1].getShare(1,2));

nt++; 

  textLabels[nt] = "";  // null string terminates list
  
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  GridStretcher gridStretcher(box.getDomainDimension(),box.getRangeDimension());
  DialogData & stretchDialog = gui.getDialogSibling();
  stretchDialog.setWindowTitle("Stretching of Grid Lines");
  stretchDialog.setExitCommand("close stretching options", "close");
  gridStretcher.buildDialog(stretchDialog);

  StretchTransform & stretchedBox = *new StretchTransform;
  stretchedBox.setMapping(box);
  stretchedBox.incrementReferenceCount();
  bool useStretchedBox=false; // this indicates we have not stretched the box yet

  gi.pushGUI(dialog);
  gi.appendToTheDefaultPrompt("buildBox>");  

  SelectionInfo select; select.nSelect=0;

  int stretchReturnValue=-1;
  bool userHasSetGridLines=false;

  int len=0;
  for( int it=0; ; it++ )
  {
    bool boxHasChanged=false;
	
    if( it>0 )
    {
      gi.savePickCommands(false); // temporarily turn off saving of pick commands.     

      gi.getAnswer(answer,"", select);
         
      gi.savePickCommands(true); // turn back on
    }
    else
    {
      answer="plot";
      boxHasChanged=true;
    }
    if( answer=="exit" )
    {
      break;
    }
    else if( len=answer.matches("project pick to nearest object") )
    {
      sScanF(answer(len,answer.length()-1),"%i",&projectToNearest);
      dialog.setToggleState("project pick to nearest object",projectToNearest);
    }
    else if( len=answer.matches("x bounds:" ) )
    {
      sScanF(answer(len,answer.length()-1),"%e %e",&xa,&xb);
      dialog.setTextLabel("x bounds:",sPrintF(line, "%.6g, %.6g",xa,xb));

      box.setVertices(xa,xb,ya,yb,za,zb);
      boxHasChanged=true;  // fix this
    }
    else if( len=answer.matches("y bounds:" ) )
    {
      sScanF(answer(len,answer.length()-1),"%e %e",&ya,&yb);
      dialog.setTextLabel("y bounds:",sPrintF(line, "%.6g, %.6g",ya,yb));

      box.setVertices(xa,xb,ya,yb,za,zb);
      boxHasChanged=true;  // fix this
    }
    else if( len=answer.matches("z bounds:" ) )
    {
      sScanF(answer(len,answer.length()-1),"%e %e",&za,&zb);
      dialog.setTextLabel("z bounds:",sPrintF(line, "%.6g, %.6g",za,zb));

      box.setVertices(xa,xb,ya,yb,za,zb);
      boxHasChanged=true;  // fix this
    }
    else if( len=answer.matches("lines:" ) )
    {
      userHasSetGridLines=true;
      
      int nx,ny,nz;
      sScanF(answer(len,answer.length()-1),"%i %i %i",&nx,&ny,&nz);
      dialog.setTextLabel("lines:",sPrintF(line, "%i, %i, %i",nx,ny,nz));

      volumeGrids[numberOfVolumeGrids-1].setGridDimensions(0,nx);
      volumeGrids[numberOfVolumeGrids-1].setGridDimensions(1,ny);
      volumeGrids[numberOfVolumeGrids-1].setGridDimensions(2,nz);
      boxHasChanged=true;  // fix this
    }
    else if( len=answer.matches("bc") )
    {
      int bc[2][3];
      int side,axis;
      for( axis=0; axis<box.getDomainDimension(); axis++ )
	for( side=0; side<=1; side++ )
	  bc[side][axis]=volumeGrids[numberOfVolumeGrids-1].getBoundaryCondition(side,axis);
	
      sScanF(answer(len,answer.length()-1),"%i %i %i %i %i %i (l r b t b f)",&bc[0][0],&bc[1][0],&bc[0][1],&bc[1][1],&bc[0][2],&bc[1][2]);

      for( axis=0; axis<box.getDomainDimension(); axis++ )
	for( side=0; side<=1; side++ )
	  volumeGrids[numberOfVolumeGrids-1].setBoundaryCondition(side,axis,bc[side][axis]);

      dialog.setTextLabel("bc",sPrintF(line,"%i %i %i %i %i %i (l r b t b f)",
				       volumeGrids[numberOfVolumeGrids-1].getBoundaryCondition(0,0),
				       volumeGrids[numberOfVolumeGrids-1].getBoundaryCondition(1,0),
				       volumeGrids[numberOfVolumeGrids-1].getBoundaryCondition(0,1),
				       volumeGrids[numberOfVolumeGrids-1].getBoundaryCondition(1,1),
				       volumeGrids[numberOfVolumeGrids-1].getBoundaryCondition(0,2),
				       volumeGrids[numberOfVolumeGrids-1].getBoundaryCondition(1,2)));
      boxHasChanged=true;  // fix this
    }
    else if( len=answer.matches("share") )
    {
      int share[2][3];
      int side,axis;
      for( axis=0; axis<box.getDomainDimension(); axis++ )
	for( side=0; side<=1; side++ )
	  share[side][axis]=volumeGrids[numberOfVolumeGrids-1].getShare(side,axis);
	
      sScanF(answer(len,answer.length()-1),"%i %i %i %i %i %i",&share[0][0],&share[1][0],&share[0][1],&share[1][1],
                                     &share[0][2],&share[1][2]);

      for( axis=0; axis<box.getDomainDimension(); axis++ )
	for( side=0; side<=1; side++ )
	  volumeGrids[numberOfVolumeGrids-1].setShare(side,axis,share[side][axis]);

      dialog.setTextLabel("share",sPrintF(line,"%i %i %i %i %i %i",
				       volumeGrids[numberOfVolumeGrids-1].getShare(0,0),
				       volumeGrids[numberOfVolumeGrids-1].getShare(1,0),
				       volumeGrids[numberOfVolumeGrids-1].getShare(0,1),
				       volumeGrids[numberOfVolumeGrids-1].getShare(1,1),
				       volumeGrids[numberOfVolumeGrids-1].getShare(0,2),
				       volumeGrids[numberOfVolumeGrids-1].getShare(1,2)));
      boxHasChanged=true;  // fix this
    }
    else if( len=answer.matches("name") )
    {
      aString name = answer(len+1,answer.length()-1);
      volumeGrids[numberOfVolumeGrids-1].setName(Mapping::mappingName,name);
      dialog.setTextLabel("name",name);
    }
    
    else if( len=answer.matches("box details..." ) )
    {
      box.update(mapInfo);
      boxHasChanged=true;
    }
    else if( answer.matches("x min") ) pickOption=pickXmin;
    else if( answer.matches("x max") ) pickOption=pickXmax;
    else if( answer.matches("y min") ) pickOption=pickYmin;
    else if( answer.matches("y max") ) pickOption=pickYmax;
    else if( answer.matches("z min") ) pickOption=pickZmin;
    else if( answer.matches("z max") ) pickOption=pickZmax;
    else if( len=answer.matches("set x min") )
    {
      sScanF(answer(len,answer.length()-1),"%e",&xa);
      box.setVertices(xa,xb,ya,yb,za,zb);
      boxHasChanged=true;
    }
    else if( len=answer.matches("set x max") )
    {
      sScanF(answer(len,answer.length()-1),"%e",&xb);
      box.setVertices(xa,xb,ya,yb,za,zb);
      boxHasChanged=true;
    }
    else if( len=answer.matches("set y min") )
    {
      sScanF(answer(len,answer.length()-1),"%e",&ya);
      box.setVertices(xa,xb,ya,yb,za,zb);
      boxHasChanged=true;
    }
    else if( len=answer.matches("set y max") )
    {
      sScanF(answer(len,answer.length()-1),"%e",&yb);
      box.setVertices(xa,xb,ya,yb,za,zb);
      boxHasChanged=true;
    }
    else if( len=answer.matches("set z min") )
    {
      sScanF(answer(len,answer.length()-1),"%e",&za);
      box.setVertices(xa,xb,ya,yb,za,zb);
      boxHasChanged=true;
    }
    else if( len=answer.matches("set z max") )
    {
      sScanF(answer(len,answer.length()-1),"%e",&zb);
      box.setVertices(xa,xb,ya,yb,za,zb);
      boxHasChanged=true;
    }
//     else if( len=answer.matches("select box point") ) 
//     {
//       real x[3];
//       sScanF(answer(len,answer.length()-1),"%e %e %e",&x[0],&x[1],&x[2]);
          
//       if( pickOption==pickXmin ) xa=x[0];
//       else if( pickOption==pickXmax ) xb=x[0];
//       else if( pickOption==pickYmin ) ya=x[1];
//       else if( pickOption==pickYmax ) yb=x[1];
//       else if( pickOption==pickZmin ) za=x[2];
//       else if( pickOption==pickZmax ) zb=x[2];

//       box.setVertices(xa,xb,ya,yb,za,zb);

//       boxHasChanged=true;
//     }
    else if( answer=="stretching..." )
    {
      stretchDialog.showSibling();
      continue;
    }
    else if( answer=="close stretching options" )
    {
      stretchDialog.hideSibling();
      continue;
    }
    else if( stretchReturnValue=gridStretcher.update(answer,stretchDialog,mapInfo,stretchedBox) )
    {
      // NOTE: in the call to gridStretcher.update: 
      //               gridIndexRange: marks the actual boundaries
      //               gridIndexRange: marks the points to be projected on a surface grid
      printf("answer=%s was processed by gridStretcher.update, returnValue=%i\n",(const char*)answer,
             stretchReturnValue);
      if( stretchReturnValue==GridStretcher::gridWasChanged )
      {
        if( !useStretchedBox )
	{
	  // replace box with the stretched version
          for( int axis=0; axis<box.getDomainDimension(); axis++ )
	    stretchedBox.setGridDimensions(axis,box.getGridDimensions(axis));
	  
	  volumeGrids.remove(boxNumber);
	  volumeGrids.add(stretchedBox);
	  useStretchedBox=true;
	}
	boxHasChanged=true;
	
      }
      else
      {
	continue;
      }
    }
    else if( select.nSelect )
    {
      real x[3];
      x[0]=select.x[0];
      x[1]=select.x[1];
      x[2]=select.x[2];
      printf("The point (%9.3e,%9.3e,%9.3e) was picked \n",x[0],x[1],x[2]);
      
      if( projectToNearest )
      {
        realArray xa(1,3);
	xa(0,0)=select.x[0]; xa(0,1)=select.x[1]; xa(0,2)=select.x[2];

        // select.globalID = select.selection(0, 0); // global ID value of the closest object
        // select.zbMin    = select.selection(0, 1); // zBuffer value of the closest object

        // Identify the closest object as a surface or volume grid and project

        bool wasProjected=false;
        if( pSurface!=NULL && pSurface->getGlobalID()==select.globalID )
	{
	  MappingProjectionParameters mpParams;
          pSurface->project( xa,mpParams );

	  printf("Project onto the surface, projected pt=(%9.3e,%9.3e,%9.3e)\n",xa(0,0),xa(0,1),xa(0,2));

	  wasProjected=true;
	}
	else
	{
	  for( int grid=0; grid<numberOfVolumeGrids; grid++ )
	  {
	    Mapping & volume = volumeGrids[grid]; 
	    if( volume.getGlobalID()==select.globalID )
	    {
	      printf("Project onto the volume grid %i\n",grid);

              realArray r(1,3), xp(1,3);
              r=-.1;
	      volume.inverseMap(xa,r);

		
	      int sideMin=0, axisMin=0;
	      int side;
	      real rDiffMin=REAL_MAX,rDiff;
	      for( int axis=0; axis<volume.getDomainDimension(); axis++ )
	      {
		if( fabs(r(0,axis)) < fabs(r(0,axis)-1.)  )
		{
		  rDiff=fabs(r(0,axis)); side=0;
		}
		else
		{
		  rDiff=fabs(r(0,axis)-1.); side=1;
		}
		if( rDiff < rDiffMin )
		{
		  axisMin=axis;
		  sideMin=side;
		  rDiffMin=rDiff;
		}
	      }
              r(0,axisMin)=real(sideMin);
	      volume.map(r,xp);
	      
	      printf(" Volume grid %i was chosen as closest.. x=(%8.2e,%8.2e,%8.2e), r=(%8.2e,%8.2e,%8.2e)"
                     " xp=(%8.2e,%8.2e,%8.2e)\n",
		     grid,xa(0,0),xa(0,1),xa(0,2),r(0,0),r(0,1),r(0,2),xp(0,0),xp(0,1),xp(0,2));

              x[0]=xp(0,0);
	      x[1]=xp(0,1);
	      x[2]=xp(0,2);
	      
	      wasProjected=true;
	      break;
	    }
	  }
	}
	
      }  // end project to nearest

      if( pickOption==pickXmin ) 
      {
        xa=x[0];
        gi.outputToCommandFile(sPrintF(line,"set x min %e\n",x[0]));
      }
      else if( pickOption==pickXmax ) 
      {
	xb=x[0];
        gi.outputToCommandFile(sPrintF(line,"set x max %e\n",x[0]));
      }
      else if( pickOption==pickYmin ) 
      {
	ya=x[1];
        gi.outputToCommandFile(sPrintF(line,"set y min %e\n",x[1]));
      }
      else if( pickOption==pickYmax ) 
      {
	yb=x[1];
        gi.outputToCommandFile(sPrintF(line,"set y max %e\n",x[1]));
      }
      else if( pickOption==pickZmin ) 
      {
	za=x[2];
        gi.outputToCommandFile(sPrintF(line,"set z min %e\n",x[2]));
      }
      else if( pickOption==pickZmax ) 
      {
	zb=x[2];
        gi.outputToCommandFile(sPrintF(line,"set z max %e\n",x[2]));
      }

      box.setVertices(xa,xb,ya,yb,za,zb);

      boxHasChanged=true;  
    }
    else if( answer=="plot" )
    {
    }
    else
    {
      printf("ERROR: unknown response = [%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }

    dialog.getRadioBox(0).setCurrentChoice((int)pickOption );  // always set this

	
    if( boxHasChanged )
    {
      dialog.setTextLabel(0,sPrintF(line, "%.6g, %.6g",xa,xb));
      dialog.setTextLabel(1,sPrintF(line, "%.6g, %.6g",ya,yb));
      dialog.setTextLabel(2,sPrintF(line, "%.6g, %.6g",za,zb));

      if( !userHasSetGridLines && targetGridSpacing[0]>0. )
      {
        // recompute grid lines based if the user has not already set the grid lines
        int num[3]={1,1,1};  //
        for( int axis=0; axis<box.getDomainDimension(); axis++ )
	{
	  num[axis]=int( (xab[1][axis]-xab[0][axis])/targetGridSpacing[0]+1.5);
          volumeGrids[numberOfVolumeGrids-1].setGridDimensions(axis,num[axis]);
	}
	dialog.setTextLabel(3,sPrintF(line, "%i, %i, %i",num[0],num[1],num[2]));
      }

      volumeGrids.eraseCompositeSurface(gi,numberOfVolumeGrids-1);
    }
    
    plot(mapInfo);

  }

  box.decrementReferenceCount();
  gi.popGUI();

  gi.unAppendTheDefaultPrompt();
  return 0;
}



int MappingBuilder::
assignBoundaryConditions(MappingInformation & mapInfo )
// =================================================================================================
// /Description:
//    Assign bc's and share values by picking points with the mouse.
// =================================================================================================
{
  
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  aString answer,line;
  SelectionInfo select; select.nSelect=0;  
  // Mapping *surface = pSurface;

  int bc=1, share=1;

  GUIState dialog;
  dialog.setWindowTitle("Pick faces to assign BC and share");
  dialog.setExitCommand("exit", "exit");


  aString bcPlotOptionCommand[] = { "colour boundaries by grid number",
				    "colour boundaries by BC number",
				    "colour boundaries by share number",
				    "" };
  dialog.addRadioBox("volume grids boundary colour:",bcPlotOptionCommand, bcPlotOptionCommand, 
		     (int)bcPlotOption);

  enum PickingOptionsEnum
  {
    pickToAssignBoundaryConditions,
    pickToHideGrids,
    pickToQueryGrids
  };
  PickingOptionsEnum pickingOption=pickToAssignBoundaryConditions;
  
  aString opLabel1[] = {"assign BC","hide grids","query grids",""};  //
  // GUIState::addPrefix(opLabel1,"picking:",cmd,maxCommands);
  // dialog.addOptionMenu("Picking:", opLabel1,opLabel1,(int)pickingOption);
  int numberOfColumns=3;
  dialog.addRadioBox("Picking:", opLabel1,opLabel1,(int)pickingOption,numberOfColumns);



  aString pbLabels[] = {"undo last assignment",
                        "show all",
			""};
  int numRows=2;
  dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

  aString tbCommands[] = {"plot lines on non-physical boundaries",
			  ""};
  int tbState[6];
  tbState[0] = plotNonPhysicalBoundaries==true; 
  tbState[1] = 0; 
  
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


  const int numberOfTextStrings=5;
  aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "boundary condition:"; 
  sPrintF(textStrings[nt], "%i",bc); nt++; 
  textLabels[nt] = "shared boundary flag:"; 
  sPrintF(textStrings[nt], "%i",share); nt++; 

  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  // save the assignements that we make so we can undo them
  const int maxHistory=20;
  int currentHistory=maxHistory-1;  // -1 mod maxHistory
  IntegerArray commandHistory(5,maxHistory);
  commandHistory=-1;  // -1 means this is not a valid command.
      
  gi.pushGUI(dialog);
  gi.appendToTheDefaultPrompt("assignBC>");  

  int len=0;
  for( int it=0; ; it++ )
  {
    if( it>0 )
    {
      gi.savePickCommands(false); // temporarily turn off saving of pick commands.     

      gi.getAnswer(answer,"", select);

      printf("answer=[%s]\n",(const char*)answer);

      gi.savePickCommands(true); // turn back on
    }
    else
    {
      answer="plot";
    }
    if( answer=="exit" )
    {
      break;
    }
    else if( len=answer.matches("boundary condition:") )
    {
      // printf("answer=[%s]\n",(const char*)answer);
	  
      sScanF(answer(len,answer.length()-1),"%i",&bc);
      printf("answer=[%s],[%s], Current bc=%i\n",(const char*)answer,
	     (const char*)answer(len,answer.length()-1),bc);
      dialog.setTextLabel(0,sPrintF(line, "%i",bc));
      continue;
    }
    else if( len=answer.matches("shared boundary flag:" ) )
    {
      sScanF(answer(len,answer.length()-1),"%i",&share);
      printf("Current share=%i\n",share);
      dialog.setTextLabel(1,sPrintF(line, "%i",share));
      continue;
    }
    else if( answer=="colour boundaries by BC number" )
    {
      bcPlotOption=colourBoundariesByBCNumber;
      dialog.getRadioBox(0).setCurrentChoice((int)bcPlotOption );
      parameters.getBoundaryColourOption()=GraphicsParameters::colourByBoundaryCondition;
      volumeGrids.eraseCompositeSurface(gi);
    }
    else if( answer=="colour boundaries by share number" )
    {
      bcPlotOption=colourBoundariesByShareNumber;
      dialog.getRadioBox(0).setCurrentChoice((int)bcPlotOption );
      parameters.getBoundaryColourOption()=GraphicsParameters::colourByShare;
      volumeGrids.eraseCompositeSurface(gi);
    }
    else if( answer=="colour boundaries by grid number" )
    {
      bcPlotOption=colourBoundariesByGridNumber;
      dialog.getRadioBox(0).setCurrentChoice((int)bcPlotOption );
      parameters.getBoundaryColourOption()=GraphicsParameters::colourByGrid;
      volumeGrids.eraseCompositeSurface(gi);
    }
    else if( len=answer.matches("set BC and share") )
    {
      // this command will normally only appear in a command file.
      int grid=-1,side=-1,axis=-1,bc=-1,share=-1;
      sScanF(answer(len,answer.length()-1),"%i %i %i %i %i",&grid,&side,&axis,&bc,&share);
      if( side>=0 && side<=1 && axis>=0 && axis<=3 && grid>=0 && grid<numberOfVolumeGrids )
      {
	// Mapping & volume = mapInfo.mappingList[volumeGridNumber(grid)].getMapping();
	Mapping & volume = volumeGrids[grid]; 
	printf("Set bc=%i, share=%i on grid %i (side,axis)=(%i,%i)\n",bc,share,grid,side,axis);
	volume.setBoundaryCondition(side,axis,bc);
	volume.setShare(side,axis,share);
      }
      else
      {
	gi.outputString(sPrintF(line,"Invalid values in `set BC and share' command, side=%i, axis=%i, grid=%i",
				side,axis,grid));
	gi.stopReadingCommandFile();
      }

    }
    else if( answer=="undo last assignment" )
    {
      assert( currentHistory>=0 && currentHistory<maxHistory );
      // printf("undo: currentHistory=%i\n");
      // commandHistory.display("commandHistory");
	  
      if( commandHistory(0,currentHistory)>=0 )
      {
	int grid    =commandHistory(0,currentHistory);
	int side    =commandHistory(1,currentHistory);
	int axis    =commandHistory(2,currentHistory);
	int bcOld   =commandHistory(3,currentHistory);
	int shareOld=commandHistory(4,currentHistory);

	Range all;
	commandHistory(all,currentHistory)=-1;
	currentHistory=(currentHistory-1+maxHistory) % maxHistory;

	if( side>=0 && side<=1 && axis>=0 && axis<=3 && grid>=0 && grid<numberOfVolumeGrids )
	{
	  // Mapping & volume = mapInfo.mappingList[volumeGridNumber(grid)].getMapping();
	  Mapping & volume = volumeGrids[grid]; 
	  printf("Set bc=%i, share=%i on grid %i (side,axis)=(%i,%i)\n",bcOld,shareOld,grid,side,axis);
	  volume.setBoundaryCondition(side,axis,bcOld);
	  volume.setShare(side,axis,shareOld);

	  gi.outputToCommandFile(sPrintF(line,"set BC and share %i %i %i %i %i\n",
					 grid,side,axis,bcOld,shareOld));

	  volumeGrids.eraseCompositeSurface(gi,grid);
	}
	else
	{
	  gi.outputString(sPrintF(line,"Invalid command in history, side=%i, axis=%i, grid=%i, "
				  "shareOld=%i, bcOld=%i\n",side,axis,grid,shareOld,bcOld));
	  gi.stopReadingCommandFile();
	}
      }
      else
      {
	gi.outputString("There are no assignments to undo!");
      }
    }
    else if( answer.matches("show all") )
    {
      for( int grid=0; grid<numberOfVolumeGrids; grid++ )
      {
	volumeGrids.setIsVisible(grid,true);
              
      }
    }
    else if( answer.matches("assign BC") )
    {
      pickingOption=pickToAssignBoundaryConditions;
      dialog.getRadioBox(0).setCurrentChoice(pickingOption);
      continue;
    }
    else if( answer.matches("hide grids") )
    {
      pickingOption=pickToHideGrids;
      dialog.getRadioBox(0).setCurrentChoice(pickingOption);
      continue;
    }
    else if( answer.matches("query grids") )
    {
      pickingOption=pickToQueryGrids;
      dialog.getRadioBox(0).setCurrentChoice(pickingOption);
      continue;
    }
    else if( len=answer.matches("plot lines on non-physical boundaries") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value); plotNonPhysicalBoundaries=value;
      dialog.setToggleState("plot lines on non-physical boundaries",plotNonPhysicalBoundaries==true);       
      volumeGrids.eraseCompositeSurface(gi);
    }
    else if( select.nSelect )
    {
      if( pickingOption==pickToHideGrids )
      {
	for (int i=0; i<select.nSelect; i++)
	{
	  for( int grid=0; grid<numberOfVolumeGrids; grid++ )
	  {
	    // Mapping & volume = mapInfo.mappingList[volumeGridNumber(grid)].getMapping();
	    Mapping & volume = volumeGrids[grid]; 
	    if( volume.getGlobalID()==select.selection(i,0) )
	    {
              printf("Hide grid %i\n",grid);
	      volumeGrids.setIsVisible(grid,false);
              
	    }
	  }
	}
      }
      else if( pickingOption==pickToQueryGrids )
      {
        int gridChosen=-1;
        int minZbuffer=INT_MAX;
	for (int i=0; i<select.nSelect; i++)
	{
	  for( int grid=0; grid<numberOfVolumeGrids; grid++ )
	  {
	    // Mapping & volume = mapInfo.mappingList[volumeGridNumber(grid)].getMapping();
	    Mapping & volume = volumeGrids[grid]; 
	    if( volume.getGlobalID()==select.selection(i,0) )
	    {
              printf("Grid %i selected. x=(%8.2e,%8.2e,%8.2e) bc=[%i,%i][%i,%i][%i,%i] "
                     "share=[%i,%i][%i,%i][%i,%i]\n",
		     grid,select.x[0],select.x[1],select.x[2],
		     volume.getBoundaryCondition(0,0),volume.getBoundaryCondition(1,0),
		     volume.getBoundaryCondition(0,1),volume.getBoundaryCondition(1,1),
		     volume.getBoundaryCondition(0,2),volume.getBoundaryCondition(1,2),
		     volume.getShare(0,0),volume.getShare(1,0),
		     volume.getShare(0,1),volume.getShare(1,1),
		     volume.getShare(0,2),volume.getShare(1,2));
	      
              
              if( select.selection(i,1)<minZbuffer )
	      {
		gridChosen=grid;
		minZbuffer=select.selection(i,1);
	      }
	    }
	  }
	}
        if( gridChosen>=0 )
	{
	  printf("Closest grid is %i\n",gridChosen);
	}
      }
      else if( pickingOption==pickToAssignBoundaryConditions )
      {
	printf("Checking the selected items for the closest grid... \n");
      
        // *******  check for mulitple grids with the same closest value and assign them all *******

	realArray x(1,3), r(1,3);
        x(0,0)=select.x[0]; x(0,1)=select.x[1]; x(0,2)=select.x[2];

	for( int grid=0; grid<numberOfVolumeGrids; grid++ )
	{
	  // Mapping & volume = mapInfo.mappingList[volumeGridNumber(grid)].getMapping();
	  Mapping & volume = volumeGrids[grid]; 
  	  int gridChosen=-1;
	  for( int i=0; i<select.nSelect; i++)
	  {
	    if( volume.getGlobalID()==select.selection(i,0) && select.zbMin==select.selection(i,1) )
	    {
	     printf("grid=%i selected: i=%i ID=%i minZ=%i maxZ=%i volume.Id=%i x=%8.2e %8.2e %8.2e\n", 
                    grid,i,select.selection(i,0),
		    select.selection(i,1),select.selection(i,2), volume.getGlobalID(),
                     select.x[0], select.x[1], select.x[2]);
	      gridChosen=grid;
              break;
	    }
	  }
	  if( gridChosen>=0 )
	  {
	    // Mapping & volume = mapInfo.mappingList[volumeGridNumber(gridChosen)].getMapping();
	    Mapping & volume = volumeGrids[gridChosen]; 

	    r=-1;
	    volume.inverseMap(x,r);

	    printf(" Volume grid %i was chosen as closest.. x=(%8.2e,%8.2e,%8.2e), r=(%8.2e,%8.2e,%8.2e)\n",
		   gridChosen,x(0,0),x(0,1),x(0,2),r(0,0),r(0,1),r(0,2));
		
	    int sideMin=0, axisMin=0;
	    int side;
	    real rDiffMin=REAL_MAX,rDiff;
	    for( int axis=0; axis<volume.getDomainDimension(); axis++ )
	    {
	      if( fabs(r(0,axis)) < fabs(r(0,axis)-1.)  )
	      {
		rDiff=fabs(r(0,axis)); side=0;
	      }
	      else
	      {
		rDiff=fabs(r(0,axis)-1.); side=1;
	      }
	      if( rDiff < rDiffMin )
	      {
		axisMin=axis;
		sideMin=side;
		rDiffMin=rDiff;
	      }
	    }
	    // save the previous values of bc and share
	    currentHistory=(currentHistory+1) % maxHistory;
	    commandHistory(0,currentHistory)=gridChosen;
	    commandHistory(1,currentHistory)=sideMin;
	    commandHistory(2,currentHistory)=axisMin;
	    commandHistory(3,currentHistory)=volume.getBoundaryCondition(sideMin,axisMin);
	    commandHistory(4,currentHistory)=volume.getShare(sideMin,axisMin);

	    printf("Set bc=%i, share=%i on grid %i (side,axis)=(%i,%i)\n",bc,share,gridChosen,sideMin,axisMin);
	    volume.setBoundaryCondition(sideMin,axisMin,bc);
	    volume.setShare(sideMin,axisMin,share);
		
	    gi.outputToCommandFile(sPrintF(line,"set BC and share %i %i %i %i %i\n",
					   gridChosen,sideMin,axisMin,bc,share));
	    
	    volumeGrids.eraseCompositeSurface(gi,gridChosen);
	  } // end if grid chosen
	} // end for grid

 
      } // end pick to assign BC
      
    }
    else if( answer=="plot" )
    {
    }

    gi.erase();
    parameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,TRUE );
    parameters.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,plotNonPhysicalBoundaries);
      
    PlotIt::plot(gi,volumeGrids,parameters);
    parameters.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,false);
	
    // make a list of all numbers that are used to plot grids for the coloured squares
    IntegerArray numberList(numberOfVolumeGrids*6); // at most 6 faces
    numberList=-1;
    for( int grid=0; grid<numberOfVolumeGrids; grid++ )
    {
      int num=0;
      if( parameters.getBoundaryColourOption()==GraphicsParameters::colourByGrid )
      {
	numberList(num)=grid; num++;
      }
      else
      {
	Mapping & volume = volumeGrids[grid]; 
	
	for( int side=0; side<=1; side++ )
	{
	  for( int axis=0; axis<3; axis++ )
	  {
	    if( parameters.getBoundaryColourOption()==GraphicsParameters::colourByBoundaryCondition )
	    {
	      numberList(num)=volume.getBoundaryCondition(side,axis);  num++;
	    }
	    else if( parameters.getBoundaryColourOption()==GraphicsParameters::colourByShare )
	    {
	      numberList(num)=volume.getShare(side,axis); num++;
	    }
	  
	  }
	}
      }
      gi.drawColouredSquares(numberList,parameters);
    }
    
  }
  // make all grids visible again
  for( int grid=0; grid<numberOfVolumeGrids; grid++ )
  {
    volumeGrids.setIsVisible(grid,true);
  }

  gi.popGUI();
  gi.unAppendTheDefaultPrompt();

  return 0;
}


int MappingBuilder::
buildCurveOnSurface(MappingInformation & mapInfo)
// =================================================================================================
// /Description:
//    Build a curve on the surface by
//       1. intersecting a plane with the surface.
// =================================================================================================
{
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  // first determine a global bounding box.
  assert( pSurface!=NULL );
  Mapping & surf = *pSurface;

  RealArray boundingBox(2,3);
  real scale=0.;
  int axis;
  for( axis=0; axis<surf.getRangeDimension(); axis++ )
  {
    if( !surf.getRangeBound(Start,axis).isFinite() || !surf.getRangeBound(End,axis).isFinite() )
    {
      printf("*** WARNING: rangeBound not finite! axis=%i [%e,%e]\n",axis,
	     (real)surf.getRangeBound(Start,axis),(real)surf.getRangeBound(End,axis) );
      surf.getGrid();
    }
    boundingBox(Start,axis)=surf.getRangeBound(Start,axis);
    boundingBox(End  ,axis)=surf.getRangeBound(End,axis);
    
    scale=max(scale,boundingBox(End  ,axis)-boundingBox(Start,axis));
	
    // scale *=1.5;
	
    // printf(" ***Surface:  xa=%e xb=%e scale=%e\n",boundingBox(Start,axis),boundingBox(End  ,axis),scale);
  }
  printf("The bounding box of the surface is [%8.2e,%8.2e]x[%8.2e,%8.2e]x[%8.2e,%8.2e]\n",
	 boundingBox(0,0),boundingBox(1,0),boundingBox(0,1),boundingBox(1,1),boundingBox(0,2),boundingBox(1,2));

  
// todo: specify plane in this way:
//    real normal[3]={0.,0.,1.} // normal to plane
//    real centre[3];           // centre of plane
//    real width= (boundingBox(1,0)-boundingBox(0,0))*1.1;
//    real height=(boundingBox(1,0)-boundingBox(0,0))*1.1;
//    for( axis=0; axis<3; axis++ )
//    {
//      centre[axis]=.5*(boundingBox(0,axis)+boundingBox(1,axis));
//    }
  
  // A plane is defined by three points:
  real xc=.5*(boundingBox(0,0)+boundingBox(1,0));  // mid-point value along x
  real xp[3][3];
  // point 1:
  xp[0][0]=xc;
  xp[1][0]=boundingBox(Start,1)-scale*.1;
  xp[2][0]=boundingBox(Start,2)-scale*.1;
  // point 2:
  xp[0][1]=xc;
  xp[1][1]=boundingBox(End  ,1)+scale*.1;
  xp[2][1]=boundingBox(Start,2)-scale*.1;
  // point 3:
  xp[0][2]=xc;
  xp[1][2]=boundingBox(Start,1)-scale*.1;
  xp[2][2]=boundingBox(End  ,2)+scale*.1;

  int plotPlane=true;

  GUIState gui;
  gui.setWindowTitle("Build a Curve on the Surface");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = gui;


  aString pbLabels[] = {"cut with plane",
                        "erase last curve",
                        "edit intersection curve",
                        "add last curve to mapping list",
                        "create trimmed surface",
			""};
  int numRows=4;
  dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

  aString tbCommands[] = {"plot plane",
			  ""};
  int tbState[6];
  tbState[0] = plotPlane==true; 
  tbState[1] = 0; 
  
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

  const int numberOfTextStrings=6;
  aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

  int nt=0;

  textLabels[nt] = "plane point 1";
  sPrintF(textStrings[nt], "%g %g %g",xp[0][0],xp[1][0],xp[2][0]);
  nt++;
  textLabels[nt] = "plane point 2";
  sPrintF(textStrings[nt], "%g %g %g",xp[0][1],xp[1][1],xp[2][1]);
  nt++;
  textLabels[nt] = "plane point 3";
  sPrintF(textStrings[nt], "%g %g %g",xp[0][2],xp[1][2],xp[2][2]);
  nt++;

  int planeGridPoints[2]={21,21}; //
  textLabels[nt] = "plane grid points";
  sPrintF(textStrings[nt], "%i %i",planeGridPoints[0],planeGridPoints[1]);
  nt++;

//    textLabels[nt] = "normal to plane";
//    sPrintF(textLabels[nt], "%9.3g %9.3g %9.3g ",normal[0],normal[1],normal[2]); nt++; 

  textLabels[nt]="";  nt++;   // null string denotes last
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  gi.pushGUI(gui);
  gi.appendToTheDefaultPrompt("build curve>"); 


  PlaneMapping plane(xp[0][0],xp[1][0],xp[2][0],
                     xp[0][1],xp[1][1],xp[2][1],
		     xp[0][2],xp[1][2],xp[2][2]);
  
  plane.setGridDimensions(axis1,planeGridPoints[0]);
  plane.setGridDimensions(axis2,planeGridPoints[1]);


  SelectionInfo select; select.nSelect=0;
  int len;
  aString answer="plot", line;
  bool plotObject=true;
  int numberOfCurvesBuilt=0;
  
  for( int it=0; ; it++ )
  {
    if( it>0 )
      gi.getAnswer(answer,"", select);

    if( answer=="done" || answer=="exit" )
    {
      break;
    }
    else if( len=answer.matches("plot plane") )
    {
      sScanF(answer(len,answer.length()-1),"%i",&plotPlane);
      dialog.setToggleState("plot plane",plotPlane);       
    }
    else if( len=answer.matches("plane point") ) // handles all 3 cases
    {
      int ipt=answer.matches("plane point 1") ? 0 : answer.matches("plane point 2") ? 1 : 2;
      
      sScanF(answer(len+2,answer.length()-1),"%e %e %e",&xp[0][ipt],&xp[1][ipt],&xp[2][ipt]);

      printf(" Setting textLabel %s to %s \n",(const char*)answer(0,len),
               (const char*)sPrintF(line,"%g %g %g",xp[0][ipt],xp[1][ipt],xp[2][ipt]));
      
      dialog.setTextLabel(answer(0,len+1), sPrintF(line,"%g %g %g",xp[0][ipt],xp[1][ipt],xp[2][ipt]));

      plane.setPoints(xp[0][0],xp[1][0],xp[2][0],
		      xp[0][1],xp[1][1],xp[2][1],
		      xp[0][2],xp[1][2],xp[2][2]);
    }
    else if( len=answer.matches("plane grid points") )
    {
      sScanF(answer(len,answer.length()-1),"%i %i",&planeGridPoints[0],&planeGridPoints[1]);
      dialog.setTextLabel("plane grid points", sPrintF(line,"%i %i",planeGridPoints[0],planeGridPoints[1]));
      
      printf(" Setting points on the plane to %i,%i\n",planeGridPoints[0],planeGridPoints[1]);
      
      plane.setGridDimensions(axis1,planeGridPoints[0]);
      plane.setGridDimensions(axis2,planeGridPoints[1]);
    }
    
    else if ( answer=="cut with plane" )
    {

/* ---
      real x0=0., y0=0., z0=.5;
      real nx=0., ny=1., nz=0.;
      printf("build a curve on the reference surface by intersecting the reference surface with a plane\n");
      printf("The bounding box is [%8.2e,%8.2e]x[%8.2e,%8.2e]x[%8.2e,%8.2e]\n",
	     boundingBox(0,0),boundingBox(1,0),boundingBox(0,1),boundingBox(1,1),boundingBox(0,2),boundingBox(1,2));
      
      gi.inputString(answer,"Enter a point on the plane, x,y,z");
      sScanF(answer,"%e %e %e",&x0,&y0,&z0);
      gi.inputString(answer,"Enter the normal to the plane, nx,ny,nz");
      sScanF(answer,"%e %e %e",&nx,&ny,&nz);

      real norm = max(REAL_MIN,sqrt(nx*nx+ny*ny+nz*nz));
      nx/=norm;
      ny/=norm;
      nz/=norm;
      
      // find tangents to the plane
      real ta[3], tb[3];
      if( fabs(nx)>=max(fabs(ny),fabs(nz)) )
      {
	ta[0]=-ny, ta[1]=nx, ta[2]=0.;
      }
      else if( fabs(ny)>=fabs(nz) )
      {
	ta[0]=0., ta[1]=-nz, ta[2]=ny;
      }
      else
      {
	ta[0]=nz, ta[1]=0, ta[2]=-nx;
      }
      norm=max(REAL_MIN,sqrt(ta[0]*ta[0]+ta[1]*ta[1]+ta[2]*ta[2]));
      ta[0]/=norm;
      ta[1]/=norm;
      ta[2]/=norm;
      
      tb[0]=ny*ta[2]-nz*ta[1];
      tb[1]=nz*ta[0]-nx*ta[2];
      tb[2]=nx*ta[1]-ny*ta[0];
      

      // plane(r,s) = x0 + r*t0 + s*t1
      // a point is on the plane if n.(x-x0)=0

      // build a PlaneMapping large enough to cut the surface

      
      real x1=0., y1=0., z1=z0, 
	x2=1., y2=0., z2=z0, 
	x3=0., y3=1., z3=z0;
      
      real r=-scale;
      real s=-scale;
      x1=x0+r*ta[0]+s*tb[0];
      y1=y0+r*ta[1]+s*tb[1];
      z1=z0+r*ta[2]+s*tb[2];
      
      r=scale;
      x2=x0+r*ta[0]+s*tb[0];
      y2=y0+r*ta[1]+s*tb[1];
      z2=z0+r*ta[2]+s*tb[2];
      
      r=-scale;
      s=scale;
      x3=x0+r*ta[0]+s*tb[0];
      y3=y0+r*ta[1]+s*tb[1];
      z3=z0+r*ta[2]+s*tb[2];

      ---   */
      
      // Intersect the Plane Mapping with the CompositeSurface

      IntersectionMapping intersect;
      bool isCompositeSurface=surf.getClassName()=="CompositeSurface";
      
      bool success=false;
      if( isCompositeSurface )
      {
        CompositeSurface & cs = (CompositeSurface&)surf;
        success= intersect.intersect(plane,cs)==0;
      }
      else
        success= intersect.intersect(surf,plane)==0;

      for( axis=0; axis<surf.getRangeDimension(); axis++ )
      {
	printf(" After: intersect:  axis=%i xa=%e xb=%e\n",axis,boundingBox(Start,axis),boundingBox(End  ,axis));
      }
      
      if( success && intersect.curve!=NULL )
      {
	Mapping & curve = *intersect.curve;  // intersection curve in physical space.

//  	if( plotPlane )
//  	{
//  	  real oldCurveLineWidth;
//  	  parameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
//  	  parameters.set(GraphicsParameters::curveLineWidth,3.);

//  	  parameters.set(GI_MAPPING_COLOUR,"green");
//  	  PlotIt::plot(gi,curve,parameters);
	  
//  	  parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
//  	}
	
        NurbsMapping & nurb = (NurbsMapping&)curve;
        printf(" Creating %i new boundary curves from the curves of intersection.\n",nurb.numberOfSubCurves());

	numberOfCurvesBuilt+=nurb.numberOfSubCurves();
	
        if( numberOfExtraBoundaryCurves+nurb.numberOfSubCurves() >=maxNumberOfExtraBoundaryCurves )
	{
          printf("Sorry, too many boundary curves. Get Bill to fix this\n");
	}
	else
	{
          for( int sc=0; sc<nurb.numberOfSubCurves(); sc++ )
	  {
	    extraBoundaryCurve[numberOfExtraBoundaryCurves]=&nurb.subCurve(sc);
	    extraBoundaryCurve[numberOfExtraBoundaryCurves]->incrementReferenceCount();
	    numberOfExtraBoundaryCurves++;
	  }
	}
      }
      else
      {
	printf("\n ****No intersecting curves were found ****\n");
      }
    }
    else if( answer=="erase last curve" )
    {
      if( numberOfCurvesBuilt==0 )
      {
        printf("there are no curves to erase\n");
	continue;
      }
      numberOfCurvesBuilt--;
      numberOfExtraBoundaryCurves--;
      if( extraBoundaryCurve[numberOfExtraBoundaryCurves]->decrementReferenceCount()==0 )
      {
	delete extraBoundaryCurve[numberOfExtraBoundaryCurves];
	extraBoundaryCurve[numberOfExtraBoundaryCurves]=NULL;
      }
    }
    else if( answer=="edit intersection curve" )
    {
      if( numberOfExtraBoundaryCurves>0 )
      {
        for( int n=0; n<numberOfExtraBoundaryCurves; n++ )
	{
	  printf("edit extra boundary curve %i\n",n);
	  gi.erase();
	  extraBoundaryCurve[n]->update(mapInfo);
	}
	
      }
      plotObject=true;
    }
    else if( answer=="add last curve to mapping list" )
    {
      if( numberOfCurvesBuilt>0 )
      {
        assert( extraBoundaryCurve[numberOfExtraBoundaryCurves-1]!=NULL );
	mapInfo.mappingList.addElement(*extraBoundaryCurve[numberOfExtraBoundaryCurves-1]);
      }
      else
      {
	printf("ERROR: there are no curves built to add!\n");
      }
    }
    else if( answer=="create trimmed surface" )
    {
      // Build a trimmed surface by cutting with a plane
         
      // -- finish me : this should eventually be a separate function...
      printF(" ERROR: 'create trimmed surface' not implemented yet\n");

    }
    else if( answer=="plot" )
    {
    }
    else
    {
      gi.outputString( sPrintF(line,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }
    
    if( plotObject )
    {
      plot(mapInfo);
      if( plotPlane )
      {
	PlotIt::plot(gi,plane,parameters);
      }      
      
    }

  } // end for(it; )

  gi.popGUI(); 
  gi.unAppendTheDefaultPrompt();

  return 0;
}

int MappingBuilder::
getBoundaryCurves()
// ===============================================================================================
// /Description:
//   Return the curves that can be used for starting curves.
// ===============================================================================================
{
  if( pSurface==NULL )
    return 0;

  Mapping *surface =pSurface;

  if( numberOfBoundaryCurves==0 && surface->getClassName()=="UnstructuredMapping" )
  {
    ((UnstructuredMapping*)surface)->findBoundaryCurves(numberOfBoundaryCurves, boundaryCurves);
    printf("MappingBuilder::getBoundaryCurves %i boundary curves found from triangulation. \n",numberOfBoundaryCurves);
  }
  else if( numberOfBoundaryCurves==0 && surface->getClassName()=="CompositeSurface")
  {
    ((CompositeSurface*)surface)->findBoundaryCurves(numberOfBoundaryCurves, boundaryCurves);
    printf("MappingBuilder::getBoundaryCurves %i boundary curves found from CompositeSurface\n",numberOfBoundaryCurves);
  }

  return 0;
}

// ********************************************************************************************************
 
int MappingBuilder::
buildSurfacePatch(MappingInformation & mapInfo)
// ============================================================================================
// /Description:
//     Build a patch on the surface defined by bounding curves.
//
//   We first build a TFI mapping from the curves then project the TFI onto the surface.
// ===========================================================================================
{
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;


  aString answer,line;
  SelectionInfo select; select.nSelect=0;  
  if( pSurface==NULL )
  {
    printf("buildSurfacePatch:Sorry need a surface to build a surface patch on\n");
    return 1;
  }
   
  Mapping *surface = pSurface;
  const bool isCompositeSurface = surface->getClassName()=="CompositeSurface";

  getBoundaryCurves();
  
  // turn off plotting of surface and volume grids.
  const bool plotSurfaceGridsSave=plotSurfaceGrids;
  const bool plotVolumeGridsSave=plotVolumeGrids;
  const bool plotEdgeCurvesSave=plotEdgeCurves;
  
  plotSurfaceGrids=false;
  plotVolumeGrids=false;
  plotEdgeCurves=true;
  
  // turn off patch edges since we will plot edges separately
  referenceSurfaceParameters.set(GI_PLOT_MAPPING_EDGES,false);
  if( isCompositeSurface )
  {
    // force a redraw
    CompositeSurface & cs = (CompositeSurface&)(*surface);
    cs.eraseCompositeSurface(gi);
  }
  

  const aString boundaryColour[]={"green","red","blue","yellow"};

  int numberOfGridLines[2]={21,21};
  int explicitGhostLines[2][3]={0,0,0,0,0,0}; //  aka boundary offset
  
  Mapping *patch=NULL;  // holds the patch we build
  Mapping *curve[4]={NULL,NULL,NULL,NULL};   // pointers to 4 boundary curves
  			
  GUIState gui;
  DialogData & dialog = gui;
  dialog.setWindowTitle("Build a surface patch from bounding curves");
  dialog.setExitCommand("exit", "exit");


//    aString bcPlotOptionCommand[] = { "colour boundaries by grid number",
//  				    "colour boundaries by BC number",
//  				    "colour boundaries by share number",
//  				    "" };
//    dialog.addRadioBox("volume grids boundary colour:",bcPlotOptionCommand, bcPlotOptionCommand, 
//  		     (int)bcPlotOption);

  enum PickingOptionsEnum
  {
    pickLeftCurve,
    pickRightCurve,
    pickBottomCurve,
    pickTopCurve,
    pickToDoNothing
  };
  PickingOptionsEnum pickingOption=pickLeftCurve;
  
  aString opLabel1[] = {"choose left curve",
                        "choose right curve",
                        "choose bottom curve",
                        "choose top curve",
                        "do nothing",""};  //
  int numberOfColumns=2;
  dialog.addRadioBox("Picking:", opLabel1,opLabel1,(int)pickingOption,numberOfColumns);


   aString pbLabels[] = {"build patch",
                         "clear curves",
                         "clear patch",
                         "reverse left curve",
                         "reverse right curve",
                         "reverse bottom curve",
                         "reverse top curve",
                         "remove twist from periodic patch",
                         "stretching...",
                         "smoothing...",
			 ""};
   int numRows=5;
   dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

   aString tbCommands[] = {"project patch onto surface",
                           "plot boundary curves",
                           "plot edge curves",
			   "plot reference surface",
                           "plot patch curves",
  			   ""};

   int projectPatchOntoSurface=true;
   bool plotPatchCurves=true;

   int tbState[6];
   tbState[0] = projectPatchOntoSurface==true; 
   tbState[1] = plotBoundaryCurves==true; 
   tbState[2] = plotEdgeCurves==true; 
   tbState[3] = plotReferenceSurface==true; 
   tbState[4] = plotPatchCurves==true; 
  
   int numColumns=1;
   dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


  const int numberOfTextStrings=5;
  aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "lines"; 
  sPrintF(textStrings[nt], "%i %i",numberOfGridLines[0],numberOfGridLines[1]); nt++; 

  textLabels[nt] = "explicit ghost lines"; 
  sPrintF(textStrings[nt], "%i %i %i %i (l r b t)",explicitGhostLines[0][0],explicitGhostLines[1][0],
                          explicitGhostLines[0][1],explicitGhostLines[1][1] ); nt++; 

  aString name = "surfacePatch";
  textLabels[nt] = "name"; 
  sPrintF(textStrings[nt], "%s", (const char*)name);
  nt++;

  textLabels[nt]=""; 
  
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  // --- Build the sibling dialog for setting post stretching parameters ---
  int domainDimension=2,rangeDimension=3;
  
  GridStretcher gridStretcher(domainDimension,rangeDimension);
  DialogData & stretchDialog = gui.getDialogSibling();
  stretchDialog.setWindowTitle("Stretching of Grid Lines");
  stretchDialog.setExitCommand("close stretching options", "close");
  gridStretcher.buildDialog(stretchDialog);

  GridSmoother gridSmoother(domainDimension,rangeDimension);
  IntegerArray bc(2,3);
  bc = (int) GridSmoother::pointsFixed; // pointsSlide;
  gridSmoother.setBoundaryConditions( bc );
  DialogData & smoothDialog = gui.getDialogSibling();
  smoothDialog.setWindowTitle("Smoothing of Grid Lines");
  smoothDialog.setExitCommand("close smoothing options", "close");
  gridSmoother.buildDialog(smoothDialog);

  gi.pushGUI(gui);
  gi.appendToTheDefaultPrompt("surfacePatch>");  

  int stretchReturnValue=-1;
  DataPointMapping *dpm=NULL;

  // patchIndexRange: grid points we place on the patch unit square
  // gridIndexRange: bounds on grid lines
  // projectIndexRange: These are points we can project
  IntegerArray patchIndexRange(2,3),gridIndexRange(2,3),projectIndexRange(2,3);
  patchIndexRange=0; gridIndexRange=0;  projectIndexRange=0;
  realArray xGrid;

  const aString patchColour="turquoise";
  int side,axis;
  
  int len=0;
  for( int it=0; ; it++ )
  {
    bool plotObject=true;
    if( it>0 )
    {
      gi.savePickCommands(false); // temporarily turn off saving of pick commands.     

      gi.getAnswer(answer,"", select);

      printf("answer=[%s]\n",(const char*)answer);

      gi.savePickCommands(true); // turn back on
    }
    else
    {
      answer="plot";
    }
    if( answer=="exit" )
    {
      break;
    }
    else if( len=answer.matches("plot reference surface") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value); plotReferenceSurface=value;
      dialog.setToggleState("plot reference surface",plotReferenceSurface);
    }
    else if( len=answer.matches("plot patch curves") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value); plotPatchCurves=value;
      dialog.setToggleState("plot patch curves",plotPatchCurves);
    }
    else if( answer.matches("choose left curve") )
    {
      pickingOption=pickLeftCurve;
      dialog.getRadioBox(0).setCurrentChoice(pickingOption);
      continue;
    }
    else if( answer.matches("choose right curve") )
    {
      pickingOption=pickRightCurve;
      dialog.getRadioBox(0).setCurrentChoice(pickingOption);
      continue;
    }
    else if( answer.matches("choose bottom curve") )
    {
      pickingOption=pickBottomCurve;
      dialog.getRadioBox(0).setCurrentChoice(pickingOption);
      continue;
    }
    else if( answer.matches("choose top curve") )
    {
      pickingOption=pickTopCurve;
      dialog.getRadioBox(0).setCurrentChoice(pickingOption);
      continue;
    }
    else if( answer.matches("do nothing") )
    {
      pickingOption=pickToDoNothing;
      dialog.getRadioBox(0).setCurrentChoice(pickingOption);
      continue;
    }
    else if( len=answer.matches("project patch onto surface") )
    {
      sScanF(answer(len,answer.length()-1),"%i",&projectPatchOntoSurface); 
      dialog.setToggleState("project patch onto surface",(bool)projectPatchOntoSurface);
      continue;
    }
    else if( len=answer.matches("plot boundary curves") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value); plotBoundaryCurves=value;
      dialog.setToggleState("plot boundary curves",(bool)plotBoundaryCurves);
    }
    else if( len=answer.matches("plot edge curves") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value); plotEdgeCurves=value;
      dialog.setToggleState("plot edge curves",(bool)plotEdgeCurves);
    }
    else if( len=answer.matches("lines") )
    {
      sScanF(answer(len,answer.length()-1),"%i %i",&numberOfGridLines[0],&numberOfGridLines[1]); 
      dialog.setTextLabel("lines",sPrintF(answer,"%i %i",numberOfGridLines[0],numberOfGridLines[1]));
      if( patch!=NULL )
      {
	patch->setGridDimensions(0,numberOfGridLines[0]);
	patch->setGridDimensions(1,numberOfGridLines[1]);
      }
    }
    else if( len=answer.matches("explicit ghost lines") )
    {
      sScanF(answer(len,answer.length()-1),"%i %i %i %i",&explicitGhostLines[0][0],&explicitGhostLines[1][0],
	     &explicitGhostLines[0][1],&explicitGhostLines[1][1]);
      for( side=0; side<=1; side++ )
      {
	for( axis=0; axis<domainDimension; axis++ )
	{
	  explicitGhostLines[side][axis]=max(0,explicitGhostLines[side][axis]);
	}
      }
      dialog.setTextLabel("explicit ghost lines",sPrintF(answer, "%i %i %i %i (l r b t)",
							 explicitGhostLines[0][0],explicitGhostLines[1][0],
							 explicitGhostLines[0][1],explicitGhostLines[1][1] )); 
      gridIndexRange=patchIndexRange;
      for( axis=0; axis<domainDimension; axis++ )
      {
	for( side=0; side<=1; side++ )
	{
	  gridIndexRange(side,axis)+=explicitGhostLines[side][axis]*(1-2*side);
	}
        numberOfGridLines[axis]=gridIndexRange(1,axis)-gridIndexRange(0,axis)+1;
      }
      if( dpm!=NULL )
      {
	::display(patchIndexRange,"patch: patchIndexRange");
	::display(gridIndexRange,"patch: gridIndexRange");

//          const realArray & xd = dpm->getDataPoints(); 
//  	realArray x(xd.getLength(0),xd.getLength(1),xd.getLength(2),xd.getLength(3));  // we want base 0
//  	x=xd;  
//          printf(" xd: [%i,%i][%i,%i]\n",xd.getBase(0),xd.getBound(0),xd.getBase(1),xd.getBound(1));
//          printf(" x:  [%i,%i][%i,%i]\n",x.getBase(0),x.getBound(0),x.getBase(1),x.getBound(1));
	
        
        printf("INFO: Setting number of grid lines to %i,%i to match new explicit ghost lines\n",
               numberOfGridLines[0],numberOfGridLines[1],targetGridSpacing[0]);
	
        dialog.setTextLabel("lines",sPrintF(answer,"%i %i",numberOfGridLines[0],numberOfGridLines[1]));


        dpm->setDataPoints(xGrid,3,domainDimension,0,gridIndexRange);         
      }
    }
    else if( len=answer.matches("name") )
    {
      name = answer(len,answer.length()-1);
      dialog.setTextLabel("name",(const char*)name);
      if( patch!=NULL )
        patch->setName(Mapping::mappingName,name);
      continue;
    }
    else if( (len=answer.matches("left curve is boundary curve")) ||
             (len=answer.matches("right curve is boundary curve")) ||
             (len=answer.matches("bottom curve is boundary curve")) ||
             (len=answer.matches("top curve is boundary curve")) )
    {
      int n = answer.matches("left curve is boundary curve") ? 0 :
              answer.matches("right curve is boundary curve") ? 1 :
      	      answer.matches("bottom curve is boundary curve") ? 2 : 3;

      int b=-1;
      sScanF(answer(len,answer.length()-1),"%i",&b);
      if( b>=0 && b<numberOfBoundaryCurves )
      {
	curve[n]=boundaryCurves[b];
      }
      else
      {
	printf("ERROR: invalid boundary curve number %i\n",b);
	gi.stopReadingCommandFile();
      }
    }
    else if( (len=answer.matches("left curve is extra boundary curve")) ||
             (len=answer.matches("right curve is extra boundary curve")) ||
             (len=answer.matches("bottom curve is extra boundary curve")) ||
             (len=answer.matches("top curve is extra boundary curve")) )
    {
      int n = answer.matches("left curve is extra boundary curve") ? 0 :
              answer.matches("right curve is extra boundary curve") ? 1 :
      	      answer.matches("bottom curve is extra boundary curve") ? 2 : 3;

      int b=-1;
      sScanF(answer(len,answer.length()-1),"%i",&b);
      if( b>=0 && b<numberOfExtraBoundaryCurves )
      {
	curve[n]=extraBoundaryCurve[b];
      }
      else
      {
	printf("ERROR: invalid boundary curve number %i\n",b);
	gi.stopReadingCommandFile();
      }
    }
    else if( (len=answer.matches("left curve is edge curve")) ||
             (len=answer.matches("right curve is edge curve")) ||
             (len=answer.matches("bottom curve is edge curve")) ||
             (len=answer.matches("top curve is edge curve")) )
    {
      int n = answer.matches("left curve is edge curve") ? 0 :
              answer.matches("right curve is edge curve") ? 1 :
      	      answer.matches("bottom curve is edge curve") ? 2 : 3;

      int e=-1;
      sScanF(answer(len,answer.length()-1),"%i",&e);
      int numberOfEdgeCurves=0;
      if( isCompositeSurface && ((CompositeSurface*)surface)->getCompositeTopology()!=NULL )
      {
	CompositeSurface & cs = (CompositeSurface&)(*surface);
	CompositeTopology & compositeTopology = *cs.getCompositeTopology();

	int numberOfEdgeCurves=compositeTopology.getNumberOfEdgeCurves();
	if( e>=0 && e<numberOfEdgeCurves )
	{
	  curve[n]=&compositeTopology.getEdgeCurve(e);
	}
      }
      
      if( e<0 && e>=numberOfEdgeCurves )
      {
	printf("ERROR: invalid edge curve number %i, numberOfEdgeCurves=%i \n",e,numberOfEdgeCurves);
	gi.stopReadingCommandFile();
      }
     
    }
    else if( (len=answer.matches("reverse left curve")) ||
             (len=answer.matches("reverse right curve")) ||
             (len=answer.matches("reverse bottom curve")) ||
             (len=answer.matches("reverse top curve")) )
    {
      int n = answer.matches("reverse left curve") ? 0 :
              answer.matches("reverse right curve") ? 1 :
      	      answer.matches("reverse bottom curve") ? 2 : 3;

      if( curve[n]!=NULL )
      {
	if( curve[n]->getClassName()=="NurbsMapping" )
	{
	  NurbsMapping & nurbs=(NurbsMapping &)(*curve[n]);
          real ra,rb;
          nurbs.getParameterBounds(0,ra,rb);
	  nurbs.setDomainInterval(rb,ra);
          printf("New parameter bounds [rb,ra]=[%8.2e,%8.2e]\n",rb,ra);
	}
        else
	{
          printf(" curve is not a NURBS. Not implemented yet\n");
	}
      }
    }
    else if( answer=="clear curves" )
    {
      for( int c=0; c<4; c++ )
      {
	curve[c]=NULL;
      }
    }
    else if( answer=="clear patch" )
    {
//        if( patch!=NULL && patch->decrementReferenceCount()==0 )
//  	delete patch;
      patch=NULL;
    }
    else if( answer=="remove twist from periodic patch" )
    {
      int numSpecified = int(curve[0]!=NULL)+int(curve[1]!=NULL)+int(curve[2]!=NULL)+int(curve[3]!=NULL);
      if( numSpecified==2 && ( (curve[0]!=NULL && curve[1]!=NULL) || (curve[2]!=NULL && curve[3]!=NULL)) )
      {
        const int n1=curve[0]!=NULL ? 0 : 2;
        const int n2=n1+1;
	if( (bool)curve[n1]->getIsPeriodic(0) && (bool)curve[n2]->getIsPeriodic(0) )
	{
	  // try to remove the twist from a patch made from two periodic curves
	  printf("*** try to remove the twist from a patch made from two periodic curves ***\n");
	  
          const realArray & x1 = curve[n1]->getGrid();
          // const realArray & x2 = curve[n2]->getGrid();
	  
          realArray x(1,3), r(1,1), xm(1,3),rm(1,1);
	  x(0,0)=x1(0,0,0,0);
	  x(0,1)=x1(0,0,0,1);
	  x(0,2)=x1(0,0,0,2);
	  
          r=-1.;
          curve[n2]->inverseMap(x,r);
 
          // check the midpoint too
          int im=x1.getLength(0)/2;
	  xm(0,0)=x1(im,0,0,0);
	  xm(0,1)=x1(im,0,0,1);
	  xm(0,2)=x1(im,0,0,2);
          rm=-1;
	  curve[n2]->inverseMap(xm,rm);

          real rAverage=.5*(rm(0,0)-.5 + r(0,0));
          printf("The branch point r=0 on curve %i, x=(%8.2e,%8.2e,%8.2e) matches to r=%9.3e on curve %i.\n"
                 "The offset near the midpoint is %9.3e. The average offset is %10.4e \n",
                 n1,x(0,0),x(0,1),x(0,2),r(0,0),n2, rm(0,0)-.5,rAverage);

	  r(0,0)=rAverage;
          gi.inputString(answer,sPrintF(line,"Enter a value for new branch point on curve %i (default=%9.3e)\n",n2,
                      r(0,0)));
	  if( answer!="" )
	  {
	    sScanF(answer,"%e",&r(0,0));
	  }
	  if( curve[n2]->getClassName()=="NurbsMapping" )
	  {
	    NurbsMapping & nurbs=(NurbsMapping &)(*curve[n2]);
	    real ra,rb;
	    nurbs.getParameterBounds(0,ra,rb);
            if( r(0,0)<.5 )
	    {
	      ra=r(0,0)-1.;
	      rb=ra+1.;
	    }
	    else
	    {
              ra=r(0,0)-1.;
              rb=ra+1.;
	    }
	    nurbs.setDomainInterval(ra,rb);
	    printf("New parameter bounds for curve %i [ra,rb]=[%8.2e,%8.2e]\n",n2,ra,rb);
	  }
	  else
	  {
	    printf(" curve is not a NURBS. Not implemented yet\n");
	  }
	  
	}
	else
	{
          printf("ERROR: the two curves are not periodic\n");
	}
	
      }
      else
      {
	printf("ERROR: there should be exactly two periodic curves specified\n");
      }
      
      printf("Choose `build patch' to recompute the patch with the new twist\n");
      
      continue;
    }
    else if( answer=="build patch" )
    {
      assert( patch==NULL || patch==dpm );
      
      patch = new TFIMapping(curve[0],curve[1],curve[2],curve[3]);
      patch->incrementReferenceCount();
      patch->setName(Mapping::mappingName,name); 

      for(axis=0; axis<domainDimension; axis++ )
      {
        for( side=0; side<=1; side++ )
	{
	  int n=side+2*axis;
	  if( curve[n]==NULL && !(bool)patch->getIsPeriodic(axis) )
	    explicitGhostLines[side][axis]=1;
	  else
	    explicitGhostLines[side][axis]=0;
	}
      }
      dialog.setTextLabel("explicit ghost lines",sPrintF(answer, "%i %i %i %i (l r b t)(green red blue yellow)",
							 explicitGhostLines[0][0],explicitGhostLines[1][0],
							 explicitGhostLines[0][1],explicitGhostLines[1][1] ));

      if( targetGridSpacing[0]>0. && numberOfGridLines[0]==21 && numberOfGridLines[1]==21 )
      {
        real arcLength[4]={0.,0.,0.,0.};
        for( int i=0; i<4; i++ )
	{
	  if( curve[i]!=NULL )
	    arcLength[i]=curve[i]->getArcLength();
	}
        if( arcLength[0]!=0. || arcLength[1]!=0. )
	{
	  real arc=arcLength[0]+arcLength[1];
          if( arcLength[0]!=0. && arcLength[1]!=0. ) 
            arc*=.5;
	  
	  numberOfGridLines[1]=max(5,int(arc/targetGridSpacing[0]+.5));
	}
	else if( curve[2]!=NULL && curve[3]!=NULL )
	{
	  const realArray & x1 = curve[2]->getGrid();
	  const realArray & x2 = curve[3]->getGrid();
	  
          real arc = sqrt( SQR(x1(0,0,0,0)-x2(0,0,0,0))+
                           SQR(x1(0,0,0,1)-x2(0,0,0,1))+
                           SQR(x1(0,0,0,2)-x2(0,0,0,2)) );
	  
	  numberOfGridLines[1]=max(5,int(arc/targetGridSpacing[0]+.5));

	}
        if( arcLength[2]!=0. || arcLength[3]!=0. )
	{
	  real arc=arcLength[2]+arcLength[3];
          if( arcLength[2]!=0. && arcLength[3]!=0. ) 
            arc*=.5;
	  numberOfGridLines[0]=max(5,int(arc/targetGridSpacing[0]+.5));
	}
	else if( curve[0]!=NULL && curve[1]!=NULL )
	{
	  const realArray & x1 = curve[0]->getGrid();
	  const realArray & x2 = curve[1]->getGrid();
	  
          real arc = sqrt( SQR(x1(0,0,0,0)-x2(0,0,0,0))+
                           SQR(x1(0,0,0,1)-x2(0,0,0,1))+
                           SQR(x1(0,0,0,2)-x2(0,0,0,2)) );
	  
	  numberOfGridLines[0]=max(5,int(arc/targetGridSpacing[0]+.5));
	}
        printf("INFO: Setting number of grid lines to %i,%i from targetSpacing=%8.2e\n",
               numberOfGridLines[0],numberOfGridLines[1],targetGridSpacing[0]);
	
        dialog.setTextLabel("lines",sPrintF(answer,"%i %i",numberOfGridLines[0],numberOfGridLines[1]));
      }
      

      // Build a DPM or Nurbs surface
      if( dpm==NULL )
      {
	dpm = new DataPointMapping;
	dpm->incrementReferenceCount();
      }
      dpm->setName(Mapping::mappingName,name); 
      for(axis=0; axis<domainDimension; axis++ )
      {
        // patchIndexRange: grid points we place on the patch unit square
        patchIndexRange(0,axis)=-explicitGhostLines[0][axis];
	patchIndexRange(1,axis)=+explicitGhostLines[1][axis]+numberOfGridLines[axis]-1;
	
	patch->setGridDimensions(axis,patchIndexRange(1,axis)-patchIndexRange(0,axis)+1);
        dpm->setIsPeriodic(axis,patch->getIsPeriodic(axis));
      }
      // gridIndexRange: bounds on grid lines
      gridIndexRange=0;
      for( axis=0; axis<patch->getDomainDimension(); axis++ )
	gridIndexRange(1,axis)=numberOfGridLines[axis]-1;

      // projectIndexRange: These are points we can project
      projectIndexRange=patchIndexRange;
      
//        gid=gridIndexRange;
//        for( side=0; side<=1; side++ )
//        {
//  	for( axis=0; axis<domainDimension; axis++ )
//  	{
//  	  gid(side,axis)+=explicitGhostLines[side][axis]*(1-2*side);
//  	}
//        }
      ::display(patchIndexRange,"patch: patchIndexRange");
      ::display(gridIndexRange,"patch: gridIndexRange");
//      ::display(gid,"patch: gid");
      

      Index I1,I2,I3;
      int numberOfGhostLines=1;
      getIndex(gridIndexRange,I1,I2,I3,numberOfGhostLines);
      
      real dr[2];
      for( axis=0; axis<domainDimension; axis++ )
        dr[axis]=1./max(1,patchIndexRange(1,axis)-patchIndexRange(0,axis));
      
      realArray r(I1,I2,I3,domainDimension);
      xGrid.redim(I1,I2,I3,rangeDimension);

      int i1,i2,i3=0;
      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
        r(I1,i2,i3,1)=(i2-patchIndexRange(0,1))*dr[1];
      for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
        r(i1,I2,i3,0)=(i1-patchIndexRange(0,0))*dr[0];

      // ::display(r,"r");
      
      patch->mapGrid(r,xGrid);
      
      // xGrid.redim(0);
      // xGrid=patch->getGrid();   // what about ghost points?  --> specify BC's with a option

      if( projectPatchOntoSurface )
      {
        // projectAndSmooth(surface,x);

	printf("*** project points onto the reference surface...\n");
       
	MappingProjectionParameters mpParams;
	typedef MappingProjectionParameters MPP;
	mpParams.setIsAMarchingAlgorithm(false);
	intArray & subSurfaceIndex = mpParams.getIntArray(MPP::subSurfaceIndex);
	subSurfaceIndex=-1;  // set initial guess

	getIndex(projectIndexRange,I1,I2,I3); // we project these points
        Range Rx=rangeDimension;
        realArray x(I1,I2,I3,Rx);
	x(I1,I2,I3,Rx)=xGrid(I1,I2,I3,Rx);

        Range R=I1.getLength()*I2.getLength()*I3.getLength();
        x.reshape(R,Rx);
	
	surface->project(x,mpParams);

        x.reshape(I1,I2,I3,Rx);

        xGrid(I1,I2,I3,Rx)=x(I1,I2,I3,Rx);
	
      }

      dpm->setDataPoints(xGrid,3,domainDimension,0,gridIndexRange);

      assert( patch!=dpm );
      if( patch->decrementReferenceCount()==0 )
        delete patch;

      patch=dpm;
//        for(int axis=0; axis<2; axis++ )
//    	patch->setGridDimensions(axis,numberOfGridLines[axis]);

      // these are for the grid stretcher
      // make a copy of the original grid
//        xGrid.redim(0);
//        xGrid=patch->getGrid();  // fix this -- need ghost points

    }
    else if( select.nSelect )
    {
      if( pickingOption==pickLeftCurve ||
          pickingOption==pickRightCurve ||    
          pickingOption==pickBottomCurve ||
          pickingOption==pickTopCurve )
      {
	Mapping *curveChosen=NULL;
      
	for (int i=0; i<select.nSelect && curveChosen==NULL; i++)
	{
          int b;
	  for( b=0; b<numberOfBoundaryCurves; b++ )
	  {
//              printf(" boundaryCurves ID=%i select.selection(i,0)=%i\n",
//  		   boundaryCurves[b]->getGlobalID(),select.selection(i,0));
	    
	    if( boundaryCurves[b]->getGlobalID()==select.selection(i,0) )
	    {
	      printf("Boundary curve %i chosen.\n",b);
	      curveChosen=boundaryCurves[b];

              if( pickingOption==pickLeftCurve )
                gi.outputToCommandFile(sPrintF(line,"left curve is boundary curve %i\n",b));
              else if( pickingOption==pickRightCurve )
                gi.outputToCommandFile(sPrintF(line,"right curve is boundary curve %i\n",b));
              else if( pickingOption==pickBottomCurve )
                gi.outputToCommandFile(sPrintF(line,"bottom curve is boundary curve %i\n",b));
              else 
                gi.outputToCommandFile(sPrintF(line,"top curve is boundary curve %i\n",b));
	      break;
	    }
	  }
	  for( b=0; b<numberOfExtraBoundaryCurves; b++ )
	  {
	    if( extraBoundaryCurve[b]->getGlobalID()==select.selection(i,0) )
	    {
	      printf("ExtraBoundary curve %i chosen.\n",b);
	      curveChosen=extraBoundaryCurve[b];

              if( pickingOption==pickLeftCurve )
                gi.outputToCommandFile(sPrintF(line,"left curve is extra boundary curve %i\n",b));
              else if( pickingOption==pickRightCurve )
                gi.outputToCommandFile(sPrintF(line,"right curve is extra boundary curve %i\n",b));
              else if( pickingOption==pickBottomCurve )
                gi.outputToCommandFile(sPrintF(line,"bottom curve is extra boundary curve %i\n",b));
              else 
                gi.outputToCommandFile(sPrintF(line,"top curve is extra boundary curve %i\n",b));
	      break;
	    }
	  }

          // ***** check edge curves *****
	  if( curveChosen==NULL &&
              isCompositeSurface && ((CompositeSurface*)surface)->getCompositeTopology()!=NULL )
	  {
	    CompositeSurface & cs = (CompositeSurface&)(*surface);
	    CompositeTopology & compositeTopology = *cs.getCompositeTopology();

	    int numberOfEdgeCurves=compositeTopology.getNumberOfEdgeCurves();
	    for( int e=0; e<numberOfEdgeCurves; e++ )
	    {
	      Mapping & edge = compositeTopology.getEdgeCurve(e);
	      if( edge.getGlobalID()==select.selection(i,0) )
	      {
		printf("Edge curve %i chosen.\n",e);
		curveChosen=&edge;

		if( pickingOption==pickLeftCurve )
		  gi.outputToCommandFile(sPrintF(line,"left curve is edge curve %i\n",e));
		else if( pickingOption==pickRightCurve )
		  gi.outputToCommandFile(sPrintF(line,"right curve is edge curve %i\n",e));
		else if( pickingOption==pickBottomCurve )
		  gi.outputToCommandFile(sPrintF(line,"bottom curve is edge curve %i\n",e));
		else 
		  gi.outputToCommandFile(sPrintF(line,"top curve is edge curve %i\n",e));
		break;
	      }
	    }

	  }

	}
        if( curveChosen!=NULL )
	{
	  int n = pickingOption==pickLeftCurve ? 0 : pickingOption==pickRightCurve ? 1 : 
	    pickingOption==pickBottomCurve ? 2 : 3;

	  curve[n]=curveChosen;
	}
	else
	{
          printf("No curve chosen: There are %i boundary curves and %i extra boundary curves to choose from\n",
                    numberOfBoundaryCurves,numberOfExtraBoundaryCurves);
	}
	
      }
    }
    else if( answer=="stretching..." )
    {
      stretchDialog.showSibling();
      continue;
    }
    else if( answer=="close stretching options" )
    {
      stretchDialog.hideSibling();
      continue;
    }
    else if( dpm!=NULL && 
             (stretchReturnValue=gridStretcher.update(answer,stretchDialog,mapInfo,xGrid,
                                                      gridIndexRange,projectIndexRange,*dpm,surface)) )
    {
      // NOTE: in the call to gridStretcher.update: 
      //               gridIndexRange: marks the actual boundaries
      //               gridIndexRange: marks the points to be projected on a surface grid
      printf("answer=%s was processed by gridStretcher.update, returnValue=%i\n",(const char*)answer,
             stretchReturnValue);
      if( stretchReturnValue==GridStretcher::gridWasChanged )
      {
      }
      else
      {
	continue;
      }
    }
    else if( answer=="smoothing..." )
    {
      smoothDialog.showSibling();
      continue;
    }
    else if( answer=="close new smoothing options" )
    {
      smoothDialog.hideSibling();
      continue;
    }
    else if( gridSmoother.updateOptions( answer,smoothDialog,mapInfo ) )
    {
      printf("answer=%s was processed by the gridSmoother\n",(const char*)answer);
      
      if( answer.matches("GSM:smooth grid") ||
          answer.matches("smooth grid") )
      {
	assert( surface!=NULL );
	assert( dpm!=NULL );
      
        int projectGhost[2][3]={0,0,0,0,0,0};
        Mapping *boundaryMappings[2][3]={NULL,NULL,NULL,NULL,NULL,NULL}; //
	for( side=0; side<=1; side++ )
	{
	  for( axis=0; axis<domainDimension; axis++ )
	  {
	    projectGhost[side][axis]=explicitGhostLines[side][axis]>0;
            boundaryMappings[side][axis]=curve[side+2*axis];
	  }
	}
        // **** supply Mappings to the GridSmoother for projecting boundaries ****
	gridSmoother.setBoundaryMappings( boundaryMappings );
        // gridSmoother.setMatchingCurves( matchingCurves );
	gridSmoother.smooth(*surface,*dpm,gi,parameters,projectGhost );

      }
      else
      {
	continue;
      }
    }
    else if( answer=="plot" )
    {
    }
    else
    {
      gi.outputString( sPrintF(line,"Unknown response=%s",(const char*)answer) );
      printf("Unknown response=[%s] \n",(const char*)answer);
      gi.stopReadingCommandFile();
    }
    
    if( plotObject )
    {
      plot(mapInfo);
    
      if( patch!=NULL )
      {
	// Plot the patch
	parameters.set(GI_NUMBER_OF_GHOST_LINES_TO_PLOT,1);
        parameters.set(GI_MAPPING_COLOUR,patchColour);
	PlotIt::plot(gi,*patch,parameters);

	parameters.set(GI_NUMBER_OF_GHOST_LINES_TO_PLOT,(plotGhostPoints? 1 : 0)); 

	// On a surface grid plot the boundary lines so we know where the ghost points are
	real oldCurveLineWidth;
	parameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
	parameters.set(GraphicsParameters::curveLineWidth,5.);

	for( axis=0; axis<domainDimension; axis++ )
	{
	  for( side=0; side<=1; side++ )
	  {
	    // if the grid fails with a negative volume there may be only on grid line
	    if( dpm->getGridDimensions((axis+1)%domainDimension) > 1 ) 
	    {
	      ReductionMapping edge(*dpm,axis,(real)side);

	      parameters.set(GI_MAPPING_COLOUR,boundaryColour[side+2*axis]);
	      PlotIt::plot(gi,edge,parameters);
	    }
	  }
	}
	parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);


      }

      // plot patch boundary curves
      if( plotPatchCurves )
      {
	for( int c=0; c<4; c++ )
	{
	  if( curve[c]!=NULL ) // && boundaryChosen[c] )
	  {
	    real oldCurveLineWidth;
	    parameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
	    parameters.set(GraphicsParameters::curveLineWidth,4.);

	    parameters.set(GI_MAPPING_COLOUR,boundaryColour[c]);
	    PlotIt::plot(gi,*curve[c],parameters);
	  
	    parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
	  }
	}
      }
    }
    
//      parameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,TRUE );
//      parameters.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,plotNonPhysicalBoundaries);
      
//      PlotIt::plot(gi,volumeGrids,parameters);
//      parameters.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,false);
	
    
  }

  if( patch!=NULL )
  {
    printf("Add the patch as a new surface grid\n");
    
    surfaceGrids.add(*patch);
    numberOfSurfaceGrids++;

    if( patch->decrementReferenceCount()==0 )
      delete patch;
  }
  else
  {
    printf("No new patch was created\n");
  }

  // reset:
  plotSurfaceGrids=plotSurfaceGridsSave;
  plotVolumeGrids=plotVolumeGridsSave;
  plotEdgeCurves=plotEdgeCurvesSave;

  gi.popGUI();
  gi.unAppendTheDefaultPrompt();

  return 0;
}



int MappingBuilder::
build(  MappingInformation & mapInfo, Mapping *surface /* = NULL */ )
// ========================================================================================
// /Description:
//     Interface for building multiple hyperbolic grids on a composite surface.
//  With this function you can build surface grids, volume grids and box grids. You can also
//  set boundary conditions by picking sides with the mouse.
// ========================================================================================
{
  int returnValue=0;
  
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  
//  int plotNonPhysicalBoundaries=TRUE;
//  bool plotQualityOfCells=FALSE;

  plotReferenceSurface=TRUE;
  choosePlotBoundsFromReferenceSurface=FALSE;
  plotSurfaceGrids=true;
  plotVolumeGrids=true;
  plotBoundaryConditionMappings=TRUE;
  plotBoundaryCurves=true;
  plotNonPhysicalBoundaries=true;
  plotGhostPoints=false;
  numberOfGhostLinesToPlot=1;
  plotBlockBoundaries=true;
  plotGridLines=true;

  numberOfSurfaceGrids=0;
  numberOfVolumeGrids=0;
  numberOfBoxGrids=0;

  // We store the surface and volume grids in CompositeSurface's so we can easily replot them
  // CompositeSurface surfaceGrids, volumeGrids;
  
  // kkc go through mapInfo and add all the volume grids to volumeGrids
  bool addedSomeVolumeGrids=false;
  bool addedSomeSurfaceGrids=false;
  for ( int m=0; m<mapInfo.mappingList.getLength(); m++ )
  {
    if ( mapInfo.mappingList[m].getDomainDimension()==3 )
    {
      cout<<"adding volume grid "<<mapInfo.mappingList[m].getName(Mapping::mappingName)<<endl;
      // mapInfo.mappingList[m].getMapping().incrementReferenceCount();
      volumeGrids.add(mapInfo.mappingList[m].getMapping());
      volumeGrids.setColour(numberOfVolumeGrids,gi.getColourName(numberOfVolumeGrids));
      numberOfVolumeGrids++;
      addedSomeVolumeGrids = true;
    }
    if( mapInfo.mappingList[m].getDomainDimension()==2 && mapInfo.mappingList[m].getRangeDimension()==3 &&
        mapInfo.mappingList[m].getClassName()=="DataPointMapping" )
    {
      cout<<"adding surface grid "<<mapInfo.mappingList[m].getName(Mapping::mappingName)<<endl;
      // mapInfo.mappingList[m].getMapping().incrementReferenceCount();
      surfaceGrids.add(mapInfo.mappingList[m].getMapping());
      numberOfSurfaceGrids++;
      addedSomeSurfaceGrids = true;
    }
    
  }
  
  aString answer,line,answer2; 

  bool plotObject=TRUE;

  bool mappingChosen = (surface!=NULL);

  bool surfaceGrid;
  bool newSurface;
  if(surface && surface->getClassName()=="CompositeSurface" )
  {
    surface->uncountedReferencesMayExist();
    surface->incrementReferenceCount();
    newSurface=true;
    surfaceGrid=true;
  }
  else
  {
    newSurface=false;
    surfaceGrid=false;
  }
  
  
  // By default transform the last mapping in the list (if this mapping is uninitialized, mappingChosen==FALSE)
  if( !mappingChosen )
  {
    int number= mapInfo.mappingList.getLength();
    for( int i=number-1; i>=0; i-- )
    {
      Mapping *mapPointer=mapInfo.mappingList[i].mapPointer;
      if( (mapPointer->getDomainDimension()==2 && mapPointer->getRangeDimension()==3) ||
          (mapPointer->getDomainDimension()==1 && mapPointer->getRangeDimension()==2) )
      {
        surface=mapPointer;   // use this one
	surface->uncountedReferencesMayExist();
	surface->incrementReferenceCount();

        mappingChosen=TRUE;
        newSurface=true;
      
        if( surface->getClassName()=="CompositeSurface" )
          surfaceGrid=true;

	break; 
      }
    }
  }
  if( !mappingChosen && !addedSomeVolumeGrids && !addedSomeSurfaceGrids )
  {
    cout << "MappingBuilder:ERROR: there are no mappings that can be used!! \n";
    cout << "A mapping should have domainDimension==rangeDimension-1   \n";
    return 1;
  }

  pSurface=surface;

  aString menu[] = 
  {
    "!MappingBuilder",
//    "start from which curve/surface?",
//    "build a new hyperbolic mapping",
    "exit", 
    "" 
  };

  GUIState gui;
  gui.setWindowTitle("Mapping Builder");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = gui;


  dialog.addInfoLabel("Total points on surface grids: 0.");
  dialog.addInfoLabel("Total points on volume grids : 0.");

  const int num=mapInfo.mappingList.getLength();
  aString *label = new aString[num+2];

  dialog.setOptionMenuColumns(1);

  const int maxCommands= max(20,num+2);
  aString *cmd = new aString [maxCommands];
  int j=0;
  int currentStartingCurve = 0;
  
  for( int i=0; i<num; i++ )
  {
    MappingRC & map = mapInfo.mappingList[i];
    if( ( (map.getDomainDimension()==2 && map.getRangeDimension()==3) ||
	  (map.getDomainDimension()==1 && map.getRangeDimension()==2) ) )
    {
      label[j]=map.getName(Mapping::mappingName);
      cmd[j]="Start curve:"+label[j];
      if (&(map.getMapping()) == surface)
	currentStartingCurve = j;
      
      j++;
    }
  }
  if ( j==0 )
    {
      label[j] = cmd[j] = "-- none --";
      j++;
    }
  label[j]=""; cmd[j]="";   // null string terminates the menu
  const int numberOfPossibleStartingCurves=j;

  // addPrefix(label,prefix,cmd,maxCommands);

  dialog.addOptionMenu("Start from:", cmd,label,currentStartingCurve);
  delete [] label;

  aString opLabel[] = {
                    // "choose starting curve",
                       "choose surface grid",
                       "nothing",""}; //
  /// addPrefix(label,prefix,cmd,maxCommands);
  dialog.addOptionMenu("selection:", opLabel,opLabel,0);

  aString opLabel1[] = {"none",""}; //
  /// addPrefix(label,prefix,cmd,maxCommands);
  dialog.addOptionMenu("active grid:", opLabel1,opLabel1,0);

//     aString opLabel2[] = {"volume grid","surface grid",""}; //
//     // addPrefix(label,prefix,cmd,maxCommands);
//     dialog.addOptionMenu("Type:", opLabel2,opLabel2,surfaceGrid==true);

//     aString opLabel3[] = {"choose a surface grid","off",""}; //
//     GUIState::addPrefix(opLabel3,"picking:",cmd,maxCommands);
//     pickingOption=pickToChooseInitialCurve;
//     dialog.addOptionMenu("Picking:", cmd,opLabel3,pickingOption);
//     dialog.getOptionMenu(3).setSensitive(surfaceGrid==true);

  aString pbLabels[] = {"create surface grid...",
			"create volume grid...",
                        "build surface patch from curves...",
                        "change a grid",
                        "build a box grid",
                        "edit reference surface",
                        "assign BC and share values",
                        "build curve on surface",
                        "save grids to a file...",
                        "edit intersection curve",
                        "delete mapping",
                        "add surface grid",
                        "add volume grid",
//                        "step",
//                        "step all",
			""};
  // addPrefix(pbLabels,prefix,cmd,maxCommands);
  int numRows=7;
  dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

  aString tbCommands[] = {"plot reference surface",
			  "plot shaded on reference surface",
			  "plot lines on reference surface",
                          "plot surface grids",
                          "plot volume grids",
			  "plot lines on non-physical",
                          "plot ghost points",
                          "plot block boundaries",
                          "plot grid lines",
                          ""};
  int tbState[9];
  tbState[0] = plotReferenceSurface==true; 
  tbState[1] = 1; 
  tbState[2] = 0; 
  tbState[3] = plotSurfaceGrids;
  tbState[4] = plotVolumeGrids;
  tbState[5] = plotNonPhysicalBoundaries==true; 
  tbState[6] = plotGhostPoints==true;
  tbState[7] = plotBlockBoundaries==true;
  tbState[8] = plotGridLines==true;

  int numColumns=2;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

//     dialog.setSensitive(surfaceGrid==true,DialogData::pushButtonWidget,3);

  bcPlotOption=colourBoundariesByGridNumber;
  
  aString bcPlotOptionCommand[] = { "colour boundaries by grid number",
				    "colour boundaries by BC number",
				    "colour boundaries by share number",
				    "" };
  dialog.addRadioBox("volume grids boundary colour:",bcPlotOptionCommand, bcPlotOptionCommand, 
                     (int)colourBoundariesByGridNumber);
    

  const int numberOfTextStrings=5;
  aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

  aString referenceSurfaceColour="default";

  int nt=0;
  textLabels[nt] = "surface colour"; 
  sPrintF(textStrings[nt], "%s",(const char*)referenceSurfaceColour); nt++; 
//  textLabels[nt] = "Starting curve bounds"; 
//  sPrintF(textStrings[nt], "%g, %g",0.,1.); nt++; 

  textLabels[nt] = "target grid spacing"; 
  sPrintF(textStrings[nt], "%g, %g (tang,norm)((<0 : use default)",targetGridSpacing[0],targetGridSpacing[1]); nt++; 

  textLabels[nt] = "ghost lines to plot:"; 
  sPrintF(textStrings[nt], "%i",numberOfGhostLinesToPlot); nt++; 

  // addPrefix(textLabels,prefix,cmd,maxCommands);
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  gui.buildPopup(menu);

  delete [] cmd;


  gi.pushGUI(gui);
  gi.appendToTheDefaultPrompt("builder>");  

  // GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

  // GraphicsParameters referenceSurfaceParameters;
  referenceSurfaceParameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

  referenceSurfaceParameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,true);
  referenceSurfaceParameters.set(GI_PLOT_UNS_FACES,true);

  referenceSurfaceParameters.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,false);
  referenceSurfaceParameters.set(GI_PLOT_UNS_EDGES,false);

  SelectionInfo select; select.nSelect=0;
   //  PickInfo3D pick;  pick.active=0;
  int len;
  
  numberOfBoundaryCurves=0;
  boundaryCurves=NULL;
  int activeSurfaceGrid=0;
  int activeVolumeGrid=0;

  if( extraBoundaryCurve==NULL )
  {
    numberOfExtraBoundaryCurves=0;
    maxNumberOfExtraBoundaryCurves=100;
    extraBoundaryCurve = new Mapping *[maxNumberOfExtraBoundaryCurves];
  }
  else
  {
    assert( maxNumberOfExtraBoundaryCurves>0 );
  }
  
  for(int it=0; ; it++)
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
    {
      gi.getAnswer(answer,"", select);
    }

    printf("MappingBuilder: answer=[%s]\n",(const char*)answer);


    if( select.nSelect )
    {
      printf("Selection \n");
      int curveFound=-1;
      
      for (int i=0; i<select.nSelect && curveFound<0; i++)
      {
	printf("i=%i, ID=%i, minZ=%i, maxZ=%i\n", i,select.selection(i,0),
	       select.selection(i,1),select.selection(i,2));
	for( int b=0; b<numberOfBoundaryCurves; b++ )
	{
	  if( boundaryCurves[b]->getGlobalID()==select.selection(i,0) )
	  {
	    printf("Boundary curve %i selected\n",b);
	    // mapPointer=boundaryCurves[b];
	    curveFound=b;
            break;
	  }
	}
      }
      if( curveFound>=0 )
      {
        // build a new HyperbolicMapping for the start curve -- check if there is already one there ?

	HyperbolicMapping & hyp = *new HyperbolicMapping(); 
	Mapping *mapPointer=&hyp;  mapPointer->incrementReferenceCount();
	mapInfo.mappingList.addElement(*mapPointer);

	hyp.setName(Mapping::mappingName,surface->getName(Mapping::mappingName)+
		    sPrintF(line,"-surface%i",numberOfSurfaceGrids+1));
	hyp.setSurface( *surface );
        Mapping & startingCurve = *boundaryCurves[curveFound];
	hyp.setStartingCurve( startingCurve );
        real estimatedDistanceToMarch; 
        int estimatedLinesToMarch;
	
	hyp.estimateMarchingParameters( estimatedDistanceToMarch, estimatedLinesToMarch,0,gi );
	// set hyperbolic marching parameters
	IntegerArray ipar(5);
	RealArray rpar(5);

	ipar(0)=0; // region number
	ipar(1)=estimatedLinesToMarch;
	hyp.setParameters(HyperbolicMapping::linesInTheNormalDirection,ipar,rpar);

	ipar(0)=0; // region number
	rpar(0)=estimatedDistanceToMarch;
	hyp.setParameters(HyperbolicMapping::distanceToMarch,ipar,rpar);
	
	if( surface->getClassName()=="UnstructuredMapping" )
	{
	  if( startingCurve.getIsPeriodic(axis1)==Mapping::functionPeriodic )
	  {
	    ipar(0)=HyperbolicMapping::periodic;
	    ipar(1)=HyperbolicMapping::periodic;
	  }
	  else
	  {
	    ipar(0)=HyperbolicMapping::matchToABoundaryCurve;
	    ipar(1)=HyperbolicMapping::matchToABoundaryCurve;
	  }
          ipar(2)=1; ipar(3)=1;
          hyp.setParameters(HyperbolicMapping::THEboundaryConditions,ipar,rpar);

          ipar=0;
	  hyp.setParameters(HyperbolicMapping::projectGhostBoundaries,ipar,rpar);

          ipar=1;
	  hyp.setParameters(HyperbolicMapping::growInTheReverseDirection,ipar,rpar);
    
	}
	// hyp.display();
	hyp.setBoundaryCurves( numberOfBoundaryCurves,boundaryCurves );
	
	hyp.generateNew();

	// hyp.display();
        surfaceGrids.add(hyp);
	numberOfSurfaceGrids++;

        updateActiveGridMenu( mapInfo,dialog,surfaceGrids,numberOfSurfaceGrids-1 );
      }
      
    }
    else if( len=answer.matches("Start curve:") )
    {
      aString name=answer(len,answer.length()-1);
      const int num=mapInfo.mappingList.getLength();
      for( int i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( ( (map.getDomainDimension()==2 && map.getRangeDimension()==3) ||
	      (map.getDomainDimension()==1 && map.getRangeDimension()==2) ) )
	{
	  if( name==map.getName(Mapping::mappingName) )
	  {
	    if( surface!=0 && surface->decrementReferenceCount()==0 ) 
	      delete surface;
	    surface=mapInfo.mappingList[i].mapPointer;

            printF(" New reference surface is %s\n",(const char*)surface->getName(Mapping::mappingName));
	    
	    surface->incrementReferenceCount();
	    pSurface=surface;
	    
            // *wdh* 2012/11/28 -- set the active surface grid 
	    for(int i=0; i<numberOfSurfaceGrids; i++ )
	    {
	      if( name==surfaceGrids[i].getName(Mapping::mappingName) )
	      {
		activeSurfaceGrid=i;
		printF("Setting the active surface grid to number %i, name=%s.\n",activeSurfaceGrid,
		       (const char*)surfaceGrids[i].getName(Mapping::mappingName));
		break;
	      }
	    }

            plotObject=true;
            newSurface=true;
            break;
	  }
	}
      }
      
    }
    else if( answer=="create surface grid..." )
    {
      HyperbolicMapping & hyp = *new HyperbolicMapping(); 
      Mapping *mapPointer=&hyp;                     mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);

      hyp.setName(Mapping::mappingName,surface->getName(Mapping::mappingName)+
                  sPrintF(line,"-surface%i",numberOfSurfaceGrids+1));
      hyp.setSurface( *surface );

      hyp.addBoundaryCurves( numberOfExtraBoundaryCurves,extraBoundaryCurve );

      // for surface grids both tangential and marching ds values are defined by targetGridSpacing[0]
      hyp.setParameters(HyperbolicMapping::THEtargetGridSpacing,targetGridSpacing[0]);
      hyp.setParameters(HyperbolicMapping::THEinitialGridSpacing,targetGridSpacing[0]);
      if( targetGridSpacing[0]>0. )
        hyp.setParameters(HyperbolicMapping::THEspacingOption,(int)HyperbolicMapping::distanceFromLinesAndSpacing);
      
      
      mapPointer->update(mapInfo); mapPointer->decrementReferenceCount();
       
      activeSurfaceGrid=numberOfSurfaceGrids;  // make this new surface the active one
      surfaceGrids.add(hyp);
      numberOfSurfaceGrids++;
      
      updateActiveGridMenu( mapInfo,dialog,surfaceGrids,numberOfSurfaceGrids-1 );

    }
    else if( answer=="create volume grid..." )
    {
      if( numberOfSurfaceGrids==0 )
      {
	printf("You must build a surface grid before you can build a volume grid\n");
      }
      else
      {
	HyperbolicMapping & hyp = *new HyperbolicMapping(); 
	Mapping *mapPointer=&hyp;                     mapPointer->incrementReferenceCount();
	mapInfo.mappingList.addElement(*mapPointer);

	hyp.setName(Mapping::mappingName,surface->getName(Mapping::mappingName)+
		    sPrintF(line,"-volume%i",numberOfVolumeGrids+1));

        Mapping & activeSurface = surfaceGrids[activeSurfaceGrid];
	hyp.setSurface( activeSurface,false );

        hyp.setPlotOption(HyperbolicMapping::setPlotBoundsFromGlobalBounds,true);  // keep the same view.
	
	hyp.setParameters(HyperbolicMapping::THEinitialGridSpacing,targetGridSpacing[1]);
        if( targetGridSpacing[1]>0. )
          hyp.setParameters(HyperbolicMapping::THEspacingOption,(int)HyperbolicMapping::distanceFromLinesAndSpacing);

        // set default BC for volume grids to outward splay
        IntegerArray ipar(6);
	ipar=HyperbolicMapping::outwardSplay;
	for( int axis=0; axis<=1; axis++ )
	{ // *wdh* 2013/08/10 -- set periodic BC's 
	  if( activeSurface.getIsPeriodic(axis)==Mapping::functionPeriodic )
	  {
	    for( int side=0; side<=1; side++ )
	      ipar(side+2*axis)=HyperbolicMapping::periodic;
	  }
	}
	
	hyp.setParameters(HyperbolicMapping::THEboundaryConditions,ipar);

	mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
       
	volumeGrids.add(hyp);
        volumeGrids.setColour(numberOfVolumeGrids,gi.getColourName(numberOfVolumeGrids));
	numberOfVolumeGrids++;
	
      }
    }
    else if( answer=="build surface patch from curves..." )
    {
      buildSurfacePatch(mapInfo);

      dialog.setToggleState("plot reference surface",plotReferenceSurface);
      updateActiveGridMenu( mapInfo,dialog,surfaceGrids,numberOfSurfaceGrids-1 );
    }
    else if( len=answer.matches("surface colour") )
    {
      referenceSurfaceColour=answer(len+1,answer.length()-1);
      cout << "answer=[" << answer << "]" << endl;
      cout << "referenceSurfaceColour=[" << referenceSurfaceColour << "]" << endl;
      
      if( pSurface!=NULL )
      {
	if( referenceSurfaceColour=="default" )
	{
	  // The default is multi-coloured
	  if( pSurface->getClassName()=="CompositeSurface" )
	  {
	    CompositeSurface & cs = (CompositeSurface&)(*pSurface);
	    for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
	    {
	      cs.setColour(s,gi.getColourName(s));
	    }
	    cs.eraseCompositeSurface(gi);
	  }
	}
	else
	{
	  if( pSurface->getClassName()=="CompositeSurface" )
	  {
	    CompositeSurface & cs = (CompositeSurface&)(*pSurface);
	    for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
	    {
	      cs.setColour(s,referenceSurfaceColour);
	    }
	    cs.eraseCompositeSurface(gi);
	  }
	}
	plotObject=true;      
      }
      else
      {
        printf("INFO:Sorry, there is no reference surface to set the colour for\n");
      }
    }
    else if( len=answer.matches("target grid spacing") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e",&targetGridSpacing[0],&targetGridSpacing[1]);
      dialog.setTextLabel("target grid spacing",sPrintF(answer,"%g, %g (tang,norm)(<0 : use default)",targetGridSpacing[0],
                       targetGridSpacing[1] ));
      
    }
    else if( dialog.getTextValue(answer,"ghost lines to plot:","%i",numberOfGhostLinesToPlot) )
    {
      printF("INFO: number of ghost lines to plot = %i. \n",numberOfGhostLinesToPlot);
      surfaceGrids.eraseCompositeSurface(gi);
      volumeGrids.eraseCompositeSurface(gi);
      plotObject=true;
    }
    else if( answer=="build a box grid" )
    {
        
      // ============ make this a separate function ===============
      buildBoxGrid( mapInfo );
      
    }
    else if( answer=="step" )
    {


    }
    else if( answer=="assign BC and share values" )
    {

      // plot boundaries by BC by default
      if( parameters.getBoundaryColourOption()!=GraphicsParameters::colourByBoundaryCondition )
      {
	parameters.getBoundaryColourOption()=GraphicsParameters::colourByBoundaryCondition;
	bcPlotOption=colourBoundariesByBCNumber;
	dialog.getRadioBox(0).setCurrentChoice((int)bcPlotOption );
        volumeGrids.eraseCompositeSurface(gi);
      }
      
      assignBoundaryConditions( mapInfo );

    }
    else if( len=answer.matches("plot reference surface") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value); plotReferenceSurface=value;
      dialog.setToggleState("plot reference surface",plotReferenceSurface);
    }
    else if( len=answer.matches("plot shaded on reference surface") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value);
      dialog.setToggleState("plot shaded on reference surface",value);
      referenceSurfaceParameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,value);
      referenceSurfaceParameters.set(GI_PLOT_UNS_FACES,value);
    }
    else if( len=answer.matches("plot lines on reference surface") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value); 
      dialog.setToggleState("plot lines on reference surface",value);
      referenceSurfaceParameters.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,value);
      referenceSurfaceParameters.set(GI_PLOT_UNS_EDGES,value);
    }
    else if( len=answer.matches("plot surface grids") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value);
      dialog.setToggleState("plot surface grids",value);
      plotSurfaceGrids=value;
    }
    else if( len=answer.matches("plot volume grids") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value);
      dialog.setToggleState("plot volume grids",value);
      plotVolumeGrids=value;
    }
    else if( len=answer.matches("plot lines on non-physical") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value); plotNonPhysicalBoundaries=value;
      dialog.setToggleState("plot lines on non-physical",plotNonPhysicalBoundaries==true);       

      volumeGrids.eraseCompositeSurface(gi);
      plotObject=true;
    }
    else if( len=answer.matches("plot ghost points") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value); plotGhostPoints=value;
      dialog.setToggleState("plot ghost points",plotGhostPoints);
      surfaceGrids.eraseCompositeSurface(gi);
      volumeGrids.eraseCompositeSurface(gi);
      plotObject=true;
    }
    else if( len=answer.matches("plot block boundaries") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value); plotBlockBoundaries=value;
      dialog.setToggleState("plot block bounadries",plotBlockBoundaries);
      volumeGrids.eraseCompositeSurface(gi);
      plotObject=true;
    }
    else if( len=answer.matches("plot grid lines") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value); plotGridLines=value;
      dialog.setToggleState("plot grid lines",plotGridLines);
      surfaceGrids.eraseCompositeSurface(gi);
      volumeGrids.eraseCompositeSurface(gi);
      plotObject=true;
    }
    else if( answer=="edit reference surface" )
    {
      if( surface!=0 )
      {
	surface->update(mapInfo);
	plotObject=true;
      }
      else
      {
	printf("MappingBuilder:ERROR:There is no reference surface to edit\n");
      }
    }
    else if( answer=="colour boundaries by BC number" )
    {
      bcPlotOption=colourBoundariesByBCNumber;
      dialog.getRadioBox(0).setCurrentChoice((int)bcPlotOption );

      parameters.getBoundaryColourOption()=GraphicsParameters::colourByBoundaryCondition;
      volumeGrids.eraseCompositeSurface(gi);
      
      plotObject=true;
    }
    else if( answer=="colour boundaries by share number" )
    {
      bcPlotOption=colourBoundariesByShareNumber;
      dialog.getRadioBox(0).setCurrentChoice((int)bcPlotOption );

      parameters.getBoundaryColourOption()=GraphicsParameters::colourByShare;
      volumeGrids.eraseCompositeSurface(gi);
      plotObject=true;
    }
    else if( answer=="colour boundaries by grid number" )
    {
      bcPlotOption=colourBoundariesByGridNumber;
      dialog.getRadioBox(0).setCurrentChoice((int)bcPlotOption );

      parameters.getBoundaryColourOption()=GraphicsParameters::colourByGrid;
      volumeGrids.eraseCompositeSurface(gi);
      plotObject=true;
    }
    else if( answer=="build curve on surface" )
    {
      buildCurveOnSurface(mapInfo);
      plotObject=true;

/* ----
      // first determine a global bounding box.
      Mapping & surf = *surface;

      RealArray boundingBox(2,3);
      real scale=0.;
      int axis;
      for( axis=0; axis<surf.getRangeDimension(); axis++ )
      {
	if( !surf.getRangeBound(Start,axis).isFinite() || !surf.getRangeBound(End,axis).isFinite() )
	{
	  printf("*** WARNING: rangeBound not finite! axis=%i [%e,%e]\n",axis,
		 (real)surf.getRangeBound(Start,axis),(real)surf.getRangeBound(End,axis) );
	  surf.getGrid();
	}
	boundingBox(Start,axis)=surf.getRangeBound(Start,axis);
	boundingBox(End  ,axis)=surf.getRangeBound(End,axis);
    
        scale=max(scale,boundingBox(End  ,axis)-boundingBox(Start,axis));
	
        // scale *=1.5;
	
	// printf(" ***Surface:  xa=%e xb=%e scale=%e\n",boundingBox(Start,axis),boundingBox(End  ,axis),scale);
      }
      

      real x0=0., y0=0., z0=.5;
      real nx=0., ny=1., nz=0.;
      printf("build a curve on the reference surface by intersecting the reference surface with a plane\n");
      printf("The bounding box is [%8.2e,%8.2e]x[%8.2e,%8.2e]x[%8.2e,%8.2e]\n",
	     boundingBox(0,0),boundingBox(1,0),boundingBox(0,1),boundingBox(1,1),boundingBox(0,2),boundingBox(1,2));
      
      gi.inputString(answer,"Enter a point on the plane, x,y,z");
      sScanF(answer,"%e %e %e",&x0,&y0,&z0);
      gi.inputString(answer,"Enter the normal to the plane, nx,ny,nz");
      sScanF(answer,"%e %e %e",&nx,&ny,&nz);

      real norm = max(REAL_MIN,sqrt(nx*nx+ny*ny+nz*nz));
      nx/=norm;
      ny/=norm;
      nz/=norm;
      
      // find tangents to the plane
      real ta[3], tb[3];
      if( fabs(nx)>=max(fabs(ny),fabs(nz)) )
      {
	ta[0]=-ny, ta[1]=nx, ta[2]=0.;
      }
      else if( fabs(ny)>=fabs(nz) )
      {
	ta[0]=0., ta[1]=-nz, ta[2]=ny;
      }
      else
      {
	ta[0]=nz, ta[1]=0, ta[2]=-nx;
      }
      norm=max(REAL_MIN,sqrt(ta[0]*ta[0]+ta[1]*ta[1]+ta[2]*ta[2]));
      ta[0]/=norm;
      ta[1]/=norm;
      ta[2]/=norm;
      
      tb[0]=ny*ta[2]-nz*ta[1];
      tb[1]=nz*ta[0]-nx*ta[2];
      tb[2]=nx*ta[1]-ny*ta[0];
      

      // plane(r,s) = x0 + r*t0 + s*t1
      // a point is on the plane if n.(x-x0)=0

      // build a PlaneMapping large enough to cut the surface

      
      real x1=0., y1=0., z1=z0, 
           x2=1., y2=0., z2=z0, 
           x3=0., y3=1., z3=z0;
      
      real r=-scale;
      real s=-scale;
      x1=x0+r*ta[0]+s*tb[0];
      y1=y0+r*ta[1]+s*tb[1];
      z1=z0+r*ta[2]+s*tb[2];
      
      r=scale;
      x2=x0+r*ta[0]+s*tb[0];
      y2=y0+r*ta[1]+s*tb[1];
      z2=z0+r*ta[2]+s*tb[2];
      
      r=-scale;
      s=scale;
      x3=x0+r*ta[0]+s*tb[0];
      y3=y0+r*ta[1]+s*tb[1];
      z3=z0+r*ta[2]+s*tb[2];

      PlaneMapping plane(x1, y1, z1, x2, y2, z2, x3, y3, z3);
      plane.setGridDimensions(axis1,21);
      plane.setGridDimensions(axis1,21);
      
      // Intersect the Plane Mapping with the CompositeSurface

      IntersectionMapping intersect;
      bool isCompositeSurface=surface->getClassName()=="CompositeSurface";
      
      bool plotPlane=true;
      if( plotPlane )
      {
        PlotIt::plot(gi,plane,parameters);
	gi.pause();
      }

      bool success=false;
      if( isCompositeSurface )
      {
        CompositeSurface & cs = (CompositeSurface&)(*surface);
        success= intersect.intersect(plane,cs)==0;
      }
      else
        success= intersect.intersect(*surface,plane)==0;

      for( axis=0; axis<surf.getRangeDimension(); axis++ )
      {
	 printf(" After: surface:  axis=%i xa=%e xb=%e\n",axis,boundingBox(Start,axis),boundingBox(End  ,axis));
      }
      
      if( success && intersect.curve!=NULL )
      {
	Mapping & curve = *intersect.curve;  // intersection curve in physical space.

	if( plotPlane )
	{
	  real oldCurveLineWidth;
	  parameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
	  parameters.set(GraphicsParameters::curveLineWidth,3.);

	  parameters.set(GI_MAPPING_COLOUR,"green");
	  PlotIt::plot(gi,curve,parameters);
	  
	  parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
	}
	
        NurbsMapping & nurb = (NurbsMapping&)curve;
        printf(" Creating %i new boundary curves from the curves of intersection.\n",nurb.numberOfSubCurves());

        if( numberOfExtraBoundaryCurves+nurb.numberOfSubCurves() >=maxNumberOfExtraBoundaryCurves )
	{
          printf("Sorry, too many boundary curves. Get Bill to fix this\n");
	}
	else
	{
          for( int sc=0; sc<nurb.numberOfSubCurves(); sc++ )
	  {
	    extraBoundaryCurve[numberOfExtraBoundaryCurves]=&nurb.subCurve(sc);
	    extraBoundaryCurve[numberOfExtraBoundaryCurves]->incrementReferenceCount();
	    numberOfExtraBoundaryCurves++;
	  }
	}
      }
      else
      {
	printf("\n ****No intersecting curves were found ****\n");
      }
      if( false )
      {
	gi.pause(); // inputString(answer,"Enter a char to continue");
	gi.erase();
	extraBoundaryCurve[numberOfExtraBoundaryCurves-1]->update(mapInfo);
      }
      plotObject=true;
    -------------- */


    }
    else if( answer=="edit intersection curve" )
    {
      if( numberOfExtraBoundaryCurves>0 )
      {
        for( int n=0; n<numberOfExtraBoundaryCurves; n++ )
	{
	  printf("edit extra boundary curve %i\n",n);
	  gi.erase();
	  extraBoundaryCurve[n]->update(mapInfo);
	}
	
      }
    }
    else if( answer=="change a grid" )
    {
      DialogData gridDialog;
      gridDialog.setWindowTitle("Change a Grid");
      gridDialog.setExitCommand("exit", "continue");

      int num=numberOfSurfaceGrids;
      aString *label = new aString [num+1];
      int i;
      for(i=0; i<numberOfSurfaceGrids; i++ )
      {
	// Mapping& map =mapInfo.mappingList[surfaceGridNumber(i)].getMapping();
        Mapping& map =surfaceGrids[i];
	label[i]=map.getName(Mapping::mappingName);
      }
      label[num]="";
      
      
      gridDialog.addOptionMenu("Change:", label,label,0);

      gridDialog.openDialog();

      gi.getAnswer(answer,"" );
      int gridToChange=-1;
      if( answer!="exit" )
      {
	for(i=0; i<numberOfSurfaceGrids; i++ )
	{
	  if( answer==label[i] )
	  {
            gridToChange=i;
            break;
	  }
	}
      }
      gridDialog.closeDialog();
      delete [] label;

      if( gridToChange>=0 && gridToChange<numberOfSurfaceGrids )
      {
	    
        surfaceGrids.eraseCompositeSurface(gi,gridToChange);

	Mapping & map =surfaceGrids[gridToChange];
	map.update(mapInfo);
      }
      else if( answer!="exit" )
      {
	printf("ERROR: unknown response = [%s]\n",(const char*)answer);
      }
    }
    else if( len=answer.matches("Starting curve bounds") )
    {
      real ra,rb;
      sScanF(answer(len,answer.length()-1),"%e %e",&ra,&rb);
      dialog.setTextLabel(0,sPrintF(line, "%g, %g",ra,rb));
      if( activeSurfaceGrid>=0 && activeSurfaceGrid<numberOfSurfaceGrids )
      {
        
      }
      
    }
    else if( len=answer.matches("active grid:") )
    {
      aString name = answer(len,answer.length());
      int i;
      bool found=false;
      for( i=0; i<numberOfSurfaceGrids; i++ )
      {
        if( name==surfaceGrids[i].getName(Mapping::mappingName) )
	{
	  activeSurfaceGrid=i;
          found=true;
	  break;
	}
      }
      if( !found )
      {
	for( i=0; i<numberOfVolumeGrids; i++ )
	{
	  // if( name==mapInfo.mappingList[volumeGridNumber(i)].getName(Mapping::mappingName) )
	  if( name==volumeGrids[i].getName(Mapping::mappingName) )
	  {
	    activeVolumeGrid=i;
	    found=true;
	    break;
	  }
	}
      }
      if( !found )
      {
	printf("ERROR searching for the active grid, name=[%s]\n",(const char*)name);
      }
      printf(" activeSurfaceGrid=%i, activeVolumeGrid=%i\n",activeSurfaceGrid,activeVolumeGrid);
      
      // assign activeSurfaceGrid      
    }
    else if( answer=="save grids to a file..." )
    {
      bool saveVolumeGrids=true;
      bool saveSurfaceGrids=false;
      aString fileName = "model.hdf";
      

      GUIState gui;
      gui.setWindowTitle("File output");
      gui.setExitCommand("exit", "exit");
      DialogData & dialog = gui;      

      
      aString pbLabels[] = {"save file",
			    ""};
      // addPrefix(pbLabels,prefix,cmd,maxCommands);
      int numRows=1;
      dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

      aString tbCommands[] = {"save volume grids",
			      "save surface grids",
			      ""};
      int tbState[6];
      tbState[0] = saveVolumeGrids==true; 
      tbState[1] = saveSurfaceGrids==true; 
      int numColumns=1;
      dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

      const int numberOfTextStrings=5;
      aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

      int nt=0;
      textLabels[nt] = "file name:"; 
      textStrings[nt]=fileName;

      dialog.setTextBoxes(textLabels, textLabels, textStrings);

      gi.pushGUI(gui);

      for( ;; )
      {
	gi.getAnswer(answer,"");
	if( answer=="exit" )
	{
	  break;
	}
	else if( len=answer.matches("save volume grids") )
	{
          int value;
	  sScanF(answer(len,answer.length()-1),"%i",&value);
	  saveVolumeGrids=value;
	}
	else if( len=answer.matches("save surface grids") )
	{
          int value;
	  sScanF(answer(len,answer.length()-1),"%i",&value);
	  saveSurfaceGrids=value;
	}
	else if( len=answer.matches("file name: ") )
	{
	  fileName=answer(len,answer.length()-1);
          dialog.setTextLabel("file name:",fileName);
	}
	else if( answer=="save file" )
	{
          HDF_DataBase db;
          db.mount(fileName,"I");

          int grid;
	  if( saveSurfaceGrids )
	  {
	    for( grid=0; grid<numberOfSurfaceGrids; grid++ )
	    {
	      Mapping map(surfaceGrids[grid]);
	      printf("Saving surface grid %s\n",(const char*)map.getName(Mapping::mappingName));
	      map.put(db,map.getName(Mapping::mappingName));  // put the mapping
	    }
	  }
	  if( saveVolumeGrids )
	  {
	    for( grid=0; grid<numberOfVolumeGrids; grid++ )
	    {
	      // MappingRC & map = mapInfo.mappingList[volumeGridNumber(grid)];
	      MappingRC map(volumeGrids[grid]);
	      printf("Saving volume grid %s\n",(const char*)map.getName(Mapping::mappingName));
	      map.put(db,map.getName(Mapping::mappingName));  // put the mapping
	    }
	  }
          db.unmount();
          printF("Wrote file %s\n",(const char*)fileName);
          printF("To read this file back in use the 'open a data-base' command from\n"
                 "the 'create mappings' menu\n");
	  
	}
      }
      gi.popGUI();

    }
    else if( answer=="add surface grid" ||
             answer=="add volume grid" )
    { // finish me 
      bool addSurfaceGrid = answer=="add surface grid";
      if( addSurfaceGrid )
        printF("Add an existing mapping to the list of surface grids\n");
      else
        printF("Add an existing mapping to the list of volume grids\n");


      int num=mapInfo.mappingList.getLength();
      if( num<=0 )
      {
	printF("WARNING: There are no %s grid mappings available! \n",
             (addSurfaceGrid ? "surface" : "volume"));
        gi.stopReadingCommandFile();
	continue;
      }

      int domainDim = addSurfaceGrid ? 2 : 3;
      aString *menu2 = new aString[num+3];
      int i=0;
      menu2[i++]="!add a grid";
      int mappingListStart=i;
      for( int j=0; j<num; j++ )
      {
        MappingRC & map =mapInfo.mappingList[j];
	if( map.getDomainDimension()==domainDim && map.getRangeDimension()==3 )
	{
   	   menu2[i++]=mapInfo.mappingList[j].getName(Mapping::mappingName);
	}
	
      }
      int mappingListEnd=i-1;
      menu2[i++]="none"; 
      menu2[i]="";   // null string terminates the menu

      gi.buildCascadingMenu( menu2,mappingListStart,mappingListEnd );

      gi.getMenuItem(menu2,answer2,"add which mapping?");
      delete [] menu2;

      int mapNumber=-1;
      if( answer2!="none" )
      {
	for( int j=0; j<num; j++ )
	{
	  if( answer2==mapInfo.mappingList[j].getName(Mapping::mappingName) )
	  {
	    mapNumber=j;
	    break;
	  }
	}
      }
      if( mapNumber>=0 )
      {
	Mapping & mapToAdd =*mapInfo.mappingList[mapNumber].mapPointer;
        printf("add mapping %s as a %s grid\n",
	       (const char *) mapToAdd.getName(Mapping::mappingName),
	       (addSurfaceGrid ? "surface" : "volume"));
        // mapInfo.mappingList.deleteElement(map);  // delete the mapping

	if( addSurfaceGrid )
	{
	  surfaceGrids.add(mapToAdd);
	  numberOfSurfaceGrids++;        
	}
	else
	{
	  volumeGrids.add(mapToAdd);
	  numberOfVolumeGrids++;        
	}
	
      }
      else if( answer2!="none" )
      {
        printF("MappingBuilder::ERROR: unknown mapping name =[]!",(const char*)answer2);
        gi.stopReadingCommandFile();
      }
      
    }
    else if( answer=="add volume grid" )
    { // finish me
    }
    else if( answer=="delete mapping" )
    {
      int num=mapInfo.mappingList.getLength();
      if( num<=0 )
      {
	printf("There are no mappings available! \n");
	continue;
      }

      aString *menu2 = new aString[num+3];
      int i=0;
      menu2[i++]="!delete mapping";
      int mappingListStart=i;
      for( int j=0; j<num; j++ )
	menu2[i++]=mapInfo.mappingList[j].getName(Mapping::mappingName);
      int mappingListEnd=i-1;
      menu2[i++]="none"; 
      menu2[i]="";   // null string terminates the menu

      gi.buildCascadingMenu( menu2,mappingListStart,mappingListEnd );

      int map = gi.getMenuItem(menu2,answer2,"delete which mapping?");
      delete [] menu2;

      gi.indexInCascadingMenu( map,mappingListStart,mappingListEnd);

      if( map>=0 && answer2!="none" )
      {
	Mapping *mapPointer=mapInfo.mappingList[map].mapPointer;
        printf("delete mapping %s \n",(const char *) mapPointer->getName(Mapping::mappingName));
        mapInfo.mappingList.deleteElement(map);  // delete the mapping

	// we also look for the mapping in the list of surface and volume grids
        const aString & name = mapPointer->getName(Mapping::mappingName);
        if( mapPointer->getDomainDimension()==2 && mapPointer->getRangeDimension()==3 )
	{
	  for( int grid=0; grid<numberOfSurfaceGrids; grid++ )
	  {
	    if( name == surfaceGrids[grid].getName(Mapping::mappingName) )
	    {
	      printF("delete surface grid=%i, name=%s\n",grid,(const char*)name);
	      surfaceGrids.remove(grid);
	      break;
	    }
	  }
	}
	else if( mapPointer->getDomainDimension()==3 && mapPointer->getRangeDimension()==3 )
	{
	  for( int grid=0; grid<numberOfVolumeGrids; grid++ )
	  {
	    if( name == volumeGrids[grid].getName(Mapping::mappingName) )
	    {
	      printF("delete volume grid=%i, name=%s\n",grid,(const char*)name);
	      volumeGrids.remove(grid);
	      break;
	    }
	  }
	}

      }
      else if( map<0 )
      {
        gi.outputString("Error: unknown mapping name!");
        gi.stopReadingCommandFile();
      }
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
    {
    }
    else 
    {
      gi.outputString( sPrintF(line,"Unknown response=%s",(const char*)answer) );
      printf("Unknown response=[%s] \n",(const char*)answer);
      gi.stopReadingCommandFile();
    }

    if( newSurface )
    {
      newSurface=false;
      
      if( surface->getClassName()=="UnstructuredMapping" ) // *** fix for CompositeSurface ***
      {
        for( int b=0; b<numberOfBoundaryCurves; b++ )
	{
	  if( boundaryCurves[b]->decrementReferenceCount()==0 )
	  {
	    delete boundaryCurves[b];
	  }
	}
        delete [] boundaryCurves;
	
	((UnstructuredMapping*)surface)->findBoundaryCurves(numberOfBoundaryCurves, boundaryCurves);
	printf(" ** %i boundary curves found \n",numberOfBoundaryCurves);

      }
    }
    int g,num=0;
    for( g=0; g<volumeGrids.numberOfSubSurfaces(); g++ )
    {
      Mapping & map=volumeGrids[g];
      num=num+map.getGridDimensions(0)*map.getGridDimensions(1)*map.getGridDimensions(2);
    }
    dialog.setInfoLabel(1,sPrintF(line,"Total points on volume grids : %i.",num));
    num=0;
    for( g=0; g<surfaceGrids.numberOfSubSurfaces(); g++ )
    {
      Mapping & map=surfaceGrids[g];
      num=num+map.getGridDimensions(0)*map.getGridDimensions(1);
    }
    dialog.setInfoLabel(0,sPrintF(line,"Total points on surface grids : %i.",num));

    if( plotObject )
    {
      plot(mapInfo);
      
    }
  }
//  gi.erase(); // ** AP don't like the plot to dissapear just because this function is exited

  gi.popGUI();
  gi.unAppendTheDefaultPrompt();

  return returnValue;
}


