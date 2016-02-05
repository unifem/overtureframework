#include "GridSmoother.h"
#include "arrayGetIndex.h"
#include "EquiDistribute.h"
#include "display.h"
#include "MappingInformation.h"
#include "ReductionMapping.h"

#include "DataPointMapping.h"
#include "GenericGraphicsInterface.h"
#include "CompositeSurface.h"
#include "MatchingCurve.h"

static bool useNewBoundaryConditions=true;

#define ellipticSmooth EXTERN_C_NAME(ellipticsmooth)
#define fixedControlFunctions EXTERN_C_NAME(fixedcontrolfunctions)
#define smoothSurfaceNormals EXTERN_C_NAME(smoothsurfacenormals)

extern "C"
{
void ellipticSmooth(const int&md, const int&nd, const int &indexRange,
     const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
     const int&n1a,const int&n1b,const int&n1c,const int&n2a,const int&n2b,const int&n2c,
     const int&n3a,const int&n3b,const int&n3c, const real&omega, real &u, const real&source, 
     const real & normal, const int & ipar, const real & rpar );

  void fixedControlFunctions( const int&md,const int&nd, 
			    const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
			    const int&n1a,const int&n1b,const int&n1c,const int&n2a,const int&n2b,const int&n2c,
			    const int&n3a,const int&n3b,const int&n3c, real&f, const real&dr, 
			    const int&ndipar,const int&ndrpar,const int&npar, const int&ipar, const real&rpar );

   void smoothSurfaceNormals(const int&nd, 
     const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
     const int&n1a,const int&n1b,const int&n2a,const int&n2b,
     const int&n3a,const int&n3b, const int &nit, const real&omega, real &normal, real &normal2 );

}

GridSmoother::
GridSmoother(int domainDimension_, int rangeDimension_ )
{
  domainDimension=domainDimension_;
  rangeDimension=rangeDimension_;
  
  numberOfIterations=5;
  numberOfEquidistributionIterations=0;
  numberOfLaplacianSmooths=0;
  numberOfEllipticSmooths=1;

  smoothNormals=false;
  numberOfNormalSmooths=3;

  // for blending projected solution and smoothed solution (surface grids)n
  blendingFactor=1.; // .5;    // 1=smooth, 0=project
  
  maximumProjectionCorrection=0.;
  
  projectSmoothedGridOntoReferenceSurface=true;
  smoothGridGhostPoints=true;
  for( int side=0; side<2; side++ )
  {
    for( int axis=0; axis<3; axis++ )
    {
      smoothingOffset[side][axis]=0;
      smoothingRegion[side][axis]=-side;
      boundaryMapping[side][axis]=NULL;
    }
  }
  
  omega=1.; // reduce if there are control functions added
  
  useInitialGridAsControlGrid=false;
  numberOfControlFunctionSmooths=3;

  arclengthWeight=.5;
  curvatureWeight=.5;
  areaWeight=0.;

  totalIterations=0;
  numberOfWeightSmooths=3;
  
  mpParams.setIsAMarchingAlgorithm(false);
  mpParams.setAdjustForCornersWhenMarching(false);

  bc.redim(2,3);
  bc=pointsSlide;

  controlFunctionComputed=false;
  
  // initialGridHasBeenSaved = true if the initial grid has been saved (used for resets)
  dbase.put<bool>("initialGridHasBeenSaved")=false;

  // resetGrid: is set to true if the grid should be reset
  dbase.put<bool>("resetGrid")=false;

  // We are testing out different options for computing ghost points
  //  0 = old method -- pre November 2015
  //  1 = new method
  dbase.put<int>("ghostPointOption")=0;

  // We smooth (at most) this many ghost points:
  dbase.put<int>("numberOfGhostPointsToSmooth")=1;

}

GridSmoother::
~GridSmoother()
{
}

// ===================================================================================================
/// \brief call this function when the initial grid has been recomputed and thus the
///        control function will change
// ===================================================================================================
void GridSmoother::
reset()
{
  controlFunctionComputed=false;
  dbase.get<bool>("initialGridHasBeenSaved")=false; // initial grid is no-longer valid
}


// =======================================================================================
/// \brief: Set weights for equidistribution.
// =======================================================================================
void GridSmoother::
setWeights( real arclength, real curvature, real area )
{
  arclengthWeight=arclength;
  curvatureWeight=curvature;
  areaWeight=area;
}

int GridSmoother::
setBoundaryConditions( int bc_[2][3] )
// ========================================================================================
// ========================================================================================
{
  bc(0,0)=bc_[0][0];
  bc(1,0)=bc_[1][0];
  bc(0,1)=bc_[0][1];
  bc(1,1)=bc_[1][1];
  bc(0,2)=bc_[0][2];
  bc(1,2)=bc_[1][2];
  return 0;
}

int GridSmoother::
setBoundaryConditions( IntegerArray & bc_ )
// ========================================================================================
// ========================================================================================
{
  bc(0,0)=bc_(0,0);
  bc(1,0)=bc_(1,0);
  bc(0,1)=bc_(0,1);
  bc(1,1)=bc_(1,1);
  bc(0,2)=bc_(0,2);
  bc(1,2)=bc_(1,2);
  return 0;
}

int GridSmoother::
setBoundaryMappings( Mapping *boundaryMapping_[2][3] )
// ===========================================================================
// /Description: Supply Mappings to project boundary values onto
// ===========================================================================
{
  for( int side=0; side<2; side++ )
  {
    for( int axis=0; axis<3; axis++ )
    {
      if( boundaryMapping[side][axis]!=NULL && boundaryMapping[side][axis]->decrementReferenceCount()==0 )
        delete boundaryMapping[side][axis];
      boundaryMapping[side][axis]=boundaryMapping_[side][axis];
      if( boundaryMapping[side][axis]!=NULL )
        boundaryMapping[side][axis]->incrementReferenceCount();
    }
  }
  return 0;
}

int GridSmoother::
setMatchingCurves(std::vector<MatchingCurve> & matchingCurves_ )
// ===========================================================================
// /Description: Supply interior matching curves.
// ===========================================================================
{
  matchingCurves=matchingCurves_;
  return 0;
}




int GridSmoother::
buildDialog(DialogData & dialog)
// ========================================================================================
// ========================================================================================
{
  dialog.setOptionMenuColumns(1);
  aString bcChoices[]={ "periodic",
                        "fixed",
                        "slide",
                        "smoothed",
                        ""   };  //
  
  const int numberOfBoundaryConditions=4;
  const int maxCommands=numberOfBoundaryConditions+1;
  aString bcCmd[maxCommands];

  GUIState::addPrefix(bcChoices,"GSM:BC: left ",bcCmd,maxCommands);
  dialog.addOptionMenu("BC: left [green]  ", bcCmd,bcChoices,bc(0,0)+1);
  GUIState::addPrefix(bcChoices,"GSM:BC: right ",bcCmd,maxCommands);
  dialog.addOptionMenu("BC: right [red]   ", bcCmd,bcChoices,bc(1,0)+1);
  GUIState::addPrefix(bcChoices,"GSM:BC: bottom ",bcCmd,maxCommands);
  dialog.addOptionMenu("BC: bottom [blue] ", bcCmd,bcChoices,bc(0,1)+1);
  GUIState::addPrefix(bcChoices,"GSM:BC: top ",bcCmd,maxCommands);
  dialog.addOptionMenu("BC: top [yellow]  ", bcCmd,bcChoices,bc(1,1)+1);
  if( domainDimension==3 )
  {
    GUIState::addPrefix(bcChoices,"GSM:BC: back ",bcCmd,maxCommands);
    dialog.addOptionMenu("BC: back [orange]  ", bcCmd,bcChoices,bc(0,2)+1);
    GUIState::addPrefix(bcChoices,"GSM:BC: front ",bcCmd,maxCommands);
    dialog.addOptionMenu("BC: front [violet] ", bcCmd,bcChoices,bc(1,2)+1);
  }

  aString pbLabels[] = {"smooth grid",
                        "reset grid",
                        "clear `do not project' regions",
                        "help grid smoother",
			""};

  const int maximumCommands=30;
  aString cmdWithPrefix[maximumCommands];
  GUIState::addPrefix(pbLabels,"GSM:",cmdWithPrefix,maximumCommands);

  int numRows=2;
  dialog.setPushButtons( cmdWithPrefix, pbLabels, numRows ); 


  aString tbCommands[] = {"project smoothed grid onto reference surface",
                          "smooth ghost points",
                          "smooth normals",
                          "use initial grid as control grid",
			  ""};
  int tbState[10];
  tbState[0] = projectSmoothedGridOntoReferenceSurface==true; 
  tbState[1] = smoothGridGhostPoints==true;
  tbState[2] = smoothNormals==true;
  tbState[3] = useInitialGridAsControlGrid!=0;
  
  int numColumns=1;
  GUIState::addPrefix(tbCommands,"GSM:",cmdWithPrefix,maximumCommands);
  dialog.setToggleButtons(cmdWithPrefix, tbCommands, tbState, numColumns); 


  const int numberOfTextStrings=30;
  aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "number of iterations";
  sPrintF(textStrings[nt], "%i", numberOfIterations); nt++; 

  textLabels[nt] = "number of elliptic smooths";
  sPrintF(textStrings[nt], "%i", numberOfEllipticSmooths); nt++; 

  textLabels[nt] = "number of control function smooths";
  sPrintF(textStrings[nt], "%i", numberOfControlFunctionSmooths); nt++; 

  textLabels[nt] = "number of laplacian smooths";
  sPrintF(textStrings[nt], "%i", numberOfLaplacianSmooths); nt++; 

  textLabels[nt] = "number of equidistribution iterations";
  sPrintF(textStrings[nt], "%i", numberOfEquidistributionIterations); nt++; 


  textLabels[nt] = "number of ghost to smooth";
  sPrintF(textStrings[nt], "%i", dbase.get<int>("numberOfGhostPointsToSmooth")); nt++; 

  textLabels[nt] = "ghost point option";
  sPrintF(textStrings[nt], "%i", dbase.get<int>("ghostPointOption")); nt++; 

  if( regionsNotToProject.getLength(0)==0 )
  {
    Range all;
    regionsNotToProject.redim(10,5);
    regionsNotToProject(all,0)=-1;
    regionsNotToProject(0,0)=0;
    regionsNotToProject(0,1)=0;
    regionsNotToProject(0,2)=-1;
    regionsNotToProject(0,3)=0;
    regionsNotToProject(0,4)=-1;
  }
  
  textLabels[nt] = "do not project";
  sPrintF(textStrings[nt], "%i, %i %i %i %i (id, l r b t)",regionsNotToProject(0,0),regionsNotToProject(0,1),regionsNotToProject(0,2),
           regionsNotToProject(0,3),regionsNotToProject(0,4)); nt++; 

  textLabels[nt] = "smoothing offset";
  sPrintF(textStrings[nt], "%i %i %i %i %i %i (l r b t b f)",smoothingOffset[0][0],smoothingOffset[1][0],
             smoothingOffset[0][1],smoothingOffset[1][1],smoothingOffset[0][2],smoothingOffset[1][2] ); nt++; 
  
//    textLabels[nt] = "smoothing region";
//    sPrintF(textStrings[nt], "%i %i %i %i %i %i (l r b t b f)",smoothingRegion[0][0],smoothingRegion[1][0],
//               smoothingRegion[0][1],smoothingRegion[1][1],smoothingRegion[0][2],smoothingRegion[1][2] ); nt++; 

  if( rpar.getLength(0)==0 )
  {
    ipar.redim(2,10);
    ipar=0;
    rpar.redim(4,10);
    rpar=0.;

    ipar(0,0)=lineAttraction;
    ipar(1,0)=0;  // axis
    rpar(0,0)=0.;  // a
    rpar(1,0)=5.;  // b
    rpar(2,0)=.5;  // c
    
    ipar(0,1)=lineAttraction;
    ipar(1,1)=1;
    rpar(0,1)=0.;
    rpar(1,1)=10.;
    rpar(2,1)=.0;

    ipar(0,2)=lineAttraction;
    ipar(1,2)=2;
    rpar(0,2)=0.;
    rpar(1,2)=10.;
    rpar(2,2)=.5;
    
  }

  int stretchID=0;
  textLabels[nt] = "line attraction r1";
  sPrintF(textStrings[nt], "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
         rpar(0,stretchID),rpar(1,stretchID),rpar(2,stretchID) ); nt++; 

  stretchID=1;
  textLabels[nt] = "line attraction r2";
  sPrintF(textStrings[nt], "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
         rpar(0,stretchID),rpar(1,stretchID),rpar(2,stretchID) ); nt++; 

  if( domainDimension==3 )
  {
    stretchID=2;
    textLabels[nt] = "line attraction r3";
    sPrintF(textStrings[nt], "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
	    rpar(0,stretchID),rpar(1,stretchID),rpar(2,stretchID) ); nt++; 
  }

//    textLabels[nt] = "arclength weight";
//    sPrintF(textStrings[nt], "%g (for equidistribution)",arclengthWeight); nt++;

//    textLabels[nt] = "curvature weight";
//    sPrintF(textStrings[nt], "%g (for equidistribution)",curvatureWeight); nt++;

//    textLabels[nt] = "area weight";
//    sPrintF(textStrings[nt], "%g (for equidistribution)",areaWeight); nt++;

    textLabels[nt] = "relaxation coeff";
    sPrintF(textStrings[nt], "%g (for elliptic smooths)",omega); nt++;

    textLabels[nt] = "blending factor";
    sPrintF(textStrings[nt], "%g (for blended projection 0=project)",blendingFactor); nt++;

//    textLabels[nt] = "number of weight smooths";
//    sPrintF(textStrings[nt], "%i (for equidistribution)", numberOfWeightSmooths); nt++; 

  assert( nt<numberOfTextStrings );
  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]=""; 
  // addPrefix(textLabels,prefix,cmd,maxCommands);

  GUIState::addPrefix(textLabels,"GSM:",cmdWithPrefix,maximumCommands);
  dialog.setTextBoxes(cmdWithPrefix, textLabels, textStrings);


  return 0;
}

bool GridSmoother::
updateOptions(aString & answer_, DialogData & dialog, MappingInformation & mapInfo )
// ==========================================================================================
// /Description:
//     Assign values in the dialog
//
// /answer (input) : check this answer to see if it is a GridSmoother answer.
//
// /Return value: true if the answer was processed, false otherwise.
// ==========================================================================================
{
  aString bcChoices[]={ "periodic",
                        "fixed",
                        "slide",
                        "smoothed",
                        ""   };  //

  bool returnValue=true;
  aString line;
  int len;
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  const aString prefix="GSM:";

  aString answer=answer_;
  // take off the prefix
  // printf("GridSmoother: answer=[%s] [%s]\n",(const char*)answer,(const char*)answer(0,prefix.length()-1));
  
  if( answer(0,prefix.length()-1)==prefix )
    answer=answer(prefix.length(),answer.length()-1);
  else
  {
    return false;
  }
  // printf("GridSmoother: without prefix: answer=[%s]\n",(const char*)answer);
  
  if( answer=="help grid smoother" )
  {
    gi.outputString("\n"
                    " ---------------------------------------------------------------------------\n"
                    "               Grid Smoother "
                    " The grid smoother can be used to smooth 2d and 3d volume and surface grids \n"
                    " \n"
                    "  Currently there are 3 types of smoothers. \n"
                    "     o elliptic grid smoother (most useful) \n"
                    "     o laplacian smoother (uses simple 5 point laplacian)  \n"
                    "     o equidistribution smoother (experimental) \n"
                    " NOTES on parameters: \n"
                    "   o number of elliptic smooths : number of sub-smooths of the elliptic grid \n"
                    "           generation equations. Points are not projected onto surface grids \n"
                    "           during sub-smooths (since this is expensive) but other BC's are applied.\n"
                    " ---------------------------------------------------------------------------\n");
  }

  else if( dialog.getTextValue(answer,"number of ghost to smooth","%i",dbase.get<int>("numberOfGhostPointsToSmooth")) )
  {
     // we need to recompute the control function since it's dimensions have changed
    controlFunctionComputed=false; 
  }
  else if( dialog.getTextValue(answer,"ghost point option","%i",dbase.get<int>("ghostPointOption")) ){} //

  else if( (len=answer.matches("number of iterations")) )
  {
    sScanF(answer(len,answer.length()-1),"%i",&numberOfIterations);
    dialog.setTextLabel("number of iterations",sPrintF(line, "%i",numberOfIterations));
  }
  else if( (len=answer.matches("number of equidistribution iterations")) )
  {
    sScanF(answer(len,answer.length()-1),"%i",&numberOfEquidistributionIterations);
    dialog.setTextLabel("number of equidistribution iterations",sPrintF(line, "%i",numberOfEquidistributionIterations));
  }
  else if( (len=answer.matches("number of laplacian smooths")) )
  {
    sScanF(answer(len,answer.length()-1),"%i",&numberOfLaplacianSmooths);
    dialog.setTextLabel("number of laplacian smooths",sPrintF(line, "%i",numberOfLaplacianSmooths));
  }
  else if( (len=answer.matches("number of elliptic smooths")) )
  {
    sScanF(answer(len,answer.length()-1),"%i",&numberOfEllipticSmooths);
    dialog.setTextLabel("number of elliptic smooths",sPrintF(line, "%i",numberOfEllipticSmooths));
  }
  else if( (len=answer.matches("number of control function smooths")) )
  {
    sScanF(answer(len,answer.length()-1),"%i",&numberOfControlFunctionSmooths);
    dialog.setTextLabel("number of control function smooths",sPrintF(line, "%i",numberOfControlFunctionSmooths));
  }
  else if( (len=answer.matches("number of weight smooths")) )
  {
    sScanF(answer(len,answer.length()-1),"%i",&numberOfWeightSmooths);
    dialog.setTextLabel("number of weight smooths",sPrintF(line, "%i (for equidistribution)",numberOfWeightSmooths));
  }
  else if( (len=answer.matches("project smoothed grid onto reference surface")) )
  {
    sScanF(answer(len,answer.length()-1),"%i",&projectSmoothedGridOntoReferenceSurface);
    dialog.setToggleState("project smoothed grid onto reference surface",
                   projectSmoothedGridOntoReferenceSurface);
  }
  else if( answer=="clear `do not project' regions" )
  {
    Range all;
    regionsNotToProject(all,0)=-1;
    regionsNotToProject(0,0)=0;
    regionsNotToProject(0,1)=0;
    regionsNotToProject(0,2)=-1;
    regionsNotToProject(0,3)=0;
    regionsNotToProject(0,4)=-1;
    dialog.setTextLabel("do not project",
			sPrintF(line, "%i, %i %i %i %i (id, l r b t)", regionsNotToProject(0,0),regionsNotToProject(0,1),
				regionsNotToProject(0,2),regionsNotToProject(0,3),regionsNotToProject(0,4)));
  }
  else if( (len=answer.matches("do not project")) )
  {
    printf("INFO: For surface grids, specify one or more rectangles where the points should not be projected after smoothing.\n"
           "    : For each rectangle specify an id(>=0) and the range of points:  id, i1a i1b i2a i2b\n");

    int id=-1;
    sScanF(answer(len,answer.length()-1),"%i",&id);
    
    if( id<0 || id>10000 ) // assume no more than 10000 for a sanity check
    {
      printf("ERROR: invalid rectangle id=%i\n",id);
    }
    else 
    {
      if( id>regionsNotToProject.getBound(0) )
      {
        int num=regionsNotToProject.getLength(0);
	regionsNotToProject.resize(id+5,regionsNotToProject.dimension(1));
        Range all;
	regionsNotToProject(Range(num,regionsNotToProject.getBound(0)),all)=-1;  // init new entries
      }
      
      sScanF(answer(len,answer.length()-1),"%i %i %i %i %i",&regionsNotToProject(id,0),&regionsNotToProject(id,1),
             &regionsNotToProject(id,2),&regionsNotToProject(id,3),&regionsNotToProject(id,4));

      printf(" INFO: do not project points in the rectangle id=%i [%i,%i][%i,%i]\n",regionsNotToProject(id,0),
             regionsNotToProject(id,1),regionsNotToProject(id,2),
	     regionsNotToProject(id,3),regionsNotToProject(id,4));
    }
    dialog.setTextLabel("do not project",
            sPrintF(line, "%i, %i %i %i %i (id, l r b t)", regionsNotToProject(0,0),regionsNotToProject(0,1),
             regionsNotToProject(0,2),regionsNotToProject(0,3),regionsNotToProject(0,4)));
  }
  else if( (len=answer.matches("smoothing offset")) )
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
    dialog.setTextLabel("smoothing offset",
          sPrintF(line,"%i %i %i %i %i %i (l r b t b f)",smoothingOffset[0][0],smoothingOffset[1][0],
		  smoothingOffset[0][1],smoothingOffset[1][1],smoothingOffset[0][2],smoothingOffset[1][2]));
  }
//    else if( (len=answer.matches("smoothing region")) )
//    {
//      printf("INFO: The smoothing region defines the sub-set of points to smooth.\n"
//             "    : If ghost points are smooth theses will be the points adjacent to the smoothing region\n");

//      sScanF(answer(len,answer.length()-1),"%i %i %i %i %i %i",&smoothingRegion[0][0],&smoothingRegion[1][0],
//               &smoothingRegion[0][1],&smoothingRegion[1][1],&smoothingRegion[0][2],&smoothingRegion[1][2] );
//      dialog.setTextLabel("smooth region",
//            sPrintF(line,"%i %i %i %i %i %i (l r b t b f)",smoothingRegion[0][0],smoothingRegion[1][0],
//  		  smoothingRegion[0][1],smoothingRegion[1][1],smoothingRegion[0][2],smoothingRegion[1][2]));
//    }
  else if( (len=answer.matches("relaxation coeff")) )
  {
    sScanF(answer(len,answer.length()-1),"%e",&omega);
    dialog.setTextLabel("relaxation coeff",sPrintF(line, "%g (for laplacian smooths)",omega));
  }
  else if( (len=answer.matches("blending factor")) )
  {
    sScanF(answer(len,answer.length()-1),"%e",&blendingFactor);
    dialog.setTextLabel("blending factor",sPrintF(line, "%g (for blended projection)",blendingFactor));
  }
  else if( (len=answer.matches("arclength weight")) )
  {
    sScanF(answer(len,answer.length()-1),"%e",&arclengthWeight);
    dialog.setTextLabel("arclength weight",sPrintF(line, "%g (for equidistribution)",arclengthWeight));
  }
  else if( (len=answer.matches("curvature weight")) )
  {
    sScanF(answer(len,answer.length()-1),"%e",&curvatureWeight);
    dialog.setTextLabel("curvature weight",sPrintF(line, "%g (for equidistribution)",curvatureWeight));
  }
  else if( (len=answer.matches("area weight")) )
  {
    sScanF(answer(len,answer.length()-1),"%e",&areaWeight);
    dialog.setTextLabel("area weight",sPrintF(line, "%g (for equidistribution)",areaWeight));
  }
  else if( (len=answer.matches("smooth ghost points")) )
  {
    sScanF(answer(len,answer.length()-1),"%i",&smoothGridGhostPoints);
    dialog.setToggleState("smooth ghost points",smoothGridGhostPoints);
  }
  else if( (len=answer.matches("smooth normals")) )
  {
    sScanF(answer(len,answer.length()-1),"%i",&smoothNormals);
    dialog.setToggleState("smooth normals",smoothNormals);
  }
  else if( (len=answer.matches("use initial grid as control grid")) )
  {
    sScanF(answer(len,answer.length()-1),"%i",&useInitialGridAsControlGrid);
    dialog.setToggleState("use initial grid as control grid",useInitialGridAsControlGrid);
    
    controlFunctionComputed=false;  // we need to recompute the control function 
  }

  else if( answer.matches("BC:") )
  {
    int side=0,axis=0;
    if( (len=answer.matches("BC: left ")) )
    {
      side=0;  axis=0;
    }
    else if( (len=answer.matches("BC: right ")) )
    {
      side=1;  axis=0;
    }
    else if( (len=answer.matches("BC: bottom" )) )
    {
      side=0;  axis=1;
    }
    else if( (len=answer.matches("BC: top" )) )
    {
      side=1;  axis=1;
    }
    else if( (len=answer.matches("BC: back" )) )
    {
      side=0;  axis=2;
    }
    else if( (len=answer.matches("BC: front" )) )
    {
      side=1;  axis=2;
    }

    const int length=answer.length();
    while( len<length && answer[len]==' ' ) // skip blanks
      len++;

    line=answer(len,answer.length()-1);

    // search for the BC name in the list of choices.
    int bcChosen=-1; 
    for( int i=0; bcChoices[i]!=""; i++ )
    {
      if( line==bcChoices[i] )
      {
	bcChosen=i;
	break;
      }
    }
    if( bcChosen==-1 )
    {
      printf("ERROR: unknown BC: answer=[%s], line=[%s]\n",(const char*)answer,(const char*)line);
      printf(" Available boundary conditions are:\n");
      for( int i=0; bcChoices[i]!=""; i++ )
      {
	printf("[%s]\n",(const char*)bcChoices[i]);
      }
    }
    else if( bcChosen>=0 )
    {
      bc(side,axis)=bcChosen-1;  // BC's start at -1
      int optionMenuNumber=side+2*axis;
      dialog.getOptionMenu(optionMenuNumber).setCurrentChoice(bc(side,axis)+1);
    }
  }
  else if( (len=answer.matches("line attraction r")) )
  {
    int axis = (len=answer.matches("line attraction r1")) ? 0 :
               (len=answer.matches("line attraction r2")) ? 1 : 2;

    gi.outputString(sPrintF(line,"INFO: Attract grid lines along axis%i",axis+1));
    int stretchID=-1;
    sScanF(answer(len,answer.length()-1),"%i",&stretchID);

    if( stretchID<0 || stretchID>100 )
    {
      printf("ERROR: invalid value for stretchID=%i\n",stretchID);
      return returnValue;
    }
    
    if( stretchID>rpar.getBound(1) )
    {
      int num=rpar.getLength(1);
      rpar.resize(rpar.getLength(0),stretchID+10);
      ipar.resize(ipar.getLength(0),stretchID+10);
      Range all;
      rpar(all,Range(num,rpar.getBound(1)))=0;
      ipar(all,Range(num,ipar.getBound(1)))=0;
    }
    ipar(0,stretchID)=lineAttraction;
    ipar(1,stretchID)=axis;
    
    sScanF(answer(len,answer.length()-1),"%i %e %e %e",&stretchID,
	   &rpar(0,stretchID),&rpar(1,stretchID),&rpar(2,stretchID) );

    if( axis==0 )
    {
      dialog.setTextLabel("line attraction r1",sPrintF(line, "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
				  rpar(0,stretchID),rpar(1,stretchID),rpar(2,stretchID) ));
    }
    else if( axis==1 )
    {
      dialog.setTextLabel("line attraction r2",sPrintF(line, "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
				  rpar(0,stretchID),rpar(1,stretchID),rpar(2,stretchID) ));
    }
    else
    {
      dialog.setTextLabel("line attraction r3",sPrintF(line, "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
				  rpar(0,stretchID),rpar(1,stretchID),rpar(2,stretchID) ));
    }
    
  }
  else if( answer=="reset grid" )
  {
    // grid will be reset on next call to smooth:
    dbase.get<bool>("resetGrid")=true;
  }
  else if( answer=="smooth grid" || answer=="smooth new grid" )
  {
    // handled by calling routine

    //  smoothGrid();
    //  plotObject=true; 
  }
  else
  {
    returnValue=false;
  }


  return returnValue;
}

int GridSmoother::
periodicUpdate( realArray & x, const IntegerArray & indexRange )
// =============================================================================================
// =============================================================================================
{
  Index Is1,Is2,Is3,Ie1,Ie2,Ie3;
  int is[3]={0,0,0}; 
  Range Rx=x.dimension(3); // (0,rangeDimension-1);
  
  for( int dir=0; dir<domainDimension; dir++ )
  {
    if( bc(0,dir)<0 )
    {
      is[dir]=1;
      int extra=1; // get ghost points
      getBoundaryIndex(indexRange,Start,dir,Is1,Is2,Is3,extra);
      getBoundaryIndex(indexRange,End  ,dir,Ie1,Ie2,Ie3,extra);
      x(Ie1,Ie2,Ie3,Rx)=x(Is1,Is2,Is3,Rx);
      x(Ie1+is[0],Ie2+is[1],Ie3+is[2],Rx)=x(Is1+is[0],Is2+is[1],Is3+is[2],Rx);
      x(Is1-is[0],Is2-is[1],Is3-is[2],Rx)=x(Ie1-is[0],Ie2-is[1],Ie3-is[2],Rx);

      is[dir]=0;
    }
  }
  return 0;
}


void GridSmoother::
computeNormals( realArray & normal, const Index & I1, const Index & I2, const Index & I3, const realArray & x ) 
// =========================================================================================================
// Compute the normals from a difference approximation to the tangents
// =========================================================================================================
{
  realArray norm(I1,I2,I3),xr1(I1,I2,I3,3),xr2(I1,I2,I3,3);
  for( int dir=0; dir<rangeDimension; dir++ )
  {
    xr1(I1,I2,I3,dir)=x(I1+1,I2,I3,dir)-x(I1-1,I2,I3,dir);
    xr2(I1,I2,I3,dir)=x(I1,I2+1,I3,dir)-x(I1,I2-1,I3,dir);
  }

  normal(I1,I2,I3,0)=xr1(I1,I2,I3,1)*xr2(I1,I2,I3,2)-xr1(I1,I2,I3,2)*xr2(I1,I2,I3,1);
  normal(I1,I2,I3,1)=xr1(I1,I2,I3,2)*xr2(I1,I2,I3,0)-xr1(I1,I2,I3,0)*xr2(I1,I2,I3,2);
  normal(I1,I2,I3,2)=xr1(I1,I2,I3,0)*xr2(I1,I2,I3,1)-xr1(I1,I2,I3,1)*xr2(I1,I2,I3,0);

  norm=max( REAL_MIN*100., sqrt(SQR(normal(I1,I2,I3,0))+SQR(normal(I1,I2,I3,1))+SQR(normal(I1,I2,I3,2))) );
  normal(I1,I2,I3,0)/norm;
  normal(I1,I2,I3,1)/norm;
  normal(I1,I2,I3,2)/norm;
}

int GridSmoother::
applyBoundaryConditions(Mapping & map, DataPointMapping & dpm, 
                        realArray & x, 
                        const IntegerArray & indexRange, const IntegerArray & gids, 
                        const Index Iv[3], const Index Jv[3], const Index Kv[3],
                        realArray & normal, bool projectSurfaceGrids /* =true */ )
// ===============================================================
// /Description:
// ************ Boundary Conditions ********************
//
//      (1) "smooth" ghost points if appropriate
//      (2) project sliding-BC points back onto the boundary
//      (3) project points onto interior matching curves
//      (4) for surface grids project points onto the reference surface.
// /Kv (input) : project these points
// ===============================================================
{
  int debugBC=0;

  const Index &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  const Index &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  const Index &K1=Kv[0], &K2=Kv[1], &K3=Kv[2];

  Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];
  Index Nv[3], &N1=Nv[0], &N2=Nv[1], &N3=Nv[2];
  Index Ig1,Ig2,Ig3,Ip1,Ip2,Ip3;
  Range Rx=rangeDimension;

  // (1) fix up ghost points
  //  --> apply BC to get ghost values, project ghost points if necessary
  //      project boundary points back onto the boundary curve 

  // *wdh* 100210 try this:
  useNewBoundaryConditions=useNewBoundaryConditions && domainDimension==2 && rangeDimension==2;

  // We are testing out different options for computing ghost points
  //  0 = old method -- pre November 2015
  //  1 = new method
  const int & ghostPointOption = dbase.get<int>("ghostPointOption");


  if( smoothGridGhostPoints && 
      ghostPointOption==0 && 
      !useNewBoundaryConditions )  // no need to do this will new method -- ghost points computed already
  {
    real omegab=1.;
    for( int side=0; side<=1; side++ )
    {
      for( int axis=0; axis<domainDimension; axis++ )
      {
	if( bc(side,axis)!=(int)periodic && smoothingOffset[side][axis]==0 )
	{
	  const int extra=1;  // to get corners
	  getBoundaryIndex(indexRange,side,axis,Ib1,Ib2,Ib3,extra);
	  getGhostIndex(indexRange,side,axis,Ig1,Ig2,Ig3,+1,extra); // first ghost line
	  getGhostIndex(indexRange,side,axis,Ip1,Ip2,Ip3,-1,extra); // first line inside

          if( debugBC ) 
             printf("smooth ghost points (side,axis)=(%i,%i) points [%i,%i][%i,%i]\n",
		 side,axis,Ig1.getBase(),Ig1.getBound(),Ig2.getBase(),Ig2.getBound());
	  
	  x(Ig1,Ig2,Ig3,Rx)=(1.-omegab)*x(Ig1,Ig2,Ig3) + omegab*(2.*x(Ib1,Ib2,Ib3,Rx)-x(Ip1,Ip2,Ip3,Rx));
		
	}
      }
    }
  }

  periodicUpdate(x,indexRange);

  // When should we project ??  what about ghost points, redo?
  for( int side=0; side<=1; side++ )
  {
    for( int axis=0; axis<domainDimension; axis++ )
    {
      // printf(" GridSmoother:applyBoundaryConditions: side,axis,bc,smoothingOffset: %i %i %i %i "
      //        "boundaryMapping=%i\n",side,axis,bc(side,axis),smoothingOffset[side][axis],
      //         (int)boundaryMapping[side][axis]);
      
      if( bc(side,axis)==pointsSlide && smoothingOffset[side][axis]==0 )
      {
        // int numGhost=0;
        // getBoundaryIndex(indexRange,side,axis,Ib1,Ib2,Ib3,numGhost); // *wdh* 040926 -- project ghost points too
        getBoundaryIndex(indexRange,side,axis,Ib1,Ib2,Ib3); // *wdh* 040926 -- project ghost points too

        // if we don't have a bcMapping, use the edge of the dpm!
        Mapping *bcMapping=boundaryMapping[side][axis];
	if( bcMapping==NULL )
	{
          // If there is no boundaryMapping just make one from the current dpm
	  bcMapping=new ReductionMapping(dpm,axis,(real)side);
          bcMapping->incrementReferenceCount();
          if( debugBC )
  	    printf("++++GridSmoother:applyBoundaryConditions: project points onto *dpm* (side,axis)=(%i,%i)\n",
                   side,axis);
	}
	else
	{
  	  if( debugBC )
            printf("++++GridSmoother:applyBoundaryConditions: project points onto (side,axis)=(%i,%i)\n",side,axis);
	}
	
	// project points x(Ib1,Ib2,Ib3,Rx)
	MappingProjectionParameters mpParamsBC;
	mpParamsBC.setIsAMarchingAlgorithm(false);
	const int num=Ib1.getLength()*Ib2.getLength()*Ib3.getLength();
	realArray xx(Ib1,Ib2,Ib3,Rx);
	xx(Ib1,Ib2,Ib3,Rx)=x(Ib1,Ib2,Ib3,Rx);
	xx.reshape(num,Rx);
	bcMapping->project(xx,mpParamsBC);
	xx.reshape(Ib1,Ib2,Ib3,Rx);
	x(Ib1,Ib2,Ib3,Rx)=xx(Ib1,Ib2,Ib3,Rx);
	  
        if( boundaryMapping[side][axis]==NULL )
	{
	  if( bcMapping->decrementReferenceCount()==0 )
	    delete bcMapping;
	}
	
      }
    }
  }
    
  const int numberOfMatchingCurves=matchingCurves.size();
  if( numberOfMatchingCurves>0 )
  {
    for( int i=0; i<numberOfMatchingCurves; i++ )
    {
      MatchingCurve & match = matchingCurves[i];
      const int gridLine=match.gridLine;  // this should have been computed when the start curve was evaluated
      assert( gridLine>=0 );

      assert( match.curve!=NULL && match.projectionParameters!=NULL );
       
      Mapping & matchingMap = *match.curve;
      MappingProjectionParameters & mpParams = *match.projectionParameters;
   
      printf("Projecting gridLine i1=%i onto the interior matching curve %i\n",gridLine,i);

      realArray xx(1,K2,K3,Rx);
      xx(0,K2,K3,Rx)=x(gridLine,K2,K3,Rx);
      xx.reshape(xx.getLength(0)*xx.getLength(1)*xx.getLength(2),Rx);
      matchingMap.project(xx,mpParams);
      xx.reshape(1,K2,K3,Rx);
      x(gridLine,K2,K3,Rx)=xx(0,K2,K3,Rx);
    }
  }
  

  if( domainDimension<rangeDimension )
  {
    // *** surface grid ***

    bool extrapolateNormals=false;

    if( projectSmoothedGridOntoReferenceSurface && projectSurfaceGrids )
    {
      // project the grid onto the original surface (project ghost points as well)

      const int numK=K1.getLength()*K2.getLength()*K3.getLength();
      realArray xK(K1,K2,K3,Rx);
      
      xK(K1,K2,K3,Rx)=x(K1,K2,K3,Rx);
      //::display(xK,"xK before project","%8.2e ");

      xK.reshape(numK,Rx);

      Range R=numK;
      realArray & surfaceNormal= mpParams.getRealArray(MappingProjectionParameters::normal);
      if( surfaceNormal.dimension(0)!=R ) 
      {
	// for a composite surface surfaceNormal holds the previous normal on input to project
	mpParams.reset();
	surfaceNormal.redim(R,3);
	surfaceNormal=0.;       
      }
      
      
      map.project(xK,mpParams);
      xK.reshape(K1,K2,K3,Rx);
      maximumProjectionCorrection=max(fabs(x(K1,K2,K3,Rx)-xK(K1,K2,K3,Rx)));
      
      // ::display(xK,"xK after project","%8.2e ");
      if( projectSmoothedGridOntoReferenceSurface )
      {
        Range all;
	int numberOfRegionsNotToProject=sum(regionsNotToProject(all,0)>=0);
	if( numberOfRegionsNotToProject==0 )
	{
	  x(K1,K2,K3,Rx)=xK(K1,K2,K3,Rx);
	}
	else
	{
          // first project all points then undo below
	  realArray xSmooth(J1,J2,J3,Rx), normalSmooth(K1,K2,K3,Rx);
	  xSmooth=x(J1,J2,J3,Rx);  // This is the smoothed surface (we need ghost points for normals below)

	  x(K1,K2,K3,Rx)=xK(K1,K2,K3,Rx);  // project all points 

	  const real alpha=blendingFactor;
	  for( int n=0; n<=regionsNotToProject.getBound(0); n++ )
	  {
	    if( regionsNotToProject(n,0)>=0 )
	    {
	      int i1a=max(K1.getBase(),min(K1.getBound(),regionsNotToProject(n,1)));
	      int i1b=max(K1.getBase(),min(K1.getBound(),regionsNotToProject(n,2)));
	      int i2a=max(K2.getBase(),min(K2.getBound(),regionsNotToProject(n,3)));
	      int i2b=max(K2.getBase(),min(K2.getBound(),regionsNotToProject(n,4)));
	      if( i1a<=i1b && i2a<=i2b )
	      {
		Index R1=Range(i1a,i1b), R2=Range(i2a,i2b);
                printf("applyBC: blend proj and smoothed: blendingFactor=%5.2f rectangle id=%i [%i,%i][%i,%i] "
                         "  K=projected=[%i,%i][%i,%i]\n",
                       alpha,regionsNotToProject(n,0),i1a,i1b,i2a,i2b,K1.getBase(),K1.getBound(),
                       K2.getBase(),K2.getBound());
		
                x(R1,R2,K3,Rx)=(1.-alpha)*x(R1,R2,K3,Rx)+alpha*xSmooth(R1,R2,K3,Rx);

                // compute normals from the surface
                R1=Range(max(I1.getBase(),R1.getBase()),min(I1.getBound(),R1.getBound())); 
                R2=Range(max(I2.getBase(),R2.getBase()),min(I2.getBound(),R2.getBound())); 
                computeNormals( normalSmooth, R1,R2,K3, xSmooth );
                normal(R1,R2,K3,Rx)=(1.-alpha)*normal(R1,R2,K3,Rx)+alpha*normalSmooth(R1,R2,K3,Rx);

                // should we extrap normals??
	      }
	    }
	  }
	}
	
      }
      
      periodicUpdate(x,indexRange);

      // For a surface grid assign ghost points on the normal if we did not project the ghost points
      if( domainDimension==2 && rangeDimension==3 )
      {
        surfaceNormal.reshape(K1,K2,K3,Rx);
        normal(K1,K2,K3,Rx)=surfaceNormal(K1,K2,K3,Rx);
	surfaceNormal.reshape(numK,Rx);

	extrapolateNormals=true;
        N1=K1; N2=K2; N3=K3;  // normals were assigned at these points
	
      }
    }
    else if( projectSurfaceGrids )
    {
      // we need a normal for the surface grid generator
      // If we don't project onto the surface then we just compute the normal from the grid itself.
      computeNormals( normal, I1,I2,I3, x );
//        realArray norm(I1,I2,I3),xr1(I1,I2,I3,3),xr2(I1,I2,I3,3);
//        for( int dir=0; dir<rangeDimension; dir++ )
//        {
//  	xr1(I1,I2,I3,dir)=x(I1+1,I2,I3,dir)-x(I1-1,I2,I3,dir);
//  	xr2(I1,I2,I3,dir)=x(I1,I2+1,I3,dir)-x(I1,I2-1,I3,dir);
//        }

//        normal(I1,I2,I3,0)=xr1(I1,I2,I3,1)*xr2(I1,I2,I3,2)-xr1(I1,I2,I3,2)*xr2(I1,I2,I3,1);
//        normal(I1,I2,I3,1)=xr1(I1,I2,I3,2)*xr2(I1,I2,I3,0)-xr1(I1,I2,I3,0)*xr2(I1,I2,I3,2);
//        normal(I1,I2,I3,2)=xr1(I1,I2,I3,0)*xr2(I1,I2,I3,1)-xr1(I1,I2,I3,1)*xr2(I1,I2,I3,0);

//        norm=max( REAL_MIN*100., sqrt(SQR(normal(I1,I2,I3,0))+SQR(normal(I1,I2,I3,1))+SQR(normal(I1,I2,I3,2))) );
//        normal(I1,I2,I3,0)/norm;
//        normal(I1,I2,I3,1)/norm;
//        normal(I1,I2,I3,2)/norm;
      
      extrapolateNormals=true;

      N1=I1; N2=I2; N3=I3; // normals were assigned at these points
    }
    
    if( extrapolateNormals )
    {
      // We smooth points from Iv=(I1,I2,I3) : we have normals at Nv=(N1,N2,N3) 
      // We need normals at Iv plus ghost points
      int dir;
      Ibv[2]=I3;
      for( dir=0; dir<domainDimension; dir++ )
	Ibv[dir]=Range(Iv[dir].getBase()-1,Iv[dir].getBound()+1);
	
      for( dir=0; dir<domainDimension; dir++ )
      {
	int is1=dir==0 ? 1 : 0;
	int is2=dir==0 ? 0 : 1;
	if( Iv[dir].getBase()<=Nv[dir].getBase() ) // this means the ghost point was  not projected
	{
          assert( Iv[dir].getBase()==Nv[dir].getBase() );
	  
	  Ibv[dir]=Iv[dir].getBase()-1;
	  normal(Ib1,Ib2,Ib3,Rx)=normal(Ib1+is1,Ib2+is2,Ib3,Rx);  // extrapolate the normal
	  Ibv[dir]=Range(Iv[dir].getBase()-1,Iv[dir].getBound()+1);
	}
	if( Iv[dir].getBound()>=Nv[dir].getBound() )
	{
          assert( Iv[dir].getBound()==Nv[dir].getBound() );
	  
	  Ibv[dir]=Iv[dir].getBound()+1;
	  normal(Ib1,Ib2,Ib3,Rx)=normal(Ib1-is1,Ib2-is2,Ib3,Rx); // extrapolate the normal
	  Ibv[dir]=Range(Iv[dir].getBase()-1,Iv[dir].getBound()+1);
	}
      }

      if( smoothNormals )
      {
        printf("smooth surface normals %i times\n",numberOfNormalSmooths*2);
	
        realArray normal2(normal.dimension(0),normal.dimension(1),normal.dimension(2),normal.dimension(3));
        real omegaNormal=.75;
	::display(normal,"normal before smooth","%5.2f ");
	smoothSurfaceNormals(rangeDimension, 
		      x.getBase(0),x.getBound(0),x.getBase(1),x.getBound(1),x.getBase(2),x.getBound(2),
		      I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
		      numberOfNormalSmooths,omegaNormal,
                      *normal.getDataPointer(),*normal2.getDataPointer());
	::display(normal,"normal after smooth","%5.2f ");

      }
    }

  }
  // ::display(x,"x after appplyBC","%8.2e ");


  return 0;
}

int GridSmoother::
smooth(Mapping & map,
       DataPointMapping & dpm, 
       GenericGraphicsInterface & gi, 
       GraphicsParameters & parameters,
       int projectGhost[2][3] )
// ========================================================================================================
/// \details 
///     This is an interactive routine that can used to smooth a DataPointMapping that sits
///   on an underlying Mapping (such as a HyperbolicSurfaceMapping that is created on some other surface).
/// 
///   The grid can be smoothed by a combination of :
///       (1) elliptic grid generation (default). The elliptic grid generator can use the initial grid as
///           a "control grid" -- this prevents the smoothed grid from changing too much from the original.
///       (2) simple Laplacian smoothing 
///       (3) an equidistribution method. The equidistribution method smooths the grid by attempting 
///           to equidistribute a weight function that is a combination of measures of the arclength, 
///           curvature and area.
///     
/// \param map (input) : This is the Mapping that really defines the surface or the boundary of a 3D volume.
///        This Mapping is used to project back onto after each smoothing step.
///  dpm (input) : This is the DataPointMapping that ...
// ========================================================================================================
{

  typedef MappingProjectionParameters MPP;
  

//    intArray & subSurfaceIndex = mpParams.getIntArray(MPP::subSurfaceIndex);
//    subSurfaceIndex=-1;  // set initial guess
      
  assert( domainDimension == dpm.getDomainDimension() );
  assert( rangeDimension == dpm.getRangeDimension() );

  // Here is the array of grid points
  const realArray & xy = dpm.getDataPoints();  // grid points plus ghost points

  // Save a copy of the initial so we can reset the grid
  // (since this routine over-writes the DPM)
  bool & initialGridHasBeenSaved= dbase.get<bool>("initialGridHasBeenSaved");
  if( !initialGridHasBeenSaved )
  {
    // --- make a copy of the initial grid for resets ---

    printF("--GSM-- save initial grid for resets.\n");

    if( !dbase.has_key("xyInitial") )
      dbase.put<realArray>("xyInitial");
      
    realArray & xyInitial = dbase.get<realArray>("xyInitial");
    xyInitial.redim(xy);
    xyInitial=xy;

    initialGridHasBeenSaved=true;
  }


  IntegerArray indexRange(2,3);
  indexRange=0;
  // maxGhost = maximum number of ghost points that we can smooth
  int maxGhost=INT_MAX;
  for( int axis=0; axis<domainDimension; axis++ )
  {
    indexRange(End,axis)=dpm.getGridDimensions(axis)-1;
    maxGhost=min(maxGhost, indexRange(0,axis)-xy.getBase(axis),xy.getBound(axis)-indexRange(1,axis));
    if( dpm.getIsPeriodic(axis) )
    {
      bc(0,axis)=bc(1,axis)=(int)periodic;    // *****
      printf(" dpm.getIsPeriodic(%i) = %i \n",axis,dpm.getIsPeriodic(axis));
    }
  }
  

  // *********** gids: smooth this sub-set of points ***************
  IntegerArray gids; 
  gids=indexRange;
  // adjust for the boundary offset
  for( int axis=0; axis<domainDimension; axis++ )
  {
      
    gids(Start,axis)=max(xy.getBase(axis),min(xy.getBound(axis),indexRange(Start,axis)+smoothingOffset[Start][axis]));
    gids(End  ,axis)=max(xy.getBase(axis),min(xy.getBound(axis),indexRange(End  ,axis)-smoothingOffset[End][axis]));

    if( bc(Start,axis)==pointsFixed )
      gids(Start,axis)+=1;

    if( bc(End,axis)==pointsFixed )
      gids(End,axis)-=1;

    

  }
  // gids(0,1)=max(1,gids(0,1));  


  Range Rx=rangeDimension;

  // I1,I2,I3 : These are the points we smooth
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  ::getIndex(gids,I1,I2,I3); 

  // J1,J2,J3 : include ghost points
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];

  // smooth (at most) this many ghost points: 
  const int & numberOfGhostPointsToSmooth = dbase.get<int>("numberOfGhostPointsToSmooth");
  
  // Actual number of ghost to smooth
  int numGhostToSmooth = min( maxGhost,numberOfGhostPointsToSmooth);
  
  // (J1,J2,J3) : includes ghost to smooth
  ::getIndex(indexRange,J1,J2,J3,numGhostToSmooth);  

  // K1,K2,K3 : set of points to project (include appropriate ghost points)
  Index Kv[3], &K1=Kv[0], &K2=Kv[1], &K3=Kv[2];
  K1=0; K2=0; K3=0;
  for( int axis=0; axis<domainDimension; axis++ )
  {
    int ka=projectGhost[0][axis] ? (bc(0,axis)==pointsFixed ? 2 : 1) : 0;
    int kb=projectGhost[1][axis] ? (bc(1,axis)==pointsFixed ? 2 : 1) : 0;
    
    Kv[axis]=Range(gids(0,axis)-ka,gids(1,axis)+kb);
  }
  
  bool & resetGrid = dbase.get<bool>("resetGrid");
  if( resetGrid )
  {
    printF("--GSM-- reset the grid.\n");

    assert( initialGridHasBeenSaved );

    assert( dbase.has_key("xyInitial") );
    realArray & xyInitial = dbase.get<realArray>("xyInitial");
    dpm.setDataPoints(xyInitial(J1,J2,J3,Rx),3,domainDimension,0,indexRange); 
    
    resetGrid=false;
    return 0;
  }


  printF("             ************ GridSmoother ******\n"
         "  indexRange=[%i,%i][%i,%i][%i,%i] dim=[%i,%i][%i,%i][%i,%i] ghost=[%i,%i][%i,%i][%i,%i] (DataPointMapping bounds)\n"
         "  I=[%i,%i][%i,%i][%i,%i], gids= [%i,%i][%i,%i][%i,%i] : points to smooth\n"
         "  J=[%i,%i][%i,%i][%i,%i] : points to smooth including numGhostToSmooth=%i,\n"
         "  K=[%i,%i][%i,%i][%i,%i] : points to project,\n"
         "  projectGhost=[%i,%i][%i,%i][%i,%i],  bc=[%i,%i][%i,%i][%i,%i]\n",
	 indexRange(0,0),indexRange(1,0),indexRange(0,1),indexRange(1,1),indexRange(0,2),indexRange(1,2),
	 xy.getBase(0),xy.getBound(0),xy.getBase(1),xy.getBound(1),xy.getBase(2),xy.getBound(2),
         indexRange(0,0)-xy.getBase(0),xy.getBound(0)-indexRange(1,0),
         indexRange(0,1)-xy.getBase(1),xy.getBound(1)-indexRange(1,1),
         indexRange(0,2)-xy.getBase(2),xy.getBound(2)-indexRange(1,2),
         I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
	 gids(0,0),gids(1,0),gids(0,1),gids(1,1),gids(0,2),gids(1,2),
         J1.getBase(),J1.getBound(),J2.getBase(),J2.getBound(),J3.getBase(),J3.getBound(),numGhostToSmooth,
         K1.getBase(),K1.getBound(),K2.getBase(),K2.getBound(),K3.getBase(),K3.getBound(),
         projectGhost[0][0],projectGhost[1][0],
         projectGhost[0][1],projectGhost[1][1],
         projectGhost[0][2],projectGhost[1][2],
         bc(0,0),bc(1,0),bc(0,1),bc(1,1),bc(0,2),bc(1,2));


  realArray x(J1,J2,J3,rangeDimension);
  realArray rI(I1,I2,I3,domainDimension),xI(I1,I2,I3,rangeDimension);
  
  realArray normal;  // holds normal for surface grids

  realArray omegav(J1,J2,J3);
  omegav=omega;  // we allow omega to vary
  
  const int numI = I1.length()*I2.length()*I3.length();
  const int numJ = J1.length()*J2.length()*J3.length();

  
  real dr[3]; 
  for( int axis=0; axis<3; axis++ )
    dr[axis]=1./max(1,indexRange(1,axis)-indexRange(0,axis));

  const real eps = REAL_MIN*1000.;  // used to avoid dividing by a zero "diagonal coeff" in the elliptic equations.
  real alpha=1.;  // smoothing coefficient in BC for tangential component, alpha=0 --> orthogonal BC

  // We are testing out different options for computing ghost points
  //  0 = old method -- pre November 2015
  //  1 = new method
  const int & ghostPointOption = dbase.get<int>("ghostPointOption");
  
  int option=0;
  int iparam[] = {option,bc(0,0),bc(1,0),bc(0,1),bc(1,1),bc(0,2),bc(1,2), numGhostToSmooth, ghostPointOption};  //
  real rparam[] = { dr[0],dr[1],dr[2],eps,alpha }; //

  if( !useNewBoundaryConditions )
  { // do not use new BC's 
    for( int m=1; m<=6; m++ ) 
      if( iparam[m]==pointsSlide ) 
        iparam[m]=pointsFixed;   
  }

  if( numberOfEllipticSmooths>0 && !controlFunctionComputed )
  {
    controlFunctionComputed=true;

    Range Dx=domainDimension;
    source.redim(J1,J2,J3,domainDimension); 
    source=0.;

    // const realArray & xy = dpm.getDataPoints();  // includes ghost points
    // Here is the initial grid: 
    const realArray & xy = dbase.get<realArray>("xyInitial");
    x(J1,J2,J3,Rx)=xy(J1,J2,J3,Rx);

    if( domainDimension==2 && rangeDimension==3 )
    {
      // for surface grids we must compute the normal the first time
      normal.redim(J1,J2,J3,rangeDimension);
      applyBoundaryConditions(map,dpm,x,indexRange,gids,Iv,Jv,Kv,normal);
      // ::display(normal,"normal","%8.2e ");
    }

    if( useInitialGridAsControlGrid )
    {
      // **** determine the source so that the current grid would be the solution ****
      //       compute control functions to match current solution

      option=1; // option=1 : compute control functions to match current solution
      iparam[0]=option;
      ellipticSmooth( domainDimension, rangeDimension, *indexRange.getDataPointer(),
                      x.getBase(0),x.getBound(0),x.getBase(1),x.getBound(1),x.getBase(2),x.getBound(2),
		      I1.getBase(),I1.getBound(),1,I2.getBase(),I2.getBound(),1,I3.getBase(),I3.getBound(),1,
		      *omegav.getDataPointer(), *x.getDataPointer(), *source.getDataPointer(), 
                      *normal.getDataPointer(),iparam[0],rparam[0] );

      // **** smooth the control function ****
      Index Ib1,Ib2,Ib3,Ig1,Ig2,Ig3;
      real omega=.8;
      for( int it=0; it<numberOfControlFunctionSmooths; it++ )
      {
        // Boundary conditions (do first to fill in ghost values)
	for( int side=0; side<=1; side++ )
	{
	  for( int axis=0; axis<domainDimension; axis++ )
	  {
	    if( bc(side,axis)!=(int)periodic )
	    {
	      const int extra=0;  
	      getBoundaryIndex(gids,side,axis,Ib1,Ib2,Ib3,extra);
	      getGhostIndex(gids,side,axis,Ig1,Ig2,Ig3,+1,extra); // first ghost line
	      if( true ) 
		printf("smooth control function:BC: it=%i:  (side,axis)=(%i,%i) points [%i,%i][%i,%i]\n",
		       it,side,axis,Ig1.getBase(),Ig1.getBound(),Ig2.getBase(),Ig2.getBound());
	  
	      source(Ig1,Ig2,Ig3,Dx)=source(Ib1,Ib2,Ib3,Dx);
		
	    }
	  }
	}
        periodicUpdate( source, indexRange ); // is this correct?

        if( domainDimension==2 )
	{
          // smooth P along i2 and Q along i1 in order to maintain spacing 
	  source(I1,I2,I3,0)=(1.-omega)*source(I1,I2,I3,0)+(omega*.5)*(source(I1,I2-1,I3,0)+source(I1,I2+1,I3,0));
	  source(I1,I2,I3,1)=(1.-omega)*source(I1,I2,I3,1)+(omega*.5)*(source(I1-1,I2,I3,1)+source(I1+1,I2,I3,1));
	}
        else
	{

          // smooth P along i2,i3 and Q along i1,i3 and R along i1,i2 in order to maintain spacing 
	  source(I1,I2,I3,0)=(1.-omega)*source(I1,I2,I3,0)+(omega*.25)*(source(I1,I2-1,I3,0)+source(I1,I2+1,I3,0)+
									source(I1,I2,I3-1,0)+source(I1,I2,I3+1,0));
	  source(I1,I2,I3,1)=(1.-omega)*source(I1,I2,I3,1)+(omega*.25)*(source(I1-1,I2,I3,1)+source(I1+1,I2,I3,1)+
									source(I1,I2,I3-1,1)+source(I1,I2,I3+1,1));
	  source(I1,I2,I3,2)=(1.-omega)*source(I1,I2,I3,2)+(omega*.25)*(source(I1-1,I2,I3,2)+source(I1+1,I2,I3,2)+
									source(I1,I2-1,I3,2)+source(I1,I2+1,I3,2));
	}

      }
      // ::display(source,"source","%8.2e ");

    }
    else if( false )
    {
      // Compute the control functions lines and points of attraction -- these are fixed
      fixedControlFunctions( domainDimension, rangeDimension, 
			     x.getBase(0),x.getBound(0),x.getBase(1),x.getBound(1),x.getBase(2),x.getBound(2),
			     J1.getBase(),J1.getBound(),1,J2.getBase(),J2.getBound(),1,J3.getBase(),J3.getBound(),1,
			     *source.getDataPointer(),dr[0],ipar.getLength(0),rpar.getLength(0),
			     ipar.getLength(1),ipar(0,0),rpar(0,0) );
    }
    
  }
  
  for( int it=0; it<numberOfIterations; it++ )
  {
    const realArray & xy = dpm.getDataPoints();  // includes ghost points
    x(J1,J2,J3,Rx)=xy(J1,J2,J3,Rx);

    // ***************************************************
    // ************** Equidistribute *********************
    // ***************************************************

    // *********** We equidistribute one axis at a time *********
    for( int ite=0; ite<numberOfEquidistributionIterations; ite++ )
    {
      for( int axis=axis1; axis<domainDimension; axis++ )
      {
	// redistribute points in the direction axis:
	// We smooth the points defined by indexRange (1 ghost line needed)
	// ---> returns r(I1,I2,I3,.) : new positions of grdi points in parameter space

        // ***** what about bc here ****  indexRange->gids but what about BC's
	equiGridSmoother(domainDimension,rangeDimension,indexRange,bc, axis,
			 xy,rI, // return rI 
			 arclengthWeight,curvatureWeight,areaWeight,numberOfWeightSmooths);

	// ::display(rI,"GridSmoother: Here are the r coord's of the equidistributed grid","%5.3f ");

	// ********* re-evaluate the positions of the grid points **********
	rI.reshape(numI,domainDimension);
	xI.reshape(numI,rangeDimension);
	dpm.map(rI,xI);                          // do not include ghost points
	rI.reshape(I1,I2,I3,domainDimension);
	xI.reshape(I1,I2,I3,rangeDimension);

        // x(I1,I2,I3,Rx)=xI(I1,I2,I3,Rx);
        // *** Here we average the results from the equid's in each direction ***
        if( axis==0 )
          x(I1,I2,I3,Rx)=(1./domainDimension)*xI(I1,I2,I3,Rx);
        else
          x(I1,I2,I3,Rx)+=(1./domainDimension)*xI(I1,I2,I3,Rx);
      
      
      }
      applyBoundaryConditions(map,dpm,x,indexRange,gids,Iv,Jv,Kv,normal);
    }

    
    if( numberOfEllipticSmooths>0 && domainDimension==2 && rangeDimension==3 && it==0 )
    {
      // for surface grids we must compute the normal the first time
      normal.redim(J1,J2,J3,rangeDimension);
      applyBoundaryConditions(map,dpm,x,indexRange,gids,Iv,Jv,Kv,normal);
      // ::display(normal,"normal","%8.2e ");
    }
      
    if( numberOfEllipticSmooths>0 )
    {
      assert( x.dimension(0)==source.dimension(0) && 
              x.dimension(1)==source.dimension(1) &&
              x.dimension(2)==source.dimension(2) );
    }
    
    // ************************************************************
    // ************** Elliptic Grid Generation ********************
    // ************************************************************
    for( int jt=0; jt<numberOfEllipticSmooths; jt++ )
    {
      // these are sub-smooths where we do not project surface grids, except for the last iteration
      //::display(x,"x before ellipticSmooth","%8.2e ");
      //::display(normal,"normal before ellipticSmooth","%8.2e ");

      option=0;
      iparam[0]=option;
      ellipticSmooth( domainDimension, rangeDimension, *indexRange.getDataPointer(),
                      x.getBase(0),x.getBound(0),x.getBase(1),x.getBound(1),x.getBase(2),x.getBound(2),
		      I1.getBase(),I1.getBound(),1,I2.getBase(),I2.getBound(),1,I3.getBase(),I3.getBound(),1,
		      *omegav.getDataPointer(), *x.getDataPointer(), *source.getDataPointer(),
                      *normal.getDataPointer(),iparam[0],rparam[0] );
      
      // ::display(x,"x after ellipticSmooth","%8.2e ");

      bool projectSurfaceGrids=jt==(numberOfEllipticSmooths-1);
      applyBoundaryConditions(map,dpm,x,indexRange,gids,Iv,Jv,Kv,normal,projectSurfaceGrids);
    }
    

    // ***********************************************************
    // ****************** Laplacian Smooths **********************
    // ***********************************************************
    for( int jt=0; jt<numberOfLaplacianSmooths; jt++ )
    {
      if( domainDimension==2 )
      {
	x(I1,I2,I3,Rx)+=(omega*.25)*(x(I1+1,I2,I3,Rx)+x(I1-1,I2,I3,Rx)+
				     x(I1,I2+1,I3,Rx)+x(I1,I2-1,I3,Rx)-4.*x(I1,I2,I3,Rx));
      }
      else
      {
	x(I1,I2,I3,Rx)+=(omega/6.)*(x(I1+1,I2,I3,Rx)+x(I1-1,I2,I3,Rx)+
				    x(I1,I2+1,I3,Rx)+x(I1,I2-1,I3,Rx)+
				    x(I1,I2,I3+1,Rx)+x(I1,I2,I3+1,Rx)-6.*x(I1,I2,I3,Rx));

      }
      applyBoundaryConditions(map,dpm,x,indexRange,gids,Iv,Jv,Kv,normal);
    }
      
    real xDiff=max(fabs(x(J1,J2,J3,Rx)-xy(J1,J2,J3,Rx)));
    if( domainDimension==2 && rangeDimension==3 )
    {
      printf("it %i, smooth:I=[%i,%i][%i,%i][%i,%i]  |projection|=%8.2e |correction| = %8.2e \n",totalIterations,
	     I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
             maximumProjectionCorrection,xDiff);
    }
    else
    {
      printf("it %i, smooth:I=[%i,%i][%i,%i][%i,%i] |correction| = %8.2e \n",totalIterations,
	     I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),xDiff);
      
    }
    
      dpm.setDataPoints(x(J1,J2,J3,Rx),3,domainDimension,0,indexRange); 

    totalIterations++;
  } // end for it


  return 0;
}

