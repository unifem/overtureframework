$solution=100;
$assumeExactSolution=0;
$go="halt";
$option=0;
$numGrids=4; #by default we have 4 grids to do convergence test
* ----------------------------- get command line arguments ---------------------------------------
GetOptions("solution=i"=>\$solution,"option=i"=>\$option,\
	   "assumeExactSolution=i"=>\$assumeExactSolution,\
	   "go=s"=>\$go,"numGrids=i"=>\$numGrids);
* -------------------------------------------------------------------------------------------------
@results=(\
"rhos10_E10_Re10_cfls3_Longer_AMP_FEM_noBSmoother_NBNB_G",\
"rhos10_E10_Re10_cfls3_Longer_NBS2_AMP_FD1_NBNB_vNBS_G",\
"rhos10_E10_Re10_Longer_NBS2_AMP_FD1_ABAM_G",\
"rhos10_E10_Re10_newGrid_Longer_NBS2_AMP_FD1_ABAM_G",\
"rhos10_E10_Re10_Longer_NBS2_AMP_FD1_ABAM_G",\
"rhos10_E10_Re10_cfls2_newGrids_Longer_NBS2_AMP_FD1_NBNB_vNBS_G",\
"rhos10_E10_Re10_cfls2_Longer_AMP_FD1_NBNB_vNBS_fixedWidth_G");
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
else{for($i=0;$i<$numGrids;$i++){$g=2**$i;$cmd.=$results[$option]."$g/".$results[$option]."$g.show\n";}}
specify files (coarse to fine)
printf("$cmd\n");
$cmd
exit
choose a solution
    $solution
$cmd="do not assume fine grid holds exact solution";   
if($assumeExactSolution eq 1){$cmd="assume fine grid holds exact solution";} 
printf("$cmd\n"); 
$cmd
compute errors
$cmd="#";
if($go eq "go"){$cmd="exit";}
$cmd
    

