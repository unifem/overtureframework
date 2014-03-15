// #define BOUNDS_CHECK
#include "Ogmg.h"
#include "ParallelUtility.h"
#include "gridFunctionNorms.h"
#include "App.h"

//\begin{>>OgmgInclude.tex}{\subsection{defect(level)}}
void Ogmg::
defect(const int & level)
//---------------------------------------------------------------------------------------------
// /Description:
//    Defect computation 
//
//   Fill in  defectMG[level] with f-Lu
//\end{OgmgInclude.tex} 
//---------------------------------------------------------------------------------------------
{
  if( Ogmg::debug & 16 )
    printf("%*.1s Ogmg:defect, level = %i, ",level*2,"  ",level);
  CompositeGrid & mgcg = multigridCompositeGrid();
  for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
    defect(level,grid);

  defectMG.multigridLevel[level].periodicUpdate(); // **** is this needed ?? *****

}

#undef C
#undef M123
#define M123(m1,m2,m3) (m1+halfWidth1+width1*(m2+halfWidth2+width2*(m3+halfWidth3)))
// define C(m1,m2,m3,I1,I2,I3) c(I1,I2,I3,M123(m1,m2,m3))
#define C(m1,m2,m3,I1,I2,I3) c(M123(m1,m2,m3),I1,I2,I3)
// define C(m1,m2,m3,I1,I2,I3) c(M123(m1,m2,m3),I1,I2,I3)


//\begin{>>OgmgInclude.tex}{\subsection{defect(level,grid)}}
void Ogmg::
defect(const int & level, const int & grid)
//---------------------------------------------------------------------------------------------
// /Description:
//    Defect computation on a component grid
//
//    Compute defectMG.multigridLevel[level][grid]
//\end{OgmgInclude.tex} 
//---------------------------------------------------------------------------------------------
{
  real time=getCPU();

  realArray & u = uMG.multigridLevel[level][grid];
  realArray & f = fMG.multigridLevel[level][grid];     // *** use f at level 1!
  realMappedGridFunction & c =  level==0 ? cMG[grid] : cMG.multigridLevel[level][grid];
  CompositeGrid & mgcg = multigridCompositeGrid();
  MappedGrid & mg = mgcg.multigridLevel[level][grid];  
  realArray & defect = defectMG.multigridLevel[level][grid];

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  getIndex(mg.extendedIndexRange(),I1,I2,I3);

  if( equationToSolve!=OgesParameters::userDefined && mg.isRectangular() )  // *this is funny, why rectangular ??? -------------
  {
    // do NOT compute the defect on the boundary for the predefine equations with dirichlet BC's
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      if( boundaryCondition(0,axis,grid)==OgmgParameters::extrapolate && boundaryCondition(1,axis,grid)==OgmgParameters::extrapolate )
	Iv[axis]=Range(Iv[axis].getBase()+1,Iv[axis].getBound()-1);
      else if( boundaryCondition(0,axis,grid)==OgmgParameters::extrapolate  )
	Iv[axis]=Range(Iv[axis].getBase()+1,Iv[axis].getBound());
      else if( boundaryCondition(1,axis,grid)==OgmgParameters::extrapolate  )
	Iv[axis]=Range(Iv[axis].getBase(),Iv[axis].getBound()-1);
    }
  }
  

  // defect=0.;                   // *** this IS currently necessary **** fix this ******
  assign(defect,0.);  // *wdh* 100411
  
  getDefect(level,grid,f,u,I1,I2,I3,defect);

  if( Ogmg::debug & 16 )
  {
    real maximumDefect=maxNorm(defectMG.multigridLevel[level][grid]); // max(fabs(defect(I1,I2,I3)));
    printf("maximumDefect = %e \n",maximumDefect);
  }
  
  if( Ogmg::debug & 64 )
    display(u,"defect: Here is u",debugFile);
  if( Ogmg::debug & 4 )
  {
    display(defect,sPrintF("defect: Here is the defect, level=%i, grid=%i",level,grid),debugFile);
    if( level>0 )
    {
      display(u,sPrintF("defect: Here is the u level=%i, grid=%i",level,grid),debugFile);
      display(f,sPrintF("defect: Here is the f level=%i, grid=%i",level,grid),debugFile);
    }
    
  }
  
  tm[timeForDefect]+=getCPU()-time;
}


//\begin{>>OgmgInclude.tex}{\subsection{defect(level,grid)}}
real Ogmg::
defectMaximumNorm(const int & level, int approximationStride /* =1 */ )
//---------------------------------------------------------------------------------------------
// /Description:
//    Compute the maximum norm of the defect 
//
// /WARNING: Calling this routine will change defectMG.multigridLevel[level][grid]
//
// /approximationStride : use approximately this subset of points in each direction. If a given
//    direction only has a few points then this value is reduced. If approximationStride=1 then
//    this will be the true norm.
//
//\end{OgmgInclude.tex} 
//---------------------------------------------------------------------------------------------
{
  const int option=1;  // max-norm
  real norm=0.;
  CompositeGrid & mgcg = multigridCompositeGrid();
  for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
  {
    norm=max(norm,defectNorm(level,grid,option,approximationStride));
  }
  return norm;
}


//\begin{>>OgmgInclude.tex}{\subsection{defect(level,grid)}}
real Ogmg::
defectNorm(const int & level, const int & grid, int option /* =0 */, int approximationStride /* =8 */ )
//---------------------------------------------------------------------------------------------
// /Description:
//    Compute a norm of the defect (optionally approximate, using a subset of the total points)
//
// /WARNING: Calling this routine will change defectMG.multigridLevel[level][grid]
//
// /option: 0=get l2-norm, 1=get max-norm
// /approximationStride : use approximately this subset of points in each direction. If a given
//    direction only has a few points then this value is reduced. If approximationStride=1 then
//    this will be the true norm.
//
//\end{OgmgInclude.tex} 
//---------------------------------------------------------------------------------------------
{
  real time=getCPU();

  realArray & u = uMG.multigridLevel[level][grid];
  realArray & f = fMG.multigridLevel[level][grid];     // *** use f at level 1!
  //  realMappedGridFunction & c =  level==0 ? cMG[grid] : cMG.multigridLevel[level][grid];
  CompositeGrid & mgcg = multigridCompositeGrid();
  MappedGrid & mg = mgcg.multigridLevel[level][grid];  
  realArray & defect = defectMG.multigridLevel[level][grid];

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  getIndex(mg.extendedIndexRange(),I1,I2,I3);  // this may be ok 
  // getIndex(mg.gridIndexRange(),I1,I2,I3);         // *wdh* 100716

  real fractionOfPointsComputed=1.;
  
  if( approximationStride<1 )
  {
    Overture::abort("Ogmg:defectNorm:ERROR: invalid approximationStride");
  }

  if( false )
  {
    ::display(f,sPrintF(" defectNorm: f, level=%i, grid=%i",level,grid),"%10.2e ");
    OV_ABORT("stop here");
  }
  

  // do NOT compute the defect on the boundary with dirichlet BC's
  const int numGhost = orderOfAccuracy/2; // *wdh* 100716
  for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
  {
    int ia=Iv[axis].getBase();
    int ib=Iv[axis].getBound();
      
    if( boundaryCondition(0,axis,grid)==OgmgParameters::extrapolate  )
      ia+=numGhost;
    if( boundaryCondition(1,axis,grid)==OgmgParameters::extrapolate  )
      ib-=numGhost;

    int stride=approximationStride;
    if( ib-ia < 4 )
      stride=1;
    else if( ib-ia < 8 )
      stride=min(stride,2);
    else if( ib-ia < 20 )
      stride=min(stride,3);
    else if( ib-ia < 40 )
      stride=min(stride,4);

    ib = ib - ((ib-ia) % stride); // *wdh* 100716 

    Iv[axis]=Range(ia,ib,stride);
    // printf("defectNorm: level=%i grid=%i axis=%i [ia,ib,stride]=[%i,%i,%i]\n",level,grid,axis,ia,ib,stride);
    

    fractionOfPointsComputed*=1./stride;
  }
  
  int defectOption= option==0 ? 1 : 2;
  real norm;
  norm=getDefect(level,grid,f,u,I1,I2,I3,defect,-1,defectOption);

  // Make this a function to call:
  int totalGridPoints=0; 
  int myGridPoints=0;
  for( int gg=0; gg<mgcg.multigridLevel[level].numberOfComponentGrids(); gg++ )
  {
    const IntegerArray & gid = mgcg.multigridLevel[level][gg].gridIndexRange();
    int numGridPoints=(gid(1,0)-gid(0,0)+1)*(gid(1,1)-gid(0,1)+1)*(gid(1,2)-gid(0,2)+1);
    totalGridPoints+=numGridPoints;
    if( gg==grid ) myGridPoints=numGridPoints;
  }
  workUnits(level)+=(fractionOfPointsComputed*myGridPoints)/max(1,totalGridPoints);

  if( Ogmg::debug & 16 )
  {
    printF("defectNorm: level=%i grid=%i %s-norm= %e \n",level,grid,(option==0 ? "l2" : "max"),norm);
  }
  tm[timeForDefectNorm]+=getCPU()-time;

  return norm;
}


#define defectOpt EXTERN_C_NAME(defectopt)

extern "C"
{

  void defectOpt( const int &nd,  const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
                       const int &nd3a, const int &nd3b,
		       const int &n1a, const int &n1b, const int &n1c,
                       const int &n2a, const int &n2b, const int &n2c,
                       const int &n3a, const int &n3b, const int &n3c, 
                       const int &ndc, const real & defect, const real & f, const real & c,
                       const real & u, const int & mask, 
                       const real & cc, const real & varCoeff, int & ipar, real & rpar );

}

//\begin{>>OgmgInclude.tex}{\subsection{getDefect}}
real Ogmg::
getDefect(const int & level, 
	  const int & grid, 
	  realArray & f,     
	  realArray & u, 
          const Index & I1,
          const Index & I2,
          const Index & I3,
	  realArray & defect,
          const int lineSmoothOption /* = -1 */,
          const int defectOption /* = 0 */,
          real & defectL2Norm, real & defectMaxNorm )
//==================================================================================
// /Description:
//    Defect computation on a component grid
//
//   Determine the defect = f - C*u
//
//   This routine knows how to efficiently compute the defect for rectangular
//   and non-rectangular grids. It also knows how to compute the defect for
//   line smoothers
// Input -
//   level,grid
//   f,u
//   I1,I2,I3
//   lineSmoothOption = -1 :
//                    = 0,1,2 : compute defect for line solve in direction 0,1,2
//   defectOption = 0 : compute defect
//                  1  : compute defect and l2 norm
//                  2  : compute defect and max norm
// Output -
//   defect
//
//  /Return Value: l2-norm or max norm of the defect depending on defectOption
//
//\end{OgmgInclude.tex} 
//==================================================================================
{
  realMappedGridFunction & c =   level==0 ? cMG[grid] : cMG.multigridLevel[level][grid];
  CompositeGrid & mgcg = multigridCompositeGrid();
  MappedGrid & mg = mgcg.multigridLevel[level][grid];  
  const int numberOfDimensions = mg.numberOfDimensions();
  
  real defectNorm=0.;
  
  if( true && parameters.useOptimizedVersion )
  {
    const IntegerArray & d = mg.dimension();

    int nab[3][3], &n1a=nab[0][0], &n1b=nab[1][0], &n1c=nab[2][0],
                   &n2a=nab[0][1], &n2b=nab[1][1], &n2c=nab[2][1],
                   &n3a=nab[0][2], &n3b=nab[1][2], &n3c=nab[2][2];

    n1a = I1.getBase();
    n1b = I1.getBound();
    n1c = I1.getStride();
    n2a = I2.getBase();
    n2b = I2.getBound();
    n2c = I2.getStride();
    n3a = I3.getBase();
    n3b = I3.getBound();
    n3c = I3.getStride();

    const int ndc=c.getLength(0);

    const bool rectangular=(*c.getOperators()).isRectangular() &&
                        ( level < mgcg.numberOfMultigridLevels()-numberOfExtraLevels ); 

    // ** const bool sparseStencil=rectangular && assumeSparseStencilForRectangularGrids;
    
    // const int general=0, sparse=1, constantCoeff=2, sparseConstantCoefficients=3;
   
    real dx[3]={1.,1.,1.};
    int sparseStencil=general;
    if( mg.isRectangular() ) 
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

    intArray & mask = mg.mask();
    #ifdef USE_PPP
      realSerialArray defectLocal; getLocalArrayWithGhostBoundaries(defect,defectLocal);
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
      realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f,fLocal);
      realSerialArray cLocal; getLocalArrayWithGhostBoundaries(c,cLocal);
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal); 

      realArray & varCoeffd = varCoeff!=NULL ? (*varCoeff).multigridLevel[level][grid] : u;
      realSerialArray varCoeffLocal; getLocalArrayWithGhostBoundaries(varCoeffd,varCoeffLocal);  
      real *up=getDataPointer(uLocal);
      real *vcp = getDataPointer(varCoeffLocal);

      IntegerArray dd(2,3);
      for( int axis=0; axis<3; axis++ )
      {
	dd(0,axis)=maskLocal.getBase(axis);  // use the one with getLocalArrayWithGhostBoundaries
	dd(1,axis)=maskLocal.getBound(axis);
      }
      
      
    #else
      const realSerialArray & defectLocal = defect;
      const realSerialArray & uLocal = u;
      const realSerialArray & fLocal = f;
      const realSerialArray & cLocal = c;
      const intSerialArray & maskLocal = mask;
      const IntegerArray & dd = d;
      real *up=getDataPointer(uLocal);
      real *vcp = varCoeff!=NULL ? getDataPointer((*varCoeff).multigridLevel[level][grid]) : up;
    #endif


    // *wdh* 100617 -- restrict bounds in serial too (needed for order=4)
    // restrict index bounds to the local array bounds and adjust for stride.  *wdh* 100104 
    if( lineSmoothOption>=0 )
    {
      // For the line smooth, evaluate the defect on as many parallel ghost as possible
      for( int axis=0; axis<3; axis++ )
      {
	int hw = axis<numberOfDimensions ? (orderOfAccuracy)/2 : 0;  // stencil half-width
	int ia = nab[0][axis], ib=nab[1][axis], stride=nab[2][axis];
	ia = max(ia,maskLocal.getBase(axis) +hw);
	// adjust ia so it offset by a factor of stride from ia0
	// ia += (stride - (ia-nab[0][axis])%stride)%stride;  // the offset should be positive *wdh* 100617
	ia += (stride - (ia-nab[0][axis])%stride + stride*hw )%stride;  // the offset should be positive *wdh* 100617
	assert( (ia-nab[0][axis])%stride == 0 && ia>=maskLocal.getBase(axis) +hw );
	ib = min(ib,maskLocal.getBound(axis)-hw);
	// adjust ib so it offset by a factor of stride from ib0
	// ib -= (stride - (nab[1][axis]-ib)%stride)%stride;  
	ib -= (stride - (nab[1][axis]-ib)%stride + stride*hw)%stride;   // the offset should be positive *wdh* 100617
	assert( (nab[1][axis]-ib)%stride == 0 && ib <= maskLocal.getBound(axis)-hw );

	nab[0][axis]=ia;
	nab[1][axis]=ib;
	// printF(" lineSmoothOption=%i axis=%i hw=%i ia=%i ib=%i maskLocal=[%i,%i]\n",
	//       lineSmoothOption,axis, hw, ia,ib,maskLocal.getBase(axis),maskLocal.getBound(axis));
      }
    }
    else
    {
     #ifdef USE_PPP
      // n1a = max(n1a,maskLocal.getBase(0)  +mask.getGhostBoundaryWidth(0));
      // n1b = min(n1b,maskLocal.getBound(0) -mask.getGhostBoundaryWidth(0));
      // n2a = max(n2a,maskLocal.getBase(1)  +mask.getGhostBoundaryWidth(1));
      // n2b = min(n2b,maskLocal.getBound(1) -mask.getGhostBoundaryWidth(1));
      // n3a = max(n3a,maskLocal.getBase(2)  +mask.getGhostBoundaryWidth(2));
      // n3b = min(n3b,maskLocal.getBound(2) -mask.getGhostBoundaryWidth(2));

      // -- when restricting bounds in parallel with a stride we need to make sure that maintain
      //    the same set of strided points on all processors -- *wdh* 100928 

      for( int axis=0; axis<3; axis++ )
      {
	int ia = nab[0][axis], ib=nab[1][axis], stride=nab[2][axis];

        ia = max(ia,maskLocal.getBase(axis) +mask.getGhostBoundaryWidth(axis));
	// adjust ia so it offset by a factor of stride from the original ia 
        ia += (stride - (ia-nab[0][axis])%stride + stride )%stride;
	assert( (ia-nab[0][axis])%stride == 0 && ia>=maskLocal.getBase(axis) +mask.getGhostBoundaryWidth(axis));

	ib = min(ib,maskLocal.getBound(axis)-mask.getGhostBoundaryWidth(axis));
	// adjust ib so it offset by a factor of stride from ib0
	ib -= (stride - (nab[1][axis]-ib)%stride + stride)%stride;  
	assert( (nab[1][axis]-ib)%stride == 0 && ib <= maskLocal.getBound(axis)-mask.getGhostBoundaryWidth(axis) );

	nab[0][axis]=ia;
	nab[1][axis]=ib;
      }
     #endif
      
    }

    int ipar[10]={defectOption,lineSmoothOption,orderOfAccuracy,sparseStencil,0,0,0,0,0,0};  //
    real rpar[10]={dx[0],dx[1],dx[2],0.,0.,0.,0.,0.,0.,0.};  //
    // ::display(constantCoefficients,"constantCoefficients");
    const real *pcc = constantCoefficients.getBound(2)>=level ? &constantCoefficients(0,grid,level) : rpar;

    defectOpt( mg.numberOfDimensions(), dd(0,0),dd(1,0),dd(0,1),dd(1,1),dd(0,2),dd(1,2),
	       n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, 
	       *getDataPointer(defectLocal),
	       *getDataPointer(fLocal),
	       *getDataPointer(cLocal),
	       *up,
               *getDataPointer(maskLocal),
               *pcc,*vcp,ipar[0],rpar[0] );

    // *wdh* 100928 - changed to return square of defect and count so we can compute in parallel 
    real defectSquared = rpar[3];
    real count         = rpar[4];
    defectMaxNorm      = rpar[5];
    #ifdef USE_PPP
      defectSquared = ParallelUtility::getSum(defectSquared); 
      count         = ParallelUtility::getSum(count); 
      defectMaxNorm = ParallelUtility::getMaxValue(defectMaxNorm);
    #endif

    defectL2Norm=sqrt( defectSquared/max(1.,count) );

    defectNorm=max(defectL2Norm,defectMaxNorm);

    // printf("getDefect:defectOption=%i  defectL2Norm=%8.2e defectMaxNorm=%8.2e defectNorm=%8.2e\n",
    //   defectOption,defectL2Norm,defectMaxNorm,defectNorm);
    
    // printf(" n1=[%i,%i,%i] n2=[%i,%i,%i] n3=[%i,%i,%i] \n",n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c);
    
    // ::display(defect,"defect from opt","%7.1e ");
  }
  else
  {
    u.reshape(1,u.dimension(0),u.dimension(1),u.dimension(2));
    defect.reshape(1,defect.dimension(0),defect.dimension(1),defect.dimension(2));
    f.reshape(1,f.dimension(0),f.dimension(1),f.dimension(2));
  
    evaluateTheDefectFormula(level,grid,c,u,f,defect,mg,I1,I2,I3,I1,I2,I3,lineSmoothOption);

    u.reshape(u.dimension(1),u.dimension(2),u.dimension(3));
    defect.reshape(defect.dimension(1),defect.dimension(2),defect.dimension(3));
    f.reshape(f.dimension(1),f.dimension(2),f.dimension(3));
    
    // ::display(defect,"defect from OLD","%7.1e ");

    // @PD realArray3[defect] Range[I1,I2,I3]


    // ------------ zero out unused points -----------------
    // *wdh* this does seem to be needed
    where( mg.mask()(I1,I2,I3)<=0 )  
    {
      defect(I1,I2,I3)=0.;  //  @PAW
    }
  }

  return defectNorm;
}

// @PD realArray4[defect,f,c,u] Range[I1,I2,I3,I1u,I2u,I3u] 


void Ogmg::
evaluateTheDefectFormula(const int & level, 
			 const int & grid, 
			 const realArray & c,
			 const realArray & u,  
			 const realArray & f, 
			 realArray & defect, 
			 MappedGrid & mg,
			 const Index & I1,
			 const Index & I2,
			 const Index & I3,
			 const Index & I1u,
			 const Index & I2u,
			 const Index & I3u,
                         const int lineSmoothOption)
// ==================================================================================================
//   /Description:
//     This routine actually evaluates the defect for various cases, lineSmooth or not. It can be used
//     to evaluate the defect on neumann BC's using I1u,I2u,I3u.
//
// /I1,I2,I3 (input) : evaluate defect, f and c at these Index's
// /I1u,I2u,I3u (input) : evaluate u here
// /lineSmoothOption (input) : if lineSmoothOption==0,1,2 then we require the defect for a line smooth
//   in direction 0,1,2. If lineSmoothOption==-1 then we need the full defect.
// ==================================================================================================
{
          
  // ****************** Defect is wrong for extrapolation equations on ghost lines *****************
  CompositeGrid & mgcg = multigridCompositeGrid();
  realMappedGridFunction & cmg = level==0 ? cMG[grid] : cMG.multigridLevel[level][grid];

  // The matrix generated on extra levels by operator averaging will have a full stencil
  // and thus will not count as "rectangular"
  const int rectangular=(*cmg.getOperators()).isRectangular() &&
                        ( level < mgcg.numberOfMultigridLevels()-numberOfExtraLevels ); 
  // const int rectangular=FALSE;
  if( Ogmg::debug & 8 )
    fPrintF(debugFile,"%*.1s evaluateTheDefectFormula:level=%i, grid=%i, rectangular=%i \n",level*4," ",
           level,grid,rectangular);
  if( Ogmg::debug & 64 )
  {
    if( rectangular )
      display(isConstantCoefficients,"isConstantCoefficients",debugFile);
    if( rectangular && isConstantCoefficients(grid) )
    {
      
      display(constantCoefficients,sPrintF(buff,"constantCoefficients for level=%i ",level),debugFile);
    }
  }
  


  if( lineSmoothOption<-1 || lineSmoothOption >= mg.numberOfDimensions() )
  {
    cout << "Ogmg::getDefect:ERROR invalid value for lineSmoothOption=" << lineSmoothOption << endl;
    exit(1);
  }

  if( mg.numberOfDimensions()==2 )
  {
    if( rectangular && assumeSparseStencilForRectangularGrids )
    {

      // Here we can assume that the operator is a 5-point  operator on a rectangular grid

      if( isConstantCoefficients(grid) )
      {
	if( lineSmoothOption==-1 )
	{ // general defect
	  defect(0,I1,I2,I3)=f(0,I1,I2,I3)-(
	    constantCoefficients(M123( 0, 0,0),grid)*u(0,I1u  ,I2u  ,I3u)+   // **** could be wrong, assumes 5 pt
	    constantCoefficients(M123( 1, 0,0),grid)*u(0,I1u+1,I2u  ,I3u)+
	    constantCoefficients(M123( 0, 1,0),grid)*u(0,I1u  ,I2u+1,I3u)+
	    constantCoefficients(M123(-1, 0,0),grid)*u(0,I1u-1,I2u  ,I3u)+
	    constantCoefficients(M123( 0,-1,0),grid)*u(0,I1u  ,I2u-1,I3u)
	    );
	}
	else if( lineSmoothOption==0 )
	{ // defect for line smooth in direction 0
	  defect(0,I1,I2,I3)=f(0,I1,I2,I3)-(
	    c(M123( 0, 1,0),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u)+
	    c(M123( 0,-1,0),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u)
	    );
	}
	else if( lineSmoothOption==1 )
	{ // defect for line smooth in direction 1
	  defect(0,I1,I2,I3)=f(0,I1,I2,I3)-(
	    c(M123( 1, 0,0),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u)+
	    c(M123(-1, 0,0),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u)
	    );
	}
	else
	{
	  throw "error";
	}
        // **** do any boundaries ****
        Index Ib1,Ib2,Ib3;
        int side,axis;
	for( axis=0; axis<mg.numberOfDimensions(); axis++ )
	{
	  for( side=0; side<=1; side++ )
	  {
	    if( boundaryCondition(side,axis)>0 && boundaryCondition(side,axis)==OgmgParameters::extrapolate )
	    {
	      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	      defect(0,Ib1,Ib2,Ib3)=f(0,Ib1,Ib2,Ib3)-(
		c(M123( 0, 0,0),Ib1,Ib2,Ib3)*u(0,Ib1  ,Ib2  ,Ib3)+
		c(M123( 1, 0,0),Ib1,Ib2,Ib3)*u(0,Ib1+1,Ib2  ,Ib3)+
		c(M123( 0, 1,0),Ib1,Ib2,Ib3)*u(0,Ib1  ,Ib2+1,Ib3)+
		c(M123(-1, 0,0),Ib1,Ib2,Ib3)*u(0,Ib1-1,Ib2  ,Ib3)+
		c(M123( 0,-1,0),Ib1,Ib2,Ib3)*u(0,Ib1  ,Ib2-1,Ib3)
		);
	    }
	  }
	}
      }
      else
      {
        // ============== non constant coefficient rectangular case ===============

        if( level>=2 && debug & 16 )
	{
	  display(u,sPrintF(buff," evaluate defect level=%i : u",level),debugFile,"%9.1e");
	  display(f,sPrintF(buff," evaluate defect level=%i : f",level),debugFile,"%9.1e");
	  display(c,sPrintF(buff," evaluate defect level=%i : c",level),debugFile,"%9.1e");
	}
	

	if( lineSmoothOption==-1 )
	{ // general defect
	  defect(0,I1,I2,I3)=f(0,I1,I2,I3)-(                     // @PA
	    c(M123( 0, 0,0),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u)+
	    c(M123( 1, 0,0),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u)+
	    c(M123( 0, 1,0),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u)+
	    c(M123(-1, 0,0),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u)+
	    c(M123( 0,-1,0),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u)
	    );
/* ----
            const int i1Bound=I1.getBound();
            const int i2Bound=I2.getBound();
            const int i3Bound=I3.getBound();
            const int 
	    for( int i3=I3.getBase(); i3<i3Bound; i3+=stride3 )
	    for( int i2=I2.getBase(); i2<i2Bound; i2+=stride2 )
	    for( int i1=I1.getBase(); i1<i1Bound; i1+=stride1 )
	    {
	      defect(0,I1,I2,I3)=f(0,I1,I2,I3)-(
		c(M123( 0, 0,0),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u)+
		c(M123( 1, 0,0),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u)+
		c(M123( 0, 1,0),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u)+
		c(M123(-1, 0,0),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u)+
		c(M123( 0,-1,0),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u)
		);
	    }
----- */    
	}
	else if( lineSmoothOption==0 )
	{ // defect for line smooth in direction 0
	  defect(0,I1,I2,I3)=f(0,I1,I2,I3)-(               // @PA
	     c(M123( 0, 1,0),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u)
	    +c(M123( 0,-1,0),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u)
	    );
	}
	else if( lineSmoothOption==1 )
	{ // defect for line smooth in direction 1
	  defect(0,I1,I2,I3)=f(0,I1,I2,I3)-(               // @PA
	     c(M123( 1, 0,0),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u)
	    +c(M123(-1, 0,0),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u)
	    );
	}
	else
	{
	  throw "error";
	}
      }
    }
    else  // ----- not rectangular -----
    {
      if( lineSmoothOption==-1 )
      { // general 3x3 defect
	defect(0,I1,I2,I3)=f(0,I1,I2,I3)-(                  // @PA
	   c(M123( 0, 0,0),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u)
	  +c(M123( 1, 0,0),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u)
	  +c(M123( 0, 1,0),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u)
	  +c(M123(-1, 0,0),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u)
	  +c(M123( 0,-1,0),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u)
	  +c(M123( 1, 1,0),I1,I2,I3)*u(0,I1u+1,I2u+1,I3u)
	  +c(M123( 1,-1,0),I1,I2,I3)*u(0,I1u+1,I2u-1,I3u)
	  +c(M123(-1, 1,0),I1,I2,I3)*u(0,I1u-1,I2u+1,I3u)
	  +c(M123(-1,-1,0),I1,I2,I3)*u(0,I1u-1,I2u-1,I3u)
	  );
      }
      else if( lineSmoothOption==0 )
      { // defect for line smooth in direction 0
 	defect(0,I1,I2,I3)=f(0,I1,I2,I3)-(                  // @PA
	   c(M123( 0, 1,0),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u)
	  +c(M123( 0,-1,0),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u)
	  +c(M123( 1, 1,0),I1,I2,I3)*u(0,I1u+1,I2u+1,I3u)
	  +c(M123( 1,-1,0),I1,I2,I3)*u(0,I1u+1,I2u-1,I3u)
	  +c(M123(-1, 1,0),I1,I2,I3)*u(0,I1u-1,I2u+1,I3u)
	  +c(M123(-1,-1,0),I1,I2,I3)*u(0,I1u-1,I2u-1,I3u)
	  );
      }
      else if( lineSmoothOption==1 )
      { // defect for line smooth in direction 1
	defect(0,I1,I2,I3)=f(0,I1,I2,I3)-(                  // @PA
	   c(M123( 1, 0,0),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u)
	  +c(M123(-1, 0,0),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u)
	  +c(M123( 1, 1,0),I1,I2,I3)*u(0,I1u+1,I2u+1,I3u)
	  +c(M123( 1,-1,0),I1,I2,I3)*u(0,I1u+1,I2u-1,I3u)
	  +c(M123(-1, 1,0),I1,I2,I3)*u(0,I1u-1,I2u+1,I3u)
	  +c(M123(-1,-1,0),I1,I2,I3)*u(0,I1u-1,I2u-1,I3u)
	  );
      }
      else
      {
        cout << "Ogmg:evaluateTheDefectFormula:ERROR: invalid lineSmoothOption \n";
        Overture::abort();
      }
      
    }
  }
  else // ---- 3D -----
  {

    if( rectangular && assumeSparseStencilForRectangularGrids )
    {
      // Here we can assume that the operator is a 7-point  operator on a rectangular grid

      if( lineSmoothOption==-1 )
      { // general defect
	defect(0,I1,I2,I3)=f(0,I1,I2,I3)-(                    // @PA
	   c(M123( 0, 0, 0),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u  )
	  +c(M123( 1, 0, 0),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u  )
	  +c(M123( 0, 1, 0),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u  )
	  +c(M123(-1, 0, 0),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u  )
	  +c(M123( 0,-1, 0),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u  )
	  +c(M123( 0, 0, 1),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u+1)
	  +c(M123( 0, 0,-1),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u-1)
	  );
      }
      else if( lineSmoothOption==0 )
      { // defect for line smooth in direction 0
	defect(0,I1,I2,I3)=f(0,I1,I2,I3)-(                         // @PA
	  // c(M123( 0, 0, 0),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u  )
	  // +c(M123( 1, 0, 0),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u  )
	   c(M123( 0, 1, 0),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u  )
	  // +c(M123(-1, 0, 0),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u  )
	  +c(M123( 0,-1, 0),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u  )
	  +c(M123( 0, 0, 1),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u+1)
	  +c(M123( 0, 0,-1),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u-1)
	  );
      }
      else if( lineSmoothOption==1 )
      { // defect for line smooth in direction 1
	defect(0,I1,I2,I3)=f(0,I1,I2,I3)-(                         // @PA
	  // c(M123( 0, 0, 0),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u  )
	   c(M123( 1, 0, 0),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u  )
	  // +c(M123( 0, 1, 0),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u  )
	  +c(M123(-1, 0, 0),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u  )
	  // +c(M123( 0,-1, 0),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u  )
	  +c(M123( 0, 0, 1),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u+1)
	  +c(M123( 0, 0,-1),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u-1)
	  );
      }
      else if( lineSmoothOption==2 )
      { // defect for line smooth in direction 2
	defect(0,I1,I2,I3)=f(0,I1,I2,I3)-(                         // @PA
	  // c(M123( 0, 0, 0),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u  )
	   c(M123( 1, 0, 0),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u  )
	  +c(M123( 0, 1, 0),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u  )
	  +c(M123(-1, 0, 0),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u  )
	  +c(M123( 0,-1, 0),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u  )
	  // +c(M123( 0, 0, 1),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u+1)
	  // +c(M123( 0, 0,-1),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u-1)
	  );
      }
      else
      {
        cout << "Ogmg:evaluateTheDefectFormula:ERROR: invalid lineSmoothOption \n";
        Overture::abort();
      }
    }
    else
    {
      // ==================== non rectangular ================
      if( lineSmoothOption==-1 )
      { // general defect
	defect(0,I1,I2,I3)=f(0,I1,I2,I3)-(                    // @PA
	   c(M123(-1,-1,-1),I1,I2,I3)*u(0,I1u-1,I2u-1,I3u-1)
	  +c(M123( 0,-1,-1),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u-1)
	  +c(M123( 1,-1,-1),I1,I2,I3)*u(0,I1u+1,I2u-1,I3u-1)
	  +c(M123(-1, 0,-1),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u-1)
	  +c(M123( 0, 0,-1),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u-1)
	  +c(M123( 1, 0,-1),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u-1)
	  +c(M123(-1, 1,-1),I1,I2,I3)*u(0,I1u-1,I2u+1,I3u-1)
	  +c(M123( 0, 1,-1),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u-1)
	  +c(M123( 1, 1,-1),I1,I2,I3)*u(0,I1u+1,I2u+1,I3u-1)

	  +c(M123(-1,-1, 0),I1,I2,I3)*u(0,I1u-1,I2u-1,I3u  )
	  +c(M123( 0,-1, 0),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u  )
	  +c(M123( 1,-1, 0),I1,I2,I3)*u(0,I1u+1,I2u-1,I3u  )
	  +c(M123(-1, 0, 0),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u  )
	  +c(M123( 0, 0, 0),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u  )
	  +c(M123( 1, 0, 0),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u  )
	  +c(M123(-1, 1, 0),I1,I2,I3)*u(0,I1u-1,I2u+1,I3u  )
	  +c(M123( 0, 1, 0),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u  )
	  +c(M123( 1, 1, 0),I1,I2,I3)*u(0,I1u+1,I2u+1,I3u  )

	  +c(M123(-1,-1, 1),I1,I2,I3)*u(0,I1u-1,I2u-1,I3u+1)
	  +c(M123( 0,-1, 1),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u+1)
	  +c(M123( 1,-1, 1),I1,I2,I3)*u(0,I1u+1,I2u-1,I3u+1)
	  +c(M123(-1, 0, 1),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u+1)
	  +c(M123( 0, 0, 1),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u+1)
	  +c(M123( 1, 0, 1),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u+1)
	  +c(M123(-1, 1, 1),I1,I2,I3)*u(0,I1u-1,I2u+1,I3u+1)
	  +c(M123( 0, 1, 1),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u+1)
	  +c(M123( 1, 1, 1),I1,I2,I3)*u(0,I1u+1,I2u+1,I3u+1)
	  );
      }
      else if( lineSmoothOption==0 )
      { // defect for line smooth in direction 0
	defect(0,I1,I2,I3)=f(0,I1,I2,I3)-(                   // @PA
	   c(M123(-1,-1,-1),I1,I2,I3)*u(0,I1u-1,I2u-1,I3u-1)
	  +c(M123( 0,-1,-1),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u-1)
	  +c(M123( 1,-1,-1),I1,I2,I3)*u(0,I1u+1,I2u-1,I3u-1)
	  +c(M123(-1, 0,-1),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u-1)
	  +c(M123( 0, 0,-1),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u-1)
	  +c(M123( 1, 0,-1),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u-1)
	  +c(M123(-1, 1,-1),I1,I2,I3)*u(0,I1u-1,I2u+1,I3u-1)
	  +c(M123( 0, 1,-1),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u-1)
	  +c(M123( 1, 1,-1),I1,I2,I3)*u(0,I1u+1,I2u+1,I3u-1)

	  +c(M123(-1,-1, 0),I1,I2,I3)*u(0,I1u-1,I2u-1,I3u  )
	  +c(M123( 0,-1, 0),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u  )
	  +c(M123( 1,-1, 0),I1,I2,I3)*u(0,I1u+1,I2u-1,I3u  )
	   // +c(M123(-1, 0, 0),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u  )
	   // +c(M123( 0, 0, 0),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u  )
	   // +c(M123( 1, 0, 0),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u  )
	  +c(M123(-1, 1, 0),I1,I2,I3)*u(0,I1u-1,I2u+1,I3u  )
	  +c(M123( 0, 1, 0),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u  )
	  +c(M123( 1, 1, 0),I1,I2,I3)*u(0,I1u+1,I2u+1,I3u  )

	  +c(M123(-1,-1, 1),I1,I2,I3)*u(0,I1u-1,I2u-1,I3u+1)
	  +c(M123( 0,-1, 1),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u+1)
	  +c(M123( 1,-1, 1),I1,I2,I3)*u(0,I1u+1,I2u-1,I3u+1)
	  +c(M123(-1, 0, 1),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u+1)
	  +c(M123( 0, 0, 1),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u+1)
	  +c(M123( 1, 0, 1),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u+1)
	  +c(M123(-1, 1, 1),I1,I2,I3)*u(0,I1u-1,I2u+1,I3u+1)
	  +c(M123( 0, 1, 1),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u+1)
	  +c(M123( 1, 1, 1),I1,I2,I3)*u(0,I1u+1,I2u+1,I3u+1)
	  );
      }
      else if( lineSmoothOption==1 )
      { // defect for line smooth in direction 1
	defect(0,I1,I2,I3)=f(0,I1,I2,I3)-(                   // @PA
	   c(M123(-1,-1,-1),I1,I2,I3)*u(0,I1u-1,I2u-1,I3u-1)
	  +c(M123( 0,-1,-1),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u-1)
	  +c(M123( 1,-1,-1),I1,I2,I3)*u(0,I1u+1,I2u-1,I3u-1)
	  +c(M123(-1, 0,-1),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u-1)
	  +c(M123( 0, 0,-1),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u-1)
	  +c(M123( 1, 0,-1),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u-1)
	  +c(M123(-1, 1,-1),I1,I2,I3)*u(0,I1u-1,I2u+1,I3u-1)
	  +c(M123( 0, 1,-1),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u-1)
	  +c(M123( 1, 1,-1),I1,I2,I3)*u(0,I1u+1,I2u+1,I3u-1)

	  +c(M123(-1,-1, 0),I1,I2,I3)*u(0,I1u-1,I2u-1,I3u  )
	   // +c(M123( 0,-1, 0),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u  )
	  +c(M123( 1,-1, 0),I1,I2,I3)*u(0,I1u+1,I2u-1,I3u  )
	  +c(M123(-1, 0, 0),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u  )
	   // +c(M123( 0, 0, 0),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u  )
	  +c(M123( 1, 0, 0),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u  )
	  +c(M123(-1, 1, 0),I1,I2,I3)*u(0,I1u-1,I2u+1,I3u  )
	   // +c(M123( 0, 1, 0),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u  )
	  +c(M123( 1, 1, 0),I1,I2,I3)*u(0,I1u+1,I2u+1,I3u  )

	  +c(M123(-1,-1, 1),I1,I2,I3)*u(0,I1u-1,I2u-1,I3u+1)
	  +c(M123( 0,-1, 1),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u+1)
	  +c(M123( 1,-1, 1),I1,I2,I3)*u(0,I1u+1,I2u-1,I3u+1)
	  +c(M123(-1, 0, 1),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u+1)
	  +c(M123( 0, 0, 1),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u+1)
	  +c(M123( 1, 0, 1),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u+1)
	  +c(M123(-1, 1, 1),I1,I2,I3)*u(0,I1u-1,I2u+1,I3u+1)
	  +c(M123( 0, 1, 1),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u+1)
	  +c(M123( 1, 1, 1),I1,I2,I3)*u(0,I1u+1,I2u+1,I3u+1)
	  );
      }
      else if( lineSmoothOption==2 )
      { // defect for line smooth in direction 1
	defect(0,I1,I2,I3)=f(0,I1,I2,I3)-(                   // @PA
	   c(M123(-1,-1,-1),I1,I2,I3)*u(0,I1u-1,I2u-1,I3u-1)
	  +c(M123( 0,-1,-1),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u-1)
	  +c(M123( 1,-1,-1),I1,I2,I3)*u(0,I1u+1,I2u-1,I3u-1)
	  +c(M123(-1, 0,-1),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u-1)
	   // +c(M123( 0, 0,-1),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u-1)
	  +c(M123( 1, 0,-1),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u-1)
	  +c(M123(-1, 1,-1),I1,I2,I3)*u(0,I1u-1,I2u+1,I3u-1)
	  +c(M123( 0, 1,-1),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u-1)
	  +c(M123( 1, 1,-1),I1,I2,I3)*u(0,I1u+1,I2u+1,I3u-1)

	  +c(M123(-1,-1, 0),I1,I2,I3)*u(0,I1u-1,I2u-1,I3u  )
	  +c(M123( 0,-1, 0),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u  )
	  +c(M123( 1,-1, 0),I1,I2,I3)*u(0,I1u+1,I2u-1,I3u  )
	  +c(M123(-1, 0, 0),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u  )
	   // +c(M123( 0, 0, 0),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u  )
	  +c(M123( 1, 0, 0),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u  )
	  +c(M123(-1, 1, 0),I1,I2,I3)*u(0,I1u-1,I2u+1,I3u  )
	  +c(M123( 0, 1, 0),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u  )
	  +c(M123( 1, 1, 0),I1,I2,I3)*u(0,I1u+1,I2u+1,I3u  )

	  +c(M123(-1,-1, 1),I1,I2,I3)*u(0,I1u-1,I2u-1,I3u+1)
	  +c(M123( 0,-1, 1),I1,I2,I3)*u(0,I1u  ,I2u-1,I3u+1)
	  +c(M123( 1,-1, 1),I1,I2,I3)*u(0,I1u+1,I2u-1,I3u+1)
	  +c(M123(-1, 0, 1),I1,I2,I3)*u(0,I1u-1,I2u  ,I3u+1)
	   // +c(M123( 0, 0, 1),I1,I2,I3)*u(0,I1u  ,I2u  ,I3u+1)
	  +c(M123( 1, 0, 1),I1,I2,I3)*u(0,I1u+1,I2u  ,I3u+1)
	  +c(M123(-1, 1, 1),I1,I2,I3)*u(0,I1u-1,I2u+1,I3u+1)
	  +c(M123( 0, 1, 1),I1,I2,I3)*u(0,I1u  ,I2u+1,I3u+1)
	  +c(M123( 1, 1, 1),I1,I2,I3)*u(0,I1u+1,I2u+1,I3u+1)
	  );
      }
      else
      {
        cout << "Ogmg:evaluateTheDefectFormula:ERROR: invalid lineSmoothOption \n";
        Overture::abort();
      }

    }
  }
}

#undef C
#undef M123

