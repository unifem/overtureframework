#include "Interpolate.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "PlotStuff.h"
#include "LineMapping.h"
#include "SquareMapping.h"
#include "BoxMapping.h"
#include "display.h"

// #include "testUtils.h"
// #include "TestParameters.h"

#define interpFineFromCoarse EXTERN_C_NAME(interpfinefromcoarse)
#define interpFineFromCoarseWithMask EXTERN_C_NAME(interpfinefromcoarsewithmask)

extern "C"
{
 void interpFineFromCoarse(
       const int &ndfra,const int &ndfrb,const int &ndfsa,const int &ndfsb,const int &ndfta,const int &ndftb,
       real & uf,
       const int &ndcra,const int &ndcrb,const int &ndcsa,const int &ndcsb,const int &ndcta,const int &ndctb,
       const real & uc,
       const int &nd,const int &nra,const int &nrb,const int &nsa,const int &nsb,const int &nta,const int &ntb, 
       const int &width,const int &ratios, const int &ndca,const int &ca,const int &cb,
       const int &ishift, const int &centerings, const int & update );

 void interpFineFromCoarseWithMask(
       const int &ndfra,const int &ndfrb,const int &ndfsa,const int &ndfsb,const int &ndfta,const int &ndftb,
       real & uf,
       const int &ndcra,const int &ndcrb,const int &ndcsa,const int &ndcsb,const int &ndcta,const int &ndctb,
       const real & uc,
       const int &nd,const int &nra,const int &nrb,const int &nsa,const int &nsb,const int &nta,const int &ntb, 
       const int &width,const int &ratios, const int &ndca,const int &ca,const int &cb,
       const int &ishift, const int &centerings, const int & update, const int &mask );
}


main ()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  cout << "========================================" << endl;
  cout << endl;
  cout << "  Interpolate test routine              " << endl;
  cout << endl;
  cout << "========================================" << endl;

  real executionTime;
  
  //...set up graphics
//   bool plotStuffInitialize = LogicalFalse;
//   PlotStuff ps(plotStuffInitialize, "testInterpolate");       // create a PlotStuff object
//   PlotStuffParameters psp;         // This object is used to change plotting parameters
  char buffer[80];

  int debug=0;


  Index Iv[3], &I1 = Iv[0], &I2 = Iv[1], &I3 = Iv[2];


  int ndStart=1, ndEnd=3;        // 1,3
  int ratioStart=2, ratioEnd=8;  // 2,4
  int widthStart=1, widthEnd=7;  // 1,7


  real maxErr=0.;

  for( int coarseFromFine=0; coarseFromFine<=1; coarseFromFine++ )
  {
  for( int numberOfDimensions=ndStart; numberOfDimensions<=ndEnd; numberOfDimensions++ )
  {

  for( int cc=0; cc<=1; cc++ ) // test vertex and cell centered
  {
    aString centeringName = cc==0 ? "vertex" : "cell  ";

    if( coarseFromFine==0 )
    {
      if( cc==1) continue;  // skip cell centred for now.
      
      widthStart=2;  widthEnd=2;  // for coarseFromFine we just use a linear polynomial to test
    }
    else
    {
       widthStart=1, widthEnd=7; 
    }
    
  for( int iw=widthStart; iw<=widthEnd; iw++ )  // test different interpolation widths
  {
    int width = iw; // iw==0 ? 2 : iw==1 ? 3 : 5;
    
    int degreeSpace = width-1; // cg.numberOfDimensions();
    int degreeTime = 0;
    int numberOfComponents=1;
    OGPolyFunction polyTrue(degreeSpace,numberOfDimensions,numberOfComponents,degreeTime);
  
    real fx=1., fy=1., fz=1., ft=0.;
    OGTrigFunction trigTrue(fx, fy, fz, ft);        //  defines cos(pi*x)*cos(pi*y)*cos(pi*z)*cos(pi*t)

    OGFunction & exact = polyTrue;
//    OGFunction & exact = trigTrue;

    for( int ir=ratioStart; ir<=ratioEnd; ir++ )  // test ratio=2,3, and 4
    {

      int ratio=ir; // ir==0 ? 2 : 4;

      LineMapping coarseLine;
      LineMapping fineLine;

      real xmin=0., xmax=1., ymin=0., ymax=1.;
      SquareMapping coarseSquare (xmin, xmax, ymin, ymax);
      SquareMapping fineSquare   (xmin, xmax, ymin, ymax);

      BoxMapping coarseBox;
      BoxMapping fineBox;

      Mapping *cm, *fm;
      if( numberOfDimensions==1 )
      {
	cm=&coarseLine; fm=&fineLine;
      }
      else if(  numberOfDimensions==2 )
      {
	cm=&coarseSquare; fm=&fineSquare;
      }
      else
      {
	cm=&coarseBox; fm=&fineBox;
      }
      Mapping & coarseMapping = *cm;
      Mapping & fineMapping = *fm;
      

      int nx[3]={21,21,21}; // {41,41,41}; //{5,5,5}; // coarse grid
      if( numberOfDimensions==3 )
      {
	nx[0]=6; nx[1]=6; nx[2]=6;
      }
    
      int axis;
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	coarseMapping.setGridDimensions(axis, nx[axis]+1);
	fineMapping.setGridDimensions(axis, nx[axis]*ratio+1);

      }

      MappedGrid cg(coarseMapping);
      MappedGrid fg(fineMapping);   
      const int numberOfGhostPoints=4; // for width<=8
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
        if( cc==1 )
	{
          cg.setIsCellCentered(axis,true);
          fg.setIsCellCentered(axis,true);
	}
	for( int side=0; side<=1; side++ )
	{
	  fg.setNumberOfGhostPoints(side,axis,numberOfGhostPoints);
	  cg.setNumberOfGhostPoints(side,axis,numberOfGhostPoints);
	}
      }
      cg.update(MappedGrid::THEvertex |  MappedGrid::THEcenter);
      fg.update(MappedGrid::THEvertex |  MappedGrid::THEcenter | MappedGrid::THEmask);

      const intArray & mask = fg.mask();

      realMappedGridFunction uFine(fg);
      uFine.setName("uFine");
      realMappedGridFunction uCoarse(cg);


      int n=0;
      getIndex(cg.dimension(),I1,I2,I3);
      uCoarse=exact(cg,I1,I2,I3,n,0.);
  
      getIndex(fg.dimension(),I1,I2,I3);
      uFine=exact(fg,I1,I2,I3,n,0.);

      realArray uFineOld(I1,I2,I3,numberOfComponents);

      IntegerArray amrRefinementRatio(3);
      amrRefinementRatio=ratio;
    
      InterpolateParameters interpParams (numberOfDimensions, debug);

      interpParams.setAmrRefinementRatio(ratio); //              (testParams.amrRefinementRatio);
      interpParams.setInterpolateOrder(width); //                (testParams.interpolateOrder);
      if( false && cc==1 ) // this is not supported
        interpParams.setGridCentering(GridFunctionParameters::cellCentered);

      bool timing = false; // LogicalTrue;
      Interpolate interpolate;
      interpolate.initialize (interpParams, timing);

      if( coarseFromFine==0  )
      {
        //   ****** test coarse from fine ******

        aString testName[]={ "injection       ",
			     "fullWeighting100",
			     "fullWeighting010",
			     "fullWeighting001",
			     "fullWeighting110",
			     "fullWeighting101",
			     "fullWeighting011",
			     "fullWeighting111" };
	
	

        Interpolate::InterpolateOptionEnum interpOption;
	
	int numberOfTests=numberOfDimensions==1 ? 2 : numberOfDimensions==2? 4 : 5;
	for( int it=0; it<numberOfTests; it++ )
	{
	  getIndex(cg.gridIndexRange(),I1,I2,I3);
	  if( numberOfDimensions==1 )
	  {
	    if( it==0 )
	      interpOption=Interpolate::fullWeighting100;
            else
              interpOption=Interpolate::injection;
	  }
	  else if( numberOfDimensions==2 )
	  {
	    if( it==0 )
	    {
	      interpOption=Interpolate::fullWeighting110;
	    }
	    else if( it==1 )
	    {
              interpOption=Interpolate::fullWeighting100;
	      I2=3;
	    }
	    else if( it==2 )
	    {
              interpOption=Interpolate::fullWeighting010;
	      I1=2;
	    }
            else if( it==3 )
	    {
              interpOption=Interpolate::injection;
	    }
	    
	  
	  }
	  else 
	  {
	    if( it==0 )
	    {
	      interpOption=Interpolate::fullWeighting111;
	    }
	    else if( it==1 )
	    {
              interpOption=Interpolate::fullWeighting110;
	      I3=3;
	    }
	    else if( it==2 )
	    {
              interpOption=Interpolate::fullWeighting101;
	      I2=2;
	    }
	    else if( it==3 )
	    {
              interpOption=Interpolate::fullWeighting011;
	      I1=2;
	    }
            else if( it==4 )
	    {
              interpOption=Interpolate::injection;
	    }

	  }
	
    
	  uCoarse(I1,I2,I3,n)=-999.;

          int update=0;
          real time=getCPU();
          interpolate.interpolateCoarseFromFine( uCoarse, Iv, uFine, amrRefinementRatio, interpOption, update);
          time=getCPU()-time;
	  realArray error;
         
	  if( !update )
	    error=uCoarse(I1,I2,I3,n)-exact(cg,I1,I2,I3,n,0.);
	  else
	    error=uCoarse(I1,I2,I3,n)-2.*exact(cg,I1,I2,I3,n,0.);
  
	  real err = max(fabs( error ));
	  maxErr=max(maxErr,err);

          printf("coarseFromFine %iD %s ratio=%i %s : max error =%8.1e cpu=%8.1e\n",
		     numberOfDimensions,(const char*)centeringName,ratio,
                     (const char*)testName[interpOption],err,time);

	}
	
      }
      else
      {
        //   ****** test fine from coarse ******
	int numberOfTests=numberOfDimensions==1 ? 4 : numberOfDimensions==2? 6 : 5;
	for( int it=0; it<numberOfTests; it++ )
	{
	  getIndex(fg.gridIndexRange(),I1,I2,I3);

	  if( numberOfDimensions==1 )
	  {
	    if( it==0 )
	      I1=3*ratio;
	    else if( it==1 )
	      I1=3*ratio+1;
	    else if( it==2 )
	      I1=3*ratio+2;
	    else if( it==3 )
	      I1=3*ratio+3;
	  }
	  else if( numberOfDimensions==2 )
	  {
	    if( it==0 )
	    {
	      I2=3*ratio;
	    }
	    else if( it==1 )
	      I1=4*ratio;
	    else if( it==2 )
	      I2=3*ratio+1;
	    else if( it==3 )
	      I2=3*ratio+2;
	    else if( it==4 )
	      I2=3*ratio+3;
	    else 
	    {
	      // do whole block
	    }
	  
	  }
	  else 
	  {
	    if( it==0 )
	    {
	      I2=3*ratio;
	    }
	    else if( it==1 )
	      I1=2*ratio;
	    else if( it==2 )
	      I2=3*ratio+1;
	    else if( it==3 )
	      I3=2*ratio+2;
	    else if( it==4 )
	      I3=2*ratio+3;
	  }
	
    
	  uFine(I1,I2,I3,n)=-999.;
  
  
	  if( debug & 2)
	    printf(" Interp points [%i,%i][%i,%i][%i,%i]\n",I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),
		   I3.getBase(),I3.getBound());
  
	  real time0=0., time=0.;
	  real err=0.;
	  bool computedOldWay=false;
	  if( cc==0 && (ratio == 2 || ratio==4 || ratio==8 || ratio==16) )
	  {
	    computedOldWay=true;
	    time0=getCPU();
	    interpolate.interpolateCoarseToFine (uFine, Iv, uCoarse, amrRefinementRatio);
	    time=getCPU()-time0;
	    err = max(fabs( uFine(I1,I2,I3,n)-exact(fg,I1,I2,I3,n,0.)));
	    maxErr=max(maxErr,err);

	    uFineOld(I1,I2,I3,n)=uFine(I1,I2,I3,n);
	  
	    printf("%iD %s width=%i ratio=%i test %i..............: max error      =%8.1e cpu=%8.1e\n",
		   numberOfDimensions,(const char*)centeringName,width,ratio,it,err,time);
	  }
	
  
	  const IntegerArray & df=fg.dimension();
	  const IntegerArray & dc=cg.dimension();
  
	  int ndca=0, ca=0,cb=0;


	  int shift[3]={0,0,0};  // set to 1 to prefer a "left" stencil
	  int useMask=0;
	  int centering=cc; // 1=cell centered

	  int ratios[3]={ratio,ratio,ratio}; //
	  int centerings[3]={centering,centering,centering}; //
	  int update=0;
	  for( int mm=0; mm<=3; mm++ )
	  {
	    update= mm % 2;  // *** if update==1 we compute uf=uf+...
	    if( !update )
	      uFine(I1,I2,I3,n)=-999.;

	    time0=getCPU();
	    if( mm<=1)
	    {
	      interpFineFromCoarse( df(0,0),df(1,0),df(0,1),df(1,1),df(0,2),df(1,2),
				    *uFine.getDataPointer(),
				    dc(0,0),dc(1,0),dc(0,1),dc(1,1),dc(0,2),dc(1,2),
				    *uCoarse.getDataPointer(),
				    numberOfDimensions,
				    I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),
				    I3.getBase(),I3.getBound(),
				    width,ratios[0],ndca,ca,cb, shift[0], centerings[0],update );
	  
	    }
	    else
	    {
	      interpFineFromCoarseWithMask( df(0,0),df(1,0),df(0,1),df(1,1),df(0,2),df(1,2),
					    *uFine.getDataPointer(),
					    dc(0,0),dc(1,0),dc(0,1),dc(1,1),dc(0,2),dc(1,2),
					    *uCoarse.getDataPointer(),
					    numberOfDimensions,
					    I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),
					    I3.getBase(),I3.getBound(),
					    width,ratios[0],ndca,ca,cb, shift[0], centerings[0], update, *mask.getDataPointer() );
	  
	    }
	  
	    real time2=getCPU()-time0;

	    realArray error;
         
	    if( !update )
	      error=uFine(I1,I2,I3,n)-exact(fg,I1,I2,I3,n,0.);
	    else
	      error=uFine(I1,I2,I3,n)-2.*exact(fg,I1,I2,I3,n,0.);
  
	    err = max(fabs( error ));
	    maxErr=max(maxErr,err);

	    if( debug & 2 )
	    {
	      display(uFine(I1,I2,I3,n),"uFine(I1,I2,I3,n)","%5.2f ");
	      display(error,"error","%8.1e ");
	    }
  
	    real diffFromOld=0.;
	    if( computedOldWay && !update )
	    {
	      diffFromOld=max(fabs(uFine(I1,I2,I3,n)-uFineOld(I1,I2,I3,n)));
	    }
	  
	    if( mm==0 )
	      printf("%iD %s width=%i ratio=%i test %i..............: max error (opt)=%8.1e cpu=%8.1e  ratio=%5.1f (diff=%8.1e)\n",
		     numberOfDimensions,(const char*)centeringName,width,ratio,it,err,time2,time/time2,diffFromOld);
	    else if( mm==1 )
	      printf("%iD %s width=%i ratio=%i test %i (update).....: max error (opt)=%8.1e cpu=%8.1e  ratio=%5.1f (diff=%8.1e)\n",
		     numberOfDimensions,(const char*)centeringName,width,ratio,it,err,time2,time/time2,diffFromOld);
	    else if( mm==2 )
	      printf("%iD %s width=%i ratio=%i test %i..............: max error (opt)=%8.1e cpu=%8.1e  ratio=%5.1f (diff=%8.1e)\n",
		     numberOfDimensions,(const char*)centeringName,width,ratio,it,err,time2,time/time2,diffFromOld);
	    else
	      printf("%iD %s width=%i ratio=%i test %i (update/mask): max error (opt)=%8.1e cpu=%8.1e  ratio=%5.1f (diff=%8.1e)\n",
		     numberOfDimensions,(const char*)centeringName,width,ratio,it,err,time2,time/time2,diffFromOld);
	  }
	
	  uFine(I1,I2,I3,n)=exact(fg,I1,I2,I3,n,0.);
	} // end for it
      } // else 
    } // end ir
  } // end iw
  
  } // end cc
  
  } // end numberOfDimensions
  } // end coarseFromFine
  
  printf(" *****maxErr = %8.1e ******\n",maxErr);

  if( fabs(maxErr) < REAL_EPSILON*100. )  // this test is only valid for a polynomial true solution.
  {
    printf(" **** Test apparently successful *****\n");
    return 0;
  }
  else
  {
    printf(" **** There is a large error somewhere *****\n");
    return 1;
  }
  

}
