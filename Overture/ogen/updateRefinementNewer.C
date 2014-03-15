// This file automatically generated from updateRefinementNewer.bC with bpp.
#include "Overture.h"
#include "Ogen.h"
#include "display.h"
#include "HDF_DataBase.h"
#include "ParallelUtility.h"
#include "UpdateRefinementData.h"

// The next file contains:
//    macro setMaskAtAlignedHoles(DIM,RATIO)
// #Include updateRefinementMacros.h
// =============================================================================
//
// Mark hole points on the refinement grid that lie between two holes on the
// base grid
//
// RATIO: 2,4,general -- refinement ratio
// DIM: 2,3 -- number of dimensions
// =============================================================================


int 
outputRefinementInfoNew( GridCollection & gc, 
                   			 const aString & gridFileName, 
                   			 const aString & fileName );

#define FOR_3D(i1,i2,i3,I1,I2,I3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3DS(i1,i2,i3,I1,I2,I3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); int I1Stride=I1.getStride(),  I2Stride=I2.getStride(), I3Stride=I3.getStride(); for(i3=I3Base; i3<=I3Bound; i3+=I3Stride ) for(i2=I2Base; i2<=I2Bound; i2+=I2Stride ) for(i1=I1Base; i1<=I1Bound; i1+=I1Stride )

#define FOR_3S(i1,i2,i3,I1,I2,I3) I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); I1Stride=I1.getStride(),  I2Stride=I2.getStride(), I3Stride=I3.getStride(); for(i3=I3Base; i3<=I3Bound; i3+=I3Stride ) for(i2=I2Base; i2<=I2Bound; i2+=I2Stride ) for(i1=I1Base; i1<=I1Bound; i1+=I1Stride )

#define FOR_3R()  I1rBase  =I1r.getBase(),  I2rBase  =I2r.getBase(),  I3rBase  =I3r.getBase();   I1rBound =I1r.getBound(), I2rBound =I2r.getBound(), I3rBound =I3r.getBound();  I1rStride=I1r.getStride(),I2rStride=I2r.getStride(),I3rStride=I3r.getStride();  for(i3r=I3rBase; i3r<=I3rBound; i3r+=I3rStride)   for(i2r=I2rBase; i2r<=I2rBound; i2r+=I2rStride)   for(i1r=I1rBase; i1r<=I1rBound; i1r+=I1rStride)

#define FOR_3BR()  I1bBase =I1b.getBase(),   I2bBase =I2b.getBase(),  I3bBase =I3b.getBase();   I1bBound=I1b.getBound(),  I2bBound=I2b.getBound(), I3bBound=I3b.getBound();  I1rBase  =I1r.getBase(),  I2rBase  =I2r.getBase(),  I3rBase  =I3r.getBase();   I1rBound =I1r.getBound(), I2rBound =I2r.getBound(), I3rBound =I3r.getBound();  I1rStride=I1r.getStride(),I2rStride=I2r.getStride(),I3rStride=I3r.getStride();  for(i3b=I3bBase,i3r=I3rBase; i3b<=I3bBound; i3b++,i3r+=I3rStride)   for(i2b=I2bBase,i2r=I2rBase; i2b<=I2bBound; i2b++,i2r+=I2rStride)   for(i1b=I1bBase,i1r=I1rBase; i1b<=I1bBound; i1b++,i1r+=I1rStride)

#define FOR_3IJD(i1,i2,i3,I1,I2,I3,j1,j2,j3,J1,J2,J3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); int J1Base =J1.getBase(),   J2Base =J2.getBase(),  J3Base =J3.getBase();  for(i3=I3Base,j3=J3Base; i3<=I3Bound; i3++,j3++) for(i2=I2Base,j2=J2Base; i2<=I2Bound; i2++,j2++) for(i1=I1Base,j1=J1Base; i1<=I1Bound; i1++,j1++)


// =============================================================================================================
// /Description:
//    Compute a new gridIndexRange, dimension
//             and boundaryCondition array that will be valid for the local grid on a processor.
// 
//    Set the gid to match the ends of the local array.
//    Set the bc(side,axis) to -1 (periodic) for internal boundaries between processors
//
// NOTES: In parallel we cannot assume the rsxy array is defined on all ghost points -- it will not
// be set on the extra ghost points put at the far ends of the array. -- i.e. internal boundary ghost 
// points will be set but not external
// =============================================================================================================
static void
getLocalBoundsAndBoundaryConditions( const intMappedGridFunction & a, 
                                                                          IntegerArray & gidLocal, 
                                                                          IntegerArray & dimensionLocal, 
                                                                          IntegerArray & bcLocal )
{

    MappedGrid & mg = *a.getMappedGrid();
    
    const IntegerArray & dimension = mg.dimension();
    const IntegerArray & gid = mg.gridIndexRange();
    const IntegerArray & bc = mg.boundaryCondition();
    
    gidLocal = gid;
    bcLocal = bc;
    dimensionLocal=dimension;
    
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
//      printf(" axis=%i gidLocal(0,axis)=%i a.getLocalBase(axis)=%i  dimension(0,axis)=%i\n",axis,gidLocal(0,axis),
//                        a.getLocalBase(axis),dimension(0,axis));
//      printf(" axis=%i gidLocal(1,axis)=%i a.getLocalBound(axis)=%i dimension(0,axis)=%i\n",axis,gidLocal(1,axis),
//                        a.getLocalBound(axis),dimension(1,axis));
        if( a.getLocalBase(axis) == a.getBase(axis) ) 
        {
            assert( dimension(0,axis)==a.getLocalBase(axis) );
            gidLocal(0,axis) = gid(0,axis); 
            dimensionLocal(0,axis) = dimension(0,axis); 
        }
        else
        {
            gidLocal(0,axis) = a.getLocalBase(axis)+a.getGhostBoundaryWidth(axis);
            dimensionLocal(0,axis) = a.getLocalBase(axis); 
      // for internal ghost mark as periodic since these behave in the same was as periodic
      // ** we cannot mark as "0" since the mask may be non-zero at these points and assignBC will 
      // access points out of bounds
            bcLocal(0,axis) = -1; // bc(0,axis)>=0 ? 0 : -1;
        }
        
        if( a.getLocalBound(axis) == a.getBound(axis) ) 
        {
            assert( dimension(1,axis) == a.getLocalBound(axis) );
            
            gidLocal(1,axis) = gid(1,axis); 
            dimensionLocal(1,axis) = dimension(1,axis); 
        }
        else
        {
            gidLocal(1,axis) = a.getLocalBound(axis)-a.getGhostBoundaryWidth(axis);
            dimensionLocal(1,axis) = a.getLocalBound(axis);
      // for internal ghost mark as periodic since these behave in the same was as periodic
            bcLocal(1,axis) = -1; // bc(1,axis)>=0 ? 0 : -1;
        }
        
    }
}


#define moda(n,m) (n)>0 ? (n)%(m) : ((m)-(n))%(m)


//\begin{>>ogenUpdateInclude.tex}{\subsubsection{updateRefinement : Adapative Grid updateOverlap}}
int Ogen::
updateRefinementNewer(CompositeGrid & cg, 
                  		      const int & refinementLevel /* = -1 */ )
// ===================================================================================
// /Description:
//    Update the mask and interpolation points for refinement grids.
//
// /refinementLevel (input): update this refinement level. By default update all refinement levels.
// /Notes:
//  A refinement grid prefers to interpolate from
//   \begin{enumerate}
//     \item Another refinement at the same level and same base grid
//     \item Another refinement at the same level and different base grid
//     \item The base grid.  ( -> ?? no need to check lower refinement levels ?? )
//   \end{enumerate}
//  Interpolation points on the base grid are not changed.
//\end{ogenUpdateInclude.tex}
// ===================================================================================
{
    real timeStart=getCPU();
    const int np=max(1,Communication_Manager::Number_Of_Processors);
    bool useOpt=true; 
  // bool useOptNew=true;

    Overture::checkMemoryUsage("Ogen::updateRefinementNewer (start)");  

  // printF("\n ***************** updateRefinementNewer *************\n\n");

//  debug=1;
//   debug=7;  // *************************

    if( cg.numberOfBaseGrids() <=1 )
    {
    // assign mask values at points hidden by finer patches
        cg.setMaskAtRefinements();
        return 0;
    }
    
  // const bool useNewAlgorithm=TRUE; 
    if( debug & 2 )
    {
        fprintf(plogFile, "\n ***** updateRefinementNew START ******\n");
    }


    updateParameters(cg); // this will compute interpolationOverlap

  // cg.update( MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEmask );
    int grid;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
        if( cg[grid].isRectangular() )
            cg[grid].update(MappedGrid::THEmask );
        else
            cg[grid].update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );
    }
    
  // dimension new serial array interpolation data arrays
    const int numberOfGrids=cg.numberOfGrids();
  // adjustSizeMacro(cg->interpolationPointLocal,numberOfGrids);
        while( cg->interpolationPointLocal.getLength() < numberOfGrids )
            cg->interpolationPointLocal.addElement();
        while( cg->interpolationPointLocal.getLength() > numberOfGrids )
            cg->interpolationPointLocal.deleteElement();
  // adjustSizeMacro(cg->interpoleeGridLocal,numberOfGrids);
        while( cg->interpoleeGridLocal.getLength() < numberOfGrids )
            cg->interpoleeGridLocal.addElement();
        while( cg->interpoleeGridLocal.getLength() > numberOfGrids )
            cg->interpoleeGridLocal.deleteElement();
  // adjustSizeMacro(cg->variableInterpolationWidthLocal,numberOfGrids);
        while( cg->variableInterpolationWidthLocal.getLength() < numberOfGrids )
            cg->variableInterpolationWidthLocal.addElement();
        while( cg->variableInterpolationWidthLocal.getLength() > numberOfGrids )
            cg->variableInterpolationWidthLocal.deleteElement();
  // adjustSizeMacro(cg->interpoleeLocationLocal,numberOfGrids);
        while( cg->interpoleeLocationLocal.getLength() < numberOfGrids )
            cg->interpoleeLocationLocal.addElement();
        while( cg->interpoleeLocationLocal.getLength() > numberOfGrids )
            cg->interpoleeLocationLocal.deleteElement();
  // adjustSizeMacro(cg->interpolationCoordinatesLocal,numberOfGrids);
        while( cg->interpolationCoordinatesLocal.getLength() < numberOfGrids )
            cg->interpolationCoordinatesLocal.addElement();
        while( cg->interpolationCoordinatesLocal.getLength() > numberOfGrids )
            cg->interpolationCoordinatesLocal.deleteElement();

    IntegerArray & numberOfInterpolationPointsLocal = cg->numberOfInterpolationPointsLocal;
    numberOfInterpolationPointsLocal.redim(numberOfGrids);
    Range G=cg.numberOfBaseGrids();
    numberOfInterpolationPointsLocal(G)=cg.numberOfInterpolationPoints(G);
    
    
    real time1=getCPU();
    real timeForUpdate=time1-timeStart;
  // printf(" cg.interpoleeGrid[0]: min=%i, max=%i (start)\n",min(cg.interpoleeGrid[0]),max(cg.interpoleeGrid[0]));

    Range Rx(0,cg.numberOfDimensions()-1),all;
    const int & numberOfDimensions = cg.numberOfDimensions();

    Index Iv[4], &I1 = Iv[0], &I2=Iv[1], &I3=Iv[2];
    Index Jv[3], &J1 = Jv[0], &J2=Jv[1], &J3=Jv[2];
    Index Kv[3], &K1 = Kv[0], &K2=Kv[1], &K3=Kv[2];
    Index Ivr[3], &I1r = Ivr[0], &I2r=Ivr[1], &I3r=Ivr[2];
    Index Ivb[3], &I1b = Ivb[0], &I2b=Ivb[1], &I3b=Ivb[2];
            
    int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
    int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
    int kv[3], &k3=kv[2];
    int lv[3], &l3=lv[2];
    int ie[3], &ie1=ie[0], &ie2=ie[1], &ie3=ie[2];
    int iBase[3],iBound[3];

//  realArray rr(1,3),xx(1,3),rb(1,3); rr=-1.; rb=-1.;
//  RealArray rrs(1,3), xxs(1,3),rbs(1,3); rrs=-1.; rbs=-1.;
    
//  intSerialArray interpolates(1), useBackupRules(1);
//  useBackupRules=FALSE;
//  const int notAssigned = INT_MIN;
//  const int mgLevel=0;  // *** multigrid level
    
  // 
  // If checkForOneSided=TRUE then canInterpolate will not allow a one-sided interpolation
  // stencil to use ANY interiorBoundaryPoint's -- this is actually too strict. We really
  // only want to disallow interpolation that has less than the minimum overlap distance
  //
    checkForOneSided=FALSE;  
    int axis,dir;
    int l;

  // allocate temporary arrays to hold the new interpolation points on the refinement level.
  // * intSerialArray **interpolationPoints = new intSerialArray* [cg.numberOfRefinementLevels()];

    int i1b,i2b,i3b,i1r,i2r,i3r;
    int I1bBase,I2bBase,I3bBase; 
    int I1bBound,I2bBound, I3bBound;
    int I1rBase,I2rBase,I3rBase; 
    int I1rBound,I2rBound,I3rBound;
    int I1rStride,I2rStride,I3rStride;
    int I1Base,I2Base,I3Base; 
    int I1Bound,I2Bound,I3Bound;
    int I1Stride,I2Stride,I3Stride;

    real timeForMarkOffAxis=0., timeForMarkInterp=0.;
    
  // * for( l=1; l<cg.numberOfRefinementLevels(); l++ )
  // *   interpolationPoints[l] = new intSerialArray [cg.refinementLevel[l].numberOfComponentGrids()];
    
  // ******************************************************************************************
  //  For each refinement level above the coarsest,
  //  build a mask array for each refinement grid, based on the mask array for coarser levels.
  //  The mask array will indicate which points should be interpolated.    
  // ******************************************************************************************
    for( l=1; l<cg.numberOfRefinementLevels(); l++ )
    {
        GridCollection & rl = cg.refinementLevel[l];
    // IntegerArray *interpolationPoints = new IntegerArray [rl.numberOfComponentGrids()];
        
        int g;
        for( g=0; g<rl.numberOfComponentGrids(); g++ )
        {
            int grid =rl.gridNumber(g);        // index into cg
            int bg = cg.baseGridNumber(grid);  // base grid for this refinement
            
      // printf("updateOverlap(refinements): update level=%i, g=%i, grid=%i from base grid %i\n",l,g,grid,bg);

            MappedGrid & cr = rl[g];              // refined grid
            MappedGrid & cb = cg[bg];             // base grid


            int rf[3];  // refinement factors to the base grid.
            rf[0]=rl.refinementFactor(0,g);
            rf[1]=rl.refinementFactor(1,g);
            rf[2]=rl.refinementFactor(2,g);
            
            assert( rf[0]>0 && rf[1]>0 && rf[2]>0 );

            int rrf[3]={1,1,1}; // refinement factors to the next level
            const int gParent=0;  // assume refinement factors are the same for all grids.
            for( axis=0; axis<3; axis++ )
                rrf[axis]=rl.refinementFactor(0,g)/cg.refinementLevel[l-1].refinementFactor(0,gParent);


      // Here are arrays that hold the local dimensions and bc's (with bc==-1 at parallel boundaries)
            IntegerArray gidb(2,3),dimensionb(2,3),bcb(2,3);
            IntegerArray gidr(2,3),dimensionr(2,3),bcr(2,3);
            
            getLocalBoundsAndBoundaryConditions(cr.mask(),gidr,dimensionr,bcr);
            getLocalBoundsAndBoundaryConditions(cb.mask(),gidb,dimensionb,bcb);


            const intArray & maskb = cb.mask();   // base grid mask
            intArray & maskr = cr.mask();         // *** here is the mask we need to assign ***
#ifndef USE_PPP
            const intSerialArray & maskbLocal = maskb;
            intSerialArray & maskrLocal = maskr;
#else
            intSerialArray maskrLocal; getLocalArrayWithGhostBoundaries(maskr,maskrLocal);
            intSerialArray maskbLocal; // getLocalArrayWithGhostBoundaries(maskb,maskbLocal);

      // maskbLocal: we need a copy of the base grid mask points that lie underneath this refinement grid
      //             This will require a parallel copy (below)
            int ghost[4]={0,0,0,0}; //
            for( axis=0; axis<numberOfDimensions; axis++ )
            {
	// *wdh* 060320 : use 2 ghost pts now that the local fine grid is a bit bigger
	// -- really only need an extra ghost on parallel ghost -- sometimes 
      	ghost[axis]=2;  // one ghost pt on the coarse grid will correspond to rrf points on the refinement grid
            }
      	
            getIndex(cr.extendedIndexRange(),I1,I2,I3);
            Iv[3]=0;

      // Copy coarse grid values maskb, to local arrays maskbLocal that are distributed 
      // in the same way as a fine grid maskr:  (ghost=extra ghost values to include on maskbLocal)
            CopyArray::copyCoarseToFine( maskb, maskr, Iv, maskbLocal, rf, ghost);
      	
#endif

            if( debug & 2 )
            {
      	fprintf(plogFile, "\n ---------Start a new grid -------\n"
                                "myid=%i l=%i g=%i grid=%i bg=%i\n"
                                "        maskbLocal bounds =[%i,%i][%i,%i]\n"
            		"        maskrLocal bounds =[%i,%i][%i,%i]\n",
                                myid,l,g,grid,bg,
            		maskbLocal.getBase(0),maskbLocal.getBound(0),
            		maskbLocal.getBase(1),maskbLocal.getBound(1),
            		maskrLocal.getBase(0),maskrLocal.getBound(0),
            		maskrLocal.getBase(1),maskrLocal.getBound(1));
            }


            maskrLocal=MappedGrid::ISdiscretizationPoint;

      // make a copy of the mask which is larger than the extended index range, just
      // so we can index it in a convenient way. (we may want to index points outside the extendedIndexRange)
            getIndex(cr.extendedIndexRange(),I1,I2,I3);

            bool ok=true;
            bool assignLocal=true;  // if false, the refinement grid does not exist on this processor
#ifdef USE_PPP
      // restrict bounds to local array *wdh* 030619 -- include ghost (bugp5.cmd)
            assignLocal=ParallelUtility::getLocalArrayBounds(maskr,maskrLocal,I1,I2,I3,1);  

            for( axis=0; axis<3; axis++ )
            {
      	Iv[axis]=Range(max(cr.extendedIndexRange(0,axis),Iv[axis].getBase()),
                   		       min(cr.extendedIndexRange(1,axis),Iv[axis].getBound()));
            }
#endif
            J3=I3;
            for( axis=0; axis<numberOfDimensions; axis++ )
            {
      	int base  = Iv[axis].getBase();
      	int bound = Iv[axis].getBound();

                int baseMod = base>0 ? base % rf[axis] : rf[axis] - ((-base) % rf[axis]);
                int boundMod = bound>0 ? bound % rf[axis] : rf[axis] - ((-bound) % rf[axis]);
      	
                base = baseMod == 0 ? base-rf[axis] : base  - baseMod;
      	bound= boundMod==0 ? bound+rf[axis] : bound + rf[axis]-boundMod;

      	assert( base%rf[axis] ==0 && bound%rf[axis] ==0 );

      	Iv[axis]=Range(max(base,cb.dimension(Start,axis)*rf[axis]), min(bound,cb.dimension(End,axis)*rf[axis]));

//         base =max(cr.dimension(Start,axis),base);  // *wdh* 060319 switch max <-> min
// 	bound=min(cr.dimension(End  ,axis),bound);
                Jv[axis]=Range(base,bound);
            }


            intSerialArray maskLocal;    // ** mask for refinement grid (with extra space)
            if( assignLocal )
                maskLocal.redim(J1,J2,J3);

            getIndex(cr.dimension(),J1,J2,J3);

#ifdef USE_PPP
      // ** ParallelUtility::getLocalArrayBounds(maskr,maskrLocal,J1,J2,J3);  // restrict bounds to local array
            for( int dir=0; dir<3; dir++ )
            {
      	Jv[dir]=Range(max(maskLocal.getBase(dir),maskrLocal.getBase(dir)),
                  		      min(maskLocal.getBound(dir),maskrLocal.getBound(dir)));
            }
#endif

            if( assignLocal )
            {
        	  
      	maskLocal=MappedGrid::ISdiscretizationPoint; 
                maskLocal(J1,J2,J3)=maskrLocal(J1,J2,J3);       // default values.
            
	// getIndex(cr.indexRange(),I1,I2,I3);
      	if( debug & 4 )
        	  fprintf(plogFile,"refinement level=%i, g=%i, indexRange=(%i,%i)X(%i,%i) Iv[0]=(%i,%i)\n",l,g,
              		  cr.indexRange(0,0),
              		  cr.indexRange(1,0),cr.indexRange(0,1),cr.indexRange(1,1),Iv[0].getBase(),Iv[0].getBound());
                        

	// Ivr : refinement grid bounds plus a stride
	// Ivb : base grid bounds, stride 1
      	for( axis=0; axis<3; axis++ )
      	{
        	  Ivr[axis]=Range(Iv[axis].getBase(),Iv[axis].getBound(),rf[axis]);
        	  Ivb[axis]=Range(floorDiv(Iv[axis].getBase(),rf[axis]),
                    			  floorDiv(Iv[axis].getBound(),rf[axis]));
      	}
      	if( debug & 4 )
        	  fprintf(plogFile," grid=%i : Iv=[%i,%i], Ivr=[%i,%i] Ivb=[%i,%i] rf=%i \n",grid,
              		  Iv[0].getBase(),Iv[0].getBound(),
              		  Ivr[0].getBase(),Ivr[0].getBound(),
              		  Ivb[0].getBase(),Ivb[0].getBound(),rf[0]);
            
      	for( axis=0; axis<numberOfDimensions; axis++ )
      	{
        	  if( Ivb[axis].getBase() < maskbLocal.getBase(axis) ||
            	      Ivb[axis].getBound() > maskbLocal.getBound(axis) )
        	  {
          	    printf("updateRefinementNew:ERROR local maskb array does not match maskr. myid=%i, grid=%i, bg=%i\n"
               		   "  Ivr=[%i,%i,%i][%i,%i,%i][%i,%i,%i] : refinement grid bounds plus a stride\n"
                                      "  Ivb=[%i,%i,%i][%i,%i,%i][%i,%i,%i] : base grid bounds (stride 1)\n"
                                      "  maskbLocal=[%i,%i][%i,%i][%i,%i]\n"
                                      "  maskrLocal=[%i,%i][%i,%i][%i,%i]\n",
               		   myid,grid,bg,
               		   Ivr[0].getBase(),Ivr[0].getBound(),Ivr[0].getStride(),
               		   Ivr[1].getBase(),Ivr[1].getBound(),Ivr[1].getStride(),
               		   Ivr[2].getBase(),Ivr[2].getBound(),Ivr[2].getStride(),
               		   Ivb[0].getBase(),Ivb[0].getBound(),Ivb[0].getStride(),
               		   Ivb[1].getBase(),Ivb[1].getBound(),Ivb[1].getStride(),
               		   Ivb[2].getBase(),Ivb[2].getBound(),Ivb[2].getStride(),
               		   maskbLocal.getBase(0),maskbLocal.getBound(0),
               		   maskbLocal.getBase(1),maskbLocal.getBound(1),
               		   maskbLocal.getBase(2),maskbLocal.getBound(2),
               		   maskrLocal.getBase(0),maskrLocal.getBound(0),
               		   maskrLocal.getBase(1),maskrLocal.getBound(1),
               		   maskrLocal.getBase(2),maskrLocal.getBound(2));

          	    if( plogFile!=NULL )
          	    {
            	      fprintf(plogFile,"updateRefinementNew:ERROR local maskb array does not match maskr. myid=%i, \n"
                  		      "  Ivb=[%i,%i][%i,%i][%i,%i]  maskbLocal=[%i,%i][%i,%i][%i,%i]\n",
                  		      myid,
                  		      Ivb[0].getBase(),Ivb[0].getBound(),
                  		      Ivb[1].getBase(),Ivb[1].getBound(),
                  		      Ivb[2].getBase(),Ivb[2].getBound(),
                  		      maskbLocal.getBase(0),maskbLocal.getBound(0),
                  		      maskbLocal.getBase(1),maskbLocal.getBound(1),
                  		      maskbLocal.getBase(2),maskbLocal.getBound(2));

            	      fclose(plogFile);
          	    }
        	  
          	    Overture::abort("error: local maskb array does not match maskr");
        	  }
      	
      	}
            

	// 
	//       X--.--X--.--X
	//       |  |  |  |  |
	//       .  0  0  0  .
	//       |  |  |  |  |
	//       X--0--0--0--X
	//       |  |  |  |  |
	//       .  0  0  0  .
	//       |  |  |  |  |
	//       X--.--X--.--X
	//

	//       if( FALSE && g==1 )
	//       {
	//         maskb.display("g=1, maskb");
	//         maskb(I1b,I2b,I3b).display("g=1, b(I1b,I2b,I3b)");
	//         mask(I1,I2,I3).display("g=1, mask after 1");
	//       }
            
	// special case if refinement aligns with the extendedIndexRange of a base grid
	// interpolation side -- set the ghost values in the mask to zero.
      	for( axis=0; axis<numberOfDimensions; axis++ )
      	{
        	  for( int side=Start; side<=End; side++ )
        	  {
          	    if( bcb(side,axis)==0 && // use local BC instead of: cb.boundaryCondition(side,axis)==0 && 
            		(cr.indexRange(side,axis) % rf[axis])==0 && // *wdh* 000630
            		floorDiv(cr.indexRange(side,axis),rf[axis])==cb.extendedIndexRange(side,axis) )
          	    {
	      // *wdh* 060206 - these next two lines do not seem to be used:
	      // getBoundaryIndex(cr.dimension(),side,axis,I1,I2,I3);
	      // Iv[axis]=cr.indexRange(side,axis);

            	      getBoundaryIndex(cr.dimension(),side,axis,J1,J2,J3);
            	      for( int ghost=1; ghost<=cr.numberOfGhostPoints(side,axis); ghost++ )
            	      {
            		Jv[axis]=cr.indexRange(side,axis)-ghost*(1-2*side);
#ifdef USE_PPP
            		if( !ParallelUtility::getLocalArrayBounds(maskr,maskrLocal,J1,J2,J3) )
              		  continue;
#endif
            		maskLocal(J1,J2,J3)=0;
            	      }
          	    }
        	  }
      	}

      	if( debug & 4 )
      	{
        	  fprintf(plogFile," after: grid=%i : Ivr=[%i,%i] Ivb=[%i,%i] \n",grid,Ivr[0].getBase(),Ivr[0].getBound(),
              		  Ivb[0].getBase(),Ivb[0].getBound());
        	  displayMask(maskLocal,"mask (1)",plogFile);
        	  fflush(plogFile);
      	}
            
            } // end if assignLocal

            int * maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
            const int maskDim0=maskLocal.getRawDataSize(0);
            const int maskDim1=maskLocal.getRawDataSize(1);
#undef MASK
#define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]
            const int * maskbp = maskbLocal.Array_Descriptor.Array_View_Pointer2;
            const int maskbDim0=maskbLocal.getRawDataSize(0);
            const int maskbDim1=maskbLocal.getRawDataSize(1);
#undef MASKB
#define MASKB(i0,i1,i2) maskbp[i0+maskbDim0*(i1+maskbDim1*(i2))]
            int * maskrp = maskrLocal.Array_Descriptor.Array_View_Pointer2;
            const int maskrDim0=maskrLocal.getRawDataSize(0);
            const int maskrDim1=maskrLocal.getRawDataSize(1);
#undef MASKR
#define MASKR(i0,i1,i2) maskrp[i0+maskrDim0*(i1+maskrDim1*(i2))]

            if( assignLocal )
            {
      	
	// ---- mark refinement holes that coincide with base grid holes. ----
	// Note: I1b : no stride, I1r: stride
      	FOR_3BR()
      	{
        	  if( MASKB(i1b,i2b,i3b)==0 )
        	  {
          	    MASK(i1r,i2r,i3r)=0;
        	  }
      	}

      	if( debug & 4 )
        	  displayMask(maskLocal,"mask (2)",plogFile);
            
      	int r;
	// Now mark refinement holes that lie directly between
	// base grid holes along each axis OR next to a hole point along an axis !!

	// Mark the mask at refinement grid points hat lie directly between base grid holes
      	if( numberOfDimensions==2 )
      	{
        	  if( rf[0]==2 && rf[1]==2 )
        	  {
          // 	    setMaskAtAlignedHoles(2,2);
                    I1bBase =I1b.getBase(),   I2bBase =I2b.getBase(),  I3bBase =I3b.getBase(); 
                    I1bBound=I1b.getBound(),  I2bBound=I2b.getBound(), I3bBound=I3b.getBound();
                    I1rBase  =I1r.getBase(),   I2rBase  =I2r.getBase(),   I3rBase  =I3r.getBase(); 
                    I1rBound =I1r.getBound(),  I2rBound =I2r.getBound(),  I3rBound =I3r.getBound();
                    I1rStride=I1r.getStride(), I2rStride=I2r.getStride(), I3rStride=I3r.getStride();
                    for(i3b=I3bBase,i3r=I3rBase; i3b<=I3bBound; i3b++,i3r+=I3rStride) 
                    for(i2b=I2bBase,i2r=I2rBase; i2b<=I2bBound; i2b++,i2r+=I2rStride) 
                    for(i1b=I1bBase,i1r=I1rBase; i1b<=I1bBound; i1b++,i1r+=I1rStride)
                    {
                        if( MASKB(i1b,i2b,i3b)==0 )
                        {
            //               #If "2" == "2"
                // *wdh* 070513 -- fixed so that only points between two adjacent maskb==0 points are marked 0 
                                if( i1b<I1bBound ) 
                                {
                          	if( MASKB(i1b+1,i2b,i3b)==0 ) MASK(i1r+1,i2r,i3r)=0;
                                }
                                else
                                {
                          	if( MASKB(i1b-1,i2b,i3b)==0 ) MASK(i1r-1,i2r,i3r)=0;
                                }
                                if( i2b<I2bBound ) 
                                {
                          	if( MASKB(i1b,i2b+1,i3b)==0 ) MASK(i1r,i2r+1,i3r)=0;
                                }
                                else
                                {
                          	if( MASKB(i1b,i2b-1,i3b)==0 ) MASK(i1r,i2r-1,i3r)=0;
                                }
                //                 #If "2" == "3"
                        }
                    }
        	  }
        	  else if( rf[0]==4 && rf[1]==4 )
        	  {
          // 	    setMaskAtAlignedHoles(2,4);
                    I1bBase =I1b.getBase(),   I2bBase =I2b.getBase(),  I3bBase =I3b.getBase(); 
                    I1bBound=I1b.getBound(),  I2bBound=I2b.getBound(), I3bBound=I3b.getBound();
                    I1rBase  =I1r.getBase(),   I2rBase  =I2r.getBase(),   I3rBase  =I3r.getBase(); 
                    I1rBound =I1r.getBound(),  I2rBound =I2r.getBound(),  I3rBound =I3r.getBound();
                    I1rStride=I1r.getStride(), I2rStride=I2r.getStride(), I3rStride=I3r.getStride();
                    for(i3b=I3bBase,i3r=I3rBase; i3b<=I3bBound; i3b++,i3r+=I3rStride) 
                    for(i2b=I2bBase,i2r=I2rBase; i2b<=I2bBound; i2b++,i2r+=I2rStride) 
                    for(i1b=I1bBase,i1r=I1rBase; i1b<=I1bBound; i1b++,i1r+=I1rStride)
                    {
                        if( MASKB(i1b,i2b,i3b)==0 )
                        {
            //               #If "4" == "2"
            //               #Elif "4" == "4"
                                if( i1b<I1bBound )
                                {
                                    if( MASKB(i1b+1,i2b,i3b)==0 )
                          	{
                            	  MASK(i1r+1,i2r,i3r)=0;
                            	  MASK(i1r+2,i2r,i3r)=0;
                            	  MASK(i1r+3,i2r,i3r)=0;
                          	}
                                }
                                else
                                {
                                    if( MASKB(i1b-1,i2b,i3b)==0 )
                          	{
                            	  MASK(i1r-1,i2r,i3r)=0;
                            	  MASK(i1r-2,i2r,i3r)=0;
                            	  MASK(i1r-3,i2r,i3r)=0;
                          	}
                                }
                                if( i2b<I2bBound )
                                {
                                    if( MASKB(i1b,i2b+1,i3b)==0 )
                          	{
                            	  MASK(i1r,i2r+1,i3r)=0;
                            	  MASK(i1r,i2r+2,i3r)=0;
                            	  MASK(i1r,i2r+3,i3r)=0;
                          	}
                                }
                                else
                                {
                                    if( MASKB(i1b,i2b-1,i3b)==0 )
                          	{
                            	  MASK(i1r,i2r-1,i3r)=0;
                            	  MASK(i1r,i2r-2,i3r)=0;
                            	  MASK(i1r,i2r-3,i3r)=0;
                          	}
                                }
                //                 #If "2" == "3"
                        }
                    }
        	  }
        	  else
        	  {
          // 	    setMaskAtAlignedHoles(2,general);
                    I1bBase =I1b.getBase(),   I2bBase =I2b.getBase(),  I3bBase =I3b.getBase(); 
                    I1bBound=I1b.getBound(),  I2bBound=I2b.getBound(), I3bBound=I3b.getBound();
                    I1rBase  =I1r.getBase(),   I2rBase  =I2r.getBase(),   I3rBase  =I3r.getBase(); 
                    I1rBound =I1r.getBound(),  I2rBound =I2r.getBound(),  I3rBound =I3r.getBound();
                    I1rStride=I1r.getStride(), I2rStride=I2r.getStride(), I3rStride=I3r.getStride();
                    for(i3b=I3bBase,i3r=I3rBase; i3b<=I3bBound; i3b++,i3r+=I3rStride) 
                    for(i2b=I2bBase,i2r=I2rBase; i2b<=I2bBound; i2b++,i2r+=I2rStride) 
                    for(i1b=I1bBase,i1r=I1rBase; i1b<=I1bBound; i1b++,i1r+=I1rStride)
                    {
                        if( MASKB(i1b,i2b,i3b)==0 )
                        {
            //               #If "general" == "2"
            //               #Elif "general" == "4"
            //               #Elif "general" == "general"
                                if( i1b<I1bBound )
                                {
                                    if( MASKB(i1b+1,i2b,i3b)==0 )
                                        for( r=1; r<rf[0]; r++ ) 
                              	    MASK(i1r+r,i2r,i3r)=0;
                                }
                                else 
                                {
                                    if( MASKB(i1b-1,i2b,i3b)==0 )
                                        for( r=1; r<rf[0]; r++ ) 
                              	    MASK(i1r-r,i2r,i3r)=0;
                                }
                                if( i2b<I2bBound )
                                {
                                    if( MASKB(i1b,i2b+1,i3b)==0 )
                                        for( r=1; r<rf[1]; r++ )
                              	    MASK(i1r,i2r+r,i3r)=0;
                                }
                                else 
                                {
                                    if( MASKB(i1b,i2b-1,i3b)==0 )
                                        for( r=1; r<rf[1]; r++ )
                              	    MASK(i1r,i2r-r,i3r)=0;
                                }
                //                 #If "2" == "3"
                        }
                    }
        	  }
      	}
      	else if( numberOfDimensions==3 )
      	{
        	  if( rf[0]==2 && rf[1]==2 && rf[2]==2 )
        	  {
          // 	    setMaskAtAlignedHoles(3,2);
                    I1bBase =I1b.getBase(),   I2bBase =I2b.getBase(),  I3bBase =I3b.getBase(); 
                    I1bBound=I1b.getBound(),  I2bBound=I2b.getBound(), I3bBound=I3b.getBound();
                    I1rBase  =I1r.getBase(),   I2rBase  =I2r.getBase(),   I3rBase  =I3r.getBase(); 
                    I1rBound =I1r.getBound(),  I2rBound =I2r.getBound(),  I3rBound =I3r.getBound();
                    I1rStride=I1r.getStride(), I2rStride=I2r.getStride(), I3rStride=I3r.getStride();
                    for(i3b=I3bBase,i3r=I3rBase; i3b<=I3bBound; i3b++,i3r+=I3rStride) 
                    for(i2b=I2bBase,i2r=I2rBase; i2b<=I2bBound; i2b++,i2r+=I2rStride) 
                    for(i1b=I1bBase,i1r=I1rBase; i1b<=I1bBound; i1b++,i1r+=I1rStride)
                    {
                        if( MASKB(i1b,i2b,i3b)==0 )
                        {
            //               #If "2" == "2"
                // *wdh* 070513 -- fixed so that only points between two adjacent maskb==0 points are marked 0 
                                if( i1b<I1bBound ) 
                                {
                          	if( MASKB(i1b+1,i2b,i3b)==0 ) MASK(i1r+1,i2r,i3r)=0;
                                }
                                else
                                {
                          	if( MASKB(i1b-1,i2b,i3b)==0 ) MASK(i1r-1,i2r,i3r)=0;
                                }
                                if( i2b<I2bBound ) 
                                {
                          	if( MASKB(i1b,i2b+1,i3b)==0 ) MASK(i1r,i2r+1,i3r)=0;
                                }
                                else
                                {
                          	if( MASKB(i1b,i2b-1,i3b)==0 ) MASK(i1r,i2r-1,i3r)=0;
                                }
                //                 #If "3" == "3"
                                if( i3b<I3bBound ) 
                                {
                          	if( MASKB(i1b,i2b,i3b+1)==0 ) MASK(i1r,i2r,i3r+1)=0;
                                }
                                else
                                {
                          	if( MASKB(i1b,i2b,i3b-1)==0 ) MASK(i1r,i2r,i3r-1)=0;
                                }
                        }
                    }
        	  }
        	  else if( rf[0]==4 && rf[1]==4 && rf[2]==4 )
        	  {
          // 	    setMaskAtAlignedHoles(3,4);
                    I1bBase =I1b.getBase(),   I2bBase =I2b.getBase(),  I3bBase =I3b.getBase(); 
                    I1bBound=I1b.getBound(),  I2bBound=I2b.getBound(), I3bBound=I3b.getBound();
                    I1rBase  =I1r.getBase(),   I2rBase  =I2r.getBase(),   I3rBase  =I3r.getBase(); 
                    I1rBound =I1r.getBound(),  I2rBound =I2r.getBound(),  I3rBound =I3r.getBound();
                    I1rStride=I1r.getStride(), I2rStride=I2r.getStride(), I3rStride=I3r.getStride();
                    for(i3b=I3bBase,i3r=I3rBase; i3b<=I3bBound; i3b++,i3r+=I3rStride) 
                    for(i2b=I2bBase,i2r=I2rBase; i2b<=I2bBound; i2b++,i2r+=I2rStride) 
                    for(i1b=I1bBase,i1r=I1rBase; i1b<=I1bBound; i1b++,i1r+=I1rStride)
                    {
                        if( MASKB(i1b,i2b,i3b)==0 )
                        {
            //               #If "4" == "2"
            //               #Elif "4" == "4"
                                if( i1b<I1bBound )
                                {
                                    if( MASKB(i1b+1,i2b,i3b)==0 )
                          	{
                            	  MASK(i1r+1,i2r,i3r)=0;
                            	  MASK(i1r+2,i2r,i3r)=0;
                            	  MASK(i1r+3,i2r,i3r)=0;
                          	}
                                }
                                else
                                {
                                    if( MASKB(i1b-1,i2b,i3b)==0 )
                          	{
                            	  MASK(i1r-1,i2r,i3r)=0;
                            	  MASK(i1r-2,i2r,i3r)=0;
                            	  MASK(i1r-3,i2r,i3r)=0;
                          	}
                                }
                                if( i2b<I2bBound )
                                {
                                    if( MASKB(i1b,i2b+1,i3b)==0 )
                          	{
                            	  MASK(i1r,i2r+1,i3r)=0;
                            	  MASK(i1r,i2r+2,i3r)=0;
                            	  MASK(i1r,i2r+3,i3r)=0;
                          	}
                                }
                                else
                                {
                                    if( MASKB(i1b,i2b-1,i3b)==0 )
                          	{
                            	  MASK(i1r,i2r-1,i3r)=0;
                            	  MASK(i1r,i2r-2,i3r)=0;
                            	  MASK(i1r,i2r-3,i3r)=0;
                          	}
                                }
                //                 #If "3" == "3"
                                if( i3b<I3bBound )
                                {
                                    if( MASKB(i1b,i2b,i3b+1)==0 )
                          	{
                            	  MASK(i1r,i2r,i3r+1)=0;
                            	  MASK(i1r,i2r,i3r+2)=0;
                            	  MASK(i1r,i2r,i3r+3)=0;
                          	}
                                }
                                else 
                                {
                                    if( MASKB(i1b,i2b,i3b-1)==0 ) 
                          	{
                            	  MASK(i1r,i2r,i3r-1)=0;
                            	  MASK(i1r,i2r,i3r-2)=0;
                            	  MASK(i1r,i2r,i3r-3)=0;
                          	}
                                }
                        }
                    }
        	  }
        	  else
        	  {
          // 	    setMaskAtAlignedHoles(3,general);
                    I1bBase =I1b.getBase(),   I2bBase =I2b.getBase(),  I3bBase =I3b.getBase(); 
                    I1bBound=I1b.getBound(),  I2bBound=I2b.getBound(), I3bBound=I3b.getBound();
                    I1rBase  =I1r.getBase(),   I2rBase  =I2r.getBase(),   I3rBase  =I3r.getBase(); 
                    I1rBound =I1r.getBound(),  I2rBound =I2r.getBound(),  I3rBound =I3r.getBound();
                    I1rStride=I1r.getStride(), I2rStride=I2r.getStride(), I3rStride=I3r.getStride();
                    for(i3b=I3bBase,i3r=I3rBase; i3b<=I3bBound; i3b++,i3r+=I3rStride) 
                    for(i2b=I2bBase,i2r=I2rBase; i2b<=I2bBound; i2b++,i2r+=I2rStride) 
                    for(i1b=I1bBase,i1r=I1rBase; i1b<=I1bBound; i1b++,i1r+=I1rStride)
                    {
                        if( MASKB(i1b,i2b,i3b)==0 )
                        {
            //               #If "general" == "2"
            //               #Elif "general" == "4"
            //               #Elif "general" == "general"
                                if( i1b<I1bBound )
                                {
                                    if( MASKB(i1b+1,i2b,i3b)==0 )
                                        for( r=1; r<rf[0]; r++ ) 
                              	    MASK(i1r+r,i2r,i3r)=0;
                                }
                                else 
                                {
                                    if( MASKB(i1b-1,i2b,i3b)==0 )
                                        for( r=1; r<rf[0]; r++ ) 
                              	    MASK(i1r-r,i2r,i3r)=0;
                                }
                                if( i2b<I2bBound )
                                {
                                    if( MASKB(i1b,i2b+1,i3b)==0 )
                                        for( r=1; r<rf[1]; r++ )
                              	    MASK(i1r,i2r+r,i3r)=0;
                                }
                                else 
                                {
                                    if( MASKB(i1b,i2b-1,i3b)==0 )
                                        for( r=1; r<rf[1]; r++ )
                              	    MASK(i1r,i2r-r,i3r)=0;
                                }
                //                 #If "3" == "3"
                                if( i3b<I3bBound )
                                {
                                    if( MASKB(i1b,i2b,i3b+1)==0 )
                                        for( r=1; r<rf[2]; r++ )
                              	    MASK(i1r,i2r,i3r+r)=0;
                                }
                                else 
                                {
                                    if( MASKB(i1b,i2b,i3b-1)==0 ) 
                            	  for( r=1; r<rf[2]; r++ )
                              	    MASK(i1r,i2r,i3r-r)=0;
                                }
                        }
                    }
        	  }
      	}

      	if( debug & 4 )
        	  displayMask(maskLocal,"mask (3)",plogFile);
      	
            } // end if assignLocal
            
//        if( false )
//        {
//  	if( max(abs(mask-maskSave2))!=0 )
//  	{
//  	  printf("**** ERROR in setMaskAtAlignedHoles ****\n");
              
//  	  if( debug & 4 )
//  	  {
//  	    fPrintF(logFile,"**** ERROR in setMaskAtAlignedHoles ****\n");
//  	    displayMask(maskSave2,"maskSave2 (old way)",logFile);
//  	    displayMask(mask,"mask (new way)",logFile);
//  	    mask=abs(mask-maskSave2);
//  	    displayMask(mask,"diff",logFile);
//  	  }

//  	  mask=maskSave2;
//  	}
//  	else
//  	{
//  	  printf("**** PASSED setMaskAtAlignedHoles ****\n");
//  	}
//        }
            
            if( assignLocal )
            {

	// Now mark off-axis points as holes if any corner of the cell is a hole.
      	real timea=getCPU();
      	Index Jvr[3],Jvb[3];
	// Jvr[0]=Ivr[0], Jvr[1]=Ivr[1], Jvr[2]=Ivr[2];
	// Jvb[0]=Ivb[0], Jvb[1]=Ivb[1], Jvb[2]=Ivb[2];
	// for( int step=0; step<=100; step++ )
	// {
      	
	//   Ivr[0]=Jvr[0], Ivr[1]=Jvr[1], Ivr[2]=Jvr[2];
	//   Ivb[0]=Jvb[0], Ivb[1]=Jvb[1], Ivb[2]=Jvb[2];

      	markOffAxisRefinementMask( numberOfDimensions,Ivr,Ivb,rf,maskLocal,maskbLocal );
	// }
            
      	timeForMarkOffAxis+=getCPU()-timea;

      	if( debug & 4 )
        	  displayMask(maskLocal,"mask (4)",plogFile);


	// ****************************************************************
	// *** Finally mark extra ghost line values of the refinement. ****
	// ****************************************************************

      	for( axis=0; axis<numberOfDimensions; axis++ )
      	{
        	  for( int side=Start; side<=End; side++ )
        	  {
	    // there may be some hanging points for rrf > 2
	    //  --------X
	    //          |    / this point needs to be marked as interpolation
	    //          |   /
	    //      + + +  +  x x          <--- ghost line on refinement (no refinement patch above)
	    //  --------X--+--+-x-X        <--- top edge of refinement
	    //      + + +  +  + + x x
	    //          |       + | x x<----- interp pts on refinement
	    //                  + | + x
	    // *wdh* 060603 if( rrf[axis]>2 && cr.boundaryCondition(side,axis)==0  )
          	    if( rrf[axis]>2 && bcr(side,axis)==0  )
          	    {
            	      for( int ghost=1; ghost<=1; ghost++ )
            	      {
            		getBoundaryIndex(cr.gridIndexRange(),side,axis,I1,I2,I3);

		// printf(" ===== grid=%i (side,axis)=(%i,%i) baseGrid=%i \n",grid,side,axis,bg);
		// displayMask(mask(I1,I2,I3),"mask(I1,I2,I3) (boundary line)");

		// const int axisp1 = (axis+1) % numberOfDimensions;

            		Iv[axis] = cr.indexRange(side,axis)-ghost*(1-2*side);

		// restrict bounds to this processor:
            		bool ok=ParallelUtility::getLocalArrayBounds(maskr,maskrLocal,I1,I2,I3);
            		if( !ok ) continue;  // no pts on this processor

                // **** this looks wrong -- not used in 2D below  -- check 3D ---

                // $$$$$$$$$$$$$$$$ check this $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

                // Iv[axisp1]=Range(Iv[axisp1].getBase()-rrf[axisp1],Iv[axisp1].getBound(),rrf[axisp1]);

                // *wdh* 061216 -- 
                                for( int dir=1; dir<numberOfDimensions; dir++ )  // loop over tangential directions
            		{
                                    const int dirp=(axis+dir) % numberOfDimensions;
// 		  if( Iv[dirp].getBase()>Iv[dirp].getBound()-rrf[dirp] )
// 		  {
// 		    printf("updateRefinement:ERROR: myid=%i, there are not enough points along axis=%i, Iv=[%i,%i]\n"
// 			   "    I assume that there are at least %i points for refinement ratio=%i\n"
// 			   "    This is now treated as a FATAL error. \n",
// 			   myid,dirp,Iv[dirp].getBase(),Iv[dirp].getBound(),rrf[dirp],rrf[dirp]);
// 		    Overture::abort("error");
// 		  }
            		
                  // Shift the tangential Index's to the left by the refinement ratio:
              		  Iv[dirp]=Range(Iv[dirp].getBase()-rrf[dirp],Iv[dirp].getBound()-rrf[dirp],rrf[dirp]);

                                    #ifdef USE_PPP
                // 		    setLocalBoundsWithStride(Iv[dirp],dirp,rrf[dirp],ok);
                                {
                  // /Description:
                  // Adjust the Index Iv[dirp] with a stride rrf[dirp] to fit on this processor.
                  // This shift should work since the mask array has been enlarged to account for the refinement rrf[dirp]
                  // 
                  // /Iv[dirp] (input/output) : if ok==true on output then Iv[dirp] is the adjusted index
                  // /ok (output) : return true if there is an non-empty Index Iv[dirp]
                                    int na=Iv[dirp].getBase();
                                    int nb=Iv[dirp].getBound();
                                    int ma=maskLocal.getBase(dirp);
                                    int mb=maskLocal.getBound(dirp);
                                    ok=true;
                                    if( na<ma )
                                    { // Shift na by a multiple of rrf[dirp] so that it starts on this processor
                                        na += ( (ma-na +rrf[dirp]-1)/rrf[dirp] )*rrf[dirp];
                                        ok=false;
                                    }
                                    if( nb>mb )
                                    { // Shift nb by a multiple of rrf[dirp] so that it ends on this processor
                                        nb -= ( (nb-mb +rrf[dirp]-1)/rrf[dirp] )*rrf[dirp];
                                        ok=false;
                                    }	      
                                    if( !ok )
                                    { // bounds were adjusted
                                        ok = nb>=na; 
                                        if( ok )
                                            Iv[dirp]=Range(na,nb,rrf[dirp]);
                                    }
                                }
                		    if( !ok ) break;
                                    #endif
                                    assert( Iv[dirp].getBase()<=Iv[dirp].getBound() );
                                    assert( (Iv[dirp].getBound()+rrf[dirp]-1)<=maskLocal.getBound(dirp) );
            		}
            		if( !ok ) continue;
            		

                                assert( I1.getBase()>=maskLocal.getBase(0) && I1.getBound()<=maskLocal.getBound(0) );
                                assert( I2.getBase()>=maskLocal.getBase(1) && I2.getBound()<=maskLocal.getBound(1) );
                                assert( I3.getBase()>=maskLocal.getBase(2) && I3.getBound()<=maskLocal.getBound(2) );
            	      
	      // displayMask(mask(I1,I2,I3),"mask(I1,I2,I3) (every 4th pt)");

            		if( numberOfDimensions==2 )
            		{
		  // Mark intermediate points on ghost lines when exactly one coarse grid point is an interior point
		  //         ----X-+-+-+-0----
              		  if( axis==0 )
              		  {
		    // *wdh* 061215 Range I2m(I2.getBase(),I2.getBound()-rrf[1],rrf[1]);
		    // *wdh* 061215 FOR_3S(i1,i2,i3,I1,I2m,I3)  // *wdh* 061103 -- fixed loop bounds
                		    FOR_3S(i1,i2,i3,I1,I2,I3)  
                		    {
                  		      if( ((MASK(i1,i2,i3)==0)+(MASK(i1,i2+rrf[1],i3)==0)) ==1 )
                  		      {
                  			for( int r=1; r<rrf[1]; r++ )
                  			{
                    			  MASK(i1,i2+r,i3)=0;
                  			}
                  		      }
                		    }
              		  }
              		  else
              		  {
		    // *wdh* 061215 Range I1m(I1.getBase(),I1.getBound()-rrf[0],rrf[0]);
		    // *wdh* 061215 FOR_3S(i1,i2,i3,I1m,I2,I3)  // *wdh* 061103 -- fixed loop bounds
                		    FOR_3S(i1,i2,i3,I1,I2,I3)  // *wdh* 061103 -- fixed loop bounds
                		    {
                  		      if( ((MASK(i1,i2,i3)==0)+(MASK(i1+rrf[0],i2,i3)==0)) ==1 )
                  		      {
                  			for( int r=1; r<rrf[0]; r++ )
                  			{
                    			  MASK(i1+r,i2,i3)=0;
                  			}
                  		      }
                		    }
              		  }
              		  
            		}
            		else if( numberOfDimensions==3 )
            		{
              		  const int axisp2 = (axis+2) % numberOfDimensions;

		  // Mark intermediate points on ghost lines when exactly one coarse grid point is an interior point
		  // check the 4 corners of the face.

		  // *wdh* 061215 Range I1m(I1.getBase(),I1.getBound()-rrf[0],rrf[0]);   // **** this looks wrong
		  // *wdh* 061215 Range I2m(I2.getBase(),I2.getBound()-rrf[1],rrf[1]);
		  // *wdh* 061215 Range I3m(I3.getBase(),I3.getBound()-rrf[2],rrf[2]);
              		  if( axis==0 )
              		  {
		    // *wdh* 061215 FOR_3S(i1,i2,i3,I1,I2m,I3m) // *wdh* 061103 -- fixed loop bounds
                		    FOR_3S(i1,i2,i3,I1,I2,I3) 
                		    {
                  		      if( ((MASK(i1,i2       ,i3       )==0)+(MASK(i1,i2+rrf[1],i3       )==0)+
                     			   (MASK(i1,i2       ,i3+rrf[2])==0)+(MASK(i1,i2+rrf[1],i3+rrf[2])==0)) ==1 )
                  		      {
                  			for( int r3=1; r3<rrf[2]; r3++ )
                    			  for( int r2=1; r2<rrf[1]; r2++ )
                    			  {
                      			    MASK(i1,i2+r2,i3+r3)=0;
                    			  }
                  		      }
                		    }
              		  }
              		  else if( axis==1 )
              		  {
		    // *wdh* 061215 FOR_3S(i1,i2,i3,I1m,I2,I3m) // *wdh* 061103 -- fixed loop bounds
                		    FOR_3S(i1,i2,i3,I1,I2,I3) 
                		    {
                  		      if( ((MASK(i1,i2       ,i3       )==0)+(MASK(i1+rrf[0],i2,i3       )==0)+
                     			   (MASK(i1,i2       ,i3+rrf[2])==0)+(MASK(i1+rrf[0],i2,i3+rrf[2])==0)) ==1 )
                  		      {
                  			for( int r3=1; r3<rrf[2]; r3++ )
                    			  for( int r1=1; r1<rrf[0]; r1++ )
                    			  {
                      			    MASK(i1+r1,i2,i3+r3)=0;
                    			  }
                  		      }
                		    }
              		  }
              		  else
              		  {
		    // *wdh* 061215 FOR_3S(i1,i2,i3,I1m,I2m,I3) // *wdh* 061103 -- fixed loop bounds
                		    FOR_3S(i1,i2,i3,I1,I2,I3) 
                		    {
                  		      if( ((MASK(i1       ,i2       ,i3)==0)+(MASK(i1       ,i2+rrf[1],i3)==0)+
                     			   (MASK(i1+rrf[0],i2       ,i3)==0)+(MASK(i1+rrf[0],i2+rrf[1],i3)==0)) ==1 )
                  		      {
                  			for( int r2=1; r2<rrf[1]; r2++ )
                    			  for( int r1=1; r1<rrf[0]; r1++ )
                    			  {
                      			    MASK(i1+r1,i2+r2,i3)=0;
                    			  }
                  		      }
                		    }
              		  }
            		}
            	      }
          	    }  // end if( rrf[axis]>2 && ..

          	    getBoundaryIndex(cr.dimension(),side,axis,I1,I2,I3);
          	    Iv[axis]=cr.indexRange(side,axis);
          	    ok=true;
#ifdef USE_PPP
          	    ok=ParallelUtility::getLocalArrayBounds(maskr,maskrLocal,I1,I2,I3);
#endif
          	    if( ok )
          	    {
            	      getBoundaryIndex(cr.dimension(),side,axis,J1,J2,J3);
            	      for( int ghost=2; ghost<=cr.numberOfGhostPoints(side,axis); ghost++ )
            	      {
            		Jv[axis]=cr.indexRange(side,axis)-ghost*(1-2*side);
#ifdef USE_PPP
            		ok=ParallelUtility::getLocalArrayBounds(maskr,maskrLocal,J1,J2,J3);
#endif
		// special case if refinement aligns with the extendedIndexRange of a base grid
		// interpolation side.
            		if( ok ) // *wdh* 061103
            		{
              		  FOR_3IJD(i1,i2,i3,I1,I2,I3,j1,j2,j3,J1,J2,J3)
              		  {
                		    if( MASK(i1,i2,i3)==0 )
                		    {
                  		      MASK(j1,j2,j3)=0;
                		    }
                		    else if( MASK(j1,j2,j3)!=0 )  // *wdh* 981017
                		    {
                  		      MASK(j1,j2,j3)=MappedGrid::ISghostPoint;
                		    }
              		  }
            		}
            	      }
          	    }
        	  }
      	}  // end for axis
      	
            } // end if assignLocal
            
            if( debug & 4 )
      	displayMask(maskLocal,"mask (5) (finished marking maskLocal)",plogFile);


      // ***** end marking ghost lines ******

//        if( jj==0 ) 
//        { maskSave2=mask; // new way
//          mask=maskSave1;  // reset 
//        }
//        } // end for jj
            
//        if( max(abs(mask-maskSave2))!=0 )
//        {
//  	printf("**** ERROR in setMaskAtAlignedHoles ****\n");
              
//  	if( debug & 4 )
//  	{
//            fPrintF(logFile,"**** ERROR in setMaskAtAlignedHoles ****\n");
//  	  displayMask(mask,"mask (old way)",logFile);
//  	  displayMask(maskSave2,"maskSave2 (new way)",logFile);
//            maskSave1=abs(mask-maskSave2);
//  	  displayMask(maskSave1,"diff",logFile);
//  	}
//        }
//        else
//        {
//  	printf("**** PASSED set Hanging nodes ****\n");
//        }

            real timei=getCPU();
      // mark interpolation points on the refinement
      // intSerialArray & ip = interpolationPoints[l][g];
            intSerialArray & ip = cg->interpolationPointLocal[grid];  // use the local array

            int maxInterpNew; // estimate the number of interp points we will have *wdh* 061124
            if( numberOfDimensions==2 )
                maxInterpNew=2*(maskrLocal.getLength(0)+maskrLocal.getLength(1));  // pts on a diagonal
            else
                maxInterpNew=2*(maskrLocal.getLength(0)*maskrLocal.getLength(1)+
                                                maskrLocal.getLength(0)*maskrLocal.getLength(2)+
                                                maskrLocal.getLength(1)*maskrLocal.getLength(2));

      // Note: in parallel using cg.numberOfInterpolationPoints(bg) will generally give too large an estimate
      //       so we estimate from the size of maskrLocal
            maxInterpNew=min(maxInterpNew,
                   		       cg.numberOfInterpolationPoints(bg)*(max(rf[0],rf[1],rf[2])+numberOfDimensions-1))+100;
            ip.redim(maxInterpNew,3);

            int * ipp = ip.Array_Descriptor.Array_View_Pointer1;
            int ipDim0=ip.getRawDataSize(0);
#undef IP
#define IP(i0,i1) ipp[i0+ipDim0*(i1)]

      // *wdh* 981102 getIndex(cr.indexRange(),I1,I2,I3);
            getIndex(cr.extendedIndexRange(),I1,I2,I3);

      // build maskI(i1,i2,i3) : ==1 at an interpolation point.
            if( max(abs(cr.discretizationWidth()(Rx)-3))!=0 )
            {
      	printf("updateRefinement:ERROR:sorry, not implemented yet for this discretizationWidth\n");
      	cr.discretizationWidth().display("discretizationWidth");
      	throw "error";   
            }

            
            getIndex(cr.dimension(),J1,J2,J3);
#ifdef USE_PPP
            ok=ParallelUtility::getLocalArrayBounds(maskr,maskrLocal,J1,J2,J3);
#else
            ok=true;
#endif
            if( ok )
                maskrLocal(J1,J2,J3)=maskLocal(J1,J2,J3);   // copy back to the original mask.

      // **** do we need to update ghost boundaries on maskr ???? --> maybe not****

            if( debug & 4  )
            {
                displayMask(maskLocal,"************ maskLocal after marking holes ***************",plogFile);

                displayMask(maskrLocal,"************ maskrLocal after marking holes ***************",plogFile);
                fflush(plogFile);
                displayMask(maskr,"************ maskr after marking holes ***************",logFile);
                fflush(logFile);
            }
            
      // useOpt=false;  // ***wdh***

            int numberOfInterpolationPoints=0;
//        maskSave1.redim(0);
//        maskSave2.redim(0);
//        maskSave1=maskr;  // ***wdh***
            
            numberOfInterpolationPoints=0;  // reset ***wdh***
      	

      // ---> restrict I1,I2,I3 to local array with NO ghost points (avoid duplicate counting of interp pts)
            if( assignLocal )
            {

      	ok=ParallelUtility::getLocalArrayBounds(maskr,maskrLocal,I1,I2,I3);  // restrict bounds to local array
  
      	if( ok && numberOfDimensions==2 )
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3)
        	  {
          	    if( MASK(i1,i2,i3)>0 && 
            		(MASK(i1-1,i2-1,i3)==0 || MASK(i1,i2-1,i3)==0 || MASK(i1+1,i2-1,i3)==0 ||
             		 MASK(i1-1,i2  ,i3)==0 ||                        MASK(i1+1,i2  ,i3)==0 ||
             		 MASK(i1-1,i2+1,i3)==0 || MASK(i1,i2+1,i3)==0 || MASK(i1+1,i2+1,i3)==0 ) )
          	    {
            	      MASKR(i1,i2,i3)=numberOfInterpolationPoints+1; 
	      // make a list of overlapping grid style interpolation points
            	      for( axis=0; axis<3; axis++ )
            		IP(numberOfInterpolationPoints,axis)=iv[axis];

            	      numberOfInterpolationPoints++;
            	      if( numberOfInterpolationPoints>=maxInterpNew )
            	      {
            		maxInterpNew=int(max(maxInterpNew*1.25,maxInterpNew+1000));
            		ip.resize(maxInterpNew,3);
		// recompute pointers to ip
            		ipp = ip.Array_Descriptor.Array_View_Pointer1;
            		ipDim0=ip.getRawDataSize(0);

            	      }
          	    }
        	  }
      	}
      	else if( ok ) // 3D
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3)
        	  {
          	    if( MASK(i1,i2,i3)>0 &&
            		(MASK(i1-1,i2-1,i3-1)==0 || MASK(i1,i2-1,i3-1)==0 || MASK(i1+1,i2-1,i3-1)==0 ||
             		 MASK(i1-1,i2  ,i3-1)==0 || MASK(i1,i2  ,i3-1)==0 || MASK(i1+1,i2  ,i3-1)==0 ||
             		 MASK(i1-1,i2+1,i3-1)==0 || MASK(i1,i2+1,i3-1)==0 || MASK(i1+1,i2+1,i3-1)==0 ||

             		 MASK(i1-1,i2-1,i3  )==0 || MASK(i1,i2-1,i3  )==0 || MASK(i1+1,i2-1,i3  )==0 ||
             		 MASK(i1-1,i2  ,i3  )==0 ||                          MASK(i1+1,i2  ,i3  )==0 ||
             		 MASK(i1-1,i2+1,i3  )==0 || MASK(i1,i2+1,i3  )==0 || MASK(i1+1,i2+1,i3  )==0 ||

             		 MASK(i1-1,i2-1,i3+1)==0 || MASK(i1,i2-1,i3+1)==0 || MASK(i1+1,i2-1,i3+1)==0 ||
             		 MASK(i1-1,i2  ,i3+1)==0 || MASK(i1,i2  ,i3+1)==0 || MASK(i1+1,i2  ,i3+1)==0 ||
             		 MASK(i1-1,i2+1,i3+1)==0 || MASK(i1,i2+1,i3+1)==0 || MASK(i1+1,i2+1,i3+1)==0 ) )
          	    {
            	      MASKR(i1,i2,i3)=numberOfInterpolationPoints+1; 
	      // make a list of overlapping grid style interpolation points
            	      for( axis=0; axis<3; axis++ )
            		IP(numberOfInterpolationPoints,axis)=iv[axis];

            	      numberOfInterpolationPoints++;
            	      if( numberOfInterpolationPoints>=maxInterpNew )
            	      {
            		maxInterpNew=int(max(maxInterpNew*1.25,maxInterpNew+1000));
            		ip.resize(maxInterpNew,3);
		// recompute pointers to ip
            		ipp = ip.Array_Descriptor.Array_View_Pointer1;
            		ipDim0=ip.getRawDataSize(0);
            	      }
          	    }
        	  }
      	}
      	
            }  // end if assignLocal
            

      // *wdh* 060311 -- update ghost here so we can remove un-needed ---
            maskr.updateGhostBoundaries();


            if( debug & 4  )
            {
                displayMask(maskrLocal,"************ maskrLocal after marking interp pts (1) ***************",plogFile);
        // display(maskrLocal,"************ maskrLocal after marking interp pts (1) ***************",plogFile,"%4i");
                fflush(plogFile);
            }
            

//        if( max(abs(maskSave1-maskr))!=0 )
//        {
//  	printf("**** ERROR in marking interp ****\n");
              
//  	if( debug & 4 )
//  	{
//            fPrintF(logFile,"**** ERROR in setMaskAtAlignedHoles ****\n");
//  	  displayMask(maskSave1,"maskSave1 (old way)",logFile);
//  	  displayMask(maskr,"maskr (new way)",logFile);
//            maskSave2=abs(maskr-maskSave1);
//  	  displayMask(maskSave2,"diff",logFile);
//            maskr=maskSave1;
//  	}
//        }
//        else
//        {
//  	printf("**** PASSED marking interp ****\n");
//        }

//      useOpt=true;  // ***wdh***


      // throw away any unnecessary interpolation points.
            int dw[3];
            dw[0]=cr.discretizationWidth(axis1)/2;
            dw[1]=cr.discretizationWidth(axis2)/2;
            dw[2]=cr.discretizationWidth(axis3)/2;
            const int ISneeded = MappedGrid::ISinterpolationPoint | MappedGrid::ISdiscretizationPoint;
            i3=cr.indexRange(Start,axis3);
            iBase[2]=iBound[2]=i3;
            
            int i, ii=0;
      // opt version
      	
            const int irMin[3]={cr.indexRange(0,0),cr.indexRange(0,1),cr.indexRange(0,2)}; //
            const int irMax[3]={cr.indexRange(1,0),cr.indexRange(1,1),cr.indexRange(1,2)}; //
            if( numberOfDimensions==2 )
            {
      	for( i=0; i<numberOfInterpolationPoints; i++ )
      	{
        	  bool pointIsNeeded=MASKR(IP(i,0),IP(i,1),0) & ISneeded;
        	  if( !pointIsNeeded )
        	  {
          	    for( axis=0; axis<2; axis++ )
          	    {
            	      iBase[axis] =max(irMin[axis],IP(i,axis)-dw[axis]);
            	      iBound[axis]=min(irMax[axis],IP(i,axis)+dw[axis]);
          	    }
          	    const int s3=0;
          	    for( int s2=iBase[1]; s2<=iBound[1] && !pointIsNeeded; s2++ )
            	      for( int s1=iBase[0]; s1<=iBound[0]; s1++ )
            	      {
            		if( MASKR(s1,s2,s3) & ISneeded )
            		{
              		  pointIsNeeded=true;
              		  break;
            		}
            	      }
        	  }
          	    
        	  if( pointIsNeeded )
        	  {
          	    IP(ii,0)=IP(i,0);
          	    IP(ii,1)=IP(i,1);
          	    ii++;
        	  }
        	  else
        	  {
	    // printf(" ***** throw away an unneeded point\n");
          	    MASKR(IP(i,0),IP(i,1),0)=0;
        	  }
      	}
            }
            else
            {
      	for( i=0; i<numberOfInterpolationPoints; i++ )
      	{
        	  bool pointIsNeeded=MASKR(IP(i,0),IP(i,1),IP(i,2)) & ISneeded;
        	  if( !pointIsNeeded )
        	  {
          	    for( axis=0; axis<3; axis++ )
          	    {
            	      iBase[axis] =max(irMin[axis],IP(i,axis)-dw[axis]);
            	      iBound[axis]=min(irMax[axis],IP(i,axis)+dw[axis]);
          	    }
          	    for( int s3=iBase[2]; s3<=iBound[2] && !pointIsNeeded; s3++ )
            	      for( int s2=iBase[1]; s2<=iBound[1] && !pointIsNeeded; s2++ )
            		for( int s1=iBase[0]; s1<=iBound[0]; s1++ )
            		{
              		  if( MASKR(s1,s2,s3) & ISneeded )
              		  {
                		    pointIsNeeded=TRUE;
                		    break;
              		  }
            		}
        	  }
          	    
        	  if( pointIsNeeded )
        	  {
          	    IP(ii,0)=IP(i,0);
          	    IP(ii,1)=IP(i,1);
          	    IP(ii,2)=IP(i,2);
          	    ii++;
        	  }
        	  else
        	  {
	    // printf(" ***** throw away an unneeded point\n");
          	    MASKR(IP(i,0),IP(i,1),IP(i,2))=0;
        	  }
      	}
            }
      	
            if( debug & 4  )
            {
                displayMask(maskrLocal,"******** maskrLocal after throw away un-needed interp pts **********",plogFile);
                fflush(plogFile);
            }      
//      useOpt=true;  // ***wdh***

            
            if( numberOfInterpolationPoints!=ii && assignLocal )
            {
                if( debug & 4 ) 
                    fprintf(plogFile,"** fix up after throw away un-needed, numberOfInterpolationPoints=%i ii=%i\n",
              		  numberOfInterpolationPoints,ii);
      	
        // Some un-necessary points were removed.
	// Make sure ghost line values are marked properly. This is needed if we
	// removed un-necessary interpolation points
      	for( axis=0; axis<numberOfDimensions; axis++ )
      	{
        	  for( int side=Start; side<=End; side++ )
        	  {
          	    getBoundaryIndex(cr.dimension(),side,axis,I1,I2,I3);
          	    Iv[axis]=cr.indexRange(side,axis);

                        ok=ParallelUtility::getLocalArrayBounds(maskr,maskrLocal,I1,I2,I3);
            	    if( !ok ) continue;
                        J1=I1; J2=I2; J3=I3;

//             // check that this ghost line is on this processor:
//             bool ok = Iv[axis].getBase()>=dimensionr(0,axis) && Iv[axis].getBase()<=dimensionr(1,axis);
// 	    if( !ok ) continue;
// 	    getBoundaryIndex(cr.dimension(),side,axis,J1,J2,J3);

          	    for( int ghost=1; ghost<=cr.numberOfGhostPoints(side,axis); ghost++ )
          	    {
            	      Jv[axis]=cr.indexRange(side,axis)-ghost*(1-2*side);

              // make sure this ghost line is still on this processor -- this should always be true?
                            bool ok = Jv[axis].getBase()>=maskrLocal.getBase(axis) && 
            		Jv[axis].getBase()<=maskrLocal.getBound(axis); 
                	      if( !ok ) continue;

            	      where( maskrLocal(I1,I2,I3)==0 )
            	      {
            		maskrLocal(J1,J2,J3)=0;
            	      }
          	    }
        	  }
      	}
            }
            numberOfInterpolationPoints=ii;
            if( numberOfInterpolationPoints>0 )
            {
      	if( numberOfDimensions==2 )
        	  for( i=0; i<numberOfInterpolationPoints; i++ )
          	    MASKR(IP(i,0),IP(i,1),i3)=-(i+1);
      	else    
        	  for( i=0; i<numberOfInterpolationPoints; i++ )
          	    MASKR(IP(i,0),IP(i,1),IP(i,2))=-(i+1);
            }


      // This is the local number on this processor:
            numberOfInterpolationPointsLocal(grid)=numberOfInterpolationPoints;

      // *** 061124 -- resize ip to eliminate wasted space ---
            ip.resize(numberOfInterpolationPoints,3);
            ipp = ip.Array_Descriptor.Array_View_Pointer1;
            ipDim0=ip.getRawDataSize(0);
            if( Ogen::debug & 4 )
            {
                displayMask(maskrLocal,"*********updateRefinement: maskrLocal after marking interpolation *************",
                                        plogFile);
                displayMask(maskr,"*********updateRefinement: maskr after marking interpolation *************",
                                        logFile);
            }
            
            if( debug & 4 )
            {
                fprintf(plogFile,"myid=%i, number of interpolation points on grid=%i is %i \n",
                                myid,grid,numberOfInterpolationPoints);
      	for( i=0; i<numberOfInterpolationPoints; i++ )
      	{
                    fprintf(plogFile,"ip(%i)=(%i,%i,%i), ",i,IP(i,0),IP(i,1),(numberOfDimensions==2 ? 0 : IP(i,2)));
        	  if( i % 5 == 4 ) fprintf(plogFile,"\n");
      	}
      	fprintf(plogFile,"\n");

	// Send all ip points to processor 0 to be output:

                Index Iv[2];
      	Iv[0]=Range(numberOfInterpolationPoints);
      	Iv[1]=Range(numberOfDimensions);
                intSerialArray ipp;
                const int p0=0;
                CopyArray::getAggregateArray( ip,Iv, ipp,p0 ); 

                if( myid==p0 )
      	{
        	  int numInterp=ipp.getLength(0);
        	  fPrintF(logFile,"total number of interpolation points on grid=%i is %i (np=%i)\n",
              		  grid,numInterp,np); 
        	  for( i=0; i<numInterp; i++ )
        	  {
          	    fPrintF(logFile,"ip(%i)=(%i,%i,%i), ",i,ipp(i,0),ipp(i,1),(numberOfDimensions==2 ? 0 : ipp(i,2)));
          	    if( i % 5 == 4 ) fPrintF(logFile,"\n");
        	  }
                    fPrintF(logFile,"\n");
      	}

            }

//      useOpt=false;  // ***wdh***

            timeForMarkInterp+=getCPU()-timei;

        } // end for g

    } // end for (level)
    

    real timeForMarkMask=getCPU()-time1;







  // printf(" cg.interpoleeGrid[0]: min=%i, max=%i (before update)\n",min(cg.interpoleeGrid[0]),max(cg.interpoleeGrid[0]));

  // For now we build the interpolation arrays (to be the correct length)
    if( cg.numberOfComponentGrids() > cg.numberOfBaseGrids() )
    {
    // pretend that all refinement grids have zero interp points so the arrays are zero length
        Range R(cg.numberOfBaseGrids(),cg.numberOfComponentGrids()-1);
        cg.numberOfInterpolationPoints(R)=0; 
    }
    
    cg.update(
        CompositeGrid::THEinterpolationPoint       |
        CompositeGrid::THEinterpoleeGrid           |
        CompositeGrid::THEinterpoleeLocation       |
        CompositeGrid::THEinterpolationCoordinates, 
        CompositeGrid::COMPUTEnothing);



    UpdateRefinementData urd; // this object holds data for passing around to other functions

    real & timeForInterpData = urd.timeForInterpData;
    real & timeForCopyInterpPoints = urd.timeForCopyInterpPoints;  

  // ********************************************
  // **** Now fill in the interpolation data ****
  // ********************************************
    if( false )
    {
        updateRefinementFillInterpolationData(cg,urd);
    }
    else
    {
        updateRefinementFillInterpolationDataNew(cg,urd);
    }
    


  // ---------------------
//   if( true )
//   {
//     Overture::abort("finished for now");
//   }


//   if( debug & 4 )
//   {
//     for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//     {
//       display(cg.interpolationPoint[grid],sPrintF(buff,"cg.interpolationPoint[%i]",grid),logFile);
//       display(cg.interpoleeGrid[grid],sPrintF(buff,"cg.interpoleeGrid[%i]",grid),logFile);
//       display(cg.variableInterpolationWidth[grid],sPrintF(buff,"cg.variableInterpolationWidth[%i]",grid),logFile);
//       display(cg.interpolationCoordinates[grid],sPrintF(buff,"cg.interpolationCoordinates[%i]",grid),logFile);
//       displayMask(cg[grid].mask(),sPrintF(buff,"cg[%i].mask",grid),logFile);
//     }
//   }



  //  *wdh* 000424 Tell the CompositeGrid that the interpolation data have been computed:
    cg->computedGeometry |=
        CompositeGrid::THEmask                     |
        CompositeGrid::THEinterpolationCoordinates |
        CompositeGrid::THEinterpolationPoint       |
        CompositeGrid::THEinterpoleeLocation       |
        CompositeGrid::THEinterpoleeGrid;
  // ** CompositeGrid::THEmultigridLevel;  // *wdh*

  // assign mask values at points hidden by finer patches
    cg.setMaskAtRefinements();
    

  // double check that we have computed the interpolation points on refinements
  // properly.


    if( debug & 16 )
    {
        for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
        {
            displayMask(cg[grid].mask(),sPrintF(buff,"cg[%i].mask",grid),logFile);
        }
    }

//   if( debug & 2 )
//   {
//     for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//       for( int grid2=0; grid2<cg.numberOfComponentGrids(); grid2++ )
// 	fprintf(plogFile,"end: cg.interpolationStartEndIndex(0:1,grid=%i,grid2=%i)=%i %i \n",grid,grid2,
// 		cg.interpolationStartEndIndex(0,grid,grid2),
// 		cg.interpolationStartEndIndex(1,grid,grid2));
//   }

    if( false && debug & 2  )
    {
    // **** fix this for parallel ***

        printf("check interpolation on refinements...\n");
        int numberOfErrors=checkRefinementInterpolation( cg );
        if( numberOfErrors==0 )
        {
            printf("...no errors found\n");
        }
    }
    
  // for testing: 
    if( false )
    {
        printf("I will save a file `updateRefinementDebug.cmd' that can be used with the `refine' test\n"
         	   " program in order to regenerate the adaptive grid and test it.\n");
        outputRefinementInfoNew( cg, "bugGrid.hdf","updateRefinementDebug.cmd" );

        fclose(logFile);
        throw "error";
    }

    Overture::checkMemoryUsage("Ogen::updateRefinementNewer (end)");  

    real totalTime=getCPU()-timeStart;

    totalTime         =ParallelUtility::getMaxValue(totalTime);
    timeForUpdate     =ParallelUtility::getMaxValue(timeForUpdate);
    timeForMarkMask   =ParallelUtility::getMaxValue(timeForMarkMask);
    timeForMarkOffAxis=ParallelUtility::getMaxValue(timeForMarkOffAxis);
    timeForMarkInterp =ParallelUtility::getMaxValue(timeForMarkInterp);
    timeForInterpData =ParallelUtility::getMaxValue(timeForInterpData);
    timeForCopyInterpPoints=ParallelUtility::getMaxValue(timeForCopyInterpPoints);
    
    if( debug & 1 && myid==0 )
    {
        printf("updateRefinement: total cpu=%8.2e, update=%5.2f%% mark-mask=%5.2f%% (off-axis=%5.2f%%, interp=%5.2f%%) "
         	   "interp-update=%5.2f%% (copy ip=%5.2f%%)\n",totalTime,
         	   100.*timeForUpdate/totalTime,
         	   100.*timeForMarkMask/totalTime,
         	   100.*timeForMarkOffAxis/totalTime,
         	   100.*timeForMarkInterp/totalTime,
         	   100.*timeForInterpData/totalTime,
                      100.*timeForCopyInterpPoints/totalTime);
    }
    
    return 0;
}



