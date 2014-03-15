// #define BOUNDS_CHECK
#define OV_DEBUG
#include "MeshQuality.h"
#include "InterpolatePoints.h"
#include "GL_GraphicsInterface.h"
#include "ArraySimple.h"

ArraySimpleFixed<real,3,3,1,1> 
IdentityMetricEvaluator::computeMetric(const ArraySimpleFixed<real,3,1,1,1> &x)
{
  ArraySimpleFixed<real,3,3,1,1> T;
  T = 0;
  T(0,0)=T(1,1)=T(2,2)=1;
  return T;
}

ArraySimpleFixed<real,3,3,1,1> 
IdentityMetricEvaluator::computeMetric(int id)
{
  ArraySimpleFixed<real,3,3,1,1> T;
  T = 0;
  T(0,0)=T(1,1)=T(2,2)=1;
  return T;
}

ArraySimpleFixed<real,3,3,1,1> 
IdentityMetricEvaluator::computeMetric(void *entity)
{
  ArraySimpleFixed<real,3,3,1,1> T;
  T = 0;
  T(0,0)=T(1,1)=T(2,2)=1;
  return T;
}

ArraySimpleFixed<real,3,3,1,1> 
MetricCGFunctionEvaluator::computeMetric(const ArraySimpleFixed<real,3,1,1,1> &x)
{
  ArraySimpleFixed<real,3,3,1,1> T;
  T = 0;

  if ( cgf )
    {
      if ( cgf->getCompositeGrid()->numberOfDimensions()==2 )
	{
	  ArraySimpleFixed<real,2,1,1,1> x2d;
	  ArraySimpleFixed<real,2,2,1,1> T2d;

	  T2d=0;
	  x2d[0] = x[0];
	  x2d[1] = x[1];
	  interpolateFromControlFunction(x2d,T2d,*cgf);
	  for ( int i=0; i<2; i++ )
	    for ( int j=0; j<2; j++ )
	      T(i,j) = T2d(i,j);
	}
      else
	{
	  interpolateFromControlFunction(x,T,*cgf);
	}
    }
  else
    {
      ArraySimpleFixed<real,3,3,1,1> T;
      T = 0;
      T(0,0)=T(1,1)=T(2,2)=1;
    }

  return T;
}

ArraySimpleFixed<real,3,3,1,1> 
MetricCGFunctionEvaluator::computeMetric(int id)
{
  ArraySimpleFixed<real,3,3,1,1> T;
  T = 0;
  T(0,0)=T(1,1)=T(2,2)=1;
  return T;
}

ArraySimpleFixed<real,3,3,1,1> 
MetricCGFunctionEvaluator::computeMetric(void *entity)
{
  ArraySimpleFixed<real,3,3,1,1> T;
  T = 0;
  T(0,0)=T(1,1)=T(2,2)=1;
  return T;
}

MeshQualityMetrics::
MeshQualityMetrics() : umap(NULL), referenceTransformation(NULL) { }

MeshQualityMetrics::
MeshQualityMetrics(UnstructuredMapping &umap_) : umap(&umap_), referenceTransformation(NULL) { }

const realArray &
MeshQualityMetrics::
computeMetric(MeshQualityMetric metric)
{
  if ( umap==NULL ) 
    return metrics[int(metric)];

  //  computeJacobianProperties();

  int rangeDim = umap->getRangeDimension();
  int nElems = umap->size(UnstructuredMapping::EntityTypeEnum(umap->getDomainDimension()));

  UnstructuredMapping::EntityTypeEnum etype = UnstructuredMapping::EntityTypeEnum(umap->getDomainDimension());

  UnstructuredMappingIterator elem,elem_end;
  UnstructuredMappingAdjacencyIterator aiter, aiter_end;

  ArraySimpleFixed<real,3,1,1,1> xc,xx[8];

  realArray & m = metrics[int(metric)];
  if ( m.getLength(0)!=nElems )
    m.redim(nElems);

  if ( jacobianProperties.getLength(0)==0 )
    jacobianProperties.redim(nElems,int(numberOfProperties));

  const realArray &verts = umap->getNodes();

  elem_end = umap->end(etype);
  real N2,det,K;
  for ( elem=umap->begin(etype); elem!=elem_end; elem++ )
    {
      int e = *elem;
      aiter_end = umap->adjacency_end(elem,UnstructuredMapping::Vertex);
      xc=0.;
      int nv = 0;
      for ( aiter=umap->adjacency_begin(elem,UnstructuredMapping::Vertex); aiter!=aiter_end; aiter++ )
	{
	  int v = *aiter;
	  for ( int a=0; a<rangeDim; a++ )
	    {
	      xc[a] += verts(v,a);
	      xx[nv][a] = verts(v,a);
	    }
	  nv++;
	}
      
      for ( int a=0; a<rangeDim; a++ )
	xc[a] /= real(nv);
      
      if ( rangeDim==2 )
	{
	  ArraySimpleFixed<real,2,2,1,1> T,J;
	  ArraySimpleFixed<real,2,1,1,1>  xc2,xx2[4];
	  for ( int a=0; a<rangeDim; a++ )
	    {
	      xc2[a] = xc[a];
	      for ( int n=0; n<nv; n++ )
		xx2[n][a] = xx[n][a];
	    }

	  T = computeWeight(xc2,umap->computeElementType(elem.getType(),e));

	  switch ( umap->computeElementType(elem.getType(),e) ) {
	  case UnstructuredMapping::triangle:
	    {
	      J=computeJacobian(xx2[0],xx2[1],xx2[2],T);
	      break;
	    }
	  case UnstructuredMapping::quadrilateral:
	    {
	      J=computeJacobian(xx2[0],xx2[1],xx2[2],xx2[3],T);
	      break;
	    }
	  default:
	    break;
	  }

	  computeJacobianProperties(N2,det,K,J);
	}
      else
	{
	  ArraySimpleFixed<real,3,3,1,1> T,J;

	  T = computeWeight(xc,umap->computeElementType(elem.getType(),e));

	  switch ( umap->computeElementType(elem.getType(),e) ) {
	  case UnstructuredMapping::tetrahedron:
	    {
	      J=computeJacobian(xx[0],xx[1],xx[2],xx[3],T);
	      break;
	    }
	  case UnstructuredMapping::pyramid:
	    {
	      J=computeJacobian(xx[0],xx[1],xx[2],xx[3],xx[4],T);
	      break;
	    }
	  case UnstructuredMapping::triPrism:
	    {
	      //	      J=computeJacobian(xx[0],xx[1],xx[2],xx[3],xx[4],xx[5],T);
	      abort();
	      break;
	    }
	  case UnstructuredMapping::hexahedron:
	    {
	      J=computeJacobian(xx[0],xx[1],xx[2],xx[3],xx[4],xx[5],xx[6],xx[7],T);
	      break;
	    }
	  default:
	    break;
	  }
	  computeJacobianProperties(N2,det,K,J);
	  
	}

      jacobianProperties(e,int(normSquared)) = N2;
      jacobianProperties(e,int(determinant)) = det;
      jacobianProperties(e,int(conditionNum)) = K;
      
      if ( metric==volumeMetric )
	{
	  m(e) = min(jacobianProperties(e,int(determinant)),1./jacobianProperties(e,int(determinant)));
	}
      else if ( metric==shapeMetric )
	{
	    m(e) = real(rangeDim)/jacobianProperties(e,int(conditionNum));
	}
      else if ( metric==volumeShapeMetric )
	{
	    {
	      real t = jacobianProperties(e,int(determinant));
	      m(e) = min(t,1./t)*real(rangeDim)/jacobianProperties(e,int(conditionNum));
	    }
	}
    }

  return metrics[int(metric)];
}

void 
MeshQualityMetrics::
setReferenceTransformation(MetricEvaluator *rt)
{
  //  if ( referenceTransformation!=NULL )
  //    delete referenceTransformation;

  referenceTransformation = rt;
}

void
MeshQualityMetrics::
plot(GL_GraphicsInterface &gi) 
{ 
  if ( umap==NULL ) return;

  GraphicsParameters gp;
  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT, true);
  gp.set(GI_MAPPING_COLOUR, "black");
  gp.set(GI_PLOT_UNS_EDGES,true);
  gp.set(GI_PLOT_UNS_BOUNDARY_EDGES,true);
  gp.set(GI_PLOT_UNS_FACES,true);
  int nContourLevels = gp.get(GI_NUMBER_OF_CONTOUR_LEVELS, nContourLevels);
  GUIState interface;

  interface.setWindowTitle("Mesh Metrics");
  interface.setExitCommand("Exit","Exit");

  aString tbCmds[] = { "Plot Mesh", "" };
  bool plotMesh = true;
  int tbState[] = {int(plotMesh)};
  interface.setToggleButtons(tbCmds,tbCmds,tbState,1);

  aString rbCmds[] = { "Volume Metric", "Shape Metric", "Combined Metric", "None", "" };
  interface.addRadioBox("Metric to Plot", rbCmds, rbCmds, int(numberOfProperties), 1);

  //  MeshQualityMetric metricToPlot=numberOfQualityMetrics;
  MeshQualityMetric metricToPlot=shapeMetric;

  aString answer;

  gi.pushGUI(interface);
  int i=0;

  for (;;)
    {
      
      if ( i!=0 )
	gi.getAnswer(answer,"");
      else
	{
	  i++;
	  answer=="";
	}
      
      if ( answer.matches("Exit") )
	break;
      else if ( answer.matches("Plot Mesh") )
	{
	  plotMesh = !plotMesh;
	  interface.setToggleState(0,plotMesh);
	}
      else if ( answer.matches("Volume Metric") )
	{
	  metricToPlot = volumeMetric;
	}
      else if ( answer.matches("Shape Metric") )
	{
	  metricToPlot = shapeMetric;
	}
      else if ( answer.matches("Combined Metric") )
	{
	  metricToPlot = volumeShapeMetric;
	}
      else if ( answer.matches("None") )
	{
	  metricToPlot = numberOfQualityMetrics;
	}

      gi.erase();
      gi.setAxesDimension(umap->getRangeDimension());
      // now plot each element coloured by its metric value
      if ( metricToPlot != numberOfQualityMetrics && umap->getRangeDimension()==2 )
	{
	  const realArray & m = computeMetric(metricToPlot);
	  int nElems = umap->getNumberOfElements();
	  const realArray &xyz = umap->getNodes();
	  const intArray &elems = umap->getElements();

	  real uMin = min(m);
	  real uMax = max(m);
	  
	  real deltaU = uMax-uMin;
	  cout<<"uMin, uMax "<<uMin<<" "<<uMax<<endl;

	  if ( deltaU==0. )
	    {
	      uMax += 0.5;
	      uMin -= 0.5;
	      deltaU = uMax-uMin;
	    }

	  real deltaUInverse = deltaU==0. ? 1. : 1./deltaU;

	  int dlist = gi.generateNewDisplayList();
	  assert(dlist!=0);

	  glNewList(dlist,GL_COMPILE);

	  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	  glShadeModel(GL_SMOOTH);
	  glEnable(GL_POLYGON_OFFSET_FILL);
	  real dum;
	  glPolygonOffset(1.,gp.get(GI_SURFACE_OFFSET, dum)*OFFSET_FACTOR);  

	  for ( int e=0; e<nElems; e++ )
	    {
	      glBegin(GL_POLYGON);
	      for ( int n=0; n<umap->getNumberOfNodesThisElement(e); n++ )
		{
		  gi.setColourFromTable( (m(e)-uMin)*deltaUInverse, gp );
		  glVertex3(xyz(elems(e,n),0),xyz(elems(e,n),1), 0.);
		}
	      glEnd();
	    }
	  glEndList();
	  RealArray nullArray;
	  gi.displayColourBar(nContourLevels, nullArray, uMin,uMax,gp);
	  //gi.redraw();
	}
      else if ( metricToPlot != numberOfQualityMetrics && umap->getRangeDimension()==3 )
	{
	  const realArray & m = computeMetric(metricToPlot);
	  int nElems = umap->getNumberOfElements();
	  const realArray &xyz = umap->getNodes();
	  const intArray &faceElems = umap->getFaceElements();
	  const intArray &elems = umap->getElements();
	  const intArray &face = umap->getFaces();

	  real uMin = min(m);
	  real uMax = max(m);
	  
	  real deltaU = uMax-uMin;
	  cout<<"uMin, uMax "<<uMin<<" "<<uMax<<endl;
	  if ( deltaU==0. )
	    {
	      uMax += 0.5;
	      uMin -= 0.5;
	      deltaU = uMax-uMin;
	    }

	  real deltaUInverse = deltaU==0. ? 1. : 1./deltaU;
	  int dlist = gi.generateNewDisplayList(0);
	  glNewList(dlist,GL_COMPILE);
	  glDisable(GL_POLYGON_OFFSET_FILL);
	  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);  // plot lines on surface for a wireframe

	  int i,n;
	  //int numberOfNodesPerElement=3;
	  real x1[3],x2[3], normal[3];
	  bool plotable = false;
	  int rangeDim = umap->getRangeDimension();
	  for( int f=0; f<umap->getNumberOfFaces(); f++ )
	    {
	      int nnod = umap->getNumberOfNodesThisFace(f);
	      int n;
	      //jacobianProperties(e,int(determinant))
	      plotable = false;
	      bool setColour0 = false;

	      if ( faceElems(f,0) != -1 )
		plotable = setColour0 = m(faceElems(f,0))<=0.2;
	      if ( faceElems(f,1) != -1 )
		plotable = plotable || m(faceElems(f,1))<=0.2;

	      if ( plotable )
		{
		  //cout<<"plotting face "<<f<<endl;
		  glBegin(GL_POLYGON);  // draw shaded filled polygons
		  if (setColour0)
		    gi.setColourFromTable( (m(faceElems(f,0))-uMin)*deltaUInverse, gp );
		  else
		    gi.setColourFromTable( (m(faceElems(f,1))-uMin)*deltaUInverse, gp );
					   
		  for( n=0; n<umap->getNumberOfNodesThisFace(f); n++ )
		    {
		      int mm=face(f,n);
		      // printf(" i=%i n=%i m=%i\n",i,n,m);
		      //assert( m>=0 && m<numberOfNodes && numAdjFaces(m)>0);
		    
		      //glNormal3v(normal);
		      glVertex3(xyz(mm,0),xyz(mm,1),xyz(mm,2));
		    }
		  //m=element(0,i);
		  //glVertex3(x(m,0),x(m,1),0.);
		  glEnd();    
		}
	    
	    }
	  glEndList();
	  RealArray nullArray;
	  gi.displayColourBar(nContourLevels, nullArray, uMin,uMax,gp);
	  //gi.redraw();

	}

      if ( plotMesh ) 
	PlotIt::plot(gi,*umap,gp);

      //      gi.redraw();
    }

  gi.popGUI();
}

void
MeshQualityMetrics::
outputHistogram(aString fileName)
{
  if ( umap==NULL ) return;

  real maxm = -REAL_MAX;
  real minm = REAL_MAX;

  int nbin = 10;

  real db = 1./real(nbin);
  //  const realArray & m = computeMetric(volumeShapeMetric);
  const realArray & m = computeMetric(shapeMetric);
  ArraySimple<int> count(nbin);

  int b;
  for ( b=0; b<nbin; b++ )
    count[b] = 0;

  int e;
  for ( e=0; e<umap->getNumberOfElements(); e++ )
    {
      count[ min(int(m(e)/db),nbin-1) ]++;
      maxm = max(maxm, m(e));
      minm = min(minm, m(e));
    }

  for ( b=0; b<nbin; b++ )
    cout<<(b+0.5)*db<<" "<<count[b]<<endl;
    
  cout<<"max = "<<maxm<<" min = "<<minm<<endl;

  real avgEdge=0, minEdge=REAL_MAX, maxEdge=0.;
  real elen;
  Range AXES(0,umap->getRangeDimension()-1);
  const realArray &xyz = umap->getNodes();
  const intArray &edge = umap->getEdges();
  for ( e=0; e<umap->getNumberOfEdges(); e++ )
    {
      elen = sqrt(sum(pow(xyz(edge(e,1),AXES)-xyz(edge(e,0),AXES),2)));
      avgEdge += elen;
      minEdge = min(minEdge,elen);
      maxEdge = max(maxEdge,elen);
    }

  cout<<"averge edge length = "<<avgEdge/umap->getNumberOfEdges()<<"\n";
  cout<<"min edge length    = "<<minEdge<<"\n";
  cout<<"max edge length    = "<<maxEdge<<endl;

}

void 
MeshQualityMetrics::
computeJacobianProperties()
{
  if ( umap==NULL ) return;
  
  int rangeDim = umap->getRangeDimension();

  int nElems = umap->getNumberOfElements();
  //realArray jac(nElems, rangeDim, rangeDim);
  const realArray &xyz = umap->getNodes();
  const intArray &elems = umap->getElements();

  jacobianProperties.redim(nElems, numberOfProperties);
  int nj = umap->getMaxNumberOfNodesPerElement();
  jacobians.redim(nElems,nj,rangeDim,rangeDim);

  ArraySimpleFixed<real,3,1,1,1> x0;
  ArraySimpleFixed<real,3,3,1,1> W,Winv;
  realArray T(3,3);
  T=0.;
  T(0,0) = T(1,1) = T(2,2) = 1.;

  realArray xc(1,rangeDim);
  InterpolatePoints interpolator;
  int nbad = 0;
  int ntet = 0;
  for ( int e=0; e<nElems; e++ )
    {
      
      int nnodes = umap->getNumberOfNodesThisElement(e);
	  
      // compute the weighting matrix from the element type 
      //  and the reference transformation
      //  (tets and triangles have adjustments based on thier shape)
      if ( referenceTransformation!=NULL )
	{
	  ArraySimpleFixed<real,3,1,1,1> xc;
	  ArraySimpleFixed<real,3,3,1,1> TT;
	  xc=0;
	  for ( int p=0; p<nnodes; p++ )
	    for ( int a=0; a<rangeDim; a++ )
	      xc(a) += xyz(elems(e,p),a)/real(nnodes);
	    
	  TT = referenceTransformation->computeMetric(xc);
	  for ( int a=0; a<rangeDim; a++ )
	    for ( int aa=0; aa<rangeDim; aa++ )
	      T(a,aa) =TT(a,aa);
	  
#if 0
	  // compute the element center
	  if ( rangeDim==2 )
	    {
	      ArraySimpleFixed<real,2,1,1,1> xc;
	      ArraySimpleFixed<real,2,2,1,1> Tt;

	      xc[0] = xc[1] = 0.;
	      for ( int p=0; p<nnodes; p++ )
		for ( int a=0; a<rangeDim; a++ )
		  xc(a) += xyz(elems(e,p),a)/real(nnodes);

	      //	      interpolateFromControlFunction ( xc, Tt, *referenceTransformation );

	      for ( int a=0; a<rangeDim; a++ )
		for ( int aa=0; aa<rangeDim; aa++ )
		  T(a,aa) =Tt(a,aa);
	      
	    }
	  else
	    {
	      ArraySimpleFixed<real,3,1,1,1> xc;
	      ArraySimpleFixed<real,3,3,1,1> Tt;

	      xc[0] = xc[1] = xc[2] = 0.;
	      for ( int p=0; p<nnodes; p++ )
		for ( int a=0; a<rangeDim; a++ )
		  xc(a) += xyz(elems(e,p),a)/real(nnodes);

	      //	      interpolateFromControlFunction ( xc, Tt, *referenceTransformation );

	      for ( int a=0; a<rangeDim; a++ )
		for ( int aa=0; aa<rangeDim; aa++ )
		  T(a,aa) =Tt(a,aa);
	    }
#endif
	  //	  T.reshape(1,9);
	  //	  interpolator.interpolatePoints(xc, *referenceTransformation, T);
	  //	  T.reshape(3,3);
	}
#if 0
      W(0,0) = 1.; W(0,1) =     W(0,2) = 0.;
      W(1,0) = 0.; W(1,1) = 1.; W(1,2) = 0.;
      W(2,0) =     W(2,1) = 0.; W(2,2) = 1.;

      if ( false && umap->getElementType(e)==UnstructuredMapping::triangle )
	{
	  Winv(0,0) = T(0,0) - T(1,0)/sqrt(3.);
	  Winv(0,1) = T(0,1) - T(1,1)/sqrt(3.);
	  Winv(1,0) = 2*T(1,0)/sqrt(3.);
	  Winv(1,1) = 2*T(1,1)/sqrt(3.);
	}
      else if ( false && umap->getElementType(e)==UnstructuredMapping::tetrahedron )
	{
	  Winv(0,0) = T(0,0) - T(1,0)/sqrt(3.) - T(2,0)/sqrt(3.)/sqrt(2.);
	  Winv(0,1) = T(0,1) - T(1,1)/sqrt(3.) - T(2,1)/sqrt(3.)/sqrt(2.);
	  Winv(0,2) = T(0,2) - T(1,2)/sqrt(3.) - T(2,2)/sqrt(3.)/sqrt(2.);

	  Winv(1,0) = 2*T(1,0)/sqrt(3.) - T(2,0)/sqrt(3.)/sqrt(2.);
	  Winv(1,1) = 2*T(1,1)/sqrt(3.) - T(2,1)/sqrt(3.)/sqrt(2.);
	  Winv(1,2) = 2*T(1,2)/sqrt(3.) - T(2,2)/sqrt(3.)/sqrt(2.);

	  Winv(2,0) = sqrt(3.)*T(2,0)/sqrt(2.);
	  Winv(2,1) = sqrt(3.)*T(2,1)/sqrt(2.);
	  Winv(2,2) = sqrt(3.)*T(2,2)/sqrt(2.);
	  ntet++;
	}
      else if ( umap->getElementType(e)==UnstructuredMapping::pyramid )
	{
	  Winv(0,0) = T(0,0)*3./2;
	  Winv(0,1) = T(0,1)*3./2;
	  Winv(0,2) = T(0,2);

	  Winv(1,0) = T(1,0)*3./2;
	  Winv(1,1) = T(1,1)*3./2;
	  Winv(1,2) = T(1,2)/2.;

	  Winv(2,0) = T(2,0)*3./2;
	  Winv(2,1) = T(2,1)*3./2;
	  Winv(2,2) = T(2,2);

	}
      else
	{
	  for ( int r=0; r<rangeDim; r++ )
	    for ( int c=0; c<rangeDim; c++ )
	      Winv(r,c) = T(r,c);
	}
#endif

      int n0 = 0;
      real shapeMetric = 0.;
      
      for ( int n=0; n<nnodes; n++ )
	{
	  real normSQ;
	  real det;
	  real cNum;

	  int r,c;

	  if ( rangeDim==2 )
	    {
	      
	      ArraySimpleFixed<real,2,1,1,1> x0, x1, x2, x3;
	      ArraySimpleFixed<real,2,2,1,1> jac,T;

	      for ( r=0; r<rangeDim; r++ )
		for ( c=0; c<rangeDim; c++ )
		  T(r,c) = Winv(r,c);
	      
	      if ( umap->getElementType(e)==UnstructuredMapping::triangle )
		{
		  for ( r=0; r<rangeDim; r++ )
		    {
		      x0[r] = xyz(elems(e,n),r);
		      x1[r] = xyz(elems(e,(n+1)%nnodes),r);
		      x2[r] = xyz(elems(e,(n-1+nnodes)%nnodes),r);
		    }
		  
		  jac = computeJacobian(x0,x1,x2,T);
		}
	      else
		{
		  for ( r=0; r<rangeDim; r++ )
		    {
		      x0[r] = xyz(elems(e,n),r);
		      x1[r] = xyz(elems(e,(n+1)%nnodes),r);
		      x2[r] = xyz(elems(e,(n+2)%nnodes),r);
		      x3[r] = xyz(elems(e,(n+3)%nnodes),r);
		    }

		  jac = computeJacobian(x0,x1,x2,x3,T);
		}

	      computeJacobianProperties(normSQ, det, cNum, jac);

	      for ( r=0; r<rangeDim; r++ )
		for ( c=0; c<rangeDim; c++ )
		  jacobians(e,n,r,c) = jac(r,c);
	      
	    }
	  else
	    {
	      ArraySimpleFixed<real,3,1,1,1> x0, x1, x2, x3;
	      ArraySimpleFixed<real,3,3,1,1> jac,T;

	      for ( int a=0; a<rangeDim; a++ )
		x0[a] = xyz(elems(e,n),a);

	      for ( r=0; r<rangeDim; r++ )
		for ( c=0; c<rangeDim; c++ )
		  T(r,c) = Winv(r,c);

	      if ( umap->getElementType(e)==UnstructuredMapping::tetrahedron )
		{
		  if ( n%2==0 )
		    for ( r=0; r<rangeDim; r++ )
		      {
			x1[r] = xyz(elems(e,(n+1)%nnodes),r);
			x2[r] = xyz(elems(e,(n+2)%nnodes),r);
			x3[r] = xyz(elems(e,(n+3)%nnodes),r);
		      }
		  else
		    for ( r=0; r<rangeDim; r++ )
		      {
			x1[r] = xyz(elems(e,(n+1)%nnodes),r);
			x2[r] = xyz(elems(e,(n+3)%nnodes),r);
			x3[r] = xyz(elems(e,(n+2)%nnodes),r);
		      }
		  jac = computeJacobian(x0,x1,x2,x3,T);
		  computeJacobianProperties(normSQ, det, cNum, jac);
		  
		}
	      else if ( umap->getElementType(e)==UnstructuredMapping::pyramid )
		{
		  // approximate the pyramid jacobian as a degenerate hex, using a FD for the term
		  //  ( then Djac/Dxi = { {-2/3,-2/3,-1/2 | -2},{0},{0} }
		  ArraySimpleFixed<real,3,3,1,1> Jt;

		  Jt(0,0) = -( xyz(elems(e,0),0)+xyz(elems(e,3),0)+xyz(elems(e,4),0)-
			       (xyz(elems(e,1),0)+xyz(elems(e,2),0)+xyz(elems(e,4),0)))/3.;
		  Jt(1,0) = -( xyz(elems(e,0),1)+xyz(elems(e,3),1)+xyz(elems(e,4),1)-
			       (xyz(elems(e,1),1)+xyz(elems(e,2),1)+xyz(elems(e,4),1)))/3.;
		  Jt(2,0) = -( xyz(elems(e,0),2)+xyz(elems(e,3),2)+xyz(elems(e,4),2)-
			       (xyz(elems(e,1),2)+xyz(elems(e,2),2)+xyz(elems(e,4),2)))/3.;

		  Jt(0,1) = -( xyz(elems(e,0),0)+xyz(elems(e,1),0)+xyz(elems(e,4),0)-
			       (xyz(elems(e,2),0)+xyz(elems(e,3),0)+xyz(elems(e,4),0)))/3.;
		  Jt(1,1) = -( xyz(elems(e,0),1)+xyz(elems(e,1),1)+xyz(elems(e,4),1)-
			       (xyz(elems(e,2),1)+xyz(elems(e,3),1)+xyz(elems(e,4),1)))/3.;
		  Jt(2,1) = -( xyz(elems(e,0),2)+xyz(elems(e,1),2)+xyz(elems(e,4),2)-
			       (xyz(elems(e,2),2)+xyz(elems(e,3),2)+xyz(elems(e,4),2)))/3.;

		  Jt(0,2) = -2*( (xyz(elems(e,0),0)+xyz(elems(e,1),0)+
				   xyz(elems(e,2),0)+xyz(elems(e,3),0))/4.-
				  (xyz(elems(e,4),0)));
		  Jt(1,2) = -2*( (xyz(elems(e,0),1)+xyz(elems(e,1),1)+xyz(elems(e,2),1)+
				   xyz(elems(e,3),1))/4.-
				  (xyz(elems(e,4),1)));
		  Jt(2,2) = -2*( (xyz(elems(e,0),2)+xyz(elems(e,1),2)+xyz(elems(e,2),2)+
				   xyz(elems(e,3),2))/4.-
				  (xyz(elems(e,4),2)));

		  int r,c,cc;
		  for ( r=0; r<3; r++ )
		    for ( c=0; c<3; c++ )
		      jac(r,c) =0.;
		  
		  for ( r=0; r<3; r++ )
		    for ( c=0; c<3; c++ )
		      for ( cc=0; cc<3; cc++ )
			jac(r,c) += Jt(r,cc)*Winv(cc,c);

		  computeJacobianProperties(normSQ, det, cNum, jac);

		  		  
		}
	      else if ( umap->getElementType(e)==UnstructuredMapping::hexahedron )
		{
		  // use a second order FD approximation to the jacobian
		  // at the cell center for hexes ( then Djac/Dxi = { {-.25,-.25,-.25},{0},{0} } )
		  ArraySimpleFixed<real,3,3,1,1> Jt;
		  
		  Jt(0,0) = -( xyz(elems(e,0),0)+xyz(elems(e,3),0)+xyz(elems(e,4),0)+xyz(elems(e,7),0)-
			       (xyz(elems(e,1),0)+xyz(elems(e,2),0)+xyz(elems(e,5),0)+xyz(elems(e,6),0)))/4.;
		  Jt(1,0) = -( xyz(elems(e,0),1)+xyz(elems(e,3),1)+xyz(elems(e,4),1)+xyz(elems(e,7),1)-
			       (xyz(elems(e,1),1)+xyz(elems(e,2),1)+xyz(elems(e,5),1)+xyz(elems(e,6),1)))/4.;
		  Jt(2,0) = -( xyz(elems(e,0),2)+xyz(elems(e,3),2)+xyz(elems(e,4),2)+xyz(elems(e,7),2)-
			       (xyz(elems(e,1),2)+xyz(elems(e,2),2)+xyz(elems(e,5),2)+xyz(elems(e,6),2)))/4.;

		  Jt(0,1) = -( xyz(elems(e,0),0)+xyz(elems(e,1),0)+xyz(elems(e,4),0)+xyz(elems(e,5),0)-
			       (xyz(elems(e,2),0)+xyz(elems(e,3),0)+xyz(elems(e,6),0)+xyz(elems(e,7),0)))/4.;
		  Jt(1,1) = -( xyz(elems(e,0),1)+xyz(elems(e,1),1)+xyz(elems(e,4),1)+xyz(elems(e,5),1)-
			       (xyz(elems(e,2),1)+xyz(elems(e,3),1)+xyz(elems(e,6),1)+xyz(elems(e,7),1)))/4.;
		  Jt(2,1) = -( xyz(elems(e,0),2)+xyz(elems(e,1),2)+xyz(elems(e,4),2)+xyz(elems(e,5),2)-
			       (xyz(elems(e,2),2)+xyz(elems(e,3),2)+xyz(elems(e,6),2)+xyz(elems(e,7),2)))/4.;

		  Jt(0,2) = -( xyz(elems(e,0),0)+xyz(elems(e,1),0)+xyz(elems(e,2),0)+xyz(elems(e,3),0)-
			       (xyz(elems(e,4),0)+xyz(elems(e,5),0)+xyz(elems(e,6),0)+xyz(elems(e,7),0)))/4.;
		  Jt(1,2) = -( xyz(elems(e,0),1)+xyz(elems(e,1),1)+xyz(elems(e,2),1)+xyz(elems(e,3),1)-
			       (xyz(elems(e,4),1)+xyz(elems(e,5),1)+xyz(elems(e,6),1)+xyz(elems(e,7),1)))/4.;
		  Jt(2,2) = -( xyz(elems(e,0),2)+xyz(elems(e,1),2)+xyz(elems(e,2),2)+xyz(elems(e,3),2)-
			       (xyz(elems(e,4),2)+xyz(elems(e,5),2)+xyz(elems(e,6),2)+xyz(elems(e,7),2)))/4.;

		  int r,c,cc;
		  for ( r=0; r<3; r++ )
		    for ( c=0; c<3; c++ )
		      jac(r,c) =0.;
		  
		  for ( r=0; r<3; r++ )
		    for ( c=0; c<3; c++ )
		      for ( cc=0; cc<3; cc++ )
			jac(r,c) += Jt(r,cc)*Winv(cc,c);
		  
		  computeJacobianProperties(normSQ, det, cNum, jac);
		  
		}
	      else
		{
		  jac(0,0)=jac(1,1)=jac(2,2)=1.;
		  jac(0,1)=jac(1,0)=jac(1,2)=jac(2,1)=jac(0,2)=jac(2,0) = 0.;
		  
		  normSQ = 3;
		  det = 1;
		  cNum = 3;
		  // 	      computeJacobianProperties(normSQ, det, cNum, jac);
		  for ( r=0; r<rangeDim; r++ )
		    for ( c=0; c<rangeDim; c++ )
		      jacobians(e,n,r,c) = jac(r,c);
		}
	      
	      for ( r=0; r<rangeDim; r++ )
		for ( c=0; c<rangeDim; c++ )
		  jacobians(e,n,r,c) = jac(r,c);
	      
	    }

	  //	  if ( det<0. )
	  //	    {
	  //      nbad++;
	  //	      cout<<"BAD DET : "<<det<<" "<<e<<endl;
// 	      cout<<jacobians(e,n,0,0)<<" "<<jacobians(e,n,0,1)<<" "<<jacobians(e,n,0,2)<<endl;
// 	      cout<<jacobians(e,n,1,0)<<" "<<jacobians(e,n,1,1)<<" "<<jacobians(e,n,1,2)<<endl;
// 	      cout<<jacobians(e,n,2,0)<<" "<<jacobians(e,n,2,1)<<" "<<jacobians(e,n,2,2)<<endl;
	      //	      if ( umap->getElementType(e)==UnstructuredMapping::pyramid ) cout<<"PYRAMID"<<endl;
	      //	      cout<<"----"<<endl;
	      //	    }


	  if ( rangeDim/cNum > shapeMetric )
	    {
	      n0 = n;
	      shapeMetric = rangeDim/cNum;
	      jacobianProperties(e,int(normSquared)) = normSQ;
	      jacobianProperties(e,int(determinant)) = det;
	      jacobianProperties(e,int(conditionNum))= cNum;
	    }
	}
      //cout<<"k is "<<jacobianProperties(e,int(conditionNum))<<endl;
    }
  cout<<"number of bad elements is "<<nbad/4<<endl;
  cout<<"number of tets is "<<ntet<<endl;
  cout<<"number of elements is "<<nElems<<endl;
}
