#include "IntersectionMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "CircleMapping.h"
#include "TriangleClass.h"
#include "SplineMapping.h"
#include "DataPointMapping.h"
#include "NurbsMapping.h"
#include "CompositeSurface.h"
#include "Inverse.h"
#include "TrimmedMapping.h"

#include <float.h>
#ifndef OV_USE_OLD_STL_HEADERS
#include <list>
#include <vector>
OV_USINGNAMESPACE(std);
#else
#include <list.h>
#include <vector.h>
#endif

#define DGECO EXTERN_C_NAME(dgeco)
#define SGECO EXTERN_C_NAME(sgeco)
#define DGESL EXTERN_C_NAME(dgesl)
#define SGESL EXTERN_C_NAME(sgesl)

#ifdef OV_USE_DOUBLE
#define GECO DGECO
#define GESL DGESL
#else
#define GECO SGECO
#define GESL SGESL
#endif


extern "C"
{
  void SGECO( real & b, const int & nbd, const int & nb, int & ipvt,real & rcond, real & work );

  void DGECO( real & b, const int & nbd, const int & nb, int & ipvt,real & rcond, real & work );

  void SGESL(real & a,const int & lda,const int & n,int & ipvt, real & b, int & job);

  void DGESL(real & a,const int & lda,const int & n,int & ipvt, real & b, int & job);
  
}


IntersectionMapping::
IntersectionMapping() : Mapping(1,3,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief  Default Constructor
/// \param Author: WDH
//===========================================================================
{ 
  IntersectionMapping::className="IntersectionMapping";
  setName( Mapping::mappingName,"intersectionMapping");

  setGridDimensions( axis1,21 );

  map1=NULL;
  map2=NULL;
  rCurve1=NULL;
  rCurve2=NULL;
  curve=NULL;
  mappingHasChanged();
}


IntersectionMapping::
IntersectionMapping(Mapping & map1_,
		    Mapping & map2_ )
: Mapping(1,3,parameterSpace,cartesianSpace) 
//===========================================================================
/// \brief 
///    Define a mapping for the intersection of map1\_ and map2\_
/// \param map1_, map2_ : two surfaces in 3D
//===========================================================================
{
  IntersectionMapping::className="IntersectionMapping";
  setName( Mapping::mappingName,"intersectionMapping");
  setGridDimensions( axis1,21 );
  
  map1=&map1_;
  map2=&map2_;
  rCurve1=NULL;
  rCurve2=NULL;
  curve=NULL;

  determineIntersection();

  mappingHasChanged();
    
}


// Copy constructor is deep by default
IntersectionMapping::
IntersectionMapping( const IntersectionMapping & map, const CopyType copyType )
{
  IntersectionMapping::className="IntersectionMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "IntersectionMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

IntersectionMapping::
~IntersectionMapping()
{ 
  if( debug & 4 )
    cout << " IntersectionMapping::Desctructor called" << endl;

  if( curve!=NULL && curve->decrementReferenceCount()==0 )
    delete curve;
  if( rCurve1!=NULL && rCurve1->decrementReferenceCount()==0 )
    delete rCurve1;
  if( rCurve2!=NULL && rCurve2->decrementReferenceCount()==0 )
    delete rCurve2;

}

IntersectionMapping & IntersectionMapping::
operator=( const IntersectionMapping & X )
{
  if( IntersectionMapping::className != X.getClassName() )
  {
    cout << "IntersectionMapping::operator= ERROR trying to set a IntersectionMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  map1     = X.map1;
  map2     = X.map2;
  rCurve1  = X.rCurve1;
  rCurve2  = X.rCurve2;
  curve    = X.curve;
  

  this->Mapping::operator=(X);            // call = for derivee class
  return *this;
}

int IntersectionMapping::
intersect(Mapping & map1_, Mapping & map2_,
	  GenericGraphicsInterface *gi /* =NULL */,
	  GraphicsParameters & params /* =nullGraphicsParameters */)
// ==================================================================================
/// \details 
///    Determine the intersection between two mappings, optionally supply graphic parameters
///     so the intersection curves can be plotted, (for debugging purposes).
///    NEW FEATURE: If the intersection curve has disjoint segments, these segments will be 
///    stored as sub curves in the NURBS for the physical and parameter curves on each surface.
/// \param map1_, map2_ (input) : These two mappings will be intersected.
/// \param gi, paramas (input) : Optional parameters for graphics.
/// \return  0 for success
// ==================================================================================
{
  if( map1_.getDomainDimension()!=2 || map1_.getRangeDimension()!=3 ||
      map1_.getDomainDimension()!=2  || map2_.getRangeDimension()!=3 )
  {
    cout << "IntersectionMapping::intersect:ERROR: sorry, I can only intersect surfaces in 3D \n";
    return 1;
  }
  
  int returnValue=0;
  
  if( rCurve1!=NULL && rCurve1->decrementReferenceCount()==0 )
  {
    delete rCurve1; rCurve1=NULL;
  }
  if( rCurve2!=NULL && rCurve2->decrementReferenceCount()==0 )
  {
    delete rCurve2; rCurve2=NULL;
  }
  if( curve!=NULL && curve->decrementReferenceCount()==0 )
  {
    delete curve;   curve=NULL;
  }
  


  if( map1_.getClassName()!="CompositeSurface" && map2_.getClassName()!="CompositeSurface" )
  {
    // kkc 020802 check to see if the two mappings actually intersect!
    if ( !map1_.intersects(map2_) )
    {
      printf("IntersectionMapping::the mappings do not intersect!\n");
      return 1;
    }

    map1=&map1_;
    map2=&map2_;
    returnValue=determineIntersection(gi,params);
  }
  else
  {
    // Intersection with a CompositeSurface
    if( map1_.getClassName()!="CompositeSurface" && map2_.getClassName()=="CompositeSurface" )
    {
      returnValue=intersectWithCompositeSurface(map1_, (CompositeSurface&)map2_,gi,params);
    }
    else 
    {
      returnValue=intersectWithCompositeSurface(map2_, (CompositeSurface&)map1_,gi,params);
    }
  }

  if ( curve )
    setIsPeriodic(0,curve->getIsPeriodic(0));

  mappingHasChanged();
  return returnValue;
}

// Holds results from trim-curve intersections
class Vect
{
public:
real rp;
real crossProduct;
Vect(real a, real b){rp=a;crossProduct=b;} //

bool operator==(const Vect & a )const{return rp==a.rp;}  //
bool operator<(const Vect & a )const{return rp<a.rp;}  //
};

int IntersectionMapping::
intersectWithCompositeSurface(Mapping & map1_, CompositeSurface & cs,
			      GenericGraphicsInterface *gi /* =NULL */,
			      GraphicsParameters & params /* =nullGraphicsParameters */)
// ==================================================================================
/// \details 
///     A Protected routine that computes the intersection between a Mapping
///  and a CompositeSurface. 
/// 
/// \param map1_, map2_ (input) : These two mappings will be intersected.
/// \param gi, paramas (input) : Optional parameters for graphics.
/// \return  0 for success
/// 
/// \param Output: The output intersection curve is a NurbsMapping. The number of subcurves of this
///  mapping defines the number of disconnected components of the intersection.
/// 
// ==================================================================================
{
// The intersection of one or more CompositeSurfaces is a bit more complicated.
//
//   For each sub-surface do
//      c1 : curve of intersection with un-trimmed sub-surface
//      cs[i] : split c1 into sub-curves (based on intersections with trim curves)
//      Add c1[i] to masterList or merge with a curve in the masterList
//   end
//   Merge curves in masterList
//   Add masterList curves as sub-curves to to the final intersection-curve 

  int debugi=3; // 3;

  if(  map1_.getClassName()=="CompositeSurface" )
  {
    printf("IntersectionMapping::ERROR:intersectWithCompositeSurface: map1 cannot be a CompositeSurface too!\n");
    return 1;
  }
  
  map1=&map1_;

  ListOfMappingRC masterList;  // curve segments formed from trimmed-intersections
  // bool useMaster=true;
      
  const real rEps=1.e-5; // tolerance for end-points meeting the unit square

  // First compute intersection curves for each sub-surface.
  int s;
  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    if( cs.isVisible(s) )
    {
      if( debugi & 1 ) printf("  ******Check for intersection with sub-surface s=%i\n",s);
      map2=&cs[s];
      bool isTrimmed = map2->getClassName()=="TrimmedMapping";
      if( isTrimmed )
      {
	map2=((TrimmedMapping*)map2)->untrimmedSurface();
      }
      if( true )
      {
	if( rCurve1!=NULL && rCurve1->decrementReferenceCount()==0 )
	  delete rCurve1; rCurve1=NULL;
        rCurve1=NULL;
	if( rCurve2!=NULL && rCurve2->decrementReferenceCount()==0 )
	  delete rCurve2; 
        rCurve2=NULL;
	if( curve!=NULL && curve->decrementReferenceCount()==0 )
	{
          // cout << "**delete curve=" << curve << endl;
	  delete curve;  
	}
        curve=NULL;
      }
      
      // printf("\n****************IntersectionMapping:Intersect with sub-surface %i***********\n",s);
	  
      if( false ) 
      {
        // *wdh* 051011 -- increase surface by a bit to make sure the intersection curve
        // will cross the outer trimming curve
        if( map2->getClassName()=="NurbsMapping" )
	{
	  NurbsMapping *nurbs2 = (NurbsMapping*)map2;
	  real r1a=-rEps, r1b=1.+rEps, r2a=-rEps, r2b=1.+rEps;
	  nurbs2->setDomainInterval(r1a,r1b,r2a,r2b);
	}
	
      }
      

      // This next call will determine the curves of intersection rCurve1, rCurve2 and curve
      int returnValue= determineIntersection(gi,params);

      if( returnValue==0 )
      {
  	// NurbsMapping & csCurve1=*((NurbsMapping*)rCurve1);
  	NurbsMapping & csCurve2=*((NurbsMapping*)rCurve2);
	NurbsMapping & csCurve=*((NurbsMapping*)curve);

	if( debugi & 1 ) 
           printf("  ***intersection found with CompositeSurface subSurface=%i : %i intersection curves ****\n",s,
                      csCurve.numberOfSubCurves());


	if( !isTrimmed )
        {
          // not trimmed -- keep the whole intersection curve
          if( debugi & 1 ) printf(" add curve to master list (untrimmed) ref count=%i ->",csCurve.getReferenceCount());
	  
          // masterList.addElement(csCurve);
	  
          // printf(" %i\n",csCurve.getReferenceCount());
          // cout << "** added curve=" << curve << endl;

          for( int sc=0; sc<csCurve.numberOfSubCurves(); sc++ )
             masterList.addElement(csCurve.subCurve(sc));
	}
	else 
	{
	  // We need to clip the curve by the trimming curves.


	  TrimmedMapping & trim = ((TrimmedMapping&)cs[s]);
	    
          const real parallelEps=REAL_EPSILON*1000.; // eps for checking parallel intersections
	    
	  if( debugi & 1 ) 
            printf("  **subSurface %i is a TrimmedMapping, trim.getNumberOfTrimCurves()=%i csCurve.numberOfSubCurves()=%i\n",s,
                             trim.getNumberOfTrimCurves(),csCurve.numberOfSubCurves());

	  int totalNumberOfIntersectionPoints=0;
          for( int sc=0; sc<csCurve.numberOfSubCurves(); sc++ )  // loop over subCurves
	  {
  	    IntersectionMapping trimIntersection; // to compute the intersection of trim curves with csCurve2
	    realArray r1,r2,r;

            NurbsMapping & nurbs2=csCurve2.subCurve(sc);
	    

	    int numberOfIntersectionPoints=0;
	    int nParallel=0;  // number of intersections that are nearly parallel
	  
	    list<Vect> tp;  // tp = "trim points", holds points of intersection

	    realArray rr(1,1),x1(1,2),xr1(1,2,1), x2(1,2),xr2(1,2,1);

	    // We could get intersections with all trim curves
	    for( int tc=0; tc<trim.getNumberOfTrimCurves(); tc++ )
	    {
	      Mapping & trimCurve = *trim.getTrimCurve(tc);
	      int ni=0;
	      trimIntersection.intersectCurves(nurbs2, trimCurve, ni, r1,r2,r );
	      

  	      if( debugi & 1 )
	      {
                printf(" **trim number of intersections with trim curve tc=%i is ni=%i\n",tc,ni);
		for( int n=0; n<ni; n++ )
  		  printf("  **trim intersection n=%i, cut-curve:r1=%e, trimCurve:r2=%e, r=(%e,%e)\n",
			 n,r1(n),r2(n),r(0,n),r(1,n));
	      }
	      
	      if( (ni%2)!=0 )
	      {
		printf(" *****INFO: There are an odd number of intersections with the trim curve: ni=%i. \n",ni);
	      }
               
	      numberOfIntersectionPoints+=ni;
	      for( int n=0; n<ni; n++ )
	      {

		// Check for inside or outside.
		//  The cross product of the tangent vectors should tell us if we are
		//  moving from outside to inside.
		//  If trim curve orientation is
		//        clockwise : cross-product= 1 -> outside to inside
		//                                 =-1 -> inside to outside
		//                 ^
		//                 |
		//        ------------>-----csCurve2
		//                 |
		//                 |trimCurve
		rr(0,0)=r1(n);
		nurbs2.map(rr,x1,xr1);
		rr(0,0)=r2(n);
		trimCurve.map(rr,x2,xr2);
		
		real crossProduct=xr1(0,0)*xr2(0,1)-xr1(0,1)*xr2(0,0);
		crossProduct/=max(sqrt(SQR(xr1(0,0))+SQR(xr1(0,1))),REAL_MIN*100.);
		crossProduct/=max(sqrt(SQR(xr2(0,0))+SQR(xr2(0,1))),REAL_MIN*100.);

		// ***troublesome case: If the curves are parallel over an interval we may
		// just get a lot of intersections
               
		// crossProduct*=trim.trimOrientation(tc);
		// *wdh* 051011 if( r1(n)>0. && r1(n)<1. )
		if( fabs(crossProduct)>.01 || (r1(n)>0. && r1(n)<1.) )
		{
		  tp.push_back(Vect(r1(n),crossProduct));
		  if( fabs(crossProduct)<parallelEps )
		    nParallel++;
		}
	      
		if( debugi & 1 ) 
  		  printf("  **interCurve:trimCurve=%i orien=%i intersection n=%i: r1=%8.2e cross=%8.1e "
                         " xr1=(%8.1e,%8.1e) xr2=(%8.1e,%8.1e)\n",
  			 tc,trim.trimOrientation(tc),n,r1(n),crossProduct,xr1(0,0),xr1(0,1),xr2(0,0),xr2(0,1));


	      }
	    }

            // *wdh* 051010
	    // Intersections on the boundaries r_i=0 or r_i=1 may be missed since the
	    // curves may not quite intersect
	    // --> evaluate end points of nurbs2
	    //     if an end-point is at r=0 or r=1 then make sure it is in the list
	    if( true && numberOfIntersectionPoints==0 )  
	    {
	      
	      realArray ra(1,1),rb(1,2);
              for( int side=0; side<=1; side++ )
	      {
		ra=(real)side;
                int crossProduct=-(1-2*side); //  +1 : outside to inside, -1 : inside to outside

		nurbs2.map(ra,rb);
		printf(" End point of intersection curve is r=(%9.2e,%9.2e)\n",rb(0,0),rb(0,1));
                if( fabs(rb(0,0))<rEps || fabs(rb(0,1))<rEps ||
                    fabs(rb(0,0)-1.)<rEps || fabs(rb(0,1)-1.)<rEps )
		{
                  // for now assume that any duplicates will be caught later -- is this ok?
                  numberOfIntersectionPoints++;

		  printf(" End point of intersection curve is added! \n");
		  
		  tp.push_back(Vect(ra(0,0),crossProduct));  

                  break;  // we only need to add one intersection point to the list
		}

	      }
	      
	    }

	    totalNumberOfIntersectionPoints+=numberOfIntersectionPoints;

	    if( numberOfIntersectionPoints==0 )
	      continue;
	  

	    tp.sort(); // sort by r values.

	    if( debugi & 1 ) 
	    {
	      printf("  **trim curves for s=%i intersects cut-curve at %i points: r=", s,numberOfIntersectionPoints);
	      list<Vect>::iterator itp=tp.begin();
	      for( ;itp!=tp.end();itp++ )
	      {
		Vect & v = *itp;
		printf("%9.3e(cp=%8.1e), ",v.rp,v.crossProduct);
	      }
	      printf("\n");
	    }

	    if( nParallel>1 )
	    {
	      // remove consecutive nearly parallel intersections
	      // since the likely cause is the intersection of two parallel lines ?
	      list<Vect>::iterator itp=tp.begin();
	      real cp1=(*itp).crossProduct; itp++;
	      real cp2;
	      for( ;itp!=tp.end();itp++ )
	      {
		cp2=(*itp).crossProduct;
		if( fabs(cp1)<parallelEps && fabs(cp2)<parallelEps )
		{
		  // remove this element and the previous
		  itp--;              // backup 
		  itp=tp.erase(itp);  // return value points to the element after the one erased
		  itp=tp.erase(itp);
                  if( itp==tp.end() ) break;
		  cp1=(*itp).crossProduct; 
		  while( fabs(cp1)<parallelEps )
		  {
		    itp=tp.erase(itp); 
		    if( itp==tp.end() ) break;
		    cp1=(*itp).crossProduct; 
		  }
		}
		else
		  cp1=cp2;
	      }

	      int newNum=tp.size();
	      printf(" Consecutive nearly parallel intersections were removed. %i points were removed.\n",
		     numberOfIntersectionPoints-newNum);

	      numberOfIntersectionPoints=newNum;
	      if( debugi & 1 ) 
	      {
		printf("  -->trim curves for s=%i intersects cut-curve at %i points: r=", s,numberOfIntersectionPoints);
		list<Vect>::iterator itp=tp.begin();
		for( ;itp!=tp.end();itp++ )
		{
		  Vect & v = *itp;
		  printf("%9.3e(cp=%8.1e), ",v.rp,v.crossProduct);
		}
		printf("\n");
	      }


	    }

	  
	    //  (1) sort intersections by the value of r1
	    //  (2) for each intersection determine which side is outside and inside
	    //  (3) Classify the segments as inside or outside
	    //
	    //        -----X----------X--------------X------------X----
	    //         out      in          out          in          out
	    //
	    //  (4) split curve and keep inside segments

	    NurbsMapping & interCurve = csCurve.subCurve(sc);
		
	    // ***** Here we assume that the first and last intersections are with the outer curve
	    //       This needs to be fixed.

	    bool keepRightMost=tp.back().crossProduct<0.;    // inside to outside
	    bool keepLeftMost=tp.front().crossProduct>0.;    // outside to inside

	    if( debugi & 1 ) 
	    {
              printf(" tp.back: crossProduct=%8.2e, tp.rp=%8.2e\n",tp.back().crossProduct,tp.back().rp);
              printf(" tp.front: crossProduct=%8.2e, tp.rp=%8.2e\n",tp.front().crossProduct,tp.front().rp);
	      

	      if( keepRightMost )
		printf("  INFO: keeping right most segment. (TrimmedSurface must not be cut completely\n");
	      if( keepLeftMost )
		printf("  INFO: keeping left most segment. (TrimmedSurface must not be cut completely\n");
	    }
	  
	    bool keep=keepRightMost; // false;
	    real rb=1.;
	    real ras,rbs=1.;
	    list<Vect>::iterator itp=tp.end();
	    while( itp!=tp.begin() )
	    {
	      itp--;
	      Vect & v = *itp;
	      real ra=v.rp;
	      ras=ra/max(REAL_EPSILON*10.,rb); // scaled version for splitting current segment which ends at "rbs"
	      // real cpa=v.crossProduct;
	    
	      printF("split at ras=%e (ra=%e)\n",ras,ra);
                  
	      if( rb<1. && ras>1.-rEps )
	      {
		// skip this split -- must be a duplicate point
		continue;
	      }

	      NurbsMapping &left = *new NurbsMapping;  left.incrementReferenceCount(); 
	      NurbsMapping &right = *new NurbsMapping; right.incrementReferenceCount(); 

              if( fabs(ras)<rEps )
	      {
		// no need to split at zero *wdh*051011
                right=interCurve;
	      }
              else if( fabs(ras-1.)<rEps )
	      {
		// no need to split at 1. *wdh*051011
                left=interCurve;
	      }
	      else
	      {
		bool ok = interCurve.split( ras,left,right )==0;


		if( !ok )
		{
		  printf("ERROR:Splitting curve at ras=%8.2e, 1-ras=%8.2e, rEps=%8.2e\n",ras,1-ras,rEps);
		}
		interCurve=left;
	      }
	      
	      if( keep )
	      {
		if( debugi & 1 ) 
		  printf("  Add the right segment to the master list ra=%8.2e rb=%8.2e (ras=%8.2e,rbs=%8.2e)\n",
			 ra,rb,ras,rbs);
		masterList.addElement(right);

	      }
	      rb=ra;
	      rbs=ras;
	      keep=!keep;  // keep next

	      if( itp==tp.begin() && keepLeftMost )
	      {
		if( debugi & 1 )
		  printf("  Add the left segment to the master list ra=%8.2e rb=%8.2e (ras=%8.2e,rbs=%8.2e)\n",
			 0.,ra,0.,ras);
		// masterList.addElement(right);
		masterList.addElement(left); // *wdh* 021120
	      }

	      if( left.decrementReferenceCount()==0 ) delete &left;
	      if( right.decrementReferenceCount()==0 ) delete &right;
	    }
	  } // end for sc (sub-curve)

          // *wdh* 081102 -- if there are were intersections with sub-curves we should add the curves that were found
	  if( totalNumberOfIntersectionPoints==0 ) 
	  {
	    for( int sc=0; sc<csCurve.numberOfSubCurves(); sc++ )
	      masterList.addElement(csCurve.subCurve(sc));
	  }
	  
	}  // end !isTrimmed
      }
    } // end isVisible

  } // end for s
  
  if( true )
  {
    if( rCurve1!=NULL && rCurve1->decrementReferenceCount()==0 )
      delete rCurve1; 
    rCurve1=NULL;
    if( rCurve2!=NULL && rCurve2->decrementReferenceCount()==0 )
      delete rCurve2; 
    rCurve2=NULL;
    if( curve!=NULL && curve->decrementReferenceCount()==0 )
      delete curve;   
    curve=NULL;
  }
  
  // Now try to merge curves together
  if( debugi & 1 ) printf("Merge curves: number of curves in masterList = %i\n",masterList.getLength());
  if( masterList.getLength()>0 )
  {
    if( curve!=NULL && curve->decrementReferenceCount()==0 )
      delete curve;
    NurbsMapping & master = *new NurbsMapping;
    curve=&master;
    curve->incrementReferenceCount();

    int numberRemaining=masterList.getLength();

    const int maxNumberOfDisconnectedCurves=100;  // save at most this many disconnected pieces
    for( int sc=0; sc<maxNumberOfDisconnectedCurves; sc++ )
    {
      NurbsMapping & current = *new NurbsMapping; current.incrementReferenceCount();

      current=(NurbsMapping&)(masterList[numberRemaining-1].getMapping());
      masterList.deleteElement(numberRemaining-1);
      numberRemaining--;

      // Now loop through the curves and attempt to merge to the current
      // keep loop as long as at least 1 curve was merged.
      while( numberRemaining>0 )
      {
	bool curvesWereMerged=false;
	for( int n=numberRemaining-1; n>=0; n-- )
	{
	  bool wasMerged=current.merge((NurbsMapping&)(masterList[n].getMapping()))==0;
	  if( wasMerged )
	  {
            if( debugi & 1 ) printf(" ..curves merged! sc=%i n=%i numberRemaining=%i\n",sc,n,numberRemaining);
	    
	    curvesWereMerged=true;
	    masterList.deleteElement(n);
	    numberRemaining--;
	  }
          else
	  {
            if( debugi & 1 ) printf(" ..no merge for sc=%i n=%i numberRemaining=%i\n",sc,n,numberRemaining);
	  }
	  
	}
	if( !curvesWereMerged ) break;
      }

      // No need to keep the sub curves (don't delete subCurve=0 : this is the actual nurbs.
      for( int i=current.numberOfSubCurves()-1; i>=1; i-- )
	current.deleteSubCurve(i);
	    
      if( sc==0 ) 
	master=current;   // save first curve as the master
      else
	master.addSubCurve(current);  // save other curves as sub-curves
	    
      if( current.decrementReferenceCount()==0 ) delete &current;
	    
      if( numberRemaining==0 ) break;
    } //  end for sc
	  
    // reduce the number of points for plotting -- there are usually way too many
    master.setGridDimensions(axis1,max(21,master.getGridDimensions(axis1)/10));
    if( numberRemaining==0 && master.numberOfSubCurves()==1 )
    {
      if( debugi & 1 ) printf("  **SUCCESS all sub-curves were merged into one master.\n");
	    
    }
    else if( numberRemaining==0 && master.numberOfSubCurves()>1 )
    {
      if( debugi & 1 ) printf("  **WARNING not all curves were merged into one. Number of curves=%i\n",
	     master.numberOfSubCurves());
    }
    else
    {
      if( debugi & 1 ) printf("  **ERRROR %i curves were created but %i still remain.\n",
	     master.numberOfSubCurves(),numberRemaining);
    }
    
  }

  return 0;
}




BoundingBox* IntersectionMapping::
createBoundingBox( BoundingBox & child1, BoundingBox & child2 )
// protected routine to create a bounding box with given children
{
  BoundingBox *box = new BoundingBox();
  box->child1=&child1;
  box->child2=&child2;
  return box;
}

void IntersectionMapping::
destroyBoundingBox( BoundingBox & box )
// protected routine to destroy the box created by createBoundingBox
{
  box.child1=NULL;
  box.child2=NULL;
  delete &box;
}


int IntersectionMapping::
newtonIntersection(realArray & x, realArray & r1, realArray & r2, const realArray & n )
// =====================================================================================================
/// \details  
///    This is a protected routine to determine the exact intersection point on two surfaces using Newton's
///   method.
/// 
///  Solve for (x,r1,r2 ) such that 
///  \begin{verbatim}
/// 
///     map1(r1) - x = 0
///     map2(r2) - x = 0
///     n.x = c
///  \end{verbatim}
/// 
/// \param x(.,3) (input/output) : initial guess to the intersection point (in the Range space)
/// \param r1(.,2) (input/output): initial guess to the intersection point (in the domain space of map1)
/// \param r2(.,2) (input/output): initial guess to the intersection point (in the domain space of map2)
/// \param n(.,3) : a normal vector to a plane that crosses the intersection curve, often choosen
///          to be  n(i,.) = x(i+1,.) - x(i-1,.) if we are computing x(i,.)
/// \return  0 for success. 1 if the newton iteration did not converge, 2 if there is a
///  zero normal vector.
// =====================================================================================================
{
  
  int iStart=x.getBase(0);
  int iEnd  =x.getBound(0);

  Range R(iStart,iEnd);

  if( min(fabs(n(R,0))+fabs(n(R,1))+fabs(n(R,2))) == 0. )
  {
    cout << "IntersectionMapping::newtonIntersection: ERROR there is a zero normal! \n";
    return 2;
  }

  // constraint is n.x = c := n.x_0
  realArray c(R);
  c=n(R,0)*x(R,0)+n(R,1)*x(R,1)+n(R,2)*x(R,2);
    

  Range R1(0,0), R2(0,1), R3(0,2), R4(0,3);
  realArray matrix(R,Range(0,3),Range(0,3)), delta(R,4), x1(R,3), x2(R,3), x1r1(R,3,2), x2r2(R,3,2);

  realArray m(1,4,4), d(1,4), work(4);
  IntegerArray ipvt(4);
  real rcond;
  int job=0;
  
  //
  // solve the 4x4 system:
  //    map1(r1) - map2(r2) = 0   <--->  F(r1,r2)=0
  //    n.map2(r2)          = c
  //
  real eps=SQRT(REAL_EPSILON*10.);  // should be ok if converging quadratically
  int maximumNumberOfIterations=10;
  int iteration;
  for( iteration=0; iteration<maximumNumberOfIterations; iteration++ )
  {
    
    map1->map(r1,x1,x1r1);
    map2->map(r2,x2,x2r2);
  
  
    matrix(R,R3,R2  )= x1r1(R,R3,R2);
    matrix(R,R3,R2+2)=-x2r2(R,R3,R2);
    matrix(R,3,R2)=0.;
    matrix(R,3,2)= n(R,0)*x2r2(R,0,0)+n(R,1)*x2r2(R,1,0)+n(R,2)*x2r2(R,2,0);
    matrix(R,3,3)= n(R,0)*x2r2(R,0,1)+n(R,1)*x2r2(R,1,1)+n(R,2)*x2r2(R,2,1);
  
    delta(R,R3)=-x1(R,R3)+x2(R,R3);
    delta(R,3)=c-n(R,0)*x2(R,0)-n(R,1)*x2(R,1)-n(R,2)*x2(R,2);
  
    // solve matrix*delta = f0
    //    factor one matrix at a time 
    for( int i=iStart; i<=iEnd; i++ )
    {
      m(0,R4,R4)=matrix(i,R4,R4);
      d(0,R4)=delta(i,R4);
      // if( max(fabs(x1r1(i,R3,1)))<.0001 )
      // printf("IntersectionMapping::newtonIntersection: ERROR it=%i, i=%i, matrix is singular \n",iteration,i);
      GECO( m(0,0,0),4,4,ipvt(0),rcond,work(0) ); // factor
      if( rcond==0. )
      {
	printf("IntersectionMapping::newtonIntersection:ERROR: condition number is zero! it=%i, i=%i, \n",iteration,i);
        matrix(i,R4,R4).display("Here is the matrix");
      }
      // if( max(fabs(x1r1(i,R3,R2)))<.01 || TRUE || Mapping::debug & 4 )
      //   printf("IntersectionMapping::newtonIntersection: it=%i, i=%i, rcond=%e, \n",iteration,i,rcond);
      GESL( m(0,0,0),4,4,ipvt(0),d(0,0),job);     // solve
      if( Mapping::debug & 4 )
      {
	printf("newtonIntersection: it=%i, i=%i, rcond=%e, max(fabs(delta)) = %e eps=%e \n",iteration,i,rcond,max(fabs(d)),eps);
      }
      delta(i,R4)=d(0,R4); 
      
    }
    r1(R,R2)+=delta(R,R2);
    r2(R,R2)+=delta(R,R2+2);

    const real deltaMax=max(fabs(delta));
    if( deltaMax < eps )
      break;
    else if( deltaMax>.4 )
    {
      printf("IntersectionMapping::newtonIntersection:ERROR: newton appears to be diverging. The maximum\n"
             "  correction to the r values is %e at iteration=%i \n",deltaMax,iteration);
      return 1;
    }
  }
  if( iteration>=maximumNumberOfIterations )
  {
    printf("newtonIntersection:ERROR no convergence, max(fabs(delta))=%e \n",
           max(fabs(delta)) );

    // kkc
    return 1;
  }
  x=.5*(x1+x2);

  return 0;
}


int IntersectionMapping::
project( realArray & x,
	 int & iStart, 
	 int & iEnd,
	 periodicType periodic)
// =====================================================================================================
/// \details  
///      Project the points x(iStart:iEnd,0:6) onto the intersection
///   NOTE: When the points are projected onto the curves it is possible that points
///       fold back on themselves if they get out of order. This routine will try and
///       detect this situation and it may remove some points to fix the problem.
///  Return values: 0 for success, otherwise failure.
// =====================================================================================================
{
// AP: The only information extracted from the curve seems to be the periodicity. Pass in that info instead
//    if( curve==NULL )
//    {
//      cout << "IntersectionMapping::project:Error: The intersection curve has not been generated yet \n";
//      exit(1);    
//    }

  if( Mapping::debug & 2 )
    printf("IntersectionMapping::project: project points exactly onto the surface\n");

//  periodicType periodic=curve->getIsPeriodic(axis1);
  if( Mapping::debug & 2 )  printf("IntersectionMapping::project: periodic = %i \n",periodic);

//  int iStart=x.getBase(0);
//  int iEnd  =x.getBound(0);

  Range R(iStart,iEnd);
  realArray xN(R,3),r1(R,2),r2(R,2),normal(R,3);
  Range R2(0,1),R3(0,2);

  Range I(iStart+1,iEnd-1);  // interior points
  normal(I,R3)=x(I+1,R3)-x(I-1,R3);     // normal to curve, defines a plane for the newton method
  // assign normals on end points
  normal(iStart,R3)=periodic ? x(iStart+1,R3)-x(iEnd-1,R3) : x(iStart+1,R3)-x(iStart,R3);
  normal(iEnd  ,R3)=periodic ? x(iStart+1,R3)-x(iEnd-1,R3) : x(iEnd    ,R3)-x(iEnd-1,R3);

  xN(R,R3)=x(R,R3);
  // the r values maybe be in the range [-1,2] for periodic domains
  int axis;
  if( TRUE )
  {
    for( axis=axis1; axis<=axis2; axis++ )
    {
      if( map1->getIsPeriodic(axis) )
	r1(R,axis)= mod( x(R,axis+3)+2., 1.);  // periodic shift into [0,1], add 2 to handle negative values
      else
	r1(R,axis)= x(R,axis+3);
      if( map2->getIsPeriodic(axis) )
	r2(R,axis)= mod( x(R,axis+5)+2., 1.);
      else
	r2(R,axis)= x(R,axis+5);
    }
  }
  else
  {
    r1(R,R2)= mod( x(R,R2+3)+2., 1.);  // periodic shift into [0,1], add 2 to handle negative values
    r2(R,R2)= mod( x(R,R2+5)+2., 1.);
  }
  
  int failure = newtonIntersection(xN, r1, r2, normal );
  if( failure )
    return 1;

  // check for points that reversed order
  // dot(i) =  D+x(i+1) dot D+ x(i)  : this should be positive except at sharp corners
  //        if dot(i) is not positive at two consequetive points then we likely have points reversed.
#define DOT(i) (xN((i)+1,0)-xN((i),0))*(xN((i),0)-xN((i)-1,0)) +  \
  	       (xN((i)+1,1)-xN((i),1))*(xN((i),1)-xN((i)-1,1)) +  \
	       (xN((i)+1,2)-xN((i),2))*(xN((i),2)-xN((i)-1,2))

  Range Rm = Range(iStart+1,iEnd-1); // one less point
  realArray dot(R);
  dot(iStart)=dot(iEnd)=1.;
  dot(Rm) =DOT(Rm);
  
  if( Mapping::debug & 2 ) 
    printf("---IntersectionMapping: iStart=%i, iEnd=%i, min(fabs(dot)) = %e \n",iStart,iEnd,min(fabs(dot(Rm))));

  const real xDiff =max(x(R,R3))-min(x(R,R3));
  const real epsX = xDiff*1.e-4; // discard points if they are this close together
  if( TRUE )
  {
    for( int i=iStart+1; i<=iEnd-1; i++ )
    {
      if( dot(i)<=0. )
      {
        printf(" points near dot<=0 : ");
	for( int j=max(iStart+1,i-3); j<=min(iEnd-1,i+3); j++ )
	  printf("dot(%i)=%7.3e, ",j,dot(j));
	printf("\n");
      }
      
      if( max(fabs(xN(i,R3)-xN(i-1,R3))) < epsX )
      {
        printf("---IntersectionMapping: Project: point %i is very close to point %i, discarding\n",i,i-1);
	// remove point i
	xN(Range(i,iEnd-1),R3)=xN(Range(i+1,iEnd),R3);
	r1(Range(i,iEnd-1),R2)=r1(Range(i+1,iEnd),R2);
	r2(Range(i,iEnd-1),R2)=r2(Range(i+1,iEnd),R2);
	x(Range(i,iEnd-1),Range(0,6))=x(Range(i+1,iEnd),Range(0,6));  // x is used below in periodic adjustment!
	// recompute dot and go back and re-check to see if the points are ok
	dot(i-1)=DOT(i-1);
	dot(i)=DOT(i);
        if( i+2<=iEnd )
          dot(Range(i+1,iEnd-1))=dot(Range(i+2,iEnd));
	i--;
	iEnd--;
	R=Range(iStart,iEnd);
      }
      else if( dot(i)<=0. && ( dot(i+1)<=0. || (i<iEnd-2 && dot(i+2)<=0.)) ) // also check for two point reversed
      {
	// The points are probably overlapped as below:
	//           i-1          i
	//     -------x-----------x
	//                  x-------------x--------
	//                 i+1           i+2
	printf("---IntersectionMapping: Project: there seems to be points in the wrong order! remove point i=%i,\n"
	       " dot(i=%i) = %e, dot(i=%i)=%e, xN(i) = (%7.3e,%7.3e,%7.3e), xN(i+1) = (%7.3e,%7.3e,%7.3e), "
               "iStart=%i, iEnd=%i\n"
	       ,i+1,i,dot(i),i+1,dot(i+1),xN(i,0),xN(i,1),xN(i,2),xN(i+1,0),xN(i+1,1),xN(i+1,2),iStart,iEnd);
	// remove point i+1 
	xN(Range(i+1,iEnd-1),R3)=xN(Range(i+2,iEnd),R3);
	r1(Range(i+1,iEnd-1),R2)=r1(Range(i+2,iEnd),R2);
	r2(Range(i+1,iEnd-1),R2)=r2(Range(i+2,iEnd),R2);
	x(Range(i+1,iEnd-1),Range(0,6))=x(Range(i+2,iEnd),Range(0,6));  // x is used below in periodic adjustment!
	// recompute dot and go back and re-check to see if the points are ok
	dot(i)=DOT(i);
	dot(i+1)=DOT(i+1);
        if( (i+3)<=iEnd )
          dot(Range(i+2,iEnd-1))=dot(Range(i+3,iEnd));
	i--;
	iEnd--;
	R=Range(iStart,iEnd);
      }
      else if( dot(i)<=0. )
      {
	printf("---IntersectionMapping: Project: WARNING: There is a sharp corner in the intersection curve!\n");
        printf("   dot(%i)=%e dot(i=%i)=%e, dot(%i)=%e, max(fabs(xN(i,R3)-xN(i-1,R3)))=%e \n",
             i-1,dot(i-1),i,dot(i),i+1,dot(i+1),max(fabs(xN(i,R3)-xN(i-1,R3))));
      }
    }
  }
  if( Mapping::debug & 2 )
  {
    for( int i=iStart; i<=iEnd; i++ )
    {
      printf("After newton: i=%i, x(before) = (%e,%e,%e), \n"
	     "                     x(after) = (%e,%e,%e)  \n",i,x(i,0),x(i,1),x(i,2),
	     xN(i,0),xN(i,1),xN(i,2));
    }
  }
  
  // shift r values back to their original domain
  if( TRUE )
  {
    for( axis=axis1; axis<=axis2; axis++)
    {
      if( map1->getIsPeriodic(axis) )
	r1(R,axis)+=  x(R,axis+3) - mod( x(R,axis+3)+2., 1.);
      if( map2->getIsPeriodic(axis) )
	r2(R,axis)+=  x(R,axis+5) - mod( x(R,axis+5)+2., 1.);
    }
  }
  else
  {
    // shift r values back to their original domain
    r1(R,R2)+=  x(R,R2+3) - mod( x(R,R2+3)+2., 1.);
    r2(R,R2)+=  x(R,R2+5) - mod( x(R,R2+5)+2., 1.);
  }
  
  x(R,R3)  =xN(R,R3);
  x(R,R2+3)=r1(R,R2);
  x(R,R2+5)=r2(R,R2);

  if( periodic )
    x(iEnd,R3)=x(iStart,R3);
  
  return 0;
  
}



int IntersectionMapping::
determineIntersection(GenericGraphicsInterface *gi /* =NULL */,
    	              GraphicsParameters & params /* =nullGraphicsParameters */ )
// =====================================================================================================
/// \details  
///    This is a protected routine to determine the intersection curve(s) between two surfaces.
/// 
/// \param Notes:
/// 
///   (1) First obtain an initial guess to the intersection: Using the bounding boxes that cover
///       the surface to determine a list of pairs of (leaf) bounding boxes that intersect. Triangulate
///       the surface quadrilaterals that are found in this "collision" list and find all line segments
///       that are formed when two triangles intersect.
/// 
///   (2) Join the line segments found in step 1 into a continuous curve(s). There will be three 
///     different intersection curves -- a curve in the Range space (x) and a curve in each of the
///     domain spaces (r). Since the domain spaces may be periodic it may be necessary to shift 
///     parts of the domain-space curves by +1 or -1 so that the curves are continuous. Note that
///     the domain curves will sometimes have to be outside the unit square. It is up to ?? to map
///     these values back to [0,1] if they are used.
///     
///   (3) Now fit a NURBS curve to all of the intersection curves, using chord-length of the space-curve
///       to parameterize the three curves.
///   (4) Re-evaluate the points on the curve using Newton's method to obtain the points that are exactly
///       on on the intersection of the surfaces. Refit the NURBS curves using these new points. 
/// 
/// \return  0 for success, otherwise failure.
// =====================================================================================================
{
  
  // Mapping::debug=3;

  // make sure the bounding boxes and the grid are made (even for mappings with analytic inverses)
  map1->approximateGlobalInverse->initialize(); 
  map2->approximateGlobalInverse->initialize(); 
  
  // kkc 020802 First check 5 points on the two surfaces to make sure the
  //            two surfaces do not coincide. (for example, two coincident planes.)
  realArray r(5,2);
  realArray x1(5,3),x2(5,3);
  r(0,0) = r(0,1) = 0.;
  r(1,0) = r(1,1) = 1.;
  r(2,0) = 0.; r(2,1) = 1.;
  r(3,0) = 1.; r(3,1) = 0.;
  r(4,0) = r(4,1) = .5;

  // get 5 points from the first surface
  map1->map(r,x1);
  // project those first 5 points onto the second surface
  map2->inverseMap(x1,r);
  map2->map(r,x2);
  // compare the distances between x2 and x1, if the max is less than 10*FLT_MIN, 
  // the surfaces are the same.
  if ( max(fabs(x2-x1)) < 10*FLT_EPSILON ) 
    {
      printf("IntersectionMapping:: surfaces coincide!\n");
      return 1;
    }


  // Make a collision list of all leaf node bounding boxes of map1 that intersect leaf
  // leaf node bounding boxes of map2

  Range R2(0,1),R3(0,2),R(0,6);

  typedef BoundingBox BoundingBoxTreePtr[3];
  
  BoundingBoxTreePtr *bTree1 = map1->approximateGlobalInverse->boundingBoxTree;
  BoundingBoxTreePtr *bTree2 = map2->approximateGlobalInverse->boundingBoxTree;

  BoundingBoxStack boxStack;
  
  int numPtsGrid1 = map1->getGridDimensions(0)*map1->getGridDimensions(1);
  int numPtsGrid2 = map2->getGridDimensions(0)*map2->getGridDimensions(1);

  const int maxNumberOfIntersectionBoxes=max(numPtsGrid1+numPtsGrid2,1000);
  printF("IntersectionMapping: numPtsGrid1=%i numPtsGrid2=%i maxNumberOfIntersectionBoxes=%i\n",
          numPtsGrid1,numPtsGrid2,maxNumberOfIntersectionBoxes);

  BoundingBox *intersectionBoxList[maxNumberOfIntersectionBoxes];
  int numberOfCollisions=0;

  // make a box with the top box from both trees
  boxStack.push( *createBoundingBox(bTree1[0][0],bTree2[0][0]) );

  int numberOfBoxesChecked=0;
  while( !boxStack.isEmpty() )
  {
    numberOfBoxesChecked++;

    BoundingBox & box0=boxStack.pop();             // get a box off the stack
    BoundingBox & box1 = *box0.child1;
    BoundingBox & box2 = *box0.child2;
   
    if( box1.child1==NULL )
    {
      if( box2.child1==NULL )
      {
	// save these two leaf nodes in the collision list
	intersectionBoxList[numberOfCollisions++]=createBoundingBox(box1,box2);
        assert( numberOfCollisions<maxNumberOfIntersectionBoxes );
      }
      else
      {
	if( box1.intersects(*box2.child1) )
	  boxStack.push( *createBoundingBox(box1,*box2.child1) );

	if( box1.intersects(*box2.child2) )
	  boxStack.push( *createBoundingBox(box1,*box2.child2) );

      }
    }
    else if(  box2.child1==NULL )
    {
      if( box1.child1->intersects(box2) )
	 boxStack.push( *createBoundingBox(*box1.child1,box2) );

      if( box1.child2->intersects(box2) )
	 boxStack.push( *createBoundingBox(*box1.child2,box2) );

    }
    else
    {
      if( box1.child1->intersects(*box2.child1) )
	boxStack.push( *createBoundingBox(*box1.child1,*box2.child1) );

      if( box1.child2->intersects(*box2.child1) )
	boxStack.push( *createBoundingBox(*box1.child2,*box2.child1) );

      if( box1.child1->intersects(*box2.child2) )
	boxStack.push( *createBoundingBox(*box1.child1,*box2.child2) );

      if( box1.child2->intersects(*box2.child2) )
	boxStack.push( *createBoundingBox(*box1.child2,*box2.child2) );
    }
    
    destroyBoundingBox(box0);
  }

  if( Mapping::debug & 2 )
    printf("IntersectionMapping::determineIntersection: numberOfBoxesChecked=%i,numberOfCollisions=%i \n",
	   numberOfBoxesChecked,numberOfCollisions);
  
  MappingParameters mapParams1,mapParams2;  // use these to get the mask for Trimmed surfaces

  const realArray & grid1 = map1->getGrid(mapParams1);
  const realArray & grid2 = map2->getGrid(mapParams2);

  // On TrimmedMappings we use the mask array which indicates unused points on the grid.
  intArray & mask1 = mapParams1.mask;
  intArray & mask2 = mapParams2.mask;
  bool maskPoints1=FALSE, maskPoints2=FALSE;
  if( mask1.getLength(0) > 0 )
  {
    maskPoints1=TRUE;
    mask1.reshape(grid1.dimension(0),grid1.dimension(1));
  }
  if( mask2.getLength(0) > 0 )
  {
    maskPoints2=TRUE;
    mask2.reshape(grid2.dimension(0),grid2.dimension(1));
  }
  

/* ---
  bTree1[0][0].domainBound.display("bTree1[0][0].domainBound");
  grid1.display(" grid 1 ");
  bTree2[0][0].domainBound.display("bTree2[0][0].domainBound");
  grid2.display(" grid 2 ");
--- */  

  // 
  int numberOfSegments=0;
  real xa[3], xb[3];

  int maxNumberOfSegments=1000;  // 2012/06/22
  realArray seg(maxNumberOfSegments,7,2);  // 0:6 holds (x,y,z) (r1,r2) (r1,r2)
  int iv1[2], iv2[2];
  int & i1 = iv1[0];
  int & i2 = iv2[0];
  int & j1 = iv1[1];
  int & j2 = iv2[1];

  
  real eps=REAL_EPSILON*10.;  
  // we also need a relative epsilon for measuring in physical space
  const RealArray & bb1 = map1->approximateGlobalInverse->getBoundingBox();
  const RealArray & bb2 = map2->approximateGlobalInverse->getBoundingBox();
    
  real xScale = max( max(bb1(End,R3)-bb1(Start,R3)), max( bb2(End,R3)-bb2(Start,R3)) );
  if( Mapping::debug & 4 )
    printf(" *** xScale=%e \n",xScale);
  real epsX= eps*xScale;
  
  int collision;
  
  // kkc surfaces may coincide only on part of the domain, the assertion usually fails in this case
  //     instead of aborting, set an error and return -2;
  bool intersectionIsOK = true;
  
  for( collision=0; collision<numberOfCollisions && intersectionIsOK; collision++ )
  {
    
    BoundingBox & box = *intersectionBoxList[collision];
    BoundingBox & box1 = *box.child1;
    BoundingBox & box2 = *box.child2;

    int t1=0;
    const int maxNumberOfTriangles=100;
    Triangle *triangleList1[maxNumberOfTriangles], *triangleList2[maxNumberOfTriangles];
    int ti;
    for( ti=0; ti<maxNumberOfTriangles; ti++ )
    {
      triangleList1[ti]=triangleList2[ti]=NULL;
    }
    int i3=0;
    for( i1=box1.domainBound(Start,axis1); i1<=box1.domainBound(End,axis1)-1; i1++ )
    {
      for( i2=box1.domainBound(Start,axis2); i2<=box1.domainBound(End,axis2)-1; i2++ )
      {
        if( maskPoints1 && 
            (mask1(i1,i2,i3)==0 || mask1(i1+1,i2,i3)==0 || mask1(i1,i2+1,i3)==0 || mask1(i1+1,i2+1,i3)==0 ) )
	  continue;
	triangleList1[t1++]= new Triangle( grid1,i1,i2,i3,0 );
	triangleList1[t1++]= new Triangle( grid1,i1,i2,i3,1 );
      }
      assert( t1<maxNumberOfTriangles );
    } 
    int t2=0;
    for( i1=box2.domainBound(Start,axis1); i1<=box2.domainBound(End,axis1)-1; i1++ )
    {
      for( i2=box2.domainBound(Start,axis2); i2<=box2.domainBound(End,axis2)-1; i2++ )
      {
        if( maskPoints2 && 
            (mask2(i1,i2,i3)==0 || mask2(i1+1,i2,i3)==0 || mask2(i1,i2+1,i3)==0 || mask2(i1+1,i2+1,i3)==0 ) )
	  continue;
	triangleList2[t2++]= new Triangle( grid2,i1,i2,i3,0 );
	triangleList2[t2++]= new Triangle( grid2,i1,i2,i3,1 );
      }
      assert( t2<maxNumberOfTriangles );
    }
    // find any intersections between triangle1 and triangle2
    
    real dr[2][2];
    if( false )
    {
      dr[0][0]=1./(max(1,grid1.getBound(0)-grid1.getBase(0)));
      dr[0][1]=1./(max(1,grid1.getBound(1)-grid1.getBase(1)));

      dr[1][0]=1./(max(1,grid2.getBound(0)-grid2.getBase(0)));
      dr[1][1]=1./(max(1,grid2.getBound(1)-grid2.getBase(1)));
    }
    else
    { // new ay -- *wdh* 070319
      for( int dir=0; dir<=1; dir++ )
      {
	dr[0][dir]=1./max(1,map1->getGridDimensions(dir)-1);
	dr[1][dir]=1./max(1,map2->getGridDimensions(dir)-1);
      }
    }
    

    
/* ----
   for( t1=-1; t1<numberOfTriangles1-1;  )
   {
   for( int m1=0; m1<=1; m1++ )
   {
   t1++;
   Triangle & triangle1 = *triangleList1[t1];
   for( t2=-1; t2<numberOfTriangles2-1;  )
   {
   for( int m2=0; m2<=1; m2++ )
   {
   t2++;
   ----- */
    if( debug & 4 )
      printf(" collision %i: number of triangles to compare: t1=%i, t2=%i \n",collision,t1,t2);
    t1=-1;

    for( i1=box1.domainBound(Start,axis1); i1<=box1.domainBound(End,axis1)-1 && intersectionIsOK; i1++ )
    {
      for( i2=box1.domainBound(Start,axis2); i2<=box1.domainBound(End,axis2)-1 && intersectionIsOK; i2++ )
      {
        if( maskPoints1 && 
            (mask1(i1,i2,i3)==0 || mask1(i1+1,i2,i3)==0 || mask1(i1,i2+1,i3)==0 || mask1(i1+1,i2+1,i3)==0 ) )
	  continue;
        for( int m1=0; m1<=1 && intersectionIsOK; m1++ )
	{
          t1++;
	  Triangle & triangle1 = *triangleList1[t1];
	  t2=-1;
	  for( j1=box2.domainBound(Start,axis1); j1<=box2.domainBound(End,axis1)-1 && intersectionIsOK; j1++ )
	  {
	    for( j2=box2.domainBound(Start,axis2); j2<=box2.domainBound(End,axis2)-1 && intersectionIsOK; j2++ )
	    {
	      if( maskPoints2 && 
		  (mask2(i1,i2,i3)==0 || mask2(i1+1,i2,i3)==0 || mask2(i1,i2+1,i3)==0 || mask2(i1+1,i2+1,i3)==0 ) )
		continue;
	      for( int m2=0; m2<=1 && intersectionIsOK; m2++ )
	      {
                t2++;
		if( triangle1.intersects( *triangleList2[t2], xa,xb ) )
		{
		  // save the line segment
		  if( Mapping::debug & 2 )
		    printf(" intersecting segment: xa=(%g,%g,%g) to xb=(%g,%g,%g)\n",
			   xa[0],xa[1],xa[2],xb[0],xb[1],xb[2]);
		  if( max(fabs(xb[0]-xa[0]),max(fabs(xb[1]-xa[1]),fabs(xb[2]-xa[2]))) < epsX )
		  {
		    if( Mapping::debug & 2 )
		      printf(" discarding segment of zero or near zero length \n");
		  }
		  else
		  {
		    seg(numberOfSegments,0,0)=xa[0];
		    seg(numberOfSegments,1,0)=xa[1];
		    seg(numberOfSegments,2,0)=xa[2];

		    seg(numberOfSegments,0,1)=xb[0];
		    seg(numberOfSegments,1,1)=xb[1];
		    seg(numberOfSegments,2,1)=xb[2];

		    // now find the unit square coordinates
		    for( int n=0; n<=1; n++ )  // do coordinate system for curve1 and curve 2
		    {
		      Triangle & tri =  n==0 ? triangle1 : *triangleList2[t2];
		    
		      for( int ab=0; ab<=1; ab++ )  // do xa and xb
		      {
			real *xx = ab==0 ? xa : xb;
		      
			// find alpha1, alpha2 such that xx-x1 = alpha1*(x2-x1) + alpha2*(x3-x1)
			// where the triangle has vertices (x1,x2,x3)
			real alpha1,alpha2;
			tri.getRelativeCoordinates(xx,alpha1,alpha2 );

			if( (n==0 && m1==1) || (n==1 && m2==1) ) // when mi==1 the triangle is upper right quad
			{
			  alpha1=1.-alpha1;
			  alpha2=1.-alpha2;
			}
			seg(numberOfSegments,3+2*n,ab)=(iv1[n]+alpha1)*dr[n][0];
			seg(numberOfSegments,4+2*n,ab)=(iv2[n]+alpha2)*dr[n][1];
		    
		      }
		    }
		    numberOfSegments++;
		    // kkc assert( numberOfSegments<1000 );
		    if( numberOfSegments>=maxNumberOfSegments )
		    {
                      if( maxNumberOfSegments>1000000 )
		      {
                        printF("IntersectionMapping::ERROR: maxNumberOfSegments=%i ! is something wrong here?\n",maxNumberOfSegments);
                        intersectionIsOK=false;
		      }
		      else
		      {
			maxNumberOfSegments = maxNumberOfSegments*2;
			seg.resize(maxNumberOfSegments,7,2);
			printF("IntersectionMapping::WARNING: increasing maxNumberOfSegments to %i.\n",maxNumberOfSegments);
		      }
		      
		    }
		    
		  }
		}
	      }
	    }
	  }
	}
      }
    }
    for( ti=0; ti<maxNumberOfTriangles; ti++ )
    {
      delete triangleList1[ti];
      delete triangleList2[ti];
    }
  }
  
   
  for( collision=0; collision<numberOfCollisions; collision++ )
    destroyBoundingBox(*intersectionBoxList[collision]);  // 990905
  

  if ( !intersectionIsOK ) 
    {
      printf(" The intersection was peculiar, perhaps the surfaces overlap? ");
      return -2;
    }

  if( numberOfSegments==0 )
  {
    if( Mapping::debug & 1 ) printf(" No intersecting segments were found! \n");
    return 1;
  }
  if( Mapping::debug & 2 )
    printf(" number of intersecting segments = %i\n",numberOfSegments);
  
// sort the segments into a polygon
// AP save the result in the array x, starting at iStart ending at iEnd

  realArray x(Range(-(numberOfSegments+1),(numberOfSegments+1)),R);
  IntegerArray used(numberOfSegments);
  used=0;

  int i, iStart, iEnd;

  int subCurveToChoose=4;  // look for at the most this number of subCurves
  int segmentToStartFrom=0;
  int numberOfSegmentsRemaining=numberOfSegments;
  Range xAxes(0,2), Ri;
  realArray r1, uBar;
 
// allocate the NURBS that will become curve, rcurve1, rcurve2
  NurbsMapping & nurbs = *new NurbsMapping; nurbs.incrementReferenceCount();
  NurbsMapping & nurbs1 = *new NurbsMapping; nurbs1.incrementReferenceCount();
  NurbsMapping & nurbs2 = *new NurbsMapping; nurbs2.incrementReferenceCount();

  for( int subCurve=0; subCurve<subCurveToChoose; subCurve++ ) // AP changed to C-indexing
  {
    if( Mapping::debug & 2 ) printf("IntersectionMapping: Building subCurve %i\n", subCurve);

    // kkc x can be redimmed in reorder making it smaller than it needs to be on subsequent segments
    x.resize(Range(-(numberOfSegments+1),(numberOfSegments+1)),R);
    
    iStart=segmentToStartFrom;

    // kkc 311002 add min because segmentToStartFrom might be the last segment
    //            this can happen when the loop to check for additional subcurves finds
    //            only the last segment. this loop is at // kkc THIS IS THE LOOP I MEAN 311002
    iEnd=min(segmentToStartFrom+1, numberOfSegments);


    x(iStart,R)=seg(segmentToStartFrom,R,0);
    x(iEnd  ,R)=seg(segmentToStartFrom,R,1);
    used(segmentToStartFrom)=1;  // segment 0 has been used

    bool done=FALSE;
    while( !done )
    {
      done=TRUE;
      for( int j=0; j<numberOfSegments; j++ )
      {
	if( !used(j) )
	{
//  	 printf(" checking seg(%i) =  (%e,%e,%e),  (%e,%e,%e)\n",j,
//  	       seg(j,0,0),seg(j,1,0),seg(j,2,0),seg(j,0,1),seg(j,1,1),seg(j,2,1));
	
	  if( max(fabs(seg(j,xAxes,0)-x(iStart,xAxes)))< epsX )
	  {
	    iStart--;
	    x(iStart,R)=seg(j,R,1);
	    used(j)=1; done=FALSE;
	  }
	  else if( max(fabs(seg(j,xAxes,1)-x(iStart,xAxes)))< epsX )
	  {
	    iStart--;
	    x(iStart,R)=seg(j,R,0);
	    used(j)=1; done=FALSE;
	  }
	  else if( max(fabs(seg(j,xAxes,0)-x(iEnd  ,xAxes)))< epsX )
	  {
	    iEnd++;
	    x(iEnd,R)=seg(j,R,1);
	    used(j)=1; done=FALSE;
	  }
	  else if( max(fabs(seg(j,xAxes,1)-x(iEnd  ,xAxes)))< epsX )
	  {
	    iEnd++;
	    x(iEnd,R)=seg(j,R,0);
	    used(j)=1; done=FALSE;
	  }
	  assert( iStart>-(numberOfSegments+1) && iEnd<(numberOfSegments+1) );
	}
      }
    }

// AP count the number of remaining segments. Necessary to do before the call to reduce().
    numberOfSegmentsRemaining-=iEnd-iStart;

    if( Mapping::debug & 4 && gi!=NULL ) // Mapping::debug & 2 && gi!=NULL && iEnd-iStart !=numberOfSegments )
    {
      // plot the segment
      printf( "plot subCurve %i, no. of segments=%i, total = %i \n",subCurve, iEnd-iStart,numberOfSegments);

      DataPointMapping dpm;
      realArray x1;
      x1.redim(iEnd-iStart+1,3);
      x1=x(Range(iStart,iEnd),Range(0,2));

      GraphicsParameters localParameters(TRUE);  // TRUE means this is gets default values
      GraphicsParameters & gparams = params.isDefault() ? localParameters : params;

//    dpm.setDataPoints(x(Range(iStart,iEnd),Range(0,2)),1,1);
      dpm.setDataPoints(x1,1,1);
      if(subCurve % 2 == 1 )
        gparams.set(GI_MAPPING_COLOUR,"green");
      else if( subCurve==2 )
        gparams.set(GI_MAPPING_COLOUR,"yellow");

      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      PlotIt::plot(*gi,dpm,gparams);
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    }

// AP: Is this test really necessary anymore???
//    if( iEnd-iStart+1 != numberOfSegments+1 )
//    {
//      if( Mapping::debug & 2 )
//        printf("WARNING some segments were not added to the polygon\n");
//      for( int j=0; j<numberOfSegments; j++ )
//      {
//        if( !used(j) )
//        {
//  	printf(" unused: seg(%i) =  (%e,%e,%e),  (%e,%e,%e)\n",j,
//  	       seg(j,0,0),seg(j,1,0),seg(j,2,0),seg(j,0,1),seg(j,1,1),seg(j,2,1));
//        }
//      }
//    }

//
// AP: Reorder the array x to compensate for periodicity in parameter space
//

    periodicType periodic;
    periodicType curveIsPeriodic[2];  // for two parameter curves

    // kkc
    realArray xtmp;
    Ri=Range(iStart,iEnd);
    xtmp = x(Ri,R);
    
#if 0
    reOrder(xtmp, iStart, iEnd, periodic, curveIsPeriodic, epsX);
    //    reduce(xtmp, iStart, iEnd);
    x(Ri,R) = xtmp;
    reduce(x,iStart,iEnd);

#else
    reOrder(x, iStart, iEnd, periodic, curveIsPeriodic, epsX);

    reduce(x, iStart, iEnd);
#endif
  
    Ri=Range(iStart,iEnd);

//
// AP: Make the NURBS curves from the arrays x and r1. Also save the parametrization in uBar
//

  // Fit a nurb to the curve, (paramerized by chord length)

//
// AP: Modify the NURBS to store all disjoint curves in subcurves in one NURBS curve
//
    // *wdh* need to fix reference counting here ***
    NurbsMapping *sc, *sc1, *sc2;
    if (subCurve == 0)
    {
      sc = &nurbs;
      sc1 = &nurbs1;
      sc2 = &nurbs2;
    }
    else
    {
      sc = new NurbsMapping(1, 3); 
      sc1 = new NurbsMapping(1, 2);
      sc2 = new NurbsMapping(1, 2);
    }
    
    sc->interpolate(x(Ri,R3),1,uBar);  // interpolate and retrieve parameterization, uBar

    // Nurbify the parameters curves, use same parameterization as above

    r1.redim(iEnd-iStart+1,2);
    r1=x(Ri,Range(3,4));
    sc1->interpolate(r1,0,uBar);  // use the parameterization uBar


    // This next value is scaled with the maximum length of any segment 
    // We declare the curve to be periodic if the distance between the first and
    // last point is less than this fraction of the max dist between consecutive pts on the curve.
    const real epsPeriodic=.25;  

    Range Rm(iStart,iEnd-1);
    const real dsMax1=max( fabs(x(Rm+1,3)-x(Rm,3))+fabs(x(Rm+1,4)-x(Rm,4)) );
    const real endDist1=fabs(x(iStart,3)-x(iEnd,3))+fabs(x(iStart,4)-x(iEnd,4));
    curveIsPeriodic[0] =  endDist1< epsPeriodic*dsMax1 ? functionPeriodic : notPeriodic;

    r1=x(Ri,Range(5,6));
    sc2->interpolate(r1,0,uBar);

    const real dsMax2=max( fabs(x(Rm+1,5)-x(Rm,5))+fabs(x(Rm+1,6)-x(Rm,6)) );
    const real endDist2=fabs(x(iStart,5)-x(iEnd,5))+fabs(x(iStart,6)-x(iEnd,6));
    curveIsPeriodic[1] =  endDist2< epsPeriodic*dsMax2 ? functionPeriodic : notPeriodic;

    if( (curveIsPeriodic[0]!=notPeriodic) || (curveIsPeriodic[1]!=notPeriodic) )
    {
      // if one parameter curve is periodic then all curves mut be either function or derivative periodic
      periodic=functionPeriodic;
      if( !curveIsPeriodic[0] ) curveIsPeriodic[0]=derivativePeriodic;
      if( !curveIsPeriodic[1] ) curveIsPeriodic[1]=derivativePeriodic;
      
    }
    sc1->setIsPeriodic(axis1,curveIsPeriodic[0]);
    sc2->setIsPeriodic(axis1,curveIsPeriodic[1]);
    
    
    if( Mapping::debug & 2 )
    {
      printf("***space curve periodic=%i, rCurve1 periodic=%i (endDist1=%8.2e)(eps1=%8.2e), "
             "rCurve2 periodic=%i (endDist2=%8.2e)(eps2=%8.2e)\n",periodic,
	     curveIsPeriodic[0],endDist1,epsPeriodic*dsMax1, curveIsPeriodic[1],endDist2,epsPeriodic*dsMax2 );
    }
    

    if( Mapping::debug & 2 )
      printf("Fit a nurbs to the curves, (paramerized by chord length) \n");


// ** done above     sc->interpolate(x(Ri,R3),1,uBar);  // retrieve parameterization, uBar
    if( periodic )
      sc->setIsPeriodic(axis1,functionPeriodic);

    // re-evaluate the curves at equi-arclength positions
    if( Mapping::debug & 2 )
      printf("re-evaluate the curves at equi-arclength positions \n");

  // We project the points. We may have to iterate if the number of points changes and project twice.
  // Maybe we should iterate always?
    for( int it=0; it<=1; it++ )
    {
      realArray r(iEnd-iStart+1,1), x1(iEnd-iStart+1,3);

      real dr = 1./(max(1,iEnd-iStart));
      r.seqAdd(0.,dr);
      // r.display("Here is r for evaluating the nurbs");
  
      sc->map(r,x1);
      x(Ri,R3)=x1;
      if( sc->getIsPeriodic(axis1) )// used to be "periodic"
	x(iEnd,R3)=x(iStart,R3);
  
      sc1->map(r,r1);
      x(Ri,Range(3,4))=r1;
  
      sc2->map(r,r1);
      x(Ri,Range(5,6))=r1;
  
      // now project the points exactly onto the surface
      // **** note that the projected points may not stay in order if they are close together
      //      but far from the surface, here we assume that they do ****

      bool numberOfPointsChanged=FALSE;
    
      bool projectPoints=TRUE; // AP testing...
      if( projectPoints )
      {
	int failure = project(x,iStart,iEnd, periodic);
	if( failure )
	  return 1;
      
	Ri=Range(iStart,iEnd);
	if( r1.getLength(0) != iEnd-iStart+1 )
	{
	  numberOfPointsChanged=TRUE;
	  r1.resize(iEnd-iStart+1,R2);
	}
      }

      if( Mapping::debug & 4 )
      {
	printf(" AFTER PROJECT: polygon for intersection: iStart=%i, iEnd=%i \n",iStart,iEnd);
	for( i=iStart; i<=iEnd; i++ )
	  printf(" x(%i,.) = (%e,%e,%e) r1=(%5.3f,%5.3f), r2=(%5.3f,%5.3f)\n",i,
		 x(i,0),x(i,1),x(i,2),x(i,3),x(i,4),x(i,5),x(i,6));
      }

      // regenerate the nurbs now that the points have been projected
      // Keep the same parameterization for all curves

      if( Mapping::debug & 2 )
	printf("regenerate the nurbs now that the points have been projected \n");

      uBar.redim(0);
      sc->interpolate(x(Ri,R3),1,uBar);   // returns uBar

      r1=x(Ri,Range(3,4));
      sc1->interpolate(r1,0,uBar);                                // uses uBar
   
      r1=x(Ri,Range(5,6));
      sc2->interpolate(r1,0,uBar);                                // uses uBar
      if( !numberOfPointsChanged )
	break;
    } // end for it=0,1,...

// add in the subcurves in the main NURBS
    if (subCurve > 0)
    {
      nurbs.merge(*sc);
      nurbs1.merge(*sc1);
      nurbs2.merge(*sc2);
    }
    
  
// check if we need to make another subcurve...
    if( numberOfSegmentsRemaining==0 )
    {
      break;
    }
    else
    {
// look for another sub-curve
// AP      if( Mapping::debug & 2 )
      printf(" **** segments remain, looking for sub-curve %i \n",subCurve+1);
      segmentToStartFrom=-1;

      // kkc THIS IS THE LOOP I MEAN 311002
      // see comment above at // kkc 311002
      for( int j=0; j<numberOfSegments; j++ )
      {
	if( !used(j) )
	{
	  segmentToStartFrom=j;
	  break;
	}
      }
      assert( segmentToStartFrom>=0 );
    }

//
// AP: end subcurve construction loop here
//
  } // end for subCurve...
  
// save the pointers to the main nurbs
  curve = &nurbs;
  rCurve1 = &nurbs1;
  rCurve2 = &nurbs2;

/* ----
  r1.display(" Here is rCurve1 ");
  DataPointMapping & dpm2 = *new DataPointMapping;
  dpm2.setDataPoints(r1,1,1);
  rCurve1 = &dpm2;

//  NurbsMapping & nurbs1 = *new NurbsMapping;
//  nurbs1.interpolate(r1);
//  rCurve1 = &nurbs1;


  delete rCurve2;
  DataPointMapping & dpm3 = *new DataPointMapping;
  r1=x(Range(iStart,iEnd),Range(5,6));
  dpm3.setDataPoints(r1,1,1);
  rCurve2 = &dpm3;
--- */

  return 0;
}








int
equidistribute( const realArray & w, realArray & r );

int IntersectionMapping::
reparameterize(const real & arcLengthWeight /* =1. */, 
               const real & curvatureWeight /* =.2 */)
//=====================================================================================
/// \brief  
///     Redistribute points on the intersection curve to place more points where the
///   curvature is large. 
/// \details  The default distribution of points in the intersection curve
///   is equally spaced in arc length (really chord length). To cluster more points
///   near sharp corners, call this routine with a non-zero value for {\tt curvatureWeight}.
///   In this case the points will be placed to equidistribute the weight function
///   \begin{verbatim}
///       w(r) = 1 + arcLength(r)*arcLengthWeight + curvature(r)*curvatureWeight
///    where
///       arcLength(r) =  | x_r |
///       curvature(r) =  | x_rr |    (*** this is not really the curvature, but close ***)
///   \end{verbatim}
///  Note that the point distribution only depends on the ratio of arcLengthWeight to curvatureWeight 
///  and not on their absolute vaules. The weight function must be positive everywhere.
///  Also note that for the unit circle, $| x_r |=2\pi$ and $| x_{rr}|= (2\pi)^2$ so that the curvature
///  is naturally $2\pi$ times larger in the weight function.
/// 
/// \param arcLengthWeight (input) : weight for the arc length, should be positive.
/// \param curvatureWeight (input) : weight for the curvature, should normally be non-negative. 
//=====================================================================================
{
  int n = curve->getGridDimensions(axis1);
  Range R(0,n-1), R2(0,1), R3(0,2), R7(0,6);
  realArray r(n,1), x(R,R7), xr(n,3,1), w(n);
  
  real dr = 1./(max(1,n-1));
  r.seqAdd(0.,dr);
  r(n-1)=1.;
  curve->map(r,x,xr);
  
  // first compute the curvature in the interior
  Range I(1,n-2);
  w(I)= SQRT( SQR(xr(I+1,0,0)-xr(I-1,0,0))+SQR(xr(I+1,1,0)-xr(I-1,1,0))+SQR(xr(I+1,2,0)-xr(I-1,2,0)) );
  periodicType periodic=curve->getIsPeriodic(axis1);
  if( periodic )
  {
    w(0)= SQRT( SQR(xr(1,0,0)-xr(n-2,0,0))+SQR(xr(1,1,0)-xr(n-2,1,0))+SQR(xr(1,2,0)-xr(n-2,2,0)) );
    w(n-1)=w(0);
  }
  else
  {
    w(0)=w(1);
    w(n-1)=w(n-2);
  }
  if( Mapping::debug & 2 )
  {
    w.display("IntersectionMapping::reparameterize: here is the curvature");
    SQRT( SQR(xr(R,0))+SQR(xr(R,1))+SQR(xr(R,2)) ).display(" arclength ");
  }

  w(R)= w(R)*(curvatureWeight/(2.*dr)) + SQRT( SQR(xr(R,0))+SQR(xr(R,1))+SQR(xr(R,2)) ) *arcLengthWeight;

  if( Mapping::debug & 2 )
    w.display("IntersectionMapping::reparameterize: here is the weight function");

  // smooth out the weight function a bit:
  for( int it=0; it<=3; it++ )
  {
    w(I)= .5*w(I) + .25*(w(I+1)+w(I-1));    // under-relaxed Jacobi
    if( periodic )
    {
      w(0)=  .5*w(0) + .25*(w(1)+w(n-2));
      w(n-1)=w(0);
    }
    else
    {
      w(0)=w(1);
      w(n-1)=w(n-2);
    }
  }
  if( Mapping::debug & 2 )
    w.display("IntersectionMapping::reparameterize: here is the weight function after smoothing");
  

  equidistribute( w,r );

  if( Mapping::debug & 2 )
    r.display("IntersectionMapping::reparameterize: here is the equi-distributed r");

  // evaluate the three intersection curves at these new values of r
  curve->map(r,x);
  if( periodic )
    x(n-1,R3)=x(0,R3);

  realArray r1(n,2);
  rCurve1->map(r,r1);
  x(R,R2+3)=r1;
  rCurve2->map(r,r1);
  x(R,R2+5)=r1;
  
  // project all points onto the actual intersection
  int iStart=x.getBase(0), iEnd=x.getBound(0);
  project(x, iStart, iEnd, curve->getIsPeriodic(axis1));
  R=Range(iStart,iEnd);   // number of points could change.
  // project(x);
  
  // re-interpolate the nurbs, now supply a uniform parameterization
  if( Mapping::debug & 2 )
    printf("re-interpolate the nurbs with new points \n");
  
  r.seqAdd(0.,dr);
  ((NurbsMapping*)curve)->interpolate(x(R,R3),0,r);

  r1=x(R,Range(3,4));
  ((NurbsMapping*)rCurve1)->interpolate(r1,0,r);
  r1=x(R,Range(5,6));
  ((NurbsMapping*)rCurve2)->interpolate(r1,0,r);
  

  return 0;
}


int IntersectionMapping::
intersectCurves(Mapping & curve1, 
                Mapping & curve2, 
		int & numberOfIntersectionPoints, 
		realArray & r1, 
		realArray & r2,
		realArray & x )
// =====================================================================================================
/// \details  
///    Determine the intersection between two 2D curves.
/// 
/// \param curve1, curve2 (input) : intersect these curves
/// \param numberOfIntersectionPoints (output): the number of intersection points found.
/// \param r1,r2,x (output) : r1(i),r2(i),x(0:1,i) the intersection point(s) for $i=0,\ldots,numberOfIntersectionPoints-1$
///     are $curve1(r1(i))=curve2(r2(i))=x(i)$
///      
/// 
// =====================================================================================================
{
  int debugi=0; // 3;

  // tolerance for intersections of line segments (parameterized on [0,1])
  const real rTol=REAL_EPSILON*1000.;

  // make sure the bounding boxes and the grid are made (even for mappings with analytic inverses)
  curve1.approximateGlobalInverse->initialize(); 
  curve2.approximateGlobalInverse->initialize(); 
  
  // Make a collision list of all leaf node bounding boxes of curve1 that intersect leaf
  // leaf node bounding boxes of curve2

  Range R2(0,1);

  typedef BoundingBox BoundingBoxTreePtr[3];
  
  BoundingBoxTreePtr *bTree1 = curve1.approximateGlobalInverse->boundingBoxTree;
  BoundingBoxTreePtr *bTree2 = curve2.approximateGlobalInverse->boundingBoxTree;

  BoundingBoxStack boxStack;
  
  const int maxNumberOfIntersectionBoxes=1000;
  BoundingBox *intersectionBoxList[maxNumberOfIntersectionBoxes];
  int numberOfCollisions=0;

  // make a box with the top box from both trees
  boxStack.push( *createBoundingBox(bTree1[0][0],bTree2[0][0]) );

  int nparallel = 0; // the number of intersection points where the curves appear parallel

  int numberOfBoxesChecked=0;
  while( !boxStack.isEmpty() )
  {
    numberOfBoxesChecked++;

    BoundingBox & box0=boxStack.pop();             // get a box off the stack
    BoundingBox & box1 = *box0.child1;
    BoundingBox & box2 = *box0.child2;
   
    if( box1.child1==NULL )
    {
      if( box2.child1==NULL )
      {
	// save these two leaf nodes in the collision list
	intersectionBoxList[numberOfCollisions++]=createBoundingBox(box1,box2);
        assert( numberOfCollisions<maxNumberOfIntersectionBoxes );
      }
      else
      {
	if( box1.intersects(*box2.child1) )
	  boxStack.push( *createBoundingBox(box1,*box2.child1) );

	if( box1.intersects(*box2.child2) )
	  boxStack.push( *createBoundingBox(box1,*box2.child2) );

      }
    }
    else if(  box2.child1==NULL )
    {
      if( box1.child1->intersects(box2) )
	 boxStack.push( *createBoundingBox(*box1.child1,box2) );

      if( box1.child2->intersects(box2) )
	 boxStack.push( *createBoundingBox(*box1.child2,box2) );

    }
    else
    {
      if( box1.child1->intersects(*box2.child1) )
	boxStack.push( *createBoundingBox(*box1.child1,*box2.child1) );

      if( box1.child2->intersects(*box2.child1) )
	boxStack.push( *createBoundingBox(*box1.child2,*box2.child1) );

      if( box1.child1->intersects(*box2.child2) )
	boxStack.push( *createBoundingBox(*box1.child1,*box2.child2) );

      if( box1.child2->intersects(*box2.child2) )
	boxStack.push( *createBoundingBox(*box1.child2,*box2.child2) );
    }
    
    destroyBoundingBox(box0);
  }

  if( debugi & 2 )
    printf("IntersectionMapping::intersectCurves: numberOfBoxesChecked=%i,numberOfCollisions=%i \n",
	   numberOfBoxesChecked,numberOfCollisions);
  
  const realArray & grid1 = curve1.getGrid();
  const realArray & grid2 = curve2.getGrid();

/* ---
  bTree1[0][0].domainBound.display("bTree1[0][0].domainBound");
  grid1.display(" grid 1 ");
  bTree2[0][0].domainBound.display("bTree2[0][0].domainBound");
  grid2.display(" grid 2 ");
--- */  

  // 
  int numberOfIntersections=0;
  realArray seg(100,4);  // 0:3 holds (x,y) (r10) (r20)
  
  // we also need a relative epsilon for measuring in physical space
  const RealArray & bb1 = curve1.approximateGlobalInverse->getBoundingBox();
  const RealArray & bb2 = curve2.approximateGlobalInverse->getBoundingBox();
    
  real xScale = max( max(bb1(End,R2)-bb1(Start,R2)), max( bb2(End,R2)-bb2(Start,R2)) );
  if( debugi & 4 ) printf(" *** xScale=%e \n",xScale);

  real dr1 = 1./(curve1.getGridDimensions(axis1)-1);
  real dr2 = 1./(curve2.getGridDimensions(axis1)-1);

  real x1a[2], x1b[2], x2a[2], x2b[2];
  int collision;
  for( collision=0; collision<numberOfCollisions; collision++ )
  {
    
    BoundingBox & box = *intersectionBoxList[collision];
    BoundingBox & box1 = *box.child1;
    BoundingBox & box2 = *box.child2;

    int i1, i2=0, i3=0, j2=0, j3=0;
    for( i1=box1.domainBound(Start,axis1); i1<=box1.domainBound(End,axis1)-1; i1++ )
    {
      x1a[0]=grid1(i1  ,i2,i3,0);
      x1a[1]=grid1(i1  ,i2,i3,1);
      x1b[0]=grid1(i1+1,i2,i3,0);
      x1b[1]=grid1(i1+1,i2,i3,1);
      for( int j1=box2.domainBound(Start,axis1); j1<=box2.domainBound(End,axis1)-1; j1++ )
      {
	x2a[0]=grid2(j1  ,j2,j3,0);
	x2a[1]=grid2(j1  ,j2,j3,1);
	x2b[0]=grid2(j1+1,j2,j3,0);
	x2b[1]=grid2(j1+1,j2,j3,1);
        // check if the line segment (x1a,x1b) intersects (x2a,x2b)
        // Solve: dx1 r1 + x1a = dx2 r2 + x2a  for r1 and r2,   dx1=x1b-x1a, dx2=x2b-x2a
        // answer is (r10,r20)
        //   [ dx1[0] -dx2[0] ][ r1 ] = [ x2a-x1a ]
        //   [ dx1[1] -dx2[1] ][ r2 ] 
        real a11= x1b[0]-x1a[0],
             a21= x1b[1]-x1a[1],
             a12=-x2b[0]+x2a[0],
             a22=-x2b[1]+x2a[1],
	     b1= x2a[0]-x1a[0],
  	     b2= x2a[1]-x1a[1];
        real det=a11*a22-a12*a21;
        real r10,r20;
	bool intersects=FALSE;
        // if( det!=0. )
	if( fabs(det)>REAL_MIN*1000. )  // *wdh* To avoid nan's
	{
	  r10=( a22*b1-a12*b2)/det;
	  r20=(-a21*b1+a11*b2)/det;

	  // *wdh* 051011 intersects= r10>=0. && r10<=1. && r20>=0. && r20<=1.;
          // *wdh* 051011 : allow a small tolerance for intersections:
	  intersects= r10>=-rTol && r10<=1.+rTol && r20>=-rTol && r20<=1.+rTol;
	}
	else
	{
          // lines are parallel
          if( debugi & 2 ) printf("intersectCurves:INFO: The line segments are parallel \n");
          //  r10=b1/a11 or b2/a21
          //  r20=b1/a12 or b2/a22
          if( fabs(a11)+fabs(a21) > 0. )
    	    intersects= fabs(a21*b1-a11*b2) < REAL_EPSILON*10.;
	  else if( fabs(a12)+fabs(a22) > 0. )
    	    intersects= fabs(a12*b2-a22*b1) < REAL_EPSILON*10.;
          else
	  {
	    printf("intersectCurves:WARNING: The line segments are both length zero! \n");
            r10=.5;   // fix this
            r20=.5;   // fix this
	  }
	}
	if( intersects )
	{
	  seg(numberOfIntersections,0)=x1a[0]+a11*r10;  // x - intersect
	  seg(numberOfIntersections,1)=x1a[1]+a21*r10;  // y - intersect
	  seg(numberOfIntersections,2)=(i1+r10)*dr1;
	  seg(numberOfIntersections,3)=(j1+r20)*dr2;
          // check for duplicates
          for( int i=0; i<numberOfIntersections; i++ )
	  {
            if( fabs(seg(i,2)-seg(numberOfIntersections,2)) < REAL_EPSILON*10. &&
                fabs(seg(i,3)-seg(numberOfIntersections,3)) < REAL_EPSILON*10. )
            {
	      intersects=FALSE;
	      break;
	    }
	  }
          if( intersects )
  	    numberOfIntersections++;
          assert( numberOfIntersections<100 );
	}
      }
    }
  }
  
  numberOfIntersectionPoints=numberOfIntersections;
  for( collision=0; collision<numberOfCollisions; collision++ )
    destroyBoundingBox(*intersectionBoxList[collision]);  // 990905
  
  if( numberOfIntersections==0 )
  {
     if( debugi & 2 )  printf("intersectCurves: No intersection found! \n");
    return 1;
  }
  if( debugi & 2 )
    printf(" intersectCurves: number of intersections = %i. dr1=%8.2e, dr2=%8.2e \n",numberOfIntersections,dr1,dr2);
  
  x.redim(2,numberOfIntersectionPoints);
  r1.redim(numberOfIntersectionPoints);
  r2.redim(numberOfIntersectionPoints);

  realArray ra(1,1), rb(1,1), x1(1,2), x2(1,2), x1r1(1,2,1), x2r2(1,2,1), m(1,2,2), delta(1,2), work(2);
  IntegerArray ipvt(2);
  real rcond;
  int job=0;
  int numberOfRealIntersections = numberOfIntersectionPoints;
  int intersectionID = 0;
  for( int i=0; i<numberOfIntersectionPoints; i++ )
  {

    //
    // solve the 2x2 system:
    //    map1(r1) - map2(r2) = 0   <--->  F(r1,r2)=0
    //
    real eps=SQRT(REAL_EPSILON*10.);  // should be ok if converging quadratically
    int maximumNumberOfIterations=20; // 10; *wdh* 021119 allow more iterations
    ra(0,0)=seg(i,2);
    rb(0,0)=seg(i,3);

    bool first=true;
    int iteration;
    for( iteration=0; iteration<maximumNumberOfIterations; iteration++ )
    {
    
      curve1.map(ra,x1,x1r1);
      curve2.map(rb,x2,x2r2);
  
  
      m(0,R2,0)= x1r1(0,R2,0);
      m(0,R2,1)=-x2r2(0,R2,0);
  
      delta(0,R2)=-x1(0,R2)+x2(0,R2);
  
      // solve matrix*delta = f0
      GECO( m(0,0,0),2,2,ipvt(0),rcond,work(0) ); // factor
      if( rcond < REAL_EPSILON )
      {
	printf("intersectCurves:WARNING: The curves are apparently parallel at the intersection point \n");
	nparallel++;
        break;   // *wdh* 021005 : break here to avoid nan's
      }
      GESL( m(0,0,0),2,2,ipvt(0),delta(0,0),job);     // solve
      if( debugi & 2 )
      {
	printf("intersectCurves:newtonIntersection: it=%i, i=%i, rcond=%e, max(fabs(delta)) = %e eps=%e ra=%8.2e rb=%8.2e \n",
	       iteration,i,rcond,max(fabs(delta)),eps,ra(0,0),rb(0,0));
      }

      real maxDelta=max(fabs(delta(0,0)),fabs(delta(0,1)));
      
      if( maxDelta<.1 )
      {
	ra(0,0)+=delta(0,0);
	rb(0,0)+=delta(0,1);
      }
      else
      {
        // limit the correction *wdh* 021119
        if( maxDelta>.5 && first )
	{
	  // something is wrong here -- there could be a corner in the curve: look for a better guess
          // there was an intersection of line-segments : length dr1 and dr2
          // let's look for a better guess
          first=false;
	  
          int i1=int(seg(i,2)/dr1);   // closest pt less than
	  int i2=int(seg(i,3)/dr2);

          const int num=100;  // split the interval into this many sub-pieces
          realArray r1(num,1),x1(num,2), r2(num,1),x2(num,2);

          real r1a=(i1-1)*dr1;  // ** it was necessary to look on a larger interval ***
          real r1b=(i1+2)*dr1;
          real r2a=(i2-1)*dr2;
          real r2b=(i2+2)*dr2;

          real deltar1=(r1b-r1a)/(num-1);
	  real deltar2=(r2b-r2a)/(num-1);

 	  r1.seqAdd(r1a,deltar1);  // evaluate interval at num sub-points
 	  r2.seqAdd(r2a,deltar2);
	  
          curve1.map(r1,x1);
	  curve2.map(r2,x2);
	  real dist,distMin=REAL_MAX;
          int j1Min,j2Min;
	  for( int j1=0; j1<num; j1++ )
	  {
	    for( int j2=0; j2<num; j2++ )
	    {
	      dist=SQR(x1(j1,0)-x2(j2,0))+SQR(x1(j1,1)-x2(j2,1));
	      if( dist<distMin )
	      {
		j1Min=j1;
		j2Min=j2;
		distMin=dist;
	      }
	    }
	  }
	  ra(0,0)=r1a+j1Min*deltar1;
	  rb(0,0)=r2a+j2Min*deltar2;
	  if( debugi & 2 )
	    printf("****intersectCurves: brute force search for a closer intersection pt. New r1=%9.3e r2=%9.3e dist(x)=%8.2e\n"
		   "   r1=[%9.3e,%9.3e] r2=[%9.3e,%9.3e] i1,i2 = (%i,%i) j1Min,j2Min=(%i,%i), old guess (r1,r2)=(%9.3e,%9.3e) "
                   " dr1=%8.2e dr2=%8.2e\n",
		   ra(0,0),rb(0,0),distMin,r1a,r1b,r2a,r2b,i1,i2,j1Min,j2Min,seg(i,2),seg(i,3),dr1,dr2);
	}
	else
	{
	  ra(0,0)+=delta(0,0)*.1/maxDelta;
	  rb(0,0)+=delta(0,1)*.1/maxDelta;
	}
      }
      

      if( max(fabs(delta)) < eps )
	break;

    }
    if( iteration>=maximumNumberOfIterations )
    {
      printf("intersectCurves:newtonIntersection:ERROR no convergence, rcond=%e, max(fabs(delta))=%e x1=(%e,%e), x2=(%e,%e)\n",
	     rcond,max(fabs(delta)),x1(0,0),x1(0,1),x2(0,0),x2(0,1));
      numberOfRealIntersections--;
    }
    else 
      {
	// assign return values:
	
	x(0,intersectionID)=.5*(x1(0,0)+x2(0,0));
	x(1,i)=.5*(x1(0,1)+x2(0,1));
	
	r1(intersectionID)=ra(0,0);
	r2(intersectionID)=rb(0,0);
	intersectionID++;
      }

  }
  if( debug & 2)
    cout<<"number of real intersections "<<numberOfRealIntersections<<endl;
  numberOfIntersectionPoints = numberOfRealIntersections;
  x.resize(2,numberOfIntersectionPoints);
  r1.resize(numberOfIntersections);
  r2.resize(numberOfIntersections);

  if ( (nparallel == numberOfIntersectionPoints) && numberOfIntersectionPoints!=0 )
    return -1;
  else
    return 0;
}


void IntersectionMapping::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
//=====================================================================================
/// \brief  Evaluate the intersection curve.
//=====================================================================================
{
  if( curve==NULL )
  {
    cout << "IntersectionMapping::map: Error: The intersection curve has not been generated yet \n";
    exit(1);    
  }
  curve->map(r,x,xr,params);

}


int IntersectionMapping::
get( const GenericDataBase & dir, const aString & name)
//=================================================================================
/// \brief  get a mapping from the database.
//=================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  subDir.get( IntersectionMapping::className,"className" );
  
  // save the Mappings we use *** this could be wasteful is they are already saved ****
  // save the intersection curves.
  char buff[40];
  for( int i=0; i<5; i++ )
  {
    Mapping *mapPointer = 
      i==0 ? map1 : 
      i==1 ? map2 :
      i==2 ? curve :
      i==3 ? rCurve1 :
             rCurve2;
    mapPointer=NULL;
      
    bool mapExists;
    subDir.get( mapExists , sPrintF(buff,"mapping%iExists",i));
    if( mapExists )
    {
      aString mapClassName;
      subDir.get( mapClassName, sPrintF(buff,"mapping%iClassName",i));

      mapPointer = Mapping::makeMapping( mapClassName );
      if( mapPointer!=NULL )
      {
	mapPointer->get( subDir,sPrintF(buff,"mapping%i",i));
      }
      else
      {
	cout << "IntersectionMapping::get:ERROR unable to make the mapping with className = " 
	     << (const char *)mapClassName << endl;
	throw "error";
      }
    }
  }
  
  Mapping::get( subDir, "Mapping" );
  delete &subDir;
  return 0;
}

int IntersectionMapping::
put( GenericDataBase & dir, const aString & name) const
//=================================================================================
/// \brief  put the mapping to the database.
//=================================================================================
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 
  subDir.put( IntersectionMapping::className,"className" );
  
  // save the Mappings we use *** this could be wasteful is they are already saved ****
  // save the intersection curves.
  char buff[40];
  for( int i=0; i<5; i++ )
  {
    Mapping *mapPointer = 
      i==0 ? map1 : 
      i==1 ? map2 :
      i==2 ? curve :
      i==3 ? rCurve1 :
             rCurve2;

    subDir.put( mapPointer!=0 , sPrintF(buff,"mapping%iExists",i));
    if( mapPointer!=NULL )
    {
      subDir.put( mapPointer->getClassName(), sPrintF(buff,"mapping%iClassName",i));
      mapPointer->put( subDir,sPrintF(buff,"mapping%i",i));
    }
  }
  
  Mapping::put( subDir, "Mapping" );
  delete &subDir;
  return 0;
}

Mapping *IntersectionMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==IntersectionMapping::className )
    retval = new IntersectionMapping();
  return retval;
}

    

int IntersectionMapping::
update( MappingInformation & mapInfo ) 
//=====================================================================================
/// \brief  Interactively create and/or change the mapping.
/// \param mapInfo (input): Holds a graphics interface to use.
//=====================================================================================
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sPrintF
  aString menu[] = 
    {
      "!IntersectionMapping",
      "choose mappings to intersect",
      "intersect",
      "plot intersection",
      "reparameterize",
      "add parameter curves to list of mappings",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "show parameters",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "choose mappings to intersect",
      "intersect          : intersect the mappings",
      "plot intersection  : plot intersection curves (3 versions of)",
      "reparameterize     : redistribute points according to arc-length and curvature",
      "add parameter curves to list of mappings: add the parameter space intersection curves to the list of mappings",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  aString answer,line,answer2; 

  bool plotObject=map1!=NULL && map2!=NULL;
  GraphicsParameters params;
  params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  gi.appendToTheDefaultPrompt("IntersectionMapping>"); // set the default prompt

  // plottingbounds:
  RealArray xBound(2,3);

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="choose mappings to intersect" )
    { 
      // Make a menu with the Mapping names (only curves or surfaces!)
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int j=0;
      for( int i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( map.getDomainDimension()== (map.getRangeDimension()-1) && map.mapPointer!=this )
	{
	  subListNumbering(j)=i;
          menu2[j++]=map.getName(mappingName);
	}
      }
      menu2[j]="";   // null string terminates the menu
      if( j==0 )
      {
	gi.outputString("IntersectionMapping:ERROR:There are no curves or surfaces to intersect");
	gi.stopReadingCommandFile();
        delete [] menu2;
        continue;
      }
      
	
      aString choice[] = {"choose first mapping",
                         "choose second mapping"
                        }; 
      for( int side=0; side<=1; side++ )
      {
        aString prompt = choice[side];
        int mapNumber = gi.getMenuItem(menu2,answer2,prompt);
        mapNumber=subListNumbering(mapNumber);  // map number in the original list
        if( mapNumber<0 )
	{
	  cout << "unknown response=" << answer2 << endl;
	  gi.stopReadingCommandFile();
	  break;
	}
        if( side==0 )
          map1=mapInfo.mappingList[mapNumber].mapPointer;
        else
          map2=mapInfo.mappingList[mapNumber].mapPointer;
      }
      
      delete [] menu2;

      if( map1==map2 )
      { 
	gi.outputString("Error: both mappings cannot be the same! choose again");
	map1=map2=NULL;
	continue;
      }
      if( map1->getDomainDimension()!=2 || map1->getRangeDimension()!=3 ||
          map2->getDomainDimension()!=2 || map2->getRangeDimension()!=3 )
      {
	gi.outputString("Error: both mappings must be surfaces in 3D! choose again");
	map1=map2=NULL;
	continue;
      }

      // Define properties of this mapping

      setName(mappingName,map1->getName(mappingName)+".intersect."+map2->getName(mappingName));

      setDomainDimension(map1->getDomainDimension()-1);
      setRangeDimension(map1->getRangeDimension());
      for( int axis=0; axis<domainDimension-1; axis++ )
      {
        // setGridDimensions(axis,surface->getGridDimensions(axis));
        // setIsPeriodic(axis,surface->getIsPeriodic(axis));
        for( int side=Start; side<=End; side++ )
	{
          // setBoundaryCondition(side,axis,surface->getBoundaryCondition(side,axis));
          // setShare(side,axis,surface->getShare(side,axis));
	}
      }

      // determine bounds on the mappings for plotting
      Bound b;
      xBound=0.;
      for( int m=0; m<2; m++)
      {
	Mapping & map = m==0 ? *map1 : *map2;
	for( int axis=0; axis<map.getRangeDimension(); axis++ )
	{
	  b = map.getRangeBound(Start,axis);
	  if( b.isFinite() )
	    xBound(Start,axis)=min(xBound(Start,axis),(real)b);
	  b = map.getRangeBound(End,axis);
	  if( b.isFinite() )
	    xBound(End,axis)=max(xBound(End,axis),(real)b);
	}
      }

      params.set(GI_PLOT_BOUNDS,xBound);
      params.set(GI_USE_PLOT_BOUNDS,TRUE); 


      mappingHasChanged(); 
      plotObject=TRUE;
      
    }
    else if( answer=="intersect" )
    {
      
      int failure = determineIntersection(&gi,params);
      if( failure )
      {
	printf("IntersectionMapping::update:ERROR: unable to compute the intersection\n");
      }
      else
      {
        mappingHasChanged(); 
        plotObject=TRUE;
      }
    }
    else if( answer=="add parameter curves to list of mappings" )
    {
      if( rCurve1!=NULL && rCurve2!=NULL )
      {
        rCurve1->setName(mappingName,sPrintF(line,"%s-trimCurve",(const char*)map1->getName(mappingName)));
        rCurve2->setName(mappingName,sPrintF(line,"%s-trimCurve",(const char*)map2->getName(mappingName)));
	
	printF("Adding the parameter space intersection curves to the list of mappings,"
               " names are [%s] and [%s]\n",(const char*)rCurve1->getName(mappingName),(const char*)rCurve2->getName(mappingName));
        mapInfo.mappingList.addElement(*rCurve1);
	mapInfo.mappingList.addElement(*rCurve2);
      }
      else
      {
	printF("WARNING:The parameter space intersection curves have not been computed yet. No mappings added.\n");
      }
      
    }
    else if( answer=="reparameterize" )
    {
      real arcLengthWeight=1., curvatureWeight=.2;
      gi.inputString(answer,"Enter the arclength weight and curvature weight. (default=1.,.2)");
      if( answer!="" )
        sScanF(answer,"%e %e",&arcLengthWeight,&curvatureWeight);
      
      printf("using:   arcLengthWeight=%e, curvatureWeight=%e\n",arcLengthWeight,curvatureWeight);
      
      reparameterize(arcLengthWeight,curvatureWeight);
      mappingHasChanged(); 
      plotObject=TRUE;
    }
    else if( answer=="show parameters" )
    {
      display();
    }
    else if( answer=="plot" )
    {
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      gi.erase();
      PlotIt::plot(gi,*this,params); 
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
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
             answer=="periodicity" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
      if( answer=="lines" )
      { // note: the number of lines here is different from the number of lines used for the 
        // intersection curve.
        mappingHasChanged(); 
      }
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=TRUE;
    else if( answer=="plot intersection" )
    {
      gi.erase();
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      params.set(GI_MAPPING_COLOUR,"green");
      params.set(GI_TOP_LABEL,"actual intersection curve in space");
      PlotIt::plot(gi, *curve,params );
      params.set(GI_USE_PLOT_BOUNDS,FALSE); 
      if( rCurve1!=NULL )
      {
        gi.erase();
        params.set(GI_TOP_LABEL,"parametric curve rCurve1");
        PlotIt::plot(gi, *rCurve1,params );
      }
      if( rCurve2!=NULL )
      {
        gi.erase();
        params.set(GI_TOP_LABEL,"parametric curve rCurve2");
        PlotIt::plot(gi, *rCurve2,params );
      }
      params.set(GI_TOP_LABEL,"");
      params.set(GI_USE_PLOT_BOUNDS,TRUE); 
      params.set(GI_PLOT_BOUNDS,xBound);
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    }
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
    }

    if( plotObject )
    {

      gi.erase();
      if( map1!=NULL && map2!=NULL)
      {
        params.set(GI_SURFACE_OFFSET,(real)10.);  // offset the surfaces so we see intersection line
	params.set(GI_TOP_LABEL,"");
        params.set(GI_MAPPING_COLOUR,"red");
        PlotIt::plot(gi,*map1,params); 
        params.set(GI_MAPPING_COLOUR,"blue");
        PlotIt::plot(gi, *map2,params); 
        params.set(GI_SURFACE_OFFSET,(real)3.);
      }
      if( curve!=NULL )
      {
        // increase the line thickness
	real oldCurveLineWidth;
	params.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
	params.set(GraphicsParameters::curveLineWidth,3.);
	params.set(GI_TOP_LABEL,getName(mappingName));
        params.set(GI_MAPPING_COLOUR,"green");
	PlotIt::plot(gi,*this,params);   
	params.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
      }
    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
  
}

void IntersectionMapping::
reOrder(realArray & x, int & iStart, int & iEnd, periodicType & periodic, 
	periodicType curveIsPeriodic[2], real epsX )
{
//  printf("Inside reOrder()\n");
  
  int i, axis;
  Range xAxes(0,2), R(0,6);

  if( Mapping::debug & 2 )
  {
    printf("reOrder:BEFORE SHIFT: polygon for intersection: iStart=%i, iEnd=%i \n",iStart,iEnd);
    for( i=iStart; i<=iEnd; i++ )
      printf(" x(%i,.) = (%e,%e,%e) r1=(%5.3f,%5.3f), r2=(%5.3f,%5.3f)\n",i,
	     x(i,0),x(i,1),x(i,2),x(i,3),x(i,4),x(i,5),x(i,6));
  }

  curveIsPeriodic[0] = notPeriodic; curveIsPeriodic[1] = notPeriodic; // for two parameter curves

  periodicType mapIsPeriodic[2][2];  // holds periodicity for the two surfaces
  for( axis=axis1; axis<=axis2; axis++ )
  {
    mapIsPeriodic[0][axis]=map1->getIsPeriodic(axis);
    mapIsPeriodic[1][axis]=map2->getIsPeriodic(axis);
  }
  
  periodic =  max(fabs(x(iStart,xAxes)-x(iEnd,xAxes)))< epsX ? functionPeriodic : notPeriodic;  
  
  // check to see if the parameter curves cross any periodic boundaries.
  //
  // The parameter curves may cross periodic boundaries of map1 or map2.
  // If the curve only crosses the boundary once then we can shift the parameter curve
  // Otherwise the parameter curve will lie outside the unit square.
  //
  //                 -----------------
  //                 |               |
  //                 |               |
  //                 |               |
  //         periodic|-->---ES--->---|   S=start, E=end  ---> this curve can be shifted so S=0, E=1
  //        boundary |               |
  //                 |               |
  //                 |               |
  //                 -----------------
  //
  //                 -----------------
  //                 |               |
  //                 |->-         ->-|
  //                 |    \     /    |   --> curve cannot be shifted to lie in [0,1]
  //         periodic|    |     |    |   --> move left hand section of curve over by 1 
  //        boundary |___/       \___|       (or move right hand section by -1) to give a contiguous curve
  //                 |               |
  //                 |               |
  //                 -----------------
  //
  //    torus:
  //                 -----------------
  //                 |   /           |
  //                 |  /            |
  //                 | /             |
  //         periodic|/            / |   S=start, E=end  this curve crosses twice
  //        boundary |           S   |
  //                 |          E    |
  //                 |         /     |
  //                 -----------------
  //                    periodic boundary


  int numberOfCrossingsForCurve[2]= {0,0};

  real jump=.75;  // if a curve segment in parameter space is longer than this -> must be jumping from 0 <-> 1
  int iStartNew=iStart;  // iStartNew gets reset if we need to reorder the points
  
  // *wdh* changed [6] to [7] below 040808
  int numberOfJumps[7] ={ 0,0,0,0,0,0,0 };  // number of jumps for each curve
  int jumpStart[7] ={ 0,0,0,0,0,0,0 };      // position of the jump
  bool reordered=FALSE;
  bool reverseOrder=FALSE;

// AP: Loop over the two curves in parameter space
  for( int pCurve=0; pCurve<=1; pCurve++ ) // do each parameter curve
  {
    int m1= pCurve==0 ? 3 : 5;  // r1
    int m2= pCurve==0 ? 4 : 6;  // r2
    int shift1=0, shift2=0;

    for( i=iStart; i<=iEnd; i++ )
    {
      int im1= i!=iStart ? i-1 : iEnd-1;
	
      if( shift1!=0 )
	x(i,m1)+=shift1;
      if( shift2!=0 )
	x(i,m2)+=shift2;
	
      if( fabs(x(i,m1)-x(im1,m1)) > jump || fabs(x(i,m2)-x(im1,m2)) > jump ) // look to see where r jumps from 1 to 0
      {
        real diff1=x(i,m1)-x(im1,m1);
        real diff2=x(i,m2)-x(im1,m2);
        
        if( (fabs(diff1) > jump && (bool)mapIsPeriodic[pCurve][axis1])  || 
            (fabs(diff2) > jump && (bool)mapIsPeriodic[pCurve][axis1]) )
	{
          // The parameter value jumps across a periodic boundary
	  printf("parameter curve %i jumps across a periodic boundary at i=%i, diff1=%e, diff2=%e \n",pCurve+1,i,
		 fabs(diff1),fabs(diff2));
	  numberOfCrossingsForCurve[pCurve]++;

	
	  if( fabs(diff1) > jump && (bool)mapIsPeriodic[pCurve][axis1] )  // r1 jumps
	  {
            jumpStart[m1]=i;
            printf(" i=%i, im1=%i, x(im1)=%e, x(i)=%e, jumpStart = %i\n",i,im1,x(im1,m1),x(i,m1),jumpStart[m1]);

	    x(i,m1)-=shift1; 
	    shift1= diff1 < 0. ? shift1+1 : shift1-1;
	    printf("Shift for curve %i in direction axis1 set to %i \n",pCurve+1,shift1);
	    x(i,m1)+=shift1;
	    
            numberOfJumps[m1]++;
	  }
	  if( fabs(diff2) > jump && (bool)mapIsPeriodic[pCurve][axis2] )  // r2 jumps
	  {
            jumpStart[m2]=i;
	    numberOfJumps[m2]++;
	    x(i,m2)-=shift2;
	    shift2= diff2 < 0. ? shift2+1 : shift2-1;
	    printf("Shift for curve %i in direction axis2 set to %i \n",pCurve+1,shift2);
	    x(i,m2)+=shift2;
	  }
	}
	else if( (fabs(diff1) > jump || fabs(diff2) > jump ) && i!=iStart )
	{
          printf("Parameter value jumps on a non-periodic surface \n");
	  printf("Need to reorder the points \n");
          if( iStartNew!=iStart )
	  {
	    printf("This has happened twice! There is something wrong here \n");
	    throw "error";
	  }
          if( fabs(diff1) > jump )
	  {
            iStartNew= x(i,m1) < x(im1,m1) ? i : im1;
/* ---
            if( fabs(x(iStartNew,m1))>eps )
	    {
	      printf("ERROR: curve does not start at zero! x(iStartNew,m1)=%e \n",x(iStartNew,m1));
	      throw "error";
	    }
---- */
	  }
          else
	  {
            iStartNew= x(i,m2) < x(im1,m2) ? i : im1;
/* ----
            if( fabs(x(iStartNew,m2))>eps )
	    {
	      printf("ERROR: curve does not start at zero! x(iStartNew,m2)=%e \n",x(iStartNew,m2));
	      throw "error";
	    }
 ---- */
	  }
	}
      }
    } // end for i=iStart...iEnd
    
    
    if( !reordered ) // we can only reorder one of the curves.
    {
      if( numberOfJumps[m1]==1 )
      {
	iStartNew=jumpStart[m1];
        x(Range(iStartNew,iEnd),m1)-=shift1;  // remove the shift
        reordered=TRUE;
        if( x(iStartNew,m1)>x(iStartNew-1,m1) )
          reverseOrder=TRUE;
      }
      else if( numberOfJumps[m2]==1 )
      {
	iStartNew=jumpStart[m2];
        x(Range(iStartNew,iEnd),m2)-=shift2;  // remove the shift
        reordered=TRUE;
        if( x(iStartNew,m2)>x(iStartNew-1,m2) )
          reverseOrder=TRUE;
      }
    }

    if( iStart!=iStartNew ) // **** check this ****
    {
      // we may need to shift the values so the value of r1(iStart,0) \approx 0
      printf("Shifting points in the non periodic case, iStart=%i, iStartNew=%i \n",iStart,iStartNew);
      
      realArray y(Range(iStartNew,iEnd-iStart+iStartNew),R);

      if( periodic==functionPeriodic )
      {
	// curve is periodic: we need to remove one of the end points and add a new one at 0 or at 1
        if( fabs(x(iStartNew,m1)) < fabs(x(iStartNew-1,m1)-1.) )
	{ 
          // add a point at r=1.
	  y(Range(iStartNew,iEnd),R)=x(Range(iStartNew,iEnd),R);
	  y(Range(iEnd+1,iEnd-iStart+iStartNew),R)=x(Range(iStart,iStartNew-1),R); 
	  iEnd=iEnd-iStart+iStartNew;
	  iStart=iStartNew;
	}
	else
	{
          // add a point at r=0.
/* ------
          y(iStartNew,R)=x(iStartNew-1,R);
	  y(iStartNew,m1)-=1.;    // shift m1 or m2 ??
----- */	  
          
	  y(Range(iStartNew,iEnd),R)=x(Range(iStartNew,iEnd),R);   
	  y(Range(iEnd+1,iEnd-iStart+iStartNew),R)=x(Range(iStart,iStartNew-1),R); 
	  iEnd=iEnd-iStart+iStartNew;
	  iStart=iStartNew;
	}
      }
      else
      {
	y(Range(iStartNew,iEnd),R)=x(Range(iStartNew,iEnd),R);
	y(Range(iEnd+1,iEnd-iStart+iStartNew),R)=x(Range(iStart,iStartNew-1),R); 
	iEnd=iEnd-iStart+iStartNew;
	iStart=iStartNew;
      }
      
      if( reverseOrder )
      {
        printf("REVERSE the order of the points\n");
	
        x.redim(Range(iStart,iEnd),R);
        const int iEndPlusStart=iEnd+iStart;
        for( int i=iStart; i<=iEnd; i++ )
          x(i,R)=y(iEndPlusStart-i,R);
      }
      else
      {
        x.redim(0);
        x=y;
      }
    }
  } // end for pCurve=0,1
  

  if( Mapping::debug & 4 )
  {
    printf("reOrder:AFTER SHIFT: polygon for intersection: iStart=%i, iEnd=%i \n",iStart,iEnd);
    for( i=iStart; i<=iEnd; i++ )
      printf(" x(%i,.) = (%e,%e,%e) r1=(%5.3f,%5.3f), r2=(%5.3f,%5.3f)\n",i,
	     x(i,0),x(i,1),x(i,2),x(i,3),x(i,4),x(i,5),x(i,6));
  }
}

void IntersectionMapping::
reduce(realArray & x, int & iStart, int & iEnd)
{
//  printf("Inside reduce()\n");
  // reduce the number of points on the intersection curve as it will usually have
  // many times more points than the original curves.
  Range Ri(iStart,iEnd), R(0,6);

  if( iEnd-iStart > 25 )
  {
    Index I=Range(iStart+1,iEnd-1,2);  // try to remove every other point
    // if the curve is smooth at a point then the second undivided difference will be <= the average value
    // keep the point if mask==TRUE  (don't remove real corners in the curve)
    intArray mask(Ri); mask=1;
    mask(I) = ( fabs(x(I+1,0)-2.*x(I,0)+x(I-1,0)) >  fabs(x(I+1,0)+2.*x(I,0)+x(I-1,0)) ) ||
              ( fabs(x(I+1,1)-2.*x(I,1)+x(I-1,1)) >  fabs(x(I+1,1)+2.*x(I,1)+x(I-1,1)) ) ||
              ( fabs(x(I+1,2)-2.*x(I,2)+x(I-1,2)) >  fabs(x(I+1,2)+2.*x(I,2)+x(I-1,2)) );

    // mask.display("remove point mask");

    int j=iStart;
    for( int i=iStart; i<=iEnd; i++ )
    {
      if( mask(i) )
      {
	x(j,R)=x(i,R);  // keep this point
	j++;
      }
    }
    if( Mapping::debug & 2 ) 
      printf(">>>IntersectionMapping: reducing number of points on the curve from %i to %i \n",
           iEnd-iStart+1,j-iStart);
    iEnd=j-1;
  }
}
