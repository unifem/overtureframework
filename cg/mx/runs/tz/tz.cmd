#
# cgmx -- test the twilightzone
#
# Usage:
#   
#  cgmx [-noplot] tz -g=<name> -tz=<poly/trig> -degreex=<> -degreet=<> -tf=<tFinal> -tp=<tPlot> ...
#                    -bcn=[pec|d|s] -diss=<> -order=<2/4> -debug=<num> -bg=<backGround> -cons=[0/1] ...
#                    -method=[nfdtd|Yee|sosup] -dm=[none|gdm] -go=[run/halt/og]
# 
#  -dm : dispersion model
#  -diss : coeff of artificial diffusion 
#  -bcn : d=dirichlet, pec=perfect electrical conductor, s=symmetry
#  -go : run, halt, og=open graphics
#  -cons : 1= conservative difference 
#
# Examples: (see memo file for more examples)
# 
# dirichlet BC: 
#  cgmx -noplot tz -g=square10.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go         [exact]
#  cgmx -noplot tz -g=square16.order4 -degreex=4 -degreet=4 -diss=0. -tf=.5 -go=go      [exact]
#  cgmx -noplot tz -g=nonSquare10.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go      [exact]
#  cgmx -noplot tz -g=rotatedSquare10.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go  [exact]
#  cgmx -noplot tz -g=nonSquare10.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go      [exact]
#  cgmx -noplot tz -g=sise2.order2.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go     [exact]
#  cgmx -noplot tz -g=rsis2.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go            [exact]
# 
#  cgmx tz -g=cice1.order2.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=halt
#  cgmx tz noplot -g=cice1.order2.hdf -degreex=2 -degreet=2 -diss=0.1 -tf=.5 -tz=trig -go=go
#  cgmx tz noplot -g=cice2.order2.hdf -degreex=2 -degreet=2 -diss=0.1 -tf=.5 -tz=trig -go=go
#   -- 3d --
#  cgmx tz noplot -g=box10.hdf -degreex=2 -degreet=2 -diss=0. -tf=1. -go=go            [exact]
#  cgmx tz noplot -g=nonBox5.hdf -degreex=2 -degreet=2 -diss=0. -tf=1. -go=go          [exact]
#  cgmx tz noplot -g=rotatedBox10.hdf -degreex=2 -degreet=2 -diss=0. -tf=1. -go=go     [exact]
#  cgmx tz noplot -g=bib2e.hdf -degreex=2 -degreet=2 -diss=0. -tf=1. -go=go            [exact]
#  cgmx tz noplot -g=rbibe2.order2.hdf -degreex=2 -degreet=2 -diss=0. -tf=1. -go=go    [exact]
# 
# -- Yee scheme
#  cgmx -noplot tz -g=square10 -degreex=2 -degreet=2 -method=Yee -tf=.5 -tp=.1 -bc=d -go=go [exact]
#  cgmx -noplot tz -g=box5 -degreex=2 -degreet=2 -method=Yee -tf=.5 -tp=.1 -bc=d -go=go [exact]
#  mpirun -np 1 $cgmxp tz -g=square10 -degreex=2 -degreet=2 -method=Yee -tf=.5 -tp=.1 -bc=d -go=halt
#  mpirun -np 1 $cgmxp tz -g=box10 -degreex=2 -degreet=2 -method=Yee -tf=.5 -tp=.1 -bc=d -go=halt
#  -- TZ material variable coeff
#   cgmx -noplot tz -g=square5 -degreex=1 -degreet=2 -method=Yee -tf=.5 -tp=.1 -bc=d -useTZmaterials=1 -go=og  [exact]
#   cgmx -noplot tz -g=box5 -degreex=1 -degreet=2 -method=Yee -tf=.5 -tp=.1 -bc=d -useTZmaterials=1 -go=og  [exact]
# 
# -- sosup
#  cgmx -noplot tz -g=square20 -degreex=1 -degreet=1 -method=sosup -tf=.5 -tp=.1 -bc=d -go=go [exact]
#  cgmx -noplot tz -g=square20.order4.ng3 -degreex=3 -degreet=3 -method=sosup -tf=.5 -tp=.1 -bc=d -go=go [exact]
#  cgmx -noplot tz -g=square40.order6.ng4 -degreex=5 -degreet=5 -method=sosup -tf=.5 -tp=.1 -bc=d -go=go [almost exact 
#
# -- symmetry bc
#  cgmx -noplot tz -g=square10        -degreex=2 -degreet=2 -diss=0. -tf=.5 -bcn=s -go=go    [exact]
#  cgmx -noplot tz -g=nonSquare10     -degreex=2 -degreet=2 -diss=0. -tf=.5 -bcn=s -go=go    [exact]  
#  cgmx -noplot tz -g=rotatedSquare10 -degreex=2 -degreet=2 -diss=0. -tf=.5 -bcn=s -go=go    [exact]
#  cgmx -noplot tz -g=square16.order4 -degreex=4 -degreet=4 -diss=0. -tf=.5 -bcn=s -go=go    [exact]
#  cgmx -noplot tz -g=rotatedSquare8.order4 -degreex=4 -degreet=4 -diss=0. -tf=.5 -bcn=s -go=go    [exact]
#
#  cgmx -noplot tz -g=box10.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -bcn=s -go=go             [exact]
#  cgmx -noplot tz -g=rotatedBox10 -degreex=2 -degreet=2 -diss=0. -tf=.5 -bcn=s -go=go          [exact]
#  cgmx -noplot tz -g=rotatedBox16.order4 -degreex=4 -degreet=4 -diss=0. -tf=.5 -bcn=s -go=go   [exact]
# 
# parallel: 
#  mpirun -np 1 $cgmxp noplot tz.cmd -g=square10.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go
#  mpirun -np 1 $cgmxp noplot tz.cmd -g=square5.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go -debug=15
#
#  mpirun -np 2 $cgmxp noplot tz -g=sise.hdf -degreex=2 -degreet=2 -diss=0. -tf=.1 -go=go -debug=7
#  totalview srun -a -N1 -n2 -ppdebug $cgmxp noplot tz -g=sise.hdf -degreex=2 -degreet=2 -diss=0. -tf=.1 -go=go -debug=7
# 
#  mpirun-wdh -np 2 $cgmxp noplot tz.cmd -g=square10.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go 
#  srun -N1 -n1 -ppdebug $cgmxp noplot tz.cmd -g=square10.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=halt 
#  totalview srun -a -N1 -n4 -ppdebug $cgmxp noplot tz.cmd -g=square10.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go
#  srun -ppdebug -N2 -n2 memcheck_all $cgmxp noplot tz.cmd -g=square10.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 
# 
#  srun -N1 -n1 -ppdebug $cgmxp noplot tz -g=sise2.order2.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go
#  srun -N1 -n4 -ppdebug $cgmxp noplot tz -g=sise2.order4.hdf -degreex=4 -degreet=4 -diss=0. -tf=.5 -go=go [exact]
#  mpirun-wdh -np 4 $cgmxp noplot tz -g=nonBox8.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go
#  mpirun-wdh -np 1 $cgmxp noplot tz -g=rbibe1.order2.hdf -degreex=2 -degreet=2 -diss=0. -tf=.1 -go=go
#  totalview srun -a -N1 -n2 -ppdebug $cgmxp noplot tz -g=rbibe1.order2.hdf -degreex=2 -degreet=2 -diss=0. -tf=.1 -go=go
#  mpirun-wdh -np 4 $cgmxp noplot tz -g=rbibe2.order2.hdf -degreex=2 -degreet=2 -diss=0. -tf=.5 -go=go
# 
# --- set default values for parameters ---
# 
$noplot=""; $backGround="square"; $grid="square10"; $mu=1.; $lambda=1.;$method="NFDTD"; 
$debug = 0;  $tPlot=.1; $diss=.1; $dissOrder=2; $bcn="pec"; $cons=0; $dm="none"; $domain="all"; 
$tz = "poly"; $degreex=2; $degreet=2; $fx=.5; $fy=$fx; $fz=$fx; $ft=$fx; $useTZmaterials=0;
$order = 2; $go="run"; $useSosupDissipation=0; $sosupParameter=1.; $sosupDissipationOption=0; 
$tFinal=1.; $cfl=.9; 
# GDM parameters
$npv=1; $alphaP=1.; $modeGDM=-1; 
@a0 = (); @a1=(); @b0=(); @b1=(); # these must be null for GetOptions to work, defaults are given below
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"diss=f"=>\$diss,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug,"dissOrder=i"=>\$dissOrder, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,\
  "useSosupDissipation=i"=>\$useSosupDissipation,"sosupParameter=f"=>\$sosupParameter,\
  "sosupDissipationOption=i"=>\$sosupDissipationOption,"domain=s"=>\$domain,\
  "mu=f"=>\$mu,"lambda=f"=>\$lambda,"dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"method=s"=>\$method,\
  "useTZmaterials=i"=>\$useTZmaterials,"fx=f"=>\$fx,"fy=f"=>\$fy,"fz=f"=>\$fz,"ft=f"=>\$ft,"dm=s"=>\$dm,\
  "alphaP=f"=>\$alphaP,"a0=f{1,}"=>\@a0,"a1=f{1,}"=>\@a1,"b0=f{1,}"=>\@b0,"b1=f{1,}"=>\@b1,"npv=i"=>\$npv );
# -------------------------------------------------------------------------------------------------
# Give defaults here for array arguments: 
if( $a0[0] eq "" ){ @a0=(1,0,0,0); }
if( $a1[0] eq "" ){ @a1=(0,0,0,0); }
if( $b0[0] eq "" ){ @b0=(0,0,0,0); }
if( $b1[0] eq "" ){ @b1=(0,0,0,0); }
printf(" a0[0]=%f, a0[1]=%f\n",$a0[0],$a0[1]);
printf(" b1[0]=%f, b1[1]=%f\n",$b1[0],$b1[1]);
# 
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="polynomial"; }else{ $tz="trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
# 
if( $bcn eq "d" ){ $bcn = "bc: all=dirichlet"; }
if( $bcn eq "pec" ){ $bcn = "bc: all=perfectElectricalConductor"; }
if( $bcn eq "s" ){ $bcn = "bc: all=symmetry"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "go" ){ $go = "movie mode\n finish"; }
#
if( $dm eq "none" ){ $dm="no dispersion"; }
if( $dm eq"drude" || $dm eq "Drude" ){ $dm="Drude"; }
if( $dm eq"gdm" ){ $dm="GDM"; }
#
$grid
# 
$method
# dispersion model:
$dm
# 
# Drude params 1 1 all (gamma,omegap,domain-name)
$cmd="#"; 
if( $npv == 1 ){ $cmd = "GDM params $a0[0] $a1[0] $b0[0] $b1[0] all (a0,a1,b0,b1,domain-name)"; }
if( $npv == 2 ){ \
   $cmd  = "GDM domain name: $domain\n"; \
   $cmd .= " number of polarization vectors: $npv\n"; \
   $cmd .= " GDM coeff: 0 $a0[0] $a1[0] $b0[0] $b1[0] (eqn, a0,a1,b0,b1)\n"; \
   $cmd .= " GDM coeff: 1 $a0[1] $a1[1] $b0[1] $b1[1] (eqn, a0,a1,b0,b1)"; \
      }
$cmd
#
#
#* twilightZoneInitialCondition
twilightZone
$tz
degreeSpace, degreeTime $degreex $degreet 
TZ omega: $fx $fy $fz $ft (fx,fy,fz,ft)
# 
use twilightZone materials $useTZmaterials
# 
tPlot $tPlot
tFinal $tFinal
dissipation $diss
order of dissipation $dissOrder
#
use sosup dissipation $useSosupDissipation
sosup parameter $sosupParameter
sosup dissipation option $sosupDissipationOption
#
cfl $cfl
use conservative difference $cons
#
$bcn
#
debug $debug
check errors 1
plot errors 1
continue
$go

