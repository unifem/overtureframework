#!/usr/bin/env tcsh

set debug = 1
set N = 40 
set g = ogen
#set g = rbox.hdf
#set g = rsquare.hdf
set g="ogen";#t2/cice4.order4.s4.ml4.hdf; set gf=""
set gfac = 2
set gridtomove=""
#set gf = $Overture/sampleGrids/square.cmd; set gfac=2; set N=21; set gridtomove="square"; #40
#set gf = $Overture/sampleGrids/box.cmd; set N=21
#set gf = $Overture/sampleGrids/smoothedPoly.cmd; set gfac=3
#set gf = $Overture/sampleGrids/annulus.cmd; set N=41; set gfac=2
#set gf = $Overture/sampleGrids/smoothedPoly.cmd; set gfac=2
#set gf = $Overture/sampleGrids/annulus.cmd; set N=41; set gridtomove="Annulus";
#set gf = $Overture/sampleGrids/cylinder.cmd; set gfac=8
set gf = $Overture/sampleGrids/mycicArg.cmd; set N=41; set gfac=16; set gridtomove="Annulus"
#set gf = $Overture/sampleGrids/mysisArg.cmd; set N=41; set gfac=8; set gridtomove="inner-square";

set degx = 1
set degt = 0
set tz = trig
#set tz = poly
set sbc = "dirichlet" 
set bbc = "d" 
set fx = 1
set fy = 1
set fz = $fy
set ft = 1
set adc = 1
set nu = 1e-1
set bcn = "all=dirichlet" #"all=noSlipWall" ##"all=neumannBoundaryCondition""all=dirichlet"
set tf = 10 #0.5 #5000 #10
set go = #run
set tp =  1.5625e-3;
set noplot = #noplot
set ts=afs
set order=4
set grep = 'grep -E "^   0\.100|^   5\.000"'
set valg=""
#set valg = 'valgrind --db-attach=yes'; set noplot = "noplot"
#set valg = 'valgrind --dsymutil=yes'; set noplot = "noplot"
set do = "compact"
set model = "ins" #"boussinesq" #"ins"
set kThermal = 0.01
set thermalExpansivity=0.1
set psolver = "best" #"mg"
set ad4 = 1
set ad2 = 0
set afit = 20
set aftol=1e-8
set parallel = #'/usr/apps/mpich2/latest/bin/mpirun -n 4'
set totalview = ; set tvarg = ;#set totalview=totalview; set tvarg = -a;
set cgins = $CGBUILDPREFIX/ins/bin/cgins
set move = #"shift"#"rotate" #"shift"
set uplot = "u (error)";

$totalview $parallel $valg $cgins $tvarg $CG/ins/cmd/tz -ml=2 -numberOfParallelGhost=3 -move=$move -gridToMove=$gridtomove -rate=.25 -ad2=$ad2 -ad4=$ad4 -interp=e --grid_nge=1 --grid_factor=$gfac --grid_order=$order --cicorder=$order --boxorder=$order --tzorder=$order -g=$g -gf=$gf --N=$N --nx=$N -degreex=$degx -degreet=$degt  -ts=$ts -tz=$tz -newts=1 -debug=$debug --boxbc=$bbc --squarebc=$sbc -bcn=$bcn -fx=$fx -fy=$fy -fz=$fz -ft=$ft -advectionCoefficient=$adc -nu=$nu -tf=$tf -go=$go -tp=$tp -do=$do $noplot -psolver=$psolver -model=$model -kThermal=$kThermal -thermalExpansivity=$thermalExpansivity -rgd=fixed -rtolp=1.e-10 -aftol=$aftol -afit=$afit -uplot="$uplot" | tee out
echo cgins $CG/ins/cmd/tz --grid_factor=$gfac --boxorder=$order -grid_order=$order -order=$order -g=$g -gf=$gf --N=$N --nx=$N -degreex=$degx -degreet=$degt  -ts=$ts -tz=$tz -newts=1 -debug=$debug --boxbc=$bbc --squarebc=$sbc -bcn=\# -fx=$fx -fy=$fy -fz=$fz -ft=$ft -advectionCoefficient=$adc -nu=$nu -tf=$tf -go=$go -tp=$tp -do=$do $noplot   -psolver=$psolver -model=$model -kThermal=$kThermal -thermalExpansivity=$thermalExpansivity 

