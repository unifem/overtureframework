// #define BOUNDS_CHECK

#include "MappingsFromCAD.h"

//#include "GL_GraphicsInterface.h"
#include "GenericGraphicsInterface.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include <float.h>
#include "NurbsMapping.h"
#include "LineMapping.h"
#include "CircleMapping.h"
#include "MatrixTransform.h"
#include "IgesReader.h"
#include "TrimmedMapping.h"
#include "TFIMapping.h"
#include "HDF_DataBase.h"
#include "CompositeSurface.h"
#include "UnstructuredMapping.h"
#include "SweepMapping.h"
#include "RevolutionMapping.h"
#include "display.h"

#include "TriangleWrapper.h"
#include "UnstructuredMapping.h"

extern real timeToMergeNurbs;
extern real timeToMergeNurbsAddSubCurve;
extern real timeToMergeNurbsArcLength;
extern real timeToMergeNurbsElevateDegree;
extern real timeToMergeNurbsOther;

static real timeToBuildTrimmedMappings=0.;
static real timeForCreateSurface=0.;
static real timeToBuildNurbsSurfaces=0.;
static real timeToBuildNurbsCurves=0.;
static real timeToMergeNurbsCurves=0.;
static real timeToCreateCurves=0.;
static real timeToAdjustCurves=0.;
static real timeToCopyNurbsCurves=0.;
static real timeToBuildCurvesOther=0.;

MappingsFromCAD::
MappingsFromCAD()
// =========================================================================================================
/// \details 
///      Build Mapping's from CAD files. 
// =========================================================================================================
{
  igesPointer=NULL;
  mapInfoPointer=NULL;
  revCount=0, revCurveCount=0, axisLineCount=0;
  subCurveBound[0] = NULL;
  subCurveBound[1] = NULL;
  lineType = NULL;
 
}

MappingsFromCAD::
~MappingsFromCAD()
{
  if (lineType!=NULL) delete [] lineType;
  if (subCurveBound[0]!=NULL) delete [] subCurveBound[0];
  if (subCurveBound[1]!=NULL) delete [] subCurveBound[1];
}




// int 
// plotAListOfMappings(MappingInformation & mapInfo, 
// 		    const int & mapNumberToPlot,
// 		    int & numberOfMapsPlotted,   
// 		    IntegerArray & listOfMapsToPlot, 
// 		    aString *localColourNames,
// 		    const int & numberOfColourNames,
// 		    const bool & plotTheAxes,
//                     GraphicsParameters & params )
// // =======================================================================================================
// // Plot a new Mapping, given that other Mapping's have already been plotted
// // /mapNumberToPlot (input) : number in mapInfo.mappingList of mapping to plot (if >=0), if <0 then redraw
// //    all the mapping's
// // /numberOfMapsPlotted (input/output) : Mappings that have been plotted so far
// // /listOfMapsToPlot (input/output) :numbers in mapInfo.mappingList of mappings plotted so far
// // =======================================================================================================
// {
//   assert(mapInfo.graphXInterface!=NULL);
//   GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
//   if( !gi.graphicsIsOn() )
//     return 0;

//   if( plotTheAxes )
//     gi.plotTheAxes=1;
//   else
//     gi.plotTheAxes=0;
    
//   int redraw=0;
//   if( mapNumberToPlot > 0 )
//   {
//     // plot the new mapping 
//     params.set(GI_MAPPING_COLOUR,localColourNames[numberOfMapsPlotted % numberOfColourNames]);
//     PlotIt::plot(gi,*(mapInfo.mappingList[mapNumberToPlot].mapPointer),params);

//     listOfMapsToPlot(numberOfMapsPlotted++)=mapNumberToPlot;
    
//     if( params.plotBoundsChanged )
//     {
//       cout << "plotAListOfMappings: ERROR: plot bounds have changed! something is wrong here \n";
//       redraw=2;
//     }
//     else
//       redraw=0;
//   }


//   int i;
//   if( (redraw==2 && numberOfMapsPlotted>1) || mapNumberToPlot<0 )
//   {

//     gi.erase();
//     for( i=0; i<numberOfMapsPlotted; i++)
//     {
//       params.set(GI_MAPPING_COLOUR,localColourNames[i % numberOfColourNames]);
//       Mapping * map = mapInfo.mappingList[listOfMapsToPlot(i)].mapPointer;
//       PlotIt::plot(gi,*map,params);
//       if( gi.plotTheAxes )
// 	gi.plotTheAxes=max(gi.plotTheAxes,map->getRangeDimension()); // to get coorect dimension for axes

//     }
//     redraw=0;

//     if( params.labelGridsAndBoundaries )
//     {
//       cout << "++++++++++++ draw coloured squares +++++++++++ \n";
    
//       IntegerArray numberList(min(numberOfColourNames,numberOfMapsPlotted)); numberList=0;
//       for( i=0; i<min(numberOfColourNames,numberOfMapsPlotted); i++)
// 	numberList(i)=i;
    
//       gi.drawColouredSquares(numberList, params, numberOfColourNames, localColourNames);
//     }
//   }
  
//   return 0;
// }

int MappingsFromCAD::
getTransformationMatrix(const int & item, IgesReader & iges, RealArray & rotation, RealArray & translation)
// =========================================================================================================
/// \details 
///      Extract the rotation matrix and translation vector from the iges file.
/// \param item (input): item ID for a transformationMatrix
/// \param iges (input): 
/// \param rotation, translation (output):
// =========================================================================================================
{

  if( iges.entity(item) != IgesReader::transformationMatrix )
  {
    cout << "*** getTransformationMatrix: This is not a transformation matrix! \n";
    rotation=0;
    rotation(0,0)=1.;
    rotation(1,1)=1.;
    rotation(2,2)=1.;
    translation=0;
    return 1;
  }
  else
  {
    int formOfMatrix = iges.formData(item);
    realArray data(13);
    iges.readData(item,data,13);

//     int matrix=iges.matrix(item);
//     if ( matrix )
//       {
// 	cout<<"WARNING : transformation matrix has another transformation matrix! "<<endl;
// 	matrix=iges.sequenceToItem(matrix);
//       }

    switch(formOfMatrix)
    {
    case 0:
      rotation(0,0) = data(1);
      rotation(0,1) = data(2);
      rotation(0,2) = data(3);

      rotation(1,0) = data(5);
      rotation(1,1) = data(6);
      rotation(1,2) = data(7);

      rotation(2,0) = data(9);
      rotation(2,1) = data(10);
      rotation(2,2) = data(11);

      translation(0) = data(4);
      translation(1) = data(8);
      translation(2) = data(12);

      break;
    default:
      rotation=0;
      rotation(0,0)=1.;
      rotation(1,1)=1.;
      rotation(2,2)=1.;
      translation=0;
             
      printf("***getTransformationMatrix : Matrix form not recognized");
    }
    
  }
  return(0);
}

realArray
applyMatrixTransform(realArray & v, const RealArray & rotation, const RealArray & translation )
// ==============================================================================================
// /Desciption:
//   Apply the matrix transformation and translation to a vector v.
//
// ==============================================================================================
{
  realArray x(3);

  for( int axis=0; axis<3; axis++ )
    x(axis)=rotation(axis,0)*v(0)+rotation(axis,1)*v(1)+rotation(axis,2)*v(2)+translation(axis);

  return x;
}

realArray
applyMatrixTransform(realArray & v, const RealArray & rotation )
// ==============================================================================================
// /Desciption:
//   Apply the matrix transformation rotation.
//
// ==============================================================================================
{
  realArray x(3);

  for( int axis=0; axis<3; axis++ )
    x(axis)=rotation(axis,0)*v(0)+rotation(axis,1)*v(1)+rotation(axis,2)*v(2);

  return x;
}


int MappingsFromCAD::
readOneCurve(int curve,
	     IgesReader & iges, 
	     Mapping * & mapPointer,
             RealArray & curveParameterScale)
// ===============================================================================================
/// \details 
///      Read a curve from the iges file.
/// \param item (input): item representing a curve
/// \return  0 for success, 1 if unable to build the curve.
// ===============================================================================================
{
//  cout << "ReadOneCurve: The curve is a " << iges.entityName(iges.entity(curve)) << endl;
  real time0=getCPU();
  
  curveParameterScale(0)=0.;
  curveParameterScale(1)=1.;
  
  if( iges.entity(curve)==IgesReader::rationalBSplineCurve || // 126
      iges.entity(curve)==IgesReader::parametricSplineCurve )  // 112
  {
    mapPointer = new NurbsMapping();  
    mapPointer->incrementReferenceCount();
    ((NurbsMapping*)mapPointer)->readFromIgesFile(iges,curve);
    curveParameterScale(0) = ((NurbsMapping*)mapPointer)->getOriginalDomainBound(0,0);
    curveParameterScale(1) = ((NurbsMapping*)mapPointer)->getOriginalDomainBound(1,0);
  }
  else if( iges.entity(curve)==IgesReader::line ) // 110
  {
    // line: data(1)=x0, data(2)=y0, data(3)=z0,
    // line: data(4)=x1, data(5)=y1, data(6)=z1,
    realArray data(7); // need a new array
    iges.readData(curve,data,7);
      
    real x0=data(1), y0=data(2), z0=data(3);
    real x1=data(4), y1=data(5), z1=data(6);

    mapPointer = new LineMapping(data(1),data(2),data(3),data(4),data(5),data(6),2);  // 2=number of points
    mapPointer->incrementReferenceCount();
  }
  else if( iges.entity(curve)==IgesReader::circularArc ) // 100
  { // note arc lies in a z-plane, z=data(1)

//      printf("readOneCurve:circular arc found: DE sequence number=%i, matrix=%i \n",iges.sequenceNumber(curve),
//               iges.matrix(curve));

    RealArray data(10);
    
    iges.readData(curve,data,8);
    RealArray o(3),pt1(3),pt2(3);
    
    o(0)=data(2);
    o(1)=data(3);
    o(2)=data(1);
    pt1(0)=data(4);
    pt1(1)=data(5);
    pt1(2)=data(1);

    pt2(0)=data(6);
    pt2(1)=data(7);
    pt2(2)=data(1);

    int matrix=0;
    RealArray matrixTransform(3,3),translation(3);
    if( iges.matrix(curve)!=0 )
    {
      matrix=iges.matrix(curve);
      matrix=iges.sequenceToItem(matrix);
      int returnValue=getTransformationMatrix(matrix,iges,matrixTransform,translation);
      if( returnValue==0 && Mapping::debug & 1 )
      {
	display(matrixTransform,"circularArc: matrixTransform");
	display(translation,"circularArc: translation");
      }
    }


    pt1-=o;
    pt2-=o;
    real radius=SQRT(sum(pt1*pt1));
    real radius2=SQRT(sum(pt2*pt2));
    if( radius==0. )
    {
      printf("ERROR: reading a circular arc from an IGES file: radius==0\n");
      throw "error";
    }
    else if( fabs(radius-radius2)> 1.e-3*radius )
    {
      printf("ERROR: reading a circular arc from an IGES file: radius=%e != radius2=%e diff=%e\n",
           radius,radius2,fabs(radius-radius2));
      throw "error";
    }
    
    if( Mapping::debug & 1 )
    {
      printf("circularArc: radius=%e, centre=(%e,%e,%e)\n",radius,o(0),o(1),o(2));
      printf("           : pt1=(%e,%e,%e) pt2=(%e,%e,%e) \n",
	     pt1(0)+o(0),pt1(1)+o(1),pt1(2)+o(2),
	     pt2(0)+o(0),pt2(1)+o(1),pt2(2)+o(2) );
    }
    
    pt1/=radius;
    pt2/=radius2;
    
    // 0 <= theta1 <= twoPi
    real cos1=pt1(0), sin1=pt1(1);
    real theta1=atan2((double)sin1,(double)cos1);

    if( theta1<0. ) theta1+=Pi;

    // theta1 <= theta2 <= twoPi+theta1
    real cos2=pt2(0), sin2=pt2(1);
    real theta2=atan2((double)sin2,(double)cos2);
    if( theta2<=theta1 ) theta2+=twoPi;
    
    real theta = theta2-theta1;

    if( Mapping::debug & 1 ) 
      printf("readMappings::circularArc: theta1=%e theta2=%e, delta-theta=%e \n",theta1,theta2,theta);

    if( theta>0. ) // *wdh* added 031119 check for theta>0. (f16)
    {
      curveParameterScale(0)=theta1;
      curveParameterScale(1)=theta2;
    }
    else
    {
      curveParameterScale(0)=0.;  // *wdh* added 031119 (f16)
      curveParameterScale(1)=Pi;
    }
    
    pt2(0)=-pt1(1);
    pt2(1)= pt1(0);
     
    

    real startAngle=0.;
    real endAngle=theta/twoPi;  // endAngle in [0,1]
    
    mapPointer = new NurbsMapping();  
    mapPointer->incrementReferenceCount();
    NurbsMapping & map = *((NurbsMapping*)mapPointer);

    if( Mapping::debug & 4 )
      printf("**** readMappings:circularArc: center=(%e,%e,%e) v1=(%e,%e,%e) v2=(%e,%e,%e) \n"
             "  (v1.v2=0?) radius=%e startAngle=%e endAngle=%e\n",
	     o(0),o(1),o(2),pt1(0),pt1(1),pt1(2), pt2(0),pt2(1),pt2(2),radius,startAngle,endAngle);
    // o= center
    // pt1, pt2 : orthogonal unit vectors
    map.circle( o,pt1,pt2,radius,startAngle,endAngle);
    

    if( Mapping::debug & 1 )
    {
      // check
      realArray r(3,1), x(3,3);
      r(0,0)=0.;
      r(1,0)=.5;
      r(2,0)=1.;
    

      map.map(r,x);
    
      printf("Before any transform circle : x(r=0)=(%10.5e,%10.5e,%10.5e) x(r=.5)=(%10.5e,%10.5e,%10.5e) "
             "x(r=1)=(%10.5e,%10.5e,%10.5e)\n",
	     x(0,0),x(0,1),x(0,2),x(1,0),x(1,1),x(1,2),x(2,0),x(2,1),x(2,2));
    }

    if( matrix!=0 )
    {
      // apply the matrix and translation operators
      map.matrixTransform(matrixTransform);
      map.shift(translation(0),translation(1),translation(2));
      if( Mapping::debug & 1 )
      {
	display(matrixTransform,"circularArc: matrixTransform");
	display(translation,"circularArc: translation");
      }
    }
    return 0;
  }
  else if( iges.entity(curve)==IgesReader::conicArc ) // 104
  {

    // NOTE: This could be a 3D curve defining a tabulated cylinder
    realArray data(12);
    iges.readData(curve,data,12);

    real a,b,c,d,e,f,zt,x1,y1,x2,y2;
    a=data(1); 
    b=data(2); 
    c=data(3); 
    d=data(4); 
    e=data(5); 
    f=data(6); 
    zt=data(7); 
    x1=data(8); 
    y1=data(9); 
    x2=data(10); 
    y2=data(11); 
    
    int form = iges.formData(curve);

//   form doesn't seem correct ??
//      printf("***readOneCurve: conicArc found: form=%i (1=ell,2=hyp,3=par)  a,b,c,d,e,f=%f,%f,%f,%f,%f,%f, "
//  	   "(x1,y1)=(%f,%f) (x2,y2)=(%f,%f) zt= %f\n",form,a,b,c,d,e,f,x1,y1,x2,y2,zt);
    
    int matrix=0;
    RealArray matrixTransform(3,3),translation(3);
    if( iges.matrix(curve)!=0 )
    {
      matrix=iges.matrix(curve);
      matrix=iges.sequenceToItem(matrix);
      int returnValue=getTransformationMatrix(matrix,iges,matrixTransform,translation);
      if( (Mapping::debug & 1) && returnValue==0  )
      {
	display(matrixTransform,"conicArc: matrixTransform");
	display(translation,"conicArc: translation");
      }
    }

    mapPointer = new NurbsMapping();  
    mapPointer->incrementReferenceCount();
    NurbsMapping & map = *((NurbsMapping*)mapPointer);
    
    map.conic( a,b,c,d,e,f,zt,x1,y1,x2,y2 );

    if( matrix!=0 )
    {
      // apply the matrix and translation operators
      map.matrixTransform(matrixTransform);
      map.shift(translation(0),translation(1),translation(2));
    }

    // Overture::abort("Error");
    
  }
  else 
  { 
    printf("readOneCurve:ERROR: curve %i has unknown type = %i\n", curve, iges.entity(curve));
    mapPointer = NULL;
    return 1;
  }
  timeToBuildNurbsCurves+=getCPU()-time0;
  
  return 0;
}

int MappingsFromCAD::
scaleCurve( Mapping & mapping, Mapping & surf, RealArray & surfaceParameterScale)
// =====================================================================================
/// \details 
///     Scale a mapping used in the parameter space of a surface to [0,1]x[0,1]. This
///  is necesssary since Mappings always have a [0,1]x[0,1] parameter space but IGES surfaces
///  may not. For example, a surface of revolution may be parameterized by $[0,2\pi]$ in the IGES
///  representation.
/// \param mapping (input/output): scale this mapping
/// \param surface (input): scale mapping to lie in the parameter space of this surface.
/// \param surfaceParameterScale (input) : defines the scaling factors for som types of surfaces.
// =========================================================================================
{
  if( mapping.getClassName()=="NurbsMapping" )
  {
    NurbsMapping & nurb = (NurbsMapping&)mapping;
    if( surf.getClassName()=="NurbsMapping" )
    {
      NurbsMapping & nurbSurf = (NurbsMapping&)surf;
      nurb.parametricCurve(nurbSurf); // this will scale curve to fit [0,1] parameter space and make 2D
    }
    else if( surf.getClassName()=="RevolutionMapping" )
    {
      nurb.parametricCurve(nurb,FALSE); // this will make the curve 2D
      RevolutionMapping & revSurf = (RevolutionMapping&)surf;
      real sa, ta;
      revSurf.getRevolutionAngle( sa, ta );
      if( Mapping::debug & 2 )
      {
	printf("scaleCurve: (from createCompositeCurve): scale curve to a RevolutionMapping:getRevolutionAngle: sa=%e,ta=%e\n",sa,ta);
	// printf("createCompoisteCurve: line: (%e,%e) to (%e,%e)\n",x0,y0,x1,y1);
      
	// nurb.shift(0., -sa*twoPi, 0.);
	// nurb.scale(1.,1./(ta-sa)/twoPi,1.); // shift and scale the nurb to [0,1]
	printf("scaleCurve: surfaceParameterScale(0:1,0)=(%e,%e) \n",surfaceParameterScale(0,0),surfaceParameterScale(1,0));
      
	// printf("scaleCurve: surfaceParameterScale(0:1,0)=(%e,%e) \n",
	//                     surfaceParameterScale(0,0),surfaceParameterScale(1,0));
	//      nurb.getControlPoints().display("cp before scale");
      }
      
      real shift0=surfaceParameterScale(0,0);
      nurb.shift(-shift0, -sa*twoPi, 0.);
      real scale0=surfaceParameterScale(1,0)-surfaceParameterScale(0,0);
      assert( scale0>0. );
      if( Mapping::debug & 2 ) 
      {
	printf(" scaleCurve: Shift `r' by %9.3e then scale by %9.3e = 1./%9.3e\n",-shift0,1./scale0,scale0);
	printf(" scaleCurve: shift theta by %e then scale theta by 1/(twoPi*(ta-sa))=%e \n",
	       -sa*twoPi, 1./(ta-sa)/twoPi);
      }
      
      nurb.scale(1./scale0,1./(ta-sa)/twoPi,1.); // shift and scale the nurb to [0,1]
      if( Mapping::debug & 8 ) 
        nurb.getControlPoints().display("cp after scale");
    }
    else
    {
      nurb.parametricCurve(nurb,FALSE); // this will make the curve 2D
    }
  }
  return 0;
}
  

int MappingsFromCAD::
createCompositeCurve(const int & item, 
		     IgesReader & iges, 
                     int & maximumNumberOfSubCurves,
                     int & numberOfSubCurves, 
		     Mapping ** & mapPointer,
                     Mapping *surf,
                     RealArray & surfaceParameterScale,
		     bool scaleTheCurve/*=true*/,
                     bool expectPhysicalSpaceCurve /* =false */ )
// =================================================================================
/// \details 
///      Create a composite curve and return pointers to the sub-curves.
/// 
/// \param maximumNumberOfSubCurves (input/output) : on input the max allowed, could be increased.
/// \param numberOfSubCurves (output) : on output the actual number
/// 
/// \param mapPointer (output): an array of Mappings. mapPointer[0] points to the composite curve
///  that is the merged version of the sub curves. mapPointer[i] i=1,2,...,numberOfSubCurves
///  points to the sub-curve.
/// \param surf (input): This is the NURBS surface that the composite curve sits on (i.e. is in the 
///   parametric space of).
/// 
/// \param boundaryType (output) : simpleReparameterization means the composite curve marked out a
///     rectangle in parameter space.
/// \param curveBound[2][2] (output) : if the boundaryType is a simpleReparameterization then these values 
///    mark the rectangle bounds.
/// \param subCurveBounds[2][2] (output) : bounds on the subCurve.
/// \param expectPhysicalSpaceCurve (input) : if true expect physcial space curves with rangeDimension==3
/// 
/// \param Remark: in the IGES file, the data for a composite curve is
///  \begin{verbatim}
///      0 : entity number
///      1 : number of sub-curves      
///      2 : pointer to descriptor of first sub-curve
///      3 : pointer to descriptor of 2nd sub-curve
///      ...
///  \end{verbatim}
// =============================================================
{
  assert( mapInfoPointer!=NULL );
  MappingInformation &mapInfo = *mapInfoPointer;

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  char buff[80];
  realArray tangent(2,2,5);

  realArray data(2);
  iges.readData(item,data,2);

  int numberOfCurves = (int)data(1);

//  Mapping::debug=7; // 15; 

  if( Mapping::debug & 2 )
    cout<<"number of curves on compositeCurve : "<<numberOfCurves<<endl;

  if( numberOfCurves>=maximumNumberOfSubCurves || (lineType==NULL) )
  {
    maximumNumberOfSubCurves=numberOfCurves+2;
    //cout << "createCompositeCurve:WARNING: increasing maximumNumberOfSubCurves to " << maximumNumberOfSubCurves << endl;
    if (mapPointer!=NULL) delete [] mapPointer;
    if (lineType!=NULL) delete [] lineType;
    if (subCurveBound[0]!=NULL) delete [] subCurveBound[0];
    if (subCurveBound[1]!=NULL) delete [] subCurveBound[1];

    mapPointer = new Mapping * [maximumNumberOfSubCurves];
    lineType = new int[maximumNumberOfSubCurves];
    subCurveBound[0] = new real[maximumNumberOfSubCurves];
    subCurveBound[1] = new real[maximumNumberOfSubCurves];

  }

  int i;
  for( i=0; i<maximumNumberOfSubCurves; i++ )
    {
      mapPointer[i]=NULL;
      lineType[i] = general;
    }

  data.redim(2+numberOfCurves);
  iges.readData(item,data,2+numberOfCurves);
  realArray r(1,1),x1(1,3),x2(1,3);
 
  // mapPointer[0] holds merged curve
  // mapPointer[i] holds sub-curve i=1,2,..

  real expectedArcLength = 0;

  int status=0;
  for( i=1; i<=numberOfCurves; i++ )
  {
    lineType[i]=general;

    real time3=getCPU();
    int sequence = (int)data(1+i);
    int curve = iges.sequenceToItem(sequence);
    timeToBuildCurvesOther+=getCPU()-time3;
    
    if( Mapping::debug & 4 )
      cout << "compositeCurve, sub-curve=" << i << ", of " << numberOfCurves << ", seq = " << sequence
           << " is an " << iges.entityName(iges.entity(curve)) << ". curve = " << curve 
	   << " iges.entity(curve) = " << iges.entity(curve) << endl;

    if( iges.entity(curve)==IgesReader::rationalBSplineCurve ||  // 126
	iges.entity(curve)==IgesReader::parametricSplineCurve ||  // 112
        iges.entity(curve)==IgesReader::circularArc ) // 100
    { 
      real time1=getCPU();
      mapPointer[i] = new NurbsMapping();  mapPointer[i]->incrementReferenceCount();

      ((NurbsMapping*)mapPointer[i])->readFromIgesFile(iges,curve);
      // kkc 051401
      ((NurbsMapping*)mapPointer[i])->truncateToDomainBounds();

      // scale the parametric curve to [0,1]x[0,1]
      if ( scaleTheCurve ) scaleCurve( *mapPointer[i],*surf,surfaceParameterScale);

      timeToBuildNurbsCurves+=getCPU()-time1;

#ifdef INLINEMERGE
      if( i>1 )
      { // merge curves if possible 
//        int merge = ((NurbsMapping*)mapPointer[0])->forcedMerge((NurbsMapping&)(*mapPointer[i]));
        real time2=getCPU();
        int merge = ((NurbsMapping*)mapPointer[0])->merge((NurbsMapping&)(*mapPointer[i]), true);
        timeToMergeNurbsCurves+=getCPU()-time2;

	if( merge!=0 )
	{
	  status=1;
	  printf("compositeCurve::ERROR unable to merge curves! (rationalBSplineCurve)"
		 " curve=%i out of numberOfCurves=%i \n",i,numberOfCurves);
	}
	
      }
#endif
    }
    else if( iges.entity(curve)==IgesReader::line ) // 110
    {
      // line: data(1)=x0, data(2)=y0, data(3)=z0,
      // line: data(4)=x1, data(5)=y1, data(6)=z1,
      real time1=getCPU();

      realArray data(7); // need a new array
      iges.readData(curve,data,7);
      
      real x0=data(1), y0=data(2), z0=data(3);
      real x1=data(4), y1=data(5), z1=data(6);

      // kkc 050512 in some circumstances we are actually building a physical space curve
      bool hasZ = expectPhysicalSpaceCurve || ( z0!=0. || z1!=0. );
      if( hasZ &&  !expectPhysicalSpaceCurve  )
	printf("****** WARNING: parameter space curve expected but z values for parametric line are not zero!!\n"
               "    z0=%8.2e z1=%8.2e ****\n",z0,z1);
	
      mapPointer[i] = new NurbsMapping; mapPointer[i]->incrementReferenceCount();

      const int rd = hasZ ? 3 : 2;
      RealArray p1(1,rd), p2(1,rd);
      p1(0,0) = x0;
      p1(0,1) = y0;
      p2(0,0) = x1;
      p2(0,1) = y1;
      if ( hasZ )
	{
	  p1(0,2) = z0;
	  p2(0,2) = z1;
	}

      if( Mapping::debug & 4 )
      {
	printf("createCompositeCurve:line: end pts: x0=(%9.3e,%9.3e,%9.3e,) x1=(%9.3e,%9.3e,%9.3e,)\n",
               x0,y0,z0,x1,y1,z1);
      }
      
      ((NurbsMapping*)mapPointer[i])->line(p1,p2);

      if( Mapping::debug & 4 )
        cout << "Nurbs for line has " << mapPointer[i]->getGridDimensions(axis1) << " points \n";

      // scale the parametric curve to [0,1]x[0,1]
      if ( scaleTheCurve ) scaleCurve( *mapPointer[i],*surf,surfaceParameterScale);
      ((NurbsMapping&)(*mapPointer[i])).truncateToDomainBounds();
      timeToBuildNurbsCurves+=getCPU()-time1;
      
#ifdef INLINEMERGE
      if( i>1 )
      { // merge curves if possible 
//        int merge = ((NurbsMapping*)mapPointer[0])->forcedMerge((NurbsMapping&)(*mapPointer[i]));
        real time2=getCPU();
        int merge = ((NurbsMapping*)mapPointer[0])->merge((NurbsMapping&)(*mapPointer[i]), true);
        timeToMergeNurbsCurves+=getCPU()-time2;
	if( merge!=0 )
	  {
	    status=1;
	    printf("createCompositeCurve::ERROR unable to merge curves! (line) i=%i, numberOfCurves=%i \n",
		   i,numberOfCurves);
	  }

      }
#endif
    }
    else 
    { 
      printf("createCompositeCurve:ERROR: curve %i has unknown type = %i\n", curve, iges.entity(curve));
      {throw "error";}
    }

    expectedArcLength += mapPointer[i]->getArcLength();

// tmp
    if( Mapping::debug & 16 )
    {
      params.set(GI_TOP_LABEL,"composite curve (physical space curve)");
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      if (i>1 && mapPointer[0])
      {
 	NurbsMapping *nm= (NurbsMapping *)mapPointer[0];
 	for (int q=0; q<nm->numberOfSubCurvesInList(); q++)
 	  printf("subcurve %i, globalID=%i, original=%i\n", q, nm->subCurveFromList(q).getGlobalID(), 
                   nm->isSubCurveOriginal(q));
      
	PlotIt::plot(gi,*mapPointer[0],params);
      }
      else if (i==1 && mapPointer[1])
      {
	printf("GlobalID of subcurve 1: %i\n", mapPointer[1]->getGlobalID());
	PlotIt::plot(gi,*mapPointer[1],params);
      }
    
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      params.set(GI_TOP_LABEL," ");
    }

    if( numberOfCurves==4 && 
        mapPointer[i]->getRangeDimension()==2 ) // *wdh* 050610 -- only check parameter curves
    {
      // determine if the curve is horizontal or vertical
      real time1=getCPU();

//      int n= max( 11, mapPointer[i]->getGridDimensions(axis1));
//       realArray r(n,1),x(n,2);
//       assert( n>1 );
//       real dr=1./(n-1);
//       r.seqAdd(0.,dr);
//       mapPointer[i]->map(r,x);

      realArray x;
      x.reference(mapPointer[i]->getGrid());
      Range R=x.dimension(0);
      int n=x.getLength(0);
      x.reshape(R,x.dimension(3));
      // x.display("here is x for the sub curve");
    
      real eps = FLT_EPSILON*50.;  // **** relative tolerance for a straight curve *****

      // save tangent to look for corners between subcurves
      Range Axes(0,1);
      
      tangent(Start,Axes,i)=x(1,Axes)-x(0,Axes);
      real norm = SQRT( SQR(tangent(Start,0,i)) + SQR(tangent(Start,1,i)) );
      if( norm < eps )
      {
	printf("createCompositeCurve: error: tangent is zero at start. norm=%8.2e (n=%i)\n",norm,n);
      }
      else
        tangent(Start,Axes,i)/norm;
      
      tangent(End  ,Axes,i)=x(n-1,Axes)-x(n-2,Axes);
      norm = SQRT( SQR(tangent(End,0,i)) + SQR(tangent(End,1,i)) );
      if( norm < eps )
      {
	printf("createCompositeCurve: error: tangent is zero at end. norm=%8.2e (n=%i)\n",norm,n);
      }
      else
        tangent(End,Axes,i)/norm;

      real xMin=min(x(R,axis1));
      real xMax=max(x(R,axis1));
      real yMin=min(x(R,axis2));
      real yMax=max(x(R,axis2));

      if( Mapping::debug & 4 )
        printf(" subCurve %i, xMax=%e, xMin=%e, yMax=%e, yMin=%e \n",i,xMax,xMin,yMax,yMin);
    
      if( xMax-xMin < eps*(fabs(xMax)+fabs(xMin)) ) // This will fail if xMax=xMin=0!!!
      {
        if( Mapping::debug & 4 )
	  cout << "subcurve" << i << " is vertical \n";
	lineType[i]=vertical;
        assert( i<20 );   // fix this 
	subCurveBound[Start][i]=yMin;
	subCurveBound[End  ][i]=yMax;
      }
      else if( yMax-yMin < eps*(fabs(yMax)+fabs(yMin)) ) // This will fail if yMax=yMin=0!!!
      {
        if( Mapping::debug & 4 )
	  cout << "subcurve" << i << " is horizontal \n";
	lineType[i]=horizontal;
        assert( i<20 );
	subCurveBound[Start][i]=xMin;
	subCurveBound[End  ][i]=xMax;
      }

      timeToAdjustCurves+=getCPU()-time1;
    } // end if numberOfCurves==4
    
    
    
    if( i==1 )
    { // here we start the merged curve
      if( Mapping::debug & 2 )
      {
	printf(" **** CREATE mapPointer[0] item=%i\n",item);
      }
      real time1=getCPU();
      mapPointer[0] = new NurbsMapping();  mapPointer[0]->incrementReferenceCount();

      (NurbsMapping&)(*(mapPointer[0]))=(NurbsMapping&)(*(mapPointer[1])); // deep copy
      mapPointer[0]->setName(Mapping::mappingName,sPrintF(buff,"mapPointer[0] item=%i",item));

      timeToCopyNurbsCurves+=getCPU()-time1;
       
      // PlotIt::plot(gi,*mapPointer[i],params);
    }
    /* ----
    if( FALSE && Mapping::debug & 4  )
    {
      params.set(GI_TOP_LABEL,"composite curve[0]");
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      PlotIt::plot(gi,*mapPointer[0]);
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      params.set(GI_TOP_LABEL," ");
    }
    --- */    

  } // end for( i= 

#ifndef INLINEMERGE  
  real mergeTol = 0.001*expectedArcLength;
  bool attemptPeriodicMerge = false;
  //  cout<<"expected arc length is "<<expectedArcLength<<" with tol "<<mergeTol<<endl;
  for ( int j=2; j<=numberOfCurves && numberOfCurves>1; j++ )
    {
      attemptPeriodicMerge = (j==numberOfCurves);
      if ( ((NurbsMapping*)mapPointer[0])->merge((NurbsMapping&)(*mapPointer[j]), true, mergeTol, attemptPeriodicMerge )!=0 )
	{
	  status=1;
	  printf("createCompositeCurve::ERROR unable to merge curves! (line) i=%i, numberOfCurves=%i, mergeTol=%g \n",
		 j,numberOfCurves,mergeTol);
	}
    }
#endif

  // check to see if the curve has any "narrow" regions in it, that is the curve
  // loops around and approaches itself. In this case
  // we want to increase the number of points on the curve so that the algorithms in
  // TrimmedMapping will work better
  if( false && // *wdh* 010831 ************ this is done in TrimmedMapping
      status==0 && mapPointer[0]->getGridDimensions(axis1) > 11 )
  {
    real time1=getCPU();
    
    Mapping & map = *mapPointer[0];

    const int n=map.getGridDimensions(axis1);
    
    realArray r(n,1),x(n,2); 
    const real h=1./(n-1);
    r.seqAdd(0.,h);
    map.map(r,x);
    realArray s(n);
    s(0)=0.;
    int i;
    for( i=1; i<n; i++ )
      s(i)=s(i-1)+SQRT( SQR(x(i,0)-x(i-1,0)) + SQR(x(i,1)-x(i-1,1)) );
    
    // ::display(s," Here is s","%6.2e ");
    const int increment=max(5,n/20);  // check at most 20 points
    real ratio=10., arcDist;
    const int iStart=  increment/2;
    const int iEnd  =n-increment/2;
    for( i=iStart; i<iEnd; i+=increment )
    {
      arcDist = fabs(s(i+1)-s(i-1));
      if( arcDist>0. )
      {
	for( int j=iStart; j<iEnd; j+=increment )
	{
	  if( j!=i )
	  {
	    real dist = SQRT( SQR(x(i,0)-x(j,0)) + SQR(x(i,1)-x(j,1)) );
	    real d= fabs(s(j)-s(i));
	    // printf(" i=%i, j=%i, dist=%e arcDist=%e, ratio=%e\n",i,j,dist,arcDist,dist/arcDist);
	    ratio = min(ratio, dist/arcDist );
	  
	  }
	}
      }
    }
    if( ratio<1. )
    {
      const int maximumNumberOfPointsOnTheCurve=301;
      int newGridPoints = ratio>0. ? min( max(n,int(1.5*n/ratio)),maximumNumberOfPointsOnTheCurve) :
	maximumNumberOfPointsOnTheCurve;
      if( Mapping::debug & 4 )
        printf("++++createCompositeCurve: n=%i, min (distance(i,j)/arclength(i,j)) = %e new n=%i +++++++\n",
	     n,ratio,newGridPoints);
      map.setGridDimensions(axis1, newGridPoints);
    }
    
    timeToAdjustCurves+=getCPU()-time1;
  }
  // status ==1 : ERROR: unable to merge the curves.
  // Plot the curves to see what is going on.
  if( status==1 )
  {
    if( status==1 )
    {
      printf("status=1: ERROR unable to merge the compositeCurve.\n");
      printf(" curve[0] is the partial composite curve\n");
    }
    if( false || Mapping::debug & 4 ) // *wdh* 050610
    {
      printf("status=1: ERROR unable to merge the compositeCurve. here are the curves to be plotted\n");
      MappingInformation mapInfo;
      mapInfo.graphXInterface=&gi;
      char buff[80];
      for( int j=0; j<=numberOfCurves; j++ )
      {
	(*mapPointer[j]).setName(Mapping::mappingName,sPrintF(buff,"curve[%i]",j));
	mapInfo.mappingList.addElement(*mapPointer[j]);
      }
#if 1
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      gi.erase();
      viewMappings(mapInfo);
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
#endif
    }
  }
  else
  {
    if( Mapping::debug & 4 )
    {
      printf("CompositeCurve was merged succesfully\n"); 

      params.set(GI_TOP_LABEL,"merged composite curve");
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      PlotIt::plot(gi,*mapPointer[0],params);
    
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      params.set(GI_TOP_LABEL," ");
    }

  }
  
  
  // Sometimes the trimmed region is just a "rectangle" and can be represented more simply
  // by a reparameterization or forming a Coon's patch (transf-finite interpolation).

  boundaryType=generalBoundary;
  if( numberOfCurves==4 )
  {
    if( lineType[1]==horizontal && lineType[2]==vertical && lineType[3]==horizontal && lineType[4]==vertical )
    {
      if( Mapping::debug & 2 )
        cout << "\n ++++++++++++ simpleReparameterization ++++++++ \n";
      
      boundaryType=simpleReparameterization;
      curveBound[Start][axis1]=subCurveBound[Start][1];
      curveBound[End  ][axis1]=subCurveBound[End  ][1];
      curveBound[Start][axis2]=subCurveBound[Start][2];
      curveBound[End  ][axis2]=subCurveBound[End  ][2];
    }
    else if( lineType[1]==vertical && lineType[2]==horizontal && lineType[3]==vertical && lineType[4]==horizontal )
    {
      if( Mapping::debug & 2 )
        cout << "\n ++++++++++++ simpleReparameterization ++++++++ \n";

      boundaryType=simpleReparameterization;
      curveBound[Start][axis1]=subCurveBound[Start][2];
      curveBound[End  ][axis1]=subCurveBound[End  ][2];
      curveBound[Start][axis2]=subCurveBound[Start][1];
      curveBound[End  ][axis2]=subCurveBound[End  ][1];
    }
    else if( (lineType[1]==horizontal && lineType[3]==horizontal) || 
             (lineType[1]==vertical   && lineType[3]==vertical  )  )
    {
      if( Mapping::debug & 2 )
        cout << "\n ++++++++++++ sides 1 and 3 are straight : coons24 patch ++++++++ \n";
      if( lineType[1]==horizontal )
        boundaryType=coons24LeftRight;
      else
        boundaryType=coons24BottomTop;
    }
    else if( (lineType[2]==horizontal && lineType[4]==horizontal) || 
             (lineType[2]==vertical   && lineType[4]==vertical  )  )
    {
      if( Mapping::debug & 2 )
        cout << "\n ++++++++++++ sides 2 and 4 are straight : coons13 patch ++++++++ \n";

      if( lineType[2]==horizontal )   
        boundaryType=coons13LeftRight;
      else
        boundaryType=coons13BottomTop;
    }
    else
    {
      // make sure that there are 4 corners
      bool fourCorners=TRUE;
      for( i=1; i<5; i++ ) // i = curve number
      {
        int ip1 = (i % 4) +1 ;
        real curl = tangent(End,0,i)*tangent(Start,1,ip1)-tangent(End,1,i)*tangent(Start,0,ip1);
        fourCorners = fourCorners && ( fabs(asin(curl)) > Pi*.25 );  // a corner if angle > 45 degress
        if( !fourCorners )
          break;
      }
      if( fourCorners )      
      {
        if( Mapping::debug & 4 )
          cout << "\n ++++++++++++ general coons ++++++++ \n";
        boundaryType=coons;
      }
      else
      {
        if( Mapping::debug & 4 )
          cout << " \n ++++++++++ 4 sides but not four corners --- no coons used +++++++++ \n";
      }
    }
  }

  numberOfSubCurves=numberOfCurves;

  return status;

}


int MappingsFromCAD::
createCurveOnAParametricSurface(const int & item, 
				IgesReader & iges, 
                                Mapping *surf, 
				Mapping *&mapPointer,
                                int & maximumNumberOfSubCurves,
                                Mapping ** & subCurve,
                                RealArray & surfaceParameterScale)
// =================================================================================================
/// \details 
///       Build a curve on a parametric surface. This curve will normally be a trimming curve.
/// 
/// \param surf (input) : pointer to the surface in which the parametric curve lives.
/// \param mapPointer (output): Pointer to the merged curve
/// \param subCurve[i] (output): pointers to the sub curves.
/// 
///  The data for the trimmed surface in the IGES file is
///  \begin{verbatim}
///  data(0) : entity number 
///  data(1) : indicates the way the curve on the surface has been created
///               0 = Unspecified
///               1 = Projection of a given curve on the surface
///               2 = Intersection of two surfaces
///               3 = Isoparametric curve, i.e. either a u or v-parametric 
///  data(2) : Pointer to DE of the surface on which the curve lies 
///  data(3) : Pointer to DE of the entity that contains the definition of
///               the curve B in the parametric space (u,v) of the surface 
///  data(4) : Pointer to the DE of the curve C 
///  data(5) : Indicates preferred representation in the sending system:
///                0 = Unspecified
///               1 = S o B is preferred
///               2 = C is preferred
///               3 = C and S o B are equally preferred 
///  \end{verbatim}
// ================================================================================================
{
  // cout << "createCurveOnAParametricSurface \n";
  real time0=getCPU();
  
  assert( mapInfoPointer!=NULL );
  MappingInformation &mapInfo = *mapInfoPointer;
  
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  realArray data(6);
  iges.readData(item,data,6);
  
  int sequence = (int)data(3);    // data(3) = parametric curve, data(4)=curve itself


  // 050512 kkc, sometimes the curve is not provided in the parameterspace, only physical space
  //             in this case, read in the curve and project it onto the surface
  //             create a spline from the projected points in the parameter space of the surface
  int prefRep = (int)data(5);
  bool useParameterSpaceCurve = prefRep==0 || prefRep==1 || prefRep==3;

  if( Mapping::debug & 4 )
    printf(" *** createCurveOnAParametricSurface: sequence for parametric curve==%i preferedRep=%i\n",
       sequence,prefRep);

  if ( !useParameterSpaceCurve && 
       sequence==0 )  // always use parameter space curve if it is there since we need to build it somehow
    {
      int physCurve_seq = (int)data(4);
      int physCurve = iges.sequenceToItem(physCurve_seq);
      RealArray curveParameterScale(3);
      Mapping **tmpCurves = 0;

      int matrix = iges.matrix(physCurve);

      int status = 0;
      int nsc=0;
      const int numPointsOnTrimCurve = 21; // 51; // *wdh* 050611 101;

      realArray r(numPointsOnTrimCurve),x(numPointsOnTrimCurve,3),rp(numPointsOnTrimCurve,2);
      r.seqAdd(0.,1./(numPointsOnTrimCurve-1));
      
      for( int i=0; i<maximumNumberOfSubCurves; i++ )
	  subCurve[i]=NULL;

      if( Mapping::debug & 4 )
      {
	cout << "trim curve in physical space " << item << ", seq = " << physCurve_seq
	     << " is a " << iges.entityName(iges.entity(physCurve)) << endl;
      }
      
      //      Mapping::debug = 2;
      if (iges.entity(physCurve)==IgesReader::compositeCurve)
      {
	int tmpMaxNumberOfSubCurves = 0;
	bool expectPhysicalSpaceCurve=true;
	status = createCompositeCurve(physCurve,iges,tmpMaxNumberOfSubCurves,nsc,tmpCurves,surf,
				      surfaceParameterScale,false,expectPhysicalSpaceCurve);
	  
      }
      else
      {
	tmpCurves = new Mapping*[2];
	tmpCurves[0] = 0;
	nsc = 2;
	status = readOneCurve(physCurve, iges, tmpCurves[0], curveParameterScale);
	tmpCurves[0]->incrementReferenceCount();
	tmpCurves[1] = tmpCurves[0];
      }

      if ( !subCurve || nsc>maximumNumberOfSubCurves )
	{
	  if ( nsc>maximumNumberOfSubCurves )
	    {
	      delete [] subCurve;
	    }

	  maximumNumberOfSubCurves = max(maximumNumberOfSubCurves,nsc+1);
	      
	  subCurve = new Mapping *[maximumNumberOfSubCurves];
	  for ( int i=0; i<maximumNumberOfSubCurves; i++ )
	    subCurve[i] = 0;
	}

      NurbsMapping *nurb_p = 0;
      for ( int i=1; i<=nsc; i++ )
	{
	  Mapping *tmpCurve = tmpCurves[i];
	  tmpCurve->map(r,x);
	  if ( matrix!=0 )
	    {
	      cout<<"found a rotation matrix!"<<endl;
	      matrix = iges.sequenceToItem(matrix);
	      RealArray rotation, translation;
	      getTransformationMatrix(matrix, iges, rotation, translation);
	      x = applyMatrixTransform(x, rotation, translation );
	    }

	  //	  x.display("HERE IS X");
	  rp=-1;
	  surf->inverseMap(x,rp);

          if( true )
	  { //  -- double check results --
            // evaluate the mapping at the end-points using the inverse values rp and compare to the original x


	    realArray rc(2,2), xc(2,3), xrc(2,3,2);  // for checking 
            const int np[2]={x.getBase(0),x.getBound(0)};  // check the end pts
            for( int j=0; j<=1; j++ )
	    {
              if( fabs(rp(np[j],0))<2. && fabs(rp(np[j],1))<2. )
	      {
		rc(j,0)=rp(np[j],0); rc(j,1)=rp(np[j],1); 
	      }
	      else
	      { // the end point could not be inverted -- base the point on the neighbouring point
                int ia=np[j];  // end point index
		int ib = ia==0 ? ia+1 : ia-1;  // ib = neighbouring point index

                rc(j,0)= rp(ib,0) < .5 ? 0. : 1.;
                rc(j,1)= rp(ib,1) < .5 ? 0. : 1.;

	      }
	      
	    }

            surf->map(rc,xc,xrc);

	    real xScale=0.;
	    for( int m=0; m<3; m++ )
	      xScale=max(xScale,surf->getRangeBound(End,m)-surf->getRangeBound(Start,m));

            const real eps = 1.e-4; // sqrt(REAL_EPSILON)*10; // REAL_EPSILON*1000.;
            bool xok=true, xrok=true;
            real xDiff[2], xJac[2], xrNorm[2], xsNorm[2];
	    for( int j=0; j<=1; j++ )
	    {
              xDiff[j]=fabs(xc(j,0)-x(np[j],0)) + fabs(xc(j,1)-x(np[j],1)) + fabs(xc(j,2)-x(np[j],2)); 
	      xDiff[j]/=xScale;

              // xJac = | xr X xs |
              xJac[j] = ( fabs( xrc(j,1,0)*xrc(j,2,1) - xrc(j,2,0)*xrc(j,1,1) )+
			  fabs( xrc(j,2,0)*xrc(j,0,1) - xrc(j,0,0)*xrc(j,2,1) )+
			  fabs( xrc(j,0,0)*xrc(j,1,1) - xrc(j,1,0)*xrc(j,0,1) ) );
	      xJac[j]/=xScale;
              xrNorm[j]= (fabs(xrc(j,0,0))+fabs(xrc(j,1,0))+fabs(xrc(j,2,0)))/xScale;
              xsNorm[j]= (fabs(xrc(j,0,1))+fabs(xrc(j,1,1))+fabs(xrc(j,2,1)))/xScale;
	      

              if( xDiff[j]>eps )
	      {
		xok=false;
                if( Mapping::debug & 2 )
		  printf("==> Error projecting physical curve (i=%i): x=(%9.3e,%9.3e,%9.3e) --> rc=(%9.3e,%9.3e) -> "
			 " xc=(%9.3e,%9.3e,%9.3e)\n"
			 "   --> xDiff=%9.3e > eps=%8.2e, |x.r X x.s |/xScale=%8.2e \n",
			 i,x(np[j],0),x(np[j],1),x(np[j],2),rc(j,0),rc(j,1),
			 xc(j,0),xc(j,1),xc(j,2),xDiff[j],eps,xJac[j]);
	      }
              if( xJac[j]<eps )
	      {
                xrok=false;
		printf("==> WARNNING projecting physical curve (i=%i): surface looks singular\n"
                       "   -->  |x.r X x.s |/xScale=%8.2e < eps (xDiff=%9.3e) \n"
                       " xr=(%9.3e,%9.3e,%9.3e), xs=(%9.3e,%9.3e,%9.3e), xScale=%8.2e,\n"
                       " x=(%9.3e,%9.3e,%9.3e) --> rc=(%9.3e,%9.3e) ->  xc=(%9.3e,%9.3e,%9.3e)\n"
                       " |xr|/xScale=%8.2e |xs|/xScale=%8.2e \n",
                       i,xJac[j],xDiff[j],
                       xrc(j,0,0),xrc(j,1,0),xrc(j,2,0), xrc(j,0,1),xrc(j,1,1),xrc(j,2,1),xScale,
                       x(np[j],0),x(np[j],1),x(np[j],2),rc(j,0),rc(j,1),
                       xc(j,0),xc(j,1),xc(j,2),xrNorm[j],xsNorm[j]);


                int ia=np[j];  // end point index
		int ib = ia==0 ? ia+1 : ia-1;  // ib = neighbouring point index
		
		if( xrNorm[j]<eps )
		{
                  // We are at s=0 or s=1 -- the r value is undefined
		  rp(ia,0)= rp(ib,0);
		  rp(ia,1)= rp(ib,1)<.5 ? 0. : 1.;
		}
                else if( xsNorm[j]<eps )
		{
                  // We are at r=0 or r=1 -- the s value is undefined
		  rp(ia,1)= rp(ib,1);
		  rp(ia,0)= rp(ib,0)<.5 ? 0. : 1.;

		}

	      }
	    } // end for j=0,1
	    
            
            // if singular at j==0 or (j==1 && i==nsc) 
            //    -- check the end point of curve "i-1" or "i+1" to see if we need to
            // add a new segment to join with the current

            


	  }  // end if true -- double check results --
	  
	    
	    
	  if( Mapping::debug & 4) 
	  {
	    ::display(rp,sPrintF("\n>>> physical space subCurve=%i projected onto surface (rp)",i),"%4.2f "); 

	  }
	  

          if( surf->getIsPeriodic(axis1)==Mapping::functionPeriodic || 
              surf->getIsPeriodic(axis2)==Mapping::functionPeriodic )
	  {
            // periodic fixup: the inverted points near a branch cut may be on the wrong side of the
            // branch cut. We need to correct these point. To do this we look for neighbouring points
            // that have moved away from the branch cut -- then we know whether we are near 0 or 1.
            // In general we need to check nearby sub-curves since a a sub-curve may lie entirely along
            // the branch cut.

	    if( Mapping::debug & 4) 
              ::display(rp,sPrintF("subCurve %i projected onto surface, BEFORE periodic fix",i),"%3.1f "); 

            const real ptol=1.e-3;  // tolerance for how close a point is to a branch cut (in parameter space)
	    
	    const int base=rp.getBase(0), bound=rp.getBound(0);
	    for( int dir=0; dir<=1; dir++ )
	    {
	      if( surf->getIsPeriodic(dir)==Mapping::functionPeriodic )
	      {
		if( fabs(rp(base,dir))<ptol || fabs(rp(base,dir)-1.)<ptol )
		{
		  // if the first point is near the branch cut we need to decide which side it should really be on
		  real rCut =  fabs(rp(base,dir))<ptol ? 0. : 1.;
		  bool found=false;
		  // look at the next points and find one that moves off the branch cut -- this will
		  // tell us which side we are on
		  for( int ii=base+1; ii<=bound; ii++ )
		  {
		    if( fabs(rp(ii,dir)-rCut)>ptol )
		    {
		      // this point is not too near the branch cut -- use this side for the first point
		      if( fabs(rp(ii,dir))<.5  )  // inside point is nearer to zero
		      {
			if( rCut>.5 )
			  rp(base,dir)-=1.;
		      }
		      else  // inside point is nearer to 1
		      {
			if( rCut<.5 )
			  rp(base,dir)+=1.;
		      }
		      found=true;
		      break;
		    }
		  }

		  if( !found )
		  {
                    // look for a nearby sub-curve
                    real xScale=0.;
		    for( int m=0; m<3; m++ )
		      xScale=max(xScale,surf->getRangeBound(End,m)-surf->getRangeBound(Start,m));

                    int sc=-1;
                    for( int jj=1; jj<nsc; jj++ )
		    {
		      int jc = (i+jj) % nsc;
                      assert( jc!=i );
		      
		      const realArray & xx = tmpCurves[jc]->getGrid();

                      real xDist = (fabs(xx(0,0)-x(bound,0))+
				    fabs(xx(0,1)-x(bound,1))+
				    fabs(xx(0,2)-x(bound,2)));
		      
                      if( xDist < xScale*ptol )
		      {
			sc=jc;
                        if( Mapping::debug & 4) 
                          printf(" subCurve %i: distance to start of subCurve %i =%9.3e (rCut=%4.2f)\n",
                                 i,sc,xDist,rCut);
			break;
		      }
		    }
		    if( sc==-1 )
		    {
                       printf(" createCurveOnAParametricSurface:ERROR: Unable to find a neighbouring subCurve\n"
                              " for determining the periodic fix!\n");
		    }
		    else
		    {
                      // This is wasteful -- should probably pre-compute all the projections onto the surface
                      // of all the sub-curves ---
		      realArray xx; xx = tmpCurves[sc]->getGrid();
		      Range I = xx.getLength(0); 
		      assert( I.getBase()==0 );
		      xx.reshape(I,3);
		      realArray rr(I,2); 
                      rr=-1;
		      surf->inverseMap(xx,rr); 

		      for( int ii=rr.getBase(0); ii<=rr.getBound(0); ii++ )
		      {
			if( fabs(rr(ii,dir)-rCut)>ptol )
			{
                          printf(" point on nearby subCurve: rr(%i,%i)=%8.2e\n",ii,dir,rr(ii,dir));
			  
			  // this point is not too near the branch cut -- use this side for the first point
			  if( fabs(rr(ii,dir))<.5 )  // inside point is nearer to zero
			  {
			    if( rCut>.5 )
			      rp(base,dir)-=1.;
			  }
			  else  // inside point is nearer to 1
			  {
			    if( rCut<.5 )
			      rp(base,dir)+=1.;
			  }
			  found=true;
			  break;
			}
		      }
		    }
		  }
		  if( !found )
		  {
		    printf("createCurveOnAParametricSurface:ERROR:unable to determine which side of a branch cut"
			   " the first point on a\n"
			   "trimming curve should lie!\n");
		  }
	  
		}

                // Now adjust the points for periodicity -- the first point is correct and we adjust all
                // subsequent points to match
		for( int ii=base+1; ii<=bound; ii++ )
		{
		  real rdist=rp(ii,dir)-rp(ii-1,dir);
		  if( fabs(rdist)>.5  )              // consecutive pts are far apart
		  {
		    if( rdist<0. )
		      rp(ii,dir)+=1.;   // shift to make them closer.
		    else
		      rp(ii,dir)-=1.; 
		  }
		}

	      }
	    }
	    if( Mapping::debug & 4) 
              ::display(rp,sPrintF("subCurve %i projected onto surface, after periodic fix",i),"%3.1f "); 

	  }  // end if periodic surface
	  

	  if ( i==1 )
	  {
	    nurb_p = new NurbsMapping(1,2); nurb_p->incrementReferenceCount();
	    nurb_p->interpolate(rp);
	    //	      subCurve[0] = nurb_p;
	    //	      subCurve[0]->incrementReferenceCount();
	    //	      subCurve[1] = new NurbsMapping;subCurve[1]->incrementReferenceCount();
	    //	      *((NurbsMapping*)subCurve[1])=*nurb_p;
	  }
	  else
	  {
	    NurbsMapping & nurb = * new NurbsMapping(1,2); nurb.incrementReferenceCount();
	    nurb.interpolate(rp);

            // if nurb does not match to the end of nurb_p --- check to see if we are on a singular side
            // and need to add a new line segement  -- also check that nurb closes for i==nsc 
            
            // if does not match nurb_p(1) -> nurb(0) and singular[1] then
            //    add a line segment to join 
            // if i==snsc and does not match nurb(1) -> nurb_p(0) then 
            //    add a line segment to join

            const realArray & rNurb = nurb_p->getGrid();
	    if( Mapping::debug & 8 ) 
	    {
	      ::display(rNurb,sPrintF("\n>>> rNurb : getGrid for current merged curve"),"%4.2f "); 

	    }

            int np1=rNurb.getBound(0);
	    real rDist = fabs(rNurb(np1,0,0,0)-rp(0,0))+fabs(rNurb(np1,0,0,1)-rp(0,1));

//             int np2=rp.getBound(0);
// 	    real rDist2 = fabs(rNurb(0,0,0,0)-rp(np2,0))+fabs(rNurb(0,0,0,1)-rp(np2,1));
//             real rDist = min(rDist1,rDist2);
	    

            if( Mapping::debug & 4 )
              printf("**** merging new sub-curve i=%i : distance to existing curve=%8.2e\n",i,rDist);

            const real rEps=1.e-4;
            if( rDist>rEps )
	    {
              realArray rm(2,2);
	      rm(0,0)=rNurb(np1,0,0,0);  rm(0,1)=rNurb(np1,0,0,1);
	      rm(1,0)=rp(0,0);           rm(1,1)=rp(0,1);
	      
	      NurbsMapping & nurbConnector = * new NurbsMapping(1,2); nurbConnector.incrementReferenceCount();
	      nurbConnector.interpolate(rm);     
	      
	      printf("**** Adding a straight-line connector Nurbs before sub-curve %i****\n",i);
	      
              nurb_p->merge(nurbConnector);
	    }
	    
	    nurb_p->merge(nurb);

	    // check that the nurb_p closes when the last sub-curve is added
            if( i==nsc && nurb_p->getIsPeriodic(axis1)==Mapping::notPeriodic ) 
	    {
              const realArray & rNurb = nurb_p->getGrid();
	      const int np1=rNurb.getBound(0);
	      
              realArray rm(2,2);
	      rm(0,0)=rNurb(np1,0,0,0);  rm(0,1)=rNurb(np1,0,0,1);
	      rm(1,0)=rNurb(  0,0,0,0);  rm(1,1)=rNurb(  0,0,0,1);
	      
	      NurbsMapping & nurbConnector = * new NurbsMapping(1,2); nurbConnector.incrementReferenceCount();
	      nurbConnector.interpolate(rm);     
	      
	      printf("**** Adding a straight-line connector Nurbs to close curve at end****\n");
	      
              nurb_p->merge(nurbConnector);
	    }

	    //subCurve[i] = &nurb;
	  }
	}

      for ( int i=1; i<nsc; i++ )
	if ( tmpCurves[i]->decrementReferenceCount()==0 )delete tmpCurves[i];
      
      delete [] tmpCurves;
      mapPointer = nurb_p;//subCurve[0];
      mapPointer->setDomainSpace(Mapping::parameterSpace);
      mapPointer->setRangeSpace(Mapping::parameterSpace);
      NurbsMapping & nurbSurf = (NurbsMapping&)(*surf);
      //      nurb_p->parametricCurve(nurbSurf);
      //      scaleCurve( *mapPointer,*surf,surfaceParameterScale);

      return status;
    }

  int curve = iges.sequenceToItem(sequence);
  if( Mapping::debug & 2 )
  {
    cout << "curveOnAParametricSurface, seq = " << sequence
	 << " is a " << iges.entityName(iges.entity(curve)) << endl;
  }

  int status = 0;
  if( iges.entity(curve)==IgesReader::compositeCurve )
  {
    int numberOfSubCurves=0; // *wdh* 010726  20;
    status = createCompositeCurve(curve,iges,maximumNumberOfSubCurves,numberOfSubCurves,subCurve,surf,
             surfaceParameterScale);

    mapPointer=subCurve[0];    // this is really the merged curve
    mapPointer->setDomainSpace(Mapping::parameterSpace);
    mapPointer->setRangeSpace(Mapping::parameterSpace);
    

    if(  Mapping::debug & 2 )
    {
      gi.erase();
      params.set(GI_TOP_LABEL,"scaled parametric trimming curve");
      PlotIt::plot(gi,*mapPointer,params);
      gi.redraw(TRUE);
      params.set(GI_TOP_LABEL," ");
    }
    
  }
  else if( iges.entity(curve)==IgesReader::copiusData )
  {
    sequence = (int)data(3);    // data(3) = parametric curve, data(4)=curve itself
    curve = iges.sequenceToItem(sequence);

    int form=iges.formData(curve);
  
    iges.readData(curve,data,6);
   
    int interpretationFlag = (int)data(1); 
    int n = (int)data(2);
//    printf(" form=%i, curve=%i, interpretationFlag=%i, n=%i\n",form,curve,interpretationFlag,n);

    if( form==11 )
    {
      // linear path entity -- piecewise linear line
      data.redim(4+2*n);
      iges.readData(curve,data,4+2*n);

      // data.display("data");

      real zt=data(3);      // constant z level

      realArray x(n,2), r(n);
      Range R=n, R2(0,2*n-1,2);
      x(R,0)=data(4+R2);
      x(R,1)=data(5+R2);
      
      // use 'index' parameterization instead of the default arc-length:
      r.seqAdd(0.,1./max(1.,(n-1.)));
      // x(R,0).display("x");
      // x(R,1).display("y");
      
      // Build a nurb curve of degree 1
      NurbsMapping & nurb = * new NurbsMapping(1,2); nurb.incrementReferenceCount();
      int degree=1;
      // nurb.interpolate(x,0,Overture::nullRealDistributedArray(),degree);
      nurb.interpolate(x,0,r,degree);
      // nurb.update(mapInfo);

      // PlotIt::plot(gi,nurb);
      
      NurbsMapping & nurbSurf = (NurbsMapping&)(*surf);
      nurb.parametricCurve(nurbSurf); // this will scale curve and make 2D

      // gi.erase();
      // PlotIt::plot(gi,nurb);

      mapPointer=&nurb;
      mapPointer->setDomainSpace(Mapping::parameterSpace);
      mapPointer->setRangeSpace(Mapping::parameterSpace);

      // if( fabs(x(n,0)-x(0,0))+fabs(x(n,1)-x(0,1)) < 
      mapPointer->setIsPeriodic(axis1,Mapping::functionPeriodic);
      
    }
    else
    {
      printf("ERROR: unimplemented copiusData entry with form=%i\n",form);
      
      {throw "error";}
    }
    
  }
  else if( iges.entity(curve)==IgesReader::rationalBSplineCurve ||  // 126
	   iges.entity(curve)==IgesReader::parametricSplineCurve ||  // 112
	   iges.entity(curve)==IgesReader::circularArc ) // 100
  { 
    real time1=getCPU();
    mapPointer = new NurbsMapping();  mapPointer->incrementReferenceCount();

    ((NurbsMapping*)mapPointer)->readFromIgesFile(iges,curve);
    // kkc 051401
    ((NurbsMapping*)mapPointer)->truncateToDomainBounds();

      // scale the parametric curve to [0,1]x[0,1]
    scaleCurve( *mapPointer,*surf,surfaceParameterScale);
    timeToBuildNurbsCurves+=getCPU()-time1;
  }
  else
  {
    printf("\n\n ***** createCurveOnAParametricSurface:ERROR:unknown curve on a parametric surface! \n");
    printf(" **** iges.entity(curve) = %i \n",iges.entity(curve));
    {throw "error";}
    
//      NurbsMapping & nurb = * new NurbsMapping(1,2); nurb.incrementReferenceCount();
//      mapPointer=&nurb;
//      mapPointer->setDomainSpace(Mapping::parameterSpace);
//      mapPointer->setRangeSpace(Mapping::parameterSpace);
//      status=1;
    
  }
  
  timeToCreateCurves+=getCPU()-time0;
  
  return status;

}

int MappingsFromCAD::
createSurface(int surface, 
              IgesReader & iges, 
              Mapping *&mapPointer,
              RealArray & surfaceParameterScale /* = Overture::nullRealArray() */ )
// =========================================================================================
/// \details 
///     Read in a surface and create a Mapping for it.
/// 
/// \param surface (input): try to build iges.entity(surface).
/// \param mapPointer (output): NULL is no Mapping created.
/// \param surfaceParameterScale (input): if supplied on input, return the scaling for the parameter space
///    of the surface, needed to scale trimming curves.
/// 
/// \return  0 is a Mapping was built, 1 if no Mapping built.
// ========================================================================================
{
  real time0=getCPU();
  
  mapPointer=NULL;

  assert( mapInfoPointer!=NULL );
  MappingInformation &mapInfo = *mapInfoPointer;

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  char buff[80];

  RealArray curveParameterScale(2);
  curveParameterScale(0)=0.;      // ra or sa for a curve
  curveParameterScale(1)=1.;      // rb or sb for a curve
  if( surfaceParameterScale.getLength(0)==2 )
  {
    surfaceParameterScale(0,0)=0.;  // ra
    surfaceParameterScale(1,0)=1.;  // rb
    surfaceParameterScale(0,1)=0.;  // sa
    surfaceParameterScale(1,1)=1.;  // sb
  }
  int status=0;
  if( iges.entity(surface)==IgesReader::trimmedSurface )
  {
    if( Mapping::debug & 2 )
      cout << "createSurface::INFO: trimmed surface found \n";
    if( Mapping::debug & 4 )
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);

    status = createTrimmedSurface(surface,iges,mapPointer);

    if( Mapping::debug & 4 )
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  }
  else if( iges.entity(surface)==IgesReader::boundedSurface )
  {
    if( Mapping::debug & 2 )
      cout << "createSurface::INFO: bounded surface found \n";
    if( Mapping::debug & 4 )
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
    
    status = createBoundedSurface(surface, iges, mapPointer);

    if( Mapping::debug & 4 )
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  }
  else if( iges.entity(surface)==IgesReader::rationalBSplineSurface ||
	   iges.entity(surface)==IgesReader::parametricSplineSurface )
  {
    real time1=getCPU();
    
    mapPointer = new NurbsMapping;
    mapPointer->incrementReferenceCount();
    ((NurbsMapping *) mapPointer)->readFromIgesFile(iges,surface);
    ((NurbsMapping *) mapPointer)->setDomainInterval(0.,1.,0.,1.);
    mapPointer->setName(Mapping::mappingName,sPrintF(buff,"nurbs %i",surface));
   
    timeToBuildNurbsSurfaces+=getCPU()-time1;
    
    if(  Mapping::debug & 4 )
    {
      gi.erase();
      PlotIt::plot(gi,*mapPointer);
      gi.redraw(TRUE);
    }

  }
  else if( iges.entity(surface)==IgesReader::tabulatedCylinder )
  {
    realArray inputData(5);
    iges.readData(surface,inputData,5);
    
    realArray straightLine(3);
    straightLine = inputData(Range(2,4));

    int curve = iges.sequenceToItem((int)inputData(1)); // inputData(1) = sequence number for the curve
    if( curve<0 )
    {
      printf("ERROR reading curve number for a tabulatedCylinder: curve=%i inputData=%e %e %e (%i %i %i)\n",
	     curve,inputData(0),inputData(1),inputData(2),(int)inputData(0),(int)inputData(1),(int)inputData(2));
      Overture::abort("error");
    }
    
    if( Mapping::debug & 2 )
      cout << "createSurface::INFO: The underlying surface is a tabulated cylinder." << endl
  	 << "The sequence # of the defining curve is " << (int) inputData(1) 
          << ", which is a " << iges.entityName(iges.entity(curve)) << endl;

    RealArray matrixTransform(3,3),translation(3);
    int matrix=iges.matrix(surface);
    if( matrix!=0 )
    {
      matrix=iges.sequenceToItem(matrix);
      int returnValue=getTransformationMatrix(matrix,iges,matrixTransform,translation);
      if( (Mapping::debug & 1) && returnValue==0  )
      {
	display(matrixTransform,"tabulatedCylinder: matrixTransform","%8.2e ");
	display(translation,"tabulatedCylinder: translation","%8.2e ");
      }
    }
    
    Mapping *tabCylCurve=NULL;
    readOneCurve(curve, iges, tabCylCurve,curveParameterScale); // increments the reference count of tabCylCurve
    
    if( tabCylCurve ) // create the tabulated cylinder mapping
    {
      if( tabCylCurve->getClassName()=="NurbsMapping" )
      {
        if( Mapping::debug & 2 )
          printf("**** createSurface: build a tabulated cylinder using the NurbsMapping general cylinder\n");

	NurbsMapping & nurbsCurve = (NurbsMapping&)(*tabCylCurve);
	mapPointer = new NurbsMapping; mapPointer->incrementReferenceCount();
        NurbsMapping & cylinder = (NurbsMapping&)(*mapPointer);
        realArray r(1,3),x(1,3);
        r=0.;
	nurbsCurve.map(r,x);

        // real d[3]={straightLine(0)-x(0,0), straightLine(1)-x(0,1), straightLine(2)-x(0,2)}; //
        // This is wrong -- fix this 
        real d[3]={straightLine(0)-x(0,0), straightLine(1)-x(0,1), straightLine(2)-x(0,2)}; //
	cylinder.generalCylinder(nurbsCurve,d);
	
	if( matrix!=0 )
	{
	  // apply the matrix and translation operators
	  cylinder.matrixTransform(matrixTransform);
	  cylinder.shift(translation(0),translation(1),translation(2));
	}

      }
      else
      {
	// old way
	if( matrix!=0 )
	{
          // We should probably just convert the tabCylCurve into a NurbsMapping and use the above code !
	  printf("\n ***NurbsMapping::readFromIgesFile:WARNING: matrixTransform!=0 for a tabulatedCylinder *****\n\n");
	}
    

	mapPointer = new SweepMapping(NULL, tabCylCurve, NULL, 3); // increments the reference count of tabCylCurve
	mapPointer->incrementReferenceCount();
	((SweepMapping *) mapPointer)->setStraightLine(straightLine(0), straightLine(1), straightLine(2));
      }
      if (tabCylCurve->decrementReferenceCount() == 0)
	delete tabCylCurve;
    }
    else 
    {
      cout << "createSurface::ERROR: Tabulated cylinder can not be created since the curve is undefined" << endl;
      mapPointer=NULL; 
      return 1;
    }

  }
  else if( iges.entity(surface)==IgesReader::surfaceOfRevolution )
  {
    realArray inputData(5);
    RealArray lineOrigin(3), lineTangent(3);
    aString revName;
    iges.readData(surface,inputData,5);

    int matrix=iges.matrix(surface);
    RealArray matrixTransform(3,3),translation(3);
    if( matrix!=0 )
    {
      printf(" ***surfaceOfRevolution: matrix != 0 ****\n");

      matrix=iges.sequenceToItem(matrix);
      int returnValue=getTransformationMatrix(matrix,iges,matrixTransform,translation);
      if( (Mapping::debug & 1) && returnValue==0  )
      {
	display(matrixTransform,"*** surfaceOfRevolution: matrixTransform","%8.2e ");
	display(translation,"**** surfaceOfRevolution: translation","%8.2e ");
      }
    }

    int line = iges.sequenceToItem((int)inputData(1)); // inputData(1) = sequence number for the axis of rev.
    int curve = iges.sequenceToItem((int)inputData(2)); // inputData(2) = sequence number for the curve to be rev.

    if( Mapping::debug & 1 )
      printf("createSurface::INFO: surfaceOfRevolution found\n");
    if( Mapping::debug & 2 )
    {
      cout << "createSurface::INFO: The underlying surface is a surface of revolution." << endl
           << "The axis of revolution is a " << iges.entityName(iges.entity(line)) << endl
           << "and the curve to be revolved is a " << iges.entityName(iges.entity(curve)) << endl
           << "Starting and ending angles are (radians): " << inputData(3) << " " << inputData(4) << endl;
    }
    
    // From the IGES manual:
    // The default parameterization for a surface of revolution is
    //     xv(x,theta) :  with x in [a,b]   and theta in [TA,TB]
    //   [a,b] : domain bounds on the generatrix curve (revCurve)

    realArray r(2,3), x(2,3);
    Mapping * axisLine=NULL, *revCurve=NULL;
    readOneCurve(line, iges, axisLine,curveParameterScale);  // increments reference count for axisLine
    readOneCurve(curve, iges, revCurve,curveParameterScale); // increments reference count for revCurve

    // curveParameterScale.display("surfaceOfRevolution: curveParameterScale");

    if( surfaceParameterScale.getLength(0)==2 )
    {
      surfaceParameterScale(0,0)=curveParameterScale(0);
      surfaceParameterScale(1,0)=curveParameterScale(1);

      if( Mapping::debug & 2 )
        printf("****surfaceOfRevolution: parameter scale= [%e,%e]\n",
             surfaceParameterScale(0,0),surfaceParameterScale(1,0));
      
//        printf("****surfaceOfRevolution: Changing parameter scale from [%e,%e] to [%e,%e]\n",
//  	     surfaceParameterScale(0,0),surfaceParameterScale(1,0),0.,twoPi);
      
//         surfaceParameterScale(0,0)=0.;
//         surfaceParameterScale(1,0)=1.; // twoPi;
      

    }

    axisLine->setName(Mapping::mappingName,sPrintF(buff,"axisline-%i",++axisLineCount));
    revCurve->setName(Mapping::mappingName,sPrintF(buff,"revcurve-%i",++revCurveCount));

    // Evaluate the starting and ending points of the line
    if (axisLine)
    {
      r = 0;
      r(1,0) = 1;
      axisLine->map(r,x);
      for (int ii=0; ii<3; ii++)
      {
	lineOrigin(ii)  = x(0,ii);
	lineTangent(ii) = x(1,ii)-x(0,ii);
      }
      if( Mapping::debug & 1 ) 
        printf("surfaceOfRevolution: line: lineOrigin=(%9.2e,%9.2e,%9.2e) lineTangent=(%9.2e,%9.2e,%9.2e)\n",
	       lineOrigin(0),lineOrigin(1),lineOrigin(2),lineTangent(0),lineTangent(1),lineTangent(2));
      
    }
    else
    {
      cout << "createSurface::ERROR, the axis could not be found in the IGES file" << endl;
      mapPointer=NULL;
      return 1;
    }
    // won't need the axisLine any more
    axisLine->decrementReferenceCount(); // decrements reference count for axisLine
    delete axisLine; axisLine=NULL;
    
    if (revCurve) // create the surface of revolution mapping
    {
      r = 0;
      r(1,0) = 1;
      revCurve->map(r,x);  // wdh: what is this for?
    
      mapPointer = new RevolutionMapping(*revCurve, inputData(3)/twoPi, inputData(4)/twoPi,  
				    lineOrigin, lineTangent); // increments reference count for revCurve
      mapPointer->incrementReferenceCount();
      mapPointer->setName(Mapping::mappingName,sPrintF(buff,"revolution-%i",++revCount));
      if (revCurve->decrementReferenceCount()==0) // decrement reference count for revCurve
	delete revCurve; 
    }
    else 
    {
      cout << "ERROR: The surface of revolution can not be created since the curve is undefined" << endl;
      mapPointer=NULL; 
      return 1;
    }
  }
  else if ( iges.entity(surface)==IgesReader::plane )
    {
      // kkc added bounded plane entity (unbounded (form==0) is not yet supported; but should be if subordinate to a trimmed surface?)
      realArray inputData(9);
      iges.readData(surface,inputData,9);

      int form = iges.formData(surface);

      real A,B,C,D;
      A = inputData(0);
      B = inputData(1);
      C = inputData(2);
      D = inputData(3);

      cout<<"plane:  form "<<form<<"  A,B,C,D "<<A<<"  "<<B<<"  "<<C<<"  "<<D<<"  "<<endl;
      cout<<"plane:  curve "<<int(inputData(4))<<endl;
    
      status = 123;
    }
  else
    {
      cout<<"UNKNOWN SURFACE : "<<iges.entity(surface)<<endl;
    }

  int matrix=iges.matrix(surface);
  if( matrix!=0 )
    cout<<"WARNING : there was an ignored matrix transform!"<<endl;

  timeForCreateSurface+=getCPU()-time0;
  
  return status;

}




int MappingsFromCAD::
createTrimmedSurface(const int & item, 
		     IgesReader & iges, 
                     Mapping *&mapPointer)
// =======================================================================================================
/// \details  Create a trimmed surface, reading it from an IGES file.
/// 
///  The data for the trimmed surface in the IGES file is
///  \begin{verbatim}
///    data[0] : entity number
///    data[1] : Pointer to DE of surface to be trimmed 
///    data[2] : 0 = Outer boundary is D, 1= otherwise 
///    data[3] : Number of simple closed curves of inner boundary 
///    data[4] : Pointer to DE of the first simple closed curve 
///    data[...] : Pointer to DE of the ... simple closed curve 
///  \end{verbatim}
/// 
/// \param mapPointer : Points to the "trimmed" mapping. This will usually be a TrimmedMapping unless the
///     trimmed region can be replaced by a reparameterized NURBS (if the trimmed region is a rectangle)
///     or by a TFIMapping (with 2 or 4 sides specified) if the trimmed region is a single outer curve with 4 sides.
///      
// =======================================================================================================
{
  assert( mapInfoPointer!=NULL );
  MappingInformation &mapInfo = *mapInfoPointer;

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;


  realArray data(4);
  iges.readData(item,data,4);

  int  outerBoundary = (int)data(2);
  int  numberOfInnerCurves = (int)data(3);

  if( Mapping::debug & 2 )
  {
    cout << " outerBoundary= " << outerBoundary << ", 0 = Outer boundary is D, 1= otherwise" << endl;
    cout << " numberOfInnerCurves= " << numberOfInnerCurves << ", Number of simple closed curves of inner boundary\n";
  }
  
  if (outerBoundary+numberOfInnerCurves != 0)
  {
    data.redim(outerBoundary+numberOfInnerCurves+4);
    iges.readData(item,data,outerBoundary+numberOfInnerCurves+4);
  }
  else
  {
    cout << "there are no trimming curves in trimmed surface "<<item<<endl;
  }

  int surface=iges.sequenceToItem((int)data(1)); // data(1) = sequence number for the surface to be trimmed
  if( surface==-1 )
  {
    cout << "ERROR: surface to be trimmed not found, sequence = " << (int)data(1) << endl;
    mapPointer=NULL;
    return 1;
  }
  
  // By default the parameterization of the IGES surface is on [0,1]x[0,1]
  // In some cases the IGES surface is parameterized as [ra,rb]x[sa,sb], these values are
  // saved in the  surfaceParameterScale array: 
  RealArray surfaceParameterScale(2,2),curveParameterScale(2);
  surfaceParameterScale(0,0)=0.;  // ra
  surfaceParameterScale(1,0)=1.;  // rb
  surfaceParameterScale(0,1)=0.;  // sa
  surfaceParameterScale(1,1)=1.;  // sb
  curveParameterScale(0)=0.;      // ra or sa for a curve
  curveParameterScale(1)=1.;      // rb or sb for a curve
  

  Mapping *nurbs=NULL; // Holds the pointer to the untrimmed mapping

  // *****************************************
  // ***** create the un-trimmed surface *****
  // *****************************************

  createSurface(surface,iges,nurbs, surfaceParameterScale);
  
  if( nurbs==NULL )
  {
    cout << "createTrimmedSurface::ERROR: unable to build the surface to be trimmed: " 
         << iges.entityName(iges.entity(surface)) << endl;
    mapPointer=NULL;
    return 1;
  }

  if( nurbs->getClassName()=="NurbsMapping" )
  {
    // add more points for trimmed patches with both an inner and outer boundary
    if( outerBoundary+numberOfInnerCurves >= 2 )
    { 
      int num=min(61,(outerBoundary+numberOfInnerCurves)*21);
      nurbs->setGridDimensions(axis1,max(num,nurbs->getGridDimensions(axis1)));
      nurbs->setGridDimensions(axis2,max(num,nurbs->getGridDimensions(axis2)));
    }
  }

  // params.set(GI_TOP_LABEL,"un-trimmed surface");
  // PlotIt::plot(gi,*nurbs,params);

  int sequence, boundaryCurve;
  Mapping **curvePointer = numberOfInnerCurves==0 ? NULL : new Mapping* [numberOfInnerCurves];  
  int i;
  for(i=0; i<numberOfInnerCurves; i++)
    curvePointer[i]=NULL;
  
  int maximumNumberOfSubCurves=0;//20;  // initial guess at maximum number of sub curves
  Mapping **subCurve = NULL;//new Mapping * [maximumNumberOfSubCurves];
  //for( i=0; i<maximumNumberOfSubCurves; i++)
  //  subCurve[i]=NULL;
  
  int curveStatus = 0;
  // inner boundary curves
  for(i=0; i<numberOfInnerCurves; i++)
  {
    sequence= (int)data(5+i);
    boundaryCurve = iges.sequenceToItem(sequence);
    if( Mapping::debug & 2 )
      cout << "inner boundary Curve " << i << ", seq = " << sequence
	   << " is a " << iges.entityName(iges.entity(boundaryCurve)) << endl;

    curvePointer[i]=NULL;
    if( iges.entity(boundaryCurve)==IgesReader::curveOnAParametricSurface ) // 142
    {
      curveStatus = max(curveStatus,createCurveOnAParametricSurface(boundaryCurve,iges,nurbs,curvePointer[i],
                          maximumNumberOfSubCurves,subCurve,surfaceParameterScale));
    }
    else
    {
      printf("createTrimmedSurface:ERROR: inner boundary curve is NOT a curveOnAParametricSurface! \n");
      mapPointer=NULL;
      return 1;
    }

    for(int ii=1; ii<maximumNumberOfSubCurves; ii++)  // dont delete subCurve[0] !!
    {
      if( subCurve[ii]!=NULL )
      {
        subCurve[ii]->decrementReferenceCount();
        delete subCurve[ii]; subCurve[ii]=NULL;
      }
    }

    if ( curvePointer[i]!=0 )
      if  (curvePointer[i]->getIsPeriodic(0) != Mapping::functionPeriodic ) curveStatus = 1;
  }


  // outer boundary curve if outerBoundary=1, otherwise boundary of the trimee is the outer boundary
  Mapping *outerBoundaryCurve=NULL;

  if (outerBoundary == 1)
  {
    sequence= (int)data(4);
    boundaryCurve = iges.sequenceToItem(sequence);
    if( Mapping::debug & 2 )
    {
      cout << "outer boundary Curve " << i << ", seq = " << sequence 
	   << " is a " << iges.entityName(iges.entity(boundaryCurve)) << endl;
    }
    if( iges.entity(boundaryCurve)==IgesReader::curveOnAParametricSurface ) // 142
      curveStatus = max(curveStatus, createCurveOnAParametricSurface(boundaryCurve,iges,nurbs,outerBoundaryCurve,
                        maximumNumberOfSubCurves,subCurve,surfaceParameterScale));

    if ( curveStatus==0 )
      if ( outerBoundaryCurve->getIsPeriodic(0)!=Mapping::functionPeriodic ) curveStatus = 1;
  }

  
  mapPointer=NULL;
  bool surfaceIsANurbs=nurbs->getClassName()=="NurbsMapping";
// Try to turn the trimmed mapping into a compose mapping instead?
// AP turned this off since there is no logic to prevent the ComposeMappings from beeing
// singular, i.e., there is no test to make sure the sub-curves meet at an angle smaller
// than 180 degrees.
  if( false &&   // *wdh* turn this off? 010228
      outerBoundary==1 && numberOfInnerCurves==0 && surfaceIsANurbs && curveStatus==0 )
  {
    // see if we can make a non-trimmed mapping
    real time0=getCPU();

    // createSimpleTrimmedSurface();

    if( boundaryType==simpleReparameterization )
    {
      if( Mapping::debug & 2 )
        cout << "\n\n +++++++++++++ trimmed patch is a simple reparameterization ++++++ \n\n";
      ((NurbsMapping *) nurbs)->reparameterize(curveBound[Start][axis1],curveBound[End][axis1],
                            curveBound[Start][axis2],curveBound[End][axis2]); // scale uKnot and vKnot
      mapPointer=nurbs;
      outerBoundaryCurve->decrementReferenceCount();
      delete outerBoundaryCurve;  outerBoundaryCurve=NULL;
      
      for(i=1; i<maximumNumberOfSubCurves; i++)  // don't delete subCurve[0] = outerBoundaryCurve
      {
        if( subCurve[i]!=NULL )
	{
          subCurve[i]->decrementReferenceCount();
   	  delete subCurve[i];
   	  subCurve[i]=NULL;
	}
      }

    }
    else if( boundaryType==coons13LeftRight ||  boundaryType==coons13BottomTop )
    {
      if( Mapping::debug & 2 )
        cout << "\n\n +++++++++++++ trimmed patch is a coons13 reparameterization ++++++ \n\n";
      ((NurbsMapping*)subCurve[3])->reparameterize(1.,0.);   // reverse parameterization
    
      TFIMapping *coonsPatch;
      if( boundaryType==coons13LeftRight )
         coonsPatch= new TFIMapping(subCurve[1],subCurve[3]);
      else
         coonsPatch= new TFIMapping(NULL,NULL,subCurve[1],subCurve[3]);
      coonsPatch->setRangeSpace(Mapping::parameterSpace);
      coonsPatch->incrementReferenceCount();
      ComposeMapping *compose = new ComposeMapping(*coonsPatch,*nurbs);
      for( int axis=0; axis<2; axis++ )
        compose->setGridDimensions(axis,max(coonsPatch->getGridDimensions(axis),
                                            nurbs->getGridDimensions(axis),5));
      
      coonsPatch->decrementReferenceCount();
      nurbs->decrementReferenceCount();
      mapPointer=compose;
      mapPointer->incrementReferenceCount();

      outerBoundaryCurve->decrementReferenceCount();
      delete outerBoundaryCurve;  outerBoundaryCurve=NULL;
      for(i=1; i<maximumNumberOfSubCurves; i++)  // don't delete subCurve[0] = outerBoundaryCurve
      {
        if( subCurve[i]!=NULL )
	{
	  subCurve[i]->decrementReferenceCount();
	  if( i!=1 && i!=3 )
	    delete subCurve[i];
	  subCurve[i]=NULL;
	}
      }
    }
    else if( boundaryType==coons24LeftRight ||  boundaryType==coons24BottomTop )
    {
      if( Mapping::debug & 2 )
        cout << "\n\n +++++++++++++ trimmed patch is a coons24 reparameterization ++++++ \n\n";
      ((NurbsMapping*)subCurve[4])->reparameterize(1.,0.);   // reverse parameterization
      TFIMapping *coonsPatch;
      if( boundaryType==coons24LeftRight )
        coonsPatch= new TFIMapping(subCurve[4],subCurve[2]);
      else
        coonsPatch= new TFIMapping(NULL,NULL,subCurve[4],subCurve[2]);
      coonsPatch->setRangeSpace(Mapping::parameterSpace);
      coonsPatch->incrementReferenceCount();
      ComposeMapping *compose = new ComposeMapping(*coonsPatch,*nurbs);
      for( int axis=0; axis<2; axis++ )
        compose->setGridDimensions(axis,max(coonsPatch->getGridDimensions(axis),
                                            nurbs->getGridDimensions(axis),5));
      coonsPatch->decrementReferenceCount();
      nurbs->decrementReferenceCount();
      mapPointer=compose;
      mapPointer->incrementReferenceCount();

      outerBoundaryCurve->decrementReferenceCount();
      delete outerBoundaryCurve;  outerBoundaryCurve=NULL;
      for(i=1; i<maximumNumberOfSubCurves; i++)  // don't delete subCurve[0] = outerBoundaryCurve
      {
        if( subCurve[i]!=NULL )
	{
          subCurve[i]->decrementReferenceCount();
	  if( i!=2 && i!=4 )
	    delete subCurve[i];
	  subCurve[i]=NULL;
	}
      }
    }
    else if( boundaryType==coons )
    {
      if( Mapping::debug & 2 )
        cout << "\n\n +++++++++++++ trimmed patch is a coons reparameterization ++++++ \n\n";
      ((NurbsMapping*)subCurve[2])->reparameterize(1.,0.);   // reverse parameterization
      ((NurbsMapping*)subCurve[4])->reparameterize(1.,0.);   // reverse parameterization
      TFIMapping *coonsPatch = new TFIMapping(subCurve[1],subCurve[2],subCurve[3],subCurve[4]);
      coonsPatch->setRangeSpace(Mapping::parameterSpace);
      coonsPatch->incrementReferenceCount();
      ComposeMapping *compose = new ComposeMapping(*coonsPatch,*nurbs);
      for( int axis=0; axis<2; axis++ )
        compose->setGridDimensions(axis,max(coonsPatch->getGridDimensions(axis),
                                            nurbs->getGridDimensions(axis),5));
      coonsPatch->decrementReferenceCount();
      nurbs->decrementReferenceCount();
      mapPointer=compose;
      mapPointer->incrementReferenceCount();

      outerBoundaryCurve->decrementReferenceCount();
      delete outerBoundaryCurve;  outerBoundaryCurve=NULL;
    }
    
    timeToBuildTrimmedMappings+=getCPU()-time0;
    
  } // end if turn trimmed mapping into composemapping
  else if ( outerBoundary==0 && numberOfInnerCurves==0 )
  {
      mapPointer = (Mapping *)nurbs;
      mapPointer->incrementReferenceCount();
  }

  if( mapPointer==NULL )  
  {
    if( Mapping::debug & 2 )
      printf(" ***** Build a TrimmedMapping ****\n");
    
    if( nurbs->getDomainDimension()!=2 )
    {
      printf("MappingsFromCAD::createTrimmedSurface:ERROR: nurbs has domainDimension=%i, was expecting 2!\n",
        nurbs->getDomainDimension());
      throw "error";
    }

    real time0=getCPU();
    mapPointer = new TrimmedMapping(*nurbs,outerBoundaryCurve,numberOfInnerCurves,curvePointer);
    timeToBuildTrimmedMappings+=getCPU()-time0;

    // *kkc* 260902the constructor checks the validity of the trimming itself
    //        if ( curveStatus != 0 ) ((TrimmedMapping *)mapPointer)->invalidateTrimming();
    curveStatus = ((TrimmedMapping *)mapPointer)->trimmingIsValid() ? 0 : 1;

    nurbs->decrementReferenceCount();
    mapPointer->incrementReferenceCount();
    if( outerBoundaryCurve!=NULL )
      outerBoundaryCurve->decrementReferenceCount();

    for( i=0; i<numberOfInnerCurves; i++ )
      curvePointer[i]->decrementReferenceCount();

    delete [] curvePointer;
    // 
    if( outerBoundary==1 )
    {
      for(i=1; i<maximumNumberOfSubCurves; i++)  // don't delete subCurve[0] = composite curve
      {
        if( subCurve[i]!=NULL )
	{
	  subCurve[i]->decrementReferenceCount();
	  delete subCurve[i];
	  subCurve[i]=NULL;
	}
      }
    }
    
  }
  
  if( Mapping::debug & 2 )
  {
    params.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
    gi.erase();
    params.set(GI_TOP_LABEL,"trimmed surface");
    PlotIt::plot(gi,*mapPointer,params);
    gi.redraw(TRUE);
    params.set(GI_TOP_LABEL," ");
  }

//  cout << "createTrimmedSurface: mapPointer = " << mapPointer << endl;
//  params.set(GI_TOP_LABEL,"trimmed surface (2)");
//  PlotIt::plot(gi,*mapPointer,params);

  delete [] subCurve;

  return curveStatus;

}


int MappingsFromCAD::
readFiniteElements(IgesReader & iges )
// =================================================================================================
/// \details 
///    Create an UnstructuredMapping from finite elements in the IGES file.
// ================================================================================================
{
  assert( mapInfoPointer!=NULL );
  MappingInformation &mapInfo = *mapInfoPointer;
  
  const int numberOfEntities=iges.numberOfEntities();
  const int maxNum=numberOfEntities*2;
  
  realArray data(50), nodeData(50);
  realArray x(maxNum,3);
  intArray nodeID(maxNum);
  nodeID=-1;
  
  int i;
  int numberOfFiniteElements=0;
  int numberOfNodes=0;
  
  for( i=0; i<numberOfEntities; i++ )
  {
    if( iges.entity(i)==IgesReader::node )
    {
      int parameterData = iges.parameterData(i);
      int sequenceNumber = iges.sequenceNumber(i);
      
      assert( sequenceNumber>=0 && sequenceNumber<maxNum && numberOfNodes<maxNum );
      nodeID(sequenceNumber)=numberOfNodes;
      // fprintf(file,"node %i, seq=%i par=%i\n",numberOfNodes,entityInfo(1,i),entityInfo(2,i));
      
      iges.readParameterData(parameterData,nodeData,4);
      x(numberOfNodes,0)=nodeData(1);
      x(numberOfNodes,1)=nodeData(2);
      x(numberOfNodes,2)=nodeData(3);
      numberOfNodes++;
    }
    else if( iges.entity(i)==IgesReader::finiteElement )
      numberOfFiniteElements++;
  }
  printf("number of nodes = %i number of elements = %i\n",numberOfNodes,numberOfFiniteElements);
  
  // now build the elements -- 3 int's that denote the id's of the nodes.
  intArray element(numberOfFiniteElements,3);
  int elementCount=0;

  for( i=0; i<numberOfEntities; i++ )
  {
    if( iges.entity(i)==IgesReader::finiteElement )
    {
      int item=i;
      iges.readData(item,data,3);
      int topologyType=(int)data(1);
      int numberOfNodes=(int)data(2);
      assert( numberOfNodes==3 );     // for now we only do triangles !

      // printf("item=%i topologyType=%i numberOfNodes=%i \n",item,topologyType,numberOfNodes);
      assert( 3+numberOfNodes < 50 );
      iges.readData(item,data,3+numberOfNodes);
      assert( elementCount<maxNum );
      
      for( int n=0; n<numberOfNodes; n++ )
      {
        int nodePointer= (int)data(n+3);
        assert( nodePointer>=0 && nodePointer<maxNum && nodeID(nodePointer)>=0 );
        element(elementCount,n)=nodeID(nodePointer);
	

	// iges.readParameterData(nodePointer,nodeData,2);
	// int entityType = (int) nodeData(0);
        // int parameterData = (int) nodeData(1);
        
/* ---
        if( entityType==IgesReader::node )
	{
	  iges.readParameterData(parameterData,nodeData,4);
	  printf(" x=(%e,%e,%e) \n",nodeData(1),nodeData(2),nodeData(3));
	}
	else
	{
	  printf("Unknown entityType\n");
	}
----- */
      }
      elementCount++;
      
    }
  }

  UnstructuredMapping *tri = new UnstructuredMapping;
  tri->incrementReferenceCount();
  mapInfo.mappingList.addElement(*tri);
  tri->decrementReferenceCount();

  Range R(0,numberOfNodes-1), R3(0,2);
  
  tri->setNodesAndConnectivity(x(R,R3),element);

  //  PlotIt::plot(gi,*tri);
  //  gi.plotPoints(x(R,R3));
  
  return 0;
}


int MappingsFromCAD::
createIgesReader( GenericGraphicsInterface & gi, aString & fileName, IgesReader * &iges, FILE * &fp,
		  bool useGivenFileName /* = FALSE */)
// =================================================================================================
/// \details 
///     Open an IGES file and associate it with and IgesReader.
// =================================================================================================
{
  fp=NULL;
  if (!useGivenFileName)
    gi.inputFileName(fileName,"Enter the name of the IGES file");

  fp=fopen((const char*)fileName,"r");

// if you supply a file name, it better open ok...
  if (useGivenFileName && fp == NULL)
    return 1;

  int attempts=0;
  while( fp==NULL && attempts<5 )
  {
    aString name = fileName+".igs";
    fp=fopen((const char*)name,"r");
    if( fp!=NULL )
    {
      fileName=name;
      break;
    }
    
    gi.outputString("createIgesReader:ERROR:File not found.");
    if( gi.readingFromCommandFile() )
    {
      gi.stopReadingCommandFile();     // *wdh* 011111
      return 1;                        // *wdh* 020612 
    }

    gi.inputString(fileName,"File not found, enter another name for the IGES file");
    fp=fopen((const char*)fileName,"r");
    attempts++;
  }
  if( attempts>=5 )
  {
    gi.outputString("Too many tries");
    return 1;
  }


  iges=new IgesReader;
  iges->readIgesFile((const char*)fileName);

  iges->fp = fopen(fileName,"rb");  // *wdh* 020617

  fp = iges->fp;
  //fclose(fp);
// *wdh*  fp = fopen(fileName,"rb");  // *wdh* 020617

  return 0;
}

int MappingsFromCAD::
isUntrimmedSurface(IgesReader & iges, int item )
// =============================================================================
/// \details 
///  Return TRUE is this IGES item is an untrimmed surface type that we deal with
// ==============================================================================
{
  return iges.entity(item)==IgesReader::rationalBSplineSurface ||
         iges.entity(item)==IgesReader::parametricSplineSurface ||
         iges.entity(item)==IgesReader::tabulatedCylinder ||
	 iges.entity(item)==IgesReader::surfaceOfRevolution;
}


void MappingsFromCAD::
fileContents( aString fileName, IgesReader * &iges_, int & numberOfNurbs, int & numberOfFiniteElements,
	      int & numberOfNodes, int & status )
//===============================================================================================
// /Description:
//    Read an IGES file and find out what it contains
//
//===============================================================================================
{
  numberOfNurbs = 0;
  numberOfFiniteElements = 0;
  numberOfNodes=0;

  int returnValue;
  
  iges_ = new IgesReader;
  returnValue= iges_->readIgesFile((const char*)fileName);

  if( returnValue!=0 )
  {
    printf("Return code from readIgesFile: %i\n", returnValue);
    status = -1;
    return;
  }

// re-open the file as binary to read the data
//  fclose(iges_->fp);  // *wdh* 020617
  iges_->fp = fopen(fileName,"rb");  // *wdh* 020617

  FILE *fp = iges_->fp;
  //  fclose(fp);
// re-open the file as binar to read the data

// *wdh*  fp = fopen(fileName,"rb");  

  assert( iges_!=NULL );
  IgesReader & iges = *iges_;

  bool readUntrimmedSurface=TRUE; 

// read both types of trimmed surfaces:
  bool readTrimmedSurface=TRUE;
  bool readBoundedSurface=TRUE;

  bool listDependentItems=FALSE;  // by default do not list dependent items
  bool readInvisibleItems=FALSE;  // by default do not list invisible items
  
  // Make list of contents
  int numberOfUntrimmedSurfaces=0, numberOfBoundedSurfaces=0, numberOfTrimmedSurfaces=0;

  int numberOfVisibleDependent=0, numberOfUntrimmedAndInvisible=0, numberOfUntrimmedAndDependentBSurfs=0;
  int numberOfBoundaryEntities=0;

  int i;
  for( i=0; i<iges.numberOfEntities(); i++ )
  {
    const bool listThisItem = (readInvisibleItems || iges.isVisible(i)) && 
      ( listDependentItems || iges.isIndependent(i) );

    if( listThisItem &&
	(readUntrimmedSurface && isUntrimmedSurface(iges,i) )   ||
	(readTrimmedSurface && iges.entity(i)==IgesReader::trimmedSurface) ||
	(readBoundedSurface && iges.entity(i)==IgesReader::boundedSurface) ||
	(readTrimmedSurface && iges.entity(i)==IgesReader::plane) )
    {
      numberOfNurbs++;
      if( iges.entity(i)==IgesReader::trimmedSurface || iges.entity(i)==IgesReader::plane)
	numberOfTrimmedSurfaces++;
      if( iges.entity(i)==IgesReader::boundedSurface )
	numberOfBoundedSurfaces++;
      if( isUntrimmedSurface(iges,i) )
	numberOfUntrimmedSurfaces++;
	  
    }
    else if( iges.entity(i)==IgesReader::finiteElement )
    {
      numberOfFiniteElements++;
    }
    else if( iges.entity(i)==IgesReader::node )
    {
      numberOfNodes++;
    }
    else if(  isUntrimmedSurface(iges,i) )
    {
      if ( !iges.isVisible(i) )
      {
	numberOfUntrimmedAndInvisible++;
      }
      else
      {
	numberOfUntrimmedAndDependentBSurfs++;
      }
    }
    else if( iges.entity(i)==IgesReader::boundedSurface )
    {
      numberOfBoundedSurfaces++;
    }
    else if(iges.entity(i)==IgesReader::boundary )
    {
      numberOfBoundaryEntities++;
    }
    else if( iges.isVisible(i) && !iges.isIndependent(i) )
      numberOfVisibleDependent++;

  } // end for i

// print info 
  printf("There are %i surfaces (NURBS, splines...)\n", numberOfNurbs );
  printf("There are %i finite elements and %i nodes\n", numberOfFiniteElements, numberOfNodes );

  printf("There are %i trimmed surfaces and %i untrimmed surfaces\n", numberOfTrimmedSurfaces,
	 numberOfUntrimmedSurfaces);
  if( numberOfUntrimmedAndDependentBSurfs > 0 )
  {
    printf("** INFO: There are %i untrimmed Nurbs in the file that are not listed since they are DEPENDENT.\n",
	   numberOfUntrimmedAndDependentBSurfs);
  }

  if ( numberOfUntrimmedAndInvisible > 0 )
  {
    printf("** INFO: There are %i untrimmed Nurbs that are not listed since they are INVISIBLE.\n",
	   numberOfUntrimmedAndInvisible);
  }

  if( numberOfVisibleDependent>0 )
    printf("** INFO: There are %i visible but dependent items other than untrimmed Nurbs.\n",
	   numberOfVisibleDependent);

  status = 0; // all is well
}

CompositeSurface * MappingsFromCAD::
readSomeNurbs( MappingInformation & mapInfo, IgesReader *iges_, int startMap, int endMap, 
	       int numberOfNurbs, int & error ) // numberOfNurbs should be stored in the IgesReader class
//===============================================================================================
// /Description:
//    Read the specified surfaces in an IGES file and build Mappings. 
//
// /mapInfo (input/output): This object supplies the graphics interface to use. 
//      Mappings created are saved here.
//
// /Notes:
// \begin{itemize}
//   \item Mappings chosen are added to the mapInfo.mappingList.
//   \item mapToItem(mapNumber) = item number in the IgesReader.
// \end{itemize}
//===============================================================================================
{
  CompositeSurface *csPointer = NULL ; // pointer to the composite surface that will be made

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
    
  mapInfoPointer=&mapInfo;

// check numberOfNurbs, startMap and endMap
  if (numberOfNurbs <= 0)
  {
    error = 1;
    return csPointer;
  }
  else if (startMap<0 || startMap > endMap)
  {
    error = 2;
    return csPointer;
  }
  else if (endMap < startMap || endMap >= numberOfNurbs)
  {
    error = 3;
    return csPointer;
    
  }
  else if (iges_ == NULL)
  {
    error = 4;
    return csPointer;
  }
  bool readUntrimmedSurface=TRUE; 

// read both types of trimmed surfaces:
  bool readTrimmedSurface=TRUE;
  bool readBoundedSurface=TRUE;

  bool listDependentItems=FALSE;  // by default do not list dependent items
  bool readInvisibleItems=FALSE;  // by default do not list invisible items
  
  IgesReader & iges = *iges_;
  const int numberOfItems=numberOfNurbs;

  IntegerArray mapNumber;
  int initialNumberOfMaps=mapInfo.mappingList.getLength();
  mapNumber.redim(initialNumberOfMaps+numberOfNurbs+100);   // + number of trimmed NURBS

  int numberOfMapsCreated=0;
  int map=0;
  Mapping *mapPointer;
  
// Do the actual reading
  real time0=getCPU();
  
  int visibleItem=-1;
  map=startMap;

  // ***********************************************************************************
  // **** Here we loop over all items in the IgesReader object and build the appropriate items: ****
  // ***********************************************************************************

  for( int item=0; item<iges.numberOfEntities(); item++ )
  {
    const bool listThisItem = (readInvisibleItems || iges.isVisible(item)) && 
      ( listDependentItems || iges.isIndependent(item) );

    //    cout<<readInvisibleItems<<"  "<<iges.isVisible(item)<<"  "<<listDependentItems<<"  "<<iges.isIndependent(item)<<"  "<<iges.entityName(iges.entity(item))<<endl;
    if( listThisItem && 
	( (readUntrimmedSurface &&  isUntrimmedSurface(iges,item)) ||
	  (readTrimmedSurface         && iges.entity(item)==IgesReader::trimmedSurface) ||
	  (readBoundedSurface   && iges.entity(item)==IgesReader::boundedSurface) ||
	  iges.entity(item)==IgesReader::tabulatedCylinder ||
	  iges.entity(item)==IgesReader::surfaceOfRevolution  ) )
    {
      visibleItem++;
      if( visibleItem < startMap )
	continue;
      else if( visibleItem > endMap )
	break;
	  
      mapPointer=NULL;

      if ( Mapping::debug & 2 )
	printf("attempting to create item=%i, %s\n",item,(const char*)iges.entityName(iges.entity(item)));

      int status = createSurface(item,iges,mapPointer);

// add the mapping to the list in mapInfo
      if( mapPointer!=NULL )
      {
	mapInfo.mappingList.addElement(*mapPointer);      
	mapPointer->decrementReferenceCount();
	    
	mapNumber(numberOfMapsCreated++)=visibleItem;
	if( Mapping::debug & 2 )
	  cout << "*****item  " << numberOfMapsCreated-1 << " created *****\n";
	else
	  if ( status == 0 )
	    cout << "+";
	  else 
	    cout << "-";

	map++;
      }
      else if ( status!=0 )
      {
	if( Mapping::debug & 2 )
	  cout<<"\nError reading "<<iges.entityName(iges.entity(item))<<" entity, item : "<<item<<endl;
	else
	  cout<< "e";
      }
    } 
    else if ( listThisItem && iges.entity(item)==IgesReader::singularSubfigureInstance )
    {
      //kkc 040213 added construction of singular subfigure instance surfaces
      //           These items consist of collections of surfaces translated/scaled/rotated to 
      //           their actual locations.
      realArray data(6), ftrans(3);
      ftrans = 0;
      iges.readData( item, data, 6 );
      int subfigureDef_ptr = int(data(1));
      for ( int a=0; a<3; a++ )
	ftrans(a) = data(a+2);
      real S = data(5);
      
      cout<<"singular subfig instance params : "<<ftrans(0)<<"  "<<ftrans(1)<<"  "<<ftrans(2)<<"  "<<S<<endl;
      if ( subfigureDef_ptr==-1 )
	{
	  cout<<"ERROR : subfigure instance does not point to a valid subfigure definition!"<<endl;
	  continue;
	}

      int subFigDef = iges.sequenceToItem(subfigureDef_ptr);
      int matrix=iges.matrix(item);
      RealArray rotation(3,3),translation(3);
      rotation = 0;
      rotation(0,0) = rotation(1,1) = rotation(2,2) = 1;
      translation = 0;

      if ( matrix!=0 )
	{
	  getTransformationMatrix(iges.sequenceToItem(matrix), iges, rotation, translation);	
	}
      //cout<<"WARNING : subfigure instance has a transformation matrix"<<endl;
      
      data.resize(4);
      iges.readData(subFigDef, data, 4);
      int depth = int(data(1));
      // data(2) should have been a string with the subfigure name
      int nEnt  = int(data(3));
      cout<<"depth "<<depth<<"  nEnt  "<<nEnt<<endl;

      int subFigDefMatrix=iges.matrix(subFigDef);

      if ( subFigDefMatrix!=0 )
	cout<<"WARNING : subfigure definition has a transformation matrix"<<endl;



      if ( depth>0 )
	{
	  cout<<"ERROR : NOT SUPPORTED : subfigure definition contains nested subfigure instances!"<<endl;
	  continue;
	}

      data.resize(data.getLength(0) + nEnt);
      iges.readData(subFigDef, data,data.getLength(0));
      
      for ( int sfe=0; sfe<nEnt; sfe++ ) 
	{
	  int item = iges.sequenceToItem(int(data(4+sfe)));
	  visibleItem++;
	  if( visibleItem < startMap )
	    continue;
	  else if( visibleItem > endMap )
	    break;
	  
	  mapPointer=NULL;
	  
	  if ( Mapping::debug & 2 )
	    cout<<"\nattempting to create a "<<iges.entityName(iges.entity(item))<<endl;
	  
	  int status = createSurface(item,iges,mapPointer);
	  
	  // add the mapping to the list in mapInfo
	  if( mapPointer!=NULL )
	    {
	      if ( matrix!=0 )
		{
		  if ( mapPointer->getClassName()=="NurbsMapping" )
		    {
		      ((NurbsMapping *)mapPointer)->matrixTransform(rotation);
		      ((NurbsMapping *)mapPointer)->shift(translation(0),
							  translation(1),
							  translation(2));
		    }
		  else if ( mapPointer->getClassName()=="TrimmedMapping" )
		    {
		      TrimmedMapping *tm = (TrimmedMapping *)mapPointer;
		      if ( tm->surface && tm->surface->getClassName()=="NurbsMapping" )
			{
			  ((NurbsMapping *)tm->surface)->matrixTransform(rotation);
			  ((NurbsMapping *)tm->surface)->shift(translation(0),
							       translation(1),
							       translation(2));

			  tm->setUnInitialized();
			  //			  tm->mappingHasChanged();
			}
		      else if ( tm->surface )
			{
			  Mapping *oldMap = tm->surface;
			  oldMap->incrementReferenceCount();
			  MatrixTransform *newMap = new MatrixTransform(*oldMap);
			  newMap->rotate(rotation);
			  newMap->shift(translation(0),
					translation(1),
					translation(2));
			  if ( oldMap->decrementReferenceCount()==0 )
			    delete oldMap;

			  int nt = tm->getNumberOfTrimCurves();
			  Mapping **tc = new Mapping* [nt];
			  for ( int c=0; c<nt; c++ )
			    {
			      tc[c] = tm->trimCurves[c];
			      tc[c]->incrementReferenceCount();
			    }

			  tm->setCurves( *newMap, nt, tc );
			  for ( int c=0; c<nt; c++ )
			    {
			      if ( tc[c]->decrementReferenceCount()==0 )
				delete tc[c];
			    }
			  delete [] tc;
			  //			  tm->surface = newMap;
			  //			  tm->setUnInitialized();
			}
		      else
			{
			  cout<<"ERROR : trimmed surface has a null surface pointer! cannot perform matrix transform"<<endl;
			}
		    }
		  else
		    {
		      Mapping *oldMap = mapPointer;
		      MatrixTransform *newMap = new MatrixTransform(*oldMap);
		      newMap->rotate(rotation);
		      newMap->shift(translation(0),
				    translation(1),
				    translation(2));
		      if ( oldMap->decrementReferenceCount()==0 )
			delete oldMap;

		      mapPointer = (Mapping *)newMap;
		    }

		}

	      mapInfo.mappingList.addElement(*mapPointer);      
	      mapPointer->decrementReferenceCount();
	      
	      mapNumber(numberOfMapsCreated++)=visibleItem;
	      if( Mapping::debug & 2 )
		cout << "*****item  " << numberOfMapsCreated-1 << " created *****\n";
	      else
		if ( status == 0 )
		  cout << "+";
		else 
		  cout << "-";
	      
	      if ( matrix!=0 )	      
	      map++;
	    }
	  else if ( status!=0 )
	    {
	      if( Mapping::debug & 2 )
		cout<<"\nError reading "<<iges.entityName(iges.entity(item))<<" entity, item : "<<item<<endl;
	      else
		cout<< "e";
	    }
	}
    }

  } // end for item

  cout<<"finished reading mappings"<<endl;

  // save the surfaces that were created as a CompositeSurface
  int numberOfSurfacesCreated = numberOfMapsCreated;

  // Generate a CompositeMapping for all NURBS
  if( numberOfSurfacesCreated > 0 )
  {
    printf("\nGenerating a CompositeSurface\n");
    csPointer = new CompositeSurface;
    csPointer->incrementReferenceCount();
      
    CompositeSurface & cs = *csPointer;
    cs.setName(Mapping::mappingName,"New CompositeSurface");

    startMap=0, endMap=numberOfSurfacesCreated;
    printf("adding "); // should only add the nurbs!!!
    for( map=max(0,startMap); map<=min(endMap,numberOfSurfacesCreated-1); map++ )
      {
	cs.add(*mapInfo.mappingList[map+initialNumberOfMaps].mapPointer,mapNumber(map));
	cs.setColour(map,gi.getColourName(map));
      
	printf("%i,",map);
	fflush(stdout);
      }
    printf("\n");
//      mapInfo.mappingList.addElement(cs); 
//      csPointer->decrementReferenceCount();
  }


//    // read any finite elements and put them into unstructuredMappings
//    if( totalNumberOfFiniteElements>0 )
//    {
//      printf("\nGenerating unstructured mappings...\n");
//      for( file=0; file<numberOfIgesFiles; file ++ )
//      {
//        // skip the file if there are no finite elements in it
//        if (numberOfFiniteElements[file] == 0)
//  	continue;
//        assert( igesArray[file]!=NULL );
//        IgesReader & iges = *igesArray[file];

//        readFiniteElements( iges );

//        // get the pointer to the last mapping added
//        int num = mapInfo.mappingList.getLength();
//        if( num>0 )
//        {
//  	mapPointer = &mapInfo.mappingList[num-1].getMapping();
//        }

//        if( mapPointer!=NULL )
//        {
//  	mapNumber(numberOfMapsCreated++)=visibleItem;
//  	cout << "*****item  " << numberOfMapsCreated-1 << " created *****\n";

//          // call the update function for each finite element mapping	    
//  	UnstructuredMapping *triPtr = (UnstructuredMapping *) mapPointer;
//  	UnstructuredMapping & tri = *triPtr;
	
//  	tri.update(mapInfo);
//        }

//      } // end for all files
//    }

  printf("\n ++++readSomeSurfaces: time to generate Mappings = %8.2e (time for Trimmed = %8.2e)\n\n",
            getCPU()-time0,timeToBuildTrimmedMappings);

  error = 0; // all is well
  
  return csPointer;
}

int 
buildParameterCurveFromSpaceCurve( NurbsMapping & curve, realArray & endPoint, 
                                   Mapping & surface, NurbsMapping *&rCurve,
                                   const int edgeOrientation, const int periodic )
//===============================================================================================
/// \details 
///     Create a curve in the parameter space of a surface by projecting a curve in R3 onto the surface.
/// 
/// \param curve (input) : space curve to project
/// \param endPoint(2,3) (input) : points in R3 that denote the start and end points on "curve"
/// \param surface (input) : surface in R3 on which to project
/// \param rCurve (output) : pointer to the parameter curve 
/// \param Notes:
/// 
//===============================================================================================
{
  if( rCurve==NULL ) 
  {
    rCurve = new NurbsMapping;
    rCurve->incrementReferenceCount();
  }
  
  real ta=0., tb=1.;

  if( periodic==0 )
  {
    // find the parameter positions of the end points on the curve 
    realArray rc(2,1);
    curve.inverseMap(endPoint,rc);

    ta=rc(0,0);
    tb=rc(1,0);
  }
  
  printf("    ---build parameter curve: end points in parameter space: (ta,tb)=(%6.4f,%6.4f) x0=(%8.5f,%8.5f,%8.5f)"
                 " x1=(%8.5f,%8.5f,%8.5f)\n",ta,tb,endPoint(0,0),endPoint(0,1),endPoint(0,2),endPoint(1,0),endPoint(1,1),endPoint(1,2));
   
  int nt=51;  // evaluate the space curve a this many points
  real dt=(tb-ta)/(nt-1);
  
  realArray t(nt,1),x(nt,3), r(nt,2);
  
  t.seqAdd(ta,dt); // use equally spaced points
  
  curve.map(t,x);
  
  r=-1;
  surface.inverseMap(x,r);

  printf(" ***surface getIsPeriodic=%i %i\n",surface.getIsPeriodic(axis1),surface.getIsPeriodic(axis2));
  
  ::display(r,"r coordinates on surface for parameter curve","%4.2f ");

  // adjust points near branch cuts to be on the correct side of the branch cut.

  // The material of the face lies to the left of the curve
  //  if r-periodic
  //    if( s(i+1)>s(i) ) prefer r=1 else prefer r=0
  //  if s-periodic 
  //    if( r(i+1)>r(i) ) prefer s=0 else prefer s=1
  const real rEps=.001; // REAL_EPSILON*1000.;
  for( int axis=0; axis<2; axis++ )
  {
    if( surface.getIsPeriodic(axis)==Mapping::functionPeriodic )
    {
      int dir=axis;
      int dirp1 = (axis+1) % 2;
      const real shift = axis==0 ? 1. : -1.;
      int i;
      if( false )
      {
        const real jump=.75; // if the r coordinate jumps by more than this amount we must have crossed a branch cut
        int im1=nt-1;
	for( i=0; i<nt; i++ )
	{
	  real diff = fabs(r(i,dir)-r(im1,dir));
	  if( diff>jump )
	  {
            if( r(i,dir)>r(im1,dir) )
	    {
	      r(i,dir)-=1.;
	    }
	    else
	    {
	      r(i,dir)+=1.;
	    }
	  }
          im1=i;
	}
      }
      else
      {
	for( i=0; i<nt-1; i++ )
	{
	  // real diff = fabs(r(i+1,dir)-r(i,dir));
	  if( fabs(r(i,dir))<rEps )
	  {
	    if( r(i+1,dirp1)>r(i,dirp1) )
	      r(i,dir)+=shift;
	  }
	  else if( fabs(r(i,dir)-1.)<rEps )
	  {
	    if( r(i+1,dirp1)>r(i,dirp1) )
	      r(i,0)-=shift; 
	  }
	}
	// specal case: last point
	i=nt-1;
	if( fabs(r(i,dir))<rEps )
	{
	  if( r(i,dirp1)>r(i-1,dirp1) )
	    r(i,dir)+=shift;
	}
	else if( fabs(r(i,dir)-1.)<rEps )
	{
	  if( r(i,dirp1)>r(i-1,dirp1) )
	    r(i,0)-=shift; 
	}
      }
      
    }
  }
  

  // should check that points were inverted properly *****

  int degree=1;  // do this for now
  rCurve->interpolate(r,0,Overture::nullRealDistributedArray(),degree);

  return 0;
}

int
createSurfaceForAFace( Mapping *&surface, int surf,
                       IgesReader &iges, 
		       MappingInformation & mapInfo, IntegerArray & mapNumber, 
		       int & numberOfMapsCreated, int & visibleItem)
//===============================================================================================
/// \details 
///     Create the mapping (surface) that defines the geometry of a face on a manifold solid BRep.
/// 
/// \param mapInfo (input/output): This object supplies the graphics interface to use. 
/// 
/// \param Notes:
/// 
//===============================================================================================
{
  if( iges.entity(surf)==IgesReader::rightCircularCylindricalSurface )
  {
    int form=iges.formData(surf);
    realArray rccsData(5);
    iges.readData(surf,rccsData,5);
    real radius=rccsData(3);

    int point =(int)iges.sequenceToItem(int(rccsData(1))); // point iges type=116
    realArray pointData(4);
    iges.readData(point,pointData,4);
    real xp=pointData(1), yp=pointData(2), zp=pointData(3);

    int axis =(int)iges.sequenceToItem(int(rccsData(2)));  // direction iges type=123
    realArray axisData(4);
    iges.readData(axis,axisData,4);
    real xa=axisData(1), ya=axisData(2), za=axisData(3);

    int refDir=0;
    real xd=1., yd=0., zd=0.;
    if( form==1 )
    { // parameterized
      refDir =(int)iges.sequenceToItem(int(rccsData(4)));
      realArray refDirData(4);
      iges.readData(refDir,refDirData,4);
      xd=refDirData(1), yd=refDirData(2), zd=refDirData(3);
    }
    else
    {
      // find a reference direction -- a direction orthogonal to (xa,ya,za) 

      if( fabs(za) <= min(fabs(xa),fabs(ya)) )
      { // za is smallest, use: 
	xd=-ya; yd=xa; zd=0.;
      }
      else if( fabs(ya) <= min(fabs(xa),fabs(za)) )
      { // ya is smallest, use: 
	xd=-za; yd=0.; zd=xa;
      }
      else
      { // xa is smallest, use: 
	xd=0.; yd=-za; zd=ya;
      }
	
    }
      

    printf("  --surf=rightCircularCylindricalSurface: form=%i (0=, 1=parameterized)\n"
	   "         r=%8.2e pt-on-axis=(%8.2e,%8.2e,%8.2e) axis=(%8.2e,%8.2e,%8.2e) refDir=(%8.2e,%8.2e,%8.2e)\n",
	   form,radius,xp,yp,zp,xa,ya,za,xd,yd,zd);

    // The parameterized surface is defined as
    //      S(u,v) = cv + r*( cos(u)*xv + sin(u)*yv ) + v*zv
    //   cv = point
    //   zv = axis
    //   dv = refDir
    //   xv = dv - (dv.zv)zv
    //   yv = zv X xv 

    // build a circle with centre cv, radius, in the plane defined by the orthogonal unit vectors xv,yv

    real halfLength=radius*2.;  // we have to guess a halfLength  // ***** fix this : use domain bounds ??

    RealArray cv(3), xv(3), yv(3), zv(3), dv(3);

    // shift the centre back a halfLength along the axis
    cv(0)=xp-halfLength*xa; cv(1)=yp-halfLength*ya; cv(2)=zp-halfLength*za;
    // cv0)=xp; cv(1)=yp; cv(2)=zp;
      
    zv(0)=xa; zv(1)=ya; zv(2)=za;

    real dot = xd*xa+yd*ya+zd*za;
    xv(0)=xd-dot*xa; xv(1)=yd-dot*ya; xv(2)=zd-dot*za; 
    real norm=max(REAL_MIN*100.,sqrt(xv(0)*xv(0)+xv(1)*xv(1)+xv(2)*xv(2)));
    xv/=norm;
      
    yv(0)=zv(1)*xv(2)-zv(2)*xv(1);
    yv(1)=zv(2)*xv(0)-zv(0)*xv(2);
    yv(2)=zv(0)*xv(1)-zv(1)*xv(0);
    norm=max(REAL_MIN*100.,sqrt(yv(0)*yv(0)+yv(1)*yv(1)+yv(2)*yv(2)));
    yv/=norm;

    NurbsMapping & circle = *new NurbsMapping;  circle.incrementReferenceCount();

    printf(" ***Build a circle:  cv=(%5.3f,%5.3f,%5.3f) xv=(%5.3f,%5.3f,%5.3f) yv=(%5.3f,%5.3f,%5.3f)\n",
	   cv(0),cv(1),cv(2),  xv(0),xv(1),xv(2),    yv(0),yv(1),yv(2) );
    
 
    circle.circle(cv,xv,yv,radius);
    circle.setGridDimensions(axis1,51);
      
    NurbsMapping & cylinder = *new NurbsMapping;  cylinder.incrementReferenceCount();
    surface = &cylinder;
    
    cylinder.setGridDimensions(axis1,41);
    cylinder.setGridDimensions(axis2,21);

    // build a cylinder using the circle and the direction
    norm=max(REAL_MIN*100.,sqrt(zv(0)*zv(0)+zv(1)*zv(1)+zv(2)*zv(2)));
    dv=zv*(2.*halfLength/norm);
    // dv=zv*radius;
    // dv=-5.*zv; 
    cylinder.generalCylinder( circle,&dv(0) );

    printf(" ***cylinder: circle.getIsPeriodic=%i cylinder=%i %i\n",circle.getIsPeriodic(axis1),
           cylinder.getIsPeriodic(axis1),cylinder.getIsPeriodic(axis2));

    mapInfo.mappingList.addElement(cylinder);      
    if( circle.decrementReferenceCount()==0 ) delete &circle;
    cylinder.decrementReferenceCount();
	    
    visibleItem++;
    mapNumber(numberOfMapsCreated++)=visibleItem;

  }
  else if( iges.entity(surf)==IgesReader::rightCircularConicalSurface )
  {
    // ************************************************************
    // ************** conical surface *****************************
    // ************************************************************

    int form=iges.formData(surf);
    realArray rccsData(6);
    iges.readData(surf,rccsData,6);
    real radius=rccsData(3);
    real semiAngle=rccsData(4)*Pi/180.;  // semi-angle in degrees --> radians

    int point =(int)iges.sequenceToItem(int(rccsData(1))); // point iges type=116
    realArray pointData(4);
    iges.readData(point,pointData,4);
    real xp=pointData(1), yp=pointData(2), zp=pointData(3);

    int axis =(int)iges.sequenceToItem(int(rccsData(2)));  // direction iges type=123
    realArray axisData(4);
    iges.readData(axis,axisData,4);
    real xa=axisData(1), ya=axisData(2), za=axisData(3);

    int refDir=0;
    real xd=1., yd=0., zd=0.;
    if( form==1 )
    { // parameterized
      refDir =(int)iges.sequenceToItem(int(rccsData(5)));
      realArray refDirData(4);
      iges.readData(refDir,refDirData,4);
      xd=refDirData(1), yd=refDirData(2), zd=refDirData(3);
    }
    else
    {
      // find a reference direction -- a direction orthogonal to (xa,ya,za) 

      if( fabs(za) <= min(fabs(xa),fabs(ya)) )
      { // za is smallest, use: 
	xd=-ya; yd=xa; zd=0.;
      }
      else if( fabs(ya) <= min(fabs(xa),fabs(za)) )
      { // ya is smallest, use: 
	xd=-za; yd=0.; zd=xa;
      }
      else
      { // xa is smallest, use: 
	xd=0.; yd=-za; zd=ya;
      }
	
    }
      

    printf("  --surf=rightCircularConcialSurface: form=%i (0=, 1=parameterized) r=%8.2e angle=%8.2e \n"
	   "         pt-on-axis=(%8.2e,%8.2e,%8.2e) axis=(%8.2e,%8.2e,%8.2e) refDir=(%8.2e,%8.2e,%8.2e)\n",
	   form,radius,semiAngle,xp,yp,zp,xa,ya,za,xd,yd,zd);

    // The parameterized surface is defined as
    //      S(u,v) = cv + (r+v*tan(s))*( cos(u)*xv + sin(u)*yv ) + v*zv
    //   cv = point
    //   zv = axis
    //   dv = refDir
    //   xv = dv - (dv.zv)zv
    //   yv = zv X xv 

      // build a circle with centre cv, radius, in the plane defined by the orthogonal unit vectors xv,yv

    real halfLength=radius*2.;  // we have to guess a halfLength  // ***** fix this : use domain bounds ??

    RealArray cv(3), xv(3), yv(3), zv(3), dv(3);

      // center:
    cv(0)=xp; cv(1)=yp; cv(2)=zp;
      
    zv(0)=xa; zv(1)=ya; zv(2)=za;

    real dot = xd*xa+yd*ya+zd*za;
    xv(0)=xd-dot*xa; xv(1)=yd-dot*ya; xv(2)=zd-dot*za; 
    real norm=max(REAL_MIN*100.,sqrt(xv(0)*xv(0)+xv(1)*xv(1)+xv(2)*xv(2)));
    xv/=norm;
      
    yv(0)=zv(1)*xv(2)-zv(2)*xv(1);
    yv(1)=zv(2)*xv(0)-zv(0)*xv(2);
    yv(2)=zv(0)*xv(1)-zv(1)*xv(0);
    norm=max(REAL_MIN*100.,sqrt(yv(0)*yv(0)+yv(1)*yv(1)+yv(2)*yv(2)));
    yv/=norm;

    NurbsMapping & circle1 = *new NurbsMapping;  circle1.incrementReferenceCount();

    real radius1 =radius*2.; // make the cone bigger *** fix this *** 
    // radius+v*tan(s)=radius1 => v=(radius1-radius)/tan(s)
    real v=(radius1-radius)/tan(semiAngle);  // parameter value v where the cone radius is equal to radius1

    RealArray cv1(3);
    cv1 = cv+ zv*v;
    printf(" ***Cone: build circle1:  radius1=%8.2e, cv1=(%5.3f,%5.3f,%5.3f) xv=(%5.3f,%5.3f,%5.3f) "
	   " yv=(%5.3f,%5.3f,%5.3f)\n",radius1,
	   cv1(0),cv1(1),cv1(2),  xv(0),xv(1),xv(2),    yv(0),yv(1),yv(2) );
    
 
    circle1.circle(cv1,xv,yv,radius1);
    circle1.setGridDimensions(axis1,51);
      
    // Make another circle near the origin of the cone
    NurbsMapping & circle2 = *new NurbsMapping;  circle2.incrementReferenceCount();

    real radius2 =radius*REAL_EPSILON*100.;
    // radius+v*tan(s)=radius2 => v=(radius2-radius)/tan(s)
    v=(radius2-radius)/tan(semiAngle);

    RealArray cv2(3);
    cv2 = cv+zv*v;
    printf(" ***Cone: build circle2:  radius2=%8.2e, cv2=(%5.3f,%5.3f,%5.3f) xv=(%5.3f,%5.3f,%5.3f) "
	   "yv=(%5.3f,%5.3f,%5.3f)\n",radius2, cv2(0),cv2(1),cv2(2),  xv(0),xv(1),xv(2),    yv(0),yv(1),yv(2) );
    
    circle2.circle(cv2,xv,yv,radius2);
    circle2.setGridDimensions(axis1,51);

    NurbsMapping & cylinder = *new NurbsMapping;  cylinder.incrementReferenceCount();
    surface = &cylinder;


      // build a "cylinder" that interpolates between the two circles

    cylinder.generalCylinder( circle1,circle2 );

    cylinder.setGridDimensions(axis1,41);
    cylinder.setGridDimensions(axis2,21);

    printf(" ***cone: circle1.getIsPeriodic=%i cone=%i %i\n",circle1.getIsPeriodic(axis1),
           cylinder.getIsPeriodic(axis1),cylinder.getIsPeriodic(axis2));

    mapInfo.mappingList.addElement(cylinder);      
    if( circle1.decrementReferenceCount()==0 ) delete &circle1;
    if( circle2.decrementReferenceCount()==0 ) delete &circle2;
    cylinder.decrementReferenceCount();

    visibleItem++;  
    mapNumber(numberOfMapsCreated++)=visibleItem;

  } // end cone
  else if( iges.entity(surf)==IgesReader::planeSurface )
  {
    // ************************************************************
    // **************** plane surface (190)************************
    // ************************************************************

    int form=iges.formData(surf);
    int matrix=iges.matrix(surf);
    if( matrix!=0 )
    {
      printf("***createManifoldSolidBRepObject::WARNING: matrixTransform!=0 for planeSurface. matrix=%i\n",matrix);
      if( matrix!=0 )
      {
	RealArray matrixTransform(3,3), translation(3);
	matrix=iges.sequenceToItem(matrix);
	int returnValue=MappingsFromCAD::getTransformationMatrix(matrix,iges,matrixTransform,translation);
	if( returnValue==0 && Mapping::debug & 1 )
	{
	  ::display(matrixTransform,"planeSurface: matrixTransform");
	  ::display(translation,"planeSurface: translation");
	}
      }
    }

    realArray planeData(4);
    iges.readData(surf,planeData,4);

    int point =(int)iges.sequenceToItem(int(planeData(1))); // point iges type=116
    realArray pointData(4);
    iges.readData(point,pointData,4);
    real xp=pointData(1), yp=pointData(2), zp=pointData(3);

    int normal =(int)iges.sequenceToItem(int(planeData(2)));  // direction iges type=123
    realArray normalData(4);
    iges.readData(normal,normalData,4);
    real xa=normalData(1), ya=normalData(2), za=normalData(3);

    int refDir=0;
    real xd=1., yd=0., zd=0.;
    if( form==1 )
    { // parameterized
      refDir =(int)iges.sequenceToItem(int(planeData(3)));
      realArray refDirData(4);
      iges.readData(refDir,refDirData,4);
      xd=refDirData(1), yd=refDirData(2), zd=refDirData(3);
    }
    else
    {
      // find a reference direction -- a direction orthogonal to (xa,ya,za) 

      if( fabs(za) <= min(fabs(xa),fabs(ya)) )
      { // za is smallest, use: 
	xd=-ya; yd=xa; zd=0.;
      }
      else if( fabs(ya) <= min(fabs(xa),fabs(za)) )
      { // ya is smallest, use: 
	xd=-za; yd=0.; zd=xa;
      }
      else
      { // xa is smallest, use: 
	xd=0.; yd=-za; zd=ya;
      }
	
    }
    realArray cv(3),zv(3), xv(3), yv(3);
      
    cv(0)=xp; cv(1)=yp; cv(2)=zp;
    zv(0)=xa; zv(1)=ya; zv(2)=za;

    real dot = xd*xa+yd*ya+zd*za;
    xv(0)=xd-dot*xa; xv(1)=yd-dot*ya; xv(2)=zd-dot*za; 
    real norm=max(REAL_MIN*100.,sqrt(xv(0)*xv(0)+xv(1)*xv(1)+xv(2)*xv(2)));
    xv/=norm;
      
    yv(0)=zv(1)*xv(2)-zv(2)*xv(1);
    yv(1)=zv(2)*xv(0)-zv(0)*xv(2);
    yv(2)=zv(0)*xv(1)-zv(1)*xv(0);
    norm=max(REAL_MIN*100.,sqrt(yv(0)*yv(0)+yv(1)*yv(1)+yv(2)*yv(2)));
    yv/=norm;

      
    NurbsMapping & plane = *new NurbsMapping;  plane.incrementReferenceCount();
    surface = &plane;

    real pt1[3], pt2[3], pt3[3];

    real length=40.;   // choose a size for the plane -- fix this 
    for( int m=0; m<3; m++ )
    {
      pt1[m]=cv(m) -xv(m)*length - yv(m)*length;
      pt2[m]=cv(m) +xv(m)*length - yv(m)*length;
      pt3[m]=cv(m) -xv(m)*length + yv(m)*length;
    }
      
    plane.plane(pt1,pt2,pt3);

    mapInfo.mappingList.addElement(plane);      
    plane.decrementReferenceCount();
	    
    visibleItem++;
    mapNumber(numberOfMapsCreated++)=visibleItem;
  }
  else
  {
    printf("\n ****createSurfaceForAFace:ERROR: Surface type surf=%s not handled yet ****\n\n",
	   (const char*)iges.entityName(iges.entity(surf)));
  }

  if( surface!=NULL ) 
    return 0;
  else 
    return 1;
}


int MappingsFromCAD::
createManifoldSolidBRepObject(int entity, IgesReader &iges, 
			      MappingInformation & mapInfo, IntegerArray & mapNumber, 
                              int & numberOfMapsCreated, int & visibleItem)
//===============================================================================================
/// \details 
///     Build a set of surfaces that are defined by a solid model
/// 
/// \param mapInfo (input/output): This object supplies the graphics interface to use. 
///       Mappings created are saved here.
/// 
/// \param Notes:
/// 
//===============================================================================================
{
  printf(" >>>>INFO: inside createManifoldSolidBRepObject <<<<\n");
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  visibleItem--;  // this will be incremented again below
  
  realArray data(4);
  iges.readData(entity,data,4);

  int shell = iges.sequenceToItem(int(data(1)));  // this is the DE of the shell

  int orientation = (int)data(2);
  
  int numberOfVoidShells=(int)data(3);
  printf(" ---> orientation=%i, numberOfVoidShells=%i\n",orientation,numberOfVoidShells);
  

  // read a shell
  iges.readData(shell,data,2);  

  int numberOfFaces=(int)data(1);
  printf(" Shell: numberOfFaces=%i\n",numberOfFaces);
  
  data.redim(2+numberOfFaces*2);
  iges.readData(shell,data,2+numberOfFaces*2);
  for( int f=0; f<numberOfFaces; f++ )
  {
    int face = (int)iges.sequenceToItem(int(data(2+2*f)));
    int faceOrientation = (int)data(3+2*f);

    realArray faceData(4);
    iges.readData(face,faceData,4);

    int surf = (int)iges.sequenceToItem(int(faceData(1)));
    int numberOfLoops=(int)faceData(2);
    int outerLoopFlag=(int)faceData(3);
    printf(" face f=%i, faceOrientation=%i, numberOfLoops=%i outerLoopFlag=%i surf=%s (de=%i)\n",f,faceOrientation,
	   numberOfLoops,outerLoopFlag, (const char*)iges.entityName(iges.entity(surf)),surf);
    
    // ===== build the surface for this face =====
    Mapping *surface=NULL;
    createSurfaceForAFace( surface, surf,iges, mapInfo,mapNumber,numberOfMapsCreated,visibleItem);
    if( surface==NULL )
    {
      printf("\n\n ********** ERROR creating the surface for this face, f=%i ****************\n\n",f);
    }
    


    faceData.redim(4+numberOfLoops);
    iges.readData(face,faceData,4+numberOfLoops);

    NurbsMapping **loopCurve = new NurbsMapping * [numberOfLoops];

    for( int l=0; l<numberOfLoops; l++ )
    {
      loopCurve[l]=NULL;  // holds the trim curve defining this loop

      int loop=(int)iges.sequenceToItem(int(faceData(4+l)));

      realArray loopData(7);
      iges.readData(loop,loopData,7);
      int numberOfEdgeTuples=(int)loopData(1);
      printf("  ..loop l=%i: numberOfEdgeTuples=%i\n",l,numberOfEdgeTuples);

      // ****
      int edgeCount=3;
      int numberOfParameterSpaceCurves=(int)loopData(edgeCount+3);
      // read an extra 3 so we get the next numberOfParameterSpaceCurves:
      int endLoopData=edgeCount+5+2*numberOfParameterSpaceCurves +3; 
      loopData.redim(endLoopData);
      
      iges.readData(loop,loopData,endLoopData);
      realArray edgeListData;
      int edgeListID=-1; // id of the current edgeList (-1 = no edge list has been read)
      
      realArray vertexListData;
      int vertexListID=-1; // id of the current vertexList (-1 = no list has been read)
      int numberOfVertexTuplesInList;
      

      for( int e=0; e<numberOfEdgeTuples; e++ )
      {
	int edge=(int)iges.sequenceToItem(int(loopData(edgeCount)));
        int edgeType=(int)loopData(edgeCount-1);
        int edgeListIndex=int(loopData(edgeCount+1));
	int edgeOrientation=int(loopData(edgeCount+2));
	numberOfParameterSpaceCurves=(int)loopData(edgeCount+3);

        printf("    ..edge e=%i, edge (de)=%i , edgeType=%i (0=edge, 1=vertex) edgeListIndex=%i, "
                "edgeOrientation=%i, numberOfParameterSpaceCurves=%i\n",
	       e,edge,edgeType,edgeListIndex,edgeOrientation,numberOfParameterSpaceCurves);

        if( edgeType==1 )
	{
	  printf("\n     *****WARNING: The edgeType is a VERTEX -- finish this case *****\n\n");
	}

        for( int c=0; c<numberOfParameterSpaceCurves; c++ )
	{
          int isoParametricFlag=int(loopData(edgeCount+3+2*c));
	  int curve=(int)iges.sequenceToItem(int(loopData(edgeCount+3+1+2*c)));
          printf("      ...curve c=%i isoParametricFlag=%i curve=%s\n",c,isoParametricFlag,
                  (const char*)iges.entityName(iges.entity(curve)));
	}

        // --- read the edge list if we haven't read it yet ---
        if( edge!=edgeListID )
	{
	  edgeListID=edge;
	  edgeListData.redim(2);
	  iges.readData(edge,edgeListData,2);
	  int numberOfEdgeTuplesInList=(int)edgeListData(1);
	  edgeListData.redim(2+5*numberOfEdgeTuplesInList);
	  iges.readData(edge,edgeListData,edgeListData.getLength(0));
	}

        int edgeCurvePointer=2+5*(edgeListIndex-1);
        int edgeCurve = (int)iges.sequenceToItem(int(edgeListData(edgeCurvePointer)));
	int startVertexList=(int)iges.sequenceToItem(int(edgeListData(edgeCurvePointer+1)));
	int startVertexIndex=int(edgeListData(edgeCurvePointer+2));
	int endVertexList=(int)iges.sequenceToItem(int(edgeListData(edgeCurvePointer+3)));
	int endVertexIndex=int(edgeListData(edgeCurvePointer+4));
	
        // ==== Find the end-points of rhe edge =====
        realArray endPoint(2,3);

	// --- read the vertex list if we haven't read it yet ---
        if( startVertexList!=vertexListID )
	{
	  vertexListID=startVertexList;
	  vertexListData.redim(2);
	  iges.readData(vertexListID,vertexListData,2);
	  numberOfVertexTuplesInList=(int)vertexListData(1);
	  vertexListData.redim(2+3*numberOfVertexTuplesInList);
	  iges.readData(vertexListID,vertexListData,vertexListData.getLength(0));
	}

        // check the orientation and reverse the end points if necessary
        const int i0=edgeOrientation==1 ? 0 : 1;
	const int i1=edgeOrientation==1 ? 1 : 0;

        assert( startVertexIndex<=numberOfVertexTuplesInList );
        int vertexOffset=2+(startVertexIndex-1)*3;
        endPoint(i0,0)=vertexListData(vertexOffset);
        endPoint(i0,1)=vertexListData(vertexOffset+1);
        endPoint(i0,2)=vertexListData(vertexOffset+2);

        assert( endVertexList==startVertexList ); // do this for now 

        assert( endVertexIndex<=numberOfVertexTuplesInList );
	vertexOffset=2+(endVertexIndex-1)*3;
        endPoint(i1,0)=vertexListData(vertexOffset);
        endPoint(i1,1)=vertexListData(vertexOffset+1);
        endPoint(i1,2)=vertexListData(vertexOffset+2);
        

        printf("       ...edgeCurve=%s (de)=%i startVertexList=%i (de), startVertexIndex=%i, "
                "endVertexList=%i (de), endVertexIndex=%i \n"
	       "         --> start-vertex=(%8.2e,%8.2e,%8.2e) end-vertex=(%8.2e,%8.2e,%8.2e)\n",
	       (const char*)iges.entityName(iges.entity(edgeCurve)),edgeCurve,startVertexList,startVertexIndex,
                           endVertexList, endVertexIndex,
                  endPoint(0,0),endPoint(0,1),endPoint(0,2), endPoint(1,0),endPoint(1,1),endPoint(1,2));

        // note: what if the edgeType is a "vertex" ****

	if( iges.entity(edgeCurve)==IgesReader::circularArc || 
            iges.entity(edgeCurve)==IgesReader::line )
	{
          // ============================================================
          // ==== Build the model space curve representing this edge ====
          // ============================================================

          NurbsMapping & curve = *new NurbsMapping; curve.incrementReferenceCount();
          curve.setGridDimensions(axis1,31);
	  curve.readFromIgesFile(iges,edgeCurve);
	  
	  mapInfo.mappingList.addElement(curve);      
	  curve.decrementReferenceCount();
	  visibleItem++;
	  mapNumber(numberOfMapsCreated++)=visibleItem;


          // ========================================================================
          // ==== Build the parameter space curve by projecting onto the surface ====
          // ========================================================================
          // fill in end points


          int periodic = startVertexIndex==endVertexIndex;  
	  
          NurbsMapping *rCurve=NULL;
          if( surface!=NULL )
	  {
            buildParameterCurveFromSpaceCurve( curve, endPoint, *surface, rCurve, edgeOrientation, periodic );
	  

          // ===== Merge the parameter curve with current trim curve ====
	    if( loopCurve[l]==NULL )
	    {
	      loopCurve[l]=rCurve;
	    }
	    else
	    {
              if( false )
	      {
		loopCurve[l]->merge(*rCurve);
	      }
	      else
	      { // do this for now
		loopCurve[l]->addSubCurve(*rCurve);
	      }
	      
	    }
	  }
	  
	}

        // now read more data..
        if( e<numberOfEdgeTuples-1 )
	{
	  numberOfParameterSpaceCurves=(int)loopData(edgeCount+3);
	  edgeCount+=5+2*numberOfParameterSpaceCurves;
          // read an extra 3 so we get the next numberOfParameterSpaceCurves: 
	  endLoopData=edgeCount+4+2*numberOfParameterSpaceCurves +3; 
	  loopData.redim(endLoopData);
	  iges.readData(loop,loopData,endLoopData);
	}
      }  // end for e
      
      if( true || Mapping::debug & 1 )
      {
        // plot the trim curve

        if( loopCurve[l]!=NULL )
	{
          
          if( true )
	  {
	    loopCurve[l]->update(mapInfo) ;
	  }
	  else
	  {
	    GraphicsParameters params;
	    params.set(GI_TOP_LABEL,sPrintF("Face %i loop %i",f,l));
	    params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);

      
	    PlotIt::plot(gi,*loopCurve[l],params);
    
	    params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
	    params.set(GI_TOP_LABEL," ");
	  }
	  
	}
	
      }
      

    } // end for l (loop)
    
    

  }
  
  return 0;
}



int MappingsFromCAD::
readMappings( MappingInformation & mapInfo, aString fileName /* = nullString */, 
	      bool chooseAll /*= FALSE*/ )
//===============================================================================================
/// \details 
///     Read a CAD file and build Mappings. 
/// 
/// \param Supported files:
///     Currently only IGES files can be read.
/// 
/// \param mapInfo (input/output): This object supplies the graphics interface to use. 
///       Mappings created are saved here.
/// 
/// \param Notes:
///  <ul>
///    <li> Mappings chosen are added to the mapInfo.mappingList.
///    <li> mapToItem(mapNumber) = item number in the IgesReader.
///  </ul>
//===============================================================================================
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
    
  mapInfoPointer=&mapInfo;

  const int maximumNumberOfIgesFiles=100;
  int numberOfIgesFiles=0;
  IgesReader *igesArray[maximumNumberOfIgesFiles];  // array of pointers to different IgesReaders
  FILE *igesFilePointer[maximumNumberOfIgesFiles];
  int i;
  for( i=0; i<maximumNumberOfIgesFiles; i++ )
  {
    igesArray[i]=NULL;
    igesFilePointer[i]=NULL;
  }

  IntegerArray numberOfEntitiesPerFile(maximumNumberOfIgesFiles);
  numberOfEntitiesPerFile=0;

  // read both types of surfaces:
  bool readUntrimmedSurface=TRUE; 
  bool readTrimmedSurface=TRUE;
  bool readBoundedSurface=TRUE;

  bool listDependentItems=FALSE;  // by default do not list dependent items
  bool readInvisibleItems=FALSE;  // by default do not list invisible items

  params.set(GI_LABEL_GRIDS_AND_BOUNDARIES,FALSE);
  
  params.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,FALSE);
  params.set(GI_GRID_LINE_COLOUR_OPTION,GraphicsParameters::colourByGrid);

  bool useFileName = (fileName != nullString);

  // Open all IGES files
  aString answer,line;
  aString menu[]=
  {
    "!Open IGES files",
    "open another IGES file",
    "continue",
    ""   // null string terminates the menu
  };
  
  gi.appendToTheDefaultPrompt("Open IGES files>"); // set the default prompt

  bool done=FALSE;
  int returnValue;
  
  while( !done )
  {

    if (numberOfIgesFiles != 0) gi.getMenuItem(menu,answer); // Don't show the menu the first time

    // First time, we go here directly
    if (numberOfIgesFiles == 0 || answer == "open another IGES file" ) 
    {
      if( numberOfIgesFiles > maximumNumberOfIgesFiles-1 )
      {
	printf("Sorry, only %i files can be opened\n",maximumNumberOfIgesFiles);
      }
      else
      {
	returnValue=createIgesReader(gi, fileName, igesArray[numberOfIgesFiles],
					 igesFilePointer[numberOfIgesFiles], useFileName );
	if( returnValue!=0 )
	  break;

	if (chooseAll)
	  done = TRUE;
	
	numberOfIgesFiles++;
      }
    }
    else if(answer == "continue" )
    {
      done = TRUE;
    }
  }

  gi.unAppendTheDefaultPrompt();  // reset the prompt

// the re is no point continuing if we couldn't read the files! 
  if (returnValue != 0)
  {
    for( i=0; i<maximumNumberOfIgesFiles; i++ ) // *wdh* 030825
    {
      delete igesArray[i];
    }
    return returnValue;
  }

  // Make lists of contents

  int totalNumberOfEntitiesInFiles, totalNumberOfNurbss;
  int totalNumberOfFiniteElements, totalNumberOfNodes, numberOfUntrimmedSurfaces;
  int numberOfBoundedSurfaces, numberOfTrimmedSurfaces, numberOfUntrimmedAndDependentBSurfs;
  int numberOfFiniteElements[maximumNumberOfIgesFiles];
  int numberOfNodes, numberOfVisibleDependent, numberOfUntrimmedAndInvisible;
  int numberOfBoundaryEntities;

  int file;
  int startMap, endMap, numberInList;
  int map;
  Mapping *mapPointer;  // pointer to Mapping that we are currently working on

  int numberOfMapsCreated;
  char buff[200];
  CompositeSurface *csPointer =NULL ; // pointer to last composite surface we have made
  IntegerArray mapNumber,mapList;
  int initialNumberOfMaps;

  // outer loop to get the relevant items listed
  done=FALSE;
  do
  {
    totalNumberOfEntitiesInFiles=0;
    
    totalNumberOfNurbss=0, totalNumberOfFiniteElements=0, totalNumberOfNodes=0;
    numberOfUntrimmedSurfaces=0;

    numberOfTrimmedSurfaces=0, numberOfUntrimmedAndDependentBSurfs=0;
    // ?? numberOfFiniteElements[maximumNumberOfIgesFiles];

    numberOfNodes=0;
    numberOfVisibleDependent=0, numberOfUntrimmedAndInvisible=0;
    numberOfBoundedSurfaces=0;
    numberOfBoundaryEntities=0;

    // Read the information in all files!
    for( file=0; file<numberOfIgesFiles; file ++ )
    {
      // Count the number of items that we can build
      assert( igesArray[file]!=NULL );
      IgesReader & iges = *igesArray[file];
      assert( igesFilePointer[file]!=NULL );
      // FILE *fp = igesFilePointer[file];

      numberOfEntitiesPerFile(file)=iges.numberOfEntities();
      totalNumberOfEntitiesInFiles+=iges.numberOfEntities();
      int numberOfNurbss=0;
      numberOfFiniteElements[file]=0;
      
      for( i=0; i<iges.numberOfEntities(); i++ )
      {
	const bool listThisItem = (readInvisibleItems || iges.isVisible(i)) && 
	  ( listDependentItems || iges.isIndependent(i) );

	if( listThisItem &&
	    (readUntrimmedSurface && isUntrimmedSurface(iges,i) )   ||
	    (readTrimmedSurface && iges.entity(i)==IgesReader::trimmedSurface) ||
	    (readBoundedSurface && iges.entity(i)==IgesReader::boundedSurface) )
        {
	  numberOfNurbss++;
	  if( iges.entity(i)==IgesReader::trimmedSurface )
	    numberOfTrimmedSurfaces++;
	  if( iges.entity(i)==IgesReader::boundedSurface )
	    numberOfBoundedSurfaces++;
	  if( isUntrimmedSurface(iges,i) )
	    numberOfUntrimmedSurfaces++;
	  
	}
 	else if( iges.entity(i)==IgesReader::finiteElement )
        {
	  numberOfFiniteElements[file]++;
	}
	else if( iges.entity(i)==IgesReader::node )
        {
	  numberOfNodes++;
	}
	else if(  isUntrimmedSurface(iges,i) )
        {
	  if ( !iges.isVisible(i) )
          {
	    numberOfUntrimmedAndInvisible++;
	  }
	  else
          {
	    numberOfUntrimmedAndDependentBSurfs++;
	  }
	}
	else if( iges.entity(i)==IgesReader::boundedSurface )
	{
	  numberOfBoundedSurfaces++;
	}
	else if(iges.entity(i)==IgesReader::boundary )
	{
	  numberOfBoundaryEntities++;
	}
	else if( iges.isVisible(i) && !iges.isIndependent(i) )
	  numberOfVisibleDependent++;

      } // end for i
      totalNumberOfFiniteElements += numberOfFiniteElements[file];
      totalNumberOfNodes += numberOfNodes;
      totalNumberOfNurbss+=numberOfNurbss;
      printf("There were %i surfaces (NURBS, splines...) found in file # %i\n", numberOfNurbss, file );
      printf("There were %i finite elements and %i nodes found in file # %i\n", 
	     numberOfFiniteElements[file], numberOfNodes, file );
    } // end for all files

    printf("\nThere were %i finite elements and %i nodes found in the file(s)\n", 
	   totalNumberOfFiniteElements, totalNumberOfNodes);
    printf("There were %i surfaces found in the file(s)\n",totalNumberOfNurbss);
    printf("There were %i trimmed surfaces and %i untrimmed surfaces\n",numberOfTrimmedSurfaces,
	   numberOfUntrimmedSurfaces);
    if( numberOfUntrimmedAndDependentBSurfs > 0 )
    {
      printf("** INFO: There were %i untrimmed Nurbs in the file that are not listed since they are DEPENDENT.\n"
	     "         Choose the `read dependent surfaces' menu item to see these\n",
	     numberOfUntrimmedAndDependentBSurfs);
    }

    if ( numberOfUntrimmedAndInvisible > 0 )
    {
      printf("** INFO: There were %i untrimmed Nurbs that are not listed since they are INVISIBLE.\n"
	     "         Choose the `read invisible surfaces' menu item to see these\n", 
	     numberOfUntrimmedAndInvisible);
    }

    if( numberOfVisibleDependent>0 )
      printf("** INFO: There were %i visible but dependent items other than untrimmed Nurbs.\n"
	     "         These are not in the list of items.\n",
	     numberOfVisibleDependent);
  

    const int numberOfItems=totalNumberOfNurbss;

    // Make a new menu
    aString menu[]={
      "!Choose surfaces to read",
      "choose all",
      "choose some",
      "choose a list",
      "read dependent surfaces (toggle)",
      "read invisible surfaces (toggle)",
      "break",
      ""
    };

    params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

    initialNumberOfMaps=mapInfo.mappingList.getLength();

    // mapNumber(i) = item ?   i=0,...,numberOfMapsCreated
    mapNumber.redim(initialNumberOfMaps+totalNumberOfNurbss+100);   // + number of trimmed NURBS

    gi.appendToTheDefaultPrompt("Choose items>"); // set the default prompt

    numberOfMapsCreated=0;
    answer="";
    map=0;
    startMap=-1; 
    endMap=totalNumberOfNurbss; // This is wrong *********************************************
    numberInList=0;

    for(;;)
    {
      // check if we need this menu
      if( totalNumberOfNurbss == 0 && numberOfUntrimmedAndDependentBSurfs == 0 &&
	  numberOfUntrimmedAndInvisible == 0 )
      {
	done = TRUE;
	break;
      }
    
      if (chooseAll)
	answer="choose all";
      else
	gi.getMenuItem(menu,answer,"choose");
      
      if( answer=="choose all" )
      {
	startMap=0;
	endMap=totalNumberOfNurbss-1;
	done = TRUE;
	break;
      }
      else if( answer=="read invisible surfaces (toggle)" )
      {
	readInvisibleItems = !readInvisibleItems;
	printf("\nInvisible untrimmed surfaces will %s read\n", (readInvisibleItems? "be" : "NOT be"));
	done = FALSE;
	break; // get back to the listing
      }
      else if( answer=="read dependent surfaces (toggle)" )
      {
	listDependentItems = !listDependentItems;
	printf("\nDependent untrimmed surfaces will %s read\n", (listDependentItems? "be" : "NOT be"));
	done = FALSE;
	break; // get back to the listing
      }
      else if( answer=="break" )
      {
	gi.unAppendTheDefaultPrompt();  // reset the prompt
	return 0;
      }
      else if( answer=="choose some" )
      {
	gi.inputString(line,sPrintF(buff,"Enter start and end, (between 0 and %i, -1=all) ",
				    totalNumberOfNurbss-1));
	if( line!="" )
        {
	  sScanF(line,"%i %i",&startMap,&endMap);
	  if( endMap==-1 )
	    endMap=totalNumberOfNurbss-1;
	}
	endMap=min(endMap,totalNumberOfNurbss);
	startMap=max(0,startMap);
	if (startMap <= endMap )
        {
	  done = TRUE;
	  break;
	}
	else
	  printf("Sorry, you have to pick some surfaces before you can continue\n");
      }
      else if( answer=="choose a list" )
      {
	const int sortInAscendingOrder=+1;
	numberInList = gi.getValues(sPrintF(buff,"Enter mapping numbers (range 0..%i)",
					    totalNumberOfNurbss-1),
				    mapList,0,totalNumberOfNurbss-1,sortInAscendingOrder);

	// printf("numberInList=%i\n",numberInList);
	
	if( numberInList>0 )
        {
	  startMap=mapList(0);
	  endMap=mapList(numberInList-1);
	}

	if (startMap <= endMap && startMap >= 0 && endMap < numberOfItems )
        {
	  done = TRUE;
	  break;
	}
	else
	  printf("Sorry, you have to pick some surfaces before you can continue\n");

      }
      else 
      {
        cout << "Unknown response: [" << answer << "]\n";
	gi.stopReadingCommandFile();
        done=TRUE;
        break;
      }

    }

    gi.unAppendTheDefaultPrompt();  // reset the prompt

  } while (!done);


  // Do the actual reading
  real time0=getCPU();
  timeToBuildTrimmedMappings=0.;
  timeForCreateSurface=0.;
  timeToBuildNurbsSurfaces=0.;
  timeToCopyNurbsCurves=0.;
  timeToCreateCurves=0.;
  timeToBuildNurbsCurves=0.;
  timeToMergeNurbsCurves=0.;
  timeToAdjustCurves=0.;
  timeToBuildCurvesOther=0.;
  
  // printf("&&&&& startMap=%i, endMap=%i, numberInList=%i totalNumberOfEntitiesInFiles=%i \n",startMap,endMap,numberInList,totalNumberOfEntitiesInFiles);

  int visibleItem=-1;
  map=startMap;
  int listNumber=0;
  real cadTolerance=0.;
  // ***********************************************************************************
  // **** Here we loop over all items in all files and build the appropriate items: ****
  // ***********************************************************************************


  for( int fileItem=0; fileItem<totalNumberOfEntitiesInFiles; fileItem++ )
  {
    file = 0;
    int item=fileItem; // item will hold the item number in the particular file
    while( item >= numberOfEntitiesPerFile(file) && file<numberOfIgesFiles )
    {
      item-=numberOfEntitiesPerFile(file);
      file++;
    }
    if( item>=numberOfEntitiesPerFile(file) )
    {
      {throw "error";}
    }
	  
    assert( igesArray[file]!=NULL );
    IgesReader & iges = *igesArray[file];
    assert( igesFilePointer[file]!=NULL );

    cadTolerance=max(cadTolerance,iges.getTolerance());
    
    // FILE *fp = igesFilePointer[file];

    // printf("fileItem=%i, file=%i, item=%i isVisible=%i independent=%i, entity=%i\n",fileItem,file,item,
    //       iges.isVisible(item),iges.isIndependent(item),iges.entity(item));
	  
    const bool listThisItem = (readInvisibleItems || iges.isVisible(item)) && 
      ( listDependentItems || iges.isIndependent(item) );

    if( listThisItem && 
	( (readUntrimmedSurface &&  isUntrimmedSurface(iges,item)) ||
	  (readTrimmedSurface   && iges.entity(item)==IgesReader::trimmedSurface) ||
	  (readBoundedSurface   && iges.entity(item)==IgesReader::boundedSurface) ||
	  iges.entity(item)==IgesReader::tabulatedCylinder ||
	  iges.entity(item)==IgesReader::surfaceOfRevolution ||
          iges.entity(item)==IgesReader::manifoldSolidB_RepObject ) )
    {
      visibleItem++;
      if( numberInList>0 && visibleItem!=map )
	continue;
      else if( visibleItem<startMap )
	continue;
      else if( visibleItem > endMap )
	break;
	  
      mapPointer=NULL;

      if( Mapping::debug & 2 )
	printf("attempting to create item=%i, %s\n",item,(const char*)iges.entityName(iges.entity(item)));



      if( iges.entity(item)!=IgesReader::manifoldSolidB_RepObject )
      {
	// ************* Build a surface entity ***************
	int status = createSurface(item,iges,mapPointer);



	if( mapPointer!=NULL )
	{
	  mapInfo.mappingList.addElement(*mapPointer);      
	  mapPointer->decrementReferenceCount();
	    
	  mapNumber(numberOfMapsCreated++)=visibleItem;
	  if( Mapping::debug & 2 )
	    cout << "*****item  " << numberOfMapsCreated-1 << " created *****\n";
	  else
	    if ( status == 0 )
	      cout << "+";
	    else 
	      cout << "-";

	  if( numberInList>0 )
	  { // if we are reading in a list then choose the next map from the list
	    listNumber++;
	    if( listNumber<numberInList )
	      map=mapList(listNumber);
	    else
	      break;
	  }
	  else
	    map++;

	  //mapInfo.graphXInterface->plot(*mapPointer);
	}
	else
	{
	  if( true) cout<<"\nError reading "<<iges.entityName(iges.entity(item))<<" entity, item : "<<item<<endl;
	  if( status!=0 )
	  {
	    if( Mapping::debug & 2 )
	      cout<<"\nError reading "<<iges.entityName(iges.entity(item))<<" entity, item : "<<item<<endl;
	    else
	      cout<< "e";
	  }
	}
      }
      else
      {
        // *******  build a manifoldSolidB_RepObject **************
        printf(" ***** manifoldSolidB_RepObject found !! **************\n");
	
	createManifoldSolidBRepObject(item,iges, mapInfo,mapNumber,numberOfMapsCreated,visibleItem);

      }

    }
    else
    {
      if ( Mapping::debug & 2 )
	printf("skip item=%i, %s\n",item,(const char*)iges.entityName(iges.entity(item)));

      if( iges.isVisible(item) && (bool)iges.isIndependent(item) &&
	  iges.entity(item)!=IgesReader::transformationMatrix )
      {
	printf("*** readMappings:unknown: fileItem=%i, file=%i, item=%i isVisible=%i "
	       "independent=%i, entity=%i\n",fileItem,file,item,
	       iges.isVisible(item),iges.isIndependent(item),iges.entity(item));

      }
    }
  } // end for( int fileItem=0

  cout<<"finished reading mappings"<<endl;

  // save the surfaces that were created as a CompositeSurface
  int numberOfSurfacesCreated = numberOfMapsCreated;

  //  cout << "Number of mappings in the list before reading the unstructured part: " <<
  //    mapInfo.mappingList.getLength();

  //  cout << "Number of mappings in the list after reading the IGES file: " <<
  //    mapInfo.mappingList.getLength();

  // Generate a CompositeMapping for all NURBS
  if( numberOfSurfacesCreated > 0 )
  {
    printf("\nGenerating a CompositeSurface\n");
    csPointer = new CompositeSurface;
    csPointer->incrementReferenceCount();
      
    CompositeSurface & cs = *csPointer;
    if( fileName[0]=='/' )
    {
      // remove full path name from the fileName  *wdh* 011117
      int i=fileName.length()-1;
      while( fileName[i]!='/' )
      {
	i--;
      }
      fileName=fileName(i+1,fileName.length()-1);
    }
    cs.setName(Mapping::mappingName,fileName+".compositeSurface");
    cs.setTolerance(cadTolerance);

    startMap=0, endMap=numberOfSurfacesCreated;
    printf("adding "); // should only add the nurbs!!!
    for( map=max(0,startMap); map<=min(endMap,numberOfSurfacesCreated-1); map++ )
      {
	cs.add(*mapInfo.mappingList[map+initialNumberOfMaps].mapPointer,mapNumber(map));
	cs.setColour(map,gi.getColourName(map));
      
	printf("%i,",map);
	fflush(stdout);
      }
    printf("\n");
    mapInfo.mappingList.addElement(cs); 
    csPointer->decrementReferenceCount();
  }


  // read any finite elements and put them into unstructuredMappings
  if( totalNumberOfFiniteElements>0 )
  {
    printf("\nGenerating unstructured mappings...\n");
    for( file=0; file<numberOfIgesFiles; file ++ )
    {
      // skip the file if there are no finite elements in it
      if (numberOfFiniteElements[file] == 0)
	continue;
      assert( igesArray[file]!=NULL );
      IgesReader & iges = *igesArray[file];

      readFiniteElements( iges );

      // get the pointer to the last mapping added
      int num = mapInfo.mappingList.getLength();
      if( num>0 )
      {
	mapPointer = &mapInfo.mappingList[num-1].getMapping();
      }

      if( mapPointer!=NULL )
      {
	mapNumber(numberOfMapsCreated++)=visibleItem;
	cout << "*****item  " << numberOfMapsCreated-1 << " created *****\n";

        // call the update function for each finite element mapping	    
	UnstructuredMapping *triPtr = (UnstructuredMapping *) mapPointer;
	UnstructuredMapping & tri = *triPtr;
	
	tri.update(mapInfo);
      }

    } // end for all files
  }

  printf("\n ++++readMappings: time to gen Maps = %8.2e (surf=%8.2e Trim=%8.2e untrim=%8.2e, curves=%8.2e"
         "(build=%8.2e,merge=%8.2e,adjust=%8.2e,copy=%8.2e,other=%8.2e) )\n\n",
	 getCPU()-time0,timeForCreateSurface,timeToBuildTrimmedMappings,timeToBuildNurbsSurfaces,
         timeToCreateCurves,timeToBuildNurbsCurves,timeToMergeNurbsCurves,timeToAdjustCurves,timeToCopyNurbsCurves,
         timeToBuildCurvesOther);
  printf(" merge=%8.2e(addSubCurve=%8.2e,arcLength=%8.2e,el degree=%8.2e,other=%8.2e)\n",
         timeToMergeNurbs,timeToMergeNurbsAddSubCurve,
          timeToMergeNurbsArcLength,timeToMergeNurbsElevateDegree,timeToMergeNurbsOther);
  

// call the compositeSurface update function if a compositeSurface was generated
  if (csPointer && !chooseAll)
  {
    CompositeSurface & cs = *csPointer;
    cs.update(mapInfo);
  }
  
  for( i=0; i<maximumNumberOfIgesFiles; i++ ) // *wdh* 030825
  {
    delete igesArray[i];
  }
  return 0;
}


static int 
adjustTrimmedMappingForNarrows(TrimmedMapping & map, GenericGraphicsInterface *ps = NULL)
{
  TriangleWrapper triwrap;
  UnstructuredMapping umap(2,2, Mapping::parameterSpace, Mapping::parameterSpace);

  TriangleWrapperParameters & triParams = triwrap.getParameters();
  if ( !triParams.getFreezeSegments() )
    triParams.toggleFreezeSegments();  // do not add any new segments to the curves
  triParams.setMinimumAngle(5.);    // make a small angle to reduce the number of vertices added

  //
  // first count the number of vertices ( and edges ) contained in the trimming curves
  //
  const int nSamples = 80; // get a sampling of the curves, please make this a multiple of 4
  realArray xd(nSamples,1), xr(nSamples,2);  // domain and range space sample arrays
  real dXd = 1./real(nSamples-1); // spacing in domain space
  xd.seqAdd(0., dXd);


#ifdef OLDSTUFF
  int nEdges = (nSamples-1)*(1+map.getNumberOfInnerCurves());
  int numberOfVertices = nSamples * ( 1 + map.getNumberOfInnerCurves() );
#else
  int nEdges = (nSamples-1)*(map.getNumberOfTrimCurves());
  int numberOfVertices = nSamples * map.getNumberOfTrimCurves();
#endif

  realArray initial_vertices(numberOfVertices, 2);
  Index S(0, nSamples), AXES(0, 2);
  
  Mapping *outerCurve = map.getOuterCurve();
  if ( outerCurve != NULL ) 
    outerCurve->map(xd,xr);
  else
    {
      real dx = 1./real(nSamples/4 - 1);
      int idx=0;
      for ( int i1=0; i1<nSamples/4; i1++ )
	{
	  xr(idx,0) = i1*dx;
	  xr(idx,1) = 0.0;
	  idx++;
	}

      for ( int i2=0; i2<nSamples/4-1; i2++ )
	{
	  xr(idx,0) = 1.0;
	  xr(idx,1) = (i2+1)*dx;
	  idx++;
	}

      for ( int i3=0; i3<nSamples/4-1; i3++ )
	{
	  xr(idx,0) = 1.0 - (i3+1)*dx;
	  xr(idx,1) = 1.0;
	  idx++;
	}

      for ( int i4=0; i4<nSamples/4-2; i4++ )
	{
	  xr(idx,0) = 0.0;
	  xr(idx,1) = 1.0 - (i4+1)*dx;
	  idx++;
	}
    }

  initial_vertices(S, AXES) = xr(S,AXES);

#ifdef OLDSTUFF
  for ( int innerCurve=0; innerCurve<map.getNumberOfInnerCurves(); innerCurve++ )
#else
  for ( int innerCurve=1; innerCurve<map.getNumberOfTrimCurves()-1; innerCurve++ )
#endif
    {
      Mapping *innerMap = map.getInnerCurve(innerCurve);

      if ( innerMap == NULL )
	{
	  return 1;
	}
      
      innerMap->map(xd, xr);
      initial_vertices(S + (innerCurve+1)*nSamples,AXES) = xr(S,AXES);
    }

  intArray initial_edges(nEdges, 2);
  int edge = 0;
#ifdef OLDSTUFF
  for ( int c=0; c<(map.getNumberOfInnerCurves()+1); c++ )
#else
  for ( int c=0; c<map.getNumberOfTrimCurves(); c++ )
#endif
    {
      // remember that the curves are periodic so do all but the last edge
      // and then create the last edge connecting the end of the curve to the beginning
      for ( int e=0; e<nSamples-2; e++ )
	{
	  initial_edges(edge, 0) = nSamples*c + e;
	  initial_edges(edge, 1) = nSamples*c + e + 1;
	  edge++;
	}
      initial_edges(edge,0) = nSamples*(1+c)-2;
      initial_edges(edge,1) = nSamples*c;
      edge++;
    }

  triwrap.initialize( initial_edges, initial_vertices );
  triwrap.generate();

  umap.setNodesAndConnectivity( triwrap.getPoints(), triwrap.generateElementList(), 2 );

  if ( ps != NULL ) PlotIt::plot(*ps,umap);

  real minLength = 1.0; // remember, this is in the parameter space
  const intArray & faces = umap.getFaces();
  const intArray & faceElements = umap.getFaceElements();
  const realArray & vertices = umap.getNodes();
  real minFloor = 1./(101.*101.);
  for ( int f=0; f<umap.getNumberOfFaces(); f++ )
    {
      if ( faceElements(f,0)!=-1 && faceElements(f,1)!=-1 )
	{
	  minLength = min(minLength, max( minFloor, 
					  sum(pow(vertices(faces(f,0),AXES) -  
						  vertices(faces(f,1),AXES),2))));
	}
    }
  minLength = sqrt(minLength);
  int newGridSize = min(101, max(int(1./minLength), map.getGridDimensions(0), map.getGridDimensions(1)));

  if ( Mapping::debug & 2 )
    cout<<"setting trimmed mapping grid dimensions to : "<<newGridSize<<endl;

  map.setGridDimensions(0, newGridSize);
  map.setGridDimensions(1, newGridSize);
  
  // map.mappingHasChanged();
  // *wdh* 010901  no need to explicitly build quad tree
  // *wdh* 010901 map.initialize();
  // *wdh* 010901 map.createTrimmedSurface();

  return 0;
}

int MappingsFromCAD::
createBoundedSurface(int entity, IgesReader &iges, Mapping *&mapPointer)
// =======================================================================================================
/// \details  Create a trimmed surface from a Bounded Surface IGES entity 143
/// 
/// \param entity (input): the iges entity to read
/// \param iges   (input): the iges reader that is tied to the current file
/// \param mapPointer (output) : Points to the "trimmed" mapping. This will usually be a TrimmedMapping unless the
///     trimmed region can be replaced by a reparameterized NURBS (if the trimmed region is a rectangle)
///     or by a TFIMapping (with 2 or 4 sides specified) if the trimmed region is a single outer curve with 4 sides.
/// \param Returns : 0 on success, nonzero on failure
/// 
///  The data for the bounded surface in the IGES file is
///  \begin{verbatim}
///    data[0] : entity number
///    data[1] : the type of boundary surface representation; 0 = entities ref. model space, 
///                                                           1 = entities reference both model and parameter spaces 
///    data[2] : Pointer to DE of surface to be trimmed 
///    data[3] : Number of boundary entities (141) that make up the boundary
///    data[4] : Pointer to DE of the first boundary entity (141)
///    data[...] : Pointer to DE of the ... boundary entity (141)
///  \end{verbatim}
/// 
/// \param Authors : KKC && WDH 
// =======================================================================================================
{
  realArray data(4); 
  iges.readData(entity, data,4);

  int surfaceRep = int(data(1)); // 0 - boundary entities only reference model space curves
  int untrimmedSurf = int(data(2)); // pointer to DE of the untrimmed entity
  int numberOfBoundaryEntities = int(data(3)); // number of 141 entities that make up the boundary

  if ( Mapping::debug &2 )
    {
      cout<<"surface representation : "<<surfaceRep<<endl;
      cout<<"untrimmed surface entity : "<<untrimmedSurf<<endl;
      cout<<"number of 141s : " <<numberOfBoundaryEntities<<endl;
    }

  if ( surfaceRep!=1 )
    {
      cout<<"ERROR : currently cannot read boundary entities referencing only the model space"<<endl;
      mapPointer=NULL;
      return 1;
    }

  int surface = iges.sequenceToItem(untrimmedSurf);

  
  if ( surface==-1 ) 
    {
      cout<<"ERROR : surface to be trimmed could not be found :"<<surface<<endl;
      mapPointer = NULL;
      return 1;
    }

  NurbsMapping *nurbs=NULL;

  // By default the parameterization of the IGES surface is on [0,1]x[0,1]
  // In some cases the IGES surface is parameterized as [ra,rb]x[sa,sb], these values are
  // saved in the  surfaceParameterScale array: 
  RealArray surfaceParameterScale(2,2),curveParameterScale(2);
  surfaceParameterScale(0,0)=0.;  // ra
  surfaceParameterScale(1,0)=1.;  // rb
  surfaceParameterScale(0,1)=0.;  // sa
  surfaceParameterScale(1,1)=1.;  // sb
  curveParameterScale(0)=0.;      // ra or sa for a curve
  curveParameterScale(1)=1.;      // rb or sb for a curve

  //
  // create the untrimmed surface
  //
  Mapping *nurbsMap = NULL;
  createSurface(surface, iges, nurbsMap, surfaceParameterScale);
  
  if ( nurbsMap->getClassName() == "NurbsMapping" )
    nurbs = ( NurbsMapping * )nurbsMap;
  else
    nurbs = NULL;

  if( nurbs==NULL )
    {
      cout << "createBoundedSurface::ERROR: unable to build the surface to be trimmed: " 
	   << iges.entityName(iges.entity(surface)) << endl;
      mapPointer=NULL;
      return 1;
    }

  if( nurbs->getClassName()=="NurbsMapping" )
    {
      // add more points for trimmed patches with both an inner and outer boundary
      if( numberOfBoundaryEntities >= 2 )
	{ 
	  int num=min(61,numberOfBoundaryEntities*21);
	  nurbs->setGridDimensions(axis1,max(num,nurbs->getGridDimensions(axis1)));
	  nurbs->setGridDimensions(axis2,max(num,nurbs->getGridDimensions(axis2)));
	}
    }

  //
  // now read the boundary entities
  //
  NurbsMapping *boundaryCurve = NULL;
  NurbsMapping **boundaryEntities = new NurbsMapping* [numberOfBoundaryEntities];

  data.redim(4 + numberOfBoundaryEntities );
  iges.readData(entity, data, 4 + numberOfBoundaryEntities);
  int dataOffset = 4;

  real maxBoundingBoxArea = -REAL_MAX;
  int outerCurveNum = -1;
  for ( int bdyEnt=0; bdyEnt<numberOfBoundaryEntities; bdyEnt++ )
    {
      // get the next 141 entity and return it as a Nurbs
      boundaryEntities[bdyEnt] = NULL;
      int next141item = iges.sequenceToItem( int(data(dataOffset + bdyEnt)) );

      createBoundaryEntity(next141item, iges, untrimmedSurf, boundaryEntities[bdyEnt], curveParameterScale);

      if ( boundaryEntities[bdyEnt]==NULL )
	{
	  cout<<"ERROR : could not create boundary entity "<<bdyEnt<<" for surface "<<entity<<endl;
	  for ( int delEnt=0; delEnt<bdyEnt; delEnt++ ) 
	    if ( boundaryEntities[delEnt]->decrementReferenceCount() == 0 ) delete boundaryEntities[delEnt];
	  delete boundaryEntities;
	  return 1;
	}
      else
	{
	  //
	  // try to guess if bdyEnt is the outer trimming curve
	  // the one with the largest bounding box area will be used  as the outer trimming curve.
	  //
	  RealArray box;
	  box = boundaryEntities[bdyEnt]->getBoundingBox();
	  if ( (box(1,0)-box(0,0))*(box(1,1)-box(0,1))  > maxBoundingBoxArea )
	    { // log current guess for the outter trimming curve
	      maxBoundingBoxArea = (box(1,0)-box(0,0))*(box(1,1)-box(0,1));
	      outerCurveNum = bdyEnt;
	    }
	}
	  
    }

  if ( outerCurveNum == -1 )
    {
      mapPointer = NULL;
      cout<<"ERROR : could not create outer boundary entity "<<" for surface "<<entity<<endl;
      for ( int delEnt=0; delEnt<numberOfBoundaryEntities; delEnt++ ) 
	if ( boundaryEntities[delEnt]->decrementReferenceCount() == 0 ) delete boundaryEntities[delEnt];
      delete boundaryEntities;
      return 1;
    }

  NurbsMapping **innerCurves;
  if ( numberOfBoundaryEntities>1 )
    innerCurves =  new NurbsMapping * [numberOfBoundaryEntities-1];
  else
    innerCurves = NULL;

  int ent, i_ent=0;
  for ( ent=0; ent<numberOfBoundaryEntities; ent++ )
    {
      boundaryEntities[ent]->setDomainSpace(Mapping::parameterSpace);
      boundaryEntities[ent]->setRangeSpace(Mapping::parameterSpace);
      boundaryEntities[ent]->parametricCurve((NurbsMapping &)*nurbs);
      if ( ent!=outerCurveNum )
	{
	  innerCurves[i_ent] = boundaryEntities[ent];
	  innerCurves[i_ent]->incrementReferenceCount();
	  i_ent++;
	}
      if ( boundaryEntities[ent]->getClassName()=="NurbsMapping" )
	{
	  for ( int ent_sub=0; ent_sub<((NurbsMapping *)boundaryEntities[ent])->numberOfSubCurves(); ent_sub++ )
	    {
	      Mapping & subcurve = ( (NurbsMapping *)boundaryEntities[ent])->subCurve(ent_sub);
	      if ( subcurve.getClassName()=="NurbsMapping" )
		((NurbsMapping &)subcurve).parametricCurve((NurbsMapping &)*nurbs);
	    }
	}
      //scaleCurve(* boundaryEntities[ent], *nurbs, surfaceParameterScale);
    }

  int curveStatus = 0;
  boundaryCurve = boundaryEntities[outerCurveNum];

  mapPointer = new TrimmedMapping ( (Mapping &) *nurbs, numberOfBoundaryEntities, (Mapping **)boundaryEntities);

  if (curveStatus!=0) ((TrimmedMapping *)mapPointer)->invalidateTrimming(); // should be performed autmatically by constructor

  nurbs->decrementReferenceCount();
  mapPointer->incrementReferenceCount();
  for ( ent=0; ent<numberOfBoundaryEntities; ent++ )
    if ( boundaryEntities[ent]->decrementReferenceCount() == 0 ) delete boundaryEntities[ent];

  delete [] boundaryEntities;

  return 0;

}

int MappingsFromCAD::
readOneCurveAsNURBS(int curve,
		    IgesReader &iges,
		    NurbsMapping *&mapPointer,
		    RealArray & curveParameterScale)
// ===============================================================================================
/// \details 
///      Read a curve from the iges file and place it ina NurbsMapping.
/// \param curve (input): item representing a curve
/// \return  0 for success, 1 if unable to build the curve.
// ===============================================================================================
{
  mapPointer = NULL;

  curveParameterScale(0)=0.;
  curveParameterScale(1)=1.;

  if ( iges.entity(curve)==IgesReader::rationalBSplineCurve ||
       iges.entity(curve)==IgesReader::circularArc )
  {
    mapPointer = new NurbsMapping();  mapPointer->incrementReferenceCount();
    mapPointer->readFromIgesFile(iges,curve);
  }
  else if ( iges.entity(curve)==IgesReader::line )
  {
    // code taken from createCompositeCurve
    RealArray data(7); // need a new array
    iges.readData(curve,data,7);
      
    real x0=data(1), y0=data(2), z0=data(3);
    real x1=data(4), y1=data(5), z1=data(6);

    if( z0!=0. || z1!=0. )
      cout << "****** z values for parametric line are not zero!! *******\n";

    mapPointer = new NurbsMapping; mapPointer->incrementReferenceCount();

    RealArray p1(1,2), p2(1,2);
    p1(0,0) = x0;
    p1(0,1) = y0;
    p2(0,0) = x1;
    p2(0,1) = y1;
    ((NurbsMapping*)mapPointer)->line(p1,p2);

    if( Mapping::debug & 4 )
      cout << "Nurbs for line has " << mapPointer->getGridDimensions(axis1) << " points \n";
  }

  if ( mapPointer == NULL )
  {
    cout << "ERROR : could not read curve "<<curve<<" as a Nurbs"<<endl;
    return 1;
  }

  curveParameterScale(0) = ((NurbsMapping*)mapPointer)->getOriginalDomainBound(0,0);
  curveParameterScale(1) = ((NurbsMapping*)mapPointer)->getOriginalDomainBound(1,0);

  return 0;
}

int MappingsFromCAD::
createBoundaryEntity(const int & item, 
		     IgesReader & iges, 
		     const int & untrimmedSurfSeq,
		     NurbsMapping *&mapPointer,
		     RealArray & curveParameterScale)
// ===============================================================================================
/// \details 
///      Read a Boundary Entity (iges 141) from a file
/// \param item (input): item representing a curve
/// \param iges (input): iges reader containing the entity
/// \param untrimmedSurfSeq (input): sequence number of the expected untrimmed surface entity (sanity check)
/// \param mapPointer (output) : pointer to a NurbsMapping containing the resulting curve, NULL if a problem occurred
/// \return  0 for success, 1 if unable to build the curve.
/// 
/// \param Remarks :
/// \begin{verbatim}
///  The parameter data in a boundary entity (141) is :
///  + Boundary Curve Entity Number (141)
///  + type of the boundary surface representaion
///  + preferred representation of the boundary surface
///  + pointer to the untrimmed surface
///  + number of model space curves comprising this boundary entity
///  + de of model space curve 1
///     - sense for model space curve 1 
///     - number of associated parameter space curves for model space curve 1
///         o parameter space curve
///         o parameter space curve
///  + de of model space curve 2
///     - sense for model space curve 2
///     - number of associated parameter space curves for model space curve 2
///         o parameter space curve
///         o parameter space curve
///         o parameter space curve
///  + de of model space curve ...
///     - ...
//\end{ReadMappingsInclude.tex}
// ===============================================================================================
{

  realArray data(5);
  iges.readData( item, data, 5 );

  int surfaceRep = int(data(1)); // 0 - model space; 1 - model and parametric spaces
  int prefRep    = int(data(2)); // preferred representation in the sending system ( ignored for now )
  int surface    = int(data(3)); // pointer to de of untrimmed surface
  int numberOfCurves = int(data(4)); // number of sub curves in this boundary

  if ( Mapping::debug & 2 )
    cout <<"processing "<<surfaceRep<<" "<<prefRep<<" "<<surface<<" "<<numberOfCurves<<endl;

  if ( surfaceRep == 0 )
    {
      cout<<"ERROR : currently cannot read boundary entities referencing only the model space"<<endl;
      mapPointer=NULL;
      return 1;
    }

  if ( surface != untrimmedSurfSeq )
    {
      cout<<"ERROR : surface entity "<<surface<<" does not match expected entity "<<untrimmedSurfSeq<<endl;
      mapPointer = NULL;
      return 1;
    }

  // boundary curves are composed of multiple curves which may be composed of mutliple parameter space curves :
  // Boundary Curve (141)
  //     + sub curve 1
  //        - parameterized sub curve 
  //        - parameterized sub curve 
  //     + sub curve 2
  //        - parameterized sub curve 
  //        - parameterized sub curve
  //        - parameterized sub curve
  //     + ...
  
  int curveHeaderSize = 3;
  int currentDataSize = 5;
  
  for ( int bdyCurve=0; bdyCurve<numberOfCurves; bdyCurve++ )
    {
      currentDataSize += curveHeaderSize;
      data.resize(currentDataSize);

      //
      // read initial curve data for curve bdyCurve
      //
      iges.readData(item, data, currentDataSize);
      
      int modelSpaceCurve = int(data(currentDataSize-3));
      int sense = int(data(currentDataSize-2));  // 1 - do not reverse direction, 2 - reverse direction
      int numberOfParameterSpaceCurves = int(data(currentDataSize-1));

      if ( Mapping::debug & 2 )
	cout<<"createBoundaryEntity: processing bdy curve "<<modelSpaceCurve<<" " <<sense<<" "<<numberOfParameterSpaceCurves<<endl;
      
      //
      // now we know how many parameter space curves are in this segment of the boundary curve
      // resize data accordingly and read the parameter curves
      // 
      currentDataSize += numberOfParameterSpaceCurves;
      data.resize(currentDataSize);
      iges.readData(item, data, currentDataSize);
      
      // loop through the parameter space curves, appending them to the current boundary entity curve
      NurbsMapping *currentCurve = NULL;
      NurbsMapping *nextArc = NULL;
      for ( int c=0; c<numberOfParameterSpaceCurves; c++ )
	{
	  int curveItem = iges.sequenceToItem(int(data(currentDataSize-numberOfParameterSpaceCurves+c)));
	  readOneCurveAsNURBS(curveItem, iges, nextArc, curveParameterScale);
	  if ( nextArc == NULL )
	    {
	      cout<<"ERROR : could not read parameter space curve "<<curveItem<<endl;
	      mapPointer = NULL;
	      return 1;
	    }
	  else if ( currentCurve == NULL )
	    currentCurve = nextArc;
	  else
	    {
	      currentCurve->truncateToDomainBounds();
//	      int merge = currentCurve->forcedMerge(*nextArc);
	      bool attemptPeriodic = (c==(numberOfParameterSpaceCurves-1) && bdyCurve==(numberOfCurves-1) );
	      int merge = currentCurve->merge(*nextArc, true, -1, attemptPeriodic);
	      if( merge!=0 )
		{
		  printf("createBoundaryEntity::ERROR unable to merge parameter space boundary curves! "
			 " curve=%i out of numberOfCurves=%i \n",c,numberOfParameterSpaceCurves);
		  return 1;
		}
	      if ( nextArc->decrementReferenceCount()==0 ) delete nextArc;
	    }
	} // parameter curves

      if ( currentCurve!=NULL) 
	if ( sense == 2 ) currentCurve->reparameterize(1.,0.);

      if ( currentCurve==NULL )
	{
	  cout<<"ERROR : could not create a curve for boundary entity "<<item<<endl;
	  mapPointer = NULL;
	  return 1;
	}
      else if (mapPointer==NULL)
	mapPointer = currentCurve;
      else
	{
	  mapPointer->truncateToDomainBounds();
//	  int merge = mapPointer->forcedMerge(*currentCurve);
	  bool attemptPeriodic = (bdyCurve==(numberOfCurves-1));
	  int merge = mapPointer->merge(*currentCurve, true,-1,attemptPeriodic);
	  if( merge!=0 )
	    {
	      printf("createBoundarEntity::ERROR unable to merge boundary curves! "
		     " curve=%i out of numberOfCurves=%i \n",bdyCurve,numberOfCurves);
	      return 1;
	    }
	  if ( currentCurve->decrementReferenceCount()==0 ) delete currentCurve;
	}
      
    } // model curves

  mapPointer->incrementReferenceCount();

  return 0;
}

