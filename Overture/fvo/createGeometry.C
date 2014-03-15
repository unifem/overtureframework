#include "MappedGridFiniteVolumeOperators.h"
// =================================================================================
void MappedGridFiniteVolumeOperators::
createFaceNormal (MappedGrid & mg)
// =================================================================================
{
	//
	// compute faceNormal for use by differentiation operators
	//


  if (debug) createFaceNormalDisplay.interactivelySetInteractiveDisplay ("createFaceNormalDisplay initialization");

  REALMappedGridFunction x, y, z;

  x.link (mg.vertex(), Range(xComponent,xComponent));
  y.link (mg.vertex(), Range(yComponent,yComponent));
  if (numberOfDimensions > 2) z.link (mg.vertex(), Range(zComponent,zComponent));

  faceNormal.updateToMatchGrid (mg, all, all, all, faceRange, numberOfDimensions);
  faceNormal.setFaceCentering (GridFunctionParameters::all);

  Index I1, I2, I3;

  if (numberOfDimensions == 2)
  {

    int extra = max (mg.numberOfGhostPoints());

	// ========================================
	// getIndex for rx,ry
	// ========================================

//    int component;
//    component = ndnd(rAxis,0);
//    getIndex (faceNormal, component, I1, I2, I3, extra);
    getIndex (faceNormal, rAxis, I1, I2, I3, extra);


    faceNormal(I1,I2,I3,rAxis,xAxis) =  (y(I1  ,I2+1,I3) - y(I1,I2,I3));
    faceNormal(I1,I2,I3,rAxis,yAxis) = -(x(I1  ,I2+1,I3) - x(I1,I2,I3));
	
//    int component = ndnd(sAxis,0);	 
    getIndex (faceNormal, sAxis, I1, I2, I3, extra);


    faceNormal(I1,I2,I3,sAxis,xAxis) = -(y(I1+1,I2  ,I3) - y(I1,I2,I3));
    faceNormal(I1,I2,I3,sAxis,yAxis) =  (x(I1+1,I2  ,I3) - x(I1,I2,I3));

  }

  if (numberOfDimensions == 3)
  {
    REAL half = 1./2.;
    int extra = max (mg.numberOfGhostPoints());


		//  rface:
		// 	001	011
		//
		//	000	010
		//

    getIndex (faceNormal, ndnd(rAxis,0), I1, I2, I3, extra);



    faceNormal(I1,I2,I3,rAxis,xAxis) = half*(
		(y(I1  ,I2+1,I3+1)-y(I1,I2,I3))*(z(I1  ,I2  ,I3+1)-z(I1  ,I2+1,I3))
	       -(z(I1  ,I2+1,I3+1)-z(I1,I2,I3))*(y(I1  ,I2  ,I3+1)-y(I1  ,I2+1,I3))
		 );

    faceNormal(I1,I2,I3,rAxis,yAxis) = half*(
		(z(I1  ,I2+1,I3+1)-z(I1,I2,I3))*(x(I1  ,I2  ,I3+1)-x(I1  ,I2+1,I3))
	       -(x(I1  ,I2+1,I3+1)-x(I1,I2,I3))*(z(I1  ,I2  ,I3+1)-z(I1  ,I2+1,I3))
		 );

    faceNormal(I1,I2,I3,rAxis,zAxis) = half*(
		(x(I1  ,I2+1,I3+1)-x(I1,I2,I3))*(y(I1  ,I2  ,I3+1)-y(I1  ,I2+1,I3))
	       -(y(I1  ,I2+1,I3+1)-y(I1,I2,I3))*(x(I1  ,I2  ,I3+1)-x(I1  ,I2+1,I3))
		 );

		 // sface:
		 //	001	101
		 //
		 //	000	100
		 //

    getIndex (faceNormal, ndnd(sAxis,0), I1, I2, I3, extra);


    faceNormal(I1,I2,I3,sAxis,xAxis) = half*(
		(y(I1+1,I2  ,I3+1)-y(I1,I2,I3))*(z(I1  ,I2  ,I3+1)-z(I1+1,I2  ,I3))
	       -(z(I1+1,I2  ,I3+1)-z(I1,I2,I3))*(y(I1  ,I2  ,I3+1)-y(I1+1,I2  ,I3))
		 );

    faceNormal(I1,I2,I3,sAxis,yAxis) = half*(
		(z(I1+1,I2  ,I3+1)-z(I1,I2,I3))*(x(I1  ,I2  ,I3+1)-x(I1+1,I2  ,I3))
	       -(x(I1+1,I2  ,I3+1)-x(I1,I2,I3))*(z(I1  ,I2  ,I3+1)-z(I1+1,I2  ,I3))
		 );

    faceNormal(I1,I2,I3,sAxis,zAxis) = half*(
		(x(I1+1,I2  ,I3+1)-x(I1,I2,I3))*(y(I1  ,I2  ,I3+1)-y(I1+1,I2  ,I3))
	       -(y(I1+1,I2  ,I3+1)-y(I1,I2,I3))*(x(I1  ,I2  ,I3+1)-x(I1+1,I2  ,I3))
		 );

		 // tface:
		 //	010	110
		 //
		 //	000 	100
		 //
    getIndex (faceNormal, ndnd(tAxis,0), I1, I2, I3, extra);



    faceNormal(I1,I2,I3,tAxis,xAxis) = half*(
		(y(I1+1,I2+1,I3  )-y(I1,I2,I3))*(z(I1  ,I2+1,I3  )-z(I1+1,I2  ,I3))
	       -(z(I1+1,I2+1,I3  )-z(I1,I2,I3))*(y(I1  ,I2+1,I3  )-y(I1+1,I2  ,I3))
		 );

    faceNormal(I1,I2,I3,tAxis,yAxis) = half*(
		(z(I1+1,I2+1,I3  )-z(I1,I2,I3))*(x(I1  ,I2+1,I3  )-x(I1+1,I2  ,I3))
	       -(x(I1+1,I2+1,I3  )-x(I1,I2,I3))*(z(I1  ,I2+1,I3  )-z(I1+1,I2  ,I3))
		 );

    faceNormal(I1,I2,I3,tAxis,zAxis) = half*(
		(x(I1+1,I2+1,I3  )-x(I1,I2,I3))*(y(I1  ,I2+1,I3  )-y(I1+1,I2  ,I3))
	       -(y(I1+1,I2+1,I3  )-y(I1,I2,I3))*(x(I1  ,I2+1,I3  )-x(I1+1,I2  ,I3))
		 );

  }

  faceNormal.periodicUpdate();
  if (debug) createFaceNormalDisplay.display (faceNormal, "DEBUG: faceNormal: ");
  //faceNormal.display ("faceNormal");
  faceNormalDefined = TRUE;
    
}

// =================================================================================
void MappedGridFiniteVolumeOperators::
createFaceNormalCG (MappedGrid & mg)
// =================================================================================
{
	//
	// compute faceNormalCG for use by differentiation operators
	//


  if (debug) createFaceNormalDisplay.interactivelySetInteractiveDisplay ("createFaceNormalDisplay initialization");

  REALMappedGridFunction x, y, z;

  x.link (mg.vertex(), Range(xComponent,xComponent));
  y.link (mg.vertex(), Range(yComponent,yComponent));
  if (numberOfDimensions > 2) z.link (mg.vertex(), Range(zComponent,zComponent));

  faceNormalCG.updateToMatchGrid (mg, all, all, all, numberOfDimensions, faceRange);
  faceNormalCG.setFaceCentering (GridFunctionParameters::all);

  Index I1, I2, I3;

  if (numberOfDimensions == 2)
  {

    int extra = max (mg.numberOfGhostPoints());

	// ========================================
	// getIndex for rx,ry
	// ========================================

//    int component;
//    component = ndnd(rAxis,0);
//    getIndex (faceNormalCG, component, I1, I2, I3, extra);
    getIndex (faceNormalCG, rAxis, I1, I2, I3, extra);


    faceNormalCG(I1,I2,I3,xAxis,rAxis) =  (y(I1  ,I2+1,I3) - y(I1,I2,I3));
    faceNormalCG(I1,I2,I3,yAxis,rAxis) = -(x(I1  ,I2+1,I3) - x(I1,I2,I3));
	
//    int component = ndnd(sAxis,0);	 
    getIndex (faceNormalCG, sAxis, I1, I2, I3, extra);


    faceNormalCG(I1,I2,I3,xAxis,sAxis) = -(y(I1+1,I2  ,I3) - y(I1,I2,I3));
    faceNormalCG(I1,I2,I3,yAxis,sAxis) =  (x(I1+1,I2  ,I3) - x(I1,I2,I3));

  }

  if (numberOfDimensions == 3)
  {
    REAL half = 1./2.;
    int extra = max (mg.numberOfGhostPoints());


		//  rface:
		// 	001	011
		//
		//	000	010
		//

    getIndex (faceNormalCG, rAxis, I1, I2, I3, extra);



    faceNormalCG(I1,I2,I3,xAxis,rAxis) = half*(
		(y(I1  ,I2+1,I3+1)-y(I1,I2,I3))*(z(I1  ,I2  ,I3+1)-z(I1  ,I2+1,I3))
	       -(z(I1  ,I2+1,I3+1)-z(I1,I2,I3))*(y(I1  ,I2  ,I3+1)-y(I1  ,I2+1,I3))
		 );

    faceNormalCG(I1,I2,I3,yAxis,rAxis) = half*(
		(z(I1  ,I2+1,I3+1)-z(I1,I2,I3))*(x(I1  ,I2  ,I3+1)-x(I1  ,I2+1,I3))
	       -(x(I1  ,I2+1,I3+1)-x(I1,I2,I3))*(z(I1  ,I2  ,I3+1)-z(I1  ,I2+1,I3))
		 );

    faceNormalCG(I1,I2,I3,zAxis,rAxis) = half*(
		(x(I1  ,I2+1,I3+1)-x(I1,I2,I3))*(y(I1  ,I2  ,I3+1)-y(I1  ,I2+1,I3))
	       -(y(I1  ,I2+1,I3+1)-y(I1,I2,I3))*(x(I1  ,I2  ,I3+1)-x(I1  ,I2+1,I3))
		 );

		 // sface:
		 //	001	101
		 //
		 //	000	100
		 //

    getIndex (faceNormalCG, sAxis, I1, I2, I3, extra);


    faceNormalCG(I1,I2,I3,xAxis,sAxis) = half*(
		(y(I1+1,I2  ,I3+1)-y(I1,I2,I3))*(z(I1  ,I2  ,I3+1)-z(I1+1,I2  ,I3))
	       -(z(I1+1,I2  ,I3+1)-z(I1,I2,I3))*(y(I1  ,I2  ,I3+1)-y(I1+1,I2  ,I3))
		 );

    faceNormalCG(I1,I2,I3,yAxis,sAxis) = half*(
		(z(I1+1,I2  ,I3+1)-z(I1,I2,I3))*(x(I1  ,I2  ,I3+1)-x(I1+1,I2  ,I3))
	       -(x(I1+1,I2  ,I3+1)-x(I1,I2,I3))*(z(I1  ,I2  ,I3+1)-z(I1+1,I2  ,I3))
		 );

    faceNormalCG(I1,I2,I3,zAxis,sAxis) = half*(
		(x(I1+1,I2  ,I3+1)-x(I1,I2,I3))*(y(I1  ,I2  ,I3+1)-y(I1+1,I2  ,I3))
	       -(y(I1+1,I2  ,I3+1)-y(I1,I2,I3))*(x(I1  ,I2  ,I3+1)-x(I1+1,I2  ,I3))
		 );

		 // tface:
		 //	010	110
		 //
		 //	000 	100
		 //
    getIndex (faceNormalCG, tAxis, I1, I2, I3, extra);



    faceNormalCG(I1,I2,I3,xAxis,tAxis) = half*(
		(y(I1+1,I2+1,I3  )-y(I1,I2,I3))*(z(I1  ,I2+1,I3  )-z(I1+1,I2  ,I3))
	       -(z(I1+1,I2+1,I3  )-z(I1,I2,I3))*(y(I1  ,I2+1,I3  )-y(I1+1,I2  ,I3))
		 );

    faceNormalCG(I1,I2,I3,yAxis,tAxis) = half*(
		(z(I1+1,I2+1,I3  )-z(I1,I2,I3))*(x(I1  ,I2+1,I3  )-x(I1+1,I2  ,I3))
	       -(x(I1+1,I2+1,I3  )-x(I1,I2,I3))*(z(I1  ,I2+1,I3  )-z(I1+1,I2  ,I3))
		 );

    faceNormalCG(I1,I2,I3,zAxis,tAxis) = half*(
		(x(I1+1,I2+1,I3  )-x(I1,I2,I3))*(y(I1  ,I2+1,I3  )-y(I1+1,I2  ,I3))
	       -(y(I1+1,I2+1,I3  )-y(I1,I2,I3))*(x(I1  ,I2+1,I3  )-x(I1+1,I2  ,I3))
		 );

  }

  faceNormalCG.periodicUpdate();
  if (debug) createFaceNormalDisplay.display (faceNormalCG, "DEBUG: faceNormalCG: ");
    
  faceNormalCGDefined = TRUE;
}

// =================================================================================
void MappedGridFiniteVolumeOperators::
createCellVolume (MappedGrid & mg)
// =================================================================================
{
	//
	// compute cellVolume for use by differentiation operators
	//

//  cout << "MappedGridFiniteVolumeOperators::createCellVolume() not yet implemented" << endl;


  if (debug) createCellVolumeDisplay.interactivelySetInteractiveDisplay ("createCellVolumeDisplay initialization");

  REALMappedGridFunction x, y, z;
  x.link (mg.vertex(), Range(xComponent,xComponent));
  y.link (mg.vertex(), Range(yComponent,yComponent));
  if (numberOfDimensions > 2) z.link (mg.vertex(), Range(zComponent,zComponent));

  if (debug) {
    createCellVolumeDisplay.display (x, " DEBUG: This is x:");
    createCellVolumeDisplay.display (y, " DEBUG: This is y:");
    if (numberOfDimensions > 2) createCellVolumeDisplay.display (z, " DEBUG: This is z:");
  }

  cellVolume.updateToMatchGrid (mg, all, all, all);

     // 950619: kludge; the generic case doesn't work

  int axis;
  ForAllAxes(axis) cellVolume.setIsCellCentered (FALSE, axis);
  ForAxes(axis)    cellVolume.setIsCellCentered (TRUE,  axis );

  Index I1, I2, I3;

  if (numberOfDimensions == 2) {

    int extra = max (mg.numberOfGhostPoints());
    getIndex (cellVolume, I1, I2, I3, extra);

    REAL half = 1./2.;

    cellVolume (I1,I2,I3) =
      half*(
        (x(I1+1,I2+1,I3) - x(I1,I2,I3))*(y(I1,I2+1,I3) - y(I1+1,I2,I3))
       -(y(I1+1,I2+1,I3) - y(I1,I2,I3))*(x(I1,I2+1,I3) - x(I1+1,I2,I3))
	   );
  }

  if (numberOfDimensions == 3) {

    int extra = max (mg.numberOfGhostPoints());
    getIndex (cellVolume, I1, I2, I3, extra);

    REAL sixth = 1./6.;

    cellVolume (I1,I2,I3) =
      sixth*
      ((y(I1+1,I2  ,I3  ) - y(I1,I2,I3))*(z(I1  ,I2  ,I3+1) - z(I1+1,I2  ,I3  ))
      -(z(I1+1,I2  ,I3  ) - z(I1,I2,I3))*(y(I1  ,I2  ,I3+1) - y(I1+1,I2  ,I3  ))
      +(y(I1  ,I2+1,I3+1) - y(I1,I2,I3))*(z(I1  ,I2  ,I3+1) - z(I1  ,I2+1,I3  ))
      -(z(I1  ,I2+1,I3+1) - z(I1,I2,I3))*(y(I1  ,I2  ,I3+1) - y(I1  ,I2+1,I3  ))
      +(y(I1+1,I2+1,I3  ) - y(I1,I2,I3))*(z(I1  ,I2+1,I3  ) - z(I1+1,I2  ,I3  ))
      -(z(I1+1,I2+1,I3  ) - z(I1,I2,I3))*(y(I1  ,I2+1,I3  ) - y(I1+1,I2  ,I3  )))*(x(I1+1,I2+1,I3+1)-x(I1,I2,I3))
      +
      ((z(I1+1,I2  ,I3  ) - z(I1,I2,I3))*(x(I1  ,I2  ,I3+1) - x(I1+1,I2  ,I3  ))
      -(x(I1+1,I2  ,I3  ) - x(I1,I2,I3))*(z(I1  ,I2  ,I3+1) - z(I1+1,I2  ,I3  ))
      +(z(I1  ,I2+1,I3+1) - z(I1,I2,I3))*(x(I1  ,I2  ,I3+1) - x(I1  ,I2+1,I3  ))
      -(x(I1  ,I2+1,I3+1) - x(I1,I2,I3))*(z(I1  ,I2  ,I3+1) - z(I1  ,I2+1,I3  ))
      +(z(I1+1,I2+1,I3  ) - z(I1,I2,I3))*(x(I1  ,I2+1,I3  ) - x(I1+1,I2  ,I3  ))
      -(x(I1+1,I2+1,I3  ) - x(I1,I2,I3))*(z(I1  ,I2+1,I3  ) - z(I1+1,I2  ,I3  )))*(y(I1+1,I2+1,I3+1)-y(I1,I2,I3))
      +
      ((x(I1+1,I2  ,I3  ) - x(I1,I2,I3))*(y(I1  ,I2  ,I3+1) - y(I1+1,I2  ,I3  ))
      -(y(I1+1,I2  ,I3  ) - y(I1,I2,I3))*(x(I1  ,I2  ,I3+1) - x(I1+1,I2  ,I3  ))
      +(x(I1  ,I2+1,I3+1) - x(I1,I2,I3))*(y(I1  ,I2  ,I3+1) - y(I1  ,I2+1,I3  ))
      -(y(I1  ,I2+1,I3+1) - y(I1,I2,I3))*(x(I1  ,I2  ,I3+1) - x(I1  ,I2+1,I3  ))
      +(x(I1+1,I2+1,I3  ) - x(I1,I2,I3))*(y(I1  ,I2+1,I3  ) - y(I1+1,I2  ,I3  ))
      -(y(I1+1,I2+1,I3  ) - y(I1,I2,I3))*(x(I1  ,I2+1,I3  ) - x(I1+1,I2  ,I3  )))*(z(I1+1,I2+1,I3+1)-z(I1,I2,I3))
      ;
  }

  cellVolume.periodicUpdate();
  if (debug) createCellVolumeDisplay.display (cellVolume, "DEBUG: cellVolume");

  cellVolumeDefined = TRUE;
}

// =================================================================================
void MappedGridFiniteVolumeOperators::
createCenterNormal (MappedGrid &mg)
	//========================================
	// Author:		D.L.Brown
	// Date Created:	950504
	// Date Modified:	950504
	//
	// Purpose:
	//	compute centerNormal array
	//	It is computed by averaging the faceNormal array to 
	//	cell centers.
	//
	// Interface: (inputs)
	//	MappedGrid &mg	grid to use
	//
	// Interface: (output)
	//	On return, the centerNormal array is stored in the MappedGridFiniteVolumeOperators class
	//
	// Status and Warnings:
	//  Side effect: since centerNormal is computed from faceNormal by 
	//    averaging, if the faceNormal array doesn't exist before this
	//    routine is called, it will exist afterwards. Perhaps this
	//    should not be so.
	//
	//  There are no known bugs, nor will there ever be.
	// 
	//========================================
{
  if (!faceNormalDefined) createFaceNormal (mg); // perhaps some logic should be included to delete this at the end

  if (debug) createCenterNormalDisplay.interactivelySetInteractiveDisplay ("createCenterNormalDisplay initialization");


    centerNormal.updateToMatchGrid (mg, all, all, all, numberOfDimensions, numberOfDimensions);

    int axis;
    ForAllAxes(axis) centerNormal.setIsCellCentered (FALSE, axis);
    ForAxes(axis)    centerNormal.setIsCellCentered (TRUE,  axis);
    centerNormal = 0.;

    int extra = max (mg.numberOfGhostPoints());
    Index I1, I2, I3;
    getIndex(centerNormal, I1, I2, I3, extra);

    int component, face, direction;
    ForAxes(component)
    {
      ForAxes (face) 
      {
      direction = face;
      centerNormal(I1,I2,I3,face,component) = average(faceNormal, direction, face, component, 0, 0, 0, I1, I2, I3)(I1,I2,I3);
      }
    }

  centerNormalDefined = TRUE;

  if (debug) createCenterNormalDisplay.display (centerNormal, "createCenterNormal: centerNormal");

}
// =================================================================================
void MappedGridFiniteVolumeOperators::
createCenterNormalCG (MappedGrid &mg)
	//========================================
	// Author:		D.L.Brown
	// Date Created:	950504
	// Date Modified:	950504
	//
	// Purpose:
	//	compute centerNormalCG array (CMPGRD storage order of centerNormal array)
	//	It is computed by averaging the faceNormalCG array to 
	//	cell centers.
	//
	// Interface: (inputs)
	//	MappedGrid &mg	grid to use
	//
	// Interface: (output)
	//	On return, the centerNormal array is stored in the MappedGridFiniteVolumeOperators class
	//
	// Status and Warnings:
	//  Side effect: since centerNormal is computed from faceNormalCG by 
	//    averaging, if the faceNormalCG array doesn't exist before this
	//    routine is called, it will exist afterwards. Perhaps this
	//    should not be so.
	//
	//  There are no known bugs, nor will there ever be.
	// 
	//========================================
{
  if (!faceNormalCGDefined) createFaceNormalCG (mg); // perhaps some logic should be included to delete this at the end

  if (debug) createCenterNormalDisplay.interactivelySetInteractiveDisplay ("createCenterNormalDisplay initialization");


    centerNormalCG.updateToMatchGrid (mg, all, all, all, numberOfDimensions, numberOfDimensions);

    int axis;
    ForAllAxes(axis) centerNormalCG.setIsCellCentered (FALSE, axis);
    ForAxes(axis)    centerNormalCG.setIsCellCentered (TRUE,  axis);
    centerNormalCG = 0.;

    int extra = max (mg.numberOfGhostPoints());
    Index I1, I2, I3;
    getIndex(centerNormalCG, I1, I2, I3, extra);

//    int component, face, direction;
    int component, face;
    /*
    ForAxes(component)
    {
      ForAxes (face) 
      {
      direction = face;
      centerNormalCG(I1,I2,I3,component,face) = average(faceNormalCG, direction, component, face,  0, 0, 0, I1, I2, I3)(I1,I2,I3);
      }
    }
    */

    int i1,i2,i3;
    REAL HALF = 0.5;
    ForAxes(component){
      ForAxes (face) {
	  i1 = inc(face,rAxis); i2 = inc(face,sAxis); i3 = inc(face,tAxis);
	    centerNormalCG(I1,I2,I3,component,face) = HALF*(
		faceNormalCG(I1+i1,I2+i2,I3+i3,component,face) + faceNormalCG(I1,I2,I3,component,face));
      }
    }

  centerNormalCGDefined = TRUE;

  if (debug) createCenterNormalDisplay.display (centerNormalCG, "createCenterNormal: centerNormalCG");

}
// =================================================================================

// =================================================================================
void MappedGridFiniteVolumeOperators::
createFaceArea (MappedGrid &mg)
	//========================================
	// Author:		D.L.Brown
	// Date Created:	950504
	// Date Modified:	950504
	//
	// Purpose:
	//	compute faceArea array
	//	This is done by finding the magnitude of the 
	//	faceNormal vectors on each face
	//
	// Interface: (inputs)
	//	MappedGrid &mg	grid to use
	//
	// Interface: (output)
	//	On return, the faceArea array is stored in the MappedGridFiniteVolumeOperators class
	//
	// Status and Warnings:
	//	Side Effect: creates the faceNormal array if its not already there.
	//
	//  There are no known bugs, nor will there ever be.
	// 
	//========================================
{

  if (debug) createFaceAreaDisplay.interactivelySetInteractiveDisplay ("createFaceAreaDisplay initialization");

  if (!faceNormalDefined) createFaceNormal (mg);

  faceArea.updateToMatchGrid (mg, all, all, all, faceRange);
  faceArea.setFaceCentering (GridFunctionParameters::all);
  faceArea = 0.;

  int face;
  ForAxes (face)
  {
    int extra = max (mg.numberOfGhostPoints());
    Index I1, I2, I3;
    getIndex(faceArea, face, I1, I2, I3, extra);
    
    int component;
    ForAxes (component)
    {
      faceArea(I1,I2,I3,face) += faceNormal(I1,I2,I3,face,component)*faceNormal(I1,I2,I3,face,component);
    }
    faceArea(I1,I2,I3,face) = sqrt(faceArea(I1,I2,I3,face));
  }

  if (debug) createFaceAreaDisplay.display(faceArea, "createFaceArea: faceArea");

  faceAreaDefined = TRUE;

}
// =================================================================================
// =================================================================================
void MappedGridFiniteVolumeOperators::
createVertexJacobian (MappedGrid& mg)
// =================================================================================
{

  if (debug) createVertexJacobianDisplay.interactivelySetInteractiveDisplay ("createVertexJacobianDisplay initialization");
  //REALMappedGridFunction & xyrs = mg.vertexDerivative();

  REALMappedGridFunction xyrs;
  xyrs.reference (mg.vertexDerivative());


  vertexJacobian.updateToMatchGrid (mg, all, all, all);
  int axis;
  ForAllAxes(axis) vertexJacobian.setIsCellCentered (FALSE, axis);

  Index I1, I2, I3;
  getIndex(mg.dimension(), I1, I2, I3);


//This macro for readability of the formulas below
#undef XYRS
#define XYRS(l,m) xyrs(I1,I2,I3,ndnd(l,m))

  if (numberOfDimensions == 2)
  {
    vertexJacobian(I1,I2,I3) = XYRS(xAxis,rAxis)*XYRS(yAxis,sAxis)
			     - XYRS(xAxis,sAxis)*XYRS(yAxis,rAxis);
  }

  if (numberOfDimensions == 3)
  {

    vertexJacobian(I1,I2,I3) = 
      XYRS(xAxis,rAxis)*(XYRS(yAxis,sAxis)*XYRS(zAxis,tAxis) - XYRS(yAxis,tAxis)*XYRS(zAxis,sAxis))
    + XYRS(xAxis,sAxis)*(XYRS(yAxis,tAxis)*XYRS(zAxis,rAxis) - XYRS(yAxis,rAxis)*XYRS(zAxis,tAxis))
    + XYRS(xAxis,tAxis)*(XYRS(yAxis,rAxis)*XYRS(zAxis,sAxis) - XYRS(yAxis,sAxis)*XYRS(zAxis,rAxis));

  }

  if (debug) createVertexJacobianDisplay.display (vertexJacobian, "DEBUG: vertexJacobian:");
}

