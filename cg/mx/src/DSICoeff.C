//#define BOUNDS_CHECK
//#define OV_DEBUG

#include "UnstructuredGeometry.h"

#include "Maxwell.h"

#include "fia.h" // fortran interface macros
#include "ULink.h"
#include "Geom.h"

namespace {
  const bool new2d =true;
  const bool globalLSR =true;
  const bool dbgLSR = false;
  const bool outputErr = true;
  const bool cacheCoeff = false;
  const bool cacheRCoeff = true;
  const bool useModifiedStencil = true;
  const bool buildDissipationOperator = false;

  bool useGhostInReconstruction = true;
  const bool useCleanLSR = true;
  const bool useUnitNormals = true;
  const bool weightByDistance = true;
  const bool weightByNormal = false;
  const bool useCenteredSet = false;
  const bool filterOutParallel = false;
  const bool normalizeLSRDX = true;
  int maxSample = 20;

  real adjSearchTime;
  real lscTime;
  real extcTime;
  real extcTime_1;
  real extcTime_2;
  real csetTime;

  real minRC, maxRC;
  int minNGB, maxNGB;
  int extcCalls;
  
  int nsearch,nhops,minEqs;
  UnstructuredMapping::EntityTypeEnum hAdj, eAdj;
}

#define LSCOEFF EXTERN_C_NAME(lscoeff)

extern int 
getCenters( MappedGrid &mg, UnstructuredMapping::EntityTypeEnum cent, realArray &xe );

extern "C" void LSCOEFF ( 
				     F90_REAL8 *a,
				     F90_INTEGER *m,
				     F90_INTEGER *n,
				     F90_REAL8 *qri,
				     F90_REAL8 *rcond,
				     F90_REAL8 *wrk,
				     F90_INTEGER *ierr );

extern "C" void F90_ID(dgefa,DGEFA) ( // linpac gaussian elimination
				     F90_REAL8 *a,
				     F90_INTEGER *lda,
				     F90_INTEGER *n,
				     F90_INTEGER *ipvt,
				     F90_INTEGER *info );

extern "C" void F90_ID(dgeco,DGECO) ( // linpac gaussian elimination
				     F90_REAL8 *a,
				     F90_INTEGER *lda,
				     F90_INTEGER *n,
				     F90_INTEGER *ipvt,
				     F90_REAL8 *rcond,
				     F90_REAL8 *z);

extern "C" void F90_ID(dgedi,DGEDI) ( // linpac inverse from dgefa output
				     F90_REAL8 *a,
				     F90_INTEGER *lda,
				     F90_INTEGER *n,
				     F90_INTEGER *ipvt,
				     F90_REAL8 *det,
				     F90_REAL8 *work,
				     F90_INTEGER *job );


extern "C" void F90_ID(amubdg,AMUBDG) ( // sparsekit routine to count the nonzeros in a matrix - matrix multiply (CSR format)
				       F90_INTEGER *nrow,
				       F90_INTEGER *ncol,
				       F90_INTEGER *ncolb,
				       F90_INTEGER *ja,
				       F90_INTEGER *ia,
				       F90_INTEGER *jb,
				       F90_INTEGER *ib,
				       F90_INTEGER *ndegr,
				       F90_INTEGER *nnz,
				       F90_INTEGER *iw );
	
extern "C" void F90_ID(amub,AMUB) ( // sparsekit matrix-matrix multiply with CSR format arrays
				   F90_INTEGER *nrow,
				   F90_INTEGER *ncol,
				   F90_INTEGER *job,
				   F90_REAL8 *a,
				   F90_INTEGER *ja,
				   F90_INTEGER *ia,
				   F90_REAL8 *b,
				   F90_INTEGER *jb,
				   F90_INTEGER *ib,
				   F90_REAL8 *c,
				   F90_INTEGER *jc,
				   F90_INTEGER *ic,
				   F90_INTEGER *nzmax,
				   F90_INTEGER *iw,
				   F90_INTEGER *ierr);

extern "C" void F90_ID(apmbt,APMBT) ( // sparsekit A +/- B^T
				   F90_INTEGER *nrow,
				   F90_INTEGER *ncol,
				   F90_INTEGER *job,
				   F90_REAL8 *a,
				   F90_INTEGER *ja,
				   F90_INTEGER *ia,
				   F90_REAL8 *b,
				   F90_INTEGER *jb,
				   F90_INTEGER *ib,
				   F90_REAL8 *c,
				   F90_INTEGER *jc,
				   F90_INTEGER *ic,
				   F90_INTEGER *nzmax,
				   F90_INTEGER *iw,
				   F90_INTEGER *ierr);

extern "C" void F90_ID(amux,AMUX) ( // sparsekit matrix-vector multiply with CSR format arrays
				   F90_INTEGER *nrow,
				   F90_REAL8 *v,
				   F90_REAL8 *vo,
				   F90_REAL8 *a,
				   F90_INTEGER *ja,
				   F90_INTEGER *ia);

extern "C" void F90_ID(apmbt,APMBT) ( 
				   F90_INTEGER *nrow,
				   F90_INTEGER *ncol,
				   F90_INTEGER *job,
				   F90_REAL8 *a,
				   F90_INTEGER *ja,
				   F90_INTEGER *ia,
				   F90_REAL8 *b,
				   F90_INTEGER *jb,
				   F90_INTEGER *ib,
				   F90_REAL8 *c,
				   F90_INTEGER *jc,
				   F90_INTEGER *ic,
				   F90_INTEGER *nzmax,
				   F90_INTEGER *iw,
				   F90_INTEGER *ierr);

extern "C" void F90_ID(pspltm,PSPLTM) ( // sparsekit matrix plotter
				       F90_INTEGER *nrow,
				       F90_INTEGER *ncol,
				       F90_INTEGER *mode,
				       F90_INTEGER *ja,
				       F90_INTEGER *ia,
				       char *title,
				       F90_INTEGER *ptitle,
				       F90_REAL *size,
				       char *munt,
				       F90_INTEGER *nlines,
				       F90_INTEGER *lines,
				       F90_INTEGER *iunt,int,int);

extern "C" void F90_ID(dump,dump) ( // sparsekit matrix plotter
				       F90_INTEGER *nrow1,
				       F90_INTEGER *nrow2,
				       F90_LOGICAL *values,
				       F90_REAL8    *a,
				       F90_INTEGER *ja,
				       F90_INTEGER *ia,
				       F90_INTEGER *iout);

extern "C" void F90_ID(aplbdg,APLBDG) ( // sparsekit count of nonzeros in A+B for CSR format matrices
				       F90_INTEGER *nrow,
				       F90_INTEGER *ncol,
				       F90_INTEGER *ja,
				       F90_INTEGER *ia,
				       F90_INTEGER *jb,
				       F90_INTEGER *ib,
				       F90_INTEGER *ndegr,
				       F90_INTEGER *nnz,
				       F90_INTEGER *iw );

extern "C" void F90_ID(aplb,APLB) ( // sparsekit A+B for CSR format matrices
				   F90_INTEGER *nrow,
				   F90_INTEGER *ncol,
				   F90_INTEGER *job,
				   F90_REAL8 *a,
				   F90_INTEGER *ja,
				   F90_INTEGER *ia,
				   F90_REAL8 *b,
				   F90_INTEGER *jb,
				   F90_INTEGER *ib,
				   F90_REAL8 *c,
				   F90_INTEGER *jc,
				   F90_INTEGER *ic,
				   F90_INTEGER *nzmax,
				   F90_INTEGER *iw,
				   F90_INTEGER *ierr);

extern "C" void F90_ID(filter,FILTER) ( 
				       F90_INTEGER *nrow, 
				       F90_INTEGER *job, 
				       F90_REAL8 *dtol, 
				       F90_REAL8 *a, 
				       F90_INTEGER *ja, 
				       F90_INTEGER *ia,
				       F90_REAL8 *b, 
				       F90_INTEGER *jb, 
				       F90_INTEGER *ib, 
				       F90_INTEGER *len, 
				       F90_INTEGER *ierr );

extern "C" void F90_ID(f90_fopen,F90_FOPEN)(F90_INTEGER *iunt, char *fname, int);
extern "C" void F90_ID(f90_fclose,F90_FCLOSE)(F90_INTEGER *iunt);


namespace {

  typedef ArraySimpleFixed<real,3,1,1,1> V3D;
  int intersect3DLines(const V3D &P0, const V3D &T0, const V3D& P2, const V3D &T2,
		       real &a0, real &a2,
		       V3D &P1)
  {
    // This is a utility routine required by several of P&T's algorithms.
    //  It returns the intersection of two 3D lines defined by points and tangents.
    //  NOTE: the return value is 1 if the lines are parallel and zero otherwise.

    // Basically we need to solve
    //  P0 + a0*T0 = P1
    //  P2 + a2*T2 = P1
    //  i.e., the system 
    //  a2*T2 - a0*T0 = P0 - P2
    //
    //  [         ]{ a0 }
    //  [ -T0  T2 ]{    } = P0 - P2
    //  [         ]{ a2 } 

    //  - or -
    //  A a = b, with A 2x3 
    //  solve using least squares
    //  A^TA a = A^T b

    ArraySimpleFixed<real, 3,3,1,1> ata;
    
    ata(0,0) = T0[0]*T0[0] + T0[1]*T0[1] + T0[2]*T0[2];
    ata(0,1) = ata(1,0) = -T0[0]*T2[0] - T0[1]*T2[1] - T0[2]*T2[2];
    ata(1,1) = T2[0]*T2[0] + T2[1]*T2[1] + T2[2]*T2[2];
    
    real det = ata(0,0)*ata(1,1) - ata(1,0)*ata(0,1);
    if ( fabs(det)<REAL_EPSILON )
      return 1;

    real b[] = {0., 0.};
    for ( int a=0; a<3; a++ )
      {
	b[0] -= T0[a] * (P0[a]-P2[a]);
	b[1] += T2[a] * (P0[a]-P2[a]);
      }

    a0 = (ata(1,1)*b[0] - ata(0,1)*b[1])/det;
    a2 = (ata(0,0)*b[0] - ata(1,0)*b[1])/det;

    for ( int a=0; a<3; a++ )
      P1[a] = P0[a] + a0*T0[a];

    return 0;
  }

  void determineCenteredSet( const int rDim,
			     const UnstructuredMappingIterator &iter, 
			     const ArraySimple<UnstructuredMappingAdjacencyIterator> &adjEnts,
			     const realArray &verts,
			     const intArray &entities,
			     const realArray &normal,
			     ArraySimple<bool> &used, real filt_tol=0.5 )
  {
    real t0 = getCPU();

    used.resize( adjEnts.size() );
    used = true;

     if ( iter.isGhost() )
       {
 	for ( int i=maxSample; i<adjEnts.size(); i++ )
 	  used[i] = false;
 	return;
       }

    ArraySimpleFixed<real,3,1,1,1> xec, xaec;
    xec=xaec=0.;
    int nv=0;
    int ecur = *iter;
    for ( int v=0; v<entities.getLength(1) && entities(ecur,v)>-1; v++ )
      {
	const int vidx = entities(ecur,v);
	for ( int a=0; a<rDim; a++ )
	  xec[a] += verts(vidx,a);
	nv++;
      }

    for ( int a=0; a<rDim; a++ )
      xec[a] /= real(nv);

    //    cout<<"xec = "<<xec[0]<<"  "<<xec[1]<<"  "<<xec[2]<<endl;
    ArraySimpleFixed<int,2,2,2,1> bcount;
    bcount=0;

    real n1mag=0;
    for ( int a=0; a<rDim; a++ )
      n1mag += normal(ecur,0,0,a)*normal(ecur,0,0,a);

    int nskipped = 0;
    int nkept = 0;
    //    real filt_tol = .1;
    used = false;
    real fto = filt_tol;
    while ( nkept<minEqs && filt_tol>(fto*1e-4) )
      {
	nskipped = 0;
	bcount = 0;
	for ( int e=0; e<adjEnts.size(0) ; e++ )
	  {
	    int ae = *adjEnts(e);
	    
	    
	    if ( ae!=ecur && !used(e) &&
		 ( useGhostInReconstruction || ( !(adjEnts(e).isBC() || adjEnts(e).isGhost())) || iter.isGhost() ) )
	      {
		int nv=0;
		xaec = 0;
		real ndot=0,n2mag=0;
		for ( int a=0; a<rDim; a++ )
		  {
		    ndot += normal(ae,0,0,a)*normal(ecur,0,0,a);
		    n2mag += normal(ae,0,0,a)*normal(ae,0,0,a);
		  }
		
		real filt = fabs(1.-fabs(ndot)/sqrt(n2mag*n1mag));
		
		//	    if ( filt<REAL_EPSILON )
		//	      cout<<"would have discarded "<<ae<<" : "<<filt<<"  "<<ndot<<"  "<<n2mag<<"  "<<n1mag<<endl;
		
		if (  ((!filterOutParallel) || filt>=filt_tol) && nkept<=maxSample )
		  {
		    for ( int v=0; v<entities.getLength(1) && entities(ae,v)>-1; v++ )
		      {
			const int vidx = entities(ae,v);
			for ( int a=0; a<rDim; a++ )
			  xaec[a] += verts(vidx,a);
			nv++;
		      }
		    
		    ArraySimpleFixed<int,3,1,1,1> ix;
		    ix = 0;
		    for ( int a=0; a<rDim; a++ )
		      {
			real x = xaec[a]/real(nv);
			xaec[a] = x;
			ix[a] = (x>xec[a]) ? 1 : 0;
		      }
		    //	    cout<<"xaec("<<ae<<") = "<<xaec[0]<<"  "<<xaec[1]<<"  "<<xaec[2]<<endl;
		    //	    cout<<"ix("<<ae<<") = "<<ix[0]<<"  "<<ix[1]<<"  "<<ix[2]<<endl;
		    bcount(ix[0],ix[1],ix[2])++;
		    used(e)  = true;
		    nkept++;
		  }
		else
		  {
		    used(e) = false;
		    nskipped++;
		  }
	      }
	    else
	      nskipped++;
	  }
	filt_tol /= 2.;
      }

    int minb=INT_MAX, maxb=0;
    int ib = int(ceil(pow(2,rDim)));
    for ( int a=0; a<ib; a++ )
      {
	minb = min(minb, bcount[a]);
	maxb = max(maxb, bcount[a]);
      }
    //    assert(minb>=rDim);
    bool useAll = !useCenteredSet;

//     if ( dbgLSR && minb<rDim && !iter.isGhost() )
//       {
// 	cout<<"WARNING : REDUCED QUALITY INTERP AT "<<ecur<<", only "<<minb<<" in one region, nadj = "<<adjEnts.size()<<endl;
// 	cout<<bcount<<endl;
//       }
//     else 
    if ( minb<rDim || iter.isGhost() )
      {
	useAll = true;
	//	used = true;
	if ( dbgLSR && !iter.isGhost() )
	  {
	    cout<<"WARNING : REDUCED QUALITY INTERP AT "<<UnstructuredMapping::EntityTypeStrings[iter.getType()]<<"  "<<ecur<<", only "<<minb<<" in one region, nadj = "<<adjEnts.size()<<endl;
	    cout<<bcount<<endl;
	  }
      }

    bcount=0;
    for ( int e=0; e<adjEnts.size(0) && !useAll; e++ )
      {
	int ae = *adjEnts(e);
	
	
	if ( ae!=ecur && used(e) )
	  {
	    int nv=0;
	    xaec = 0;
	    for ( int v=0; v<entities.getLength(1) && entities(ae,v)>-1; v++ )
	      {
		const int vidx = entities(ae,v);
		for ( int a=0; a<rDim; a++ )
		  xaec[a] += verts(vidx,a);
		nv++;
	      }
	    
	    ArraySimpleFixed<int,3,1,1,1> ix;
	    ix = 0;
	    for ( int a=0; a<rDim; a++ )
	      {
		real x = xaec[a]/real(nv);
		xaec[a] /= real(nv);
		ix[a] = (x>xec[a]) ? 1 : 0;
	      }
	    if ( minb>(rDim) && bcount(ix[0],ix[1],ix[2])>=rDim ) 
	      {
		used(e)=false;
		nskipped++;
	      }
	    bcount(ix[0],ix[1],ix[2])++;
	  }
      }

    csetTime += getCPU()-t0;

    return;
  }

  bool computeDSIReconstructionCoefficients( UnstructuredMapping &umap, 
					     UnstructuredMappingIterator &iter, 
					     ArraySimple<UnstructuredMappingAdjacencyIterator> &adjEnts,
					     realArray &normal, 
					     ArraySimple<real> &coeff, 
					     ArraySimple<int> &index, int &nnz, int perImage=-1 )
  {

    real t0 = getCPU();

    assert(adjEnts.size(0));
    assert(globalLSR);

    int rDim = umap.getRangeDimension();

    UnstructuredMapping::EntityTypeEnum projType = iter.getType();
    const intArray &entities = umap.getEntities(projType);
    const realArray &verts = umap.getNodes();
    ArraySimple<bool> used(adjEnts.size()); used=true;
    determineCenteredSet( rDim, iter, adjEnts, verts, entities, normal, used );

    int neqFound = 0;
    
    int nDeg = 1; //linear reconstruction
    int nEqg = adjEnts.size(0);//+nEQ2Add; // initial guess
    int nTerm = rDim==2 ? 3:4;
    int Pcols = rDim*nTerm - 1;
    ArraySimple<real> P(nEqg, Pcols),wgts(nEqg);
    
    if ( nEqg>index.size(0)+1 ) 
      {
	index.resize(nEqg+1);
	coeff.resize(nEqg,coeff.size(1));
      }

    nnz=0;
    coeff = 0.;
    index = -1;
    int ecur = *iter;
    index(nnz) = ecur;
    nnz++;

    ArraySimpleFixed<real,3,1,1,1> n_k, n_j; // n_k is the normal corresponding to iter, n_j are the adjacent normals
    n_k = n_j = 0.;
    int maxDim=0;
    real maxNorm=0.;
    // find the maximum normal component
    for ( int a=0; a<rDim; a++ )
      {
	n_k[a] = normal(ecur,0,0,a);
	if ( fabs(n_k[a])>maxNorm )
	  {
	    maxNorm = fabs(n_k[a]);
	    maxDim = a;
	  }
      }

    real mag_n_k = sqrt(ASmag2(n_k));

    if ( useUnitNormals )
      for ( int a=0; a<rDim; a++ )
	n_k[a] /= mag_n_k;

    ArraySimpleFixed<real,3,1,1,1> xc_k, xc_j; // xc_k is the average vertex coordinate (center) corresponding to iter
                                               // xc_j stores the center coordinate for neighbors
    int nverts_k = 0;
    xc_k = xc_j = 0.;
    for ( int v=0; v<entities.getLength(1) && entities(ecur,v)>-1; v++,nverts_k++ )
      for ( int a=0; a<rDim; a++ )
	xc_k[a] += verts(entities(ecur,v),a);

    for ( int a=0; a<rDim; a++ )
      xc_k[a] /= real(nverts_k);
    
    real lscale=1;
    if ( normalizeLSRDX )
      {
	lscale = 0;
	for ( int v=1; v<entities.getLength(1) && entities(ecur,v)>-1; v++)
	  {
	    real l=0;
	    for ( int a=0; a<rDim; a++ )
	      l+=(verts(entities(ecur,v),a)-verts(entities(ecur,v-1),a))*(verts(entities(ecur,v),a)-verts(entities(ecur,v-1),a));
	    
	    lscale = max(lscale,l);
	  }
	
	lscale = sqrt(lscale);
      }

    // now loop through the adjacent entities and construct P, the matrix we will use for the LS system
    //     note that the gaussian elimination of the row "above" P in the full system is folded into this computation
    //     i.e. P does not contain the row corresponding to ecur
    real maxP = 0;
    real minP = REAL_MAX;
    bool okInterp = false;
    real rcond = 0;

    ArraySimple<real> Pfilt;
    ArraySimple<bool> zeroedCol(Pcols);
    ArraySimple<real> qri,Pinv;
    ArraySimple<real> wrk;
    int ierr=0;
    int nzeroed = 0;


    int adjE;
    int ntries=0;
    int curr_nhops = nhops;
    real filt_tol = 0.5;
    real rctol = 1e-5;
    int oldMaxSample=maxSample;
    for ( adjE=0; adjE<adjEnts.size(0) && !okInterp; adjE++ )
      {
	if ( used(adjE) )
	  {
	    if ( !useGhostInReconstruction && !iter.isGhost() ) assert(!adjEnts[adjE].isGhost());
	    UnstructuredMappingAdjacencyIterator &adj = adjEnts[adjE];
	    int eadj = *adj;
	    assert(eadj!=ecur);
	    
	    index(nnz) = eadj;
	    nnz++;

	    xc_j=n_j=0.;
	    for ( int a=0; a<rDim; a++ )
	      {
		n_j[a] = normal(eadj,0,0,a);

		int nv=0;
		for ( int v=0; v<entities.getLength(1) && entities(eadj,v)>-1; v++,nv++ )
		  xc_j[a] += verts(entities(eadj,v),a);
		
		xc_j[a] /= real(nv);

	      }

	    real mag_n_j = sqrt(ASmag2(n_j));
	    if ( useUnitNormals )
	      for ( int a=0; a<rDim; a++ )
		n_j[a] /= mag_n_j;

	    // add the row to P
	    real w = 1;
	    if ( weightByDistance )
	      {
		w=0.;
		for ( int a=0; a<rDim; a++ )
		  w += (xc_j[a]-xc_k[a])*(xc_j[a]-xc_k[a])/(lscale*lscale);

		w = 1./w;
	      }

	    if ( weightByNormal )
	      {
		real ndot=0;
		for ( int a=0; a<rDim; a++ )
		  ndot += n_j[a]*n_k[a];
		if ( !useUnitNormals )
		  ndot /= mag_n_j*mag_n_k;
		
		w = w*(1.-ndot);
	      }

	    //	    if ( neqFound==4 && ecur==5915) w = 10000*w;

	    wgts(neqFound) = w;
// 	    if ( ecur==5915 && iter.getType()==UnstructuredMapping::Edge)
// 	      {
// 		cout<<"for Edge "<<ecur<<"  :  neighb = "<<eadj<<" : w = "<<w<<" : wgts(neqFound) = "<<wgts(neqFound)<<endl;
// 	      }

	    for ( int a=0; a<rDim; a++ )
	      {
		int dblk = ( maxDim+a )%rDim;
		for ( int t=1; t<nTerm; t++ )
		  {
		    P(neqFound, t+a*nTerm-1) = w * (n_k[maxDim]*n_j[dblk]*(xc_j[t-1]-xc_k[t-1])/lscale);
		    maxP = max(maxP,fabs(P(neqFound, t+a*nTerm-1)));
		    if (fabs(P(neqFound,t+a*nTerm-1))) minP = min(minP,fabs(P(neqFound, t+a*nTerm-1)));
		  }

		if ( a>0 )
		  {
		    P(neqFound,a*nTerm-1) = w * (n_k[maxDim]*n_j[dblk] - n_k[dblk]*n_j[maxDim]);
		    maxP = max(maxP,fabs(P(neqFound,a*nTerm-1)));
		    if (fabs(P(neqFound,a*nTerm-1))) minP = min(minP,fabs(P(neqFound, a*nTerm-1)));
		  }
	      }

	    neqFound++;

	    if ( neqFound>=minEqs )
	    {
	      zeroedCol = false;
	      nzeroed=0;
	      for ( int c=0; c<Pcols; c++ )
		{
		  bool hasnzero = false;
		  for ( int r=0; r<neqFound && !hasnzero; r++ )
		    hasnzero = ((fabs(P(r,c))/maxP)>(100*REAL_EPSILON));
		  //	  hasnzero = ((fabs(P(r,c))+maxP)!=maxP);
		  //hasnzero = ((fabs(P(r,c))/maxP)>(100*REAL_EPSILON));
		  
		  if ( !hasnzero )
		    {
		      zeroedCol(c) = true;
		      nzeroed++;
		      //	    for ( int cc=icol; cc<Pcols-1; cc++ )
		      //	      for ( int r=0; r<neqFound; r++ )
		      //		P(r,cc) = P(r,cc+1);
		    }
		}
	      
	      int ncol = Pcols - nzeroed;
	      
	      //    if ( nzeroed )
	      {
		//	cout<<"nzeroed is "<<nzeroed<<", "<<ecur<<endl;
		Pfilt.resize(neqFound,ncol);
		int cc=0;
		for ( int c=0; c<Pcols; c++ )
		  if ( !zeroedCol(c) )
		    {
		      for ( int i=0; i<neqFound; i++ )
			Pfilt(i,cc) = P(i,c);
		      cc++;
		    }
		
		assert(cc==ncol);
	      }
	      
	      assert(neqFound>=ncol);
	      //    ArraySimple<real> &Psend_to_lscoeff = nzeroed ? Pfilt : P;
	      ArraySimple<real> &Psend_to_lscoeff =  Pfilt;
	      
	      wrk.resize(neqFound,neqFound+1 );
	      wrk = 0.;
	      // qri is P^{-1}, the pseudo-inverse of P
	      qri.resize(ncol,neqFound);
	      qri = 0.;
	      
	      ierr=0;
	      real t0l = getCPU();
	      LSCOEFF( Psend_to_lscoeff.ptr(), &neqFound, &ncol, qri.ptr(), &rcond, wrk.ptr(), &ierr);
	      lscTime += getCPU()-t0l;
	      real t02 = getCPU();

	      okInterp = (!ierr && rcond>(rctol));

	    }
	
	  }
	if ( adjE==(adjEnts.size()-1) && !okInterp && (ntries < 2) )
	  {
	    //	    cout<<"  :  try = "<<ntries<<" : rcond = "<<rcond<<" : "<<neqFound<<" : nadj = "<<adjEnts.size()<<endl;

	    if (neqFound>=maxSample) 
	      {
		maxSample+=10;
	      }
	    int ns = (iter.isGhost() || !::useGhostInReconstruction)? 2*nsearch : nsearch;
	    bool ug = true;//iter.isGhost() || useGhostInReconstruction;
	    
	    curr_nhops++;
	    if ( projType==UnstructuredMapping::Face )
	      unstructuredLink(umap, adjEnts, iter, curr_nhops, ns, hAdj,ug );
	    else
	      unstructuredLink(umap, adjEnts, iter, curr_nhops, ns, eAdj,ug );
	    
	    filt_tol /= 2.;
	    determineCenteredSet( rDim, iter, adjEnts, verts, entities, normal, used, filt_tol );
	    

	    nEqg = adjEnts.size();
	    assert(nEqg);
	    if ( nEqg>index.size(0)-1 ) 
	      {
		index.resize(nEqg+1);
		coeff.resize(nEqg,coeff.size(1));
	      }

	    index = -1;

	    adjE=-1;
	    neqFound = 0;
	    nnz = 0;
	    index(nnz) = ecur;
	    nnz++;

	    //	    coeff = 0.;
	    P.resize(nEqg, Pcols);
	    wgts.resize(nEqg);
	    ntries++;
	  }
      }

    maxSample=oldMaxSample;
    maxRC = max(rcond,maxRC);
    minRC = min(rcond,minRC);
    maxNGB = max(maxNGB, neqFound);
    minNGB = min(minNGB, neqFound);

    if ( !okInterp && outputErr ) cout<<"RCOND = "<<rcond<<", tol = "<<(rctol)<<endl;
    if ( !okInterp && outputErr ) cout<<"WARNING : possible poor interpolation : rcond = "<<rcond<<" : neqf = "<<neqFound<<"  : nadj = "<<adjEnts.size()<<" : ntries = "<<ntries<<" : filt_tol = "<<filt_tol<<endl;

    extcTime_1 += getCPU()-t0;

    for ( int e=adjE; e<used.size(); e++ )
      used[e] = false;

#ifdef REDO_LSCOEFF
    // remove zeroed columns
    ArraySimple<bool> zeroedCol(Pcols);
    zeroedCol = false;
    int nzeroed=0;
    for ( int c=0; c<Pcols; c++ )
      {
	bool hasnzero = false;
	for ( int r=0; r<neqFound && !hasnzero; r++ )
	  hasnzero = ((fabs(P(r,c))/maxP)>(10*REAL_EPSILON));
	//	  hasnzero = ((fabs(P(r,c))+maxP)!=maxP);
	//hasnzero = ((fabs(P(r,c))/maxP)>(100*REAL_EPSILON));

	if ( !hasnzero )
	  {
	    zeroedCol(c) = true;
	    nzeroed++;
	    //	    for ( int cc=icol; cc<Pcols-1; cc++ )
	    //	      for ( int r=0; r<neqFound; r++ )
	    //		P(r,cc) = P(r,cc+1);
	  }
      }

    int ncol = Pcols - nzeroed;

    ArraySimple<real> Pfilt;
    //    if ( nzeroed )
      {
	//	cout<<"nzeroed is "<<nzeroed<<", "<<ecur<<endl;
	Pfilt.resize(neqFound,ncol);
	int cc=0;
	for ( int c=0; c<Pcols; c++ )
	  if ( !zeroedCol(c) )
	    {
	      for ( int i=0; i<neqFound; i++ )
		Pfilt(i,cc) = P(i,c);
	      cc++;
	    }

	assert(cc==ncol);
      }

      assert(neqFound>=ncol);
      //    ArraySimple<real> &Psend_to_lscoeff = nzeroed ? Pfilt : P;
    ArraySimple<real> &Psend_to_lscoeff =  Pfilt;

    ArraySimple<real> wrk( neqFound,neqFound+1 );
    wrk = 0.;
    // qri is P^{-1}, the pseudo-inverse of P
    ArraySimple<real> qri( ncol, neqFound ),Pinv(Pcols,neqFound);
    qri = 0.;
    
    int ierr=0;
    real t0l = getCPU();
    LSCOEFF ( Psend_to_lscoeff.ptr(), &neqFound, &ncol, qri.ptr(), &rcond, wrk.ptr(), &ierr);
    lscTime += getCPU()-t0l;
#endif
    real t02 = getCPU();

    if ( ierr && outputErr )
      {
	cout<<"rcond = "<<rcond<<endl;
	cout<<"maxP = "<<maxP<<endl;
	cout<<"minP = "<<minP<<endl;
	cout<<"maxP+minP==maxP : "<<((maxP+minP)==maxP)<<endl;
	cout<<"minP/maxP = "<<minP/maxP<<endl;
	cout<<"minP/maxP < 10*REAL_EPSILON "<<((minP/maxP) < 10*REAL_EPSILON)<<endl;
	cout<<"P was rank deficient for the LS problem!"<<endl;
	cout<<"ENTITY WAS "<<ecur<<endl;
	cout<<"ENTITY TYPE IS "<<iter.getType()<<endl;
	cout<<"NEQS "<<neqFound<<endl;
	cout<<"isghost "<<iter.isGhost()<<endl;
	cout<<"ZEROED was "<<nzeroed<<endl;
	cout<<"maxP was "<<maxP<<endl;
	//cout<<Pfilt<<endl;
	for ( int r=0; r<Pfilt.size(0); r++ )
	  {
	    cout<<"[ ";
	    for ( int c=0; c<Pfilt.size(1); c++ )
	      {
		cout<<Pfilt(r,c)<<"  ";
	      }
	    cout<<" ] "<<endl;
	  }
	for ( int i=0; i<adjE; i++ )
		  if ( used[i] ) cout<<UnstructuredMapping::EntityTypeStrings[iter.getType()]<<"  "<<*adjEnts[i]<<endl;
      }
    
    // now expand out zeroed columns
    Pinv.resize(Pcols,neqFound);
    Pinv = 0;
    int rc=0;
    for ( int r=0; r<Pcols; r++ )
      if ( !zeroedCol(r) )
	{
	  for ( int c=0; c<neqFound; c++ )
	    Pinv(r,c) = qri(rc,c);
	  rc++;
	}

    // now compute the full coefficient array, R (we may use it sometime to look at derivatives)
    ArraySimple<real> R(rDim*nTerm,nnz);
    R=0;

    // reuse the wrk array to store some usefull stuff needed to compute R
    wrk=0.;
    // wrk(:,0) will store n^TP^-1
    // wrk(:,1) will store P^{-1}m

    // n is Pcols long, n={0 0 ny 0 0 nz 0 0]
    // m is neqFound long
    // P^{-1} is Pcols x neqFound
    // wrk(:,0) will store neqFound terms
    // wrk(:,1) will store Pcols terms
    real ntPmm =0.; // this is n^T P{-1} m

    for ( int r=0; r<neqFound; r++ )
      for ( int a=1; a<rDim; a++ ) // for each block of n that starts with a nonzero
	wrk(r,0) += n_k[(maxDim+a)%rDim]*wgts(r)*Pinv(a*nTerm-1,r);

    int r=0;
    for ( int e=0; e<adjE /*adjEnts.size(0)*/; e++ )
      {
	ArraySimpleFixed<real,3,1,1,1> n;
	n =0.;
	for ( int a=0; a<rDim; a++ )
	  n[a] = normal(*adjEnts(e),0,0,a);

	if ( useUnitNormals )
	  {
	    real magn = sqrt(ASmag2(n));
	    for ( int a=0; a<rDim; a++ )
	      n[a] /= magn;
	  }

	if ( used(e) )
	  {
	    real n1j = n[maxDim];//normal(*adjEnts(e),0,0,maxDim);
	    for ( int c=0; c<Pcols; c++ )
	      wrk(c,1) += Pinv(c,r)*wgts(r)*n1j;
	    ntPmm += wrk(r,0)*n1j;
	    r++;
	  }
      }
    
//     for ( int a=1; a<rDim; a++ )
//       ntPmm += n_k[(maxDim+a)%rDim]*wrk(a*nTerm-1,1);

    // now fill in R
    // upper left corner
    R(0,0) = (1.+ntPmm)/n_k[maxDim];
    
    // top row, entries 1..nnz
    for ( int i=0; i<neqFound; i++ )
      R(0,i+1) = -wrk(i,0);

    // left column, entries 1..Pcols+1
    for ( int i=0; i<Pcols; i++ )
      R(i+1,0) = -wrk(i,1);

    // and the rest is just n_k[maxDim] P^{-1}
    for ( int j=0; j<neqFound; j++ )
      for ( int i=0; i<Pcols; i++ )
	R(i+1,j+1) = n_k[maxDim]*Pinv(i,j)*wgts(j);

    if ( useUnitNormals )
      {
	for ( int j=0; j<nnz; j++ )
	  {
	    int e=index(j);
	    ArraySimpleFixed<real,3,1,1,1> n;
	    n =0.;
	    for ( int a=0; a<rDim; a++ )
	      n[a] = normal(e,0,0,a);

	    real mag_n = sqrt(ASmag2(n));

	    for ( int i=0; i<(Pcols+1); i++ )
	      R(i,j) /= mag_n;
	  }

      }

    // now the coefficients for evaluating U are 
    // [ v 0 0 ]
    // [ 0 v 0 ] R
    // [ 0 0 v ]
    // where v = [1, dx/lscale, dy/lscale, dz/lscale]^T
    // for U_k it is easy, v = [1,0,0,0]
    
    for ( int a=0; a<rDim; a++ )
      for ( int i=0; i<nnz; i++ )
	  coeff(i,(maxDim+a)%rDim) = R(a*nTerm,i);

    //     if ( ecur==5915 && iter.getType()==UnstructuredMapping::Edge)

    real t1 = getCPU();
    extcTime_2 += t1-t02;
    extcTime += t1-t0;
    extcCalls++;
    return okInterp;
  }

			     
  void extractDSIReconstructionCoefficients( UnstructuredMapping &umap, 
					     UnstructuredMappingIterator &iter, 
					     ArraySimple<UnstructuredMappingAdjacencyIterator> &adjEnts,
					     realArray &normal, 
					     ArraySimple<real> &coeff, 
					     ArraySimple<int> &index, int &nnz, int perImage=-1 )
  {
    cout<<"USING OLD RECONSTRUCTION CODE"<<endl;
    int rDim = umap.getRangeDimension();
    UnstructuredMapping::EntityTypeEnum projType = iter.getType();
    UnstructuredMapping::EntityTypeEnum cellType = UnstructuredMapping::EntityTypeEnum(umap.getDomainDimension());
    UnstructuredMapping::EntityTypeEnum faceType = UnstructuredMapping::Face;
    nnz=0;
    real wsum = 0.;

    const realArray &verts = umap.getNodes();

    coeff = 0;

    bool useLSR = globalLSR;// && !iter.isGhost();

    //    cout<<"useLSR, isGhost "<<useLSR<<"  "<<iter.isGhost()<<endl;
    if ( useLSR )//&& rDim==2 && projType==UnstructuredMapping::Edge)
      {
	// find the least squares approximation to 
	// [ n_x M | n_y M ] { g_x, g_y } = U
	// where n_x and n_y are vectors of the x and y components of normal at each edge
	// M is the basis matrix (currently the vandermonde matrix)
	// and g_x and g_y are the Taylor's series coefficients for E_x and E_y

	//	const intArray &edges = umap.getEntities(UnstructuredMapping::Edge);
	real t0=getCPU();

	const intArray &entities = umap.getEntities(projType);
	ArraySimple<bool> used(adjEnts.size()); used=true;
	determineCenteredSet( rDim, iter, adjEnts, verts, entities, normal, used );


	ArraySimple<real> Msub, Usub, R,RtRinv,wgts;
	ArraySimple<bool> zeroedCol;
	int neqFound = 0;
	int nEQ2Add = rDim*rDim;

	int nDeg = 1; //linear reconstruction
	int nEqg = adjEnts.size(0)+nEQ2Add; // initial guess
	int MsubCol = 1;
#if 0
	for ( int d=1; d<(nDeg+1); d++ )
	  MsubCol += (d+1);
#else
	MsubCol = rDim==2 ? 3 : 4;
#endif
	
	int nTerm = MsubCol;
	//	MsubCol = 2*nTerm -1;
	MsubCol = rDim*nTerm -1;
	Msub.resize(nEqg,MsubCol); // Msub is [ n_x M | n_y M ] below the first row 
	                           // (after the first row has been eliminated)
	
	wgts.resize(nEqg);
	wgts = 1.;

	assert(adjEnts.size(0));
	//	ArraySimple< UnstructuredMappingAdjacencyIterator > adjEnts;
	//	unstructuredLink(umap, adjEnts, iter, 1, rDim*nTerm);

	Msub = 0.;
	Usub.resize(nEqg);         // Usub is rhs vector after elimination of the first row
	zeroedCol.resize(MsubCol);
	zeroedCol = false;

	//	index.resize(nEqg+1);
	index = -1;

	int ecur =  *iter;

	index(nnz) = ecur; // nnz already initialized to zero above
	nnz++;

	ArraySimpleFixed<real,3,1,1,1> xec, ec_norm;
	xec=ec_norm=0.;
	real maxNorm=0;
	int maxDim = 0;
	// M is the Vandermonde matrix of order MsubCol in the dimensions x_i (x,y,z).
	//   Since we gaussian eliminate the first row and multiply the remaining equations by [ n_x M | n_y M ](0,0)
	//   to avoid divisions we would like to have [ n_x M | n_y M ](0,0) be non-zero.  To achieve this, we permute
	//   the order of the entries in [ n_x M | n_y M ] so that the maximum (0,0) value (coor direction with
	//   the max normal component) is in the first block.  For example, on a cartesian grid, the horizontal edges
	//   (whose y-normal component is zero) would look like [ n_x M | n_y M ] but the vertical edges would look
	//   like [ n_y M | n_x M ].
	//   
	for ( int a=0; a<rDim; a++ )
	  {
	    ec_norm[a] = normal(ecur,0,0,a);
	    if ( fabs(ec_norm[a])>maxNorm )
	      {
		maxNorm = fabs(ec_norm[a]);
		maxDim = a;
	      }
	  }

 	real ecmag = sqrt(ASmag2(ec_norm));
// 	for ( int a=0; a<rDim; a++ )
// 	  ec_norm[a] /= ecmag;

	int nv=0;
	for ( int v=0; v<entities.getLength(1) && entities(ecur,v)>-1; v++ )
	  {
	    for ( int a=0; a<rDim; a++ )
	      xec[a] += verts(entities(ecur,v),a);
	    nv++;
	  }

	for ( int a=0; a<rDim; a++ )
	  xec[a] /= real(nv);
	
	if ( dbgLSR && (*iter)==0 && iter.getType()==UnstructuredMapping::Face)
	  {
	    cout<<"xec "<<xec<<endl;
	    cout<<"ec_norm "<<ec_norm<<endl;
	    cout<<"maxDim "<<maxDim<<endl;
	    cout<<"nTerm "<<nTerm<<endl;
	    cout<<"nv "<<nv<<endl;
	  }
		
	real ecn_mag = sqrt(ASmag2(ec_norm));

	UnstructuredMappingAdjacencyIterator adj, adj_end;

	for ( int adjE = 0; adjE<adjEnts.size(0); adjE++ )
	  {
	    adj = adjEnts(adjE);
	    //	      cout<<*adj<<"  "<<*iter<<"  "<<(adj!=iter)<<endl;
	    if ( (*adj)!=(*iter) && used(adjE) )
	      {
		if (Msub.size(0)==(neqFound+nEQ2Add))
		  {
		    ArraySimple<real> mt;
		    mt = Msub;
		    Msub.resize(neqFound+10,MsubCol);
		    Msub = 0;
		    for ( int r=0; r<neqFound; r++ )
		      for ( int c=0; c<MsubCol; c++ )
			Msub(r,c) = mt(r,c);
		    
		    wgts.resize(neqFound+10);

		    //		      index.resize(nnz+10);
		  }
		
		int ae = *adj;
		
		index(nnz) = ae;
		nnz++;
		
		ArraySimpleFixed<real,3,1,1,1> xaec, aec_norm;
		xaec=aec_norm=0.;
		real dotp=0;
		for ( int a=0; a<rDim; a++ )
		  {
		    //		      xaec[a] = .5*( verts(edges(ae,0),a) + verts(edges(ae,1),a) );
		    aec_norm[a] = normal(ae,0,0,a);
		    
		    dotp += aec_norm[a]*ec_norm[a];
		  }
		
		real aecmag = sqrt(ASmag2(aec_norm));
		// 		  for ( int a=0; a<rDim; a++ )
		// 		    aec_norm[a] /= aecmag;
		
		int nv=0;
		for ( int v=0; v<entities.getLength(1) && entities(ae,v)>-1; v++ )
		  {
		    int ent = entities(ae,v);
		    for ( int a=0; a<rDim; a++ )
		      xaec[a] += verts(ent,a);
		    nv++;
		  }
		
		for ( int a=0; a<rDim; a++ )
		  xaec[a] = xaec[a]/real(nv) - xec[a];
		
		dotp = fabs(dotp)/(aecmag*ecmag);
		
		if ( dbgLSR && (*iter)==0 && iter.getType()==UnstructuredMapping::Face)
		  {
		    cout<<"xaec "<<xaec<<endl;
		    cout<<"aec_norm "<<aec_norm<<endl;
		    cout<<"dotp "<<dotp<<endl;
		  }
		
		//		real w = (1.-dotp)*ec_norm[maxDim];
		real w = 1.;//1./sqrt(ASmag2(xaec));
		w = 1./ASmag2(xaec);
		for ( int a=0; a<rDim; a++ )
		  {
		    int dblk = ( maxDim+a ) % rDim;
		    //		      cout<<"dblk = "<<dblk<<endl;
		    for ( int t=1; t<nTerm; t++ )
		      { // XXX this only works for linear reconstruction right now
			Msub(neqFound, t+a*nTerm -1 ) = w*ec_norm[maxDim]*aec_norm[dblk]*xaec[t-1];// - ec_norm[dblk]*aec_norm[maxDim]*xec[t-1];
			//			  Msub(neqFound, t+a*nTerm -1 ) = w*aec_norm[dblk]*xaec[t-1];// - ec_norm[dblk]*aec_norm[maxDim]*xec[t-1];
			
			//			  cout<<t<<"  "<<ec_norm[maxDim]<<"  "<<aec_norm[dblk]<<"  "<<xaec[t-1]<<endl;
			//cout<<neqFound<<", "<<t+a*nTerm-1<<" : "<<Msub(neqFound, t+a*nTerm -1 )<<"  "<<ec_norm[maxDim]*aec_norm[dblk]*xaec[t-1]<<endl;
			//			  if ( (t+a*nTerm-1) == 1 )
			
		      }
		    if ( a>0 )
		      {
			//			  cout<<0<<"  "<<ec_norm[maxDim]<<"  "<<aec_norm[dblk]<<"  "<<ec_norm[dblk]<<"  "<<aec_norm[maxDim]<<"  "<<endl;
			Msub(neqFound, a*nTerm -1 ) = w*(ec_norm[maxDim]*aec_norm[dblk] - ec_norm[dblk]*aec_norm[maxDim]);
			//Msub(neqFound, a*nTerm -1 ) = w*aec_norm[dblk] - ec_norm[dblk]*aec_norm[maxDim];
		      }
		  }

		wgts(neqFound) = w;

		neqFound++;
	      }
	  }
	  extcTime_1 += getCPU()-t0;
	//	cout<<"neqFound = "<<neqFound<<endl;

	if ( Msub.size(0)>neqFound )
	  {
	    ArraySimple<real> mt;
	    mt = Msub;
	    Msub.resize(neqFound, MsubCol     );
	    Msub = 0;
	    for ( int r=0; r<neqFound; r++ )
	      for ( int c=0; c<MsubCol; c++ )
		Msub(r,c) = mt(r,c);
	  }

	
	//	    if ( false && (*iter)==71 )
	if ( dbgLSR && (*iter)==0 && iter.getType()==UnstructuredMapping::Face ) {
	  //	    R=inv( [(Msub^T).Msub] ) 
	  printF("[");
	  for ( int r=0; r<neqFound; r++ )
	    {
	      printF("[");
	      for ( int c=0; c<MsubCol ; c++ )
		{
		  printF("%15.10e ",Msub(r,c));
		  if ( c<MsubCol-1 )
		    printF(", ");
		}
	      if ( r<neqFound-1 )
 		  printF("],\n");
	      else
		printF("]]\n");
	    }
	}

	int nzeroed = 0;
	int icol =0;
	
	for ( int c=0; c<MsubCol; c++ )
	  {
	    bool hasnzero = false;
	    for ( int r=0; r<neqFound && !hasnzero; r++ )
	      {
		hasnzero =  fabs(Msub(r,icol))>100*REAL_MIN/*REAL_EPSILON*/ ;
		//		hasnzero =  fabs(Msub(r,icol))>REAL_EPSILON ;
	      }

	    if ( !hasnzero )
	      { // this column needs to go away, keep track of which one it is
		//   and after the least squares system is solved for the remaining
		//   coefficients set the corresponding coefficient to zero
		zeroedCol(c) = true;
		nzeroed++;
		for ( int cc=icol; cc<MsubCol-1; cc++ )
		  for ( int r=0; r<neqFound; r++ )
		    Msub(r,cc) = Msub(r,cc+1);
	      }
	    else
	      icol++;
	  }

	if ( nzeroed )
	  cout<<"nzeroed is "<<nzeroed<<endl;

	ArraySimple<real> M(MsubCol+1,nnz);
	ArraySimple<real> RinvMsT(MsubCol,neqFound);
	M = 0;
	RinvMsT = 0;
	icol = 0;
	int irow = 0;

#if 0
	R.resize(MsubCol,MsubCol);
	R=0;
	for ( int r=0; r<(MsubCol-nzeroed); r++ )
	  {
	    for ( int c=0; c<(MsubCol-nzeroed); c++ )
	      {
		for ( int cc=0; cc<neqFound; cc++ )
		  R(r,c) += Msub(cc,r)*Msub(cc,c);
	      }
	  }
	
	int len = MsubCol-nzeroed;
	ArraySimple<real> work( neqFound );
	ArraySimple<int> ipvt( neqFound );
	real det;
	F90_INTEGER job=3;
	F90_INTEGER info=0;
	//	    cout<<"nzeroed = "<<nzeroed<<endl;
	if ( false && (*iter)==0 && iter.getType()==UnstructuredMapping::Face )
	  {
 	    cout<<"R = "<<R<<endl;
	    //	    abort();
	  }



	for ( int r=0; r<MsubCol; r++ )
	  if ( !zeroedCol(r) )
	    {
	      for ( int c=0; c<neqFound ; c++ )
		{
		  RinvMsT(r,c) = 0;
		  icol = 0;
		  for ( int cc=0; cc<MsubCol; cc++ )
		    if ( !zeroedCol(cc) )
		      {
			RinvMsT(r,c) += R(irow,icol)*Msub(c,icol);
			icol++;
		      }
		}
	      irow++;
	    }
#endif
	
	//	if ( true || !nzeroed ) {
	  int n = MsubCol - nzeroed;
	  
	  ArraySimple<real> wrk( neqFound,neqFound+1 );
	  wrk = 0.;
	  ArraySimple<real> qri( n, neqFound );
	  qri = 0.;
	  
	  int ierr=0;
	  real rcond;
	  //	  LSCOEFF ( mn.ptr()/*Msub.ptr()*/, &neqFound, &n, qri.ptr(), wrk.ptr(), &ierr);
	  real t0l=getCPU();
	  LSCOEFF ( Msub.ptr(), &neqFound, &n, qri.ptr(), &rcond,wrk.ptr(), &ierr);
	  lscTime += getCPU()-t0l;
	  
	  real t02 = getCPU();

	  RinvMsT = 0;
	  int rc=0;
	  for ( int r=0; r<MsubCol; r++ )
	    if ( !zeroedCol(r) )
	      {
		for ( int c=0; c<neqFound; c++ )
		  //		  if ( !zeroedCol(c) )
		  RinvMsT(r,c) = qri(rc,c);
		rc++;
	      }
	  
	  if ( ierr )
	    {
	      cout<<"xec "<<xec<<endl;
	      cout<<"ec_norm "<<ec_norm<<endl;
	      cout<<"maxDim "<<maxDim<<endl;
	      cout<<"nTerm "<<nTerm<<endl;
	      cout<<"nv "<<nv<<endl;
	    }
	  
	  if (dbgLSR && (*iter)==0 && iter.getType()==UnstructuredMapping::Face)
	    cout<<"QRI "<<qri<<endl;
	  //	}

	// now fill in the full coefficient matrix including the row for the current edge
	//     this looks like :    [ 1/nx_0 | -ny_0*{R^{-1}Msub^T_{2,j}}/nx_0 ] { U_0 }
	//                      g = [      -                                 ]   {  -  }
	//                          [   0  |      R^{-1}Msub^T               ]   {  Unx_0 - U0*nx  } // subtraction is from the gaussian elimination
	//	

	//                      g = U_0 [M] {1, -nx} + nx_0 M {0, U}
	//                      {E} = g.{1, dx, dy, 1, dx, dy} = {E_x, E_y} = {g_x, g_y}.{V, V}
	M(0,0) = 1/normal(ecur,0,0,maxDim);
	for ( int r=1; r<rDim; r++ )
	  for ( int t=1; t<nnz; t++ )
	    { 
	      //	    M(0,t) = -normal(ecur,0,0,(maxDim+1)%rDim)*RinvMsT(nTerm*((maxDim+1)%rDim)-1,t-1)/normal(ecur,0,0,maxDim);
	      M(0,t) -= normal(ecur,0,0,(maxDim+r)%rDim)*wgts(t-1)*RinvMsT(nTerm*r-1,t-1)/normal(ecur,0,0,maxDim);
	      //	    cout<<(maxDim+1)%rDim<<"  "<<normal(ecur,(maxDim+1)%rDim)<<"  "<<M(0,t)<<endl;
	    }
	
	//	cout<<"M0 = "<<M<<endl;
	
	for ( int r=1; r<MsubCol+1; r++ )
	  //nnz	  for ( int c=0; c<neqFound; c++ )
	  for ( int c=0; c<nnz-1; c++ )
	    {
	      M(r,c+1) = RinvMsT(r-1,c)*wgts(c);
	      // 	      cout<<"r, c+1 "<<r<<"  "<<c+1<<"  "<<M(r,c+1)<<endl;
	      // 	      cout<<"Mr"<<M<<endl;
	    }

	
	//	cout<<"M = "<<M<<endl;
	
	
	//	coeff.resize(nnz,rDim);
	coeff = 0;
	
	for ( int r=0; r<rDim; r++ )
	  coeff(0,(maxDim+r)%rDim) = M(r*nTerm,0);
	
	for ( int i=1; i<nnz; i++ )
	  {
	    for ( int r=0; r<rDim; r++ )
	      {
		coeff(0,(maxDim+r)%rDim) -= M(r*nTerm,i)*normal(index(i),0,0,maxDim);
		coeff(i,(maxDim+r)%rDim) += normal(ecur,0,0,maxDim)*M(r*nTerm,i);
	      }
	  }
	
	if (dbgLSR && (*iter)==0 && iter.getType()==UnstructuredMapping::Face)
	  cout<<"coeff("<<ecur<<") = "<<coeff<<endl;
	
	real t1 = getCPU();
	extcTime_2 += t1-t02;
	extcTime += t1-t0;
	extcCalls++;
	return;
      }

    abort();

    return;
  }

  void fixupPEC( CompositeGrid &cg, ArraySimple<int> &Hindex, ArraySimple<double> &Hcoeff, int pecID )
  {
    for ( int grid=0; grid<cg.numberOfGrids(); grid++ )
      {
	MappedGrid &mg = cg[grid];
	
	const IntegerArray &bci = *mg.getUnstructuredBCInfo(  UnstructuredMapping::Edge );

	assert( mg.getGridType()==MappedGrid::unstructuredGrid );
    
	UnstructuredMapping & umap = (UnstructuredMapping &) mg.mapping().getMapping();

	ArraySimple<bool> isPEC(umap.size(UnstructuredMapping::Edge));

	isPEC = false;

	int nPec = 0;
	for ( int i=0; i<bci.getLength(0); i++ )
	{
	  int ei = bci(i,0);
	  if ( bci(i,1)==pecID )
	    {
	      isPEC[ei] = true;
	      nPec++;
	    }
	}
	cout<<"RESET "<<nPec<<" PEC coefficients"<<endl;

	for ( int i=0; i<Hindex.size(); i++ )
	  if ( isPEC[Hindex[i]-1] ) Hcoeff[i] = 0.;

      }
    
  }

}

void
Maxwell::
setupDSICoefficients()
{
  adjSearchTime = 0;
  lscTime=0;
  extcTime=0;
  extcTime_1=0;
  extcTime_2=0;
  csetTime=0;
  extcCalls=0;
  CompositeGrid &cg = *cgp;
  UnstructuredMapping::EntityTypeEnum cellType = UnstructuredMapping::EntityTypeEnum(cg.numberOfDimensions());
  UnstructuredMapping::EntityTypeEnum faceType = UnstructuredMapping::Face;
  UnstructuredMapping::EntityTypeEnum edgeType = UnstructuredMapping::Edge;
	      
  this->useGhostInReconstruction = true;
  ::useGhostInReconstruction = this->useGhostInReconstruction;

  int nDim = cg.numberOfDimensions();
  int rDim = nDim;

  int nDeg = 1; //linear reconstruction
  int nEqg = 10; // initial guess
  int MsubCol = 1;
#if 0
  for ( int d=1; d<(nDeg+1); d++ )
    MsubCol += (d+1);
#else
  MsubCol = rDim==2 ? 3 : 4;
#endif
	      
  int nTerm = MsubCol;
  minEqs = rDim * nTerm ;
  int nadd = rDim*nTerm;//iter.isGhost() ? rDim : 1;
  minEqs += 1;//rDim;//nadd/2;
  nsearch = rDim*nTerm + nadd;
  //nsearch *= (rDim-1);
  //nsearch += rDim;// add just a few more for good luck :)
  nhops = 2;
  //  eAdj = UnstructuredMapping::Vertex;
  eAdj =  UnstructuredMapping::Face;
  //  eAdj =  rDim==2 ? UnstructuredMapping::Face : UnstructuredMapping::Vertex;
  //  hAdj = UnstructuredMapping::Edge;
  hAdj = UnstructuredMapping::Region;

  int nEEq=0, nHEq=0;
  minRC = REAL_MAX;
  maxRC = 0;
  minNGB = INT_MAX;
  maxNGB = 0;

  ArraySimple<int> eqOffsetH(cg.numberOfGrids()+1), eqOffsetE(cg.numberOfGrids()+1);
  eqOffsetH = 0;
  eqOffsetE =0;

  for ( int grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid &mg = cg[grid];

      realMappedGridFunction &mgfE = mgp ? fields[2] : getCGField(EField,0)[grid];
      realMappedGridFunction &mgfH = mgp ? fields[0] : getCGField(HField,0)[grid];

      nEEq += mgfE.getLength(0)*mgfE.getLength(1)*mgfE.getLength(2);
      eqOffsetE(grid+1) = eqOffsetE(grid) + mgfE.getLength(0)*mgfE.getLength(1)*mgfE.getLength(2);

      nHEq += mgfH.getLength(0)*mgfH.getLength(1)*mgfH.getLength(2);
      eqOffsetH(grid+1) = eqOffsetH(grid) + mgfH.getLength(0)*mgfH.getLength(1)*mgfH.getLength(2);
    }

  cout<<"nEEq, nHEq "<<nEEq<<"  "<<nHEq<<endl;
  Eoffset.resize(nEEq+1);
  Eoffset = 0;
  Hoffset.resize(nHEq+1);
  Hoffset = 0;

#if 0
  REoffset.resize(cg.numberOfDimensions()*nEEq+1);
  RHoffset.resize(cg.numberOfDimensions()*nHEq+1);
#endif

  //  ArraySimple<real> dispCoeff;
  real maxDC=0;
  dispCoeff.resize(nHEq);
  dispCoeff = 0;
  E_dispCoeff.resize(nEEq);
  E_dispCoeff = 0.;
  // now compute the number of non-zeros in the matrices

  int nnZE=0;
  int nnZH=0;
  int nnZRE = 0;
  int nnZRH = 0;
  int nnZREmax = 0;
  int nnZRHmax = 0;

  //kkc links used to compute dsi operators
  ArraySimple<ArraySimple<UnstructuredMappingAdjacencyIterator> > uElinks(nEEq),uFlinks(nHEq);

  if ( cacheCoeff )
    {
      uElinks.resize(nEEq);
      uFlinks.resize(nHEq);
    }

  for ( int grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid &mg = cg[grid];
      
      int eqoE = eqOffsetE(grid);
      int eqoH = eqOffsetH(grid);

      if ( mg.getGridType()==MappedGrid::structuredGrid )
	{
	  // DSI on a structured grid has a fixed stencil width
	  if (cg.numberOfDimensions()==2)
	    {
	      nnZE+=4*(eqOffsetE(grid+1)-eqOffsetE(grid));
	      nnZH+=4*(eqOffsetH(grid+1)-eqOffsetH(grid));
	    }
	  else
	    abort();
	}
      else
	{
	  UnstructuredMapping &umap = (UnstructuredMapping &)mg.mapping().getMapping();

	  UnstructuredMappingIterator iter,iter_end;
	  UnstructuredMappingAdjacencyIterator aiter,aiter_end, eiter,eiter_end;

	  if ( cg.numberOfDimensions()==2 && !new2d )
	    {
	      nnZE += 4*umap.size(faceType);  // this should change if we use a more general reconstruction

	      iter_end = umap.end(faceType);
	      for ( iter=umap.begin(faceType); iter!=iter_end; iter++ )
		nnZH += 3*(umap.computeElementType(faceType,*iter)==UnstructuredMapping::quadrilateral ? 4 : 3); // *3 since its the face+2 neigbhs.
	    }
	  else
	    {
	      iter_end=umap.end(faceType);
	      for ( iter=umap.begin(faceType); iter!=iter_end; iter++ )
		{
		  UnstructuredMappingAdjacencyIterator aiter,aiter_end;
		  int nnZRHL = 0;
		  ArraySimple< UnstructuredMappingAdjacencyIterator > dum;
		  ArraySimple< UnstructuredMappingAdjacencyIterator > &adjEnts = cacheCoeff ? uFlinks[*iter] : dum;
		  //		  int nadd = rDim*nTerm;//iter.isGhost() ? rDim : 1;
		  //		  int nsearch = rDim*nTerm + nadd;
		  //		  nsearch *= 2;

		  real t0 = getCPU();
		  
		  int ns = iter.isGhost() ? 2*nsearch : nsearch;
		  bool ug = true;//iter.isGhost() || useGhostInReconstruction;

		  //		  if ( *iter==3402 )
		  //		    cout<<"hey"<<endl;
		  if ( rDim==2 ) // shouldn't this go away?
		    unstructuredLink(umap, adjEnts, iter, nhops, ns, UnstructuredMapping::Edge, ug);
		  else
		    unstructuredLink(umap, adjEnts, iter, nhops, ns, hAdj,ug);
		  adjSearchTime += getCPU()-t0;

		  nnZRHL = adjEnts.size(0);
		  //		  cout<<"nnZRHL("<<*iter<<") = "<<nnZRHL<<endl;
		  // 		      for ( ; aiter!=aiter_end; aiter++ )
		  // 			{
		  
		  // 			  UnstructuredMappingAdjacencyIterator eiter;
		  // 			  eiter = umap.adjacency_begin(aiter, faceType);
		  // 			  nnZRHL+= eiter.nAdjacent() - 1;
		  // 			}
		  nnZRH += nnZRHL+1;
		  nnZRHmax = max( nnZRHmax, nnZRHL+1 );
		  
		  aiter_end = umap.adjacency_end(iter, UnstructuredMapping::EntityTypeEnum(edgeType));
		  aiter = umap.adjacency_begin(iter, UnstructuredMapping::EntityTypeEnum(edgeType));
		  nnZE += aiter.nAdjacent()*(min(maxSample,nnZRHL)+1);
		}
		  
	      iter_end=umap.end(edgeType);
	      
	      for ( iter=umap.begin(edgeType); iter!=iter_end; iter++ )
		{
		  UnstructuredMappingAdjacencyIterator aiter,aiter_end;
		  int nnZREL = 0;
		  ArraySimple< UnstructuredMappingAdjacencyIterator > dum;
		  ArraySimple< UnstructuredMappingAdjacencyIterator > &adjEnts = cacheCoeff ? uElinks[*iter] : dum;

		  //		  int nadd = rDim*nTerm;//iter.isGhost() ? rDim : 1;
		  
		  //		  int nsearch = rDim*nTerm + nadd;
		  //		  nsearch *= 2;
		  real t0=getCPU();
		  int ns = iter.isGhost() ? 2*nsearch : nsearch;
		  bool ug = true;//iter.isGhost() || useGhostInReconstruction;

		  unstructuredLink(umap, adjEnts, iter, nhops, ns,eAdj,ug);
		  adjSearchTime += getCPU()-t0;
		  //		      unstructuredLink(umap, adjEnts, iter, 1, rDim*nTerm+nadd);
		  nnZREL = adjEnts.size(0);
		  //		  cout<<"nnZREL = "<<nnZREL<<endl;
		  // 		      for ( ; aiter!=aiter_end; aiter++ )
		  // 			{
		  
		  // 			  UnstructuredMappingAdjacencyIterator eiter;
		  // 			  eiter = umap.adjacency_begin(aiter, edgeType);
		  // 			  nnZREL+= eiter.nAdjacent() - 1;
		  // 			}
		  nnZRE += nnZREL+1;
		  nnZREmax = max( nnZREmax, nnZREL+1 );

		  aiter_end = umap.adjacency_end(iter, UnstructuredMapping::EntityTypeEnum(cellType));
		  aiter = umap.adjacency_begin(iter, UnstructuredMapping::EntityTypeEnum(cellType));
		  nnZH += aiter.nAdjacent()*(min(maxSample,nnZREL)+1);
		  //		      cout<<"nnZH "<<nnZH<<endl;
		}
	    }
	}
    }
  
  // there are equations for each component of the vector fields so the number of nonzeros must be adjusted accordingly
  cout<<"guessed nnZRH, nnZRE "<<nnZRH<<"  "<<nnZRE<<endl;
  cout<<"guessed nnZRHmax, nnZREmax "<<nnZRHmax<<"  "<<nnZREmax<<endl;
  nnZRH *= cg.numberOfDimensions(); 
  nnZRE *= cg.numberOfDimensions(); 
  //  cout<<"2 nnZRH, nnZRE "<<nnZRH<<"  "<<nnZRE<<endl;
  
  Ecoeff.resize(nnZE);
  Ecoeff = 0.;
  Eindex.resize(nnZE);
  Eindex = -1;

  // each of these describes 3 equations in the reconstruction arrays
  //      (one equation for each dimension)

  ArraySimple< ArraySimple<real> > RECcache, RHCcache;
  ArraySimple< ArraySimple<int> > REIcache, RHIcache;

  if ( cacheCoeff ) {
    RECcache.resize(nEEq);
    REIcache.resize(nEEq);
    RHCcache.resize(nHEq);
    RHIcache.resize(nHEq);
  }
  ArraySimple<real> REeqCoeff_tmp(nnZREmax,rDim);
  ArraySimple<real> RHeqCoeff_tmp(nnZRHmax,rDim);

  ArraySimple<int> REeqIdx_tmp(nnZREmax);
  ArraySimple<int> RHeqIdx_tmp(nnZRHmax);

  double dispPow = double(orderOfArtificialDissipation/2);
  //  if ( rDim==2 ) dispPow--;
  // reset these so we can use them as a running index into the coeff and index arrays
  int nnZHT = nnZH;
  nnZE=0;
  nnZH=0;

  int minEngb=INT_MAX, minFngb=INT_MAX;
  int minEloc=  -1;
  real minEdgeDist=REAL_MAX;

  real fdot_min = REAL_MAX;

  for ( int grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid &mg = cg[grid];

      //      int eqoE = eqOffsetE(grid);
      int eqoH = eqOffsetH(grid);

      if ( mg.getGridType()==MappedGrid::structuredGrid )
	{
	  abort();
	}
      else // unstructured operators
	{
	  UnstructuredMapping &umap = (UnstructuredMapping &)mg.mapping().getMapping();
	  
	  bool vCent = mg.isAllVertexCentered();
	  realArray &cFArea = vCent ? mg.centerArea() : mg.faceArea();
	  const realArray &cFNorm = vCent ? mg.centerNormal() : mg.faceNormal();
	  const realArray &cEArea = vCent ? mg.faceArea() : mg.centerArea();
	  const realArray &cENorm = vCent ? mg.faceNormal() : mg.centerNormal();

	  // 	  cFArea.display("cFArea");
// 	  cFNorm.display("cFNorm");
// 	  cEArea.display("cEArea");
//	  cFArea.display("cFArea");

// 	  cENorm.display("cENorm");

	  const realArray &verts = mg.vertex();

	  //	  realArray faceAreaNormals, edgeAreaNormals;

	  if ( rDim==3 )
	    {
	      faceAreaNormals.redim(cFNorm.getLength(0), cFNorm.getLength(1), cFNorm.getLength(2), cg.numberOfDimensions());
	      for ( int i3=cFArea.getBase(2); i3<=cFArea.getBound(2); i3++ )
		for ( int i2=cFArea.getBase(1); i2<=cFArea.getBound(1); i2++ )
		  for ( int i1=cFArea.getBase(0); i1<=cFArea.getBound(0); i1++ )
		    for ( int a=0; a<nDim; a++ )
		      faceAreaNormals(i1,i2,i3,a) = cFArea(i1,i2,i3)*cFNorm(i1,i2,i3,a);
	    }
	  else
	    {
	      cFArea.redim(umap.size(UnstructuredMapping::Face),1,1);
	      UnstructuredGeometry::computeGeometry(umap, 0, 0, 0, &cFArea, 0, 0);
	    }


	  edgeAreaNormals.redim(cENorm.getLength(0), cENorm.getLength(1), cENorm.getLength(2), cg.numberOfDimensions());
	  for ( int i3=cEArea.getBase(2); i3<=cEArea.getBound(2); i3++ )
	    for ( int i2=cEArea.getBase(1); i2<=cEArea.getBound(1); i2++ )
	      for ( int i1=cEArea.getBase(0); i1<=cEArea.getBound(0); i1++ )
		for ( int a=0; a<nDim; a++ )
		  edgeAreaNormals(i1,i2,i3,a) = cEArea(i1,i2,i3)*cENorm(i1,i2,i3,a);
	  

	  UnstructuredMappingIterator iter,iter_end;
	  UnstructuredMappingAdjacencyIterator aiter,aiter_end, eiter,eiter_end;

	  real dtoeps = 1./eps;
	  real dtbymu = 1./mu;

	  const realArray &nodes  = umap.getNodes();
	  const intArray &edges = umap.getEntities(edgeType);
	  const intArray &face = umap.getEntities(faceType);
	  
	  //	  face.display();
	  nnZE = nnZH = 0;
	  realArray faceCenters, edgeCenters, cellCenters;
	  getCenters( mg, faceType, faceCenters);
	  getCenters( mg, edgeType, edgeCenters);
	  getCenters( mg, cellType, cellCenters);
	  iter_end = umap.end(edgeType);

	  
	  real dropEps = 1e-4;
	  // // //
	  // CONSTRUCT CURL H OPERATOR (E update)
	  // // //
	  if ( rDim==2 )
	    {
	      for ( iter=umap.begin(edgeType); iter!=iter_end; iter++ )
		{
		  aiter=umap.adjacency_begin(iter,faceType);
		  aiter_end = umap.adjacency_end(iter,faceType);
		  int nadj = aiter.nAdjacent();
		  //		  for ( aiter=umap.adjacency_begin(iter,faceType); aiter!=aiter_end; aiter++ )
		  //		    nadj++;
		      
		  Eoffset(*iter) = nnZE;
		  if ( !iter.isGhost() /*|| nadj==2*/ )
		    {
		      E_dispCoeff(*iter) = pow(fabs(cEArea(*iter,0,0)),orderOfArtificialDissipation);
		      aiter_end = umap.adjacency_end(iter,faceType);
		      for ( aiter=umap.adjacency_begin(iter,faceType); aiter!=aiter_end; aiter++ )
			{
			  Ecoeff(nnZE) = dtoeps*aiter.orientation();
			  Eindex(nnZE) = *aiter;
			  nnZE++;
			}
		    }
		      
		}
	      
	      Eoffset(Eoffset.size(0)-1) = nnZE;
	      minEngb=1;
	    }
	  else
	    {

	      E_dispCoeff.resize(nEEq);
	      for ( iter=umap.begin(edgeType); iter!=iter_end; iter++ )
		{
		  UnstructuredMappingAdjacencyIterator aiter, aiter_end;
		  aiter_end = umap.adjacency_end(iter, faceType);

		  int nadj = 0;
		  for ( aiter=umap.adjacency_begin(iter,faceType); aiter!=aiter_end; aiter++ )
		    nadj++;

		  if ( !iter.isBC() && nadj<minEngb )
		    {
		      minEngb = nadj;
		      minEloc = *iter;
		    }

		  ArraySimpleFixed<real,3,1,1,1> edgeVec;

		  for ( int a=0; a<rDim; a++ )
		    edgeVec[a] = (verts(edges(*iter,1),0,0,a)-verts(edges(*iter,0),0,0,a));

// 		  if ( !iter.isBC() && nadj<4 )
// 		    {
// 		      cout<<"Edge "<<*iter<<" has only "<<nadj<<" neighboring faces"<<endl;
// 		    }

		  Eoffset(*iter) = nnZE;

		  if (  !(iter.isGhost() /*|| iter.isBC()*/) )//|| ( rDim==3 && nadj>2 ) || (rDim==2 && nadj==2) )
		    {
		      int nnz=0;
		      if ( useModifiedStencil && rDim==3)
			{

			  for ( int a=0; a<rDim; a++ )
			    edgeAreaNormals(*iter,0,0,a) = 0.;
			  
			  UnstructuredMappingAdjacencyIterator edgeReg, edgeReg_end, regFace,regFace_end, faceEdge,faceEdge_end;
			  edgeReg_end = umap.adjacency_end(iter, UnstructuredMapping::Region);
			  ArraySimpleFixed<int, 2,1,1,1> facesToUse;
			  facesToUse=-1;
			  for ( edgeReg=umap.adjacency_begin(iter, UnstructuredMapping::Region); edgeReg!=edgeReg_end; edgeReg++ )
			    {
			      // find the two faces that share this edge in this region
			      regFace_end = umap.adjacency_end(edgeReg, UnstructuredMapping::Face);
			      int nFacesFound = 0;
			      for ( regFace=umap.adjacency_begin(edgeReg, UnstructuredMapping::Face); regFace!=regFace_end; regFace++ )
				{
				  faceEdge_end = umap.adjacency_end(regFace, UnstructuredMapping::Edge);
				  for ( faceEdge=umap.adjacency_begin(regFace, UnstructuredMapping::Edge); 
					faceEdge!=faceEdge_end && (*faceEdge)!=(*iter); faceEdge++ )
				    {}
				  if ( faceEdge!=faceEdge_end )
				    {
				      assert((*faceEdge)==(*iter));
				      facesToUse[nFacesFound] = *regFace;
				      nFacesFound++;
				    }
				}
			      
			      assert(nFacesFound==2);
			      ArraySimpleFixed<real,3,1,1,1> faceCents[2], tangent, nrm_calc, edgeCent,intersection, nrm_contrib;
			      
			      for ( int a=0; a<rDim; a++ )
				{
				  edgeCent[a] = edgeCenters(*iter,a);
				  for ( int f=0; f<2; f++ )
				    faceCents[f][a] = faceCenters(facesToUse[f],a);
				  tangent[a] = faceCents[1][a]-faceCents[0][a];
				}
			      
			      real a0,a1;
			      
			      intersect3DLines(faceCents[0],tangent,edgeCent,edgeVec,a0,a1,intersection);

			      nrm_calc = areaNormal3D(faceCents[0],faceCents[1],edgeCent);

			      assert(nrm_calc[0]==nrm_calc[0]);
			      real sgnDot = 0.;
			      for ( int a=0; a<rDim; a++ )
				sgnDot += nrm_calc[a]*edgeVec[a];
			      real sgn = sgnDot>0 ? 1 : -1;
			     
			      real edgeDist=0;

			      // compute area normals from edge integrals
			      nrm_contrib[0] = faceCents[0][1]*faceCents[1][2] - faceCents[0][2]*faceCents[1][1];
			      nrm_contrib[1] = faceCents[0][2]*faceCents[1][0] - faceCents[0][0]*faceCents[1][2];
			      nrm_contrib[2] = faceCents[0][0]*faceCents[1][1] - faceCents[0][1]*faceCents[1][0];

			      for ( int a=0; a<rDim; a++ )
				{
#if 0
				  edgeAreaNormals(*iter,0,0,a) += sgn*nrm_calc[a];
#else
				  edgeAreaNormals(*iter,0,0,a) += sgn*nrm_contrib[a]/2;
#endif
				  edgeDist += (faceCents[a][0]+tangent[a]*a0 - edgeCent[a]-edgeVec[a]*a1)*(faceCents[a][0]+tangent[a]*a0 - edgeCent[a]-edgeVec[a]*a1);
				}

			      minEdgeDist = min(minEdgeDist,edgeDist);

			      
			      if ( true ) // this is where the variable disp option should go
				{
				  for ( int f=0; f<2; f++ )
				    {
				      int nnzC=0;
				      int peri = -1;
				      // to save memory we recompute these coefficients every time <-- REALLY SLOW but saves memory
				      UnstructuredMappingIterator tmpi;
				      tmpi = umap.begin(UnstructuredMapping::Face);
				      tmpi.setLocation(facesToUse[f]);
				  
				      bool recompute = true;
				      if ( cacheCoeff && !RHCcache(facesToUse[f]).size() )
					{
					  RHCcache(facesToUse[f]).resize(nnZRHmax,rDim);
					  RHIcache(facesToUse[f]).resize(nnZRHmax);
					}
				      else if ( cacheCoeff && RHCcache(facesToUse[f]).size() )
					{
					  recompute = false;
					  nnzC = RHIcache(facesToUse[f]).size(0);
					}
				      
				      ArraySimple<real> & RHeqCoeff = cacheCoeff ? RHCcache(facesToUse[f]) : RHeqCoeff_tmp;
				      ArraySimple<int> & RHeqIdx = cacheCoeff ? RHIcache(facesToUse[f]) : RHeqIdx_tmp;

				      ArraySimple<UnstructuredMappingAdjacencyIterator> dum;
				      ArraySimple<UnstructuredMappingAdjacencyIterator> &links = cacheCoeff ? uFlinks[facesToUse[f]] : dum;
				      
				      if ( !cacheCoeff )
					{
					  real t0 = getCPU();
					  int ns = tmpi.isGhost() ? 2*nsearch : nsearch;
					  bool ug = true;//tmpi.isGhost() || useGhostInReconstruction;
					  
					  unstructuredLink(umap, links, tmpi, nhops, ns, hAdj, ug);
					  adjSearchTime += getCPU()-t0;
					}
				  
				      if ( recompute )
					{
					  if ( !useCleanLSR )
					    extractDSIReconstructionCoefficients( umap, tmpi, links, 
										  faceAreaNormals, RHeqCoeff, RHeqIdx, nnzC,peri);
					  else
					    computeDSIReconstructionCoefficients( umap, tmpi, links, 
										  faceAreaNormals, RHeqCoeff, RHeqIdx, nnzC,peri);
					}
				      //				extractDSIReconstructionCoefficients( umap, tmpi, uFlinks[*aiter], 
				      //								      faceAreaNormals, RHeqCoeff, RHeqIdx, nnzC,peri);

				      if ( false && recompute )
					{
					  RHeqCoeff.resize(nnzC,rDim);
					  RHeqIdx.resize(nnzC);
					}
			      
				      for ( int c=0/*RHoffset(*aiter)*/; c<nnzC/*RHoffset(*aiter+1)*/; c++ )
					{
					  int i=0;
					  bool found=false;
					  while ( i<nnz && Eindex(Eoffset(*iter)+i)!=RHeqIdx(c) ) i++;
					  
					  
					  //			  cout<<"==="<<endl;
					  assert( (Eindex(Eoffset(*iter)+i)==RHeqIdx(c)) || i==nnz );
					  
					  if ( i==nnz )
					    {
					      Eindex(Eoffset(*iter)+i) = RHeqIdx(c);
					      nnz++;
					    }
					  
					  
					  real cf = tangent[0]*RHeqCoeff(c,0) + tangent[1]*RHeqCoeff(c,1);
					  if ( rDim==3 ) cf += tangent[2]*RHeqCoeff(c,2);
				  
					  cf /= 2.;

					  Ecoeff(Eoffset(*iter)+i) += sgn * ( cf )/eps;
					  
					  //ev[0]*RHcoeff(c) +
					  //ev[1]*RHcoeff(c+fidx) +
					  //				ev[2]*RHcoeff(c+2*fidx) )/eps;
					}
				      
				    }
				}
			    }
			  
			  E_dispCoeff(*iter) = 0;
			  real fdot=0;
			  for ( int a=0; a<rDim; a++ )
			    {
			      E_dispCoeff(*iter) += edgeAreaNormals(*iter,0,0,a)*edgeAreaNormals(*iter,0,0,a);
			      fdot+=edgeAreaNormals(*iter,0,0,a)*edgeVec[a];
			    }

			  E_dispCoeff(*iter) = sqrt(E_dispCoeff(*iter));
			  fdot_min = min(fdot_min, fabs(fdot)/(sqrt(ASmag2(edgeVec))*E_dispCoeff(*iter)));

			  nnZE += nnz;
			}
		      else
			{
			  real checkSgn[3];
			  checkSgn[0] = checkSgn[1] = checkSgn[2] = 0;
			  
			  E_dispCoeff(*iter) = cEArea(*iter,0,0);


			  for ( aiter=umap.adjacency_begin(iter, faceType); aiter!=aiter_end; aiter++ )
			    {
			      real vef[3], ve[3],evf[3];
			      for ( int a=0; a<3; a++ )
				{
				  vef[a] = faceCenters(*aiter,a)-.5*( verts(edges(*iter,1),0,0,a)+
								      verts(edges(*iter,0),0,0,a) );
				  ve[a]  = verts(edges(*iter,1),0,0,a)-verts(edges(*iter,0),0,0,a);
				  evf[a] = faceAreaNormals(*aiter,0,0,a);
				}


			      real ev[2][3];
			      assert(rDim==3);
			      UnstructuredMappingAdjacencyIterator faceCell, faceCell_end;
			      faceCell_end = umap.adjacency_end(aiter, cellType);
			      faceCell = umap.adjacency_begin(aiter,cellType);
			      int c1 = *faceCell;
			      faceCell++;
			      ArraySimpleFixed<real,3,1,1,1> cc[2],edgeCenter,edge_nrm,nrm_calc;
			      if ( faceCell!=faceCell_end )
				{
				  int c2 = *faceCell;
				  for ( int a=0; a<3; a++ )
				    {
				      //  ev[a] = cellCenters(c2,a)-cellCenters(c1,a);
				      cc[0][a] = cellCenters(c1,a);
				      cc[1][a] = cellCenters(c2,a);
				      ev[1][a] = cellCenters(c2,a)-faceCenters(*aiter,a);
				      //  vef[a] = .5*(cellCenters(c2,a)+cellCenters(c1,a))-.5*( verts(edges(*iter,1),0,0,a)+
				      // 												 verts(edges(*iter,0),0,0,a) );;
				    }
				  //ev[a] = faceAreaNormals(*iter,a);//cellCenters(c2,a)-cellCenters(c1,a);
				}
			      else
				for ( int a=0; a<3; a++ )
				  ev[1][a]= 0.;
			    
			      //			  else
			    
			      for ( int a=0; a<3; a++ )
				{
				  ev[0][a] = faceCenters(*aiter,a)-cellCenters(c1,a);
				  edgeCenter[a] = edgeCenters(*iter,a);
				}

			      nrm_calc = areaNormal3D(cc[0],cc[1],edgeCenter);
#if 0
			      int sgn = ( evf[0]*(vef[1]*ve[2]-vef[2]*ve[1]) -
					  evf[1]*(vef[0]*ve[2]-vef[2]*ve[0]) +
					  evf[2]*(vef[0]*ve[1]-vef[1]*ve[0]) )>0 ? -1 : 1;
#else
			      int sgn = ( ve[0]*(vef[1]*ev[0][2]-vef[2]*ev[0][1]) -
					  ve[1]*(vef[0]*ev[0][2]-vef[2]*ev[0][0]) +
					  ve[2]*(vef[0]*ev[0][1]-vef[1]*ev[0][0]) )>0 ? 1 : -1;
#endif
				  
			      real nrmdot = 0.;
			      for ( int a=0; a<rDim; a++ )
				{
				  checkSgn[a] += (ev[0][a]+ev[1][a])*sgn;
				  edge_nrm[a] = edgeAreaNormals(*iter,0,0,a);
				  nrmdot += sgn*nrm_calc[a]*edge_nrm[a];
				}

			      //			  assert(nrmdot>REAL_EPSILON);

			      assert( (evf[0]*ev[0][0] + evf[1]*ev[0][1] + evf[2]*ev[0][2])>=0 );

			      real fdot=0,evmag=0;
			      for ( int a=0; a<3; a++ )
				{
				  fdot += (ev[0][a]+ev[1][a])*cFNorm(*aiter,0,0,a);
				  evmag+= (ev[0][a]+ev[1][a])*(ev[0][a]+ev[1][a]);
				}

			      fdot /= sqrt(evmag);

			      if ( (1.-fabs(fdot))>dropEps*cFArea(*aiter,0,0) || !useVariableDissipation) 
				{
				  int nnzC=0;
				  int peri = -1;
				  // to save memory we recompute these coefficients every time <-- REALLY SLOW but saves memory
				  UnstructuredMappingIterator tmpi;
				  tmpi = aiter;

				  bool recompute = true;
				  if ( cacheCoeff && !RHCcache(*aiter).size() )
				    {
				      RHCcache(*aiter).resize(nnZRHmax,rDim);
				      RHIcache(*aiter).resize(nnZRHmax);
				    }
				  else if ( cacheCoeff && RHCcache(*aiter).size() )
				    {
				      recompute = false;
				      nnzC = RHIcache(*aiter).size(0);
				    }

				  ArraySimple<real> & RHeqCoeff = cacheCoeff ? RHCcache(*aiter) : RHeqCoeff_tmp;
				  ArraySimple<int> & RHeqIdx = cacheCoeff ? RHIcache(*aiter) : RHeqIdx_tmp;

				  ArraySimple<UnstructuredMappingAdjacencyIterator> dum;
				  ArraySimple<UnstructuredMappingAdjacencyIterator> &links = cacheCoeff ? uFlinks[*aiter] : dum;

				  if ( !cacheCoeff )
				    {
				      real t0 = getCPU();
				      int ns = tmpi.isGhost() ? 2*nsearch : nsearch;
				      bool ug = true;//tmpi.isGhost() || useGhostInReconstruction;

				      unstructuredLink(umap, links, tmpi, nhops, ns, hAdj, ug);
				      adjSearchTime += getCPU()-t0;
				    }

				  if ( recompute )
				    {
				      if ( !useCleanLSR )
					extractDSIReconstructionCoefficients( umap, tmpi, links, 
									      faceAreaNormals, RHeqCoeff, RHeqIdx, nnzC,peri);
				      else
					computeDSIReconstructionCoefficients( umap, tmpi, links, 
									      faceAreaNormals, RHeqCoeff, RHeqIdx, nnzC,peri);
				    }
				  //				extractDSIReconstructionCoefficients( umap, tmpi, uFlinks[*aiter], 
				  //								      faceAreaNormals, RHeqCoeff, RHeqIdx, nnzC,peri);

				  if ( false && recompute )
				    {
				      RHeqCoeff.resize(nnzC,rDim);
				      RHeqIdx.resize(nnzC);
				    }
			      
				  for ( int c=0/*RHoffset(*aiter)*/; c<nnzC/*RHoffset(*aiter+1)*/; c++ )
				    {
				      int i=0;
				      bool found=false;
				      while ( i<nnz && Eindex(Eoffset(*iter)+i)!=RHeqIdx(c) ) i++;
				  
				  
				      //			  cout<<"==="<<endl;
				      assert( (Eindex(Eoffset(*iter)+i)==RHeqIdx(c)) || i==nnz );
				  
				      if ( i==nnz )
					{
					  Eindex(Eoffset(*iter)+i) = RHeqIdx(c);
					  nnz++;
					}
				  
				  
				      real cf = ev[0][0]*RHeqCoeff(c,0) + ev[0][1]*RHeqCoeff(c,1);
				      if ( rDim==3 ) cf += ev[0][2]*RHeqCoeff(c,2);
				  
				      if ( faceCell!=faceCell_end )
					{
					  cf += ev[1][0]*RHeqCoeff(c,0) + ev[1][1]*RHeqCoeff(c,1);
					  if ( rDim==3 ) cf += ev[1][2]*RHeqCoeff(c,2);
					}

				      Ecoeff(Eoffset(*iter)+i) += sgn * ( cf )/eps;
				  
				      //ev[0]*RHcoeff(c) +
				      //ev[1]*RHcoeff(c+fidx) +
				      //				ev[2]*RHcoeff(c+2*fidx) )/eps;
				    }
			      
				}
			      else
				{
				  int i=0;
				  bool found=false;
				  while ( i<nnz && Eindex(Eoffset(*iter)+i)!=(*aiter) ) i++;
				  
				  
				  //			  cout<<"==="<<endl;
				  assert( (Eindex(Eoffset(*iter)+i)==(*aiter) || i==nnz ));
			      
				  if ( i==nnz )
				    {
				      Eindex(Eoffset(*iter)+i) = *aiter;
				      nnz++;
				    }
				  
				  int sgn = ( evf[0]*(vef[1]*ve[2]-vef[2]*ve[1]) -
					      evf[1]*(vef[0]*ve[2]-vef[2]*ve[0]) +
					      evf[2]*(vef[0]*ve[1]-vef[1]*ve[0]) )>0 ? -1 : 1;
				  
				  real cf = sqrt(evmag)/cFArea(*aiter,0,0);
				  //( ev[0]*RHeqCoeff(c,0) + 
				  //					  ev[1]*RHeqCoeff(c,1) +
				  //					  ev[2]*RHeqCoeff(c,2);
				  
				  Ecoeff(Eoffset(*iter)+i) += sgn * ( cf )/eps;
				}
			    }
			  nnZE += nnz;
			  if ( !iter.isGhost() )
			    {
			      real checkSgnMag=0;
			      for ( int a=0; a<checkSgnMag; a++ )
				checkSgnMag += checkSgn[a]*checkSgn[a];

			      assert(checkSgnMag<REAL_EPSILON );
			    }

			}
		    }
		}
	      Eoffset( Eoffset.size(0)-1 ) = nnZE;
	    }
	  RHCcache.resize(0);
	  RHIcache.resize(0);
	  Ecoeff.resize(nnZE);
	  Eindex.resize(nnZE);
	  uFlinks.resize(0);

	  Hcoeff.resize(nnZHT);
	  Hcoeff = 0.;
	  Hindex.resize(nnZHT);
	  Hindex = -1;

	  // // //
	  // CONSTRUCT CURL E OPERATOR (H update) and H DISSIPATION OPERATOR
	  // // //
	  ArraySimple<int> vertexDispCoeff( umap.capacity(UnstructuredMapping::Vertex));
	  vertexDispCoeff = 0;

	  iter_end = umap.end(faceType);
	  for ( iter=umap.begin(faceType); iter!=iter_end; iter++ )
	    {
	      dispCoeff(*iter) = pow(fabs(cFArea(*iter,0,0)),dispPow);
	      maxDC = max(dispCoeff(*iter),maxDC);

	      UnstructuredMappingAdjacencyIterator aiter, aiter_end;
	      aiter_end = umap.adjacency_end(iter, cellType);

	      int nadj = 0;
	      for ( aiter=umap.adjacency_begin(iter,cellType); aiter!=aiter_end; aiter++ )
		nadj++;

	      Hoffset(*iter) = nnZH;
	      if ( !iter.isGhost() || nadj==2 )
		{
		  int nnz = 0;
		  aiter_end = umap.adjacency_end(iter, edgeType);
		  int vvf=0;
		  int nvf = umap.adjacency_begin(iter, UnstructuredMapping::Vertex).nAdjacent();
		  bool toggleDisp = false;
		  int npos=0, nneg=0;
		  real sgnCheck[3];
		  sgnCheck[0] = sgnCheck[1]=sgnCheck[2] = 0;
		  for ( aiter=umap.adjacency_begin(iter, edgeType); aiter!=aiter_end; aiter++ )
		    {
		      int sgn = aiter.orientation();
		      ArraySimpleFixed<real,3,1,1,1> ve;
		      for ( int a=0; a<rDim; a++ )
			{
			  ve[a]  = verts(edges(*aiter,1),0,0,a)-verts(edges(*aiter,0),0,0,a);
			  sgnCheck[a] += sgn*ve[a];
			}


		      if ( sgn<0 ) 
			nneg++;
		      else 
			npos++;
		      
		      if ( dbgLSR && (*iter)==0 )
			{
			  cout<<"sgn = "<<sgn<<endl;
			  cout<<ve[0]<<"  "<<ve[1]<<endl;
			  for ( int a=0; a<rDim; a++ )
			    cout<<verts(edges(*aiter,0),0,0,a)<<"  ";
			  cout<<endl;
			  for ( int a=0; a<rDim; a++ )
			    cout<<verts(edges(*aiter,1),0,0,a)<<"  ";
			  cout<<endl;
			  
			}
			    
		      UnstructuredMappingIterator tmpi;
		      tmpi = aiter;
		      if ( rDim==2 &&  mgp )
			{
			  for ( int a=0; a<rDim; a++ )
			    ve[a] = verts(face(*iter,(vvf+1)%nvf),a)-verts(face(*iter,vvf),a);

			  sgn=1;
			  vvf++;
			}

		      real edot=0;
		      real vemag=0;
		      for ( int a=0; a<rDim; a++ )
			{
			  edot += ve[a]*cENorm(*aiter,0,0,a);
			  vemag += ve[a]*ve[a];
			}
		      
		      edot /= sqrt(vemag);

		      if ( (1.-fabs(edot))>dropEps*cEArea(*aiter,0,0) || !useVariableDissipation ) 
			{
			  toggleDisp=true;
			  int nnzC=0;
			  int peri = -1;
			  // to save memory we recompute these coefficients every time <-- REALLY SLOW but saves memory
			  bool recompute = true;
			  if ( cacheCoeff && !RECcache(*aiter).size() )
			    {
			      RECcache(*aiter).resize(nnZREmax,rDim);
			      REIcache(*aiter).resize(nnZREmax);
			    }
			  else if ( cacheCoeff && RECcache(*aiter).size() )
			    {
			      recompute = false;
			      nnzC = REIcache(*aiter).size(0);
			    }
			  
			  ArraySimple<real> & REeqCoeff = cacheCoeff ? RECcache(*aiter) : REeqCoeff_tmp;
			  ArraySimple<int> & REeqIdx = cacheCoeff ? REIcache(*aiter) : REeqIdx_tmp;
			  
			  if ( recompute )
			    {
			      if (dbgLSR)
				cout<<"calling extract DSIC for iter "<<*tmpi<<endl;
			      ArraySimple<UnstructuredMappingAdjacencyIterator> dum;
			      ArraySimple<UnstructuredMappingAdjacencyIterator> &links = cacheCoeff ? uElinks[*aiter] : dum;
			      if ( !cacheCoeff )
				{
				  real t0 = getCPU();
				  int ns = tmpi.isGhost() ? 2*nsearch : nsearch;
				  bool ug = true;//tmpi.isGhost() || useGhostInReconstruction;

				  unstructuredLink(umap, links, tmpi, nhops, ns, eAdj, ug);
				  adjSearchTime += getCPU()-t0;
				}



			      if ( !useCleanLSR )
				extractDSIReconstructionCoefficients( umap, tmpi, links, 
								      edgeAreaNormals, REeqCoeff, REeqIdx, nnzC,peri);
			      else
				computeDSIReconstructionCoefficients( umap, tmpi, links, 
								      edgeAreaNormals, REeqCoeff, REeqIdx, nnzC,peri);
			      //			      extractDSIReconstructionCoefficients( umap, tmpi, uElinks[*aiter], 
			      //								    edgeAreaNormals, REeqCoeff, REeqIdx, nnzC,peri);
			      if (dbgLSR)
				cout<<"found "<<nnzC<<" coeffs for "<<*tmpi<<endl;
			      //			      REeqCoeff.resize(nnzC,rDim);
			      //			      REeqIdx.resize(nnzC);
			    }
			  else if (dbgLSR)
			    cout<<"did NOT call extract DSIC for iter "<<*tmpi<<", nnzC = "<<nnzC<<endl;
			   

			  assert(nnzC);
			  for ( int c=0/*REoffset(*aiter)*/; c<nnzC/*REoffset(*aiter+1)*/; c++ )
			    {
			      int i=0;
			      bool found=false;
			      while ( i<nnz && Hindex(Hoffset(*iter)+i)!=REeqIdx(c) ) i++;
			      
			      assert( (Hindex(Hoffset(*iter)+i)==REeqIdx(c)) || i==nnz );
			      
			      if ( i==nnz )
				{
				  Hindex(Hoffset(*iter)+i) = REeqIdx(c);
				  nnz++;
				}
			      
			      real cf = ve[0]*REeqCoeff(c,0) + ve[1]*REeqCoeff(c,1) ;
			      if ( rDim==3 ) cf += ve[2]*REeqCoeff(c,2);
			      if ( rDim==2) cf /= cFArea(*iter,0,0);//*cFArea(*iter,0,0));

			      Hcoeff(Hoffset(*iter)+i) -= sgn * ( cf )/mu;
			    }
			  
			}
		      else
			{
			  int i=0;
			  bool found=false;
			  while ( i<nnz && Hindex(Hoffset(*iter)+i)!=(*aiter) ) i++;
			      
			  assert( (Hindex(Hoffset(*iter)+i)==(*aiter)) || i==nnz );
			  
			  if ( i==nnz )
			    {
			      Hindex(Hoffset(*iter)+i) = *aiter;
			      nnz++;
			    }
			  
			  real cf = sqrt(vemag)/cEArea(*aiter,0,0);
			  if ( rDim==2) cf /= cFArea(*iter,0,0);//*cFArea(*iter,0,0));
			  Hcoeff(Hoffset(*iter)+i) -= sgn * ( cf )/mu;
			}
		    }
		  real sgnCheckMag=0.;
		  for ( int a=0; a<rDim; a++ )
		    sgnCheckMag += sgnCheck[a]*sgnCheck[a];
		  assert(sgnCheckMag < REAL_EPSILON);
		  assert(nneg!=0);
		  assert(npos!=0);
		  nnZH += nnz;

		  if ( toggleDisp && useVariableDissipation)
		    {
		      UnstructuredMappingAdjacencyIterator vit, vit_end;
		      vit_end = umap.adjacency_end(iter,UnstructuredMapping::Vertex);
		      for ( vit=umap.adjacency_begin(iter,UnstructuredMapping::Vertex);
			    vit!=vit_end;
			    vit++ )
			{
			  vertexDispCoeff(*vit) = 1;
			}
		    }
		}
	    }
	  Hoffset( Hoffset.size(0)-1 ) = nnZH;

	  for ( iter=umap.begin(faceType); useVariableDissipation && iter!=iter_end; iter++ )
	    {
	      UnstructuredMappingAdjacencyIterator vit, vit_end;
	      vit_end = umap.adjacency_end(iter,UnstructuredMapping::Vertex);
	      int avg=0;
	      for ( vit=umap.adjacency_begin(iter,UnstructuredMapping::Vertex);
		    vit!=vit_end;
		    vit++ )
		{
		  avg+=vertexDispCoeff(*vit);
		}

	      dispCoeff( *iter ) *= (real(avg)/real(vit.nAdjacent()));
	    }
	}
    }
  
  RECcache.resize(0);
  REIcache.resize(0);

  // ghost boundaries will not have contributed non-zeros so nnZ{H,E} will in general be smaller than initially guessed
  uElinks.resize(0);

  Hcoeff.resize(nnZH);
  Hindex.resize(nnZH);

  //      cout<<Eoffset<<endl;
  //      cout<<Hoffset<<endl;
  //cout<<Ecoeff<<endl;
  //      cout<<Eindex<<endl;
  // now shift index and coeff arrays to make them 1 based indices

  //  cout<<REcoeff<<endl;
  //  cout<<REindex<<endl;
  //  cout<<REoffset<<endl;

  for ( int i=0; i<Eoffset.size(); i++ )
    Eoffset[i]++;
  for ( int i=0; i<Eindex.size(); i++ )
    Eindex[i]++;
  
  for ( int i=0; i<Hoffset.size(); i++ )
    Hoffset[i]++;
  for ( int i=0; i<Hindex.size(); i++ )
    Hindex[i]++;

  int job=1,ierr=0;

  int nrow = Hoffset.size(0)-1;
  int ncol = Eoffset.size(0)-1;
  int nnz = Hcoeff.size(0);
  real ftol = 100*REAL_MIN;

  fixupPEC( cg, Hindex, Hcoeff, perfectElectricalConductor );

  //#define FILTERM
#ifdef FILTERM
  cout<<"prefilter H size = "<<Hoffset(Hoffset.size(0)-1)<<endl;
  cout<<"prefilter E size = "<<Eoffset(Eoffset.size(0)-1)<<endl;
  F90_ID(filter,FILTER) (&nrow,&job,&ftol,Hcoeff.ptr(),Hindex.ptr(),Hoffset.ptr(),
			 Hcoeff.ptr(),Hindex.ptr(),Hoffset.ptr(),&nnz,&ierr);

  Hindex.resize(Hoffset(Hoffset.size(0)-1)-1);
  Hcoeff.resize(Hoffset(Hoffset.size(0)-1)-1); 

  nnz=Ecoeff.size(0); 
  F90_ID(filter,FILTER) (&ncol,&job,&ftol,Ecoeff.ptr(),Eindex.ptr(),Eoffset.ptr(),
			 Ecoeff.ptr(),Eindex.ptr(),Eoffset.ptr(),&nnz,&ierr);

  Eindex.resize(Eoffset(Eoffset.size(0)-1)-1);
  Ecoeff.resize(Eoffset(Eoffset.size(0)-1)-1);
  cout<<"postfilter H size = "<<Hoffset(Hoffset.size(0)-1)<<endl;
  cout<<"postilter E size = "<<Eoffset(Eoffset.size(0)-1)<<endl;
#endif
  //       if ( cg.numberOfDimensions()==3 ) 
  // 	return;
  
  //      cout<<nrow<<"  "<<ncol<<endl;

  if ( buildDissipationOperator && artificialDissipation>REAL_MIN ) 
    {
      ArraySimple<int> ndegr(nrow), iw(max(nrow,ncol));
      nnz = 0;
  
      F90_ID(amubdg,AMUBDG) ( &nrow,&ncol,&nrow, Hindex.ptr(), Hoffset.ptr(), Eindex.ptr(),Eoffset.ptr(),
			      ndegr.ptr(), & nnz, iw.ptr() );
  
      //   ArraySimple<int> CCindex(nnz), CCoffset(nrow+1);
      //   ArraySimple<real> CCcoeff(nnz);
      Dindex.resize(nnz);
      Doffset.resize(nrow+1);
      Dcoeff.resize(nnz);
      
      cout<<"NNZERO IN CURL CURL IS "<<nnz<<", "<<sizeof(real)*nnz<<endl;
  

  
      F90_ID(amub,AMUB) ( &nrow,&ncol,&job,
			  Hcoeff.ptr(), Hindex.ptr(), Hoffset.ptr(),
			  Ecoeff.ptr(), Eindex.ptr(), Eoffset.ptr(),
			  Dcoeff.ptr(), Dindex.ptr(), Doffset.ptr(),
			  &nnz, iw.ptr(), &ierr);
  
      if ( ierr )
	cout<<"ERROR IN AMUB "<<ierr<<endl;
  
#if 0
      Dindex = CCindex;
      Doffset = CCoffset;
      Dcoeff = CCcoeff;
#endif
 
      for ( int i=0; i<Dcoeff.size() ; i++ )
	{
	  Dcoeff[i] *= -1; // since the operator for the curl of curl H has a -ve sign in it for -curl E
	  //	  CCcoeff[i] *= -1;
	}
  
      if ( useVariableDissipation ) 
	{
	  int nnDH=0; // the running nonzero count for the curlcurl
	  for ( int f=0; useVariableDissipation && f<dispCoeff.size(); f++ )
	    {
	      real dc = dispCoeff(f);
	      int doo = Doffset(f)-1;
	      Doffset(f) = nnDH+1;
	      if ( dc > REAL_MIN ) 
		{
		  for ( int n=doo; n<Doffset(f+1)-1; n++ )
		    {
		      Dcoeff(nnDH) = Dcoeff(n);//*dc;///(dt*dt); 
		      //		      CCcoeff(nnDH) = CCcoeff(n)*dc;///(dt*dt);
		      Dindex(nnDH) = Dindex(n);
		      //		      CCindex(nnDH) = CCindex(n);
		      nnDH++;
		    }
		}
	    }
	  Dcoeff.resize(nnDH);
	  Dindex.resize(nnDH);
	  Doffset(Doffset.size(0)-1) = nnDH+1;
	  //	  CCcoeff.resize(nnDH);
	  //	  CCindex.resize(nnDH);
	  //	  CCoffset(CCoffset.size(0)-1) = nnDH+1;
	}
      else
	{ // scaling now handled in advanceUnstructured
	  for ( int f=0; f<dispCoeff.size() && false; f++ )
	    {
	      abort();
	      real dc = dispCoeff(f);
	      for ( int n=Doffset(f)-1; n<Doffset(f+1)-1; n++ )
		{
		  Dcoeff(n) *= dc;///(dt*dt); 
		  //		  CCcoeff(n) *= dc;///(dt*dt);
		}
	    }
	}
  

      // now we need to add the equations for the boundaries
#if 0
      int nnDH=0; // the running nonzero count for the curlcurl
      ArraySimple<real> DcoeffOld = Dcoeff;
      ArraySimple<int> DindexOld = Dindex, 	      DoffsetOld  = Doffset;
  
  
      for ( int grid=0;  grid<cg.numberOfGrids() ; grid++ )
	{
	  MappedGrid &mg = cg[grid];
      
	  //      int eqoE = eqOffsetE(grid);
	  int eqoH = eqOffsetH(grid);
      
	  if ( mg.getGridType()==MappedGrid::structuredGrid )
	    {
	      abort();
	    }
	  else
	    {
	      UnstructuredMapping &umap = (UnstructuredMapping &)mg.mapping().getMapping();
	      UnstructuredMappingIterator iter,iter_end;
	  
	      // // // PERIODIC BC
	      // 	      std::string perTag = std::string("periodic ") + UnstructuredMapping::EntityTypeStrings[int(UnstructuredMapping::Face)].c_str();
	      // 	      std::string ghostTag = std::string("Ghost ")+UnstructuredMapping::EntityTypeStrings[int(UnstructuredMapping::Face)].c_str();
	      // 	      UnstructuredMapping::tag_entity_iterator git, git_end;
	      // 	      git =  umap.tag_entity_begin(ghostTag);
	      // 	      git_end = umap.tag_entity_end(ghostTag);
	  
	      int nnzToAdd = 0;
	      //	      for ( ; git!=git_end ; git++ )
	      //		nnzToAdd += 2;
	      // #if 0
	      // 	      iter_end = umap.end(faceType);
	      // 	      for ( iter=umap.begin(faceType); iter!=iter_end; iter++ )
	      // 		{
	      // 		  if ( iter.isGhost() )
	      // 		    {
	      // 		      if ( umap.hasTag(faceType,*iter,perTag) )
	      // 			{
	      // 			  //			  Doffset(eqoH+*iter) = nnDH+1;
	      // 			  int ep = (int)umap.getTagData(faceType, *iter, perTag);
	      // 			  nnzToAdd += DoffsetOld(eqoH+ep+1)-DoffsetOld(eqoH+ep);
	      // 			  //			  Dindex(nnDH) = eqoH+*iter + 1;
	      // 			  //			  Dcoeff(nnDH) = 1;
	      // 			  //			  nnDH++;
	      // 			  //			  Dindex(nnDH) = eqoH+ep + 1;
	      // 			  //			  Dcoeff(nnDH) = -1;
	      // 			  //			  nnDH++;
	      // 			}
	      // 		    }
	      // 		}
	      // #else
	      if ( mg.getUnstructuredPeriodicBC(faceType) )
		{
		  const IntegerArray &perImages = *mg.getUnstructuredPeriodicBC(faceType);
		  for ( int p=0;  p<perImages.getLength(0); p++ )
		    {
		      nnzToAdd += DoffsetOld(eqoH+perImages(p,1)+1)-DoffsetOld(eqoH+perImages(p,1));
		  
		  
		    }
		}
	  
	      if ( mg.getUnstructuredBCInfo( UnstructuredMapping::Edge ) )
		{
		  const IntegerArray &bci = *mg.getUnstructuredBCInfo( UnstructuredMapping::Edge );
	      
		  nnzToAdd += bci.getLength(0);
		}
	  
	      //#endif
	      bool vCent = mg.isAllVertexCentered();
	      const realArray &cFNorm = vCent ? mg.centerNormal() : mg.faceNormal();
	  
	      Dcoeff.resize(Dcoeff.size()+nnzToAdd);
	      Dindex.resize(Dcoeff.size()+nnzToAdd);
	      iter_end = umap.end(faceType);
	      int perloc=0;
	  
	      for ( iter=umap.begin(faceType); iter!=iter_end; iter++ )
		{
		  int nadj = umap.adjacency_begin(iter,UnstructuredMapping::Region).nAdjacent();
		  if (  iter.isGhost() /*|| nadj>1*/)
		    {
		      //if ( umap.hasTag(faceType,*iter,perTag) )
		      if ( mg.getUnstructuredPeriodicBC(faceType) && mg.getUnstructuredPeriodicBC(faceType)->getLength(0) && (*mg.getUnstructuredPeriodicBC(faceType))(perloc,0)==*iter)
			{
			  const IntegerArray &perImages = *mg.getUnstructuredPeriodicBC(faceType);
		      
			  if ( (*iter)==perImages(perloc,0) /*DoffsetOld(eqoH + *iter)<0*/ )
			    {
			      //			  DoffsetOld(eqoH + *iter) = -DoffsetOld(eqoH + *iter);
			      int ep = perImages(perloc,1);//(int)umap.getTagData(faceType, *iter, perTag);
			      perloc++;
			      Doffset(eqoH+*iter) = nnDH+1;
			  
			      int c1 = *iter;
			      int c2 = ep;
			  
			      int sgn = ( rDim==3 ? ( cFNorm(c1,0,0,0)*cFNorm(c2,0,0,0) + 
						      cFNorm(c1,0,0,1)*cFNorm(c2,0,0,1) + 
						      cFNorm(c1,0,0,2)*cFNorm(c2,0,0,2) ) > 0 : true ) ? 1 : -1;
			  
			  
			      for ( int c=DoffsetOld(eqoH+ep)-1; c<DoffsetOld(eqoH+ep+1)-1; c++ )
				{
				  assert(DindexOld(c)!=(eqoH+*iter+1));
				  //			      Dindex(nnDH) = DindexOld(c)==(eqoH+*iter+1) ? eqoH+*iter+1 : DindexOld(c);
				  Dindex(nnDH) = DindexOld(c)==(eqoH+ep+1) ? eqoH+*iter+1 : DindexOld(c);
				  Dcoeff(nnDH) = /*DindexOld(c)==(eqoH+ep+1) ?*/ sgn*DcoeffOld(c) /*: DcoeffOld(c)*/ ;
				  nnDH++;
				}
			  
			    }
			}
		      else
			{
			  Doffset(eqoH+*iter) = nnDH+1;
			}
		  
		    }
		  else
		    {
		      int doo = DoffsetOld(eqoH+*iter)-1;
		      Doffset(eqoH+*iter) = nnDH+1;
		      for ( int i=doo; i<DoffsetOld(eqoH+*iter+1)-1; i++ )
			{
			  Dcoeff(nnDH) = DcoeffOld(i);
			  Dindex(nnDH) = DindexOld(i);
			  nnDH++;
			}
		    }
		}
	  
	    }
      
	}
    
      //      assert(nnDH==Dcoeff.size(0));
      Dindex.resize(nnDH);
      Dcoeff.resize(nnDH);

      Doffset(Doffset.size(0)-1) = nnDH +1;
#endif

      //        cout<<"DOFFSET "<<Doffset<<endl;
      //      cout<<"DINDEX "<<Dindex<<endl;
      //      cout<<"DCOEFF "<<Dcoeff<<endl;
#if 0
            CCoffset = Doffset;
            CCindex = Dindex;
            CCcoeff = Dcoeff;
#endif


      for ( int d=0; false && d<(orderOfArtificialDissipation/2-1); d++ )
	{

	  //	  if ( orderOfArtificialDissipation==2 )
	    cout<<"HEY, WHAT ARE YOU DOING HERE!"<<endl;

	  ArraySimple<int> Tindex=Dindex, Toffset=Doffset;
	  ArraySimple<real> Tcoeff=Dcoeff;

	  F90_ID(amubdg,AMUBDG) ( &nrow,&ncol,&nrow, CCindex.ptr(), CCoffset.ptr(), Dindex.ptr(),Doffset.ptr(),
				  ndegr.ptr(), & nnz, iw.ptr() );

	  Dindex.resize(nnz);
	  //	  Doffset(ncol+1);
	  Dcoeff.resize(nnz);

	  F90_ID(amub,AMUB) ( &nrow,&ncol,&job,
			      CCcoeff.ptr(), CCindex.ptr(), CCoffset.ptr(),
			      Tcoeff.ptr(), Tindex.ptr(), Toffset.ptr(),
			      Dcoeff.ptr(), Dindex.ptr(), Doffset.ptr(),
			      &nnz, iw.ptr(), &ierr);

	  // kkc always -1 with curl curl	  sgn*=-1;
	  int nnDH=0;
	  ArraySimple<int> DindexOld = Dindex, 	      DoffsetOld  = Doffset;
	  ArraySimple<real> DcoeffOld;
	  DcoeffOld = Dcoeff;

	  for ( int grid=0; grid<cg.numberOfGrids() ; grid++ )
	    {
	      MappedGrid &mg = cg[grid];
	      
	      //      int eqoE = eqOffsetE(grid);
	      int eqoH = eqOffsetH(grid);
	      
	      if ( mg.getGridType()==MappedGrid::structuredGrid )
		{
		  abort();
		}
	      else
		{
		  UnstructuredMapping &umap = (UnstructuredMapping &)mg.mapping().getMapping();
		  UnstructuredMappingIterator iter,iter_end;
		  
		  // // // PERIODIC BC
		  // 		  std::string perTag = std::string("periodic ") + UnstructuredMapping::EntityTypeStrings[int(UnstructuredMapping::Face)].c_str();
		  // 		  std::string ghostTag = std::string("Ghost ")+UnstructuredMapping::EntityTypeStrings[int(UnstructuredMapping::Face)].c_str();
		  // 		  UnstructuredMapping::tag_entity_iterator git, git_end;
		  // 		  git =  umap.tag_entity_begin(ghostTag);
		  // 		  git_end = umap.tag_entity_end(ghostTag);
		  
		  iter_end = umap.end(faceType);

		  bool vCent = mg.isAllVertexCentered();
		  const realArray &cFNorm = vCent ? mg.centerNormal() : mg.faceNormal();
		  //	      if ( mg.getUnstructuredPeriodicBC(faceType) )
		  //		{
		  //		  const IntegerArray &perImages = *mg.getUnstructuredPeriodicBC(faceType);
		  //		  for ( int p=0; p<perImages.getLength(0); p++ )
		  //		    {
		  // DoffsetOld(i) will be set to negative if it has a periodic image
		  //               we will use this when constructing bc equations to find
		  //               out which faces are periodic
		  //		      if ( DoffsetOld(eqoH+perImages(p,0))>0 ) 
		  //			DoffsetOld(eqoH+perImages(p,0))  = -DoffsetOld(eqoH+perImages(p,0));
		  //		    }

		  int perloc = 0;
		  for ( iter=umap.begin(faceType); iter!=iter_end; iter++ )
		    {
		      int nadj = umap.adjacency_begin(iter,UnstructuredMapping::Region).nAdjacent();
		      if ( /*iter.isGhost() &&*/ nadj>1 )
			{
			  //			  if ( umap.hasTag(faceType,*iter,perTag) )
			  if ( mg.getUnstructuredPeriodicBC(faceType) && mg.getUnstructuredPeriodicBC(faceType)->getLength(0) && (*mg.getUnstructuredPeriodicBC(faceType))(perloc,0)==*iter)
			    {
			      const IntegerArray &perImages = *mg.getUnstructuredPeriodicBC(faceType);
			      if ( (*iter)==perImages(perloc,0) )/*DoffsetOld(eqoH + *iter)<0*/ 
				{
				  //			      DoffsetOld(eqoH + *iter) = -DoffsetOld(eqoH + *iter);
				  Doffset(eqoH+*iter) = nnDH+1;
				  //			      int ep = (int)umap.getTagData(faceType, *iter, perTag);
				  int ep = perImages(perloc,1);//(int)umap.getTagData(faceType, *iter, perTag);
				  perloc++;
				  Doffset(eqoH+*iter) = nnDH+1;
				  
				  int c1 = *iter;
				  int c2 = ep;
				  
				  int sgn = ( rDim==3 ? ( cFNorm(c1,0,0,0)*cFNorm(c2,0,0,0) + 
							  cFNorm(c1,0,0,1)*cFNorm(c2,0,0,1) + 
							  cFNorm(c1,0,0,2)*cFNorm(c2,0,0,2) ) > 0 : true ) ? 1 : -1;
				  
				  
				  for ( int c=DoffsetOld(eqoH+ep)-1; c<DoffsetOld(eqoH+ep+1)-1; c++ )
				    {
				      //				  assert(DindexOld(c)!=(eqoH+*iter+1));
				      //				  Dindex(nnDH) = DindexOld(c)==(eqoH+*iter+1) ? eqoH+*iter+1 : DindexOld(c);
				      Dindex(nnDH) = DindexOld(c)==(eqoH+ep+1) ? eqoH+*iter+1 : DindexOld(c);
				      Dcoeff(nnDH) = /*DindexOld(c)==(eqoH+ep+1) ?*/ sgn*DcoeffOld(c) /*: DcoeffOld(c) */; 
				      nnDH++;
				    }
				  
				  // 			  Doffset(eqoH+*iter) = nnDH+1;
				  // 			  int ep = (int)umap.getTagData(faceType, *iter, perTag);
				  // 			  Dindex(nnDH) = eqoH+*iter + 1;
				  // 			  Dcoeff(nnDH) = 1;
				  // 			  nnDH++;
				  // 			  Dindex(nnDH) = eqoH+ep + 1;
				  // 			  Dcoeff(nnDH) = -1;
				  // 			  nnDH++;
				}
			    }
			  else
			    {
			      //			      cout<<"ERROR : can only handle periodic boundaries in matvec for now!"<<endl;
			      Doffset(eqoH+*iter) = nnDH+1;
			    }
			}
		      else
			{
			  int doo = DoffsetOld(eqoH+*iter)-1;
			  assert(DoffsetOld(eqoH+*iter)>0);
			  Doffset(eqoH+*iter) = nnDH+1;
			  for ( int i=doo; i<DoffsetOld(eqoH+*iter+1)-1; i++ )
			    {
			      Dcoeff(nnDH) = DcoeffOld(i);
			      Dindex(nnDH) = DindexOld(i);
			      nnDH++;
			    }
			}
		      
		    }
		  //}
		}
	    }
	}
  
      //      cout<<"CDISS * SGN = "<<cdiss*sgn<<endl;
  
#if 0
      ArraySimple<real> AMATcoeff;
      ArraySimple<int> AMATindex,AMAToffset;
      AMATcoeff.resize(Ecoeff.size()+Hcoeff.size());
      AMATcoeff = 0.;
      AMATindex.resize(Ecoeff.size()+Hcoeff.size());
      AMAToffset.resize(Ecoeff.size()+Hcoeff.size());
      job=1;// the - sign is alread in the H operator
      int nnzata = Ecoeff.size()+Hcoeff.size();
      iw.resize(nnzata);
      if ( rDim==2 )
	{
	  for ( int r=0; r<nrow; r++ )
	    {
	      real dc = dispCoeff(r);
	      for ( int o=Hoffset(r)-1; o<Hoffset(r+1)-1; o++ )
		Hcoeff(o)*=dc;
	    }
	}

      F90_ID(apmbt,APMBT) ( &nrow,&ncol,&job,
			    Hcoeff.ptr(), Hindex.ptr(), Hoffset.ptr(),
			    Ecoeff.ptr(), Eindex.ptr(), Eoffset.ptr(),
			    AMATcoeff.ptr(), AMATindex.ptr(), AMAToffset.ptr(),
			    &nnzata, iw.ptr(), &ierr);
      real maxrn = 0;
      ArraySimple<real> dccoeff( nrow );
      dccoeff = 0.;
      int dcoff = 0;
      for ( int r=0; r<nrow; r++ )
	{
	  real dc = dispCoeff(r);
	  for ( int n=CCoffset(r)-1; n<CCoffset(r+1)-1; n++ )
	    if ( fabs(dc)>REAL_MIN )
	      CCcoeff(n) /= dc;

	  real rsum = 0;
	  int nsum=0;
	  for ( int o=Hoffset(r)-1; o<Hoffset(r+1)-1; o++,nsum++ )
	    rsum += fabs(Hcoeff(o));
	  rsum /= real(nsum);

	  real rmax = 0;
	  //      cout<<"r = "<<r<<" : ";
	  for ( int o=AMAToffset(r)-1; o<AMAToffset(r+1)-1; o++ )
	    {
	      //  cout<<AMATindex(o)-1<<"  "<<AMATcoeff(o)<<", ";
	      rmax = max(rmax,fabs(AMATcoeff(o)));
	    }
	  //      cout<<endl;
	  if ( rsum<REAL_MIN ) rsum=1;
	  int doo = Doffset(r);
	  Doffset(r) = dcoff+1;
	  if ( rmax /*/rsum*/ > fabs(dispCoeff(r))/*REAL_EPSILON*/ )
	    {
	      for ( int n=doo-1; n<Doffset(r+1)-1; n++ )
		{
		  Dcoeff(dcoff) = Dcoeff(n);//*artificialDissipationInterval*cdiss*sgn; 
		  Dindex(dcoff) = Dindex(n);
		  dcoff++;
		}
	    }
	  if ( rDim==2 )
	    {
	      //	  real dc = dispCoeff(r);
	      for ( int o=Hoffset(r)-1; o<Hoffset(r+1)-1; o++ )
		Hcoeff(o)/=dc;
	    }
	}
      Doffset(Doffset.size(0)-1) = dcoff+1;

      Dcoeff.resize(dcoff);
      Dindex.resize(dcoff);
#endif

      // build matrices for energy computation
      //   ArraySimple<real> At;
      //   ArraySimple<int> Atoffset, Atindex;

      //   At=CCcoeff;
      //   Atindex = CCindex;
      //   Atoffset = CCoffset;

      //  cout<<CCcoeff<<endl;
#if 0
      cout<<"curl H"<<endl;
      F90_INTEGER r1=1, r2=Eoffset.size(0)-1;
      F90_LOGICAL values = true;
      F90_INTEGER iout = 6;
      F90_ID(dump,DUMP) ( &r1, &r2, &values, Ecoeff.ptr(), Eindex.ptr(), Eoffset.ptr(),&iout);

      cout<<endl<<endl;

      cout<<"curl E"<<endl;
      r1=1, r2=Hoffset.size(0)-1;
      F90_ID(dump,DUMP) ( &r1, &r2, &values, Hcoeff.ptr(), Hindex.ptr(), Hoffset.ptr(),&iout);

      cout<<endl<<endl;

  

      string title="DSI Curl Curl Matrix";
      string munt="IN";
      int ptitle=0;
      float size=5;
      int mode=0;
      int iunt=1;
      int nlines=0;
      int lines;

      string fname = "curlcurl_matrix.ps";
      F90_ID(f90_fopen,F90_FOPEN)(&iunt,FCDTOCP(fname.c_str()) CPLENARG(fname.length()));
      F90_ID(pspltm,PSPLTM) (&nrow,&nrow,&mode,CCindex.ptr(), CCoffset.ptr(),FCDTOCP(title.c_str()),
			     &ptitle,&size,FCDTOCP(munt.c_str()),&nlines,&lines,&iunt
			     CPLENARG(title.length()) CPLENARG(munt.length()));
      F90_ID(f90_fclose,F90_FCLOSE)(&iunt);


      fname = "dissp4_matrix.ps";
      title="Dissipation Matrix";

      F90_ID(f90_fopen,F90_FOPEN)(&iunt,FCDTOCP(fname.c_str()) CPLENARG(fname.length()));
      F90_ID(pspltm,PSPLTM) (&nrow,&nrow,&mode,Dindex.ptr(), Doffset.ptr(),FCDTOCP(title.c_str()),
			     &ptitle,&size,FCDTOCP(munt.c_str()),&nlines,&lines,&iunt
			     CPLENARG(title.length()) CPLENARG(munt.length()));
      F90_ID(f90_fclose,F90_FCLOSE)(&iunt);

      fname = "curlE.ps";
      title="Curl E Matrix";

      F90_ID(f90_fopen,F90_FOPEN)(&iunt,FCDTOCP(fname.c_str()) CPLENARG(fname.length()));
      F90_ID(pspltm,PSPLTM) (&nrow,&ncol,&mode,Hindex.ptr(), Hoffset.ptr(),FCDTOCP(title.c_str()),
			     &ptitle,&size,FCDTOCP(munt.c_str()),&nlines,&lines,&iunt
			     CPLENARG(title.length()) CPLENARG(munt.length()));
      F90_ID(f90_fclose,F90_FCLOSE)(&iunt);

      fname = "curlH.ps";
      title="Curl E Matrix";

      F90_ID(f90_fopen,F90_FOPEN)(&iunt,FCDTOCP(fname.c_str()) CPLENARG(fname.length()));
      F90_ID(pspltm,PSPLTM) (&ncol,&nrow,&mode,Eindex.ptr(), Eoffset.ptr(),FCDTOCP(title.c_str()),
			     &ptitle,&size,FCDTOCP(munt.c_str()),&nlines,&lines,&iunt
			     CPLENARG(title.length()) CPLENARG(munt.length()));
      F90_ID(f90_fclose,F90_FCLOSE)(&iunt);
#endif
    }
  //        cout<<Dcoeff<<endl;
  
  cout<<"FDOT_MIN = "<<fdot_min<<", 1-FDOT_MIN = "<<1.-fdot_min<<", 1/(1-fdot_min) = "<<1./(1.-fdot_min)<<endl;

  int sgn= buildDissipationOperator ? -1 : 1;
  real cdiss = artificialDissipation;

  for ( int f=0; false && f<dispCoeff.size() ; f++ )
    {
      dispCoeff(f) *= artificialDissipationInterval*cdiss*sgn; 
    }

  for ( int e=0; false && e<E_dispCoeff.size() ; e++ )
    {
      E_dispCoeff(e) *= artificialDissipationInterval*cdiss*sgn; 
    }

  cout<<sizeof(real)*Ecoeff.size()<<endl;
  cout<<sizeof(real)*Hcoeff.size()<<endl;
  cout<<sizeof(real)*Dcoeff.size()<<endl;
  //  cout<<sizeof(real)*RHcoeff.size()<<endl;
  //  cout<<sizeof(real)*REcoeff.size()<<endl;

  cout<<sizeof(int)*( Eindex.size() + Eoffset.size() +
		      Hindex.size() + Hoffset.size() +
		      Dindex.size() + Doffset.size() )<<endl;


  cout<<"rcond min = "<<minRC<<", rcond max = "<<maxRC<<endl;
  cout<<"neqFound min = "<<minNGB<<", neqFound max = "<<maxNGB<<endl;
  cout<<"maxDC = "<<maxDC<<endl;
  cout<<"adjSearchTime = "<<adjSearchTime<<endl;
  cout<<"lscTime       = "<<lscTime<<endl;
  cout<<"extcTime      = "<<extcTime<<"  "<<extcCalls<<endl;
  cout<<"extcTime_1      = "<<extcTime_1<<endl;
  cout<<"extcTime_2      = "<<extcTime_2<<endl;
  cout<<"csetTime      = "<<csetTime<<"  "<<endl;
  cout<<"minEngb = "<<minEngb<<" : at "<<minEloc<<endl;
  cout<<"minEdgeDist = "<<sqrt(minEdgeDist)<<endl;
}

void 
Maxwell::
reconstructDSIField( real t, Maxwell::FieldEnum field, 
		     realMappedGridFunction &from, realMappedGridFunction &to )
{

  ::useGhostInReconstruction = this->useGhostInReconstruction;
  //  if( from.grid->numberOfDimensions==2 )
  //    return;


  if ( from.grid->numberOfDimensions==2 && (!new2d || field==HField) )
    {
      //      to.updateToMatchGridFunction(from);
      to = from;
      return;
    }


  assert(field==EField || field==HField);

//   if (  field==HField )
//     {
//       cout<<"******** RHindex "<<RHindex<<endl;
//       cout<<"******** RHoffset "<<RHoffset<<endl;
//     }
  MappedGrid  &mg = *from.getMappedGrid();
  UnstructuredMapping &umap = (UnstructuredMapping &)mg.mapping().getMapping();
  int rDim = umap.getRangeDimension();

  bool vCent = mg.isAllVertexCentered();
  realArray &cFArea = vCent ? mg.centerArea() : mg.faceArea();
  const realArray &cFNorm = vCent ? mg.centerNormal() : mg.faceNormal();
  const realArray &cEArea = vCent ? mg.faceArea() : mg.centerArea();
  const realArray &cENorm = vCent ? mg.faceNormal() : mg.centerNormal();
  
  // 	  cFArea.display("cFArea");
  // 	  cFNorm.display("cFNorm");
  // 	  cEArea.display("cEArea");
  // 	  cENorm.display("cENorm");
  
  const realArray &verts = mg.vertex();
  
#if 0 
  realArray normals;
 
  if ( field==HField )
    {
      normals.redim(cFNorm.getLength(0), cFNorm.getLength(1), cFNorm.getLength(2), rDim);
      for ( int i3=cFArea.getBase(2); i3<=cFArea.getBound(2); i3++ )
	for ( int i2=cFArea.getBase(1); i2<=cFArea.getBound(1); i2++ )
	  for ( int i1=cFArea.getBase(0); i1<=cFArea.getBound(0); i1++ )
	    for ( int a=0; a<rDim; a++ )
	      normals(i1,i2,i3,a) = cFArea(i1,i2,i3)*cFNorm(i1,i2,i3,a);
    }
  else
    {
      normals.redim(cENorm.getLength(0), cENorm.getLength(1), cENorm.getLength(2), rDim);
      for ( int i3=cEArea.getBase(2); i3<=cEArea.getBound(2); i3++ )
	for ( int i2=cEArea.getBase(1); i2<=cEArea.getBound(1); i2++ )
	  for ( int i1=cEArea.getBase(0); i1<=cEArea.getBound(0); i1++ )
	    for ( int a=0; a<rDim; a++ )
	      normals(i1,i2,i3,a) = cEArea(i1,i2,i3)*cENorm(i1,i2,i3,a);
    }
#else
  realArray &normals = field==EField ?  edgeAreaNormals : faceAreaNormals;
#endif
  
  UnstructuredMapping::EntityTypeEnum etype = field==EField ? UnstructuredMapping::Edge : UnstructuredMapping::Face;
  int nE = umap.size( etype );
  UnstructuredMappingIterator iter,iter_end;
  
  assert(nE==normals.getLength(0));
  
  if ( field==EField && cacheRCoeff && !REcoeff.size() )
    {
      REcoeff.resize(nE);
      REindex.resize(nE);
    }
  else if ( field==HField && cacheRCoeff && !RHcoeff.size() )
    {
      RHcoeff.resize(nE);
      RHindex.resize(nE);
    }
  
  int nDeg = 1; //linear reconstruction
  int nEqg = 10; // initial guess
  int MsubCol = 1;
  MsubCol = rDim==2 ? 3 : 4;
  
  int nTerm = MsubCol;
  int nadd = rDim*nTerm;//iter.isGhost() ? rDim : 1;
  //    int nsearch = rDim*nTerm + nadd;
  //    nsearch *= 2;
  
  int nnZR=0, nnZRL=0, nnZRmax=0;
  
  iter_end=umap.end(etype);
  
  nnZRmax=20;
  ArraySimple<real> eqCoeff_tmp(nnZRmax,3);
  ArraySimple<int> eqIdx_tmp(nnZRmax);
  
  iter_end = umap.end(etype);
  int idx=0;
  int ne=0;
  //  const IntegerArray &perImages = *mg.getUnstructuredPeriodicBC(etype);
  int maxNNZ = 0;
  int minNNZ = INT_MAX;
  int avgNNZ = 0;
  ArraySimple< UnstructuredMappingAdjacencyIterator > adjEnts(nnZRmax);
  for ( iter=umap.begin(etype); iter!=iter_end; iter++ )
    {
      if ( true )
	{
	  int nnz=0;
	  eqCoeff_tmp = 0;

	  //	  ArraySimple< UnstructuredMappingAdjacencyIterator > &adjEnts = ulinks[*iter];
	  // the following saves memory even if it slows things down a bit
	  
	  real t0=getCPU();

	  ArraySimple< ArraySimple<real> > &Rcoeff = field==HField ? RHcoeff : REcoeff;
	  ArraySimple< ArraySimple<int> > &Rindex = field==HField ? RHindex : REindex;

	  if ( !cacheRCoeff || !Rcoeff(*iter).size() )
	    {
	      int ns = iter.isGhost() ? 2*nsearch : nsearch;
	      bool ug = true;//iter.isGhost() || useGhostInReconstruction;
	      if ( etype==UnstructuredMapping::Face )
		unstructuredLink(umap, adjEnts, iter, nhops, ns, hAdj,ug );
	      else
		unstructuredLink(umap, adjEnts, iter, nhops, ns, eAdj,ug );
	      
	      adjSearchTime += getCPU()-t0;
	      if ( eqCoeff_tmp.size(0)<(adjEnts.size(0)+1) )
		{
		  eqCoeff_tmp.resize(adjEnts.size(0)+1,3);
		  eqIdx_tmp.resize(adjEnts.size(0)+1);
		}
	      
	      //       if ( etype==UnstructuredMapping::Edge && *iter==14344 )
	      // 	{
	      // 	  for ( int a=0; a<adjEnts.size(); a++ )
	      // 	    cout<<"Highlight Entity Edge "<<*adjEnts[a]<<endl;
	      // 	}
	      
	      
	      if ( dbgLSR && iter.isGhost() )
		{
		  cout<<"adj edges("<<*iter<<")= ";
		  for ( int ii=0; ii<adjEnts.size(); ii++ )
		    cout<<"("<<adjEnts[ii].orientation()<<")"<<*adjEnts[ii]<<"  ";
		  cout<<endl;
		}
	      //	  extractDSIReconstructionCoefficients( umap, iter, ulinks[*iter], normals, eqCoeff, eqIdx, nnz);
	      if ( !useCleanLSR )
		extractDSIReconstructionCoefficients( umap, iter, adjEnts, normals, eqCoeff_tmp, eqIdx_tmp, nnz);
	      else
		computeDSIReconstructionCoefficients( umap, iter, adjEnts, normals, eqCoeff_tmp, eqIdx_tmp, nnz);
	      if ( cacheRCoeff )
		{
		  Rcoeff(*iter) = eqCoeff_tmp;
		  Rindex(*iter) = eqIdx_tmp;
		  Rindex(*iter).resize(nnz);
		}
	    }
	  else
	    nnz = Rindex(*iter).size(0);

	  ArraySimple<real> & eqCoeff = cacheRCoeff ? Rcoeff(*iter) : eqCoeff_tmp;
	  ArraySimple<int> & eqIdx = cacheRCoeff ? Rindex(*iter) : eqIdx_tmp;
      
	  assert(nnz>0);
	  minNNZ = min(minNNZ,nnz);
	  maxNNZ = max(maxNNZ,nnz);
	  avgNNZ+= nnz;
	  for ( int a=0; a<rDim; a++ )
	    to(*iter,0,0,a) = 0;

#if 0
	  IntegerArray eext(1);
	  RealArray fullField(1,rDim);
	  eext(0) = *iter;
	  reconstructDSIAtEntities( t, field, eext, from, fullField);
	    for ( int a=0; a<rDim; a++ )
	      to(*iter,0,0,a) = fullField(0,a);
#else
	  for ( int e=0; e<nnz; e++ )
	    for ( int a=0; a<rDim; a++ )
	      to(*iter,0,0,a) += from(eqIdx(e),0,0)*eqCoeff(e,a);
#endif    

	}
    }

//   cout<<"adjSearchTime (R) = "<<adjSearchTime<<endl;
//   cout<<"lscTime       (R) = "<<lscTime<<endl;
//   cout<<"extcTime      (R) = "<<extcTime<<"  "<<extcCalls<<endl;
//   cout<<"extcTime_1      = "<<extcTime_1<<endl;
//   cout<<"extcTime_2      = "<<extcTime_2<<endl;
//   cout<<"csetTime      (R)= "<<csetTime<<"  "<<endl;
//   cout<<"min, avg, max nnz = "<<minNNZ<<",  "<<avgNNZ/umap.size(etype)<<",  "<<maxNNZ<<endl;
  
//   int nrow = field==EField ? REoffset.size(0)-1 : RHoffset.size(0)-1;
//   F90_REAL8 *fromPtr = from.Array_Descriptor.Array_View_Pointer1;
//   F90_REAL8 *toPtr   = to.Array_Descriptor.Array_View_Pointer1;
//   F90_REAL8 *coeffPtr = field==EField ? REcoeff.ptr() : RHcoeff.ptr();
//   F90_INTEGER *indexPtr = field==EField ? REindex.ptr() : RHindex.ptr();
//   F90_INTEGER *offsetPtr = field==EField ? REoffset.ptr() : RHoffset.ptr();

//   F90_ID(amux,AMUX) ( &nrow, fromPtr, toPtr, coeffPtr, indexPtr, offsetPtr );

  //  applyDSIBC(to, t,field==EField, false);

}

bool
Maxwell::
reconstructDSIAtEntities( real t, FieldEnum field, IntegerArray &entities, 
			  realMappedGridFunction &from, RealArray &to)
{
  ::useGhostInReconstruction = this->useGhostInReconstruction;

  if ( from.grid->numberOfDimensions==2 && (!new2d || field==HField) )
    {
      //      to.updateToMatchGridFunction(from);
      //      to = from;
      to.redim(entities.getLength(0),1);
      for ( int p=0; p<entities.getLength(0); p++ )
	to(p,0) = from(p,0,0,0);

      return true;
    }

  assert(field==EField || field==HField);

  MappedGrid  &mg = *from.getMappedGrid();
  UnstructuredMapping &umap = (UnstructuredMapping &)mg.mapping().getMapping();
  int rDim = umap.getRangeDimension();

  to.redim(entities.getLength(0), rDim);
  to = 0.;
  
  bool vCent = mg.isAllVertexCentered();
  realArray &cFArea = vCent ? mg.centerArea() : mg.faceArea();
  const realArray &cFNorm = vCent ? mg.centerNormal() : mg.faceNormal();
  const realArray &cEArea = vCent ? mg.faceArea() : mg.centerArea();
  const realArray &cENorm = vCent ? mg.faceNormal() : mg.centerNormal();
  
  const realArray &verts = mg.vertex();
  
#if 0
  realArray normals;
  
  if ( field==HField )
    {
      normals.redim(cFNorm.getLength(0), cFNorm.getLength(1), cFNorm.getLength(2), rDim);
      for ( int i3=cFArea.getBase(2); i3<=cFArea.getBound(2); i3++ )
	for ( int i2=cFArea.getBase(1); i2<=cFArea.getBound(1); i2++ )
	  for ( int i1=cFArea.getBase(0); i1<=cFArea.getBound(0); i1++ )
	    for ( int a=0; a<rDim; a++ )
	      normals(i1,i2,i3,a) = cFArea(i1,i2,i3)*cFNorm(i1,i2,i3,a);
    }
  else
    {
      normals.redim(cENorm.getLength(0), cENorm.getLength(1), cENorm.getLength(2), rDim);
      for ( int i3=cEArea.getBase(2); i3<=cEArea.getBound(2); i3++ )
	for ( int i2=cEArea.getBase(1); i2<=cEArea.getBound(1); i2++ )
	  for ( int i1=cEArea.getBase(0); i1<=cEArea.getBound(0); i1++ )
	    for ( int a=0; a<rDim; a++ )
	      normals(i1,i2,i3,a) = cEArea(i1,i2,i3)*cENorm(i1,i2,i3,a);
    }
#else
  realArray &normals = field==EField ?  edgeAreaNormals : faceAreaNormals;
#endif

  UnstructuredMapping::EntityTypeEnum etype = field==EField ? UnstructuredMapping::Edge : UnstructuredMapping::Face;
  int nE = umap.size( etype );
  UnstructuredMappingIterator iter,iter_end;
  
  if ( field==EField && cacheRCoeff && !REcoeff.size() )
    {
      REcoeff.resize(nE);
      REindex.resize(nE);
    }
  else if ( field==HField && cacheRCoeff && !RHcoeff.size() )
    {
      RHcoeff.resize(nE);
      RHindex.resize(nE);
    }      
  
  int nDeg = 1; //linear reconstruction
  int nEqg = 10; // initial guess
  int MsubCol = 1;
  MsubCol = rDim==2 ? 3 : 4;
  
  int nTerm = MsubCol;
  
  int nnZR=0, nnZRL=0, nnZRmax=0;
  
  iter_end=umap.end(etype);
  
  nnZRmax=20;
  ArraySimple<real> eqCoeff_tmp(nnZRmax,3);
  ArraySimple<int> eqIdx_tmp(nnZRmax);
  
  iter_end = umap.end(etype);
  int idx=0;
  int ne=0;
  //  const IntegerArray &perImages = *mg.getUnstructuredPeriodicBC(etype);
  
  ArraySimple< UnstructuredMappingAdjacencyIterator > adjEnts(nnZRmax);
  //  for ( iter=umap.begin(etype); iter!=iter_end; iter++ )
  iter = umap.begin(etype);
  bool allOk = true;
  for ( int p=0; p<entities.getLength(0); p++ )
    {
      if ( !iter.setLocation(entities(p)) )
	{
	  cout<<"Invalid entity for reconstruction = "<<entities(p)<<endl;
	  abort();
	}

      int nnz=0;
      eqCoeff_tmp = 0;
      
      //	  ArraySimple< UnstructuredMappingAdjacencyIterator > &adjEnts = ulinks[*iter];
      // the following saves memory even if it slows things down a bit
      int nadd = rDim*nTerm;//iter.isGhost() ? rDim : 1;
      //      int nsearch = rDim*nTerm + nadd;
      //      nsearch *= 2;
      
      ArraySimple< ArraySimple<real> > &Rcoeff = field==HField ? RHcoeff : REcoeff;
      ArraySimple< ArraySimple<int> > &Rindex = field==HField ? RHindex : REindex;

      if ( !cacheRCoeff || !Rcoeff(*iter).size() )
	{
	  real t0=getCPU();
	  int ns = (iter.isGhost() || !::useGhostInReconstruction)? 2*nsearch : nsearch;
	  bool ug = true;//iter.isGhost() || useGhostInReconstruction;
	  
	  if ( etype==UnstructuredMapping::Face )
	    unstructuredLink(umap, adjEnts, iter, nhops, ns, hAdj,ug );
	  else
	    unstructuredLink(umap, adjEnts, iter, nhops, ns, eAdj,ug);
	  
	  adjSearchTime += getCPU()-t0;
	  if ( eqCoeff_tmp.size(0)<(adjEnts.size(0)+1) )
	    {
	      eqCoeff_tmp.resize(adjEnts.size(0)+1,3);
	      eqIdx_tmp.resize(adjEnts.size(0)+1);
	    }
	  
	  if ( dbgLSR && iter.isGhost() )
	    {
	      cout<<"adj edges("<<*iter<<")= ";
	      for ( int ii=0; ii<adjEnts.size(); ii++ )
		cout<<"("<<adjEnts[ii].orientation()<<")"<<*adjEnts[ii]<<"  ";
	      cout<<endl;
	    }
	  //	  extractDSIReconstructionCoefficients( umap, iter, ulinks[*iter], normals, eqCoeff, eqIdx, nnz);
	  if ( !useCleanLSR )
	    extractDSIReconstructionCoefficients( umap, iter, adjEnts, normals, eqCoeff_tmp, eqIdx_tmp, nnz);
	  else
	    allOk = allOk && computeDSIReconstructionCoefficients( umap, iter, adjEnts, normals, eqCoeff_tmp, eqIdx_tmp, nnz);
	  if ( cacheRCoeff )
	    {
	      Rcoeff(*iter) = eqCoeff_tmp;
	      Rindex(*iter) = eqIdx_tmp;
	      Rindex(*iter).resize(nnz);
	    }
	}
      else
	nnz=Rindex(*iter).size(0);

      ArraySimple<real> & eqCoeff = cacheRCoeff ? Rcoeff(*iter) : eqCoeff_tmp;
      ArraySimple<int> & eqIdx = cacheRCoeff ? Rindex(*iter) : eqIdx_tmp;
      
      assert(nnz>0);
	  
      for ( int a=0; a<rDim; a++ )
	to(p,a) = 0;
      
      for ( int e=0; e<nnz; e++ )
	{
	  for ( int a=0; a<rDim; a++ )
	    {
	      //	    if ( fabs(from(eqIdx(e),0,0))>1e-10 ) cout<<"HEY ! : "<<e<<" : "<<eqIdx(e)<<"  : "<<from(eqIdx(e),0,0)<<endl;
	      //	      cout<<from(eqIdx(e),0,0)<<"  "<<eqCoeff(e,a)<<endl;
	      to(p,a) += from(eqIdx(e),0,0)*eqCoeff(e,a);
	    }
	}
      
    }

  return allOk;
//   cout<<"adjSearchTime (RAE) = "<<adjSearchTime<<endl;
//   cout<<"lscTime       (RAE) = "<<lscTime<<endl;
//   cout<<"extcTime      (RAE) = "<<extcTime<<"  "<<extcCalls<<endl;
//   cout<<"csetTime      (RAE)= "<<csetTime<<"  "<<endl;
  
//   int nrow = field==EField ? REoffset.size(0)-1 : RHoffset.size(0)-1;
//   F90_REAL8 *fromPtr = from.Array_Descriptor.Array_View_Pointer1;
//   F90_REAL8 *toPtr   = to.Array_Descriptor.Array_View_Pointer1;
//   F90_REAL8 *coeffPtr = field==EField ? REcoeff.ptr() : RHcoeff.ptr();
//   F90_INTEGER *indexPtr = field==EField ? REindex.ptr() : RHindex.ptr();
//   F90_INTEGER *offsetPtr = field==EField ? REoffset.ptr() : RHoffset.ptr();

//   F90_ID(amux,AMUX) ( &nrow, fromPtr, toPtr, coeffPtr, indexPtr, offsetPtr );

  //  applyDSIBC(to, t,field==EField, false);
}
	      
