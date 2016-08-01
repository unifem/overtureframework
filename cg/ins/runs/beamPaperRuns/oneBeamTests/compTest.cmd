$solution=100;
$assumeExactSolution=0;
$go="halt";
$option=0;
$numGrids=4; #by default we have 4 grids to do convergence test
$startGrid=0;
* ----------------------------- get command line arguments ---------------------------------------
GetOptions("solution=i"=>\$solution,"option=i"=>\$option,\
	   "assumeExactSolution=i"=>\$assumeExactSolution,\
	   "go=s"=>\$go,"numGrids=i"=>\$numGrids,"startGrid=i"=>\$startGrid);
* -------------------------------------------------------------------------------------------------
@results=(\
"rhos10_E10_Re10_vNBS_vNIS_AMP_FD1_ABAM_Fixed_G",\
"rhos10_E10_Re10_vNBS_vNIS_AMP_FD1_ABAM_Fixed_noRecomputeGVOnCorrection_G",\
"rhos10_E10_Re10_vNBS_vNIS_AMP_FD1_ABAM_G",\
"rhos10_E10_Re10_vNBS_vNIS_AMP_FD1_ABAM_noRecomputeGVOnCorrection_G");
#
#
$cmd="";
$numResults=@results;
print("numResults=$numResults\n");
if($option >= $numResults || $option < 0){\
print("\n\n\n************************ERROR*******************************\n");\
print("option $option does not exist. Input option between [0,$#results] to do comp for the results:\n");\
foreach(@results){print("$_\n");}\
print("************************************************************\n");\
$cmd=' ';}\
else{$endGrid=$startGrid+$numGrids;\
for($i=$startGrid;$i<$endGrid;$i++){$g=2**$i;$cmd.=$results[$option]."$g/".$results[$option]."$g.show\n";}}
specify files (coarse to fine)
printf("$cmd\n");
$cmd
exit
choose a solution
    $solution
assume fine grid holds exact solution $assumeExactSolution
compute errors
$cmd="#";
if($go eq "go"){$cmd="exit";}
$cmd
    


