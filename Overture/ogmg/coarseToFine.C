#include "Ogmg.h"
#include "Interpolant.h"
#include "ParallelUtility.h"
#include "gridFunctionNorms.h"

//\begin{>>OgmgInclude.tex}{\subsection{coarseToFine(level)}}
void Ogmg::
coarseToFine(const int & level) 
//---------------------------------------------------------------------------------------------
// /Description:
//    Correction: coarse to fine transfer.
// \[
//          u[level] += \textbf{Prolongation}[ u[level+1] ]
// \]
//\end{OgmgInclude.tex} 
//---------------------------------------------------------------------------------------------
{
  real time=getCPU();
  CompositeGrid & mgcg = multigridCompositeGrid();
  realCompositeGridFunction & u =uMG.multigridLevel[level];
  realCompositeGridFunction & f =fMG.multigridLevel[level];

  if( Ogmg::debug & 16 )
  {
    printF("%*.1s Ogmg::coarseToFine:level = %i \n",level*2,"  ",level);
    u.display("coarseToFine: Here is u before the correction",debugFile,"%10.2e");
  }
  if( Ogmg::debug & 4 )
  {
    defect(level);
    real maximumDefect=maxNorm(defectMG.multigridLevel[level]);
    fPrintF(debugFile,"%*.1s Ogmg:coarseToFine, level = %i, defect before correction = %e \n",
            level*4,"  ",level,maximumDefect);
  }

  if( Ogmg::debug & 8 )
  {
    uMG.multigridLevel[level].display(sPrintF("coarseToFine: u before coarseToFine level=%i",level),
                                      debugFile,"%10.2e");
    uMG.multigridLevel[level+1].display(sPrintF("coarseToFine: uCoarse before coarseToFine level+1=%i",level+1),
                                      debugFile,"%10.2e");
  }

  for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
    coarseToFine(level,grid);

  if( Ogmg::debug & 8 )
  {
    uMG.multigridLevel[level].display("coarseToFine: u after coarseToFine but before interp",debugFile,"%10.2e");
  }

  interpolate( u,-1,level );                      // Interpolate the corrected solution

  if( Ogmg::debug & 8 )
  {
    uMG.multigridLevel[level].display("coarseToFine: u after intep",debugFile,"%10.2e");
  }

  applyBoundaryConditions( level,u,f );  // set ghost values, periodic update.
  


  if( Ogmg::debug & 4 ) 
  {
    defect(level);
    real maximumDefect=maxNorm(defectMG.multigridLevel[level]);

    // printF("%*.1s Ogmg:coarseToFine, level = %i, defect after correction = %e \n",level*4,"  ",level,maximumDefect);

    fPrintF(debugFile,"%*.1s Ogmg:coarseToFine, level = %i, defect after correction = %e \n",
	   level*4,"  ",level,maximumDefect);
  }
  if(  Ogmg::debug & 16 &&  ps!=0 &&  ps->isGraphicsWindowOpen() )
  {
    psp.set(GI_TOP_LABEL,sPrintF(buff,"coarseToFine: level=%i, defect after coarseToFine (cycle=%i)",
            level,numberOfCycles)); 
    ps->erase();
    PlotIt::contour(*ps,defectMG.multigridLevel[level],psp); 
    psp.set(GI_TOP_LABEL,sPrintF(buff,"coarseToFine: level=%i, solution after coarseToFine (cycle=%i)",
            level,numberOfCycles)); 
    ps->erase();
    PlotIt::contour(*ps,uMG.multigridLevel[level],psp);
  }

  if( Ogmg::debug & 8 && level==0 )
  {
    uMG.multigridLevel[level+1].display("coarseToFine: Here is u on the coarse grid",debugFile,"%10.2e");
    uMG.multigridLevel[level].display("coarseToFine: Here is u after correction",debugFile,"%10.2e");
    defectMG.multigridLevel[level].display("coarseToFine: here is the defect on the fine grid",debugFile,"%10.2e");
  }

  tm[timeForCoarseToFine]+=getCPU()-time;
}

//---------------------------------------------------------------------------------------------
//   Prolongation on a component grid
//
//     u.multigridLevel[level] += Prolongation[ u.multigridLevel[level+1] ]
//   
//---------------------------------------------------------------------------------------------
//     ...2nd order interpolation
#define Q2000(j1,j2,j3) ( uCoarse(j1,j2,j3) )
#define Q2100(j1,j2,j3) ( cp2(0,cf1)*uCoarse(j1,j2,j3)+cp2(1,cf1)*uCoarse(j1+1,j2  ,j3  ) )
#define Q2010(j1,j2,j3) ( cp2(0,cf2)*uCoarse(j1,j2,j3)+cp2(1,cf2)*uCoarse(j1  ,j2+1,j3  ) )
#define Q2001(j1,j2,j3) ( cp2(0,cf3)*uCoarse(j1,j2,j3)+cp2(1,cf3)*uCoarse(j1  ,j2  ,j3+1) )
#define Q2110(j1,j2,j3) ( cp2(0,cf2)*  Q2100(j1,j2,j3)+cp2(1,cf2)*  Q2100(j1  ,j2+1,j3  ) )
#define Q2101(j1,j2,j3) ( cp2(0,cf3)*  Q2100(j1,j2,j3)+cp2(1,cf3)*  Q2100(j1  ,j2  ,j3+1) )
#define Q2011(j1,j2,j3) ( cp2(0,cf3)*  Q2010(j1,j2,j3)+cp2(1,cf3)*  Q2010(j1  ,j2  ,j3+1) )
#define Q2111(j1,j2,j3) ( cp2(0,cf3)*  Q2110(j1,j2,j3)+cp2(1,cf3)*  Q2110(j1  ,j2  ,j3+1) )

//     ...fourth order interpolation
#define Q4000(j1,j2,j3) ( uCoarse(j1,j2,j3) )

#define Q4100(j1,j2,j3) ( cp4( 0,cf1)*uCoarse(j1  ,j2  ,j3  )+cp4(1,cf1)*uCoarse(j1+1,j2  ,j3  ) \
                         +cp4(-1,cf1)*uCoarse(j1-1,j2  ,j3  )+cp4(2,cf1)*uCoarse(j1+2,j2  ,j3  ) )

#define Q4010(j1,j2,j3) ( cp4( 0,cf2)*uCoarse(j1  ,j2  ,j3  )+cp4(1,cf2)*uCoarse(j1  ,j2+1,j3  ) \
                         +cp4(-1,cf2)*uCoarse(j1  ,j2-1,j3  )+cp4(2,cf2)*uCoarse(j1  ,j2+2,j3  ) )

#define Q4001(j1,j2,j3) ( cp4( 0,cf3)*uCoarse(j1  ,j2  ,j3  )+cp4(1,cf3)*uCoarse(j1  ,j2  ,j3+1) \
                         +cp4(-1,cf3)*uCoarse(j1  ,j2  ,j3-1)+cp4(2,cf3)*uCoarse(j1  ,j2  ,j3+2) )

#define Q4110(j1,j2,j3) ( cp4( 0,cf2)*  Q4100(j1  ,j2  ,j3  )+cp4(1,cf2)*  Q4100(j1  ,j2+1,j3  ) \
                         +cp4(-1,cf2)*  Q4100(j1  ,j2-1,j3  )+cp4(2,cf2)*  Q4100(j1  ,j2+2,j3  ) )

#define Q4101(j1,j2,j3) ( cp4( 0,cf3)*  Q4100(j1  ,j2  ,j3  )+cp4(1,cf3)*  Q4100(j1  ,j2  ,j3+1) \
                         +cp4(-1,cf3)*  Q4100(j1  ,j2  ,j3-1)+cp4(2,cf3)*  Q4100(j1  ,j2  ,j3+2) )

#define Q4011(j1,j2,j3) ( cp4( 0,cf3)*  Q4010(j1  ,j2  ,j3  )+cp4(1,cf3)*  Q4010(j1  ,j2  ,j3+1) \
                         +cp4(-1,cf3)*  Q4010(j1  ,j2  ,j3-1)+cp4(2,cf3)*  Q4010(j1  ,j2  ,j3+2) )

#define Q4111(j1,j2,j3) ( cp4( 0,cf3)*  Q4110(j1  ,j2  ,j3  )+cp4(1,cf3)*  Q4110(j1  ,j2  ,j3+1) \
                         +cp4(-1,cf3)*  Q4110(j1  ,j2  ,j3-1)+cp4(2,cf3)*  Q4110(j1  ,j2  ,j3+2) )

//\begin{>>OgmgInclude.tex}{\subsection{coarseToFine(level,grid)}}
void Ogmg::
coarseToFine(const int & level, const int & grid)
//===================================================================
// /Description:
//  Correct a Component Grid
// \[
//      u(i,j) = u(i,j) + P[ u2(i,j) ]   \qquad\textrm{( P : Prolongation )}
// \]
//  cp21,cp22,cp23 : coeffcients for prolongation, 2nd order
//  cp41,cp41,cp43 : coeffcients for prolongation, 4th order
//
//\end{OgmgInclude.tex} 
//===================================================================
  //     & cp21( 0:1),cp22( 0:1),cp23( 0:1),
  //     & cp41(-1:2),cp42(-1:2),cp43(-1:2)
{
  CompositeGrid & mgcg = multigridCompositeGrid();
  MappedGrid & mgFine              = mgcg.multigridLevel[level][grid];  
  MappedGrid & mgCoarse            = mgcg.multigridLevel[level+1][grid];  
  realArray & uFine   = uMG.multigridLevel[level][grid];
  realArray & uCoarse = uMG.multigridLevel[level+1][grid];
  const int & numberOfDimensions = mgFine.numberOfDimensions();
  #ifdef USE_PPP
    intSerialArray mask; getLocalArrayWithGhostBoundaries(mgFine.mask(),mask);
  #else
    const intSerialArray & mask = mgFine.mask();
  #endif

//     if( Ogmg::debug & 4 )
//     {
//       displayMask(mask,sPrintF("coarseToFine: level=%i grid=%i",level,grid),pDebugFile);
//     }
    
  // int numberOfFictitiousPoints = orderOfAccuracy/2;
  int numberOfFictitiousPoints = 0;  // *wdh* 030525 : no need to correct fictitious points since BC's will assign these

  if( true )
  {
    // ***** new way ******
    // assert( orderOfAccuracy==2 ); // **need another variable to specify the orderOfCoarseToFineTransfer
    Range all;
    const IntegerArray & ratio = mgcg.multigridCoarseningRatio(all,grid,level+1);
    Index Iv[3], &I1 = Iv[0], &I2=Iv[1], &I3=Iv[2];
    getIndex(mgFine.indexRange(),I1,I2,I3,numberOfFictitiousPoints);

    int update=1;  // this means add the correction onto uFine
    // *wdh*030607 interp.interpolateFineFromCoarse(uFine,mask,Iv,uCoarse,ratio,update,coarseToFineInterpolationWidth);
    // wdh 100118 interp.interpolateFineFromCoarse(uFine,mask,Iv,uCoarse,ratio,update);
    interp.interpolateFineFromCoarse(uFine,mask,Iv,uCoarse,ratio,update,parameters.coarseToFineTransferWidth);

    return;
  }
  
  Overture::abort();


  RealArray cp2(Range(0,1),Range(1,2));
  RealArray cp4(Range(-1,2),Range(1,2));
  cp2(0,1)=1.; cp2(1,1)=0.;  // if coarsening factor =1 we just transfer the data
  cp2(0,2)=.5; cp2(1,2)=.5;  // coarsen factor = 2

  cp4(-1,1)=0.;     cp4(0,1)=1.;    cp4(1,1)=0.;    cp4(2,1)=0.;  
  cp4(-1,2)=-.0625; cp4(0,2)=.5625; cp4(1,2)=.5625; cp4(2,2)=-.0625;  // 4-point order interpolation


/* ---
  where( mgCoarse.mask()==0 )  // ******************************************8
    uCoarse=1.e9;

---- */

  int cf1,cf2,cf3, cf[3];
  cf1=cf[0]=mgcg.multigridCoarseningRatio(axis1,grid,level+1);  // coarsening factor
  cf2=cf[1]=mgcg.multigridCoarseningRatio(axis2,grid,level+1);  
  cf3=cf[2]=mgcg.multigridCoarseningRatio(axis3,grid,level+1);  

  assert(cf[0]==2 && (cf[1]==2 || numberOfDimensions<2) && (cf[2]==2 || numberOfDimensions<3));
  
  //----------------------------------------------------------------------------------------
  // There are two types of corrections:
  //   (1) when a fine grid and coarse grid point coincide, use Index's I1,I2,I3
  //   (2) when a fine grid point is midway between coarse grid points, use I1p,I2p,I3p
  //
  //        1--2--1--2--1--2------ ... -----2--1--2--1  fine grid
  //        X-----B-----X--------- ... -----X--B-----X  coarse grid
  //   
  //           B=boundary
  //
  //   Note that we use more fictitious points on the fine grid than on the coarse
  //-----------------------------------------------------------------------------------------
  
  // ****** old way *****

  Index Iav[3], &I1a = Iav[0], &I2a=Iav[1], &I3a=Iav[2];
  Index Jav[3], &J1a = Jav[0], &J2a=Jav[1], &J3a=Jav[2];
  Index I1,I2,I3, J1,J2,J3;
  Index I1p,I2p,I3p,J1p,J2p,J3p;
  int nf0,nf1;


  getIndex(mgFine.indexRange(),I1a,I2a,I3a);
/* ----
  int axis;
  for( axis=0; axis<mgFine.numberOfDimensions(); axis++ )
  {
    int shift=(mgFine.indexRange(Start,axis)-mgFine.extendedIndexRange(Start,axis)) % cf[axis];
    if( shift!=0 )
      Iav[axis]=Range(Iav[axis].getBase()-shift,Iav[axis].getBound());
    shift=(mgFine.extendedIndexRange(End,axis)-mgFine.indexRange(End,axis)) % cf[axis];
    if( shift!=0 )
      Iav[axis]=Range(Iav[axis].getBase(),Iav[axis].getBound()+shift);
  }
---- */
  // ************* this only works for coarsening factor=2 *********
  //----------------------------------
  //---  Get Index's for fine grid ---
  //----------------------------------
  nf0=((numberOfFictitiousPoints+1)/2)*2;   
  nf1=((numberOfFictitiousPoints-2)/2)*2;
  I1p=                          IndexBB(I1a.getBase()-nf0,I1a.getBound()+nf1,2);
  I2p= numberOfDimensions > 1 ? IndexBB(I2a.getBase()-nf0,I2a.getBound()+nf1,2) : I2a;
  I3p= numberOfDimensions > 2 ? IndexBB(I3a.getBase()-nf0,I3a.getBound()+nf1,2) : I3a;

  nf0=((numberOfFictitiousPoints)/2)*2;  
  nf1=((numberOfFictitiousPoints)/2)*2;
  I1 =                          IndexBB(I1a.getBase()-nf0,I1a.getBound()+nf1,2);
  I2 = numberOfDimensions > 1 ? IndexBB(I2a.getBase()-nf0,I2a.getBound()+nf1,2) : I2a;
  I3 = numberOfDimensions > 2 ? IndexBB(I3a.getBase()-nf0,I3a.getBound()+nf1,2) : I3a;

  //------------------------------------
  //---  Get Index's for coarse grid ---
  //------------------------------------
  getIndex(mgCoarse.indexRange(),J1a,J2a,J3a);   // this is ok

  nf0=((numberOfFictitiousPoints+1)/2);  
  nf1=((numberOfFictitiousPoints-2)/2);
  J1p=                          IndexBB(J1a.getBase()-nf0,J1a.getBound()+nf1);
  J2p= numberOfDimensions > 1 ? IndexBB(J2a.getBase()-nf0,J2a.getBound()+nf1) : J2a;
  J3p= numberOfDimensions > 2 ? IndexBB(J3a.getBase()-nf0,J3a.getBound()+nf1) : J3a;
  nf0=((numberOfFictitiousPoints)/2);  
  nf1=((numberOfFictitiousPoints)/2);
  J1 =                          IndexBB(J1a.getBase()-nf0,J1a.getBound()+nf1);
  J2 = numberOfDimensions > 1 ? IndexBB(J2a.getBase()-nf0,J2a.getBound()+nf1) : J2a;
  J3 = numberOfDimensions > 2 ? IndexBB(J3a.getBase()-nf0,J3a.getBound()+nf1) : J3a;

  where( mask(I1,I2,I3)>0 )  // ** 020205 do not change iterpolation pts for iterative interp **
    uFine(I1,I2,I3)+=uCoarse(J1,J2,J3);
  if( orderOfAccuracy==2 )
  {
    where( mask(I1p+1,I2,I3)>0 )  
      uFine(I1p+1,I2,I3)+=Q2100(J1p,J2 ,J3);
    if( numberOfDimensions > 1 )
    {
      where( mask(I1,I2p+1,I3)>0 ) 
        uFine(I1   ,I2p+1,I3)+=Q2010(J1 ,J2p,J3);
      where( mask(I1p+1,I2p+1,I3)>0 ) 
        uFine(I1p+1,I2p+1,I3)+=Q2110(J1p,J2p,J3);
    }  
    if( numberOfDimensions>2 )
    {
      where( mask(I1   ,I2   ,I3p+1)>0 ) 
        uFine(I1   ,I2   ,I3p+1)+=Q2001(J1 ,J2 ,J3p);
      where( mask(I1p+1,I2   ,I3p+1)>0 ) 
        uFine(I1p+1,I2   ,I3p+1)+=Q2101(J1p,J2 ,J3p);
      where( mask(I1   ,I2p+1,I3p+1)>0 ) 
        uFine(I1   ,I2p+1,I3p+1)+=Q2011(J1 ,J2p,J3p);
      where( mask(I1p+1,I2p+1,I3p+1)>0 ) 
        uFine(I1p+1,I2p+1,I3p+1)+=Q2111(J1p,J2p,J3p);
    }
  }
  else
  {   // -------- fourth-order ------------
    where( mask(I1p+1,I2,I3)>0 ) 
      uFine(I1p+1,I2,I3)+=Q4100(J1p,J2 ,J3);
    if( numberOfDimensions > 1 )
    {
      where( mask(I1   ,I2p+1,I3)>0 )  
	uFine(I1   ,I2p+1,I3)+=Q4010(J1 ,J2p,J3);
      where( mask(I1p+1,I2p+1,I3)>0 )  
	uFine(I1p+1,I2p+1,I3)+=Q4110(J1p,J2p,J3);
    }  
    if( numberOfDimensions>2 )
    {
      where( mask(I1   ,I2   ,I3p+1)>0 ) 
	uFine(I1   ,I2   ,I3p+1)+=Q4001(J1 ,J2 ,J3p);
      where( mask(I1p+1,I2   ,I3p+1)>0 ) 
	uFine(I1p+1,I2   ,I3p+1)+=Q4101(J1p,J2 ,J3p);
      where( mask(I1   ,I2p+1,I3p+1)>0 ) 
	uFine(I1   ,I2p+1,I3p+1)+=Q4011(J1 ,J2p,J3p);
      where( mask(I1p+1,I2p+1,I3p+1)>0 ) 
	uFine(I1p+1,I2p+1,I3p+1)+=Q4111(J1p,J2p,J3p);
    }
  }
}

#undef Q2000
#undef Q2100
#undef Q2010
#undef Q2001
#undef Q2110
#undef Q2101
#undef Q2011
#undef Q2111
#undef Q4000
#undef Q4100
#undef Q4010
#undef Q4001
#undef Q4110
#undef Q4101
#undef Q4011
#undef Q4111
