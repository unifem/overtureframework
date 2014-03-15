#include "Ogmg.h"
#include "display.h"
#include "ParallelUtility.h"

// static int *nipn=NULL, *ndipn=NULL, **ipn=NULL;  // add to class and destroy when done
      
static int numberOfNotEnoughInterpolationSpaceMessages=0;


#define smoothRedBlackOpt EXTERN_C_NAME(smoothredblackopt)
#define smoothJacobiOpt EXTERN_C_NAME(smoothjacobiopt)
#define getInterpNeighbours EXTERN_C_NAME(getinterpneighbours)
#define markInterpNeighbours EXTERN_C_NAME(markinterpneighbours)

extern "C"
{
  void smoothJacobiOpt( const int &nd,  const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
                       const int &nd3a, const int &nd3b,
		       const int &n1a, const int &n1b, const int &n1c,
                       const int &n2a, const int &n2b, const int &n2c,
                       const int &n3a, const int &n3b, const int &n3c,
                       const int &ndc, const real & f, const real & c,
                       const real & u, const real & v, const int & mask, const int & option, 
                       const int & order, const int & sparseStencil,
                       const real & cc, const real & varCoeff, const real & dx, const real & omega, const int & bc,
                       const int &np, const int &ndip, const int &ip, const int & ipar );

  void smoothRedBlackOpt( const int &nd,  const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
                       const int &nd3a, const int &nd3b,
		       const int &n1a, const int &n1b, const int &n1c,
                       const int &n2a, const int &n2b, const int &n2c,
                       const int &n3a, const int &n3b, const int &n3c, 
                       const int &ndc, const real & f, const real & c,
                       const real & u, const int & mask, const int & option, 
                       const int & order, const int & sparseStencil,
                       const real & cc, const real & varCoeff, const real & dx, const real & omega,
                       const int & useLocallyOptimalOmega, const real & variableOmegaScaleFactor );

    
  // serial version: 
  void getInterpNeighbours( const int &nd,  const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
	  		    const int &nd3a, const int &nd3b,
			    const int &n1a, const int &n1b,
			    const int &n2a, const int &n2b,
			    const int &n3a, const int &n3b,
			    const int &mask, int &mask2,
			    const int & nip,const int &ndip,const int &ip, 
			    int &nipn, const int &ndipn, int &ipn, const int &width, const int & ierr );

  // parallel version (does not use the interpolationPoint array)
  void markInterpNeighbours( const int &nd,  const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
	  		     const int &nd3a, const int &nd3b,
			     const int &n1a, const int &n1b,
			     const int &n2a, const int &n2b,
			     const int &n3a, const int &n3b,
			     const int &eir, const int &mask, int &mask2,
			     int &nipn, const int &ndipn, int &ipn, const int &width, const int & ierr );

}


//\begin{>>OgmgInclude.tex}{\subsection{smoothBoundary}}
void Ogmg::
smoothBoundary(int level, 
               int grid, 
               int bcOption[6], 
               int numberOfLayers /* =1 */, 
               int numberOfIterations /* =1 */ )
// ======================================================================================
//   /Description:
//       Smooth points near boundaries.
//
// /bcOption[6] (input): smooth (side,axis) if bcOption[side+2*axis]=1
//\end{OgmgInclude.tex} 
// ======================================================================================
{
  if( numberOfLayers<1 || numberOfIterations<1 )
    return;

  if( level>parameters.numberOfLevelsForBoundarySmoothing )
    return;

  real time0=getCPU();
  
  if( debug & 8 )
    printF("Smooth the boundary of grid=%i level=%i, numberOfLayers=%i\n",grid,level,numberOfLayers);
  

  realMappedGridFunction & u = uMG.multigridLevel[level][grid];
  realMappedGridFunction & f = fMG.multigridLevel[level][grid];
  realMappedGridFunction & c =  level==0 ? cMG[grid] : cMG.multigridLevel[level][grid];
  realArray & defect = defectMG.multigridLevel[level][grid];
  CompositeGrid & mgcg = multigridCompositeGrid();
  MappedGrid & mg = mgcg.multigridLevel[level][grid];  
  const intArray & mask = mg.mask();

  #ifdef USE_PPP
    intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
    // realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
  #else 
    const intSerialArray & maskLocal=mask;
    // const realSerialArray & uLocal=u;
  #endif
  const int *pmask = maskLocal.getDataPointer();
  const real *pf = f.getDataPointer();
  const real *pc = c.getDataPointer();

//  Index Iav[3], &I1a=Iav[0], &I2a=Iav[1], &I3a=Iav[2];
//  getIndex(mg.gridIndexRange(),I1a,I2a,I3a); //

  int pnab[6]={0,0,0,0,0,0};  //
#define nab(side,axis) pnab[(side)+2*(axis)]

  const IntegerArray & mgBC=mg.boundaryCondition();
  int bc[6]={0,0,0,0,0,0};
  for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      // nab(side,axis)=mg.gridIndexRange(side,axis);
      nab(side,axis)=mg.indexRange(side,axis);
      
      if( mgBC(side,axis)>0 )
      {
//  	bc[side+2*axis]= bcOption==0 ? boundaryCondition(side,axis,grid)==equation : 
//  	  bcOption==1 ? boundaryCondition(side,axis,grid)==extrapolate : 1;
	bc[side+2*axis]= bcOption[side+2*axis];
        if( boundaryCondition(side,axis,grid)==OgmgParameters::extrapolate )
	{
	  nab(side,axis)+= 1-2*side;  // do not smooth ON the dirichlet boundaries.
	}
	
      }
      else
      {
	bc[side+2*axis]=0;
      }
    }
    // adjust for internal parallel boundaries -- we could take into account dw[axis] 
    int side=0;
    if( mask.getLocalBase(axis) != mask.getBase(axis) )
    {
      nab(side,axis)=max(mg.indexRange(side,axis),maskLocal.getBase(axis)+mask.getGhostBoundaryWidth(axis));
      bc[side+2*axis]=0;
    }
    side=1;
    if( mask.getLocalBound(axis) != mask.getBound(axis) )
    {
      nab(side,axis)=min(mg.indexRange(side,axis),maskLocal.getBound(axis)-mask.getGhostBoundaryWidth(axis));
      bc[side+2*axis]=0;
    }
    
  }
  

  const IntegerArray & d = mg.dimension();
  const int ndc=c.getLength(0);

  const bool rectangular=(*c.getOperators()).isRectangular() && ( level < mgcg.numberOfMultigridLevels()-numberOfExtraLevels ); 

  real dx[3]={1.,1.,1.};
  int sparseStencil=general;
  if( mg.isRectangular() ) // ***
  {
    if( equationToSolve!=OgesParameters::userDefined ) // ***  defect is used on the boundary
    {
      if( equationToSolve==OgesParameters::divScalarGradOperator ||
	  equationToSolve==OgesParameters::variableHeatEquationOperator ||
	  equationToSolve==OgesParameters::divScalarGradHeatEquationOperator )
      {
	sparseStencil=level==0 ? sparseVariableCoefficients : variableCoefficients;
      }
      else
      {
	sparseStencil=level==0 ? sparseConstantCoefficients : constantCoeff;
      }
    }
    else if( rectangular && assumeSparseStencilForRectangularGrids )
      sparseStencil=sparse;

    mg.getDeltaX(dx);
  }

  real *vp = defect.getDataPointer(); // temp space for jacobi
  real *up= u.getDataPointer();
  real *vcp = varCoeff!=NULL ? (*varCoeff).multigridLevel[level][grid].getDataPointer() : up;
  const real *pcc = constantCoefficients.getBound(2)>=level ? &constantCoefficients(0,grid,level) : vp;
  
  int ipar[10];
  ipar[0]=numberOfLayers;

  int option=3;  // Gauss-Seidel on the boundary
  real omega=1.; // .8;

  if( debug & 4 )
  {
    printF("smoothBoundary: layers=%i nab= [%i,%i][%i,%i][%i,%i] bc=[%i,%i][%i,%i][%i,%i] "
	   " omega=%8.2e \n",numberOfLayers,
	   pnab[0],pnab[1],pnab[2],pnab[3],pnab[4],pnab[5],bc[0],bc[1],bc[2],bc[3],bc[4],bc[5],omega);
  }

  const int np=0,ndip=1,ip=0;
  const int n1c=1, n2c=1, n3c=1;
  for( int iter=0; iter<numberOfIterations; iter++ )
  {
    if( false ) // flip between 3 and 5 sometimes helps...
      ipar[0]=numberOfLayers - 2*(iter % 2);
      

    smoothJacobiOpt( mg.numberOfDimensions(), 
		     maskLocal.getBase(0),maskLocal.getBound(0),
		     maskLocal.getBase(1),maskLocal.getBound(1),
		     maskLocal.getBase(2),maskLocal.getBound(2),
		     pnab[0],pnab[1],n1c,pnab[2],pnab[3],n2c,pnab[4],pnab[5],n3c, ndc,
		     *pf,*pc,*up,*vp,*pmask,
		     option, orderOfAccuracy, sparseStencil, *pcc, *vcp, dx[0], omega,
		     bc[0],np,ndip,ip, ipar[0] );


    applyBoundaryConditions( level,grid,u,f ); 
  }



  tm[timeForBoundarySmooth]+=getCPU()-time0;
}
#undef nab

//\begin{>>OgmgInclude.tex}{\subsection{smoothBoundary}}
void Ogmg::
smoothInterpolationNeighbours(int level, int grid )
// ======================================================================================
//   /Description:
//      Smooth neighbours of interpolation points
//
// /bcOption[6] (input): smooth (side,axis) if bcOption[side+2*axis]=1
//\end{OgmgInclude.tex} 
// ======================================================================================
{
  if( parameters.numberOfInterpolationLayersToSmooth<1 || parameters.numberOfInterpolationSmoothIterations<1 ||
      level>=parameters.numberOfLevelsForInterpolationSmoothing )
    return;

  CompositeGrid & mgcg = multigridCompositeGrid();
  #ifdef USE_PPP
    const int nip = mgcg.numberOfComponentGrids()>1 ? 
                    mgcg.multigridLevel[level]->numberOfInterpolationPointsLocal(grid) : 0 ;
  #else
    const int nip = mgcg.multigridLevel[level].numberOfInterpolationPoints(grid);
  #endif

  // *fix me* -- in parallel we may have to smooth pts even if there are no local interp. pts: 
  if( nip==0 ) return; // no communication allowed after this point

  real time0=getCPU();
  
  const int numberOfLayers=parameters.numberOfInterpolationLayersToSmooth;
  const int numberOfIterations=parameters.numberOfInterpolationSmoothIterations;
  
  if( debug & 4 )
    fprintf(pDebugFile,"  --- Smooth interpolation neighbours of grid=%i level=%i, numberOfLayers=%i, nip=%i  ---\n",
	   grid,level,numberOfLayers,nip);
  

  realMappedGridFunction & u = uMG.multigridLevel[level][grid];
  realMappedGridFunction & f = fMG.multigridLevel[level][grid];
  realMappedGridFunction & c =  level==0 ? cMG[grid] : cMG.multigridLevel[level][grid];
  realArray & defect = defectMG.multigridLevel[level][grid];
  MappedGrid & mg = mgcg.multigridLevel[level][grid];  
  const intArray & mask = mg.mask();

  #ifdef USE_PPP
    intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
  #else 
    const intSerialArray & maskLocal=mask;
    const realSerialArray & uLocal=u;
  #endif
  const int * pmask = maskLocal.getDataPointer();
  const real *pf = f.getDataPointer();
  const real *pc = c.getDataPointer();

  // const IntegerArray & d = mg.dimension();
  const IntegerArray & gir = mg.gridIndexRange();
  
  const int ndc=c.getLength(0);

  const bool rectangular=(*c.getOperators()).isRectangular() && ( level < mgcg.numberOfMultigridLevels()-numberOfExtraLevels ); 

  real dx[3]={1.,1.,1.};
  int sparseStencil=general;
  if( mg.isRectangular() ) // ***
  {
    if( equationToSolve!=OgesParameters::userDefined ) // ***  defect is used on the boundary
    {
      if( equationToSolve==OgesParameters::divScalarGradOperator ||
	  equationToSolve==OgesParameters::variableHeatEquationOperator ||
	  equationToSolve==OgesParameters::divScalarGradHeatEquationOperator )
      {
	sparseStencil=level==0 ? sparseVariableCoefficients : variableCoefficients;
      }
      else
      {
	sparseStencil=level==0 ? sparseConstantCoefficients : constantCoeff;
      }
    }
    else if( rectangular && assumeSparseStencilForRectangularGrids )
      sparseStencil=sparse;

    mg.getDeltaX(dx);
  }

  real *vp = defect.getDataPointer(); // temp space for jacobi
  real *up= u.getDataPointer();
  real *vcp = varCoeff!=NULL ? (*varCoeff).multigridLevel[level][grid].getDataPointer() : up;


  const int numLevels=parameters.numberOfLevelsForInterpolationSmoothing;
  const int gid=level+numLevels*grid;


  if( parameters.numberOfInterpolationLayersToSmooth>0 && level<numLevels && 
      nip>0 && (nipn==NULL || ipn[gid]==NULL) )
  {
    // *** allocate space and/or determine interpolation neighbours that should be smoothed. ***

    #ifdef USE_PPP
      CompositeGrid & cg = mgcg.multigridLevel[level];
      intSerialArray ip;
      if( ( grid<cg.numberOfBaseGrids() && 
            cg->localInterpolationDataState==CompositeGridData::localInterpolationDataForAMR ) || 
            cg->localInterpolationDataState==CompositeGridData::noLocalInterpolationData )       
      {
        // ip.reference( cg.interpolationPoint[grid].getLocalArray() );
        OV_ABORT("lineSmooth:ERROR: interpolation info should be local!");
      }
      else
      {
        ip.reference( cg->interpolationPointLocal[grid] );
      }
    #else
      const intArray & ip = mgcg.multigridLevel[level].interpolationPoint[grid];
    #endif

    const int width=parameters.numberOfInterpolationLayersToSmooth;

    if( nipn==NULL )
    {
      const int numArrays=numLevels*mgcg.multigridLevel[0].numberOfComponentGrids();

      numberOfIBSArrays=numArrays;
      nipn  = new int [numArrays];
      ndipn = new int [numArrays];
      ipn   = new int * [numArrays];
      for( int i=0; i<numArrays; i++ )
      {
	nipn[i]=0;
	ndipn[i]=0;
	ipn[i]=NULL;
      }
    }

    // ** const int maxNumberOfNeighbours = nip*int(pow(2*width+1,mg.numberOfDimensions())-1 +.5); // this is normally too many **FIX**

    // The total number of neighbours of interpolation points is not very much larger than the current number
    // The worst case seems to be when the interp points are arranged in a circle
    //     2D :     New/Old = [ 2*pi*(r+dr) ]/[ 2*pi*r ] = 1+dr/r 
    //     3D :    New/Old = [ (4/3)pi*(r+dr)^2 ]/[ (4/3)pi*(r)^2 ] = (1+dr/r)^2

    const int maxNumberOfNeighbours = int( width*max(nip*(1.1 +4*4*4/nip)+20.,100.) ); 

    ndipn[gid]=maxNumberOfNeighbours;


    assert( ipn[gid]==NULL );
      
    ipn[gid] = new int [ndipn[gid]*mg.numberOfDimensions()];  // we store (i1,i2) or (i1,i2,i3)

    // no need to initialize mask2, this is done below
    intSerialArray mask2(maskLocal.dimension(0),maskLocal.dimension(1),maskLocal.dimension(2)); 
    int *pmask2 = mask2.getDataPointer();
    const int *pip = ip.getDataPointer();
      
    const IntegerArray & d = mg.dimension();
    const IntegerArray & mir = mg.indexRange(); 
    // valid neighbours live here (N.B. Do not include periodic boundaries or Dirichlet boundaries)
    int pir[6]={0,0,0,0,0,0};
    int peir[6]={0,0,0,0,0,0};
#define ir(side,axis) pir[(side)+2*(axis)]
#define eir(side,axis) peir[(side)+2*(axis)]
    int numGhost=orderOfAccuracy/2;
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      for( int side=0; side<=1; side++ )
      {
	ir(side,axis)=mir(side,axis);
	if( boundaryCondition(side,axis,grid)==OgmgParameters::extrapolate ) // dirichlet BC
	  ir(side,axis)+= 1-2*side;

        eir(side,axis)=ir(side,axis);
	if( boundaryCondition(side,axis,grid)==0 )
	  eir(side,axis)-= numGhost*(1-2*side);
      }
      // limit to parallel bounds -- we do not smooth parallel ghost points, but we do look for mask<0
      // pts there.
      int side=0;
      ir(side,axis) =max( ir(side,axis),maskLocal.getBase(axis)+mask.getGhostBoundaryWidth(axis));
      eir(side,axis)=max(eir(side,axis),maskLocal.getBase(axis));
      side=1;
      ir(side,axis) =min( ir(side,axis),maskLocal.getBound(axis)-mask.getGhostBoundaryWidth(axis));
      eir(side,axis)=min(eir(side,axis),maskLocal.getBound(axis));
      
    }

    int ierr=0;
    #ifdef USE_PPP
    // The parallel version does not use the ip array, just the mask
    markInterpNeighbours( mg.numberOfDimensions(), 
			 maskLocal.getBase(0),maskLocal.getBound(0),
			 maskLocal.getBase(1),maskLocal.getBound(1),
			 maskLocal.getBase(2),maskLocal.getBound(2),
			  pir[0],pir[1],pir[2],pir[3],pir[4],pir[5],
			 peir[0],*pmask,*pmask2, 
			 nipn[gid],ndipn[gid],ipn[gid][0], width, ierr );
    #else
    getInterpNeighbours( mg.numberOfDimensions(), 
			 maskLocal.getBase(0),maskLocal.getBound(0),
			 maskLocal.getBase(1),maskLocal.getBound(1),
			 maskLocal.getBase(2),maskLocal.getBound(2),
			 pir[0],pir[1],pir[2],pir[3],pir[4],pir[5],
			 *pmask,*pmask2, nip,ip.getLength(0),*pip, 
			 nipn[gid],ndipn[gid],ipn[gid][0], width, ierr );
    #endif
      
    int numberOfTrys=0;
    while( ierr!=0 )
    {
      numberOfTrys++;

      const real factor = 1.5 + numberOfTrys/2.;  // *wdh* 2012/05/05
      int newNumber= int( ndipn[gid]*(factor+.5) );
     
      if( numberOfNotEnoughInterpolationSpaceMessages<5 )
      {
	numberOfNotEnoughInterpolationSpaceMessages++;
	
	printf("Ogmg:smoothInterpolationNeighbours:WARNING: Not enough space was allocated for interpolation neighbours\n"
	       "    level=%i grid=%i ...numberOfInterpolationPoints=%i ...estimated number of neighbours=%i"
               " new number=%i (factor=%f) \n",
	       level,grid,nip,ndipn[gid],newNumber,factor);
      }
      else
      {
        numberOfNotEnoughInterpolationSpaceMessages++;
	printf("Ogmg:smoothInterpolationNeighbours:Too many Not enough space was allocated messages."
               " I will not print anymore\n");
      }

      ndipn[gid]=newNumber;

      delete [] ipn[gid];
      ipn[gid] = new int [ndipn[gid]*mg.numberOfDimensions()];  

      #ifdef USE_PPP
      // The parallel version does not use the ip array, just the mask
      markInterpNeighbours( mg.numberOfDimensions(), 
			   maskLocal.getBase(0),maskLocal.getBound(0),
			   maskLocal.getBase(1),maskLocal.getBound(1),
			   maskLocal.getBase(2),maskLocal.getBound(2), 
			   pir[0],pir[1],pir[2],pir[3],pir[4],pir[5],
			   peir[0],*pmask,*pmask2,  
			   nipn[gid],ndipn[gid],ipn[gid][0], width, ierr );
      #else
      getInterpNeighbours( mg.numberOfDimensions(), 
			   maskLocal.getBase(0),maskLocal.getBound(0),
			   maskLocal.getBase(1),maskLocal.getBound(1),
			   maskLocal.getBase(2),maskLocal.getBound(2), 
			   pir[0],pir[1],pir[2],pir[3],pir[4],pir[5],
			   *pmask,*pmask2, nip,ip.getLength(0),*pip, 
			   nipn[gid],ndipn[gid],ipn[gid][0], width, ierr );
      #endif
	
      assert( numberOfTrys<5 );
    }
    if( debug & 4 )
      fprintf(pDebugFile,"****Ogmg:smoothInterpolationNeighbours: level=%i grid=%i nip=%i, nipn=%i "
	     "(ndipn=%i, excess=%i excess/nipn=%6.2f)\n",
	     level,grid,nip,nipn[gid],ndipn[gid],ndipn[gid]-nipn[gid],(ndipn[gid]-nipn[gid])/real(nipn[gid]));
      
      
    if( pDebugFile!=NULL && ( debug & 4 || debug & 16) )
    {
      fprintf(pDebugFile,"\n ****** INFO for Smoothing Interpolation Neighbours grid=%i level=%i width=%i ********\n",
	      grid,level,width);

      #ifndef USE_PPP
       fprintf(pDebugFile,"nip=%i, ip=",nip);
       for( int i=0; i<nip; i++ )
       {
 	 fprintf(pDebugFile,"(%3i,%3i)",ip(i,0),ip(i,1));
 	 if( i%10 == 9 ) fprintf(pDebugFile,"\n  ");
       }
      #endif
	
      fprintf(pDebugFile,"\n\n nipn=%i, ndipn=%i, ipn=",nipn[gid],ndipn[gid]);      
      int *pipn = ipn[gid];
      const int ndipn0=ndipn[gid];
#define IPN(i,m) pipn[(i)+ndipn0*(m)]
	 
      for( int i=0; i<nipn[gid]; i++ )
      {
	fprintf(pDebugFile,"(%3i,%3i)",IPN(i,0),IPN(i,1));
	if( i%10 == 9 ) fprintf(pDebugFile,"\n  ");
      }
      fprintf(pDebugFile,"\n");
	 
    }
    
  } // end if parameters.numberOfInterpolationLayersToSmooth>0 && level<numLevels && ...
  
  

  if( nipn!=NULL && nipn[gid]>0 )
  {
    // int option=4;  // interpolation neighbours with Gauss-Seidel
    // real omega=1.; // .8;
    int option=5;  // interpolation neighbours with Jacobi
    real omega=.9;
     
    int ipar[10];
    ipar[0]=numberOfLayers;
    int bc[6]={0,0,0,0,0,0}; 

    const int n1c=1, n2c=1, n3c=1;
    if( debug & 4 )
      printf("Smooth interp neighbours: level=%i grid=%i, number of neighbours=nipn[gid]=%i \n",
	     level,grid,nipn[gid]);

    if( false && Ogmg::debug & 1 )
    {
      char buff[100];
      display(uLocal,sPrintF(buff,"smoothIN:level=%i,grid=%iHere is the solution BEFORE smoothIN, it %i",
			level,grid,0),pDebugFile,"%9.1e");
    }
    for( int iter=0; iter<numberOfIterations; iter++ )
    {
      smoothJacobiOpt( mg.numberOfDimensions(), 
		       maskLocal.getBase(0),maskLocal.getBound(0),
		       maskLocal.getBase(1),maskLocal.getBound(1),
		       maskLocal.getBase(2),maskLocal.getBound(2),
		       gir(0,0),gir(1,0),n1c,gir(0,1),gir(1,1),n2c,gir(0,2),gir(1,2),n3c, ndc,
		       *pf,*pc,*up,*vp,*pmask,
		       option, orderOfAccuracy, sparseStencil, constantCoefficients(0,grid,level), *vcp, dx[0], omega,
		       bc[0], nipn[gid],ndipn[gid],ipn[gid][0], ipar[0] );

      // maybe not applyBoundaryConditions( level,grid,u,f ); // this is needed if interp boundary hits physical boundary (hcic.order4)
      
      if( false && Ogmg::debug & 1 )
      {
	char buff[100];
	display(uLocal,sPrintF(buff,"smoothIN:level=%i,grid=%i, Here is the solution AFTER smoothIN, it %i",
			  level,grid,iter),pDebugFile,"%9.1e");
      }
      
    }
  }
  


  tm[timeForInterpolationSmooth]+=getCPU()-time0;
}
#undef ir
#undef eir
