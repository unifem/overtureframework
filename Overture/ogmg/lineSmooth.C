// This file automatically generated from lineSmooth.bC with bpp.
#include "Ogmg.h"
#include "TridiagonalSolver.h"
#include "ParallelUtility.h"

static int numberOfLinesSolves=0;

#define lineSmoothBuild EXTERN_C_NAME(linesmoothbuild)
#define lineSmoothRHS EXTERN_C_NAME(linesmoothrhs)
#define lineSmoothUpdate EXTERN_C_NAME(linesmoothupdate)
extern "C"
{
  void lineSmoothBuild(const int&nd,
                                            const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
                                const int&nda1a,const int&nda1b,const int&nda2a,const int&nda2b,const int&nda3a,const int&nda3b,
            		const int&ndc, const real&coeff, const real&cc, 
                                real&a, real&b, real&c, real&d, real&e, const real&s, const real&u, const real&f, const int&mask, 
                                  const real&rsxy, const int&ipar, const real&rpar, const int&ndbcd, const real&bcData );

void lineSmoothRHS( const int&nd,
                		    const int&nd1a,const int&nd1b,
                		    const int&nd2a,const int&nd2b,
                		    const int&nd3a,const int&nd3b,
                		    const int&ndc, const real&coeff, const real&cc,  
                		    real&r, const real&s, const real&u, const real&f, 
                		    const int&mask, const real&rsxy, const int&ipar, 
                		    const real&rpar, const int&ndbcd, const real&bcData );

void lineSmoothUpdate( const int&nd,
                                              const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
                                              real&u, const real&defect, const int&mask, const int&ndc, const real&c, 
                                              const int&ipar, const real&rpar );
}



#undef M123
#define M123(m1,m2,m3) (m1+halfWidth1+width1*(m2+halfWidth2+width2*(m3+halfWidth3)))
// define C(m1,m2,m3,I1,I2,I3) c(I1,I2,I3,M123(m1,m2,m3))
// undef C
// define C(m1,m2,m3,I1,I2,I3) coeff(M123(m1,m2,m3),I1,I2,I3)

#define FOR_3(i1,i2,i3,I1,I2,I3) I1Bound=I1.getBound();  I2Bound=I2.getBound(); I3Bound=I3.getBound(); for(i3=I3.getBase(); i3<=I3Bound; i3++) for(i2=I2.getBase(); i2<=I2Bound; i2++) for(i1=I1.getBase(); i1<=I1Bound; i1++)

#define FOR_3S(i1,i2,i3,I1,I2,I3) I1Bound=I1.getBound();  I2Bound=I2.getBound(); I3Bound=I3.getBound(); I1Stride=I1.getStride();  I2Stride=I2.getStride(); I3Stride=I3.getStride(); for(i3=I3.getBase(); i3<=I3Bound; i3+=I3Stride) for(i2=I2.getBase(); i2<=I2Bound; i2+=I2Stride) for(i1=I1.getBase(); i1<=I1Bound; i1+=I1Stride)


// ===============================================================================================
// This next macro assign the RHS for the 4th order Neumann+extrapolation condition
// on curvilinear grids. The Neumann BC looks like
//         c1*ur + c2*us + c3*u = f
// The tridiagonal matrix includes all terms on a line normal to the boundary
// The RHS has to include the tangential terms on the boundary (except the diagonal)
//
//  DIR = R, S or T
//  DIM = 2 or 3
// ===============================================================================================


// =======================================================================================
// Macro: Set boundary conditions for the line smooth solve.
// =======================================================================================

//\begin{>>OgmgInclude.tex}{\subsection{smoothLine}}
void Ogmg::
smoothLine(const int & level, const int & grid, 
                      const int & direction, 
                      bool useZebra /* =true */,
                      const int smoothBoundarySide /* = -1 */ )
//---------------------------------------------------------------------------------------------
// /Description:
//    Line Smoother. Zebra line smoothing.
//    
// /direction (input) : smooth on lines in this direction, 0,1,2
// /useZebra (input) : if true use alternating line zebra smoothing.
//\end{OgmgInclude.tex} 
//---------------------------------------------------------------------------------------------
{
    const int debugSave = Ogmg::debug;
  // Ogmg::debug=15; // 7; // 15;
    
    assert( direction>-1 && direction<3 );

    const int m11= 0,m12= 5,m13=10,m14=15,m15=20,
                        m21= 1,m22= 6,m23=11,m24=16,m25=21,
                        m31= 2,m32= 7,m33=12,m34=17,m35=22,
                        m41= 3,m42= 8,m43=13,m44=18,m45=23,
                        m51= 4,m52= 9,m53=14,m54=19,m55=24;
    const int m111=   0,m211=   1,m311=   2,m411=   3,m511=   4,
                        m121=   5,m221=   6,m321=   7,m421=   8,m521=   9,
                        m131=  10,m231=  11,m331=  12,m431=  13,m531=  14,
                        m141=  15,m241=  16,m341=  17,m441=  18,m541=  19,
                        m151=  20,m251=  21,m351=  22,m451=  23,m551=  24,
                        m112=  25,m212=  26,m312=  27,m412=  28,m512=  29,
                        m122=  30,m222=  31,m322=  32,m422=  33,m522=  34,
                        m132=  35,m232=  36,m332=  37,m432=  38,m532=  39,
                        m142=  40,m242=  41,m342=  42,m442=  43,m542=  44,
                        m152=  45,m252=  46,m352=  47,m452=  48,m552=  49,
                        m113=  50,m213=  51,m313=  52,m413=  53,m513=  54,
                        m123=  55,m223=  56,m323=  57,m423=  58,m523=  59,
                        m133=  60,m233=  61,m333=  62,m433=  63,m533=  64,
                        m143=  65,m243=  66,m343=  67,m443=  68,m543=  69,
                        m153=  70,m253=  71,m353=  72,m453=  73,m553=  74,
                        m114=  75,m214=  76,m314=  77,m414=  78,m514=  79,
                        m124=  80,m224=  81,m324=  82,m424=  83,m524=  84,
                        m134=  85,m234=  86,m334=  87,m434=  88,m534=  89,
                        m144=  90,m244=  91,m344=  92,m444=  93,m544=  94,
                        m154=  95,m254=  96,m354=  97,m454=  98,m554=  99,
                        m115= 100,m215= 101,m315= 102,m415= 103,m515= 104,
                        m125= 105,m225= 106,m325= 107,m425= 108,m525= 109,
                        m135= 110,m235= 111,m335= 112,m435= 113,m535= 114,
                        m145= 115,m245= 116,m345= 117,m445= 118,m545= 119,
                        m155= 120,m255= 121,m355= 122,m455= 123,m555= 124;



    realMappedGridFunction & uu= uMG.multigridLevel[level][grid];
    realMappedGridFunction & ff= fMG.multigridLevel[level][grid];
    realMappedGridFunction & coeff =  level==0 ? cMG[grid] : cMG.multigridLevel[level][grid];
    realArray & u = uMG.multigridLevel[level][grid];
    realArray & f = fMG.multigridLevel[level][grid];
    realArray & defect = defectMG.multigridLevel[level][grid];

    CompositeGrid & mgcg = multigridCompositeGrid();
    MappedGrid & mg = mgcg.multigridLevel[level][grid];  
    const int numberOfDimensions = mg.numberOfDimensions();
    int i1,i2,i3;

    const intArray & mask = mg.mask();
    #ifdef USE_PPP
        realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
        realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f,fLocal);
        realSerialArray defectLocal; getLocalArrayWithGhostBoundaries(defect,defectLocal);
        realSerialArray coeffLocal; getLocalArrayWithGhostBoundaries(coeff,coeffLocal);
        intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
    #else
        realArray & uLocal = u;
        realArray & fLocal = f;
        realArray & defectLocal = defect;
        realArray & coeffLocal = coeff;
        const intArray & maskLocal = mask;
    #endif

    const int * maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
    const int maskDim0=maskLocal.getRawDataSize(0);
    const int maskDim1=maskLocal.getRawDataSize(1);
#undef MASK
#define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]


    real *defectp = defectLocal.Array_Descriptor.Array_View_Pointer2;
    const int defectDim0=defectLocal.getRawDataSize(0);
    const int defectDim1=defectLocal.getRawDataSize(1);
#undef DEFECT
#define DEFECT(i0,i1,i2) defectp[i0+defectDim0*(i1+defectDim1*(i2))]

    real *up = uLocal.Array_Descriptor.Array_View_Pointer2;
    const int uDim0=uLocal.getRawDataSize(0);
    const int uDim1=uLocal.getRawDataSize(1);
#undef U
#define U(i0,i1,i2) up[i0+uDim0*(i1+uDim1*(i2))]

    real *fp = fLocal.Array_Descriptor.Array_View_Pointer2;
    const int fDim0=fLocal.getRawDataSize(0);
    const int fDim1=fLocal.getRawDataSize(1);
#undef F
#define F(i0,i1,i2) fp[i0+fDim0*(i1+fDim1*(i2))]
    real *coeffp = coeffLocal.Array_Descriptor.Array_View_Pointer3;
    const int coeffDim0=coeffLocal.getRawDataSize(0);
    const int coeffDim1=coeffLocal.getRawDataSize(1);
    const int coeffDim2=coeffLocal.getRawDataSize(2);
#undef COEFF
#define COEFF(i0,i1,i2,i3) coeffp[i0+coeffDim0*(i1+coeffDim1*(i2+coeffDim2*(i3)))]

    #ifdef USE_PPP
        const int numberOfInterpolationPoints = mgcg.numberOfComponentGrids()>1 ? 
                                                                                        mgcg.multigridLevel[level]->numberOfInterpolationPointsLocal(grid) : 0 ;
        intSerialArray ip;
        if(numberOfInterpolationPoints>0 )
        {
            CompositeGrid & cg = mgcg.multigridLevel[level];
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
            
        }
    #else
        const int numberOfInterpolationPoints = mgcg.multigridLevel[level].numberOfInterpolationPoints(grid);
        const intArray & ip = numberOfInterpolationPoints>0 ? mgcg.multigridLevel[level].interpolationPoint[grid] : maskp; 
    #endif

    const int * ipp = ip.Array_Descriptor.Array_View_Pointer2;
    const int ipDim0=ip.getRawDataSize(0);
#define IP(i0,i1) ipp[i0+ipDim0*(i1)]


    intSerialArray bcLocal(2,3);
    int isPeriodic[3];
    for( int axis=0; axis<3; axis++ )
    {
        if( bc(0,axis,grid)==OgesParameters::extrapolate ||
                bc(1,axis,grid)==OgesParameters::extrapolate )
        {
            printF("Ogmg:lineSmooth:ERROR: finish me for bc=extrapolate\n");
            OV_ABORT("error");
        }
        

        if( mask.getLocalBase(axis) == mask.getBase(axis) ) 
        {
            bcLocal(0,axis)=boundaryCondition(0,axis,grid);
        }
        else
        { // parallel ghost boundary:
            bcLocal(0,axis)=OgmgParameters::parallelGhostBoundary;
        }
        if( mask.getLocalBound(axis) == mask.getBound(axis) ) 
        {
            bcLocal(1,axis)=boundaryCondition(1,axis,grid);
        }
        else
        { // parallel ghost boundary:
            bcLocal(1,axis)=OgmgParameters::parallelGhostBoundary;
        }
    // In parallel the periodicity flag depends on whether the axis is split across processors.
    // If it is then we treat as a parallel ghost -- i.e. a dirichlet BC 
        isPeriodic[axis]=mg.isPeriodic(axis);
        if( isPeriodic[axis] &&
                (bcLocal(0,axis)==OgmgParameters::parallelGhostBoundary ||
                  bcLocal(1,axis)==OgmgParameters::parallelGhostBoundary) )
        {
            isPeriodic[axis]=Mapping::notPeriodic;
            bcLocal(0,axis)=OgmgParameters::parallelGhostBoundary;  // is this correct ? 
            bcLocal(1,axis)=OgmgParameters::parallelGhostBoundary;
        }
        
    }
    if( debug & 4 )
    {
        fprintf(pDebugFile,"\n **** lineSmooth: level=%i, grid=%i, direction=%i bcLocal=[%i,%i][%i,%i][%i,%i]"
                        " bcSupplied=%i\n",level,grid,direction,
          	    bcLocal(0,0),bcLocal(1,0),bcLocal(0,1),bcLocal(1,1),bcLocal(0,2),bcLocal(1,2),bcSupplied);
    }
            

    int I1Bound, I2Bound, I3Bound;
    int I1Stride, I2Stride, I3Stride;
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    Index Dv[3], &D1=Dv[0], &D2=Dv[1], &D3=Dv[2];
    getIndex(mg.extendedIndexRange(),I1,I2,I3);  // include boundary  -- holds boundary conditions
                                                                              
    D1=I1; D2=I2; D3=I3;  // points where we should evaluate the defect ** could do better **


  // -- finish me for ok=false ...


    const int width = orderOfAccuracy+1;  // 3 or 5

   // The tridiagonal (pentadiagonal) system will have a BC of normal, extended or periodic along axis==direction
    bool extendedSystem=false; // true if we have neumann or mixed BC's on an end

    int side,axis;
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
        const bool dirichletStart = (bcLocal(Start,direction)==OgmgParameters::extrapolate ||
                         				 bcLocal(Start,direction)==OgmgParameters::parallelGhostBoundary);
        const bool dirichletEnd = (bcLocal(End,direction)==OgmgParameters::extrapolate ||
                         			       bcLocal(End,direction)==OgmgParameters::parallelGhostBoundary);
        if( axis==direction )
        {
      // include ghost line for neumann or mixed BC'S                     
            if( bcLocal(Start,direction)>0 &&  ( !dirichletStart || orderOfAccuracy==4) )
            {
//	extendedSystem=extendedSystem || orderOfAccuracy==2 || parameters.fourthOrderBoundaryConditionOption==0;
      	extendedSystem=extendedSystem || orderOfAccuracy==2 || orderOfAccuracy==4;
        // we need 1 ghost line for dirichlet (4th-order) or 2 ghost lines for Neumann fourth-order
                int numGhost= !dirichletStart ? orderOfAccuracy/2 : 1;
      	Iv[direction]=Range(Iv[direction].getBase()-numGhost,Iv[direction].getBound());

            }
            if( bcLocal(End,direction)>0 && ( !dirichletEnd || orderOfAccuracy==4) )
            {
//	extendedSystem=extendedSystem || orderOfAccuracy==2 || parameters.fourthOrderBoundaryConditionOption==0;
      	extendedSystem=extendedSystem || orderOfAccuracy==2 || orderOfAccuracy==4;
        // we need 1 ghost line for dirichlet (4th-order) or 2 ghost lines for Neumann fourth-order
                int numGhost= !dirichletEnd ? orderOfAccuracy/2 : 1;
      	Iv[direction]=Range(Iv[direction].getBase(),Iv[direction].getBound()+numGhost);
            }
        }
        else if( true )
        {
      // This axis is not in the direction of the line solve.
      // we only need to solve the equations on the adjacent boundary if there is a neumann/mixed BC
            if( !mg.isPeriodic(axis) && bcLocal(0,axis)>=0 && bcLocal(0,axis)!=OgmgParameters::equation )
            {
                int numGhost= bcLocal(0,axis)==0 ? orderOfAccuracy/2 : 1;  // *wdh* 100716 -- do not apply line smooth on interp boundary ghost (order 4)
                Iv[axis]=Range(Iv[axis].getBase()+numGhost,Iv[axis].getBound());
            }
            if( !mg.isPeriodic(axis) && bcLocal(1,axis)>=0 && bcLocal(1,axis)!=OgmgParameters::equation )
            {
                int numGhost= bcLocal(1,axis)==0 ? orderOfAccuracy/2 : 1;   // *wdh* 100716  
                Iv[axis]=Range(Iv[axis].getBase(),Iv[axis].getBound()-numGhost);
            }
        }
    }

  // Save global index bounds 
    Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];
    Ig1=I1, Ig2=I2, Ig3=I3;  
    
  // -- We should include all parallel ghost along axis==direction -- makes a larger overlap of line solvers
  //    But not in other directions. We could include some ghost in the tangential to avoid communication
    int includeGhost=0;    
    bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);
    const int maxGhostToUse=10;  // =1 
    int numParallelGhost[2]={0,0};  // remember the number of parallel ghost we keep along axis=direction
        
    if( ok )
    {
        int ia=Iv[direction].getBase(), ib=Iv[direction].getBound();
        int numGhost=min(maxGhostToUse,mask.getGhostBoundaryWidth(direction));  
        if( bcLocal(0,direction)==OgmgParameters::parallelGhostBoundary && 
                mask.getLocalBase(direction) != mask.getBase(direction) )  // no need to add ghost points on far left side parallel ghost
        {
            numParallelGhost[0]=numGhost;
            ia-=numGhost;
        }
        
        if( bcLocal(1,direction)==OgmgParameters::parallelGhostBoundary &&
                mask.getLocalBound(direction) != mask.getBound(direction) )  // no need to add ghost points on far right side parallel ghost
        { 
            numParallelGhost[1]=numGhost;
            ib+=numGhost;
        }
        
    // parallel and periodic, include an extra point on the ends: 
        if( bcLocal(0,direction)==OgmgParameters::parallelGhostBoundary && 
                mask.getLocalBase(direction) == mask.getBase(direction) ) ia-=1;
        if( bcLocal(1,direction)==OgmgParameters::parallelGhostBoundary &&
                mask.getLocalBound(direction) == mask.getBound(direction) ) ib+=1;  

        Iv[direction] =Range(ia,ib);

    // for periodic BC's we include extra parallel ghost at the start and end 
        Igv[direction] = Range( min(ia,Igv[direction].getBase()), max(ib,Igv[direction].getBound()) );
        Dv[direction] = Range( min(ia,Dv[direction].getBase()), max(ib,Dv[direction].getBound()) );
    }


    if( debug & 4 )
    {
        fprintf(pDebugFile,"lineSmooth: maskLocal=[%i,%i][%i,%i][%i,%i], Iv=[%i,%i][%i,%i][%i,%i], Ig=[%i,%i][%i,%i][%i,%i], \n"
                                              "Dv=[%i,%i][%i,%i][%i,%i]\n",
                        maskLocal.getBase(0),maskLocal.getBound(0),
                        maskLocal.getBase(1),maskLocal.getBound(1),
                        maskLocal.getBase(2),maskLocal.getBound(2),
          	    Iv[0].getBase(),Iv[0].getBound(),
          	    Iv[1].getBase(),Iv[1].getBound(),
          	    Iv[2].getBase(),Iv[2].getBound(),
          	    Igv[0].getBase(),Igv[0].getBound(),
          	    Igv[1].getBase(),Igv[1].getBound(),
          	    Igv[2].getBase(),Igv[2].getBound(),
          	    Dv[0].getBase(),Dv[0].getBound(),
          	    Dv[1].getBase(),Dv[1].getBound(),
          	    Dv[2].getBase(),Dv[2].getBound());
    }
    

    if( Ogmg::debug & 64 )
        display(fLocal,sPrintF(buff,"lineSmooth: direction=%i: Here is f",direction),pDebugFile);

    const bool rectangular=(*coeff.getOperators()).isRectangular() &&
        ( level < mgcg.numberOfMultigridLevels()-numberOfExtraLevels ); 
    StencilTypeEnum sparseStencil=general;
    const bool isRectangular=mg.isRectangular();
    if( isRectangular )
    {
        if( equationToSolve!=OgesParameters::userDefined ) 
            sparseStencil=level==0 ? sparseConstantCoefficients : constantCoeff;
        else if( rectangular && assumeSparseStencilForRectangularGrids )
            sparseStencil=sparse;
    }
    const int gridType = isRectangular ? 0 : 1;

    real jacobiWork;  // = number of mults+divides for a jacobi iteration
    if(level==0 && (sparseStencil==constantCoeff || sparseStencil==sparseConstantCoefficients) ) 
    {
        jacobiWork=orderOfAccuracy==2 ? ( mg.numberOfDimensions()==2 ? 6. : 8.) :  // sparse 5-point or 7-point stencil
            ( mg.numberOfDimensions()==2 ? 10. : 14.); // 4th-order 9-pt or 13
    }
    else
    {
        jacobiWork=orderOfAccuracy==2 ? ( mg.numberOfDimensions()==2 ? 10. : 28.) : // 9-pt or 27-pt
            ( mg.numberOfDimensions()==2 ? 25. : 125.); // 4th-order: 25 or 125 pt stencil
    }
    

    real dx[3]={1.,1.,1.};
    if( sparseStencil==constantCoeff || sparseStencil==sparseConstantCoefficients || isRectangular )
        mg.getDeltaX( dx );

    bool useEquationOnGhostForDirichlet= orderOfAccuracy==2 ? 0 : useEquationOnGhostLineForDirichletBC(mg,level);
    bool useEquationOnGhostForNeumann  = orderOfAccuracy==2 ? 0 : useEquationOnGhostLineForNeumannBC(mg,level);


  // we may apply the eqn as a BC on dirichlet boundaries.  
    const int bcOptionD = (useEquationOnGhostForDirichlet ? 1 : 
                   			 (level>0 && parameters.useSymmetryForDirichletOnLowerLevels) ? 2 : 0);  

    
  // Here is the BC option for Neumann BC's
  //   -- this needs to match what is done in applyBoundaryConditions!
    int bcOptionN;
    if( level==0 && 
            (orderOfAccuracy==2 || (parameters.neumannSecondGhostLineBC==OgmgParameters::useEquationToSecondOrder 
                        			      && useEquationOnGhostForNeumann) ) )
        bcOptionN=1; // use "eqn" BC for first (order=2) or second (order=4) ghost line
    else if( level>0 && parameters.lowerLevelNeumannSecondGhostLineBC==OgmgParameters::useEquationToSecondOrder )
        bcOptionN=3; // Use mixed BC to second order on all ghost
    else if( level>0 && parameters.useSymmetryForNeumannOnLowerLevels )
        bcOptionN=2;  // even symmetry
    else
        bcOptionN=0;  // extrapolate 2nd (order=4) ghost
        
  // This may not be a true Neumann BC if the user has only supplied coefficients for the BC on the ghost line.
    int isNeumannBC[2];
    for( int side=0; side<=1; side++ )
    {
          isNeumannBC[side] = (equationToSolve!=OgesParameters::userDefined ||	
                      			    (bcSupplied && (bc(side,direction,grid)==OgesParameters::neumann || 
                                  					    bc(side,direction,grid)==OgesParameters::mixed )));
    }
    
  // This next variable is only used for fourth-order
    const int orderOfExtrapD= bcOptionD==1 ? 4 : parameters.orderOfExtrapolationForDirichletOnLowerLevels;
  // *wdh* 110309 const int orderOfExtrapN= 4;  // ********** 
    const int orderOfExtrapN = level==0 ? parameters.orderOfExtrapolationForNeumann : 
                                                                                parameters.orderOfExtrapolationForNeumannOnLowerLevels;
    const int useBoundaryForcing=level==0;  // BC has an in-homogeneous RHS
    
    int ipar[] ={ mg.numberOfDimensions(),
            		direction,
            		(int)sparseStencil,
            		orderOfAccuracy,
            		I1.getBase(),I1.getBound(),1,
            		I2.getBase(),I2.getBound(),1,
            		I3.getBase(),I3.getBound(),1,
            		bcLocal(0,0),
            		bcLocal(1,0),         
            		bcLocal(0,1),         
            		bcLocal(1,1),         
            		bcLocal(0,2),         
            		bcLocal(1,2),
            		bcOptionD,
            		bcOptionN,
            		orderOfExtrapD,
            		orderOfExtrapN,
            		gridType,
            		I1.getBase(),I1.getBound(),
            		I2.getBase(),I2.getBound(),
            		I3.getBase(),I3.getBound(),
                                grid,                        // ipar[30]
                                level,                       // ipar[31]
                                (int)equationToSolve,        // ipar[32]
                                useBoundaryForcing,          // ipar[33]
                                isNeumannBC[0],             // ipar[34]
                                isNeumannBC[1],             // ipar[35]
                                myid                        // [36]
                                  }; //         

    real *uptr=uLocal.getDataPointer();
    real *sptr= uptr;  // not used for now
    #ifdef USE_PPP
    real *prsxy=uptr; if( !isRectangular ){ prsxy=mg.inverseVertexDerivative().getLocalArray().getDataPointer(); }  // 
    #else
        real *prsxy= isRectangular ? uptr : mg.inverseVertexDerivative().getDataPointer();
    #endif
            
    real rpar[] = { dx[0],dx[1],dx[2],mg.gridSpacing(0),mg.gridSpacing(1),mg.gridSpacing(2) }; //
    const real *pcc = constantCoefficients.getBound(2)>=level ? &constantCoefficients(0,grid,level) : rpar;

    real time0;
    if( !lineSmoothIsInitialized(direction,grid,level) )
    {
        time0=getCPU();
        
        lineSmoothIsInitialized(direction,grid,level)=true;
        realSerialArray a,b,c,d,e;

        if( ok )
        {
            a.redim(I1,I2,I3); b.redim(I1,I2,I3); c.redim(I1,I2,I3); 

            if( orderOfAccuracy==4 )
            {
      	d.redim(I1,I2,I3);
      	e.redim(I1,I2,I3);
            }

      // if( Ogmg::debug & 4 )
      //  displayCoeff(coeff,sPrintF("coeff for grid %i",grid));

            lineSmoothBuild( numberOfDimensions,
                   		       uLocal.getBase(0),uLocal.getBound(0),
                   		       uLocal.getBase(1),uLocal.getBound(1),
                   		       uLocal.getBase(2),uLocal.getBound(2),
                   		       a.getBase(0),a.getBound(0),a.getBase(1),a.getBound(1),a.getBase(2),a.getBound(2),
                   		       coeffLocal.getLength(0), *coeffLocal.getDataPointer(), 
                   		       *pcc,
                   		       *a.getDataPointer(), *b.getDataPointer(), *c.getDataPointer(), 
                   		       *d.getDataPointer(), *e.getDataPointer(), 
                   		       *sptr, *uptr, *fLocal.getDataPointer(), 
                   		       *maskLocal.getDataPointer(), *prsxy, ipar[0], rpar[0], 
                   		       boundaryConditionData.getLength(0), boundaryConditionData(0,0,0,grid) );
        
      // if( Ogmg::debug & 8 )
            if( Ogmg::debug & 4 )
            {
      	fprintf(pDebugFile,"***lineSmooth: equationToSolve=%i level=%i sparseStencil=%i bcOptionN=%i *****\n",
            		(int)equationToSolve,level,sparseStencil,bcOptionN);
            
      	if( true )
        	  displayMask(maskLocal,"maskLocal",pDebugFile);

      	display(a,sPrintF(buff,"lineSmooth: tridiagonal matrix, a, grid=%i direction=%i",grid,direction),pDebugFile,"%6.1f ");
      	display(b,sPrintF(buff,"lineSmooth: tridiagonal matrix, b, grid=%i direction=%i",grid,direction),pDebugFile,"%6.1f ");
      	display(c,sPrintF(buff,"lineSmooth: tridiagonal matrix, c, grid=%i direction=%i",grid,direction),pDebugFile,"%6.1f ");
      	if( orderOfAccuracy==4 )
      	{
        	  display(d,sPrintF(buff,"lineSmooth: tridiagonal matrix, d, grid=%i direction=%i",grid,direction),pDebugFile,"%6.1f ");
        	  display(e,sPrintF(buff,"lineSmooth: tridiagonal matrix, e, grid=%i direction=%i",grid,direction),pDebugFile,"%6.1f ");
      	}
            }
            
        }
        
    // printf("Factor the tridiagonal matrix for direction=%i \n",direction);
        

    // *************************************************************
    // ************Factor the Matrices *****************************
    // *************************************************************

        TridiagonalSolver::SystemType
            systemType = (bool)isPeriodic[direction] ? TridiagonalSolver::periodic :
                                                                      extendedSystem ? TridiagonalSolver::extended : 
                                                                                                        TridiagonalSolver::normal;
        
        if( Ogmg::debug & 4 ) fprintf(pDebugFile,">>>>>>>>>>>>>>> TridiagonalSolver::systemType = %s\n",
                         				 systemType==TridiagonalSolver::periodic ? "periodic" : 
                                                                  systemType==TridiagonalSolver::extended ? "extended" : "normal");

    // assert( tridiagonalSolver[level][grid][direction]==NULL ); // *wdh* 100419 may be non-null if coeff's change
        delete tridiagonalSolver[level][grid][direction];

        if( ok )
        {
            tridiagonalSolver[level][grid][direction] = new TridiagonalSolver;
            if( orderOfAccuracy==2 )
            {
	// second order tridiagonal system
      	tridiagonalSolver[level][grid][direction]->factor(a,b,c,systemType,direction); 
            }
            else
            {
	// fourth order : penta-diagonal system
      	tridiagonalSolver[level][grid][direction]->factor(a,b,c,d,e,systemType,direction); 
            }
        }
        
        tm[timeForTridiagonalFactorInSmooth]+=getCPU()-time0;

    // if( ok && (Ogmg::debug & 64) )
        if( ok && (Ogmg::debug & 4) )
        {
            display(a,"AFTER FACTOR: Here is the tridiagonal matrix, a",pDebugFile,"%6.0f ");
            display(b,"AFTER FACTOR: Here is the tridiagonal matrix, b",pDebugFile,"%6.0f ");
            display(c,"AFTER FACTOR: Here is the tridiagonal matrix, c",pDebugFile,"%6.0f ");
            if( orderOfAccuracy==4 )
            {
      	display(d,"AFTER FACTOR: Here is the tridiagonal matrix, d",pDebugFile,"%6.0f ");
      	display(e,"AFTER FACTOR: Here is the tridiagonal matrix, e",pDebugFile,"%6.0f ");
            }
        }
    // LU factor, n-1 multiplies, n-1 divisions 
    //    ***** fourth-order : 8 mults+divides

        real wu = (orderOfAccuracy==2 ? 2. : 8. )/jacobiWork;
        workUnits(level)+=wu*mask.elementCount()/real(numberOfGridPoints); // use mask.elementCount and not mask.elementCount
    }

    if( Ogmg::debug & 8 )
    {
        display(u,sPrintF("lineSmooth: Here is the solution u at start, level=%i, grid=%i ",level,grid),
          	    debugFile,"%8.2e ");
        display(f,sPrintF("lineSmooth: Here is the rhs f at start, level=%i, grid=%i ",level,grid),debugFile,"%8.2e ");
    }

    enum LineOptionEnum
    {
        zebraLine=0,
        jacobiLine
    };
        
    int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
    int stridev[3], &stride1=stridev[0], &stride2=stridev[1], &stride3=stridev[2];
    int shift[3]; shift[0]=0; shift[1]=0; shift[2]=0; 
    int j1,j2,j3;
    Range Izv[3], &I1z=Izv[0], &I2z=Izv[1], &I3z=Izv[2];
    Range Dzv[3], &D1z=Dzv[0], &D2z=Dzv[1], &D3z=Dzv[2];

  //    printf(" ****************** zebra: parameters.numberOfSubSmooths(grid=%i,level=%i)=%i \n",
  //  	 grid,level,parameters.numberOfSubSmooths(grid,level));
    
    const bool usingConstantCoefficients=sparseStencil==constantCoeff || sparseStencil==sparseConstantCoefficients;

    if( smoothBoundarySide==0 || smoothBoundarySide==1 )
    {
        #ifdef USE_PPP
            OV_ABORT("lineSmooth:boundarySmooth: check me for parallel");
        #endif
        assert( numberOfDimensions==2 );
        if( direction==0 )
        {
            I2= smoothBoundarySide==0 ? Range(I2.getBase()+1,I2.getBase()+1) : Range(I2.getBound()-1,I2.getBound()-1);
            D2=I2;
        }
        else
        {
            I1= smoothBoundarySide==0 ? Range(I1.getBase()+1,I1.getBase()+1) : Range(I1.getBound()-1,I1.getBound()-1);
            D1=I1;
        }
    }
    
    if( Ogmg::debug & 8 )
    {
        const IntegerArray & gid = mg.gridIndexRange();
        printF("lineSmooth: level=%i grid=%i gid=[%i,%i][%i,%i][%i,%i] I1=[%i,%i] I2=[%i,%i] I3=[%i,%i]\n",
         	   level,grid,gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2), 
                      I1.getBase(),I1.getBound(),
                      I2.getBase(),I2.getBound(),
                      I3.getBase(),I3.getBound());
    }

    if( true )
    {
        defectLocal=0.;  // *wdh* 100716 do this to avoid nan's : twoRhombus4.order4.ml3 with alz
    }
    
    for( int iteration=0; iteration<parameters.numberOfSubSmooths(grid,level); iteration++ )
    {
        bool preSmooth=true; // numberOfLinesSolves%2 == 0; // true;
        if( preSmooth &&  parameters.numberOfBoundaryLayersToSmooth>0 )
        {
            
      // int numberOfLayers=parameters.numberOfBoundaryLayersToSmooth + (numberOfLinesSolves%3)-1;
            int numberOfLayers=parameters.numberOfBoundaryLayersToSmooth; 
      // int numberOfLayers=parameters.numberOfBoundaryLayersToSmooth;

            int bc[6]={0,0,0,0,0,0}; //
            bc[0+2*direction]=1; // smooth boundaries axis==direction
            bc[1+2*direction]=1;
            if( parameters.smootherType(grid,level)==alternatingLineJacobi ||
                    parameters.smootherType(grid,level)==alternatingLineZebra )
            {
        // for alternating smoothers we smooth all boundaries
	//  bc[0]=bc[1]=bc[2]=bc[3]=bc[4]=bc[5]=1;
            }
            bc[0]=bc[1]=bc[2]=bc[3]=bc[4]=bc[5]=1;
            

            smoothBoundary(level,grid,bc,numberOfLayers,parameters.numberOfBoundarySmoothIterations );
        }



        if( useZebra )
        {
      // ******* Alternating Zebra Smoothing ******

            if( debug & 4 && ( level==0 ) ) 
                fprintf(pDebugFile,"smooth zebra level=%i direction=%i (usingConstantCoefficients=%i)\n",level,direction,
                                					  (int)usingConstantCoefficients);
            

      // this extra shift is added so that we solve on the same order of red/black points as an older version
            const int delta1=bcLocal(0,(direction+1)% mg.numberOfDimensions())<0 ? 0 : 1;
            const int delta2=bcLocal(0,(direction+2)% mg.numberOfDimensions())<0 ? 0 : 1;
            
            real & omega=parameters.omegaLineZebra;
            real variableOmegaFactor=1.;
      // scale factor for the locally optimal omega
            if( orderOfAccuracy==2 )
      	variableOmegaFactor=2.*parameters.variableOmegaScaleFactor; // 1.01231; // 1.085/1.071796 ! NOTE
            else
      	variableOmegaFactor=parameters.variableOmegaScaleFactor;

            if( omega<0. )
            { // assign default values
      	omega=1.; // numberOfDimensions==2 ? 1.1 : 1.15;
      	if( numberOfDimensions==3 )
      	{
        	  if( orderOfAccuracy==2 )
        	  {
	    // ** line zebra acts like RB in the two other directions
	    // real cmax=1.-1./3.;
	    // omega=variableOmegaFactor/(1.+sqrt(1.-cmax*cmax));

          	    omega=1.09;  // "2D" value
        	  }
        	  else // fourth-order accurate
        	  {
          	    omega=1.15; //  ! what should this be ?
        	  }
      	}
            }
            real oneMinusOmega=1.-omega;
            const int variableCoefficients = !(sparseStencil==constantCoeff || sparseStencil==sparseConstantCoefficients);
      // useOmega= 0 : do not use omega
      //           1 : use constant omega
      //           2 : use variable omega 
            int useOmega=(!variableCoefficients || !parameters.useLocallyOptimalLineOmega) ? 1 : 2;

      // printF("lineSmooth: variableCoefficients=%i, useOmega=%i, parameters.useLocallyOptimalLineOmega)\n",variableCoefficients,useOmega,
      //     (int)parameters.useLocallyOptimalLineOmega);
            

      // *wdh* 110216 -- do not use variable omega in 3D -- there is a bug here 
      // c.f. lineSmoothUpdate with sibe in parallel with cgins
      // *wdh* 2011/08/24 -- bug with variable omega and flatPlateWingGrid in parallel --      
      // ***** NEED TO FINISH computeOmega3d in lineSmoothOpt.bf *******
            if( numberOfDimensions==3 )
                useOmega=1;  

            if( fabs(omega-1.)<10.*REAL_EPSILON )
                useOmega=0;

            for( int zebra3=0; zebra3<mg.numberOfDimensions()-1; zebra3++)
      	for( int zebra2=0; zebra2<2; zebra2++)
      	{
	  // Here is the stride for zebra smoothing:
        	  if( true )
        	  {
          	    stride1 = direction==axis1 ? 1 : 2;
          	    stride2 = direction==axis2 ? 1 : 2;
          	    stride3 = (mg.numberOfDimensions()==3 && direction!=axis3) ? 2 : 1; 
	    // shift[0,1,2] = 0 or 1 : shift the stencil to get red or black points
          	    shift[direction]=0;
          	    shift[(direction+1)% mg.numberOfDimensions()]=(zebra2+zebra3+delta1) %2;  

          	    if( mg.numberOfDimensions()==3 )
            	      shift[(direction+2)% mg.numberOfDimensions()]=(zebra2+delta2)%2;

        	  }
        	  else
        	  {
	    // For testing set strides to 1 -- turn off zebra
          	    stride1 = 1;
          	    stride2 = 1;
          	    stride3 = 1;
          	    shift[0]=shift[1]=shift[2]=0;
        	  }
        	  
       	 
	  // note: use the global bounds here: 
        	  I1z=IndexBB(Ig1.getBase()+shift[0],Ig1.getBound(),stride1);
        	  I2z=IndexBB(Ig2.getBase()+shift[1],Ig2.getBound(),stride2);
        	  I3z=IndexBB(Ig3.getBase()+shift[2],Ig3.getBound(),stride3);

        	  if( debug & 4 )
          	    fprintf(pDebugFile,"direction=%i zebra2=%i, zebra3=%i, I1z=(%i,%i,%i), I2z=(%i,%i,%i), I3z=(%i,%i,%i) useOmega=%i.\n",
                                        direction,zebra2,zebra3,
                		    I1z.getBase(),I1z.getBound(),I1z.getStride(),I2z.getBase(),I2z.getBound(),I2z.getStride(),
                		    I3z.getBase(),I3z.getBound(),I3z.getStride(),useOmega);
            
	  // determine the defect : defect(I1z,I2z,I3z) = f - c* u

        	  real time1=getCPU();

          // We only need to evaluate the defect on a restricted set of points
          //  (We must avoid evaluating the defect at some end points where it would not be a valid operation)
        	  
                    for( axis=0; axis<3; axis++ )
        	  {
          	    int base = Izv[axis].getBase(), bound=Izv[axis].getBound();
                        if( base  < Dv[axis].getBase()  ) base +=stridev[axis];
                        if( bound > Dv[axis].getBound() ) bound-=stridev[axis];
          	    Dzv[axis]=Range(base,bound,Izv[axis].getStride());
        	  }

          // To do: currently we evaluate the defect on boundaries and possibly ghost pts (fourth order)
          // For dirichlet BC's we only need to eval the defect on interior points
          // Dzv[direction]=Range(Dzv[direction].getBase()+2,Dzv[direction].getBound()-2);
        	  

                    if( debug & 4 )
                        fprintf(pDebugFile,"lineSmooth: Eval defect at these points: D1z=[%i,%i,%i] D2z=[%i,%i,%i] D3z=[%i,%i,%i]\n",
                		    D1z.getBase(),D1z.getBound(),D1z.getStride(),
                		    D2z.getBase(),D2z.getBound(),D2z.getStride(),
                		    D3z.getBase(),D3z.getBound(),D3z.getStride());
//          printf(" u : [%i,%i][%i,%i][%i,%i]\n",u.getBase(0),u.getBound(0),u.getBase(1),u.getBound(1),u.getBase(2),u.getBound(2));
//          printf(" f : [%i,%i][%i,%i][%i,%i]\n",f.getBase(0),f.getBound(0),f.getBase(1),f.getBound(1),f.getBase(2),f.getBound(2));
//          printf(" defect : [%i,%i][%i,%i][%i,%i]\n",defect.getBase(0),defect.getBound(0),defect.getBase(1),defect.getBound(1),defect.getBase(2),defect.getBound(2));

	  // *** getDefect(level,grid,f,u,I1z,I2z,I3z,defect,direction);
                    if( Ogmg::debug & 4 )
        	  {
          	    defect=0.;  // set to zero for debugging since getDefect only assigns the zebra lines
        	  }
        	  

          // ---------------------------------------------------------------------------------------
          // -------------------- Compute the defect -----------------------------------------------
          // ---------------------------------------------------------------------------------------
        	  getDefect(level,grid,f,u,D1z,D2z,D3z,defect,direction);

        	  time0=getCPU();
        	  tm[timeForDefectInSmooth]+=time0-time1;

        	  if( ok )
        	  {
          	    if( debug & 4 )
          	    {
            	      fprintf(pDebugFile,"lineSmooth: global: Izv=[%i,%i,%i][%i,%i,%i][%i,%i,%i]\n",
                  		      Izv[0].getBase(),Izv[0].getBound(),Izv[0].getStride(),
                  		      Izv[1].getBase(),Izv[1].getBound(),Izv[1].getStride(),
                  		      Izv[2].getBase(),Izv[2].getBound(),Izv[2].getStride());
          	    }
	    // restrict zebra bounds to the local processor
          	    int ia,ib;
          	    for( axis=0; axis<numberOfDimensions; axis++ )
          	    {
            	      int ia0=Izv[axis].getBase(), ib0=Izv[axis].getBound(), stride=Izv[axis].getStride();
            	      if( axis==direction )
            	      { // include parallel ghost points along "direction"
                                int numGhost=min(maxGhostToUse,mask.getGhostBoundaryWidth(direction));  
            		ia = max( ia0, maskLocal.getBase(axis) +mask.getGhostBoundaryWidth(axis)-numGhost );
            		ib = min( ib0, maskLocal.getBound(axis)-mask.getGhostBoundaryWidth(axis)+numGhost );
            	      }
            	      else
            	      {
            		ia = max( ia0, maskLocal.getBase(axis) +mask.getGhostBoundaryWidth(axis) ); // no ghost 
            		ia += (stride - (ia-ia0)%stride)%stride; // adjust ia so it offset by a factor of stride from ia0
            		assert( (ia-ia0)%stride == 0 );
            		ib = min( ib0, maskLocal.getBound(axis)-mask.getGhostBoundaryWidth(axis) ); // no ghost 
            		ib -= (stride - (ib0-ib)%stride)%stride; // adjust ib so it offset by a factor of stride from ib0
            		assert( (ib0-ib)%stride == 0 );
            	      }
            	      Izv[axis] =Range(ia,ib,stride);
          	    }
          	    if( debug & 4 )
          	    {
            	      fprintf(pDebugFile,"lineSmooth: local:  Izv=[%i,%i,%i][%i,%i,%i][%i,%i,%i]\n",
                  		      Izv[0].getBase(),Izv[0].getBound(),Izv[0].getStride(),
                  		      Izv[1].getBase(),Izv[1].getBound(),Izv[1].getStride(),
                  		      Izv[2].getBase(),Izv[2].getBound(),Izv[2].getStride());
          	    }
        	  

            // ---------------------------------------------------------------------------------------
            // -------------------- Line smooth boundary conditions ----------------------------------
            // ---------------------------------------------------------------------------------------  
          // 	    lineSmoothBoundaryConditions(FOR_3S,Izv,I1z,I2z,I3z); 
          // if( false ) // this is done last now
          // {
          // // Set values at interpolation points
          //   if( mg.numberOfDimensions()==2 )
          //   {
          //     i3=I3z.getBase();
          //     for( int i=0; i<numberOfInterpolationPoints; i++ )
          //     {
          //       i1=IP(i,0);
          //       i2=IP(i,1);
          //       DEFECT(i1,i2,i3)=U(i1,i2,i3); 
          //     }
          //   }
          //   else
          //   {
          //     for( int i=0; i<numberOfInterpolationPoints; i++ )
          //     {
          //       i1=IP(i,0);
          //       i2=IP(i,1);
          //       i3=IP(i,2);
          //       DEFECT(i1,i2,i3)=U(i1,i2,i3); 
          //     }
          //   }
          // }
          // For the constant coeff case we must fill in the BC's into the defect array
          // for fourth order we need to assign the rhs for extrapolation conditions
                    if( (true || usingConstantCoefficients ) // **** we must always assign Neumann BC's -- could avoid dirichlet below
                            || orderOfAccuracy==4 )
                    {
            // Boundary Conditions
                        Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
                        axis=direction;
                        for( int side=0; side<=1; side++ )
                        {
                            if( bcLocal(side,axis)>0 )
                            {
                                is1=is2=is3=0;
                                isv[axis]=1-2*side;
                                if( bcLocal(side,axis)==OgmgParameters::parallelGhostBoundary )
                                {
                   // "Dirichlet" BC at parallel ghost: set to current solution value
                           	 J1=I1z; J2=I2z; J3=I3z;
                           	 Jv[axis]=side==0 ? Izv[axis].getBase() : Izv[axis].getBound();
                           	 if( orderOfAccuracy==2 )
                           	 {
                             	   if( false && debug & 4 )
                               	     fprintf(pDebugFile," lineSmoothRHS: parallelGhostBoundary grid=%i side=%i J1=[%i,%i,%i] J2=[%i,%i,%i] J3=[%i,%i,%i]\n",grid,side,
                                                              J1.getBase(),J1.getBound(),J1.getStride(),
                                     		     J2.getBase(),J2.getBound(),J2.getStride(),
                                     		     J3.getBase(),J3.getBound(),J3.getStride());
                             	   FOR_3S(i1,i2,i3,J1,J2,J3)
                             	   {
          	     // *wdh* 2011/08/26 -- we need to set rhs=u on parallel ghost that are also interp. pts
                       // if( MASK(i1,i2,i3)>0 )
                               	     if( MASK(i1,i2,i3)!=0 )
                               	     {
                                 	       DEFECT(i1,i2,i3)=U(i1,i2,i3);
                               	     }
                               	     else
                               	     {
                                 	       DEFECT(i1,i2,i3)=0.;
                               	     }
                             	   }
                           	 }
                           	 else
                           	 {
                             	   FOR_3S(i1,i2,i3,J1,J2,J3)
                             	   {
                                              j1=i1+is1; j2=i2+is2; j3=i3+is3; // first line in 
          	     // *wdh* 2011/08/26 -- we need to set rhs=u on parallel ghost that are also interp. pts
          	     // if( MASK(i1,i2,i3)>0 )
                               	     if( MASK(i1,i2,i3)!=0 )
                               	     {
                                 	       DEFECT(i1,i2,i3)=U(i1,i2,i3);
                                 	       DEFECT(j1,j2,j3)=U(j1,j2,j3);
                               	     }
                               	     else
                               	     {
                                 	       DEFECT(i1,i2,i3)=0.;
                                 	       DEFECT(j1,j2,j3)=0.;
                               	     }
                             	   }
                           	 }
                   // --- Fixup any interpolation points that lie in the parallel ghost region ---
                   //   These points are not in the list of interpolation points. BUG fixed: *wdh* 2011/08/26
                   //   NOTE: the final parallel ghost line has already been set above, so we loop
                   //         over one less line of parallel ghost points
                                      int ia,ib;
                           	 if( side==0)
                           	 {
                             	   ia=Izv[axis].getBase()+1;
                                          ib=Izv[axis].getBase()+numParallelGhost[0]-1;
          	   // Jv[axis]= Range(ia+1,ia+numParallelGhost[0]-1);
                           	 }
                           	 else
                           	 {
                             	   ia=Izv[axis].getBound()-numParallelGhost[1]+1;
                             	   ib=Izv[axis].getBound()-1;
          	   // Jv[axis]= Range(ib-numParallelGhost[1]+1,ib-1);
                           	 }
                           	 if( ia<=ib )  // ia>ib can occur for parallel ghost on the far left and right ends of an array.
                           	 {
                             	   Jv[axis]=Range(ia,ib);
                             	   FOR_3S(i1,i2,i3,J1,J2,J3)
                             	   {
                               	     if( MASK(i1,i2,i3)<0 )
                               	     {
                                 	       DEFECT(i1,i2,i3)=U(i1,i2,i3);
                               	     }
                             	   }
                           	 }
                                }
                                else if( bcLocal(side,axis)==OgmgParameters::extrapolate )
                                {
          	// *********************************
          	// ******* Dirichlet BC ************
          	// *********************************
          	 // printf("Assign DirichletRHS for TRID side=%i axis=%i \n",side,axis);
           	    // J1=I1z; J2=I2z; J3=I3z;
           	    // Jv[axis]=side==0 ? Izv[axis].getBase() : Izv[axis].getBound();
           	    // do not include ghost pts on adj sides.
           	    // ** getBoundaryIndex(mg.gridIndexRange(),side,axis,J1,J2,J3);  // this fills in values even if not needed		
                           	 J1=I1z; J2=I2z; J3=I3z;
                           	 Jv[axis]=side==0 ? Izv[axis].getBase() : Izv[axis].getBound();
                           	 if( orderOfAccuracy==2 )
                           	 {
                             	   FOR_3S(i1,i2,i3,J1,J2,J3)
                             	   {
                               	     if( MASK(i1,i2,i3)>0 )
                               	     {
                                 	       DEFECT(i1,i2,i3)=F(i1,i2,i3);
                               	     }
                               	     else
                               	     {
                                 	       DEFECT(i1,i2,i3)=0.;
                               	     }
                             	   }
                           	 }
                           	 else
                           	 {
                                          assert( orderOfAccuracy==4 );
          	   // if( parameters.fourthOrderBoundaryConditionOption==0 || level>0 )
                             	   if( !useEquationOnGhostForDirichlet )
                             	   {
                               	     FOR_3S(i1,i2,i3,J1,J2,J3)
                               	     {
                                 	       j1=i1+is1; j2=i2+is2; j3=i3+is3; // boundary point
                                 	       if( MASK(j1,j2,j3)>0 )
                                 	       {
                                   	         DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                   	         DEFECT(i1,i2,i3)=0.;           // extrapolation or symmetry 
                                 	       }
                                 	       else
                                 	       {
                                 		 DEFECT(i1,i2,i3)=0.;
                                 	       }
                               	     }
                             	   }
                             	   else
                             	   {
                       // --- Use equation to 2nd order on the first ghost line ---
                               	     if( equationToSolve!=OgesParameters::laplaceEquation )
                               	     {
                                                  printf("lineSmoothBC: ERROR: equationToSolve!=laplaceEquation. This case is not yet implemented.\n");
                                 	       OV_ABORT("ERROR");
                               	     }
                       // +++ NOTE: These equations must match those in bcOpt.bf +++
                                              int ms2 = direction==0 ? 1 : direction==1 ? width : width*width;  // shift for 2nd-order stencil in a fourth order operator
                                              ms2*=1-2*side;
                                              real fi=0.;  // RHS for equation-bc is zero for level>0
                               	     if( numberOfDimensions==2 )
                               	     {
                                 	       if( axis==0 )
                                 	       {
                                 		 if( usingConstantCoefficients )
                                 		 {
                                   		   FOR_3S(i1,i2,i3,J1,J2,J3)
                                   		   {
                                     		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                              if( MASK(j1,j2,j3)>0 )
                                                              {
                                       		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                                  if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                                       		       DEFECT(i1,i2,i3)=fi-(U(j1,j2+1,j3)+U(j1,j2-1,j3))/(dx[1]*dx[1]);  // equation      
                                     		     }
                                                              else
                                     		     {
                                       		       DEFECT(j1,j2,j3)=0.;
                                                                  DEFECT(i1,i2,i3)=0.;
                                     		     }
                                   		   }
                                 		 }
                                 		 else
                                 		 {
                                   		   FOR_3S(i1,i2,i3,J1,J2,J3)
                                   		   {
                                     		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                              if( MASK(j1,j2,j3)>0 )
                                                              {
                                       		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                                  if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                                       		       DEFECT(i1,i2,i3)=fi-(  // eqn to 2nd order
                                       		       COEFF(m22+ms2,i1,i2,i3)*U(j1-1,j2-1,j3)+COEFF(m32+ms2,i1,i2,i3)*U(j1,j2-1,j3)+COEFF(m42+ms2,i1,i2,i3)*U(j1+1,j2-1,j3)+
          		    // COEFF(m23+ms2,i1,i2,i3)*U(j1-1,j2  ,j3)+COEFF(m33+ms2,i1,i2,i3)*U(j1,j2  ,j3)+COEFF(m43+ms2,i1,i2,i3)*U(j1+1,j2  ,j3)+
                                       		       COEFF(m24+ms2,i1,i2,i3)*U(j1-1,j2+1,j3)+COEFF(m34+ms2,i1,i2,i3)*U(j1,j2+1,j3)+COEFF(m44+ms2,i1,i2,i3)*U(j1+1,j2+1,j3) 
                                       			 );
                                     		     }
                                                              else
                                     		     {
                                       		       DEFECT(j1,j2,j3)=0.;
                                                                  DEFECT(i1,i2,i3)=0.;
                                     		     }
                                   		   }
                                 		 }
                                 	       }
                                 	       else if( axis==1 )
                                 	       {
                                 		 if( usingConstantCoefficients )
                                 		 {
                                   		   FOR_3S(i1,i2,i3,J1,J2,J3)
                                   		   {
                                     		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                              if( MASK(j1,j2,j3)>0 )
                                                              {
                                       		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                                  if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                                       		       DEFECT(i1,i2,i3)=fi-(U(j1+1,j2,j3)+U(j1-1,j2,j3))/(dx[0]*dx[0]);  // equation      
                                     		     }
                                                              else
                                     		     {
                                       		       DEFECT(j1,j2,j3)=0.;
                                                                  DEFECT(i1,i2,i3)=0.;
                                     		     }
                                   		   }
                                 		 }
                                 		 else
                                 		 {
                                   		   FOR_3S(i1,i2,i3,J1,J2,J3)
                                   		   {
                                     		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                              if( MASK(j1,j2,j3)>0 )
                                                              {
                                       		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                                  if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                                       		       DEFECT(i1,i2,i3)=fi-(  // eqn to 2nd order
                                   		   COEFF(m22+ms2,i1,i2,i3)*U(j1-1,j2-1,j3)/* +COEFF(m32+ms2,i1,i2,i3)*U(j1,j2-1,j3) */+COEFF(m42+ms2,i1,i2,i3)*U(j1+1,j2-1,j3)+
                                   		   COEFF(m23+ms2,i1,i2,i3)*U(j1-1,j2  ,j3)/* +COEFF(m33+ms2,i1,i2,i3)*U(j1,j2  ,j3) */+COEFF(m43+ms2,i1,i2,i3)*U(j1+1,j2  ,j3)+
                                   		   COEFF(m24+ms2,i1,i2,i3)*U(j1-1,j2+1,j3)/* +COEFF(m34+ms2,i1,i2,i3)*U(j1,j2+1,j3) */+COEFF(m44+ms2,i1,i2,i3)*U(j1+1,j2+1,j3) 
                                       		       );
                                     		     }
                                                              else
                                     		     {
                                       		       DEFECT(j1,j2,j3)=0.;
                                                                  DEFECT(i1,i2,i3)=0.;
                                     		     }
                                   		   }
                                 		 }
                                 	       }
                                 	       else
                                 	       {
                                                      OV_ABORT("ERROR: invalid axis");
                                 	       }
                               	     }
                               	     else if( numberOfDimensions==3 )
                               	     {
                                 	       if( axis==0 )
                                 	       {
                                 		 if( usingConstantCoefficients )
                                 		 {
                                   		   FOR_3S(i1,i2,i3,J1,J2,J3)
                                   		   {
                                     		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                              if( MASK(j1,j2,j3)>0 )
                                                              {
                                       		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                                  if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                                       		       DEFECT(i1,i2,i3)=fi-( (U(j1,j2+1,j3)+U(j1,j2-1,j3))/(dx[1]*dx[1])
                                                        				            +(U(j1,j2,j3+1)+U(j1,j2,j3-1))/(dx[2]*dx[2]) );
                                     		     }
                                                              else
                                     		     {
                                       		       DEFECT(j1,j2,j3)=0.;
                                                                  DEFECT(i1,i2,i3)=0.;
                                     		     }
                                   		   }
                                 		 }
                                 		 else
                                 		 {
                                   		   FOR_3S(i1,i2,i3,J1,J2,J3)
                                   		   {
                                     		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                              if( MASK(j1,j2,j3)>0 )
                                                              {
                                       		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                                  if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                                       		       DEFECT(i1,i2,i3)=fi-(  // eqn to 2nd order
                                              COEFF(m222+ms2,i1,i2,i3)*U(j1-1,j2-1,j3-1)+COEFF(m322+ms2,i1,i2,i3)*U(j1,j2-1,j3-1)+COEFF(m422+ms2,i1,i2,i3)*U(j1+1,j2-1,j3-1)+
                                              COEFF(m232+ms2,i1,i2,i3)*U(j1-1,j2  ,j3-1)+COEFF(m332+ms2,i1,i2,i3)*U(j1,j2  ,j3-1)+COEFF(m432+ms2,i1,i2,i3)*U(j1+1,j2  ,j3-1)+
                                              COEFF(m242+ms2,i1,i2,i3)*U(j1-1,j2+1,j3-1)+COEFF(m342+ms2,i1,i2,i3)*U(j1,j2+1,j3-1)+COEFF(m442+ms2,i1,i2,i3)*U(j1+1,j2+1,j3-1)+
                                              COEFF(m223+ms2,i1,i2,i3)*U(j1-1,j2-1,j3  )+COEFF(m323+ms2,i1,i2,i3)*U(j1,j2-1,j3  )+COEFF(m423+ms2,i1,i2,i3)*U(j1+1,j2-1,j3  )+
          	//   COEFF(m233+ms2,i1,i2,i3)*U(j1-1,j2  ,j3  )+COEFF(m333+ms2,i1,i2,i3)*U(j1,j2  ,j3  )+COEFF(m433+ms2,i1,i2,i3)*U(j1+1,j2  ,j3  )+
                                              COEFF(m243+ms2,i1,i2,i3)*U(j1-1,j2+1,j3  )+COEFF(m343+ms2,i1,i2,i3)*U(j1,j2+1,j3  )+COEFF(m443+ms2,i1,i2,i3)*U(j1+1,j2+1,j3  )+
                                              COEFF(m224+ms2,i1,i2,i3)*U(j1-1,j2-1,j3+1)+COEFF(m324+ms2,i1,i2,i3)*U(j1,j2-1,j3+1)+COEFF(m424+ms2,i1,i2,i3)*U(j1+1,j2-1,j3+1)+
                                              COEFF(m234+ms2,i1,i2,i3)*U(j1-1,j2  ,j3+1)+COEFF(m334+ms2,i1,i2,i3)*U(j1,j2  ,j3+1)+COEFF(m434+ms2,i1,i2,i3)*U(j1+1,j2  ,j3+1)+
                                              COEFF(m244+ms2,i1,i2,i3)*U(j1-1,j2+1,j3+1)+COEFF(m344+ms2,i1,i2,i3)*U(j1,j2+1,j3+1)+COEFF(m444+ms2,i1,i2,i3)*U(j1+1,j2+1,j3+1) 
                                       		       );
                                     		     }
                                                              else
                                     		     {
                                       		       DEFECT(j1,j2,j3)=0.;
                                                                  DEFECT(i1,i2,i3)=0.;
                                     		     }
                                   		   }
                                 		 }
                                 	       }
                                 	       else if( axis==1 )
                                 	       {
                                 		 if( usingConstantCoefficients )
                                 		 {
                                   		   FOR_3S(i1,i2,i3,J1,J2,J3)
                                   		   {
                                     		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                              if( MASK(j1,j2,j3)>0 )
                                                              {
                                       		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                                  if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                                       		       DEFECT(i1,i2,i3)=fi-( (U(j1+1,j2,j3)+U(j1-1,j2,j3))/(dx[0]*dx[0])
                                                      					    +(U(j1,j2,j3+1)+U(j1,j2,j3-1))/(dx[2]*dx[2]) );
                                     		     }
                                                              else
                                     		     {
                                       		       DEFECT(j1,j2,j3)=0.;
                                                                  DEFECT(i1,i2,i3)=0.;
                                     		     }
                                   		   }
                                 		 }
                                 		 else
                                 		 {
                                   		   FOR_3S(i1,i2,i3,J1,J2,J3)
                                   		   {
                                     		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                              if( MASK(j1,j2,j3)>0 )
                                                              {
                                       		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                                  if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                                       		       DEFECT(i1,i2,i3)=fi-(
                                              COEFF(m222+ms2,i1,i2,i3)*U(j1-1,j2-1,j3-1)+COEFF(m322+ms2,i1,i2,i3)*U(j1,j2-1,j3-1)+COEFF(m422+ms2,i1,i2,i3)*U(j1+1,j2-1,j3-1)+
                                              COEFF(m232+ms2,i1,i2,i3)*U(j1-1,j2  ,j3-1)+COEFF(m332+ms2,i1,i2,i3)*U(j1,j2  ,j3-1)+COEFF(m432+ms2,i1,i2,i3)*U(j1+1,j2  ,j3-1)+
                                              COEFF(m242+ms2,i1,i2,i3)*U(j1-1,j2+1,j3-1)+COEFF(m342+ms2,i1,i2,i3)*U(j1,j2+1,j3-1)+COEFF(m442+ms2,i1,i2,i3)*U(j1+1,j2+1,j3-1)+
                                              COEFF(m223+ms2,i1,i2,i3)*U(j1-1,j2-1,j3  )+/*COEFF(m323+ms2,i1,i2,i3)*U(j1,j2-1,j3  )*/+COEFF(m423+ms2,i1,i2,i3)*U(j1+1,j2-1,j3  )+
                               	     COEFF(m233+ms2,i1,i2,i3)*U(j1-1,j2  ,j3  )+/*COEFF(m333+ms2,i1,i2,i3)*U(j1,j2  ,j3  )*/+COEFF(m433+ms2,i1,i2,i3)*U(j1+1,j2  ,j3  )+
                                              COEFF(m243+ms2,i1,i2,i3)*U(j1-1,j2+1,j3  )+/*COEFF(m343+ms2,i1,i2,i3)*U(j1,j2+1,j3  )*/+COEFF(m443+ms2,i1,i2,i3)*U(j1+1,j2+1,j3  )+
                                              COEFF(m224+ms2,i1,i2,i3)*U(j1-1,j2-1,j3+1)+COEFF(m324+ms2,i1,i2,i3)*U(j1,j2-1,j3+1)+COEFF(m424+ms2,i1,i2,i3)*U(j1+1,j2-1,j3+1)+
                                              COEFF(m234+ms2,i1,i2,i3)*U(j1-1,j2  ,j3+1)+COEFF(m334+ms2,i1,i2,i3)*U(j1,j2  ,j3+1)+COEFF(m434+ms2,i1,i2,i3)*U(j1+1,j2  ,j3+1)+
                                              COEFF(m244+ms2,i1,i2,i3)*U(j1-1,j2+1,j3+1)+COEFF(m344+ms2,i1,i2,i3)*U(j1,j2+1,j3+1)+COEFF(m444+ms2,i1,i2,i3)*U(j1+1,j2+1,j3+1) 
                                       		       );
                                       		     }
                                                              else
                                     		     {
                                       		       DEFECT(j1,j2,j3)=0.;
                                                                  DEFECT(i1,i2,i3)=0.;
                                     		     }
                                   		   }
                                 		 }
                                 	       }
                                 	       else // axis==2
                                 	       {
                                 		 if( usingConstantCoefficients )
                                 		 {
                                   		   FOR_3S(i1,i2,i3,J1,J2,J3)
                                   		   {
                                     		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                              if( MASK(j1,j2,j3)>0 )
                                                              {
                                       		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                                  if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                                       		       DEFECT(i1,i2,i3)=fi-( (U(j1+1,j2,j3)+U(j1-1,j2,j3))/(dx[0]*dx[0])
                                                      					    +(U(j1,j2+1,j3)+U(j1,j2-1,j3))/(dx[1]*dx[1]) );
                                     		     }
                                                              else
                                     		     {
                                       		       DEFECT(j1,j2,j3)=0.;
                                                                  DEFECT(i1,i2,i3)=0.;
                                     		     }
                                   		   }
                                 		 }
                                 		 else
                                 		 {
                                   		   FOR_3S(i1,i2,i3,J1,J2,J3)
                                   		   {
                                     		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                              if( MASK(j1,j2,j3)>0 )
                                                              {
                                       		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                                  if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                                       		       DEFECT(i1,i2,i3)=fi-(
                                              COEFF(m222+ms2,i1,i2,i3)*U(j1-1,j2-1,j3-1)+COEFF(m322+ms2,i1,i2,i3)*U(j1,j2-1,j3-1)+COEFF(m422+ms2,i1,i2,i3)*U(j1+1,j2-1,j3-1)+
                                              COEFF(m232+ms2,i1,i2,i3)*U(j1-1,j2  ,j3-1)+/*COEFF(m332+ms2,i1,i2,i3)*U(j1,j2  ,j3-1)*/+COEFF(m432+ms2,i1,i2,i3)*U(j1+1,j2  ,j3-1)+
                                              COEFF(m242+ms2,i1,i2,i3)*U(j1-1,j2+1,j3-1)+COEFF(m342+ms2,i1,i2,i3)*U(j1,j2+1,j3-1)+COEFF(m442+ms2,i1,i2,i3)*U(j1+1,j2+1,j3-1)+
                                              COEFF(m223+ms2,i1,i2,i3)*U(j1-1,j2-1,j3  )+COEFF(m323+ms2,i1,i2,i3)*U(j1,j2-1,j3  )+COEFF(m423+ms2,i1,i2,i3)*U(j1+1,j2-1,j3  )+
                               	     COEFF(m233+ms2,i1,i2,i3)*U(j1-1,j2  ,j3  )+/*COEFF(m333+ms2,i1,i2,i3)*U(j1,j2  ,j3  )*/+COEFF(m433+ms2,i1,i2,i3)*U(j1+1,j2  ,j3  )+
                                              COEFF(m243+ms2,i1,i2,i3)*U(j1-1,j2+1,j3  )+COEFF(m343+ms2,i1,i2,i3)*U(j1,j2+1,j3  )+COEFF(m443+ms2,i1,i2,i3)*U(j1+1,j2+1,j3  )+
                                              COEFF(m224+ms2,i1,i2,i3)*U(j1-1,j2-1,j3+1)+COEFF(m324+ms2,i1,i2,i3)*U(j1,j2-1,j3+1)+COEFF(m424+ms2,i1,i2,i3)*U(j1+1,j2-1,j3+1)+
                                              COEFF(m234+ms2,i1,i2,i3)*U(j1-1,j2  ,j3+1)+/*COEFF(m334+ms2,i1,i2,i3)*U(j1,j2  ,j3+1)*/+COEFF(m434+ms2,i1,i2,i3)*U(j1+1,j2  ,j3+1)+
                                              COEFF(m244+ms2,i1,i2,i3)*U(j1-1,j2+1,j3+1)+COEFF(m344+ms2,i1,i2,i3)*U(j1,j2+1,j3+1)+COEFF(m444+ms2,i1,i2,i3)*U(j1+1,j2+1,j3+1) 
                                       		       );
                                       		     }
                                                              else
                                     		     {
                                       		       DEFECT(j1,j2,j3)=0.;
                                                                  DEFECT(i1,i2,i3)=0.;
                                     		     }
                                   		   }
                                 		 }
                                 	       }
                               	     }
                               	     else
                               	     {
                                 	       Overture::abort();
                               	     }
                             	   }
                           	 }
                                }
                                else if( bcLocal(side,axis)==OgmgParameters::equation )
                                {
          	 // *****************************************************
          	 // ************ neumann  or mixed **********************
          	 // *****************************************************
                           	 J1=I1z; J2=I2z; J3=I3z;
                           	 Jv[axis]=side==0 ? Izv[axis].getBase() : Izv[axis].getBound();
                   // *********** FIX ME for non-orthogonal curvilinear *********************8
                           	 if( orderOfAccuracy==2 )
                           	 {
          	   // *** this case is now handled below in lineSmoothRHS
                             	   if( true )
                             	   {
                             	   }
                             	   else if( level==0 )
                             	   {
                               	     FOR_3S(i1,i2,i3,J1,J2,J3)
                               	     {
                                 	       j1=i1+is1; j2=i2+is2; j3=i3+is3;
                                 	       if( MASK(j1,j2,j3)>0 )
                                 	       {
                                 		 DEFECT(i1,i2,i3)=F(i1,i2,i3);   // neumann BC
                                 	       }
                                 	       else
                                 	       {
                                 		 DEFECT(i1,i2,i3)=0.;
                                 	       }
                               	     }
                             	   }
                             	   else
                             	   {
                               	     FOR_3S(i1,i2,i3,J1,J2,J3)
                               	     {
                                 	       DEFECT(i1,i2,i3)=0.;           // neumann BC
                               	     }
                             	   }
                           	 }
                           	 else // fourth-order
                           	 {
                                          assert( orderOfAccuracy==4 );
                     // **** fix this ****
                                          if( useEquationOnGhostForNeumann )
                             	   {
                       // *** these values are now filled in below 
                                              if( true )
                               	     {
                               	     }
                               	     else if( level==0 || level>0 )
                               	     {
                                 	       if( usingConstantCoefficients )
                                 	       {
          		 // Neumann and  u.xxx = f.x - g.yy
                           // i1,i2,i3 = 2nd ghost
                           // j1,j2,j3 = 1st ghost
                                 		 FOR_3S(i1,i2,i3,J1,J2,J3)
                                 		 {
                                   		   j1=i1+is1; j2=i2+is2; j3=i3+is3;
                                   		   fPrintF(debugFile,"N4Lx: i=(%i,%i,%i) j=(%i,%i,%i) F=%8.2e\n",i1,i2,i3,j1,j2,j3,F(j1,j2,j3));
                                   		   if( MASK(j1+is1,j2+is2,j3+is3)>0 ) // check the mask on the boundary
                                   		   {
                                     		     DEFECT(j1,j2,j3)=F(j1,j2,j3);   // neumann BC
                                     		     DEFECT(i1,i2,i3)=0.;            // u.xxx=
                                   		   }
                                   		   else
                                   		   {
                                     		     DEFECT(j1,j2,j3)=0.;
                                     		     DEFECT(i1,i2,i3)=0.;
                                   		   }
                                 		 }
                                 	       }
                                 	       else
                                 	       {
                                    	          Overture::abort("lineSolve: Error useEquationOnGhostForNeumann not implemented.");
                                 	       }
                               	     }
                             	   }
                                          else
                                          {
          	     // extrapolation for 2nd ghost or symmetry
                               	     if( level==0 )
                               	     {
                                                  assert( bcOptionN==0 );
                                 	       if( usingConstantCoefficients )
                                 	       {
          		 // Neumann + extrapolation for rectangular grids.
                                 		 FOR_3S(i1,i2,i3,J1,J2,J3)
                                 		 {
                                   		   j1=i1+is1; j2=i2+is2; j3=i3+is3;
          		   // fPrintF(debugFile,"N4: i=(%i,%i,%i) j=(%i,%i,%i) F=%8.2e\n",i1,i2,i3,j1,j2,j3,F(j1,j2,j3));
                                   		   if( MASK(j1+is1,j2+is2,j3+is3)>0 ) // check the mask on the boundary
                                   		   {
                                     		     DEFECT(j1,j2,j3)=F(j1,j2,j3);   // neumann BC
          		     // *wdh* 110309 DEFECT(i1,i2,i3)=0.;            // extrapolation: second ghost line                    
                              // extrapolation: second ghost line
                                                            if( orderOfExtrapN <= 4 )
                                                              DEFECT(i1,i2,i3)=0.;            
                                                            else if( orderOfExtrapN == 5 )
                               // For 5th order extrapolation we need to set the RHS since the formula does not fit in the penta matrix
                               // 1 -5 10 -10 5 -1 
                                                              DEFECT(i1,i2,i3)=U(i1+5*is1,i2+5*is2,i3+5*is3);
                                                            else
                                                              OV_ABORT("ERROR:lineSmooth: orderOfExtrapN invalid");
                                   		   }
                                   		   else
                                   		   {
                                     		     DEFECT(j1,j2,j3)=0.;
                                     		     DEFECT(i1,i2,i3)=0.;
                                   		   }
                                 		 }
                                 	       }
                                 	       else
                                 	       {
                                 		 if( numberOfDimensions==2 && direction==0 )
                                 		 {
                         // 		   neumannExtrapCurvilinear(R,2,FOR_3S,Izv,I1z,I2z,I3z);
                                                  FOR_3S(i1,i2,i3,J1,J2,J3)
                                                  {
                                                      j1=i1+is1; j2=i2+is2; j3=i3+is3; // first ghost line 
                         //    fPrintF(debugFile,"N4: i=(%i,%i,%i) j=(%i,%i,%i) F=%8.2e C=[%6.2f,%6.2f,%6.2f,%6.2f,%6.2f], "
                         //           " [%6.2f,%6.2f,%6.2f,%6.2f]\n",
                         //             i1,i2,i3,j1,j2,j3,
                         //              F(j1,j2,j3),COEFF(m13,j1,j2,j3),COEFF(m23,j1,j2,j3),COEFF(m33,j1,j2,j3),
                         //               COEFF(m43,j1,j2,j3),COEFF(m53,j1,j2,j3),
                         //               COEFF(m31,j1,j2,j3),COEFF(m32,j1,j2,j3),COEFF(m34,j1,j2,j3),COEFF(m35,j1,j2,j3));
                                                      if( MASK(j1+is1,j2+is2,j3+is3)>0 ) // check the mask on the boundary
                                                      {
                           //                            #If "2" == "2" && "R" == "R"
                             // neumann BC: nd=2 dir=0
                                                          DEFECT(j1,j2,j3)=F(j1,j2,j3)- (
                                                                COEFF(m31,j1,j2,j3)*U(j1+is1,j2-2,j3)
                                                              +COEFF(m32,j1,j2,j3)*U(j1+is1,j2-1,j3)
                                                              +COEFF(m34,j1,j2,j3)*U(j1+is1,j2+1,j3)
                                                              +COEFF(m35,j1,j2,j3)*U(j1+is1,j2+2,j3) );
                             // extrapolation: second ghost line
                                                          if( orderOfExtrapN <= 4 )
                                                              DEFECT(i1,i2,i3)=0.;            
                                                          else if( orderOfExtrapN == 5 )
                               // For 5th order extrapolation we need to set the RHS since the formula does not fit in the penta matrix
                               // 1 -5 10 -10 5 -1 
                                                              DEFECT(i1,i2,i3)=U(i1+5*is1,i2+5*is2,i3+5*is3);
                                                          else
                                                              OV_ABORT("ERROR:lineSmooth: orderOfExtrapN invalid");
                                                      }
                                                      else
                                                      {
                                                          DEFECT(j1,j2,j3)=0.;
                                                          DEFECT(i1,i2,i3)=0.;
                                                      }
                                                  }
                                 		 }
                                 		 else if(  numberOfDimensions==2 && direction==1 )
                                 		 {
                         // 		   neumannExtrapCurvilinear(S,2,FOR_3S,Izv,I1z,I2z,I3z);
                                                  FOR_3S(i1,i2,i3,J1,J2,J3)
                                                  {
                                                      j1=i1+is1; j2=i2+is2; j3=i3+is3; // first ghost line 
                         //    fPrintF(debugFile,"N4: i=(%i,%i,%i) j=(%i,%i,%i) F=%8.2e C=[%6.2f,%6.2f,%6.2f,%6.2f,%6.2f], "
                         //           " [%6.2f,%6.2f,%6.2f,%6.2f]\n",
                         //             i1,i2,i3,j1,j2,j3,
                         //              F(j1,j2,j3),COEFF(m13,j1,j2,j3),COEFF(m23,j1,j2,j3),COEFF(m33,j1,j2,j3),
                         //               COEFF(m43,j1,j2,j3),COEFF(m53,j1,j2,j3),
                         //               COEFF(m31,j1,j2,j3),COEFF(m32,j1,j2,j3),COEFF(m34,j1,j2,j3),COEFF(m35,j1,j2,j3));
                                                      if( MASK(j1+is1,j2+is2,j3+is3)>0 ) // check the mask on the boundary
                                                      {
                           //                            #If "2" == "2" && "S" == "R"
                           //                            #Elif "2" == "2" && "S" == "S"
                                                          DEFECT(j1,j2,j3)=F(j1,j2,j3)- (
                                                                COEFF(m13,j1,j2,j3)*U(j1-2,j2+is2,j3)
                                                              +COEFF(m23,j1,j2,j3)*U(j1-1,j2+is2,j3)
                                                              +COEFF(m43,j1,j2,j3)*U(j1+1,j2+is2,j3)
                                                              +COEFF(m53,j1,j2,j3)*U(j1+2,j2+is2,j3) );
                             // extrapolation: second ghost line
                                                          if( orderOfExtrapN <= 4 )
                                                              DEFECT(i1,i2,i3)=0.;            
                                                          else if( orderOfExtrapN == 5 )
                               // For 5th order extrapolation we need to set the RHS since the formula does not fit in the penta matrix
                               // 1 -5 10 -10 5 -1 
                                                              DEFECT(i1,i2,i3)=U(i1+5*is1,i2+5*is2,i3+5*is3);
                                                          else
                                                              OV_ABORT("ERROR:lineSmooth: orderOfExtrapN invalid");
                                                      }
                                                      else
                                                      {
                                                          DEFECT(j1,j2,j3)=0.;
                                                          DEFECT(i1,i2,i3)=0.;
                                                      }
                                                  }
                                 		 }
                                 		 else if(  numberOfDimensions==3 && direction==0 )
                                 		 {
                         // 		   neumannExtrapCurvilinear(R,3,FOR_3S,Izv,I1z,I2z,I3z);
                                                  FOR_3S(i1,i2,i3,J1,J2,J3)
                                                  {
                                                      j1=i1+is1; j2=i2+is2; j3=i3+is3; // first ghost line 
                         //    fPrintF(debugFile,"N4: i=(%i,%i,%i) j=(%i,%i,%i) F=%8.2e C=[%6.2f,%6.2f,%6.2f,%6.2f,%6.2f], "
                         //           " [%6.2f,%6.2f,%6.2f,%6.2f]\n",
                         //             i1,i2,i3,j1,j2,j3,
                         //              F(j1,j2,j3),COEFF(m13,j1,j2,j3),COEFF(m23,j1,j2,j3),COEFF(m33,j1,j2,j3),
                         //               COEFF(m43,j1,j2,j3),COEFF(m53,j1,j2,j3),
                         //               COEFF(m31,j1,j2,j3),COEFF(m32,j1,j2,j3),COEFF(m34,j1,j2,j3),COEFF(m35,j1,j2,j3));
                                                      if( MASK(j1+is1,j2+is2,j3+is3)>0 ) // check the mask on the boundary
                                                      {
                           //                            #If "3" == "2" && "R" == "R"
                           //                            #Elif "3" == "2" && "R" == "S"
                           //                            #Elif "3" == "3" && "R" == "R"
                             // neumann BC: nd=3 dir=0
                                                          DEFECT(j1,j2,j3)=F(j1,j2,j3)- (
                                                                COEFF(m313,j1,j2,j3)*U(j1+is1,j2-2,j3)
                                                              +COEFF(m323,j1,j2,j3)*U(j1+is1,j2-1,j3)
                                                              +COEFF(m343,j1,j2,j3)*U(j1+is1,j2+1,j3)
                                                              +COEFF(m353,j1,j2,j3)*U(j1+is1,j2+2,j3)
                                                              +COEFF(m331,j1,j2,j3)*U(j1+is1,j2  ,j3-2)
                                                              +COEFF(m332,j1,j2,j3)*U(j1+is1,j2  ,j3-1)
                                                              +COEFF(m334,j1,j2,j3)*U(j1+is1,j2  ,j3+1)
                                                              +COEFF(m335,j1,j2,j3)*U(j1+is1,j2  ,j3+2) );
                             // extrapolation: second ghost line
                                                          if( orderOfExtrapN <= 4 )
                                                              DEFECT(i1,i2,i3)=0.;            
                                                          else if( orderOfExtrapN == 5 )
                               // For 5th order extrapolation we need to set the RHS since the formula does not fit in the penta matrix
                               // 1 -5 10 -10 5 -1 
                                                              DEFECT(i1,i2,i3)=U(i1+5*is1,i2+5*is2,i3+5*is3);
                                                          else
                                                              OV_ABORT("ERROR:lineSmooth: orderOfExtrapN invalid");
                                                      }
                                                      else
                                                      {
                                                          DEFECT(j1,j2,j3)=0.;
                                                          DEFECT(i1,i2,i3)=0.;
                                                      }
                                                  }
                                 		 }
                                 		 else if(  numberOfDimensions==3 && direction==1 )
                                 		 {
                         // 		   neumannExtrapCurvilinear(S,3,FOR_3S,Izv,I1z,I2z,I3z);
                                                  FOR_3S(i1,i2,i3,J1,J2,J3)
                                                  {
                                                      j1=i1+is1; j2=i2+is2; j3=i3+is3; // first ghost line 
                         //    fPrintF(debugFile,"N4: i=(%i,%i,%i) j=(%i,%i,%i) F=%8.2e C=[%6.2f,%6.2f,%6.2f,%6.2f,%6.2f], "
                         //           " [%6.2f,%6.2f,%6.2f,%6.2f]\n",
                         //             i1,i2,i3,j1,j2,j3,
                         //              F(j1,j2,j3),COEFF(m13,j1,j2,j3),COEFF(m23,j1,j2,j3),COEFF(m33,j1,j2,j3),
                         //               COEFF(m43,j1,j2,j3),COEFF(m53,j1,j2,j3),
                         //               COEFF(m31,j1,j2,j3),COEFF(m32,j1,j2,j3),COEFF(m34,j1,j2,j3),COEFF(m35,j1,j2,j3));
                                                      if( MASK(j1+is1,j2+is2,j3+is3)>0 ) // check the mask on the boundary
                                                      {
                           //                            #If "3" == "2" && "S" == "R"
                           //                            #Elif "3" == "2" && "S" == "S"
                           //                            #Elif "3" == "3" && "S" == "R"
                           //                            #Elif "3" == "3" && "S" == "S"
                                                          DEFECT(j1,j2,j3)=F(j1,j2,j3)- (
                                                                COEFF(m133,j1,j2,j3)*U(j1-2,j2+is2,j3)
                                                              +COEFF(m233,j1,j2,j3)*U(j1-1,j2+is2,j3)
                                                              +COEFF(m433,j1,j2,j3)*U(j1+1,j2+is2,j3)
                                                              +COEFF(m533,j1,j2,j3)*U(j1+2,j2+is2,j3)
                                                              +COEFF(m331,j1,j2,j3)*U(j1  ,j2+is2,j3-2)
                                                              +COEFF(m332,j1,j2,j3)*U(j1  ,j2+is2,j3-1)
                                                              +COEFF(m334,j1,j2,j3)*U(j1  ,j2+is2,j3+1)
                                                              +COEFF(m335,j1,j2,j3)*U(j1  ,j2+is2,j3+2) );
                             // extrapolation: second ghost line
                                                          if( orderOfExtrapN <= 4 )
                                                              DEFECT(i1,i2,i3)=0.;            
                                                          else if( orderOfExtrapN == 5 )
                               // For 5th order extrapolation we need to set the RHS since the formula does not fit in the penta matrix
                               // 1 -5 10 -10 5 -1 
                                                              DEFECT(i1,i2,i3)=U(i1+5*is1,i2+5*is2,i3+5*is3);
                                                          else
                                                              OV_ABORT("ERROR:lineSmooth: orderOfExtrapN invalid");
                                                      }
                                                      else
                                                      {
                                                          DEFECT(j1,j2,j3)=0.;
                                                          DEFECT(i1,i2,i3)=0.;
                                                      }
                                                  }
                                 		 }
                                 		 else if(  numberOfDimensions==3 && direction==2 )
                                 		 {
                         // 		   neumannExtrapCurvilinear(T,3,FOR_3S,Izv,I1z,I2z,I3z);
                                                  FOR_3S(i1,i2,i3,J1,J2,J3)
                                                  {
                                                      j1=i1+is1; j2=i2+is2; j3=i3+is3; // first ghost line 
                         //    fPrintF(debugFile,"N4: i=(%i,%i,%i) j=(%i,%i,%i) F=%8.2e C=[%6.2f,%6.2f,%6.2f,%6.2f,%6.2f], "
                         //           " [%6.2f,%6.2f,%6.2f,%6.2f]\n",
                         //             i1,i2,i3,j1,j2,j3,
                         //              F(j1,j2,j3),COEFF(m13,j1,j2,j3),COEFF(m23,j1,j2,j3),COEFF(m33,j1,j2,j3),
                         //               COEFF(m43,j1,j2,j3),COEFF(m53,j1,j2,j3),
                         //               COEFF(m31,j1,j2,j3),COEFF(m32,j1,j2,j3),COEFF(m34,j1,j2,j3),COEFF(m35,j1,j2,j3));
                                                      if( MASK(j1+is1,j2+is2,j3+is3)>0 ) // check the mask on the boundary
                                                      {
                           //                            #If "3" == "2" && "T" == "R"
                           //                            #Elif "3" == "2" && "T" == "S"
                           //                            #Elif "3" == "3" && "T" == "R"
                           //                            #Elif "3" == "3" && "T" == "S"
                           //                            #Elif "3" == "3" && "T" == "T"
                                                          DEFECT(j1,j2,j3)=F(j1,j2,j3)- (
                                                                COEFF(m133,j1,j2,j3)*U(j1-2,j2  ,j3+is3)
                                                              +COEFF(m233,j1,j2,j3)*U(j1-1,j2  ,j3+is3)
                                                              +COEFF(m433,j1,j2,j3)*U(j1+1,j2  ,j3+is3)
                                                              +COEFF(m533,j1,j2,j3)*U(j1+2,j2  ,j3+is3)
                                                              +COEFF(m313,j1,j2,j3)*U(j1  ,j2-2,j3+is3)
                                                              +COEFF(m323,j1,j2,j3)*U(j1  ,j2-1,j3+is3)
                                                              +COEFF(m343,j1,j2,j3)*U(j1  ,j2+1,j3+is3)
                                                              +COEFF(m353,j1,j2,j3)*U(j1  ,j2+2,j3+is3) );
                             // extrapolation: second ghost line
                                                          if( orderOfExtrapN <= 4 )
                                                              DEFECT(i1,i2,i3)=0.;            
                                                          else if( orderOfExtrapN == 5 )
                               // For 5th order extrapolation we need to set the RHS since the formula does not fit in the penta matrix
                               // 1 -5 10 -10 5 -1 
                                                              DEFECT(i1,i2,i3)=U(i1+5*is1,i2+5*is2,i3+5*is3);
                                                          else
                                                              OV_ABORT("ERROR:lineSmooth: orderOfExtrapN invalid");
                                                      }
                                                      else
                                                      {
                                                          DEFECT(j1,j2,j3)=0.;
                                                          DEFECT(i1,i2,i3)=0.;
                                                      }
                                                  }
                                 		 }
          //  	       FOR_3S(i1,i2,i3,J1,J2,J3)
          //  	       {
          //  		 j1=i1+is1; j2=i2+is2; j3=i3+is3;
          //  		 fPrintF(debugFile,"N4: i=(%i,%i,%i) j=(%i,%i,%i) F=%8.2e\n",i1,i2,i3,j1,j2,j3,F(j1,j2,j3));
          //                   // neumann BC: nd=2 dir=0
          //  		 DEFECT(j1,j2,j3)=F(j1,j2,j3)- (
          //                                 COEFF(m31,i1,i2,i3)*U(j1+is1,j2-2,j3)
          //                                +COEFF(m32,i1,i2,i3)*U(j1+is1,j2-1,j3)
          //                                +COEFF(m34,i1,i2,i3)*U(j1+is1,j2+1,j3)
          //  			      +COEFF(m35,i1,i2,i3)*U(j1+is1,j2+1,j3) );
          //  		 DEFECT(i1,i2,i3)=0.;            // extrapolation: second ghost line                    
          //  	       }
                                 	       }
                               	     }
                               	     else // level>0 
                               	     {
                         // assert( parameters.useSymmetryForNeumannOnLowerLevels );
                                                  assert( orderOfExtrapN <= 4 );  // assume this for now
                                 	       FOR_3S(i1,i2,i3,J1,J2,J3)
                                 	       {
                                 		 j1=i1+is1; j2=i2+is2; j3=i3+is3;
                                 		 DEFECT(j1,j2,j3)=0.;           // neumann BC or symmetry or mixed BC
                                 		 DEFECT(i1,i2,i3)=0.;           // second ghost line : extrap or symmetry or mixed BC
                                 	       }
                               	     }
                             	   }
                           	 }
                                }
                                else
                                {
                          	printf(" ERROR: unknown bc = %i\n",bcLocal(side,axis));
                                    Overture::abort("ERROR");
                                }
                            }
                        }
                    }
                    if( true || (useEquationOnGhostForNeumann &&  orderOfAccuracy==4) )
                    {
            // new optimised BC's 
                        ipar[ 4]=I1z.getBase();
                        ipar[ 5]=I1z.getBound();
                        ipar[ 6]=I1z.getStride();
                        ipar[ 7]=I2z.getBase();
                        ipar[ 8]=I2z.getBound();
                        ipar[ 9]=I2z.getStride();
                        ipar[10]=I3z.getBase();
                        ipar[11]=I3z.getBound();
                        ipar[12]=I3z.getStride();
                        const real *pcc = constantCoefficients.getBound(2)>=level ? &constantCoefficients(0,grid,level) : rpar;
                        const real *pbcd = boundaryConditionData.getLength(3)>=grid ? &boundaryConditionData(0,0,0,grid) : rpar;
            // NOTE: do NOT use view pointers here
                        lineSmoothRHS( numberOfDimensions,
                                                      uLocal.getBase(0),uLocal.getBound(0),
                                                      uLocal.getBase(1),uLocal.getBound(1),
                                                      uLocal.getBase(2),uLocal.getBound(2),
                                 		 coeffLocal.getLength(0), *coeffLocal.getDataPointer(),
                                 		 *pcc,
                                 		 *defectLocal.getDataPointer(), 
                                 		 *sptr, *uptr, *fLocal.getDataPointer(), *maskLocal.getDataPointer(), *prsxy, ipar[0], rpar[0], 
                                 		 boundaryConditionData.getLength(0),*pbcd );
                    }
          // Set values at interpolation points  *wdh* 100716 -- do this last!
                    if( mg.numberOfDimensions()==2 )
                    {
                        i3=I3z.getBase();
                        for( int i=0; i<numberOfInterpolationPoints; i++ )
                        {
                            i1=IP(i,0);
                            i2=IP(i,1);
                            DEFECT(i1,i2,i3)=U(i1,i2,i3); 
                        }
                    }
                    else
                    {
                        for( int i=0; i<numberOfInterpolationPoints; i++ )
                        {
                            i1=IP(i,0);
                            i2=IP(i,1);
                            i3=IP(i,2);
                            DEFECT(i1,i2,i3)=U(i1,i2,i3); 
                        }
                    }
        	  }
        	  
        	  tm[timeForRelaxInSmooth]+=getCPU()-time0;

//  	  if( false && level==0 )
//  	  { // define the defect for Neumann or mixed BC's on level 0 at ghost points
//  	    //     -- these may be inhomogeneous
//  	    // **** this assumes BC is normal direction only ******

//              // ???? check this ???????????

//  	    int iv[3]; 
//  	    iv[0]=I1z.getBase(); iv[1]=I2z.getBase(); iv[2]=I3z.getBase();
//  	    if( iv[direction]==mg.gridIndexRange()(Start,direction)-1 && mg.boundaryCondition(Start,direction)>0 )
//  	      DEFECT(iv[0],iv[1],iv[2])=f(iv[0],iv[1],iv[2]);
//  	    iv[0]=I1z.getBound(); iv[1]=I2z.getBound(); iv[2]=I3z.getBound();
//  	    if( iv[direction]==mg.gridIndexRange()(End,direction)+1 && mg.boundaryCondition(End,direction)>0 )
//  	      DEFECT(iv[0],iv[1],iv[2])=f(iv[0],iv[1],iv[2]);
//  	  }	

        	  if( Ogmg::debug & 8 ) 
        	  {
                        fPrintF(debugFile," lineSmooth: get defect for points: D1z,D2z,D3z=[%i,%i][%i,%i][%i,%i]\n",
                		    D1z.getBase(),D1z.getBound(),D2z.getBase(),D2z.getBound(),D3z.getBase(),D3z.getBound());
          	    
          	    display(defectLocal(I1,I2,I3),sPrintF("lineSmooth: Here is rhs before the zebra tridiagonalSolver, "
                		    "level=%i grid=%i ",level,grid),pDebugFile,"%8.1e ");
        	  }
            
        	  if( false && parameters.alternateSmoothingDirections ) // negative strides not allowed
        	  {
            // Reverse the order of the lines we solve -- 
	    // Two possibilities for now:
          	    int numMod = parameters.totalNumberOfSubSmooths(grid,level)+iteration % 2;
          	    if( numMod == 1 )
          	    {
                            if( direction!=axis1 )
              	        I1z=Range(I1z.getBound(),I1z.getBase(),-I1z.getStride());
                            if( direction!=axis2 )
              	        I2z=Range(I2z.getBound(),I2z.getBase(),-I2z.getStride());
                            if( direction!=axis3 )
              	        I3z=Range(I3z.getBound(),I3z.getBase(),-I3z.getStride());
            	      
          	    }
        	  }
        	  
        	  time1=getCPU();

          // -----------------------------------------------------------
          // -------------- Solve the Tridiagonal Systems --------------
          // -----------------------------------------------------------
        	  if( ok )
          	    tridiagonalSolver[level][grid][direction]->solve(defectLocal,I1z,I2z,I3z);

        	  time0=getCPU();
        	  tm[timeForTridiagonalSolverInSmooth]+=time0-time1;

          // ******* TO DO **** add variable omega for 3D *************

        	  if( ok && Ogmg::debug & 8 )
        	  {
                        fprintf(pDebugFile,"\n"
                		    "lineSmooth: I1 ,I2 ,I3 =[%i,%i,%i][%i,%i,%i][%i,%i,%i] \n"
                		    "            I1z,I2z,I3z=[%i,%i,%i][%i,%i,%i][%i,%i,%i] \n",
                		    I1.getBase(),I1.getBound(),I1.getStride(),
                		    I2.getBase(),I2.getBound(),I2.getStride(),
                		    I3.getBase(),I3.getBound(),I3.getStride(),
                		    I1z.getBase(),I1z.getBound(),I1z.getStride(),
                		    I2z.getBase(),I2z.getBound(),I2z.getStride(),
                		    I3z.getBase(),I3z.getBound(),I3z.getStride());
                        fprintf(pDebugFile,
                		    " uLocal      = [%i,%i][%i,%i][%i,%i]\n"
                		    " defectLocal = [%i,%i][%i,%i][%i,%i]\n"
                		    " maskLocal   = [%i,%i][%i,%i][%i,%i]\n"
                		    " coeffLocal  = [%i,%i][%i,%i][%i,%i]\n",
                		    uLocal.getBase(0),uLocal.getBound(0),
                		    uLocal.getBase(1),uLocal.getBound(1),
                		    uLocal.getBase(2),uLocal.getBound(2),
                		    defectLocal.getBase(0),defectLocal.getBound(0),
                		    defectLocal.getBase(1),defectLocal.getBound(1),
                		    defectLocal.getBase(2),defectLocal.getBound(2),
                		    maskLocal.getBase(0),maskLocal.getBound(0),
                		    maskLocal.getBase(1),maskLocal.getBound(1),
                		    maskLocal.getBase(2),maskLocal.getBound(2),
                		    coeffLocal.getBase(1),coeffLocal.getBound(1),
                		    coeffLocal.getBase(2),coeffLocal.getBound(2),
                		    coeffLocal.getBase(3),coeffLocal.getBound(3));

                        fprintf(pDebugFile," orderOfAccuracy=%i variableCoefficients=%i useOmega=%i\n",
                		    orderOfAccuracy,(int)variableCoefficients,(int)useOmega);

          	    display(uLocal,sPrintF("lineSmooth:Here is the solution BEFORE u=defect, level=%i grid=%i ",
                        			      level,grid),pDebugFile,"%9.2e ");

          	    display(defectLocal(I1,I2,I3),"lineSmooth: solution from the zebra tridiagonalSolver ",pDebugFile,"%8.1e ");

        	  }
        	  
        	  if( ok )
        	  {
          	    int ipar2[]={ I1z.getBase(),I1z.getBound(),I1z.getStride(),
                    			  I2z.getBase(),I2z.getBound(),I2z.getStride(), //
                    			  I3z.getBase(),I3z.getBound(),I3z.getStride(), //
                    			  direction,orderOfAccuracy,variableCoefficients,useOmega};

                        real rpar2[]={omega,variableOmegaFactor}; //

            // -----------------------------------------
            // ---- Update the solution ----------------
            // ---- u <- (1-omega)*u + omega*defect ----
            // -----------------------------------------
                        lineSmoothUpdate( mg.numberOfDimensions(),
                                                            uLocal.getBase(0),uLocal.getBound(0),
                                                            uLocal.getBase(1),uLocal.getBound(1),
                                                            uLocal.getBase(2),uLocal.getBound(2),
                        			      *uLocal.getDataPointer(), *defectLocal.getDataPointer(), *maskLocal.getDataPointer(), 
                                                            coeffLocal.getLength(0), *coeffLocal.getDataPointer(),
                                                            ipar2[0], rpar2[0] );
        	  }

        	  tm[timeForRelaxInSmooth]+=getCPU()-time0;

        	  if( ok && Ogmg::debug & 8 )
          	    display(uLocal,sPrintF("lineSmooth:Here is the solution u=defect, level=%i grid=%i ",
                                                  level,grid),pDebugFile,"%9.2e ");

	  // * applyBoundaryConditions( level,uu,ff );
            
          // uu.periodicUpdate();  // ************** added for testing  *WDH* 2013/11/24
        	  

        	  if( Ogmg::debug & 16 )
        	  {
          	    defect(level,grid);
          	    display(defectMG.multigridLevel[level][grid],sPrintF(buff,"lineSmooth: direction=%i, zebra2=%i, defect:",
                                                 								 direction,zebra2),debugFile);
          	    display(u,sPrintF(buff,"lineSmooth: direction=%i: Here is u",direction),debugFile);
        	  }


          // ---------------------------------------------------------------------------------------
          // --------------------- Apply full boundary conditions ----------------------------------
          // ---------------------------------------------------------------------------------------  
        	  applyBoundaryConditions( level,grid,uu,ff );


        	  if( Ogmg::debug & 8 )
        	  {
          	    display(uLocal,sPrintF("lineSmooth: Here is the solution u after applyBoundaryConditions, level=%i grid=%i ",
                           				   level,grid),pDebugFile,"%9.2e ");
        	  }
        	  if(  Ogmg::debug & 8 )
        	  {
          	    defect=0.;
                        getDefect(level,grid,f,u,D1,D2,D3,defect);
          	    if( ok )
            	      display(defectLocal(I1,I2,I3),sPrintF("lineSmooth: Defect after apply boundary conditions, level=%i grid=%i ",
                                     					       level,grid),pDebugFile,"%9.2e ");

        	  }
        	  

      	}
        }
        else   //  ****** Jacobi Line Smooth **************
        {
            real time1=getCPU();

//        printf(" I1=[%i,%i] I2=[%i,%i] I3=[%i,%i]\n",I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),
//  	     I3.getBase(),I3.getBound());
//        printf(" u : [%i,%i][%i,%i][%i,%i]\n",u.getBase(0),u.getBound(0),u.getBase(1),u.getBound(1),u.getBase(2),u.getBound(2));
//        printf(" f : [%i,%i][%i,%i][%i,%i]\n",f.getBase(0),f.getBound(0),f.getBase(1),f.getBound(1),f.getBase(2),f.getBound(2));
//        printf(" defect : [%i,%i][%i,%i][%i,%i]\n",defect.getBase(0),defect.getBound(0),defect.getBase(1),defect.getBound(1),defect.getBase(2),defect.getBound(2));
            
            if( ok && debug & 4 )
      	fPrintF(debugFile,"direction=%i jacobi line smooth I1=(%i,%i,%i), I2=(%i,%i,%i), I3=(%i,%i,%i) \n",
            		direction,
            		I1.getBase(),I1.getBound(),I1.getStride(),I2.getBase(),I2.getBound(),I2.getStride(),
            		I3.getBase(),I3.getBound(),I3.getStride());
            

      // We only need to evaluate the defect at points where the ...
            getDefect(level,grid,f,u,D1,D2,D3,defect,direction);

            time0=getCPU();
            tm[timeForDefectInSmooth]+=time0-time1;

            if( ok )
            {
      // 	lineSmoothBoundaryConditions(FOR_3,Iv,I1,I2,I3);
      // if( false ) // this is done last now
      // {
      // // Set values at interpolation points
      //   if( mg.numberOfDimensions()==2 )
      //   {
      //     i3=I3.getBase();
      //     for( int i=0; i<numberOfInterpolationPoints; i++ )
      //     {
      //       i1=IP(i,0);
      //       i2=IP(i,1);
      //       DEFECT(i1,i2,i3)=U(i1,i2,i3); 
      //     }
      //   }
      //   else
      //   {
      //     for( int i=0; i<numberOfInterpolationPoints; i++ )
      //     {
      //       i1=IP(i,0);
      //       i2=IP(i,1);
      //       i3=IP(i,2);
      //       DEFECT(i1,i2,i3)=U(i1,i2,i3); 
      //     }
      //   }
      // }
      // For the constant coeff case we must fill in the BC's into the defect array
      // for fourth order we need to assign the rhs for extrapolation conditions
            if( (true || usingConstantCoefficients ) // **** we must always assign Neumann BC's -- could avoid dirichlet below
                    || orderOfAccuracy==4 )
            {
        // Boundary Conditions
                Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
                axis=direction;
                for( int side=0; side<=1; side++ )
                {
                    if( bcLocal(side,axis)>0 )
                    {
                        is1=is2=is3=0;
                        isv[axis]=1-2*side;
                        if( bcLocal(side,axis)==OgmgParameters::parallelGhostBoundary )
                        {
               // "Dirichlet" BC at parallel ghost: set to current solution value
                   	 J1=I1; J2=I2; J3=I3;
                   	 Jv[axis]=side==0 ? Iv[axis].getBase() : Iv[axis].getBound();
                   	 if( orderOfAccuracy==2 )
                   	 {
                     	   if( false && debug & 4 )
                       	     fprintf(pDebugFile," lineSmoothRHS: parallelGhostBoundary grid=%i side=%i J1=[%i,%i,%i] J2=[%i,%i,%i] J3=[%i,%i,%i]\n",grid,side,
                                                      J1.getBase(),J1.getBound(),J1.getStride(),
                             		     J2.getBase(),J2.getBound(),J2.getStride(),
                             		     J3.getBase(),J3.getBound(),J3.getStride());
                     	   FOR_3(i1,i2,i3,J1,J2,J3)
                     	   {
      	     // *wdh* 2011/08/26 -- we need to set rhs=u on parallel ghost that are also interp. pts
                   // if( MASK(i1,i2,i3)>0 )
                       	     if( MASK(i1,i2,i3)!=0 )
                       	     {
                         	       DEFECT(i1,i2,i3)=U(i1,i2,i3);
                       	     }
                       	     else
                       	     {
                         	       DEFECT(i1,i2,i3)=0.;
                       	     }
                     	   }
                   	 }
                   	 else
                   	 {
                     	   FOR_3(i1,i2,i3,J1,J2,J3)
                     	   {
                                      j1=i1+is1; j2=i2+is2; j3=i3+is3; // first line in 
      	     // *wdh* 2011/08/26 -- we need to set rhs=u on parallel ghost that are also interp. pts
      	     // if( MASK(i1,i2,i3)>0 )
                       	     if( MASK(i1,i2,i3)!=0 )
                       	     {
                         	       DEFECT(i1,i2,i3)=U(i1,i2,i3);
                         	       DEFECT(j1,j2,j3)=U(j1,j2,j3);
                       	     }
                       	     else
                       	     {
                         	       DEFECT(i1,i2,i3)=0.;
                         	       DEFECT(j1,j2,j3)=0.;
                       	     }
                     	   }
                   	 }
               // --- Fixup any interpolation points that lie in the parallel ghost region ---
               //   These points are not in the list of interpolation points. BUG fixed: *wdh* 2011/08/26
               //   NOTE: the final parallel ghost line has already been set above, so we loop
               //         over one less line of parallel ghost points
                              int ia,ib;
                   	 if( side==0)
                   	 {
                     	   ia=Iv[axis].getBase()+1;
                                  ib=Iv[axis].getBase()+numParallelGhost[0]-1;
      	   // Jv[axis]= Range(ia+1,ia+numParallelGhost[0]-1);
                   	 }
                   	 else
                   	 {
                     	   ia=Iv[axis].getBound()-numParallelGhost[1]+1;
                     	   ib=Iv[axis].getBound()-1;
      	   // Jv[axis]= Range(ib-numParallelGhost[1]+1,ib-1);
                   	 }
                   	 if( ia<=ib )  // ia>ib can occur for parallel ghost on the far left and right ends of an array.
                   	 {
                     	   Jv[axis]=Range(ia,ib);
                     	   FOR_3(i1,i2,i3,J1,J2,J3)
                     	   {
                       	     if( MASK(i1,i2,i3)<0 )
                       	     {
                         	       DEFECT(i1,i2,i3)=U(i1,i2,i3);
                       	     }
                     	   }
                   	 }
                        }
                        else if( bcLocal(side,axis)==OgmgParameters::extrapolate )
                        {
      	// *********************************
      	// ******* Dirichlet BC ************
      	// *********************************
      	 // printf("Assign DirichletRHS for TRID side=%i axis=%i \n",side,axis);
       	    // J1=I1; J2=I2; J3=I3;
       	    // Jv[axis]=side==0 ? Iv[axis].getBase() : Iv[axis].getBound();
       	    // do not include ghost pts on adj sides.
       	    // ** getBoundaryIndex(mg.gridIndexRange(),side,axis,J1,J2,J3);  // this fills in values even if not needed		
                   	 J1=I1; J2=I2; J3=I3;
                   	 Jv[axis]=side==0 ? Iv[axis].getBase() : Iv[axis].getBound();
                   	 if( orderOfAccuracy==2 )
                   	 {
                     	   FOR_3(i1,i2,i3,J1,J2,J3)
                     	   {
                       	     if( MASK(i1,i2,i3)>0 )
                       	     {
                         	       DEFECT(i1,i2,i3)=F(i1,i2,i3);
                       	     }
                       	     else
                       	     {
                         	       DEFECT(i1,i2,i3)=0.;
                       	     }
                     	   }
                   	 }
                   	 else
                   	 {
                                  assert( orderOfAccuracy==4 );
      	   // if( parameters.fourthOrderBoundaryConditionOption==0 || level>0 )
                     	   if( !useEquationOnGhostForDirichlet )
                     	   {
                       	     FOR_3(i1,i2,i3,J1,J2,J3)
                       	     {
                         	       j1=i1+is1; j2=i2+is2; j3=i3+is3; // boundary point
                         	       if( MASK(j1,j2,j3)>0 )
                         	       {
                           	         DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                           	         DEFECT(i1,i2,i3)=0.;           // extrapolation or symmetry 
                         	       }
                         	       else
                         	       {
                         		 DEFECT(i1,i2,i3)=0.;
                         	       }
                       	     }
                     	   }
                     	   else
                     	   {
                   // --- Use equation to 2nd order on the first ghost line ---
                       	     if( equationToSolve!=OgesParameters::laplaceEquation )
                       	     {
                                          printf("lineSmoothBC: ERROR: equationToSolve!=laplaceEquation. This case is not yet implemented.\n");
                         	       OV_ABORT("ERROR");
                       	     }
                   // +++ NOTE: These equations must match those in bcOpt.bf +++
                                      int ms2 = direction==0 ? 1 : direction==1 ? width : width*width;  // shift for 2nd-order stencil in a fourth order operator
                                      ms2*=1-2*side;
                                      real fi=0.;  // RHS for equation-bc is zero for level>0
                       	     if( numberOfDimensions==2 )
                       	     {
                         	       if( axis==0 )
                         	       {
                         		 if( usingConstantCoefficients )
                         		 {
                           		   FOR_3(i1,i2,i3,J1,J2,J3)
                           		   {
                             		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                      if( MASK(j1,j2,j3)>0 )
                                                      {
                               		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                          if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                               		       DEFECT(i1,i2,i3)=fi-(U(j1,j2+1,j3)+U(j1,j2-1,j3))/(dx[1]*dx[1]);  // equation      
                             		     }
                                                      else
                             		     {
                               		       DEFECT(j1,j2,j3)=0.;
                                                          DEFECT(i1,i2,i3)=0.;
                             		     }
                           		   }
                         		 }
                         		 else
                         		 {
                           		   FOR_3(i1,i2,i3,J1,J2,J3)
                           		   {
                             		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                      if( MASK(j1,j2,j3)>0 )
                                                      {
                               		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                          if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                               		       DEFECT(i1,i2,i3)=fi-(  // eqn to 2nd order
                               		       COEFF(m22+ms2,i1,i2,i3)*U(j1-1,j2-1,j3)+COEFF(m32+ms2,i1,i2,i3)*U(j1,j2-1,j3)+COEFF(m42+ms2,i1,i2,i3)*U(j1+1,j2-1,j3)+
      		    // COEFF(m23+ms2,i1,i2,i3)*U(j1-1,j2  ,j3)+COEFF(m33+ms2,i1,i2,i3)*U(j1,j2  ,j3)+COEFF(m43+ms2,i1,i2,i3)*U(j1+1,j2  ,j3)+
                               		       COEFF(m24+ms2,i1,i2,i3)*U(j1-1,j2+1,j3)+COEFF(m34+ms2,i1,i2,i3)*U(j1,j2+1,j3)+COEFF(m44+ms2,i1,i2,i3)*U(j1+1,j2+1,j3) 
                               			 );
                             		     }
                                                      else
                             		     {
                               		       DEFECT(j1,j2,j3)=0.;
                                                          DEFECT(i1,i2,i3)=0.;
                             		     }
                           		   }
                         		 }
                         	       }
                         	       else if( axis==1 )
                         	       {
                         		 if( usingConstantCoefficients )
                         		 {
                           		   FOR_3(i1,i2,i3,J1,J2,J3)
                           		   {
                             		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                      if( MASK(j1,j2,j3)>0 )
                                                      {
                               		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                          if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                               		       DEFECT(i1,i2,i3)=fi-(U(j1+1,j2,j3)+U(j1-1,j2,j3))/(dx[0]*dx[0]);  // equation      
                             		     }
                                                      else
                             		     {
                               		       DEFECT(j1,j2,j3)=0.;
                                                          DEFECT(i1,i2,i3)=0.;
                             		     }
                           		   }
                         		 }
                         		 else
                         		 {
                           		   FOR_3(i1,i2,i3,J1,J2,J3)
                           		   {
                             		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                      if( MASK(j1,j2,j3)>0 )
                                                      {
                               		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                          if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                               		       DEFECT(i1,i2,i3)=fi-(  // eqn to 2nd order
                           		   COEFF(m22+ms2,i1,i2,i3)*U(j1-1,j2-1,j3)/* +COEFF(m32+ms2,i1,i2,i3)*U(j1,j2-1,j3) */+COEFF(m42+ms2,i1,i2,i3)*U(j1+1,j2-1,j3)+
                           		   COEFF(m23+ms2,i1,i2,i3)*U(j1-1,j2  ,j3)/* +COEFF(m33+ms2,i1,i2,i3)*U(j1,j2  ,j3) */+COEFF(m43+ms2,i1,i2,i3)*U(j1+1,j2  ,j3)+
                           		   COEFF(m24+ms2,i1,i2,i3)*U(j1-1,j2+1,j3)/* +COEFF(m34+ms2,i1,i2,i3)*U(j1,j2+1,j3) */+COEFF(m44+ms2,i1,i2,i3)*U(j1+1,j2+1,j3) 
                               		       );
                             		     }
                                                      else
                             		     {
                               		       DEFECT(j1,j2,j3)=0.;
                                                          DEFECT(i1,i2,i3)=0.;
                             		     }
                           		   }
                         		 }
                         	       }
                         	       else
                         	       {
                                              OV_ABORT("ERROR: invalid axis");
                         	       }
                       	     }
                       	     else if( numberOfDimensions==3 )
                       	     {
                         	       if( axis==0 )
                         	       {
                         		 if( usingConstantCoefficients )
                         		 {
                           		   FOR_3(i1,i2,i3,J1,J2,J3)
                           		   {
                             		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                      if( MASK(j1,j2,j3)>0 )
                                                      {
                               		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                          if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                               		       DEFECT(i1,i2,i3)=fi-( (U(j1,j2+1,j3)+U(j1,j2-1,j3))/(dx[1]*dx[1])
                                                				            +(U(j1,j2,j3+1)+U(j1,j2,j3-1))/(dx[2]*dx[2]) );
                             		     }
                                                      else
                             		     {
                               		       DEFECT(j1,j2,j3)=0.;
                                                          DEFECT(i1,i2,i3)=0.;
                             		     }
                           		   }
                         		 }
                         		 else
                         		 {
                           		   FOR_3(i1,i2,i3,J1,J2,J3)
                           		   {
                             		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                      if( MASK(j1,j2,j3)>0 )
                                                      {
                               		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                          if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                               		       DEFECT(i1,i2,i3)=fi-(  // eqn to 2nd order
                                      COEFF(m222+ms2,i1,i2,i3)*U(j1-1,j2-1,j3-1)+COEFF(m322+ms2,i1,i2,i3)*U(j1,j2-1,j3-1)+COEFF(m422+ms2,i1,i2,i3)*U(j1+1,j2-1,j3-1)+
                                      COEFF(m232+ms2,i1,i2,i3)*U(j1-1,j2  ,j3-1)+COEFF(m332+ms2,i1,i2,i3)*U(j1,j2  ,j3-1)+COEFF(m432+ms2,i1,i2,i3)*U(j1+1,j2  ,j3-1)+
                                      COEFF(m242+ms2,i1,i2,i3)*U(j1-1,j2+1,j3-1)+COEFF(m342+ms2,i1,i2,i3)*U(j1,j2+1,j3-1)+COEFF(m442+ms2,i1,i2,i3)*U(j1+1,j2+1,j3-1)+
                                      COEFF(m223+ms2,i1,i2,i3)*U(j1-1,j2-1,j3  )+COEFF(m323+ms2,i1,i2,i3)*U(j1,j2-1,j3  )+COEFF(m423+ms2,i1,i2,i3)*U(j1+1,j2-1,j3  )+
      	//   COEFF(m233+ms2,i1,i2,i3)*U(j1-1,j2  ,j3  )+COEFF(m333+ms2,i1,i2,i3)*U(j1,j2  ,j3  )+COEFF(m433+ms2,i1,i2,i3)*U(j1+1,j2  ,j3  )+
                                      COEFF(m243+ms2,i1,i2,i3)*U(j1-1,j2+1,j3  )+COEFF(m343+ms2,i1,i2,i3)*U(j1,j2+1,j3  )+COEFF(m443+ms2,i1,i2,i3)*U(j1+1,j2+1,j3  )+
                                      COEFF(m224+ms2,i1,i2,i3)*U(j1-1,j2-1,j3+1)+COEFF(m324+ms2,i1,i2,i3)*U(j1,j2-1,j3+1)+COEFF(m424+ms2,i1,i2,i3)*U(j1+1,j2-1,j3+1)+
                                      COEFF(m234+ms2,i1,i2,i3)*U(j1-1,j2  ,j3+1)+COEFF(m334+ms2,i1,i2,i3)*U(j1,j2  ,j3+1)+COEFF(m434+ms2,i1,i2,i3)*U(j1+1,j2  ,j3+1)+
                                      COEFF(m244+ms2,i1,i2,i3)*U(j1-1,j2+1,j3+1)+COEFF(m344+ms2,i1,i2,i3)*U(j1,j2+1,j3+1)+COEFF(m444+ms2,i1,i2,i3)*U(j1+1,j2+1,j3+1) 
                               		       );
                             		     }
                                                      else
                             		     {
                               		       DEFECT(j1,j2,j3)=0.;
                                                          DEFECT(i1,i2,i3)=0.;
                             		     }
                           		   }
                         		 }
                         	       }
                         	       else if( axis==1 )
                         	       {
                         		 if( usingConstantCoefficients )
                         		 {
                           		   FOR_3(i1,i2,i3,J1,J2,J3)
                           		   {
                             		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                      if( MASK(j1,j2,j3)>0 )
                                                      {
                               		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                          if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                               		       DEFECT(i1,i2,i3)=fi-( (U(j1+1,j2,j3)+U(j1-1,j2,j3))/(dx[0]*dx[0])
                                              					    +(U(j1,j2,j3+1)+U(j1,j2,j3-1))/(dx[2]*dx[2]) );
                             		     }
                                                      else
                             		     {
                               		       DEFECT(j1,j2,j3)=0.;
                                                          DEFECT(i1,i2,i3)=0.;
                             		     }
                           		   }
                         		 }
                         		 else
                         		 {
                           		   FOR_3(i1,i2,i3,J1,J2,J3)
                           		   {
                             		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                      if( MASK(j1,j2,j3)>0 )
                                                      {
                               		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                          if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                               		       DEFECT(i1,i2,i3)=fi-(
                                      COEFF(m222+ms2,i1,i2,i3)*U(j1-1,j2-1,j3-1)+COEFF(m322+ms2,i1,i2,i3)*U(j1,j2-1,j3-1)+COEFF(m422+ms2,i1,i2,i3)*U(j1+1,j2-1,j3-1)+
                                      COEFF(m232+ms2,i1,i2,i3)*U(j1-1,j2  ,j3-1)+COEFF(m332+ms2,i1,i2,i3)*U(j1,j2  ,j3-1)+COEFF(m432+ms2,i1,i2,i3)*U(j1+1,j2  ,j3-1)+
                                      COEFF(m242+ms2,i1,i2,i3)*U(j1-1,j2+1,j3-1)+COEFF(m342+ms2,i1,i2,i3)*U(j1,j2+1,j3-1)+COEFF(m442+ms2,i1,i2,i3)*U(j1+1,j2+1,j3-1)+
                                      COEFF(m223+ms2,i1,i2,i3)*U(j1-1,j2-1,j3  )+/*COEFF(m323+ms2,i1,i2,i3)*U(j1,j2-1,j3  )*/+COEFF(m423+ms2,i1,i2,i3)*U(j1+1,j2-1,j3  )+
                       	     COEFF(m233+ms2,i1,i2,i3)*U(j1-1,j2  ,j3  )+/*COEFF(m333+ms2,i1,i2,i3)*U(j1,j2  ,j3  )*/+COEFF(m433+ms2,i1,i2,i3)*U(j1+1,j2  ,j3  )+
                                      COEFF(m243+ms2,i1,i2,i3)*U(j1-1,j2+1,j3  )+/*COEFF(m343+ms2,i1,i2,i3)*U(j1,j2+1,j3  )*/+COEFF(m443+ms2,i1,i2,i3)*U(j1+1,j2+1,j3  )+
                                      COEFF(m224+ms2,i1,i2,i3)*U(j1-1,j2-1,j3+1)+COEFF(m324+ms2,i1,i2,i3)*U(j1,j2-1,j3+1)+COEFF(m424+ms2,i1,i2,i3)*U(j1+1,j2-1,j3+1)+
                                      COEFF(m234+ms2,i1,i2,i3)*U(j1-1,j2  ,j3+1)+COEFF(m334+ms2,i1,i2,i3)*U(j1,j2  ,j3+1)+COEFF(m434+ms2,i1,i2,i3)*U(j1+1,j2  ,j3+1)+
                                      COEFF(m244+ms2,i1,i2,i3)*U(j1-1,j2+1,j3+1)+COEFF(m344+ms2,i1,i2,i3)*U(j1,j2+1,j3+1)+COEFF(m444+ms2,i1,i2,i3)*U(j1+1,j2+1,j3+1) 
                               		       );
                               		     }
                                                      else
                             		     {
                               		       DEFECT(j1,j2,j3)=0.;
                                                          DEFECT(i1,i2,i3)=0.;
                             		     }
                           		   }
                         		 }
                         	       }
                         	       else // axis==2
                         	       {
                         		 if( usingConstantCoefficients )
                         		 {
                           		   FOR_3(i1,i2,i3,J1,J2,J3)
                           		   {
                             		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                      if( MASK(j1,j2,j3)>0 )
                                                      {
                               		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                          if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                               		       DEFECT(i1,i2,i3)=fi-( (U(j1+1,j2,j3)+U(j1-1,j2,j3))/(dx[0]*dx[0])
                                              					    +(U(j1,j2+1,j3)+U(j1,j2-1,j3))/(dx[1]*dx[1]) );
                             		     }
                                                      else
                             		     {
                               		       DEFECT(j1,j2,j3)=0.;
                                                          DEFECT(i1,i2,i3)=0.;
                             		     }
                           		   }
                         		 }
                         		 else
                         		 {
                           		   FOR_3(i1,i2,i3,J1,J2,J3)
                           		   {
                             		     j1=i1+is1; j2=i2+is2; j3=i3+is3;  // boundary point
                                                      if( MASK(j1,j2,j3)>0 )
                                                      {
                               		       DEFECT(j1,j2,j3)=F(j1,j2,j3);  // dirichlet BC
                                                          if( level==0 ) fi=F(i1,i2,i3);  // *wdh* 100112 - we must be consistent with bcOpt
                               		       DEFECT(i1,i2,i3)=fi-(
                                      COEFF(m222+ms2,i1,i2,i3)*U(j1-1,j2-1,j3-1)+COEFF(m322+ms2,i1,i2,i3)*U(j1,j2-1,j3-1)+COEFF(m422+ms2,i1,i2,i3)*U(j1+1,j2-1,j3-1)+
                                      COEFF(m232+ms2,i1,i2,i3)*U(j1-1,j2  ,j3-1)+/*COEFF(m332+ms2,i1,i2,i3)*U(j1,j2  ,j3-1)*/+COEFF(m432+ms2,i1,i2,i3)*U(j1+1,j2  ,j3-1)+
                                      COEFF(m242+ms2,i1,i2,i3)*U(j1-1,j2+1,j3-1)+COEFF(m342+ms2,i1,i2,i3)*U(j1,j2+1,j3-1)+COEFF(m442+ms2,i1,i2,i3)*U(j1+1,j2+1,j3-1)+
                                      COEFF(m223+ms2,i1,i2,i3)*U(j1-1,j2-1,j3  )+COEFF(m323+ms2,i1,i2,i3)*U(j1,j2-1,j3  )+COEFF(m423+ms2,i1,i2,i3)*U(j1+1,j2-1,j3  )+
                       	     COEFF(m233+ms2,i1,i2,i3)*U(j1-1,j2  ,j3  )+/*COEFF(m333+ms2,i1,i2,i3)*U(j1,j2  ,j3  )*/+COEFF(m433+ms2,i1,i2,i3)*U(j1+1,j2  ,j3  )+
                                      COEFF(m243+ms2,i1,i2,i3)*U(j1-1,j2+1,j3  )+COEFF(m343+ms2,i1,i2,i3)*U(j1,j2+1,j3  )+COEFF(m443+ms2,i1,i2,i3)*U(j1+1,j2+1,j3  )+
                                      COEFF(m224+ms2,i1,i2,i3)*U(j1-1,j2-1,j3+1)+COEFF(m324+ms2,i1,i2,i3)*U(j1,j2-1,j3+1)+COEFF(m424+ms2,i1,i2,i3)*U(j1+1,j2-1,j3+1)+
                                      COEFF(m234+ms2,i1,i2,i3)*U(j1-1,j2  ,j3+1)+/*COEFF(m334+ms2,i1,i2,i3)*U(j1,j2  ,j3+1)*/+COEFF(m434+ms2,i1,i2,i3)*U(j1+1,j2  ,j3+1)+
                                      COEFF(m244+ms2,i1,i2,i3)*U(j1-1,j2+1,j3+1)+COEFF(m344+ms2,i1,i2,i3)*U(j1,j2+1,j3+1)+COEFF(m444+ms2,i1,i2,i3)*U(j1+1,j2+1,j3+1) 
                               		       );
                               		     }
                                                      else
                             		     {
                               		       DEFECT(j1,j2,j3)=0.;
                                                          DEFECT(i1,i2,i3)=0.;
                             		     }
                           		   }
                         		 }
                         	       }
                       	     }
                       	     else
                       	     {
                         	       Overture::abort();
                       	     }
                     	   }
                   	 }
                        }
                        else if( bcLocal(side,axis)==OgmgParameters::equation )
                        {
      	 // *****************************************************
      	 // ************ neumann  or mixed **********************
      	 // *****************************************************
                   	 J1=I1; J2=I2; J3=I3;
                   	 Jv[axis]=side==0 ? Iv[axis].getBase() : Iv[axis].getBound();
               // *********** FIX ME for non-orthogonal curvilinear *********************8
                   	 if( orderOfAccuracy==2 )
                   	 {
      	   // *** this case is now handled below in lineSmoothRHS
                     	   if( true )
                     	   {
                     	   }
                     	   else if( level==0 )
                     	   {
                       	     FOR_3(i1,i2,i3,J1,J2,J3)
                       	     {
                         	       j1=i1+is1; j2=i2+is2; j3=i3+is3;
                         	       if( MASK(j1,j2,j3)>0 )
                         	       {
                         		 DEFECT(i1,i2,i3)=F(i1,i2,i3);   // neumann BC
                         	       }
                         	       else
                         	       {
                         		 DEFECT(i1,i2,i3)=0.;
                         	       }
                       	     }
                     	   }
                     	   else
                     	   {
                       	     FOR_3(i1,i2,i3,J1,J2,J3)
                       	     {
                         	       DEFECT(i1,i2,i3)=0.;           // neumann BC
                       	     }
                     	   }
                   	 }
                   	 else // fourth-order
                   	 {
                                  assert( orderOfAccuracy==4 );
                 // **** fix this ****
                                  if( useEquationOnGhostForNeumann )
                     	   {
                   // *** these values are now filled in below 
                                      if( true )
                       	     {
                       	     }
                       	     else if( level==0 || level>0 )
                       	     {
                         	       if( usingConstantCoefficients )
                         	       {
      		 // Neumann and  u.xxx = f.x - g.yy
                       // i1,i2,i3 = 2nd ghost
                       // j1,j2,j3 = 1st ghost
                         		 FOR_3(i1,i2,i3,J1,J2,J3)
                         		 {
                           		   j1=i1+is1; j2=i2+is2; j3=i3+is3;
                           		   fPrintF(debugFile,"N4Lx: i=(%i,%i,%i) j=(%i,%i,%i) F=%8.2e\n",i1,i2,i3,j1,j2,j3,F(j1,j2,j3));
                           		   if( MASK(j1+is1,j2+is2,j3+is3)>0 ) // check the mask on the boundary
                           		   {
                             		     DEFECT(j1,j2,j3)=F(j1,j2,j3);   // neumann BC
                             		     DEFECT(i1,i2,i3)=0.;            // u.xxx=
                           		   }
                           		   else
                           		   {
                             		     DEFECT(j1,j2,j3)=0.;
                             		     DEFECT(i1,i2,i3)=0.;
                           		   }
                         		 }
                         	       }
                         	       else
                         	       {
                            	          Overture::abort("lineSolve: Error useEquationOnGhostForNeumann not implemented.");
                         	       }
                       	     }
                     	   }
                                  else
                                  {
      	     // extrapolation for 2nd ghost or symmetry
                       	     if( level==0 )
                       	     {
                                          assert( bcOptionN==0 );
                         	       if( usingConstantCoefficients )
                         	       {
      		 // Neumann + extrapolation for rectangular grids.
                         		 FOR_3(i1,i2,i3,J1,J2,J3)
                         		 {
                           		   j1=i1+is1; j2=i2+is2; j3=i3+is3;
      		   // fPrintF(debugFile,"N4: i=(%i,%i,%i) j=(%i,%i,%i) F=%8.2e\n",i1,i2,i3,j1,j2,j3,F(j1,j2,j3));
                           		   if( MASK(j1+is1,j2+is2,j3+is3)>0 ) // check the mask on the boundary
                           		   {
                             		     DEFECT(j1,j2,j3)=F(j1,j2,j3);   // neumann BC
      		     // *wdh* 110309 DEFECT(i1,i2,i3)=0.;            // extrapolation: second ghost line                    
                          // extrapolation: second ghost line
                                                    if( orderOfExtrapN <= 4 )
                                                      DEFECT(i1,i2,i3)=0.;            
                                                    else if( orderOfExtrapN == 5 )
                           // For 5th order extrapolation we need to set the RHS since the formula does not fit in the penta matrix
                           // 1 -5 10 -10 5 -1 
                                                      DEFECT(i1,i2,i3)=U(i1+5*is1,i2+5*is2,i3+5*is3);
                                                    else
                                                      OV_ABORT("ERROR:lineSmooth: orderOfExtrapN invalid");
                           		   }
                           		   else
                           		   {
                             		     DEFECT(j1,j2,j3)=0.;
                             		     DEFECT(i1,i2,i3)=0.;
                           		   }
                         		 }
                         	       }
                         	       else
                         	       {
                         		 if( numberOfDimensions==2 && direction==0 )
                         		 {
                     // 		   neumannExtrapCurvilinear(R,2,FOR_3,Iv,I1,I2,I3);
                                          FOR_3(i1,i2,i3,J1,J2,J3)
                                          {
                                              j1=i1+is1; j2=i2+is2; j3=i3+is3; // first ghost line 
                     //    fPrintF(debugFile,"N4: i=(%i,%i,%i) j=(%i,%i,%i) F=%8.2e C=[%6.2f,%6.2f,%6.2f,%6.2f,%6.2f], "
                     //           " [%6.2f,%6.2f,%6.2f,%6.2f]\n",
                     //             i1,i2,i3,j1,j2,j3,
                     //              F(j1,j2,j3),COEFF(m13,j1,j2,j3),COEFF(m23,j1,j2,j3),COEFF(m33,j1,j2,j3),
                     //               COEFF(m43,j1,j2,j3),COEFF(m53,j1,j2,j3),
                     //               COEFF(m31,j1,j2,j3),COEFF(m32,j1,j2,j3),COEFF(m34,j1,j2,j3),COEFF(m35,j1,j2,j3));
                                              if( MASK(j1+is1,j2+is2,j3+is3)>0 ) // check the mask on the boundary
                                              {
                       //                        #If "2" == "2" && "R" == "R"
                         // neumann BC: nd=2 dir=0
                                                  DEFECT(j1,j2,j3)=F(j1,j2,j3)- (
                                                        COEFF(m31,j1,j2,j3)*U(j1+is1,j2-2,j3)
                                                      +COEFF(m32,j1,j2,j3)*U(j1+is1,j2-1,j3)
                                                      +COEFF(m34,j1,j2,j3)*U(j1+is1,j2+1,j3)
                                                      +COEFF(m35,j1,j2,j3)*U(j1+is1,j2+2,j3) );
                         // extrapolation: second ghost line
                                                  if( orderOfExtrapN <= 4 )
                                                      DEFECT(i1,i2,i3)=0.;            
                                                  else if( orderOfExtrapN == 5 )
                           // For 5th order extrapolation we need to set the RHS since the formula does not fit in the penta matrix
                           // 1 -5 10 -10 5 -1 
                                                      DEFECT(i1,i2,i3)=U(i1+5*is1,i2+5*is2,i3+5*is3);
                                                  else
                                                      OV_ABORT("ERROR:lineSmooth: orderOfExtrapN invalid");
                                              }
                                              else
                                              {
                                                  DEFECT(j1,j2,j3)=0.;
                                                  DEFECT(i1,i2,i3)=0.;
                                              }
                                          }
                         		 }
                         		 else if(  numberOfDimensions==2 && direction==1 )
                         		 {
                     // 		   neumannExtrapCurvilinear(S,2,FOR_3,Iv,I1,I2,I3);
                                          FOR_3(i1,i2,i3,J1,J2,J3)
                                          {
                                              j1=i1+is1; j2=i2+is2; j3=i3+is3; // first ghost line 
                     //    fPrintF(debugFile,"N4: i=(%i,%i,%i) j=(%i,%i,%i) F=%8.2e C=[%6.2f,%6.2f,%6.2f,%6.2f,%6.2f], "
                     //           " [%6.2f,%6.2f,%6.2f,%6.2f]\n",
                     //             i1,i2,i3,j1,j2,j3,
                     //              F(j1,j2,j3),COEFF(m13,j1,j2,j3),COEFF(m23,j1,j2,j3),COEFF(m33,j1,j2,j3),
                     //               COEFF(m43,j1,j2,j3),COEFF(m53,j1,j2,j3),
                     //               COEFF(m31,j1,j2,j3),COEFF(m32,j1,j2,j3),COEFF(m34,j1,j2,j3),COEFF(m35,j1,j2,j3));
                                              if( MASK(j1+is1,j2+is2,j3+is3)>0 ) // check the mask on the boundary
                                              {
                       //                        #If "2" == "2" && "S" == "R"
                       //                        #Elif "2" == "2" && "S" == "S"
                                                  DEFECT(j1,j2,j3)=F(j1,j2,j3)- (
                                                        COEFF(m13,j1,j2,j3)*U(j1-2,j2+is2,j3)
                                                      +COEFF(m23,j1,j2,j3)*U(j1-1,j2+is2,j3)
                                                      +COEFF(m43,j1,j2,j3)*U(j1+1,j2+is2,j3)
                                                      +COEFF(m53,j1,j2,j3)*U(j1+2,j2+is2,j3) );
                         // extrapolation: second ghost line
                                                  if( orderOfExtrapN <= 4 )
                                                      DEFECT(i1,i2,i3)=0.;            
                                                  else if( orderOfExtrapN == 5 )
                           // For 5th order extrapolation we need to set the RHS since the formula does not fit in the penta matrix
                           // 1 -5 10 -10 5 -1 
                                                      DEFECT(i1,i2,i3)=U(i1+5*is1,i2+5*is2,i3+5*is3);
                                                  else
                                                      OV_ABORT("ERROR:lineSmooth: orderOfExtrapN invalid");
                                              }
                                              else
                                              {
                                                  DEFECT(j1,j2,j3)=0.;
                                                  DEFECT(i1,i2,i3)=0.;
                                              }
                                          }
                         		 }
                         		 else if(  numberOfDimensions==3 && direction==0 )
                         		 {
                     // 		   neumannExtrapCurvilinear(R,3,FOR_3,Iv,I1,I2,I3);
                                          FOR_3(i1,i2,i3,J1,J2,J3)
                                          {
                                              j1=i1+is1; j2=i2+is2; j3=i3+is3; // first ghost line 
                     //    fPrintF(debugFile,"N4: i=(%i,%i,%i) j=(%i,%i,%i) F=%8.2e C=[%6.2f,%6.2f,%6.2f,%6.2f,%6.2f], "
                     //           " [%6.2f,%6.2f,%6.2f,%6.2f]\n",
                     //             i1,i2,i3,j1,j2,j3,
                     //              F(j1,j2,j3),COEFF(m13,j1,j2,j3),COEFF(m23,j1,j2,j3),COEFF(m33,j1,j2,j3),
                     //               COEFF(m43,j1,j2,j3),COEFF(m53,j1,j2,j3),
                     //               COEFF(m31,j1,j2,j3),COEFF(m32,j1,j2,j3),COEFF(m34,j1,j2,j3),COEFF(m35,j1,j2,j3));
                                              if( MASK(j1+is1,j2+is2,j3+is3)>0 ) // check the mask on the boundary
                                              {
                       //                        #If "3" == "2" && "R" == "R"
                       //                        #Elif "3" == "2" && "R" == "S"
                       //                        #Elif "3" == "3" && "R" == "R"
                         // neumann BC: nd=3 dir=0
                                                  DEFECT(j1,j2,j3)=F(j1,j2,j3)- (
                                                        COEFF(m313,j1,j2,j3)*U(j1+is1,j2-2,j3)
                                                      +COEFF(m323,j1,j2,j3)*U(j1+is1,j2-1,j3)
                                                      +COEFF(m343,j1,j2,j3)*U(j1+is1,j2+1,j3)
                                                      +COEFF(m353,j1,j2,j3)*U(j1+is1,j2+2,j3)
                                                      +COEFF(m331,j1,j2,j3)*U(j1+is1,j2  ,j3-2)
                                                      +COEFF(m332,j1,j2,j3)*U(j1+is1,j2  ,j3-1)
                                                      +COEFF(m334,j1,j2,j3)*U(j1+is1,j2  ,j3+1)
                                                      +COEFF(m335,j1,j2,j3)*U(j1+is1,j2  ,j3+2) );
                         // extrapolation: second ghost line
                                                  if( orderOfExtrapN <= 4 )
                                                      DEFECT(i1,i2,i3)=0.;            
                                                  else if( orderOfExtrapN == 5 )
                           // For 5th order extrapolation we need to set the RHS since the formula does not fit in the penta matrix
                           // 1 -5 10 -10 5 -1 
                                                      DEFECT(i1,i2,i3)=U(i1+5*is1,i2+5*is2,i3+5*is3);
                                                  else
                                                      OV_ABORT("ERROR:lineSmooth: orderOfExtrapN invalid");
                                              }
                                              else
                                              {
                                                  DEFECT(j1,j2,j3)=0.;
                                                  DEFECT(i1,i2,i3)=0.;
                                              }
                                          }
                         		 }
                         		 else if(  numberOfDimensions==3 && direction==1 )
                         		 {
                     // 		   neumannExtrapCurvilinear(S,3,FOR_3,Iv,I1,I2,I3);
                                          FOR_3(i1,i2,i3,J1,J2,J3)
                                          {
                                              j1=i1+is1; j2=i2+is2; j3=i3+is3; // first ghost line 
                     //    fPrintF(debugFile,"N4: i=(%i,%i,%i) j=(%i,%i,%i) F=%8.2e C=[%6.2f,%6.2f,%6.2f,%6.2f,%6.2f], "
                     //           " [%6.2f,%6.2f,%6.2f,%6.2f]\n",
                     //             i1,i2,i3,j1,j2,j3,
                     //              F(j1,j2,j3),COEFF(m13,j1,j2,j3),COEFF(m23,j1,j2,j3),COEFF(m33,j1,j2,j3),
                     //               COEFF(m43,j1,j2,j3),COEFF(m53,j1,j2,j3),
                     //               COEFF(m31,j1,j2,j3),COEFF(m32,j1,j2,j3),COEFF(m34,j1,j2,j3),COEFF(m35,j1,j2,j3));
                                              if( MASK(j1+is1,j2+is2,j3+is3)>0 ) // check the mask on the boundary
                                              {
                       //                        #If "3" == "2" && "S" == "R"
                       //                        #Elif "3" == "2" && "S" == "S"
                       //                        #Elif "3" == "3" && "S" == "R"
                       //                        #Elif "3" == "3" && "S" == "S"
                                                  DEFECT(j1,j2,j3)=F(j1,j2,j3)- (
                                                        COEFF(m133,j1,j2,j3)*U(j1-2,j2+is2,j3)
                                                      +COEFF(m233,j1,j2,j3)*U(j1-1,j2+is2,j3)
                                                      +COEFF(m433,j1,j2,j3)*U(j1+1,j2+is2,j3)
                                                      +COEFF(m533,j1,j2,j3)*U(j1+2,j2+is2,j3)
                                                      +COEFF(m331,j1,j2,j3)*U(j1  ,j2+is2,j3-2)
                                                      +COEFF(m332,j1,j2,j3)*U(j1  ,j2+is2,j3-1)
                                                      +COEFF(m334,j1,j2,j3)*U(j1  ,j2+is2,j3+1)
                                                      +COEFF(m335,j1,j2,j3)*U(j1  ,j2+is2,j3+2) );
                         // extrapolation: second ghost line
                                                  if( orderOfExtrapN <= 4 )
                                                      DEFECT(i1,i2,i3)=0.;            
                                                  else if( orderOfExtrapN == 5 )
                           // For 5th order extrapolation we need to set the RHS since the formula does not fit in the penta matrix
                           // 1 -5 10 -10 5 -1 
                                                      DEFECT(i1,i2,i3)=U(i1+5*is1,i2+5*is2,i3+5*is3);
                                                  else
                                                      OV_ABORT("ERROR:lineSmooth: orderOfExtrapN invalid");
                                              }
                                              else
                                              {
                                                  DEFECT(j1,j2,j3)=0.;
                                                  DEFECT(i1,i2,i3)=0.;
                                              }
                                          }
                         		 }
                         		 else if(  numberOfDimensions==3 && direction==2 )
                         		 {
                     // 		   neumannExtrapCurvilinear(T,3,FOR_3,Iv,I1,I2,I3);
                                          FOR_3(i1,i2,i3,J1,J2,J3)
                                          {
                                              j1=i1+is1; j2=i2+is2; j3=i3+is3; // first ghost line 
                     //    fPrintF(debugFile,"N4: i=(%i,%i,%i) j=(%i,%i,%i) F=%8.2e C=[%6.2f,%6.2f,%6.2f,%6.2f,%6.2f], "
                     //           " [%6.2f,%6.2f,%6.2f,%6.2f]\n",
                     //             i1,i2,i3,j1,j2,j3,
                     //              F(j1,j2,j3),COEFF(m13,j1,j2,j3),COEFF(m23,j1,j2,j3),COEFF(m33,j1,j2,j3),
                     //               COEFF(m43,j1,j2,j3),COEFF(m53,j1,j2,j3),
                     //               COEFF(m31,j1,j2,j3),COEFF(m32,j1,j2,j3),COEFF(m34,j1,j2,j3),COEFF(m35,j1,j2,j3));
                                              if( MASK(j1+is1,j2+is2,j3+is3)>0 ) // check the mask on the boundary
                                              {
                       //                        #If "3" == "2" && "T" == "R"
                       //                        #Elif "3" == "2" && "T" == "S"
                       //                        #Elif "3" == "3" && "T" == "R"
                       //                        #Elif "3" == "3" && "T" == "S"
                       //                        #Elif "3" == "3" && "T" == "T"
                                                  DEFECT(j1,j2,j3)=F(j1,j2,j3)- (
                                                        COEFF(m133,j1,j2,j3)*U(j1-2,j2  ,j3+is3)
                                                      +COEFF(m233,j1,j2,j3)*U(j1-1,j2  ,j3+is3)
                                                      +COEFF(m433,j1,j2,j3)*U(j1+1,j2  ,j3+is3)
                                                      +COEFF(m533,j1,j2,j3)*U(j1+2,j2  ,j3+is3)
                                                      +COEFF(m313,j1,j2,j3)*U(j1  ,j2-2,j3+is3)
                                                      +COEFF(m323,j1,j2,j3)*U(j1  ,j2-1,j3+is3)
                                                      +COEFF(m343,j1,j2,j3)*U(j1  ,j2+1,j3+is3)
                                                      +COEFF(m353,j1,j2,j3)*U(j1  ,j2+2,j3+is3) );
                         // extrapolation: second ghost line
                                                  if( orderOfExtrapN <= 4 )
                                                      DEFECT(i1,i2,i3)=0.;            
                                                  else if( orderOfExtrapN == 5 )
                           // For 5th order extrapolation we need to set the RHS since the formula does not fit in the penta matrix
                           // 1 -5 10 -10 5 -1 
                                                      DEFECT(i1,i2,i3)=U(i1+5*is1,i2+5*is2,i3+5*is3);
                                                  else
                                                      OV_ABORT("ERROR:lineSmooth: orderOfExtrapN invalid");
                                              }
                                              else
                                              {
                                                  DEFECT(j1,j2,j3)=0.;
                                                  DEFECT(i1,i2,i3)=0.;
                                              }
                                          }
                         		 }
      //  	       FOR_3(i1,i2,i3,J1,J2,J3)
      //  	       {
      //  		 j1=i1+is1; j2=i2+is2; j3=i3+is3;
      //  		 fPrintF(debugFile,"N4: i=(%i,%i,%i) j=(%i,%i,%i) F=%8.2e\n",i1,i2,i3,j1,j2,j3,F(j1,j2,j3));
      //                   // neumann BC: nd=2 dir=0
      //  		 DEFECT(j1,j2,j3)=F(j1,j2,j3)- (
      //                                 COEFF(m31,i1,i2,i3)*U(j1+is1,j2-2,j3)
      //                                +COEFF(m32,i1,i2,i3)*U(j1+is1,j2-1,j3)
      //                                +COEFF(m34,i1,i2,i3)*U(j1+is1,j2+1,j3)
      //  			      +COEFF(m35,i1,i2,i3)*U(j1+is1,j2+1,j3) );
      //  		 DEFECT(i1,i2,i3)=0.;            // extrapolation: second ghost line                    
      //  	       }
                         	       }
                       	     }
                       	     else // level>0 
                       	     {
                     // assert( parameters.useSymmetryForNeumannOnLowerLevels );
                                          assert( orderOfExtrapN <= 4 );  // assume this for now
                         	       FOR_3(i1,i2,i3,J1,J2,J3)
                         	       {
                         		 j1=i1+is1; j2=i2+is2; j3=i3+is3;
                         		 DEFECT(j1,j2,j3)=0.;           // neumann BC or symmetry or mixed BC
                         		 DEFECT(i1,i2,i3)=0.;           // second ghost line : extrap or symmetry or mixed BC
                         	       }
                       	     }
                     	   }
                   	 }
                        }
                        else
                        {
                  	printf(" ERROR: unknown bc = %i\n",bcLocal(side,axis));
                            Overture::abort("ERROR");
                        }
                    }
                }
            }
            if( true || (useEquationOnGhostForNeumann &&  orderOfAccuracy==4) )
            {
        // new optimised BC's 
                ipar[ 4]=I1.getBase();
                ipar[ 5]=I1.getBound();
                ipar[ 6]=I1.getStride();
                ipar[ 7]=I2.getBase();
                ipar[ 8]=I2.getBound();
                ipar[ 9]=I2.getStride();
                ipar[10]=I3.getBase();
                ipar[11]=I3.getBound();
                ipar[12]=I3.getStride();
                const real *pcc = constantCoefficients.getBound(2)>=level ? &constantCoefficients(0,grid,level) : rpar;
                const real *pbcd = boundaryConditionData.getLength(3)>=grid ? &boundaryConditionData(0,0,0,grid) : rpar;
        // NOTE: do NOT use view pointers here
                lineSmoothRHS( numberOfDimensions,
                                              uLocal.getBase(0),uLocal.getBound(0),
                                              uLocal.getBase(1),uLocal.getBound(1),
                                              uLocal.getBase(2),uLocal.getBound(2),
                         		 coeffLocal.getLength(0), *coeffLocal.getDataPointer(),
                         		 *pcc,
                         		 *defectLocal.getDataPointer(), 
                         		 *sptr, *uptr, *fLocal.getDataPointer(), *maskLocal.getDataPointer(), *prsxy, ipar[0], rpar[0], 
                         		 boundaryConditionData.getLength(0),*pbcd );
            }
      // Set values at interpolation points  *wdh* 100716 -- do this last!
            if( mg.numberOfDimensions()==2 )
            {
                i3=I3.getBase();
                for( int i=0; i<numberOfInterpolationPoints; i++ )
                {
                    i1=IP(i,0);
                    i2=IP(i,1);
                    DEFECT(i1,i2,i3)=U(i1,i2,i3); 
                }
            }
            else
            {
                for( int i=0; i<numberOfInterpolationPoints; i++ )
                {
                    i1=IP(i,0);
                    i2=IP(i,1);
                    i3=IP(i,2);
                    DEFECT(i1,i2,i3)=U(i1,i2,i3); 
                }
            }
            }
            
            tm[timeForRelaxInSmooth]+=getCPU()-time0;

//        if( false && level==0 )
//        { // define the defect for Neumann or mixed BC's on level 0 at ghost points
//  	//     -- these may be inhomogeneous
//  	// **** this assumes BC is normal direction only ******
//  	int iv[3]; 
//  	iv[0]=I1.getBase(); iv[1]=I2.getBase(); iv[2]=I3.getBase();
//  	if( iv[direction]==mg.gridIndexRange()(Start,direction)-1 && mg.boundaryCondition(Start,direction)>0 )
//  	  DEFECT(iv[0],iv[1],iv[2])=f(iv[0],iv[1],iv[2]);
//  	iv[0]=I1.getBound(); iv[1]=I2.getBound(); iv[2]=I3.getBound();
//  	if( iv[direction]==mg.gridIndexRange()(End,direction)+1 && mg.boundaryCondition(End,direction)>0 )
//  	  DEFECT(iv[0],iv[1],iv[2])=f(iv[0],iv[1],iv[2]);
//        }	

            if( Ogmg::debug & 8 )
      	display(defectLocal(I1,I2,I3),"Here is the rhs before the (jacobi) tridiagonalSolver ",pDebugFile,"%8.2e ");

            time1=getCPU();

            if( ok )
      	tridiagonalSolver[level][grid][direction]->solve(defectLocal,I1,I2,I3);

            time0=getCPU();
            tm[timeForTridiagonalSolverInSmooth]+=time0-time1;

            if( ok && Ogmg::debug & 8 )
      	display(defectLocal(I1,I2,I3),"Here is the solution from the (jacobi) tridiagonalSolver ",pDebugFile,"%8.2e ");
            
            real & omega=parameters.omegaLineJacobi;
            if( omega<0. )
            {
	// assign default values
                omega=.8;
            }
            real omegam=1.-omega;
            if( ok )
            {
      	FOR_3(i1,i2,i3,I1,I2,I3)
      	{
        	  if( MASK(i1,i2,i3) >=0 )
        	  {           
          	    U(i1,i2,i3)=omegam*U(i1,i2,i3)+omega*DEFECT(i1,i2,i3);
        	  }
      	}
            }
            
            tm[timeForRelaxInSmooth]+=getCPU()-time0;

            if( Ogmg::debug & 8 )
      	display(u,sPrintF(buff,"Here is the solution u=(1-omega)*u+omega*defect omega=%5.2f",omega),debugFile,"%8.2e ");

	  // * applyBoundaryConditions( level,uu,ff );
            
            if( Ogmg::debug & 16 )
            {
      	defect(level,grid);
      	display(defectMG.multigridLevel[level][grid],sPrintF(buff,"smoothLine, direction=%i defect:",direction),debugFile);
      	display(u,sPrintF(buff,"smoothLine, direction=%i: Here is u",direction),debugFile);
            }
            applyBoundaryConditions( level,grid,uu,ff );

            if( Ogmg::debug & 8 )
      	display(u,"Here is the solution u after applyBoundaryConditions ",debugFile,"%8.2e ");
        }


    // back substitute 2n-2 multiplies, n divisions
    //    ***** fourth-order : 5 mults+divides

    // real wu = mg.numberOfDimensions()==2 ? 3./6. : 3./8.;
        real wu = (orderOfAccuracy==2 ? 3. : 5. )/jacobiWork;
    // workUnits(level)+=(1.+wu)/mgcg.multigridLevel[level].numberOfComponentGrids();  
        workUnits(level)+=(1.+wu)*mask.elementCount()/real(numberOfGridPoints);


        bool postSmooth=false; // numberOfLinesSolves%2 == 0; // true;
        if( postSmooth &&  parameters.numberOfBoundaryLayersToSmooth>0 )
        {
            
      // int numberOfLayers=parameters.numberOfBoundaryLayersToSmooth + (numberOfLinesSolves%3)-1;
            int numberOfLayers=parameters.numberOfBoundaryLayersToSmooth; 
      // int numberOfLayers=parameters.numberOfBoundaryLayersToSmooth;

            int bc[6]={0,0,0,0,0,0}; //
            bc[0+2*direction]=1; // smooth boundaries axis==direction
            bc[1+2*direction]=1;
            if( parameters.smootherType(grid,level)==alternatingLineJacobi ||
                    parameters.smootherType(grid,level)==alternatingLineZebra )
            {
        // for alternating smoothers we smooth all boundaries
                bc[0]=bc[1]=bc[2]=bc[3]=bc[4]=bc[5]=1;
            }
            

            smoothBoundary(level,grid,bc,numberOfLayers,parameters.numberOfBoundarySmoothIterations );
        }

        numberOfLinesSolves++;
        
    } // end for iteration
    
    Ogmg::debug=debugSave; // reset 

}

static const int maximumNumberOfLevelsForDirectionToSmooth=20;
static int directionToSmooth[maximumNumberOfLevelsForDirectionToSmooth]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


//\begin{>>OgmgInclude.tex}{\subsection{alternatingLineSmooth}}
void Ogmg:: 
alternatingLineSmooth(const int & level, const int & grid, bool useZebra /* =true */)
//-------------------------------------------------------------------------
// /Description:
//    Here we do alternating line smooths; one line smooth in each direction.
//\end{OgmgInclude.tex} 
//------------------------------------------------------------------------
{
    CompositeGrid & mgcg = multigridCompositeGrid();
    int axis;
    if( false && parameters.useSplitStepLineSolver )
    {
    // alternating-alternating  ** what should this be in 3D ??
        if( numberOfCycles % 2 == 0)
        {
            smoothLine(level,grid,axis1,useZebra);
            smoothLine(level,grid,axis2,useZebra);
            if( mgcg.numberOfDimensions() ==3 )
              smoothLine(level,grid,axis3,useZebra);
        }
        else 
        {
            if( mgcg.numberOfDimensions() ==3 )
              smoothLine(level,grid,axis3,useZebra);
            smoothLine(level,grid,axis2,useZebra);
            smoothLine(level,grid,axis1,useZebra);
        }
        
        return;
    }
    
    if( parameters.useSplitStepLineSolver )
    {
    // only do one direction per smooth, alternating through the directions
        const int nd=mgcg.numberOfDimensions();

        if( true )
        {
      // change line direction for each CYCLE
            int axisToSmooth = numberOfCycles % nd;
            smoothLine(level,grid,axisToSmooth,useZebra);
        }
        else
        {
      // change line direction after each SMOOTH

            assert( level<maximumNumberOfLevelsForDirectionToSmooth );
            int axisToSmooth = (directionToSmooth[level]/1) % nd;
            directionToSmooth[level]++;
        
            smoothLine(level,grid,axisToSmooth,useZebra);
        }
        
//    axisToSmooth = (directionToSmooth[level]/1) % nd;
//    directionToSmooth[level]++;
        
//    smoothLine(level,grid,axisToSmooth,useZebra);



//      if( (numberOfCycles % nd) == 0)
//       smoothLine(level,grid,axis1,useZebra);
//      else if( (numberOfCycles % nd) == 1)
//       smoothLine(level,grid,axis2,useZebra);
//      else
//       smoothLine(level,grid,axis3,useZebra);

        return;
    }
    

  // *** TRUE line solver ***
    for( axis=0; axis<mgcg.numberOfDimensions(); axis++ )
    {
        smoothLine(level,grid,axis,useZebra);

        if( false )
            smoothRedBlack(level,grid);
    }
    
    if( false )
    {
    // extra smooth near boundaries
        for( int it=0; it<1; it++ )
            for( axis=0; axis<mgcg.numberOfDimensions(); axis++ )
      	for( int side=0; side<=1; side++ )
        	  smoothLine(level,grid,axis,false,side);
    }
    
}

#undef C
#undef M123
