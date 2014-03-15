* 
* Get commonly used command line options
* 
if( $cfl eq "" ){ $cfl=.9; }
if( $coupled eq "" ){ $coupled=1; }
if( $debug eq "" ){ $debug=0; }
if( $dtMax eq "" ){ $dtMax=.2; }
if( $gravity eq "" ){ $gravity="0. 0. 0."; }
if( $go eq "" ){ $go="halt"; }
if( $idebug eq "" ){ $idebug=0; }
if( $implicitFactor eq "" ){ $implicitFactor=.5; }
if( $implicitVariation eq "" ){ $implicitVariation="viscous"; }
if( $iTol eq "" ){ $iTol=1.e-3; }
if( $kThermal eq "" ){ $kThermal=.1/.72; }
if( $ktcFluid eq "" ){ $ktcFluid=.1; }
if( $kappa eq "" ){ $kappa=.1; }
if( $ktcSolid eq "" ){ $ktcSolid=.1; }
if( $mixedInterface eq "" ){ $mixedInterface=0; }
if( $mu eq "" ){ $mu=.1; }
if( $nu eq "" ){ $nu=.1; }
if( $numberOfCorrections eq "" ){ $numberOfCorrections=1; }
if( $iOmega eq "" ){ $iOmega=1.; }
if( $pdebug eq "" ){ $pdebug=0; }
if( $prandtl eq "" ){ $prandtl=.72; }
if( $refactorFrequency eq "" ){ $refactorFrequency=100; }
if( $rtolp eq "" ){ $rtolp=1.e-6; }if( $atolp eq "" ){ $atolp=1.e-7; }
if( $rtoli eq "" ){ $rtoli=1.e-6; }if( $atoli eq "" ){ $atoli=1.e-7; }
if( $show eq "" ) { $show = " "; }
if( $solver eq "" ){ $solver="yale"; }
if( $thermalExpansivity eq "" ){ $thermalExpansivity=1.; }
if( $tFinal eq "" ){ $tFinal=10.; }
if( $tPlot eq "" ){ $tPlot=.1; }
if( $ts eq "" ){ $ts="pc"; }
if( $tz eq "" ){ $tz="none"; }
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"tp=f"=>\$tPlot,"solver=s"=>\$solver,"tz=s"=>\$tz,\
 "show=s"=>\$show,"ts=s"=>\$ts,"go=s"=>\$go,"debug=i"=>\$debug,"nc=i"=> \$numberOfCorrections,"iOmega=f"=>\$iOmega,\
 "iTol=f"=>\$iTol,"noplot=s"=>\$noplot,"coupled=i"=>\$coupled,"dtMax=f"=>\$dtMax,"nu=f"=>\$nu,"kappa=f"=>\$kappa,\
 "ktcFluid=f"=>\$ktcFluid,"kThermal=f"=>\$kThermal,"ktcSolid=f"=>\$ktcSolid,"iv=s"=>\$implicitVariation,\
 "imp=f"=>\$implicitFactor,"mixedInterface=i"=>\$mixedInterface,"cfl=f"=>\$cfl,"prandtl=f"=>\$prandtl,"mu=f"=>\$mu,\
 "thermalExpansivity=f"=>\$thermalExpansivity,"idebug=i"=>\$idebug,"pdebug=i"=>\$pdebug,"gravity=s"=>\$gravity,\
 "rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"rtoli=f"=>\$rtoli,"atoli=f"=>\$atoli,"rf=i"=> \$refactorFrequency);
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $ts eq "fe" ){ $ts="forward Euler";  $tsd="forward Euler"; }
if( $ts eq "be" ){ $ts="backward Euler"; $tsd="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit";       $tsd="implicit";  }
if( $ts eq "pc" ){ $ts="adams PC";       $tsd="adams PC";  }
if( $ts eq "mid"){ $ts="midpoint";       $tsd="forward Euler"; }  
if( $ts eq "ss" ){ $ts = "steady state RK-line"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $uMin=-1; $uMax=1.; }
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "useNewImplicitMethod\n implicitFullLinearized"; }\
else{ $implicitVariation = "useNewImplicitMethod\n implicitFullLinearized"; }
