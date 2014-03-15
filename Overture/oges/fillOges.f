      subroutine fillOges(ndra,ndrb,ndsa,ndsb,ndta,ndtb, nds,ndc, coeff,
     &     neq, grid,peqn, classify,unused, rightNullVector,

      real coeff(nds,ndc,ndra:ndrb,ndsa:ndsb,ndta:ndtb)
      integer classify(ndra:ndrb,ndsa:ndsb,ndta:ndtb,ndc)
c begin statement functions
      equationNo(n,i1,i2,i3,grid)=
     &  n+1+   numberOfComponents*(i1-ndra+
     &              ndr*(i2-ndsa+
     &               nds*(i3-ndta))) + peqn(grid+1)

      ndr=ndrb-ndra+1
      nds=ndsb-ndsa+1
      ndt=ndtb-ndta+1

      do i3=ndta,ndtb
        do i2=ndsa,ndsb
          do i1=ndra,ndrb
c           ....load the matrix into (ia,ja,a) (Throw away small elements)
            do n=0,nc
	      ieqn=equationNo(n,i1,i2,i3,grid)
              if( ieqn .le. .or. ieqn .gt. neq )then
                write(*,*) 'Oges:generate: ieqn out of range, ieqn=',ieqn
              end if

              if( isparse.eq.0 ) ia(ieqn)=ii+1


              if( classify(i1,i2,i3,n).eq.unused )then
c               null equation, set to the identity
	        ii++;
	        if( ii > Oges::ndja )then
                  cout << "Oges::generateMatrix: ...not enough space to store matrix" << endl;
c                 generateMatrixError( Oges::nda,ieqn );
                  return
	        end if
	        if( isparse.eq.1 ) ia(ii)=ieqn;
	        ja(ii)=ieqn;
	        a(ii)=1.;
              else
c	        compute a scale factor; used to throw away small values
	        scale=max(fabs(COEFF(S,n,i1,i2,i3)))*2.*matrixCutoff;
	        if( scale.eq.0. )then
                  if( classify(i1,i2,i3,n) .lt. 10 )then
    		    cout << "Oges:generateMatrix:ERROR matrix has a row with all zero coefficients \n";
                    printf("The offending equation is (i1,i2,i3,n,grid)=(%i,%i,%i,%i,%i)\n",i1,i2,i3,n,grid);
                    printf("...classify(i1,i2,i3,n)=%i\n",classifyX(i1,i2,i3,n));
		    throw "Oges:ERROR";
		  else
                    break;
		  end if
                end if

  	        do i=1,nds
c		  printf("i1=%i,I2=%i,i3=%i, ieqn=%i, coeff=%e \n",i1,i2,i3,ieqn,COEFF(i,n,i1,i2,i3));
		  if( abs(COEFF(i,n,i1,i2,i3)) .gt. scale )then
  		    jeqn=EQUATIONNUMBER(i,n,i1,i2,i3);
		    if( jeqn <= 0 || jeqn > numberOfEquations )
		      cout << "generateMatrix: jeqn out of range, jeqn=" << jeqn 
		        << " <0 or > numberOfEquations=" << numberOfEquations<< endl;
		      printf(" i1=%i, i2=%i, i3=%i, grid=%i, i=%i, classify=%i "
		  	     " stencilLength=%i, coeff=%e \n", i1,i2,i3,grid,i,
			     classifyX(i1,i2,i3),stencilLength,COEFF(i,n,i1,i2,i3) );
		    end if
		    ii++;
		    if( ii > Oges::ndja )then
		      cout << "Oges::generateMatrix: ...not enough space to store matrix" << endl;
		      generateMatrixError( Oges::nda,ieqn );
		      return;
		    end if
		    if( isparse==1 ) ia(ii)=ieqn;
		    ja(ii)=jeqn;
		    a(ii)=COEFF(i,n,i1,i2,i3);
                  end if
                end do
                if( isACoefficientMatrix && compatibilityConstraint )
c	          // add compatibility constraint
                  value = rightNullVector(i1,i2,i3);
                  if( value != 0. )then
		    ii++;
		    if( ii > Oges::ndja )then
		      cout << "Oges::generateMatrix: ...not enough space to store matrix" << endl;
		      generateMatrixError( Oges::nda,ieqn );
		      return;
		    end if
		    if( isparse==1 ) ia(ii)=ieqn;
		    ja(ii)=extraEquationNumber(0);
		    a(ii)=value;
		  end if
                end if


	  
          // Add in "dense" extra equations such as those equations that define
          // an "integral" type constraint (e.g. setting the mean pressure to zero)
          // *** Fix this -- only works for one dense equation ****
          if( numberOfExtraEquations > 0 
	      && equationNo(0,i1,i2,i3,grid)==extraEquationNumber(0)
              && addDenseExtraEquations != NULL )
          {
	      
            if( debug & 2 )
              cout << "generate: adding denseExtraEquations..." << endl;
            real cdc;
            scale=matrixCutoff;  // *******
            int gridc;	      
            for( gridc=0; gridc < (*coefficientsOfDenseExtraEquations).numberOfComponentGrids(); gridc++ )
	    {
              RealDistributedArray & c = (*coefficientsOfDenseExtraEquations)[gridc];
              int base4=c.getBase(axis3+1);
              // **** should the nc loop go outside or inside
              for( int nc=c.getBase(axis3+1); nc<=c.getBound(axis3+1); nc++ )
	      {
    	        for( int i3c=c.getBase(axis3); i3c<=c.getBound(axis3); i3c++ )
		{
		  for( int i2c=c.getBase(axis2); i2c<=c.getBound(axis2); i2c++ )
	  	  {
		    for( int i1c=c.getBase(axis1); i1c<=c.getBound(axis1); i1c++ )
		    {
		      cdc=c(i1c,i2c,i3c,nc);
		      if( fabs(cdc) > scale )
		      {
			jeqn=equationNo(nc-base4,i1c,i2c,i3c,gridc); 
			if( jeqn < 0 || jeqn > numberOfEquations )
			  cout << "generate:2 jeqn out of range, jeqn=" << jeqn << endl;
			ii++;
                        if( ii > Oges::ndja )
			{
		          cout << "Oges::generateMatrix: ...not enough space to store matrix" << endl;
                          generateMatrixError( Oges::nda,ieqn );
	         	  return;
		        }
                        if( isparse==1 ) ia(ii)=ieqn;
			ja(ii)=jeqn;
			a(ii)=cdc;
		      }
		    }
		  }
		}
	      }
	    }
	    addDenseExtraEquations = NULL;
	  } // end if numberOfExtra
	}
      }
    }
    cpuFill+=getCPU()-cpu1;
  }
