//   (Ex).t = (1/eps)*[  (Hz).y ]
//   (Ey).t = (1/eps)*[ -(Hz).x ]
//   (Hz).t = (1/mu) *[ (Ex).y - (Ey).x ]

#define exTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc[0]
#define eyTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc[1]
#define hzTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc[5]

#define extTrue(x,y,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc[0]
#define eytTrue(x,y,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc[1]
#define hztTrue(x,y,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc[5]

#define exLaplacianTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*(-(twoPi*twoPi*(kx*kx+ky*ky))*pwc[0])
#define eyLaplacianTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*(-(twoPi*twoPi*(kx*kx+ky*ky))*pwc[1])
#define hzLaplacianTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*(-(twoPi*twoPi*(kx*kx+ky*ky))*pwc[5])

// Here is a plane wave with the shape of a Gaussian
// xi = kx*(x)+ky*(y)-cc*(t)
// cc=  c*sqrt( kx*kx+ky*ky );
#define hzGaussianPulse(xi)  exp(-betaGaussianPlaneWave*((xi)*(xi)))
#define exGaussianPulse(xi)  hzGaussianPulse(xi)*(-ky/(eps*cc))
#define eyGaussianPulse(xi)  hzGaussianPulse(xi)*( kx/(eps*cc))

#define hzLaplacianGaussianPulse(xi)  ((4.*betaGaussianPlaneWave*betaGaussianPlaneWave*(kx*kx+ky*ky))*xi*xi-\
                                        (2.*betaGaussianPlaneWave*(kx*kx+ky*ky)))*exp(-betaGaussianPlaneWave*((xi)*(xi)))
#define exLaplacianGaussianPulse(xi)  hzLaplacianGaussianPulse(xi,t)*(-ky/(eps*cc))
#define eyLaplacianGaussianPulse(xi)  hzLaplacianGaussianPulse(xi,t)*( kx/(eps*cc))

// 3D
//
//   (Ex).t = (1/eps)*[ (Hz).y - (Hy).z ]
//   (Ey).t = (1/eps)*[ (Hx).z - (Hz).x ]
//   (Ez).t = (1/eps)*[ (Hy).x - (Hx).y ]
//   (Hx).t = (1/mu) *[ (Ey).z - (Ez).y ]
//   (Hy).t = (1/mu) *[ (Ez).x - (Ex).z ]
//   (Hz).t = (1/mu) *[ (Ex).y - (Ey).x ]

// ****************** finish this -> should `rotate' the 2d solution ****************

#define exTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc[0]
#define eyTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc[1]
#define ezTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc[2]

#define extTrue3d(x,y,z,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc[0]
#define eytTrue3d(x,y,z,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc[1]
#define eztTrue3d(x,y,z,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc[2]



#define hxTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc[3]
#define hyTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc[4]
#define hzTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc[5]

#define exLaplacianTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*(-(twoPi*twoPi*(kx*kx+ky*ky+kz*kz))*pwc[0])
#define eyLaplacianTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*(-(twoPi*twoPi*(kx*kx+ky*ky+kz*kz))*pwc[1])
#define ezLaplacianTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*(-(twoPi*twoPi*(kx*kx+ky*ky+kz*kz))*pwc[2])

#define hxLaplacianTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*(-(twoPi*twoPi*(kx*kx+ky*ky+kz*kz))*pwc[3])
#define hyLaplacianTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*(-(twoPi*twoPi*(kx*kx+ky*ky+kz*kz))*pwc[4])
#define hzLaplacianTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*(-(twoPi*twoPi*(kx*kx+ky*ky+kz*kz))*pwc[5])



//==================================================================================================
// Evaluate Tom Hagstom's exact solution defined as an integral of Guassian sources
// 
// OPTION: OPTION=solution or OPTION=error OPTION=bounary to compute the solution or the error or
//     the boundary condition
//
//==================================================================================================
#beginMacro getGaussianIntegralSolution(OPTION,VEX,VEY,VHZ,t,I1,I2,I3)

if( initialConditionOption==gaussianIntegralInitialCondition )
{
  
  double wt,wx,wy;
  const int nsources=1;
  double xs[nsources], ys[nsources], tau[nsources], var[nsources], amp[nsources];
  xs[0]=0.;
  ys[0]=1.e-8*1./3.;  // should not be on a grid point
  tau[0]=-.95;
  var[0]=30.;
  amp[0]=1.;
   
  double period= 1.;  // period in y
  double time=t;
   
  int i1,i2,i3;

  FOR_3D(i1,i2,i3,I1,I2,I3)
  {
    double x=X(i1,i2,i3,0); 
    double y=X(i1,i2,i3,1);

    exmax(wt,wx,wy,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x,y,time);

    #If #OPTION eq "solution"
      VEX(i1,i2,i3) = wy;
      VEY(i1,i2,i3) =-wx;
      VHZ(i1,i2,i3)= wt;
    #Elif #OPTION eq "error" 
      ERREX(i1,i2,i3) = VEX(i1,i2,i3) - wy;
      ERREY(i1,i2,i3) = VEY(i1,i2,i3) + wx;
      ERRHZ(i1,i2,i3) = VHZ(i1,i2,i3) - wt;

    #Else
      U(i1,i2,i3,ex) = wy;
      U(i1,i2,i3,ey) =-wx;
      U(i1,i2,i3,hz) = wt;
    #End
	
  }
}

#endMacro


//==================================================================================================
// The DEFINE_GF_MACRO is a helper for EXTRACT_GFP and sets up the cpp macros for a given field
//
//==================================================================================================
#beginMacro DEFINE_GF_MACRO(GFM,GFP,DIM0,DIM1,DIM2,DFA,CCX)

#ifdef GFM ##
ERROR : GFM ##  already defined!
#else
#define GFM ## (i0,i1,i2) GFP ## [i0+DIM0 ## *(i1+DIM1 ## *(i2+DIM2 ## *( CCX )))]
#endif

#endMacro
//==================================================================================================

//==================================================================================================
// The EXTRACT_GFP macro extracts gridfunction pointers and array bounds.
//                 We use these to write code that works in 2/3D for both the nfdtd and
//                 dsi schemes.
//
// The macro expects the user to have the following variables defined in the enclosing scope:
//    Index I1,I2,I3 - used to get the index ranges for each grid
//    CompositeGrid &cg - the composite grid used by the fields
//    int grid      - the current grid to setup the pointers for
//    int i1,i2,i3  - grid indices into the arrays at the appropriate centering
//    MappedGrid *Maxwell:mgp - or -
//          CompositeGrid *Maxwell cgfields -or- CompositeGrid *Maxwell dsi_cgfields
//
// The macro defines:
//    cpp macros:
//               UH{XYZ}(i0,i1,i2) - access the current h field at i1,i2,i3 with the appropriate centering
//               UE{XYZ}(i0,i1,i2) - access the current e field at i1,i2,i3 with the appropriate centering        
//               UMH{XYZ}(i0,i1,i2) - the h field at the previous timestep
//               UME{XYZ}(i0,i1,i2) - the e field at the previous timestep
//               UNH{XYZ}(i0,i1,i2) - the h field at the next timestep
//               UNE{XYZ}(i0,i1,i2) - the h field at the next timestep
//
//               ERRH{XYZ}(i0,i1,i2) - acces the h field error gridfunction
//               ERRE{XYZ}(i0,i1,i2) - acces the e field error gridfunction
//
//               XEP(i0,i1,i2,i3) - coordinates of e centering
//               XHP(i0,i1,i2,i3) - coordinates of h centering
// 
//    variables:
//               MappedGrid &mg - the current mapped grid (cg[grid])
//
//               const bool isStructured - true for structured grids
//               const bool isRectangular - true for rectangular grids
//
//               realMappedGridFunction uh - view of the current h or h.n field
//               realMappedGridFunction ue - view of the current e or e.n field
//               realMappedGridFunction umh - view of the previous h or h.n field
//               realMappedGridFunction ume - view of the previous e or e.n field
//               realMappedGridFunction unh - view of the next h or h.n field
//               realMappedGridFunction une - view of the next e or e.n field
//               realMappedGridFunction errh - view of the h field error
//               realMappedGridFunction erre - view of the e field error
//               realArray xe - view of the x coordinates at the e centering
//               realArray xh - view of the x coordinates at the h centering
//               realArray ye - view of the y coordinates at the e centering
//               realArray yh - view of the y coordinates at the h centering
//               realArray ze - view of the z coordinates at the e centering
//               realArray zh - view of the z coordinates at the h centering
//               realArray xce - coordinates of the e centering
//               realArray xch - coordinates of the h centering
//               realArray emptyArray - used for setting references to things we don't need
//               real *uhp - data pointer for the current h or h.n field
//               real *uep - data pointer for the current h or e.n field
//               real *umhp - data pointer for the previous h or h.n field
//               real *umep - data pointer for the previous h or e.n field
//               real *unhp - data pointer for the next h or h.n field
//               real *unep - data pointer for the next h or e.n field
//               real *xep - data pointer for the coordinates at the e centering
//               real *xhp - data pointer for the coordinates at the h centering
//
//               real dx[3] - dx in each direction for rectangular grids  (={0,0,0} if !isRectangular)
//               real xab[2][3] - coordinate bounds for rectangular grids (={ {0,0},.. } if !isRectangular)
//
//               int uhDim0,uhDim1,uhDim2 - array dimensions for the e gridfunctions
//               int ueDim0,ueDim1,ueDim2 - array dimensions for the h gridfunctions
//               int xeDim0,xeDim1,xeDim2 - array dimensions for the e centering coordinates
//               int xhDim0,xhDim1,xhDim2 - array dimensions for the h centering coordinates
//
// KNOWN ASSUMPTIONS:  * gridFunctions for the same variable at different time levels have the same
//                                   raw data sizes
//                     * there are unrecognized and perhaps subtle assumptions being made
//         
// OPTION: 
//==================================================================================================
#beginMacro EXTRACT_GFP(OPTION)

#ifdef EXTGFP_SENTINEL
ERROR : XXX : you have not closed the current EXTRACT_GFP macro before starting a new one!
#endif
#define EXTGFP_SENTINEL
#ifdef EXTGFP_SENTINEL

realArray emptyArray;
realSerialArray emptySerialArray;

Range all;
Index I1,I2,I3;
Index Iev[3], &Ie1=Iev[0], &Ie2=Iev[1], &Ie3=Iev[2];
Index Ihv[3], &Ih1=Ihv[0], &Ih2=Ihv[1], &Ih3=Ihv[2];

MappedGrid & mg = cg[grid];
const bool isStructured = mg.getGridType()==MappedGrid::structuredGrid;
const bool isRectangular = mg.isRectangular();
assert( !(isRectangular && !isStructured) ); // just a little check on the MappedGrid's data

real dx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
if( isRectangular )
  mg.getRectangularGridParameters( dx, xab );

//realMappedGridFunction uh,ue,umh,ume,unh,une;
realSerialArray uh,ue,umh,ume,unh,une,errh,erre;
realSerialArray xe,xh,ye,yh,ze,zh,xce,xch;
realSerialArray uepp, uhpp; // dsi projection arrays

intArray & mask = mg.mask();
#ifdef USE_PPP
  intSerialArray maskLocal;  getLocalArrayWithGhostBoundaries(mask,maskLocal);
#else
  intSerialArray & maskLocal = mask;
#endif


// const bool buildCenter = !( isRectangular &&
// 			    ( initialConditionOption==squareEigenfunctionInitialCondition ||
// 			      initialConditionOption==gaussianPulseInitialCondition ||
//                               (forcingOption==gaussianChargeSource && initialConditionOption==defaultInitialCondition)
//                               || initialConditionOption==userDefinedKnownSolutionInitialCondition 
//                               || initialConditionOption==userDefinedInitialConditionsOption
//                                // || initialConditionOption==planeMaterialInterfaceInitialCondition
// 			       // ||  initialConditionOption==annulusEigenfunctionInitialCondition
// 			       ) 
// 			    ); // fix this 

const bool buildCenter = vertexArrayIsNeeded( grid );
if( buildCenter )
{
  // printF("assignInitialConditions:INFO:build the grid vertices, grid=%i\n",grid);
  mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex);
}

const realArray & center = buildCenter ? mg.center() : emptyArray;
realSerialArray uLocal;

real dtb2=dt*.5;
real tE = t, tH = t;


getIndex(mg.dimension(),I1,I2,I3);
int includeGhost=1;
bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);
#If #OPTION ne "FORCING"
if( !ok ) continue;  // no communication allowed after this point : check this ******************************************************
#Else
if( !ok ) return 0;  // no communication allowed after this point : check this ******************************************************
#End



if( method==nfdtd || method==sosup ) 
{
  Ie1 = Ih1 = I1;
  Ie2 = Ih2 = I2;
  Ie3 = Ih3 = I3;
  
  // nfdtd uses one gridFunction per time level
  realMappedGridFunction & uall = mgp==NULL ? getCGField(HField,current)[grid] : fields[current];
  realMappedGridFunction & umall = mgp==NULL ? getCGField(HField,prev)[grid] : fields[prev];
  realMappedGridFunction & unall = mgp==NULL ? getCGField(HField,next)[grid] : fields[next];

  #ifdef USE_PPP
    getLocalArrayWithGhostBoundaries(uall,uLocal);
    realSerialArray umLocal; getLocalArrayWithGhostBoundaries(umall,umLocal);
    realSerialArray unLocal; getLocalArrayWithGhostBoundaries(unall,unLocal);
  #else
    uLocal.reference(uall);
    realSerialArray & umLocal = umall;
    realSerialArray & unLocal = unall;
  #endif

  if ( cg.numberOfDimensions()==2 ) 
  {
    ue.reference( uLocal(I1,I2,I3,Range(ex,ey)) );
    uh.reference( uLocal(I1,I2,I3,hz) );
    ume.reference( umLocal(I1,I2,I3,Range(ex,ey)) );
    umh.reference( umLocal(I1,I2,I3,hz) );
    une.reference( unLocal(I1,I2,I3,Range(ex,ey)) );
    unh.reference( unLocal(I1,I2,I3,hz) );

    if( errp )
    {
      #ifdef USE_PPP
        realSerialArray errLocal; getLocalArrayWithGhostBoundaries(*errp,errLocal);
        errh.reference(errLocal(I1,I2,I3,hz));
        erre.reference(errLocal(I1,I2,I3,Range(ex,ey)));
      #else
        errh.reference((*errp)(I1,I2,I3,hz));
        erre.reference((*errp)(I1,I2,I3,Range(ex,ey)));
      #endif
    }
    else if ( cgerrp )
    {
      #ifdef USE_PPP
        realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);
        errh.reference(errLocal(I1,I2,I3,hz));
        erre.reference(errLocal(I1,I2,I3,Range(ex,ey)));
      #else
        errh.reference((*cgerrp)[grid](I1,I2,I3,hz));
        erre.reference((*cgerrp)[grid](I1,I2,I3,Range(ex,ey)));
      #endif
    }
  }
  else
  {
    if ( solveForElectricField )
    {
      ue.reference( uLocal(I1,I2,I3,Range(ex,ez)) );
      ume.reference( umLocal(I1,I2,I3,Range(ex,ez)) );
      une.reference( unLocal(I1,I2,I3,Range(ex,ez)) );
      if ( errp )
      {
        #ifdef USE_PPP
          realSerialArray errLocal; getLocalArrayWithGhostBoundaries(*errp,errLocal);
          erre.reference(errLocal(I1,I2,I3,Range(ex,ez)));
        #else
          erre.reference((*errp)(I1,I2,I3,Range(ex,ez)));
        #endif

      }
      else if ( cgerrp )
      {
        #ifdef USE_PPP
          realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);
          erre.reference(errLocal(I1,I2,I3,Range(ex,ez)));
        #else
          erre.reference((*cgerrp)[grid](I1,I2,I3,Range(ex,ez)));
        #endif
      }
    }

    if ( solveForMagneticField )
    {
      uh.reference( uLocal(I1,I2,I3,Range(hx,hz)) );
      umh.reference( umLocal(I1,I2,I3,Range(hx,hz)) );
      unh.reference( unLocal(I1,I2,I3,Range(hx,hz)) );
      if ( errp )
      {
        #ifdef USE_PPP
          realSerialArray errLocal; getLocalArrayWithGhostBoundaries(*errp,errLocal);
          errh.reference(errLocal(I1,I2,I3,Range(hx,hz)));
        #else
          errh.reference((*errp)(I1,I2,I3,Range(hx,hz)));
        #endif
      }
      else if ( cgerrp )
      {
        #ifdef USE_PPP
          realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);
          errh.reference(errLocal(I1,I2,I3,Range(hx,hz)));
        #else
          errh.reference((*cgerrp)[grid](I1,I2,I3,Range(hx,hz)));
        #endif
      }
    }
  }
  
  #ifdef USE_PPP
    realSerialArray xLocal; if( buildCenter ) getLocalArrayWithGhostBoundaries(center,xLocal);
  #else
    const realSerialArray & xLocal = center;
  #endif

  if( buildCenter )
  {
    // *wdh* 041015 these next assignments fail in P++ if buildCenter==false -- but why make the reference?
    xe.reference( (buildCenter ? xLocal(I1,I2,I3,0) : emptySerialArray) );
    ye.reference( (buildCenter ? xLocal(I1,I2,I3,1) : emptySerialArray) );
    if ( numberOfDimensions==3 )
      ze.reference( (buildCenter ? xLocal(I1,I2,I3,2) : emptySerialArray) );

    // nfdtd uses the same centering for e and h
    xh.reference(xe);
    yh.reference(ye);
    zh.reference(ze);  // *wdh* 090628
  }

  if ( buildCenter )
  {
    xce.reference(xLocal);
    xch.reference(xce);
  }
}
else // dsi or yee scheme
{
  tE -= dtb2;

  if ( method==dsiMatVec /*&& numberOfDimensions==3*/ )
  {
    realMappedGridFunction &uep = (mgp==NULL ? getCGField(EField,current)[grid] : fields[current+numberOfTimeLevels]);
    realMappedGridFunction &uhp = (mgp==NULL ? getCGField(HField,current)[grid] : fields[current]);
    realMappedGridFunction &uepm = (mgp==NULL ? getCGField(EField,next)[grid] : fields[next+numberOfTimeLevels]);
    realMappedGridFunction &uhpm = (mgp==NULL ? getCGField(HField,next)[grid] : fields[next]);

    #ifdef USE_PPP
      realSerialArray uepLocal; getLocalArrayWithGhostBoundaries(uep,uepLocal);
      realSerialArray uhpLocal; getLocalArrayWithGhostBoundaries(uhp,uhpLocal);
      realSerialArray uepmLocal; getLocalArrayWithGhostBoundaries(uepm,uepmLocal);
      realSerialArray uhpmLocal; getLocalArrayWithGhostBoundaries(uhpm,uhpmLocal);
    #else
      realSerialArray & uepLocal=uep;
      realSerialArray & uhpLocal=uhp;
      realSerialArray & uepmLocal=uepm;
      realSerialArray & uhpmLocal=uhpm;
    #endif

    realMappedGridFunction uer, uhr, uerm, uhrm;
    uepp.reference(uepLocal);
    uhpp.reference(uhpLocal);

#If #OPTION == "FORCING"

    ue.reference ( uepLocal );
    uh.reference ( uhpLocal );
    ume.reference ( uepmLocal );
    umh.reference ( uhpmLocal );

#Else

    uer.updateToMatchGrid(mg,GridFunctionParameters::edgeCentered,numberOfDimensions);
    uerm.updateToMatchGrid(mg,GridFunctionParameters::edgeCentered,numberOfDimensions);
    if ( numberOfDimensions==2 )
    {
      uhr.updateToMatchGrid(mg,GridFunctionParameters::cellCentered);
      uhrm.updateToMatchGrid(mg,GridFunctionParameters::cellCentered);
    }
    else
    {
      uhr.updateToMatchGrid(mg,GridFunctionParameters::faceCenteredAll,numberOfDimensions);
      uhrm.updateToMatchGrid(mg,GridFunctionParameters::faceCenteredAll,numberOfDimensions);
    }      


    uer = uerm = -100;
    uhr = uhrm = -100;

    #If #OPTION == "IC"

    #Else
      reconstructDSIField(tE,EField,uep,uer);  
      reconstructDSIField(tE-dt,EField,uepm,uerm);

      reconstructDSIField(tH,HField,uhp,uhr);  
      reconstructDSIField(tH-dt,HField,uhpm,uhrm);
    #End

    #ifdef USE_PPP
      realSerialArray uerLocal; getLocalArrayWithGhostBoundaries(uer,uerLocal);
      realSerialArray uhrLocal; getLocalArrayWithGhostBoundaries(uhr,uhrLocal);
      realSerialArray uermLocal; getLocalArrayWithGhostBoundaries(uerm,uermLocal);
      realSerialArray uhrmLocal; getLocalArrayWithGhostBoundaries(uhrm,uhrmLocal);
    #else
      realSerialArray & uerLocal=uer;
      realSerialArray & uhrLocal=uhr;
      realSerialArray & uermLocal=uerm;
      realSerialArray & uhrmLocal=uhrm;
    #endif
    ue.reference ( uerLocal );
    uh.reference ( uhrLocal );
    ume.reference ( uermLocal );
    umh.reference ( uhrmLocal );

    //      cout<<"UMH SIZE "<<umh.getLength(0)<<"  "<<umh.getLength(3)<<endl;
#End

  }
  else
  {
    #ifdef USE_PPP
      Overture::abort("finish me for parallel");
    #else
      ue.reference ( (mgp==NULL ? getCGField(EField,current)[grid] : fields[current+numberOfTimeLevels]) );
      uh.reference ( (mgp==NULL ? getCGField(HField,current)[grid] : fields[current]) );
      ume.reference ( (mgp==NULL ? getCGField(EField,next)[grid] : fields[next+numberOfTimeLevels]) );
      umh.reference ( (mgp==NULL ? getCGField(HField,next)[grid] : fields[next]) );
    #endif
  }

// #Else
//       ue.reference ( (mgp==NULL ? getCGField(EField,current)[grid] : fields[current+numberOfTimeLevels]) );
//       uh.reference ( (mgp==NULL ? getCGField(HField,current)[grid] : fields[current]) );
//       ume.reference ( (mgp==NULL ? getCGField(EField,next)[grid] : fields[next+numberOfTimeLevels]) );
//       umh.reference ( (mgp==NULL ? getCGField(HField,next)[grid] : fields[next]) );

// #End

  if ( errp )
  {
    #ifdef USE_PPP
      Overture::abort("finish me for parallel");
    #else
      erre.reference(errp[0]);
      errh.reference(errp[1]);
    #endif
  }
  else if ( cgerrp )
  {
    #ifdef USE_PPP
      Overture::abort("finish me for parallel");
    #else
      erre.reference(cgerrp[0][grid]);
      errh.reference(cgerrp[1][grid]);
    #endif
  }

  Ih1 = Range(uh.getBase(0),uh.getBound(0));
  Ih2 = Range(uh.getBase(1),uh.getBound(1));
  Ih3 = Range(uh.getBase(2),uh.getBound(2));

  Ie1 = Range(ue.getBase(0),ue.getBound(0));
  Ie2 = Range(ue.getBase(1),ue.getBound(1));
  Ie3 = Range(ue.getBase(2),ue.getBound(2));

  I1 = Ih1;
  I2 = Ih2;
  I3 = Ih3;

  if ( buildCenter )
  {
    const realArray & center = mg.isAllVertexCentered() ? mg.corner() : mg.center();
      
    //      cout<<"CENTER SIZE "<<center.getLength(0)<<endl;
    //      cout<<"MG VERTEX/CELL CENT "<<mg.isAllVertexCentered()<<"  "<<mg.isAllCellCentered()<<endl;
    //      getFaceCenters(mg, xce);
   #ifdef USE_PPP
     Overture::abort("finish me for parallel");
   #else    

    if ( mg.numberOfDimensions()==2 )
    {
      getCenters(mg, UnstructuredMapping::Edge, xce);
      xch.reference(center);
      xce.reshape(Range(xce.getLength(0)),1,1,Range(xce.getLength(1)));
    }
    else
    {
      getCenters(mg, UnstructuredMapping::Face, xch);
      getCenters(mg, UnstructuredMapping::Edge, xce);
      xce.reshape(Range(xce.getLength(0)),1,1,Range(xce.getLength(1)));
      xch.reshape(Range(xch.getLength(0)),1,1,Range(xch.getLength(1)));

    }
      


    //xh.reference(xch(all,all,all,0));
    //yh.reference(xch(all,all,all,1));
    xh.reference(xch(Ih1,Ih2,Ih3,0,all));
    yh.reference(xch(Ih1,Ih2,Ih3,1,all));
    if ( numberOfDimensions==3 )
      zh.reference(xch(Ih1,Ih2,Ih3,2,all));

    xe.reference(xce(Ie1,Ie2,Ie3,0,all));
    ye.reference(xce(Ie1,Ie2,Ie3,1,all));
    if ( numberOfDimensions==3 )
      ze.reference(xce(Ie1,Ie2,Ie3,2,all));
  #endif

  }
}

real *uhp=0,*uep=0,*umhp=0,*umep=0,*unhp=0,*unep=0,*xep=0,*xhp=0,*errhp=0,*errep=0;
int uhDim0,uhDim1,uhDim2,uhDimFA;
int ueDim0,ueDim1,ueDim2,ueDimFA;
int xeDim0,xeDim1,xeDim2;
int xhDim0,xhDim1,xhDim2;
uhDim0=uhDim1=uhDim2=ueDim0=ueDim1=ueDim2=xeDim0=xeDim1=xeDim2=xhDim0=xhDim1=xhDim2=-1;

// #ifdef USE_PPP

// realSerialArray uel; getLocalArrayWithGhostBoundaries(ue,uel);
// realSerialArray uhl; getLocalArrayWithGhostBoundaries(uh,uhl);
// realSerialArray umel; if( ume.getLength(0)>0 ) ume.getLocalArrayWithGhostBoundaries(ume,umel);
// realSerialArray umhl; if( umh.getLength(0)>0 ) umh.getLocalArrayWithGhostBoundaries(umh,umhl);
// realSerialArray unel; getLocalArrayWithGhostBoundaries(une,unel);
// realSerialArray unhl; getLocalArrayWithGhostBoundaries(unh,unhl);
// realSerialArray xcel; getLocalArrayWithGhostBoundaries(xce,xcel);
// realSerialArray xchl; getLocalArrayWithGhostBoundaries(xch,xchl);

// realSerialArray errel; getLocalArrayWithGhostBoundaries(erre,errel);
// realSerialArray errhl; getLocalArrayWithGhostBoundaries(errh,errhl);

// // intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskl);

// #else

const realSerialArray &uel = ue;
const realSerialArray &uhl = uh;
const realSerialArray &umel = ume;
const realSerialArray &umhl = umh;
const realSerialArray &unel = une;
const realSerialArray &unhl = unh;
const realSerialArray &xcel = xce;
const realSerialArray &xchl = xch;
const realSerialArray &errel = erre;
const realSerialArray &errhl = errh;

// const intSerialArray & maskLocal = mask;

// #endif

// H field pointer and array definitions
uhp = uhl.Array_Descriptor.Array_View_Pointer3;
if ( umhl.getLength(0) )
{
  umhp = umhl.Array_Descriptor.Array_View_Pointer3;
}
unhp = unhl.Array_Descriptor.Array_View_Pointer3;
uhDim0 = uhl.getRawDataSize(0);
uhDim1 = uhl.getRawDataSize(1);
uhDim2 = uhl.getRawDataSize(2);
uhDimFA = uhl.getRawDataSize(4);

//DEFINE_GF_MACRO is a bpp macro
DEFINE_GF_MACRO(UHX,uhp,uhDim0,uhDim1,uhDim2,uhDimFA,hx);
DEFINE_GF_MACRO(UHY,uhp,uhDim0,uhDim1,uhDim2,uhDimFA,hy);
DEFINE_GF_MACRO(UHZ,uhp,uhDim0,uhDim1,uhDim2,uhDimFA,hz);

DEFINE_GF_MACRO(UMHX,umhp,uhDim0,uhDim1,uhDim2,uhDimFA,hx);
DEFINE_GF_MACRO(UMHY,umhp,uhDim0,uhDim1,uhDim2,uhDimFA,hy);
DEFINE_GF_MACRO(UMHZ,umhp,uhDim0,uhDim1,uhDim2,uhDimFA,hz);

DEFINE_GF_MACRO(UNHX,unhp,uhDim0,uhDim1,uhDim2,uhDimFA,hx);
DEFINE_GF_MACRO(UNHY,unhp,uhDim0,uhDim1,uhDim2,uhDimFA,hy);
DEFINE_GF_MACRO(UNHZ,unhp,uhDim0,uhDim1,uhDim2,uhDimFA,hz);

errhp = errhl.Array_Descriptor.Array_View_Pointer3;
DEFINE_GF_MACRO(ERRHX,errhp,uhDim0,uhDim1,uhDim2,uhDimFA,hx);
DEFINE_GF_MACRO(ERRHY,errhp,uhDim0,uhDim1,uhDim2,uhDimFA,hy);
DEFINE_GF_MACRO(ERRHZ,errhp,uhDim0,uhDim1,uhDim2,uhDimFA,hz);

// #ifdef UNH
// ERROR : UNH already defined!
// #else
// #define UNH(i0,i1,i2,i3) unhp[i0+uhDim0*(i1+uhDim1*(i2+uhDim2*(i3)))]
// #endif

xhp = xchl.Array_Descriptor.Array_View_Pointer3;
xhDim0 = xchl.getRawDataSize(0);
xhDim1 = xchl.getRawDataSize(1);
xhDim2 = xchl.getRawDataSize(2);
#ifdef XHP
ERROR : XHP already defined!
#else
#define XHP(i0,i1,i2,i3) xhp[i0+xhDim0*(i1+xhDim1*(i2+xhDim2*(i3)))]
#endif

#ifdef X
ERROR : X already defined!
#else
#define X(i0,i1,i2,i3) xhp[i0+xhDim0*(i1+xhDim1*(i2+xhDim2*(i3)))]
#endif

// E Field pointer and array definitions
uep = uel.Array_Descriptor.Array_View_Pointer3;
if ( umel.getLength(0) )
{
  umep = umel.Array_Descriptor.Array_View_Pointer3;
}
unep = unel.Array_Descriptor.Array_View_Pointer3;
ueDim0 = uel.getRawDataSize(0);
ueDim1 = uel.getRawDataSize(1);
ueDim2 = uel.getRawDataSize(2);
ueDimFA = uel.getRawDataSize(4);

//DEFINE_GF_MACRO is a bpp macro
DEFINE_GF_MACRO(UEX,uep,ueDim0,ueDim1,ueDim2,ueDimFA,ex);
DEFINE_GF_MACRO(UEY,uep,ueDim0,ueDim1,ueDim2,ueDimFA,ey);
DEFINE_GF_MACRO(UEZ,uep,ueDim0,ueDim1,ueDim2,ueDimFA,ez);

DEFINE_GF_MACRO(UMEX,umep,ueDim0,ueDim1,ueDim2,ueDimFA,ex);
DEFINE_GF_MACRO(UMEY,umep,ueDim0,ueDim1,ueDim2,ueDimFA,ey);
DEFINE_GF_MACRO(UMEZ,umep,ueDim0,ueDim1,ueDim2,ueDimFA,ez);

DEFINE_GF_MACRO(UNEX,unep,ueDim0,ueDim1,ueDim2,ueDimFA,ex);
DEFINE_GF_MACRO(UNEY,unep,ueDim0,ueDim1,ueDim2,ueDimFA,ey);
DEFINE_GF_MACRO(UNEZ,unep,ueDim0,ueDim1,ueDim2,ueDimFA,ez);

errep = errel.Array_Descriptor.Array_View_Pointer3;
DEFINE_GF_MACRO(ERREX,errep,ueDim0,ueDim1,ueDim2,ueDimFA,ex);
DEFINE_GF_MACRO(ERREY,errep,ueDim0,ueDim1,ueDim2,ueDimFA,ey);
DEFINE_GF_MACRO(ERREZ,errep,ueDim0,ueDim1,ueDim2,ueDimFA,ez);

const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
const int maskDim0=maskLocal.getRawDataSize(0);
const int maskDim1=maskLocal.getRawDataSize(1);
const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

// #ifdef UE
// ERROR : UE already defined!
// #else
// #define UE(i0,i1,i2,i3) uep[i0+ueDim0*(i1+ueDim1*(i2+ueDim2*(i3)))]
// #endif

xep = xcel.Array_Descriptor.Array_View_Pointer3;
xeDim0 = xcel.getRawDataSize(0);
xeDim1 = xcel.getRawDataSize(1);
xeDim2 = xcel.getRawDataSize(2);
#ifdef XEP
ERROR : XEP already defined!
#else
#define XEP(i0,i1,i2,i3) xep[i0+xeDim0*(i1+xeDim1*(i2+xeDim2*(i3)))]
#endif

#endMacro
//==================================================================================================
//==================================================================================================



//==================================================================================================
// This bpp macro undefs the cpp macros defined by EXTRACT_GFP
//               UH(i0,i1,i2) - access the current h field at i1,i2,i3 with the appropriate centering
//               UE(i0,i1,i2) - access the current e field at i1,i2,i3 with the appropriate centering        
//               UMH(i0,i1,i2) - the h field at the previous timestep
//               UME(i0,i1,i2) - the e field at the previous timestep
//               UNH(i0,i1,i2) - the h field at the next timestep
//               UNE(i0,i1,i2) - the h field at the next timestep
//               XEP(i0,i1,i2,i3) - coordinates of e centering
//               XHP(i0,i1,i2,i3) - coordinates of h centering
// OPTION: 
//==================================================================================================
#beginMacro EXTRACT_GFP_END(OPTION)


#If ( #OPTION == "IC" )
if( method==dsiMatVec  )
{
  #ifdef USE_PPP
    Overture::abort("finish me for parallel");
  #else

  // XXX the following is broken for PARALLEL right now (need to use more macros...
  bool vCent = mg.isAllVertexCentered();
  int nDim = mg.numberOfDimensions();
  realMappedGridFunction &uep = (mgp==NULL ? getCGField(EField,current)[grid] : fields[current+numberOfTimeLevels]);
  realMappedGridFunction &uhp = (mgp==NULL ? getCGField(HField,current)[grid] : fields[current]);
  realMappedGridFunction &uepm = (mgp==NULL ? getCGField(EField,next)[grid] : fields[next+numberOfTimeLevels]);
  realMappedGridFunction &uhpm = (mgp==NULL ? getCGField(HField,next)[grid] : fields[next]);

  if ( mg.numberOfDimensions()==2 )
  {
    for ( int e=0; e<edgeAreaNormals.getLength(0); e++ )
    {
      uep(e,0,0,0) = 0;
      uepm(e,0,0,0) =  0;
      for ( int a=0; a<mg.numberOfDimensions(); a++ )
      {
	uep(e,0,0,0) += edgeAreaNormals(e,0,0,a)*ue(e,0,0,a);
	uepm(e,0,0,0) += edgeAreaNormals(e,0,0,a)*ume(e,0,0,a);
      }
    }
    uhp = uh;
    uhpm = umh;
  }
  else
  {
    //      cout<<"geom sizes "<<cFArea.getLength(0)<<"  "<<cEArea.getLength(0)<<endl;
    for ( int f=0; f<faceAreaNormals.getLength(0); f++ )
    {
      uhp(f,0,0,0) = 0;
      uhpm(f,0,0,0) =  0;
      for ( int a=0; a<mg.numberOfDimensions(); a++ )
      {
	uhp(f,0,0,0) += faceAreaNormals(f,0,0,a)*uh(f,0,0,a);
	uhpm(f,0,0,0) += faceAreaNormals(f,0,0,a)*umh(f,0,0,a);
      }
    }

    for ( int e=0; e<edgeAreaNormals.getLength(0); e++ )
    {
      uep(e,0,0,0) = 0;
      uepm(e,0,0,0) =  0;
      for ( int a=0; a<mg.numberOfDimensions(); a++ )
      {
	uep(e,0,0,0) += edgeAreaNormals(e,0,0,a)*ue(e,0,0,a);
	uepm(e,0,0,0) += edgeAreaNormals(e,0,0,a)*ume(e,0,0,a);
      }
    }
  }
  #endif
}

if( method==nfdtd || method==sosup )
{
  (mgp==NULL ? getCGField(HField,current)[grid] : fields[current]).periodicUpdate();
  (mgp==NULL ? getCGField(HField,prev)[grid] : fields[prev]).periodicUpdate();
  
}
else
{
  (mgp==NULL ? getCGField(HField,current)[grid] : fields[current]).periodicUpdate();
  (mgp==NULL ? getCGField(EField,current)[grid] : fields[current+2]).periodicUpdate();
}

#End

#undef         UHX
#undef         UHY
#undef         UHZ
#undef         UEX
#undef         UEY
#undef         UEZ
#undef         UMHX
#undef         UMHY
#undef         UMHZ
#undef         UMEX
#undef         UMEY
#undef         UMEZ
#undef         UNHX
#undef         UNHY
#undef         UNHZ
#undef         UNEX
#undef         UNEY
#undef         UNEZ
#undef         XEP
#undef         XHP
#undef         X

#undef         ERRHX
#undef         ERRHY
#undef         ERRHZ

#undef         ERREX
#undef         ERREY
#undef         ERREZ

#undef         MASK

#endif 
#undef EXTGFP_SENTINEL
#endMacro
