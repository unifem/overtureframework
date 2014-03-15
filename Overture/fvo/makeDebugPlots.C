#include "CompositeGridFunction.h"
#include "GenericGraphicsInterface.h"
#include "PlotIt.h"

//================================================================================
void 
makeDebugContourPlots (const realCompositeGridFunction &u, 
		       const aString &label,
		       const int &component,
		       GenericGraphicsInterface *ps,
		       GraphicsParameters *psp)
//
// function for internal debug contour plots
// 981105 DLB
{
  
  if (!ps || !psp) return;
  psp->set (GI_TOP_LABEL, label);
  psp->set (GI_COMPONENT_FOR_CONTOURS, component);
  PlotIt::contour (*ps, u, *psp);
}

void
makeDebugContourPlots (const realCompositeGridFunction &u, 
		       const aString &label,
		       const Index &components,
		       GenericGraphicsInterface *ps,
		       GraphicsParameters *psp)
{
  if (!ps || !psp) return;
  psp->set (GI_TOP_LABEL, label);
  for (int i=components.getBase(); i<=components.getBound(); i++)
  {
    psp->set (GI_COMPONENT_FOR_CONTOURS, i);
    PlotIt::contour (*ps, u, *psp);
  }
}

void 
makeDebugStreamLinePlots (const realCompositeGridFunction & u,
			  const aString & label,
			  GenericGraphicsInterface *ps,
			  GraphicsParameters *psp)
//
//
{
  if (!ps || !psp) return;
  psp->set (GI_TOP_LABEL, label);
  PlotIt::streamLines (*ps, u, *psp);
}

