#include "Overture.h"
#include "GL_GraphicsInterface.h"
#include "DataPointMapping.h"
#include "MappingInformation.h"
#include "DataFormats.h"
#include "display.h"
#include "Ogen.h"
#include "ShowFileParameter.h"
#include "Ogshow.h"

int
plot3dToOverture(GenericGraphicsInterface & gi, aString & showFileName )
// ===============================================================================
// /Description:
//    Convert plot3d files into a show file.
//
// /gi (input) : graphics interface
// /showFileName (output) : name of the show file that was created.
// 
// ===============================================================================
{
  
//     cout << "Usage: `showPlot3d [grid.in][q.save]' \n"
//             "          grid.in : grid file (plot3d format) \n" 
//             "          q.save  : q file (plot 3d format) \n";

//   Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
    
//  GL_GraphicsInterface gi(plotOption,"showPlot3d");          // create a GL_GraphicsInterface object

  CompositeGrid cg;
  realCompositeGridFunction v;
  RealArray par(20);

  GraphicsParameters psp;                       // create an object that is used to pass parameters
  MappingInformation mapInfo;
  mapInfo.graphXInterface=&gi;
  gi.appendToTheDefaultPrompt("plot3d>"); // set the default prompt

  showFileName = "plot3d.show";

//  char buff[180];  // buffer for sprintf

  GUIState dialog;

  dialog.setWindowTitle("Plot3d To Overture");
  dialog.setExitCommand("exit","Exit");


//   dialog.setOptionMenuColumns(1);

//   enum LinerTypeEnum
//   {
//     linearLiner,
//     quadraticLiner,
//     freeFormLiner
//   } linerType=linearLiner;
  
//   aString linerTypeCommands[] = {"linear liner...", "quadratic liner...", "free form liner...", "" };
//   dialog.addOptionMenu("method:", linerTypeCommands, linerTypeCommands, (int)linerType );


  aString pushButtonCommands[] = {"save show file",
                                  "component names",
                                  "change grids",
                                  "plot grid",
                                  "plot solution",
                                  "help",
				  ""};
  int numRows=3;
  dialog.setPushButtons(pushButtonCommands,  pushButtonCommands, numRows ); 

  aString tbCommands[] = {"q file is cell centered",
                          "convert to primitive variables",
                          "convert to vertex centered",
                          "convert to cell centered",
                          "expect iblank",
 			  ""};

  bool convertToPrimitive=false;
  bool convertToVertexCentered=false;
  bool convertToCellCentered=false;
  bool expectIblank=false;
  bool qFileIsCellCentered=true;
  
  bool gridWasRead=false;
  bool solutionWasRead=false;
  bool showFileWasSaved=false;

  int tbState[10];
  tbState[0] = qFileIsCellCentered;
  tbState[1] = convertToPrimitive;
  tbState[2] = convertToVertexCentered;
  tbState[3] = convertToCellCentered;
  tbState[4] = expectIblank;
  

  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

// **  dialog.addInfoLabel("Volume = 0");

  // ----- Text strings ------
  const int numberOfTextStrings=20;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  aString name="none", fName="none";
  aString showFileLabel[2];
  showFileLabel[0]="Compressible Navier Stokes";
  showFileLabel[1]="Steady State";
  
  int nt=0;
  textCommands[nt] = "grid file";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s", (const char*)name);  nt++; 


  textCommands[nt] = "q file";  
  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s",(const char*)name);  nt++; 

  textCommands[nt] = "f file";  
  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s",(const char*)name);  nt++; 

  textCommands[nt] = "show file name";  
  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s",(const char*)showFileName);  nt++; 

  textCommands[nt] = "show file label 1:";  
  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s",(const char*)showFileLabel[0]);  nt++; 
  textCommands[nt] = "show file label 2:";  
  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s",(const char*)showFileLabel[1]);  nt++; 


  // null strings terminate list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  real machNumber=1., alpha=0., reynoldsNumber=0., t=0., gamma=1.4, Rg=1.;

  const int maxNumberOfComponents=50;
  aString componentName[maxNumberOfComponents];
  componentName[0]="rho";
  componentName[1]="u";
  componentName[2]="v";
  componentName[3]="w";
  componentName[4]="E";
  componentName[5]="p";
  

  aString answer="", line;
  aString gridName="", qName="";
  bool getGrid=false, getQFile=false, getFFile=false;
  
  gi.pushGUI(dialog);

  for( int it=0;; it++ )
  {
     
    gi.getAnswer(answer, "");
   
    int len;
    if( answer=="help" )
    {
      printF("=============================================================================================\n"
             "                --- plot3dToOverture: read plot3d grid files or solution files ---           \n"
             " \n"
             "  o Use this routine to read a plot3d grid file and optionally a plot3d solution file. The\n"
             "    resulting grids and solutions are saved to a show file. You should always choose 'save show file'\n"
             "    before exiting this routine if you want to save the results.\n"
             "  o plot3d grid files can also be read in from the 'create mappings' menu (e.g. with ogen). Use this\n"
             "    approach if you want to build an overlapping grid using ogen with the plot3d grids.\n"
             "  o Plot3d solution files are known as q files or f files. f files contain turbulence \n"
             "     model parameters.\n"
             "  o There are many different forms of plot3d files and we have to guess the format of the file\n"
             "    by reading the first few lines. Different formats include for example:\n"
             "      - single grid, formatted/unformatted, single/double precision. \n"
             "      - single grid q file, formatted/unformatted, single/double precision. \n"
             "      - multiple grid, formatted/unformatted, single/double precision, with/without iblank. \n"
             "      - multiple grid q file, formatted/unformatted, single/double precision, with/without iblank. \n"
             "    Overture reads/writes plot3d files using the functions in DataFormats.C, DataFormatsMG.C and dpm.f\n"
             "  o Choose the option 'expect iblank' if the plot3d file contains an iblank (mask) array. This\n"
             "    helps to determine the correct plot3d file format.\n"
             "  o Turn on the option 'q file is cell centered' if the plot3d solution data is cell centered.\n"
             "    Turn off this option if the solution data is located at verticies (nodes).\n"
             "  o Turn on the option 'convert to primitive variables' if the plot3d solution data are \n"
             "    conservative variables (density,momemtum,total energy) and you want to plot  \n"
             "    primitive variables (density,velocity,temperature).\n"
             "  o Plot3d files have no boundary condition information so you will need to use the \n"
             "    command 'change grids' to change the boundary conditions (and other grid info) after you\n"
             "    have read in the grid.\n"
             "=============================================================================================\n"
	);
    }
    else if( (len=answer.matches("grid file")) )
    {
      gridName = answer(len+1,answer.length()-1);
      getGrid=true;
     
      printF(" Looking for plot3d grid file = [%s]\n",(const char*)gridName);

    }
    else if( dialog.getToggleValue(answer,"q file is cell centered",qFileIsCellCentered) ){}//
    else if( dialog.getToggleValue(answer,"convert to primitive variables",convertToPrimitive) ){}//
    else if( dialog.getToggleValue(answer,"convert to vertex centered",convertToVertexCentered) ){}//
    else if( dialog.getToggleValue(answer,"convert to cell centered",convertToCellCentered) ){}//
    else if( dialog.getToggleValue(answer,"expect iblank",expectIblank) ){}//

    else if( dialog.getTextValue(answer,"show file label 1:","%s",showFileLabel[0]) ){}//
    else if( dialog.getTextValue(answer,"show file label 2:","%s",showFileLabel[1]) ){}//
    else if( (len=answer.matches("q file")) )
    {
      qName = answer(len+1,answer.length()-1);
      getQFile=true;
      printf(" Looking for plot3d q file (solution file) = [%s]\n",(const char*)qName);
    }
    else if( (len=answer.matches("f file")) )
    {
      fName = answer(len+1,answer.length()-1);
      getFFile=true;
      printf(" Looking for plot3d f file (solution file) = [%s]\n",(const char*)fName);
    }
    else if( answer=="component names" )
    {
      // get names for components
      for(int n=0; n<maxNumberOfComponents; n++ )
      {
	gi.inputString(answer,sPrintF("Enter the name of component %i (or `done' to finish)",n));
	if( answer=="done" )
	  break;
	int i=0;
	while( answer[i]==' ' && i<answer.length() ) i++;
       
	componentName[n]=answer(i,answer.length()-1);
      }
    }
    else if( answer=="plot grid" )
    {
      if( !gridWasRead )
      {
	printF("You should first choose a grid file before you can plot the grid.\n");
	continue;
      }
      
      PlotIt::plot(gi, cg,psp);
    }
    else if( answer=="plot solution" )
    {
      if( !solutionWasRead )
      {
	printF("You should first choose a 'q file' before you can plot the solution.\n");
	continue;
      }

      PlotIt::contour(gi,v,psp);

    }
    else if( answer=="erase" )
    {
      gi.erase();
    }
    else if( answer=="save show file" )
    {
      
      printF("Saving the current grid and solution to the show file [%s]\n",(const char*)showFileName);
     
      showFileWasSaved=true;

      Ogshow show(showFileName);
      show.saveGeneralComment("Grids and solutions and from plot3d format"); 

       

      ListOfShowFileParameters showFileParams;
      showFileParams.push_back(ShowFileParameter("reynoldsNumber",reynoldsNumber));
      showFileParams.push_back(ShowFileParameter("machNumber",machNumber));
      showFileParams.push_back(ShowFileParameter("gamma",gamma));
      showFileParams.push_back(ShowFileParameter("Rg",Rg));

      int rc=0, uc=1, vc=2, wc=3, tc=4, pc=5;   // **************************** fix this 
      
      showFileParams.push_back(ShowFileParameter("densityComponent",rc));
      showFileParams.push_back(ShowFileParameter("temperatureComponent",tc));
      showFileParams.push_back(ShowFileParameter("pressureComponent",pc));
      showFileParams.push_back(ShowFileParameter("uComponent",uc));
      showFileParams.push_back(ShowFileParameter("vComponent",vc));
      showFileParams.push_back(ShowFileParameter("wComponent",wc));

      int numberOfSpecies=0;
      showFileParams.push_back(ShowFileParameter("numberOfSpecies",numberOfSpecies));

      show.saveGeneralParameters(showFileParams);

      if( !solutionWasRead )
      {
        // no solution was read -- just make a single component solution
	v.updateToMatchGrid(cg);
	v=0.;
      }
      

      show.startFrame();                                         // start a new frame
      show.saveComment(0,showFileLabel[0]);   // comment 0 (shown on plot)
      show.saveComment(1,showFileLabel[1]);   // comment 1 (shown on plot)
      show.saveSolution( v ); 

      show.close();
     
    }
    else if( answer=="change grids" )
    {
      printF("Change the boundary conditions, share values etc. for specified grids.\n");
      
      printF("ERROR: 'change grids' -- this is not implemented yet. Finish me Bill!\n");

    }
    else if ( answer=="exit" )
    {
      break;
    }
    else 
    {
      gi.outputString("could not understand command : "+answer);
    }


    if( getGrid )
    {
      getGrid=false;
     
      // read in the grid(s) 

      const int ngd=100;  // fix this 
      intArray *maskArray = new intArray [ngd];
  
      DataFormats::readPlot3d(mapInfo,gridName,maskArray,expectIblank);
  

      // Create an overlapping grid generator
      Ogen ogen(gi);

      int numberOfGrids = mapInfo.mappingList.getLength();
      // indicate which mappings should be used in the CompositeGrid (use all in this case)
      IntegerArray mapList(numberOfGrids);
      for( int grid=0; grid<numberOfGrids; grid++ )
	mapList(grid)=grid;


      // Put the mappings into the CompositeGrid
      ogen.buildACompositeGrid(cg,mapInfo,mapList);


//      int numberOfComponentGrids = mapInfo.mappingList.getLength();
//      int numberOfDimensions=3;
//      if( maskArray[0].getLength(2)==1 )
//        numberOfDimensions=2;

//      cg.setNumberOfGridsAndDimensions(numberOfComponentGrids,numberOfDimensions);
     
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        MappedGrid & mg = cg[grid];
	intArray & mask = maskArray[grid];

	if( convertToCellCentered )
	  cg[grid].changeToAllCellCentered();            // make a cell centered grid

	mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask);  
  
	Range I1(0,mask.getBound(0)-mask.getBase(0));  // shift to base 0
	Range I2(0,mask.getBound(1)-mask.getBase(1));
	Range I3(0,mask.getBound(2)-mask.getBase(2));

	if( mask.getLength(0)>0 )
	{
	  printF("fill in the mask array for grid=%i... plot3d-mask=[%i,%i][%i,%i][%i,%i]\n",grid,
		 mask.getBase(0),mask.getBound(0),mask.getBase(1),mask.getBound(1),mask.getBase(2),mask.getBound(2) );
	  // printf(" min(mask)=%i max(mask)=%i \n",min(mask),max(mask));
    
  	  intArray & maskg = mg.mask();
	  maskg(I1,I2,I3)=mask;
	  // mg.mask().display("mask");
    
	  // tell the MappedGrid that the mask was computed
	  mg->computedGeometry |= CompositeGrid::THEmask;

	}
       
      }
      delete [] maskArray;

      // tell the CG that the mask was computed
      cg->computedGeometry |= CompositeGrid::THEmask;

      gridWasRead=true;
      printF("\n ==== Plot3d Grid was read. Use 'change grids' to change boundary conditions etc. for each grid ===\n");


    }
   
    if( getQFile || getFFile )
    {

      // read in a "q" file with data

      realArray *ua = new realArray[cg.numberOfComponentGrids()];
      aString plot3dName= getQFile? qName : fName;
      DataFormats::readPlot3d(ua,par,plot3dName);

      // par(0)=fsmach; par(1)=alpha; par(2)=re; par(3)=time; par(4)=gaminf;
      real machNumber=par(0);
      real alpha=par(1);
      real reynoldsNumber=par(2);
      real t = par(3);
      real gamma=par(4);
      real Rg=gamma/(gamma-1.);    // *** check this ***

      sPrintF(showFileLabel[1],"Re = %9.3e, Ma=%6.4f, alpha=%6.4f",reynoldsNumber,machNumber,alpha);
      dialog.setTextLabel("show file label 1:",showFileLabel[1]);
     

      
//     int nv=v.getComponentDimension(0); 
      int nv=0;
      if( getFFile )
	nv=v[0].getLength(3);
     
      printf(" *** plot3dToOverture: nv=%i ****\n",nv);
     
      // number of q components: 
      int nq=ua[0].getLength(3);

      Range N(nv,nv+nq-1);
      Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
      Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];
      int is[3];
     
      Range all;  
      realCompositeGridFunction w;
      if( getFFile )
	w=v; // save q values
      v.updateToMatchGrid(cg,all,all,all,nv+nq);
     
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {

	realMappedGridFunction & u = v[grid]; 
	realArray & u0 = ua[grid];

	int offset=-1, offset3=-1;
	if( qFileIsCellCentered )
	{
	  // The cell centred data include the ghost points (at least from isaac)
	  offset=-2;
	  offset3 = cg.numberOfDimensions()==2 ? -1 : -2;
	}
       
	I1=Range(u0.getBase(0)+offset,u0.getBound(0)+offset); 
	I2=Range(u0.getBase(1)+offset,u0.getBound(1)+offset); 
	I3=Range(u0.getBase(2)+offset3,u0.getBound(2)+offset3); 
  
	printf("*** grid=%i I1,I2,I3=[%i,%i][%i,%i][%i,%i]\n",grid,
	       I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
  
	if( getFFile )
	  u(all,all,all,Range(0,nv-1))=w[grid];
	v[grid](all,all,all,Range(nv,nq-1))=0.;

	if( nv==0 )
	  u(all,all,all,0)=1.; // default values for rho
       
	Range J1=u0.dimension(0), J2=u0.dimension(1), J3=u0.dimension(2), M=u0.dimension(3);

	u(I1,I2,I3,N)=u0(J1,J2,J3,M);

	// assign first ghost -- set equal to first line in. 
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	{
	  for( int side=0; side<=1; side++ )
	  {
	    is[0]=0; is[1]=0; is[2]=0;
	    is[axis]=1-2*side;
	    Ib1=u.dimension(0); Ib2=u.dimension(1); Ib3=u.dimension(2);
	    Ibv[axis]=side==0 ? Iv[axis].getBase() : Iv[axis].getBound();
	   
	    // u(Ib1-is[0],Ib2-is[1],Ib3-is[2],N)=u(Ib1,Ib2,Ib3,N);
	  }
	}
       
	   


	if( qFileIsCellCentered && convertToVertexCentered )
	{
	  if( cg.numberOfDimensions()==2 )
	  {
	    // extrap "ghost lines"
          
	    // average to the vertex: 
	    u(I1,I2,I3,N)=.25*( u(I1,I2,I3,N)+u(I1-1,I2,I3,N) + u(I1,I2-1,I3,N)+ u(I1-1,I2-1,I3,N) );
	   
	    OV_ABORT("finish this");

	  }
	  else
	  {
	    OV_ABORT("finish this");

	  }
	 
	}
       
	getIndex(cg[grid].dimension(),I1,I2,I3);

	if( convertToPrimitive && getQFile && nq>=cg.numberOfDimensions()+2 )
	{
	  realArray rhoInverse;
	  rhoInverse = 1./max(REAL_EPSILON,u(I1,I2,I3,0));
    
	  u(I1,I2,I3,1)*=rhoInverse;
	  u(I1,I2,I3,2)*=rhoInverse;
	  if( cg.numberOfDimensions()==3 )
	  {
	    u(I1,I2,I3,3)*=rhoInverse;
           
	    // get p from  E=p/(gamma-1) + .5*rho*( u^2 )
	    u(I1,I2,I3,4) = (gamma-1.)*( u(I1,I2,I3,4)
					 -.5*u(I1,I2,I3,0)*( SQR(u(I1,I2,I3,1))+SQR(u(I1,I2,I3,2))+SQR(u(I1,I2,I3,3)) ) );
	  }
	  else
	  {
	    u(I1,I2,I3,3) = (gamma-1.)*( u(I1,I2,I3,3)
					 -.5*u(I1,I2,I3,0)*( SQR(u(I1,I2,I3,1))+SQR(u(I1,I2,I3,2)) ) );
	  }
	 

	}
       

//   int j1,j2,j3;
//   for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
//   for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
//     for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
//     {
//       j1=i1+1;
//       j2=i2+1;
//       j3=i3+1;
      
//       u(i1,i2,i3,0)=u0(j1,j2,j3,0);
//       printf(" i1,i2,i3=%i,%i,%i u=%4.1f  u0=%4.1f \n",i1,i2,i3,u(i1,i2,i3,0),u0(j1,j2,j3,0));
      
//     }
       
      }
     
      v.setName("q");                              // give names to grid function ...
      for( int n=0,m=0; n<nq+nv; n++,m++ )
      {
	if( componentName[n]=="w" && cg.numberOfDimensions()==2 ) m++;  // skip "w" in 2D
       
	v.setName(componentName[m],n);
      }
//   display(u0(all,all,all,0),"u0(I1,I2,I3,0)","%5.1f ");
//   display(u(all,all,all,0),"u(I1,I2,I3,0)","%4.1f ");

      solutionWasRead=true;

      delete [] ua;

    } //end if getQFile || getFFile )
    
   
//   if( mg.numberOfDimensions()==2 )
//   {
//     PlotIt::streamLines(gi,u,psp);
//   }
   
    getQFile=false;
    getFFile=false;

  }
  
  if( !showFileWasSaved )
  {
    printF("\n *** plot3dTOverture: WARNING! You should choose 'save show file' file if you want to save the solution for "
           " plotting etc. ***\n");
  }
  
  gi.erase();
  gi.popGUI();  // pop dialog



  return 0;
}



