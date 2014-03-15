#include "StretchTransform.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "arrayGetIndex.h"
#include "DataPointMapping.h"
#include "GL_GraphicsInterface.h"
#include "ParallelUtility.h"

// Declare and define base and bounds, perform loop
#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base=I1.getBase(), I2Base=I2.getBase(), I3Base=I3.getBase();\
  int I1Bound=I1.getBound(), I2Bound=I2.getBound(), I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

// Perform loop
#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(), I2Base=I2.getBase(), I3Base=I3.getBase();\
  I1Bound=I1.getBound(), I2Bound=I2.getBound(), I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

// static inline 
// double
// tetVolume6(real *p1, real*p2, real *p3, real *p4 )
// {
//   // Rteurn 6 times the volume of the tetrahedra
//   // (p2-p1)x(p3-p1) points in the direction of p4 ( p1,p2,p3 are counter clockwise viewed from p4 )
//   // 6 vol = (p4-p1) . ( (p2-p1)x(p3-p1) )
//   return  ( (p4[0]-p1[0])*( (p2[1]-p1[1])*(p3[2]-p1[2]) - (p2[2]-p1[2])*(p3[1]-p1[1]) ) -
// 	    (p4[1]-p1[1])*( (p2[0]-p1[0])*(p3[2]-p1[2]) - (p2[2]-p1[2])*(p3[0]-p1[0]) ) +
// 	    (p4[2]-p1[2])*( (p2[0]-p1[0])*(p3[1]-p1[1]) - (p2[1]-p1[1])*(p3[0]-p1[0]) ) ) ;
	  
// }

// static inline 
// real
// hexVolume( real *v000, real *v100, real *v010, real *v110, real *v001, real *v101, 
//            real *v011, real *v111 )
// // =====================================================================================================
// // Return true if the hex defined by the vertices v000,v100,... has any tetrahedra that are negative.
// // =====================================================================================================
// {
//   return (tetVolume6(v000,v100,v010, v001)+
// 	  tetVolume6(v110,v010,v100, v111)+
// 	  tetVolume6(v101,v001,v111, v100)+
// 	  tetVolume6(v011,v111,v001, v010)+
// 	  tetVolume6(v100,v010,v001, v111));
// }


static int
getBoundaryGridSpacing( Mapping & map, const int direction, const real rp, RealArray & spacings )
// ==================================================================================================
// /Description:
//    Determine the min,ave,max grid spacings near a position parameter location 
// /direction,rp (input) : determine the spacing near r[direction]==rp
// /spacings (output) : spacings(0:2) = min,ave,max grid spacings
// ==================================================================================================
{
  spacings.redim(3);
  spacings=0.;

  // (we could use getGridSerial here -- BUT this is sometimes the entire grid)
  const realArray & xd = map.getGrid(); // grids points 
  OV_GET_SERIAL_ARRAY_CONST(real,xd,x); // x = grid points local to this processor

  real dsMin=REAL_MAX,dsAve=0.,dsMax=0.;

  int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
  isv[0]=isv[1]=isv[2]=0;
  isv[direction]=1;
      
  const IntegerArray gid(2,3); 
  int axis;
  for( axis=0; axis<3; axis++ )
  {
    gid(0,axis)=xd.getBase(axis);
    gid(1,axis)=xd.getBound(axis);
  }

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  ::getIndex(gid,I1,I2,I3);  // do all points

  const real dr=1./max(1,Iv[direction].getBound()-Iv[direction].getBase());
  const int ip = int( rp/dr+.5); // closest grid point
  Iv[direction]=max(Iv[direction].getBase(),min(Iv[direction].getBound()-1,ip ));

  int includeGhost=0;
  bool ok=ParallelUtility::getLocalArrayBounds(xd,x, I1,I2,I3,includeGhost);

  int i1,i2,i3;
  real numDs=0.;
  const int domainDimension=map.getDomainDimension();
  const int rangeDimension=map.getRangeDimension();
  if( ok )
  {
    if( rangeDimension==1 )
    {
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	real ds= sqrt( SQR(x(i1+is1,i2,i3,0)-x(i1,i2,i3,0)) );
	dsMin=min(dsMin,ds);
	dsMax=max(dsMax,ds);
	dsAve+=ds;
	numDs++;
      }
    }
    else if( rangeDimension==2 )
    {
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	real ds= sqrt( SQR(x(i1+is1,i2+is2,i3,0)-x(i1,i2,i3,0))+
		       SQR(x(i1+is1,i2+is2,i3,1)-x(i1,i2,i3,1)) );
	dsMin=min(dsMin,ds);
	dsMax=max(dsMax,ds);
	dsAve+=ds;
	numDs++;
      }
    }
    else
    {
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	real ds= sqrt( SQR(x(i1+is1,i2+is2,i3+is3,0)-x(i1,i2,i3,0))+
		       SQR(x(i1+is1,i2+is2,i3+is3,1)-x(i1,i2,i3,1))+   
		       SQR(x(i1+is1,i2+is2,i3+is3,2)-x(i1,i2,i3,2)) );
	dsMin=min(dsMin,ds);
	dsMax=max(dsMax,ds);
	dsAve+=ds;
	numDs++;
      }
    }
  }
  
  dsMin = ParallelUtility::getMinValue(dsMin);
  dsMax = ParallelUtility::getMaxValue(dsMax);
  dsAve = ParallelUtility::getSum(dsAve);
  numDs = ParallelUtility::getSum(numDs);

  dsAve/=max(1,numDs);

  spacings(0)=dsMin;
  spacings(1)=dsAve;
  spacings(2)=dsMax;

  return 0;
}

int StretchTransform::
buildStretchingParametersDialog(DialogData & dialog, 
                                const StretchMapping::StretchingType stretchType,
                                const int direction )
// ==========================================================================================
// /Description:
//     Build a dialog for specifying stretching parameters.
//
// stretchType (input) : build a dialog for parameters of this stretching type.
// ==========================================================================================
{

  const aString prefix="STP:";
  aString line;

  aString prefix2;
  if( stretchType==StretchMapping::inverseHyperbolicTangent )
    sPrintF(prefix2,"stretch r%i itanh: ",direction+1);
  else if( stretchType==StretchMapping::hyperbolicTangent )
    sPrintF(prefix2,"stretch r%i tanh: ",direction+1);
  else if( stretchType==StretchMapping::exponential )
    sPrintF(prefix2,"stretch r%i exp: ",direction+1);
  else if( stretchType==StretchMapping::exponentialBlend )
    sPrintF(prefix2,"stretch r%i expb: ",direction+1);
  else if( stretchType==StretchMapping::exponentialToLinear )
    sPrintF(prefix2,"stretch r%i expl: ",direction+1);
  else 
  {
    printF("buildStretchingParametersDialog:ERROR:Unknown stretchType=%i\n",stretchType);
    return 1;
  }

  const int maximumCommands=30;
  aString cmdWithPrefix[maximumCommands];

  // dialog.deleteInfoLabels();  // remove any existing labels
  dialog.addInfoLabel(prefix2(11,prefix2.length()-1-2)+" stretching");

  if( stretchType==StretchMapping::inverseHyperbolicTangent )
  {
    dialog.addInfoLabel("layer parameters:");
    dialog.addInfoLabel("interval parameters:");
  }
  else if( stretchType==StretchMapping::hyperbolicTangent )
  {
    dialog.addInfoLabel("parameters:");
  }
  else if( stretchType==StretchMapping::exponential ||
           stretchType==StretchMapping::exponentialToLinear )
  {
    dialog.addInfoLabel("parameters:");
  }
  else if( stretchType==StretchMapping::exponentialBlend )
  {
  }
  else
  {
  }

  if( stretchType==StretchMapping::exponential )
  {
    aString opLabel[] = {"r=0",
			 "r=1",
			 ""}; //
    GUIState::addPrefix(opLabel,prefix+prefix2+sPrintF(line,"cluster at ",direction+1),
                                    cmdWithPrefix,maximumCommands);

    int side = 0; // ipar[direction](0,0);
    dialog.addOptionMenu("cluster points at:", cmdWithPrefix,opLabel,(int)side);
  }



  aString pbLabels[] = {"reset parameters",
  			""};

  int numRows=3;
  GUIState::addPrefix(pbLabels,prefix+prefix2,cmdWithPrefix,maximumCommands);
  dialog.setPushButtons( cmdWithPrefix, pbLabels, numRows ); 
//   dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

//    aString tbCommands[] = { // "determine parameters from grid spacing",
//                            "normalize",
//   			  ""};

//    bool determineStretchingParametersFromGridSpacing=false;  // add to class
//    bool normalize=true;                           

//    int tbState[10];
//    tbState[0] = determineStretchingParametersFromGridSpacing;
//    tbState[1] = normalize;
//    int numColumns=1;
//    GUIState::addPrefix(tbCommands,prefix+prefix2,cmdWithPrefix,maximumCommands);
//    dialog.setToggleButtons(cmdWithPrefix, tbCommands, tbState, numColumns); 

  const int numberOfTextStrings=20;
  aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

  int nt=0;
  if( stretchType==StretchMapping::inverseHyperbolicTangent )
  {
    textLabels[nt]="position and min dx";
    sPrintF(textStrings[nt], "%.2g %.2g",0.,.1); nt++; 


    const int numberToDisplay=1;
    int k;
    for( k=0; k<numberToDisplay; k++ )
    {
      sPrintF(textLabels[nt],"layer",direction+1);
      sPrintF(textStrings[nt], "%i %.2g %.2g %.2g (id>=0,weight,exponent,position)", -1, 1.,10.,0.); 
      nt++;
    }
    for( k=0; k<numberToDisplay; k++ )
    {
      sPrintF(textLabels[nt],"interval",direction+1);
      sPrintF(textStrings[nt], "%i %.2g %.2g %.2g (id>=0,weight,exponent,start position)",-1, 1.,10.,0.); 
      nt++;
    }
    sPrintF(textLabels[nt],"interval end",direction+1);
    sPrintF(textStrings[nt], "%.2g (end position)",1.5); nt++;  
  
   
//      textLabels[nt]="maximum spacing"; 
//      sPrintF(textStrings[nt], "%.2g",.1); nt++;
  }
  else if( stretchType==StretchMapping::hyperbolicTangent )
  {
    // Note: fraction of points in layer is determined by "b"
     real a0=0., ar=1., b=2., a=-.9*ar/b, c=.5;   // choose a0 > a1*b1 to be invertible

     real ratio=10.;
     a = (1./ratio - ar)/b;
  
    textLabels[nt]="position and min dx";
    sPrintF(textStrings[nt], "%.2g %.2g",c,1./ratio); nt++; 

//       sPrintF(textLabels[nt],"linear",direction+1);
//       sPrintF(textStrings[nt], "%.2g %.2g (a0,ar)",0.,1.); nt++;

//       sPrintF(textLabels[nt],"tanh ",direction);
//       sPrintF(textStrings[nt], "%.2g %.2g %.2g (weight,exponent,position)",1.,10.,0.); nt++;

     sPrintF(textLabels[nt],"parameters",direction+1);
     sPrintF(textStrings[nt], "%.2g %.2g %.2g %.2g %.2g (a0,ar,a,b,c)",a0,ar,a,b,c); nt++;

  }
  else if( stretchType==StretchMapping::exponential )
  {
     real dxMin=-1.;
     sPrintF(textLabels[nt],"min dx",direction+1);
     sPrintF(textStrings[nt], "%.2g",dxMin); nt++;

     real a0=0., ar=1., a=1., b=5., c=.5;  
//       sPrintF(textLabels[nt],"exponent",direction+1);
//       sPrintF(textStrings[nt], "%.2g",b); nt++;

     sPrintF(textLabels[nt],"parameters",direction+1);
     sPrintF(textStrings[nt], "%.2g %.2g %.2g %.2g %.2g (a0,ar,a,b,c)",a0,ar,a,b,c); nt++;

  }
  else if( stretchType==StretchMapping::exponentialBlend )
  {
    // currently there are no parameters to set here
  }
  else if( stretchType==StretchMapping::exponentialToLinear )
  {
    real a=1.e-2, b=5., c=0., weight=10.;  

    real dxMin=-1., dxMax=-1.;
    sPrintF(textLabels[nt],"min dx, max dx");
    sPrintF(textStrings[nt], "%.2g %.2g",dxMin,dxMax); nt++;

    sPrintF(textLabels[nt],"position");
    sPrintF(textStrings[nt], "%.2g",c); nt++;

    sPrintF(textLabels[nt],"linear weight");
    sPrintF(textStrings[nt], "%.2g",weight); nt++;

    sPrintF(textLabels[nt],"parameters");
    sPrintF(textStrings[nt], "%.2g %.2g %.2g (a,b,c)",a,b,c); nt++;
  }
  else 
  {
    printF("buildStretchingParametersDialog:ERROR:Unknown stretchType=%i\n",stretchType);
  }

  // null strings terminal list
  assert( nt<numberOfTextStrings );
  textLabels[nt]="";   textStrings[nt]="";  
  // addPrefix(textLabels,prefix,cmd,maxCommands);
  GUIState::addPrefix(textLabels,prefix+prefix2,cmdWithPrefix,maximumCommands);
  dialog.setTextBoxes(cmdWithPrefix, textLabels, textStrings);

//    if( stretchType==StretchMapping::inverseHyperbolicTangent )
//    {
//      dialog.setSensitive(false,DialogData::textBoxWidget,"stretch minimum spacing");
//      dialog.setSensitive(false,DialogData::textBoxWidget,"stretch maximum spacing");
//    }
  
  return 0;
}

int StretchTransform::
resizeParameterArrays( int stretchID, IntegerArray & ipar, RealArray & rpar )
{
  if( stretchID<0 || stretchID>100 )
  {
    printF("StretchTransform::ERROR: invalid value for stretchID=%i\n",stretchID);
    return 1;
  }
    
  if( stretchID>rpar.getBound(1) )
  {
    int num=rpar.getLength(1);
    rpar.resize(rpar.getLength(0),stretchID+10);
    ipar.resize(ipar.getLength(0),stretchID+10);
    Range all;
    rpar(all,Range(num,rpar.getBound(1)))=0.;
    ipar(all,Range(num,ipar.getBound(1)))=-1;
  }
  return 0;
}

int StretchTransform::
updateStretchingParameters(aString & answer_, IntegerArray *ipar, RealArray *rpar,
                           int stretchingType[3], int numberOfMultigridLevels,
                           real stretchResolutionFactor,
                           DialogData *stretchParametersDialog[3],
                           DialogData & dialog, MappingInformation & mapInfo )
//================================================================================================
// /Description:
//    Look for an answer that changes the stretching parameters.
// /Return value: return true if the answer was processed.
//================================================================================================
{
  const aString prefix="STP:";

  aString answer=answer_;
  aString line;
  
  // take off the prefix
  if( answer(0,prefix.length()-1)==prefix )
    answer=answer(prefix.length(),answer.length()-1);
  else
  {
    return 0;
  }

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  int len;
  int direction=-1;
  if( answer.matches("stretch r1 ") ||
      answer.matches("stretch r2 ") ||
      answer.matches("stretch r3 ") )
  {
    if( len=answer.matches("stretch r1 ") )
      direction=0;
    else if( len=answer.matches("stretch r2 ") )
      direction=1;
    else if( len=answer.matches("stretch r3 ") )
      direction=2;

  }
  if( direction<0 )
  {
    printF("updateStretchingParameters:ERROR: answer=[%s]\n",(const char*) answer);
    return 0;
  }
  answer=answer(len,answer.length()-1); // remove the prefix

  if( len=answer.matches("itanh: layer") )
  {
    int id=-1;
    real a=1., b=10., c=0.;
    sScanF(&answer[len],"%i %e %e %e",&id,&a,&b,&c);

    resizeParameterArrays( id,ipar[direction],rpar[direction] );
    
    ipar[direction](0,id)=0;   // layer
    rpar[direction](0,id)=a;
    rpar[direction](1,id)=b;
    rpar[direction](2,id)=c;

    stretchParametersDialog[direction]->setTextLabel("layer",
               sPrintF(line,"%i %.2g %.2g %.2g (id>=0,weight,exponent,position)",id,a,b,c));
  }
  else if( len=answer.matches("itanh: interval") )
  {
    int id=-1;
    real d=1., e=10., f=0.;
    sScanF(&answer[len],"%i %e %e %e",&id,&d,&e,&f);


    resizeParameterArrays( id,ipar[direction],rpar[direction] );
    
    ipar[direction](0,id)=1;   // interval

    rpar[direction](0,id)=d;
    rpar[direction](1,id)=e;
    rpar[direction](2,id)=f;

    stretchParametersDialog[direction]->setTextLabel("interval",
               sPrintF(line,"%i %.2g %.2g %.2g (id>=0,weight,exponent,position)",id,d,e,f));
  }
  else if( len=answer.matches("itanh: position and min dx") )
  {
    printF("INFO: itanh: position and min dx: This option currently only works with one layer\n");
    int id=0;

    real c, dx=.1;
    sScanF(&answer[len],"%e %e",&c,&dx);

    real a=rpar[direction](0,id), b=rpar[direction](1,id);
    

    a=1.;  // **************** do this for now *****************
    b=10.;
    
    Range all;
    ipar[direction](0,all)=0;  // reset any other parameters
    rpar[direction](0,all)=0.;

    rpar[direction](0,id)=a;
    rpar[direction](1,id)=b;
    rpar[direction](2,id)=c;

    // determine b so that spacing is dxMin

    StretchMapping & stretch = stretchedSquare->stretchFunction(direction);

    int numberOfLayers=1;
    stretch.setNumberOfLayers( numberOfLayers );
    stretch.setStretchingType(StretchMapping::inverseHyperbolicTangent);
    stretch.setLayerParameters(id, a,b,c );
    stretch.setIsPeriodic((bool)map2.getIsPeriodic(direction));
    

    // ****************** finish this ********************
    // Solve: 
    //   f(b) = 1./ratio 
    //   where f = [S(c+dr)-S(c)]/dr

    const real dr = 1./(max(1,getGridDimensions(direction)-1));
    const real ra=c<.9 ? c : c-dr; 
    const real rb=ra+dr; 


    // determine the grid spacing on the unstretched grid
    RealArray spacings;
    getBoundaryGridSpacing( map2.getMapping(),direction,c,spacings );
    real dxOld=spacings(1); // average spacing
    
    real ratio=dxOld/dx;
    
    if( ratio<=1. )
    {
      printF("ERROR: The new grid spacing dx=%8.2e must be less than the unstretched grid spacing = %8.2e\n",
	     dx,dxOld);
      return 1;
    }

    real bMin=1.;
    real bMax=ratio*10.;

    real fMin,fMax;

    realArray r1(1,1), r2(1,1), s1(1,1), s2(1,1);
    r1=ra;
    r2=rb;

#define EVAL_STRETCH(b,f) \
    stretch.setLayerParameters(id, a,b,c ); \
    stretch.map(r1,s1); \
    stretch.map(r2,s2); \
    f=(s2(0,0)-s1(0,0))/dr-1./ratio;
    
    
    EVAL_STRETCH(bMin,fMin)
    EVAL_STRETCH(bMax,fMax)
    
    printF(" dxOld=%8.2e ratio=%8.2e ra=%8.2e rb=%8.2e bMin=%8.2e bMax=%8.2e fMin=%8.2e, fMax=%8.2e\n",dxOld,ratio,
                  ra,rb,bMin,bMax,fMin,fMax);
    
    if( fMin*fMax >0. )
    {
      printF("itanh: position and min dx:ERROR computing the exponent! fMin*fMax >0.\n");
      return 1;
    }

    int it=0, maxIt=20;
    const real tol=.001;
    while( bMax-bMin>tol && it<maxIt )
    {
      real bMid=.5*(bMax+bMin);
      real fMid;

      EVAL_STRETCH(bMid,fMid)

      if( fMid*fMax<=0. )
      {
	fMin=fMid;
	bMin=bMid;
      }
      else
      {
	fMax=fMid;
	bMax=bMid;
      }
      printF(" it=%i : (b,f) : min=(%8.2e,%8.2e) max=(%8.2e,%8.2e)\n",it,bMin,fMin,bMax,fMax);
      it++;
    }
    b=.5*(bMin+bMax);

    printF(" Setting layer parameters a=%8.2e, b=%8.2e, c=%8.2e to achieve dxMin=%8.2e\n",
	   a,b,c,dx);

    ipar[direction](0,id)=0;   // layer
    rpar[direction](0,id)=a;
    rpar[direction](1,id)=b;
    rpar[direction](2,id)=c;

    stretchParametersDialog[direction]->setTextLabel("position and min dx",sPrintF(line,"%.2g %.2g",c,dx));
  }
  else if( len=answer.matches("tanh: position and min dx") )
  {
    // Note: the fraction of points in the layer is determined by "b" !
    //       b=2 seems to give about 1/2 points in the layer

    real a0=0., ar=1., b=2., a=-.9*ar/b, c=.5;   // choose a0 > a1*b1 to be invertible
    real dx=.01;
    sScanF(&answer[len],"%e %e",&c,&dx);

    if( c<.01 || c>.99 )
      b=1.;

    // determine the grid spacing on the unstretched grid
    RealArray spacings;
    getBoundaryGridSpacing( map2.getMapping(),direction,c,spacings );
    real dxOld=spacings(1); // average spacing
    
    real ratio=dxOld/dx;
    real g=1./ratio;
    const real dr = 1./(max(1,getGridDimensions(direction)-1));
    
    a=(g-1.)*ar/(tanh(b*dr)/dr-g*(tanh(b*(1.-c))-tanh(-b*c)));
    

    printF("New tanh parameters: a0=%8.2e ar=%8.2e a=%8.2e b=%8.2e c=%8.2e \n",a0,ar,a,b,c);
     
    rpar[direction](0,0)=a0;
    rpar[direction](1,0)=ar;
    rpar[direction](2,0)=a;
    rpar[direction](3,0)=b;
    rpar[direction](4,0)=c;
     
    stretchParametersDialog[direction]->setTextLabel("position and min dx",sPrintF(line,"%.2g %.2g",c,dx));
  }
  else if( len=answer.matches("tanh: parameters") )
  {
    real a0=0., ar=1., b=5., a=-.9*ar/b, c=.5;   // choose a0 > a1*b1 to be invertible
    sScanF(&answer[len],"%e %e %e %e %e",&a0,&ar,&a,&b,&c);
    printF("tanh parameters: a0=%8.2e ar=%8.2e a=%8.2e b=%8.2e c=%8.2e \n",a0,ar,a,b,c);
    
    rpar[direction](0,0)=a0;
    rpar[direction](1,0)=ar;
    rpar[direction](2,0)=a;
    rpar[direction](3,0)=b;
    rpar[direction](4,0)=c;

    stretchParametersDialog[direction]->setTextLabel("parameters",
                                   sPrintF(line,"%.2g %.2g %.2g %.2g %.2g (a0,ar,a,b,c)",a0,ar,a,b,c));
    
  }
//    else if( len=answer.matches("tanh") )
//    {
//      if( answer(len,len+5)=="linear" )
//      {
//        real a0=0., ar=1.;
//        sScanF(&answer[len],"%e %e",&a0,&ar);
//        rpar[direction](0,0)=a0;
//        rpar[direction](1,0)=ar;
//      }
//      else
//      {
//        real a=1., b=10., c=0.;
//        sScanF(&answer[len],"%e %e %e",&a,&b,&c);
//        rpar[direction](2,0)=a;
//        rpar[direction](3,0)=b;
//        rpar[direction](4,0)=c;
//      }
    
//    }
  else if( len=answer.matches("exp: cluster at ") )
  {
    if( answer(len,len+2)=="r=0" )
    {
      ipar[direction](0,0)=0;
    }
    else if( answer(len,len+2)=="r=1" )
    {
      ipar[direction](0,0)=1;
    }
    else
    {
      printF("ERROR: answer==[%s] answer(len,len+2)=[%s] is not `r=0' nor 'r=1'\n",
	     (const char*)answer,(const char*)answer(len,len+2));
      gi.stopReadingCommandFile();
    }
    stretchParametersDialog[direction]->getOptionMenu("cluster points at:").setCurrentChoice(ipar[direction](0,0));

  }
  else if( len=answer.matches("exp: exponent") )
  {
    real b;
    sScanF(&answer[len],"%e",&b);
    rpar[direction](3,0)=b;

    stretchParametersDialog[direction]->setTextLabel("exponent",sPrintF(line,"%.2g",b));
  }
  else if( len=answer.matches("exp: parameters") )
  {
    real a0=0., ar=1.;
    real a=1., b=10., c=0.;
    sScanF(&answer[len],"%e %e %e %e %e",&a0,&ar,&a,&b,&c);
    rpar[direction](0,0)=a0;
    rpar[direction](1,0)=ar;
    rpar[direction](2,0)=a;
    rpar[direction](3,0)=b;
    rpar[direction](4,0)=c;
    
    stretchParametersDialog[direction]->setTextLabel("parameters",
                                   sPrintF(line,"%.2g %.2g %.2g %.2g %.2g (a0,ar,a,b,c)",a0,ar,a,b,c));
  }
  else if( len=answer.matches("exp: min dx") )
  {
    real dx=.1;
    sScanF(&answer[len],"%e",&dx);

    // determine the grid spacing on the unstretched grid
    RealArray spacings;
    int side=ipar[direction](0,0);
    if( side!=0 && side!=1 )
    {
      printF("ERROR: side=%i for an exponential stretching, direction=%i\n",side,direction);
      side=0;
    }
    
    getBoundaryGridSpacing( map2.getMapping(),direction,real(side),spacings );
    real dxOld=spacings(1); // average spacing
    
    real ratio=dxOld/dx;
    
    if( ratio<=1. )
    {
      printF("ERROR: The new grid spacing dx=%8.2e must be less than the unstrteched grid spacing = %8.2e\n",
	     dx,dxOld);
      return 1;
    }
    // approx: b + log(b) = log(ratio)

    real b = log(ratio) - log(log(ratio));
    printF(" Guess for b=%8.2e\n",b);
    
    // we should solve

    //   1./ratio = S'(b) 
    //   where S' = (ar + a*b*exp(-b*c) )/( ar+a*exp(b*(1-c))-a*exp(-b*c) )

    const real dr = 1./(max(1,getGridDimensions(direction)-1));
    const real ra=real(side);
    const real rb=ra+dr*(1-2*side);

#define ALPHA(b) ( ar+a*exp((b)*(1.-c))-a*exp(-(b)*c) )
// define SPRIME(b) (ar + a*(b)*exp((b)*(rp-c)) )/( ar+a*exp((b)*(1.-c))-a*exp(-(b)*c) ) - 1./ratio
// SPRIME:use discrete version to get spacing more accurately [S(r+dr)-S(r)]/dr
#define SPRIME(b) (ar + a*(exp((b)*(rb-c))-exp((b)*(ra-c)))/dr )/( ar+a*exp((b)*(1.-c))-a*exp(-(b)*c) ) - 1./ratio

    real ar=rpar[direction](1,0), a=rpar[direction](2,0), c=rpar[direction](4,0);

    real bMin=.75;
    real bMax=10.;
    real fMin=SPRIME(bMin);
    real fMax=SPRIME(bMax);
    real alphaMax= ALPHA(bMax);
    
    printF(" a*exp((b)*(1.-c))=%8.2e\n",a*exp((bMax)*(1.-c)));
    printF(" a*exp(-(b)*c)=%8.2e\n",a*exp(-(bMax)*c));
    printF(" ar+a*exp((b)*(1.-c))-a*exp(-(b)*c)=%8.2e alphaMax=%8.2e\n",ar+a*exp((bMax)*(1.-c))-a*exp(-(bMax)*c),alphaMax);
    
    printF(" ra=%8.2e ar=%8.2e a=%8.2e c=%8.2e dx=%8.2e, dxOld=%8.2e, ratio=%8.2e "
           "fMin=%8.2e, fMax=%8.2e, alphaMax=%8.2e\n",
               ra, ar,a,c,dx,dxOld,ratio,fMin,fMax,alphaMax);
    

    assert( fMin*fMax<= 0. );
    int it=0, maxIt=20;
    const real tol=.001;
    while( bMax-bMin>tol && it<maxIt )
    {
      real bMid=.5*(bMax+bMin);
      real fMid=SPRIME(bMid);
      if( fMid*fMax<=0. )
      {
	fMin=fMid;
	bMin=bMid;
      }
      else
      {
	fMax=fMid;
	bMax=bMid;
      }
      printF(" it=%i : (b,f) : min=(%8.2e,%8.2e) max=(%8.2e,%8.2e)\n",it,bMin,fMin,bMax,fMax);
      it++;
    }
    b=.5*(bMin+bMax);

    printF("exp: Choosing exponent=%8.2e (to obtain a smallest grid spacing of %8.2e, original spacing=%8.2e)\n",
                 b,dx,dxOld);
    
    rpar[direction](3,0)=b;

    stretchParametersDialog[direction]->setTextLabel("min dx",sPrintF(line,"%.2g",dx));
  }

  else if( len=answer.matches("expl: parameters") )
  {
    real a=1., b=5., c=0.;
    sScanF(&answer[len],"%e %e %e",&a,&b,&c);
    rpar[direction](0,0)=a;
    rpar[direction](1,0)=b;
    rpar[direction](2,0)=c;
    
    stretchParametersDialog[direction]->setTextLabel("parameters",
                                   sPrintF(line,"%.2g %.2g %.2g (a,b,c)",a,b,c));
  }
  else if( dialog.getTextValue(answer,"expl: position","%e",rpar[direction](2,0)) )
  {
    stretchParametersDialog[direction]->setTextLabel("parameters",sPrintF(line,"%.2g %.2g %.2g (a,b,c)",
					             rpar[direction](0,0),rpar[direction](1,0),rpar[direction](2,0)));
  }
  else if( dialog.getTextValue(answer,"expl: linear weight","%e",rpar[direction](3,0)) )
  {
  }
  else if( len=answer.matches("expl: min dx, max dx") )
  {
    // Choose parameters to set :
    //    dxMin : min grid spacing
    //    dxMax : max grid spacing


    real dxMin=1.e-3, dxMax=1.e-1;
    sScanF(&answer[len],"%e %e",&dxMin,&dxMax);

    // determine the grid spacing on the unstretched grid
    // RealArray spacings;
    const real c=rpar[direction](2,0);
    
    real cFact = ( c==0. || c==1. ) ? 1. : 2.;

    // c==0 or c==1 : a/(1+a) = dxMin/dxMax
    // 0 < c < 1  :  2a/(1+a) = dxMin/dxMax

    // increase the number of grid points by the stretchResolutionFactor 
    // since stretched grids seem to have larger errors

    // make "a" larger by spacingFactor since we add more points below when computing nr 
    real a = stretchResolutionFactor*(dxMin/dxMax)/cFact;  
    // Choose b so that :
    //         a*exp(b) = k >> 1 
    // so that at r=1  (k/(1+k)) = 1 - 1/k + ... is nearly 1 and thus
    // the grid spacing is nearly uniform at r=1.
    // Choose a larger value to k to make linear region larger.
    const real weight = rpar[direction](3,0);
    real b = log(weight/a)*cFact;  
    printF(" Guess for b=%8.2e\n",b);

    rpar[direction](0,0)=a;
    rpar[direction](1,0)=b;
    printF("... Setting a=%e, b=%e\n",a,b);

    //StretchMapping & stretch = stretchedSquare->stretchFunction(direction);
    //stretch.setExponentialToLinearParameters( a,b,c );

    applyStretching( stretchingType,ipar,rpar );
    mappingHasChanged();  // do this before gridStatistics

    RealArray gridStats;
    gridStatistics( *this,gridStats,NULL  );

    real dxMin0 = gridStats(3+3*direction+0), dxAve0=gridStats(3+3*direction+1), dxMax0=gridStats(3+3*direction+2);

    int nr = getGridDimensions(direction);
    printF("Current: nr=%i, dxMin=%e dxAve=%e, dxMax = %e\n",nr,dxMin0,dxAve0,dxMax0);
    
    // set the number of grid lines : 
     
    const real dr = 1./(max(1,getGridDimensions(direction)-1));
    nr = int( (nr-1)*dxMin0/dxMin + 1.5 );

    // adjust nr for multigrid 
    int ml2 = int( pow(2,numberOfMultigridLevels) + .5 );
    nr = int( int(nr+ml2-2)/ml2 )*ml2+1;

    printF(".. New nr = %i (numberOfMultigridLevels=%i)\n",nr,numberOfMultigridLevels);
    setGridDimensions( direction, nr );
    mappingHasChanged();  

    stretchParametersDialog[direction]->setTextLabel("min dx, max dx",sPrintF(line,"%.2g %.2g",dxMin,dxMax));
    stretchParametersDialog[direction]->setTextLabel("parameters",sPrintF(line,"%.2g %.2g %.2g (a,b,c)",a,b,c));
  }



  else if( answer=="itanh: reset parameters" ||
           answer=="tanh: reset parameters" ||
	   answer=="exp: reset parameters" ||
	   answer=="expl: reset parameters" ||
	   answer=="expb: reset parameters"  )
  {
    // *** reset to default values ***
    rpar[direction]=0.;
    ipar[direction]=0;
    if( stretchingType[direction]==StretchMapping::inverseHyperbolicTangent )
    {
      rpar[direction](3,0)=1.5;  // end value for the last interval
    }
    else if( stretchingType[direction]==StretchMapping::hyperbolicTangent )
    {
      real a0=0., ar=1., b1=5., a1=-.9*ar/b1, c1=.5;   // choose a0 > a1*b1 to be invertible
      rpar[direction](0,0)=a0;
      rpar[direction](1,0)=ar;
      rpar[direction](2,0)=a1;
      rpar[direction](3,0)=b1;
      rpar[direction](4,0)=c1;
    }
    else if( stretchingType[direction]==StretchMapping::exponential )
    {
      real a0=0., ar=1., b1=5., a1=1., c1=.5;  
      ipar[direction](0,0)=0;  // stretch at r=0
	  
      rpar[direction](0,0)=a0;
      rpar[direction](1,0)=ar;
      rpar[direction](2,0)=a1;
      rpar[direction](3,0)=b1;
      rpar[direction](4,0)=c1;
    }
    else if( stretchingType[direction]==StretchMapping::exponentialToLinear )
    {
      real a=1.e-2, b=5., c=0., weight=10.;
      rpar[direction](0,0)=a;
      rpar[direction](1,0)=b;
      rpar[direction](2,0)=c;
      rpar[direction](3,0)=weight;
    }
  }
  else
  {
    printF("Unknown stretching command: [%s]\n",(const char *)answer);
    gi.stopReadingCommandFile();
    return 0;
  }


  // display current stretching parameters in the info label
  
  RealArray & rp = rpar[direction];
  IntegerArray & ip = ipar[direction];
    
  int numberOfInfoLabels=1;
  aString label[]= {"","","",""};
  if( stretchingType[direction]==StretchMapping::inverseHyperbolicTangent )
  {
    numberOfInfoLabels=2;
    int i, numberOfLayers=0, numberOfIntervals=0;
    label[0]="layer parameters:";  // default label if there are no layers
    label[1]="interval parameters:";
    for( i=0; i<=rp.getBound(1); i++ )
    {
      if( ip(0,i)==0 && rp(0,i)>0. )
      {
        if(numberOfLayers==0 ) 
          label[0]="layers: ";
	
	label[0]+=sPrintF(line,"%i:(%3.1f,%3.1f,%3.1f) ",numberOfLayers,rp(0,i),rp(1,i),rp(2,i));
        numberOfLayers++;
      }
      else if( ip(0,i)==1 && rp(0,i)>0. )
      {
        if(numberOfIntervals==0 ) 
          label[1]="layers: ";
	label[1]+=sPrintF(line,"%i:(%3.1f,%3.1f,%3.1f) ",numberOfIntervals,rp(0,i),rp(1,i),rp(2,i));
        numberOfIntervals++;
      }
	
    }
  }
  else if( stretchingType[direction]==StretchMapping::hyperbolicTangent ||
           stretchingType[direction]==StretchMapping::exponential )
  {
    numberOfInfoLabels=1;
    label[0]=sPrintF(line,"(a0,ar,a,b,c)=(%3.1f,%3.1f,%3.1f,%3.1f,%3.1f) ",rp(0,0),rp(1,0),rp(2,0),rp(3,0),rp(4,0));
  }
  else if( stretchingType[direction]==StretchMapping::exponentialToLinear )
  {
    numberOfInfoLabels=1;
    label[0]=sPrintF(line,"(a,b,c)=(%9.2e,%4.2f,%3.1f) ",rp(0,0),rp(1,0),rp(2,0));
  }
  else
  {
    numberOfInfoLabels=0;
  }
  
  for( int i=0; i<numberOfInfoLabels; i++ )
  {
    stretchParametersDialog[direction]->setInfoLabel(i+1,label[i]);
  }
  
  return 1;
}

int StretchTransform::
applyStretching( int stretchingType[3], IntegerArray *ipars, RealArray *rpars )
//=============================================================================
/// \details 
///      Apply the stretching parameters.
///    
///   stretchParams(0,i) = axis
///   stretchParams(1,i) = 
/// 
///  itanh:
///    layer    : a,b,c
///    interval : d,e,f   ... plus end value for f
/// 
///  tanh
/// 
/// 
///  exp
/// 
//=============================================================================
{
  assert( stretchedSquare!=NULL );

  int axis;
  for( axis=0; axis<domainDimension; axis++ )
  {
    RealArray & rpar = rpars[axis];
    IntegerArray & ipar = ipars[axis];
    
    StretchMapping & stretch = stretchedSquare->stretchFunction(axis);

    if( stretchingType[axis]==StretchMapping::inverseHyperbolicTangent )
    {

      // count the number of layers and intervals
      int numberOfLayers=0, numberOfIntervals=0;
      int i;
      for( i=0; i<=rpar.getBound(1); i++ )
      {
	if( ipar(0,i)==0 && rpar(0,i)>0. )
	{
	  numberOfLayers++;
	}
        else if( ipar(0,i)==1 && rpar(0,i)>0. )
	{
	  numberOfIntervals++;
	}
	
      }
      
      printF(" Stretch: axis=%i setNumberOfLayers=%i\n",axis,numberOfLayers);
      
      
      stretch.setNumberOfLayers( numberOfLayers );
      if( numberOfLayers>0 ) 
      {
	stretch.setStretchingType(StretchMapping::inverseHyperbolicTangent);


	int index=0;
	for( i=0; i<=rpar.getBound(1); i++ )
	{
	  if( ipar(0,i)==0 && rpar(0,i)>0. )
	  {
	    printF(" Stretch:itanh:layer: axis=%i index=%i i=%i (a,b,c)=(%8.2e,%8.2e,%8.2e)\n",axis,index,i,
		   rpar(0,i),rpar(1,i),rpar(2,i));

	    stretch.setLayerParameters(index, rpar(0,i),rpar(1,i),rpar(2,i));
	    index++;
	  }
	}
      }
      stretch.setNumberOfIntervals( numberOfIntervals );
      if( numberOfIntervals>0 )  // **** what if this has been reset to zero ??
      {

	stretch.setStretchingType(StretchMapping::inverseHyperbolicTangent);

	int index=0;
	for( i=0; i<=rpar.getBound(1); i++ )
	{
	  if( ipar(0,i)==1 && rpar(0,i)>0. )
	  {
	    printF(" Stretch:itanh:interval axis=%i index=%i i=%i (a,b,c)=(%8.2e,%8.2e,%8.2e)\n",axis,index,i,
		   rpar(0,i),rpar(1,i),rpar(2,i));

	    stretch.setIntervalParameters(index, rpar(0,i),rpar(1,i),rpar(2,i));
	    index++;
	  }
	}
        stretch.setIntervalParameters(index, 0.,0.,rpar(3,0));  // final f value
      }
    }
    else if( stretchingType[axis]==StretchMapping::hyperbolicTangent )
    {
      printF(" Stretch:tanh: axis=%i (a0,ar,a,b,c)=(%8.2e,%8.2e,%8.2e,%8.2e,%8.2e)\n",axis,
	     rpar(0,0),rpar(1,0),rpar(2,0),rpar(3,0),rpar(4,0));

      stretch.setStretchingType(StretchMapping::hyperbolicTangent);
      stretch.setHyperbolicTangentParameters(rpar(0,0),rpar(1,0),rpar(2,0),rpar(3,0),rpar(4,0));
    }
    else if( stretchingType[axis]==StretchMapping::exponential )
    {
      real a0=rpar(0,0), ar=rpar(1,0), a=rpar(2,0), b=rpar(3,0), c=rpar(4,0);  
      if( ipar(0,0)==1 ) // stretch at r=1
      {
	c=1.-c;
        b=-b;
	a0+=ar;
        ar=-ar;
	printF(" Stretch:exp: axis=%i -- adjusting parameters to stretch at r=1.\n",axis);
      }
      printF(" Stretch:exp: axis=%i (a0,ar,a,b,c)=(%8.2e,%8.2e,%8.2e,%8.2e,%8.2e)\n",axis,a0,ar,a,b,c);

      stretch.setStretchingType(StretchMapping::exponential);
      stretch.setExponentialParameters(a0,ar,a,b,c);
    }
    else if( stretchingType[axis]==StretchMapping::exponentialBlend )
    {
      
      printF(" Stretch:exp blend: axis=%i\n",axis);
      
      stretch.setStretchingType(StretchMapping::exponentialBlend);
    }
    else if( stretchingType[axis]==StretchMapping::exponentialToLinear )
    {
      
      real a=rpar(0,0), b=rpar(1,0), c=rpar(2,0), weight=rpar(3,0);
      printF(" Stretch:exponentialToLinear: axis=%i, (a,b,c)=(%9.2e,%4.2f,%3.1f)\n",axis,a,b,c);
      
      stretch.setStretchingType(StretchMapping::exponentialToLinear);
      stretch.setExponentialToLinearParameters(a,b,c);
    }
    else if( stretchingType[axis]==StretchMapping::linearSpacing )
    {
      real a0=rpar(0,0);
      real a1=rpar(1,0);
      if( ipar(0,0)==1 ) // stretch at r=1
      {
	printf(" Stretch:linearSpacing: axis=%i -- CANNOT adjust parameters to stretch at r=1. Fix this!\n",axis);
      }      

      printf(" Stretch:linearSpacing: axis=%i\n inner_dx=%8.2e outer_dx=%8.2e\n",axis,a0,a1);
      stretch.setStretchingType(StretchMapping::linearSpacing);
      stretch.setLinearSpacingParameters(a0,a1);
    }
    else if( stretchingType[axis]!=StretchMapping::noStretching )
    {
      printF("applyStretching:ERROR: unknown stretchingType[axis=%i]=%i\n",axis,stretchingType[axis]);
      
    }
    


  }
  
  return 0;
}



int StretchTransform::
update( MappingInformation & mapInfo,
	const aString & command,
	DialogData *interface /* =NULL */ ) 
//=============================================================================
/// \details 
///      Set stretching parameters
///    
//=============================================================================
{
  int returnValue=0;
  
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  aString prefix = "STRT:"; // prefix for commands to make them unique.


  const bool executeCommand = command!=nullString;
  if( false &&   // don't check prefix for now
      executeCommand && command(0,prefix.length()-1)!=prefix && command!="build dialog" )
    return 1;

  aString answer,line,answer2; 

  bool plotObject=true;


  bool mappingChosen= stretchedSquare!=NULL;

  // By default transform the last mapping in the list (if this mapping is unitialized)
  if( !mappingChosen )
  {
    if( mapInfo.mappingList.getLength()>0 )
    {
      // define the mappings to be composed:
      Mapping & map = *mapInfo.mappingList[mapInfo.mappingList.getLength()-1].mapPointer;
      setMapping(map);

      mappingChosen=true;
    }
    else
    {
      cout << "StretchTransfrom:ERROR: no mappings to transform!! \n";
      return 1;
    }
  }
  

  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!StretchTransform",
      "transform which mapping?",
//      "stretch",
      "edit unstretched mapping",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "show parameters",
      "check",
      "check inverse",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "    Transform a Mapping by stretching along the Coordinate directions",
      "transform which mapping? : choose the mapping to transform",
//      "stretch            : define stretching in each coordinate direction",
      "edit unstretched mapping: make changes to the unstretched mapping",
      " ",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "show parameters    : print current values for parameters",
      "check              : check the mapping and derivatives",
      "check inverse      : input points to check the inverse",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };








  aString stretchingTypeName[]={"itanh",
				"tanh",
				"exp",
				"exp blend",
                                "exp to linear",
				"none",""}; //

  const int noStretching=StretchMapping::noStretching;
  
  int stretchingType[3]={noStretching,noStretching,noStretching}; //
  real targetSpacing=-1.;
  real targetPosition=-1.;

  bool showNonPhysicalBoundaries=true;
  // automaticallyUpdateStretching: if true then update stretched grid after each change to the parameters, otherwise
  //         the user needs to explicitly choose "stretch grid"
  bool automaticallyUpdateStretching=true; 

  int numberOfMultigridLevels=0;  // used when the number of grid points is changed

  // increase the number of grid points by the following factor since stretched grids seem to have larger errors
  real stretchResolutionFactor=1.25;   

  IntegerArray ipar[3]; // (3,10);
  RealArray rpar[3]; // (4,10);
//    ipar=-1;
//    rpar=0.;

  GUIState gui;
  gui.setWindowTitle("Stretch Grid Lines");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;


  if( interface==NULL || command=="build dialog" )
  {

    // addPrefix(label,prefix,cmd,maxCommands);
    dialog.setOptionMenuColumns(1);
    const int maxStretchingTypes=20;
    aString stretchingTypeCommand[maxStretchingTypes];
    
    for( int axis=0; axis<domainDimension; axis++ )
    {
      // allow each axis to be stretched
      aString stretchLabel;
      sPrintF(stretchLabel,"Stretch r%i:",axis+1);
      GUIState::addPrefix(stretchingTypeName,stretchLabel,stretchingTypeCommand,maxStretchingTypes);
      dialog.addOptionMenu(stretchLabel,stretchingTypeCommand,stretchingTypeName,stretchingType[axis]);
    }

    aString colourBoundaryCommands[] = { "colour by bc",
					 "colour by share",
					 "" };
    // dialog.addRadioBox("boundaries:",colourBoundaryCommands, colourBoundaryCommands, 0 );
    dialog.addOptionMenu("boundaries:",colourBoundaryCommands, colourBoundaryCommands, 0 );

    aString pbLabels[] = {"stretch grid",
                          "print grid statistics",
                          "help stretching",
                          "mapping parameters...",
                          "plot r1 stretching",
                          "plot r2 stretching",
                          "plot r3 stretching",
			  ""};

    if( domainDimension<3 ) pbLabels[6]="";
    if( domainDimension<2 ) pbLabels[5]="";
    

    // addPrefix(pbLabels,prefix,cmd,maxCommands);
    int numRows=3;
    dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

    //    dialog.setSensitive(surfaceGrid==true,DialogData::pushButtonWidget,3);
    
    const int maximumCommands=30;
    aString cmdWithPrefix[maximumCommands];

    const int numberOfTextStrings=20;
    aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];
    

    int nt=0;
//      textLabels[nt]="target spacing";
//      sPrintF(textStrings[nt], "%.2g",targetSpacing); nt++;

//      textLabels[nt]="target position";
//      sPrintF(textStrings[nt], "%.2g",targetPosition); nt++;

    textLabels[nt] = "name"; 
    sPrintF(textStrings[nt], "%s", (const char*)getName(mappingName)); 
    nt++; 

    textLabels[nt] = "multigrid levels"; 
    sPrintF(textStrings[nt], "%i",numberOfMultigridLevels); 
    nt++; 

    textLabels[nt] = "stretch resolution factor"; 
    sPrintF(textStrings[nt], "%.3g",stretchResolutionFactor); 
    nt++; 

    // null strings terminal list
    assert( nt<numberOfTextStrings );
    textLabels[nt]="";   textStrings[nt]="";  
    // addPrefix(textLabels,prefix,cmd,maxCommands);
    GUIState::addPrefix(textLabels,prefix,cmdWithPrefix,maximumCommands);
    dialog.setTextBoxes(cmdWithPrefix, textLabels, textStrings);

    aString tbCommands[] = {"show lines on non-physical boundaries",
                            "automatically update stretching",
			    ""};
    int tbState[10];
    tbState[0] = showNonPhysicalBoundaries;
    tbState[1] = automaticallyUpdateStretching;
    int numColumns=1;
    dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


    RealArray gridStats;
    gridStatistics( *this,gridStats,NULL  );
      
    int nl=0;
    for( int axis=0; axis<domainDimension; axis++ )
    {
      dialog.addInfoLabel(sPrintF(line,"axis%i grid spacing (min,ave,max)=(%7.1e,%7.1e,%7.1e)",
				  axis+1,gridStats(3+3*axis+0),gridStats(3+3*axis+1),gridStats(3+3*axis+2)));
      nl++;
    }
    dialog.addInfoLabel(sPrintF(line,"grid volumes (min,ave,max)=(%7.1e,%7.1e,%7.1e)",
				gridStats(0),gridStats(1),gridStats(2)));

    gui.buildPopup(menu);

  }

  // --- Build the sibling dialogs for changing parameters ---
  // **** or should we only build the dialog on demand ****
  DialogData *stretchParametersDialog[3];
  for( int axis=0; axis<3; axis++ )
  {
    stretchParametersDialog[axis] = &gui.getDialogSibling();
    stretchParametersDialog[axis]->setWindowTitle(sPrintF(line,"Stretch r%i",axis+1));
    stretchParametersDialog[axis]->setExitCommand(sPrintF(line,"close r%i stretching parameters",axis+1), "close");

//      buildStretchingParametersDialog(*stretchParametersDialog[axis],
//  				    (StretchMapping::StretchingType)stretchingType[axis],
//  				    axis );

  }
  
  // make a dialog sibling for setting general mapping parameters
  DialogData & mappingParametersDialog = gui.getDialogSibling();
  buildMappingParametersDialog( mappingParametersDialog );

  if( !executeCommand  )
  {
    gi.pushGUI(gui);
    gi.appendToTheDefaultPrompt("StretchTransform>"); // set the default prompt
  }

  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
  parameters.set(GI_LABEL_GRIDS_AND_BOUNDARIES,true); // turn on plotting of coloured squares

//    GraphicsParameters referenceSurfaceParameters;
//    referenceSurfaceParameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

  
  SelectionInfo select; select.nSelect=0;
  int len;
  
  for(int it=0; ; it++)
  {
    if( !executeCommand )
    {
      if( it==0 && plotObject )
	answer="plotObject";
      else
      {
	plotObject=false;  // by default no need to redraw.

        gi.savePickCommands(false); // temporarily turn off saving of pick commands.     

	gi.getAnswer(answer,"", select);
         
        gi.savePickCommands(true); // turn back on

      }
    }
    else
    {
      if( it==0 ) 
        answer=command;
      else
        break;
    }

    // printF("answer=[%s]\n",(const char*)answer);

    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);   // strip off the prefix

    bool stretchGrid=false; // set to true if the grid needs updating
    
    if( getMappingParametersOption(answer,mappingParametersDialog,gi ) ) // new way *wdh* 100407
    {
      // Changes were made to generic mapping parameters such as lines, BC's, share, periodicity
      printF("Answer=%s found in getMappingParametersOption\n",(const char*)answer);
      mappingHasChanged(); 
      plotObject=true;
    }
    else if( answer=="colour by bc" || 
             answer=="colour by share" )
    {
      if( answer=="colour by bc" )
      {
        parameters.set(GI_BOUNDARY_COLOUR_OPTION,GraphicsParameters::colourByBoundaryCondition);
        // dialog.getRadioBox(0).setCurrentChoice(0);
        dialog.getOptionMenu("boundaries:").setCurrentChoice(0);
      }
      else if( answer=="colour by share" )
      {
        parameters.set(GI_BOUNDARY_COLOUR_OPTION,GraphicsParameters::colourByShare);
        // dialog.getRadioBox(0).setCurrentChoice(1);
        dialog.getOptionMenu("boundaries:").setCurrentChoice(1);
      }
      plotObject=true;
    }
    else if( len=answer.matches("Stretch r") )
    { 
      int direction=-1;
      sScanF(&answer[len],"%i",&direction);
      direction--;
      
      if( direction<0 || direction>=domainDimension )
      {
        printF("Invalid stretching direction = %i\n");
        gi.stopReadingCommandFile();
        continue;
      }

      aString name=answer(len+2,answer.length()-1);
      int sType= (name=="itanh" ? StretchMapping::inverseHyperbolicTangent :
                  name=="tanh"  ? StretchMapping::hyperbolicTangent : 
                  name=="exp"   ? StretchMapping::exponential :
                  name=="exp blend" ? StretchMapping::exponentialBlend : 
                  name=="exp to linear" ? StretchMapping::exponentialToLinear :
                  name=="none" ? StretchMapping::noStretching : -1);

      if( sType==stretchingType[direction] )
      {
	// The stretching type has not changed
        if( sType!=StretchMapping::noStretching )
	{
	  stretchParametersDialog[direction]->showSibling();
	}
        continue;
      }
      
      if( sType>=0 )
        stretchingType[direction]=sType;
      else
      {
        printF("Invalid stretching type = [%s]\n",(const char*)name);
        gi.stopReadingCommandFile();
        continue;
      }

      // printF(" Setting stretchingType[direction=%i]=%i \n",direction,stretchingType[direction]);

      StretchMapping & stretch = stretchedSquare->stretchFunction(direction);
      stretch.setStretchingType(StretchMapping::StretchingType(stretchingType[direction]));
      stretch.setIsPeriodic(getIsPeriodic(direction)); // *wdh* 030512
      
      dialog.getOptionMenu(answer(0,len+1)).setCurrentChoice(stretchingType[direction]);  

      stretchParametersDialog[direction]->closeDialog();
      if( stretchingType[direction]!=StretchMapping::noStretching )
      {
	buildStretchingParametersDialog(*stretchParametersDialog[direction],
					(StretchMapping::StretchingType)stretchingType[direction],
					direction );

        if( gi.isGraphicsWindowOpen() && !gi.readingFromCommandFile()  )
  	  stretchParametersDialog[direction]->openDialog(0);
	stretchParametersDialog[direction]->showSibling();
      }
      
      // Initialize parameters

      rpar[direction].redim(5,10);
      ipar[direction].redim(3,10);
      rpar[direction]=0.;
      ipar[direction]=-1;
      if( stretchingType[direction]==StretchMapping::inverseHyperbolicTangent )
      {
	rpar[direction](3,0)=1.5;  // end value for the last interval
      }
      else if( stretchingType[direction]==StretchMapping::hyperbolicTangent )
      {
	real a0=0., ar=1., b1=5., a1=-.9*ar/b1, c1=.5;   // choose a0 > a1*b1 to be invertible
	rpar[direction](0,0)=a0;
	rpar[direction](1,0)=ar;
	rpar[direction](2,0)=a1;
	rpar[direction](3,0)=b1;
	rpar[direction](4,0)=c1;
      }
      else if( stretchingType[direction]==StretchMapping::exponential )
      {
	real a0=0., ar=1., b1=5., a1=1., c1=.5;  
	ipar[direction](0,0)=0;  // stretch at r=0
	  
	rpar[direction](0,0)=a0;
	rpar[direction](1,0)=ar;
	rpar[direction](2,0)=a1;
	rpar[direction](3,0)=b1;
	rpar[direction](4,0)=c1;
      }
      else if( stretchingType[direction]==StretchMapping::exponentialToLinear )
      {
	real a=1.e-2, b=5., c=0., weight=10.;
	rpar[direction](0,0)=a;
	rpar[direction](1,0)=b;
	rpar[direction](2,0)=c;
	rpar[direction](3,0)=weight;
      }

    }
//      else if( answer=="change r1 stretching parameters..." ||
//               answer=="change r2 stretching parameters..." ||
//               answer=="change r2 stretching parameters..." )
//      {

//        int direction;
//        if( answer=="change r1 stretching parameters..." )
//          direction=0;
//        else if( answer=="change r2 stretching parameters..." )     
//          direction=1;
//        else
//          direction=2;

//        if( stretchingType[direction]==noStretching )
//        {
//  	printF("WARNING:You should specify a stretching type for direction=%i before you can change parameters\n",
//                      direction);
//  	continue;
//        }

//        stretchParametersDialog[direction]->closeDialog();
//        buildStretchingParametersDialog(*stretchParametersDialog[direction],
//                                        (StretchMapping::StretchingType)stretchingType[direction],
//  				      direction );

//        stretchParametersDialog[direction]->openDialog(0);
//        stretchParametersDialog[direction]->showSibling();
//      }
    else if( answer=="close r1 stretching parameters" ||
             answer=="close r2 stretching parameters" ||
             answer=="close r3 stretching parameters" )
    {

      int direction;
      if( answer=="close r1 stretching parameters" )
        direction=0;
      else if( answer=="close r2 stretching parameters" )     
        direction=1;
      else
        direction=2;

       stretchParametersDialog[direction]->hideSibling();
    }
    else if( updateStretchingParameters(answer, ipar,rpar, stretchingType, numberOfMultigridLevels,
                                        stretchResolutionFactor, stretchParametersDialog, dialog,mapInfo ) )
    {
      printF("Answer processed by updateStretchingParameters\n");

      stretchGrid=true;
    }
    else if( answer=="print grid statistics" )
    {
      RealArray gridStats;
      gridStatistics( *this,gridStats,stdout );
    }
    else if( len=answer.matches("name ") )
    {
      setName(mappingName,answer(len,answer.length()-1));
      dialog.setTextLabel("name",sPrintF(line, "%s", (const char*)getName(mappingName))); 
      plotObject=true;
    }
    else if( dialog.getTextValue(answer,"multigrid levels","%i",numberOfMultigridLevels) )
    {
      printF("If the number of grid points is changed automatically then this number will support %i"
             " multigrid levels\n",numberOfMultigridLevels);
    }
    else if( dialog.getTextValue(answer,"stretch resolution factor","%e",stretchResolutionFactor) )
    {
      printF("If the number of grid points is changed automatically then this number will be increased by\n"
             "   the stretchResolutionFactor = %6.3f to add extra points on stretched grids\n",stretchResolutionFactor);
    }
    else if( answer=="transform which mapping?" )
    { // Make a menu with the Mapping names
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+1];
      for( int i=0; i<num; i++ )
        menu2[i]=mapInfo.mappingList[i].getName(mappingName);

      menu2[num]="";   // null string terminates the menu
      int mapNumber = gi.getMenuItem(menu2,answer2);
      delete [] menu2;
      if( mapNumber<0 )
      {
        gi.outputString("Error: unknown mapping to stretch!");
        gi.stopReadingCommandFile();
      }
      else
      {
	if( mapInfo.mappingList[mapNumber].mapPointer==this )
	{
	  cout << "StretchMapping::ERROR: you cannot transform this mapping, "
                  "this would be recursive!\n";
	  continue;
	}
	// define the mappings to be composed:
	Mapping & map = *mapInfo.mappingList[mapNumber].mapPointer;

        setMapping( map );

        setName(mappingName,aString("stretched-")+map2.getName(mappingName));
        dialog.setTextLabel("name",sPrintF(line, "%s", (const char*)getName(mappingName))); 

	mappingChosen=true;
	plotObject=true;
      }
    }
    else if( answer=="edit unstretched mapping" )
    {
      assert( stretchedSquare!=NULL );
      if( mappingChosen )
      {
	map2.update(mapInfo);
      }
      else
      {
        printF("Sorry: cannot edit mapping before one is chosen\n");
      }
    }
    else if( dialog.getToggleValue(answer,"show lines on non-physical boundaries",showNonPhysicalBoundaries) )
    {
      parameters.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,showNonPhysicalBoundaries);
    }
    else if ( dialog.getToggleValue(answer,"automatically update stretching",automaticallyUpdateStretching ) )
    {
      if( !automaticallyUpdateStretching )
        printF(" automaticallyUpdateStretching=false: you should choose \"stretch grid\" to apply the stretching\n");
    }
    else if( answer=="show parameters" )
    {
      display(" ****** Parameters for the stretched mapping ******* ");
      stretchedSquare->display(" ***** Here are the parameters for the unit square stretching ***** ");
    }
    else if( answer=="plot" )
    {
      if( !mappingChosen )
      {
	gi.outputString("you must first choose a mapping to transform");
	continue;
      }
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      gi.erase();
      PlotIt::plot(gi,*this,parameters); 
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    else if( answer=="plot r1 stretching" ||
             answer=="plot r2 stretching" ||
             answer=="plot r3 stretching" )
    {
      int direction=-1;
      if( answer=="plot r1 stretching" )
	direction=0;
      else if( answer=="plot r2 stretching" )
	direction=1;
      else 
	direction=2;
      
      StretchMapping & stretch = stretchedSquare->stretchFunction(direction);

      const realArray & xg = stretch.getGrid();
      // xg.display("xg");

      const int n = getGridDimensions(0);
      printF(" direction=%i, n=%i\n",direction,n);
      stretch.setGridDimensions(axis1,n);
      
      real dr = 1./max(1,n-1);
      realArray r(n,1);
      r.seqAdd(0.,dr);
      
      realArray x(n,1),xr(n,1);
      stretch.map(r,x,xr);
      // r.display("r");
      // x.display("x");
      // xr.display("xr");
      
      real xrMax=max(fabs(xr));
      xr*=1./max(REAL_MIN*100.,xrMax);
      printF("Derivative of stretching function scaled by %8.2e\n",xrMax);
      
      realArray xrd(n,1,1,2);
      Range R=n;
      xrd(R,0,0,0)=r(R,0);
      xrd(R,0,0,1)=xr(R,0);
      
      DataPointMapping xrMap;
      xrMap.setDataPoints(xrd,3,1);
      xrMap.setIsPeriodic(axis1,stretch.getIsPeriodic(axis1));
      
      // Now plot the ratio of the grid spacings : max( dx(i+1)/dx(i) , dx(i)/dx(i+1) )
      DataPointMapping dxrMap;
      Range Rm(0,n-3);

      r.redim(n-2,1);
      xrd.redim(n-2,1,1,2);
      
      real drm=1./max(1,n-2);
      r.seqAdd(drm,drm);
      xrd(Rm,0,0,0)=r(Rm,0);
      xrd(Rm,0,0,1)=(x(Rm+2,0)-x(Rm+1,0))/max(1.e-5,(x(Rm+1,0)-x(Rm,0)));
      xrd(Rm,0,0,1)=max(xrd(Rm,0,0,1),1./xrd(Rm,0,0,1));

      // xrd.display("xrd");

      dxrMap.setDataPoints(xrd,3,1);

      printF("======================================================================\n"
	     " INFO on plotting the stretching function:\n"
	     "    x(r) is the stretching function \n"
	     "    dx/dr is the derivative of the stretching function (scaled) \n"
	     "    dx-ratio = the ratio of adjacent grid spacings \n"
	     "             = max(r,1/r) where r=dx(i+1)/dx(i)  \n"
	     "        where dx(i) = x(i+1)-x(i) is the grid spacing \n"
	     "======================================================================\n");

//      stretch.update(mapInfo );
      parameters.set(GI_PLOT_GRID_POINTS_ON_CURVES,true);
//      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      gi.erase();

      GUIState gui;
      gui.setWindowTitle("Plot stretch");
      gui.setExitCommand("continue", "continue");
      gui.addInfoLabel("x(r) is the stretching function");
      gui.addInfoLabel("dx-ratio = max(r,1/r) , r=dx(i+1)/dx(i)");

      gi.pushGUI(gui);
      
      for( int it=0;; it++ )
      {
	if( it==0  )
	  answer="plotObject";
	else
	  gi.getAnswer(answer,"", select);

        if( answer=="exit" || answer=="continue" )
          break;

        gi.setAxesLabels("r","x");

        real size=.05;
	real xpos=.0, ypos=.85;
	int centering=0;

        parameters.set(GI_TOP_LABEL,sPrintF(line,"r%i %s stretching function",direction+1,
                 (const char*)stretchingTypeName[stretchingType[direction]] ));

	parameters.set(GI_MAPPING_COLOUR,"green");
	PlotIt::plot(gi,xrMap,parameters);  

	parameters.set(GI_MAPPING_COLOUR,"blue");
	PlotIt::plot(gi,dxrMap,parameters);  // ratio of grid spacings

        parameters.set(GI_MAPPING_COLOUR,"red");
	PlotIt::plot(gi,stretch,parameters); 

        // **** this next stuff should be moved to a function in GL_GraphicsInterface *************
        int labelList=gi.getNewLabelList(gi.getCurrentWindow());
        assert( labelList!=0 );
	glDeleteLists(labelList,1);
	glNewList(labelList,GL_COMPILE);
        gi.label("dx-ratio",xpos-size*3,ypos,size,+1,0,parameters,"blue"); 
        gi.label(sPrintF(line,"%2.1f*dx/dr",1./xrMax),xpos+size,ypos,size,0,0,parameters,"green"); 
        gi.label("x(r)",xpos+size*5,ypos,size,-1,0,parameters,"red"); 
	glEndList();

	
      }

//      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      parameters.set(GI_PLOT_GRID_POINTS_ON_CURVES,false);

      gi.popGUI();
      plotObject=true;
    }
    else if( answer=="help stretching" )
    {
      gi.outputString(
        "====================================================================================================\n"
	"                   Stretching a Mapping \n"
        " \n"
	" The mapping can be stretched along each of the coordinate directions, r1,r2 or r3.\n"
        " When one chooses a stretching type for a given coordinate direction, a new dialog\n"
        " window will open. This new dialog allows one to set the stretching parameters.\n"
        " \n"
	"There are a number of functions that can be used to stretch grid lines. For boundary layer\n"
        " stretching the EXP function is probably most useful. For stretching an internal layer the \n"
        " ITANH or TANH functions are good. For stretching at multiple locations use ITANH.\n"
        " \n"
        "   ----------------------------------------------------------------------------------------     \n"
        "                         ITANH  \n"
	" itanh stretching uses layer functions (inverse tanh) and interval functions (inverse log(cosh))\n"
	"     layer function: U_i(x) = .5*a_i*tanh(b_i*(x-c_i))   \n"
	"     interval function V_i(x) = see documentation \n"
        " To stretch grids lines to one position (such as r=0, r=.5,  or r=1) with a given grid spacing dx \n"
        " you can use the command (found in the dialog window that opens when `itanh' stretching is chosen) \n"
        "           `stretch r1 itanh: position and min dx 0. .01'  \n"
        " Stretching lines to more than one location is a bit harder and requires using the commands \n"
	" `stretch r1 itanh: id a b c ' defines a stretching of grid points along coordinate direction r1\n"
	"   where id = the unique identifier for the stretching, id=0,1,2,3... \n"
	"         a = weight of the stretching, a>0 (a good value is 1., a=0 means no stretching) \n"
	"         b = the exponent of the stretching, b>0 (b=5 give some stretching b=10 gives more)\n"
	"         c = the position of the stretching on the unit interval, 0 <= c <=1\n"
	" You may define multiple stretchings along each of the coordinate directions by choosing\n"
	" a different id. \n"
	" NOTE: that each stretching (over all directions) must have a unique id. \n"
	" Example: To stretch at both ends of direction r1 and in the middle of direction r2\n"
	"    stretch r1 itanh: 0 1. 10. 0.    (id=0 : stretch at r1=0.)\n"
	"    stretch r1 itanh: 1 1. 10. 1.    (id=1 : stretch at r1=1.)\n"
	"    stretch r2 itanh: 2 1. 5. .5     (id=2 : stretch at r2=.5\n"
        "   ----------------------------------------------------------------------------------------     \n"
        "                         TANH  \n"
        "  tanh stretching uses a hyperbolic tangent function to define the stretching\n"
        "        x(r) = [a0 + ar*r + a*tanh(b*(r-c)) - offset ]*scale   \n"
        "   The easiest way to stretch is to specify the position of the stretching (e.g. r=0, r=.5, r=1.) \n"
        "   and the required minimum grid spacing, dx (in the dialog window that opens when `tanh' stretching \n"
        "   is chosen) The value of 'a' will be determined from the given values of dx,c,ar and b.\n"
        "   ----------------------------------------------------------------------------------------     \n"
        "                         EXP  \n"
        "   exp stretching uses an exponential function to define the stretching\n"
        "        x(r) = [a0 + ar*r + a*exp(b*(r-c)) - offset ]*scale   \n"
        "   The easiest way to stretch is to specify the position of the stretching (e.g. r=0, c=1) \n"
        "   and the required minimum grid spacing, dx. The exponent b will be determined to achieve this\n"
        "   value for dx (given ar, a, and c). \n"
        "   NOTE that the grid can only be stretched at the end points r=0 or r=1 with this stretching function.\n"
        "   ----------------------------------------------------------------------------------------     \n"
        "                         EXPL  \n"
        "   expl stretching transtitions from exponential stretching to uniform spacing:\n"
	"       x(r) = [ log( 1 + a*exp(s*b*(r-c)) ) - x0 ]*scale \n"
	"     a = dxMin/dxMax (e.g. 1.e-2 or 1.e-3 ),\n"
	"     b = stretching exponent (you probably want a*exp(b*r) > 5 ), \n"
	"     c = 0. or 1. to put the stretching ar r=0 or r=1. (then s=1-2*c is +1 or -1),\n"
	"     x0 and scale are chosen automatically so that x(0)=0 and x(1)=1.\n"
        "   The easiest way to set the parameters is by specifying the minimum and maximum grid spacing and then\n"
        "   the program will automatically determine all the parameters AND the number of grid points. In this case\n"
        "   the value of b is chosen so that a*exp(b)=W where W is the `linear weight' parameter (by default W=10.).\n"
        "   Choose a large value for 'linear weight' to increase the region where the stretching is linear.\n"
        "   ----------------------------------------------------------------------------------------     \n"
        " \n"
	"==================================================================================================\n" );
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
    }
    else if( updateWithCommand(mapInfo, answer) )
    {
      // changes were made to generic mapping parameters such as lines, BC's, share, periodicity
      mappingHasChanged();
      plotObject=true;
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity" ||
             answer=="check"||
             answer=="check inverse" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
      mappingHasChanged(); 
      plotObject=true;
    }
    else if( answer=="stretch" ) 
    {
      // *** Here is the old way of stretching ***

      stretchedSquare->setName(mappingName,"Stretched Unit Square");
      for( int axis=0; axis<domainDimension; axis++ )
      { // label stretchedSquare axes
        stretchedSquare->setName(mappingItemName(domainAxis1Name+axis),
                         map2.getName(mappingItemName(domainAxis1Name+axis)));
        stretchedSquare->setName(mappingItemName(rangeAxis1Name +axis),
                         map2.getName(mappingItemName(domainAxis1Name+axis)));
      }
      stretchedSquare->update(mapInfo);
      mappingHasChanged(); 
      plotObject=true;
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=true;
    else if( answer!="stretch grid" )
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s\n",(const char*)answer) );
      printF("Unknown response=[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }

    if( (automaticallyUpdateStretching && stretchGrid) || answer=="stretch grid" )
    {
      
      applyStretching( stretchingType,ipar,rpar );

      mappingHasChanged();  // do this before gridStatistics *wdh* 100326
      plotObject=true;
      
      RealArray gridStats;
      gridStatistics( *this,gridStats,NULL );
      
      int nl=0;
      for( int axis=0; axis<domainDimension; axis++ )
      {
	dialog.setInfoLabel(nl,sPrintF(line,"axis%i grid spacing (min,ave,max)=(%7.1e,%7.1e,%7.1e)",
				      axis+1,gridStats(3+3*axis+0),gridStats(3+3*axis+1),gridStats(3+3*axis+2)));
        nl++;
      }
      dialog.setInfoLabel(nl,sPrintF(line,"grid volumes (min,ave,max)=(%7.1e,%7.1e,%7.1e)",
				     gridStats(0),gridStats(1),gridStats(2)));

    }

    if( plotObject )
    {
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters);  
    }
  }

  gi.erase();

  if( !executeCommand  )
  {
    gi.popGUI();
    gi.unAppendTheDefaultPrompt();
  }

  return returnValue;
}

