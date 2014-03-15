
//#define BOUNDS_CHECK
//#define OV_DEBUG

#include "Maxwell.h"
#include "display.h"
#include "MappedGridOperators.h"
#include "UnstructuredMapping.h"
#include "UnstructuredOperators.h"
#include "ULink.h"

#include "PlotIt.h"

enum AvgType {
  simple,
  leastSq
};

namespace {
  void unstructuredDissipation( int order, UnstructuredMapping::EntityTypeEnum centering, 
				realMappedGridFunction &field_gf, realMappedGridFunction &disp_gf,
				ArraySimple<ArraySimple<UnstructuredMappingAdjacencyIterator> >&ulinks);

  void symmetricSmoother( UnstructuredMapping::EntityTypeEnum centering, 
			  realMappedGridFunction &, realMappedGridFunction &,
			  realArray &,ArraySimple<ArraySimple<int> > &indices);

  void applyDissipationToField(MappedGrid &mg, UnstructuredMapping::EntityTypeEnum centering,
			       realMappedGridFunction &field, realMappedGridFunction &fieldn,
			       ArraySimple<real> & Ccoeff_1,ArraySimple<int> & Coffset_1, ArraySimple<int> & Cindex_1,
			       ArraySimple<real> & Ccoeff_2,ArraySimple<int> & Coffset_2, ArraySimple<int> & Cindex_2,
			       realMappedGridFunction &disp, ArraySimple<real> &dispCoeff, realArray &areaNormals,
			       real artificialDissipation, int artificialDissipationInterval, int orderOfArtificialDissipation,
			       real t, real dt, Maxwell &mx, bool recompute=true );

  enum DType {
    curlcurl,
    AtransA,
    AAtrans,
    simpleSmoother,
    smoothProjection
  };

  bool dampE = true;
  bool dampH = true;
  
  DType dtype = simpleSmoother;
}

#define dsimv EXTERN_C_NAME(dsimv)
#define dsimv2 EXTERN_C_NAME(dsimv2)

extern "C" void dsimv(real*,int*,int*,real*,real*,real*,int*,int*,real*);
extern "C" void dsimv2(int *,real*,int*,int*,real*,real*,int*,int*,real*);

void Maxwell::
advanceUnstructuredDSIMV( int current, real t, real dt, realMappedGridFunction *field )
{
  // XXX currently will not work for composite grids since the field array points to MappedGridFunctions
  //       and the CSR arrays arrays are for the whole CG

  // XXX the MV version only keeps track of eDotN in the E gridfunctions

  if( debug ) printF(" inside advanceUnstructuredDSIMV\n");

  const int next = (current+1) %2;

  realArray & hField  = field[current];
  realArray & hFieldn = field[next];
  realArray & eField  = field[current+2];
  realArray & eFieldn = field[next+2];

  realMappedGridFunction & hField_gf  = field[current];
  realMappedGridFunction & hFieldn_gf = field[next];
  realMappedGridFunction & eField_gf  = field[current+2];
  realMappedGridFunction & eFieldn_gf = field[next+2];


  CompositeGrid & cg  = *cgp;

//    eField.display("DSIMV eField");
//    hField.display("DSIMV hField");

  int nE = Eoffset.size(0)-1, nd = cg.numberOfDimensions();

  dsimv(&dt,&nE, &nd,
	eField.Array_Descriptor.Array_View_Pointer1,hField.Array_Descriptor.Array_View_Pointer1,Ecoeff.ptr(),
	Eoffset.ptr(),Eindex.ptr(),
	eFieldn.Array_Descriptor.Array_View_Pointer1);
// 	eField.getDataPointer(),hField.getDataPointer(),Ecoeff.ptr(),
// 	Eoffset.ptr(),Eindex.ptr(),
// 	eFieldn.getDataPointer());
  
  if ( dampE && dissipation && (numberOfStepsTaken%artificialDissipationInterval==0) )
    {
      real tm0 = getCPU();
      //      realMappedGridFunction eDisp;
      //      eDisp.updateToMatchGrid(cg[0],GridFunctionParameters::edgeCentered);
      applyDissipationToField(cg[0],UnstructuredMapping::Edge,
			      eField_gf,eFieldn_gf,
			      Ecoeff,Eoffset,Eindex,
			      Hcoeff,Hoffset,Hindex,
			      /*eDisp*/ *e_dissipation,E_dispCoeff,edgeAreaNormals,
			      artificialDissipation,artificialDissipationInterval,orderOfArtificialDissipation,
			      t,dt,*this);

      timing(timeForDissipation) += getCPU()-tm0;
    }
  else if ( dampE && dissipation )
    {
      real tm0 = getCPU();
      //      realMappedGridFunction eDisp;
      //      eDisp.updateToMatchGrid(cg[0],GridFunctionParameters::edgeCentered);
      applyDissipationToField(cg[0],UnstructuredMapping::Edge,
			      eField_gf,eFieldn_gf,
			      Ecoeff,Eoffset,Eindex,
			      Hcoeff,Hoffset,Hindex,
			      /*eDisp*/ *e_dissipation,E_dispCoeff,edgeAreaNormals,
			      artificialDissipation,artificialDissipationInterval,orderOfArtificialDissipation,
			      t,dt,*this, false);

      timing(timeForDissipation) += getCPU()-tm0;
    }

  applyDSIForcing( field[next+2], t, dt, true );
  applyDSIBC(field[next+2],t+dt/2., true);

//   eFieldn.display("eDotN DSI-MV");


  int nH = Hoffset.size(0)-1;
  dsimv(&dt, &nH, &nd,
	hField.getDataPointer(), eFieldn.getDataPointer(), Hcoeff.ptr(),
	Hoffset.ptr(), Hindex.ptr(),
	hFieldn.getDataPointer());

  if ( dampH && dissipation && (numberOfStepsTaken%artificialDissipationInterval==0) )
    {
      real tm0 = getCPU();

      applyDissipationToField(cg[0],UnstructuredMapping::Face,
			      hField_gf,hFieldn_gf,
			      Hcoeff,Hoffset,Hindex,
			      Ecoeff,Eoffset,Eindex,
			      *dissipation,dispCoeff,faceAreaNormals,
			      artificialDissipation,artificialDissipationInterval,orderOfArtificialDissipation,
			      t,dt,*this);

      timing(timeForDissipation) += getCPU()-tm0;
    } 
  else if ( dampH && dissipation )
    {
      real tm0 = getCPU();
      
      applyDissipationToField(cg[0],UnstructuredMapping::Face,
			      hField_gf,hFieldn_gf,
			      Hcoeff,Hoffset,Hindex,
			      Ecoeff,Eoffset,Eindex,
			      *dissipation,dispCoeff,faceAreaNormals,
			      artificialDissipation,artificialDissipationInterval,orderOfArtificialDissipation,
			      t,dt,*this, false);

      timing(timeForDissipation) += getCPU()-tm0;
    }
//       real one=1;
//       realMappedGridFunction hft,hft_r,dtmp;

//       int ncol=0;

//       int sgn = 1;

//       if ( Dcoeff.size() )
// 	{
// 	  cout<<"USING Disp Operator!!"<<endl;
// 	  dsimv2(&ncol,&one, &nH, &nd,
// 		 hField.Array_Descriptor.Array_View_Pointer1, Dcoeff.ptr(),
// 		 Doffset.ptr(), Dindex.ptr(),
// 		 dissipation->getDataPointer());
// 	}
//       else
// 	{

// 	  hft.updateToMatchGrid(*dissipation->getMappedGrid(),GridFunctionParameters::edgeCentered);
// 	  //      realMappedGridFunction hft1;
// 	  hft=0.;
	  
// 	  switch(dtype) {
// 	  case curlcurl:
// 	  case AtransA:
// 	    dsimv2(&ncol,&one, &nE, &nd,
// 		   hField.Array_Descriptor.Array_View_Pointer1, Ecoeff.ptr(),
// 		   Eoffset.ptr(), Eindex.ptr(),
// 		   hft.Array_Descriptor.Array_View_Pointer1);
	  
// 	    applyDSIBC(hft,t,true,true,curlHBC); 
// 	  //                                      hft_r.updateToMatchGrid(cg[0],GridFunctionParameters::edgeCentered,nd);
// 	  //                                      hft_r=0;
// 	  //                                      reconstructDSIField(t,EField,hft,hft_r);
// 	  //       //hft.display("HFT");
// 	  //       			      			      	          hft_r.display("HFT_R");
// 	  //      cout<<"MAX HFT = "<<max(fabs(hft))<<endl;
// 	  //      cout<<"MIN HFT = "<<min(fabs(hft))<<endl;
// 	    break;
// 	  case AAtrans:
// 	    one=1;
// 	    ncol = nE;
// 	    dsimv2(&ncol,&one, &nH, &nd,
// 		   hField.Array_Descriptor.Array_View_Pointer1, Hcoeff.ptr(),
// 		   Hoffset.ptr(), Hindex.ptr(),
// 		   hft.Array_Descriptor.Array_View_Pointer1);
// 	    applyDSIBC(hft,t,true,true,curlHBC); 
// 	    break;
// 	  simpleSmoother:
// 	  default:
// 	    break;
// 	  }

	  
// 	  switch(dtype) {
// 	  case curlcurl:
// 	    one = -1;
// 	    ncol=0;
// 	    dsimv2(&ncol,&one, &nH, &nd,
// 		   hft.getDataPointer(), Hcoeff.ptr(),
// 		   Hoffset.ptr(), Hindex.ptr(),
// 		   dissipation->getDataPointer());
// 	    break;
// 	  case AtransA:
// 	    one = 1;
// 	    ncol = nH;
// 	    dsimv2(&ncol,&one, &nE, &nd,
// 		   hft.getDataPointer(), 
// 		   Ecoeff.ptr(), Eoffset.ptr(), Eindex.ptr(),
// 		   dissipation->getDataPointer());
// 	    break;
// 	  case AAtrans:
// 	    ncol = 0;
// 	    dsimv2(&ncol,&one, &nH, &nd,
// 		   hft.getDataPointer(), Hcoeff.ptr(),
// 		   Hoffset.ptr(), Hindex.ptr(),
// 		   dissipation->getDataPointer());
// 	    break;
// 	  case simpleSmoother:
// 	    {
// 	      hft_r.updateToMatchGrid(cg[0],
// 				      (nd==2 ? GridFunctionParameters::cellCentered : GridFunctionParameters::faceCenteredAll),
// 				      (nd==2 ? 1 : nd) );

// 	      reconstructDSIField(t,HField,field[current],hft_r);

// 	      if ( orderOfArtificialDissipation>2 )
// 		{
// 		  dtmp.updateToMatchGrid(cg[0],
// 					 (nd==2 ? GridFunctionParameters::cellCentered : GridFunctionParameters::faceCenteredAll),
// 					 (nd==2 ? 1 : nd) );
// 		  realArray emptyArray;
// 		  ArraySimple<ArraySimple<int> > &indices = centering==UnstructuredMapping::Edge ? REindex : RHindex;
		  
// 		  symmetricSmoother( UnstructuredMapping::Face, hft_r, dtmp, emptyArray, index );
// 		}
// 	      else
// 		symmetricSmoother( UnstructuredMapping::Face, hft_r, *dissipation, faceAreaNormals );

// 	      //	      applyDSIBC(*dissipation,t,false,true,zeroBC);
// 	    }
// 	  default:
// 	    break;
// 	  }
	  
// 	  //applyDSIBC(*dissipation,t,false,true,forceExtrap);
// 	  //      cout<<"MAX D.N = "<<max(fabs(*dissipation))<<endl;
// 	  //      cout<<"MIN D.N = "<<min(fabs(*dissipation))<<endl;

// 	  //      dissipation->display("DISSIPATION");
// 	  //      realArray hft;
// 	  //cout<<"MAX D.N (premult)= "<<max(fabs(*dissipation))<<endl;
// 	  //cout<<"MIN D.N (premult)= "<<min(fabs(*dissipation))<<endl;

// 	} // !Dcoeff.size()

//       one = -1;
//       //	  applyDSIBC(*dissipation,t,false,true,forceExtrap);
      
//       for ( int d=0; d<(orderOfArtificialDissipation/2-1); d++ )
// 	{
// 	  //	  cout<<"computing dissipation "<<d<<endl;
// 	  //	  applyDSIBC(*dissipation,t+dt/2.,false,true,true);
// 	  //	  cout<<"AFTER BC MAX D.N = "<<max(*dissipation)<<endl;
// 	  //	  cout<<"AFTER BC MIN D.N = "<<min(*dissipation)<<endl;
	  
// 	  //	  PlotIt::contour(*Overture::getGraphicsInterface(), hft);
// 	  //	  hft.breakReference();
// 	  if ( Dcoeff.size() )
// 	    {
// 	      applyDSIBC(*dissipation,t,false,true,forceExtrap);
// 	      if ( nd==2 )
// 		hft.updateToMatchGrid(*dissipation->getMappedGrid(),GridFunctionParameters::cellCentered);
// 	      else
// 		hft.updateToMatchGrid(*dissipation->getMappedGrid(),GridFunctionParameters::faceCenteredAll);
// 	      hft = *dissipation;
// 	      dsimv2(&ncol,&one, &nH, &nd,
// 		     hft.getDataPointer(), Dcoeff.ptr(),
// 		     Doffset.ptr(), Dindex.ptr(),
// 		     dissipation->getDataPointer());
	      
// 	      //      PlotIt::contour(*Overture::getGraphicsInterface(), *dissipation);
// 	      applyDSIBC(*dissipation,t,false,true,forceExtrap);
// 	    }
// 	  else
// 	    {
	      
// 	      if ( dtype!=simpleSmoother )
// 		{
// 		  applyDSIBC(*dissipation,t,false,true,curlcurlHBC);
		  
// 		  dsimv2(&ncol,&one, &nE, &nd,
// 			 dissipation->Array_Descriptor.Array_View_Pointer1, Ecoeff.ptr(),
// 			 Eoffset.ptr(), Eindex.ptr(),
// 			 hft.Array_Descriptor.Array_View_Pointer1);
		  
// 		  //      applyDSIBC(hft,t,true,true,zeroBC); // this zeroes edges on the boundary
// 		  //hft.display("HFT PRE BC ");
// 		  //      eFieldn = eField + dt*hft;
// 		  //      applyDSIForcing( field[next+2], t, dt, true );
// 		  //      applyDSIBC(field[next+2],t+dt/2., true);
		  
// 		  //      hft = (eFieldn - eField)/dt;
// 		  //	  applyDSIBC(hft,t,true,true,zeroBC); 
// 		  //	  applyDSIBC(hft,t,true,true,forceExtrap); // this extrapolates the ghost values
// 		  applyDSIBC(hft,t,true,true,curlcurlcurlHBC); 
		  
// 		  //                              hft_r.updateToMatchGrid(cg[0],GridFunctionParameters::edgeCentered,nd);
// 		  //                              hft_r=0;
// 		  //                              reconstructDSIField(t,EField,hft,hft_r);
// 		  //hft.display("HFT");
// 		  //			      	          hft_r.display("HFT_R");
// 		  //                        cout<<"MAX HFT = "<<max(fabs(hft))<<endl;
// 		  //                        cout<<"MIN HFT = "<<min(fabs(hft))<<endl;
// 		  one=1;
// 		  dsimv2(&ncol,&one, &nH, &nd,
// 			 hft.getDataPointer(), Hcoeff.ptr(),
// 			 Hoffset.ptr(), Hindex.ptr(),
// 			 dissipation->getDataPointer());
// 		  sgn=1;
// 		}
// 	      else
// 		{
// 		  assert(orderOfArtificialDissipation==4);
// 		  symmetricSmoother( UnstructuredMapping::Face, dtmp, *dissipation, faceAreaNormals );
// 		  sgn=-1;
// 		}
// 	    }// if ! Dcoeff.size()
// 	} // if higher order
      
//       realArray &dsp = *dissipation;
//       if ( dtype!=simpleSmoother )
// 	{
// 	  for ( int i=0; i<nH; i++ )
// 	    {
// 	      dsp(i,0,0) = dispCoeff(i)*dsp(i,0,0);
// 	      hFieldn(i,0,0) -= dt*dsp(i,0,0);
// 	    }
// 	}
//       else
// 	{
// 	  for ( int i=0; i<nH; i++ )
// 	    {
// 	      hFieldn(i,0,0) += real(sgn)*dt*artificialDissipation*real(artificialDissipationInterval)*dsp(i,0,0);
// 	    }
// 	}
//       //cout<<"MAX D.N (postmult)= "<<max(fabs(*dissipation))<<endl;
//       //cout<<"MIN D.N (postmult)= "<<min(fabs(*dissipation))<<endl;
      
      
//       //      applyDSIBC(*dissipation,false);
      
//       //      dissipation->display("DISSIPATION");

  
  applyDSIForcing( field[next], t+dt/2., dt, false );
  applyDSIBC(field[next],t+dt,false);
  //  field[next].display();

  //assignBoundaryConditions( 0, 0, t, dt, field[next] ); //this really only applies the forcing if any (should move that)


  //  cout<<t<<"  "<<hFieldn(map.size(UnstructuredMapping::Face)/2+23,0,0)<<endl;
}

void Maxwell::
advanceUnstructuredDSI( int current, real t, real dt, realMappedGridFunction *field )
{
  // adapted from Bill's version to use the new UnstructuredMapping interface, 030820

  // added simple artificial dissipation 030828

  int debug=0;


  AvgType avgt = simple;
  //AvgType avgt = leastSq;

  if( debug ) printF(" inside advanceUnstructuredDSI\n");
  
  MappedGrid & mg = *(field[0].getMappedGrid());

  assert( mg.getGridType()==MappedGrid::unstructuredGrid );
  
  UnstructuredMapping & map = (UnstructuredMapping &) mg.mapping().getMapping();

  if ( !ulinks.size() )
    {
      UnstructuredMappingIterator face;
      
      UnstructuredMapping &umap = (UnstructuredMapping &) mg.mapping().getMapping();
      ulinks.resize(umap.size(UnstructuredMapping::Face));
      for ( face=umap.begin(UnstructuredMapping::Face); face!=umap.end(UnstructuredMapping::Face); face++ )
	{
	  unstructuredLink(umap, ulinks(*face), face, 1, int(pow(2,umap.getRangeDimension())));
	}
    }
	    
  const realArray & nodes = map.getNodes();
  
  int numberOfFaces = map.size(UnstructuredMapping::Edge);

  const int next = (current+1) %2;

  realArray & hField  = field[current];
  realArray & hFieldn = field[next];
  realArray & eField  = field[current+2];
  realArray & eFieldn = field[next+2];

  if ( mg.numberOfDimensions()==3 )
    {
      hFieldn = hField;
      applyDSIBC(field[next],t+dt,false);
      eFieldn = eField;
      applyDSIBC(field[next+2],t+dt/2., true);
      return;
    }


  hField.reshape(hField.dimension(0),hField.dimension(3));
  hFieldn.reshape(hFieldn.dimension(0),hFieldn.dimension(3));

  eField.reshape(eField.dimension(0),eField.dimension(3));
  eFieldn.reshape(eFieldn.dimension(0),eFieldn.dimension(3));

  
//   int nCells = map.size(UnstructuredMapping::Face);
		
//   int i1 = nCells/2+10;
//   int i2=0, i3=0;

//   cout<<"    --      "<<hField(i1,i2,i3)<<endl;

  // first advance E on the faces
  realArray eDotN(numberOfFaces);
  realArray nx(numberOfFaces),ny(numberOfFaces);

  int f;
  UnstructuredMappingIterator iter;

  // the grid geometry was built so that the faceArea and faceNormal are on edges
  //     (normal vertex centered).  Maybe the geometry should be updated with the 
  //     GridFunction setup instead?  Then this would be centerArea and centerNormal (?)
  const realArray & cFArea = mg.faceArea();
  const realArray & cFNorm = mg.faceNormal();

  //  cFNorm.display();
  //  dual grid areas
  //  const realArray & dcFArea = mg.centerArea();
  //  const realArray & dcFNorm = mg.centerNormal();

  UnstructuredMappingIterator iter_end;
  UnstructuredMappingAdjacencyIterator aiter, aiter_end;

  iter_end = map.end(UnstructuredMapping::Edge);
  //  hField.display("DSI hField");

  for ( iter=map.begin(UnstructuredMapping::Edge); iter!=iter_end; iter++ )
    {
      int f = *iter;

      nx(f) = cFArea(f,0,0)*cFNorm(f,0,0,0);
      ny(f) = cFArea(f,0,0)*cFNorm(f,0,0,1);

      eDotN(f) = nx(f)*eField(f,0)+ny(f)*eField(f,1);

      real dtoeps = dt/(eps);

      // YES, I KNOW there are only two adjacencies...
      aiter_end = map.adjacency_end(iter,UnstructuredMapping::Face);
      real edno = eDotN(f);
      for ( aiter=map.adjacency_begin(iter,UnstructuredMapping::Face); aiter!=aiter_end; aiter++ )
	{
	  eDotN(f) += dtoeps*aiter.orientation()*hField(*aiter);// + dt*cdiss*(ediss(f,0,0,0)*nx(f) + ediss(f,0,0,1)*ny(f));  
	  //	  cout<<"DSI  "<<f<<"  "<<dtoeps<<"  "<<aiter.orientation()<<"  "<<hField(*aiter)<<endl;
	}
      //      cout<<"DSI ediff "<<eDotN(f)-edno<<endl;
      //      cout<<"DSI enew "<<f<<"  "<<eDotN(f)<<endl;
    }

  // Now determine the full E vector at each face
  const intArray &edges = map.getEntities(UnstructuredMapping::Edge);
  const intArray &faces = map.getEntities(UnstructuredMapping::Face);

  //  eDotN.display("eDotN DSI");

  for( iter=map.begin(UnstructuredMapping::Edge,true); iter!=map.end(UnstructuredMapping::Edge,true); iter++ )
  {
    f=*iter;

    real exb=0., eyb=0.; // holds sum of E's

    // loop through the two vertices on this edge and check neighboring edges


    UnstructuredMappingAdjacencyIterator ve, ve_end, fe,fe_end, ev, ev_end;


    int navg = 0;
    real wsum=0;
    //    eFieldn = 0;
    //    hFieldn = 0;

    real ata00=nx(f)*nx(f),ata01=nx(f)*ny(f), ata11=ny(f)*ny(f);
    real ate0 = eDotN(f)*nx(f), ate1 = eDotN(f)*ny(f);

    aiter_end = map.adjacency_end(iter,UnstructuredMapping::Face);
    for ( aiter=map.adjacency_begin(iter,UnstructuredMapping::Face); aiter!=aiter_end; aiter++ )
      {
	fe_end = map.adjacency_end(aiter,UnstructuredMapping::Edge);
	for ( fe=map.adjacency_begin(aiter,UnstructuredMapping::Edge); fe!=fe_end; fe++ )
	  {
	    int f1 = *fe;

	    if ( f1!=f && (edges(f,0)==edges(f1,0) || edges(f,0)==edges(f1,1) ||
			   edges(f,1)==edges(f1,0) || edges(f,1)==edges(f1,1) ) )
	      {

		if ( avgt==simple )
		  {
		    real exa,eya, det; 
		    
		    // compute Ev from EDotN(f) and EDotN(f1) 
		    real w = avgt==simple ? 1. : (fabs(nx(f)*ny(f1) - ny(f)*nx(f1)))/(cFArea(f,0,0)*cFArea(f1,0,0));
		    
		    det=nx(f)*ny(f1)-ny(f)*nx(f1);
		    assert( fabs(det)>REAL_MIN*1000. );
		    exa=(eDotN(f) *ny(f1)-eDotN(f1)*ny(f) )/det;
		    eya=(eDotN(f1)*nx(f) -eDotN(f) *nx(f1))/det;
		    
		    exb+=w*exa;     
		    eyb+=w*eya;
		    navg++;
		  
		    wsum = navg;
		  }
		else
		  {
		    ata00 += nx(f1)*nx(f1);
		    ata01 += nx(f1)*ny(f1);
		    ata11 += ny(f1)*ny(f1);

		    ate0 += eDotN(f1)*nx(f1);
		    ate1 += eDotN(f1)*ny(f1);
		  }
	      }
	  	    
	  }
      }

    if ( avgt==leastSq )
      {
	real det = ata00*ata11 - ata01*ata01;
	assert(fabs(det)>REAL_MIN);
	
	exb = (ata11*ate0 - ata01*ate1)/det;
	eyb = (ata00*ate1 - ata01*ate0)/det;
	wsum = 1.;
      }

    eFieldn(f,0) = exb/wsum;
    eFieldn(f,1) = eyb/wsum;
  }

  eField.reshape(eField.dimension(0),1,1,eField.dimension(1));
  eFieldn.reshape(eFieldn.dimension(0),1,1,eFieldn.dimension(1));

  real cdiss = artificialDissipation;
  if ( cdiss>0. && false )
    {
      int ad_order = orderOfArtificialDissipation;
      
      realMappedGridFunction ediss;
      
      unstructuredDissipation( ad_order, UnstructuredMapping::Edge, field[current+2], ediss, ulinks );

      for( iter=map.begin(UnstructuredMapping::Edge,true); iter!=map.end(UnstructuredMapping::Edge,true); iter++ )
	{
	  f=*iter;
	  eFieldn(f,0,0,0) += cdiss*dt*ediss(f,0,0,0);
	  eFieldn(f,0,0,1) += cdiss*dt*ediss(f,0,0,1);
	}

      //            Range R(map.size(UnstructuredMapping::Edge));
      //            cout<<"min/max ediss0 "<<min(ediss(R,0,0,0))<<"  "<<max(ediss(R,0,0,0))<<endl;
      //            cout<<"min/max ediss1 "<<min(ediss(R,0,0,1))<<"  "<<max(ediss(R,0,0,1))<<endl;
    }

  applyDSIBC(field[next+2],t+dt/2.,true);


  // advance H 
  
  real dtByMu=dt/mu;

  for( iter=map.begin(UnstructuredMapping::Face,true); iter!=map.end(UnstructuredMapping::Face,true); iter++ )
    {
      real ht=0.; // holds dH/dt
      real pArea=0.;   // area of polygon 


      ArraySimpleFixed<real,4,3,1,1> verts;
      
      aiter_end = map.adjacency_end(iter,UnstructuredMapping::Vertex);
      int v=0;
      for ( aiter=map.adjacency_begin(iter,UnstructuredMapping::Vertex); aiter!=aiter_end; aiter++ )
	{
	  for ( int a=0; a<map.getRangeDimension(); a++ )
	    verts(v,a) = nodes(*aiter,a);
	  v++;
	}

      int nv=v;
      aiter=map.adjacency_begin(iter,UnstructuredMapping::Edge);
      for ( int v=0; v<nv; v++ )
	{
	  int f = *aiter;
	  int e = *iter;

	  int n0 = v;
	  int n1 = (v+1)%nv;
	  real px = (verts(n1,0)-verts(n0,0));
	  real py = (verts(n1,1)-verts(n0,1));

	  ht+= px*eFieldn(f,0,0,0)+py*eFieldn(f,0,0,1);
	  
	  // area of a polygon = +/- (1/2) sum{ x_i y_{i+1} - x_{i+1} y_i }
	  pArea+= (verts(n0,0)*verts(n1,1)-verts(n1,0)*verts(n0,1));

	  aiter++;
	}

      pArea*=.5;
      assert( pArea>0. );
      
      int e = *iter;
      hFieldn(e)=hField(e) -  dtByMu*ht/pArea;// + dt*cdiss*hdiss(e,0,0);  // here is the new value for H
//       cout<<"HFIELD ADDITION "<<e<<"  "<<-dtByMu*ht/pArea<<endl;
//       cout<<"NEW HFIELD "<<e<<"  "<<hFieldn(e)<<endl;
    }

  hField.reshape(hField.dimension(0),1,1,hField.dimension(1));
  hFieldn.reshape(hFieldn.dimension(0),1,1,hFieldn.dimension(1));
  // kkc apply periodic boundary conditions using tagged periodic elements.
  //  std::string perTag = std::string("periodic ") + UnstructuredMapping::EntityTypeStrings[int(UnstructuredMapping::Face)].c_str();
  //  UnstructuredMapping::tag_entity_iterator git, git_end;

  if ( cdiss>0. )
    {
      int ad_order = orderOfArtificialDissipation;
      
      realMappedGridFunction hdiss1,hdiss;
      unstructuredDissipation( ad_order, UnstructuredMapping::Face, field[current], hdiss,ulinks );
      //     unstructuredDissipation( 2, UnstructuredMapping::Face, field[current], hdiss1 );
      //     unstructuredDissipation( 2, UnstructuredMapping::Face, hdiss1, hdiss );
	    //unstructuredDissipation( ad_order, UnstructuredMapping::Edge, field[next+2], ediss );

      //      field[next] += cdiss*dt*hdiss;
      //field[next+2] += cdiss*dt*ediss;
      for( iter=map.begin(UnstructuredMapping::Face,true); iter!=map.end(UnstructuredMapping::Face,true); iter++ )
	{
	  f=*iter;
	  hFieldn(f,0,0) += cdiss*dt*hdiss(f,0,0);
	}

      //      dissipation->updateToMatchGridFunction(hdiss);
            *dissipation = hdiss;
      //            cout<<"min/max hdiss "<<min(hdiss)<<"  "<<max(hdiss)<<endl;
    }

  applyDSIBC(field[next],t+dt,false);
  //assignBoundaryConditions( 0, 0, t, dt, field[next] ); //this really only applies the forcing if any (should move that)

  getForcing( current, 0, hFieldn ,t+dt, 1);
  //  cout<<t<<"  "<<hFieldn(map.size(UnstructuredMapping::Face)/2+23,0,0)<<endl;
  //  cout<<"    ---     "<<hFieldn(i1,i2,i3)<<endl;

}

namespace {

  void symmetricSmoother( UnstructuredMapping::EntityTypeEnum centering, 
			  realMappedGridFunction &u, realMappedGridFunction &uxx,
			  realArray &projectionNormal, ArraySimple<ArraySimple<int> > &indices)
  {
    bool projectField = projectionNormal.getLength(0);
    MappedGrid &mg = *u.getMappedGrid();
    UnstructuredMapping & umap = (UnstructuredMapping &) mg.mapping().getMapping();

    UnstructuredMapping::EntityTypeEnum adjType = UnstructuredMapping::Vertex;
    //        	centering==UnstructuredMapping::Edge ? UnstructuredMapping::Face : UnstructuredMapping::Edge;

    UnstructuredMappingIterator u_entity,u_entity_end;

    ArraySimple<UnstructuredMappingAdjacencyIterator> adjEntities;
    int rDim = umap.getRangeDimension();
    int minRefs = rDim+1;
    int maxRefs = 200;
    int nHops = centering==UnstructuredMapping::Face ? 1 : 2;
    int dimb = min(rDim, u.getLength(3));

    u_entity_end = umap.end(centering);
    ArraySimple<int> dummy;
    int minN = INT_MAX, maxN = 0;
    for ( u_entity=umap.begin(centering); u_entity!=u_entity_end; u_entity++ )
      {
	ArraySimple<int>  &index = indices.size() ? indices[*u_entity] : dummy;

	if ( !index.size() )
	  {
	    unstructuredLink(umap, adjEntities, u_entity, nHops, minRefs,adjType);
	    index.resize(min(adjEntities.size(),maxRefs));
	    for ( int i=0; i<min(adjEntities.size(),maxRefs); i++ )
	      index[i] = *adjEntities[i];
	  }

	int ent_index = *u_entity;
	//	int nAdj = adjEntities.size();
	int nAdj = index.size();
	minN = min(minN, nAdj);
	maxN = max(maxN, nAdj);
	ArraySimpleFixed<real,3,1,1,1> udp,e_nrm;
	udp = 0.;

	if ( true || (!u_entity.isGhost() && !u_entity.isBC()) )
	  {
	    if ( projectField )
	      for ( int a=0; a<dimb; a++ )
		{
		  e_nrm[a] = projectionNormal(ent_index,0,0,a);
		  udp[0] -= u(ent_index,0,0,a)*e_nrm[a]*real(nAdj);
		}
	    else
	      for ( int a=0; a<dimb; a++ )
		udp[a] -= u(ent_index,0,0,a)*real(nAdj);
	    
	    //    for ( int i_adj=0; i_adj<adjEntities.size(); i_adj++ )
	    for ( int i_adj=0; i_adj<index.size(); i_adj++ )
	      {
		UnstructuredMappingAdjacencyIterator &adj = adjEntities[i_adj];
		//int adj_index = *adj;
		int adj_index = index[i_adj];
		
		if ( projectField )
		  for ( int a=0; a<dimb; a++ )
		    udp[0] += u(adj_index,0,0,a)*e_nrm[a];
		else
		  for ( int a=0; a<dimb; a++ )
		    udp[a] += u(adj_index,0,0,a);
	      }
	  }

	if ( projectField )
	  uxx(ent_index,0,0) = udp[0];
	else
	  for ( int a=0; a<dimb; a++ )
	    uxx(ent_index,0,0,a) = udp[a];
      }

    //    cout<<"minN, maxN = "<<minN<<"  "<<maxN<<endl;
  }

//   void applyDissipationToField(MappedGrid &mg, UnstructuredMapping::EntityTypeEnum centering,
// 			       realMappedGridFunction &field, realMappedGridFunction &fieldn,
// 			       ArraySimple<real> & Ccoeff_1,ArraySimple<int> & Coffset_1, ArraySimple<int> & Cindex_1,
// 			       ArraySimple<real> & Ccoeff_2,ArraySimple<int> & Coffset_2, ArraySimple<int> & Cindex_2,
// 			       realMappedGridFunction &disp, realArray &dispCoeff, realArray &areaNormals,
// 			       real artificialDissipation, int artificialDissipationInterval, int orderOfArtificialDissipation,
// 			       real t, real dt,Maxwell &mx)
  void applyDissipationToField(MappedGrid &mg, UnstructuredMapping::EntityTypeEnum centering,
			       realMappedGridFunction &field, realMappedGridFunction &fieldn,
			       ArraySimple<real> & Ccoeff_1,ArraySimple<int> & Coffset_1, ArraySimple<int> & Cindex_1,
			       ArraySimple<real> & Ccoeff_2,ArraySimple<int> & Coffset_2, ArraySimple<int> & Cindex_2,
			       realMappedGridFunction &disp, ArraySimple<real> &dispCoeff, realArray &areaNormals,
			       real artificialDissipation, int artificialDissipationInterval, int orderOfArtificialDissipation,
			       real t, real dt, Maxwell &mx, bool recompute )
  {
    real one=1;
    realMappedGridFunction hft,hft_r,dtmp;
    
    int ncol=0;
    int nd=mg.numberOfDimensions();
    int sgn = 1;
    
    UnstructuredMapping::EntityTypeEnum otherCentering = 
      centering==UnstructuredMapping::Face ? UnstructuredMapping::Edge : UnstructuredMapping::Face;

    int n1=Coffset_1.size()-1;
    int n2=Coffset_2.size()-1;

    if ( recompute )
      {
	if ( centering==UnstructuredMapping::Face )
	  hft.updateToMatchGrid(mg,GridFunctionParameters::edgeCentered);
	else if ( nd==2 )
	  hft.updateToMatchGrid(mg,GridFunctionParameters::cellCentered);
	else
	  hft.updateToMatchGrid(mg,GridFunctionParameters::faceCenteredAll);

	ArraySimple<ArraySimple<int> > &indices = centering==UnstructuredMapping::Edge ? mx.SEindex : mx.SHindex;
	if ( !indices.size() ) indices.resize( n1 );

	switch(dtype) {
	case curlcurl:      
	  dsimv2(&ncol,&one, &n2, &nd,
		 field.Array_Descriptor.Array_View_Pointer1, Ccoeff_2.ptr(),
		 Coffset_2.ptr(), Cindex_2.ptr(),
		 hft.Array_Descriptor.Array_View_Pointer1);
	  mx.applyDSIBC(hft,t,centering!=UnstructuredMapping::Edge,true,Maxwell::curlHBC); 
	  break;
	case AtransA:
	  dsimv2(&ncol,&one, &n1, &nd,
		 field.Array_Descriptor.Array_View_Pointer1, Ccoeff_1.ptr(),
		 Coffset_1.ptr(), Cindex_1.ptr(),
		 hft.Array_Descriptor.Array_View_Pointer1);
	  
	  mx.applyDSIBC(hft,t,centering!=UnstructuredMapping::Edge,true,Maxwell::curlHBC); 
	  //                                      hft_r.updateToMatchGrid(cg[0],GridFunctionParameters::edgeCentered,nd);
	  //                                      hft_r=0;
	  //                                      reconstructDSIField(t,EField,hft,hft_r);
	  //       //hft.display("HFT");
	  //       			      			      	          hft_r.display("HFT_R");
	  //      cout<<"MAX HFT = "<<max(fabs(hft))<<endl;
	  //      cout<<"MIN HFT = "<<min(fabs(hft))<<endl;
	  break;
	case AAtrans:
	  one=1;
	  ncol = n1;
	  dsimv2(&ncol,&one, &n2, &nd,
		 field.Array_Descriptor.Array_View_Pointer1, Ccoeff_2.ptr(),
		 Coffset_2.ptr(), Cindex_2.ptr(),
		 hft.Array_Descriptor.Array_View_Pointer1);
	  mx.applyDSIBC(hft,t,centering!=UnstructuredMapping::Edge,true,Maxwell::curlHBC); 
	  break;
	case simpleSmoother:
	case smoothProjection:
	default:
	  break;
	}

	  
	realArray emptyArray;
	switch(dtype) {
	case curlcurl:
	  one = -1;
	  ncol=0;
	  dsimv2(&ncol,&one, &n1, &nd,
		 hft.getDataPointer(), Ccoeff_1.ptr(),
		 Coffset_1.ptr(), Cindex_1.ptr(),
		 disp.getDataPointer());
	  break;
	case AtransA:
	  one = 1;
	  ncol = n2;
	  dsimv2(&ncol,&one, &n1, &nd,
		 hft.getDataPointer(), 
		 Ccoeff_1.ptr(), Coffset_1.ptr(), Cindex_1.ptr(),
		 disp.getDataPointer());
	  break;
	case AAtrans:
	  ncol = 0;
	  dsimv2(&ncol,&one, &n2, &nd,
		 hft.getDataPointer(), Ccoeff_2.ptr(),
		 Coffset_2.ptr(), Cindex_2.ptr(),
		 disp.getDataPointer());
	  break;
	case simpleSmoother:
	  if ( centering==UnstructuredMapping::Edge )
	    hft_r.updateToMatchGrid(mg,GridFunctionParameters::edgeCentered,nd);
	  else if ( nd==2 && centering==UnstructuredMapping::Face)
	    hft_r.updateToMatchGrid(mg,GridFunctionParameters::cellCentered);
	  else
	    hft_r.updateToMatchGrid(mg,GridFunctionParameters::faceCenteredAll,nd);
	  
	  mx.reconstructDSIField(t, centering==UnstructuredMapping::Face ? Maxwell::HField : Maxwell::EField, field,hft_r);
	  
	  if ( orderOfArtificialDissipation>2 )
	    {
	      
	      if ( centering==UnstructuredMapping::Edge )
		dtmp.updateToMatchGrid(mg,GridFunctionParameters::edgeCentered,nd);
	      else if ( nd==2 && centering==UnstructuredMapping::Face )
		dtmp.updateToMatchGrid(mg,GridFunctionParameters::cellCentered);
	      else
		dtmp.updateToMatchGrid(mg,GridFunctionParameters::faceCenteredAll,nd);
	      
	      realArray emptyArray;
	      
	      symmetricSmoother( centering, hft_r, dtmp, emptyArray, indices );
	    }
	  else
	    symmetricSmoother( centering, hft_r, disp, areaNormals,indices );
	  
	  //	      applyDSIBC(*dissipation,t,false,true,zeroBC);
	  break;
	case smoothProjection:
	  cout<<"SMOOTHING PROJECTION"<<endl;
	  if ( orderOfArtificialDissipation>2 )
	    {
	      
	      if ( centering==UnstructuredMapping::Edge )
		dtmp.updateToMatchGrid(mg,GridFunctionParameters::edgeCentered);
	      else if ( nd==2 && centering==UnstructuredMapping::Face )
		dtmp.updateToMatchGrid(mg,GridFunctionParameters::cellCentered);
	      else
		dtmp.updateToMatchGrid(mg,GridFunctionParameters::faceCenteredAll);
	      
	      symmetricSmoother( centering, field, dtmp, emptyArray, indices );
	    }
	  else
	    symmetricSmoother( centering, field, disp, emptyArray,indices );
	  break;
	default:
	  break;
	}
	  
	one = -1;
    
	for ( int d=0; d<(orderOfArtificialDissipation/2-1); d++ )
	  {
	    if ( dtype!=simpleSmoother && dtype!=smoothProjection)
	      {
		mx.applyDSIBC(disp,t,centering==UnstructuredMapping::Edge,true,Maxwell::curlcurlHBC);
	    
		dsimv2(&ncol,&one, &n2, &nd,
		       disp.Array_Descriptor.Array_View_Pointer1, Ccoeff_2.ptr(),
		       Coffset_2.ptr(), Cindex_2.ptr(),
		       hft.Array_Descriptor.Array_View_Pointer1);
	    
		mx.applyDSIBC(hft,t,centering==UnstructuredMapping::Edge,true,Maxwell::curlcurlcurlHBC); 
	    
		one=1;
		dsimv2(&ncol,&one, &n1, &nd,
		       hft.getDataPointer(), Ccoeff_1.ptr(),
		       Coffset_1.ptr(), Cindex_1.ptr(),
		       disp.getDataPointer());
		sgn=1;
	      }
	    else if (dtype==smoothProjection )
	      {
		realArray emptyArray;
		assert(orderOfArtificialDissipation==4);
		symmetricSmoother( centering, field, disp, emptyArray,indices );
		sgn=-1;
	      }
	    else
	      {
		assert(orderOfArtificialDissipation==4);
		symmetricSmoother( centering, dtmp, disp, areaNormals,indices );
		sgn=-1;
	      }
	  } // if higher order
      } // if recompute

    realArray &dsp = disp;
    if ( dtype!=simpleSmoother && dtype!=smoothProjection )
      {
	for ( int i=0; i<n1; i++ )
	  {
	    dsp(i,0,0) = dispCoeff(i)*dsp(i,0,0);
	    fieldn(i,0,0) -= dt*dsp(i,0,0);
	  }
      }
    else
      {
	//	cout<<"MAX fabs(DSP) = "<<max(fabs(disp))<<endl;
	for ( int i=0; i<n1; i++ )
	  {	
	    //	      cout<<"DISSIPATION ON EDGE 4483 = "<<real(sgn)*dt*artificialDissipation*real(artificialDissipationInterval)*dsp(i,0,0)<<", "<<"field is = "<<fieldn(i,0,0)<<", dsp = "<<dsp(i,0,0)<<endl;
	    fieldn(i,0,0) += real(sgn)*dt*artificialDissipation*dsp(i,0,0);
	  }
      }
  }


  void unstructuredDissipation( int order, UnstructuredMapping::EntityTypeEnum centering, 
				realMappedGridFunction &field_gf, realMappedGridFunction &disp_gf,
				ArraySimple<ArraySimple<UnstructuredMappingAdjacencyIterator> > &ulink)
  {
    assert( order>1 && order%2==0 );
    assert( centering==UnstructuredMapping::Edge || centering==UnstructuredMapping::Face );

    MappedGrid & mg = *field_gf.getMappedGrid();
    assert( mg.getGridType()==MappedGrid::unstructuredGrid );

    UnstructuredMapping & umap = (UnstructuredMapping &) mg.mapping().getMapping();
    
    const realArray &xyz = umap.getNodes();

    //    mg.update( MappedGrid::THEcenterArea | MappedGrid::THEcenterNormal | MappedGrid::THEcellVolume );

    Range all;
    disp_gf.updateToMatchGridFunction(field_gf);

    //    UnstructuredMapping::EntityTypeEnum adjType = centering==UnstructuredMapping::Edge ? UnstructuredMapping::Face : UnstructuredMapping::Edge;
        UnstructuredMapping::EntityTypeEnum adjType = centering==UnstructuredMapping::Edge ? UnstructuredMapping::Vertex : UnstructuredMapping::Edge;
    //    UnstructuredMapping::EntityTypeEnum adjType = UnstructuredMapping::Vertex;

    UnstructuredMappingIterator iter,iter_end;
    UnstructuredMappingAdjacencyIterator aiter,aiter_end, aait, aait_end, vit,vit_end;
    
    realArray &disp = (realArray &)disp_gf;
    realArray &field = (realArray &)field_gf;
    realMappedGridFunction dtmp_gf;
    dtmp_gf.updateToMatchGridFunction(field_gf);
    dtmp_gf = field;

    realArray &dtmp = (realArray &)dtmp_gf;
    //    dtmp = field;

    int nComp = disp.getLength(3);

    real sgn = 1;

    realArray & dcFVols = mg.cellVolume();

    for ( int lp=2; lp<=order; lp+=2 )
      {
	disp = 0;

	iter_end = umap.end(centering,true);
	for ( iter = umap.begin(centering,true); iter!=iter_end; iter++ )
	  {

	    // here we compute 
	    //     d_i = \sum_{a=0}^{a=(nAdjacent-1)} c_{ia}(u_a - u_i)
	    // ie we loop through the adjacent "centering" entities to entity i and compute a 
	    // simple dissipation based on averaging. 

	    // For now, c_{ia} = 1/nAdjacent
	    
	    ArraySimpleFixed<real,3,1,1,1> x0,xi;

	    vit_end = umap.adjacency_end( iter, UnstructuredMapping::Vertex );
	    int nv=0;
	    real vScale=0;
	    x0=0.;
	    for ( vit=umap.adjacency_begin(iter,UnstructuredMapping::Vertex); vit!=vit_end; vit++ )
	      {
		vScale += dcFVols(*vit,0,0,0);
		for ( int a=0; a<2; a++ )
		  x0[a] += xyz(*vit,a);
		nv++;
	      }

	    for ( int a=0; a<2; a++ )
	      x0[a] /= real(nv);

	    vScale/=real(nv);
	    vScale = sqrt(vScale);

	    int i = *iter;
	    int na=0;

	    if ( centering==UnstructuredMapping::Face )
	      {
#if 1
		for ( int adj=0; adj<ulink[*iter].size(); adj++ )
		  {
		    UnstructuredMappingAdjacencyIterator &aiter = ulink[*iter][adj];
		    if ( aiter!=iter )
		      {
			int a = *aiter;
			disp(i,0,0) += (dtmp(a,0,0)-dtmp(i,0,0));
			na++;
		      }
		    
		  }
	      
#else
		aiter_end = umap.adjacency_end(iter, adjType);
		for ( aiter=umap.adjacency_begin(iter,adjType); aiter!=aiter_end; aiter++ )
		  {
		    aait_end = umap.adjacency_end(aiter, centering);
		    for ( aait=umap.adjacency_begin(aiter,centering); aait!=aait_end; aait++ )
		      {
			if ( aait!=iter )
			  {
			    
			    int e =*iter;
			    int e1=*aait;
			    
			    //			    cout<<i<<"  "<<e1<<endl;
			    vit_end = umap.adjacency_end( aait, UnstructuredMapping::Vertex );
			    int nv=0;
			    xi=0.;
			    for ( vit=umap.adjacency_begin(aait,UnstructuredMapping::Vertex); vit!=vit_end; vit++ )
			      {
				for ( int a=0; a<2; a++ )
				  xi[a] += xyz(*vit,a);
				nv++;
			      }
			    
			    for ( int a=0; a<2; a++ )
			      xi[a] /= real(nv);
			    

			    //			    cout<<"xi is "<<xi<<endl;
			    real hmag = sqrt( (xi[0]-x0[0])*(xi[0]-x0[0]) + (xi[1]-x0[1])*(xi[1]-x0[1]) ) ;
			    
			    int a = *aait;
			    //			    for ( int c=0; c<nComp; c++ )
			    //			      disp(i,0,0,c) += (dtmp(a,0,0,c)-dtmp(i,0,0,c));
			    
			    disp(i,0,0) += (dtmp(a,0,0)-dtmp(i,0,0));
			    na++;
			    
			  }
			
		      }
		  }
#endif
		//		cout<<"======= na "<<na<<endl;
	      }
	    else
	      {
		const intArray &edges = umap.getEntities(UnstructuredMapping::Edge);

		UnstructuredMappingAdjacencyIterator fe, fe_end;

		int f = i;//*iter;
		ArraySimpleFixed<int,4,1,1,1> adjE;
		int nn=0;
		aiter_end = umap.adjacency_end(iter,UnstructuredMapping::Face);
		for ( aiter=umap.adjacency_begin(iter,UnstructuredMapping::Face); aiter!=aiter_end; aiter++ )
		  {
		    fe_end = umap.adjacency_end(aiter,UnstructuredMapping::Edge);
		    for ( fe=umap.adjacency_begin(aiter,UnstructuredMapping::Edge); fe!=fe_end; fe++ )
		      {
			int f1 = *fe;
			
			if ( f1!=f && ! (edges(f,0)==edges(f1,0) || edges(f,0)==edges(f1,1) ||
					 edges(f,1)==edges(f1,0) || edges(f,1)==edges(f1,1) ) )
			  {
			    for ( int c=0; c<nComp; c++ )
			      disp(i,0,0,c) += (dtmp(f1,0,0,c)-dtmp(i,0,0,c));

			    na++;
			  }
			else if ( f1!=f )
			  {
			    adjE[nn] = f1;
			    nn++;
			  }

		      }
		  }

		aiter_end = umap.adjacency_end(iter, UnstructuredMapping::Vertex);
		for ( aiter=umap.adjacency_begin(iter,UnstructuredMapping::Vertex); aiter!=aiter_end; aiter++ )
		  {
		    aait_end = umap.adjacency_end(aiter, centering);
		    for ( aait=umap.adjacency_begin(aiter,centering); aait!=aait_end; aait++ )
		      {
			int f1 = *aait;
			if ( aait!=iter && f1!=adjE[0] && f1!=adjE[1] && f1!=adjE[2] && f1!=adjE[3] )
			  {
			    for ( int c=0; c<nComp; c++ )
			      disp(i,0,0,c) += (dtmp(f1,0,0,c)-dtmp(i,0,0,c));
			    
			    na++;

			  }
		      }


		  }
	      }
	    // 	    for ( int c=0; c<nComp; c++ )
	    // disp(i,0,0,c) /= real(na);
	    
	  }


	//XXXX	applyDSIBC( disp_gf, centering==UnstructuredMapping::Edge );
	//	cout<<"nep is "<<nep<<endl;
	//	cout<<"===="<<endl;
	
	sgn *= -1;
	
	if ( order>2 && lp!=order )
	  dtmp = disp;


      }

    iter_end = umap.end(centering);
    for ( iter = umap.begin(centering); iter!=iter_end; iter++ )
      for ( int c=0; c<nComp; c++ )
    	disp(*iter,0,0,c) = -sgn*disp(*iter,0,0,c);
      

  }

}

void Maxwell::
applyDSIBC( realMappedGridFunction &gf, real t, bool isEField, bool isProjection, int bcopt  )
  {

    real tm0 = getCPU();

    real maxgf,mingf;
    maxgf=max(gf);
    mingf=min(gf);


    MappedGrid & mg = *gf.getMappedGrid();
    assert( mg.getGridType()==MappedGrid::unstructuredGrid );
    
    UnstructuredMapping & umap = (UnstructuredMapping &) mg.mapping().getMapping();

    int dDim = umap.getDomainDimension();
    int rDim = umap.getRangeDimension();

    const realArray &nodes = umap.getNodes();

    UnstructuredMapping::EntityTypeEnum efieldCent = UnstructuredMapping::Edge;
    UnstructuredMapping::EntityTypeEnum hfieldCent = UnstructuredMapping::Face;
    // // // PERIODIC BC
    std::string perTag = std::string("periodic ") + UnstructuredMapping::EntityTypeStrings[umap.getDomainDimension()].c_str();
    std::string ghostTag = std::string("Ghost ")+UnstructuredMapping::EntityTypeStrings[int(UnstructuredMapping::Edge)].c_str();

    UnstructuredMapping::tag_entity_iterator git, git_end;
    git =  umap.tag_entity_begin(perTag);
    git_end = umap.tag_entity_end(perTag);
  
    bool vCent = mg.isAllVertexCentered();

    const realArray &cFArea = vCent ? mg.centerArea() : mg.faceArea();
    const realArray &cFNorm = vCent ? mg.centerNormal() : mg.faceNormal();
    const realArray &cEArea = vCent ? mg.faceArea() : mg.centerArea();
    const realArray &cENorm = vCent ? mg.faceNormal() : mg.centerNormal();

    //    const realArray & norms = isEField ? cENorm : cFNorm;
    //    const realArray & areas = isEField ? cEArea : cFArea;

    const realArray & areaNorms = isEField ? edgeAreaNormals : faceAreaNormals;

    const realArray &verts = mg.vertex();
    const intArray &edges = umap.getEntities(UnstructuredMapping::Edge);

//     cEArea.display("cEArea");
//     cENorm.display("cENorm");
//     verts.display("verts");

    if ( isProjection )
      { // we cannot use gf.periodicUpdate() since the normals may flip the sign of the projections
	//     on periodic faces and edges
	UnstructuredMapping::EntityTypeEnum cent = isEField ? efieldCent : hfieldCent;

	const IntegerArray &pbc = *mg.getUnstructuredPeriodicBC( cent );

	const realArray & norms = isEField ? cENorm : cFNorm;
	const realArray & areas = isEField ? cEArea : cFArea;

	int pbn = pbc.getLength(0);
	if ( rDim==3 )
	  for ( int i=0; i<pbn; i++ )
	    {
	      int c1 = pbc(i,0);
	      int c2 = pbc(i,1);
	      

	      int sgn = 1;//( norms(c1,0,0,0)*norms(c2,0,0,0) + 
			  //norms(c1,0,0,1)*norms(c2,0,0,1) + 
			  //norms(c1,0,0,2)*norms(c2,0,0,2) ) > 0 ? 1 : -1;

	      //	      for ( int a=0; a<rDim; a++ )
	      gf(c1,0,0,0) = sgn*gf(c2,0,0,0);//*areas(c1,0,0)/areas(c2,0,0);

	    }
	else
	  {
	    if ( isEField )
	      for ( int i=0; i<pbn; i++ )
		{
		  int c1 = pbc(i,0);
		  int c2 = pbc(i,1);

		  int sgn = 1;//( norms(c1,0,0,0)*norms(c2,0,0,0) + 
		  //	      norms(c1,0,0,1)*norms(c2,0,0,1) ) > 0 ? 1 : -1;
//   		  cout<<"edges: "<<edges(c1,0)<<"  "<<edges(c1,1)<<endl;
//   		  cout<<"       "<<edges(c2,0)<<"  "<<edges(c2,1)<<endl;
// 		  cout<<norms(c1,0,0,0)<<"  "<<norms(c2,0,0,0)<<endl;
// 		  cout<<norms(c1,0,0,1)<<"  "<<norms(c2,0,0,1)<<endl;
				  
// 		  cout<<areas(c1,0,0)<<"  "<<areas(c2,0,0)<<endl;;
//    		  cout<<( norms(c1,0,0,0)*norms(c2,0,0,0) + 
//    			  norms(c1,0,0,1)*norms(c2,0,0,1) )<<endl;

		  //		  for ( int a=0; a<rDim; a++ )
		  //		  cout<<areas(c1,0,0)/areas(c2,0,0)<<endl;
		  gf(c1,0,0,0) = sgn*gf(c2,0,0,0);//*areas(c1,0,0)/areas(c2,0,0);
		}
	    else
	      for ( int i=0; i<pbn; i++ )
		{
		  int c1 = pbc(i,0);
		  int c2 = pbc(i,1);
		  gf(c1,0,0) = gf(c2,0,0);
		}
		
	  }
	
      }
    else
      {
	gf.periodicUpdate();
      }

    // // // NONPERIODIC BC

    // on an unstructured *Vertex centered* grid this will be the cell-face normal in the FV sense

    const realArray & dcFNorm = mg.centerNormal();

    realArray xcent;
    if ( forcingOption==twilightZoneForcing )
      getCenters(mg, (isEField ? efieldCent : hfieldCent), xcent);
    
    const IntegerArray &bci = *mg.getUnstructuredBCInfo(  (isEField ? efieldCent : hfieldCent) );//UnstructuredMapping::Edge );

    maxgf = -REAL_MAX;
    mingf = REAL_MAX;
    IntegerArray eext(1);
    RealArray fullField(1,3);
    fullField=0.;
    if ( bcopt&forcedBC )
      {

	for ( int i=0; i<bci.getLength(0); i++ )
	  {
	    int ei = bci(i,0);

	    if ( forcingOption==twilightZoneForcing )
	      {
		assert( tz!=NULL );
		OGFunction & e = *tz;
		
		real x0 = xcent(ei,0);
		real y0 = xcent(ei,1);
		real z0 = rDim==3 ? xcent(ei,2) : 0.;
		if ( bcopt & curlHBC )
		  {
		    if ( rDim==2 )
		      {
			if ( isEField )
			  {
			    fullField(0,0) =  e.y(x0,y0,z0,hz,t);
			    fullField(0,1) = -e.x(x0,y0,z0,hz,t);
			  }
		      }
		    else
		      {
			fullField(0,0) =  e.y(x0,y0,z0,hz,t)-e.z(x0,y0,z0,hy,t);
			fullField(0,1) =-(e.x(x0,y0,z0,hz,t)-e.z(x0,y0,z0,hx,t));
			fullField(0,2) =  e.x(x0,y0,z0,hy,t)-e.y(x0,y0,z0,hx,t);
		      }
		  }
		else if ( bcopt & curlcurlHBC )
		  {
		    fullField(0,0) = (e.xy(x0,y0,z0,1,t)-e.yy(x0,y0,z0,0,t))-(-(e.xz(x0,y0,z0,2,t)-e.zz(x0,y0,z0,0,t)));
		    fullField(0,1) =-((e.xx(x0,y0,z0,1,t)-e.xy(x0,y0,z0,0,t))-(e.yz(x0,y0,z0,2,t)-e.zz(x0,y0,z0,1,t)));
		    fullField(0,2) = (e.xy(x0,y0,z0,1,t)-e.yy(x0,y0,z0,0,t))-(-(e.xx(x0,y0,z0,2,t)-e.xz(x0,y0,z0,0,t)));
		  }
		else if ( bcopt & curlcurlcurlHBC )
		  {
		    
		  }
		else // bcopt & curlcurlcurlcurlHBC 
		  {
		  }

		if ( isProjection )
		  {
		    if ( rDim==2 )
		      gf(ei,0,0,0) = ( fullField(0,0)*areaNorms(ei,0,0,0)
				       +fullField(0,1)*areaNorms(ei,0,0,1) );
		    else
		      gf(ei,0,0,0) = ( fullField(0,0)*areaNorms(ei,0,0,0)
				       +fullField(0,1)*areaNorms(ei,0,0,1) 
				       +fullField(0,2)*areaNorms(ei,0,0,2) );
		  }
		else
		  {
		    for ( int a=0; a<rDim; a++ )
		      gf(ei,0,0,a) = fullField(0,a);
		  }		
	      }
	  }
	
      }
    else if ( isEField )
      for ( int i=0; i<bci.getLength(0); i++ )
	{
	  int ei = bci(i,0);
	  if ( bci(i,1)==perfectElectricalConductor || bcopt==zeroBC )
	    if ( isProjection )
	      gf(ei,0,0,0) = 0;
	    else
	      for ( int a=0; a<rDim; a++ )
		gf(ei,0,0,a) = 0;
	  else if ( /*umap.isGhost( UnstructuredMapping::Edge, ei ) &&*/ (bcopt&forceExtrap) )
	    {
	      //cout<<"extrapolating edge field at "<<ei<<endl;
	      //	      assert( umap.isGhost( UnstructuredMapping::Face, ei ) );
	      bool oldUseGhostInReconstruction = this->useGhostInReconstruction;
	      this->useGhostInReconstruction = false;
	      UnstructuredMappingAdjacencyIterator aiter, aiter_end;
	      // 050707 	      UnstructuredMapping::EntityTypeEnum ntype = rDim==3 ? UnstructuredMapping::Region : UnstructuredMapping::Vertex;
	      UnstructuredMapping::EntityTypeEnum ntype = (rDim==3 ? UnstructuredMapping::Region : UnstructuredMapping::Edge);
	      //UnstructuredMapping::EntityTypeEnum ntype = UnstructuredMapping::Vertex;
	      aiter_end = umap.adjacency_end(UnstructuredMapping::Edge,ei,ntype);
// 	      if ( isProjection )
// 		gf(ei,0,0,0) = 0.;
// 	      else
// 		for ( int a=0; a<rDim; a++ )
// 		  gf(ei,0,0,a) = 0.;

	      int na=0;

	      UnstructuredMappingIterator iter=umap.begin(UnstructuredMapping::Edge);
	      iter.setLocation(ei);
	      
	      ArraySimple< UnstructuredMappingAdjacencyIterator > adj;

	      int nh=unstructuredLink(umap, adj, iter, 1, 4, ntype);
	      bool foundInterior = !adj[0].isGhost() && !adj[0].isBC() /*&& !umap.hasTag(UnstructuredMapping::Edge,*adj[0],"__bcnum Edge" )*/;
	      real wsum = 0,w;
	      for ( int i=1; i<adj.size() && !foundInterior; i++ )
		{
		  foundInterior = !adj[i].isGhost() && !adj[i].isBC()/*&& !umap.hasTag(UnstructuredMapping::Edge,*adj[i],"__bcnum Edge" )*/;
		}
	      if ( !foundInterior )
		{
		  nh=unstructuredLink(umap, adj, iter, 4, 4, ntype);
		  for ( int i=1; i<adj.size() && !foundInterior; i++ )
		    {
		      foundInterior = !adj[i].isGhost() && !adj[i].isBC()/*&& !umap.hasTag(UnstructuredMapping::Edge,*adj[i],"__bcnum Edge" )*/;
		    }
		  assert(foundInterior);
		}
		
	      //	      cout<<"NHOPS = "<<nh<<", NADJ = "<<adj.size()<<endl;


	      //	      cout<<"NADJ FOR BC = "<<adj.size()<<endl;
	      // 	      for ( aiter=umap.adjacency_begin(UnstructuredMapping::Face,ei,ntype);
	      // 		    aiter!=aiter_end;
	      // 		    aiter++ )
	      // 		{
	      // 		  UnstructuredMappingAdjacencyIterator aif, aif_end;
	      // 		  aif_end = umap.adjacency_end(aiter, UnstructuredMapping::Face);
	      // 		  for ( aif=umap.adjacency_begin(aiter, UnstructuredMapping::Face);
	      // 			aif!=aif_end;
	      // 			aif++ )
	      // 		    {
	      real maxw=0;
	      //	      cout<<"xc "<<xc<<endl;
	      //	      cout<<"nadj = "<<adj.size()<<endl;
	      int nChecked = 0;
	      wsum=0;
	      bool foundUsable=false;
	      int navg = 0;
	      gf(ei,0,0,0) =  0;
	      for ( int ae=0; ae<adj.size() && !foundUsable; ae++ )
		{
		  UnstructuredMappingAdjacencyIterator &aif = adj[ae];
		  if ( !aif.isGhost() && !aif.isBC() /*&& !umap.hasTag(UnstructuredMapping::Edge,*aif,"__bcnum Edge" )*/)
		    {
		      eext(0) = *aif;
		      if ( (foundUsable = reconstructDSIAtEntities( t, EField, eext, gf, fullField) ) )
			{
			  //			  cout<<fullField(0,0)<<"  "<<fullField(0,1)<<"  "<<fullField(0,2)<<endl;
			  for ( int a=0; a<rDim; a++ )
			    gf(ei,0,0,0) += fullField(0,a)*edgeAreaNormals(ei,0,0,a);
			  navg++;
			}
		    }
		}

	      if ( !foundUsable ) cout<<" WARNING : might not have found usable entity for extrapolation bc for Edge"<<ei<<endl;
	      
	      this->useGhostInReconstruction = oldUseGhostInReconstruction;
	    }
	  else if ( bci(i,1)==dirichlet && bcopt!=forceExtrap /*&& umap.isGhost(UnstructuredMapping::Edge,ei)*/)
	    {
	      if ( forcingOption==twilightZoneForcing )
		{
		  assert( tz!=NULL );
		  OGFunction & e = *tz;

		  real x0 = xcent(ei,0);
		  real y0 = xcent(ei,1);
		  real z0 = rDim==3 ? xcent(ei,2) : 0.;
		  if ( isProjection )
		    {
		      if ( rDim==2 )
			gf(ei,0,0,0) = 
			  ( e(x0,y0,z0,0,t)*areaNorms(ei,0,0,0)
			    +e(x0,y0,z0,1,t)*areaNorms(ei,0,0,1) );
		      else
			gf(ei,0,0,0) = ( e(x0,y0,z0,0,t)*areaNorms(ei,0,0,0)
						       +e(x0,y0,z0,1,t)*areaNorms(ei,0,0,1) 
						       +e(x0,y0,z0,2,t)*areaNorms(ei,0,0,2) );
		    }
		  else
		    {
		      for ( int a=0; a<rDim; a++ )
			gf(ei,0,0,a) = e(x0,y0,z0,a,t);
		    }
		}
	    }
	}
    else
      for ( int i=0; i<bci.getLength(0); i++ )
	{
	  int ei = bci(i,0);
	  UnstructuredMappingAdjacencyIterator hInside,hOutside;
	  //	  hInside = umap.adjacency_begin(UnstructuredMapping::Edge,ei,UnstructuredMapping::EntityTypeEnum(UnstructuredMapping::Face));
	  //	  hOutside = umap.adjacency_begin(UnstructuredMapping::Edge,ei,UnstructuredMapping::EntityTypeEnum(UnstructuredMapping::Face));
// 	  if ( hInside.isGhost() )
// 	    { 
// 	      hInside++;
// 	    }
// 	  else
// 	    {
// 	      hOutside++;
// 	    }

	  if ( (umap.isGhost( UnstructuredMapping::Face, ei ) && bci(i,1) == perfectElectricalConductor) || (bcopt&forceExtrap) )
	    {
	      //	      assert( umap.isGhost( UnstructuredMapping::Face, ei ) );
	    bool oldUseGhostInReconstruction = this->useGhostInReconstruction;
	    this->useGhostInReconstruction = false;
	    
	    UnstructuredMappingAdjacencyIterator aiter, aiter_end;
	    UnstructuredMapping::EntityTypeEnum ntype = (rDim==3 ? UnstructuredMapping::Region : UnstructuredMapping::Face);
	    //UnstructuredMapping::EntityTypeEnum ntype = UnstructuredMapping::Vertex;
	    aiter_end = umap.adjacency_end(UnstructuredMapping::Face,ei,ntype);
	    
	    int na=0;

	    UnstructuredMappingIterator iter=umap.begin(UnstructuredMapping::Face);
	    iter.setLocation(ei);
	    
	    ArraySimple< UnstructuredMappingAdjacencyIterator > adj;
	    
	    int nh=unstructuredLink(umap, adj, iter, 1, 4, ntype);
	    bool foundInterior = !adj[0].isGhost() && !adj[0].isBC();
	    real wsum = 0,w;
	    for ( int i=1; i<adj.size() && !foundInterior; i++ )
	      {
		foundInterior = !adj[i].isGhost() && !adj[i].isBC();
	      }
	    if ( !foundInterior )
	      {
		nh=unstructuredLink(umap, adj, iter, 4, 4, ntype);
		for ( int i=1; i<adj.size() && !foundInterior; i++ )
		  {
		    foundInterior = !adj[i].isGhost() && !adj[i].isBC();
		  }
		assert(foundInterior);
	      }
	    
	    if ( isProjection )
	      gf(ei,0,0,0) = 0.;
	    else
	      for ( int a=0; a<rDim; a++ )
		gf(ei,0,0,a) = 0.;
	    
	    bool foundUsable=false;
	    int navg = 0;
	    gf(ei,0,0,0) =  0;
	    for ( int ae=0; ae<adj.size() && !foundUsable; ae++ )
	      {
		UnstructuredMappingAdjacencyIterator &aif = adj[ae];
		if ( !aif.isGhost() && !aif.isBC() /*&& !umap.hasTag(UnstructuredMapping::Edge,*aif,"__bcnum Edge" )*/)
		  {
		    eext(0) = *aif;
		    if ( (foundUsable = reconstructDSIAtEntities( t, HField, eext, gf, fullField) ) )
		      {
			//			  cout<<fullField(0,0)<<"  "<<fullField(0,1)<<"  "<<fullField(0,2)<<endl;
			for ( int a=0; a<rDim; a++ )
			  gf(ei,0,0,0) += fullField(0,a)*faceAreaNormals(ei,0,0,a);
			navg++;
		      }
		  }
	      }
	    
	      if ( !foundUsable ) cout<<" WARNING : might not have found usable entity for extrapolation bc for Face"<<ei<<endl;
	      
	      this->useGhostInReconstruction = oldUseGhostInReconstruction;
	      
	  }
       else if ( umap.isGhost(UnstructuredMapping::Face,ei) && (bcopt & zeroBC ) )
	  {
	    if ( isProjection )
	      {
		gf(ei,0,0,0) = 0.;
	      }
	    else 
	      {
		for ( int a=0; a<rDim; a++ )
		  gf(ei,0,0,a) = 0.;
	      }
	    //	    cout<<"ZEROED "<<ei<<"  "<<gf(ei,0,0,0)<<endl;
	  }
       else if ( bci(i,1)==dirichlet && !(bcopt&forceExtrap) )
	  {
	    if ( forcingOption==twilightZoneForcing )
	      {
		assert( tz!=NULL );
		OGFunction & e = *tz;
		//		  int ei = *hOutside;
		real x0 = xcent(ei,0);
		real y0 = xcent(ei,1);
		real z0 = rDim==3 ? xcent(ei,2) : 0.;
		if ( isProjection )
		  {
		    if ( rDim==2 )
		      gf(ei,0,0,0) = /*areas(ei,0,0)*/( e(x0,y0,z0,0,t) );
		    else
		      gf(ei,0,0,0) =
			 ( e(x0,y0,z0,0,t)*areaNorms(ei,0,0,0)
			   +e(x0,y0,z0,1,t)*areaNorms(ei,0,0,1)
			   +e(x0,y0,z0,2,t)*areaNorms(ei,0,0,2));
		  }
		else if ( !isProjection )
		  {
		    for ( int a=0; a<rDim; a++ )
		      gf(ei,0,0,a) = e(x0,y0,z0,a,t);
		  }
	      }
	  }
	  
	}

    if ( false && bcopt&forceExtrap )
      {
	cout<<"MAXGF "<<maxgf<<endl;
	cout<<"MINGF "<<mingf<<endl;
      }

    // // //
    if ( isProjection ) // other timings will be included in the setup and plotting times
      timing(timeForBoundaryConditions) += getCPU()-tm0;

  }

void Maxwell::
applyDSIForcing( realMappedGridFunction &gf, real t, real dt, bool isEField, bool isProjection )
{
  bool computeForcing = (forcingOption==twilightZoneForcing || 
			 forcingOption==gaussianSource ||
			 forcingOption==magneticSinusoidalPointSource);
  
  if( !computeForcing )
    return ;
  
  real tm0 = getCPU();
  
  MappedGrid & mg = *gf.getMappedGrid();
  int numberOfDimensions = mg.numberOfDimensions();
  
  assert( mg.getGridType()==MappedGrid::unstructuredGrid );
  
  UnstructuredMapping & umap = (UnstructuredMapping &) mg.mapping().getMapping();
  
  int dDim = umap.getDomainDimension();
  int rDim = umap.getRangeDimension();
  
  const realArray &nodes = umap.getNodes();
  
  UnstructuredMapping::EntityTypeEnum efieldCent = UnstructuredMapping::Edge;
  UnstructuredMapping::EntityTypeEnum hfieldCent = UnstructuredMapping::Face;
  
  UnstructuredMapping::EntityTypeEnum cent = isEField ? efieldCent : hfieldCent;
  
  
  bool vCent = mg.isAllVertexCentered();
  
  const realArray &cFArea = vCent ? mg.centerArea() : mg.faceArea();
  const realArray &cFNorm = vCent ? mg.centerNormal() : mg.faceNormal();
  const realArray &cEArea = vCent ? mg.faceArea() : mg.centerArea();
  const realArray &cENorm = vCent ? mg.faceNormal() : mg.centerNormal();
  
  //    const realArray & norms = isEField ? cENorm : cFNorm;
  //    const realArray & areas = isEField ? cEArea : cFArea;
  const realArray & areaNorms = isEField ? edgeAreaNormals : faceAreaNormals;
  
  realMappedGridFunction f(mg);
  
  if ( isEField )
    f.updateToMatchGrid(mg,GridFunctionParameters::edgeCentered,numberOfDimensions);
  else
    {
      if ( numberOfDimensions==2 )
	{
	  f.updateToMatchGrid(mg,GridFunctionParameters::cellCentered);
	}
      else
	{
	  f.updateToMatchGrid(mg,GridFunctionParameters::faceCenteredAll,numberOfDimensions);
	}      
    }
  
  real timef = getCPU();
  getForcing( 0, 0, f, t, dt, (isEField ? 1 : 3) );
  timing(timeForForcing) += getCPU()-timef;
  
  if ( isProjection )
    { 
      if ( numberOfDimensions==2 && isEField )
	{
	  int i3=0;
	  for ( int i1=f.getBase(0); i1<=f.getBound(0); i1++ )
	    for ( int i2=f.getBase(1); i2<=f.getBound(1); i2++ )
	      //		for ( int i3=f.getBase(2); i3<=f.getBound(2); i3++ )
	      {
		gf(i1,i2,i3,0) += dt*( f(i1,i2,i3,0)*areaNorms(i1,i2,i3,0) +
				       f(i1,i2,i3,1)*areaNorms(i1,i2,i3,1) );
		
	      }
	}
      else if ( numberOfDimensions==2 )
	{
	  int i3=0;
	  for ( int i1=f.getBase(0); i1<=f.getBound(0); i1++ )
	    for ( int i2=f.getBase(1); i2<=f.getBound(1); i2++ )
	      gf(i1,i2,i3,0) += dt* f(i1,i2,i3,0);
	}
      else
	{
	  for ( int i1=f.getBase(0); i1<=f.getBound(0); i1++ )
	    for ( int i2=f.getBase(1); i2<=f.getBound(1); i2++ )
	      for ( int i3=f.getBase(2); i3<=f.getBound(2); i3++ )
		{
		  gf(i1,i2,i3,0) += dt* ( f(i1,i2,i3,0)*areaNorms(i1,i2,i3,0) +
					  f(i1,i2,i3,1)*areaNorms(i1,i2,i3,1) +
					  f(i1,i2,i3,2)*areaNorms(i1,i2,i3,2) );
		  
		  
		}
	}
      
    }
  else
    { // will this option ever be used? why would it?
      gf += dt*f;
    }
  
  //    if ( isProjection ) // other timings will be included in the setup and plotting times
  //      timing(timeForForcing) += getCPU()-tm0;
  
}
