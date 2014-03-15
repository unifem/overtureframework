#include "Ogen.h"

//  Define a triple for loop.  The macro is defined only within this file.
#define COMPOSITE_GRID_FOR_3(range,i,j,k)                              \
        for (k=((Integer*)(range))[4]; k<=((Integer*)(range))[5]; k++) \
        for (j=((Integer*)(range))[2]; j<=((Integer*)(range))[3]; j++) \
        for (i=((Integer*)(range))[0]; i<=((Integer*)(range))[1]; i++)

/* -----
bool Ogen::
canInterpolateNew(CompositeGrid & cg,
	       const Integer&      k10,
	       const Integer&      k20,
	       const RealArray&    r,
	       const LogicalArray& ok,
	       const LogicalArray& useBackupRules,
	       const Logical       checkForOneSided) 
// ======================================================================================
// /Description:
//  Determine whether points on grid k1 at r in the coordinates of grids k2
//  can be interpolated from grids k2.
//
// ======================================================================================
{
  const int numberOfDimensions=cg.numberOfDimensions();
  
  MappedGrid& g = cg[k20];
  Integer iv1[3], &i1=iv1[0], &i2=iv1[1], &i3=iv1[2], ks, kd, iab_[2*3];
  Logical isOneSided, oneSided[3][2], returnValue = LogicalTrue, invalid;
  IntegerArray iab2(1,2,3); RealArray rA(1,numberOfDimensions);

  const Integer base = r.getBase(0), bound = r.getBound(0),
    k1 = cg.componentGridNumber(k10), k2 = cg.componentGridNumber(k20),
    l  = cg.multigridLevelNumber(k10);

  Range Rx(0,numberOfDimensions-1);
  RealArray ov(Rx); ov = useBackupRules(0) ? cg.backupInterpolationOverlap(Rx,k1,k2,l) : 
                            cg.interpolationOverlap(Rx,k1,k2,l);

  Range R=r.dimension(0);
  
  int axis;
  for( axis=0; axis<numberOfDimensions; axis++ )
  {
    rr(R,axis)= r(R,kd)*(1./g.gridSpacing(kd)) + g.indexRange(0,axis);   
    real shift = g.isCellCentered(axis) ? .5 : 0.;
    equals(iab(R,0,axis),floor(evaluate(rr(R,axis)-(ov(axis)+shift))));
    equals(iab(R,1,axis),floor(evaluate(rr(R,axis)+(ov(axis)+shift))));
    
    if( !g.isPeriodic(axis) )
    {
       //  Check if point is too close to an interpolated side.
      if( g_boundaryCondition(0,axis)==0 )
	invalid(R) = iab(R,0,axis) < g_extendedIndexRange(0,axis);
      else
      {
	where( iab(R,0,axis) < g.extendedIndexRange(0,axis) )
	{
	  iab(R,0,axis) = g.extendedIndexRange(0,axis);
	  iab(R,1,axis) = iab(R,0,axis) + Integer(floor((Real).5 * iw0_(axis) + ov0_(axis) + (Real).5));
	}
      }
      if( g_boundaryCondition(1,axis)==0 )
	invalid(R) = iab(R,1,axis) > g_extendedIndexRange(1,axis);
      else
      {
	where( iab(R,1,axis) > g.extendedIndexRange(1,axis) )
	{
	  iab(R,1,axis) = g.extendedIndexRange(1,axis);
	  iab(R,0,axis) = iab(R,1,axis) - Integer(floor((Real).5 * iw0_(axis) + ov0_(axis) + (Real).5));
	}
      }
    }
  } // end for


//
//      Check that all points in the stencil are either discretization points
//      or interpolation points.  Backup discretization points and backup
//      interpolation points are also allowed.
//

  for( int m3=0; m3<width3; m3++ )
  {
    i3=iab(R,0,axis3)+m3;
    for( int m2=0; m2<width2; m2++ )
    {
      i2=iab(R,0,axis2)+m2;
      for( int m1=0; m1<width1; m1++ )
      {
	i1=iab(R,0,axis1)+m1;
	where( valid )
	{
	  valid =  !(mask(i1,i2,i3) & CompositeGrid::ISusedPoint);
	}
      }
    }
  }
  
  return returnValue;
}


----- */

bool Ogen::
canInterpolate(CompositeGrid & cg,
	       const Integer&      k10,
	       const Integer&      k20,
	       const RealArray&    r,
	       const LogicalArray& ok,
	       const LogicalArray& useBackupRules,
	       const Logical       checkForOneSided) 
{
//
//  Determine whether points on grid k1 at r in the coordinates of grids k2
//  can be interpolated from grids k2.
//
  const int numberOfDimensions=cg.numberOfDimensions();
  
    MappedGrid& g = cg[k20];
    Integer iv1[3], &i1=iv1[0], &i2=iv1[1], &i3=iv1[2], ks, kd, iab_[2*3];
    Logical isOneSided, oneSided[3][2], returnValue = LogicalTrue, invalid;
    IntegerArray iab2(1,2,3); RealArray rA(1,numberOfDimensions);
// *wdh* 980607    const Real a = -(Real)100. * epsilon, b = (Real)1. - a;
    const Real a = -(Real)2. * cg.epsilon(), b = (Real)1. - a;
    const Integer base = r.getBase(0), bound = r.getBound(0),
      k1 = cg.componentGridNumber(k10), k2 = cg.componentGridNumber(k20),
      l  = cg.multigridLevelNumber(k10);

#define iab(i,j) iab_[(i) + 2 * (j)]
#ifdef DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
#define g_boundaryCondition(i,j)  g.boundaryCondition  ((i),(j))
#define g_dimension(i,j)          g.dimension          ((i),(j))
#define g_discretizationWidth(i)  g.discretizationWidth((i))
#define g_gridSpacing(i)          g.gridSpacing        ((i))
#define g_indexRange(i,j)         g.indexRange         ((i),(j))
#define g_extendedIndexRange(i,j) g.extendedIndexRange ((i),(j))
#define g_isCellCentered(i)       g.isCellCentered     ((i))
#define g_isPeriodic(i)           g.isPeriodic         ((i))
#define g_mask(i,j,k)             g.mask()             ((i),(j),(k))
#define r_(i,j)                   r                    ((i),(j))
#define useBackupRules_(i)        useBackupRules       ((i))
#define ok_(i)                    ok                   ((i))
#define iw0_(i)                   iw0                  ((i),k1,k2,l)
#define ov0_(i)                   ov0                  ((i),k1,k2,l)
#else
#define g_boundaryCondition(i,j)  g_boundaryCondition_  [(i) + 2 * (j)]
#define g_dimension(i,j)          g_dimension_          [(i) + 2 * (j)]
#define g_discretizationWidth(i)  g_discretizationWidth_[(i)]
#define g_gridSpacing(i)          g_gridSpacing_        [(i)]
#define g_indexRange(i,j)         g_indexRange_         [(i) + 2 * (j)]
#define g_extendedIndexRange(i,j) g_extendedIndexRange_ [(i) + 2 * (j)]
#define g_isCellCentered(i)       g_isCellCentered_     [(i)]
#define g_isPeriodic(i)           g_isPeriodic_         [(i)]
#define g_mask(i,j,k)             g_mask_               [(i)+i10*(j)+j10*(k)]
#define r_(i,j)                   r__                   [(i) + r_s * (j)]
#define useBackupRules_(i)        useBackupRules__      [(i)]
#define ok_(i)                    ok__                  [(i)]
#define iw0_(i)                   iw0__                 [(i)]
#define ov0_(i)                   ov0__                 [(i)]
    Integer *g_boundaryCondition_   = g.boundaryCondition()  .getDataPointer(),
            *g_dimension_           = g.dimension()          .getDataPointer(),
            *g_discretizationWidth_ = g.discretizationWidth().getDataPointer(),
            *g_indexRange_          = g.indexRange()         .getDataPointer(),
            *g_extendedIndexRange_  = g.extendedIndexRange() .getDataPointer(),
            *g_isCellCentered_      = g.isCellCentered()     .getDataPointer(),
            *g_isPeriodic_          = g.isPeriodic()         .getDataPointer(),
            *g_mask_                = g.mask()               .getDataPointer(),
            *useBackupRules__       = useBackupRules         .getDataPointer(),
            *ok__                   = ok                     .getDataPointer(),
            *interpolationWidth__   = &cg.interpolationWidth(0,k1,k2,l),
        *backupInterpolationWidth__ = &cg.backupInterpolationWidth(0,k1,k2,l);
    Real    *g_gridSpacing_         = g.gridSpacing()        .getDataPointer(),
            *r__                    = r                      .getDataPointer(),
            *interpolationOverlap__ = &cg.interpolationOverlap(0,k1,k2,l),
      *backupInterpolationOverlap__ = &cg.backupInterpolationOverlap(0,k1,k2,l);
    const Integer i10 = g_dimension(1,0) - g_dimension(0,0) + 1,
           j10 = i10 * (g_dimension(1,1) - g_dimension(0,1) + 1),
      r_s = &r(base,1) - &r(base,0);
    g_mask_ = &g_mask(-g_dimension(0,0),-g_dimension(0,1),-g_dimension(0,2));
    r__               = &r_(-base,0);
    ok__              = &ok_(-base);
    useBackupRules__  = &useBackupRules_(-base);
#endif // DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
    const Integer *g_I1 = g.I1(), *g_I2 = g.I2(), *g_I3 = g.I3();
    
    // wdh: we can interpolate from extended index range
    real rBound[3][2];
    for (kd=0; kd<3; kd++) 
    {
      rBound[kd][0]=a+(g_extendedIndexRange(0,kd)-g_indexRange(0,kd))*g_gridSpacing(kd);   
      rBound[kd][1]=b+(g_extendedIndexRange(1,kd)-g_indexRange(1,kd))*g_gridSpacing(kd);
    }

    for (Integer i=base; i<=bound; i++) if (ok_(i)) {
//
//      Determine the stencil of points to check.
//
#ifdef DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
        IntegerArray& iw0 = useBackupRules_(i) ?
          backupInterpolationWidth : interpolationWidth;
        RealArray& ov0 = useBackupRules_(i) ?
          backupInterpolationOverlap : interpolationOverlap;
#else
        Integer* iw0__ = useBackupRules_(i) ?
          backupInterpolationWidth__ : interpolationWidth__;
        Real* ov0__ = useBackupRules_(i) ?
          backupInterpolationOverlap__ : interpolationOverlap__;
#endif // DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
        invalid = isOneSided = LogicalFalse;
        for (kd=0; kd<3; kd++) 
        {
            oneSided[kd][0] = oneSided[kd][1] = LogicalFalse;
            if (kd < numberOfDimensions) 
            {
	      // *wdh if (invalid = r_(i,kd) < a || r_(i,kd) > b) break;
	        if (invalid = r_(i,kd) < rBound[kd][0] || r_(i,kd) > rBound[kd][1]) break;
                Real rr = r_(i,kd) / g_gridSpacing(kd) + g_indexRange(0,kd);
                iab(0,kd) = Integer(floor(rr - ov0_(kd) -
                  (g_isCellCentered(kd) ? (Real).5 : (Real)0.)));
                iab(1,kd) = Integer(floor(rr + ov0_(kd) +
                  (g_isCellCentered(kd) ? (Real).5 : (Real)1.)));
                if (!g_isPeriodic(kd)) {
                    if (iab(0,kd) < g_extendedIndexRange(0,kd)) {
//                      Check if point is too close to an interpolated side.
                        if (invalid = !g_boundaryCondition(0,kd)) break;
//                      One-sided interpolation is used close to a boundary.
                        isOneSided = oneSided[kd][0] = LogicalTrue;
                        iab(0,kd) = g_extendedIndexRange(0,kd);
                        iab(1,kd) = iab(0,kd) +
                          Integer(floor((Real).5 * iw0_(kd) + ov0_(kd) + (Real).5));
                    } // end if
                    if (iab(1,kd) > g_extendedIndexRange(1,kd)) {
//                      Check if point is too close to an interpolated side.
                        if (invalid = !g_boundaryCondition(1,kd)) break;
//                      One-sided interpolation is used close to a boundary.
                        isOneSided = oneSided[kd][1] = LogicalTrue;
                        iab(1,kd) = g_extendedIndexRange(1,kd);
                        iab(0,kd) = iab(1,kd) -
                          Integer(floor((Real).5 * iw0_(kd) + ov0_(kd) + (Real).5));
                    } // end if
                } // end if
            } else {
                iab(0,kd) = g_extendedIndexRange(0,kd);
                iab(1,kd) = g_extendedIndexRange(1,kd);
            } // end if
        } // end for
//
//      Check that all points in the stencil are either discretization points
//      or interpolation points.  Backup discretization points and backup
//      interpolation points are also allowed.
//
        if (!invalid) COMPOSITE_GRID_FOR_3(iab_, i1, i2, i3)
          if (invalid = invalid ||
            !(g_mask(g_I1[i1],g_I2[i2],g_I3[i3]) & CompositeGrid::ISusedPoint)) break;

        if (!invalid && checkForOneSided && isOneSided) {
//
//          Check for one-sided interpolation from BC points
//          that interpolate from the interior of another grid.
//
//          Find the interpolation stencil.
            for (kd=0; kd<numberOfDimensions; kd++) rA(0,kd) = r_(i,kd);
            cg.getInterpolationStencil(k10, k20, rA, iab2, useBackupRules);

#ifdef DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
#define     iab2_(i,j,k) iab2((i),(j),(k))
#else
#define     iab2_(i,j,k) iab2__[(j) + 2 * (k)]
#endif // DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
            Integer* iab2__ = iab2.getDataPointer();
            for (kd=0; kd<3; kd++) {
                for (ks=0; ks<2; ks++) if (oneSided[kd][ks]) {
                    Integer iab21=iab2_(0,0,kd), iab22=iab2_(0,1,kd);
//                  Restrict the interpolation stencil to points that could be
//                  boundary discretization points of side (kd,ks) of the grid.
                    if (ks == 0) {
                      iab2_(0,0,kd) = g_extendedIndexRange(0,kd);
                      iab2_(0,1,kd) = min0(
                        iab2_(0,1,kd),
                        iab2_(0,0,kd) +
                        (g_discretizationWidth(kd) - 1) / 2 - 1);
                    } else {
                      iab2_(0,1,kd) = g_extendedIndexRange(1,kd);
                      iab2_(0,0,kd) = max0(iab2_(0,0,kd), iab2_(0,1,kd) -
                         (g_discretizationWidth(kd) - 1) / 2 + 1);
                    } // end if
//
//                  Check that all points in the stencil are either
//                  discretization points or interpolation points that are not
//                  interpolated one-sided from another grid.  Backup
//                  discretization points and backup interpolation points that
//                  are not interpolated one-sided from another grid are also
//                  allowed.
//
/* ------ *wdh* 980702
                    COMPOSITE_GRID_FOR_3(iab2__, i1, i2, i3)
                      if (invalid = invalid ||
                        g_mask(g_I1[i1],g_I2[i2],g_I3[i3]) &
                        ISinteriorBoundaryPoint) break;
                    if (invalid) break;
------- */
                    COMPOSITE_GRID_FOR_3(iab2__, i1, i2, i3)
		    {
                      if (invalid = invalid || g_mask(g_I1[i1],g_I2[i2],g_I3[i3]) & CompositeGrid::ISinteriorBoundaryPoint)
		      {
                        // Make sure that we are not too close to an the interpolation point
			real rDist=0.;
                        real cellCenterederedOffset=g_isCellCentered(kd) ? .5 : 0.;
			for( int dir=0; dir<numberOfDimensions; dir++ )
			  rDist=max(rDist,fabs( r_(i,dir)/g_gridSpacing(dir)
						-(iv1[dir]+cellCenterederedOffset-g_indexRange(Start,dir))));
			if( rDist > ov0_(0) )  // use ov_(0) as the minimum overlap. Normally=.5
			{
			  // printf("CompositeGrid::canInterpolate: near an interior boundary point but rDist=%e"
                          //       ", ov=%6.2e, so this point is ok! \n",rDist,ov0_(0));
                          invalid=FALSE;  // this point is ok after all
			}
                        else
			  break;
		      }
                      if (invalid) break;
		    }
		    
//                  Restore the interpolation stencil;
                    iab2_(0,0,kd) = iab21;
                    iab2_(0,1,kd) = iab22;
                } // end if, end for
                if (invalid) break;
            } // end for
        } // end if

        if (invalid) ok_(i) = returnValue = LogicalFalse;

    } else {
        returnValue = LogicalFalse;
    } // end if, end for
    return returnValue;
#undef iab
#undef g_boundaryCondition
#undef g_dimension
#undef g_discretizationWidth
#undef g_gridSpacing
#undef g_indexRange
#undef g_extendedIndexRange
#undef g_isCellCentered
#undef g_isPeriodic
#undef g_mask
#undef r_
#undef useBackupRules_
#undef ok_
#undef iw0_
#undef ov0_
#undef iab2_
}
#undef COMPOSITE_GRID_FOR_3

