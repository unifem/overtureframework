/*
 * $Header: /usr/gapps/overture/OvertureRepoCVS/overture/Overture/hdf/fc.h,v 1.3 1999/12/11 00:21:36 henshaw Exp $
 *
 * Conversions from Fortran to C
 *
 */

/*
 * Definitions for use of int or long.
 */
#ifdef LONGINT
#define Int long
#else
#define Int int
#endif

#ifdef DOUBLE
#define Float double
#else
#define Float float
#endif

/* Definitions for conversion of Fortran character arguments to C */

#ifdef STR_DESC
typedef struct {
  char* Addr;
  int Length;
 } Str_Desc;
#endif

/* Definitions for conversions of Fortran subroutine names */

#ifdef STR_LEN
#define cgtin_  cgtin_c_
#define cgtout_ cgtout_c_
#define cgsetp_ cgsetp_c_
#define cgcrsr_ cgcrsr_c_
#define dskfop_ dskfop_c_
#endif

#ifdef NO_UNDER
#define cgwsid_ cgwsid
#define cgscws_ cgscws
#define cgtin_  cgtin
#define cgtout_ cgtout 
#define cgsetp_ cgsetp
#define cgrset_ cgrset 
#define cgstpl_ cgstpl 
#define cgndpl_ cgndpl
#define cgeras_ cgeras 
#define cgwndo_ cgwndo 
#define cgshad_ cgshad
#define cgmove_ cgmove 
#define cgdraw_ cgdraw 
#define cgmark_ cgmark
#define cgcrsr_ cgcrsr 
#define dskcal_ dskcal 
#define dskcfr_ dskcfr 
#define dskfop_ dskfop 
#define dskfxt_ dskfxt
#define dskfcl_ dskfcl 
#define dskfre_ dskfre 
#define dskfwr_ dskfwr
#define cgoptm_ cgoptm 
#define cgcltm_ cgcltm 
#define cgread_ cgread
#define cgwrit_ cgwrit 
#define cgslep_ cgslep 
#define second_ second
#define ior_    ior       
#define iand_   iand     
#define ishft_  ishft
#define ibcopy_ ibcopy
#define g1setp_ g1setp
#define g1rset_ g1rset
#define g1opws_ g1opws
#define g1clws_ g1clws
#define g1tin_  g1tin
#define g1crsr_ g1crsr
#define g1tout_ g1tout
#define g1cci_  g1cci
#define g1ccn_  g1ccn
#define g1gcol_ g1gcol
#define gparv_  gparv
#define gpclst_ gpclst
#define gpcmt3_ gpcmt3
#define gpest_  gpest
#define gpdlst_ gpdlst
#define gppl2_  gppl2
#define gppl3_  gppl3
#define gpdpl3_ gpdpl3
#define gpdrv_  gpdrv
#define gpmssc_ gpmssc
#define gpmt_   gpmt
#define gpopst_ gpopst
#define gpplcd_ gpplcd
#define gpplci_ gpplci
#define gppm2_  gppm2
#define gppm3_  gppm3
#define gppmcd_ gppmcd
#define gppmci_ gppmci
#define gprotx_ gprotx
#define gproty_ gproty
#define gprotz_ gprotz
#define gpsc3_  gpsc3
#define gptrl3_ gptrl3
#define gpupws_ gpupws
#define gpvp_   gpvp
#define gpvch_  gpvch
#define gpvmp3_ gpvmp3
#define gpmlx3_ gpmlx3
#define gpvmt3_ gpvmt3
#define gpadcn_ gpadcn
#define gpbici_ gpbici
#define gpbsci_ gpbsci
#define gpbspr_ gpbspr
#define gpdcm_  gpdcm
#define gpecd_  gpecd
#define gpeci_  gpeci
#define gpef_   gpef
#define gpfdmo_ gpfdmo
#define gphid_  gphid
#define gpici_  gpici
#define gpis_   gpis
#define gpivf_  gpivf
#define gplmo_  gplmo
#define gplsr_  gplsr
#define gplss_  gplss
#define gppgd3_ gppgd3
#define gprcn_  gprcn
#define gpsci_  gpsci
#define gpspr_  gpspr
#define gpxvr_  gpxvr
#define gpchh_  gpchh
#define gptx2_  gptx2
#define gptx3_  gptx3
#define gptxal_ gptxal
#define gptxpt_ gptxpt
#define gpchup_ gpchup
#define gptxfo_ gptxfo
#define gptxpr_ gptxpr
#define gptxci_ gptxci
#define gppg2_  gppg2
#define gppg3_  gppg3
#define gpicd_  gpicd
#define gpbicd_ gpbicd
#define gpscd_  gpscd
#define gpbscd_ gpbscd
#define gplwsc_ gplwsc
#define gpxcr_  gpxcr
#define gpqcch_ gpqcch
#define gpshdf_ gpshdf
#define gpawev_ gpawev
#define gprast_ gprast
#define gpqlsr_ gpqlsr
#define gpqrct_ gpqrct
#define gpqhmo_ gpqhmo
#define gpqopw_ gpqopw
#define gpqcvr_ gpqcvr
#endif

#ifdef NO_LOWER
#define cgwsid_ CGWSID
#define cgscws_ CGSCWS
#define cgtin_  CGTIN   
#define cgtout_ CGTOUT 
#define cgsetp_ CGSETP
#define cgrset_ CGRSET 
#define cgstpl_ CGSTPL 
#define cgndpl_ CGNDPL
#define cgeras_ CGERAS 
#define cgwndo_ CGWNDO 
#define cgshad_ CGSHAD
#define cgmove_ CGMOVE 
#define cgdraw_ CGDRAW 
#define cgmark_ CGMARK
#define cgcrsr_ CGCRSR 
#define dskcal_ DSKCAL 
#define dskcfr_ DSKCFR 
#define dskfop_ DSKFOP 
#define dskfxt_ DSKFXT
#define dskfcl_ DSKFCL 
#define dskfre_ DSKFRE 
#define dskfwr_ DSKFWR
#define cgoptm_ CGOPTM 
#define cgcltm_ CGCLTM 
#define cgread_ CGREAD
#define cgwrit_ CGWRIT 
#define cgslep  CGSLEP  
#define second_ SECOND
#define ior_    IOR       
#define iand_   IAND     
#define ishft_  ISHFT
#define ibcopy_ IBCOPY
#define g1setp_ G1SETP
#define g1rset_ G1RSET
#define g1opws_ G1OPWS
#define g1clws_ G1CLWS
#define g1tin_  G1TIN
#define g1crsr_ G1CRSR
#define g1tout_ G1TOUT
#define g1cci_  G1CCI
#define g1ccn_  G1CCN
#define g1gcol_ G1GCOL
#define gparv_  GPARV
#define gpclst_ GPCLST
#define gpcmt3_ GPCMT3
#define gpest_  GPEST
#define gpdlst_ GPDLST
#define gppl2_  GPPL2
#define gppl3_  GPPL3
#define gpdpl3_ GPDPL3
#define gpdrv_  GPDRV
#define gpmssc_ GPMSSC
#define gpmt_   GPMT
#define gpopst_ GPOPST
#define gpplcd_ GPPLCD
#define gpplci_ GPPLCI
#define gppm2_  GPPM2
#define gppm3_  GPPM3
#define gppmcd_ GPPMCD
#define gppmci_ GPPMCI
#define gprotx_ GPROTX
#define gproty_ GPROTY
#define gprotz_ GPROTZ
#define gpsc3_  GPSC3
#define gptrl3_ GPTRL3
#define gpupws_ GPUPWS
#define gpvp_   GPVP
#define gpvch_  GPVCH
#define gpvmp3_ GPVMP3
#define gpmlx3_ GPMLX3
#define gpvmt3_ GPVMT3
#define gpadcn_ GPADCN
#define gpbici_ GPBICI
#define gpbsci_ GPBSCI
#define gpbspr_ GPBSPR
#define gpdcm_  GPDCM
#define gpecd_  GPECD
#define gpeci_  GPECI
#define gpef_   GPEF
#define gpfdmo_ GPFDMO
#define gphid_  GPHID
#define gpici_  GPICI
#define gpis_   GPIS
#define gpivf_  GPIVF
#define gplmo_  GPLMO
#define gplsr_  GPLSR
#define gplss_  GPLSS
#define gppgd3_ GPPGD3
#define gprcn_  GPRCN
#define gpsci_  GPSCI
#define gpspr_  GPSPR
#define gpxvr_  GPXVR
#define gpchh_  GPCHH
#define gptx2_  GPTX2
#define gptx3_  GPTX3
#define gptxal_ GPTXAL
#define gptxpt_ GPTXPT
#define gpchup_ GPCHUP
#define gptxfo_ GPTXFO
#define gptxpr_ GPTXPR
#define gptxci_ GPTXCI
#define gppg2_  GPPG2
#define gppg3_  GPPG3
#define gpicd_  GPICD
#define gpbicd_ GPBICD
#define gpscd_  GPSCD
#define gpbscd_ GPBSCD
#define gplwsc_ GPLWSC
#define gpxcr_  GPXCR
#define gpqcch_ GPQCCH
#define gpshdf_ GPSHDF
#define gpawev_ GPAWEV
#define gprast_ GPRAST
#define gpqlsr_ GPQLSR
#define gpqrct_ GPQRCT
#define gpqhmo_ GPQHMO
#define gpqopw_ GPQOPW
#define gpqcvr_ GPQCVR
#endif
