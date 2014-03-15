#include <fstream>

#include "rap.h"
#include "MappingInformation.h"
#include "ModelBuilder.h"
#include "CompositeTopology.h"

#include "UnstructuredMapping.h"
#include "SquareMapping.h"
#include "BoxMapping.h"
#include "MappedGrid.h"
#include "CompositeGrid.h"

using namespace std;

namespace {

  void classifyLattice( CompositeSurface &model, MappedGrid &grid )
  {
    grid.update(MappedGrid::THEvertex | MappedGrid::THEmask);

    intArray &mask = (intArray&)grid.mask();
    realArray &vertex = (realArray &)grid.vertex();

    mask = MappedGrid::ISdiscretizationPoint;

    IntegerArray inoutFlag;

    vertex.reshape(vertex.getLength(0)*vertex.getLength(1)*vertex.getLength(2),3);

    model.insideOrOutside( vertex, inoutFlag );

    Range R0(grid.dimension(0,0),grid.dimension(1,0)),
      R1(grid.dimension(0,1),grid.dimension(1,1)),
      R2(grid.dimension(0,2),grid.dimension(1,2)),
      R3(0,2);

    vertex.reshape( R0, R1, R2, R3 );
    inoutFlag.reshape( R0, R1, R2 );
    where( inoutFlag(R0,R1,R2)==0 )
      {
	mask(R0,R1,R2)=0;
      }
  }

}

int main(int argc, char *argv[])
{

  aString commandFileName="";
  bool plotOption=true;
  if( argc > 0 && argc<4 )
  { // look at arguments for "noplot"
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line.matches("noplot") )
        plotOption=false;
      else if( commandFileName=="" )
        commandFileName=line;    
    }
  }
  else
    cout << "Usage: `"<<argv[0]<<" [noplot][file.cmd]' \n"
      "          noplot:   run without graphics \n" 
      "          file.cmd: read this command file \n";

  GenericGraphicsInterface & gi = 
    *Overture::getGraphicsInterface("Lattice Maker",plotOption);

  GraphicsParameters gp,latticeGP;

  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT, TRUE);
  gp.set(GI_PLOT_UNS_BOUNDARY_EDGES,TRUE);
  gp.set(GI_PLOT_UNS_EDGES,TRUE);
  latticeGP.set(GI_PLOT_THE_OBJECT_AND_EXIT, TRUE);

  if ( commandFileName.length() )
    gi.readCommandFile(commandFileName);

  gi.saveEchoFile("cgen.log");
  gi.saveCommandFile("cgen.cmd");
  
  ModelBuilder modelBuilder;
  MappingInformation mapInfo; mapInfo.graphXInterface = &gi; mapInfo.gp_ = &gp;
  CompositeSurface model,deletedSurfaces;
  ListOfMappingRC curveList;
  PointList points;

  BoxMapping box;
  MappedGrid grid((Mapping &)box);
    
  GUIState gui;
  
  gui.setWindowTitle("lattice generator");
  gui.setExitCommand("exit","exit");

  enum pd0Enum {
    importIges,
    loadModel,
    saveModel,
    saveMesh,
    nPD
  };

#if 1
  aString pdCmd0[] = { "Import Iges", "Load Model", "Save Model", "Save Lattice","" };
  gui.addPulldownMenu("File",pdCmd0,pdCmd0, GI_PUSHBUTTON);
  PullDownMenu &fileMenu  = gui.getPulldownMenu(0);
#else

  aString pdCmd0[] =  {"Import Iges", "Load Model", "Save Model",">Save Lattice","Compressed","Full","exit",""};
                                                                                                  
  //gui.buildPopup(pdCmd0);
  gui.addPulldownMenu("File",pdCmd0,pdCmd0,GI_PUSHBUTTON);
  PullDownMenu &fileMenu = gui.getPulldownMenu(0);
#endif

  enum pd1Enum {
    topoSettings,
    latticePlotting,
    surfacePlotting,
    nST
  };

  aString pdCmd1[] = { "Topology", "Lattice Plotting", "Surface Plotting","" };
  gui.addPulldownMenu("Settings",pdCmd1,pdCmd1,GI_PUSHBUTTON);
  PullDownMenu &settingsMenu = gui.getPulldownMenu(1);
  
  enum pbEnum {
    simpleGeometry,
    editModel,
    editBox,
    generateMesh,
    nPB
  };

  aString pbCmd[] = { "Simple Geometry", "Edit Model", "Generate Lattice","" };
  gui.setPushButtons(pbCmd,pbCmd,nPB/2);

  enum tbEnum {
    plotGeomTB,
    plotLatticeTB
  };

  aString tbCmd[] = {"geometry","lattice",""};
  int tbState[] = { 1,0,0};
  gui.setToggleButtons(tbCmd,tbCmd,tbState,1);
  bool plotGeom=true, plotLattice=false;

  enum txtEnum {
    gridSpacing,
    xminmax,
    yminmax,
    zminmax,
    nTXT
  };

  real el=-1; // edge length
  aString txtCmd[] = { "Grid Spacing", ""};//"xmin,xmax","ymin,ymax","zmin,zmax","" };
  aString txtData[] = { "", ""};//"-","-","-", "" };
  sPrintF(txtData[gridSpacing],"%g",el);
  gui.setTextBoxes(txtCmd,txtCmd,txtData);

  RealArray latticeBounds(2,3);
  box.getVertices( latticeBounds(0,0),latticeBounds(1,0),
		   latticeBounds(0,1),latticeBounds(1,1),
		   latticeBounds(0,2),latticeBounds(1,2) );

  int len;
  aString answer,txtAns;

  DialogData &saveGui = gui.getDialogSibling();
                                                                                                  
  saveGui.setWindowTitle("Save Lattice");
  aString sComm0[] = {"Compressed Format", "Image Format", ""};
  aString sComm1[] = {"compressed","image",""};
  saveGui.addRadioBox("Format Type",sComm1,sComm0,0,2);
  saveGui.setExitCommand("close","close");
			  
                                                                                                  
 
  gi.pushGUI(gui);

  bool plotAndExit = true;

  bool boxChanged = false;

  while(1)
    {
      gi.getAnswer(answer,"");

      if ( answer=="exit" )
	break;
      else if(answer==pdCmd0[saveMesh]){
	saveGui.showSibling();
      }
      else if ( gui.getToggleValue(answer, tbCmd[plotGeomTB], plotGeom ) ){}
      else if ( gui.getToggleValue(answer,tbCmd[plotLatticeTB], plotLattice) ){}
//       else if ( (boxChanged=gui.getTextValue(answer, txtCmd[xminmax], "%s", txtAns)) )
// 	{
// 	  sScanF(txtAns,"%g %g",&latticeBounds(0,0),&latticeBounds(1,0));
// 	}
//       else if ( (boxChanged=gui.getTextValue(answer, txtCmd[yminmax], "%s", txtAns)) )
// 	{
// 	  sScanF(txtAns,"%g %g",&latticeBounds(0,1),&latticeBounds(1,1));
// 	}
//       else if ( (boxChanged=gui.getTextValue(answer, txtCmd[zminmax], "%s", txtAns)) )
// 	{
// 	  sScanF(txtAns,"%g %g",&latticeBounds(0,2),&latticeBounds(1,2));
// 	}
      else if ( (boxChanged=gui.getTextValue(answer,txtCmd[gridSpacing],"%g",el)) ){ boxChanged=true; }
      else if ( answer==pdCmd0[importIges] )
	{
	  modelBuilder.newModel(gi,mapInfo,model);
	  sPrintF(txtData[gridSpacing],"%g",el);
	  gui.setTextLabel(gridSpacing,txtData[gridSpacing]);
	  boxChanged=true;
	}
      else if ( answer=="Edit Model" )
	{
	  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
	  gi.erase();
	  PlotIt::plot(gi,model,gp);
	  modelBuilder.editModel(mapInfo,model,deletedSurfaces,curveList,points);
	  sPrintF(txtData[gridSpacing],"%g",el);
	  gui.setTextLabel(gridSpacing,txtData[gridSpacing]);
	  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
	  boxChanged = true;
	}
      else if ( answer=="Simple Geometry" )
	{
	  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

	  gi.erase();
	  PlotIt::plot(gi,model,gp);
	  modelBuilder.simpleGeometry(mapInfo,model,curveList,points);

	  sPrintF(txtData[gridSpacing],"%g",el);
	  gui.setTextLabel(gridSpacing,txtData[gridSpacing]);
	  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
	  boxChanged = true;
	}
      else if ( answer=="Lattice Plotting" )
	{
	  gi.erase();
	  latticeGP.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
	  PlotIt::plot(gi,grid,latticeGP);
	  latticeGP.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
	}
      else if ( answer=="Surface Plotting" )
	{
	  plotAndExit = false;
	}
      else if ( answer==pdCmd1[topoSettings] )
	{
	  model.updateTopology();
	  boxChanged = true;
	}

      if ( boxChanged )
	{
	  model.recomputeBoundingBox();

	  if ( el<0 )
	    {
	      for ( int a=0; a<3; a++ )
		{
		  el = max(el, ((real)model.getRangeBound(1,a)-(real)model.getRangeBound(0,a))/10.);
		}
	    }
		       
	  for ( int a=0; a<3; a++ )
	    for (int s=0; s<2; s++ )
	      {
		latticeBounds(s,a) = (real)model.getRangeBound(s,a) + .5*el*(2*s-1);
		  cout<<"BOUNDS "<<(real)model.getRangeBound(0,a)<<"  "<<(real)model.getRangeBound(1,a)<<endl;
	      }

	  latticeBounds.display("LATTICE BOUNDS");

	  box.setVertices(latticeBounds(0,0),latticeBounds(1,0),
			  latticeBounds(0,1),latticeBounds(1,1),
			  latticeBounds(0,2),latticeBounds(1,2) );
	  for ( int a=0; a<3; a++ )
	    {
	      box.setGridDimensions( a,int((latticeBounds(1,a)-latticeBounds(0,a))/el) + 1 );
	    }

	  SquareMapping sq;
	  grid.setMapping((Mapping &)sq);
	  grid.setMapping((Mapping &)box);

	  for ( int a=0; a<3; a++ )
	    {
	      grid.setBoundaryCondition(0,a,0);
	      grid.setBoundaryCondition(1,a,0);
	    }

	  if ( model.isTopologyDetermined() )
	    classifyLattice(model,grid);

	  boxChanged=false;
	}

      if ( plotAndExit )
	gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      else
	gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);

      gi.erase();

      if ( plotGeom )
	if ( model.isTopologyDetermined() )
	  PlotIt::plot(gi,(Mapping &)*model.getCompositeTopology(true)->getTriangulation(),gp);
	else
	  PlotIt::plot(gi,model,gp);

      if ( plotLattice )
	{
	  PlotIt::plot(gi, grid, latticeGP);
	}

      if ( !plotAndExit )  plotAndExit = true;
    }

  gi.popGUI();
}
