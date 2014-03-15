// ================================================================================================
// Phoney versions of some plot routines to be used when we only build the OvertureMapping library
// ================================================================================================

#include "GL_GraphicsInterface.h"
// include "OvertureTypes.h"

// ******************************* these next two one should be implemented ******
void GL_GraphicsInterface::
plot( const realArray & t, 
      const realArray & x, 
      const aString & title /* = nullString */, 
      const aString & tName /* = nullString */,
      const aString *xName        /* =NULL */,
      GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */ ) 
{
}

void GL_GraphicsInterface::
plot(const realArray & x, 
     const realArray & t,
     const realArray & u_, 
     GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */ )
//=============================================================================
// /Description:
//    Use PlotStuff to make a surface plot of a sequence of 1D functions
// /x (input) : t(0:n-1) - values along the horizontal axis ("x-axis")
// /t (input) : t(0:nv-1) - times corresponding to the different components of u ("y-axis")
// /u (input) : u(0:n-1,0:nv-1) - values to plot, nv components  ("z-axis")
// 
// /parameters: Use these parameters to set the title, sub-titles and axis labels etc.
//\end{PlotStuffInclude.tex}  
//=============================================================================
{
}

int GL_GraphicsInterface::
fileOutput( realGridCollectionFunction & u )
{
  return 0;
}



// Plot a MappedGrid
void GL_GraphicsInterface::
plot(MappedGrid & mg, 
     GraphicsParameters & parameters )
{
}


// Plot contours and/or a shaded surface plot of a realMappedGridFunction in 2D or 3D
void GL_GraphicsInterface::
contour(const realMappedGridFunction & u, 
	GraphicsParameters & parameters)
{
}


// Plot streamlines of a 2D vector field
void GL_GraphicsInterface::
streamLines(const realMappedGridFunction & uv, 
	    GraphicsParameters & parameters)
{
}


// Plot a GridCollection or Composite grid
void GL_GraphicsInterface::
plot(GridCollection & cg, 
     GraphicsParameters & parameters)
{
}


// Plot contours and/or a shaded surface plot of a GridCollectionFunction/CompositeGridFunction in 2D
void GL_GraphicsInterface::
contour(const realGridCollectionFunction & u, 
	GraphicsParameters & parameters)
{
}


// Plot streamlines of a vector field
void GL_GraphicsInterface::
streamLines(const realGridCollectionFunction & uv, 
	    GraphicsParameters & parameters)
{
}


// plot an advancing front
void GL_GraphicsInterface::
plot(AdvancingFront & front, 
     GraphicsParameters & parameters)
{
}

