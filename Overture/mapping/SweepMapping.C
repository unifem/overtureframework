#include "SweepMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "display.h"
// include <float.h>

//--------------------------------------------------------------------------
//  Define a mapping by sweeping one mapping along
//  a curve represented by another mapping
//--------------------------------------------------------------------------

SweepMapping::
SweepMapping(Mapping *sweepmap /* = NULL */,
	     Mapping *dirsweepmap /* = NULL */,
	     Mapping *scale /* = NULL */ ,
	     const int domainDimension0 /* =3 */ )
  : Mapping(domainDimension0,3,parameterSpace,cartesianSpace)   
//===========================================================================
/// \details  Define a sweep mapping or an extruded mapping.
/// 
///  Build a mapping defined by a sweep surface or curve
///  (a mapping with domainDimension=2 rangeDimension=3 or 
///  domainDimension=1, rangeDimension=3) and a sweep
///  curve or line (domainDimension=1, rangeDimension=3).
/// 
/// \param sweepmap (input) : is the mapping for the sweep surface or curve; default: an
///             annulus with inner radius=0 and outer radius=1
/// \param dirsweepmap (input) : The mapping for the sweep curve; default: a half circle
///               of radius=4.
/// \param scale (input) : to scale up $(>1)$ or down $(0<s<1)$; default $1$.
///  
/// \param Author: Thomas Rutaganira. 
/// \param Changes: WDH + AP
//===========================================================================
{ 
  initialize();
  setName( Mapping::mappingName,"Sweep");

  orientation=1.;

  sweepType=sweep;
  za=0.;
  zb=1.;
  
  setGridDimensions( axis2,11 );
  setGridDimensions( axis1,21 );
  setGridDimensions( axis3,11 );

  centeringOption=useCenterOfSweepSurface;
  center[0]=center[1]=center[2]=0.;
  
  sweepMap=sweepmap;
  if( sweepMap && !sweepMap->uncountedReferencesMayExist() )
  {
    sweepMap->incrementReferenceCount();
  }

  dirSweepMap=dirsweepmap;
  if( dirSweepMap && !dirSweepMap->uncountedReferencesMayExist() )
  {
    dirSweepMap->incrementReferenceCount();
  }

  scaleSpline = scale;
  if( scaleSpline!=NULL )
  {
    scaleSpline->uncountedReferencesMayExist();
    scaleSpline->incrementReferenceCount();
  }
  
  rowSpline0 = NULL;
  rowSpline1 = NULL;
  rowSpline2 = NULL;
 
/* ----
  if( scaleSpline==NULL )  // ***** fix this *** may not need this
  {
    //Initialize the scaleSpline to 1.
    scaleSpline=new SplineMapping(1);
    scaleSpline->incrementReferenceCount();
    realArray xScale;
    xScale.redim(42);
    xScale=1.;
    ((SplineMapping*)scaleSpline)->setPoints(xScale);
  }
--- */
  if( dirSweepMap!=NULL && sweepMap!=NULL ) 
    setMappingProperties();

}

int SweepMapping::
initialize()
{
  SweepMapping::className="SweepMapping";
  sweepMap=NULL;
  dirSweepMap=NULL;
  scaleSpline=NULL;
  rowSpline0 = NULL;
  rowSpline1 = NULL;
  rowSpline2 = NULL;
  
  straightLine.redim(3,2);
  straightLine = 0;
  straightLine(0,1) = 1;
  
  return 0;
}


// Copy constructor is deep by default
SweepMapping::
SweepMapping( const SweepMapping & map, const CopyType copyType )
{
  initialize();

  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "SweepMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

SweepMapping::
~SweepMapping()
{ 
  if( (debug/4) % 2 )
   cout << " SweepMapping::Destructor called" << endl;
  if( sweepMap!=NULL && sweepMap->decrementReferenceCount()==0 )
    delete sweepMap;
  if( dirSweepMap!=NULL && dirSweepMap->decrementReferenceCount()==0 )
    delete dirSweepMap;

  if( rowSpline0!=NULL && rowSpline0->decrementReferenceCount()==0 ) 
    delete rowSpline0;
  if( rowSpline1!=NULL && rowSpline1->decrementReferenceCount()==0 )  
    delete rowSpline1;
  if( rowSpline2!=NULL && rowSpline2->decrementReferenceCount()==0 )  
    delete rowSpline2;

  if( scaleSpline!=NULL && scaleSpline->decrementReferenceCount()==0 )
    delete scaleSpline;
}


SweepMapping & SweepMapping::
operator =( const SweepMapping & x )
{
  if( SweepMapping::className != x.getClassName() )
  {
    cout << "SweepMapping::operator= ERROR trying to set a SweepMapping = to a" 
      << " mapping of type " << x.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(x);            // call = for derivee class

  if( sweepMap!=NULL && sweepMap->decrementReferenceCount()==0 )
    delete sweepMap;
  sweepMap          = x.sweepMap;
  if( sweepMap!=NULL )
    sweepMap->incrementReferenceCount();

  if( dirSweepMap!=NULL && dirSweepMap->decrementReferenceCount()==0 )
    delete dirSweepMap;
  dirSweepMap          = x.dirSweepMap;
  if( dirSweepMap!=NULL )
    dirSweepMap->incrementReferenceCount();

  if( rowSpline0!=NULL && rowSpline0->decrementReferenceCount()==0 )
    delete rowSpline0;
  rowSpline0          = x.rowSpline0;
  if( rowSpline0!=NULL )
    rowSpline0->incrementReferenceCount();

  if( rowSpline1!=NULL && rowSpline1->decrementReferenceCount()==0 )
    delete rowSpline1;
  rowSpline1          = x.rowSpline1;
  if( rowSpline1!=NULL )
    rowSpline1->incrementReferenceCount();

  if( rowSpline2!=NULL && rowSpline2->decrementReferenceCount()==0 )
    delete rowSpline2;
  rowSpline2          = x.rowSpline2;
  if( rowSpline2!=NULL )
    rowSpline2->incrementReferenceCount();

  if( scaleSpline!=NULL && scaleSpline->decrementReferenceCount()==0 )
    delete scaleSpline;
  scaleSpline          = x.scaleSpline;
  if( scaleSpline!=NULL )
    scaleSpline->incrementReferenceCount();

  centeringOption    = x.centeringOption;
  sweepType          = x.sweepType;
  orientation        = x.orientation;

  za                 = x.za;
  zb                 = x.zb;
  
  straightLine       = x.straightLine;

  for( int axis=0; axis<3; axis++ )
    center[axis]=x.center[axis];
  
  return *this;
}

void SweepMapping::
setSweepSurface(Mapping *sweepmap)
//===========================================================================
/// \details  Specify the mapping to use as the sweepMap,
///                a 3D surface or a 3D curve. If it is a 3D
///                surface, the resulting SweepMapping will be a
///                3D volume and if it is a 3D curve, the SweepMapping
///                will be a 3D surface.
//===========================================================================
{
 sweepMap=sweepmap;
 if( sweepMap !=NULL )
 {
  sweepMap->uncountedReferencesMayExist();
  sweepMap->incrementReferenceCount();
 }

 if( sweepMap !=NULL && dirSweepMap !=NULL )
   findRowSplines();

}

int SweepMapping::
setCentering( CenteringOptionsEnum centering )
//===========================================================================
/// \details  Specify the centering.
/// \param centering (input) : Specify the manner in which the reference surface should be centered.
///    One of {\bf useCenterOfSweepSurface}, {\bf useCenterOfSweepCurve} or {\bf specifiedCenter}.
///    See the documentation for further details.
//===========================================================================
{
  centeringOption=centering;
  findRowSplines();
  return 0;
}

int SweepMapping::
setOrientation( real orientation_ /* =1. */ )
//===========================================================================
/// \details  Specify the orientation of the sweepmapping, +1 or -1.
///    When the sweep surface is rotated to align with the sweep curve it may
///  face in a forward or reverse direction depending on the orientation. Thus if a
///  swept surface appears `inside-out' one should change the orientation.
//===========================================================================
{
  orientation = orientation_>0. ? 1. : -1;
  if( sweepMap !=NULL && dirSweepMap !=NULL )
   findRowSplines();
  return 0;
}

int SweepMapping::
setExtrudeBounds(real za_ /* =0. */, 
                 real zb_ /* =1. */ )
//===========================================================================
/// \details  Specify the bounds on an extruded mapping.
/// \param za_,zb_ (input) : 
//===========================================================================
{
  za=za_;
  zb=zb_;
  sweepType=extrude;
  setBasicInverseOption(canInvert);  // basicInverse is available
  mappingHasChanged();
  return 0;
}

int SweepMapping::
setStraightLine(real lx /* =0. */, real ly /* =0. */, real lz /* =1. */)
//===========================================================================
/// \details  Specify the straight line of a tabulated cylinder mapping
/// \param lx,ly,lz (input) : 
//===========================================================================
{
  straightLine(0,1)=lx;
  straightLine(1,1)=ly;
  straightLine(2,1)=lz;

  sweepType=tabulatedCylinder;
  if (dirSweepMap)
  {
    setMappingProperties();
  }
  
  return 0;
}




void SweepMapping::
setSweepCurve(Mapping *dirsweepmap)
//===========================================================================
/// \details  Specify the mapping to use as the curve to
///                sweep along (a  3D curve).
//===========================================================================
{
  sweepType=sweep;
  dirSweepMap=dirsweepmap;
  if( dirSweepMap !=NULL )
  {
    dirSweepMap->uncountedReferencesMayExist();
    dirSweepMap->incrementReferenceCount();
  }
  if ((sweepMap != NULL)&&(dirSweepMap !=NULL))
  {
    findRowSplines();
  }
}

void SweepMapping::
setScale(Mapping *scale)
//===========================================================================
/// \details  Specify the mapping to use as the curve to
///                sweep along (a  3D curve).
//===========================================================================
{
 if( scaleSpline!=NULL && scaleSpline->decrementReferenceCount()==0 )
   delete scaleSpline;
 scaleSpline=scale;
 scaleSpline->uncountedReferencesMayExist();
 scaleSpline->incrementReferenceCount();
 
 if ((sweepMap != NULL)&&(dirSweepMap !=NULL))
   findRowSplines();

}

int SweepMapping::
setMappingProperties()
//===========================================================================
///  Access: protected.
/// \details  Initialize the parameters of the
///   sweep mapping. 
/// 
//===========================================================================
{
  
  switch(sweepType)
  {
  case tabulatedCylinder:
  {
    realArray r(1,1), x(1,3);
    r(0,0)=0;
    dirSweepMap->map(r,x);
    
    straightLine(0,0)=straightLine(0,1)-x(0,0);
    straightLine(1,0)=straightLine(1,1)-x(0,1);
    straightLine(2,0)=straightLine(2,1)-x(0,2);

    setRangeDimension(dirSweepMap->getRangeDimension());
    setDomainDimension(dirSweepMap->getDomainDimension()+1);

    setGridDimensions(axis1,max(dirSweepMap->getGridDimensions(axis1), 3));
    setGridDimensions(axis2,10);

    setIsPeriodic(axis1,dirSweepMap->getIsPeriodic(axis1));
    setIsPeriodic(axis2,notPeriodic);

    for( int side=0; side<=1; side++ )
    {
      if (getIsPeriodic(axis1))
	setBoundaryCondition(side,axis1,-1);
      else
	setBoundaryCondition(side,axis1,0);
      setBoundaryCondition(side,axis2,0);
    
      setShare(side,axis1,0);
      setShare(side,axis2,0);
    }
    
    break;
  }
  
    
  case sweep:
  case extrude:
  {
    setDomainDimension(sweepMap->getDomainDimension()+1);
    if( dirSweepMap!=NULL )
      setRangeDimension(dirSweepMap->getRangeDimension());
    else
      setRangeDimension(3);

    for( int axis=axis1; axis<domainDimension-1; axis++ )
    {
      // *wdh* setGridDimensions(axis,max(sweepMap->getGridDimensions(axis),getGridDimensions(axis)));
      setGridDimensions(axis,sweepMap->getGridDimensions(axis));
      for( int side=0; side<=1; side++ )
      {
	setBoundaryCondition(side,axis,sweepMap->getBoundaryCondition(side,axis));
	setShare(side,axis,sweepMap->getShare(side,axis));
      }
      setIsPeriodic(axis,sweepMap->getIsPeriodic(axis));
    }
    if(dirSweepMap!=NULL ) 
    {
      setGridDimensions(domainDimension-1,max(dirSweepMap->getGridDimensions(axis1),
					      getGridDimensions(domainDimension-1)));

      setIsPeriodic(axis3,dirSweepMap->getIsPeriodic(axis1) );  // set periodicity *wdh* 021230
      
      findRowSplines();
    }
    break;
  }
  
  default:
    ;
  }
  
  if( sweepType==extrude )
    setBasicInverseOption(canInvert);  // basicInverse is available
  else
    setBasicInverseOption(canDoNothing);

  mappingHasChanged();
  
  return 0;
}



void SweepMapping::
crossProduct(real *v1, real *v2, real *res)
{
  res[0]=v1[1]*v2[2]-v1[2]*v2[1];
  res[1]=-(v1[0]*v2[2]-v2[0]*v1[2]);
  res[2]=v1[0]*v2[1]-v2[0]*v1[1];
}

real SweepMapping::
dotProduct(real *v1, real *v2)
{
  return(v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2]);
}


void SweepMapping ::
findRowSplines(void)
//================================================================
/// \details 
///  This function initializes the splines rowSpline0, 1, 2 that will
///  gives the matrix transformation as well as its derivatives for
///  the mapping calculations. A point of the spline gives
///  a row for the matrix transformation. 
//=================================================================
{
  setBasicInverseOption(canDoNothing);

  Index I1 = Range(0,41); // compute the splines using 42 points
  realArray Point0(I1,3), Point1(I1,3), Point2(I1,3); //Stores the points to defi ne the rotations 
  real n0[3], n1[3], n2[3]; //Stores the new basis vectors
  real rot0[3], rot1[3], rot2[3]; //Stores the rotation coeff around the second basis vector
  real n0T[3], n1T[3], n2T[3]; //Stores the transpose of the new bases
  real mat1[3][3];

  real vb[3];

  //The notation is not classical. It is written to comform to the way the
  //dot product is computed. This means that the vector are put in rows
  //instead of columns

  int itry, jtry, idim, jdim, ic1, ic2;
  real v1[3], v2[3], tangt[3], v0[3], norm2, normaleSurface[3];

  Index Itmp, Jtmp;
  int ntotal, i, j, k;
  realArray rS1, xS1;

  if( rowSpline0 != NULL && rowSpline0->decrementReferenceCount()==0 ) delete rowSpline0;
  rowSpline0 = new SplineMapping(3); rowSpline0->incrementReferenceCount();
  rowSpline0->setParameterizationType(SplineMapping::index);
  if (rowSpline1 != NULL && rowSpline1->decrementReferenceCount()==0) delete rowSpline1;
  rowSpline1 = new SplineMapping(3); rowSpline1->incrementReferenceCount();
  rowSpline1->setParameterizationType(SplineMapping::index);
  if (rowSpline2 != NULL && rowSpline2->decrementReferenceCount()==0) delete rowSpline2;
  rowSpline2 = new SplineMapping(3); rowSpline2->incrementReferenceCount();
  rowSpline2->setParameterizationType(SplineMapping::index);

  if( getIsPeriodic(axis3))
  {
    rowSpline0->setIsPeriodic(axis1,derivativePeriodic);
    rowSpline1->setIsPeriodic(axis1,derivativePeriodic);
    rowSpline2->setIsPeriodic(axis1,derivativePeriodic);
  }
  
  // make the spline length to be 42  **********************
  rowSpline0->setGridDimensions(axis1,42);
  rowSpline1->setGridDimensions(axis1,42);
  rowSpline2->setGridDimensions(axis1,42);
  idim=getGridDimensions(axis1);
  if ((sweepMap->getDomainDimension())==2) 
    jdim=getGridDimensions(axis2);
  else 
    jdim=1;
  rS1.redim(idim,jdim,sweepMap->getDomainDimension());
// *wdh* xS1.redim(idim*jdim,sweepMap->getRangeDimension());
  xS1.redim(idim*jdim,3); xS1=0.;
  for (j=0;j<jdim;j++)
    rS1(Range(0,idim-1),j,0).seqAdd(0.,1./real(idim-1));
  if (jdim>1)
  {
    for (i=0;i<idim;i++)
      rS1(i,Range(0,jdim-1),1).seqAdd(0.,1./real(jdim-1));
  }

  rS1.reshape(idim*jdim,rS1.dimension(2));
  sweepMap->map(rS1,xS1);
  xS1.reshape(idim,jdim,xS1.dimension(1));

  // -- Find the center of the sweep surface or sweep curve --
  real sweepCenter[3];  //  *wdh* 110731
  if ((sweepMap->getIsPeriodic(axis1) == functionPeriodic) &&
      ( domainDimension==3 && (sweepMap->getIsPeriodic(axis2) == functionPeriodic)) )
  {
    Itmp=Range(0,idim-2),Jtmp=Range(0,jdim-2);
    ntotal=(idim-1)*(jdim-1);
  }
  else if (sweepMap->getIsPeriodic(axis1) == functionPeriodic){
    Itmp=Range(0,idim-2), Jtmp=Range(0,jdim-1);
    ntotal=(idim-1)*jdim;
  }
  else if (domainDimension==3 && (sweepMap->getIsPeriodic(axis2) == functionPeriodic) )
  {
    Itmp=Range(0,idim-1), Jtmp=Range(0,jdim-2);
    ntotal=idim*(jdim-1);
  }
  else {
    Itmp=Range(0,idim-1), Jtmp=Range(0,jdim-1);
    ntotal=idim*jdim;
  }

  for( int axis=0; axis<3; axis++ )
    sweepCenter[axis]=sum(xS1(Itmp,Jtmp,axis))/real(ntotal);


  if( centeringOption==useCenterOfSweepSurface )
  {
    for( int axis=0; axis<3; axis++ )
      center[axis]=sweepCenter[axis];
  }
  else if( centeringOption==useCenterOfSweepCurve )
  {
    for( int axis=0; axis<3; axis++ )
      center[axis]=0.;
  }
  else if( centeringOption==specifiedCenter )
  {
    // user specified center 
  }
  
 
  //The unit tangent to the
  //sweep curve
  realArray xtmp,rtmp,xrtmp, normXr;
  rtmp.redim(I1);
  normXr.redim(I1);
  xtmp.redim(I1,dirSweepMap->getRangeDimension());
  xrtmp.redim(I1,dirSweepMap->getRangeDimension(),
	      dirSweepMap->getDomainDimension());
  rtmp.seqAdd(0.,1./41.);
  dirSweepMap->map(rtmp,xtmp,xrtmp);
  Index Irangedim=Range(0,(dirSweepMap->getRangeDimension())-1);
  normXr(I1)=sqrt(xrtmp(I1,0,0)*xrtmp(I1,0,0)+
		  xrtmp(I1,1,0)*xrtmp(I1,1,0)+
		  xrtmp(I1,2,0)*xrtmp(I1,2,0));
  for (i=0;i<=Irangedim.getBound();i++)
    xrtmp(I1,i,0)/=normXr;

  // -- Find the normal to the reference surface ---
  //
  // WDH: we SHOULD just compute x.r on the surface and then form the normal 

  //Find two linearly independent vectors on the surface
  //  Note: xS1(idim,jdim,Rx) : holds points x on the reference surface
  itry=idim/2, jtry=jdim/2;
  ic1 =itry/2, ic2 =jtry/2;
  // *wdh
  v0[2]=0.; v1[2]=0.;
  if( domainDimension==3 )
  {
    // *wdh* 110731 Use the sweepCenter (not center) when computing the normal 
    for( i=0; i<sweepMap->getRangeDimension(); i++)
    {
      v0[i]=xS1(ic1 ,ic2 ,i)-sweepCenter[i];
      v1[i]=xS1(itry,jtry,i)-sweepCenter[i];
    }
    crossProduct(v0,v1,normaleSurface);
    norm2=sqrt(dotProduct(normaleSurface,normaleSurface));
   
    // printF("SweepMapping: INFO: surfaceNormal = (%8.2e,%8.2e,%8.2e)\n",
    //         normaleSurface[0],normaleSurface[1],normaleSurface[2]);
    

    if( norm2 < 10.*REAL_EPSILON)
    {
      // printF("SweepMapping: INFO: trouble: normal has length 0 : norm2=%9.3e\n",norm2);
      
      for( i=0; i<sweepMap->getRangeDimension(); i++)
      {
	v0[i]=xS1(ic1 ,jtry,i)-sweepCenter[i];
	v1[i]=xS1(itry,jtry,i)-sweepCenter[i];
      }
      crossProduct(v0,v1,normaleSurface);
      norm2=sqrt(dotProduct(normaleSurface,normaleSurface));

      // printF("SweepMapping: new surfaceNormal = (%8.2e,%8.2e,%8.2e)\n",
      //         normaleSurface[0],normaleSurface[1],normaleSurface[2]);

    }
  }
  else
  {
    // for a line, form a normal form the tangent vector
    for (i=0;i<sweepMap->getRangeDimension(); i++)
      v0[i]=xS1(ic1,ic2,i)-sweepCenter[i];

    if( rangeDimension==2 )
    {
      normaleSurface[0]=v0[1];
      normaleSurface[1]=v0[0];
    }
    else
    {
      real v0Min=min(fabs(v0[0]),fabs(v0[1]),fabs(v0[2]));
      if( fabs(v0[2]) == v0Min )
      {
        normaleSurface[0]=v0[1];
        normaleSurface[1]=v0[0];
        normaleSurface[2]=v0[2];
      }
      else if( fabs(v0[1]) == v0Min )
      {
        normaleSurface[0]=v0[2];
        normaleSurface[1]=v0[1];
        normaleSurface[2]=v0[0];
      }
      else 
      {
        normaleSurface[0]=v0[0];
        normaleSurface[1]=v0[2];
        normaleSurface[2]=v0[1];
      }
      
    }
    norm2=sqrt(dotProduct(normaleSurface,normaleSurface));

  }
  assert( norm2!=0. );
  
  //The unit normale to the surface is the first basis vector
  for (i=0;i<3;i++)
  {
    normaleSurface[i]/=norm2*orientation;
    n0[i]=normaleSurface[i];
    vb[i]=xrtmp(0,i,0);
    tangt[i]=n0[i];
  }

  printF("SweepMapping: new surfaceNormal = (%8.2e,%8.2e,%8.2e)\n",
         normaleSurface[0],normaleSurface[1],normaleSurface[2]);

  //The transpose of n0
  n0T[0]=n0[0];
  n1T[0]=n0[1];
  n2T[0]=n0[2];

  // Need to rotate the surface so that its normal coincide
  // with the tangent at 0
  crossProduct(n0,vb,n1);
  norm2=sqrt(dotProduct(n1,n1));
  if (norm2>10.*REAL_EPSILON)
  {
    for (i=0;i<3;i++) n1[i] /= norm2;

    n0T[1]=n1[0];
    n1T[1]=n1[1];
    n2T[1]=n1[2];
  
    crossProduct(n0,n1,n2);
    norm2=sqrt(dotProduct(n2,n2));
    for (i=0;i<3;i++) n2[i]/=norm2;

    rot0[0]=dotProduct(n0,vb);
    rot0[1]=0.;
    rot0[2]=-dotProduct(vb,n2);

    rot1[0]=0., rot1[1]=1., rot1[2]=0.;

    rot2[0]=-rot0[2], rot2[1]=0., rot2[2]=rot0[0];
    n0T[2]=n2[0];
    n1T[2]=n2[1];
    n2T[2]=n2[2];
  
    for (i=0;i<3;i++)
      tangt[i]=xrtmp(0,i);
  }
  else 
  {
    norm2=sqrt(dotProduct(v0,v0));
    for (i=0;i<3;i++)
      n1[i]=v0[i]/norm2;

    crossProduct(n0,n1,n2);
    norm2=sqrt(dotProduct(n2,n2));
    for (i=0;i<3;i++) n2[i]/=norm2;

    rot0[0]=1., rot0[1]=0., rot0[2]=0.;
    rot1[0]=0., rot1[1]=1., rot1[2]=0.;
    rot2[0]=0., rot2[1]=0., rot2[2]=1.;

    n0T[1]=n1[0];
    n1T[1]=n1[1];
    n2T[1]=n1[2];

    n0T[2]=n2[0];
    n1T[2]=n2[1];
    n2T[2]=n2[2];
  }

  mat1[0][0]=dotProduct(rot0,n0T);
  mat1[0][1]=dotProduct(rot0,n1T);
  mat1[0][2]=dotProduct(rot0,n2T);

  mat1[1][0]=dotProduct(rot1,n0T);
  mat1[1][1]=dotProduct(rot1,n1T);
  mat1[1][2]=dotProduct(rot1,n2T);

  mat1[2][0]=dotProduct(rot2,n0T);
  mat1[2][1]=dotProduct(rot2,n1T);
  mat1[2][2]=dotProduct(rot2,n2T);

  v0[0]=mat1[0][0], v0[1]=mat1[1][0], v0[2]=mat1[2][0];
  v1[0]=mat1[0][1], v1[1]=mat1[1][1], v1[2]=mat1[2][1];
  v2[0]=mat1[0][2], v2[1]=mat1[1][2], v2[2]=mat1[2][2];

  Point0(0,0)=dotProduct(n0T,v0);
  Point0(0,1)=dotProduct(n0T,v1);
  Point0(0,2)=dotProduct(n0T,v2);

  Point1(0,0)=dotProduct(n1T,v0);
  Point1(0,1)=dotProduct(n1T,v1);
  Point1(0,2)=dotProduct(n1T,v2);

  Point2(0,0)=dotProduct(n2T,v0);
  Point2(0,1)=dotProduct(n2T,v1);
  Point2(0,2)=dotProduct(n2T,v2);

  for (k=1;k<=41;k++)
  {
    for (i=0;i<=2;i++)
    {
      n0[i]=tangt[i];
      tangt[i]=xrtmp(k,i,0);
    }
    n0T[0]=n0[0];
    n1T[0]=n0[1];
    n2T[0]=n0[2];

    v0[0]=Point0(k-1,0), v0[1]=Point1(k-1,0), v0[2]=Point2(k-1,0);
    v1[0]=Point0(k-1,1), v1[1]=Point1(k-1,1), v1[2]=Point2(k-1,1);
    v2[0]=Point0(k-1,2), v2[1]=Point1(k-1,2), v2[2]=Point2(k-1,2);
    crossProduct(n0,tangt,n1);
    norm2=sqrt(dotProduct(n1,n1));
    if (norm2>10.*REAL_EPSILON)
    {      // Consider the else case
      for (i=0;i<3;i++) n1[i] /= norm2;
      n0T[1]=n1[0], n1T[1]=n1[1], n2T[1]=n1[2];

      crossProduct(n0,n1,n2);
      norm2=sqrt(dotProduct(n2,n2));
      for (i=0;i<3;i++) n2[i] /= norm2;

      n0T[2]=n2[0], n1T[2]=n2[1], n2T[2]=n2[2];

      //The rotation vectors
      rot0[0]=dotProduct(n0,tangt);
      rot0[1]=0.;
      rot0[2]=-dotProduct(tangt,n2);

      //printf("k=%i\t cos=%g\t sin=%g\n",k,rot0[0],rot0[2]);
      rot1[0]=0., rot1[1]=1., rot1[2]=0.;

      rot2[0]=-rot0[2], rot2[1]=0., rot2[2]=rot0[0];

      //Find the global transformation matrix. It is
      //according to our row vector notation
      // nT x rot x n  where x is the matrix multiplication

      mat1[0][0]=dotProduct(n0,v0);
      mat1[0][1]=dotProduct(n0,v1);
      mat1[0][2]=dotProduct(n0,v2);

      mat1[1][0]=dotProduct(n1,v0);
      mat1[1][1]=dotProduct(n1,v1);
      mat1[1][2]=dotProduct(n1,v2);

      mat1[2][0]=dotProduct(n2,v0);
      mat1[2][1]=dotProduct(n2,v1);
      mat1[2][2]=dotProduct(n2,v2);

      v0[0]=mat1[0][0], v0[1]=mat1[1][0], v0[2]=mat1[2][0];
      v1[0]=mat1[0][1], v1[1]=mat1[1][1], v1[2]=mat1[2][1];
      v2[0]=mat1[0][2], v2[1]=mat1[1][2], v2[2]=mat1[2][2];

      mat1[0][0]=dotProduct(rot0,v0);
      mat1[0][1]=dotProduct(rot0,v1);
      mat1[0][2]=dotProduct(rot0,v2);
 
      mat1[1][0]=dotProduct(rot1,v0);
      mat1[1][1]=dotProduct(rot1,v1);
      mat1[1][2]=dotProduct(rot1,v2);
 
      mat1[2][0]=dotProduct(rot2,v0);
      mat1[2][1]=dotProduct(rot2,v1);
      mat1[2][2]=dotProduct(rot2,v2);
 
      v0[0]=mat1[0][0], v0[1]=mat1[1][0], v0[2]=mat1[2][0];
      v1[0]=mat1[0][1], v1[1]=mat1[1][1], v1[2]=mat1[2][1];
      v2[0]=mat1[0][2], v2[1]=mat1[1][2], v2[2]=mat1[2][2];
 
      Point0(k,0)=dotProduct(n0T,v0);
      Point0(k,1)=dotProduct(n0T,v1);
      Point0(k,2)=dotProduct(n0T,v2);

      Point1(k,0)=dotProduct(n1T,v0);
      Point1(k,1)=dotProduct(n1T,v1);
      Point1(k,2)=dotProduct(n1T,v2);

      Point2(k,0)=dotProduct(n2T,v0);
      Point2(k,1)=dotProduct(n2T,v1);
      Point2(k,2)=dotProduct(n2T,v2);
    }
    else {
      for (i=0;i<3;i++){
	Point0(k,i)=Point0(k-1,i);
	Point1(k,i)=Point1(k-1,i);
	Point2(k,i)=Point2(k-1,i);
      }
    }
  }

  //Define points for the splines that will give matrix transformations
  // as well as it derivative along the third direction
  rowSpline0->setPoints(Point0(I1,0), Point0(I1,1), Point0(I1,2));
  rowSpline1->setPoints(Point1(I1,0), Point1(I1,1), Point1(I1,2));
  rowSpline2->setPoints(Point2(I1,0), Point2(I1,1), Point2(I1,2));

  mappingHasChanged();

}

void SweepMapping::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
{
#ifdef USE_PPP
  Overture::abort("SweepMapping::map: ERROR: fix me Bill!");
#else
  mapS(r,x,xr,params);
#endif
}


void SweepMapping::
mapS(const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params)
//==================================================================
/// \details  Use the transformations defined by rowSpline0, 
///  rowSpline1, and rowSpline2 and the additional scaling mapping 
///  to compute the image(s) and/or the derivatives for the parameter
///  point(s) defined by $r$.
//==================================================================
{
  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  if( sweepType==extrude )
  {
    // **************************************************
    // *************  Extrusion  ************************
    // **************************************************

    // Check that the pointer is there
    if( sweepMap==NULL )
    {
      printf("SweepMapping::map:ERROR: Sweep mapping has not been defined yet\n");
      return;
    }
    #ifdef USE_PPP
      sweepMap->mapS(r,x,xr);
    #else
      sweepMap->map(r,x,xr);
    #endif
    if( computeMap )
    {
      if (domainDimension == 3)
	x(I,axis3)=za+r(I,axis3)*(zb-za);
      else if (domainDimension == 2)
      {
	x(I,axis3)=za+r(I,axis2)*(zb-za);
      }
      
    }
    
    if( computeMapDerivative )
    {
      Range R2(0,1);
      xr(I,axis3,R2)=0.;
      if (domainDimension == 3)
      {
	xr(I,R2,axis3)=0.;
	xr(I,axis3,axis3)=zb-za;
      }
      else if (domainDimension == 2)
      {
	xr(I,R2,axis2)=0.;
	xr(I,axis3,axis2)=zb-za;
      }
    }
    return;
  }

  if( sweepType==tabulatedCylinder )
  {
    // **************************************************
    // *************  Tabulated Cylinder ****************
    // **************************************************
    if( dirSweepMap==NULL )
    {
      printf("SweepMapping::map:ERROR: The sweep curve has not been defined yet\n");
      return;
    }
    #ifdef USE_PPP
      dirSweepMap->mapS(r,x,xr);
    #else
      dirSweepMap->map(r,x,xr);
    #endif
    if( computeMap )
      for (int jj=0; jj<rangeDimension; jj++)
      {
	x(I,jj) += r(I,axis2)*straightLine(jj,0);
      }
    if( computeMapDerivative )
    {
      for (int jj=0; jj<rangeDimension; jj++)
      {
	xr(I,jj,1) = straightLine(jj,0);
      }
    }
    return;
  }

  if( sweepMap==NULL || dirSweepMap==NULL )
  {
    printf("SweepMapping::map:ERROR: Sweep mapping has not been defined yet\n");
    return;
  }
  

  int i;

  RealArray rI, rS, rS1, xS1; 
  RealArray v1; // For handling points
  RealArray v1p0, v1p1;     // For the derivatives
  RealArray xSweepMap, xrSweepMap;
  RealArray xDirSweepMap, xrDirSweepMap;
  RealArray xRowSpline0, xrRowSpline0;
  RealArray xRowSpline1, xrRowSpline1;
  RealArray xRowSpline2, xrRowSpline2;
  RealArray xScaleSpline, xrScaleSpline;


  if (domainDimension==2)
  {
    //3D surface case
    rI.redim(I,1), rS.redim(I,1); //Need base 0
    rI=r(I,axis1);     // For the sweep curve
    rS=r(I,axis2);     // The sweep curve
  }
  if (domainDimension==3)
  {
    //3D volume case
    rI.redim(I,Range(axis1,axis2)), rS.redim(I,1); 
    rI=r(I,Range(axis1,axis2));     // The sweep surface
    rS=r(I,axis3);     // The sweep curve
  }

  xRowSpline0.redim(I,getRangeDimension());
  xRowSpline1.redim(I,getRangeDimension());
  xRowSpline2.redim(I,getRangeDimension());
  // *wdh  xSweepMap.redim(I,sweepMap->getRangeDimension());
  xSweepMap.redim(I,3); xSweepMap=0.;

  xDirSweepMap.redim(I,dirSweepMap->getRangeDimension());
  v1.redim(I,getRangeDimension());
  xScaleSpline.redim(I);
  
  if (computeMapDerivative)
  {
    xrRowSpline0.redim(I,getRangeDimension(),1);
    xrRowSpline1.redim(I,getRangeDimension(),1);
    xrRowSpline2.redim(I,getRangeDimension(),1);
    // *wdh*   xrSweepMap.redim(I,sweepMap->getRangeDimension(),
    xrSweepMap.redim(I,getRangeDimension(),sweepMap->getDomainDimension()); 
    xrSweepMap=0.;
		    
    xrDirSweepMap.redim(I,dirSweepMap->getRangeDimension(),
			dirSweepMap->getDomainDimension());
    v1p0.redim(I,getRangeDimension());
    v1p1.redim(I,getRangeDimension());
    xrScaleSpline.redim(I,1,1);
  }

  // We can scale the mapping being sweep
  if( scaleSpline!=NULL )
  {
    #ifdef USE_PPP
    scaleSpline->mapS( rS,xScaleSpline,xrScaleSpline);
    #else
    scaleSpline->map(rS,xScaleSpline,xrScaleSpline);
    #endif
  }
  else
  {
    xScaleSpline=1.;
    xrScaleSpline=0.;
  }
  
  #ifdef USE_PPP
  rowSpline0->mapS(rS,xRowSpline0,xrRowSpline0);
  rowSpline1->mapS(rS,xRowSpline1,xrRowSpline1);
  rowSpline2->mapS(rS,xRowSpline2,xrRowSpline2);
  #else
  rowSpline0->map(rS,xRowSpline0,xrRowSpline0);
  rowSpline1->map(rS,xRowSpline1,xrRowSpline1);
  rowSpline2->map(rS,xRowSpline2,xrRowSpline2);
  #endif

  // evaluate the surface or curve being swept
  #ifdef USE_PPP
  sweepMap->mapS(rI,xSweepMap,xrSweepMap);
  #else
  sweepMap->map(rI,xSweepMap,xrSweepMap);
  #endif
  // evaluate the sweep curve
  #ifdef USE_PPP
  dirSweepMap->mapS(rS,xDirSweepMap,xrDirSweepMap);
  #else
  dirSweepMap->map(rS,xDirSweepMap,xrDirSweepMap);
  #endif

  if (computeMap)
  {
    for (i=0;i<3;i++) v1(I,i)=(xSweepMap(I,i)-center[i])*xScaleSpline(I);

    x(I,0)=xRowSpline0(I,0)*v1(I,0)+xRowSpline0(I,1)*v1(I,1)+xRowSpline0(I,2)*v1(I,2)+xDirSweepMap(I,0);
    x(I,1)=xRowSpline1(I,0)*v1(I,0)+xRowSpline1(I,1)*v1(I,1)+xRowSpline1(I,2)*v1(I,2)+xDirSweepMap(I,1);
    x(I,2)=xRowSpline2(I,0)*v1(I,0)+xRowSpline2(I,1)*v1(I,1)+xRowSpline2(I,2)*v1(I,2)+xDirSweepMap(I,2);

//      x(I,0)=0.;
//      for (i=0;i<3;i++) x(I,0) += xRowSpline0(I,i)*v1(I,i);
//      x(I,1)=0.;
//      for (i=0;i<3;i++) x(I,1) += xRowSpline1(I,i)*v1(I,i);
//      x(I,2)=0.;
//      for (i=0;i<3;i++) x(I,2) += xRowSpline2(I,i)*v1(I,i);

//      for (i=0;i<3;i++) x(I,i) += xDirSweepMap(I,i);
  }

  if (computeMapDerivative)
  {
    for (i=0;i<3;i++)
    {
      v1p0(I,i)=xrSweepMap(I,i,0);
      if( domainDimension==3 )
        v1p1(I,i)=xrSweepMap(I,i,1);
      v1(I,i)=(xSweepMap(I,i)-center[i]);
    }
    xr(I,0,0)=0.;
    for (i=0;i<3;i++) xr(I,0,0) += xRowSpline0(I,i)*v1p0(I,i)*xScaleSpline(I);
    xr(I,1,0)=0.;
    for (i=0;i<3;i++) xr(I,1,0) += xRowSpline1(I,i)*v1p0(I,i)*xScaleSpline(I);
    xr(I,2,0)=0.;
    for (i=0;i<3;i++) xr(I,2,0) += xRowSpline2(I,i)*v1p0(I,i)*xScaleSpline(I);

    if( domainDimension==3 )
    {
      xr(I,0,1)=0.;
      for (i=0;i<3;i++) xr(I,0,1) += xRowSpline0(I,i)*v1p1(I,i)*xScaleSpline(I);
      xr(I,1,1)=0.;
      for (i=0;i<3;i++) xr(I,1,1) += xRowSpline1(I,i)*v1p1(I,i)*xScaleSpline(I);
      xr(I,2,1)=0.;
      for (i=0;i<3;i++) xr(I,2,1) += xRowSpline2(I,i)*v1p1(I,i)*xScaleSpline(I);
    }
    
    const int dir= domainDimension-1;
    xr(I,0,dir)=0.;
    for (i=0;i<3;i++) xr(I,0,dir) += (xrRowSpline0(I,i,0)*xScaleSpline(I)+
				    xRowSpline0(I,i)*xrScaleSpline(I,0,0))*v1(I,i);
    xr(I,1,dir)=0.;
    for (i=0;i<3;i++) xr(I,1,dir) += (xrRowSpline1(I,i,0)*xScaleSpline(I)+
				    xRowSpline1(I,i)*xrScaleSpline(I,0,0))*v1(I,i);
    xr(I,2,dir)=0.;
    for (i=0;i<3;i++) xr(I,2,dir) += (xrRowSpline2(I,i,0)*xScaleSpline(I)+
				    xRowSpline2(I,i)*xrScaleSpline(I,0,0))*v1(I,i);
    for (i=0;i<3;i++) xr(I,i,dir) += xrDirSweepMap(I,i,0);

  }
  // ::display(x,"x for sweep");
  

}

void SweepMapping::
basicInverse( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
{
#ifdef USE_PPP
  Overture::abort("SweepMapping::basicInverse: ERROR: fix me Bill!");
#else
  basicInverseS(x,r,rx,params);
#endif
}

void SweepMapping::
basicInverseS( const RealArray & x, 
	       RealArray & r, 
	       RealArray & rx /* = Overture::nullRealDistributedArray() */,
	       MappingParameters & params /* =Overture::nullMappingParameters() */ )
{
  assert( sweepType==extrude );
  
  if( sweepMap==NULL )
  {
    printf("SweepMapping::basicInverse:ERROR: Sweep mapping has not been defined yet\n");
    return;
  }
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  #ifdef USE_PPP
    sweepMap->inverseMapS(x,r,rx);
  #else
    sweepMap->inverseMap(x,r,rx);
  #endif
  assert( (zb-za)!=0. );
  
  if( computeMap )
  {
    if (domainDimension == 3)
    {
      r(I,axis3)=(x(I,axis3)-za)*(1./(zb-za));
    }
    else if (domainDimension == 2)
    {
      r(I,axis2)=(x(I,axis3)-za)*(1./(zb-za));
    }
    
    
  }
  if( computeMapDerivative )
  {
    Range R2(0,1);
    rx(I,axis3,R2)=0.;
    rx(I,R2,axis3)=0.;
    if (domainDimension == 3)
    {
      rx(I,axis3,axis3)=1./(zb-za);
    }
    else if (domainDimension == 2)
    {
      rx(I,axis2,axis3)=1./(zb-za);
    }
    
  }
  return;
}





// get a mapping from the database
int SweepMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase &subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( (debug/4) % 2 )
    cout << "Entering SweepMapping::get" << endl;

  subDir.get( SweepMapping::className,"className" ); 
  if( SweepMapping::className != "SweepMapping" )
  {
    cout << "SweepMapping::get ERROR in className!" << endl;
    cout << "className from the database = " << SweepMapping::className << endl;
  }

  subDir.get( center, "center",3);
  int temp;
  subDir.get( temp, "centeringOption"); centeringOption=(CenteringOptionsEnum)temp;
  
  subDir.get( orientation, "orientation" );
  
  subDir.get( temp, "sweepType"); sweepType=(SweepTypeEnum)temp;
  subDir.get( za, "za" ); 
  subDir.get( zb, "zb" ); 

  subDir.get( straightLine, "straightLine" ); 

  aString mappingname;
  int exists=0;



  if( sweepMap!=NULL && sweepMap->decrementReferenceCount()==0 )
    delete sweepMap;
  sweepMap=NULL;
  subDir.get(exists,"sweepMapExists");
  if( exists )
  {
    subDir.get(mappingname,"sweepMap");
    sweepMap = Mapping::makeMapping( mappingname );
    if( sweepMap == NULL )
    {
      cout << "SweepMapping::get:ERROR unable to make the mapping with className="
	   << (const char *)mappingname << endl;
      {throw "error";}
    }
    sweepMap->get(subDir,"sweepmap");
    sweepMap->incrementReferenceCount();
  }

  if( dirSweepMap!=NULL && dirSweepMap->decrementReferenceCount()==0 )
    delete dirSweepMap;
  dirSweepMap=NULL;
  subDir.get(exists,"dirSweepMapExists");
  if( exists )
  {
    subDir.get(mappingname,"dirSweepMap");
    dirSweepMap = Mapping::makeMapping( mappingname ); 
    if (dirSweepMap == NULL){
      cout << "SweepMapping::get:ERROR unable to make the mapping with className=" << mappingname << endl;
      {throw "error";}
    }
    dirSweepMap->get(subDir,"dirsweepmap");
    dirSweepMap->incrementReferenceCount();
  }
  
  if( rowSpline0!=NULL && rowSpline0->decrementReferenceCount()==0 ) 
    delete rowSpline0;
  if( rowSpline1!=NULL && rowSpline1->decrementReferenceCount()==0 )  
    delete rowSpline1;
  if( rowSpline2!=NULL && rowSpline2->decrementReferenceCount()==0 )  
    delete rowSpline2;

  rowSpline0=rowSpline1=rowSpline2=NULL;

  subDir.get(exists,"rowSplinesExist");
  if( exists )
  {
    subDir.get(mappingname,"rowSpline0");
    rowSpline0 = (SplineMapping *)Mapping::makeMapping( mappingname );
    if (rowSpline0 == NULL){
      cout << "SweepMapping::get:ERROR unable to make the mapping with className=" << mappingname << endl;
      {throw "error";}
    }
    rowSpline0->get(subDir,"rowspline0");
    rowSpline0->incrementReferenceCount();

    subDir.get(mappingname,"rowSpline1");
    rowSpline1 = (SplineMapping *)Mapping::makeMapping( mappingname );
    if (rowSpline1 == NULL){
      cout << "SweepMapping::get:ERROR unable to make the mapping with className=" << mappingname << endl;
      {throw "error";}
    }
    rowSpline1->get(subDir,"rowspline1");
    rowSpline1->incrementReferenceCount();

    subDir.get(mappingname,"rowSpline2");
    rowSpline2 = (SplineMapping *)Mapping::makeMapping( mappingname );
    if (rowSpline2 == NULL){
      cout << "SweepMapping::get:ERROR unable to make the mapping with className=" << mappingname << endl;
      {throw "error";}
    }
    rowSpline2->get(subDir,"rowspline2");
    rowSpline2->incrementReferenceCount();
  }

  if( scaleSpline!=NULL && scaleSpline->decrementReferenceCount()==0 )
    delete scaleSpline;
  scaleSpline=NULL;
  subDir.get(exists,"scaleSplineExists");
  if( exists )
  {
    subDir.get(mappingname,"scaleSpline");
    scaleSpline = (SplineMapping *)Mapping::makeMapping( mappingname ); 
    if (scaleSpline == NULL){
      cout << "SweepMapping::get:ERROR unable to make the mapping with className=" << mappingname << endl;
      {throw "error";}
    }
    scaleSpline->get(subDir,"scalespline");
    scaleSpline->incrementReferenceCount();
  }
  
  Mapping::get( subDir, "Mapping" );
  mappingHasChanged();
  delete &subDir;
  return 0;
}
int SweepMapping::
put( GenericDataBase & dir, const aString & name) const
// ================================================================
// /Description:
//   Put a mapping to a database.
// ================================================================
{  
  GenericDataBase & subDir = *dir.virtualConstructor();    // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( SweepMapping::className,"className" );

  subDir.put( center, "center",3);
  subDir.put( (int)centeringOption, "centeringOption");
  subDir.put( orientation, "orientation" );

  subDir.put( (int)sweepType, "sweepType");
  subDir.put( za, "za" ); 
  subDir.put( zb, "zb" ); 

  subDir.put( straightLine, "straightLine" ); 

  int exists=sweepMap!=NULL;
  subDir.put(exists,"sweepMapExists");
  if( exists )
  {
    subDir.put( sweepMap->getClassName(), "sweepMap");
    sweepMap->put(subDir,"sweepmap");
  }
  exists=dirSweepMap!=NULL;
  subDir.put(exists,"dirSweepMapExists");
  if( exists )
  {
    subDir.put( dirSweepMap->getClassName(), "dirSweepMap");
    dirSweepMap->put(subDir,"dirsweepmap");
  }
  exists=rowSpline0!=NULL;
  subDir.put(exists,"rowSplinesExist");
  if( exists )
  {
    subDir.put( rowSpline0->getClassName(), "rowSpline0");
    rowSpline0->put(subDir,"rowspline0");
    subDir.put( rowSpline1->getClassName(), "rowSpline1");
    rowSpline1->put(subDir,"rowspline1");
    subDir.put( rowSpline2->getClassName(), "rowSpline2");
    rowSpline2->put(subDir,"rowspline2");
  }
  exists=scaleSpline!=NULL;
  subDir.put(exists,"scaleSplineExists");
  if( exists )
  {
    subDir.put( scaleSpline->getClassName(), "scaleSpline");
    scaleSpline->put(subDir,"scalespline");
  }
  
  Mapping::put( subDir, "Mapping" );
  delete &subDir;
  return 0;
}

Mapping* SweepMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==SweepMapping::className )
    retval = new SweepMapping();
  return retval;
}

aString SweepMapping::
getClassName() const
{
  return SweepMapping::className;
}

//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int SweepMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = { 
      "!SweepMapping",
//    "choose sweep surface/curve",
    "choose reference mapping",
    "choose sweep curve",
    "orientation",
    ">scaling",
      "specify scaling factors",
        "choose a scaling curve",
      "<>centering options",
        "use center of sweep curve",
        "use center of sweep surface",
        "specify center",
      "<extrude",
      "sweep a line",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "check",
      "check inverse",
      "show parameters",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "specify scaling factors : interactively enter the scaling coefficients from the begining to the end"
      "centering options",
      "  use center of sweep curve : choose the center from the sweep curve",
      "  use center of sweep surface : choose the center as the center of the sweep surface",
      "orientation : +1 or -1, to rotate the initial sweep surface by 180 degrees choose -1",
      "extrude : extrude a 2D mapping into 3D along a straight line",
      "sweep a line: Define a surface by sweeping a straight line along the sweep curve",
      " ",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "check inverse      : input points to check the inverse",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  aString answer,answer2,line;
  int i;
  
  // look for appropriate reference surface and sweep curve
  if( sweepMap==NULL || dirSweepMap==NULL )
  {
    int num=mapInfo.mappingList.getLength();
    for( i=num-1; i>=0; i-- )
    {
      MappingRC & map = mapInfo.mappingList[i];
// find a candidate for sweepMap, if none is provided
      if( sweepMap==NULL && map.getDomainDimension()==2 && map.getRangeDimension()>=2 )
      {
	sweepMap=&map.getMapping();
// AP	sweepMap->uncountedReferencesMayExist();
	if( sweepMap && !sweepMap->uncountedReferencesMayExist() )
	  sweepMap->incrementReferenceCount();
      }

// find a candidate for dirSweepMap, if none is provided
      if( dirSweepMap==NULL && map.getDomainDimension()==1 && map.getRangeDimension()==3 )
      {
	dirSweepMap=&map.getMapping();
// AP	dirSweepMap->uncountedReferencesMayExist();
	if( dirSweepMap && !dirSweepMap->uncountedReferencesMayExist() )
	  dirSweepMap->incrementReferenceCount();
      }

// AP: The following statement resets the extrusion bounds!
//        if( sweepMap!=NULL && dirSweepMap==NULL )
//  	setExtrudeBounds();

      if( sweepMap!=NULL )
	setMappingProperties();
    }
  }


  bool plotObject=sweepMap!=NULL && (dirSweepMap!=NULL || sweepType==extrude);

  GraphicsParameters params;
  params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  gi.appendToTheDefaultPrompt("Sweep>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 
    if (answer=="choose reference mapping")
    {
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int j=0;
      for (i=0;i<num;i++)
      {
        MappingRC & map = mapInfo.mappingList[i];
        if( map.getDomainDimension()<=2 && map.getRangeDimension()>=2 )
	{
	  subListNumbering(j)=i;
	  menu2[j++]=map.getName(mappingName);
	}
      }
      if (j==0)
      {
        gi.outputString("SweepMapping::WARNING: No proper mapping found");
        delete [] menu2;
	continue;
      }
      menu2[j]="";

      int sweepMapNumber = gi.getMenuItem(menu2,answer2,"Enter the reference surface");
      if (sweepMapNumber<0)
      {
	cout << "Unknown response: " << answer2 << endl;
	throw "error";
      }
      sweepMapNumber=subListNumbering(sweepMapNumber);
      if( sweepMap!=NULL && sweepMap->decrementReferenceCount()==0 )
	delete sweepMap;
      sweepMap=mapInfo.mappingList[sweepMapNumber].mapPointer;
      sweepMap->uncountedReferencesMayExist();
      sweepMap->incrementReferenceCount();

      delete [] menu2;

      setMappingProperties();
      plotObject=sweepMap!=NULL && (dirSweepMap!=NULL || sweepType==extrude);
    }
    else if (answer=="choose sweep curve")
    {
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int j=0;
      for (i=0;i<num;i++)
      {
        MappingRC & map = mapInfo.mappingList[i];
        if( map.getDomainDimension()==1 && map.getRangeDimension()==3 )
	{
	  subListNumbering(j)=i;
	  menu2[j++]=map.getName(mappingName);
	}
      }
      if (j==0)
      {
        gi.outputString("SweepMapping::WARNING: No proper mapping found");
        delete [] menu2;
	continue;
      }
      menu2[j]="";

      int curveNumber = gi.getMenuItem(menu2,answer2,"Enter the sweep curve");
      if (curveNumber<0)
      {
	cout << "Unknown response: " << answer2 << endl;
	throw "error";
      }
      curveNumber=subListNumbering(curveNumber);
      if( dirSweepMap!=NULL && dirSweepMap->decrementReferenceCount()==0 )
	delete dirSweepMap;
      dirSweepMap=mapInfo.mappingList[curveNumber].mapPointer;
      dirSweepMap->uncountedReferencesMayExist();
      dirSweepMap->incrementReferenceCount();

      delete [] menu2;

      setMappingProperties();
      plotObject=sweepMap!=NULL && (dirSweepMap!=NULL || sweepType==extrude);
    }
    else if (answer=="specify scaling factors")
    {
      int numOfScalingCoeff=2;
      realArray scalingCoeff;
      char buff[180];
      gi.inputString(line,sPrintF(buff,"Enter the number of scaling coefficients (default=%i): ",numOfScalingCoeff));
      if( line!="" ) sScanF(line,"%i",&numOfScalingCoeff);
      if (numOfScalingCoeff<2){
       gi.outputString("ERROR: At least two coefficients are needed, at the begining and at the end!!!\n");
       continue;
      }

      scalingCoeff.redim(numOfScalingCoeff);
      for (int icount=0;icount<numOfScalingCoeff;icount++){
	gi.inputString(line,sPrintF(buff,"Enter scale %i (a real number)",icount));
	if (line != "") sScanF(line,"%f",&(scalingCoeff(icount)));
      }
      //update the scaleSpline
      if (scaleSpline != NULL && scaleSpline->decrementReferenceCount()==0 ) 
        delete scaleSpline;
      scaleSpline= new SplineMapping(1);
      scaleSpline->incrementReferenceCount();
      ((SplineMapping*)scaleSpline)->setPoints(scalingCoeff);
      findRowSplines();
    }
    else if( answer=="choose a scaling curve" )
    {
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int j=0;
      for (i=0;i<num;i++)
      {
        MappingRC & map = mapInfo.mappingList[i];
        if( map.getDomainDimension()==1 && map.getRangeDimension()==1 )
	{
	  subListNumbering(j)=i;
	  menu2[j++]=map.getName(mappingName);
	}
      }
      if (j==0)
      {
        gi.outputString("SweepMapping::WARNING: There are no possible Mappings to use for a scaling function");
        gi.outputString("                     : The scaling function should have domainDimension==rangeDimension=1");
        delete [] menu2;
	continue;
      }
      menu2[j]="";

      int mapNumber = gi.getMenuItem(menu2,answer2,sPrintF(buff,"Enter the scaling curve"));
      if( mapNumber<0)
      {
	cout << "Unknown response: " << answer2 << endl;
	throw "error";
      }
      if( scaleSpline!=NULL && scaleSpline->decrementReferenceCount()==0 )
        delete scaleSpline;
      
      scaleSpline=mapInfo.mappingList[subListNumbering(mapNumber)].mapPointer; 
      scaleSpline->uncountedReferencesMayExist();
      scaleSpline->incrementReferenceCount();

      findRowSplines();
    }
    else if( answer=="use center of sweep curve" )
    {
      setCentering(useCenterOfSweepCurve);
    }
    else if( answer=="use center of sweep surface" )
    {
      setCentering(useCenterOfSweepSurface);
    }
    else if( answer=="specify center" )
    {
      gi.inputString(line,sPrintF(buff,"Enter center: xc,yc,zc"));
      sScanF(line,"%e %e %e",&center[0], &center[1], &center[2]);

      printf("Using center=(%8.2e,%8.2e,%8.2e)\n",center[0], center[1], center[2]);

      setCentering(specifiedCenter);
    }
    else if( answer=="orientation" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the orientation, +1 or -1, (current=%3.1f)",orientation));
      real temp;
      sScanF(line,"%e",&temp);
      setOrientation(temp);
    }
    else if( answer=="sweep a line" )
    {
      real lx = straightLine(0,1), ly = straightLine(1,1), lz = straightLine(2,1);
      
      gi.inputString(line,sPrintF(buff,"Enter the direction of the straight line lx, ly, lz "
				  "default=(%e,%e,%e): ", lx, ly, lz));
      
      if( line!="" ) 
        sScanF(line,"%e %e %e",&lx, &ly, &lz);
      
      setStraightLine(lx, ly, lz);
      
    }
    else if( answer=="extrude" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the extrusion bounds za,zb default=(%e,%e): ",za,zb));
      if( line!="" ) 
        sScanF(line,"%e %e",&za,&zb);
      setExtrudeBounds( za,zb );
    }
    else if( answer=="lines"  ||
        answer=="boundary conditions"  ||
        answer=="share"  ||
        answer=="mappingName"  ||
        answer=="periodicity"  ||
        answer=="check"        ||
        answer=="check inverse"  )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
    }
    else if( answer=="plot" )
    {
    params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
    params.set(GI_TOP_LABEL,getName(mappingName));
    gi.erase();
    PlotIt::plot(gi,*this,params);   
    params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
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
      // printf("plot the sweep\n");
      
      params.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,params);   

      // plot the sweep curve
      if( dirSweepMap!=NULL )
      {
	real oldCurveLineWidth;
	params.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
	params.set(GraphicsParameters::curveLineWidth,3.);
	params.set(GI_MAPPING_COLOUR,"green");
	PlotIt::plot(gi,*dirSweepMap,params); 
	params.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);

	params.set(GI_MAPPING_COLOUR,"red");
      }
      
    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
}

