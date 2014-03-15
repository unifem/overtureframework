// This file automatically generated from updateRefinementNew.bC with bpp.
#include "Overture.h"
#include "Ogen.h"
#include "display.h"
#include "HDF_DataBase.h"
#include "ParallelUtility.h"
#include "CanInterpolate.h"

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

// ** add this to ParallelUtility **

// REDISTRIBUTE_ARRAY(intArray)
int redistribute(const intArray & u, intArray & v, const Range & P)
// /Description:
//    Build v, a copy of the array u that lives on the processors defined by the Range P
//  NOTE: P must be the same for all processors.
{
  // build a Partition that lives on this processor
    Partitioning_Type partition; 
    partition.SpecifyProcessorRange(P); 
    if( u.getInternalPartitionPointer()!=NULL )
    {
        Partitioning_Type uPartition=u.getPartition();
        for( int axis=0; axis<MAX_ARRAY_DIMENSION; axis++ )
        {
            int ghost=uPartition.getGhostBoundaryWidth(axis);
            if( ghost>0 )
      	partition.partitionAlongAxis(axis, true , ghost);
            else
      	partition.partitionAlongAxis(axis, false, 0);
        }
    }
    v.partition(partition);   
    v.redim(u.dimension(0),u.dimension(1),u.dimension(2));
    v = u; // copy mask to this processor
    return 0;
}

// ** add this to ParallelUtility **

// REDISTRIBUTE_ARRAY(intArray,intSerialArray)
int redistribute( const intArray & u, intSerialArray & v )
// /Description:
//    Build v, a copy of the array u that lives on the local processor
{
    const int np=max(1,Communication_Manager::Number_Of_Processors);
    Index Iv[4]={u.dimension(0),u.dimension(1),u.dimension(2),u.dimension(3)};
  // every processor gets a copy of the entire array:
    IndexBox *vBox = new IndexBox [np];
    for( int p=0; p<np; p++ )
    {
        vBox[p].setBounds(u.getBase(0),u.getBound(0),
                                            u.getBase(1),u.getBound(1),
                                            u.getBase(2),u.getBound(2),
                                            u.getBase(3),u.getBound(3) );
    }
    v.redim(u.dimension(0),u.dimension(1),u.dimension(2),u.dimension(3));
    CopyArray::copyArray( u,Iv,vBox,v ); 
    delete [] vBox;
    return 0;
}

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



//\begin{>>RegridInclude.tex}{\subsection{outputRefinementInfo}} 
int 
outputRefinementInfoNew( GridCollection & gc, 
                                            const aString & gridFileName, 
                                              const aString & fileName )
// =======================================================================================
// /Description:
//   This function will output a command file for the "refine" test code.
// /gc(input) : name of the grid.
// /refinementRatio (input) : refinement ratio.
// /gridFileName (input) : grid file name, such as "cic.hdf". This is not essential,
//    but then you will have to edit the comamnd file to add the correct name.
// /fileName (input) : name of the output command file, such as "bug.cmd"
// The output will be a file of the form
// \begin{verbatim}
// choose a grid
//   cic.hdf
// add a refinement
//   0 1 4 10 12 15
// add a refinement
//   0 1 3 10 15 19
// add a refinement
//   1 1 12 16 0 7
// add a refinement
//   1 1 16 20 3 7
// \end{verbatim}
//\end{RegridInclude.tex} 
// ========================================================================================
{
    printf("*** outputing a command file %s for refine ****\n",(const char*)fileName);
    
    FILE *file=fopen(fileName,"w");
    fprintf(file,"choose a grid\n"
        	  " %s \n",(const char*)gridFileName);
    for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
    {
        if( gc.refinementLevelNumber(grid)>0 )
        {
      //  Find a parent grid at the same multigrid level.
            int level=gc.refinementLevelNumber(grid);
            int p;
            for (p=0; gc.refinementLevelNumber(p)!=level-1 || gc.baseGridNumber(p)!=gc.baseGridNumber(grid); p++);

            int refinementRatio[3]={1,1,1};
            for( int axis=0; axis<gc.numberOfDimensions(); axis++ )
            {
      	refinementRatio[axis]=gc.refinementFactor(axis,grid)/gc.refinementFactor(axis,p);
                assert( refinementRatio[axis]==2 || refinementRatio[axis]==4 );
            }
            
            MappedGrid & mg = gc[grid];
            fprintf(file,"add a refinement\n"
                            " %i %i  %i %i %i %i %i %i %i\n",gc.baseGridNumber(grid),gc.refinementLevelNumber(grid),
                            mg.gridIndexRange(0,0)/refinementRatio[0],mg.gridIndexRange(1,0)/refinementRatio[0],
                            mg.gridIndexRange(0,1)/refinementRatio[1],mg.gridIndexRange(1,1)/refinementRatio[1],
                            mg.gridIndexRange(0,2)/refinementRatio[2],mg.gridIndexRange(1,2)/refinementRatio[2],
                          refinementRatio[0] );
        }
    }
    fclose(file);

    bool saveTheGrid=true;
    if( saveTheGrid )
    {
    // save the GridCollection (CompositeGrid) consisting of the base level grids
    // This may be necessary for moving grids if we want know the grid when the error occured.

        if( gc.getClassName()=="CompositeGrid" )
        {
            CompositeGrid & cg = (CompositeGrid &)gc;
            
            CompositeGrid c; // this will hold the base grids
        
            c = cg;  // make a copy

      // delete all refinement grids
            c.deleteRefinementLevels();
            c.updateReferences();
            
      //  *wdh* Tell the CompositeGrid that the interpolation data have been computed:
            c->computedGeometry |=
      	CompositeGrid::THEmask                     |
      	CompositeGrid::THEinterpolationCoordinates |
      	CompositeGrid::THEinterpolationPoint       |
      	CompositeGrid::THEinterpoleeLocation       |
      	CompositeGrid::THEinterpoleeGrid;

      // if( c.numberOfInterpolationPoints(0)>0 )
      // {
      //   printf(" ***** c.numberOfInterpolationPoints(0)=%i\n",c.numberOfInterpolationPoints(0));
	// c.interpolationPoint[0].display("c.interpolationPoint[0]");
      // }
            


            printf("Saving the current CompositeGrid in %s\n",(const char*)gridFileName);

            HDF_DataBase dataFile;
            dataFile.mount(gridFileName,"I");

            int streamMode=1; // save in compressed form.
            dataFile.put(streamMode,"streamMode");
            if( !streamMode )
      	dataFile.setMode(GenericDataBase::noStreamMode); // this is now the default
            else
            {
      	dataFile.setMode(GenericDataBase::normalMode); // need to reset if in noStreamMode
            }
                      
            if( c.numberOfGrids() > 1 || c.numberOfInterpolationPoints(0)>0 )
      	c.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask );
            else
      	c.destroy(CompositeGrid::EVERYTHING);
            

            const aString gridName="bugGrid";
            c.put(dataFile,gridName);
            dataFile.unmount();
            
      // -------------
            if( false )
            {
      	CompositeGrid cg2;
      	dataFile.mount(gridFileName,"R");
      	cg2.get(dataFile,gridName);
            
	// printf(" ***** cg2.numberOfInterpolationPoints(0)=%i\n",cg2.numberOfInterpolationPoints(0));
	// cg2.interpolationPoint[0].display("cg2.interpolationPoint[0]");
            }
            
      // ----------------
        }
        
        


    }
    


    return 0;
}

//\begin{>>ogenUpdateInclude.tex}{\subsubsection{updateRefinement : Adapative Grid updateOverlap}}
int Ogen::
updateRefinementNew(CompositeGrid & cg, 
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

    using namespace CanInterpolate;

//  debug=1;
//  debug=7;  // *************************

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

    realArray rr(1,3),xx(1,3),rb(1,3); rr=-1.; rb=-1.;
    RealArray rrs(1,3), xxs(1,3),rbs(1,3); rrs=-1.; rbs=-1.;
    
    intSerialArray interpolates(1), useBackupRules(1);
    useBackupRules=FALSE;
    const int notAssigned = INT_MIN;
    const int mgLevel=0;  // *** multigrid level
    
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
          	    printf("updateRefinementNew:ERROR local maskb array does not match maskr. myid=%i, \n"
               		   "  Ivb=[%i,%i][%i,%i][%i,%i]  maskbLocal=[%i,%i][%i,%i][%i,%i]\n",
               		   myid,
               		   Ivb[0].getBase(),Ivb[0].getBound(),
               		   Ivb[1].getBase(),Ivb[1].getBound(),
               		   Ivb[2].getBase(),Ivb[2].getBound(),
               		   maskbLocal.getBase(0),maskbLocal.getBound(0),
               		   maskbLocal.getBase(1),maskbLocal.getBound(1),
               		   maskbLocal.getBase(2),maskbLocal.getBound(2));

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
//  	    fprintf(logFile,"**** ERROR in setMaskAtAlignedHoles ****\n");
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

            		const int axisp1 = (axis+1) % numberOfDimensions;

            		Iv[axis] = cr.indexRange(side,axis)-ghost*(1-2*side);

		// restrict bounds to this processor:
            		bool ok=ParallelUtility::getLocalArrayBounds(maskr,maskrLocal,I1,I2,I3);
            		if( !ok ) continue;  // no pts on this processor

//               // check that this ghost line is on this processor:
//               bool ok = Iv[axis].getBase()>=dimensionr(0,axis) && Iv[axis].getBase()<=dimensionr(1,axis);
// 	      if( !ok ) continue;
            	      
		// displayMask(mask(I1,I2,I3),"mask(I1,I2,I3) (ghost line)");

#define moda(n,m) (n)>0 ? (n)%(m) : ((m)-(n))%(m)



                            Iv[axisp1]=Range(Iv[axisp1].getBase()-rrf[axisp1],Iv[axisp1].getBound(),rrf[axisp1]);
#ifdef USE_PPP
            // 	      setLocalBoundsWithStride(Iv[axisp1],axisp1,rrf[axisp1],ok);
                                    	      {
            		// /Description:
            		// Adjust the Index Iv[axisp1] with a stride rrf[axisp1] to fit on this processor.
            		// This shift should work since the mask array has been enlarged to account for the refinement rrf[axisp1]
            		// 
            		// /Iv[axisp1] (input/output) : if ok==true on output then Iv[axisp1] is the adjusted index
            		// /ok (output) : return true if there is an non-empty Index Iv[axisp1]
                                    		int na=Iv[axisp1].getBase();
                                    		int nb=Iv[axisp1].getBound();
                                    		int ma=maskLocal.getBase(axisp1);
                                    		int mb=maskLocal.getBound(axisp1);
                                    		ok=true;
                                    		if( na<ma )
                                    		{ // Shift na by a multiple of rrf[axisp1] so that it starts on this processor
                                      		  na += ( (ma-na +rrf[axisp1]-1)/rrf[axisp1] )*rrf[axisp1];
                                      		  ok=false;
                                    		}
                                    		if( nb>mb )
                                    		{ // Shift nb by a multiple of rrf[axisp1] so that it ends on this processor
                                      		  nb -= ( (nb-mb +rrf[axisp1]-1)/rrf[axisp1] )*rrf[axisp1];
                                      		  ok=false;
                                    		}	      
                                    		if( !ok )
                                    		{ // bounds were adjusted
                                      		  ok = nb>=na; 
                                      		  if( ok )
                                        		    Iv[axisp1]=Range(na,nb,rrf[axisp1]);
                                    		}
                                    	      }
            	      if( !ok ) continue;
#endif


	      // displayMask(mask(I1,I2,I3),"mask(I1,I2,I3) (every 4th pt)");

            	      if( numberOfDimensions==2 )
            	      {
                // Mark intermediate points on ghost lines when exactly one coarse grid point is an interior point
                //         ----X-+-+-+-0----
            		if( axis==0 )
            		{
                                    Range I2m(I2.getBase(),I2.getBound()-rrf[1],rrf[1]);
              		  FOR_3S(i1,i2,i3,I1,I2m,I3)  // *wdh* 061103 -- fixed loop bounds
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
                                    Range I1m(I1.getBase(),I1.getBound()-rrf[0],rrf[0]);
              		  FOR_3S(i1,i2,i3,I1m,I2,I3)  // *wdh* 061103 -- fixed loop bounds
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
                                Iv[axisp2]=Range(Iv[axisp2].getBase(),Iv[axisp2].getBound()-rrf[axisp2],rrf[axisp2]);
#ifdef USE_PPP
              // 	        setLocalBoundsWithStride(Iv[axisp2],axisp2,rrf[axisp2],ok);
                                        	      {
              		// /Description:
              		// Adjust the Index Iv[axisp2] with a stride rrf[axisp2] to fit on this processor.
              		// This shift should work since the mask array has been enlarged to account for the refinement rrf[axisp2]
              		// 
              		// /Iv[axisp2] (input/output) : if ok==true on output then Iv[axisp2] is the adjusted index
              		// /ok (output) : return true if there is an non-empty Index Iv[axisp2]
                                        		int na=Iv[axisp2].getBase();
                                        		int nb=Iv[axisp2].getBound();
                                        		int ma=maskLocal.getBase(axisp2);
                                        		int mb=maskLocal.getBound(axisp2);
                                        		ok=true;
                                        		if( na<ma )
                                        		{ // Shift na by a multiple of rrf[axisp2] so that it starts on this processor
                                          		  na += ( (ma-na +rrf[axisp2]-1)/rrf[axisp2] )*rrf[axisp2];
                                          		  ok=false;
                                        		}
                                        		if( nb>mb )
                                        		{ // Shift nb by a multiple of rrf[axisp2] so that it ends on this processor
                                          		  nb -= ( (nb-mb +rrf[axisp2]-1)/rrf[axisp2] )*rrf[axisp2];
                                          		  ok=false;
                                        		}	      
                                        		if( !ok )
                                        		{ // bounds were adjusted
                                          		  ok = nb>=na; 
                                          		  if( ok )
                                            		    Iv[axisp2]=Range(na,nb,rrf[axisp2]);
                                        		}
                                        	      }
              	        if( !ok ) continue;
#endif

                // Mark intermediate points on ghost lines when exactly one coarse grid point is an interior point
                // check the 4 corners of the face.

            		Range I1m(I1.getBase(),I1.getBound()-rrf[0],rrf[0]);
            		Range I2m(I2.getBase(),I2.getBound()-rrf[1],rrf[1]);
            		Range I3m(I3.getBase(),I3.getBound()-rrf[2],rrf[2]);
            		if( axis==0 )
            		{
              		  FOR_3S(i1,i2,i3,I1,I2m,I3m) // *wdh* 061103 -- fixed loop bounds
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
              		  FOR_3S(i1,i2,i3,I1m,I2,I3m) // *wdh* 061103 -- fixed loop bounds
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
              		  FOR_3S(i1,i2,i3,I1m,I2m,I3) // *wdh* 061103 -- fixed loop bounds
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
            		ParallelUtility::getLocalArrayBounds(maskr,maskrLocal,J1,J2,J3);
#endif
		// special case if refinement aligns with the extendedIndexRange of a base grid
		// interpolation side.
            		if( true )
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
            		else
            		{
              		  where( maskLocal(I1,I2,I3)==0 )
              		  {
                		    maskLocal(J1,J2,J3)=0;
              		  }
              		  elsewhere( maskLocal(J1,J2,J3)!=0 )  // *wdh* 981017
              		  {
                		    maskLocal(J1,J2,J3)=MappedGrid::ISghostPoint;
              		  }
            		}
            	      
            	      }
          	    }
        	  }
      	}  // end for axis
      	
            } // end if assignLocal
            
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
//            fprintf(logFile,"**** ERROR in setMaskAtAlignedHoles ****\n");
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

            int maxInterpNew=cg.numberOfInterpolationPoints(bg)*(max(rf[0],rf[1],rf[2])+numberOfDimensions-1)+100;
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
      	else if( ok )
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
//            fprintf(logFile,"**** ERROR in setMaskAtAlignedHoles ****\n");
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
                    fprintf(plogFile,"ip(%i)=(%i,%i,%i), ",i,IP(i,0),IP(i,1),(numberOfDimensions==2 ? 0. : IP(i,2)));
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
        	  fprintf(logFile,"total number of interpolation points on grid=%i is %i (np=%i)\n",
              		  grid,numInterp,np); 
        	  for( i=0; i<numInterp; i++ )
        	  {
          	    fprintf(logFile,"ip(%i)=(%i,%i,%i), ",i,ipp(i,0),ipp(i,1),(numberOfDimensions==2 ? 0. : ipp(i,2)));
          	    if( i % 5 == 4 ) fprintf(logFile,"\n");
        	  }
                    fprintf(logFile,"\n");
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



    real timeForInterpData=getCPU();
    real timeForCopyInterpPoints=0.;
    
  // ********************************************
  // **** Now fill in the interpolation data ****
  // ********************************************
    int pShift[3]={0,0,0};

//   IntegerArray baseGridMarked(cg.numberOfBaseGrids());  // not used ??
//   baseGridMarked=false;

  // Dimension the interpolationStartEndIndex array and copy values from base grids (which do not change)
    IntegerArray ise; ise=cg.interpolationStartEndIndex;
  // This next is wrong -- will break a reference with the rcData in the GC
  //  cg.interpolationStartEndIndex.redim(4,cg.numberOfComponentGrids(),cg.numberOfComponentGrids()); // is this needed?
    cg.interpolationStartEndIndex=-1;
    Range B = cg.numberOfBaseGrids();
    cg.interpolationStartEndIndex(all,B,B)=ise(all,B,B);

  // localMaskCopy[g] : build a copy of the mask array on grid g for use with canInterpolate.
  //                    This should be fixed so that we don't need to make this copy. 
    intSerialArray *localMaskCopy = new intSerialArray [cg.numberOfComponentGrids()];
    for( grid=0; grid<cg.numberOfBaseGrids(); grid++ )
    {
    // localMaskCopy[grid] will be a copy on this processor of the entire mask
        redistribute(cg[grid].mask(),localMaskCopy[grid]);
    }

        

    int iv0[3]={0,0,0};
    real dx[3]={0.,0.,0.},xab[2][3]={0.,0.,0.,0.,0.,0.};
#define VERTEX0(i0,i1,i2) xab[0][0]+dx[0]*(i0-iv0[0])
#define VERTEX1(i0,i1,i2) xab[0][1]+dx[1]*(i1-iv0[1])
#define VERTEX2(i0,i1,i2) xab[0][2]+dx[2]*(i2-iv0[2])


    for( l=1; l<cg.numberOfRefinementLevels(); l++ )
    { // Begin levels =1,...

        GridCollection & rl = cg.refinementLevel[l];


    // *** Stage I 
    //       o make a local copy of the interpolation data for this base grid
    //       o fill in local base grid mask with interpolation point numbers

        if( debug & 4 )
        {
            if( myid==0 ) fprintf(logFile,"\n **** Level l=%i : STAGE I ****\n",l);
            fprintf(plogFile,"\n **** Level l=%i : STAGE I ****\n",l);
        }

    // Allocate mask arrays for the portions of the base grid that lie 
    // underneath the refinement grid (local portion there-of)
        intSerialArray *maskBaseGrid = new intSerialArray [rl.numberOfComponentGrids()];  // delete these when done

    // Arrays to hold a local copy of the interpolation data for this base grid
        intSerialArray *ipBaseGridA = new intSerialArray [rl.numberOfComponentGrids()]; // delete these 
        intSerialArray *interpoleeGridBaseGridA= new intSerialArray [rl.numberOfComponentGrids()];
        realSerialArray *interpolationCoordinatesBaseGridA= new realSerialArray [rl.numberOfComponentGrids()];

    // number of local base grid interpolation points for each refinement grid:
        int *niBaseGrid = new int [rl.numberOfComponentGrids()]; // delete this
        int g;
        for( g=0; g<rl.numberOfComponentGrids(); g++ )
        {
            niBaseGrid[g]=0;
        }
        
        for( g=0; g<rl.numberOfComponentGrids(); g++ )
        {  // Begin grids on this level g=0,...


            int grid =rl.gridNumber(g);           // index into cg
            int bg = cg.baseGridNumber(grid);     // base grid for this refinement
            MappedGrid & cr = rl[g];              // refined grid
            MappedGrid & cb = cg[bg];             // base grid
            const intArray & maskb = cb.mask();
            intArray & maskr = cr.mask();
            
            #ifdef USE_PPP
              intSerialArray maskrLocal; getLocalArrayWithGhostBoundaries(maskr,maskrLocal);
            #else
              intSerialArray & maskrLocal=maskr; 
            #endif
            

      // Make a local copy of the interpolation data for this base grid
            intArray & ipBG = cg.interpolationPoint[bg];
            intArray & interpoleeGridBG = cg.interpoleeGrid[bg];
            realArray & interpolationCoordinatesBG = cg.interpolationCoordinates[bg];

            const int niBG=cg.numberOfInterpolationPoints(bg);

            if( niBG==0 ) continue;
            
            intSerialArray & ipBaseGrid = ipBaseGridA[g];
            intSerialArray & interpoleeGridBaseGrid = interpoleeGridBaseGridA[g];
            realSerialArray & interpolationCoordinatesBaseGrid = interpolationCoordinatesBaseGridA[g];

            ipBaseGrid.redim(niBG,numberOfDimensions);
            interpoleeGridBaseGrid.redim(niBG);
            interpolationCoordinatesBaseGrid.redim(niBG,numberOfDimensions);

            int rf[3];  // refinement factors (to the BASE GRID!)
            rf[0]=rl.refinementFactor(0,g);
            rf[1]=rl.refinementFactor(1,g);
            rf[2]=rl.refinementFactor(2,g);

            intSerialArray & maskbLocal = maskBaseGrid[g];

      // We need a copy of the base grid mask points that lie underneath this refinement grid
      //    maskbLocal : the portion of maskb that unlies maskrLocal (i.e. using the partition of maskr)
            int ghost[4]={0,0,0,0}; //
            for( axis=0; axis<numberOfDimensions; axis++ )
      	ghost[axis]=1;  // one ghost pt on the coarse grid will correspond to rrf points on the refinement grid
      	
            getIndex(cr.extendedIndexRange(),I1,I2,I3);
            Iv[3]=0;
            CopyArray::copyCoarseToFine( maskb, maskr, Iv, maskbLocal, rf, ghost);

            bool isPeriodic[3]={false,false,false}; //
            int ndr[3]={0,0,0};  // period in index space
            for( dir=0; dir<numberOfDimensions; dir++ )
            {
      	isPeriodic[dir]=  cb.isPeriodic(dir)==Mapping::functionPeriodic;
      	ndr[dir]=cb.gridIndexRange(1,dir)-cb.gridIndexRange(0,dir);
            }
            
      // this next loop uses scalar indexing communication  -- this can be improved ---
            real timeCopyIp=getCPU();
            iv[2]=cb.indexRange(Start,axis3);
            int ii=0;
            for( int i=0; i<niBG; i++ )
            {
        // make a copy of the base grid interpolation data that sits under this refinement grid:
                bool ok=true;
      	for( int dir=0; dir<numberOfDimensions; dir++ )
      	{
        	  iv[dir]=ipBG(i,dir);  // communication here -- 

          // int ivr = iv[dir]*rf[dir]; // index in refinement index space
	  // if( ivr<maskrLocal.getBase(dir) || ivr>maskrLocal.getBound(dir) ) // *** use local maskb bounds ***
                    if( iv[dir]<maskbLocal.getBase(dir) || iv[dir]>maskbLocal.getBound(dir) )
        	  { 
            // base grid interpolation pt is outside this refinement -- ignore it if the periodic image is also outside
                        if( isPeriodic[dir] )
          	    {
              // is the periodic image of an interp in this local array?
                            if( iv[dir]<maskbLocal.getBase(dir) &&
                                    iv[dir]+ndr[dir] <= maskbLocal.getBound(dir)  )
            	      { // periodic image is inside
                                iv[dir]+=ndr[dir];
                                assert( iv[dir] >= maskbLocal.getBase(dir) );
            	      }
            	      else if( iv[dir]>maskbLocal.getBound(dir) &&
                                              iv[dir]-ndr[dir]>=maskbLocal.getBase(dir) )
            	      {// periodic image is inside
                                iv[dir]-=ndr[dir];
                                assert( iv[dir] <= maskbLocal.getBound(dir) );
            	      }
            	      else
            	      {
                                ok=false;
            	      }
          	    }
          	    else
          	    {
            	      ok=false;
          	    }
          	    
	    // ** break;
        	  }
      	}
	// ** if( ok )
      	{
          // all processors must do the same P++ array op's
        	  assert( ii<=interpoleeGridBaseGrid.getBound(0) );
        	  
                    interpoleeGridBaseGrid(ii)=interpoleeGridBG(i);        // communication here --
                    assert( interpoleeGridBaseGrid(ii)>=0 && interpoleeGridBaseGrid(ii)<cg.numberOfBaseGrids() );
        	  
        	  for( int dir=0; dir<numberOfDimensions; dir++ )
        	  {
          	    ipBaseGrid(ii,dir)=iv[dir];
                        interpolationCoordinatesBaseGrid(ii,dir)=interpolationCoordinatesBG(i,dir); // and here ---
        	  }
                    if( ok ){ ii++; }
        	  
      	}
            }
            timeForCopyInterpPoints+=getCPU()-timeCopyIp;
            
            niBaseGrid[g]=ii;  // number of base grid interpolation points under this local refinement grid
            

            if( debug & 4 )
            {
                fprintf(plogFile,"myid=%i, l=%i g=%i grid=%i bg=%i: ni=%i niBaseGrid[g=%i]=%i (local base grid interp pts)\n",
                                myid,l,g,grid,bg,cg.numberOfInterpolationPoints(bg),bg,niBaseGrid[g]);
      	fprintf(plogFile,"Here is the base grid interpolation data that lies in the local refinement grid\n");
      	for( int i=0; i<niBaseGrid[g]; i++ )
      	{
        	  fprintf(plogFile," i=%i, ip=(%i,%i) donor=(%i) ci=(%8.2e,%8.2e)\n",i,
                                            ipBaseGrid(i,0),ipBaseGrid(i,1),
              		  interpoleeGridBaseGrid(i),
                                    interpolationCoordinatesBaseGrid(i,0),interpolationCoordinatesBaseGrid(i,1));
      	}
                fflush(plogFile);
            }
            

            if( debug & 4 )
            {
                fprintf(plogFile,"\n ==== Assign base grid mask with interp pts grid=%i (g=%i,bg=%i) at level=%i ====\n",
            		grid,g,l);
            }
            
      // printf(" interpoleeGridBG: bg=%i, min=%i, max=%i (before update)\n",bg,min(interpoleeGridBG),max(interpoleeGridBG));


            int * maskbp = maskbLocal.Array_Descriptor.Array_View_Pointer2;
            const int maskbDim0=maskbLocal.getRawDataSize(0);
            const int maskbDim1=maskbLocal.getRawDataSize(1);
#undef MASKB
#define MASKB(i0,i1,i2) maskbp[i0+maskbDim0*(i1+maskbDim1*(i2))]

            int * ipbgp = ipBaseGrid.Array_Descriptor.Array_View_Pointer1;
            int ipbgDim0=ipBaseGrid.getRawDataSize(0);
#undef IPBG
#define IPBG(i0,i1) ipbgp[i0+ipbgDim0*(i1)]

            const int isRectangular = cr.isRectangular();

      // mark base grid interpolation points with an index into its interpolation arrays
      // this let's us go from a mask<0 point to the index in the interpolationPoint array.
            i3=cb.indexRange(Start,axis3);
            const int ni=niBaseGrid[g]; // interp points on the local base grid
            if( numberOfDimensions==2 )
            {
      	for( int i=0; i<ni; i++ )
      	{
        	  MASKB(IPBG(i,0),IPBG(i,1),i3)=-(i+1);
      	}
            }
            else
            {
      	for( int i=0; i<ni; i++ )
      	{
        	  MASKB(IPBG(i,0),IPBG(i,1),IPBG(i,2))=-(i+1);
      	}
            }
      	
      // **** perform a periodic update on maskbLocal if it spans a periodic direction *****

      // cb.mask().periodicUpdate();
      	
            for( dir=0; dir<numberOfDimensions; dir++ )
            {
      	if( isPeriodic[dir] )
      	{ // does the local array span the periodic direction:
        	  if( maskbLocal.getBase(dir) <cb.indexRange(0,dir) && 
            	      maskbLocal.getBound(dir)>cb.indexRange(1,dir) )
        	  {
                        for( int d=0; d<3; d++ )
          	    {
            	      Jv[d]=maskbLocal.dimension(d); Kv[d]=Jv[d];
          	    }
            // assign left ghost points from right interior values
          	    Jv[dir]=Range(maskbLocal.getBase(dir),cb.indexRange(0,dir)-1);
          	    Kv[dir]=Jv[dir]+ndr[dir];
          	    maskbLocal(J1,J2,J3)=maskbLocal(K1,K2,K3);

	    // assign right ghost points from left interior values
          	    Jv[dir]=Range(cb.indexRange(1,dir)+1,maskbLocal.getBound(dir));
          	    Kv[dir]=Jv[dir]-ndr[dir];
          	    maskbLocal(J1,J2,J3)=maskbLocal(K1,K2,K3);
        	  }
      	}
      	
            }
            
            if( debug & 4 )
            {
	// fprintf(logFile,"*** cg.numberOfInterpolationPoints(bg)=%i\n",cg.numberOfInterpolationPoints(bg));
	// display(ipBG,"ipBG",logFile);
      	displayMask(maskbLocal,sPrintF(buff,"Here is maskbLocal with interp pts marked, bg=%i (g=%i grid=%i)",bg,g,grid),plogFile);
      	display(maskbLocal,sPrintF(buff,"Here is maskbLocal with interp pts marked, bg=%i (g=%i grid=%i)",bg,g,grid),plogFile,"%4i ");
            }
            
            
        }  // end for grid g=0,...



    // Stage II: 
    //    o make a list of potential donor points 


    // For each refinement grid interpolation point save a list of potential base grid interpolation points

    // interpolationPointBaseGridA[g][i][bg] = interp. point index on base grid
    // interpoleeBaseGridA[g][i][bg] = donor base grid
    // numberOfPossibleInterpoleeBaseGridsA[g][i] = number of potential donor base grids
    // interpCoordsA[g][bg][3*i]  : interp coordinates of the point on a given base grid

        int **interpolationPointBaseGridA = new int* [rl.numberOfComponentGrids()];
        int **interpoleeBaseGridA = new int* [rl.numberOfComponentGrids()];
        int ** numberOfPossibleInterpoleeBaseGridsA = new int* [rl.numberOfComponentGrids()];
        real **interpCoordsA = new real* [rl.numberOfComponentGrids()];

        const int maximumNumberOfPossibleBaseGrids=10;

        for( g=0; g<rl.numberOfComponentGrids(); g++ )
        {  // Begin grids on this level g=0,...


            int grid =rl.gridNumber(g);           // index into cg
            int bg = cg.baseGridNumber(grid);     // base grid for this refinement
            MappedGrid & cr = rl[g];              // refined grid
            MappedGrid & cb = cg[bg];             // base grid
//       const intArray & maskb = cb.mask();
//       intArray & maskr = cr.mask();

            const int ni = numberOfInterpolationPointsLocal(grid);  // number of new interp points
      // const intSerialArray & ip = interpolationPoints[l][g];      // new interp points 
            const intSerialArray & ip = cg->interpolationPointLocal[grid];  // use the local array

            interpolationPointBaseGridA[g] = new int [ni*maximumNumberOfPossibleBaseGrids];
            interpoleeBaseGridA[g] = new int [ni*maximumNumberOfPossibleBaseGrids];
            numberOfPossibleInterpoleeBaseGridsA[g] = new int [ni];
            interpCoordsA[g] = new real [3*ni*maximumNumberOfPossibleBaseGrids]; // these are not assigned until later
            
            #define interpolationPointBaseGrid(i,bg) interpolationPointBaseGridA[g][(bg)+maximumNumberOfPossibleBaseGrids*(i)]    
            #define interpoleeBaseGrid(i,bg) interpoleeBaseGridA[g][(bg)+maximumNumberOfPossibleBaseGrids*(i)]
            #define numberOfPossibleInterpoleeBaseGrids(i) numberOfPossibleInterpoleeBaseGridsA[g][i]
            #define interpCoords(i,bg,dir) interpCoordsA[g][(bg)+maximumNumberOfPossibleBaseGrids*((i)+ni*(dir))]
            
      // these names are too close to the above
            intSerialArray & ipBaseGrid = ipBaseGridA[g];
            intSerialArray & interpoleeGridBaseGrid = interpoleeGridBaseGridA[g];
            realSerialArray & interpolationCoordinatesBaseGrid = interpolationCoordinatesBaseGridA[g];


//       intArray & ipBG = cg.interpolationPoint[bg];
//       intArray & interpoleeGridBG = cg.interpoleeGrid[bg];
//       realArray & interpolationCoordinatesBG = cg.interpolationCoordinates[bg];

            if( debug & 4 )
                fprintf(plogFile,"\n ========= Find donor pts for grid=%i (g=%i) bg=%i at refinement level=%i =========\n",
            		grid,g,bg,l);

            int rf[3];  // refinement factors (to the BASE GRID!)
            rf[0]=rl.refinementFactor(0,g);
            rf[1]=rl.refinementFactor(1,g);
            rf[2]=rl.refinementFactor(2,g);

            bool isPeriodic[3]={false,false,false}; //
            for( dir=0; dir<numberOfDimensions; dir++ )
            {
      	isPeriodic[dir]=  cb.isPeriodic(dir)==Mapping::functionPeriodic;
            }

            intSerialArray & maskbLocal = maskBaseGrid[g];

            int * maskbp = maskbLocal.Array_Descriptor.Array_View_Pointer2;
            const int maskbDim0=maskbLocal.getRawDataSize(0);
            const int maskbDim1=maskbLocal.getRawDataSize(1);
            #undef MASKB
            #define MASKB(i0,i1,i2) maskbp[i0+maskbDim0*(i1+maskbDim1*(i2))]

            int * ipp = ip.Array_Descriptor.Array_View_Pointer1;
            int ipDim0=ip.getRawDataSize(0);
            #undef IP
            #define IP(i0,i1) ipp[i0+ipDim0*(i1)]

            int * ipbgp = ipBaseGrid.Array_Descriptor.Array_View_Pointer1;
            int ipbgDim0=ipBaseGrid.getRawDataSize(0);
            #undef IPBG
            #define IPBG(i0,i1) ipbgp[i0+ipbgDim0*(i1)]


            i3=j3=k3=l3=cr.indexRange(Start,axis3);
            const int *girp = cb.gridIndexRange().getDataPointer();
            #define GIR(side,axis) girp[side+2*(axis)]

            bool retry=true;  // in parallel we initially turn on the retry option to we don't have to repeat a point
            for( int i=0; i<ni; i++ ) // refinement interpolation points
            {
//         // int interpoleeFound=0; // 0=not found, 1=found but from a base grid, 2=found from a refinement (done)
// 	int interpolee=-1;     // best guess so far for an interpolee grid 
            
                bool coincident=true;
                for( axis=0; axis<numberOfDimensions; axis++ )
      	{
                    iv[axis]=IP(i,axis);      // check this interp point

                    if( iv[axis] % rf[axis] != 0 )
          	    coincident=false;     // this pt does not lie on a base grid pt
                    
          // [ kv[dir] : lv[dir] ] -- look in this box of points on the base grid mask

        	  kv[axis]=floorDiv(iv[axis],rf[axis]);              // base grid pt <= iv
                    if( isPeriodic[axis] )
        	  { // adjust for periodicity .. but only if we are outside the local base grid mask
                        if( kv[axis]<maskbLocal.getBase(axis) || kv[axis]>=maskbLocal.getBound(axis) ) // *wdh* 060312
          	    {
            	      int period=GIR(End,axis)-GIR(Start,axis);
            	      kv[axis] =((kv[axis]+period-GIR(Start,axis)) % period)+GIR(Start,axis);
          	    }
          	    
        	  }
          // lv[axis]=(iv[axis]+rf[axis]-1)/rf[axis];  // base grid pt >= iv
                    lv[axis]=kv[axis] + ((iv[axis]%rf[axis]) !=0); // add 1 if iv is not coincident
                    if( retry )
        	  { // If we are re-doing this point, increase the size of the base grid region that we search.
          	    if( kv[axis]==lv[axis] )
            	      lv[axis]+=1;
            // coincident=false; // assume this
        	  }
        	  
      	}
      	if( debug & 4)
        	  fprintf(plogFile,"\n>>  Interp. pt %i=(%i,%i,%i) (grid %i, base=%i) base coords=kv=(%i,%i):lv=(%i,%i)"
                                          "...\n",i,i1,i2,i3,grid,bg,kv[0],kv[1],lv[0],lv[1]);

	// **** For this interp pt., make a list of possible donor base grids (usually only one) *** 
      	numberOfPossibleInterpoleeBaseGrids(i)=0;
      	for( j3=kv[2]; j3<=lv[2]; j3++ ) // loop over neighbouring base grid points.
      	{
        	  for( j2=kv[1]; j2<=lv[1]; j2++ )
        	  {
          	    for( j1=kv[0]; j1<=lv[0]; j1++ )
          	    {
            	      int ib=-MASKB(j1,j2,j3)-1;
            	      if( ib>=0 && ib<niBaseGrid[g] )
            	      {
		// ** The base grid point jv is an interpolation point. ***
                //   ib is an index into the local base grid interpolation data arrays

                                assert( ib<=interpoleeGridBaseGrid.getBound(0) );

            		int bgDonor=interpoleeGridBaseGrid(ib);  // donor grid corresponding to the base grid interp pt.
                                assert( bgDonor>=0 && bgDonor<cg.numberOfBaseGrids());
		// make sure we don't already have this one in the list.
            		bool alreadyFound=false;
            		for( int nb=0; nb<numberOfPossibleInterpoleeBaseGrids(i); nb++ )
            		{
              		  if( interpoleeBaseGrid(i,nb)==bgDonor )
              		  {
                		    alreadyFound=true;
                		    break;
              		  }
            		}
            		if( !alreadyFound )
            		{
              		  if( debug & 4)
                		    fprintf(plogFile,"  ..interp. pt %i=(%i,%i,%i) (grid %i, base=%i). Base interp pt %i is close"
                      			    " ipBG=(%i,%i) =? jv=(%i,%i), donor base-grid=%i.\n",
                      			    i,i1,i2,i3,grid,bg,ib,IPBG(ib,0),IPBG(ib,1),j1,j2,interpoleeGridBaseGrid(ib));
      	
              		  interpolationPointBaseGrid(i,numberOfPossibleInterpoleeBaseGrids(i))=ib;
              		  interpoleeBaseGrid(i,numberOfPossibleInterpoleeBaseGrids(i))=bgDonor;
              		  numberOfPossibleInterpoleeBaseGrids(i)++;
              		  assert( numberOfPossibleInterpoleeBaseGrids(i)<=maximumNumberOfPossibleBaseGrids );
                                    fflush(plogFile);
            		}
            	      }
          	    }
        	  }
      	} // end for j3
      	if( numberOfPossibleInterpoleeBaseGrids(i)==0 )
      	{
                    printf("\n updateRefinement:ERROR: unable to find a base grid interp. pt.! See log file for details\n");
                    fprintf(plogFile,"\n updateRefinement:ERROR: unable to find a base grid interp. pt.! "
                                                  "numberOfPossibleInterpoleeBaseGrids=0\n");
        	  
        	  fprintf(plogFile,"  ..interp. pt %i=(%i,%i,%i) (grid %i, base=%i). niBaseGrid[g]=%i\n",
              		  i,i1,i2,i3,grid,bg,niBaseGrid[g]);
        	  fprintf(plogFile,"Mask values: ib=-mask-1 --> index into base grid interp points. 0 <= ib < %i\n",
              		  niBaseGrid[g]);
        	  fprintf(plogFile,"Mask on the base grid (bg=%i), pts [%i,%i][%i,%i][%i,%i]:\n",bg,
                                        kv[0],lv[0],kv[1],lv[1],kv[2],lv[2]);
        	  for( j3=kv[2]; j3<=lv[2]; j3++ ) // loop over neighbouring base grid points.
        	  {
          	    for( j2=kv[1]; j2<=lv[1]; j2++ )
          	    {
            	      for( j1=kv[0]; j1<=lv[0]; j1++ )
            	      {
                                int mm=MASKB(j1,j2,j3);  // int ib=-maskb(j1,j2,j3)-1;
            		fprintf(plogFile,"%6i ",mm); 
            	      }
                            fprintf(plogFile,"\n");
          	    }
        	  }
        	  Overture::abort("error");
      	}

            }  // end for i 

        }// end for g 
        


    // Stage III: 
    //    o invert donor points 

    // ** first try -- invert 1 pt at a time **

        for( g=0; g<rl.numberOfComponentGrids(); g++ )
        {  // Begin grids on this level g=0,...


            int grid =rl.gridNumber(g);           // index into cg
            int bg = cg.baseGridNumber(grid);     // base grid for this refinement
            MappedGrid & cr = rl[g];              // refined grid
            MappedGrid & cb = cg[bg];             // base grid

            const bool isRectangular=cr.isRectangular();
            const RealArray & vertex = isRectangular ? Overture::nullRealArray() : cr.vertex().getLocalArray();

            if( isRectangular )
            { // these next values are use in the VERTEX0 macro
      	cr.getRectangularGridParameters( dx, xab );
      	iv0[0]=cr.gridIndexRange(0,0);
      	iv0[1]=cr.gridIndexRange(0,1);
      	iv0[2]=cr.gridIndexRange(0,2);

            }
            if( debug & 8 )
            {
      	if( !cb.isRectangular() ) display(cb.vertex(),sPrintF("vertex on the base grid=%i",bg),logFile,"%3.1f ");
      	if( !cr.isRectangular() ) display(vertex,sPrintF("vertex on the refinement grid=%i",grid),logFile,"%3.1f ");
            }
            
            if( debug & 4 )
                fprintf(plogFile,"\n =============== get interpolation coord's grid=%i (g=%i) at refinement level=%i ============\n",
            		grid,g,l);

      // printf(" interpoleeGridBG: bg=%i, min=%i, max=%i (before update)\n",bg,min(interpoleeGridBG),max(interpoleeGridBG));

            int rf[3];  // refinement factors (to the BASE GRID!)
            rf[0]=rl.refinementFactor(0,g);
            rf[1]=rl.refinementFactor(1,g);
            rf[2]=rl.refinementFactor(2,g);

      // const intSerialArray & ip = interpolationPoints[l][g];      // new interp points 
            const intSerialArray & ip = cg->interpolationPointLocal[grid];  // use the local array
            int * ipp = ip.Array_Descriptor.Array_View_Pointer1;
            int ipDim0=ip.getRawDataSize(0);
            #undef IP
            #define IP(i0,i1) ipp[i0+ipDim0*(i1)]

            i3=j3=k3=l3=cr.indexRange(Start,axis3);

            const int ni=numberOfInterpolationPointsLocal(grid);  // number of interp points on this processor

            for( int i=0; i<ni; i++ ) // refinement interpolation points
            {
                bool coincident=true;
                for( axis=0; axis<numberOfDimensions; axis++ )
      	{
                    iv[axis]=IP(i,axis);    
                    if( iv[axis] % rf[axis] != 0 )
          	    coincident=false;     // this pt does not lie on a base grid pt
      	}
      	if( debug & 4)
        	  fprintf(plogFile,"\n>>Interp. pt %i=(%i,%i,%i) (grid %i, base=%i) numPossibleBase=%i"
                                          " Trying to interpolate...\n",i,i1,i2,i3,grid,bg,numberOfPossibleInterpoleeBaseGrids(i));


        // loop over possible base grids
      	for( int nb=0; nb<numberOfPossibleInterpoleeBaseGrids(i); nb++ )
      	{
        	  if( debug & 4)
          	    fprintf(plogFile," ...invert pt on base grid %i (grid=%i isRectangular=%i)\n",interpoleeBaseGrid(i,nb), 
                                    grid,isRectangular);
          	    
          // find the coordinates of the interpolation point on this base grid:
        	  int baseGridInterpolee=interpoleeBaseGrid(i,nb);
        	  assert( baseGridInterpolee>=0 && baseGridInterpolee<cg.numberOfBaseGrids());
        	  
        	  if( !coincident )
        	  {
	    // invert the mapping to locate the point.
          	    if( isRectangular )
          	    {
            	      xxs(0,0)=VERTEX0(i1,i2,i3);
            	      xxs(0,1)=VERTEX1(i1,i2,i3);
            	      xxs(0,2)=VERTEX2(i1,i2,i3);
          	    }
          	    else
          	    {
            	      for( dir=0; dir<numberOfDimensions; dir++ )
            		xxs(0,dir)=vertex(i1,i2,i3,dir);              // *************  use vertexLocal
          	    }

            // ***** we need to do more than one point at a time ****
                        rbs=-1.;  // *wdh* 040324
          	    cg[baseGridInterpolee].mapping().getMapping().inverseMapS(xxs(0,Rx),rbs);

        	  }
        	  else
        	  {
                        realSerialArray & interpolationCoordinatesBaseGrid = interpolationCoordinatesBaseGridA[g];

          	    int ib=interpolationPointBaseGrid(i,nb);
          	    for( dir=0; dir<numberOfDimensions; dir++ )
            	      rbs(0,dir)=interpolationCoordinatesBaseGrid(ib,dir);
          	    
        	  }
          // save interp coords
                    for( dir=0; dir<numberOfDimensions; dir++ )
                        interpCoords(i,nb,dir)=rbs(0,dir);

        	  if( debug & 4)
          	    fprintf(plogFile,"            pt %i=(%i,%i,%i) (grid %i, base=%i) --> invert: r=(%8.2e,%8.2e,%8.2e)"
                		    " (coincident=%i)\n",
                		    i,i1,i2,i3,grid,bg,rbs(0,0),rbs(0,1),rbs(0,2),coincident);
        	  
      	}
            } 
        }  // end for g 

        
        
    // Now get a copy of the mask on all grids on this level
        for( g=0; g<rl.numberOfComponentGrids(); g++ )
        {
            int grid =rl.gridNumber(g);           // index into cg
      // localMaskCopy[grid] will be a copy on this processor of the entire mask
            redistribute(cg[grid].mask(),localMaskCopy[grid]);
        }
        

    // Stage IV: 
    //    o canInterpolate ? 

        IntegerArray multipleInterpoleeGrids(rl.numberOfComponentGrids());
        multipleInterpoleeGrids=false;


        for( g=0; g<rl.numberOfComponentGrids(); g++ )
        {  // Begin grids on this level g=0,...


            int grid =rl.gridNumber(g);           // index into cg
            int bg = cg.baseGridNumber(grid);     // base grid for this refinement
            MappedGrid & cr = rl[g];              // refined grid
            MappedGrid & cb = cg[bg];             // base grid

            const bool isRectangular=cr.isRectangular();
            const RealArray & vertex = isRectangular ? Overture::nullRealArray() : cr.vertex().getLocalArray();

            if( isRectangular )
            { // these next values are use in the VERTEX0 macro
      	cr.getRectangularGridParameters( dx, xab );
      	iv0[0]=cr.gridIndexRange(0,0);
      	iv0[1]=cr.gridIndexRange(0,1);
      	iv0[2]=cr.gridIndexRange(0,2);

            }
            if( debug & 4 )
                fprintf(plogFile,"\n =============== canInterpolate? grid=%i (g=%i) at refinement level=%i ============\n",
            		grid,g,l);


            int rf[3];  // refinement factors (to the BASE GRID!)
            rf[0]=rl.refinementFactor(0,g);
            rf[1]=rl.refinementFactor(1,g);
            rf[2]=rl.refinementFactor(2,g);

            i3=j3=k3=l3=cr.indexRange(Start,axis3);

            const int ni=numberOfInterpolationPointsLocal(grid);  // number of interp points on this processor

      // *** temp arrays for now:
            const int nid=max(1,ni);
//       intSerialArray interpoleeGrid(nid); 
//       realSerialArray interpolationCoordinates(nid,numberOfDimensions); 
//       intSerialArray variableInterpolationWidth(nid); 

            intSerialArray & interpolationPoint = cg->interpolationPointLocal[grid]; 
            interpolationPoint.resize(nid,numberOfDimensions);  // ** does this resize maintain the old values?
            
            intSerialArray & interpoleeGrid = cg->interpoleeGridLocal[grid]; 
            interpoleeGrid.redim(nid);
            intSerialArray & variableInterpolationWidth = cg->variableInterpolationWidthLocal[grid]; 
            variableInterpolationWidth.redim(nid);
            realSerialArray & interpolationCoordinates = cg->interpolationCoordinatesLocal[grid];
            interpolationCoordinates.redim(nid,numberOfDimensions);

            intSerialArray & interpoleeLocation = cg->interpoleeLocationLocal[grid]; 
            interpoleeLocation.redim(nid,numberOfDimensions);
            interpoleeLocation=notAssigned; // these are assigned below


      // const intSerialArray & ip = interpolationPoints[l][g];      // new interp points 
            const intSerialArray & ip = cg->interpolationPointLocal[grid];  // use the local array
            int * ipp = ip.Array_Descriptor.Array_View_Pointer1;
            int ipDim0=ip.getRawDataSize(0);
            #undef IP
            #define IP(i0,i1) ipp[i0+ipDim0*(i1)]


            bool retry=true;  // in parallel we initially turn on the retry option to we don't have to repeat a point

            for( int i=0; i<ni; i++ ) // refinement interpolation points
            {
                bool coincident=true;
                for( axis=0; axis<numberOfDimensions; axis++ )
      	{
                    iv[axis]=IP(i,axis);    

                    if( iv[axis] % rf[axis] != 0 )
          	    coincident=false;     // this pt does not lie on a base grid pt

          // compute kv, lv here or just below for debugging?
      	}
      	if( debug & 4)
      	{
        	  fprintf(plogFile,"\n>>canInterpolate:Interp. pt %i=(%i,%i,%i) (grid %i, base=%i)\n"
              		  " Trying to interpolate...\n",i,i1,i2,i3,grid,bg);
      	}
      	

	// check the possible base grids.
	// *** we should try to check the last valid choice ****
      	bool canInterpolate=false;
      	bool backupCanInterpolate=false;
                int interpolee=-1;
      	for( int nb=0; nb<numberOfPossibleInterpoleeBaseGrids(i); nb++ )
      	{
          // find the coordinates of the interpolation point on this base grid:
        	  int baseGridInterpolee=interpoleeBaseGrid(i,nb);

        	  if( debug & 4)
          	    fprintf(plogFile," ...check next base grid, nb=%i, base grid %i (grid=%i isRectangular=%i)\n",
                                    interpoleeBaseGrid(i,nb),bg,grid,isRectangular);
          	    
        	  
        	  if( true )
        	  { // get x coords for info messages:
          	    if( isRectangular )
          	    {
            	      xxs(0,0)=VERTEX0(i1,i2,i3);
            	      xxs(0,1)=VERTEX1(i1,i2,i3);
            	      xxs(0,2)=VERTEX2(i1,i2,i3);
          	    }
          	    else
          	    {
            	      for( dir=0; dir<numberOfDimensions; dir++ )
            		xxs(0,dir)=vertex(i1,i2,i3,dir);             
          	    }
        	  }
        	  
          // get the interp coords
                    for( dir=0; dir<numberOfDimensions; dir++ )
                        rbs(0,dir)=interpCoords(i,nb,dir); 



          // ******** now check canInterpolate ****
        	  MappedGrid & ibg = cg[baseGridInterpolee]; // the interpolee base grid.
          	    
          // ******************************************************************************************
	  // *** now check to see if we can interpolate from any refinement grids on this base grid ***
          // ******************************************************************************************

        	  for( int level=l; level>=0 && !canInterpolate ; level-- )
        	  {
          	    GridCollection & rll = cg.refinementLevel[level];

          	    for( int g2=0; g2<rll.numberOfComponentGrids() && !canInterpolate; g2++ )
          	    {
            	      int grid2=rll.gridNumber(g2);
            	      if( rll.baseGridNumber(g2)==baseGridInterpolee )
            	      {
		// ie[3]=={ie1,ie2,ie3} : nearest point on the interpolee grid
            		for( axis=0; axis<numberOfDimensions; axis++ )
            		{
              		  ie[axis]=int( (rbs(0,axis)/ibg.gridSpacing(axis))*rll.refinementFactor(axis,g2)+
                        				ibg.indexRange(Start,axis)+.5 );  // closest point (cell centered??)
		  // adjust points for periodicity -- the refinement patch may go from [-10,10] for example.
              		  if( ibg.isPeriodic(axis)==Mapping::functionPeriodic )
              		  {
                		    int period=(ibg.gridIndexRange(End,axis)-ibg.gridIndexRange(Start,axis))*
                  		      rll.refinementFactor(axis,g2);
                		    int ieNew =( (ie[axis]-rll[g2].indexRange(Start,axis)+period) % period ) +
                  		      rll[g2].indexRange(Start,axis);
                		    pShift[axis]=ieNew-ie[axis];

		    // fprintf(plogFile,"periodic shift: ie[%i]=%i ieNew=%i, period=%i\n",axis,ie[axis],
		    //        ieNew,period);
                      			    
                		    ie[axis]=ieNew;
              		  }
              		  else
                		    pShift[axis]=0;
            		}
            		if( debug & 2 )
            		{
              		  fprintf(plogFile," ..check refinement grid2=%i level=%i ie=(%i,%i) rb=(%4.2f,%4.2f) xx=(%8.2e,%8.2e)"
                                                    "bounds=[%i,%i]x[%i,%i]\n",
                    			  grid2,level,ie1,ie2,rbs(0,0),rbs(0,1),xxs(0,0),xxs(0,1),
                                                    rll[g2].indexRange(Start,0),rll[g2].indexRange(End,0),
                    			  rll[g2].indexRange(Start,1),rll[g2].indexRange(End,1) );
            		}
                  			
                // const IntegerArray & g2IndexRange = rll[g2].indexRange();
                                const IntegerArray & g2IndexRange = rll[g2].extendedIndexRange(); // *wdh* 040804 

            		if( ie1<g2IndexRange(Start,0) || ie1>g2IndexRange(End,0) ||
                		    ie2<g2IndexRange(Start,1) || ie2>g2IndexRange(End,1) )        
              		  continue;
            		if( numberOfDimensions==3 && 
                		    (ie3<g2IndexRange(Start,2) || ie3>g2IndexRange(End,2)) )
              		  continue;

		// we are inside this refinement grid.
            		if( debug & 4)
              		  fprintf(plogFile,"  ..pt is inside refinement grid g2=%i (grid2=%i) at level %i\n",
                    			  g2,grid2,level);

            		interpolee=rll.gridNumber(g2);
            		MappedGrid & ig = cg[interpolee];  
            		for( axis=0; axis<numberOfDimensions; axis++ )
            		{
              		  const int rf = rll.refinementFactor(axis,g2);
              		  rrs(0,axis)=(rbs(0,axis)*rf/ibg.gridSpacing(axis)+pShift[axis]
                        			      -(ig.indexRange(Start,axis)-ibg.indexRange(Start,axis)*rf) )*ig.gridSpacing(axis);
            		}
            		if( interpolee==baseGridInterpolee )
            		{
                                    for( axis=0; axis<numberOfDimensions; axis++ )
                		    if( ig.isPeriodic(axis) )
                		    {
                  		      rrs(0,axis)=fmod(rrs(0,axis)+1.,1.);   // base grid may be periodic, shift to [0,1]
                		    }
            		}
            		
                // *** implicit interpolation parameters should be ok to use ****
                // we need to allow for interpolation from the boundary of two refinement grids.

                // const intSerialArray & maski = ig.mask().getLocalArray(); // do this for now
                                const intSerialArray & maski = localMaskCopy[interpolee]; // do this for now


            		interpolates(0)=true;
                // ** new can interpolate goes here **
// 		cg.rcData->canInterpolate(grid,interpolee, rr, interpolates, useBackupRules, 
// 					  checkForOneSided );
            		cgCanInterpolate(grid,interpolee, rrs, interpolates, useBackupRules, checkForOneSided, cg,maski );

            		if( interpolates(0) )
            		{
              		  canInterpolate=true;
              		  if( debug & 4)
                		    fprintf(plogFile,"  ..pt %i can interp from refine grid %i, r=(%6.2e,%6.2e), rb=(%6.2e,%6.2e)"
                                                        " coincident=%i\n",
                      			    i,rll.gridNumber(g2),rrs(0,0),rrs(0,1),rbs(0,0),rbs(0,1),coincident);

		  // assign all these below : interpoleeLocation(i,Rx)=0;  // ******
              		  break;
            		}
            		else 
            		{
              		  if( level==0 
                         		         || retry ) // *wdh* added 040804
              		  {
		    // try lower order interpolation as a backup : backupCanInterpolate=true
                		    const int width=cg.interpolationWidth(0,grid,grid2,0);
                		    const real ov = cg.interpolationOverlap(0,grid,grid2,0);

                    // temporarily change these for the canInterpolate function:
                		    cg.interpolationWidth(Rx,grid,grid2,0)=max(2,width-1);  // *wdh* max added 040804
                		    cg.interpolationOverlap(Rx,grid,grid2,0)-=max(0.,.5);   // *wdh* max added 040804 

                		    interpolates(0)=true;
                    // ** new can interpolate goes here **
// 		    cg.rcData->canInterpolate(grid,interpolee, rr, interpolates, useBackupRules, 
// 					      checkForOneSided );
                  		    cgCanInterpolate(grid,interpolee, rrs, interpolates, useBackupRules, checkForOneSided, cg,maski );
  

                		    if( interpolates(0) )
                		    {
                  		      if( debug & 4)
                  			fprintf(plogFile,"  ..pt %i can backup interp from refine grid %i, r=(%6.2e,%6.2e) width=%i\n",
                        				i,rll.gridNumber(g2),rrs(0,0),rrs(0,1),width-1);

                  		      backupCanInterpolate=true;                 // *** we keep looking in this case

                      // **** fix this:
                  		      interpoleeGrid(i)=interpolee;
                  		      variableInterpolationWidth(i)=width-1;
                  		      interpolationCoordinates(i,Rx)=rrs(0,Rx);
                		    }
                		    else // cannot interpolate
                		    {
		      // Allow interpolation if we are just outside a physical boundary
                      // This case can happen, for e.g., when a cartesian grid has a higher priority
                      // than a boundary fitted grid (cicd.cmd) and the stair-step boundary lies
                      // very close to the physical boundary

                      // *NOTE* if there is a refinement grid on this interpolee grid we should probably
                      //        use it instead

                                            RealArray rps(1,3);  // will hold the projected interp point
                                            const IntegerArray & bc = cg[interpolee].boundaryCondition();
                                            bool pointWasProjected=false;
                                            for( dir=0; dir<numberOfDimensions; dir++ )
                  		      {
                                                rps(0,dir)=rrs(0,dir);
                                                for( int side=0; side<=1; side++ )
                  			{
                    			  if( bc(side,dir)>0  && ( (side==0 && rrs(0,dir)<0.) || (side==1 && rrs(0,dir)>1.) ) )
                    			  {
                                                        pointWasProjected=true;
                                                        rps(0,dir)=side;  // move the interpolation location to be on the boundary
                                                        break;
                    			  }
                  			}
                  		      }
                  		      if( pointWasProjected )
                  		      {
                  			if( debug & 4)
                                                    fprintf(plogFile,"  ..pt %i try to interp pt from just? outside a boundary,"
                         			       "r=(%6.2e,%6.2e,%6.2e) r(projected)=(%6.2e,%6.2e,%6.2e)\n",
                         			       i,rrs(0,0),rrs(0,1),rrs(0,2),rps(0,0),rps(0,1),rps(0,2) );

                  			interpolates(0)=true;

                        // *** new canInterpolate ***
// 			cg.rcData->canInterpolate(grid,interpolee, rp, interpolates, useBackupRules, 
// 						  checkForOneSided );
                    			cgCanInterpolate(grid,interpolee, rps, interpolates, useBackupRules, checkForOneSided, cg,maski );

                                                if( interpolates(0) )
                  			{
                    			  if( debug & 4)
                      			    printf("updateRefinement:INFO: backup interpolation from just? outside a boundary,"
                           				   "grid=%i interpolee=%i r=(%6.2e,%6.2e,%6.2e) r(projected)=(%6.2e,%6.2e,%6.2e)\n",
                                                                      grid,interpolee,rrs(0,0),rrs(0,1),rrs(0,2),rps(0,0),rps(0,1),rps(0,2) );
                    			  if( debug & 4)
                      			    fprintf(plogFile,"  ..pt %i is just outside a boundary, "
                                                                      "can backup interp from refine grid %i, r=(%6.2e,%6.2e,%6.2e) "
                                                                        "r(projected)=(%6.2e,%6.2e,%6.2e) width=%i\n",
                            				    i,rll.gridNumber(g2),rrs(0,0),rrs(0,1),rrs(0,2),
                                                                        rps(0,0),rps(0,1),rps(0,2),width-1);

                    			  backupCanInterpolate=true;                 // *** we keep looking in this case

                    			  interpoleeGrid(i)=interpolee;
                    			  variableInterpolationWidth(i)=width-1;
                    			  interpolationCoordinates(i,Rx)=rps(0,Rx);
                  			}

                  		      }
                  		      
                  		      
                		    } // end cannot interpolate

                		    cg.interpolationWidth(Rx,grid,grid2,0)=width; // reset
                		    cg.interpolationOverlap(Rx,grid,grid2,0)=ov;
        
              		  }
              		  if( !backupCanInterpolate )
              		  {
                		    if( debug & 4 )
                  		      fprintf(plogFile,"  ..pt %i can NOT interp from refine grid %i, r=(%6.2e,%6.2e), rb=(%6.2e,%6.2e)"
                                                        " coincident=%i\n",
                        			      i,rll.gridNumber(g2),rrs(0,0),rrs(0,1),rbs(0,0),rbs(0,1),coincident);
              		  }
                		    
            		}
                  			
            	      }
          	    } // end for( g2 )
        	  } // for level
      	} // for( nb... : for possible base grids

      	if( canInterpolate )
      	{
	  // interpoleeFound=2; // found but from a base grid.

        	  interpoleeGrid(i)=interpolee;
        	  interpolationCoordinates(i,Rx)=rrs(0,Rx);
        	  variableInterpolationWidth(i)=cg.interpolationWidth(0,grid,interpolee,mgLevel);

        	  if( debug & 4)
          	    fprintf(plogFile,"  >>pt %i can interp from interpolee grid %i, r=(%6.2e,%6.2e) width=%i\n",
                		    i,interpoleeGrid(i),interpolationCoordinates(i,0),interpolationCoordinates(i,1),
                		    variableInterpolationWidth(i));
              		  
        	  if( debug & 4)
        	  {
          	    if( coincident ) 
          	    {

            	      fprintf(plogFile,"<<interp pt %6i=(%i,%i,%i) (grid %i, base=%i) is coincident and interps from grid %i,"
                  		      " r=(%6.2e,%6.2e,%6.2e) \n",
                  		      i,i1,i2,i3,grid,bg,interpolee,rrs(0,0),rrs(0,1),rrs(0,2));
          	    }
          	    else
          	    {
            	      fprintf(plogFile,"  ..inbetween interp.point %5i (refine=%i, base=%i) can interp "
                  		      "from grid %i, r=(%6.2e,%6.2e), width=%i \n",i,grid,bg,interpolee,rrs(0,0),rrs(0,1),
                  		      variableInterpolationWidth(i));
          	    }
        	  }
      	}
      	else if( backupCanInterpolate )
      	{
	  // canInterpolate=true;
        	  if( debug & 4)
          	    fprintf(plogFile,"  >>pt %i can backup interp from interpolee grid %i, r=(%6.2e,%6.2e) width=%i\n",
                		    i,interpoleeGrid(i),interpolationCoordinates(i,0),interpolationCoordinates(i,1),
                		    variableInterpolationWidth(i));
                    
	  // interpoleeFound=1; // found but from a base grid.
      	}
      	else
      	{
                    if( !retry )
        	  { // We failed to interpolate -- try again
          	    if( debug & 4 ) 
          	    {
                            fprintf(plogFile,"  ***Unable to interp the refinement pt on the first try. Try again"
                  		      " checking a larger region on the base grid...");
          	    }
          	    retry=true;
          	    i--;   // redo this value of i

                        Overture::abort("retry not implemented");
          	    
          	    continue;
        	  }
                    else
        	  {
                        fprintf(plogFile,"Unable to interp even with a retry!\n");
        	  }
        	  
          // ***** FAILED to interpolate -- output diagnostics ******
                    const int *girp = cb.gridIndexRange().getDataPointer();
        	  bool isPeriodic[3]={false,false,false}; //
        	  for( dir=0; dir<numberOfDimensions; dir++ )
        	  {
          	    isPeriodic[dir]=  cb.isPeriodic(dir)==Mapping::functionPeriodic;
        	  }

        	  for( axis=0; axis<numberOfDimensions; axis++ )
        	  {
            // *** compute kv, lv bounds ***
          	    kv[axis]=floorDiv(iv[axis],rf[axis]);              // base grid pt <= iv
          	    if( isPeriodic[axis] )
          	    { // adjust for periodicity
            	      int period=GIR(End,axis)-GIR(Start,axis);
            	      kv[axis] =((kv[axis]+period-GIR(Start,axis)) % period)+GIR(Start,axis);
          	    }
	    // lv[axis]=(iv[axis]+rf[axis]-1)/rf[axis];  // base grid pt >= iv
          	    lv[axis]=kv[axis] + ((iv[axis]%rf[axis]) !=0); // add 1 if iv is not coincident
          	    if( retry )
          	    { // If we are re-doing this point, increase the size of the base grid region that we search.
            	      if( kv[axis]==lv[axis] )
            		lv[axis]+=1;
	      // coincident=false; // assume this
          	    }
        	  
        	  }

                    intSerialArray & maskbLocal = maskBaseGrid[g];
        
                    int * maskbp = maskbLocal.Array_Descriptor.Array_View_Pointer2;
                    const int maskbDim0=maskbLocal.getRawDataSize(0);
                    const int maskbDim1=maskbLocal.getRawDataSize(1);
                    #undef MASKB
                    #define MASKB(i0,i1,i2) maskbp[i0+maskbDim0*(i1+maskbDim1*(i2))]

        	  printf("updateRefinement:ERROR: refinement patch interpolation point cannot interpolate! \n");
        	  fprintf(plogFile,"updateRefinement:ERROR: refinement patch interpolation point cannot interpolate! \n");
        	  fprintf(plogFile,"Mask on the base grid (bg=%i), pts [%i,%i][%i,%i][%i,%i]:\n",bg,
                                        kv[0],lv[0],kv[1],lv[1],kv[2],lv[2]);
        	  for( j3=kv[2]; j3<=lv[2]; j3++ ) // loop over neighbouring base grid points.
        	  {
          	    for( j2=kv[1]; j2<=lv[1]; j2++ )
          	    {
            	      for( j1=kv[0]; j1<=lv[0]; j1++ )
            	      {
                                int mm=MASKB(j1,j2,j3);  // int ib=-maskb(j1,j2,j3)-1;
            		fprintf(plogFile,"%6i ",mm); 
            	      }
                            fprintf(plogFile,"\n");
          	    }
        	  }
                    for( int dir=0; dir<numberOfDimensions; dir++ )
        	  {
          	    if( kv[dir]==lv[dir] )
          	    {
            	      lv[dir]+=1;
          	    }
        	  }
        	  fprintf(plogFile,"*** show more pts: Mask on the base grid (bg=%i), pts [%i,%i][%i,%i][%i,%i]:\n",bg,
              		  kv[0],lv[0],kv[1],lv[1],kv[2],lv[2]);
        	  for( j3=kv[2]; j3<=lv[2]; j3++ ) // loop over neighbouring base grid points.
        	  {
          	    for( j2=kv[1]; j2<=lv[1]; j2++ )
          	    {
            	      for( j1=kv[0]; j1<=lv[0]; j1++ )
            	      {
                                int mm=MASKB(j1,j2,j3);  // int ib=-maskb(j1,j2,j3)-1;
            		fprintf(plogFile,"%6i ",mm); 
            	      }
                            fprintf(plogFile,"\n");
          	    }
        	  } 

        	  intSerialArray & interpoleeGridBaseGrid = interpoleeGridBaseGridA[g];
        	  
        	  for( j3=kv[2]; j3<=lv[2]; j3++ ) // loop over neighbouring base grid points.
        	  {
          	    for( j2=kv[1]; j2<=lv[1]; j2++ )
          	    {
            	      for( j1=kv[0]; j1<=lv[0]; j1++ )
            	      {
            		int ib=-MASKB(j1,j2,j3)-1;
            		if( ib>=0 && ib<numberOfInterpolationPointsLocal(bg) )
            		{
		  // the base grid point jv is an interpolation point.
              		  int ipbg=interpoleeGridBaseGrid(ib);
                                    fprintf(plogFile," ** The refinement grid point is close to base grid "
                                                                    "interpolation pt=%i, donor grid=%i\n",
                    			  ib,ipbg);
            		}
            	      }
          	    }
        	  }
        	  
        	  if( true )
        	  {
                        printf("I will save a file `updateRefinementDebug.cmd' that can be used with the `refine' test\n"
                                      " program in order to regenerate the adaptive grid and test it.\n");
                        outputRefinementInfoNew( cg, "bugGrid.hdf","updateRefinementDebug.cmd" );

          	    fclose(plogFile);
          	    throw "error";
        	  }
        	  else
        	  {

          	    interpoleeGrid(i)=interpolee;
          	    interpolationCoordinates(i,Rx)=rrs(0,Rx);
          	    variableInterpolationWidth(i)=cg.interpolationWidth(0,grid,interpolee,mgLevel);

        	  }
	  // throw "error";
      	}
      	

        	if( i>0 && !multipleInterpoleeGrids(g) )
          	  multipleInterpoleeGrids(g) =interpoleeGrid(i)!=interpoleeGrid(i-1);

	// *** retry=false;
            } // end for i :  loop over interp points


            if( numberOfInterpolationPointsLocal(grid)>0 )
            {
        // ** now mark interpolation points in the proper way ***
                intArray & maskr = cr.mask(); 
                const intSerialArray & maskrLocal = maskr.getLocalArray();
      	const int ni=numberOfInterpolationPointsLocal(grid); 
      	if( numberOfDimensions==2 )
      	{
        	  i3=cr.indexRange(Start,axis3);
                    for( int i=0; i<ni; i++ )
                  	    maskrLocal(IP(i,0),IP(i,1),i3)=MappedGrid::ISinterpolationPoint;   
      	}
      	else
      	{
                    for( int i=0; i<ni; i++ )
          	    maskrLocal(IP(i,0),IP(i,1),IP(i,2))=MappedGrid::ISinterpolationPoint;   
      	}

            }

      // make sure the parallel ghost points agree: 
            cr.mask().updateGhostBoundaries();



        } // end for g
        
        
    //   ***************************************
    //   **** Sort interp pts by donor grid ****
    //   ****  assign interpoleeLocations   ****
    //   ***************************************
        for( g=0; g<rl.numberOfComponentGrids(); g++ )
        {  
            int grid =rl.gridNumber(g);     

            intSerialArray & interpolationPoint = cg->interpolationPointLocal[grid]; 
            intSerialArray & interpoleeGrid = cg->interpoleeGridLocal[grid]; 
            intSerialArray & variableInterpolationWidth = cg->variableInterpolationWidthLocal[grid]; 
            realSerialArray & interpolationCoordinates = cg->interpolationCoordinatesLocal[grid];
            intSerialArray & interpoleeLocation = cg->interpoleeLocationLocal[grid]; 

            const int ni=numberOfInterpolationPointsLocal(grid); 
            IntegerArray & ise = cg.interpolationStartEndIndex;

      // sort the interpolation points by interpolee grid.
      // ise(all,grid,all)=-1;   // --> this is already done above now
            if( multipleInterpoleeGrids(g) )
            {
      	if( debug & 4 )
        	  fprintf(plogFile," ************* computeOverlap: Sorting interpolation points... ********************\n");
	// First count the number of interpolee points for each grid.
      	IntegerArray ng(cg.numberOfComponentGrids()+1);
      	intSerialArray ig(interpoleeGrid);  // we do need copies of these
      	intSerialArray ip(interpolationPoint);
      	intSerialArray il(interpoleeLocation);
      	realSerialArray ic(interpolationCoordinates);
      	intSerialArray iw(variableInterpolationWidth);
      	
      	ng=0;
      	for( int i=0; i<ni; i++ )
        	  ng(interpoleeGrid(i)+1)+=1;
      	
      	for( int gg=2; gg<=cg.numberOfComponentGrids(); gg++ )  // note <=
        	  ng(gg)+=ng(gg-1); // ng(g) now points to the starting position for interpolee grid "g"
        	  
      	for( int grid2=0; grid2<cg.numberOfComponentGrids(); grid2++ )
      	{
        	  if( ng(grid2+1)-ng(grid2)>0 )
        	  {
          	    ise(0,grid,grid2)=ng(grid2);      // start value
          	    ise(1,grid,grid2)=ng(grid2+1)-1;  // end value
	    // end value for implicit points: (could sort to put any implicit points first)
          	    ise(2,grid,grid2)= ise(1,grid,grid2);
        	  }
//             printf("     grid=%i, grid2=%i, ng(grid2)=%i, ng(grid2+1)=%i SE=[%i,%i]\n",
//                      grid,grid2,ng(grid2),ng(grid2+1),ise(0,grid,grid2),
//                       ise(1,grid,grid2));
      	}
      	
	// Now fill in the points -- this could be sped up --
      	for( int i=0; i<ni; i++ )
      	{
        	  int pos=ng(ig(i)); ng(ig(i))+=1;
        	  interpoleeGrid(pos)=ig(i);
        	  interpolationPoint(pos,Rx)=ip(i,Rx);
        	  interpolationCoordinates(pos,Rx)=ic(i,Rx);
        	  interpoleeLocation(pos,Rx)=il(i,Rx);
        	  variableInterpolationWidth(pos)=iw(i);
      	}
            }
            else
            {
      	if( ni>0 )
      	{
        	  int grid2=interpoleeGrid(0);
	  //  printf(" grid=%i, grid2=%i, ng(grid2)=%i, ni=%i\n",grid,grid2,cg.numberOfInterpolationPoints(grid));

        	  ise(0,grid,grid2)=0;      // start value
        	  ise(1,grid,grid2)=numberOfInterpolationPointsLocal(grid)-1;  // end value
	  // end value for implicit points: (could sort to put any implicit points first)
        	  ise(2,grid,grid2)= ise(1,grid,grid2);
      	}
            }
            if( debug & 4 )
            {
      	for( int grid2=0; grid2<cg.numberOfComponentGrids(); grid2++ )
        	  fprintf(plogFile," ==> grid=%i grid2=%i ise=%i %i \n",grid,grid2,ise(0,grid,grid2),ise(1,grid,grid2));
            }
            

            IntegerArray interpolationWidth(3);
            interpolationWidth=1;
            for( int i=0; i<ni; i++ )
            {
      	int grid2=interpoleeGrid(i);
                interpolationWidth(Rx)=variableInterpolationWidth(i);
      	MappedGrid & g2 = cg[grid2];
      	for( int axis=0; axis<numberOfDimensions; axis++ )
      	{
                    if( interpoleeLocation(i,axis)==notAssigned )
        	  {
	    // Get the lower-left corner of the interpolation cube.
          	    int intLoc=int(floor(interpolationCoordinates(i,axis)/g2.gridSpacing(axis) + g2.indexRange(0,axis) -
                         				 .5 * interpolationWidth(axis) + (g2.isCellCentered(axis) ? .5 : 1.)));
          	    if (!g2.isPeriodic(axis)) 
          	    {
            	      if( (intLoc < g2.extendedIndexRange(0,axis)) && (g2.boundaryCondition(Start,axis)>0) )
            	      {
		//                        Point is close to a BC side.
		//                        One-sided interpolation used.
            		intLoc = g2.extendedIndexRange(0,axis);
            	      }
            	      if( (intLoc + interpolationWidth(axis) - 1 > g2.extendedIndexRange(1,axis))
              		  && (g2.boundaryCondition(End,axis)>0) )
            	      {
		//                        Point is close to a BC side.
		//                        One-sided interpolation used.
            		intLoc = g2.extendedIndexRange(1,axis) - interpolationWidth(axis) + 1;
            	      }
          	    } // end if
          	    interpoleeLocation(i,axis) = intLoc;
        	  }
      	} // end for axis
            } // end for i 
        } // end for g 
        

    // *** Output all interpolation data for this grid ***
        if( debug & 2 )
        {
            fprintf(plogFile,"\n *** End of Stage IV -- Summary for myid=%i, np=%i ***\n",myid,np);
            for( g=0; g<rl.numberOfComponentGrids(); g++ )
            {  
      	int grid =rl.gridNumber(g);     

      	intSerialArray & interpolationPoint = cg->interpolationPointLocal[grid]; 
      	intSerialArray & interpoleeGrid = cg->interpoleeGridLocal[grid]; 
      	intSerialArray & variableInterpolationWidth = cg->variableInterpolationWidthLocal[grid]; 
      	realSerialArray & interpolationCoordinates = cg->interpolationCoordinatesLocal[grid];
      	intSerialArray & interpoleeLocation = cg->interpoleeLocationLocal[grid]; 

                const int ni=numberOfInterpolationPointsLocal(grid); 
                fprintf(plogFile,"\n ---- grid=%i ni=%i ---\n",grid,ni);
                for( int i=0; i<ni; i++ )
      	{
        	  fprintf(plogFile," grid=%i: i=%4i ip=(%3i,%3i,%3i) donor=%i il=(%3i,%3i,%3i) ci=(%9.2e,%9.2e,%9.2e)\n",
                                    grid,i,
              		  interpolationPoint(i,0),interpolationPoint(i,1),
                                              (numberOfDimensions==2 ? 0 : interpolationPoint(i,2)),
              		  interpoleeGrid(i),
              		  interpoleeLocation(i,0),interpoleeLocation(i,1),
                                              (numberOfDimensions==2 ? 0 : interpoleeLocation(i,2)),
              		  interpolationCoordinates(i,0),interpolationCoordinates(i,1),
                                    (numberOfDimensions==2 ? 0 : interpolationCoordinates(i,2)));
      	}
      	
            }
            fflush(plogFile);
        }
    

    // clean up 

        delete [] maskBaseGrid;
        delete [] ipBaseGridA;
        delete [] interpoleeGridBaseGridA;
        delete [] interpolationCoordinatesBaseGridA;
        delete [] niBaseGrid;

        for( int g=0; g<rl.numberOfComponentGrids(); g++ )
        {
            delete [] interpolationPointBaseGridA[g];
            delete [] interpoleeBaseGridA[g];
            delete [] numberOfPossibleInterpoleeBaseGridsA[g];
            delete [] interpCoordsA[g];
        }
        delete [] interpolationPointBaseGridA;
        delete [] interpoleeBaseGridA;
        delete [] numberOfPossibleInterpoleeBaseGridsA;
        delete [] interpCoordsA;
//    delete [] ;
        
            
    } // done refinement levels

    delete [] localMaskCopy;
    

    timeForInterpData=getCPU()-timeForInterpData;

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


// ================================================================================
//  DIM : 2,3
//  RATIO: 2,4, general
//  AXIS: AXIS1,AXIS2,AXIS3
//  rv1,rv2: 2 or (r1,r2,r3)
// ================================================================================

int Ogen::
markOffAxisRefinementMask( int numberOfDimensions, Index Ivr[3], Index Ivb[3], int rf[3],
                     			   intSerialArray & mask, const intSerialArray & maskb )
// =====================================================================================
// /Description: 
//     Mark off-axis mask points -- i.e. mark points that do not align exactly with
// coarse grid points.
//
//                   
// 
// =====================================================================================
{
    bool useNewAlgorithm=true;
    bool useOpt=true;

    int * maskp = mask.Array_Descriptor.Array_View_Pointer2;
    const int maskDim0=mask.getRawDataSize(0);
    const int maskDim1=mask.getRawDataSize(1);
#undef MASK
#define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]
    const int * maskbp = maskb.Array_Descriptor.Array_View_Pointer2;
    const int maskbDim0=maskb.getRawDataSize(0);
    const int maskbDim1=maskb.getRawDataSize(1);
#undef MASKB
#define MASKB(i0,i1,i2) maskbp[i0+maskbDim0*(i1+maskbDim1*(i2))]

    Index &I1r = Ivr[0], &I2r=Ivr[1], &I3r=Ivr[2];
    Index &I1b = Ivb[0], &I2b=Ivb[1], &I3b=Ivb[2];
    
    int i1b,i2b,i3b,i1r,i2r,i3r;
    int I1bBase,I2bBase,I3bBase; 
    int I1bBound,I2bBound, I3bBound;
    int I1rBase,I2rBase,I3rBase; 
    int I1rBound,I2rBound,I3rBound;
    int I1rStride,I2rStride,I3rStride;

    int axis;
    for( axis=0; axis<numberOfDimensions; axis++ )
    {
        Ivr[axis]=Range(Ivr[axis].getBase(),Ivr[axis].getBound()-rf[axis],rf[axis]);
        Ivb[axis]=Range(Ivb[axis].getBase(),Ivb[axis].getBound()-1);
    }
    if( numberOfDimensions==2 )
    {
    // only zero middle points with 2 or more 0 neighbours
    //        X---O   O---O
    //        | x |   | o |
    //        X---X   X---X
        if( rf[0]==2 && rf[1]==2 )
        {
            if( !useOpt )
            {
      	const intSerialArray & maskb0 = evaluate(maskb(I1b,I2b,I3b)==0);
      	where(  maskb0+(maskb(I1b+1,I2b  ,I3b)==0)+(maskb(I1b,I2b+1,I3b)==0)+(maskb(I1b+1,I2b+1,I3b)==0) > 1)
      	{
        	  for( int r2=1; r2<rf[1]; r2++ )
        	  {
          	    for( int r1=1; r1<rf[0]; r1++ )
          	    {
            	      mask(I1r+r1,I2r+r2,I3r)=0;
          	    }
        	  }
      	}
            }
            else
            {
      	FOR_3BR()
      	{
          	  if( (MASKB(i1b,i2b,i3b)==0)+(MASKB(i1b+1,i2b,i3b)==0)+
                            (MASKB(i1b,i2b+1,i3b)==0)+(MASKB(i1b+1,i2b+1,i3b)==0) > 1)
        	  {
                        MASK(i1r+1,i2r+1,i3r)=0;
        	  }
      	}
            }
            
        }
        else
        {

            const intSerialArray mask00=maskb(I1b  ,I2b  ,I3b)==0;
            const intSerialArray mask10=maskb(I1b+1,I2b  ,I3b)==0;
            const intSerialArray mask01=maskb(I1b  ,I2b+1,I3b)==0;
            const intSerialArray mask11=maskb(I1b+1,I2b+1,I3b)==0;
            
            setRefinementMaskFace(mask,Start,axis3,numberOfDimensions,rf,I1r,I2r,I3r,mask00,mask10,mask01,mask11);

        }
    }
    else // numberOfDimensions == 3
    {

        if( useNewAlgorithm )
        {
            if( rf[0]==2 && rf[1]==2 && rf[2]==2 )
            {
	// new way 
      	if( !useOpt )
      	{
        	  Ivr[axis3]=Range(Ivr[axis3].getBase(),Ivr[axis3].getBound()+rf[axis3],rf[axis3]);
        	  Ivb[axis3]=Range(Ivb[axis3].getBase(),Ivb[axis3].getBound()+1);
        	  where( (maskb(I1b,I2b  ,I3b)==0)+(maskb(I1b+1,I2b  ,I3b)==0)+
             		 (maskb(I1b,I2b+1,I3b)==0)+(maskb(I1b+1,I2b+1,I3b)==0) > 1 )
        	  {
          	    for( int r2=1; r2<rf[1]; r2++ )
          	    {
            	      for( int r1=1; r1<rf[0]; r1++ )
            	      {
            		mask(I1r+r1,I2r+r2,I3r)=0;
            	      }
          	    }
        	  }
        	  Ivr[axis3]=Range(Ivr[axis3].getBase(),Ivr[axis3].getBound()-rf[axis3],rf[axis3]);
        	  Ivb[axis3]=Range(Ivb[axis3].getBase(),Ivb[axis3].getBound()-1);

        	  Ivr[axis1]=Range(Ivr[axis1].getBase(),Ivr[axis1].getBound()+rf[axis1],rf[axis1]);
        	  Ivb[axis1]=Range(Ivb[axis1].getBase(),Ivb[axis1].getBound()+1);
        	  where( (maskb(I1b,I2b,I3b  )==0)+(maskb(I1b  ,I2b+1,I3b  )==0)+
             		 (maskb(I1b,I2b,I3b+1)==0)+(maskb(I1b  ,I2b+1,I3b+1)==0) > 1 )
        	  {
          	    for( int r3=1; r3<rf[2]; r3++ )
          	    {
            	      for( int r2=1; r2<rf[1]; r2++ )
            	      {
            		mask(I1r,I2r+r2,I3r+r3)=0;
            	      }
          	    }
        	  }
        	  Ivr[axis1]=Range(Ivr[axis1].getBase(),Ivr[axis1].getBound()-rf[axis1],rf[axis1]);
        	  Ivb[axis1]=Range(Ivb[axis1].getBase(),Ivb[axis1].getBound()-1);

        	  Ivr[axis2]=Range(Ivr[axis2].getBase(),Ivr[axis2].getBound()+rf[axis2],rf[axis2]);
        	  Ivb[axis2]=Range(Ivb[axis2].getBase(),Ivb[axis2].getBound()+1);
        	  where( (maskb(I1b,I2b,I3b  )==0)+(maskb(I1b+1,I2b,I3b  )==0)+
             		 (maskb(I1b,I2b,I3b+1)==0)+(maskb(I1b+1,I2b,I3b+1)==0) > 1 )
        	  {
          	    for( int r3=1; r3<rf[2]; r3++ )
          	    {
            	      for( int r1=1; r1<rf[0]; r1++ )
            	      {
            		mask(I1r+r1,I2r,I3r+r3)=0;
            	      }
          	    }
        	  }
        	  Ivr[axis2]=Range(Ivr[axis2].getBase(),Ivr[axis2].getBound()-rf[axis2],rf[axis2]);
        	  Ivb[axis2]=Range(Ivb[axis2].getBase(),Ivb[axis2].getBound()-1);


	  // center remains if 2 or more face centers remain.
        	  where(  (mask(I1r+1,I2r+1,I3r  )==0)+(mask(I1r+1,I2r+1,I3r+2)==0)+
              		  (mask(I1r+1,I2r  ,I3r+1)==0)+(mask(I1r+1,I2r+2,I3r+1)==0)+
              		  (mask(I1r  ,I2r+1,I3r+1)==0)+(mask(I1r+2,I2r+1,I3r+1)==0) > 4 )
        	  {
          	    for( int r3=1; r3<rf[2]; r3++ )
          	    {
            	      for( int r2=1; r2<rf[1]; r2++ )
            	      {
            		for( int r1=1; r1<rf[0]; r1++ )
            		{
              		  mask(I1r+r1,I2r+r2,I3r+r3)=0;
            		}
            	      }
          	    }
        	  }
      	}
      	else 
      	{ // ************* opt version ***********************

                    for( int axis=0; axis<numberOfDimensions; axis++ )
        	  {
          	    Ivr[axis]=Range(Ivr[axis].getBase(),Ivr[axis].getBound()+rf[axis],rf[axis]);
          	    Ivb[axis]=Range(Ivb[axis].getBase(),Ivb[axis].getBound()+1);
        	  }
        	  
                    FOR_3BR()
        	  {
            // mark a face i3=const if there are at least 2 holes on the face
          	    if( i1b<I1bBound && i2b<I2bBound &&
                                (MASKB(i1b,i2b  ,i3b)==0)+(MASKB(i1b+1,i2b  ,i3b)==0)+
            		(MASKB(i1b,i2b+1,i3b)==0)+(MASKB(i1b+1,i2b+1,i3b)==0) > 1 )
          	    {
            	      MASK(i1r+1,i2r+1,i3r)=0;
          	    }
            // mark a face i1=const if there are at least 2 holes on the face
          	    if( i2b<I2bBound && i3b<I3bBound &&
                                (MASKB(i1b,i2b  ,i3b)==0)+(MASKB(i1b,i2b  ,i3b+1)==0)+
            		(MASKB(i1b,i2b+1,i3b)==0)+(MASKB(i1b,i2b+1,i3b+1)==0) > 1 )
          	    {
            	      MASK(i1r,i2r+1,i3r+1)=0;
          	    }
            // mark a face i2=const if there are at least 2 holes on the face
          	    if( i1b<I1bBound && i3b<I3bBound &&
                                (MASKB(i1b  ,i2b,i3b)==0)+(MASKB(i1b  ,i2b,i3b+1)==0)+
            		(MASKB(i1b+1,i2b,i3b)==0)+(MASKB(i1b+1,i2b,i3b+1)==0) > 1 )
          	    {
            	      MASK(i1r+1,i2r,i3r+1)=0;
          	    }

        	  }
        	  
                    for( int axis=0; axis<numberOfDimensions; axis++ )
        	  {
          	    Ivr[axis]=Range(Ivr[axis].getBase(),Ivr[axis].getBound()-rf[axis],rf[axis]);
          	    Ivb[axis]=Range(Ivb[axis].getBase(),Ivb[axis].getBound()-1);
        	  }
          // center remains if 2 or more face centers remain.
                    FOR_3R()
        	  {
          	    if(  (MASK(i1r+1,i2r+1,i3r  )==0)+(MASK(i1r+1,i2r+1,i3r+2)==0)+
             		 (MASK(i1r+1,i2r  ,i3r+1)==0)+(MASK(i1r+1,i2r+2,i3r+1)==0)+
             		 (MASK(i1r  ,i2r+1,i3r+1)==0)+(MASK(i1r+2,i2r+1,i3r+1)==0) > 4 )
          	    {
            	      MASK(i1r+1,i2r+1,i3r+1)=0;
          	    }
        	  }
        	  
      	} // end opt
      	
            }
            else
            {
	// refinement factors > 2 

                if( !useOpt )
      	{
	  // -- first assign faces --
        	  Ivr[axis3]=Range(Ivr[axis3].getBase(),Ivr[axis3].getBound()+rf[axis3],rf[axis3]);
        	  Ivb[axis3]=Range(Ivb[axis3].getBase(),Ivb[axis3].getBound()+1);
      	
        	  if( true )
        	  {
          	    const intSerialArray mask00=maskb(I1b  ,I2b  ,I3b)==0;
          	    const intSerialArray mask10=maskb(I1b+1,I2b  ,I3b)==0;
          	    const intSerialArray mask01=maskb(I1b  ,I2b+1,I3b)==0;
          	    const intSerialArray mask11=maskb(I1b+1,I2b+1,I3b)==0;
            
          	    setRefinementMaskFace(mask,Start,axis3,numberOfDimensions,rf,I1r,I2r,I3r,mask00,mask10,mask01,mask11);
        	  }
            

        	  Ivr[axis3]=Range(Ivr[axis3].getBase(),Ivr[axis3].getBound()-rf[axis3],rf[axis3]);
        	  Ivb[axis3]=Range(Ivb[axis3].getBase(),Ivb[axis3].getBound()-1);

        	  Ivr[axis1]=Range(Ivr[axis1].getBase(),Ivr[axis1].getBound()+rf[axis1],rf[axis1]);
        	  Ivb[axis1]=Range(Ivb[axis1].getBase(),Ivb[axis1].getBound()+1);

        	  if( true )
        	  {
          	    const intSerialArray mask00=maskb(I1b  ,I2b  ,I3b  )==0;
          	    const intSerialArray mask10=maskb(I1b  ,I2b+1,I3b  )==0;
          	    const intSerialArray mask01=maskb(I1b  ,I2b  ,I3b+1)==0;
          	    const intSerialArray mask11=maskb(I1b  ,I2b+1,I3b+1)==0;
            
          	    setRefinementMaskFace(mask,Start,axis1,numberOfDimensions,rf,I1r,I2r,I3r,mask00,mask10,mask01,mask11);
        	  }

        	  Ivr[axis1]=Range(Ivr[axis1].getBase(),Ivr[axis1].getBound()-rf[axis1],rf[axis1]);
        	  Ivb[axis1]=Range(Ivb[axis1].getBase(),Ivb[axis1].getBound()-1);

        	  Ivr[axis2]=Range(Ivr[axis2].getBase(),Ivr[axis2].getBound()+rf[axis2],rf[axis2]);
        	  Ivb[axis2]=Range(Ivb[axis2].getBase(),Ivb[axis2].getBound()+1);
        	  if( true )
        	  {
          	    const intSerialArray mask00=maskb(I1b  ,I2b  ,I3b  )==0;
          	    const intSerialArray mask10=maskb(I1b  ,I2b  ,I3b+1)==0;
          	    const intSerialArray mask01=maskb(I1b+1,I2b  ,I3b  )==0;
          	    const intSerialArray mask11=maskb(I1b+1,I2b  ,I3b+1)==0;
            
          	    setRefinementMaskFace(mask,Start,axis2,numberOfDimensions,rf,I1r,I2r,I3r,mask00,mask10,mask01,mask11);
        	  }


        	  Ivr[axis2]=Range(Ivr[axis2].getBase(),Ivr[axis2].getBound()-rf[axis2],rf[axis2]);
        	  Ivb[axis2]=Range(Ivb[axis2].getBase(),Ivb[axis2].getBound()-1);
        
      	}
                else 
      	{
	  // optimized version
        	  int mask000,mask100,mask010,mask110,mask001,mask101,mask011,mask111;
                    int r1,r2,r3;

                    for( int axis=0; axis<numberOfDimensions; axis++ )
        	  {
          	    Ivr[axis]=Range(Ivr[axis].getBase(),Ivr[axis].getBound()+rf[axis],rf[axis]);
          	    Ivb[axis]=Range(Ivb[axis].getBase(),Ivb[axis].getBound()+1);
        	  }
        	  
                    if( numberOfDimensions==2 )
        	  {
          	    FOR_3BR()
          	    {
	      // mark a face i3=const if there are at least 2 holes on the face
            	      if( i1b<I1bBound && i2b<I2bBound )
            	      {
                  	        mask000=MASKB(i1b  ,i2b  ,i3b  )==0;
            		mask100=MASKB(i1b+1,i2b  ,i3b  )==0;
            		mask010=MASKB(i1b  ,i2b+1,i3b  )==0;
            		mask110=MASKB(i1b+1,i2b+1,i3b  )==0;

            // 		setMaskOnFace(2,general,mask000,mask100,mask010,mask110,AXIS3,r1,r2);
                            if( mask000+mask100+mask010+mask110 > 1 )
                            {
                // more than one corner of this cell has a zero -- all points in cell are zeroed.
                // beginFaceLoops(AXIS3)
                //                 #If "AXIS3" == "AXIS3"
                                  r3=0;
                                  for( r2=1; r2<=rf[1]-1; r2++ )
                                  for( r1=1; r1<=rf[0]-1; r1++ )
                                {
                                    MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
                            else if( mask000 )  // only lower left corner is zero
                            {
                // beginFaceLoops(AXIS3)
                //                 #If "AXIS3" == "AXIS3"
                                  r3=0;
                                  for( r2=1; r2<=rf[1]-1; r2++ )
                                  for( r1=1; r1<=rf[0]-1; r1++ )
                                {
                                    if( r1+r2 < rf[0] ) 
                              	MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
                            else if( mask100 ) // only lower right corner is zero.
                            {
                // beginFaceLoops(AXIS3)
                //                 #If "AXIS3" == "AXIS3"
                                  r3=0;
                                  for( r2=1; r2<=rf[1]-1; r2++ )
                                  for( r1=1; r1<=rf[0]-1; r1++ )
                                {
                                    if( r1>r2 )
                              	MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
                            else if( mask010 )
                            {
                // beginFaceLoops(AXIS3)
                //                 #If "AXIS3" == "AXIS3"
                                  r3=0;
                                  for( r2=1; r2<=rf[1]-1; r2++ )
                                  for( r1=1; r1<=rf[0]-1; r1++ )
                                {
                                    if( r1<r2 )
                              	MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
                            else if( mask110 )
                            {
                // beginFaceLoops(AXIS3)
                //                 #If "AXIS3" == "AXIS3"
                                  r3=0;
                                  for( r2=1; r2<=rf[1]-1; r2++ )
                                  for( r1=1; r1<=rf[0]-1; r1++ )
                                {
                                    if( r1+r2 > rf[0] )
                              	MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
            	      }
          	    }
          	    
        	  }
        	  else // 3D
        	  {
            // if( rf[0]==4 && rf[1]==4 && rf[2]==4 )
          	    
          	    FOR_3BR()
          	    {
	      // mark a face i3=const if there are at least 2 holes on the face
            	      mask000=MASKB(i1b  ,i2b  ,i3b  )==0;

            	      if( i1b<I1bBound && i2b<I2bBound )
            	      {
            		mask100=MASKB(i1b+1,i2b  ,i3b  )==0;
                      	        mask010=MASKB(i1b  ,i2b+1,i3b  )==0;
            		mask110=MASKB(i1b+1,i2b+1,i3b  )==0;

            // 		setMaskOnFace(3,general,mask000,mask100,mask010,mask110,AXIS3,r1,r2);
                            if( mask000+mask100+mask010+mask110 > 1 )
                            {
                // more than one corner of this cell has a zero -- all points in cell are zeroed.
                // beginFaceLoops(AXIS3)
                //                 #If "AXIS3" == "AXIS3"
                                  r3=0;
                                  for( r2=1; r2<=rf[1]-1; r2++ )
                                  for( r1=1; r1<=rf[0]-1; r1++ )
                                {
                                    MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
                            else if( mask000 )  // only lower left corner is zero
                            {
                // beginFaceLoops(AXIS3)
                //                 #If "AXIS3" == "AXIS3"
                                  r3=0;
                                  for( r2=1; r2<=rf[1]-1; r2++ )
                                  for( r1=1; r1<=rf[0]-1; r1++ )
                                {
                                    if( r1+r2 < rf[0] ) 
                              	MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
                            else if( mask100 ) // only lower right corner is zero.
                            {
                // beginFaceLoops(AXIS3)
                //                 #If "AXIS3" == "AXIS3"
                                  r3=0;
                                  for( r2=1; r2<=rf[1]-1; r2++ )
                                  for( r1=1; r1<=rf[0]-1; r1++ )
                                {
                                    if( r1>r2 )
                              	MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
                            else if( mask010 )
                            {
                // beginFaceLoops(AXIS3)
                //                 #If "AXIS3" == "AXIS3"
                                  r3=0;
                                  for( r2=1; r2<=rf[1]-1; r2++ )
                                  for( r1=1; r1<=rf[0]-1; r1++ )
                                {
                                    if( r1<r2 )
                              	MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
                            else if( mask110 )
                            {
                // beginFaceLoops(AXIS3)
                //                 #If "AXIS3" == "AXIS3"
                                  r3=0;
                                  for( r2=1; r2<=rf[1]-1; r2++ )
                                  for( r1=1; r1<=rf[0]-1; r1++ )
                                {
                                    if( r1+r2 > rf[0] )
                              	MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
            	      }
            	      if( i2b<I2bBound && i3b<I3bBound )
            	      {
        
            		mask010=MASKB(i1b  ,i2b+1,i3b  )==0;
            		mask001=MASKB(i1b  ,i2b  ,i3b+1)==0;
            		mask011=MASKB(i1b  ,i2b+1,i3b+1)==0;
            // 		setMaskOnFace(3,general,mask000,mask010,mask001,mask011,AXIS1,r2,r3);
                            if( mask000+mask010+mask001+mask011 > 1 )
                            {
                // more than one corner of this cell has a zero -- all points in cell are zeroed.
                // beginFaceLoops(AXIS1)
                //                 #If "AXIS1" == "AXIS3"
                //                 #Elif "AXIS1" == "AXIS2"
                //                 #Elif "AXIS1" == "AXIS1"
                                  r1=0;
                                  for( r3=1; r3<=rf[2]-1; r3++ )
                                  for( r2=1; r2<=rf[1]-1; r2++ )
                                {
                                    MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
                            else if( mask000 )  // only lower left corner is zero
                            {
                // beginFaceLoops(AXIS1)
                //                 #If "AXIS1" == "AXIS3"
                //                 #Elif "AXIS1" == "AXIS2"
                //                 #Elif "AXIS1" == "AXIS1"
                                  r1=0;
                                  for( r3=1; r3<=rf[2]-1; r3++ )
                                  for( r2=1; r2<=rf[1]-1; r2++ )
                                {
                                    if( r2+r3 < rf[0] ) 
                              	MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
                            else if( mask010 ) // only lower right corner is zero.
                            {
                // beginFaceLoops(AXIS1)
                //                 #If "AXIS1" == "AXIS3"
                //                 #Elif "AXIS1" == "AXIS2"
                //                 #Elif "AXIS1" == "AXIS1"
                                  r1=0;
                                  for( r3=1; r3<=rf[2]-1; r3++ )
                                  for( r2=1; r2<=rf[1]-1; r2++ )
                                {
                                    if( r2>r3 )
                              	MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
                            else if( mask001 )
                            {
                // beginFaceLoops(AXIS1)
                //                 #If "AXIS1" == "AXIS3"
                //                 #Elif "AXIS1" == "AXIS2"
                //                 #Elif "AXIS1" == "AXIS1"
                                  r1=0;
                                  for( r3=1; r3<=rf[2]-1; r3++ )
                                  for( r2=1; r2<=rf[1]-1; r2++ )
                                {
                                    if( r2<r3 )
                              	MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
                            else if( mask011 )
                            {
                // beginFaceLoops(AXIS1)
                //                 #If "AXIS1" == "AXIS3"
                //                 #Elif "AXIS1" == "AXIS2"
                //                 #Elif "AXIS1" == "AXIS1"
                                  r1=0;
                                  for( r3=1; r3<=rf[2]-1; r3++ )
                                  for( r2=1; r2<=rf[1]-1; r2++ )
                                {
                                    if( r2+r3 > rf[0] )
                              	MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
            	      }
                            if( i1b<I1bBound && i3b<I3bBound )
            	      {

            		mask001=MASKB(i1b  ,i2b  ,i3b+1)==0;
            		mask100=MASKB(i1b+1,i2b  ,i3b  )==0;
            		mask101=MASKB(i1b+1,i2b  ,i3b+1)==0;
            // 		setMaskOnFace(3,general,mask000,mask001,mask100,mask101,AXIS2,r3,r1);  // **** 070601 *** r1,r3 -> r3,r1
                            if( mask000+mask001+mask100+mask101 > 1 )
                            {
                // more than one corner of this cell has a zero -- all points in cell are zeroed.
                // beginFaceLoops(AXIS2)
                //                 #If "AXIS2" == "AXIS3"
                //                 #Elif "AXIS2" == "AXIS2"
                                  r2=0;
                                  for( r3=1; r3<=rf[2]-1; r3++ )
                                  for( r1=1; r1<=rf[0]-1; r1++ )
                                {
                                    MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
                            else if( mask000 )  // only lower left corner is zero
                            {
                // beginFaceLoops(AXIS2)
                //                 #If "AXIS2" == "AXIS3"
                //                 #Elif "AXIS2" == "AXIS2"
                                  r2=0;
                                  for( r3=1; r3<=rf[2]-1; r3++ )
                                  for( r1=1; r1<=rf[0]-1; r1++ )
                                {
                                    if( r3+r1 < rf[0] ) 
                              	MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
                            else if( mask001 ) // only lower right corner is zero.
                            {
                // beginFaceLoops(AXIS2)
                //                 #If "AXIS2" == "AXIS3"
                //                 #Elif "AXIS2" == "AXIS2"
                                  r2=0;
                                  for( r3=1; r3<=rf[2]-1; r3++ )
                                  for( r1=1; r1<=rf[0]-1; r1++ )
                                {
                                    if( r3>r1 )
                              	MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
                            else if( mask100 )
                            {
                // beginFaceLoops(AXIS2)
                //                 #If "AXIS2" == "AXIS3"
                //                 #Elif "AXIS2" == "AXIS2"
                                  r2=0;
                                  for( r3=1; r3<=rf[2]-1; r3++ )
                                  for( r1=1; r1<=rf[0]-1; r1++ )
                                {
                                    if( r3<r1 )
                              	MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }
                            else if( mask101 )
                            {
                // beginFaceLoops(AXIS2)
                //                 #If "AXIS2" == "AXIS3"
                //                 #Elif "AXIS2" == "AXIS2"
                                  r2=0;
                                  for( r3=1; r3<=rf[2]-1; r3++ )
                                  for( r1=1; r1<=rf[0]-1; r1++ )
                                {
                                    if( r3+r1 > rf[0] )
                              	MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                }
                            }

            	      }
            	      
          	    } // end for3
          	    

        	  }

                    for( int axis=0; axis<numberOfDimensions; axis++ )
        	  {
          	    Ivr[axis]=Range(Ivr[axis].getBase(),Ivr[axis].getBound()-rf[axis],rf[axis]);
          	    Ivb[axis]=Range(Ivb[axis].getBase(),Ivb[axis].getBound()-1);
        	  }
      	} // end optimized version
      	
      	
	// *** interior points: 
        //        We need to mark the ? points below:
        //
        //            x--0--0--0--0
        //            |  |  |  |  |
        //            x--?--?--?--0
        //            |  |  |  |  |
        //            x--?--?--?--0
        //            |  |  |  |  |
        //            x--?--?--?--0
        //            |  |  |  |  |
        //            x--x--x--x--x
        //
        //   to discretize:
        ///   1) there must be a valid point on a face in each direction:
        //       (mask(left face)!=0 || mask(right face)!=0) && (mask(bottom face)!=0 || mask(top face)!=0) && ...
        //   and
        //    2) one of the closests face(s) must be valid:
        //        mask(closest face)!=0   ( if more than one closest face then at least one should be !=0 )

                if( !useOpt )
      	{
        	  intSerialArray maskd,maskc;
        	  maskc.redim(I1r.getLength(),I2r.getLength(),I3r.getLength());
        	  for( int r3=1; r3<rf[2]; r3++ )
        	  {
          	    for( int r2=1; r2<rf[1]; r2++ )
          	    {
            	      for( int r1=1; r1<rf[0]; r1++ )
            	      {
		// set maskd = true if there is at least one face in each direction that is valid:
            		maskd=(( (mask(I1r   ,I2r+r2,I3r+r3)!=0) || (mask(I1r+rf[0],I2r+r2   ,I3r+r3   )!=0) ) &&
                   		       ( (mask(I1r+r1,I2r   ,I3r+r3)!=0) || (mask(I1r+r1   ,I2r+rf[1],I3r+r3   )!=0) ) &&
                   		       ( (mask(I1r+r1,I2r+r2,I3r   )!=0) || (mask(I1r+r1   ,I2r+r2   ,I3r+rf[2])!=0) ));

		// set maskc = true if there is a valid point on one of the closest face(s)
            		const int r1d = min(r1,rf[0]-r1); // distance to nearest face along axis1
            		const int r2d = min(r2,rf[1]-r2);
            		const int r3d = min(r3,rf[2]-r3);
            		maskc=0;
            		if( r1d <= min(r2d,r3d) )
            		{ // there is a nearest face along axis1
              		  if( r1<=rf[0]/2 )
                		    maskc=maskc || (mask(I1r      ,I2r+r2,I3r+r3)!=0);
              		  if( r1>=rf[0]/2 )
                		    maskc=maskc || (mask(I1r+rf[0],I2r+r2,I3r+r3)!=0);
            		}
            		if( r2d <=min(r1d,r3d) )
            		{ // there is a nearest face along axis2
              		  if( r2<=rf[1]/2 )
                		    maskc=maskc || (mask(I1r+r1,I2r+rf[1],I3r+r3)!=0);
              		  if( r2>=rf[1]/2 )
                		    maskc=maskc || (mask(I1r+r1,I2r+rf[1],I3r+r3)!=0);
            		}
            		if( r3d <=min(r1d,r2d) )
            		{ // there is a nearest face along axis3
              		  if( r3<=rf[2]/2 )
                		    maskc=maskc || (mask(I1r+r1,I2r+r2,I3r+rf[2])!=0);
              		  if( r3>=rf[2]/2 )
                		    maskc=maskc || (mask(I1r+r1,I2r+r2,I3r+rf[2])!=0);
            		}
            		where( !(maskd && maskc) )
            		{
              		  mask(I1r+r1,I2r+r2,I3r+r3)=0;
            		}
            	      
            	      }
          	    }
        	  }
      	}
      	else 
      	{
	  // ----- opt version with bug fix-----
          //   *** I think that this can be re-worked to be faster ***
                    int maskd,maskc;
                    FOR_3R()
        	  {
          	    for( int r3=1; r3<rf[2]; r3++ )
          	    {
                  	      const int r3d = min(r3,rf[2]-r3); // distance to nearest face along axis3
            	      for( int r2=1; r2<rf[1]; r2++ )
            	      {
            		const int r2d = min(r2,rf[1]-r2); // distance to nearest face along axis2
            		for( int r1=1; r1<rf[0]; r1++ )
            		{

		  // set maskd = true if there is at least one face in each direction that is valid:
              		  maskd=(( (MASK(i1r   ,i2r+r2,i3r+r3)!=0) || (MASK(i1r+rf[0],i2r+r2   ,i3r+r3   )!=0) ) &&
                   			 ( (MASK(i1r+r1,i2r   ,i3r+r3)!=0) || (MASK(i1r+r1   ,i2r+rf[1],i3r+r3   )!=0) ) && 
                   			 ( (MASK(i1r+r1,i2r+r2,i3r   )!=0) || (MASK(i1r+r1   ,i2r+r2   ,i3r+rf[2])!=0) ));
            		
              		  if( !maskd )
              		  {
                		    MASK(i1r+r1,i2r+r2,i3r+r3)=0;
                                        continue;
              		  }
                  // Some adjacent faces are valid -- we need to check more carefully

              		  const int r1d = min(r1,rf[0]-r1); // distance to nearest face along axis1
		  // set maskc = true if there is a valid point on one of the closest face(s)
              		  maskc=0;
              		  if( r1d <= min(r2d,r3d) )
              		  { // there is a nearest face along axis1
                		    if( r1<=rf[0]/2 )
                  		      maskc=maskc || (MASK(i1r      ,i2r+r2,i3r+r3)!=0);
                		    if( r1>=rf[0]/2 )
                  		      maskc=maskc || (MASK(i1r+rf[0],i2r+r2,i3r+r3)!=0);
                                        if( maskc ) continue;
              		  } 
              		  if( r2d <=min(r1d,r3d) )
              		  { // there is a nearest face along axis2
                		    if( r2<=rf[1]/2 )
                  		      maskc=maskc || (MASK(i1r+r1,i2r      ,i3r+r3)!=0);   // bug found here *wdh* 030913
                		    if( r2>=rf[1]/2 )
                  		      maskc=maskc || (MASK(i1r+r1,i2r+rf[1],i3r+r3)!=0);
                                        if( maskc ) continue;
              		  }
              		  if( r3d <=min(r1d,r2d) )
              		  { // there is a nearest face along axis3
                		    if( r3<=rf[2]/2 )
                  		      maskc=maskc || (MASK(i1r+r1,i2r+r2,i3r      )!=0);  // bug found here *wdh* 030913
                		    if( r3>=rf[2]/2 )
                  		      maskc=maskc || (MASK(i1r+r1,i2r+r2,i3r+rf[2])!=0);
                                        if( maskc ) continue;
              		  }
              		  if( !maskc )
              		  {
                		    MASK(i1r+r1,i2r+r2,i3r+r3)=0;
              		  }
            		}
            	      }
          	    }
        	  } // end for

                } // end opt version
      	
            }
            

        }
        else // ************************OLD WAY**************************************************
        {
      // old way: refinement region matches coarse grid
            Ivr[axis3]=Range(Ivr[axis3].getBase(),Ivr[axis3].getBound()+rf[axis3],rf[axis3]);
            Ivb[axis3]=Range(Ivb[axis3].getBase(),Ivb[axis3].getBound()+1);
            where( (maskb(I1b,I2b,I3b)==0)   || (maskb(I1b+1,I2b  ,I3b)==0) ||
           	     (maskb(I1b,I2b+1,I3b)==0) || (maskb(I1b+1,I2b+1,I3b)==0) )
            {
      	for( int r2=1; r2<rf[1]; r2++ )
      	{
        	  for( int r1=1; r1<rf[0]; r1++ )
        	  {
          	    mask(I1r+r1,I2r+r2,I3r)=0;
        	  }
      	}
            }
            Ivr[axis3]=Range(Ivr[axis3].getBase(),Ivr[axis3].getBound()-rf[axis3],rf[axis3]);
            Ivb[axis3]=Range(Ivb[axis3].getBase(),Ivb[axis3].getBound()-1);

            Ivr[axis1]=Range(Ivr[axis1].getBase(),Ivr[axis1].getBound()+rf[axis1],rf[axis1]);
            Ivb[axis1]=Range(Ivb[axis1].getBase(),Ivb[axis1].getBound()+1);
            where( (maskb(I1b,I2b,I3b)==0)   || (maskb(I1b  ,I2b+1,I3b)==0) ||
           	     (maskb(I1b,I2b,I3b+1)==0) || (maskb(I1b  ,I2b+1,I3b+1)==0) )
            {
      	for( int r3=1; r3<rf[2]; r3++ )
      	{
        	  for( int r2=1; r2<rf[1]; r2++ )
        	  {
          	    mask(I1r,I2r+r2,I3r+r3)=0;
        	  }
      	}
            }
            Ivr[axis1]=Range(Ivr[axis1].getBase(),Ivr[axis1].getBound()-rf[axis1],rf[axis1]);
            Ivb[axis1]=Range(Ivb[axis1].getBase(),Ivb[axis1].getBound()-1);

            Ivr[axis2]=Range(Ivr[axis2].getBase(),Ivr[axis2].getBound()+rf[axis2],rf[axis2]);
            Ivb[axis2]=Range(Ivb[axis2].getBase(),Ivb[axis2].getBound()+1);
            where( (maskb(I1b,I2b,I3b)==0)   || (maskb(I1b+1,I2b,I3b)==0) ||
           	     (maskb(I1b,I2b,I3b+1)==0) || (maskb(I1b+1,I2b,I3b+1)==0) )
            {
      	for( int r3=1; r3<rf[2]; r3++ )
      	{
        	  for( int r1=1; r1<rf[0]; r1++ )
        	  {
          	    mask(I1r+r1,I2r,I3r+r3)=0;
        	  }
      	}
            }
            Ivr[axis2]=Range(Ivr[axis2].getBase(),Ivr[axis2].getBound()-rf[axis2],rf[axis2]);
            Ivb[axis2]=Range(Ivb[axis2].getBase(),Ivb[axis2].getBound()-1);


            where( (maskb(I1b,I2b,I3b)==0)       || (maskb(I1b+1,I2b  ,I3b  )==0) ||
           	     (maskb(I1b  ,I2b+1,I3b  )==0) || (maskb(I1b+1,I2b+1,I3b  )==0) ||
           	     (maskb(I1b  ,I2b  ,I3b+1)==0) || (maskb(I1b+1,I2b  ,I3b+1)==0) ||
           	     (maskb(I1b  ,I2b+1,I3b+1)==0) || (maskb(I1b+1,I2b+1,I3b+1)==0) )
            {
      	for( int r3=1; r3<rf[2]; r3++ )
      	{
        	  for( int r2=1; r2<rf[1]; r2++ )
        	  {
          	    for( int r1=1; r1<rf[0]; r1++ )
          	    {
            	      mask(I1r+r1,I2r+r2,I3r+r3)=0;
          	    }
        	  }
      	}
            }
        }

    }
    return 0;
}



int Ogen::
setRefinementMaskFace(intSerialArray & mask,
                                            int side, int axis, 
                                            int numberOfDimensions, int rf[3],
                                            Index & I1r, Index & I2r, Index & I3r,
                                            const intSerialArray & mask00, 
                                            const intSerialArray & mask10,
                                            const intSerialArray & mask01,
                                            const intSerialArray & mask11)
// =============================================================================================
// /Description:
//    Assign the mask values for a refinement grid on a face (side,axis).
// /side,axis (input): defines the face to assign. For a 2d grid axis should equal 2.
// /mask00,mask01,maks10,mask11 (input): values of (mask==0) on the corners of the face.
// =============================================================================================
{
    assert( rf[0]==rf[1] && ( numberOfDimensions==2 || rf[1]==rf[2]) );
          	    
    int rStart[3]= {0,0,0}; //
    int rEnd[3]= {0,0,0}; //

    for( int dir=0; dir<numberOfDimensions; dir++ )
    {
        rStart[dir]=1;
        rEnd[dir]=rf[dir]-1;
    }
    rStart[axis]=0;
    rEnd[axis]=0;

    int rv[3], &r1=rv[0], &r2=rv[1], &r3=rv[2];

    const int dir1 = (axis+1) % 3;
    const int dir2 = (axis+2) % 3;
    
    where( mask00+mask10+mask01+mask11 > 1 )
    {
    // more than one corner of this cell has a zero -- all points in cell are zeroed.
        for( r3=rStart[2]; r3<=rEnd[2]; r3++ )
        for( r2=rStart[1]; r2<=rEnd[1]; r2++ )
        for( r1=rStart[0]; r1<=rEnd[0]; r1++ )
        {
            mask(I1r+r1,I2r+r2,I3r+r3)=0;
        }
    }
    elsewhere( mask00 )  // only lower left corner is zero
    {
        for( r3=rStart[2]; r3<=rEnd[2]; r3++ )
        for( r2=rStart[1]; r2<=rEnd[1]; r2++ )
        for( r1=rStart[0]; r1<=rEnd[0]; r1++ )
        {
            if( rv[dir1]+rv[dir2] < rf[0] ) 
      	mask(I1r+r1,I2r+r2,I3r+r3)=0;
        }
    }
    elsewhere( mask10 ) // only lower right corner is zero.
    {
        for( r3=rStart[2]; r3<=rEnd[2]; r3++ )
        for( r2=rStart[1]; r2<=rEnd[1]; r2++ )
        for( r1=rStart[0]; r1<=rEnd[0]; r1++ )
        {
            if( rv[dir1]>rv[dir2] )
      	mask(I1r+r1,I2r+r2,I3r+r3)=0;
        }
    }
    elsewhere( mask01 )
    {
        for( r3=rStart[2]; r3<=rEnd[2]; r3++ )
        for( r2=rStart[1]; r2<=rEnd[1]; r2++ )
        for( r1=rStart[0]; r1<=rEnd[0]; r1++ )
        {
            if( rv[dir1]<rv[dir2] )
      	mask(I1r+r1,I2r+r2,I3r+r3)=0;
        }
    }
    elsewhere( mask11 )
    {
        for( r3=rStart[2]; r3<=rEnd[2]; r3++ )
        for( r2=rStart[1]; r2<=rEnd[1]; r2++ )
        for( r1=rStart[0]; r1<=rEnd[0]; r1++ )
        {
            if( rv[dir1]+rv[dir2] > rf[0] )
      	mask(I1r+r1,I2r+r2,I3r+r3)=0;
        }
    }

    return 0;
}



int Ogen::
checkRefinementInterpolationNew( CompositeGrid & cg )
// ==========================================================================================
// Here we check the new interpolation added for refinements
// ==========================================================================================
{
    int numberOfErrors=0;
    const int numberOfDimensions=cg.numberOfDimensions();
    
    Range Rx=numberOfDimensions;
    realArray x(1,3),r(1,3);
    const real eps = REAL_EPSILON*100.;
    
    int iv0[3]={0,0,0};
    real dx[3]={0.,0.,0.},xab[2][3]={0.,0.,0.,0.,0.,0.};
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
        if( grid!=cg.baseGridNumber(grid) )
        {
      // this is not a base grid 
            if( cg.numberOfInterpolationPoints(grid)>0 )
            {
                MappedGrid & mg = cg[grid];
      	const IntegerArray & extended = extendedGridIndexRange(mg);
      	const realArray & vertex = mg.vertex();
      	if( mg.isRectangular() )
      	{
        	  mg.getRectangularGridParameters( dx, xab );
        	  iv0[0]=mg.gridIndexRange(0,0);
        	  iv0[1]=mg.gridIndexRange(0,1);
        	  iv0[2]=mg.gridIndexRange(0,2);
      	}
      	const intArray & ip = cg.interpolationPoint[grid];
      	const intArray & interpoleeGrid = cg.interpoleeGrid[grid];
      	const realArray & ci = cg.interpolationCoordinates[grid];
      	
      	int i;
                int i3=extended(Start,axis3);
      	for( i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
      	{
        	  int interpolee=interpoleeGrid(i);
        	  if( interpolee<0 || interpolee>cg.numberOfComponentGrids() )
        	  {
                        numberOfErrors++;
          	    printf("checkRefinementInterpolation: ERROR: grid=%i, i=%i, invalid interpolee=%i\n",grid,i,interpolee);
        	  }
                    int axis;
        	  for( axis=0; axis<numberOfDimensions; axis++ )
        	  {
          	    if( ip(i,axis)<extended(Start,axis) || ip(i,axis)>extended(End,axis) )
          	    {
                            numberOfErrors++;
            	      printf("checkRefinementInterpolation: ERROR: grid=%i, i=%i, invalid interpolation point\n",grid,i);
          	    }
                        if( ci(i,axis)<-.25 || ci(i,axis)>1.25 )
          	    {
                            numberOfErrors++;
            	      printf("checkRefinementInterpolation: ERROR: grid=%i, i=%i, invalid interpolation coords\n",grid,i);
          	    }
          	    
                        if( mg.isRectangular() )
          	    {
            	      if( numberOfDimensions==2 )
            	      {
            		x(0,0)=VERTEX0(ip(i,0),ip(i,1),i3);
            		x(0,1)=VERTEX1(ip(i,0),ip(i,1),i3);
            	      }
            	      else
            	      {
            		x(0,0)=VERTEX0(ip(i,0),ip(i,1),ip(i,2));
            		x(0,1)=VERTEX1(ip(i,0),ip(i,1),ip(i,2));
            		x(0,2)=VERTEX2(ip(i,0),ip(i,1),ip(i,2));
            	      }
          	    }
          	    else
          	    {
            	      if( numberOfDimensions==2 )
            		x(0,axis)=vertex(ip(i,0),ip(i,1),i3,axis);
            	      else
            		x(0,axis)=vertex(ip(i,0),ip(i,1),ip(i,2),axis);
          	    }
          	    
        	  }
        	  r=-1.;
        	  cg[interpolee].mapping().inverseMap(x,r);
                      
        	  if( max(fabs(r(0,Rx)-ci(i,Rx)))> eps )
        	  {
                        numberOfErrors++;
          	    printf("checkRefinementInterpolation: ERROR: grid=%i, i=%i, incorrect interpolation coords. "
                                      " ip=(%i,%i,%i) interpolee=%i\n",grid,i,ip(i,0),ip(i,1),(numberOfDimensions==2 ? 0 : ip(i,2)),
                                          interpolee);
                        printf("   interpolationCoordinates=(%9.3e,%9.3e,%9.3e) inverseMap=(%9.3e,%9.3e,%9.3e) \n",
               		   ci(i,0),ci(i,1),(numberOfDimensions==2 ? 0. : ci(i,2)),r(0,0),r(0,1),r(0,2));
        	  }
      	}
            }
        }
    }

    return numberOfErrors;
}

#undef VERTEX0
#undef VERTEX1
#undef VERTEX2



