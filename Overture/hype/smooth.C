#include "HyperbolicMapping.h"
#include "MappingInformation.h"
#include "DataPointMapping.h"
#include "display.h"
#include "GL_GraphicsInterface.h"

#include "CompositeSurface.h"

#include "TrimmedMapping.h"
#include "NurbsMapping.h"
#include "SplineMapping.h"

#include "MappingProjectionParameters.h"
#include "UnstructuredMapping.h"

#include "CompositeTopology.h"
#include "arrayGetIndex.h"

static int projectSmoothedGridOntoReferenceSurface=1;
static int smoothingOffset[2][3] ={0,0,0,0,0,0};
// static int smoothingRegion[2][3] ={0,0,0,0,0,0};

static int smoothGridGhostPoints=1;
static int smoothGridProjectFrequency=1;  // to add
static real smoothGridOmega=.5;

int HyperbolicMapping::
buildSmoothDialog(DialogData & smoothDialog )
// ==========================================================================================
// /Description:
//   Build the smoothing dialog.
// ==========================================================================================
{
//    const int numLabels=6;
//    aString opLabel[numLabels] = {"edges",
//  				"coordinate line 0",
//  				"coordinate line 1",
//  				"points on surface",
//                                  "boundary curve",
//  				""}; //
//    aString opCmd[numLabels];
//    GUIState::addPrefix(opLabel,"initial curve:",opCmd,numLabels);

//    int choice = 0;
//    if( surface!=NULL && surfaceGrid && surface->getClassName()=="UnstructuredMapping" )
//    {
//      initialCurveOption=initialCurveFromBoundaryCurves;
//    }
//    surfaceGridParametersDialog.addOptionMenu("initial curve from:", opCmd,opLabel,(int)initialCurveOption);


  aString pbLabels[] = {"smooth grid",
                        // "reset",
			""};
  int numRows=1;
  smoothDialog.setPushButtons( pbLabels, pbLabels, numRows ); 


  aString tbCommands[] = {"project smoothed grid onto reference surface",
                          "smooth ghost points",
			  ""};
  int tbState[10];
  tbState[0] = projectSmoothedGridOntoReferenceSurface==true; 
  tbState[1] = smoothGridGhostPoints==true;
  int numColumns=1;
  smoothDialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

  const int numberOfTextStrings=20;
  aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "number of smoothing iterations";
  sPrintF(textStrings[nt], "%i", numberOfSmoothingIterations); nt++; 

  textLabels[nt] = "smoothing offset";
  sPrintF(textStrings[nt], "%i %i %i %i %i %i (l r b t b f)",smoothingOffset[0][0],smoothingOffset[1][0],
             smoothingOffset[0][1],smoothingOffset[1][1],smoothingOffset[0][2],smoothingOffset[1][2] ); nt++; 

//    textLabels[nt] = "smoothing region";
//    sPrintF(textStrings[nt], "%i %i %i %i %i %i (l r b t b f)",smoothingRegion[0][0],smoothingRegion[1][0],
//               smoothingRegion[0][1],smoothingRegion[1][1],smoothingRegion[0][2],smoothingRegion[1][2] ); nt++; 

  textLabels[nt] = "relaxation coeff";
  sPrintF(textStrings[nt], "%e",smoothGridOmega); nt++;

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  // addPrefix(textLabels,prefix,cmd,maxCommands);
  smoothDialog.setTextBoxes(textLabels, textLabels, textStrings);

  return 0;
}

bool HyperbolicMapping::
updateSmoothingOptions(aString & answer, DialogData & smoothDialog, MappingInformation & mapInfo )
// ==========================================================================================
// /Description:
//     Assign values in the smoothDialog
//
// /answer (input) : check this answer to see if it is a marching parameter.
//
// /Return value: true if the answer was processed, false otherwise.
// ==========================================================================================
{
  bool returnValue=true;
  aString line;
  int len;
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  if( len=answer.matches("number of smoothing iterations") )
  {
    sScanF(answer(len,answer.length()-1),"%i",&numberOfSmoothingIterations);
    smoothDialog.setTextLabel("number of smoothing iterations",sPrintF(line, "%i",numberOfSmoothingIterations));
  }
  else if( len=answer.matches("project smoothed grid onto reference surface") )
  {
    sScanF(answer(len,answer.length()-1),"%i",&projectSmoothedGridOntoReferenceSurface);
    smoothDialog.setToggleState("project smoothed grid onto reference surface",
                   projectSmoothedGridOntoReferenceSurface);
  }
  else if( len=answer.matches("smoothing offset") )
  {
    printf("INFO: Setting the smoothing offset to a positive value will restrict the smoothing to a smaller domain\n"
           "    : For example, setting left side value to '1' will prevent smoothing on the left boundary\n");

    sScanF(answer(len,answer.length()-1),"%i %i %i %i %i %i",&smoothingOffset[0][0],&smoothingOffset[1][0],
             &smoothingOffset[0][1],&smoothingOffset[1][1],&smoothingOffset[0][2],&smoothingOffset[1][2] );
    for( int axis=0; axis<domainDimension; axis++ )
    {
      for( int side=0; side<=1; side++ )
      {
	if( smoothingOffset[side][axis]<0 )
	{
	  smoothingOffset[side][axis]=0;
	  printf(" INFO: smoothingOffset must be at least 0, setting smoothingOffset[%i][%i]=%i\n",
		 side,axis,smoothingOffset[side][axis]);
	}
      }
    }
    smoothDialog.setTextLabel("smoothing offset",
          sPrintF(line,"%i %i %i %i %i %i (l r b t b f)",smoothingOffset[0][0],smoothingOffset[1][0],
		  smoothingOffset[0][1],smoothingOffset[1][1],smoothingOffset[0][2],smoothingOffset[1][2]));
  }
//    else if( len=answer.matches("smoothing region") )
//    {
//      printf("INFO: The smoothing region defines the sub-set of points to smooth.\n"
//             "    : If ghost points are smooth theses will be the points adjacent to the smoothing region\n");

//      sScanF(answer(len,answer.length()-1),"%i %i %i %i %i %i",&smoothingRegion[0][0],&smoothingRegion[1][0],
//               &smoothingRegion[0][1],&smoothingRegion[1][1],&smoothingRegion[0][2],&smoothingRegion[1][2] );
//      smoothDialog.setTextLabel("smooth region",
//            sPrintF(line,"%i %i %i %i %i %i (l r b t b f)",smoothingRegion[0][0],smoothingRegion[1][0],
//  		  smoothingRegion[0][1],smoothingRegion[1][1],smoothingRegion[0][2],smoothingRegion[1][2]));
//    }
  else if( len=answer.matches("relaxation coeff") )
  {
    sScanF(answer(len,answer.length()-1),"%e",&smoothGridOmega);
    smoothDialog.setTextLabel("relaxation coeff",sPrintF(line, "%e",smoothGridOmega));
  }
  else if( len=answer.matches("smooth ghost points") )
  {
    sScanF(answer(len,answer.length()-1),"%i",&smoothGridGhostPoints);
    smoothDialog.setToggleState("smooth ghost points",smoothGridGhostPoints);
  }
  else if( answer=="smooth grid" )
  {
    smoothGrid();
    plotObject=true; 
  }
  else
  {
    returnValue=false;
  }

  return returnValue;
}

int HyperbolicMapping::
// ===================================================================================
// /Description:
//     Smooth the hyperbolic mapping
// ===================================================================================
smoothGrid()
{

  if( surfaceGrid )
  {
    if( surface==NULL )
    {
      printf("smoothGrid:ERROR: There is no reference surface defined!\n");
      return 1;
    }
    if( dpm==NULL )
    {
      printf("smoothGrid:ERROR: There is no grid defined yet! dpm==NULL\n");
      return 1;
    }
      
    MappingProjectionParameters mpParams;
    typedef MappingProjectionParameters MPP;
    mpParams.setIsAMarchingAlgorithm(false);
    realArray xx;
    intArray & subSurfaceIndex = mpParams.getIntArray(MPP::subSurfaceIndex);
    subSurfaceIndex=-1;  // set initial guess

    Range Rx=3;
    realArray & x = xHyper;
//    x=dpm->getGrid();
    x.reshape(x.dimension(0),x.dimension(2),1,Rx);

    realArray xOld;
    xOld=x;
    
    IntegerArray gid;
    gid=gridIndexRange;
    gid(Range(0,1),axis2)=gridIndexRange(Range(0,1),axis3);
    gid(Range(0,1),axis3)=0;

    IntegerArray gids; // smooth these points
    gids=gid;
    // adjust for the boundary offset
    int axis;
    for( axis=0; axis<domainDimension; axis++ )
    {
      if( ! (bool)getIsPeriodic(axis) )
      {
	gid(Start,axis)=max(x.getBase(axis),min(x.getBound(axis),gid(Start,axis)
                         +boundaryOffset[Start][axis]));
	gid(End  ,axis)=max(x.getBase(axis),min(x.getBound(axis),gid(End  ,axis)
                         -boundaryOffset[End  ][axis]));
	gids(Start,axis)=max(x.getBase(axis),min(x.getBound(axis),gid(Start,axis)
                         +smoothingOffset[Start][axis]));
	gids(End  ,axis)=max(x.getBase(axis),min(x.getBound(axis),gid(End  ,axis)
                         -smoothingOffset[End][axis]));

//          gids(Start,axis)=max(smoothingRegion[0][axis],gids(Start,axis));
//          gids(End  ,axis)=max(smoothingRegion[1][axis],gids(End  ,axis));
	
      }
    }
    gids(0,1)=max(1,gids(0,1));  

    printf("*** smooth: gids= [%i,%i][%i,%i] gid=[%i,%i][%i,%i] gridIndexRange=[%i,%i][%i,%i] \n"
           "      indexRange=[%i,%i][%i,%i]    x=[%i,%i][%i,%i]\n",
	   gids(0,0),gids(1,0),gids(0,1),gids(1,1),
	   gid(0,0),gid(1,0),gid(0,1),gid(1,1),
	   gridIndexRange(0,0),gridIndexRange(1,0),gridIndexRange(0,2),gridIndexRange(1,2),
	   indexRange(0,0),indexRange(1,0),indexRange(0,2),indexRange(1,2),
           x.getBase(0),x.getBound(0),x.getBase(1),x.getBound(1));
    
    Range I1,I2,I3;
    I1=Range(gids(0,0),gids(1,0)); // dpm->getGridDimensions(0);  // use gids??
    I2=Range(gids(0,1),gids(1,1)); // dpm->getGridDimensions(1);
    I3=Range(0,0);

    Index Ib1,Ib2,Ib3,Ig1,Ig2,Ig3,Ip1,Ip2,Ip3;
    
    const real omega=smoothGridOmega;
    for( int it=0; it<numberOfSmoothingIterations; it++ )
    {
      x(I1,I2,I3,Rx)=(1.-omega)*x(I1,I2,I3,Rx)+
	(omega/4.)*(x(I1-1,I2,I3,Rx)+x(I1+1,I2,I3,Rx)+
		    x(I1,I2-1,I3,Rx)+x(I1,I2+1,I3,Rx) );


      // fix up ghost points
      //  --> apply BC to get ghost values, project ghost points if necessary
      //      project boundary points back onto the boundary curve 
      if( smoothGridGhostPoints )
      {
	for( int side=0; side<=1; side++ )
	{
	  for( axis=0; axis<domainDimension; axis++ )
	  {
	    const int extra=1;  // to get corners
	    getBoundaryIndex(gids,side,axis,Ib1,Ib2,Ib3,extra);
	    getGhostIndex(gids,side,axis,Ig1,Ig2,Ig3,+1,extra); // first ghost line
	    getGhostIndex(gids,side,axis,Ip1,Ip2,Ip3,-1,extra); // first line inside
	  
	    x(Ig1,Ig2,Ig3,Rx)=(1.-omega)*x(Ig1,Ig2,Ig3) +
	      omega*(2.*x(Ib1,Ib2,Ib3,Rx)-x(Ip1,Ip2,Ip3,Rx));
	  
	  }
	}
      }
      
      if( projectSmoothedGridOntoReferenceSurface )
      {
	printf("*** smooth: project points onto the reference surface...\n");
       
        Range J1,J2,J3;
	J1=I1, J2=I2, J3=I3;   // include appropriate ghost points
	
        xx=x(J1,J2,J3,Rx);
        Range R=J1.getLength()*J2.getLength();
        xx.reshape(R,Rx);
	
	surface->project(xx,mpParams);

        xx.reshape(J1,J2,J3,Rx);
        x(J1,J2,J3,Rx)=xx;
      }
      
      real maxDiff=max(fabs(x-xOld));
      printf(" it=%i : smooth grid points [%i,%i][%i,%i], |x-xOld|=%8.2e\n",it,I1.getBase(),I1.getBound(),
               I2.getBase(),I2.getBound(),maxDiff );

    }


    dpm->setDataPoints(x,3,domainDimension,0,gid);
    x.reshape(x.dimension(0),1,x.dimension(1),Rx);

  }
  else
  {
    printf("smoothGrid:SORRY: not implemented yet for volume grids\n");
  }

  setBasicInverseOption(dpm->getBasicInverseOption());
  reinitialize();  // *wdh* 000503
      
  mappingHasChanged();
  plotObject=true; 
  plotHyperbolicSurface=true;
  
  return 0;
}





