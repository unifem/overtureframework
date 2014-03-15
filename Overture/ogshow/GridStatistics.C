// This file automatically generated from GridStatistics.bC with bpp.
#include "GridStatistics.h"
#include "ParallelUtility.h"


// Declare and define base and bounds, perform loop
#define  FOR_3D(i1,i2,i3,I1,I2,I3)int I1Base=I1.getBase(), I2Base=I2.getBase(), I3Base=I3.getBase();int I1Bound=I1.getBound(), I2Bound=I2.getBound(), I3Bound=I3.getBound();for( i3=I3Base; i3<=I3Bound; i3++ )  for( i2=I2Base; i2<=I2Bound; i2++ )  for( i1=I1Base; i1<=I1Bound; i1++ )

// Perform loop
#define  FOR_3(i1,i2,i3,I1,I2,I3)I1Base=I1.getBase(), I2Base=I2.getBase(), I3Base=I3.getBase();I1Bound=I1.getBound(), I2Bound=I2.getBound(), I3Bound=I3.getBound();for( i3=I3Base; i3<=I3Bound; i3++ )  for( i2=I2Base; i2<=I2Bound; i2++ )  for( i1=I1Base; i1<=I1Bound; i1++ )

static inline 
double
tetVolume6(real *p1, real*p2, real *p3, real *p4 )
/// Return 6 times the volume of the tetrahedra
///
/// (p2-p1)x(p3-p1) points in the direction of p4 ( p1,p2,p3 are counter clockwise viewed from p4 )
/// 6 vol = (p4-p1) . ( (p2-p1)x(p3-p1) )
{
    return  ( (p4[0]-p1[0])*( (p2[1]-p1[1])*(p3[2]-p1[2]) - (p2[2]-p1[2])*(p3[1]-p1[1]) ) -
          	    (p4[1]-p1[1])*( (p2[0]-p1[0])*(p3[2]-p1[2]) - (p2[2]-p1[2])*(p3[0]-p1[0]) ) +
          	    (p4[2]-p1[2])*( (p2[0]-p1[0])*(p3[1]-p1[1]) - (p2[1]-p1[1])*(p3[0]-p1[0]) ) ) ;
        	  
}


static inline 
real
hexVolume( real *v000, real *v100, real *v010, real *v110, real *v001, real *v101, 
                      real *v011, real *v111 )
// =====================================================================================================
/// \brief Return the volume of the hexahedron defined by the vertices v000,v100,... 
// =====================================================================================================
{
    return (tetVolume6(v000,v100,v010, v001)+
        	  tetVolume6(v110,v010,v100, v111)+
        	  tetVolume6(v101,v001,v111, v100)+
        	  tetVolume6(v011,v111,v001, v010)+
        	  tetVolume6(v100,v010,v001, v111))/6.;
}



int GridStatistics::
checkForNegativeVolumes(GridCollection & gc, int numberOfGhost /* =0 */, FILE *file /* =stdout */ )
// ================================================================================
/// \brief Check a grid for negative volumes and return the number of negative volumes found
/// \param numberOfGhost (input) : include this many ghost points.
/// \param file (input) : output messages to this file if file!=NULL
// ================================================================================
{
    if( file!=NULL )
    {
        fPrintF(file,"--- checkForNegativeVolumes ---\n");
    }

    int numberOfNegativeVolumes=0;
    for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
    {
        numberOfNegativeVolumes += checkForNegativeVolumes(gc[grid],numberOfGhost,file,grid);
    }

    return numberOfNegativeVolumes;
}


int GridStatistics::
checkForNegativeVolumes(MappedGrid & mg, int numberOfGhost /* =0 */, FILE *file /* =stdout */, int grid /* =0 */ )
// ================================================================================
//
/// \brief Check a grid for negative volumes and return the number of negative volumes found
/// \param numberOfGhost (input) : include this many ghost points.
/// \param file (input) : output messages to this file if file!=NULL
/// \param grid (input) : grid number (if part of a GridCollection)
//
// ================================================================================
{
    real volMin=REAL_MAX,volAve=0.,volMax=0.;
    int numberOfVolumes=0;
    int numberOfNegativeVolumes=0;
    int numberOfGridPoints=0;
    
    const int domainDimension = mg.domainDimension();
    const int rangeDimension =  mg.rangeDimension();
    Mapping & mapping = mg.mapping().getMapping();
    const IntegerArray & gid = mg.gridIndexRange();
    
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    ::getIndex(gid,I1,I2,I3,numberOfGhost); 

    if( mg.getGridType()==GenericGrid::structuredGrid )
    {
        int axis;
        for( axis=0; axis<domainDimension; axis++ )
            Iv[axis]=Range(Iv[axis].getBase(),Iv[axis].getBound()-1);  // only compute at cell centres.

        const real orientation = mapping.getSignForJacobian();

        const bool isRectangular=mg.isRectangular();
        if( !isRectangular )
        {
            mg.update(MappedGrid::THEvertex );

            bool ok=true;
            #ifdef USE_PPP
                realSerialArray vertex; getLocalArrayWithGhostBoundaries(mg.vertex(),vertex);
                ok = ParallelUtility::getLocalArrayBounds(mg.vertex(),vertex,I1,I2,I3);
            #else
                const realSerialArray & vertex = mg.vertex();
            #endif

            const real *vertexp = vertex.Array_Descriptor.Array_View_Pointer3;
            const int vertexDim0=vertex.getRawDataSize(0);
            const int vertexDim1=vertex.getRawDataSize(1);
            const int vertexDim2=vertex.getRawDataSize(2);
#define x(i0,i1,i2,i3) vertexp[i0+vertexDim0*(i1+vertexDim1*(i2+vertexDim2*(i3)))]

            int i1,i2,i3;
            if( domainDimension==2 && rangeDimension==2 )
            {
                if( ok )
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3)
        	  {
	    // area of a polygon = (1/2) sum{ x_i y_{i+1} - x_{i+1} y_i
          	    real vol = ( x(i1  ,i2  ,i3,0)*x(i1+1,i2  ,i3,1)-x(i1+1,i2  ,i3,0)*x(i1  ,i2  ,i3,1) +  // (i1,i2)
                   			 x(i1+1,i2  ,i3,0)*x(i1+1,i2+1,i3,1)-x(i1+1,i2+1,i3,0)*x(i1+1,i2  ,i3,1) + 
                   			 x(i1+1,i2+1,i3,0)*x(i1  ,i2+1,i3,1)-x(i1  ,i2+1,i3,0)*x(i1+1,i2+1,i3,1) + 
                   			 x(i1  ,i2+1,i3,0)*x(i1  ,i2  ,i3,1)-x(i1  ,i2  ,i3,0)*x(i1  ,i2+1,i3,1) );
        
          	    vol*=.5*orientation;
          	    volMin=min(volMin,vol);
          	    volMax=max(volMax,vol);
          	    volAve+=vol;
          	    numberOfVolumes++;

          	    numberOfGridPoints++;
          	    if( vol<=0. ) numberOfNegativeVolumes++;
            
        	  }
      	}
      	volMin=ParallelUtility::getMinValue(volMin);
      	volMax=ParallelUtility::getMaxValue(volMax);
                volAve=ParallelUtility::getSum(volAve);
                numberOfGridPoints=ParallelUtility::getSum(numberOfGridPoints);
      	
      	volAve/=max(1,numberOfGridPoints);
        
            }
            else if( domainDimension==2 && rangeDimension==3 )
            {
      	printF("printGridStatistics: not implemented yet for surface grids.\n");
            }
            else if( domainDimension==3 )
            {
	// ************ 3D ***********************

      	real v[2][2][2][3];
      	if( ok )
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3)
        	  {
          	    for( int axis=0; axis<3; axis++ )
          	    {
            	      v[0][0][0][axis]=x(i1  ,i2  ,i3  ,axis);
            	      v[1][0][0][axis]=x(i1+1,i2  ,i3  ,axis);
            	      v[0][1][0][axis]=x(i1  ,i2+1,i3  ,axis);
            	      v[1][1][0][axis]=x(i1+1,i2+1,i3  ,axis);
            	      v[0][0][1][axis]=x(i1  ,i2  ,i3+1,axis);
            	      v[1][0][1][axis]=x(i1+1,i2  ,i3+1,axis);
            	      v[0][1][1][axis]=x(i1  ,i2+1,i3+1,axis);
            	      v[1][1][1][axis]=x(i1+1,i2+1,i3+1,axis);
          	    }

          	    real vol=hexVolume(v[0][0][0],v[1][0][0],v[0][1][0],v[1][1][0],
                         			       v[0][0][1],v[1][0][1],v[0][1][1],v[1][1][1])*orientation;
            
          	    volMin=min(volMin,vol);
          	    volMax=max(volMax,vol);
          	    volAve+=vol;
          	    numberOfVolumes++;
        	  
          	    numberOfGridPoints++;
          	    if( vol<=0. ) numberOfNegativeVolumes++;
            
        	  }
      	}
      	
      	volMin=ParallelUtility::getMinValue(volMin);
      	volMax=ParallelUtility::getMaxValue(volMax);
                volAve=ParallelUtility::getSum(volAve);
                numberOfGridPoints=ParallelUtility::getSum(numberOfGridPoints);
      	volAve/=max(1,numberOfGridPoints);

            }
        
            if( file!=NULL )
            {
	// -- output some results --
      	if( numberOfNegativeVolumes==0 )
        	  fPrintF(file," grid %i, name=%s. (curvilinear) has NO negative volumes, %i grid-pts (including %i ghost points)\n"
              		  ,grid,(const char*)mg.getName(),numberOfGridPoints,numberOfGhost);
      	else
        	  fPrintF(file,
              		  "ERROR: grid %i, name=%s. (curvilinear) has %i negative volumes, %i grid-pts (including %i ghost points)\n"
              		  "     : cell volumes: [%8.2e,%8.2e,%8.2e] [min,ave,max] \n"
              		  ,grid,(const char*)mg.getName(),numberOfNegativeVolumes,
              		  numberOfGridPoints,numberOfGhost,volMin,volAve,volMax);
            }
        } // end if !rectangular

        
    }
    else // unstructured grid
    {
        printF("GridStatistics::checkForNegativeVolumes: WARNING: Not implemented yet for unstructured grids\n");

    }

    return numberOfNegativeVolumes;

}





void GridStatistics::
printGridStatistics(CompositeGrid & cg, FILE *file /* =stdout */ )
// ================================================================================
/// \brief Output grid statistics for a CompositeGrid. 
// ================================================================================
{
    GridCollection & gc = cg;
    printGridStatistics(gc,file);
}

void GridStatistics::
printGridStatistics(GridCollection & gc, FILE *file /* =stdout */ )
// ================================================================================
/// \brief Output grid statistics for a GridCollection. 
// ================================================================================
{
    int totalNumberOfGridPoints=0;
    int totalNumberOfGridIndexRangePoints=0;
    int totalNumberOfVolumes=0;
    real volMin=REAL_MAX,volAve=0.,volMax=0.;

  // printf(" printGridStatistics: gc.getClassName()=%s\n",(const char*)gc.getClassName());
    
    bool isCompositeGrid = gc.getClassName()=="CompositeGrid"; 
    int totalNumberOfInterpolationPoints=0;
    if( isCompositeGrid )
    {
        CompositeGrid & cg = (CompositeGrid&)gc;
        totalNumberOfInterpolationPoints= sum(cg.numberOfInterpolationPoints);
    }

    int ipar[10]; 
    real rpar[10];
    for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
    {
        MappedGrid & mg = gc[grid];
        printGridStatistics( mg,file,grid,ipar,rpar );

        const IntegerArray & gid = mg.gridIndexRange();
        const IntegerArray & dim = mg.dimension();
        totalNumberOfGridPoints+=(dim(1,0)-dim(0,0)+1)*(dim(1,1)-dim(0,1)+1)*(dim(1,2)-dim(0,2)+1);
        totalNumberOfGridIndexRangePoints+=(gid(1,0)-gid(0,0)+1)*(gid(1,1)-gid(0,1)+1)*(gid(1,2)-gid(0,2)+1);
        
        int numberOfVolumes=ipar[0];
        totalNumberOfVolumes+=numberOfVolumes;
        
        volMin=min(volMin,rpar[0]);
        volAve+=rpar[1]*numberOfVolumes;
        volMax=max(volMax,rpar[2]);
    }
    volAve/=max(1,totalNumberOfVolumes);
    fPrintF(file,"\n"
                    " ******************Grid Statistics Summary****************************\n"
                    "   number of grids =%i \n"
        	  "   Total number of grid points %i (gridIndexRange), %i (dimension) <<<\n"
                    "   Total number of interpolation points %i \n"
                    "   cell volumes: [%8.2e,%8.2e,%8.2e]  [min,ave,max] \n"
                    " *********************************************************************\n"
                    ,gc.numberOfComponentGrids(),
        	  totalNumberOfGridIndexRangePoints,totalNumberOfGridPoints,totalNumberOfInterpolationPoints,
                      volMin,volAve,volMax);
}


void GridStatistics::
printGridStatistics(MappedGrid & mg, FILE *file /* =stdout */, 
                                        int grid /* =0 */, int *ipar /* =NULL */, real *rpar /* =NULL */ )
// =================================================================================================
/// \brief Print statistics about a component grid
///
/// Optionally return:
/// 
///      if( ipar!=NULL )
///      {
///        ipar[0]=numberOfVolumes;
///      }
///      if( rpar!=NULL )
///      {
///        rpar[0]=volMin;
///        rpar[1]=volAve;
///        rpar[2]=volMax;
///      }
// =================================================================================================
{
    real volMin=REAL_MAX,volAve=0.,volMax=0.;
    real dsMin[3], dsAve[3], dsMax[3];
    int numberOfVolumes=0;
    int numberOfNegativeVolumes=0;
    int numberOfGridPoints=0;
    
    const int domainDimension = mg.domainDimension();
    const int rangeDimension =  mg.rangeDimension();
    Mapping & mapping = mg.mapping().getMapping();
    const IntegerArray & gid = mg.gridIndexRange();
    const IntegerArray & bc = mg.boundaryCondition();
    const IntegerArray & isPeriodic = mg.isPeriodic();
    const IntegerArray & share = mg.sharedBoundaryFlag();
    const IntegerArray & dw = mg.discretizationWidth();
    
    getGridSpacing(mg,dsMin,dsAve,dsMax );
    

    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    ::getIndex(gid,I1,I2,I3); 

    if( mg.getGridType()==GenericGrid::structuredGrid )
    {
        int axis;
        for( axis=0; axis<domainDimension; axis++ )
            Iv[axis]=Range(Iv[axis].getBase(),Iv[axis].getBound()-1);  // only compute at cell centres.

        const real orientation = mapping.getSignForJacobian();

        const bool isRectangular=mg.isRectangular();
        if( !isRectangular )
        {
            mg.update(MappedGrid::THEvertex );

            bool ok=true;
            #ifdef USE_PPP
                realSerialArray vertex; getLocalArrayWithGhostBoundaries(mg.vertex(),vertex);
                ok = ParallelUtility::getLocalArrayBounds(mg.vertex(),vertex,I1,I2,I3);
            #else
                const realSerialArray & vertex = mg.vertex();
            #endif

            const real *vertexp = vertex.Array_Descriptor.Array_View_Pointer3;
            const int vertexDim0=vertex.getRawDataSize(0);
            const int vertexDim1=vertex.getRawDataSize(1);
            const int vertexDim2=vertex.getRawDataSize(2);
#define x(i0,i1,i2,i3) vertexp[i0+vertexDim0*(i1+vertexDim1*(i2+vertexDim2*(i3)))]

            int i1,i2,i3;
            if( domainDimension==2 && rangeDimension==2 )
            {
                if( ok )
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3)
        	  {
	    // area of a polygon = (1/2) sum{ x_i y_{i+1} - x_{i+1} y_i
          	    real vol = ( x(i1  ,i2  ,i3,0)*x(i1+1,i2  ,i3,1)-x(i1+1,i2  ,i3,0)*x(i1  ,i2  ,i3,1) +  // (i1,i2)
                   			 x(i1+1,i2  ,i3,0)*x(i1+1,i2+1,i3,1)-x(i1+1,i2+1,i3,0)*x(i1+1,i2  ,i3,1) + 
                   			 x(i1+1,i2+1,i3,0)*x(i1  ,i2+1,i3,1)-x(i1  ,i2+1,i3,0)*x(i1+1,i2+1,i3,1) + 
                   			 x(i1  ,i2+1,i3,0)*x(i1  ,i2  ,i3,1)-x(i1  ,i2  ,i3,0)*x(i1  ,i2+1,i3,1) );
        
          	    vol*=.5*orientation;
          	    volMin=min(volMin,vol);
          	    volMax=max(volMax,vol);
          	    volAve+=vol;
          	    numberOfVolumes++;

          	    numberOfGridPoints++;
          	    if( vol<=0. ) numberOfNegativeVolumes++;
            
        	  }
      	}
      	volMin=ParallelUtility::getMinValue(volMin);
      	volMax=ParallelUtility::getMaxValue(volMax);
                volAve=ParallelUtility::getSum(volAve);
                numberOfGridPoints=ParallelUtility::getSum(numberOfGridPoints);
      	
      	volAve/=max(1,numberOfGridPoints);
        
            }
            else if( domainDimension==2 && rangeDimension==3 )
            {
      	printF("printGridStatistics: not implemented yet for surface grids.\n");
            }
            else if( domainDimension==3 )
            {
	// ************ 3D ***********************

      	real v[2][2][2][3];
      	if( ok )
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3)
        	  {
          	    for( int axis=0; axis<3; axis++ )
          	    {
            	      v[0][0][0][axis]=x(i1  ,i2  ,i3  ,axis);
            	      v[1][0][0][axis]=x(i1+1,i2  ,i3  ,axis);
            	      v[0][1][0][axis]=x(i1  ,i2+1,i3  ,axis);
            	      v[1][1][0][axis]=x(i1+1,i2+1,i3  ,axis);
            	      v[0][0][1][axis]=x(i1  ,i2  ,i3+1,axis);
            	      v[1][0][1][axis]=x(i1+1,i2  ,i3+1,axis);
            	      v[0][1][1][axis]=x(i1  ,i2+1,i3+1,axis);
            	      v[1][1][1][axis]=x(i1+1,i2+1,i3+1,axis);
          	    }

          	    real vol=hexVolume(v[0][0][0],v[1][0][0],v[0][1][0],v[1][1][0],
                         			       v[0][0][1],v[1][0][1],v[0][1][1],v[1][1][1])*orientation;
            
          	    volMin=min(volMin,vol);
          	    volMax=max(volMax,vol);
          	    volAve+=vol;
          	    numberOfVolumes++;
        	  
          	    numberOfGridPoints++;
          	    if( vol<=0. ) numberOfNegativeVolumes++;
            
        	  }
      	}
      	
      	volMin=ParallelUtility::getMinValue(volMin);
      	volMax=ParallelUtility::getMaxValue(volMax);
                volAve=ParallelUtility::getSum(volAve);
                numberOfGridPoints=ParallelUtility::getSum(numberOfGridPoints);
      	volAve/=max(1,numberOfGridPoints);

            }
        
            if( domainDimension==3 )
            {
      	fPrintF(file,
            		" -----------------------------------------------------------------------\n"
            		"         Grid Statistics for grid %i, name=%s. (curvilinear) (mapping=%s)\n"
            		" grid lines  : [%i:%i,%i:%i,%i:%i], total points = %i\n"
                                " ghost points: [%i:%i,%i:%i,%i:%i]\n"
            		" cell volumes: [%8.2e,%8.2e,%8.2e] [min,ave,max] \n"
            		" grid spacing: [%8.2e,%8.2e,%8.2e] [min,ave,max] (r1) \n"
            		"             : [%8.2e,%8.2e,%8.2e] [min,ave,max] (r2) \n"
            		"             : [%8.2e,%8.2e,%8.2e] [min,ave,max] (r3) \n"
            		" number of negative volumes = %i \n"
            		,grid,(const char*)mg.getName(),(const char*)mg.mapping().getClassName(),
            		gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
            		numberOfGridPoints,
                                mg.numberOfGhostPoints(0,0),mg.numberOfGhostPoints(1,0),
                                mg.numberOfGhostPoints(0,1),mg.numberOfGhostPoints(1,1),
                                mg.numberOfGhostPoints(0,2),mg.numberOfGhostPoints(1,2),
            		volMin,volAve,volMax,
            		dsMin[0],dsAve[0],dsMax[0],
            		dsMin[1],dsAve[1],dsMax[1],
            		dsMin[2],dsAve[2],dsMax[2],
                                numberOfNegativeVolumes);
            }
            else
            {
      	fPrintF(file,
            		" -----------------------------------------------------------------------\n"
            		"         Grid Statistics for grid %i, name=%s. (curvilinear) (mapping=%s)\n"
            		" grid lines  : [%i:%i,%i:%i], total points = %i\n"
                                " ghost points: [%i:%i,%i:%i]\n"
            		" cell volumes: [%8.2e,%8.2e,%8.2e] [min,ave,max] \n"
            		" grid spacing: [%8.2e,%8.2e,%8.2e] [min,ave,max] (r1) \n"
            		"             : [%8.2e,%8.2e,%8.2e] [min,ave,max] (r2) \n"
            		" number of negative volumes = %i \n"
            		,grid,(const char*)mg.getName(),(const char*)mg.mapping().getClassName(),
            		gid(0,0),gid(1,0),gid(0,1),gid(1,1),
            		numberOfGridPoints,
                                mg.numberOfGhostPoints(0,0),mg.numberOfGhostPoints(1,0),
                                mg.numberOfGhostPoints(0,1),mg.numberOfGhostPoints(1,1),
            		volMin,volAve,volMax,
            		dsMin[0],dsAve[0],dsMax[0],
            		dsMin[1],dsAve[1],dsMax[1],
            		numberOfNegativeVolumes);
            }
            if( rangeDimension==2 )
            {
      	fPrintF(file,
            		" bc = [%i,%i, %i,%i] share=[%i,%i, %i,%i] isPeriodic=[%i,%i] dw=%i\n"
            		" -----------------------------------------------------------------------\n",
            		bc(0,0),bc(1,0),bc(0,1),bc(1,1),share(0,0),share(1,0),share(0,1),share(1,1),
                                isPeriodic(0),isPeriodic(1),dw(0) );
            }
            else if( rangeDimension==3 )
            {
      	fPrintF(file,
                                " bc = [%i,%i, %i,%i, %i,%i] share=[%i,%i, %i,%i, %i,%i] isPeriodic=[%i,%i,%i] dw=%i\n"
            		" -----------------------------------------------------------------------\n",
                                bc(0,0),bc(1,0),bc(0,1),bc(1,1),bc(0,2),bc(1,2),
                                share(0,0),share(1,0),share(0,1),share(1,1),share(0,2),share(1,2),
                                isPeriodic(0),isPeriodic(1),isPeriodic(2),dw(0) );
            }
            

        } 
        else // rectangular
        {
            real dx[3];
            mg.getDeltaX(dx);
            if( rangeDimension<3 ) dx[2]=1.;
            if( rangeDimension<2 ) dx[1]=1.;
            numberOfGridPoints=(gid(1,0)-gid(0,0)+1)*(gid(1,1)-gid(0,1)+1)*(gid(1,2)-gid(0,2)+1);
            volMin=volAve=volMax=dx[0]*dx[1]*dx[2];
            
            if( domainDimension==1 )
      	numberOfVolumes=(gid(1,0)-gid(0,0));
            else if( domainDimension==2 )
      	numberOfVolumes=(gid(1,0)-gid(0,0))*(gid(1,1)-gid(0,1));
            else
      	numberOfVolumes=(gid(1,0)-gid(0,0))*(gid(1,1)-gid(0,1))*(gid(1,2)-gid(0,2));

            if( domainDimension==3 )
            {
      	fPrintF(file,
            		" -----------------------------------------------------------------------\n"
            		"         Grid Statistics for grid %i, name=%s. (rectangular) (mapping=%s)\n"
            		" grid lines  : [%i:%i,%i:%i,%i:%i], total points = %i\n"
                                " ghost points: [%i:%i,%i:%i,%i:%i]\n"
            		" cell volume : %8.2e \n"
            		" grid spacing: [%8.2e,%8.2e,%8.2e] [dx,dy,dz]\n"
                                " bc = [%i,%i, %i,%i, %i,%i] share=[%i,%i, %i,%i, %i,%i] isPeriodic=[%i,%i,%i] dw=%i\n"
            		" -----------------------------------------------------------------------\n"
            		,grid,(const char*)mg.getName(),(const char*)mg.mapping().getClassName(),
            		gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
            		numberOfGridPoints,
                                mg.numberOfGhostPoints(0,0),mg.numberOfGhostPoints(1,0),
                                mg.numberOfGhostPoints(0,1),mg.numberOfGhostPoints(1,1),
                                mg.numberOfGhostPoints(0,2),mg.numberOfGhostPoints(1,2),
            		volMin,
            		dx[0],dx[1],dx[2],
                                bc(0,0),bc(1,0),bc(0,1),bc(1,1),bc(0,2),bc(1,2),
                                share(0,0),share(1,0),share(0,1),share(1,1),share(0,2),share(1,2),
                                isPeriodic(0),isPeriodic(1),isPeriodic(2), dw(0));
            }
            else
            {
      	fPrintF(file,
            		" -----------------------------------------------------------------------\n"
            		"         Grid Statistics for grid %i, name=%s. (rectangular) (mapping=%s)\n"
            		" grid lines  : [%i:%i,%i:%i], total points = %i\n"
                                " ghost points: [%i:%i,%i:%i]\n"
            		" cell volume : %8.2e \n"
            		" grid spacing: [%8.2e,%8.2e] [dx,dy]\n"
                                " bc = [%i,%i, %i,%i] share=[%i,%i, %i,%i] isPeriodic=[%i,%i] dw=%i\n"
            		" -----------------------------------------------------------------------\n"
            		,grid,(const char*)mg.getName(),(const char*)mg.mapping().getClassName(),
            		gid(0,0),gid(1,0),gid(0,1),gid(1,1),
            		numberOfGridPoints,
                                mg.numberOfGhostPoints(0,0),mg.numberOfGhostPoints(1,0),
                                mg.numberOfGhostPoints(0,1),mg.numberOfGhostPoints(1,1),
            		volMin,
            		dx[0],dx[1],
                                bc(0,0),bc(1,0),bc(0,1),bc(1,1),share(0,0),share(1,0),share(0,1),share(1,1),
                                isPeriodic(0),isPeriodic(1), dw(0));
            }
            
        }
    
        if( ipar!=NULL )
        {
            ipar[0]=numberOfVolumes;
        }
        if( rpar!=NULL )
        {
            rpar[0]=volMin;
            rpar[1]=volAve;
            rpar[2]=volMax;
        }
        
    }
    else // unstructured grid
    {
        fPrintF(file,
          	    " -----------------------------------------------------------------------\n"
                        "         Grid Statistics for grid %i, name=%s. (unstructured) \n"
                        "            Not implemented yet. \n"
                        " -----------------------------------------------------------------------\n"
                        ,grid,(const char*)mg.getName());
    }


}

#undef x

void GridStatistics::
getNumberOfPoints(MappedGrid & mg, int & numberOfPoints, MaskOptionEnum maskOption /* =ignoreMask */ )
{
    printF("GridStatistics:getNumberOfPoints:WARNING: not implemented yet\n");
    numberOfPoints=1;
}

void GridStatistics::
getNumberOfPoints(GridCollection & gc, int & totalNumberOfGridPoints, MaskOptionEnum maskOption /* =ignoreMask */ )
// ================================================================================
/// \brief Determine the total number of grid points in a GricCollection.
///
///  \param gc (input) 
///  \param totalNumberOfGridPoints (output): total number of grid points in gc
///  \param maskOption (input) : indicates which points to include.
/// ================================================================================
{
    if( maskOption!=ignoreMask )
    {
        printF("GridStatistics:getNumberOfPoints(gc):WARNING: maskOption!=ignoreMask not implemented yet\n");
    }
    

    totalNumberOfGridPoints=0;
    for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
    {
        MappedGrid & mg = gc[grid];
        const IntegerArray & dim = mg.dimension();
        totalNumberOfGridPoints+=(dim(1,0)-dim(0,0)+1)*(dim(1,1)-dim(0,1)+1)*(dim(1,2)-dim(0,2)+1);
    }

}

void GridStatistics::
getGridSpacingAndNumberOfPoints(MappedGrid & mg, real dsMin[3], real dsAve[3], real dsMax[3],
                                                                int & numberOfPoints, MaskOptionEnum maskOption /* =ignoreMask */ )
// =================================================================================================
/// \brief Compute the min, average and maximum grid spacing along each axis and the number of grid points.
///
///  \param mg (input): gris to use
///  \param dsMin[axis],dsAve[axis],dsMax[axis] : output, min, average and max grid spacing 
///                 along axis=0,1,..,nd-1 (nd=number of dimensions)
///  \param numberOfPoints (output) : output, number of grid points 
///  \param maskOption (input) : indicates which points to include.
// =================================================================================================
{
    
    getGridSpacing(mg,dsMin,dsAve,dsMax,maskOption );
    getNumberOfPoints(mg,numberOfPoints,maskOption);
}

void GridStatistics::
getGridSpacing(MappedGrid & mg, real dsMin[3], real dsAve[3], real dsMax[3], 
                              MaskOptionEnum maskOption /* =ignoreMask */ )
// =================================================================================================
/// \brief Compute the min, average and maximum grid spacing along each axis
// =================================================================================================
{
    if( maskOption!=ignoreMask )
    {
        printF("GridStatistics:getGridSpacing:WARNING: not implemented yet for maskOption!=ignoreMask \n");
    }
    
    const int domainDimension = mg.domainDimension();
    const int rangeDimension =  mg.rangeDimension();
    const IntegerArray & gid = mg.gridIndexRange();
    int axis;
    for( axis=0; axis<domainDimension; axis++ )
    {
        dsMin[axis]=REAL_MAX;
        dsAve[axis]=0;
        dsMax[axis]=0.;
    }
    for( axis=domainDimension; axis<3; axis++ )
    { // default values for invalid dimensions
        dsMin[axis]=1.;
        dsAve[axis]=1;
        dsMax[axis]=1.;
    }
    

    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    ::getIndex(gid,I1,I2,I3); 


    if( mg.getGridType()==GenericGrid::structuredGrid )
    {
        const bool isRectangular=mg.isRectangular();
        if( !isRectangular )
        {
            mg.update( MappedGrid::THEvertex );
            
            bool ok=true;
            #ifdef USE_PPP
                realSerialArray vertex; getLocalArrayWithGhostBoundaries(mg.vertex(),vertex);
                ok = ParallelUtility::getLocalArrayBounds(mg.vertex(),vertex,I1,I2,I3);
            #else
                const realSerialArray & vertex = mg.vertex();
            #endif

            int I1Base=I1.getBase(), I2Base=I2.getBase(), I3Base=I3.getBase();
            int I1Bound=I1.getBound(), I2Bound=I2.getBound(), I3Bound=I3.getBound();
            if( !ok )
            { // there are no points on this processor
      	I1Bound=I1Base-1; I2Bound=I2Base-1; I3Bound=I3Base-1;
            }

            const real *vertexp = vertex.Array_Descriptor.Array_View_Pointer3;
            const int vertexDim0=vertex.getRawDataSize(0);
            const int vertexDim1=vertex.getRawDataSize(1);
            const int vertexDim2=vertex.getRawDataSize(2);
#undef x
#define x(i0,i1,i2,i3) vertexp[i0+vertexDim0*(i1+vertexDim1*(i2+vertexDim2*(i3)))]

            int i1,i2,i3;
            if( domainDimension==2 && rangeDimension==2 )
            {
	// compute first grid line spacing
      	i3=gid(0,2);
      	int numDs=0;
      	for( i2=I2Base; i2<=I2Bound; i2++ )
      	for( i1=I1Base; i1< I1Bound; i1++ )
      	{
        	  real ds= sqrt( SQR(x(i1+1,i2,i3,0)-x(i1,i2,i3,0))+
                   			 SQR(x(i1+1,i2,i3,1)-x(i1,i2,i3,1)) );
        	  dsMin[0]=min(dsMin[0],ds);
        	  dsMax[0]=max(dsMax[0],ds);
        	  dsAve[0]+=ds;
        	  numDs++;
      	}
                dsMin[0]=ParallelUtility::getMinValue(dsMin[0]);
                dsMax[0]=ParallelUtility::getMaxValue(dsMax[0]);

                numDs=ParallelUtility::getSum(numDs);
      	dsAve[0]=ParallelUtility::getSum(dsAve[0]);
      	dsAve[0]/=max(1,numDs);

      	numDs=0;
      	for( i2=I2Base; i2< I2Bound; i2++ )
      	for( i1=I1Base; i1<=I1Bound; i1++ )
      	{
        	  real ds= sqrt( SQR(x(i1,i2+1,i3,0)-x(i1,i2,i3,0))+
                   			 SQR(x(i1,i2+1,i3,1)-x(i1,i2,i3,1)) );
        	  dsMin[1]=min(dsMin[1],ds);
        	  dsMax[1]=max(dsMax[1],ds);
        	  dsAve[1]+=ds;
        	  numDs++;
      	}
                dsMin[1]=ParallelUtility::getMinValue(dsMin[1]);
                dsMax[1]=ParallelUtility::getMaxValue(dsMax[1]);
                numDs=ParallelUtility::getSum(numDs);
      	dsAve[1]=ParallelUtility::getSum(dsAve[1]);
      	dsAve[1]/=max(1,numDs);
        
                dsMin[2]=dsMax[2]=dsAve[2]=1.;
      	
            }
            else if( domainDimension==2 && rangeDimension==3 )
            {
      	printF("GridStatistics::getGridSpacing: not implemented yet for surface grids.\n");
            }
            else if( domainDimension==3 )
            {
	// ************ 3D ***********************

      	int numDs;
                numDs=0;
      	for( i3=I3Base; i3<=I3Bound; i3++ ) 
      	for( i2=I2Base; i2<=I2Bound; i2++ ) 
                for( i1=I1Base; i1< I1Bound; i1++ )
      	{
        	  real ds= sqrt( SQR(x(i1+1,i2,i3,0)-x(i1,i2,i3,0))+
                   			 SQR(x(i1+1,i2,i3,1)-x(i1,i2,i3,1))+
                   			 SQR(x(i1+1,i2,i3,2)-x(i1,i2,i3,2)) );
        	  dsMin[0]=min(dsMin[0],ds);
        	  dsMax[0]=max(dsMax[0],ds);
        	  dsAve[0]+=ds;
        	  numDs++;
      	}
                dsMin[0]=ParallelUtility::getMinValue(dsMin[0]);
                dsMax[0]=ParallelUtility::getMaxValue(dsMax[0]);
                numDs=ParallelUtility::getSum(numDs);
      	dsAve[0]=ParallelUtility::getSum(dsAve[0]);
      	dsAve[0]/=max(1,numDs);

                numDs=0;
      	for( i3=I3Base; i3<=I3Bound; i3++ ) 
      	for( i2=I2Base; i2< I2Bound; i2++ ) 
                for( i1=I1Base; i1<=I1Bound; i1++ )
      	{
        	  real ds= sqrt( SQR(x(i1,i2+1,i3,0)-x(i1,i2,i3,0))+
                   			 SQR(x(i1,i2+1,i3,1)-x(i1,i2,i3,1))+
                   			 SQR(x(i1,i2+1,i3,2)-x(i1,i2,i3,2)) );
        	  dsMin[1]=min(dsMin[1],ds);
        	  dsMax[1]=max(dsMax[1],ds);
        	  dsAve[1]+=ds;
        	  numDs++;
      	}
                dsMin[1]=ParallelUtility::getMinValue(dsMin[1]);
                dsMax[1]=ParallelUtility::getMaxValue(dsMax[1]);
                numDs=ParallelUtility::getSum(numDs);
      	dsAve[1]=ParallelUtility::getSum(dsAve[1]);
      	dsAve[1]/=max(1,numDs);

                numDs=0;
      	for( i3=I3Base; i3< I3Bound; i3++ ) 
      	for( i2=I2Base; i2<=I2Bound; i2++ ) 
                for( i1=I1Base; i1<=I1Bound; i1++ )
      	{
        	  real ds= sqrt( SQR(x(i1,i2,i3+1,0)-x(i1,i2,i3,0))+
                   			 SQR(x(i1,i2,i3+1,1)-x(i1,i2,i3,1))+
                   			 SQR(x(i1,i2,i3+1,2)-x(i1,i2,i3,2)) );
        	  dsMin[2]=min(dsMin[2],ds);
        	  dsMax[2]=max(dsMax[2],ds);
        	  dsAve[2]+=ds;
        	  numDs++;
      	}
                dsMin[2]=ParallelUtility::getMinValue(dsMin[2]);
                dsMax[2]=ParallelUtility::getMaxValue(dsMax[2]);
                numDs=ParallelUtility::getSum(numDs);
      	dsAve[2]=ParallelUtility::getSum(dsAve[2]);
      	dsAve[2]/=max(1,numDs);
            }
        
        } 
        else // rectangular
        {
            real dx[3];
            mg.getDeltaX(dx);

            for( axis=0; axis<domainDimension; axis++ )
            {
      	dsMin[axis]=dsMax[axis]=dsAve[axis]=dx[axis];
            }
            
        }
        
    }
    else
    {
        printF("GridStatistics::getGridSpacing: not implemented yet for unstructured grids.\n");

        dsMin[0]=dsMax[0]=dsAve[0]=1.;
        dsMin[1]=dsMax[1]=dsAve[1]=1.;
        dsMin[2]=dsMax[2]=dsAve[2]=1.;

    }
    
}

