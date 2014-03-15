//
// Who to blame:  Geoff Chesshire
//

#include "CompositeGrid.h"

Integer CompositeGridData::computeGeometry(
  const Integer& what,
  const Integer& how) {
    Integer returnValue = 0;
#ifndef _SERIAL_APP_ARRAY_H

//     if (what & THEinverseMap) {
// //
// //      Compute boundary-adjustment data.
// //
// //      The minimum allowed cosine of the angle between
// //      normal vectors of shared boundary surfaces:
//         const Real degrees = atan(1.) / 45.,
//           minimumNormalCosine = cos(18. * degrees);
// //
//         for (Integer k1=0; k1<numberOfGrids; k1++)
//           for (Integer k2=0; k2<numberOfBaseGrids; k2++) {
//             MappedGrid &g1 = grid[k1], &g2 = grid[k2];
//             assert(baseGridNumber(k2) == k2);
//             BoundaryAdjustmentArray& bA12 = boundaryAdjustment(k1,k2);
// //
// //          Check whether any boundary adjustments need to be computed.
// //
//             Logical needAdjustment1 = LogicalFalse;
//             if (bA12.getNumberOfElements())
//              for (Integer kd1=0;
//                   kd1<numberOfDimensions && !needAdjustment1; kd1++)
//               for (Integer ks1=0; ks1<2  && !needAdjustment1; ks1++) {
//                 BoundaryAdjustment& bA = bA12(ks1,kd1);
//                 if (how & COMPUTEgeometry ||
//                   !(   computedGeometry & THEinverseMap) ||
//                   !(bA.computedGeometry & THEinverseMap))
//                   for (Integer kd2=0;
//                        kd2<numberOfDimensions && !needAdjustment1; kd2++)
//                     for (Integer ks2=0; ks2<2 && !needAdjustment1; ks2++)
//                       needAdjustment1 =
//                         g1.boundaryCondition(ks1,kd1) > 0 &&
//                         g2.boundaryCondition(ks2,kd2) > 0 &&
//                         g1.sharedBoundaryFlag(ks1,kd1) &&
//                         g2.sharedBoundaryFlag(ks2,kd2) ==
//                         g1.sharedBoundaryFlag(ks1,kd1);
//             } // end for, end for, end if

//             if (needAdjustment1) {
// //
// //              Mark the boundary adjustments on all sides as not computed.
// //
//                 Integer kd1, kd2, ks1, ks2;
//                 for (kd1=0; kd1<numberOfDimensions; kd1++)
//                   for (ks1=0; ks1<2; ks1++) {
//                     BoundaryAdjustment& bA = bA12(ks1,kd1);
//                     bA.computedGeometry &= ~(THEmask | THEinverseMap);
//                     if( !bA.boundaryAdjustment.isNull()) // *wdh*
//                       bA.boundaryAdjustment = (Real)0.;
//                 } // end for, end for
// //
// //              Compute the boundary adjustments.
// //
//                 for (Integer it=0; it<numberOfDimensions; it++)
//                  for (kd1=0; kd1<numberOfDimensions; kd1++)
//                   for (ks1=0; ks1<2; ks1++) {
//                     BoundaryAdjustment& bA = bA12(ks1,kd1);
// //
// //                  Do nothing more if the first iteration had no effect.
// //
//                     if (it && (bA.computedGeometry & (THEinverseMap | THEmask))
//                       != (THEinverseMap | THEmask)) break;
// //
// //                  Check whether any boundaries of g2 share with (ks1,kd1).
// //
//                     Logical needAdjustment2 = LogicalFalse;
//                     if (how & COMPUTEgeometry ||
//                       !(   computedGeometry & THEinverseMap) ||
//                       !(bA.computedGeometry & THEinverseMap))
//                      for (kd2=0;kd2<numberOfDimensions&&!needAdjustment2;kd2++)
//                       for (ks2=0; ks2<2 && !needAdjustment2; ks2++)
//                         needAdjustment2 =
//                           g1.boundaryCondition(ks1,kd1) > 0 &&
//                           g2.boundaryCondition(ks2,kd2) > 0 &&
//                           g1.sharedBoundaryFlag(ks1,kd1) &&
//                           g2.sharedBoundaryFlag(ks2,kd2) ==
//                           g1.sharedBoundaryFlag(ks1,kd1);
//                     if (needAdjustment2) {
// //
// //                      Some sides of g2 share with side (ks1,kd1) of g1.
// //
// //                      Compute geometry at the boundary centers.
// //
//                         Index i[3], &i1 = i[0], &i2 = i[1], &i3 = i[2];
//                         Range d[3], &d1 = d[0], &d2 = d[1], &d3 = d[2],
//                               d0 = numberOfDimensions;
//                         RealArray ba, ob, ag;
//                         ba.reference(bA.boundaryAdjustment
//                           .getLocalArrayWithGhostBoundaries());
//                         ob.reference(bA.oppositeBoundary
//                           .getLocalArrayWithGhostBoundaries());
//                         ag.reference(bA.acrossGrid
//                           .getLocalArrayWithGhostBoundaries());
//                         IntegerArray dimensionL(2,3);
//                         Integer numberOfPoints = 1;
//                         for (kd2=0; kd2<3; kd2++) {
//                             dimensionL(0,kd2) = ob.getBase(kd2);
//                             dimensionL(1,kd2) = ob.getBound(kd2);
//                             i[kd2] = d[kd2] =
//                               Range(dimensionL(0,kd2), dimensionL(1,kd2));
//                             numberOfPoints *=
//                               dimensionL(1,kd2) - dimensionL(0,kd2) + 1;
//                         } // end for
//                         if (numberOfPoints) {
//                             Range p = numberOfPoints; RealArray x1(p,d0);
//                             ba.reshape(p,d0);ob.reshape(p,d0);ag.reshape(p,d0);
//                             for (ks2=0; ks2<2; ks2++) {
//                                 IntegerArray ip(d1,d2,d3,d0);
//                                 RealArray r(d1,d2,d3,d0);
//                                 for (kd2=0; kd2<numberOfDimensions; kd2++)
//                                   if (kd2 == kd1) {
//                                     ip(i1,i2,i3,kd2) =
//                                       dimensionL((ks1+ks2)%2,kd2);
//                                     r(i1,i2,i3,kd2) = Real((ks1 + ks2) % 2);
//                                 } else {
//                                     for (Integer k=dimensionL(0,kd2);
//                                       k<=dimensionL(1,kd2); k++) {
//                                         i[kd2] = k;
//                                         ip(i1,i2,i3,kd2) = k;
//                                         r(i1,i2,i3,kd2) =
//                                           g1.gridSpacing(kd2) *
//                                           ( k - g1.gridIndexRange(0,kd2)
//                                           + (g1.isCellCentered(kd2) ?
//                                           (Real).5 : (Real)0.) );
//                                     } // end for
//                                     i[kd2] = d[kd2];
//                                 } // end if, end for
//                                 r.reshape(p,d0); ip.reshape(p,d0);
//                                 if (ks2 == 0) {
//                                     RealArray xr1(p,d0,d0), rx2(p,d0,d0),
//                                       r2(p,d0), x2(p,d0), x3(p,d0);
// //
// //                                  Compute the boundary points of grid g1.
// //
//                                     g1.mapping().map(r, x1, xr1);
// //
// //                                  Apply the boundary adjustments already done.
// //
//                                     adjustBoundary(k1, k2, ip, x2 = x1);
// //
// //                                  Compute the boundary parameters on grid g2.
// //
//                                     g2.mapping().inverseMap(x2, r, rx2);
//                                     LogicalArray ok = r(p,0) != (Real)10.;
// //
// //                                  Check where the inverse is valid.
// //
//                                     for (Integer kd4=0;
//                                       kd4<g1.numberOfDimensions(); kd4++) {
//                                         if (g2.sharedBoundaryFlag(0,kd4) !=
//                                             g1.sharedBoundaryFlag(ks1,kd1))
//                                           where (r(p,kd4) < Real(-.4))
//                                             ok = LogicalFalse;
//                                         if (g2.sharedBoundaryFlag(1,kd4) !=
//                                             g1.sharedBoundaryFlag(ks1,kd1))
//                                           where (r(p,kd4) > Real(1.4))
//                                             ok = LogicalFalse;
//                                     } // end for
// //
// //                                  Project the results onto the boundary of g2.
// //
//                                     g1.adjustBoundary(g2, ks1, kd1, r2 = r, ok);
// //
// //                                  Compute the projected boundary points.
// //
//                                     g2.mapping().map(r2, x3);
// #ifdef BOGUS
// //
// //                                  Ignore points where the shift of r is zero
// //                                  or the minimum angle between normal vectors
// //                                  is larger than a tolerance.  We check all
// //                                  normal vectors of grid g2 because we don't
// //                                  know which one is the boundary surface
// //                                  normal.  The boundary surface normal of g2
// //                                  should have the smallest angle with respect
// //                                  to the boundary surface normal (bn1) of g1.
// //                                  The (un-normalized) normals to g2 are the
// //                                  transpose of the inverse mapping derivative
// //                                  (rx2) of the mapping on g2.
// //
//                                     switch (numberOfDimensions) {
//                                       case 1:
// //                                      Check the total shift of r.
//                                         where (r(p,0) == r2(p,0))
//                                           ok = LogicalFalse;
//                                       break;
//                                       case 2: {
// //                                      Compute the boundary unit normal vector.
//                                         RealArray bn1(p,d0), bnn(p);
//                                         Integer i;
//                                         for (i=0; i<numberOfDimensions; i++)
//                                           bn1(p,i) = (1-2*i) * (1-2*kd1) *
//                                             xr1(p,1-i,1-kd1);
//                                         bnn = bn1(p,0) * bn1(p,0)
//                                             + bn1(p,1) * bn1(p,1);
//                                         for (i=0; i<numberOfDimensions; i++)
//                                           where (bnn != (Real)0.)
//                                             bn1(p,i) /= sqrt(bnn);
// //                                      Check the total shift of r and
// //                                      the minimum angle between normals..
//                                         where ((r(p,0) == r2(p,0) &&
//                                                 r(p,1) == r2(p,1)) || (
//                                           abs( bn1(p,0) * rx2(p,0,0)
//                                              + bn1(p,1) * rx2(p,0,1) ) <
//                                           minimumNormalCosine *
//                                           sqrt( rx2(p,0,0) * rx2(p,0,0)
//                                               + rx2(p,0,1) * rx2(p,0,1) ) &&
//                                           abs( bn1(p,0) * rx2(p,1,0)
//                                              + bn1(p,1) * rx2(p,1,1) ) <
//                                           minimumNormalCosine *
//                                           sqrt( rx2(p,1,0) * rx2(p,1,0)
//                                               + rx2(p,1,1) * rx2(p,1,1) ) ))
//                                           ok = LogicalFalse;
//                                       } break;
//                                       case 3: {
// //                                      Compute the boundary unit normal vector.
//                                         RealArray bn1(p,d0), bnn(p);
//                                         Integer i;
//                                         for (i=0; i<numberOfDimensions; i++) {
//                                             Integer j=(i+1)%3, k=(j+1)%3,
//                                                   m=(kd1+1)%3, n=(m+1)%3;
//                                             bn1(p,i) = xr1(p,m,j) * xr1(p,n,k)
//                                                      - xr1(p,m,k) * xr1(p,n,j);
//                                         } // end for
//                                         bnn = bn1(p,0) * bn1(p,0)
//                                             + bn1(p,1) * bn1(p,1)
//                                             + bn1(p,2) * bn1(p,2);
//                                         for (i=0; i<numberOfDimensions; i++)
//                                           where (bnn != (Real)0.)
//                                             bn1(p,i) /= sqrt(bnn);
// //                                      Check the total shift of r and
// //                                      the minimum angle between normals..
//                                         where ((r(p,0) == r2(p,0) &&
//                                                 r(p,1) == r2(p,1) &&
//                                                 r(p,2) == r2(p,2)) || (
//                                           abs( bn1(p,0) * rx2(p,0,0)
//                                              + bn1(p,1) * rx2(p,0,1)
//                                              + bn1(p,2) * rx2(p,0,2) ) <
//                                           minimumNormalCosine *
//                                           sqrt( rx2(p,0,0) * rx2(p,0,0)
//                                               + rx2(p,0,1) * rx2(p,0,1)
//                                               + rx2(p,0,2) * rx2(p,0,2) ) &&
//                                           abs( bn1(p,0) * rx2(p,1,0)
//                                              + bn1(p,1) * rx2(p,1,1)
//                                              + bn1(p,2) * rx2(p,1,2) ) <
//                                           minimumNormalCosine *
//                                           sqrt( rx2(p,1,0) * rx2(p,1,0)
//                                               + rx2(p,1,1) * rx2(p,1,1)
//                                               + rx2(p,1,2) * rx2(p,1,2) ) &&
//                                           abs( bn1(p,0) * rx2(p,2,0)
//                                              + bn1(p,1) * rx2(p,2,1)
//                                              + bn1(p,2) * rx2(p,2,2) ) <
//                                           minimumNormalCosine *
//                                           sqrt( rx2(p,2,0) * rx2(p,2,0)
//                                               + rx2(p,2,1) * rx2(p,2,1)
//                                               + rx2(p,2,2) * rx2(p,2,2) ) ))
//                                           ok = LogicalFalse;
//                                       } break;
//                                     } // end switch
// #else
// //
// //                                  Ignore points where the shift of r is zero.
// //
//                                     switch (numberOfDimensions) {
//                                       case 1:
//                                         where (r(p,0) == r2(p,0))
//                                           ok = LogicalFalse;
//                                       break;
//                                       case 2: {
//                                         where (r(p,0) == r2(p,0) &&
//                                                r(p,1) == r2(p,1))
//                                           ok = LogicalFalse;
//                                       } break;
//                                       case 3: {
//                                         where (r(p,0) == r2(p,0) &&
//                                                r(p,1) == r2(p,1) &&
//                                                r(p,2) == r2(p,2))
//                                           ok = LogicalFalse;
//                                       } break;
//                                     } // end switch
// #endif // BOGUS
// //
// //                                  Compute the boundary adjustment.
// //
//                                     if (sum(ok)) {
//                                         where (ok)
//                                           for (Integer kd4=0;
//                                             kd4<numberOfDimensions; kd4++)
//                                               ba(p,kd4) += x3(p,kd4) - x2(p,kd4);
//                                         bA.computedGeometry |= THEmask;
//                                     } else {
//                                         break; // The adjustment is zero.
//                                     } // end if
//                                 } else {
// //
// //                                  Compute the opposite boundary points
// //                                  and the accross-grid vectors.
// //
//                                     g1.mapping().map(r, ob); ag = ob - x1;
// //
// //                                  Normalize ag to one over its length.
// //
//                                     RealArray agg(p);
//                                     switch (numberOfDimensions) {
//                                       case 1:
//                                         agg = ag(p,0) * ag(p,0);
//                                       break;
//                                       case 2:
//                                         agg = ag(p,0) * ag(p,0)
//                                             + ag(p,1) * ag(p,1);
//                                       break;
//                                       case 3:
//                                         agg = ag(p,0) * ag(p,0)
//                                             + ag(p,1) * ag(p,1)
//                                             + ag(p,2) * ag(p,2);
//                                       break;
//                                     } // end switch
//                                     for (Integer kd4=0;
//                                       kd4<g1.numberOfDimensions(); kd4++)
//                                         where (agg != (Real)0.)
//                                           ag(p,kd4) /= agg;
//                                 } // end if
//                             } // end for
//                         } // end if
//                         bA.computedGeometry |= THEinverseMap;
//                     } // end if
//                 } // end for, end for
//             } // end if
//         } // end for, end for
//         computedGeometry |= THEinverseMap;
//     } // end if

//     if (what & (THEinterpolationCoordinates |
//                 THEinterpoleeLocation       |
//                 THEinterpolationCondition   )) {
// //
// //      Compute the interpolation coordinates and/or interpolation condition.
// //      For this, we assume that interpolationPoint and interpoleeGrid already
// //      contain good data.  It may help if interpolationCoordinates already
// //      contains approximate data, good enough to use as an initial guess for
// //      the inverse mapping.
// //
//         computedGeometry |= what & (THEinterpolationCoordinates |
//           THEinterpoleeLocation  |  THEinterpolationCondition   );
//         for (Integer k1=0; k1<numberOfGrids; k1++)
//           if (numberOfInterpolationPoints(k1) &&
//             computedGeometry & THEinterpoleeGrid &&
//             computedGeometry & THEinterpolationPoint) {
//             MappedGrid &g1 = grid[k1];
//             Range d0 = numberOfDimensions, one = 1, two = 2, three = 3,
//               p = numberOfInterpolationPoints(k1);
//             IntegerArray i1(p,three), &iG = interpoleeGrid[k1];
//             LogicalArray ok(p);
//             RealArray x1(p,d0), r2(p,d0), ic2;
//             i1(p,d0) = interpolationPoint[k1];
//             for (Integer kd=numberOfDimensions; kd<3; kd++)
//               i1(p,kd) = g1.dimension(0,kd);

// //          Get an initial guess for the inverse mapping.
//             if (what & THEinterpolationCoordinates)
//                  r2 = interpolationCoordinates[k1];
//             else r2 = (Real).5;

// //             if (what & THEinterpolationCondition) {
// //                 ic2.reference(interpolationCondition[k1]);
// //                 RealArray xr1(p,d0,d0), rx2(p,d0,d0);
// //                 if ((g1.computedGeometry() & THEcenter &&
// //                      g1.computedGeometry() & THEcenterDerivative) ||
// //                     (g1.isAllVertexCentered() &&
// //                      g1.computedGeometry() & THEvertex &&
// //                      g1.computedGeometry() & THEvertexDerivative)) {
// // //                  Center or vertex is assumed to contain good data.
// //                     RealArray x; RealMappedGridFunction xr;
// //                     if (g1.computedGeometry() & THEcenter &&
// //                         g1.computedGeometry() & THEcenterDerivative) {
// //                         x .reference(g1.center());
// //                         xr.reference(g1.centerDerivative());
// //                     } else {
// //                         x .reference(g1.vertex());
// //                         xr.reference(g1.vertexDerivative());
// //                     } // end if
// //                     for (Integer i=0; i<numberOfInterpolationPoints(k1); i++)
// //                       for (Integer kd1=0; kd1<numberOfDimensions; kd1++) {
// //                         x1(i,kd1) = x(i1(i,0),i1(i,1),i1(i,2),kd1);
// //                         for (Integer kd2=0; kd2<numberOfDimensions; kd2++)
// //                           xr1(i,kd1,kd2) = xr(i1(i,0),i1(i,1),i1(i,2),kd1,kd2);
// //                     } // end for
// //                 } else {
// // //                  We must compute the centers and center derivatives.
// //                     RealArray r1(p,d0);
// //                     for (Integer kd=0; kd<numberOfDimensions; kd++) {
// //                         for (Integer i=0;
// //                           i<numberOfInterpolationPoints(k1); i++)
// //                             r1(i,kd) = g1.gridSpacing(kd) *
// //                               (i1(i,kd) - g1.gridIndexRange(0,kd));
// //                         if (g1.isCellCentered(kd))
// //                           r1(p,kd) += g1.gridSpacing(kd) * (Real).5;
// //                     } // end for
// //                     g1.mapping().map(r1, x1, xr1);
// //                 } // end if
// // //              Invert the mapping for each interpolation point.
// //                 for (Integer k2=0; k2<numberOfGrids; k2++)
// //                   if (k2 != k1) {
// //                     if (numberOfGrids == 2) {
// //                         adjustBoundary(k1, baseGridNumber(k2), i1, x1);
// //                         grid[k2].mapping().inverseMap(x1, r2, rx2);
// //                         ok = r2(p,0) != (Real)10.;
// // #ifdef ADJUST_FOR_PERIODICITY
// //                         grid[k2].adjustForPeriodicity(r2, ok);
// // #endif // ADJUST_FOR_PERIODICITY
// //                         grid[k1].getInverseCondition(grid[k2],
// //                           xr1, rx2, ic2, ok);
// //                     } else {
// // //                      Gather/scatter points interpolated from grid k2.
// //                         IntegerArray i3(p,d0); Integer i, j;
// //                         RealArray r3(p,d0), x3(p,d0), xr3(p,d0,d0), ic3(p);
// //                         for (i=j=0; i<numberOfInterpolationPoints(k1);
// //                           i++) if (k2 == iG(i)) {
// //                             for (Integer kd1=0; kd1<numberOfDimensions; kd1++) {
// //                                 i3(j,kd1) = i1(i,kd1);
// //                                 r3(j,kd1) = r2(i,kd1);
// //                                 x3(j,kd1) = x1(i,kd1);
// //                                 for (Integer kd2=0; kd2<numberOfDimensions;
// //                                   kd2++) xr3(j,kd1,kd2) = xr1(i,kd1,kd2);
// //                             } // end for
// //                             j++;
// //                         } // end if, end for
// //                         if (j) {
// //                             Range q = j;
// //                             adjustBoundary(
// //                               k1, baseGridNumber(k2), i3(q,d0), x3(q,d0));
// //                             grid[k2].mapping().inverseMapC(x3(q,d0), r3(q,d0), rx2(q,d0,d0));
// //                             ok(q) = r3(q,0) != (Real)10.;
// // #ifdef ADJUST_FOR_PERIODICITY
// //                             grid[k2].adjustForPeriodicity(r3(q,d0), ok(q));
// // #endif // ADJUST_FOR_PERIODICITY
// //                             grid[k1].getInverseCondition(grid[k2],
// //                               xr3(q,d0,d0), rx2(q,d0,d0), ic3(q), ok(q));
// //                             for (i=j=0; i<numberOfInterpolationPoints(k1); i++)
// //                               if (k2 == iG(i)) {
// //                                 for (Integer kd=0; kd<numberOfDimensions; kd++)
// //                                   r2(i,kd) = r3(j,kd);
// //                                 ic2(i) = ic3(j++);
// //                             } // end if, end for
// //                         } // end if
// //                     } // end if
// //                 } // end if, end for
// //
// //            } 
// //            else if (what & THEinterpolationCoordinates) {

// //             if (what & THEinterpolationCoordinates) {
// //                 if (g1.computedGeometry() & THEcenter ||
// //                    (g1.isAllVertexCentered() &&
// //                     g1.computedGeometry() & THEvertex)) {
// // //                  Center or vertex is assumed to contain good data.
// //                     RealArray x; RealMappedGridFunction xr;
// //                     if (g1.computedGeometry() & THEcenter)
// //                          x.reference(g1.center());
// //                     else x.reference(g1.vertex());
// //                     for (Integer i=0; i<numberOfInterpolationPoints(k1); i++)
// //                       for (Integer kd=0; kd<numberOfDimensions; kd++)
// //                         x1(i,kd) = x(i1(i,0),i1(i,1),i1(i,2),kd);
// //                 } else {
// // //                  We must compute the centers.
// //                     RealArray r1(p,d0);
// //                     for (Integer kd=0; kd<numberOfDimensions; kd++) {
// //                         for (Integer i=0;
// //                           i<numberOfInterpolationPoints(k1); i++)
// //                             r1(i,kd) = g1.gridSpacing(kd) *
// //                               (i1(i,kd) - g1.gridIndexRange(0,kd));
// //                         if (g1.isCellCentered(kd))
// //                           r1(p,kd) += g1.gridSpacing(kd) * (Real).5;
// //                     } // end for
// //                     g1.mapping().map(r1, x1);
// //                 } // end if
// // //              Invert the mapping for each interpolation point.
// //                 for (Integer k2=0; k2<numberOfGrids; k2++)
// //                   if (k2 != k1) {
// //                     if (numberOfGrids == 2) {
// //                         adjustBoundary(k1, baseGridNumber(k2), i1, x1);
// //                         grid[k2].mapping().inverseMap(x1, r2);
// //                         ok = r2(p,0) != (Real)10.;
// // #ifdef ADJUST_FOR_PERIODICITY
// //                         grid[k2].adjustForPeriodicity(r2, ok);
// // #endif // ADJUST_FOR_PERIODICITY
// //                     } else {
// // //                      Gather/scatter points interpolated from grid k2.
// //                         IntegerArray i3(p,d0); RealArray r3(p,d0), x3(p,d0);
// //                         Integer i, j;
// //                         for (i=j=0; i<numberOfInterpolationPoints(k1); i++)
// //                           if (k2 == iG(i)) {
// //                             for (Integer kd=0; kd<numberOfDimensions; kd++) {
// //                                 i3(j,kd) = i1(i,kd);
// //                                 r3(j,kd) = r2(i,kd);
// //                                 x3(j,kd) = x1(i,kd);
// //                             } // end for
// //                             j++;
// //                         } // end if, end for
// //                         if (j) {
// //                             Range q = j;
// //                             adjustBoundary(
// //                               k1, baseGridNumber(k2), i3(q,d0), x3(q,d0));
// //                             grid[k2].mapping().inverseMapC(x3(q,d0), r3(q,d0));
// //                             ok(q) = r3(q,0) != (Real)10.;
// // #ifdef ADJUST_FOR_PERIODICITY
// //                             grid[k2].adjustForPeriodicity(r3(q,d0), ok(q));
// // #endif // ADJUST_FOR_PERIODICITY
// //                             for (i=j=0; i<numberOfInterpolationPoints(k1); i++)
// //                               if (k2 == iG(i)) {
// //                                 for (Integer kd=0; kd<numberOfDimensions; kd++)
// //                                   r2(i,kd) = r3(j,kd);
// //                                 j++;
// //                             } // end if, end for
// //                         } // end if
// //                     } // end if
// //                 } // end if, end for
// //             } // end if (interpolation coordinates )

// //          Update inverseCondition, inverseCoordinates and/or inverseGrid.
//             Logical updateICon = computedGeometry & THEinverseMap &&
//                                  what             & THEinterpolationCondition,
//                     updateIMap = computedGeometry & THEinverseMap;
//             if (updateICon || updateIMap) {
//                 RealArray iCond, iCoord; IntegerArray iGrid;
// //                if (updateICon) iCond .reference(inverseCondition[k1]);
//                 if (updateIMap) iCoord.reference(inverseCoordinates[k1]);
//                 if (updateIMap) iGrid .reference(inverseGrid[k1]);
//                 for (Integer i=0; i<numberOfInterpolationPoints(k1); i++)
//                   if (r2(i,0) != (Real)10.) {
//                     Integer &i01 = i1(i,0), &i02 = i1(i,1), &i03 = i1(i,2);
//                     if (updateICon) iCond(i01,i02,i03) = ic2(i);
//                     if (updateIMap) {
//                         for (Integer kd=0; kd<numberOfDimensions; kd++)
//                           iCoord(kd,i01,i02,i03) = r2(i,kd);
//                         iGrid(i01,i02,i03) = iG(i);
//                     } // end if
//                 } // end if, end for
//             } // end if

// //          Check if the mapping inversion failed anywhere.
//             if (max(r2(p,0)) == (Real)10.) {
//                 returnValue |= COMPUTEfailed;
//                 computedGeometry &= ~(what &
//                   (THEinterpolationCoordinates | THEinterpolationCondition));

//             } else {
//                 if (what & THEinterpolationCoordinates)
//                 interpolationCoordinates[k1] = r2;

//                 if (what & THEinterpoleeLocation) {
// //                  Find new interpolation stencils and check if any changed.
//                     IntegerArray &iL = interpoleeLocation[k1],
//                       interpolationStencil(p,two,three), useBackupRules(p);
//                     RealArray r3(p,d0);
//                     useBackupRules = LogicalFalse;
//                     for (Integer k2=0; k2<numberOfGrids; k2++) if (k1 != k2 &&
//                       multigridLevelNumber(k1) == multigridLevelNumber(k2)) {
//                         Integer i, j, k, k2c = componentGridNumber(k2);
//                         for (i=0, j=0; i<numberOfInterpolationPoints(k1); i++)
//                           if (iG(i) == k2c) {
//                             for (Integer kd=0; kd<numberOfDimensions; kd++)
//                               r3(j,kd) = r2(i,kd);
//                             j++;
//                         } // end if, end for
//                         if (j) {
// //                          Find new interpolation stencils.
//                             Range q = j;
//                             getInterpolationStencil(k1, k2, r3(q,d0),
//                               interpolationStencil(q,two,three),
//                               useBackupRules(q));
//                             for (i=0, j=0, k=0;
//                               i<numberOfInterpolationPoints(k1); i++)
//                               if (iG(i) == k2c) {
//                                 Integer kd, changed;
//                                 for ( kd=0, changed=0;
//                                   kd<numberOfDimensions; kd++)
//                                   if (iG(i) == k2c) {
//                                     if (iL(i,kd)!=interpolationStencil(j,0,kd)){
//                                         iL(i,kd) = interpolationStencil(j,0,kd);
//                                         changed = 1;
//                                     } // end if
//                                 } // end if, end for
//                                 if (changed) {
//                                     for (Integer kd=0; kd<numberOfDimensions;
//                                       kd++) r3(k,kd) = r3(j,kd);
//                                     k++;
//                                 } // end if
//                                 j++;
//                             } // end if, end for
//                             if (k) {
// //                              Check whether stencils that changed are valid.
//                                 Range q = k; ok(q) = LogicalTrue;
//                                 if (!canInterpolate(k1, k2, r3(q,d0), ok(q),
//                                   useBackupRules(q), LogicalTrue)) {
//                                     returnValue      |=  COMPUTEfailed;
//                                     computedGeometry &= ~THEinterpoleeLocation;
//                                 } // end if
//                             } // end if
//                         } // end if
//                     } // end if, end for
//                 } // end if
//             } // end if

//         } else if (numberOfInterpolationPoints(k1)) {
//             returnValue |= COMPUTEfailed;
//             computedGeometry &= ~(what & (THEinterpolationCoordinates |
//               THEinterpoleeLocation    |  THEinterpolationCondition   ));
//         } // end if, end for
//     } // end if

#endif // _SERIAL_APP_ARRAY_H
    return returnValue;
}
