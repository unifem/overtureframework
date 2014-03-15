* Process command line options using GetOpt::Long, 
* GetOpt::Std is less "ugly" but also less flexible.  Note that
* both Std and Long are imported by OvertureParser.
*
GetOptions( "grid=s"=>\$baseGrid,  \
            "pde=s"=> \$pde, \
	    "resolution=i"=>\$res, \
	    "tztype=s"=>\$tztype, \
	    "space_degree=i"=>\$polySpace,\
	    "time_degree=i"=>\$polyTime, \
            "tmax=f"=> \$tmax, \
	    "debug=i"=>\$debug, \
	    "oges_debug=i"=>\$oges_debug, \
	    "cfl=f"=>\$cfl, \
	    "av2=f"=>\$av2, \
	    "av4=f"=>\$av4, \
	    "f_t=f"=>\$f_t, \
	    "f_x=f"=>\$f_x, \
	    "f_y=f"=>\$f_y, \
	    "f_z=f"=>\$f_z, \
            "implicit_factor=f"=>\$ifac, \
            "bc=s"=>\$bc, \
	    "strick=f"=>\$strick,\
	    "maxit=i"=>\$maxit);
use OverBlownConvergenceTestOptions;
*
* fix up some defaults
if ( !$res ) {$res = 0;};
if ( !$tmax ) {$tmax = 1.;};
*if ( $polySpace ) {$polySpace = 2;};
*if ( $polyTime ) {$polyTime = 2;};
    print "TZTYPE = $tztype\n";
if ( !$tztype ) {$tztype = "poly";};
if ( !$debug ) {$debug = 0;};
if ( !$oges_debug ) {$oges_debug = 0;};
if ( !$cfl ) {$cfl = .9;};
if ( !$av2 ) {$av2=0.;};
if ( !$av4 ) {$av4=0.;};
if ( !$f_x ) {$f_x = 1;};
if ( !$f_y ) {$f_y = $f_x;};
if ( !$f_z ) {$f_z = $f_x;};
if ( !$bc )  {$bc = "periodic";};
if ( !$ifac ) {$ifac = 1.;};
if ( $strick eq "" ) { $strick = 1./6.; };
$pdeName = getPDEString($pde);
$gridName= formGridName($baseGrid,$res,$bc);
$tzName = getTwilightZoneString($tztype);
$pdeOptions = getPDEOptions($pde);
$bcSpec = "boundary conditions\n";
if ($bc ne "periodic") { $bcSpec = $bcSpec."all=$bc\n";};
$bcSpec = $bcSpec."done\n";
*
*
$gridName
$pdeName
exit
debug
$debug
Oges::debug (od=)
$oges_debug
turn on twilight zone
final time $tmax
no plotting
times to plot $tmax
$pdeOptions
$bcSpec
implicit factor
$ifac
cfl
$cfl
OBPDE:av2,av4 $av2,$av4
OBPDE:scoeff $strick
twilight zone options...
  OBTZ:degree in space $polySpace
  OBTZ:degree in time $polyTime
  OBTZ:frequencies (x,y,z,t) $f_x,$f_y,$f_z,$f_t
  $tzName
pause
continue
*max iterations $maxit
*plot iterations $maxit
pause
movie mode
finish
