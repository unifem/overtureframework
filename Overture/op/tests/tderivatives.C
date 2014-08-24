// This file automatically generated from tderivatives.bC with bpp.
#include "Overture.h"
#include "CompositeGridOperators.h"
#include "OGTrigFunction.h"  // Trigonometric function
#include "OGPolyFunction.h"  // polynomial function
#include "GridFunctionParameters.h"
#include "display.h"
#include "Checker.h"
#include "ParallelUtility.h"


// Macro: Compute the derivatives of the exact solution

void 
passByValue( realMappedGridFunction u, realCompositeGridFunction u0 )
{
    display(u,"passByValue: u:","%4.1f ");
    u0.display("passByValue u0:","%4.1f ");
    display(u.x(),"PassByValue: u.x()","%4.1f ");
    u0.x().display("PassByValue: u0.x()","%4.1f ");
    
}  


//================================================================================
//  Test out all the derivatives
//================================================================================
int 
main(int argc, char **argv)
{
  // Diagnostic_Manager::setSmartReleaseOfInternalMemory( On );

    Overture::start(argc,argv);  // initialize Overture

    aString checkFileName;
    if( REAL_EPSILON == DBL_EPSILON )
        checkFileName="tderivatives.dp.check.new";  // double precision
    else  
        checkFileName="tderivatives.sp.check.new";

    Checker checker(checkFileName);  // for saving a check file.

    const int maxNumberOfGridsToTest=3;
    int numberOfGridsToTest=maxNumberOfGridsToTest;
    aString gridName[maxNumberOfGridsToTest] =   { "square20", "cic", "sib" };
    aString buff;
        
    int tz=1;
    int degreeSpace = 6;   // For polynomial TZ
    int degreeTime = 1;

    bool useSinglePrecisionTolerance=false;
    if( argc > 1 )
    { 
        int len=0;
        for( int i=1; i<argc; i++ )
        {
            aString line;
            line=argv[i];
            if( line.matches("tz=trig") || line.matches("-tz=trig") )
      	tz=1;
            else if( line.matches("tz=poly") || line.matches("-tz=poly")  )
      	tz=0;
            else if( line.matches("-sp") )
      	useSinglePrecisionTolerance=true;
            else if( (len=line.matches("-degree=")) )
            {
                sScanF(line(len,line.length()-1),"%i",&degreeSpace);
      	printf("Setting degreeSpace=%i for polynomial TZ\n",degreeSpace);
            }
            else
            {
      	numberOfGridsToTest=1;
                gridName[0]=argv[i];
            }
        }
    }
    else
    {
        printF("Usage: `tderivatives [<gridName>] [-tz=trig][-tz=poly][-sp][-degree=<value>]' \n");
        printF("       -sp : use single precision tol even in double precision code\n");
        
    }
    
    int debug=0;
/* --
    int debug=7;
    cout << "Enter debug \n";
    cin >> debug;  
--- */

    // real cutOff = REAL_EPSILON == (DBL_EPSILON && !useSinglePrecisionTolerance ) ? 5.e-12 : 2.e-3;  // *wdh* 030112
    real cutOff = 2.e-3;  // *wdh* 2014/08/22

    printF(" **** setting cutOff tolerance = %8.2e\n",cutOff);
    
    checker.setCutOff(cutOff);

    real worstError=0;
    for( int it=0; it<numberOfGridsToTest; it++ )
    {
        aString nameOfOGFile=gridName[it];
        
        CompositeGrid cg;
        aString name=nameOfOGFile;    // make a copy since the full path name is returned now
        int found = getFromADataBase(cg,name)==0;
        if( !found ) return 1;
            
        const int numberOfDimensions=cg.numberOfDimensions();
        cg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEinverseVertexDerivative );

        printF("\n *****************************************************************\n"
                      " ******** Checking grid: %s ************ \n"
                      " *****************************************************************\n\n",(const char*)nameOfOGFile);
        
        checker.setLabel(nameOfOGFile,0);

        int grid;

        Index I1,I2,I3,N;
        Range all;
        realMappedGridFunction u,v;   // define some component grid functions

        MappedGridOperators op;                     // define some differential operators
        u.setOperators(op);                         // Tell u which operators to use

    //  OGTrigFunction trigTrue(1.,1.,1.);  // create an exact solution (Twilight-Zone solution)
        const int numberOfComponents=5;

        RealArray fx(numberOfComponents), fy(numberOfComponents), fz(numberOfComponents), ft(numberOfComponents);
        fx=1.;   fx(0)=.5; fx(1)=1.5;
        fy=.1;   fy(0)=.4; fy(1)= .5;
        if( cg.numberOfDimensions()==3 )
        {
            fz=1.;   fz(0)=.3; fz(1)=-.3;
        }
        else
        {
            fz=0;
        }
    
        ft=1.;   ft(0)=.6; ft(1)=.35;
    
        OGTrigFunction trigTrue(fx, fy, fz, ft);        //  defines cos(pi*x)*cos(pi*y)*cos(pi*z)*cos(pi*t)

        RealArray gx(numberOfComponents), gy(numberOfComponents), gz(numberOfComponents), gt(numberOfComponents);
        gx=.5;   gx(0)=.5; gx(1)=.25;
        gy=.1;   gy(0)=.4; gy(1)= .5;
        if( cg.numberOfDimensions()==3 )
        {
            gz=.25;   gz(0)=.3; gz(1)=-.3;
        }
        else
        {
            gz=0.;
        }
        gt=1.;   gt(0)=.6; gt(1)=.35;
        trigTrue.setShifts(gx,gy,gz,gt);
    
        RealArray amp(numberOfComponents);
        amp=.5;
        amp(0)=.25;
        trigTrue.setAmplitudes(amp);

        RealArray cc(numberOfComponents);
        cc=1.;
        cc(0)=-.5;
        trigTrue.setConstants(cc);


        OGPolyFunction polyTrue(degreeSpace,cg.numberOfDimensions(),numberOfComponents,degreeTime);

        RealArray spatialCoefficientsForTZ(7,7,7,numberOfComponents);  
        spatialCoefficientsForTZ=0.;
        RealArray timeCoefficientsForTZ(7,numberOfComponents);      
        timeCoefficientsForTZ=0.;

        for( int m1=0; m1<=degreeSpace; m1++ )
            for( int m2=0; m2<=degreeSpace; m2++ )
      	for( int m3=0; m3<=degreeSpace; m3++ )
        	  for( int n=0; n<numberOfComponents; n++ )
        	  {
          	    if( (m1+m2+m3) <= degreeSpace )
            	      spatialCoefficientsForTZ(m1,m2,m3,n)= 1./( m1*m1 + 2.*m2*m2 + 3.*m3*m3 + n+1.);
        	  }

        for( int n=0; n<numberOfComponents; n++ )
        {
            for( int i=0; i<=4; i++ )
      	timeCoefficientsForTZ(i,n)= i<=degreeTime ? 1./(i+1) : 0. ;
        }
        polyTrue.setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ ); 


        OGFunction & exact = tz==0 ? (OGFunction&)polyTrue : (OGFunction&)trigTrue;

        GridFunctionParameters gfp;
        realCompositeGridFunction uu(cg,gfp);
        uu.updateToMatchGrid(cg,gfp);
        uu.updateToMatchGrid(cg,gfp.outputType);
    

        real error,time;
        int n=0;      // only test first component

        for(grid=0; grid<cg.numberOfGrids(); grid++ )
        {
            MappedGrid & mg = cg[grid];
            checker.setLabel(mg.getName(),1);

            u.updateToMatchGrid(mg,all,all,all,Range(0,0));
            v.updateToMatchGrid(mg,all,all,all,Range(0,0));
        
            op.updateToMatchGrid(mg);
        
            const realArray & center = mg.center();
            const intArray & mask = mg.mask();
            OV_GET_SERIAL_ARRAY_CONST(int,mask,maskLocal);
            OV_GET_SERIAL_ARRAY_CONST(real,center,xLocal);
            OV_GET_SERIAL_ARRAY(real,u,uLocal);
            OV_GET_SERIAL_ARRAY(real,v,vLocal);
            


            getIndex(mg.dimension(),I1,I2,I3);                                             // assign I1,I2,I3

            bool ok=ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,1);


            int ntd=0,nxd=0,nyd=0,nzd=0;
            int isRectangular=0;
            exact.gd(uLocal,xLocal,numberOfDimensions,isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,n,0.); 

            realMappedGridFunction ed(mg),edd(mg),sx(mg);  
            OV_GET_SERIAL_ARRAY(real,ed,edLocal);
            OV_GET_SERIAL_ARRAY(real,edd,eddLocal);
            OV_GET_SERIAL_ARRAY(real,sx,sxLocal);

            realMappedGridFunction w(mg,all,all,all,mg.numberOfDimensions());  // for divergence and vorticity
            w.setOperators(op);
            OV_GET_SERIAL_ARRAY(real,w,wLocal);

            for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
            {
                exact.gd(wLocal,xLocal,numberOfDimensions,isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,axis,0.); 
            }
            
            realMappedGridFunction scalar(mg,all,all,all);  // for divScalarGrad
            scalar.setOperators(op);
            OV_GET_SERIAL_ARRAY(real,scalar,scalarLocal);
            
      // for harmonic averaging we do not want a negative scalar
      // **      scalar(I1,I2,I3)=1.+exact(mg,I1,I2,I3,1,0.);
      // realArray xy; xy.partition(center.getPartition());
      // realMappedGridFunction xy(mg);
            realSerialArray xy;
            real xyMax=0.;
            if( ok )
            {
      	xy.redim(I1,I2,I3);
                xy = 1.+xLocal(I1,I2,I3,0)+2.*xLocal(I1,I2,I3,1)+.25*xLocal(I1,I2,I3,0)*xLocal(I1,I2,I3,1);

                xyMax=max(fabs(xy));
            }
            xyMax=ParallelUtility::getMaxValue(xyMax);
            
            scalarLocal(I1,I2,I3)=1.+xy*(.5/xyMax);

      // scalar=2.; // *********************************

            Range D=mg.numberOfDimensions();
            realMappedGridFunction tensor(mg,all,all,all,2,2);  // for testing derivatives of tensors
            tensor.setOperators(op);
            OV_GET_SERIAL_ARRAY(real,tensor,tensorLocal);
            tensorLocal.reshape(tensorLocal.dimension(0),tensorLocal.dimension(1),tensorLocal.dimension(2),2,2);
            tensorLocal=0.;

            if( ok )
            {
      	exact.gd(edLocal,xLocal,numberOfDimensions,isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,0.);
      	tensorLocal(I1,I2,I3,0,0)=edLocal(I1,I2,I3); 
      	exact.gd(edLocal,xLocal,numberOfDimensions,isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,1,0.);
      	tensorLocal(I1,I2,I3,1,0)=edLocal(I1,I2,I3); 
      	exact.gd(edLocal,xLocal,numberOfDimensions,isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,2,0.);
      	tensorLocal(I1,I2,I3,0,1)=edLocal(I1,I2,I3); 
      	exact.gd(edLocal,xLocal,numberOfDimensions,isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,3,0.);
      	tensorLocal(I1,I2,I3,1,1)=edLocal(I1,I2,I3); 
            }
            
      // here is the tensor coefficient for divTensorGrad:
            const int ndSq=SQR(mg.numberOfDimensions());
            realMappedGridFunction cTensor(mg,all,all,all,ndSq);
            cTensor.setOperators(op);
            OV_GET_SERIAL_ARRAY(real,cTensor,cTensorLocal);
            
            for( int m1=0; m1<mg.numberOfDimensions(); m1++ )
            for( int m2=0; m2<mg.numberOfDimensions(); m2++ )  
            {
                int nn=m1+m2*(mg.numberOfDimensions());
                if( ok )
      	{
	  // make tensor coefficients variable in space
                    int mm = m1+m2;   // make symmetric
                    mm = mm % numberOfComponents;
          // cTensor(I1,I2,I3,nn)=exact(mg,I1,I2,I3,mm,0.);
                    exact.gd(edLocal,xLocal,numberOfDimensions,isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,mm,0.);
                    cTensorLocal(I1,I2,I3,nn)=edLocal(I1,I2,I3); 

      	}
      	
            }
            

            Range R1,R2,R3;
            R1=Range(I1.getBase(),I1.getBound());
            R2=Range(I2.getBase(),I2.getBound());
            R3=Range(I3.getBase(),I3.getBound());


      // Compute the derivatives of the exact solution (macro)
      // computeDerivativesOfExactSolution(mg,2);
              realMappedGridFunction ex(mg),ey(mg),ez(mg); // to hold derivatives of the exact solution
              OV_GET_SERIAL_ARRAY(real,ex,exLocal);
              OV_GET_SERIAL_ARRAY(real,ey,eyLocal);
              OV_GET_SERIAL_ARRAY(real,ez,ezLocal);
              if( ok )
              {
                  exact.gd(exLocal,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,n);
                  exact.gd(eyLocal,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,n);
                  if( mg.numberOfDimensions()==3 )
                      exact.gd(ezLocal,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,n);
              }
       //        #If "2" == "2"
                realMappedGridFunction exx(mg),exy(mg),exz(mg),eyy(mg),eyz(mg),ezz(mg),eLap(mg);
                OV_GET_SERIAL_ARRAY(real,exx,exxLocal);
                OV_GET_SERIAL_ARRAY(real,exy,exyLocal);
                OV_GET_SERIAL_ARRAY(real,exz,exzLocal);
                OV_GET_SERIAL_ARRAY(real,eyy,eyyLocal);
                OV_GET_SERIAL_ARRAY(real,eyz,eyzLocal);
                OV_GET_SERIAL_ARRAY(real,ezz,ezzLocal);
                OV_GET_SERIAL_ARRAY(real,eLap,eLapLocal);
                if( ok )
                {
                    exact.gd(exxLocal,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,n);
                    exact.gd(exyLocal,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,n);
                    exact.gd(eyyLocal,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,n);
                    if( mg.numberOfDimensions()==2 )
                    {
                        eLapLocal=exxLocal+eyyLocal;
                    }
                    else
                    {
                        exact.gd(exzLocal,xLocal,numberOfDimensions,isRectangular,0,1,0,1,I1,I2,I3,n);
                        exact.gd(eyzLocal,xLocal,numberOfDimensions,isRectangular,0,0,1,1,I1,I2,I3,n);
                        exact.gd(ezzLocal,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,n);
                        eLapLocal=exxLocal+eyyLocal+ezzLocal;
                    }
                }

      // ---- compute all derivatives for orders of accuracy 2,4,6,8 ----
            const int maxOrder=8;
            for( int order=2; order<=maxOrder; order+=2 )
      //  for( int order=4; order<=8; order+=2 )
            {
                checker.setLabel(sPrintF(buff,"order=%i",order),2);
                checker.setLabel("std",3);

                real gridError=0.;
      	u.operators->setOrderOfAccuracy(order);

      	getIndex(mg.dimension(),I1,I2,I3,-(order/2));  // reduce size for 2nd or 4th order

                bool ok=ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,1);

      	v=u.x()+u.yy();
      	GridFunctionParameters gfp;
      	v=u.x(gfp);
      	gfp=GridFunctionParameters::cellCentered;
      	v=u.y(gfp);
      	GridFunctionParameters cellCentered   = GridFunctionParameters::cellCentered,
        	  vertexCentered = GridFunctionParameters::vertexCentered;
      	v=u.xx(cellCentered);


        // testDerivatives(x,xDerivative);   // this is a macro
                {
        // --- compute the derivative here (non-optimized) ----
                time=getCPU();
                v=u.x(I1,I2,I3);
                time=getCPU()-time;
        //         #If "x" eq "x"
          // exact.gd(ed,0,1,0,0,I1,I2,I3,n,0.);
                  ed=ex;
        // error = max(fabs(v(I1,I2,I3)-ed(I1,I2,I3)))/max(1.+fabs(ed(I1,I2,I3))); 
                real edMax = max(fabs(edLocal(I1,I2,I3)));
                edMax = ParallelUtility::getMaxValue(edMax);
                real errMax = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));
                errMax = ParallelUtility::getMaxValue(errMax);
                error = errMax/(1.+edMax);
                gridError=max(gridError,error);
        // cout << "u.xx       : Maximum relative error (order=" << order << ") = " << error << endl;
                checker.printMessage("u.x",error,time);
        // --- compute the deriavtive (optimized) ----
                v=-123456.;
                time=getCPU();
                op.derivative(MappedGridOperators::xDerivative,u,v,I1,I2,I3);
                time=getCPU()-time;
        // error = max(fabs(v(I1,I2,I3)-ed(I1,I2,I3)))/max(1.+fabs(ed(I1,I2,I3)));
                edMax = max(fabs(edLocal(I1,I2,I3)));
                edMax = ParallelUtility::getMaxValue(edMax);
                errMax = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));
                errMax = ParallelUtility::getMaxValue(errMax);
                error = errMax/(1.+edMax);
                gridError=max(gridError,error);
                  checker.printMessage("u.x (opt)",error,time);
                }

      	if( debug & 4 )
      	{
        	  display(fabs(u.x()(I1,I2,I3)-ex(I1,I2,I3,n)),"Error in u.x","%4.1f ");
        	  display(ex(I1,I2,I3,n)," exact u.x","%4.1f ");
        	  display(u.x()(I1,I2,I3),"computed u.x","%4.1f ");

        	  display(exact(mg,I1,I2,I3,n)," exact u","%4.1f ");
        	  display(u(I1,I2,I3),"discrete u","%4.1f ");
      	}
            
        // testDerivatives(y,yDerivative);   // this is a macro
                {
        // --- compute the derivative here (non-optimized) ----
                time=getCPU();
                v=u.y(I1,I2,I3);
                time=getCPU()-time;
        //         #If "y" eq "x"
        //         #Elif "y" eq "y"
         //  exact.gd(ed,0,0,1,0,I1,I2,I3,n,0.);
                  ed=ey;
        // error = max(fabs(v(I1,I2,I3)-ed(I1,I2,I3)))/max(1.+fabs(ed(I1,I2,I3))); 
                real edMax = max(fabs(edLocal(I1,I2,I3)));
                edMax = ParallelUtility::getMaxValue(edMax);
                real errMax = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));
                errMax = ParallelUtility::getMaxValue(errMax);
                error = errMax/(1.+edMax);
                gridError=max(gridError,error);
        // cout << "u.xx       : Maximum relative error (order=" << order << ") = " << error << endl;
                checker.printMessage("u.y",error,time);
        // --- compute the deriavtive (optimized) ----
                v=-123456.;
                time=getCPU();
                op.derivative(MappedGridOperators::yDerivative,u,v,I1,I2,I3);
                time=getCPU()-time;
        // error = max(fabs(v(I1,I2,I3)-ed(I1,I2,I3)))/max(1.+fabs(ed(I1,I2,I3)));
                edMax = max(fabs(edLocal(I1,I2,I3)));
                edMax = ParallelUtility::getMaxValue(edMax);
                errMax = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));
                errMax = ParallelUtility::getMaxValue(errMax);
                error = errMax/(1.+edMax);
                gridError=max(gridError,error);
                  checker.printMessage("u.y (opt)",error,time);
                }

        // testDerivatives(xx,xxDerivative);   // this is a macro
                {
        // --- compute the derivative here (non-optimized) ----
                time=getCPU();
                v=u.xx(I1,I2,I3);
                time=getCPU()-time;
        //         #If "xx" eq "x"
        //         #Elif "xx" eq "y"
        //         #Elif "xx" eq "z"
        //         #Elif "xx" eq "xx"
         //  exact.gd(ed,0,2,0,0,I1,I2,I3,n,0.);
                  ed=exx;
        // error = max(fabs(v(I1,I2,I3)-ed(I1,I2,I3)))/max(1.+fabs(ed(I1,I2,I3))); 
                real edMax = max(fabs(edLocal(I1,I2,I3)));
                edMax = ParallelUtility::getMaxValue(edMax);
                real errMax = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));
                errMax = ParallelUtility::getMaxValue(errMax);
                error = errMax/(1.+edMax);
                gridError=max(gridError,error);
        // cout << "u.xx       : Maximum relative error (order=" << order << ") = " << error << endl;
                checker.printMessage("u.xx",error,time);
        // --- compute the deriavtive (optimized) ----
                v=-123456.;
                time=getCPU();
                op.derivative(MappedGridOperators::xxDerivative,u,v,I1,I2,I3);
                time=getCPU()-time;
        // error = max(fabs(v(I1,I2,I3)-ed(I1,I2,I3)))/max(1.+fabs(ed(I1,I2,I3)));
                edMax = max(fabs(edLocal(I1,I2,I3)));
                edMax = ParallelUtility::getMaxValue(edMax);
                errMax = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));
                errMax = ParallelUtility::getMaxValue(errMax);
                error = errMax/(1.+edMax);
                gridError=max(gridError,error);
                  checker.printMessage("u.xx (opt)",error,time);
                }

                op.setNumberOfDerivativesToEvaluate(1);
      	op.setDerivativeType(0,MappedGridOperators::xxDerivative,v);
                v=-123456.;

                time=getCPU();
      	op.getDerivatives(u,I1,I2,I3);
      	time=getCPU()-time;
      	
	// error = max(fabs(v(I1,I2,I3)-exx(I1,I2,I3)))/max(1.+fabs(exx(I1,I2,I3)));
                real exxMax = max(fabs(exxLocal(I1,I2,I3)));
                exxMax = ParallelUtility::getMaxValue(exxMax);

                real errMax = max(fabs(vLocal(I1,I2,I3)-exxLocal(I1,I2,I3)));
                errMax = ParallelUtility::getMaxValue(errMax);

                error = errMax/(1.+exxMax);


                gridError=max(gridError,error);
                checker.printMessage("u.xx (get)",error,time);

	// if( (grid==1 && order==2 ) || debug & 4 )
      	if( debug & 4 )
      	{
        	  fabs(v(I1,I2,I3)-exx(I1,I2,I3,n)).display("Error in u.xx (opt)");
        	  v(I1,I2,I3).display("u.xx");
        	  display(exx(I1,I2,I3,n),"exact.xx","%4.1f ");
      	}

        // testDerivatives(xy,xyDerivative);   // this is a macro
                {
        // --- compute the derivative here (non-optimized) ----
                time=getCPU();
                v=u.xy(I1,I2,I3);
                time=getCPU()-time;
        //         #If "xy" eq "x"
        //         #Elif "xy" eq "y"
        //         #Elif "xy" eq "z"
        //         #Elif "xy" eq "xx"
        //         #Elif "xy" eq "xy"
          // exact.gd(ed,0,1,1,0,I1,I2,I3,n,0.);
                  ed=exy;
        // error = max(fabs(v(I1,I2,I3)-ed(I1,I2,I3)))/max(1.+fabs(ed(I1,I2,I3))); 
                real edMax = max(fabs(edLocal(I1,I2,I3)));
                edMax = ParallelUtility::getMaxValue(edMax);
                real errMax = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));
                errMax = ParallelUtility::getMaxValue(errMax);
                error = errMax/(1.+edMax);
                gridError=max(gridError,error);
        // cout << "u.xx       : Maximum relative error (order=" << order << ") = " << error << endl;
                checker.printMessage("u.xy",error,time);
        // --- compute the deriavtive (optimized) ----
                v=-123456.;
                time=getCPU();
                op.derivative(MappedGridOperators::xyDerivative,u,v,I1,I2,I3);
                time=getCPU()-time;
        // error = max(fabs(v(I1,I2,I3)-ed(I1,I2,I3)))/max(1.+fabs(ed(I1,I2,I3)));
                edMax = max(fabs(edLocal(I1,I2,I3)));
                edMax = ParallelUtility::getMaxValue(edMax);
                errMax = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));
                errMax = ParallelUtility::getMaxValue(errMax);
                error = errMax/(1.+edMax);
                gridError=max(gridError,error);
                  checker.printMessage("u.xy (opt)",error,time);
                }

        // testDerivatives(yy,yyDerivative);   // this is a macro
                {
        // --- compute the derivative here (non-optimized) ----
                time=getCPU();
                v=u.yy(I1,I2,I3);
                time=getCPU()-time;
        //         #If "yy" eq "x"
        //         #Elif "yy" eq "y"
        //         #Elif "yy" eq "z"
        //         #Elif "yy" eq "xx"
        //         #Elif "yy" eq "xy"
        //         #Elif "yy" eq "xz"
        //         #Elif "yy" eq "yy"
         //  exact.gd(ed,0,0,2,0,I1,I2,I3,n,0.);
                  ed=eyy;
        // error = max(fabs(v(I1,I2,I3)-ed(I1,I2,I3)))/max(1.+fabs(ed(I1,I2,I3))); 
                real edMax = max(fabs(edLocal(I1,I2,I3)));
                edMax = ParallelUtility::getMaxValue(edMax);
                real errMax = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));
                errMax = ParallelUtility::getMaxValue(errMax);
                error = errMax/(1.+edMax);
                gridError=max(gridError,error);
        // cout << "u.xx       : Maximum relative error (order=" << order << ") = " << error << endl;
                checker.printMessage("u.yy",error,time);
        // --- compute the deriavtive (optimized) ----
                v=-123456.;
                time=getCPU();
                op.derivative(MappedGridOperators::yyDerivative,u,v,I1,I2,I3);
                time=getCPU()-time;
        // error = max(fabs(v(I1,I2,I3)-ed(I1,I2,I3)))/max(1.+fabs(ed(I1,I2,I3)));
                edMax = max(fabs(edLocal(I1,I2,I3)));
                edMax = ParallelUtility::getMaxValue(edMax);
                errMax = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));
                errMax = ParallelUtility::getMaxValue(errMax);
                error = errMax/(1.+edMax);
                gridError=max(gridError,error);
                  checker.printMessage("u.yy (opt)",error,time);
                }
            
                if( order==2 )
      	{ // **** check 2nd-order accurate conservative and non-conservative derivative scalar derivative ****
                    for( int c=0; c<=1; c++ )
        	  {
          	    
                        bool conservative= c==0;
          	    op.useConservativeApproximations(conservative);
                        if( conservative )
          	    {
                            checker.setLabel("cons",3);
          	    }
          	    else
          	    {
                            checker.setLabel("std",3);
          	    }
          	    
          	    aString averageType;
          	    int na = conservative ? 1 : 0; // averaging only applies to conservative
          	    for( int a=0; a<=na; a++ )
          	    {
            	      if( a==0 )
            	      {
            		op.setAveragingType(MappedGridOperators::arithmeticAverage);
            		if( conservative )
              		  averageType=" arith";
            		else
              		  averageType="";
		// printf("  --Using arithmetic average\n");
            	      }
            	      else
            	      {
            		op.setAveragingType(MappedGridOperators::harmonicAverage);
            		averageType=" harmonic";
		// printf("  --Using harmonic average\n");
            	      }

            	      const char xyz[] = "xyz";
            	      for( int m1=0; m1<mg.numberOfDimensions(); m1++ )
            	      {
            		for( int m2=0; m2<mg.numberOfDimensions(); m2++ )
            		{
              		  aString name; sPrintF(name,"(s u.%c).%c%s",xyz[m1],xyz[m2], (const char*)averageType);

                  //  Compute a derivative: D_X1( s D_X2 u )
                  //      X1 = x_{m1},   X2 = x_{m2}
              		  MappedGridOperators::derivativeTypes derivType;
              		  if( m1==0 && m2==0 )
                		    derivType= MappedGridOperators::xDerivativeScalarXDerivative;
              		  else if( m1==0 && m2==1 )
                		    derivType= MappedGridOperators::xDerivativeScalarYDerivative;
              		  else if( m1==1 && m2==1 )
                		    derivType= MappedGridOperators::yDerivativeScalarYDerivative;
              		  else if( m1==1 && m2==0 )
                		    derivType= MappedGridOperators::yDerivativeScalarXDerivative;
              		  else if( m1==0 && m2==2 )
                		    derivType= MappedGridOperators::xDerivativeScalarZDerivative;
              		  else if( m1==1 && m2==2 )
                		    derivType= MappedGridOperators::yDerivativeScalarZDerivative;
              		  else if( m1==2 && m2==2 )
                		    derivType= MappedGridOperators::zDerivativeScalarZDerivative;
              		  else if( m1==2 && m2==0 )
                		    derivType= MappedGridOperators::zDerivativeScalarXDerivative;
              		  else if( m1==2 && m2==1 )
                		    derivType= MappedGridOperators::zDerivativeScalarYDerivative;

                  		  time=getCPU();
		  // ***  v=u.derivativeScalarDerivative(scalar,m1,m2,I1,I2,I3); // this is broken for P++
                  // *just use the opt version*
                  // Note: this gives slightly different results for 3D curvilinear non-conservative for
                  //   orthoSphere (north-pole) -- could be a difference in approx. rxz and rzx
                                    op.derivative(derivType,u,scalar,v,I1,I2,I3);
              		  time=getCPU()-time;

		  // printf("%s \n",(const char*)name);
		  // v.display(" *** old way ***");

              		  int ntd=0, nxd=(m1==0)+(m2==0), nyd=(m1==1)+(m2==1), nzd=(m1==2)+(m2==2);
              		  exact.gd(edd,ntd,nxd,nyd,nzd,I1,I2,I3,n);
                  // compute exact_X2
              		  nxd=(m2==0), nyd=(m2==1), nzd=(m2==2);  // 
              		  exact.gd(ed,ntd,nxd,nyd,nzd,I1,I2,I3,n);
            		
		  // op.derivative(derivType,scalar,sx,I1,I2,I3);
                  // NOTE: scalar.x(I1,I2,I3) = non-conservative
                  //       op.derivative(MappedGridOperators::xDerivative,.. ) =conservative !
              		  op.useConservativeApproximations(false); // turn off temporarily so we match old way
              		  if( m1==0 )
              		  {
		    // sx=scalar.x(I1,I2,I3);
                                        op.derivative(MappedGridOperators::xDerivative,scalar,sx,I1,I2,I3);
              		  }
              		  else if( m1==1 )
              		  {
		    // sx=scalar.y(I1,I2,I3);
                                        op.derivative(MappedGridOperators::yDerivative,scalar,sx,I1,I2,I3);
              		  }
              		  else
              		  {
		    // sx=scalar.z(I1,I2,I3);
                                        op.derivative(MappedGridOperators::zDerivative,scalar,sx,I1,I2,I3);
              		  }
              		  op.useConservativeApproximations(conservative); // reset 
              		  
// 		  error = max(fabs(v(I1,I2,I3)-(edd(I1,I2,I3)*scalar(I1,I2,I3)+
// 		  				sx(I1,I2,I3)*ed(I1,I2,I3)) ))/max(1.+fabs(scalar(I1,I2,I3)*edd(I1,I2,I3)));

                		  real eddMax = max(fabs(scalarLocal(I1,I2,I3)*eddLocal(I1,I2,I3)));
                		  eddMax = ParallelUtility::getMaxValue(eddMax);

                		  real errMax = max(fabs(vLocal(I1,I2,I3)-(eddLocal(I1,I2,I3)*scalarLocal(I1,I2,I3)+sxLocal(I1,I2,I3)*edLocal(I1,I2,I3))));

                		  errMax = ParallelUtility::getMaxValue(errMax);

                		  error = errMax/(1.+eddMax);


              		  gridError=max(gridError,error);
              		  checker.printMessage(name,error,time);

              		  if( false )
              		  {
                		    v=-123456.;
                		    time=getCPU();
                		    op.derivative(derivType,u,scalar,v,I1,I2,I3);
                		    time=getCPU()-time;

		    // printf("%s \n",(const char*)name);
		    // v.display(" *** opt version ***");

                		    error = max(fabs(v(I1,I2,I3)-(edd(I1,I2,I3)*scalar(I1,I2,I3)+
                                      						  sx(I1,I2,I3)*ed(I1,I2,I3)) ))/max(1.+fabs(scalar(I1,I2,I3)*edd(I1,I2,I3)));
		    // *** todo: add:  
                		    checker.printMessage(name+"(opt)",error,time);

                		    gridError=max(gridError,error);
              		  }
              		  
          	    
            		} // m2
            	      } //m1
          	    }  // for a
        	  } // for c
      	}


                if( order==4 || order==6 || order==8 )
      	{
	  // ==== So Far there are only a few 4th order conservative approximations =====
        	  op.useConservativeApproximations(true);
        	  checker.setLabel("cons",3);
        	  time=getCPU();
        	  op.derivative(MappedGridOperators::laplacianOperator,u,v,I1,I2,I3);
        	  time=getCPU()-time;

	  // error = max(fabs(v(I1,I2,I3)-(eLap(I1,I2,I3,n))))/max(fabs(eLap(I1,I2,I3,n)));
        	  real eLapMax = max(fabs(eLapLocal(I1,I2,I3,n))); eLapMax = ParallelUtility::getMaxValue(eLapMax);
        	  real errMax = max(fabs(vLocal(I1,I2,I3)-(eLapLocal(I1,I2,I3,n)))); errMax = ParallelUtility::getMaxValue(errMax);
        	  error = errMax/(eLapMax);


        	  gridError=max(gridError,error);
	  // cout << "u.laplacian: Maximum relative error (order=" << order << ") = " << error << endl;
        	  checker.printMessage("laplacian (opt)",error,time);

        	  v=-123456.;
        	  time=getCPU();
        	  op.derivative(MappedGridOperators::divergenceScalarGradient,u,scalar,v,I1,I2,I3);
        	  time=getCPU()-time;

        	  op.useConservativeApproximations(false);  // ************* reset for derivatives of scalar below

                    eLapMax = max(fabs(eLapLocal(I1,I2,I3,0)));
        	  eLapMax = ParallelUtility::getMaxValue(eLapMax);

        	  realSerialArray scalarxLocal(I1,I2,I3),scalaryLocal(I1,I2,I3);
        	  op.derivative(MappedGridOperators::xDerivative, scalarLocal,scalarxLocal,I1,I2,I3,0);
        	  op.derivative(MappedGridOperators::yDerivative, scalarLocal,scalaryLocal,I1,I2,I3,0);

        	  if( mg.numberOfDimensions()==2 )
        	  {
// 	    error = max(fabs(v(I1,I2,I3)
// 			     -( eLap(I1,I2,I3,0)*scalar(I1,I2,I3)
// 				+scalar.x(I1,I2,I3)(I1,I2,I3)*ex(I1,I2,I3,0)
// 				+scalar.y(I1,I2,I3)(I1,I2,I3)*ey(I1,I2,I3,0) )))/
// 	      (max(fabs(eLap(I1,I2,I3,0)))+1.);

          	    real errMax = max(fabs(vLocal(I1,I2,I3)
                           				   -( eLapLocal(I1,I2,I3,0)*scalarLocal(I1,I2,I3)
                              				      +scalarxLocal(I1,I2,I3)*exLocal(I1,I2,I3,0)
                              				      +scalaryLocal(I1,I2,I3)*eyLocal(I1,I2,I3,0) )));
          	    errMax = ParallelUtility::getMaxValue(errMax);

        	  }
        	  else
        	  {

// 	    error = max(fabs(v(I1,I2,I3)
// 			     -( eLap(I1,I2,I3,0)*scalar(I1,I2,I3)
// 				+scalar.x(I1,I2,I3)(I1,I2,I3)*ex(I1,I2,I3,0)
// 				+scalar.y(I1,I2,I3)(I1,I2,I3)*ey(I1,I2,I3,0)
// 				+scalar.z(I1,I2,I3)(I1,I2,I3)*ez(I1,I2,I3,0) )))/
// 	      (max(fabs(eLap(I1,I2,I3,0)))+1.);

          	    realSerialArray scalarzLocal(I1,I2,I3);
          	    op.derivative(MappedGridOperators::zDerivative, scalarLocal,scalarzLocal,I1,I2,I3,0);
          	    real errMax = max(fabs(vLocal(I1,I2,I3)
                           				   -( eLapLocal(I1,I2,I3,0)*scalarLocal(I1,I2,I3)
                              				      +scalarxLocal(I1,I2,I3)*exLocal(I1,I2,I3,0)
                              				      +scalaryLocal(I1,I2,I3)*eyLocal(I1,I2,I3,0)
                              				      +scalarzLocal(I1,I2,I3)*ezLocal(I1,I2,I3,0) )));
          	    errMax = ParallelUtility::getMaxValue(errMax);


        	  }

                    error = errMax/(eLapMax+1.);
        	  gridError=max(gridError,error);
        	  checker.printMessage("divSGrad (opt)",error,time);

                    checker.setLabel("std",3);
      	}
      	
	// ============ Check divergenceTensorGradient ================
                for( int c=0; c<=1; c++ )
      	{
        	  bool conservative= c==0;
        	  op.useConservativeApproximations(conservative);
        	  if( conservative )
          	    checker.setLabel("cons",3);
        	  else
          	    checker.setLabel("std",3);

        	  v=-123456.;
        	  time=getCPU();
        	  op.derivative(MappedGridOperators::divergenceTensorGradient,u,cTensor,v,I1,I2,I3);
        	  time=getCPU()-time;

        	  op.useConservativeApproximations(false);  // ************* reset for derivatives below

                    real eLapMax = max(fabs(eLapLocal(I1,I2,I3,0))); eLapMax = ParallelUtility::getMaxValue(eLapMax);

        	  if( mg.numberOfDimensions()==2 )
        	  {
// 	    error = max(fabs(v(I1,I2,I3)
// 	       -( exx(I1,I2,I3,0)*cTensor(I1,I2,I3,0)+cTensor.x(I1,I2,I3,0)(I1,I2,I3,0)*ex(I1,I2,I3,0)+
//                   exy(I1,I2,I3,0)*cTensor(I1,I2,I3,1)+cTensor.y(I1,I2,I3,1)(I1,I2,I3,1)*ex(I1,I2,I3,0)+
//                   exy(I1,I2,I3,0)*cTensor(I1,I2,I3,2)+cTensor.x(I1,I2,I3,2)(I1,I2,I3,2)*ey(I1,I2,I3,0)+
//                   eyy(I1,I2,I3,0)*cTensor(I1,I2,I3,3)+cTensor.y(I1,I2,I3,3)(I1,I2,I3,3)*ey(I1,I2,I3,0))))/
// 	      (max(fabs(eLap(I1,I2,I3,0)))+1.);

          	    realSerialArray ctx0(I1,I2,I3), cty1(I1,I2,I3), ctx2(I1,I2,I3), cty3(I1,I2,I3);
                        op.useConservativeApproximations(false); // turn off temporarily so we match old way
          	    op.derivative(MappedGridOperators::xDerivative, cTensorLocal,ctx0,I1,I2,I3,0);
          	    op.derivative(MappedGridOperators::yDerivative, cTensorLocal,cty1,I1,I2,I3,1);
          	    op.derivative(MappedGridOperators::xDerivative, cTensorLocal,ctx2,I1,I2,I3,2);
          	    op.derivative(MappedGridOperators::yDerivative, cTensorLocal,cty3,I1,I2,I3,3);
          	    op.useConservativeApproximations(conservative);
          	    
          	    errMax = max(fabs(vLocal(I1,I2,I3)
             	       -( exxLocal(I1,I2,I3,0)*cTensorLocal(I1,I2,I3,0)+ctx0*exLocal(I1,I2,I3,0)+
                                    exyLocal(I1,I2,I3,0)*cTensorLocal(I1,I2,I3,1)+cty1*exLocal(I1,I2,I3,0)+
                                    exyLocal(I1,I2,I3,0)*cTensorLocal(I1,I2,I3,2)+ctx2*eyLocal(I1,I2,I3,0)+
                                    eyyLocal(I1,I2,I3,0)*cTensorLocal(I1,I2,I3,3)+cty3*eyLocal(I1,I2,I3,0))));
                        errMax = ParallelUtility::getMaxValue(errMax);
        	  }
        	  else
        	  {
// 	    error = max(fabs(v(I1,I2,I3)
// 	       -( exx(I1,I2,I3,0)*cTensor(I1,I2,I3,0)+cTensor.x(I1,I2,I3,0)(I1,I2,I3,0)*ex(I1,I2,I3,0)+
//                   exy(I1,I2,I3,0)*cTensor(I1,I2,I3,1)+cTensor.y(I1,I2,I3,1)(I1,I2,I3,1)*ex(I1,I2,I3,0)+
//                   exz(I1,I2,I3,0)*cTensor(I1,I2,I3,2)+cTensor.z(I1,I2,I3,2)(I1,I2,I3,2)*ex(I1,I2,I3,0)+
//                   exy(I1,I2,I3,0)*cTensor(I1,I2,I3,3)+cTensor.x(I1,I2,I3,3)(I1,I2,I3,3)*ey(I1,I2,I3,0)+
//                   eyy(I1,I2,I3,0)*cTensor(I1,I2,I3,4)+cTensor.y(I1,I2,I3,4)(I1,I2,I3,4)*ey(I1,I2,I3,0)+
//                   eyz(I1,I2,I3,0)*cTensor(I1,I2,I3,5)+cTensor.z(I1,I2,I3,5)(I1,I2,I3,5)*ey(I1,I2,I3,0)+
//                   exz(I1,I2,I3,0)*cTensor(I1,I2,I3,6)+cTensor.x(I1,I2,I3,6)(I1,I2,I3,6)*ez(I1,I2,I3,0)+
//                   eyz(I1,I2,I3,0)*cTensor(I1,I2,I3,7)+cTensor.y(I1,I2,I3,7)(I1,I2,I3,7)*ez(I1,I2,I3,0)+
//                   ezz(I1,I2,I3,0)*cTensor(I1,I2,I3,8)+cTensor.z(I1,I2,I3,8)(I1,I2,I3,8)*ez(I1,I2,I3,0))))/
// 	      (max(fabs(eLap(I1,I2,I3,0)))+1.);

          	    realSerialArray ctx0(I1,I2,I3), ctx3(I1,I2,I3), ctx6(I1,I2,I3);
          	    realSerialArray cty1(I1,I2,I3), cty4(I1,I2,I3), cty7(I1,I2,I3);
          	    realSerialArray ctz2(I1,I2,I3), ctz5(I1,I2,I3), ctz8(I1,I2,I3);

                        op.useConservativeApproximations(false); // turn off temporarily so we match old way
          	    op.derivative(MappedGridOperators::xDerivative, cTensorLocal,ctx0,I1,I2,I3,0);
          	    op.derivative(MappedGridOperators::yDerivative, cTensorLocal,cty1,I1,I2,I3,1);
          	    op.derivative(MappedGridOperators::zDerivative, cTensorLocal,ctz2,I1,I2,I3,2);
          	    op.derivative(MappedGridOperators::xDerivative, cTensorLocal,ctx3,I1,I2,I3,3);
          	    op.derivative(MappedGridOperators::yDerivative, cTensorLocal,cty4,I1,I2,I3,4);
          	    op.derivative(MappedGridOperators::zDerivative, cTensorLocal,ctz5,I1,I2,I3,5);
          	    op.derivative(MappedGridOperators::xDerivative, cTensorLocal,ctx6,I1,I2,I3,6);
          	    op.derivative(MappedGridOperators::yDerivative, cTensorLocal,cty7,I1,I2,I3,7);
          	    op.derivative(MappedGridOperators::zDerivative, cTensorLocal,ctz8,I1,I2,I3,8);
                        op.useConservativeApproximations(conservative);

          	    errMax = max(fabs(vLocal(I1,I2,I3)
             	       -( exxLocal(I1,I2,I3,0)*cTensorLocal(I1,I2,I3,0)+ctx0*exLocal(I1,I2,I3,0)+
                                    exyLocal(I1,I2,I3,0)*cTensorLocal(I1,I2,I3,1)+cty1*exLocal(I1,I2,I3,0)+
                                    exzLocal(I1,I2,I3,0)*cTensorLocal(I1,I2,I3,2)+ctz2*exLocal(I1,I2,I3,0)+
                                    exyLocal(I1,I2,I3,0)*cTensorLocal(I1,I2,I3,3)+ctx3*eyLocal(I1,I2,I3,0)+
                                    eyyLocal(I1,I2,I3,0)*cTensorLocal(I1,I2,I3,4)+cty4*eyLocal(I1,I2,I3,0)+
                                    eyzLocal(I1,I2,I3,0)*cTensorLocal(I1,I2,I3,5)+ctz5*eyLocal(I1,I2,I3,0)+
                                    exzLocal(I1,I2,I3,0)*cTensorLocal(I1,I2,I3,6)+ctx6*ezLocal(I1,I2,I3,0)+
                                    eyzLocal(I1,I2,I3,0)*cTensorLocal(I1,I2,I3,7)+cty7*ezLocal(I1,I2,I3,0)+
                                    ezzLocal(I1,I2,I3,0)*cTensorLocal(I1,I2,I3,8)+ctz8*ezLocal(I1,I2,I3,0))));
                        errMax = ParallelUtility::getMaxValue(errMax);
        	  }

        	  error = errMax/(eLapMax+1.);

        	  gridError=max(gridError,error);
        	  checker.printMessage("divTGrad (opt)",error,time);

                    checker.setLabel("std",3);
        	  
      	} // end for( c ) : end divTensorGrad
            
      	
      	if( mg.numberOfDimensions()==2 )
      	{
        	  time=getCPU();
        	  v=w.vorticity(I1,I2,I3);
        	  time=getCPU()-time;

                    exact.gd(ed ,0,1,0,0,I1,I2,I3,1); // ex(1)
// 	  error = max(fabs(v(I1,I2,I3)-(ed(I1,I2,I3)-ey(I1,I2,I3,0))))/
// 	                      (max(fabs(ed(I1,I2,I3)-ey(I1,I2,I3,0)))+1.);

        	  real edMax = max(fabs(edLocal(I1,I2,I3)-eyLocal(I1,I2,I3,0)));
        	  edMax = ParallelUtility::getMaxValue(edMax);
        	  errMax = max(fabs(vLocal(I1,I2,I3)-(edLocal(I1,I2,I3)-eyLocal(I1,I2,I3,0))));
        	  errMax = ParallelUtility::getMaxValue(errMax);
        	  error = errMax/(edMax+1.);

                    gridError=max(gridError,error);
                	  checker.printMessage("u.vorticity",error,time);

                  #ifndef USE_PPP
          // *********************** FIX FOR PARALLEL **********
        	  time=getCPU();
        	  const realMappedGridFunction & grad = u.grad();  // *********************** FIX FOR PARALLEL **********
          // realMappedGridFunction grad(mg,all,all,all,numberOfDimensions);
          // op.derivative(MappedGridOperators::gradient,w,grad,I1,I2,I3);  // *********************** FIX FOR PARALLEL **********
        	  time=getCPU()-time;
                    OV_GET_SERIAL_ARRAY_CONST(real,grad,gradLocal);

// 	  error = max(
//                       max(fabs(grad(I1,I2,I3,0)-ex(I1,I2,I3,0)))/(max(fabs(ex(I1,I2,I3,0)))+1.),
//                       max(fabs(grad(I1,I2,I3,1)-ey(I1,I2,I3,0)))/(max(fabs(ey(I1,I2,I3,0)))+1.) );

                    real exMax = max(fabs(exLocal(I1,I2,I3,0)));  exMax = ParallelUtility::getMaxValue(exMax);
                    real eyMax = max(fabs(eyLocal(I1,I2,I3,0)));  eyMax = ParallelUtility::getMaxValue(eyMax);

                    real errx = max(fabs(gradLocal(I1,I2,I3,0)-exLocal(I1,I2,I3,0))); errx = ParallelUtility::getMaxValue(errx);
                    real erry = max(fabs(gradLocal(I1,I2,I3,1)-eyLocal(I1,I2,I3,0))); erry = ParallelUtility::getMaxValue(erry);
        	  
                    error = max( errx/(exMax+1.) , erry/(eyMax+1.) );

                    gridError=max(gridError,error);
                	  checker.printMessage("u.grad",error,time);

        	  time=getCPU();
        	  const realMappedGridFunction & gradw = w.grad();// *********************** FIX FOR PARALLEL **********
          // realMappedGridFunction gradw(mg,all,all,all,numberOfDimensions,numberOfDimensions);
          // op.derivative(MappedGridOperators::gradient,w,gradw,I1,I2,I3);  // *********************** FIX FOR PARALLEL **********
        	  time=getCPU()-time;
                    OV_GET_SERIAL_ARRAY_CONST(real,gradw,gradwLocal);

                    error=0.;
        	  for( int c=0; c<mg.numberOfDimensions(); c++ )
        	  {
                        exact.gd(ed ,0,1,0,0,I1,I2,I3,c); // ex(c)
                        exact.gd(edd,0,0,1,0,I1,I2,I3,c); // ey(c)

// 	    error = max(error,max(
// 	      max(fabs(gradw(I1,I2,I3,c,0)-ed (I1,I2,I3)))/(max(fabs(ed (I1,I2,I3)))+4.),
// 	      max(fabs(gradw(I1,I2,I3,c,1)-edd(I1,I2,I3)))/(max(fabs(edd(I1,I2,I3)))+4.))
// 	      );

                      real edMax = max(fabs(edLocal(I1,I2,I3)));    edMax = ParallelUtility::getMaxValue(edMax);
                      real eddMax = max(fabs(eddLocal(I1,I2,I3)));  eddMax = ParallelUtility::getMaxValue(eddMax);

                      #define GC(c,d) (c)+numberOfDimensions*(d)
         	   real errx = max(fabs(gradwLocal(I1,I2,I3,GC(c,0))-edLocal(I1,I2,I3)));  errx = ParallelUtility::getMaxValue(errx);
         	   real erry = max(fabs(gradwLocal(I1,I2,I3,GC(c,1))-eddLocal(I1,I2,I3))); erry = ParallelUtility::getMaxValue(erry);

                      error = max( error, errx/(edMax+4.), erry/(eddMax+4.) );

        	  }
        	  
                    gridError=max(gridError,error);
                	  checker.printMessage("w.grad",error,time);
                  #endif

                    for( int c=0; c<=1; c++ )
        	  {
                        bool conservative= c==0;
          	    op.useConservativeApproximations(conservative);
                        if( c==0 )
          	    {
                            checker.setLabel("cons",3);
          	    }
          	    else
          	    {
                            checker.setLabel("std",3);
          	    }
          	    

          	    time=getCPU();

            // v=w.div(I1,I2,I3);  // divergence
                        op.derivative(MappedGridOperators::divergence,w,v,I1,I2,I3);
          	    time=getCPU()-time;

	    // ::display(v(I1,I2,I3),"v=div(w)");
          	    
                        exact.gd(ed,0,0,1,0,I1,I2,I3,1); // ey(1)
	    // error = max(fabs(v(I1,I2,I3)-(ex(I1,I2,I3,0)+ed(I1,I2,I3))))/
	    //                                max(1.+fabs(ex(I1,I2,I3)+ed(I1,I2,I3)));

                        real edMax = max(fabs(exLocal(I1,I2,I3)+edLocal(I1,I2,I3)));      edMax = ParallelUtility::getMaxValue(edMax);
          	    real errx = max(fabs(vLocal(I1,I2,I3)-(exLocal(I1,I2,I3,0)+edLocal(I1,I2,I3))));  errx = ParallelUtility::getMaxValue(errx);
                        error = errx/(1.+edMax);
          	    
          	    gridError=max(gridError,error);

                        printf("\n >>>");
          	    checker.printMessage("u.div (opt)",error,time);
                        printf("\n");


                        if( conservative && order!=2 )
                            continue;


          	    aString averageType;
          	    int na = conservative ? 1 : 0; // averaging only applies to conservative
          	    for( int a=0; a<=na; a++ )
          	    {
            	      if( a==0 )
            	      {
            		op.setAveragingType(MappedGridOperators::arithmeticAverage);
            		if( conservative )
              		  averageType="arith";
            		else
              		  averageType="";
		// printf("  --Using arithmetic average\n");
            	      }
            	      else
            	      {
            		op.setAveragingType(MappedGridOperators::harmonicAverage);
            		averageType="harmonic";
		// printf("  --Using harmonic average\n");
            	      }

            	      time=getCPU();
              // #ifdef USE_PPP
            	      v=u.laplacian(I1,I2,I3); 
              // #else
              //   op.derivative(MappedGridOperators::laplacianOperator,u,v,I1,I2,I3);
              // #endif
            	      time=getCPU()-time;

	      // error = max(fabs(v(I1,I2,I3)-eLap(I1,I2,I3,n)))/max(fabs(eLap(I1,I2,I3,n)));

                            real eLapMax = max(fabs(eLapLocal(I1,I2,I3,0))); eLapMax = ParallelUtility::getMaxValue(eLapMax);
                            real errx = max(fabs(vLocal(I1,I2,I3)-eLapLocal(I1,I2,I3,n)));  errx = ParallelUtility::getMaxValue(errx);
                            error = errx/eLapMax;

            	      gridError=max(gridError,error);
	      // cout << "u.laplacian: Maximum relative error (order=" << order << ") = " << error << endl;
            	      checker.printMessage(sPrintF(buff,"laplacian %s",(const char*)averageType),error,time);

            	      if( c==1 )
            	      {
            		v=-123456.;
            		time=getCPU();
            		op.derivative(MappedGridOperators::laplacianOperator,u,v,I1,I2,I3);
            		time=getCPU()-time;

		// error = max(fabs(v(I1,I2,I3)-eLap(I1,I2,I3,n)))/max(fabs(eLap(I1,I2,I3,n)));

            		real eLapMax = max(fabs(eLapLocal(I1,I2,I3,n))); eLapMax = ParallelUtility::getMaxValue(eLapMax);
            		real errx = max(fabs(vLocal(I1,I2,I3)-eLapLocal(I1,I2,I3,n)));  errx = ParallelUtility::getMaxValue(errx);
            		error = errx/eLapMax;

            		gridError=max(gridError,error);
		// cout << "u.laplacian: Maximum relative error (order=" << order << ") = " << error << endl;
            		checker.printMessage("laplacian(opt)",error,time);
            	      }
          	    

              // if( order>6 ) continue;  // *********** divScalarGrad not 8th order yet

                        
            	      time=getCPU();
                            #ifndef USE_PPP
                  	        v=u.divScalarGrad(scalar,I1,I2,I3);   // this case broken for P++
                            #else
                                op.derivative(MappedGridOperators::divergenceScalarGradient,u,scalar,v,I1,I2,I3);
                            #endif		
            	      time=getCPU()-time;

// 	      error = max(fabs(v(I1,I2,I3)
// 			       -( eLap(I1,I2,I3,0)*scalar(I1,I2,I3)
// 				  +scalar.x(I1,I2,I3)(I1,I2,I3)*ex(I1,I2,I3,0)
// 				  +scalar.y(I1,I2,I3)(I1,I2,I3)*ey(I1,I2,I3,0) )))/
// 		(max(fabs(eLap(I1,I2,I3,0)))+1.);

                            eLapMax = max(fabs(eLapLocal(I1,I2,I3,0))); eLapMax = ParallelUtility::getMaxValue(eLapMax);
            	      realSerialArray scalarxLocal(I1,I2,I3),scalaryLocal(I1,I2,I3);
                            op.useConservativeApproximations(false); // turn off temporarily so we match old way
            	      op.derivative(MappedGridOperators::xDerivative, scalarLocal,scalarxLocal,I1,I2,I3,0);
            	      op.derivative(MappedGridOperators::yDerivative, scalarLocal,scalaryLocal,I1,I2,I3,0);
                            op.useConservativeApproximations(conservative); // reset 

            	      real errMax = max(fabs(vLocal(I1,I2,I3)
                             				     -( eLapLocal(I1,I2,I3,0)*scalarLocal(I1,I2,I3)
                              					+scalarxLocal(I1,I2,I3)*exLocal(I1,I2,I3,0)
                              					+scalaryLocal(I1,I2,I3)*eyLocal(I1,I2,I3,0) )));
            	      errMax = ParallelUtility::getMaxValue(errMax);

            	      error = errMax/(eLapMax+1.);
            	      
            	      gridError=max(gridError,error);
            	      if( FALSE )
            	      {
            		display(u.divScalarGrad(scalar,I1,I2,I3)(I1,I2,I3),"u.divScalarGrad");
            		display(evaluate( eLap(I1,I2,I3,0)*scalar(I1,I2,I3)
                          				  +scalar.x(I1,I2,I3)(I1,I2,I3)*ex(I1,I2,I3,0)
                          				  +scalar.y(I1,I2,I3)(I1,I2,I3)*ey(I1,I2,I3,0) ),"exact.divScalarGrad");
            	      }
            	      checker.printMessage(sPrintF(buff,"divSGrad %s",(const char*)averageType),error,time);


            	      v=-123456.;
            	      time=getCPU();
            	      op.derivative(MappedGridOperators::divergenceScalarGradient,u,scalar,v,I1,I2,I3);
            	      time=getCPU()-time;
// 	      error = max(fabs(v(I1,I2,I3)
// 			       -( eLap(I1,I2,I3,0)*scalar(I1,I2,I3)
// 				  +scalar.x(I1,I2,I3)(I1,I2,I3)*ex(I1,I2,I3,0)
// 				  +scalar.y(I1,I2,I3)(I1,I2,I3)*ey(I1,I2,I3,0) )))/
// 		(max(fabs(eLap(I1,I2,I3,0)))+1.);


            	      errMax = max(fabs(vLocal(I1,I2,I3)
                        				-( eLapLocal(I1,I2,I3,0)*scalarLocal(I1,I2,I3)
                           				   +scalarxLocal(I1,I2,I3)*exLocal(I1,I2,I3,0)
                           				   +scalaryLocal(I1,I2,I3)*eyLocal(I1,I2,I3,0) )));
            	      errMax = ParallelUtility::getMaxValue(errMax);
            	      error = errMax/(eLapMax+1.);


            	      gridError=max(gridError,error);
            	      checker.printMessage(sPrintF(buff,"divSGrad (opt) %s",(const char*)averageType),error,time);
            	      if( false )
            	      {
            		display(v(I1,I2,I3),"divScalarGrad (opt)");
            		display(evaluate( eLap(I1,I2,I3,0)*scalar(I1,I2,I3)
                          				  +scalar.x(I1,I2,I3)(I1,I2,I3)*ex(I1,I2,I3,0)
                          				  +scalar.y(I1,I2,I3)(I1,I2,I3)*ey(I1,I2,I3,0) ),"exact.divScalarGrad");
            	      }
          	    

          	    } // end for a
        	  }
      	}
      	else  // 3D 
      	{
          // testDerivatives(z,zDerivative);   // this is a macro
                    {
          // --- compute the derivative here (non-optimized) ----
                    time=getCPU();
                    v=u.z(I1,I2,I3);
                    time=getCPU()-time;
          //           #If "z" eq "x"
          //           #Elif "z" eq "y"
          //           #Elif "z" eq "z"
           //  exact.gd(ed,0,0,0,1,I1,I2,I3,n,0.);
                      ed=ez;
          // error = max(fabs(v(I1,I2,I3)-ed(I1,I2,I3)))/max(1.+fabs(ed(I1,I2,I3))); 
                    real edMax = max(fabs(edLocal(I1,I2,I3)));
                    edMax = ParallelUtility::getMaxValue(edMax);
                    real errMax = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));
                    errMax = ParallelUtility::getMaxValue(errMax);
                    error = errMax/(1.+edMax);
                    gridError=max(gridError,error);
          // cout << "u.xx       : Maximum relative error (order=" << order << ") = " << error << endl;
                    checker.printMessage("u.z",error,time);
          // --- compute the deriavtive (optimized) ----
                    v=-123456.;
                    time=getCPU();
                    op.derivative(MappedGridOperators::zDerivative,u,v,I1,I2,I3);
                    time=getCPU()-time;
          // error = max(fabs(v(I1,I2,I3)-ed(I1,I2,I3)))/max(1.+fabs(ed(I1,I2,I3)));
                    edMax = max(fabs(edLocal(I1,I2,I3)));
                    edMax = ParallelUtility::getMaxValue(edMax);
                    errMax = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));
                    errMax = ParallelUtility::getMaxValue(errMax);
                    error = errMax/(1.+edMax);
                    gridError=max(gridError,error);
                      checker.printMessage("u.z (opt)",error,time);
                    }

        	  if( debug & 4 )
        	  {
                        display(u(I1,I2,I3),"u","%4.2f ");
                        display(u(I1,I2,I3+1)-u(I1,I2,I3),"ut","%4.2f ");
          	    display(fabs(v(I1,I2,I3)-ez(I1,I2,I3,n)),"Error in u.z","%4.2f ");
          	    display(v(I1,I2,I3),"u.z","%4.2f ");
          	    display(ez(I1,I2,I3,n),"exact.z","%4.2f ");

                        exit(1);
        	  }

          // testDerivatives(xz,xzDerivative);   // this is a macro
                    {
          // --- compute the derivative here (non-optimized) ----
                    time=getCPU();
                    v=u.xz(I1,I2,I3);
                    time=getCPU()-time;
          //           #If "xz" eq "x"
          //           #Elif "xz" eq "y"
          //           #Elif "xz" eq "z"
          //           #Elif "xz" eq "xx"
          //           #Elif "xz" eq "xy"
          //           #Elif "xz" eq "xz"
           //  exact.gd(ed,0,1,0,1,I1,I2,I3,n,0.);
                      ed=exz;
          // error = max(fabs(v(I1,I2,I3)-ed(I1,I2,I3)))/max(1.+fabs(ed(I1,I2,I3))); 
                    real edMax = max(fabs(edLocal(I1,I2,I3)));
                    edMax = ParallelUtility::getMaxValue(edMax);
                    real errMax = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));
                    errMax = ParallelUtility::getMaxValue(errMax);
                    error = errMax/(1.+edMax);
                    gridError=max(gridError,error);
          // cout << "u.xx       : Maximum relative error (order=" << order << ") = " << error << endl;
                    checker.printMessage("u.xz",error,time);
          // --- compute the deriavtive (optimized) ----
                    v=-123456.;
                    time=getCPU();
                    op.derivative(MappedGridOperators::xzDerivative,u,v,I1,I2,I3);
                    time=getCPU()-time;
          // error = max(fabs(v(I1,I2,I3)-ed(I1,I2,I3)))/max(1.+fabs(ed(I1,I2,I3)));
                    edMax = max(fabs(edLocal(I1,I2,I3)));
                    edMax = ParallelUtility::getMaxValue(edMax);
                    errMax = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));
                    errMax = ParallelUtility::getMaxValue(errMax);
                    error = errMax/(1.+edMax);
                    gridError=max(gridError,error);
                      checker.printMessage("u.xz (opt)",error,time);
                    }
          // testDerivatives(yz,yzDerivative);   // this is a macro
                    {
          // --- compute the derivative here (non-optimized) ----
                    time=getCPU();
                    v=u.yz(I1,I2,I3);
                    time=getCPU()-time;
          //           #If "yz" eq "x"
          //           #Elif "yz" eq "y"
          //           #Elif "yz" eq "z"
          //           #Elif "yz" eq "xx"
          //           #Elif "yz" eq "xy"
          //           #Elif "yz" eq "xz"
          //           #Elif "yz" eq "yy"
          //           #Elif "yz" eq "yz"
           //  exact.gd(ed,0,0,1,1,I1,I2,I3,n,0.);
                      ed=eyz;
          // error = max(fabs(v(I1,I2,I3)-ed(I1,I2,I3)))/max(1.+fabs(ed(I1,I2,I3))); 
                    real edMax = max(fabs(edLocal(I1,I2,I3)));
                    edMax = ParallelUtility::getMaxValue(edMax);
                    real errMax = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));
                    errMax = ParallelUtility::getMaxValue(errMax);
                    error = errMax/(1.+edMax);
                    gridError=max(gridError,error);
          // cout << "u.xx       : Maximum relative error (order=" << order << ") = " << error << endl;
                    checker.printMessage("u.yz",error,time);
          // --- compute the deriavtive (optimized) ----
                    v=-123456.;
                    time=getCPU();
                    op.derivative(MappedGridOperators::yzDerivative,u,v,I1,I2,I3);
                    time=getCPU()-time;
          // error = max(fabs(v(I1,I2,I3)-ed(I1,I2,I3)))/max(1.+fabs(ed(I1,I2,I3)));
                    edMax = max(fabs(edLocal(I1,I2,I3)));
                    edMax = ParallelUtility::getMaxValue(edMax);
                    errMax = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));
                    errMax = ParallelUtility::getMaxValue(errMax);
                    error = errMax/(1.+edMax);
                    gridError=max(gridError,error);
                      checker.printMessage("u.yz (opt)",error,time);
                    }
          // testDerivatives(zz,zzDerivative);   // this is a macro
                    {
          // --- compute the derivative here (non-optimized) ----
                    time=getCPU();
                    v=u.zz(I1,I2,I3);
                    time=getCPU()-time;
          //           #If "zz" eq "x"
          //           #Elif "zz" eq "y"
          //           #Elif "zz" eq "z"
          //           #Elif "zz" eq "xx"
          //           #Elif "zz" eq "xy"
          //           #Elif "zz" eq "xz"
          //           #Elif "zz" eq "yy"
          //           #Elif "zz" eq "yz"
          //           #Elif "zz" eq "zz"
            // exact.gd(ed,0,0,0,2,I1,I2,I3,n,0.);
                      ed=ezz;
          // error = max(fabs(v(I1,I2,I3)-ed(I1,I2,I3)))/max(1.+fabs(ed(I1,I2,I3))); 
                    real edMax = max(fabs(edLocal(I1,I2,I3)));
                    edMax = ParallelUtility::getMaxValue(edMax);
                    real errMax = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));
                    errMax = ParallelUtility::getMaxValue(errMax);
                    error = errMax/(1.+edMax);
                    gridError=max(gridError,error);
          // cout << "u.xx       : Maximum relative error (order=" << order << ") = " << error << endl;
                    checker.printMessage("u.zz",error,time);
          // --- compute the deriavtive (optimized) ----
                    v=-123456.;
                    time=getCPU();
                    op.derivative(MappedGridOperators::zzDerivative,u,v,I1,I2,I3);
                    time=getCPU()-time;
          // error = max(fabs(v(I1,I2,I3)-ed(I1,I2,I3)))/max(1.+fabs(ed(I1,I2,I3)));
                    edMax = max(fabs(edLocal(I1,I2,I3)));
                    edMax = ParallelUtility::getMaxValue(edMax);
                    errMax = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));
                    errMax = ParallelUtility::getMaxValue(errMax);
                    error = errMax/(1.+edMax);
                    gridError=max(gridError,error);
                      checker.printMessage("u.zz (opt)",error,time);
                    }

                    time=getCPU();
        	  const realMappedGridFunction & vort = w.vorticity();
        	  time=getCPU()-time;
        	  OV_GET_SERIAL_ARRAY_CONST(real,vort,vortLocal);
        	  
                    exact.gd(ed ,0,0,1,0,I1,I2,I3,2); // ey(2)
                    exact.gd(edd,0,0,0,1,I1,I2,I3,1); // ez(1)

	  // error = max(fabs(vort(I1,I2,I3,0)-(ed(I1,I2,I3)-edd(I1,I2,I3))))/
	  //   (max(fabs(ed(I1,I2,I3))+fabs(edd(I1,I2,I3)))+4.);

        	  real edMax = max(fabs(edLocal(I1,I2,I3)+eddLocal(I1,I2,I3)));      edMax = ParallelUtility::getMaxValue(edMax);
        	  real errx = max(fabs(vortLocal(I1,I2,I3,0)-(edLocal(I1,I2,I3,0)-eddLocal(I1,I2,I3))));  errx = ParallelUtility::getMaxValue(errx);
        	  error = errx/(4.+edMax);


                    gridError=max(gridError,error);
                    checker.printMessage("vorticity0",error,time);

                    exact.gd(ed ,0,1,0,0,I1,I2,I3,2); // ex(2)

	  // error = max(fabs(vort(I1,I2,I3,1)-(ez(I1,I2,I3,0)-ed(I1,I2,I3))))/
	  //   (max(fabs(ez(I1,I2,I3,0))+fabs(ed(I1,I2,I3)))+4.);

        	  edMax = max(fabs(ezLocal(I1,I2,I3,0)+edLocal(I1,I2,I3)));      edMax = ParallelUtility::getMaxValue(edMax);
        	  errx = max(fabs(vortLocal(I1,I2,I3,1)-(ezLocal(I1,I2,I3,0)-edLocal(I1,I2,I3))));  errx = ParallelUtility::getMaxValue(errx);
        	  error = errx/(4.+edMax);

                    gridError=max(gridError,error);
                    checker.printMessage("vorticity1",error,time);

                    exact.gd(ed ,0,1,0,0,I1,I2,I3,1); // ex(1)

	  // error = max(fabs(vort(I1,I2,I3,2)-(ed(I1,I2,I3)-ey(I1,I2,I3,0))))/
	  //   (max(fabs(ed(I1,I2,I3))+fabs(ey(I1,I2,I3,0)))+4.);

        	  edMax = max(fabs(eyLocal(I1,I2,I3,0)+edLocal(I1,I2,I3)));      edMax = ParallelUtility::getMaxValue(edMax);
        	  errx = max(fabs(vortLocal(I1,I2,I3,2)-(edLocal(I1,I2,I3)-eyLocal(I1,I2,I3,0))));  errx = ParallelUtility::getMaxValue(errx);
        	  error = errx/(4.+edMax);

                    gridError=max(gridError,error);
                    checker.printMessage("vorticity2",error,time);

                #ifndef USE_PPP
          // -- fix these for parallel 
                    time=getCPU();
        	  const realMappedGridFunction & grad = u.grad();
        	  time=getCPU()-time;
        	  error = max(
                                            max(fabs(grad(I1,I2,I3,0)-ex(I1,I2,I3,0)))/(max(fabs(ex(I1,I2,I3,0)))+1.),
                                            max(fabs(grad(I1,I2,I3,1)-ey(I1,I2,I3,0)))/(max(fabs(ey(I1,I2,I3,0)))+1.),
                                            max(fabs(grad(I1,I2,I3,2)-ez(I1,I2,I3,0)))/(max(fabs(ez(I1,I2,I3,0)))+1.)
          	    );
                    gridError=max(gridError,error);
                    checker.printMessage("grad",error,time);

                    time=getCPU();
        	  const realMappedGridFunction & gradw = w.grad();
        	  time=getCPU()-time;
        	  error = max(
                                            max(fabs(gradw(I1,I2,I3,0,0)-ex(I1,I2,I3,0)))/(max(fabs(ex(I1,I2,I3,0)))+2.),
                                            max(fabs(gradw(I1,I2,I3,0,1)-ey(I1,I2,I3,0)))/(max(fabs(ey(I1,I2,I3,0)))+2.),
                                            max(fabs(gradw(I1,I2,I3,0,2)-ez(I1,I2,I3,0)))/(max(fabs(ez(I1,I2,I3,0)))+2.));

        	  exact.gd(ed ,0,1,0,0,I1,I2,I3,1); // ex(1)
        	  error = max(error,max(fabs(gradw(I1,I2,I3,1,0)-ed(I1,I2,I3)))/(max(fabs(ed(I1,I2,I3)))+2.));
        	  exact.gd(ed ,0,0,1,0,I1,I2,I3,1); // ey(1)
        	  error = max(error,max(fabs(gradw(I1,I2,I3,1,1)-ed(I1,I2,I3)))/(max(fabs(ed(I1,I2,I3)))+2.));
        	  exact.gd(ed ,0,0,0,1,I1,I2,I3,1); // ez(1)
        	  error = max(error,max(fabs(gradw(I1,I2,I3,1,2)-ed(I1,I2,I3)))/(max(fabs(ed(I1,I2,I3)))+2.));

                    gridError=max(gridError,error);
                    checker.printMessage("w.grad",error,time);
                #endif

                    for( int c=0; c<=1; c++ )
        	  {
                        bool conservative= c==0;
          	    op.useConservativeApproximations(conservative);

                        if( c==0 )
          	    {
                            checker.setLabel("cons",3);
          	    }
          	    else
          	    {
                            checker.setLabel("std",3);
          	    }
          	    

            // ------------------------ div --------------------------
          	    time=getCPU();
	    // v=w.div(I1,I2,I3);
                        op.derivative(MappedGridOperators::divergence,w,v,I1,I2,I3);
          	    time=getCPU()-time;

                        exact.gd(ed ,0,0,1,0,I1,I2,I3,1); // ey(1)
                        exact.gd(edd,0,0,0,1,I1,I2,I3,2); // ez(2)

	    // error=max(fabs(v(I1,I2,I3)-(ex(I1,I2,I3,0)+ed(I1,I2,I3)+edd(I1,I2,I3))))/
	    //   max(fabs(ex(I1,I2,I3,0))+fabs(ed(I1,I2,I3))+fabs(edd(I1,I2,I3))+2.);


                        real edMax = max(fabs(exLocal(I1,I2,I3,0))+fabs(edLocal(I1,I2,I3))+fabs(eddLocal(I1,I2,I3)));  
                        edMax = ParallelUtility::getMaxValue(edMax);
          	    real errx = max(fabs(vLocal(I1,I2,I3)-(exLocal(I1,I2,I3,0)+edLocal(I1,I2,I3)+eddLocal(I1,I2,I3))));  
                        errx = ParallelUtility::getMaxValue(errx);
                        error = errx/(2.+edMax);

          	    gridError=max(gridError,error);

          	    printf("\n >>>");
          	    checker.printMessage("u.div (opt)",error,time);
                        printf("\n");
            // ------------------------------------------------------

                        if( c==0 && order!=2 )
                            continue;

          	    time=getCPU();
          	    v=u.laplacian(I1,I2,I3);
          	    time=getCPU()-time;

	    // error = max(fabs(v(I1,I2,I3) -(eLap(I1,I2,I3,n))))/max(fabs(eLap(I1,I2,I3,n)));

          	    real eLapMax = max(fabs(eLapLocal(I1,I2,I3,n))); eLapMax = ParallelUtility::getMaxValue(eLapMax);
          	    real errMax = max(fabs(vLocal(I1,I2,I3)-(eLapLocal(I1,I2,I3,n)))); errMax = ParallelUtility::getMaxValue(errMax);
          	    error = errMax/(eLapMax);


          	    gridError=max(gridError,error);
                        checker.printMessage("laplacian",error,time);

                        if( c==1 )
          	    {
            	      v=-123456.;
            	      time=getCPU();
            	      op.derivative(MappedGridOperators::laplacianOperator,u,v,I1,I2,I3);
            	      time=getCPU()-time;

	      // error = max(fabs(v(I1,I2,I3)-(eLap(I1,I2,I3,n))))/max(fabs(eLap(I1,I2,I3,n)));

            	      real eLapMax = max(fabs(eLapLocal(I1,I2,I3,n))); eLapMax = ParallelUtility::getMaxValue(eLapMax);
            	      errMax = max(fabs(vLocal(I1,I2,I3)-(eLapLocal(I1,I2,I3,n)))); errMax = ParallelUtility::getMaxValue(errMax);
            	      error = errMax/(eLapMax);

            	      gridError=max(gridError,error);
            	      checker.printMessage("laplacian (opt)",error,time);
          	    }


          	    time=getCPU();
                        #ifndef USE_PPP
          	    v=u.divScalarGrad(scalar,I1,I2,I3); // broken in P++
                        #else
          	    op.derivative(MappedGridOperators::divergenceScalarGradient,u,scalar,v,I1,I2,I3);
                        #endif
          	    time=getCPU()-time;
// 	    error = max(fabs(v(I1,I2,I3)
// 			     -( eLap(I1,I2,I3,0)*scalar(I1,I2,I3)
// 				+scalar.x(I1,I2,I3)(I1,I2,I3)*ex(I1,I2,I3,0)
// 				+scalar.y(I1,I2,I3)(I1,I2,I3)*ey(I1,I2,I3,0)
// 				+scalar.z(I1,I2,I3)(I1,I2,I3)*ez(I1,I2,I3,0) )))/
// 	      (max(fabs(eLap(I1,I2,I3,0)))+1.);

          	    realSerialArray scalarxLocal(I1,I2,I3),scalaryLocal(I1,I2,I3),scalarzLocal(I1,I2,I3);
          	    op.useConservativeApproximations(false); // turn off temporarily so we match old way
          	    op.derivative(MappedGridOperators::xDerivative, scalarLocal,scalarxLocal,I1,I2,I3,0);
          	    op.derivative(MappedGridOperators::yDerivative, scalarLocal,scalaryLocal,I1,I2,I3,0);
          	    op.derivative(MappedGridOperators::zDerivative, scalarLocal,scalarzLocal,I1,I2,I3,0);
          	    op.useConservativeApproximations(conservative); // reset 

                        eLapMax = max(fabs(eLapLocal(I1,I2,I3,0))); eLapMax = ParallelUtility::getMaxValue(eLapMax);
          	    errMax = max(fabs(vLocal(I1,I2,I3)
                        			      -( eLapLocal(I1,I2,I3,0)*scalarLocal(I1,I2,I3)
                         				 +scalarxLocal(I1,I2,I3)*exLocal(I1,I2,I3,0)
                         				 +scalaryLocal(I1,I2,I3)*eyLocal(I1,I2,I3,0)
                         				 +scalarzLocal(I1,I2,I3)*ezLocal(I1,I2,I3,0) )));
          	    errMax = ParallelUtility::getMaxValue(errMax);
          	    error = errMax/(eLapMax+1.);
          	    

          	    gridError=max(gridError,error);
          	    if( FALSE )
          	    {
            	      display(u.divScalarGrad(scalar,I1,I2,I3)(I1,I2,I3),"u.divScalarGrad");
          	    }
                        checker.printMessage("divSGrad",error,time);

          	    v=-123456.;
          	    time=getCPU();
          	    op.derivative(MappedGridOperators::divergenceScalarGradient,u,scalar,v,I1,I2,I3);
          	    time=getCPU()-time;
// 	    error = max(fabs(v(I1,I2,I3)
// 			     -( eLap(I1,I2,I3,0)*scalar(I1,I2,I3)
// 				+scalar.x(I1,I2,I3)(I1,I2,I3)*ex(I1,I2,I3,0)
// 				+scalar.y(I1,I2,I3)(I1,I2,I3)*ey(I1,I2,I3,0)
// 				+scalar.z(I1,I2,I3)(I1,I2,I3)*ez(I1,I2,I3,0) )))/
// 	      (max(fabs(eLap(I1,I2,I3,0)))+1.);

                        eLapMax = max(fabs(eLapLocal(I1,I2,I3,0))); eLapMax = ParallelUtility::getMaxValue(eLapMax);
          	    errMax = max(fabs(vLocal(I1,I2,I3)
                        			      -( eLapLocal(I1,I2,I3,0)*scalarLocal(I1,I2,I3)
                         				 +scalarxLocal(I1,I2,I3)*exLocal(I1,I2,I3,0)
                         				 +scalaryLocal(I1,I2,I3)*eyLocal(I1,I2,I3,0)
                         				 +scalarzLocal(I1,I2,I3)*ezLocal(I1,I2,I3,0) )));
          	    errMax = ParallelUtility::getMaxValue(errMax);
          	    error = errMax/(eLapMax+1.);


          	    gridError=max(gridError,error);
          	    checker.printMessage("divSGrad (opt)",error,time);
          	    
        	  }
      	}

                for( int m=0; m<cg.numberOfDimensions(); m++ )
      	{
                    exact.gd(ed,0,1,0,0,I1,I2,I3,m); // ex(m)

                    #ifndef USE_PPP
                  	    error = max(fabs(w.x(I1,I2,I3,m)(I1,I2,I3)-ed(I1,I2,I3)))/max(fabs(ed(I1,I2,I3)));
                    #else
                        op.derivative(MappedGridOperators::xDerivative,w,v,I1,I2,I3,m);
                        real edMax = max(fabs(edLocal(I1,I2,I3)));      edMax = ParallelUtility::getMaxValue(edMax);
          	    real errx = max(fabs(vLocal(I1,I2,I3)-edLocal(I1,I2,I3)));  errx = ParallelUtility::getMaxValue(errx);
          	    error = errx/(1.+edMax);
                    #endif

                    gridError=max(gridError,error);
        	  cout << "w.x(all," << m << ") : Maximum relative error (order=" << order << ") = " << error << endl;
      	}

        // ERROR here in function xi: unable to take a derivative of a matrix grid function in parallel.
        // The problem is that this requires a reshape at the end which doesn't seem to work.

                #ifndef USE_PPP
        	  error = max(fabs(tensor.x(I1,I2,I3,0,0)(I1,I2,I3)-ex(I1,I2,I3,0)))/max(fabs(ex(I1,I2,I3,0)));
                    gridError=max(gridError,error);
        	  cout << "T(0,0).x   : Maximum relative error (order=" << order << ") = " << error << endl;

                    exact.gd(ed,0,1,0,0,I1,I2,I3,1); // ex(1)
            	  error = max(fabs(tensor.x(I1,I2,I3,1,0)(I1,I2,I3)-ed(I1,I2,I3)))/max(fabs(ed(I1,I2,I3)));
                    gridError=max(gridError,error);
        	  cout << "T(1,0).x   : Maximum relative error (order=" << order << ") = " << error << endl;

                
                    exact.gd(ed,0,1,0,0,I1,I2,I3,2); // ex(2)
          // edd(I1,I2,I3)=tensor.x(I1,I2,I3,0,1)(I1,I2,I3);
        	  edd=tensor.x(I1,I2,I3,0,1); 
        	  if( debug & 4  )
        	  {
          	    ::display(tensor(I1,I2,I3,0,1),"tensor(I1,I2,I3,0,1)","%5.2f ");
          	    tensor.x(I1,I2,I3,0,1).display("tensor.x(I1,I2,I3,0,1).display");
          	    ::display(tensor.x(I1,I2,I3,0,1),"tensor.x(I1,I2,I3,0,1)","%5.2f ");
          	    ::display(edd(I1,I2,I3),"edd=tensor.x","%5.2f ");
          	    ::display(ed(I1,I2,I3),"exact.x","%5.2f ");
          	    ::display(fabs(ed(I1,I2,I3)-edd(I1,I2,I3)),"error","%5.2f ");
        	  }
        	  
        	  error = max(fabs(tensor.x(I1,I2,I3,0,1)(I1,I2,I3)-ed(I1,I2,I3)))/max(fabs(ed(I1,I2,I3)));
        	  gridError=max(gridError,error);
        	  cout << "T(0,1).x   : Maximum relative error (order=" << order << ") = " << error << endl;
      	
        	  exact.gd(ed,0,1,0,0,I1,I2,I3,3); // ex(3)
        	  error = max(fabs(tensor.x(I1,I2,I3,1,1)(I1,I2,I3)-ed(I1,I2,I3)))/max(fabs(ed(I1,I2,I3)));
        	  gridError=max(gridError,error);
        	  cout << "T(1,1).x   : Maximum relative error (order=" << order << ") = " << error << endl;

                #endif
      	printf("+++++++++++ Worst relative error is %e for order of accuracy %i on grid %s ++++++++++++++\n\n",
             	       gridError,order,(const char*)mg.mapping().getName(Mapping::mappingName));
      	worstError=max(worstError,gridError);

                printF(" order=%i grid=%i worstError=%8.2e\n",order,grid,worstError);
      	
            } // end for order
        }  

    // --- Now test the derivatives for CompositeGridFunctions: ---
        #ifndef USE_PPP
            bool testCompositeGridFunctionOperators=true;
        #else    
            bool testCompositeGridFunctionOperators=false; // for now, skip these in parallel
        #endif
        if( testCompositeGridFunctionOperators )
        {
            cout <<  " \n ***** Now test CompositeGridFunction Derivatives ***** \n";

            realCompositeGridFunction q(cg),q1,q2,q3(cg,all,all,all,cg.numberOfDimensions());
            CompositeGridOperators cgo(cg);
            q.setOperators(cgo);
            q3.setOperators(cgo);
            if( debug & 16 )
            {
      	Interpolant interpolant( cg );

      	for( grid=0; grid<cg.numberOfGrids(); grid++)
      	{
        	  q[grid]=grid+1.;
      	}
      	q1=q+2*q;
      	if( debug & 16 )
        	  q.display("Here is q before interpolate");
      	q.interpolate();
      	if( debug & 16 )
        	  q.display("Here is q after interpolate");
      	if( debug & 16 )
        	  q1.display("Here is q1=q+2*q before interpolate");
      	q1.interpolate();
      	if( debug & 16 )
        	  q1.display("Here is q1 after interpolate");

      	intCompositeGridFunction compare(cg);
      	q1=1.;
      	q2=q1+1.;
      	compare = q1 < q2;
      	if( max(abs(compare-1)) != 0 )
      	{
        	  cout << "**** error  in compare < \n";
        	  compare.display("Here is q1 < q2");
      	}

      	compare = q1 >= q2;
      	if( max(abs(compare)) != 0 )
      	{
        	  cout << "**** error  in compare >= \n";
        	  compare.display("Here is q1 >= q2");
      	}
    

      	q1=2.;
      	q2=1.-q1;
      	real err1=max(fabs(q2+1.));
      	if( err1 != 0. )
        	  printf("Error: q1=2.; q2=1.-q1 : q2 is not equal to -1!, max(abs(q2+1))=%e \n",err1);
        

      	q=1.;
            }
            exact.assignGridFunction(q);
            exact.assignGridFunction(q3);

            if( debug & 4 )
            {
      	cout << "testing derivatives in pass by value\n";
      	passByValue(q[0],q);
      	q1=q+5.*q;
      	cout << "testing derivatives in pass by value of q1=q+5.*q \n";
      	passByValue(q1[0],q1);
            }


            if( debug & 4 )
      	q.display("Here is q for testing derivatives");  

            realCompositeGridFunction scalar(cg);
            scalar.setOperators(cgo);
      // for harmonic averaging we do not want a negative scalar
            for( grid=0; grid<cg.numberOfGrids(); grid++ )
            {
      	realMappedGridFunction xy(cg[grid]); 
      	xy = cg[grid].vertex()(all,all,all,0)+cg[grid].vertex()(all,all,all,1);
      	real xyMax=max(fabs(xy));
      	scalar[grid]=1.+xy*(.5/xyMax);
	// scalar[grid]=1.;
            }
        
      // ---- compute all derivatives to 2nd and 4th order accuracy ----
            for( int order=2; order<=4; order+=2 )
            {
      	q.getOperators()->setOrderOfAccuracy(order);
      	q3.getOperators()->setOrderOfAccuracy(order);

#ifndef USE_PPP
                q2.link(q,Range(0,0));
#else
                q2.link(q,Range(0,0));
#endif

      	q1=q2.x();
//    error = max(fabs(q1-exact.x(cg)));
      	error=0.;
      	for( grid=0; grid<cg.numberOfGrids(); grid++ )
      	{
        	  getIndex(cg[grid].dimension(),I1,I2,I3,-(order/2));  // reduce size for 2nd or 4th order
        	  realMappedGridFunction ex(cg[grid]); 
        	  exact.gd(ex,0,1,0,0,I1,I2,I3,n);
        	  error=max(error,max(fabs(q1[grid](I1,I2,I3,n)-ex(I1,I2,I3,n)))/max(abs(ex(I1,I2,I3,n))));
      	}      
      	worstError=max(worstError,error);
      	cout << "q.x        : Maximum relative error (order=" << order << ") = " << error << endl;

      	q1=q2.y();
      	error=0.;
      	for( grid=0; grid<cg.numberOfGrids(); grid++ )
      	{
        	  getIndex(cg[grid].dimension(),I1,I2,I3,-(order/2));  // reduce size for 2nd or 4th order
        	  realMappedGridFunction ey(cg[grid]); 
        	  exact.gd(ey,0,0,1,0,I1,I2,I3,n);
        	  error=max(error,max(fabs(q1[grid](I1,I2,I3,n)-ey(I1,I2,I3,n)))/max(abs(ey(I1,I2,I3,n))));
      	}      
      	worstError=max(worstError,error);
      	cout << "q.y        : Maximum relative error (order=" << order << ") = " << error << endl;

      	q1=q.xx();
      	error=0.;
      	for( grid=0; grid<cg.numberOfGrids(); grid++ )
      	{
        	  getIndex(cg[grid].dimension(),I1,I2,I3,-(order/2));  // reduce size for 2nd or 4th order
        	  realMappedGridFunction exx(cg[grid]); 
        	  exact.gd(exx,0,2,0,0,I1,I2,I3,n);
        	  error=max(error,max(fabs(q1[grid](I1,I2,I3,n)-exx(I1,I2,I3,n)))/max(abs(exx(I1,I2,I3,n))));
      	}      
      	worstError=max(worstError,error);
      	cout << "q.xx       : Maximum relative error (order=" << order << ") = " << error << endl;

      	q1=q.xy();
      	error=0.;
      	for( grid=0; grid<cg.numberOfGrids(); grid++ )
      	{
        	  getIndex(cg[grid].dimension(),I1,I2,I3,-(order/2));  // reduce size for 2nd or 4th order
        	  realMappedGridFunction exy(cg[grid]); 
        	  exact.gd(exy,0,1,1,0,I1,I2,I3,n);
        	  error=max(error,max(fabs(q1[grid](I1,I2,I3,n)-exy(I1,I2,I3,n)))/max(1.+abs(exy(I1,I2,I3,n))));
      	}      
      	worstError=max(worstError,error);
      	cout << "q.xy       : Maximum relative error (order=" << order << ") = " << error << endl;

      	q1=q.yy();
      	error=0.;
      	for( grid=0; grid<cg.numberOfGrids(); grid++ )
      	{
        	  getIndex(cg[grid].dimension(),I1,I2,I3,-(order/2));  // reduce size for 2nd or 4th order
        	  realMappedGridFunction eyy(cg[grid]); 
        	  exact.gd(eyy,0,0,2,0,I1,I2,I3,n);
        	  error=max(error,max(fabs(q1[grid](I1,I2,I3,n)-eyy(I1,I2,I3,n)))/max(1.+abs(eyy(I1,I2,I3,n))));
      	}      
      	worstError=max(worstError,error);
      	cout << "q.yy       : Maximum relative error (order=" << order << ") = " << error << endl;

      	for( int c=0; c<=1; c++ )
      	{
        	  cgo.useConservativeApproximations(c==0);
        	  if( c==0 )
          	    printf("**Using conservative approximations:\n");
        	  else
          	    printf("**Using standard approximations:\n");
	  // ---------- divScalarGrad --------------
        	  for( int a=0; a<=1; a++ )
        	  {
          	    if( a==1 )
          	    {
            	      cgo.setAveragingType(MappedGridOperators::arithmeticAverage);
            	      printf("  --Using arithmetic average\n");
          	    }
          	    else
          	    {
            	      cgo.setAveragingType(MappedGridOperators::harmonicAverage);
            	      printf("  --Using harmonic average\n");
          	    }

#ifndef USE_PPP
          	    q1=q.divScalarGrad(scalar); // this is broken for P++
#else
          	    for( grid=0; grid<cg.numberOfGrids(); grid++ )
          	    {
                            getIndex(cg[grid].dimension(),I1,I2,I3,-(order/2));
            	      cgo[grid].derivative(MappedGridOperators::divergenceScalarGradient,
                                                                      q[grid],scalar[grid],q1[grid],I1,I2,I3);
          	    }
#endif
          	    error=0.;
          	    for( grid=0; grid<cg.numberOfGrids(); grid++ )
          	    {
            	      MappedGrid & mg = cg[grid];
            	      getIndex(mg.dimension(),I1,I2,I3,-(order/2));  // reduce size for 2nd or 4th order

            	      const intArray & mask = mg.mask();
            	      OV_GET_SERIAL_ARRAY_CONST(int,mask,maskLocal);
            	      OV_GET_SERIAL_ARRAY(real,mg.center(),xLocal);
                        
            	      bool ok=ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,1);
            	      int isRectangular=0;
            // 	      computeDerivativesOfExactSolution(mg,2)
                          realMappedGridFunction ex(mg),ey(mg),ez(mg); // to hold derivatives of the exact solution
                          OV_GET_SERIAL_ARRAY(real,ex,exLocal);
                          OV_GET_SERIAL_ARRAY(real,ey,eyLocal);
                          OV_GET_SERIAL_ARRAY(real,ez,ezLocal);
                          if( ok )
                          {
                              exact.gd(exLocal,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,n);
                              exact.gd(eyLocal,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,n);
                              if( mg.numberOfDimensions()==3 )
                                  exact.gd(ezLocal,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,n);
                          }
             //              #If "2" == "2"
                            realMappedGridFunction exx(mg),exy(mg),exz(mg),eyy(mg),eyz(mg),ezz(mg),eLap(mg);
                            OV_GET_SERIAL_ARRAY(real,exx,exxLocal);
                            OV_GET_SERIAL_ARRAY(real,exy,exyLocal);
                            OV_GET_SERIAL_ARRAY(real,exz,exzLocal);
                            OV_GET_SERIAL_ARRAY(real,eyy,eyyLocal);
                            OV_GET_SERIAL_ARRAY(real,eyz,eyzLocal);
                            OV_GET_SERIAL_ARRAY(real,ezz,ezzLocal);
                            OV_GET_SERIAL_ARRAY(real,eLap,eLapLocal);
                            if( ok )
                            {
                                exact.gd(exxLocal,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,n);
                                exact.gd(exyLocal,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,n);
                                exact.gd(eyyLocal,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,n);
                                if( mg.numberOfDimensions()==2 )
                                {
                                    eLapLocal=exxLocal+eyyLocal;
                                }
                                else
                                {
                                    exact.gd(exzLocal,xLocal,numberOfDimensions,isRectangular,0,1,0,1,I1,I2,I3,n);
                                    exact.gd(eyzLocal,xLocal,numberOfDimensions,isRectangular,0,0,1,1,I1,I2,I3,n);
                                    exact.gd(ezzLocal,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,n);
                                    eLapLocal=exxLocal+eyyLocal+ezzLocal;
                                }
                            }

            		if( cg.numberOfDimensions()==2 )
            		{
              		  error = max(fabs(q1[grid](I1,I2,I3)
                           				   -( (exx(I1,I2,I3,0)+eyy(I1,I2,I3,0))*scalar[grid](I1,I2,I3)
                              				      +scalar[grid].x(I1,I2,I3)(I1,I2,I3)*ex(I1,I2,I3,0)
                              				      +scalar[grid].y(I1,I2,I3)(I1,I2,I3)*ey(I1,I2,I3,0) )))/
                		    (max(fabs(exx(I1,I2,I3,0)+eyy(I1,I2,I3,0)))+1.);

              		  if( debug & 4 )
              		  {
                		    realArray t = fabs(q1[grid](I1,I2,I3)
                               				       -( (exx(I1,I2,I3,0)+eyy(I1,I2,I3,0))*scalar[grid](I1,I2,I3)
                                					  +scalar[grid].x(I1,I2,I3)(I1,I2,I3)*ex(I1,I2,I3,0)
                                					  +scalar[grid].y(I1,I2,I3)(I1,I2,I3)*ey(I1,I2,I3,0) ));
                		    display(t,"error in u.divScalarGrad");
              		  }
            	      
            		}
            		else
            		{
              		  error = max(fabs(q1[grid](I1,I2,I3)
                           				   -( (exx(I1,I2,I3,0)+eyy(I1,I2,I3,0)+ezz(I1,I2,I3,0))*scalar[grid](I1,I2,I3)
                              				      +scalar[grid].x(I1,I2,I3)(I1,I2,I3)*ex(I1,I2,I3,0)
                              				      +scalar[grid].y(I1,I2,I3)(I1,I2,I3)*ey(I1,I2,I3,0)
                              				      +scalar[grid].z(I1,I2,I3)(I1,I2,I3)*ez(I1,I2,I3,0) )))/
                		    (max(fabs(exx(I1,I2,I3,0)+eyy(I1,I2,I3,0)+ezz(I1,I2,I3,0)))+1.);
            		}
          	    }
          	    worstError=max(worstError,error);
          	    cout << "    u.divSGrad : Maximum relative error (order=" << order << ") = " << error << endl;

	    // ---------- scalarGrad --------------
#ifndef USE_PPP
          	    q3=q.scalarGrad(scalar); // broken for P++
          	    error=0.;
          	    for( grid=0; grid<cg.numberOfGrids(); grid++ )
          	    {
            	      MappedGrid & mg = cg[grid];

            	      const intArray & mask = mg.mask();
            	      OV_GET_SERIAL_ARRAY_CONST(int,mask,maskLocal);
            	      OV_GET_SERIAL_ARRAY(real,mg.center(),xLocal);

            	      bool ok=ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,1);
            	      int isRectangular=0;
            // 	      computeDerivativesOfExactSolution(mg,1);
                          realMappedGridFunction ex(mg),ey(mg),ez(mg); // to hold derivatives of the exact solution
                          OV_GET_SERIAL_ARRAY(real,ex,exLocal);
                          OV_GET_SERIAL_ARRAY(real,ey,eyLocal);
                          OV_GET_SERIAL_ARRAY(real,ez,ezLocal);
                          if( ok )
                          {
                              exact.gd(exLocal,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,n);
                              exact.gd(eyLocal,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,n);
                              if( mg.numberOfDimensions()==3 )
                                  exact.gd(ezLocal,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,n);
                          }
             //              #If "1" == "2"
          	    
            	      getIndex(mg.dimension(),I1,I2,I3,-(order/2));  // reduce size for 2nd or 4th order
            	      error = max(fabs(q3[grid](I1,I2,I3,0)-scalar[grid](I1,I2,I3)*ex(I1,I2,I3,0) )/
                    			  (max(fabs(scalar[grid](I1,I2,I3)*ex(I1,I2,I3,0)))+1.));
            	      if( cg.numberOfDimensions()>1 )
            	      {
            		error = max(error,max(fabs(q3[grid](I1,I2,I3,1)-scalar[grid](I1,I2,I3)*ey(I1,I2,I3,0))/
                              				      (max(fabs(scalar[grid](I1,I2,I3)*ey(I1,I2,I3,0)))+1.)));
            	      }
            	      else if( cg.numberOfDimensions()>2 )
            	      {
            		error = max(error,max(fabs(q3[grid](I1,I2,I3,2)-scalar[grid](I1,I2,I3)*ez(I1,I2,I3,0))/
                              				      (max(fabs(scalar[grid](I1,I2,I3)*ez(I1,I2,I3,0)))+1.)));
            	      }
          	    }
          	    worstError=max(worstError,error);
          	    cout << "    u.scalarGrad:Maximum relative error (order=" << order << ") = " << error << endl;
#endif
        	  }
      	}
            
      	q1.destroy();
      	q1=q.grad();
      	q1.setOperators(cgo);
      	if( debug & 4 )
        	  q1.display("Here is q1=q.grad()");
      	error=0.;
      	for( grid=0; grid<cg.numberOfGrids(); grid++ )
      	{
        	  getIndex(cg[grid].dimension(),I1,I2,I3,-(order/2));  // reduce size for 2nd or 4th order

        	  MappedGrid & mg = cg[grid];
        	  const intArray & mask = mg.mask();
        	  OV_GET_SERIAL_ARRAY_CONST(int,mask,maskLocal);
        	  OV_GET_SERIAL_ARRAY(real,mg.center(),xLocal);

        	  bool ok=ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,1);
        	  int isRectangular=0;

        // 	  computeDerivativesOfExactSolution(cg[grid],1);
                  realMappedGridFunction ex(cg[grid]),ey(cg[grid]),ez(cg[grid]); // to hold derivatives of the exact solution
                  OV_GET_SERIAL_ARRAY(real,ex,exLocal);
                  OV_GET_SERIAL_ARRAY(real,ey,eyLocal);
                  OV_GET_SERIAL_ARRAY(real,ez,ezLocal);
                  if( ok )
                  {
                      exact.gd(exLocal,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,n);
                      exact.gd(eyLocal,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,n);
                      if( cg[grid].numberOfDimensions()==3 )
                          exact.gd(ezLocal,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,n);
                  }
         //          #If "1" == "2"

        	  error=max(error,max(fabs(q1[grid](I1,I2,I3,0)-ex(I1,I2,I3)))/max(abs(ex(I1,I2,I3,n))));
        	  error=max(error,max(fabs(q1[grid](I1,I2,I3,1)-ey(I1,I2,I3)))/max(abs(ey(I1,I2,I3,n))));
        	  if( cg.numberOfDimensions()==3 )
          	    error=max(error,max(fabs(q1[grid](I1,I2,I3,2)-ez(I1,I2,I3)))/max(1.+abs(ez(I1,I2,I3,n))));
      	}      
      	worstError=max(worstError,error);
      	cout << "q.grad     : Maximum relative error (order=" << order << ") = " << error << endl;
    
      	realCompositeGridFunction q5(cg,all,all,all,cg.numberOfDimensions()==2 ? 1 :cg.numberOfDimensions());
      	q5=q1.vorticity();

      	q5.updateToMatchGrid(cg,all,all,all,1);
      	q5=q1.div();
      	q1.updateToMatchGrid(cg,all,all,all,1);
            }
            
        }  // end if testCompositeGridFunctionOperators
        

    }  // end loop over grids
    
    printf("\n\n ************************************************************************************************\n");
    if( worstError > .025 )
        printf(" ************** Warning, there is a large error somewhere, worst error =%e ******************\n",
         	   worstError);
    else
        printf(" ************** Test apparently successful, worst error =%e ******************\n",worstError);
    printf(" **************************************************************************************************\n\n");


/* -----


  // ***** can't use ux ****


  // --- make a list of derivatives to evaluate all at once (this is more efficient) ---
    RealArray ux,uy;                           // these arrays will hold the answers
    u.op->setNumberOfDerivativesToEvaluate( 2 );
    u.op->setDerivativeType( 0, MappedGridOperators::xDerivative, ux );
    u.op->setDerivativeType( 1, MappedGridOperators::yDerivative, uy );
    u.op->setOrderOfAccuracy(2);

    u.getDerivatives(I1,I2,I3);               // evaluate all the derivatives, answer in ux and uy

    error = max(fabs(ux(I1,I2,I3)-cos(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2))));
    cout << "Maximum error in ux: (2nd order) = " << error << endl;
    error = max(fabs(uy(I1,I2,I3)+sin(mg.vertex()(I1,I2,I3,axis1))*sin(mg.vertex()(I1,I2,I3,axis2))));
    cout << "Maximum error in uy: (2nd order) = " << error << endl;

---- */

    
    Overture::finish();          
    cout << "Program Terminated Normally! \n";
    return 0;
}
