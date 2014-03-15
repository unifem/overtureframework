#include "HyperbolicMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "DataPointMapping.h"
#include "OGgetIndex.h"
#include "EquiDistribute.h"
#include "display.h"
#include "TridiagonalSolver.h"
#include "GL_GraphicsInterface.h"



int HyperbolicMapping::
variationalGeneration()
// ======================================================================================
//  /Description:
//     Smooth a grid using variational equations.
// ======================================================================================
{

  Index D1,D2,D3;
  ::getIndex(dimension,D1,D2,D3);
    
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  ::getIndex(indexRange,I1,I2,I3); 


  RealArray & x = xHyper;

  const real omega=.5;
  if( domainDimension==2 )
  {
    x(I1,I2,I3,xAxes)=(1.-omega)x(I1,I2,I3,xAxes) +
      .25*omega*( x(I1+1,I2,I3,xAxes)+x(I1-1,I2,I3,xAxes)+x(I1,I2+1,I3,xAxes)+x(I11,I2-1,I3,xAxes) );

    // boundary conditions.
  }


  if( domainDimension==2 )
  {
    x.reshape(x.dimension(0),x.dimension(2),1,xAxes);
    dpm->setDataPoints(x(I1,I3,0,xAxes),3,domainDimension);
    x.reshape(x.dimension(0),1,x.dimension(1),xAxes);
  }
  else
    dpm->setDataPoints(x(I1,I2,I3,xAxes),3,domainDimension);



  return 0;
}




int HyperbolicMapping::
smooth(GenericGraphicsInterface & gi, GraphicsParameters & parameters)
// ======================================================================================
//  /Description:
//     Smooth a grid.
// ======================================================================================
{

  if( surface==NULL || dpm==NULL )
  {
    gi.outputString("Generate the hyperbolic grid first, before smoothing\n");
  }

/* ----
  real arcLengthWeight_,curvatureWeight_,areaWeight_;
  IntegerArray boundaryCondition0(2,3);
  boundaryCondition0=1;
  equiGridSmoother(*surface,*dpm,gi,parameters,boundaryCondition0,arcLengthWeight_,curvatureWeight_,areaWeight_);
--- */

  
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  aString menu[] = 
    {
      "smooth",
      "show parameters",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "smooth             : smooth grid (after generation)",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  aString answer,line,answer2; 

  bool plotObject=TRUE;


  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  gi.appendToTheDefaultPrompt("smooth>"); // set the default prompt

  bool plotReferenceSurface=TRUE;
  bool plotShadedMappingBoundaries =FALSE;
  bool choosePlotBoundsFromReferenceSurface=FALSE;
  bool plotHyperbolicSurface=FALSE;
  
  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="smooth" )
    { 
    else if( answer=="plot reference surface (toggle)" )
    {
      plotReferenceSurface=!plotReferenceSurface;
      plotObject=TRUE;
    }
    else if( answer=="plot shaded boundaries on reference surface (toggle)" )
    {
      plotShadedMappingBoundaries = !plotShadedMappingBoundaries;   
    }
    else if( answer=="plot bounds derived from reference surface (toggle)" )
    {
      choosePlotBoundsFromReferenceSurface=!choosePlotBoundsFromReferenceSurface;
    }
    else if( answer=="normal curvature weight" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the normal curvature weight (default=%e)",normalCurvatureWeight));
      if( line!="" )
	sScanF( line,"%e",&normalCurvatureWeight);
    }
    else if( answer=="show parameters" )
    {
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
      continue;
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=TRUE;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }

    if( plotObject )
    {
      gi.erase();
      if( plotReferenceSurface && surface!=NULL && ( surfaceGrid || abs(growthOption)==2 || dpm==NULL ) )
      {
        // plot reference surface
        parameters.set(GI_TOP_LABEL,getName(mappingName)+" (initial surface)");
//        parameters.set(GI_SURFACE_OFFSET,(real)20.);  
	parameters.set(GI_MAPPING_COLOUR,"blue");
        parameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,plotShadedMappingBoundaries);
        PlotIt::plot(gi,*surface,parameters);   // *** recompute every time ?? ***
        parameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,TRUE );
//        parameters.set(GI_SURFACE_OFFSET,(real)3.);  

        parameters.set(GI_USE_PLOT_BOUNDS,TRUE); 

      }
      if( surfaceGrid  && startCurve!=NULL )
      { 
        // plot the initial curve
        parameters.set(GI_USE_PLOT_BOUNDS,TRUE); 
	parameters.set(GI_MAPPING_COLOUR,"green");
	real oldCurveLineWidth;
	parameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
	parameters.set(GraphicsParameters::curveLineWidth,3.);
	PlotIt::plot(gi,*startCurve,parameters);  
	parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
      }
      if( plotHyperbolicSurface && dpm!=NULL )
      {
        // plot hyperbolic surface
        parameters.set(GI_TOP_LABEL,getName(mappingName));
        parameters.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,TRUE);
	parameters.set(GI_MAPPING_COLOUR,"red");
        PlotIt::plot(gi,*this,parameters);   // *** recompute every time ?? ***
        parameters.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,FALSE);
      }
      // plot any mappings used for boundary conditions
      for( int axis=0; axis<domainDimension-1; axis++ )
      {
	for( int side=0; side<=1; side++ )
	{
	  if( boundaryCondition(side,axis)==matchToMapping  && boundaryConditionMapping[side][axis]!=NULL  )
	  {
            parameters.set(GI_USE_PLOT_BOUNDS,TRUE); 
	    PlotIt::plot(gi,*boundaryConditionMapping[side][axis],parameters);
	  }
	}
      }
      

      if( !choosePlotBoundsFromReferenceSurface )
        parameters.set(GI_USE_PLOT_BOUNDS,FALSE); 

    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;



  return 0;
}


