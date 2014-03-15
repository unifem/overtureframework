// This file automatically generated from markPointsNeeded.bC with bpp.
#include "Ogen.h"
#include "Overture.h"
#include "display.h"
#include "conversion.h"
#include "ParallelUtility.h"
#include "CanInterpolate.h"


static const int ISneededPoint = CompositeGrid::ISreservedBit2;  // from Cgsh.h

// Define a macro to index an A++ array with 3 dimensions *NOTE* a legal macro  --> #define MASK
// #define DEF_ARRAY_MACRO_3D(int,mask,MASK) //   int * mask ## p = mask.Array_Descriptor.Array_View_Pointer2;//   const int mask ## Dim0=mask.getRawDataSize(0);//   const int mask ## Dim1=mask.getRawDataSize(1);// #define MASK(i0,i1,i2) mask ## p[i0+mask ## Dim0*(i1+mask ## Dim1*(i2))]

// Macro to extract a local array with ghost boundaries
//  type = int/float/double/real
//  xd = distributed array
//  xs = serial array 
#ifdef USE_PPP
  #define GET_LOCAL(type,xd,xs)type ## SerialArray xs; getLocalArrayWithGhostBoundaries(xd,xs)
  #define GET_LOCAL_CONST(type,xd,xs)type ## SerialArray xs; getLocalArrayWithGhostBoundaries(xd,xs)
#else
  #define GET_LOCAL(type,xd,xs)type ## SerialArray & xs = xd
  #define GET_LOCAL_CONST(type,xd,xs)const type ## SerialArray & xs = xd
#endif

#define  FOR_3(i1,i2,i3,I1,I2,I3)I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();for( i3=I3Base; i3<=I3Bound; i3++ )  for( i2=I2Base; i2<=I2Bound; i2++ )  for( i1=I1Base; i1<=I1Bound; i1++ )

#define  FOR_3D(i1,i2,i3,I1,I2,I3)int I1Base,I2Base,I3Base;int I1Bound,I2Bound,I3Bound;I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();for( i3=I3Base; i3<=I3Bound; i3++ )  for( i2=I2Base; i2<=I2Bound; i2++ )  for( i1=I1Base; i1<=I1Bound; i1++ )

#define FOR_3IJD(i1,i2,i3,I1,I2,I3,j1,j2,j3,J1,J2,J3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); int J1Base =J1.getBase(),   J2Base =J2.getBase(),  J3Base =J3.getBase();  for(i3=I3Base,j3=J3Base; i3<=I3Bound; i3++,j3++) for(i2=I2Base,j2=J2Base; i2<=I2Bound; i2++,j2++) for(i1=I1Base,j1=J1Base; i1<=I1Bound; i1++,j1++)


// // =====================================================================================
// //  *OLD* Fill in the arrays that hold the donor data 
// //
// // index[0..2] : grid point on the donor mesh 
// // 
// // =====================================================================================
// #beginMacro fillDonorData(index,piab)

//  // We will check the donorMask on the following processor:
//  int donorProcessor = donorMask.Array_Descriptor.findProcNum( index ); 

//  if( i>=maxNumToSend )
//  {
//    printf("markNeeded:WARNING: Increasing donorInfo size for grid=%i maxNumToSend=%i\n",grid,maxNumToSend);
//    maxNumToSend=maxNumToSend*2;
//    donorInfo.resize(maxNumToSend,numDonorData);
//  }
// //  printf("markNeeded:Send: grid=%i donor=%i myid=%i donorProcessor=%i i=%i numToSend[dp]=%i "
// //         "[i1a,i1b][i2a,i2b]=[%i,%i][%i,%i]\n",
// //	grid,donor,myid,donorProcessor,i,numToSend[donorProcessor],piab[0],piab[1],piab[2],piab[3]);

//  donorInfo(i,0)=donorProcessor;
//  donorInfo(i,1)=donor;
//  for( int n=0; n<nd2; n++ )
//    donorInfo(i,2+n)=piab[n];

//  // numDonor(donorGrid,donorProcessor)++; // use a sparse array here
//  numToSend[donorProcessor]++; 
//  i++;
// #endMacro	  

static int numberOfIncreasingDonorMessage=0;

// ========================================================================================
//  Fill in the arrays that hold the donor data -- this macro is used by fillDonorDataMP
//
// index[0..2] : grid point on the donor mesh 
// 
// ========================================================================================

// ==========================================================================================
// This macro assigns donorProc(s1,s2,s3) to be the processor that owns the corner of the
//  donor stencil :  (iab(s1,0),iab(s2,1),iab(s3,2))
// ==========================================================================================

//============================================================================================
// Fill the donor data taking into account sending the info to different processors
// This macro assumes that the donor processor will only assign stencil points that actually
// belong on that processor.
//
// NOTES:
//   The donor stencil may live on a processor boundary. In this case we need to send the 
//  stencil to all the processors that own part of the stencil. There may be 1, 2, 4, or 8 
//  processors that we need to send information to. 
// 
//============================================================================================


int Ogen::
markPointsNeededForInterpolationNew( CompositeGrid & cg, const int & grid, const int & lowerOrUpper /* =-1 */ )
// =============================================================================================================
// /Description:
//    Mark points that are needed for interpolation. For each interpolation point that has discretization
//  points in it's stencil (or that has already been marked as needed) mark its donor points.
//
//             *** Parallel Version ***
// 
// /lowerOrUpper (input) : if -1 check grids of lower priority, if +1 check grids of higher priority
//
//
//  Steps:
//
//     (1) Make a list ipn[i] i=0,1,2,..  of interpolation points on cg[grid] that are needed-for-discretization
//         or needed-for-interpolation
//     (2) Determine the donor grid, donor stencils and donor processor (processor holding the donor mask values)
//         corresponding to each ipn[i]
//     (3) Send the donor stencils to the donor processor
//     (4) Mark the donor stencil mask values as needed-for-interpolation
// 
// =============================================================================================================
{
#ifndef USE_PPP
    printf("markPointsNeededForInterpolationNew:ERROR:This function should only be called in parallel!\n");
    Overture::abort("error");
    return 0;
#else

    const int np=Communication_Manager::numberOfProcessors();
    const MPI_Comm & OV_COMM = Overture::OV_COMM;
    
    const int numberOfBaseGrids=cg.numberOfBaseGrids();
    const int numberOfComponentGrids=cg.numberOfComponentGrids();

    const int numberOfDimensions = cg.numberOfDimensions();
    MappedGrid & c = cg[grid];
    intArray & maskgd = c.mask();
    intArray & inverseGridd = cg.inverseGrid[grid];
    realArray & rId = cg.inverseCoordinates[grid];
    RealArray & interpolationOverlap = cg.interpolationOverlap;

    GET_LOCAL(int,maskgd,maskg);
    GET_LOCAL(int,inverseGridd,inverseGridg);
    GET_LOCAL(real,rId,rI);
        

    Index I1,I2,I3;
    getIndex(c.extendedIndexRange(),I1,I2,I3); 

    bool ok=ParallelUtility::getLocalArrayBounds(maskgd,maskg,I1,I2,I3);

    int axis;
    int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
    int index[3] = {0,0,0}; //
    real rv[3]= {0.,0.,0.};
    int l =0;  // multigrid level
    
    int ivd[4]={0,0,0,0};  // 
    int pDonorProc[8];
    #define donorProc(s1,s2,s3) (pDonorProc[(s1)+2*((s2)+2*(s3))])
  // -- temp: 
    int pDp[8];
    #define dp(s1,s2,s3) (pDp[(s1)+2*((s2)+2*(s3))])

    const int nd2=numberOfDimensions*2;
    int piab[6], pjab[6];
#define iab(side,axis) piab[(side)+2*(axis)]
#define jab(side,axis) pjab[(side)+2*(axis)]
    int * maskgp = maskg.Array_Descriptor.Array_View_Pointer2;
    const int maskgDim0=maskg.getRawDataSize(0);
    const int maskgDim1=maskg.getRawDataSize(1);
#define MASK(i0,i1,i2) maskgp[i0+maskgDim0*(i1+maskgDim1*(i2))]
    int * inverseGridgp = inverseGridg.Array_Descriptor.Array_View_Pointer2;
    const int inverseGridgDim0=inverseGridg.getRawDataSize(0);
    const int inverseGridgDim1=inverseGridg.getRawDataSize(1);
#define INVERSEGRID(i0,i1,i2) inverseGridgp[i0+inverseGridgDim0*(i1+inverseGridgDim1*(i2))]


    int *numToSend = new int [np];
    int *numReceived = new int [np];
    for( int p=0; p<np; p++ )
    {
        numToSend[p]=0;
    }
    

    IntegerArray donorInfo; // buffer to hold donor info
    
    int i=0; // counts donor points 
    if( ok )
    {
        int maxNumToSend = 0;  // guess how many values we will send
        if( numberOfDimensions==2 )
        {
            maxNumToSend=100 + (I1.getLength()+I2.getLength())*2;
        }
        else
        {
            maxNumToSend=500 + (I1.getLength()*I2.getLength() + 
                                                    I1.getLength()*I3.getLength() + 
                                                    I2.getLength()*I3.getLength() )*2;
        }
        
        const int numDonorData=2+numberOfDimensions*2;
        donorInfo.redim(maxNumToSend,numDonorData);
        

//     // build index boxes that cover the local array for each grid.
//     IndexBox *localGridIndexBox = new IndexBox[cg.numberOfComponentGrids];
//     for( int gg=0; gg<cg.numberOfComponentGrids; gg++ )
//     {
//       CopyArray::getLocalArrayBox( myid, cg[gg].mask(), localGridIndexBox[gg] );
//     }
        


        FOR_3D(i1,i2,i3,I1,I2,I3)
        {
            if( MASK(i1,i2,i3) & MappedGrid::ISinterpolationPoint  ) 
            {
      	const int donor=INVERSEGRID(i1,i2,i3);
      	if( donor==grid && allowHangingInterpolation )
        	  continue;
        	  
      	assert( donor>=0 && donor<numberOfBaseGrids ); // && donor!=grid );

      	if( (lowerOrUpper==-1 && donor<=grid) || (lowerOrUpper==+1 && donor>=grid) ) // ******* move this up ******
      	{
        	  int m=MASK(i1,i2,i3);
        	  if( m & ISneededPoint  ||                  // a needed point
            	      isNeededForDiscretization(c,iv) )      // or it is needed for discretisation...
        	  {

	    // mark the interpolee points as needed

          	    real ov = interpolationOverlap(axis1,grid,donor,l);  // *wdh 00015
          	    const bool explicitInterp = !cg.interpolationIsImplicit(grid,donor,l);

          	    if( m & MappedGrid::USESbackupRules )
          	    {
            	      const int backup=backupValues[grid](i1,i2,i3);
            	      if( backup<0 )
            		ov-=1.;  // implicit interpolation
            	      else
            		ov-=.5;  // lower order interpolation ** may not be right if order reduced by more than 1??
          	    }
            	      
          	    real oneSidedShift = 2.*ov+1.;
          	    if( explicitInterp )
          	    { // for explicit interp, ov is increased by 1 in both directions -- for one sided we only
	      // need to increase by 1 total:
            	      oneSidedShift = 2.*ov; // *wdh* 040718 I think this is correct (cf cicbug.cmd)
          	    }

          	    MappedGrid & g2=cg[donor];
            //  iab(0:1,0:2) : donor stencil
            //  jab(0:1,0:2) : donor stencil that crosses periodic boundaries
          	    for( axis=0; axis<3; axis++ )
          	    {
            	      if( axis<cg.numberOfDimensions() )
            	      {
            		rv[axis] = rI(i1,i2,i3,axis)/g2.gridSpacing(axis) + g2.indexRange(0,axis);
            		iab(0,axis)=Integer(floor(rv[axis]-ov - (g2.isCellCentered(axis) ? .5 : 0. )));
            		iab(1,axis)=Integer(floor(rv[axis]+ov + (g2.isCellCentered(axis) ? .5 : 1. )));
            		if( !g2.isPeriodic(axis) )
            		{
              		  if( iab(0,axis) < g2.extendedIndexRange(0,axis) )
              		  {
                		    if( g2.boundaryCondition(Start,axis)>0 ) 
                		    {
		      // Point is close to a BC side. One-sided interpolation used.
                  		      iab(0,axis) = g2.extendedIndexRange(0,axis);
                  		      iab(1,axis) = min(g2.extendedIndexRange(1,axis),   // *wdh* added min 040327
                              					Integer(floor(iab(0,axis) + oneSidedShift )));
                		    } // end if
                		    else
                		    {
                  		      printF("Ogen:markPtsNeeded:WARNING:grid=%i, donor=%i, (i1,i2,i3)=(%i,%i,%i), g2.bc=%i"
                       			     "  interpolee location is invalid. (will shift to boundary)\n"
                       			     "  ov=%8.2e (orig=%8.2e) rI(%i)=%8.2e, r/dr=%8.2e -> iab=%i < g2.extendedIndexRange=%i\n",
                       			     grid,donor,i1,i2,i3,g2.boundaryCondition(Start,axis),
                                                          ov,interpolationOverlap(axis1,grid,donor,l),
                       			     axis,rI(i1,i2,i3,axis),rv[axis],iab(0,axis),g2.extendedIndexRange(0,axis));
                  		      fprintf(plogFile,"Ogen:markPtsNeeded:WARNING:grid=%i, donor=%i, (i1,i2,i3)=(%i,%i,%i), g2.bc=%i"
                       			     "  interpolee location is invalid. (will shift to boundary)\n"
                       			     "  ov=%8.2e (orig=%8.2e) rI(%i)=%8.2e, r=%8.2e -> iab=%i < g2.extendedIndexRange=%i\n",
                       			     grid,donor,i1,i2,i3,g2.boundaryCondition(Start,axis),
                                                          ov,interpolationOverlap(axis1,grid,donor,l),
                       			     axis,rI(i1,i2,i3,axis),rv[axis],iab(0,axis),g2.extendedIndexRange(0,axis));
                  		      iab(0,axis) = g2.extendedIndexRange(0,axis);
                  		      iab(1,axis) =  min(g2.extendedIndexRange(1,axis),   // *wdh* added min 040327
                               					 Integer(floor(iab(0,axis) + oneSidedShift )));
                		    }
              		  }
              		  if( iab(1,axis) > g2.extendedIndexRange(1,axis) )
              		  {
                		    if( g2.boundaryCondition(End,axis)>0 ) 
                		    {
		      // Point is close to a BC side. One-sided interpolation used.
                  		      iab(1,axis) = g2.extendedIndexRange(1,axis);
                  		      iab(0,axis) = max(g2.extendedIndexRange(0,axis),   // *wdh* added max 040327
                              					Integer(floor(iab(1,axis) - oneSidedShift )));
                		    } // end if
                		    else
                		    {
                  		      iab(1,axis) = g2.extendedIndexRange(1,axis);
                  		      iab(0,axis) = max(g2.extendedIndexRange(0,axis),   // *wdh* added max 040327
                              					Integer(floor(iab(1,axis) - oneSidedShift )));

                  		      printF("Ogen:markPtsNeeded:WARNING:grid=%i, donor=%i, (i1,i2,i3)=(%i,%i,%i), g2.bc=%i"
                       			     "  interpolee location is invalid. (will shift to boundary)\n"
                       			     "  ov=%8.2e (orig=%8.2e) rI(%i)=%8.2e, r/dr=%8.2e -> iab=%i > g2.extendedIndexRange=%i"
                                                          " new bounds=[%i,%i]\n",
                       			     grid,donor,i1,i2,i3,g2.boundaryCondition(End,axis),
                                                          ov,interpolationOverlap(axis1,grid,donor,l),
                       			     axis,rI(i1,i2,i3,axis),rv[axis],iab(1,axis),g2.extendedIndexRange(1,axis),
                                                          iab(0,axis),iab(1,axis));
                  		      fprintf(plogFile,"Ogen:markPtsNeeded:WARNING:grid=%i, donor=%i, (i1,i2,i3)=(%i,%i,%i), g2.bc=%i"
                       			     "  interpolee location is invalid. (will shift to boundary)\n"
                       			     "  ov=%8.2e (orig=%8.2e) rI(%i)=%8.2e, r/dr=%8.2e -> iab=%i > g2.extendedIndexRange=%i"
                                                          " new bounds=[%i,%i]\n",
                       			     grid,donor,i1,i2,i3,g2.boundaryCondition(End,axis),
                                                          ov,interpolationOverlap(axis1,grid,donor,l),
                       			     axis,rI(i1,i2,i3,axis),rv[axis],iab(1,axis),g2.extendedIndexRange(1,axis),
                                                          iab(0,axis),iab(1,axis));
                		    }
              		  }
                		    
              		  jab(0,axis)=iab(0,axis);
              		  jab(1,axis)=iab(1,axis);
              		  
            		} // end if  !g2.isPeriodic(axis)
            		else
            		{ // periodic: 
              		  jab(0,axis)=max(iab(0,axis),g2.extendedIndexRange(0,axis));
              		  jab(1,axis)=min(iab(1,axis),g2.extendedIndexRange(1,axis));
            		}
            	      }
            	      else 
            	      { // axis >= numberOfDimensions: 
            		iab(0,axis) = jab(0,axis)=g2.extendedIndexRange(0,axis);
            		iab(1,axis) = jab(1,axis)=g2.extendedIndexRange(1,axis);
            	      } // end if, end for_1
          	    }

                        if( debug & 8 )
          	    {
            	      fprintf(plogFile,"Ogen:markPtsNeeded:grid=%i, donor=%i, (i1,i2,i3)=(%i,%i,%i), "
                                            "r=[%6.3f,%6.3f,%6.3f], iab=[%i,%i][%i,%i][%i,%i]\n",
                  		      grid,donor,i1,i2,i3,rv[0],rv[1],rv[2],iab(0,0),iab(1,0),iab(0,1),iab(1,1),iab(0,2),iab(1,2));
          	    }
          	    



	    // Mark interpolee points on donor as needed for interpolation.
	    // note that iab could go outside the extendedIndexRange on periodic edges so we need jab.

//            if( jab(0,axis1)<g2.extendedIndexRange(0,axis1) )  // for debugging
//  	  {
//  	    printf("markPointsNeededForInterpolation:ERROR: jab(0,0)=%i but g2.extendedIndexRange(0,0)=%i\n"
//                     " g2.isPeriodic(0)=%i g2.boundaryCondition(Start,0)=%i \n",
//                     jab(0,0),g2.extendedIndexRange(0,0),g2.isPeriodic(0),g2.boundaryCondition(Start,0));
//  	  }


          	    const intArray & donorMask = cg[donor].mask();

// 	    if( false )
// 	    {
            	      
// 	      for( int axis=0; axis<numberOfDimensions; axis++ )
// 	      { // index = index of closest grid point on the donor grid
// 		// index[axis] = int( rv[axis]/g2.gridSpacing(axis)+g2.gridIndexRange(0,axis) +.5 );
// 		index[axis] = (jab(0,axis)+jab(1,axis))/2;
// 	      }

// 	      fillDonorData(index,pjab);  // macro
// 	    }

          // 	    fillDonorDataMP(jab,pjab); // macro 
          // We will check the donorMask on the following processor:
          // getDonorProcessor( 0,0,0,donorProc );
                      ivd[0]=iab(0,0), ivd[1]=iab(0,1), ivd[2]=iab(0,2); 
                      donorProc(0,0,0) = donorMask.Array_Descriptor.findProcNum( ivd );
          // getDonorProcessor( 1,1,1,donorProc );
                      ivd[0]=iab(1,0), ivd[1]=iab(1,1), ivd[2]=iab(1,2); 
                      donorProc(1,1,1) = donorMask.Array_Descriptor.findProcNum( ivd );
                    if( donorProc(0,0,0)==donorProc(1,1,1) )
                    {
            // donor cell lives on 1 processor only 
            // fillDonorDataProc(donorProc(0,0,0),pjab);
                          int donorProcessor = donorProc(0,0,0);
                          if( i>=maxNumToSend )
                          {
                              if( numberOfIncreasingDonorMessage<2 )
                              {
                                  numberOfIncreasingDonorMessage++;
                                  printf("markNeeded:WARNING: Increasing donorInfo size for grid=%i maxNumToSend=%i\n",grid,maxNumToSend);
                              }
                              else if( numberOfIncreasingDonorMessage==2 )
                              {
                                  numberOfIncreasingDonorMessage++;
                                  printf("markNeeded:INFO: Too many: Increasing donorInfo size messages, I will not print anymore\n");
                              }
                              maxNumToSend=maxNumToSend*2;
                              donorInfo.resize(maxNumToSend,numDonorData);
                          }
            //  printf("markNeeded:Send: grid=%i donor=%i myid=%i donorProcessor=%i i=%i numToSend[dp]=%i "
            //         "[i1a,i1b][i2a,i2b]=[%i,%i][%i,%i]\n",
            //	grid,donor,myid,donorProcessor,i,numToSend[donorProcessor],pjab[0],pjab[1],pjab[2],pjab[3]);
             // we could check for duplicate donor data which might occur if one grid is much finer than the other
                          donorInfo(i,0)=donorProcessor;
                          donorInfo(i,1)=donor;
                          for( int n=0; n<nd2; n++ )
                              donorInfo(i,2+n)=pjab[n];
             // numDonor(donorGrid,donorProcessor)++; // use a sparse array here
                          numToSend[donorProcessor]++; 
                          i++;
                    }
                    else
                    {
          //   printf(" markPointsNeeded: myid=%i : donor stencil spans processors, grid=%i, donor=%i pt i=%i\n",myid,
          // 	 grid,donor,i);
            // getDonorProcessor( 1,0,0,donorProc );
                          ivd[0]=iab(1,0), ivd[1]=iab(0,1), ivd[2]=iab(0,2); 
                          donorProc(1,0,0) = donorMask.Array_Descriptor.findProcNum( ivd );
            // we could avoid the next call in 2d in some cases. Do this for now 
            // getDonorProcessor( 0,1,0,donorProc );
                          ivd[0]=iab(0,0), ivd[1]=iab(1,1), ivd[2]=iab(0,2); 
                          donorProc(0,1,0) = donorMask.Array_Descriptor.findProcNum( ivd );
                        if( numberOfDimensions==2 )
                        {  
                            donorProc(1,1,0)=donorProc(1,1,1);
                            int s1b = donorProc(0,0,0)==donorProc(1,0,0) ? 0 : 1;
                            int s2b = donorProc(0,0,0)==donorProc(0,1,0) ? 0 : 1;
                            for( int s2=0; s2<=s2b; s2++ )for( int s1=0; s1<=s1b; s1++ )
                            { // we fill 1, 2, or 4 times
                // --- double check that we have the correct donorProc --
                                if( true )
                                {
                // 	getDonorProcessor( s1,s2,0,dp );
                                  ivd[0]=iab(s1,0), ivd[1]=iab(s2,1), ivd[2]=iab(0,2); 
                                  dp(s1,s2,0) = donorMask.Array_Descriptor.findProcNum( ivd );
                          	assert( dp(s1,s2,0)==donorProc(s1,s2,0) );
                                }
                // fillDonorDataProc(donorProc(s1,s2,0),pjab);
                                  int donorProcessor = donorProc(s1,s2,0);
                                  if( i>=maxNumToSend )
                                  {
                                      if( numberOfIncreasingDonorMessage<2 )
                                      {
                                          numberOfIncreasingDonorMessage++;
                                          printf("markNeeded:WARNING: Increasing donorInfo size for grid=%i maxNumToSend=%i\n",grid,maxNumToSend);
                                      }
                                      else if( numberOfIncreasingDonorMessage==2 )
                                      {
                                          numberOfIncreasingDonorMessage++;
                                          printf("markNeeded:INFO: Too many: Increasing donorInfo size messages, I will not print anymore\n");
                                      }
                                      maxNumToSend=maxNumToSend*2;
                                      donorInfo.resize(maxNumToSend,numDonorData);
                                  }
                //  printf("markNeeded:Send: grid=%i donor=%i myid=%i donorProcessor=%i i=%i numToSend[dp]=%i "
                //         "[i1a,i1b][i2a,i2b]=[%i,%i][%i,%i]\n",
                //	grid,donor,myid,donorProcessor,i,numToSend[donorProcessor],pjab[0],pjab[1],pjab[2],pjab[3]);
                 // we could check for duplicate donor data which might occur if one grid is much finer than the other
                                  donorInfo(i,0)=donorProcessor;
                                  donorInfo(i,1)=donor;
                                  for( int n=0; n<nd2; n++ )
                                      donorInfo(i,2+n)=pjab[n];
                 // numDonor(donorGrid,donorProcessor)++; // use a sparse array here
                                  numToSend[donorProcessor]++; 
                                  i++;
                            }
                        }
                        else
                        {
              // getDonorProcessor( 0,0,1,donorProc );
                              ivd[0]=iab(0,0), ivd[1]=iab(0,1), ivd[2]=iab(1,2); 
                              donorProc(0,0,1) = donorMask.Array_Descriptor.findProcNum( ivd );
                            donorProc(1,1,0)= donorProc(0,0,0)==donorProc(1,0,0) ? donorProc(0,1,0) : donorProc(1,0,0);
                            donorProc(1,0,1)= donorProc(0,0,0)==donorProc(1,0,0) ? donorProc(0,0,1) : donorProc(1,0,0);
                            donorProc(0,1,1)= donorProc(0,0,0)==donorProc(0,1,0) ? donorProc(0,0,1) : donorProc(0,1,0);
                            const int s1b = donorProc(0,0,0)==donorProc(1,0,0) ? 0 : 1;
                            const int s2b = donorProc(0,0,0)==donorProc(0,1,0) ? 0 : 1;
                            const int s3b = donorProc(0,0,0)==donorProc(0,0,1) ? 0 : 1;
                            for( int s3=0; s3<=s3b; s3++ )for( int s2=0; s2<=s2b; s2++ )for( int s1=0; s1<=s1b; s1++ )
                            {
                                if( s1+s2+s3 == 2 )
                                {
                // 	getDonorProcessor( s1,s2,s3,donorProc );
                                  ivd[0]=iab(s1,0), ivd[1]=iab(s2,1), ivd[2]=iab(s3,2); 
                                  donorProc(s1,s2,s3) = donorMask.Array_Descriptor.findProcNum( ivd );
                                }
                            }
                            for( int s3=0; s3<=s3b; s3++ )for( int s2=0; s2<=s2b; s2++ )for( int s1=0; s1<=s1b; s1++ )
                            { // we fill 1, 2, 4 or 8 times
                // --- double check that we have the correct donorProc --
                                if( true )
                                {
                // 	getDonorProcessor( s1,s2,s3,dp );
                                  ivd[0]=iab(s1,0), ivd[1]=iab(s2,1), ivd[2]=iab(s3,2); 
                                  dp(s1,s2,s3) = donorMask.Array_Descriptor.findProcNum( ivd );
                          	assert( dp(s1,s2,s3)==donorProc(s1,s2,s3) );
                                }
                // fillDonorDataProc(donorProc(s1,s2,s3),pjab);
                                  int donorProcessor = donorProc(s1,s2,s3);
                                  if( i>=maxNumToSend )
                                  {
                                      if( numberOfIncreasingDonorMessage<2 )
                                      {
                                          numberOfIncreasingDonorMessage++;
                                          printf("markNeeded:WARNING: Increasing donorInfo size for grid=%i maxNumToSend=%i\n",grid,maxNumToSend);
                                      }
                                      else if( numberOfIncreasingDonorMessage==2 )
                                      {
                                          numberOfIncreasingDonorMessage++;
                                          printf("markNeeded:INFO: Too many: Increasing donorInfo size messages, I will not print anymore\n");
                                      }
                                      maxNumToSend=maxNumToSend*2;
                                      donorInfo.resize(maxNumToSend,numDonorData);
                                  }
                //  printf("markNeeded:Send: grid=%i donor=%i myid=%i donorProcessor=%i i=%i numToSend[dp]=%i "
                //         "[i1a,i1b][i2a,i2b]=[%i,%i][%i,%i]\n",
                //	grid,donor,myid,donorProcessor,i,numToSend[donorProcessor],pjab[0],pjab[1],pjab[2],pjab[3]);
                 // we could check for duplicate donor data which might occur if one grid is much finer than the other
                                  donorInfo(i,0)=donorProcessor;
                                  donorInfo(i,1)=donor;
                                  for( int n=0; n<nd2; n++ )
                                      donorInfo(i,2+n)=pjab[n];
                 // numDonor(donorGrid,donorProcessor)++; // use a sparse array here
                                  numToSend[donorProcessor]++; 
                                  i++;
                            }
                        }
                    }
          	    

	    // we need to mark the periodic images that lie inside the grid
            // (mark pts across processor boundaries ? <- may not be necessary since these may not be checked ?)

          	    if( g2.isPeriodic(axis1) || g2.isPeriodic(axis2) || (cg.numberOfDimensions()>2 && g2.isPeriodic(axis3)) )
          	    {
            	      bool needToMark=false;
            	      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
            	      {
            		if( g2.isPeriodic(axis) )
            		{
              		  if( iab(0,axis)<g2.indexRange(Start,axis) )
              		  {
                		    needToMark=true;
                		    const int ndr =g2.gridIndexRange(End,axis)-g2.gridIndexRange(Start,axis); 
                		    iab(0,axis)+=ndr;
                		    iab(1,axis)=min(iab(1,axis)+ndr,g2.dimension(End,axis));
              		  }
              		  else if( iab(1,axis)>g2.indexRange(End,axis) )
              		  {
                		    needToMark=true;
                		    const int ndr =g2.gridIndexRange(End,axis)-g2.gridIndexRange(Start,axis); 
                		    iab(0,axis)=max(iab(0,axis)-ndr,g2.dimension(Start,axis));
                		    iab(1,axis)-=ndr;
              		  }
            		}
            	      }
            	      if( needToMark )
            	      {
                // mark periodic images as needed
// 		if( false )
// 		{
// 		  for( int axis=0; axis<numberOfDimensions; axis++ )
// 		  { // index = index of closest grid point on the donor grid
// 		    index[axis] = (iab(0,axis)+iab(1,axis))/2;
// 		  }

// 		  fillDonorData(index,piab);  // macro
// 		}

                // 	        fillDonorDataMP(iab,piab); // macro 
                // We will check the donorMask on the following processor:
                // getDonorProcessor( 0,0,0,donorProc );
                                  ivd[0]=iab(0,0), ivd[1]=iab(0,1), ivd[2]=iab(0,2); 
                                  donorProc(0,0,0) = donorMask.Array_Descriptor.findProcNum( ivd );
                // getDonorProcessor( 1,1,1,donorProc );
                                  ivd[0]=iab(1,0), ivd[1]=iab(1,1), ivd[2]=iab(1,2); 
                                  donorProc(1,1,1) = donorMask.Array_Descriptor.findProcNum( ivd );
                                if( donorProc(0,0,0)==donorProc(1,1,1) )
                                {
                  // donor cell lives on 1 processor only 
                  // fillDonorDataProc(donorProc(0,0,0),piab);
                                      int donorProcessor = donorProc(0,0,0);
                                      if( i>=maxNumToSend )
                                      {
                                          if( numberOfIncreasingDonorMessage<2 )
                                          {
                                              numberOfIncreasingDonorMessage++;
                                              printf("markNeeded:WARNING: Increasing donorInfo size for grid=%i maxNumToSend=%i\n",grid,maxNumToSend);
                                          }
                                          else if( numberOfIncreasingDonorMessage==2 )
                                          {
                                              numberOfIncreasingDonorMessage++;
                                              printf("markNeeded:INFO: Too many: Increasing donorInfo size messages, I will not print anymore\n");
                                          }
                                          maxNumToSend=maxNumToSend*2;
                                          donorInfo.resize(maxNumToSend,numDonorData);
                                      }
                  //  printf("markNeeded:Send: grid=%i donor=%i myid=%i donorProcessor=%i i=%i numToSend[dp]=%i "
                  //         "[i1a,i1b][i2a,i2b]=[%i,%i][%i,%i]\n",
                  //	grid,donor,myid,donorProcessor,i,numToSend[donorProcessor],piab[0],piab[1],piab[2],piab[3]);
                   // we could check for duplicate donor data which might occur if one grid is much finer than the other
                                      donorInfo(i,0)=donorProcessor;
                                      donorInfo(i,1)=donor;
                                      for( int n=0; n<nd2; n++ )
                                          donorInfo(i,2+n)=piab[n];
                   // numDonor(donorGrid,donorProcessor)++; // use a sparse array here
                                      numToSend[donorProcessor]++; 
                                      i++;
                                }
                                else
                                {
                //   printf(" markPointsNeeded: myid=%i : donor stencil spans processors, grid=%i, donor=%i pt i=%i\n",myid,
                // 	 grid,donor,i);
                  // getDonorProcessor( 1,0,0,donorProc );
                                      ivd[0]=iab(1,0), ivd[1]=iab(0,1), ivd[2]=iab(0,2); 
                                      donorProc(1,0,0) = donorMask.Array_Descriptor.findProcNum( ivd );
                  // we could avoid the next call in 2d in some cases. Do this for now 
                  // getDonorProcessor( 0,1,0,donorProc );
                                      ivd[0]=iab(0,0), ivd[1]=iab(1,1), ivd[2]=iab(0,2); 
                                      donorProc(0,1,0) = donorMask.Array_Descriptor.findProcNum( ivd );
                                    if( numberOfDimensions==2 )
                                    {  
                                        donorProc(1,1,0)=donorProc(1,1,1);
                                        int s1b = donorProc(0,0,0)==donorProc(1,0,0) ? 0 : 1;
                                        int s2b = donorProc(0,0,0)==donorProc(0,1,0) ? 0 : 1;
                                        for( int s2=0; s2<=s2b; s2++ )for( int s1=0; s1<=s1b; s1++ )
                                        { // we fill 1, 2, or 4 times
                      // --- double check that we have the correct donorProc --
                                            if( true )
                                            {
                      // 	getDonorProcessor( s1,s2,0,dp );
                                              ivd[0]=iab(s1,0), ivd[1]=iab(s2,1), ivd[2]=iab(0,2); 
                                              dp(s1,s2,0) = donorMask.Array_Descriptor.findProcNum( ivd );
                                      	assert( dp(s1,s2,0)==donorProc(s1,s2,0) );
                                            }
                      // fillDonorDataProc(donorProc(s1,s2,0),piab);
                                              int donorProcessor = donorProc(s1,s2,0);
                                              if( i>=maxNumToSend )
                                              {
                                                  if( numberOfIncreasingDonorMessage<2 )
                                                  {
                                                      numberOfIncreasingDonorMessage++;
                                                      printf("markNeeded:WARNING: Increasing donorInfo size for grid=%i maxNumToSend=%i\n",grid,maxNumToSend);
                                                  }
                                                  else if( numberOfIncreasingDonorMessage==2 )
                                                  {
                                                      numberOfIncreasingDonorMessage++;
                                                      printf("markNeeded:INFO: Too many: Increasing donorInfo size messages, I will not print anymore\n");
                                                  }
                                                  maxNumToSend=maxNumToSend*2;
                                                  donorInfo.resize(maxNumToSend,numDonorData);
                                              }
                      //  printf("markNeeded:Send: grid=%i donor=%i myid=%i donorProcessor=%i i=%i numToSend[dp]=%i "
                      //         "[i1a,i1b][i2a,i2b]=[%i,%i][%i,%i]\n",
                      //	grid,donor,myid,donorProcessor,i,numToSend[donorProcessor],piab[0],piab[1],piab[2],piab[3]);
                       // we could check for duplicate donor data which might occur if one grid is much finer than the other
                                              donorInfo(i,0)=donorProcessor;
                                              donorInfo(i,1)=donor;
                                              for( int n=0; n<nd2; n++ )
                                                  donorInfo(i,2+n)=piab[n];
                       // numDonor(donorGrid,donorProcessor)++; // use a sparse array here
                                              numToSend[donorProcessor]++; 
                                              i++;
                                        }
                                    }
                                    else
                                    {
                    // getDonorProcessor( 0,0,1,donorProc );
                                          ivd[0]=iab(0,0), ivd[1]=iab(0,1), ivd[2]=iab(1,2); 
                                          donorProc(0,0,1) = donorMask.Array_Descriptor.findProcNum( ivd );
                                        donorProc(1,1,0)= donorProc(0,0,0)==donorProc(1,0,0) ? donorProc(0,1,0) : donorProc(1,0,0);
                                        donorProc(1,0,1)= donorProc(0,0,0)==donorProc(1,0,0) ? donorProc(0,0,1) : donorProc(1,0,0);
                                        donorProc(0,1,1)= donorProc(0,0,0)==donorProc(0,1,0) ? donorProc(0,0,1) : donorProc(0,1,0);
                                        const int s1b = donorProc(0,0,0)==donorProc(1,0,0) ? 0 : 1;
                                        const int s2b = donorProc(0,0,0)==donorProc(0,1,0) ? 0 : 1;
                                        const int s3b = donorProc(0,0,0)==donorProc(0,0,1) ? 0 : 1;
                                        for( int s3=0; s3<=s3b; s3++ )for( int s2=0; s2<=s2b; s2++ )for( int s1=0; s1<=s1b; s1++ )
                                        {
                                            if( s1+s2+s3 == 2 )
                                            {
                      // 	getDonorProcessor( s1,s2,s3,donorProc );
                                              ivd[0]=iab(s1,0), ivd[1]=iab(s2,1), ivd[2]=iab(s3,2); 
                                              donorProc(s1,s2,s3) = donorMask.Array_Descriptor.findProcNum( ivd );
                                            }
                                        }
                                        for( int s3=0; s3<=s3b; s3++ )for( int s2=0; s2<=s2b; s2++ )for( int s1=0; s1<=s1b; s1++ )
                                        { // we fill 1, 2, 4 or 8 times
                      // --- double check that we have the correct donorProc --
                                            if( true )
                                            {
                      // 	getDonorProcessor( s1,s2,s3,dp );
                                              ivd[0]=iab(s1,0), ivd[1]=iab(s2,1), ivd[2]=iab(s3,2); 
                                              dp(s1,s2,s3) = donorMask.Array_Descriptor.findProcNum( ivd );
                                      	assert( dp(s1,s2,s3)==donorProc(s1,s2,s3) );
                                            }
                      // fillDonorDataProc(donorProc(s1,s2,s3),piab);
                                              int donorProcessor = donorProc(s1,s2,s3);
                                              if( i>=maxNumToSend )
                                              {
                                                  if( numberOfIncreasingDonorMessage<2 )
                                                  {
                                                      numberOfIncreasingDonorMessage++;
                                                      printf("markNeeded:WARNING: Increasing donorInfo size for grid=%i maxNumToSend=%i\n",grid,maxNumToSend);
                                                  }
                                                  else if( numberOfIncreasingDonorMessage==2 )
                                                  {
                                                      numberOfIncreasingDonorMessage++;
                                                      printf("markNeeded:INFO: Too many: Increasing donorInfo size messages, I will not print anymore\n");
                                                  }
                                                  maxNumToSend=maxNumToSend*2;
                                                  donorInfo.resize(maxNumToSend,numDonorData);
                                              }
                      //  printf("markNeeded:Send: grid=%i donor=%i myid=%i donorProcessor=%i i=%i numToSend[dp]=%i "
                      //         "[i1a,i1b][i2a,i2b]=[%i,%i][%i,%i]\n",
                      //	grid,donor,myid,donorProcessor,i,numToSend[donorProcessor],piab[0],piab[1],piab[2],piab[3]);
                       // we could check for duplicate donor data which might occur if one grid is much finer than the other
                                              donorInfo(i,0)=donorProcessor;
                                              donorInfo(i,1)=donor;
                                              for( int n=0; n<nd2; n++ )
                                                  donorInfo(i,2+n)=piab[n];
                       // numDonor(donorGrid,donorProcessor)++; // use a sparse array here
                                              numToSend[donorProcessor]++; 
                                              i++;
                                        }
                                    }
                                }

            	      }
          	    }

        	  } // end if "needed"
      	} // end if lowerOrUpper...
            } // end if MASK ...
        } // end FOR_3D
    } // end if ok 
    
    int totalNumberToSend=i;


  // Send/receive numToSend[p] <-> numReceived[p]
    
    MPI_Status status;
    const int tag0=928617;
    for( int p=0; p<np; p++ )
    {
    // printf("markNeeded: grid=%i myid=%i Send %i pts to p=%i\n",grid,myid,numToSend[p],p);

        int tags=tag0+p, tagr=tag0+myid;
        MPI_Sendrecv(&numToSend[p],   1, MPI_INT, p, tags, 
                                  &numReceived[p], 1, MPI_INT, p, tagr, OV_COMM, &status ); 

    // printf("markNeeded: grid=%i myid=%i Receive %i pts from p=%i\n",grid,myid,numReceived[p],p);
    }


  // Allocate receive buffers
    int **prb = new int* [np];
    const int numDonorDataPerReceive=1+2*numberOfDimensions;
    for( int p=0; p<np; p++ )
    {
        const int numData = max(1,numReceived[p]*numDonorDataPerReceive);
        prb[p] = new int [numData];

    }
    
  // --- post receives ---
    MPI_Request *receiveRequest = new MPI_Request[np];  
    MPI_Request *sendRequest = new MPI_Request[np];  
    const int tag1=1418632;
    for( int p=0; p<np; p++ )
    {  
        int tag=tag1+myid;
        MPI_Irecv( prb[p], numReceived[p]*numDonorDataPerReceive, MPI_INT, p, tag, OV_COMM, &receiveRequest[p] );
    }


  // --- Allocate send buffers ---
    int **psb = new int* [np];
    for( int p=0; p<np; p++ )
    {
        const int numData = max(1,numToSend[p]*numDonorDataPerReceive);
        psb[p] = new int [numData];
    }


    #define sendBuff(p,i1,i2) psb[p][(i2)+numDonorDataPerReceive*(i1)]
    int * numSent = new int [np];
    for( int p=0; p<np; p++ )
    {
        numSent[p]=0;
    }
    
  // Fill send buffers: Make lists of the data that will be sent to each processor
    for( int i=0; i<totalNumberToSend; i++ )
    {
        int p=donorInfo(i,0);
        int j=numSent[p];
        
        for( int n=0; n<numDonorDataPerReceive; n++ )
        {
            sendBuff(p,j,n)=donorInfo(i,n+1);  // we could potentially sort by donor grid ?
        }
        
        numSent[p]++;
        
    }
    if( true || debug & 4 )
    { // sanity check:
        for( int p=0; p<np; p++ )
        {
            assert( numSent[p]==numToSend[p] );
        }
    }
    delete [] numSent;
    

  // Send buffers, psb[p]
    for( int p=0; p<np; p++ )
    {
        int tag=tag1+p;
        MPI_Isend(psb[p], numToSend[p]*numDonorDataPerReceive, MPI_INT, p, tag, OV_COMM, &sendRequest[p] );  
    }
    
  // --- wait for all the receives to finish ---
    MPI_Status *receiveStatus= new MPI_Status[np];  
    MPI_Waitall(np,receiveRequest,receiveStatus);


    if( true || debug & 4 )
    { // sanity check:
        for( int p=0; p<np; p++ )
        {
            int num=0;
            MPI_Get_count( &receiveStatus[p], MPI_INT, &num );
            assert( num==numReceived[p]*numDonorDataPerReceive );
        }
    }
    

  // **** Mark donor-mask points as needed ****
    #define receiveBuff(p,i1,i2) prb[p][(i2)+numDonorDataPerReceive*(i1)]
    int i1a,i1b,i2a,i2b,i3a=0,i3b=0;
    int validReceive=1;
    
    for( int p=0; p<np; p++ )
    {
        for( int i=0; i<numReceived[p]; i++ )
        {
            int donor=receiveBuff(p,i,0);
            assert( donor>=0 && donor<numberOfBaseGrids );
            
            intArray & donorMaskd = cg[donor].mask();
            GET_LOCAL(int,donorMaskd,donorMask);

            i1a=receiveBuff(p,i,1); i1b=receiveBuff(p,i,2); i2a=receiveBuff(p,i,3); i2b=receiveBuff(p,i,4); 

      // if( i1a>i1b || i1a<donorMask.getBase(0) || i1b>donorMask.getBound(0) )
            if( i1a>i1b || i2a>i2b )
            {
      	printf("markNeeded:ERROR: grid=%i donor=%i myid=%i p=%i np=%i i=%i [i1a,i1b][i2a,i2b]=[%i,%i][%i,%i]"
                              " i1a>i1b OR i2a>i2b! donorMask=[%i,%i][%i,%i]\n",
             	       grid,donor,myid,p,np,i,i1a,i1b,i2a,i2b,donorMask.getBase(0),donorMask.getBound(0),
                              donorMask.getBase(1),donorMask.getBound(1));
      	validReceive=0;
      	break;
            }
            i1a=max(i1a,donorMask.getBase(0));  i1b=min(i1b,donorMask.getBound(0));
            i2a=max(i2a,donorMask.getBase(1));  i2b=min(i2b,donorMask.getBound(1));
              
            
            if( numberOfDimensions==3 )
            { 
                i3a=receiveBuff(p,i,5); i3b=receiveBuff(p,i,6); 
        // assert( i3a<=i3b && i3a>=donorMask.getBase(2) && i3b<=donorMask.getBound(2) );
                assert( i3a<=i3b );

      	i3a=max(i3a,donorMask.getBase(2));  i3b=min(i3b,donorMask.getBound(2));

            }//
            for( int i3=i3a; i3<=i3b; i3++ )
            for( int i2=i2a; i2<=i2b; i2++ )
            for( int i1=i1a; i1<=i1b; i1++ )
            {
        // if( true || debug & 4 )
        //   printf("markNeeded: myid=%i: grid=%i donor=%i mark pt (%i,%i,%i) as needed\n",myid,grid,donor,i1,i2,i3);
      	
                donorMask(i1,i2,i3) |= ISneededPoint;
            }
        }
    }
    
    if( true )
    {
        validReceive=ParallelUtility::getMinValue(validReceive);
        if( !validReceive )
        {
            printF("markNeeded: An error occured. I am going to output the grid\n");
            aString gridFileName="gridFailed.hdf", gridName="gridFailed";
            printF("Saving the current grids in the file %s\n",(const char*)gridFileName);
            saveGridToAFile( cg,gridFileName,gridName );

            OV_ABORT("ERROR");
        }
    }  
  // wait for sends to finish on this processor before we can clean up
    MPI_Waitall(np,sendRequest,receiveStatus);

    for( int p=0; p<np; p++ )
    {
        delete [] prb[p];
        delete [] psb[p];
    }
    delete [] prb;
    delete [] psb;
    
    delete [] receiveRequest;
    delete [] sendRequest;

    delete [] numToSend;
    delete [] numReceived;
    delete [] receiveStatus;
    
    return 0;
#endif
}
#undef sendBuff
#undef receiveBuff
