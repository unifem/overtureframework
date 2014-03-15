#!/usr/bin/env tcsh

set is = (1 2 3 4 5)
#set Ns = (11 21 41 81 161)
set Dts= (.1 .05 .025 .0125)
#set Dts= (.001 .001 .001 .001)
set Dts= (.0001 .00005 .000025 .0000125)
#set Dts= (.01 .005 .0025 .00125)
#set Dts= (.01 .01 .01 .01)
set Dts= (.1 .1 .1 .1 .1);set Ns = (21 41 81 161 321)
#set Dts= (.1 .01 .0001 ); set is = (1 2 3); set Ns = (21 41 81)
#set Dts= (.1 .00625 3.90625e-4 ); set is = (1 2 3)
#set is = (2 3 4)
#set Dts = (.1 .1 .05 .025)
set gfac = (1 2 4 8 16)

#set N = 160
set degx = 0
set degt = 1
set tz = trig
#set tz = poly
set sbc = "dirichlet" #"periodic"
set bbc = "d" #"p" #"d"
set fx = 1
set fy = 1
set fz = $fy
set ft = 1
set adc = 1
set nu = 1e-1
set tf = 1.0 #.005 #10
set go = run
set debug = 0
#set tp = 
set noplot = #noplot
set ts=afs
set order=2; set ad2=1; set ad4=0
set order=4; set ad4=1; set ad2=0
set grep = 'grep -E "^   0\.100|^   0\.500"'
set cfl = 1e0
set do="compact"
set psolver = "best" #"yale"
set g = ogen
set gridtomove2 = "#";
#set gf = $Overture/sampleGrids/square.cmd; set is = (1 2 3 4 5); set Dts= (.1 .1 .1 .1 .1);set Ns = (21 41 81 161 321); set tf=0.1;set gridtomove="square";
set gf = $Overture/sampleGrids/square.cmd; set is = (1 2 3 4); set Dts= (.1 .05 .025 0.0125);set Ns = (21 41 81 161 321); set tf=1;set gridtomove="square";
#set gf = $Overture/sampleGrids/square.cmd; set is = (1 2 3 4); set Dts= (.1 .05 .025 0.0125);set Ns = (21 21 21 21 21); set tf=1;set gridtomove="square";
set gf = $Overture/sampleGrids/square.cmd; set is = (1 2 3 4); set Dts= (0.1 0.025 0.00625 1.5625e-3);set Ns = (21 41 81 161 321); set tf=1.0;set gridtomove="square";
#set gf = $Overture/sampleGrids/square.cmd; set is = (1 2 3 4 5); set Dts= (.01 .01 .01 .01 .01);set Ns = (21 41 81 161 321);set gridtomove="square";
#set gf = $Overture/sampleGrids/box.cmd; set gridtomove="box";
#set gf = $Overture/sampleGrids/annulus.cmd; set Ns = (41 81 161 321); set Dts=(0.1 0.05 0.025 0.0125); set is=(1 2 3 4); set gfac=(1 2 3 4 5); set gridtomove="Annulus";
#set gf = $Overture/sampleGrids/annulus.cmd; set Ns = (41 81 161 321); set Dts=(0.1 0.025 0.00625 1.5625e-3); set is=(1 2 3 4); set gfac=(1 2 3 4 5); set gridtomove="Annulus";
#set gf = $Overture/sampleGrids/annulus.cmd; set Ns = (41 81 161 321); set Dts=(0.1 0.05 0.025 0.0125); set is=(1 2 3 4); set gfac=(1 2 3 4 5); set gridtomove="Annulus";
#set gf = $Overture/sampleGrids/annulus.cmd; set Ns = (41 81 161 321 641); set Dts=(0.1 0.1 0.1 0.1 0.1); set is=(1 2 3 4 5); set gfac=(1 2 4 8 16)
#set gf = $Overture/sampleGrids/smoothedPoly.cmd; set Dts=(0.01 0.005 .0025 ); set is = (1 2 3); set gfac = (4 8 16); set tf=.05; set Ns=(81 161 321 641);
set gf = $Overture/sampleGrids/cylinder.cmd; set Dts=(0.1 0.025 0.00625 1.5625e-3); set is=(1 2 3 4); set gfac=(1 2 4 8); set tf=1.0; 
#set gf = $Overture/sampleGrids/smoothedPoly.cmd; set Dts=(0.01 0.0025 0.000625 1.5625e-4); set is = (1 2 3 4); set gfac = ( 1 2 4 8); set tf=.05
#set gf = $Overture/sampleGrids/smoothedPoly.cmd; set Dts=(0.1 0.025 .00625 1.5625e-3); set is = (1 2 3 4); set gfac = ( 2 4 8 16); set tf=.5
#set gf = $Overture/sampleGrids/cylinder.cmd; set Dts=(0.01 .01 .01 .01); set is=(1 2 3 4); set gfac=(1 2 4 8); set tf=.05
#set gf = $Overture/sampleGrids/smoothedPoly.cmd; set Dts=(.1 0.1 0.1 0.1); set is = (1 2 3 4); set gfac = ( 2 4 8 16); set tf=.005
#set gf = $Overture/sampleGrids/cicArg.cmd; set Ns = (81 161 321 641); set Dts=(0.1 0.1 0.1 0.1); set is=(1 2 3 4); set gfac=(2 4 8 16); set tf = .5; set ft=.5 #set psolver=yale
#set gf = $Overture/sampleGrids/mycicArg.cmd; set Ns = (81 161 321 641); set Dts=(0.1 0.025 0.00625 1.5625e-3); set is=(1 2 3 4); set gfac=(2 4 8 16); set tf=1.0; set gridtomove="Annulus"; #set psolver=yale
#set gf = $Overture/sampleGrids/mycicArg.cmd; set Ns = (21 41 81 161 ); set Dts=(0.1 0.025 0.00625 1.5625e-3); set is=(1 2 3 4); set gfac=(2 4 8 16); set tf=.5; set gridtomove="Annulus"; #set is=(1 ); #set psolver=yale
#set gf = $Overture/sampleGrids/mycicArg.cmd; set Ns = (21 41 81 161 ); set Dts=(0.1 0.05 0.025 0.0125); set is=(1 2 3 4); set gfac=(2 4 8 16); set tf=.5; set gridtomove="Annulus"; #set psolver=yale
##set gf = $Overture/sampleGrids/mycicArg.cmd; set Ns = (21 41 81 161 ); set Dts=(0.01 0.005 0.0025 0.00125); set is=(1 2 3 4); set gfac=(2 4 8 16); set tf=.05; set gridtomove="Annulus"; set psolver=yale
#set gf = $Overture/sampleGrids/cicArg.cmd; set Ns = (81 161 321 641); set Dts=(0.1 0.05 0.025 0.0125); set is=(1 2 3 4); set gfac=(2 4 8 16); set tf=.5; set gridtomove="Annulus"; #set psolver=yale
#set gf = $Overture/sampleGrids/mycicArg.cmd; set Ns = (81 161 321 641); set Dts=(0.01 0.0025 0.000625 1.5625e-4 ); set is=(1 2 3 4); set gfac=(2 4 8 16); set tf = 0.05;; set gridtomove="Annulus"; ##set psolver=yale
#set gf = $Overture/sampleGrids/mysisArg.cmd; set is=(1 2 3 4); set gfac=(2 4 8 16); set tf=0.5; set Dts=(0.1 0.05 0.025 0.0125); set gridtomove="inner-square";
#set gf = $Overture/sampleGrids/mysisArg.cmd; set is=(1 2 3 4); set gfac=(2 4 8 16); set tf=0.05; set Dts=(0.01 0.005 0.0025 0.00125); set gridtomove="inner-square";
#set gf = $Overture/sampleGrids/mysisArg.cmd; set is=(1 2 3 4); set gfac=(2 4 8 16); set tf=1.0; set Dts=(0.1 0.025 0.00625 1.5625e-3); set gridtomove="inner-square";

#set gf = mysibArg.cmd; set is = (1 2 3 4); set gfac = (1 2 4 8); set tf=1.0; set Dts=(0.1 0.025 0.00625 1.5625e-3); set gridtomove='north-pole'; set gridtomove2 = 'south-pole';

set uplot="u (error)"
set model="ins"; #"boussinesq" #"ins"
set kThermal = .01
set thermalExpansivity=0.1
set move = #"shift" #"rotate"
set bcn="all=dirichlet" #"all=noSlipWall"
rm convergence.out
touch convergence.out
foreach i ($is)
#foreach i (4)
set N=$Ns[$i]
set tp=$Dts[$i]
set gfa=$gfac[$i]
echo "running N=$N, dt=$tp"

cgins $CG/ins/cmd/tz   -move=$move -gridToMove=$gridtomove -gridToMove2=$gridtomove2  -rate=.125 -ad2=$ad2 -ad4=$ad4  -rgd=fixed  -interp=e --grid_nge=1 --grid_factor=$gfa --boxorder=$order --grid_order=$order -cicorder=$order -tzorder=$order -g=$g -gf=$gf --N=$N -degreex=$degx -degreet=$degt  -ts=$ts -tz=$tz -newts=1 -debug=$debug --boxbc=$bbc --squarebc=$sbc -bcn=$bcn -fx=$fx -fy=$fy -fz=$fz  -ft=$ft -advectionCoefficient=$adc -nu=$nu -tf=$tf -go=$go -tp=$tp $noplot  -cfl=$cfl -dtMax=$tp -do=$do -psolver=$psolver -model=$model -kThermal=$kThermal -thermalExpansivity=$thermalExpansivity  -aftol=1e-10 -afit=40 -uplot="$uplot" |grep -E "^   0\.100|^   0\.250|^   1\.0|total number of grid" | tee -a convergence.out

#cgins $CG/ins/cmd/tz  -move=$move -gridToMove=$gridtomove -rate=.125 -ad2=$ad2 -ad4=$ad4  -rgd=fixed  -interp=e --grid_nge=1 --grid_factor=$gfa --boxorder=$order --grid_order=$order -cicorder=$order -tzorder=$order -g=$g -gf=$gf --N=$N -degreex=$degx -degreet=$degt  -ts=$ts -tz=$tz -newts=1 -debug=$debug --boxbc=$bbc --squarebc=$sbc -bcn=$bcn -fx=$fx -fy=$fy -fz=$fz  -ft=$ft -advectionCoefficient=$adc -nu=$nu -tf=$tf -go=$go -tp=$tp $noplot  -cfl=$cfl -dtMax=$tp -do=$do -psolver=$psolver -model=$model -kThermal=$kThermal -thermalExpansivity=$thermalExpansivity  -aftol=1e-10 -afit=40 -uplot="$uplot" |grep -E "^   0\.010|^   0\.05|total number of grid" | tee -a convergence.out
#| grep -E "^   0.05|total number of grid" 


#echo tz --grid_factor=$gfa --boxorder=6 --boxorder=6 --grid_order=4 -order=$order -g=$g -gf=$gf --N=$N -degreex=$degx -degreet=$degt  -ts=$ts -tz=$tz -newts=1 -debug=$debug --boxbc=$bbc --squarebc=$sbc -bcn="\#" -fx=$fx -fy=$fy  -fz=$fz  -ft=$ft -advectionCoefficient=$adc -nu=$nu -tf=$tf -go=$go -tp=$tp $noplot -cfl=$cfl -dtMax=$tp -do=$do -model=$model -kThermal=$kThermal -thermalExpansivity=$thermalExpansivity -aftol=1e-2 -afit=5
end

