#include <iostream>

#include "UnstructuredMapping.h"
#include "UnstructuredOperators.h"
#include "SquareMapping.h"
#include "BoxMapping.h"
#include "OGTrigFunction.h"
#include "OGPolyFunction.h"

#include "GenericGraphicsInterface.h"
#include "PlotIt.h"

extern bool verifyUnstructuredConnectivity( UnstructuredMapping &umap, bool verbose );

namespace {

  enum DerivativeToTest {
    noDerivative,
    xDerivative,
    yDerivative,
    zDerivative,
    xxDerivative,
    xyDerivative,
    xzDerivative,
    yyDerivative,
    yzDerivative,
	//zxDerivative,
	//zyDerivative,
    zzDerivative,
    laplacianOperator,
    gradient,
    divergence,
    numberOfDerivativesToTest
  };

  ArraySimpleFixed<real,3,1,1,1> runTest(DerivativeToTest dtype, MappedGrid &mg, 
					 realMappedGridFunction &u, OGFunction &ogf, GenericGraphicsInterface *gip=0)
  {

    UnstructuredMapping &umap = *((UnstructuredMapping*)(mg->mapping.mapPointer));

    realMappedGridFunction ux;
    
    bool plotIt = gip!=0;
    
    Index I1,I2,I3;

    //    getIndex(mg.gridIndexRange(),I1,I2,I3);
    getIndex(u,I1,I2,I3);
    UnstructuredMapping::EntityTypeEnum cellType=UnstructuredMapping::Vertex;
    if ( mg.isAllCellCentered() )
      {
	cellType = UnstructuredMapping::EntityTypeEnum(umap.getDomainDimension());
	I1 = Range(umap.size(cellType));
	u(I1,I2,I3) = ogf(mg,I1,I2,I3,0,0,GridFunctionParameters::cellCentered);
      }
    else
      u(I1,I2,I3) = ogf(mg,I1,I2,I3,0,0,GridFunctionParameters::vertexCentered);

    if (plotIt)
      {
	PlotIt::contour(*gip,u);
	gip->erase();
      }
    
    realMappedGridFunction error;

    real t0 = getCPU();

    switch(dtype)
      {
      case noDerivative:
	error.updateToMatchGridFunction(u);
	error = fabs(u(I1,I2,I3)-ogf(mg,I1,I2,I3,0,0));
	break;
      case xDerivative:
	ux = u.x();
	error.updateToMatchGridFunction(u);
	error = fabs(ux(I1,I2,I3) - ogf.x(mg,I1,I2,I3,0,0));
// 	ux(I1,I2,I3).display("ux");
// 	ogf.x(mg,I1,I2,I3,0,0).display("ogf.x");
	break;
      case yDerivative:
	ux = u.y();
	error.updateToMatchGridFunction(u);
	error = fabs(ux(I1,I2,I3) - ogf.y(mg,I1,I2,I3,0,0));
	break;
      case zDerivative:
	ux = u.z();
	error.updateToMatchGridFunction(u);
	error = fabs(ux(I1,I2,I3) - ogf.z(mg,I1,I2,I3,0,0));
	break;
      case xxDerivative:
	ux = u.xx();
	error.updateToMatchGridFunction(u);
	error = fabs(ux(I1,I2,I3) - ogf.xx(mg,I1,I2,I3,0,0));
	break;
      case xyDerivative:
	ux = u.xy();
	error.updateToMatchGridFunction(u);
	error = fabs(ux(I1,I2,I3) - ogf.xy(mg,I1,I2,I3,0,0));
	break;
      case xzDerivative:
	ux = u.xz();
	error.updateToMatchGridFunction(u);
	error = fabs(ux(I1,I2,I3) - ogf.xz(mg,I1,I2,I3,0,0));
	break;
	//    case MappedGridOperators::yxDerivative:
	//      UXIXJ(1,0);
	//      break;
      case yyDerivative:
	ux = u.yy();
	error.updateToMatchGridFunction(u);
	error = fabs(ux(I1,I2,I3) - ogf.yy(mg,I1,I2,I3,0,0));
	break;
      case yzDerivative:
	ux = u.yz();
	error.updateToMatchGridFunction(u);
	error = fabs(ux(I1,I2,I3) - ogf.yz(mg,I1,I2,I3,0,0));
	break;
	//    case MappedGridOperators::zxDerivative:
	//      xixj_uFV2(2,0,u,scalar,ux,CC);
	//      break;
	//    case MappedGridOperators::zyDerivative:
	//      xixj_uFV2(2,1,u,scalar,ux,CC);
	//      break;
      case zzDerivative:
	ux = u.zz();
	error.updateToMatchGridFunction(u);
	error = fabs(ux(I1,I2,I3) - ogf.zz(mg,I1,I2,I3,0,0));
	break;
      case laplacianOperator:
	ux = u.laplacian();
	error.updateToMatchGridFunction(ux);
	if ( umap.getDomainDimension()==3 )
	  error = fabs(ux(I1,I2,I3) - ( ogf.xx(mg,I1,I2,I3,0,0) + ogf.yy(mg,I1,I2,I3,0,0) + ogf.zz(mg,I1,I2,I3,0,0) ) );
	else
	  error = fabs(ux(I1,I2,I3) - ( ogf.xx(mg,I1,I2,I3,0,0) + ogf.yy(mg,I1,I2,I3,0,0) ) );
	break;
      case gradient:
	ux = u.grad();
	// use the magnitude of fabs( grad(u) - grad(ogf) )
	error.updateToMatchGridFunction(u);
	//	error.redim(u);
	error(I1,I2,I3) = fabs(ux(I1,I2,I3,0) - ogf.x(mg,I1,I2,I3,0,0))*fabs(ux(I1,I2,I3,0) - ogf.x(mg,I1,I2,I3,0,0));
	error(I1,I2,I3) += fabs(ux(I1,I2,I3,1) - ogf.y(mg,I1,I2,I3,0,0))*fabs(ux(I1,I2,I3,1) - ogf.y(mg,I1,I2,I3,0,0));
	if ( umap.getDomainDimension()==3 )
	  error(I1,I2,I3) += fabs(ux(I1,I2,I3,2) - ogf.z(mg,I1,I2,I3,0,0))*fabs(ux(I1,I2,I3,2) - ogf.z(mg,I1,I2,I3,0,0));
	error = sqrt(error);
	break;
      case divergence:
	ux = u.div();
	error.updateToMatchGridFunction(ux);

	if ( umap.getDomainDimension()==3 )
	  error = fabs(ux(I1,I2,I3) - ( ogf.x(mg,I1,I2,I3,0,0) + ogf.y(mg,I1,I2,I3,0,0) + ogf.z(mg,I1,I2,I3,0,0) ) );
	else
	  error = fabs(ux(I1,I2,I3) - ( ogf.x(mg,I1,I2,I3,0,0) + ogf.y(mg,I1,I2,I3,0,0) ) );
	break;
      default:
	cout<<"ERROR : testUOPS : operator "<<dtype<<" not implemented yet"<<endl;
	ArraySimpleFixed<real,3,1,1,1> err;
	err = REAL_MAX;
	return err;
      }

    real timing = getCPU()-t0;
//     UnstructuredMappingIterator v,v_end;
//     const realArray &verts = mg.vertex();
    
//     UnstructuredMapping::tag_entity_iterator bdyEnt;
    
//     for ( bdyEnt = umap.tag_entity_begin("boundary entity");
// 	  bdyEnt != umap.tag_entity_end("boundary entity");
// 	  bdyEnt++ )
//       if ( bdyEnt->et==UnstructuredMapping::Vertex )
// 	ux(bdyEnt->e,0,0) = ogf.x(verts(bdyEnt->e,0,0,0), verts(bdyEnt->e,0,0,1), 0.);
    
    const realArray &cellVolume = mg.cellVolume();
    UnstructuredMappingIterator vert, vert_end;
    vert_end = umap.end(cellType,true);
    real linf = -REAL_MAX;
    real l1 = 0;
    real l2 = 0;
    real vol=0;
    real maxv=-REAL_MAX,minv=REAL_MAX;
    for ( vert=umap.begin(cellType,true);
	  vert!=vert_end;
	  vert++ )
      {
	//	if ( !vert.isGhost() )
	//		if ( !umap.hasTag(cellType,*vert,std::string("boundary ")+UnstructuredMapping::EntityTypeStrings[cellType].c_str()) ) // this interface for finding bc info will have to change...
	  {
	    real e = error(*vert,0,0);
	    real v = cellVolume(*vert,0,0);
	    linf = max(linf, e);
	    l1+=e*v;
	    l2+=e*e*v;
	    vol+=v;
	    maxv = max(v,maxv);
	    minv = min(v,minv);
	  }
      }
    //         ux.display("ux");
    //             error.display("error");
    cout<<"linf     = "<<linf<<endl;
    cout<<"l1       = "<<l1/vol<<endl;
    cout<<"l2       = "<<sqrt(l2/vol)<<endl;
    cout<<"maxv     = "<<maxv<<endl;
    cout<<"minv     = "<<minv<<endl;
    cout<<"cpu time = "<<timing<<endl;
    cout<<"-------"<<endl;
    
    if (plotIt) 
      {
	PlotIt::contour(*gip,ux);
	gip->erase();
	PlotIt::contour(*gip,error);
	gip->erase();
      }

    ArraySimpleFixed<real,3,1,1,1> err;
    err[0] = linf;
    err[1] = l1;
    err[2] = l2;
    return err;
  }

}

int main(int argc, char *argv[])
{
  Overture::start(argc,argv);

  //  GenericGraphicsInterface &gi = *Overture::getGraphicsInterface();
  //  GraphicsParameters gp;

  enum Centering {
    vertexCentered,
    cellCentered
  };

  Centering centering = vertexCentered;
  int rDim = 2;

  std::string dtest = "";
  bool plot=false;
  bool verifyMesh = false;
  string gridToTest ="";
  bool useTri=false;
  bool usePoly = true;
  if ( argc>1 )
    {
      for ( int i=1; i<argc; i++ )
	{
	  std::string arg = argv[i];
	  if ( arg=="vcent" )
	    centering=vertexCentered;
	  else if (arg=="ccent")
	    centering=cellCentered;
	  else if (arg=="3d")
	    rDim = 3;
	  else if (arg=="2d")
	    rDim = 2;
	  else if (arg=="plot")
	    plot=true;
	  else if (arg=="checkmesh")
	    verifyMesh = true;
	  else if (arg=="shell")
	    gridToTest = arg;
	  else if ( arg=="tri" )
	    useTri=true;
	  else if ( arg=="useTrig")
	    usePoly=false;
	  else
	    dtest = arg;
	}
    }

  gridToTest = gridToTest.length() ? gridToTest : ( rDim==2 ? "square" : "box" );

  if ( dtest!="" )
    cout<<"will test "<<dtest<<" operator "<<endl;
  else
    cout<<"will test all the operators"<<endl;
  
  cout<<"centering : "<<(centering==vertexCentered ? "vertex" : "cell" )<<endl;
  cout<<"dimension : "<<rDim<<endl;
  cout<<"using grid: "<<gridToTest<<endl;
  cout<<"tw func.  : "<<(usePoly ? "polynomial" : "trig")<<endl;

  GenericGraphicsInterface *gip=0;
  if (plot) gip=Overture::getGraphicsInterface();

  int n = 3;

  OGTrigFunction ogtrig;
  OGPolyFunction ogpoly(1,rDim,1,0);

  OGFunction &ogf = (usePoly ? (OGFunction &)ogpoly : (OGFunction &)ogtrig);
  //OGFunction &ogf = ogpoly;

  SquareMapping square(0.,1.,
		       0.,1.);
  
  BoxMapping box(0.,1.,
		 0.,1.,
		 0.,1.);

  
  //Mapping & smap = square;
  //Mapping & smap = box;
  Mapping &smap = (rDim==2 ? (Mapping &)square : (Mapping &)box);

  int nConv = 3;

  string shellGrids[] = { "square.20.tri.msh",
			  "square.30.tri.msh",
			  "square.40.tri.msh",
			  "square.50.tri.msh",
			  "square.80.tri.msh",
			  "cyl.tri.30.msh",
			  "cyl.tri.40.msh",
			  "cyl.tri.50.msh",
			  "cyl.tri.60.msh",
			  "cyl.tri.70.msh",
			  "slab.20.msh",
			  "slab.30.msh",
			  "slab.60.msh",
			  "slab.80.msh",
			  "shell.tet.1.msh",
			  "shell.tet.2.msh",
			  "shell.tet.3.msh",
			  "shell.tet.4.msh",
			  "shell.tet.5.msh",
			  "shell.tet.6.msh",
			  "shell.tet.7.msh" };
    
  if ( gridToTest=="shell" )
    nConv = 21;

  ArraySimple< ArraySimpleFixed<real,3,1,1,1> > errors(numberOfDerivativesToTest,nConv);;

  for ( int i=0; i<nConv; i++ )
    {
      real t0 = getCPU();

      int f = pow(2,i);
      int N = (n-1)*f+1;
      UnstructuredMapping umap;

      umap.setPreferTriangles(useTri);

      if ( gridToTest!="shell" )
	{
	  for ( int d=0; d<smap.getDomainDimension(); d++ )
	    smap.setGridDimensions(d,N);
	  
	  umap.buildFromAMapping(smap);
	}
      else
	{
	  umap.get(shellGrids[i]);
	  umap.expandGhostBoundary();
	}

      if ( verifyMesh ) verifyUnstructuredConnectivity(umap,true);

      
      MappedGrid mg(umap);
      GridFunctionParameters::GridFunctionType cent= GridFunctionParameters::vertexCentered;

      if ( centering==vertexCentered )
	mg.changeToAllVertexCentered();
      else
	{
	  mg.changeToAllCellCentered();
	  cent = GridFunctionParameters::cellCentered;
	}

      mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );//| MappedGrid::THEminMaxEdgeLength);
      
      MappedGridOperators mgop(mg);

      realMappedGridFunction u(mg,cent),ux(mg,cent);

      u.setOperators(mgop);

      real initTime = getCPU()-t0;
//        mg.faceNormal().display("faceNormal");
//        mg.faceArea().display("faceArea");
//        mg.cellVolume().display("cellVolume");
//             mg.center().display("cellCenter");

      if ( gridToTest!="shell" )
	cout<<"NORMS for N="<<N<<endl;
      else
	cout<<"NORMS for "<<shellGrids[i]<<endl;

      //      cout<<"EMIN = "<<mg.minimumEdgeLength(0)<<endl;
      //      cout<<"EMAX = "<<mg.maximumEdgeLength(0)<<endl;
      cout<<"INIT TIME = "<<initTime<<endl;

      if ( dtest=="" || dtest=="NONE" )
	{
	  cout<<"IDENTITY OPERATOR : "<<endl;
	  errors(xDerivative,i) = runTest(noDerivative, mg, u, ogf,gip);
	}

      if ( dtest=="" || dtest=="X" )
	{
	  cout<<"X OPERATOR : "<<endl;
	  errors(xDerivative,i) = runTest(xDerivative, mg, u, ogf,gip);
	}

      if ( dtest=="" || dtest=="Y" )
	{
	  cout<<"Y OPERATOR : "<<endl;
	  errors(yDerivative,i) = runTest(yDerivative, mg, u, ogf,gip);
	}

      if ( (dtest=="" || dtest=="Z") && mg.numberOfDimensions()==3 )
	{
	  cout<<"Z OPERATOR : "<<endl;
	  errors(yDerivative,i) = runTest(yDerivative, mg, u, ogf,gip);
	}

      if ( dtest=="" || dtest=="XX" )
	{
	  cout<<"XX OPERATOR : "<<endl;
	  errors(xxDerivative,i) = runTest(xxDerivative,mg,u,ogf,gip);
	}

      if ( dtest=="" || dtest=="YY" )
	{
	  cout<<"YY OPERATOR : "<<endl;
	  errors(yyDerivative,i) = runTest(yyDerivative,mg,u,ogf,gip);
	}

      if ( (dtest=="" || dtest=="ZZ") && mg.numberOfDimensions()==3 )
	{
	  cout<<"ZZ OPERATOR : "<<endl;
	  errors(zzDerivative,i) = runTest(zzDerivative, mg, u, ogf,gip);
	}

      if ( dtest=="" || dtest=="XY" )
	{
	  cout<<"XY OPERATOR : "<<endl;
	  errors(xyDerivative,i) = runTest(xyDerivative,mg,u,ogf,gip);
	}

      if ( (dtest=="" || dtest=="XZ") && mg.numberOfDimensions()==3 )
	{
	  cout<<"XZ OPERATOR : "<<endl;
	  errors(xzDerivative,i) = runTest(xzDerivative, mg, u, ogf,gip);
	}

      if ( (dtest=="" || dtest=="YZ") && mg.numberOfDimensions()==3 )
	{
	  cout<<"YZ OPERATOR : "<<endl;
	  errors(yzDerivative,i) = runTest(yzDerivative, mg, u, ogf,gip);
	}

      if ( dtest=="" || dtest=="LAPLACIAN" )
	{
	  cout<<"LAPLACIAN OPERATOR : "<<endl;
	  errors(laplacianOperator,i) = runTest(laplacianOperator, mg, u, ogf,gip);
	}

//       cout<<"DIVERGENCE OPERATOR : "<<endl;
//       errors(divergence,i) = runTest(divergence, mg, uvec, ogf,gip);

      if ( dtest=="" || dtest=="GRADIENT" )
	{
	  cout<<"GRADIENT OPERATOR : "<<endl;
	  errors(gradient,i) = runTest(gradient, mg, u, ogf,gip);
	}
    }


  Overture::finish();
  
  return 0;
}
