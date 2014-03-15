//
// Who to blame:  Geoff Chesshire and Bill Henshaw
//

#include "MappedGrid.h"

Integer MappedGridData::computeGeometry(
  const Integer& what,
  const Integer& how) {
    Integer returnValue = 0;
    Range d0 = numberOfDimensions, one = 1, all;
    if (how & USEdifferenceApproximation ||
      mapping.mapPointer == NULL || mapping.getClassName() == "Mapping") {
//
//      Compute the coordinates of the vertices from the mapping, and
//      compute all other geometric quantities from the vertex coordinates.
//      If the grid is vertex-centered then compute center if needed, and
//      if the grid is cell-centered then compute corner if needed also.
//
        Range d[3], &d1=d[0], &d2=d[1], &d3=d[2];
        Index i[3], &i1=i[0], &i2=i[1], &i3=i[2],
              j[3], &j1=j[0], &j2=j[1], &j3=j[2],
              k[3], &k1=k[0], &k2=k[1], &k3=k[2],
              l[3], &l1=l[0], &l2=l[1], &l3=l[2];
        Integer kd;
        for (kd=0; kd<3; kd++) i[kd] = j[kd] = k[kd] = l[kd] = d[kd] =
          Range(dimension(0,kd), dimension(1,kd));
        RealDistributedArray x;
        if (what & (
            THEvertex                  |
            THEvertexDerivative        |
            THEvertexJacobian          |
            THEinverseVertexDerivative |
            THEboundingBox             ) || (
          isAllVertexCentered && what & (
            THEcenter                  |
            THEcenterDerivative        |
            THEcenterJacobian          |
            THEinverseCenterDerivative )) || (
          isAllCellCentered && what &
            THEcorner                  )) {
//
//          Compute geometry at the vertices.
//
            if (!(how & COMPUTEgeometry && what & THEvertex) &&
              computedGeometry & THEvertex) {
//              Vertex is assumed to contain good data.
                x.reference(*vertex);
            } else if (isAllVertexCentered &&
              !(how & COMPUTEgeometry && what & THEcenter) &&
              computedGeometry & THEcenter) {
//              Center is assumed to contain good data.
                x.reference(*center);
            } else if (isAllCellCentered &&
              !(how & COMPUTEgeometry && what & THEcorner) &&
              computedGeometry & THEcorner) {
//              Corner is assumed to contain good data.
                x.reference(*corner);
            } else {
//              We must compute the vertices.
                if (what & THEvertex) {
                    x.reference(*vertex);
                } else if (isAllVertexCentered && what & THEcenter) {
                    x.reference(*center);
                } else if (isAllCellCentered && what & THEcorner) {
                    x.reference(*corner);
                } else {
                    x.partition(partition); x.redim(d1,d2,d3,d0);
                } // end if
                RealArray xL;
                xL.reference(x.getLocalArrayWithGhostBoundaries());
                Range dL[3], &d1L=dL[0], &d2L=dL[1], &d3L=dL[2];
                Index iL[3], &i1L=iL[0], &i2L=iL[1], &i3L=iL[2];
                IntegerArray dimensionL(2,3);
                Integer numberOfPoints = 1;
                for (kd=0; kd<3; kd++) {
                    dimensionL(0,kd) = xL.getBase(kd),
                    dimensionL(1,kd) = xL.getBound(kd);
                    iL[kd] = dL[kd] = Range(dimensionL(0,kd), dimensionL(1,kd));
                    numberOfPoints *= dimensionL(1,kd) - dimensionL(0,kd) + 1;
                } // end for
                if (numberOfPoints) {
                    Range p = numberOfPoints;
                    RealArray r(d1L,d2L,d3L,d0);
                    for (kd=0; kd<numberOfDimensions; kd++) {
                        for (Integer k=dimensionL(0,kd);
                          k<=dimensionL(1,kd); k++) {
                            iL[kd] = k;
                            r(i1L,i2L,i3L,kd) = gridSpacing(kd) *
                              (k - indexRange(0,kd));
                        } // end for
                        iL[kd] = dL[kd];
                    } // end for
                    if (mapping.mapPointer == NULL) {
                        xL = r;
                        cerr << "MappedGridData::computeGeometry():  "
                             << "WARNING:  The grid's mapping is missing."
                             << endl
                             << "The identity mapping is used in its place.  "
                             << "The result is incorrect geometry."
                             << endl;
                        returnValue |= COMPUTEfailed;
                    } else if (mapping.getClassName() == "Mapping") {
                        xL = r;
                        cerr << "MappedGridData::computeGeometry():  WARNING:"
                             <<endl
                             << "The type of the grid's mapping is the "
                             << "base class \"Mapping\".  This is invalid."
                             << endl
                             << "The identity mapping is used in its place.  "
                             << "The result is incorrect geometry."
                             << endl;
                        returnValue |= COMPUTEfailed;
                    } else {
                        r.reshape(p,d0); xL.reshape(p,d0);
                        mapping.map(r, xL);
                    } // end if
                    if (what & THEvertex)
                      computedGeometry |= THEvertex;
                    if (isAllVertexCentered && what & THEcenter)
                      computedGeometry |= THEcenter;
                    if (isAllCellCentered && what & THEcorner)
                      computedGeometry |= THEcorner;
                    if (returnValue & COMPUTEfailed && (what & THEvertex || (
                      isAllVertexCentered && what & THEcenter) || (
                      isAllCellCentered   && what & THEcorner))) cerr
                      << "MappedGridData::computeGeometry():  WARNING:" << endl
                      << "Incorrect geometry was computed for "
                      << "vertex and/or center." << endl;
                } // end if
            } // end if

//          xr is needed for vertexJacobian and inverseVertexDerivative.
#ifdef DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
            RealMappedGridFunction xr;
#else
            RealDistributedArray xr;
#endif // DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
            if (!(what & THEvertexDerivative) &&
              computedGeometry & THEvertexDerivative) {
//              VertexDerivative is assumed to contain good data.
                xr.reference(*vertexDerivative);
#ifndef DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                xr.reshape(d1, d2, d3, d0, d0); //***** Needed until MappedGridFunctions use higher-dimensional arrays.
#endif // DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
            } else if (isAllVertexCentered && !(what & THEcenterDerivative) &&
              computedGeometry & THEcenterDerivative) {
//              CenterDerivative is assumed to contain good data.
                xr.reference(*centerDerivative);
#ifndef DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                xr.reshape(d1, d2, d3, d0, d0); //***** Needed until MappedGridFunctions use higher-dimensional arrays.
#endif // DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
            } else if (what & (
                THEvertexDerivative        |
                THEvertexJacobian          |
                THEinverseVertexDerivative ) || (
              isAllVertexCentered && what & (
                THEcenterDerivative        |
                THEcenterJacobian          |
                THEinverseCenterDerivative ))) {
//              We must compute the derivative at the vertices.
                if (what & THEvertexDerivative)
                     xr.reference(*vertexDerivative);
                else if (isAllVertexCentered && what & THEcenterDerivative)
                     xr.reference(*centerDerivative);
#ifdef DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                else xr.updateToMatchGrid(*this, all, all, all, d0, d0);
#else
                else { xr.partition(partition); xr.redim(d1, d2, d3, d0, d0); }
                xr.reshape(d1, d2, d3, d0, d0); //***** Needed until MappedGridFunctions use higher-dimensional arrays.
#endif // DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                for (kd=0; kd<numberOfDimensions; kd++) {
                    xr(d1,d2,d3,d0,kd) = x;
                    i[kd] = Range(dimension(0,kd)+1, dimension(1,kd)-1);
                    j[kd] = i[kd] + 1;
                    k[kd] = i[kd] - 1;
                    xr(i1,i2,i3,d0,kd) = ((Real).5 / gridSpacing(kd)) *
                      (xr(j1,j2,j3,d0,kd) - xr(k1,k2,k3,d0,kd));
//                  Extrapolate to the left.
                    i[kd] = Range(dimension(0,kd), dimension(0,kd));
                    j[kd] = i[kd] + 1;
                    xr(i1,i2,i3,d0,kd) = xr(j1,j2,j3,d0,kd);
//                  Extrapolate to the right.
                    i[kd] = Range(dimension(1,kd), dimension(1,kd));
                    k[kd] = i[kd] - 1;
                    xr(i1,i2,i3,d0,kd) = xr(k1,k2,k3,d0,kd);
                    i[kd] = j[kd] = k[kd] = d[kd];
                } // end for
                if (what & THEvertexDerivative)
                  vertexDerivative->periodicUpdate();
                else if (isAllVertexCentered && what & THEcenterDerivative)
                  centerDerivative->periodicUpdate();
                if (what & THEvertexDerivative)
                  computedGeometry |= THEvertexDerivative;
                if (isAllVertexCentered && what & THEcenterDerivative)
                  computedGeometry |= THEcenterDerivative;
                if (returnValue & COMPUTEfailed) cerr
                  << "MappedGridData::computeGeometry():  WARNING:" << endl
                  << "Incorrect geometry was computed for vertexDerivative, "
                  << "centerDerivative," << endl
                  << "inverseVertexDerivative, inverseCenterDerivative, "
                  << "vertexJacobian and/or " << endl
                  << "centerJacobian." << endl;
            } // end if

            RealDistributedArray vj; // Needed for inverseVertexDerivative.
            if (what & (
                THEvertexJacobian          |
                THEinverseVertexDerivative ) || (
              isAllVertexCentered && what & (
                THEcenterJacobian          |
                THEinverseCenterDerivative ))) {
                if (what & THEvertexJacobian) {
                    vj.reference(*vertexJacobian);
                } else if (isAllVertexCentered && what & THEcenterJacobian) {
                    vj.reference(*centerJacobian);
                } else {
                    vj.partition(partition);
                    vj.redim(d1,d2,d3);
                } // end if
                switch (numberOfDimensions) {
                  case 1:
                    vj = xr;
                  break;
                  case 2:
                    vj = xr(d1,d2,d3,0,0) * xr(d1,d2,d3,1,1)
                       - xr(d1,d2,d3,0,1) * xr(d1,d2,d3,1,0);
                  break;
                  case 3:
                    vj =
                      (xr(d1,d2,d3,0,0) * xr(d1,d2,d3,1,1) -
                       xr(d1,d2,d3,0,1) * xr(d1,d2,d3,1,0)) * xr(d1,d2,d3,2,2) +
                      (xr(d1,d2,d3,0,1) * xr(d1,d2,d3,1,2) -
                       xr(d1,d2,d3,0,2) * xr(d1,d2,d3,1,1)) * xr(d1,d2,d3,2,0) +
                      (xr(d1,d2,d3,0,2) * xr(d1,d2,d3,1,0) -
                       xr(d1,d2,d3,0,0) * xr(d1,d2,d3,1,2)) * xr(d1,d2,d3,2,1);
                  break;
                } // end switch
                if (what & THEvertexJacobian)
                  vertexJacobian->periodicUpdate();
                else if (isAllVertexCentered && what & THEcenterJacobian)
                  centerJacobian->periodicUpdate();
                if (what & THEvertexJacobian)
                  computedGeometry |= THEvertexJacobian;
                if (isAllVertexCentered && what & THEcenterJacobian)
                  computedGeometry |= THEcenterJacobian;
            } // end if

            if (what &
                THEinverseVertexDerivative || (
              isAllVertexCentered && what &
                THEinverseCenterDerivative )) {
#ifdef DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                RealDistributedArray det; det.partition(partition);
                det.redim(d1,d2,d3); RealMappedGridFunction rx;
#else
                RealDistributedArray det, rx;
                det.partition(partition); rx.partition(partition);
                det.redim(d1,d2,d3);
#endif // DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                if (what & THEinverseVertexDerivative)
                     rx.reference(*inverseVertexDerivative);
                else rx.reference(*inverseCenterDerivative);
#ifndef DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                rx.reshape(d1, d2, d3, d0, d0); //***** Needed until MappedGridFunctions use higher-dimensional arrays.
#endif // DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                where (vj == (Real)0.) det = (Real)1.;
                otherwise() det = (Real)1. / vj;
                switch (numberOfDimensions) {
                  case 1:
                    rx = det;
                  break;
                  case 2: {
                    rx = (Real)0.;
                    for (Integer kd3=0; kd3<numberOfDimensions; kd3++) {
                        Integer kd4=1-kd3;
                        for (Integer kd1=0; kd1<numberOfDimensions; kd1++) {
                            Integer kd2=1-kd1;
                            rx(d1,d2,d3,kd3,kd1) =
                              (Real)((kd2 - kd1) * (kd4 - kd3)) *
                              xr(d1,d2,d3,kd2,kd4) * det(d1,d2,d3);
                        } // end for
                    } // end for
                  } break;
                  case 3: {
                    rx = (Real)0.;
                    for (Integer kd4=0; kd4<numberOfDimensions; kd4++) {
                        Integer kd5=(kd4+1)%3, kd6=(kd5+1)%3;
                        for (Integer kd1=0; kd1<numberOfDimensions; kd1++) {
                            Integer kd2=(kd1+1)%3, kd3=(kd2+1)%3;
                            rx(d1,d2,d3,kd4,kd1) =
                              ( xr(d1,d2,d3,kd2,kd5) *
                                xr(d1,d2,d3,kd3,kd6)
                              - xr(d1,d2,d3,kd2,kd6) *
                                xr(d1,d2,d3,kd3,kd5) ) * det(d1,d2,d3);
                        } // end for
                    } // end for
                  } break;
                } // end switch
                if (what & THEinverseVertexDerivative)
                     inverseVertexDerivative->periodicUpdate();
                else inverseCenterDerivative->periodicUpdate();
                if (what & THEinverseVertexDerivative)
                  computedGeometry |= THEinverseVertexDerivative;
                if (isAllVertexCentered && what & THEinverseCenterDerivative)
                  computedGeometry |= THEinverseCenterDerivative;
            } // end if
#ifdef DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
            xr.destroy(); vj.redim(0); // Save some space.
#else
            xr.redim(0); vj.redim(0); // Save some space.
#endif // DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS

//             if (what & THEminMaxEdgeLength) {
//                 RealDistributedArray y; y.partition(partition);
//                 y.redim(d1,d2,d3,d0);
//                 for (kd=0; kd<3; kd++) if (kd < numberOfDimensions) {
//                     i[kd] = Range(dimension(0,kd), dimension(1,kd)-1);
//                     j[kd] = Range(dimension(0,kd)+1, dimension(1,kd));
//                     y(i1,i2,i3,d0) = x(j1,j2,j3,d0) - x(i1,i2,i3,d0);
//                     y(i1,i2,i3,d0) = y(i1,i2,i3,d0) * y(i1,i2,i3,d0);
//                     RealDistributedArray z; z.partition(partition);
//                     z.redim(i1,i2,i3); z = (Real)0.;
//                     for (Integer kd1=0; kd1<numberOfDimensions; kd1++)
//                       z += y(i1,i2,i3,kd1);
//                     minimumEdgeLength(kd) = Sqrt(min(z));
//                     maximumEdgeLength(kd) = Sqrt(max(z));
//                     i[kd] = j[kd] = d[kd];
//                 } else {
//                     minimumEdgeLength(kd) = (Real)1.;
//                     maximumEdgeLength(kd) = (Real)1.;
//                 } // end if, end for
//                 computedGeometry |= THEminMaxEdgeLength;
//                 if (returnValue & COMPUTEfailed) cerr
//                   << "MappedGridData::computeGeometry():  WARNING:  " << endl
//                   << "Incorrect geometry was computed for minimumEdgeLength "
//                   << "and maximumEdgeLength." << endl;
//             } // end if

            if (what & THEboundingBox) {
                boundingBox = (Real)0.;
                for (kd=0; kd<numberOfDimensions; kd++)
                  i[kd] = Range(gridIndexRange(0,kd), gridIndexRange(1,kd));
                for (kd=0; kd<numberOfDimensions; kd++) {
                    boundingBox(0,kd) = min(x(i1,i2,i3,kd));
                    boundingBox(1,kd) = max(x(i1,i2,i3,kd));
                } // end for
                for (kd=numberOfDimensions; kd<3; kd++)
                  boundingBox(0,kd) = boundingBox(1,kd) = (Real)0.;
                for (kd=0; kd<numberOfDimensions; kd++) i[kd] = d[kd];
                computedGeometry |= THEboundingBox;
                if (returnValue & COMPUTEfailed) cerr
                  << "MappedGridData::computeGeometry():  WARNING:"     << endl
                  << "Incorrect geometry was computed for boundingBox." << endl;
            } // end if
        } // end if
//
//      Compute the coordinates of the corners from the mapping, and
//      compute all other geometric quantities from the corner coordinates.
//
        if (what & (
            THEcellVolume              |
            THEcenterNormal            |
            THEcenterArea              |
            THEfaceNormal              |
            THEfaceArea                ) || (
          !isAllVertexCentered && what & (
            THEcenter                  |
            THEcenterDerivative        |
            THEcenterJacobian          |
            THEinverseCenterDerivative )) || (
          !isAllCellCentered && what &
            THEcorner                  )) {
//
//          Compute geometry at the cell corners.
//
            if (isAllCellCentered && what & (
              THEvertex                  |
              THEvertexDerivative        |
              THEvertexJacobian          |
              THEinverseVertexDerivative |
              THEboundingBox             |
              THEcorner                  )) {
//              x was already computed at the corners.
            } else if (!(how & COMPUTEgeometry && what & THEcorner) &&
              computedGeometry & THEcorner) {
//              Corner is assumed to contain good data.
                x.reference(*corner);
            } else {
//              We must compute the corners.
                if (what & THEcorner) {
                    x.reference(*corner);
                } else {
                    x.partition(partition);
                    x.redim(d1,d2,d3,d0);
                } // end if
                RealArray xL;
                xL.reference(x.getLocalArrayWithGhostBoundaries());
                Range dL[3], &d1L=dL[0], &d2L=dL[1], &d3L=dL[2];
                Index iL[3], &i1L=iL[0], &i2L=iL[1], &i3L=iL[2];
                IntegerArray dimensionL(2,3);
                Integer numberOfPoints = 1;
                for (kd=0; kd<3; kd++) {
                    dimensionL(0,kd) = xL.getBase(kd),
                    dimensionL(1,kd) = xL.getBound(kd);
                    iL[kd] = dL[kd] = Range(dimensionL(0,kd), dimensionL(1,kd));
                    numberOfPoints *= dimensionL(1,kd) - dimensionL(0,kd) + 1;
                } // end for
                if (numberOfPoints) {
                    Range p = numberOfPoints;
                    RealArray r(d1L,d2L,d3L,d0);
                    for (kd=0; kd<numberOfDimensions; kd++) {
                        for (Integer k=dimensionL(0,kd);
                          k<=dimensionL(1,kd); k++) {
                            iL[kd] = k;
                            r(i1L,i2L,i3L,kd) = gridSpacing(kd) *
                              ( k - indexRange(0,kd)
                              - (isCellCentered(kd) ? (Real)0. : (Real).5) );
                        } // end for
                        iL[kd] = dL[kd];
                    } // end for
                    if (mapping.mapPointer == NULL) {
                        xL = r;
                        cerr << "MappedGridData::computeGeometry():  "
                             << "WARNING:  The grid's mapping is missing."
                             << endl
                             << "The identity mapping is used in its place.  "
                             << "The result is incorrect geometry."
                             << endl;
                        returnValue |= COMPUTEfailed;
                    } else if (mapping.getClassName() == "Mapping") {
                        xL = r;
                        cerr << "MappedGridData::computeGeometry():  WARNING:"
                             <<endl
                             << "The type of the grid's mapping is the "
                             << "base class \"Mapping\".  This is invalid."
                             << endl
                             << "The identity mapping is used in its place.  "
                             << "The result is incorrect geometry."
                             << endl;
                        returnValue |= COMPUTEfailed;
                    } else {
                        r.reshape(p,d0); xL.reshape(p,d0);
                        mapping.map(r, xL);
                    } // end if
                    if (what & THEcorner)
                      computedGeometry |= THEcorner;
                    if (returnValue & COMPUTEfailed && what & THEcorner) cerr
                      << "MappedGridData::computeGeometry():  WARNING:" << endl
                      << "Incorrect geometry was computed for corner." << endl;
                } // end if
            } // end if

            if (!isAllVertexCentered && what & THEcenter) {
                *center = x;
                for (kd=0; kd<numberOfDimensions; kd++) {
                    i[kd] = Range(dimension(0,kd), dimension(1,kd)-1);
                    j[kd] = i[kd] + 1;
                    (*center)(i1,i2,i3,d0) = (Real).5 *
                      ((*center)(i1,i2,i3,d0) + (*center)(j1,j2,j3,d0));
                    i[kd] = j[kd] = d[kd];
                } // end for
                center->periodicUpdate();
                computedGeometry |= THEcenter;
                if (returnValue & COMPUTEfailed) cerr
                  << "MappedGridData::computeGeometry():  WARNING:" << endl
                  << "Incorrect geometry was computed for center, " << endl;
            } // end if

//          xr is needed for centerJacobian and inverseCenterDerivative.
#ifdef DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
            RealMappedGridFunction xr;
#else
            RealDistributedArray xr;
#endif // DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
            if (!isAllVertexCentered &&
              !(what & THEcenterDerivative) &&
              computedGeometry & THEcenterDerivative) {
//              CenterDerivative is assumed to contain good data.
                xr.reference(*centerDerivative);
#ifndef DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                xr.reshape(d1, d2, d3, d0, d0); //***** Needed until MappedGridFunctions use higher-dimensional arrays.
#endif // DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
            } else if (!isAllVertexCentered && what & (
              THEcenterDerivative        |
              THEinverseCenterDerivative |
              THEcenterJacobian          )) {
//              We must compute the derivative at the centers.
                if (what & THEcenterDerivative)
                     xr.reference(*centerDerivative);
#ifdef DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                else xr.updateToMatchGrid(*this, all, all, all, d0, d0);
#else
                else xr.redim(d1, d2, d3, d0, d0);
                xr.reshape(d1, d2, d3, d0, d0); //***** Needed until MappedGridFunctions use higher-dimensional arrays.
#endif // DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                for (Integer kd1=0; kd1<numberOfDimensions; kd1++) {
                    xr(d1,d2,d3,d0,kd1) = x;
                    i[kd1] = Range(dimension(0,kd1), dimension(1,kd1)-1);
                    j[kd1] = i[kd1] + 1;
                    xr(i1,i2,i3,d0,kd1) =
                      ((Real)1. / gridSpacing(kd1)) *
                      ( xr(j1,j2,j3,d0,kd1) - xr(i1,i2,i3,d0,kd1) );
//                  Extrapolate to the right.
                    i[kd1] = Range(dimension(1,kd1), dimension(1,kd1));
                    j[kd1] = i[kd1] - 1;
                    xr(i1,i2,i3,d0,kd1) = xr(j1,j2,j3,d0,kd1);
                    i[kd1] = j[kd1] = d[kd1];
//                  Average in the other directions.
                    for (Integer kd2=0; kd2<numberOfDimensions; kd2++)
                      if (kd2 != kd1) {
                        i[kd2] = Range(dimension(0,kd2), dimension(1,kd2)-1);
                        j[kd2] = i[kd2] + 1;
                        xr(i1,i2,i3,d0,kd1) = (Real).5 *
                          (xr(i1,i2,i3,d0,kd1) + xr(j1,j2,j3,d0,kd1));
//                      Extrapolate to the right.
                        i[kd2] = Range(dimension(1,kd2), dimension(1,kd2));
                        j[kd2] = i[kd2] - 1;
                        xr(i1,i2,i3,d0,kd1) = xr(j1,j2,j3,d0,kd1);
                        i[kd2] = j[kd2] = d[kd2];
                    } // end if, end for
                } // end for
                if (what & THEcenterDerivative) {
                    centerDerivative->periodicUpdate();
                    computedGeometry |= THEcenterDerivative;
                } // end if
            } // end if

            RealDistributedArray cj; // Needed for inverseCenterDerivative.
            if (!isAllVertexCentered && what & (
              THEcenterJacobian          |
              THEinverseCenterDerivative )) {
                if (what & THEcenterJacobian)
                     cj.reference(*centerJacobian);
                else cj.redim(d1,d2,d3);
                switch (numberOfDimensions) {
                  case 1:
                    cj = xr;
                  break;
                  case 2:
                    cj = xr(i1,i2,i3,0,0) * xr(i1,i2,i3,1,1)
                       - xr(i1,i2,i3,0,1) * xr(i1,i2,i3,1,0);
                  break;
                  case 3:
                    cj =
                      (xr(i1,i2,i3,0,0) * xr(i1,i2,i3,1,1) -
                       xr(i1,i2,i3,0,1) * xr(i1,i2,i3,1,0)) * xr(i1,i2,i3,2,2) +
                      (xr(i1,i2,i3,0,1) * xr(i1,i2,i3,1,2) -
                       xr(i1,i2,i3,0,2) * xr(i1,i2,i3,1,1)) * xr(i1,i2,i3,2,0) +
                      (xr(i1,i2,i3,0,2) * xr(i1,i2,i3,1,0) -
                       xr(i1,i2,i3,0,0) * xr(i1,i2,i3,1,2)) * xr(i1,i2,i3,2,1);
                  break;
                } // end switch
                if (what & THEcenterJacobian) {
                    centerJacobian->periodicUpdate();
                    computedGeometry |= THEcenterJacobian;
                } // end if
            } // end if

            if (!isAllVertexCentered && what & THEinverseCenterDerivative) {
                RealDistributedArray det; det.partition(partition);
                det.redim(d1,d2,d3);
                where (cj == (Real)0.) det = (Real)1.;
                otherwise() det = (Real)1. / cj;
                switch (numberOfDimensions) {
                  case 1:
                    *inverseCenterDerivative = det;
                  break;
                  case 2: {
                    *inverseCenterDerivative = (Real)0.;
                    for (Integer kd3=0; kd3<numberOfDimensions; kd3++) {
                        Integer kd4=1-kd3;
                        for (Integer kd1=0; kd1<numberOfDimensions; kd1++) {
                            Integer kd2=1-kd1;
                            (*inverseCenterDerivative)(d1,d2,d3,kd3,kd1) =
                              (Real)((kd2 - kd1) * (kd4 - kd3)) *
                              xr(d1,d2,d3,kd2,kd4) * det;
                        } // end for
                    } // end for
                  } break;
                  case 3: {
                    *inverseCenterDerivative = (Real)0.;
                    for (Integer kd4=0; kd4<numberOfDimensions; kd4++) {
                        Integer kd5=(kd4+1)%3, kd6=(kd5+1)%3;
                        for (Integer kd1=0; kd1<numberOfDimensions; kd1++) {
                            Integer kd2=(kd1+1)%3, kd3=(kd2+1)%3;
                            (*inverseCenterDerivative)(d1,d2,d3,kd4,kd1) =
                              ( xr(d1,d2,d3,kd2,kd5) *
                                xr(d1,d2,d3,kd3,kd6)
                              - xr(d1,d2,d3,kd2,kd6) *
                                xr(d1,d2,d3,kd3,kd5) ) * det;
                        } // end for
                    } // end for
                  } break;
                } // end switch
                inverseCenterDerivative->periodicUpdate();
                computedGeometry |= THEinverseCenterDerivative;
            } // end if
#ifdef DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
            xr.destroy(); cj.redim(0); // Save some space.
#else
            xr.redim(0); cj.redim(0); // Save some space.
#endif // DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS

            if (what & THEcellVolume) {
                for (kd=0; kd<numberOfDimensions; kd++)
                  i[kd] = Range(dimension(0,kd), dimension(1,kd)-1);
                switch (numberOfDimensions) {
                  case 1:
                    (*cellVolume)(i1,i2,i3) = x(i1+1,i2,i3) - x(i1,i2,i3);
                  break;
                  case 2:
                    (*cellVolume)(i1,i2,i3) = (Real).5 *
                      ( (x(i1+1,i2+1,i3,0) - x(i1  ,i2,i3,0)) *
                        (x(i1  ,i2+1,i3,1) - x(i1+1,i2,i3,1))
                      - (x(i1  ,i2+1,i3,0) - x(i1+1,i2,i3,0)) *
                        (x(i1+1,i2+1,i3,1) - x(i1  ,i2,i3,1)) );
                  break;
                  case 3:
                    (*cellVolume)(i1,i2,i3) = ((Real)1. / (Real)6.) *
                      ( ( (x(i1  ,i2+1,i3+1,1) - x(i1  ,i2  ,i3  ,1)) *
                          (x(i1  ,i2  ,i3+1,2) - x(i1  ,i2+1,i3  ,2))
                        - (x(i1  ,i2+1,i3+1,2) - x(i1  ,i2  ,i3  ,2)) *
                          (x(i1  ,i2  ,i3+1,1) - x(i1  ,i2+1,i3  ,1))
                        + (x(i1+1,i2  ,i3+1,1) - x(i1  ,i2  ,i3  ,1)) *
                          (x(i1+1,i2  ,i3  ,2) - x(i1  ,i2  ,i3+1,2))
                        - (x(i1+1,i2  ,i3+1,2) - x(i1  ,i2  ,i3  ,2)) *
                          (x(i1+1,i2  ,i3+1,1) - x(i1  ,i2  ,i3+1,1))
                        + (x(i1+1,i2+1,i3  ,1) - x(i1  ,i2  ,i3  ,1)) *
                          (x(i1  ,i2+1,i3  ,2) - x(i1+1,i2  ,i3  ,2))
                        - (x(i1+1,i2+1,i3  ,2) - x(i1  ,i2  ,i3  ,2)) *
                          (x(i1  ,i2+1,i3  ,1) - x(i1+1,i2  ,i3  ,1)) ) *
                          (x(i1+1,i2+1,i3+1,0) - x(i1  ,i2  ,i3  ,0))
                      + ( (x(i1  ,i2+1,i3+1,2) - x(i1  ,i2  ,i3  ,2)) *
                          (x(i1  ,i2  ,i3+1,0) - x(i1  ,i2+1,i3  ,0))
                        - (x(i1  ,i2+1,i3+1,0) - x(i1  ,i2  ,i3  ,0)) *
                          (x(i1  ,i2  ,i3+1,2) - x(i1  ,i2+1,i3  ,2))
                        + (x(i1+1,i2  ,i3+1,2) - x(i1  ,i2  ,i3  ,2)) *
                          (x(i1+1,i2  ,i3  ,0) - x(i1  ,i2  ,i3+1,0))
                        - (x(i1+1,i2  ,i3+1,0) - x(i1  ,i2  ,i3  ,0)) *
                          (x(i1+1,i2  ,i3  ,2) - x(i1  ,i2  ,i3+1,2))
                        + (x(i1+1,i2+1,i3  ,2) - x(i1  ,i2  ,i3  ,2)) *
                          (x(i1  ,i2+1,i3  ,0) - x(i1+1,i2  ,i3  ,0))
                        - (x(i1+1,i2+1,i3  ,0) - x(i1  ,i2  ,i3  ,0)) *
                          (x(i1  ,i2+1,i3  ,2) - x(i1+1,i2  ,i3  ,2)) ) *
                          (x(i1+1,i2+1,i3+1,1) - x(i1  ,i2  ,i3  ,1))
                      + ( (x(i1  ,i2+1,i3+1,0) - x(i1  ,i2  ,i3  ,0)) *
                          (x(i1  ,i2  ,i3+1,1) - x(i1  ,i2+1,i3  ,1))
                        - (x(i1  ,i2+1,i3+1,1) - x(i1  ,i2  ,i3  ,1)) *
                          (x(i1  ,i2  ,i3+1,0) - x(i1  ,i2+1,i3  ,0))
                        + (x(i1+1,i2  ,i3+1,0) - x(i1  ,i2  ,i3  ,0)) *
                          (x(i1+1,i2  ,i3  ,1) - x(i1  ,i2  ,i3+1,1))
                        - (x(i1+1,i2  ,i3+1,1) - x(i1  ,i2  ,i3  ,1)) *
                          (x(i1+1,i2  ,i3  ,0) - x(i1  ,i2  ,i3+1,0))
                        + (x(i1+1,i2+1,i3  ,0) - x(i1  ,i2  ,i3  ,0)) *
                          (x(i1  ,i2+1,i3  ,1) - x(i1+1,i2  ,i3  ,1))
                        - (x(i1+1,i2+1,i3  ,1) - x(i1  ,i2  ,i3  ,1)) *
                          (x(i1  ,i2+1,i3  ,0) - x(i1+1,i2  ,i3  ,0)) ) *
                          (x(i1+1,i2+1,i3+1,2) - x(i1  ,i2  ,i3  ,2)) );
                  break;
                } // end switch
                for (kd=0; kd<numberOfDimensions; kd++) i[kd] = d[kd];
                Real minCellVolume = min((*cellVolume)(i1,i2,i3));
                if (minCellVolume == (Real)0.) cerr
                  << "MappedGridData::computeGeometry():  WARNING:  "
                  << "Computed a zero cellVolume." << endl;
                else if (minCellVolume < (Real)0.) cerr
                  << "MappedGridData::computeGeometry():  WARNING:  "
                  << "Computed a negative cellVolume." << endl;
                cellVolume->periodicUpdate();
                computedGeometry |= THEcellVolume;
                if (returnValue & COMPUTEfailed) cerr
                  << "MappedGridData::computeGeometry():  WARNING:  "   << endl
                  << "Incorrect geometry was computed for cellVolume, " << endl;
            } // end if

            if (what & (
              THEcenterNormal |
              THEcenterArea   |
              THEfaceNormal   |
              THEfaceArea     )) {
#ifdef DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                RealMappedGridFunction fn, cn;
#else
                RealArray fn, cn;
#endif // DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                if (what & THEfaceNormal) fn.reference(*faceNormal);
#ifdef DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                else fn.updateToMatchGrid(*this, all, all, all, d0, d0);
#else
                else fn.redim(d1, d2, d3, d0, d0);
                fn.reshape(d1, d2, d3, d0, d0); //***** Needed until MappedGridFunctions use higher-dimensional arrays.
#endif // DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                switch (numberOfDimensions) {
                  case 1:
                    fn = (Real)1.;
                  break;
                  case 2: {
                    fn = (Real)0.;
                    for (Integer kd1=0; kd1<numberOfDimensions; kd1++) {
                        Integer kd2=1-kd1;
                        i[kd2] = Range(dimension(0,kd2), dimension(1,kd2)-1);
                        j[kd2] = i[kd2] + 1;
                        for (Integer kd3=0; kd3<numberOfDimensions; kd3++) {
                            Integer kd4=1-kd3;
                            fn(i1,i2,i3,kd3,kd1) =
                              (Real)((kd2 - kd1) * (kd4 - kd3)) *
                              (x(j1,j2,j3,kd4) - x(i1,i2,i3,kd4));
                        } // end for
                        i[kd2] = j[kd2] = d[kd2];
                    } // end for
                  } break;
                  case 3: {
                    fn = (Real)0.;
                    for (Integer kd1=0; kd1<numberOfDimensions; kd1++) {
                        Integer kd2=(kd1+1)%3, kd3=(kd2+1)%3;
                        i[kd2] = Range(dimension(0,kd2), dimension(1,kd2)-1);
                        i[kd3] = Range(dimension(0,kd3), dimension(1,kd3)-1);
                        k[kd2] = i[kd2]; j[kd2] = l[kd2] = i[kd2] + 1;
                        j[kd3] = i[kd3]; k[kd3] = l[kd3] = i[kd3] + 1;
                        for (Integer kd4=0; kd4<numberOfDimensions; kd4++) {
                            Integer kd5=(kd4+1)%3, kd6=(kd5+1)%3;
                            fn(i1,i2,i3,kd4,kd1) = (Real).5 *
                              ( (x(l1,l2,l3,kd5) - x(i1,i2,i3,kd5)) *
                                (x(k1,k2,k3,kd6) - x(j1,j2,j3,kd6))
                              - (x(l1,l2,l3,kd6) - x(i1,i2,i3,kd6)) *
                                (x(k1,k2,k3,kd5) - x(j1,j2,j3,kd5)) );
                        } // end for
                        i[kd2] = j[kd2] = k[kd2] = l[kd2] = d[kd2];
                        i[kd3] = j[kd3] = k[kd3] = l[kd3] = d[kd3];
                    } // end for
                  } break;
                } // end switch
                if (what & THEfaceNormal) {
                    faceNormal->periodicUpdate();
                    computedGeometry |= THEfaceNormal;
                } // end if
                if (what & THEfaceArea) {
#ifdef DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                    RealMappedGridFunction fa; fa.reference(*faceArea);
                    fa.updateToMatchGrid(*this, all, all, all, one, d0);
#else
                    RealArray fa; fa.reference(*faceArea);
                    fa.reshape(d1, d2, d3, one, d0);
#endif // DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                    switch (numberOfDimensions) {
                      case 1:
                        fa = (Real)1.;
                      break;
                      case 2: {
                        fa = (Real)1.;
                        for (Integer kd1=0; kd1<numberOfDimensions; kd1++) {
                            Integer kd2=1-kd1;
                            i[kd2] = Range(dimension(0,kd2),dimension(1,kd2)-1);
                            fa(i1,i2,i3,0,kd1) = sqrt
                              ( fn(i1,i2,i3,0,kd1) * fn(i1,i2,i3,0,kd1)
                              + fn(i1,i2,i3,1,kd1) * fn(i1,i2,i3,1,kd1) );
                            i[kd2] = d[kd2];
                        } // end for
                      } break;
                      case 3: {
                        fa = (Real)1.;
                        for (Integer kd1=0; kd1<numberOfDimensions; kd1++) {
                            Integer kd2=(kd1+1)%3, kd3=(kd2+1)%3;
                            i[kd2] = Range(dimension(0,kd2),dimension(1,kd2)-1);
                            i[kd3] = Range(dimension(0,kd3),dimension(1,kd3)-1);
                            fa(i1,i2,i3,0,kd1) = sqrt
                              ( fn(i1,i2,i3,0,kd1) * fn(i1,i2,i3,0,kd1)
                              + fn(i1,i2,i3,1,kd1) * fn(i1,i2,i3,1,kd1)
                              + fn(i1,i2,i3,2,kd1) * fn(i1,i2,i3,2,kd1) );
                            i[kd2] = d[kd2]; i[kd3] = d[kd3];
                        } // end for
                      } break;
                    } // end switch
                    faceArea->periodicUpdate();
                    computedGeometry |= THEfaceArea;
                } // end if
                if (what & (
                  THEcenterNormal |
                  THEcenterArea   )) {
                    if (what & THEcenterNormal) cn.reference(*centerNormal);
#ifdef DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                    else cn.updateToMatchGrid(*this, all, all, all, d0, d0);
#else
                    else cn.redim(d1, d2, d3, d0, d0);
                    cn.reshape(d1, d2, d3, d0, d0); //***** Needed until MappedGridFunctions use higher-dimensional arrays.
#endif // DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                    cn = fn;
                    for (kd=0; kd<numberOfDimensions; kd++) {
                        i[kd] = Range(dimension(0,kd), dimension(1,kd)-1);
                        j[kd] = i[kd] + 1;
                        cn(i1,i2,i3,d0,kd) = (Real).5 *
                          (cn(i1,i2,i3,d0,kd) + cn(j1,j2,j3,d0,kd));
                        i[kd] = j[kd] = d[kd];
                    } // end for
                    if (what & THEcenterNormal) {
                        centerNormal->periodicUpdate();
                        computedGeometry |= THEcenterNormal;
                    } // end if
                } // end if
                if (what & THEcenterArea) {
#ifdef DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                    RealMappedGridFunction ca; ca.reference(*centerArea);
                    ca.updateToMatchGrid(*this, all, all, all, one, d0);
#else
                    RealArray ca; ca.reference(*centerArea);
                    ca.reshape(d1, d2, d3, one, d0);
#endif // DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                    switch (numberOfDimensions) {
                      case 1:
                        ca = (Real)1.;
                      break;
                      case 2: {
                        ca = (Real)1.;
                        for (Integer kd1=0; kd1<numberOfDimensions; kd1++) {
                            Integer kd2=1-kd1;
                            i[kd2] = Range(dimension(0,kd2),dimension(1,kd2)-1);
                            ca(i1,i2,i3,0,kd1) = sqrt
                              ( cn(i1,i2,i3,0,kd1) * cn(i1,i2,i3,0,kd1)
                              + cn(i1,i2,i3,1,kd1) * cn(i1,i2,i3,1,kd1) );
                            i[kd2] = d[kd2];
                        } // end for
                      } break;
                      case 3: {
                        ca = (Real)1.;
                        for (Integer kd1=0; kd1<numberOfDimensions; kd1++) {
                            Integer kd2=(kd1+1)%3, kd3=(kd2+1)%3;
                            i[kd2] = Range(dimension(0,kd2),dimension(1,kd2)-1);
                            i[kd3] = Range(dimension(0,kd3),dimension(1,kd3)-1);
                            ca(i1,i2,i3,0,kd1) = sqrt
                              ( cn(i1,i2,i3,0,kd1) * cn(i1,i2,i3,0,kd1)
                              + cn(i1,i2,i3,1,kd1) * cn(i1,i2,i3,1,kd1)
                              + cn(i1,i2,i3,2,kd1) * cn(i1,i2,i3,2,kd1) );
                            i[kd2] = d[kd2]; i[kd3] = d[kd3];
                        } // end for
                      } break;
                    } // end switch
                    centerArea->periodicUpdate();
                    computedGeometry |= THEcenterArea;
                } // end if
                if (returnValue & COMPUTEfailed) cerr
                  << "MappedGridData::computeGeometry():  WARNING:  " << endl
                  << "Incorrect geometry was computed for centerNormal, "
                  << "centerArea, faceNormal and/or faceArea." << endl;
            } // end if
        } // end if

        if (what &
            THEvertexBoundaryNormal ||
          isAllVertexCentered && (what &
            THEcenterBoundaryNormal ) || (
          numberOfDimensions > 1 && what &
            THEcenterBoundaryTangent )) {
//
//          Compute geometry at the boundary vertices.
//
            Range d[3], &d1=d[0], &d2=d[1], &d3=d[2];
            Index i[3], &i1=i[0], &i2=i[1], &i3=i[2],
                  j[3], &j1=j[0], &j2=j[1], &j3=j[2],
                  k[3], &k1=k[0], &k2=k[1], &k3=k[2],
                  l[3], &l1=l[0], &l2=l[1], &l3=l[2],
                  m[3], &m1=m[0], &m2=m[1], &m3=m[2];
            Integer q[3], &q1=q[0], &q2=q[1], &q3=q[2];
            for (Integer kd1=0; kd1<numberOfDimensions; kd1++)
              for (Integer ks=0; ks<2; ks++) {
                for (Integer kd2=0; kd2<3; kd2++) {
                    if (kd2 == kd1) {
                        i[kd2] = j[kd2] = k[kd2] = l[kd2] = m[kd2] =
                          Range(gridIndexRange(ks,kd2), gridIndexRange(ks,kd2));
                        d[kd2] =
                          Range(gridIndexRange(ks,kd2)-1, gridIndexRange(ks,kd2)+1);
                    } else {
                        i[kd2] = j[kd2] = k[kd2] = l[kd2] = m[kd2] = d[kd2] =
                          Range(dimension(0,kd2), dimension(1,kd2));
                    } // end if
                    q[kd2] = (dimension(0,kd2) + dimension(1,kd2)) / 2;
                } // end for

                if (computedGeometry & THEvertex) {
                    x.reference(*vertex);
                } else if (isAllVertexCentered &&
                  computedGeometry & THEcenter) {
                    x.reference(*center);
                } else if (isAllCellCentered &&
                  computedGeometry & THEcorner) {
                    x.reference(*corner);
                } else {
//                  Compute geometry at the boundary vertices.
                    x.partition(partition); x.redim(d1,d2,d3,d0);
                    RealArray xL;
                    xL.reference(x.getLocalArrayWithGhostBoundaries());
                    Range dL[3], &d1L=dL[0], &d2L=dL[1], &d3L=dL[2];
                    Index iL[3], &i1L=iL[0], &i2L=iL[1], &i3L=iL[2];
                    IntegerArray dimensionL(2,3);
                    Integer numberOfPoints = 1;
                    for (kd=0; kd<3; kd++) {
                        dimensionL(0,kd) = xL.getBase(kd),
                        dimensionL(1,kd) = xL.getBound(kd);
                        iL[kd] = dL[kd] =
                          Range(dimensionL(0,kd), dimensionL(1,kd));
                        numberOfPoints *=
                          dimensionL(1,kd) - dimensionL(0,kd) + 1;
                    } // end for
                    if (numberOfPoints) {
                        Range p = numberOfPoints;
                        RealArray r(d1L,d2L,d3L,d0);
                        for (kd2=0; kd2<numberOfDimensions; kd2++)
                          if (kd2 == kd1) {
                            for (Integer k=gridIndexRange(ks,kd2)-1;
                                    k<=gridIndexRange(ks,kd2)+1; k++) {
                                iL[kd2] = k;
                                r(i1L,i2L,i3L,kd2) = gridSpacing(kd2) *
                                  (k - indexRange(0,kd2));
                            } // end for
                            iL[kd2] = Range(gridIndexRange(ks,kd2),
                                            gridIndexRange(ks,kd2));
                        } else {
                            for (Integer k=dimensionL(0,kd2);
                              k<=dimensionL(1,kd2); k++) {
                                iL[kd2] = k;
                                r(i1L,i2L,i3L,kd2) = gridSpacing(kd2) *
                                  (k - indexRange(0,kd2));
                            } // end for
                            iL[kd2] = Range(dimensionL(0,kd2),
                                            dimensionL(1,kd2));
                        } // end if, end for
                        if (mapping.mapPointer == NULL) {
                            xL = r;
                            cerr << "MappedGridData::computeGeometry():  "
                                 << "WARNING:  The grid's mapping is missing."
                                 << endl
                                 << "The identity mapping is used in its "
                                 << "place.  The result is incorrect geometry."
                                 << endl
                                 << "Incorrect geometry was computed for "
                                 << "vertexBoundaryNormal and/or " << endl
                                 << "centerBoundaryNormal" << endl;
                            returnValue |= COMPUTEfailed;
                        } else if (mapping.getClassName() == "Mapping") {
                            xL = r;
                            cerr
                              << "MappedGridData::computeGeometry():  WARNING:"
                              << endl
                              << "The type of the grid's mapping is the base "
                              << "class \"Mapping\".  This is invalid."
                              << endl
                              << "The identity mapping is used in its place.  "
                              << "The result is incorrect geometry."
                              << endl
                              << "Incorrect geometry was computed for "
                              << "vertexBoundaryNormal and/or " << endl
                              << "centerBoundaryNormal" << endl;
                            returnValue |= COMPUTEfailed;
                        } else {
                            r.reshape(p,d0); xL.reshape(p,d0);
                            mapping.map(r, xL);
                        } // end if
                    } // end if
                } // end if

                if (what &
                    THEvertexBoundaryNormal ||
                  isAllVertexCentered && what &
                    THEcenterBoundaryNormal ) {
//                  Get the sign of the jacobian at the center of the grid.
                    Real jac = ks ? 1. : -1.;
                    switch (numberOfDimensions) {
                      case 1:
                        jac *= x(q1+1,0) - x(q1-1,0);
                      break;
                      case 2:
                        jac *= (x(q1+1,q2,q3,0) - x(q1-1,q2,q3,0)) *
                               (x(q1,q2+1,q3,1) - x(q1,q2-1,q3,1))
                             - (x(q1,q2+1,q3,0) - x(q1,q2-1,q3,0)) *
                               (x(q1+1,q2,q3,1) - x(q1-1,q2,q3,1));
                      break;
                      case 3:
                        jac *= ( (x(q1+1,q2,q3,0) - x(q1-1,q2,q3,0)) *
                                 (x(q1,q2+1,q3,1) - x(q1,q2-1,q3,1))
                               - (x(q1,q2+1,q3,0) - x(q1,q2-1,q3,0)) *
                                 (x(q1+1,q2,q3,1) - x(q1-1,q2,q3,1)) ) *
                                 (x(q1,q2,q3+1,2) - x(q1,q2,q3-1,2))
                             + ( (x(q1,q2+1,q3,0) - x(q1,q2-1,q3,0)) *
                                 (x(q1,q2,q3+1,1) - x(q1,q2,q3-1,1))
                               - (x(q1,q2,q3+1,0) - x(q1,q2,q3-1,0)) *
                                 (x(q1,q2+1,q3,1) - x(q1,q2-1,q3,1)) ) *
                                 (x(q1+1,q2,q3,2) - x(q1-1,q2,q3,2))
                             + ( (x(q1,q2,q3+1,0) - x(q1,q2,q3-1,0)) *
                                 (x(q1+1,q2,q3,1) - x(q1-1,q2,q3,1))
                               - (x(q1+1,q2,q3,0) - x(q1-1,q2,q3,0)) *
                                 (x(q1,q2,q3+1,1) - x(q1,q2,q3-1,1)) ) *
                                 (x(q1,q2+1,q3,2) - x(q1,q2-1,q3,2));
                      break;
                    } // end switch
                    jac = (jac < 0.) ? -1. : (jac > 0.) ? 1. : 0.;
    
                    RealDistributedArray fn, fnn; fn.partition(partition);
                    fnn.partition(partition); fnn.redim(i1,i2,i3);
                    if (what & THEvertexBoundaryNormal)
                         fn.reference(*vertexBoundaryNormal[kd1][ks]);
                    else fn.reference(*centerBoundaryNormal[kd1][ks]);
                    switch (numberOfDimensions) {
                      case 1:
                        fn = jac;
                      break;
                      case 2: {
                        fn = (Real)0.;
                        Integer kd2 = 1 - kd1, kd3;
                        i[kd2] = Range(dimension(0,kd2)+1, dimension(1,kd2)-1);
                        j[kd2] = i[kd2] + 1; k[kd2] = i[kd2] - 1;
                        for (kd3=0; kd3<numberOfDimensions; kd3++) {
                            Integer kd4=1-kd3;
                            fn(i1,i2,i3,kd3) =
                              (Real)((kd2 - kd1) * (kd4 - kd3)) *
                              (x(j1,j2,j3,kd4) - x(k1,k2,k3,kd4));
                        } // end for
                        fnn(i1,i2,i3) = jac / sqrt
                          ( fn(i1,i2,i3,0) * fn(i1,i2,i3,0)
                          + fn(i1,i2,i3,1) * fn(i1,i2,i3,1) );
                        for (kd3=0; kd3<numberOfDimensions; kd3++)
                          fn(i1,i2,i3,kd3) = fn(i1,i2,i3,kd3) * fnn(i1,i2,i3);
                        if (isPeriodic(kd2) != Mapping::notPeriodic) {
//                          Fix up for periodicity.
                            i[kd2] = Range(dimension(0,kd2), dimension(0,kd2));
                            j[kd2] = i[kd2] +
                              (indexRange(1,kd2) - indexRange(0,kd2));
                            fn(i1,i2,i3,d0) = fn(j1,j2,j3,d0);
                            i[kd2] = Range(dimension(1,kd2), dimension(1,kd2));
                            j[kd2] = i[kd2] -
                              (indexRange(1,kd2) - indexRange(0,kd2));
                            fn(i1,i2,i3,d0) = fn(j1,j2,j3,d0);
                        } // end if
                        i[kd2] = j[kd2] = k[kd2] =
                          Range(dimension(0,kd2), dimension(1,kd2));
                      } break;
                      case 3: {
                        fn = (Real)0.;
                        Integer kd2 = (kd1 + 1) % 3, kd3 = (kd2 + 1) % 3, kd4;
                        i[kd2] = Range(dimension(0,kd2)+1, dimension(1,kd2)-1);
                        i[kd3] = Range(dimension(0,kd3)+1, dimension(1,kd3)-1);
                        j[kd2] = i[kd2] + 1; k[kd2] = i[kd2] - 1;
                        l[kd3] = i[kd3] + 1; m[kd3] = i[kd3] - 1;
                        l[kd2] = m[kd2] = i[kd2]; j[kd3] = k[kd3] = i[kd3];
                        for (kd4=0; kd4<numberOfDimensions; kd4++) {
                            Integer kd5=(kd4+1)%3, kd6=(kd5+1)%3;
                            fn(i1,i2,i3,kd4) =
                              (x(j1,j2,j3,kd5) - x(k1,k2,k3,kd5)) *
                              (x(l1,l2,l3,kd6) - x(m1,m2,m3,kd6)) -
                              (x(l1,l2,l3,kd5) - x(m1,m2,m3,kd5)) *
                              (x(j1,j2,j3,kd6) - x(k1,k2,k3,kd6));
                        } // end for
                        fnn(i1,i2,i3) = jac / sqrt
                          ( fn(i1,i2,i3,0) * fn(i1,i2,i3,0)
                          + fn(i1,i2,i3,1) * fn(i1,i2,i3,1)
                          + fn(i1,i2,i3,2) * fn(i1,i2,i3,2) );
                        for (kd4=0; kd4<numberOfDimensions; kd4++)
                          fn(i1,i2,i3,kd4) *= fnn(i1,i2,i3);
                        i[kd2] = j[kd2] = k[kd2] = l[kd2] = m[kd2] =
                          Range(dimension(0,kd2), dimension(1,kd2));
                        i[kd3] = j[kd3] = k[kd3] = l[kd3] = m[kd3] =
                          Range(dimension(0,kd3), dimension(1,kd3));
                        if (isPeriodic(kd2) != Mapping::notPeriodic) {
//                          Fix up for periodicity.
                            i[kd2] = Range(dimension(0,kd2), dimension(0,kd2));
                            j[kd2] = i[kd2] +
                              (indexRange(1,kd2) - indexRange(0,kd2));
                            fn(i1,i2,i3,d0) = fn(j1,j2,j3,d0);
                            i[kd2] = Range(dimension(1,kd2), dimension(1,kd2));
                            j[kd2] = i[kd2] -
                              (indexRange(1,kd2) - indexRange(0,kd2));
                            fn(i1,i2,i3,d0) = fn(j1,j2,j3,d0);
                            i[kd2] = j[kd2] =
                              Range(dimension(0,kd2), dimension(1,kd2));
                        } // end if
                        if (isPeriodic(kd3) != Mapping::notPeriodic) {
//                          Fix up for periodicity.
                            i[kd3] = Range(dimension(0,kd3), dimension(0,kd3));
                            l[kd3] = i[kd3] +
                              (indexRange(1,kd3) - indexRange(0,kd3));
                            fn(i1,i2,i3,d0) = fn(l1,l2,l3,d0);
                            i[kd3] = Range(dimension(1,kd3), dimension(1,kd3));
                            l[kd3] = i[kd3] -
                              (indexRange(1,kd3) - indexRange(0,kd3));
                            fn(i1,i2,i3,d0) = fn(l1,l2,l3,d0);
                            i[kd3] = l[kd3] =
                              Range(dimension(0,kd3), dimension(1,kd3));
                        } // end if
                      } break;
                    } // end switch
                } // end if
                if (isAllVertexCentered && numberOfDimensions > 1 &&
                  what & THEcenterBoundaryTangent) {
                    const Range d0m = numberOfDimensions - 1;
#ifdef DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                    RealMappedGridFunction& fn = centerBoundaryTangent[kd1][ks];
                    RealDistributedArray fnn; fnn.partition(partition);
                    fnn.redim(i1,i2,i3);
                    fn = (Real)0.;
                    for (kd2=0; kd2<numberOfDimensions-1; kd2++) {
                        Integer kd3 = (kd1 + kd2 + 1) % numberOfDimensions;
                        i[kd3] = Range(dimension(0,kd3)+1, dimension(1,kd3)-1);
                        j[kd3] = i[kd3] + 1; k[kd3] = i[kd3] - 1;
                        fn(i1,i2,i3,d0,kd2) = x(j1,j2,j3,d0) - x(k1,k2,k3,d0);
                        i[kd3] = j[kd3] = k[kd3] =
                          Range(dimension(0,kd3), dimension(1,kd3));
                        switch (numberOfDimensions) {
                          case 2:
                            fnn(i1,i2,i3) = (Real)1. / sqrt
                              ( fn(i1,i2,i3,0,kd2) * fn(i1,i2,i3,0,kd2)
                              + fn(i1,i2,i3,1,kd2) * fn(i1,i2,i3,1,kd2) );
                          break;
                          case 3:
                            fnn(i1,i2,i3) = (Real)1. / sqrt
                              ( fn(i1,i2,i3,0,kd2) * fn(i1,i2,i3,0,kd2)
                              + fn(i1,i2,i3,1,kd2) * fn(i1,i2,i3,1,kd2)
                              + fn(i1,i2,i3,2,kd2) * fn(i1,i2,i3,2,kd2) );
                          break;
                        } // end switch
                        for (kd3=0; kd3<numberOfDimensions; kd3++)
                          fn(i1,i2,i3,kd3,kd2) *= fnn(i1,i2,i3);
                    } // end for
#else
                    RealDistributedArray fn, fnn; fnn.partition(partition);
                    fnn.redim(i1,i2,i3,one,d0m);
                    fn.reference(*centerBoundaryTangent[kd1][ks]);
                    fn.reshape(i1, i2, i3, d0, d0m); //***** Needed until MappedGridFunctions use higher-dimensional arrays.
                    fn = (Real)0.; Integer kd2;
                    for (kd2=0; kd2<numberOfDimensions-1; kd2++) {
                        Integer kd3 = (kd1 + kd2 + 1) % numberOfDimensions;
                        i[kd3] = Range(dimension(0,kd3)+1, dimension(1,kd3)-1);
                        j[kd3] = i[kd3] + 1; k[kd3] = i[kd3] - 1;
                        fn(i1,i2,i3,d0,kd2) = x(j1,j2,j3,d0) - x(k1,k2,k3,d0);
                        i[kd3] = j[kd3] = k[kd3] =
                          Range(dimension(0,kd3), dimension(1,kd3));
                    } // end for
                    switch (numberOfDimensions) {
                      case 2:
                        fnn(i1,i2,i3,0,d0m) = (Real)1. / sqrt
                          ( fn(i1,i2,i3,0,d0m) * fn(i1,i2,i3,0,d0m)
                          + fn(i1,i2,i3,1,d0m) * fn(i1,i2,i3,1,d0m) );
                      break;
                      case 3:
                        fnn(i1,i2,i3,0,d0m) = (Real)1. / sqrt
                          ( fn(i1,i2,i3,0,d0m) * fn(i1,i2,i3,0,d0m)
                          + fn(i1,i2,i3,1,d0m) * fn(i1,i2,i3,1,d0m)
                          + fn(i1,i2,i3,2,d0m) * fn(i1,i2,i3,2,d0m) );
                      break;
                    } // end switch
                    for (kd2=0; kd2<numberOfDimensions; kd2++)
                      fn(i1,i2,i3,kd2,d0m) *= fnn(i1,i2,i3,0,d0m);
#endif // DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                    centerBoundaryTangent[kd1][ks]->periodicUpdate();
                } // end if
            } // end for, end for
            if (what & THEvertexBoundaryNormal)
              computedGeometry |= THEvertexBoundaryNormal;
            if (isAllVertexCentered && what & THEcenterBoundaryNormal)
              computedGeometry |= THEcenterBoundaryNormal;
            if (isAllVertexCentered && numberOfDimensions > 1 &&
              what & THEcenterBoundaryTangent)
              computedGeometry |= THEcenterBoundaryTangent;
        } // end if

        if (!isAllVertexCentered && (what &
            THEcenterBoundaryNormal || (
          numberOfDimensions > 1 && what &
            THEcenterBoundaryTangent ))) {
//
//          Compute geometry at the boundary corners.
//
            Range d[3], &d1=d[0], &d2=d[1], &d3=d[2];
            Index i[3], &i1=i[0], &i2=i[1], &i3=i[2],
                  j[3], &j1=j[0], &j2=j[1], &j3=j[2],
                  k[3], &k1=k[0], &k2=k[1], &k3=k[2],
                  l[3], &l1=l[0], &l2=l[1], &l3=l[2],
                  m[3], &m1=m[0], &m2=m[1], &m3=m[2];
            Integer q[3], &q1=q[0], &q2=q[1], &q3=q[2];
            for (Integer kd1=0; kd1<numberOfDimensions; kd1++)
              for (Integer ks=0; ks<2; ks++) {
                for (Integer kd2=0; kd2<3; kd2++) {
                    if (kd2 == kd1) {
                        i[kd2] = j[kd2] = k[kd2] = l[kd2] = m[kd2] =
                          Range(gridIndexRange(ks,kd2), gridIndexRange(ks,kd2));
                        d[kd2] =
                          Range(gridIndexRange(ks,kd2)-1, gridIndexRange(ks,kd2)+1);
                    } else {
                        i[kd2] = j[kd2] = k[kd2] = l[kd2] = m[kd2] = d[kd2] =
                          Range(dimension(0,kd2), dimension(1,kd2));
                    } // end if
                    q[kd2] = (dimension(0,kd2) + dimension(1,kd2)) / 2;
                } // end for

                if (isAllCellCentered && computedGeometry & THEcorner) {
                    x.reference(*corner);
                } else if (isAllCellCentered && computedGeometry & THEvertex) {
                    x.reference(*vertex);
                } else {
//                  Compute geometry at the boundary corners.
                    x.partition(partition); x.redim(d1,d2,d3,d0);
                    RealArray xL;
                    xL.reference(x.getLocalArrayWithGhostBoundaries());
                    Range dL[3], &d1L=dL[0], &d2L=dL[1], &d3L=dL[2];
                    Index iL[3], &i1L=iL[0], &i2L=iL[1], &i3L=iL[2];
                    IntegerArray dimensionL(2,3);
                    Integer numberOfPoints = 1;
                    for (Integer kd=0; kd<3; kd++) {
                        dimensionL(0,kd) = xL.getBase(kd),
                        dimensionL(1,kd) = xL.getBound(kd);
                        iL[kd] = dL[kd] =
                          Range(dimensionL(0,kd), dimensionL(1,kd));
                        numberOfPoints *=
                          dimensionL(1,kd) - dimensionL(0,kd) + 1;
                    } // end for
                    if (numberOfPoints) {
                        Range p = numberOfPoints;
                        RealArray r(d1L,d2L,d3L,d0);
                        for (Integer kd2=0; kd2<numberOfDimensions; kd2++)
                          if (kd2 == kd1) {
                            for (Integer k=gridIndexRange(ks,kd2)-1;
                                    k<=gridIndexRange(ks,kd2)+1; k++) {
                                iL[kd2] = k;
                                r(i1L,i2L,i3L,kd2) = gridSpacing(kd2) *
                                  (k - indexRange(0,kd2));
                            } // end for
                            iL[kd2] = Range(gridIndexRange(ks,kd2),
                                            gridIndexRange(ks,kd2));
                        } else {
                            for (Integer k=dimensionL(0,kd2);
                              k<=dimensionL(1,kd2);
                                 k++) {
                                iL[kd2] = k;
                                r(i1L,i2L,i3L,kd2) = gridSpacing(kd2) *
                                  ( k - indexRange(0,kd2)
                                  - (isCellCentered(kd2) ? (Real)0. : (Real).5) );
                            } // end for
                            iL[kd2] = Range(dimensionL(0,kd2),
                                            dimensionL(1,kd2));
                        } // end if, end for
                        if (mapping.mapPointer == NULL) {
                            xL = r;
                            cerr << "MappedGridData::computeGeometry():  "
                                 << "WARNING:  The grid's mapping is missing."
                                 << endl
                                 << "The identity mapping is used in its "
                                 << "place.  The result is incorrect geometry."
                                 << endl
                                 << "Incorrect geometry was computed for "
                                 << "vertexBoundaryNormal and/or " << endl
                                 << "centerBoundaryNormal" << endl;
                            returnValue |= COMPUTEfailed;
                        } else if (mapping.getClassName() == "Mapping") {
                            xL = r;
                            cerr
                              << "MappedGridData::computeGeometry():  WARNING:"
                              << endl
                              << "The type of the grid's mapping is the base "
                              << "class \"Mapping\".  This is invalid."
                              << endl
                              << "The identity mapping is used in its place.  "
                              << "The result is incorrect geometry."
                              << endl
                              << "Incorrect geometry was computed for "
                              << "vertexBoundaryNormal and/or " << endl
                              << "centerBoundaryNormal" << endl;
                            returnValue |= COMPUTEfailed;
                        } else {
                            r.reshape(p,d0); xL.reshape(p,d0);
                            mapping.map(r, xL);
                        } // end if
                    } // end if
                } // end if

                if (what & THEcenterBoundaryNormal) {
//                  Get the sign of the jacobian at the center of the grid.
                    Real jac = ks ? 1. : -1.;
                    switch (numberOfDimensions) {
                      case 1:
                        jac *= x(q1+1,0) - x(q1-1,0);
                      break;
                      case 2:
                        jac *= (x(q1+1,q2,q3,0) - x(q1-1,q2,q3,0)) *
                               (x(q1,q2+1,q3,1) - x(q1,q2-1,q3,1))
                             - (x(q1,q2+1,q3,0) - x(q1,q2-1,q3,0)) *
                               (x(q1+1,q2,q3,1) - x(q1-1,q2,q3,1));
                      break;
                      case 3:
                        jac *= ( (x(q1+1,q2,q3,0) - x(q1-1,q2,q3,0)) *
                                 (x(q1,q2+1,q3,1) - x(q1,q2-1,q3,1))
                               - (x(q1,q2+1,q3,0) - x(q1,q2-1,q3,0)) *
                                 (x(q1+1,q2,q3,1) - x(q1-1,q2,q3,1)) ) *
                                 (x(q1,q2,q3+1,2) - x(q1,q2,q3-1,2))
                             + ( (x(q1,q2+1,q3,0) - x(q1,q2-1,q3,0)) *
                                 (x(q1,q2,q3+1,1) - x(q1,q2,q3-1,1))
                               - (x(q1,q2,q3+1,0) - x(q1,q2,q3-1,0)) *
                                 (x(q1,q2+1,q3,1) - x(q1,q2-1,q3,1)) ) *
                                 (x(q1+1,q2,q3,2) - x(q1-1,q2,q3,2))
                             + ( (x(q1,q2,q3+1,0) - x(q1,q2,q3-1,0)) *
                                 (x(q1+1,q2,q3,1) - x(q1-1,q2,q3,1))
                               - (x(q1+1,q2,q3,0) - x(q1-1,q2,q3,0)) *
                                 (x(q1,q2,q3+1,1) - x(q1,q2,q3-1,1)) ) *
                                 (x(q1,q2+1,q3,2) - x(q1,q2-1,q3,2));
                      break;
                    } // end switch
                    jac = (jac < 0.) ? -1. : (jac > 0.) ? 1. : 0.;
    
                    RealDistributedArray fn, fnn; fn.partition(partition);
                    fnn.partition(partition); fnn.redim(i1,i2,i3);
                    fn.reference(*centerBoundaryNormal[kd1][ks]);
                    switch (numberOfDimensions) {
                      case 1:
                        fn = jac;
                      break;
                      case 2: {
                        fn = (Real)0.;
                        Integer kd2=1-kd1;
                        i[kd2] = Range(dimension(0,kd2), dimension(1,kd2)-1);
                        j[kd2] = i[kd2] + 1;
                        for (Integer kd3=0; kd3<numberOfDimensions; kd3++) {
                            Integer kd4=1-kd3;
                            fn(i1,i2,i3,kd3) =
                              (Real)((kd2 - kd1) * (kd4 - kd3)) *
                              (x(j1,j2,j3,kd4) - x(i1,i2,i3,kd4));
                        } // end for
                        fnn(i1,i2,i3) = jac / sqrt
                          ( fn(i1,i2,i3,0) * fn(i1,i2,i3,0)
                          + fn(i1,i2,i3,1) * fn(i1,i2,i3,1) );
                        for (kd3=0; kd3<numberOfDimensions; kd3++)
                          fn(i1,i2,i3,kd3) = fn(i1,i2,i3,kd3) * fnn(i1,i2,i3);
                        if (isPeriodic(kd2) != Mapping::notPeriodic) {
//                          Fix up for periodicity.
                            i[kd2] = Range(dimension(1,kd2), dimension(1,kd2));
                            j[kd2] = i[kd2] -
                              (indexRange(1,kd2) - indexRange(0,kd2));
                            fn(i1,i2,i3,d0) = fn(j1,j2,j3,d0);
                        } // end if
                        i[kd2] = j[kd2] =
                          Range(dimension(0,kd2), dimension(1,kd2));
                      } break;
                      case 3: {
                        fn = (Real)0.;
                        Integer kd2=(kd1+1)%3, kd3=(kd2+1)%3;
                        i[kd2] = Range(dimension(0,kd2), dimension(1,kd2)-1);
                        i[kd3] = Range(dimension(0,kd3), dimension(1,kd3)-1);
                        k[kd2] = i[kd2]; j[kd2] = l[kd2] = i[kd2] + 1;
                        j[kd3] = i[kd3]; k[kd3] = l[kd3] = i[kd3] + 1;
                        for (Integer kd4=0; kd4<numberOfDimensions; kd4++) {
                            Integer kd5=(kd4+1)%3, kd6=(kd5+1)%3;
                            fn(i1,i2,i3,kd4) = (Real).5 *
                              (x(l1,l2,l3,kd5) - x(i1,i2,i3,kd5)) *
                              (x(k1,k2,k3,kd6) - x(j1,j2,j3,kd6)) -
                              (x(l1,l2,l3,kd6) - x(i1,i2,i3,kd6)) *
                              (x(k1,k2,k3,kd5) - x(j1,j2,j3,kd5));
                        } // end for
                        fnn(i1,i2,i3) = jac / sqrt
                          ( fn(i1,i2,i3,0) * fn(i1,i2,i3,0)
                          + fn(i1,i2,i3,1) * fn(i1,i2,i3,1)
                          + fn(i1,i2,i3,2) * fn(i1,i2,i3,2) );
                        for (kd4=0; kd4<numberOfDimensions; kd4++)
                          fn(i1,i2,i3,kd4) *= fnn(i1,i2,i3);
                        i[kd2] = j[kd2] = k[kd2] = l[kd2] =
                          Range(dimension(0,kd2), dimension(1,kd2));
                        i[kd3] = j[kd3] = k[kd3] = l[kd3] =
                          Range(dimension(0,kd3), dimension(1,kd3));
                        if (isPeriodic(kd2) != Mapping::notPeriodic) {
//                          Fix up for periodicity.
                            i[kd2] = Range(dimension(1,kd2), dimension(1,kd2));
                            j[kd2] = i[kd2] -
                              (indexRange(1,kd2) - indexRange(0,kd2));
                            fn(i1,i2,i3,d0) = fn(j1,j2,j3,d0);
                            i[kd2] = j[kd2] =
                              Range(dimension(0,kd2), dimension(1,kd2));
                        } // end if
                        if (isPeriodic(kd3) != Mapping::notPeriodic) {
//                          Fix up for periodicity.
                            i[kd3] = Range(dimension(1,kd3), dimension(1,kd3));
                            k[kd3] = i[kd3] -
                              (indexRange(1,kd3) - indexRange(0,kd3));
                            fn(i1,i2,i3,d0) = fn(k1,k2,k3,d0);
                            i[kd3] = k[kd3] =
                              Range(dimension(0,kd3), dimension(1,kd3));
                        } // end if
                      } break;
                    } // end switch
                } // end if
                if (numberOfDimensions > 1 &&
                  what & THEcenterBoundaryTangent) {
                    const Range d0m = numberOfDimensions - 1; Integer kd2;
#ifdef DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                    RealMappedGridFunction& fn = centerBoundaryTangent[kd1][ks];
                    RealDistributedArray fnn; fnn.partition(partition);
                    fnn.redim(i1,i2,i3);
                    fn = (Real)0.;
                    for (kd2=0; kd2<numberOfDimensions-1; kd2++) {
                        Integer kd3 = (kd1 + kd2 + 1) % numberOfDimensions;
                        i[kd3] = Range(dimension(0,kd3), dimension(1,kd3)-1);
                        j[kd3] = i[kd3] + 1;
                        fn(i1,i2,i3,d0,kd2) = x(j1,j2,j3,d0) - x(i1,i2,i3,d0);
                        i[kd3] = j[kd3] =
                          Range(dimension(0,kd3), dimension(1,kd3));
                        switch (numberOfDimensions) {
                          case 2:
                            fnn(i1,i2,i3) = (Real)1. / sqrt
                              ( fn(i1,i2,i3,0,kd2) * fn(i1,i2,i3,0,kd2)
                              + fn(i1,i2,i3,1,kd2) * fn(i1,i2,i3,1,kd2) );
                          break;
                          case 3: {
                            Integer kd4 = (kd3 + 1) % numberOfDimensions;
                            i[kd4] = Range(dimension(0,kd4), dimension(1,kd4)-1);
                            j[kd4] = i[kd4] + 1;
                            fn(i1,i2,i3,d0,kd2) =
                              fn(i1,i2,i3,d0,kd2) + fn(j1,j2,j3,d0,kd2);
                            i[kd3] = j[kd3] =
                              Range(dimension(0,kd3), dimension(1,kd3));
                            fnn(i1,i2,i3) = (Real)1. / sqrt
                              ( fn(i1,i2,i3,0,kd2) * fn(i1,i2,i3,0,kd2)
                              + fn(i1,i2,i3,1,kd2) * fn(i1,i2,i3,1,kd2)
                              + fn(i1,i2,i3,2,kd2) * fn(i1,i2,i3,2,kd2) );
                          } break;
                        } // end switch
                        for (kd3=0; kd3<numberOfDimensions; kd3++)
                          fn(i1,i2,i3,kd3,kd2) *= fnn(i1,i2,i3);
                    } // end for
#else
                    RealDistributedArray fn, fnn; fnn.partition(partition);
                    fnn.redim(i1,i2,i3,one,d0m);
                    fn.reference(*centerBoundaryTangent[kd1][ks]);
                    fn.reshape(i1, i2, i3, d0, d0m); //***** Needed until MappedGridFunctions use higher-dimensional arrays.
                    fn = (Real)0.;
                    for (kd2=0; kd2<numberOfDimensions-1; kd2++) {
                        Integer kd3 = (kd1 + kd2 + 1) % numberOfDimensions;
                        i[kd3] = Range(dimension(0,kd3), dimension(1,kd3)-1);
                        j[kd3] = i[kd3] + 1;
                        fn(i1,i2,i3,d0,kd2) = x(j1,j2,j3,d0) - x(i1,i2,i3,d0);
                        i[kd3] = j[kd3] =
                          Range(dimension(0,kd3), dimension(1,kd3));
                        if (numberOfDimensions == 3) {
                            Integer kd4 = (kd3 + 1) % numberOfDimensions;
                            i[kd4] = Range(dimension(0,kd4), dimension(1,kd4)-1);
                            j[kd4] = i[kd4] + 1;
                            fn(i1,i2,i3,d0,kd2) =
                              fn(i1,i2,i3,d0,kd2) + fn(j1,j2,j3,d0,kd2);
                            i[kd3] = j[kd3] =
                              Range(dimension(0,kd3), dimension(1,kd3));
                        } // end if
                    } // end for
                    switch (numberOfDimensions) {
                      case 2:
                        fnn(i1,i2,i3,0,d0m) = (Real)1. / sqrt
                          ( fn(i1,i2,i3,0,d0m) * fn(i1,i2,i3,0,d0m)
                          + fn(i1,i2,i3,1,d0m) * fn(i1,i2,i3,1,d0m) );
                      break;
                      case 3:
                        fnn(i1,i2,i3,0,d0m) = (Real)1. / sqrt
                          ( fn(i1,i2,i3,0,d0m) * fn(i1,i2,i3,0,d0m)
                          + fn(i1,i2,i3,1,d0m) * fn(i1,i2,i3,1,d0m)
                          + fn(i1,i2,i3,2,d0m) * fn(i1,i2,i3,2,d0m) );
                      break;
                    } // end switch
                    for (kd2=0; kd2<numberOfDimensions; kd2++)
                      fn(i1,i2,i3,kd2,d0m) *= fnn(i1,i2,i3,0,d0m);
#endif // DO_NOT_USE_HIGHER_DIMENSIONAL_ARRAYS
                    centerBoundaryTangent[kd1][ks]->periodicUpdate();
                } // end if
            } // end for, end for
            if (what & THEcenterBoundaryNormal)
              computedGeometry |= THEcenterBoundaryNormal;
            if (numberOfDimensions > 1 && what & THEcenterBoundaryTangent)
              computedGeometry |= THEcenterBoundaryTangent;
        } // end if

    } else {
//
//      Compute the all geometric quantities directly from the mapping.
//
        Range dD[3], &d1D=dD[0], &d2D=dD[1], &d3D=dD[2]; Integer kd;
        for (kd=0; kd<3; kd++)
          dD[kd] = Range(dimension(0,kd), dimension(1,kd));
        if (what & (
            THEvertex                  |
            THEvertexDerivative        |
            THEvertexJacobian          |
            THEinverseVertexDerivative |
            THEboundingBox             ) || (
          isAllVertexCentered && what & (
            THEcenter                  |
            THEcenterDerivative        |
            THEcenterJacobian          |
            THEinverseCenterDerivative |
            THEcenterNormal            |
            THEcenterArea              |
            THEcellVolume              )) || (
          isAllCellCentered && what &
            THEcorner                  )) {
//
//          Compute geometry at the vertices.
//
//          xD might be needed for the boundingBox.
            RealDistributedArray xD; RealArray x;
            if ((what | computedGeometry) &
                THEvertex) {
                xD.reference(*vertex);
            } else if (isAllVertexCentered && (what | computedGeometry) &
                THEcenter) {
                xD.reference(*center);
            } else if (isAllCellCentered && (what | computedGeometry) &
                THEcorner) {
                xD.reference(*corner);
            } else {
                xD.partition(partition);
                xD.redim(d1D,d2D,d3D,d0);
            } // end if
            x.reference(xD.getLocalArrayWithGhostBoundaries());

            Range d[3], &d1=d[0], &d2=d[1], &d3=d[2];
            Index i[3], &i1=i[0], &i2=i[1], &i3=i[2];
            IntegerArray dimensionL(2,3);
            Integer numberOfPoints = 1;
            for (kd=0; kd<3; kd++) {
                dimensionL(0,kd) = x.getBase(kd),
                dimensionL(1,kd) = x.getBound(kd);
                i[kd] = d[kd] = Range(dimensionL(0,kd), dimensionL(1,kd));
                numberOfPoints *= dimensionL(1,kd) - dimensionL(0,kd) + 1;
            } // end for
            Range p = numberOfPoints;
            if (numberOfPoints) x.reshape(p,d0);

            RealArray xr;
            if ((what | computedGeometry) &
                THEvertexDerivative) {
                xr.reference(vertexDerivative->
                  getLocalArrayWithGhostBoundaries());
                if (numberOfPoints) xr.reshape(p,d0,d0);
            } else if (isAllVertexCentered && (what | computedGeometry) &
                THEcenterDerivative) {
                xr.reference(centerDerivative->
                  getLocalArrayWithGhostBoundaries());
                if (numberOfPoints) xr.reshape(p,d0,d0);
            } else if (numberOfPoints) {
                xr.redim(p,d0,d0);
            } // end if

            if (((how & COMPUTEgeometry && what &
                 (THEvertex | THEvertexDerivative)) ||
                 !(computedGeometry & THEvertex) ||
                 !(computedGeometry & THEvertexDerivative)) ||
               (isAllVertexCentered &&
                 ((how & COMPUTEgeometry && what &
                 (THEcenter | THEcenterDerivative)) ||
                 !(computedGeometry & THEcenter) ||
                 !(computedGeometry & THEcenterDerivative))) ||
               (isAllCellCentered &&
                 ((how & COMPUTEgeometry && what & THEcorner) ||
                 !(computedGeometry & THEcorner)))) {
//              The vertex geometry either does not exist or must be recomputed.
                if (numberOfPoints) {
                    RealArray r(d1,d2,d3,d0);
                    for (kd=0; kd<numberOfDimensions; kd++) {
                        for (Integer k=dimensionL(0,kd);
                          k<=dimensionL(1,kd); k++) {
                            i[kd] = k;
                            r(i1,i2,i3,kd) = gridSpacing(kd) *
                              (k - indexRange(0,kd));
                        } // end for
                        i[kd] = d[kd];
                    } // end for
                    r.reshape(p,d0);
                    mapping.map(r, x, xr);
                } // end if
                if (what & THEvertex)
                  computedGeometry |= THEvertex;
                if (isAllVertexCentered && what & THEcenter)
                  computedGeometry |= THEcenter;
                if (isAllCellCentered && what & THEcorner)
                  computedGeometry |= THEcorner;
                if (what & THEvertexDerivative)
                  computedGeometry |= THEvertexDerivative;
                if (isAllVertexCentered && what & THEcenterDerivative)
                  computedGeometry |= THEcenterDerivative;
            } // end if

            RealArray vj; // This might be needed for inverseVertexDerivative.
            if (what & (
                THEvertexJacobian          |
                THEinverseVertexDerivative ) || (
              isAllVertexCentered && what & (
                THEcenterJacobian          |
                THEinverseCenterDerivative |
                THEcellVolume              ))) {
                if (numberOfPoints) {
                    if (what & THEvertexJacobian) {
                        vj.reference(vertexJacobian->
                          getLocalArrayWithGhostBoundaries());
                        vj.reshape(p);
                    } else if (isAllVertexCentered && what & THEcenterJacobian) {
                        vj.reference(centerJacobian->
                          getLocalArrayWithGhostBoundaries());
                        vj.reshape(p);
                    } else {
                        vj.redim(p);
                    } // end if
                    switch (numberOfDimensions) {
                      case 1:
                        vj = xr;
                      break;
                      case 2:
                        vj = xr(p,0,0) * xr(p,1,1) - xr(p,0,1) * xr(p,1,0);
                      break;
                      case 3:
                        vj =
                          (xr(p,0,0)*xr(p,1,1)-xr(p,0,1)*xr(p,1,0))*xr(p,2,2) +
                          (xr(p,0,1)*xr(p,1,2)-xr(p,0,2)*xr(p,1,1))*xr(p,2,0) +
                          (xr(p,0,2)*xr(p,1,0)-xr(p,0,0)*xr(p,1,2))*xr(p,2,1);
                      break;
                    } // end switch
                    if (isAllVertexCentered && what & THEcellVolume) {
                        RealArray cv;
                        cv.reference(
                          cellVolume->getLocalArrayWithGhostBoundaries());
                        cv.reshape(p);
                        cv = (gridSpacing(0) * gridSpacing(1) * gridSpacing(2))
                           * vj;
                        Real minCellVolume = min(*cellVolume);
                        if (minCellVolume == (Real)0.) cerr
                          << "MappedGridData::computeGeometry():  WARNING:  "
                          << "Computed a zero cellVolume." << endl;
                        else if (minCellVolume < (Real)0.) cerr
                          << "MappedGridData::computeGeometry():  WARNING:  "
                          << "Computed a negative cellVolume." << endl;
                    } // end if
                } // end if
                if (what & THEvertexJacobian)
                  computedGeometry |= THEvertexJacobian;
                if (isAllVertexCentered && what & THEcenterJacobian)
                  computedGeometry |= THEcenterJacobian;
                if (isAllVertexCentered && what & THEcellVolume)
                  computedGeometry |= THEcellVolume;
            } // end if

            RealArray vn; // This might be needed for inverseVertexDerivative.
            if (what &
                THEinverseVertexDerivative || (
              isAllVertexCentered && what & (
                THEinverseCenterDerivative |
                THEcenterNormal            |
                THEcenterArea              ))) {
                if (numberOfPoints) {
                    if (isAllVertexCentered && what & THEcenterNormal) {
                        vn.reference(centerNormal->
                          getLocalArrayWithGhostBoundaries());
                        vn.reshape(p,d0,d0);
                    } else {
                        vn.redim(p,d0,d0);
                    } // end if
                    switch (numberOfDimensions) {
                      case 1:
                        vn = (Real)1.;
                      break;
                      case 2: {
                        for (Integer i=0; i<numberOfDimensions; i++)
                          for (Integer l=0; l<numberOfDimensions; l++)
                            vn(p,l,i) = (gridSpacing(1-i) * (1-2*l) * (1-2*i)) *
                              xr(p,1-l,1-i);
                      } break;
                      case 3: {
                        for (Integer i=0; i<numberOfDimensions; i++)
                          for (Integer l=0; l<numberOfDimensions; l++) {
                            Integer j=(i+1)%3, k=(j+1)%3, m=(l+1)%3, n=(m+1)%3;
                            vn(p,l,i) = (gridSpacing(j) * gridSpacing(k)) *
                              (xr(p,m,j) * xr(p,n,k) - xr(p,m,k) * xr(p,n,j));
                        } // end for
                      } break;
                    } // end switch
                } // end if
                if (isAllVertexCentered && what & THEcenterNormal)
                  computedGeometry |= THEcenterNormal;
            } // end if

            if (isAllVertexCentered && what & THEcenterArea) {
                if (numberOfPoints) {
                    RealArray ca;
                    ca.reference(centerArea->getLocalArrayWithGhostBoundaries());
                    ca.reshape(p,one,d0);
                    switch (numberOfDimensions) {
                      case 1:
                        ca = (Real)1.;
                      break;
                      case 2: {
                        for (Integer l=0; l<numberOfDimensions; l++)
                          ca(p,0,l) = sqrt
                            (vn(p,0,l) * vn(p,0,l) + vn(p,1,l) * vn(p,1,l));
                      } break;
                      case 3: {
                        for (Integer l=0; l<numberOfDimensions; l++)
                          ca(p,0,l) = sqrt
                            ( vn(p,0,l) * vn(p,0,l)
                            + vn(p,1,l) * vn(p,1,l)
                            + vn(p,2,l) * vn(p,2,l) );
                      } break;
                    } // end switch
                } // end if
                computedGeometry |= THEcenterArea;
            } // end if

            if (what &
                THEinverseVertexDerivative || (
              isAllVertexCentered && what &
                THEinverseCenterDerivative )) {
                if (numberOfPoints) {
                    RealArray det(p), rx;
                    if (what & THEinverseVertexDerivative)
                         rx.reference(inverseVertexDerivative->
                           getLocalArrayWithGhostBoundaries());
                    else rx.reference(inverseCenterDerivative->
                           getLocalArrayWithGhostBoundaries());
                    rx.reshape(p,d0,d0);
                    where (vj == (Real)0.) det = (Real)1.;
                    otherwise() det = (Real)1. / vj;
                    switch (numberOfDimensions) {
                      case 1:
                        rx = det;
                      break;
                      case 2: {
                        for (Integer i=0; i<2; i++)
                          for (Integer l=0; l<2; l++)
                            rx(p,i,l) = ((Real)1. / gridSpacing(1-i)) *
                              vn(p,l,i) * det;
                      } break;
                      case 3: {
                        for (Integer i=0; i<numberOfDimensions; i++)
                          for (Integer l=0; l<numberOfDimensions; l++) {
                            Integer j=(i+1)%3, k=(j+1)%3;
                            rx(p,i,l) =
                              ((Real)1. / (gridSpacing(j) * gridSpacing(k))) *
                              vn(p,l,i) * det;
                        } // end for
                      } break;
                    } // end switch
                } // end if
                if (what & THEinverseVertexDerivative)
                  computedGeometry |= THEinverseVertexDerivative;
                if (isAllVertexCentered && what & THEinverseCenterDerivative)
                  computedGeometry |= THEinverseCenterDerivative;
            } // end if
            vn.redim(0); // Save some space.

//             if (what & THEminMaxEdgeLength) {
//                 Index iD[3], &i1D=iD[0], &i2D=iD[1], &i3D=iD[2],
//                       jD[3], &j1D=jD[0], &j2D=jD[1], &j3D=iD[2];
//                 for (kd=0; kd<3; kd++) iD[kd] = jD[kd] = dD[kd];
//                 RealDistributedArray y; y.partition(partition);
//                 y.redim(d1D,d2D,d3D,d0);
//                 for (kd=0; kd<3; kd++) if (kd < numberOfDimensions) {
//                     iD[kd] = Range(dimension(0,kd), dimension(1,kd)-1);
//                     jD[kd] = Range(dimension(0,kd)+1, dimension(1,kd));
//                     y(i1D,i2D,i3D,d0) = xD(j1D,j2D,j3D,d0) - xD(i1D,i2D,i3D,d0);
//                     y(i1D,i2D,i3D,d0) = y (i1D,i2D,i3D,d0) * y (i1D,i2D,i3D,d0);
//                     RealDistributedArray z; z.partition(partition);
//                     z.redim(i1D,i2D,i3D); z = (Real)0.;
//                     for (Integer kd1=0; kd1<numberOfDimensions; kd1++)
//                       z += y(i1D,i2D,i3D,kd1);
//                     minimumEdgeLength(kd) = Sqrt(min(z));
//                     maximumEdgeLength(kd) = Sqrt(max(z));
//                     iD[kd] = jD[kd] = dD[kd];
//                 } else {
//                     minimumEdgeLength(kd) = (Real)1.;
//                     maximumEdgeLength(kd) = (Real)1.;
//                 } // end if, end for
//                 computedGeometry |= THEminMaxEdgeLength;
//             } // end if

            if (what & THEboundingBox) {
                boundingBox = (Real)0.;
                Index iD[3], &i1D=iD[0], &i2D=iD[1], &i3D=iD[2];
                for (kd=0; kd<3; kd++)
                  iD[kd] = Range(gridIndexRange(0,kd), gridIndexRange(1,kd));
                for (kd=0; kd<numberOfDimensions; kd++) {
                    boundingBox(0,kd) = min(xD(i1D,i2D,i3D,kd));
                    boundingBox(1,kd) = max(xD(i1D,i2D,i3D,kd));
                } // end for
                for (kd=numberOfDimensions; kd<3; kd++)
                  boundingBox(0,kd) = boundingBox(1,kd) = (Real)0.;
                computedGeometry |= THEboundingBox;
            } // end if
        } // end if

        if (!isAllVertexCentered && what & (
          THEcenter                  |
          THEcenterDerivative        |
          THEcenterJacobian          |
          THEinverseCenterDerivative |
          THEcellVolume              |
          THEcenterNormal            |
          THEcenterArea              )) {
//
//          Compute geometry at the centers.
//
            RealDistributedArray xD; RealArray x;
            if ((what | computedGeometry) & THEcenter) {
                x.reference(center->getLocalArrayWithGhostBoundaries());
            } else {
                xD.partition(partition);
                xD.redim(d1D,d2D,d3D,d0);
                x.reference(xD.getLocalArrayWithGhostBoundaries());
            } // end if

            Range d[3], &d1=d[0], &d2=d[1], &d3=d[2];
            Index i[3], &i1=i[0], &i2=i[1], &i3=i[2];
            IntegerArray dimensionL(2,3);
            Integer numberOfPoints = 1;
            for (kd=0; kd<3; kd++) {
                dimensionL(0,kd) = x.getBase(kd),
                dimensionL(1,kd) = x.getBound(kd);
                i[kd] = d[kd] = Range(dimensionL(0,kd), dimensionL(1,kd));
                numberOfPoints *= dimensionL(1,kd) - dimensionL(0,kd) + 1;
            } // end for
            Range p = numberOfPoints;
            if (numberOfPoints) x.reshape(p,d0);

            RealArray xr;
            if ((what | computedGeometry) & THEcenterDerivative) {
                xr.reference(centerDerivative->
                  getLocalArrayWithGhostBoundaries());
                if (numberOfPoints) xr.reshape(p,d0,d0);
            } else if (numberOfPoints) {
                xr.redim(p,d0,d0);
            } // end if

            if ((how & COMPUTEgeometry && what &
                (THEcenter | THEcenterDerivative)) ||
                !(computedGeometry & THEcenter) ||
                !(computedGeometry & THEcenterDerivative)) {
//              The center geometry either does not exist or must be recomputed.
                if (numberOfPoints) {
                    RealArray r(d1,d2,d3,d0);
                    for (kd=0; kd<numberOfDimensions; kd++) {
                        for (Integer k=dimensionL(0,kd);
                          k<=dimensionL(1,kd); k++) {
                            i[kd] = k;
                            r(i1,i2,i3,kd) = gridSpacing(kd) *
                              ( k - indexRange(0,kd)
                              + (isCellCentered(kd) ? (Real).5 : (Real)0.) );
                        } // end for
                        i[kd] = d[kd];
                    } // end for
                    r.reshape(p,d0);
                    mapping.map(r, x, xr);
                } // end if
                if (what & THEcenter)
                  computedGeometry |= THEcenter;
                if (what & THEcenterDerivative)
                  computedGeometry |= THEcenterDerivative;
            } // end if

            RealArray cj; // This might be needed for inverseCenterDerivative.
            if (what & (
              THEcenterJacobian          |
              THEinverseCenterDerivative |
              THEcellVolume              )) {
                if (numberOfPoints) {
                    if (what & THEcenterJacobian) {
                        cj.reference(centerJacobian->
                          getLocalArrayWithGhostBoundaries());
                        cj.reshape(p);
                    } else {
                        cj.redim(p);
                    } // end if
                    switch (numberOfDimensions) {
                      case 1:
                        cj = xr;
                      break;
                      case 2:
                        cj = xr(p,0,0) * xr(p,1,1) - xr(p,0,1) * xr(p,1,0);
                      break;
                      case 3:
                        cj =
                          (xr(p,0,0)*xr(p,1,1)-xr(p,0,1)*xr(p,1,0))*xr(p,2,2) +
                          (xr(p,0,1)*xr(p,1,2)-xr(p,0,2)*xr(p,1,1))*xr(p,2,0) +
                          (xr(p,0,2)*xr(p,1,0)-xr(p,0,0)*xr(p,1,2))*xr(p,2,1);
                      break;
                    } // end switch
                    if (what & THEcellVolume) {
                        RealArray cv;
                        cv.reference(
                          cellVolume->getLocalArrayWithGhostBoundaries());
                        cv.reshape(p);
                        cv = (gridSpacing(0) * gridSpacing(1) * gridSpacing(2))
                           * cj;
                        Real minCellVolume = min(*cellVolume);
                        if (minCellVolume == (Real)0.) cerr
                          << "MappedGridData::computeGeometry():  WARNING:  "
                          << "Computed a zero cellVolume." << endl;
                        else if (minCellVolume < (Real)0.) cerr
                          << "MappedGridData::computeGeometry():  WARNING:  "
                          << "Computed a negative cellVolume." << endl;
                    } // end if
                } // end if
                if (what & THEcenterJacobian)
                  computedGeometry |= THEcenterJacobian;
                if (what & THEcellVolume)
                  computedGeometry |= THEcellVolume;
            } // end if

            RealArray cn; // This might be needed for inverseCenterDerivative.
            if (what & (
                THEinverseCenterDerivative |
                THEcenterNormal            |
                THEcenterArea              )) {
                if (numberOfPoints) {
                    if (what & THEcenterNormal) {
                        cn.reference(centerNormal->
                          getLocalArrayWithGhostBoundaries());
                        cn.reshape(p,d0,d0);
                    } else {
                        cn.redim(p,d0,d0);
                    } // end if
                    switch (numberOfDimensions) {
                      case 1:
                        cn = (Real)1.;
                      break;
                      case 2: {
                        for (Integer i=0; i<numberOfDimensions; i++)
                          for (Integer l=0; l<numberOfDimensions; l++)
                            cn(p,l,i) = (gridSpacing(1-i) * (1-2*l) * (1-2*i)) *
                              xr(p,1-l,1-i);
                      } break;
                      case 3: {
                        for (Integer i=0; i<numberOfDimensions; i++)
                          for (Integer l=0; l<numberOfDimensions; l++) {
                            Integer j=(i+1)%3, k=(j+1)%3, m=(l+1)%3, n=(m+1)%3;
                            cn(p,l,i) = (gridSpacing(j) * gridSpacing(k)) *
                              (xr(p,m,j) * xr(p,n,k) - xr(p,m,k) * xr(p,n,j));
                        } // end for
                      } break;
                    } // end switch
                } // end if
                if (what & THEcenterNormal) computedGeometry |= THEcenterNormal;
                if (what & THEcenterArea) {
                    if (numberOfPoints) {
                        RealArray ca;
                        ca.reference(centerArea->getLocalArrayWithGhostBoundaries());
                        ca.reshape(p,one,d0);
                        switch (numberOfDimensions) {
                          case 1:
                            ca = (Real)1.;
                          break;
                          case 2: {
                            for (Integer l=0; l<numberOfDimensions; l++)
                              ca(p,0,l) = sqrt
                                (cn(p,0,l) * cn(p,0,l) + cn(p,1,l) * cn(p,1,l));
                          } break;
                          case 3: {
                            for (Integer l=0; l<numberOfDimensions; l++)
                              ca(p,0,l) = sqrt
                                ( cn(p,0,l) * cn(p,0,l)
                                + cn(p,1,l) * cn(p,1,l)
                                + cn(p,2,l) * cn(p,2,l) );
                          } break;
                        } // end switch
                    } // end if
                    computedGeometry |= THEcenterArea;
                } // end if
            } // end if

            if (what & THEinverseCenterDerivative) {
                if (numberOfPoints) {
                    RealArray det(p), rx;
                    rx.reference(inverseCenterDerivative->
                      getLocalArrayWithGhostBoundaries());
                    rx.reshape(p,d0,d0);
                    where (cj == (Real)0.) det = (Real)1.;
                    otherwise() det = (Real)1. / cj;
                    switch (numberOfDimensions) {
                      case 1:
                        rx = det;
                      break;
                      case 2: {
                        for (Integer i=0; i<2; i++)
                          for (Integer l=0; l<2; l++)
                            rx(p,i,l) = ((Real)1. / gridSpacing(1-i)) *
                              cn(p,l,i) * det;
                      } break;
                      case 3: {
                        for (Integer i=0; i<numberOfDimensions; i++)
                          for (Integer l=0; l<numberOfDimensions; l++) {
                            Integer j=(i+1)%3, k=(j+1)%3;
                            rx(p,i,l) =
                              ((Real)1. / (gridSpacing(j) * gridSpacing(k))) *
                              cn(p,l,i) * det;
                        } // end for
                      } break;
                    } // end switch
                } // end if
                computedGeometry |= THEinverseCenterDerivative;
            } // end if
        } // end if

        if (!isAllCellCentered && what & THEcorner) {
//
//          Compute geometry at the corners.
//
            RealArray x;
            x.reference(corner->getLocalArrayWithGhostBoundaries());

            Range d[3], &d1=d[0], &d2=d[1], &d3=d[2];
            Index i[3], &i1=i[0], &i2=i[1], &i3=i[2];
            IntegerArray dimensionL(2,3);
            Integer numberOfPoints = 1;
            for (kd=0; kd<3; kd++) {
                dimensionL(0,kd) = x.getBase(kd),
                dimensionL(1,kd) = x.getBound(kd);
                i[kd] = d[kd] = Range(dimensionL(0,kd), dimensionL(1,kd));
                numberOfPoints *= dimensionL(1,kd) - dimensionL(0,kd) + 1;
            } // end for
            Range p = numberOfPoints;

            if (numberOfPoints) {
                x.reshape(p,d0);
                RealArray r(d1,d2,d3,d0);
                for (kd=0; kd<numberOfDimensions; kd++) {
                    for (Integer k=dimensionL(0,kd);
                      k<=dimensionL(1,kd); k++) {
                        i[kd] = k;
                        r(i1,i2,i3,kd) = gridSpacing(kd) *
                          ( k - indexRange(0,kd)
                          - (isCellCentered(kd) ? (Real)0. : (Real).5) );
                    } // end for
                    i[kd] = d[kd];
                } // end for
                r.reshape(p,d0);
                mapping.map(r, x);
            } // end if
            computedGeometry |= THEcorner;
        } // end if

        if (what & (
          THEfaceNormal |
          THEfaceArea   )) {
//
//          Compute geometry at the cell faces.
//
            RealDistributedArray xD; RealArray x;
            xD.partition(partition); xD.redim(d1D,d2D,d3D,d0);
            x.reference(xD.getLocalArrayWithGhostBoundaries());
            Range d[3], &d1=d[0], &d2=d[1], &d3=d[2];
            Index i[3], &i1=i[0], &i2=i[1], &i3=i[2];
            IntegerArray dimensionL(2,3);
            Integer numberOfPoints = 1;
            for (kd=0; kd<3; kd++) {
                dimensionL(0,kd) = x.getBase(kd),
                dimensionL(1,kd) = x.getBound(kd);
                i[kd] = d[kd] = Range(dimensionL(0,kd), dimensionL(1,kd));
                numberOfPoints *= dimensionL(1,kd) - dimensionL(0,kd) + 1;
            } // end for
            Range p = numberOfPoints;

            RealArray fn;
            if (what & THEfaceNormal) {
                fn.reference(faceNormal->getLocalArrayWithGhostBoundaries());
                fn.reshape(p,d0,d0);
            } else if (numberOfPoints) {
                fn.redim(p,d0,d0);
            } // end if

            if (numberOfPoints) for (kd=0; kd<numberOfDimensions; kd++) {
                RealArray r(d1,d2,d3,d0);
                for (Integer kd1=0; kd1<numberOfDimensions; kd1++) {
                    for (Integer k=dimensionL(0,kd1); k<=dimensionL(1,kd1); k++) {
                        i[kd1] = k;
                        r(i1,i2,i3,kd1) = gridSpacing(kd1) *
                          ( k - indexRange(0,kd1)
                          + (kd1 == kd           ? (Real)0. : (Real).5)
                          - (isCellCentered(kd1) ? (Real)0. : (Real).5) );
                    } // end for
                    i[kd1] = d[kd1];
                } // end for
                r.reshape(p,d0); x.reshape(p,d0); RealArray xr(p,d0,d0);
                mapping.map(r, x, xr);

                switch (numberOfDimensions) {
                  case 1:
                    fn = (Real)1.;
                  break;
                  case 2: {
                    for (Integer l=0; l<numberOfDimensions; l++)
                      fn(p,l,kd) = (gridSpacing(1-kd) * (1-2*l) * (1-2*kd)) *
                        xr(p,1-l,1-kd);
                  } break;
                  case 3: {
                    for (Integer l=0; l<numberOfDimensions; l++) {
                        Integer j=(kd+1)%3, k=(j+1)%3, m=(l+1)%3, n=(m+1)%3;
                        fn(p,l,kd) = (gridSpacing(j) * gridSpacing(k)) *
                          (xr(p,m,j) * xr(p,n,k) - xr(p,m,k) * xr(p,n,j));
                    } // end for
                  } break;
                } // end switch
            } // end for, end if
            if (what & THEfaceNormal) computedGeometry |= THEfaceNormal;
            xD.redim(0); x.redim(0); // Save some space.

            if (what & THEfaceArea) {
                if (numberOfPoints) {
                    RealArray fa;
                    fa.reference(faceArea->getLocalArrayWithGhostBoundaries());
                    fa.reshape(p,one,d0);
                    switch (numberOfDimensions) {
                      case 1:
                        fa = (Real)1.;
                      break;
                      case 2: {
                        for (Integer l=0; l<numberOfDimensions; l++)
                          fa(p,0,l) = sqrt
                            (fn(p,0,l) * fn(p,0,l) + fn(p,1,l) * fn(p,1,l));
                      } break;
                      case 3: {
                        for (Integer l=0; l<numberOfDimensions; l++)
                          fa(p,0,l) = sqrt
                            ( fn(p,0,l) * fn(p,0,l)
                            + fn(p,1,l) * fn(p,1,l)
                            + fn(p,2,l) * fn(p,2,l) );
                      } break;
                    } // end switch
                } // end if
                computedGeometry |= THEfaceArea;
            } // end if
        } // end if

        if (what &
            THEvertexBoundaryNormal || (
          isAllVertexCentered && (what &
            THEcenterBoundaryNormal || (
          numberOfDimensions > 1 && what &
            THEcenterBoundaryTangent)))) {
//
//          Compute geometry at the boundary vertices.
//
            Range d[3], &d1 = d[0], &d2 = d[1], &d3 = d[2];
            Index i[3], &i1 = i[0], &i2 = i[1], &i3 = i[2];
            for (kd=0; kd<numberOfDimensions; kd++)
              for (Integer ks=0; ks<2; ks++) {
                IntegerArray dimensionL(2,3);
                RealArray fn;
                if (what & THEvertexBoundaryNormal)
                     fn.reference(vertexBoundaryNormal[kd][ks]->
                       getLocalArrayWithGhostBoundaries());
                else if (what & THEcenterBoundaryNormal)
                     fn.reference(centerBoundaryNormal[kd][ks]->
                       getLocalArrayWithGhostBoundaries());
                else fn.reference(centerBoundaryTangent[kd][ks]->
                       getLocalArrayWithGhostBoundaries());
                Integer numberOfPoints = 1;
                for (Integer kd1=0; kd1<3; kd1++) {
                    dimensionL(0,kd1) = fn.getBase(kd1);
                    dimensionL(1,kd1) = fn.getBound(kd1);
                    i[kd1] = d[kd1] = 
                      Range(dimensionL(0,kd1), dimensionL(1,kd1));
                    numberOfPoints *=
                      dimensionL(1,kd1) - dimensionL(0,kd1) + 1;
                } // end for
                if (numberOfPoints) {
                    Range p = numberOfPoints;
                    RealArray r(d1,d2,d3,d0);
                    for (Integer kd1=0; kd1<numberOfDimensions; kd1++)
                      if (kd1==kd) {
                        r(i1,i2,i3,kd1) = (Real)ks;
                    } else {
                        for (Integer k=dimensionL(0,kd1);
                          k<=dimensionL(1,kd1); k++) {
                            i[kd1] = k;
                            r(i1,i2,i3,kd1) = gridSpacing(kd1) *
                              (k - indexRange(0,kd1));
                        } // end for
                        i[kd1] = d[kd1];
                    } // end if, end for
                    r.reshape(p,d0); RealArray x(p,d0), xr(p,d0,d0);
                    mapping.map(r, x, xr);
                    r.redim(0); x.redim(0);

                    if (what &
                        THEvertexBoundaryNormal ||
                      (isAllVertexCentered && what &
                        THEcenterBoundaryNormal)) {
//                      Get the sign of the jacobian at the center of the grid.
                        Integer q = numberOfPoints / 2; Real jac = ks ? 1. : -1.;
                        switch (numberOfDimensions) {
                          case 1:
                            jac *= xr(q,0,0);
                          break;
                          case 2:
                            jac *= xr(q,0,0) * xr(q,1,1) - xr(q,0,1) * xr(q,1,0);
                          break;
                          case 3:
                            jac *=
                              (xr(q,0,0)*xr(q,1,1)-xr(q,0,1)*xr(q,1,0))*xr(q,2,2) +
                              (xr(q,0,1)*xr(q,1,2)-xr(q,0,2)*xr(q,1,1))*xr(q,2,0) +
                              (xr(q,0,2)*xr(q,1,0)-xr(q,0,0)*xr(q,1,2))*xr(q,2,1);
                          break;
                        } // end switch
                        jac = (jac < 0.) ? -1. : (jac > 0.) ? 1. : 0.;
    
                        RealArray fnn(p); fn.reshape(p,d0);
                        switch (numberOfDimensions) {
                          case 1:
                            fn = jac;
                          break;
                          case 2: {
                            for (Integer l=0; l<numberOfDimensions; l++)
                              fn(p,l) = (Real)((1-2*l)*(1-2*kd)) * xr(p,1-l,1-kd);
                            fnn(p) = jac / sqrt
                              (fn(p,0) * fn(p,0) + fn(p,1) * fn(p,1));
                            for (l=0; l<numberOfDimensions; l++) fn(p,l) *= fnn(p);
                          } break;
                          case 3: {
                            for (Integer l=0; l<numberOfDimensions; l++) {
                                Integer j=(kd+1)%3, k=(j+1)%3, m=(l+1)%3, n=(m+1)%3;
                                fn(p,l) = xr(p,m,j)*xr(p,n,k) - xr(p,m,k)*xr(p,n,j);
                            } // end for
                            fnn(p) = jac / sqrt(fn(p,0) * fn(p,0) +
                              fn(p,1) * fn(p,1) + fn(p,2) * fn(p,2));
                            for (l=0; l<numberOfDimensions; l++) fn(p,l) *= fnn(p);
                          } break;
                        } // end switch
                    } //end if
                    if (numberOfDimensions > 1 &&
                      what & THEcenterBoundaryTangent) {
                        fn.reference(centerBoundaryTangent[kd][ks]->
                          getLocalArrayWithGhostBoundaries());
                        const Range d0m = numberOfDimensions - 1; Integer kd1;
                        RealArray fnn(p,one,d0m); fn.reshape(p,d0,d0m);
                        for (kd1=0; kd1<numberOfDimensions-1; kd1++) {
                            Integer kd2 = (kd + kd1 + 1) % numberOfDimensions;
                            fn(p,d0,kd1) = xr(p,d0,kd2);
                        } // end for
                        switch (numberOfDimensions) {
                          case 2:
                            fnn(p,0,d0m) = (Real)1. / sqrt
                              ( fn(p,0,d0m) * fn(p,0,d0m) +
                                fn(p,1,d0m) * fn(p,1,d0m) );
                          break;
                          case 3:
                            fnn(p,0,d0m) = (Real)1. / sqrt
                              ( fn(p,0,d0m) * fn(p,0,d0m) +
                                fn(p,1,d0m) * fn(p,1,d0m) +
                                fn(p,2,d0m) * fn(p,2,d0m) );
                          break;
                        } // end switch
                        for (kd1=0; kd1<numberOfDimensions; kd1++)
                          fn(p,kd1,d0m) *= fnn(p,0,d0m);
                    } // end if
                } // end if
            } // end for, end for
            if (what & THEvertexBoundaryNormal)
              computedGeometry |= THEvertexBoundaryNormal;
            if (isAllVertexCentered && what & THEcenterBoundaryNormal)
              computedGeometry |= THEcenterBoundaryNormal;
            if (isAllVertexCentered && numberOfDimensions > 1 &&
              what & THEcenterBoundaryTangent)
              computedGeometry |= THEcenterBoundaryTangent;
        } // end if

        if (!isAllVertexCentered && (what &
            THEcenterBoundaryNormal || (
          numberOfDimensions > 1 && what &
            THEcenterBoundaryTangent))) {
//
//          Compute geometry at the boundary centers.
//
            Range d[3], &d1 = d[0], &d2 = d[1], &d3 = d[2];
            Index i[3], &i1 = i[0], &i2 = i[1], &i3 = i[2];
            for (kd=0; kd<numberOfDimensions; kd++)
              for (Integer ks=0; ks<2; ks++) {
                IntegerArray dimensionL(2,3);
                RealArray fn;
                if (what & THEcenterBoundaryNormal)
                     fn.reference(centerBoundaryNormal[kd][ks]->
                       getLocalArrayWithGhostBoundaries());
                else fn.reference(centerBoundaryTangent[kd][ks]->
                       getLocalArrayWithGhostBoundaries());
                Integer numberOfPoints = 1;
                for (Integer kd1=0; kd1<3; kd1++) {
                    dimensionL(0,kd1) = fn.getBase(kd1);
                    dimensionL(1,kd1) = fn.getBound(kd1);
                    i[kd1] = d[kd1] = 
                      Range(dimensionL(0,kd1), dimensionL(1,kd1));
                    numberOfPoints *=
                      dimensionL(1,kd1) - dimensionL(0,kd1) + 1;
                } // end for
                if (numberOfPoints) {
                    Range p = numberOfPoints;
                    RealArray r(d1,d2,d3,d0);
                    for (Integer kd1=0; kd1<numberOfDimensions; kd1++)
                      if (kd1==kd) {
                        r(i1,i2,i3,kd1) = (Real)ks;
                    } else {
                        for (Integer k=dimensionL(0,kd1);
                          k<=dimensionL(1,kd1); k++) {
                            i[kd1] = k;
                            r(i1,i2,i3,kd1) = gridSpacing(kd1) *
                              ( k - indexRange(0,kd1)
                              + (isCellCentered(kd1) ? (Real).5 : (Real)0.) );
                        } // end for
                        i[kd1] = d[kd1];
                    } // end if, end for
                    r.reshape(p,d0); RealArray x(p,d0), xr(p,d0,d0);
                    mapping.map(r, x, xr);
                    r.redim(0); x.redim(0);

                    if (what & THEcenterBoundaryNormal) {
//                      Get the sign of the jacobian at the center of the grid.
                        Integer q = numberOfPoints / 2;
                        Real jac = ks ? 1. : -1.;
                        switch (numberOfDimensions) {
                          case 1:
                            jac *= xr(q,0,0);
                          break;
                          case 2:
                            jac *= xr(q,0,0)*xr(q,1,1) - xr(q,0,1)*xr(q,1,0);
                          break;
                          case 3:
                            jac *= (xr(q,0,0)*xr(q,1,1) - xr(q,0,1)*xr(q,1,0)) *
                                    xr(q,2,2) +
                                   (xr(q,0,1)*xr(q,1,2) - xr(q,0,2)*xr(q,1,1)) *
                                    xr(q,2,0) +
                                   (xr(q,0,2)*xr(q,1,0) - xr(q,0,0)*xr(q,1,2)) *
                                    xr(q,2,1);
                          break;
                        } // end switch
                        jac = (jac < 0.) ? -1. : (jac > 0.) ? 1. : 0.;
    
                        RealArray fnn(p); fn.reshape(p,d0);
                        switch (numberOfDimensions) {
                          case 1:
                            fn = jac;
                          break;
                          case 2: {
                            for (Integer l=0; l<numberOfDimensions; l++)
                              fn(p,l,0) =
                                (Real)((1-2*l)*(1-2*kd)) * xr(p,1-l,1-kd);
                            fnn(p) = jac / sqrt
                              (fn(p,0) * fn(p,0) + fn(p,1) * fn(p,1));
                            for (l=0; l<numberOfDimensions; l++)
                              fn(p,l) = fn(p,l) * fnn(p);
                          } break;
                          case 3: {
                            for (Integer l=0; l<numberOfDimensions; l++) {
                                Integer j=(kd+1)%3, k=(j+1)%3,
                                        m=(l +1)%3, n=(m+1)%3;
                                fn(p,l) =
                                  xr(p,m,j) * xr(p,n,k) - xr(p,m,k) * xr(p,n,j);
                            } // end for
                            fnn(p) = jac / sqrt(fn(p,0) * fn(p,0) +
                              fn(p,1) * fn(p,1) + fn(p,2) * fn(p,2));
                            for (l=0; l<numberOfDimensions; l++)
                              fn(p,l) *= fnn(p);
                          } break;
                        } // end switch
                    } // end if
                    if (numberOfDimensions > 1 &&
                      what & THEcenterBoundaryTangent) {
                        fn.reference(centerBoundaryTangent[kd][ks]->
                          getLocalArrayWithGhostBoundaries());
                        const Range d0m = numberOfDimensions - 1; Integer kd1;
                        RealArray fnn(p,one,d0m); fn.reshape(p,d0,d0m);
                        for (kd1=0; kd1<numberOfDimensions-1; kd1++) {
                            Integer kd2 = (kd + kd1 + 1) % numberOfDimensions;
                            fn(p,d0,kd1) = xr(p,d0,kd2);
                        } // end for
                        switch (numberOfDimensions) {
                          case 2:
                            fnn(p,0,d0m) = (Real)1. / sqrt
                             ( fn(p,0,d0m) * fn(p,0,d0m) +
                               fn(p,1,d0m) * fn(p,1,d0m) );
                          break;
                          case 3:
                            fnn(p,0,d0m) = (Real)1. / sqrt
                             ( fn(p,0,d0m) * fn(p,0,d0m) +
                               fn(p,1,d0m) * fn(p,1,d0m) +
                               fn(p,2,d0m) * fn(p,2,d0m) );
                          break;
                        } // end switch
                        for (kd1=0; kd1<numberOfDimensions; kd1++)
                          fn(p,kd1,d0m) *= fnn(p,0,d0m);
                    } // end if
                } // end if
            } // end for, end for
            if (what & THEcenterBoundaryNormal)
              computedGeometry |= THEcenterBoundaryNormal;
            if (numberOfDimensions > 1 && what & THEcenterBoundaryTangent)
              computedGeometry |= THEcenterBoundaryTangent;
        } // end if
    } // end if

    return returnValue;
}
