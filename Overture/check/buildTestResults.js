//
//  Results of the build tests
//  *** The results that appear here are automatically changed by checkOverture.p ****
//

var configure=0, buildRap=1, build=2, buildGrids=3, gridFunctions=4, testRap=5, oges=6;
var buildOverBlown=1, testOverBlown=2;

numberOfTests =0;

if( packageName == 'Overture' )
{
  numberOfTests = 6;   // number of tests we currently perform.
  tests[configure]='Configure';
  tests[buildRap]='Rapsodi';
  tests[build]='Overture';
  tests[buildGrids]='GG Tests';
  tests[gridFunctions]='GF/OP';
  tests[testRap]='Rap Tests';
  tests[oges]='Oges tests'; // future test
}
if( packageName == 'OverBlown' )
{
  numberOfTests = 3;
  tests[configure]='Configure';
  tests[buildOverBlown]='Build';
  tests[testOverBlown]='Test';
}    


// ---- results -----

// user = 'henshaw';

var p=-1;



// // ***** results for the SUN, Single precision, CC ******
// p++;
// var SUN_S_CC=p;
// platforms[SUN_S_CC]= 'Sun Solaris 2.6';
// compiler[p] = 'CC,sp, debug';
// comment[SUN_S_CC]='dilbert [Mon Mar 25 19:29:25 US/Pacific 2002]';
// testStatus[SUN_S_CC] = new Array( NA, NA, NA, NA, NA, NA);
// if( packageName == 'Overture' )
// {
// testStatus[SUN_S_CC][build]=UNKNOWN;
// testStatus[SUN_S_CC][buildGrids]=UNKNOWN;
// testStatus[SUN_S_CC][gridFunctions]=UNKNOWN;
// // AP Add entry for configure and rap
// testStatus[SUN_S_CC][configure]=UNKNOWN;
// testStatus[SUN_S_CC][buildRap]=UNKNOWN;
// testStatus[SUN_S_CC][testRap]=UNKNOWN;
// }
// if( packageName == 'OverBlown' )
// {
// testStatus[SUN_S_CC][configure]=UNKNOWN;
// testStatus[SUN_S_CC][buildOverBlown]=UNKNOWN;
// testStatus[SUN_S_CC][testOverBlown]=UNKNOWN;
// }

// ***** results for SUN, double precision, CC *****
// p++;
// var SUN_D_CC=p;
// platforms[SUN_D_CC]= 'Sun Solaris sun.d.CC';
// compiler[SUN_D_CC] = 'CC,dp, debug';
// comment[SUN_D_CC]='dilbert [Mon Mar 25 19:30:10 US/Pacific 2002]';
// testStatus[SUN_D_CC] = new Array( NA, NA, NA, NA, NA, NA);
// if( packageName == 'Overture' )
// {
// testStatus[SUN_D_CC][build]=UNKNOWN;
// testStatus[SUN_D_CC][buildGrids]=UNKNOWN;
// testStatus[SUN_D_CC][gridFunctions]=UNKNOWN;
// // AP Add entry for configure and rap
// testStatus[SUN_D_CC][configure]=UNKNOWN;
// testStatus[SUN_D_CC][buildRap]=UNKNOWN;
// testStatus[SUN_D_CC][testRap]=UNKNOWN;
// }
// if( packageName == 'OverBlown' )
// {
// testStatus[SUN_D_CC][configure]=UNKNOWN;
// testStatus[SUN_D_CC][buildOverBlown]=UNKNOWN;
// testStatus[SUN_D_CC][testOverBlown]=UNKNOWN;
// }

// // ***** results for SUN, double precision, CC, parallel *****
// p++;
// var SUN_D_CC_MPI=p;
// platforms[SUN_D_CC_MPI]= 'Sun Solaris 2.6';
// compiler[SUN_D_CC_MPI] = 'CC,dp, debug,mpi';
// comment[SUN_D_CC_MPI]='dilbert [Mon Mar 25 17:27:40 US/Pacific 2002]';
// testStatus[SUN_D_CC_MPI] = new Array( NA, NA, NA, NA, NA, NA);
// if( packageName == 'Overture' )
// {
// testStatus[SUN_D_CC_MPI][build]=UNKNOWN;
// testStatus[SUN_D_CC_MPI][buildGrids]=NA;
// testStatus[SUN_D_CC_MPI][gridFunctions]=NA;
// // AP Add entry for configure and rap
// testStatus[SUN_D_CC_MPI][configure]=UNKNOWN;
// testStatus[SUN_D_CC_MPI][buildRap]=UNKNOWN;
// testStatus[SUN_D_CC_MPI][testRap]=NA;
// }
// if( packageName == 'OverBlown' )
// {
// testStatus[SUN_D_CC_MPI][configure]=NA;
// testStatus[SUN_D_CC_MPI][buildOverBlown]=NA;
// testStatus[SUN_D_CC_MPI][testOverBlown]=NA;
// }

// ***** results for DEC, double precision, cxx ******
p++;
var DEC_D_CXX=p;
platforms[DEC_D_CXX]= 'Dec Alpha dec.d.cxx';
compiler[DEC_D_CXX] = 'cxx,dp, debug';
comment[DEC_D_CXX]='dilbert [Mon Mar 25 16:57:44 2002]';
testStatus[DEC_D_CXX] = new Array( NA, NA, NA, NA, NA, NA);
if( packageName == 'Overture' )
{
testStatus[DEC_D_CXX][build]=UNKNOWN;
testStatus[DEC_D_CXX][buildGrids]=UNKNOWN;
testStatus[DEC_D_CXX][gridFunctions]=UNKNOWN;
// AP Add entry for configure and rap
testStatus[DEC_D_CXX][configure]=UNKNOWN;
testStatus[DEC_D_CXX][buildRap]=UNKNOWN;
testStatus[DEC_D_CXX][testRap]=UNKNOWN;
}
if( packageName == 'OverBlown' )
{
testStatus[DEC_D_CXX][configure]=UNKNOWN;
testStatus[DEC_D_CXX][buildOverBlown]=UNKNOWN;
testStatus[DEC_D_CXX][testOverBlown]=UNKNOWN;
}

// // ***** results for DEC, single precision, cxx ******
// p++;
// var DEC_S_CXX=p;
// platforms[DEC_S_CXX]= 'Dec Alpha';
// compiler[DEC_S_CXX] = 'cxx,sp, debug';
// comment[DEC_S_CXX]='dilbert [Mon Mar 25 16:57:36 2002]';
// testStatus[DEC_S_CXX] = new Array( NA, NA, NA, NA, NA, NA);
// if( packageName == 'Overture' )
// {
// testStatus[DEC_S_CXX][build]=UNKNOWN;
// testStatus[DEC_S_CXX][buildGrids]=UNKNOWN;
// testStatus[DEC_S_CXX][gridFunctions]=UNKNOWN;
// // AP Add entry for configure and rap
// testStatus[DEC_S_CXX][configure]=UNKNOWN;
// testStatus[DEC_S_CXX][buildRap]=UNKNOWN;
// testStatus[DEC_S_CXX][testRap]=UNKNOWN;
// }
// if( packageName == 'OverBlown' )
// {
// testStatus[DEC_S_CXX][configure]=UNKNOWN;
// testStatus[DEC_S_CXX][buildOverBlown]=UNKNOWN;
// testStatus[DEC_S_CXX][testOverBlown]=UNKNOWN;
// }

// ***** results for DEC, double precision, cxx, mpi ******
p++;
var DEC_D_CXX_MPI=p;
platforms[DEC_D_CXX_MPI]= 'Dec Alpha dec.d.cxx.mpi';
compiler[DEC_D_CXX_MPI] = 'cxx,dp, debug,mpi';
comment[DEC_D_CXX_MPI]='dilbert [Mon Mar 25 16:57:44 2002]';
testStatus[DEC_D_CXX_MPI] = new Array( NA, NA, NA, NA, NA, NA);
if( packageName == 'Overture' )
{
testStatus[DEC_D_CXX_MPI][build]=UNKNOWN;
testStatus[DEC_D_CXX_MPI][buildGrids]=NA;
testStatus[DEC_D_CXX_MPI][gridFunctions]=NA;
// AP Add entry for configure and rap
testStatus[DEC_D_CXX_MPI][configure]=UNKNOWN;
testStatus[DEC_D_CXX_MPI][buildRap]=UNKNOWN;
testStatus[DEC_D_CXX_MPI][testRap]=NA;
}
if( packageName == 'OverBlown' )
{
testStatus[DEC_D_CXX][configure]=UNKNOWN;
testStatus[DEC_D_CXX][buildOverBlown]=UNKNOWN;
testStatus[DEC_D_CXX][testOverBlown]=NA;
}

// ***** results for Intel, double precision, gcc, pgf77 ******
p++;
var INTEL_D_GCC=p;
platforms[INTEL_D_GCC]= 'Intel Linux intel.d.gcc';
compiler[INTEL_D_GCC] = 'gcc,pgf77, dp,debug';
comment[INTEL_D_GCC]='dilbert [Mon Mar 25 15:59:08 2002]';
testStatus[INTEL_D_GCC] = new Array( NA, NA, NA,  NA, NA, NA);
if( packageName == 'Overture' )
{
testStatus[INTEL_D_GCC][build]=UNKNOWN;
testStatus[INTEL_D_GCC][buildGrids]=UNKNOWN;
testStatus[INTEL_D_GCC][gridFunctions]=UNKNOWN;
// AP Add entry for configure and rap
testStatus[INTEL_D_GCC][configure]=UNKNOWN;
testStatus[INTEL_D_GCC][buildRap]=UNKNOWN;
testStatus[INTEL_D_GCC][testRap]=UNKNOWN;
}
if( packageName == 'OverBlown' )
{
testStatus[INTEL_D_GCC][configure]=UNKNOWN;
testStatus[INTEL_D_GCC][buildOverBlown]=UNKNOWN;
testStatus[INTEL_D_GCC][testOverBlown]=UNKNOWN;
}

// ***** results for Intel, single precision, gcc, pgf77 ******
p++;
var INTEL_S_GCC=p;
platforms[INTEL_S_GCC]= 'Intel Linux intel.s.gcc';
compiler[INTEL_S_GCC] = 'gcc,pgf77, sp,debug';
comment[INTEL_S_GCC]='dilbert [Mon Mar 25 15:59:08 2002]';
testStatus[INTEL_S_GCC] = new Array( NA, NA, NA,  NA, NA, NA);
if( packageName == 'Overture' )
{
testStatus[INTEL_S_GCC][build]=UNKNOWN;
testStatus[INTEL_S_GCC][buildGrids]=UNKNOWN;
testStatus[INTEL_S_GCC][gridFunctions]=UNKNOWN;
// AP Add entry for configure and rap
testStatus[INTEL_S_GCC][configure]=UNKNOWN;
testStatus[INTEL_S_GCC][buildRap]=UNKNOWN;
testStatus[INTEL_S_GCC][testRap]=UNKNOWN;
}
if( packageName == 'OverBlown' )
{
testStatus[INTEL_S_GCC][configure]=UNKNOWN;
testStatus[INTEL_S_GCC][buildOverBlown]=UNKNOWN;
testStatus[INTEL_S_GCC][testOverBlown]=UNKNOWN;
}

// ***** results for Intel, double precision, gcc, fort77 ******
p++;
var INTEL_D_GCC_FORT77=p;
platforms[INTEL_D_GCC_FORT77]= 'Intel Linux intel.d.gcc.fort77';
compiler[INTEL_D_GCC_FORT77] = 'gcc,fort77, dp,debug';
comment[INTEL_D_GCC_FORT77]='dilbert [Mon Mar 25 15:59:08 2002]';
testStatus[INTEL_D_GCC_FORT77] = new Array( NA, NA, NA,  NA, NA, NA);
if( packageName == 'Overture' )
{
testStatus[INTEL_D_GCC_FORT77][build]=UNKNOWN;
testStatus[INTEL_D_GCC_FORT77][buildGrids]=UNKNOWN;
testStatus[INTEL_D_GCC_FORT77][gridFunctions]=UNKNOWN;
// AP Add entry for configure and rap
testStatus[INTEL_D_GCC_FORT77][configure]=UNKNOWN;
testStatus[INTEL_D_GCC_FORT77][buildRap]=UNKNOWN;
testStatus[INTEL_D_GCC_FORT77][testRap]=UNKNOWN;
}
if( packageName == 'OverBlown' )
{
testStatus[INTEL_D_GCC_FORT77][configure]=UNKNOWN;
testStatus[INTEL_D_GCC_FORT77][buildOverBlown]=UNKNOWN;
testStatus[INTEL_D_GCC_FORT77][testOverBlown]=UNKNOWN;
}

// ***** results for Intel, single precision, gcc, g77 ******
p++;
var INTEL_S_GCC_G77=p;
platforms[INTEL_S_GCC_G77]= 'Intel Linux intel.s.gcc.g77';
compiler[INTEL_S_GCC_G77] = 'gcc,g77, dp,debug';
comment[INTEL_S_GCC_G77]='dilbert [Mon Mar 25 15:59:08 2002]';
testStatus[INTEL_S_GCC_G77] = new Array( NA, NA, NA,  NA, NA, NA);
if( packageName == 'Overture' )
{
testStatus[INTEL_S_GCC_G77][build]=UNKNOWN;
testStatus[INTEL_S_GCC_G77][buildGrids]=UNKNOWN;
testStatus[INTEL_S_GCC_G77][gridFunctions]=UNKNOWN;
// AP Add entry for configure and rap
testStatus[INTEL_S_GCC_G77][configure]=UNKNOWN;
testStatus[INTEL_S_GCC_G77][buildRap]=UNKNOWN;
testStatus[INTEL_S_GCC_G77][testRap]=UNKNOWN;
}
if( packageName == 'OverBlown' )
{
testStatus[INTEL_S_GCC_G77][configure]=UNKNOWN;
testStatus[INTEL_S_GCC_G77][buildOverBlown]=UNKNOWN;
testStatus[INTEL_S_GCC_G77][testOverBlown]=UNKNOWN;
}

// ***** results for Intel, double precision, gcc, ifc (Intel fortran compiler) ******
p++;
var INTEL_D_GCC_IFC=p;
platforms[INTEL_D_GCC_IFC]= 'Intel Linux intel.d.gcc.ifc';
compiler[INTEL_D_GCC_IFC] = 'gcc,ifc, dp,debug';
comment[INTEL_D_GCC_IFC]='dilbert [Mon Mar 25 15:59:08 2002]';
testStatus[INTEL_D_GCC_IFC] = new Array( NA, NA, NA,  NA, NA, NA);
if( packageName == 'Overture' )
{
testStatus[INTEL_D_GCC_IFC][build]=UNKNOWN;
testStatus[INTEL_D_GCC_IFC][buildGrids]=UNKNOWN;
testStatus[INTEL_D_GCC_IFC][gridFunctions]=UNKNOWN;
// AP Add entry for configure and rap
testStatus[INTEL_D_GCC_IFC][configure]=UNKNOWN;
testStatus[INTEL_D_GCC_IFC][buildRap]=UNKNOWN;
testStatus[INTEL_D_GCC_IFC][testRap]=UNKNOWN;
}
if( packageName == 'OverBlown' )
{
testStatus[INTEL_D_GCC_IFC][configure]=UNKNOWN;
testStatus[INTEL_D_GCC_IFC][buildOverBlown]=UNKNOWN;
testStatus[INTEL_D_GCC_IFC][testOverBlown]=UNKNOWN;
}


// ***** results for SGI, double precision, CC ******
// p++;
// var SGI_D_CC=p;
// platforms[SGI_D_CC]= 'SGI irix sgi.d.CC';
// compiler[SGI_D_CC] = 'CC,dp, debug';
// comment[SGI_D_CC]='dilbert [Mon Mar 25 15:59:08 2002]';
// testStatus[SGI_D_CC] = new Array( NA, NA, NA,  NA, NA, NA);
// if( packageName == 'Overture' )
// {
// testStatus[SGI_D_CC][build]=UNKNOWN;
// testStatus[SGI_D_CC][buildGrids]=UNKNOWN;
// testStatus[SGI_D_CC][gridFunctions]=UNKNOWN;
// testStatus[SGI_D_CC][configure]=UNKNOWN;
// testStatus[SGI_D_CC][buildRap]=UNKNOWN;
// testStatus[SGI_D_CC][testRap]=UNKNOWN;
// }
// if( packageName == 'OverBlown' )
// {
// testStatus[SGI_D_CC][configure]=UNKNOWN;
// testStatus[SGI_D_CC][buildOverBlown]=UNKNOWN;
// testStatus[SGI_D_CC][testOverBlown]=UNKNOWN;
// }

// Total number of platforms
numberOfPlatforms = p+1;
