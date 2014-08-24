#include "FilletMapping.h"
#include "HyperbolicMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "NormalMapping.h"
#include "StretchMapping.h"
#include "ReductionMapping.h"
#include "LineMapping.h"
#include "TFIMapping.h"
#include "ComposeMapping.h"
// * for debugging:
//#include "PlotStuff.h"
#include "GenericGraphicsInterface.h"

FilletMapping::
FilletMapping() : Mapping(1,2,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief  Default Constructor
///   Define a fillet mapping that creates a smooth surface in
///   the region where two curves (surfaces) intersect
//===========================================================================
{ 
  FilletMapping::className="FilletMapping";
  setName( Mapping::mappingName,"FilletMapping");
  setGridDimensions( axis1,15 );
  curve[0]=NULL;
  curve[1]=NULL;

  setup();

}


FilletMapping::
FilletMapping(Mapping & curve1, 
	      Mapping & curve2)
  : Mapping(1,2,parameterSpace,cartesianSpace) 
//===========================================================================
/// \brief 
/// \param curve1, curve2 (input): 
//===========================================================================
{
  FilletMapping::className="FilletMapping";
  setName( Mapping::mappingName,"FilletMapping");

  curve[0]=&curve1;
  if( !curve[0]->uncountedReferencesMayExist() ) 
    curve[0]->incrementReferenceCount();
  
  curve[1]=&curve2;
  if( !curve[1]->uncountedReferencesMayExist() ) 
    curve[1]->incrementReferenceCount();

  setup();
  setupForNewCurves();
}


// Copy constructor is deep by default
FilletMapping::
FilletMapping( const FilletMapping & map, const CopyType copyType )
{
  FilletMapping::className="FilletMapping";
  setup();
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "FilletMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

FilletMapping::
~FilletMapping()
{ 
  if( debug & 4 )
    cout << " FilletMapping::Desctructor called" << endl;
  delete s[0];
  delete s[1];
  delete blendingFunction;
  if( curve[0]!=NULL && curve[0]->decrementReferenceCount()==0 )
    delete curve[0];
  if( curve[1]!=NULL && curve[1]->decrementReferenceCount()==0 )
    delete curve[1];
}



FilletMapping & FilletMapping::
operator=( const FilletMapping & x )
{
  if( FilletMapping::className != x.getClassName() )
  {
    cout << "FilletMapping::operator= ERROR trying to set a FilletMapping = to a" 
      << " mapping of type " << x.getClassName() << endl;
    return *this;
  }
  if( curve[0]!=NULL && curve[0]->decrementReferenceCount()==0 )
    delete curve[0];
  if( curve[1]!=NULL && curve[1]->decrementReferenceCount()==0 )
    delete curve[1];

  curve[0]=x.curve[0];                  // this sets the pointer only
  curve[0]->incrementReferenceCount();
  curve[1]=x.curve[1];                  // this sets the pointer only
  curve[1]->incrementReferenceCount();

  blendingFunction   =x.blendingFunction;                    // this sets the pointer only
  filletType=x.filletType;
  intersectionFound=x.intersectionFound;
  intersectionToUse=x.intersectionToUse;
  filletWidth=x.filletWidth;
  filletOverlap=x.filletOverlap;
  orient=x.orient;
  blendingFactor=x.blendingFactor;
  newCurves=x.newCurves;
  s[0]=x.s[0];
  s[1]=x.s[1];

  offsetType[0]=x.offsetType[0];
  offsetType[1]=x.offsetType[1];
  joinType=x.joinType;
  
  this->Mapping::operator=(x);            // call = for derivee class
  return *this;
}

int FilletMapping::
setup()
{
  blendingFunction=NULL;
  filletType=nonParametric;
  joinType=fillet;
  
  intersectionFound=FALSE;
  
  intersectionToUse=0;   // use this intersection point/curve
  filletWidth=.3;        // width of the fillet
  filletOverlap=.2;      // overlap this amount onto the curves/surfaces
  orient=2;
  blendingFactor=10.;  // determines the sharpness of the blend
  newCurves=TRUE;
  s[0]=NULL;
  s[1]=NULL;
  offsetType[0]=hyperbolicOffset;
  offsetType[1]=hyperbolicOffset;
  
  uninitialized=TRUE;
  mappingHasChanged();
  return 0;
}


int FilletMapping::
setupForNewCurves()
{
  if( curve[0]!=NULL && curve[1]!=NULL )
  {
    for( int i=0; i<=1; i++ )
    {
      Mapping & c = *(curve[i]);
      if( c.getRangeDimension() - c.getDomainDimension() != 1 )
      {
	cout << "FilletMapping::ERROR: c" << i+1 << " is not a c in 2D or surface in 3D \n";
	cout << " c" << i+1 << " : domainDimension = " << c.getDomainDimension()
	     << ",  rangeDimension = " << c.getRangeDimension() << endl;
	throw "error";
      }
    }
    setDomainDimension(curve[0]->getDomainDimension());
    setRangeDimension(curve[0]->getRangeDimension());
    for( int axis=axis1; axis<domainDimension; axis++ )
      setGridDimensions( axis,21 );
  }
  uninitialized=TRUE;
  mappingHasChanged();
  return 0;
}


int FilletMapping::
setCurves(Mapping & curve1, 
	  Mapping & curve2)
//===========================================================================
/// \details  Supply the curves or surfaces from which the fillet will be defined.
/// \param curve1, curve2 (input): 
//===========================================================================
{
  if( curve[0]!=NULL && curve[0]->decrementReferenceCount()==0 )
    delete curve[0];
  if( curve[1]!=NULL && curve[1]->decrementReferenceCount()==0 )
    delete curve[1];

  curve[0]=&curve1;
  curve[1]=&curve2;

  if( !curve[0]->uncountedReferencesMayExist() ) 
    curve[0]->incrementReferenceCount();
  if( !curve[1]->uncountedReferencesMayExist() ) 
    curve[1]->incrementReferenceCount();

  newCurves=TRUE;
  intersectionFound=FALSE;
  
  setupForNewCurves();
  return 0;
}


void FilletMapping::
initialize()
{
  uninitialized=FALSE;
  
  if( blendingFunction==NULL )
  {
    blendingFunction= new StretchMapping(StretchMapping::hyperbolicTangent);
    ((StretchMapping*)blendingFunction)->setIsNormalized(FALSE);
    ((StretchMapping*)blendingFunction)->setHyperbolicTangentParameters(.5,0.,.5,10.,.5);  // .5+.5tanh(10*(r-.5))
  }
  
  assert( curve[0]!=NULL && curve[1]!=NULL );
  
  sc.redim(2,4);
  // orient= 0 :  "curve 1- to curve 2-",
  // orient= 1 :  "curve 1+ to curve 2-",
  // orient= 2 :  "curve 1- to curve 2+",
  // orient= 3 :  "curve 1+ to curve 2+",
  real pm[2];
  pm[0] = orient   % 2 == 0 ? -1. : +1.;
  pm[1] = orient/2 % 2 == 0 ? -1. : +1.;

  if( domainDimension==1 )
  {
    if( newCurves )
    {
      newCurves=FALSE;
      // compute the intersection points between the curves
      printf("Fillet: find intersection point(s) between the curves\n");
      inter.intersectCurves(*curve[0],*curve[1],numberOfIntersections,rIntersect[0],rIntersect[1],xIntersect);
      int i;
      for( i=0; i<numberOfIntersections; i++ )
      {
	printf("intersection point %i : r1=%e, r2=%e, x=(%e,%e) \n",i,rIntersect[0](i),rIntersect[1](i),
	       xIntersect(0,i),xIntersect(1,i));
      }
    }
    intersectionFound=TRUE;
    // compute the coefficients in the functions s_i(r) = (1-r)*sc(i,0)+r*sc(i,1)+...
    realArray rr(1,1),cr(1,2,1);
    
    real shift = 0.;  // .2;
    for( int i=0; i<=1; i++ )
    {
      // determine crNorm= || curve.r || for scaling:
      rr(0,0)=rIntersect[i](0);
      curve[i]->map(rr,Overture::nullRealDistributedArray(),cr);
      real crNorm=SQRT( SQR(cr(0,0,0))+SQR(cr(0,1,0)) );
      
      // define the parameters for the functions s_i(r)
      sc(i,0)=rIntersect[i](intersectionToUse);   // intersection point
      sc(i,1)=sc(i,0)-pm[i]*.5*filletWidth/crNorm;
      sc(i,2)=sc(i,0)-pm[i]*(.5*filletWidth+filletOverlap)/crNorm;
      sc(i,3)=sc(i,0)+pm[i]*shift*.5*filletWidth/crNorm;
    }
    if( Mapping::debug & 2 )
      sc.display("FilletMapping::initialize: Here is sc(i,0:3)");
  }
  else if( domainDimension==2 )
  {
    char buff[80];
    if( newCurves )
    {
      newCurves=FALSE;
      // compute the intersection points between the curves
      // Mapping *inter.rCurve1 : 
      // Mapping *inter.rCurve2 : 
      // Mapping *inter.curve   : 
      int result = inter.intersect(*curve[0],*curve[1]);
      if( result!=0 )
      {
	cout << "FilletMapping:ERROR in computing the intersection curve \n";
        intersectionFound=FALSE;
      }
      else
      {
        intersectionFound=TRUE;
        if( inter.curve->getIsPeriodic(axis1)==functionPeriodic )
	{
          // curve is periodic -> fillet is periodic
          setIsPeriodic(axis1,functionPeriodic);
          setBoundaryCondition(Start,axis1,-1);
          setBoundaryCondition(End  ,axis1,-1);
	}
      }
    }
    if( intersectionFound )
    {
      intersectionFound=TRUE;
      printf("intersection curve found. Now compute surfaces with the hyperbolic grid generator..\n");
      // now compute s1,s2:
      if( filletType==parametric )
      {
	for( int i=0; i<=1; i++ )
	{
	  Mapping & c = i==0 ? *inter.rCurve1 : *inter.rCurve2;
          if( offsetType[i]==hyperbolicOffset )
	  {
	    s[i]=new HyperbolicMapping(c);  // ****************************************** reference count *****
	    s[i]->setName(mappingName,sPrintF(buff,"Parameter space grid s[%i]",i));
	
	    HyperbolicMapping & hype = (HyperbolicMapping&)*s[i];
	    hype.setParameters(HyperbolicMapping::growInBothDirections);
	    IntegerArray ipar(2);
	    RealArray rpar(2);
	    ipar(0)=0; // region number
	    rpar(0)=.5*filletWidth+filletOverlap;   // distance to march

	    printf("PARAMETER surface grid %i : distance to march=%e \n",i,rpar(0));

	    hype.setParameters(HyperbolicMapping::distanceToMarch,ipar,rpar,HyperbolicMapping::bothDirections);
	    // ***** here we generate the hyperbolic grid in the parameter space ****
	    hype.generate();
	  }
	  else
	  {
	    // make a curve offset from c (parameter space)
            real dist = (.5*filletWidth+filletOverlap)*pm[i];  
            s[i]= new NormalMapping(c,dist); s[i]->incrementReferenceCount();

	  }
	}
      }
      else
      {
	for( int i=0; i<=1; i++ )
	{
          if(  offsetType[i]==hyperbolicOffset )
	  {
	    s[i]=new HyperbolicMapping(*curve[i],*inter.curve);
	    s[i]->setName(mappingName,sPrintF(buff,"Physical space grid s[%i]",i));
	
	    HyperbolicMapping & hype = (HyperbolicMapping&)*s[i];
	    hype.setParameters(HyperbolicMapping::growInBothDirections);
	    IntegerArray ipar(2);
	    RealArray rpar(2);
	    ipar(0)=0;
	    rpar(0)=.5*filletWidth+filletOverlap;   // distance to march
	    hype.setParameters(HyperbolicMapping::distanceToMarch,ipar,rpar,
			       HyperbolicMapping::bothDirections);

	    printf("surface grid %i : distance to march=%e \n",i,rpar(0));
	  
	  // ***** here we generate the hyperbolic grid in physical space ****
	    hype.generate();
	  }
	  else
	  {
            // Make a TFIMapping in parameter space then a Compose Mapping

	    Mapping & intersectionCurve = i==0 ? *inter.rCurve1 : *inter.rCurve2;
	    realArray r(2,1),x(2,2);
	    r=0; r(1,0)=1.;
	    intersectionCurve.map(r,x);  // intersection curve may not be parameterized from [0,1]
	    printf("Intersection curve in parameter space: r_0=[%e,%e], r_1=[%e,%e] \n",x(0,0),x(1,0),x(0,1),x(1,1));
	    // we have two choices for the shape of the patch in the parameter space.
      
	    Mapping *line;
            
	    if( fabs(x(1,0)-x(0,0)) > fabs(x(1,1)-x(0,1)) )
	    {
              // assumes angular direction is axis1
              real endOfJoin=.5;  // **** fix this ****
 	      line = new LineMapping(x(0,axis1),endOfJoin, x(1,axis1),endOfJoin );  
	    }
	    else
	    {
              real endOfJoin=.5;
	      line = new LineMapping(endOfJoin,x(0,axis2), endOfJoin,x(1,axis2) ); 
	    }
	    line->incrementReferenceCount();

	    Mapping & tfi = *new TFIMapping(0,0,&intersectionCurve,line);   // tfi mapping in parameter space.
            tfi.incrementReferenceCount();

	    tfi.setRangeSpace(parameterSpace);
      
	    s[i] = new ComposeMapping(tfi,*curve[i]);
            
//            PlotStuff & gi = *Overture::getGraphicsInterface();
            GenericGraphicsInterface & gi = (GenericGraphicsInterface &) *Overture::getGraphicsInterface();
            GraphicsParameters params;
	    params.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
            gi.erase();
	    PlotIt::plot(gi,*s[i],params);
	    params.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

            line->decrementReferenceCount();
            tfi.decrementReferenceCount();

	  }
	}
      }
    }
    real shift = 0.;  // .2;
    for( int i=0; i<=1; i++ )
    {
      // determine crNorm= || curve.r || for scaling:
      // rr(0,0)=rIntersect[i](0);                                   
      // curve[i]->map(rr,Overture::nullRealDistributedArray(),cr);
      real crNorm=1.; // SQRT( SQR(cr(0,0,0))+SQR(cr(0,1,0)) );  // ***** fix this *****
      
      // define the parameters for the functions s_i(r)
      sc(i,0)=.5;                                                      // intersection point
      sc(i,1)=sc(i,0)-pm[i]*.5*filletWidth/crNorm;                     // distance from intersection point for c1
      sc(i,2)=sc(i,0)-pm[i]*(.5*filletWidth+filletOverlap)/crNorm;     // distance from intersection point for c2
      sc(i,3)=sc(i,0)+pm[i]*shift*.5*filletWidth/crNorm;
    }
  }
}

void FilletMapping::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
//=====================================================================================
/// \brief  Evaluate the mapping and/or derivatives. 
//=====================================================================================
{
  if( uninitialized )
    initialize();
  if( !intersectionFound )
  {
    cout << "FilletMapping::ERROR: the fillet has not been created yet\n";
    return;
  }
    
  if( params.coordinateType != cartesian )
    cerr << "FilletMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

    // evaluate the two curves
  realArray c[2], cr[2], t(I), b(I), br;
  int axis;
  
  c[0].redim(I,rangeDimension); 
  c[1].redim(I,rangeDimension); 
  if( computeMapDerivative )
  {
    cr[0].redim(I,rangeDimension,domainDimension);
    cr[1].redim(I,rangeDimension,domainDimension);
    br.redim(I);
  }
  // compute coefficients of quadratic blend function for fillet:
  real c0[2],c1[2],c2[2];
  int i;
  for( i=0; i<=1; i++ )
  {
    c0[i]=sc(i,2+i);
    c1[i]=sc(i,3-i)-sc(i,2+i)+(16./3.)*(sc(i,1)-.75*sc(i,2)-.25*sc(i,3));
    c2[i]=-((16./3.)*(sc(i,1)-.75*sc(i,2)-.25*sc(i,3)));
  }

  if( domainDimension==1 )
  {
    const realArray & rI = r(I,axis1);

    for( int i=0; i<=1; i++ )
    {
      t=c0[i]+rI*(c1[i]+rI*c2[i]);
      curve[i]->map(t,c[i],cr[i]);
    }

    //blendingFunction->map(r(I,axis1),b,br);  
    const realArray & tanhb = evaluate(tanh(blendingFactor*(rI-.5))); 
    b=.5+.5*tanhb;
  
    if( computeMap )
    {
      for( axis=axis1; axis<rangeDimension; axis++ )
	x(I,axis) = (1.-b)*c[0](I,axis) + b*c[1](I,axis);
    }
    if( computeMapDerivative )
    {
      br=.5*blendingFactor*(1.-SQR(tanhb));  
      const realArray & t0r = evaluate( c1[0]+(2.*c2[0])*rI );
      const realArray & t1r = evaluate( c1[1]+(2.*c2[1])*rI );
      
      for( axis=axis1; axis<rangeDimension; axis++ )
	xr(I,axis,axis1) = (1.-b)*cr[0](I,axis)*t0r + b*cr[1](I,axis)*t1r +(c[1](I,axis)-c[0](I,axis))*br;
    }
  }
  else if( domainDimension==2 )
  {
    // orient= 0 :  "curve 1- to curve 2-",
    // orient= 1 :  "curve 1+ to curve 2-",
    // orient= 2 :  "curve 1- to curve 2+",
    // orient= 3 :  "curve 1+ to curve 2+",
    // real pm[2];
    // pm[0] = orient   % 2 == 0 ? -1. : +1.;
    // pm[1] = orient/2 % 2 == 0 ? -1. : +1.;

    const realArray & rI = r(I,axis2);  // normal direction
    realArray rr(I,domainDimension), normal(I,domainDimension), rc(I,domainDimension), rcr(I,domainDimension,1);

    for( int i=0; i<=1; i++ )
    {
/* -----
      t=r(I,axis2);
      if( i==0 )
        inter.rCurve1->map(t,rc,rcr);   // intersection curve in the parameter space of curve[0]
      else
        inter.rCurve2->map(t,rc,rcr);   // intersection curve in the parameter space of curve[1]

      // we create offset curves from the intersection curve by extending in the normal direction
      normal(I,axis1)= rcr(I,axis2);     // must get the correct sign **********
      normal(I,axis2)=-rcr(I,axis1);
      t=SQRT( SQR(normal(I,axis1)) + SQR(normal(I,axis2)) );
      for( axis=axis1; axis<domainDimension; axis++ )
        normal(I,axis)/=t;
      for( axis=axis1; axis<domainDimension; axis++ )
      {
	c1(I,axis) = rc(I,axis) + normal(I,axis)*sc(i,1);
	c2(I,axis) = rc(I,axis) + normal(I,axis)*sc(i,2);
	c3(I,axis) = rc(I,axis) + normal(I,axis)*sc(i,3);
      }
      if( i==0 )
      {
        for( axis=axis1; axis<domainDimension; axis++ )
	  rr(I,axis)=((1.-rI)*c2(I,axis))
	    +rI*(c3(I,axis)+(1.-rI)*((16./3.)*(c1(I,axis)-.75*c2(I,axis)-.25*c3(I,axis))));
      }
      else
      {
        for( axis=axis1; axis<domainDimension; axis++ )
	  rr(I,axis)=((1.-rI)*c3(I,axis))
	    +rI*(c2(I,axis)+(1.-rI)*((16./3.)*(c1(I,axis)-.75*c2(I,axis)-.25*c3(I,axis))));
      }
----- */

      // rc(I,axis1) : normal to the intersection curve 
      rc(I,axis1)=r(I,axis1); // tangential
      rc(I,axis2)=c0[i]+rI*(c1[i]+rI*c2[i]);
      
      if( filletType==parametric )
      {
        s[i]->map(rc,rr);   // hyperbolic surface has been generated in parameter space.
        curve[i]->map(rr,c[i],cr[i]);
      }
      else
      {
        s[i]->map(rc,c[i],cr[i]);
      }
    }

    //blendingFunction->map(r(I,axis1),b,br);  
    const realArray & tanhb = evaluate(tanh(blendingFactor*(rI-.5))); 
    b=.5+.5*tanhb;
  
    if( computeMap )
    {
      for( axis=axis1; axis<rangeDimension; axis++ )
	x(I,axis) = (1.-b)*c[0](I,axis) + b*c[1](I,axis);
    }
    if( computeMapDerivative )
    {
      br=.5*blendingFactor*(1.-SQR(tanhb));  // db/r2  rI = axis2
      // rc0r = d(rc(i=0))/dr2
      const realArray & rc0r = evaluate(c1[0]+(2.*c2[0])*rI);
      // rc1r = d(rc(i=1))/dr2
      const realArray & rc1r = evaluate(c1[1]+(2.*c2[1])*rI);
      for( axis=axis1; axis<rangeDimension; axis++ )
      {
	xr(I,axis,axis1) = (1.-b)*cr[0](I,axis,axis1) + b*cr[1](I,axis,axis1);
	xr(I,axis,axis2) = (1.-b)*cr[0](I,axis,axis2)*rc0r + b*cr[1](I,axis,axis2)*rc1r
                           +(c[1](I,axis)-c[0](I,axis))*br;
      }
    }
  }
}


//=================================================================================
// get a mapping from the database
//=================================================================================
int FilletMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering FilletMapping::get" << endl;
  subDir.setMode(GenericDataBase::streamInputMode);

  subDir.get( FilletMapping::className,"className" ); 
  if( FilletMapping::className != "FilletMapping" )
  {
    cout << "FilletMapping::get ERROR in className!" << endl;
  }
  int temp;
  subDir.get(temp,"filletType"); filletType=(FilletTypeEnum&)temp;
  subDir.get(temp,"joinType"); joinType=(JoinTypeEnum&)temp;
  subDir.get(intersectionFound,"intersectionFound");
  
  subDir.get(intersectionToUse,"intersectionToUse");
  subDir.get(filletWidth,"filletWidth");
  subDir.get(filletOverlap,"filletOverlap");
  subDir.get(orient,"orient");
  subDir.get(blendingFactor,"blendingFactor");
  subDir.get(newCurves,"newCurves");

  

  int tempArray[2];
  subDir.get(tempArray,"offsetType",2);  
  offsetType[0]=(OffsetTypeEnum)tempArray[0]; offsetType[1]=(OffsetTypeEnum)tempArray[1];
  
  // get the curves that we use 
  char buff[40];
  aString curveClassName;
  int i;
  for( i=0; i<2; i++ )
  {
    int curveExists=-1;
    subDir.get(curveExists,sPrintF(buff,"curve%iExists",i));
    assert( curveExists==0 || curveExists==1 );
    if( curveExists )
    { 
      subDir.get(curveClassName,sPrintF(buff,"curve%iClassName",i));
      curve[i] = Mapping::makeMapping( curveClassName ); // ***** this does a new -- who will delete? ***
      if( curve[i]==NULL )
      {
	cout << "FilletMapping::get:ERROR unable to make the mapping with className = " 
	  << (const char *)curveClassName << endl;
	return 1;
      }
      curve[i]->get( subDir,sPrintF(buff,"curve%i",i) );
      curve[i]->incrementReferenceCount();
    }
    else
    {
      cout << "FilletMapping::get:ERROR unable to find all the curves associated with this Mapping!\n";
      return 1;
    }
  }
  // get curves of intersection
  for( i=0; i<2; i++ )
  {
    int curveExists=-1;
    subDir.get(curveExists,sPrintF(buff,"s%iExists",i));
    assert( curveExists==0 || curveExists==1 );
    if( curveExists )
    { 
      subDir.get(curveClassName,sPrintF(buff,"s%iClassName",i));
      s[i] = Mapping::makeMapping( curveClassName ); 
      if( s[i]==NULL )
      {
	cout << "FilletMapping::get:ERROR unable to make the intersection curve with className = " 
	  << (const char *)curveClassName << endl;
	return 1;
      }
      s[i]->get( subDir,sPrintF(buff,"s%i",i) );
    }
    else
    {
      cout << "FilletMapping::get:ERROR unable to find all the curves associated with this Mapping!\n";
      return 1;
    }
  }

  int blendingFunctionExists=-1;
  subDir.get(blendingFunctionExists,sPrintF(buff,"blendingFunctionExists"));
  assert( blendingFunctionExists==0 || blendingFunctionExists==1 );
  if( blendingFunctionExists )
  { 
    subDir.get(curveClassName,"curveClassName");
    blendingFunction = Mapping::makeMapping( curveClassName ); // ***** this does a new -- who will delete? ***
    if( blendingFunction==NULL )
    {
      cout << "FilletMapping::get:ERROR unable to make the blending function mapping with className = " 
	   << (const char *)curveClassName << endl;
      return 1;
    }
    blendingFunction->get( subDir,sPrintF(buff,"blendingFunction") );
  }
  else
  {
    cout << "FilletMapping::get:ERROR unable to find the blending function associated with this Mapping!\n";
    return 1;
  }

  Mapping::get( subDir, "Mapping" );
  mappingHasChanged();

  delete &subDir;

  return 0;
}

int FilletMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 
  
  // save the mapping as a stream of data by default, this is more efficient
  subDir.setMode(GenericDataBase::streamOutputMode);

  subDir.put( FilletMapping::className,"className" );

  subDir.put((int)filletType,"filletType"); 
  subDir.put((int)joinType,"joinType"); 
  subDir.put(intersectionFound,"intersectionFound");
  
  subDir.put(intersectionToUse,"intersectionToUse");
  subDir.put(filletWidth,"filletWidth");
  subDir.put(filletOverlap,"filletOverlap");
  subDir.put(orient,"orient");
  subDir.put(blendingFactor,"blendingFactor");
  subDir.put(newCurves,"newCurves");
  int tempArray[2];
  tempArray[0]=offsetType[0]; tempArray[1]=offsetType[1];
  subDir.put(tempArray,"offsetType",2);

  // save the curves that we use *** this could be wasteful is they are already saved ****
  char buff[40];
  int i;
  for( i=0; i<2; i++ )
  {
    int curveExists=   curve[i]!=NULL ? 1 : 0;
    subDir.put(curveExists,sPrintF(buff,"curve%iExists",i));
    if( curveExists )
    {
      subDir.put(curve[i]->getClassName(),sPrintF(buff,"curve%iClassName",i));
      curve[i]->put( subDir,sPrintF(buff,"curve%i",i) );
    }
  }
  // save the intersection curves
  for( i=0; i<2; i++ )
  {
    int curveExists=   s[i]!=NULL ? 1 : 0;
    subDir.put(curveExists,sPrintF(buff,"s%iExists",i));
    if( curveExists )
    {
      subDir.put(s[i]->getClassName(),sPrintF(buff,"s%iClassName",i));
      s[i]->put( subDir,sPrintF(buff,"s%i",i) );
    }
  }

  int blendingFunctionExists=   blendingFunction!=NULL ? 1 : 0;
  subDir.put(blendingFunctionExists,sPrintF(buff,"blendingFunctionExists"));
  if( blendingFunctionExists )
  {
    subDir.put(blendingFunction->getClassName(),"curveClassName");
    blendingFunction->put( subDir,sPrintF(buff,"blendingFunction") );
  }

  Mapping::put( subDir, "Mapping" );

  delete &subDir;
  return 0;
}

Mapping *FilletMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==FilletMapping::className )
    retval = new FilletMapping();
  return retval;
}

    

int FilletMapping::
update( MappingInformation & mapInfo ) 
//=====================================================================================
/// \brief  Interactively create and/or change the Fillet mapping.
/// \param mapInfo (input): Holds a graphics interface to use.
//=====================================================================================
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  DialogData *interface = mapInfo.interface;

  aString prefix = "FILLET:"; // prefix for commands to make them unique.

  const bool executeCommand = mapInfo.commandOption==MappingInformation::readOneCommand;
  aString command = !executeCommand ? aString("") : *mapInfo.command;
  
  bool plotSurfaces=TRUE;   // plot the two intersecting surfaces
  
  char buff[180];  // buffer for sPrintF
  aString menu[] = 
    {
      "!FilletMapping",
      "compute fillet",
      "choose curves",
      ">orient fillet",
        "orient curve 1- to curve 2-",
        "orient curve 1+ to curve 2-",
        "orient curve 1- to curve 2+",
        "orient curve 1+ to curve 2+",
      "<width",
      "overlap",
      "blending factor",
      "choose intersection",
      "plot intersecting surfaces (toggle)",
      "plot surface mappings",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "check",
      "show parameters",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "compute fillet     : recompute fillet with current parameters",
      "choose curves",
      "orient fillet   ",
      "width              : define width of the fillet",
      "overlap            : define the overlap of the fillet",
      "blending factor    : coefficient in tanh blending function",
      "choose intersection: choose which intersection point/curve to use",
      "plot intersecting surfaces (toggle) : ",
      "plot surface mappings: plot mappings that live on each intersecting surface",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "check              : check this mapping",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  GUIState gui;
  gui.setWindowTitle("Fillet Mapping");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;

  if( interface==NULL || command=="build dialog" )
  {
    // Make a menu with the Mapping names (only curves or surfaces!)
    int numberOfMaps=mapInfo.mappingList.getLength();
    int numberOfFaces=numberOfMaps*6;  // up to 6 sides per grid
    aString *label = new aString[numberOfFaces+2];
    int i, j=0;
    for( i=0; i<numberOfMaps; i++ )
    {
      MappingRC & map = mapInfo.mappingList[i];
      if( (map.getDomainDimension()== (map.getRangeDimension()-1) && map.mapPointer!=this) ||
	  (map.getDomainDimension()==map.getRangeDimension() && map.getDomainDimension()>1 ) )
      {
	if( map.getDomainDimension()==map.getRangeDimension()-1 )
	{
	  label[j++]=map.getName(mappingName);
	}
	else
	{
	  // include all sides that are physical boundaries.
	  for( int axis=axis1; axis<map.getDomainDimension(); axis++ )
	  {
	    for( int side=Start; side<=End; side++ )
	    {
	      if( map.getBoundaryCondition(side,axis)>0 )
	      {
		label[j++]=sPrintF(buff,"%s (side=%i,axis=%i)",(const char *)map.getName(mappingName),side,axis);
	      }
	    }
	  }
	}
      }
    }
    if( j==0 )
    {
      gi.outputString("FilletMapping::WARNING: There are no appropriate curves/surfaces to choose from");
    }
    label[j++]="none"; 
    label[j]="";   // null string terminates the menu
    int numberOfCurves=j;

    dialog.setOptionMenuColumns(1);

    aString *cmd = new aString[numberOfCurves+1];
    cmd[numberOfCurves]="";
    for( i=0; i<numberOfCurves; i++ )
      cmd[i]="Start curve 1:" + label[i];
    dialog.addOptionMenu("Start curve 1:", cmd,label,numberOfCurves-1);

    for( i=0; i<numberOfCurves; i++ )
      cmd[i]="Start curve 2:" + label[i];
    dialog.addOptionMenu("Start curve 2:", cmd,label,numberOfCurves-1);


    delete [] label;
    delete [] cmd;
    
    aString opLabel[] = {"orient curve 1- to curve 2-",
			 "orient curve 1+ to curve 2-",
			 "orient curve 1- to curve 2+",
			 "orient curve 1+ to curve 2+", "" };

    // addPrefix(label,prefix,cmd,maxCommands);
    dialog.addOptionMenu("orientation:", opLabel,opLabel,0);

    aString opLabel0[] = {"fillet",
			 "collar", "" };

    // addPrefix(label,prefix,cmd,maxCommands);
    dialog.addOptionMenu("join type:", opLabel0,opLabel0,0);

    aString opLabel1[] = {"parameter space fillet",
 			  "physical space fillet",
                          "" };

    // addPrefix(label,prefix,cmd,maxCommands);
    dialog.addOptionMenu("type:", opLabel1,opLabel1,(int)filletType);

    aString opLabel2[] = {"hyperbolic",
			  "offset curve",
                          ""     };
    aString opCmd[3];
    opCmd[0]="curve 1 offset type:"+opLabel2[0];
    opCmd[1]="curve 1 offset type:"+opLabel2[1];
    opCmd[2]="";
    // addPrefix(label,prefix,cmd,maxCommands);
    dialog.addOptionMenu("curve 1 offset type:", opCmd,opLabel2,0);

    opCmd[0]="curve 2 offset type:"+opLabel2[0];
    opCmd[1]="curve 2 offset type:"+opLabel2[1];
    opCmd[2]="";
    // addPrefix(label,prefix,cmd,maxCommands);
    dialog.addOptionMenu("curve 2 offset type:", opCmd,opLabel2,0);

    // dialog.getOptionMenu(3).setSensitive(surfaceGrid==true);

    aString pbLabels[] = {"compute fillet",
                          "edit intersection curve",
			  ""};
    // addPrefix(pbLabels,prefix,cmd,maxCommands);
    int numRows=2;
    dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

    // dialog.setSensitive(surfaceGrid==true,DialogData::pushButtonWidget,3);

    aString tbCommands[] = {"plot initial curves",
			    ""};
    int tbState[2];
    tbState[0] = plotSurfaces;
    tbState[1] = 0;
    int numColumns=1;
    dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

    const int numberOfTextStrings=5;
    aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

    int nt=0;
    textLabels[nt] = "width"; 
    sPrintF(textStrings[nt], "%g", filletWidth); nt++; 

    textLabels[nt] = "overlap"; 
    sPrintF(textStrings[nt], "%g",filletOverlap); nt++; 

    textLabels[nt] = "blending factor"; 
    sPrintF(textStrings[nt], "%g",blendingFactor); nt++; 

    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );

    // addPrefix(textLabels,prefix,cmd,maxCommands);
    dialog.setTextBoxes(textLabels, textLabels, textStrings);

    gui.buildPopup(menu);

  }
  if( !executeCommand  )
  {
    gi.pushGUI(gui);
    gi.appendToTheDefaultPrompt("FilletMapping>"); // set the default prompt
  }

  aString answer,line,answer2; 

  bool plotObject= curve[0]!=NULL && curve[1]!=NULL;
  newCurves= uninitialized &&  curve[0]!=NULL && curve[1]!=NULL;
      
  GraphicsParameters params;
  params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  int len=0;
  
  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getAnswer(answer,"");
 

    if( answer=="choose curves" )
    { 
      // include all curves/surfaces and all faces of voulme grids with bc>0

      newCurves=TRUE;  // tells initialize to recompute the intersection
      // Make a menu with the Mapping names (only curves or surfaces!)
      int numberOfMaps=mapInfo.mappingList.getLength();
      int numberOfFaces=numberOfMaps*6;  // up to 6 sides per grid
      aString *menu2 = new aString[numberOfFaces+2];
      IntegerArray subListNumbering(numberOfFaces);
      int i, j=0;
      for( i=0; i<numberOfMaps; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( (map.getDomainDimension()== (map.getRangeDimension()-1) && map.mapPointer!=this) ||
            (map.getDomainDimension()==map.getRangeDimension() && map.getDomainDimension()>1 ) )
	{
          if( map.getDomainDimension()==map.getRangeDimension()-1 )
	  {
    	    subListNumbering(j)=i;
            menu2[j++]=map.getName(mappingName);
          }
	  else
	  {
            // include all sides that are physical boundaries.
	    for( int axis=axis1; axis<map.getDomainDimension(); axis++ )
	    {
	      for( int side=Start; side<=End; side++ )
	      {
                if( map.getBoundaryCondition(side,axis)>0 )
		{
  	          subListNumbering(j)=i;
                  menu2[j++]=sPrintF(buff,"%s (side=%i,axis=%i)",(const char *)map.getName(mappingName),side,axis);
		}
	      }
	    }
          }
	}
      }
      if( j==0 )
      {
	gi.outputString("FilletMapping::WARNING: There are no appropriate curves/surfaces to choose from");
        continue;
      }
      menu2[j++]="none"; 
      menu2[j]="";   // null string terminates the menu
	
      aString choice[] = {"choose curve 1",
                         "choose curve 2"
                        }; 
      for( i=0; i<=1; i++ )
      {
        aString prompt = choice[i];
        int mapNumber = gi.getMenuItem(menu2,answer2,prompt);
        mapNumber=subListNumbering(mapNumber);  // map number in the original list
        if( mapInfo.mappingList[mapNumber].mapPointer==this )
        {
    	  cout << "FilletMapping::ERROR: you cannot use this mapping, this would be recursive!\n";
          continue;
        }
        Mapping & map =* mapInfo.mappingList[mapNumber].mapPointer;

	if( curve[i]!=NULL && curve[i]->decrementReferenceCount()==0 )
	  delete curve[i];

	if( map.getDomainDimension()==map.getRangeDimension()-1 )
	{
          curve[i]=mapInfo.mappingList[mapNumber].mapPointer;
	  if( !curve[i]->uncountedReferencesMayExist() ) 
	    curve[i]->incrementReferenceCount();
          
	}
	else if( map.getDomainDimension()==map.getRangeDimension() )
	{
	  // we need to build a Mapping taht corresponds to a side of a volume grid.
          int side=-1, axis=-1;
          // sScanF(answer2,"(side=%i axis=%i)",&side,&axis); // remember that commas are removed
          int length=answer2.length();
	  for( int j=0; j<length-6; j++ )
	  {
	    if( answer2(j,j+5)=="(side=" ) 
	    {
              sScanF(answer2(j,length-1),"(side=%i axis=%i",&side,&axis); // remember that commas are removed
              break;
	    }
	  }
          if( side<0 || axis<0 )
	  {
	    cout << "Error getting (side,axis) from choice!\n";
	    throw "error";
 	  }
          printf(" create a mapping for (side,axis)=(%i,%i) \n",side,axis);
	  curve[i]= new ReductionMapping(map,axis,side); 
          curve[i]->incrementReferenceCount();
	}
	else
	{
	  throw "error";
	}
      }
      
      delete [] menu2;
      if( answer2=="none" )
        continue;
      // Define properties of this mapping
      setDomainDimension(curve[0]->getDomainDimension());
      setRangeDimension(curve[0]->getRangeDimension());
      for( int axis=0; axis<domainDimension; axis++ )
        setGridDimensions(axis,21);

      uninitialized=TRUE;
      plotObject=TRUE;
    }
    else if( answer.matches("Start curve 1:") || answer.matches("Start curve 2:") )
    {
      len= answer.matches("Start curve 1:");
      int curveNumber;
      if( len>0 )
	curveNumber=0;
      else
      {
        len=answer.matches("Start curve 2:");
	curveNumber=1;
      }
      aString name = answer(len,answer.length()-1);

      Mapping *mapPointer=NULL;
      int numberOfMaps=mapInfo.mappingList.getLength();
      int choice=-1;
      for( int i=0; i<numberOfMaps && mapPointer==NULL; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( (map.getDomainDimension()== (map.getRangeDimension()-1) && map.mapPointer!=this) ||
	    (map.getDomainDimension()==map.getRangeDimension() && map.getDomainDimension()>1 ) )
	{
	  if( map.getDomainDimension()==map.getRangeDimension()-1 )
	  {
            choice++;
	    if( name==map.getName(mappingName) )
	    {
	      mapPointer=&map.getMapping();
	      break;
	    }
	  }
	  else
	  {
	    // include all sides that are physical boundaries.
	    for( int axis=axis1; axis<map.getDomainDimension() && mapPointer==NULL; axis++ )
	    {
	      for( int side=Start; side<=End; side++ )
	      {
		if( map.getBoundaryCondition(side,axis)>0 )
		{
                  choice++;
		  if( name==sPrintF(buff,"%s (side=%i,axis=%i)",(const char *)map.getName(mappingName),side,axis))
		  {
                    mapPointer = new ReductionMapping(map.getMapping(),axis,side); 
                    mapPointer->incrementReferenceCount();
		    break;
		  }
		}
	      }
	    }
	  }
	}
      }
      if( mapPointer!=NULL )
      {
        dialog.getOptionMenu(curveNumber).setCurrentChoice(choice);
        newCurves=TRUE; 
        plotObject=TRUE;
        curve[curveNumber]=mapPointer;
        if( curveNumber==0 )
	{
	  // Define properties of this mapping
	  setDomainDimension(curve[0]->getDomainDimension());
	  setRangeDimension(curve[0]->getRangeDimension());
	  for( int axis=0; axis<domainDimension; axis++ )
	    setGridDimensions(axis,21);
	}
	uninitialized=TRUE;
      }
      else if( name!="none" )
      {
	printf("Unknown curve! name=[%s]\n",(const char*)name);
      }
      
    }
    else if( answer.matches("compute fillet") )
    {
      if( curve[0]==NULL || curve[1]==NULL )
      {
        printf("You must first choose the two intersecting curves (or surfaces)\n");
      }
      else if( uninitialized )
      {
        initialize();
        mappingHasChanged(); 
      }
      else
        printf("FilletMapping:INFO: No need to recompute fillet\n");
    }
    else if( answer=="width" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the fillet width (current=(%e)): ",filletWidth));
      if( line!="" ) 
	sScanF(line,"%e",&filletWidth);
      uninitialized=TRUE;
    }
    else if( answer=="overlap" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the fillet overlap (current=(%e)): ",filletOverlap));
      if( line!="" ) 
	sScanF(line,"%e",&filletOverlap);
      uninitialized=TRUE;
    }
    else if( answer=="blending factor" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the tanh blending factor (current=(%e)): ",blendingFactor));
      if( line!="" ) 
      {
	sScanF(line,"%e",&blendingFactor);
        printf(" blending factor =%e\n",blendingFactor);
      }

      uninitialized=TRUE; // no need to recompute
    }
    else if( (len=answer.matches("width")) )
    {
      sScanF(answer(len,answer.length()-1),"%e",&filletWidth);
      dialog.setTextLabel(0,sPrintF(answer2,"%g",filletWidth));
      uninitialized=TRUE;
    }
    else if( (len=answer.matches("overlap")) )
    {
      sScanF(answer(len,answer.length()-1),"%e",&filletOverlap);
      dialog.setTextLabel(1,sPrintF(answer2,"%g",filletOverlap));
      uninitialized=TRUE;
    }
    else if( (len=answer.matches("blending factor")) )
    {
      sScanF(answer(len,answer.length()-1),"%e",&blendingFactor);
      printf(" blending factor =%e\n",blendingFactor);
      dialog.setTextLabel(2,sPrintF(answer2,"%g",blendingFactor));

      uninitialized=TRUE; // no need to recompute
    }
    else if( answer=="choose intersection" )
    {
      gi.inputString(line,sPrintF(buff,"Choose intersection point/curve (0,...,%i) (current=(%i)): ",
              numberOfIntersections-1,intersectionToUse));
      if( line!="" ) 
	sScanF(line,"%i",&intersectionToUse);
      uninitialized=TRUE;
    }
    else if( answer(0,5)=="orient" )
    {
      int response=-1;
      if( answer=="orient curve 1- to curve 2-" )
        response=0;
      else if( answer=="orient curve 1+ to curve 2-" )
        response=1;
      else if( answer=="orient curve 1- to curve 2+" )
        response=2;
      else if( answer=="orient curve 1+ to curve 2+" )
        response=3;

      if( response >= 0 && response <= 3 )
      {
        orient=response;
        uninitialized=TRUE;

        dialog.getOptionMenu("orientation:").setCurrentChoice(response);
      }
    }
    else if( answer=="edit intersection curve" )
    {
      inter.update(mapInfo);
      uninitialized=TRUE;
    }
    else if( answer=="parameter space fillet" )
    {
      filletType=parametric;
      dialog.getOptionMenu("type:").setCurrentChoice((int)filletType);
      uninitialized=TRUE;
    }
    else if( answer=="physical space fillet" )
    {
      filletType=nonParametric;
      dialog.getOptionMenu("type:").setCurrentChoice((int)filletType);
      uninitialized=TRUE;
    }
    else if( (len=answer.matches("curve 1 offset type:")) || answer.matches("curve 2 offset type:") )
    {
      int c;
      if( len>0 )
      {
	c=0;
      }
      else
      {
	c=1;
	len=answer.matches("curve 2 offset type:");
      }
      
      aString name = answer(len,answer.length()-1);
      int choice;
      if( name.matches("hyperbolic") )
      {
	choice=0;
	offsetType[c]=hyperbolicOffset;
      }
      else
      {
	choice=1;
        offsetType[c]=offsetCurve;
      }
      dialog.getOptionMenu(4+c).setCurrentChoice(choice);
      uninitialized=TRUE;
    }
    else if( answer=="fillet" )
    {
      joinType=fillet;
      dialog.getOptionMenu("join type:").setCurrentChoice((int)joinType);
      uninitialized=TRUE;
    }
    else if( answer=="collar" )
    {
      joinType=collar;
      dialog.getOptionMenu("join type:").setCurrentChoice((int)joinType);
      uninitialized=TRUE;
    }
    else if( answer=="show parameters" )
    {
      printf(" filletWidth = %e, filletOverlap = %e \n",filletWidth,filletOverlap);
      display();
    }
    else if( answer=="plot intersecting surfaces (toggle)" )
    {
      plotSurfaces=!plotSurfaces;
    }
    else if( (len=answer.matches("plot initial curves")) )
    {
      int toggle;
      sScanF(answer(len,answer.length()-1),"%i",&toggle);       
      plotSurfaces=toggle;
      dialog.setToggleState(0,toggle);
      printf(" plotSurfaces=%i\n",plotSurfaces);
      
    }
    else if( answer=="plot surface mappings" )
    {
      if( domainDimension==2 && curve[0]!=NULL && curve[1]!=NULL )
      {
        if(  s[0]==NULL || s[1]==NULL )
          initialize();
	
        // plot the grids in the parameter spaces for debugging
        params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
        params.set(GI_USE_PLOT_BOUNDS,FALSE); 

        for( int i=0; i<=1; i++ )
	{
          gi.erase();
          params.set(GI_TOP_LABEL,s[i]->getName(mappingName));
          PlotIt::plot(gi,*s[i],params);
	}
      }
      else
      {
        gi.outputString("Sorry: no grids to plot");
      }
    }
    else if( answer=="plot" )
    {
      plotObject=TRUE;
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity"  ||
             answer=="check" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=TRUE;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }

    if( plotObject )
    {
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      gi.erase();

      if( intersectionFound )
      {
        printf("plot the fillet... \n");
	
        params.set(GI_TOP_LABEL,getName(mappingName));
        params.set(GI_MAPPING_COLOUR,"green");
        PlotIt::plot(gi,*this,params);  
      }
      
      if( plotSurfaces )
      {
	// params.set(GI_PLOT_SHADED_SURFACE,FALSE);

        params.set(GI_SURFACE_OFFSET,(real)20.);  // offset the surfaces so we see the fillet better
	params.set(GI_TOP_LABEL,"");
	if( curve[0]!=NULL )
	{
	  params.set(GI_MAPPING_COLOUR,"red");
	  PlotIt::plot(gi,*curve[0],params);   
	}
	if( curve[1]!=NULL )
	{
	  params.set(GI_MAPPING_COLOUR,"blue");
	  PlotIt::plot(gi,*curve[1],params);   
	}
        params.set(GI_SURFACE_OFFSET,(real)3.);   // reset to default

	// params.set(GI_PLOT_SHADED_SURFACE,TRUE);
      }
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);

//      gi.erase();
//      PlotIt::plot(gi,*blendingFunction);
    }
  }
  gi.erase();
  if( !executeCommand  )
  {
    gi.popGUI();
    gi.unAppendTheDefaultPrompt();
  }
  return 0;
  
}
