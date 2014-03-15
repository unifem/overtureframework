//#define BOUNDS_CHECK
#include "Overture.h"
#include "UnstructuredMapping.h"
#include "MappedGridFunction.h"
#include "MappedGridOperators.h"
#include "PlotStuff.h"
#include "HDF_DataBase.h"
#include "ArraySimple.h"
#include "OGTrigFunction.h"
#include "OGPolyFunction.h"
#include "OGPulseFunction.h"

#include "UnstructuredOperators.h"

void performComputation(PlotStuff &ps, GraphicsParameters &params, UnstructuredMapping &umap, OGFunction &exact, aString &op);

int main(int argc, char *argv[])
{
  
  Overture::start(argc, argv);

  int plotOption=TRUE;
  aString commandFileName="";
  if( argc > 1 )
    { // look at arguments for "noplot" or some other name
      aString line;
      for( int i=1; i<argc; i++ )
	{
	  line=argv[i];
	  if( line=="noplot" )
	    plotOption=FALSE;
	  else if( commandFileName=="" )
	    commandFileName=line;    
	}
    }
  else
    cout << "Usage: unstest [noplot] [file.cmd]"<<endl;

  PlotStuff ps(plotOption);
  GraphicsParameters params;

  ps.saveCommandFile("unsEx.cmd");

  if ( commandFileName!="" )
    ps.readCommandFile(commandFileName);

  GUIState gui;
  gui.setWindowTitle("Unstructured Difference Operator Example");
  gui.setExitCommand("quit","Quit");
  
  aString rbCommands[] = {"trig","poly","pulse",""};
  aString rbLabels[]   = {"Trig Func","Polyn Func", "Pulse Func","" };
  gui.addRadioBox("Functions",rbCommands,rbLabels,0);
  RadioBox &rbox = gui.getRadioBox(0);

  aString fCommands[] = {"x","xx","y","yy","laplacian",""};
  gui.addRadioBox("Operators",fCommands,fCommands,4);
  RadioBox fbox = gui.getRadioBox(1);
  
  aString pbCommands[] = {"Read Ingrid", "Read HDF", ""};
  gui.setPushButtons(pbCommands,pbCommands,1);

  aString answer="";
  aString meshFile;
  aString buf;

  UnstructuredMapping umap;
  bool compute;
  OGFunction *exact = new OGTrigFunction();
  int rangeDimension;
  int nnodes;
  int nelems;
  real delt[3];
  
  real bounds[2][3];
  bool mapread = false;
  ps.pushGUI(gui);

  aString currentOp = "laplacian";

  for (;;)
    {
      compute = true;
      ps.getAnswer(answer,"");

      if ( answer=="quit" )
	break;
      else if ( answer=="Read Ingrid" || answer=="Read HDF" )
	{
	  if ( answer=="Read Ingrid" )
	    {
	      ps.inputFileName(meshFile,"",".msh");
	      if ( meshFile.length()>0 && meshFile!=" " )
		{
		  umap.get(meshFile);
		  mapread = true;
		}
	      else 
		{
		  buf="";
		  sPrintF(buf,"Bad file name: `%s'", (char *)(const char *)meshFile);
		  ps.createMessageDialog(buf, errorDialog);
		}
	    }
	  else if ( answer=="Read HDF" )
	    {
	      ps.inputFileName(meshFile,"",".hdf");
	      if ( meshFile.length()>0 && meshFile!=" " )
		{
		  HDF_DataBase db;
		  db.mount(meshFile,"R");
		  umap.get(db,"");
		  mapread = true;
		}
	      else 
		{
		  buf="";
		  sPrintF(buf,"Bad file name: `%s'", (char *)(const char *)meshFile);
		  ps.createMessageDialog(buf, errorDialog);
		}
	    }
	  rangeDimension = umap.getRangeDimension();
	  nnodes = umap.getNumberOfNodes();
	  nelems = umap.getNumberOfElements();
	  bounds[0][2] = bounds[1][2] = 0.0;
	  for ( int a=0; a<2; a++ )
	    for ( int r=0; r<rangeDimension; r++ )
	      bounds[a][r] = (real)umap.getRangeBound(a,r);

	  delt[2] = 0;
	  for ( int r=0; r<rangeDimension; r++ )
	    delt[r] = (bounds[1][r]-bounds[0][r])/5.;

	  cout<<"the mesh is "<<rangeDimension<<"D with "<<nnodes<<" nodes and "<<nelems<<" elements"<<endl;
	}
      else if ( answer=="x" || answer=="xx" | answer=="y" || answer=="yy" || answer=="laplacian" )
	{
	  currentOp = answer;
	  cout<<"will use operator "<<currentOp<<endl;
	}
      else if ( answer=="trig" )
	{
	  delete exact;
	  exact = new OGTrigFunction(1/delt[0],1/delt[1]);
	}
      else if ( answer=="poly" )
	{
	  delete exact;
	  exact = new OGPolyFunction();
	}
      else if ( answer=="pulse")
	{
	  delete exact;
	  exact = new OGPulseFunction();
	}
      else 
	{
	  ps.outputString("ERROR : unknown command "+answer);
	  compute = false;
	}

      if ( compute && mapread )
	performComputation(ps,params,umap,*exact, currentOp);
    }
  ps.popGUI();
  return 0;
}

void 
performComputation(PlotStuff &ps, GraphicsParameters &params, UnstructuredMapping &umap, OGFunction &exact, aString &op)
{

  // grab some usefull information from the UnstructuredMapping : problem dimension, number of nodes, number of elements,
  //                                                              and a list of the faces (used for bc)
  int rangeDimension = umap.getRangeDimension();
  int nnodes = umap.getNumberOfNodes();
  int nelems = umap.getNumberOfElements();
  const intArray & faces = umap.getFaces();

  // create a MappedGrid for the UnstructuredMapping so we can create and plot GridFunctions, also update the vertex array
  MappedGrid umesh(umap);
  umesh.update(MappedGrid::THEvertex);

  Range all;

  // create a node centered mapped grid function for the unstructured mesh
  //   note that cell centered grid functions on unstructured mesh do not work yet.

  // Note: unstructured meshes return functions dimensioned (nnodes,1,1,ncomponents)
  realMappedGridFunction u(umesh,all,all,all,1);  
  realMappedGridFunction up1;

  // initialize the result to be the original function
  up1=u;

  // suggested cast to RealArray (see Overture developer's guide)
  RealArray & function = (RealArray &)u;//meshFunction;
  RealArray & testDer  = (RealArray &)up1;

  // Note: an unstructured mesh returns vertices dimensioned (nnodes,1,1,rangeDimension)
  RealArray &vertices = umesh.vertex();
  
  // initialize the function to the TwilightZone function passed into this function
  if ( rangeDimension==2 )
    for ( int n=0; n<nnodes; n++ )
      function(n,0,0,0) = exact(vertices(n,0,0,0),vertices(n,0,0,1),0.0);

  // lets take a look!
  params.set(GI_TOP_LABEL,"Input Function");
  ps.erase();
  PlotIt::contour(ps,u,params);

  // reshape to make index more concise
  function.reshape(nnodes);
  testDer.reshape(nnodes);

  // create some unstructured mesh operators tailored to our specific mesh
  //  UnsOperators unsOper(umesh);
  UnsOperators unsOper(umesh);
  
  // now, do the work, in this case, take a node centered laplacian of the function
  if ( op=="x" )
    up1 = unsOper.x(u);
  else if (op=="xx")
    up1 = unsOper.xx(u);
  else if (op=="y")
    up1 = unsOper.y(u);
  else if (op=="yy")
    up1 = unsOper.yy(u);
  else if (op=="laplacian")
    up1 = unsOper.laplacian(u);

  // set the boundary nodes to the exact value 
  if ( op=="x" )
    for ( int fbdy=0; fbdy<umap.getNumberOfBoundaryFaces(); fbdy++ )
      {
	int fnm = umap.getBoundaryFace(fbdy);
	int nn = faces(fnm,0);
	testDer(nn) = exact.x(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0);
	nn = faces(fnm,1);
	testDer(nn) = exact.x(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0);
      }
  else if (op=="xx")
    for ( int fbdy=0; fbdy<umap.getNumberOfBoundaryFaces(); fbdy++ )
      {
	int fnm = umap.getBoundaryFace(fbdy);
	int nn = faces(fnm,0);
	testDer(nn) = exact.xx(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0);
	nn = faces(fnm,1);
	testDer(nn) = exact.xx(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0);
      }
  else if (op=="y")
    for ( int fbdy=0; fbdy<umap.getNumberOfBoundaryFaces(); fbdy++ )
      {
	int fnm = umap.getBoundaryFace(fbdy);
	int nn = faces(fnm,0);
	testDer(nn) = exact.y(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0);
	nn = faces(fnm,1);
	testDer(nn) = exact.y(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0);
      }
  else if (op=="yy")
    for ( int fbdy=0; fbdy<umap.getNumberOfBoundaryFaces(); fbdy++ )
      {
	int fnm = umap.getBoundaryFace(fbdy);
	int nn = faces(fnm,0);
	testDer(nn) = exact.yy(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0);
	nn = faces(fnm,1);
	testDer(nn) = exact.yy(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0);
      }
  else if (op=="laplacian")
    for ( int fbdy=0; fbdy<umap.getNumberOfBoundaryFaces(); fbdy++ )
      {
	int fnm = umap.getBoundaryFace(fbdy);
	int nn = faces(fnm,0);
	testDer(nn) = ( exact.xx(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0) + 
			exact.yy(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0) );
	nn = faces(fnm,1);
	testDer(nn) = ( exact.xx(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0) + 
			exact.yy(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0) );
      }

  // ok, reshape again so we can plot
  function.reshape(nnodes,1,1,1);
  testDer.reshape(nnodes,1,1,1);

  // now, take a look at the laplacian
  ps.erase();
  params.set(GI_TOP_LABEL,"Function after Operator "+op+" Applied");
  PlotIt::contour(ps,up1,params);


  // compute some norms
  real linf = -REAL_MAX;
  real l2=0,l1=0,asum=0;
  for ( int nn=0; nn<nnodes; nn++ )
    {
      real ex=REAL_MAX;
      if ( op=="x" )
	ex = exact.x(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0);
      else if (op=="xx")
	ex = exact.xx(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0);
      else if (op=="y")
	ex = exact.y(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0);
      else if (op=="yy")
	ex = exact.yy(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0);
      else if (op=="laplacian")
	ex = exact.xx(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0) + exact.yy(vertices(nn,0,0,0),vertices(nn,0,0,1),0.0);

      real err = fabs(ex-up1(nn,0,0,0));
      real area = unsOper.area(nn);
      
      linf = max(linf, err);
      l1 += err*area;
      l2 += err*err*area;
      asum += area;
    }
  
  l1 /=asum;
  l2 = sqrt(l2/asum);
  cout<<"linf "<<linf<<endl;
  cout<<"l1   "<<l1<<endl;
  cout<<"l2   "<<l2<<endl;

}


