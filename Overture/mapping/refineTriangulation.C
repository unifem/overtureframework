// #define BOUNDS_CHECK
#define OV_DEBUG

#include "Mapping.h"
#include "rap.h"
#include "TriangleWrapper.h"
#include "UnstructuredMapping.h"
#include "CompositeSurface.h"
#include "CompositeTopology.h"
#include "GenericGraphicsInterface.h"
#include "MappingProjectionParameters.h"
#include "PlotIt.h"


using namespace std;

#define ELEMENTS

int rapOpenModel(GenericGraphicsInterface &gi, GraphicsParameters &gp, CompositeSurface &model);
int rapOpenModel(aString modelFileName, CompositeSurface &model);

realArray evaluateDeviation(CompositeSurface &model);

real refineVisibleSurfaces(GenericGraphicsInterface &gi,
			   CompositeSurface &model,
			   real absoluteTol);

bool scaleNodes(Mapping &referenceSurface, realArray &rc, bool collapsedEdge[2][3], bool isTrimmedMapping, real rai)
{
  // scale the nodes to ajust for a collapsed edge
  // this code is taken from compositeTopology.C:buildSubSurfaceTriangulation
  //   it must match the code there


  Range all,R;

#if 0
  real ra= rai;
  const real /*ra=.05,*/ rba=1.-ra;
	      
  // real rShift = s==0 ? .25 : .75;
  real rShift=.5;
  if( collapsedEdge[0][0] || collapsedEdge[1][0] || collapsedEdge[0][1] || collapsedEdge[1][1] )
  {
#if 0
    if( isTrimmedMapping )
    {
      // On a TrimmedMapping we guess the r location of the singular point
      //  *** this could be done better ***
      if( collapsedEdge[0][1] || collapsedEdge[1][1] )
      {
        // rShift=average value or r or s .5*( min + max ) ? 
	Range E=numberOfEdgePoints;  // don't count extra interior nodes.
        rShift=.5*( min(rc(E,0))+max(rc(E,0)) );
        printf(" ** guess singular location to be r0=%e\n",rShift);
      }
      else
      {
	Range E=numberOfEdgePoints;  // don't count extra interior nodes.
        rShift=.5*( min(rc(E,1))+max(rc(E,1)) );
        printf(" ** guess singular location to be r1=%e\n",rShift);
      }
    }
#endif
    if( collapsedEdge[0][1] )
    {
      // bottom collapsed -- apply a scaling function to the nodes:
      printf(" ** bottom collapsed, scale nodes to get better triangles shapes\n");

      rc(R,0) = (rc(R,0)-rShift)*(ra + rba*rc(R,1));
      //rc(R,0) *= ra + rba*rc(R,1);
//       if( numberOfInnerCurves>0 )
// 	holes(all,0) = (holes(all,0)-rShift)*(ra + rba*holes(all,1));
      // holes(all,0)*=ra + rba*holes(all,1);
    }
    else if( collapsedEdge[1][1] )
    {
      // top collapsed -- apply a scaling function to the nodes:
      printf(" ** top collapsed, scale nodes to get better triangles shapes\n");
      rc(R,0) = (rc(R,0)-rShift)*(1.-rba*rc(R,1));
      // rc(R,0) *= 1.-rba*rc(R,1);
//       if( numberOfInnerCurves>0 )
// 	holes(all,0) = (holes(all,0)-rShift)*(1.-rba*holes(all,1));
      // holes(all,0)*=1.-rba*holes(all,1);
    }
    else if( collapsedEdge[0][0] )
    {
      printf(" ** left collapsed, scale nodes to get better triangles shapes\n");
      rc(R,1) = (rc(R,1)-rShift)*(ra + rba*rc(R,0));
      // rc(R,1) *= ra + rba*rc(R,0);
//       if( numberOfInnerCurves>0 )
// 	holes(all,1) = (holes(all,1)-rShift)*(ra + rba*holes(all,0));
      // holes(all,1)*=ra + rba*holes(all,0);
    }
    else if( collapsedEdge[1][0] )
    {
      printf(" ** right collapsed, scale nodes to get better triangles shapes\n");
      rc(R,1) = (rc(R,1)-rShift)*(1.-rba*rc(R,0));
      // rc(R,1) *= 1.-rba*rc(R,0);
 //      if( numberOfInnerCurves>0 )
// 	holes(all,1) = (holes(all,1)-rShift)*(1.-rba*holes(all,0));
      // holes(all,1)*=1.-rba*holes(all,0);
    }
  }
#else
  const real ra = rai;

  const real ra_r=(real)referenceSurface.getDomainBound(0,0)+ra, ra_s=(real)referenceSurface.getDomainBound(0,1)+ra,
    rba_r=(real)referenceSurface.getDomainBound(1,0)-ra_r, rba_s=(real)referenceSurface.getDomainBound(1,1)-ra_s;

  //  cout<<ra_r<<"  "<<ra_s<<"  "<<rba_r<<"  "<<rba_s<<endl;
  //  cout<<min(rc(R,0))<<"  "<<min(rc(R,1))<<"  "<<max(rc(R,0))<<"  "<<max(rc(R,1))<<endl;
  // real rShift = s==0 ? .25 : .75;
  real rShift=.5;
  if( collapsedEdge[0][0] || collapsedEdge[1][0] || collapsedEdge[0][1] || collapsedEdge[1][1] )
  {
    if( collapsedEdge[0][1] )
    {
      // bottom collapsed -- apply a scaling function to the nodes:
      printf(" ** bottom collapsed, scale nodes to get better triangles shapes\n");

      //      rc(R,0) = (rc(R,0)-rShift)*(ra + rba*rc(R,1));
      rc(R,0) = (rc(R,0)-rShift)*(ra_s + rba_s*rc(R,1));
      // kkc rc(R,0) *= ra + rba*rc(R,1);
      //kkc	holes(all,0) = (holes(all,0)-rShift)*(ra + rba*holes(all,1));
      // holes(all,0)*=ra + rba*holes(all,1);
    }
    else if( collapsedEdge[1][1] )
    {
      // top collapsed -- apply a scaling function to the nodes:
      printf(" ** top collapsed, scale nodes to get better triangles shapes\n");
      rc(R,0) = (rc(R,0)-rShift)*(1.-rba_s*rc(R,1));
      //kkc       rc(R,0) = (rc(R,0)-rShift)*(1.-rba*rc(R,1));
      // rc(R,0) *= 1.-rba*rc(R,1);
      //kkc	holes(all,0) = (holes(all,0)-rShift)*(1.-rba*holes(all,1));
      // holes(all,0)*=1.-rba*holes(all,1);
    }
    else if( collapsedEdge[0][0] )
    {
      printf(" ** left collapsed, scale nodes to get better triangles shapes\n");
      rc(R,1) = (rc(R,1)-rShift)*(ra_r + rba_r*rc(R,0));
      // kkc      rc(R,1) = (rc(R,1)-rShift)*(ra + rba*rc(R,0));
      // rc(R,1) *= ra + rba*rc(R,0);
      // kkc	holes(all,1) = (holes(all,1)-rShift)*(ra + rba*holes(all,0));
      // holes(all,1)*=ra + rba*holes(all,0);
    }
    else if( collapsedEdge[1][0] )
    {
      printf(" ** right collapsed, scale nodes to get better triangles shapes\n");
      rc(R,1) = (rc(R,1)-rShift)*(1.-rba_r*rc(R,0));
      // kkc      rc(R,1) = (rc(R,1)-rShift)*(1.-rba*rc(R,0));
      // rc(R,1) *= 1.-rba*rc(R,0);
      //kkc	holes(all,1) = (holes(all,1)-rShift)*(1.-rba*holes(all,0));
      // holes(all,1)*=1.-rba*holes(all,0);
    }
  }
#endif

  return true;
}

bool unScaleNodes(Mapping &referenceSurface, realArray &rt, bool collapsedEdge[2][3], real rai)
{
  // undo the scaling done above
  // this code is taken from compositeTopology.C:buildSubSurfaceTriangulation
  //   it must match the code there
  Range all,R;

#if 0
  real ra= rai;
  const real /*ra=.05,*/ rba=1.-ra;

  // real rShift = s==0 ? .25 : .75;
  real rShift=.5;
  if( collapsedEdge[0][0] || collapsedEdge[1][0] || collapsedEdge[0][1] || collapsedEdge[1][1] )
  {
    if( collapsedEdge[0][1] )
    {
      // bottom collapsed -- apply a scaling function to the nodes:
      printf(" ** bottom collapsed, un-scale nodes to get better triangles shapes\n");
      rt(all,0) = rt(all,0)/(ra + rba*rt(all,1))+rShift;
      // rt(all,0) /= ra + rba*rt(all,1);
    }
    else if( collapsedEdge[1][1] )
    {
      rt(all,0) = rt(all,0)/(1.-rba*rt(all,1))+rShift;
      // rt(all,0) /= 1.-rba*rt(all,1);
    }
    else if( collapsedEdge[0][0] )
    {
      rt(all,1) = rt(all,1)/(ra + rba*rt(all,0))+rShift;
      // rt(all,1) /= ra + rba*rt(all,0);
    }
    else if( collapsedEdge[1][0] )
    {
      rt(all,1) = rt(all,1)/(1.-rba*rt(all,0))+rShift;
      // rt(all,1) /= 1.-rba*rt(all,0);
    }
  }
#else

  real ra = rai;
  real rShift=.5;
  const real ra_r=(real)referenceSurface.getDomainBound(0,0)+ra, ra_s=(real)referenceSurface.getDomainBound(0,1)+ra,
    rba_r=(real)referenceSurface.getDomainBound(1,0)-ra_r, rba_s=(real)referenceSurface.getDomainBound(1,1)-ra_s;


  if( collapsedEdge[0][0] || collapsedEdge[1][0] || collapsedEdge[0][1] || collapsedEdge[1][1] )
    {
      if( collapsedEdge[0][1] )
	{
	  // bottom collapsed -- apply a scaling function to the nodes:
	  printf(" ** bottom collapsed, un-scale nodes to get better triangles shapes\n");
	  rt(all,0) = rt(all,0)/(ra_s + rba_s*rt(all,1))+rShift;
	  //kkc      rt(all,0) = rt(all,0)/(ra + rba*rt(all,1))+rShift;
	  // rt(all,0) /= ra + rba*rt(all,1);
	}
      else if( collapsedEdge[1][1] )
	{
	  rt(all,0) = rt(all,0)/(1.-rba_s*rt(all,1))+rShift;
	  //kkc      rt(all,0) = rt(all,0)/(1.-rba*rt(all,1))+rShift;
	  // rt(all,0) /= 1.-rba*rt(all,1);
	}
      else if( collapsedEdge[0][0] )
	{
	  rt(all,1) = rt(all,1)/(ra_r + rba_r*rt(all,0))+rShift;
	  //kkc      rt(all,1) = rt(all,1)/(ra + rba*rt(all,0))+rShift;
	  // rt(all,1) /= ra + rba*rt(all,0);
	}
      else if( collapsedEdge[1][0] )
	{
	  rt(all,1) = rt(all,1)/(1.-rba_r*rt(all,0))+rShift;
	  //kkc      rt(all,1) = rt(all,1)/(1.-rba*rt(all,0))+rShift;
	  // rt(all,1) /= 1.-rba*rt(all,0);
	}
    }
#endif
  return true;
}

bool findRBound(realArray &rc, bool collapsedEdge[2][3], real &rBound, int &axis, bool &atMin)
{

  Range all;
  if( collapsedEdge[0][1] )
    {
      // bottom collapsed -- apply a scaling function to the nodes:
      axis = 1;
      atMin = true;
      rBound = REAL_MAX;
      for ( int i=0; i<rc.getLength(0); i++ )
	if ( fabs(rc(i,0)) > FLT_EPSILON && rc(i,0)<rBound )
	  rBound = rc(i,0);
    }
  else if( collapsedEdge[1][1] )
    {
      real rMax = max(rc(all,0));
      axis = 1;
      atMin = false;
      rBound = 0.;
      for ( int i=0; i<rc.getLength(0); i++ )
	if ( fabs(rc(i,0)-rMax) > FLT_EPSILON && rc(i,0)>rBound )
	  rBound = rc(i,0);
      
    }
  else if( collapsedEdge[0][0] )
    {
      axis = 0;
      atMin = true;
      rBound = REAL_MAX;
      for ( int i=0; i<rc.getLength(0); i++ )
	if ( fabs(rc(i,1)) > FLT_EPSILON && rc(i,1)<rBound )
	  rBound = rc(i,1);
    }
  else if( collapsedEdge[1][0] )
    {
      real rMax = max(rc(all,1));
      cout<<"rMax is "<<rMax<<endl;
      axis = 0;
      atMin = false;
      rBound = 0.;
      for ( int i=0; i<rc.getLength(0); i++ )
	if ( fabs(rc(i,1)-rMax) > FLT_EPSILON && rc(i,1)>rBound )
	  rBound = rc(i,1);
    }
  else
    return false;

  cout<<"has collapsed edge, axis "<<axis<<" rBound = "<<rBound<< (atMin ? " at the start " : " at the end")<<endl;
  return true;
}

#if 0
int main(int argc, char *argv[])
{

  Overture::start(argc,argv);

  GenericGraphicsInterface &gi = *Overture::getGraphicsInterface();
  GraphicsParameters gp;
  
  CompositeSurface model;
  MappingInformation mapInfo;
  mapInfo.graphXInterface = &gi;

  if ( argc==1 )
    rapOpenModel(gi, gp, model);
  else
    rapOpenModel(argv[1],model);

  //PlotIt::plot(gi,model,gp);

  realArray initialDeviation = evaluate(evaluateDeviation(model));
  cout<<"min deviation from model : "<<min(initialDeviation)<<endl<<
    "max deviation from model "<<max(initialDeviation)<<endl;

  real epsAbs = .5*(max(initialDeviation)-min(initialDeviation));
  //real epsAbs = .05;

  model.updateTopology();
  real maxDev = refineVisibleSurfaces(gi, model, epsAbs);

  cout<<"final max deviation from model "<<maxDev<<endl;

  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
  gp.set(GI_PLOT_UNS_EDGES,true);
  CompositeTopology &topo = *model.getCompositeTopology();
  gi.erase();
  for ( int s=0; s<model.numberOfSubSurfaces(); s++ )
    {
      PlotIt::plot(gi,*topo.getTriangulationSurface(s),gp);
    }

  GUIState gui;
  gui.setExitCommand("Exit","Exit");
  gi.pushGUI(gui);
  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
  aString answer;
  gi.getAnswer(answer,"");
  gi.popGUI();

  Overture::finish();

  return 0;
}
#endif

realArray evaluateDeviation(CompositeSurface &model)
{
  int rdim = model.getRangeDimension();
  int ddim = model.getDomainDimension();

  Range RDIM(rdim), DDIM(ddim);
 
  if ( !model.isTopologyDetermined() ) model.updateTopology();

  CompositeTopology &topo = *model.getCompositeTopology();

  UnstructuredMapping &globalTri = *topo.getTriangulation();

  int numberOfElements = globalTri.getNumberOfElements();
  Range NELEMS(numberOfElements);

  realArray rCenter(NELEMS, DDIM), xCenter(NELEMS, RDIM), xProjected(NELEMS,RDIM);
  const realArray &nodes = globalTri.getNodes();
  const intArray &elems = globalTri.getElements();

  rCenter = 0;
  xCenter = 0;

  for ( int e=0; e<numberOfElements; e++ )
    {
      // OY! here we assuming composite topologies only keep triangulations!
      xCenter(e,RDIM) = ( nodes(elems(e,0),RDIM) + nodes(elems(e,1),RDIM) + nodes(elems(e,2),RDIM) )/3;
    }

  xProjected(NELEMS, RDIM) = xCenter(NELEMS, RDIM);

  MappingProjectionParameters mp;
  mp.getRealArray(MappingProjectionParameters::r) = rCenter;
  mp.getRealArray(MappingProjectionParameters::x) = xCenter;
  model.project(xProjected,mp);

  realArray dev;
  dev = sqrt(sum(pow(xCenter-xProjected,2),1));

  return dev;
}

realArray computeDeviation(UnstructuredMapping &umap, Mapping &cmap)
{
  int rdim = cmap.getRangeDimension();
  int ddim = cmap.getDomainDimension();
 
  Range RDIM(rdim), DDIM(ddim);

  int numberOfElements = umap.getNumberOfElements();

#ifdef ELEMENTS
  Range NELEMS(numberOfElements);
#else
  Range NELEMS(umap.getNumberOfEdges());
#endif

  realArray rCenter(NELEMS, DDIM), xCenter(NELEMS, RDIM), xProjected(NELEMS,RDIM);
  const realArray &nodes = umap.getNodes();
  const intArray &elems = umap.getElements();
  const intArray &edges = umap.getEdges();

  rCenter = 0;
  xCenter = 0;

#ifdef ELEMENTS
  for ( int e=0; e<numberOfElements; e++ )
    {
      // OY! here we assuming composite topologies only keep triangulations!
      xCenter(e,RDIM) = ( nodes(elems(e,0),RDIM) + nodes(elems(e,1),RDIM) + nodes(elems(e,2),RDIM) )/3;
    }
#else
  for ( int e=0; e<umap.getNumberOfEdges(); e++ )
    {
      // OY! here we assuming composite topologies only keep triangulations!
      xCenter(e,RDIM) = ( nodes(edges(e,0),RDIM) + nodes(edges(e,1),RDIM) )/2.;
    }
#endif
  
  xProjected = xCenter;

  MappingProjectionParameters mp;
  mp.getRealArray(MappingProjectionParameters::r) = rCenter;
  mp.getRealArray(MappingProjectionParameters::x) = xCenter;

  if ( cmap.getClassName()=="TrimmedMapping" )
    {
      ((TrimmedMapping&)cmap).surface->project(xProjected,mp);
    }
  else
    cmap.project(xProjected,mp);

  realArray dev;
  dev = sqrt(sum(pow(xCenter-xProjected,2),1));

  return dev;
}


real refineTriangulation(UnstructuredMapping &umap, Mapping &cmap, real absoluteTol)
{
  int rdim = cmap.getRangeDimension();
  int ddim = cmap.getDomainDimension();

  Range RDIM(rdim), DDIM(ddim);

  realArray udev = evaluate(computeDeviation(umap,cmap));
  
  Range nOldNodes(umap.getNumberOfNodes());

  int nNewNodes = 0;

  int numberOfGridPoints[2];
  bool collapsedEdge[2][3];
  real averageArclength[2];
  real elementDensityTolerance=.05;

  cmap.determineResolution(numberOfGridPoints,collapsedEdge,averageArclength,elementDensityTolerance );
  real aspectRatio=averageArclength[0]/max(REAL_MIN,averageArclength[1]);
  bool hasCollapsedEdge = ( collapsedEdge[0][0] || collapsedEdge[1][0] ||
			    collapsedEdge[0][1] || collapsedEdge[1][1] );

  real rMaxLim[] = { REAL_MAX, REAL_MAX };
  real rMinLim[] = { 0, 0 };

#ifdef ELEMENTS
  for ( int e=0; e<umap.getNumberOfElements(); e++ )
    if ( udev(e)>absoluteTol ) 
      {
	//	nNewNodes++;
	int nNew=min(2,int(udev(e)/absoluteTol));
	nNewNodes += 1 + 3*(nNew-1);
      }
#else
  for ( int e=0; e<umap.getNumberOfEdges(); e++ )
    if ( udev(e)>absoluteTol ) nNewNodes++;
#endif

  const realArray &oldNodes = umap.getNodes();
  realArray oldParameterNodes(nOldNodes, DDIM);
  oldParameterNodes=-1;
  if ( cmap.getClassName()=="TrimmedMapping" ) 
    {
      ((TrimmedMapping &)cmap).surface->inverseMap(oldNodes, oldParameterNodes);

      if ( false ){
      Overture::getGraphicsInterface()->erase();
      Overture::getGraphicsInterface()->plotPoints(oldParameterNodes);
      cout<<((TrimmedMapping &)cmap).surface->getClassName()<<endl;
      Overture::getGraphicsInterface()->erase();
      //PlotIt::plot(*Overture::getGraphicsInterface(),*((TrimmedMapping &)cmap).surface);
      cout<<((TrimmedMapping &)cmap).surface->getIsPeriodic(0)<<" "<<((TrimmedMapping &)cmap).surface->getIsPeriodic(1)<<endl;
      }

      const intArray &ufaces = umap.getFaces();
      const intArray &elems = umap.getElements();
      const intArray &faceElems = umap.getFaceElements();

      if ( (bool)((TrimmedMapping &)cmap).surface->getIsPeriodic(0) || 
           (bool)((TrimmedMapping &)cmap).surface->getIsPeriodic(1) )
	{ // check the projection on periodic boundaries
	  real dx[2];
	  dx[0] = dx[1]= 0.;
	  int pdir = 0;
	  if ( ((TrimmedMapping &)cmap).surface->getIsPeriodic(0) ) 
	    dx[0] = 1;
	  else
	    {
	      dx[1] = 1;
	      pdir = 1;
	    }

	  intArray moved(nOldNodes);
	  moved = 0;
	  for ( int bf=0; bf<umap.getNumberOfBoundaryFaces(); bf++ )
	    {

	      int f = umap.getBoundaryFace(bf);
	      real x[2][3];
	      x[0][0] = oldParameterNodes(ufaces(f,0),0);
	      x[1][0] = oldParameterNodes(ufaces(f,0),1);
	      x[0][1] = oldParameterNodes(ufaces(f,1),0);
	      x[1][1] = oldParameterNodes(ufaces(f,1),1);

	      int nd=-1;
	      // find the off face node
	      if ( elems(faceElems(f,0),0)!=ufaces(f,0) && elems(faceElems(f,0),0)!=ufaces(f,1) )
		{
		  x[0][2] = oldParameterNodes(elems(faceElems(f,0),0),0);
		  x[1][2] = oldParameterNodes(elems(faceElems(f,0),0),1);
		  nd=0;
		}
	      else if ( elems(faceElems(f,0),1)!=ufaces(f,0) && elems(faceElems(f,0),1)!=ufaces(f,1) )
		{
		  x[0][2] = oldParameterNodes(elems(faceElems(f,0),1),0);
		  x[1][2] = oldParameterNodes(elems(faceElems(f,0),1),1);
		  nd=1;
		}
	      else if ( elems(faceElems(f,0),2)!=ufaces(f,0) && elems(faceElems(f,0),2)!=ufaces(f,1) )
		{
		  x[0][2] = oldParameterNodes(elems(faceElems(f,0),2),0);
		  x[1][2] = oldParameterNodes(elems(faceElems(f,0),2),1);
		  nd=2;
		}

	      // compute the triangle area, if it is -tive, the inverseMap put the face on the wrong boundary
	      real area = (x[0][1]-x[0][0])*(x[1][2]-x[1][0]) - (x[1][1]-x[1][0])*(x[0][2]-x[0][0]);
	      if ( area<0. )
		{
		  //		  cout<<area<<"  "<<oldParameterNodes(ufaces(f,0),0)<<" "<<oldParameterNodes(ufaces(f,0),1)<<"    "<<oldParameterNodes(ufaces(f,1),0)<<"  "<<oldParameterNodes(ufaces(f,1),1)<<endl;

		  real dist1 = (x[0][2]-x[0][0])*(x[0][2]-x[0][0]) + (x[1][2]-x[1][0])*(x[1][2]-x[1][0]);
		  real dist2 = (x[0][2]-x[0][1])*(x[0][2]-x[0][1]) + (x[1][2]-x[1][1])*(x[1][2]-x[1][1]);
		  real emag = (x[0][1]-x[0][0])*(x[0][1]-x[0][0]) + (x[1][1]-x[1][0])*(x[1][1]-x[1][0]);

		  //cout<<emag<<"  "<<dist1<<"  "<<dist2<<endl;
		  if ( emag>.25 )
		    {
		      // move only one of the nodes
		      if ( pdir==0 && ( fabs(x[0][0]-x[0][2])<100*REAL_EPSILON ) )
			{
			  if ( x[0][0]>x[0][1] )
			    oldParameterNodes(ufaces(f,0),0) -= dx[0];
			  else
			    oldParameterNodes(ufaces(f,0),0) += dx[0];
			  moved(ufaces(f,0)) =1;
			}
		      else if ( pdir==0 && ( fabs(x[0][1]-x[0][2])<100*REAL_EPSILON ) )
			{
			  if ( x[0][0]<x[0][1] )
			    oldParameterNodes(ufaces(f,1),0) -= dx[0];
			  else
			    oldParameterNodes(ufaces(f,1),0) += dx[0];
			  moved(ufaces(f,1)) =1;
			}

		      else if ( pdir==1 && ( fabs(x[1][0]-x[1][2])<100*REAL_EPSILON ) )
			{
			  if ( x[1][0]>x[1][1] )
			    oldParameterNodes(ufaces(f,0),1) -= dx[1];
			  else
			    oldParameterNodes(ufaces(f,0),1) += dx[1];
			  moved(ufaces(f,0)) =1;
			}
		      else if ( pdir==1 && ( fabs(x[1][1]-x[1][2])<100*REAL_EPSILON ) )
			{
			  if ( x[1][0]<x[1][1] )
			    oldParameterNodes(ufaces(f,1),1) -= dx[1];
			  else
			    oldParameterNodes(ufaces(f,1),1) += dx[1];
			  moved(ufaces(f,1)) =1;
			}

		      else if ( dist1>.25 && !moved(ufaces(f,0)) )
			{
			  if ( pdir==0 )
			    if ( x[0][0]>x[0][1] )
			      oldParameterNodes(ufaces(f,0),0) -= dx[0];
			    else
			      oldParameterNodes(ufaces(f,0),0) += dx[0];
			  else
			    if ( x[1][0]>x[1][1] )
			      oldParameterNodes(ufaces(f,0),1) -= dx[1];
			    else
			      oldParameterNodes(ufaces(f,0),1) += dx[1];
			  
			  moved(ufaces(f,0)) =1;
			}
		      else if ( dist2>.25 && !moved(ufaces(f,1)) ) 
			{
			  if ( pdir==0 )
			    if ( x[0][0]<x[0][1] )
			      oldParameterNodes(ufaces(f,1),0) -= dx[0];
			    else
			      oldParameterNodes(ufaces(f,1),0) += dx[0];
			  else
			    if ( x[1][0]<x[1][1] )
			      oldParameterNodes(ufaces(f,1),1) -= dx[1];
			    else
			      oldParameterNodes(ufaces(f,1),1) += dx[1];
			  
			  moved(ufaces(f,1)) =1;
			  
			}
		    }
		  else 
		    {
		      // move both or neither
		      if ( dist1>.25 && dist2>.25 )
			{
			  if ( !moved(ufaces(f,0)) )
			    {
			      if ( pdir==0 )
				if ( x[0][0]>x[0][1] )
				  oldParameterNodes(ufaces(f,0),0) -= dx[0];
				else
				  oldParameterNodes(ufaces(f,0),0) += dx[0];
			      else
				if ( x[1][0]>x[1][1] )
				  oldParameterNodes(ufaces(f,0),1) -= dx[1];
				else
				  oldParameterNodes(ufaces(f,0),1) += dx[1];
			      
			      moved(ufaces(f,0)) =1;
			    }
			  
			  if ( !moved(ufaces(f,1)) ) 
			    {
			      if ( pdir==0 )
				if ( x[0][0]<x[0][1] )
				  oldParameterNodes(ufaces(f,1),0) -= dx[0];
				else
				  oldParameterNodes(ufaces(f,1),0) += dx[0];
			      else
				if ( x[1][0]<x[1][1] )
				  oldParameterNodes(ufaces(f,1),1) -= dx[1];
				else
				  oldParameterNodes(ufaces(f,1),1) += dx[1];
			      
			      moved(ufaces(f,1)) =1;
			      
			    }
			}
		    }
		  
// 		        Overture::getGraphicsInterface()->erase();
// 		        Overture::getGraphicsInterface()->plotPoints(oldParameterNodes);

		  //		  cout<<"after move  "<<oldParameterNodes(ufaces(f,0),0)<<" "<<oldParameterNodes(ufaces(f,0),1)<<"    "<<oldParameterNodes(ufaces(f,1),0)<<"  "<<oldParameterNodes(ufaces(f,1),1)<<endl;
		}
	    } 
	}
    }
  else
    cmap.inverseMap(oldNodes, oldParameterNodes);

  //  oldParameterNodes.display("old nodes");
  //oldNodes.display("old nodes");
  if ( false ){
    realArray tmpNodes(nOldNodes,RDIM);
    cmap.map(oldParameterNodes, tmpNodes);
    UnstructuredMapping tmpu(2,3,Mapping::parameterSpace, Mapping::cartesianSpace);
    tmpu.setNodesAndConnectivity(tmpNodes, umap.getElements());
    Overture::getGraphicsInterface()->erase();
    PlotIt::plot(*Overture::getGraphicsInterface(),tmpu);
    
  }

  const intArray &elem = umap.getElements();
  const intArray &edges = umap.getEdges();
  const intArray &ufaces = umap.getFaces();

  cout<<"adding "<<nNewNodes<<" nodes"<<endl;

  real rBound;
  int axis;
  bool atMin;

  if ( hasCollapsedEdge )
    findRBound(oldParameterNodes,collapsedEdge,rBound, axis, atMin);


  if ( nNewNodes )
    {
      realArray newParameterNodes(umap.getNumberOfNodes() + nNewNodes, cmap.getDomainDimension());
      
      newParameterNodes = 0;
      newParameterNodes(nOldNodes,DDIM) = oldParameterNodes(nOldNodes,DDIM);

      int ax2Adjust = axis;
      if ( hasCollapsedEdge )
	cout<<"will adjust axis "<<ax2Adjust<<endl;



      int nn=0;
      int nUN = umap.getNumberOfNodes();
#ifdef ELEMENTS
      for ( int e=0; e<umap.getNumberOfElements(); e++ )
	{

	  if ( udev(e)>absoluteTol )
	    {

	      int nNew=min(2,int(udev(e)/absoluteTol));

	      newParameterNodes(nUN+nn,DDIM) = ( oldParameterNodes(elem(e,0),DDIM) + 
						 oldParameterNodes(elem(e,1),DDIM) + 
						 oldParameterNodes(elem(e,2),DDIM))/3;
	      nn++;

	      if ( nNew>1 )
		{
		  ArraySimpleFixed<real,3,2,1,1> dxc;
		  for ( int a=0; a<2; a++ )
		    {
		      real xc = ( oldParameterNodes(elem(e,0),a)+ 
				  oldParameterNodes(elem(e,1),a) + 
				  oldParameterNodes(elem(e,2),a))/3;
#if 0

		      dxc(0,a) = (xc - oldParameterNodes(elem(e,0),a))/real(nNew);
		      dxc(1,a) = (xc - oldParameterNodes(elem(e,1),a))/real(nNew);
		      dxc(2,a) = (xc - oldParameterNodes(elem(e,2),a))/real(nNew);

#else
		      real xc0 = ( oldParameterNodes(elem(e,0),a)+ 
				  oldParameterNodes(elem(e,1),a))/2;
		      real xc1 = ( oldParameterNodes(elem(e,1),a)+ 
				  oldParameterNodes(elem(e,2),a))/2;
		      real xc2 = ( oldParameterNodes(elem(e,0),a)+ 
				  oldParameterNodes(elem(e,2),a))/2;

		      dxc(0,a) = (xc - xc0)/real(nNew);
		      dxc(1,a) = (xc - xc1)/real(nNew);
		      dxc(2,a) = (xc - xc2)/real(nNew);

#endif		      

		    }

		  for ( int n=0; n<nNew-1; n++ )
		    {
		      for ( int a=0; a<2; a++ )
			{
#if 0
			  newParameterNodes(nUN+nn,a) = oldParameterNodes(elem(e,0),a) - (n+1)*dxc(0,a);
			  newParameterNodes(nUN+nn+1,a) = oldParameterNodes(elem(e,1),a) - (n+1)*dxc(1,a);
			  newParameterNodes(nUN+nn+2,a) = oldParameterNodes(elem(e,2),a) - (n+1)*dxc(2,a);
#else
			  real xc = ( oldParameterNodes(elem(e,0),a)+ 
				      oldParameterNodes(elem(e,1),a) + 
				      oldParameterNodes(elem(e,2),a))/3;

			  newParameterNodes(nUN+nn,a) = xc + (n+1)*dxc(0,a);
			  newParameterNodes(nUN+nn+1,a) = xc + (n+1)*dxc(1,a);
			  newParameterNodes(nUN+nn+2,a) = xc +  (n+1)*dxc(2,a);
#endif
			}
		      nn += 3;
		    }
		}

// 	      if ( false && hasCollapsedEdge )
// 		{
// 		  if ( atMin )
// 		    {
// 		      real minr = min(oldParameterNodes(elem(e,0),ax2Adjust),oldParameterNodes(elem(e,1),ax2Adjust),oldParameterNodes(elem(e,2),ax2Adjust));
// 		      if ( newParameterNodes(nUN+nn,ax2Adjust)<rBound )
// 			newParameterNodes(nUN+nn,ax2Adjust) = rBound;//.5*(rBound + minr);
// 		    }
// 		  else
// 		    {
// 		      real maxr = max(oldParameterNodes(elem(e,0),ax2Adjust),oldParameterNodes(elem(e,1),ax2Adjust),oldParameterNodes(elem(e,2),ax2Adjust));
// 		      if ( newParameterNodes(nUN+nn,ax2Adjust)>rBound )
// 			newParameterNodes(nUN+nn,ax2Adjust) = rBound;//.5*(rBound + maxr);
// 		    }
// 		}

	    }
	}
#else
      for ( int e=0; e<umap.getNumberOfEdges(); e++ )
	{
	  if ( udev(e)>absoluteTol && ufaces(e,1)>=0 )
	    {
	      newParameterNodes(umap.getNumberOfNodes()+nn,DDIM) = ( oldParameterNodes(edges(e,0),DDIM) + 
								     oldParameterNodes(edges(e,1),DDIM) )/2.;
	      nn++;
	    }
	}
#endif

      intArray faces(umap.getNumberOfBoundaryFaces(),2);
      const intArray &ufaces = umap.getFaces();
      int h=0;
      for ( int f=0; f<umap.getNumberOfBoundaryFaces(); f++ )
	{
	  int bf = umap.getBoundaryFace(f);
	  faces(f,0) = ufaces(bf,0);
	  faces(f,1) = ufaces(bf,1);

	  if ( ! f%2 )
	    {
	      real dx = -oldParameterNodes(faces(f,1),1)+oldParameterNodes(faces(f,0),1);
	      real dy = oldParameterNodes(faces(f,1),0)+oldParameterNodes(faces(f,0),0);
	    }
	}

      if ( cmap.getClassName()=="TrimmedMapping" )
	if ( ((TrimmedMapping &)cmap).getNumberOfTrimCurves()>1 )
	  {
	    cout<<"trimmed mapping with multiple curves"<<endl;
	  }

      TriangleWrapper triangulator;
      TriangleWrapperParameters &tp = triangulator.getParameters();
      tp.saveNeighbourList(true);
      tp.setQuietMode(true);
      tp.setVerboseMode(0);
      if ( !tp.getFreezeSegments() ) tp.toggleFreezeSegments();
      if ( !tp.getVoronoi() ) tp.toggleVoronoi();

      if ( false ) {
      Overture::getGraphicsInterface()->erase();
      PlotIt::plot(*Overture::getGraphicsInterface(),cmap);
      Overture::getGraphicsInterface()->erase();
      Overture::getGraphicsInterface()->plotPoints(oldParameterNodes);
      Overture::getGraphicsInterface()->erase();
      Overture::getGraphicsInterface()->plotPoints(newParameterNodes);
      }

       if ( hasCollapsedEdge )
 	scaleNodes(cmap,newParameterNodes, collapsedEdge, cmap.getClassName()=="TrimmedMapping",absoluteTol);
      
      {
	Range R(newParameterNodes.getLength(0));
	newParameterNodes(R,0) *=aspectRatio;
      }

      intArray nullFaces;
      triangulator.initialize(faces,newParameterNodes);
      //      triangulator.initialize(nullFaces,newParameterNodes);
      //      triangulator.setHoles(holes);
      triangulator.generate();
      
      newParameterNodes.redim(0);
      newParameterNodes = triangulator.getPoints();
      {
	Range R(newParameterNodes.getLength(0));
	newParameterNodes(R,0) /=aspectRatio;
      }

      if ( hasCollapsedEdge )
       	unScaleNodes(cmap,newParameterNodes, collapsedEdge,absoluteTol);


      if (false) {
	UnstructuredMapping tmpu;
	tmpu.setNodesAndConnectivity(newParameterNodes, evaluate(triangulator.generateElementList()));
	PlotIt::plot(*Overture::getGraphicsInterface(),tmpu);
      }

      realArray newNodes(newParameterNodes.getLength(0),rdim);
      cmap.map(newParameterNodes,newNodes);

      umap.setNodesAndConnectivity(newNodes,evaluate(triangulator.generateElementList()));

      if ( false ) {
	realArray udn;
	udn = computeDeviation(umap,cmap);
	newParameterNodes.resize(newParameterNodes.getLength(0),3);
	for ( int n=0; n<newParameterNodes.getLength(0); n++ )
	  {
	    newParameterNodes(n,2) = udn(n);
	  }
	Overture::getGraphicsInterface()->erase();
	Overture::getGraphicsInterface()->plotPoints(newParameterNodes);
      }

      if  (false) {
      Overture::getGraphicsInterface()->erase();
      PlotIt::plot(*Overture::getGraphicsInterface(),umap);
      }
      //      umap.setNodesElementsAndNeighbours(newNodes,triangulator.generateElementList(), triangulator.getNeighbours());

    }

  return max(evaluate(computeDeviation(umap,cmap)));
}

real refineVisibleSurfaces(GenericGraphicsInterface &gi,
			   CompositeSurface &model,
			   real absoluteTol)
{
  cout<<"absolute tolerance is "<<absoluteTol<<endl;

  CompositeTopology & topo = *model.getCompositeTopology();

  real maxDev = 0;
  for ( int s=0; s<model.numberOfSubSurfaces(); s++ )
    {
      UnstructuredMapping &umap = *topo.getTriangulationSurface(s);
      Mapping &cmap = model[s];

      int step=0;
      real dev;
      while ( (dev=refineTriangulation(umap,cmap,absoluteTol))>absoluteTol && step<10 )
	{
	  cout<<"Surface " << s << ": deviation after step "<<step<<" is "<<dev<<endl;
	  step++;
// 	  gi.erase();
// 	  PlotIt::plot(gi,umap);
	}
      maxDev = max(maxDev,dev);
      cout<<"deviation after "<<step<<" steps is "<<dev<<endl;
    }

  return maxDev;
}

int rapOpenModel(GenericGraphicsInterface &gi, GraphicsParameters &gp, CompositeSurface &model)
{
  HDF_DataBase db;
  aString modelFileName;
  gi.inputFileName(modelFileName, "", ".hdf");
  if (modelFileName.length() > 0 && modelFileName != " " && db.mount(modelFileName,"R") == 0)
    {
      CompositeSurface * subModel_=NULL;
      // To obtain the topology from the first model, we read it into the main model directly
      // For subsequent parts, we just add the sub surfaces
      if (model.numberOfSubSurfaces()>0)
	{
	  subModel_ = new CompositeSurface;
	  subModel_->incrementReferenceCount();
	  
	  subModel_->get(db,"Rap model");          // get the model from data base
	}
      else
	{
	  model.get(db,"Rap model");
	}
      //      sGrids.get(db,"Rap surface grids"); // get the surface grids
      //      vGrids.get(db,"Rap volume grids");  // get the volume grids
      db.unmount();                       // close the data base
      
      if (subModel_)
	{
	  printf("Adding...\n");
	  int map;
	  for( map=0; map<subModel_->numberOfSubSurfaces(); map++ )
	    {
	      // AP: An offset should be added to the surface ID
	      model.add((*subModel_)[map], subModel_->getSurfaceID(map) ); 
	      printf("%i,",map);
	      fflush(stdout);
	    }
	  printf("\n");
	  
	  // delete the subModel
	  if (subModel_->decrementReferenceCount() == 0)
	    delete subModel_;
	}
    }
  else
    {
      aString buf;
      sPrintF(buf,"Could not open the data base `%s'", SC modelFileName);
      gi.createMessageDialog(buf, errorDialog);
      return 1;
    }
  return 0;
}

int rapOpenModel(aString modelFileName, CompositeSurface &model)
{
  HDF_DataBase db;
  if (modelFileName.length() > 0 && modelFileName != " " && db.mount(modelFileName,"R") == 0)
    {
      CompositeSurface * subModel_=NULL;
      // To obtain the topology from the first model, we read it into the main model directly
      // For subsequent parts, we just add the sub surfaces
      if (model.numberOfSubSurfaces()>0)
	{
	  subModel_ = new CompositeSurface;
	  subModel_->incrementReferenceCount();
	  
	  subModel_->get(db,"Rap model");          // get the model from data base
	}
      else
	{
	  model.get(db,"Rap model");
	}
      //      sGrids.get(db,"Rap surface grids"); // get the surface grids
      //      vGrids.get(db,"Rap volume grids");  // get the volume grids
      db.unmount();                       // close the data base
      
      if (subModel_)
	{
	  printf("Adding...\n");
	  int map;
	  for( map=0; map<subModel_->numberOfSubSurfaces(); map++ )
	    {
	      // AP: An offset should be added to the surface ID
	      model.add((*subModel_)[map], subModel_->getSurfaceID(map) ); 
	      printf("%i,",map);
	      fflush(stdout);
	    }
	  printf("\n");
	  
	  // delete the subModel
	  if (subModel_->decrementReferenceCount() == 0)
	    delete subModel_;
	}
    }
  else
    {
      aString buf;
      sPrintF(buf,"Could not open the data base `%s'", SC modelFileName);
      return 1;
    }
  return 0;
}
